
// ImageResizingDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "..\shared\EnBitmapEx.h"
#include "..\shared\EnToolbar.h"

// CImageResizingDlg dialog
class CImageResizingDlg : public CDialogEx
{
// Construction
public:
	CImageResizingDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGERESIZING_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;



	// Generated message map functions
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_DestImage;
	CStatic m_SrcImage;

	CBitmap m_bmpSrc;
	CEnBitmapEx m_bmpDest;

	CEnToolBar m_toolbar;
};
