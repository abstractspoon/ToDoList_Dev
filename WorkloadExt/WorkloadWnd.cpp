// WorkloadWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "WorkloadExt.h"
#include "WorkloadWnd.h"
#include "WorkloadStatic.h"
#include "WorkloadMsg.h"

#include "..\shared\misc.h"
#include "..\shared\themed.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\datehelper.h"
#include "..\shared\timehelper.h"
#include "..\shared\localizer.h"
#include "..\shared\autoflag.h"
#include "..\shared\holdredraw.h"
#include "..\shared\dlgunits.h"

#include "..\3rdparty\T64Utils.h"
#include "..\3rdparty\dibdata.h"

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
// CWorkloadWnd

CWorkloadWnd::CWorkloadWnd(CWnd* pParent /*=NULL*/)
	: 
	CDialog(IDD_WorkloadTREE_DIALOG, pParent), 
	m_ctrlWorkload(m_tree, m_list),
	m_bReadOnly(FALSE),
	m_bInSelectTask(FALSE),
#pragma warning(disable:4355)
	m_dlgPrefs(this)
#pragma warning(default:4355)
{
	m_icon.LoadIcon(IDR_Workload);
}

CWorkloadWnd::~CWorkloadWnd()
{
}

void CWorkloadWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWorkloadWnd)
	DDX_Control(pDX, IDC_SNAPMODES, m_cbSnapModes);
	DDX_Control(pDX, IDC_WorkloadLIST, m_list);
	DDX_Control(pDX, IDC_WorkloadTREE, m_tree);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DISPLAY, m_cbDisplayOptions);
	DDX_Text(pDX, IDC_SELECTEDTASKDATES, m_sSelectedTaskDates);
}

BEGIN_MESSAGE_MAP(CWorkloadWnd, CDialog)
	//{{AFX_MSG_MAP(CWorkloadWnd)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TVN_KEYUP, IDC_WorkloadTREE, OnKeyUpWorkload)
	ON_CBN_SELCHANGE(IDC_DISPLAY, OnSelchangeDisplay)
	ON_NOTIFY(NM_CLICK, IDC_WorkloadLIST, OnClickWorkloadList)
	ON_NOTIFY(TVN_SELCHANGED, IDC_WorkloadTREE, OnSelchangedWorkloadTree)
	ON_COMMAND(ID_Workload_GOTOTODAY, OnWorkloadGotoToday)
	ON_UPDATE_COMMAND_UI(ID_Workload_GOTOTODAY, OnUpdateWorkloadGotoToday)
	ON_COMMAND(ID_Workload_NEWDEPENDS, OnWorkloadNewDepends)
	ON_UPDATE_COMMAND_UI(ID_Workload_NEWDEPENDS, OnUpdateWorkloadNewDepends)
	ON_COMMAND(ID_Workload_PREFS, OnWorkloadPreferences)
	ON_UPDATE_COMMAND_UI(ID_Workload_PREFS, OnUpdateWorkloadPreferences)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_Workload_EDITDEPENDS, OnWorkloadEditDepends)
	ON_UPDATE_COMMAND_UI(ID_Workload_EDITDEPENDS, OnUpdateWorkloadEditDepends)
	ON_COMMAND(ID_Workload_DELETEDEPENDS, OnWorkloadDeleteDepends)
	ON_UPDATE_COMMAND_UI(ID_Workload_DELETEDEPENDS, OnUpdateWorkloadDeleteDepends)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, OnHelp)
	ON_WM_HELPINFO()
	ON_WM_SETFOCUS()
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_WorkloadTREE, OnBeginEditTreeLabel)
	ON_WM_ERASEBKGND()
	ON_WM_NCDESTROY()

	ON_REGISTERED_MESSAGE(WM_GTLC_DATECHANGE, OnWorkloadNotifyDateChange)
	ON_REGISTERED_MESSAGE(WM_GTLC_DRAGCHANGE, OnWorkloadNotifyDragChange)
	ON_REGISTERED_MESSAGE(WM_GTLC_COMPLETIONCHANGE, OnWorkloadNotifyCompletionChange)
	ON_REGISTERED_MESSAGE(WM_GTLC_NOTIFYSORT, OnWorkloadNotifySortChange)
	ON_REGISTERED_MESSAGE(WM_GTLC_NOTIFYZOOM, OnWorkloadNotifyZoomChange)
	ON_REGISTERED_MESSAGE(WM_WorkloadDEPENDDLG_CLOSE, OnWorkloadDependencyDlgClose)
	ON_REGISTERED_MESSAGE(WM_GTLC_PREFSHELP, OnWorkloadPrefsHelp)
	ON_REGISTERED_MESSAGE(WM_GTLC_GETTASKICON, OnWorkloadGetTaskIcon)
	ON_REGISTERED_MESSAGE(WM_GTLC_MOVETASK, OnWorkloadMoveTask)
	ON_CBN_SELCHANGE(IDC_SNAPMODES, OnSelchangeSnapMode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkloadWnd message handlers

void CWorkloadWnd::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	
	delete this;
}

void CWorkloadWnd::OnHelp()
{
	GetParent()->SendMessage(WM_IUI_DOHELP, 0, (LPARAM)GetTypeID());
}

BOOL CWorkloadWnd::OnHelpInfo(HELPINFO* /*lpHelpInfo*/)
{
	OnHelp();
	return TRUE;
}

LRESULT CWorkloadWnd::OnWorkloadPrefsHelp(WPARAM /*wp*/, LPARAM /*lp*/)
{
	CString sHelpID(GetTypeID());
	sHelpID += _T("_PREFS");
	
	GetParent()->SendMessage(WM_IUI_DOHELP, 0, (LPARAM)(LPCTSTR)sHelpID);
	return 0L;
}

void CWorkloadWnd::SetReadOnly(bool bReadOnly)
{
	m_bReadOnly = bReadOnly;
	m_ctrlWorkload.SetReadOnly(bReadOnly);

	m_toolbar.RefreshButtonStates(FALSE);
}

BOOL CWorkloadWnd::Create(DWORD dwStyle, const RECT &/*rect*/, CWnd* pParentWnd, UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (CDialog::Create(IDD_WorkloadTREE_DIALOG, pParentWnd))
	{
		SetWindowLong(*this, GWL_STYLE, dwStyle);
		SetDlgCtrlID(nID);

		return TRUE;
	}

	return FALSE;
}

void CWorkloadWnd::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// NOTE: sort is handled by the app
	CString sKey(szKey);

	pPrefs->WriteProfileInt(sKey, _T("MonthDisplay"), m_ctrlWorkload.GetMonthDisplay());
	pPrefs->WriteProfileInt(sKey, _T("SortColumn"), m_ctrlWorkload.GetSortColumn());
	pPrefs->WriteProfileInt(sKey, _T("SortAscending"), m_ctrlWorkload.GetSortAscending());

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

	m_ctrlWorkload.GetTreeColumnOrder(aTreeOrder);
	m_ctrlWorkload.GetColumnWidths(aTreeWidths, aListWidths);
	m_ctrlWorkload.GetTrackedColumns(aTreeTracked, aListTracked);

	SaveColumnState(pPrefs, (sKey + _T("\\TreeOrder")), aTreeOrder);
	SaveColumnState(pPrefs, (sKey + _T("\\TreeWidths")), aTreeWidths);
	SaveColumnState(pPrefs, (sKey + _T("\\ListWidths")), aListWidths);
	SaveColumnState(pPrefs, (sKey + _T("\\TreeTracked")), aTreeTracked);
	SaveColumnState(pPrefs, (sKey + _T("\\ListTracked")), aListTracked);

	m_dlgPrefs.SavePreferences(pPrefs, sKey);
}

void CWorkloadWnd::SaveColumnState(IPreferences* pPrefs, LPCTSTR szKey, const CIntArray& aStates) const
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

int CWorkloadWnd::LoadColumnState(const IPreferences* pPrefs, LPCTSTR szKey, CIntArray& aStates) const
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

void CWorkloadWnd::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey, bool bAppOnly) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// application preferences
	m_ctrlWorkload.SetOption(GTLCF_TASKTEXTCOLORISBKGND, pPrefs->GetProfileInt(_T("Preferences"), _T("ColorTaskBackground"), FALSE));
	m_ctrlWorkload.SetOption(GTLCF_TREATSUBCOMPLETEDASDONE, pPrefs->GetProfileInt(_T("Preferences"), _T("TreatSubCompletedAsDone"), FALSE));
	m_ctrlWorkload.SetOption(GTLCF_STRIKETHRUDONETASKS, pPrefs->GetProfileInt(_T("Preferences"), _T("StrikethroughDone"), TRUE));
	m_ctrlWorkload.SetOption(GTLCF_DISABLEDEPENDENTDRAGGING, pPrefs->GetProfileInt(_T("Preferences"), _T("AutoAdjustDependents"), TRUE));
	m_ctrlWorkload.SetOption(GTLCF_DISPLAYISODATES, pPrefs->GetProfileInt(_T("Preferences"), _T("DisplayDatesInISO"), FALSE));
	m_ctrlWorkload.SetOption(GTLCF_SHOWSPLITTERBAR, (pPrefs->GetProfileInt(_T("Preferences"), _T("HidePaneSplitBar"), TRUE) == FALSE));

	m_tree.ShowCheckboxes(pPrefs->GetProfileInt(_T("Preferences"), _T("AllowCheckboxAgainstTreeItem"), TRUE));

	// get alternate line color from app prefs
	COLORREF crAlt = CLR_NONE;

	if (pPrefs->GetProfileInt(_T("Preferences"), _T("AlternateLineColor"), TRUE))
		crAlt = pPrefs->GetProfileInt(_T("Preferences\\Colors"), _T("AlternateLines"), DEF_ALTLINECOLOR);

	m_ctrlWorkload.SetAlternateLineColor(crAlt);

	// get grid line color from app prefs
	COLORREF crGrid = CLR_NONE;

	if (pPrefs->GetProfileInt(_T("Preferences"), _T("SpecifyGridColor"), TRUE))
		crGrid = pPrefs->GetProfileInt(_T("Preferences\\Colors"), _T("GridLines"), DEF_GRIDLINECOLOR);

	m_ctrlWorkload.SetGridLineColor(crGrid);

	DWORD dwWeekends = pPrefs->GetProfileInt(_T("Preferences"), _T("Weekends"), (DHW_SATURDAY | DHW_SUNDAY));
	CDateHelper::SetWeekendDays(dwWeekends);

	double dHoursInDay = pPrefs->GetProfileDouble(_T("Preferences"), _T("HoursInDay"), 8.0);
	CTimeHelper::SetHoursInWorkday(dHoursInDay);

	// Workload specific options
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
		UpdateWorkloadCtrlPreferences();


		// column order
		CIntArray aTreeOrder, aTreeWidths, aListWidths, aTreeTracked, aListTracked;

		if (LoadColumnState(pPrefs, (sKey + _T("\\TreeOrder")), aTreeOrder))
		{
			m_ctrlWorkload.SetTreeColumnOrder(aTreeOrder);
		}
		
		// column widths
		if (!LoadColumnState(pPrefs, (sKey + _T("\\TreeWidths")), aTreeWidths) ||
			!LoadColumnState(pPrefs, (sKey + _T("\\ListWidths")), aListWidths) ||
			!m_ctrlWorkload.SetColumnWidths(aTreeWidths, aListWidths))
		{
			m_ctrlWorkload.ResizeColumnsToFit();
		}
		
		// column tracking
		if (LoadColumnState(pPrefs, (sKey + _T("\\TreeTracked")), aTreeTracked) &&
			LoadColumnState(pPrefs, (sKey + _T("\\ListTracked")), aListTracked))
		{
			m_ctrlWorkload.SetTrackedColumns(aTreeTracked, aListTracked);
		}
		
		if (GetSafeHwnd())
			UpdateData(FALSE);
	}
}

void CWorkloadWnd::LoadSnapModePreference(const IPreferences* pPrefs, LPCTSTR szSnapKey, GTLC_MONTH_DISPLAY nDisplay, LPCTSTR szDisplay, GTLC_SNAPMODE nDefaultSnap) 
{
	m_mapDisplaySnapModes[nDisplay]	= (GTLC_SNAPMODE)pPrefs->GetProfileInt(szSnapKey, szDisplay, nDefaultSnap);
}

void CWorkloadWnd::SaveSnapModePreference(IPreferences* pPrefs, LPCTSTR szSnapKey, GTLC_MONTH_DISPLAY nDisplay, LPCTSTR szDisplay) const
{
	GTLC_SNAPMODE nSnap = GTLCSM_FREE;

	VERIFY (m_mapDisplaySnapModes.Lookup(nDisplay, nSnap));
	pPrefs->WriteProfileInt(szSnapKey, szDisplay, nSnap);
}

void CWorkloadWnd::SetUITheme(const UITHEME* pTheme)
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

		m_ctrlWorkload.SetSplitBarColor(m_theme.crAppBackDark);
	}
}

void CWorkloadWnd::FilterToolTipMessage(MSG* pMsg) 
{
	m_ctrlWorkload.FilterToolTipMessage(pMsg);
}

bool CWorkloadWnd::ProcessMessage(MSG* pMsg) 
{
	if (!IsWindowEnabled())
		return false;

	switch (pMsg->message)
	{
	// handle 'escape' during dependency editing
	case WM_KEYDOWN:
		{
			AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
			switch (pMsg->wParam)
			{
			case VK_ESCAPE:
				if (m_ctrlWorkload.CancelOperation())
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

	// Drag and drop messages
	if (m_ctrlWorkload.ProcessMessage(pMsg))
		return true;

	// toolbar messages
	return (m_tbHelper.ProcessMessage(pMsg) != FALSE);
}

bool CWorkloadWnd::PrepareNewTask(ITaskList* pTask) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return m_ctrlWorkload.PrepareNewTask(pTask);
}

bool CWorkloadWnd::GetLabelEditRect(LPRECT pEdit)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (m_ctrlWorkload.GetLabelEditRect(pEdit))
	{
		// convert to screen coords
		ClientToScreen(pEdit);
		return true;
	}

	return false;
}

IUI_HITTEST CWorkloadWnd::HitTest(const POINT& ptScreen) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// try tree header
	// 6.9: disable header click because it changes the 
	// tree/list columns not the Workload columns
	if (m_ctrlWorkload.PointInHeader(ptScreen))
		return IUI_NOWHERE;//IUI_COLUMNHEADER;

	// then specific task
	if (m_ctrlWorkload.HitTestTask(ptScreen))
		return IUI_TASK;

	// else check else where in tree or list client
	CRect rWorkload;
	
	m_tree.GetClientRect(rWorkload);
	m_tree.ClientToScreen(rWorkload);

	if (rWorkload.PtInRect(ptScreen))
		return IUI_TASKLIST;

	m_list.GetClientRect(rWorkload);
	m_list.ClientToScreen(rWorkload);

	if (rWorkload.PtInRect(ptScreen))
		return IUI_TASKLIST;

	// else 
	return IUI_NOWHERE;
}


bool CWorkloadWnd::SelectTask(DWORD dwTaskID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CAutoFlag af(m_bInSelectTask, TRUE);

	return (m_ctrlWorkload.SelectTask(dwTaskID) != FALSE);
}

bool CWorkloadWnd::SelectTasks(const DWORD* /*pdwTaskIDs*/, int /*nTaskCount*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return false; // only support single selection
}

bool CWorkloadWnd::WantTaskUpdate(IUI_ATTRIBUTE nAttribute) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (CWorkloadTreeListCtrl::WantEditUpdate(nAttribute) != FALSE);
}

void CWorkloadWnd::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const IUI_ATTRIBUTE* pAttributes, int nNumAttributes)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_ctrlWorkload.UpdateTasks(pTasks, nUpdate, CSet<IUI_ATTRIBUTE>(pAttributes, nNumAttributes));

	// Month Display may change for large date ranges
	BuildDisplayCombo();

	UpdateSelectedTaskDates();
}

bool CWorkloadWnd::DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData) 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (nCmd)
	{
	case IUI_EXPANDALL:
		m_ctrlWorkload.ExpandAll(TRUE);
		return true;

	case IUI_COLLAPSEALL:
		m_ctrlWorkload.ExpandAll(FALSE);
		return true;

	case IUI_EXPANDSELECTED:
		m_ctrlWorkload.ExpandItem(m_ctrlWorkload.GetSelectedItem(), TRUE, TRUE);
		return true;

	case IUI_COLLAPSESELECTED:
		m_ctrlWorkload.ExpandItem(m_ctrlWorkload.GetSelectedItem(), FALSE);
		return true;

	case IUI_TOGGLABLESORT:
		if (pData)
		{
			m_ctrlWorkload.Sort(MapColumn(pData->nSortBy), TRUE);
			return true;
		}
		break;

	case IUI_SORT:
		if (pData)
		{
			m_ctrlWorkload.Sort(MapColumn(pData->nSortBy), FALSE);
			return true;
		}
		break;

	case IUI_MULTISORT:
		if (pData)
		{
			WorkloadSORTCOLUMNS sort;

			sort.cols[0].nBy = m_ctrlWorkload.MapAttributeToColumn(pData->sort.nAttrib1);
			sort.cols[0].bAscending = (pData->sort.bAscending1 ? TRUE : FALSE);

			sort.cols[1].nBy = m_ctrlWorkload.MapAttributeToColumn(pData->sort.nAttrib2);
			sort.cols[1].bAscending = (pData->sort.bAscending2 ? TRUE : FALSE);

			sort.cols[2].nBy = m_ctrlWorkload.MapAttributeToColumn(pData->sort.nAttrib3);
			sort.cols[2].bAscending = (pData->sort.bAscending3 ? TRUE : FALSE);
			
			m_ctrlWorkload.Sort(sort);
			return true;
		}
		break;

	case IUI_SETFOCUS:
		m_ctrlWorkload.SetFocus();
		return true;

	case IUI_RESIZEATTRIBCOLUMNS:
		m_ctrlWorkload.ResizeColumnsToFit();
		return true;
		
	case IUI_SELECTTASK:
		if (pData)
			return SelectTask(pData->dwTaskID);
		break;
		
	case IUI_GETNEXTTASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTASK:
	case IUI_GETPREVTOPLEVELTASK:
		if (pData)
		{
			DWORD dwNextID =  m_ctrlWorkload.GetNextTask(pData->dwTaskID, nCmd);
			
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

			if (m_ctrlWorkload.SaveToImage(bmImage))
			{
				CDibData dib;

				if (dib.CreateDIB(bmImage) && dib.SaveDIB(pData->szFilePath))
					return true;
			}
		}
		break;

	case IUI_SETTASKFONT:
		if (pData)
		{
			CHoldRedraw hr(*this);
			m_ctrlWorkload.SetFont(pData->hFont, TRUE);
		}
		break;

	case IUI_SELECTFIRSTTASK:
	case IUI_SELECTNEXTTASK:
	case IUI_SELECTNEXTTASKINCLCURRENT:
	case IUI_SELECTPREVTASK:
	case IUI_SELECTLASTTASK:
		if (pData)
			return (m_ctrlWorkload.SelectTask(nCmd, pData->select) != FALSE);
		break;

		return true;

	case IUI_MOVETASK:
		if (pData)
		{
			ASSERT(pData->move.dwSelectedTaskID == m_ctrlWorkload.GetSelectedTaskID());

			return (m_ctrlWorkload.MoveSelectedItem(pData->move) != FALSE);
		}
		break;

	}

	return false;
}

bool CWorkloadWnd::CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData) const 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	switch (nCmd)
	{
	case IUI_EXPANDALL:
	case IUI_COLLAPSEALL:
	case IUI_RESIZEATTRIBCOLUMNS:
	case IUI_SELECTTASK:
		return true;

	case IUI_SAVETOIMAGE:
		return (m_tree.GetCount() > 0);

	case IUI_EXPANDSELECTED:
		{
			HTREEITEM htiSel = m_ctrlWorkload.GetSelectedItem();
			return (m_ctrlWorkload.CanExpandItem(htiSel, TRUE) != FALSE);
		}
		break;

	case IUI_COLLAPSESELECTED:
		{
			HTREEITEM htiSel = m_ctrlWorkload.GetSelectedItem();
			return (m_ctrlWorkload.CanExpandItem(htiSel, FALSE) != FALSE);
		}
		break;

	case IUI_TOGGLABLESORT:
	case IUI_SORT:
		if (pData)
			return (CWorkloadTreeListCtrl::WantSortUpdate(pData->nSortBy) != FALSE);
		break;

	case IUI_MULTISORT:
		return true;

	case IUI_SETFOCUS:
		return (CDialogHelper::IsChildOrSame(this, GetFocus()) == FALSE);
		
	case IUI_GETNEXTTASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTASK:
	case IUI_GETPREVTOPLEVELTASK:
		if (pData)
		{
			DWORD dwNextID =  m_ctrlWorkload.GetNextTask(pData->dwTaskID, nCmd);

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
			return (m_ctrlWorkload.CanMoveSelectedItem(pData->move) != FALSE);
		break;
	}

	// all else
	return false;
}

GTLC_COLUMN CWorkloadWnd::MapColumn(DWORD dwColumn)
{
	return CWorkloadTreeListCtrl::MapAttributeToColumn((IUI_ATTRIBUTE)dwColumn);
}

DWORD CWorkloadWnd::MapColumn(GTLC_COLUMN nColumn)
{
	return (DWORD)CWorkloadTreeListCtrl::MapColumnToAttribute(nColumn);
}

void CWorkloadWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	Resize(cx, cy);
}

BOOL CWorkloadWnd::OnInitDialog() 
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
	m_ctrlWorkload.Initialize(IDC_TREEHEADER);
	m_ctrlWorkload.ExpandAll();

	CRect rClient;
	GetClientRect(rClient);
	Resize(rClient.Width(), rClient.Height());

	BuildSnapCombo();
	BuildDisplayCombo();
	
	m_ctrlWorkload.ScrollToToday();
	m_ctrlWorkload.SetFocus();
	
	m_tree.ShowTaskIcons();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWorkloadWnd::Resize(int cx, int cy)
{
	if (m_tree.GetSafeHwnd())
	{
		CRect rWorkload(0, 0, cx, cy);
		rWorkload.top = CDlgUnits(this).ToPixelsY(28);

		m_ctrlWorkload.Resize(rWorkload);

		// selected task dates takes available space
		int nOffset = cx - CDialogHelper::GetCtrlRect(this, IDC_SELECTEDTASKDATES).right;
		CDialogHelper::ResizeCtrl(this, IDC_SELECTEDTASKDATES, nOffset, 0);

		// always redraw the selected task dates
		GetDlgItem(IDC_SELECTEDTASKDATES)->Invalidate(FALSE);
	}
}

HBRUSH CWorkloadWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CWorkloadWnd::OnEraseBkgnd(CDC* pDC) 
{
	// let the Workload do its thing
	m_ctrlWorkload.HandleEraseBkgnd(pDC);

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

void CWorkloadWnd::OnKeyUpWorkload(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CWorkloadWnd::SendParentSelectionUpdate()
{
	DWORD dwTaskID = m_ctrlWorkload.GetSelectedTaskID();
	GetParent()->SendMessage(WM_IUI_SELECTTASK, 0, dwTaskID);
}

void CWorkloadWnd::OnClickWorkloadList(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	UpdateSelectedTaskDates();
	SendParentSelectionUpdate();

	*pResult = 0;
}

BOOL CWorkloadWnd::SetMonthDisplay(GTLC_MONTH_DISPLAY nDisplay)
{
	m_ctrlWorkload.ValidateMonthDisplay(nDisplay);

	if (m_ctrlWorkload.SetMonthDisplay(nDisplay))
	{
		CDialogHelper::SelectItemByData(m_cbDisplayOptions, nDisplay);

		BuildSnapCombo();
		
		GTLC_SNAPMODE nSnap = GTLCSM_FREE;
		VERIFY(m_mapDisplaySnapModes.Lookup(nDisplay, nSnap));

		m_ctrlWorkload.SetSnapMode(nSnap);
		CDialogHelper::SelectItemByData(m_cbSnapModes, nSnap);

		return TRUE;
	}

	// else
	return FALSE;
}

void CWorkloadWnd::OnSelchangeDisplay() 
{
	GTLC_MONTH_DISPLAY nCurDisplay = m_ctrlWorkload.GetMonthDisplay();
	GTLC_MONTH_DISPLAY nNewDisplay = (GTLC_MONTH_DISPLAY)CDialogHelper::GetSelectedItemData(m_cbDisplayOptions);

	if (!SetMonthDisplay(nNewDisplay))
	{
		// restore previous selection
		CDialogHelper::SelectItemByData(m_cbSnapModes, nCurDisplay);
	}
}

void CWorkloadWnd::OnSelchangeSnapMode() 
{
	// save snap mode as we go
	GTLC_MONTH_DISPLAY nCurDisplay = m_ctrlWorkload.GetMonthDisplay();
	GTLC_SNAPMODE nSnap = (GTLC_SNAPMODE)CDialogHelper::GetSelectedItemData(m_cbSnapModes);

	m_mapDisplaySnapModes[nCurDisplay] = nSnap;
	m_ctrlWorkload.SetSnapMode(nSnap);
}

LRESULT CWorkloadWnd::OnWorkloadNotifyZoomChange(WPARAM /*wp*/, LPARAM lp)
{
	CDialogHelper::SelectItemByData(m_cbDisplayOptions, (GTLC_MONTH_DISPLAY)lp);

	return 0L;
}

LRESULT CWorkloadWnd::OnWorkloadNotifySortChange(WPARAM /*wp*/, LPARAM lp)
{
	// notify app
	GetParent()->SendMessage(WM_IUI_SORTCOLUMNCHANGE, 0, MapColumn((GTLC_COLUMN)lp));

	return 0L;
}

void CWorkloadWnd::OnSelchangedWorkloadTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Ignore selection changes during a move because we
	// _Know_ that the logical selection does not change
	if (m_ctrlWorkload.IsMovingTask())
		return;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;

	// Ignore setting selection to 'NULL' unless there are no tasks at all
	// because we know it's temporary only
	if ((pNMTreeView->itemNew.hItem == NULL) && (m_tree.GetCount() != 0))
		return;
		
	UpdateSelectedTaskDates();

	// ignore notifications arising out of SelectTask()
	if (m_bInSelectTask && (pNMTreeView->action == TVC_UNKNOWN))
		return;

	// we're only interested in non-keyboard changes
	// because keyboard gets handled in OnKeyUpWorkload
	if (pNMTreeView->action != TVC_BYKEYBOARD)
		SendParentSelectionUpdate();
}

void CWorkloadWnd::OnBeginEditTreeLabel(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	*pResult = TRUE; // cancel our edit
	
	// notify app to edit
	GetParent()->SendMessage(WM_IUI_EDITSELECTEDTASKTITLE);
}

void CWorkloadWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CDialog::OnSetFocus(pOldWnd);
	
	m_tree.SetFocus();
}

void CWorkloadWnd::UpdateWorkloadCtrlPreferences()
{
	m_ctrlWorkload.SetOption(GTLCF_DISPLAYTRAILINGTASKTITLE, m_dlgPrefs.GetDisplayTrailingTaskTitle());
	m_ctrlWorkload.SetOption(GTLCF_DISPLAYTRAILINGALLOCTO, m_dlgPrefs.GetDisplayTrailingAllocTo());
	m_ctrlWorkload.SetOption(GTLCF_AUTOSCROLLTOTASK, m_dlgPrefs.GetAutoScrollSelection());
	m_ctrlWorkload.SetOption(GTLCF_CALCPARENTDATES, m_dlgPrefs.GetAutoCalcParentDates());
	m_ctrlWorkload.SetOption(GTLCF_CALCMISSINGSTARTDATES, m_dlgPrefs.GetCalculateMissingStartDates());
	m_ctrlWorkload.SetOption(GTLCF_CALCMISSINGDUEDATES, m_dlgPrefs.GetCalculateMissingDueDates());
	m_ctrlWorkload.SetOption(GTLCF_DISPLAYPROGRESSINBAR, m_dlgPrefs.GetDisplayProgressInBar());
	m_ctrlWorkload.SetOption(GTLCF_DECADESAREONEBASED, m_dlgPrefs.GetDecadesAreOneBased());
	m_ctrlWorkload.SetOption(GTLCF_DISPLAYPARENTROLLUPS, m_dlgPrefs.GetDisplayParentsAsRollups());

	m_ctrlWorkload.SetTodayColor(m_dlgPrefs.GetTodayColor());
	m_ctrlWorkload.SetWeekendColor(m_dlgPrefs.GetWeekendColor());
	m_ctrlWorkload.SetNonWorkingHoursColor(m_dlgPrefs.GetNonWorkingHoursColor());
	m_ctrlWorkload.SetDefaultColor(m_dlgPrefs.GetDefaultColor());
	m_ctrlWorkload.SetMilestoneTag(m_dlgPrefs.GetMilestoneTag());

	COLORREF crParent;
	GTLC_PARENTCOLORING nOption = (GTLC_PARENTCOLORING)m_dlgPrefs.GetParentColoring(crParent);

	m_ctrlWorkload.SetParentColoring(nOption, crParent);

	CDWordArray aColumnVis;
	m_dlgPrefs.GetColumnVisibility(aColumnVis);
	m_ctrlWorkload.SetTreeColumnVisibility(aColumnVis);
}

LRESULT CWorkloadWnd::OnWorkloadNotifyDateChange(WPARAM wp, LPARAM lp)
{
	COleDateTime dtStart, dtDue;
	if (m_ctrlWorkload.GetSelectedTaskDates(dtStart, dtDue))
	{
		IUITASKMOD mod[2] = { { IUI_NONE, 0 }, { IUI_NONE, 0 } };
		int nNumMod = 1;
		
		switch (wp)
		{
		case GTLCD_START:
			if (CDateHelper::GetTimeT64(dtStart, mod[0].tValue))
			{
				mod[0].nAttrib = IUI_STARTDATE;
			}
			break;
			
		case GTLCD_END:
			if (CDateHelper::GetTimeT64(dtDue, mod[0].tValue))
			{
				mod[0].nAttrib = IUI_DUEDATE;
			}
			break;
			
		case GTLCD_WHOLE:
			{
				const WorkloadITEM* pGIPreDrag = (const WorkloadITEM*)lp;
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
		}
		
		if (mod[0].nAttrib != IUI_NONE)
			return GetParent()->SendMessage(WM_IUI_MODIFYSELECTEDTASK, nNumMod, (LPARAM)&mod[0]);
	}

	return 0L;
}

void CWorkloadWnd::UpdateSelectedTaskDates()
{
	COleDateTime dtStart, dtDue;
	
	if (m_ctrlWorkload.GetSelectedTaskDates(dtStart, dtDue))
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

LRESULT CWorkloadWnd::OnWorkloadNotifyDragChange(WPARAM wp, LPARAM /*lp*/)
{
	// save snap changes as we go
	GTLC_SNAPMODE nSnap = (GTLC_SNAPMODE)wp;
	GTLC_MONTH_DISPLAY nDisplay = m_ctrlWorkload.GetMonthDisplay();

	m_mapDisplaySnapModes[nDisplay] = nSnap;

	CDialogHelper::SelectItemByData(m_cbSnapModes, nSnap);
	UpdateSelectedTaskDates();

	return 0L;
}

void CWorkloadWnd::BuildSnapCombo()
{
	m_cbSnapModes.ResetContent();

	switch (m_ctrlWorkload.GetMonthDisplay())
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

	CDialogHelper::SelectItemByData(m_cbSnapModes, m_ctrlWorkload.GetSnapMode());
}

void CWorkloadWnd::BuildDisplayCombo()
{
	GTLC_MONTH_DISPLAY nCurDisplay = m_ctrlWorkload.GetMonthDisplay();

	m_cbDisplayOptions.ResetContent();

	for (int nMode = 0; nMode < NUM_DISPLAYMODES; nMode++)
	{
		const GTCDISPLAYMODE& mode = DISPLAYMODES[nMode];

		if (!m_ctrlWorkload.CanSetMonthDisplay(mode.nDisplay))
		{
			int nCurMode = FindDisplay(nCurDisplay);

			if (nMode < nCurMode)
				nCurDisplay = mode.nDisplay;

			break;
		}

		// else
		CDialogHelper::AddString(m_cbDisplayOptions, mode.nStringID, mode.nDisplay);
	}
	
	CDialogHelper::SelectItemByData(m_cbDisplayOptions, nCurDisplay);
}

void CWorkloadWnd::OnWorkloadGotoToday() 
{
	m_ctrlWorkload.ScrollToToday();
	
	// and set focus back to it
	m_ctrlWorkload.SetFocus();
}

void CWorkloadWnd::OnUpdateWorkloadGotoToday(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CWorkloadWnd::OnWorkloadPreferences() 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (m_dlgPrefs.DoModal() == IDOK)
	{
		// update Workload control
		UpdateWorkloadCtrlPreferences();
		
		// and set focus back to it
		m_ctrlWorkload.SetFocus();
	}
}

void CWorkloadWnd::OnUpdateWorkloadPreferences(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CWorkloadWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if (!bShow)
		m_ctrlWorkload.CancelOperation();
}

void CWorkloadWnd::OnWorkloadNewDepends() 
{
	OnWorkloadDepends(GCDDM_ADD);
}

void CWorkloadWnd::OnUpdateWorkloadNewDepends(CCmdUI* pCmdUI) 
{
	OnUpdateWorkloadDepends(GCDDM_ADD, pCmdUI);
}

void CWorkloadWnd::OnWorkloadEditDepends() 
{
	OnWorkloadDepends(GCDDM_EDIT);
}

void CWorkloadWnd::OnUpdateWorkloadEditDepends(CCmdUI* pCmdUI) 
{
	OnUpdateWorkloadDepends(GCDDM_EDIT, pCmdUI);
}

void CWorkloadWnd::OnWorkloadDeleteDepends() 
{
	OnWorkloadDepends(GCDDM_DELETE);
}

void CWorkloadWnd::OnUpdateWorkloadDeleteDepends(CCmdUI* pCmdUI) 
{
	OnUpdateWorkloadDepends(GCDDM_DELETE, pCmdUI);
}

void CWorkloadWnd::OnWorkloadDepends(GCDD_MODE nMode)
{
	if (!m_bReadOnly)
	{
		ASSERT (m_dlgDepends.GetSafeHwnd() == NULL);
		
		if (m_dlgDepends.Create(nMode, this))
		{
			VERIFY (m_ctrlWorkload.BeginDependencyEdit(&m_dlgDepends));
			m_toolbar.RefreshButtonStates();
		}
	}
}

void CWorkloadWnd::OnUpdateWorkloadDepends(GCDD_MODE nMode, CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_dlgDepends.GetMode() == nMode);
	pCmdUI->Enable(!m_bReadOnly && ((m_dlgDepends.GetMode() == nMode) || (m_dlgDepends.GetMode() == GCDDM_NONE)));
}

LRESULT CWorkloadWnd::OnWorkloadDependencyDlgClose(WPARAM wp, LPARAM lp)
{
	UNREFERENCED_PARAMETER(lp);
	ASSERT(((HWND)lp) == m_dlgDepends);
	
	if (m_dlgDepends.IsPickingCompleted())
	{
		// make sure the from task is selected
		DWORD dwFromTaskID = m_dlgDepends.GetFromTask();

		if (m_ctrlWorkload.GetSelectedTaskID() != dwFromTaskID)
		{
			SelectTask(dwFromTaskID);

			// explicitly update parent because SelectTask will not
			SendParentSelectionUpdate();
		}

		CDWordArray aDepends;
		m_ctrlWorkload.GetSelectedTaskDependencies(aDepends);
		
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
				VERIFY(Misc::RemoveItem(dwPrevToTask, aDepends));
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
			// update Workload ctrl
			m_ctrlWorkload.SetSelectedTaskDependencies(aDepends);
		}
	}

	m_toolbar.RefreshButtonStates(FALSE);
	m_ctrlWorkload.OnEndDepedencyEdit();
	m_ctrlWorkload.SetFocus();

	return 0L;
}

LRESULT CWorkloadWnd::OnWorkloadNotifyCompletionChange(WPARAM /*wp*/, LPARAM lp) 
{
	IUITASKMOD mod = { IUI_DONEDATE, 0 };

	if (lp) // done/not done
		VERIFY(CDateHelper::GetTimeT64(CDateHelper::GetDate(DHD_NOW), mod.tValue));
	else
		mod.tValue = T64Utils::T64_NULL;

	return GetParent()->SendMessage(WM_IUI_MODIFYSELECTEDTASK, 1, (LPARAM)&mod);
}

LRESULT CWorkloadWnd::OnWorkloadGetTaskIcon(WPARAM wp, LPARAM lp)
{
	return GetParent()->SendMessage(WM_IUI_GETTASKICON, wp, lp);
}

LRESULT CWorkloadWnd::OnWorkloadMoveTask(WPARAM wp, LPARAM lp)
{
	return GetParent()->SendMessage(WM_IUI_MOVESELECTEDTASK, wp, lp);
}
