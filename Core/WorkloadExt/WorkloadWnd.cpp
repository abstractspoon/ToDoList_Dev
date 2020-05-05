// WorkloadWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "WorkloadExt.h"
#include "WorkloadWnd.h"
#include "WorkloadStatic.h"
#include "WorkloadMsg.h"
#include "EditAllocationsDlg.h"

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
#include "..\shared\WorkingWeek.h"

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

const int PADDING = 3;
const UINT IDC_WORKLOADCTRL = 1001;

/////////////////////////////////////////////////////////////////////////////
// CWorkloadWnd

CWorkloadWnd::CWorkloadWnd(CWnd* pParent /*=NULL*/)
	: 
	CDialog(IDD_WORKLOAD_DIALOG, pParent), 
	m_bReadOnly(FALSE),
	m_bInSelectTask(FALSE),
	m_bUpdatingSlider(FALSE),
#pragma warning(disable:4355)
	m_dlgPrefs(this)
#pragma warning(default:4355)
{
	m_icon.Load(IDR_WORKLOAD);
	m_dtPeriod.m_bInclusive = TRUE;
}

CWorkloadWnd::~CWorkloadWnd()
{
}

void CWorkloadWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWorkloadWnd)
	DDX_DateTimeCtrl(pDX, IDC_PERIODBEGIN, m_dtPeriod.m_dtStart);
	DDX_DateTimeCtrl(pDX, IDC_PERIODENDINCLUSIVE, m_dtPeriod.m_dtEnd);
	DDX_Text(pDX, IDC_PERIODDURATION, m_sPeriodDuration);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_PERIODBEGIN, m_dtcPeriodStart);
	DDX_Control(pDX, IDC_PERIODENDINCLUSIVE, m_dtcPeriodEnd);
	DDX_Control(pDX, IDC_ACTIVEDATERANGE, m_sliderDateRange);
}

BEGIN_MESSAGE_MAP(CWorkloadWnd, CDialog)
	//{{AFX_MSG_MAP(CWorkloadWnd)
	ON_COMMAND(ID_WORKLOAD_PREFS, OnWorkloadPreferences)
	ON_UPDATE_COMMAND_UI(ID_WORKLOAD_PREFS, OnUpdateWorkloadPreferences)
	ON_COMMAND(ID_WORKLOAD_EDITALLOCATIONS, OnWorkloadEditAllocations)
	ON_UPDATE_COMMAND_UI(ID_WORKLOAD_EDITALLOCATIONS, OnUpdateWorkloadEditAllocations)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_PERIODBEGIN, OnChangePeriodBegin)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_PERIODENDINCLUSIVE, OnChangePeriodEnd)

	ON_COMMAND(ID_MOVEPERIODBACKONEMONTH, OnMovePeriodBackOneMonth)
	ON_COMMAND(ID_MOVEPERIODSTARTBACKONEMONTH, OnMovePeriodStartBackOneMonth)
	ON_COMMAND(ID_MOVEPERIODSTARTFORWARDONEMONTH, OnMovePeriodStartForwardOneMonth)
	ON_COMMAND(ID_THISMONTHPERIOD, OnResetPeriodToThisMonth)
	ON_COMMAND(ID_MOVEPERIODENDFORWARDONEMONTH, OnMovePeriodEndForwardOneMonth)
	ON_COMMAND(ID_MOVEPERIODENDBACKONEMONTH, OnMovePeriodEndBackOneMonth)
	ON_COMMAND(ID_MOVEPERIODFORWARDONEMONTH, OnMovePeriodForwardOneMonth)

	ON_UPDATE_COMMAND_UI(ID_MOVEPERIODBACKONEMONTH, OnUpdateMovePeriodBackOneMonth)
	ON_UPDATE_COMMAND_UI(ID_MOVEPERIODSTARTBACKONEMONTH, OnUpdateMovePeriodStartBackOneMonth)
	ON_UPDATE_COMMAND_UI(ID_MOVEPERIODSTARTFORWARDONEMONTH, OnUpdateMovePeriodStartForwardOneMonth)
	ON_UPDATE_COMMAND_UI(ID_THISMONTHPERIOD, OnUpdateResetPeriodToThisMonth)
	ON_UPDATE_COMMAND_UI(ID_MOVEPERIODENDFORWARDONEMONTH, OnUpdateMovePeriodEndForwardOneMonth)
	ON_UPDATE_COMMAND_UI(ID_MOVEPERIODENDBACKONEMONTH, OnUpdateMovePeriodEndBackOneMonth)
	ON_UPDATE_COMMAND_UI(ID_MOVEPERIODFORWARDONEMONTH, OnUpdateMovePeriodForwardOneMonth)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, OnHelp)
	ON_WM_HELPINFO()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_NCDESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()

	ON_REGISTERED_MESSAGE(WM_WLCN_COMPLETIONCHANGE, OnWorkloadNotifyCompletionChange)
	ON_REGISTERED_MESSAGE(WM_WLCN_SORTCHANGE, OnWorkloadNotifySortChange)
	ON_REGISTERED_MESSAGE(WM_TLC_ITEMSELCHANGE, OnWorkloadNotifySelChange)
	ON_REGISTERED_MESSAGE(RANGE_CHANGED, OnActiveDateRangeChange)

	ON_REGISTERED_MESSAGE(WM_WLC_EDITTASKICON, OnWorkloadEditTaskIcon)
	ON_REGISTERED_MESSAGE(WM_WLC_EDITTASKTITLE, OnWorkloadEditTaskTitle)
	ON_REGISTERED_MESSAGE(WM_WLC_EDITTASKALLOCATIONS, OnWorkloadEditTaskAllocations)
	ON_REGISTERED_MESSAGE(WM_WLC_PREFSHELP, OnWorkloadPrefsHelp)
	ON_REGISTERED_MESSAGE(WM_WLC_GETTASKICON, OnWorkloadGetTaskIcon)
	ON_REGISTERED_MESSAGE(WM_WLC_MOVETASK, OnWorkloadMoveTask)
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
	
	if (CDialog::Create(IDD_WORKLOAD_DIALOG, pParentWnd))
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
	
	// NOTE: Most of sort is handled by the app
	CString sKey(szKey);

	pPrefs->WriteProfileString(sKey, _T("SortByAllocTo"), m_ctrlWorkload.GetSortByAllocTo());
	pPrefs->WriteProfileDouble(sKey, _T("PeriodBegin"), m_dtPeriod.m_dtStart.m_dt);
	pPrefs->WriteProfileDouble(sKey, _T("PeriodEnd"), m_dtPeriod.m_dtEnd.m_dt);

	// column widths
	CIntArray aTreeOrder, aTreeWidths, aTreeTracked;

	m_ctrlWorkload.GetTreeColumnOrder(aTreeOrder);
	m_ctrlWorkload.GetTreeColumnWidths(aTreeWidths);
	m_ctrlWorkload.GetTreeTrackedColumns(aTreeTracked);

	SaveColumnState(pPrefs, (sKey + _T("\\TreeOrder")), aTreeOrder);
	SaveColumnState(pPrefs, (sKey + _T("\\TreeWidths")), aTreeWidths);
	SaveColumnState(pPrefs, (sKey + _T("\\TreeTracked")), aTreeTracked);

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

void CWorkloadWnd::InitWorkingWeek(const IPreferences* pPrefs)
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

void CWorkloadWnd::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey, bool bAppOnly)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// application preferences
	InitWorkingWeek(pPrefs);

	m_ctrlWorkload.SetOption(WLCF_TASKTEXTCOLORISBKGND, pPrefs->GetProfileInt(_T("Preferences"), _T("ColorTaskBackground"), FALSE));
	m_ctrlWorkload.SetOption(WLCF_TREATSUBCOMPLETEDASDONE, pPrefs->GetProfileInt(_T("Preferences"), _T("TreatSubCompletedAsDone"), FALSE));
	m_ctrlWorkload.SetOption(WLCF_STRIKETHRUDONETASKS, pPrefs->GetProfileInt(_T("Preferences"), _T("StrikethroughDone"), TRUE));
	m_ctrlWorkload.SetOption(WLCF_DISPLAYISODATES, pPrefs->GetProfileInt(_T("Preferences"), _T("DisplayDatesInISO"), FALSE));
	m_ctrlWorkload.SetOption(WLCF_SHOWSPLITTERBAR, (pPrefs->GetProfileInt(_T("Preferences"), _T("HidePaneSplitBar"), TRUE) == FALSE));
	m_ctrlWorkload.SetOption(WLCF_ALLOWPARENTALLOCATIONS, pPrefs->GetProfileInt(_T("Preferences"), _T("AllowParentTimeTracking"), TRUE));

	m_ctrlWorkload.EnableTreeCheckboxes(IDB_CHECKBOXES, pPrefs->GetProfileInt(_T("Preferences"), _T("AllowCheckboxAgainstTreeItem"), TRUE));
	m_ctrlWorkload.EnableTreeLabelTips(!pPrefs->GetProfileInt(_T("Preferences"), _T("ShowInfoTips"), FALSE));

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

	// Workload specific options
	if (!bAppOnly)
	{
		// NOTE: Most of sort is handled by the app
		CString sKey(szKey);
		
		m_dlgPrefs.LoadPreferences(pPrefs, sKey);
		UpdateWorkloadCtrlPreferences();

		m_ctrlWorkload.SetSortByAllocTo(pPrefs->GetProfileString(sKey, _T("SortByAllocTo")));

		// Current period
		m_dtPeriod.m_dtStart = pPrefs->GetProfileDouble(sKey, _T("PeriodBegin"), CDateHelper::GetDate(DHD_BEGINTHISMONTH));
		m_dtPeriod.m_dtEnd = pPrefs->GetProfileDouble(sKey, _T("PeriodEnd"), CDateHelper::GetDate(DHD_ENDTHISMONTH));

		UpdatePeriod();
		UpdateRangeSlider();

		// column order
		CIntArray aTreeOrder, aTreeWidths, aTreeTracked;

		if (LoadColumnState(pPrefs, (sKey + _T("\\TreeOrder")), aTreeOrder))
		{
			m_ctrlWorkload.SetTreeColumnOrder(aTreeOrder);
		}
		
		// column widths
		if (LoadColumnState(pPrefs, (sKey + _T("\\TreeWidths")), aTreeWidths) &&
			m_ctrlWorkload.SetTreeColumnWidths(aTreeWidths))
		{
			m_ctrlWorkload.AdjustSplitterToFitTreeColumns();
		}
		else
		{
			m_ctrlWorkload.ResizeListColumnsToFit();
		}
		
		// column tracking
		if (LoadColumnState(pPrefs, (sKey + _T("\\TreeTracked")), aTreeTracked))
		{
			m_ctrlWorkload.SetTrackedTreeColumns(aTreeTracked);
		}
		
		if (GetSafeHwnd())
			UpdateData(FALSE);
	}
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
		m_ctrlWorkload.SetBackgroundColor(m_theme.crAppBackLight);

		m_sliderDateRange.SetParentBackgroundColor(m_theme.crAppBackLight);
	}
}

void CWorkloadWnd::FilterToolTipMessage(MSG* pMsg) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_ctrlWorkload.FilterToolTipMessage(pMsg);
}

bool CWorkloadWnd::ProcessMessage(MSG* pMsg) 
{
	if (!IsWindowEnabled())
		return false;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

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
		m_ctrlWorkload.CWnd::ClientToScreen(pEdit);
		return true;
	}

	return false;
}

IUI_HITTEST CWorkloadWnd::HitTest(POINT ptScreen) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// try tree header
	if (m_ctrlWorkload.PointInHeader(ptScreen))
		return IUI_NOWHERE;//IUI_COLUMNHEADER;

	// then specific task
	if (m_ctrlWorkload.HitTestTask(ptScreen, false))
		return IUI_TASK;

	// else 
	return IUI_NOWHERE;
}

DWORD CWorkloadWnd::HitTestTask(POINT ptScreen, bool bTitleColumnOnly) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_ctrlWorkload.HitTestTask(ptScreen, bTitleColumnOnly);
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

bool CWorkloadWnd::WantTaskUpdate(TDC_ATTRIBUTE nAttribute) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (CWorkloadCtrl::WantEditUpdate(nAttribute) != FALSE);
}

void CWorkloadWnd::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_ctrlWorkload.UpdateTasks(pTasks, nUpdate);
	m_toolbar.RefreshButtonStates(FALSE);
	
	UpdatePeriod();
	UpdateRangeSlider();
}

bool CWorkloadWnd::DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData) 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_toolbar.RefreshButtonStates(FALSE);

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

	case IUI_SORT:
		if (pData)
		{
			m_ctrlWorkload.Sort(MapColumn(pData->nSortBy), (pData->bSortAscending ? TRUE : FALSE));
			return true;
		}
		break;

	case IUI_MULTISORT:
		if (pData)
		{
			WORKLOADSORTCOLUMNS sort;

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
		m_ctrlWorkload.ResizeListColumnsToFit(TRUE);
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

			return (m_ctrlWorkload.MoveSelectedTask(pData->move) != FALSE);
		}
		break;
	}

	return false;
}

void CWorkloadWnd::SetTaskFont(HFONT hFont)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHoldRedraw hr(*this);
	
	m_ctrlWorkload.SetFont(hFont, TRUE);
}

bool CWorkloadWnd::CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData) const 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	switch (nCmd)
	{
	case IUI_EXPANDALL:
		return (m_ctrlWorkload.CanExpandAll() != FALSE);
		
	case IUI_COLLAPSEALL:
		return (m_ctrlWorkload.CanCollapseAll() != FALSE);
		
	case IUI_RESIZEATTRIBCOLUMNS:
	case IUI_SELECTTASK:
		return true;

	case IUI_SAVETOIMAGE:
		return (m_ctrlWorkload.GetTaskCount() > 0);

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

	case IUI_SORT:
		if (pData)
			return (CWorkloadCtrl::WantSortUpdate(pData->nSortBy) != FALSE);
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
			return (m_ctrlWorkload.CanMoveSelectedTask(pData->move) != FALSE);
		break;
	}

	// all else
	return false;
}

WLC_COLUMNID CWorkloadWnd::MapColumn(DWORD dwColumn)
{
	return CWorkloadCtrl::MapAttributeToColumn((TDC_ATTRIBUTE)dwColumn);
}

DWORD CWorkloadWnd::MapColumn(WLC_COLUMNID nColumn)
{
	return (DWORD)CWorkloadCtrl::MapColumnToAttribute(nColumn);
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
		
	CRect rCtrl = CDialogHelper::GetCtrlRect(this, IDC_WORKLOAD_FRAME);
	VERIFY(m_ctrlWorkload.Create(this, rCtrl, IDC_WORKLOADCTRL));

 	m_ctrlWorkload.SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWorkloadWnd::Resize(int cx, int cy)
{
	if (m_ctrlWorkload.GetSafeHwnd())
	{
		CRect rWorkload = CDialogHelper::GetChildRect(&m_ctrlWorkload);
		rWorkload.right = cx;
		rWorkload.bottom = cy;

		m_ctrlWorkload.MoveWindow(rWorkload);

		CRect rSlider = CDialogHelper::GetChildRect(&m_sliderDateRange);
		rSlider.right = (cx - 10);

		m_sliderDateRange.MoveWindow(rSlider);
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
	// clip out our children
	CDialogHelper::ExcludeChild(&m_toolbar, pDC);
	CDialogHelper::ExcludeChild(&m_ctrlWorkload, pDC);
	CDialogHelper::ExcludeChild(&m_dtcPeriodStart, pDC);
	CDialogHelper::ExcludeChild(&m_dtcPeriodEnd, pDC);
	CDialogHelper::ExcludeChild(&m_sliderDateRange, pDC);

	CDialogHelper::ExcludeCtrl(this, IDC_ACTIVEDATERANGE_LABEL, pDC);

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

void CWorkloadWnd::SendParentSelectionUpdate()
{
	DWORD dwTaskID = m_ctrlWorkload.GetSelectedTaskID();
	GetParent()->SendMessage(WM_IUI_SELECTTASK, 0, dwTaskID);
}

LRESULT CWorkloadWnd::OnWorkloadNotifySortChange(WPARAM wp, LPARAM lp)
{
	// notify app
	GetParent()->SendMessage(WM_IUI_SORTCOLUMNCHANGE, wp, MapColumn((WLC_COLUMNID)lp));

	return 0L;
}

LRESULT CWorkloadWnd::OnWorkloadNotifySelChange(WPARAM /*wp*/, LPARAM /*lp*/) 
{
	SendParentSelectionUpdate();

	m_toolbar.RefreshButtonStates(FALSE);

	return 0L;
}

LRESULT CWorkloadWnd::OnWorkloadEditTaskTitle(WPARAM /*wp*/, LPARAM /*lp*/)
{
	ASSERT(!m_bReadOnly);

	// notify app to edit
	GetParent()->SendMessage(WM_IUI_EDITSELECTEDTASKTITLE);

	return 0L;
}

void CWorkloadWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CDialog::OnSetFocus(pOldWnd);
	
	m_ctrlWorkload.SetFocus();
}

void CWorkloadWnd::UpdateWorkloadCtrlPreferences()
{
	CDWordArray aColumnVis;
	m_dlgPrefs.GetColumnVisibility(aColumnVis);
	m_ctrlWorkload.SetTreeColumnVisibility(aColumnVis);

	int nPercent;
	COLORREF color;
	
	BOOL bEnable = m_dlgPrefs.GetOverload(nPercent, color);
	m_ctrlWorkload.EnableOverload(bEnable, nPercent, color);

	bEnable = m_dlgPrefs.GetUnderload(nPercent, color);
	m_ctrlWorkload.EnableUnderload(bEnable, nPercent, color);

	m_ctrlWorkload.SetOverlapColor(m_dlgPrefs.GetOverlapColor());

	m_ctrlWorkload.SetOption(WLCF_CALCMISSINGALLOCATIONS, m_dlgPrefs.GetAutoCalculateMissingAllocations());
	m_ctrlWorkload.SetOption(WLCF_PREFERTIMEESTFORCALCS, m_dlgPrefs.GetPreferTimeEstimateForCalcs());
	m_ctrlWorkload.SetOption(WLCF_RECALCALLOCATIONS, m_dlgPrefs.GetRecalculateAllocations());
	m_ctrlWorkload.SetOption(WLCF_RECALCPROPORTIONALLY, m_dlgPrefs.GetRecalculateAllocationsProportionally());
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

LRESULT CWorkloadWnd::OnWorkloadNotifyCompletionChange(WPARAM /*wp*/, LPARAM lp) 
{
	ASSERT(!m_bReadOnly);

	IUITASKMOD mod = { TDCA_DONEDATE, 0 };

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

LRESULT CWorkloadWnd::OnWorkloadEditTaskIcon(WPARAM wp, LPARAM lp)
{
	return GetParent()->SendMessage(WM_IUI_EDITSELECTEDTASKICON, wp, lp);
}

LRESULT CWorkloadWnd::OnWorkloadMoveTask(WPARAM wp, LPARAM lp)
{
	return GetParent()->SendMessage(WM_IUI_MOVESELECTEDTASK, wp, lp);
}

void CWorkloadWnd::EditSelectedWorkloadAllocations(LPCTSTR szAllocTo)
{
	ASSERT(CanEditSelectedTaskAllocations());

	WORKLOADITEM wi;
	VERIFY(m_ctrlWorkload.GetSelectedTask(wi));

	CEditAllocationsDlg dialog(wi, m_ctrlWorkload.GetAllocatedToList(), szAllocTo);
	
	if (dialog.DoModal() == IDOK)
	{
		const WORKLOADITEM& wiNew = dialog.GetAllocations();

		if (!wiNew.mapAllocatedDays.MatchAll(wi.mapAllocatedDays))
		{
			IUITASKMOD mod[2] = { { TDCA_METADATA, 0 }, { TDCA_ALLOCTO, 0 } };
			int nNumMods = 0;

			// always
			CString sMetaData = wiNew.mapAllocatedDays.Encode();
			mod[nNumMods++].szValue = sMetaData;

			// sometimes
			CString sAllocTo;

			if (!Misc::MatchAll(wi.aAllocTo, wiNew.aAllocTo))
			{
				sAllocTo = Misc::FormatArray(wiNew.aAllocTo, '\n');
				mod[nNumMods++].szValue = sAllocTo;
			}

			if (nNumMods)
			{
				if (GetParent()->SendMessage(WM_IUI_MODIFYSELECTEDTASK, nNumMods, (LPARAM)&mod[0]))
				{
					m_ctrlWorkload.SetSelectedTaskAllocations(wiNew);
				}
			}
		}
	}
}

void CWorkloadWnd::OnWorkloadEditAllocations() 
{
	EditSelectedWorkloadAllocations(NULL);
}

LRESULT CWorkloadWnd::OnWorkloadEditTaskAllocations(WPARAM wp, LPARAM lp)
{
	ASSERT(!m_bReadOnly);

	if (CanEditSelectedTaskAllocations((DWORD)lp))
	{
		EditSelectedWorkloadAllocations((LPCTSTR)wp);
		return TRUE;
	}

	// else
	ASSERT(0);
	return FALSE;
}

void CWorkloadWnd::OnUpdateWorkloadEditAllocations(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanEditSelectedTaskAllocations());
}

BOOL CWorkloadWnd::CanEditSelectedTaskAllocations(DWORD dwTaskID) const
{
	if (m_bReadOnly || !m_ctrlWorkload.GetSafeHwnd() || !dwTaskID)
		return FALSE;

	if (dwTaskID != m_ctrlWorkload.GetSelectedTaskID())
	{
		ASSERT(0);
		return FALSE;
	}

	// else
	WORKLOADITEM wi;

	if (!m_ctrlWorkload.GetSelectedTask(wi))
	{
		ASSERT(0);
		return FALSE;
	}
	
	return (!wi.bParent || m_ctrlWorkload.HasOption(WLCF_ALLOWPARENTALLOCATIONS));
}

void CWorkloadWnd::OnChangePeriodBegin(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	UpdateData();
	UpdatePeriod();
	UpdateRangeSlider();
	
	*pResult = 0;
}

void CWorkloadWnd::OnChangePeriodEnd(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	UpdateData();
	UpdatePeriod();
	UpdateRangeSlider();
	
	*pResult = 0;
}

void CWorkloadWnd::OnMovePeriodBackOneMonth() 
{
	m_dtPeriod.Offset(-1, DHU_MONTHS);

	UpdatePeriod();
	UpdateRangeSlider();
}

LRESULT CWorkloadWnd::OnActiveDateRangeChange(WPARAM /*wp*/, LPARAM /*lp*/)
{
	if (!m_bUpdatingSlider)
	{
		m_dtPeriod.Set(m_sliderDateRange.GetLeft(), m_sliderDateRange.GetRight());
		UpdatePeriod();
	}

	return 0L;
}

void CWorkloadWnd::UpdatePeriod()
{
	ValidatePeriod();
	m_ctrlWorkload.SetCurrentPeriod(m_dtPeriod);

	if (m_dtPeriod.IsValid())
		m_sPeriodDuration = CEnString(IDS_PERIODDURATION, m_dtPeriod.GetWeekdayCount());
	else
		m_sPeriodDuration.Empty();

	m_toolbar.RefreshButtonStates(FALSE);
	SetDlgItemText(IDC_ACTIVEDATERANGE_LABEL, CEnString(IDS_ACTIVEDATERANGE, m_dtPeriod.Format()));

	UpdateData(FALSE);
}

void CWorkloadWnd::UpdateRangeSlider()
{
	CAutoFlag af(m_bUpdatingSlider, TRUE);
	const COleDateTimeRange& dtData = m_ctrlWorkload.GetDataDateRange();

	if (dtData.IsValid())
	{
		m_sliderDateRange.SetMinMax(dtData.GetStart(), dtData.GetEnd());

		if (dtData.Contains(m_dtPeriod))
		{
			m_sliderDateRange.SetRange(m_dtPeriod.GetStart(), m_dtPeriod.GetEnd());
			m_sliderDateRange.EnableWindow(TRUE);
		}
		else
		{
			m_sliderDateRange.SetRange(dtData.GetStart(), dtData.GetEnd());
			m_sliderDateRange.EnableWindow(FALSE);
		}
	}
	else
	{
		m_sliderDateRange.SetRange(0.0, 1.0);
		m_sliderDateRange.EnableWindow(FALSE);
	}

	m_sliderDateRange.SetStep(1.0); // 1 day
}

void CWorkloadWnd::OnUpdateMovePeriodBackOneMonth(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanMovePeriodStartBackwards());
}

void CWorkloadWnd::OnMovePeriodStartForwardOneMonth() 
{
	if (m_dtPeriod.OffsetStart(1, DHU_MONTHS))
	{
		UpdatePeriod();
		UpdateRangeSlider();
	}
}

void CWorkloadWnd::OnUpdateMovePeriodStartForwardOneMonth(CCmdUI* pCmdUI) 
{
	COleDateTimeRange temp = m_dtPeriod;

	pCmdUI->Enable(temp.IsValid() && temp.OffsetStart(1, DHU_MONTHS));
}

void CWorkloadWnd::OnMovePeriodStartBackOneMonth() 
{
	if (m_dtPeriod.OffsetStart(-1, DHU_MONTHS))
	{
		UpdatePeriod();
		UpdateRangeSlider();
	}
}

void CWorkloadWnd::OnUpdateMovePeriodStartBackOneMonth(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanMovePeriodStartBackwards());
}

void CWorkloadWnd::OnResetPeriodToThisMonth() 
{
	m_dtPeriod.Set(DHD_BEGINTHISMONTH, DHD_ENDTHISMONTH);
	
	UpdatePeriod();
	UpdateRangeSlider();
}

void CWorkloadWnd::OnUpdateResetPeriodToThisMonth(CCmdUI* pCmdUI) 
{
	COleDateTimeRange dtTemp(DHD_BEGINTHISMONTH, DHD_ENDTHISMONTH);

	pCmdUI->Enable(m_ctrlWorkload.GetDataDateRange().Contains(dtTemp));
}

void CWorkloadWnd::OnMovePeriodEndForwardOneMonth() 
{
	if (m_dtPeriod.OffsetEnd(1, DHU_MONTHS))
	{
		UpdatePeriod();
		UpdateRangeSlider();
	}
}

void CWorkloadWnd::OnUpdateMovePeriodForwardOneMonth(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanMovePeriodEndForwards());
}

void CWorkloadWnd::OnMovePeriodEndBackOneMonth() 
{
	if (m_dtPeriod.OffsetEnd(-1, DHU_MONTHS))
	{
		UpdatePeriod();
		UpdateRangeSlider();
	}
}

void CWorkloadWnd::OnUpdateMovePeriodEndBackOneMonth(CCmdUI* pCmdUI) 
{
	COleDateTimeRange temp = m_dtPeriod;

	pCmdUI->Enable(temp.IsValid() && temp.OffsetEnd(-1, DHU_MONTHS));
}

void CWorkloadWnd::OnMovePeriodForwardOneMonth() 
{
	m_dtPeriod.Offset(1, DHU_MONTHS);

	UpdatePeriod();
	UpdateRangeSlider();
}

void CWorkloadWnd::OnUpdateMovePeriodEndForwardOneMonth(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CanMovePeriodEndForwards());
}

BOOL CWorkloadWnd::CanMovePeriodEndForwards() const
{
	return (m_dtPeriod.IsValid() && 
			m_ctrlWorkload.GetDataDateRange().IsValid() &&
			(m_dtPeriod.GetEnd() < m_ctrlWorkload.GetDataDateRange().GetEnd()));
}

BOOL CWorkloadWnd::CanMovePeriodStartBackwards() const
{
	return (m_dtPeriod.IsValid() && 
			m_ctrlWorkload.GetDataDateRange().IsValid() &&
			(m_dtPeriod.GetStart() > m_ctrlWorkload.GetDataDateRange().GetStart()));
}

BOOL CWorkloadWnd::ValidatePeriod()
{
	// Prevent current period from leaving data date range
	const COleDateTimeRange& dtData = m_ctrlWorkload.GetDataDateRange();

	if (!dtData.IsValid())
		return FALSE;

	if (!m_dtPeriod.IsValid())
	{
		m_dtPeriod = dtData;
		return TRUE;
	}

	int nDiff = (int)(dtData.GetStart() - m_dtPeriod.GetStart());

	if (nDiff > 0)
	{
		m_dtPeriod.Offset(nDiff, DHU_DAYS);
		m_dtPeriod.GetIntersection(m_dtPeriod, dtData);

		return TRUE;
	}

	nDiff = (int)(dtData.GetEnd() - m_dtPeriod.GetEnd());

	if (nDiff < 0)
	{
		m_dtPeriod.Offset(nDiff, DHU_DAYS);
		m_dtPeriod.GetIntersection(m_dtPeriod, dtData);
		
		return TRUE;
	}

	// no change
	return FALSE;
}
