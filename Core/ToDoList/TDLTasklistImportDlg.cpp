// TDLTasklistImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLTasklistImportDlg.h"
#include "tdcmsg.h"
#include "preferencesdlg.h"
#include "TDCToDoCtrlPreferenceHelper.h"

#include "..\shared\DialogHelper.h"
#include "..\shared\holdredraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CTDLTaskTreeImportCtrl::CTDLTaskTreeImportCtrl() 
	: 
	CTDLTaskTreeCtrl(m_ilIcons, 
					m_data, 
					m_styles, 
					m_tld, 
					m_visibleCols.GetVisibleColumns(), 
					m_aCustAttribDefs),
	m_data(m_styles, m_aCustAttribDefs),
#pragma warning (disable: 4355)
	m_exporter(m_data, *this, m_mgrContent)
#pragma warning (default: 4355)
{
	CPreferencesDlg prefs;

	TODOITEM tdiDef;
	prefs.GetDefaultTaskAttributes(tdiDef);

	m_data.SetDefaultCommentsFormat(tdiDef.cfComments);
	m_data.SetDefaultTimeUnits(tdiDef.timeEstimate.nUnits, tdiDef.timeSpent.nUnits);
	m_data.SetDefaultStatus(tdiDef.sStatus);

	CTDCToDoCtrlPreferenceHelper::PopulateStyles(prefs, m_styles);
	m_styles[TDCS_READONLY] = TRUE;

	SetGridlineColor(prefs.GetGridlineColor());
	SetAlternateLineColor(prefs.GetAlternateLineColor());

	CDWordArray aColors;
	prefs.GetPriorityColors(aColors);
	SetPriorityColors(aColors);

	CString sFaceName(_T("Tahoma"));
	int nFontSize = 8;
	
	prefs.GetTreeFont(sFaceName, nFontSize);
	VERIFY(GraphicsMisc::CreateFont(m_font, sFaceName, nFontSize));
}

BOOL CTDLTaskTreeImportCtrl::BuildTree(const CString& sFilePath)
{
	CTaskFile tasks;

	if (!tasks.Load(sFilePath))
		return FALSE;

	tasks.GetCustomAttributeDefs(m_aCustAttribDefs);

	m_data.BuildDataModel(tasks);

	AddTasksToTree(m_data.GetStructure(), NULL);

	m_visibleCols.SetAllColumnsVisible();
	m_visibleCols.SetColumnVisible(TDCC_TRACKTIME, FALSE);
	m_visibleCols.SetColumnVisible(TDCC_ICON, FALSE);
	m_visibleCols.SetColumnVisible(TDCC_DONE, FALSE);

	OnColumnVisibilityChange(m_visibleCols.GetVisibleColumns());
	RecalcAllColumnWidths();
	
	m_ilIcons.LoadImages(sFilePath);
	OnImageListChange();

	SetFont(m_font);
	
	return TRUE;
}

void CTDLTaskTreeImportCtrl::AddTasksToTree(const TODOSTRUCTURE* pTDSParent, HTREEITEM htiParent)
{
	if (!pTDSParent)
		return;

	int nSubtask = pTDSParent->GetSubTaskCount();
	
	// reverse-order tree-insertion is quicker according to Raymond Chen
	while (nSubtask--)
	{
		const TODOSTRUCTURE* pTDSChild = pTDSParent->GetSubTask(nSubtask);
		DWORD dwTaskID = pTDSChild->GetTaskID();
		
		// add this item to tree
		HTREEITEM htiChild = InsertItem(dwTaskID, htiParent, TVI_FIRST);
		ASSERT(htiChild);
			
		// and its children
		AddTasksToTree(pTDSChild, htiChild); // RECURSIVE call
	}
}

int CTDLTaskTreeImportCtrl::GetSelectedTasks(CTaskFile& tasks, BOOL bWantAllSubtasks)
{
	tasks.SetCustomAttributeDefs(m_aCustAttribDefs);

	// Get an ordered list so that the parent always precede their subtasks
	CHTIList listSel;
	TSH().CopySelection(listSel, bWantAllSubtasks, TRUE);

	POSITION pos = listSel.GetHeadPosition();

	while (pos)
		AddTreeItemToTasks(listSel.GetNext(pos), tasks, bWantAllSubtasks);

	return tasks.GetTaskCount();
}

void CTDLTaskTreeImportCtrl::AddTreeItemToTasks(HTREEITEM hti, CTaskFile& tasks, BOOL bWantAllSubtasks)
{
	if (!hti)
		return;

	DWORD dwTaskID = GetTaskID(hti);
	DWORD dwParentID = GetTaskID(GetParentItem(hti));

	HTREEITEM htiParent = m_tcTasks.GetParentItem(hti);
	HTASKITEM htParent = (dwParentID ? tasks.FindTask(dwParentID) : NULL);

	if (bWantAllSubtasks)
	{
		m_exporter.ExportTask(dwTaskID, tasks, htParent);
	}
	else
	{
		const TODOITEM* pTDI = NULL;
		const TODOSTRUCTURE* pTDS = NULL;

		if (m_data.GetTrueTask(dwTaskID, pTDI, pTDS))
		{
			HTASKITEM hTask = tasks.NewTask(pTDI->sTitle, htParent, dwTaskID, 0);
			ASSERT(hTask);

			if (hTask)
				m_exporter.ExportAllTaskAttributes(pTDI, pTDS, tasks, hTask);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTDLTasklistImportDlg dialog

CTDLTasklistImportDlg::CTDLTasklistImportDlg(const CString& sFilePath, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_TDLIMPORTEXPORT_DIALOG, _T(""), pParent), 
	m_eFilePath(FES_NOBROWSE), 
	m_nLoadRes(TDCF_UNSET),
	m_bFirstShow(TRUE)
{
	//{{AFX_DATA_INIT(CTDLTasklistImportDlg)
	m_bImportSubtasks = TRUE;
	//}}AFX_DATA_INIT
	m_bResetCreationDate = TRUE;
	m_sFilePath = sFilePath;
}


void CTDLTasklistImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLTasklistImportDlg)
	DDX_Check(pDX, IDC_RESETCREATIONDATE, m_bResetCreationDate);
	DDX_Text(pDX, IDC_TDLFILEPATH, m_sFilePath);
	DDX_Control(pDX, IDC_TDLFILEPATH, m_eFilePath);
	DDX_Check(pDX, IDC_IMPORTSUBTASKS, m_bImportSubtasks);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLTasklistImportDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLTasklistImportDlg)
	ON_BN_CLICKED(IDC_SELECTALL, OnSelectall)
	ON_BN_CLICKED(IDC_SELECTNONE, OnSelectnone)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_TDCN_SELECTIONCHANGE, OnTDCNotifySelectionChange)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLTasklistImportDlg message handlers

BOOL CTDLTasklistImportDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();
	
	// create task tree in the space of IDC_TODOCTRL
	CRect rToDoCtrl = CDialogHelper::GetCtrlRect(this, IDC_TODOCTRL);

	if (!m_taskTree.Create(this, rToDoCtrl, IDC_TODOCTRL + 1) || !m_taskTree.BuildTree(m_sFilePath))
		EndDialog(IDOK);
	else
		m_nLoadRes = TDCF_SUCCESS;
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTDLTasklistImportDlg::OnEraseBkgnd(CDC* pDC)
{
	// Don't know why but exaanding all the tasks prior
	// to the tree actually being visible causes it all
	// sorts of rendering problems some of which are a
	// consequence of my choices but which are hard to change.
 	if (m_bFirstShow)
	{
		m_bFirstShow = FALSE;

		m_taskTree.ExpandAll();
		m_taskTree.SelectAll();
	}
	
	CDialogHelper::ExcludeChild(&m_taskTree, pDC);

	return CDialog::OnEraseBkgnd(pDC);
}

void CTDLTasklistImportDlg::OnRepositionControls(int dx, int dy)
{
	CDialogHelper::ResizeChild(&m_taskTree, dx, dy);
	CDialogHelper::ResizeChild(&m_eFilePath, dx, 0);

	CDialogHelper::OffsetCtrl(this, IDC_SELECTALL, 0, dy);
	CDialogHelper::OffsetCtrl(this, IDC_SELECTNONE, 0, dy);
	CDialogHelper::OffsetCtrl(this, IDC_EXPANDALL, 0, dy);
	CDialogHelper::OffsetCtrl(this, IDC_IMPORTSUBTASKS, 0, dy);
	CDialogHelper::OffsetCtrl(this, IDC_RESETCREATIONDATE, 0, dy);

	CDialogHelper::OffsetCtrl(this, IDC_DIVIDER, 0, dy);
	CDialogHelper::ResizeCtrl(this, IDC_DIVIDER, dx, 0);

	CDialogHelper::OffsetCtrl(this, IDOK, dx, dy);
	CDialogHelper::OffsetCtrl(this, IDCANCEL, dx, dy);
}

void CTDLTasklistImportDlg::OnSelectall() 
{
	CWaitCursor wait;
	CHoldRedraw hr(m_taskTree);

	m_taskTree.SelectAll();
}

void CTDLTasklistImportDlg::OnSelectnone() 
{
	CWaitCursor wait;
	CHoldRedraw hr(m_taskTree);

	m_taskTree.DeselectAll();
}

LRESULT CTDLTasklistImportDlg::OnTDCNotifySelectionChange(WPARAM, LPARAM)
{
	GetDlgItem(IDOK)->EnableWindow(m_taskTree.GetSelectedCount());
	GetDlgItem(IDC_IMPORTSUBTASKS)->EnableWindow(m_taskTree.SelectionHasSubtasks());

	return 0L;
}

void CTDLTasklistImportDlg::OnOK()
{
	CDialog::OnOK();

	if (m_nLoadRes == TDCF_SUCCESS)
		m_taskTree.GetSelectedTasks(m_tasksSelected, m_bImportSubtasks);
}

IIMPORTEXPORT_RESULT CTDLTasklistImportDlg::GetSelectedTasks(ITaskList* pTasks)
{
	if (pTasks == NULL)
	{
		ASSERT(0);
		return IIER_BADINTERFACE;
	}

	switch (m_nLoadRes)
	{
	case TDCF_SUCCESS:
		{
			ResetSelectedTaskCreationDate(m_tasksSelected.GetFirstTask(), TRUE);

			if (!m_tasksSelected.CopyTasksTo(pTasks))
				return IIER_SOMEFAILED;
		}
		return IIER_SUCCESS;

	case TDCF_BADMSXML:
	case TDCF_NOTTASKLIST:
		return IIER_BADFORMAT;

	case TDCF_INUSE:
	case TDCF_NOTALLOWED:
	case TDCF_BADNETWORK:
	case TDCF_NOTEXIST:
	case TDCF_NOENCRYPTIONDLL:
	case TDCF_UNKNOWNENCRYPTION:
		return IIER_BADFILE;
	}

	// all else
	return IIER_OTHER;
}

void CTDLTasklistImportDlg::ResetSelectedTaskCreationDate(HTASKITEM hTask, BOOL bAndSiblings)
{
	if (m_bResetCreationDate && hTask)
	{
		m_tasksSelected.SetTaskCreationDate(hTask, time(NULL));

		// first child
		ResetSelectedTaskCreationDate(m_tasksSelected.GetFirstTask(hTask), TRUE);

		// handle sibling tasks WITHOUT RECURSION
		if (bAndSiblings)
		{
			HTASKITEM hSibling = m_tasksSelected.GetNextTask(hTask);
			
			while (hSibling)
			{
				// FALSE == don't recurse on siblings
				ResetSelectedTaskCreationDate(hSibling, FALSE);
				
				hSibling = m_tasksSelected.GetNextTask(hSibling);
			}
		}
	}
}
