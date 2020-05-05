// tdlcommentsctrl.cpp : implementation file
//

#include "stdafx.h"
#include "TDCSimpleTextContent.h"
#include "tdcmsg.h"
#include "tdcstatic.h"
#include "resource.h"

#include "..\shared\toolbarhelper.h"
#include "..\shared\enfiledialog.h"
#include "..\shared\wclassdefines.h"
#include "..\shared\autoflag.h"
#include "..\shared\richedithelper.h"
#include "..\shared\misc.h"
#include "..\shared\webmisc.h"
#include "..\shared\enstring.h"
#include "..\shared\filemisc.h"
#include "..\shared\enmenu.h"
#include "..\shared\clipboard.h"
#include "..\shared\localizer.h"
#include "..\shared\msoutlookhelper.h"

#include "..\Interfaces\ipreferences.h"
#include "..\Interfaces\ITaskList.h"
#include "..\Interfaces\richeditspellcheck.h"
#include "..\Interfaces\TasklistSchemaDef.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CTDCSimpleTextContent::CTDCSimpleTextContent()
{
	m_icon.Load(IDI_SIMPLETEXT);
}

CTDCSimpleTextContent::~CTDCSimpleTextContent()
{
}

LPCTSTR CTDCSimpleTextContent::GetTypeID() const 
{ 
	static LPCTSTR sID = _T("PLAIN_TEXT"); 
	return sID; 
}

LPCTSTR CTDCSimpleTextContent::GetTypeDescription() const 
{ 
	static LPCTSTR sDesc = _T("Simple Text"); 
	return sDesc; 
}

HICON CTDCSimpleTextContent::GetTypeIcon() const 
{ 
	return m_icon; 
}

IContentControl* CTDCSimpleTextContent::CreateCtrl(unsigned short nCtrlID, unsigned long nStyle, 
													long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent)
{
	CTDLSimpleTextContentCtrl* pComments = new CTDLSimpleTextContentCtrl;
	CRect rect(nLeft, nTop, nLeft + nWidth, nTop + nHeight);

	if (pComments->Create(nStyle, rect, CWnd::FromHandle(hwndParent), nCtrlID))
		return pComments;

	// else
	delete pComments;
	return NULL;
}

void CTDCSimpleTextContent::SetLocalizer(ITransText* pTT) 
{ 
	CLocalizer::Initialize(pTT); 
}

void CTDCSimpleTextContent::Release() 
{ 
	delete this; 
}

int CTDCSimpleTextContent::ConvertToHtml(const unsigned char* /*pContent*/, int /*nLength*/, LPCTSTR /*szCharset*/,
										LPTSTR& /*szHtml*/, LPCTSTR /*szImageDir*/) 
{ 
	return 0; // not supported
}

void CTDCSimpleTextContent::FreeHtmlBuffer(LPTSTR& /*szHtml*/) 
{
	// not required
}

void CTDCSimpleTextContent::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const 
{
	pPrefs->WriteProfileInt(szKey, _T("InlineSpellChecking"), CTDLSimpleTextContentCtrl::IsInlineSpellCheckingEnabled());
}

void CTDCSimpleTextContent::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly) 
{
	if (!bAppOnly)
		CTDLSimpleTextContentCtrl::EnableInlineSpellChecking(pPrefs->GetProfileInt(szKey, _T("InlineSpellChecking"), FALSE));

	CTDLSimpleTextContentCtrl::SetPasteSourceUrls(pPrefs->GetProfileInt(_T("Preferences"), _T("IncludeWebLinksInCommentsPaste"), TRUE));
}

/////////////////////////////////////////////////////////////////////////////

const UINT WM_SETWORDWRAP = (WM_USER+1);
const DWORD SIMPLETEXT_COMMENTS = (DWORD)(LPCTSTR)_T("SIMPLETEXT_COMMENTS");

/////////////////////////////////////////////////////////////////////////////

BOOL CTDLSimpleTextContentCtrl::s_bInlineSpellChecking = TRUE;
BOOL CTDLSimpleTextContentCtrl::s_bPasteSourceUrls = TRUE;

/////////////////////////////////////////////////////////////////////////////

static const CLIPFORMAT CF_PREFERRED[] = 
{ 
	CF_HDROP,
	CB_TEXTFORMAT,
};
const long NUM_PREF = sizeof(CF_PREFERRED) / sizeof(CLIPFORMAT);

/////////////////////////////////////////////////////////////////////////////

CTDLSimpleTextContentCtrl::CTDLSimpleTextContentCtrl() 
	: 
	CUrlRichEditCtrl(CTRLCLICKTOFOLLOW, IDS_CTRLCLICKTOFOLLOWLINK),
	m_bWordWrap(TRUE),
#pragma warning (disable: 4355)
	m_reSpellCheck(*this)
#pragma warning (default: 4355)
{
	// add custom protocol to comments field for linking to task IDs
	AddProtocol(TDL_PROTOCOL, TRUE);

	EnableSelectOnFocus(FALSE);
}

CTDLSimpleTextContentCtrl::~CTDLSimpleTextContentCtrl()
{
}

BEGIN_MESSAGE_MAP(CTDLSimpleTextContentCtrl, CUrlRichEditCtrl)
	//{{AFX_MSG_MAP(CTDLSimpleTextContentCtrl)
	ON_WM_CONTEXTMENU()
	ON_WM_SETCURSOR()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_HELPINFO()
	ON_WM_GETDLGCODE()
	ON_COMMAND_RANGE(ID_COMMENTS_CUT, ID_COMMENTS_LAST, OnCommentsMenuCmd)
	ON_UPDATE_COMMAND_UI_RANGE(ID_COMMENTS_CUT, ID_COMMENTS_LAST, OnUpdateCommentsMenuCmd)
	ON_CONTROL_REFLECT_EX(EN_CHANGE, OnChangeText)
	ON_CONTROL_REFLECT_EX(EN_KILLFOCUS, OnKillFocus)
	ON_MESSAGE(WM_SETWORDWRAP, OnSetWordWrap)
	ON_NOTIFY_REFLECT_EX(TTN_NEEDTEXT, OnGetTooltip)
	ON_WM_NCDESTROY()

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// IContentCtrl

int CTDLSimpleTextContentCtrl::GetTextContent(LPTSTR szContent, int nLength) const
{
	if (!szContent)
		return GetWindowTextLength();
	
	ASSERT(nLength > 0);
	return GetWindowText(szContent, nLength);
}

bool CTDLSimpleTextContentCtrl::SetTextContent(LPCTSTR szContent, bool bResetSelection) 
{ 
	CReSaveCaret resc(bResetSelection ? NULL : GetSafeHwnd());

	SendMessage(WM_SETTEXT, 0, (LPARAM)szContent);

	// reset caret
	if (bResetSelection)
		SetSel(0, 0);

	return true; 
}

bool CTDLSimpleTextContentCtrl::InsertTextContent(LPCWSTR szContent, bool bAtEnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (InsertSimpleText(szContent, (bAtEnd ? TRUE : FALSE)) != FALSE);
}

int CTDLSimpleTextContentCtrl::GetContent(unsigned char* /*pContent*/) const 
{ 
	return 0; 
}

bool CTDLSimpleTextContentCtrl::SetContent(const unsigned char* /*pContent*/, int /*nLength*/, bool /*bResetSelection*/) 
{ 
	return false; 
}

void CTDLSimpleTextContentCtrl::SetReadOnly(bool bReadOnly)
{
	CUrlRichEditCtrl::SetReadOnly(bReadOnly);

	SetBackgroundColor(!bReadOnly, GetSysColor(COLOR_3DFACE));
}

void CTDLSimpleTextContentCtrl::Enable(bool bEnable)
{
	EnableWindow(bEnable ? TRUE : FALSE);
}

HWND CTDLSimpleTextContentCtrl::GetHwnd() const 
{ 
	return GetSafeHwnd(); 
}

LPCTSTR CTDLSimpleTextContentCtrl::GetTypeID() const 
{ 
	return _T("PLAIN_TEXT"); 
}

bool CTDLSimpleTextContentCtrl::ProcessMessage(MSG* pMsg) 
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
				CopySimpleText();
				return TRUE;

			case 'v':
			case 'V':
				Paste();
				return TRUE;

			case 'x':
			case 'X':
				CutSimpleText();
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

void CTDLSimpleTextContentCtrl::FilterToolTipMessage(MSG* /*pMsg*/) 
{
	// Not supported
}

ISpellCheck* CTDLSimpleTextContentCtrl::GetSpellCheckInterface() 
{ 
	return &m_reSpellCheck; 
}

bool CTDLSimpleTextContentCtrl::Undo() 
{ 
	return (CUrlRichEditCtrl::Undo() != 0); 
}

bool CTDLSimpleTextContentCtrl::Redo() 
{ 
	return (CUrlRichEditCtrl::Redo() != 0); 
}

void CTDLSimpleTextContentCtrl::SetUITheme(const UITHEME* /*pTheme*/) 
{
	// Not supported
}

void CTDLSimpleTextContentCtrl::SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const
{
	pPrefs->WriteProfileInt(szKey, _T("WordWrap"), m_bWordWrap);
}

void CTDLSimpleTextContentCtrl::LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly)
{
	if (!bAppOnly)
	{
		BOOL bWordWrap = pPrefs->GetProfileInt(szKey, _T("WordWrap"), TRUE);

		// we need to post the wordwrap initialization else the richedit
		// get very confused about something and doesn't repaint properly
		// when resizing
		PostMessage(WM_SETWORDWRAP, bWordWrap, (LPARAM)GetSafeHwnd());
	}
}

bool CTDLSimpleTextContentCtrl::FindReplaceAll(LPCTSTR szFind, LPCTSTR szReplace, bool bCaseSensitive, bool bWholeWord)
{
	return (CUrlRichEditCtrl::ReplaceAll(szFind, szReplace, bCaseSensitive, bWholeWord) > 0);
}

/////////////////////////////////////////////////////////////////////////////
// CTDLSimpleTextContentCtrl message handlers

BOOL CTDLSimpleTextContentCtrl::OnHelpInfo(HELPINFO* /*lpHelpInfo*/)
{
	GetParent()->SendMessage(WM_ICC_DOHELP, 0, SIMPLETEXT_COMMENTS);
	return TRUE;
}

void CTDLSimpleTextContentCtrl::OnNcDestroy()
{
	CUrlRichEditCtrl::OnNcDestroy();

	delete this;
}

BOOL CTDLSimpleTextContentCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	dwStyle |= ES_NOHIDESEL | WS_HSCROLL | WS_VSCROLL | ES_WANTRETURN | ES_MULTILINE | WS_TABSTOP;

 	return CUrlRichEditCtrl::Create(dwStyle, rect, pParentWnd, nID);
}

void CTDLSimpleTextContentCtrl::SetContentFont(HFONT hFont)
{
	SendMessage(WM_SETFONT, (WPARAM)hFont);
}

BOOL CTDLSimpleTextContentCtrl::OnChangeText() 
{
	CUrlRichEditCtrl::OnChangeText();

	// If we don't have the focus and inline spell-checking
	// is enabled then assume that this is probably not a 
	// user edit so we don't pass it on
	if ((GetFocus() != this) && IsInlineSpellCheckingEnabled())
		return TRUE;
	
	if (IsWindowEnabled() && !(GetStyle() & ES_READONLY))
		GetParent()->SendMessage(WM_ICC_CONTENTCHANGE);
	
	return FALSE;
}

BOOL CTDLSimpleTextContentCtrl::OnKillFocus() 
{
	GetParent()->SendMessage(WM_ICC_KILLFOCUS);
	
	return FALSE; // continue routing
}

void CTDLSimpleTextContentCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
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
				if (m_sContextUrl.IsEmpty())
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

void CTDLSimpleTextContentCtrl::OnCommentsMenuCmd(UINT nCmdID) 
{
	switch (nCmdID)
	{
	case ID_COMMENTS_UNDO:
		if (CanEdit())
			CTextDocument(GetSafeHwnd()).Undo();
		break;

	case ID_COMMENTS_REDO:
		if (CanEdit())
			CTextDocument(GetSafeHwnd()).Redo();
		break;

	case ID_COMMENTS_CUT:
		CutSimpleText();
		break;

	case ID_COMMENTS_DELETE:
		if (CanEdit())
			ReplaceSel(_T(""));
		break;

	case ID_COMMENTS_FIND:
		DoEditFind(IDS_FIND_TITLE);
		break;
		
	case ID_COMMENTS_FINDREPLACE:
		DoEditReplace(IDS_REPLACE_TITLE);
		break;
		
	case ID_COMMENTS_COPY:
		CopySimpleText();
		break;
		
	case ID_COMMENTS_PASTE:
		Paste();
		break;
		
	case ID_COMMENTS_PASTEASREF:
		if (CanEdit())
		{
			// try to get the clipboard for any tasklist
			ITaskList* pClipboard = (ITaskList*)GetParent()->SendMessage(WM_ICC_GETCLIPBOARD, 0, FALSE);

			// verify that we can get the corresponding filename
			CString sFileName;
			ITaskList17* pClip17 = GetITLInterface<ITaskList17>(pClipboard, IID_TASKLIST17);

			if (pClip17)
			{
				sFileName = pClip17->GetFileName(false);
				sFileName.Replace(_T(" "), _T("%20"));
			}
			else // get the clipboard for just this tasklist
			{
				pClipboard = (ITaskList*)GetParent()->SendMessage(WM_ICC_GETCLIPBOARD, 0, TRUE);
			}

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
		if (!m_sContextUrl.IsEmpty())
			GoToUrl(m_sContextUrl);
		break;
		
	case ID_COMMENTS_COPYURL:
		if (!m_sContextUrl.IsEmpty())
			CClipboard(*this).SetText(m_sContextUrl);
		break;
		
	case ID_COMMENTS_FILEBROWSE:
		if (CanEdit())
		{
			CString sFile = CUrlParser::GetUrlAsFile(m_sContextUrl);
						
			CFileOpenDialog dialog(IDS_BROWSEFILE_TITLE, NULL, sFile, (EOFN_DEFAULTOPEN | OFN_ALLOWMULTISELECT));
			
			if (dialog.DoModal() == IDOK)
			{
				CStringArray aPaths;
				
				if (dialog.GetPathNames(aPaths) > 0)
				{
					SetFocus();
					CRichEditHelper::PasteFiles(*this, aPaths, REP_ASFILEURL, FALSE);
				}
			}
		}
		break;
		
	case ID_COMMENTS_SPELLCHECK:
		if (CanEdit())
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

void CTDLSimpleTextContentCtrl::SetWordWrap(BOOL bWrap)
{
	SetTargetDevice(NULL, bWrap ? 0 : 1);
	m_bWordWrap = bWrap;
}

void CTDLSimpleTextContentCtrl::OnUpdateCommentsMenuCmd(CCmdUI* pCmdUI)
{
	switch (pCmdUI->m_nID)
	{
	case ID_COMMENTS_UNDO:
		pCmdUI->Enable(CanEdit() && CanUndo());
		break;
					
	case ID_COMMENTS_REDO:
		pCmdUI->Enable(CanEdit() && SendMessage(EM_CANREDO));
		break;
					
	case ID_COMMENTS_CUT:
		pCmdUI->Enable(CanCutSelectedText());
		break;
		
	case ID_COMMENTS_COPY:
		pCmdUI->Enable(CanCopySelectedText());
		break;
		
	case ID_COMMENTS_PASTE:
		pCmdUI->Enable(CanPaste());
		break;
		
	case ID_COMMENTS_DELETE:
		pCmdUI->Enable(CanEdit());
		break;
		
	case ID_COMMENTS_PASTEASREF:
		pCmdUI->Enable(CanEdit() && !IsTDLClipboardEmpty());
		break;

	case ID_COMMENTS_FIND:
		pCmdUI->Enable(TRUE);
		break;
		
	case ID_COMMENTS_FINDREPLACE:
		pCmdUI->Enable(CanEdit());
		break;
		
	case ID_COMMENTS_SELECTALL:
		pCmdUI->Enable(GetTextLength());
		break;
		
	case ID_COMMENTS_SPELLCHECK:
		pCmdUI->Enable(GetTextLength() && CanEdit());
		break;
		
	case ID_COMMENTS_OPENURL:
	case ID_COMMENTS_COPYURL: 
		// if pCmdUI->m_pMenu is NOT null then we need to set the menu
		// text to the url, else it's just MFC checking that the option
		// is still enabled
		pCmdUI->Enable(!m_sContextUrl.IsEmpty());

		if (!m_sContextUrl.IsEmpty() && pCmdUI->m_pMenu)
		{
			CString sText, sMenu;
			pCmdUI->m_pMenu->GetMenuString(pCmdUI->m_nID, sMenu, MF_BYCOMMAND);
			
			// restrict url length to 250 pixels
			CEnString sUrl(m_sContextUrl);

			if (WebMisc::IsFileURI(sUrl))
				sUrl = CUrlParser::GetUrlAsFile(m_sContextUrl);

			CClientDC dc(this);
			sUrl.FormatDC(&dc, 250, ES_PATH);

			sText.Format(_T("%s: %s"), sMenu, sUrl);
			pCmdUI->SetText(sText);
		}
		break;
		
	case ID_COMMENTS_FILEBROWSE:
		pCmdUI->Enable(CanEdit());
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

BOOL CTDLSimpleTextContentCtrl::Paste()
{
	if (!CanEdit())
		return FALSE;
	
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
	return CRichEditHelper::PasteFiles(*this, aFiles, REP_ASFILEURL, FALSE);
}

BOOL CTDLSimpleTextContentCtrl::CanPaste()
{
	if (!CanEdit())
		return FALSE;

	// for reasons that I'm not entirely clear on even if we 
	// return that CF_HDROP is okay, the richedit itself will
	// veto the drop. So I'm experimenting with handling this ourselves
	if (CClipboard().HasFormat(CF_HDROP))
		return TRUE;

	// else
	return CanPasteSimpleText();
}


BOOL CTDLSimpleTextContentCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
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

BOOL CTDLSimpleTextContentCtrl::IsTDLClipboardEmpty() const 
{ 
	// try for any clipboard first
	ITaskList* pClipboard = (ITaskList*)GetParent()->SendMessage(WM_ICC_GETCLIPBOARD, 0, FALSE);
	ITaskList4* pClip4 = GetITLInterface<ITaskList4>(pClipboard, IID_TASKLIST4);

	if (pClip4)
		return (pClipboard->GetFirstTask() == NULL);

	// else try for 'our' clipboard only
	return (!GetParent()->SendMessage(WM_ICC_HASCLIPBOARD, 0, TRUE)); 
}

LRESULT CTDLSimpleTextContentCtrl::SendNotifyCustomUrl(LPCTSTR szUrl) const
{
	CString sUrl(szUrl);
	sUrl.MakeLower();

	if (sUrl.Find(TDL_PROTOCOL) != -1 || sUrl.Find(TDL_EXTENSION) != -1)
	{
		return GetParent()->SendMessage(WM_ICC_TASKLINK, 0, (LPARAM)(LPCTSTR)sUrl);
	}

	return SendNotifyFailedUrl(szUrl);
}

LRESULT CTDLSimpleTextContentCtrl::SendNotifyFailedUrl(LPCTSTR szUrl) const
{
	return GetParent()->SendMessage(WM_ICC_FAILEDLINK, (WPARAM)GetSafeHwnd(), (LPARAM)szUrl);
}

void CTDLSimpleTextContentCtrl::PreSubclassWindow() 
{
	CUrlRichEditCtrl::PreSubclassWindow();
}

LRESULT CTDLSimpleTextContentCtrl::OnSetWordWrap(WPARAM wp, LPARAM lp)
{
	UNREFERENCED_PARAMETER (lp);
	ASSERT (lp == (LPARAM)GetSafeHwnd());
	
	SetWordWrap(wp);
	return 0L;
}

int CTDLSimpleTextContentCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CUrlRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	LimitText(1024 * 1024 * 1024); // one gigabyte
	EnableToolTips();
	
	CUrlRichEditCtrl::EnableInlineSpellChecking(s_bInlineSpellChecking);
	
	return 0;
}

BOOL CTDLSimpleTextContentCtrl::OnGetTooltip(NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->idFrom == (UINT)GetDlgCtrlID());
	ASSERT(pNMHDR->hwndFrom == GetSafeHwnd());

	TOOLTIPTEXT* pTTN = (TOOLTIPTEXT*)pNMHDR;
	*pResult = 0;

	ICCLINKTOOLTIP tooltip = { 0 };
	tooltip.szLink = pTTN->lpszText;

	if (GetParent()->SendMessage(WM_ICC_GETLINKTOOLTIP, (WPARAM)GetSafeHwnd(), (LPARAM)&tooltip))
	{
		lstrcpyn(pTTN->szText, tooltip.szTooltip, ICCLINKTOOLTIPLEN);

		*pResult = TRUE;
		return TRUE;
	}

	return FALSE;
}

CLIPFORMAT CTDLSimpleTextContentCtrl::GetAcceptableClipFormat(LPDATAOBJECT lpDataOb, CLIPFORMAT format)
{ 
	// check for outlook first
	if (CMSOutlookHelper::IsOutlookObject(lpDataOb))
		return CMSOutlookHelper::CF_OUTLOOK;

	// FALSE ensures we only get what we allow
	return CRichEditBaseCtrl::GetAcceptableClipFormat(lpDataOb, format, CF_PREFERRED, NUM_PREF, FALSE);
}

UINT CTDLSimpleTextContentCtrl::OnGetDlgCode()
{
	return (DLGC_WANTALLKEYS & ~DLGC_HASSETSEL);
}
