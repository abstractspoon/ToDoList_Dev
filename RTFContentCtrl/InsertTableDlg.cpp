// InsertTableDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rtfcontentctrl.h"
#include "InsertTableDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsertTableDlg dialog


CInsertTableDlg::CInsertTableDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsertTableDlg::IDD, pParent), m_nNumRows(4), m_nNumCols(4), m_nColWidth(3)

{
	//{{AFX_DATA_INIT(CInsertTableDlg)
	//}}AFX_DATA_INIT
// 	m_nPadLeft = 0;
// 	m_nPadTop = 0;
// 	m_nPadRight = 0;
// 	m_nPadBottom = 0;
}


void CInsertTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsertTableDlg)
	DDX_Control(pDX, IDC_COLWIDTH, m_cbColWidth);
	DDX_Control(pDX, IDC_NUMCOLS, m_cbCols);
	DDX_Control(pDX, IDC_NUMROWS, m_cbRows);
	//}}AFX_DATA_MAP
// 	DDX_Text(pDX, IDC_PADLEFT, m_nPadLeft);
// 	DDX_Text(pDX, IDC_PADTOP, m_nPadTop);
// 	DDX_Text(pDX, IDC_PADRIGHT, m_nPadRight);
// 	DDX_Text(pDX, IDC_PADBOTTOM, m_nPadBottom);

	if (pDX->m_bSaveAndValidate)
	{
		CString sValue;

		GetDlgItem(IDC_NUMCOLS)->GetWindowText(sValue);
		m_nNumCols = max(_ttoi(sValue), 1);

		GetDlgItem(IDC_NUMROWS)->GetWindowText(sValue);
		m_nNumRows = max(_ttoi(sValue), 1);

		GetDlgItem(IDC_COLWIDTH)->GetWindowText(sValue);
		m_nColWidth = max(_ttoi(sValue), 1);
	}
	else
	{
		CString sValue;

		sValue.Format(_T("%d"), m_nNumCols);
		m_cbCols.SelectString(-1, sValue);
		
		sValue.Format(_T("%d"), m_nNumRows);
		m_cbRows.SelectString(-1, sValue);
		
		sValue.Format(_T("%d"), m_nColWidth);
		m_cbColWidth.SelectString(-1, sValue);
	}
}


BEGIN_MESSAGE_MAP(CInsertTableDlg, CDialog)
	//{{AFX_MSG_MAP(CInsertTableDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsertTableDlg message handlers

BOOL CInsertTableDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
void CInsertTableDlg::GetPadding(CRect& rPadding) const
{
	rPadding.left = m_nPadLeft;
	rPadding.top = m_nPadTop;
	rPadding.right = m_nPadRight;
	rPadding.bottom = m_nPadBottom;
}
*/