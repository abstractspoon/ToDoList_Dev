// TDLPasteTaskAttributesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLPasteTaskAttributesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLPasteTaskAttributesDlg dialog


CTDLPasteTaskAttributesDlg::CTDLPasteTaskAttributesDlg(const CTDCCustomAttribDefinitionArray& aCustAttribs, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(CTDLPasteTaskAttributesDlg::IDD, _T(""), pParent),
	m_lbSpecificAttributes(aCustAttribs, FALSE)
{
	//{{AFX_DATA_INIT(CTDLPasteTaskAttributesDlg)
	m_bIncludeComments = FALSE;
	m_bIncludeCustomAttribs = FALSE;
	m_bIncludeDates = FALSE;
	m_bOnlyOverwriteEmpty = TRUE;
	m_bOnlyOverwriteWithNonEmpty = TRUE;
	m_bSelectSpecificAttribs = 0;
	m_bIncludeTaskTitle = FALSE;
	//}}AFX_DATA_INIT
}


void CTDLPasteTaskAttributesDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLPasteTaskAttributesDlg)
	DDX_Control(pDX, IDC_SPECIFICATTRIBS, m_lbSpecificAttributes);
	DDX_Check(pDX, IDC_INCLUDECOMMENTS, m_bIncludeComments);
	DDX_Check(pDX, IDC_INCLUDECUSTOMATTRIB, m_bIncludeCustomAttribs);
	DDX_Check(pDX, IDC_INCLUDEDATES, m_bIncludeDates);
	DDX_Check(pDX, IDC_INCLUDETITLE, m_bIncludeTaskTitle);
	DDX_Check(pDX, IDC_ONLYOVERWRITEMPTY, m_bOnlyOverwriteEmpty);
	DDX_Check(pDX, IDC_ONLYOVERWRITEWITHNONEMPTY, m_bOnlyOverwriteWithNonEmpty);
	DDX_Radio(pDX, IDC_SELECTALL, m_bSelectSpecificAttribs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLPasteTaskAttributesDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLPasteTaskAttributesDlg)
	ON_BN_CLICKED(IDC_SELECTALL, OnSelectAllAttributes)
	ON_BN_CLICKED(IDC_SELECTSPECIFIC, OnSelectSpecificAttributes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLPasteTaskAttributesDlg message handlers

BOOL CTDLPasteTaskAttributesDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();
	
	EnableDisableControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CTDLPasteTaskAttributesDlg::GetSelectedAttributes(CTDCAttributeMap& mapAttrib) const
{
	if (m_bSelectSpecificAttribs)
		return m_lbSpecificAttributes.GetSelectedAttributes(mapAttrib);

	// else
	m_lbSpecificAttributes.GetAvailableAttributes(mapAttrib, m_bIncludeCustomAttribs);

	if (!m_bIncludeTaskTitle)
		mapAttrib.Remove(TDCA_TASKNAME);

	if (!m_bIncludeComments)
		mapAttrib.Remove(TDCA_COMMENTS);

	if (!m_bIncludeDates)
	{
		mapAttrib.Remove(TDCA_STARTDATE);
		mapAttrib.Remove(TDCA_DUEDATE);
		mapAttrib.Remove(TDCA_DONEDATE);
		mapAttrib.Remove(TDCA_CREATIONDATE);
		mapAttrib.Remove(TDCA_LASTMODDATE);
	}

	return mapAttrib.GetCount();
}

void CTDLPasteTaskAttributesDlg::OnSelectAllAttributes()
{
	UpdateData();
	EnableDisableControls();
}

void CTDLPasteTaskAttributesDlg::OnSelectSpecificAttributes()
{
	UpdateData();
	EnableDisableControls();
}

void CTDLPasteTaskAttributesDlg::EnableDisableControls()
{
	GetDlgItem(IDC_SPECIFICATTRIBS)->EnableWindow(m_bSelectSpecificAttribs);
	GetDlgItem(IDC_INCLUDECOMMENTS)->EnableWindow(!m_bSelectSpecificAttribs);
	GetDlgItem(IDC_INCLUDECUSTOMATTRIB)->EnableWindow(!m_bSelectSpecificAttribs);
	GetDlgItem(IDC_INCLUDEDATES)->EnableWindow(!m_bSelectSpecificAttribs);
	GetDlgItem(IDC_INCLUDETITLE)->EnableWindow(!m_bSelectSpecificAttribs);
}
