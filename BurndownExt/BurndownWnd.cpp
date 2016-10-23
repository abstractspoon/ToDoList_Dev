// BurndownWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BurndownExt.h"
#include "BurndownWnd.h"

#include "..\shared\mapex.h"
#include "..\shared\misc.h"
#include "..\shared\themed.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\datehelper.h"
#include "..\shared\holdredraw.h"
#include "..\shared\enstring.h"

#include "..\Interfaces\ipreferences.h"

#include <float.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const int MIN_XSCALE_SPACING = 50; // pixels

enum
{
	SCALE_DAY		= 1,
	SCALE_WEEK		= 7,
	SCALE_MONTH		= 30,
	SCALE_2MONTH	= 60,
	SCALE_QUARTER	= 90,
	SCALE_HALFYEAR	= 180,
	SCALE_YEAR		= 365,
};

static int SCALES[] = 
{
	SCALE_DAY,
	SCALE_WEEK,
	SCALE_MONTH,
	SCALE_2MONTH,
	SCALE_QUARTER,
	SCALE_HALFYEAR,
	SCALE_YEAR,
};
static int NUM_SCALES = sizeof(SCALES) / sizeof(int);

/////////////////////////////////////////////////////////////////////////////

STATSITEM::STATSITEM() : dwTaskID(0) 
{
}

STATSITEM::~STATSITEM() 
{
}

BOOL STATSITEM::HasStart() const
{
	return CDateHelper::IsDateSet(dtStart);
}

BOOL STATSITEM::IsDone() const
{
	return CDateHelper::IsDateSet(dtDone);
}

void STATSITEM::MinMax(COleDateTime& dtMin, COleDateTime& dtMax) const
{
	MinMax(dtStart, dtMin, dtMax);
	MinMax(dtDone, dtMin, dtMax);
}

void STATSITEM::MinMax(const COleDateTime& date, COleDateTime& dtMin, COleDateTime& dtMax)
{
	if (CDateHelper::IsDateSet(date))
	{
		if (CDateHelper::IsDateSet(dtMin))
			dtMin = min(dtMin, date);
		else
			dtMin = date;
		
		if (CDateHelper::IsDateSet(dtMax))
			dtMax = max(dtMax, date);
		else
			dtMax = date;
	}
}

/////////////////////////////////////////////////////////////////////////////

static CMapStatsItems* PSORTDATA = NULL;

/////////////////////////////////////////////////////////////////////////////
// CBurndownWnd dialog


CBurndownWnd::CBurndownWnd(CWnd* pParent /*=NULL*/)
	: 
	CDialog(IDD_STATISTICS_DLG, pParent),
	m_nDisplay(0),
	m_nScale(1),
	m_hIcon(NULL)
{
	//{{AFX_DATA_INIT(CBurndownWnd)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_STATISTICS);
}

CBurndownWnd::~CBurndownWnd()
{

}

void CBurndownWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBurndownWnd)
	DDX_Control(pDX, IDC_FRAME, m_stFrame);
	DDX_CBIndex(pDX, IDC_DISPLAY, m_nDisplay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBurndownWnd, CDialog)
	//{{AFX_MSG_MAP(CBurndownWnd)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, OnHelp)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBurndownWnd message handlers

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

void CBurndownWnd::SavePreferences(IPreferences* /*pPrefs*/, LPCTSTR /*szKey*/) const 
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
// 	CString sKey(szKey);

}

void CBurndownWnd::LoadPreferences(const IPreferences* /*pPrefs*/, LPCTSTR /*szKey*/, bool /*bAppOnly*/) 
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
// 	CString sKey(szKey);

	// application preferences

	// burn down specific options
// 	if (!bAppOnly)
// 	{
// 
// 	}
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

bool CBurndownWnd::WantEditUpdate(IUI_ATTRIBUTE nAttribute) const
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	switch (nAttribute)
	{
	case IUI_DONEDATE:
	case IUI_STARTDATE:
	case IUI_CREATIONDATE:
		return true;
	}

	// all else 
	return false;
}

bool CBurndownWnd::WantSortUpdate(IUI_ATTRIBUTE /*nAttribute*/) const
{
// 	AFX_MANAGE_STATE(AfxGetStaticModuleState());
// 	
// 	switch (nAttribute)
// 	{
// 	case IUI_DONEDATE:
// 	case IUI_STARTDATE:
// 		return true;
// 	}

	// all else 
	return false;
}

void CBurndownWnd::BuildData(const ITaskList15* pTasks)
{
	// reset data structures
	m_data.RemoveAll();
	m_aDateOrdered.RemoveAll();

	if (pTasks->GetTaskCount())
	{
		CDateHelper::ClearDate(m_dtEarliestDone);
		CDateHelper::ClearDate(m_dtLatestDone);

		// get the info we're interested in
		BuildData(pTasks, pTasks->GetFirstTask(), TRUE);

		// sort by start date
		SortData();

		// rebuild graph
		BuildGraph();
	}
	else if (CDateHelper::IsDateSet(m_dtEarliestDone))
	{
		// just clear the data but leave the axes
		m_graph.ClearData(0);
	}
	else
	{
		m_dtEarliestDone = m_dtLatestDone = COleDateTime::GetCurrentTime();
	}
}

void CBurndownWnd::BuildData(const ITaskList15* pTasks, HTASKITEM hTask, BOOL bAndSiblings)
{
	if (hTask == NULL)
		return;

	// Not interested in references
	if (pTasks->IsTaskReference(hTask))
		return;

	// only interested in leaf tasks
	if (!pTasks->IsTaskParent(hTask))
	{
		DWORD dwTaskID = pTasks->GetTaskID(hTask);
		STATSITEM si;

		// check we haven't got this task already
		if (!m_data.Lookup(dwTaskID, si))
		{
			// interested in all tasks
			si.dwTaskID = dwTaskID;
			si.dtStart = GetTaskStartDate(pTasks, hTask);
			si.dtDone = GetTaskDoneDate(pTasks, hTask);

			// make sure start is less than done
			if (si.IsDone())
				si.dtStart = min(si.dtStart, si.dtDone);

			// update data extents as we go
			si.MinMax(m_dtEarliestDone, m_dtLatestDone);
			
			m_data[si.dwTaskID] = si;
			m_aDateOrdered.Add(si.dwTaskID);
		}
	}
	else // Process children
	{
		BuildData(pTasks, pTasks->GetFirstTask(hTask), TRUE);
	}

	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			BuildData(pTasks, hSibling, FALSE);
			
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
}

void CBurndownWnd::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const IUI_ATTRIBUTE* pAttributes, int nNumAttributes)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// we must have been initialized already
	ASSERT(m_graph.GetSafeHwnd());

	CHoldRedraw hr(m_graph);
		
	const ITaskList15* pTasks14 = GetITLInterface<ITaskList15>(pTasks, IID_TASKLIST15);
	
	switch (nUpdate)
	{
	case IUI_ALL:
		BuildData(pTasks14); // builds graph too
		break;
		
	case IUI_NEW:
	case IUI_EDIT:
		if (nUpdate == IUI_NEW)
			BuildData(pTasks14, pTasks14->GetFirstTask(), TRUE);
		else
			UpdateTask(pTasks14, pTasks14->GetFirstTask(), nUpdate, CSet<IUI_ATTRIBUTE>(pAttributes, nNumAttributes), TRUE);

		UpdateDataExtents();
		SortData();
		BuildGraph();
		break;
		
	case IUI_DELETE:
		if (RemoveDeletedTasks(pTasks14))
		{
			UpdateDataExtents();
			BuildGraph();
		}
		break;
		
	case IUI_MOVE:
		break;
		
	default:
		ASSERT(0);
	}
}

void CBurndownWnd::UpdateDataExtents()
{
	if (m_data.GetCount())
	{
		CDateHelper::ClearDate(m_dtEarliestDone);
		CDateHelper::ClearDate(m_dtLatestDone);

		DWORD dwTaskID;
		STATSITEM si;

		POSITION pos = m_data.GetStartPosition();

		while (pos)
		{
			m_data.GetNextAssoc(pos, dwTaskID, si);
			ASSERT(dwTaskID);

			si.MinMax(m_dtEarliestDone, m_dtLatestDone);
		}
	}
	else
	{
		m_dtEarliestDone = m_dtLatestDone = COleDateTime::GetCurrentTime();
	}
}

void CBurndownWnd::UpdateTask(const ITaskList15* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib, BOOL bAndSiblings)
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
		STATSITEM si;
		
		if (m_data.Lookup(dwTaskID, si))
		{
			if (attrib.HasKey(IUI_STARTDATE))
			{
				si.dtStart = GetTaskStartDate(pTasks, hTask);
				
				// make sure start is less than done
				if (si.IsDone())
					si.dtStart = min(si.dtStart, si.dtDone);
				
				m_data[dwTaskID] = si;
			}
				
			if (attrib.HasKey(IUI_DONEDATE))
			{
				si.dtDone = GetTaskDoneDate(pTasks, hTask);
				m_data[dwTaskID] = si;
			}
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

COleDateTime CBurndownWnd::GetTaskStartDate(const ITaskList15* pTasks, HTASKITEM hTask)
{
	time64_t tDate = 0;
	COleDateTime dtStart;

	if (pTasks->GetTaskStartDate64(hTask, FALSE, tDate))
		dtStart = GetTaskDate(tDate);
	
	if (!CDateHelper::IsDateSet(dtStart) && pTasks->GetTaskCreationDate64(hTask, tDate))
		dtStart = GetTaskDate(tDate);
	
	return dtStart;
}

COleDateTime CBurndownWnd::GetTaskDoneDate(const ITaskList15* pTasks, HTASKITEM hTask)
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

BOOL CBurndownWnd::RemoveDeletedTasks(const ITaskList15* pTasks)
{
	// iterating sorted data is quickest
	int nOrgCount = m_aDateOrdered.GetSize();
	int nItem = nOrgCount;

	while (nItem--)
	{
		DWORD dwTaskID = m_aDateOrdered[nItem];

		if (!pTasks->FindTask(dwTaskID))
		{
			m_aDateOrdered.RemoveAt(nItem);
			m_data.RemoveKey(dwTaskID);
		}
	}

	return (m_aDateOrdered.GetSize() != nOrgCount);
}

void CBurndownWnd::Release()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (GetSafeHwnd())
		DestroyWindow();
	
	delete this;
}

bool CBurndownWnd::DoAppCommand(IUI_APPCOMMAND nCmd, DWORD /*dwExtra*/) 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	switch (nCmd)
	{
	case IUI_EXPANDALL:
	case IUI_COLLAPSEALL:
	case IUI_EXPANDSELECTED:
	case IUI_COLLAPSESELECTED:
	case IUI_TOGGLABLESORT:
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

bool CBurndownWnd::CanDoAppCommand(IUI_APPCOMMAND nCmd, DWORD /*dwExtra*/) const 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	switch (nCmd)
	{
	case IUI_EXPANDALL:
	case IUI_COLLAPSEALL:
	case IUI_EXPANDSELECTED:
	case IUI_COLLAPSESELECTED:
	case IUI_TOGGLABLESORT:
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


BOOL CBurndownWnd::OnEraseBkgnd(CDC* pDC) 
{
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

	CRect rFrame;
	m_stFrame.GetWindowRect(rFrame);
	ScreenToClient(rFrame);
	rFrame.DeflateRect(1, 1);

	VERIFY(m_graph.SubclassDlgItem(IDC_GRAPH, this));

	m_graph.SetTitle(CEnString(IDS_BURNDOWN_TITLE));
	m_graph.SetBkGnd(GetSysColor(COLOR_WINDOW));

	m_graph.SetDatasetStyle(0, HMX_DATASET_STYLE_AREALINE);
	m_graph.SetDatasetPenColor( 0, RGB( 255, 128, 255) );
	m_graph.SetDatasetMinToZero(0, true);

	m_graph.SetXText(CEnString(IDS_TIME_AXIS));
	m_graph.SetXLabelStep(m_nScale);
	m_graph.SetXLabelsAreTicks(true);

	m_graph.SetYText(CEnString(IDS_TASK_AXIS));
	m_graph.SetYTicks(10);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
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

		int nOldScale = m_nScale;
		RebuildXScale();
		
		// handle scale change
		if (m_nScale != nOldScale)
			BuildGraph();
	}
}

void CBurndownWnd::SortData()
{
	if (!IsDataSorted())
	{
		ASSERT(PSORTDATA == NULL);

		PSORTDATA = &m_data;
		qsort(m_aDateOrdered.GetData(), m_aDateOrdered.GetSize(), sizeof(DWORD*), CompareStatItems);
		PSORTDATA = NULL;
	}
}

int CBurndownWnd::CompareStatItems(const void* pV1, const void* pV2)
{
	ASSERT(PSORTDATA);

	DWORD dwSI1 = *(static_cast<const DWORD*>(pV1));
	DWORD dwSI2 = *(static_cast<const DWORD*>(pV2));

	ASSERT(dwSI1 && dwSI2 && (dwSI1 != dwSI2));
	
	STATSITEM si1, si2;

	VERIFY (PSORTDATA->Lookup(dwSI1, si1));
	VERIFY (PSORTDATA->Lookup(dwSI2, si2));

	// incomplete tasks always come last
	BOOL bStart1 = CDateHelper::IsDateSet(si1.dtStart);
	BOOL bStart2 = CDateHelper::IsDateSet(si2.dtStart);

	if (!bStart1 && !bStart2)
		return 0;

	if (!bStart1)
		return -1; // no date precedes any date

	if (!bStart2)
		return 1; // no date precedes any date

	if (si1.dtStart < si2.dtStart)
		return -1;

	if (si1.dtStart > si2.dtStart)
		return 1;

	return 0;
}

BOOL CBurndownWnd::IsDataSorted() const
{
	int nNumItems = m_aDateOrdered.GetSize();
	COleDateTime dtLast;

	// look for the first item whose start date
	// preceeds the start date of the previous task
	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		DWORD dwTaskID = m_aDateOrdered[nItem];
		STATSITEM si;

		if (m_data.Lookup(dwTaskID, si))
		{
			ASSERT (CDateHelper::IsDateSet(si.dtStart));

			if (si.dtStart < dtLast)
				return FALSE;

			dtLast = si.dtStart;
		}
		else
		{
			// should never get here
			ASSERT(0);
		}
	}

	return TRUE;
}

int CBurndownWnd::CalculateRequiredScale() const
{
	// calculate new x scale
	int nDataWidth = m_graph.GetDataArea().cx;
	int nNumDays = GetDataDuration();

	// work thru the available scales until we find a suitable one
	int nScale = 0;

	for (; nScale < NUM_SCALES; nScale++)
	{
		int nSpacing = MulDiv(SCALES[nScale], nDataWidth, nNumDays);

		if (nSpacing > MIN_XSCALE_SPACING)
			return SCALES[nScale];
	}

	return SCALE_YEAR;
}

void CBurndownWnd::RebuildXScale()
{
	m_graph.ClearXScaleLabels();

	// calc new scale
	m_nScale = CalculateRequiredScale();
	m_graph.SetXLabelStep(m_nScale);

	// Get new start and end dates
	COleDateTime dtStart = GetGraphStartDate();
	COleDateTime dtEnd = GetGraphEndDate();

	// build ticks
	int nNumDays = ((int)dtEnd.m_dt - (int)dtStart.m_dt);
	COleDateTime dtTick = dtStart;
	CString sTick;

	for (int nDay = 0; nDay <= nNumDays; nDay += m_nScale)
	{
		sTick = CDateHelper::FormatDate(dtTick);
		m_graph.SetXScaleLabel(nDay, sTick);

		// next Tick date
		switch (m_nScale)
		{
		case SCALE_DAY:
			dtTick.m_dt += 1.0;
			break;
			
		case SCALE_WEEK:
			CDateHelper::OffsetDate(dtTick, 1, DHU_WEEKS);
			break;
			
		case SCALE_MONTH:
			CDateHelper::OffsetDate(dtTick, 1, DHU_MONTHS);
			break;
			
		case SCALE_2MONTH:
			CDateHelper::OffsetDate(dtTick, 2, DHU_MONTHS);
			break;
			
		case SCALE_QUARTER:
			CDateHelper::OffsetDate(dtTick, 3, DHU_MONTHS);
			break;
			
		case SCALE_HALFYEAR:
			CDateHelper::OffsetDate(dtTick, 6, DHU_MONTHS);
			break;
			
		case SCALE_YEAR:
			CDateHelper::OffsetDate(dtTick, 1, DHU_YEARS);
			break;
			
		default:
			ASSERT(0);
		}
	}

	//m_graph.Invalidate();
}

COleDateTime CBurndownWnd::GetGraphStartDate() const
{
	COleDateTime dtStart(m_dtEarliestDone);

	// back up a bit to always show first completion
	dtStart -= COleDateTimeSpan(7.0);

	SYSTEMTIME st = { 0 };
	VERIFY(dtStart.GetAsSystemTime(st));

	switch (m_nScale)
	{
	case SCALE_DAY:
	case SCALE_WEEK:
		// make sure we start at the beginning of a week
		dtStart.m_dt -= st.wDayOfWeek;
		return dtStart;
		
	case SCALE_MONTH:
		st.wDay = 1; // start of month;
		break;
		
	case SCALE_2MONTH:
		st.wDay = 1; // start of month;
		st.wMonth = (WORD)(st.wMonth - ((st.wMonth - 1) % 2)); // previous even month
		break;

	case SCALE_QUARTER:
		st.wDay = 1; // start of month;
		st.wMonth = (WORD)(st.wMonth - ((st.wMonth - 1) % 3)); // previous quarter
		break;
		
	case SCALE_HALFYEAR:
		st.wDay = 1; // start of month;
		st.wMonth = (WORD)(st.wMonth - ((st.wMonth - 1) % 6)); // previous half-year
		break;
		
	case SCALE_YEAR:
		st.wDay = 1; // start of month;
		st.wMonth = 1; // start of year
		break;

	default:
		ASSERT(0);
	}

	return COleDateTime(st.wYear, st.wMonth, st.wDay, 0, 0, 0);
}

COleDateTime CBurndownWnd::GetGraphEndDate() const
{
	COleDateTime dtEnd = max(m_dtLatestDone, COleDateTime::GetCurrentTime()) + COleDateTimeSpan(1.0);

	// avoid unnecessary call to GetAsSystemTime()
	if (m_nScale == SCALE_DAY)
		return dtEnd;

	SYSTEMTIME st = { 0 };
	VERIFY(dtEnd.GetAsSystemTime(st));

	switch (m_nScale)
	{
	case SCALE_DAY:
		ASSERT(0); // handled above
		break;
		
	case SCALE_WEEK:
		break;
		
	case SCALE_MONTH:
		st.wDay = (WORD)CDateHelper::GetDaysInMonth(st.wMonth, st.wYear); // end of month;
		break;
		
	case SCALE_2MONTH:
		CDateHelper::IncrementMonth(st, ((st.wMonth - 1) % 2)); // next even month
		st.wDay = (WORD)CDateHelper::GetDaysInMonth(st.wMonth, st.wYear); // end of month;
		break;

	case SCALE_QUARTER:
		CDateHelper::IncrementMonth(st, ((st.wMonth - 1) % 3)); // next quarter
		st.wDay = (WORD)CDateHelper::GetDaysInMonth(st.wMonth, st.wYear); // end of month;
		break;
		
	case SCALE_HALFYEAR:
		CDateHelper::IncrementMonth(st, ((st.wMonth - 1) % 6)); // next half-year
		st.wDay = (WORD)CDateHelper::GetDaysInMonth(st.wMonth, st.wYear); // end of month;
		break;
		
	case SCALE_YEAR:
		st.wDay = 31;
		st.wMonth = 12;
		break;

	default:
		ASSERT(0);
	}

	return COleDateTime(st.wYear, st.wMonth, st.wDay, 0, 0, 0);
}

void CBurndownWnd::BuildGraph()
{
	m_graph.ClearData(0);

	RebuildXScale();

	// build the graph
	COleDateTime dtStart = GetGraphStartDate();
	COleDateTime dtEnd = GetGraphEndDate();

	int nNumDays = ((int)dtEnd.m_dt - (int)dtStart.m_dt);

	for (int nDay = 0; nDay <= nNumDays; nDay++)
	{
		COleDateTime date(dtStart.m_dt + nDay);
		int nNumNotDone = CalculateIncompleteTaskCount(date);

		m_graph.AddData(0, nNumNotDone);
	}

	m_graph.CalcDatas();
}

int CBurndownWnd::GetDataDuration() const
{
	double dStart = m_dtEarliestDone;
	ASSERT(dStart > 0.0);

	double dEnd = max(m_dtLatestDone, COleDateTime::GetCurrentTime()).m_dt + 1;
	ASSERT(dEnd >= dStart);

	return ((int)dEnd - (int)dStart);
}

int CBurndownWnd::CalculateIncompleteTaskCount(const COleDateTime& date)
{
	// work thru items until we hit the first task whose 
	// start date > date, counting how many are not complete as we go
	int nNumItems = m_aDateOrdered.GetSize();
	int nNumNotDone = 0;

	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		DWORD dwTaskID = m_aDateOrdered[nItem];
		STATSITEM si;
		VERIFY (GetStatsItem(dwTaskID, si));

		if (si.dtStart > date)
			break;

		if (!si.IsDone() || (si.dtDone > date))
			nNumNotDone++;
	}

	return nNumNotDone;
}

BOOL CBurndownWnd::GetStatsItem(DWORD dwTaskID, STATSITEM& si) const
{
	return m_data.Lookup(dwTaskID, si);
}
