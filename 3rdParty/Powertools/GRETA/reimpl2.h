//+---------------------------------------------------------------------------
//
//  Copyright ( C ) Microsoft, 1994 - 2002.
//
//  File:       reimpl2.h
//
//  Functions:  helpers for matching and substituting regular expressions
//
//  Notes:      implementation details that really belong in a cpp file,
//              but can't because of template weirdness
//
//  Author:     Eric Niebler ( ericne@microsoft.com )
//
//  History:    8/15/2001   ericne   Created
//
//----------------------------------------------------------------------------

#ifndef REIMPL_H
#define REIMPL_H

//
// Helper functions for match and substitute
//

namespace detail
{

// For use while doing uppercase/lowercase conversions:
inline  char   regex_toupper(  char   ch ) { using namespace std; return (  char   )toupper( ch ); }
inline  char   regex_tolower(  char   ch ) { using namespace std; return (  char   )tolower( ch ); }
inline wchar_t regex_toupper( wchar_t ch ) { using namespace std; return ( wchar_t )towupper( ch ); }
inline wchar_t regex_tolower( wchar_t ch ) { using namespace std; return ( wchar_t )towlower( ch ); }

template< typename IBeginT, typename IEndT >
inline void regex_toupper( IBeginT ibegin, IEndT iend )
{
    typedef typename std::iterator_traits<IEndT>::value_type char_type;
    typedef std::char_traits<char_type> traits_type;

    for( ; iend != ibegin; ++ibegin )
        traits_type::assign( *ibegin, regex_toupper( *ibegin ) );
}

template< typename IBeginT, typename IEndT >
inline void regex_tolower( IBeginT ibegin, IEndT iend )
{
    typedef typename std::iterator_traits<IEndT>::value_type char_type;
    typedef std::char_traits<char_type> traits_type;

    for( ; iend != ibegin; ++ibegin )
        traits_type::assign( *ibegin, regex_tolower( *ibegin ) );
}

//
// Helper fn for swapping two auto_ptr's
//
template< typename T >
inline void swap_auto_ptr( std::auto_ptr<T> & left, std::auto_ptr<T> & right )
{
    std::auto_ptr<T> temp( left );
    left  = right;
    right = temp;
}

template< typename T >
inline void reset_auto_ptr( std::auto_ptr<T> & left )
{
    std::auto_ptr<T> temp( 0 );
    left = temp;
}

template< typename T, typename U >
inline void reset_auto_ptr( std::auto_ptr<T> & left, U * right )
{
    std::auto_ptr<T> temp( right );
    left = temp;
}

typedef int instantiator;

inline instantiator REGEX_CDECL instantiator_helper( ... )
{
    return instantiator();
}

// --------------------------------------------------------------------------
//
// Class:       match_param
//
// Description: Struct that contains the state of the matching operation.
//              Passed by reference to all recursive_match_all and recursive_match_this routines.
//
// Methods:     match_param - ctor
//
// Members:     ibufferbegin - start of the buffer
//              ibegin       - start of this iteration
//              iend        - end of the string
//              prgbackrefs  - pointer to backref array
//
// History:     8/14/2000 - ericne - Created
//
// --------------------------------------------------------------------------
template< typename IterT >
struct match_param
{
    typedef backref_tag<IterT>              backref_type;
    typedef sub_expr_base<IterT> const *    sub_expr_ptr;

    // for performance reasons, the most frequently used fields 
    // are placed at offsets which are a power of 2 (assuming
    // a 32-bit architecture, and iterators which are 32 bits).

    backref_type *      m_prgbackrefs;      // offsetof == 0
    IterT               m_iend;             // offsetof == 4
    IterT               m_icur;             // offsetof == 8
    size_t              m_cbackrefs;
    sub_expr_ptr        m_pnext;            // offsetof == 16
    IterT               m_ibufferbegin;
    IterT               m_imatchbegin;
    sub_expr_ptr        m_pfirst;
    unsafe_stack *      m_pstack;           // offsetof == 32
    bool                m_no0len;
    bool                m_reserved;

    match_param
    (
        IterT           ibufferbegin,
        IterT           imatchbegin,
        IterT           iend,
        backref_type *  prgbackrefs,
        size_t          cbackrefs
    )
        : m_prgbackrefs( prgbackrefs )
        , m_iend( iend )
        , m_icur( imatchbegin )
        , m_cbackrefs( cbackrefs )
        , m_pnext( 0 )
        , m_ibufferbegin( ibufferbegin )
        , m_imatchbegin( imatchbegin )
        , m_pfirst( 0 )
        , m_pstack( 0 )
        , m_no0len( false )
        , m_reserved( false )
    {
    }
};

// --------------------------------------------------------------------------
//
// Class:       arena_allocator
//
// Description: A small, fast allocator for speeding up pattern compilation.
//              Every basic_rpattern object has an arena as a member.
//              sub_expr objects can only be allocated from this arena.
//              Memory is alloc'ed in chunks using the underlying allocator.
//              Chunks are freed en-masse when clear() or finalize() is called.
//
// History:     8/17/2001 - ericne - Created
//
// Notes:       This is NOT a std-compliant allocator and CANNOT be used with
//              STL containers. arena_allocator objects maintain state, and
//              STL containers are allowed to assume their allocators do
//              not maintain state. In regexpr2.cpp, I define slist<>, a simple
//              arena-friendly singly-linked list for use with the arena
//              allocator.
//
// --------------------------------------------------------------------------
template< typename AllocT = std::allocator<char> >
struct pool_impl
{
    typedef typename rebind<AllocT, char>::type char_allocator_type;

    struct mem_block
    {
        size_t  m_offset;
        size_t  m_blocksize;
        mem_block * m_pnext;
        unsigned char m_data[ 1 ];
    };
#if !defined(_MSC_VER) | 1200 < _MSC_VER
    struct pool_data : char_allocator_type
    {
        pool_data( size_t default_size, char_allocator_type const & alloc )
            : char_allocator_type( alloc )
            , m_pfirst( 0 )
            , m_default_size( default_size )
        {
        }
        mem_block * m_pfirst;
        size_t      m_default_size;
        char_allocator_type & get_allocator()
        {
            return *this;
        }
    } m_data;
#else
    struct pool_data
    {
        pool_data( size_t default_size, char_allocator_type const & alloc )
            : m_alloc( alloc )
            , m_pfirst( 0 )
            , m_default_size( default_size )
        {
        }
        char_allocator_type m_alloc;
        mem_block * m_pfirst;
        size_t      m_default_size;
        char_allocator_type & get_allocator()
        {
            return m_alloc;
        }
    } m_data;
#endif
    void new_block( size_t size );
    void clear();
    void * allocate( size_t size );
    explicit pool_impl( size_t default_size, char_allocator_type const & alloc = char_allocator_type() );
    ~pool_impl();
    char_allocator_type get_allocator() const
    {
        return const_cast<pool_impl*>(this)->m_data.get_allocator();
    }
};

template< typename T, typename AllocT = std::allocator<char> >
class arena_allocator
{
public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T *pointer;
    typedef T const *const_pointer;
    typedef T & reference;
    typedef T const & const_reference;
    typedef T value_type;

    typedef typename rebind<AllocT, char>::type			char_alloc_type;
    typedef pool_impl<AllocT> pool_impl_t;
    typedef typename rebind<AllocT, pool_impl_t>::type	pool_alloc_type;

    explicit arena_allocator( size_t default_size, char_alloc_type const & alloc = char_alloc_type() )
        : m_pool( 0 )
    {
        char_alloc_type char_alloc( alloc );
        pool_alloc_type pool_alloc( convert_allocator<pool_impl_t>( char_alloc, 0 ) );
        m_pool = pool_alloc.allocate( 1, 0 );
        pool_alloc.construct( m_pool, pool_impl_t( default_size, char_alloc ) ); // can't throw
    }
#if !defined(_MSC_VER) | 1200 < _MSC_VER
    arena_allocator( arena_allocator const & that )
        : m_pool( that.m_pool )
    {
    }
#endif
    template< typename U >
    arena_allocator( arena_allocator<U> const & that )
        : m_pool( that.m_pool )
    {
    }
    ~arena_allocator()
    { // Many arena_allocators may point to m_pool, so don't delete it.
    } // Rather, wait for someone to call finalize().
    pointer allocate( size_type size, void const * =0 )
    {
        return static_cast<T*>( m_pool->allocate( size * sizeof(T) ) );
    }
    void deallocate( void *, size_type )
    { // no-op. deallocation happens when pool is finalized or cleared.
    }
    void construct( pointer p, T const & t )
    {
        new( static_cast<void*>(p) ) T( t );
    }
    void destroy( pointer p )
    {
        regex::detail::destroy( p );
    }
#if !defined(_MSC_VER) | 1200 < _MSC_VER
    template< typename U > struct rebind
    {
        typedef arena_allocator<U> other;
    };
#endif
    void clear()
    {
        m_pool->clear();
    }
    void finalize()
    {
        char_alloc_type char_alloc( m_pool->get_allocator() );
        pool_alloc_type pool_alloc( convert_allocator<pool_impl_t>( char_alloc, 0 ) );
        pool_alloc.destroy( m_pool );
        pool_alloc.deallocate( m_pool, 1 );
        m_pool = 0;
    }
    void swap( arena_allocator & that )
    {
        using std::swap;
        swap( m_pool, that.m_pool );
    }

    // the pool lives here
    pool_impl_t * m_pool;
};

// Dummy struct used by the pool allocator to align returned pointers
struct not_pod
{
    virtual ~not_pod() {}
};

template< typename AllocT >
inline pool_impl<AllocT>::pool_impl( size_t default_size, char_allocator_type const & alloc )
    : m_data( default_size, alloc )
{
}

template< typename AllocT >
inline pool_impl<AllocT>::~pool_impl()
{
    clear();
}

template< typename AllocT >
inline void pool_impl<AllocT>::clear()
{
    for( mem_block * pnext; m_data.m_pfirst; m_data.m_pfirst = pnext )
    {
        pnext = m_data.m_pfirst->m_pnext;
        m_data.get_allocator().deallocate( reinterpret_cast<char*>( m_data.m_pfirst ), m_data.m_pfirst->m_blocksize );
    }
}

template< typename AllocT >
inline void pool_impl<AllocT>::new_block( size_t size )
{
    size_t blocksize = regex_max( m_data.m_default_size, size ) + offsetof( mem_block, m_data );
    mem_block * pnew = reinterpret_cast<mem_block*>( m_data.get_allocator().allocate( blocksize, 0 ) );
    if( 0 == pnew )
    {
        throw std::bad_alloc();
    }
    pnew->m_offset      = 0;
    pnew->m_blocksize   = blocksize;
    pnew->m_pnext       = m_data.m_pfirst;
    m_data.m_pfirst     = pnew;
}

template< typename AllocT >
inline void * pool_impl<AllocT>::allocate( size_t size )
{
    if( 0 == size )
        size = 1;

    if( 0 == m_data.m_pfirst || m_data.m_pfirst->m_offset + size > m_data.m_default_size )
        new_block( size );

    void * pnew = m_data.m_pfirst->m_data + m_data.m_pfirst->m_offset;

    // ensure returned pointers are always suitably aligned
    m_data.m_pfirst->m_offset += ( ( size + alignof<not_pod>::value - 1 )
                                 & ~( alignof<not_pod>::value - 1 ) );

    return pnew;
}

// The regex_arena is a basic, vanilla arena_allocator.
typedef arena_allocator<char> regex_arena;

template< typename T >
type_with_size<3> allocator_picker( arena_allocator<T> const &, int );

template<> struct rebind_helper<3>
{
	template< typename, typename ElemT>
	struct inner
	{
		typedef arena_allocator<ElemT> type;
	};
};

// --------------------------------------------------------------------------
//
// Class:       sub_expr_base
//
// Description: patterns are "compiled" into a directed graph of sub_expr_base
//              structs.  Matching is accomplished by traversing this graph.
//
// Methods:     ~sub_expr_base - virt dtor so cleanup happens correctly
//              recursive_match_all      - match this sub-expression and all following
//                               sub-expression
//
// History:     8/14/2000 - ericne - Created
//
// --------------------------------------------------------------------------
template< typename IterT >
struct sub_expr_base
{
    virtual ~sub_expr_base() = 0;

    virtual bool recursive_match_all_( match_param<IterT> &, IterT ) const = 0; //throw()
    virtual bool recursive_match_all_c( match_param<IterT> &, IterT ) const = 0; //throw()
    virtual bool iterative_match_this_( match_param<IterT> & ) const = 0; //throw()
    virtual bool iterative_match_this_c( match_param<IterT> & ) const = 0; //throw()
    virtual bool iterative_rematch_this_( match_param<IterT> & ) const = 0; //throw()
    virtual bool iterative_rematch_this_c( match_param<IterT> & ) const = 0; //throw()

    // Use the regex_arena for memory management
    static void * operator new( size_t size, regex_arena & arena )
    {
        return arena.allocate( size );
    }
    static void operator delete( void *, regex_arena & )
    {
    }

    // Invoke the d'tor, but don't bother freeing memory. That will
    // happen automatically when the arena object gets destroyed.
    static void operator delete( void * )
    {
    }

    // For choosing an appropriate virtual function based on a compile time constant
    bool recursive_match_all_helper( match_param<IterT> & param, IterT icur, false_t ) const //throw()
    {
        return recursive_match_all_( param, icur );
    }
    bool recursive_match_all_helper( match_param<IterT> & param, IterT icur, true_t ) const //throw()
    {
        return recursive_match_all_c( param, icur );
    }
    bool iterative_match_this_helper( match_param<IterT> & param, false_t ) const //throw()
    {
        return iterative_match_this_( param );
    }
    bool iterative_match_this_helper( match_param<IterT> & param, true_t ) const //throw()
    {
        return iterative_match_this_c( param );
    }
    bool iterative_rematch_this_helper( match_param<IterT> & param, false_t ) const //throw()
    {
        return iterative_rematch_this_( param );
    }
    bool iterative_rematch_this_helper( match_param<IterT> & param, true_t ) const //throw()
    {
        return iterative_rematch_this_c( param );
    }
private:
    // don't allocate sub-expressions directly on the heap; they should
    // be allocated from an arena
    static void * operator new( size_t size );
    // disable all the vector new's and delete's.
    static void * operator new[]( size_t size, regex_arena & arena );
    static void operator delete[]( void *, regex_arena & );
    static void * operator new[]( size_t size );
    static void operator delete[]( void * );
};

template< typename IterT >
inline sub_expr_base<IterT>::~sub_expr_base()
{
}

// --------------------------------------------------------------------------
//
// Class:       subst_node
//
// Description: Substitution strings are parsed into an array of these
//              structures in order to speed up subst operations.
//
// Members:     stype         - type of this struct
//              .m_subst_string  - do a string substitution
//             .m_subst_backref - do a bacref substitution
//              op            - execute an operation
//
// History:     8/14/2000 - ericne - Created
//
// --------------------------------------------------------------------------
struct subst_node
{
    enum
    {
        PREMATCH  = -1,
        POSTMATCH = -2
    };

    enum subst_type
    {
        SUBST_STRING,
        SUBST_BACKREF,
        SUBST_OP
    };

    enum op_type
    {
        UPPER_ON   = SUBST_UPPER_ON,
        UPPER_NEXT = SUBST_UPPER_NEXT,
        LOWER_ON   = SUBST_LOWER_ON,
        LOWER_NEXT = SUBST_LOWER_NEXT,
        ALL_OFF    = SUBST_ALL_OFF
    };

    struct string_offsets
    {
        ptrdiff_t       m_rstart;
        ptrdiff_t       m_rlength;
    };

    subst_type          m_stype;

    union
    {
        string_offsets  m_subst_string;
        size_t          m_subst_backref;
        op_type         m_op;
    };
};

typedef std::list<subst_node> subst_list_type;
size_t DEFAULT_BLOCK_SIZE();

template< typename IterT >
class boyer_moore;

// --------------------------------------------------------------------------
//
// Class:       basic_rpattern_base_impl
//
// Description:
//
// Methods:     basic_rpattern_base_impl - ctor
//              flags                   - get the state of the flags
//              uses_backrefs           - true if the backrefs are referenced
//              get_first_subexpression - return ptr to first sub_expr struct
//              get_width               - get min/max nbr chars this pattern can match
//              loops                   - if false, we only need to try to match at 1st position
//              cgroups                 - number of visible groups
//              _cgroups_total          - total number of groups, including hidden ( ?: ) groups
//              get_pat                 - get string representing the pattern
//              get_subst               - get string representing the substitution string
//              get_subst_list          - get the list of subst nodes
//              _normalize_string       - perform character escaping
//
// Members:     m_fuses_backrefs        - true if subst string refers to backrefs
//              m_floop                 - false if pat only needs to be matched in one place
//              m_cgroups               - total count of groups
//              m_cgroups_visible       - count of visible groups
//              m_flags                 - the flags
//              m_nwidth                - width of this pattern
//              m_pat                   - pattern string
//              m_subst                 - substitution string
//              m_subst_list            - list of substitution nodes
//              m_pfirst                - ptr to first subexpression to match
//
// Typedefs:    char_type               -
//              string_type             -
//              size_type               -
//
// History:     8/14/2000 - ericne - Created
//
// --------------------------------------------------------------------------
template< typename IterT >
class basic_rpattern_base_impl
{
    basic_rpattern_base_impl( basic_rpattern_base_impl<IterT> const & );
    basic_rpattern_base_impl & operator=( basic_rpattern_base_impl<IterT> const & );
protected:
    typedef typename std::iterator_traits<IterT>::value_type char_type;
    typedef std::char_traits<char_type>                   traits_type;

    typedef std::basic_string<char_type>                  string_type;
    typedef size_t                                        size_type;

    typedef backref_tag<IterT>                             backref_type;
    typedef std::vector<backref_type>                     backref_vector;

    friend struct matcher_helper<IterT>;

    explicit basic_rpattern_base_impl
    (
        REGEX_FLAGS         flags = NOFLAGS,
        REGEX_MODE          mode  = MODE_DEFAULT,
        string_type const & pat   = string_type(),
        string_type const & subst = string_type()
    )                                               //throw()
        : m_arena( DEFAULT_BLOCK_SIZE() )
        , m_fuses_backrefs( false )
        , m_floop( true )
        , m_fok_to_recurse( true )
        , m_cgroups( 0 )
        , m_cgroups_visible( 0 )
        , m_flags( flags )
        , m_mode( mode )
        , m_nwidth( uninit_width() )
        , m_pat( new string_type( pat ) )
        , m_subst( new string_type( subst ) )
        , m_subst_list()
        , m_pfirst( 0 )
        , m_invisible_groups()
        , m_search( 0 )
    {
    }

    virtual ~basic_rpattern_base_impl()
    {
        // We're not going to be calling destructors because all allocated
        // memory associated with the parsed pattern resides in the arena.
        // The memory will be freed when the arena gets destroyed.
        //delete m_pfirst;
        reset_auto_ptr( m_pat );
        reset_auto_ptr( m_subst );
        m_arena.finalize();
    }

    regex_arena m_arena;           // The sub_expr arena

    bool        m_fuses_backrefs;  // true if the substitution uses backrefs
    bool        m_floop;           // false if m_pfirst->recursive_match_all only needs to be called once
    bool        m_fok_to_recurse;  // false if the pattern would recurse too deeply
    size_t      m_cgroups;         // number of groups ( always at least one )
    size_t      m_cgroups_visible; // number of visible groups
    REGEX_FLAGS m_flags;           // flags used to customize search/replace
    REGEX_MODE  m_mode;            // Used to pick the fast or safe algorithm
    width_type  m_nwidth;          // width of the pattern

    std::auto_ptr<string_type>   m_pat;   // contains the unparsed pattern
    std::auto_ptr<string_type>   m_subst; // contains the unparsed substitution

    subst_list_type              m_subst_list; // used to speed up substitution
    sub_expr_base<IterT> const * m_pfirst;     // first subexpression in pattern

    std::list<size_t>           m_invisible_groups; // groups w/o backrefs

    boyer_moore<typename string_type::const_iterator>  * m_search;

    size_t _cgroups_total() const //throw()
    {
        return m_cgroups;
    }

    bool _loops() const //throw()
    {
        return m_floop;
    }

    size_t _get_next_group_nbr()
    {
        return m_cgroups++;
    }

    void _normalize_string( string_type & str ) const //throw()
    {
        if( NORMALIZE & flags() )
            process_escapes( str, true );
    }

    bool _save_backrefs() const //throw()
    {
        return m_fuses_backrefs || ! ( flags() & NOBACKREFS );
    }

    sub_expr_base<IterT> const * _get_first_subexpression() const //throw()
    {
        return m_pfirst;
    }

    REGEX_FLAGS flags() const //throw()
    {
        return m_flags;
    }

    REGEX_MODE mode() const // throw()
    {
        return m_mode;
    }

    width_type get_width() const //throw()
    {
        return m_nwidth;
    }

    size_t cgroups() const //throw()
    {
        return m_cgroups_visible;
    }

    string_type const & get_pat() const //throw()
    {
        return *m_pat;
    }

    string_type const & get_subst() const //throw()
    {
        return *m_subst;
    }

    bool _ok_to_recurse() const; //throw();

    void swap( basic_rpattern_base_impl<IterT> & that ); // throw();

    enum { npos = static_cast<size_type>( -1 ) };

    static instantiator instantiate()
    {
        typedef basic_rpattern_base_impl this_type;

        return instantiator_helper
        (
            &this_type::_ok_to_recurse,
            &this_type::swap
        );
    }
};

template< typename IterT >
struct matcher_helper
{
    typedef basic_rpattern_base_impl< IterT >    rpattern_type;
    typedef typename rpattern_type::size_type    size_type;
    typedef typename rpattern_type::char_type    char_type;
    typedef typename rpattern_type::traits_type  traits_type;
    typedef typename rpattern_type::backref_type backref_type;

    static bool _do_match_iterative_helper
    (
        sub_expr_base<IterT> const * expr,
        match_param<IterT> & param,
        IterT icur
    );

    static bool _do_match_iterative_helper_c
    (
        sub_expr_base<IterT> const * expr,
        match_param<IterT> & param,
        IterT icur
    );

    static bool _do_match_recursive
    (
        sub_expr_base<IterT> const * expr,
        match_param<IterT> & param,
        IterT icur
    );

    static bool _do_match_recursive_c
    (
        sub_expr_base<IterT> const * expr,
        match_param<IterT> & param,
        IterT icur
    );

    static bool _do_match_impl
    (
        rpattern_type const & pat,
        match_param<IterT> & param,
        bool const use_null
    );

    static bool _do_match_with_stack
    (
        rpattern_type const & pat,
        match_param<IterT> & param,
        bool const use_null
    );

    template< typename Alloc1T, typename Alloc2T >
    static void _fixup_backrefs
    (
        std::vector<backref_type,Alloc1T> & rgbackrefs,
        std::list<size_t,Alloc2T> const & invisible
    )
    {
        typedef typename std::list<size_t,Alloc2T>::const_iterator iter_type;

        // Remove information about the "invisible" groups
        if( rgbackrefs[0].matched )
        {
            size_t dropped = 0;
            iter_type const end = invisible.end();
            iter_type curr = invisible.begin(), next = invisible.begin();

            for( ; end != curr; curr = next, ++dropped )
            {
                if( end == ++next )
                {
                    std::copy(
                        rgbackrefs.begin() + *curr + 1,
                        rgbackrefs.end(),
                        rgbackrefs.begin() + *curr - dropped );
                }
                else
                {
                    std::copy(
                        rgbackrefs.begin() + *curr + 1,
                        rgbackrefs.begin() + *next,
                        rgbackrefs.begin() + *curr - dropped );
                }
            }

            rgbackrefs.resize( rgbackrefs.size() - dropped );
        }
        else
        {
            rgbackrefs.resize( rgbackrefs.size() - invisible.size() );
        }
    }

    template< typename AllocT >
    static bool _do_try_match
    (
        rpattern_type const & pat,
        match_param<IterT> & param,
        std::vector<backref_type,AllocT> & rgbackrefs,
        bool const use_null
    )
    {
        bool success;
        rgbackrefs.resize( pat._cgroups_total() );
        param.m_prgbackrefs = & rgbackrefs[0];
        param.m_cbackrefs = rgbackrefs.size();

        REGEX_SEH_TRY
        {
            if( pat._ok_to_recurse() )
            {
                success = _do_match_impl( pat, param, use_null );
            }
            else
            {
                success = _do_match_with_stack( pat, param, use_null );
            }
        }
        REGEX_SEH_EXCEPT( REGEX_SEH_STACK_OVERFLOW == _exception_code() )
        {
            // we have overflowed the stack. reset the guard page.
            REGEX_RESET_STK_OFLW();

            // This match fails silently.
            for( size_t i=0; i < param.m_cbackrefs; ++i )
            {
                param.m_prgbackrefs[i] = static_init<backref_type>::value;
            }

            success = false;
        }

        _fixup_backrefs( rgbackrefs, pat.m_invisible_groups );
        return success;
    }

    template< typename AllocT >
    static bool _do_match
    (
        rpattern_type const & pat,
        basic_match_results<IterT,AllocT> & results,
        IterT ibegin,
        IterT iend,
        bool use_null
    )
    {
        typedef typename basic_match_results<IterT,AllocT>::backref_vector backref_vector;

        results.m_ibegin = ibegin;
        match_param<IterT> param( ibegin, ibegin, iend, 0, 0 );

        if( GLOBAL & pat.flags() ) // do a global find
        {
            // The NOBACKREFS flag is ignored in the match method.
            bool const fAll   = ( ALLBACKREFS   == ( ALLBACKREFS   & pat.flags() ) );
            bool const fFirst = ( FIRSTBACKREFS == ( FIRSTBACKREFS & pat.flags() ) );

            backref_vector rgtempbackrefs( results.m_rgbackrefs.get_allocator() );

            while( _do_try_match( pat, param, results.m_rgbackrefs, use_null ) )
            {
                backref_type const & br = param.m_prgbackrefs[0];

                // Handle specially the backref flags
                if( fFirst )
                {
                    rgtempbackrefs.push_back( br );
                }
                else if( fAll )
                {
                    rgtempbackrefs.insert(
                        rgtempbackrefs.end(),
                        results.m_rgbackrefs.begin(),
                        results.m_rgbackrefs.end() );
                }
                else
                {
                    rgtempbackrefs.swap( results.m_rgbackrefs );
                }

                param.m_imatchbegin = br.second;
                param.m_no0len = ( br.first == br.second );
            }

            // restore the backref vectors
            results.m_rgbackrefs.swap( rgtempbackrefs );
            return ! results.m_rgbackrefs.empty();
        }
        else
        {
            return _do_try_match( pat, param, results.m_rgbackrefs, use_null );
        }
    }

    template< typename AllocT >
    static bool _do_match_c
    (
        rpattern_type const & pat,
        basic_match_results<IterT,AllocT> & results,
        char_type const * szbegin
    )
    {
        if( RIGHTMOST & pat.flags() )
        {
            // We need to know the end of the string if we're doing a
            // RIGHTMOST match.
            char_type const * szend = szbegin;
            std::advance( szend, traits_type::length( szbegin ) );
            return _do_match( pat, results, szbegin, szend, false );
        }
        else
        {
            return _do_match( pat, results, szbegin, 0, true );
        }
    }

    static size_t _do_count
    (
        rpattern_type const & pat,
        IterT ibegin,
        IterT iend,
        bool use_null
    )
    {
        size_t cmatches = 0;
        std::vector<backref_type> rgbackrefs;

        // If your compile breaks here, it is because CharT const * is not
        // convertible to type IterT. Check the declaration of your rpattern object.
        match_param<IterT> param( ibegin, ibegin, iend, 0, 0 );

        while( _do_try_match( pat, param, rgbackrefs, use_null ) )
        {
            backref_type const & br = param.m_prgbackrefs[0];
            ++cmatches;
            param.m_imatchbegin = br.second;

            param.m_no0len = ( br.first == br.second );
        }

        return cmatches;
    }

    template< typename CharT, typename TraitsT, typename AllocT >
    static size_t _do_split
    (
        rpattern_type const & pat,
        basic_split_results<CharT, TraitsT, AllocT> & results,
        IterT ibegin,
        IterT iend,
        int limit,
        bool use_null
    )
    {
        typedef typename basic_split_results<CharT, TraitsT, AllocT>::string_type  string_type;
        typedef typename rebind<AllocT, backref_type>::type                      backref_allocator;

        std::vector<backref_type,backref_allocator> rgbackrefs( 
            convert_allocator<backref_type>( results.strings().get_allocator(), 0 ) );
            
        typedef typename rebind<AllocT, CharT>::type                              char_allocator_type;
        char_allocator_type char_allocator = 
            convert_allocator<CharT>( results.strings().get_allocator(), 0 );

        // reserve some initial space
        results.strings().clear();
        results.strings().reserve( 10 );

        match_param<IterT> param( ibegin, ibegin, iend, 0, 0 );

        while( 1 != limit && _do_try_match( pat, param, rgbackrefs, use_null ) )
        {
            backref_type const & br = param.m_prgbackrefs[0];
            param.m_no0len = ( br.first == br.second );

            // discard zero-width matches at the beginning and end of the buffer
            if( param.m_no0len )
            {
                // if we're at the beginning, skip
                if( br.first == param.m_ibufferbegin )
                    continue;

                // if we're at the end, break
                if( use_null ? 0 == *param.m_imatchbegin : param.m_imatchbegin == param.m_iend )
                    break;
            }

            string_type tmp( param.m_imatchbegin, br.first, char_allocator );
            results.strings().push_back( tmp );
            param.m_imatchbegin = br.second;

            // add any groups
            for( size_t i = 1; i < rgbackrefs.size(); ++i )
            {
                backref_type const & br = rgbackrefs[i];
                string_type tmp( br.first, br.second, char_allocator );
                results.strings().push_back( tmp );
            }

            if( limit > 0 )
                --limit;
        }

        // append the last string, unless it's empty and limit is 0
        if( use_null )
        {
            if( *param.m_imatchbegin || 0 != limit )
                results.strings().push_back( string_type( &*param.m_imatchbegin, char_allocator ) );
        }
        else
        {
            if( param.m_imatchbegin != param.m_iend || 0 != limit )
                results.strings().push_back( string_type( param.m_imatchbegin, param.m_iend, char_allocator ) );
        }

        // remove trailing empty fields
        if( 0 == limit )
        {
            while( results.size() && results.back().empty() )
            {
                results.strings().pop_back();
            }
        }

        return results.size();
    }

    template< typename CharT, typename TraitsT, typename AllocT >
    static size_t _do_subst_internal
    (
        std::basic_string<CharT, TraitsT, AllocT> & str,
        basic_subst_results<CharT, TraitsT, AllocT> const & results,
        rpattern_type const & pat,
        size_type strpos,
        size_type strlen
    )
    {
        typedef subst_list_type::const_iterator iter_type;
        enum { UPPER = -1, NIL, LOWER } next = NIL, rest = NIL;
        bool first = true;
        size_t old_strpos = strpos;
        typename std::basic_string<CharT, TraitsT, AllocT>::iterator itstrlen = str.begin();
        std::advance( itstrlen, strpos + strlen );
        std::basic_string<char_type> const & subst = pat.get_subst();

        for( iter_type isubst = pat.m_subst_list.begin(); pat.m_subst_list.end() != isubst; ++isubst )
        {
            size_t sublen = 0;
            typename std::basic_string<CharT, TraitsT, AllocT>::const_iterator itsubpos1; // iter into str
            typename std::basic_string<CharT, TraitsT, AllocT>::const_iterator itsublen1;
            typename std::basic_string<char_type>::const_iterator              itsubpos2; // iter into subst string
            typename std::basic_string<char_type>::const_iterator              itsublen2;
            typename std::basic_string<CharT, TraitsT, AllocT>::iterator       itstrpos = str.begin();
            std::advance( itstrpos, strpos );

            switch( isubst->m_stype )
            {
            case subst_node::SUBST_STRING:
                itsubpos2 = subst.begin();
                std::advance( itsubpos2, isubst->m_subst_string.m_rstart );
                itsublen2 = itsubpos2;
                std::advance( itsublen2, isubst->m_subst_string.m_rlength );

                if( first )
                    str.replace( itstrpos, itstrlen, itsubpos2, itsublen2 );
                else
                    str.insert( itstrpos, itsubpos2, itsublen2 );
                sublen = std::distance( itsubpos2, itsublen2 );
                break;

            case subst_node::SUBST_BACKREF:
                switch( isubst->m_subst_backref )
                {
                case subst_node::PREMATCH:
                    itsubpos1 = results.backref_str().begin();
                    itsublen1 = itsubpos1;
                    std::advance( itsublen1, sublen = results.rstart() );
                    break;
                case subst_node::POSTMATCH:
                    itsubpos1 = results.backref_str().begin();
                    std::advance( itsubpos1, results.rstart() + results.rlength() );
                    itsublen1 = results.backref_str().end();
                    break;
                default:
                    itsubpos1 = results.backref_str().begin();
                    std::advance( itsubpos1, results.rstart( isubst->m_subst_backref ) );
                    itsublen1 = itsubpos1;
                    std::advance( itsublen1, results.rlength( isubst->m_subst_backref ) );
                    break;
                }

                if( first )
                    str.replace( itstrpos, itstrlen, itsubpos1, itsublen1 );
                else
                    str.insert( itstrpos, itsubpos1, itsublen1 );
                sublen = std::distance( itsubpos1, itsublen1 );
                break;

            case subst_node::SUBST_OP:
                switch( isubst->m_op )
                {
                case subst_node::UPPER_ON:
                    rest = UPPER;
                    break;
                case subst_node::UPPER_NEXT:
                    next = UPPER;
                    break;
                case subst_node::LOWER_ON:
                    rest = LOWER;
                    break;
                case subst_node::LOWER_NEXT:
                    next = LOWER;
                    break;
                case subst_node::ALL_OFF:
                    rest = NIL;
                    break;
                default:
                    REGEX_ASSERT(false);
                    break;
                }
                continue; // jump to the next item in the list

            default:
                REGEX_ASSERT(false);
                break;
            }

            first = false;

            // Are we upper- or lower-casing this string?
            if( rest )
            {
                typename std::basic_string<CharT, TraitsT, AllocT>::iterator ibegin = str.begin();
                std::advance( ibegin, strpos );
                typename std::basic_string<CharT, TraitsT, AllocT>::const_iterator iend = ibegin;
                std::advance( iend, sublen );
                switch( rest )
                {
                case UPPER:
                    regex_toupper( ibegin, iend );
                    break;
                case LOWER:
                    regex_tolower( ibegin, iend );
                    break;
                default:
                    REGEX_ASSERT(false);
                    break;
                }
            }

            // Are we upper- or lower-casing the next character?
            if( next )
            {
                switch( next )
                {
                case UPPER:
                    str[strpos] = regex_toupper( str[strpos] );
                    break;
                case LOWER:
                    str[strpos] = regex_tolower( str[strpos] );
                    break;
                default:
                    REGEX_ASSERT(false);
                    break;
                }
                next = NIL;
            }

            strpos += sublen;
        }

        // If *first* is still true, then we never called str.replace, and the substitution
        // string is empty. Erase the part of the string that the pattern matched.
        if( first )
            str.erase( strpos, strlen );

        // return length of the substitution
        return strpos - old_strpos;
    }

    template< typename CharT, typename TraitsT, typename AllocT >
    static size_t _do_subst
    (
        rpattern_type const & pat,
        std::basic_string<CharT, TraitsT, AllocT> & str,
        basic_subst_results<CharT, TraitsT, AllocT> & results,
        size_type pos,
        size_type len
    )
    {
        typedef std::basic_string<CharT, TraitsT, AllocT> string_type;
        typedef typename basic_subst_results<CharT, TraitsT, AllocT>::backref_vector backref_vector;

        results.m_pbackref_str = pat._save_backrefs() ? &( results.m_backref_str = str ) : &str;
        results.m_ibegin = results.m_pbackref_str->begin();

        size_t csubst = 0;
        size_type stop_offset = results.m_pbackref_str->size();
        if( len != rpattern_type::npos )
            stop_offset = regex_min( size_t( pos + len ), stop_offset );

        match_param<IterT> param( results.m_ibegin, results.m_ibegin, results.m_ibegin, 0, 0 );

        std::advance( param.m_imatchbegin, pos );
        std::advance( param.m_iend, stop_offset );
        param.m_ibufferbegin = param.m_imatchbegin;

        if( GLOBAL & pat.flags() )
        {
            bool const fAll   = ( ALLBACKREFS   == ( ALLBACKREFS   & pat.flags() ) );
            bool const fFirst = ( FIRSTBACKREFS == ( FIRSTBACKREFS & pat.flags() ) );
            backref_vector rgtempbackrefs( results.m_rgbackrefs.get_allocator() ); // temporary vector used if fsave_backrefs

            size_type pos_offset = 0; // keep track of how much the backref_str and
                                      // the current string are out of sync

            while( _do_try_match( pat, param, results.m_rgbackrefs, false ) )
            {
                backref_type const & br = param.m_prgbackrefs[0];
                ++csubst;
                size_type match_length = std::distance( br.first, br.second );
                pos = std::distance( results.m_ibegin, br.first );
                size_type subst_length = _do_subst_internal( str, results, pat, pos + pos_offset, match_length );

                if( pat._save_backrefs() )
                {
                    pos += match_length;
                    pos_offset += ( subst_length - match_length );

                    // Handle specially the backref flags
                    if( fFirst )
                    {
                        rgtempbackrefs.push_back( br );
                    }
                    else if( fAll )
                    {
                        rgtempbackrefs.insert(
                            rgtempbackrefs.end(),
                            results.m_rgbackrefs.begin(),
                            results.m_rgbackrefs.end() );
                    }
                    else
                    {
                        rgtempbackrefs.swap( results.m_rgbackrefs );
                    }
                }
                else
                {
                    pos += subst_length;
                    stop_offset += ( subst_length - match_length );
                    results.m_ibegin = results.m_pbackref_str->begin();

                    // we're not saving backref information, so we don't
                    // need to do any special backref maintenance here
                }

                // prevent a pattern that matches 0 characters from matching
                // again at the same point in the string
                param.m_no0len = ( 0 == match_length );

                param.m_imatchbegin = results.m_ibegin;
                std::advance( param.m_imatchbegin, pos ); // ineffecient for bidirectional iterators.

                param.m_iend = results.m_ibegin;
                std::advance( param.m_iend, stop_offset ); // ineffecient for bidirectional iterators.
            }

            // If we did special backref handling, swap the backref vectors
            if( pat._save_backrefs() )
            {
                results.m_rgbackrefs.swap( rgtempbackrefs );
            }
            else if( ! results.m_rgbackrefs[0].matched )
            {
                results.m_rgbackrefs.clear();
            }
        }
        else if( _do_try_match( pat, param, results.m_rgbackrefs, false ) )
        {
            backref_type const & br = param.m_prgbackrefs[0];
            ++csubst;
            _do_subst_internal(
                str, results, pat,
                std::distance( results.m_ibegin, br.first ),
                std::distance( br.first, br.second ) );
            results.m_ibegin = results.m_pbackref_str->begin();
        }

        if( NOBACKREFS == ( pat.flags() & NOBACKREFS ) )
        {
            results.m_rgbackrefs.clear();
        }

        return csubst;
    }

    static instantiator instantiate()
    {
        return instantiator_helper
        (
            &matcher_helper::_do_match_iterative_helper,
            &matcher_helper::_do_match_iterative_helper_c,
            &matcher_helper::_do_match_recursive,
            &matcher_helper::_do_match_recursive_c,
            &matcher_helper::_do_match_with_stack,
            &matcher_helper::_do_match_impl
        );
    }
};


//
// Some helper functions needed by process_escapes
//
template< typename CharT >
inline bool regex_isxdigit( CharT ch )
{
    return ( REGEX_CHAR(CharT,'0') <= ch && REGEX_CHAR(CharT,'9') >= ch )
        || ( REGEX_CHAR(CharT,'a') <= ch && REGEX_CHAR(CharT,'f') >= ch )
        || ( REGEX_CHAR(CharT,'A') <= ch && REGEX_CHAR(CharT,'F') >= ch );
}

template< typename CharT >
inline int regex_xdigit2int( CharT ch )
{
    if( REGEX_CHAR(CharT,'a') <= ch && REGEX_CHAR(CharT,'f') >= ch )
        return ch - REGEX_CHAR(CharT,'a') + 10;
    if( REGEX_CHAR(CharT,'A') <= ch && REGEX_CHAR(CharT,'F') >= ch )
        return ch - REGEX_CHAR(CharT,'A') + 10;
    return ch - REGEX_CHAR(CharT,'0');
}

} // namespace detail

// --------------------------------------------------------------------------
//
// Function:    process_escapes
//
// Description: Turn the escape sequnces \f \n \r \t \v \\ into their
//              ASCII character equivalents. Also, optionally process
//              perl escape sequences.
//
// Returns:     void
//
// Arguments:   str      - the string to process
//              fPattern - true if the string is to be processed as a regex
//
// Notes:       When fPattern is true, the perl escape sequences are not
//              processed.  If there is an octal or hex excape sequence, we
//              don't want to turn it into a regex metacharacter here.  We
//              leave it unescaped so the regex parser correctly interprests
//              it as a character literal.
//
// History:     8/1/2001 - ericne - Created
//
// --------------------------------------------------------------------------
template< typename CharT, typename TraitsT, typename AllocT >
inline void process_escapes( std::basic_string<CharT, TraitsT, AllocT> & str, bool fPattern ) //throw()
{
    typedef typename std::basic_string<CharT, TraitsT, AllocT>::size_type size_type;

    size_type i = 0;
    size_type const npos = std::basic_string<CharT, TraitsT, AllocT>::npos;

    if( str.empty() )
        return;

    while( npos != ( i = str.find( REGEX_CHAR(CharT,'\\'), i ) ) )
    {
        if( str.size() - 1 == i )
            return;

        switch( str[i+1] )
        {
        case REGEX_CHAR(CharT,'a'):
            str.replace( i, 2, 1, REGEX_CHAR(CharT,'\a') );
            break;
        case REGEX_CHAR(CharT,'b'):
            if( ! fPattern )
                str.replace( i, 2, 1, REGEX_CHAR(CharT,'\b') );
            else
                ++i;
            break;
        case REGEX_CHAR(CharT,'e'):
            str.replace( i, 2, 1, CharT( 27 ) );
            break;
        case REGEX_CHAR(CharT,'f'):
            str.replace( i, 2, 1, REGEX_CHAR(CharT,'\f') );
            break;
        case REGEX_CHAR(CharT,'n'):
            str.replace( i, 2, 1, REGEX_CHAR(CharT,'\n') );
            break;
        case REGEX_CHAR(CharT,'r'):
            str.replace( i, 2, 1, REGEX_CHAR(CharT,'\r') );
            break;
        case REGEX_CHAR(CharT,'t'):
            str.replace( i, 2, 1, REGEX_CHAR(CharT,'\t') );
            break;
        case REGEX_CHAR(CharT,'v'):
            str.replace( i, 2, 1, REGEX_CHAR(CharT,'\v') );
            break;
        case REGEX_CHAR(CharT,'\\'):
            if( fPattern )
            {
                if( i+3 < str.size() && REGEX_CHAR(CharT,'\\') == str[i+2] && REGEX_CHAR(CharT,'\\') == str[i+3] )
                    str.erase( i, 2 );
                ++i;
            }
            else
                str.erase( i, 1 );
            break;
        case REGEX_CHAR(CharT,'0'): case REGEX_CHAR(CharT,'1'): case REGEX_CHAR(CharT,'2'): case REGEX_CHAR(CharT,'3'):
        case REGEX_CHAR(CharT,'4'): case REGEX_CHAR(CharT,'5'): case REGEX_CHAR(CharT,'6'): case REGEX_CHAR(CharT,'7'):
            if( ! fPattern )
            {
                size_t j=i+2;
                CharT ch = CharT( str[i+1] - REGEX_CHAR(CharT,'0') );
                for( ; j-i < 4 && j < str.size() && REGEX_CHAR(CharT,'0') <= str[j] && REGEX_CHAR(CharT,'7') >= str[j]; ++j )
                    ch = CharT( ch * 8 + ( str[j] - REGEX_CHAR(CharT,'0') ) );
                str.replace( i, j-i, 1, ch );
            }
            break;
        case REGEX_CHAR(CharT,'x'):
            if( ! fPattern )
            {
                CharT ch = 0;
                size_t j=i+2;
                for( ; j-i < 4 && j < str.size() && detail::regex_isxdigit( str[j] ); ++j )
                    ch = CharT( ch * 16 + detail::regex_xdigit2int( str[j] ) );
                str.replace( i, j-i, 1, ch );
            }
            break;
        case REGEX_CHAR(CharT,'c'):
            if( ! fPattern && i+2 < str.size() )
            {
                CharT ch = str[i+2];
                if( REGEX_CHAR(CharT,'a') <= ch && REGEX_CHAR(CharT,'z') >= ch )
                    ch = detail::regex_toupper( ch );
                str.replace( i, 3, 1, CharT( ch ^ 0x40 ) );
            }
            break;
        default:
            if( fPattern )
                ++i;
            else
                str.erase( i, 1 );
            break;
        }
        ++i;
        if( str.size() <= i )
            return;
    }
}

#endif
