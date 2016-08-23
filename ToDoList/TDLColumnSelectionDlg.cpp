// TDLColumnSelectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLColumnSelectionDlg.h"

#include "../shared/preferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLColumnSelectionDlg dialog


CTDLColumnSelectionDlg::CTDLColumnSelectionDlg(const TDCCOLEDITFILTERVISIBILITY& vis, 
											   const TDCCOLEDITFILTERVISIBILITY& visDefault, 
											   CWnd* pParent)
	: 
	CTDLDialog(CTDLColumnSelectionDlg::IDD, pParent), 
	m_visColAttrib(vis),
	m_visDefault(visDefault),
	m_nAttribShow(vis.GetShowEditsAndFilters())
{
	//{{AFX_DATA_INIT(CTDLColumnSelectionDlg)
	//}}AFX_DATA_INIT

	// restore state
	CPreferences prefs;

	m_bActiveTasklist = prefs.GetProfileInt(_T("ColumnAttributes"), _T("ActiveTasklist"), TRUE);
	m_bUpdatePrefs = prefs.GetProfileInt(_T("ColumnAttributes"), _T("UpdatePrefs"), FALSE);
}


void CTDLColumnSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLColumnSelectionDlg)
	DDX_Control(pDX, IDC_VISIBILITY, m_lcVisibility);
	DDX_Radio(pDX, IDC_ALLTASKLISTS, m_bActiveTasklist);
	DDX_Check(pDX, IDC_UPDATEPREFS, m_bUpdatePrefs);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_SHOWALLATTRIB, (int&)m_nAttribShow);
}


BEGIN_MESSAGE_MAP(CTDLColumnSelectionDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLColumnSelectionDlg)
	ON_BN_CLICKED(IDC_SELECTALLCOLS, OnSelectallcols)
	ON_BN_CLICKED(IDC_CLEARALLCOLS, OnClearallcols)
	ON_BN_CLICKED(IDC_DEFAULTCOLS, OnDefaultcols)
	ON_BN_CLICKED(IDC_SHOWALLATTRIB, OnChangeAttribShow)
	ON_BN_CLICKED(IDC_SHOWANYATTRIB, OnChangeAttribShow)
	ON_BN_CLICKED(IDC_SHOWATTRIBASCOLUMN, OnChangeAttribShow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLColumnSelectionDlg message handlers

void CTDLColumnSelectionDlg::GetColumnEditFilterVisibility(TDCCOLEDITFILTERVISIBILITY& vis) const
{
	vis = m_visColAttrib;
	vis.SetShowEditsAndFilters(m_nAttribShow);
}

void CTDLColumnSelectionDlg::OnOK()
{
	CTDLDialog::OnOK();

	m_lcVisibility.GetVisibility(m_visColAttrib);

	// save state
	CPreferences prefs;

	prefs.WriteProfileInt(_T("ColumnAttributes"), _T("ActiveTasklist"), m_bActiveTasklist);
	prefs.WriteProfileInt(_T("ColumnAttributes"), _T("UpdatePrefs"), m_bUpdatePrefs);
}

BOOL CTDLColumnSelectionDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();
	
	m_lcVisibility.SetVisibility(m_visColAttrib);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLColumnSelectionDlg::OnSelectallcols() 
{
	m_lcVisibility.SetAllVisible(TRUE);
}

void CTDLColumnSelectionDlg::OnClearallcols() 
{
	m_lcVisibility.SetAllVisible(FALSE);
}

void CTDLColumnSelectionDlg::OnDefaultcols() 
{
	m_nAttribShow = m_visDefault.GetShowEditsAndFilters();
	UpdateData(FALSE);

	m_lcVisibility.SetVisibility(m_visDefault);
}

void CTDLColumnSelectionDlg::OnChangeAttribShow() 
{
	UpdateData();
	
	m_lcVisibility.SetShowEditsAndFilters(m_nAttribShow);
}
