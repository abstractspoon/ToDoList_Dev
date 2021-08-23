
// ImageResizingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageResizing.h"
#include "ImageResizingDlg.h"
#include "afxdialogex.h"

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

	m_bmpSrc.LoadBitmap(IDB_BITMAP1);
	m_SrcImage.SetBitmap(m_bmpSrc);

	m_bmpDest.LoadBitmap(IDB_BITMAP1);
	m_bmpDest.Resize(57, 57);

	m_DestImage.SetBitmap(m_bmpDest);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

