// GanttChartWnd.cpp : implementation file
//

#include "stdafx.h"
#include "NetworkDiagramExt.h"
#include "NetworkDiagramWnd.h"
#include "NetworkDiagramStatic.h"
#include "NetworkDiagramMsg.h"

#include "..\shared\themed.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\datehelper.h"
#include "..\shared\localizer.h"
#include "..\shared\autoflag.h"
#include "..\shared\holdredraw.h"
#include "..\shared\dlgunits.h"

#include "..\3rdparty\T64Utils.h"
#include "..\3rdparty\GdiPlus.h"
#include "..\3rdparty\XNamedColors.h"

#include "..\Interfaces\ipreferences.h"

#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const COLORREF DEF_ALTLINECOLOR		= RGB(230, 230, 255);
const COLORREF DEF_GRIDLINECOLOR	= RGB(192, 192, 192);
const COLORREF DEF_DONECOLOR		= RGB(128, 128, 128);

/////////////////////////////////////////////////////////////////////////////

#ifndef LVS_EX_DOUBLEBUFFER
#define LVS_EX_DOUBLEBUFFER 0x00010000
#endif

/////////////////////////////////////////////////////////////////////////////

const int PADDING = 3;
const UINT IDC_GANTTCTRL = 1001;

/////////////////////////////////////////////////////////////////////////////
// CGanttChartWnd

CNetworkDiagramWnd::CNetworkDiagramWnd(CWnd* pParent /*=NULL*/)
	: 
	CDialog(IDD_NETWORKDIAGRAM_DIALOG, pParent), 
	m_bReadOnly(FALSE),
	m_bInSelectTask(FALSE)
{
	m_icon.Load(IDR_NETWORKDIAGRAM);
}

CNetworkDiagramWnd::~CNetworkDiagramWnd()
{
}

void CNetworkDiagramWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGanttChartWnd)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_LIST1, m_ctrlNetwork);
}

BEGIN_MESSAGE_MAP(CNetworkDiagramWnd, CDialog)
	//{{AFX_MSG_MAP(CGanttChartWnd)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, OnHelp)
	ON_WM_HELPINFO()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_NCDESTROY()
	ON_WM_LBUTTONDBLCLK()

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGanttChartWnd message handlers

void CNetworkDiagramWnd::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	
	delete this;
}

void CNetworkDiagramWnd::OnHelp()
{
	GetParent()->SendMessage(WM_IUI_DOHELP, 0, (LPARAM)GetTypeID());
}

BOOL CNetworkDiagramWnd::OnHelpInfo(HELPINFO* /*lpHelpInfo*/)
{
	OnHelp();
	return TRUE;
}

void CNetworkDiagramWnd::SetReadOnly(bool bReadOnly)
{
	m_bReadOnly = bReadOnly;
	m_ctrlNetwork.SetReadOnly(bReadOnly);
}

BOOL CNetworkDiagramWnd::Create(DWORD dwStyle, const RECT &/*rect*/, CWnd* pParentWnd, UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (CDialog::Create(IDD_NETWORKDIAGRAM_DIALOG, pParentWnd))
	{
		SetWindowLong(*this, GWL_STYLE, dwStyle);
		SetDlgCtrlID(nID);

		return TRUE;
	}

	return FALSE;
}

void CNetworkDiagramWnd::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// NOTE: sort is handled by the app
	CString sKey(szKey);

}

void CNetworkDiagramWnd::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey, bool bAppOnly) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
}

void CNetworkDiagramWnd::SetUITheme(const UITHEME* pTheme)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	GraphicsMisc::VerifyDeleteObject(m_brBack);
	
	if (CThemed::IsAppThemed() && pTheme)
	{
		m_theme = *pTheme;
		m_brBack.CreateSolidBrush(m_theme.crAppBackLight);

	}
}

void CNetworkDiagramWnd::FilterToolTipMessage(MSG* pMsg) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_ctrlNetwork.FilterToolTipMessage(pMsg);
}

bool CNetworkDiagramWnd::ProcessMessage(MSG* pMsg) 
{
	if (!IsWindowEnabled())
		return false;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

// 	if (m_ctrlNetwork.ProcessMessage(pMsg))
// 		return true;

	return false;
}

bool CNetworkDiagramWnd::PrepareNewTask(ITaskList* pTask) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return m_ctrlNetwork.PrepareNewTask(pTask);
}

bool CNetworkDiagramWnd::GetLabelEditRect(LPRECT pEdit)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
// 	if (m_ctrlNetwork.GetLabelEditRect(pEdit))
// 	{
// 		// convert to screen coords
// 		m_ctrlNetwork.CWnd::ClientToScreen(pEdit);
// 		return true;
// 	}

	return false;
}

IUI_HITTEST CNetworkDiagramWnd::HitTest(POINT ptScreen) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// try tree header
// 	if (m_ctrlNetwork.PointInHeader(ptScreen))
// 		return IUI_NOWHERE;//IUI_COLUMNHEADER;
// 
// 	// then specific task
// 	if (m_ctrlNetwork.HitTestTask(ptScreen, false))
// 		return IUI_TASK;

	// else 
	return IUI_NOWHERE;
}

DWORD CNetworkDiagramWnd::HitTestTask(POINT ptScreen, bool bTitleColumnOnly) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return 0;//m_ctrlNetwork.HitTestTask(ptScreen, bTitleColumnOnly);
}

bool CNetworkDiagramWnd::SelectTask(DWORD dwTaskID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CAutoFlag af(m_bInSelectTask, TRUE);

	return false;//(m_ctrlNetwork.SelectTask(dwTaskID) != FALSE);
}

bool CNetworkDiagramWnd::SelectTasks(const DWORD* /*pdwTaskIDs*/, int /*nTaskCount*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return false; // only support single selection
}

bool CNetworkDiagramWnd::WantTaskUpdate(TDC_ATTRIBUTE nAttribute) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (CNetworkDiagramCtrl::WantEditUpdate(nAttribute) != FALSE);
}

void CNetworkDiagramWnd::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_ctrlNetwork.UpdateTasks(pTasks, nUpdate);
}

bool CNetworkDiagramWnd::DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData) 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

/*
	switch (nCmd)
	{
	case IUI_EXPANDALL:
		//m_ctrlNetwork.ExpandAll(TRUE);
		return true;

	case IUI_COLLAPSEALL:
		//m_ctrlNetwork.ExpandAll(FALSE);
		return true;

	case IUI_EXPANDSELECTED:
		//m_ctrlNetwork.ExpandItem(m_ctrlNetwork.GetSelectedItem(), TRUE, TRUE);
		return true;

	case IUI_COLLAPSESELECTED:
		//m_ctrlNetwork.ExpandItem(m_ctrlNetwork.GetSelectedItem(), FALSE);
		return true;

	case IUI_SORT:
		if (pData)
		{
			return true;
		}
		break;

	case IUI_MULTISORT:
		if (pData)
		{
			return true;
		}
		break;

	case IUI_SETFOCUS:
		m_ctrlNetwork.SetFocus();
		return true;

	case IUI_RESIZEATTRIBCOLUMNS:
		m_ctrlNetwork.ResizeListColumnsToFit(TRUE);
		return true;
		
	case IUI_SELECTTASK:
		if (pData)
			return SelectTask(pData->dwTaskID);
		break;
		
	case IUI_GETNEXTTASK:
	case IUI_GETNEXTVISIBLETASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTASK:
	case IUI_GETPREVVISIBLETASK:
	case IUI_GETPREVTOPLEVELTASK:
		if (pData)
		{
			DWORD dwNextID = m_ctrlNetwork.GetNextTask(pData->dwTaskID, nCmd);
			
			if (dwNextID && (dwNextID != pData->dwTaskID))
			{
				pData->dwTaskID = dwNextID;
				return true;
			}
		}
		break;
		
	case IUI_SAVETOIMAGE:
		if (pData)
		{
			CLockUpdates lock(GetSafeHwnd());
			CBitmap bmImage;

			if (m_ctrlNetwork.SaveToImage(bmImage))
				return (CGdiPlusBitmap(bmImage).SaveAsFile(pData->szFilePath) != FALSE);
		}
		break;

	case IUI_SELECTFIRSTTASK:
	case IUI_SELECTNEXTTASK:
	case IUI_SELECTNEXTTASKINCLCURRENT:
	case IUI_SELECTPREVTASK:
	case IUI_SELECTLASTTASK:
		if (pData)
			return (m_ctrlNetwork.SelectTask(nCmd, pData->select) != FALSE);
		break;

	case IUI_MOVETASK:
		if (pData)
		{
			ASSERT(pData->move.dwSelectedTaskID == m_ctrlNetwork.GetSelectedTaskID());

			return (m_ctrlNetwork.MoveSelectedTask(pData->move) != FALSE);
		}
		break;
	}
*/

	return false;
}

void CNetworkDiagramWnd::SetTaskFont(HFONT hFont)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHoldRedraw hr(*this);
	
	m_ctrlNetwork.SetFont(hFont, TRUE);
}

bool CNetworkDiagramWnd::CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData) const 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
/*
	switch (nCmd)
	{
	case IUI_EXPANDALL:
		return (m_ctrlNetwork.CanExpandAll() != FALSE);

	case IUI_COLLAPSEALL:
		return (m_ctrlNetwork.CanCollapseAll() != FALSE);

	case IUI_RESIZEATTRIBCOLUMNS:
	case IUI_SELECTTASK:
		return true;

	case IUI_SAVETOIMAGE:
		return (m_ctrlNetwork.GetTaskCount() > 0);

	case IUI_EXPANDSELECTED:
		{
			HTREEITEM htiSel = m_ctrlNetwork.GetSelectedItem();
			return (m_ctrlNetwork.CanExpandItem(htiSel, TRUE) != FALSE);
		}
		break;

	case IUI_COLLAPSESELECTED:
		{
			HTREEITEM htiSel = m_ctrlNetwork.GetSelectedItem();
			return (m_ctrlNetwork.CanExpandItem(htiSel, FALSE) != FALSE);
		}
		break;

	case IUI_SORT:
		if (pData)
			return (CNetworkDiagramCtrl::WantSortUpdate(pData->nSortBy) != FALSE);
		break;

	case IUI_MULTISORT:
		return true;

	case IUI_SETFOCUS:
		return (CDialogHelper::IsChildOrSame(this, GetFocus()) == FALSE);
		
	case IUI_GETNEXTVISIBLETASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVVISIBLETASK:
	case IUI_GETPREVTOPLEVELTASK:
		if (pData)
		{
			DWORD dwNextID =  m_ctrlNetwork.GetNextTask(pData->dwTaskID, nCmd);

			return (dwNextID && (dwNextID != pData->dwTaskID));
		}
		break;

	case IUI_SELECTFIRSTTASK:
	case IUI_SELECTNEXTTASK:
	case IUI_SELECTNEXTTASKINCLCURRENT:
	case IUI_SELECTPREVTASK:
	case IUI_SELECTLASTTASK:
		return true;

	case IUI_MOVETASK:
		if (pData)
			return (m_ctrlNetwork.CanMoveSelectedTask(pData->move) != FALSE);
		break;
	}
*/

	// all else
	return false;
}

void CNetworkDiagramWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	Resize(cx, cy);
}

BOOL CNetworkDiagramWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ctrlNetwork.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNetworkDiagramWnd::Resize(int cx, int cy)
{
	if (m_ctrlNetwork.GetSafeHwnd())
	{
		m_ctrlNetwork.MoveWindow(0, 0, cx, cy);
	}
}

HBRUSH CNetworkDiagramWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC && m_brBack.GetSafeHandle())
	{
		pDC->SetTextColor(m_theme.crAppText);
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brBack;
	}

	return hbr;
}

BOOL CNetworkDiagramWnd::OnEraseBkgnd(CDC* pDC) 
{
	//return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}

void CNetworkDiagramWnd::SendParentSelectionUpdate()
{
	DWORD dwTaskID = m_ctrlNetwork.GetSelectedTaskID();
	GetParent()->SendMessage(WM_IUI_SELECTTASK, 0, dwTaskID);
}

void CNetworkDiagramWnd::OnClickNetworkList(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	SendParentSelectionUpdate();

	*pResult = 0;
}

void CNetworkDiagramWnd::OnSetFocus(CWnd* /*pOldWnd*/) 
{
	m_ctrlNetwork.SetFocus();
}

void CNetworkDiagramWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if (!bShow)
		m_ctrlNetwork.CancelOperation();
}

