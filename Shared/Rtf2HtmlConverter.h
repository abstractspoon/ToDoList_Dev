// Rtf2HtmlConverter.h: interface for the CRtf2HtmlConverter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTF2HTMLCONVERTER_H__2F622EC4_0FFE_47E5_B583_637116DE9B2A__INCLUDED_)
#define AFX_RTF2HTMLCONVERTER_H__2F622EC4_0FFE_47E5_B583_637116DE9B2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMSWordHelper;
	
class CRtfHtmlConverter  
{
public:
	CRtfHtmlConverter(BOOL bAllowUseOfMSWord = TRUE);
	virtual ~CRtfHtmlConverter();

	void SetAllowUseOfMSWord(BOOL bAllow);
	void Release();

	BOOL ConvertRtfToHtml(const char* szRtf, LPCTSTR szCharSet, CString& sHtml, LPCTSTR szImageDir);
	BOOL ConvertHtmlToRtf(const char* szHtml, LPCTSTR szCharSet, CString& sRtf, LPCTSTR szImageDir);

	static BOOL IsRTF(const char* szRTF);
	
protected:
	CMSWordHelper* m_pWordHelper;
	CStringArray m_aTempImageFolders;
	BOOL m_bAllowMSWordUse;
	
protected:
	BOOL ConvertRtfToHtmlWithMSWord(LPCTSTR szRtfFile, LPCTSTR szCharSet, CString& sHtml, LPCTSTR szUniqueDir);
	BOOL ConvertRtfToHtmlWithIntenso(LPCTSTR szRtfFile, LPCTSTR szCharSet, CString& sHtml, LPCTSTR szUniqueDir);
	BOOL ConvertRtfToHtmlWithSautin(LPCTSTR szRtfFile, LPCTSTR szCharSet, CString& sHtml, LPCTSTR szUniqueDir);

	BOOL ConvertHtmlToRtfWithMSWord(LPCTSTR szHtmlFile, LPCTSTR szCharSet, CString& sRtf, LPCTSTR szUniqueDir);
	BOOL ConvertHtmlToRtfWithSautin(LPCTSTR szHtmlFile, LPCTSTR szCharSet, CString& sRtf, LPCTSTR szUniqueDir);
	BOOL ConvertHtmlToRtfWithViskoe(LPCTSTR szHtmlFile, LPCTSTR szCharSet, CString& sRtf, LPCTSTR szUniqueDir);

	void CleanupTemporaryImages();
	static BOOL ProcessMSWordHtml(const CString& sUniqueDir, CString& sHtml);
};

#endif // !defined(AFX_RTF2HTMLCONVERTER_H__2F622EC4_0FFE_47E5_B583_637116DE9B2A__INCLUDED_)
