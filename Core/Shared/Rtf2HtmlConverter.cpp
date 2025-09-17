// Rtf2HtmlConverter.cpp: implementation of the CRtf2HtmlConverter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Rtf2HtmlConverter.h"
#include "mswordhelper.h"
#include "filemisc.h"
#include "misc.h"

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

//////////////////////////////////////////////////////////////////////

// create a unique image folder every time this is called for this session
static int s_nImgCount = 1;

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
	
	FileMisc::TerminatePath(sImageDir);
	sUniqueDir.Format(_T("%simg%d"), sImageDir, s_nImgCount++);

	FileMisc::DeleteFolder(sUniqueDir);
	FileMisc::CreateFolder(sUniqueDir);
	
	// track temp image folders for later cleanup
	if (bTempImageDir)
		m_aTempImageFolders.Add(sUniqueDir);
	
	// save rtf as temp Ansi file
	CString sTempRtf = FileMisc::GetTempFilePath(RTF2HTML_FNAME, _T("rtf")), sTempHtml;

	// Make sure RTF format is at least 'rtf1'
	if (strncmp(szRTF, "{\\rtf\\", 6) == 0)
	{
		CString sRtf(szRTF);
		//Misc::EncodeAsUnicode(sRtf, CP_UTF8);

		sRtf.Replace(L"{\\rtf\\", L"{\\rtf1\\");
		Misc::EncodeAsMultiByte(sRtf, CP_UTF8);
	
		FileMisc::SaveFile(sTempRtf, (LPCSTR)(LPCWSTR)sRtf); 
	}
	else
	{
		FileMisc::SaveFile(sTempRtf, szRTF);
	}

	// do the conversion
 	if (ConvertRtfToHtmlWithMSWord(sTempRtf, szCharSet, sHtml, sUniqueDir))
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
		
		if (bSuccess && FileMisc::LoadFile(sTempHtml, sHtml, FALSE, CP_UTF8))
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
	HMODULE hMod = LoadLibrary(sRtf2HtmlPath);
	
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
				CString sCharSet;
				
				if (!Misc::IsEmpty(szCharSet))
					sCharSet.Format(_T("/CS:%s"), szCharSet);
	
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

BOOL CRtfHtmlConverter::ConvertHtmlToRtf(LPCSTR szHtml, LPCTSTR szCharSet, CString& sRtf, LPCTSTR szImageDir)
{
	CString sImageDir(szImageDir), sUniqueDir;
	BOOL bTempImageDir = sImageDir.IsEmpty();
	
	if (bTempImageDir)
		sImageDir = FileMisc::GetTempFolder();
	
	// save html as temp Ansi file
	CString sTempHtml = FileMisc::GetTempFilePath(HTML2RTF_FNAME, _T("htm"));
	FileMisc::SaveFile(sTempHtml, szHtml); 
	
	if (ConvertHtmlToRtfWithMSWord(sTempHtml, szCharSet, sRtf, sImageDir))
	{
		// TODO
	}

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
			bSuccess = FileMisc::LoadFile(sTempRtf, sRtf, FALSE, CP_UTF8);

		// remove the temp rtf file
		::DeleteFile(sTempRtf);
	}
	
	return bSuccess;
}

