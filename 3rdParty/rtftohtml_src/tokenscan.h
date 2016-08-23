/*
 * tokenscan.h - token scanning library stuff
 */


/*
 * Flags for scanFlags field
 */

#define	tsNoConcatDelims	0x01	/* all delimiters are significant */


typedef	struct TSScanner	TSScanner;

struct TSScanner
{
	void	(*scanInit) ();
	char	*(*scanScan) ();
	char	*scanDelim;
	char	*scanQuote;
	char	*scanEscape;
	char	*scanEos;
	int	scanFlags;
};


void	TSScanInit ();
char	*TSScan ();
void	TSSetScanner ();
void	TSGetScanner ();
void	TSSetScanPos ();
char	*TSGetScanPos ();
int	TSIsScanDelim ();
int	TSIsScanQuote ();
int	TSIsScanEscape ();
int	TSIsScanEos ();
int	TSTestScanFlags ();
