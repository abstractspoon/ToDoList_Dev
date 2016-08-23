/*
 * TransInit.c -- Read in the RTF to HTML translation Tables
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "tokenscan.h"
#include "rtf.h"
#include "rtftohtml.h"
#include "string.h"
#include "transinit.h"

#define SNone 	0
#define STMatch 1
#define SPMatch 2
#define STTags	3
#define SPTags	4

char *StrToken(char * file, int lineno)
{
	char * token;
	if((token = TSScan ()) == (char *) NULL){
		RTFPanic ("In file %s, Not enough fields in table at line:%d",
		file,lineno);
		}
	return(token);
}
char * 
mystrdup(ptr)
char *ptr;
{
	char * ptr2;
	int i,j;
	int isesc=0;
	int mlen;
	mlen=strlen(ptr)+1;
	if((ptr2= (char *) malloc((int) sizeof(char)*(mlen)))==NULL){
		RTFPanic ("Memory Allocation Failed");
	}
	for(i=0,j=0;i<mlen-1;i++){
		if(isesc){
			if(ptr[i]=='n'){
				ptr2[j++]='\n';
			} else if (ptr[i]=='t'){
				ptr2[j++]='\t';
			} else {
				ptr2[j++]='\\';
				ptr2[j++]=ptr[i];
			}
			isesc=0;
		} else if(ptr[i]=='\\'){
			isesc=1;
		} else {
			ptr2[j++]=ptr[i];
		}
	}
	ptr2[j]='\0';
	if(j>=mlen){
		RTFPanic ("Error - overwrote string");
	}
	return(ptr2);	
}

long atobmask(char *str, char * file, int lineno)
{
	long bmask=0;
	char *str2=str;
	while(*str2!='\0'){
		bmask=bmask<<1;
		if(*str2=='1')bmask++;
		else if(*str2!='0'){
			RTFPanic ("In file %s, Invalid bitmask %s at line:%d",
			str,file,lineno);
		}
		str2++;
	}
	return(bmask);
}
void
AddRec(len,alloc,ptr,size)
int *len;
int *alloc;
void **ptr ;
size_t size;
{
	if(*len==*alloc){
		*alloc+=10;
		if(*ptr==NULL){
			*ptr= (void *) malloc(size**alloc);
		} else {
			*ptr= (void *) realloc(*ptr,size**alloc);
		}
		if(*ptr==NULL){
			RTFPanic ("Memory Allocation Failed (%d bytes)",size**alloc);
		}
	}
	(*len)++;
}
int TransInit(char *file)
{
	FILE	*f;
	char	buf[512];
	/* TStyle_typ    bmask; */
	TSScanner	scanner;
	char *scanDelim;
	char *scanEscape;
	int State=SNone;
	int i;
	int transver=0;
	char * token;
	int lineno=0;
	
	/* First Free up any definitions that may be there */
	for(i=0;i<TMatchLen;i++){
		free(TMatchArr[i].Font);
	}
	TMatchLen=0;
	for(i=0;i<PMatchLen;i++){
		free(PMatchArr[i].PStyle);
	}
	PMatchLen=0;
	for(i=0;i<PTagLen;i++){
		free(PTagArr[i].Name);
		free(PTagArr[i].StartTag);
		free(PTagArr[i].EndTag);
		free(PTagArr[i].Col2Tag);
		free(PTagArr[i].TabTag);
		free(PTagArr[i].ParTag);
	}
	PTagLen=0;
	for(i=0;i<TTagLen;i++){
		free(TTagArr[i].StartTag);
		free(TTagArr[i].EndTag);
	}
	TTagLen=0;
	
// 	if ((f = RTFOpenLibFile (file, "r")) == (FILE *) NULL)
// 		return (0);

	if ((f = fopen (file, "r")) == (FILE *) NULL)
		return (0);


	/*
	 * Set the list of delimiters to "," while reading the
	 * file.  Restore it later.
	 */
	TSGetScanner (&scanner);
	scanDelim=scanner.scanDelim;
	scanner.scanDelim = ",\n \t";
	scanEscape = scanner.scanEscape;
	scanner.scanEscape = "";
	TSSetScanner (&scanner);
		
	/* read file */

	while (fgets (buf, (int) sizeof (buf), f) != (char *) NULL)
	{
		lineno++;
		if(buf[0] == '#')	/* skip comment lines */
			continue;
		TSScanInit (buf);
		if(buf[0]== '.'){
			if((token = TSScan ()) == (char *) NULL)
				continue;	/* skip blank lines */
			if(strcmp(token,".Version")==0){
				transver=atoi(StrToken(file,lineno));
				continue;
			}
			if(strcmp(token,".TMatch")==0){
				State=STMatch;
				if(TMatchLen>0){
					RTFPanic ("Duplicate Definition of %s in: %s at %d",
					".TMatch",file,lineno);
				}
				continue;
			} else if(strcmp(token,".PMatch")==0) {
				State=SPMatch;
				if(PMatchLen>0){
					RTFPanic ("Duplicate Definition of %s in: %s at %d",
					".PMatch",file,lineno);
				}
				continue;
			} else if(strcmp(token,".PTag")==0) {
				State=SPTags;
				if(PTagLen>0){
					RTFPanic ("Duplicate Definition of %s in: %s at %d",
					".PTag",file,lineno);
				}
				continue;
			} else if(strcmp(token,".TTag")==0) {
				State=STTags;
				if(TTagLen>0){
					RTFPanic ("Duplicate Definition of %s in: %s at %d",
					".TTag",file,lineno);
				}
				continue;
			} else {
				RTFPanic ("Invalid Control Word %s in: %s at %d",token,file,lineno);
			}
		} 
		if((token = TSScan ()) == (char *) NULL)
			continue;	/* skip blank lines */
		if(transver != Htran){
			RTFPanic ("The version of html-trans is incompatible with this release of rtftohtml.");
		}

		switch(State){
		case STMatch:
				AddRec(&TMatchLen,&TMatchAlloc,(void **)&TMatchArr,sizeof(TMatchRec));
				TMatchArr[TMatchLen-1].Font=mystrdup(token);
				TMatchArr[TMatchLen-1].FSize=atoi(StrToken(file,lineno));
				TMatchArr[TMatchLen-1].TStyle=atobmask(StrToken(file,lineno),file,lineno);
				TMatchArr[TMatchLen-1].TMask=atobmask(StrToken(file,lineno),file,lineno);
				token=StrToken(file,lineno);
				if(strcmp(token,"_Discard")==0){
					TMatchArr[TMatchLen-1].TTidx=MTDiscard;
				} else if(strcmp(token,"_Name")==0){
					TMatchArr[TMatchLen-1].TTidx=MTName;
				} else if(strcmp(token,"_HRef")==0){
					TMatchArr[TMatchLen-1].TTidx=MTHref;
				} else if(strcmp(token,"_Hot")==0){
					TMatchArr[TMatchLen-1].TTidx=MTHot;
				} else if(strcmp(token,"_FootNote")==0){
					TMatchArr[TMatchLen-1].TTidx=MTFootNote;
				} else if(strcmp(token,"_Literal")==0){
					TMatchArr[TMatchLen-1].TTidx=MTLiteral;
				} else {
					for(i=0;i<TTagLen;i++){
						if(strcmp(token,TTagArr[i].Name)==0){
							TMatchArr[TMatchLen-1].TTidx=i;
							break;
						}
					}
					if(i==TTagLen){
						RTFPanic ("Tag Name %s not in .TTag table, file:%s line %d",
						token,file,lineno);
					}
				}
				break;
		case SPMatch:
				AddRec(&PMatchLen,&PMatchAlloc,(void **)&PMatchArr,sizeof(PMatchRec));
				PMatchArr[PMatchLen-1].PStyle=mystrdup(token);
				PMatchArr[PMatchLen-1].NestLev=atoi(StrToken(file,lineno));
				token=StrToken(file,lineno);
				if(strcmp(token,"_Discard")==0){
					PMatchArr[PMatchLen-1].PTidx=MTDiscard;
				} else if(strcmp(token,"_Name")==0){
					PMatchArr[PMatchLen-1].PTidx=MTName;
				} else if(strcmp(token,"_HRef")==0){
					PMatchArr[PMatchLen-1].PTidx=MTHref;
				} else if(strcmp(token,"_Hot")==0){
					PMatchArr[PMatchLen-1].PTidx=MTHot;
				} else if(strcmp(token,"_FootNote")==0){
					PMatchArr[PMatchLen-1].PTidx=MTFootNote;
				} else if(strcmp(token,"_Literal")==0){
					PMatchArr[PMatchLen-1].PTidx=MTLiteral;
				} else {
					for(i=0;i<PTagLen;i++){
						if(strcmp(token,PTagArr[i].Name)==0){
							PMatchArr[PMatchLen-1].PTidx=i;
							break;
						}
					}
					if(i==PTagLen){
						RTFPanic ("PTag Name %s not in .PTag table, file:%s line %d",
						token,file,lineno);
					}
				}
				break;
		case SPTags:
				AddRec(&PTagLen,&PTagAlloc,(void **)&PTagArr,sizeof(PTagRec));
				PTagArr[PTagLen-1].Name=mystrdup(token);
				PTagArr[PTagLen-1].StartTag=mystrdup(StrToken(file,lineno));
				PTagArr[PTagLen-1].EndTag=mystrdup(StrToken(file,lineno));
				PTagArr[PTagLen-1].Col2Tag=mystrdup(StrToken(file,lineno));
				PTagArr[PTagLen-1].TabTag=mystrdup(StrToken(file,lineno));
				PTagArr[PTagLen-1].ParTag=mystrdup(StrToken(file,lineno));
				PTagArr[PTagLen-1].AllowText=atoi(StrToken(file,lineno));
				PTagArr[PTagLen-1].CanNest=atoi(StrToken(file,lineno));
				PTagArr[PTagLen-1].DeleteCol1=atoi(StrToken(file,lineno));
				PTagArr[PTagLen-1].DoFold=atoi(StrToken(file,lineno));
				PTagArr[PTagLen-1].ToCLev=atoi(StrToken(file,lineno));
				break;
		case STTags:
				AddRec(&TTagLen,&TTagAlloc,(void **)&TTagArr,sizeof(TTagRec));
				TTagArr[TTagLen-1].Name=mystrdup(token);
				TTagArr[TTagLen-1].StartTag=mystrdup(StrToken(file,lineno));
				TTagArr[TTagLen-1].EndTag=mystrdup(StrToken(file,lineno));
				break;
		case SNone:
				RTFPanic ("Missing Control Word in %s, at line %d",file,lineno);
		}
	}
	scanner.scanDelim = scanDelim;
	scanner.scanEscape = scanEscape;
	TSSetScanner (&scanner);
	fclose(f);
	if(TTagLen==0){
		RTFPanic ("Missing table %s, in file: %s",".TTag",file);
	}
	if(PTagLen==0){
		RTFPanic ("Missing table %s, in file: %s",".PTag",file);
	}
	if(TMatchLen==0){
		RTFPanic ("Missing table %s, in file: %s",".TMatch",file);
		/*exit(1);*/
	}
	if(PMatchLen==0){
		RTFPanic ("Missing table %s, in file: %s",".PMatch",file);
	}

	return (1);
}
