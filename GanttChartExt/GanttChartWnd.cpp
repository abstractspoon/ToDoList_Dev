// GanttChartWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "GanttChartExt.h"
#include "GanttChartWnd.h"
#include "GanttStatic.h"
#include "GanttMsg.h"

#include "..\shared\misc.h"
#include "..\shared\themed.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\datehelper.h"
#include "..\shared\localizer.h"
#include "..\shared\autoflag.h"

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

/////////////////////////////////////////////////////////////////////////////
// CGanttChartWnd

CGanttChartWnd::CGanttChartWnd(CWnd* pParent /*=NULL*/)
	: 
	CDialog(IDD_GANTTTREE_DIALOG, pParent), 
	m_hIcon(NULL),
	m_bReadOnly(FALSE),
	m_bInSelectTask(FALSE),
#pragma warning(disable:4355)
	m_dlgPrefs(this)
#pragma warning(default:4355)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_GANTTCHART);
}

CGanttChartWnd::~CGanttChartWnd()
{
}

void CGanttChartWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGanttChartWnd)
	DDX_Control(pDX, IDC_SNAPMODES, m_cbSnapModes);
	DDX_Control(pDX, IDC_GANTTLIST, m_list);
	DDX_Control(pDX, IDC_GANTTTREE, m_tree);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DISPLAY, m_cbDisplayOptions);
	DDX_Text(pDX, IDC_SELECTEDTASKDATES, m_sSelectedTaskDates);
}

BEGIN_MESSAGE_MAP(CGanttChartWnd, CDialog)
	//{{AFX_MSG_MAP(CGanttChartWnd)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TVN_KEYUP, IDC_GANTTTREE, OnKeyUpGantt)
	ON_CBN_SELCHANGE(IDC_DISPLAY, OnSelchangeDisplay)
	ON_NOTIFY(NM_CLICK, IDC_GANTTLIST, OnClickGanttList)
	ON_NOTIFY(TVN_SELCHANGED, IDC_GANTTTREE, OnSelchangedGanttTree)
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
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_GANTTTREE, OnBeginEditTreeLabel)
	ON_WM_ERASEBKGND()

	ON_REGISTERED_MESSAGE(WM_GTLC_DATECHANGE, OnGanttNotifyDateChange)
	ON_REGISTERED_MESSAGE(WM_GTLC_DRAGCHANGE, OnGanttNotifyDragChange)
	ON_REGISTERED_MESSAGE(WM_GTLC_NOTIFYSORT, OnGanttNotifySortChange)
	ON_REGISTERED_MESSAGE(WM_GTLC_NOTIFYZOOM, OnGanttNotifyZoomChange)
	ON_REGISTERED_MESSAGE(WM_GANTTDEPENDDLG_CLOSE, OnGanttDependencyDlgClose)
	ON_REGISTERED_MESSAGE(WM_GTLC_PREFSHELP, OnGanttPrefsHelp)
	ON_CBN_SELCHANGE(IDC_SNAPMODES, OnSelchangeSnapMode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGanttChartWnd message handlers

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
	
	CString sKey(szKey);

	pPrefs->WriteProfileInt(sKey, _T("MonthDisplay"), m_ctrlGantt.GetMonthDisplay());
	pPrefs->WriteProfileInt(sKey, _T("SortColumn"), m_ctrlGantt.GetSortColumn());
	pPrefs->WriteProfileInt(sKey, _T("SortAscending"), m_ctrlGantt.GetSortAscending());

	// snap modes
	CString sSnapKey = sKey + _T("\\Snap");

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

	// column widths
	CIntArray aTreeOrder, aTreeWidths, aListWidths, aTreeTracked, aListTracked;

	m_ctrlGantt.GetColumnOrder(aTreeOrder);
	m_ctrlGantt.GetColumnWidths(aTreeWidths, aListWidths);
	m_ctrlGantt.GetTrackedColumns(aTreeTracked, aListTracked);

	SaveColumnState(pPrefs, (sKey + _T("\\TreeOrder")), aTreeOrder);
	SaveColumnState(pPrefs, (sKey + _T("\\TreeWidths")), aTreeWidths);
	SaveColumnState(pPrefs, (sKey + _T("\\ListWidths")), aListWidths);
	SaveColumnState(pPrefs, (sKey + _T("\\TreeTracked")), aTreeTracked);
	SaveColumnState(pPrefs, (sKey + _T("\\ListTracked")), aListTracked);

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

void CGanttChartWnd::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey, bool bAppOnly) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// application preferences
	m_ctrlGantt.SetOption(GTLCF_TASKTEXTCOLORISBKGND, pPrefs->GetProfileInt(_T("Preferences"), _T("ColorTaskBackground"), FALSE));
	m_ctrlGantt.SetOption(GTLCF_TREATSUBCOMPLETEDASDONE, pPrefs->GetProfileInt(_T("Preferences"), _T("TreatSubCompletedAsDone"), FALSE));
	m_ctrlGantt.SetOption(GTLCF_STRIKETHRUDONETASKS, pPrefs->GetProfileInt(_T("Preferences"), _T("StrikethroughDone"), TRUE));

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

	DWORD dwWeekends = pPrefs->GetProfileInt(_T("Preferences"), _T("Weekends"), (DHW_SATURDAY | DHW_SUNDAY));
	CDateHelper::SetWeekendDays(dwWeekends);

	// gantt specific options
	if (!bAppOnly)
	{
		CString sKey(szKey);
		
		// snap modes
		CString sSnapKey = sKey + _T("\\Snap");

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

		// last display
		GTLC_MONTH_DISPLAY nDisplay = (GTLC_MONTH_DISPLAY)pPrefs->GetProfileInt(sKey, _T("MonthDisplay"), GTLC_DISPLAY_MONTHS_LONG);
		SetMonthDisplay(nDisplay);

		m_dlgPrefs.LoadPreferences(pPrefs, sKey);
		UpdateGanttCtrlPreferences();

		// sort
		GTLC_COLUMN nCol = (GTLC_COLUMN)pPrefs->GetProfileInt(sKey, _T("SortColumn"), GTLCC_NONE);
		BOOL bAscending = pPrefs->GetProfileInt(sKey, _T("SortAscending"), TRUE);

		if (nCol != GTLCC_NONE)
			m_ctrlGantt.Sort(nCol, FALSE, bAscending);

		// column order
		CIntArray aTreeOrder, aTreeWidths, aListWidths, aTreeTracked, aListTracked;

		if (LoadColumnState(pPrefs, (sKey + _T("\\TreeOrder")), aTreeOrder))
		{
			m_ctrlGantt.SetColumnOrder(aTreeOrder);
		}
		
		// column widths
		if (LoadColumnState(pPrefs, (sKey + _T("\\TreeWidths")), aTreeWidths) &&
			LoadColumnState(pPrefs, (sKey + _T("\\ListWidths")), aListWidths))
		{
			m_ctrlGantt.SetColumnWidths(aTreeWidths, aListWidths);
		}
		else
		{
			m_ctrlGantt.ResizeColumnsToFit();
		}
		
		// column tracking
		if (LoadColumnState(pPrefs, (sKey + _T("\\TreeTracked")), aTreeTracked) &&
			LoadColumnState(pPrefs, (sKey + _T("\\ListTracked")), aListTracked))
		{
			m_ctrlGantt.SetTrackedColumns(aTreeTracked, aListTracked);
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
	}
}

bool CGanttChartWnd::ProcessMessage(MSG* pMsg) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (!IsWindowEnabled())
		return false;

	m_tree.ProcessMessage(pMsg);
	
	switch (pMsg->message)
	{
	// handle 'escape' during dependency editing
	case WM_KEYDOWN:
		{
			switch (pMsg->wParam)
			{
			case VK_ESCAPE:
				if (m_ctrlGantt.CancelOperation())
					return true;
				break;

			case VK_ADD:
				// Eat because Windows own processing does not understand how we do things!
				if (Misc::ModKeysArePressed(MKS_CTRL) && (m_list.GetSafeHwnd() == pMsg->hwnd))
					return true;
				break;
			}
		}
		break;
	}

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
		ClientToScreen(pEdit);
		return true;
	}

	return false;
}

IUI_HITTEST CGanttChartWnd::HitTest(const POINT& ptScreen) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// try tree header
	// 6.9: disable header click because it changes the 
	// tree/list columns not the gantt columns
	if (m_ctrlGantt.PtInHeader(ptScreen))
		return IUI_NOWHERE;//IUI_COLUMNHEADER;

	// then specific task
	if (m_ctrlGantt.HitTestTask(ptScreen))
		return IUI_TASK;

	// else check else where in tree or list client
	CRect rGantt;
	
	m_tree.GetClientRect(rGantt);
	m_tree.ClientToScreen(rGantt);

	if (rGantt.PtInRect(ptScreen))
		return IUI_TASKLIST;

	m_list.GetClientRect(rGantt);
	m_list.ClientToScreen(rGantt);

	if (rGantt.PtInRect(ptScreen))
		return IUI_TASKLIST;

	// else 
	return IUI_NOWHERE;
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

bool CGanttChartWnd::WantEditUpdate(IUI_ATTRIBUTE nAttribute) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (CGanttTreeListCtrl::WantEditUpdate(nAttribute) != FALSE);
}

bool CGanttChartWnd::WantSortUpdate(IUI_ATTRIBUTE nAttribute) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return (CGanttTreeListCtrl::WantSortUpdate(nAttribute) != FALSE);
}

void CGanttChartWnd::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const IUI_ATTRIBUTE* pAttributes, int nNumAttributes)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_ctrlGantt.UpdateTasks(pTasks, nUpdate, CSet<IUI_ATTRIBUTE>(pAttributes, nNumAttributes));

	// Month Display may change for large date ranges
	BuildDisplayCombo();

	UpdateSelectedTaskDates();
}

void CGanttChartWnd::Release()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (GetSafeHwnd())
		DestroyWindow();
	
	delete this;
}

bool CGanttChartWnd::DoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra) 
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

	case IUI_TOGGLABLESORT:
	case IUI_SORT:
		if (WantSortUpdate((IUI_ATTRIBUTE)dwExtra))
		{
			m_ctrlGantt.Sort(MapColumn(dwExtra), (nCmd == IUI_TOGGLABLESORT));
			return true;
		}
		break;

	case IUI_SETFOCUS:
		m_ctrlGantt.SetFocus();
		return true;

	case IUI_RESIZEATTRIBCOLUMNS:
		m_ctrlGantt.ResizeColumnsToFit();
		return true;
		
	case IUI_GETNEXTTASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTASK:
	case IUI_GETPREVTOPLEVELTASK:
		{
			DWORD* pTaskID = (DWORD*)dwExtra;
			DWORD dwNextID =  m_ctrlGantt.GetNextTask(*pTaskID, nCmd);
			
			if (dwNextID && (dwNextID != *pTaskID))
			{
				*pTaskID = dwNextID;
				return true;
			}
		}
		break;
	}

	return false;
}

bool CGanttChartWnd::CanDoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra) const 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	switch (nCmd)
	{
	case IUI_EXPANDALL:
	case IUI_COLLAPSEALL:
	case IUI_RESIZEATTRIBCOLUMNS:
		return true;

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

	case IUI_TOGGLABLESORT:
	case IUI_SORT:
		return WantSortUpdate((IUI_ATTRIBUTE)dwExtra);

	case IUI_SETFOCUS:
		return (m_ctrlGantt.HasFocus() != FALSE);
		
	case IUI_GETNEXTTASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTASK:
	case IUI_GETPREVTOPLEVELTASK:
		{
			DWORD* pTaskID = (DWORD*)dwExtra;
			DWORD dwNextID = m_ctrlGantt.GetNextTask(*pTaskID, nCmd);
			
			return (dwNextID && (dwNextID != *pTaskID));
		}
		return true;
	}

	// all else
	return false;
}

GTLC_COLUMN CGanttChartWnd::MapColumn(DWORD dwColumn)
{
	switch ((int)dwColumn)
	{
	case IUI_ALLOCTO:		return GTLCC_ALLOCTO;
	case IUI_TASKNAME:		return GTLCC_TITLE;
	case IUI_DUEDATE:		return GTLCC_ENDDATE;
	case IUI_STARTDATE:		return GTLCC_STARTDATE;
	case IUI_PERCENT:		return GTLCC_PERCENT;
	}

	return GTLCC_NONE;
}

DWORD CGanttChartWnd::MapColumn(GTLC_COLUMN nColumn)
{
	switch (nColumn)
	{
	case GTLCC_ALLOCTO:		return IUI_ALLOCTO;
	case GTLCC_TITLE:		return IUI_TASKNAME;
	case GTLCC_ENDDATE:		return IUI_DUEDATE;
	case GTLCC_STARTDATE:	return IUI_STARTDATE;
	case GTLCC_PERCENT:		return IUI_PERCENT;
	}
	
	return (DWORD)IUI_NONE;
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
	
	// init syncer
	m_ctrlGantt.Initialize(m_tree, m_list, IDC_TREEHEADER);
	m_ctrlGantt.ExpandAll();

	CRect rClient;
	GetClientRect(rClient);
	Resize(rClient.Width(), rClient.Height());

	BuildSnapCombo();
	BuildDisplayCombo();
	
	m_ctrlGantt.ScrollToToday();
	m_ctrlGantt.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGanttChartWnd::Resize(int cx, int cy)
{
	if (m_tree.GetSafeHwnd())
	{
		CRect rDisplay;
		m_cbDisplayOptions.GetWindowRect(rDisplay);
		ScreenToClient(rDisplay);

		CRect rGantt(0, rDisplay.bottom + 9, cx, cy);
		m_ctrlGantt.Resize(rGantt);

		// selected task dates takes available space
		int nOffset = cx - CDialogHelper::GetCtrlRect(this, IDC_SELECTEDTASKDATES).right;
		CDialogHelper::ResizeCtrl(this, IDC_SELECTEDTASKDATES, nOffset, 0);

		// always redraw the selected task dates
		GetDlgItem(IDC_SELECTEDTASKDATES)->Invalidate(FALSE);
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
	// let the gantt do its thing
	m_ctrlGantt.HandleEraseBkgnd(pDC);

	// clip out our children
	CDialogHelper::ExcludeChild(&m_toolbar, pDC);

	CDialogHelper::ExcludeCtrl(this, IDC_SELECTEDTASKDATES_LABEL, pDC);
	CDialogHelper::ExcludeCtrl(this, IDC_SELECTEDTASKDATES, pDC);
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
		UpdateSelectedTaskDates();
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
	UpdateSelectedTaskDates();
	SendParentSelectionUpdate();

	*pResult = 0;
}

BOOL CGanttChartWnd::SetMonthDisplay(GTLC_MONTH_DISPLAY nDisplay)
{
	m_ctrlGantt.ValidateMonthDisplay(nDisplay);

	if (m_ctrlGantt.SetMonthDisplay(nDisplay))
	{
		CDialogHelper::SelectItemByData(m_cbDisplayOptions, nDisplay);

		BuildSnapCombo();
		
		GTLC_SNAPMODE nSnap = GTLCSM_FREE;
		VERIFY(m_mapDisplaySnapModes.Lookup(nDisplay, nSnap));

		m_ctrlGantt.SetSnapMode(nSnap);
		CDialogHelper::SelectItemByData(m_cbSnapModes, nSnap);

		return TRUE;
	}

	// else
	return FALSE;
}

void CGanttChartWnd::OnSelchangeDisplay() 
{
	GTLC_MONTH_DISPLAY nCurDisplay = m_ctrlGantt.GetMonthDisplay();
	GTLC_MONTH_DISPLAY nNewDisplay = (GTLC_MONTH_DISPLAY)CDialogHelper::GetSelectedItemData(m_cbDisplayOptions);

	if (!SetMonthDisplay(nNewDisplay))
	{
		// restore previous selection
		CDialogHelper::SelectItemByData(m_cbSnapModes, nCurDisplay);
	}
}

void CGanttChartWnd::OnSelchangeSnapMode() 
{
	// save snap mode as we go
	GTLC_MONTH_DISPLAY nCurDisplay = m_ctrlGantt.GetMonthDisplay();
	GTLC_SNAPMODE nSnap = (GTLC_SNAPMODE)CDialogHelper::GetSelectedItemData(m_cbSnapModes);

	m_mapDisplaySnapModes[nCurDisplay] = nSnap;
	m_ctrlGantt.SetSnapMode(nSnap);
}

LRESULT CGanttChartWnd::OnGanttNotifyZoomChange(WPARAM /*wp*/, LPARAM lp)
{
	CDialogHelper::SelectItemByData(m_cbDisplayOptions, (GTLC_MONTH_DISPLAY)lp);

	return 0L;
}

LRESULT CGanttChartWnd::OnGanttNotifySortChange(WPARAM /*wp*/, LPARAM lp)
{
	// notify app
	GetParent()->SendMessage(WM_IUI_SORTCOLUMNCHANGE, 0, MapColumn((GTLC_COLUMN)lp));

	return 0L;
}

void CGanttChartWnd::OnSelchangedGanttTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;
	
	UpdateSelectedTaskDates();

	// ignore notifications arising out of SelectTask()
	if (m_bInSelectTask && (pNMTreeView->action == TVC_UNKNOWN))
		return;

	// we're only interested in non-keyboard changes
	// because keyboard gets handled in OnKeyUpGantt
	if (pNMTreeView->action != TVC_BYKEYBOARD)
		SendParentSelectionUpdate();
}

void CGanttChartWnd::OnBeginEditTreeLabel(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	*pResult = TRUE; // cancel our edit
	
	// notify app to edit
	GetParent()->SendMessage(WM_IUI_EDITSELECTEDTASKTITLE);
}

void CGanttChartWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CDialog::OnSetFocus(pOldWnd);
	
	m_tree.SetFocus();
}

void CGanttChartWnd::UpdateGanttCtrlPreferences()
{
	m_ctrlGantt.SetOption(GTLCF_DISPLAYALLOCTOAFTERITEM, m_dlgPrefs.GetDisplayAllocTo());
	m_ctrlGantt.SetOption(GTLCF_AUTOSCROLLTOTASK, m_dlgPrefs.GetAutoScrollSelection());
	m_ctrlGantt.SetOption(GTLCF_CALCPARENTDATES, m_dlgPrefs.GetAutoCalcParentDates());
	m_ctrlGantt.SetOption(GTLCF_CALCMISSINGSTARTDATES, m_dlgPrefs.GetCalculateMissingStartDates());
	m_ctrlGantt.SetOption(GTLCF_CALCMISSINGDUEDATES, m_dlgPrefs.GetCalculateMissingDueDates());
	m_ctrlGantt.SetOption(GTLCF_DISPLAYPROGRESSINBAR, m_dlgPrefs.GetDisplayProgressInBar());

	m_ctrlGantt.SetTodayColor(m_dlgPrefs.GetTodayColor());
	m_ctrlGantt.SetWeekendColor(m_dlgPrefs.GetWeekendColor());
	m_ctrlGantt.SetDefaultColor(m_dlgPrefs.GetDefaultColor());
	m_ctrlGantt.SetMilestoneTag(m_dlgPrefs.GetMilestoneTag());

	COLORREF crParent;
	GTLC_PARENTCOLORING nOption = (GTLC_PARENTCOLORING)m_dlgPrefs.GetParentColoring(crParent);

	m_ctrlGantt.SetParentColoring(nOption, crParent);
}

LRESULT CGanttChartWnd::OnGanttNotifyDateChange(WPARAM wp, LPARAM lp)
{
	COleDateTime dtStart, dtDue;
	if (m_ctrlGantt.GetSelectedTaskDates(dtStart, dtDue))
	{
		IUITASKMOD mod[2] = { { IUI_NONE, 0 }, { IUI_NONE, 0 } };
		int nNumMod = 1;
		
		switch (wp)
		{
		case GTLCHT_BEGIN:
			if (CDateHelper::GetTimeT64(dtStart, mod[0].tValue))
			{
				mod[0].nAttrib = IUI_STARTDATE;
			}
			break;
			
		case GTLCHT_MIDDLE:
			{
				const GANTTITEM* pGIPreDrag = (const GANTTITEM*)lp;
				ASSERT(pGIPreDrag);
				
				// if the pre-drag start or due dates were not set
				// we set them explicitly else we offset the task
				BOOL bStartSet = CDateHelper::IsDateSet(pGIPreDrag->dtStart);
				BOOL bDueSet = CDateHelper::IsDateSet(pGIPreDrag->dtDue);

				if (bStartSet && bDueSet)
				{
					if (CDateHelper::GetTimeT64(dtStart, mod[0].tValue))
						mod[0].nAttrib = IUI_OFFSETTASK;
				}
				else
				{
					if (CDateHelper::GetTimeT64(dtStart, mod[0].tValue) &&
						CDateHelper::GetTimeT64(dtDue, mod[1].tValue))
					{
						mod[0].nAttrib = IUI_STARTDATE;
						mod[1].nAttrib = IUI_DUEDATE;
						nNumMod = 2;
					}
				}
			}
			break;
			
		case GTLCHT_END:
			if (CDateHelper::GetTimeT64(dtDue, mod[0].tValue))
			{
				mod[0].nAttrib = IUI_DUEDATE;
			}
			break;
		}
		
		if (mod[0].nAttrib != IUI_NONE)
			return GetParent()->SendMessage(WM_IUI_MODIFYSELECTEDTASK, nNumMod, (LPARAM)&mod[0]);
	}

	return 0L;
}

void CGanttChartWnd::UpdateSelectedTaskDates()
{
	COleDateTime dtStart, dtDue;
	
	if (m_ctrlGantt.GetSelectedTaskDates(dtStart, dtDue))
	{
		CString sStart, sDue;

		if (CDateHelper::DateHasTime(dtStart))
			sStart = CDateHelper::FormatDate(dtStart, DHFD_TIME | DHFD_NOSEC);
		else
			sStart = CDateHelper::FormatDate(dtStart);

		if (CDateHelper::DateHasTime(dtDue))
			sDue = CDateHelper::FormatDate(dtDue, DHFD_TIME | DHFD_NOSEC);
		else
			sDue = CDateHelper::FormatDate(dtDue);

		m_sSelectedTaskDates.Format(_T("%s - %s"), sStart, sDue);
	}
	else
	{
		m_sSelectedTaskDates.Empty();
	}

	UpdateData(FALSE);
}

LRESULT CGanttChartWnd::OnGanttNotifyDragChange(WPARAM wp, LPARAM /*lp*/)
{
	// save snap changes as we go
	GTLC_SNAPMODE nSnap = (GTLC_SNAPMODE)wp;
	GTLC_MONTH_DISPLAY nDisplay = m_ctrlGantt.GetMonthDisplay();

	m_mapDisplaySnapModes[nDisplay] = nSnap;

	CDialogHelper::SelectItemByData(m_cbSnapModes, nSnap);
	UpdateSelectedTaskDates();

	return 0L;
}

void CGanttChartWnd::BuildSnapCombo()
{
	m_cbSnapModes.ResetContent();

	switch (m_ctrlGantt.GetMonthDisplay())
	{
	case GTLC_DISPLAY_YEARS:
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTHALFYEAR, GTLCSM_NEARESTHALFYEAR);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTMONTH, GTLCSM_NEARESTMONTH);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTYEAR, GTLCSM_NEARESTYEAR);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_FREE, GTLCSM_FREE);
		break;
		
	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTMONTH, GTLCSM_NEARESTMONTH);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTQUARTER, GTLCSM_NEARESTQUARTER);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_FREE, GTLCSM_FREE);
		break;
		
	case GTLC_DISPLAY_MONTHS_SHORT:
	case GTLC_DISPLAY_MONTHS_MID:
	case GTLC_DISPLAY_MONTHS_LONG:
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTDAY, GTLCSM_NEARESTDAY);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTMONTH, GTLCSM_NEARESTMONTH);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_FREE, GTLCSM_FREE);
		break;
		
	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTDAY, GTLCSM_NEARESTDAY);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTWEEK, GTLCSM_NEARESTWEEK);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_FREE, GTLCSM_FREE);
		break;
		
	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTHALFDAY, GTLCSM_NEARESTHALFDAY);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTHOUR, GTLCSM_NEARESTHOUR);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTDAY, GTLCSM_NEARESTDAY);
		CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_FREE, GTLCSM_FREE);
		break;
	}

	CDialogHelper::SelectItemByData(m_cbSnapModes, m_ctrlGantt.GetSnapMode());
}

void CGanttChartWnd::BuildDisplayCombo()
{
	GTLC_MONTH_DISPLAY nCurDisplay = m_ctrlGantt.GetMonthDisplay();

	m_cbDisplayOptions.ResetContent();

	for (int nMode = 0; nMode < GTLC_DISPLAY_COUNT; nMode++)
	{
		const GTCDISPLAYMODE& mode = DISPLAYMODES[nMode];

		if (!m_ctrlGantt.CanSetMonthDisplay(mode.nDisplay))
		{
			nCurDisplay = min(nCurDisplay, mode.nDisplay);
			break;
		}

		// else
		CDialogHelper::AddString(m_cbDisplayOptions, mode.nStringID, mode.nDisplay);
	}
	
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
		// update gantt control
		UpdateGanttCtrlPreferences();
		
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

		CStringArray aDepends;
		m_ctrlGantt.GetSelectedTaskDependencies(aDepends);
		
		switch (wp)
		{
		case GCDDM_ADD:
			{
				DWORD dwToTaskID = m_dlgDepends.GetToTask();
				aDepends.Add(Misc::Format(dwToTaskID));
			}
			break;
			
		case GCDDM_EDIT:
			{
				DWORD dwToTaskID = m_dlgDepends.GetToTask();
				aDepends.Add(Misc::Format(dwToTaskID));
			}
			 //fall thru to delete prev dependency
			
		case GCDDM_DELETE:
			{
				DWORD dwPrevToTask;
				VERIFY(m_dlgDepends.GetFromDependency(dwPrevToTask));

				CString sDepend = Misc::Format(dwPrevToTask);
				VERIFY(Misc::RemoveItem(sDepend, aDepends));
			}
			break;
			
		default:
			ASSERT(0);
			return 0L;
			
		}

		// notify parent
		CString sDepends = Misc::FormatArray(aDepends);
		IUITASKMOD mod = { IUI_DEPENDENCY, 0 };

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

