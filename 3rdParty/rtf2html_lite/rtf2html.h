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

// This is a header file for RTF -> HTML conversion

#include "stdafx.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "gstring.h"
#include "sets.h"

#if !defined(RTFHTML_H)
#define RTFHTML_H

#define R2H_VERSION "0.3"
#define R2H_GENERATOR "RTF to HTML Converter " R2H_VERSION ", (C) Sergey A. Galin, 2001-2004"
#define R2H_ABOUT R2H_GENERATOR


//#define R2H_DEBUG   1 /* tells that the program is compiled for debug */
#define PAR "P"     /* HTML tag that will be used for paragraph formatting */
#define USE_FONT_TAGS 1


#if defined(LINUX)
    #define UNIX_PATHS 1
#endif  

#if defined(UNIX_PATHS)
    #define TREE_SYMBOL   "/" 
    #define TREE_SYMBOL_CHAR  '/'
#else
    #define TREE_SYMBOL   "\\"  
    #define TREE_SYMBOL_CHAR  '\\'
#endif


#if !defined(_MSC_VER) && !defined(DLLBUILD)
    #define CONSOLE_APP  1     /* tells that console I/O is available */
    #if !defined(__GNUC__)
        // using 'pascal' call method helps to link program 
        // with object modules created by non-C++ compliers (e.g. Pascal)
        #define EXPORTED_FN(typ) typ pascal
    #else
        #define EXPORTED_FN(typ) typ
    #endif  
#else
    // MSC DLL Exporting.
    // Don't forget to list all exported functions in EXPORTS section in 
    // project's .def file to have them compiled properly
    #if defined(_MSC_VER)
        #define EXPORTED_FN(typ) extern "C" _declspec(dllexport) typ //pascal
    #else
        #define EXPORTED_FN(typ) extern "C" __declspec(dllexport) typ 
    #endif
#endif

// Although mkdir and unlink functions seems to be ANSI, 
// they are defined in different ways in different compilators.
#if defined(__GNUC__)
    #include <unistd.h>
    #include <sys/stat.h>
    #if defined(__WIN32__)
        #define MKDIR_FN(x)   mkdir(x)   
    #else
        #define MKDIR_FN(x)   mkdir(x, 0755)
    #endif
    #define UNLINK_FN(x)  unlink(x)
#else    
    #if defined(_MSC_VER)
        #include <direct.h>
        #define MKDIR_FN  _mkdir
        #define UNLINK_FN _unlink
    #else
        #include <dir.h>
        #define MKDIR_FN  mkdir
        #define UNLINK_FN unlink
    #endif
#endif    

#if defined(R2H_DEBUG) && defined(CONSOLE_APP)
     #define SGTRACE    printstring
     #define SGTRACEINT printlong
     #if defined(R2H_VERBOSE)
         #define SGVERB  printstring
	 #define SGVERBI printlong
     #else
         #define SGVERB(x)     ((void)0)
	 #define SGVERBI(m,x)  ((void)0)
     #endif
     inline void printstring(const char* x){ printf("%s", x); }
     inline void printlong(const char* m, long x){ printf("%s %ld\n", m, x); }
#else
     #define SGTRACE(x)        ((void)0)
     #define SGTRACEINT(m, x)  ((void)0)
     #define SGVERB(x)         ((void)0)
     #define SGVERBI(m,x)      ((void)0)
#endif

#define ARRAY_ITEMCOUNT(x) (sizeof(x)/sizeof(x[0]))

inline int Twips2Points(int twips){ return twips/20; }
inline int Points2Twips(int points){ return points*20; }
inline int hex2bin(char c){return (c>='0' && c<='9')?c-'0':toupper(c)-'A'+10;}

void HtmlChar(CgString &buf, char c);
void Text2HtmlWS(CgString& output, const char *input);
CgString& ClassStyle2HTML(CgString& output, const char* c, const char* s);
CgString& r2hExtractFilePath(CgString& output, const char *fn);
CgString& r2hExtractFileName(CgString& output, const char *fn);

const int r2hMaxFilesScan=100;
int r2hFileExists(const char* fn);
int r2hSuchImageExists(const char *fn);
int r2hTestRewrite(const char* fn, int DeleteTest=0);
int r2hCreateFileName(CgString &output, const char *path, unsigned long *Number);

void ImageRtf2Binary(FILE *out, const char *buf);

// RTF color definition
// physically identical to standard RGB structure
struct CRTFColor{
    unsigned char R, G, B;
    CRTFColor& cRGB(char r, char g, char b){ R=r; G=g; B=b; return *this;}
    CRTFColor& cRGB(long rgb){ R=(rgb>>16)&0xFF; G=(rgb>>8)&0xFF; B=rgb&0xFF; return *this; }
    int operator==(const CRTFColor& c){ return R==c.R && G==c.G && B==c.B; }
};

inline CgString& operator<<(CgString& s, const CRTFColor &c){
    char buf[8];
    sprintf(buf, "#%02X%02X%02X", (unsigned)c.R, (unsigned)c.G, (unsigned)c.B);
    return s<<buf;
}

enum TSymbolAction{
    saIgnore=0,     // ignore this tag without any action
    saUTF,          // \u tag (UNICODE character)
    saSkip,         // skip tag {} group
    saTag,          // place HTML tag or text string
    saFontSize,     // set font size (adds font size to used sizes table)
    saFontColor,    // set font color (adds color to ColorTable)
    saFontBg,       // set font background color (adds color to ColorTable)

    saBodyFont,   // add CSS2 code to BODY tag

    saParStyle,   // set paragraph style (inline CSS)
    saParClass,     // set paragraph class (reference to global CSS) 
    
    saFont,         // requires marking that font is used
    saFontStyle,  // add a substring to current font style (inline CSS)
    saFontClass,  // add a substring to current font class (reference to global CSS) 

    saListText,     // completely special processing :(
    saListLevel,  // Not implemented yet. May be useful to generate nested <UL> / <OL> tags

    saTableTag,
    saTrowd,    // table row definition
    saCell,     // end of cell
    saRow,      // end of row
    saItap,     // embedded table level
    saNested,   // open a nested table properties definition

    saPar,          // new paragraph (special processing)
    saHeadTag,      // get text content from RTF group and put it to tag in HEAD
    saHeadTagDig,   // use digital parameter of RTF tag in HEAD tag

    saFontTable,    // process font table
    saColorTable, // process color table

    saPict,     // picture
    saShpPict,    // original picture (copy of original file in original format)
    saNonShpPict  // compatible picture
};

// do special processing if keyword has a 0 parameter ?
enum TZeroAction{ zaVoid=0, zaIgnore=1, zaUseB=2 };

// ### Do not modify these constants!
typedef unsigned long r2hFlags;
const r2hFlags            
  r2hUseImageSize=1,
  r2hUseImageDirectory=2,
  r2hPreferUnicode=4;

// Internal 'escaping' character. It must be put into buffer before characters
// that should not be HTML-encoded. It can be any ASCII characeter which is 
// unlikey present in any RTF, except '\0'.
const char r2hEscapeChar=char(1);

struct TRTFSymbol {
    char          *Keyword;     // RTF keyword
    TSymbolAction  Action;      // what to do with this keyword
    char          *A, *B, *C;   // text before par / after par / closing tag
    int            Divider,     // divide keyword parameter by...
                   ZeroAction,  // special action if kwdpar==0 ?
                   Wad;         // additional field

    int NoZeros(){ return ZeroAction==zaIgnore && B!=NULL; }
};

struct TFontAlias{
    char *Tag;
    char *Name;
};

class TRTFImage{
    public:
        CgString pibName;
        char Extension[8];
        long ScaleX, ScaleY, CropL, CropR, CropT, CropB, Width, Height, WGoal, HGoal,
            BlipTag, ShapeType;
        int  FlipH, FlipV;

        void Clear(){ 
            pibName.Truncate(); 
            strcpy(Extension, "wmf");
            ScaleX=ScaleY=100;
            CropL=CropR=CropT=CropB=0;
            Width=Height=20;  
	    WGoal=HGoal=0; //??
            FlipH=FlipV=0;
        }

  TRTFImage():pibName(){ Clear();}
};

struct TRTFSimpleTag{
    const char* Tag;
    int Code;
};

// RTF parser error codes

const int
    r2hMaxMessageLen=80,
    ecOK=0,
    ecCannotOpenFile=1,
    ecCannotCreateFile=2,
    ecCannotCreateDirectory=3,
    ecCannotGenerateName=4,
	ecCannotAllocateInputBuffer=5,
	ecErrorWritingOutputFile=6;

const size_t
    maxClosingTags=128,
    maxTag=32,
    maxKeyword=32;

const long ParNotUsed=-1111111L;

enum TWordType{ wtWord, wtHex, wtDecimal };
enum TSeqType{ stNone, stString, stKeyword };

typedef void TProgressFn(int percentage);

enum TTableTagState{
    tsNone=0,
    tsTableTag=1,
    tsRowTag=2,
    tsCellTag=3,
    tsCellText=4
};

class TTableState{
    public:
        TTableTagState Tag;
        CgString       CellText, CellParClass, CellParStyle;
        CgString      *PrevOutput;

        // constructors / destructors and some operators required for correct
        // compilation of CSet<TTableState>, e.g. operator==, operator=...
        TTableState(): Tag(tsNone), CellText(), CellParClass(), CellParStyle(){;}
        ~TTableState(){ CellText.~CgString(); }
        //TTableState(const TTableState &s): Tag(s.Tag), CellText(s.CellText){;}
        TTableState& operator=(const TTableState &s){ Tag=s.Tag; CellText=s.CellText; return *this; }
        int operator==(const TTableState &ts){ return Tag==ts.Tag && CellText==ts.CellText; }

        void ApplyCellTag(CRevString& closingtags);
    void ApplyRowTag();
};

typedef CSet<CgString>    CgStringArray;
typedef CSet<CRTFColor>   CRTFColorArray;
typedef CSet<TTableState> CTableStack;


class CRTF2HTML{
    protected:
		const char *OutFile;
		char *InBuffer, *OutBuffer;
		int InIndex, InLength;


        // CSS / HTML builder information: global variables to hold HTML parameters
        CgString HeadTags, Style, BodyStyle;
        CIntSet UsedFontSizes, UsedFonts, BgColors, FontColors;
        CgStringArray Fonts;
        CRTFColorArray ColorTable;
        CTableStack TableStack;

        // buffers used in Parser() and other parsing routines temporarily
        CgString FontStyle, FontClass, ListText; 
        int ListLevel, NextListLevel, InListText;

        // buffer used in Image Parsing routines
        TRTFImage Image;
        int InShpPict, ImageUsable, ShpPictUsed;

        // callback function (NOT IMPLEMENTED YET)
        TProgressFn *ProgressFn;
        long Count;

        // Basic I/O functions
//        int GetCh(){ return getc(InHandle); }
//        int UngetCh(int c){ return fseek(InHandle, -1, SEEK_CUR); /*ungetc(c, InHandle);*/ }
//        int PutCh(int c){ return putc(c, OutHandle); }
//        int Eof(){ return feof(InHandle); }
		int GetCh(){ return (InIndex+1 != InLength) ? InBuffer[InIndex++] : -1; }
        int UngetCh(int c){ InIndex--; return c; }
        int PutCh(int c){ return 0; }
		int Eof(){ return (InIndex+1 != InLength) ? 0 : 1; }
        int PutS(const char *s);

        // High-level RTF-oriented I/O functions
        void LoadKeyword(CgString& s, int &endchr);
        long LoadParameter(int &endchr);
        TSeqType LoadSequence(CgString &s, long *par=NULL, int *endchr=NULL);
        void SkipBrackets(int lev=0);
        void LoadTextValue(CgString &s);
        void UnicodeChar(CgString *output, long kwdpar);

        //  Special tag pasing functions
        void ParseFont();
        void ParseFontTable();
        void ParseColorTable();
    
        void ParseImageProperties();
        int  ParseImage();
        int  ImagePropertyCode;


        void FlushPar(CRevString &closingtags, int &P_tag, int Clear);
        void FlushList();
        void PushTable();
        void PopTable();
        int  Parser();
        void UpdateStyle();

        CgString *output;

    public:
        r2hFlags Flags;

        CgString ImageDirectory;

        CRTF2HTML(long flg=0, const char *dir=""): ProgressFn(NULL), Image(), Flags(flg), 
	  OutFile(NULL), InBuffer(NULL), OutBuffer(NULL), output(NULL), ImageDirectory(dir){;}
        //~CRTF2HTML();
        int Process(const char *rtf, const char *html);
        int ProcessBuffer(const char *rtf, long len, char **html);
};

EXPORTED_FN(int) Convert(const char *rtf, const char *html, r2hFlags Flags=0, const char *imgdir="");
EXPORTED_FN(int) ConvertBuffer(const char *rtf, long len, char **html, r2hFlags Flags=0, const char *imgdir="");
EXPORTED_FN(char*) r2hErrorMessage(int code, char *result);
EXPORTED_FN(char*) r2hAboutMessage();

#endif
