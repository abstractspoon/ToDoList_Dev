/* ==========================================================================
File :			RuleRichEdit.cpp

  Class :			CRulerRichEdit
  
	Author :		Johan Rosengren, Abstrakt Mekanik AB
	Iain Clarke
	
	  Date :			2004-04-17
	  
		Purpose :		"CRulerRichEdit" is derived from "CWnd". 
		
		  Description :	The class, in addition to the normal "CWnd", 
		  handles horizontal scrollbar messages - forcing an 
		  update of the parent (to synchronize the ruler). The 
		  change notification is called for the same reason. 
		  "WM_GETDLGCODE" is handled, we want all keys in a 
		  dialog box instantiation.
		  
			Usage :			This class is only useful as a child of the 
			"CRulerRichEditCtrl".
			
========================================================================*/

#include "stdafx.h"
#include "resource.h"
#include "RulerRichEdit.h"
#include "createfilelinkdlg.h"

#include "..\shared\RichEdithelper.h"
#include "..\shared\autoflag.h"
#include "..\shared\enbitmap.h"
#include "..\shared\misc.h"
#include "..\shared\webmisc.h"
#include "..\shared\filemisc.h"
#include "..\shared\subclass.h"
#include "..\shared\HookMgr.h"
#include "..\shared\msoutlookhelper.h"
#include "..\shared\clipboard.h"
#include "..\shared\Rtf2HtmlConverter.h"

#include "..\3rdparty\clipboardbackup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

static const CLIPFORMAT CF_PREFERRED[] = 
{ 
	CF_HDROP,
	(CLIPFORMAT)CBF_RTF,
	(CLIPFORMAT)CBF_RETEXTOBJ, 
	CF_BITMAP,
	(CLIPFORMAT)CBF_HTML,
	CB_TEXTFORMAT,
	CF_METAFILEPICT,    
	CF_SYLK,            
	CF_DIF,             
	CF_TIFF,            
	CF_OEMTEXT,         
	CF_DIB,             
	CF_PALETTE,         
	CF_PENDATA,         
	CF_RIFF,            
	CF_WAVE,            
	CF_ENHMETAFILE
};
const long NUM_PREF = sizeof(CF_PREFERRED) / sizeof(CLIPFORMAT);

/////////////////////////////////////////////////////////////////////////////
// CFindReplaceDialogEx

// for some reason (that I cannot divine) dialog keyboard handling 
// is not working so we install a keyboard hook and handle

class CFindReplaceDialogEx : public CFindReplaceDialog, public CHookMgr<CFindReplaceDialogEx>  
{
   friend class CHookMgr<CFindReplaceDialogEx>;

public:
	virtual ~CFindReplaceDialogEx() {}
	static CFindReplaceDialogEx& Instance() { return CHookMgr<CFindReplaceDialogEx>::GetInstance(); }

protected:
	CFindReplaceDialogEx() {}

	virtual void PostNcDestroy() { ReleaseHooks(); }
	virtual BOOL OnInitDialog();
	virtual BOOL OnKeyboard(UINT uVirtKey, UINT /*uFlags*/);
};

BOOL CFindReplaceDialogEx::OnInitDialog()
{
	CFindReplaceDialog::OnInitDialog();

	InitHooks(HM_KEYBOARD);
	return TRUE;
}

BOOL CFindReplaceDialogEx::OnKeyboard(UINT uVirtKey, UINT uFlags)
{
	// only handle messages belonging to us
	HWND hFocus = ::GetFocus();

	if (::IsChild(*this, hFocus) && Misc::IsKeyPressed(uVirtKey))
	{
		MSG msg = { hFocus, WM_KEYDOWN, uVirtKey, uFlags, 0, {0, 0} };

		if (::IsDialogMessage(*this, &msg))
			return TRUE;
	}
	
	// all else
	return FALSE;
}

CFindReplaceDialog* CRulerRichEdit::NewFindReplaceDlg()
{
	// because we reuse the same dialog we must clear the terminating flag
	CFindReplaceDialog& dialog = CFindReplaceDialogEx::Instance();
	dialog.m_fr.Flags &= ~FR_DIALOGTERM;

	return &dialog;
}

/////////////////////////////////////////////////////////////////////////////
// CRulerRichEdit

BOOL CRulerRichEdit::s_bConvertWithMSWord = FALSE;
BOOL CRulerRichEdit::s_bPasteSourceUrls = TRUE;

/////////////////////////////////////////////////////////////////////////////

CRulerRichEdit::CRulerRichEdit(CRtfHtmlConverter& rtfHtml) 
	: 
	CUrlRichEditCtrl(CTRLCLICKTOFOLLOW, IDS_CTRLCLICKTOFOLLOWLINK),
	m_bIMEComposing(FALSE), 
	m_nFileLinkOption(REP_ASIMAGE), 
	m_bReduceImageColors(TRUE),
	m_rtfHtml(rtfHtml)
	
{
	EnableSelectOnFocus(FALSE);
}

CRulerRichEdit::~CRulerRichEdit()
{
}

BEGIN_MESSAGE_MAP(CRulerRichEdit, CUrlRichEditCtrl)
//{{AFX_MSG_MAP(CRulerRichEdit)
	ON_WM_HSCROLL()
	ON_WM_GETDLGCODE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DROPFILES, OnDropFiles)
	ON_MESSAGE(WM_IME_STARTCOMPOSITION, OnIMEStartComposition)
	ON_MESSAGE(WM_IME_ENDCOMPOSITION, OnIMEEndComposition)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRulerRichEdit message handlers

void CRulerRichEdit::SetFileLinkOption(RE_PASTE nLinkOption, BOOL bDefault, BOOL bReduceImageColors) 
{ 
	m_nFileLinkOption = nLinkOption; 
	m_bLinkOptionIsDefault = bDefault;
	m_bReduceImageColors = bReduceImageColors;
}

int CRulerRichEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CUrlRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CRulerRichEdit::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CUrlRichEditCtrl::OnHScroll( nSBCode, nPos, pScrollBar );
	
	if ( nSBCode == SB_THUMBTRACK )
	{
		SCROLLINFO	si;
		ZeroMemory( &si, sizeof( SCROLLINFO ) );
		si.cbSize = sizeof( SCROLLINFO );
		GetScrollInfo( SB_HORZ, &si );
		
		si.nPos = nPos;
		SetScrollInfo( SB_HORZ, &si );
		
		// notify parent
		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), EN_HSCROLL), (LPARAM)GetSafeHwnd());
	}
}

LRESULT CRulerRichEdit::OnDropFiles(WPARAM wp, LPARAM /*lp*/) 
{
	CWaitCursor cursor;
	CStringArray aFiles;
	
	int nNumFiles = FileMisc::GetDropFilePaths((HDROP)wp, aFiles);

	::DragFinish((HDROP)wp);
	::CloseClipboard();

	switch (nNumFiles)
	{
	case -1: // error
		AfxMessageBox(IDS_PASTE_ERROR, MB_OK | MB_ICONERROR);
		break;

	case 0:
		break;

	default:
		// Only ever link to folders so no need to prompt
		if (FileMisc::FolderExists(aFiles[0]))
			return CRichEditHelper::PasteFiles(*this, aFiles, REP_ASFILEURL, FALSE);

		// else
		if (!m_bLinkOptionIsDefault)
		{
			CCreateFileLinkDlg dialog(aFiles[0], m_nFileLinkOption, FALSE, m_bReduceImageColors);

			if (dialog.DoModal() != IDOK)
				return 0L;
			
			m_nFileLinkOption = dialog.GetLinkOption();
			m_bLinkOptionIsDefault = dialog.GetMakeLinkOptionDefault();
			m_bReduceImageColors = dialog.GetReduceImageColors();
		}

		return CRichEditHelper::PasteFiles(*this, aFiles, m_nFileLinkOption, m_bReduceImageColors);
	}

	// else
	return 0L;
}

HRESULT CRulerRichEdit::GetDragDropEffect(BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect)
{
	if (!fDrag) // allowable dest effects
	{
		BOOL bEnable = !(GetStyle() & ES_READONLY) && IsWindowEnabled();
		
		if (bEnable)
		{
			BOOL bFileDrop = (!CMSOutlookHelper::IsOutlookObject(m_lpDragObject) &&
								Misc::HasFlag(*pdwEffect, DROPEFFECT_LINK));
			
			if (bFileDrop)
			{
				DWORD dwEffect = DROPEFFECT_NONE;
				BOOL bShift = Misc::HasFlag(grfKeyState, MK_SHIFT);

				// if SHIFT is down then show this as a copy because we are embedding
				dwEffect = (DROPEFFECT_MOVE | (bShift ? DROPEFFECT_COPY : 0));
				
				TrackDragCursor();
				
				// make sure allowed type
				if ((dwEffect & *pdwEffect) == dwEffect) 
					*pdwEffect = dwEffect;

				return S_OK;
			}
		}
	}

	// all else
	return CUrlRichEditCtrl::GetDragDropEffect(fDrag, grfKeyState, pdwEffect);
}

CLIPFORMAT CRulerRichEdit::GetAcceptableClipFormat(LPDATAOBJECT lpDataOb, CLIPFORMAT format) 
{ 
	// check for outlook first
	if (CMSOutlookHelper::IsOutlookObject(lpDataOb))
		return CMSOutlookHelper::CF_OUTLOOK;

	// TRUE allows fallback formats if our preferred didn't match
	return CRichEditBaseCtrl::GetAcceptableClipFormat(lpDataOb, format, CF_PREFERRED, NUM_PREF, TRUE);
}

UINT CRulerRichEdit::OnGetDlgCode() 
{
	return (DLGC_WANTALLKEYS & ~DLGC_HASSETSEL);
}

void CRulerRichEdit::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	
	CUrlRichEditCtrl::OnLButtonDblClk(nFlags, point);
}

BOOL CRulerRichEdit::AppendSourceUrls(LPCTSTR szUrls)
{
	if (s_bPasteSourceUrls)
		return CUrlRichEditCtrl::AppendSourceUrls(szUrls);

	return FALSE;
}

LRESULT CRulerRichEdit::OnIMEStartComposition(WPARAM /*wp*/, LPARAM /*lp*/)
{
	// There's a bug where I'm not receiving the 'end composition' 
	// notification resulting in text changes not triggering a change
	// notification to the app.
	//m_bIMEComposing = TRUE;

	return Default();
}

LRESULT CRulerRichEdit::OnIMEEndComposition(WPARAM /*wp*/, LPARAM /*lp*/)
{
	m_bIMEComposing = FALSE;

	return Default();
}

HRESULT CRulerRichEdit::GetContextMenu(WORD seltyp, LPOLEOBJECT lpoleobj, 
									   CHARRANGE FAR* lpchrg, HMENU FAR* lphmenu)
{
	if (seltyp & (SEL_OBJECT | SEL_MULTIOBJECT))
	{
		// Load the default context menu and strip out inappropriate options
		HMENU hMenu = ::LoadMenu(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_POPUP));
		*lphmenu = ::GetSubMenu(hMenu, 0); 

		// COM now owns the submenu so we must 
		// remove it before cleaning-up the menu
		VERIFY(::RemoveMenu(hMenu, 0, MF_BYPOSITION));
		::DestroyMenu(hMenu);

		// remove all but simple edit commands
		int nItem = ::GetMenuItemCount(*lphmenu);

		while (nItem--)
		{
			switch (::GetMenuItemID(*lphmenu, nItem))
			{
			case ID_EDIT_COPY:
			case ID_EDIT_CUT:
			case ID_EDIT_PASTE:
			case ID_EDIT_DELETE:
			case ID_EDIT_SELECT_ALL:
				break;

			default:
				::RemoveMenu(*lphmenu, nItem, MF_BYPOSITION);
			}
		}

		// insert a separator before 'Select All'
		::InsertMenu(*lphmenu, ID_EDIT_SELECT_ALL, (MF_BYCOMMAND | MF_SEPARATOR), 0, NULL);

		return S_OK;
	}

	// all else
	return CUrlRichEditCtrl::GetContextMenu(seltyp, lpoleobj, lpchrg, lphmenu);
}

BOOL CRulerRichEdit::IsRTF(const char* szRTF) const 
{ 
	return m_rtfHtml.IsRTF(szRTF); 
}

BOOL CRulerRichEdit::GetClipboardHtmlForPasting(CString& sHtml, CString& sSourceUrl)
{
	CClipboard cb;
	
	if (!cb.HasFormat(CBF_RTF) && 
		!cb.HasFormat(CBF_RETEXTOBJ) && 
		!cb.HasFormat(CBF_EMBEDDEDOBJ) &&
		!cb.HasFormat(CF_DIB) &&
		!cb.HasFormat(CF_BITMAP) &&
		!cb.HasFormat(CBF_ONENOTELINK) &&
		cb.GetText(sHtml, CBF_HTML))
	{
		return ProcessHtmlForPasting(sHtml, sSourceUrl);
	}

	// all else
	return FALSE;
}

BOOL CRulerRichEdit::ProcessHtmlForPasting(CString& sHtml, CString& sSourceUrl)
{
	if (!sHtml.IsEmpty())
	{
#ifdef _UNICODE
		// convert to unicode for unpackaging because
		// CF_HTML is saved to the clipboard as UTF8
		Misc::EncodeAsUnicode(sHtml, CP_UTF8);
#endif
		Misc::Trim(sHtml);

		CClipboard::UnpackageHTMLFragment(sHtml, sSourceUrl);
		
		if (!sHtml.IsEmpty() && !WebMisc::IsAboutBlank(sSourceUrl))
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

BOOL CRulerRichEdit::PasteSimple()
{
	CStringArray aFiles;
	int nNumFiles = CClipboard().GetDropFilePaths(aFiles);

	if (nNumFiles == -1)
	{
		AfxMessageBox(IDS_PASTE_ERROR, MB_OK | MB_ICONERROR);
		return FALSE;
	}
	else if (nNumFiles == 0)
	{
		return PasteSimpleText(s_bPasteSourceUrls);
	}

	// else one or more filenames 
	return CRichEditHelper::PasteFiles(*this, aFiles, REP_ASFILEURL, FALSE);
}

BOOL CRulerRichEdit::Paste()
{
	CStringArray aFiles;
	int nNumFiles = CClipboard().GetDropFilePaths(aFiles);

	if (nNumFiles == -1)
	{
		AfxMessageBox(IDS_PASTE_ERROR, MB_OK | MB_ICONERROR);
		return FALSE;
	}
	else if (nNumFiles == 0)
	{
		CWaitCursor cursor;

		// If there isn't Rich Text on the clipboard...
		if (!CClipboard::HasFormat(CBF_RTF) && 
			!CClipboard::HasFormat(CBF_RETEXTOBJ) && 
			!CClipboard::HasFormat(CBF_EMBEDDEDOBJ))
		{
			// If the clipboard contains a bitmap, copy it and reduce its 
			// colour depth to 8-bit, and use 'paste special' to prevent 
			// richedit's default resizing
			CString sSourceUrl;

			if (CClipboard::HasFormat(CF_BITMAP))
			{
				CClipboardBackup cbb(*this);
				BOOL bClipboardSaved = FALSE;

				CClipboard cb;
				cb.GetHTMLSourceLink(sSourceUrl);

				if (GetReduceImageColors())
				{
					CEnBitmap bmp;

					if (bmp.CopyImage(cb.GetBitmap()) && cb.Close())
					{
						bClipboardSaved = cbb.Backup();
						ASSERT(bClipboardSaved);

						if (!bmp.CopyToClipboard(GetSafeHwnd(), 8))
						{
							ASSERT(0);
							bClipboardSaved = FALSE;
						}
					}
				}

				PasteSpecial(CF_BITMAP);

				// restore the clipboard if necessary
				if (bClipboardSaved)
					VERIFY(cbb.Restore());

				AppendSourceUrls(sSourceUrl);
				return TRUE;
			}

			// If there is a OneNote link on the clipboard
			// paste the raw text instead
			if (CClipboard::HasFormat(CBF_ONENOTELINK))
			{
				ASSERT(CClipboard::HasText());

				CUrlRichEditCtrl::PasteSimpleText(FALSE);
				return TRUE;
			}

			// if there is HTML convert it to RTF and insert it
			CString sHtml;

			if (GetClipboardHtmlForPasting(sHtml, sSourceUrl))
			{
				// Always set this to make sure it is current
				m_rtfHtml.SetAllowUseOfMSWord(s_bConvertWithMSWord);

				CString sRTF;

				if (m_rtfHtml.ConvertHtmlToRtf((LPCSTR)(LPCTSTR)sHtml, NULL, sRTF, NULL))
				{
					VERIFY(SetTextEx(sRTF));

					AppendSourceUrls(sSourceUrl);
					return TRUE;
				}
				else
				{
					ASSERT(CClipboard::HasText());

					CUrlRichEditCtrl::PasteSimpleText(s_bPasteSourceUrls);
					return TRUE;
				}
			}
		}

		// Fallback and all else -> Default paste
		CUrlRichEditCtrl::Paste(s_bPasteSourceUrls);
	
		return TRUE;
	}
	
	// else one or more filenames 
	RE_PASTE nLinkOption = GetFileLinkOption();

	if (FileMisc::FolderExists(aFiles[0]))
	{
		// Only ever paste folders as links
		nLinkOption = REP_ASFILEURL;
	}
	else if (!IsFileLinkOptionDefault())
	{
		CCreateFileLinkDlg dialog(aFiles[0], nLinkOption, FALSE, m_bReduceImageColors);

		if (dialog.DoModal() != IDOK)
			return FALSE; // cancelled

		// else
		nLinkOption = dialog.GetLinkOption();
		BOOL bDefault = dialog.GetMakeLinkOptionDefault();
		BOOL bReduceImageColors = dialog.GetReduceImageColors();

		SetFileLinkOption(nLinkOption, bDefault, bReduceImageColors);
	}

	return CRichEditHelper::PasteFiles(GetSafeHwnd(), aFiles, nLinkOption, m_bReduceImageColors);
}

BOOL CRulerRichEdit::PasteFiles(const CStringArray& aFiles)
{
	if (!IsFileLinkOptionDefault())
	{
		CCreateFileLinkDlg dialog(aFiles[0], m_nFileLinkOption, FALSE, m_bReduceImageColors);
		
		if (dialog.DoModal() != IDOK)
			return FALSE;

		// else
		RE_PASTE nLinkOption = dialog.GetLinkOption();
		BOOL bDefault = dialog.GetMakeLinkOptionDefault();
		BOOL bReduceImageColors = dialog.GetReduceImageColors();
			
		SetFileLinkOption(nLinkOption, bDefault, bReduceImageColors);
	}

	return CRichEditHelper::PasteFiles(GetSafeHwnd(), aFiles, m_nFileLinkOption, m_bReduceImageColors);
}

BOOL CRulerRichEdit::CanPaste() const
{
	if (!CanEdit())
		return FALSE;

	for (int i=0; i < NUM_PREF; i++)
	{
		if (CClipboard::HasFormat(CF_PREFERRED[i]))
			return TRUE;
	}

	// else
	return CRichEditCtrl::CanPaste(0);
}

BOOL CRulerRichEdit::CopyRtfToClipboardAsHtml(const CString& sRTF, BOOL bAppend)
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

BOOL CRulerRichEdit::CopyToClipboardAsHtml()
{
	CUrlRichEditCtrl::Copy();	

	// convert RTF to HTML and copy to clipboard
	CString sRTF;

	if (CClipboard().GetText(sRTF, CBF_RTF))
		return CopyRtfToClipboardAsHtml(sRTF, FALSE);

	// else
	return FALSE;
}

BOOL CRulerRichEdit::Cut() 
{
	// snapshot RTF for copying to clipboard as HTML
	CString sRtf = GetRTF();
	CUrlRichEditCtrl::Cut();	
	
	// do the copy
	return CopyRtfToClipboardAsHtml(sRtf);
}

HRESULT CRulerRichEdit::QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT* lpcfFormat, 
										  DWORD reco, BOOL fReally, HGLOBAL hMetaPict)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HRESULT hr = CUrlRichEditCtrl::QueryAcceptData(lpdataobj, lpcfFormat, reco, fReally, hMetaPict);

	// is this a HTML drop actually happening?
	if ((hr == S_OK) && fReally && (*lpcfFormat == CBF_HTML))
	{
		CWaitCursor cursor;
		CString sHtml = CClipboard::GetText(lpdataobj, CBF_HTML), sSourceUrl;
		
		if (ProcessHtmlForPasting(sHtml, sSourceUrl))
		{
			// Always set this to make sure it is current
			m_rtfHtml.SetAllowUseOfMSWord(s_bConvertWithMSWord);
			
			CString sRTF;
			
			if (m_rtfHtml.ConvertHtmlToRtf((LPCSTR)(LPCTSTR)sHtml, NULL, sRTF, NULL))
			{
				VERIFY(SetTextEx(sRTF));
				AppendSourceUrls(sSourceUrl);
			}
			
			return S_OK;
		}

		// else try for plain URL
		sSourceUrl = CClipboard::GetText(lpdataobj, CBF_URLW);

		if (!sSourceUrl.IsEmpty())
		{
			if (CEnBitmap::IsSupportedImageFile(sSourceUrl))
			{
				CString sImagePath;

				// If it's not a file URI then download the image
				if (!WebMisc::DecodeFileURI(sSourceUrl, sImagePath))
				{
					CString sFileName = FileMisc::GetFileNameFromPath(sSourceUrl, FALSE);
					CString sExt = FileMisc::GetExtension(sSourceUrl);

					sImagePath = FileMisc::GetTempFilePath(sFileName, sExt);
					FileMisc::DeleteFile(sImagePath, TRUE);

					if (!WebMisc::DownloadFile(sSourceUrl, sImagePath))
						sImagePath.Empty();
				}

				if (!sImagePath.IsEmpty())
				{
					if (!IsFileLinkOptionDefault())
					{
						CCreateFileLinkDlg dialog(sImagePath, m_nFileLinkOption, FALSE, m_bReduceImageColors);
						
						if (dialog.DoModal() != IDOK)
							return E_FAIL;
						
						// else
						RE_PASTE nLinkOption = dialog.GetLinkOption();
						BOOL bDefault = dialog.GetMakeLinkOptionDefault();
						BOOL bReduceImageColors = dialog.GetReduceImageColors();
						
						SetFileLinkOption(nLinkOption, bDefault, bReduceImageColors);
					}

					if (m_nFileLinkOption != REP_ASFILEURL)
					{
						if (CRichEditHelper::PasteFile(*this, sImagePath, m_nFileLinkOption, m_bReduceImageColors))
							return S_OK;
					}
				}
			}

			// else
			ReplaceSel(sSourceUrl, TRUE);
			return S_OK;
		}

		// else
		return E_FAIL;
	}
	
	// else
	return hr;
}
