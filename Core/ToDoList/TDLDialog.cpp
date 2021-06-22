// TDLDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLDialog.h"

#include "..\shared\FileMisc.h"
#include "..\shared\FileEdit.h"
#include "..\shared\Localizer.h"

#include "..\Interfaces\Preferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLDialog dialog

CIcon CTDLDialog::s_iconTDL;

/////////////////////////////////////////////////////////////////////////////

CTDLDialog::CTDLDialog(UINT nIDTemplate, LPCTSTR szPrefsKey, CWnd* pParent)
	: 
	CDialog(nIDTemplate, pParent), 
	m_btnHelp(nIDTemplate),
	m_sizePrev(0, 0),
	m_sizeOrg(0, 0),
	m_sPrefsKey(szPrefsKey)
{
	//{{AFX_DATA_INIT(CTDLDialog)
	//}}AFX_DATA_INIT
}

void CTDLDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CTDLDialog)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTDLDialog, CDialog)
	//{{AFX_MSG_MAP(CTDLDialog)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_WM_HELPINFO()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_REGISTERED_MESSAGE(WM_FE_GETFILEICON, OnGetFileIcon)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLDialog message handlers

BOOL CTDLDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	VERIFY(m_btnHelp.Create(IDC_HELPBUTTON, this));

	if ((GetStyle() & (WS_THICKFRAME | WS_SYSMENU)) == (WS_THICKFRAME | WS_SYSMENU))
	{
		// Because we are combining WS_THICKFRAME and WS_SYSMENU
		// to get a resizing dialog with a close button we also
		// get a generic system menu icon which we don't really 
		// want. This call gets rid of the icon.
		ModifyStyleEx(0, WS_EX_DLGMODALFRAME);
	}

	// restore size	
	if (IsResizable())
	{
		VERIFY(m_sbGrip.Initialize(this));

		if (!m_sPrefsKey.IsEmpty())
		{
			CPreferences prefs;
			int nWidth = prefs.GetProfileInt(m_sPrefsKey, _T("Width"), -1);
			int nHeight = prefs.GetProfileInt(m_sPrefsKey, _T("Height"), -1);

			if ((nWidth > 0) && (nHeight > 0))
			{
				MoveWindow(0, 0, nWidth, nHeight);
				CenterWindow();
			}
		}
	}
	
	return TRUE;
}

BOOL CTDLDialog::IsResizable() const
{
	return (GetStyle() & WS_THICKFRAME);
}

void CTDLDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (bShow && CLocalizer::IsInitialized())
		CDialogHelper::ResizeButtonStaticTextFieldsToFit(this);

	CDialog::OnShowWindow(bShow, nStatus);
}

void CTDLDialog::OnDestroy()
{
	if (!m_sPrefsKey.IsEmpty() && IsResizable())
	{
		// save current size
		CPreferences prefs;

		prefs.WriteProfileInt(m_sPrefsKey, _T("Width"), m_sizePrev.cx);
		prefs.WriteProfileInt(m_sPrefsKey, _T("Height"), m_sizePrev.cy);
	}
}

BOOL CTDLDialog::OnHelpInfo(HELPINFO* /*lpHelpInfo*/)
{
	AfxGetApp()->WinHelp(m_btnHelp.GetHelpID());
	return TRUE;
}

void CTDLDialog::OnSize(UINT nType, int cx, int cy)
{
	// initialize min size
	if (m_sizeOrg.cx == 0 || m_sizeOrg.cy == 0)
	{
		CRect rWindow;
		GetWindowRect(rWindow);

		m_sizeOrg = rWindow.Size();
	}

	CDialog::OnSize(nType, cx, cy);

	// move the controls required
	if (IsResizable() && !IsIconic())
	{
		if (m_btnHelp.GetSafeHwnd() && (m_sizePrev.cx > 0 || m_sizePrev.cy > 0))
		{
			int nDx = (cx - m_sizePrev.cx);
			int nDy = (cy - m_sizePrev.cy);

			OnRepositionControls(nDx, nDy);

			Invalidate(FALSE);
			UpdateWindow();
		}

		m_sizePrev.cx = cx;
		m_sizePrev.cy = cy;
	}

	m_btnHelp.UpdatePosition();
}

void CTDLDialog::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CDialog::OnGetMinMaxInfo(lpMMI);

	if (IsResizable())
	{
		lpMMI->ptMinTrackSize.x = m_sizeOrg.cx;
		lpMMI->ptMinTrackSize.y = m_sizeOrg.cy;
	}
}

LRESULT CTDLDialog::OnGetFileIcon(WPARAM /*wParam*/, LPARAM lParam)
{
	if (FileMisc::HasExtension((LPCTSTR)lParam, _T("tdl")))
	{
		if (!s_iconTDL.IsValid())
			s_iconTDL.Load(IDR_MAINFRAME_STD);

		return (LRESULT)(HICON)s_iconTDL;
	}

	return 0L;
}
