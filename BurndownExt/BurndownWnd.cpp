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

enum DISPLAYTYPE
{
	SD_BURNDOWN,
	SD_BURNDOWN2,
	SD_SPRINT,
};

struct DISPLAYITEM
{
	UINT nTitleID;
	UINT nYAxisID;
	DISPLAYTYPE nDisplay;
};

static DISPLAYITEM STATSDISPLAY[] = 
{
	{ IDS_DISPLAY_BURNDOWN, IDS_DISPLAY_BURNDOWN_YAXIS, SD_BURNDOWN	},
#ifdef _DEBUG
	{ IDS_DISPLAY_BURNDOWN2, IDS_DISPLAY_BURNDOWN_YAXIS, SD_BURNDOWN2 },
#endif
	{ IDS_DISPLAY_SPRINT, IDS_DISPLAY_SPRINT_YAXIS,	SD_SPRINT },
};
static int NUM_DISPLAY = sizeof(STATSDISPLAY) / sizeof(DISPLAYITEM);

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

STATSITEM::STATSITEM() : dwTaskID(0), dTimeEstDays(0.0), dTimeSpentDays(0.0)
{
	CDateHelper::ClearDate(dtStart);
	CDateHelper::ClearDate(dtDone);
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

CStatsItemArray::CStatsItemArray()
{

}

CStatsItemArray::~CStatsItemArray()
{
	RemoveAll();
}

BOOL CStatsItemArray::HasItem(DWORD dwTaskID) const
{
	return (FindItem(dwTaskID) != -1);
}

int CStatsItemArray::FindItem(DWORD dwTaskID) const
{
	int nIndex = GetSize();

	while (nIndex--)
	{
		const STATSITEM* pSI = GetAt(nIndex);

		if (pSI->dwTaskID == dwTaskID)
			return nIndex;
	}

	// not found
	return -1;
}

STATSITEM* CStatsItemArray::GetItem(DWORD dwTaskID) const
{
	int nFind = FindItem(dwTaskID);

	if (nFind == -1)
		return NULL;

	return GetAt(nFind);
}

void CStatsItemArray::RemoveAll()
{
	int nIndex = GetSize();

	while (nIndex--)
	{
		STATSITEM* pSI = GetAt(nIndex);
		delete pSI;
	}

	CArray<STATSITEM*, STATSITEM*>::RemoveAll();
}

void CStatsItemArray::RemoveAt(int nIndex, int nCount)
{
	ASSERT(nIndex >= 0 && nIndex < GetSize());
	ASSERT(nCount > 0);

	nIndex += nCount;

	while (nIndex--)
	{
		STATSITEM* pSI = GetAt(nIndex);
		delete pSI;

		CArray<STATSITEM*, STATSITEM*>::RemoveAt(nIndex);
	}
}

void CStatsItemArray::Sort()
{
	if (!IsSorted())
	{
		qsort(GetData(), GetSize(), sizeof(STATSITEM*), CompareItems);
	}
}

BOOL CStatsItemArray::IsSorted()
{
	int nNumItems = GetSize();

	if (nNumItems < 2)
		return TRUE;
	
	const STATSITEM* pSI = GetAt(0);
	ASSERT(pSI);

	COleDateTime dtPrev = pSI->dtStart;

	for (int nItem = 1; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = GetAt(nItem);
		ASSERT(pSI);

		if (pSI->HasStart())
		{
			// Stop if the preceding task has no start date
			// OR the preceding task has a later date
			if (!CDateHelper::IsDateSet(dtPrev) || (pSI->dtStart < dtPrev))
			{
				return FALSE;
			}
		}

		dtPrev = pSI->dtStart;
	}

	return TRUE;
}

int CStatsItemArray::CompareItems(const void* pV1, const void* pV2)
{
	typedef STATSITEM* LPSTATSITEM;

	const STATSITEM* pSI1 = *(static_cast<const LPSTATSITEM*>(pV1));
	const STATSITEM* pSI2 = *(static_cast<const LPSTATSITEM*>(pV2));

	ASSERT(pSI1 && pSI2 && (pSI1 != pSI2));

	// Sort by start date
	// Tasks without a start date come last to optimise searching
	BOOL bStart1 = CDateHelper::IsDateSet(pSI1->dtStart);
	BOOL bStart2 = CDateHelper::IsDateSet(pSI2->dtStart);

	if (bStart1 != bStart2)
	{
		return (bStart1 ? -1 : 1);
	}
	else if (!bStart1 && !bStart2)
	{
		return 0;
	}

	if (pSI1->dtStart < pSI2->dtStart)
		return -1;

	if (pSI1->dtStart > pSI2->dtStart)
		return 1;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CBurndownWnd dialog

CBurndownWnd::CBurndownWnd(CWnd* pParent /*=NULL*/)
	: 
	CDialog(IDD_STATISTICS_DLG, pParent),
	m_nDisplay(0),
	m_nScale(1),
	m_bGraphNeedsRebuildOnShow(FALSE)
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
		m_nDisplay = CDialogHelper::GetSelectedItemData(m_cbDisplay);
	else
		CDialogHelper::SelectItemByData(m_cbDisplay, m_nDisplay);

	DDX_CBIndex(pDX, IDC_DISPLAY, m_nDisplay);
}


BEGIN_MESSAGE_MAP(CBurndownWnd, CDialog)
	//{{AFX_MSG_MAP(CBurndownWnd)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, OnHelp)
	ON_WM_HELPINFO()
	ON_CBN_SELCHANGE(IDC_DISPLAY, OnSelchangeDisplay)
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(WM_REBUILDGRAPH, OnRebuildGraph)
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

	m_graph.SetTitle(CEnString(IDS_BURNDOWN_TITLE));
	m_graph.SetBkGnd(GetSysColor(COLOR_WINDOW));

	m_graph.SetXText(CEnString(IDS_TIME_AXIS));
	m_graph.SetXLabelsAreTicks(true);

	m_graph.SetYTicks(10);

	// Init combo
	for (int nDisplay = 0; nDisplay < NUM_DISPLAY; nDisplay++)
	{
		const DISPLAYITEM& di = STATSDISPLAY[nDisplay];
		CDialogHelper::AddString(m_cbDisplay, di.nTitleID, nDisplay);
	}
	m_cbDisplay.SetCurSel(0);
	RebuildGraph();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
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
	case IUI_TIMEEST:
	case IUI_TIMESPENT:
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

void CBurndownWnd::BuildData(const ITASKLISTBASE* pTasks)
{
	// reset data structures
	m_data.RemoveAll();

	if (pTasks->GetTaskCount())
	{
		CDateHelper::ClearDate(m_dtEarliestDate);
		CDateHelper::ClearDate(m_dtLatestDate);

		BuildData(pTasks, pTasks->GetFirstTask(), TRUE, FALSE);
		m_data.Sort();
		RebuildGraph();
	}
	else if (CDateHelper::IsDateSet(m_dtEarliestDate))
	{
		// just clear the data but leave the axes
		m_graph.ClearData(0);
	}
	else
	{
		m_dtEarliestDate = m_dtLatestDate = COleDateTime::GetCurrentTime();
	}
}

void CBurndownWnd::BuildData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL bAndSiblings, BOOL bCheckExist)
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

		// check we haven't got this task already
		if (!bCheckExist || !m_data.HasItem(dwTaskID))
		{
			STATSITEM* pSI = new STATSITEM;

			// interested in all tasks
			pSI->dwTaskID = dwTaskID;
			pSI->dtStart = GetTaskStartDate(pTasks, hTask);
			pSI->dtDone = GetTaskDoneDate(pTasks, hTask);
			pSI->dTimeEstDays = GetTaskTimeInDays(pTasks, hTask, TRUE);
			pSI->dTimeSpentDays = GetTaskTimeInDays(pTasks, hTask, FALSE);

			// make sure start is less than done
			if (pSI->IsDone() && pSI->HasStart())
				pSI->dtStart = min(pSI->dtStart, pSI->dtDone);

			// update data extents as we go
			pSI->MinMax(m_dtEarliestDate, m_dtLatestDate);
			
			m_data.Add(pSI);
		}
	}
	else // Process children
	{
		BuildData(pTasks, pTasks->GetFirstTask(hTask), TRUE, bCheckExist);
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

	CHoldRedraw hr(m_graph);
		
	switch (nUpdate)
	{
	case IUI_ALL:
		BuildData(pTasks); // builds graph too
		break;
		
	case IUI_NEW:
	case IUI_EDIT:
		if (nUpdate == IUI_NEW)
			BuildData(pTasks, pTasks->GetFirstTask(), TRUE, TRUE);
		else
			UpdateTask(pTasks, pTasks->GetFirstTask(), nUpdate, CSet<IUI_ATTRIBUTE>(pAttributes, nNumAttributes), TRUE);

		UpdateDataExtents();
		m_data.Sort();
		RebuildGraph();
		break;
		
	case IUI_DELETE:
		if (RemoveDeletedTasks(pTasks))
		{
			UpdateDataExtents();
			RebuildGraph();
		}
		break;
		
	default:
		ASSERT(0);
	}
}

void CBurndownWnd::UpdateDataExtents()
{
	if (m_data.GetCount())
	{
		CDateHelper::ClearDate(m_dtEarliestDate);
		CDateHelper::ClearDate(m_dtLatestDate);

		int nItem = m_data.GetSize();

		while (nItem--)
			m_data[nItem]->MinMax(m_dtEarliestDate, m_dtLatestDate);
	}
	else
	{
		m_dtEarliestDate = m_dtLatestDate = COleDateTime::GetCurrentTime();
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
			if (attrib.HasKey(IUI_DONEDATE))
				pSI->dtDone = GetTaskDoneDate(pTasks, hTask);

			if (attrib.HasKey(IUI_STARTDATE))
			{
				pSI->dtStart = GetTaskStartDate(pTasks, hTask);
				
				// make sure start is less than done
				if (pSI->IsDone() && pSI->HasStart())
					pSI->dtStart = min(pSI->dtStart, pSI->dtDone);
			}
				
			if (attrib.HasKey(IUI_TIMEEST))
				pSI->dTimeEstDays = GetTaskTimeInDays(pTasks, hTask, TRUE);

			if (attrib.HasKey(IUI_TIMESPENT))
				pSI->dTimeSpentDays = GetTaskTimeInDays(pTasks, hTask, FALSE);
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

double CBurndownWnd::GetTaskTimeInDays(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL bEstimate)
{
	TDC_UNITS nUnits = TDCU_NULL;
	double dTime = (bEstimate ? pTasks->GetTaskTimeEstimate(hTask, nUnits, false) : pTasks->GetTaskTimeSpent(hTask, nUnits, false));

	switch (nUnits)
	{
	case TDCU_WEEKDAYS:
	case TDCU_DAYS:
		return dTime;
	}

	// all the rest
	TH_UNITS nTHUnits = MapUnitsToTHUnits(nUnits);

	return CTimeHelper(8, 5).GetTime(dTime, nTHUnits, THU_WEEKDAYS);
}

TH_UNITS CBurndownWnd::MapUnitsToTHUnits(TDC_UNITS nUnits)
{
	switch (nUnits)
	{
	case TDCU_NULL:		return THU_NULL;
	case TDCU_MINS:		return THU_MINS;
	case TDCU_HOURS:	return THU_HOURS;
	case TDCU_DAYS:		return THU_DAYS;
	case TDCU_WEEKDAYS:	return THU_WEEKDAYS;
	case TDCU_WEEKS:	return THU_WEEKS;
	case TDCU_MONTHS:	return THU_MONTHS;
	case TDCU_YEARS:	return THU_YEARS;
	}

	// all else
	ASSERT(0);
	return THU_NULL;
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

void CBurndownWnd::Release()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (GetSafeHwnd())
		DestroyWindow();
	
	delete this;
}

bool CBurndownWnd::DoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra) 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	switch (nCmd)
	{
	case IUI_SAVETOIMAGE:
		if (dwExtra && (m_data.GetCount() > 0))
		{
			CBitmap bmImage;

			if (m_graph.SaveToImage(bmImage))
			{
				HBITMAP* pHBM = (HBITMAP*)dwExtra;
				*pHBM = (HBITMAP)bmImage.Detach();

				return true;
			}
		}
		break;

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
	case IUI_SAVETOIMAGE:
		return (m_data.GetCount() > 0);

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
	case IUI_SELECTTASK:
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
			RebuildGraph();
	}
}

int CBurndownWnd::CalculateRequiredXScale() const
{
	// calculate new x scale
	int nDataWidth = m_graph.GetDataArea().cx;
	int nNumDays = GetDataDuration();

	// work thru the available scales until we find a suitable one
	for (int nScale = 0; nScale < NUM_SCALES; nScale++)
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
	m_nScale = CalculateRequiredXScale();
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
}

COleDateTime CBurndownWnd::GetGraphStartDate() const
{
	if (m_nDisplay == SD_SPRINT)
		return m_dtEarliestDate;

	// else
	COleDateTime dtStart(m_dtEarliestDate);

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
	if (m_nDisplay == SD_SPRINT)
		return m_dtLatestDate;

	COleDateTime dtEnd = (m_dtLatestDate + COleDateTimeSpan(7.0));

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

void CBurndownWnd::RebuildGraph()
{
	if (!m_graph.IsWindowVisible())
	{
		m_bGraphNeedsRebuildOnShow = TRUE;
		return;
	}

	CWaitCursor cursor;

	m_graph.ClearData();

	const DISPLAYITEM& di = STATSDISPLAY[m_nDisplay];

	m_graph.SetTitle(CEnString(di.nTitleID));
	m_graph.SetYText(CEnString(di.nYAxisID));

	if (m_data.GetCount())
		RebuildXScale();

	DWORD dwTick = GetTickCount();
	
	switch (di.nDisplay)
	{
	case SD_BURNDOWN:
		BuildBurndownGraph();
		break;

	case SD_BURNDOWN2:
		BuildBurndown2Graph();
		break;

	case SD_SPRINT:
		BuildSprintGraph();
		break;
	}

	TRACE(_T("CBurndownWnd::RebuildGraph(%s) took %d ms\n"), CEnString(di.nTitleID), (GetTickCount() - dwTick));

	m_graph.Redraw();
}

void CBurndownWnd::BuildBurndownGraph()
{
	m_graph.SetDatasetStyle(0, HMX_DATASET_STYLE_AREALINE);
	m_graph.SetDatasetPenColor(0, COLOR_GREEN);
	m_graph.SetDatasetMinToZero(0, true);

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

int CBurndownWnd::CalculateIncompleteTaskCount(const COleDateTime& date)
{
	// work thru items until we hit the first task whose 
	// start date > date, counting how many are not complete as we go
	int nNumItems = m_data.GetSize();
	int nNumNotDone = 0;

	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = m_data[nItem];

		if (pSI->dtStart > date)
			break;

		if (!pSI->IsDone() || (pSI->dtDone > date))
			nNumNotDone++;
	}

	return nNumNotDone;
}

void CBurndownWnd::BuildBurndown2Graph()
{
	m_graph.SetDatasetStyle(0, HMX_DATASET_STYLE_AREALINE);
	m_graph.SetDatasetPenColor(0, COLOR_BLUE);
	m_graph.SetDatasetMinToZero(0, true);
	
	// build the graph
	COleDateTime dtStart = GetGraphStartDate();
	COleDateTime dtEnd = GetGraphEndDate();

	int nNumDays = ((int)dtEnd.m_dt - (int)dtStart.m_dt);
	int nFirstIncompleteItem = 0;

	for (int nDay = 0; nDay <= nNumDays; nDay++)
	{
		COleDateTime date(dtStart.m_dt + nDay);
		int nNumNotDone = CalculateIncompleteTaskCount(date, nFirstIncompleteItem, nFirstIncompleteItem);

		m_graph.AddData(0, nNumNotDone);
	}

	m_graph.CalcDatas();
}

int CBurndownWnd::CalculateIncompleteTaskCount(const COleDateTime& date, int nItemFrom, int& nFirstIncompleteItem)
{
	// work thru items until we hit the first task whose 
	// start date > date, counting how many are not complete as we go
	if (m_dtEarliestDate > date)
		return 0;

	nFirstIncompleteItem = -1;

	int nNumItems = m_data.GetSize();
	int nNumNotDone = 0;

	for (int nItem = nItemFrom; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = m_data[nItem];

		if (pSI->dtStart > date)
			break;

		if (!pSI->IsDone() || (pSI->dtDone > date))
		{
			nNumNotDone++;

			if (nFirstIncompleteItem == -1)
				nFirstIncompleteItem = nItem;
		}
	}

	if (nFirstIncompleteItem == -1)
		nFirstIncompleteItem = nItemFrom;

	return nNumNotDone;
}

void CBurndownWnd::BuildSprintGraph()
{
	enum 
	{ 
		SPRINT_EST,
		SPRINT_SPENT
	};

	m_graph.SetDatasetStyle(SPRINT_EST, HMX_DATASET_STYLE_LINE);
	m_graph.SetDatasetPenColor(SPRINT_EST,  COLOR_RED);
	m_graph.SetDatasetMinToZero(SPRINT_EST, true);

	m_graph.SetDatasetStyle(SPRINT_SPENT, HMX_DATASET_STYLE_AREALINE);
	m_graph.SetDatasetPenColor(SPRINT_SPENT, COLOR_YELLOW);
	m_graph.SetDatasetMinToZero(SPRINT_SPENT, true);

	// build the graph
	COleDateTime dtStart = GetGraphStartDate();
	COleDateTime dtEnd = GetGraphEndDate();

	int nNumDays = ((int)dtEnd.m_dt - (int)dtStart.m_dt);

	// Time Estimate
	double dTotalEst = CalcTotalTimeEstimateInDays();

	for (int nDay = 0; nDay <= nNumDays; nDay++)
	{
		double dEst = ((nDay * dTotalEst) / nNumDays);
		m_graph.AddData(SPRINT_EST, (dTotalEst - dEst));
	}

	// Time Spent
	for (int nDay = 0; nDay <= nNumDays; nDay++)
	{
		COleDateTime date(dtStart.m_dt + nDay);
		double dSpent = CalculateTimeSpentInDays(date);

		m_graph.AddData(SPRINT_SPENT, (dTotalEst - dSpent));
	}

	m_graph.CalcDatas();
}

double CBurndownWnd::CalculateTimeSpentInDays(const COleDateTime& date)
{
	int nNumItems = m_data.GetSize();
	double dDays = 0;

	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		const STATSITEM* pSI = m_data[nItem];

		// We can stop as soon as we pass a task's start date
		BOOL bHasStart = pSI->HasStart();

		if (bHasStart && (pSI->dtStart >= date))
			break;
		
		// Ignore tasks with no time spent
		if (pSI->dTimeSpentDays <= 0)
			continue;
		
		if (pSI->IsDone())
		{
			if (date >= pSI->dtDone)
			{
				dDays += pSI->dTimeSpentDays;
			}
			else if (bHasStart && (date > pSI->dtStart))
			{
				double dProportion = (date.m_dt - pSI->dtStart.m_dt) / (pSI->dtDone.m_dt - pSI->dtStart.m_dt);

				dDays += (pSI->dTimeSpentDays * min(dProportion, 1.0));
			}
		}
		else if (bHasStart && (date > pSI->dtStart))
		{
			COleDateTime dtNow(COleDateTime::GetCurrentTime());
			double dProportion = (date.m_dt - pSI->dtStart.m_dt) / (dtNow.m_dt - pSI->dtStart.m_dt);

			dDays += (pSI->dTimeSpentDays * min(dProportion, 1.0));
		}
	}

	return dDays;
}

double CBurndownWnd::CalcTotalTimeEstimateInDays() const
{
	double dDays = 0;
	int nItem = m_data.GetSize();
	
	while (nItem--)
		dDays += m_data.GetAt(nItem)->dTimeEstDays;

	return dDays;
}

int CBurndownWnd::GetDataDuration() const
{
	double dStart = m_dtEarliestDate;
	ASSERT(dStart > 0.0);

	double dEnd = (m_dtLatestDate.m_dt + 1.0);
	ASSERT(dEnd >= dStart);

	return ((int)dEnd - (int)dStart);
}

void CBurndownWnd::OnSelchangeDisplay()
{
	UpdateData();

	RebuildGraph();
}

void CBurndownWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow && m_bGraphNeedsRebuildOnShow)
		PostMessage(WM_REBUILDGRAPH);
}

LRESULT CBurndownWnd::OnRebuildGraph(WPARAM /*wp*/, LPARAM /*lp*/)
{
	ASSERT(m_bGraphNeedsRebuildOnShow);

	m_bGraphNeedsRebuildOnShow = FALSE;
	RebuildGraph();

	return 0L;
}


