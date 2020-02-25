// GanttChartWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "GanttChartExt.h"
#include "GanttChartWnd.h"
#include "GanttStatic.h"
#include "GanttMsg.h"

#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\themed.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\datehelper.h"
#include "..\shared\timehelper.h"
#include "..\shared\localizer.h"
#include "..\shared\autoflag.h"
#include "..\shared\holdredraw.h"
#include "..\shared\dlgunits.h"
#include "..\shared\WorkingWeek.h"

#include "..\3rdparty\T64Utils.h"
#include "..\3rdparty\dibdata.h"
#include "..\3rdparty\GdiPlus.h"

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

CGanttChartWnd::CGanttChartWnd(CWnd* pParent /*=NULL*/)
	: 
	CDialog(IDD_GANTTTREE_DIALOG, pParent), 
	m_bReadOnly(FALSE),
	m_bInSelectTask(FALSE),
#pragma warning(disable:4355)
	m_dlgPrefs(this)
#pragma warning(default:4355)
{
	m_icon.Load(IDR_GANTTCHART);
}

CGanttChartWnd::~CGanttChartWnd()
{
}

void CGanttChartWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGanttChartWnd)
	DDX_Control(pDX, IDC_SNAPMODES, m_cbSnapModes);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DISPLAY, m_cbDisplayOptions);
	DDX_Control(pDX, IDC_ACTIVEDATERANGE, m_sliderDateRange);
}

BEGIN_MESSAGE_MAP(CGanttChartWnd, CDialog)
	//{{AFX_MSG_MAP(CGanttChartWnd)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TVN_KEYUP, IDC_GANTTTREE, OnKeyUpGantt)
	ON_CBN_SELCHANGE(IDC_DISPLAY, OnSelchangeDisplay)
	ON_NOTIFY(NM_CLICK, IDC_GANTTLIST, OnClickGanttList)
	ON_COMMAND(ID_GANTT_GOTOTODAY, OnGanttGotoToday)
	ON_UPDATE_COMMAND_UI(ID_GANTT_GOTOTODAY, OnUpdateGanttGotoToday)
	ON_COMMAND(ID_GANTT_NEWDEPENDS, OnGanttNewDepends)
	ON_UPDATE_COMMAND_UI(ID_GANTT_NEWDEPENDS, OnUpdateGanttNewDepends)
	ON_COMMAND(ID_GANTT_PREFS, OnGanttPreferences)
	ON_UPDATE_COMMAND_UI(ID_GANTT_PREFS, OnUpdateGanttPreferences)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_GANTT_EDITDEPENDS, OnGanttEditDepends)
	ON_UPDATE_COMMAND_UI(ID_GANTT_EDITDEPENDS, OnUpdateGanttEditDepends)
	ON_COMMAND(ID_GANTT_DELETEDEPENDS, OnGanttDeleteDepends)
	ON_UPDATE_COMMAND_UI(ID_GANTT_DELETEDEPENDS, OnUpdateGanttDeleteDepends)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, OnHelp)
	ON_WM_HELPINFO()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_NCDESTROY()
	ON_WM_LBUTTONDBLCLK()

	ON_REGISTERED_MESSAGE(WM_GTLC_EDITTASKTITLE, OnGanttEditTaskTitle)
	ON_REGISTERED_MESSAGE(WM_GTLC_DATECHANGE, OnGanttNotifyDateChange)
	ON_REGISTERED_MESSAGE(WM_GTLC_DRAGCHANGE, OnGanttNotifyDragChange)
	ON_REGISTERED_MESSAGE(WM_GTLC_COMPLETIONCHANGE, OnGanttNotifyCompletionChange)
	ON_REGISTERED_MESSAGE(WM_GTLC_NOTIFYSORT, OnGanttNotifySortChange)
	ON_REGISTERED_MESSAGE(WM_GTLC_NOTIFYZOOM, OnGanttNotifyZoomChange)
	ON_REGISTERED_MESSAGE(WM_GTLC_PREFSHELP, OnGanttPrefsHelp)
	ON_REGISTERED_MESSAGE(WM_GTLC_GETTASKICON, OnGanttGetTaskIcon)
	ON_REGISTERED_MESSAGE(WM_GTLC_EDITTASKICON, OnGanttEditTaskIcon)
	ON_REGISTERED_MESSAGE(WM_GTLC_MOVETASK, OnGanttMoveTask)
	ON_REGISTERED_MESSAGE(WM_TLC_ITEMSELCHANGE, OnGanttNotifySelChanged)

	ON_REGISTERED_MESSAGE(WM_GANTTDEPENDDLG_CLOSE, OnGanttDependencyDlgClose)
	ON_REGISTERED_MESSAGE(RANGE_CHANGED, OnActiveDateRangeChange)
	ON_CBN_SELCHANGE(IDC_SNAPMODES, OnSelchangeSnapMode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGanttChartWnd message handlers

void CGanttChartWnd::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	
	delete this;
}

void CGanttChartWnd::OnHelp()
{
	GetParent()->SendMessage(WM_IUI_DOHELP, 0, (LPARAM)GetTypeID());
}

BOOL CGanttChartWnd::OnHelpInfo(HELPINFO* /*lpHelpInfo*/)
{
	OnHelp();
	return TRUE;
}

LRESULT CGanttChartWnd::OnGanttPrefsHelp(WPARAM /*wp*/, LPARAM /*lp*/)
{
	CString sHelpID(GetTypeID());
	sHelpID += _T("_PREFS");
	
	GetParent()->SendMessage(WM_IUI_DOHELP, 0, (LPARAM)(LPCTSTR)sHelpID);
	return 0L;
}

void CGanttChartWnd::SetReadOnly(bool bReadOnly)
{
	m_bReadOnly = bReadOnly;
	m_ctrlGantt.SetReadOnly(bReadOnly);

	m_toolbar.RefreshButtonStates(FALSE);
}

BOOL CGanttChartWnd::Create(DWORD dwStyle, const RECT &/*rect*/, CWnd* pParentWnd, UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (CDialog::Create(IDD_GANTTTREE_DIALOG, pParentWnd))
	{
		SetWindowLong(*this, GWL_STYLE, dwStyle);
		SetDlgCtrlID(nID);

		return TRUE;
	}

	return FALSE;
}

void CGanttChartWnd::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// NOTE: sort is handled by the app
	CString sKey(szKey);

	pPrefs->WriteProfileInt(sKey, _T("MonthDisplay"), m_ctrlGantt.GetMonthDisplay());
	pPrefs->WriteProfileInt(sKey, _T("SortColumn"), m_ctrlGantt.GetSortColumn());
	pPrefs->WriteProfileInt(sKey, _T("SortAscending"), m_ctrlGantt.GetSortAscending());

	// snap modes
	CString sSnapKey = sKey + _T("\\Snap");

	SaveSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_QUARTERCENTURIES, _T("QuarterCentury"));
	SaveSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_DECADES, _T("Decade"));
	SaveSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_YEARS, _T("Year"));
	SaveSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_QUARTERS_SHORT, _T("QuarterShort"));
	SaveSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_QUARTERS_MID, _T("QuarterMid"));
	SaveSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_QUARTERS_LONG, _T("QuarterLong"));
	SaveSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_MONTHS_SHORT, _T("MonthShort"));
	SaveSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_MONTHS_MID, _T("MonthMid"));
	SaveSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_MONTHS_LONG, _T("MonthLong"));
	SaveSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_WEEKS_SHORT, _T("WeekShort"));
	SaveSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_WEEKS_MID, _T("WeekMid"));
	SaveSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_WEEKS_LONG, _T("WeekLong"));
	SaveSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_DAYS_SHORT, _T("DayShort"));
	SaveSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_DAYS_MID, _T("DayMid"));
	SaveSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_DAYS_LONG, _T("DayLong"));
	SaveSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_HOURS, _T("Hour"));

	// column widths
	CIntArray aTreeOrder, aTreeWidths, aListWidths, aTreeTracked, aListTracked;

	m_ctrlGantt.GetTreeColumnOrder(aTreeOrder);
	m_ctrlGantt.GetColumnWidths(aTreeWidths, aListWidths);
	m_ctrlGantt.GetTrackedColumns(aTreeTracked, aListTracked);

	SaveColumnState(pPrefs, (sKey + _T("\\TreeOrder")), aTreeOrder);
	SaveColumnState(pPrefs, (sKey + _T("\\TreeWidths")), aTreeWidths);
	SaveColumnState(pPrefs, (sKey + _T("\\ListWidths")), aListWidths);
	SaveColumnState(pPrefs, (sKey + _T("\\TreeTracked")), aTreeTracked);
	SaveColumnState(pPrefs, (sKey + _T("\\ListTracked")), aListTracked);

	// Active date range
	GANTTDATERANGE dtRange;

	if (m_sliderDateRange.GetSelectedRange(dtRange))
	{
		pPrefs->WriteProfileDouble(_T("ActiveRange"), _T("Start"), dtRange.GetStart().m_dt);
		pPrefs->WriteProfileDouble(_T("ActiveRange"), _T("End"), dtRange.GetEnd().m_dt);
	}
	else
	{
		pPrefs->DeleteProfileSection(_T("ActiveRange"));
	}

	m_dlgPrefs.SavePreferences(pPrefs, sKey);
}

void CGanttChartWnd::SaveColumnState(IPreferences* pPrefs, LPCTSTR szKey, const CIntArray& aStates) const
{
	int nCol = aStates.GetSize();
	pPrefs->WriteProfileInt(szKey, _T("Count"), nCol);

	while (nCol--)
	{
		CString sItemKey;
		sItemKey.Format(_T("Item%d"), nCol);
		pPrefs->WriteProfileInt(szKey, sItemKey, aStates[nCol]);
	}
}

int CGanttChartWnd::LoadColumnState(const IPreferences* pPrefs, LPCTSTR szKey, CIntArray& aStates) const
{
	int nCol = pPrefs->GetProfileInt(szKey, _T("Count"), 0);

	if (!nCol)
		return 0;

	aStates.SetSize(nCol);
	
	while (nCol--)
	{
		CString sItemKey;
		sItemKey.Format(_T("Item%d"), nCol);
		aStates[nCol] = pPrefs->GetProfileInt(szKey, sItemKey);
	}

	return aStates.GetSize();
}

void CGanttChartWnd::InitWorkingWeek(const IPreferences* pPrefs)
{
	DWORD dwWeekends = pPrefs->GetProfileInt(_T("Preferences"), _T("Weekends"), (DHW_SATURDAY | DHW_SUNDAY));

	double dHoursInDay = pPrefs->GetProfileDouble(_T("Preferences"), _T("HoursInDay"), 8.0);
	double dStartOfDayInHours = pPrefs->GetProfileDouble(_T("Preferences"), _T("StartOfWorkdayInHours"), 9.0);
	double dStartOfLunchInHours = pPrefs->GetProfileDouble(_T("Preferences"), _T("StartOfLunchInHours"), 13.0);
	double dEndOfLunchInHours = dStartOfLunchInHours;

	if (pPrefs->GetProfileInt(_T("Preferences"), _T("HasLunchBreak"), TRUE))
		dEndOfLunchInHours = pPrefs->GetProfileDouble(_T("Preferences"), _T("EndOfLunchInHours"), 14.0);

	CWorkingWeek::Initialise(dwWeekends, dHoursInDay, dStartOfDayInHours, dStartOfLunchInHours, dEndOfLunchInHours);
}

void CGanttChartWnd::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey, bool bAppOnly) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// application preferences
	InitWorkingWeek(pPrefs);

	m_ctrlGantt.SetOption(GTLCF_TASKTEXTCOLORISBKGND, pPrefs->GetProfileInt(_T("Preferences"), _T("ColorTaskBackground"), FALSE));
	m_ctrlGantt.SetOption(GTLCF_TREATSUBCOMPLETEDASDONE, pPrefs->GetProfileInt(_T("Preferences"), _T("TreatSubCompletedAsDone"), FALSE));
	m_ctrlGantt.SetOption(GTLCF_STRIKETHRUDONETASKS, pPrefs->GetProfileInt(_T("Preferences"), _T("StrikethroughDone"), TRUE));
	m_ctrlGantt.SetOption(GTLCF_DISABLEDEPENDENTDRAGGING, pPrefs->GetProfileInt(_T("Preferences"), _T("AutoAdjustDependents"), TRUE));
	m_ctrlGantt.SetOption(GTLCF_DISPLAYISODATES, pPrefs->GetProfileInt(_T("Preferences"), _T("DisplayDatesInISO"), FALSE));
	m_ctrlGantt.SetOption(GTLCF_SHOWSPLITTERBAR, (pPrefs->GetProfileInt(_T("Preferences"), _T("HidePaneSplitBar"), TRUE) == FALSE));

	m_ctrlGantt.EnableTreeCheckboxes(IDB_CHECKBOXES, pPrefs->GetProfileInt(_T("Preferences"), _T("AllowCheckboxAgainstTreeItem"), TRUE));
	m_ctrlGantt.EnableTreeLabelTips(!pPrefs->GetProfileInt(_T("Preferences"), _T("ShowInfoTips"), FALSE));

	// get alternate line color from app prefs
	COLORREF crAlt = CLR_NONE;

	if (pPrefs->GetProfileInt(_T("Preferences"), _T("AlternateLineColor"), TRUE))
		crAlt = pPrefs->GetProfileInt(_T("Preferences\\Colors"), _T("AlternateLines"), DEF_ALTLINECOLOR);

	m_ctrlGantt.SetAlternateLineColor(crAlt);

	// get grid line color from app prefs
	COLORREF crGrid = CLR_NONE;

	if (pPrefs->GetProfileInt(_T("Preferences"), _T("SpecifyGridColor"), TRUE))
		crGrid = pPrefs->GetProfileInt(_T("Preferences\\Colors"), _T("GridLines"), DEF_GRIDLINECOLOR);

	m_ctrlGantt.SetGridLineColor(crGrid);
	
	// gantt specific options
	if (!bAppOnly)
	{
		// NOTE: sort is handled by the app
		CString sKey(szKey);
		
		// snap modes
		CString sSnapKey = sKey + _T("\\Snap");

		LoadSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_QUARTERCENTURIES, _T("QuarterCentury"), GTLCSM_NEARESTYEAR);
		LoadSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_DECADES, _T("Decade"), GTLCSM_NEARESTYEAR);
		LoadSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_YEARS, _T("Year"), GTLCSM_NEARESTMONTH);
		LoadSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_QUARTERS_SHORT, _T("QuarterShort"), GTLCSM_NEARESTMONTH);
		LoadSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_QUARTERS_MID, _T("QuarterMid"), GTLCSM_NEARESTMONTH);
		LoadSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_QUARTERS_LONG, _T("QuarterLong"), GTLCSM_NEARESTMONTH);
		LoadSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_MONTHS_SHORT, _T("MonthShort"), GTLCSM_NEARESTDAY);
		LoadSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_MONTHS_MID, _T("MonthMid"), GTLCSM_NEARESTDAY);
		LoadSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_MONTHS_LONG, _T("MonthLong"), GTLCSM_NEARESTDAY);
		LoadSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_WEEKS_SHORT, _T("WeekShort"), GTLCSM_NEARESTDAY);
		LoadSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_WEEKS_MID, _T("WeekMid"), GTLCSM_NEARESTDAY);
		LoadSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_WEEKS_LONG, _T("WeekLong"), GTLCSM_NEARESTDAY);
		LoadSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_DAYS_SHORT, _T("DayShort"), GTLCSM_NEARESTHOUR);
		LoadSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_DAYS_MID, _T("DayMid"), GTLCSM_NEARESTHOUR);
		LoadSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_DAYS_LONG, _T("DayLong"), GTLCSM_NEARESTHOUR);
		LoadSnapModePreference(pPrefs, sSnapKey, GTLC_DISPLAY_HOURS, _T("Hour"), GTLCSM_NEARESTHOUR);

		// last display
		GTLC_MONTH_DISPLAY nDisplay = (GTLC_MONTH_DISPLAY)pPrefs->GetProfileInt(sKey, _T("MonthDisplay"), GTLC_DISPLAY_MONTHS_LONG);
		SetMonthDisplay(nDisplay);

		m_dlgPrefs.LoadPreferences(pPrefs, sKey);
		UpdateGanttCtrlPreferences();
		
		// column order
		CIntArray aTreeOrder, aTreeWidths, aListWidths, aTreeTracked, aListTracked;

		if (LoadColumnState(pPrefs, (sKey + _T("\\TreeOrder")), aTreeOrder))
		{
			m_ctrlGantt.SetTreeColumnOrder(aTreeOrder);
		}
		
		// column widths
		if (LoadColumnState(pPrefs, (sKey + _T("\\TreeWidths")), aTreeWidths) &&
			LoadColumnState(pPrefs, (sKey + _T("\\ListWidths")), aListWidths) &&
			m_ctrlGantt.SetColumnWidths(aTreeWidths, aListWidths))
		{
			m_ctrlGantt.AdjustSplitterToFitTreeColumns();
		}
		else
		{
			m_ctrlGantt.ResizeListColumnsToFit();
		}
		
		// column tracking
		if (LoadColumnState(pPrefs, (sKey + _T("\\TreeTracked")), aTreeTracked) &&
			LoadColumnState(pPrefs, (sKey + _T("\\ListTracked")), aListTracked))
		{
			m_ctrlGantt.SetTrackedColumns(aTreeTracked, aListTracked);
		}

		// Active range
		m_dtPrevActiveRange.Reset();

		double dStart = pPrefs->GetProfileDouble(_T("ActiveRange"), _T("Start"), 0);
		double dEnd = pPrefs->GetProfileDouble(_T("ActiveRange"), _T("End"), -1);

		if (dEnd > dStart)
		{
			m_dtPrevActiveRange.SetStart(dStart);
			m_dtPrevActiveRange.SetEnd(dEnd);
		}
		
		if (GetSafeHwnd())
			UpdateData(FALSE);
	}
}

void CGanttChartWnd::LoadSnapModePreference(const IPreferences* pPrefs, LPCTSTR szSnapKey, GTLC_MONTH_DISPLAY nDisplay, LPCTSTR szDisplay, GTLC_SNAPMODE nDefaultSnap) 
{
	m_mapDisplaySnapModes[nDisplay]	= (GTLC_SNAPMODE)pPrefs->GetProfileInt(szSnapKey, szDisplay, nDefaultSnap);
}

void CGanttChartWnd::SaveSnapModePreference(IPreferences* pPrefs, LPCTSTR szSnapKey, GTLC_MONTH_DISPLAY nDisplay, LPCTSTR szDisplay) const
{
	GTLC_SNAPMODE nSnap = GTLCSM_FREE;

	VERIFY (m_mapDisplaySnapModes.Lookup(nDisplay, nSnap));
	pPrefs->WriteProfileInt(szSnapKey, szDisplay, nSnap);
}

void CGanttChartWnd::SetUITheme(const UITHEME* pTheme)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	GraphicsMisc::VerifyDeleteObject(m_brBack);
	
	if (CThemed::IsAppThemed() && pTheme)
	{
		m_theme = *pTheme;
		m_brBack.CreateSolidBrush(m_theme.crAppBackLight);

		// intentionally set background colours to be same as ours
		m_toolbar.SetBackgroundColors(m_theme.crAppBackLight, m_theme.crAppBackLight, FALSE, FALSE);
		m_toolbar.SetHotColor(m_theme.crToolbarHot);

		m_ctrlGantt.SetSplitBarColor(m_theme.crAppBackDark);
		m_sliderDateRange.SetParentBackgroundColor(m_theme.crAppBackLight);
	}
}

void CGanttChartWnd::FilterToolTipMessage(MSG* pMsg) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_ctrlGantt.FilterToolTipMessage(pMsg);
}

bool CGanttChartWnd::ProcessMessage(MSG* pMsg) 
{
	if (!IsWindowEnabled())
		return false;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_ctrlGantt.ProcessMessage(pMsg))
		return true;

	// toolbar messages
	return (m_tbHelper.ProcessMessage(pMsg) != FALSE);
}

bool CGanttChartWnd::PrepareNewTask(ITaskList* pTask) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return m_ctrlGantt.PrepareNewTask(pTask);
}

bool CGanttChartWnd::GetLabelEditRect(LPRECT pEdit)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (m_ctrlGantt.GetLabelEditRect(pEdit))
	{
		// convert to screen coords
		m_ctrlGantt.CWnd::ClientToScreen(pEdit);
		return true;
	}

	return false;
}

IUI_HITTEST CGanttChartWnd::HitTest(POINT ptScreen) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// try tree header
	if (m_ctrlGantt.PointInHeader(ptScreen))
		return IUI_NOWHERE;//IUI_COLUMNHEADER;

	// then specific task
	if (m_ctrlGantt.HitTestTask(ptScreen, false))
		return IUI_TASK;

	// else 
	return IUI_NOWHERE;
}

DWORD CGanttChartWnd::HitTestTask(POINT ptScreen, bool bTitleColumnOnly) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_ctrlGantt.HitTestTask(ptScreen, bTitleColumnOnly);
}

bool CGanttChartWnd::SelectTask(DWORD dwTaskID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CAutoFlag af(m_bInSelectTask, TRUE);

	return (m_ctrlGantt.SelectTask(dwTaskID) != FALSE);
}

bool CGanttChartWnd::SelectTasks(const DWORD* /*pdwTaskIDs*/, int /*nTaskCount*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return false; // only support single selection
}

bool CGanttChartWnd::WantTaskUpdate(TDC_ATTRIBUTE nAttribute) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (CGanttCtrl::WantEditUpdate(nAttribute) != FALSE);
}

void CGanttChartWnd::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_ctrlGantt.UpdateTasks(pTasks, nUpdate);

	GANTTDATERANGE dtDataRange;

	if (!m_ctrlGantt.GetDataDateRange(dtDataRange))
	{
		ASSERT(!m_ctrlGantt.GetTaskCount());

		m_sliderDateRange.EnableWindow(FALSE);
		return;
	}

	GTLC_MONTH_DISPLAY nDisplay = m_ctrlGantt.GetMonthDisplay();

	m_sliderDateRange.SetMonthDisplay(nDisplay);
	m_sliderDateRange.SetDataRange(dtDataRange);
	m_sliderDateRange.EnableWindow(TRUE);

	if (m_dtPrevActiveRange.IsValid())
	{
		if (m_sliderDateRange.SetSelectedRange(m_dtPrevActiveRange))
			m_ctrlGantt.SetActiveDateRange(m_dtPrevActiveRange);

		m_dtPrevActiveRange.Reset();
	}
		
	// Month Display may change for large date ranges
	BuildDisplayCombo();
	UpdateActiveRangeLabel();
}

bool CGanttChartWnd::DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData) 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (nCmd)
	{
	case IUI_EXPANDALL:
		m_ctrlGantt.ExpandAll(TRUE);
		return true;

	case IUI_COLLAPSEALL:
		m_ctrlGantt.ExpandAll(FALSE);
		return true;

	case IUI_EXPANDSELECTED:
		m_ctrlGantt.ExpandItem(m_ctrlGantt.GetSelectedItem(), TRUE, TRUE);
		return true;

	case IUI_COLLAPSESELECTED:
		m_ctrlGantt.ExpandItem(m_ctrlGantt.GetSelectedItem(), FALSE);
		return true;

	case IUI_SORT:
		if (pData)
		{
			m_ctrlGantt.Sort(MapColumn(pData->nSortBy), (pData->bSortAscending ? TRUE : FALSE));
			return true;
		}
		break;

	case IUI_MULTISORT:
		if (pData)
		{
			GANTTSORTCOLUMNS sort;

			sort.cols[0].nBy = m_ctrlGantt.MapAttributeToColumn(pData->sort.nAttrib1);
			sort.cols[0].bAscending = (pData->sort.bAscending1 ? TRUE : FALSE);

			sort.cols[1].nBy = m_ctrlGantt.MapAttributeToColumn(pData->sort.nAttrib2);
			sort.cols[1].bAscending = (pData->sort.bAscending2 ? TRUE : FALSE);

			sort.cols[2].nBy = m_ctrlGantt.MapAttributeToColumn(pData->sort.nAttrib3);
			sort.cols[2].bAscending = (pData->sort.bAscending3 ? TRUE : FALSE);
			
			m_ctrlGantt.Sort(sort);
			return true;
		}
		break;

	case IUI_SETFOCUS:
		m_ctrlGantt.SetFocus();
		return true;

	case IUI_RESIZEATTRIBCOLUMNS:
		m_ctrlGantt.ResizeListColumnsToFit(TRUE);
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
			DWORD dwNextID = m_ctrlGantt.GetNextTask(pData->dwTaskID, nCmd);
			
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

			if (m_ctrlGantt.SaveToImage(bmImage))
				return (CGdiPlusBitmap(bmImage).SaveAsFile(pData->szFilePath) != FALSE);
		}
		break;

	case IUI_SELECTFIRSTTASK:
	case IUI_SELECTNEXTTASK:
	case IUI_SELECTNEXTTASKINCLCURRENT:
	case IUI_SELECTPREVTASK:
	case IUI_SELECTLASTTASK:
		if (pData)
			return (m_ctrlGantt.SelectTask(nCmd, pData->select) != FALSE);
		break;

		return true;

	case IUI_MOVETASK:
		if (pData)
		{
			ASSERT(pData->move.dwSelectedTaskID == m_ctrlGantt.GetSelectedTaskID());

			return (m_ctrlGantt.MoveSelectedTask(pData->move) != FALSE);
		}
		break;

	}

	return false;
}

void CGanttChartWnd::SetTaskFont(HFONT hFont)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHoldRedraw hr(*this);
	
	m_ctrlGantt.SetFont(hFont, TRUE);
}

bool CGanttChartWnd::CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData) const 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	switch (nCmd)
	{
	case IUI_EXPANDALL:
		return (m_ctrlGantt.CanExpandAll() != FALSE);

	case IUI_COLLAPSEALL:
		return (m_ctrlGantt.CanCollapseAll() != FALSE);

	case IUI_RESIZEATTRIBCOLUMNS:
	case IUI_SELECTTASK:
		return true;

	case IUI_SAVETOIMAGE:
		return (m_ctrlGantt.GetTaskCount() > 0);

	case IUI_EXPANDSELECTED:
		{
			HTREEITEM htiSel = m_ctrlGantt.GetSelectedItem();
			return (m_ctrlGantt.CanExpandItem(htiSel, TRUE) != FALSE);
		}
		break;

	case IUI_COLLAPSESELECTED:
		{
			HTREEITEM htiSel = m_ctrlGantt.GetSelectedItem();
			return (m_ctrlGantt.CanExpandItem(htiSel, FALSE) != FALSE);
		}
		break;

	case IUI_SORT:
		if (pData)
			return (CGanttCtrl::WantSortUpdate(pData->nSortBy) != FALSE);
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
			DWORD dwNextID =  m_ctrlGantt.GetNextTask(pData->dwTaskID, nCmd);

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
			return (m_ctrlGantt.CanMoveSelectedTask(pData->move) != FALSE);
		break;
	}

	// all else
	return false;
}

GTLC_COLUMN CGanttChartWnd::MapColumn(DWORD dwColumn)
{
	return CGanttCtrl::MapAttributeToColumn((TDC_ATTRIBUTE)dwColumn);
}

DWORD CGanttChartWnd::MapColumn(GTLC_COLUMN nColumn)
{
	return (DWORD)CGanttCtrl::MapColumnToAttribute(nColumn);
}

void CGanttChartWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	Resize(cx, cy);
}

BOOL CGanttChartWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// non-translatables
	CLocalizer::EnableTranslation(*GetDlgItem(IDC_SELECTEDTASKDATES), FALSE);

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
		
	CRect rCtrl = CDialogHelper::GetCtrlRect(this, IDC_GANTTCHART_FRAME);
	VERIFY(m_ctrlGantt.Create(this, rCtrl, IDC_GANTTCTRL));

	BuildSnapCombo();
	BuildDisplayCombo();
	
	m_ctrlGantt.ScrollToToday();
	m_ctrlGantt.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGanttChartWnd::Resize(int cx, int cy)
{
	if (m_ctrlGantt.GetSafeHwnd())
	{
		CRect rGantt(0, 0, cx, cy);
		rGantt.top = CDlgUnits(this).ToPixelsY(28);

		m_ctrlGantt.MoveWindow(rGantt);

		// selected task dates takes available space
		CRect rSlider = CDialogHelper::GetChildRect(&m_sliderDateRange);
		rSlider.right = (cx - 10);

		m_sliderDateRange.MoveWindow(rSlider);
	}
}

HBRUSH CGanttChartWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CGanttChartWnd::OnEraseBkgnd(CDC* pDC) 
{
	// clip out our children
	CDialogHelper::ExcludeChild(&m_toolbar, pDC);
	CDialogHelper::ExcludeChild(&m_ctrlGantt, pDC);

	CDialogHelper::ExcludeCtrl(this, IDC_ACTIVEDATERANGE_LABEL, pDC);
	CDialogHelper::ExcludeCtrl(this, IDC_ACTIVEDATERANGE, pDC);
	CDialogHelper::ExcludeCtrl(this, IDC_SNAPMODES_LABEL, pDC);
	CDialogHelper::ExcludeCtrl(this, IDC_SNAPMODES, pDC);
	CDialogHelper::ExcludeCtrl(this, IDC_DISPLAY_LABEL, pDC);
	CDialogHelper::ExcludeCtrl(this, IDC_DISPLAY, pDC);

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

void CGanttChartWnd::OnKeyUpGantt(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ASSERT (!m_bInSelectTask);
	
	NMTVKEYDOWN* pTVKD = (NMTVKEYDOWN*)pNMHDR;
	
	switch (pTVKD->wVKey)
	{
	case VK_UP:
	case VK_DOWN:
	case VK_PRIOR:
	case VK_NEXT:
		SendParentSelectionUpdate();
		break;
	}
	
	*pResult = 0;
}

void CGanttChartWnd::SendParentSelectionUpdate()
{
	DWORD dwTaskID = m_ctrlGantt.GetSelectedTaskID();
	GetParent()->SendMessage(WM_IUI_SELECTTASK, 0, dwTaskID);
}

void CGanttChartWnd::OnClickGanttList(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	SendParentSelectionUpdate();

	*pResult = 0;
}

BOOL CGanttChartWnd::SetMonthDisplay(GTLC_MONTH_DISPLAY nDisplay)
{
	m_ctrlGantt.ValidateMonthDisplay(nDisplay);

	// Cache and clear previous range selection
	GANTTDATERANGE dtActive;

	if (m_sliderDateRange.IsValid())
		m_sliderDateRange.GetSelectedRange(dtActive);

	GTLC_MONTH_DISPLAY nPrevDisplay = m_ctrlGantt.GetMonthDisplay();

	CHoldRedraw hr(*this);

	m_ctrlGantt.ClearActiveDateRange();
	m_sliderDateRange.ClearSelectedRange();

	if (m_ctrlGantt.SetMonthDisplay(nDisplay))
	{
		CDialogHelper::SelectItemByData(m_cbDisplayOptions, nDisplay);

		BuildSnapCombo();
		
		GTLC_SNAPMODE nSnap = GTLCSM_FREE;
		VERIFY(m_mapDisplaySnapModes.Lookup(nDisplay, nSnap));

		m_ctrlGantt.SetSnapMode(nSnap);
		CDialogHelper::SelectItemByData(m_cbSnapModes, nSnap);

		// Resync range slider
		if (m_ctrlGantt.HasDateRange())
		{
			m_sliderDateRange.SetMonthDisplay(nDisplay);
			m_sliderDateRange.EnableWindow(TRUE);

			// Restore previous active selection
			if (dtActive.IsValid())
			{
				// We only need to fixup the active selection if the primary 
				// display group is changing else the column count is unchanged 
				// so the previous selection is okay by default
				if (!GanttStatic::IsSameDisplayGroup(nPrevDisplay, nDisplay))
				{
					GANTTDATERANGE dtMaxRange;

					if (m_ctrlGantt.GetMaxDateRange(dtMaxRange) && dtActive.IntersectWith(dtMaxRange))
						dtActive.Set(dtActive, nDisplay, m_dlgPrefs.GetDecadesAreZeroBased());
					else
						dtActive.Reset();
				}

				if (dtActive.IsValid())
				{
					m_ctrlGantt.SetActiveDateRange(dtActive);
					m_sliderDateRange.SetSelectedRange(dtActive);
				}
			}

			UpdateActiveRangeLabel();
		}
		else
		{
			m_sliderDateRange.EnableWindow(FALSE);
		}

		m_ctrlGantt.Invalidate(FALSE); // because of the CHoldRedraw

		return TRUE;
	}

	// else
	return FALSE;
}

void CGanttChartWnd::OnSelchangeDisplay() 
{
	GTLC_MONTH_DISPLAY nCurDisplay = m_ctrlGantt.GetMonthDisplay();
	GTLC_MONTH_DISPLAY nNewDisplay = CDialogHelper::GetSelectedItemData(m_cbDisplayOptions, GTLC_DISPLAY_NONE);

	if (!SetMonthDisplay(nNewDisplay))
	{
		// restore previous selection
		CDialogHelper::SelectItemByData(m_cbDisplayOptions, nCurDisplay);
	}
}

void CGanttChartWnd::OnSelchangeSnapMode() 
{
	// save snap mode as we go
	GTLC_MONTH_DISPLAY nCurDisplay = m_ctrlGantt.GetMonthDisplay();
	GTLC_SNAPMODE nSnap = CDialogHelper::GetSelectedItemData(m_cbSnapModes, GTLCSM_NONE);

	m_mapDisplaySnapModes[nCurDisplay] = nSnap;
	m_ctrlGantt.SetSnapMode(nSnap);
}

LRESULT CGanttChartWnd::OnGanttNotifyZoomChange(WPARAM wp, LPARAM lp)
{
	UNREFERENCED_PARAMETER(wp);
	ASSERT((GTLC_MONTH_DISPLAY)wp != (GTLC_MONTH_DISPLAY)lp);

	// Update display combo selection
	GTLC_MONTH_DISPLAY nDisplay = (GTLC_MONTH_DISPLAY)lp;
	CDialogHelper::SelectItemByData(m_cbDisplayOptions, nDisplay);

	// Rebuild snap combo
	BuildSnapCombo();

	GTLC_SNAPMODE nSnap = GTLCSM_FREE;
	VERIFY(m_mapDisplaySnapModes.Lookup(nDisplay, nSnap));

	m_ctrlGantt.SetSnapMode(nSnap);
	CDialogHelper::SelectItemByData(m_cbSnapModes, nSnap);

	// Update slider active range
	GANTTDATERANGE dtActive;

	if (m_sliderDateRange.IsValid())
		m_sliderDateRange.GetSelectedRange(dtActive);

	GTLC_MONTH_DISPLAY nPrevDisplay = m_sliderDateRange.GetMonthDisplay();
	m_sliderDateRange.SetMonthDisplay(nDisplay);

	if (dtActive.IsValid())
	{
		// We only need to fixup the active selection if the primary 
		// display group is changing else the column count is unchanged 
		// so the previous selection is okay by default
		if (!GanttStatic::IsSameDisplayGroup(nPrevDisplay, nDisplay))
		{
			GANTTDATERANGE dtMaxRange;

			if (m_ctrlGantt.GetMaxDateRange(dtMaxRange) && dtActive.IntersectWith(dtMaxRange))
				dtActive.Set(dtActive, nDisplay, m_dlgPrefs.GetDecadesAreZeroBased());
			else
				dtActive.Reset();
		}

		if (dtActive.IsValid())
		{
			m_ctrlGantt.SetActiveDateRange(dtActive);
			m_sliderDateRange.SetSelectedRange(dtActive);
		}
	}

	UpdateActiveRangeLabel();

	return 0L;
}

LRESULT CGanttChartWnd::OnGanttNotifySortChange(WPARAM wp, LPARAM lp)
{
	// notify app
	GetParent()->SendMessage(WM_IUI_SORTCOLUMNCHANGE, wp, MapColumn((GTLC_COLUMN)lp));

	return 0L;
}

LRESULT CGanttChartWnd::OnGanttNotifySelChanged(WPARAM /*wp*/, LPARAM /*lp*/)
{
	SendParentSelectionUpdate();

	return 0L;
}

LRESULT CGanttChartWnd::OnGanttEditTaskTitle(WPARAM /*wp*/, LPARAM /*lp*/)
{
	// notify app to edit
	GetParent()->SendMessage(WM_IUI_EDITSELECTEDTASKTITLE);

	return 0L;
}

void CGanttChartWnd::OnSetFocus(CWnd* /*pOldWnd*/) 
{
	m_ctrlGantt.SetFocus();
}

void CGanttChartWnd::UpdateGanttCtrlPreferences()
{
	m_sliderDateRange.SetDecadesAreZeroBased(m_dlgPrefs.GetDecadesAreZeroBased());

	m_ctrlGantt.SetOption(GTLCF_DISPLAYTRAILINGTASKTITLE, m_dlgPrefs.GetDisplayTrailingTaskTitle());
	m_ctrlGantt.SetOption(GTLCF_DISPLAYTRAILINGALLOCTO, m_dlgPrefs.GetDisplayTrailingAllocTo());
	m_ctrlGantt.SetOption(GTLCF_AUTOSCROLLTOTASK, m_dlgPrefs.GetAutoScrollSelection());
	m_ctrlGantt.SetOption(GTLCF_CALCPARENTDATES, m_dlgPrefs.GetAutoCalcParentDates());
	m_ctrlGantt.SetOption(GTLCF_CALCMISSINGSTARTDATES, m_dlgPrefs.GetCalculateMissingStartDates());
	m_ctrlGantt.SetOption(GTLCF_CALCMISSINGDUEDATES, m_dlgPrefs.GetCalculateMissingDueDates());
	m_ctrlGantt.SetOption(GTLCF_DISPLAYPROGRESSINBAR, m_dlgPrefs.GetDisplayProgressInBar());
	m_ctrlGantt.SetOption(GTLCF_DECADESAREZEROBASED, m_dlgPrefs.GetDecadesAreZeroBased());
	m_ctrlGantt.SetOption(GTLCF_DISPLAYPARENTROLLUPS, m_dlgPrefs.GetDisplayParentsAsRollups());

	m_ctrlGantt.SetTodayColor(m_dlgPrefs.GetTodayColor());
	m_ctrlGantt.SetWeekendColor(m_dlgPrefs.GetWeekendColor());
	m_ctrlGantt.SetNonWorkingHoursColor(m_dlgPrefs.GetNonWorkingHoursColor());
	m_ctrlGantt.SetDefaultColor(m_dlgPrefs.GetDefaultColor());
	m_ctrlGantt.SetMilestoneTag(m_dlgPrefs.GetMilestoneTag());

	COLORREF crParent;
	GTLC_PARENTCOLORING nOption = (GTLC_PARENTCOLORING)m_dlgPrefs.GetParentColoring(crParent);

	m_ctrlGantt.SetParentColoring(nOption, crParent);

	CDWordArray aColumnVis;
	m_dlgPrefs.GetColumnVisibility(aColumnVis);
	m_ctrlGantt.SetTreeColumnVisibility(aColumnVis);
}

LRESULT CGanttChartWnd::OnGanttNotifyDateChange(WPARAM wp, LPARAM lp)
{
	COleDateTime dtStart, dtDue;
	if (m_ctrlGantt.GetSelectedTaskDates(dtStart, dtDue))
	{
		IUITASKMOD mod[2] = { { TDCA_NONE, 0 }, { TDCA_NONE, 0 } };
		int nNumMod = 1;
		
		switch (wp)
		{
		case GTLCD_START:
			if (CDateHelper::GetTimeT64(dtStart, mod[0].tValue))
			{
				mod[0].nAttrib = TDCA_STARTDATE;
			}
			break;
			
		case GTLCD_END:
			if (CDateHelper::GetTimeT64(dtDue, mod[0].tValue))
			{
				mod[0].nAttrib = TDCA_DUEDATE;
			}
			break;
			
		case GTLCD_WHOLE:
			{
				const GANTTITEM* pGIPreDrag = (const GANTTITEM*)lp;
				ASSERT(pGIPreDrag);
				
				// if the pre-drag start or due dates were not set
				// we set them explicitly else we offset the task
				BOOL bStartSet = pGIPreDrag->HasStartDate();
				BOOL bDueSet = pGIPreDrag->HasDueDate();

				if (bStartSet && bDueSet)
				{
					if (CDateHelper::GetTimeT64(dtStart, mod[0].tValue))
					{
						mod[0].nAttrib = TDCA_OFFSETTASK;
						mod[0].bOffsetSubtasks = true;
					}
				}
				else
				{
					if (CDateHelper::GetTimeT64(dtStart, mod[0].tValue) &&
						CDateHelper::GetTimeT64(dtDue, mod[1].tValue))
					{
						mod[0].nAttrib = TDCA_STARTDATE;
						mod[1].nAttrib = TDCA_DUEDATE;
						nNumMod = 2;
					}
				}
			}
			break;
		}
		
		if (mod[0].nAttrib != TDCA_NONE)
			return GetParent()->SendMessage(WM_IUI_MODIFYSELECTEDTASK, nNumMod, (LPARAM)&mod[0]);
	}

	return 0L;
}

void CGanttChartWnd::UpdateActiveRangeLabel()
{
	CString sRange = m_sliderDateRange.FormatRange();

	SetDlgItemText(IDC_ACTIVEDATERANGE_LABEL, CEnString(IDS_ACTIVEDATERANGE, sRange));
}

LRESULT CGanttChartWnd::OnGanttNotifyDragChange(WPARAM wp, LPARAM /*lp*/)
{
	// save snap changes as we go
	GTLC_SNAPMODE nSnap = (GTLC_SNAPMODE)wp;
	GTLC_MONTH_DISPLAY nDisplay = m_ctrlGantt.GetMonthDisplay();

	m_mapDisplaySnapModes[nDisplay] = nSnap;
	CDialogHelper::SelectItemByData(m_cbSnapModes, nSnap);

	return 0L;
}

void CGanttChartWnd::BuildSnapCombo()
{
	m_cbSnapModes.ResetContent();

	switch (m_ctrlGantt.GetMonthDisplay())
	{
	case GTLC_DISPLAY_QUARTERCENTURIES:
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTHALFYEAR, GTLCSM_NEARESTHALFYEAR);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTYEAR, GTLCSM_NEARESTYEAR);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTDECADE, GTLCSM_NEARESTDECADE);
		break;
		
	case GTLC_DISPLAY_DECADES:
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTMONTH, GTLCSM_NEARESTMONTH);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTHALFYEAR, GTLCSM_NEARESTHALFYEAR);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTYEAR, GTLCSM_NEARESTYEAR);
		break;
		
	case GTLC_DISPLAY_YEARS:
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTMONTH, GTLCSM_NEARESTMONTH);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTHALFYEAR, GTLCSM_NEARESTHALFYEAR);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTYEAR, GTLCSM_NEARESTYEAR);
		break;
		
	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTMONTH, GTLCSM_NEARESTMONTH);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTQUARTER, GTLCSM_NEARESTQUARTER);
		break;
		
	case GTLC_DISPLAY_MONTHS_SHORT:
	case GTLC_DISPLAY_MONTHS_MID:
	case GTLC_DISPLAY_MONTHS_LONG:
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTDAY, GTLCSM_NEARESTDAY);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTMONTH, GTLCSM_NEARESTMONTH);
		break;
		
	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTHALFDAY, GTLCSM_NEARESTHALFDAY);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTDAY, GTLCSM_NEARESTDAY);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTWEEK, GTLCSM_NEARESTWEEK);
		break;
		
	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTHOUR, GTLCSM_NEARESTHOUR);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTHALFDAY, GTLCSM_NEARESTHALFDAY);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTDAY, GTLCSM_NEARESTDAY);
		break;

	case GTLC_DISPLAY_HOURS:
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTHALFHOUR, GTLCSM_NEARESTHALFHOUR);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTHOUR, GTLCSM_NEARESTHOUR);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTHALFDAY, GTLCSM_NEARESTHALFDAY);
		break;
	}
	// Add to all
	CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_FREE, GTLCSM_FREE);

	CDialogHelper::SelectItemByData(m_cbSnapModes, m_ctrlGantt.GetSnapMode());
}

void CGanttChartWnd::BuildDisplayCombo()
{
	GTLC_MONTH_DISPLAY nCurDisplay = m_ctrlGantt.GetMonthDisplay();
	BOOL bOneBasedDecades = !m_dlgPrefs.GetDecadesAreZeroBased();

	m_cbDisplayOptions.ResetContent();

	for (int nMode = 0; nMode < NUM_DISPLAYMODES; nMode++)
	{
		const GTCDISPLAYMODE& mode = DISPLAYMODES[nMode];

		if (!m_ctrlGantt.CanSetMonthDisplay(mode.nDisplay))
		{
			int nCurMode = GanttStatic::FindDisplay(nCurDisplay);

			if (nMode < nCurMode)
				nCurDisplay = mode.nDisplay;

			break;
		}

		// else
		CEnString sItemText(mode.nStringID);

		// Handle one-based decades
		if (bOneBasedDecades)
		{
			if (mode.nDisplay == GTLC_DISPLAY_DECADES)
			{
				sItemText.Replace(_T("2000-2009"), _T("2001-2010"));
			}
			else if (mode.nDisplay == GTLC_DISPLAY_QUARTERCENTURIES)
			{
				sItemText.Replace(_T("2000-2024"), _T("2001-2025"));
			}
		}

		CDialogHelper::AddString(m_cbDisplayOptions, sItemText, mode.nDisplay);
	}
	
	CDialogHelper::RefreshMaxDropWidth(m_cbDisplayOptions);
	CDialogHelper::SelectItemByData(m_cbDisplayOptions, nCurDisplay);
}

void CGanttChartWnd::OnGanttGotoToday() 
{
	m_ctrlGantt.ScrollToToday();
	
	// and set focus back to it
	m_ctrlGantt.SetFocus();
}

void CGanttChartWnd::OnUpdateGanttGotoToday(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CGanttChartWnd::OnGanttPreferences() 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (m_dlgPrefs.DoModal() == IDOK)
	{
		// Update UI if 'zero-based decades' has changed
		BOOL bDecadeChange = (m_dlgPrefs.GetDecadesAreZeroBased() != m_ctrlGantt.HasOption(GTLCF_DECADESAREZEROBASED));

		// update gantt control
		UpdateGanttCtrlPreferences();
		
		if (bDecadeChange)
		{
			BuildDisplayCombo();
			UpdateActiveRangeLabel();

			GANTTDATERANGE dtRange;

			if (m_sliderDateRange.GetSelectedRange(dtRange))
				m_ctrlGantt.SetActiveDateRange(dtRange);
		}

		// and set focus back to it
		m_ctrlGantt.SetFocus();
	}
}

void CGanttChartWnd::OnUpdateGanttPreferences(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CGanttChartWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if (!bShow)
		m_ctrlGantt.CancelOperation();
}

void CGanttChartWnd::OnGanttNewDepends() 
{
	OnGanttDepends(GCDDM_ADD);
}

void CGanttChartWnd::OnUpdateGanttNewDepends(CCmdUI* pCmdUI) 
{
	OnUpdateGanttDepends(GCDDM_ADD, pCmdUI);
}

void CGanttChartWnd::OnGanttEditDepends() 
{
	OnGanttDepends(GCDDM_EDIT);
}

void CGanttChartWnd::OnUpdateGanttEditDepends(CCmdUI* pCmdUI) 
{
	OnUpdateGanttDepends(GCDDM_EDIT, pCmdUI);
}

void CGanttChartWnd::OnGanttDeleteDepends() 
{
	OnGanttDepends(GCDDM_DELETE);
}

void CGanttChartWnd::OnUpdateGanttDeleteDepends(CCmdUI* pCmdUI) 
{
	OnUpdateGanttDepends(GCDDM_DELETE, pCmdUI);
}

void CGanttChartWnd::OnGanttDepends(GCDD_MODE nMode)
{
	if (!m_bReadOnly)
	{
		ASSERT (m_dlgDepends.GetSafeHwnd() == NULL);
		
		if (m_dlgDepends.Create(nMode, this))
		{
			VERIFY (m_ctrlGantt.BeginDependencyEdit(&m_dlgDepends));
			m_toolbar.RefreshButtonStates();
		}
	}
}

void CGanttChartWnd::OnUpdateGanttDepends(GCDD_MODE nMode, CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_dlgDepends.GetMode() == nMode);
	pCmdUI->Enable(!m_bReadOnly && ((m_dlgDepends.GetMode() == nMode) || (m_dlgDepends.GetMode() == GCDDM_NONE)));
}

LRESULT CGanttChartWnd::OnGanttDependencyDlgClose(WPARAM wp, LPARAM lp)
{
	UNREFERENCED_PARAMETER(lp);
	ASSERT(((HWND)lp) == m_dlgDepends);
	
	if (m_dlgDepends.IsPickingCompleted())
	{
		// make sure the from task is selected
		DWORD dwFromTaskID = m_dlgDepends.GetFromTask();

		if (m_ctrlGantt.GetSelectedTaskID() != dwFromTaskID)
		{
			SelectTask(dwFromTaskID);

			// explicitly update parent because SelectTask will not
			SendParentSelectionUpdate();
		}

		CDWordArray aDepends;
		m_ctrlGantt.GetSelectedTaskDependencies(aDepends);
		
		switch (wp)
		{
		case GCDDM_ADD:
			aDepends.Add(m_dlgDepends.GetToTask());
			break;
			
		case GCDDM_EDIT:
			aDepends.Add(m_dlgDepends.GetToTask());
			 //fall thru to delete prev dependency
			
		case GCDDM_DELETE:
			{
				DWORD dwPrevToTask;
				VERIFY(m_dlgDepends.GetFromDependency(dwPrevToTask));
				VERIFY(Misc::RemoveItemT(dwPrevToTask, aDepends));
			}
			break;
			
		default:
			ASSERT(0);
			return 0L;
			
		}

		// notify parent
		CString sDepends = Misc::FormatArray(aDepends, '\n');
		IUITASKMOD mod = { TDCA_DEPENDENCY, 0 };

		mod.szValue = sDepends;
			
		if (GetParent()->SendMessage(WM_IUI_MODIFYSELECTEDTASK, 1, (LPARAM)&mod))
		{
			// update gantt ctrl
			m_ctrlGantt.SetSelectedTaskDependencies(aDepends);
		}
	}

	m_toolbar.RefreshButtonStates(FALSE);
	m_ctrlGantt.OnEndDepedencyEdit();
	m_ctrlGantt.SetFocus();

	return 0L;
}

LRESULT CGanttChartWnd::OnGanttNotifyCompletionChange(WPARAM /*wp*/, LPARAM lp) 
{
	IUITASKMOD mod = { TDCA_DONEDATE, 0 };

	if (lp) // done/not done
		VERIFY(CDateHelper::GetTimeT64(CDateHelper::GetDate(DHD_NOW), mod.tValue));
	else
		mod.tValue = T64Utils::T64_NULL;

	return GetParent()->SendMessage(WM_IUI_MODIFYSELECTEDTASK, 1, (LPARAM)&mod);
}

LRESULT CGanttChartWnd::OnGanttGetTaskIcon(WPARAM wp, LPARAM lp)
{
	return GetParent()->SendMessage(WM_IUI_GETTASKICON, wp, lp);
}

LRESULT CGanttChartWnd::OnGanttMoveTask(WPARAM wp, LPARAM lp)
{
	return GetParent()->SendMessage(WM_IUI_MOVESELECTEDTASK, wp, lp);
}

LRESULT CGanttChartWnd::OnGanttEditTaskIcon(WPARAM wp, LPARAM lp)
{
	return GetParent()->SendMessage(WM_IUI_EDITSELECTEDTASKICON, wp, lp);
}

LRESULT CGanttChartWnd::OnActiveDateRangeChange(WPARAM /*wp*/, LPARAM /*lp*/)
{
	GANTTDATERANGE dtSel;

	if (m_sliderDateRange.GetSelectedRange(dtSel))
		m_ctrlGantt.SetActiveDateRange(dtSel);
	else
		m_ctrlGantt.ClearActiveDateRange();

	BuildDisplayCombo();
	UpdateActiveRangeLabel();

	return 0L;
}
