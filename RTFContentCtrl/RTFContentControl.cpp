// RTFContentControl.cpp : implementation file
//

#include "stdafx.h"
#include "RTFContentCtrl.h"
#include "RTFContentControl.h"
#include "CreateFileLinkDlg.h"
#include "RTFPreferencesDlg.h"

#include "..\todolist\tdcmsg.h"
#include "..\todolist\tdlschemadef.h"

#include "..\shared\enfiledialog.h"
#include "..\shared\autoflag.h"
#include "..\shared\richedithelper.h"
#include "..\shared\webmisc.h"
#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\enstring.h"
#include "..\shared\preferences.h"
#include "..\shared\binarydata.h"
#include "..\shared\mswordhelper.h"
#include "..\shared\enmenu.h"
#include "..\shared\toolbarhelper.h"
#include "..\shared\clipboard.h"
#include "..\shared\localizer.h"

#include "..\Interfaces\uitheme.h"
#include "..\Interfaces\itasklist.h"

#include "..\3rdparty\compression.h"
#include "..\3rdparty\zlib\zlib.h"
#include "..\3rdparty\clipboardbackup.h"

#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const UINT PASTE_FMTS[] = 
{ 
	CBF_RTF,
	CBF_RETEXTOBJ, 
	CBF_EMBEDDEDOBJ,
	CF_BITMAP,
		
#ifndef _UNICODE
	CF_TEXT,
#else
	CF_UNICODETEXT,
#endif
	CF_DIB,
	CF_HDROP,
	CBF_HTML
};
const int NUM_PASTE_FMTS = (sizeof(PASTE_FMTS) / sizeof(PASTE_FMTS[0]));

/////////////////////////////////////////////////////////////////////////////
// CRTFContentControl

BOOL CRTFContentControl::s_bConvertWithMSWord = FALSE;
BOOL CRTFContentControl::s_bInlineSpellChecking = TRUE;
BOOL CRTFContentControl::s_bPasteSourceUrls = TRUE;

/////////////////////////////////////////////////////////////////////////////

CRTFContentControl::CRTFContentControl(CRtfHtmlConverter& rtfHtml) 
	: 
	m_bAllowNotify(TRUE), 
	m_reSpellCheck(m_rtf),
	m_mgrShortcuts(TRUE),
	m_rtfHtml(rtfHtml),
#pragma warning(disable:4355)
	m_dlgPrefs(this)
#pragma warning(default:4355)
{
	ZeroMemory(&m_lfCurrent, sizeof(LOGFONT));

	// add custom protocol to comments field for linking to task IDs
	m_rtf.AddProtocol(TDL_PROTOCOL, TRUE);

	CString sTooltip;
	sTooltip.LoadString(ID_HELP);
	Misc::Trim(sTooltip);

	CWinHelpButton::SetDefaultTooltip(CLocalizer::TranslateText((LPCTSTR)sTooltip));
}

CRTFContentControl::~CRTFContentControl()
{
}


BEGIN_MESSAGE_MAP(CRTFContentControl, CRulerRichEditCtrl)
	//{{AFX_MSG_MAP(CRTFContentControl)
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	//}}AFX_MSG_
	ON_COMMAND(ID_HELP, OnHelp)
	ON_WM_HELPINFO()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_COPYFORMATTING, OnEditCopyFormatting)
	ON_COMMAND(ID_EDIT_COPYASHTML, OnEditCopyAsHtml)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_COMMAND(ID_EDIT_FILEBROWSE, OnEditFileBrowse)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_COMMAND(ID_EDIT_FINDREPLACE, OnEditFindReplace)
	ON_COMMAND(ID_EDIT_HORZRULE, OnEditHorzRule)
	ON_COMMAND(ID_EDIT_OPENURL, OnEditOpenUrl)
	ON_COMMAND(ID_EDIT_COPYURL, OnEditCopyUrl)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_PASTEASREF, OnEditPasteasRef)
	ON_COMMAND(ID_EDIT_PASTEFORMATTING, OnEditPasteFormatting)
	ON_COMMAND(ID_EDIT_PASTESIMPLE, OnEditPasteSimple)
	ON_COMMAND(ID_EDIT_RIGHTALIGN, OnEditRightAlign)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_EDIT_SHOWRULER, OnEditShowRuler)
	ON_COMMAND(ID_EDIT_SHOWTOOLBAR, OnEditShowToolbar)
	ON_COMMAND(ID_EDIT_SPELLCHECK, OnEditSpellcheck)
	ON_COMMAND(ID_EDIT_SUBSCRIPT, OnEditSubscript)
	ON_COMMAND(ID_EDIT_SUPERSCRIPT, OnEditSuperscript)
	ON_COMMAND(ID_EDIT_INLINESPELLCHECK, OnEditInlineSpellcheck)
	ON_COMMAND(ID_PREFERENCES, OnPreferences)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPYFORMATTING, OnUpdateEditCopyFormatting)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FILEBROWSE, OnUpdateEditFileBrowse)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FINDREPLACE, OnUpdateEditFindReplace)
	ON_UPDATE_COMMAND_UI(ID_EDIT_HORZRULE, OnUpdateEditHorzRule)
	ON_UPDATE_COMMAND_UI(ID_EDIT_OPENURL, OnUpdateEditOpenCopyUrl)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPYURL, OnUpdateEditOpenCopyUrl)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTEASREF, OnUpdateEditPasteasRef)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTEFORMATTING, OnUpdateEditPasteFormatting)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTESIMPLE, OnUpdateEditPasteSimple)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SHOWRULER, OnUpdateEditShowRuler)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SHOWTOOLBAR, OnUpdateEditShowToolbar)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SPELLCHECK, OnUpdateEditSpellcheck)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SUBSCRIPT, OnUpdateEditSubscript)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SUPERSCRIPT, OnUpdateEditSuperscript)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INLINESPELLCHECK, OnUpdateEditInlineSpellcheck)
	ON_EN_CHANGE(RTF_CONTROL, OnChangeText)
	ON_EN_KILLFOCUS(RTF_CONTROL, OnKillFocus)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_STYLECHANGING()
	ON_REGISTERED_MESSAGE(WM_UREN_CUSTOMURL, OnCustomUrl)
	ON_REGISTERED_MESSAGE(WM_UREN_FAILEDURL, OnFailedUrl)
	ON_REGISTERED_MESSAGE(WM_RTF_PREFSHELP, OnPrefsHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRTFContentControl message handlers

LRESULT CRTFContentControl::OnPrefsHelp(WPARAM /*wp*/, LPARAM /*lp*/)
{
	OnHelp();
	return 0L;
}

void CRTFContentControl::OnHelp()
{
	CString sHelpID(GetTypeID());
	sHelpID += _T("_PREFS");
	
	GetParent()->SendMessage(WM_ICC_DOHELP, 0, (LPARAM)(LPCTSTR)sHelpID);
}

BOOL CRTFContentControl::OnHelpInfo(HELPINFO* /*lpHelpInfo*/)
{
	OnHelp();
	return TRUE;
}

void CRTFContentControl::OnChangeText() 
{
	if (m_bAllowNotify && !m_rtf.IsIMEComposing())
		GetParent()->SendMessage(WM_ICC_COMMENTSCHANGE);

	m_dlgPrefs.SetFileLinkOption(m_rtf.GetFileLinkOption(), !m_rtf.IsFileLinkOptionDefault());
}

void CRTFContentControl::OnKillFocus() 
{
	if (m_bAllowNotify)
		GetParent()->SendMessage(WM_ICC_COMMENTSKILLFOCUS);
}

LRESULT CRTFContentControl::OnSetFont(WPARAM wp, LPARAM lp)
{
	// richedit2.0 sends a EN_CHANGE notification if it contains
	// text when it receives a font change.
	// to us though this is a bogus change so we prevent a notification
	// being sent
	CAutoFlag af(m_bAllowNotify, FALSE);

	// save off font info
	if (wp)
		::GetObject((HFONT)wp, sizeof(LOGFONT), &m_lfCurrent);
	else
		ZeroMemory(&m_lfCurrent, sizeof(LOGFONT));

	return CRulerRichEditCtrl::OnSetFont(wp, lp);
}

// ICustomControl implementation
int CRTFContentControl::GetContent(unsigned char* pContent) const
{
	int nLen = 0;
	
	if (pContent)
	{
		CString sContent = m_rtf.GetRTF();
		nLen = sContent.GetLength();
		
		// compress it
		if (!nLen)
			return 0;
		
		unsigned char* pCompressed = NULL;
		int nLenCompressed = 0;
		
		if (Compression::Compress((unsigned char*)(LPSTR)(LPCTSTR)sContent, nLen, pCompressed, nLenCompressed) && nLenCompressed)
		{
			CopyMemory(pContent, pCompressed, nLenCompressed);
			nLen = nLenCompressed;
			delete [] pCompressed;
		}
		else
			nLen = 0;
	}
	else
	{
		nLen = m_rtf.GetRTFLength();
	}
	
	return nLen;
}

bool CRTFContentControl::SetContent(const unsigned char* pContent, int nLength, bool bResetSelection)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	unsigned char* pDecompressed = NULL;

	// content may need decompressing 
	// always work in bytes
	if (nLength && !m_rtfHtml.IsRTF((const char*)pContent))
	{
		int nLenDecompressed = 0;

		if (Compression::Decompress(pContent, nLength, pDecompressed, nLenDecompressed))
		{
			pContent = pDecompressed;
			nLength = nLenDecompressed;
		}
		else
			return false;
	}

	// content must begin with rtf tag or be empty
	if (nLength && !m_rtfHtml.IsRTF((const char*)pContent))
		return false;

	CAutoFlag af(m_bAllowNotify, FALSE);
	CReSaveCaret resc(bResetSelection ? NULL : m_rtf.GetSafeHwnd());
	CString sContent;

#ifdef _UNICODE
	CBinaryData(pContent, nLength).Get(sContent);
#else
	memcpy(sContent.GetBufferSetLength(nLength), pContent, nLength);
#endif

	SetRTF(sContent);

	delete [] pDecompressed;
	
	// reset caret
	if (bResetSelection)
		m_rtf.SetSel(0, 0);

	return true; 
}

int CRTFContentControl::GetTextContent(LPTSTR szContent, int nLength) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (!szContent)
		return m_rtf.GetWindowTextLength();

	ASSERT(nLength > 0);
	
	return m_rtf.GetWindowText(szContent, nLength);
}

bool CRTFContentControl::SetTextContent(LPCTSTR szContent, bool bResetSelection)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CAutoFlag af(m_bAllowNotify, TRUE);
	CReSaveCaret resc(bResetSelection ? NULL : m_rtf.GetSafeHwnd());

	m_rtf.SendMessage(WM_SETTEXT, 0, (LPARAM)szContent);

	// reset caret
	if (bResetSelection)
		m_rtf.SetSel(0, 0);

	return true; 
}

bool CRTFContentControl::Undo() 
{ 
	return (m_rtf.Undo() != FALSE); 
}

bool CRTFContentControl::Redo() 
{ 
	return (m_rtf.Redo() != FALSE); 
}

void CRTFContentControl::SetUITheme(const UITHEME* pTheme)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
 	if (FileMisc::FileExists(pTheme->szToolbarImage))
 	{
 		m_toolbar.DestroyWindow();
 		m_toolbar.Create(this, pTheme->szToolbarImage, pTheme->crToolbarTransparency);
 	}

	m_toolbar.SetBackgroundColors(pTheme->crToolbarLight, pTheme->crToolbarDark, pTheme->nRenderStyle != UIRS_GLASS, pTheme->nRenderStyle != UIRS_GRADIENT);
	m_toolbar.SetHotColor(pTheme->crToolbarHot);
	m_ruler.SetBackgroundColor(pTheme->crToolbarLight);
}

HWND CRTFContentControl::GetHwnd() const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return GetSafeHwnd();
}

void CRTFContentControl::SetReadOnly(bool bReadOnly)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CRulerRichEditCtrl::SetReadOnly((BOOL)bReadOnly);
}

void CRTFContentControl::Release()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DestroyWindow();
	delete this;
}

void CRTFContentControl::EnableMenuItem(CMenu* pMenu, UINT nCmdID, BOOL bEnable)
{
	pMenu->EnableMenuItem(nCmdID, MF_BYCOMMAND | (bEnable ? MF_ENABLED : MF_GRAYED));
}

void CRTFContentControl::CheckMenuItem(CMenu* pMenu, UINT nCmdID, BOOL bCheck)
{
	pMenu->CheckMenuItem(nCmdID, MF_BYCOMMAND | (bCheck ? MF_CHECKED  : MF_UNCHECKED));
}

void CRTFContentControl::RemoveAdvancedFeatures(CMenu* pMenu) const
{
#ifdef _DEBUG
	UNREFERENCED_PARAMETER(pMenu);
#else
	BOOL bRemoveAdvancedFeatures = ((FileMisc::GetModuleDriveType() == DRIVE_FIXED) && 
									!CMSWordHelper::IsWordInstalled(12));
	
	if (bRemoveAdvancedFeatures)
	{
		CEnMenu::DeleteMenu(*pMenu, ID_EDIT_SUPERSCRIPT, MF_BYCOMMAND, TRUE);
		CEnMenu::DeleteMenu(*pMenu, ID_EDIT_SUBSCRIPT, MF_BYCOMMAND, TRUE);
		CEnMenu::DeleteMenu(*pMenu, ID_EDIT_INSERTTABLE, MF_BYCOMMAND, TRUE);
		CEnMenu::DeleteMenu(*pMenu, ID_EDIT_HORZRULE, MF_BYCOMMAND, TRUE);
	}
	
	if (!m_rtf.SupportsInlineSpellChecking())
		pMenu->DeleteMenu(ID_EDIT_INLINESPELLCHECK, MF_BYCOMMAND);
#endif
}

void CRTFContentControl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (pWnd == &m_rtf)
	{
		// prepare a simple edit menu
		CMenu menu;

		if (menu.LoadMenu(IDR_POPUP))
		{
			CMenu* pPopup = menu.GetSubMenu(0);

			if (pPopup)
			{
				RemoveAdvancedFeatures(pPopup);

				// check pos
				if (point.x == -1 && point.y == -1)
				{
					point = GetCaretPos();
					::ClientToScreen(m_rtf, &point);
				}

				// Remove URL commands if URL not clicked on
				if (m_rtf.GetContextUrl() == -1)
				{
					pPopup->DeleteMenu(ID_EDIT_OPENURL, MF_BYCOMMAND);
					pPopup->DeleteMenu(ID_EDIT_COPYURL, MF_BYCOMMAND);
				}

				CToolbarHelper::PrepareMenuItems(pPopup, this);
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
			}
		}
	}
}

void CRTFContentControl::InitShortcutManager()
{
	if (!m_mgrShortcuts.Initialize(this, NULL, NULL, 0))
		return;

	m_mgrShortcuts.AddShortcut(ID_EDIT_COPYFORMATTING,	'C',		HOTKEYF_CONTROL | HOTKEYF_SHIFT);
	m_mgrShortcuts.AddShortcut(ID_EDIT_PASTEFORMATTING,	'V',		HOTKEYF_CONTROL | HOTKEYF_SHIFT);
	m_mgrShortcuts.AddShortcut(ID_EDIT_OUTDENT,			'J',		HOTKEYF_CONTROL | HOTKEYF_SHIFT); 
	m_mgrShortcuts.AddShortcut(ID_EDIT_COPY,			'C',		HOTKEYF_CONTROL); 
	m_mgrShortcuts.AddShortcut(ID_EDIT_COPYASHTML,		'C',		HOTKEYF_CONTROL | HOTKEYF_ALT); 
	m_mgrShortcuts.AddShortcut(ID_EDIT_PASTE,			'V',		HOTKEYF_CONTROL); 
	m_mgrShortcuts.AddShortcut(ID_EDIT_PASTESIMPLE,		VK_INSERT,	HOTKEYF_SHIFT | HOTKEYF_EXT); 
	m_mgrShortcuts.AddShortcut(ID_EDIT_CUT,				'X',		HOTKEYF_CONTROL); 
	m_mgrShortcuts.AddShortcut(ID_EDIT_SELECT_ALL,		'A',		HOTKEYF_CONTROL); 
	m_mgrShortcuts.AddShortcut(ID_EDIT_BOLD,			'B',		HOTKEYF_CONTROL); 
	m_mgrShortcuts.AddShortcut(ID_EDIT_ITALIC,			'I',		HOTKEYF_CONTROL); 
	m_mgrShortcuts.AddShortcut(ID_EDIT_UNDERLINE,		'U',		HOTKEYF_CONTROL); 
	m_mgrShortcuts.AddShortcut(ID_EDIT_LEFTALIGN,		'L',		HOTKEYF_CONTROL); 
	m_mgrShortcuts.AddShortcut(ID_EDIT_CENTERALIGN,		'E',		HOTKEYF_CONTROL); 
	m_mgrShortcuts.AddShortcut(ID_EDIT_RIGHTALIGN,		'R',		HOTKEYF_CONTROL); 
	m_mgrShortcuts.AddShortcut(ID_EDIT_STRIKETHRU,		0xBD,		HOTKEYF_CONTROL); 
	m_mgrShortcuts.AddShortcut(ID_EDIT_GROWFONT,		0xBE,		HOTKEYF_CONTROL | HOTKEYF_EXT); 
	m_mgrShortcuts.AddShortcut(ID_EDIT_SHRINKFONT,		0xBC,		HOTKEYF_CONTROL | HOTKEYF_EXT); 
	m_mgrShortcuts.AddShortcut(ID_EDIT_FINDREPLACE,		'H',		HOTKEYF_CONTROL); 
	m_mgrShortcuts.AddShortcut(ID_EDIT_FIND,			'F',		HOTKEYF_CONTROL); 
	m_mgrShortcuts.AddShortcut(ID_EDIT_INDENT,			'J',		HOTKEYF_CONTROL); 
}

void CRTFContentControl::InitMenuIconManager()
{
	if (!m_mgrMenuIcons.Initialize(this))
		return;
	
	m_mgrMenuIcons.ClearImages();
	
	CUIntArray aCmdIDs;
	
	aCmdIDs.Add(ID_EDIT_FONT);
	aCmdIDs.Add(ID_EDIT_BOLD);
	aCmdIDs.Add(ID_EDIT_ITALIC);
	aCmdIDs.Add(ID_EDIT_UNDERLINE);
	aCmdIDs.Add(ID_EDIT_STRIKETHRU);
	aCmdIDs.Add(ID_EDIT_GROWFONT);
	aCmdIDs.Add(ID_EDIT_SHRINKFONT);
	aCmdIDs.Add(ID_EDIT_LEFTALIGN);
	aCmdIDs.Add(ID_EDIT_CENTERALIGN);
	aCmdIDs.Add(ID_EDIT_RIGHTALIGN);
	aCmdIDs.Add(ID_EDIT_JUSTIFY);
	aCmdIDs.Add(ID_EDIT_BULLET);
	aCmdIDs.Add(ID_EDIT_NUMBER);
	aCmdIDs.Add(ID_EDIT_OUTDENT);
	aCmdIDs.Add(ID_EDIT_INDENT);
	aCmdIDs.Add(ID_EDIT_INSERTTABLE);
	aCmdIDs.Add(ID_EDIT_TEXTCOLOR);
	aCmdIDs.Add(ID_EDIT_BACKCOLOR);
	aCmdIDs.Add(ID_EDIT_WORDWRAP);
	aCmdIDs.Add(ID_EDIT_INLINESPELLCHECK);
	aCmdIDs.Add(ID_PREFERENCES);
	aCmdIDs.Add(ID_HELP);
		
	m_mgrMenuIcons.AddImages(aCmdIDs, IDB_TOOLBAR, RGB(255, 0, 255));
}

BOOL CRTFContentControl::IsTDLClipboardEmpty() const 
{ 
	// try for any clipboard first
	ITaskList* pClipboard = (ITaskList*)GetParent()->SendMessage(WM_TDCM_GETCLIPBOARD, 0, FALSE);
	ITaskList4* pClip4 = GetITLInterface<ITaskList4>(pClipboard, IID_TASKLIST4);

	if (pClip4)
		return (pClipboard->GetFirstTask() == NULL);

	// else try for 'our' clipboard only
	return (!GetParent()->SendMessage(WM_TDCM_HASCLIPBOARD, 0, TRUE)); 
}

BOOL CRTFContentControl::GetClipboardHtmlForPasting(CString& sHtml, CString& sSourceUrl)
{
	CClipboard cb;
	
	if (!cb.HasFormat(CBF_RTF) && 
		!cb.HasFormat(CBF_RETEXTOBJ) && 
		!cb.HasFormat(CBF_EMBEDDEDOBJ) &&
		!cb.HasFormat(CF_DIB) &&
		!cb.HasFormat(CF_BITMAP) &&
		cb.GetText(sHtml, CBF_HTML))
	{
#ifdef _UNICODE
		// convert to unicode for unpackaging because
		// CF_HTML is saved to the clipboard as UTF8
		Misc::EncodeAsUnicode(sHtml, CP_UTF8);
#endif
		Misc::Trim(sHtml);

		cb.UnpackageHTMLFragment(sHtml, sSourceUrl);
		cb.Close();
		
		if (!sHtml.IsEmpty())
		{
#ifdef _UNICODE
			// convert back to UTF8 for translation
			Misc::EncodeAsMultiByte(sHtml, CP_UTF8);
#endif
			return TRUE;
		}
	}

	// all else
	return FALSE;
}

BOOL CRTFContentControl::Paste(BOOL bSimple)
{
	CStringArray aFiles;
	int nNumFiles = CClipboard().GetDropFilePaths(aFiles);
	
	switch (nNumFiles)
	{
	case -1:
		AfxMessageBox(IDS_PASTE_ERROR, MB_OK | MB_ICONERROR);
		return FALSE;
		
	case 0:
		if (bSimple)
		{
			m_rtf.PasteSimpleText(s_bPasteSourceUrls);
		}
		else // may need conversion
		{
			// if there is HTML but not RTF or an image then convert the 
			// HTML to RTF and add it to the current clipboard contents
			CClipboardBackup cbb(*this);
			BOOL bClipboardSaved = FALSE;

			CString sHtml, sSourceUrl;
			
			if (GetClipboardHtmlForPasting(sHtml, sSourceUrl))
			{
				// Always set this to make sure it is current
				m_rtfHtml.SetAllowUseOfMSWord(s_bConvertWithMSWord);

				CString sRTF;
				
				if (m_rtfHtml.ConvertHtmlToRtf((LPCSTR)(LPCTSTR)sHtml, NULL, sRTF, NULL))
				{
#ifdef _UNICODE
					// convert back to Ansi (not UTF8) for clipboard
					Misc::EncodeAsMultiByte(sRTF);
#endif
					// backup the current clipboard
					bClipboardSaved = cbb.Backup();
					ASSERT(bClipboardSaved);
					
					// overwrite with RTF
					CClipboard(*this).SetText(sRTF, CBF_RTF);
				}
			}

			// do the actual paste
			m_rtf.Paste(s_bPasteSourceUrls);

			// restore the clipboard
			if (bClipboardSaved)
				VERIFY(cbb.Restore());
		}
		return TRUE;
		
	default: // 1 or more filenames 
		if (bSimple)
		{
			return CRichEditHelper::PasteFiles(m_rtf, aFiles, REP_ASFILEURL);
		}
		else
		{
			if (!m_rtf.IsFileLinkOptionDefault())
			{
				CCreateFileLinkDlg dialog(aFiles[0], m_rtf.GetFileLinkOption(), FALSE);
				
				if (dialog.DoModal() != IDOK)
					return FALSE; // cancelled

				// else
				RE_PASTE nLinkOption = dialog.GetLinkOption();
				BOOL bDefault = dialog.GetMakeLinkOptionDefault();
					
				m_rtf.SetFileLinkOption(nLinkOption, bDefault);
			}
			
			return CRichEditHelper::PasteFiles(m_rtf, aFiles, m_rtf.GetFileLinkOption());
		}
	}
}

BOOL CRTFContentControl::CanPaste()
{
	// for reasons that I'm not entirely clear on even if we 
	// return that CF_HDROP is okay, the richedit itself will
	// veto the drop. So I'm experimenting with handling this ourselves
	if (CClipboard::HasFormat(CF_HDROP))
		return TRUE;

	// else try richedit itself
	BOOL bCanPaste = FALSE;
	
	for (int i=0; i < NUM_PASTE_FMTS; i++)
		bCanPaste |= m_rtf.CanPaste(PASTE_FMTS[i]);

	if (!bCanPaste)
		bCanPaste = m_rtf.CanPaste(0);
	
	return bCanPaste;
}

int CRTFContentControl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CAutoFlag af(m_bAllowNotify, FALSE);
	
	if (CRulerRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_rtf.SetEventMask(m_rtf.GetEventMask() | ENM_CHANGE);
	
	// set max edit length
	m_rtf.LimitText(1024 * 1024 * 1024); // one gigabyte
	
	InitMenuIconManager();
	InitShortcutManager();

#ifndef _DEBUG
	// if MS word is not installed we remove features that
	// our backup RTF2HTML converter cannot handle
	BOOL bRemoveAdvancedFeatures = !CMSWordHelper::IsWordInstalled(12);

	if (bRemoveAdvancedFeatures)
	{
		int nBtn = m_toolbar.CommandToIndex(ID_EDIT_INSERTTABLE);

		m_toolbar.GetToolBarCtrl().DeleteButton(nBtn); // button itself
		m_toolbar.GetToolBarCtrl().DeleteButton(nBtn); // trailing separator
	}

	// If inline spell-checking is not supported then remove
	// the appropriate toolbar button
	if (!m_rtf.SupportsInlineSpellChecking())
	{
		int nBtn = m_toolbar.CommandToIndex(ID_EDIT_INLINESPELLCHECK);

		m_toolbar.GetToolBarCtrl().DeleteButton(nBtn);
	}
#endif
	m_rtf.EnableInlineSpellChecking(s_bInlineSpellChecking);

	// helper for toolbar tooltips
	// initialize after hiding table button
	m_tbHelper.Initialize(&m_toolbar, this, &m_mgrShortcuts);

	return 0;
}

bool CRTFContentControl::ProcessMessage(MSG* pMsg) 
{
	if (!IsWindowEnabled())
		return false;

	// process editing shortcuts
	if (m_mgrShortcuts.ProcessMessage(pMsg))
		return true;

	// else custom handling
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		{
			AFX_MANAGE_STATE(AfxGetStaticModuleState());

			BOOL bCtrl = Misc::IsKeyPressed(VK_CONTROL);
			BOOL bShift = Misc::IsKeyPressed(VK_SHIFT);
			BOOL bAlt = Misc::IsKeyPressed(VK_MENU);
			BOOL bEnabled = !GetReadOnly();

			// most shortcuts are handled by the shortcut manager
			if (bCtrl && !bAlt && !bShift)
			{
				switch (pMsg->wParam)
				{
				case 'z':
				case 'Z':
					return true; // to prevent the richedit performing the undo
						
				case 'y':
				case 'Y':
					return true; // to prevent the richedit performing the redo
				}
			}
			else if (bEnabled && (pMsg->wParam == VK_TAB))
			{
				CHARRANGE cr;
				m_rtf.GetSel(cr);
				
				// if nothing is selected then just insert tabs
				if (cr.cpMin == cr.cpMax)
				{
					m_rtf.ReplaceSel(_T("\t"), TRUE);
				}
				else
				{
					if (!bShift)
						DoIndent();
					else
						DoOutdent();
				}
				
				return true;
			}
		}
		break;
	}

	return false;
}

void CRTFContentControl::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString sKey(szKey);
	
	pPrefs->WriteProfileInt(sKey, _T("ShowToolbar"), IsToolbarVisible());
	pPrefs->WriteProfileInt(sKey, _T("ShowRuler"), IsRulerVisible());
	pPrefs->WriteProfileInt(sKey, _T("WordWrap"), HasWordWrap());
	pPrefs->WriteProfileInt(sKey, _T("ForegroundColour"), m_toolbar.GetFontColor(TRUE));
	pPrefs->WriteProfileInt(sKey, _T("BackgroundColour"), m_toolbar.GetFontColor(FALSE));

	m_dlgPrefs.SavePreferences(pPrefs, sKey);
}

void CRTFContentControl::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString sKey(szKey);
	
	BOOL bShowToolbar = pPrefs->GetProfileInt(sKey, _T("ShowToolbar"), m_showToolbar);
	BOOL bShowRuler = pPrefs->GetProfileInt(sKey, _T("ShowRuler"), m_showRuler);
	BOOL bWordWrap = pPrefs->GetProfileInt(sKey, _T("WordWrap"), TRUE);
	
	ShowToolbar(bShowToolbar);
	ShowRuler(bShowRuler);
	SetWordWrap(bWordWrap);
	
	m_toolbar.SetFontColor((COLORREF)pPrefs->GetProfileInt(sKey, _T("ForegroundColour"), CLR_DEFAULT), TRUE);
	m_toolbar.SetFontColor((COLORREF)pPrefs->GetProfileInt(sKey, _T("BackgroundColour"), CLR_DEFAULT), FALSE);

	m_dlgPrefs.LoadPreferences(pPrefs, sKey);
	RE_PASTE nLinkOption = (RE_PASTE)pPrefs->GetProfileInt(sKey, _T("FileLinkOption"), REP_ASIMAGE);
	BOOL bLinkOptionIsDefault = pPrefs->GetProfileInt(sKey, _T("FileLinkOptionIsDefault"), FALSE);

	m_rtf.SetFileLinkOption(nLinkOption, bLinkOptionIsDefault);
}

void CRTFContentControl::OnStyleChanging(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	if (nStyleType == GWL_EXSTYLE)
	{
		// strip of our client edge
		if (lpStyleStruct->styleNew & WS_EX_CLIENTEDGE)
			lpStyleStruct->styleNew &= ~WS_EX_CLIENTEDGE;
	}

	CRulerRichEditCtrl::OnStyleChanging(nStyleType, lpStyleStruct);
}

LRESULT CRTFContentControl::OnCustomUrl(WPARAM wp, LPARAM lp)
{
	UNREFERENCED_PARAMETER(wp);
	ASSERT (wp == RTF_CONTROL);

	CString sUrl((LPCTSTR)lp);
	sUrl.MakeLower();

	if (sUrl.Find(TDL_PROTOCOL) != -1 || sUrl.Find(TDL_EXTENSION) != -1)
		return GetParent()->SendMessage(WM_TDCM_TASKLINK, 0, lp);

	return 0;
}

LRESULT CRTFContentControl::OnFailedUrl(WPARAM wp, LPARAM lp)
{
	ASSERT (wp == RTF_CONTROL);
	return GetParent()->SendMessage(WM_TDCM_FAILEDLINK, wp, lp);
}

void CRTFContentControl::OnEditCopy() 
{
	m_rtf.Copy();	
}

void CRTFContentControl::OnEditCopyAsHtml() 
{
	m_rtf.Copy();	

	// convert RTF to HTML and copy to clipboard
	CString sRTF;
	
	if (CClipboard().GetText(sRTF, CBF_RTF))
		CopyRtfToClipboardAsHtml(sRTF, FALSE);
}

BOOL CRTFContentControl::CopyRtfToClipboardAsHtml(const CString& sRTF, BOOL bAppend)
{
	// convert RTF to HTML and copy to clipboard
	CString sHtml;

	if (m_rtfHtml.ConvertRtfToHtml((LPCSTR)(LPCTSTR)sRTF, NULL, sHtml, NULL))
	{
		CClipboard::PackageHTMLFragment(sHtml);

#ifdef _UNICODE
		// must be multibyte format for clipboard
		Misc::EncodeAsMultiByte(sHtml, CP_UTF8);
#endif
		if (bAppend)
		{
			CClipboardBackup cb(*this);
			
			cb.Backup();
			cb.AddData(sHtml, CBF_HTML);
			
			return cb.Restore();
		}
		else
		{
			CClipboard(*this).SetText(sHtml, CBF_HTML);
		}
	}

	// else
	return FALSE;
}

void CRTFContentControl::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.HasSelection());
}

void CRTFContentControl::OnEditCopyFormatting() 
{
	CharFormat cf(CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT | 
					CFM_COLOR | CFM_BACKCOLOR | CFM_SUBSCRIPT | CFM_SUPERSCRIPT);

	m_rtf.GetSelectionCharFormat(m_cfCopiedFormat);
}

void CRTFContentControl::OnUpdateEditCopyFormatting(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.GetTextLength() && m_rtf.HasSelection());
}

void CRTFContentControl::OnEditCut() 
{
	// snapshot RTF for copying to clipboard as HTML
	CString sRtf = m_rtf.GetRTF();

	m_rtf.Cut();	

	// do the copy
	CopyRtfToClipboardAsHtml(sRtf);
}

void CRTFContentControl::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.CanEdit() && m_rtf.HasSelection());
}

void CRTFContentControl::OnEditDelete() 
{
	m_rtf.ReplaceSel(_T(""), TRUE);
}

void CRTFContentControl::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.CanEdit() && m_rtf.HasSelection());
}

void CRTFContentControl::OnEditFileBrowse() 
{
	int nUrl = m_rtf.GetContextUrl();
	CString sFile;
	
	if (nUrl != -1)
	{
		sFile = m_rtf.GetUrl(nUrl, TRUE);

		if (!FileMisc::FileExists(sFile))
			sFile.Empty();
	}
				
	CFileOpenDialog dialog(IDS_BROWSE_TITLE, NULL, sFile, (EOFN_DEFAULTOPEN | OFN_ALLOWMULTISELECT));
	
	if (dialog.DoModal() == IDOK)
	{
		CStringArray aPaths;

		if (dialog.GetPathNames(aPaths) == 0)
		{
			ASSERT(0);
			return;
		}

		if (!m_rtf.IsFileLinkOptionDefault())
		{
			CCreateFileLinkDlg dialog2(sFile, m_rtf.GetFileLinkOption(), FALSE);
			
			if (dialog2.DoModal() == IDOK)
			{
				RE_PASTE nLinkOption = dialog2.GetLinkOption();
				BOOL bDefault = dialog2.GetMakeLinkOptionDefault();
				
				m_rtf.SetFileLinkOption(nLinkOption, bDefault);
				m_dlgPrefs.SetFileLinkOption(nLinkOption, !bDefault);
			}
			else
			{
				return; // user cancelled
			}
		}
		
		CRichEditHelper::PasteFiles(m_rtf, aPaths, m_rtf.GetFileLinkOption());
	}
}

void CRTFContentControl::OnUpdateEditFileBrowse(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.CanEdit());
}

void CRTFContentControl::OnEditFind() 
{
	m_rtf.DoEditFind(IDS_FIND_TITLE);
}

void CRTFContentControl::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.GetTextLength());
}

void CRTFContentControl::OnEditFindReplace() 
{
	m_rtf.DoEditReplace(IDS_REPLACE_TITLE);
}

void CRTFContentControl::OnUpdateEditFindReplace(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.CanEdit() && m_rtf.GetTextLength());
}

void CRTFContentControl::OnEditHorzRule() 
{
	DoInsertHorzLine();
}

void CRTFContentControl::OnUpdateEditHorzRule(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.CanEdit()); 
}

void CRTFContentControl::OnEditOpenUrl() 
{
	int nUrl = m_rtf.GetContextUrl();

	if (nUrl != -1)
		m_rtf.GoToUrl(nUrl);
}

void CRTFContentControl::OnEditCopyUrl() 
{
	int nUrl = m_rtf.GetContextUrl();

	if (nUrl != -1)
		m_rtf.CopyUrlToClipboard(nUrl);
}

void CRTFContentControl::OnUpdateEditOpenCopyUrl(CCmdUI* pCmdUI) 
{
	if (pCmdUI->m_pMenu)
	{
		int nUrl = m_rtf.GetContextUrl();
		pCmdUI->Enable(nUrl != -1);
		
		if (nUrl != -1)
		{
			CEnString sMenu;
			pCmdUI->m_pMenu->GetMenuString(pCmdUI->m_nID, sMenu, MF_BYCOMMAND);
			sMenu.Translate();
			
			// restrict url length to 250 pixels
			CEnString sUrl(m_rtf.GetUrl(nUrl, TRUE));
			CClientDC dc(this);
			sUrl.FormatDC(&dc, 250, ES_PATH);
			
			CString sText;
			sText.Format(_T("%s: %s"), sMenu, sUrl);
			pCmdUI->SetText(sText);
		}
	}
}

void CRTFContentControl::OnEditPaste() 
{
	Paste(FALSE);
}

void CRTFContentControl::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.CanEdit() && CanPaste());
}

void CRTFContentControl::OnEditPasteasRef() 
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
		m_rtf.ReplaceSel(sRefs, TRUE);
	}
}

void CRTFContentControl::OnUpdateEditPasteasRef(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.CanEdit() && !IsTDLClipboardEmpty());
}

void CRTFContentControl::OnEditPasteFormatting() 
{
	m_rtf.SetSelectionCharFormat(m_cfCopiedFormat);
}

void CRTFContentControl::OnUpdateEditPasteFormatting(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.CanEdit() && m_rtf.GetTextLength() && !(m_cfCopiedFormat == CharFormat()));
}

void CRTFContentControl::OnEditPasteSimple() 
{
	Paste(TRUE); // TRUE ==  simple
}

void CRTFContentControl::OnUpdateEditPasteSimple(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.CanEdit() && CanPaste());
}

void CRTFContentControl::OnEditSelectAll() 
{
	m_rtf.SetSel(0, -1);
}

void CRTFContentControl::OnUpdateEditSelectAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.GetTextLength());
}

void CRTFContentControl::OnEditShowRuler() 
{
	ShowRuler(!IsRulerVisible());
}

void CRTFContentControl::OnUpdateEditShowRuler(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(IsRulerVisible() ? 1 : 0);
}

void CRTFContentControl::OnEditShowToolbar() 
{
	ShowToolbar(!IsToolbarVisible());
}

void CRTFContentControl::OnUpdateEditShowToolbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(IsToolbarVisible() ? 1 : 0);
}

void CRTFContentControl::OnEditSpellcheck() 
{
	GetParent()->PostMessage(WM_ICC_WANTSPELLCHECK);
}

void CRTFContentControl::OnUpdateEditSpellcheck(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.CanEdit() && m_rtf.GetTextLength());
}

void CRTFContentControl::OnEditSubscript() 
{
	DoSubscript();	
}

void CRTFContentControl::OnUpdateEditSubscript(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_rtf.SelectionHasEffect(CFM_SUBSCRIPT, CFE_SUBSCRIPT) ? 1 : 0);
	pCmdUI->Enable(m_rtf.CanEdit());
}

void CRTFContentControl::OnEditSuperscript() 
{
	DoSuperscript();	
}

void CRTFContentControl::OnUpdateEditSuperscript(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_rtf.SelectionHasEffect(CFM_SUPERSCRIPT, CFE_SUPERSCRIPT) ? 1 : 0);
	pCmdUI->Enable(m_rtf.CanEdit());
}

void CRTFContentControl::OnPreferences() 
{
	if (m_dlgPrefs.DoModal(s_bConvertWithMSWord) == IDOK)
	{
		if (m_dlgPrefs.GetPromptForFileLink())
			m_rtf.SetFileLinkOption(m_rtf.GetFileLinkOption(), FALSE);
		else
			m_rtf.SetFileLinkOption(m_dlgPrefs.GetFileLinkOption(), TRUE);

		s_bConvertWithMSWord = m_dlgPrefs.GetConvertWithMSWord();
	}
}

void CRTFContentControl::OnEditInlineSpellcheck()
{
#ifndef _DEBUG
	if (m_rtf.SupportsInlineSpellChecking())
#endif
	{
		s_bInlineSpellChecking = !s_bInlineSpellChecking;

		m_rtf.EnableInlineSpellChecking(s_bInlineSpellChecking);
	}
}

void CRTFContentControl::OnUpdateEditInlineSpellcheck(CCmdUI* pCmdUI)
{
#ifndef _DEBUG
	if (m_rtf.SupportsInlineSpellChecking())
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
}
