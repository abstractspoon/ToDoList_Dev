//+---------------------------------------------------------------------------
//
//  Copyright ( C ) Microsoft, 1994 - 2002.
//
//  File:       restack.h
//
//  Functions:  a quick-'n'-dirty, type-unsafe stack used by the iterative
//              regular expression algorithm
//
//  Notes:      Care must be taken when using this stack. You must pop off
//              the correct type of object, otherwise you get garbage. Also,
//              if you push anything that has a non-trivial destructor, then
//              be sure to explicitely pop everything off the stack and don't
//              use the unsafe_long_jump method.
//
//  Author:     Eric Niebler ( ericne@microsoft.com )
//
//  History:    11/15/2001   ericne   Created
//
//----------------------------------------------------------------------------

#ifndef HETERO_STACK_H
#define HETERO_STACK_H

#include <string>
#include <utility>
#include <typeinfo>
#include <stdexcept>
#include <functional>

#ifndef REGEX_CDECL
#ifdef _MSC_VER
#define REGEX_CDECL __cdecl
#else
#define REGEX_CDECL
#endif
#endif

#define COMMA ,
#if !defined(_MSC_VER) | 1200 < _MSC_VER
# define REGEX_VC6(x)
# define REGEX_NVC6(x) x
#else
# define REGEX_VC6(x) x
# define REGEX_NVC6(x)
#endif

namespace regex
{

namespace detail
{

// For compile-time assertions that generate
// no run-time overhead.
template< bool f > struct static_assert;
template<>         struct static_assert<true> { static_assert() {} };

// Work-around for a template-template parameter problem on VC7.0
template< typename T > struct type2type { typedef T type; };

template< bool F > struct bool2type { enum { value = F }; };

typedef bool2type<true>  true_t;
typedef bool2type<false> false_t;

#ifdef _MSC_VER
// warning C4127: conditional expression is constant
// warning C4189: local variable is initialized but not referenced
// warning C4244: conversion from 'T' to 'int', possible loss of data
// warning C4510: default constructor could not be generated
// warning C4610: struct can never be instantiated - user defined constructor required
// warning C4800: forcing value to bool 'true' or 'false' (performance warning)
#pragma warning( push )
#pragma warning( disable : 4127 4189 4244 4510 4610 4800 )

// Make sure nobody has tampered with the packing before defining the
// alignof structure
#pragma pack( push )
#pragma pack() // use the default packing
#endif

template< typename T >
class alignof
{
    struct helper
    {
        helper();
        char    m_c;
        T       m_t;
    };
public:
    enum { value = sizeof(helper)-sizeof(T) < sizeof(T) ? sizeof(helper)-sizeof(T) : sizeof(T) };
};

#ifdef _MSC_VER
#pragma pack( pop )
#endif

//
// Type traits
//

typedef char (&yes_type)[1];
typedef char (&no_type)[2];

template< bool >
struct select_helper
{
    template< typename T, typename U >
    struct nested
    {
        typedef T type;
    };
};

template<>
struct select_helper<false>
{
    template< typename T, typename U >
    struct nested
    {
        typedef U type;
    };
};

// For use in conditional typedefs
template< bool F, typename T, typename U >
struct select
{
    typedef typename select_helper<F>::template nested<T,U>::type type;
};

template< typename U >
struct convertible_helper
{
    static yes_type             check( U );
    static no_type  REGEX_CDECL check(...);
};

template< typename T >
struct factory
{
    static T& make();
};

template< typename T, typename U >
struct is_convertible
{
    enum { value = (sizeof(convertible_helper<U>::check(factory<T>::make()))==sizeof(yes_type)) };
};

template< size_t N >
struct is_power_of_two
{
    enum { value = 1==N || 0==(N%2) && is_power_of_two<N/2>::value };
};

template<>
struct is_power_of_two<0>
{
    enum { value = false };
};

// Very primative implementation of is_scalar. This doesn't work
// for void, reference types, array types or function types, but
// we don't use those types from hetero_stack.
struct bool_convertible { bool_convertible(bool); };

template< typename T >
struct is_scalar
{
    enum { value = is_convertible<T,bool_convertible>::value };
};

template< typename T >
struct has_trivial_copy
{
    enum { value = is_scalar<T>::value };
};

template< typename T >
struct has_trivial_assignment
{
    enum { value = is_scalar<T>::value };
};

template< typename T >
struct has_trivial_destructor
{
    enum { value = is_scalar<T>::value };
};

template< bool > struct destroyer_helper
{
    template< typename T >
    static void destroy( T const * pT )
    {
        pT, pT->~T();
    }
};
template<> struct destroyer_helper<true>
{
    template< typename T >
    static void destroy( T const * )
    {
    }
};
template< typename T >
void destroy( T const * pT )
{
    destroyer_helper<has_trivial_destructor<T>::value>::destroy( pT );
}

struct type_vtable
{
    std::type_info const *  typeinfo_ptr;
    size_t             size;
    size_t             aligned_size;
    void (*destroy)( void * );
    void (*copy)( void *, void const * );
};

template< typename T, size_t AlignmentT >
class type_info_ex
{
    static void destroy( void * pv )
    {
        T const * pT = static_cast<T const*>( pv );
        regex::detail::destroy( pT );
        (void)pv;
        (void)pT;
    }
    static void copy( void * dst, void const * src )
    {
        new ( dst ) T( *static_cast<T const *>( src ) );
    }
public:
    static type_vtable const vtable;

    static bool equals( type_vtable const * ptm )
    {
        return ptm == & vtable || *ptm->typeinfo_ptr == typeid(T);
    }
};

template< typename T,size_t AlignmentT >
type_vtable const type_info_ex<T,AlignmentT>::vtable =
{
    &typeid(T),
    sizeof(T),
    ( sizeof(T) + AlignmentT - 1 ) & ~( AlignmentT - 1 ),
    has_trivial_destructor<T>::value ? 0 : &type_info_ex<T,AlignmentT>::destroy,
    &type_info_ex<T,AlignmentT>::copy
};

template< typename T >
inline T & to_type( void * pv )
{
    return *static_cast<T*>( pv );
}

} // namespace detail

// --------------------------------------------------------------------------
//
// Class:       hetero_stack
//
// Description: Fast, heterogeneous stack.
//
// Methods:     allocate        - reserve space on stack
//              unwind          - unwind the stack
//              hetero_stack    - c'tor
//              ~hetero_stack   - d'tor, release all dynamic memory
//              push            - push an object on the stack
//              pop             - pop an object from the stack
//
// Members:     m_first_node    -
//              m_current_node  -
//
// Typedefs:    byte_t            -
//
// History:     10/19/2001 - ericne - Created
//
// --------------------------------------------------------------------------
template
<
    size_t  AlignmentT         = sizeof(void*),
    bool    RuntimeTypeCheckT  = true,  // should we perform run-time type checking?
    bool    AssumePodT         = false, // assume non-throwing copy/assign/destroy for better perf
    size_t  DynamicBlockSizeT  = 4096,  // blocks allocated from heap are this size
    size_t  StaticBlockSizeT   = 1024   // initial block on stack is this size
>
class hetero_stack
{
    typedef unsigned char byte_t;
    typedef detail::type_vtable const* vtable_ptr;

public:

    typedef hetero_stack<AlignmentT,RuntimeTypeCheckT,AssumePodT,DynamicBlockSizeT,StaticBlockSizeT> stack_type;

    template< typename T >
    struct aligned_sizeof
    {
        enum
        {
            // round up sizeof(T) to the nearest multiple of AlignmentT
            no_rtti   = ( sizeof( T ) + AlignmentT - 1 ) & ~( AlignmentT - 1 ),
            with_rtti = RuntimeTypeCheckT ?
                no_rtti + aligned_sizeof<vtable_ptr>::no_rtti :
                no_rtti
        };
    };

private:

    struct stack_node
    {
        struct header
        {
            stack_node * m_back;
            stack_node * m_next;
            byte_t     * m_current; // ptr into m_mem. alloc from here
            byte_t     * m_end;     // ptr to last+1 byte_t in m_mem
        };

        union
        {
            header  m_head;
            byte_t  m_align[ aligned_sizeof<header>::no_rtti ];
        };

        // This is the buffer into which values will be pushed and popped.
        // It is guaranteed to meet the AlignmentT requirements because of
        // the union above.
        byte_t  m_mem[1];

        size_t size() const // throw()
        {
            return static_cast<size_t>( m_head.m_end - m_mem );
        }
    };

    enum
    {
        DYNAMIC_BLOCK_SIZE =
            DynamicBlockSizeT > sizeof( stack_node ) ?
            DynamicBlockSizeT : sizeof( stack_node )
    };

    union
    {
        stack_node  m_node;
        byte_t      m_buf[ offsetof( stack_node, m_mem ) + StaticBlockSizeT ];
    } m_first_node;

    stack_node * m_current_node;

    // Cache these for faster access
    byte_t * m_begin;
    byte_t * m_current;
    byte_t * m_end;

    byte_t * grow( size_t size ) // throw(std::bad_alloc)
    {
        // write the cached value of current into the node.
        // OK to do this even if later statements throw.
        m_current_node->m_head.m_current = m_current;

        // Do we have a node with available memory already?
        if( m_current_node->m_head.m_next )
        {
            // Does this node have enough room?
            if( size <= m_current_node->m_head.m_next->size() )
            {
                m_current_node  = m_current_node->m_head.m_next;
                m_current       = m_current_node->m_head.m_current = m_current_node->m_mem + size;
                m_end           = m_current_node->m_head.m_end;
                return m_begin  = m_current_node->m_mem;
            }

            // Create a new node and insert it into the list
            stack_node * new_node = static_cast<stack_node*>(
                ::operator new( size + offsetof( stack_node, m_mem ) ) );

            new_node->m_head.m_back = m_current_node;
            new_node->m_head.m_next = m_current_node->m_head.m_next;

            m_current = m_end = new_node->m_head.m_current =
                new_node->m_head.m_end = new_node->m_mem + size;

            m_current_node->m_head.m_next->m_head.m_back = new_node;
            m_current_node->m_head.m_next = new_node;
            m_current_node = new_node;

            return m_begin = m_current_node->m_mem;
        }

        // We need to create a new node from scratch
        size_t new_size = detail::regex_max( size,
            static_cast<size_t>(DYNAMIC_BLOCK_SIZE) - offsetof( stack_node, m_mem ) );

        stack_node * new_node = static_cast<stack_node*>(
            ::operator new( new_size + offsetof( stack_node, m_mem ) ) );

        new_node->m_head.m_back = m_current_node;
        new_node->m_head.m_next = 0;

        m_current = new_node->m_head.m_current = new_node->m_mem + size;
        m_end     = new_node->m_head.m_end     = new_node->m_mem + new_size;

        m_current_node->m_head.m_next = new_node;
        m_current_node = new_node;

        return m_begin = m_current_node->m_mem;
    }

    byte_t * allocate( size_t size ) // throw(std::bad_alloc)
    {
        // This is the ptr to return
        byte_t * mem = m_current;

        // Advance the high-water mark
        m_current += size;

        // Check to see if we have overflowed this buffer
        if( std::less<void*>()( m_end, m_current ) ) // if( m_end < m_current )
        {
            // oops, back this out.
            m_current = mem;

            // allocate a new block and return a ptr to the new memory
            return grow( size );
        }

        return mem;
    }

    byte_t * unwind( byte_t * pb ) // throw()
    {
        // roll back the stack
        m_current = pb;

        // If we've unwound this whole block, then make the
        // previous node the current node
        if( m_current == m_begin )
        {
            // write the cached value of m_current into m_current_node
            m_current_node->m_head.m_current = m_current;
            m_current_node = m_current_node->m_head.m_back;

            // update the cache
            m_begin   = m_current_node->m_mem;
            m_current = m_current_node->m_head.m_current;
            m_end     = m_current_node->m_head.m_end;
        }

        return pb;
    }

    byte_t * unwind( size_t size ) // throw()
    {
        return unwind( m_current - size );
    }

    void long_jump_impl( void * jump_ptr, detail::bool2type<true> ) // throw()
    {
        safe_long_jump( jump_ptr );
    }

    void long_jump_impl( void * jump_ptr, detail::bool2type<false> ) // throw()
    {
        unsafe_long_jump( jump_ptr );
    }

    struct real_unwinder;
    friend struct real_unwinder;
    struct real_unwinder
    {
        real_unwinder( stack_type * pstack, size_t size ) // throw()
            : m_pstack(pstack), m_size(size) {}

        ~real_unwinder() // throw()
        {
            if( m_pstack )
                m_pstack->unwind( m_size );
        }

        void dismiss() // throw()
        {
            m_pstack = 0;
        }

    private:
        real_unwinder( real_unwinder const & );
        real_unwinder & operator=( real_unwinder const & );

        stack_type * m_pstack;
        size_t       m_size;
    };

    struct dummy_unwinder
    {
        dummy_unwinder( stack_type *, size_t ) {} // throw()
        void dismiss() {} // throw()
    };

    // Disallow these for now. Might implement them later.
    hetero_stack( hetero_stack const & );
    hetero_stack & operator=( hetero_stack const & );

public:

    class type_error : public std::logic_error
    {
        std::type_info const * m_prequested_type;
        std::type_info const * m_pactual_type;
    public:
        type_error
        (
            std::type_info const & requested_type,
            std::type_info const & actual_type,
            std::string const & s = "type error in hetero_stack"
        ) // throw()
            : std::logic_error( s + " (requested type: " + requested_type.name()
                + ", actual type: " + actual_type.name() + ")" )
            , m_prequested_type( &requested_type )
            , m_pactual_type( &actual_type )
        {
        }
        std::type_info const & requested_type() const // throw()
        {
            return *m_prequested_type;
        }
        std::type_info const & actual_type() const // throw()
        {
            return *m_pactual_type;
        }
    };

    hetero_stack() // throw()
        : m_current_node( &m_first_node.m_node )
    {
        m_first_node.m_node.m_head.m_back    = & m_first_node.m_node;
        m_first_node.m_node.m_head.m_next    = 0;
        m_begin = m_current = m_first_node.m_node.m_head.m_current = m_first_node.m_node.m_mem;
        m_end = m_first_node.m_node.m_head.m_end = m_first_node.m_buf + sizeof( m_first_node );
    }

    ~hetero_stack() // throw()
    {
        // AlignmentT must be a power of two
        detail::static_assert< detail::is_power_of_two<AlignmentT>::value > const align_test;

        // Call any destructors for objects still on the stack
        if( RuntimeTypeCheckT && ! AssumePodT )
        {
            long_jump( m_first_node.m_node.m_mem );
        }
        // delete all the memory blocks
        m_current_node = m_first_node.m_node.m_head.m_next;
        for( stack_node * next_node; m_current_node; m_current_node = next_node )
        {
            next_node = m_current_node->m_head.m_next;
            ::operator delete( static_cast<void*>( m_current_node ) );
        }
    }

    template< typename T >
    inline void push( T const & t ) // throw(std::bad_alloc,...)
    {
        // Make sure that the alignment for type T is not worse
        // than our declared alignment.
        detail::static_assert<( AlignmentT >= detail::alignof<T>::value )> const align_test;
        static_cast<void>(align_test);

        // If T won't throw in copy c'tor then we don't need to use an unwinder object.
        typedef typename detail::select< AssumePodT || detail::has_trivial_copy<T>::value,
            dummy_unwinder, real_unwinder >::type unwinder;

        // If this throws, it doesn't change state,
        // so there is nothing to roll back.
        byte_t * pb = allocate( aligned_sizeof<T>::with_rtti );

        // Rolls back the allocate if later steps throw
        // BUGBUG we can do the alloc, but not update m_current until after
        // the copy c'tor to avoid the need for an unwinder object
        unwinder guard( this, aligned_sizeof<T>::with_rtti );

        new ( pb ) T( t ); // Could throw if ! has_trivial_copy<T>::value

        // If we are debugging the stack, then push a pointer to the type_info
        // for this type T. It will be checked in pop().
        if( RuntimeTypeCheckT )
        {
            detail::to_type<vtable_ptr>( pb + aligned_sizeof<T>::no_rtti ) = & detail::type_info_ex<T,AlignmentT>::vtable;
        }

        // ok, everything succeeded -- dismiss the guard
        guard.dismiss();
    }

    template< typename T >
    inline void pop( T & t ) // throw(...)
    {
        detail::static_assert<( AlignmentT >= detail::alignof<T>::value )> const align_test;
        static_cast<void>(align_test);

        // If we are debugging the stack, then in push() we pushed a pointer
        // to the type_info struct for this type T.  Check it now.
        if( RuntimeTypeCheckT )
        {
            byte_t * pti = m_current - aligned_sizeof<vtable_ptr>::no_rtti;
            if( ! detail::type_info_ex<T,AlignmentT>::equals( detail::to_type<vtable_ptr>( pti ) ) )
                throw type_error( typeid( T ), *detail::to_type<vtable_ptr>( pti )->typeinfo_ptr );
        }

        // Don't change state yet because assignment op could throw!
        byte_t * pT = m_current - aligned_sizeof<T>::with_rtti;
        t = detail::to_type<T const>( pT ); // could throw
        T const & ref = detail::to_type<T const>( pT );
        regex::detail::destroy( &ref );
        unwind( pT );
    }

    // Call this version of pop when you don't need the popped value
    template< typename T >
    inline void pop( REGEX_VC6(detail::type2type<T> COMMA int) ) // throw(type_error,...)
    {
        detail::static_assert<( AlignmentT >= detail::alignof<T>::value )> const align_test;
        static_cast<void>(align_test);

        // If we are debugging the stack, then in push() we pushed a pointer
        // to the type_info struct for this type T.  Check it now.
        if( RuntimeTypeCheckT )
        {
            byte_t * pti = m_current - aligned_sizeof<vtable_ptr>::no_rtti;
            if( ! detail::type_info_ex<T,AlignmentT>::equals( detail::to_type<vtable_ptr>( pti ) ) )
                throw type_error( typeid( T ), *detail::to_type<vtable_ptr>( pti )->typeinfo_ptr );
        }

        byte_t * pv = unwind( aligned_sizeof<T>::with_rtti );
        T const & ref = detail::to_type<T const>( pv );
        regex::detail::destroy( &ref );
    }

    // Call this version of pop when you don't need the popped value and
    // throwing an exception isn't an option
    template< typename T >
    inline bool pop( std::nothrow_t const & ) // throw()
    {
        detail::static_assert<( AlignmentT >= detail::alignof<T>::value )> const align_test;
        static_cast<void>(align_test);

        // If we are debugging the stack, then in push() we pushed a pointer
        // to the type_info struct for this type T.  Check it now.
        if( RuntimeTypeCheckT )
        {
            byte_t * pti = m_current - aligned_sizeof<vtable_ptr>::no_rtti;
            if( ! detail::type_info_ex<T,AlignmentT>::equals( detail::to_type<vtable_ptr>( pti ) ) )
                return false; // type error, can't throw so bail.
        }

        byte_t * pv = unwind( aligned_sizeof<T>::with_rtti );
        T const & ref = detail::to_type<T const>( pv );
        regex::detail::destroy( &ref );
        return true;
    }

    template< typename T >
    inline T & top( REGEX_VC6(detail::type2type<T>) ) const // throw(type_error,...)
    {
        detail::static_assert<( AlignmentT >= detail::alignof<T>::value )> const align_test;
        static_cast<void>(align_test);

        if( RuntimeTypeCheckT )
        {
            // If we are debugging the stack, then the top of the stack is a
            // pointer to a type_info struct. Assert that we have the correct type.
            byte_t * pti = m_current - aligned_sizeof<vtable_ptr>::no_rtti;
            if( ! detail::type_info_ex<T,AlignmentT>::equals( detail::to_type<vtable_ptr>( pti ) ) )
                throw type_error( typeid( T ), *detail::to_type<vtable_ptr>( pti )->typeinfo_ptr );
        }

        byte_t * pT = m_current - aligned_sizeof<T>::with_rtti;
        return detail::to_type<T>( pT );
    }

    // Fetch the type_info for the element at the top of the stack
    std::type_info const & top_type() const // throw()
    {
        detail::static_assert< RuntimeTypeCheckT > const type_check;
        static_cast<void>(type_check);

        byte_t * pti = m_current - aligned_sizeof<vtable_ptr>::no_rtti;
        return *detail::to_type<vtable_ptr>( pti )->typeinfo_ptr;
    }

    // Get a pointer to the top of the stack
    void * set_jump() const // throw()
    {
        return m_current;
    }

    // Quick and dirty stack unwind. Does not call destructors.
    void unsafe_long_jump( void *const jump_ptr ) // throw()
    {
        for( ;; )
        {
            if( std::less<void*>()( jump_ptr, m_current_node->m_mem ) ||
                std::less<void*>()( m_current_node->m_head.m_end, jump_ptr ) )
            {
                m_current_node->m_head.m_current = m_current_node->m_mem;
                m_current_node = m_current_node->m_head.m_back;
            }
            else
            {
                m_begin   = m_current_node->m_mem;
                m_current = m_current_node->m_head.m_current = static_cast<byte_t*>( jump_ptr );
                m_end     = m_current_node->m_head.m_end;
                return;
            }
        }
    }

    // Safe long jump; does call destructors if RuntimeTypeCheckT is true.
    void safe_long_jump( void *const jump_ptr ) // throw()
    {
        detail::static_assert< RuntimeTypeCheckT > const type_check;
        static_cast<void>(type_check);

        while( m_current != jump_ptr )
        {
            // The top of the stack is a pointer to a type_vtable struct.
            m_current -= aligned_sizeof<vtable_ptr>::no_rtti;
            vtable_ptr pvtable = detail::to_type<vtable_ptr>( m_current );

            // find the start of the object
            m_current -= pvtable->aligned_size;

            // call the destructor for T
            if( pvtable->destroy )
            {
                pvtable->destroy( m_current );
            }

            // move to the previous buffer if necessary
            if( m_current == m_begin && m_current != jump_ptr )
            {
                m_current_node->m_head.m_current = m_current;
                m_current_node = m_current_node->m_head.m_back;
                m_begin   = m_current_node->m_mem;
                m_current = m_current_node->m_head.m_current;
                m_end     = m_current_node->m_head.m_end;
            }
        }
    }

    // Stack unwind. If RuntimeTypeCheckT && !AssumePodT, then destructors
    // are called.  Otherwise they are not.
    void long_jump( void * jump_ptr ) // throw()
    {
        long_jump_impl( jump_ptr, detail::bool2type<RuntimeTypeCheckT && !AssumePodT>() );
    }

    struct stack_guard
    {
        stack_type * m_ps;
        void       * m_jump_ptr;

        explicit stack_guard( stack_type * ps )
            : m_ps( ps )
            , m_jump_ptr( ps->set_jump() )
        {
        }
        ~stack_guard()
        {
            m_ps->long_jump( m_jump_ptr );
        }
    };

    bool empty() const // throw()
    {
        return m_current == m_first_node.m_node.m_mem;
    }

    // Use scoped_push for automatically pushing/popping
    // things to and from the stack. This is especially useful
    // if you want to push a bunch of things "atomically".  For
    // instance:
    //
    // typedef hetero_stack<>::scoped_pop scoped_pop;
    // scoped_pop p1 = stack.scoped_push( int(1) ); // could throw
    // scoped_pop p2 = stack.scoped_push( std::string("foo") ); // could throw
    // stack.push( float(3.14159) ); // could throw
    // p2.dismiss(); // ok, nothing threw, so ...
    // p1.dismiss(); //  ... dismiss the scoped_pops
    //
    // If p2 and p1 are not dismissed, as in the case when an
    // exception gets thrown, then they automatically pop their
    // arguments from the stack.

    class scoped_pop_base
    {
        scoped_pop_base & operator=( scoped_pop_base const & ); // disallow assignment
    protected:
        mutable stack_type * m_pstack;

        explicit scoped_pop_base( stack_type * pstack ) // throw(std::bad_alloc,...)
          : m_pstack( pstack )
        {
        }
        scoped_pop_base( scoped_pop_base const & right ) // throw() // destructive copy
          : m_pstack( right.m_pstack )
        {
            right.dismiss();
        }
    public:
        void dismiss() const // throw()
        {
            m_pstack = 0;
        }
    };

    template< typename T >
    class scoped_pop_t : public scoped_pop_base
    {
        scoped_pop_t & operator=( scoped_pop_t const & ); // disallow assignment
    public:
        scoped_pop_t( stack_type * pstack, T const & t ) // throw(std::bad_alloc,...)
          : scoped_pop_base( pstack )
        {
            // Note that if this throws an exception the destructor
            // will not get called, which is what we want.
            m_pstack->push( t );
        }
        ~scoped_pop_t() // throw()
        {
            // If we own this stack space, pop it.
            if( m_pstack )
                m_pstack->template pop<T>( std::nothrow );
        }
    };

    template< typename T >
    scoped_pop_t<T> scoped_push( T const & t ) // throw(...)
    {
        return scoped_pop_t<T>( this, t );
    }

    typedef scoped_pop_base const & scoped_pop;
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

} // namespace regex

#endif
