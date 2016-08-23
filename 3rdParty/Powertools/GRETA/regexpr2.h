//+---------------------------------------------------------------------------
//
//  Copyright ( C ) Microsoft, 1994 - 2002.
//
//  File:       regexpr2.h
//
//  Contents:   classes for regular expression pattern matching a-la perl
//
//  Classes:    basic_rpattern_base
//
//  Functions:  rpattern::match
//              rpattern::substitute
//              match_results::cbackrefs
//              match_results::backref
//              match_results::all_backrefs
//              match_results::backref_str
//
//  Author:     Eric Niebler ( ericne@microsoft.com )
//
//----------------------------------------------------------------------------

#ifndef REGEXPR_H
#define REGEXPR_H

#ifdef _MSC_VER
  // warning C4189: local variable is initialized but not referenced
  // warning C4702: unreachable code
  // warning C4710: function 'blah' not inlined
  // warning C4786: identifier was truncated to '255' characters in the debug information
# pragma warning( push )
# pragma warning( disable : 4189 4702 4710 4786 )
# define REGEX_SEH_STACK_OVERFLOW 0xC00000FDL
# if 1200 < _MSC_VER
# include <malloc.h> // for _resetstkoflw
# else
  extern "C" int __cdecl _resetstkoflw(void);
# endif
  extern "C" unsigned long __cdecl _exception_code(void);
#endif

#include <list>
#include <iosfwd>
#include <string>
#include <vector>
#include <memory>
#include <cwctype>
#include "syntax2.h"
#include "restack.h"

namespace regex
{

// This is the default alignment for the unsafe heterogeneous stack.
// If you are getting a compiler error in one of the unsafe_stack
// methods, then compile with -DREGEX_STACK_ALIGNMENT=16 or 32
#ifndef REGEX_STACK_ALIGNMENT
# define REGEX_STACK_ALIGNMENT sizeof( void* )
#endif

#if !defined( REGEX_DEBUG ) & ( defined( DEBUG ) | defined( _DEBUG ) | defined( DBG ) )
# define REGEX_DEBUG 1
#else
# define REGEX_DEBUG 0
#endif

#if !defined( REGEX_DEBUG_ITERATORS ) & defined( _HAS_ITERATOR_DEBUGGING )
# define REGEX_DEBUG_ITERATORS 1
#else
# define REGEX_DEBUG_ITERATORS 0
#endif

namespace detail
{
#if REGEX_DEBUG | REGEX_DEBUG_ITERATORS
    // Turn on hetero_stack's run-time type checking
    typedef hetero_stack<REGEX_STACK_ALIGNMENT,true,false,32,0>       unsafe_stack;
#else
    // Assume that all types pushed on stack have trivial destructors.
    typedef hetero_stack<REGEX_STACK_ALIGNMENT,false,true,4096,1024>  unsafe_stack;
#endif

    // Used to initialize variables with the same value they would have
    // if they were initialized as a static global. ( Ptrs get NULL,
    // integer types get 0, etc, etc )
    template< typename T > struct static_init { static T const value; };
    template< typename T > T const static_init<T>::value = T();

    //
    // Forward declarations
    //
    template< typename IterT > class  sub_expr;
    template< typename IterT > class  match_group_base;
    template< typename IterT > class  basic_rpattern_base_impl;
    template< typename IterT > struct match_param;
    template< typename IterT > struct sub_expr_base;
    template< typename IterT > struct matcher_helper;

    // an iterator that keeps track of whether it is singular or not.
    template< typename IterT > struct smart_iter
    {
        IterT m_iter;
        bool m_valid;

        smart_iter()
            : m_iter( static_init<IterT>::value )
            , m_valid( false )
        {
        }
        smart_iter( smart_iter const & rhs )
            : m_iter( rhs.m_iter )
            , m_valid( rhs.m_valid )
        {
        }
        smart_iter( IterT iter ) // implicit conversion OK!
            : m_iter( iter )
            , m_valid( true )
        {
        }
        smart_iter & operator=( smart_iter const & rhs )
        {
            m_iter  = rhs.m_iter;
            m_valid = rhs.m_valid;
            return *this;
        }
        friend bool operator==( smart_iter const & lhs, smart_iter const & rhs )
        {
            if( !lhs.m_valid || !rhs.m_valid )
                return lhs.m_valid == rhs.m_valid;
            else
                return lhs.m_iter == rhs.m_iter;
        }
        friend bool operator!=( smart_iter const & lhs, smart_iter const & rhs )
        {
            return ! operator==( lhs, rhs );
        }
    };

    template< typename IterT > struct iter_select
    {
        typedef typename select
        <
            REGEX_DEBUG_ITERATORS && !is_scalar<IterT>::value,
            smart_iter<IterT>,
            IterT
        >::type     type;
    };

    template< int SizeT > struct type_with_size { char buffer[ SizeT ]; };

    // make up for the fact that the VC6 std::allocator does
    // not have template constructors
    template< typename ToT, typename FromT >
    std::allocator<ToT> convert_allocator( std::allocator<FromT>, int )
    {
        return std::allocator<ToT>();
    }
    
    template< typename ToT, typename FromT >
    FromT const & REGEX_CDECL convert_allocator( FromT const & from, ... )
    {
        return from;
    }

    template< int > struct rebind_helper;

    // unknown allocator
    template< typename T >
    type_with_size<1> REGEX_CDECL allocator_picker( T const &, ... );

    template<> struct rebind_helper<1>
    {
        template< typename AllocT, typename ElemT >
        struct inner
        {
            REGEX_NVC6( typedef typename AllocT::template rebind<ElemT>::other type; )
        };
    };

    // std::allocator
    template< typename T >
    type_with_size<2> allocator_picker( std::allocator<T> const &, int );

    template<> struct rebind_helper<2>
    {
        template< typename, typename ElemT >
        struct inner
        {
            typedef std::allocator<ElemT> type;
        };
    };

    template< typename AllocT, typename ElemT >
    struct rebind
    {
        enum { alloc_type = sizeof(allocator_picker(factory<AllocT>::make(),0)) };

        typedef typename rebind_helper<alloc_type>::template inner<AllocT,ElemT>::type type;
    };
}

// --------------------------------------------------------------------------
//
// Class:       width_type
//
// Description: represents the width of a sub-expression
//
// Members:     m_min      - smallest number of characters a sub-expr can span
//              m_max      - largest number of characters a sub-expr can span
//
// History:     8/14/2000 - ericne - Created
//
// --------------------------------------------------------------------------
struct width_type
{
    size_t m_min;
    size_t m_max;
};

inline width_type const uninit_width()
{
    width_type const width = { size_t( -1 ), size_t( -1 ) };
    return width;
}

// Helper function for processing escape sequences
template< typename CharT, typename TraitsT, typename AllocT >
void process_escapes( std::basic_string<CharT, TraitsT, AllocT> & str, bool fPattern = false ); //throw()

// --------------------------------------------------------------------------
//
// Class:       backref_tag
//
// Description: Struct which contains a back-reference.  It is a template
//              on the iterator type.
//
// Methods:     backref_tag   - c'tor
//              operator bool - so that if( br ) is true if this br matched
//              operator!     - inverse of operator bool()
//
// Members:     reserved      - move along, nothing to see here
//
// History:     8/9/2001 - ericne - Created
//
// --------------------------------------------------------------------------
template< typename IterT >
class backref_tag : public std::pair<IterT, IterT>
{
    struct detail_t { detail_t * d; };

    template< typename OStreamT, typename OtherT >
    void REGEX_CDECL _do_print( OStreamT & sout, OtherT, ... ) const
    {
        typedef typename OStreamT::char_type char_type;
        typedef typename OStreamT::traits_type traits_type;
        std::ostreambuf_iterator<char_type, traits_type> iout( sout );
        for( IterT iter = first; iter != second; ++iter, ++iout )
            *iout = *iter;
    }

    // overload that is optimized for bare char*
    template< typename OStreamT >
    void _do_print( OStreamT & sout, typename OStreamT::char_type const *, int ) const
    {
        sout.write( first, static_cast<std::streamsize>( std::distance( first, second ) ) );
    }

public:
    typedef IterT iterator_type;
    typedef typename std::iterator_traits<IterT>::value_type char_type;
    typedef std::basic_string<char_type> string_type;

    typedef typename detail::iter_select<IterT>::type smart_iter_type;

    explicit backref_tag
    (
        IterT i1 = detail::static_init<IterT>::value,
        IterT i2 = detail::static_init<IterT>::value
    )
        : std::pair<IterT, IterT>( i1, i2 )
        , matched( false )
        , reserved1( i1 )
        , reserved2( 0 )
        , reserved3( false )
        , reserved4( detail::static_init<smart_iter_type>::value )
        , reserved5( detail::static_init<smart_iter_type>::value )
    {
    }

    IterT begin() const
    {
        return first;
    }

    IterT end() const
    {
        return second;
    }

    string_type const str() const
    {
        return matched ? string_type( first, second ) : string_type();
    }

    // Use the "safe bool" idiom. This allows implicit conversion to bool,
    // but not to int. It also disallows conversion to void*.
    typedef detail_t * detail_t::* bool_type;

    operator bool_type() const //throw()
    {
        return matched ? &detail_t::d : 0;
    }

    bool operator!() const //throw()
    {
        return ! matched;
    }

    template< typename CharT, typename TraitsT >
    std::basic_ostream<CharT, TraitsT> & print( std::basic_ostream<CharT, TraitsT> & sout ) const
    {
        _do_print( sout, IterT(), 0 );
        return sout;
    }

    bool   matched;

//private:
    IterT           reserved1; // used for internal book-keeping
    size_t          reserved2; // used for internal book-keeping
    bool            reserved3; // used for internal book-keeping
    smart_iter_type reserved4; // used for internal book-keeping
    smart_iter_type reserved5; // used for internal book-keeping
};

//namespace detail
//{
    // indexing into the backref vector is faster if the backref_tag struct
    // has a size that is a power of 2.
    //static static_assert<32==sizeof(backref_tag<char*>)> const check_backref_size;
//}

// --------------------------------------------------------------------------
//
// Class:       basic_match_results
//
// Description: Use this structure for returning match/substitute results
//              out from the match()/substitute() methods.
//
// Methods:     cbackrefs      -
//              backref        -
//              all_backrefs   -
//              rlength        -
//
// Members:     m_rgbackrefs   -
//
// Typedefs:    const_iterator -
//              backref_type   -
//              backref_vector -
//
// History:     8/8/2001 - ericne - Created
//
// --------------------------------------------------------------------------
template
<
    typename IterT,
    typename AllocT = std::allocator< REGEX_DEPENDENT_TYPENAME std::iterator_traits<IterT>::value_type >
>
struct basic_match_results
{
    // const_iterator is deprecated. Use iterator_type instead.
    REGEX_DEPRECATED typedef IterT                                const_iterator;
    typedef IterT                                                 iterator_type;
    typedef backref_tag<IterT>                                    backref_type;
    typedef typename detail::rebind<AllocT, backref_type>::type   allocator_type;
    typedef std::vector<backref_type, allocator_type>             backref_vector;
    friend struct detail::matcher_helper<IterT>;

    explicit basic_match_results( allocator_type const & alloc = allocator_type() )
        : m_rgbackrefs( alloc )
    {
    }

    virtual ~basic_match_results()
    {
    }

    size_t cbackrefs() const //throw()
    {
        return m_rgbackrefs.size();
    }

    backref_type const & backref( size_t cbackref ) const //throw( std::out_of_range )
    {
        return m_rgbackrefs.at( cbackref );
    }

    backref_vector const & all_backrefs() const //throw()
    {
        return m_rgbackrefs;
    }

    size_t rstart( size_t cbackref = 0 ) const //throw( std::out_of_range )
    {
        return std::distance( m_ibegin, m_rgbackrefs.at( cbackref ).first );
    }

    size_t rlength( size_t cbackref = 0 ) const //throw( std::out_of_range )
    {
        return std::distance( m_rgbackrefs.at( cbackref ).first, m_rgbackrefs.at( cbackref ).second );
    }

private:
    backref_vector m_rgbackrefs;
    IterT           m_ibegin;
};

// Unnecessary and deprecated
template< typename CharT, typename AllocT = std::allocator<CharT> >
struct basic_match_results_c : public basic_match_results<CharT const *, AllocT>
{
    typedef basic_match_results<CharT const *, AllocT> base;
    REGEX_DEPRECATED typedef typename base::const_iterator const_iterator;
    typedef typename base::iterator_type  iterator_type;
    typedef typename base::backref_type   backref_type;
    typedef typename base::allocator_type allocator_type;
    typedef typename base::backref_vector backref_vector;

    explicit basic_match_results_c( allocator_type const & alloc = allocator_type() )
        : basic_match_results<CharT const *, AllocT>( alloc )
    {
    }
};

template< typename CharT, typename TraitsT, typename AllocT >
struct subst_results_base
{
    typedef typename detail::rebind<AllocT, CharT>::type              char_allocator_type;
    typedef std::basic_string<CharT, TraitsT, char_allocator_type>    string_type;
    typedef typename string_type::const_iterator                    iterator_type;
    typedef basic_match_results<iterator_type,AllocT>                type;
};

//
// For storing the results of a substitute() operation
//
template
<
    typename CharT,
    typename TraitsT = std::char_traits<CharT>,
    typename AllocT  = std::allocator<CharT>
>
struct basic_subst_results : public subst_results_base<CharT, TraitsT, AllocT>::type
{
    typedef typename detail::rebind<AllocT, CharT>::type              char_allocator_type;
    typedef std::basic_string<CharT, TraitsT, char_allocator_type>    string_type;
    typedef typename string_type::const_iterator                    iterator_type;
    typedef basic_match_results<iterator_type, AllocT>               base;
    typedef typename base::backref_type                             backref_type;
    typedef typename base::allocator_type                           allocator_type;
    typedef typename base::backref_vector                           backref_vector;
    friend struct detail::matcher_helper<iterator_type>;

    explicit basic_subst_results( allocator_type const & alloc = allocator_type() )
        : basic_match_results< iterator_type, AllocT >( alloc )
        , m_backref_str( detail::convert_allocator<CharT>( alloc, 0 ) )
        , m_pbackref_str( &m_backref_str )
    {
    }

    string_type const & backref_str() const //throw()
    {
        return *m_pbackref_str;
    }

private:
    string_type         m_backref_str;
    string_type const * m_pbackref_str;
};


template< typename CharT, typename TraitsT, typename AllocT >
struct split_results_base
{
    typedef typename detail::rebind<AllocT, CharT>::type            char_allocator_type;
    typedef std::basic_string<CharT, TraitsT, char_allocator_type>  string_type;
    typedef typename detail::rebind<AllocT, string_type>::type      allocator_type;
    typedef std::vector<string_type,allocator_type>                 type;
};

//
// For storing the results of a split() operation
//
template
<
    typename CharT,
    typename TraitsT = std::char_traits<CharT>,
    typename AllocT  = std::allocator<CharT>
>
struct basic_split_results : private split_results_base<CharT, TraitsT, AllocT>::type
{
    typedef CharT                                                    char_type;
    typedef typename detail::rebind<AllocT, CharT>::type              char_allocator_type;
    typedef std::basic_string<CharT, TraitsT, char_allocator_type>    string_type;
    typedef typename detail::rebind<AllocT, string_type>::type       allocator_type;
    typedef std::vector<string_type,allocator_type>                 string_vector;
    typedef string_vector                                           base;

    explicit basic_split_results( allocator_type const & alloc = allocator_type() )
        : base( alloc )
    {
    }

#if !defined(_MSC_VER) | 1200 < _MSC_VER
    typedef typename allocator_type::pointer            pointer;
    typedef typename allocator_type::const_pointer      const_pointer;
#else
    typedef string_type *                               pointer;
    typedef string_type const *                         const_pointer;
#endif

    // shortcuts to the most basic read-only container operations
    using base::size_type;
    using base::difference_type;
    using base::value_type;
    using base::reference;
    using base::const_reference;
    using base::iterator;
    using base::const_iterator;
    using base::reverse_iterator;
    using base::const_reverse_iterator;
    using base::begin;
    using base::end;
    using base::rbegin;
    using base::rend;
    using base::operator[];
    using base::at;
    using base::size;
    using base::front;
    using base::back;

    string_vector & strings()
    {
        return *this;
    }

    string_vector const & strings() const
    {
        return *this;
    }
};

//
// The REGEX_MODE is a way of controlling how matching occurs.
//
enum REGEX_MODE
{
    MODE_FAST,  // Uses the fast, recursive algorithm. Could overflow stack.
    MODE_SAFE,  // Uses the slow, iterative algorithm. Can't overflow stack.
    MODE_MIXED, // Uses a heuristic to automatically determine which algorithm
                // is the most appropriate for this pattern.

    // MS VC++ has structured exception handling, which makes the
    // consequences of a stack overflow much less severe. Because of this,
    // it is possible to use the "fast" algorithm always on MS platforms,
#ifdef _MSC_VER
    MODE_DEFAULT = MODE_FAST
#else
    MODE_DEFAULT = MODE_MIXED
#endif
};

//
// helper function for resetting the intrinsic character sets.
// This should be called after changing the locale with setlocale()
//
template< typename CharT >
void reset_intrinsic_charsets( CharT ch = CharT( 0 ) );

// This is for implementation details that really belong in the
// cpp file, but can't go there because of template strangeness.
#include "reimpl2.h"

// --------------------------------------------------------------------------
//
// Class:       basic_rpattern_base
//
// Description:
//
// Methods:     basic_rpattern_base - c'tor
//              basic_rpattern_base -
//              basic_rpattern_base -
//              init                - ( re )initialize the pattern
//              init                -
//              set_substitution    - set the substitution string
//              _find_next_group    - parse the next group of the pattern
//              _find_next          - parse the next sub_expr of the pattern
//              _find_atom          - parse the next atom of the pattern
//              _quantify           - quantify the sub_expr
//              _common_init        - perform some common initialization tasks
//              _parse_subst        - parse the substitution string
//              _add.m_subst_backref  - add a backref node to the subst list
//
// Members:     m_invisible_groups  - list of hidden groups
//
// Typedefs:    syntax_type         -
//              backref_type        -
//              backref_vector      -
//              string_type         -
//              size_type           -
//
// History:     8/14/2000 - ericne - Created
//              8/5/2001 - ericne - complete overhaul
//
// --------------------------------------------------------------------------
template< typename IterT, typename SyntaxT >
class basic_rpattern_base : protected detail::basic_rpattern_base_impl<IterT>
{
protected:
    typedef detail::basic_rpattern_base_impl<IterT>   impl;
public:
    typedef SyntaxT syntax_type;
    typedef typename impl::char_type       char_type;
    typedef typename impl::traits_type     traits_type;

    typedef typename impl::string_type     string_type;
    typedef typename impl::size_type       size_type;

    typedef typename impl::backref_type    backref_type;
    typedef typename impl::backref_vector  backref_vector;

    void init
    (
        string_type const & pat,
        REGEX_FLAGS flags = NOFLAGS,
        REGEX_MODE mode = MODE_DEFAULT
    );                                                      //throw( bad_regexpr, std::bad_alloc );

    void init
    (
        string_type const & pat,
        string_type const & subst,
        REGEX_FLAGS flags = NOFLAGS,
        REGEX_MODE mode = MODE_DEFAULT
    );                                                      //throw( bad_regexpr, std::bad_alloc );

    void set_substitution
    (
        string_type const & subst
    );                                                      //throw( bad_regexpr, std::bad_alloc );

    using impl::flags;
    using impl::mode;
    using impl::get_width;
    using impl::cgroups;
    using impl::get_pat;
    using impl::get_subst;
    using impl::swap;
    using impl::npos;

protected:
    basic_rpattern_base() //throw()
        : detail::basic_rpattern_base_impl<IterT>()
    {
    }

    basic_rpattern_base( basic_rpattern_base<IterT, SyntaxT> const & that )                                                   //throw()
        : detail::basic_rpattern_base_impl<IterT>( that.flags(), that.mode(), that.get_pat(), that.get_subst() )
    {
        // Don't call _normalize_string(). If that.flags()&NORMALIZE,
        // then subst has already been normalized.
        _common_init( this->m_flags );
        _parse_subst( *this->m_subst, this->m_fuses_backrefs, this->m_subst_list ); // must come after _common_init
    }

    explicit basic_rpattern_base
    (
        string_type const & pat,
        REGEX_FLAGS flags = NOFLAGS,
        REGEX_MODE mode = MODE_DEFAULT
    )                                                   //throw( bad_regexpr, std::bad_alloc )
        : detail::basic_rpattern_base_impl<IterT>( flags, mode, pat )
    {
        _common_init( this->m_flags );
    }

    basic_rpattern_base
    (
        string_type const & pat,
        string_type const & subst,
        REGEX_FLAGS flags = NOFLAGS,
        REGEX_MODE mode = MODE_DEFAULT
    )                                                   //throw( bad_regexpr, std::bad_alloc )
        : detail::basic_rpattern_base_impl<IterT>( flags, mode, pat, subst )
    {
        _common_init( this->m_flags );
        _normalize_string( *this->m_subst );
        _parse_subst( *this->m_subst, this->m_fuses_backrefs, this->m_subst_list ); // must come after _common_init
    }

    basic_rpattern_base & operator=
    (
        basic_rpattern_base<IterT, SyntaxT> const & that
    )                                                   //throw( bad_regexpr, std::bad_alloc )
    {
        basic_rpattern_base<IterT, SyntaxT> temp( that );
        swap( temp );
        return *this;
    }

    detail::match_group_base<IterT> * _find_next_group
    (
        typename string_type::iterator & ipat,
        detail::match_group_base<IterT> * pgroup, syntax_type & sy,
        std::vector<detail::match_group_base<IterT>*> & rggroups
    );

    bool _find_next
    (
        typename string_type::iterator & ipat,
        detail::match_group_base<IterT> * pgroup, syntax_type & sy,
        std::vector<detail::match_group_base<IterT>*> & rggroups
    );

    void _find_atom
    (
        typename string_type::iterator & ipat,
        detail::match_group_base<IterT> * pgroup,
        syntax_type & sy
    );

    void _quantify
    (
        std::auto_ptr<detail::sub_expr<IterT> > & pnew,
        typename string_type::iterator & ipat,
        bool is_group,
        syntax_type & sy
    );

    void _add_subst_backref
    (
        detail::subst_node & snode,
        size_t nbackref,
        ptrdiff_t rstart,
        bool & uses_backrefs,
        detail::subst_list_type & subst_list
    ) const;

    void _parse_subst
    (
        string_type & subst,
        bool & uses_backrefs,
        detail::subst_list_type & subst_list
    ) const;

    void _common_init( REGEX_FLAGS flags );

    static detail::instantiator instantiate()
    {
        typedef basic_rpattern_base this_type;

        return detail::instantiator_helper
        (
            &detail::basic_rpattern_base_impl<IterT>::instantiate,
            static_cast<void (this_type::*)( string_type const &, REGEX_FLAGS, REGEX_MODE )>( &this_type::init ),
            static_cast<void (this_type::*)( string_type const &, string_type const &, REGEX_FLAGS, REGEX_MODE )>( &this_type::init ),
            &this_type::set_substitution,
            &this_type::_find_next_group,
            &this_type::_find_next,
            &this_type::_find_atom,
            &this_type::_add_subst_backref,
            &this_type::_parse_subst,
            &this_type::_common_init
        );
    }
};

// --------------------------------------------------------------------------
//
// Class:       basic_rpattern
//
// Description: generic regex pattern object
//
// Methods:     basic_rpattern - c'tor
//              basic_rpattern -
//              basic_rpattern -
//              match          - match from begin iter to end iter
//              match          - match a null-terminated string
//              match          - match a std::string
//              count          - count matches from begin iter to end iter
//              count          - count matches in a null-terminated string
//              count          - count matches in a std::string
//              substitute     - do substitutions in a std::string
//              _do_match      - internal implementation
//              _do_count      - internal implementation
//
// History:     8/13/2001 - ericne - Created
//
// --------------------------------------------------------------------------
template
<
    typename IterT,
    typename SyntaxT = perl_syntax<REGEX_DEPENDENT_TYPENAME std::iterator_traits<IterT>::value_type>
>
class basic_rpattern : public basic_rpattern_base<IterT, SyntaxT>
{
    typedef detail::basic_rpattern_base_impl<IterT> impl;

    template< typename CharT >
    static void same_char_types( CharT, CharT ) {}

public:
    typedef typename basic_rpattern_base<IterT, SyntaxT>::syntax_type     syntax_type;
    typedef typename basic_rpattern_base<IterT, SyntaxT>::char_type       char_type;
    typedef typename basic_rpattern_base<IterT, SyntaxT>::traits_type     traits_type;

    typedef typename basic_rpattern_base<IterT, SyntaxT>::string_type     string_type;
    typedef typename basic_rpattern_base<IterT, SyntaxT>::size_type       size_type;

    typedef typename basic_rpattern_base<IterT, SyntaxT>::backref_type    backref_type;
    typedef typename basic_rpattern_base<IterT, SyntaxT>::backref_vector  backref_vector;

    basic_rpattern() //throw()
        : basic_rpattern_base<IterT, SyntaxT>()
    {
    }

    basic_rpattern( basic_rpattern const & that )
        : basic_rpattern_base<IterT, SyntaxT>( that )
    {
    }

    explicit basic_rpattern
    (
        string_type const & pat,
        REGEX_FLAGS flags = NOFLAGS,
        REGEX_MODE mode = MODE_DEFAULT
    )                                                   //throw( bad_regexpr, std::bad_alloc )
        : basic_rpattern_base<IterT, SyntaxT>( pat, flags, mode )
    {
    }

    basic_rpattern
    (
        string_type const & pat,
        string_type const & subst,
        REGEX_FLAGS flags = NOFLAGS,
        REGEX_MODE mode = MODE_DEFAULT
    )                                                   //throw( bad_regexpr, std::bad_alloc )
        : basic_rpattern_base<IterT, SyntaxT>( pat, subst, flags, mode )
    {
    }

    basic_rpattern & operator=( basic_rpattern<IterT, SyntaxT> const & that ) //throw( bad_regexpr, std::bad_alloc )
    {
        basic_rpattern_base<IterT, SyntaxT>::operator=( that );
        return *this;
    }

    // Iter2 must be convertible to type IterT
    template< typename OtherT, typename AllocT >
    backref_type const & match
    (
        OtherT ibegin,
        OtherT iend,
        basic_match_results<IterT, AllocT> & results
    ) const
    {
        // If your compile breaks here, it is because OtherT is not
        // convertible to type IterT. Check the declaration of your rpattern object.
        detail::static_assert< detail::is_convertible<OtherT,IterT>::value > const iterator_types_are_not_convertible;
        ( void ) iterator_types_are_not_convertible;

        if( detail::matcher_helper<IterT>::_do_match( *this, results, ibegin, iend, false ) )
        {
            return results.backref(0);
        }
        else
        {
            return detail::static_init<backref_type>::value;
        }
    }

    template< typename CharT, typename AllocT >
    backref_type const & match
    (
        CharT * szbegin,
        basic_match_results<IterT, AllocT> & results
    ) const
    {
        // If your compile breaks here, it is because CharT* is not
        // convertible to type IterT. Check the declaration of your rpattern object.
        detail::static_assert< detail::is_convertible<CharT*,IterT>::value > const iterator_types_are_not_convertible;
        ( void ) iterator_types_are_not_convertible;

        if( detail::matcher_helper<IterT>::_do_match_c( *this, results, szbegin ) )
        {
            return results.backref(0);
        }
        else
        {
            return detail::static_init<backref_type>::value;
        }
    }

    template< typename CharT, typename TraitsT, typename AllocT >
    backref_type const & match
    (
        std::basic_string<CharT, TraitsT, AllocT> const & str,
        basic_match_results<IterT, AllocT> & results,
        size_type pos = 0,
        size_type len = static_cast<size_type>(-1)
    ) const
    {
        // If your compile breaks here, it is because iter_type is not
        // convertible to type IterT. Check the declaration of your rpattern object.
        typedef typename std::basic_string<CharT, TraitsT, AllocT>::const_iterator iter_type;
        detail::static_assert< detail::is_convertible<iter_type,IterT>::value > const iterator_types_are_not_convertible;
        ( void ) iterator_types_are_not_convertible;

        IterT ibegin = str.begin(), iend = str.begin();

        if( len == npos || pos + len >= str.size() )
            iend = IterT(str.end());
        else
            std::advance( iend, pos + len );

        std::advance( ibegin, pos );
        return match( ibegin, iend, results );
    }

    template< typename OtherT >
    size_t count( OtherT ibegin, OtherT iend ) const
    {
        // If your compile breaks here, it is because OtherT is not
        // convertible to type IterT. Check the declaration of your rpattern object.
        detail::static_assert< detail::is_convertible<OtherT,IterT>::value > const iterator_types_are_not_convertible;
        ( void ) iterator_types_are_not_convertible;

        return detail::matcher_helper<IterT>::_do_count( *this, ibegin, iend, false );
    }

    template< typename CharT >
    size_t count( CharT * szbegin ) const
    {
        // If your compile breaks here, it is because CharT* is not
        // convertible to type IterT. Check the declaration of your rpattern object.
        detail::static_assert< detail::is_convertible<CharT*,IterT>::value > const iterator_types_are_not_convertible;
        ( void ) iterator_types_are_not_convertible;

        return detail::matcher_helper<IterT>::_do_count( *this, szbegin, (CharT*)0, true );
    }

    template< typename CharT, typename TraitsT, typename AllocT >
    size_t count
    (
        std::basic_string<CharT, TraitsT, AllocT> const & str,
        size_type pos = 0,
        size_type len = static_cast<size_type>(-1)
    ) const
    {
        // If your compile breaks here, it is because iter_type is not
        // convertible to type IterT. Check the declaration of your rpattern object.
        typedef typename std::basic_string<CharT, TraitsT, AllocT>::const_iterator iter_type;
        detail::static_assert< detail::is_convertible<iter_type,IterT>::value > const iterator_types_are_not_convertible;
        ( void ) iterator_types_are_not_convertible;

        IterT ibegin = str.begin(), iend = str.begin();

        if( len == npos || pos + len >= str.size() )
            iend = IterT(str.end());
        else
            std::advance( iend, pos + len );

        std::advance( ibegin, pos );
        return count( ibegin, iend );
    }

    template< typename OtherT, typename CharT, typename TraitsT, typename AllocT >
    size_t split
    (
        OtherT ibegin,
        OtherT iend,
        basic_split_results<CharT, TraitsT, AllocT> & results,
        int limit = 0
    ) const
    {
        // If your compile breaks here, it is because OtherT is not
        // convertible to type IterT. Check the declaration of your rpattern object.
        detail::static_assert< detail::is_convertible<OtherT,IterT>::value > const iterator_types_are_not_convertible;
        ( void ) iterator_types_are_not_convertible;

        return detail::matcher_helper<IterT>::_do_split( *this, results, ibegin, iend, limit, false );
    }

    template< typename Char1T, typename Char2T, typename TraitsT, typename AllocT >
    size_t split
    (
        Char1T * szbegin,
        basic_split_results<Char2T, TraitsT, AllocT> & results,
        int limit = 0
    ) const
    {
        // If your compile breaks here, it is because Iter2 is not
        // convertible to type IterT. Check the declaration of your rpattern object.
        detail::static_assert< detail::is_convertible<Char1T*,IterT>::value > const iterator_types_are_not_convertible;
        ( void ) iterator_types_are_not_convertible;

        // If your compile breaks here, it's because the string you passed in doesn't have
        // the same character type as your split_results struct
        same_char_types( Char1T(), Char2T() );

        // If your compile breaks here, it is because CharT const * is not
        // convertible to type IterT. Check the declaration of your rpattern object.
        return detail::matcher_helper<IterT>::_do_split( *this, results, szbegin, (Char1T*)0, limit, true );
    }

    template< typename CharT, typename TraitsT, typename AllocT >
    size_t split
    (
        std::basic_string<CharT, TraitsT, AllocT> const & str,
        basic_split_results<CharT, TraitsT, AllocT> & results,
        int limit = 0,
        size_type pos = 0,
        size_type len = static_cast<size_type>(-1)
    ) const
    {
        // If your compile breaks here, it is because iter_type is not
        // convertible to type IterT. Check the declaration of your rpattern object.
        typedef typename std::basic_string<CharT, TraitsT, AllocT>::const_iterator iter_type;
        detail::static_assert< detail::is_convertible<iter_type,IterT>::value > const iterator_types_are_not_convertible;
        ( void ) iterator_types_are_not_convertible;

        IterT ibegin = str.begin(), iend = str.begin();

        if( len == npos || pos + len >= str.size() )
            iend = IterT(str.end());
        else
            std::advance( iend, pos + len );

        std::advance( ibegin, pos );
        return split( ibegin, iend, results, limit );
    }

    template< typename CharT, typename TraitsT, typename AllocT >
    size_t substitute
    (
        std::basic_string<CharT, TraitsT, AllocT> & str,
        basic_subst_results<CharT, TraitsT, AllocT> & results,
        size_type pos = 0,
        size_type len = static_cast<size_type>(-1)
    ) const
    {
        // If your compile breaks here, it is because iter_type is not
        // convertible to type IterT. Check the declaration of your rpattern object.
        typedef typename std::basic_string<CharT, TraitsT, AllocT>::const_iterator iter_type;
        detail::static_assert< detail::is_convertible<iter_type,IterT>::value > const iterator_types_are_not_convertible;
        ( void ) iterator_types_are_not_convertible;

        return detail::matcher_helper<IterT>::_do_subst( *this, str, results, pos, len );
    }
};

// --------------------------------------------------------------------------
//
// Class:       basic_rpattern_c
//
// Description: a pattern object optimized for matching C-style, NULL-
//              terminated strings. It treats the null-terminator as
//              the end-of-string condition.
//
// Methods:     basic_rpattern_c - c'tor
//              basic_rpattern_c -
//              basic_rpattern_c -
//              match            - match a null-terminated string
//              count            - count matches in a null-terminated string
//              _do_match_c      - internal implementation
//
// History:     8/13/2001 - ericne - Created
//
// --------------------------------------------------------------------------
template< typename CharT, typename SyntaxT = perl_syntax<CharT> >
class basic_rpattern_c : public basic_rpattern_base<CharT const *, SyntaxT>
{
    typedef detail::basic_rpattern_base_impl<CharT const *> impl;
public:
    typedef typename basic_rpattern_base<CharT const *, SyntaxT>::syntax_type     syntax_type;
    typedef typename basic_rpattern_base<CharT const *, SyntaxT>::char_type       char_type;
    typedef typename basic_rpattern_base<CharT const *, SyntaxT>::traits_type     traits_type;

    typedef typename basic_rpattern_base<CharT const *, SyntaxT>::string_type     string_type;
    typedef typename basic_rpattern_base<CharT const *, SyntaxT>::size_type       size_type;

    typedef typename basic_rpattern_base<CharT const *, SyntaxT>::backref_type    backref_type;
    typedef typename basic_rpattern_base<CharT const *, SyntaxT>::backref_vector  backref_vector;

    basic_rpattern_c() //throw()
        : basic_rpattern_base<CharT const *, SyntaxT>()
    {
    }

    basic_rpattern_c( basic_rpattern_c const & that )
        : basic_rpattern_base<CharT const *, SyntaxT>( that )
    {
    }

    explicit basic_rpattern_c
    (
        string_type const & pat,
        REGEX_FLAGS flags = NOFLAGS,
        REGEX_MODE mode = MODE_DEFAULT
    )                                                           //throw( bad_regexpr, std::bad_alloc )
        : basic_rpattern_base<CharT const *, SyntaxT>( pat, flags, mode )
    {
    }

    basic_rpattern_c & operator=( basic_rpattern_c<CharT, SyntaxT> const & that )
    {
        basic_rpattern_base<CharT const *, SyntaxT>::operator=( that );
        return *this;
    }

    template< typename AllocT >
    backref_type const & match
    (
        CharT const * szbegin,
        basic_match_results_c<CharT, AllocT> & results
    ) const
    {
        if( detail::matcher_helper<CharT const*>::_do_match_c( *this, results, szbegin ) )
        {
            return results.backref(0);
        }
        else
        {
            return detail::static_init<backref_type>::value;
        }
    }

    size_t count( CharT const * szbegin ) const
    {
        return detail::matcher_helper<CharT const*>::_do_count( *this, szbegin, (CharT const*)0, true );
    }
};



#if defined(UNICODE) | defined(_UNICODE)
typedef wchar_t rechar_t;
#else
typedef char    rechar_t;
#endif

typedef std::basic_string<rechar_t> restring;

// On many implementations of the STL, string::iterator is not a typedef
// for char*. Rather, it is a wrapper class. As a result, the regex code
// gets instantiated twice, once for bare pointers (rpattern_c) and once for
// the wrapped pointers (rpattern). But if there is a conversion from the
// bare ptr to the wrapped ptr, then we only need to instantiate the template
// for the wrapped ptr, and the code will work for the bare ptrs, too.
// This can be a significant space savings.  The REGEX_FOLD_INSTANTIONS
// macro controls this optimization. The default is "off" for backwards
// compatibility. To turn the optimization on, compile with:
// -DREGEX_FOLD_INSTANTIATIONS=1
#ifndef REGEX_FOLD_INSTANTIATIONS
#define REGEX_FOLD_INSTANTIATIONS 0
#endif

typedef ::regex::detail::select
<
    REGEX_FOLD_INSTANTIATIONS &&
        detail::is_convertible<rechar_t const *,restring::const_iterator>::value,
    restring::const_iterator,
    rechar_t const *
>::type lpctstr_t;

// For matching against null-terminated strings
typedef basic_rpattern<lpctstr_t, perl_syntax<rechar_t> >  perl_rpattern_c;
typedef basic_rpattern<lpctstr_t, posix_syntax<rechar_t> > posix_rpattern_c;

// For matching against std::strings
typedef basic_rpattern<restring::const_iterator, perl_syntax<rechar_t> >  perl_rpattern;
typedef basic_rpattern<restring::const_iterator, posix_syntax<rechar_t> > posix_rpattern;

// Default to perl syntax
typedef perl_rpattern    rpattern;
typedef perl_rpattern_c  rpattern_c;

// typedefs for the commonly used match_results and subst_results
typedef basic_match_results<restring::const_iterator> match_results;
typedef basic_match_results<lpctstr_t>                match_results_c;
typedef basic_subst_results<rechar_t>                 subst_results;
typedef basic_split_results<rechar_t>                 split_results;

#if defined(_MSC_VER) & 1200 < _MSC_VER
// These are no longer useful, and will go away in a future release
// You should be using the version without the _c
# pragma deprecated( basic_rpattern_c )
# pragma deprecated( basic_match_results_c )
#endif

#define STATIC_RPATTERN_EX( type, var, params ) \
    static type const var params;

#define STATIC_RPATTERN( var, params ) \
    STATIC_RPATTERN_EX( regex::rpattern, var, params )

#define STATIC_RPATTERN_C( var, params ) \
    STATIC_RPATTERN_EX( regex::rpattern_c, var, params )

#if defined(_MSC_VER) & 1200 < _MSC_VER
#pragma deprecated(STATIC_RPATTERN_EX)
#endif

//
// ostream inserter operator for back-references
//
template< typename CharT, typename TraitsT, typename IterT >
inline std::basic_ostream<CharT, TraitsT> & operator<<
(
    std::basic_ostream<CharT, TraitsT> & sout,
    backref_tag<IterT> const & br
)
{
    return br.print( sout );
}

} // namespace regex


//
// specializations for std::swap
//
namespace std
{
    template<>
    inline void swap( regex::detail::regex_arena & left, regex::detail::regex_arena & right )
    {
        left.swap( right );
    }

    template< typename IterT, typename SyntaxT >
    inline void swap( regex::basic_rpattern_base<IterT, SyntaxT> & left, regex::basic_rpattern_base<IterT, SyntaxT> & right )
    {
        left.swap( right );
    }
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif
