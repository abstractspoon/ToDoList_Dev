/*
	rtftohtml -	RTF to HTML translator

			Initial Implementation by Chris Hector (cjh@cray.com)
			
			(c) 1994 by Chris Hector
			May be freely distributed.

			This translator was based on rtfskel. 
			Kudos to Paul DuBois for his work in developing the
			rtf reader and skeleton code.

*/
#include	<stdio.h>
#include	<stdlib.h>
#include	"rtf.h"
#include	"rtftohtml.h"
#include "htmlout.h"
#include "transinit.h"
#include "rtf-namedef.h"

/* globals - jrm21 */
TMatchRec * TMatchArr;
int TMatchLen;
int TMatchAlloc;
PMatchRec * PMatchArr;
int PMatchLen;
int PMatchAlloc;
PTagRec *PTagArr;
int PTagLen;
int PTagAlloc;
TTagRec * TTagArr;
int TTagLen;
int TTagAlloc;
cell_t cell[CELLMAX];
int PicGoalWid;
int PicGoalHt;
char * PictExt;
int PictType;

static void	UnknownClass ();
static void	GroupClass ();
static void	ControlClass ();

static void	Destination ();
static void	SpecialChar ();
static void	DocAttr ();
static void	SectAttr ();
static void	TblAttr ();
static void	ParAttr ();
static void	CharAttr ();
static void	PictAttr ();
static void	NeXTGrAttr ();
static void	FieldAttr ();
static void	TOCAttr ();
static void	PosAttr ();

void PushIState();
/* following is in reader.c */
extern int RTFReadOutputMap(char *file, char*outMap[], int reinit);


char *outputMapName="html-map";
char *TransFile=	"html-trn";

/* Translator Options */
int debug=1; /*jrm21*/
int IMG=0;
int ToC=1;
int WriteGraf=1;

/* Input State Variables */
# define	PSAtCol1	0
# define	PSInCol1	1
# define	PSAtCol2	2
# define	PSInCol2	3

int IStyle_Chg=1;				/* Has input style changed? */
int lastleft=0;

char *ParStyle="";				/* Paragraph Style Name */
TStyle_typ TStyle;				/* Bitmask of Text Styles */
char *TFont="";				/* Points to Font String */
int TSize=0;					/* Pointsize of Font */
int inTable=0;			/* In a table? */
int cellno=0;
int firstcell=0;
int lastcell=0;
int celldidx;		/* index of the cell definition */
int ToCLev;					/* Table of Contents Level */
int CurFNote;


int destination=rtfMaxDestination;				/* The current input destination */
RTFFont		*fp;				/* Actual Font Pointer */
int	OptDestFound;			/* Set if an optional destination was encountered */

InStateStack  *ISS=NULL;


char PFileExt[16]="gif";	/* extension to be used for links 
							to pictures */
char *FPrefix;		/* Base name of output files */
char *FPrefixR;		/* Relative name of output files */

char *OutfileName;			/* output file name */
int linkself;			/* set for internal links */

char	*outMap[rtfSC_MaxChar];

char *filename;
static void ObjAttr ();


/* jrm21 - keep track of image IDs. For some reason, things go
   bad when extracting images if we pick up "blipuid" id fields,
   so we'll try the alternate "bliptag" tag id. */

struct ImageID {
  int ID;
  struct ImageID *next;
};
struct ImageID *imageID_head=NULL;



/*
 * Once-only writer initialization
 */

void
WriterInit ()
{
	FPrefix = RTFAlloc (256);
	FPrefixR = RTFAlloc (256);
	OutfileName = RTFAlloc (256);
	filename = RTFAlloc (256);
	if (FPrefix == NULL
		|| FPrefixR == NULL
		|| OutfileName == NULL
		|| filename == NULL)
		RTFPanic ("Cannot allocate filename buffers.");
	FPrefix[0] = '\0';
	FPrefixR[0] = '\0';
	OutfileName[0] = '\0';
	filename[0] = '\0';

	if (RTFReadOutputMap (outputMapName, outMap, 1) == 0)
		RTFPanic ("Cannot read output map %s", outputMapName);

	if (TransInit (TransFile) == 0)
		RTFPanic ("Cannot read Translation File %s", TransFile);
}





/*
 * Set up to process a single file
 */

int
do_main(argc, argv)
int	argc;
char	**argv;
{
	int i;
	char versid[256];


	debug=1;/*jrm21*/
		IMG=0;
		ToC=1;
		ToCLev=0;
		IStyle_Chg=1;
		ParStyle="";
		TFont="";
		WriteGraf=1;
		TSize=0;
		cellno=0;
		inTable=0;
		firstcell=0;
		lastcell=0;
		CurFNote=0;
		destination=rtfMaxDestination;
		ISS=NULL;
		strcpy(PFileExt,"gif");
		FPrefix[0]='\0';
		OptDestFound=0;

        --argc;
        ++argv;
       while (argc > 0 && **argv == '-') {
	    if (strcmp ("-V", *argv) == 0){
			sprintf(versid,"rtftohtml Version %s\n",PVers);
			RTFMsg(versid);
			return (0);
	    } else if (strcmp ("-i", *argv) == 0){
			IMG=1;
	    } else if (strcmp ("-G", *argv) == 0){
	    	WriteGraf=0;
	    } else if (strcmp ("-T", *argv) == 0){
	    	ToC=0;
	    } else if (strcmp ("-o", *argv) == 0){
                if (argc < 2) {
			return (-1);
                }
                --argc;
                ++argv;
                strcpy(OutfileName,*argv);
                /*
                    if the -o option was specified, use the file name
                    as a prefix (stripping off .html or .HTML if present.)
                */
                i=strlen(argv[0]);
                if(i>5&&(strcmp(&(argv[0][i-5]),".html")==0||
                    strcmp(&(argv[0][i-5]),".HTML")==0)){
                    i=i-5;
                }
                if(i>256)i=255;
                strncpy(FPrefix,argv[0],(size_t) i);
                FPrefix[i]=0;
	    } else if (strcmp ("-P", *argv) == 0){
			if (argc < 2) {
				return (-1);
			}
			--argc;
			++argv;
            strncpy(PFileExt,*argv,(size_t) 15);
            PFileExt[15] = '\0';
	    } else {
			RTFMsg ("Don't understand %s option.\n", *argv);
			return (-1);
	    }
	    --argc;
	    ++argv;
	}
	if (argc > 0)
	{
		if (freopen (argv[0], "r", stdin) == NULL)
		{
			RTFMsg ("Can't open \"%s\"\n", argv[0]);
			return (0);
		}

                /*
                    if the -o option was not specified, use the file name
                    as a prefix (stripping off .rtf or .RTF if present.)
                */  
                if(FPrefix[0]==0){
                    i=strlen(argv[0]);
                    if(i>4&&(strcmp(&(argv[0][i-4]),".rtf")==0||
                        strcmp(&(argv[0][i-4]),".RTF")==0)){
                        i=i-4;
                    }   
                    if(i>256)i=256;
                    strncpy(FPrefix,argv[0],(size_t) i);
                    FPrefix[i]=0;
                	sprintf(OutfileName,"%s.html",FPrefix);
                }   
	} else {
		RTFMsg("No input file given\n");
		return (0);
	}

	strcpy(FPrefixR, Basename (FPrefix));
	HTMLInit();
	
	/* install class callbacks */

	RTFSetClassCallback (rtfUnknown, UnknownClass);
	RTFSetClassCallback (rtfGroup, GroupClass);
	RTFSetClassCallback (rtfText, PutHTML);
	RTFSetClassCallback (rtfControl, ControlClass);

	/* de-install default destination callbacks */
	RTFSetDestinationCallback(rtfInfo, NULL);
	RTFSetDestinationCallback(rtfPict, NULL);
	RTFSetDestinationCallback(rtfObject, NULL);
	RTFSetDestinationCallback(rtfColorTbl, NULL);

        /*               
                Process the input stream.  Make sure the first intoken is
                a "{" so a state push will occur before anything else
                (need to preserve state 0 intact for section, paragraph,
                character default restoration).
        */               
                         
        (void) RTFGetToken ();
        if (!RTFCheckCM (rtfGroup, rtfBeginGroup))
        {                
                RTFMsg ("malformed rtf file - does not begin with \"{\"\n");
                return (0);
        }
        RTFRouteToken ();       /* send "{" through router */

	return (1);
}

int do_convert(const char* inputFile, const char* outputFile, int img, int writeGraf, int ToC)
{
	int i;
	char versid[256];


	debug=1;/*jrm21*/
	IMG=0;
	ToC=1;
	ToCLev=0;
	IStyle_Chg=1;
	ParStyle="";
	TFont="";
	WriteGraf=1;
	TSize=0;
	cellno=0;
	inTable=0;
	firstcell=0;
	lastcell=0;
	CurFNote=0;
	destination=rtfMaxDestination;
	ISS=NULL;
	strcpy(PFileExt,"gif");
	FPrefix[0]='\0';
	OptDestFound=0;
	
	if (freopen (inputFile, "r", stdin) == NULL)
		return (0);
	
    strcpy(OutfileName,outputFile);
	strcpy(FPrefixR, Basename (outputFile));
	HTMLInit();
	
	/* install class callbacks */
	
	RTFSetClassCallback (rtfUnknown, UnknownClass);
	RTFSetClassCallback (rtfGroup, GroupClass);
	RTFSetClassCallback (rtfText, PutHTML);
	RTFSetClassCallback (rtfControl, ControlClass);
	
	/* de-install default destination callbacks */
	RTFSetDestinationCallback(rtfInfo, NULL);
	RTFSetDestinationCallback(rtfPict, NULL);
	RTFSetDestinationCallback(rtfObject, NULL);
	RTFSetDestinationCallback(rtfColorTbl, NULL);
	
	/*               
	Process the input stream.  Make sure the first intoken is
	a "{" so a state push will occur before anything else
	(need to preserve state 0 intact for section, paragraph,
	character default restoration).
	*/               
	
	(void) RTFGetToken ();
	if (!RTFCheckCM (rtfGroup, rtfBeginGroup))
	{                
		RTFMsg ("malformed rtf file - does not begin with \"{\"\n");
		return (0);
	}
	RTFRouteToken ();       /* send "{" through router */

	return 1;
}
	

/*
	Token class callbacks
*/


static void UnknownClass ()
{
	/* use this to print warnings about unknown tokens */
	if(OptDestFound){
		/* Weed out optional destinations. No Warning given */
		RTFSkipGroup ();
		PopIState();
		return;
	}
	RTFMsg ("Unknown symbol \"%s\" near line %ld, position %d.\n",
				rtfTextBuf, rtfLineNum, rtfLinePos);
}


static void GroupClass ()
{
	IStyle_Chg=1;
	switch (rtfMajor)
	{
	case rtfBeginGroup:
		/* push state */
		PushIState();
		break;
	case rtfEndGroup:
		/* pop state */
		PopIState();
		break;
	}
}


/*
	Process control symbol.
*/

static void ControlClass ()
{
	switch (rtfMajor)
	{
	case rtfVersion:
		break;
	case rtfDefFont:
		/*
			rtfParam contains the default font number.  Beware
			that there is no guarantee that this font will actually
			be listed in the font table!
		*/
		break;
	case rtfCharSet:
	  /* CharSet (); -- done by reader.c */
		break;
	case rtfDestination:
		Destination ();
		break;
	case rtfFontFamily:
		/* only occurs within font table - can ignore */
		break;
	case rtfColorName:
		/* only occurs within color table - can ignore */
		break;
	case rtfSpecialChar:
		SpecialChar ();
		break;
	case rtfStyleAttr:
		/* only occurs within stylesheet - can ignore */
		break;
	case rtfDocAttr:
		DocAttr ();
		break;
	case rtfSectAttr:
		SectAttr ();
		break;
	case rtfTblAttr:
		TblAttr ();
		break;
	case rtfObjAttr:
		ObjAttr ();
		break;
	case rtfParAttr:
		ParAttr ();
		break;
	case rtfCharAttr:
		CharAttr ();
		break;
	case rtfPictAttr:
		PictAttr ();
		break;
	case rtfNeXTGrAttr:
		NeXTGrAttr ();
		break;
	case rtfFieldAttr:
		FieldAttr ();
		break;
	case rtfTOCAttr:
		TOCAttr ();
		break;
	case rtfPosAttr:
		PosAttr ();
		break;
	}
}


/*
	Control class major number handlers.  Each one switches on
	the minor numbers that occur within the major number.
*/


static void Destination ()
{
	destination=rtfMinor;
	OptDestFound=0;
	IStyle_Chg=1;
	switch (rtfMinor)
	{
	case rtfPict:
		break;
	case rtfNeXTGraphic:
		break;
	case rtfFootnote:
		firstcell=lastcell;
		break;
	case rtfHeader:
	case rtfHeaderLeft:
	case rtfHeaderRight:
	case rtfHeaderFirst:
	case rtfFooter:
	case rtfFooterLeft:
	case rtfFooterRight:
	case rtfFooterFirst:
		RTFSkipGroup ();
		PopIState();
		break;
	case rtfFNSep:
		RTFSkipGroup ();
		PopIState();
		break;
	case rtfFNContSep:
		RTFSkipGroup ();
		PopIState();
		break;
	case rtfFNContNotice:
		RTFSkipGroup ();
		PopIState();
		break;
	case rtfInfo:
		break;
	case rtfStyleSheet:
		/* will never occur because of default destination reader */
		break;
	case rtfFontTbl:
		/* will never occur because of default destination reader */
		break;
	case rtfColorTbl:
		RTFSkipGroup ();
		PopIState();
		break;
	case rtfField:
	case rtfFieldResult:
		break;
	case rtfFieldInst:
		RTFSkipGroup ();
		PopIState();
		break;
	case rtfIndex:
	case rtfIndexBold:
	case rtfIndexItalic:
	case rtfIndexText:
	case rtfIndexRange:
		break;
	case rtfTOC:
		break;
	case rtfBookmarkStart:
		break;
	case rtfBookmarkEnd:
		break;
	case rtfITitle:
		break;
	case rtfISubject:
	case rtfIAuthor:
	case rtfIOperator:
	case rtfIKeywords:
	case rtfIComment:
	case rtfIVerscomm:
	case rtfIDoccomm:
		RTFSkipGroup ();
		PopIState();
		break;
	case rtfObject:
		linkself=0;
		break;
	case rtfObjItem:
		break;
	case rtfObjData:
	case rtfObjClass:
	case rtfObjName:
	case rtfObjTopic:
		RTFSkipGroup ();
		PopIState();
		break;
	case rtfParNumText:
		break;
	case rtfObjResult:
		break;
	case rtfAnnotation:
		break;
	case rtfAnnotID:
		break;
	case rtfShapeGroup:
	  RTFSkipGroup();
	  break;
	default:
		RTFSkipGroup ();
		PopIState();
		break;		
	}
	HTMLStartDivert();
}


static void SpecialChar ()
{
	char fnote[8];
	switch (rtfMinor)
	{
	case rtfCurHeadPage:
		break;
	case rtfCurFNote:
		if(destination!=rtfFootnote)CurFNote++;
		sprintf(fnote,"%d",CurFNote);
		PutHTMLString(fnote);
		break;
	case rtfCurHeadPict:
		break;
	case rtfCurHeadDate:
		break;
	case rtfCurHeadTime:
		break;
	case rtfFormula:
		break;
	case rtfNoBrkSpace:
		rtfMinor = rtfSC_nobrkspace;
		PutHTML ();
		break;
	case rtfNoReqHyphen:
		break;
	case rtfNoBrkHyphen:
		rtfMinor = rtfSC_nobrkhyphen;
		PutHTML ();
	case rtfRow:
		if(lastcell==0){
			lastcell=celldidx;
			}
		cellno=firstcell;
		PutSpecial(rtfRow);
		break;
	case rtfSect:
		break;
	case rtfPage:
		break;
	case rtfLine:
		rtfMinor = rtfSC_newline;
		PutHTML ();
		break;
	case rtfPar:
		PutSpecial(rtfPar);
		break;
	case rtfBullet:
		rtfMinor = rtfSC_bullet;
		PutHTML ();
		break;
	case rtfCell:
		if(cellno++>=CELLMAX){
			RTFMsg ("Overflowed the total number of cells %s\n",CELLMAX);
			cellno--;
		}
		break;
	case rtfTab:
		PutSpecial(rtfTab);
		break;
/*
	case rtfCurAnnot:
		break;
*/
	case rtfCurAnnotRef:
		break;
	case rtfFNoteSep:
		break;
	case rtfFNoteCont:
		break;
	case rtfColumn:
		break;
	case rtfOptDest:
		OptDestFound=1;
		break;
	case rtfIIntVersion:
		break;
	case rtfICreateTime:
		break;
	case rtfIRevisionTime:
		break;
	case rtfIPrintTime:
		break;
	case rtfIBackupTime:
		break;
	case rtfIEditTime:
		break;
	case rtfIYear:
		break;
	case rtfIMonth:
		break;
	case rtfIDay:
		break;
	case rtfIHour:
		break;
	case rtfIMinute:
		break;
	case rtfINPages:
		break;
	case rtfINWords:
		break;
	case rtfINChars:
		break;
	case rtfIIntID:
		break;
	case rtfEmDash:
		rtfMinor = rtfSC_emdash;
		PutHTML ();
		break;
	case rtfEnDash:
		rtfMinor = rtfSC_endash;
		PutHTML ();
		break;
	case rtfLQuote:
		rtfMinor = rtfSC_quoteleft;
		PutHTML ();
		break;
	case rtfRQuote:
		rtfMinor = rtfSC_quoteright;
		PutHTML ();
		break;
	case rtfLDblQuote:
		rtfMinor = rtfSC_quotedblleft;
		PutHTML ();
		break;
	case rtfRDblQuote:
		rtfMinor = rtfSC_quotedblright;
		PutHTML ();
		break;
	}
}


static void DocAttr ()
{
	switch (rtfMinor)
	{
	case rtfPaperWidth:
		break;
	case rtfPaperHeight:
		break;
	case rtfLeftMargin:
		break;
	case rtfRightMargin:
		break;
	case rtfTopMargin:
		break;
	case rtfBottomMargin:
		break;
	case rtfFacingPage:
		break;
	case rtfGutterWid:
		break;
	case rtfDefTab:
		break;
	case rtfWidowCtrl:
		break;
	case rtfHyphHotZone:
		break;
	case rtfFNoteEndSect:
		break;
	case rtfFNoteEndDoc:
		break;
	case rtfFNoteText:
		break;
	case rtfFNoteBottom:
		break;
	case rtfFNoteStart:
		break;
	case rtfFNoteRestart:
		break;
	case rtfPageStart:
		break;
	case rtfLineStart:
		break;
	case rtfLandscape:
		break;
	case rtfFracWidth:
		break;
	case rtfNextFile:
		break;
	case rtfRTFDefault:
		break;
	case rtfRevisions:
		break;
	case rtfMirrorMargin:
		break;
	case rtfRevDisplay:
		break;
	case rtfRevBar:
		break;
	}
}


static void SectAttr ()
{
	switch (rtfMinor)
	{
	case rtfSectDef:
		break;
	case rtfNoBreak:
		break;
	case rtfColBreak:
		break;
	case rtfPageBreak:
		break;
	case rtfEvenBreak:
		break;
	case rtfOddBreak:
		break;
	case rtfPageStarts:
		break;
	case rtfPageCont:
		break;
	case rtfPageRestart:
		break;
	case rtfPageDecimal:
		break;
	case rtfPageURoman:
		break;
	case rtfPageLRoman:
		break;
	case rtfPageULetter:
		break;
	case rtfPageLLetter:
		break;
	case rtfPageNumRight:
		break;
	case rtfPageNumTop:
		break;
	case rtfHeaderY:
		break;
	case rtfFooterY:
		break;
	case rtfLineModulus:
		break;
	case rtfLineDist:
		break;
	case rtfLineStarts:
		break;
	case rtfLineRestart:
		break;
	case rtfLineRestartPg:
		break;
	case rtfLineCont:
		break;
	case rtfTopVAlign:
		break;
	case rtfBottomVAlign:
		break;
	case rtfCenterVAlign:
		break;
	case rtfJustVAlign:
		break;
	case rtfColumns:
		break;
	case rtfColumnSpace:
		break;
	case rtfColumnLine:
		break;
	case rtfENoteHere:
		break;
	case rtfTitleSpecial:
		break;
	}
}

static void ObjAttr ()
{
	switch (rtfMinor)
	{
	case rtfObjLinkSelf:
		linkself=1;
		break;
	}
}
static void TblAttr ()
{
	switch (rtfMinor)
	{
	case rtfCellBordBottom:
		break;
	case rtfCellBordTop:
		break;
	case rtfCellBordLeft:
		break;
	case rtfCellBordRight:
		break;
	case rtfRowDef:
		lastcell=0;
		cellno=firstcell;
		celldidx=firstcell;
		cell[celldidx].merge=0;
		cell[celldidx].just=AdjLPad;
		lastleft=0;
		break;
	case rtfRowLeft:
		break;
	case rtfRowRight:
		break;
	case rtfRowCenter:
		break;
	case rtfRowGapH:
		break;
	case rtfRowHt:
		break;
	case rtfRowLeftEdge:
		lastleft=rtfParam;
		break;
	case rtfCellPos:
		cell[celldidx].width=(rtfParam-lastleft)/TWIPSperCHAR;
		if(cell[celldidx].width<=1)cell[celldidx].width=2;
		lastleft=rtfParam;
		if(celldidx++>=CELLMAX){
			RTFMsg ("Overflowed the total number of cells %s\n",CELLMAX);
			celldidx--;
		}
		cell[celldidx].just=AdjLPad;
		cell[celldidx].merge=0;
		break;
	case rtfMergeRngFirst:
		break;
	case rtfMergePrevious:
		cell[celldidx].merge=1;
		break;
	}
}
int parnest=0;

static void ParAttr ()
{
	RTFStyle *newstyle;
	/* int i; */
	IStyle_Chg=1;
	switch (rtfMinor)
	{
	case rtfParDef:	/* set the input paragraph stype to null */
		ParStyle="";	
		TStyle = stylePlain;
		inTable=0;
		break;
	case rtfStyleNum:
		/* When we call RTFExpandStyle, ParAttr will get recursively
		called. We only perform processing on the outermost style. */
		if(parnest==0){
		    newstyle=RTFGetStyle(rtfParam);
		    if (newstyle == NULL)
				break;
			ParStyle=newstyle->rtfSName;
				parnest++;
				RTFExpandStyle(rtfParam);
				parnest--;
		}
		break;
	case rtfQuadLeft:
		cell[cellno].just=AdjLPad;
		break;
	case rtfQuadRight:
		cell[cellno].just=AdjRPad;
		break;
	case rtfQuadJust:
		cell[cellno].just=AdjLPad;
		break;
	case rtfQuadCenter:
		cell[cellno].just=AdjCpad;
		break;
	case rtfFirstIndent:
		break;
	case rtfLeftIndent:
		break;
	case rtfRightIndent:
		break;
	case rtfSpaceBefore:
		break;
	case rtfSpaceAfter:
		break;
	case rtfSpaceBetween:
		break;
	case rtfInTable:
		inTable=1;
		break;
	case rtfKeep:
		break;
	case rtfKeepNext:
		break;
	case rtfSideBySide:
		break;
	case rtfPBBefore:
		break;
	case rtfNoLineNum:
		break;
	case rtfTabPos:
		break;
	case rtfTabRight:
		break;
	case rtfTabCenter:
		break;
	case rtfTabDecimal:
		break;
	case rtfTabBar:
		break;
	case rtfBorderTop:
		break;
	case rtfBorderBottom:
		break;
	case rtfBorderLeft:
		break;
	case rtfBorderRight:
		break;
	case rtfBorderBar:
		break;
	case rtfBorderBox:
		break;
	case rtfBorderBetween:
		break;
	case rtfBorderSingle:
		break;
	case rtfBorderThick:
		break;
	case rtfBorderShadow:
		break;
	case rtfBorderDouble:
		break;
	case rtfBorderDot:
		break;
	case rtfBorderHair:
		break;
	case rtfBorderSpace:
		break;
	case rtfLeaderDot:
		break;
	case rtfLeaderHyphen:
		break;
	case rtfLeaderUnder:
		break;
	case rtfLeaderThick:
		break;
	}
}


/*
	Several of the attributes can be turned off with param value
	of zero (e.g., \b vs. \b0).
*/

static void CharAttr ()
{
RTFFont		*fp;
int	turnOn = (rtfParam != 0);
	IStyle_Chg=1;
	switch (rtfMinor)
	{
	case rtfPlain:
		TStyle = stylePlain;
		break;
	case rtfBold:
		if(turnOn){
		    TStyle |= styleBold;
		} else {
		    TStyle &= ~styleBold;
		}
		break;
	case rtfItalic:
		if(turnOn){
		    TStyle |= styleItalic;
		} else {
		    TStyle &= ~styleItalic;
		}
		break;
	case rtfStrikeThru:
		if(turnOn){
		    TStyle |= styleStrikeThru;
		} else {
		    TStyle &= ~styleStrikeThru;
		}
		break;
	case rtfOutline:
		if(turnOn){
		    TStyle |= styleOutline;
		} else {
		    TStyle &= ~styleOutline;
		}
		break;
	case rtfShadow:
		if(turnOn){
		    TStyle |= styleShadow;
		} else {
		    TStyle &= ~styleShadow;
		}
		break;
	case rtfSmallCaps:
		if(turnOn){
		    TStyle |= styleSmallCaps;
		} else {
		    TStyle &= ~styleSmallCaps;
		}
		break;
	case rtfAllCaps:
		if(turnOn){
		    TStyle |= styleAllCaps;
		} else {
		    TStyle &= ~styleAllCaps;
		}
		break;
	case rtfInvisible:
		if(turnOn){
		    TStyle |= styleInvisible;
		} else {
		    TStyle &= ~styleInvisible;
		}
		break;
	case rtfFontNum:
		if((fp = RTFGetFont(rtfParam)) == NULL){
			TFont="";
		} else {
			TFont=fp->rtfFName;
		}
		break;
	case rtfFontSize:
		TSize=rtfParam;
		break;
	case rtfExpand:
		break;
	case rtfUnderline:
		if(turnOn){
		    TStyle |= styleUnderline;
		} else {
		    TStyle &= ~styleUnderline;
		}
		break;
	case rtfWordUnderline:
		if(turnOn){
		    TStyle |= styleWUnderline;
		} else {
		    TStyle &= ~styleWUnderline;
		}
		break;
	case rtfDotUnderline:
		if(turnOn){
		    TStyle |= styleDUnderline;
		} else {
		    TStyle &= ~styleDUnderline;
		}
		break;
	case rtfDbUnderline:
		if(turnOn){
		    TStyle |= styleDbUnderline;
		} else {
		    TStyle &= ~styleDbUnderline;
		}
		break;
	case rtfNoUnderline:
		TStyle&=~(styleUnderline|styleWUnderline|
				styleDUnderline|styleDbUnderline);
		break;
	case rtfSuperScript:
		if(turnOn){
		    TStyle |= styleSuperScript;
		} else {
		    TStyle &= ~styleSuperScript;
		}
		break;
	case rtfSubScript:
		if(turnOn){
		    TStyle |= styleSubScript;
		} else {
		    TStyle &= ~styleSubScript;
		}
		break;
	case rtfRevised:
		break;
	case rtfForeColor:
		break;
	case rtfBackColor:
		break;
	}
}

static void PictAttr ()
{
  /* char *ptr,LinkBuf[512]; */
	switch (rtfMinor)
	{
	case rtfMacQD:
		PictExt="pict";
		PictType=FTPICT;
		RTFSetClassCallback (rtfText, NabPicture);
        break;
	case rtfWinMetafile:
		PictExt="wmf";
		PictType=FTWMF;
		RTFSetClassCallback (rtfText, NabPicture);
		break;
	case rtfWinBitmap:
		PictExt="bmp";
		PictType=FTBMF;
		RTFSetClassCallback (rtfText, NabPicture);
		break;
		/* jrm21 - added emf, pict & jpeg. We ignore word97 dests. */
	case rtfEMFPict:
	  PictExt="emf";
	  PictType=FTEMF;
	  RTFSetClassCallback (rtfText, NabPicture);
	  break;
	case rtfPNGPict:
	  PictExt="png";
	  PictType=FTPNG;
	  RTFSetClassCallback (rtfText, NabPicture);
	  break;
	case rtfJPEGPict:
	  PictExt="jpg";
	  PictType=FTJPEG;
	  RTFSetClassCallback (rtfText, NabPicture);
	  break;
	case rtfPicTag: {
	  /* add this tag to a global table. If it is already in there, then
	     skip this image. */
	  int tag;
	  struct ImageID* image;

	  tag=atoi(rtfTextBuf+8); /* 8 = strlen("\bliptag") */
	  image=imageID_head;
	  
	  while (image!=NULL) {
	    if (image->ID == tag) {
	      /* an image with this ID exists, so skip this one. */
	      RTFSkipGroup();
	      PopIState();
	      break;
	    }
	    image=image->next;
	  }

	  if (image==NULL) {
	    image=malloc(sizeof(struct ImageID));
	    if (image==NULL) {RTFPanic ("Memory Allocation Failed");}
	    image->ID=tag;
	    image->next=imageID_head;
	    imageID_head=image;
	  }

	  break; }
	case rtfPicWid:
		break;
	case rtfPicHt:
		break;
	case rtfPicGoalWid:
		PicGoalWid=rtfParam;
		break;
	case rtfPicGoalHt:
		PicGoalHt=rtfParam;
		break;
	case rtfPicScaleX:
		break;
	case rtfPicScaleY:
		break;
	case rtfPicScaled:
		break;
	case rtfPicCropTop:
		break;
	case rtfPicCropBottom:
		break;
	case rtfPicCropLeft:
		break;
	case rtfPicCropRight:
		break;
	case rtfPixelBits:
		break;
	case rtfBitmapPlanes:
		break;
	case rtfBitmapWid:
		break;
	case rtfPicBinary:
		RTFSkipGroup ();		/* Not sure this will work on binary input data */
		PopIState();
		break;
	}
}


static void NeXTGrAttr ()
{
	switch (rtfMinor)
	{
	case rtfNeXTGWidth:
		break;
	case rtfNeXTGHeight:
		break;
	}
}


static void FieldAttr ()
{
	switch (rtfMinor)
	{
	case rtfFieldDirty:
		break;
	case rtfFieldEdited:
		break;
	case rtfFieldLocked:
		break;
	case rtfFieldPrivate:
		break;
	}
}


static void TOCAttr ()
{
	switch (rtfMinor)
	{
	case rtfTOCType:
		break;
	case rtfTOCLevel:
		ToCLev=rtfParam;
		break;
	}
}


static void PosAttr ()
{
	switch (rtfMinor)
	{
	case rtfPosX:
		break;
	case rtfPosXCenter:
		break;
	case rtfPosXInside:
		break;
	case rtfPosXLeft:
		break;
	case rtfPosXOutSide:
		break;
	case rtfPosXRight:
		break;
	case rtfPosY:
		break;
	case rtfPosYInline:
		break;
	case rtfPosYTop:
		break;
	case rtfPosYCenter:
		break;
	case rtfPosYBottom:
		break;
	case rtfAbsWid:
		break;
/*
	case rtfTextDistX:
		break;
*/
	case rtfTextDistY:
		break;
	case rtfRPosMargV:
		break;
	case rtfRPosPageV:
		break;
	case rtfRPosMargH:
		break;
	case rtfRPosPageH:
		break;
	case rtfRPosColH:
		break;
	}
}

InStateStack *SaveIState(){
	InStateStack *NewState;
    NewState=(InStateStack *) malloc(
		(int) sizeof(InStateStack));
	    if(NewState==NULL){
		RTFPanic ("Memory Allocation Failed");
    }
    NewState->ParStyle=ParStyle;
    NewState->TStyle=TStyle;
    NewState->TFont=TFont;
    NewState->TSize=TSize;
    NewState->destination=destination;
    NewState->firstcell=firstcell;
    NewState->inTable=inTable;
    NewState->cellno=cellno;
    NewState->lastcell=lastcell;
    NewState->ToCLev=ToCLev;
    IStyle_Chg=1;    
    return(NewState);
}
void PushIState(){
/*
	Push the current state. Note that defaults are not set,
	so resetting the default state (such as for headers/footers)
	must be done after the Push.
*/
	InStateStack *Current=ISS;
	ISS=SaveIState();
    ISS->Next=Current;
}
void
RestoreIState(TheState)
InStateStack *TheState;
{
    ParStyle=TheState->ParStyle;
    TStyle=TheState->TStyle;
    TFont=TheState->TFont;
    TSize=TheState->TSize;
    ToCLev=TheState->ToCLev;
    if(destination==rtfFootnote){
    	/* Footnotes get their own tables */
    	firstcell=TheState->firstcell;
    	inTable=TheState->inTable;
    	cellno=TheState->cellno;
    	lastcell=TheState->lastcell;
    }
    destination=TheState->destination;
    RTFFree(TheState);
    IStyle_Chg=1;
}

void PopIState(){
	InStateStack *Next;
    if(ISS==NULL){
		RTFPanic ("Input State Stack Popped too many times");
    }
    HTMLEndDivert();
    Next=ISS->Next;
	RestoreIState(ISS);
    ISS=Next;
}
