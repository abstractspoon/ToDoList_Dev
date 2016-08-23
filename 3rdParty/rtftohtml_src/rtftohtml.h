#ifndef _RTFTOHTML_H
#define _RTFTOHTML_H

#include "rtf.h"
#include "vers.h"
#define Htran 3

/*	Text Styles */

    
# define stylePlain			0
# define styleBold			1<<0
# define styleItalic		1<<1
# define styleStrikeThru	1<<2
# define styleOutline		1<<3
# define styleShadow		1<<4
# define styleSmallCaps		1<<5
# define styleAllCaps		1<<6
# define styleInvisible		1<<7
# define styleUnderline		1<<8
# define styleWUnderline	1<<9
# define styleDUnderline	1<<10
# define styleDbUnderline	1<<11
# define styleSuperScript	1<<12
# define styleSubScript		1<<13

typedef long TStyle_typ;
	
# define MTLiteral	9999
# define MTDiscard	9998
# define MTName		9997
# define MTHref		9996
# define MTHot		9995
# define MTFootNote 9994
# define MTSPECIAL  9993

typedef struct  {
	char * Font;			/* Font */
	int FSize;				/* font size */
	TStyle_typ TStyle;		/* Styles that need to match */
	TStyle_typ TMask;		/* Styles that are considered */
	int TTidx;				/* Index into Output Text Style table - or - MTType*/
} TMatchRec;
extern TMatchRec * TMatchArr;
extern int TMatchLen;
extern int TMatchAlloc;

typedef struct  {
	char *PStyle;	/* The Paragraph Style (or some specials) */
	int NestLev;	/* The nesting level represented by this style */
	int PTidx;		/* Index into PTags table */
} PMatchRec;
extern PMatchRec * PMatchArr;
extern int PMatchLen;
extern int PMatchAlloc;

typedef struct  {
    char *Name;
    char *StartTag;
    char *EndTag;
    char *Col2Tag;
    char *TabTag;
    char *ParTag;
    int AllowText;
    int CanNest;
    int DeleteCol1;
    int DoFold;
    int ToCLev;
}PTagRec;
extern PTagRec *PTagArr;
extern int PTagLen;
extern int PTagAlloc;

typedef struct  {
    char *Name;
    char *StartTag;
    char *EndTag;
} TTagRec;
extern TTagRec * TTagArr;
extern int TTagLen;
extern int TTagAlloc;

typedef struct  InStateRec{
	char *ParStyle;				/* Paragraph Style Name */
	TStyle_typ TStyle;			/* Bitmask of Text Styles */
	char *TFont;				/* Points to Font String */
	int TSize;					/* Pointsize of Font */
	int destination;			/* The current input destination */
	int isfootref;				/* Is this a footnote ref? */
	int inTable;				/* In a table */
	int firstcell;				/* first cell definition */
	int cellno;					/* current cell number */
	int lastcell;				/* last cell number */
	int ToCLev;					/* Table of Contents Level */
    struct InStateRec *Next;
} InStateStack;


/* Destinations can be files or strings.  */


#define DSALLOC		256

struct SDest {
    char *ptr;			/* Output string  */
    int alloc;			/* allocation size */
    int used;			/* Number of bytes in destination */
};


#define CELLMAX	80
typedef struct {
	short width;				/* width in character positions */
	short just;				/* justify (left,center,right) */
	short merge;			/* merged with previous cell? */
	struct SDest cbuff;		/* buffer for holding cell data */
	short cpos;				/* output char position */
} cell_t;
extern cell_t cell[CELLMAX];
#define TWIPSperCHAR	109
#define AdjL	0
#define AdjLPad	1
#define AdjRPad	2
#define AdjCpad	3

void	PutHTML ();


/* output file types */

# define	FTTEXT	1		/* text (.html) file */
# define	FTPICT	2		/* PICT graphic file */
# define	FTWMF	3		/* Windows metafile */
# define	FTBMF	4		/* Bitmap file */
/* jrm21 */
#define		FTEMF	5	/* png */
#define		FTPNG	6	/* png */
#define		FTJPEG	7	/* png */

#ifdef __cplusplus
extern "C"
{
#endif
	void WriterInit();
	int do_main ();
	int do_convert(const char* inputFile, const char* outputFile, int img, int writeGraf, int ToC);

	FILE *OpenOutputFile ();
	char *Basename ();

#ifdef __cplusplus
};
#endif

extern char *FPrefix;		/* Base name of output files */
extern char *FPrefixR;		/* Relative name of output files */

extern char *OutfileName;			/* output file name */
extern char *InfileName;	/* input file name */

extern char PFileExt[];	/* extension to be used for links to pictures */
extern int linkself;

/* Translator Options */
extern int debug;
extern int IMG;
extern int ToC;
extern int WriteGraf;
extern int PicGoalWid;
extern int PicGoalHt;
extern char * PictExt;
extern int PictType;

extern int IStyle_Chg;		/* Has input style changed? */

/* Input State Variables */
extern char *ParStyle;		/* Paragraph Style Name */
extern TStyle_typ TStyle;	/* Bitmask of Text Styles */
extern char *TFont;		/* Points to Font String */
extern int TSize;		/* Pointsize of Font */
extern int inTable;			/* In a table? */
extern int ToCLev;
extern int cellno;
extern int firstcell;
extern int lastcell;
extern int destination;		/* The current input destination */
extern RTFFont *fp;			/* Actual Font Pointer */

extern InStateStack *ISS;


extern char *outMap[];
InStateStack *SaveIState();
void RestoreIState();
void PopIState();
void NabPicture();

#endif
