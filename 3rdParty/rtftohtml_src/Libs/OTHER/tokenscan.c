/*
        TSScanInit() - initialize scanner to scan  a buffer.
        TSScan() - pull successive tokens out of a character.  Returns
        NULL when there are no more.  Has the property that once it returns
        NULL, succeeding calls continue to return NULL until TSScanInit() is
        called again.
        Default behavior is that tokens are sequences of non-blank characters,
        or quotes can be used to allow whitespace characters in the token.
        The '\' character turns off any special meaning of the following
        character.

        TSSetScanner () - install scanner.
        TSGetScanner () - get current scanner.
        TSSetScanPos() - set current position within scan buffer.
        TSGetScanPos() - get current position within scan buffer.
        TSIsScanDelim() - test whether character is in current delimiter set.
        TSIsScanQuote() - test whether character is in current quote set.
        TSIsScanEscape() - test whether character is in current escape set.
        TSIsScanEos() - test whether character is in current EOS set.
        TSTestScanFlags() - test whether scanner flags are set.

        The original buffer is destroyed by scanning, since null bytes are
        placed after each token as they are found.

        12 Apr 90 Created (as part of NIO).
        25 Sep 90 Reorganized to allow accommodating alternate scanners while
                preserving current scan position within buffer.
        26 Oct 90 Removed from NIO and made into separate library.
        25 Sep 91 Parameterized the quote and escape characters.
        06 Oct 93 Added scanFlags field to scanner structure.  Only flag so
                far is to allow all delimiter characters to be significant
                (so scanner stops on each one, rather than treating sequences
                of contiguous delimiters as one delimiter).
        14 Oct 93 Parameterized the end-of-string characters.
*/

# include       <stdio.h>

# include       "tokenscan.h"


typedef void (*VoidProc) ();
typedef char * (*CharPtrProc) ();


static int      Search ();
static char     *CanonScan ();

static TSScanner        defScan =
        { (VoidProc) NULL, CanonScan, " \t", "\"'", "\\", "\n\r", 0 };
static TSScanner        curScan =
        { (VoidProc) NULL, CanonScan, " \t", "\"'", "\\", "\n\r", 0 };

static char     *curPos;
static int      emptyToken;


void
TSScanInit (p)
char    *p;
{
        curPos = p;
        emptyToken = 0;
        if (curScan.scanInit != (VoidProc) NULL)
                (*curScan.scanInit) (p);
}


void
TSSetScanner (p)
TSScanner       *p;
{
        if (p == (TSScanner *) NULL || p->scanInit == (VoidProc) NULL)
                curScan.scanInit = defScan.scanInit;
        else
                curScan.scanInit = p->scanInit;
        if (p == (TSScanner *) NULL || p->scanScan == (CharPtrProc) NULL)
                curScan.scanScan = defScan.scanScan;
        else
                curScan.scanScan = p->scanScan;

        if (p == (TSScanner *) NULL || p->scanDelim == (char *) NULL)
                curScan.scanDelim = defScan.scanDelim;
        else
                curScan.scanDelim = p->scanDelim;
        if (p == (TSScanner *) NULL || p->scanQuote == (char *) NULL)
                curScan.scanQuote = defScan.scanQuote;
        else
                curScan.scanQuote = p->scanQuote;
        if (p == (TSScanner *) NULL || p->scanEscape == (char *) NULL)
                curScan.scanEscape = defScan.scanEscape;
        else
                curScan.scanEscape = p->scanEscape;
        if (p == (TSScanner *) NULL || p->scanEos == (char *) NULL)
                curScan.scanEos = defScan.scanEos;
        else
                curScan.scanEos = p->scanEos;

        if (p == (TSScanner *) NULL || p->scanFlags == 0)
                curScan.scanFlags = defScan.scanFlags;
        else
                curScan.scanFlags = p->scanFlags;
}


void
TSGetScanner (p)
TSScanner       *p;
{
        p->scanInit = curScan.scanInit;
        p->scanScan = curScan.scanScan;

        p->scanDelim = curScan.scanDelim;
        p->scanQuote = curScan.scanQuote;
        p->scanEscape = curScan.scanEscape;
        p->scanEos = curScan.scanEos;

        p->scanFlags = curScan.scanFlags;
}


void
TSSetScanPos (p)
char    *p;
{
        curPos = p;
}


char *
TSGetScanPos ()
{
        return (curPos);
}


/*
 * Search a (possibly NULL) string for a character.
 */

static int
Search (s, c)
char    *s;
char    c;
{
char    c2;

        if (s != (char *) NULL)
        {
                while ((c2 = *s++) != '\0')
                {
                        if (c == c2)
                                return (1);
                }
        }
        return (0);
}


int
TSIsScanDelim (c)
char    c;
{
        return (Search (curScan.scanDelim, c));
}


int
TSIsScanQuote (c)
char    c;
{
        return (Search (curScan.scanQuote, c));
}


int
TSIsScanEscape (c)
char    c;
{
        return (Search (curScan.scanEscape, c));
}


int
TSIsScanEos (c)
char    c;
{
        if (c == '\0')          /* null character ALWAYS terminates string */
                return (1);
        return (Search (curScan.scanEos, c));
}


int
TSTestScanFlags (flags)
int     flags;
{
        return ((curScan.scanFlags & flags) == flags);
}


char *
TSScan ()
{
        return ((*curScan.scanScan) ());
}


static char *
CanonScan ()
{
char    *pos, *start, *p, c, quote = 0;
int     escape = 0, haveToken = 0;

        if (emptyToken)         /* fix to point into buffer */
        {
                emptyToken = 0;
                return ("");
        }
        pos = TSGetScanPos ();

        if (!TSTestScanFlags (tsNoConcatDelims))
        {
                while (TSIsScanDelim (*pos))            /* skip delimiters */
                        ++pos;
        }
        start = p = pos;                        /* start of token */
        while (!TSIsScanEos (c = *pos))         /* eos terminates any token */
        {
                haveToken = 1;
                if (escape)                     /* previous char was escape; */
                {                               /* pass this one literally */
                        *p++ = *pos++;
                        escape = 0;
                }
                else if (TSIsScanEscape (c))    /* this char is escape; */
                {                               /* pass next one literally */
                        ++pos;
                        escape = 1;
                }
                else if (quote)                 /* in quote mode */
                {
                        if (c == quote)         /* end quote mode */
                                quote = 0;
                        else                    /* no end quote yet */
                                *p++ = *pos;    /* add char */
                        ++pos;                  /* skip to next char */
                }
                else if (TSIsScanQuote (c))     /* begin quote mode */
                {
                        ++pos;
                        quote = c;
                }
                else if (TSIsScanDelim (c))     /* end of token */
                {
                        ++pos;                  /* skip past delimiter */
                        /*
                         * If also end of string and all delims are significant,
                         * there's an implicit empty token at end.  We won't be
                         * able to tell that from looking at the string itself,
                         * so set a flag for next call.
                         */
                        if (TSIsScanEos (*pos)
                                && TSTestScanFlags (tsNoConcatDelims))
                                ++emptyToken;
                        break;
                }
                else
                        *p++ = *pos++;
        }
        *p = '\0';
        TSSetScanPos (pos);
        return (haveToken ? start : (char *) NULL);
}
