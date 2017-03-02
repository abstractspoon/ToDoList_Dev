// urlricheditctrl.cpp : implementation file
//

#include "stdafx.h"
#include "urlricheditctrl.h"
#include "autoflag.h"
#include "richedithelper.h"
#include "winclasses.h"
#include "wclassdefines.h"
#include "filemisc.h"
#include "misc.h"
#include "msoutlookhelper.h"
#include "clipboard.h"
#include "webmisc.h"
#include "xmlcharmap.h"
#include "enstring.h"

#include "..\3rdParty\msoutl.h"

#include <afxole.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const UINT TIMER_REPARSE = 1;
const UINT PAUSE = 1000; // 1 second

/////////////////////////////////////////////////////////////////////////////
// CUrlRichEditCtrl

CUrlRichEditCtrl::CUrlRichEditCtrl() 
	: 
	m_nContextUrl(-1), 
	m_lpDragObject(NULL),
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
	AddProtocol(_T("onenote:///"), FALSE);
	AddProtocol(_T("excel:"), FALSE);
	AddProtocol(_T("winword:"), FALSE);
	AddProtocol(_T("thunderlink://"), FALSE);
	AddProtocol(_T("wiki:"), FALSE);

	// Note: The correct file URI protocol has an extra trailing slash
	// but we use the incorrect one to pick up badly formatted URIs
	m_nFileProtocol = AddProtocol(_T("file:///"), FALSE);
	m_nFileProtocol2 = AddProtocol(_T("file://"), FALSE);
}

CUrlRichEditCtrl::~CUrlRichEditCtrl()
{
}

BEGIN_MESSAGE_MAP(CUrlRichEditCtrl, CRichEditBaseCtrl)
//{{AFX_MSG_MAP(CUrlRichEditCtrl)
	ON_WM_RBUTTONUP()
	ON_WM_KEYUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_SHOWWINDOW()
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_WM_SYSKEYDOWN()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_MESSAGE(WM_DROPFILES, OnDropFiles)

	ON_CONTROL_REFLECT_EX(EN_CHANGE, OnChangeText)
	ON_NOTIFY_REFLECT_EX(EN_LINK, OnNotifyLink)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUrlRichEditCtrl message handlers

int CUrlRichEditCtrl::AddProtocol(LPCTSTR szProtocol, BOOL bWantNotify)
{
	int nExist = MatchProtocol(szProtocol);
	
	if (nExist == -1)
	{
		PROTOCOL prot(szProtocol, bWantNotify);
		return m_aProtocols.Add(prot);
	}
	
	return nExist;
}

int CUrlRichEditCtrl::MatchProtocol(LPCTSTR szText) const
{
   int nNumProt = m_aProtocols.GetSize();

   for (int nProt = 0; nProt < nNumProt; nProt++)
   {
      const PROTOCOL& prot = m_aProtocols[nProt];

      if (_tcsnicmp(szText, prot.sProtocol, prot.sProtocol.GetLength()) == 0)
         return nProt;
   }

   return -1;
}

BOOL CUrlRichEditCtrl::OnChangeText() 
{
	// kill any existing timer
	KillTimer(TIMER_REPARSE);

	// and start a new one
	SetTimer(TIMER_REPARSE, PAUSE, NULL);
	
	return FALSE;
}

LRESULT CUrlRichEditCtrl::OnDropFiles(WPARAM wp, LPARAM /*lp*/) 
{
	CHARRANGE crSelOrg;
	GetSel(crSelOrg); // save this off
	BOOL bEnable = !(GetStyle() & ES_READONLY) && IsWindowEnabled();
	
	if (!bEnable)
		return 0;
	
	CStringArray aFiles;
	CString sText;
	
	SetSel(m_crDropSel);
	
	int nNumFiles = FileMisc::GetDropFilePaths((HDROP)wp, aFiles);

	::DragFinish((HDROP)wp);
	::CloseClipboard();

	if (nNumFiles > 0)
		return CRichEditHelper::PasteFiles(*this, aFiles, REP_ASFILEURL);

	// else 
	return nNumFiles;
}

void CUrlRichEditCtrl::PreSubclassWindow() 
{
	SetEventMask(GetEventMask() | ENM_CHANGE | ENM_DROPFILES | ENM_DRAGDROPDONE );
	DragAcceptFiles();
	
	// enable multilevel undo
	SendMessage(EM_SETTEXTMODE, TM_MULTILEVELUNDO);

	m_ncBorder.Initialize(GetSafeHwnd());

	CRichEditBaseCtrl::PreSubclassWindow();
}

LRESULT CUrlRichEditCtrl::OnSetText(WPARAM /*wp*/, LPARAM lp)
{
	// eat duplicate messages
	CString sText;
	GetWindowText(sText);
	
	BOOL bChange = (sText != (LPCTSTR)lp);
	LRESULT lr = 0;
	
	if (bChange)
	{
		CRichEditHelper::ClearUndo(GetSafeHwnd());
		
		lr = Default();
		ParseAndFormatText(TRUE);
	}
	
	return lr;
}

LRESULT CUrlRichEditCtrl::OnSetFont(WPARAM /*wp*/, LPARAM /*lp*/)
{
	LRESULT lr = Default();
	ParseAndFormatText(TRUE);
	
	return lr;
}

BOOL CUrlRichEditCtrl::FindEndOfUrl(LPCTSTR& szPos, int& nUrlLen, BOOL bBraced, BOOL bFile)
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
		while (!IsBaseDelim(szPos))
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

BOOL CUrlRichEditCtrl::IsBaseDelim(LPCTSTR szText)
{
	if (Misc::IsEmpty(szText))
		return TRUE; // end of string

	static LPCTSTR BASEDELIMS[] = 
	{ 
		_T(" "), 
		_T("\n"),
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

BOOL CUrlRichEditCtrl::IsFileProtocol(int nProtocol) const
{
	ASSERT((nProtocol >= 0) && (nProtocol < m_aProtocols.GetSize()));

	return ((nProtocol == m_nFileProtocol) || (nProtocol == m_nFileProtocol2));
}

void CUrlRichEditCtrl::ParseAndFormatText(BOOL bForceReformat)
{
	KillTimer(TIMER_REPARSE);
	AF_NOREENTRANT // prevent reentrancy
		
	// parse the control content
	CString sText;
	GetWindowText(sText);
	
	// richedit2 uses '\r\n' whereas richedit uses just '\n'
	if (!CWinClasses::IsClass(*this, WC_RICHEDIT))
		sText.Replace(_T("\r\n"), _T("\n"));
	
	// parse the text into an array of URLPOS
	CUrlArray aUrls;
	LPCTSTR szPos = sText;
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
		int nUrlStart = (szPos - (LPCTSTR)sText);
		BOOL bBraced = ((nUrlStart > 0) && (szPos[-1] == '<'));
		
		VERIFY(FindEndOfUrl(szPos, nUrlLen, bBraced, IsFileProtocol(nProt)));
		
		// Only save if the link is more than just the protocol
		if (nUrlLen > m_aProtocols[nProt].sProtocol.GetLength())
		{
			URLITEM urli;

			urli.cr.cpMin = nUrlStart;
			urli.cr.cpMax = (nUrlStart + nUrlLen);
			urli.sUrl = CString(szUrlStart, nUrlLen);
			urli.bWantNotify = m_aProtocols[nProt].bWantNotify;
			
			InsertInOrder(urli, aUrls);
		}

		bPrevCharWasDelim = TRUE;
	}
	
	// compare aUrls with m_aUrls to see if anything has changed
	BOOL bReformat = !sText.IsEmpty() && (bForceReformat || !UrlsMatch(aUrls));
	
	// then overwrite (always)	
	m_aUrls.Copy(aUrls);
	
	if (bReformat)
	{
		BOOL bVisible = IsWindowVisible();
		CRePauseUndo rep(*this);
		
		if (bVisible)
			SetRedraw(FALSE);
		
		// save current selection
		CHARRANGE crSel;
		GetSel(crSel);
		
		// and first line
		int nFirstLine = GetFirstVisibleLine();
		
		// save/reset event mask
		DWORD dwEventMask = SetEventMask(0);
		
		// retrieve default character attribs
		CHARFORMAT cf;
		cf.cbSize = sizeof(cf);
		cf.dwMask = CFM_LINK;
		
		// format urls
		int nUrls = aUrls.GetSize();
		CHARRANGE cr = { 0, 0 };
		
		for (int nUrl = 0; nUrl < nUrls; nUrl++)
		{
			// clear formatting from the end of the previous
			// url to the start of this url
			cr.cpMax = aUrls[nUrl].cr.cpMin;
			cf.dwEffects = 0;
			
			SetSel(cr);
			SetSelectionCharFormat(cf);
			
			// update for next url
			cr.cpMin = aUrls[nUrl].cr.cpMax;
			
			// then format url
			cf.dwEffects = CFM_LINK;
			
			SetSel(aUrls[nUrl].cr);
			SetSelectionCharFormat(cf);
		}	
		
		// clear formatting for whatever's left
		cr.cpMax = -1;
		cf.dwEffects = 0;
		
		SetSel(cr);
		SetSelectionCharFormat(cf);
		
		// restore selection
		SetSel(crSel);
		
		// and first line
		SetFirstVisibleLine(nFirstLine);
		
		// restore event mask
		SetEventMask(dwEventMask);

		if (bVisible)
		{
			SetRedraw(TRUE);
			Invalidate(FALSE);
		}
	}
}

BOOL CUrlRichEditCtrl::UrlsMatch(const CUrlArray& aUrls)
{
	int nUrls = aUrls.GetSize();

	if (nUrls !=  m_aUrls.GetSize())
		return FALSE;

	// compare aUrls with m_aUrls to see if anything has changed
	for (int nUrl = 0; nUrl < nUrls; nUrl++)
	{
		if (aUrls[nUrl].cr != m_aUrls[nUrl].cr)
			return FALSE;
	}

	// no change
	return TRUE;
}

void CUrlRichEditCtrl::InsertInOrder(URLITEM& urli, CUrlArray& aUrls)
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

BOOL CUrlRichEditCtrl::GoToUrl(int nUrl) const
{
	if (nUrl < 0 || nUrl >= m_aUrls.GetSize())
		return FALSE;
	
	const URLITEM& urli = m_aUrls[nUrl];
		
	if (!urli.bWantNotify)
	{
		CString sUrl = GetUrl(nUrl, TRUE);

		// if it fails to run then forward on to parent
		if (FileMisc::Run(*this, sUrl) > 32)
			return TRUE;

		// else
		SendNotifyFailedUrl(sUrl);
		return FALSE;
	}

	// else
	SendNotifyCustomUrl(urli.sUrl);
	return TRUE;
}

BOOL CUrlRichEditCtrl::CopyUrlToClipboard(int nUrl) const
{
	if (nUrl < 0 || nUrl >= m_aUrls.GetSize())
		return FALSE;
	
	return CClipboard(*this).SetText(GetUrl(nUrl, TRUE));
}

LRESULT CUrlRichEditCtrl::SendNotifyCustomUrl(LPCTSTR szUrl) const
{
	return GetParent()->SendMessage(WM_UREN_CUSTOMURL, GetDlgCtrlID(), (LPARAM)szUrl);
}

LRESULT CUrlRichEditCtrl::SendNotifyFailedUrl(LPCTSTR szUrl) const
{
	return GetParent()->SendMessage(WM_UREN_FAILEDURL, GetDlgCtrlID(), (LPARAM)szUrl);
}

/////////////////////////////////////////////////////////////////////////////
// CUrlRichEditCtrl

HRESULT CUrlRichEditCtrl::QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT* lpcfFormat, 
										 DWORD /*reco*/, BOOL fReally, HGLOBAL /*hMetaPict*/)
{
	BOOL bEnable = !(GetStyle() & ES_READONLY) && IsWindowEnabled();
		
	// always clear drag object
	m_lpDragObject = NULL;

	if (bEnable)
	{
		// snap-shot drag object when dragging
		if (!fReally)
			m_lpDragObject = lpdataobj;

   		*lpcfFormat = GetAcceptableClipFormat(lpdataobj, *lpcfFormat);

		// is this an outlook drop actually happening?
		if (fReally && (*lpcfFormat == CMSOutlookHelper::CF_OUTLOOK))
		{
			BOOL bEnable = !(GetStyle() & ES_READONLY) && IsWindowEnabled();
			ASSERT(bEnable);

			if (!bEnable)
				return E_FAIL;

			// insert outlook links at the cursor position
			SetSel(m_crDropSel);

			COleDataObject dataobj;
			dataobj.Attach(lpdataobj, FALSE);

			ASSERT(CMSOutlookHelper::IsOutlookObject(&dataobj));

			CMSOutlookHelper outlook;
			OutlookAPI::Selection* pSelection = outlook.GetSelection();
			
			CString sLinks;
			int nNumSel = pSelection->GetCount();

			// 1-based indexing
			for (short nSel = 1; nSel <= nNumSel; nSel++)
			{
				OutlookAPI::_Item item(pSelection->Item(COleVariant(nSel)));

				// format 'nicely' unless shift is pressed
				DWORD dwFlags = OAFMT_BRACED;

				if (!Misc::ModKeysArePressed(MKS_SHIFT))
					dwFlags |= OAFMT_NICE;

				sLinks += CMSOutlookHelper::FormatItemAsUrl(item, dwFlags);
				
				if ((nSel < nNumSel) || (nNumSel > 1))
					sLinks += '\n';
			}

			// cleanup
			delete pSelection;

			// trailing newline
			if (!sLinks.IsEmpty())
			{
				// leading and trailing spaces
				if (m_crDropSel.cpMin > 0)
				{
					if (nNumSel == 1)
						sLinks = ' ' + sLinks + ' ';
					else
						sLinks = '\n' + sLinks;
				}

				ReplaceSel(sLinks, TRUE);
				ParseAndFormatText();
				SetFocus();

				return S_OK;
			}
			else
				return E_FAIL;
		}

		return S_OK;
   }
	
   // else
   return E_FAIL;
}

CLIPFORMAT CUrlRichEditCtrl::GetAcceptableClipFormat(LPDATAOBJECT lpDataOb, CLIPFORMAT format)
{ 
	CLIPFORMAT formats[] = 
	{ 
		CF_HDROP,
			
#ifndef _UNICODE
		CF_TEXT,
#else
		CF_UNICODETEXT,
#endif
	};
	
	const long nNumFmts = sizeof(formats) / sizeof(CLIPFORMAT);
	
	// check for outlook first
	if (CMSOutlookHelper::IsOutlookObject(lpDataOb))
		return CMSOutlookHelper::CF_OUTLOOK;
    
	// else
	COleDataObject dataobj;
    dataobj.Attach(lpDataOb, FALSE);

	for (int nFmt = 0; nFmt < nNumFmts; nFmt++)
	{
		if (format && (format == formats[nFmt]))
			return format;
		
		FORMATETC fmtEtc = { formats[nFmt], 0 };

		if (dataobj.IsDataAvailable(formats[nFmt], &fmtEtc))
			return formats[nFmt];
	}
	
	return format;
}

HRESULT CUrlRichEditCtrl::GetDragDropEffect(BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect)
{
	if (!fDrag) // allowable dest effects 
	{
		BOOL bEnable = !(GetStyle() & ES_READONLY) && IsWindowEnabled();
		
		if (!bEnable)
		{
			*pdwEffect = DROPEFFECT_NONE;
		}
		else
		{
			DWORD dwEffect = DROPEFFECT_NONE;

			// test for outlook object first
			if (CMSOutlookHelper::IsOutlookObject(m_lpDragObject))
			{
				dwEffect = DROPEFFECT_LINK;
			}
			else
			{
				// we can deduce (I think) that what's being dragged is a file
				// by whether pdwEffect include the LINK effect.
				BOOL bFileDrop = Misc::HasFlag(*pdwEffect, DROPEFFECT_LINK);

				// if so save off the current selection pos (for now) because it gets reset
				// when the files are dropped
				if (bFileDrop)
				{
					// can't return DROPEFFECT_LINK else we don't
					// get notified of the file drop (go figure)
					dwEffect = DROPEFFECT_MOVE;

// 					// keep track of cursor
// 					TrackDragCursor();
				}
				else // it's text
				{
					BOOL bCtrl = Misc::HasFlag(grfKeyState, MK_CONTROL);

					dwEffect = (bCtrl ? DROPEFFECT_COPY : DROPEFFECT_MOVE);
				}
			}
			
			if (dwEffect & *pdwEffect) // make sure allowed type
				*pdwEffect = dwEffect;
		}
		
		// keep track of cursor
		if (*pdwEffect != DROPEFFECT_NONE)
			TrackDragCursor();
		else
			m_crDropSel.cpMin = m_crDropSel.cpMax = 0;
	}

	return S_OK;
}

void CUrlRichEditCtrl::TrackDragCursor()
{
	// also track the cursor for the drop position
	CPoint ptCursor(::GetMessagePos());
				
	ScreenToClient(&ptCursor);
	int nChar = CharFromPoint(ptCursor);
	m_crDropSel.cpMin = m_crDropSel.cpMax = nChar;

	SetFocus();
	SetSel(m_crDropSel);
	ShowCaret();
}

HRESULT CUrlRichEditCtrl::GetContextMenu(WORD /*seltype*/, LPOLEOBJECT /*lpoleobj*/, 
											CHARRANGE* /*lpchrg*/, HMENU* /*lphmenu*/)
{
	CPoint point = m_ptContextMenu;
	
	// send on as a simple context menu message
	SendMessage(WM_CONTEXTMENU, (WPARAM)GetSafeHwnd(), MAKELPARAM(point.x, point.y));
	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////

CString CUrlRichEditCtrl::GetUrl(int nURL, BOOL bAsFile) const
{
	ASSERT (nURL >= 0 && nURL < m_aUrls.GetSize());
	
	if ((nURL >= 0) && (nURL < m_aUrls.GetSize()))
	{
		CString sUrl(m_aUrls[nURL].sUrl);
		
		if (bAsFile && WebMisc::IsFileURI(sUrl))
		{
			CString sFilePath;

			if (WebMisc::DecodeFileURI(sUrl, sFilePath))
			{
				FileMisc::ExpandPathEnvironmentVariables(sFilePath);
				sUrl = sFilePath;
			}
		}
		
		return sUrl;
	}
	
	// else
	return "";
}

CString CUrlRichEditCtrl::CreateFileLink(LPCTSTR szFile)
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

void CUrlRichEditCtrl::Paste(BOOL bAppendSourceUrl)
{
	CString sClipText;
	
	// try pasting a file path
	if (CClipboard().GetText(sClipText) && FileMisc::IsPath(sClipText))
	{
		PathReplaceSel(sClipText, TRUE);
		return;
	}

	// Do the default paste
	CRichEditBaseCtrl::Paste();

	// If there is an associated URL then paste that after
	if (bAppendSourceUrl)
	{
		CString sClipURL;
		
		if (CClipboard().GetHTMLSourceLink(sClipURL))
			VERIFY(AppendSourceUrls(sClipURL));
	}
}

BOOL CUrlRichEditCtrl::PasteSimpleText(BOOL bAppendSourceUrl)
{
	// Do the default paste
	if (CRichEditBaseCtrl::PasteSimpleText())
	{
		// If there is an associated URL then paste that after
		if (bAppendSourceUrl)
		{
			CString sClipURL;
			
			if (CClipboard().GetHTMLSourceLink(sClipURL))
				VERIFY(AppendSourceUrls(sClipURL));
		}

		return TRUE;
	}
	
	return FALSE;
}

BOOL CUrlRichEditCtrl::AppendSourceUrls(LPCTSTR szUrls)
{
	if (Misc::IsEmpty(szUrls))
		return FALSE;

	CStringArray aLinks, aUnused;
	CString sLinks(szUrls), sReplace;

	Misc::Trim(sLinks);
	
	if (WebMisc::ExtractHtmlLinks(sLinks, aLinks, aUnused))
	{
		sReplace.Format(_T("\n( %s )\n"), Misc::FormatArray(aLinks, ' '));
	}
	else
	{
		sReplace.Format(_T("\n( %s )\n"), sLinks);
	}
	ReplaceSel(sReplace, TRUE);

	return TRUE;
}

void CUrlRichEditCtrl::AppendURLsToLinkText(CString& sLinkText, const CString& sURLs)
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

void CUrlRichEditCtrl::PathReplaceSel(LPCTSTR lpszPath, BOOL bFile)
{
	CString sPath(lpszPath);
	sPath.TrimLeft();

	if (bFile || FileMisc::IsPath(lpszPath))
		sPath = CreateFileLink(lpszPath);
	
	// add space fore and aft depending on selection
	CHARRANGE crSel, crSelOrg;
	GetSel(crSelOrg); // save this off
	GetSel(crSel);
	
	// enlarge to include end items
	if (crSel.cpMin > 0)
		crSel.cpMin--;
	
	if (crSel.cpMax < GetTextLength() - 1)
		crSel.cpMax++;
	
	SetSel(crSel);
	CString sSelText = GetSelText();
	SetSel(crSelOrg);
	
	// test
	if (!sSelText.IsEmpty())
	{
		if (!isspace(sSelText[0]))
			sPath = ' ' + sPath;
		
		if (!isspace(sSelText[sSelText.GetLength() - 1]))
			sPath += ' ';
	}
	
	ReplaceSel(sPath, TRUE);
	ParseAndFormatText();
	
	// set the new selection to be the dropped text
	SetSel(crSelOrg.cpMin, crSelOrg.cpMin + sPath.GetLength());
	SetFocus();
}

void CUrlRichEditCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_APPS)
	{
		m_ptContextMenu = GetCaretPos();
		
		// does this location lie on a url?
		m_nContextUrl = FindUrl(m_ptContextMenu);
		
		// convert point to screen coords for WM_CONTEXTMENU handling
		ClientToScreen(&m_ptContextMenu);
	}
	
	CRichEditBaseCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CUrlRichEditCtrl::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_F10 && GetKeyState(VK_SHIFT))
	{
		m_ptContextMenu = GetCaretPos();

		// does this location lie on a url?
		m_nContextUrl = FindUrl(m_ptContextMenu);

		// convert point to screen coords
		ClientToScreen(&m_ptContextMenu);

		// eat message else we'll get a WM_KEYUP with VK_APPS
	}

	CRichEditBaseCtrl::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CUrlRichEditCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_nContextUrl = FindUrl(point);
	
	// move the caret to the pos clicked
	int nChar = CharFromPoint(point);
	
	if (nChar >= 0)
	{
		// don't reset the selection if the character
		// falls within the current selection
		CHARRANGE crSel;
		GetSel(crSel);
		
		if (!ContainsPos(crSel, nChar))
			SetSel(nChar, nChar);
	}
	
	CRichEditBaseCtrl::OnRButtonDown(nFlags, point);
}

void CUrlRichEditCtrl::OnRButtonUp(UINT nHitTest, CPoint point) 
{
	m_ptContextMenu = point;
	ClientToScreen(&m_ptContextMenu);
	
	if (FindUrl(point) != m_nContextUrl)
		m_nContextUrl = -1;
	
	CRichEditBaseCtrl::OnRButtonUp(nHitTest, point);
}

void CUrlRichEditCtrl::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CRichEditBaseCtrl::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

int CUrlRichEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditBaseCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetEventMask(GetEventMask() | ENM_CHANGE | ENM_DROPFILES | ENM_DRAGDROPDONE | ENM_LINK);
	DragAcceptFiles();
	
	// enable multilevel undo
	SendMessage(EM_SETTEXTMODE, TM_MULTILEVELUNDO);

	m_ncBorder.Initialize(GetSafeHwnd());

	return 0;
}

void CUrlRichEditCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
{
	// if we arrived here then it means that noone had derived
	// from us and handled OnContextMenu. sow e must forward to 
	// our parent else we'll end up in a recursive loop
	GetParent()->SendMessage(WM_CONTEXTMENU, (WPARAM)GetSafeHwnd(), MAKELPARAM(point.x, point.y));
}

BOOL CUrlRichEditCtrl::OnNotifyLink(NMHDR* pNMHDR, LRESULT* pResult)
{
	BOOL bShift = Misc::IsKeyPressed(VK_SHIFT);
	ENLINK* pENL = (ENLINK*)pNMHDR;

	switch (pENL->msg)
	{
	case WM_SETCURSOR:
		if (bShift)
		{
			// because we're overriding the default behaviour we need to
			// handle the cursor being over a selected block
			CHARRANGE crSel;
			GetSel(crSel);

			CPoint ptCursor(GetMessagePos());
			ScreenToClient(&ptCursor);

			int nChar = CharFromPoint(ptCursor);
			LPCTSTR nCursor = (ContainsPos(crSel, nChar) ? IDC_ARROW : IDC_IBEAM);

			SetCursor(AfxGetApp()->LoadStandardCursor(nCursor));

			*pResult = TRUE;
			return TRUE;
		}
		break;

	case WM_LBUTTONDOWN:
		m_nContextUrl = FindUrl(pENL->chrg);
		
		// Shift allows clicking on the link without opening it
		if (bShift)
		{
			SetFocus();
			
			int nPos = CharFromPoint(pENL->lParam);
			SetSel(nPos, nPos);
			
			// eat
			m_nContextUrl = -1;
			return TRUE; // we handle it
		}
		break;

	case WM_LBUTTONUP:
		if (!bShift)
		{
			int nUrl = FindUrl(pENL->chrg);

			if (nUrl == -1)
			{
				CString sUrl = GetTextRange(pENL->chrg);
				FileMisc::Run(*this, sUrl);
			}
			else if (nUrl == m_nContextUrl)
			{
				if (!bShift)
					GoToUrl(m_nContextUrl);
			}
		}
		m_nContextUrl = -1;
		return TRUE; // we handle all links
	}

	return FALSE;
}

int CUrlRichEditCtrl::FindUrl(const CHARRANGE& cr) const
{
	int nUrl = m_aUrls.GetSize();
	
	while (nUrl--)
	{
		const URLITEM& urli = m_aUrls[nUrl];
		
		if (urli.cr == cr)
			return nUrl;
	}

	// not found
	return -1;
}

int CUrlRichEditCtrl::FindUrl(const CPoint& point) const
{
	int nPos = CharFromPoint(point);
	int nUrl = m_aUrls.GetSize();

	while (nUrl--)
	{
		const URLITEM& urli = m_aUrls[nUrl];

		if (ContainsPos(urli.cr, nPos))
			return nUrl;
	}

	// not found
	return -1;
}

void CUrlRichEditCtrl::OnTimer(UINT nIDEvent) 
{
	// if we've arrived here then it means that the user
	// has paused for long enough to reparse the latest changes
	if (nIDEvent == TIMER_REPARSE)
		ParseAndFormatText();
	
	CRichEditBaseCtrl::OnTimer(nIDEvent);
}
