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

CUrlRichEditCtrl::CUrlRichEditCtrl(URE_LINKHANDLING nLinkHandling, UINT nIDInstructionMsg)
	: 
	m_nLinkHandling(nLinkHandling),
	m_lpDragObject(NULL)
{
	if (nIDInstructionMsg)
		m_sLinkInstruction = CEnString(nIDInstructionMsg);
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
	ON_WM_CONTEXTMENU()
	ON_WM_SYSKEYDOWN()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
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
	return m_parser.AddProtocol(szProtocol, bWantNotify);
}

BOOL CUrlRichEditCtrl::OnChangeText() 
{
	if (!IsAutoUrlDetectionEnabled())
	{
		// kill any existing timer
		KillTimer(TIMER_REPARSE);

		// and start a new one
		SetTimer(TIMER_REPARSE, PAUSE, NULL);
	}
	
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
		return CRichEditHelper::PasteFiles(*this, aFiles, REP_ASFILEURL, FALSE);

	// else 
	return nNumFiles;
}

void CUrlRichEditCtrl::Initialise()
{
	CRichEditBaseCtrl::Initialise();

	SetEventMask(GetEventMask() | ENM_CHANGE | ENM_DROPFILES | ENM_DRAGDROPDONE | ENM_LINK);
	SendMessage(EM_SETTEXTMODE, TM_MULTILEVELUNDO);

	DragAcceptFiles();

	if (m_parser.HasProtocols())
	{
		CStringArray aProtocols;

		VERIFY(m_parser.GetProtocols(aProtocols));

		// This will fail below Windows 8 and we will parse ourselves
		CRichEditBaseCtrl::EnableAutoUrlDetection(aProtocols);
	}

	m_ncBorder.Initialize(GetSafeHwnd());
}

LRESULT CUrlRichEditCtrl::OnSetText(WPARAM wp, LPARAM lp)
{
	LRESULT lr = 0;

	if (IsAutoUrlDetectionEnabled())
	{
		lr = CRichEditBaseCtrl::OnSetText(wp, lp);
	}
	else
	{
		CString sText;
		GetWindowText(sText);
		
		// eat duplicate messages
		BOOL bChange = (sText != (LPCTSTR)lp);
		
		if (bChange)
		{
			CRichEditHelper::ClearUndo(GetSafeHwnd());
			
			lr = CRichEditBaseCtrl::OnSetText(wp, lp);

			ParseAndFormatText(TRUE);
		}
	}
	
	return lr;
}

LRESULT CUrlRichEditCtrl::OnSetFont(WPARAM wp, LPARAM lp)
{
	LRESULT lr = CRichEditBaseCtrl::OnSetFont(wp, lp);

	if (!IsAutoUrlDetectionEnabled())
	{
		ParseAndFormatText(TRUE);
	}
	
	return lr;
}

BOOL CUrlRichEditCtrl::FindStartOfUrl(LPCTSTR szText, int nTextLen, LPCTSTR& szPos) const
{
	ASSERT(szPos && szText);
	ASSERT(szPos >= szText);
	ASSERT(szPos < (szText + nTextLen));

	int nPos = (szPos - szText);

	if (IsStartOfLine(nPos))
	{
		szPos++;
	}
	else if (IsEndOfLine(nPos))
	{
		szPos--;
	}

	while (szPos > szText)
	{
		if (CUrlParser::IsBaseDelim(szPos - 1))
		{
			if (szPos[-1] == '<')
				szPos--;

			break;
		}

		szPos--;
	}

	return (m_parser.MatchProtocol(szPos) != -1);
}

void CUrlRichEditCtrl::ParseAndFormatText(BOOL bForceReformat)
{
	if (IsAutoUrlDetectionEnabled())
		return;

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
	m_parser.ParseText(sText, aUrls);
	
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

BOOL CUrlRichEditCtrl::UrlsMatch(const CUrlArray& aUrls) const
{
	ASSERT(!IsAutoUrlDetectionEnabled());

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

BOOL CUrlRichEditCtrl::GoToUrl(const CString& sUrl) const
{
	int nProtocol = m_parser.MatchProtocol(sUrl);
		
	// If it's an unknown protocol OR our parent wants to handle it
	// just forward it on
	if ((nProtocol == -1) || m_parser.ProtocolWantsNotification(nProtocol))
	{
		SendNotifyCustomUrl(sUrl);
		return TRUE;
	}

	// Handle Outlook manually because under Windows 10 ShellExecute 
	// will succeed even if Outlook is not installed
	if (CMSOutlookHelper::IsOutlookUrl(sUrl))
	{
		if (CMSOutlookHelper::HandleUrl(*this, sUrl))
			return TRUE;
	}
	else if (WebMisc::IsFileURI(sUrl))
	{
		if (FileMisc::Run(*this, CUrlParser::GetUrlAsFile(sUrl)) >= SE_ERR_SUCCESS)
		{
			return TRUE;
		}
	}
	else if (FileMisc::Run(*this, sUrl) >= SE_ERR_SUCCESS)
	{
		return TRUE;
	}

	// All else forward to parent as failures
	SendNotifyFailedUrl(sUrl);
	return FALSE;
}

LRESULT CUrlRichEditCtrl::SendNotifyCustomUrl(LPCTSTR szUrl) const
{
	return GetParent()->SendMessage(WM_UREN_CUSTOMURL, GetDlgCtrlID(), (LPARAM)szUrl);
}

LRESULT CUrlRichEditCtrl::SendNotifyFailedUrl(LPCTSTR szUrl) const
{
	return GetParent()->SendMessage(WM_UREN_FAILEDURL, GetDlgCtrlID(), (LPARAM)szUrl);
}

BOOL CUrlRichEditCtrl::GetUrlTooltip(const CString& sUrl, CString& sTooltip) const
{
	sTooltip.Empty();

	TOOLTIPTEXT tip = { 0 };

	tip.hdr.hwndFrom = GetSafeHwnd();
	tip.hdr.idFrom = GetDlgCtrlID();
	tip.hdr.code = TTN_NEEDTEXT;
	tip.lpszText = (LPTSTR)(LPCTSTR)sUrl;

	if (GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&tip))
	{
		if (tip.szText[0])
		{
			sTooltip = tip.szText;
		}
		else if (sUrl != tip.lpszText)
		{
			sTooltip = tip.lpszText;
		}
	}

	if (!m_sLinkInstruction.IsEmpty())
	{
		if (!sTooltip.IsEmpty())
			sTooltip += '\n';

		sTooltip += m_sLinkInstruction;
	}

	return !sTooltip.IsEmpty();
}

/////////////////////////////////////////////////////////////////////////////
// CUrlRichEditCtrl

HRESULT CUrlRichEditCtrl::QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT* lpcfFormat, 
										 DWORD /*reco*/, BOOL fReally, HGLOBAL /*hMetaPict*/)
{
	// always clear drag object
	m_lpDragObject = NULL;

	if (CanEdit())
	{
		// snap-shot drag object when dragging
		if (!fReally)
			m_lpDragObject = lpdataobj;

   		*lpcfFormat = GetAcceptableClipFormat(lpdataobj, *lpcfFormat);

		if (*lpcfFormat == 0)
			return E_FAIL;

		// is this an outlook drop actually happening?
		if (fReally && (*lpcfFormat == CMSOutlookHelper::CF_OUTLOOK))
		{
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
	// check for outlook first
	if (CMSOutlookHelper::IsOutlookObject(lpDataOb))
		return CMSOutlookHelper::CF_OUTLOOK;

	// else
	return format;
}

HRESULT CUrlRichEditCtrl::GetDragDropEffect(BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect)
{
	if (!fDrag) // allowable dest effects 
	{
		BOOL bEnable = (!(GetStyle() & ES_READONLY) && IsWindowEnabled());
		
		if (!bEnable)
		{
			*pdwEffect = DROPEFFECT_NONE;
		}
		else if (CMSOutlookHelper::IsOutlookObject(m_lpDragObject))
		{
			*pdwEffect = DROPEFFECT_LINK;
		}
		else
		{
			CLIPFORMAT cf = GetAcceptableClipFormat(m_lpDragObject, 0);

			switch (cf)
			{
			case 0:
				*pdwEffect = DROPEFFECT_NONE;
				break;

			case CF_HDROP:
				// can't return DROPEFFECT_LINK else we don't
				// get notified of the file drop (go figure)
				*pdwEffect = DROPEFFECT_MOVE;
				break;

			case CB_TEXTFORMAT:
				{
					if (Misc::HasFlag(grfKeyState, MK_CONTROL))
						*pdwEffect = DROPEFFECT_COPY;
					else
						*pdwEffect = DROPEFFECT_MOVE;
				}
				break;
			}
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

CString CUrlRichEditCtrl::GetContextUrl(BOOL bAsFile) const
{
	if (bAsFile)
		return CUrlParser::GetUrlAsFile(m_sContextUrl);
	
	// else
	return m_sContextUrl;
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

void CUrlRichEditCtrl::PathReplaceSel(LPCTSTR lpszPath, BOOL bFile)
{
	CString sPath(lpszPath);
	sPath.TrimLeft();

	if (bFile || FileMisc::IsPath(lpszPath))
		sPath = CUrlParser::CreateFileLink(lpszPath);
	
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
		if (!isspace(Misc::First(sSelText)))
			sPath = ' ' + sPath;
		
		if (!isspace(Misc::Last(sSelText)))
			sPath += ' ';
	}
	
	ReplaceSel(sPath, TRUE);

	if (!IsAutoUrlDetectionEnabled())
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
		if (SelectionHasEffect(CFM_LINK, CFE_LINK))
			m_sContextUrl = FindUrl(GetSelStart());
		else
			m_sContextUrl.Empty();

		ClientToScreen(&m_ptContextMenu); // for WM_CONTEXTMENU handling

	}
	
	CRichEditBaseCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CUrlRichEditCtrl::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ((nChar == VK_F10) && GetKeyState(VK_SHIFT))
	{
		m_ptContextMenu = GetCaretPos();
		
		// does this location lie on a url?
		if (SelectionHasEffect(CFM_LINK, CFE_LINK))
			m_sContextUrl = FindUrl(GetSelStart());
		else
			m_sContextUrl.Empty();
		
		ClientToScreen(&m_ptContextMenu); // for WM_CONTEXTMENU handling
	}

	CRichEditBaseCtrl::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CUrlRichEditCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// move the caret to the pos clicked unless
	// the character falls within the current selection
	if (!SelectionContainsPos(point))
	{
		int nPos = CharFromPoint(point);
		SetSel(nPos, nPos);
	}

	// OnNotifyLink will handle setting this
	m_sContextUrl.Empty();
	
	CRichEditBaseCtrl::OnRButtonDown(nFlags, point);
}

void CUrlRichEditCtrl::OnRButtonUp(UINT nHitTest, CPoint point) 
{
	m_ptContextMenu = point;
	ClientToScreen(&m_ptContextMenu);

	CRichEditBaseCtrl::OnRButtonUp(nHitTest, point);
}

void CUrlRichEditCtrl::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CRichEditBaseCtrl::OnShowWindow(bShow, nStatus);
}

void CUrlRichEditCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
{
	// if we arrived here then it means that noone had derived
	// from us and handled OnContextMenu. so we must forward to 
	// our parent else we'll end up in a recursive loop
	GetParent()->SendMessage(WM_CONTEXTMENU, (WPARAM)GetSafeHwnd(), MAKELPARAM(point.x, point.y));
}

BOOL CUrlRichEditCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// OnNotifyLink will update
	if (!Misc::IsKeyPressed(VK_LBUTTON) && !Misc::IsKeyPressed(VK_RBUTTON))
		m_sContextUrl.Empty();
	
	return CRichEditBaseCtrl::OnSetCursor(pWnd, nHitTest, message);
}

void CUrlRichEditCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// OnNotifyLink will update
	if (!Misc::IsKeyPressed(VK_LBUTTON) && !Misc::IsKeyPressed(VK_RBUTTON))
		m_sContextUrl.Empty();
	
	CRichEditBaseCtrl::OnMouseMove(nFlags, point);
}

BOOL CUrlRichEditCtrl::WantFollowLink(BOOL bCtrl, BOOL bShift) const
{
	if (bShift && bCtrl)
		return FALSE;

	switch (m_nLinkHandling)
	{
	case SHIFTCLICKTOEDIT:
		return !bShift;

	case CTRLCLICKTOFOLLOW:
		return bCtrl;
	}

	ASSERT(0);
	return FALSE;
}

BOOL CUrlRichEditCtrl::SelectionContainsMessagePos() const
{
	CPoint ptCursor(GetMessagePos());
	ScreenToClient(&ptCursor);

	return SelectionContainsPos(ptCursor);
}

BOOL CUrlRichEditCtrl::SelectionContainsPos(const CPoint& ptClient) const
{
	CHARRANGE crSel;
	GetSel(crSel);

	if (crSel.cpMin == crSel.cpMax)
		return FALSE;

	int nChar = CharFromPoint(ptClient);

	if (nChar == -1)
		return FALSE;

	return ContainsPos(crSel, nChar);
}

BOOL CUrlRichEditCtrl::OnNotifyLink(NMHDR* pNMHDR, LRESULT* pResult)
{
	BOOL bShift = Misc::IsKeyPressed(VK_SHIFT);
	BOOL bCtrl = Misc::IsKeyPressed(VK_CONTROL);

	ENLINK* pENL = (ENLINK*)pNMHDR;

	switch (pENL->msg)
	{
	case WM_SETCURSOR:
		m_sContextUrl = GetTextRange(pENL->chrg);

		if (!WantFollowLink(bCtrl, bShift))
		{
			// because we're overriding the default behaviour we need to
			// handle the cursor being over a selected block
			LPCTSTR nCursor = IDC_IBEAM;

			if (SelectionContainsMessagePos())
				nCursor = IDC_ARROW;

			SetCursor(AfxGetApp()->LoadStandardCursor(nCursor));
			*pResult = TRUE;

			return TRUE;
		}
		break;

	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
		m_sContextUrl = GetTextRange(pENL->chrg);
		break;

	case WM_LBUTTONDOWN:
		m_sContextUrl = GetTextRange(pENL->chrg);

		if (!WantFollowLink(bCtrl, bShift))
		{
			// If pENL->lParam is zero then this is a synthesized 
			// message in response to the <enter> key being pressed
			// when the caret is within a hyperlink
			if (pENL->lParam == 0)
			{
				// Insert a newline at the caret position
				ReplaceSel(_T("\r\n"), TRUE);
				return TRUE; // Handled
			}
			else if (!SelectionContainsPos(pENL->lParam))
			{
				SetFocus();
			
				int nPos = CharFromPoint(pENL->lParam);
				SetSel(nPos, nPos);
			
				// eat
				m_sContextUrl.Empty();
				return TRUE; // Handled
			}
		}
		// else default behaviour
		break;

	case WM_LBUTTONUP:
		if (WantFollowLink(bCtrl, bShift))
		{
			// Verify the same link is beneath the cursor
			CString sUrl = GetTextRange(pENL->chrg);

			if (!sUrl.IsEmpty() && (m_sContextUrl == sUrl))
				GoToUrl(m_sContextUrl);
		}
		return TRUE; // we handle all links
	}

	return FALSE;
}

CString CUrlRichEditCtrl::FindUrl(const CPoint& point) const
{
	return FindUrl(CharFromPoint(point));
}

CString CUrlRichEditCtrl::FindUrl(int nPos) const
{
	if (nPos >= 0)
	{
		if (IsAutoUrlDetectionEnabled())
		{
			CString sText;
			GetWindowText(sText);

			if (nPos < sText.GetLength())
			{
				LPCTSTR szStart = ((LPCTSTR)sText + nPos);

				if (FindStartOfUrl(sText, sText.GetLength(), szStart))
				{
					int nUrlLen = 0;
					BOOL bBraced = (szStart[0] == '<');
					BOOL bFile = m_parser.IsFileProtocol(szStart);

					LPCTSTR szEnd = szStart;

					if (CUrlParser::FindEndOfUrl(szEnd, nUrlLen, bBraced, bFile))
					{
						if (bBraced)
						{
							szStart++;
							nUrlLen--;
						}

						return CString(szStart, nUrlLen);
					}
				}
			}
		}
		else
		{
			int nUrl = m_aUrls.GetSize();
			
			while (nUrl--)
			{
				const URLITEM& urli = m_aUrls[nUrl];
				
				if (ContainsPos(urli.cr, nPos))
					return urli.sUrl;
			}
		}
	}

	// else
	return _T("");
}


void CUrlRichEditCtrl::OnTimer(UINT nIDEvent) 
{
	// if we've arrived here then it means that the user
	// has paused for long enough to reparse the latest changes
	if (nIDEvent == TIMER_REPARSE)
	{
		ASSERT(!IsAutoUrlDetectionEnabled());

		ParseAndFormatText();
	}
	
	CRichEditBaseCtrl::OnTimer(nIDEvent);
}

int CUrlRichEditCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	CString sTooltip;

	if (!m_sContextUrl.IsEmpty() && GetUrlTooltip(m_sContextUrl, sTooltip))
	{
		CRect rBounds(CPoint(point.x - 1, point.y - 1),CSize(2, 2));
		UINT nID = MAKELONG(point.x, point.y);

		return CToolTipCtrlEx::SetToolInfo(*pTI, this, sTooltip, nID, rBounds);
	}

	return CRichEditBaseCtrl::OnToolHitTest(point, pTI);
}
