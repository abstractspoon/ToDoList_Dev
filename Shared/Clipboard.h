// Clipboard.h: interface for the CClipboard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIPBOARD_H__D42BA7AE_BD1A_433E_AC89_ACA7CB2ABFA9__INCLUDED_)
#define AFX_CLIPBOARD_H__D42BA7AE_BD1A_433E_AC89_ACA7CB2ABFA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////
/*
CF_TEXT             1
CF_BITMAP           2
CF_METAFILEPICT     3
CF_SYLK             4
CF_DIF              5
CF_TIFF             6
CF_OEMTEXT          7
CF_DIB              8
CF_PALETTE          9
CF_PENDATA          10
CF_RIFF             11
CF_WAVE             12
CF_UNICODETEXT      13
CF_ENHMETAFILE      14

CF_HDROP            15
CF_LOCALE           16
CF_MAX              17

CF_OWNERDISPLAY     0x0080
CF_DSPTEXT          0x0081
CF_DSPBITMAP        0x0082
CF_DSPMETAFILEPICT  0x0083
CF_DSPENHMETAFILE   0x008E
*/

static UINT CBF_HTML		= ::RegisterClipboardFormat(_T("HTML Format"));
static UINT CBF_MSURL		= ::RegisterClipboardFormat(_T("msSourceUrl"));
static UINT CBF_RTF			= ::RegisterClipboardFormat(CF_RTF);
static UINT CBF_RETEXTOBJ	= ::RegisterClipboardFormat(CF_RETEXTOBJ);
static UINT CBF_EMBEDDEDOBJ	= ::RegisterClipboardFormat(_T("Embedded Object"));

#ifdef _UNICODE
#	define CFSTR_FILENAME2 _T("FileNameW")
#else
#	define CFSTR_FILENAME2 _T("FileName")
#endif

static UINT CBF_FILENAME	= ::RegisterClipboardFormat(CFSTR_FILENAME2);

//////////////////////////////////////////////////////////////////////

struct CLIPBOARDDATA
{
	CLIPBOARDDATA(const CString& str = _T(""), UINT nFmt = 0) : sData(str), nFormat(nFmt) {}

	UINT nFormat;
	CString sData;
};
typedef CArray<CLIPBOARDDATA, CLIPBOARDDATA&> CClipboardDataArray;

//////////////////////////////////////////////////////////////////////

class CClipboard
{
public:
	CClipboard(); // for reading
	CClipboard(HWND hWnd); // for writing
	
	~CClipboard();

	BOOL Open(HWND hWnd);
	BOOL Close();
	BOOL Clear() const;

	BOOL IsOpen() const { return m_bOpen; }
	operator HWND() const { return m_hWnd; }
	
	CString GetText(UINT nFormat = 0) const;
	BOOL GetText(CString& sText, UINT nFormat = 0) const;
	BOOL SetText(const CString& sText, UINT nFormat = 0) const;

	BOOL SetData(UINT uFormat, HANDLE hMem) const;
	BOOL SetData(const CBitmap& bm) const;
	BOOL SetData(const CClipboardDataArray& aData) const;
	int GetData(CClipboardDataArray& aData) const;

	int GetDropFilePaths(CStringArray& aFilePaths) const;
	BOOL GetHTMLSourceLink(CString& sLink, BOOL bIgnoreAboutBlank = TRUE) const;
	
	static BOOL HasText();
	static BOOL HasFormat(UINT nFormat);
	static CString GetFormatName(UINT nFormat);
	static CString& PackageHTMLFragment(CString& sContent, const CString& sSourceUrl = _T(""));
	static CString& UnpackageHTMLFragment(CString& sContent, CString& sSourceUrl);

protected:
	HWND m_hWnd;
	BOOL m_bOpen;

protected:
	BOOL Open(HWND hWnd, BOOL bClear);

	static UINT GetFormat(UINT nFormat);
	static void FormatAndReplace(int nVal, int nWidth, const CString& sKey, CString& sOutput);
};

#endif // !defined(AFX_CLIPBOARD_H__D42BA7AE_BD1A_433E_AC89_ACA7CB2ABFA9__INCLUDED_)
