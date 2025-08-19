#ifndef _MISCWEB_FUNCTIONS_H_
#define _MISCWEB_FUNCTIONS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////////////////////////

// enum 
// {
// };

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace WebMisc
{
	BOOL IsOnline();
	BOOL DeleteCacheEntry(LPCTSTR szURI);
	BOOL IsURL(LPCTSTR szURL);
	BOOL IsAboutBlank(LPCTSTR szURL);
	BOOL IsBrowser(LPCTSTR szFilePath);

	int ExtractHtmlLinks(const CString& sHtml, CStringArray& aLinks, CStringArray& aLinkText);
	int ExtractFirstHtmlLink(const CString& sHtml, CString& sLink, CString& sText);
	int ExtractNextHtmlLink(const CString& sHtml, int nFrom, CString& sLink, CString& sText);
	
	BOOL GetPageTitle(const CString& sPageHtml, CString& sTitle);
	BOOL DownloadFile(LPCTSTR szDownloadUri, LPCTSTR sDownloadFile, IBindStatusCallback* pCallback = NULL);
	BOOL DownloadPage(LPCTSTR szDownloadUri, CString& sPageContents, IBindStatusCallback* pCallback = NULL);

	BOOL FormatFileURI(LPCTSTR szFilePath, CString& sFileURI, BOOL bEncodeChars = TRUE);
	BOOL DecodeFileURI(LPCTSTR szFileURI, CString& sFilePath);
	BOOL IsFileURI(LPCTSTR szFilePath);
	BOOL IsRelativeFileURI(LPCTSTR szFilePath);
	BOOL TrimFileProtocol(CString& sFilePath);
	//BOOL Encode(CString& sUrlOrFragment, DWORD dwFlags);

	BOOL RegisterProtocol(LPCTSTR szProtocol, LPCTSTR szAppName, LPCTSTR szCommand);
	BOOL UnregisterProtocol(LPCTSTR szProtocol);
	BOOL IsProtocolRegistered(LPCTSTR szProtocol, LPCTSTR szAppName = NULL, LPCTSTR szCommand = NULL);
	CString GetRegisteredProtocolCommand(LPCTSTR szProtocol);
	CString GetRegisteredProtocolAppPath(LPCTSTR szProtocol, BOOL bFilenameOnly = FALSE);

	BOOL GetProxySettings(CString& sProxy, UINT& nPort);
}

#endif // _MISCWEB_FUNCTIONS_H_