/* Think C program to make header files for Macintosh. 								*/
/* This program produces rtf.h and stdcharnames.h files for the DuBois RTF parser.	*/
/* The output files have been checked with the Compare Files command of BBEdit and	*/
/* are identical to those produced by awk and sed under UNIX.						*/

/* Input files "standard-names" and "rtf.h.dist" are expected to be in the			*/
/* the directory rtftohtml/Libs/h relative to where the program runs -CJH-		*/
/* Macintosh users may want to use a utility like FileTyper to set the output files	*/
/* creator to 'KAHL' for convenient Mac access.										*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *fnrtft = ":rtftohtml_src:Libs:h:rtf.h.dist";		/* input files */
char *fnstct = ":rtftohtml_src:Libs:h:standard-names";
char *fnrtf = ":rtftohtml_src:Libs:h:rtf.h";				/* output files */
char *fnstc = ":rtftohtml_src:Libs:h:stdcharnames.h";
FILE *fin;
FILE *frout, *fsout;

int main(int argc, char **argv)
{
	char buf[256];
	char *fstat;
	int fostat;
	int len;
	int count = 0;
	
	/* start with rtf.h */
	
	fin = fopen(fnrtft, "r");
	if (fin == NULL) {
		fprintf(stderr, "Can't open %s!\n", fnrtft);
		exit(1);
	}
	
	_fcreator = 'KAHL';
	frout = fopen(fnrtf, "w");
	if (frout == NULL) {
		fprintf(stderr, "Can't open %s for writing!\n", fnrtf);
		exit(1);
	}
	
	/* copy rtf.h.dist to rtf.h */
	
	while (feof(fin) == false) {
		fstat = fgets(buf, 255, fin);
		if (fstat != NULL) {
			fostat = fputs(buf, frout);
			if (fostat == EOF) {
				fprintf(stderr, "Error copying %s!\n", fnrtf);
				exit(1);
			}
		} else {
			if (ferror(fin) != 0) {
				fprintf(stderr, "Error reading %s!\n", fnrtft);
				exit(1);
			}
		}
	}
	fclose(fin);
	
	fprintf(frout, "\n/* The following defines are automatically generated.  Do not edit. */\n\n");
	fprintf(frout, "\n/* These must be sequential beginning from zero */\n\n");
	
	/* Now process the standard character names.	*/
	
	fin = fopen(fnstct, "r");
	if (fin == NULL) {
		fprintf(stderr, "Can't open %s!\n", fnstct);
		exit(1);
	}
	
	fsout = fopen(fnstc, "w");
	if (frout == NULL) {
		fprintf(stderr, "Can't open %s for writing!\n", fnstc);
		exit(1);
	}
	
	while (feof(fin) == false) {
		fstat = fgets(buf, 255, fin);
		if (fstat != NULL) {
			if (strlen(buf) == 1 && buf[0] == '\n') {	/* ignore empty line */
				continue;
			} else if (buf[0] == '#') {					/* ignore comment line */
				continue;
			} else {
				len = strlen(buf);
				buf[len - 1] = '\0';	/* smash newline */
				fprintf(frout, "#define rtfSC_%s\t%d\n", buf, count);
				fprintf(fsout, "\"%s\",\n", buf);
				count++;
			}
		} else {
			if (ferror(fin) != 0) {
				fprintf(stderr, "Error reading %s!\n", fnstct);
				exit(1);
			}
		}
	}
	
	fclose(fin);
	fclose(fsout);
	fprintf(frout, "\n#define rtfSC_MaxChar\t%d\n", count);
	fclose(frout);
	
	fprintf(stderr, "Finished processing %d standard characters.\n", count);
	exit(0);
}