// GanttCreateDependsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "GanttMsg.h"
#include "GanttCreateDependsDlg.h"

#include "..\shared\enstring.h"
#include "..\shared\misc.h"
#include "..\shared\localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGanttCreateDependsDlg dialog


CGanttCreateDependsDlg::CGanttCreateDependsDlg(CWnd* pParent /*=NULL*/)
	: 
	CDialog(IDD_CREATEDEPENDS_DIALOG, pParent),
	m_dwFromTaskID(0),
	m_dwNewToTaskID(0),
	m_nStage(GCDDS_PICKINGCANCELLED),
	m_nMode(GCDDM_NONE)
{
	//{{AFX_DATA_INIT(CGanttCreateDependsDlg)
	m_sCurStage = _T("");
	//}}AFX_DATA_INIT
}


void CGanttCreateDependsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGanttCreateDependsDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_CURRENTSTAGE, m_sCurStage);
}


BEGIN_MESSAGE_MAP(CGanttCreateDependsDlg, CDialog)
	//{{AFX_MSG_MAP(CGanttCreateDependsDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGanttCreateDependsDlg message handlers

GCDD_MODE CGanttCreateDependsDlg::GetMode() const
{ 
	if (GetSafeHwnd())
		return m_nMode;
	
	// else
	return GCDDM_NONE;
}

BOOL CGanttCreateDependsDlg::Create(GCDD_MODE nMode, CWnd* pOwner)
{
	// so that DoDataExchange will work
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// sanity check
	ASSERT ((nMode == GCDDM_ADD) || 
			(nMode == GCDDM_EDIT) || 
			(nMode == GCDDM_DELETE));

	// set mode here so we can handle it in WM_INITDIALOG
	m_nMode = nMode;

	if (CDialog::Create(IDD_CREATEDEPENDS_DIALOG, pOwner))
	{
		SetOwner(pOwner);
		return TRUE;
	}

	// else
	m_nStage = GCDDS_PICKINGCANCELLED;
	m_nMode = GCDDM_NONE;

	return FALSE;
}

void CGanttCreateDependsDlg::OnDestroy() 
{
	// if called from outside we treat as a cancel
	if (!IsPickingCompleted())
		Cancel(FALSE);

	if (GetOwner())
		GetOwner()->SendMessage(WM_GANTTDEPENDDLG_CLOSE, m_nMode, (LPARAM)GetSafeHwnd());

	m_nMode = GCDDM_NONE;
	
	CDialog::OnDestroy();
}

void CGanttCreateDependsDlg::OnCancel()
{
	Cancel(TRUE);
}

// external
void CGanttCreateDependsDlg::Cancel()
{
	Cancel(TRUE);
}

// internal
void CGanttCreateDependsDlg::Cancel(BOOL bDestroyWindow)
{
	// so that DestroyWindow() will work
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_nStage = GCDDS_PICKINGCANCELLED;
	m_nMode = GCDDM_NONE;
	
	if (GetSafeHwnd() && bDestroyWindow)
		DestroyWindow();
}

BOOL CGanttCreateDependsDlg::SetFromTask(DWORD dwFromTaskID)
{
	ASSERT(IsPickingFromTask());

	if (!IsPickingFromTask())
		return FALSE;

	m_dwFromTaskID = dwFromTaskID;
	m_dwCurToTaskID = 0;
	
	// set next stage
	m_nStage = GCDDS_PICKINGTOTASK;
	UpdatePrompt();

	return TRUE;
}

BOOL CGanttCreateDependsDlg::SetFromDependency(DWORD dwFromTaskID, DWORD dwCurToTaskID)
{
	// so that DestroyWindow() will work
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ASSERT(IsPickingFromDependency());

	if (!IsPickingFromDependency())
		return FALSE;

	ASSERT(dwFromTaskID);
	ASSERT(dwCurToTaskID);

	if (!dwFromTaskID || !dwCurToTaskID)
		return FALSE;
	
	m_dwFromTaskID = dwFromTaskID;
	m_dwCurToTaskID = dwCurToTaskID;
	
	// set next stage
	switch (m_nMode)
	{
	case GCDDM_EDIT:
		m_nStage = GCDDS_PICKINGTOTASK;
		UpdatePrompt();
		break;
		
	case GCDDM_DELETE:
		m_nStage = GCDDS_PICKINGCOMPLETED;

		// We're done
		DestroyWindow();
		break;
		
	case GCDDM_ADD:
	default:
		ASSERT(0);
		break;
	}

	return TRUE;
}

BOOL CGanttCreateDependsDlg::SetToTask(DWORD dwToTaskID)
{
	// so that DestroyWindow() will work
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ASSERT(IsPickingToTask());
	
	if (!IsPickingToTask())
		return FALSE;

	// can't be the same ID
	if (dwToTaskID == m_dwFromTaskID)
		return FALSE;

	m_dwNewToTaskID = dwToTaskID;
	m_nStage = GCDDS_PICKINGCOMPLETED;

	// We're done
	DestroyWindow();

	return TRUE;
}

DWORD CGanttCreateDependsDlg::GetFromTask() const
{
	switch (m_nMode)
	{
	case GCDDM_ADD:
		ASSERT(IsPickingToTask() || IsPickingCompleted());
		break;

	case GCDDM_EDIT:
		ASSERT(m_dwCurToTaskID);
		ASSERT(IsPickingToTask() || IsPickingCompleted());
		break;
		
	case GCDDM_DELETE:
		ASSERT(m_dwCurToTaskID);
		ASSERT(IsPickingCompleted());
		break;
		
	default:
		ASSERT(0);
		break;
	}

	ASSERT(m_dwFromTaskID);
	return m_dwFromTaskID;
}

DWORD CGanttCreateDependsDlg::GetFromDependency(DWORD& dwCurToTaskID) const
{
	switch (m_nMode)
	{
	case GCDDM_EDIT:
		ASSERT(IsPickingToTask() || IsPickingCompleted());
		break;

	case GCDDM_DELETE:
		ASSERT(IsPickingCompleted());
		break;

	case GCDDM_ADD:
	default:
		ASSERT(0);
		break;
	}
	
	ASSERT(m_dwFromTaskID);
	ASSERT(m_dwCurToTaskID);
	
	dwCurToTaskID = m_dwCurToTaskID;
	return m_dwFromTaskID;
}

DWORD CGanttCreateDependsDlg::GetToTask() const
{
	switch (m_nMode)
	{
	case GCDDM_EDIT:
	case GCDDM_ADD:
		ASSERT(IsPickingCompleted());
		break;
		
	case GCDDM_DELETE:
	default:
		ASSERT(0);
		break;
	}
	
	ASSERT(m_dwFromTaskID);
	ASSERT(m_dwNewToTaskID);
	
	return m_dwNewToTaskID;
}

BOOL CGanttCreateDependsDlg::IsPicking() const 
{ 
	return (IsPickingFromTask() || 
			IsPickingToTask() ||
			IsPickingFromDependency()); 
}

BOOL CGanttCreateDependsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CLocalizer::EnableTranslation(*GetDlgItem(IDC_CURRENTSTAGE), FALSE);
	
	// reset state
	m_dwFromTaskID = m_dwCurToTaskID = m_dwNewToTaskID = 0;

	// set initial stage and dialog title
	CEnString sTitle;

	switch (m_nMode)
	{
	case GCDDM_ADD:
		m_nStage = GCDDS_PICKINGFROMTASK;
		sTitle.LoadString(IDS_CREATEDEPEND_TITLE);
		break;

	case GCDDM_EDIT:
		m_nStage = GCDDS_PICKINGFROMDEPEND;
		sTitle.LoadString(IDS_EDITDEPEND_TITLE);
		break;
		
	case GCDDM_DELETE:
		m_nStage = GCDDS_PICKINGFROMDEPEND;
		sTitle.LoadString(IDS_DELETEDEPEND_TITLE);
		break;
		
	default:
		ASSERT(0);
	}

	SetWindowText(sTitle);
	UpdatePrompt();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGanttCreateDependsDlg::UpdatePrompt()
{
	// so that UpdateData() will work
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CStringArray aStages;

	switch (m_nMode)
	{
	case GCDDM_ADD:
		if (IsPickingFromTask())
		{
			aStages.Add(CEnString(IDS_PICKINGFROMTASK));
		}
		else
		{
			aStages.Add(CEnString(IDS_PICKINGFROMTASK) + CEnString(IDS_PICKED));
			aStages.Add(CEnString(IDS_PICKINGTOTASKADD));
		}
		break;
		
	case GCDDM_EDIT:
		if (IsPickingFromDependency())
		{
			aStages.Add(CEnString(IDS_PICKINGFROMDEPEND_EDIT));
		}
		else
		{
			aStages.Add(CEnString(IDS_PICKINGFROMDEPEND_EDIT) + CEnString(IDS_PICKED));
			aStages.Add(CEnString(IDS_PICKINGTOTASKEDIT));
		}
		break;
		
	case GCDDM_DELETE:
		aStages.Add(CEnString(IDS_PICKINGFROMDEPEND_DELETE));
		break;
	}

	ASSERT(aStages.GetSize());

	m_sCurStage = Misc::FormatArrayAsNumberedList(aStages);
	UpdateData(FALSE);
}
