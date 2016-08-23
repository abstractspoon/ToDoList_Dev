/*
 * fileops.c -- file operations for UNIX versions of translators.
 */
/*
 * UnixOpenLibFile () - Open a library file.  Looks in the following
 * directories:
 * -- jrm21:
 * -- Look in GSDLHOME environment var + etc/packages/rtftohtm
 * -- Look in exec program dir + LIBDIR (relative).
 *
 * - Looks in current directory.
 * - if RTFLIBDIR environment variable is set, looks in directory named by it.
 * - Looks in executable program's directory, if UnixSetProgPath() has
 * been called.
 * - Looks in library directory, LIBDIR
 *
 * Exception: if file is an absolute pathname, look only for file as named.
 *
 * Returns NULL if file cannot be found and opened.
 */
 
# include	<stdio.h>
#include <string.h>
 
# include	"rtf.h"
# include	"rtf-unix.h"
 
extern char	*getenv ();
 
static char	*progPath = (char *) NULL;
 
 
void UnixSetProgPath (char *path)
{
int	i, j, n;

	n = strlen (path);
	for (j = -1, i = 0; i < n; i++)
	{
		if (path[i] == '/')
			j = i;
	}
	if (j < 0)		/* no slash found */
	{
		path = ".";
		j = 1;
	}
	if ((progPath = RTFAlloc (j + 1)) != (char *) NULL)
	{
		strncpy (progPath, path, j);
		progPath[j] = '\0';
	}
}
 
 
FILE *UnixOpenLibFile (char *file, char	*mode)
{
  FILE	*f;
  char	buf[rtfBufSiz]; /* jrm21 - this could be a buffer overflow... */
  char	*p;
  
  
  /* jrm21 - look in GSDL first, then exec path + LIBDIR (it's relative) */
  if ((p = getenv ("GSDLHOME")) != NULL) {
    sprintf (buf, "%s/etc/packages/rtftohtm/%s", p, file);
    if ((f=fopen(buf,mode)) != NULL)
      return f;
  }
#if defined(LIBDIR)
  if (progPath != NULL)
    {
      sprintf (buf, "%s/%s/%s", progPath,LIBDIR, file);
      if ((f = fopen (buf, mode)) != NULL)
	return (f);
    }
#endif


  if ((f = fopen (file, mode)) != NULL)
    {
      return (f);
    }
  /* if abolute pathname, give up, else look in library */
  if (file[0] == '/')
    {
      return (NULL);
    }
  if ((p = getenv ("RTFLIBDIR")) != NULL)
    {
      sprintf (buf, "%s/%s", p, file);
      if ((f = fopen (buf, mode)) != NULL)
	return (f);
    }
  if (progPath != NULL)
    {
      sprintf (buf, "%s/%s", progPath, file);
      if ((f = fopen (buf, mode)) != NULL)
	return (f);
    }
#if defined(LIBDIR)
  sprintf (buf, "%s/%s", LIBDIR, file);
  f = fopen (buf, mode);	/* NULL if it fails */
  return (f);
#endif
  return (NULL);
}
