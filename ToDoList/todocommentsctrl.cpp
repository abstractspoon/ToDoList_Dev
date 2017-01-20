// tdlcommentsctrl.cpp : implementation file
//

#include "stdafx.h"
#include "todocommentsctrl.h"
#include "tdcmsg.h"
#include "tdlschemadef.h"
#include "resource.h"

#include "..\shared\toolbarhelper.h"
#include "..\shared\enfiledialog.h"
#include "..\shared\wclassdefines.h"
#include "..\shared\autoflag.h"
#include "..\shared\richedithelper.h"
#include "..\shared\richeditspellcheck.h"
#include "..\shared\misc.h"
#include "..\shared\enstring.h"
#include "..\shared\filemisc.h"
#include "..\shared\enmenu.h"
#include "..\shared\clipboard.h"

#include "..\Interfaces\ipreferences.h"
#include "..\Interfaces\ITaskList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const UINT WM_SETWORDWRAP = (WM_USER+1);
const DWORD SIMPLETEXT_COMMENTS = (DWORD)(LPCTSTR)_T("SIMPLETEXT_COMMENTS");

/////////////////////////////////////////////////////////////////////////////

BOOL CToDoCommentsCtrl::s_bInlineSpellChecking = TRUE;
BOOL CToDoCommentsCtrl::s_bPasteSourceUrls = TRUE;

/////////////////////////////////////////////////////////////////////////////
// CToDoCommentsCtrl

CToDoCommentsCtrl::CToDoCommentsCtrl() 
	: 
	m_bAllowNotify(TRUE), 
	m_bWordWrap(TRUE),
#pragma warning (disable: 4355)
	m_reSpellCheck(*this)
#pragma warning (default: 4355)
{
	// add custom protocol to comments field for linking to task IDs
	AddProtocol(TDL_PROTOCOL, TRUE);

	EnableSelectOnFocus(FALSE);
}

CToDoCommentsCtrl::~CToDoCommentsCtrl()
{
}

/////////////////////////////////////////////////////////////////////////////
// IContentCtrl

int CToDoCommentsCtrl::GetTextContent(LPTSTR szContent, int nLength) const
{
	if (!szContent)
		return GetWindowTextLength();
	
	ASSERT(nLength > 0);
	return GetWindowText(szContent, nLength);
}

bool CToDoCommentsCtrl::SetTextContent(LPCTSTR szContent, bool bResetSelection) 
{ 
	CReSaveCaret resc(bResetSelection ? NULL : GetSafeHwnd());

	SendMessage(WM_SETTEXT, 0, (LPARAM)szContent);

	// reset caret
	if (bResetSelection)
		SetSel(0, 0);

	return true; 
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CToDoCommentsCtrl, CUrlRichEditCtrl)
	//{{AFX_MSG_MAP(CToDoCommentsCtrl)
	ON_WM_CONTEXTMENU()
	ON_WM_SETCURSOR()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_HELPINFO()
	ON_COMMAND_RANGE(ID_COMMENTS_CUT, ID_COMMENTS_LAST, OnCommentsMenuCmd)
	ON_UPDATE_COMMAND_UI_RANGE(ID_COMMENTS_CUT, ID_COMMENTS_LAST, OnUpdateCommentsMenuCmd)
	ON_CONTROL_REFLECT_EX(EN_CHANGE, OnChangeText)
	ON_CONTROL_REFLECT_EX(EN_KILLFOCUS, OnKillFocus)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_MESSAGE(WM_SETWORDWRAP, OnSetWordWrap)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToDoCommentsCtrl message handlers

BOOL CToDoCommentsCtrl::OnHelpInfo(HELPINFO* /*lpHelpInfo*/)
{
	GetParent()->SendMessage(WM_ICC_DOHELP, 0, SIMPLETEXT_COMMENTS);
	return TRUE;
}

BOOL CToDoCommentsCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	dwStyle |= ES_AUTOHSCROLL | WS_HSCROLL | ES_NOHIDESEL; 

 	return CUrlRichEditCtrl::Create(dwStyle, rect, pParentWnd, nID);
}

LRESULT CToDoCommentsCtrl::OnSetFont(WPARAM wp, LPARAM lp)
{
	// richedit2.0 sends a EN_CHANGE notification if it contains
	// text when it receives a font change.
	// to us though this is a bogus change so we prevent a notification
	// being sent
	CAutoFlag af(m_bAllowNotify, FALSE);

	return CUrlRichEditCtrl::OnSetFont(wp, lp);
}

BOOL CToDoCommentsCtrl::OnChangeText() 
{
	CUrlRichEditCtrl::OnChangeText();

	// If we don't have the focus and inline spell-checking
	// is enabled then assume that this is not a 'real' change
	// and don't pass it on
	if ((GetFocus() != this) && IsInlineSpellCheckingEnabled())
		return TRUE;


	if (m_bAllowNotify && IsWindowEnabled() && !(GetStyle() & ES_READONLY))
		GetParent()->SendMessage(WM_ICC_COMMENTSCHANGE);
	
	return FALSE;
}

BOOL CToDoCommentsCtrl::OnKillFocus() 
{
	if (m_bAllowNotify)
		GetParent()->SendMessage(WM_ICC_COMMENTSKILLFOCUS);
	
	return FALSE;
}

void CToDoCommentsCtrl::SetReadOnly(bool bReadOnly)
{
	CUrlRichEditCtrl::SetReadOnly(bReadOnly);

	SetBackgroundColor(!bReadOnly, GetSysColor(COLOR_3DFACE));
}

void CToDoCommentsCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (pWnd == this)
	{
		// build the context menu ourselves
		// note: we could use the edit menu from user32.dll but because
		// we're adding items too, the languages would appear odd
		CEnMenu menu;
		
		if (menu.LoadMenu(IDR_MISC, *this, TRUE))
		{
			CMenu* pPopup = menu.GetSubMenu(4);

			if (pPopup)
			{
#ifndef _DEBUG
				if (!SupportsInlineSpellChecking())
					pPopup->DeleteMenu(ID_COMMENTS_INLINESPELLCHECK, MF_BYCOMMAND);
#endif

				// Remove URL commands if URL not clicked on
				if (m_nContextUrl == -1)
				{
					pPopup->DeleteMenu(ID_COMMENTS_OPENURL, MF_BYCOMMAND);
					pPopup->DeleteMenu(ID_COMMENTS_COPYURL, MF_BYCOMMAND);
				}
				
				CToolbarHelper::PrepareMenuItems(pPopup, this);
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
			}
		}
	}
}

void CToDoCommentsCtrl::OnCommentsMenuCmd(UINT nCmdID) 
{
	switch (nCmdID)
	{
	case ID_COMMENTS_UNDO:
		CTextDocument(GetSafeHwnd()).Undo();
		break;

	case ID_COMMENTS_REDO:
		CTextDocument(GetSafeHwnd()).Redo();
		break;

	case ID_COMMENTS_CUT:
		Cut();
		break;

	case ID_COMMENTS_DELETE:
		ReplaceSel(_T(""));
		break;

	case ID_COMMENTS_FIND:
		DoEditFind(IDS_FIND_TITLE);
		break;
		
	case ID_COMMENTS_FINDREPLACE:
		DoEditReplace(IDS_REPLACE_TITLE);
		break;
		
	case ID_COMMENTS_COPY:
		{
			// copy text ourselves to prevent font attributes being copied as well
			CClipboard(*this).SetText(GetSelText());
		}
		break;
		
	case ID_COMMENTS_PASTE:
		Paste();
		break;
		
	case ID_COMMENTS_PASTEASREF:
		{
			// try to get the clipboard for any tasklist
			ITaskList* pClipboard = (ITaskList*)GetParent()->SendMessage(WM_TDCM_GETCLIPBOARD, 0, FALSE);

			// verify that we can get the corresponding filename
			CString sFileName;
			ITaskList4* pClip4 = GetITLInterface<ITaskList4>(pClipboard, IID_TASKLIST4);

			if (pClip4)
			{
				sFileName = pClip4->GetAttribute(TDL_FILENAME);
				sFileName.Replace(_T(" "), _T("%20"));
			}
			else // get the clipboard for just this tasklist
				pClipboard = (ITaskList*)GetParent()->SendMessage(WM_TDCM_GETCLIPBOARD, 0, TRUE);

			if (pClipboard && pClipboard->GetFirstTask())
			{
				// build single string containing each top level item as a different ref
				CString sRefs, sRef;
				HTASKITEM hClip = pClipboard->GetFirstTask();
				
				while (hClip)
				{
					if (sFileName.IsEmpty())
						sRef.Format(_T(" %s%lu"), TDL_PROTOCOL, pClipboard->GetTaskID(hClip));
					else
						sRef.Format(_T(" %s%s?%lu"), TDL_PROTOCOL, sFileName, pClipboard->GetTaskID(hClip));

					sRefs += sRef;
					
					hClip = pClipboard->GetNextTask(hClip);
				}

				sRefs += ' ';
				ReplaceSel(sRefs, TRUE);
			}
		}
		break;
		
	case ID_COMMENTS_SELECTALL:
		SetSel(0, -1);
		break;
		
	case ID_COMMENTS_OPENURL:
		if (m_nContextUrl != -1)
			GoToUrl(m_nContextUrl);
		
		// reset
		m_nContextUrl = -1;
		break;
		
	case ID_COMMENTS_COPYURL:
		if (m_nContextUrl != -1)
			CopyUrlToClipboard(m_nContextUrl);
		
		// reset
		m_nContextUrl = -1;
		break;
		
	case ID_COMMENTS_FILEBROWSE:
		{
			CString sFile;
			
			if (m_nContextUrl != -1)
			{
				sFile = GetUrl(m_nContextUrl, TRUE);

				if (FileMisc::FileExists(sFile))
					sFile.Empty();
			}
						
			CFileOpenDialog dialog(IDS_BROWSEFILE_TITLE, NULL, sFile, (EOFN_DEFAULTOPEN | OFN_ALLOWMULTISELECT));
			
			if (dialog.DoModal() == IDOK)
			{
				CStringArray aPaths;

				if (dialog.GetPathNames(aPaths) > 0)
					CRichEditHelper::PasteFiles(*this, aPaths, REP_ASFILEURL);
			}

			// reset
			m_nContextUrl = -1;
		}
		break;
		
	case ID_COMMENTS_SPELLCHECK:
		GetParent()->PostMessage(WM_ICC_WANTSPELLCHECK);
		break;

	case ID_COMMENTS_WORDWRAP:
		SetWordWrap(!m_bWordWrap);
		break;

	case ID_COMMENTS_INLINESPELLCHECK:
#ifndef _DEBUG
		if (SupportsInlineSpellChecking())
#endif
		{
			s_bInlineSpellChecking = !s_bInlineSpellChecking;
			EnableInlineSpellChecking(s_bInlineSpellChecking);
		}
		break;
	}
}

void CToDoCommentsCtrl::SetWordWrap(BOOL bWrap)
{
	SetTargetDevice(NULL, bWrap ? 0 : 1);
	m_bWordWrap = bWrap;
}

void CToDoCommentsCtrl::OnUpdateCommentsMenuCmd(CCmdUI* pCmdUI)
{
	BOOL bReadOnly = (GetStyle() & ES_READONLY) || !IsWindowEnabled();
	
	switch (pCmdUI->m_nID)
	{
	case ID_COMMENTS_UNDO:
		pCmdUI->Enable(CanUndo());
		break;
					
	case ID_COMMENTS_REDO:
		pCmdUI->Enable(SendMessage(EM_CANREDO));
		break;
					
	case ID_COMMENTS_CUT:
		if (!bReadOnly)
		{
			CHARRANGE crSel;
			GetSel(crSel);
			pCmdUI->Enable(crSel.cpMin != crSel.cpMax);
		}
		else
			pCmdUI->Enable(FALSE);
		break;
		
	case ID_COMMENTS_COPY:
		{
			CHARRANGE crSel;
			GetSel(crSel);
			pCmdUI->Enable(crSel.cpMin != crSel.cpMax);
		}
		break;
		
	case ID_COMMENTS_PASTE:
		pCmdUI->Enable(!bReadOnly && CanPaste());
		break;
		
	case ID_COMMENTS_PASTEASREF:
		pCmdUI->Enable(!bReadOnly && !IsTDLClipboardEmpty());
		break;
		
	case ID_COMMENTS_SELECTALL:
		pCmdUI->Enable(GetTextLength());
		break;
		
	case ID_COMMENTS_SPELLCHECK:
		pCmdUI->Enable(GetTextLength() && !bReadOnly);
		break;
		
	case ID_COMMENTS_OPENURL:
	case ID_COMMENTS_COPYURL: 
		// if pCmdUI->m_pMenu is NOT null then we need to set the menu
		// text to the url, else it's just MFC checking that the option
		// is still enabled
		pCmdUI->Enable(m_nContextUrl != -1);

		if ((m_nContextUrl != -1) && pCmdUI->m_pMenu)
		{
			CString sText, sMenu;
			pCmdUI->m_pMenu->GetMenuString(pCmdUI->m_nID, sMenu, MF_BYCOMMAND);
			
			// restrict url length to 250 pixels
			CEnString sUrl(GetUrl(m_nContextUrl, TRUE));
			CClientDC dc(this);
			sUrl.FormatDC(&dc, 250, ES_PATH);

			sText.Format(_T("%s: %s"), sMenu, sUrl);
			pCmdUI->SetText(sText);
		}
		break;
		
	case ID_COMMENTS_FILEBROWSE:
		pCmdUI->Enable(!bReadOnly);
		break;
		
	case ID_COMMENTS_WORDWRAP:
		pCmdUI->SetCheck(m_bWordWrap ? 1 : 0);
		break;
		
	case ID_COMMENTS_INLINESPELLCHECK:
#ifndef _DEBUG
		if (SupportsInlineSpellChecking())
		{
			pCmdUI->Enable(TRUE);
			pCmdUI->SetCheck(s_bInlineSpellChecking);
		}
		else
		{
			pCmdUI->SetCheck(FALSE);
			pCmdUI->Enable(FALSE);
		}
#else
		pCmdUI->Enable(TRUE);
		pCmdUI->SetCheck(s_bInlineSpellChecking);
#endif
		break;
	}
}

BOOL CToDoCommentsCtrl::Paste()
{
	CStringArray aFiles;
	int nNumFiles = CClipboard().GetDropFilePaths(aFiles);

	switch (nNumFiles)
	{
	case -1:
		AfxMessageBox(CEnString(IDS_PASTE_ERROR), MB_OK | MB_ICONERROR);
		return FALSE;

	case 0:
		return PasteSimpleText(s_bPasteSourceUrls);
	}

	// default > 0
	return CRichEditHelper::PasteFiles(*this, aFiles, REP_ASFILEURL);
}

BOOL CToDoCommentsCtrl::CanPaste()
{
	// for reasons that I'm not entirely clear on even if we 
	// return that CF_HDROP is okay, the richedit itself will
	// veto the drop. So I'm experimenting with handling this ourselves
	if (CClipboard().HasFormat(CF_HDROP))
		return TRUE;

	// else
	return CanPasteSimpleText();
}


BOOL CToDoCommentsCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// bit of a hack but this is what we get just before the context
	// menu appears so we set the cursor back to the arrow
	if (nHitTest == HTCAPTION)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW)));
		return TRUE;
	}
	
	return CUrlRichEditCtrl::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CToDoCommentsCtrl::IsTDLClipboardEmpty() const 
{ 
	// try for any clipboard first
	ITaskList* pClipboard = (ITaskList*)GetParent()->SendMessage(WM_TDCM_GETCLIPBOARD, 0, FALSE);
	ITaskList4* pClip4 = GetITLInterface<ITaskList4>(pClipboard, IID_TASKLIST4);

	if (pClip4)
		return (pClipboard->GetFirstTask() == NULL);

	// else try for 'our' clipboard only
	return (!GetParent()->SendMessage(WM_TDCM_HASCLIPBOARD, 0, TRUE)); 
}

LRESULT CToDoCommentsCtrl::SendNotifyCustomUrl(LPCTSTR szUrl) const
{
	CString sUrl(szUrl);
	sUrl.MakeLower();

	if (sUrl.Find(TDL_PROTOCOL) != -1 || sUrl.Find(TDL_EXTENSION) != -1)
	{
		return GetParent()->SendMessage(WM_TDCM_TASKLINK, 0, (LPARAM)(LPCTSTR)sUrl);
	}

	return 0;
}

LRESULT CToDoCommentsCtrl::SendNotifyFailedUrl(LPCTSTR szUrl) const
{
	return GetParent()->SendMessage(WM_TDCM_FAILEDLINK, (WPARAM)GetSafeHwnd(), (LPARAM)szUrl);
}

void CToDoCommentsCtrl::PreSubclassWindow() 
{
	CUrlRichEditCtrl::PreSubclassWindow();
}

void CToDoCommentsCtrl::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	pPrefs->WriteProfileInt(szKey, _T("WordWrap"), m_bWordWrap);
}

void CToDoCommentsCtrl::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	BOOL bWordWrap = pPrefs->GetProfileInt(szKey, _T("WordWrap"), TRUE);

	// we need to post the wordwrap initialization else the richedit
	// get very confused about something and doesn't repaint properly
	// when resizing
	PostMessage(WM_SETWORDWRAP, bWordWrap, (LPARAM)GetSafeHwnd());
}

LRESULT CToDoCommentsCtrl::OnSetWordWrap(WPARAM wp, LPARAM lp)
{
	UNREFERENCED_PARAMETER (lp);
	ASSERT (lp == (LPARAM)GetSafeHwnd());
	
	SetWordWrap(wp);
	return 0L;
}

int CToDoCommentsCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CUrlRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	CAutoFlag af(m_bAllowNotify, FALSE); // else we can get a false edit change

	// set max edit length
	LimitText(1024 * 1024 * 1024); // one gigabyte

	CUrlRichEditCtrl::EnableInlineSpellChecking(s_bInlineSpellChecking);
	
	return 0;
}

bool CToDoCommentsCtrl::ProcessMessage(MSG* pMsg) 
{
	if (!IsWindowEnabled())
		return false;

	// process editing shortcuts
	if (pMsg->message == WM_KEYDOWN)
	{
		BOOL bCtrl = Misc::IsKeyPressed(VK_CONTROL);
		BOOL bAlt = Misc::IsKeyPressed(VK_MENU);
		BOOL bEnabled = !(GetStyle() & ES_READONLY);

		if (bCtrl && !bAlt)
		{
			switch (pMsg->wParam)
			{
			case 'c': 
			case 'C':
				Copy();
				return TRUE;
				
			case 'v':
			case 'V':
				Paste();
				return TRUE;
				
			case 'x':
			case 'X':
				Copy();
				
				if (bEnabled)
				Clear();
				return TRUE;
				
			case 'a':
			case 'A':
				SetSel(0, -1);
				return TRUE;
				
			case 'h':
			case 'H':
				if (bEnabled)
				{
					DoEditReplace(IDS_REPLACE_TITLE);
					return TRUE;
				}
				// else fall thru
				
			case 'f':
			case 'F':
				DoEditFind(IDS_FIND_TITLE);
				return TRUE;
				
			case 'z':
			case 'Z':
				return TRUE;
				
			case 'y':
			case 'Y':
				return TRUE;
			}
		}
		else if (bEnabled && (pMsg->wParam == VK_TAB))
		{
			CHARRANGE cr;
			GetSel(cr);
			
			// if nothing is selected then just insert tabs
			CString sSel = _T("\t");

			if (cr.cpMax > cr.cpMin)
			{
				sSel += GetTextRange(cr);

				// and insert a tab character after every subsequent carriage return
				int nNumReplace = sSel.Replace(_T("\r"), _T("\r\t"));

				// bump selection by 'nNumReplace' to account for the tabs
				cr.cpMax += nNumReplace;
			}
			
			// bump selection by 1 to account for the tab
			cr.cpMin++;
			cr.cpMax++;

			ReplaceSel(sSel, TRUE);
			SetSel(cr);

			return TRUE;
		}
	}

	return false;
}
