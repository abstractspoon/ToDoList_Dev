/*
	Copyright (C) 2001, 2002, 2003, 2004, 2008 Sergey A. Galin, http://sageshome.net

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdafx.h"
#include "rtf2html.h"

CRTFColor DefColors[]={
  {  0,    0,   0 }, {  0,    0, 255 }, {  0,  255, 255 }, {  0,  255,   0 },
  { 255,   0, 255 }, { 255,   0,   0 }, { 255, 255,   0 }, { 255, 255, 255 },
  {   0,   0, 128 }, {   0, 128, 128 }, {   0, 128,   0 }, { 128,   0, 128 },
  { 128,   0,   0 }, { 128, 128,   0 }, { 128, 128, 128 }, { 192, 192, 192 },
  { 153,  51,   0 }, {  51,  51, 153 }, { 255, 102,   0 }, {   0, 204, 255 }};

size_t DefColorsCount=ARRAY_ITEMCOUNT(DefColors);

/*
// Keyword descriptions taken from M$'s sample app
SYM rgsymRtf[] = {
//  keyword     dfltfPassDflt   kwd         idx
    "li",       0,      fFalse,     kwdProp,    ipropLeftInd,
    "ri",       0,      fFalse,     kwdProp,    ipropRightInd,
    
    "sbknone",  sbkNon, fTrue,      kwdProp,    ipropSbk,
    "sbkcol",   sbkCol, fTrue,      kwdProp,    ipropSbk,
    "sbkeven",  sbkEvn, fTrue,      kwdProp,    ipropSbk,
    "sbkodd",   sbkOdd, fTrue,      kwdProp,    ipropSbk,
    "sbkpage",  sbkPg,  fTrue,      kwdProp,    ipropSbk,
    "margl",    1800,   fFalse,     kwdProp,    ipropXaLeft,
    "margr",    1800,   fFalse,     kwdProp,    ipropXaRight,
    "margt",    1440,   fFalse,     kwdProp,    ipropYaTop,
    "margb",    1440,   fFalse,     kwdProp,    ipropYaBottom,

    "bin",      0,      fFalse,     kwdSpec,    ipfnBin,

    };
*/



TRTFSymbol RtfSym[]={
    {  "u", saUTF,                    NULL, NULL, NULL, 0, 0, 0 },
    #if defined(USE_FONT_TAGS)
        // B and I tags are not deprecated in HTML 4 and I use them as HTML tags.
	// Also it icreases compatibility.
	{ "b", saTag, "<B>",   NULL, "</B>",            0, 0, 0 },
	{ "i", saTag, "<I>",   NULL, "</I>",            0, 0, 0 },
    #else
	{ "b", saFontClass, "b ", "wn ", NULL, 0, zaUseB, 0 },
	{ "i", saFontClass, "i ", "sn ", NULL, 0, zaUseB, 0 },
    #endif

    { "super",  saTag, "<SUP>", NULL, "</SUP>",         0, 0, 0 },
    { "sub",    saTag, "<SUB>", NULL, "</SUB>",         0, 0, 0 },

    // UL and S (==STRIKE) tags are deprecated in HTML 4. Convert them to CSS
    { "ul",         saFontClass, "u ",  "nd ", NULL, 0, zaUseB, 0 },
    { "ulnone",     saFontClass, "nd ", NULL, NULL,     0, 0, 0 },
    { "strike",     saFontStyle, "text-decoration: line-through; ", "text-decoration: none; ", NULL, 0, zaUseB, 0 },
    { "scaps",      saFontStyle, "text-transform: lowercase; ", "text-transform: none; ", NULL, 0, zaUseB, 0 },
    { "caps",       saFontStyle, "text-transform: uppercase; ", "text-transform: none; ", NULL, 0, zaUseB, 0 },
    { "v",          saFontStyle, "visibility: hidden; ", "visibility: visible; ", NULL, 0, zaUseB, 0 },
    { "up",         saFontStyle, "vertcal-align: super; ", NULL, NULL, 0, 0, 0 },
    { "dn",         saFontStyle, "vertcal-align: sub; ",   NULL, NULL, 0, 0, 0 },

    // Lists-related tags
    { "listtext",   saListText,       NULL, NULL, NULL, 0, 0, 0 },
    { "pntext",     saListText,       NULL, NULL, NULL, 0, 0, 0 },
    { "ilvl",       saListLevel,      NULL, NULL, NULL, 0, 0, 0 },

    // Picture-related tags
    { "*\\shppict", saShpPict,        NULL, NULL, NULL, 0, 0, 0 },
    { "*\\picprop", saIgnore,         NULL, NULL, NULL, 0, 0, 0 },
    { "pict",       saPict,           NULL, NULL, NULL, 0, 0, 0 },
    { "nonshppict", saNonShpPict,     NULL, NULL, NULL, 0, 0, 0 },


    // Table-related keywords
    { "trowd",      saTrowd,          NULL, NULL, NULL, 0, 0, 1 },
    { "cell",       saCell,           NULL, NULL, NULL, 0, 0, 1 },
    { "row",        saRow,            NULL, NULL, NULL, 0, 0, 1 },
    { "*\\nesttableprops",  saNested, NULL, NULL, NULL, 0, 0, 2 },
    { "nestcell",   saCell,           NULL, NULL, NULL, 0, 0, 2 },
    { "nestrow",    saRow,            NULL, NULL, NULL, 0, 0, 2 },
    { "itap",       saItap,           NULL, NULL, NULL, 0, 0, 0 },
    { "nonesttables", saSkip,         NULL, NULL, NULL, 0, 0, 0 },
    
    { "line",       saTag,       "<BR>",    NULL, NULL, 0, 0, 0 },
    { "par",        saPar,            NULL, NULL, NULL, 0, 0, 0 },
	
    { "f",          saFont,      "f",  " ",       NULL, 0, 0, 0 },
    { "fs",         saFontSize,  "sz", " ",       NULL, 2, 0, 0 },
    { "cf",         saFontColor, "cl", " ",       NULL, 0, 0, 0 },
    { "highlight",  saFontBg,    "cb", " ",       NULL, 0, 0, 0 },
    { "chcbpat",    saFontBg,    "cb", " ",       NULL, 0, 0, 0 },    
    { "deff",       saBodyFont,  "@f", ";",       NULL, 0, 0, 0 }, // Default document font

    { "buptim",     saSkip,           NULL, NULL, NULL, 0, 0, 0 },
    { "creatim",    saSkip,           NULL, NULL, NULL, 0, 0, 0 },     
    { "doccomm",    saSkip,           NULL, NULL, NULL, 0, 0, 0 },     
    { "footer",     saSkip,           NULL, NULL, NULL, 0, 0, 0 },     
    { "footerf",    saSkip,           NULL, NULL, NULL, 0, 0, 0 },     
    { "footerl",    saSkip,           NULL, NULL, NULL, 0, 0, 0 },     
    { "footerr",    saSkip,           NULL, NULL, NULL, 0, 0, 0 },     
    { "footnote",   saSkip,           NULL, NULL, NULL, 0, 0, 0 },     
    { "ftncn",      saSkip,           NULL, NULL, NULL, 0, 0, 0 },     
    { "ftnsep",     saSkip,           NULL, NULL, NULL, 0, 0, 0 },     
    { "ftnsepc",    saSkip,           NULL, NULL, NULL, 0, 0, 0 }, 
    { "header",     saSkip,           NULL, NULL, NULL, 0, 0, 0 }, 
    { "headerf",    saSkip,           NULL, NULL, NULL, 0, 0, 0 }, 
    { "headerl",    saSkip,           NULL, NULL, NULL, 0, 0, 0 }, 
    { "headerr",    saSkip,           NULL, NULL, NULL, 0, 0, 0 }, 
    { "printim",    saSkip,           NULL, NULL, NULL, 0, 0, 0 }, 
    { "private1",   saSkip,           NULL, NULL, NULL, 0, 0, 0 },
    { "revtim",     saSkip,           NULL, NULL, NULL, 0, 0, 0 },
    { "rxe",        saSkip,           NULL, NULL, NULL, 0, 0, 0 },
    { "stylesheet", saSkip,           NULL, NULL, NULL, 0, 0, 0 },
    { "tc",         saSkip,           NULL, NULL, NULL, 0, 0, 0 },
    { "txe",        saSkip,           NULL, NULL, NULL, 0, 0, 0 },
    { "xe",         saSkip,           NULL, NULL, NULL, 0, 0, 0 },

    { "ql",         saParClass, "lft ",     NULL, NULL, 0, 0, 0 },
    { "qr",         saParClass, "rgt ",     NULL, NULL, 0, 0, 0 },
    { "qc",         saParClass, "cen ",     NULL, NULL, 0, 0, 0 },
    { "qj",         saParClass, "jus ",     NULL, NULL, 0, 0, 0 },
    { "pagebb",     saParStyle, "page-break-before: always; ", NULL, NULL, 0, 0, 0 },
    { "sb",         saParStyle, "margin-top: ",    "pt; ", NULL, 20, 1, 0 },
    { "sa",         saParStyle, "margin-bottom: ", "pt; ", NULL, 20, 1, 0 },
    { "li",         saParStyle, "margin-left: ",   "pt; ", NULL, 20, 1, 0 },
    { "ri",         saParStyle, "margin-right: ",  "pt; ", NULL, 20, 1, 0 },
    { "fi",         saParStyle, "text-indent: ",   "pt; ", NULL, 20, 1, 0 },

    { "ansicpg",    saHeadTagDig, "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=Windows-", "\">\n", NULL, 0, 0, 0 },
    { "title",      saHeadTag, "<title>", "</title>\n", NULL, 0, 0, 0 },
    { "author",     saHeadTag, "<META HTTP-EQUIV=\"Author\" CONTENT=\"","\">\n", NULL, 0, 0, 0 },
    { "keywords",   saHeadTag, "<META HTTP-EQUIV=\"Keywords\" CONTENT=\"","\">\n", NULL, 0, 0, 0 },
    { "subject",    saHeadTag, "<META HTTP-EQUIV=\"Descrption\" CONTENT=\"","\">\n", NULL, 0, 0, 0 },
    { "comment",    saSkip,           NULL, NULL, NULL, 0, 0, 0 },
    { "operator",   saHeadTag, "<!-- Operator: ", " -->\n", NULL, 0, 0, 0 },
    { "*\\company", saHeadTag, "<!-- Company: ", " -->\n", NULL, 0, 0, 0 },

    //  { "page",   saReplace,  "<DIV STYLE=\"page-break-after: always;\"></DIV>\n" },
    //  { "sect",   saReplace,  "<DIV STYLE=\"page-break-after: always;\"></DIV>\n" },

    { "ldblquote",  saTag,  "&quot;",       NULL, NULL, 0, 0, 0 },
    { "rdblquote",  saTag,  "&quot;",       NULL, NULL, 0, 0, 0 },
    { "\0x0d",      saTag,  "\0x0d",        NULL, NULL, 0, 0, 0 },
    { "\0x0a",      saTag,  "\0x0a",        NULL, NULL, 0, 0, 0 },
    // since HTML does not support tabs, just replace tab with white space
    { "tab",        saTag,  "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;", NULL, NULL, 0, 0, 0 },
    { "fonttbl",    saFontTable,      NULL, NULL, NULL, 0, 0, 0 },
    { "colortbl",   saColorTable,     NULL, NULL, NULL, 0, 0, 0 },
    
    // Completely useless tags
    { "pntxta",     saSkip,           NULL, NULL, NULL, 0, 0, 0 },
    { "pntxtb",     saSkip,           NULL, NULL, NULL, 0, 0, 0 },
    { "info",       saIgnore,         NULL, NULL, NULL, 0, 0, 0 }, // sub-tags will be parsed OK
    { "facingp",    saIgnore,         NULL, NULL, NULL, 0, 0, 0 }, // Facing Pages - not applicable to HTML
    { "landscape",  saIgnore,         NULL, NULL, NULL, 0, 0, 0 }, // Landscape paper - not applicable to HTML
    { "adjustright",saIgnore,         NULL, NULL, NULL, 0, 0, 0 }
};
size_t RtfSymCount=ARRAY_ITEMCOUNT(RtfSym);

TFontAlias FontAliases[]={
    // { "fnil", "" }  // Unknown font group - just not included
    { "froman",    "serif"      },
    { "fswiss",    "sans-serif" },
    { "fmodern",   "monospace"  },
    { "fscript",   "cursive"    },
    { "fdecor",    "fantasy"    },
    { "ftech",     "Math"       }
};
size_t FontAliasesCount=ARRAY_ITEMCOUNT(FontAliases);

TFontAlias KeyFonts[]={
    { "Times",     "Times"      },
    { "Arial",     "Arial"      },
    { "Helvetika", "Helvetika"  },
    { "Courier",   "Courier"    }
};
size_t KeyFontsCount=ARRAY_ITEMCOUNT(KeyFonts);



