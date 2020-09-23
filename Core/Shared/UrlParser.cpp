// urlricheditctrl.cpp : implementation file
//

#include "stdafx.h"
#include "urlParser.h"
#include "filemisc.h"
#include "misc.h"
#include "webmisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUrlRichEditCtrl

CUrlParser::CUrlParser()
	: 
	m_nFileProtocol(-1), 
	m_nFileProtocol2(-1)
{
	AddProtocol(_T("www."), FALSE);
	AddProtocol(_T("http://"), FALSE);
	AddProtocol(_T("https://"), FALSE);
	AddProtocol(_T("ftp://"), FALSE);
	AddProtocol(_T("outlook:"), FALSE);
	AddProtocol(_T("mailto:"), FALSE);
	AddProtocol(_T("Notes://"), FALSE);
	AddProtocol(_T("evernote://"), FALSE);
	AddProtocol(_T("onenote:"), FALSE);
	AddProtocol(_T("excel:"), FALSE);
	AddProtocol(_T("winword:"), FALSE);
	AddProtocol(_T("thunderlink://"), FALSE);
	AddProtocol(_T("wiki:"), FALSE);

	// Note: The correct file URI protocol has an extra trailing slash
	// but we use the incorrect one to pick up badly formatted URIs
	m_nFileProtocol = AddProtocol(_T("file:///"), FALSE);
	m_nFileProtocol2 = AddProtocol(_T("file://"), FALSE);
}

CUrlParser::~CUrlParser()
{
}

/////////////////////////////////////////////////////////////////////////////
// CUrlRichEditCtrl message handlers

int CUrlParser::AddProtocol(LPCTSTR szProtocol, BOOL bWantNotify)
{
	int nExist = MatchProtocol(szProtocol);
	
	if (nExist == -1)
	{
		PROTOCOL prot(szProtocol, bWantNotify);
		return m_aProtocols.Add(prot);
	}

	return nExist;
}

BOOL CUrlParser::HasProtocols() const
{
	return m_aProtocols.GetSize();
}

int CUrlParser::MatchProtocol(LPCTSTR szText) const
{
	if (szText[0] == '<')
		szText++;
	
	int nNumProt = m_aProtocols.GetSize();
	
	for (int nProt = 0; nProt < nNumProt; nProt++)
	{
		const PROTOCOL& prot = m_aProtocols[nProt];
		
		if (_tcsnicmp(szText, prot.sProtocol, prot.sProtocol.GetLength()) == 0)
			return nProt;
	}
	
	return -1;
}

BOOL CUrlParser::MatchesProtocol(LPCTSTR szText) const
{
	return (MatchProtocol(szText) != -1);
}

CString CUrlParser::GetUrlAsFile(const CString& sUrl)
{
	if (WebMisc::IsFileURI(sUrl))
	{
		CString sFilePath;
		
		if (WebMisc::DecodeFileURI(sUrl, sFilePath))
		{
			FileMisc::ExpandPathEnvironmentVariables(sFilePath);
			return sFilePath;
		}
	}
	
	// else
	return sUrl;
}

int CUrlParser::GetProtocols(CStringArray& aProtocols) const
{
	aProtocols.RemoveAll();

	int nNumProt = m_aProtocols.GetSize();
	
	for (int nProt = 0; nProt < nNumProt; nProt++)
		aProtocols.Add(m_aProtocols[nProt].sProtocol);

	return aProtocols.GetSize();
}

BOOL CUrlParser::FindEndOfUrl(LPCTSTR& szPos, int& nUrlLen, BOOL bBraced, BOOL bFile)
{
	LPCTSTR szUrlStart = szPos;
	nUrlLen = 0;

	if (bBraced)
	{
		while (*szPos && *szPos != '>')
		{
			szPos++;
			nUrlLen++;
		}
	}
	else
	{
		while (!CUrlParser::IsBaseDelim(szPos))
		{
			szPos++;
			nUrlLen++;
		}

		// make sure the url does not end in a punctuation mark
		// But don't decrement nPos because that represents
		// where we will restart processing
		if (bFile)
		{
			// Less stringent requirements
			static CString FILEPUNCTUATION(_T(".,;:&*~?\\\"'"));
						
			while (FILEPUNCTUATION.Find(szUrlStart[nUrlLen - 1]) != -1)
				nUrlLen--;
		}
		else // web
		{
			// More stringent requirements
			static CString WEBPUNCTUATION(_T(".,;:(){}[]<>&*~?\\\"'"));
						
			while (WEBPUNCTUATION.Find(szUrlStart[nUrlLen - 1]) != -1)
				nUrlLen--;
		}
	}

	return (nUrlLen > 0);
}

BOOL CUrlParser::IsBaseDelim(LPCTSTR szText)
{
	if (Misc::IsEmpty(szText))
		return TRUE; // end of string

	static LPCTSTR BASEDELIMS[] = 
	{ 
		_T(" "), 
		_T("\n"),
		_T("\r"),
		_T("\t"),
		_T(", "),
		_T(". "),
		_T("<"),
	};
	const int NUM_DEMIM = sizeof(BASEDELIMS) / sizeof(LPCTSTR);

	for (int nDelim = 0; nDelim < NUM_DEMIM; nDelim++)
	{
		LPCTSTR szDelim = BASEDELIMS[nDelim];
		
		if (szDelim[0] == *szText)
		{
			// test char after ch if 2 char delim
			if ((szDelim[1] == 0) || (szDelim[1] == *(szText + 1)))
				return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CUrlParser::IsFileProtocol(int nProtocol) const
{
	ASSERT((nProtocol >= 0) && (nProtocol < m_aProtocols.GetSize()));

	return ((nProtocol == m_nFileProtocol) || (nProtocol == m_nFileProtocol2));
}

BOOL CUrlParser::IsFileProtocol(LPCTSTR szText) const
{
	return IsFileProtocol(MatchProtocol(szText));
}

BOOL CUrlParser::ProtocolWantsNotification(int nProtocol) const
{
	if ((nProtocol < 0) || (nProtocol >= m_aProtocols.GetSize()))
	{
		ASSERT(0);
		return FALSE;
	}

	return m_aProtocols[nProtocol].bWantNotify;
}

int CUrlParser::ParseText(LPCTSTR szText, CStringArray& aUrls) const
{
	CUrlArray aParsedUrls;
	int nNumUrl = ParseText(szText, aParsedUrls);

	aUrls.SetSize(nNumUrl);

	for (int nUrl = 0; nUrl < nNumUrl; nUrl++)
		aUrls[nUrl] = aParsedUrls[nUrl].sUrl;

	return nNumUrl;
}

int CUrlParser::ParseText(LPCTSTR szText, CUrlArray& aUrls) const
{
	aUrls.RemoveAll();

	LPCTSTR szPos = szText;
	BOOL bPrevCharWasDelim = TRUE;
	
	while (*szPos) 
	{
		// if the previous item was not a delimiter then there's no
		// point checking for a protocol match so we just update the
		// value of bPrevDelim for the current char
		BOOL bStartDelim(IsBaseDelim(szPos));

		if (!bPrevCharWasDelim)
		{
			bPrevCharWasDelim = bStartDelim;
			szPos++;
			continue;
		}
		// if the current char is a delim then this can't be the start
		// of a url either
		else if (bStartDelim)
		{
			bPrevCharWasDelim = TRUE;
			szPos++;
			continue;
		}
		
		// now check for a protocol
		int nProt = MatchProtocol(szPos);
		
		// if no match then increment pos and go to next char
		if (nProt == -1)
		{
			bPrevCharWasDelim = FALSE;
			szPos++;
			continue;
		}
		
		// find the end of the url (URLDELIMS)
		int nUrlLen = 0;
		LPCTSTR szUrlStart = szPos;
		
		// check for braces (<...>)
		int nUrlStart = (szPos - szText);
		BOOL bBraced = ((nUrlStart > 0) && (szPos[-1] == '<'));
		
		VERIFY(FindEndOfUrl(szPos, nUrlLen, bBraced, IsFileProtocol(nProt)));
		
		// Only save if the link is more than just the protocol
		if (nUrlLen > m_aProtocols[nProt].sProtocol.GetLength())
		{
			URLITEM urli;

			urli.cr.cpMin = nUrlStart;
			urli.cr.cpMax = (nUrlStart + nUrlLen);
			urli.sUrl = CString(szUrlStart, nUrlLen);
			
			InsertInOrder(urli, aUrls);
		}

		bPrevCharWasDelim = TRUE;
	}

	return aUrls.GetSize();
}

void CUrlParser::InsertInOrder(URLITEM& urli, CUrlArray& aUrls)
{
	// work backwards looking for first item that comes before us
	int nUrl = aUrls.GetSize();

	while (nUrl--)
	{
		if (aUrls[nUrl].cr.cpMin < urli.cr.cpMin)
		{
			aUrls.InsertAt(nUrl + 1, urli);
			return;
		}
	}

	// else insert at start
	aUrls.InsertAt(0, urli);
}

CString CUrlParser::CreateFileLink(LPCTSTR szFile)
{
	ASSERT(FileMisc::IsPath(szFile));
	CString sLink;
		
	if (WebMisc::FormatFileURI(szFile, sLink, FALSE))
	{
		// if the path contains spaces then brace it
		if (sLink.Find(' ') != -1)
			sLink = "<" + sLink + ">";
	}
	
	return sLink;
}

void CUrlParser::AppendURLsToLinkText(CString& sLinkText, const CString& sURLs)
{
	CStringArray aLinks, aLinkText;
	int nLink = WebMisc::ExtractHtmlLinks(sURLs, aLinks, aLinkText);

	// replace link text with text + url
	while (nLink--)
	{
		CString sLinkAndText;
		sLinkAndText.Format(_T("%s ( %s )"), aLinkText[nLink], aLinks[nLink]);

		sLinkText.Replace(aLinkText[nLink], sLinkAndText);
	}
}

