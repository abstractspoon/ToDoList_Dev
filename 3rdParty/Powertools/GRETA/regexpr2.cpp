//+---------------------------------------------------------------------------
//
//  Copyright ( C ) Microsoft, 1994 - 2002.
//
//  File:       regexpr2.cpp
//
//  Contents:   implementation for rpattern methods, definitions for all the
//              subexpression types used to perform the matching, the
//              charset class definition .
//
//  Classes:    too many to list here
//
//  Functions:
//
//  Author:     Eric Niebler ( ericne@microsoft.com )
//
//  History:    12-11-1998   ericne   Created
//              01-05-2001   ericne   Removed dependency on VC's choice
//                                    of STL iterator types.
//              08-15-2001   ericne   Removed regexpr class, moved match
//                                    state to match_results container.
//              09-17-2001   nathann  Add DEBUG_HEAP_SUPPORT
//              11-16-2001   ericne   Add stack-conservative algorithm
//
//----------------------------------------------------------------------------

#ifdef _MSC_VER
// unlimited inline expansion ( compile with /Ob1 or /Ob2 )
# pragma inline_recursion( on )
# pragma inline_depth( 255 )
// warning C4127: conditional expression is constant
// warning C4355: 'this' : used in base member initializer list
// warning C4702: unreachable code
// warning C4710: function 'blah' not inlined
// warning C4786: identifier was truncated to '255' characters in the debug information
# pragma warning( push )
# pragma warning( disable : 4127 4355 4702 4710 4786 )
#endif

#include <limits>
#include <cctype>
#include <cwchar>
#include <memory>
#include <cwctype>
#include <malloc.h>
#include <algorithm>

#ifdef __MWERKS__
# include <alloca.h>
#endif

// If the implementation file has been included in the header, then we
// need to mark some functions as inline to prevent them from being multiply
// defined.  But if the implementation file is not included in the header,
// we can't mark them as inline, otherwise the linker won't find them.
#ifdef REGEXPR_H
# define REGEXPR_H_INLINE inline
#else
# define REGEXPR_H_INLINE
# include "regexpr2.h"
#endif

#ifdef REGEX_TO_INCLUDE
# include REGEX_TO_INCLUDE
#endif

// $PORT$
// _alloca is not standard
#ifndef alloca
# define alloca _alloca
#endif

namespace regex
{

namespace detail
{

inline wctype_t REGEX_CDECL regex_wctype( char const * sz )
{
    using namespace std;
    return wctype( sz );
}

namespace
{

#ifdef __GLIBC__
struct regex_ctype_t
{
    int      m_ctype;
    wctype_t m_wctype;
};

#define REGEX_DECL_CTYPE(desc)                                                  \
    inline regex_ctype_t const & wct_ ## desc()                                 \
    {                                                                           \
        static regex_ctype_t const s_wct = { _IS ## desc, regex_wctype(#desc) };\
        return s_wct;                                                           \
    }

REGEX_DECL_CTYPE(alnum)
REGEX_DECL_CTYPE(alpha)
REGEX_DECL_CTYPE(blank)
REGEX_DECL_CTYPE(cntrl)
REGEX_DECL_CTYPE(digit)
REGEX_DECL_CTYPE(graph)
REGEX_DECL_CTYPE(lower)
REGEX_DECL_CTYPE(print)
REGEX_DECL_CTYPE(punct)
REGEX_DECL_CTYPE(space)
REGEX_DECL_CTYPE(upper)
REGEX_DECL_CTYPE(xdigit)
regex_ctype_t const wct_zero = { 0,  0 };

inline regex_ctype_t & operator |= ( regex_ctype_t & lhs, regex_ctype_t const & rhs )
{
    lhs.m_ctype  |= rhs.m_ctype;
    lhs.m_wctype |= rhs.m_wctype;
    return lhs;
}
inline regex_ctype_t operator | ( regex_ctype_t lhs, regex_ctype_t const & rhs )
{
    return lhs |= rhs;
}
inline int REGEX_CDECL regex_isctype( int ch, regex_ctype_t const & desc )
{
    return __isctype( ch, desc.m_ctype );
}
inline int REGEX_CDECL regex_iswctype( wint_t wc, regex_ctype_t desc )
{
    using namespace std;
    return iswctype( wc, desc.m_wctype );
}
inline bool operator == ( regex_ctype_t const & lhs, regex_ctype_t const & rhs )
{
    return lhs.m_ctype == rhs.m_ctype && lhs.m_wctype == rhs.m_wctype;
}
inline bool operator != ( regex_ctype_t const & lhs, regex_ctype_t const & rhs )
{
    return lhs.m_ctype != rhs.m_ctype || lhs.m_wctype != rhs.m_wctype;
}
#else
typedef wctype_t regex_ctype_t;

#define REGEX_DECL_CTYPE(desc)                                                   \
    inline regex_ctype_t const wct_ ## desc()                                    \
    {                                                                            \
        static regex_ctype_t const s_wct = regex_wctype(#desc);                  \
        return s_wct;                                                            \
    }

REGEX_DECL_CTYPE(alnum)
REGEX_DECL_CTYPE(alpha)
REGEX_DECL_CTYPE(cntrl)
REGEX_DECL_CTYPE(digit)
REGEX_DECL_CTYPE(graph)
REGEX_DECL_CTYPE(lower)
REGEX_DECL_CTYPE(print)
REGEX_DECL_CTYPE(punct)
REGEX_DECL_CTYPE(space)
REGEX_DECL_CTYPE(upper)
REGEX_DECL_CTYPE(xdigit)
regex_ctype_t const wct_zero    = 0;

#if defined(_MSC_VER) & ( _MSC_VER==1200 | defined(_CPPLIB_VER) )
inline regex_ctype_t const wct_blank() { return _BLANK; } // work around for bug in VC++
inline int REGEX_CDECL regex_isctype( int ch, regex_ctype_t desc )
{
    return _isctype( ch, static_cast<int>( desc ) );
}
#else
REGEX_DECL_CTYPE(blank)
inline int REGEX_CDECL regex_isctype( int ch, regex_ctype_t desc )
{
    using namespace std;
    return iswctype( btowc( ch ), desc );
}
#endif
inline int REGEX_CDECL regex_iswctype( wint_t wc, regex_ctype_t desc )
{
    using namespace std;
    return iswctype( wc, desc );
}
#endif
} // unnamed namespace

template< typename CStringsT, typename IterT >
bool _do_match_iterative( sub_expr_base<IterT> const * expr, match_param<IterT> & param, IterT icur, CStringsT );

// NathanN:
// By defining the symbol REGEX_DEBUG_HEAP the allocator object
// no longer sub allocates memory.  This enables heap checking tools like
// AppVerifier & PageHeap to find errors like buffer overruns
#if !defined( REGEX_DEBUG_HEAP ) & REGEX_DEBUG
# define REGEX_DEBUG_HEAP 1
#else
# define REGEX_DEBUG_HEAP 0
#endif

REGEXPR_H_INLINE size_t DEFAULT_BLOCK_SIZE()
{
#if REGEX_DEBUG_HEAP
    // put each allocation in its own mem_block
    return 1;
#else
    // put multiple allocation in each mem_block
    return 352;
#endif
}

template< typename IBeginT, typename IEndT >
inline size_t parse_int( IBeginT & ibegin, IEndT iend, size_t const max_ = size_t( -1 ) )
{
    typedef typename std::iterator_traits<IEndT>::value_type char_type;
    size_t retval = 0;
    while( iend != ibegin && REGEX_CHAR(char_type,'0') <= *ibegin && REGEX_CHAR(char_type,'9') >= *ibegin && max_ > retval )
    {
        retval *= 10;
        retval += static_cast<size_t>( *ibegin - REGEX_CHAR(char_type,'0') );
        ++ibegin;
    }
    if( max_ < retval )
    {
        retval /= 10;
        --ibegin;
    }
    return retval;
}

// --------------------------------------------------------------------------
//
// Class:       boyer_moore
//
// Description: fast sub-string search algorithm
//
// Members:     m_begin - iter to first char in pattern sequence
//              m_last  - iter to last char in pattern sequence
//              m_len   - length of the pattern sequence
//              m_off   - array of offsets, indexed by ASCII char values
//
// History:     6/8/2003 - ericne - Created
//
// --------------------------------------------------------------------------
template< typename IterT >
class boyer_moore
{
    typedef typename std::iterator_traits<IterT>::value_type    char_type;
    typedef std::char_traits<char_type>                         traits_type;

    IterT               m_begin;
    IterT               m_last;
    char_type const*    m_low_last;
    unsigned char       m_len;
    unsigned char       m_off[ UCHAR_MAX + 1 ];

    static unsigned char hash_char( char ch )           { return static_cast<unsigned char>( ch ); }
    static unsigned char hash_char( signed char ch )    { return static_cast<unsigned char>( ch ); }
    static unsigned char hash_char( unsigned char ch )  { return ch; }
    static unsigned char hash_char( wchar_t ch )        { return static_cast<unsigned char>( ch % 256 ); }
    template< typename  CharT >
    static unsigned char REGEX_VC6(REGEX_CDECL) hash_char( CharT ch REGEX_VC6(...) )
    {
        return static_cast<unsigned char>( std::char_traits<CharT>::to_int_type( ch ) % 256 );
    }

    // case-sensitive Boyer-Moore search
    template< typename OtherT >
    OtherT find_with_case( OtherT begin, OtherT end ) const
    {
        typedef typename std::iterator_traits<OtherT>::difference_type diff_type;
        diff_type const endpos = std::distance( begin, end );
        diff_type offset = m_len;

        for( diff_type curpos = offset; curpos < endpos; curpos += offset )
        {
            std::advance( begin, offset );

            IterT  pat_tmp = m_last;
            OtherT str_tmp = begin;

            for( ; traits_type::eq( *str_tmp, *pat_tmp ); --pat_tmp, --str_tmp )
            {
                if( pat_tmp == m_begin )
                {
                    return str_tmp;
                }
            }

            offset = m_off[ hash_char( *begin ) ];
        }

        return end;
    }

    // case-insensitive Boyer-Moore search
    template< typename OtherT >
    OtherT find_without_case( OtherT begin, OtherT end ) const
    {
        typedef typename std::iterator_traits<OtherT>::difference_type diff_type;
        diff_type const endpos = std::distance( begin, end );
        diff_type offset = m_len;

        for( diff_type curpos = offset; curpos < endpos; curpos += offset )
        {
            std::advance( begin, offset );

            IterT               pat_tmp  = m_last;
            char_type const*    low_tmp  = m_low_last;
            OtherT              str_tmp  = begin;

            for( ; traits_type::eq( *str_tmp, *pat_tmp ) || traits_type::eq( *str_tmp, *low_tmp );
                --pat_tmp, --str_tmp, --low_tmp )
            {
                if( pat_tmp == m_begin )
                {
                    return str_tmp;
                }
            }

            offset = m_off[ hash_char( *begin ) ];
        }

        return end;
    }

public:
    // initialize the Boyer-Moore search data structure, using the
    // search sub-sequence to prime the pump.
    boyer_moore( IterT begin, IterT end, char_type const* lower = 0 )
        : m_begin( begin )
        , m_last( begin )
        , m_low_last( lower )
    {
        typedef typename std::iterator_traits<IterT>::difference_type diff_type;
        diff_type diff = std::distance( begin, end );
        m_len  = static_cast<unsigned char>( regex_min<diff_type>( diff, UCHAR_MAX ) );
        std::fill_n( m_off, ARRAYSIZE( m_off ), m_len );
        --m_len;

        for( unsigned char offset = m_len; offset; --offset, ++m_last )
        {
            m_off[ hash_char( *m_last ) ] = offset;
        }

        if( m_low_last )
        {
            for( unsigned char offset = m_len; offset; --offset, ++m_low_last )
            {
                unsigned char hash = hash_char( *m_low_last );
                m_off[ hash ] = regex_min( m_off[ hash ], offset );
            }
        }
    }

    template< typename OtherT >
    OtherT find( OtherT begin, OtherT end ) const
    {
        if( m_low_last )
        {
            return find_without_case( begin, end );
        }
        else
        {
            return find_with_case( begin, end );
        }
    }

    static void * operator new( size_t size, regex_arena & arena )
    {
        return arena.allocate( size );
    }
    static void operator delete( void *, regex_arena & )
    {
    }
};

// This class is used to speed up character set matching by providing
// a bitset that spans the ASCII range. std::bitset is not used because
// the range-checking slows it down.
// Note: The division and modulus operations are optimized by the compiler
// into bit-shift operations.
class ascii_bitvector
{
    typedef unsigned int elem_type;

    enum
    {
        CBELEM = CHAR_BIT * sizeof( elem_type ), // count of bits per element
        CELEMS = ( UCHAR_MAX+1 ) / CBELEM        // number of element in array
    };
    
    elem_type m_rg[ CELEMS ];

    // Used to inline operations like: bv1 |= ~bv2; without creating temp bit vectors.
    struct not_ascii_bitvector
    {
        ascii_bitvector const & m_ref;
        not_ascii_bitvector( ascii_bitvector const & ref )
            : m_ref( ref ) {}
    private:
        not_ascii_bitvector & operator=( not_ascii_bitvector const & );
    };

    ascii_bitvector( ascii_bitvector const & );
    ascii_bitvector & operator=( ascii_bitvector const & );
public:
    ascii_bitvector()
    {
        zero();
    }
    void zero()
    {
        std::fill_n( m_rg, ARRAYSIZE( m_rg ), 0 );
    }
    void set( unsigned char ch )
    {
        m_rg[ ( ch / CBELEM ) ] |= ( ( elem_type )1U << ( ch % CBELEM ) );
    }
    bool operator[]( unsigned char ch ) const
    {
        return 0 != ( m_rg[ ( ch / CBELEM ) ] & ( ( elem_type )1U << ( ch % CBELEM ) ) );
    }
    not_ascii_bitvector const operator~() const
    {
        return not_ascii_bitvector( *this );
    }
    ascii_bitvector & operator|=( ascii_bitvector const & that )
    {
        for( int i=0; i<CELEMS; ++i )
            m_rg[ i ] |= that.m_rg[ i ];
        return *this;
    }
    ascii_bitvector & operator|=( not_ascii_bitvector const & that )
    {
        for( int i=0; i<CELEMS; ++i )
            m_rg[ i ] |= ~that.m_ref.m_rg[ i ];
        return *this;
    }
};

typedef std::pair<wchar_t, wchar_t> range_type;

// determines if one range is less then another.
// used in binary search of range vector
struct range_less
{
    bool operator()( range_type const & rg1, range_type const & rg2 ) const
    {
        return rg1.second < rg2.first;
    }
};

// A singly-linked list, which works even if the allocator
// has per-instance state.
template< typename T, typename AllocT=std::allocator<T> >
class slist
{
    struct cons
    {
        T      car;
        cons * cdr;

        cons( T const & t, cons * nxt )
            : car( t )
            , cdr( nxt )
        {
        }
    };

    typedef typename rebind<AllocT, cons>::type cons_allocator;
    typedef typename rebind<AllocT, char>::type char_allocator;

#if !defined(_MSC_VER) | 1200 < _MCS_VER
    // Use the empty base optimization to avoid reserving
    // space for the allocator if it is empty.
    struct slist_impl : cons_allocator
    {
        cons * m_lst;

        slist_impl( cons_allocator const & alloc, cons *lst )
            : cons_allocator( alloc )
            , m_lst( lst )
        {
        }
        cons_allocator & allocator()
        {
            return *this;
        }
    };
#else
    struct slist_impl
    {
		cons_allocator	m_alloc;
        cons		   *m_lst;

        slist_impl( cons_allocator const & alloc, cons *lst )
            : m_alloc( alloc )
            , m_lst( lst )
        {
        }
        cons_allocator & allocator()
        {
            return m_alloc;
        }
    };
#endif

    slist_impl m_impl;

    // find the previous node in the list (*prev(lst)==lst)
    cons ** prev( cons *lst, cons *hint = 0 )
    {
        if( m_impl.m_lst == lst )
            return &m_impl.m_lst;
        if( !hint || hint->cdr != lst )
            for( hint=m_impl.m_lst; hint->cdr != lst; hint=hint->cdr )
                {}
        return &hint->cdr;
    }
public:
    typedef T           value_type;
    typedef T*          pointer;
    typedef T&          reference;
    typedef T const*    const_pointer;
    typedef T const&    const_reference;
    typedef size_t      size_type;

    struct iterator : public std::iterator<std::forward_iterator_tag, T>
    {
        friend class slist<T,AllocT>;
        explicit iterator( cons * pcons = 0 )
            : m_pcons( pcons )
        {
        }
        T & operator*() const
        {
            return m_pcons->car;
        }
        T * operator->() const
        {
            return &m_pcons->car;
        }
        iterator & operator++()
        {
            m_pcons = m_pcons->cdr;
            return *this;
        }
        iterator operator++( int )
        {
            iterator i( *this );
            ++*this;
            return i;
        }
        bool operator==( iterator it )
        {
            return m_pcons == it.m_pcons;
        }
        bool operator!=( iterator it )
        {
            return m_pcons != it.m_pcons;
        }
    private:
        cons * m_pcons;
    };

    // not ideal, but good enough for gov'ment work....
    typedef iterator const_iterator;

    explicit slist( char_allocator const & al = char_allocator() )
        : m_impl( convert_allocator<cons>( al, 0 ), 0 )
    {
    }
    ~slist()
    {
        clear();
    }
    void clear()
    {
        for( cons *nxt; m_impl.m_lst; m_impl.m_lst=nxt )
        {
            nxt = m_impl.m_lst->cdr;
            m_impl.allocator().destroy( m_impl.m_lst );
            m_impl.allocator().deallocate( m_impl.m_lst, 1 );
        }
    }
    void push_front( T const & t )
    {
        cons * lst = m_impl.allocator().allocate( 1, 0 );
        try
        {
            m_impl.allocator().construct( lst, cons( t, m_impl.m_lst ) );
        }
        catch(...)
        {
            m_impl.allocator().deallocate( lst, 1 );
            throw;
        }
        m_impl.m_lst = lst;
    }
    template< typename PredT >
    void sort( PredT pred )
    {
        // simple insertion sort
        cons *rst=m_impl.m_lst;
        m_impl.m_lst = 0;
        while( rst )
        {
            cons *cur=m_impl.m_lst, *prv=0;
            while( cur && ! pred( rst->car, cur->car ) )
                prv=cur, cur=cur->cdr;
            if( prv )
                prv->cdr=rst, rst=rst->cdr, prv->cdr->cdr=cur;
            else
                m_impl.m_lst=rst, rst=rst->cdr, m_impl.m_lst->cdr=cur;
        }
    }
    void sort()
    {
        this->sort( std::less<T>() );
    }
    iterator begin() const
    {
        return iterator( m_impl.m_lst );
    }
    iterator end() const
    {
        return iterator();
    }
    bool empty() const
    {
        return 0 == m_impl.m_lst;
    }
    size_t size() const
    {
        size_t len=0;
        for( cons *lst=m_impl.m_lst; lst; lst=lst->cdr, ++len )
            {}
        return len;
    }
    iterator erase( iterator it, iterator hint = iterator() )
    {
        cons **prv = prev( it.m_pcons, hint.m_pcons ); // *prv==it.p
        *prv = it.m_pcons->cdr;
        m_impl.allocator().destroy( it.m_pcons );
        m_impl.allocator().deallocate( it.m_pcons, 1 );
        return iterator( *prv );
    }
    void reverse()
    {
        cons *prv=0, *nxt;
        while( m_impl.m_lst )
            nxt = m_impl.m_lst->cdr, m_impl.m_lst->cdr = prv, prv = m_impl.m_lst, m_impl.m_lst = nxt;
        m_impl.m_lst = prv;
    }
};

template< typename AllocT >
struct basic_charset;

template< typename CharT >
struct posixcharsoff_pred
{
    CharT m_ch;
    posixcharsoff_pred( CharT ch )
        : m_ch( ch )
    {
    }
    bool operator()( regex_ctype_t desc ) const
    {
        return ! local_isctype( m_ch, desc );
    }
    static int local_isctype( char ch, regex_ctype_t desc )
    {
        return regex_isctype( ch, desc );
    }
    static int local_isctype( wchar_t ch, regex_ctype_t desc )
    {
        return regex_iswctype( ch, desc );
    }
};

template< typename CharT, bool CaseT >
struct in_charset_pred
{
    CharT m_ch;
    in_charset_pred( CharT ch )
        : m_ch( ch )
    {
    }
    template< typename AllocT >
    bool operator()( basic_charset<AllocT> const * pcs ) const
    {
        REGEX_VC6( return pcs->in( m_ch COMMA bool2type<CaseT>() ); )
        REGEX_NVC6( return pcs->template in<CaseT>( m_ch ); )
    }
};

template< typename AllocT >
struct basic_charset
{
    typedef basic_charset<std::allocator<char> >    other_type;
    typedef slist<range_type,std::allocator<char> > other_ranges_type;

    typedef slist<range_type,AllocT>                ranges_type;
    typedef slist<regex_ctype_t,AllocT>             posixcharsoff_type;
    typedef slist<other_type const*,AllocT>         nestedcharsets_type;

    typedef typename rebind<AllocT, char>::type     char_allocator_type;

    bool                m_fcompliment;
    bool                m_fskip_extended_check;
    ascii_bitvector     m_ascii_bitvector;
    regex_ctype_t       m_posixcharson;
    ranges_type         m_ranges;
    posixcharsoff_type  m_posixcharsoff;
    nestedcharsets_type m_nestedcharsets;

    explicit basic_charset( char_allocator_type const & al = char_allocator_type() )
        : m_fcompliment( false )
        , m_fskip_extended_check( false )
        , m_ascii_bitvector()
        , m_posixcharson( wct_zero )
        , m_ranges( al )
        , m_posixcharsoff( al )
        , m_nestedcharsets( al )
    {
    }

    // We'll be inheriting from this, so a virtual d'tor is regretably necessary.
    virtual ~basic_charset()
    {
    }

    void clear()
    {
        m_fcompliment = false;
        m_fskip_extended_check = false;
        m_ascii_bitvector.zero();
        m_posixcharson = wct_zero;
        m_ranges.clear();
        m_posixcharsoff.clear();
        m_nestedcharsets.clear();
    }

    // merge one charset into another
    basic_charset & operator|=( other_type const & that )
    {
        if( that.m_fcompliment )
        {
            // If no posix-style character sets are used, then we can merge this
            // nested character set directly into the enclosing character set.
            if( wct_zero == that.m_posixcharson &&
                that.m_posixcharsoff.empty() &&
                that.m_nestedcharsets.empty() )
            {
                m_ascii_bitvector |= ~ that.m_ascii_bitvector;

                // append the inverse of that.m_ranges to this->m_ranges
                wchar_t chlow = UCHAR_MAX;
                typedef typename other_ranges_type::const_iterator iter_type;
                for( iter_type prg = that.m_ranges.begin(); that.m_ranges.end() != prg; ++prg )
                {
                    if( UCHAR_MAX + 1 != prg->first )
                        m_ranges.push_front( range_type( wchar_t( chlow+1 ), wchar_t( prg->first-1 ) ) );
                    chlow = prg->second;
                }
                if( WCHAR_MAX != chlow )
                    m_ranges.push_front( range_type( wchar_t( chlow+1 ), WCHAR_MAX ) );
            }
            else
            {
                // There is no simple way to merge this nested character
                // set into the enclosing character set, so we must save
                // a pointer to the nested character set in a list.
                m_nestedcharsets.push_front( &that );
            }
        }
        else
        {
            m_ascii_bitvector |= that.m_ascii_bitvector;
            std::copy( that.m_ranges.begin(),
                       that.m_ranges.end(),
                       std::front_inserter( m_ranges ) );

            m_posixcharson |= that.m_posixcharson;
            std::copy( that.m_posixcharsoff.begin(),
                       that.m_posixcharsoff.end(),
                       std::front_inserter( m_posixcharsoff ) );

            std::copy( that.m_nestedcharsets.begin(),
                       that.m_nestedcharsets.end(),
                       std::front_inserter( m_nestedcharsets ) );
        }
        return *this;
    }

    // Note overloading based on first parameter
    void set_bit( char ch, bool const fnocase )
    {
        if( fnocase )
        {
            m_ascii_bitvector.set( static_cast<unsigned char>( regex_tolower( ch ) ) );
            m_ascii_bitvector.set( static_cast<unsigned char>( regex_toupper( ch ) ) );
        }
        else
        {
            m_ascii_bitvector.set( static_cast<unsigned char>( ch ) );
        }
    }

    // Note overloading based on first parameter
    void set_bit( wchar_t ch, bool const fnocase )
    {
        if( UCHAR_MAX >= ch )
            set_bit( static_cast<char>( ch ), fnocase );
        else
            m_ranges.push_front( range_type( ch, ch ) );
    }

    // Note overloading based on first two parameters
    void set_bit_range( char ch1, char ch2, bool const fnocase )
    {
        if( static_cast<unsigned char>( ch1 ) > static_cast<unsigned char>( ch2 ) )
            throw bad_regexpr( "invalid range specified in character set" );

        if( fnocase )
        {
            // i is unsigned int to prevent overflow if ch2 is UCHAR_MAX
            for( unsigned int i = static_cast<unsigned char>( ch1 );
                 i <= static_cast<unsigned char>( ch2 ); ++i )
            {
                m_ascii_bitvector.set( static_cast<unsigned char>( regex_toupper( (char) i ) ) );
                m_ascii_bitvector.set( static_cast<unsigned char>( regex_tolower( (char) i ) ) );
            }
        }
        else
        {
            // i is unsigned int to prevent overflow if ch2 is UCHAR_MAX
            for( unsigned int i = static_cast<unsigned char>( ch1 );
                 i <= static_cast<unsigned char>( ch2 ); ++i )
            {
                m_ascii_bitvector.set( static_cast<unsigned char>( i ) );
            }
        }
    }

    // Note overloading based on first two parameters
    void set_bit_range( wchar_t ch1, wchar_t ch2, bool const fnocase )
    {
        if( ch1 > ch2 )
            throw bad_regexpr( "invalid range specified in character set" );

        if( UCHAR_MAX >= ch1 )
            set_bit_range( static_cast<char>( ch1 ), static_cast<char>( regex_min<wchar_t>( UCHAR_MAX, ch2 ) ), fnocase );

        if( UCHAR_MAX < ch2 )
            m_ranges.push_front( range_type( regex_max( static_cast<wchar_t>( UCHAR_MAX + 1 ), ch1 ), ch2 ) );
    }

    void optimize( type2type<wchar_t> )
    {
        if( m_ranges.begin() != m_ranges.end() )
        {
            // this sorts on range_type.m_pfirst ( uses operator<() for pair templates )
            m_ranges.sort();

            // merge ranges that overlap
            typename ranges_type::iterator icur=m_ranges.begin(), iprev=icur++;
            while( icur != m_ranges.end() )
            {
                if( icur->first <= iprev->second + 1 )
                {
                    iprev->second = regex_max( iprev->second, icur->second );
                    icur = m_ranges.erase( icur, iprev );
                }
                else
                {
                    iprev=icur++;
                }
            }
        }

        // For the ASCII range, merge the m_posixcharson info
        // into the ascii_bitvector
        if( wct_zero != m_posixcharson )
        {
            // BUGBUG this is kind of expensive. Think of a better way.
            for( unsigned int i=0; i<=UCHAR_MAX; ++i )
                if( regex_isctype( i, m_posixcharson ) )
                    m_ascii_bitvector.set( static_cast<unsigned char>( i ) );
        }

        // m_fskip_extended_check is a cache which tells us whether we
        // need to check the m_posixcharsoff and m_nestedcharsets vectors,
        // which would only be used in nested user-defined character sets
        m_fskip_extended_check = m_posixcharsoff.empty() && m_nestedcharsets.empty();
    }

    void optimize( type2type<char> )
    {
        optimize( type2type<wchar_t>() );

        // the posixcharson info was merged into the ascii bitvector,
        // so we don't need to ever call regex_isctype ever again.
        m_posixcharson = wct_zero;
    }

    template< bool CaseT, typename CharT >
    bool extended_check( CharT ch REGEX_VC6(COMMA bool2type<CaseT>) ) const
    {
        REGEX_ASSERT( m_fskip_extended_check == ( m_posixcharsoff.empty() && m_nestedcharsets.empty() ) );

        if( m_fskip_extended_check )
        {
            return false;
        }

        return ( m_posixcharsoff.end() !=
                 std::find_if( m_posixcharsoff.begin(), m_posixcharsoff.end(),
                               posixcharsoff_pred<CharT>( ch ) ) )
            || ( m_nestedcharsets.end() !=
                 std::find_if( m_nestedcharsets.begin(), m_nestedcharsets.end(),
                               in_charset_pred<CharT, CaseT>( ch ) ) );
    }

    inline bool in_ranges( wchar_t ch, true_t ) const
    {
        typedef typename ranges_type::const_iterator iter_type;
        iter_type ibegin = m_ranges.begin(), iend = m_ranges.end();

        return ibegin != iend &&
            std::binary_search( ibegin, iend, range_type( ch, ch ), range_less() );
    }

    inline bool in_ranges( wchar_t ch, false_t ) const
    {
        typedef typename ranges_type::const_iterator iter_type;
        iter_type ibegin = m_ranges.begin(), iend = m_ranges.end();

        if( ibegin == iend )
            return false;

        wchar_t const chup = regex_toupper( ch );
        if( std::binary_search( ibegin, iend, range_type( chup, chup ), range_less() ) )
            return true;

        wchar_t const chlo = regex_tolower( ch );
        if( chup == chlo )
            return false;

        return std::binary_search( ibegin, iend, range_type( chlo, chlo ), range_less() );
    }

    // Note overloading based on parameter
    template< bool CaseT >
    bool in( char ch REGEX_VC6(COMMA bool2type<CaseT>) ) const
    {
        // Whoops, forgot to call optimize() on this charset
        REGEX_ASSERT( wct_zero == m_posixcharson );

        return m_fcompliment !=
               (
                    ( m_ascii_bitvector[ static_cast<unsigned char>( ch ) ] )
                 || ( extended_check REGEX_NVC6(<CaseT>) ( ch REGEX_VC6(COMMA bool2type<CaseT>()) ) )
               );
    }

    // Note overloading based on parameter
    template< bool CaseT >
    bool in( wchar_t ch REGEX_VC6(COMMA bool2type<CaseT>) ) const
    {
        // use range_match_type to see if this character is within one of the
        // ranges stored in m_rgranges.
        return m_fcompliment !=
               (
                    ( ( UCHAR_MAX >= ch ) ?
                      ( m_ascii_bitvector[ static_cast<unsigned char>( ch ) ] ) :
                      (    ( in_ranges( ch, bool2type<CaseT>() ) )
                        || ( wct_zero != m_posixcharson && regex_iswctype( ch, m_posixcharson ) ) ) )
                 || ( extended_check REGEX_NVC6(<CaseT>) ( ch REGEX_VC6(COMMA bool2type<CaseT>()) ) )
               );
    }

private:
    basic_charset & operator=( basic_charset const & that );
    basic_charset( basic_charset const & that );
};

// Intrinsic character sets are allocated on the heap with the standard allocator.
// They are either the built-in character sets, or the user-defined ones.
struct charset : public basic_charset<std::allocator<char> >
{
    charset()
    {
    }
private:
    charset( charset const & );
    charset & operator=( charset const & );
};

// charset is no longer an incomplete type so we now
// know how to destroy one. free_charset() is used in syntax2.h
REGEXPR_H_INLINE void free_charset( charset const * pcharset )
{
    delete pcharset;
}

// Custom character sets are the ones that appear in patterns between
// square brackets.  They are allocated in a regex_arena to speed up
// pattern compilation and to make rpattern clean-up faster.
struct custom_charset : public basic_charset<regex_arena>
{
    static void * operator new( size_t size, regex_arena & arena )
    {
        return arena.allocate( size );
    }
    static void operator delete( void *, regex_arena & ) {}
    static void operator delete( void * ) {}

    custom_charset( regex_arena & arena )
        : basic_charset<regex_arena>( arena )
    {
    }
private:
    custom_charset( custom_charset const & );
    custom_charset & operator=( custom_charset const & );
};

template< typename CharT >
class intrinsic_charsets
{
    struct intrinsic_charset : public charset
    {
        intrinsic_charset( bool fcompliment, regex_ctype_t desc, char const * sz )
        {
            reset( fcompliment, desc, sz );
        }
        void reset( bool fcompliment, regex_ctype_t desc, char const * sz )
        {
            clear();
            m_fcompliment  = fcompliment;
            m_posixcharson = desc;
            for( ; *sz; ++sz )
                m_ascii_bitvector.set( static_cast<unsigned char>( *sz ) );
            optimize( type2type<CharT>() );
        }
    private:
        intrinsic_charset( intrinsic_charset const & );
        intrinsic_charset & operator=( intrinsic_charset const & );
    };

    static intrinsic_charset & _get_word_charset()
    {
        static intrinsic_charset s_word_charset( false, wct_alpha()|wct_digit(), "_" );
        return s_word_charset;
    }
    static intrinsic_charset & _get_digit_charset()
    {
        static intrinsic_charset s_digit_charset( false, wct_digit(), "" );
        return s_digit_charset;
    }
    static intrinsic_charset & _get_space_charset()
    {
        static intrinsic_charset s_space_charset( false, wct_space(), "" );
        return s_space_charset;
    }
    static intrinsic_charset & _get_not_word_charset()
    {
        static intrinsic_charset s_not_word_charset( true, wct_alpha()|wct_digit(), "_" );
        return s_not_word_charset;
    }
    static intrinsic_charset & _get_not_digit_charset()
    {
        static intrinsic_charset s_not_digit_charset( true, wct_digit(), "" );
        return s_not_digit_charset;
    }
    static intrinsic_charset & _get_not_space_charset()
    {
        static intrinsic_charset s_not_space_charset( true, wct_space(), "" );
        return s_not_space_charset;
    }
public:
    static charset const & get_word_charset()
    {
        return _get_word_charset();
    }
    static charset const & get_digit_charset()
    {
        return _get_digit_charset();
    }
    static charset const & get_space_charset()
    {
        return _get_space_charset();
    }
    static charset const & get_not_word_charset()
    {
        return _get_not_word_charset();
    }
    static charset const & get_not_digit_charset()
    {
        return _get_not_digit_charset();
    }
    static charset const & get_not_space_charset()
    {
        return _get_not_space_charset();
    }
    static void reset()
    {
        _get_word_charset().reset( false, wct_alpha()|wct_digit(), "_" );
        _get_digit_charset().reset( false, wct_digit(), "" );
        _get_space_charset().reset( false, wct_space(), "" );
        _get_not_word_charset().reset( true, wct_alpha()|wct_digit(), "_" );
        _get_not_digit_charset().reset( true, wct_digit(), "" );
        _get_not_space_charset().reset( true, wct_space(), "" );
    }
};

//
// Operator implementations
//

// Evaluates the beginning-of-string condition
template< typename CStringsT >
struct bos_t
{
    template< typename IterT >
    static bool eval( match_param<IterT> const & param, IterT iter )
    {
        return param.m_ibufferbegin == iter;
    }
};

// Find the beginning of a line, either beginning of a string, or the character
// immediately following a newline
template< typename CStringsT >
struct bol_t
{
    template< typename IterT >
    static bool eval( match_param<IterT> const & param, IterT iter )
    {
        typedef typename std::iterator_traits<IterT>::value_type char_type;
        typedef std::char_traits<char_type> traits_type;

        return param.m_ibufferbegin == iter || traits_type::eq( REGEX_CHAR(char_type,'\n'), *--iter );
    }
};

// Evaluates end-of-string condition for string's
template< typename CStringsT >
struct eos_t
{
    template< typename IterT >
    static bool eval( match_param<IterT> const & param, IterT iter )
    {
        return param.m_iend == iter;
    }
};
template<>
struct eos_t<true_t>
{
    template< typename IterT >
    static bool eval( match_param<IterT> const &, IterT iter )
    {
        typedef typename std::iterator_traits<IterT>::value_type char_type;
        typedef std::char_traits<char_type> traits_type;

        return traits_type::eq( *iter, char_type() );
    }
};

// Evaluates end-of-line conditions, either the end of the string, or a
// newline character.
template< typename CStringsT >
struct eol_t
{
    template< typename IterT >
    static bool eval( match_param<IterT> const & param, IterT iter )
    {
        typedef typename std::iterator_traits<IterT>::value_type char_type;
        typedef std::char_traits<char_type> traits_type;

        return param.m_iend == iter
            || traits_type::eq( REGEX_CHAR(char_type,'\n'), *iter );
    }
};
template<>
struct eol_t<true_t>
{
    template< typename IterT >
    static bool eval( match_param<IterT> const &, IterT iter )
    {
        typedef typename std::iterator_traits<IterT>::value_type char_type;
        typedef std::char_traits<char_type> traits_type;

        return traits_type::eq( *iter, char_type() )
            || traits_type::eq( *iter, REGEX_CHAR(char_type,'\n') );
    }
};

// Evaluates perl's end-of-string conditions, either the end of the string, or a
// newline character followed by end of string. ( Only used by $ and /Z assertions )
template< typename CStringsT >
struct peos_t
{
    template< typename IterT >
    static bool eval( match_param<IterT> const & param, IterT iter )
    {
        typedef typename std::iterator_traits<IterT>::value_type char_type;
        typedef std::char_traits<char_type> traits_type;

        return param.m_iend == iter
            || ( traits_type::eq( REGEX_CHAR(char_type,'\n'), *iter ) && param.m_iend == ++iter );
    }
};
template<>
struct peos_t<true_t>
{
    template< typename IterT >
    static bool eval( match_param<IterT> const &, IterT iter )
    {
        typedef typename std::iterator_traits<IterT>::value_type char_type;
        typedef std::char_traits<char_type> traits_type;

        return traits_type::eq( *iter, char_type() )
            || ( traits_type::eq( *iter, REGEX_CHAR(char_type,'\n') )
                && traits_type::eq( *++iter, char_type() ) );
    }
};

// compare two characters, case-sensitive
template< typename CharT >
struct ch_neq_t
{
    typedef CharT char_type;
    typedef std::char_traits<char_type> traits_type;

    static bool eval( register CharT ch1, register CharT ch2 )
    {
        return ! traits_type::eq( ch1, ch2 );
    }
};

// Compare two characters, disregarding case
template< typename CharT >
struct ch_neq_nocase_t
{
    typedef CharT char_type;
    typedef std::char_traits<char_type> traits_type;

    static bool eval( register CharT ch1, register CharT ch2 )
    {
        return ! traits_type::eq( regex_toupper( ch1 ), regex_toupper( ch2 ) );
    }
};

//
// helper functions for dealing with widths.
//
inline size_t width_add( size_t a, size_t b )
{
    return ( size_t( -1 ) == a || size_t( -1 ) == b ? size_t( -1 ) : a + b );
}

inline size_t width_mult( size_t a, size_t b )
{
    if( 0 == a || 0 == b )
        return 0;

    if( size_t( -1 ) == a || size_t( -1 ) == b )
        return size_t( -1 );

    return a * b;
}

inline bool operator==( width_type const & rhs, width_type const & lhs )
{
    return ( rhs.m_min == lhs.m_min && rhs.m_max == lhs.m_max );
}

inline bool operator!=( width_type const & rhs, width_type const & lhs )
{
    return ( rhs.m_min != lhs.m_min || rhs.m_max != lhs.m_max );
}

inline width_type operator+( width_type const & rhs, width_type const & lhs )
{
    width_type width = { width_add( rhs.m_min, lhs.m_min ), width_add( rhs.m_max, lhs.m_max ) };
    return width;
}

inline width_type operator*( width_type const & rhs, width_type const & lhs )
{
    width_type width = { width_mult( rhs.m_min, lhs.m_min ), width_mult( rhs.m_max, lhs.m_max ) };
    return width;
}

inline width_type & operator+=( width_type & rhs, width_type const & lhs )
{
    rhs.m_min = width_add( rhs.m_min, lhs.m_min );
    rhs.m_max = width_add( rhs.m_max, lhs.m_max );
    return rhs;
}

inline width_type & operator*=( width_type & rhs, width_type const & lhs )
{
    rhs.m_min = width_mult( rhs.m_min, lhs.m_min );
    rhs.m_max = width_mult( rhs.m_max, lhs.m_max );
    return rhs;
}

width_type const zero_width = { 0, 0 };
width_type const worst_width = { 0, size_t( -1 ) };

template< typename IterT >
struct width_param
{
    std::vector<match_group_base<IterT>*> & m_rggroups;
    std::list<size_t> const &               m_invisible_groups;
    width_type                              m_width;

    width_param
    (
        std::vector<match_group_base<IterT>*> & rggroups,
        std::list<size_t> const & invisible_groups
    )
        : m_rggroups( rggroups )
        , m_invisible_groups( invisible_groups )
        , m_width( zero_width )
    {
    }
private:
    width_param & operator=( width_param const & );
};

template< typename CharT >
struct must_have
{
    typedef std::basic_string<CharT>                string_type;
    typedef typename string_type::const_iterator    const_iterator;

    bool            m_has;
    const_iterator  m_begin;
    const_iterator  m_end;
    CharT const *   m_lower;
};

template< typename CharT >
struct peek_param
{
    // "chars" is a list of characters. If every alternate in a group
    // begins with a character or string literal, the "chars" list can
    // be used to speed up the matching of a group.
    size_t              m_cchars;

    union
    {
        CharT           m_rgchars[2];
        CharT const *   m_pchars;
    };

    // "must" is a string that must appear in the match. It is used
    // to speed up the search.
    must_have<CharT>    m_must_have;
};

// --------------------------------------------------------------------------
//
// Class:       sub_expr
//
// Description: patterns are "compiled" into a directed graph of sub_expr
//              structs.  Matching is accomplished by traversing this graph.
//
// Methods:     sub_expr             - construct a sub_expr
//              recursive_match_this - does this sub_expr match at the given location
//              width_this           - what is the width of this sub_expr
//              ~sub_expr            - recursively delete the sub_expr graph
//              next                 - pointer to the next node in the graph
//              next                 - pointer to the next node in the graph
//              recursive_match_next - match the rest of the graph
//              recursive_match_all  - recursive_match_this and recursive_match_next
//              is_assertion         - true if this sub_expr is a zero-width assertion
//              get_width            - find the width of the graph at this sub_expr
//
// Members:     m_pnext      - pointer to the next node in the graph
//
// History:     8/14/2000 - ericne - Created
//
// --------------------------------------------------------------------------
template< typename IterT >
class sub_expr : public sub_expr_base<IterT>
{
    sub_expr * m_pnext;

protected:
    // Only derived classes can instantiate sub_expr's
    sub_expr()
        : m_pnext( 0 )
    {
    }

public:
    typedef IterT                                               iterator_type;
    typedef typename std::iterator_traits<IterT>::value_type    char_type;
    typedef std::char_traits<char_type>                         traits_type;

    virtual ~sub_expr()
    {
        delete m_pnext;
    }

    sub_expr ** pnext()
    {
        return & m_pnext;
    }

    sub_expr const * next() const
    {
        return m_pnext;
    }

    virtual sub_expr<IterT> * quantify( size_t, size_t, bool, regex_arena & )
    {
        throw bad_regexpr( "sub-expression cannot be quantified" );
    }

    // Match this object and all subsequent objects
    // If recursive_match_all returns false, it must not change any of param's state
    virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
    {
        return ( recursive_match_this_( param, icur ) && recursive_match_next_( param, icur, false_t() ) );
    }

    virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const // for C-style strings
    {
        return ( recursive_match_this_c( param, icur ) && recursive_match_next_( param, icur, true_t() ) );
    }

    // match this object only
    virtual bool recursive_match_this_( match_param<IterT> &, IterT & ) const
    {
        return true;
    }

    virtual bool recursive_match_this_c( match_param<IterT> &, IterT & ) const // for C-style strings
    {
        return true;
    }

    // Match all subsequent objects
    template< typename CStringsT >
    bool recursive_match_next_( match_param<IterT> & param, IterT icur, CStringsT ) const
    {
        return m_pnext->recursive_match_all_helper( param, icur, CStringsT() );
    }

    virtual bool iterative_match_this_( match_param<IterT> & param ) const
    {
        param.m_pnext = next();
        return true;
    }

    virtual bool iterative_match_this_c( match_param<IterT> & param ) const // for C-style strings
    {
        param.m_pnext = next();
        return true;
    }

    virtual bool iterative_rematch_this_( match_param<IterT> & ) const
    {
        return false;
    }

    virtual bool iterative_rematch_this_c( match_param<IterT> & ) const // for C-style strings
    {
        return false;
    }

    virtual bool is_assertion() const
    {
        return false;
    }

    width_type get_width( width_param<IterT> & param )
    {
        width_type temp_width = width_this( param );
        if( m_pnext )
            temp_width += m_pnext->get_width( param );
        return temp_width;
    }

    virtual width_type width_this( width_param<IterT> & ) = 0;

    virtual bool peek_this( peek_param<char_type> & ) const
    {
        return false;
    }
};

// An object of type end_of_pattern is used to mark the
// end of the pattern.  (Duh!)  It is responsible for ending
// the recursion, or for letting the search continue if
// the match is zero-width and we are trying to find a
// non-zero-width match
template< typename IterT >
class end_of_pattern : public sub_expr<IterT>
{
    bool _do_match_this( match_param<IterT> & param, IterT icur ) const
    {
        return ! param.m_no0len || param.m_imatchbegin != icur;
    }
public:
    virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
    {
        return _do_match_this( param, icur );
    }
    virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const // for C-style strings
    {
        return _do_match_this( param, icur );
    }
    virtual bool iterative_match_this_( match_param<IterT> & param ) const
    {
        param.m_pnext = 0;
        return _do_match_this( param, param.m_icur );
    }
    virtual bool iterative_match_this_c( match_param<IterT> & param ) const // for C-style strings
    {
        param.m_pnext = 0;
        return _do_match_this( param, param.m_icur );
    }
    virtual width_type width_this( width_param<IterT> & )
    {
        return zero_width;
    }
};

// Base class for sub-expressions which are zero-width
// ( i.e., assertions eat no characters during matching )
// Assertions cannot be quantified.
template< typename IterT >
class assertion : public sub_expr<IterT>
{
public:
    virtual bool is_assertion() const
    {
        return true;
    }
    virtual width_type width_this( width_param<IterT> & )
    {
        return zero_width;
    }
    virtual bool peek_this( peek_param<char_type> & peek ) const
    {
        return this->next()->peek_this( peek );
    }
};

template< typename OpT, typename OpCT >
struct opwrap
{
    typedef OpT  op_type;
    typedef OpCT opc_type;
};

template< typename IterT, typename OpWrapT >
class assert_op : public assertion<IterT>
{
public:
    virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
    {
        return ( assert_op::recursive_match_this_( param, icur ) && this->recursive_match_next_( param, icur, false_t() ) );
    }
    virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const
    {
        return ( assert_op::recursive_match_this_c( param, icur ) && this->recursive_match_next_( param, icur, true_t() ) );
    }
    virtual bool recursive_match_this_( match_param<IterT> & param, IterT & icur ) const
    {
        return OpWrapT::op_type::eval( param, icur );
    }
    virtual bool recursive_match_this_c( match_param<IterT> & param, IterT & icur ) const
    {
        return OpWrapT::opc_type::eval( param, icur );
    }
    virtual bool iterative_match_this_( match_param<IterT> & param ) const
    {
        param.m_pnext = this->next();
        return OpWrapT::op_type::eval( param, param.m_icur );
    }
    virtual bool iterative_match_this_c( match_param<IterT> & param ) const
    {
        param.m_pnext = this->next();
        return OpWrapT::opc_type::eval( param, param.m_icur );
    }
};

template< typename IterT >
inline assertion<IterT> * create_bos( REGEX_FLAGS, regex_arena & arena )
{
    return new( arena ) assert_op<IterT, opwrap<bos_t<false_t>,bos_t<true_t> > >();
}

template< typename IterT >
inline assertion<IterT> * create_eos( REGEX_FLAGS, regex_arena & arena )
{
    return new( arena ) assert_op<IterT, opwrap<peos_t<false_t>,peos_t<true_t> > >();
}

template< typename IterT >
inline assertion<IterT> * create_eoz( REGEX_FLAGS, regex_arena & arena )
{
    return new( arena ) assert_op<IterT, opwrap<eos_t<false_t>, eos_t<true_t> > >();
}

template< typename IterT >
inline assertion<IterT> * create_bol( REGEX_FLAGS flags, regex_arena & arena )
{
    switch( MULTILINE & flags )
    {
    case 0:
        return new( arena ) assert_op<IterT, opwrap<bos_t<false_t>, bos_t<true_t> > >();
    case MULTILINE:
        return new( arena ) assert_op<IterT, opwrap<bol_t<false_t>, bol_t<true_t> > >();
    default:
        REGEX_ASSERT(false);
        return 0;
    }
}

template< typename IterT >
inline assertion<IterT> * create_eol( REGEX_FLAGS flags, regex_arena & arena )
{
    switch( MULTILINE & flags )
    {
    case 0:
        return new( arena ) assert_op<IterT, opwrap<peos_t<false_t>, peos_t<true_t> > >();
    case MULTILINE:
        return new( arena ) assert_op<IterT, opwrap<eol_t<false_t>, eol_t<true_t> > >();
    default:
        REGEX_ASSERT(false);
        return 0;
    }
}

template< typename IterT, typename SubExprT = sub_expr<IterT> >
class match_wrapper : public sub_expr<IterT>
{
    match_wrapper & operator=( match_wrapper const & );
public:
    match_wrapper( SubExprT * psub )
        : m_psub( psub )
    {
    }
    virtual ~match_wrapper()
    {
        _cleanup();
    }
    virtual width_type width_this( width_param<IterT> & param )
    {
        return m_psub->width_this( param );
    }
    virtual bool peek_this( peek_param<char_type> & peek ) const
    {
        return m_psub->peek_this( peek );
    }
protected:
    void _cleanup()
    {
        delete m_psub;
        m_psub = 0;
    }

    SubExprT * m_psub;
};

template< typename IterT, typename SubExprT = sub_expr<IterT> >
class match_quantifier : public match_wrapper<IterT, SubExprT>
{
    match_quantifier & operator=( match_quantifier const & );
public:
    match_quantifier( SubExprT * psub, size_t lbound, size_t ubound )
        : match_wrapper<IterT, SubExprT>( psub )
        , m_lbound( lbound )
        , m_ubound( ubound )
    {
    }
    virtual width_type width_this( width_param<IterT> & param )
    {
        width_type this_width = match_wrapper<IterT, SubExprT>::width_this( param );
        width_type quant_width = { m_lbound, m_ubound };
        return this_width * quant_width;
    }
    virtual bool peek_this( peek_param<char_type> & peek ) const
    {
        return 0 != m_lbound && this->m_psub->peek_this( peek );
    }
protected:
    size_t const m_lbound;
    size_t const m_ubound;
};

template< typename IterT, typename SubExprT >
class atom_quantifier : public match_quantifier<IterT, SubExprT>
{
    atom_quantifier & operator=( atom_quantifier const & );
public:
    atom_quantifier( SubExprT * psub, size_t lbound, size_t ubound )
        : match_quantifier<IterT, SubExprT>( psub, lbound, ubound )
    {
    }
protected:
    void _push_frame( unsafe_stack * pstack, IterT curr, size_t count ) const
    {
        std::pair<IterT, size_t> p( curr, count );
        pstack->push( p );
    }

    void _pop_frame( match_param<IterT> & param ) const
    {
        std::pair<IterT, size_t> p;
        param.m_pstack->pop( p );
        param.m_icur = p.first;
    }
};

template< typename IterT, typename SubExprT >
class max_atom_quantifier : public atom_quantifier<IterT, SubExprT>
{
    max_atom_quantifier & operator=( max_atom_quantifier const & );

public:
    max_atom_quantifier( SubExprT * psub, size_t lbound, size_t ubound )
        : atom_quantifier<IterT, SubExprT>( psub, lbound, ubound )
    {
    }

    // Why a macro instead of a template, you ask?  Performance.  Due to a known
    // bug in the VC7 inline heuristic, I cannot get VC7 to inline the calls to
    // m_psub methods unless I use these macros.  And the performance win is
    // nothing to sneeze at. It's on the order of a 25% speed up to use a macro
    // here instead of a template.
#define DECLARE_RECURSIVE_MATCH_ALL(CSTRINGS,EXT)                                                           \
    virtual bool recursive_match_all ## EXT( match_param<IterT> & param, IterT icur ) const                 \
    {                                                                                                       \
        typedef typename std::iterator_traits<IterT>::difference_type diff_type;                            \
        /* In an ideal world, ibegin and cdiff would be members of a union    */                            \
        /* to conserve stack, but I don't know if IterT is a POD type or not. */                            \
        IterT     ibegin   = icur;                                                                          \
        diff_type cdiff    = 0; /* must be a signed integral type */                                        \
        size_t    cmatches = 0;                                                                             \
        /* greedily match as much as we can*/                                                               \
        if( this->m_ubound && this->m_psub->SubExprT::recursive_match_this ## EXT( param, icur ) )          \
        {                                                                                                   \
            if( 0 == ( cdiff = -std::distance( ibegin, icur ) ) )                                           \
                return this->recursive_match_next_( param, icur, CSTRINGS() );                              \
            while( ++cmatches < this->m_ubound && this->m_psub->SubExprT::recursive_match_this ## EXT( param, icur ) )\
                {}                                                                                          \
        }                                                                                                   \
        if( this->m_lbound > cmatches )                                                                     \
            return false;                                                                                   \
        /* try matching the rest of the pattern, and back off if necessary */                               \
        for( ; ; --cmatches, std::advance( icur, cdiff ) )                                                  \
        {                                                                                                   \
            if( this->recursive_match_next_( param, icur, CSTRINGS() ) )                                    \
                return true;                                                                                \
            if( this->m_lbound == cmatches )                                                                \
                return false;                                                                               \
        }                                                                                                   \
    }

#define DECLARE_ITERATIVE_MATCH_THIS(EXT)                                                                   \
    virtual bool iterative_match_this ## EXT( match_param<IterT> & param ) const                            \
    {                                                                                                       \
        IterT ibegin = param.m_icur;                                                                        \
        size_t cmatches = 0;                                                                                \
        if( this->m_ubound && this->m_psub->SubExprT::iterative_match_this ## EXT( param ) )                \
        {                                                                                                   \
            if( 0 == std::distance( ibegin, param.m_icur ) )                                                \
            {                                                                                               \
                cmatches = this->m_lbound;                                                                  \
            }                                                                                               \
            else                                                                                            \
            {                                                                                               \
                while( ++cmatches < this->m_ubound && this->m_psub->SubExprT::iterative_match_this ## EXT( param ) )\
                    {}                                                                                      \
            }                                                                                               \
        }                                                                                                   \
        if( cmatches >= this->m_lbound )                                                                    \
        {                                                                                                   \
            this->_push_frame( param.m_pstack, ibegin, cmatches );                                          \
            param.m_pnext = this->next();                                                                   \
            return true;                                                                                    \
        }                                                                                                   \
        param.m_icur = ibegin;                                                                              \
        return false;                                                                                       \
    }

#define DECLARE_ITERATIVE_REMATCH_THIS(EXT)                                                                 \
    virtual bool iterative_rematch_this ## EXT( match_param<IterT> & param ) const                          \
    {                                                                                                       \
        typedef std::pair<IterT, size_t> top_type;                                                          \
        size_t & cmatches = REGEX_VC6( param.m_pstack->top( type2type<top_type>() ).second )                \
                            REGEX_NVC6( param.m_pstack->template top<top_type>().second );                  \
        if( this->m_lbound != cmatches )                                                                    \
        {                                                                                                   \
            --cmatches;                                                                                     \
            this->m_psub->SubExprT::iterative_rematch_this ## EXT( param );                                 \
            param.m_pnext = this->next();                                                                   \
            return true;                                                                                    \
        }                                                                                                   \
        this->_pop_frame( param );                                                                          \
        return false;                                                                                       \
    }

    DECLARE_RECURSIVE_MATCH_ALL(false_t,_)
    DECLARE_RECURSIVE_MATCH_ALL(true_t,_c)
    DECLARE_ITERATIVE_MATCH_THIS(_)
    DECLARE_ITERATIVE_MATCH_THIS(_c)
    DECLARE_ITERATIVE_REMATCH_THIS(_)
    DECLARE_ITERATIVE_REMATCH_THIS(_c)

#undef DECLARE_RECURSIVE_MATCH_ALL
#undef DECLARE_ITERATIVE_MATCH_THIS
#undef DECLARE_ITERATIVE_REMATCH_THIS
};

template< typename IterT, typename SubExprT >
class min_atom_quantifier : public atom_quantifier<IterT, SubExprT>
{
    min_atom_quantifier & operator=( min_atom_quantifier const & );

public:
    min_atom_quantifier( SubExprT * psub, size_t lbound, size_t ubound )
        : atom_quantifier<IterT, SubExprT>( psub, lbound, ubound )
    {
    }

    // Why a macro instead of a template, you ask?  Performance.  Due to a known
    // bug in the VC7 inline heuristic, I cannot get VC7 to inline the calls to
    // m_psub methods unless I use these macros.  And the performance win is
    // nothing to sneeze at. It's on the order of a 25% speed up to use a macro
    // here instead of a template.
#define DECLARE_RECURSIVE_MATCH_ALL(CSTRINGS,EXT)                                                           \
    virtual bool recursive_match_all ## EXT( match_param<IterT> & param, IterT icur ) const                 \
    {                                                                                                       \
        IterT  icur_tmp = icur;                                                                             \
        size_t cmatches = 0;                                                                                \
        if( this->m_psub->SubExprT::recursive_match_this ## EXT( param, icur_tmp ) )                        \
        {                                                                                                   \
            if( icur_tmp == icur )                                                                          \
                return this->recursive_match_next_( param, icur, CSTRINGS() );                              \
            if( this->m_lbound )                                                                            \
            {                                                                                               \
                icur = icur_tmp;                                                                            \
                ++cmatches;                                                                                 \
            }                                                                                               \
            for( ; cmatches < this->m_lbound; ++cmatches )                                                  \
            {                                                                                               \
                if( ! this->m_psub->SubExprT::recursive_match_this ## EXT( param, icur ) )                  \
                    return false;                                                                           \
            }                                                                                               \
        }                                                                                                   \
        else if( this->m_lbound )                                                                           \
        {                                                                                                   \
            return false;                                                                                   \
        }                                                                                                   \
        do                                                                                                  \
        {                                                                                                   \
            if( this->recursive_match_next_( param, icur, CSTRINGS() ) )                                    \
                return true;                                                                                \
        }                                                                                                   \
        while( cmatches < this->m_ubound &&                                                                 \
             ( ++cmatches, this->m_psub->SubExprT::recursive_match_this ## EXT( param, icur ) ) );          \
        return false;                                                                                       \
    }

#define DECLARE_ITERATIVE_MATCH_THIS(EXT)                                                                   \
    virtual bool iterative_match_this ## EXT( match_param<IterT> & param ) const                            \
    {                                                                                                       \
        IterT ibegin = param.m_icur;                                                                        \
        size_t cmatches = 0;                                                                                \
        if( this->m_psub->SubExprT::iterative_match_this ## EXT( param ) )                                  \
        {                                                                                                   \
            if( 0 == std::distance( ibegin, param.m_icur ) )                                                \
            {                                                                                               \
                cmatches = this->m_ubound;                                                                  \
            }                                                                                               \
            else if( this->m_lbound )                                                                       \
            {                                                                                               \
                for( ++cmatches; cmatches < this->m_lbound; ++cmatches )                                    \
                {                                                                                           \
                    if( ! this->m_psub->SubExprT::iterative_match_this ## EXT( param ) )                    \
                    {                                                                                       \
                        param.m_icur = ibegin;                                                              \
                        return false;                                                                       \
                    }                                                                                       \
                }                                                                                           \
            }                                                                                               \
            else                                                                                            \
            {                                                                                               \
                param.m_icur = ibegin;                                                                      \
            }                                                                                               \
        }                                                                                                   \
        else if( this->m_lbound )                                                                           \
        {                                                                                                   \
            return false;                                                                                   \
        }                                                                                                   \
        this->_push_frame( param.m_pstack, ibegin, cmatches );                                              \
        param.m_pnext = this->next();                                                                       \
        return true;                                                                                        \
    }

#define DECLARE_ITERATIVE_REMATCH_THIS(EXT)                                                                 \
    virtual bool iterative_rematch_this ## EXT( match_param<IterT> & param ) const                          \
    {                                                                                                       \
        typedef std::pair<IterT, size_t> top_type;                                                          \
        size_t & cmatches = REGEX_VC6( param.m_pstack->top( type2type<top_type>() ).second )                \
                            REGEX_NVC6( param.m_pstack->template top<top_type>().second );                  \
        if( cmatches == this->m_ubound || ! this->m_psub->SubExprT::iterative_match_this ## EXT( param ) )  \
        {                                                                                                   \
            this->_pop_frame( param );                                                                      \
            return false;                                                                                   \
        }                                                                                                   \
        ++cmatches;                                                                                         \
        param.m_pnext = this->next();                                                                       \
        return true;                                                                                        \
    }

    DECLARE_RECURSIVE_MATCH_ALL(false_t,_)
    DECLARE_RECURSIVE_MATCH_ALL(true_t,_c)
    DECLARE_ITERATIVE_MATCH_THIS(_)
    DECLARE_ITERATIVE_MATCH_THIS(_c)
    DECLARE_ITERATIVE_REMATCH_THIS(_)
    DECLARE_ITERATIVE_REMATCH_THIS(_c)

#undef DECLARE_RECURSIVE_MATCH_ALL
#undef DECLARE_ITERATIVE_MATCH_THIS
#undef DECLARE_ITERATIVE_REMATCH_THIS
};

template< typename CharT >
struct char_nocase
{
    CharT m_chlo;
    CharT m_chhi;
    char_nocase( CharT chlo, CharT chhi )
        : m_chlo(chlo)
        , m_chhi(chhi)
    {
    }
};

template< typename IterT >
class match_char : public sub_expr<IterT>
{
    match_char & operator=( match_char const & );
public:
    typedef typename sub_expr<IterT>::char_type char_type;

    virtual width_type width_this( width_param<IterT> & )
    {
        width_type width = { 1, 1 };
        return width;
    }
    virtual bool iterative_rematch_this_( match_param<IterT> & param ) const
    {
        --param.m_icur;
        return false;
    }
    virtual bool iterative_rematch_this_c( match_param<IterT> & param ) const
    {
        --param.m_icur;
        return false;
    }
};

template< typename IterT, typename CharT >
class match_char_t : public match_char<IterT>
{
    match_char_t & operator=( match_char_t const & );

public:
    match_char_t( CharT const & ch )
        : m_ch( ch )
    {
    }

    virtual sub_expr<IterT> * quantify( size_t lbound, size_t ubound, bool greedy, regex_arena & arena )
    {
        if( greedy )
            return new( arena ) max_atom_quantifier<IterT, match_char_t<IterT, CharT> >( this, lbound, ubound );
        else
            return new( arena ) min_atom_quantifier<IterT, match_char_t<IterT, CharT> >( this, lbound, ubound );
    }
    virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
    {
        return ( match_char_t::recursive_match_this_( param, icur ) && this->recursive_match_next_( param, icur, false_t() ) );
    }
    virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const
    {
        return ( match_char_t::recursive_match_this_c( param, icur ) && this->recursive_match_next_( param, icur, true_t() ) );
    }
    virtual bool recursive_match_this_( match_param<IterT> & param, IterT & icur ) const
    {
        return _do_match_this REGEX_NVC6(<false_t>) ( param, icur REGEX_VC6(COMMA false_t()) );
    }
    virtual bool recursive_match_this_c( match_param<IterT> & param, IterT & icur ) const
    {
        return _do_match_this REGEX_NVC6(<true_t>) ( param, icur REGEX_VC6(COMMA true_t()) );
    }
    virtual bool iterative_match_this_( match_param<IterT> & param ) const
    {
        param.m_pnext = this->next();
        return _do_match_this REGEX_NVC6(<false_t>) ( param, param.m_icur REGEX_VC6(COMMA false_t()) );
    }
    virtual bool iterative_match_this_c( match_param<IterT> & param ) const
    {
        param.m_pnext = this->next();
        return _do_match_this REGEX_NVC6(<true_t>) ( param, param.m_icur REGEX_VC6(COMMA true_t()) );
    }
    virtual bool peek_this( peek_param<char_type> & peek ) const
    {
        peek_this_impl( peek, m_ch );
        return true;
    }
private:
    static bool eq( char_type left, char_type right )
    {
        return traits_type::eq( left, right );
    }
    static bool eq( char_type left, char_nocase<char_type> right )
    {
        return traits_type::eq( left, right.m_chlo ) ||
               traits_type::eq( left, right.m_chhi );
    }
    static void peek_this_impl( peek_param<char_type> & peek, char_type ch )
    {
        peek.m_cchars           = 1;
        peek.m_rgchars[0]       = ch;
        peek.m_must_have.m_has  = false;
    }
    static void peek_this_impl( peek_param<char_type> & peek, char_nocase<char_type> ch )
    {
        peek.m_cchars           = 2;
        peek.m_rgchars[0]       = ch.m_chlo;
        peek.m_rgchars[1]       = ch.m_chhi;
        peek.m_must_have.m_has  = false;
    }
    template< typename CStringsT >
    bool _do_match_this( match_param<IterT> & param, IterT & icur REGEX_VC6(COMMA CStringsT) ) const
    {
        if( eos_t<CStringsT>::eval( param, icur ) || ! eq( *icur, m_ch ) )
            return false;
        ++icur;
        return true;
    }
    CharT const m_ch;
};

template< typename IterT >
inline match_char<IterT> * create_char
(
     typename std::iterator_traits<IterT>::value_type ch,
     REGEX_FLAGS flags,
     regex_arena & arena
)
{
    typedef typename std::iterator_traits<IterT>::value_type char_type;
    typedef std::char_traits<char_type> traits_type;

    switch( NOCASE & flags )
    {
    case 0:
        return new( arena ) match_char_t<IterT, char_type>( ch );

    case NOCASE:
        {
            char_type lower = regex_tolower( ch );
            char_type upper = regex_toupper( ch );

            if( traits_type::eq( lower, upper ) )
                return new( arena ) match_char_t<IterT, char_type>( ch );
            else
                return new( arena ) match_char_t<IterT, char_nocase<char_type> >( char_nocase<char_type>( lower, upper ) );
        }
    default:
        REGEX_ASSERT(false);
        return 0;
    }
}

template< typename IterT >
class match_literal : public sub_expr<IterT>
{
    match_literal & operator=( match_literal const & );
public:
    typedef typename sub_expr<IterT>::char_type                     char_type;
    typedef std::basic_string<char_type>                            string_type;
    typedef typename string_type::iterator                          iterator;
    typedef typename string_type::const_iterator                    const_iterator;
    typedef typename std::iterator_traits<IterT>::difference_type   diff_type;

    match_literal( const_iterator ibegin, const_iterator iend )
        : m_ibegin( ibegin )
        , m_iend( iend )
        , m_dist( std::distance( m_ibegin, m_iend ) )
    {
    }

    const_iterator const m_ibegin;
    const_iterator const m_iend;
    diff_type      const m_dist; // must be signed integral type

    virtual width_type width_this( width_param<IterT> & )
    {
        width_type width = { static_cast<size_t>( m_dist ), static_cast<size_t>( m_dist ) };
        return width;
    }
    virtual bool iterative_rematch_this_( match_param<IterT> & param ) const
    {
        std::advance( param.m_icur, -m_dist );
        return false;
    }
    virtual bool iterative_rematch_this_c( match_param<IterT> & param ) const
    {
        std::advance( param.m_icur, -m_dist );
        return false;
    }
};

template< typename IterT >
class match_literal_t : public match_literal<IterT>
{
    match_literal_t & operator=( match_literal_t const & );
public:
    typedef typename match_literal<IterT>::char_type       char_type;
    typedef typename match_literal<IterT>::string_type     string_type;
    typedef typename match_literal<IterT>::iterator        iterator;
    typedef typename match_literal<IterT>::const_iterator  const_iterator;

    match_literal_t( const_iterator ibegin, const_iterator iend )
        : match_literal<IterT>( ibegin, iend )
    {
    }

    virtual sub_expr<IterT> * quantify( size_t lbound, size_t ubound, bool greedy, regex_arena & arena )
    {
        if( greedy )
            return new( arena ) max_atom_quantifier<IterT, match_literal_t<IterT> >( this, lbound, ubound );
        else
            return new( arena ) min_atom_quantifier<IterT, match_literal_t<IterT> >( this, lbound, ubound );
    }
    virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
    {
        return ( match_literal_t::recursive_match_this_( param, icur ) && this->recursive_match_next_( param, icur, false_t() ) );
    }
    virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const
    {
        return ( match_literal_t::recursive_match_this_c( param, icur ) && this->recursive_match_next_( param, icur, true_t() ) );
    }
    virtual bool recursive_match_this_( match_param<IterT> & param, IterT & icur ) const
    {
        return _do_match_this REGEX_NVC6(<false_t>) ( param, icur REGEX_VC6(COMMA false_t()) );
    }
    virtual bool recursive_match_this_c( match_param<IterT> & param, IterT & icur ) const
    {
        return _do_match_this REGEX_NVC6(<true_t>) ( param, icur REGEX_VC6(COMMA true_t()) );
    }
    virtual bool iterative_match_this_( match_param<IterT> & param ) const
    {
        param.m_pnext = this->next();
        return _do_match_this REGEX_NVC6(<false_t>) ( param, param.m_icur REGEX_VC6(COMMA false_t()) );
    }
    virtual bool iterative_match_this_c( match_param<IterT> & param ) const
    {
        param.m_pnext = this->next();
        return _do_match_this REGEX_NVC6(<true_t>) ( param, param.m_icur REGEX_VC6(COMMA true_t()) );
    }
    virtual bool peek_this( peek_param<char_type> & peek ) const
    {
        peek.m_cchars             = 1;
        peek.m_rgchars[0]         = *this->m_ibegin;
        peek.m_must_have.m_has    = true;
        peek.m_must_have.m_begin  = this->m_ibegin;
        peek.m_must_have.m_end    = this->m_iend;
        peek.m_must_have.m_lower  = 0;
        return true;
    }
private:
    template< typename CStringsT >
    bool _do_match_this( match_param<IterT> & param, IterT & icur REGEX_VC6(COMMA CStringsT) ) const
    {
        IterT icur_tmp = icur;
        const_iterator ithis = this->m_ibegin;
        for( ; this->m_iend != ithis; ++icur_tmp, ++ithis )
        {
            if( eos_t<CStringsT>::eval( param, icur_tmp ) || ! traits_type::eq( *ithis, *icur_tmp ) )
                return false;
        }
        icur = icur_tmp;
        return true;
    }
};

template< typename IterT >
class match_literal_nocase_t : public match_literal<IterT>
{
    match_literal_nocase_t & operator=( match_literal_nocase_t const & );

public:
    typedef typename match_literal<IterT>::char_type       char_type;
    typedef typename match_literal<IterT>::string_type     string_type;
    typedef typename match_literal<IterT>::iterator        iterator;
    typedef typename match_literal<IterT>::const_iterator  const_iterator;

    match_literal_nocase_t( iterator ibegin, const_iterator iend, regex_arena & arena )
        : match_literal<IterT>( ibegin, iend )
        , m_szlower( arena_allocator<char_type>( arena ).allocate( m_dist ) )
    {
        // Copy from ibegin to m_szlower
        std::copy( this->m_ibegin, this->m_iend, m_szlower );
        // Store the uppercase version of the literal in [ m_ibegin, m_iend ).
        regex_toupper( ibegin, iend );
        // Store the lowercase version of the literal in m_strlower.
        regex_tolower( m_szlower, m_szlower + this->m_dist );
    }

    virtual sub_expr<IterT> * quantify( size_t lbound, size_t ubound, bool greedy, regex_arena & arena )
    {
        if( greedy )
            return new( arena ) max_atom_quantifier<IterT, match_literal_nocase_t<IterT> >( this, lbound, ubound );
        else
            return new( arena ) min_atom_quantifier<IterT, match_literal_nocase_t<IterT> >( this, lbound, ubound );
    }
    virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
    {
        return ( match_literal_nocase_t::recursive_match_this_( param, icur ) && this->recursive_match_next_( param, icur, false_t() ) );
    }
    virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const
    {
        return ( match_literal_nocase_t::recursive_match_this_c( param, icur ) && this->recursive_match_next_( param, icur, true_t() ) );
    }
    virtual bool recursive_match_this_( match_param<IterT> & param, IterT & icur ) const
    {
        return _do_match_this REGEX_NVC6(<false_t>) ( param, icur REGEX_VC6(COMMA false_t()) );
    }
    virtual bool recursive_match_this_c( match_param<IterT> & param, IterT & icur ) const
    {
        return _do_match_this REGEX_NVC6(<true_t>) ( param, icur REGEX_VC6(COMMA true_t()) );
    }
    virtual bool iterative_match_this_( match_param<IterT> & param ) const
    {
        param.m_pnext = this->next();
        return _do_match_this REGEX_NVC6(<false_t>) ( param, param.m_icur REGEX_VC6(COMMA false_t()) );
    }
    virtual bool iterative_match_this_c( match_param<IterT> & param ) const
    {
        param.m_pnext = this->next();
        return _do_match_this REGEX_NVC6(<true_t>) ( param, param.m_icur REGEX_VC6(COMMA true_t()) );
    }
    virtual bool peek_this( peek_param<char_type> & peek ) const
    {
        peek.m_cchars             = 2;
        peek.m_rgchars[0]         = *this->m_ibegin;
        peek.m_rgchars[1]         = *m_szlower;
        peek.m_must_have.m_has    = true;
        peek.m_must_have.m_begin  = this->m_ibegin;
        peek.m_must_have.m_end    = this->m_iend;
        peek.m_must_have.m_lower  = m_szlower;
        return true;
    }
private:
    // Allocated from a regex arena. The memory will be cleaned up
    // when the arena is deallocated.
    char_type *const m_szlower;

    template< typename CStringsT >
    bool _do_match_this( match_param<IterT> & param, IterT & icur REGEX_VC6(COMMA CStringsT) ) const
    {
        IterT icur_tmp = icur;
        const_iterator ithisu    = this->m_ibegin;  // uppercase
        char_type const * ithisl = m_szlower;       // lowercase
        for( ; this->m_iend != ithisu; ++icur_tmp, ++ithisu, ++ithisl )
        {
            if( eos_t<CStringsT>::eval( param, icur_tmp ) ||
                ( ! traits_type::eq( *ithisu, *icur_tmp ) &&
                  ! traits_type::eq( *ithisl, *icur_tmp ) ) )
                return false;
        }
        icur = icur_tmp;
        return true;
    }
};

template< typename IterT, typename IBeginT, typename IEndT >
inline sub_expr<IterT> * create_literal
(
    IBeginT         ibegin,
    IEndT           iend,
    REGEX_FLAGS     flags,
    regex_arena &   arena
)
{
    // A match_char is faster than a match_literal, so prefer it
    // when the literal to match is only 1 char wide.
    if( 1 == std::distance<IEndT>( ibegin, iend ) )
    {
        return create_char<IterT>( *ibegin, flags, arena );
    }

    switch( NOCASE & flags )
    {
    case 0:
        return new( arena ) match_literal_t<IterT>( ibegin, iend );
    case NOCASE:
        return new( arena ) match_literal_nocase_t<IterT>( ibegin, iend, arena );
    default:
        REGEX_ASSERT(false);
        return 0;
    }
}

template< typename IterT >
class match_any : public sub_expr<IterT>
{
public:
    virtual width_type width_this( width_param<IterT> & )
    {
        width_type width = { 1, 1 };
        return width;
    }
    virtual bool iterative_rematch_this_( match_param<IterT> & param ) const
    {
        --param.m_icur;
        return false;
    }
    virtual bool iterative_rematch_this_c( match_param<IterT> & param ) const
    {
        --param.m_icur;
        return false;
    }
};

template< typename IterT, typename EosWrapT >
class match_any_t : public match_any<IterT>
{
    bool _do_match_this( match_param<IterT> & param, IterT & icur ) const
    {
        if( EosWrapT::op_type::eval( param, icur ) )
            return false;
        ++icur;
        return true;
    }
    bool _do_match_this_c( match_param<IterT> & param, IterT & icur ) const
    {
        if( EosWrapT::opc_type::eval( param, icur ) )
            return false;
        ++icur;
        return true;
    }
public:
    virtual sub_expr<IterT> * quantify( size_t lbound, size_t ubound, bool greedy, regex_arena & arena )
    {
        if( greedy )
            return new( arena ) max_atom_quantifier<IterT, match_any_t<IterT, EosWrapT> >( this, lbound, ubound );
        else
            return new( arena ) min_atom_quantifier<IterT, match_any_t<IterT, EosWrapT> >( this, lbound, ubound );
    }

    virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
    {
        return ( match_any_t::recursive_match_this_( param, icur ) && this->recursive_match_next_( param, icur, false_t() ) );
    }
    virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const
    {
        return ( match_any_t::recursive_match_this_c( param, icur ) && this->recursive_match_next_( param, icur, true_t() ) );
    }
    virtual bool recursive_match_this_( match_param<IterT> & param, IterT & icur ) const
    {
        return _do_match_this( param, icur );
    }
    virtual bool recursive_match_this_c( match_param<IterT> & param, IterT & icur ) const
    {
        return _do_match_this_c( param, icur );
    }
    virtual bool iterative_match_this_( match_param<IterT> & param ) const
    {
        param.m_pnext = this->next();
        return _do_match_this( param, param.m_icur );
    }
    virtual bool iterative_match_this_c( match_param<IterT> & param ) const
    {
        param.m_pnext = this->next();
        return _do_match_this_c( param, param.m_icur );
    }
};

template< typename IterT >
inline match_any<IterT> * create_any( REGEX_FLAGS flags, regex_arena & arena )
{
    switch( SINGLELINE & flags )
    {
    case 0:
        return new( arena ) match_any_t<IterT, opwrap<eol_t<false_t>, eol_t<true_t> > >();
    case SINGLELINE:
        return new( arena ) match_any_t<IterT, opwrap<eos_t<false_t>, eos_t<true_t> > >();
    default:
        REGEX_ASSERT(false);
        return 0;
    }
}

template< typename IterT >
class match_charset : public sub_expr<IterT>
{
public:
    virtual width_type width_this( width_param<IterT> & )
    {
        width_type width = { 1, 1 };
        return width;
    }
    virtual bool iterative_rematch_this_( match_param<IterT> & param ) const
    {
        --param.m_icur;
        return false;
    }
    virtual bool iterative_rematch_this_c( match_param<IterT> & param ) const
    {
        --param.m_icur;
        return false;
    }
};

template< typename IterT, typename CharSetPtrT, bool CaseT >
class match_charset_t : public match_charset<IterT>
{
    CharSetPtrT const m_pcs;

    match_charset_t & operator=( match_charset_t const & );

    template< typename CStringsT >
    bool _do_match_this( match_param<IterT> & param, IterT & icur REGEX_VC6(COMMA CStringsT) ) const
    {
        if( eos_t<CStringsT>::eval( param, icur ) || 
            ! m_pcs->REGEX_NVC6(template) in REGEX_NVC6(<CaseT>)( *icur REGEX_VC6(COMMA bool2type<CaseT>()) ) )
            return false;
        ++icur;
        return true;
    }
public:
    match_charset_t( CharSetPtrT pcs )
         : m_pcs( pcs )
    {
    }

    virtual sub_expr<IterT> * quantify( size_t lbound, size_t ubound, bool greedy, regex_arena & arena )
    {
        if( greedy )
            return new( arena ) max_atom_quantifier<IterT, match_charset_t<IterT, CharSetPtrT, CaseT> >( this, lbound, ubound );
        else
            return new( arena ) min_atom_quantifier<IterT, match_charset_t<IterT, CharSetPtrT, CaseT> >( this, lbound, ubound );
    }
    virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
    {
        return ( match_charset_t::recursive_match_this_( param, icur ) && this->recursive_match_next_( param, icur, false_t() ) );
    }
    virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const
    {
        return ( match_charset_t::recursive_match_this_c( param, icur ) && this->recursive_match_next_( param, icur, true_t() ) );
    }
    virtual bool recursive_match_this_( match_param<IterT> & param, IterT & icur ) const
    {
        return _do_match_this REGEX_NVC6(<false_t>) ( param, icur REGEX_VC6(COMMA false_t()) );
    }
    virtual bool recursive_match_this_c( match_param<IterT> & param, IterT & icur ) const
    {
        return _do_match_this REGEX_NVC6(<true_t>) ( param, icur REGEX_VC6(COMMA true_t()) );
    }
    virtual bool iterative_match_this_( match_param<IterT> & param ) const
    {
        param.m_pnext = this->next();
        return _do_match_this REGEX_NVC6(<false_t>) ( param, param.m_icur REGEX_VC6(COMMA false_t()) );
    }
    virtual bool iterative_match_this_c( match_param<IterT> & param ) const
    {
        param.m_pnext = this->next();
        return _do_match_this REGEX_NVC6(<true_t>) ( param, param.m_icur REGEX_VC6(COMMA true_t()) );
    }
};

template< typename IterT >
inline match_charset<IterT> * create_charset
(
    charset const &    cs,
    REGEX_FLAGS        flags,
    regex_arena &      arena
)
{
    switch( NOCASE & flags )
    {
    case 0:
        return new( arena ) match_charset_t<IterT, charset const*, true>( &cs );
    case NOCASE:
        return new( arena ) match_charset_t<IterT, charset const*, false>( &cs );
    default:
        REGEX_ASSERT(false);
        return 0;
    }
}

template< typename IterT >
inline match_charset<IterT> * create_custom_charset
(
    custom_charset const *  pcs,
    REGEX_FLAGS             flags,
    regex_arena &           arena
)
{
    typedef std::auto_ptr<custom_charset const> auto_charset;
    auto_charset acs( pcs );

    switch( NOCASE & flags )
    {
    case 0:
        return new( arena ) match_charset_t<IterT, auto_charset, true>( acs );
    case NOCASE:
        return new( arena ) match_charset_t<IterT, auto_charset, false>( acs );
    default:
        REGEX_ASSERT(false);
        return 0;
    }
}

template< bool IsBoundaryT >
struct word_boundary
{
    static bool eval( bool fprevword, bool fthisword )
    {
        return IsBoundaryT == ( fprevword != fthisword );
    }
};

struct word_start
{
    static bool eval( bool fprevword, bool fthisword )
    {
        return ! fprevword && fthisword;
    }
};

struct word_stop
{
    static bool eval( bool fprevword, bool fthisword )
    {
        return fprevword && ! fthisword;
    }
};

template< typename IterT, typename CondT >
class word_assertion_t : public assertion<IterT>
{
    word_assertion_t & operator=( word_assertion_t const & );
public:
    typedef typename assertion<IterT>::char_type char_type;

    word_assertion_t()
        : m_isword( intrinsic_charsets<char_type>::get_word_charset() )
    {
    }
    virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
    {
        return ( word_assertion_t::recursive_match_this_( param, icur ) && this->recursive_match_next_( param, icur, false_t() ) );
    }
    virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const
    {
        return ( word_assertion_t::recursive_match_this_c( param, icur ) && this->recursive_match_next_( param, icur, true_t() ) );
    }
    virtual bool recursive_match_this_( match_param<IterT> & param, IterT & icur ) const
    {
        return _do_match_this REGEX_NVC6(<false_t>) ( param, icur REGEX_VC6(COMMA false_t()) );
    }
    virtual bool recursive_match_this_c( match_param<IterT> & param, IterT & icur ) const
    {
        return _do_match_this REGEX_NVC6(<true_t>) ( param, icur REGEX_VC6(COMMA true_t()) );
    }
    virtual bool iterative_match_this_( match_param<IterT> & param ) const
    {
        param.m_pnext = this->next();
        return _do_match_this REGEX_NVC6(<false_t>) ( param, param.m_icur REGEX_VC6(COMMA false_t()) );
    }
    virtual bool iterative_match_this_c( match_param<IterT> & param ) const
    {
        param.m_pnext = this->next();
        return _do_match_this REGEX_NVC6(<true_t>) ( param, param.m_icur REGEX_VC6(COMMA true_t()) );
    }
private:
    bool _is_word( char_type ch ) const
    {
        return REGEX_VC6( m_isword.in( ch COMMA true_t() ) )
               REGEX_NVC6( m_isword.template in<true>( ch ) );
    }
    template< typename CStringsT >
    bool _do_match_this( match_param<IterT> & param, IterT icur REGEX_VC6(COMMA CStringsT) ) const
    {
        bool const fthisword = ! eos_t<CStringsT>::eval( param, icur ) && _is_word( *icur );
        bool const fprevword = ! bos_t<CStringsT>::eval( param, icur ) && _is_word( *--icur );

        return CondT::eval( fprevword, fthisword );
    }
    charset const & m_isword;
};

template< typename IterT >
inline assertion<IterT> * create_word_boundary
(
    bool            fisboundary,
    REGEX_FLAGS,    // flags
    regex_arena &   arena
)
{
    if( fisboundary )
        return new( arena ) word_assertion_t<IterT, word_boundary<true> >();
    else
        return new( arena ) word_assertion_t<IterT, word_boundary<false> >();
}

template< typename IterT >
inline assertion<IterT> * create_word_start( REGEX_FLAGS, regex_arena & arena )
{
    return new( arena ) word_assertion_t<IterT, word_start>();
}

template< typename IterT >
inline assertion<IterT> * create_word_stop( REGEX_FLAGS, regex_arena & arena )
{
    return new( arena ) word_assertion_t<IterT, word_stop>();
}

// an "extent" represents the range of backrefs that can be modified as the
// result of a look-ahead or look-behind
typedef std::pair<size_t, size_t> extent_type;

template< typename IterT > class max_group_quantifier;
template< typename IterT > class min_group_quantifier;

template< typename IterT >
class match_group_base : public sub_expr<IterT>
{
protected:
    typedef slist<sub_expr<IterT>*,regex_arena> alt_list_type;

private:
    match_group_base & operator=( match_group_base const & );

    void _push_frame( match_param<IterT> & param ) const
    {
        unsafe_stack * ps = param.m_pstack;

        if( size_t( -1 ) != m_cgroup )
        {
            IterT & reserved1 = param.m_prgbackrefs[ m_cgroup ].reserved1;
            ps->push( reserved1 );
            reserved1 = param.m_icur;
        }

        ps->push( m_rgalternates.begin() );
    }

    void _pop_frame( match_param<IterT> & param ) const
    {
        typedef typename alt_list_type::const_iterator iter_type;
        unsafe_stack * ps = param.m_pstack;

        REGEX_VC6( ps->pop( type2type<iter_type>() COMMA 0 ); )
        REGEX_NVC6( ps->template pop<iter_type>(); )

        if( size_t( -1 ) != m_cgroup )
            ps->pop( param.m_prgbackrefs[ m_cgroup ].reserved1 );
    }

    template< typename CStringsT >
    bool _recursive_match_all( match_param<IterT> & param, IterT icur REGEX_VC6(COMMA CStringsT) ) const
    {
        typedef typename alt_list_type::const_iterator iter_type;

        if( 0 != m_peek_chars_begin && 
            ( eos_t<CStringsT>::eval( param, icur ) ||
              m_peek_chars_end == std::find( m_peek_chars_begin, m_peek_chars_end, *icur ) ) )
        {
            return false;
        }

        if( size_t( -1 ) != m_cgroup ) // could be -1 if this is a lookahead_assertion
        {
            IterT & reserved1 = param.m_prgbackrefs[ m_cgroup ].reserved1;
            IterT old_ibegin = reserved1;
            reserved1 = icur;

            for( iter_type ialt = m_rgalternates.begin(); m_rgalternates.end() != ialt; ++ialt )
            {
                if( (*ialt)->recursive_match_all_helper( param, icur, CStringsT() ) )
                    return true;
            }

            reserved1 = old_ibegin;
        }
        else
        {
            for( iter_type ialt = m_rgalternates.begin(); m_rgalternates.end() != ialt; ++ialt )
            {
                if( (*ialt)->recursive_match_all_helper( param, icur, CStringsT() ) )
                    return true;
            }
        }

        return false;
    }
    template< typename CStringsT >
    bool _iterative_match_this( match_param<IterT> & param REGEX_VC6(COMMA CStringsT) ) const
    {
        if( 0 != m_peek_chars_begin && 
            ( eos_t<CStringsT>::eval( param, param.m_icur ) ||
              m_peek_chars_end == std::find( m_peek_chars_begin, m_peek_chars_end, *param.m_icur ) ) )
        {
            return false;
        }

        _push_frame( param );
        param.m_pnext = *m_rgalternates.begin();
        return true;
    }
    bool _iterative_rematch_this( match_param<IterT> & param ) const
    {
        typedef typename alt_list_type::const_iterator iter_type;
        iter_type next_iter = ++param.m_pstack->REGEX_NVC6(template) top REGEX_NVC6(<iter_type>) ( REGEX_VC6(type2type<iter_type>()) );
        if( m_rgalternates.end() != next_iter )
        {
            param.m_pnext = *next_iter;
            return true;
        }
        _pop_frame( param );
        return false;
    }
public:
    typedef typename sub_expr<IterT>::char_type char_type;

    match_group_base( size_t cgroup, regex_arena & arena )
        : m_rgalternates( arena_allocator<sub_expr<IterT>*>( arena ) )
        , m_cgroup( cgroup )
        , m_nwidth( uninit_width() )
        , m_pptail( 0 )
        , m_peek_chars_end( 0 )
    {
    }

    // Derived classes that own the end_group object must have a
    // destructor, and that destructor must call _cleanup().
    virtual ~match_group_base() = 0;

    virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
    {
        return _recursive_match_all REGEX_NVC6(<false_t>) ( param, icur REGEX_VC6(COMMA false_t()) );
    }
    virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const
    {
        return _recursive_match_all REGEX_NVC6(<true_t>) ( param, icur REGEX_VC6(COMMA true_t()) );
    }
    virtual bool iterative_match_this_( match_param<IterT> & param ) const
    {
        return _iterative_match_this REGEX_NVC6(<false_t>) ( param REGEX_VC6(COMMA false_t()) );
    }
    virtual bool iterative_match_this_c( match_param<IterT> & param ) const
    {
        return _iterative_match_this REGEX_NVC6(<true_t>) ( param REGEX_VC6(COMMA true_t()) );
    }
    virtual bool iterative_rematch_this_( match_param<IterT> & param ) const
    {
        return _iterative_rematch_this( param );
    }
    virtual bool iterative_rematch_this_c( match_param<IterT> & param ) const
    {
        return _iterative_rematch_this( param );
    }
    size_t group_number() const
    {
        return m_cgroup;
    }
    void add_item( sub_expr<IterT> * pitem )
    {
        *m_pptail = pitem;
        m_pptail = pitem->pnext();
    }
    void add_alternate()
    {
        m_rgalternates.push_front( 0 );
        m_pptail = &*m_rgalternates.begin();
    }
    void end_alternate()
    {
        *m_pptail = _get_end_group();
    }
    void open_group()
    {
        add_alternate();
    }
    must_have<char_type> close_group( regex_arena & arena )
    {
        end_alternate();
        m_rgalternates.reverse();
        return get_peek_chars( arena );
    }
    must_have<char_type> get_peek_chars( regex_arena & arena )
    {
        m_peek_chars_begin = 0;

        // optimization: find the lookahead characters for each alternate
        size_t total_chars = 0;
        peek_param<char_type> peek;
        typename alt_list_type::const_iterator ialt;
        for( ialt = m_rgalternates.begin(); m_rgalternates.end() != ialt; ++ialt )
        {
            if( ! (*ialt)->peek_this( peek ) )
            {
                peek.m_must_have.m_has = false;
                return peek.m_must_have;
            }
            total_chars += peek.m_cchars;
        }

        arena_allocator<char_type> alloc( arena );
        m_peek_chars_begin = alloc.allocate( total_chars, 0 );
        m_peek_chars_end   = m_peek_chars_begin;

        for( ialt = m_rgalternates.begin(); m_rgalternates.end() != ialt; ++ialt )
        {
            (*ialt)->peek_this( peek );
            char_type const * in = ( peek.m_cchars > 2 ) ? peek.m_pchars : peek.m_rgchars;
            m_peek_chars_end = std::copy( in, in + peek.m_cchars, m_peek_chars_end );
        }

        std::sort( m_peek_chars_begin, m_peek_chars_end );
        m_peek_chars_end = std::unique( m_peek_chars_begin, m_peek_chars_end );

        if( 1 < m_rgalternates.size() )
            peek.m_must_have.m_has = false;

        return peek.m_must_have;
    }
    size_t calternates() const
    {
        return m_rgalternates.size();
    }
    virtual void set_extent( extent_type const & )
    {
    }
    width_type group_width
    (
        std::vector<match_group_base<IterT>*> & rggroups,
        std::list<size_t> const &               invisible_groups
    )
    {
        // This should only be called on the top node
        REGEX_ASSERT( 0 == m_cgroup );
        if( uninit_width() == m_nwidth )
        {
            width_param<IterT> param( rggroups, invisible_groups );
            match_group_base<IterT>::width_this( param );
        }
        return m_nwidth;
    }
    virtual width_type width_this( width_param<IterT> & param )
    {
        typedef typename alt_list_type::const_iterator iter_type;
        width_type width = { size_t( -1 ), 0 };
        for( iter_type ialt = m_rgalternates.begin(); worst_width != width && m_rgalternates.end() != ialt; ++ialt )
        {
            // prevent possible infinite recursion
            if( m_cgroup < param.m_rggroups.size() )
                param.m_rggroups[ m_cgroup ] = 0;

            width_type temp_width = ( *ialt )->get_width( param );

            if( m_cgroup < param.m_rggroups.size() )
                param.m_rggroups[ m_cgroup ] = this;

            width.m_min = regex_min( width.m_min, temp_width.m_min );
            width.m_max = regex_max( width.m_max, temp_width.m_max );
        }
        return m_nwidth = width;
    }
    virtual bool peek_this( peek_param<char_type> & peek ) const
    {
        if( 0 == m_peek_chars_begin )
            return false;

        peek.m_cchars = std::distance( m_peek_chars_begin, m_peek_chars_end );
        if( 2 < peek.m_cchars )
            peek.m_pchars = m_peek_chars_begin;
        else
            std::copy( m_peek_chars_begin, m_peek_chars_end, peek.m_rgchars );

        peek.m_must_have.m_has = false;
        if( 1 == m_rgalternates.size() )
        {
            peek_param<char_type> local_peek;
            (*m_rgalternates.begin())->peek_this( local_peek );
            peek.m_must_have = local_peek.m_must_have;
        }

        return true;
    }

protected:
    void _cleanup()
    {
        typedef typename alt_list_type::const_iterator iter_type;
        for( iter_type ialt = m_rgalternates.begin(); m_rgalternates.end() != ialt; ++ialt )
            delete *ialt;
        m_rgalternates.clear();
    }

    virtual sub_expr<IterT> * _get_end_group() = 0;

    alt_list_type           m_rgalternates;
    size_t const            m_cgroup;
    width_type              m_nwidth;
    
    union
    {
        sub_expr<IterT>  ** m_pptail; // only used when adding elements
        char_type         * m_peek_chars_begin;
    };

    char_type             * m_peek_chars_end;
};

template< typename IterT >
inline match_group_base<IterT>::~match_group_base()
{
}

// A indestructable_sub_expr is an object that brings itself back
// to life after explicitly being deleted.  It is used
// to ease clean-up of the sub_expr graph, where most
// nodes are dynamically allocated, but some nodes are
// members of other nodes and are not dynamically allocated.
// The recursive delete of the sub_expr graph causes
// delete to be ( incorrectly ) called on these members.
// By inheriting these members from indestructable_sub_expr,
// explicit attempts to delete the object will have no
// effect. ( Actually, the object will be destructed and
// then immediately reconstructed. ) This is accomplished
// by calling placement new in operator delete.
template< typename IterT, typename T >
class indestructable_sub_expr : public sub_expr<IterT>
{
    static void * operator new( size_t, regex_arena & );
    static void operator delete( void *, regex_arena & );
protected:
    static void * operator new( size_t, void * pv ) { return pv; }
    static void operator delete( void *, void * ) {}
public:
    virtual ~indestructable_sub_expr() {}
    static void operator delete( void * pv ) { ::new( pv ) T; }
};

template< typename IterT >
class match_group : public match_group_base<IterT>
{
    match_group( match_group const & );
    match_group & operator=( match_group const & );
public:
    match_group( size_t cgroup, regex_arena & arena )
        : match_group_base<IterT>( cgroup, arena )
        , m_end_group( this )
    {
    }

    virtual ~match_group()
    {
        this->_cleanup();
    }

    virtual sub_expr<IterT> * quantify( size_t lbound, size_t ubound, bool greedy, regex_arena & arena )
    {
        if( greedy )
            return new( arena ) max_group_quantifier<IterT>( this, lbound, ubound );
        else
            return new( arena ) min_group_quantifier<IterT>( this, lbound, ubound );
    }

protected:
    typedef typename match_group_base<IterT>::alt_list_type alt_list_type;

    struct old_backref
    {
        IterT m_ibegin;
        IterT m_iend;
        bool  m_matched;

        old_backref() {}
        old_backref( backref_tag<IterT> const & br )
            : m_ibegin( br.first )
            , m_iend( br.second )
            , m_matched( br.matched )
        {
        }
    };

    static void restore_backref( backref_tag<IterT> & br, old_backref const & old_br )
    {
        br.first   = old_br.m_ibegin;
        br.second  = old_br.m_iend;
        br.matched = old_br.m_matched;
    }

    template< typename CStringsT >
    bool _call_back( match_param<IterT> & param, IterT icur REGEX_VC6(COMMA CStringsT) ) const
    {
        if( size_t( -1 ) != this->m_cgroup )
        {
            backref_tag<IterT> & br = param.m_prgbackrefs[ this->m_cgroup ];

            // Save the relevant portions of the backref in an old_backref struct
            old_backref old_br( br );

            br.first   = br.reserved1;
            br.second  = icur;
            br.matched = true;

            if( this->recursive_match_next_( param, icur, CStringsT() ) )
                return true;

            // Restore the backref to its saved state
            restore_backref( br, old_br );
        }
        else
        {
            if( this->recursive_match_next_( param, icur, CStringsT() ) )
                return true;
        }

        return false;
    }

    class end_group : public indestructable_sub_expr<IterT, end_group>
    {
        match_group<IterT> const *const m_pgroup;

        end_group & operator=( end_group const & );

        void _push_frame( match_param<IterT> & param ) const
        {
            size_t cgroup = m_pgroup->group_number();

            if( size_t( -1 ) != cgroup )
            {
                backref_tag<IterT> & br = param.m_prgbackrefs[ cgroup ];
                old_backref old_br( br );
                param.m_pstack->push( old_br );

                br.first   = br.reserved1;
                br.second  = param.m_icur;
                br.matched = true;
            }
        }
        void _pop_frame( match_param<IterT> & param ) const
        {
            size_t cgroup = m_pgroup->group_number();

            if( size_t( -1 ) != cgroup )
            {
                old_backref old_br;
                param.m_pstack->pop( old_br );
                match_group<IterT>::restore_backref( param.m_prgbackrefs[ cgroup ], old_br );
            }
        }
        bool _iterative_match_this( match_param<IterT> & param ) const
        {
            _push_frame( param );
            param.m_pnext = m_pgroup->next();
            return true;
        }
        bool _iterative_rematch_this( match_param<IterT> & param ) const
        {
            _pop_frame( param );
            return false;
        }
    public:
        end_group( match_group<IterT> const * pgroup = 0 )
            : m_pgroup( pgroup )
        {
        }
        virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
        {
            return m_pgroup->REGEX_NVC6(template) _call_back REGEX_NVC6(<false_t>)( param, icur REGEX_VC6(COMMA false_t()) );
        }
        virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const
        {
            return m_pgroup->REGEX_NVC6(template) _call_back REGEX_NVC6(<true_t>)( param, icur REGEX_VC6(COMMA true_t()) );
        }
        virtual bool iterative_match_this_( match_param<IterT> & param ) const
        {
            return _iterative_match_this( param );
        }
        virtual bool iterative_match_this_c( match_param<IterT> & param ) const
        {
            return _iterative_match_this( param );
        }
        virtual bool iterative_rematch_this_( match_param<IterT> & param ) const
        {
            return _iterative_rematch_this( param );
        }
        virtual bool iterative_rematch_this_c( match_param<IterT> & param ) const
        {
            return _iterative_rematch_this( param );
        }
        virtual width_type width_this( width_param<IterT> & )
        {
            return zero_width;
        }
    } m_end_group;

    friend class end_group;

    virtual sub_expr<IterT> * _get_end_group()
    {
        return & m_end_group;
    }
};

template< typename IterT >
inline void save_backrefs( backref_tag<IterT> const * ibegin, backref_tag<IterT> const * iend, IterT * prgci )
{
    for( ; ibegin != iend; ++ibegin, ++prgci )
    {
        new( static_cast<void*>( prgci ) ) IterT( ibegin->reserved1 );
    }
}

template< typename IterT >
inline void restore_backrefs( backref_tag<IterT> * ibegin, backref_tag<IterT> * iend, IterT const * prgci )
{
    for( ; ibegin != iend; ++ibegin, ++prgci )
    {
        ibegin->reserved1 = *prgci;
    }
}

template< typename IterT >
class group_wrapper : public sub_expr<IterT>
{
    match_group_base<IterT> const *const m_pgroup;

    group_wrapper & operator=( group_wrapper const & );
public:
    group_wrapper( match_group_base<IterT> const * pgroup )
        : m_pgroup( pgroup )
    {
    }
    virtual bool iterative_match_this_( match_param<IterT> & param ) const
    {
        return m_pgroup->match_group_base<IterT>::iterative_match_this_( param );
    }
    virtual bool iterative_match_this_c( match_param<IterT> & param ) const
    {
        return m_pgroup->match_group_base<IterT>::iterative_match_this_c( param );
    }
    virtual bool iterative_rematch_this_( match_param<IterT> & param ) const
    {
        return m_pgroup->match_group_base<IterT>::iterative_rematch_this_( param );
    }
    virtual bool iterative_rematch_this_c( match_param<IterT> & param ) const
    {
        return m_pgroup->match_group_base<IterT>::iterative_rematch_this_c( param );
    }
    virtual width_type width_this( width_param<IterT> & )
    {
        return zero_width;
    }
};

struct deleter
{
    template< typename T >
    void operator()( T const & t )
    {
        t.T::~T();
    }
};

// Behaves like a lookahead assertion if m_cgroup is -1, or like
// an independent group otherwise.
template< typename IterT >
class independent_group_base : public match_group_base<IterT>
{
    independent_group_base( independent_group_base const & );
    independent_group_base & operator=( independent_group_base const & );

    template< typename CStringsT >
    bool _recursive_match_all( match_param<IterT> & param, IterT icur REGEX_VC6(COMMA CStringsT) ) const
    {
        backref_tag<IterT> * prgbr = 0;

        // Copy onto the stack the part of the backref vector that could
        // be modified by the lookahead.
        if( m_extent.second )
        {
            prgbr = static_cast<backref_tag<IterT>*>( alloca( m_extent.second * sizeof( backref_tag<IterT> ) ) );
            std::uninitialized_copy(
                param.m_prgbackrefs + m_extent.first,
                param.m_prgbackrefs + m_extent.first + m_extent.second,
                prgbr );
        }

        // Match until the end of this group and then return
        // BUGBUG can the compiler optimize this?
        bool const fdomatch = CStringsT::value ?
            match_group_base<IterT>::recursive_match_all_c( param, icur ) :
            match_group_base<IterT>::recursive_match_all_( param, icur );

        if( m_fexpected == fdomatch )
        {
            // If m_cgroup != 1, then this is not a zero-width assertion.
            if( fdomatch && size_t( -1 ) != this->m_cgroup )
                icur = param.m_prgbackrefs[ this->m_cgroup ].second;

            if( this->recursive_match_next_( param, icur, CStringsT() ) )
            {
                std::for_each( prgbr, prgbr + m_extent.second, deleter() );
                return true;
            }
        }

        // if match_group::recursive_match_all returned true, the backrefs must be restored
        if( m_extent.second && fdomatch )
            std::copy( prgbr, prgbr + m_extent.second, param.m_prgbackrefs + m_extent.first );

        std::for_each( prgbr, prgbr + m_extent.second, deleter() );
        return false;
    }
    template< typename CStringsT >
    bool _iterative_match_this( match_param<IterT> & param REGEX_VC6(COMMA CStringsT) ) const
    {
        group_wrapper<IterT> expr( this );

        _push_frame( param );
        IterT ibegin = param.m_icur;

        bool const fdomatch = _do_match_iterative( &expr, param, param.m_icur, CStringsT() );

        if( m_fexpected == fdomatch )
        {
            // If m_cgroup == -1, then this is a zero-width assertion.
            if( fdomatch && size_t( -1 ) == this->m_cgroup )
                param.m_icur = ibegin;

            param.m_pnext = this->next();
            return true;
        }

        _pop_frame( param );
        return false;
    }
    bool _iterative_rematch_this( match_param<IterT> & param ) const
    {
        _pop_frame( param );
        return false;
    }
public:
    independent_group_base( size_t cgroup, regex_arena & arena )
        : match_group_base<IterT>( cgroup, arena )
        , m_fexpected( true )
        , m_extent( 0, 0 )
    {
    }
    virtual void set_extent( extent_type const & ex )
    {
        m_extent = ex;
    }
    virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
    {
        return _recursive_match_all REGEX_NVC6(<false_t>) ( param, icur REGEX_VC6(COMMA false_t()) );
    }
    virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const
    {
        return _recursive_match_all REGEX_NVC6(<true_t>) ( param, icur REGEX_VC6(COMMA true_t()) );
    }
    virtual bool iterative_match_this_( match_param<IterT> & param ) const
    {
        return _iterative_match_this REGEX_NVC6(<false_t>) ( param REGEX_VC6(COMMA false_t()) );
    }
    virtual bool iterative_match_this_c( match_param<IterT> & param ) const
    {
        return _iterative_match_this REGEX_NVC6(<true_t>) ( param REGEX_VC6(COMMA true_t()) );
    }
    virtual bool iterative_rematch_this_( match_param<IterT> & param ) const
    {
        return _iterative_rematch_this( param );
    }
    virtual bool iterative_rematch_this_c( match_param<IterT> & param ) const
    {
        return _iterative_rematch_this( param );
    }
    virtual bool peek_this( peek_param<char_type> & peek ) const
    {
        if( size_t( -1 ) == this->m_cgroup )
            return false;
        return match_group_base<IterT>::peek_this( peek );
    }
protected:

    void _push_frame( match_param<IterT> & param ) const
    {
        unsafe_stack * pstack = param.m_pstack;
        typedef typename match_param<IterT>::backref_type backref_type;
        backref_type * ibegin = param.m_prgbackrefs + m_extent.first;
        backref_type * iend   = ibegin + m_extent.second;

        for( ; iend != ibegin; ++ibegin )
        {
            pstack->push( *ibegin );
        }
        pstack->push( param.m_icur );
    }

    void _pop_frame( match_param<IterT> & param ) const
    {
        unsafe_stack * pstack = param.m_pstack;
        typedef typename match_param<IterT>::backref_type backref_type;

        backref_type * ibegin = param.m_prgbackrefs + m_extent.first;
        backref_type * iend   = ibegin + m_extent.second;

        pstack->pop( param.m_icur );
        while( iend != ibegin )
        {
            pstack->pop( *--iend );
        }
    }

    independent_group_base( bool const fexpected, regex_arena & arena )
        : match_group_base<IterT>( size_t( -1 ), arena )
        , m_fexpected( fexpected )
    {
    }

    bool const  m_fexpected;
    extent_type m_extent;
};

template< typename IterT >
class independent_group : public independent_group_base<IterT>
{
    independent_group( independent_group const & );
    independent_group & operator=( independent_group const & );
public:
    independent_group( size_t cgroup, regex_arena & arena )
        : independent_group_base<IterT>( cgroup, arena )
        , m_end_group( this )
    {
    }

    virtual ~independent_group()
    {
        this->_cleanup();
    }

    virtual sub_expr<IterT> * quantify( size_t lbound, size_t ubound, bool greedy, regex_arena & arena )
    {
        if( greedy )
            return new( arena ) max_group_quantifier<IterT>( this, lbound, ubound );
        else
            return new( arena ) min_group_quantifier<IterT>( this, lbound, ubound );
    }

protected:
    independent_group( bool const fexpected, regex_arena & arena )
        : independent_group_base<IterT>( fexpected, arena )
        , m_end_group( this )
    {
    }

    bool _call_back( match_param<IterT> & param, IterT icur ) const
    {
        if( size_t( -1 ) != this->m_cgroup )
        {
            backref_tag<IterT> & br = param.m_prgbackrefs[ this->m_cgroup ];
            br.first   = br.reserved1;
            br.second  = icur;
            br.matched = true;
        }
        return true;
    }

    class end_group : public indestructable_sub_expr<IterT, end_group>
    {
        independent_group<IterT> const *const m_pgroup;

        end_group & operator=( end_group const & );
        bool _iterative_match_this( match_param<IterT> & param ) const
        {
            size_t cgroup = m_pgroup->group_number();
            if( size_t( -1 ) != cgroup )
            {
                backref_tag<IterT> & br = param.m_prgbackrefs[ cgroup ];
                br.first   = br.reserved1;
                br.second  = param.m_icur;
                br.matched = true;
            }
            param.m_pnext = 0;
            return true;
        }
    public:
        end_group( independent_group<IterT> const * pgroup = 0 )
            : m_pgroup( pgroup )
        {
        }
        virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
        {
            return m_pgroup->_call_back( param, icur );
        }
        virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const
        {
            return m_pgroup->_call_back( param, icur );
        }
        virtual bool iterative_match_this_( match_param<IterT> & param ) const
        {
            return _iterative_match_this( param );
        }
        virtual bool iterative_match_this_c( match_param<IterT> & param ) const
        {
            return _iterative_match_this( param );
        }
        virtual width_type width_this( width_param<IterT> & )
        {
            return zero_width;
        }
    } m_end_group;

    friend class end_group;

    virtual sub_expr<IterT> * _get_end_group()
    {
        return & m_end_group;
    }
};

template< typename IterT >
class lookahead_assertion : public independent_group<IterT>
{
    lookahead_assertion( lookahead_assertion const & );
    lookahead_assertion & operator=( lookahead_assertion const & );
public:
    lookahead_assertion( bool const fexpected, regex_arena & arena )
        : independent_group<IterT>( fexpected, arena )
    {
    }
    virtual sub_expr<IterT> * quantify( size_t, size_t, bool, regex_arena & )
    {
        throw bad_regexpr( "look-ahead assertion cannot be quantified" );
    }
    virtual bool is_assertion() const
    {
        return true;
    }
    virtual width_type width_this( width_param<IterT> & param )
    {
        // calculate the group's width and store it, but return zero_width
        match_group_base<IterT>::width_this( param );
        return zero_width;
    }
    virtual bool peek_this( peek_param<char_type> & peek ) const
    {
        return this->next()->peek_this( peek );
    }
};

template< typename IterT >
class lookbehind_assertion : public independent_group_base<IterT>
{
    lookbehind_assertion( lookbehind_assertion const & );
    lookbehind_assertion & operator=( lookbehind_assertion const & );

    template< typename CStringsT >
    bool _recursive_match_all( match_param<IterT> & param, IterT icur REGEX_VC6(COMMA CStringsT) ) const
    {
        typedef typename std::iterator_traits<IterT>::difference_type diff_type;

        // This is the room in the string from the start to the current position
        diff_type room = std::distance( param.m_ibufferbegin, icur );

        // If we don't have enough room to match the lookbehind, the match fails.
        // If we wanted the match to fail, try to match the rest of the pattern.
        if( this->m_nwidth.m_min > static_cast<size_t>( room ) )
            return this->m_fexpected ? false : this->recursive_match_next_( param, icur, CStringsT() );

        backref_tag<IterT> * prgbr = 0;

        // Copy onto the stack the part of the backref vector that could
        // be modified by the lookbehind.
        if( this->m_extent.second )
        {
            prgbr = static_cast<backref_tag<IterT>*>( alloca( this->m_extent.second * sizeof( backref_tag<IterT> ) ) );
            std::uninitialized_copy(
                param.m_prgbackrefs + this->m_extent.first,
                param.m_prgbackrefs + this->m_extent.first + this->m_extent.second,
                prgbr );
        }

        IterT local_ibegin  = icur;
        std::advance( local_ibegin, -static_cast<diff_type>( regex_min<size_t>( this->m_nwidth.m_max, room ) ) );

        IterT local_iend = icur;
        std::advance( local_iend, -static_cast<diff_type>( this->m_nwidth.m_min ) );

        // Create a local param struct that has icur as param.m_iend
        match_param<IterT> local_param( param.m_ibufferbegin, param.m_imatchbegin, icur, param.m_prgbackrefs, param.m_cbackrefs );

        // Find the rightmost match that ends at icur.
        for( IterT local_icur = local_ibegin; ; ++local_icur )
        {
            // Match until the end of this group and then return
            // Note that we're calling recursive_match_all_ regardless of the CStringsT switch.
            // This is because for the lookbehind assertion, the termination condition is when
            // icur == param.m_iend, not when *icur == '\0'
            bool const fmatched = match_group_base<IterT>::recursive_match_all_( local_param, local_icur );

            // If the match results were what we were expecting, try to match the
            // rest of the pattern. If that succeeds, return true.
            if( this->m_fexpected == fmatched && this->recursive_match_next_( param, icur, CStringsT() ) )
            {
                std::for_each( prgbr, prgbr + this->m_extent.second, deleter() );
                return true;
            }

            // if match_group::recursive_match_all returned true, the backrefs must be restored
            if( fmatched )
            {
                if( this->m_extent.second )
                    std::copy( prgbr, prgbr + this->m_extent.second, param.m_prgbackrefs + this->m_extent.first );

                // Match succeeded. If this is a negative lookbehind, we didn't want it
                // to succeed, so return false.
                if( ! this->m_fexpected )
                {
                    std::for_each( prgbr, prgbr + this->m_extent.second, deleter() );
                    return false;
                }
            }

            if( local_icur == local_iend )
                break;
        }

        // No variation of the lookbehind was satisfied in a way that permited
        // the rest of the pattern to match successfully, so return false.
        std::for_each( prgbr, prgbr + this->m_extent.second, deleter() );
        return false;
    }

    template< typename CStringsT >
    bool _iterative_match_this( match_param<IterT> & param REGEX_VC6(COMMA CStringsT) ) const
    {
        typedef typename std::iterator_traits<IterT>::difference_type diff_type;

        // Save the backrefs
        this->_push_frame( param );

        // This is the room in the string from the start to the current position
        diff_type room = std::distance( param.m_ibufferbegin, param.m_icur );

        // If we don't have enough room to match the lookbehind, the match fails.
        // If we wanted the match to fail, try to match the rest of the pattern.
        if( this->m_nwidth.m_min > static_cast<size_t>( room ) )
        {
            if( this->m_fexpected )
            {
                this->_pop_frame( param );
                return false;
            }
            param.m_pnext = this->next();
            return true;
        }

        IterT local_ibegin  = param.m_icur;
        std::advance( local_ibegin, -static_cast<diff_type>( regex_min<size_t>( this->m_nwidth.m_max, room ) ) );

        IterT local_iend = param.m_icur;
        std::advance( local_iend, -static_cast<diff_type>( this->m_nwidth.m_min ) );

        // Create a local param struct that has icur as param.m_iend
        match_param<IterT> local_param( param.m_ibufferbegin, param.m_imatchbegin, param.m_icur, param.m_prgbackrefs, param.m_cbackrefs );
        local_param.m_pstack = param.m_pstack;

        group_wrapper<IterT> expr( this );

        // Find the rightmost match that ends at icur.
        for( IterT local_icur = local_ibegin; ; ++local_icur )
        {
            // Match until the end of this group and then return
            // Note that we're calling _do_match_iterative_helper regardless of the CStringsT switch.
            // This is because for the lookbehind assertion, the termination condition is when
            // icur == param.m_iend, not when *icur == '\0'
            bool const fmatched = matcher_helper<IterT>::_do_match_iterative_helper( &expr, local_param, local_icur );

            // If the match results were what we were expecting, try to match the
            // rest of the pattern. If that succeeds, return true.
            if( this->m_fexpected == fmatched )
            {
                param.m_pnext = this->next();
                return true;
            }

            // if match_group::recursive_match_all returned true, the backrefs must be restored
            if( fmatched )
            {
                // Restore the backrefs
                this->_pop_frame( param );

                // Match succeeded. If this is a negative lookbehind, we didn't want it
                // to succeed, so return false.
                if( ! this->m_fexpected )
                    return false;

                // Save the backrefs again.
                this->_push_frame( param );
            }

            if( local_icur == local_iend )
                break;
        }

        // No variation of the lookbehind was satisfied in a way that permited
        // the rest of the pattern to match successfully, so return false.
        this->_pop_frame( param );
        return false;
    }
    bool _iterative_rematch_this( match_param<IterT> & param ) const
    {
        this->_pop_frame( param );
        return false;
    }
public:
    lookbehind_assertion( bool const fexpected, regex_arena & arena )
        : independent_group_base<IterT>( fexpected, arena )
    {
    }

    virtual ~lookbehind_assertion()
    {
        this->_cleanup();
    }

    virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
    {
        return _recursive_match_all REGEX_NVC6(<false_t>) ( param, icur REGEX_VC6(COMMA false_t()) );
    }
    virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const
    {
        return _recursive_match_all REGEX_NVC6(<true_t>) ( param, icur REGEX_VC6(COMMA true_t()) );
    }
    virtual bool iterative_match_this_( match_param<IterT> & param ) const
    {
        return _iterative_match_this REGEX_NVC6(<false_t>) ( param REGEX_VC6(COMMA false_t()) );
    }
    virtual bool iterative_match_this_c( match_param<IterT> & param ) const
    {
        return _iterative_match_this REGEX_NVC6(<true_t>) ( param REGEX_VC6(COMMA true_t()) );
    }
    virtual bool iterative_rematch_this_( match_param<IterT> & param ) const
    {
        return _iterative_rematch_this( param );
    }
    virtual bool iterative_rematch_this_c( match_param<IterT> & param ) const
    {
        return _iterative_rematch_this( param );
    }

    virtual bool is_assertion() const
    {
        return true;
    }
    virtual width_type width_this( width_param<IterT> & param )
    {
        // calculate the group's width and store it, but return zero_width
        match_group_base<IterT>::width_this( param );
        return zero_width;
    }
    virtual bool peek_this( peek_param<char_type> & peek ) const
    {
        return this->next()->peek_this( peek );
    }

protected:
    struct end_group : public indestructable_sub_expr<IterT, end_group>
    {
        virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
        {
            return param.m_iend == icur;
        }
        virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const
        {
            return param.m_iend == icur;
        }
        virtual bool iterative_match_this_( match_param<IterT> & param ) const
        {
            param.m_pnext = 0;
            return param.m_iend == param.m_icur;
        }
        virtual bool iterative_match_this_c( match_param<IterT> & param ) const
        {
            param.m_pnext = 0;
            return param.m_iend == param.m_icur;
        }
        virtual width_type width_this( width_param<IterT> & )
        {
            return zero_width;
        }
    } m_end_group;

    virtual sub_expr<IterT> * _get_end_group()
    {
        return & m_end_group;
    }
};

template< typename IterT >
class group_quantifier : public match_quantifier<IterT>
{
    group_quantifier & operator=( group_quantifier const & );

    bool _iterative_match_this( match_param<IterT> & param ) const
    {
        _push_frame( param );
        param.m_pnext = this->m_psub->next(); // ptr to end_quant
        return true;
    }
    bool _iterative_rematch_this( match_param<IterT> & param ) const
    {
        _pop_frame( param );
        return false;
    }
public:
    group_quantifier
    (
        match_group_base<IterT> * psub,
        size_t lbound,
        size_t ubound,
        sub_expr<IterT> * pend_quant
    )
        : match_quantifier<IterT>( psub, lbound, ubound )
        , m_group( *psub )
    {
        *psub->pnext() = pend_quant;
    }

    // sub-classes of group_quantifer that own the end_quant
    // object must declare a destructor, and it must call _cleanup
    virtual ~group_quantifier() = 0;

    virtual bool iterative_match_this_( match_param<IterT> & param ) const
    {
        return _iterative_match_this( param );
    }
    virtual bool iterative_match_this_c( match_param<IterT> & param ) const
    {
        return _iterative_match_this( param );
    }
    virtual bool iterative_rematch_this_( match_param<IterT> & param ) const
    {
        return _iterative_rematch_this( param );
    }
    virtual bool iterative_rematch_this_c( match_param<IterT> & param ) const
    {
        return _iterative_rematch_this( param );
    }

protected:
    struct old_quant
    {
        typedef typename backref_tag<IterT>::smart_iter_type smart_iter_type;

        size_t          reserved2;
        bool            reserved3;
        smart_iter_type reserved4;
        smart_iter_type reserved5;

        old_quant()
        {
        }
        old_quant( backref_tag<IterT> const & br )
            : reserved2( br.reserved2 )
            , reserved3( br.reserved3 )
            , reserved4( br.reserved4 )
            , reserved5( br.reserved5 )
        {
        }
    };

    void _push_frame( match_param<IterT> & param ) const
    {
        typedef typename backref_tag<IterT>::smart_iter_type smart_iter_type;

        backref_tag<IterT> & br = param.m_prgbackrefs[ group_number() ];
        old_quant old_qt( br );
        param.m_pstack->push( old_qt );

        br.reserved2 = 0;    // nbr of times this group has matched
        br.reserved3 = true; // toggle used for backtracking
        br.reserved4 = static_init<smart_iter_type>::value;
        br.reserved5 = static_init<smart_iter_type>::value;
    }

    void _pop_frame( match_param<IterT> & param ) const
    {
        backref_tag<IterT> & br = param.m_prgbackrefs[ group_number() ];
        old_quant old_qt;
        param.m_pstack->pop( old_qt );

        br.reserved2 = old_qt.reserved2;
        br.reserved3 = old_qt.reserved3;
        br.reserved4 = old_qt.reserved4;
        br.reserved5 = old_qt.reserved5;
    }

    size_t group_number() const
    {
        return m_group.group_number();
    }

    size_t & cmatches( match_param<IterT> & param ) const
    {
        return param.m_prgbackrefs[ group_number() ].reserved2;
    }

    typename backref_tag<IterT>::smart_iter_type & highwater1( match_param<IterT> & param ) const
    {
        return param.m_prgbackrefs[ group_number() ].reserved4;
    }

    typename backref_tag<IterT>::smart_iter_type & highwater2( match_param<IterT> & param ) const
    {
        return param.m_prgbackrefs[ group_number() ].reserved5;
    }

    match_group_base<IterT> const & m_group;
};

template< typename IterT >
inline group_quantifier<IterT>::~group_quantifier()
{
}

template< typename IterT >
class max_group_quantifier : public group_quantifier<IterT>
{
    max_group_quantifier & operator=( max_group_quantifier const & );

    template< typename CStringsT >
    bool _recursive_match_all( match_param<IterT> & param, IterT icur REGEX_VC6(COMMA CStringsT) ) const
    {
        typedef typename backref_tag<IterT>::smart_iter_type smart_iter_type;

        smart_iter_type old_highwater1 = this->highwater1( param );
        smart_iter_type old_highwater2 = this->highwater2( param );
        size_t          old_cmatches   = this->cmatches( param );

        this->highwater1( param ) = static_init<smart_iter_type>::value;
        this->highwater2( param ) = icur;
        this->cmatches( param )   = 0;

        if( _recurse REGEX_NVC6(<CStringsT>) ( param, icur REGEX_VC6(COMMA CStringsT()) ) )
            return true;

        this->cmatches( param )   = old_cmatches;
        this->highwater2( param ) = old_highwater2;
        this->highwater1( param ) = old_highwater1;

        return false;
    }
public:
    max_group_quantifier( match_group_base<IterT> * psub, size_t lbound, size_t ubound )
        : group_quantifier<IterT>( psub, lbound, ubound, & m_end_quant )
        , m_end_quant( this )
    {
    }

    virtual ~max_group_quantifier()
    {
        // Must call _cleanup() here before the end_quant object
        // gets destroyed.
        this->_cleanup();
    }

    virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
    {
        return _recursive_match_all REGEX_NVC6(<false_t>) ( param, icur REGEX_VC6(COMMA false_t()) );
    }
    virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const
    {
        return _recursive_match_all REGEX_NVC6(<true_t>) ( param, icur REGEX_VC6(COMMA true_t()) );
    }

protected:
    template< typename CStringsT >
    bool _recurse( match_param<IterT> & param, IterT icur REGEX_VC6(COMMA CStringsT) ) const
    {
        if( this->m_ubound == this->cmatches( param ) )
            return this->recursive_match_next_( param, icur, CStringsT() );

        ++this->cmatches( param );
        if( this->m_psub->recursive_match_all_helper( param, icur, CStringsT() ) )
            return true;

        if( --this->cmatches( param ) < this->m_lbound )
            return false;

        return this->recursive_match_next_( param, icur, CStringsT() );
    }

    class end_quantifier : public indestructable_sub_expr<IterT, end_quantifier>
    {
        max_group_quantifier<IterT> const *const m_pquant;

        end_quantifier & operator=( end_quantifier const & );

        void _push_frame( match_param<IterT> & param ) const
        {
            backref_tag<IterT> & br = param.m_prgbackrefs[ m_pquant->group_number() ];
            param.m_pstack->push( br.reserved4 );
            br.reserved4 = br.reserved5;
            br.reserved5 = param.m_icur;
        }

        void _pop_frame( match_param<IterT> & param ) const
        {
            backref_tag<IterT> & br = param.m_prgbackrefs[ m_pquant->group_number() ];
            br.reserved5 = br.reserved4;
            param.m_pstack->pop( br.reserved4 );
        }

        template< typename CStringsT >
        bool _recursive_match_all( match_param<IterT> & param, IterT icur REGEX_VC6(COMMA CStringsT) ) const
        {
            typedef typename backref_tag<IterT>::smart_iter_type smart_iter_type;
            smart_iter_type old_highwater1 = m_pquant->highwater1( param );

            if( icur == old_highwater1 )
                return m_pquant->recursive_match_next_( param, icur, CStringsT() );

            m_pquant->highwater1( param ) = m_pquant->highwater2( param );
            m_pquant->highwater2( param ) = icur;

            if( m_pquant->REGEX_NVC6(template) _recurse REGEX_NVC6(<CStringsT>) ( param, icur REGEX_VC6(COMMA CStringsT()) ) )
                return true;

            m_pquant->highwater2( param ) = m_pquant->highwater1( param );
            m_pquant->highwater1( param ) = old_highwater1;

            return false;
        }
        bool _iterative_match_this( match_param<IterT> & param ) const
        {
            backref_tag<IterT> & br = param.m_prgbackrefs[ m_pquant->group_number() ];

            // forcibly break the infinite loop
            if( param.m_icur == br.reserved4 )
            {
                _push_frame( param );
                param.m_pnext = m_pquant->next();
                return true;
            }

            _push_frame( param );

            // If we've matched the max nbr of times, move on to the next
            // sub-expr.
            if( m_pquant->m_ubound == br.reserved2 )
            {
                param.m_pnext = m_pquant->next();
                br.reserved3 = false;
                return true;
            }

            // Rematch the group.
            br.reserved3 = true;
            param.m_pnext = m_pquant->m_psub;
            ++br.reserved2;
            return true;
        }
        bool _iterative_rematch_this( match_param<IterT> & param ) const
        {
            typedef typename backref_tag<IterT>::smart_iter_type smart_iter_type;

            backref_tag<IterT> & br = param.m_prgbackrefs[ m_pquant->group_number() ];

            // infinite loop forcibly broken
            if( param.m_icur == param.m_pstack->REGEX_NVC6(template) top REGEX_NVC6(<smart_iter_type>) ( REGEX_VC6(type2type<smart_iter_type>()) ) )
            {
                _pop_frame( param );
                return false;
            }

            if( br.reserved3 )
            {
                --br.reserved2;
                param.m_pnext = m_pquant->next();
                if( m_pquant->m_lbound <= br.reserved2 )
                {
                    br.reserved3 = false;
                    return true;
                }
                _pop_frame( param );
                return false;
            }
            br.reserved3 = true;
            _pop_frame( param );
            return false;
        }
    public:
        end_quantifier( max_group_quantifier<IterT> const * pquant = 0 )
            : m_pquant( pquant )
        {
        }
        virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
        {
            return _recursive_match_all REGEX_NVC6(<false_t>) ( param, icur REGEX_VC6(COMMA false_t()) );
        }
        virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const
        {
            return _recursive_match_all REGEX_NVC6(<true_t>) ( param, icur REGEX_VC6(COMMA true_t()) );
        }
        virtual bool iterative_match_this_( match_param<IterT> & param ) const
        {
            return _iterative_match_this( param );
        }
        virtual bool iterative_match_this_c( match_param<IterT> & param ) const
        {
            return _iterative_match_this( param );
        }
        virtual bool iterative_rematch_this_( match_param<IterT> & param ) const
        {
            return _iterative_rematch_this( param );
        }
        virtual bool iterative_rematch_this_c( match_param<IterT> & param ) const
        {
            return _iterative_rematch_this( param );
        }
        virtual width_type width_this( width_param<IterT> & )
        {
            return zero_width;
        }
    } m_end_quant;

    friend class end_quantifier;
};

template< typename IterT >
class min_group_quantifier : public group_quantifier<IterT>
{
    min_group_quantifier & operator=( min_group_quantifier const & );

    template< typename CStringsT >
    bool _recursive_match_all( match_param<IterT> & param, IterT icur REGEX_VC6(COMMA CStringsT) ) const
    {
        typedef typename backref_tag<IterT>::smart_iter_type smart_iter_type;

        smart_iter_type old_highwater1 = this->highwater1( param );
        smart_iter_type old_highwater2 = this->highwater2( param );
        size_t          old_cmatches   = this->cmatches( param );

        this->highwater1( param ) = static_init<smart_iter_type>::value;
        this->highwater2( param ) = icur;
        this->cmatches( param )   = 0;

        if( _recurse REGEX_NVC6(<CStringsT>) ( param, icur REGEX_VC6(COMMA CStringsT()) ) )
            return true;

        this->cmatches( param )   = old_cmatches;
        this->highwater2( param ) = old_highwater2;
        this->highwater1( param ) = old_highwater1;

        return false;
    }
public:
    min_group_quantifier( match_group_base<IterT> * psub, size_t lbound, size_t ubound )
        : group_quantifier<IterT>( psub, lbound, ubound, & m_end_quant )
        , m_end_quant( this )
    {
    }

    virtual ~min_group_quantifier()
    {
        // Must call _cleanup() here before the end_quant object
        // gets destroyed.
        this->_cleanup();
    }

    virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
    {
        return _recursive_match_all REGEX_NVC6(<false_t>) ( param, icur REGEX_VC6(COMMA false_t()) );
    }
    virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const
    {
        return _recursive_match_all REGEX_NVC6(<true_t>) ( param, icur REGEX_VC6(COMMA true_t()) );
    }

protected:

    template< typename CStringsT >
    bool _recurse( match_param<IterT> & param, IterT icur REGEX_VC6(COMMA CStringsT) ) const
    {
        if( this->m_lbound <= this->cmatches( param ) )
        {
            if( this->recursive_match_next_( param, icur, CStringsT() ) )
                return true;
        }

        if( this->m_ubound > this->cmatches( param ) )
        {
            ++this->cmatches( param );
            if( this->m_psub->recursive_match_all_helper( param, icur, CStringsT() ) )
                return true;
            --this->cmatches( param );
        }

        return false;
    }

    class end_quantifier : public indestructable_sub_expr<IterT, end_quantifier>
    {
        min_group_quantifier<IterT> const *const m_pquant;

        end_quantifier & operator=( end_quantifier const & );

        void _push_frame( match_param<IterT> & param ) const
        {
            backref_tag<IterT> & br = param.m_prgbackrefs[ m_pquant->group_number() ];
            param.m_pstack->push( br.reserved4 );
            br.reserved4 = br.reserved5;
            br.reserved5 = param.m_icur;
        }

        void _pop_frame( match_param<IterT> & param ) const
        {
            backref_tag<IterT> & br = param.m_prgbackrefs[ m_pquant->group_number() ];
            br.reserved5 = br.reserved4;
            param.m_pstack->pop( br.reserved4 );
        }

        template< typename CStringsT >
        bool _recursive_match_all( match_param<IterT> & param, IterT icur REGEX_VC6(COMMA CStringsT) ) const
        {
            typedef typename backref_tag<IterT>::smart_iter_type smart_iter_type;

            smart_iter_type old_highwater1 = m_pquant->highwater1( param );

            if( icur == old_highwater1 )
                return m_pquant->recursive_match_next_( param, icur, CStringsT() );

            m_pquant->highwater1( param ) = m_pquant->highwater2( param );
            m_pquant->highwater2( param ) = icur;

            if( m_pquant->REGEX_NVC6(template) _recurse REGEX_NVC6(<CStringsT>) ( param, icur REGEX_VC6(COMMA CStringsT()) ) )
                return true;

            m_pquant->highwater2( param ) = m_pquant->highwater1( param );
            m_pquant->highwater1( param ) = old_highwater1;

            return false;
        }

        bool _iterative_match_this( match_param<IterT> & param ) const
        {
            backref_tag<IterT> & br = param.m_prgbackrefs[ m_pquant->group_number() ];

            // forcibly break the infinite loop
            if( param.m_icur == br.reserved4 )
            {
                _push_frame( param );
                param.m_pnext = m_pquant->next();
                return true;
            }

            _push_frame( param );

            if( m_pquant->m_lbound <= br.reserved2 )
            {
                br.reserved3 = false;
                param.m_pnext = m_pquant->next();
                return true;
            }

            ++br.reserved2;
            param.m_pnext = m_pquant->m_psub;

            return true;
        }

        bool _iterative_rematch_this( match_param<IterT> & param ) const
        {
            typedef typename backref_tag<IterT>::smart_iter_type smart_iter_type;

            backref_tag<IterT> & br = param.m_prgbackrefs[ m_pquant->group_number() ];

            // infinite loop forcibly broken
            if( param.m_icur == param.m_pstack->REGEX_NVC6(template) top REGEX_NVC6(<smart_iter_type>) ( REGEX_VC6(type2type<smart_iter_type>()) ) )
            {
                _pop_frame( param );
                return false;
            }

            if( br.reserved3 )
            {
                --br.reserved2;

                _pop_frame( param );
                return false;
            }

            br.reserved3 = true;

            if( m_pquant->m_ubound > br.reserved2 )
            {
                ++br.reserved2;
                param.m_pnext = m_pquant->m_psub;
                return true;
            }

            _pop_frame( param );
            return false;
        }
    public:
        end_quantifier( min_group_quantifier<IterT> const * pquant = 0 )
            : m_pquant( pquant )
        {
        }

        virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
        {
            return _recursive_match_all REGEX_NVC6(<false_t>) ( param, icur REGEX_VC6(COMMA false_t()) );
        }
        virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const
        {
            return _recursive_match_all REGEX_NVC6(<true_t>) ( param, icur REGEX_VC6(COMMA true_t()) );
        }
        virtual bool iterative_match_this_( match_param<IterT> & param ) const
        {
            return _iterative_match_this( param );
        }
        virtual bool iterative_match_this_c( match_param<IterT> & param ) const
        {
            return _iterative_match_this( param );
        }
        virtual bool iterative_rematch_this_( match_param<IterT> & param ) const
        {
            return _iterative_rematch_this( param );
        }
        virtual bool iterative_rematch_this_c( match_param<IterT> & param ) const
        {
            return _iterative_rematch_this( param );
        }
        virtual width_type width_this( width_param<IterT> & )
        {
            return zero_width;
        }
    } m_end_quant;

    friend class end_quantifier;
};

inline void fixup_backref( size_t & cbackref, std::list<size_t> const & invisible_groups )
{
    std::list<size_t>::const_iterator iter = invisible_groups.begin();
    for( ; invisible_groups.end() != iter && cbackref >= *iter; ++iter )
    {
        ++cbackref;
    }
}

template< typename IterT >
class match_backref : public sub_expr<IterT>
{
    bool _iterative_rematch_this( match_param<IterT> & param ) const
    {
        typedef typename std::iterator_traits<IterT>::difference_type diff_type;
        backref_tag<IterT> const & br = param.m_prgbackrefs[ m_nbackref ];
        diff_type dist = std::distance( br.first, br.second );
        std::advance( param.m_icur, -dist );
        return false;
    }
public:
    match_backref( size_t nbackref )
        : m_nbackref( nbackref )
    {
    }

    // Return the width specifications of the group to which this backref refers
    virtual width_type width_this( width_param<IterT> & param )
    {
        // fix up the backref to take into account the number of invisible groups
        fixup_backref( m_nbackref, param.m_invisible_groups );

        if( m_nbackref >= param.m_rggroups.size() )
            throw bad_regexpr( "reference to nonexistent group" );

        // If the entry in the backref vector has been nulled out, then we are
        // calculating the width for this group.
        if( 0 == param.m_rggroups[ m_nbackref ] )
            return worst_width; // can't tell how wide this group will be.  :-(

        return param.m_rggroups[ m_nbackref ]->width_this( param );
    }
    virtual bool iterative_rematch_this_( match_param<IterT> & param ) const
    {
        return _iterative_rematch_this( param );
    }
    virtual bool iterative_rematch_this_c( match_param<IterT> & param ) const
    {
        return _iterative_rematch_this( param );
    }
protected:
    size_t m_nbackref;
};

template< typename CmpT, typename IterT >
class match_backref_t : public match_backref<IterT>
{
public:
    match_backref_t( size_t nbackref )
        : match_backref<IterT>( nbackref )
    {
    }
    virtual sub_expr<IterT> * quantify( size_t lbound, size_t ubound, bool greedy, regex_arena & arena )
    {
        if( greedy )
            return new( arena ) max_atom_quantifier<IterT, match_backref_t<CmpT, IterT> >( this, lbound, ubound );
        else
            return new( arena ) min_atom_quantifier<IterT, match_backref_t<CmpT, IterT> >( this, lbound, ubound );
    }
    virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
    {
        return ( match_backref_t::recursive_match_this_( param, icur ) && this->recursive_match_next_( param, icur, false_t() ) );
    }
    virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const
    {
        return ( match_backref_t::recursive_match_this_c( param, icur ) && this->recursive_match_next_( param, icur, true_t() ) );
    }
    virtual bool recursive_match_this_( match_param<IterT> & param, IterT & icur ) const
    {
        return _do_match_this REGEX_NVC6(<false_t>) ( param, icur REGEX_VC6(COMMA false_t()) );
    }
    virtual bool recursive_match_this_c( match_param<IterT> & param, IterT & icur ) const
    {
        return _do_match_this REGEX_NVC6(<true_t>) ( param, icur REGEX_VC6(COMMA true_t()) );
    }
    virtual bool iterative_match_this_( match_param<IterT> & param ) const
    {
        param.m_pnext = this->next();
        return _do_match_this REGEX_NVC6(<false_t>) ( param, param.m_icur REGEX_VC6(COMMA false_t()) );
    }
    virtual bool iterative_match_this_c( match_param<IterT> & param ) const
    {
        param.m_pnext = this->next();
        return _do_match_this REGEX_NVC6(<true_t>) ( param, param.m_icur REGEX_VC6(COMMA true_t()) );
    }
protected:
    template< typename CStringsT >
    bool _do_match_this( match_param<IterT> & param, IterT & icur REGEX_VC6(COMMA CStringsT) ) const
    {
        // Pattern compilation should have failed if the following is false:
        REGEX_ASSERT( this->m_nbackref < param.m_cbackrefs );

        // Don't match a backref that hasn't match anything
        if( ! param.m_prgbackrefs[ this->m_nbackref ].matched )
            return false;

        IterT ithis       = param.m_prgbackrefs[ this->m_nbackref ].first;
        IterT const iend  = param.m_prgbackrefs[ this->m_nbackref ].second;
        IterT icur_tmp    = icur;

        for( ; iend != ithis; ++icur_tmp, ++ithis )
        {
            if( eos_t<CStringsT>::eval( param, icur_tmp ) || CmpT::eval( *icur_tmp, *ithis ) )
                return false;
        }
        icur = icur_tmp;
        return true;
    }
};

template< typename IterT >
inline match_backref<IterT> * create_backref(
    size_t cbackref,
    REGEX_FLAGS flags, regex_arena & arena )
{
    typedef typename std::iterator_traits<IterT>::value_type char_type;

    switch( NOCASE & flags )
    {
    case 0:
        return new( arena ) match_backref_t<ch_neq_t<char_type>, IterT>( cbackref );
    case NOCASE:
        return new( arena ) match_backref_t<ch_neq_nocase_t<char_type>, IterT>( cbackref );
    default:
        REGEX_ASSERT(false);
        return 0;
    }
}

template< typename IterT >
class match_recurse : public sub_expr<IterT>
{
    match_recurse & operator=( match_recurse const & );

    void _push_frame( match_param<IterT> & param ) const
    {
        typedef typename match_param<IterT>::backref_type backref_type;
        unsafe_stack * pstack = param.m_pstack;
        backref_type * ibegin = param.m_prgbackrefs;
        backref_type * iend   = ibegin + param.m_cbackrefs;
        for( ; iend != ibegin; ++ibegin )
        {
            pstack->push( ibegin->reserved1 );
        }
    }

    void _pop_frame( match_param<IterT> & param ) const
    {
        typedef typename match_param<IterT>::backref_type backref_type;
        unsafe_stack * pstack = param.m_pstack;
        backref_type * ibegin = param.m_prgbackrefs;
        backref_type * iend   = ibegin + param.m_cbackrefs;
        while( iend != ibegin )
        {
            --iend;
            pstack->pop( iend->reserved1 );
        }
    }

    template< typename CStringsT >
    bool _recursive_match_all( match_param<IterT> & param, IterT icur REGEX_VC6(COMMA CStringsT) ) const
    {
        // Prevent infinite recursion. If icur == param.m_prgbackrefs[ 0 ].reserved1,
        // then the pattern has eaten 0 chars to date, and we would recurse forever.
        if( icur == param.m_prgbackrefs[ 0 ].reserved1 )
            return this->recursive_match_next_( param, icur, CStringsT() );

        // copy the backref vector onto the stack
        IterT * prgci = static_cast<IterT*>( alloca( param.m_cbackrefs * sizeof( IterT ) ) );
        save_backrefs<IterT>( param.m_prgbackrefs, param.m_prgbackrefs + param.m_cbackrefs, prgci );

        // Recurse.
        if( param.m_pfirst->recursive_match_all_helper( param, icur, CStringsT() ) )
        {
            // Restore the backref vector
            restore_backrefs<IterT>( param.m_prgbackrefs, param.m_prgbackrefs + param.m_cbackrefs, prgci );
            std::for_each( prgci, prgci + param.m_cbackrefs, deleter() );

            // Recursive match succeeded. Try to match the rest of the pattern
            // using the end of the recursive match as the start of the next
            return this->recursive_match_next_( param, param.m_prgbackrefs[ 0 ].second, CStringsT() );
        }

        // Recursion failed
        std::for_each( prgci, prgci + param.m_cbackrefs, deleter() );
        return false;
    }
    template< typename CStringsT >
    bool _iterative_match_this( match_param<IterT> & param REGEX_VC6(COMMA CStringsT) ) const
    {
        param.m_pstack->push( param.m_icur );

        // Prevent infine recursion
        if( param.m_icur == param.m_prgbackrefs[ 0 ].reserved1 )
        {
            param.m_pnext = this->next();
            return true;
        }

        _push_frame( param );

        if( _do_match_iterative( param.m_pfirst, param, param.m_icur, CStringsT() ) )
        {
            _pop_frame( param );
            param.m_pnext = this->next();
            return true;
        }

        _pop_frame( param );
        param.m_pstack->pop( param.m_icur );
        return false;
    }
    bool _iterative_rematch_this( match_param<IterT> & param ) const
    {
        param.m_pstack->pop( param.m_icur );
        return false;
    }
public:
    match_recurse()
    {
    }

    virtual sub_expr<IterT> * quantify( size_t, size_t, bool, regex_arena & )
    {
        throw bad_regexpr( "recursion sub-expression cannot be quantified" );
    }

    virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
    {
        return _recursive_match_all REGEX_NVC6(<false_t>) ( param, icur REGEX_VC6(COMMA false_t()) );
    }
    virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const
    {
        return _recursive_match_all REGEX_NVC6(<true_t>) ( param, icur REGEX_VC6(COMMA true_t()) );
    }
    virtual bool iterative_match_this_( match_param<IterT> & param ) const
    {
        return _iterative_match_this REGEX_NVC6(<false_t>) ( param REGEX_VC6(COMMA false_t()) );
    }
    virtual bool iterative_match_this_c( match_param<IterT> & param ) const
    {
        return _iterative_match_this REGEX_NVC6(<true_t>) ( param REGEX_VC6(COMMA true_t()) );
    }
    virtual bool iterative_rematch_this_( match_param<IterT> & param ) const
    {
        return _iterative_rematch_this( param );
    }
    virtual bool iterative_rematch_this_c( match_param<IterT> & param ) const
    {
        return _iterative_rematch_this( param );
    }
    virtual width_type width_this( width_param<IterT> & )
    {
        return worst_width;
    }
};

template< typename IterT >
inline match_recurse<IterT> * create_recurse( regex_arena & arena )
{
    return new( arena ) match_recurse<IterT>();
}

template< typename IterT >
struct backref_condition
{
    size_t m_cbackref;

    backref_condition( size_t cbackref )
        : m_cbackref( cbackref )
    {
    }

    template< typename CStringsT >
    bool recursive_match_this_helper( match_param<IterT> & param, IterT, CStringsT ) const
    {
        return m_cbackref < param.m_cbackrefs && param.m_prgbackrefs[ m_cbackref ].matched;
    }
    template< typename CStringsT >
    bool iterative_match_this_helper( match_param<IterT> & param, CStringsT ) const
    {
        return m_cbackref < param.m_cbackrefs && param.m_prgbackrefs[ m_cbackref ].matched;
    }
    template< typename CStringsT >
    bool iterative_rematch_this_helper( match_param<IterT> &, CStringsT ) const
    {
        return false;
    }
    void width_this( width_param<IterT> & param )
    {
        // fix up the backref to take into account the number of invisible groups
        fixup_backref( m_cbackref, param.m_invisible_groups );
    }
};

template< typename IterT >
struct assertion_condition
{
    std::auto_ptr<match_group_base<IterT> > m_passert;

    assertion_condition( match_group_base<IterT> * passert , regex_arena & arena )
        : m_passert( passert )
    {
        *passert->pnext() = new( arena ) end_of_pattern<IterT>;
    }

    bool recursive_match_this_helper( match_param<IterT> & param, IterT icur, false_t ) const
    {
        return m_passert->recursive_match_all_( param, icur );
    }
    bool recursive_match_this_helper( match_param<IterT> & param, IterT icur, true_t ) const
    {
        return m_passert->recursive_match_all_c( param, icur );
    }
    bool iterative_match_this_helper( match_param<IterT> & param, false_t ) const
    {
        return m_passert->iterative_match_this_( param );
    }
    bool iterative_match_this_helper( match_param<IterT> & param, true_t ) const
    {
        return m_passert->iterative_match_this_c( param );
    }
    bool iterative_rematch_this_helper( match_param<IterT> & param, false_t ) const
    {
        return m_passert->iterative_rematch_this_( param );
    }
    bool iterative_rematch_this_helper( match_param<IterT> & param, true_t ) const
    {
        return m_passert->iterative_rematch_this_c( param );
    }
    void width_this( width_param<IterT> & param )
    {
        ( void ) m_passert->width_this( param );
    }
};

template< typename IterT, typename CondT >
class match_conditional : public match_group<IterT>
{
protected:
    typedef typename match_group<IterT>::alt_list_type alt_list_type;

private:
    match_conditional & operator=( match_conditional const & );

    template< typename CStringsT >
    bool _recursive_match_all( match_param<IterT> & param, IterT icur REGEX_VC6(COMMA CStringsT) ) const
    {
        typedef typename alt_list_type::const_iterator iter_type;
        iter_type ialt = this->m_rgalternates.begin();

        if( m_condition.recursive_match_this_helper( param, icur, CStringsT() ) || this->m_rgalternates.end() != ++ialt )
        {
            return (*ialt)->recursive_match_all_helper( param, icur, CStringsT() );
        }
        return this->recursive_match_next_( param, icur, CStringsT() );
    }
    template< typename CStringsT >
    bool _iterative_match_this( match_param<IterT> & param REGEX_VC6(COMMA CStringsT) ) const
    {
        typedef typename alt_list_type::const_iterator iter_type;
        iter_type ialt = this->m_rgalternates.begin();

        if( m_condition.iterative_match_this_helper( param, CStringsT() ) )
        {
            param.m_pstack->push( true );
            param.m_pnext = *ialt;
            return true;
        }
        param.m_pstack->push( false );
        param.m_pnext = ( this->m_rgalternates.end() != ++ialt ) ? *ialt : this->next();
        return true;
    }
    template< typename CStringsT >
    bool _iterative_rematch_this( match_param<IterT> & param REGEX_VC6(COMMA CStringsT) ) const
    {
        bool condition;
        param.m_pstack->pop( condition );
        if( condition )
            m_condition.iterative_rematch_this_helper( param, CStringsT() );
        return false;
    }
public:
    typedef CondT condition_type;

    match_conditional( size_t cgroup, condition_type condition, regex_arena & arena )
        : match_group<IterT>( cgroup, arena )
        , m_condition( condition )
    {
    }

    virtual bool recursive_match_all_( match_param<IterT> & param, IterT icur ) const
    {
        return _recursive_match_all REGEX_NVC6(<false_t>) ( param, icur REGEX_VC6(COMMA false_t()) );
    }
    virtual bool recursive_match_all_c( match_param<IterT> & param, IterT icur ) const
    {
        return _recursive_match_all REGEX_NVC6(<true_t>) ( param, icur REGEX_VC6(COMMA true_t()) );
    }
    virtual bool iterative_match_this_( match_param<IterT> & param ) const
    {
        return _iterative_match_this REGEX_NVC6(<false_t>) ( param REGEX_VC6(COMMA false_t()) );
    }
    virtual bool iterative_match_this_c( match_param<IterT> & param ) const
    {
        return _iterative_match_this REGEX_NVC6(<true_t>) ( param REGEX_VC6(COMMA true_t()) );
    }
    virtual bool iterative_rematch_this_( match_param<IterT> & param ) const
    {
        return _iterative_rematch_this REGEX_NVC6(<false_t>) ( param REGEX_VC6(COMMA false_t()) );
    }
    virtual bool iterative_rematch_this_c( match_param<IterT> & param ) const
    {
        return _iterative_rematch_this REGEX_NVC6(<true_t>) ( param REGEX_VC6(COMMA true_t()) );
    }
    virtual width_type width_this( width_param<IterT> & param )
    {
        typedef typename alt_list_type::const_iterator iter_type;
        iter_type ialt = this->m_rgalternates.begin();

        width_type width = ( *ialt )->get_width( param );

        if( this->m_rgalternates.end() != ++ialt )
        {
            width_type temp_width = ( *ialt )->get_width( param );
            width.m_min = regex_min( width.m_min, temp_width.m_min );
            width.m_max = regex_max( width.m_max, temp_width.m_max );
        }
        else
        {
            width.m_min = 0;
        }

        // Have the condition calculate its width, too. This is important
        // if the condition is a lookbehind assertion.
        m_condition.width_this( param );

        return this->m_nwidth = width;
    }

protected:
    condition_type m_condition;
};

template< typename IterT >
inline match_conditional<IterT, backref_condition<IterT> > * create_backref_conditional(
    size_t cgroup,
    size_t cbackref,
    regex_arena & arena )
{
    backref_condition<IterT> cond( cbackref );
    return new( arena ) match_conditional<IterT, backref_condition<IterT> >(
        cgroup, cond, arena );
}

template< typename IterT >
inline match_conditional<IterT, assertion_condition<IterT> > * create_assertion_conditional(
    size_t cgroup,
    match_group_base<IterT> * passert,
    regex_arena & arena )
{
    assertion_condition<IterT> cond( passert, arena );
    return new( arena ) match_conditional<IterT, assertion_condition<IterT> >(
        cgroup, cond, arena );
}

//
// From basic_rpattern_base_impl
//

template< typename IterT >
REGEXPR_H_INLINE bool basic_rpattern_base_impl<IterT>::_ok_to_recurse() const //throw()
{
    switch( m_mode )
    {
    case MODE_FAST:
        return true;
    case MODE_SAFE:
        return false;
    case MODE_MIXED:
        return m_fok_to_recurse;
    default:
        return false;
    }
}

template< typename IterT >
REGEXPR_H_INLINE void basic_rpattern_base_impl<IterT>::swap( basic_rpattern_base_impl<IterT> & that ) // throw()
{
    using std::swap;
    swap( m_fuses_backrefs, that.m_fuses_backrefs );
    swap( m_floop, that.m_floop );
    swap( m_fok_to_recurse, that.m_fok_to_recurse );
    swap( m_cgroups, that.m_cgroups );
    swap( m_cgroups_visible, that.m_cgroups_visible );
    swap( m_flags, that.m_flags );
    swap( m_mode, that.m_mode );
    swap( m_nwidth, that.m_nwidth );
    swap( m_pfirst, that.m_pfirst );
    swap( m_search, that.m_search );

    swap_auto_ptr( m_pat, that.m_pat );
    swap_auto_ptr( m_subst, that.m_subst );

    m_subst_list.swap( that.m_subst_list );
    m_invisible_groups.swap( that.m_invisible_groups );
    m_arena.swap( that.m_arena );
}

// A helper class for automatically deallocating the arena when
// parsing the pattern results in an exception
class arena_guard
{
    arena_guard( arena_guard const & );
    arena_guard & operator=( arena_guard const & );
    regex_arena * m_parena;
public:
    explicit arena_guard( regex_arena & arena )
        : m_parena( &arena )
    {
    }
    ~arena_guard()
    {
        if( m_parena )
            m_parena->clear();
    }
    void dismiss()
    {
        m_parena = 0;
    }
};

template< typename CatT > 
struct is_random_access_helper
{
    enum { value = false };
};
template<> 
struct is_random_access_helper<std::random_access_iterator_tag>
{
    enum { value = true };
};
template< typename IterT > 
struct is_random_access
{
    typedef typename std::iterator_traits<IterT>::iterator_category cat_type;
    enum { value = is_random_access_helper<cat_type>::value };
};

} // namespace detail

//
// Implementation of basic_rpattern_base:
//

template< typename IterT, typename SyntaxT >
REGEXPR_H_INLINE void basic_rpattern_base<IterT, SyntaxT>::init( string_type const & pat, REGEX_FLAGS flags, REGEX_MODE mode )
{
    basic_rpattern_base<IterT, SyntaxT> temp( pat, flags, mode );
    swap( temp );
}

template< typename IterT, typename SyntaxT >
REGEXPR_H_INLINE void basic_rpattern_base<IterT, SyntaxT>::init( string_type const & pat, string_type const & subst, REGEX_FLAGS flags, REGEX_MODE mode )
{
    basic_rpattern_base<IterT, SyntaxT> temp( pat, subst, flags, mode );
    swap( temp );
}

template< typename IterT, typename SyntaxT >
REGEXPR_H_INLINE void basic_rpattern_base<IterT, SyntaxT>::_common_init( REGEX_FLAGS flags )
{
    this->m_cgroups = 0;
    std::vector<detail::match_group_base<IterT>*> rggroups;
    typename string_type::iterator ipat = this->m_pat->begin();
    syntax_type sy( flags );
    detail::match_group_base<IterT> * pgroup;

    // Set up a sentry that will free the arena memory
    // automatically on parse failure.
    {
        detail::arena_guard guard( this->m_arena );

        // This will throw on failure
        pgroup = _find_next_group( ipat, 0, sy, rggroups );

        // terminate the pattern with the end_of_pattern marker
        *pgroup->pnext() = new( this->m_arena ) detail::end_of_pattern<IterT>;

        // The parse was successful. Dismiss the parse sentry
        guard.dismiss();
    }

    REGEX_ASSERT( 0 == m_pfirst );
    m_pfirst = pgroup;

    // Calculate the width of the pattern and all groups
    this->m_nwidth = pgroup->group_width( rggroups, m_invisible_groups );

    //
    // determine if we can get away with only calling m_pfirst->recursive_match_all only once
    //

    this->m_floop = true;

    // Optimization: if first character of pattern string is '^'
    // and we are not doing a multiline match, then we only
    // need to try recursive_match_all once
    typename string_type::iterator icur = this->m_pat->begin();
    if( MULTILINE != ( MULTILINE & this->m_flags ) &&
        1 == pgroup->calternates() &&
        this->m_pat->end() != icur &&
        BEGIN_LINE == sy.reg_token( icur, this->m_pat->end() ) )
    {
        this->m_flags = ( REGEX_FLAGS ) ( m_flags & ~RIGHTMOST );
        this->m_floop = false;
    }

    // Optimization: if first 2 characters of pattern string are ".*" or ".+",
    // then we only need to try recursive_match_all once
    icur = this->m_pat->begin();
    if( RIGHTMOST != ( RIGHTMOST & this->m_flags ) &&
        SINGLELINE == ( SINGLELINE & this->m_flags ) &&
        1 == pgroup->calternates() &&
        this->m_pat->end() != icur &&
        MATCH_ANY == sy.reg_token( icur, this->m_pat->end() ) &&
        this->m_pat->end() != icur )
    {
        switch( sy.quant_token( icur, this->m_pat->end() ) )
        {
        case ONE_OR_MORE:
        case ZERO_OR_MORE:
        case ONE_OR_MORE_MIN:
        case ZERO_OR_MORE_MIN:
            this->m_floop = false;
            break;
        default:
            break;
        }
    }
}

template< typename IterT, typename SyntaxT >
REGEXPR_H_INLINE void basic_rpattern_base<IterT, SyntaxT>::set_substitution( string_type const & subst )
{
    using std::swap;
    std::auto_ptr<string_type> temp_subst( new string_type( subst ) );
    detail::subst_list_type temp_subst_list;
    bool uses_backrefs = false;

    _normalize_string( *temp_subst );
    basic_rpattern_base<IterT, SyntaxT>::_parse_subst( *temp_subst, uses_backrefs, temp_subst_list );

    detail::swap_auto_ptr( temp_subst, this->m_subst );
    swap( uses_backrefs, this->m_fuses_backrefs );
    temp_subst_list.swap( this->m_subst_list );
}

template< typename IterT, typename SyntaxT >
inline detail::match_group_base<IterT> * basic_rpattern_base<IterT, SyntaxT>::_find_next_group(
    typename string_type::iterator & ipat,
    detail::match_group_base<IterT> * pgroup_enclosing, syntax_type & sy,
    std::vector<detail::match_group_base<IterT>*> & rggroups )
{
    std::auto_ptr<detail::match_group_base<IterT> > pgroup;
    typename string_type::iterator itemp = ipat;
    REGEX_FLAGS old_flags = sy.get_flags();
    TOKEN tok = NO_TOKEN;
    size_t extent_start = this->m_cgroups;
    bool fconditional = false;

    // Look for group extensions.
    if( this->m_pat->end() != ipat && NO_TOKEN != ( tok = sy.ext_token( ipat, this->m_pat->end() ) ) )
    {
        if( this->m_pat->begin() == itemp || this->m_pat->end() == ipat )
            throw bad_regexpr( "ill-formed regular expression" );

        // Is this a recursion element?
        if( EXT_RECURSE == tok )
        {
            pgroup_enclosing->add_item( detail::create_recurse<IterT>( this->m_arena ) );

            // This pattern could recurse deeply. Note that fact here so that
            // we can opt to use a stack-conservative algorithm at match time.
            this->m_fok_to_recurse = false;
        }

        // Don't process empty groups like (?:) or (?i) or (?R)
        if( END_GROUP != sy.reg_token( itemp = ipat, this->m_pat->end() ) )
        {
            switch( tok )
            {
            case EXT_NOBACKREF:
                // note that this group is not visible, so we can fix
                // up offsets into the backref vector later
                m_invisible_groups.push_back( this->m_cgroups );
                detail::reset_auto_ptr( pgroup, new( this->m_arena ) detail::match_group<IterT>( _get_next_group_nbr(), this->m_arena ) );
                break;

            case EXT_INDEPENDENT:
                m_invisible_groups.push_back( this->m_cgroups );
                detail::reset_auto_ptr( pgroup, new( this->m_arena ) detail::independent_group<IterT>( _get_next_group_nbr(), this->m_arena ) );
                break;

            case EXT_POS_LOOKAHEAD:
                detail::reset_auto_ptr( pgroup, new( this->m_arena ) detail::lookahead_assertion<IterT>( true, this->m_arena ) );
                break;

            case EXT_NEG_LOOKAHEAD:
                detail::reset_auto_ptr( pgroup, new( this->m_arena ) detail::lookahead_assertion<IterT>( false, this->m_arena ) );
                break;

            case EXT_POS_LOOKBEHIND:
                detail::reset_auto_ptr( pgroup, new( this->m_arena ) detail::lookbehind_assertion<IterT>( true, this->m_arena ) );
                break;

            case EXT_NEG_LOOKBEHIND:
                detail::reset_auto_ptr( pgroup, new( this->m_arena ) detail::lookbehind_assertion<IterT>( false, this->m_arena ) );
                break;

            case EXT_CONDITION:
                fconditional = true;
                m_invisible_groups.push_back( this->m_cgroups );

                if( size_t cbackref = detail::parse_int( ipat, this->m_pat->end() ) &&
                    END_GROUP == sy.reg_token( ipat, this->m_pat->end() ) )
                {
                    detail::reset_auto_ptr(
                        pgroup, detail::create_backref_conditional<IterT>(
                            _get_next_group_nbr(), cbackref, this->m_arena ) );
                }
                else
                {
                    switch( sy.ext_token( itemp = ipat, this->m_pat->end() ) )
                    {
                    case EXT_POS_LOOKAHEAD:
                    case EXT_NEG_LOOKAHEAD:
                    case EXT_POS_LOOKBEHIND:
                    case EXT_NEG_LOOKBEHIND:
                        {
                            std::auto_ptr<detail::match_group_base<IterT> > pgroup_tmp(
                                _find_next_group( ipat, 0, sy, rggroups ) );
                            detail::reset_auto_ptr(
                                pgroup, detail::create_assertion_conditional<IterT>(
                                    _get_next_group_nbr(), pgroup_tmp.get(), this->m_arena ) );
                            pgroup_tmp.release();
                        }
                        break;
                    default:
                        throw bad_regexpr( "bad extension sequence" );
                    }
                }
                break;

            case EXT_COMMENT:
                while( END_GROUP != ( tok = sy.reg_token( ipat, this->m_pat->end() ) ) )
                {
                    if( NO_TOKEN == tok && this->m_pat->end() != ipat )
                        ++ipat;
                    if( this->m_pat->end() == ipat )
                        throw bad_regexpr( "Expecting end of comment" );
                }
                break;

            default:
                throw bad_regexpr( "bad extension sequence" );
            }
        }
        else
        {
            // Skip over the END_GROUP token
            ipat = itemp;
        }
    }
    else
    {
        detail::reset_auto_ptr( pgroup, new( this->m_arena ) detail::match_group<IterT>( _get_next_group_nbr(), this->m_arena ) );
        ++this->m_cgroups_visible;
    }

    if( 0 != pgroup.get() )
    {
        detail::must_have<char_type> must;

        pgroup->open_group();
        while( _find_next( ipat, pgroup.get(), sy, rggroups ) ) {}
        must = pgroup->close_group( this->m_arena );

        // if this is a conditional group, then there must be at
        // most 2 alternates.
        if( fconditional && 2 < pgroup->calternates() )
            throw bad_regexpr( "Too many alternates in conditional subexpression" );

        // if this is the top-level group and it returned a "must have"
        // string, then use that to initialize a boyer-moore search structure
        if( detail::is_random_access<IterT>::value && must.m_has && 0 == pgroup->group_number() )
        {
            typedef typename string_type::const_iterator iter_type;
            m_search = new( this->m_arena ) detail::boyer_moore<iter_type>
                ( must.m_begin, must.m_end, must.m_lower );
        }

        // Add this group to the rggroups array
        if( size_t( -1 ) != pgroup->group_number() )
        {
            if( pgroup->group_number() >= rggroups.size() )
                rggroups.resize( pgroup->group_number() + 1, 0 );
            rggroups[ pgroup->group_number() ] = pgroup.get();
        }

        // tell this group how many groups are contained within it
        pgroup->set_extent( detail::extent_type( extent_start, this->m_cgroups - extent_start ) );

        // If this is not a pattern modifier, restore the
        // flags to their previous settings.  This causes
        // pattern modifiers to have the scope of their
        // enclosing group.
        sy.set_flags( old_flags );
    }

    return pgroup.release();
}

namespace detail
{

// If we reached the end of the string before finding the end of the
// character set, then this is an ill-formed regex
template< typename IterT >
inline void check_iter( IterT icur, IterT iend )
{
    if( iend == icur )
        throw bad_regexpr( "expecting end of character set" );
}

template< typename IBeginT, typename IEndT >
inline typename std::iterator_traits<IEndT>::value_type get_escaped_char( IBeginT & icur, IEndT iend, bool normalize )
{
    typedef typename std::iterator_traits<IEndT>::value_type char_type;
    char_type ch = 0, i;
    check_iter<IEndT>( icur, iend );

    switch( *icur )
    {
    // octal escape sequence
    case REGEX_CHAR(char_type,'0'): case REGEX_CHAR(char_type,'1'): case REGEX_CHAR(char_type,'2'): case REGEX_CHAR(char_type,'3'):
    case REGEX_CHAR(char_type,'4'): case REGEX_CHAR(char_type,'5'): case REGEX_CHAR(char_type,'6'): case REGEX_CHAR(char_type,'7'):
        ch = char_type( *icur++ - REGEX_CHAR(char_type,'0') );
        for( i=0; i<2 && REGEX_CHAR(char_type,'0') <= *icur && REGEX_CHAR(char_type,'7') >= *icur; check_iter<IEndT>( ++icur, iend ) )
            ch = char_type( ch * 8 + ( *icur - REGEX_CHAR(char_type,'0') ) );
        break;
    // bell character
    case REGEX_CHAR(char_type,'a'):
        if( ! normalize )
            goto default_;
        ch = REGEX_CHAR(char_type,'\a');
        ++icur;
        break;
    // control character
    case REGEX_CHAR(char_type,'c'):
        check_iter<IEndT>( ++icur, iend );
        ch = *icur++;
        if( REGEX_CHAR(char_type,'a') <= ch && REGEX_CHAR(char_type,'z') >= ch )
            ch = detail::regex_toupper( ch );
        ch ^= 0x40;
        break;
    // escape character
    case REGEX_CHAR(char_type,'e'):
        ch = 27;
        ++icur;
        break;
    // formfeed character
    case REGEX_CHAR(char_type,'f'):
        if( ! normalize )
            goto default_;
        ch = REGEX_CHAR(char_type,'\f');
        ++icur;
        break;
    // newline
    case REGEX_CHAR(char_type,'n'):
        if( ! normalize )
            goto default_;
        ch = REGEX_CHAR(char_type,'\n');
        ++icur;
        break;
    // return
    case REGEX_CHAR(char_type,'r'):
        if( ! normalize )
            goto default_;
        ch = REGEX_CHAR(char_type,'\r');
        ++icur;
        break;
    // horizontal tab
    case REGEX_CHAR(char_type,'t'):
        if( ! normalize )
            goto default_;
        ch = REGEX_CHAR(char_type,'\t');
        ++icur;
        break;
    // vertical tab
    case REGEX_CHAR(char_type,'v'):
        if( ! normalize )
            goto default_;
        ch = REGEX_CHAR(char_type,'\v');
        ++icur;
        break;
    // hex escape sequence
    case REGEX_CHAR(char_type,'x'):
        for( ++icur, ch=i=0; i<2 && detail::regex_isxdigit( *icur ); check_iter<IEndT>( ++icur, iend ) )
            ch = char_type( ch * 16 + detail::regex_xdigit2int( *icur ) );
        break;
    // backslash
    case REGEX_CHAR(char_type,'\\'):
        if( ! normalize )
            goto default_;
        ch = REGEX_CHAR(char_type,'\\');
        ++icur;
        break;
    // all other escaped characters represent themselves
    default: default_:
        ch = *icur;
        ++icur;
        break;
    }

    return ch;
}

template< typename CharT, typename CharSetT, typename SyntaxT >
inline void parse_charset(
    std::auto_ptr<CharSetT> & pnew,
    typename std::basic_string<CharT>::iterator & icur,
    typename std::basic_string<CharT>::const_iterator iend,
    SyntaxT & sy )
{
    typedef CharT char_type;
    typedef std::basic_string<CharT> string_type;
    typedef typename string_type::const_iterator iter_type;
    typename string_type::iterator itemp = icur;
    bool const normalize = ( NORMALIZE == ( NORMALIZE & sy.get_flags() ) );

    if( iend != itemp && CHARSET_NEGATE == sy.charset_token( itemp, iend ) )
    {
        pnew->m_fcompliment = true;
        icur = itemp;
    }

    TOKEN tok;
    char_type ch_prev = 0;
    bool fhave_prev = false;
    charset const * pcharset = 0;
    typename string_type::iterator iprev = icur;
    bool const fnocase = ( NOCASE == ( NOCASE & sy.get_flags() ) );

    check_iter<iter_type>( icur, iend );

    // remember the current position and grab the next token
    tok = sy.charset_token( icur, iend );
    do
    {
        check_iter<iter_type>( icur, iend );

        if( CHARSET_RANGE == tok && fhave_prev )
        {
            // remember the current position
            typename string_type::iterator iprev2 = icur;
            fhave_prev = false;

            // ch_prev is lower bound of a range
            switch( sy.charset_token( icur, iend ) )
            {
            case CHARSET_RANGE:
            case CHARSET_NEGATE:
                icur = iprev2; // un-get these tokens and fall through
            case NO_TOKEN:
                pnew->set_bit_range( ch_prev, *icur++, fnocase );
                continue;
            case CHARSET_ESCAPE: // BUGBUG user-defined charset?
                pnew->set_bit_range( ch_prev, get_escaped_char( icur, iend, normalize ), fnocase );
                continue;
            case CHARSET_BACKSPACE:
                pnew->set_bit_range( ch_prev, char_type( 8 ), fnocase ); // backspace
                continue;
            case CHARSET_END: // fall through
            default:          // not a range.
                icur = iprev; // backup to range token
                pnew->set_bit( ch_prev, fnocase );
                pnew->set_bit( *icur++, fnocase );
                continue;
            }
        }

        if( fhave_prev )
            pnew->set_bit( ch_prev, fnocase );
        fhave_prev = false;

        switch( tok )
        {
            // None of the intrinsic charsets are case-sensitive,
            // so no special handling must be done when the NOCASE
            // flag is set.
        case CHARSET_RANGE:
        case CHARSET_NEGATE:
        case CHARSET_END:
            icur = iprev; // un-get these tokens
            ch_prev = *icur++;
            fhave_prev = true;
            continue;
        case CHARSET_BACKSPACE:
            ch_prev = char_type( 8 ); // backspace
            fhave_prev = true;
            continue;
        case ESC_DIGIT:
            *pnew |= intrinsic_charsets<char_type>::get_digit_charset();
            continue;
        case ESC_NOT_DIGIT:
            *pnew |= intrinsic_charsets<char_type>::get_not_digit_charset();
            continue;
        case ESC_SPACE:
            *pnew |= intrinsic_charsets<char_type>::get_space_charset();
            continue;
        case ESC_NOT_SPACE:
            *pnew |= intrinsic_charsets<char_type>::get_not_space_charset();
            continue;
        case ESC_WORD:
            *pnew |= intrinsic_charsets<char_type>::get_word_charset();
            continue;
        case ESC_NOT_WORD:
            *pnew |= intrinsic_charsets<char_type>::get_not_word_charset();
            continue;
        case CHARSET_ALNUM:
            pnew->m_posixcharson |= ( wct_alnum() );
            continue;
        case CHARSET_NOT_ALNUM:
            pnew->m_posixcharsoff.push_front( wct_alnum() );
            continue;
        case CHARSET_ALPHA:
            pnew->m_posixcharson |= ( wct_alpha() );
            continue;
        case CHARSET_NOT_ALPHA:
            pnew->m_posixcharsoff.push_front( wct_alpha() );
            continue;
        case CHARSET_BLANK:
            pnew->m_posixcharson |= ( wct_blank() );
            continue;
        case CHARSET_NOT_BLANK:
            pnew->m_posixcharsoff.push_front( wct_blank() );
            continue;
        case CHARSET_CNTRL:
            pnew->m_posixcharson |= ( wct_cntrl() );
            continue;
        case CHARSET_NOT_CNTRL:
            pnew->m_posixcharsoff.push_front( wct_cntrl() );
            continue;
        case CHARSET_DIGIT:
            pnew->m_posixcharson |= ( wct_digit() );
            continue;
        case CHARSET_NOT_DIGIT:
            pnew->m_posixcharsoff.push_front( wct_digit() );
            continue;
        case CHARSET_GRAPH:
            pnew->m_posixcharson |= ( wct_graph() );
            continue;
        case CHARSET_NOT_GRAPH:
            pnew->m_posixcharsoff.push_front( wct_graph() );
            continue;
        case CHARSET_LOWER:
            if( NOCASE == ( NOCASE & sy.get_flags() ) )
                pnew->m_posixcharson |= ( wct_lower()|wct_upper() );
            else
                pnew->m_posixcharson |= ( wct_lower() );
            continue;
        case CHARSET_NOT_LOWER:
            if( NOCASE == ( NOCASE & sy.get_flags() ) )
                pnew->m_posixcharsoff.push_front( wct_lower()|wct_upper() );
            else
                pnew->m_posixcharsoff.push_front( wct_lower() );
            continue;
        case CHARSET_PRINT:
            pnew->m_posixcharson |= ( wct_print() );
            continue;
        case CHARSET_NOT_PRINT:
            pnew->m_posixcharsoff.push_front( wct_print() );
            continue;
        case CHARSET_PUNCT:
            pnew->m_posixcharson |= ( wct_punct() );
            continue;
        case CHARSET_NOT_PUNCT:
            pnew->m_posixcharsoff.push_front( wct_punct() );
            continue;
        case CHARSET_SPACE:
            pnew->m_posixcharson |= ( wct_space() );
            continue;
        case CHARSET_NOT_SPACE:
            pnew->m_posixcharsoff.push_front( wct_space() );
            continue;
        case CHARSET_UPPER:
            if( NOCASE == ( NOCASE & sy.get_flags() ) )
                pnew->m_posixcharson |= ( wct_upper()|wct_lower() );
            else
                pnew->m_posixcharson |= ( wct_upper() );
            continue;
        case CHARSET_NOT_UPPER:
            if( NOCASE == ( NOCASE & sy.get_flags() ) )
                pnew->m_posixcharsoff.push_front( wct_upper()|wct_lower() );
            else
                pnew->m_posixcharsoff.push_front( wct_upper() );
            continue;
        case CHARSET_XDIGIT:
            pnew->m_posixcharson |= ( wct_xdigit() );
            continue;
        case CHARSET_NOT_XDIGIT:
            pnew->m_posixcharsoff.push_front( wct_xdigit() );
            continue;
        case CHARSET_ESCAPE:
            // Maybe this is a user-defined intrinsic charset
            pcharset = get_altern_charset( *icur, sy );
            if( 0 != pcharset )
            {
                 *pnew |= *pcharset;
                 ++icur;
                 continue;
            }
            else
            {
                ch_prev = get_escaped_char( icur, iend, normalize );
                fhave_prev = true;
            }
            continue;
        default:
            ch_prev = *icur++;
            fhave_prev = true;
            continue;
        }
    }
    while( check_iter<iter_type>( iprev = icur, iend ),
           CHARSET_END != ( tok = sy.charset_token( icur, iend ) ) );

    if( fhave_prev )
        pnew->set_bit( ch_prev, fnocase );

    pnew->optimize( type2type<char_type>() );
}

template< typename CharT, typename SyntaxT >
inline charset const * get_altern_charset( CharT ch, SyntaxT & sy )
{
    typedef std::basic_string<CharT> string_type;
    charset const * pcharset = 0;
    regex::detail::charset_map<CharT> & charset_map = sy.get_charset_map();
    typename regex::detail::charset_map<CharT>::iterator iter = charset_map.find( ch );
    if( charset_map.end() != iter )
    {
        bool const fnocase = ( NOCASE == ( sy.get_flags() & NOCASE ) );
        pcharset = iter->second.m_rgcharsets[ fnocase ];
        if( 0 == pcharset )
        {
            // tmp takes ownership of any ptrs.
            charset_map_node<CharT> tmp = iter->second;
            charset_map.erase( iter ); // prevent possible infinite recursion
            typename string_type::iterator ibegin = tmp.m_str.begin();
            std::auto_ptr<charset> pnew( new charset );
            std::auto_ptr<charset const> pold( tmp.m_rgcharsets[ !fnocase ] );
            parse_charset<CharT, charset>( pnew, ibegin, tmp.m_str.end(), sy );
            tmp.m_rgcharsets[ fnocase ] = pcharset = pnew.get();
            charset_map[ ch ] = tmp; // could throw
            // charset_map has taken ownership of these pointers now.
            pnew.release();
            pold.release();
        }
    }
    return pcharset;
}

} // namespace detail

//
// Read ahead through the pattern and treat sequential atoms
// as a single atom, making sure to handle quantification
// correctly. Warning: dense code ahead.
//
template< typename IterT, typename SyntaxT >
inline void basic_rpattern_base<IterT, SyntaxT>::_find_atom(
    typename string_type::iterator & ipat,
    detail::match_group_base<IterT> * pgroup,
    syntax_type & sy )
{
    typedef typename string_type::iterator iter_type;
    typedef typename std::iterator_traits<iter_type>::difference_type diff_type;
    iter_type itemp = ipat, ibegin;
    diff_type const nstart = std::distance( this->m_pat->begin(), ipat );

    do
    {
        if( itemp != ipat ) // Is there whitespace to skip?
        {
            diff_type dist = std::distance( this->m_pat->begin(), ipat );
            this->m_pat->erase( ipat, itemp ); // erase the whitespace from the patttern
            std::advance( ipat = this->m_pat->begin(), dist );
            if( this->m_pat->end() == ( itemp = ipat ) ) // are we at the end of the pattern?
                break;
        }
        switch( sy.quant_token( itemp, this->m_pat->end() ) )
        {
            // if {, } can't be interpreted as quantifiers, treat them as regular chars
        case BEGIN_RANGE:
            std::advance( ibegin = this->m_pat->begin(), nstart );
            if( ibegin != ipat ) // treat as a quantifier
                goto quantify;
        case NO_TOKEN:
        case END_RANGE:
        case END_RANGE_MIN:
        case RANGE_SEPARATOR:
            break;

        default:
            std::advance( ibegin = this->m_pat->begin(), nstart );
            if( ibegin == ipat ) // must be able to quantify something.
                throw bad_regexpr( "quantifier not expected" );

quantify:   if( ibegin != --ipat )
                pgroup->add_item( detail::create_literal<IterT>( ibegin, ipat, sy.get_flags(), this->m_arena ) );
            std::auto_ptr<detail::sub_expr<IterT> > pnew( detail::create_char<IterT>( *ipat++, sy.get_flags(), this->m_arena ) );
            _quantify( pnew, ipat, false, sy );
            pgroup->add_item( pnew.release() );
            return;
        }
    } while( this->m_pat->end() != ++ipat && ! sy.reg_token( itemp = ipat, this->m_pat->end() ) );

    std::advance( ibegin = this->m_pat->begin(), nstart );
    REGEX_ASSERT( ipat != ibegin );
    pgroup->add_item( detail::create_literal<IterT>( ibegin, ipat, sy.get_flags(), this->m_arena ) );
}

template< typename IterT, typename SyntaxT >
inline bool basic_rpattern_base<IterT, SyntaxT>::_find_next(
    typename string_type::iterator & ipat,
    detail::match_group_base<IterT> * pgroup,
    syntax_type & sy,
    std::vector<detail::match_group_base<IterT>*> & rggroups )
{
    std::auto_ptr<detail::sub_expr<IterT> > pnew;
    std::auto_ptr<detail::custom_charset> pcs;
    typename string_type::iterator ibegin, itemp;
    bool fdone, is_group = false;
    bool const normalize = ( NORMALIZE == ( NORMALIZE & sy.get_flags() ) );

    if( this->m_pat->end() == ipat )
    {
        if( 0 != pgroup->group_number() )
            throw bad_regexpr( "mismatched parenthesis" );
        return false;
    }

    switch( sy.reg_token( ipat, this->m_pat->end() ) )
    {
    case NO_TOKEN: // not a token. Must be an atom
        if( this->m_pat->end() == ipat )
        {
            if( 0 != pgroup->group_number() )
                throw bad_regexpr( "mismatched parenthesis" );
            return false;
        }
        _find_atom( ipat, pgroup, sy );
        return true;

    case END_GROUP:
        if( 0 == pgroup->group_number() )
            throw bad_regexpr( "mismatched parenthesis" );
        return false;

    case ALTERNATION:
        pgroup->end_alternate();
        pgroup->add_alternate();
        return true;

    case BEGIN_GROUP:
        // Find next group. could return NULL if the group is really
        // a pattern modifier, like: ( ?s-i )
        detail::reset_auto_ptr( pnew, _find_next_group( ipat, pgroup, sy, rggroups ) );
        is_group = true;
        break;

    case BEGIN_LINE:
        detail::reset_auto_ptr( pnew, detail::create_bol<IterT>( sy.get_flags(), this->m_arena ) );
        break;

    case END_LINE:
        detail::reset_auto_ptr( pnew, detail::create_eol<IterT>( sy.get_flags(), this->m_arena ) );
        break;

    case BEGIN_CHARSET:
        detail::reset_auto_ptr( pcs, new( this->m_arena ) detail::custom_charset( this->m_arena ) );
        detail::parse_charset<char_type, detail::custom_charset>(
            pcs, ipat, this->m_pat->end(), sy );
        detail::reset_auto_ptr( pnew,
            detail::create_custom_charset<IterT>( pcs.get(), sy.get_flags(), this->m_arena ) );
        pcs.release();
        break;

    case MATCH_ANY:
        detail::reset_auto_ptr( pnew, detail::create_any<IterT>( sy.get_flags(), this->m_arena ) );
        break;

    case ESC_WORD_BOUNDARY:
        detail::reset_auto_ptr( pnew, detail::create_word_boundary<IterT>( true, sy.get_flags(), this->m_arena ) );
        break;

    case ESC_NOT_WORD_BOUNDARY:
        detail::reset_auto_ptr( pnew, detail::create_word_boundary<IterT>( false, sy.get_flags(), this->m_arena ) );
        break;

    case ESC_WORD_START:
        detail::reset_auto_ptr( pnew, detail::create_word_start<IterT>( sy.get_flags(), this->m_arena ) );
        break;

    case ESC_WORD_STOP:
        detail::reset_auto_ptr( pnew, detail::create_word_stop<IterT>( sy.get_flags(), this->m_arena ) );
        break;

    case ESC_DIGIT:
        detail::reset_auto_ptr( pnew, detail::create_charset<IterT>( detail::intrinsic_charsets<char_type>::get_digit_charset(), sy.get_flags(), this->m_arena ) );
        break;

    case ESC_NOT_DIGIT:
        detail::reset_auto_ptr( pnew, detail::create_charset<IterT>( detail::intrinsic_charsets<char_type>::get_not_digit_charset(), sy.get_flags(), this->m_arena ) );
        break;

    case ESC_WORD:
        detail::reset_auto_ptr( pnew, detail::create_charset<IterT>( detail::intrinsic_charsets<char_type>::get_word_charset(), sy.get_flags(), this->m_arena ) );
        break;

    case ESC_NOT_WORD:
        detail::reset_auto_ptr( pnew, detail::create_charset<IterT>( detail::intrinsic_charsets<char_type>::get_not_word_charset(), sy.get_flags(), this->m_arena ) );
        break;

    case ESC_SPACE:
        detail::reset_auto_ptr( pnew, detail::create_charset<IterT>( detail::intrinsic_charsets<char_type>::get_space_charset(), sy.get_flags(), this->m_arena ) );
        break;

    case ESC_NOT_SPACE:
        detail::reset_auto_ptr( pnew, detail::create_charset<IterT>( detail::intrinsic_charsets<char_type>::get_not_space_charset(), sy.get_flags(), this->m_arena ) );
        break;

    case ESC_BEGIN_STRING:
        detail::reset_auto_ptr( pnew, detail::create_bos<IterT>( sy.get_flags(), this->m_arena ) );
        break;

    case ESC_END_STRING:
        detail::reset_auto_ptr( pnew, detail::create_eos<IterT>( sy.get_flags(), this->m_arena ) );
        break;

    case ESC_END_STRING_z:
        detail::reset_auto_ptr( pnew, detail::create_eoz<IterT>( sy.get_flags(), this->m_arena ) );
        break;

    case ESCAPE:
        if( this->m_pat->end() == ipat )
        {
            // BUGBUG what if the escape sequence is more that 1 character?
            detail::reset_auto_ptr( pnew, detail::create_char<IterT>( *--ipat, sy.get_flags(), this->m_arena ) );
            ++ipat;
        }
        else if( REGEX_CHAR(char_type,'0') <= *ipat && REGEX_CHAR(char_type,'9') >= *ipat )
        {
            // Parse at most 3 decimal digits.
            size_t nbackref = detail::parse_int( itemp = ipat, this->m_pat->end(), 999 );
            // If the resulting number could conceivably be a backref, then it is.
            if( REGEX_CHAR(char_type,'0') != *ipat && ( 10 > nbackref || nbackref < _cgroups_total() ) )
            {
                detail::reset_auto_ptr( pnew, detail::create_backref<IterT>( nbackref, sy.get_flags(), this->m_arena ) );
                ipat = itemp;
            }
            else
            {
                // It's an octal character escape sequence. If *ipat is 8 or 9, insert
                // a NULL character, and leave the 8 or 9 as a character literal.
                char_type ch = 0, i = 0;
                for( ; i < 3 && this->m_pat->end() != ipat && REGEX_CHAR(char_type,'0') <= *ipat && REGEX_CHAR(char_type,'7') >= *ipat; ++i, ++ipat )
                    ch = char_type( ch * 8 + ( *ipat - REGEX_CHAR(char_type,'0') ) );
                detail::reset_auto_ptr( pnew, detail::create_char<IterT>( ch, sy.get_flags(), this->m_arena ) );
            }
        }
        else if( REGEX_CHAR(char_type,'e') == *ipat )
        {
            ++ipat;
            detail::reset_auto_ptr( pnew, detail::create_char<IterT>( char_type( 27 ), sy.get_flags(), this->m_arena ) );
        }
        else if( REGEX_CHAR(char_type,'x') == *ipat )
        {
            char_type ch = 0, i = 0;
            for( ++ipat; i < 2 && this->m_pat->end() != ipat && detail::regex_isxdigit( *ipat ); ++i, ++ipat )
                ch = char_type( ch * 16 + detail::regex_xdigit2int( *ipat ) );
            detail::reset_auto_ptr( pnew, detail::create_char<IterT>( ch, sy.get_flags(), this->m_arena ) );
        }
        else if( REGEX_CHAR(char_type,'c') == *ipat )
        {
            if( this->m_pat->end() == ++ipat )
                throw bad_regexpr( "incomplete escape sequence \\c" );
            char_type ch = *ipat++;
            if( REGEX_CHAR(char_type,'a') <= ch && REGEX_CHAR(char_type,'z') >= ch )
                ch = detail::regex_toupper( ch );
            detail::reset_auto_ptr( pnew, detail::create_char<IterT>( char_type( ch ^ 0x40 ), sy.get_flags(), this->m_arena ) );
        }
        else if( REGEX_CHAR(char_type,'a') == *ipat && normalize )
        {
            ++ipat;
            detail::reset_auto_ptr( pnew, detail::create_char<IterT>( REGEX_CHAR(char_type,'\a'), sy.get_flags(), this->m_arena ) );
        }
        else if( REGEX_CHAR(char_type,'f') == *ipat && normalize )
        {
            ++ipat;
            detail::reset_auto_ptr( pnew, detail::create_char<IterT>( REGEX_CHAR(char_type,'\f'), sy.get_flags(), this->m_arena ) );
        }
        else if( REGEX_CHAR(char_type,'n') == *ipat && normalize )
        {
            ++ipat;
            detail::reset_auto_ptr( pnew, detail::create_char<IterT>( REGEX_CHAR(char_type,'\n'), sy.get_flags(), this->m_arena ) );
        }
        else if( REGEX_CHAR(char_type,'r') == *ipat && normalize )
        {
            ++ipat;
            detail::reset_auto_ptr( pnew, detail::create_char<IterT>( REGEX_CHAR(char_type,'\r'), sy.get_flags(), this->m_arena ) );
        }
        else if( REGEX_CHAR(char_type,'t') == *ipat && normalize )
        {
            ++ipat;
            detail::reset_auto_ptr( pnew, detail::create_char<IterT>( REGEX_CHAR(char_type,'\t'), sy.get_flags(), this->m_arena ) );
        }
        else if( REGEX_CHAR(char_type,'\\') == *ipat && normalize )
        {
            ++ipat;
            detail::reset_auto_ptr( pnew, detail::create_char<IterT>( REGEX_CHAR(char_type,'\\'), sy.get_flags(), this->m_arena ) );
        }
        else
        {
            // Is this a user-defined intrinsic character set?
            detail::charset const * pcharset = detail::get_altern_charset( *ipat, sy );
            if( 0 != pcharset )
                detail::reset_auto_ptr( pnew, detail::create_charset<IterT>( *pcharset, sy.get_flags(), this->m_arena ) );
            else
                detail::reset_auto_ptr( pnew, detail::create_char<IterT>( *ipat, sy.get_flags(), this->m_arena ) );
            ++ipat;
        }
        break;

        // If quotemeta, loop until we find quotemeta off or end of string
    case ESC_QUOTE_META_ON:
        for( ibegin = itemp = ipat, fdone = false; !fdone && this->m_pat->end() != ipat; )
        {
            switch( sy.reg_token( ipat, this->m_pat->end() ) )
            {
            case ESC_QUOTE_META_OFF:
                fdone = true;
                break;
            case NO_TOKEN:
                if( this->m_pat->end() != ipat )
                    ++ipat; // fallthrough
            default:
                itemp = ipat;
                break;
            }
        }
        if( itemp != ibegin )
            pgroup->add_item( detail::create_literal<IterT>( ibegin, itemp, sy.get_flags(), this->m_arena ) );

        // skip the quantification code below
        return true;

    // Should never get here for valid patterns
    case ESC_QUOTE_META_OFF:
        throw bad_regexpr( "quotemeta turned off, but was never turned on" );

    default:
        REGEX_ASSERT( ! "Unhandled token type" );
        break;
    }

    // If pnew is null, then the current subexpression is a no-op.
    if( pnew.get() )
    {
        // Look for quantifiers
        _quantify( pnew, ipat, is_group, sy );

        // Add the item to the group
        pgroup->add_item( pnew.release() );
    }
    return true;
}

template< typename IterT, typename SyntaxT >
inline void basic_rpattern_base<IterT, SyntaxT>::_quantify(
    std::auto_ptr<detail::sub_expr<IterT> > & pnew,
    typename string_type::iterator & ipat,
    bool is_group,
    syntax_type & sy )
{
    if( this->m_pat->end() != ipat && ! pnew->is_assertion() )
    {
        typename string_type::iterator itemp = ipat, itemp2;
        bool fmin = false;

        // Since size_t is unsigned, -1 is really the largest size_t
        size_t lbound = ( size_t )-1;
        size_t ubound = ( size_t )-1;
        size_t ubound_tmp;

        switch( sy.quant_token( itemp, this->m_pat->end() ) )
        {
        case ZERO_OR_MORE_MIN:
            fmin = true;
        case ZERO_OR_MORE:
            lbound = 0;
            break;

        case ONE_OR_MORE_MIN:
            fmin = true;
        case ONE_OR_MORE:
            lbound = 1;
            break;

        case ZERO_OR_ONE_MIN:
            fmin = true;
        case ZERO_OR_ONE:
            lbound = 0;
            ubound = 1;
            break;

        case BEGIN_RANGE:
            lbound = detail::parse_int( itemp, this->m_pat->end() );
            if( this->m_pat->end() == itemp )
                return; // not a valid quantifier - treat as atom

            switch( sy.quant_token( itemp, this->m_pat->end() ) )
            {
            case END_RANGE_MIN:
                fmin = true;
            case END_RANGE:
                ubound = lbound;
                break;

            case RANGE_SEPARATOR:
                itemp2 = itemp;
                ubound_tmp = detail::parse_int( itemp, this->m_pat->end() );
                if( itemp != itemp2 )
                    ubound = ubound_tmp;
                if( itemp == this->m_pat->end() )
                    return; // not a valid quantifier - treat as atom
                switch( sy.quant_token( itemp, this->m_pat->end() ) )
                {
                case END_RANGE_MIN:
                    fmin = true;
                case END_RANGE:
                    break;
                default:
                    return; // not a valid quantifier - treat as atom
                }
                break;

            default:
                return; // not a valid quantifier - treat as atom
            }

            if( ubound < lbound  )
                throw bad_regexpr( "Can't do {n, m} with n > m" );

            break;

        default:
            break;
        }

        if( ( size_t )-1 != lbound )
        {
            // If we are quantifying a group, then this pattern could recurse
            // deeply. Note that fact here so that we can opt to use a stack-
            // conservative algorithm at match time.
            if( is_group && ubound > 16 )
                this->m_fok_to_recurse = false;

            std::auto_ptr<detail::sub_expr<IterT> > pquant( pnew->quantify( lbound, ubound, ! fmin, this->m_arena ) );
            pnew.release();
            detail::reset_auto_ptr( pnew, pquant.release() );
            ipat = itemp;
        }
    }
}

template< typename IterT, typename SyntaxT >
inline void basic_rpattern_base<IterT, SyntaxT>::_add_subst_backref(
    detail::subst_node & snode,
    size_t nbackref,
    ptrdiff_t rstart,
    bool & uses_backrefs,
    detail::subst_list_type & subst_list ) const
{
    uses_backrefs = true;
    REGEX_ASSERT( detail::subst_node::SUBST_STRING == snode.m_stype );
    if( snode.m_subst_string.m_rlength )
        subst_list.push_back( snode );

    snode.m_stype = detail::subst_node::SUBST_BACKREF;
    snode.m_subst_backref = nbackref;
    subst_list.push_back( snode );

    // re-initialize the subst_node
    snode.m_stype = detail::subst_node::SUBST_STRING;
    snode.m_subst_string.m_rstart = rstart;
    snode.m_subst_string.m_rlength = 0;
}

template< typename IterT, typename SyntaxT >
inline void basic_rpattern_base<IterT, SyntaxT>::_parse_subst(
    string_type & subst,
    bool & uses_backrefs,
    detail::subst_list_type & subst_list ) const
{
    TOKEN tok;
    detail::subst_node snode;
    typename string_type::iterator icur = subst.begin();
    size_t nbackref;
    typename string_type::iterator itemp;
    bool fdone;
    syntax_type sy( this->m_flags );

    uses_backrefs = false;

    // Initialize the subst_node
    snode.m_stype = detail::subst_node::SUBST_STRING;
    snode.m_subst_string.m_rstart = 0;
    snode.m_subst_string.m_rlength = 0;

    while( subst.end() != icur )
    {
        switch( tok = sy.subst_token( icur, subst.end() ) )
        {
        case SUBST_MATCH:
            _add_subst_backref( snode, 0, std::distance( subst.begin(), icur ), uses_backrefs, subst_list );
            break;

        case SUBST_PREMATCH:
            _add_subst_backref( snode, ( size_t )detail::subst_node::PREMATCH, std::distance( subst.begin(), icur ), uses_backrefs, subst_list );
            break;

        case SUBST_POSTMATCH:
            _add_subst_backref( snode, ( size_t )detail::subst_node::POSTMATCH, std::distance( subst.begin(), icur ), uses_backrefs, subst_list );
            break;

        case SUBST_BACKREF:
            nbackref = detail::parse_int( icur, subst.end(), cgroups() - 1 ); // always at least 1 group
            if( 0 == nbackref )
                throw bad_regexpr( "invalid backreference in substitution" );

            _add_subst_backref( snode, nbackref, std::distance( subst.begin(), icur ), uses_backrefs, subst_list );
            break;

        case SUBST_QUOTE_META_ON:
            REGEX_ASSERT( detail::subst_node::SUBST_STRING == snode.m_stype );
            if( snode.m_subst_string.m_rlength )
                subst_list.push_back( snode );

            snode.m_subst_string.m_rstart = std::distance( subst.begin(), icur );
            for( itemp = icur, fdone = false; !fdone && subst.end() != icur; )
            {
                switch( tok = sy.subst_token( icur, subst.end() ) )
                {
                case SUBST_ALL_OFF:
                    fdone = true;
                    break;
                case NO_TOKEN:
                    ++icur; // fall-through
                default:
                    itemp = icur;
                    break;
                }
            }
            snode.m_subst_string.m_rlength = std::distance( subst.begin(), itemp ) - snode.m_subst_string.m_rstart;
            if( snode.m_subst_string.m_rlength )
                subst_list.push_back( snode );

            if( tok == SUBST_ALL_OFF )
            {
                snode.m_stype = detail::subst_node::SUBST_OP;
                snode.m_op    = detail::subst_node::ALL_OFF;
                subst_list.push_back( snode );
            }

            // re-initialize the subst_node
            snode.m_stype = detail::subst_node::SUBST_STRING;
            snode.m_subst_string.m_rstart = std::distance( subst.begin(), icur );
            snode.m_subst_string.m_rlength = 0;
            break;

        case SUBST_UPPER_ON:
        case SUBST_UPPER_NEXT:
        case SUBST_LOWER_ON:
        case SUBST_LOWER_NEXT:
        case SUBST_ALL_OFF:
            REGEX_ASSERT( detail::subst_node::SUBST_STRING == snode.m_stype );
            if( snode.m_subst_string.m_rlength )
                subst_list.push_back( snode );

            snode.m_stype = detail::subst_node::SUBST_OP;
            snode.m_op    = static_cast<detail::subst_node::op_type>( tok );
            subst_list.push_back( snode );

            // re-initialize the subst_node
            snode.m_stype = detail::subst_node::SUBST_STRING;
            snode.m_subst_string.m_rstart = std::distance( subst.begin(), icur );
            snode.m_subst_string.m_rlength = 0;
            break;

        case SUBST_ESCAPE:
            if( subst.end() == icur )
                throw bad_regexpr( "expecting escape sequence in substitution string" );
            REGEX_ASSERT( detail::subst_node::SUBST_STRING == snode.m_stype );
            if( snode.m_subst_string.m_rlength )
                subst_list.push_back( snode );
            snode.m_subst_string.m_rstart = std::distance( subst.begin(), icur++ );
            snode.m_subst_string.m_rlength = 1;
            break;

        case NO_TOKEN:
        default:
            ++snode.m_subst_string.m_rlength;
            ++icur;
            break;
        }
    }
    REGEX_ASSERT( detail::subst_node::SUBST_STRING == snode.m_stype );
    if( snode.m_subst_string.m_rlength )
        subst_list.push_back( snode );
}



template< typename CharT >
REGEXPR_H_INLINE void reset_intrinsic_charsets( CharT )
{
    detail::intrinsic_charsets<CharT>::reset();
}

typedef regex::detail::select
<
    REGEX_FOLD_INSTANTIATIONS &&
        detail::is_convertible<char const *,std::string::const_iterator>::value,
    std::string::const_iterator,
    char const *
>::type lpcstr_t;

typedef regex::detail::select
<
    REGEX_FOLD_INSTANTIATIONS &&
        detail::is_convertible<wchar_t const *,std::wstring::const_iterator>::value,
    std::wstring::const_iterator,
    wchar_t const *
>::type lpcwstr_t;

namespace detail
{

// Here is the main dispatch loop for the iterative match routine.
// It is responsible for calling match on the current sub-expression
// and repeating for the next sub-expression. It also backtracks
// the match when it needs to.
template< typename CStringsT, typename IterT >
inline bool _do_match_iterative( sub_expr_base<IterT> const * expr, match_param<IterT> & param, IterT icur, CStringsT )
{
    unsafe_stack::stack_guard guard( param.m_pstack );
    unsafe_stack & s = *param.m_pstack;
    void *const jump_ptr = s.set_jump(); // the bottom of the stack
    param.m_icur = icur;

    if( ! expr->iterative_match_this_helper( param, CStringsT() ) )
    {
        return false;
    }

    for( ;; )
    {
        do
        {
            if( param.m_pnext == 0 ) // This means we're done
                return true;
            s.push( expr );
            expr = param.m_pnext;
        }
        while( expr->iterative_match_this_helper( param, CStringsT() ) );

        do
        {
            if( jump_ptr == s.set_jump() ) // No more posibilities to try
                return false;
            s.pop( expr );
        }
        while( ! expr->iterative_rematch_this_helper( param, CStringsT() ) );
    }
}

template< typename IterT >
REGEXPR_H_INLINE bool matcher_helper<IterT>::_do_match_iterative_helper( sub_expr_base<IterT> const * expr, match_param<IterT> & param, IterT icur )
{
    return _do_match_iterative( expr, param, icur, false_t() );
}

template< typename IterT >
REGEXPR_H_INLINE bool matcher_helper<IterT>::_do_match_iterative_helper_c( sub_expr_base<IterT> const * expr, match_param<IterT> & param, IterT icur )
{
    return _do_match_iterative( expr, param, icur, true_t() );
}

template< typename IterT >
REGEXPR_H_INLINE bool matcher_helper<IterT>::_do_match_recursive( sub_expr_base<IterT> const * expr, match_param<IterT> & param, IterT icur )
{
    return static_cast<match_group_base<IterT> const*>(expr)->match_group_base<IterT>::recursive_match_all_( param, icur );
}

template< typename IterT >
REGEXPR_H_INLINE bool matcher_helper<IterT>::_do_match_recursive_c( sub_expr_base<IterT> const * expr, match_param<IterT> & param, IterT icur )
{
    return static_cast<match_group_base<IterT> const*>(expr)->match_group_base<IterT>::recursive_match_all_c( param, icur );
}

template< typename IterT >
REGEX_NOINLINE bool matcher_helper<IterT>::_do_match_with_stack( rpattern_type const & pat, match_param<IterT> & param, bool const use_null )
{
    unsafe_stack s;
    param.m_pstack = &s;
    return _do_match_impl( pat, param, use_null );
}

template< typename IterT >
REGEXPR_H_INLINE bool matcher_helper<IterT>::_do_match_impl( rpattern_type const & pat, match_param<IterT> & param, bool const use_null )
{
    typedef bool ( *pfndomatch_t )( sub_expr_base<IterT> const * expr, match_param<IterT> & param, IterT icur );

    bool       floop   = pat._loops();
    unsigned   flags   = pat.flags();
    width_type nwidth  = pat.get_width();

    // Create some aliases for convenience and effeciency.
    REGEX_ASSERT( 0 != param.m_prgbackrefs );

    // If the pstack parameter is not NULL, we should do a safe, iterative match.
    // Otherwise, we should do a fast, recursive match.
    pfndomatch_t pfndomatch;
    if( 0 != param.m_pstack )
        if( use_null )
            pfndomatch = &_do_match_iterative_helper_c;
        else
            pfndomatch = &_do_match_iterative_helper;
    else
        if( use_null )
            pfndomatch = &_do_match_recursive_c;
        else
            pfndomatch = &_do_match_recursive;

    sub_expr_base<IterT> const * pfirst = pat._get_first_subexpression();
    param.m_pfirst = pfirst;

    REGEX_ASSERT( param.m_cbackrefs == pat._cgroups_total() );
    std::fill_n( param.m_prgbackrefs, param.m_cbackrefs, static_init<backref_type>::value );

    if( ! use_null )
    {
        // If the minimum width of the pattern exceeds the width of the
        // string, a succesful match is impossible
        typedef typename std::iterator_traits<IterT>::difference_type diff_type;
        diff_type room = std::distance( param.m_imatchbegin, param.m_iend );

        if( nwidth.m_min <= static_cast<size_t>( room ) )
        {
            IterT local_iend = param.m_iend;
            std::advance( local_iend, -static_cast<diff_type>( nwidth.m_min ) );

            if( RIGHTMOST & flags )
            {
                // begin trying to match after the last character.
                // Continue to the beginning
                for( IterT icur = local_iend; ; --icur, param.m_no0len = false )
                {
                    if( ( *pfndomatch )( pfirst, param, icur ) )
                        break; // m_floop not used for rightmost matches
                    if( icur == param.m_imatchbegin )
                        break;
                }
            }
            else
            {
                // begin trying to match before the first character.
                // Continue to the end
                if( is_random_access<IterT>::value && pat.m_search )
                {
                    IterT icur = pat.m_search->find( param.m_imatchbegin, param.m_iend );
                    while( icur != param.m_iend )
                    {
                        if( ( *pfndomatch )( pfirst, param, icur ) || ! floop )
                            break;
                        param.m_no0len = false;
                        icur = pat.m_search->find( ++icur, param.m_iend );
                    }
                }
                else
                {
                    for( IterT icur = param.m_imatchbegin; ; ++icur, param.m_no0len = false )
                    {
                        if( ( *pfndomatch )( pfirst, param, icur ) || ! floop )
                            break;
                        if( icur == local_iend )
                            break;
                    }
                }
            }
        }
    }
    else
    {
        REGEX_ASSERT( 0 == ( RIGHTMOST & flags ) );
        // begin trying to match before the first character.
        // Continue to the end
        for( IterT icur = param.m_imatchbegin; ; ++icur, param.m_no0len = false )
        {
            if( ( *pfndomatch )( pfirst, param, icur ) || ! floop )
                break;
            if( traits_type::eq( *icur, char_type() ) )
                break;
        }
    }

    return param.m_prgbackrefs[0].matched;
}

// Here is a rudimentary typelist facility to allow the REGEX_TO_INSTANTIATE
// list to recursively generate the instantiations we are interested in.
struct empty_typelist
{
};

template< typename HeadT, typename TailT >
struct cons
{
    typedef HeadT head_type;
    typedef TailT tail_type;
};

template
<
    typename T1 =empty_typelist, typename T2 =empty_typelist, typename T3 =empty_typelist,
    typename T4 =empty_typelist, typename T5 =empty_typelist, typename T6 =empty_typelist,
    typename T7 =empty_typelist, typename T8 =empty_typelist, typename T9 =empty_typelist,
    typename T10=empty_typelist, typename T11=empty_typelist, typename T12=empty_typelist
>
struct typelist : public cons<T1,typelist<T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12> >
{
};

template<>
struct typelist
<
    empty_typelist,empty_typelist,empty_typelist,empty_typelist,
    empty_typelist,empty_typelist,empty_typelist,empty_typelist,
    empty_typelist,empty_typelist,empty_typelist,empty_typelist
>
    : public empty_typelist
{
};

// This class is responsible for instantiating basic_rpattern
// with the template parameters we are interested in. It also
// instntiates any helper routines this basic_rpattern relies
// on.
template< typename IterT, typename SyntaxT >
struct rpattern_instantiator : protected regex::basic_rpattern<IterT,SyntaxT>
{
    static instantiator instantiate()
    {
        typedef typename std::iterator_traits<IterT>::value_type char_type;
        void (*pfn)( char_type ) = &reset_intrinsic_charsets;

        return regex::basic_rpattern<IterT,SyntaxT>::instantiate() +
            matcher_helper<IterT>::instantiate() +
            instantiator_helper( pfn );
    }
};

// The regex_instantiate uses typelists and the rpattern_instantiator
// to generate instantiations for all the types in the typelist.
template< typename SyntaxT >
instantiator regex_instantiate( empty_typelist, type2type<SyntaxT> )
{
    return instantiator();
}

template< typename HeadT, typename TailT, typename SyntaxT >
instantiator regex_instantiate( cons<HeadT,TailT>, type2type<SyntaxT> )
{
    typedef typename std::iterator_traits<HeadT>::value_type char_type;
    typedef typename SyntaxT::template rebind<char_type>::other syntax_type;

    return rpattern_instantiator<HeadT,syntax_type>::instantiate() +
        regex_instantiate( TailT(), type2type<SyntaxT>() );
}

// Here is a list of types to instantiate.
#ifndef REGEX_TO_INSTANTIATE
# ifdef REGEX_WIDE_AND_NARROW
#  define REGEX_TO_INSTANTIATE std::string::const_iterator,  \
                               std::wstring::const_iterator, \
                               lpcstr_t,                     \
                               lpcwstr_t
# else
#  define REGEX_TO_INSTANTIATE restring::const_iterator,     \
                               lpctstr_t
# endif
#endif

typedef typelist<REGEX_TO_INSTANTIATE> regex_typelist;
typedef type2type<perl_syntax<char> >  perl_type;
typedef type2type<posix_syntax<char> > posix_type;

namespace
{
// Create the perl instantiations
#ifndef REGEX_NO_PERL
instantiator const perl_inst = regex_instantiate( regex_typelist(), perl_type() );
#endif

// Create the posix instantiations
#ifdef REGEX_POSIX
instantiator const posix_inst = regex_instantiate( regex_typelist(), posix_type() );
#endif
}

} // unnamed namespace

} // namespace regex

#ifdef _MSC_VER
# pragma warning( pop )
#endif
