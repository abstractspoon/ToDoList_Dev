//+---------------------------------------------------------------------------
//
//  Copyright ( C ) Microsoft, 1994 - 2002.
//
//  File:       syntax2.h
//
//  Contents:   syntax modules for regexpr
//
//  Classes:    perl_syntax, posix_syntax
//
//  Author:     Eric Niebler ( ericne@microsoft.com )
//
//  History:    3-29-00   ericne   Created
//
//----------------------------------------------------------------------------

#ifndef SYNTAX_H
#define SYNTAX_H

#ifdef _MSC_VER
#pragma warning( push )
// warning C4786: identifier was truncated to '255' characters in the debug information
#pragma warning( disable : 4786 )
#endif

#include <map>
#include <iosfwd>
#include <string>
#include <cctype>
#include <cwctype>
#include <cassert>
#include <iterator>
#include <stdexcept>

#ifndef ARRAYSIZE
# define ARRAYSIZE( a ) (sizeof(a)/sizeof((a)[0]))
#endif

#ifndef UCHAR_MAX
# define UCHAR_MAX 0xff
#endif

#ifndef WCHAR_MAX
# define WCHAR_MAX ((wchar_t)-1)
#endif

#ifdef _MSC_VER
# include <crtdbg.h>
# define REGEX_ASSERT(x)        _ASSERTE(x)
# define REGEX_FORCEINLINE      __forceinline
# define REGEX_SELECTANY        __declspec(selectany)
# define REGEX_CDECL            __cdecl
# define REGEX_SEH_TRY          __try
# define REGEX_SEH_EXCEPT(x)    __except( x )
# define REGEX_RESET_STK_OFLW() _resetstkoflw()
# if 1200 < _MSC_VER
# define REGEX_NOINLINE         __declspec(noinline)
# define REGEX_DEPRECATED       __declspec(deprecated)
# define REGEX_DEPENDENT_TYPENAME typename
# else
# define REGEX_NOINLINE
# define REGEX_DEPRECATED
# define REGEX_DEPENDENT_TYPENAME
# endif
#else
# include <cassert>
# define REGEX_ASSERT(x)        assert(x)
# define REGEX_NOINLINE
# define REGEX_FORCEINLINE      inline
# define REGEX_SELECTANY
# define REGEX_CDECL
# define REGEX_SEH_TRY
# define REGEX_SEH_EXCEPT(x)    if( false )
# define REGEX_RESET_STK_OFLW() ((void)0)
# define REGEX_DEPRECATED
# define REGEX_DEPENDENT_TYPENAME typename
#endif

#define REGEX_STRING(CharT,sz) (::regex::detail::literal<CharT>::string( sz, L##sz ))
#define REGEX_CHAR(CharT,ch) (static_cast<CharT>(::regex::detail::literal<CharT>::template character<ch,L##ch>::value))

#if defined(_MSC_VER) & _CPPLIB_VER <= 310
namespace std
{
template<>
struct iterator_traits< char * >
{    // get traits from iterator _Iter
    typedef random_access_iterator_tag iterator_category;
    typedef char value_type;
    typedef ptrdiff_t difference_type;
    typedef difference_type distance_type;    // retained
    typedef char * pointer;
    typedef char & reference;
};
template<>
struct iterator_traits< char const * >
{    // get traits from iterator _Iter
    typedef random_access_iterator_tag iterator_category;
    typedef char value_type;
    typedef ptrdiff_t difference_type;
    typedef difference_type distance_type;    // retained
    typedef char * pointer;
    typedef char & reference;
};

template<>
struct iterator_traits< wchar_t * >
{    // get traits from iterator _Iter
    typedef random_access_iterator_tag iterator_category;
    typedef wchar_t value_type;
    typedef ptrdiff_t difference_type;
    typedef difference_type distance_type;    // retained
    typedef wchar_t * pointer;
    typedef wchar_t & reference;
};
template<>
struct iterator_traits< wchar_t const * >
{    // get traits from iterator _Iter
    typedef random_access_iterator_tag iterator_category;
    typedef wchar_t value_type;
    typedef ptrdiff_t difference_type;
    typedef difference_type distance_type;    // retained
    typedef wchar_t * pointer;
    typedef wchar_t & reference;
};
}
#endif

namespace regex
{

class bad_regexpr : public std::invalid_argument
{
public:
    explicit bad_regexpr( std::string const & s )
        : std::invalid_argument( s ) {}
    virtual ~bad_regexpr() throw() {}
};

//
// Flags to control how matching occurs
//
enum REGEX_FLAGS
{
    NOFLAGS       = 0x0000,
    NOCASE        = 0x0001, // ignore case
    GLOBAL        = 0x0002, // match everywhere in the string
    MULTILINE     = 0x0004, // ^ and $ can match internal line breaks
    SINGLELINE    = 0x0008, // . can match newline character
    RIGHTMOST     = 0x0010, // start matching at the right of the string
    NOBACKREFS    = 0x0020, // only meaningful when used with GLOBAL and substitute
    FIRSTBACKREFS = 0x0040, // only meaningful when used with GLOBAL
    ALLBACKREFS   = 0x0080, // only meaningful when used with GLOBAL
    NORMALIZE     = 0x0100, // Preprocess patterns: "\\n" => "\n", etc.
    EXTENDED      = 0x0200, // ignore whitespace in pattern
};

// For backwards compatibility
REGEX_FLAGS const noflags = NOFLAGS;

// helper functions to make it easier to combine
// the regex flags.
inline REGEX_FLAGS operator|( REGEX_FLAGS f1, REGEX_FLAGS f2 )
{
    return ( REGEX_FLAGS ) ( ( unsigned )f1 | ( unsigned )f2 );
}
inline REGEX_FLAGS & operator|=( REGEX_FLAGS & f1, REGEX_FLAGS f2 )
{
    return f1 = ( f1 | f2 );
}
inline REGEX_FLAGS operator&( REGEX_FLAGS f1, REGEX_FLAGS f2 )
{
    return ( REGEX_FLAGS ) ( ( unsigned )f1 & ( unsigned )f2 );
}
inline REGEX_FLAGS & operator&=( REGEX_FLAGS & f1, REGEX_FLAGS f2 )
{
    return f1 = ( f1 & f2 );
}
#if !defined(_MSC_VER) | 1200 < _MSC_VER
inline REGEX_FLAGS operator~( REGEX_FLAGS f )
{
    return ( REGEX_FLAGS ) ~( unsigned )f;
}
#endif

//
// The following are the tokens that can be emitted by the syntax module.
// Don't reorder this list!!!
//
enum TOKEN
{
    NO_TOKEN = 0,

    // REGULAR TOKENS
    BEGIN_GROUP,
    END_GROUP,
    ALTERNATION,
    BEGIN_LINE,
    END_LINE,
    BEGIN_CHARSET,
    MATCH_ANY,
    ESCAPE,

    // QUANTIFICATION TOKENS
    ONE_OR_MORE,
    ZERO_OR_MORE,
    ZERO_OR_ONE,
    ONE_OR_MORE_MIN,
    ZERO_OR_MORE_MIN,
    ZERO_OR_ONE_MIN,
    BEGIN_RANGE,
    RANGE_SEPARATOR,
    END_RANGE,
    END_RANGE_MIN,

    // ESCAPE SEQUENCES
    ESC_DIGIT,
    ESC_NOT_DIGIT,
    ESC_SPACE,
    ESC_NOT_SPACE,
    ESC_WORD,
    ESC_NOT_WORD,
    ESC_BEGIN_STRING,
    ESC_END_STRING,
    ESC_END_STRING_z,
    ESC_WORD_BOUNDARY,
    ESC_NOT_WORD_BOUNDARY,
    ESC_WORD_START,
    ESC_WORD_STOP,
    ESC_QUOTE_META_ON,
    ESC_QUOTE_META_OFF,

    // SUBSTITUTION TOKENS
    SUBST_BACKREF,
    SUBST_PREMATCH,
    SUBST_POSTMATCH,
    SUBST_MATCH,
    SUBST_ESCAPE,
    SUBST_QUOTE_META_ON,
    SUBST_UPPER_ON,
    SUBST_UPPER_NEXT,
    SUBST_LOWER_ON,
    SUBST_LOWER_NEXT,
    SUBST_ALL_OFF,

    // CHARSET TOKENS
    CHARSET_NEGATE,
    CHARSET_ESCAPE,
    CHARSET_RANGE,
    CHARSET_BACKSPACE,
    CHARSET_END,
    CHARSET_ALNUM,
    CHARSET_NOT_ALNUM,
    CHARSET_ALPHA,
    CHARSET_NOT_ALPHA,
    CHARSET_BLANK,
    CHARSET_NOT_BLANK,
    CHARSET_CNTRL,
    CHARSET_NOT_CNTRL,
    CHARSET_DIGIT,
    CHARSET_NOT_DIGIT,
    CHARSET_GRAPH,
    CHARSET_NOT_GRAPH,
    CHARSET_LOWER,
    CHARSET_NOT_LOWER,
    CHARSET_PRINT,
    CHARSET_NOT_PRINT,
    CHARSET_PUNCT,
    CHARSET_NOT_PUNCT,
    CHARSET_SPACE,
    CHARSET_NOT_SPACE,
    CHARSET_UPPER,
    CHARSET_NOT_UPPER,
    CHARSET_XDIGIT,
    CHARSET_NOT_XDIGIT,

    // EXTENSION TOKENS
    EXT_NOBACKREF,
    EXT_POS_LOOKAHEAD,
    EXT_NEG_LOOKAHEAD,
    EXT_POS_LOOKBEHIND,
    EXT_NEG_LOOKBEHIND,
    EXT_INDEPENDENT,
    EXT_COMMENT,
    EXT_CONDITION,
    EXT_RECURSE,
    EXT_UNKNOWN
};

namespace detail
{

template< typename CharT > struct literal;

template<> struct literal<char>
{
    static char const * string( char const * sz, wchar_t const * ) { return sz; }
    template< char ch, wchar_t > struct character { enum { value = ch }; };
};

template<> struct literal<wchar_t>
{
    static wchar_t const * string( char const *, wchar_t const * sz ) { return sz; }
    template< char, wchar_t ch > struct character { enum { value = ch }; };
};

struct posix_charset_type
{
    char const * m_szcharset;
    size_t       cchars;
};

extern posix_charset_type const g_rgposix_charsets[];
extern size_t const g_cposix_charsets;

template< typename IterT >
bool is_posix_charset( IterT icur, IterT iend, char const * szcharset )
{
    for( ; iend != icur && char() != *szcharset; ++icur, ++szcharset )
    {
        if( *icur != *szcharset )
            return false;
    }
    return char() == *szcharset;
}

// Forward-declare the class that holds all the information
// about the set of characters that can be matched by a charset
struct charset;
void free_charset( charset const * );

template< typename CharT >
struct charset_map_node
{
    std::basic_string<CharT> m_str;
    charset const * m_rgcharsets[2]; // 0==case, 1==nocase

    charset_map_node()
    {
        m_rgcharsets[0] = m_rgcharsets[1] = 0;
    }

    charset_map_node( charset_map_node const & node )
    {
        *this = node;
    }

    charset_map_node & operator=( charset_map_node const & node )
    {
        m_str = node.m_str;
        m_rgcharsets[0] = node.m_rgcharsets[0];
        m_rgcharsets[1] = node.m_rgcharsets[1];
        return *this;
    }

    void set( std::basic_string<CharT> const & str )
    {
        clear();
        m_str = str;
    }

    void clear()
    {
        std::basic_string<CharT>().swap( m_str );
        free_charset( m_rgcharsets[0] );
        free_charset( m_rgcharsets[1] );
        m_rgcharsets[0] = m_rgcharsets[1] = 0;
    }
};

template< typename CharT >
class charset_map
{
    std::map<CharT, charset_map_node<CharT> > m_map;
public:
    typedef typename std::map<CharT, charset_map_node<CharT> >::iterator iterator;
    ~charset_map()
    {
        for( iterator iter = m_map.begin(); m_map.end() != iter; ++iter )
            iter->second.clear();
    }
    charset_map_node<CharT> & operator[]( CharT ch ) { return m_map[ ch ]; }
    iterator begin() { return m_map.begin(); }
    iterator end() { return m_map.end(); }
    iterator find( CharT ch ) { return m_map.find( ch ); }
    void erase( iterator iter ) { m_map.erase( iter ); }
};


inline detail::charset_map<char> & get_perl_charset_map( char )
{
    static detail::charset_map<char> s_charset_map;
    return s_charset_map;
}

inline detail::charset_map<wchar_t> & get_perl_charset_map( wchar_t )
{
    static detail::charset_map<wchar_t> s_charset_map;
    return s_charset_map;
}

inline detail::charset_map<char> & get_posix_charset_map( char )
{
    static detail::charset_map<char> s_charset_map;
    return s_charset_map;
}

inline detail::charset_map<wchar_t> & get_posix_charset_map( wchar_t )
{
    static detail::charset_map<wchar_t> s_charset_map;
    return s_charset_map;
}

inline bool regex_isspace( char ch )
{
    using namespace std;
    return 0 != isspace( ch );
}

inline bool regex_isspace( wchar_t wch )
{
    using namespace std;
    return 0 != iswspace( wch );
}

template< typename T >
T const & regex_max( T const & lhs, T const & rhs )
{
    return ( lhs > rhs ) ? lhs : rhs;
}

template< typename T >
T const & regex_min( T const & lhs, T const & rhs )
{
    return ( lhs < rhs ) ? lhs : rhs;
}

} // namespace detail

//
// The perl_syntax class encapsulates the Perl 5 regular expression syntax. It is
// used as a template parameter to basic_rpattern.  To customize regex syntax, create
// your own syntax class and use it as a template parameter instead.
//

class perl_syntax_base
{
protected:
    perl_syntax_base()
    {
    }

    static TOKEN const s_rgreg[ UCHAR_MAX + 1 ];
    static TOKEN const s_rgescape[ UCHAR_MAX + 1 ];

    static TOKEN look_up( char ch, TOKEN const rg[] )
    {
        return rg[ static_cast<unsigned char>( ch ) ];
    }
    static TOKEN look_up( wchar_t ch, TOKEN const rg[] )
    {
        return UCHAR_MAX < ch ? NO_TOKEN : rg[ static_cast<unsigned char>( ch ) ];
    }
};

// --------------------------------------------------------------------------
//
// Class:       perl_syntax
//
// Description: Module that encapsulates the Perl syntax
//
// Methods:     eat_whitespace             -
//              min_quant                  -
//              perl_syntax                -
//              perl_syntax                -
//              set_flags                  -
//              get_flags                  -
//              reg_token                  -
//              quant_token                -
//              charset_token              -
//              subst_token                -
//              ext_token                  -
//              get_charset_map            -
//              invalid_charset            -
//              register_intrinsic_charset -
//              _invalid_charset           -
//              _invalid_charset           -
//
// Members:     m_flags                    -
//              s_charset_map              -
//
// Typedefs:    iterator                   -
//              const_iterator             -
//              char_type                  -
//
// History:     11/16/2001 - ericne - Created
//
// --------------------------------------------------------------------------
template< typename CharT >
class perl_syntax : protected perl_syntax_base
{
public:
    typedef typename std::basic_string<CharT>::iterator iterator;
    typedef typename std::basic_string<CharT>::const_iterator const_iterator;
    typedef CharT char_type;
    template< typename OtherT > struct rebind { typedef perl_syntax<OtherT> other; };

private:
    REGEX_FLAGS m_flags;

    const_iterator eat_whitespace( iterator & icur, const_iterator iend )
    {
        if( m_flags & EXTENDED )
        {
            while( iend != icur && ( REGEX_CHAR(CharT,'#') == *icur || detail::regex_isspace( *icur ) ) )
            {
                if( REGEX_CHAR(CharT,'#') == *icur++ )
                {
                    while( iend != icur && REGEX_CHAR(CharT,'\n') != *icur++ ) {}
                }
                else
                {
                    for( ; iend != icur && detail::regex_isspace( *icur ); ++icur ) {}
                }
            }
        }
        return icur;
    }

    bool min_quant( iterator & icur, const_iterator iend )
    {
        return ( iend != eat_whitespace( ++icur, iend ) && REGEX_CHAR(CharT,'?') == *icur ? ( ++icur, true ) : false );
    }

public:

    perl_syntax( REGEX_FLAGS flags )
        : m_flags( flags )
    {
    }

    perl_syntax( perl_syntax<CharT> const & sy )
        : m_flags( sy.m_flags )
    {
    }

    void set_flags( REGEX_FLAGS flags )
    {
        m_flags = flags;
    }

    REGEX_FLAGS get_flags() const
    {
        return m_flags;
    }

    TOKEN reg_token( iterator & icur, const_iterator iend )
    {
        REGEX_ASSERT( iend != icur );
        if( iend == eat_whitespace( icur, iend ) )
            return NO_TOKEN;
        TOKEN tok = look_up( *icur, s_rgreg );
        if( tok )
            ++icur;
        if( ESCAPE == tok && iend != icur )
        {
            tok = look_up( *icur, s_rgescape );
            if( tok )
                ++icur;
            else
                tok = ESCAPE;
        }
        return tok;
    }
    TOKEN quant_token( iterator & icur, const_iterator iend )
    {
        REGEX_ASSERT( iend != icur );
        if( iend == eat_whitespace( icur, iend ) )
            return NO_TOKEN;
        TOKEN tok = NO_TOKEN;
        switch( *icur )
        {
        case REGEX_CHAR(CharT,'*'):
            tok = min_quant( icur, iend ) ? ZERO_OR_MORE_MIN : ZERO_OR_MORE;
            break;
        case REGEX_CHAR(CharT,'+'):
            tok = min_quant( icur, iend ) ? ONE_OR_MORE_MIN : ONE_OR_MORE;
            break;
        case REGEX_CHAR(CharT,'?'):
            tok = min_quant( icur, iend ) ? ZERO_OR_ONE_MIN : ZERO_OR_ONE;
            break;
        case REGEX_CHAR(CharT,'}'):
            tok = min_quant( icur, iend ) ? END_RANGE_MIN : END_RANGE;
            break;
        case REGEX_CHAR(CharT,'{'):
            tok = BEGIN_RANGE;
            ++icur;
            break;
        case REGEX_CHAR(CharT,','):
            tok = RANGE_SEPARATOR;
            ++icur;
            break;
        }
        return tok;
    }
    TOKEN charset_token( iterator & icur, const_iterator iend )
    {
        REGEX_ASSERT( iend != icur );
        TOKEN tok = NO_TOKEN;
        switch( *icur )
        {
        case REGEX_CHAR(CharT,'-'):
            tok = CHARSET_RANGE;
            ++icur;
            break;
        case REGEX_CHAR(CharT,'^'):
            tok = CHARSET_NEGATE;
            ++icur;
            break;
        case REGEX_CHAR(CharT,']'):
            tok = CHARSET_END;
            ++icur;
            break;
        case REGEX_CHAR(CharT,'\\'):
            tok = CHARSET_ESCAPE;
            if( iend == ++icur )
                break;
            switch( *icur )
            {
            case REGEX_CHAR(CharT,'b'):
                tok = CHARSET_BACKSPACE;
                ++icur;
                break;
            case REGEX_CHAR(CharT,'d'):
                tok = ESC_DIGIT;
                ++icur;
                break;
            case REGEX_CHAR(CharT,'D'):
                tok = ESC_NOT_DIGIT;
                ++icur;
                break;
            case REGEX_CHAR(CharT,'s'):
                tok = ESC_SPACE;
                ++icur;
                break;
            case REGEX_CHAR(CharT,'S'):
                tok = ESC_NOT_SPACE;
                ++icur;
                break;
            case REGEX_CHAR(CharT,'w'):
                tok = ESC_WORD;
                ++icur;
                break;
            case REGEX_CHAR(CharT,'W'):
                tok = ESC_NOT_WORD;
                ++icur;
                break;
            }
            break;
        case REGEX_CHAR(CharT,'['):
            if( REGEX_CHAR(CharT,':') == *( ++icur )-- )
            {
                for( size_t i=0; !tok && i < detail::g_cposix_charsets; ++i )
                {
                    if( detail::is_posix_charset<const_iterator>( icur, iend, detail::g_rgposix_charsets[i].m_szcharset ) )
                    {
                        tok = TOKEN( CHARSET_ALNUM + i );
                        std::advance( icur, detail::g_rgposix_charsets[i].cchars );
                    }
                }
            }
            break;
        }
        return tok;
    }
    TOKEN subst_token( iterator & icur, const_iterator iend )
    {
        REGEX_ASSERT( iend != icur );
        TOKEN tok = NO_TOKEN;
        switch( *icur )
        {
        case REGEX_CHAR(CharT,'\\'):
            tok = SUBST_ESCAPE;
            if( iend != ++icur )
                switch( *icur )
                {
                case REGEX_CHAR(CharT,'Q'):
                    tok = SUBST_QUOTE_META_ON;
                    ++icur;
                    break;
                case REGEX_CHAR(CharT,'U'):
                    tok = SUBST_UPPER_ON;
                    ++icur;
                    break;
                case REGEX_CHAR(CharT,'u'):
                    tok = SUBST_UPPER_NEXT;
                    ++icur;
                    break;
                case REGEX_CHAR(CharT,'L'):
                    tok = SUBST_LOWER_ON;
                    ++icur;
                    break;
                case REGEX_CHAR(CharT,'l'):
                    tok = SUBST_LOWER_NEXT;
                    ++icur;
                    break;
                case REGEX_CHAR(CharT,'E'):
                    tok = SUBST_ALL_OFF;
                    ++icur;
                    break;
                }
            break;
        case REGEX_CHAR(CharT,'$'):
            tok = SUBST_BACKREF;
            if( iend != ++icur )
                switch( *icur )
                {
                case REGEX_CHAR(CharT,'&'):
                    tok = SUBST_MATCH;
                    ++icur;
                    break;
                case REGEX_CHAR(CharT,'`'):
                    tok = SUBST_PREMATCH;
                    ++icur;
                    break;
                case REGEX_CHAR(CharT,'\''):
                    tok = SUBST_POSTMATCH;
                    ++icur;
                    break;
                }
            break;
        }
        return tok;
    }
    TOKEN ext_token( iterator & icur, const_iterator iend )
    {
        REGEX_ASSERT( iend != icur );
        if( iend == eat_whitespace( icur, iend ) )
            return NO_TOKEN;
        bool finclude;
        TOKEN tok = NO_TOKEN;
        if( REGEX_CHAR(CharT,'?') == *icur )
        {
            tok = EXT_UNKNOWN;
            ++icur;
            if( m_flags & EXTENDED )
                for( ; iend != icur && detail::regex_isspace( *icur ); ++icur ) {}
            if( iend != icur )
            {
                switch( *icur )
                {
                case REGEX_CHAR(CharT,':'):
                    tok = EXT_NOBACKREF;
                    ++icur;
                    break;
                case REGEX_CHAR(CharT,'='):
                    tok = EXT_POS_LOOKAHEAD;
                    ++icur;
                    break;
                case REGEX_CHAR(CharT,'!'):
                    tok = EXT_NEG_LOOKAHEAD;
                    ++icur;
                    break;
                case REGEX_CHAR(CharT,'#'):
                    tok = EXT_COMMENT;
                    ++icur;
                    break;
                case REGEX_CHAR(CharT,'('):
                    tok = EXT_CONDITION;
                    ++icur;
                    break;
                case REGEX_CHAR(CharT,'R'):
                    tok = EXT_RECURSE;
                    ++icur;
                    break;
                case REGEX_CHAR(CharT,'<'):
                    if( iend == eat_whitespace( ++icur, iend ) )
                        break;
                    switch( *icur )
                    {
                    case REGEX_CHAR(CharT,'='):
                        tok = EXT_POS_LOOKBEHIND;
                        ++icur;
                        break;
                    case REGEX_CHAR(CharT,'!'):
                        tok = EXT_NEG_LOOKBEHIND;
                        ++icur;
                        break;
                    }
                    break;
                case REGEX_CHAR(CharT,'>'):
                    tok = EXT_INDEPENDENT;
                    ++icur;
                    break;
                default:
                    finclude = true;
                    do
                    {
                        if( REGEX_CHAR(CharT,':') == *icur )
                        {
                            tok = EXT_NOBACKREF;
                            ++icur;
                            break;
                        }
                        if( REGEX_CHAR(CharT,')') == *icur )
                        {
                            tok = EXT_NOBACKREF;
                            break;
                        }
                        if( REGEX_CHAR(CharT,'-') == *icur && finclude )
                            finclude = false;
                        else if( REGEX_CHAR(CharT,'i') == *icur )
                            m_flags = ( REGEX_FLAGS ) ( finclude ? ( m_flags | NOCASE )     : ( m_flags & ~NOCASE ) );
                        else if( REGEX_CHAR(CharT,'m') == *icur )
                            m_flags = ( REGEX_FLAGS ) ( finclude ? ( m_flags | MULTILINE )  : ( m_flags & ~MULTILINE ) );
                        else if( REGEX_CHAR(CharT,'s') == *icur )
                            m_flags = ( REGEX_FLAGS ) ( finclude ? ( m_flags | SINGLELINE ) : ( m_flags & ~SINGLELINE ) );
                        else if( REGEX_CHAR(CharT,'x') == *icur )
                            m_flags = ( REGEX_FLAGS ) ( finclude ? ( m_flags | EXTENDED )   : ( m_flags & ~EXTENDED ) );
                        else
                            break;
                    } while( iend != eat_whitespace( ++icur, iend ) );
                    break;
                }
            }
        }
        return tok;
    }

    // Functions used for making user-defined intrinsic character sets
    static detail::charset_map<CharT> & get_charset_map()
    {
        return detail::get_perl_charset_map( CharT() );
    }
    static bool invalid_charset( CharT ch )
    {
        return _invalid_charset( ch );
    }
    static void register_intrinsic_charset( CharT ch, std::basic_string<CharT> const & str ) //throw( bad_regexpr, std::bad_alloc )
    {
        perl_syntax sy( NOFLAGS );
        if( invalid_charset( ch ) )
            throw bad_regexpr( "invalid character specified to register_intrinsic_charset" );
        std::basic_string<CharT> pat = str;
        typename std::basic_string<CharT>::iterator ibegin = pat.begin();
        if( BEGIN_CHARSET != sy.reg_token( ibegin, pat.end() ) )
            throw bad_regexpr( "expecting beginning of charset" );
        regex::detail::charset_map<CharT> & charset_map = get_charset_map();
        regex::detail::charset_map_node<CharT> & map_node = charset_map[ ch ];
        map_node.set( std::basic_string<CharT>( ibegin, pat.end() ) );
    }
 private:
    static bool _invalid_charset( char ch )
    {
        using namespace std;
        return NO_TOKEN != s_rgescape[ static_cast<unsigned char>( ch ) ]
            || isdigit( ch ) || 'e' == ch || 'x' == ch || 'c' == ch;
    }
    static bool _invalid_charset( wchar_t ch )
    {
        return UCHAR_MAX >= ch && _invalid_charset( static_cast<char>( ch ) );
    }
};

// --------------------------------------------------------------------------
//
// Class:       posix_syntax
//
// Description: Implements the basic POSIX regular expression syntax
//
// Methods:     posix_syntax               -
//              posix_syntax               -
//              get_flags                  -
//              set_flags                  -
//              reg_token                  -
//              quant_token                -
//              charset_token              -
//              subst_token                -
//              ext_token                  -
//              get_charset_map            -
//              invalid_charset            -
//              register_intrinsic_charset -
//
// Members:     m_flags                    -
//              s_charset_map              -
//
// Typedefs:    iterator                   -
//              const_iterator             -
//              char_type                  -
//
// History:     11/16/2001 - ericne - Created
//
// --------------------------------------------------------------------------
template< typename CharT >
class posix_syntax
{
    REGEX_FLAGS m_flags;

public:
    typedef typename std::basic_string<CharT>::iterator iterator;
    typedef typename std::basic_string<CharT>::const_iterator const_iterator;
    typedef CharT char_type;
    template< typename OtherT > struct rebind { typedef posix_syntax<OtherT> other; };

    posix_syntax( REGEX_FLAGS flags )
        : m_flags( flags )
    {
    }

    posix_syntax( posix_syntax<CharT> const & sy )
        : m_flags( sy.m_flags )
    {
    }

    REGEX_FLAGS get_flags() const
    {
        return m_flags;
    }

    void set_flags( REGEX_FLAGS flags )
    {
        m_flags = flags;
    }

    TOKEN reg_token( iterator & icur, const_iterator iend )
    {
        TOKEN tok = NO_TOKEN;
        switch( *icur )
        {
        case REGEX_CHAR(CharT,'.'):
            tok = MATCH_ANY;
            ++icur;
            break;
        case REGEX_CHAR(CharT,'^'):
            tok = BEGIN_LINE;
            ++icur;
            break;
        case REGEX_CHAR(CharT,'$'):
            tok = END_LINE;
            ++icur;
            break;
        case REGEX_CHAR(CharT,'['):
            tok = BEGIN_CHARSET;
            ++icur;
            break;
        case REGEX_CHAR(CharT,'\\'):
            tok = ESCAPE;
            ++icur;
            if( iend != icur )
            {
                switch( *icur )
                {
                case REGEX_CHAR(CharT,'('):
                    tok = BEGIN_GROUP;
                    ++icur;
                    break;
                case REGEX_CHAR(CharT,')'):
                    tok = END_GROUP;
                    ++icur;
                    break;
                case REGEX_CHAR(CharT,'|'):
                    tok = ALTERNATION;
                    ++icur;
                    break;
                }
            }
            break;
        }
        return tok;
    }
    TOKEN quant_token( iterator & icur, const_iterator iend )
    {
        TOKEN tok = NO_TOKEN;
        switch( *icur )
        {
        case REGEX_CHAR(CharT,'*'):
            tok = ZERO_OR_MORE;
            ++icur;
            break;
        case REGEX_CHAR(CharT,','):
            tok = RANGE_SEPARATOR;
            ++icur;
            break;
        case REGEX_CHAR(CharT,'\\'):
            ++icur;
            if( iend != icur )
            {
                switch( *icur )
                {
                case REGEX_CHAR(CharT,'?'):
                    tok = ZERO_OR_ONE;
                    ++icur;
                    break;
                case REGEX_CHAR(CharT,'+'):
                    tok = ONE_OR_MORE;
                    ++icur;
                    break;
                case REGEX_CHAR(CharT,'{'):
                    tok = BEGIN_RANGE;
                    ++icur;
                    break;
                case REGEX_CHAR(CharT,'}'):
                    tok = END_RANGE;
                    ++icur;
                    break;
                default:
                    --icur;
                    break;
                }
            }
            else
            {
                --icur;
            }
        }
        return tok;
    }
    TOKEN charset_token( iterator & icur, const_iterator iend )
    {
        TOKEN tok = NO_TOKEN;
        switch( *icur )
        {
        case REGEX_CHAR(CharT,'^'):
            tok = CHARSET_NEGATE;
            ++icur;
            break;
        case REGEX_CHAR(CharT,'-'):
            tok = CHARSET_RANGE;
            ++icur;
            break;
        case REGEX_CHAR(CharT,']'):
            tok = CHARSET_END;
            ++icur;
            break;
        case REGEX_CHAR(CharT,'['):
            if( REGEX_CHAR(CharT,':') == *( ++icur )-- )
            {
                for( size_t i=0; !tok && i < detail::g_cposix_charsets; ++i )
                {
                    if( detail::is_posix_charset<const_iterator>( icur, iend, detail::g_rgposix_charsets[i].m_szcharset ) )
                    {
                        tok = TOKEN( CHARSET_ALNUM + i );
                        std::advance( icur, detail::g_rgposix_charsets[i].cchars );
                    }
                }
            }
            break;
        }
        return tok;
    }
    TOKEN subst_token( iterator & icur, const_iterator iend )
    {
        TOKEN tok = NO_TOKEN;
        if( REGEX_CHAR(CharT,'\\') == *icur )
        {
            tok = SUBST_ESCAPE;
            ++icur;
            if( iend != icur && REGEX_CHAR(CharT,'0') <= *icur && REGEX_CHAR(CharT,'9') >= *icur )
            {
                tok = SUBST_BACKREF;
            }
        }
        return tok;
    }
    TOKEN ext_token( iterator &, const_iterator )
    {
        return NO_TOKEN;
    }

    // Functions for making user-defined intrinsic character sets
    static detail::charset_map<CharT> & get_charset_map()
    {
        return detail::get_posix_charset_map( CharT() );
    }
    static bool invalid_charset( CharT ch )
    {
        return _invalid_charset( ch );
    }
    static void register_intrinsic_charset( CharT ch, std::basic_string<CharT> const & str ) //throw( bad_regexpr, std::bad_alloc )
    {
        posix_syntax sy( NOFLAGS );
        if( invalid_charset( ch ) )
            throw bad_regexpr( "invalid character specified to register_intrinsic_charset" );
        std::basic_string<CharT> pat = str;
        typename std::basic_string<CharT>::iterator ibegin = pat.begin();
        if( BEGIN_CHARSET != sy.reg_token( ibegin, pat.end() ) )
            throw bad_regexpr( "expecting beginning of charset" );
        regex::detail::charset_map<CharT> & charset_map = get_charset_map();
        regex::detail::charset_map_node<CharT> & map_node = charset_map[ ch ];
        map_node.set( std::basic_string<CharT>( ibegin, pat.end() ) );
    }
 private:
    static bool _invalid_charset( char ch )
    {
        static char const s_invalid[] = "0123456789()|?+{}\\exc";
        return 0 != std::char_traits<CharT>::find( s_invalid, ARRAYSIZE( s_invalid ) - 1, ch );
    }
    static bool _invalid_charset( wchar_t ch )
    {
        return UCHAR_MAX >= ch && _invalid_charset( static_cast<char>( ch ) );
    }
};

} // namespace regex

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif

