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
	CTDLDialog(CTDLPasteTaskAttributesDlg::IDD, _T("PasteTaskAttributes"), pParent),
	m_lbSpecificAttributes(aCustAttribs, FALSE)
{
	//{{AFX_DATA_INIT(CTDLPasteTaskAttributesDlg)
	//}}AFX_DATA_INIT
	CPreferences prefs;

	m_bIncludeComments = prefs.GetProfileInt(m_sPrefsKey, _T("IncludeComments"), FALSE);
	m_bIncludeCustomAttribs = prefs.GetProfileInt(m_sPrefsKey, _T("IncludeCustomAttribs"), FALSE);
	m_bIncludeDates = prefs.GetProfileInt(m_sPrefsKey, _T("IncludeDates"), FALSE);
	m_bOnlyOverwriteEmpty = prefs.GetProfileInt(m_sPrefsKey, _T("OnlyOverwriteEmpty"), TRUE);
	m_bOnlyOverwriteWithNonEmpty = prefs.GetProfileInt(m_sPrefsKey, _T("OnlyOverwriteWithNonEmpty"), TRUE);
	m_bSelectSpecificAttribs = prefs.GetProfileInt(m_sPrefsKey, _T("SelectSpecificAttribs"), FALSE);
	m_bIncludeTaskTitle = prefs.GetProfileInt(m_sPrefsKey, _T("IncludeTaskTitle"), FALSE);
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

	CPreferences prefs;
	CTDCAttributeMap mapAttrib;
	CStringArray aCustAttrib;

	mapAttrib.Load(prefs, m_sPrefsKey + ("\\SelectedAttrib"));
	prefs.GetProfileArray(m_sPrefsKey + ("\\SelectedCustAttrib"), aCustAttrib);

	m_lbSpecificAttributes.SetSelectedAttributes(mapAttrib, aCustAttrib);
	
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

	mapAttrib.Add(TDCA_METADATA);

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

void CTDLPasteTaskAttributesDlg::OnOK()
{
	CTDLDialog::OnOK();

	CPreferences prefs;

	prefs.WriteProfileInt(m_sPrefsKey, _T("IncludeComments"), m_bIncludeComments);
	prefs.WriteProfileInt(m_sPrefsKey, _T("IncludeCustomAttribs"), m_bIncludeCustomAttribs);
	prefs.WriteProfileInt(m_sPrefsKey, _T("IncludeDates"), m_bIncludeDates);
	prefs.WriteProfileInt(m_sPrefsKey, _T("OnlyOverwriteEmpty"), m_bOnlyOverwriteEmpty);
	prefs.WriteProfileInt(m_sPrefsKey, _T("OnlyOverwriteWithNonEmpty"), m_bOnlyOverwriteWithNonEmpty);
	prefs.WriteProfileInt(m_sPrefsKey, _T("SelectSpecificAttribs"), m_bSelectSpecificAttribs);
	prefs.WriteProfileInt(m_sPrefsKey, _T("IncludeTaskTitle"), m_bIncludeTaskTitle);

	CTDCAttributeMap mapAttrib;
	CStringSet mapCustAttrib;
	m_lbSpecificAttributes.GetSelectedAttributes(mapAttrib, mapCustAttrib);

	mapAttrib.Save(prefs, m_sPrefsKey + ("\\SelectedAttrib"));

	CStringArray aCustAttrib;
	mapCustAttrib.CopyTo(aCustAttrib);

	prefs.WriteProfileArray(m_sPrefsKey + ("\\SelectedCustAttrib"), aCustAttrib);
}
