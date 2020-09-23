#if !defined(AFX_URLPARSER_H__B5421D69_41F2_4FCF_AC58_13D2B3D3D3C8__INCLUDED_)
#define AFX_URLPARSER_H__B5421D69_41F2_4FCF_AC58_13D2B3D3D3C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UrlParser.h : header file
//

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

struct URLITEM
{
	CHARRANGE cr;
	CString sUrl;
};
typedef CArray<URLITEM, URLITEM&> CUrlArray;

/////////////////////////////////////////////////////////////////////////////

struct PROTOCOL
{
	PROTOCOL(LPCTSTR szProtocol = NULL, BOOL bNotify = FALSE) : sProtocol(szProtocol), bWantNotify(bNotify) {}

	CString sProtocol;
	BOOL bWantNotify;
};
typedef CArray<PROTOCOL, PROTOCOL&> CProtocolArray;

/////////////////////////////////////////////////////////////////////////////
// CUrlParser 
class CUrlParser
{
public:
	CUrlParser();
	virtual ~CUrlParser();

	BOOL MatchesProtocol(LPCTSTR szText) const;
	BOOL IsFileProtocol(int nProtocol) const;
	BOOL IsFileProtocol(LPCTSTR szText) const;
	BOOL HasProtocols() const;
	BOOL ProtocolWantsNotification(int nProtocol) const;

	int AddProtocol(LPCTSTR szProtocol, BOOL bWantNotify = TRUE);
	int ParseText(LPCTSTR szText, CUrlArray& aUrls) const;
	int ParseText(LPCTSTR szText, CStringArray& aUrls) const;
	int MatchProtocol(LPCTSTR szText) const;
	int GetProtocols(CStringArray& aProtocols) const;

	static CString GetUrlAsFile(const CString& sUrl);
	static BOOL IsBaseDelim(LPCTSTR szText);
	static void InsertInOrder(URLITEM& urli, CUrlArray& aUrls);
	static CString CreateFileLink(LPCTSTR szFile);
	static void AppendURLsToLinkText(CString& sLinkText, const CString& sURLs);
	static BOOL FindEndOfUrl(LPCTSTR& szPos, int& nUrlLen, BOOL bBraced, BOOL bFile);

protected:
	CProtocolArray m_aProtocols;

	int m_nFileProtocol, m_nFileProtocol2;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_URLPARSER_H__B5421D69_41F2_4FCF_AC58_13D2B3D3D3C8__INCLUDED_)
