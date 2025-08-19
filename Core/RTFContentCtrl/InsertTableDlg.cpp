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
	: 
	CDialog(CInsertTableDlg::IDD, pParent), 
	m_nNumRows(4), 
	m_nNumCols(4), 
	m_nColWidth(3),
	m_icon(IDR_RTFCOMMENTS)
{
	//{{AFX_DATA_INIT(CInsertTableDlg)
	//}}AFX_DATA_INIT
}


void CInsertTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsertTableDlg)
	DDX_Control(pDX, IDC_COLWIDTH, m_cbColWidth);
	DDX_Control(pDX, IDC_NUMCOLS, m_cbCols);
	DDX_Control(pDX, IDC_NUMROWS, m_cbRows);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		CString sValue;

		GetDlgItemText(IDC_NUMCOLS, sValue);
		m_nNumCols = max(_ttoi(sValue), 1);

		GetDlgItemText(IDC_NUMROWS, sValue);
		m_nNumRows = max(_ttoi(sValue), 1);

		GetDlgItemText(IDC_COLWIDTH, sValue);
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
	
	SetIcon(m_icon, FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
