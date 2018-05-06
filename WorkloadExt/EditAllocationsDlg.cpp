// EditAllocationsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "EditAllocationsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditAllocationsDlg dialog


CEditAllocationsDlg::CEditAllocationsDlg(const WORKLOADITEM& wi, const CStringArray& aAllocTo, CWnd* pParent /*=NULL*/)
	: 
	CDialog(CEditAllocationsDlg::IDD, pParent), m_lcAllocations(wi, aAllocTo)
{
	//{{AFX_DATA_INIT(CEditAllocationsDlg)
	//}}AFX_DATA_INIT

	m_sTaskTitle = wi.sTitle;
}


void CEditAllocationsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditAllocationsDlg)
	DDX_Control(pDX, IDC_ALLOCATIONS, m_lcAllocations);
	DDX_Text(pDX, IDC_TASKTITLE, m_sTaskTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditAllocationsDlg, CDialog)
	//{{AFX_MSG_MAP(CEditAllocationsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditAllocationsDlg message handlers

BOOL CEditAllocationsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

const WORKLOADITEM& CEditAllocationsDlg::GetAllocations() const
{
	return m_lcAllocations.GetAllocations();
}
