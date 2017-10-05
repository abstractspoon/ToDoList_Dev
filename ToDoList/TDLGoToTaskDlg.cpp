// TDLGoToTaskDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLGoToTaskDlg.h"

#include "..\shared\Misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const LPCTSTR TITLE_MASK = _T("0123456789");

/////////////////////////////////////////////////////////////////////////////
// CTDLGoToTaskDlg dialog


CTDLGoToTaskDlg::CTDLGoToTaskDlg(const CToDoCtrl& tdc, CWnd* pParent /*=NULL*/)
	: CDialog(IDD_GOTOTASK_DIALOG, pParent), m_tdc(tdc), m_eTaskID(TITLE_MASK)
{
	//{{AFX_DATA_INIT(CTDLGoToTaskDlg)
	//}}AFX_DATA_INIT
}


void CTDLGoToTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLGoToTaskDlg)
	DDX_Control(pDX, IDC_TASKID, m_eTaskID);
	DDX_Control(pDX, IDC_TASKTITLE, m_eTaskTitle);
	DDX_Text(pDX, IDC_TASKID, m_sTaskID);
	DDX_Text(pDX, IDC_TASKTITLE, m_sTaskTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLGoToTaskDlg, CDialog)
	//{{AFX_MSG_MAP(CTDLGoToTaskDlg)
	ON_EN_SETFOCUS(IDC_TASKID, OnEditFocusChange)
	ON_EN_SETFOCUS(IDC_TASKTITLE, OnEditFocusChange)
	ON_EN_KILLFOCUS(IDC_TASKID, OnEditFocusChange)
	ON_EN_KILLFOCUS(IDC_TASKTITLE, OnEditFocusChange)
	ON_EN_CHANGE(IDC_TASKTITLE, OnChangeTaskTitle)
	ON_EN_CHANGE(IDC_TASKID, OnChangeTaskID)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLGoToTaskDlg message handlers

BOOL CTDLGoToTaskDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateEditPrompts();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLGoToTaskDlg::UpdateEditPrompts()
{
	// One time initialisation
	static CString sPrompt;

	if (sPrompt.IsEmpty())
		sPrompt.Format(_T("<%s>"), CEnString(IDS_GOTOTASK_NOSUCHTASK));

	// Hide the prompts when the 'other's' text is empty
	m_wndPrompts.SetEditPrompt(m_eTaskID, (m_sTaskTitle.IsEmpty() ? _T("") : sPrompt), TRUE);
	m_wndPrompts.SetEditPrompt(m_eTaskTitle, (m_sTaskID.IsEmpty() ? _T("") : sPrompt), TRUE);
}

void CTDLGoToTaskDlg::OnEditFocusChange() 
{
	if (GetFocus() == &m_eTaskID)
	{
		m_eTaskID.SetReadOnly(FALSE);
		m_eTaskID.SetMask(TITLE_MASK); // restore mask
	}
	else
	{
		m_eTaskID.SetReadOnly(TRUE);
		m_eTaskID.SetMask(_T(""), ME_EXCLUDE); // clear mask
	}

	if (GetFocus() == &m_eTaskTitle)
		m_eTaskTitle.SetReadOnly(FALSE);
	else
		m_eTaskTitle.SetReadOnly(TRUE);

	UpdateTaskID();
	UpdateTaskTitle();
	
	GetDlgItem(IDOK)->EnableWindow(GetTaskID() != 0);
}

void CTDLGoToTaskDlg::OnChangeTaskTitle() 
{
	if (GetFocus() == &m_eTaskTitle)
	{
		UpdateTaskID();
		GetDlgItem(IDOK)->EnableWindow(!m_sTaskID.IsEmpty());
	}
}

void CTDLGoToTaskDlg::OnChangeTaskID() 
{
	if (GetFocus() == &m_eTaskID)
	{
		UpdateTaskTitle();
		GetDlgItem(IDOK)->EnableWindow(!m_sTaskTitle.IsEmpty());
	}
}

DWORD CTDLGoToTaskDlg::FindTask(const CString& sText, CString& sTitle) const
{
	SEARCHPARAMS params;
	params.aRules.Add(SEARCHPARAM(TDCA_TASKNAME, FOP_INCLUDES, Misc::GetQuoted(sText)));

	CResultArray aResults;

	if (m_tdc.FindTasks(params, aResults))
	{
		sTitle = aResults[0].aMatched[0];
		return aResults[0].dwTaskID;
	}

	// else
	return 0;
}

void CTDLGoToTaskDlg::UpdateTaskID()
{
	UpdateData(TRUE);

	if (m_eTaskID.GetStyle() & ES_READONLY)
	{
		// Restore trailing text
		CString sTitle;
		DWORD dwTaskID = FindTask(m_sTaskTitle, sTitle);

		if (dwTaskID)
			m_sTaskID.Format(_T("%ld (%s)"), dwTaskID, sTitle);
		else
			m_sTaskID.Empty();
	}
	else
	{
		// Remove trailing text
		DWORD dwTaskID = GetTaskID();

		if (dwTaskID)
			m_sTaskID.Format(_T("%ld"), dwTaskID);
	}

	UpdateData(FALSE);
	UpdateEditPrompts();
}

void CTDLGoToTaskDlg::UpdateTaskTitle()
{
	if (m_eTaskTitle.GetStyle() & ES_READONLY)
	{
		UpdateData(TRUE);

		DWORD dwTaskID = GetTaskID();

		if (m_tdc.HasTask(dwTaskID))
			m_sTaskTitle = m_tdc.GetTaskTitle(dwTaskID);
		else
			m_sTaskTitle.Empty();

		UpdateData(FALSE);
		UpdateEditPrompts();
	}
}
