// BurndownWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BurndownExt.h"
#include "BurndownWnd.h"
#include "BurndownStatic.h"

#include "..\shared\mapex.h"
#include "..\shared\misc.h"
#include "..\shared\themed.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\datehelper.h"
#include "..\shared\enstring.h"

#include "..\3rdparty\dibdata.h"

#include "..\Interfaces\ipreferences.h"

#include <float.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const COLORREF COLOR_GREEN	= RGB(122, 204,   0); 
const COLORREF COLOR_RED	= RGB(204,   0,   0); 
const COLORREF COLOR_YELLOW = RGB(204, 164,   0); 
const COLORREF COLOR_BLUE	= RGB(0,     0, 244); 
const COLORREF COLOR_PINK	= RGB(234,  28,  74); 
const COLORREF COLOR_ORANGE	= RGB(255,  91,  21); 

/////////////////////////////////////////////////////////////////////////////

const UINT WM_REBUILDGRAPH = (WM_APP+1);

/////////////////////////////////////////////////////////////////////////////

enum // m_dwUpdateGraphOnShow
{
	REBUILD_GRAPH	= 0x01,
	RESORT_DATA		= 0x02,
	UPDATE_EXTENTS	= 0x04
};

/////////////////////////////////////////////////////////////////////////////
// CBurndownWnd dialog

CBurndownWnd::CBurndownWnd(CWnd* pParent /*=NULL*/)
	: 
	CDialog(IDD_STATISTICS_DLG, pParent),
	m_nChartType(BCT_REMAININGDAYS),
	m_dwUpdateGraphOnShow(0),
	m_graph(m_data)
{
	//{{AFX_DATA_INIT(CBurndownWnd)
	//}}AFX_DATA_INIT
	m_icon.LoadIcon(IDR_STATISTICS);
}

CBurndownWnd::~CBurndownWnd()
{

}

void CBurndownWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBurndownWnd)
	DDX_Control(pDX, IDC_FRAME, m_stFrame);
	DDX_Control(pDX, IDC_DISPLAY, m_cbDisplay);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
		m_nChartType = (BURNDOWN_CHARTTYPE)CDialogHelper::GetSelectedItemData(m_cbDisplay);
	else
		CDialogHelper::SelectItemByData(m_cbDisplay, m_nChartType);
}


BEGIN_MESSAGE_MAP(CBurndownWnd, CDialog)
	//{{AFX_MSG_MAP(CBurndownWnd)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, OnHelp)
	ON_WM_HELPINFO()
	ON_CBN_SELCHANGE(IDC_DISPLAY, OnSelchangeDisplay)
	ON_WM_SHOWWINDOW()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_REBUILDGRAPH, OnRebuildGraph)
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBurndownWnd message handlers

void CBurndownWnd::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	
	delete this;
}

void CBurndownWnd::OnHelp()
{
	GetParent()->SendMessage(WM_IUI_DOHELP, 0, (LPARAM)GetTypeID());
}

BOOL CBurndownWnd::OnHelpInfo(HELPINFO* /*lpHelpInfo*/)
{
	OnHelp();
	return TRUE;
}

BOOL CBurndownWnd::Create(DWORD dwStyle, const RECT &/*rect*/, CWnd* pParentWnd, UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	UINT nSize = sizeof(CPlex);

	if (CDialog::Create(IDD_STATISTICS_DLG, pParentWnd))
	{
		SetWindowLong(*this, GWL_STYLE, dwStyle);
		SetDlgCtrlID(nID);

		return TRUE;
	}

	return FALSE;
}

BOOL CBurndownWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// create toolbar
	if (m_toolbar.CreateEx(this))
	{
		const COLORREF MAGENTA = RGB(255, 0, 255);

		VERIFY(m_toolbar.LoadToolBar(IDR_TOOLBAR, IDB_TOOLBAR_STD, MAGENTA));
		VERIFY(m_tbHelper.Initialize(&m_toolbar, this));

		CRect rToolbar = CDialogHelper::GetCtrlRect(this, IDC_TB_PLACEHOLDER);
		m_toolbar.Resize(rToolbar.Width(), rToolbar.TopLeft());
		m_toolbar.RefreshButtonStates(TRUE);
	}

	// Initialise graph
	CRect rFrame;
	m_stFrame.GetWindowRect(rFrame);
	ScreenToClient(rFrame);
	rFrame.DeflateRect(1, 1);

	VERIFY(m_graph.SubclassDlgItem(IDC_GRAPH, this));

	// Init combo
	for (int nDisplay = 0; nDisplay < NUM_DISPLAY; nDisplay++)
	{
		const DISPLAYITEM& di = STATSDISPLAY[nDisplay];
		CDialogHelper::AddString(m_cbDisplay, di.nYAxisID, nDisplay);
	}

	RebuildGraph(FALSE, FALSE, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBurndownWnd::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	//CString sKey(szKey);

	pPrefs->WriteProfileInt(szKey, _T("GraphType"), m_nChartType);
}

void CBurndownWnd::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey, bool bAppOnly) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// application preferences
	double dHoursInDay = _ttof(pPrefs->GetProfileString(_T("Preferences"), _T("HoursInDay"), _T("8")));
	int nDaysInWeek = pPrefs->GetProfileInt(_T("Preferences"), _T("DaysInWeek"), 5);

	if (m_graph.SetTimeIntervals(nDaysInWeek, dHoursInDay))
		RebuildGraph(FALSE, FALSE, TRUE);
	
	// burn down specific options
	if (!bAppOnly)
	{
		//CString sKey(szKey);
		m_nChartType = (BURNDOWN_CHARTTYPE)pPrefs->GetProfileInt(szKey, _T("GraphType"), BCT_INCOMPLETETASKS);

		if (m_nChartType > NUM_DISPLAY)
			m_nChartType = BCT_INCOMPLETETASKS;

		CDialogHelper::SelectItemByData(m_cbDisplay, m_nChartType);
	}
}

void CBurndownWnd::SetUITheme(const UITHEME* pTheme)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	GraphicsMisc::VerifyDeleteObject(m_brBack);
	
	if (CThemed::IsAppThemed() && pTheme)
	{
		m_theme = *pTheme;
		m_brBack.CreateSolidBrush(pTheme->crAppBackLight);
		
		// intentionally set background colours to be same as ours
		m_toolbar.SetBackgroundColors(m_theme.crAppBackLight, m_theme.crAppBackLight, FALSE, FALSE);
		m_toolbar.SetHotColor(m_theme.crToolbarHot);
	}
}

bool CBurndownWnd::ProcessMessage(MSG* /*pMsg*/) 
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
//	if (!IsWindowEnabled())
//		return false;

	// process editing shortcuts
	// TODO

	return false;
}

void CBurndownWnd::FilterToolTipMessage(MSG* /*pMsg*/)
{
	// Not yet needed
}

bool CBurndownWnd::GetLabelEditRect(LPRECT /*pEdit*/)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return false;
}

bool CBurndownWnd::PrepareNewTask(ITaskList* /*pTask*/) const 
{ 
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return false; 
}

IUI_HITTEST CBurndownWnd::HitTest(const POINT& /*ptScreen*/) const
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return IUI_NOWHERE;
}

bool CBurndownWnd::SelectTask(DWORD /*dwTaskID*/)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// because we can't change the selection
	// in this plugin we don't care what it's set to
	return true;
}

bool CBurndownWnd::SelectTasks(const DWORD* /*pdwTaskIDs*/, int /*nTaskCount*/)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// because we can't change the selection
	// in this plugin we don't care what it's set to
	return true;
}

bool CBurndownWnd::WantTaskUpdate(IUI_ATTRIBUTE nAttribute) const
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	switch (nAttribute)
	{
	case IUI_DONEDATE:
	case IUI_STARTDATE:
	case IUI_CREATIONDATE:
	case IUI_TIMEEST:
	case IUI_TIMESPENT:
		return true;
	}

	// all else 
	return false;
}

void CBurndownWnd::BuildData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL bAndSiblings, BOOL bCheckExist)
{
	if (hTask == NULL)
		return;

	// Not interested in references
	if (!pTasks->IsTaskReference(hTask))
	{
		// only interested in leaf tasks
		if (!pTasks->IsTaskParent(hTask))
		{
			STATSITEM* pSI = m_data.AddItem(pTasks->GetTaskID(hTask));

			if (pSI) // means it's new
			{
				pSI->dtStart = GetTaskStartDate(pTasks, hTask);
				pSI->dtDone = GetTaskDoneDate(pTasks, hTask);
				pSI->dTimeEst = pTasks->GetTaskTimeEstimate(hTask, pSI->nTimeEstUnits, false);
				pSI->dTimeSpent = pTasks->GetTaskTimeSpent(hTask, pSI->nTimeSpentUnits, false);

				// make sure start is less than done
				if (pSI->IsDone() && pSI->HasStart())
					pSI->dtStart = min(pSI->dtStart, pSI->dtDone);
			}
		}
		else // Process children
		{
			BuildData(pTasks, pTasks->GetFirstTask(hTask), TRUE, bCheckExist);
		}
	}

	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			BuildData(pTasks, hSibling, FALSE, bCheckExist);
			
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
}

void CBurndownWnd::UpdateTasks(const ITaskList* pTaskList, IUI_UPDATETYPE nUpdate, const IUI_ATTRIBUTE* pAttributes, int nNumAttributes)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pTaskList, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return;
	}

	// we must have been initialized already
	ASSERT(m_graph.GetSafeHwnd());

	switch (nUpdate)
	{
	case IUI_ALL:
		m_data.RemoveAll();

		BuildData(pTasks, pTasks->GetFirstTask(), TRUE, FALSE);
		RebuildGraph(TRUE, TRUE, TRUE);
		break;
		
	case IUI_NEW:
		BuildData(pTasks, pTasks->GetFirstTask(), TRUE, TRUE);
		RebuildGraph(TRUE, TRUE, TRUE);
		break;
		
	case IUI_EDIT:
		UpdateTask(pTasks, pTasks->GetFirstTask(), nUpdate, CSet<IUI_ATTRIBUTE>(pAttributes, nNumAttributes), TRUE);
		RebuildGraph(TRUE, TRUE, TRUE);
		break;
		
	case IUI_DELETE:
		if (RemoveDeletedTasks(pTasks))
			RebuildGraph(FALSE, TRUE, TRUE);
		break;
		
	default:
		ASSERT(0);
	}
}

void CBurndownWnd::UpdateTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib, BOOL bAndSiblings)
{
	// handle task if not NULL (== root)
	if (hTask == NULL)
		return;

	// Not interested in references
	if (pTasks->IsTaskReference(hTask))
		return;
	
	ASSERT(nUpdate == IUI_EDIT);

	// only interested in leaf tasks
	if (!pTasks->IsTaskParent(hTask))
	{
		DWORD dwTaskID = pTasks->GetTaskID(hTask);

		// if the task we've been given is actually a parent task
		// then we'll have no record of it, so the following
		// lookup has a legitimate reason to fail
		STATSITEM* pSI = m_data.GetItem(dwTaskID);
		ASSERT(pSI);
		
		if (pSI)
		{
			if (attrib.Has(IUI_DONEDATE))
				pSI->dtDone = GetTaskDoneDate(pTasks, hTask);

			if (attrib.Has(IUI_STARTDATE))
			{
				pSI->dtStart = GetTaskStartDate(pTasks, hTask);
				
				// make sure start is less than done
				if (pSI->IsDone() && pSI->HasStart())
					pSI->dtStart = min(pSI->dtStart, pSI->dtDone);
			}

			if (attrib.Has(IUI_TIMEEST))
				pSI->dTimeEst = pTasks->GetTaskTimeEstimate(hTask, pSI->nTimeEstUnits, false);

			if (attrib.Has(IUI_TIMESPENT))
				pSI->dTimeSpent = pTasks->GetTaskTimeSpent(hTask, pSI->nTimeSpentUnits, false);
		}
		else
		{
			int breakpoint = 0;
		}
	}
	
	// children
	UpdateTask(pTasks, pTasks->GetFirstTask(hTask), nUpdate, attrib, TRUE);
	
	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			UpdateTask(pTasks, hSibling, nUpdate, attrib, FALSE);
			
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
}

COleDateTime CBurndownWnd::GetTaskStartDate(const ITASKLISTBASE* pTasks, HTASKITEM hTask)
{
	time64_t tDate = 0;
	COleDateTime dtStart;

	if (pTasks->GetTaskStartDate64(hTask, FALSE, tDate))
		dtStart = GetTaskDate(tDate);
	
	if (!CDateHelper::IsDateSet(dtStart) && pTasks->GetTaskCreationDate64(hTask, tDate))
		dtStart = GetTaskDate(tDate);
	
	return dtStart;
}

COleDateTime CBurndownWnd::GetTaskDoneDate(const ITASKLISTBASE* pTasks, HTASKITEM hTask)
{
	time64_t tDate = 0;
	COleDateTime dtDone;
	
	if (pTasks->GetTaskDoneDate64(hTask, tDate))
		dtDone = GetTaskDate(tDate);

	return dtDone;
}

COleDateTime CBurndownWnd::GetTaskDate(time64_t tDate)
{
	return (tDate > 0) ? CDateHelper::GetDate(tDate) : COleDateTime();
}

BOOL CBurndownWnd::RemoveDeletedTasks(const ITASKLISTBASE* pTasks)
{
	// iterating sorted data is quickest
	int nOrgCount = m_data.GetSize();
	int nItem = nOrgCount;

	while (nItem--)
	{
		const STATSITEM* pSI = m_data[nItem];

		if (!pTasks->FindTask(pSI->dwTaskID))
			m_data.RemoveAt(nItem);
	}

	return (m_data.GetSize() != nOrgCount);
}

bool CBurndownWnd::DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData) 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	switch (nCmd)
	{
	case IUI_SAVETOIMAGE:
		if (pData && !m_data.IsEmpty())
		{
			CBitmap bmImage;

			if (m_graph.SaveToImage(bmImage))
			{
				CDibData dib;

				if (dib.CreateDIB(bmImage) && dib.SaveDIB(pData->szFilePath))
					return true;
			}
		}
		break;

	case IUI_EXPANDALL:
	case IUI_COLLAPSEALL:
	case IUI_EXPANDSELECTED:
	case IUI_COLLAPSESELECTED:
	case IUI_SORT:
	case IUI_SETFOCUS:
	case IUI_RESIZEATTRIBCOLUMNS:
	case IUI_GETNEXTTASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTASK:
	case IUI_GETPREVTOPLEVELTASK:
		// not handled
		break;
	}

	return false;
}

bool CBurndownWnd::CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* /*pData*/) const 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	switch (nCmd)
	{
	case IUI_SAVETOIMAGE:
		return !m_data.IsSorted();

	case IUI_EXPANDALL:
	case IUI_COLLAPSEALL:
	case IUI_EXPANDSELECTED:
	case IUI_COLLAPSESELECTED:
	case IUI_SORT:
	case IUI_SETFOCUS:
	case IUI_RESIZEATTRIBCOLUMNS:
	case IUI_GETNEXTTASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTASK:
	case IUI_GETPREVTOPLEVELTASK:
	case IUI_SELECTTASK:
		// not handled
		break;
	}

	return false;
}

BOOL CBurndownWnd::OnEraseBkgnd(CDC* pDC) 
{
	CDialogHelper::ExcludeChild(&m_graph, pDC);

	// then our background
	if (m_brBack.GetSafeHandle())
	{
		CRect rClient;
		GetClientRect(rClient);

		pDC->FillSolidRect(rClient, m_theme.crAppBackLight);
		return TRUE;
	}
	
	// else
	return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CBurndownWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CBurndownWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (m_stFrame.GetSafeHwnd())
	{
		CRect rFrame;
		m_stFrame.GetWindowRect(rFrame);
		
		ScreenToClient(rFrame);
		rFrame.right = cx;
		rFrame.bottom = cy;

		m_stFrame.MoveWindow(rFrame);

		rFrame.DeflateRect(1, 1);
		m_graph.MoveWindow(rFrame);
	}
}

void CBurndownWnd::RebuildGraph(BOOL bSortData, BOOL bUpdateExtents, BOOL bCheckVisibility)
{
	if (bCheckVisibility && !m_graph.IsWindowVisible())
	{
		m_dwUpdateGraphOnShow = REBUILD_GRAPH;
		m_dwUpdateGraphOnShow |= (bSortData ? RESORT_DATA : 0);
		m_dwUpdateGraphOnShow |= (bUpdateExtents ? UPDATE_EXTENTS : 0);
		
		return;
	}

	if (bSortData)
		m_data.Sort();

	m_graph.RebuildGraph(bUpdateExtents);
}

void CBurndownWnd::OnSelchangeDisplay()
{
	UpdateData();

	m_graph.SetChartType(m_nChartType);
}

void CBurndownWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow && m_dwUpdateGraphOnShow)
	{
		ASSERT(m_dwUpdateGraphOnShow & REBUILD_GRAPH);
		PostMessage(WM_REBUILDGRAPH);
	}
}

LRESULT CBurndownWnd::OnRebuildGraph(WPARAM /*wp*/, LPARAM /*lp*/)
{
	ASSERT(m_dwUpdateGraphOnShow & REBUILD_GRAPH);

	BOOL bSortData = (m_dwUpdateGraphOnShow & RESORT_DATA);
	BOOL bUpdateExtents = (m_dwUpdateGraphOnShow & UPDATE_EXTENTS);

	m_dwUpdateGraphOnShow = 0;

	RebuildGraph(bSortData, bUpdateExtents, FALSE);

	return 0L;
}

