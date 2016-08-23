// Clipboard.cpp: implementation of the CClipboard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Clipboard.h"
#include "FileMisc.h"
#include "winclasses.h"
#include "WebMisc.h"
#include "Misc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClipboard::CClipboard() : m_hWnd(NULL), m_bOpen(FALSE)
{
	// reading can be done anonymously
	Open(NULL, FALSE);
}

CClipboard::CClipboard(HWND hWnd) : m_hWnd(NULL), m_bOpen(FALSE)
{
	Open(hWnd);
}

CClipboard::~CClipboard()
{
	Close();
}

BOOL CClipboard::Open(HWND hWnd)
{
	return Open(hWnd, TRUE);
}

// protected helper
BOOL CClipboard::Open(HWND hWnd, BOOL bClear)
{
	// sanity check
	ASSERT((::IsWindow(hWnd) && bClear) || (!hWnd && !bClear));

	if ((::IsWindow(hWnd) && !bClear) || (!hWnd && bClear))
		return FALSE;

	if (m_bOpen && hWnd && (hWnd != m_hWnd))
		Close();
	
	if (::OpenClipboard(hWnd))
	{
		m_hWnd = hWnd;
		m_bOpen = TRUE;
			
		if (bClear)
			Clear();
	}
	else
		ASSERT(0);
	
	return m_bOpen;
}

BOOL CClipboard::Close()
{
	if (m_bOpen && ::CloseClipboard())
	{
		m_bOpen = FALSE;
		m_hWnd = NULL;

		return TRUE;
	}
	// else already closed or never opened
	
	return FALSE;
}

BOOL CClipboard::Clear() const
{
	// sanity checks
	ASSERT(m_bOpen && m_hWnd);
	
	if (!m_bOpen || (m_hWnd == NULL))
		return FALSE;
	
	return ::EmptyClipboard();
}

int CClipboard::GetData(CClipboardDataArray& aData) const
{
	ASSERT(m_bOpen);

	if (!m_bOpen)
		return 0;

	aData.RemoveAll();

	UINT nFmt = ::EnumClipboardFormats(0);
	CLIPBOARDDATA data;

	while (nFmt)
	{
		if (!GetText(data.sData, nFmt))
			continue;

		data.nFormat = nFmt;
		aData.Add(data);

		// next
		nFmt = ::EnumClipboardFormats(nFmt);
	}

	return aData.GetSize();
}

BOOL CClipboard::SetData(const CClipboardDataArray& aData) const
{
	// sanity checks
	ASSERT(m_bOpen && m_hWnd);
	
	if (!m_bOpen || (m_hWnd == NULL))
		return FALSE;

	int nItem = aData.GetSize();

	while (nItem--)
	{
		const CLIPBOARDDATA& clipData = aData[nItem];
		
		if (!SetText(clipData.sData, clipData.nFormat))
			return FALSE;
	}

	return TRUE;
}

BOOL CClipboard::SetData(UINT uFormat, HANDLE hMem) const
{
	// sanity checks
	ASSERT(m_bOpen && m_hWnd);
	
	if (!m_bOpen || (m_hWnd == NULL))
		return FALSE;

	ASSERT(hMem);

	if (!hMem)
		return FALSE;
	
	return (::SetClipboardData(uFormat, hMem) == hMem);
}

BOOL CClipboard::SetData(const CBitmap& bm) const
{
	return SetData(CF_BITMAP, (HANDLE)bm.GetSafeHandle());
}

BOOL CClipboard::HasFormat(UINT nFormat)
{
	return (::IsClipboardFormatAvailable(nFormat));
}

BOOL CClipboard::HasText()
{
	return HasFormat(GetFormat(0));
}

CString& CClipboard::PackageHTMLFragment(CString& sContent, const CString& sSourceUrl)
{
	CString sHeader = _T("Version:0.9\n")
						_T("StartHTML:BBBBBBBBBB\n")
						_T("EndHTML:CCCCCCCCCC\n")
						_T("StartFragment:DDDDDDDDDD\n")
						_T("EndFragment:EEEEEEEEEE\n")
						_T("SourceURL:FFFFFFFFFF\n");
	
	// source Url
	sHeader.Replace(_T("FFFFFFFFFF"), sSourceUrl);

	// Start Html
	FormatAndReplace(sHeader.GetLength(), 10, _T("BBBBBBBBBB"), sHeader);
	
	// Start Fragment
	sHeader += _T("<html>\n<body>\n<!--StartFragment-->");
	FormatAndReplace(sHeader.GetLength(), 10, _T("DDDDDDDDDD"), sHeader);
	
	// End Fragment
	sHeader += sContent;
	sHeader.TrimRight();
	sHeader.Replace(_T("\r\n"), _T("\n"));
	FormatAndReplace(sHeader.GetLength(), 10, _T("EEEEEEEEEE"), sHeader);
	
	// End Html
	sHeader += _T("<!--EndFragment-->\n</body>\n</html>");
	FormatAndReplace(sHeader.GetLength(), 10, _T("CCCCCCCCCC"), sHeader);
	
	// copy back final result
	sContent = sHeader;

	return sContent;
}

CString& CClipboard::UnpackageHTMLFragment(CString& sContent, CString& sSourceUrl)
{
	static CString STARTHTML = _T("StartHTML:");
	static CString ENDHTML = _T("EndHTML:");
	static CString SRCURL = _T("SourceURL:");

	// check that it is packaged
	int nStart = sContent.Find(STARTHTML);
	
	if (nStart != -1)
	{
		// src url may or may not be present
		int nSrcUrl = sContent.Find(SRCURL);

		if (nSrcUrl != -1)
		{
			nSrcUrl += SRCURL.GetLength();

			int nEOL = sContent.Find('\n', nSrcUrl);
			ASSERT(nEOL > nSrcUrl);

			if (nEOL > nSrcUrl)
			{
				sSourceUrl = sContent.Mid(nSrcUrl, (nEOL - nSrcUrl));
			}
		}
		
		int nEnd = sContent.Find(ENDHTML);
		ASSERT (nEnd > nStart);
		
		if (nEnd > nStart)
		{
			// convert to char offsets
			LPCTSTR szStart = (((LPCTSTR)sContent) + nStart + STARTHTML.GetLength());
			LPCTSTR szEnd = (((LPCTSTR)sContent) + nEnd + ENDHTML.GetLength());
			
			nStart = _ttoi(szStart);
			nEnd = _ttoi(szEnd);
			
			sContent = sContent.Mid(nStart, (nEnd - nStart));
		}
	}
	
	return sContent;
}

void CClipboard::FormatAndReplace(int nVal, int nWidth, const CString& sKey, CString& sOutput)
{
	ASSERT(sOutput.Find(sKey) != -1);

	CString sVal;
	sVal.Format(_T("%0*d"), nWidth, nVal);

	ASSERT(sVal.GetLength() == nWidth);

	sOutput.Replace(sKey, sVal);
}

UINT CClipboard::GetFormat(UINT nFormat)
{
	if (nFormat == 0)
	{
#ifndef _UNICODE
		nFormat = CF_TEXT;
#else
		nFormat = CF_UNICODETEXT;
#endif
	}

	return nFormat;
}

CString CClipboard::GetText(UINT nFormat) const
{
	CString sText;
	GetText(sText, nFormat);

	return sText;
}

BOOL CClipboard::GetText(CString& sText, UINT nFormat) const
{
	ASSERT(m_bOpen);
	
	if (!m_bOpen)
		return FALSE;

	nFormat = GetFormat(nFormat);
	
	try
	{
		HANDLE hData = ::GetClipboardData(nFormat);
		
		sText = (LPCTSTR)GlobalLock(hData);
		::GlobalUnlock(hData);
	}
	catch(...)
	{
		sText.Empty();
	}
	
	return !sText.IsEmpty();
}

BOOL CClipboard::SetText(const CString& sText, UINT nFormat) const
{
	// sanity checks
	ASSERT(m_bOpen && m_hWnd);
	
	if (!m_bOpen || (m_hWnd == NULL))
		return FALSE;

	nFormat = GetFormat(nFormat);
	
	HGLOBAL hglbCopy = NULL; 
	BOOL bResult = FALSE;
	
	try
	{
		// Allocate a global memory object for the text. 
		size_t nBytes = ((sText.GetLength() + 1) * sizeof(TCHAR));
		hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, nBytes); 
		
		if (hglbCopy) 
		{ 
			// Lock the handle and copy the text to the buffer.
			LPVOID pCopy = ::GlobalLock(hglbCopy);
			CopyMemory(pCopy, (LPCTSTR)sText, nBytes); 
			::GlobalUnlock(hglbCopy); 
			
			// Place the handle on the clipboard. 
			bResult = SetData(nFormat, hglbCopy);
		}
	}
	catch(...)
	{
	}

	// cleanup
	if (!bResult && hglbCopy)
	{
		::GlobalFree(hglbCopy);
	}
	
	return bResult;
}

int CClipboard::GetDropFilePaths(CStringArray& aFilePaths) const
{
	ASSERT(m_bOpen);
	
	if (!m_bOpen)
		return -1; // error
	
	aFilePaths.RemoveAll();
	
	HANDLE hData = ::GetClipboardData(CF_HDROP);
	
	if (hData == NULL)
		return 0;
	
	return FileMisc::GetDropFilePaths((HDROP)hData, aFilePaths);
}

BOOL CClipboard::GetHTMLSourceLink(CString& sLink, BOOL bIgnoreAboutBlank) const
{
	// Internet Explorer
	if (GetText(sLink, CBF_MSURL))
	{
	}
	// Chrome and Firefox uses CF_HTML encoded as multibyte
	else if (GetText(sLink, CBF_HTML))
	{
		// Try processing this as a fragment first
		CString sUrl;

#ifdef _UNICODE
		// convert to unicode for unpackaging because
		// CF_HTML is saved to the clipboard as UTF8
		Misc::EncodeAsUnicode(sLink, CP_UTF8);
#endif
		UnpackageHTMLFragment(sLink, sUrl);
		sLink = sUrl;
	}

	if (sLink.IsEmpty())
		return FALSE;

	if (bIgnoreAboutBlank && WebMisc::IsAboutBlank(sLink))
		return FALSE;

	return TRUE;
}
