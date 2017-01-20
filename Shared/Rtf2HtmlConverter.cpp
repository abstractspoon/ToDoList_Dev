// Rtf2HtmlConverter.cpp: implementation of the CRtf2HtmlConverter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Rtf2HtmlConverter.h"

#include "..\shared\mswordhelper.h"
#include "..\shared\filemisc.h"
#include "..\shared\misc.h"

#include "..\3rdparty\html2rtf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const char* RTFTAG = "{\\rtf"; // always in bytes
const int LENTAG = strlen(RTFTAG); // in bytes

//////////////////////////////////////////////////////////////////////

const CString HTML2RTF_FNAME(_T("Html2Rtf"));
const CString RTF2HTML_FNAME(_T("Rtf2Html"));
const CString INVALID_IMGSRC(_T("src=\"C:/"));
const CString VALID_IMGSRC(_T("src=\"file:///C:/"));

//////////////////////////////////////////////////////////////////////

// Sautin RTF to HTML
struct sautin_rtf2html_options
{
	int html_type;				//0-XHTML, 1- HTML, 2-HTML+CSS
	char title[120];			//Page title	
	char extension[10];			//for example, ".html"
	char font_face[50];			//for example, "Arial" or "Times New Roman"
	char font_size[10];			//for example ,"10"
	BOOL preserve_images;		//0-no, 1 - yes 
	BOOL preserve_font_face;		//0-no, 1 - yes 
	BOOL preserve_font_size;		//0-no, 1 - yes 
	BOOL preserve_font_color;	//0-no, 1 - yes 
	BOOL preserve_hyperlinks;	//0-no, 1 - yes
	int table_borders;			//0-invisble, 1 - visible
	int encoding;				//as above
	int html_parts;				//0-html Complete, 1 - only between <body>..</body>
	char image_folder[500];		//folder where image will be stored
	char CSS_style_name[120];	//style name, for example "ts"
	int font_sizes_type;		//0-normal sizes, 1 - medium sizes, 2 - big sizes, 3 - huge sizes
	char image_subfolder[120];	//will create subfolder for storing images, if ""-empty string then images will be strored in 'image_folder' 
	
	enum
	{
		Windows_1250	= 0, 
		Windows_1251	= 1, 
		Windows_1252	= 2, 
		Windows_1253	= 3,
		Windows_1254	= 4, 
		Windows_1255	= 5, 
		Windows_1256	= 6, 
		Windows_1257	= 7,
		Windows_1258	= 8, 
		ISO_8859_1		= 9,	
		ISO_8859_2		= 10,	
		ISO_8859_3		= 11,
		ISO_8859_4		= 12,	
		ISO_8859_5		= 13,	
		ISO_8859_6		= 14,	
		ISO_8859_7		= 15,
		ISO_8859_8		= 16,	
		ISO_8859_9		= 17,	
		KOI8_R			= 18,
		UTF_8			= 19,
	};
};

//////////////////////////////////////////////////////////////////////

// Sautin HTML to RTF

struct sautin_html2rtf_options
{		
	BOOL PreserveTables;			//1 - preserve tables, 0 - transfer to text
	BOOL PreserveImages;			//1 - preserve images,  0 - skip
	BOOL PreserveHyperlinks;		//1 - preserve hyperlinks, 0 - skip
	BOOL PreserveFontFace;			//1 - preserve font faces as in HTML, 0 - all font faces will be as in 'FontFace'
	BOOL PreserveFontSize;			//1 - preserve font sizes as in HTML, 0 - all font sizes will be as in 'FontSize'
	BOOL PreserveFontColor;			//1 - preserve font colors as in HTML, 0 - black font color
	BOOL PreserveBackgroundColor;	//1 - preserve bacground colors as in HTML, 0 - skip
	BOOL PreserveAlignment;			//1 - preserve alignment as in HTML, 0 - all text will have align as in 'PageAlignment'
	BOOL PreserveTableWidth;			//1 - preserve width of columns
	BOOL PreserveNestedTables;		//1 - preserve nested tables, 0 - translate nested tables to plain tables
	int PageMarginLeft;				//page margin left, mm. For example 10
	int PageMarginRight;			//page margin right, mm. For example 10
	int PageMarginTop;				//page margin top, mm. For example 10
	int PageMarginBottom;			//page margin bottom, mm. For example 10
	int BorderVisibility;			//table borders: 1 - visible borders, 0 - hidden borders, 2 - as in HTML
	int PageOrientation;			//page orientation: 0 - Portrait, 1 - Landscape
	int PageSize;					//page size: 0 - A4, 1 - A3, 2 - A5, 3 - B5, 4 - Letter, 5 - Legal, 6 - Executive, 7 - Monarh
	int FontFace;					//default font face: Arial - 0, Times New Roman - 1, Verdana - 2, Helvetica - 3, Courier - 4, Courier New - 5, Times - 6, Georgia - 7, MS Sans Serif - 8,
									//Futura - 9, Arial Narrow - 10, Garamond - 11, Impact - 12, Lucida Console - 13, Tahoma - 14, Inform - 15, Symbol - 16, WingDings - 17, Traditional Arabic - 18
	int FontSize;					//default font size, any value from 6 to 72
	int PageAlignment;				//page alignment: 0 - left, 1 - center, 2 - right, 3 - justify
	int RtfLanguage;				//RTF language: English - 1033, Albanian - 1052, Belgian - 2067, Bulgarian - 1026, Hungarian - 1038, Danish - 1030, Spanish - 3082, Latvian - 1062, Lithuanian - 1063,
									//German - 1031, Netherlands - 1043, Norwegian - 2068, Portuguese - 2070, Romanian - 1048, Russian - 1049, Ukrainian - 1058, Finnish - 1035, French - 1036,
									//Czech - 1029, Swedish - 1053, Arabic - 1053, Turkish - 1055, Japanese - 932, SimplifiedChinese - 936, TraditionalChinese - 950, Korean - 949, Thai = 874
	int Encoding;					//AutoSelect - 0, ISO-8859-1 - 1, ISO-8859-5 - 2, KOI8-R - 3, Windows-1251 - 4, UTF-8 - 5, Windows-1254 - 6, Windows-1256 - 7,
									//Windows-1250 - 8, Windows-1252 - 9, Windows-1253 - 10, Windows-1255 - 11, Windows-1257 - 12, Windows-1258 - 13
	int OutputTextFormat;			//Output Format: Rtf - 0, Text - 1, Doc - 2 (only file with .doc extension)  
	BOOL PreservePageBreaks;		//1 - preserve page-breaks
	int ImageCompatible;			//type of produced images: Word - 0, WordPad - 1
	int PageNumbers;				//page numbers: 0 - disable, 1 - numbers from first page, 2 - from second page
	char PageHeader[150];			//page header, any string
	char PageFooter[150];			//page footer, any string
	char HtmlPath[650];				//html path for method 'htmltortf_string', will be used for find images (now is not used)
	int PageNumbersAlignV;			//page numbers vertical align: Top - 4, Bottom - 5
	int PageNumbersAlignH;			//page numbers horizontal align: 0 - left, 1 - center, 2 - right, 3 - justify
	BOOL PreserveHR;				//1 - preseve <hr>, 0 - skip
	int RtfParts;					//0 - rtf completely, 1 - only rtf body (to insert inside another rtf files)
	BOOL CreateTraceFile;			//1 - the component will create trace file, it helps to see how converting goes and shows errors
	char TraceFilePath[650];		//specifies path for trace file, for example "c:\\Trace.txt"
	int TableCellPadding;			//specifies table cell padding in pixels, any value from 0 to 10
	BOOL PreserveHttpImages;		//1 - download remote images, 0 - skip remote images

	enum eBorderVisibility
	{
		Hidden				= 0,
		Visible				= 1,		
		SameAsOriginalHtml	= 2	
	};

	enum eFontFace
	{
		f_Arial				= 0,
		f_Times_New_Roman	= 1,
		f_Verdana			= 2,
		f_Helvetica			= 3,
		f_Courier			= 4,
		f_Courier_New		= 5,
		f_Times				= 6,
		f_Georgia			= 7,
		f_MS_Sans_Serif		= 8,
		f_Futura			= 9,
		f_Arial_Narrow		= 10,
		f_Garamond			= 11,
		f_Impact			= 12,
		f_Lucida_Console	= 13,
		f_Tahoma			= 14,
		f_Inform			= 15,
		f_Symbol			= 16,
		f_WingDings			= 17,
		f_Traditional_Arabic= 18
			
	};

	enum eEncoding
	{
		AutoSelect		= 0,
		ISO_8859_1		= 1,
		ISO_8859_5		= 2,
		KOI8_R			= 3,
		Windows_1251	= 4,
		UTF_8			= 5,
		Windows_1254	= 6,
		Windows_1256	= 7,
		Windows_1250	= 8,
		Windows_1252	= 9,
		Windows_1253	= 10,
		Windows_1255	= 11,
		Windows_1257	= 12,
		Windows_1258	= 13
	};

	enum ePageAlignment
	{
		AlignLeft	= 0,
		AlignCenter	= 1,
		AlignRight	= 2,
		AlignJustify= 3,
		AlignTop	= 4,
		AlignBottom	= 5
	};

	enum eOutputTextFormat
	{
		Rtf	= 0,
		Text= 1,		
	};

	enum eLanguage
	{
		l_Albanian			= 1052,
		l_English			= 1033,
		l_Belgian			= 2067,
		l_Bulgarian			= 1026,
		l_Hungarian			= 1038,
		l_Danish			= 1030,
		l_Spanish			= 3082,
		l_Latvian			= 1062,
		l_Lithuanian		= 1063,
		l_German			= 1031,
		l_Netherlands		= 1043,
		l_Norwegian			= 2068,
		l_Portuguese		= 2070,
		l_Romanian			= 1048,
		l_Russian			= 1049,
		l_Ukrainian			= 1058,
		l_Finnish			= 1035,
		l_French			= 1036,
		l_Czech				= 1029,
		l_Swedish			= 1053,
		l_Arabic			= 1053,
		l_Turkish			= 1055,
		l_Japanese			= 932,
		l_SimplifiedChinese = 936,
		l_TraditionalChinese = 950,
		l_Korean			= 949,
		l_Thai				= 874
			
	};

	//Page Orientation
	enum ePageOrientation
	{
		Portrait	= 0,
		Landscape	= 1,
	};

	//Page Size
	enum ePageSize
	{
		A4			= 0,
		A3			= 1,
		A5			= 2,
		B5			= 3,
		Letter		= 4,
		Legal		= 5,
		Executive	= 6,
		Monarh		= 7
	};

	enum eImageCompatible
	{
		image_Word		= 0,
		image_WordPad	= 1,
	};

	enum ePageNumbers
	{
		PageNumDisable	= 0,
		PageNumFirst	= 1,
		PageNumSecond	= 2
	};
};

//////////////////////////////////////////////////////////////////////

typedef int (*FNRTF2HTML_FILE)(char* rtfFile, char* htmlFolder, struct sautin_rtf2html_options);
typedef char* (*FNRTF2HTML_STRING)(char* rtf, char* html, struct sautin_rtf2html_options);

typedef int (*FNHTML2RTF_FILE)(char* htmlFile, char* rtfFolder, struct sautin_html2rtf_options);
typedef char* (*FNHTML2RTF_STRING)(char* html, char* rtf, struct sautin_html2rtf_options);

typedef int (*FNFLUSHLIST)(); //release memory

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRtfHtmlConverter::CRtfHtmlConverter(BOOL bAllowUseOfMSWord) 
	: 
	m_pWordHelper(NULL),
	m_bAllowMSWordUse(bAllowUseOfMSWord)
{

}

CRtfHtmlConverter::~CRtfHtmlConverter()
{
	Release();
}

BOOL CRtfHtmlConverter::IsRTF(const char* szRTF)
{
	return (strncmp(szRTF, RTFTAG, LENTAG) == 0);
}

void CRtfHtmlConverter::SetAllowUseOfMSWord(BOOL bAllow)
{
	m_bAllowMSWordUse = bAllow;

	if (!bAllow && m_pWordHelper)
	{
		delete m_pWordHelper;
		m_pWordHelper = NULL;
	}
}

void CRtfHtmlConverter::Release()
{
	delete m_pWordHelper;
	m_pWordHelper = NULL;
	
	CleanupTemporaryImages();
}

void CRtfHtmlConverter::CleanupTemporaryImages()
{
	int nFolder = m_aTempImageFolders.GetSize();
	
	while (nFolder--)
	{
		const CString& sFolder = m_aTempImageFolders[nFolder];
		
		if (FileMisc::DeleteFolder(sFolder))
			m_aTempImageFolders.RemoveAt(nFolder);
	}
}

BOOL CRtfHtmlConverter::ConvertRtfToHtml(LPCSTR szRTF, LPCTSTR szCharSet, CString& sHtml, LPCTSTR szImageDir)
{
	// sanity check
	if (Misc::IsEmpty(szRTF))
	{
		ASSERT(0);
		return FALSE;
	}

	CString sImageDir(szImageDir), sUniqueDir;
	BOOL bTempImageDir = sImageDir.IsEmpty();
	
	if (bTempImageDir)
		sImageDir = FileMisc::GetTempFolder();
	
	// create a unique image folder every time this is called for this session
	static int nImgCount = 1;
	
	FileMisc::TerminatePath(sImageDir);
	sUniqueDir.Format(_T("%simg%d"), sImageDir, nImgCount++);

	FileMisc::DeleteFolder(sUniqueDir);
	FileMisc::CreateFolder(sUniqueDir);
	
	// track temp image folders for later cleanup
	if (bTempImageDir)
		m_aTempImageFolders.Add(sUniqueDir);
	
	// save rtf as temp Ansi file
	CString sTempRtf = FileMisc::GetTempFilePath(RTF2HTML_FNAME, _T("rtf")), sTempHtml;
	FileMisc::SaveFile(sTempRtf, szRTF); 

	// do the conversion
 	if (ConvertRtfToHtmlWithSautin(sTempRtf, szCharSet, sHtml, sUniqueDir))
  	{
  		// TODO
  	}
 	else if (ConvertRtfToHtmlWithMSWord(sTempRtf, szCharSet, sHtml, sUniqueDir))
 	{
 		// TODO
 	}
 	else if (ConvertRtfToHtmlWithIntenso(sTempRtf, szCharSet, sHtml, sUniqueDir))
	{
		// TODO
	}


	// cleanup temp rtf file
	// NOTE: we clean up temporary image folders when we close
	DeleteFile(sTempRtf);
	
	// cleanup empty 'permanent' image folders.
	// This will fail intentionally if the folder contains anything
	if (!bTempImageDir)
		RemoveDirectory(sUniqueDir);

	return !sHtml.IsEmpty();
}

BOOL CRtfHtmlConverter::ConvertRtfToHtmlWithMSWord(LPCTSTR szRtfFile, LPCTSTR /*szCharSet*/, CString& sHtml, LPCTSTR szUniqueDir)
{
	if (!m_bAllowMSWordUse)
		return FALSE;

	BOOL bSuccess = FALSE;

	// initialize first time
	if (m_pWordHelper == NULL)
		m_pWordHelper = new CMSWordHelper(12);
	
	if (m_pWordHelper->IsValid())
	{
		// html file must be placed inside the unique folder
		CString sTempHtml(szUniqueDir);
		FileMisc::ReplaceExtension(sTempHtml, _T("html"));
	
		// Make sure Word doesn't consider either of these file to be 'dodgy'
		VERIFY(CMSWordHelper::EnableDisabledDocument(szRtfFile));
		VERIFY(CMSWordHelper::EnableDisabledDocument(sTempHtml));
		
		// prepare file system for Word else it will spit
		::DeleteFile(sTempHtml);

		bSuccess = m_pWordHelper->ConvertFile(szRtfFile, WordAPI::wdOpenFormatRTF, 
												sTempHtml, WordAPI::wdSaveFormatFilteredHTML,
												WordAPI::msoEncodingUTF8);
		
		if (bSuccess && FileMisc::LoadFile(sTempHtml, sHtml))
		{
			bSuccess = (ProcessMSWordHtml(szUniqueDir, sHtml) > 0);
		}

		// cleanup temp file
		::DeleteFile(sTempHtml);
	}

	return bSuccess;
}

BOOL CRtfHtmlConverter::ConvertRtfToHtmlWithIntenso(LPCTSTR szRtfFile, LPCTSTR szCharSet, CString& sHtml, LPCTSTR szUniqueDir)
{
	BOOL bSuccess = FALSE;
	
	CString sRtf2HtmlPath = FileMisc::GetAppFolder() + _T("\\rtf2htmlbridge.dll");
	static HMODULE hMod = LoadLibrary(sRtf2HtmlPath);
	
	CString sTempHtml = FileMisc::GetTempFilePath(RTF2HTML_FNAME, _T("html"));

	if (hMod)
	{
		typedef int (*PFNCONVERTRTF2HTML)(LPCTSTR, LPCTSTR, LPCTSTR, 
											LPCTSTR, LPCTSTR, LPCTSTR, 
											LPCTSTR, LPCTSTR, LPCTSTR, 
											LPCTSTR, LPCTSTR);
		
		try
		{
			PFNCONVERTRTF2HTML fnRtf2Html = (PFNCONVERTRTF2HTML)GetProcAddress(hMod, "fnRtf2Html");
			
			if (fnRtf2Html)
			{
				// arguments
				CString sCharSet = _T("/CS:") + CString(szCharSet);
	
				CString sImageDir;
				sImageDir.Format(_T("/ID:%s"), szUniqueDir);
				
				bSuccess = fnRtf2Html(szRtfFile,					// rtf input file
										FileMisc::GetTempFolder(),  // folder for html output file
										sImageDir,					// folder for saving images
										_T("/IT:png"),				// image file format
										_T("/DS:content"),			// return only body of html
										sCharSet,					// character set
										_T(""),						// unused
										_T(""),						// unused 
										_T(""),						// unused 
										_T(""),						// unused 
										_T(""));					// unused
				
				if (bSuccess)
					bSuccess = (FileMisc::LoadFile(sTempHtml, sHtml) && !sHtml.IsEmpty());
			}
		}
		catch (...)
		{
			bSuccess = FALSE;
		}
	}

	// cleanup
	DeleteFile(sTempHtml);

	return bSuccess;
}

BOOL CRtfHtmlConverter::ConvertRtfToHtmlWithSautin(LPCTSTR szRtfFile, LPCTSTR /*szCharSet*/, CString& sHtml, LPCTSTR szUniqueDir)
{
	// load library
	HINSTANCE hMod = LoadLibrary(_T("rtftohtml_sautinsoft.dll"));
	
	if (hMod==NULL) 
		return FALSE;
	
	// get function convert
	FNRTF2HTML_FILE pfnConvert = (FNRTF2HTML_FILE)GetProcAddress(hMod, "rtftohtml_file");
	ASSERT(pfnConvert);

	if (!pfnConvert)
		return FALSE;

	// get function to release memory
	FNFLUSHLIST pfnFlush = (FNFLUSHLIST)GetProcAddress(hMod, "flushlist");

	if (!pfnFlush)
		return FALSE;

	CString sOutputDir(szUniqueDir);
	FileMisc::TerminatePath(sOutputDir);

	CString sTempHtml(sOutputDir + _T("Rtf2Html.htm"));

	// output folder must be ansi
#ifdef _UNICODE
	Misc::EncodeAsMultiByte(sOutputDir, CP_UTF8);
#endif

	sautin_rtf2html_options opt = { 0 };
	
	strcpy(opt.image_folder, (LPCSTR)(LPCTSTR)sOutputDir);
	strcpy(opt.extension, ".htm");
	strcpy(opt.font_face, "Verdana");
	strcpy(opt.font_size, "10");
	strcpy(opt.title,"My Page");

	opt.encoding			= sautin_rtf2html_options::UTF_8;
	opt.html_type			= 1;
	opt.preserve_font_color	= TRUE;
	opt.preserve_font_face	= TRUE;
	opt.preserve_font_size	= TRUE;
	opt.preserve_images		= TRUE;
	opt.table_borders		= 1;
	opt.html_parts			= 1; //only between <body>...</body>

	// input file must be ansi encoded
	CString sRtfFile(szRtfFile);
#ifdef _UNICODE
	Misc::EncodeAsMultiByte(sRtfFile, CP_UTF8);
#endif
	
	// converting
	if (0 == pfnConvert((LPSTR)(LPCTSTR)sRtfFile, (LPSTR)(LPCTSTR)sOutputDir, opt))
	{
		VERIFY(FileMisc::LoadFile(sTempHtml, sHtml));
	}

	// release memory
	pfnFlush();

	return !sHtml.IsEmpty();
}

BOOL CRtfHtmlConverter::ProcessMSWordHtml(const CString& sUniqueDir, CString& sHtml)
{
	if (sHtml.IsEmpty())
		return FALSE;

	// strip everything but the body
	int nStart = sHtml.Find(_T("<body"));
	ASSERT(nStart >= 0);
	
	// find next end tag
	nStart = sHtml.Find('>', nStart);
	ASSERT(nStart >= 0);
	
	int nEnd = sHtml.Find(_T("</body>"));
	ASSERT(nEnd > nStart);
	
	sHtml = sHtml.Mid(nStart + 1, nEnd - nStart - 1);

	// handle links to images which MSWord puts into a sub-folder 
	// using the filename followed by a localised string meaning 'files'
	if (sHtml.Find(_T("src=\"")) != -1)
	{
		CString sSrcFolderName = FileMisc::GetFileNameFromPath(sUniqueDir);
		
		// insert 'file:///' into image links
		CString sInvalidLink(_T("src=\"") + sSrcFolderName);
		CString sValidLink(_T("src=\"file:///") + sSrcFolderName);
		
		sHtml.Replace(sInvalidLink, sValidLink);
		
		// replace the image file paths with the passed-in unique path
		CString sDstFolderName(sUniqueDir);
		sDstFolderName.Replace('\\', '/');
		
		sHtml.Replace(sSrcFolderName, sDstFolderName);
	}

	// Reduce the size of paragraph breaks
	sHtml.Replace(_T("margin-bottom:0cm"), _T("margin-top:0cm"));

	return !sHtml.IsEmpty();
}

BOOL CRtfHtmlConverter::ConvertHtmlToRtf(const char* szHtml, LPCTSTR szCharSet, CString& sRtf, LPCTSTR szImageDir)
{
	CString sImageDir(szImageDir), sUniqueDir;
	BOOL bTempImageDir = sImageDir.IsEmpty();
	
	if (bTempImageDir)
		sImageDir = FileMisc::GetTempFolder();
	
	// save html as temp Ansi file
	CString sTempHtml = FileMisc::GetTempFilePath(HTML2RTF_FNAME, _T("htm"));
	FileMisc::SaveFile(sTempHtml, szHtml); 
	
	if (ConvertHtmlToRtfWithSautin(sTempHtml, szCharSet, sRtf, sImageDir))
	{
		// TODO
	}
	else if (ConvertHtmlToRtfWithMSWord(sTempHtml, szCharSet, sRtf, sImageDir))
	{
		// TODO
	}
// 	else if (ConvertHtmlToRtfWithViskoe(sTempHtml, szCharSet, sRtf, sImageDir))
// 	{
// 		// TODO
// 	}

	// remove the temp html file
	::DeleteFile(sTempHtml);

	return !sRtf.IsEmpty();
}

BOOL CRtfHtmlConverter::ConvertHtmlToRtfWithMSWord(LPCTSTR szHtmlFile, LPCTSTR /*szCharSet*/, CString& sRtf, LPCTSTR szUniqueDir)
{
	if (!m_bAllowMSWordUse)
		return FALSE;
	
	BOOL bSuccess = FALSE;
	
	// initialize first time
	if (m_pWordHelper == NULL)
		m_pWordHelper = new CMSWordHelper(12);
	
	if (m_pWordHelper->IsValid())
	{
		// rtf file must be placed inside the unique folder
		CString sTempRtf;
		sTempRtf.Format(_T("%s\\%s.rtf"), szUniqueDir, HTML2RTF_FNAME);
			
		// Make sure Word doesn't consider either of these file to be 'dodgy'
		VERIFY(CMSWordHelper::EnableDisabledDocument(szHtmlFile));
		VERIFY(CMSWordHelper::EnableDisabledDocument(sTempRtf));

		// prepare file system for Word else it will spit
		::DeleteFile(sTempRtf);
		
		bSuccess = m_pWordHelper->ConvertFile(szHtmlFile, WordAPI::wdOpenFormatWebPages, 
												sTempRtf, WordAPI::wdSaveFormatRTF,
												WordAPI::msoEncodingUTF8);
		
		if (bSuccess)
			bSuccess = FileMisc::LoadFile(sTempRtf, sRtf);

		// remove the temp rtf file
		::DeleteFile(sTempRtf);
	}
	
	return bSuccess;
}

BOOL CRtfHtmlConverter::ConvertHtmlToRtfWithViskoe(LPCTSTR szHtmlFile, LPCTSTR /*szCharSet*/, CString& sRtf, LPCTSTR /*szUniqueDir*/)
{
	BOOL bSuccess = FALSE;
	CString sHtml;
	
	if (FileMisc::LoadFile(szHtmlFile, sHtml))
	{
		CHtmlToRtf htm2Rtf;
		LOGFONT lf = { 0 };
	
		sRtf = htm2Rtf.Convert(sHtml, lf, 0, 0x00ffffff);
	}
	
	return bSuccess;
}

BOOL CRtfHtmlConverter::ConvertHtmlToRtfWithSautin(LPCTSTR szHtmlFile, LPCTSTR /*szCharSet*/, CString& sRtf, LPCTSTR szUniqueDir)
{
	// load library
	static HINSTANCE hMod = LoadLibrary(_T("htmltortf_sautinsoft.dll"));
	
	if (hMod == NULL) 
		return FALSE;
	
	// get function convert
	FNHTML2RTF_FILE pfnConvert = (FNHTML2RTF_FILE)GetProcAddress(hMod, "htmltortf_file");
	ASSERT(pfnConvert);
	
	if (!pfnConvert)
		return FALSE;
	
	// get function to release memory
	FNFLUSHLIST pfnFlush = (FNFLUSHLIST)GetProcAddress(hMod, "flushlist");
	
	if (!pfnFlush)
		return FALSE;
	
	sautin_html2rtf_options opt = { 0 };
	
	opt.BorderVisibility		= sautin_html2rtf_options::Hidden;
	opt.Encoding				= sautin_html2rtf_options::UTF_8;
	opt.OutputTextFormat		= sautin_html2rtf_options::Rtf;
	opt.FontFace				= 0;
	opt.FontSize				= 10;
	opt.ImageCompatible			= 0;
	opt.PageAlignment			= 0;
	opt.PageMarginBottom		= 10;
	opt.PageMarginLeft			= 10;
	opt.PageMarginRight			= 10;
	opt.PageMarginTop			= 10;
	opt.PageNumbers				= 1;
	opt.PageNumbersAlignH		= 2;
	opt.PageNumbersAlignV		= 5;
	opt.PageOrientation			= 0;
	opt.PageSize				= 4;
	opt.RtfLanguage				= 1033;
	opt.TableCellPadding		= 2;
	
	opt.PreserveAlignment		= TRUE;
	opt.PreserveBackgroundColor	= TRUE;
	opt.PreserveFontColor		= TRUE;
	opt.PreserveFontFace		= TRUE;
	opt.PreserveFontSize		= TRUE;
	opt.PreserveHR				= TRUE;
	opt.PreserveHyperlinks		= TRUE;
	opt.PreserveImages			= TRUE;
	opt.PreserveNestedTables	= TRUE;
	opt.PreservePageBreaks		= TRUE;
	opt.PreserveTables			= TRUE;
	opt.PreserveTableWidth		= TRUE;
	opt.PreserveHttpImages		= TRUE;
	opt.CreateTraceFile			= FALSE;
	
	CString sOutputDir(szUniqueDir);
	FileMisc::TerminatePath(sOutputDir);
	
	CString sTempRtf(sOutputDir + HTML2RTF_FNAME + _T(".rtf"));

	// input file must be ansi encoded
	CString sHtmlFile(szHtmlFile);
#ifdef _UNICODE
	Misc::EncodeAsMultiByte(sHtmlFile, CP_UTF8);
#endif

	// converting
	if (0 == pfnConvert((LPSTR)(LPCTSTR)sHtmlFile, (LPSTR)(LPCTSTR)sTempRtf, opt))
	{
		VERIFY(FileMisc::LoadFile(sTempRtf, sRtf));
	}

	// release memory
	pfnFlush();
	
	return !sRtf.IsEmpty();
}
