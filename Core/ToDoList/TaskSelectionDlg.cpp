// TaskSelectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TaskSelectionDlg.h"
#include "ToDoCtrl.h"

#include "..\shared\misc.h"
#include "..\shared\dialoghelper.h"

#include "..\Interfaces\Preferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaskSelectionDlg dialog


CTaskSelectionDlg::CTaskSelectionDlg(const CTDCCustomAttribDefinitionArray& aAttribDefs,
									 LPCTSTR szRegKey, 
									 BOOL bEnableSubtaskSelection,
									 BOOL bVisibleColumnsOnly) 
	: 
	m_lbAttribList(aAttribDefs),
	m_sRegKey(szRegKey), 
	m_bEnableSubtaskSelection(bEnableSubtaskSelection)
{
	//{{AFX_DATA_INIT(CTaskSelectionDlg)
	//}}AFX_DATA_INIT
	CPreferences prefs;

	m_bCompletedTasks = prefs.GetProfileInt(m_sRegKey, _T("CompletedTasks"), TRUE);
	m_bIncompleteTasks = prefs.GetProfileInt(m_sRegKey, _T("IncompleteTasks"), TRUE);
	m_nWhatTasks = prefs.GetProfileInt(m_sRegKey, _T("WhatTasks"), TSDT_FILTERED);
	m_bSelectedSubtasks = (prefs.GetProfileInt(m_sRegKey, _T("SelectedSubtasks"), TRUE) && m_bEnableSubtaskSelection);
	m_bSelectedParentTask = prefs.GetProfileInt(m_sRegKey, _T("SelectedParentTask"), FALSE);
	m_bIncludeComments = prefs.GetProfileInt(m_sRegKey, _T("IncludeCommentsWithVisible"), FALSE);

	if (bVisibleColumnsOnly)
		m_nAttribOption = TSDA_VISIBLE;
	else
		m_nAttribOption = prefs.GetProfileInt(m_sRegKey, _T("AttributeOption"), TSDA_ALL);
	
	// Manual attribute selection
	CString sGroup = m_sRegKey + _T("\\AttribVisibility");

	if (prefs.HasProfileSection(sGroup))
	{
		// Default attributes
		CTDCAttributeMap mapAttrib;
		int nAttrib = prefs.GetProfileInt(sGroup, _T("Count"), 0);

		while (nAttrib--)
		{
			CString sKey = Misc::MakeKey(_T("att%d"), nAttrib);
			TDC_ATTRIBUTE nAttribID = (TDC_ATTRIBUTE)prefs.GetProfileInt(sGroup, sKey, TDCA_NONE);
			ASSERT(nAttribID != TDCA_NONE);

			mapAttrib.Add(nAttribID);
		}

		// Custom attributes
		CStringSet mapCustAttribIDs;
		nAttrib = prefs.GetProfileInt(sGroup, _T("CustomCount"), 0);

		while (nAttrib--)
		{
			CString sKey = Misc::MakeKey(_T("custom%d"), nAttrib);
			CString sCustAttribID = prefs.GetProfileString(sGroup, sKey, 0);

			mapCustAttribIDs.Add(sCustAttribID);
		}

		m_lbAttribList.SetSelectedAttributes(mapAttrib, mapCustAttribIDs);
	}
}


void CTaskSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaskSelectionDlg)
	DDX_Radio(pDX, IDC_ALLTASKS, m_nWhatTasks);
	DDX_Check(pDX, IDC_INCLUDEPARENTTASK, m_bSelectedParentTask);
	DDX_Control(pDX, IDC_CUSTOMATTRIBLIST, m_lbAttribList);
	DDX_Check(pDX, IDC_INCLUDESUBTASKS, m_bSelectedSubtasks);
	DDX_Check(pDX, IDC_INCLUDECOMMENTS, m_bIncludeComments);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_INCLUDEDONE, m_bCompletedTasks);
	DDX_Check(pDX, IDC_INCLUDENOTDONE, m_bIncompleteTasks);

	// Because our radio buttons are out of sequence
	// we use a special technique
	static UINT RADIO_BTNS[] = { IDC_ALLATTRIB, IDC_VISIBLEATTRIB, IDC_CUSTOMATTRIB, IDC_TITLECOMMENTS };
	static int NUM_RADIO_BTNS = sizeof(RADIO_BTNS) / sizeof(RADIO_BTNS[0]);

	CDialogHelper::DDX_Radio(pDX, RADIO_BTNS, NUM_RADIO_BTNS, m_nAttribOption);
//	DDX_Radio(pDX, IDC_ALLATTRIB, m_nAttribOption);
}


BEGIN_MESSAGE_MAP(CTaskSelectionDlg, CDialog)
//{{AFX_MSG_MAP(CTaskSelectionDlg)
	ON_BN_CLICKED(IDC_ALLATTRIB, OnChangeAttribOption)
	ON_BN_CLICKED(IDC_CUSTOMATTRIB, OnChangeAttribOption)
	ON_BN_CLICKED(IDC_VISIBLEATTRIB, OnChangeAttribOption)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ALLTASKS, OnChangetasksOption)
	ON_BN_CLICKED(IDC_FILTERTASKS, OnChangetasksOption)
	ON_BN_CLICKED(IDC_INCLUDEDONE, OnIncludeDone)
	ON_BN_CLICKED(IDC_SELTASK, OnChangetasksOption)
	ON_BN_CLICKED(IDC_INCLUDENOTDONE, OnIncludeNotDone)
	ON_WM_DESTROY()
	ON_WM_ENABLE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskSelectionDlg message handlers

BOOL CTaskSelectionDlg::Create(UINT nIDRefFrame, CWnd* pParent, UINT nID)
{
	ASSERT (nIDRefFrame && pParent);
	
	if (CDialog::Create(IDD_TASKSELECTION_DIALOG, pParent))
	{
		if (nID != IDC_STATIC)
			SetDlgCtrlID(nID);
		
		CWnd* pFrame = pParent->GetDlgItem(nIDRefFrame);
		
		if (pFrame)
		{
			CRect rFrame;
			pFrame->GetWindowRect(rFrame);
			pParent->ScreenToClient(rFrame);
			
			MoveWindow(rFrame);

			// insert ourselves after this item in the Z-order
			SetWindowPos(pFrame, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

			// and then hide it
			pFrame->ShowWindow(SW_HIDE);
		}

		return TRUE;
	}
	
	return FALSE;
}

void CTaskSelectionDlg::OnOK() 
{
	CDialog::OnOK();
	
	// Save state
	CPreferences prefs;

	prefs.WriteProfileInt(m_sRegKey, _T("CompletedTasks"), m_bCompletedTasks);
	prefs.WriteProfileInt(m_sRegKey, _T("IncompleteTasks"), m_bIncompleteTasks);
	prefs.WriteProfileInt(m_sRegKey, _T("WhatTasks"), m_nWhatTasks);
	prefs.WriteProfileInt(m_sRegKey, _T("AttributeOption"), m_nAttribOption);
	prefs.WriteProfileInt(m_sRegKey, _T("SelectedSubtasks"), m_bSelectedSubtasks);
	prefs.WriteProfileInt(m_sRegKey, _T("SelectedParentTask"), m_bSelectedParentTask);
	prefs.WriteProfileInt(m_sRegKey, _T("IncludeCommentsWithVisible"), m_bIncludeComments);

	CString sGroup = m_sRegKey + _T("\\AttribVisibility");
	CTDCAttributeMap mapAttrib;
	CStringSet mapCustomAttribIDs;

	m_lbAttribList.GetSelectedAttributes(mapAttrib, mapCustomAttribIDs);

	// Default attributes
	prefs.WriteProfileInt(sGroup, _T("Count"), mapAttrib.GetCount());

	int nItem = 0;
	POSITION pos = mapAttrib.GetStartPosition();

	while (pos)
	{
		CString sKey = Misc::MakeKey(_T("att%d"), nItem++);
		prefs.WriteProfileInt(sGroup, sKey, mapAttrib.GetNext(pos));
	}

	// Custom Attributes
	prefs.WriteProfileInt(sGroup, _T("CustomCount"), mapCustomAttribIDs.GetCount());

	nItem = 0;
	pos = mapCustomAttribIDs.GetStartPosition();

	while (pos)
	{
		CString sKey = Misc::MakeKey(_T("custom%d"), nItem++);
		prefs.WriteProfileString(sGroup, sKey, mapCustomAttribIDs.GetNext(pos));
	}
}

void CTaskSelectionDlg::OnChangetasksOption() 
{
	UpdateData();
	
	BOOL bWantSelTasks = GetWantSelectedTasks();
	
	GetDlgItem(IDC_INCLUDEDONE)->EnableWindow(!bWantSelTasks);
	GetDlgItem(IDC_INCLUDENOTDONE)->EnableWindow(!bWantSelTasks);
	
	GetDlgItem(IDC_INCLUDESUBTASKS)->EnableWindow(bWantSelTasks);
	GetDlgItem(IDC_INCLUDEPARENTTASK)->EnableWindow(bWantSelTasks);
	
	GetParent()->SendMessage(WM_TASKSELDLG_CHANGE);
}

void CTaskSelectionDlg::OnIncludeDone() 
{
	UpdateData();
	
	// prevent the user unchecking both tick boxes
	if (!m_bCompletedTasks && !m_bIncompleteTasks)
	{
		m_bIncompleteTasks = TRUE;
		UpdateData(FALSE);
	}
	
	GetParent()->SendMessage(WM_TASKSELDLG_CHANGE);
}
void CTaskSelectionDlg::OnIncludeNotDone() 
{
	UpdateData();
	
	// prevent the user unchecking both tick boxes
	if (!m_bCompletedTasks && !m_bIncompleteTasks)
	{
		m_bCompletedTasks = TRUE;
		UpdateData(FALSE);
	}
	
	GetParent()->SendMessage(WM_TASKSELDLG_CHANGE);
}

BOOL CTaskSelectionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateEnableStates();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTaskSelectionDlg::UpdateEnableStates()
{
	GetDlgItem(IDC_CUSTOMATTRIBLIST)->EnableWindow(m_nAttribOption == TSDA_USER);
	GetDlgItem(IDC_INCLUDECOMMENTS)->EnableWindow(m_nAttribOption == TSDA_VISIBLE);

	BOOL bWantSelTasks = GetWantSelectedTasks();
	
	GetDlgItem(IDC_INCLUDEDONE)->EnableWindow(!bWantSelTasks);
	GetDlgItem(IDC_INCLUDENOTDONE)->EnableWindow(!bWantSelTasks);
	GetDlgItem(IDC_INCLUDEPARENTTASK)->EnableWindow(bWantSelTasks);

	GetDlgItem(IDC_INCLUDESUBTASKS)->EnableWindow(bWantSelTasks && m_bEnableSubtaskSelection);
	GetDlgItem(IDC_INCLUDESUBTASKS)->ShowWindow(m_bEnableSubtaskSelection ? SW_SHOW : SW_HIDE);
}

void CTaskSelectionDlg::SetWantWhatTasks(TSD_TASKS nWhat)
{
	m_nWhatTasks = nWhat;

	if (GetSafeHwnd())
	{
		UpdateData(FALSE);
		
		BOOL bWantSelTasks = GetWantSelectedTasks();
		
		GetDlgItem(IDC_INCLUDEDONE)->EnableWindow(!bWantSelTasks);
		GetDlgItem(IDC_INCLUDENOTDONE)->EnableWindow(!bWantSelTasks);
		GetDlgItem(IDC_INCLUDEPARENTTASK)->EnableWindow(bWantSelTasks);
		GetDlgItem(IDC_INCLUDESUBTASKS)->EnableWindow(bWantSelTasks && m_bEnableSubtaskSelection);
	}
}

void CTaskSelectionDlg::OnChangeAttribOption() 
{
	UpdateData();

	GetDlgItem(IDC_CUSTOMATTRIBLIST)->EnableWindow(m_nAttribOption == TSDA_USER);
	GetDlgItem(IDC_INCLUDECOMMENTS)->EnableWindow(m_nAttribOption == TSDA_VISIBLE);
}

int CTaskSelectionDlg::GetSelectedAttributes(const CToDoCtrl& tdc, CTDCAttributeMap& mapAttrib) const
{
	mapAttrib.RemoveAll();

	// attributes to export
	switch (m_nAttribOption)
	{
	case TSDA_ALL:
		mapAttrib.Add(TDCA_ALL);
		break;

	case TSDA_TITLECOMMENTS:
		mapAttrib.Add(TDCA_TASKNAME);
		mapAttrib.Add(TDCA_COMMENTS);
		break;

	case TSDA_VISIBLE:
		{
			// visible columns
			TDC::MapColumnsToAttributes(tdc.GetVisibleColumns(), mapAttrib);

			mapAttrib.Add(TDCA_TASKNAME);
			mapAttrib.Add(TDCA_CUSTOMATTRIB_ALL);

			if (m_bIncludeComments)
				mapAttrib.Add(TDCA_COMMENTS);
		}
		break;

	case TSDA_USER:
		m_lbAttribList.GetSelectedAttributes(mapAttrib);
		mapAttrib.Add(TDCA_TASKNAME);
		break;
	}

	return mapAttrib.GetCount();
}

void CTaskSelectionDlg::OnEnable(BOOL bEnable)
{
	CDialogHelper::EnableAllCtrls(this, bEnable);

	if (bEnable)
		UpdateEnableStates();
}

BOOL CTaskSelectionDlg::GetWantSelectedSubtasks() const
{
	return (GetWantSelectedTasks() && m_bSelectedSubtasks);
}

BOOL CTaskSelectionDlg::GetWantSelectedParentTask() const
{
	return (GetWantSelectedTasks() && m_bSelectedParentTask);
}

BOOL CTaskSelectionDlg::GetWantCompletedTasksOnly() const
{
	return (!GetWantSelectedTasks() && m_bCompletedTasks && !m_bIncompleteTasks);
}

BOOL CTaskSelectionDlg::GetWantIncompleteTasksOnly() const
{
	return (!GetWantSelectedTasks() && !m_bCompletedTasks && m_bIncompleteTasks);
}
