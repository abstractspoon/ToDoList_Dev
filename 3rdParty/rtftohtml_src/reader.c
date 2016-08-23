/*
 * - Need to document error code meanings.
 * - Need to do something with \* on destinations.
 * - Make the parameter a long?
 *
 * reader.c - RTF file reader.  Release 1.11.
 *
 * ASCII 10 (\n) and 13 (\r) are ignored and silently discarded.
 * Nulls are also discarded.
 * (although the read hook will still get a look at them.)
 *
 * "\:" is not a ":", it's a control symbol.  But some versions of
 * Word seem to write "\:" for ":".  This reader treats "\:" as a
 * plain text ":"
 *
 * 19 Mar 93
 * - Add hack to skip "{\*\keycode ... }" group in stylesheet.
 * This is probably the wrong thing to do, but it's simple.
 * 13 Jul 93
 * - Add THINK C awareness to malloc() declaration.  Necessary so
 * compiler knows the malloc argument is 4 bytes.  Ugh.
 * 07 Sep 93
 * - Text characters are mapped onto standard codes, which are placed
 * in rtfMinor.
 * - Eliminated use of index() function.
 * 05 Mar 94
 * - Added zillions of new symbols (those defined in RTF spec 1.2).
 * 14 Mar 94
 * - Public functions RTFMsg() and RTFPanic() now take variable arguments.
 * This means RTFPanic() now is used in place of what was formerly the
 * internal function Error().
 * - 8-bit characters are now legal, so they're not converted to \'xx
 * hex char representation now.
 * 01 Apr 94
 * - Added public variables rtfLineNum and rtfLinePos.
 * - #include string.h or strings.h, avoiding strncmp() problem where
 * last argument is treated as zero when prototype isn't available.
 * 04 Apr 94
 * - Treat style numbers 222 and 0 properly as "no style" and "normal".
 * 08 Apr 94
 * - Control symbol table is no longer compiled in.  It's read in the
 * first time that RTFInit() is called from the library file rtf-ctrl.
 * This shrinks all the translator binaries.  rtf-ctrl is built by
 * mkrtf-ctrl.c.
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#ifdef STDARG
#include <stdarg.h>
#else
#ifdef	VARARGS
#include <varargs.h>
#endif	/* VARARGS */
#endif	/* STDARG */

# include	"tokenscan.h"
# include	"rtf-namedef.h"

# define	rtfInternal
# include	"rtf.h"
# undef		rtfInternal


/*
 * Return pointer to new element of type t, or NULL
 * if no memory available.
 */

# define	New(t)	((t *) RTFAlloc (sizeof (t)))


/* maximum number of character values representable in a byte */

# define	charSetSize		256

/* charset stack size */

# define	maxCSStack		40


static void	_RTFGetToken ();
 void	_RTFGetToken2 ();
static int	GetChar ();
static void	ReadFontTbl ();
static void	ReadColorTbl ();
static void	ReadStyleSheet ();
static void	ReadInfoGroup ();
static void	ReadPictGroup ();
static void	ReadObjGroup ();
static void	LookupInit ();
static void	Lookup ();
static int	Hash ();

static void	CharSetInit ();
static void	ReadCharSetMaps ();


/*
 * Public variables (listed in rtf.h)
 */

int	rtfClass;
int	rtfMajor;
int	rtfMinor;
int	rtfParam;
char	*rtfTextBuf = (char *) NULL;
int	rtfTextLen;

long	rtfLineNum;
int	rtfLinePos;


/*
 * Private stuff
 */

static int	pushedChar;	/* pushback char if read too far */

static int	pushedClass;	/* pushed token info for RTFUngetToken() */
static int	pushedMajor;
static int	pushedMinor;
static int	pushedParam;
static char	*pushedTextBuf = (char *) NULL;

static int	prevChar;
static int	bumpLine;


static RTFFont	*fontList = (RTFFont *) NULL;	/* these lists MUST be */
static RTFColor	*colorList = (RTFColor *) NULL;	/* initialized to NULL */
static RTFStyle	*styleList = (RTFStyle *) NULL;


static FILE	*rtffp /*= stdin*/;

static char	*inputName = (char *) NULL;
static char	*outputName = (char *) NULL;


/*
 * This array is used to map standard character names onto their numeric codes.
 * The position of the name within the array is the code.
 * stdcharnames.h is generated in the ../h directory.
 */

static char	*stdCharName[] =
{
# include	"stdcharnames.h"
	(char *) NULL
};


/*
 * These arrays are used to map RTF input character values onto the standard
 * character names represented by the values.  Input character values are
 * used as indices into the arrays to produce standard character codes.
 */


static char	*genCharSetFile = (char *) NULL;
static int	genCharCode[charSetSize];	/* general */
static int	haveGenCharSet = 0;

static char	*symCharSetFile = (char *) NULL;
static int	symCharCode[charSetSize];	/* symbol */
static int	haveSymCharSet = 0;

static int	curCharSet = rtfCSGeneral;
static int	*curCharCode = genCharCode;

/*
 * By default, the reader is configured to handle charset mapping invisibly,
 * including reading the charset files and switching charset maps as necessary
 * for Symbol font.
 */

static int	autoCharSetFlags;

/*
 * Stack for keeping track of charset map on group begin/end.  This is
 * necessary because group termination reverts the font to the previous
 * value, which may implicitly change it.
 */

static int	csStack[maxCSStack];
int	csTop = 0;

/*
 * Initialize the reader.  This may be called multiple times,
 * to read multiple files.  The only thing not reset is the input
 * stream; that must be done with RTFSetStream().
 */

void
RTFInit ()
{
int	i;
RTFColor	*cp;
RTFFont		*fp;
RTFStyle	*sp;
RTFStyleElt	*eltList, *ep;

rtffp=stdin;/* jrm21 */ 
	rtfClass = -1;
	pushedClass = -1;
	pushedChar = EOF;

	rtfLineNum = 0;
	rtfLinePos = 0;
	prevChar = EOF;
	bumpLine = 0;

	if (rtfTextBuf == (char *) NULL)	/* initialize text buffers */
	{
		rtfTextBuf = RTFAlloc (rtfBufSiz);
		pushedTextBuf = RTFAlloc (rtfBufSiz);
		if (rtfTextBuf == (char *) NULL
			|| pushedTextBuf == (char *) NULL)
			RTFPanic ("Cannot allocate text buffers.");
		rtfTextBuf[0] = pushedTextBuf[0] = '\0';
	}

	RTFFree (inputName);
	RTFFree (outputName);
	inputName = outputName = (char *) NULL;
			
	/* initialize control symbol lookup table */
	LookupInit ();
	
	for (i = 0; i < rtfMaxClass; i++)
		RTFSetClassCallback (i, (RTFFuncPtr) NULL);
	for (i = 0; i < rtfMaxDestination; i++)
		RTFSetDestinationCallback (i, (RTFFuncPtr) NULL);

	/* install built-in destination readers */
	RTFSetDestinationCallback (rtfFontTbl, ReadFontTbl);
	RTFSetDestinationCallback (rtfColorTbl, ReadColorTbl);
	RTFSetDestinationCallback (rtfStyleSheet, ReadStyleSheet);
	RTFSetDestinationCallback (rtfInfo, ReadInfoGroup);
	RTFSetDestinationCallback (rtfPict, ReadPictGroup);
	RTFSetDestinationCallback (rtfObject, ReadObjGroup);


	RTFSetReadHook ((RTFFuncPtr) NULL);

	/* dump old lists if necessary */

	while (fontList != (RTFFont *) NULL)
	{
		fp = fontList->rtfNextFont;
		RTFFree (fontList->rtfFName);
		RTFFree ((char *) fontList);
		fontList = fp;
	}
	while (colorList != (RTFColor *) NULL)
	{
		cp = colorList->rtfNextColor;
		RTFFree ((char *) colorList);
		colorList = cp;
	}
	while (styleList != (RTFStyle *) NULL)
	{
		sp = styleList->rtfNextStyle;
		eltList = styleList->rtfSSEList;
		while (eltList != (RTFStyleElt *) NULL)
		{
			ep = eltList->rtfNextSE;
			RTFFree (eltList->rtfSEText);
			RTFFree ((char *) eltList);
			eltList = ep;
		}
		RTFFree (styleList->rtfSName);
		RTFFree ((char *) styleList);
		styleList = sp;
	}

	CharSetInit ();
	csTop = 0;
}


/*
 * Set the reader's input stream to the given stream.  Can
 * be used to redirect to other than the default (stdin).
 */

void
RTFSetStream (FILE *stream)
{
	rtffp = stream;
}


/*
 * Set or get the input or output file name.  These are never guaranteed
 * to be accurate, only insofar as the calling program makes them so.
 */

void
RTFSetInputName (name)
char	*name;
{
	if ((inputName = RTFStrSave (name)) == (char *) NULL)
		RTFPanic ("RTFSetInputName: out of memory");
}


char *
RTFGetInputName ()
{
	return (inputName);
}


void
RTFSetOutputName (name)
char	*name;
{
	if ((outputName = RTFStrSave (name)) == (char *) NULL)
		RTFPanic ("RTFSetOutputName: out of memory");
}


char *
RTFGetOutputName ()
{
	return (outputName);
}



/* ---------------------------------------------------------------------- */

/*
 * Callback table manipulation routines
 */


/*
 * Install or return a writer callback for a token class
 */


static RTFFuncPtr	ccb[rtfMaxClass];		/* class callbacks */


void
RTFSetClassCallback (class, callback)
int		class;
RTFFuncPtr	callback;
{
	if (class >= 0 && class < rtfMaxClass)
		ccb[class] = callback;
}


RTFFuncPtr
RTFGetClassCallback (class)
int	class;
{
	if (class >= 0 && class < rtfMaxClass)
		return (ccb[class]);
	return ((RTFFuncPtr) NULL);
}


/*
 * Install or return a writer callback for a destination type
 */

static RTFFuncPtr	dcb[rtfMaxDestination];	/* destination callbacks */


void
RTFSetDestinationCallback (dest, callback)
int		dest;
RTFFuncPtr	callback;
{
	if (dest >= 0 && dest < rtfMaxDestination)
		dcb[dest] = callback;
}


RTFFuncPtr
RTFGetDestinationCallback (dest)
int	dest;
{
	if (dest >= 0 && dest < rtfMaxDestination)
		return (dcb[dest]);
	return ((RTFFuncPtr) NULL);
}


/* ---------------------------------------------------------------------- */

/*
 * Token reading routines
 */


/*
 * Read the input stream, invoking the writer's callbacks
 * where appropriate.
 */

void
RTFRead ()
{
	while (RTFGetToken () != rtfEOF)
		RTFRouteToken ();
}


/*
 * Route a token.  If it's a destination for which a reader is
 * installed, process the destination internally, otherwise
 * pass the token to the writer's class callback.
 */

void
RTFRouteToken ()
{
RTFFuncPtr	p;

	if (rtfClass < 0 || rtfClass >= rtfMaxClass)	/* watchdog */
	{
		RTFPanic ("Unknown class %d: %s (reader malfunction)",
							rtfClass, rtfTextBuf);
	}
	if (RTFCheckCM (rtfControl, rtfDestination))
	{
		/* invoke destination-specific callback if there is one */
		if ((p = RTFGetDestinationCallback (rtfMinor))
							!= (RTFFuncPtr) NULL)
		{
			(*p) ();
			return;
		}
	}
	/* invoke class callback if there is one */
	if ((p = RTFGetClassCallback (rtfClass)) != (RTFFuncPtr) NULL)
		(*p) ();
}


/*
 * Skip to the end of the current group.  When this returns,
 * writers that maintain a state stack may want to call their
 * state unstacker; global vars will still be set to the group's
 * closing brace.
 */

void
RTFSkipGroup ()
{
int	level = 1;
int savecsTop;
	if(csTop>0)savecsTop=csTop-1;
	else savecsTop=0;

	RTFGetToken ();
	while (1)
	{
		if (rtfClass == rtfEOF)
			break;
		if (rtfClass == rtfGroup)
		{
			if (rtfMajor == rtfBeginGroup)
				++level;
			else if (rtfMajor == rtfEndGroup)
			{
				if (--level < 1)
					break;	/* end of initial group */
			}
		}
		_RTFGetToken2();
	}
	csTop=savecsTop;
}


/*
 * Read one token.  Call the read hook if there is one.  The
 * token class is the return value.  Returns rtfEOF when there
 * are no more tokens.
 */

int
RTFGetToken ()
{
RTFFuncPtr	p;

	for (;;)
	{
		_RTFGetToken ();
		if ((p = RTFGetReadHook ()) != (RTFFuncPtr) NULL)
			(*p) ();	/* give read hook a look at token */

		/* Silently discard newlines, carriage returns, nulls.  */
		if (!(rtfClass == rtfText
			&& (rtfMajor == '\n' || rtfMajor == '\r'
						|| rtfMajor == '\0')))
			break;
	}
	return (rtfClass);
}


/*
 * Install or return a token reader hook.
 */

static RTFFuncPtr	readHook;


void RTFSetReadHook (RTFFuncPtr	f)
{
	readHook = f;
}


RTFFuncPtr RTFGetReadHook ()
{
	return (readHook);
}


void RTFUngetToken ()
{
	if (pushedClass >= 0)	/* there's already an ungotten token */
		RTFPanic ("cannot unget two tokens");
	if (rtfClass < 0)
		RTFPanic ("no token to unget");
	pushedClass = rtfClass;
	pushedMajor = rtfMajor;
	pushedMinor = rtfMinor;
	pushedParam = rtfParam;
	(void) strcpy (pushedTextBuf, rtfTextBuf);
}


int RTFPeekToken ()
{
	_RTFGetToken ();
	RTFUngetToken ();
	return (rtfClass);
}


static void _RTFGetToken ()
{
RTFFont	*fp;

	/* first check for pushed token from RTFUngetToken() */

	if (pushedClass >= 0)
	{
		rtfClass = pushedClass;
		rtfMajor = pushedMajor;
		rtfMinor = pushedMinor;
		rtfParam = pushedParam;
		(void) strcpy (rtfTextBuf, pushedTextBuf);
		rtfTextLen = strlen (rtfTextBuf);
		pushedClass = -1;
		return;
	}

	/*
	 * Beyond this point, no token is ever seen twice, which is
	 * important, e.g., for making sure no "}" pops the font stack twice.
	 */

	_RTFGetToken2 ();
	if (rtfClass == rtfText)	/* map RTF char to standard code */
		rtfMinor = RTFMapChar (rtfMajor);

	/*
	 * If auto-charset stuff is activated, see if anything needs doing,
	 * like reading the charset maps or switching between them.
	 */

	if (autoCharSetFlags == 0)
		return;

	if ((autoCharSetFlags & rtfReadCharSet)
		&& RTFCheckCM (rtfControl, rtfCharSet))
	{
		ReadCharSetMaps ();
	}
	else if ((autoCharSetFlags & rtfSwitchCharSet)
		&& RTFCheckCMM (rtfControl, rtfCharAttr, rtfFontNum))
	{
		if ((fp = RTFGetFont (rtfParam)) != (RTFFont *) NULL)
		{
			if (fp->rtfFName!=NULL&&strncmp (fp->rtfFName, "Symbol", 6) == 0)
				curCharSet = rtfCSSymbol;
			else
				curCharSet = rtfCSGeneral;
			RTFSetCharSet (curCharSet);
		}
	}
	else if ((autoCharSetFlags & rtfSwitchCharSet) && rtfClass == rtfGroup)
	{
		switch (rtfMajor)
		{
		case rtfBeginGroup:
			if (csTop >= maxCSStack)
				RTFPanic ("_RTFGetToken: stack overflow");
			csStack[csTop++] = curCharSet;
			break;
		case rtfEndGroup:
			if (csTop <= 0)
				RTFPanic ("_RTFGetToken: stack underflow");
			curCharSet = csStack[--csTop];
			RTFSetCharSet (curCharSet);
			break;
		}
	}
}


/* this shouldn't be called anywhere but from _RTFGetToken() */

void
_RTFGetToken2 ()
{
int	sign;
int	c;

	/* initialize token vars */

	rtfClass = rtfUnknown;
	rtfParam = rtfNoParam;
	rtfTextBuf[rtfTextLen = 0] = '\0';

	/* get first character, which may be a pushback from previous token */

	if (pushedChar != EOF)
	{
		c = pushedChar;
		rtfTextBuf[rtfTextLen++] = c;
		rtfTextBuf[rtfTextLen] = '\0';
		pushedChar = EOF;
	}
	else if ((c = GetChar ()) == EOF)
	{
		rtfClass = rtfEOF;
		return;
	}

	if (c == '{')
	{
		rtfClass = rtfGroup;
		rtfMajor = rtfBeginGroup;
		return;
	}
	if (c == '}')
	{
		rtfClass = rtfGroup;
		rtfMajor = rtfEndGroup;
		return;
	}
	if (c != '\\')
	{
		/*
		 * Two possibilities here:
		 * 1) ASCII 9, effectively like \tab control symbol
		 * 2) literal text char
		 */
		if (c == '\t')			/* ASCII 9 */
		{
			rtfClass = rtfControl;
			rtfMajor = rtfSpecialChar;
			rtfMinor = rtfTab;
		}
		else
		{
			rtfClass = rtfText;
			rtfMajor = c;
		}
		return;
	}
	if ((c = GetChar ()) == EOF)
	{
		/* early eof, whoops (class is rtfUnknown) */
		return;
	}
	if (!isalpha (c))
	{
		/*
		 * Three possibilities here:
		 * 1) hex encoded text char, e.g., \'d5, \'d3
		 * 2) special escaped text char, e.g., \{, \}
		 * 3) control symbol, e.g., \_, \-, \|, \<10>
		 */
		if (c == '\'')				/* hex char */
		{
		int	c2;

			if ((c = GetChar ()) != EOF && (c2 = GetChar ()) != EOF)
			{
				/* should do isxdigit check! */
				rtfClass = rtfText;
				rtfMajor = RTFCharToHex (c) * 16
						+ RTFCharToHex (c2);
				return;
			}
			/* early eof, whoops (class is rtfUnknown) */
			return;
		}

		/* escaped char */
		/*if (index (":{}\\", c) != (char *) NULL) *//* escaped char */
		if (c == ':' || c == '{' || c == '}' || c == '\\')
		{
			rtfClass = rtfText;
			rtfMajor = c;
			return;
		} 

		/* control symbol */
		Lookup (rtfTextBuf);	/* sets class, major, minor */
		return;
	}
	/* control word */
	while (isalpha (c))
	{
		if ((c = GetChar ()) == EOF)
			break;
	}

	/*
	 * At this point, the control word is all collected, so the
	 * major/minor numbers are determined before the parameter
	 * (if any) is scanned.  There will be one too many characters
	 * in the buffer, though, so fix up before and restore after
	 * looking up.
	 */

	if (c != EOF)
		rtfTextBuf[rtfTextLen-1] = '\0';
	Lookup (rtfTextBuf);	/* sets class, major, minor */
	if (c != EOF)
		rtfTextBuf[rtfTextLen-1] = c;

	/*
	 * Should be looking at first digit of parameter if there
	 * is one, unless it's negative.  In that case, next char
	 * is '-', so need to gobble next char, and remember sign.
	 */

	sign = 1;
	if (c == '-')
	{
		sign = -1;
		c = GetChar ();
	}
	if (c != EOF && isdigit (c))
	{
		rtfParam = 0;
		while (isdigit (c))	/* gobble parameter */
		{
			rtfParam = rtfParam * 10 + c - '0';
			if ((c = GetChar ()) == EOF)
				break;
		}
		rtfParam *= sign;
	}
	/*
	 * If control symbol delimiter was a blank, gobble it.
	 * Otherwise the character is first char of next token, so
	 * push it back for next call.  In either case, delete the
	 * delimiter from the token buffer.
	 */
	if (c != EOF)
	{
		if (c != ' ')
			pushedChar = c;
		rtfTextBuf[--rtfTextLen] = '\0';
	}
}


/*
 * Read the next character from the input.  This handles setting the
 * current line and position-within-line variables.  Those variable are
 * set correctly whether lines end with CR, LF, or CRLF (the last being
 * the tricky case).
 *
 * bumpLine indicates whether the line number should be incremented on
 * the *next* input character.
 */

static int
GetChar ()
{
int	c;
int	oldBumpLine;

	if ((c = getc (rtffp)) != EOF)
	{
		rtfTextBuf[rtfTextLen++] = c;
		rtfTextBuf[rtfTextLen] = '\0';
	}
	if (prevChar == EOF)
		bumpLine = 1;
	oldBumpLine = bumpLine;	/* non-zero if prev char was line ending */
	bumpLine = 0;
	if (c == '\r')
		bumpLine = 1;
	else if (c == '\n')
	{
		bumpLine = 1;
		if (prevChar == '\r')		/* oops, previous \r wasn't */
			oldBumpLine = 0;	/* really a line ending */
	}
	++rtfLinePos;
	if (oldBumpLine)	/* were we supposed to increment the */
	{			/* line count on this char? */
		++rtfLineNum;
		rtfLinePos = 1;
	}
	prevChar = c;
	return (c);
}


/*
 * Synthesize a token by setting the global variables to the
 * values supplied.  Typically this is followed with a call
 * to RTFRouteToken().
 *
 * If a param value other than rtfNoParam is passed, it becomes
 * part of the token text.
 */

void
RTFSetToken (class, major, minor, param, text)
int	class, major, minor, param;
char	*text;
{
	rtfClass = class;
	rtfMajor = major;
	rtfMinor = minor;
	rtfParam = param;
	if (param == rtfNoParam)
		(void) strcpy (rtfTextBuf, text);
	else
		sprintf (rtfTextBuf, "%s%d", text, param);
	rtfTextLen = strlen (rtfTextBuf);
}


/* ---------------------------------------------------------------------- */

/*
 * Routines to handle mapping of RTF character sets
 * onto standard characters.
 *
 * RTFStdCharCode(name)	given char name, produce numeric code
 * RTFStdCharName(code)	given char code, return name
 * RTFMapChar(c)	map input (RTF) char code to std code
 * RTFSetCharSet(id)	select given charset map
 * RTFGetCharSet()	get current charset map
 *
 * See ../h/README for more information about charset names and codes.
 */


/*
 * Initialize charset stuff.
 */

static void CharSetInit ()
{
	autoCharSetFlags = (rtfReadCharSet | rtfSwitchCharSet);
	RTFFree (genCharSetFile);
	genCharSetFile = (char *) NULL;
	haveGenCharSet = 0;
	RTFFree (symCharSetFile);
	symCharSetFile = (char *) NULL;
	haveSymCharSet = 0;
	curCharSet = rtfCSGeneral;
	curCharCode = genCharCode;
}


/*
 * Specify the name of a file to be read when auto-charset-file reading is
 * done.
 */

void
RTFSetCharSetMap (name, csId)
char	*name;
int	csId;
{
	if ((name = RTFStrSave (name)) == (char *) NULL)	/* make copy */
		RTFPanic ("RTFSetCharSetMap: out of memory");
	switch (csId)
	{
	case rtfCSGeneral:
		RTFFree (genCharSetFile);	/* free any previous value */
		genCharSetFile = name;
		break;
	case rtfCSSymbol:
		RTFFree (symCharSetFile);	/* free any previous value */
		symCharSetFile = name;
		break;
	}
}


/*
 * Do auto-charset-file reading.
 */

static void ReadCharSetMaps ()
{
char	buf[rtfBufSiz];

	if (genCharSetFile != NULL)
		(void) strcpy (buf, genCharSetFile);
	else
		sprintf (buf, "%s.gen", &rtfTextBuf[1]);

	/* jrm21 !!!! */
	if (strncmp(buf,"ansi",4)==0 && strncmp(buf,"ansi.",5)) {
	  /* chop of first 4 letters, due to requirements of stupid
	     filesystems that don't like > 8.3 names.
	     Ie ansicp1252 (iso-8859-15 W. Europe) -> cp1252 */
	  if (RTFReadCharSetMap (buf+4, rtfCSGeneral) == 0)
	    RTFPanic ("ReadCharSetMaps: Cannot read charset map %s", buf);
	  if (symCharSetFile != NULL)
	    (void) strcpy (buf, symCharSetFile);
	  else
	    sprintf (buf, "%s.sym", &rtfTextBuf[1]);
	  if (RTFReadCharSetMap (buf+4, rtfCSSymbol) == 0)
	    RTFPanic ("ReadCharSetMaps: Cannot read charset map %s", buf);
	  return; /* success */
	}

	if (RTFReadCharSetMap (buf, rtfCSGeneral) == 0)
	  RTFPanic ("ReadCharSetMaps: Cannot read charset map %s", buf);
	if (symCharSetFile != NULL)
	  strcpy (buf, symCharSetFile);
	else
	  sprintf (buf, "%s.sym", &rtfTextBuf[1]);
	if (RTFReadCharSetMap (buf, rtfCSSymbol) == 0)
	  RTFPanic ("ReadCharSetMaps: Cannot read charset map %s", buf);
}



/*
 * Read in a file describing an RTF character set map.  Lines consist of pairs
 * associating character names with character values.
 *
 * If the filename is an absolute pathname, look in the specified location
 * only.  Otherwise try to find the file in the current directory or library.
 */

int
RTFReadCharSetMap (file, csId)
char	*file;
int	csId;
{
FILE	*f;
char	buf[rtfBufSiz];
char	*name, *p;
int	*stdCodeArray;
int	stdCode;
int	radix;
int	value;
int	i;
TSScanner	scanner;
char		*scanEscape;
char	*fn = "RTFReadCharSetMap";

	switch (csId)
	{
	default:
		return (0);	/* illegal charset id */
	case rtfCSGeneral:
		stdCodeArray = genCharCode;
		break;
	case rtfCSSymbol:
		stdCodeArray = symCharCode;
		break;
	}

/*
	if ((f = RTFOpenLibFile (file, "r")) == (FILE *) NULL)
		return (0);
*/

	if ((f = fopen (file, "r")) == (FILE *) NULL)
		return (0);


	/* clobber current mapping */

	for (i = 0; i < charSetSize; i++)
	{
		stdCodeArray[i] = rtfSC_nothing;
	}

	/*
	 * Turn off scanner's backslash escape mechanism while reading
	 * charset file.  Restore it later.
	 */
	TSGetScanner (&scanner);
	scanEscape = scanner.scanEscape;
	scanner.scanEscape = "";
	TSSetScanner (&scanner);

	/* read file */

	while (fgets (buf, (int) sizeof (buf), f) != (char *) NULL)
	{
		if(buf[0] == '#')	/* skip comment lines */
			continue;
		TSScanInit (buf);
		if ((name = TSScan ()) == (char *) NULL)
			continue;	/* skip blank lines */
		if ((stdCode = RTFStdCharCode (name)) < 0)
		{
			//RTFPanic ("%s: unknown character name: %s", fn, name);
			continue;
		}
		if ((p = TSScan ()) == (char *) NULL)
		{
			RTFPanic ("%s: malformed charset map line for character %s",
								fn, name);
			continue;
		}
		if (p[1] == '\0')	/* single char - use ascii value */
			value = p[0];
		else
		{
			radix = 10;
			if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X'))
			{
				radix = 16;
				p += 2;
			}
			value = 0;
			while (*p != '\0')
				value = value * radix + RTFCharToHex(*p++);
		}
		if (value >= charSetSize)
		{
			RTFMsg ("%s: character value %d for %s too high\n",
							fn, value, name);
			RTFPanic ("maximum value is %d", charSetSize - 1);
		}
		stdCodeArray[value] = stdCode;
	}
	scanner.scanEscape = scanEscape;
	TSSetScanner (&scanner);
	fclose(f);

	switch (csId)
	{
	case rtfCSGeneral:
		haveGenCharSet = 1;
		break;
	case rtfCSSymbol:
		haveSymCharSet = 1;
		break;
	}

	return (1);
}


/*
 * Given a standard character name (a string), find its code (a number).
 * Return -1 if name is unknown.
 */

int RTFStdCharCode (char *name)
{
int	i;

	for (i = 0; i < rtfSC_MaxChar; i++)
	{
		if (strcmp (name, stdCharName[i]) == 0)
			return (i);
	}
	return (-1);
}


/*
 * Given a standard character code (a number), find its name (a string).
 * Return NULL if code is unknown.
 */

char *RTFStdCharName (int code)
{
	if (code < 0 || code >= rtfSC_MaxChar)
		return ((char *) NULL);
	return (stdCharName[code]);
}


/*
 * Given an RTF input character code, find standard character code.
 * The translator should read the appropriate charset maps when it finds a
 * charset control.  However, the file might not contain one.  In this
 * case, no map will be available.  When the first attempt is made to
 * map a character under these circumstances, RTFMapChar() assumes ANSI
 * and reads the map as necessary.
 */

int RTFMapChar (int c)
{
	switch (curCharSet)
	{
	case rtfCSGeneral:
		if (!haveGenCharSet)
		{
			if (RTFReadCharSetMap ("ansi.gen", rtfCSGeneral) == 0)
				RTFPanic ("RTFMapChar: cannot read ansi.gen");
		}
		break;
	case rtfCSSymbol:
		if (!haveSymCharSet)
		{
			if (RTFReadCharSetMap ("ansi.sym", rtfCSSymbol) == 0)
				RTFPanic ("RTFMapChar: cannot read ansi.sym");
		}
		break;
	}
	if (c < 0 || c >= charSetSize)
		return (rtfSC_nothing);
	return (curCharCode[c]);
}


/*
 * Set the current character set.  If csId is illegal, uses general charset.
 */

void
RTFSetCharSet (csId)
int	csId;
{
	switch (csId)
	{
	default:		/* use general if csId unknown */
	case rtfCSGeneral:
		curCharCode = genCharCode;
		curCharSet = csId;
		break;
	case rtfCSSymbol:
		curCharCode = symCharCode;
		curCharSet = csId;
		break;
	}
}


int
RTFGetCharSet ()
{
	return (curCharSet);
}


/* ---------------------------------------------------------------------- */

/*
 * Special destination readers.  They gobble the destination so the
 * writer doesn't have to deal with them.  That's wrong for any
 * translator that wants to process any of these itself.  In that
 * case, these readers should be overridden by installing a different
 * destination callback.
 *
 * NOTE: The last token read by each of these reader will be the
 * destination's terminating '}', which will then be the current token.
 * That '}' token is passed to RTFRouteToken() - the writer has already
 * seen the '{' that began the destination group, and may have pushed a
 * state; it also needs to know at the end of the group that a state
 * should be popped.
 *
 * It's important that rtf.h and the control token lookup table list
 * as many symbols as possible, because these destination readers
 * unfortunately make strict assumptions about the input they expect,
 * and a token of class rtfUnknown will throw them off easily.
 */


/*
 * Read { \fonttbl ... } destination.  Old font tables don't have
 * braces around each table entry; try to adjust for that.
 */

static void ReadFontTbl ()
{
RTFFont	*fp=NULL;
char	buf[rtfBufSiz], *bp;
int	old = -1;
char	*fn = "ReadFontTbl";

	for (;;)
	{
		RTFGetToken ();
		if (RTFCheckCM (rtfGroup, rtfEndGroup))
			break;
		if (old < 0)		/* first entry - determine tbl type */
		{
			if (RTFCheckCMM (rtfControl, rtfCharAttr, rtfFontNum))
				old = 1;	/* no brace */
			else if (RTFCheckCM (rtfGroup, rtfBeginGroup))
				old = 0;	/* brace */
			else			/* can't tell! */
			  /* jrm21: some rtfs have space between the entries,
			     so continue instead of panicking */
			/* RTFPanic ("%s: Cannot determine format", fn); */
			  continue;
		}
		if (old == 0)		/* need to find "{" here */
		{
			if (!RTFCheckCM (rtfGroup, rtfBeginGroup))
			  /* jrm21: some rtfs have space between the entries,
			     so continue instead of panicking */
				/* RTFPanic ("%s: missing \"{\"", fn); */
			  continue;
			RTFGetToken ();	/* yes, skip to next token */
		}
		if ((fp = New (RTFFont)) == NULL)
			RTFPanic ("%s: cannot allocate font entry", fn);

		fp->rtfNextFont = fontList;
		fontList = fp;

		fp->rtfFName = (char *) NULL;
		fp->rtfFAltName = (char *) NULL;
		fp->rtfFNum = -1;
		fp->rtfFFamily = 0;
		fp->rtfFCharSet = 0;
		fp->rtfFPitch = 0;
		fp->rtfFType = 0;
		fp->rtfFCodePage = 0;

		while (rtfClass != rtfEOF && !RTFCheckCM (rtfText, ';'))
		{
			if (rtfClass == rtfControl)
			{
				switch (rtfMajor)
				{
				default:
					/* ignore token but announce it */
					RTFMsg ("%s: unknown token \"%s\"\n",
							fn, rtfTextBuf);
				case rtfFontFamily:
					fp->rtfFFamily = rtfMinor;
					break;
				case rtfCharAttr:
					switch (rtfMinor)
					{
					default:
						break;	/* ignore unknown? */
					case rtfFontNum:
						fp->rtfFNum = rtfParam;
						break;
					}
					break;
				case rtfFontAttr:
					switch (rtfMinor)
					{
					default:
						break;	/* ignore unknown? */
					case rtfFontCharSet:
						fp->rtfFCharSet = rtfParam;
						break;
					case rtfFontPitch:
						fp->rtfFPitch = rtfParam;
						break;
					case rtfFontCodePage:
						fp->rtfFCodePage = rtfParam;
						break;
					case rtfFTypeNil:
					case rtfFTypeTrueType:
						fp->rtfFType = rtfParam;
						break;
					}
					break;
				}
			}
			else if (RTFCheckCM (rtfGroup, rtfBeginGroup))	/* dest */
			{
				RTFSkipGroup ();	/* ignore for now */
			}
			else if (rtfClass == rtfText)	/* font name */
			{
				bp = buf;
				while (rtfClass != rtfEOF
					&& !RTFCheckCM (rtfText, ';'))
				{
					*bp++ = rtfMajor;
					(void) RTFGetToken ();
				}
				*bp = '\0';
				fp->rtfFName = RTFStrSave (buf);
				if (fp->rtfFName == (char *) NULL)
					RTFPanic ("%s: cannot allocate font name", fn);
				/* already have next token; don't read one */
				/* at bottom of loop */
				continue;
			}
			else
			{
				/* ignore token but announce it */
				RTFMsg ("%s: unknown token \"%s\"\n",
							fn, rtfTextBuf);
			}
			(void) RTFGetToken ();
		}
		if (old == 0)	/* need to see "}" here */
		{
			(void) RTFGetToken ();
			if (!RTFCheckCM (rtfGroup, rtfEndGroup))
				RTFPanic ("%s: missing \"}\"", fn);
		}
	}
	if (fp->rtfFNum == -1)
		RTFPanic ("%s: missing font number", fn);
/*
 * Could check other pieces of structure here, too, I suppose.
 */
	RTFRouteToken ();	/* feed "}" back to router */
}


/*
 * The color table entries have color values of -1 if
 * the default color should be used for the entry (only
 * a semi-colon is given in the definition, no color values).
 * There will be a problem if a partial entry (1 or 2 but
 * not 3 color values) is given.  The possibility is ignored
 * here.
 */

static void
ReadColorTbl ()
{
RTFColor	*cp;
int		cnum = 0;
char		*fn = "ReadColorTbl";

	for (;;)
	{
		(void) RTFGetToken ();
		if (RTFCheckCM (rtfGroup, rtfEndGroup))
			break;
		if ((cp = New (RTFColor)) == (RTFColor *) NULL)
			RTFPanic ("%s: cannot allocate color entry", fn);
		cp->rtfCNum = cnum++;
		cp->rtfCRed = cp->rtfCGreen = cp->rtfCBlue = -1;
		cp->rtfNextColor = colorList;
		colorList = cp;
		while (RTFCheckCM (rtfControl, rtfColorName))
		{
			switch (rtfMinor)
			{
			case rtfRed:	cp->rtfCRed = rtfParam; break;
			case rtfGreen:	cp->rtfCGreen = rtfParam; break;
			case rtfBlue:	cp->rtfCBlue = rtfParam; break;
			}
			RTFGetToken ();
		}
		if (!RTFCheckCM (rtfText, (int) ';'))
			RTFPanic ("%s: malformed entry", fn);
	}
	RTFRouteToken ();	/* feed "}" back to router */
}


/*
 * The "Normal" style definition doesn't contain any style number,
 * all others do.  Normal style is given style rtfNormalStyleNum.
 */

static void
ReadStyleSheet ()
{
RTFStyle	*sp;
RTFStyleElt	*sep, *sepLast;
char		buf[rtfBufSiz], *bp;
char		*fn = "ReadStyleSheet";

	for (;;)
	{
		(void) RTFGetToken ();
		if (RTFCheckCM (rtfGroup, rtfEndGroup))
			break;
		if ((sp = New (RTFStyle)) == (RTFStyle *) NULL)
			RTFPanic ("%s: cannot allocate stylesheet entry", fn);
		sp->rtfSName = (char *) NULL;
		sp->rtfSNum = -1;
		sp->rtfSType = rtfParStyle;
		sp->rtfSAdditive = 0;
		sp->rtfSBasedOn = rtfNoStyleNum;
		sp->rtfSNextPar = -1;
		sp->rtfSSEList = sepLast = (RTFStyleElt *) NULL;
		sp->rtfNextStyle = styleList;
		sp->rtfExpanding = 0;
		styleList = sp;
		if (!RTFCheckCM (rtfGroup, rtfBeginGroup))
			RTFPanic ("%s: missing \"{\"", fn);
		for (;;)
		{
			(void) RTFGetToken ();
			if (rtfClass == rtfEOF
				|| RTFCheckCM (rtfText, ';'))
				break;
			if (rtfClass == rtfControl)
			{
				if (RTFCheckMM (rtfSpecialChar, rtfOptDest))
					continue;	/* ignore "\*" */
				if (RTFCheckMM (rtfParAttr, rtfStyleNum))
				{
					sp->rtfSNum = rtfParam;
					sp->rtfSType = rtfParStyle;
					continue;
				}
				if (RTFCheckMM (rtfCharAttr, rtfCharStyleNum))
				{
					sp->rtfSNum = rtfParam;
					sp->rtfSType = rtfCharStyle;
					continue;
				}
				if (RTFCheckMM (rtfSectAttr, rtfSectStyleNum))
				{
					sp->rtfSNum = rtfParam;
					sp->rtfSType = rtfSectStyle;
					continue;
				}
				if (RTFCheckMM (rtfStyleAttr, rtfBasedOn))
				{
					sp->rtfSBasedOn = rtfParam;
					continue;
				}
				if (RTFCheckMM (rtfStyleAttr, rtfAdditive))
				{
					sp->rtfSAdditive = 1;
					continue;
				}
				if (RTFCheckMM (rtfStyleAttr, rtfNext))
				{
					sp->rtfSNextPar = rtfParam;
					continue;
				}
				if ((sep = New (RTFStyleElt)) == (RTFStyleElt *) NULL)
					RTFPanic ("%s: cannot allocate style element", fn);
				sep->rtfSEClass = rtfClass;
				sep->rtfSEMajor = rtfMajor;
				sep->rtfSEMinor = rtfMinor;
				sep->rtfSEParam = rtfParam;
				if ((sep->rtfSEText = RTFStrSave (rtfTextBuf))
								== (char *) NULL)
					RTFPanic ("%s: cannot allocate style element text", fn);
				if (sepLast == (RTFStyleElt *) NULL)
					sp->rtfSSEList = sep;	/* first element */
				else				/* add to end */
					sepLast->rtfNextSE = sep;
				sep->rtfNextSE = (RTFStyleElt *) NULL;
				sepLast = sep;
			}
			else if (RTFCheckCM (rtfGroup, rtfBeginGroup))
			{
				/*
				 * This passes over "{\*\keycode ... }, among
				 * other things. A temporary (perhaps) hack.
				 */
				RTFSkipGroup ();
				continue;
			}
			else if (rtfClass == rtfText)	/* style name */
			{
				bp = buf;
				while (rtfClass == rtfText)
				{
					if (rtfMajor == ';')
					{
						/* put back for "for" loop */
						(void) RTFUngetToken ();
						break;
					}
					*bp++ = rtfMajor;
					(void) RTFGetToken ();
				}
				*bp = '\0';
				if ((sp->rtfSName = RTFStrSave (buf)) == (char *) NULL)
					RTFPanic ("%s: cannot allocate style name", fn);
			}
			else		/* unrecognized */
			{
				/* ignore token but announce it */
				RTFMsg ("%s: unknown token \"%s\"\n",
							fn, rtfTextBuf);
			}
		}
		(void) RTFGetToken ();
		if (!RTFCheckCM (rtfGroup, rtfEndGroup))
			RTFPanic ("%s: missing \"}\"", fn);

		/*
		 * Check over the style structure.  A name is a must.
		 * If no style number was specified, check whether it's the
		 * Normal style (in which case it's given style number
		 * rtfNormalStyleNum).  Note that some "normal" style names
		 * just begin with "Normal" and can have other stuff following,
		 * e.g., "Normal,Times 10 point".  Ugh.
		 *
		 * Some German RTF writers use "Standard" instead of "Normal".
		 */
		if (sp->rtfSName == (char *) NULL)
			RTFPanic ("%s: missing style name", fn);
		if (sp->rtfSNum < 0)
		{
			if (strncmp (buf, "Normal", 6) != 0
				&& strncmp (buf, "Standard", 8) != 0)
				RTFPanic ("%s: missing style number", fn);
			sp->rtfSNum = rtfNormalStyleNum;
		}
		if (sp->rtfSNextPar == -1)	/* if \snext not given, */
			sp->rtfSNextPar = sp->rtfSNum;	/* next is itself */
	}
	RTFRouteToken ();	/* feed "}" back to router */
}


static void ReadInfoGroup ()
{
	RTFSkipGroup ();
	RTFRouteToken ();	/* feed "}" back to router */
}


static void ReadPictGroup ()
{
	RTFSkipGroup ();
	RTFRouteToken ();	/* feed "}" back to router */
}


static void ReadObjGroup ()
{
	RTFSkipGroup ();
	RTFRouteToken ();	/* feed "}" back to router */
}


/* ---------------------------------------------------------------------- */

/*
 * Routines to return pieces of stylesheet, or font or color tables.
 * References to style 0 are mapped onto the Normal style.
 */


RTFStyle *
RTFGetStyle (num)
int	num;
{
RTFStyle	*s;

	if (num == -1)
		return (styleList);
	for (s = styleList; s != (RTFStyle *) NULL; s = s->rtfNextStyle)
	{
		if (s->rtfSNum == num)
			break;
	}
	return (s);		/* NULL if not found */
}


RTFFont *
RTFGetFont (num)
int	num;
{
RTFFont	*f;

	if (num == -1)
		return (fontList);
	for (f = fontList; f != (RTFFont *) NULL; f = f->rtfNextFont)
	{
		if (f->rtfFNum == num)
			break;
	}
	return (f);		/* NULL if not found */
}


RTFColor *
RTFGetColor (num)
int	num;
{
RTFColor	*c;

	if (num == -1)
		return (colorList);
	for (c = colorList; c != (RTFColor *) NULL; c = c->rtfNextColor)
	{
		if (c->rtfCNum == num)
			break;
	}
	return (c);		/* NULL if not found */
}


/* ---------------------------------------------------------------------- */


/*
 * Expand style n, if there is such a style.
 */

void
RTFExpandStyle (n)
int	n;
{
RTFStyle	*s;
RTFStyleElt	*se;

	if (n == -1 || (s = RTFGetStyle (n)) == (RTFStyle *) NULL)
		return;
	if (s->rtfExpanding != 0)
		RTFPanic ("Style expansion loop, style %d", n);
	s->rtfExpanding = 1;	/* set expansion flag for loop detection */
	/*
	 * Expand "based-on" style (unless it's the same as the current
	 * style -- Normal style usually gives itself as its own based-on
	 * style).  Based-on style expansion is done by synthesizing
	 * the token that the writer needs to see in order to trigger
	 * another style expansion, and feeding to token back through
	 * the router so the writer sees it.
	 */
	if (n != s->rtfSBasedOn)
	{
		RTFSetToken (rtfControl, rtfParAttr, rtfStyleNum,
							s->rtfSBasedOn, "\\s");
		RTFRouteToken ();
	}
	/*
	 * Now route the tokens unique to this style.  RTFSetToken()
	 * isn't used because it would add the param value to the end
	 * of the token text, which already has it in.
	 */
	for (se = s->rtfSSEList; se != (RTFStyleElt *) NULL; se = se->rtfNextSE)
	{
		rtfClass = se->rtfSEClass;
		rtfMajor = se->rtfSEMajor;
		rtfMinor = se->rtfSEMinor;
		rtfParam = se->rtfSEParam;
		(void) strcpy (rtfTextBuf, se->rtfSEText);
		rtfTextLen = strlen (rtfTextBuf);
		RTFRouteToken ();
	}
	s->rtfExpanding = 0;	/* done - clear expansion flag */
}


/* ---------------------------------------------------------------------- */

/*
 * Control symbol lookup routines
 */


typedef struct RTFCtrl	RTFCtrl;

struct RTFCtrl
{
	int	major;	/* major number */
	int	minor;	/* minor number */
	char	*str;	/* symbol name */
	int	hash;	/* symbol name hash value */
};

/*
 * A minor number of -1 means the token has no minor number
 * (all valid minor numbers are >= 0).
 */

static RTFCtrl	**rtfCtrl = (RTFCtrl **) NULL;
static int	nKeys;


/*
 * Initialize lookup table hash values.  Only need to do this once.
 */

# define        ctrlFileName    "rtf-ctrl"

static void LookupInit ()
{
FILE    *f;
RTFCtrl *rp;
char    buf[rtfBufSiz];
int     line = 0, i;
char    *p1, *p2, *p3, c;
TSScanner       scanner;
char            *scanEscape;
char    *fn = "LookupInit";

        if (rtfCtrl != (RTFCtrl **) NULL)       /* already initialized */
                return;

//         if ((f = RTFOpenLibFile (ctrlFileName, "r")) == (FILE *) NULL)
//                 RTFPanic ("%s: cannot open %s file.", fn, ctrlFileName);

	if ((f = fopen (ctrlFileName, "r")) == (FILE *) NULL)
        RTFPanic ("%s: cannot open %s file.", fn, ctrlFileName);

        /*
         * Turn off scanner's backslash escape mechanism while reading
         * file.  Restore it later.
         */
        TSGetScanner (&scanner);
        scanEscape = scanner.scanEscape;
        scanner.scanEscape = "";
        TSSetScanner (&scanner);

        while (fgets (buf, (int) sizeof (buf), f) != (char *) NULL)
        {
                if (buf[0] == '#')              /* skip comments */
                        continue;
                ++line;
                TSScanInit (buf);
                p1 = TSScan ();
                p2 = TSScan ();
                p3 = TSScan ();
                if (line == 1)  /* this is the line with the count on it */
                {
                        if (p1 == (char *) NULL)        /* malformed */
                                break;
                        nKeys = atoi (p1);
                        rtfCtrl = (RTFCtrl **) RTFAlloc (nKeys * sizeof (RTFCtrl *));
                        if (rtfCtrl == (RTFCtrl **) NULL)
                                RTFPanic ("%s: out of memory.", fn);
                        for (i = 0; i < nKeys; i++)
                        {
                                rp = (RTFCtrl *) RTFAlloc (sizeof (RTFCtrl));
                                if (rp == (RTFCtrl *) NULL)
                                        RTFPanic ("%s: out of memory.", fn);
                                rtfCtrl[i] = rp;
                        }
                }
                else
                {
                        if (p3 == (char *) NULL)        /* malformed */
                                break;
                        if (line - 1 > nKeys)           /* malformed */
                                break;
                        rp = rtfCtrl[line-2];
                        rp->major = atoi (p1);
                        rp->minor = atoi (p2);
                        /* reprocess string to remove embedded escapes */
                        p1 = p2 = p3;
                        while ((c = *p1++) != '\0')
                        {
                                /*
                                 * Escaped character.  Default is to use next
                                 * character unmodified, but \n and \r are
                                 * turned into linefeed and carriage return.
                                 */
                                if (c == '\\')
                                {
                                        c = *p1++;
                                        switch (c)
                                        {
                                        case 'n':
                                                c = '\n';
                                                break;
                                        case 'r':
                                                c = '\r';
                                                break;
                                        }
                                }
                                *p2++ = c;
                        }
                        *p2 = '\0';
                        if ((rp->str = RTFStrSave (p3)) == (char *) NULL)
                                RTFPanic ("%s: out of memory.", fn);
                        rp->hash = Hash (rp->str);
                }
        }
        scanner.scanEscape = scanEscape;
        TSSetScanner (&scanner);
        (void) fclose (f);

        if (rtfCtrl == (RTFCtrl **) NULL || line - 1 != nKeys)
                RTFPanic ("%s: %s file contents malformed.", fn, ctrlFileName);
}


/*
 * Determine major and minor number of control token.  If it's
 * not found, the class turns into rtfUnknown.
 */

static void Lookup (char *s)
{
RTFCtrl	*rp;
int	hash;
int	i;

	++s;			/* skip over the leading \ character */
	hash = Hash (s);
	for (i = 0; i < nKeys; i++)
	{
	  rp = rtfCtrl[i];
	  /*printf("Maj %d\tMin %d\t\"%s\"\t%d\n",rp->major,rp->minor,rp->str,rp->hash);*/
	  if (/*0 &&*/ hash == rp->hash && strcmp (s, rp->str) == 0)
		{
			rtfClass = rtfControl;
			rtfMajor = rp->major;
			rtfMinor = rp->minor;
			return;
		}
	}
	rtfClass = rtfUnknown;
}


/*
 * Compute hash value of symbol
 */

static int
Hash (s)
char	*s;
{
char	c;
int	val = 0;

	while ((c = *s++) != '\0')
		val += (int) c;
	return (val);
}


/* ---------------------------------------------------------------------- */

/*
 * Memory allocation routines
 */


/*
 * Return pointer to block of size bytes, or NULL if there's
 * not enough memory available.
 *
 * This is called through RTFAlloc(), a define which coerces the
 * argument to int.  This avoids the persistent problem of allocation
 * failing and causing mysterious crashes under THINK C when a long is
 * passed.
 */

char *
_RTFAlloc (size)
int	size;
{
	return ((char *) malloc (size));
}


/*
 * Saves a string on the heap and returns a pointer to it.
 */


char *
RTFStrSave (s)
char	*s;
{
char	*p;

	if ((p = RTFAlloc ((int) (strlen (s) + 1))) == (char *) NULL)
		return ((char *) NULL);
	return (strcpy (p, s));
}


void
RTFFree (p)
char	*p;
{
	if (p != (char *) NULL)
		free (p);
}


/* ---------------------------------------------------------------------- */


/*
 * Token comparison routines
 */

int
RTFCheckCM (class, major)
int	class, major;
{
	return (rtfClass == class && rtfMajor == major);
}


int
RTFCheckCMM (class, major, minor)
int	class, major, minor;
{
	return (rtfClass == class && rtfMajor == major && rtfMinor == minor);
}


int
RTFCheckMM (major, minor)
int	major, minor;
{
	return (rtfMajor == major && rtfMinor == minor);
}


/* ---------------------------------------------------------------------- */


int
RTFCharToHex (c)
char	c;
{
	if (isupper (c))
		c = tolower (c);
	if (isdigit (c))
		return (c - '0');	/* '0'..'9' */
	return (c - 'a' + 10);		/* 'a'..'f' */
}


int
RTFHexToChar (i)
int	i;
{
	if (i < 10)
		return (i + '0');
	return (i - 10 + 'a');
}


/* ---------------------------------------------------------------------- */

/*
 * RTFReadOutputMap() -- Read output translation map
 */


/*
 * Read in a file describing the relation between the standard character set
 * and an RTF translator's corresponding output sequences.  Each line consists
 * of a standard character name and the output sequence for that character.
 *
 * outMap is an array of strings into which the sequences should be placed.
 * It should be declared like this in the calling program:
 *
 *	char *outMap[rtfSC_MaxChar];
 *
 * reinit should be non-zero if outMap should be initialized before reading the
 * file, zero otherwise.  (This allows the map to be constructed by reading
 * several files.)  It's assumed that any existing strings in the map were
 * allocated by RTFStrSave().  The map is initialized BEFORE any attempt is
 * made to read the file.
 *
 * If the filename is an absolute pathname, look in the specified location
 * only.  Otherwise try to find the file in the current directory or the
 * library directory.
 */

int RTFReadOutputMap (char *file, char *outMap[], int reinit)
{
FILE	*f;
char	buf[rtfBufSiz];
char	*name, *seq;
int	stdCode;
int	i;
TSScanner	scanner;
char		*scanEscape;
char		*fn = "RTFReadOutputMap";

	/* clobber current mapping */

	if (reinit)
	{
		for (i = 0; i < rtfSC_MaxChar; i++)
		{
			RTFFree (outMap[i]);
			outMap[i] = (char *) NULL;
		}
	}

//	if ((f = RTFOpenLibFile (file, "r")) == (FILE *) NULL)
//		return (0);

	if ((f = fopen (file, "r")) == (FILE *) NULL)
		return (0);

	/*
	 * Turn off scanner's backslash escape mechanism while reading
	 * file.  Restore it later.
	 */
	TSGetScanner (&scanner);
	scanEscape = scanner.scanEscape;
	scanner.scanEscape = "";
	TSSetScanner (&scanner);

	/* read file */

	while (fgets (buf, (int) sizeof (buf), f) != (char *) NULL)
	{
		if(buf[0] == '#')	/* skip comment lines */
			continue;
		TSScanInit (buf);
		if ((name = TSScan ()) == (char *) NULL)
			continue;	/* skip blank lines */
		if ((stdCode = RTFStdCharCode (name)) < 0)
		{
			RTFMsg ("%s: unknown character name: %s\n", fn, name);
			continue;
		}
		if ((seq = TSScan ()) == (char *) NULL)
		{
			RTFMsg ("%s: malformed output sequence line for character %s\n", fn, name);
			continue;
		}
		if ((seq = RTFStrSave (seq)) == (char *) NULL)
			RTFPanic ("%s: out of memory", fn);
		outMap[stdCode] = seq;
	}
	scanner.scanEscape = scanEscape;
	TSSetScanner (&scanner);
	fclose(f);
	return (1);
}


/* ---------------------------------------------------------------------- */

/*
 * Open a library file.
 */


static FILE	*(*libFileOpen) () = NULL;



void
RTFSetOpenLibFileProc (proc)
FILE	*(*proc) ();
{
	libFileOpen = proc;
}


FILE *
RTFOpenLibFile (file, mode)
char	*file;
char	*mode;
{
	if (libFileOpen == NULL)
		return ((FILE *) NULL);
	return ((*libFileOpen) (file, mode));
}


/* ---------------------------------------------------------------------- */

/*
 * Print message.  Default is to send message to stderr
 * but this may be overridden with RTFSetMsgProc().
 *
 * Message should include linefeeds as necessary.  If the default
 * function is overridden, the overriding function may want to
 * map linefeeds to another line ending character or sequence if
 * the host system doesn't use linefeeds.
 */


static void
DefaultMsgProc (s)
char	*s;
{
	fprintf (stderr, "%s", s);
}


static RTFFuncPtr	msgProc = DefaultMsgProc;


void
RTFSetMsgProc (proc)
RTFFuncPtr	proc;
{
	msgProc = proc;
}


# ifdef STDARG

/*
 * This version is for systems with stdarg
 */

void
RTFMsg (char *fmt, ...)
{
char	buf[rtfBufSiz];

	va_list args;
	va_start (args,fmt);
	vsprintf (buf, fmt, args);
	va_end (args);
	(*msgProc) (buf);
}

# else /* !STDARG */

# ifdef	VARARGS


/*
 * This version is for systems that have varargs.
 */

void
RTFMsg (va_alist)
va_dcl
{
va_list	args;
char	*fmt;
char	buf[rtfBufSiz];

	va_start (args);
	fmt = va_arg (args, char *);
	vsprintf (buf, fmt, args);
	va_end (args);
	(*msgProc) (buf);
}

# else	/* !VARARGS */

/*
 * This version is for systems that don't have varargs.
 */

void
RTFMsg (fmt, a1, a2, a3, a4, a5, a6, a7, a8, a9)
char	*fmt;
char	*a1, *a2, *a3, *a4, *a5, *a6, *a7, *a8, *a9;
{
char	buf[rtfBufSiz];

	sprintf (buf, fmt, a1, a2, a3, a4, a5, a6, a7, a8, a9);
	(*msgProc) (buf);
}

# endif	/* !VARARGS */
# endif /* !STDARG */


/* ---------------------------------------------------------------------- */


/*
 * Process termination.  Print error message and exit.  Also prints
 * current token, and current input line number and position within
 * line if any input has been read from the current file.  (No input
 * has been read if prevChar is EOF).
 */

static void
DefaultPanicProc (s)
char	*s;
{
	fprintf (stderr, "%s", s);
	exit (1);
}


static RTFFuncPtr	panicProc = DefaultPanicProc;


void
RTFSetPanicProc (proc)
RTFFuncPtr	proc;
{
	panicProc = proc;
}


# ifdef STDARG

/*
 * This version is for systems with stdarg
 */

void
RTFPanic (char *fmt, ...)
{
char	buf[rtfBufSiz];

	va_list args;
	va_start (args,fmt);
	vsprintf (buf, fmt, args);
	va_end (args);
	(void) strcat (buf, "\n");
	if (prevChar != EOF && rtfTextBuf != (char *) NULL)
	{
		sprintf (buf + strlen (buf),
			"Last token read was \"%s\" near line %ld, position %d.\n",
			rtfTextBuf, rtfLineNum, rtfLinePos);
	}
	(*panicProc) (buf);
}

# else /* !STDARG */

# ifdef	VARARGS


/*
 * This version is for systems that have varargs.
 */

void
RTFPanic (va_alist)
va_dcl
{
va_list	args;
char	*fmt;
char	buf[rtfBufSiz];

	va_start (args);
	fmt = va_arg (args, char *);
	vsprintf (buf, fmt, args);
	va_end (args);
	(void) strcat (buf, "\n");
	if (prevChar != EOF && rtfTextBuf != (char *) NULL)
	{
		sprintf (buf + strlen (buf),
			"Last token read was \"%s\" near line %ld, position %d.\n",
			rtfTextBuf, rtfLineNum, rtfLinePos);
	}
	(*panicProc) (buf);
}

# else	/* !VARARGS */

/*
 * This version is for systems that don't have varargs.
 */

void
RTFPanic (fmt, a1, a2, a3, a4, a5, a6, a7, a8, a9)
char	*fmt;
char	*a1, *a2, *a3, *a4, *a5, *a6, *a7, *a8, *a9;
{
char	buf[rtfBufSiz];

	sprintf (buf, fmt, a1, a2, a3, a4, a5, a6, a7, a8, a9);
	(void) strcat (buf, "\n");
	if (prevChar != EOF && rtfTextBuf != (char *) NULL)
	{
		sprintf (buf + strlen (buf),
			"Last token read was \"%s\" near line %ld, position %d.\n",
			rtfTextBuf, rtfLineNum, rtfLinePos);
	}
	(*panicProc) (buf);
}

# endif	/* !VARARGS */
# endif /* !STDARG */
