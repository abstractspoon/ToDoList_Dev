// TDLTasklistImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLTasklistImportDlg.h"
#include "ToDoCtrl.h"
#include "TDLContentMgr.h"
#include "tdcmsg.h"
#include "preferencesdlg.h"
#include "TDCToDoCtrlPreferenceHelper.h"

#include "..\shared\DialogHelper.h"
#include "..\shared\ContentMgr.h"
#include "..\shared\uiextensionmgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

static CUIExtensionMgr s_extm;
static CTDLContentMgr s_cm;
static CONTENTFORMAT s_cf = _T("PLAIN_TEXT");
static TDCCOLEDITFILTERVISIBILITY s_vis;

CTDLTasklistImportCtrl::CTDLTasklistImportCtrl() : CFilteredToDoCtrl(s_extm, s_cm, s_cf, s_vis)
{
}

void CTDLTasklistImportCtrl::ShowAllColumns()
{
	TDCCOLEDITFILTERVISIBILITY vis;
	vis.SetAllColumnsVisible(TRUE);

	SetColumnEditFilterVisibility(vis);
}

void CTDLTasklistImportCtrl::DeselectAll()
{
	m_taskTree.DeselectAll();
}

/////////////////////////////////////////////////////////////////////////////
// CTDLTasklistImportDlg dialog

CTDLTasklistImportDlg::CTDLTasklistImportDlg(const CString& sFilePath, CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TDLIMPORTEXPORT_DIALOG, pParent), m_eFilePath(FES_NOBROWSE), m_nLoadRes(TDCF_UNSET)
{
	//{{AFX_DATA_INIT(CTDLTasklistImportDlg)
	m_bImportSubtasks = TRUE;
	//}}AFX_DATA_INIT
	m_bResetCreationDate = TRUE;
	m_sFilePath = sFilePath;
}


void CTDLTasklistImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLTasklistImportDlg)
	DDX_Check(pDX, IDC_RESETCREATIONDATE, m_bResetCreationDate);
	DDX_Text(pDX, IDC_TDLFILEPATH, m_sFilePath);
	DDX_Control(pDX, IDC_TDLFILEPATH, m_eFilePath);
	DDX_Check(pDX, IDC_IMPORTSUBTASKS, m_bImportSubtasks);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLTasklistImportDlg, CDialog)
	//{{AFX_MSG_MAP(CTDLTasklistImportDlg)
	ON_BN_CLICKED(IDC_SELECTALL, OnSelectall)
	ON_BN_CLICKED(IDC_SELECTNONE, OnSelectnone)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_TDCN_SELECTIONCHANGE, OnTDCNotifySelectionChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLTasklistImportDlg message handlers

BOOL CTDLTasklistImportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// create todoctrl in the space of IDC_TODOCTRL
	CRect rToDoCtrl = CDialogHelper::GetCtrlRect(this, IDC_TODOCTRL);

	if (m_tdc.Create(rToDoCtrl, this, IDC_TODOCTRL+1))
	{
		CTDCToDoCtrlPreferenceHelper::UpdateToDoCtrl(CPreferencesDlg(), m_tdc);

		m_nLoadRes = m_tdc.Load(m_sFilePath);

		if (m_nLoadRes != TDCF_SUCCESS)
		{
			EndDialog(IDOK);
		}
		else
		{
			m_tdc.SetMaximizeState(TDCMS_MAXTASKLIST);
			m_tdc.SetReadonly(TRUE);
			m_tdc.SetFocusToTasks();
			m_tdc.MoveWindow(rToDoCtrl);
			m_tdc.SetUITheme(CUIThemeFile());
			m_tdc.ShowAllColumns();
			m_tdc.ResizeAttributeColumnsToFit();
			m_tdc.SetGridlineColor(RGB(200, 200, 200));
			m_tdc.ExpandTasks(TDCEC_ALL);
			m_tdc.ShowWindow(SW_SHOW);

			// Select All
			PostMessage(WM_COMMAND, MAKEWPARAM(IDC_SELECTALL, BN_CLICKED), (LPARAM)::GetDlgItem(*this, IDC_SELECTALL));

		}

	}
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLTasklistImportDlg::OnSelectall() 
{
	m_tdc.SelectAll();
}

void CTDLTasklistImportDlg::OnSelectnone() 
{
	m_tdc.DeselectAll();
}

LRESULT CTDLTasklistImportDlg::OnTDCNotifySelectionChange(WPARAM, LPARAM)
{
	GetDlgItem(IDOK)->EnableWindow(m_tdc.GetSelectedCount());
	GetDlgItem(IDC_IMPORTSUBTASKS)->EnableWindow(m_tdc.SelectedTasksHaveChildren());

	return 0L;
}

void CTDLTasklistImportDlg::OnOK()
{
	CDialog::OnOK();

	if (m_nLoadRes == TDCF_SUCCESS)
		m_tdc.GetSelectedTasks(m_tasksSelected, TDCGT_ALL, (m_bImportSubtasks ? 0 : TDCGSTF_NOTSUBTASKS));
}

IIMPORT_RESULT CTDLTasklistImportDlg::GetSelectedTasks(ITaskList8* pTasks)
{
	ASSERT(pTasks);
	
	if (!pTasks)
		return IIR_BADINTERFACE;

	switch (m_nLoadRes)
	{
	case TDCF_SUCCESS:
		ResetSelectedTaskCreationDate(m_tasksSelected.GetFirstTask(), TRUE);
		
		m_tasksSelected.CopyTasksTo(pTasks);
		return IIR_SUCCESS;

	case TDCF_BADMSXML:
	case TDCF_NOTTASKLIST:
		return IIR_BADFORMAT;

	case TDCF_INUSE:
	case TDCF_NOTALLOWED:
	case TDCF_BADNETWORK:
	case TDCF_NOTEXIST:
	case TDCF_NOENCRYPTIONDLL:
	case TDCF_UNKNOWNENCRYPTION:
		return IIR_BADFILE;
	}

	// all else
	return IIR_OTHER;
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
				ResetSelectedTaskCreationDate(hTask, FALSE);
				
				hSibling = m_tasksSelected.GetNextTask(hSibling);
			}
		}
	}
}
