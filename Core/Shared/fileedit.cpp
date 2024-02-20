// fileedit.cpp : implementation file
//

#include "stdafx.h"
#include "fileedit.h"
#include "folderdialog.h"
#include "enfiledialog.h"
#include "filemisc.h"
#include "webmisc.h"
#include "misc.h"
#include "graphicsmisc.h"
#include "enstring.h"
#include "enbitmap.h"
#include "clipboard.h"
#include "icon.h"
#include "msoutlookhelper.h"
#include "FileIcons.h"

#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

// static members
CString CFileEdit::s_sBrowseBtnTip;
CString CFileEdit::s_sGoBtnTip;
CString CFileEdit::s_sBrowseFilesTitle;
CString CFileEdit::s_sBrowseFoldersTitle;

HICON CFileEdit::s_hBrowseImage = NULL;
HICON CFileEdit::s_hGoImage = NULL;

// static members
void CFileEdit::SetDefaultButtonTips(LPCTSTR szBrowse, LPCTSTR szGo)
{
	if (!Misc::IsEmpty(szBrowse))
		s_sBrowseBtnTip = szBrowse;

	if (!Misc::IsEmpty(szGo))
		s_sGoBtnTip = szGo;
}

void CFileEdit::SetDefaultBrowseTitles(LPCTSTR szBrowseFiles, LPCTSTR szBrowseFolders)
{
	if (!Misc::IsEmpty(szBrowseFiles))
		s_sBrowseFilesTitle = szBrowseFiles;

	if (!Misc::IsEmpty(szBrowseFolders))
		s_sBrowseFoldersTitle = szBrowseFolders;
}

void CFileEdit::SetDefaultButtonImages(HICON hBrowse, HICON hGo)
{
	::DestroyIcon(s_hBrowseImage);
	s_hBrowseImage = hBrowse;

	::DestroyIcon(s_hGoImage);
	s_hGoImage = hGo;
}

/////////////////////////////////////////////////////////////////////////////
// CFileEdit

LPCTSTR FILEMASK = _T("*?\"<>|");
LPCTSTR URLMASK = _T("*\"<>|");

const UINT VIEWBTN = 0x24;
const UINT BROWSEBTN = 0x31;

IMPLEMENT_DYNAMIC(CFileEdit, CEnEdit)

CFileEdit::CFileEdit(int nStyle, LPCTSTR szFilter) : 
					CEnEdit(nStyle & FES_COMBOSTYLEBTN),
					m_nStyle(nStyle), 
					m_bTipNeeded(FALSE),
					m_sFilter(szFilter),
					m_sCurFolder(FileMisc::GetCwd())
{
	if (!(m_nStyle & FES_NOBROWSE))
	{
		CString sTip(s_sBrowseBtnTip.IsEmpty() ? FILEEDIT_BROWSE : s_sBrowseBtnTip);

		// Default images take precedence
		if (s_hBrowseImage)
			AddButton(FEBTN_BROWSE, s_hBrowseImage, sTip);
		else
			AddButton(FEBTN_BROWSE, BROWSEBTN, sTip, CALC_BTNWIDTH, _T("Wingdings"));
	}

	if (m_nStyle & FES_GOBUTTON)
	{
		BOOL bFolders = (m_nStyle & FES_FOLDERS);
		CString sTip(s_sGoBtnTip.IsEmpty() ? (bFolders ? FILEEDIT_VIEWFOLDER : FILEEDIT_VIEW) : s_sGoBtnTip);

		if (s_hGoImage)
			AddButton(FEBTN_GO, s_hGoImage, sTip);
		else
			AddButton(FEBTN_GO, VIEWBTN, sTip, CALC_BTNWIDTH, _T("Wingdings"));
	}

	// mask
	if (nStyle & FES_ALLOWURL)
		SetMask(URLMASK, ME_EXCLUDE);
	else
		SetMask(FILEMASK, ME_EXCLUDE);
}

CFileEdit::~CFileEdit()
{
	ClearImageIcon();
}


BEGIN_MESSAGE_MAP(CFileEdit, CEnEdit)
	//{{AFX_MSG_MAP(CFileEdit)
	ON_WM_PAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_KILLFOCUS()
	ON_WM_NCHITTEST()
	ON_CONTROL_REFLECT_EX(EN_CHANGE, OnChange)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_MESSAGE(WM_PASTE, OnPaste)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileEdit message handlers

void CFileEdit::EnableStyle(int nStyle, BOOL bEnable)
{
	// Buttons can only be shown/hidden via the constructor
	if (nStyle & (FES_NOBROWSE | FES_GOBUTTON))
	{
		ASSERT(0);
		return;
	}

	if (bEnable)
		m_nStyle |= nStyle;
	else
		m_nStyle &= ~nStyle;

	if (GetSafeHwnd())
		SendMessage(WM_NCPAINT);
}

BOOL CFileEdit::OnChange() 
{
	ClearImageIcon();

	EnableButton(FEBTN_GO, GetWindowTextLength());
	SendMessage(WM_NCPAINT);
	
	return FALSE; // route to parent
}

LRESULT CFileEdit::OnSetText(WPARAM /*wp*/, LPARAM /*lp*/)
{
	LRESULT lr = Default();

	OnChange();

	return lr;
}

LRESULT CFileEdit::OnPaste(WPARAM /*wp*/, LPARAM /*lp*/)
{
	// Explorer > Right-click > Copy as path will quote
	// the path which causes the mask to reject the file.
	CString sText;

	if (!m_sMask.IsEmpty() && CClipboard().GetText(sText))
	{
		Misc::Trim(sText);
		Misc::Trim(sText, '\"');

		if (!sText.IsEmpty())
			CClipboard(*this).SetText(sText);
	}

	return Default();
}

void CFileEdit::OnSetReadOnly(BOOL bReadOnly)
{
	EnableButton(FEBTN_BROWSE, !bReadOnly && IsWindowEnabled());
}

void CFileEdit::OnPaint() 
{
	m_bTipNeeded = FALSE;

	if (GetFocus() != this)
	{
		CString sText;
		GetWindowText(sText);

		if (sText.IsEmpty())
		{
			Default();
		}
		else
		{
			CPaintDC dc(this); // device context for painting
			
			CFont* pFont = GetFont();
			CFont* pFontOld = (CFont*)dc.SelectObject(pFont);
			
			// see if the text length exceeds the client width
			CRect rClient;
			GetClientRect(rClient);

			CSize sizeText = dc.GetTextExtent(sText);

			if (sizeText.cx <= rClient.Width() - 4)
			{
				DefWindowProc(WM_PAINT, (WPARAM)(HDC)dc, 0);
			}
			else
			{
				// fill bkgnd
				::FillRect(dc, rClient, GetBackgroundBrush(&dc));

				// file path
				rClient.DeflateRect(4, 1, 1, 1);
				
				dc.SetBkMode(TRANSPARENT);
				dc.SetTextColor(::GetSysColor(IsWindowEnabled() ? COLOR_WINDOWTEXT : COLOR_GRAYTEXT));
				dc.DrawText(sText, rClient, DT_PATH_ELLIPSIS);

				m_bTipNeeded = TRUE;
			}

			dc.SelectObject(pFontOld);
		}
	}
	else
	{
		Default();
	}
}

void CFileEdit::NcPaint(CDC* pDC, const CRect& rWindow)
{
	// default
	CEnEdit::NcPaint(pDC, rWindow);

	// Background color
	CRect rIcon = GetIconScreenRect();
	rIcon.OffsetRect(-rWindow.TopLeft());

	::FillRect(*pDC, rIcon, GetBackgroundBrush(pDC));
	
	// file icon
	CString sFilePath;
	GetWindowText(sFilePath);

	// remove double quotes
	sFilePath.Remove('\"');
	sFilePath.TrimLeft();
	sFilePath.TrimRight();

	// adjust pos if parent is not a combo
	if (!m_bParentIsCombo)
	{
		rIcon.top++;
		rIcon.left++;
	}
	
	DrawFileIcon(pDC, sFilePath, rIcon);
}

HBRUSH CFileEdit::GetBackgroundBrush(CDC* pDC) const
{
	UINT nMsgID = WM_CTLCOLOREDIT;

	if (!IsWindowEnabled() || (GetStyle() & ES_READONLY))
		nMsgID = WM_CTLCOLORSTATIC;

	HBRUSH hBkgnd = (HBRUSH)::SendMessage(::GetParent(GetSafeHwnd()), nMsgID, (WPARAM)pDC->GetSafeHdc(), (LPARAM)(HWND)GetSafeHwnd());

	if (!hBkgnd)
		hBkgnd = ::GetSysColorBrush(COLOR_WINDOW);

	return hBkgnd;
}

void CFileEdit::DrawFileIcon(CDC* pDC, const CString& sFilePath, const CRect& rIcon)
{
	if (sFilePath.IsEmpty())
		return;

	if (HasStyle(FES_FOLDERS))
	{
		CFileIcons::DrawFolder(pDC, rIcon.TopLeft());
		return;
	}

	int nImage = -1;
	int nImageSize = CFileIcons::GetImageSize();

	// try parent for override
	HICON hIcon = (HICON)GetParent()->SendMessage(WM_FE_GETFILEICON, GetDlgCtrlID(), (LPARAM)(LPCTSTR)sFilePath);

	if (hIcon)
	{
		ClearImageIcon();

		VERIFY(::DrawIconEx(pDC->GetSafeHdc(), rIcon.left, rIcon.top, hIcon, nImageSize, nImageSize, 0, NULL, DI_NORMAL));
		return;
	}

	// Make fullpath unless it's a URL
	CString sFullPath(sFilePath);

	if (!WebMisc::IsURL(sFilePath))
		FileMisc::MakeFullPath(sFullPath, m_sCurFolder);

	if (HasStyle(FES_DISPLAYIMAGETHUMBNAILS) && CEnBitmap::IsSupportedImageFile(sFullPath))
	{
		if (m_ilImageIcon.GetSafeHandle() == NULL)
			VERIFY(m_ilImageIcon.Create(nImageSize, nImageSize, (ILC_COLOR32 | ILC_MASK), 1, 1));

		if (m_ilImageIcon.GetImageCount() == 0)
		{
			CIcon icon(CEnBitmap::LoadImageFileAsIcon(sFullPath, GetSysColor(COLOR_WINDOW), nImageSize, nImageSize));

			if (icon.IsValid())
				m_ilImageIcon.Add(icon);
		}

		if (m_ilImageIcon.GetImageCount() > 0)
		{
			VERIFY(m_ilImageIcon.Draw(pDC, 0, rIcon.TopLeft(), ILD_TRANSPARENT));
			return;
		}
	}

	// All else
	CFileIcons::Draw(pDC, sFullPath, rIcon.TopLeft());
}

HICON CFileEdit::GetFileIcon(LPCTSTR szPath)
{
	return CFileIcons::ExtractIcon(szPath);
}

void CFileEdit::ClearImageIcon()
{
	if (m_ilImageIcon.GetSafeHandle())
	{
		ASSERT(m_ilImageIcon.GetImageCount() <= 1);

		while (m_ilImageIcon.Remove(0));
	}
}

void CFileEdit::OnBtnClick(UINT nID)
{
	switch (nID)
	{
	case FEBTN_BROWSE:
		{
			// show browse dialog
			CString sFilename;
			GetWindowText(sFilename);

			// filedialog spits if file is actually a url
			if (::PathIsURL(sFilename))
			{
				sFilename = m_sCurFolder;
			}
			else if (!sFilename.IsEmpty()) // make fullpath
			{
				FileMisc::MakeFullPath(sFilename, m_sCurFolder);
			}
			
			if (HasStyle(FES_FOLDERS))
			{
				// if folder not exists revert to current folder
				if (!FileMisc::FolderExists(sFilename))
					sFilename = m_sCurFolder;

				CFolderDialog dialog(GetBrowseTitle(TRUE), sFilename);
				
				if (dialog.DoModal() == IDOK)
				{
					SetFocus();

					CString sPath(dialog.GetFolderPath());

					if (HasStyle(FES_RELATIVEPATHS))
						sPath = FileMisc::GetRelativePath(sPath, m_sCurFolder, TRUE);

					SetWindowText(sPath);
					
					// send parent a change notification
					GetParent()->SendMessage(WM_COMMAND, MAKELPARAM(EN_CHANGE, GetDlgCtrlID()), (LPARAM)GetSafeHwnd());

					// and a browse notification
					GetParent()->SendMessage(WM_FEN_BROWSECHANGE, GetDlgCtrlID(), (LPARAM)(LPCTSTR)sPath);
				}
			}
			else // file
			{
				BOOL bOpenFileDlg = !HasStyle(FES_SAVEAS);
				DWORD dwFlags = 0;
				CString sFileExt;
				
				if (bOpenFileDlg)
				{
					bOpenFileDlg = TRUE;
					dwFlags = EOFN_DEFAULTOPEN;

					// if file not exists revert to current folder
					if (!FileMisc::FileExists(sFilename))
						sFilename.Empty();
				}
				else // open
				{
					dwFlags = EOFN_DEFAULTSAVE;

					if (HasStyle(FES_NOPROMPTOVERWRITE))
						dwFlags &= ~OFN_OVERWRITEPROMPT;

					sFileExt = m_sSaveAsFileExt;
				}
				
				CEnFileDialog dialog(bOpenFileDlg, GetBrowseTitle(FALSE), sFileExt, sFilename, dwFlags, m_sFilter);

				// custom attributes
				if (sFilename.IsEmpty())
					dialog.m_ofn.lpstrInitialDir = m_sCurFolder;

				HandleBrowseForFile(dialog);
			}
		}
		break;
		
	case FEBTN_GO:
		{
			CString sPath;
			GetWindowText(sPath);
			
			sPath.TrimLeft();
			sPath.TrimRight();
			
			if (!sPath.IsEmpty())
			{
				CWaitCursor cursor;
				
				// set the current directory in case its a relative path
				if (!m_sCurFolder.IsEmpty())
					SetCurrentDirectory(m_sCurFolder);

				// expand env vars
				VERIFY(FileMisc::ExpandPathEnvironmentVariables(sPath));

				// try our parent first
				if (!GetParent()->SendMessage(WM_FE_DISPLAYFILE, GetDlgCtrlID(), (LPARAM)(LPCTSTR)sPath))
				{
					GotoFile(*this, sPath, m_sCurFolder); 
				}
			}
		}
		break;
	}
}

void CFileEdit::HandleBrowseForFile(CEnFileDialog& dlg)
{
	if (dlg.DoModal() == IDOK)
	{
		SetFocus();
		
		CString sPath(dlg.GetPathName());
		
		if (HasStyle(FES_RELATIVEPATHS))
			sPath = FileMisc::GetRelativePath(sPath, m_sCurFolder, FALSE);
		
		SetWindowText(sPath);
		
		// send parent a change notification
		GetParent()->SendMessage(WM_COMMAND, MAKELPARAM(EN_CHANGE, GetDlgCtrlID()), (LPARAM)GetSafeHwnd());
		
		// and a browse notification
		GetParent()->SendMessage(WM_FEN_BROWSECHANGE, GetDlgCtrlID(), (LPARAM)(LPCTSTR)sPath);
	}
}

int CFileEdit::GotoFile(HWND hWnd, LPCTSTR szPath, BOOL bHandleError)
{
	return GotoFile(hWnd, szPath, FileMisc::GetCwd(), bHandleError);
}

int CFileEdit::GotoFile(HWND hWnd, LPCTSTR szPath, LPCTSTR szFolder, BOOL bHandleError)
{
	// Handle Outlook manually because under Windows 10 ShellExecute 
	// will succeed even if Outlook is not installed
	if (CMSOutlookHelper::IsOutlookUrl(szPath))
	{
		if (CMSOutlookHelper::HandleUrl(hWnd, szPath))
			return SE_ERR_SUCCESS;

		// else
		return 0;
	}

	// else
	int nRes = FileMisc::Run(hWnd, szPath, NULL, SW_SHOWNORMAL, szFolder); 
	
	if ((nRes < SE_ERR_SUCCESS) && bHandleError)
	{
		CEnString sMessage, sFullPath = FileMisc::GetFullPath(szPath, szFolder);
				
		switch (nRes)
		{
		case SE_ERR_FNF:
			sMessage.Format(FILEEDIT_FILENOTFOUND, sFullPath);
			break;
			
		case SE_ERR_NOASSOC:
			sMessage.Format(FILEEDIT_ASSOCAPPFAILURE, sFullPath);
			break;
			
		default:
			sMessage.Format(FILEEDIT_FILEOPENFAILED, sFullPath, nRes);
			break;
		}
		
		AfxMessageBox(sMessage, MB_OK);
	}

	return nRes;
}

void CFileEdit::SetCurrentFolder(LPCTSTR szFolder)
{
	m_sCurFolder = FileMisc::TerminatePath(szFolder, FALSE); 
}

void CFileEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	CEnEdit::OnNcCalcSize(bCalcValidRects, lpncsp);

	if (bCalcValidRects)
	{
		lpncsp->rgrc[0].left += CFileIcons::GetImageSize();

		if (m_bParentIsCombo)
			lpncsp->rgrc[0].left += 3;
		else
			lpncsp->rgrc[0].left += 1;
	}
}

void CFileEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEnEdit::OnKillFocus(pNewWnd);
	
	Invalidate();
}

CRect CFileEdit::GetIconScreenRect() const
{
	CRect rButton;
	GetClientRect(rButton);

	rButton.right = rButton.left;
	rButton.left -= (CFileIcons::GetImageSize() + 2);
	rButton.top -= m_nTopBorder;
	rButton.bottom += m_nBottomBorder;

	ClientToScreen(rButton);

	return rButton;
}

#if _MSC_VER >= 1400
LRESULT CFileEdit::OnNcHitTest(CPoint point)
#else
UINT CFileEdit::OnNcHitTest(CPoint point)
#endif
{
	if (GetIconScreenRect().PtInRect(point))
		return HTBORDER;
	
	return CEnEdit::OnNcHitTest(point);
}

void CFileEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEnEdit::OnSetFocus(pOldWnd);
	
	Invalidate();	
}

void CFileEdit::PreSubclassWindow() 
{
	CEnEdit::PreSubclassWindow();
	
	EnableButton(FEBTN_GO, GetWindowTextLength());
	m_bFirstShow = TRUE;
}

CString CFileEdit::GetBrowseTitle(BOOL bFolder) const
{
	if (!m_sBrowseTitle.IsEmpty())
	{
		return m_sBrowseTitle;
	}
	else if (bFolder && !s_sBrowseFoldersTitle.IsEmpty())
	{
		return s_sBrowseFoldersTitle;
	}
	else if (!bFolder && !s_sBrowseFilesTitle.IsEmpty())
	{
		return s_sBrowseFilesTitle;
	}

	// else
	return (bFolder ? FILEEDIT_SELECTFOLDER : FILEEDIT_BROWSE_TITLE);
}

int CFileEdit::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	CRect rClient;
	GetClientRect(rClient);

	if (rClient.PtInRect(point))
	{
		CString sFilePath;
		GetWindowText(sFilePath);

		if (!sFilePath.IsEmpty())
		{
			// If the parent provides a tooltip then we always show it
			// else we just use the filepath
			CString sTooltip((LPCTSTR)GetParent()->SendMessage(WM_FE_GETFILETOOLTIP, GetDlgCtrlID(), (LPARAM)(LPCTSTR)sFilePath));

			if (!sTooltip.IsEmpty() || m_bTipNeeded)
			{
				if (sTooltip.IsEmpty())
					sTooltip = sFilePath;

				return CToolTipCtrlEx::SetToolInfo(*pTI, this, sTooltip, 0xffff, rClient);
			}
		}
	}

	// else
	return CEnEdit::OnToolHitTest(point, pTI);
}
