
// ImageResizingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageResizing.h"
#include "ImageResizingDlg.h"
#include "afxdialogex.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImageResizingDlg dialog

CImageResizingDlg::CImageResizingDlg(CWnd* pParent /*=NULL*/)
	: 
	CDialogEx(IDD_IMAGERESIZING_DIALOG, pParent),
	m_bmpDest(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageResizingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEST, m_DestImage);
	DDX_Control(pDX, IDC_SRC, m_SrcImage);
}

BEGIN_MESSAGE_MAP(CImageResizingDlg, CDialogEx)
END_MESSAGE_MAP()


// CImageResizingDlg message handlers

BOOL CImageResizingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

// 	UINT nStyle = (WS_CHILD | CBRS_ALIGN_TOP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CBRS_TOOLTIPS | WS_VISIBLE);
// 
// 	if (!m_toolbar.CreateEx(this, (TBSTYLE_FLAT | TBSTYLE_WRAPABLE), nStyle))
// 		return FALSE;
// 
// 	if (!m_toolbar.LoadToolBar(IDR_APP_TOOLBAR))
// 		return FALSE;
// 
// 	m_toolbar.SetBorders(4, 2, 0, 0);
// 
// 	// colors
// 	m_toolbar.SetBackgroundColor(RGB(0, 0, 100));
// 	m_toolbar.SetImage(IDB_APP_TOOLBAR_STD, RGB(255, 0, 255));
// 	m_toolbar.Resize(1000, CPoint(0, 2));

	// ----------------------------------------------------------

	m_bmpSrc.LoadBitmap(IDB_APP_TOOLBAR_STD);
	m_SrcImage.SetBitmap(m_bmpSrc);

	m_bmpDest.LoadBitmap(IDB_APP_TOOLBAR_STD);
	m_bmpDest.ResizeImage(7.23);
	m_bmpDest.ReplaceColor(RGB(255, 0, 255), 0);

	m_DestImage.SetBitmap(m_bmpDest);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

