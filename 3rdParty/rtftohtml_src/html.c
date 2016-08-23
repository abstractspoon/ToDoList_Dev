/*
 *
 * Driver provides OpenOutputFile() and Basename() functions for the writer.
 */

# include	<stdio.h>

# include	"rtf.h"
# include	"rtftohtml.h"
#include "htmlout.h"

FILE *
OpenOutputFile (char	*name, char	*mode, int	fileType)
{
	return (fopen (name, mode));
}

/*
 * Return pointer to basename part of pathname
 */

# define	pathSep	'\\'

char *
Basename (char	*name)
{
	int	i, lsep;

	for(i=0,lsep=0;name[i]!='\0';i++)
	{
		if(name[i]==pathSep)
			lsep=i+1;
	}
	return (&name[lsep]);
}
