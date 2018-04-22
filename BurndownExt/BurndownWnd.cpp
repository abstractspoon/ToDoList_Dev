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

#include "..\3rdparty\dibdata.h"

#include "..\Interfaces\ipreferences.h"

#include <float.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const int    DEF_DAYSINWEEK = 5;
const double DEF_HOURSINDAY = 8.0;

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
	SD_INCOMPLETE,
	SD_REMAINING,

	NUM_DISPLAYTYPES
};

struct DISPLAYITEM
{
	UINT nYAxisID;
	DISPLAYTYPE nDisplay;
};

static DISPLAYITEM STATSDISPLAY[] = 
{
	{ IDS_DISPLAY_INCOMPLETE, SD_INCOMPLETE	},
	{ IDS_DISPLAY_REMAINING, SD_REMAINING },
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
	m_nDisplay(SD_INCOMPLETE),
	m_nScale(1),
	m_dwUpdateGraphOnShow(0),
	m_dHoursInDay(DEF_HOURSINDAY),
	m_nDaysInWeek(DEF_DAYSINWEEK)
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

	m_graph.SetBkGnd(GetSysColor(COLOR_WINDOW));
	m_graph.SetXLabelsAreTicks(true);
	m_graph.SetXLabelAngle(45);
	m_graph.SetYTicks(10);

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

	pPrefs->WriteProfileInt(szKey, _T("GraphType"), m_nDisplay);
}

void CBurndownWnd::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey, bool bAppOnly) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// application preferences
	double dHoursInDay = _ttof(pPrefs->GetProfileString(_T("Preferences"), _T("HoursInDay"), _T("8")));
	int nDaysInWeek = pPrefs->GetProfileInt(_T("Preferences"), _T("DaysInWeek"), 5);

	if ((dHoursInDay != m_dHoursInDay) || (nDaysInWeek != m_nDaysInWeek))
	{
		m_dHoursInDay = dHoursInDay;
		m_nDaysInWeek = nDaysInWeek;
		
		RebuildGraph(FALSE, FALSE, TRUE);
	}
	
	// burn down specific options
	if (!bAppOnly)
	{
		//CString sKey(szKey);

		m_nDisplay = pPrefs->GetProfileInt(szKey, _T("GraphType"), SD_INCOMPLETE);

		if (m_nDisplay > NUM_DISPLAYTYPES)
			m_nDisplay = SD_INCOMPLETE;

		CDialogHelper::SelectItemByData(m_cbDisplay, m_nDisplay);
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

void CBurndownWnd::BuildData(const ITASKLISTBASE* pTasks)
{
	// reset data structures
	m_data.RemoveAll();

	if (pTasks->GetTaskCount())
	{
		CDateHelper::ClearDate(m_dtEarliestDate);
		CDateHelper::ClearDate(m_dtLatestDate);

		BuildData(pTasks, pTasks->GetFirstTask(), TRUE, FALSE);
		RebuildGraph(TRUE, FALSE, FALSE);
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

				// update data extents as we go
				pSI->MinMax(m_dtEarliestDate, m_dtLatestDate);
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

void CBurndownWnd::UpdateDataExtents()
{
	m_data.GetDataExtents(m_dtEarliestDate, m_dtLatestDate);
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

		int nOldScale = m_nScale;
		RebuildXScale();
		
		// handle scale change
		if (m_nScale != nOldScale)
			RebuildGraph(FALSE, FALSE, TRUE);
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
	if (m_nDisplay == SD_REMAINING)
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
	if (m_nDisplay == SD_REMAINING)
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

void CBurndownWnd::RebuildGraph(BOOL bSortData, BOOL bUpdateExtents, BOOL bCheckVisibility)
{
	if (bCheckVisibility && !m_graph.IsWindowVisible())
	{
		m_dwUpdateGraphOnShow = REBUILD_GRAPH;
		m_dwUpdateGraphOnShow |= (bSortData ? RESORT_DATA : 0);
		m_dwUpdateGraphOnShow |= (bUpdateExtents ? UPDATE_EXTENTS : 0);
		
		return;
	}

	CWaitCursor cursor;
	CHoldRedraw hr(m_graph);

	if (bUpdateExtents)
		UpdateDataExtents();

	if (bSortData)
		m_data.Sort();

	m_graph.ClearData();
	m_graph.SetYText(CEnString(STATSDISPLAY[m_nDisplay].nYAxisID));

	if (!m_data.IsEmpty())
		RebuildXScale();

	DWORD dwTick = GetTickCount();
	
	switch (STATSDISPLAY[m_nDisplay].nDisplay)
	{
	case SD_INCOMPLETE:
		BuildBurndownGraph();
		break;

	case SD_REMAINING:
		BuildSprintGraph();
		break;
	}
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
	int nItemFrom = 0;

	for (int nDay = 0; nDay <= nNumDays; nDay++)
	{
		COleDateTime date(dtStart.m_dt + nDay);

		if (m_dtEarliestDate > date)
		{
			m_graph.AddData(0, 0);
		}
		else
		{
			int nNumNotDone = m_data.CalculateIncompleteTaskCount(date, nItemFrom, nItemFrom);
			m_graph.AddData(0, nNumNotDone);
		}
	}

	m_graph.CalcDatas();
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

	double dTotalEst = m_data.CalcTotalTimeEstimateInDays(m_nDaysInWeek, m_dHoursInDay);

	int nNumDays = ((int)dtEnd.m_dt - (int)dtStart.m_dt);

	for (int nDay = 0; nDay <= nNumDays; nDay++)
	{
		// Time Estimate
		double dEst = ((nDay * dTotalEst) / nNumDays);
		m_graph.AddData(SPRINT_EST, (dTotalEst - dEst));

		// Time Spent
		COleDateTime date(dtStart.m_dt + nDay);
		double dSpent = m_data.CalcTimeSpentInDays(date, m_nDaysInWeek, m_dHoursInDay);
		
		m_graph.AddData(SPRINT_SPENT, (dTotalEst - dSpent));
	}

	m_graph.CalcDatas();
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
	int nPrevDisplay = m_nDisplay;
	UpdateData();

	if (m_nDisplay != nPrevDisplay)
		RebuildGraph(FALSE, FALSE, FALSE);
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

