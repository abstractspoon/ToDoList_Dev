/*
	Copyright (C) 2001, 2002, 2003, 2004, 2008 Sergey A. Galin, http://sageshome.net

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdafx.h"
#include "rtf2html.h"
#include "rtfconst.h"

// read font tag contents (\f) and add the font to CSS2 table
void CRTF2HTML::ParseFont(){
    char fontname[16]; // CSS selector for the font (.f0, .f1 etc)
    int endch, fontnum=-1, AliasIndex=-1, KeyfontIndex=-1;
    CgString kw, font;
    TSeqType stype;
    long par;
    while(!Eof()){
        // reading next keyword or text substring from the input stream
        stype=LoadSequence(kw, &par, &endch);
        if(stype==stKeyword){
            // check if this is a font number definition
            if(!strcmp(kw, "f")){ fontnum=int(par); continue; }
            // check if the keyword defines font group
            for(size_t i=0; i<FontAliasesCount; i++)
                if(!strcmp(kw, FontAliases[i].Tag)){
                   AliasIndex=i; break; }
        }else if(stype==stString){
            // store string content in font name variable, assuming that
            // it describes font name;
            for(size_t p=0; p<kw.Len(); p++) // kill trailng ';'
                if(kw[p]==';'){ kw.Truncate(p); break; }
            font+=kw; // adding text string to font name
        }
        if(endch=='}'){ GetCh(); break; }// parsing complete
        // do not support any sub-groups inside font definition
        if(endch=='{'){ GetCh(); SkipBrackets(); };
    } // end while reading font group {}

    if(fontnum==-1) return; // the group was not font definition
    sprintf(fontname, "f%d", fontnum);
    font.Chop(); // kill spaces at the end of the string

    // applying Key Fonts
    if(font.Len())
        for(size_t j=0; j<KeyFontsCount; j++)
            // if this font is similar to some well-known font
            if(strstr(font, KeyFonts[j].Tag)!=NULL &&
               strcmp(font, KeyFonts[j].Tag)){ KeyfontIndex=j; break; }

    // generating CSS stylesheet entry for this font
    // fXX { font-family: font family description; }
    int i=Fonts.Expand();
    Fonts[i]<<"."<<fontname<<"{ font-family: ";
    if(!font.IsSpace() || AliasIndex>=0){
        if(font.Len()){
            Fonts[i]<<font; if(KeyfontIndex>=0 || AliasIndex>=0) Fonts[i]<<", "; }
        if(KeyfontIndex>=0){ // add "Key font name" after full font name
            Fonts[i]<<KeyFonts[KeyfontIndex].Name;
            if(AliasIndex>=0) Fonts[i]<<", "; }
        if(AliasIndex>=0) Fonts[i]<<FontAliases[AliasIndex].Name; // generic font
    }else Fonts[i]<<"Sans Serif";
    Fonts[i]<<"; }\n";
}

void CRTF2HTML::ParseFontTable(){
    int c;
    SGTRACE("Parsing FONT table...");
    while((c=GetCh())!=EOF){
        if(c=='}') break;
        if(c=='{') ParseFont(); }
    SGTRACE("OK\n");
}

void CRTF2HTML::ParseColorTable(){
    int endchr;
    CgString tag;
    long kwdpar;
    CRTFColor clr;
    SGTRACE("Parsing COLOR table...");
    while(!Eof()){
        switch(LoadSequence(tag, &kwdpar, &endchr)){
            case stKeyword:
                if(!strcmp(tag, "red"))
                    clr.R=(unsigned char)kwdpar;
                else if(!strcmp(tag, "green"))
                         clr.G=(unsigned char)kwdpar;
                     else if(!strcmp(tag, "blue"))
                              clr.B=(unsigned char)kwdpar;
                if(endchr==';') ColorTable<<clr; // add new color to table

                break;
            case stString:
                if(!strcmp(tag.KillSpaces(), ";")){
                     if(ColorTable.Count()>=DefColorsCount)
                          ColorTable<<DefColors[0];
                     else ColorTable<<DefColors[ColorTable.Count()];
                }
        } // end switch
        if(endchr=='{'){ GetCh(); SkipBrackets(); } // don't know any subgroups under color table
        if(endchr=='}'){ GetCh(); break; }
    } // end while reading file
    SGTRACEINT("OK, colors:", ColorTable.Count());
}


void CRTF2HTML::ParseImageProperties(){
  int i, endchr;
  CgString text;
  TSeqType st;
  long kwdpar;

  enum TImageProperty{ ipShapeType, ipFlipH, ipFlipV, ipPibName };
  const TRTFSimpleTag RtfImageProperties[]={
    { "shapeType",  ipShapeType },
    { "fFlipH",     ipFlipH     },
    { "fFlipV",     ipFlipV     },
    { "pibName",    ipPibName   }};
  const size_t RtfImagePropertiesCount=ARRAY_ITEMCOUNT(RtfImageProperties);

  while(!Eof()){
    st=LoadSequence(text, &kwdpar, &endchr);
    switch(st){
      case stKeyword:
        if(!strcmp(text, "sn")){ 
          text.Truncate();
          LoadTextValue(text);
          // loading image property code
          for(i=0; i<RtfImagePropertiesCount; i++)
            if(!strcmp(text, RtfImageProperties[i].Tag)){
              ImagePropertyCode=RtfImageProperties[i].Code;
              break;
            }
          return;
        }

        if(!strcmp(text, "sv")){
          if(ImagePropertyCode>=0){
            text.Truncate();
            LoadTextValue(text);
            // converting string to integer if needed for this property
            if(ImagePropertyCode!=ipPibName) kwdpar=atol(text);
            switch(ImagePropertyCode){
              case ipPibName:   Image.pibName=text;   break;
              case ipFlipH:   Image.FlipH=kwdpar!=0;  break;
              case ipFlipV:   Image.FlipV=kwdpar!=0;  break;
              case ipShapeType: Image.ShapeType=kwdpar; break;
            }
            ImagePropertyCode=-1;
          }else
            SkipBrackets();
          return;
        }
        break;
      case stString: break;
    }
    if(endchr=='{'){ GetCh(); ParseImageProperties(); }
    if(endchr=='}'){ GetCh(); break; }
  }

}

int CRTF2HTML::ParseImage(){
  int i, endchr;
  CgString text;
  TSeqType st;
  long kwdpar;
  
  enum TImageTags{ 
    tagScaleX, tagScaleY, tagCropL, tagCropR, tagCropT, tagCropB, tagPicW, tagPicH,
    tagWGoal, tagHGoal, tagBlipTag,

    tagJpegBlip, tagPngBlip, tagWMetafile
  };

  FILE *out;

  const TRTFSimpleTag RtfImageTags[]={
    { "picscalex",  tagScaleX     },
    { "picscaley",  tagScaleY     },
    { "piccropl",   tagCropL      },
    { "piccropr",   tagCropR      },
    { "piccropt",   tagCropT      },
    { "piccropb",   tagCropB      },
    { "picw",       tagPicW       },
    { "pich",       tagPicH       },
    { "picwgoal",   tagWGoal      },
    { "pichgoal",   tagHGoal      },
    { "bliptag",    tagBlipTag    },
    { "jpegblip",   tagJpegBlip   },
    { "pngblip",    tagPngBlip    },
    { "wmetafile",  tagWMetafile  }
  };

  const size_t RtfImageTagsCount=ARRAY_ITEMCOUNT(RtfImageTags);

  SGTRACE("Parsing Image...");
  Image.Clear();
  ImageUsable=0;


  CgString exFileName, exRelName, exDirectory, exFullPath;

  r2hExtractFilePath(exFullPath, OutFile ? OutFile : "."); // extract output file directory to Full Path


    if(Flags & r2hUseImageDirectory){ // subdirectory requested by user
        SGTRACE(" User_Image_Dir="); 
  exDirectory=ImageDirectory; 
    }else{ // use default directory
        SGTRACE(" Auto_Image_Dir=");
		r2hExtractFileName(exDirectory, OutFile ? OutFile : "."); // extract output file directory to output file name
  exDirectory<<".files";
    };
    SGTRACE(exDirectory); SGTRACE(" ");

    //*output<<"[OutFile: "<<OutFile<<"  Path: "<<exFullPath<<" Directory: "<<exDirectory<<"]";
    //*output<<"<b>{"<<ImageDirectory<<"}</b>";

    if(exDirectory.Len()){ 
        if(exFullPath.Len()) exFullPath<<TREE_SYMBOL;
        exFullPath<<exDirectory;
        exRelName<<exDirectory<<TREE_SYMBOL;
    }   

  // generate appriate file name for this image (no extension) !!!!!!!!!!!!!!!!
  static unsigned long count=0;
  if(!r2hCreateFileName(exFileName, exFullPath, &count)) return ecCannotGenerateName;
  // exFileName<<"Image"<<count;
  //*output<<"<b>"<<exFileName<<"<b>";

  exRelName<<exFileName;
  MKDIR_FN(exFullPath);
  
  // XXX Cannot rely on the result because function will fail if directory already exists (not error!)
  // *output<<"<BR>\nUNABLE TO CREATE DIRECTORY: "<<exFullPath<<" ERRNO: "<<errno<<"<BR>\n";
  // return ecCannotCreateDirectory;
  
  exFullPath<<TREE_SYMBOL<<exFileName;

  out=fopen(exFullPath, "wb");

  if(out==NULL){ 
    *output<<"<BR>\nUNABLE TO WRITE TO: "<<exFullPath<<"<BR>\n"; 
    return ecCannotCreateFile; }

  while(!Eof()){
    switch(st=LoadSequence(text, &kwdpar, &endchr)){
      case stKeyword:
        // looking for the keyword in symbol table
        for(i=0; i<RtfImageTagsCount; i++)
          if(!strcmp(text, RtfImageTags[i].Tag)) break;
        if(i>=RtfImageTagsCount) break;
        switch(RtfImageTags[i].Code){
          case tagScaleX:   Image.ScaleX=kwdpar;        break;
          case tagScaleY:   Image.ScaleY=kwdpar;        break;
          case tagCropL:    Image.CropL=kwdpar;         break;
          case tagCropR:    Image.CropR=kwdpar;         break;
          case tagCropT:    Image.CropT=kwdpar;         break;
          case tagCropB:    Image.CropB=kwdpar;         break;
          case tagPicW:   Image.Width=Twips2Points(kwdpar); break;
          case tagPicH:   Image.Height=Twips2Points(kwdpar);  break;
          case tagWGoal:    Image.WGoal=kwdpar;         break;
          case tagHGoal:    Image.HGoal=kwdpar;         break;
          case tagBlipTag:  Image.BlipTag=kwdpar;       break;

          case tagJpegBlip: strcpy(Image.Extension, "jpg"); ImageUsable=1;  break;
          case tagPngBlip:  strcpy(Image.Extension, "png"); ImageUsable=1;  break;
          case tagWMetafile:  strcpy(Image.Extension, "wmf"); ImageUsable=1;  break; 
        }
          
        break;

      case stString:
        
        if(text.IsSpace()) break;

        ImageRtf2Binary(out, text);

        break;

    }
        if(endchr=='{'){ GetCh(); ImagePropertyCode=-1; ParseImageProperties(); }
        if(endchr=='}'){ 
      //GetCh(); 
      break; }
  }

  
  fclose(out);

  
  CgString newfn=exFullPath;
  newfn<<"."<<Image.Extension;
  rename(exFullPath, newfn);
  exRelName<<"."<<Image.Extension;

  //*output<<"<b>ExFN: "<<exFullPath<<" /// "<<newfn<<"</b>";
  // GENERATING HTML
  //*output<<" &lt;IMAGE Width="<<Image.Width<<", Height="<<Image.Height<<", File="<<Image.pibName<<"&gt; ";

  #if !defined(UNIX_PATHS)
    // under DOS/Windows, we have to fix path slashes to conver path to HTML reference
    exRelName.ReplaceChar('\\', '/');
  #endif  

  *output<<"<IMG SRC=\""<<exRelName<<"\" ALT=\""<<
    ((Image.pibName.Len())?Image.pibName:exFileName)
    <<"\"";
  if(Flags & r2hUseImageSize)
    *output<<" WIDTH=\""<<Image.Width<<"pt\" HEIGHT=\""<<Image.Height<<"pt\"";
    *output<<">";


    SGTRACE("OK\n");
    return ecOK;
}

void CRTF2HTML::FlushPar(CRevString &closingtags, int &P_tag, int Clear){
    int flag=0;
    if(FontStyle.Len() || FontClass.Len()){
        if(TableStack.Count()){ // we are inside of some table...
            if(!TableStack.Last().CellText.Len() &&       // output not started yet
                !TableStack.Last().CellParClass.Len() &&  // and no cell class and style
                !TableStack.Last().CellParStyle.Len()){   // defined yet...
                TableStack.Last().CellParClass<<FontClass;
                TableStack.Last().CellParClass<<FontStyle;
                flag=1;
            };
        }
        if(!flag){
            if(P_tag) *output<<"\n<"PAR; else *output<<"<SPAN";
            ClassStyle2HTML(*output, FontClass, FontStyle);
            *output<<'>';
            if(!P_tag) closingtags<<"</SPAN>"; //else closingtags<<"</"PAR">";
        }
        if(Clear){ 
            FontStyle.Truncate();
            FontClass.Truncate();
        }
        P_tag=0;
    }
}

void CRTF2HTML::FlushList(){
    if(ListText.Len() && !InListText){ 
        *output<<ListText; 
        ListText.Truncate();
    }
}

void CRTF2HTML::PushTable(){
    SGTRACE("(PUSHT");
    TableStack.Expand(); // creating new array entry
    TableStack.Last().Tag=tsNone;
    TableStack.Last().PrevOutput=output;
    output=&TableStack.Last().CellText;    
    SGTRACE(")");
}

void CRTF2HTML::PopTable(){
    SGTRACE("(POPT");
    output=TableStack.Last().PrevOutput;
    *output<<"</TABLE>";
    TableStack.Pop();
    SGTRACE(")");
}

int CRTF2HTML::Parser(){
    size_t     i;
    int        endch,       // "offending" character buffer
               in_P=0,      // we are inside <P> Tag
               P_tag=0,     // we have some options for <P> tag
               code=ecOK,   // function exit code
               NestedTags=0,// allow nested table tags inside this instance
               ShpPict=0,
               SaveListLevel=ListLevel;//list level outside block being parsed
    CRevString closingtags; // closing tags on exit from this block
    CgString   text,        // read tag/text string
               tagtext,
               *saveout;
    long       kwdpar;      // read tag parameter
    TSeqType   stype;       // read sequence type (tag/string/empty)

    SGVERB("(+");

    if(NextListLevel>ListLevel) // STARTING NEW (SUB)LIST
        ListLevel=NextListLevel; // set current ListLevel to higher value
        // !!! open <OL> or <UL> tags here

    while(!Eof()){
        stype=LoadSequence(text, &kwdpar, &endch);
	
	  #if defined(R2H_VERBOSE)
	       SGVERB(text);
               if(kwdpar!=ParNotUsed){
                   char parbuf[32]; sprintf(parbuf, "%d", kwdpar); SGVERB(parbuf);
               }    
	       SGVERB(" ");
	  #endif       

        if(stype==stString){
            FlushPar(closingtags, P_tag, 1);
            if(text.Len()) FlushList();
            Text2HtmlWS(*output, text); // output the string
            continue; }

        if(stype==stKeyword){
            // looking for the keyword in symbol table
            for(i=0; i<RtfSymCount; i++)
                if(!strcmp(text, RtfSym[i].Keyword)) break;
            if(i>=RtfSymCount){ // keyword not found
                // skip unrecognized keywords starting from '*' as it is
                // required by RTF specification
                if(text[size_t(0)]=='*'){ SkipBrackets(); break; }
                continue;
            };

            if(RtfSym[i].Divider) kwdpar/=RtfSym[i].Divider;

            // processing as keyword found in table at position i
            switch(RtfSym[i].Action){
                case saIgnore: break; // just quickly ignore just this keyword

		case saUTF: UnicodeChar(output, kwdpar); break;

                // replace this keyword with a string or tag
                case saTag:
                    FlushPar(closingtags, P_tag, 1);
                    FlushList();
                    if(!kwdpar && RtfSym[i].NoZeros()) break;
                    *output<<RtfSym[i].A;
                    if(RtfSym[i].B!=NULL) *output<<kwdpar<<RtfSym[i].B;
                    if(RtfSym[i].C!=NULL) closingtags<<RtfSym[i].C;
                    break;

                case saFont:
                    UsedFonts.InSet(int(kwdpar));
                    FontClass<<RtfSym[i].A<<kwdpar<<RtfSym[i].B;
                    break;

                case saFontSize: UsedFontSizes.InSet(int(kwdpar)); // no break

                case saFontClass:
                    FontClass<<RtfSym[i].A;
                    if(RtfSym[i].ZeroAction==zaUseB){
                         if(!kwdpar) FontClass<<RtfSym[i].B; break; }
                    if(RtfSym[i].B!=NULL) FontClass<<kwdpar<<RtfSym[i].B;
                    break;

                case saFontStyle:
                    if(!kwdpar && RtfSym[i].NoZeros()) break;
                    FontStyle<<RtfSym[i].A;
                    if(RtfSym[i].ZeroAction==zaUseB){
                         if(!kwdpar) FontClass<<RtfSym[i].B; break; }
                    if(RtfSym[i].B!=NULL) FontStyle<<kwdpar<<RtfSym[i].B;
                    break;

                case saFontColor:
                    FontClass<<RtfSym[i].A<<kwdpar<<RtfSym[i].B;
                    FontColors.InSet(int(kwdpar));
                    break;

                case saFontBg:
                    FontClass<<RtfSym[i].A<<kwdpar<<RtfSym[i].B;
                    BgColors.InSet(int(kwdpar));
                    break;

                case saParClass: case saParStyle:{
                    if(!kwdpar && RtfSym[i].NoZeros()) break;
                    if(in_P && !P_tag){
                         *output<<closingtags; 
                         closingtags.Truncate();
                         in_P=0; 
	            }		 

                    // putting arguments to par style or par class
                    CgString &outstr=(RtfSym[i].Action==saParClass)? FontClass:FontStyle;
                    outstr<<RtfSym[i].A;
                    if(RtfSym[i].B!=NULL) outstr<<kwdpar<<RtfSym[i].B;
                    P_tag=1;
                    in_P=1;
                    break; }

                case saPar:
                    FlushPar(closingtags, P_tag, 1);
                    FlushList();
                    if(in_P){ // if we have paragraph tag open, close it now
                         *output<<closingtags; closingtags.Truncate();
                         in_P=0;
                    }else *output<<"<BR>";
                    break;


        // #########################################################################
        // # Images
        // #########################################################################

                case saShpPict:
                    FlushPar(closingtags, P_tag, 1);
                    FlushList();
                    InShpPict=1;
                    ShpPict=1;
                    ImageUsable=0;
                    ShpPictUsed=0;
                    break;

                case saPict: 
                    FlushPar(closingtags, P_tag, 1);
                    FlushList();
                    if((code=ParseImage())!=ecOK) goto ExitParsingBlock;
                    SkipBrackets();
                    if(InShpPict && ImageUsable) ShpPictUsed=1;
                    goto ExitParsingBlock;

                case saNonShpPict:
                    if(ShpPictUsed){ // we had already used original image file for this picture 
                        ShpPictUsed=0;
                        SkipBrackets();
                        goto ExitParsingBlock; // after skipping brackets, we have nothing to do here
                    }
                    break;

        // #########################################################################
        // # Tables
        // #########################################################################

                case saNested:
                    NestedTags=1;
                    break;
                case saTrowd:
                    // if TableStack.Count()==0 - create new (1) table level
                    if(!NestedTags && TableStack.Count()>1)
                        PopTable();

                    if(!TableStack.Count()) 
                    PushTable();
                    break;
                case saItap:
                    // if kwdpar>TableStack.Count() - create new table level
                    // else if kwdpar<TableStack.Count() - close table level
                    if(size_t(kwdpar)>TableStack.Count())
                        PushTable();
                    else if(size_t(kwdpar)<TableStack.Count())
                        PopTable();
                    break;
                case saCell:
                    // print out cell output buffer enclosed in <TD>..</TD>
                    if(TableStack.Count()){
                        TableStack.Last().ApplyCellTag(closingtags);
                    }
                    break;
                case saRow:
                    // delete cell output buffer, mark row end
                    if(TableStack.Count()){
                        TableStack.Last().ApplyRowTag();
                    }
                    break;

        // #########################################################################
        // # Lists
        // #########################################################################

                case saListText: // this is a list item start
                    saveout=output;
                    output=&ListText;
                    InListText=1;
                    Parser();
                    InListText=0;
                    output=saveout;
                    goto ExitParsingBlock;

                case saListLevel: NextListLevel=int(kwdpar); break;

                // skip this keyword and the whole sequence {}
                case saSkip: SkipBrackets(); goto ExitParsingBlock;


        // #########################################################################
        // # Global document properties
        // #########################################################################

                case saFontTable:  ParseFontTable();  break;
                case saColorTable: ParseColorTable(); break;

                case saHeadTag:
                    tagtext.Truncate();
                    LoadTextValue(tagtext);
                    HeadTags<<RtfSym[i].A;
                    Text2HtmlWS(HeadTags, tagtext);
                    HeadTags<<RtfSym[i].B;
                    break;

                case saHeadTagDig:
                    HeadTags<<RtfSym[i].A<<kwdpar<<RtfSym[i].B;
                    break;

                case saBodyFont:
                    BodyStyle<<RtfSym[i].A<<kwdpar<<RtfSym[i].B;
                    UsedFonts.InSet(int(kwdpar));
                    break;

            }
        }; // if(stype==stKeyword)
        if(endch=='}'){ GetCh(); break; } // the block is finished, exit
        if(endch=='{'){
            // start sub-block parsing
            GetCh(); // eat the bracket
            FlushPar(closingtags, P_tag, 1); // output opening HTML tag

            code=Parser();
            if(code!=ecOK) goto ExitParsingBlock;

        // Check if the Parser have found ShpPict and set ImageUsable to 1. In this case,
        // we'll have to SkipBrackets next \nonshppict tag. 

        }
    } // while

    // successful exit from the parser
    // close open tags, create paragraph "frames" etc.
ExitParsingBlock:
    FlushPar(closingtags, P_tag, 1);
    *output<<closingtags; closingtags.Truncate();

    NextListLevel=(ListLevel=SaveListLevel);

    if(ShpPict) InShpPict=0;

    SGVERB((code!=0)?" ERROR +)":"+)");
    return code;
}




int CRTF2HTML::Process(const char *rtf, const char *html){
    int code=0;
	long lSize;
	size_t numRead;
	char *rtfBuffer, *htmlBuffer;
	OutFile = html;
    FILE *InHandle=fopen(rtf, "rt"); // RTF is a text format (7-bit ASCII)
    if(InHandle==NULL){
        SGTRACE("Cannot open input file: "); SGTRACE(rtf);
        return ecCannotOpenFile;}

    FILE *OutHandle=fopen(html, "wt"); // HTML is a text format (8-bit ASCII)
    if(OutHandle==NULL){
        SGTRACE("Cannot open output file: "); SGTRACE(html);
        return ecCannotCreateFile;}

	// obtain file size:
	fseek(InHandle, 0, SEEK_END);
	lSize = ftell(InHandle);
	rewind(InHandle);

	// allocate memory to contain the whole file:
	rtfBuffer = (char*)malloc(sizeof(char)*lSize);
	if (rtfBuffer == NULL) {
        SGTRACE("Cannot allocate input buffer: "); SGTRACE(html);
        return ecCannotAllocateInputBuffer;}

	// copy the file into the buffer:
	numRead = 0;
	while(!feof(InHandle) && numRead < (size_t)lSize)
	{
		numRead += fread(rtfBuffer, sizeof(char), lSize, InHandle);
	}

	code = ProcessBuffer(rtfBuffer, lSize, &htmlBuffer);
	if (code == ecOK) {
		// write html buffer to file
		lSize = strlen(htmlBuffer);
		if (fwrite(htmlBuffer, sizeof(char), lSize, OutHandle) < (size_t)lSize) {
			SGTRACE("Error writing output file: "); SGTRACE(html);
			return ecErrorWritingOutputFile;}
	}

	if (htmlBuffer != NULL)
		free(htmlBuffer);
    fclose(InHandle); fclose(OutHandle);
    return code;

}
int CRTF2HTML::ProcessBuffer(const char *rtf, long len, char **html)
{
	InBuffer = (char*)rtf;
	InIndex = 0;
	InLength = len;
    int code=0;

    SGTRACE("Initializing Parser...\n");

    HeadTags.Truncate();
    Style.Truncate();

    BodyStyle="font-size: 12pt; color: black; background-color: white; ";

    ListText.Truncate();
    ListLevel=0;
    NextListLevel=0;
    InListText=0;

    Fonts.Clear();
    UsedFontSizes.Clear();
    BgColors.Clear();
    FontColors.Clear();
    ColorTable.Clear();

    TableStack.Clear();
//    TableStack.Expand(1); // creating item #0

    FontStyle.Truncate(); FontClass.Truncate(); 
    InShpPict=0; ImageUsable=0; ShpPictUsed=0;

    CgString data;
    output=&data;
    // #SG2 converted single call to loop - premature end of parsing workaround    
    while(!code && !Eof()){ 
      SGTRACE("Starting Parser...\n");
      code=Parser();
      SGTRACE("\nParser exit. ");
    }  
    SGTRACE("File parsing complete.\n");

	CgString cstr;
    // Saving HTML...
    SGTRACE("Saving HTML...\n");
    cstr<<"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Strict//EN\">\n<HTML>\n<HEAD>\n";
    cstr<<"<META HTTP_EQUIV=\"Generator\" CONTENT=\"" R2H_GENERATOR "\">\n";
    cstr<<HeadTags;

    // Stylesheet
    cstr<<"<STYLE TYPE=\"text/css\"><!--\n";
    UpdateStyle();
    cstr<<Style;
    cstr<<"--></STYLE>\n";
    // Body
    cstr<<"</HEAD>\n<BODY>\n"; cstr<<data; cstr<<"\n\n</BODY>\n</HTML>\n\n";

	int length = cstr.Len();
	(*html) = (char*)malloc(length+1);
	sprintf(*html, "%s", (const char*)cstr);

    SGTRACE("Processing complete.\n");
    return code;
}

void CRTF2HTML::UpdateStyle(){
    size_t i, j;
    char strbuf[16];    
    SGVERB("Update stylesheet...");
    Style<<
        "p{ margin: 0; }\n"
        "table{ border-collapse: collapse; border: none; }\n"
        "td{ border: solid 1px black; }\n"
        ".lft{ text-align: left; }\n"
        ".rgt{ text-align: right; }\n"
        ".cen{ text-align: center; }\n"
        ".jus{ text-align: justify; }\n"
    #if !defined(USE_FONT_TAGS)
        ".b{ font-weight: bold; }\n"
        ".i{ font-style: italic; }\n"	
    #endif
        ".wn{ font-weight: normal; }\n"
        ".sn{ font-style: normal; }\n"
        ".u{ text-decoration: underline; }\n"
        ".lst{ font-family: Wingdings, fantasy; }\n"
        ".strike{ text-decoration: line-through; }\n";
    for(i=0; i<UsedFontSizes.Count(); i++)
        Style<<".sz"<<UsedFontSizes[i]<<"{ font-size: "
             <<UsedFontSizes[i]<<"pt; }\n";
    // printing lines from Fonts that contain definitions
    for(j=0; j<UsedFonts.Count(); j++){
        sprintf(strbuf, ".f%d", int(UsedFonts[j]));
        int len=strlen(strbuf);
        for(i=0; i<Fonts.Count(); i++)
            if(!strncmp(strbuf, Fonts[i], len)){
                Style<<Fonts[i];
                break; }
    }
    // printing colors and background colors
    for(i=0; i<FontColors.Count(); i++)
        if(FontColors[i]>1 && FontColors[i]<=(int)ColorTable.Count())
            Style<<".cl"<<FontColors[i]<<" { color: "<<ColorTable[FontColors[i]-1]<<"; }\n";
    for(i=0; i<BgColors.Count(); i++)
        if(BgColors[i]>1 && BgColors[i]<=(int)ColorTable.Count())    
            Style<<".cb"<<BgColors[i]<<" { background-color: "<<ColorTable[BgColors[i]-1]<<"; }\n";
    Style<<"body{ "<<BodyStyle<<" }\n";
    SGVERB("Stylesheet done.\n");
//    Style<<Fonts;
}


EXPORTED_FN(int) Convert(const char *rtf, const char *html, r2hFlags Flags, const char *imgdir){
    CRTF2HTML converter(Flags, imgdir);
    //CgString msg; msg<<"Flags: "<<long(Flags)<<" Image dir: "<<imgdir;
    //MessageBox(0, msg, "Vo", MB_OK);
    SGTRACE("Convert()\n");
    return converter.Process(rtf, html);
}
EXPORTED_FN(int) ConvertBuffer(const char *rtf, long len, char **html, r2hFlags Flags, const char *imgdir){
    CRTF2HTML converter(Flags, imgdir);
    //CgString msg; msg<<"Flags: "<<long(Flags)<<" Image dir: "<<imgdir;
    //MessageBox(0, msg, "Vo", MB_OK);
    SGTRACE("Convert()\n");
    return converter.ProcessBuffer(rtf, len, html);
}
EXPORTED_FN(char*) r2hAboutMessage(){
    return R2H_ABOUT;
}
