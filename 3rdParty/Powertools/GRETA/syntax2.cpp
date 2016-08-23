//+---------------------------------------------------------------------------
//
//  Copyright ( C ) Microsoft, 1994 - 2002.
//
//  File:       syntax2.cpp
//
//  Contents:   data definitions for the syntax modules
//
//  Classes:
//
//  Functions:
//
//  Coupling:
//
//  Notes:
//
//  Author:     Eric Niebler ( ericne@microsoft.com )
//
//  History:    3-29-00   ericne   Created
//
//----------------------------------------------------------------------------

#include "syntax2.h"

namespace regex
{

REGEX_SELECTANY TOKEN const perl_syntax_base::s_rgreg[ UCHAR_MAX + 1 ] =
{
/*  0*/    NO_TOKEN,  NO_TOKEN, NO_TOKEN,      NO_TOKEN,    NO_TOKEN,  NO_TOKEN,   NO_TOKEN, NO_TOKEN,
/*  8*/    NO_TOKEN,  NO_TOKEN, NO_TOKEN,      NO_TOKEN,    NO_TOKEN,  NO_TOKEN,   NO_TOKEN, NO_TOKEN,
/* 16*/    NO_TOKEN,  NO_TOKEN, NO_TOKEN,      NO_TOKEN,    NO_TOKEN,  NO_TOKEN,   NO_TOKEN, NO_TOKEN,
/* 24*/    NO_TOKEN,  NO_TOKEN, NO_TOKEN,      NO_TOKEN,    NO_TOKEN,  NO_TOKEN,   NO_TOKEN, NO_TOKEN,
/* 32*/    NO_TOKEN,  NO_TOKEN, NO_TOKEN,      NO_TOKEN,    END_LINE,  NO_TOKEN,   NO_TOKEN, NO_TOKEN,
/* 40*/ BEGIN_GROUP, END_GROUP, NO_TOKEN,      NO_TOKEN,    NO_TOKEN,  NO_TOKEN,  MATCH_ANY, NO_TOKEN,
/* 48*/    NO_TOKEN,  NO_TOKEN, NO_TOKEN,      NO_TOKEN,    NO_TOKEN,  NO_TOKEN,   NO_TOKEN, NO_TOKEN,
/* 56*/    NO_TOKEN,  NO_TOKEN, NO_TOKEN,      NO_TOKEN,    NO_TOKEN,  NO_TOKEN,   NO_TOKEN, NO_TOKEN,
/* 64*/    NO_TOKEN,  NO_TOKEN, NO_TOKEN,      NO_TOKEN,    NO_TOKEN,  NO_TOKEN,   NO_TOKEN, NO_TOKEN,
/* 72*/    NO_TOKEN,  NO_TOKEN, NO_TOKEN,      NO_TOKEN,    NO_TOKEN,  NO_TOKEN,   NO_TOKEN, NO_TOKEN,
/* 80*/    NO_TOKEN,  NO_TOKEN, NO_TOKEN,      NO_TOKEN,    NO_TOKEN,  NO_TOKEN,   NO_TOKEN, NO_TOKEN,
/* 88*/    NO_TOKEN,  NO_TOKEN, NO_TOKEN, BEGIN_CHARSET,      ESCAPE,  NO_TOKEN, BEGIN_LINE, NO_TOKEN,
/* 96*/    NO_TOKEN,  NO_TOKEN, NO_TOKEN,      NO_TOKEN,    NO_TOKEN,  NO_TOKEN,   NO_TOKEN, NO_TOKEN,
/*104*/    NO_TOKEN,  NO_TOKEN, NO_TOKEN,      NO_TOKEN,    NO_TOKEN,  NO_TOKEN,   NO_TOKEN, NO_TOKEN,
/*112*/    NO_TOKEN,  NO_TOKEN, NO_TOKEN,      NO_TOKEN,    NO_TOKEN,  NO_TOKEN,   NO_TOKEN, NO_TOKEN,
/*120*/    NO_TOKEN,  NO_TOKEN, NO_TOKEN,      NO_TOKEN, ALTERNATION,  NO_TOKEN,   NO_TOKEN, NO_TOKEN
// and the rest are 0...
};

REGEX_SELECTANY TOKEN const perl_syntax_base::s_rgescape[ UCHAR_MAX + 1 ] =
{
/*  0*/    NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
           NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
/*  8*/    NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
           NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
/* 16*/    NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
           NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
/* 24*/    NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
           NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
/* 32*/    NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
           NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
/* 40*/    NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
           NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
/* 48*/    NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
           NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
/* 56*/    NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
           NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
/* 64*/    NO_TOKEN,   ESC_BEGIN_STRING, ESC_NOT_WORD_BOUNDARY,       NO_TOKEN,
      ESC_NOT_DIGIT, ESC_QUOTE_META_OFF,              NO_TOKEN,       NO_TOKEN,
/* 72*/    NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
           NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
/* 80*/    NO_TOKEN,  ESC_QUOTE_META_ON,              NO_TOKEN,  ESC_NOT_SPACE,
           NO_TOKEN,           NO_TOKEN,              NO_TOKEN,   ESC_NOT_WORD,
/* 88*/    NO_TOKEN,           NO_TOKEN,        ESC_END_STRING,       NO_TOKEN,
           NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
/* 96*/    NO_TOKEN,           NO_TOKEN,     ESC_WORD_BOUNDARY,       NO_TOKEN,
          ESC_DIGIT,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
/*104*/    NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
           NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN,
/*112*/    NO_TOKEN,           NO_TOKEN,              NO_TOKEN,      ESC_SPACE,
           NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       ESC_WORD,
/*120*/    NO_TOKEN,           NO_TOKEN,      ESC_END_STRING_z,       NO_TOKEN,
           NO_TOKEN,           NO_TOKEN,              NO_TOKEN,       NO_TOKEN
// and the rest are 0...
};

namespace detail
{

REGEX_SELECTANY extern posix_charset_type const g_rgposix_charsets[] =
{
    { "[:alnum:]",    9 },
    { "[:^alnum:]",  10 },
    { "[:alpha:]",    9 },
    { "[:^alpha:]",  10 },
    { "[:blank:]",    9 },
    { "[:^blank:]",  10 },
    { "[:cntrl:]",    9 },
    { "[:^cntrl:]",  10 },
    { "[:digit:]",    9 },
    { "[:^digit:]",  10 },
    { "[:graph:]",    9 },
    { "[:^graph:]",  10 },
    { "[:lower:]",    9 },
    { "[:^lower:]",  10 },
    { "[:print:]",    9 },
    { "[:^print:]",  10 },
    { "[:punct:]",    9 },
    { "[:^punct:]",  10 },
    { "[:space:]",    9 },
    { "[:^space:]",  10 },
    { "[:upper:]",    9 },
    { "[:^upper:]",  10 },
    { "[:xdigit:]",  10 },
    { "[:^xdigit:]", 11 }
};

REGEX_SELECTANY extern size_t const g_cposix_charsets = ARRAYSIZE( g_rgposix_charsets );

} // namespace detail

} // namespace regex
