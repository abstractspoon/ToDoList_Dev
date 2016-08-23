/*
 * html-unix.c -- UNIX rtftohtml wrapper
 *
 * Driver provides OpenOutputFile() and Basename() functions for the writer.
 */

# include	<stdio.h>

# include	"rtf.h"
# include	"rtf-unix.h"
# include	"rtftohtml.h"
#include "htmlout.h"

static char *usage="rtftohtml [-V] [-i] [-G] [-T] [-o filename] [-P extension] file";


int
main (argc, argv)
int	argc;
char	*argv[];
{
	/* install OS-specific callbacks into RTF library */
	UnixSetProgPath (argv[0]);
	RTFSetOpenLibFileProc (UnixOpenLibFile);

	WriterInit ();
	RTFInit ();
	switch (do_main(argc,argv))	/* set up for output file */
	{
	case 1:			/* successful */
		RTFRead ();
		HTMLCleanup();
		break;
	case 0:			/* unsuccessful; die */
		exit (1);
	case -1:		/* unsuccessful; print usage message and die*/
		RTFPanic ("Usage: %s", usage);
	}
	exit (0);
}


FILE *
OpenOutputFile (name, mode, fileType)
char	*name;
char	*mode;
int	fileType;	/* ignored */
{
	return (fopen (name, mode));
}


/*
 * Return pointer to basename part of pathname
 */

# define	pathSep	'/'

char *
Basename (name)
char	*name;
{
int	i, lsep;

	for(i=0,lsep=0;name[i]!='\0';i++)
	{
		if(name[i]==pathSep)
			lsep=i+1;
	}
	return (&name[lsep]);
}
