#ifndef _HTMLOUT_H
#define _HTMLOUT_H

#include	"rtf.h"
#include	"rtftohtml.h"
#include	"string.h"


#define MAXNEST 8

/* Destinations can be files or strings.  */


/* SSC codes are used in saving the state */
#define SSCSPECIAL	0
#define SSCSTYLECHG	1
#define SSCNCODES	2	/* Set this to number of SSCodes */

struct SSData {
	int Special;
	int cpos;
	InStateStack *TheState;
};

struct SSStruct {
	struct SDest SD;
	int alloc;
	int used;
	struct SSData *data;
} PNSave,FNSave;

struct FDest {
    char filename[256];		/* Name of output file */
    char wbuff[82];		/* Output buffer */
    FILE *outfd;		/* Output file - (file destinations only) */
    int wrap;		    /* should we wrap text or not */
    int HPTags;			/* Highest Paragraph Tag */
    int HTTags;			/* Highest Text Tag */
    int NeedPar;		/* Output Paragraph before next text */
    int InCol2;			/* We are past Column 1 */
    int PTidx;			/* Paragraph Tag Index */
    int TSpecial;		/* Special Tag */
    int cpos;			/* character position */
    int TStack[MAXNEST];	/* Stack of tags */
};

#define ConvertCaps	((TStyle&(styleAllCaps|styleSmallCaps))!=0)
#define MAXOBUF	8096

/* prototypes */
#ifdef __cplusplus
extern "C"
{
#endif
void HTMLInit();
void HTMLCleanup();
void HTMLEndDivert();
void HTMLStartDivert();
void PutHTMLString(char *);
void PutSpecial(int Special);
#ifdef __cplusplus
}
#endif

#endif
