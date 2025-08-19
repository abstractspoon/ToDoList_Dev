#include "stdafx.h"
#include "webmisc.h"
#include "misc.h"
#include "filemisc.h"
#include "xmlcharmap.h"
#include "regkey.h"

#include <comdef.h>

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <wininet.h>
#include <shlwapi.h>

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "shlwapi.lib")

///////////////////////////////////////////////////////////////////////////////////////////////////

// Missing from shlwapi.h
LWSTDAPI UrlCreateFromPathA(LPCSTR pszPath, LPSTR pszUrl, LPDWORD pcchUrl, DWORD dwFlags);
LWSTDAPI UrlCreateFromPathW(LPCWSTR pszPath, LPWSTR pszUrl, LPDWORD pcchUrl, DWORD dwFlags);
LWSTDAPI PathCreateFromUrlA(LPCSTR pszUrl, LPSTR pszPath, LPDWORD pcchPath, DWORD dwFlags);
LWSTDAPI PathCreateFromUrlW(LPCWSTR pszUrl, LPWSTR pszPath, LPDWORD pcchPath, DWORD dwFlags);

#ifdef UNICODE
#	define UrlCreateFromPath       UrlCreateFromPathW
#	define PathCreateFromUrl       PathCreateFromUrlW
#else //!UNICODE
#	define UrlCreateFromPath       UrlCreateFromPathA
#	define PathCreateFromUrl       PathCreateFromUrlA
#endif //UNICODE

///////////////////////////////////////////////////////////////////////////////////////////////////

const LPCTSTR FILEPROTOCOLS[] = 
{
	_T("file:///"),		// Correct
	_T("file://"),      // Backwards compatibility
};

const int NUM_FILEPROTOCOLS = (sizeof(FILEPROTOCOLS) / sizeof(LPCTSTR));

const CString INTERNETSETTINGS("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings");

///////////////////////////////////////////////////////////////////////////////////////////////////

BOOL WebMisc::IsOnline()
{
    DWORD dwState = 0; 
    DWORD dwSize = sizeof(DWORD);
	
    return InternetQueryOption(NULL, INTERNET_OPTION_CONNECTED_STATE, &dwState, &dwSize) && 
		(dwState & INTERNET_STATE_CONNECTED);
}

BOOL WebMisc::DeleteCacheEntry(LPCTSTR szURI)
{
	BOOL bSuccess = FALSE;
	
#if _MSC_VER >= 1400
	bSuccess = DeleteUrlCacheEntry(szURI);
#else
	LPSTR szAnsiPath = Misc::WideToMultiByte(szURI);
	bSuccess = DeleteUrlCacheEntry(szAnsiPath);
	delete [] szAnsiPath;
#endif

	if (!bSuccess && (GetLastError() == ERROR_FILE_NOT_FOUND))
		bSuccess = TRUE;

	return bSuccess;
}

BOOL WebMisc::IsURL(LPCTSTR szURL)
{
	if (::PathIsURL(szURL))
		return TRUE;

	CString sURL(szURL);

	sURL.TrimLeft();
	sURL.MakeLower();

	return ((sURL.Find(_T("www.")) == 0) || (sURL.Find(_T("ftp.")) == 0));
}

/*
BOOL WebMisc::Encode(CString& sUrlOrFragment, DWORD dwFlags)
{
	DWORD dwLen = (sUrlOrFragment.GetLength() * 4);
	CString sEncoded;

	HRESULT hr = ::UrlEscape(sUrlOrFragment, sEncoded.GetBuffer(dwLen + 1), &dwLen, dwFlags);
	sEncoded.ReleaseBuffer(dwLen);

	if (hr == S_OK)
	{
		sUrlOrFragment = sEncoded;
		return TRUE;
	}

	sUrlOrFragment.Empty();
	return FALSE;
}
*/

BOOL WebMisc::IsBrowser(LPCTSTR szFilePath)
{
	CString sFileName = FileMisc::GetFileNameFromPath(szFilePath, FALSE);

	return ((sFileName.CompareNoCase(_T("chrome")) == 0) ||
			(sFileName.CompareNoCase(_T("iexplore")) == 0) ||
			(sFileName.CompareNoCase(_T("edge")) == 0) ||
			(sFileName.CompareNoCase(_T("firefox")) == 0) ||
			(sFileName.CompareNoCase(_T("opera")) == 0));
}

int WebMisc::ExtractFirstHtmlLink(const CString& sHtml, CString& sLink, CString& sText)
{
	return ExtractNextHtmlLink(sHtml, 0, sLink, sText);
}

int WebMisc::ExtractNextHtmlLink(const CString& sHtml, int nFrom, CString& sLink, CString& sText)
{
	const CString LINK_START(_T("href=\"")), LINK_END(_T("\""));
	const CString TEXT_START(_T(">")), TEXT_END(_T("</a>"));

	const CString sHtmlLower = Misc::ToLower(sHtml);

	int nStart = sHtmlLower.Find(LINK_START, nFrom), nEnd = -1;

	if (nStart == -1)
		return -1;

	nStart += LINK_START.GetLength();
	nEnd = sHtmlLower.Find(LINK_END, nStart);

	if (nEnd == -1)
		return -1;

	sLink = sHtml.Mid(nStart, (nEnd - nStart));

	// get text
	nStart = sHtmlLower.Find(TEXT_START, nEnd + 1);

	if (nStart != -1)
	{
		nStart += TEXT_START.GetLength();
		nEnd = sHtmlLower.Find(TEXT_END, nStart);

		if (nEnd == -1)
			return -1;

		sText = sHtml.Mid(nStart, (nEnd - nStart));

		// cleanup
		Misc::Trim(sText, _T("\" "));

		// translate HTML representations
		CXmlCharMap::ConvertFromRep(sText);

		return (nEnd + TEXT_END.GetLength());
	}

	// else just link
	sText.Empty();
	return (nEnd + 1);
}

int WebMisc::ExtractHtmlLinks(const CString& sHtml, CStringArray& aLinks, CStringArray& aLinkText)
{
	CString sLink, sText;
	int nFind = ExtractFirstHtmlLink(sHtml, sLink, sText);

	while (nFind != -1)
	{
		aLinks.Add(sLink);
		aLinkText.Add(sText);

		// next link
		nFind = ExtractNextHtmlLink(sHtml, nFind, sLink, sText);
	}
	ASSERT(aLinkText.GetSize() == aLinks.GetSize());

	return aLinks.GetSize();
}

BOOL WebMisc::IsAboutBlank(LPCTSTR szURL)
{
	CString sURL(szURL);
	sURL.MakeUpper();

	return (sURL.Find(_T("ABOUT:BLANK")) != -1);
}

BOOL WebMisc::FormatFileURI(LPCTSTR szFilePath, CString& sFileURI, BOOL bEncodeChars)
{
	if (!FileMisc::IsPath(szFilePath))
		return FALSE;

	if (bEncodeChars)
	{
		DWORD dwLen = MAX_PATH;
		HRESULT hr = ::UrlCreateFromPath(szFilePath, sFileURI.GetBuffer(MAX_PATH + 1), &dwLen, 0);
		sFileURI.ReleaseBuffer(dwLen);

		if (hr == S_OK)
			return TRUE;

		sFileURI.Empty();
		return FALSE;
	}

	// else just append file protocol and flip slashes
	sFileURI.Format(_T("%s%s"), FILEPROTOCOLS[0], szFilePath);
	sFileURI.Replace('\\', '/');

	return TRUE;
}

BOOL WebMisc::DecodeFileURI(LPCTSTR szFileURI, CString& sFilePath)
{
	if (!IsFileURI(szFileURI))
		return FALSE;

	// PathCreateFromUrl does not handle relative paths
	// and always prefixes paths with '\' meaning 'root'
	// so we detect this and strips of any leading '\'
	BOOL bRelative = IsRelativeFileURI(szFileURI);

	DWORD dwLen = MAX_PATH;
	HRESULT hr = ::PathCreateFromUrl(szFileURI, sFilePath.GetBuffer(MAX_PATH + 1), &dwLen, 0);
	sFilePath.ReleaseBuffer(dwLen);
	 
	if (hr == S_OK)
	{
		ASSERT(!sFilePath.IsEmpty());

		if (bRelative)
		{
			while (sFilePath[0] == '\\')
				sFilePath = sFilePath.Mid(1);
		}

		return TRUE;
	}

	// else
	sFilePath.Empty();
	return FALSE;
}

BOOL WebMisc::IsFileURI(LPCTSTR szFileURI)
{
	for (int nProtocol = 0; nProtocol < NUM_FILEPROTOCOLS; nProtocol++)
	{
		if (_tcsstr(szFileURI, FILEPROTOCOLS[nProtocol]) != NULL)
			return TRUE;
	}

	return FALSE;
}

BOOL WebMisc::IsRelativeFileURI(LPCTSTR szFileURI)
{
	CString sFileURI(szFileURI);

	if (TrimFileProtocol(sFileURI))
	{
		sFileURI.Replace('/', '\\');
		return ::PathIsRelative(sFileURI);
	}

	return FALSE;
}

BOOL WebMisc::TrimFileProtocol(CString& sFilePath)
{
	for (int nProtocol = 0; nProtocol < NUM_FILEPROTOCOLS; nProtocol++)
	{
		LPCTSTR szProtocol(FILEPROTOCOLS[nProtocol]);
		int nFind = sFilePath.Find(szProtocol);

		if (nFind != -1)
		{
			sFilePath = sFilePath.Mid(nFind + lstrlen(szProtocol));
			return TRUE;
		}
	}

	return FALSE;
}

BOOL WebMisc::RegisterProtocol(LPCTSTR szProtocol, LPCTSTR szAppName, LPCTSTR szCommand)
{
	if (!IsProtocolRegistered(szProtocol, szAppName, szCommand))
	{
		CRegKey2 reg;

		if (reg.Open(HKEY_CLASSES_ROOT, szProtocol, FALSE) != ERROR_SUCCESS)
			return FALSE;

		CString sTemp;
		sTemp.Format(_T("URL: %s protocol"), szAppName);

		reg.Write(_T(""), sTemp);
		reg.Write(_T("URL Protocol"), _T(""));
		reg.Close();

		sTemp.Format(_T("%s\\shell\\open\\command"), szProtocol);

		if (reg.Open(HKEY_CLASSES_ROOT, sTemp, FALSE) != ERROR_SUCCESS)
			return FALSE;

		return (reg.Write(_T(""), szCommand) == ERROR_SUCCESS);
	}

	// else already registered
	return TRUE;
}

BOOL WebMisc::UnregisterProtocol(LPCTSTR szProtocol)
{
	if (CRegKey2::KeyExists(HKEY_CLASSES_ROOT, szProtocol))
		return (CRegKey2::DeleteKey(HKEY_CLASSES_ROOT, szProtocol) == ERROR_SUCCESS);

	// else not there
	return TRUE;
}

BOOL WebMisc::IsProtocolRegistered(LPCTSTR szProtocol, LPCTSTR szAppName, LPCTSTR szCommand)
{
	if (!CRegKey2::KeyExists(HKEY_CLASSES_ROOT, szProtocol))
		return FALSE;

	CRegKey2 reg;

	// optional test
	if (!Misc::IsEmpty(szAppName))
	{
		if (reg.Open(HKEY_CLASSES_ROOT, szProtocol, TRUE) != ERROR_SUCCESS)
			return FALSE;
		
		CString sTemp1;

		if (reg.Read(_T(""), sTemp1) != ERROR_SUCCESS)
			return FALSE;

		CString sTemp2;
		sTemp2.Format(_T("URL: %s protocol"), szAppName);

		if (sTemp1.CompareNoCase(sTemp2) != 0)
			return FALSE;
		
		reg.Close();
	}
	
	CString sCommand = GetRegisteredProtocolCommand(szProtocol);

	if (sCommand.IsEmpty())
		return FALSE;

	// optional test
	if (!Misc::IsEmpty(szCommand))
		return (sCommand.CompareNoCase(szCommand) == 0);

	// else
	return TRUE;
}

CString WebMisc::GetRegisteredProtocolCommand(LPCTSTR szProtocol)
{
	CString sCommand;

	if (CRegKey2::KeyExists(HKEY_CLASSES_ROOT, szProtocol))
	{
		CString sTemp;
		sTemp.Format(_T("%s\\shell\\open\\command"), szProtocol);

		CRegKey2 reg;
		
		if (reg.Open(HKEY_CLASSES_ROOT, sTemp, TRUE) == ERROR_SUCCESS)
		{
			if (reg.Read(_T(""), sCommand) != ERROR_SUCCESS)
				sCommand.Empty();
		}
	}

	return sCommand;
}

CString WebMisc::GetRegisteredProtocolAppPath(LPCTSTR szProtocol, BOOL bFilenameOnly)
{
	CString sAppPath = GetRegisteredProtocolCommand(szProtocol);

	if (!sAppPath.IsEmpty())
	{
		int nEnd = Misc::ToLower(sAppPath).Find(_T(".exe"));

		if (nEnd != -1)
		{
			sAppPath = sAppPath.Left(nEnd + 4);

			if (bFilenameOnly)
				sAppPath = FileMisc::GetFileNameFromPath(sAppPath);
		}
	}

	return sAppPath;
}

BOOL WebMisc::GetPageTitle(const CString& sPageHtml, CString& sTitle)
{
	int nTitleStart = sPageHtml.Find(_T("<title"));

	if (nTitleStart == -1)
		return FALSE;

	int nTitleStartEndBrace = sPageHtml.Find('>', nTitleStart);

	if (nTitleStartEndBrace == -1)
		return FALSE;

	int nTitleEnd = sPageHtml.Find(_T("</title>"));

	if ((nTitleEnd == -1) || (nTitleEnd <= nTitleStartEndBrace))
		return FALSE;

	sTitle = sPageHtml.Mid(nTitleStartEndBrace + 1, (nTitleEnd - (nTitleStartEndBrace + 1)));

	return !sTitle.IsEmpty();
}

BOOL WebMisc::DownloadFile(LPCTSTR szDownloadUri, LPCTSTR szDownloadFile, IBindStatusCallback* pCallback)
{
	HRESULT hr = ::URLDownloadToFile(NULL, szDownloadUri, szDownloadFile, 0, pCallback);

	switch (hr)
	{
	case S_OK:
		return TRUE;
		
	case E_ABORT:
		FileMisc::LogText(_T("WebMisc::DownloadFile(cancelled download of %s to %s)"), szDownloadUri, szDownloadFile);
		break;

	default:
		FileMisc::LogText(_T("WebMisc::DownloadFile(failed to download %s to %s)"), szDownloadUri, szDownloadFile);
		FileMisc::LogText(_T("\tURLDownloadToFile reported %s"), _com_error(hr).ErrorMessage());
		break;
	}

	return FALSE;
}

BOOL WebMisc::DownloadPage(LPCTSTR szDownloadUri, CString& sPageContents, IBindStatusCallback* pCallback)
{
	CString sTempFile = FileMisc::GetTempFilePath();

	if (!DownloadFile(szDownloadUri, sTempFile, pCallback))
		return FALSE;

	return FileMisc::LoadFile(sTempFile, sPageContents);
}

BOOL WebMisc::GetProxySettings(CString& sProxy, UINT& nPort)
{
	CRegKey2 reg;

	if (reg.Open(HKEY_CURRENT_USER, INTERNETSETTINGS, TRUE) == ERROR_SUCCESS)
	{
		// is proxy enabled?
		DWORD dwProxyEnabled = FALSE;

		if ((reg.Read(_T("ProxyEnabled"), dwProxyEnabled) == ERROR_SUCCESS) && dwProxyEnabled)
		{
			if ((reg.Read(_T("ProxyServer"), sProxy) == ERROR_SUCCESS) && !sProxy.IsEmpty())
			{
				CString sPort;

				if (Misc::Split(sProxy, sPort, ':'))
					nPort = _ttoi(sPort);
				else
					nPort = 80;

				return TRUE;
			}
		}
	}

	// all else 
	return FALSE;
}
