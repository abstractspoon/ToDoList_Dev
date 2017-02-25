// CalendarFrameWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "CalendarExt.h"
#include "CalendarWnd.h"
#include "CalMsg.h"

#include "..\Shared\DialogHelper.h"
#include "..\Shared\DateHelper.h"
#include "..\Shared\TimeHelper.h"
#include "..\Shared\FileMisc.h"
#include "..\Shared\themed.h"
#include "..\Shared\dlgunits.h"
#include "..\shared\misc.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\localizer.h"

#include "..\Interfaces\UITheme.h"
#include "..\Interfaces\IPreferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const UINT IDC_BIG_CALENDAR = 101;
const UINT IDC_MINI_CALENDAR = 102;

/////////////////////////////////////////////////////////////////////////////

const COLORREF DEF_DONECOLOR		= RGB(128, 128, 128);

/////////////////////////////////////////////////////////////////////////////
// CCalendarWnd

IMPLEMENT_DYNAMIC(CCalendarWnd, CDialog)

CCalendarWnd::CCalendarWnd()
	:	
	m_hIcon(AfxGetApp()->LoadIcon(IDR_CALENDAR)),
	m_bReadOnly(FALSE),
#pragma warning(disable:4355)
	m_dlgPrefs(this)
#pragma warning(default:4355)
{
}

CCalendarWnd::~CCalendarWnd()
{
}

void CCalendarWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_NUMWEEKS, m_cbNumWeeks);
	DDX_Control(pDX, IDC_SNAPMODES, m_cbSnapModes);
	DDX_Text(pDX, IDC_SELECTEDTASKDATES, m_sSelectedTaskDates);
}

BEGIN_MESSAGE_MAP(CCalendarWnd, CDialog)
	//{{AFX_MSG_MAP(CCalendarWnd)
	ON_WM_SETFOCUS()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_COMMAND(ID_HELP, OnHelp)
	ON_WM_HELPINFO()
	ON_COMMAND(ID_CAL_GOTOTODAY, OnGototoday)
	ON_COMMAND(ID_CAL_PREFS, OnPreferences)
	ON_CBN_SELCHANGE(IDC_NUMWEEKS, OnSelChangeNumWeeks)
	ON_CBN_SELCHANGE(IDC_SNAPMODES, OnSelChangeSnapMode)
	ON_NOTIFY(NM_CLICK, IDC_MINI_CALENDAR, OnMiniCalendarNotifyClick)
	ON_NOTIFY(NM_CLICK, IDC_BIG_CALENDAR, OnBigCalendarNotifyClick)
	ON_NOTIFY(NM_DBLCLK, IDC_MINI_CALENDAR, OnMiniCalendarNotifyDblClk)
	ON_NOTIFY(NM_DBLCLK, IDC_BIG_CALENDAR, OnBigCalendarNotifyDblClk)
	ON_REGISTERED_MESSAGE(WM_CALENDAR_DATECHANGE, OnBigCalendarNotifyDateChange)
	ON_REGISTERED_MESSAGE(WM_CALENDAR_SELCHANGE, OnBigCalendarNotifySelectionChange)
	ON_REGISTERED_MESSAGE(WM_CALENDAR_DRAGCHANGE, OnBigCalendarNotifyDragChange)
	ON_REGISTERED_MESSAGE(WM_CALENDAR_VISIBLEWEEKCHANGE, OnBigCalendarNotifyVisibleWeekChange)
	ON_REGISTERED_MESSAGE(WM_CALENDAR_PREFSHELP, OnBigCalendarPrefsHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalendarWnd message handlers

void CCalendarWnd::OnHelp()
{
	GetParent()->SendMessage(WM_IUI_DOHELP, 0, (LPARAM)GetTypeID());
}

BOOL CCalendarWnd::OnHelpInfo(HELPINFO* /*lpHelpInfo*/)
{
	OnHelp();
	return TRUE;
}

LRESULT CCalendarWnd::OnBigCalendarPrefsHelp(WPARAM /*wp*/, LPARAM /*lp*/)
{
	CString sHelpID(GetTypeID());
	sHelpID += _T("_PREFS");
	
	GetParent()->SendMessage(WM_IUI_DOHELP, 0, (LPARAM)(LPCTSTR)sHelpID);
	return 0L;
}

void CCalendarWnd::SetReadOnly(bool bReadOnly)
{
	m_bReadOnly = bReadOnly;

	m_BigCalendar.SetReadOnly(bReadOnly);
}

BOOL CCalendarWnd::Create(DWORD dwStyle, const RECT &/*rect*/, CWnd* pParentWnd, UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (CDialog::Create(IDD_CALENDAR_DIALOG, pParentWnd))
	{
		SetWindowLong(*this, GWL_STYLE, dwStyle);
		SetDlgCtrlID(nID);

		return TRUE;
	}

	return FALSE;
}

BOOL CCalendarWnd::OnInitDialog() 
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

	// create mini-calendar ctrl
	m_MiniCalendar.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | FMC_NO3DBORDER | FMC_SHOWWEEKNUMBERS, 
                          CRect(0,0,0,0), this, IDC_MINI_CALENDAR);

    m_MiniCalendar.SetDate(COleDateTime::GetCurrentTime());
    m_MiniCalendar.SetRowsAndColumns(3, 1);
	m_MiniCalendar.SetSpecialDaysCallback(CCalendarWnd::IsMiniCalSpecialDateCallback, (DWORD)this);
	m_MiniCalendar.SetFirstWeekDay(CDateHelper::GetFirstDayOfWeek());

	// create big-calendar ctrl
	m_BigCalendar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, CRect(0,0,0,0), this, IDC_BIG_CALENDAR);
	m_BigCalendar.EnableMultiSelection(FALSE);
	m_BigCalendar.SetFirstWeekDay(CDateHelper::GetFirstDayOfWeek());

	m_cbNumWeeks.SetCurSel(m_BigCalendar.GetVisibleWeeks() - 1);

	// build snap combobox
	InitSnapCombo();

	return 0;
}

void CCalendarWnd::InitSnapCombo()
{
	CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTHOUR,	TCCSM_NEARESTHOUR);
	CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTDAY,	TCCSM_NEARESTDAY);
	CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_NEARESTHALFDAY, TCCSM_NEARESTHALFDAY);
	CDialogHelper::AddString(m_cbSnapModes, IDS_SNAP_FREE,			TCCSM_FREE);

	CDialogHelper::SelectItemByData(m_cbSnapModes, m_BigCalendar.GetSnapMode());
}

void CCalendarWnd::OnSelChangeNumWeeks()
{
	m_BigCalendar.SetVisibleWeeks(m_cbNumWeeks.GetCurSel() + 1);

	CRect rClient;
	GetClientRect(rClient);
	ResizeControls(rClient.Width(), rClient.Height());
}

void CCalendarWnd::OnSelChangeSnapMode()
{
	m_BigCalendar.SetSnapMode((TCC_SNAPMODE)CDialogHelper::GetSelectedItemData(m_cbSnapModes));
}

void CCalendarWnd::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_dlgPrefs.SavePreferences(pPrefs, szKey);

	pPrefs->WriteProfileInt(szKey, _T("NumWeeks"), m_BigCalendar.GetVisibleWeeks());
}

void CCalendarWnd::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey, bool bAppOnly) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// app preferences
	m_BigCalendar.SetOption(TCCO_TASKTEXTCOLORISBKGND, pPrefs->GetProfileInt(_T("Preferences"), _T("ColorTaskBackground"), FALSE));
	m_BigCalendar.SetOption(TCCO_STRIKETHRUDONETASKS, pPrefs->GetProfileInt(_T("Preferences"), _T("StrikethroughDone"), TRUE));

	// calendar specific preferences
	if (!bAppOnly)
	{
		CString sKey(szKey);

		m_dlgPrefs.LoadPreferences(pPrefs, sKey);
		UpdateCalendarCtrlPreferences();

		int nWeeks = pPrefs->GetProfileInt(sKey, _T("NumWeeks"), 6);
		nWeeks = max(0, min(nWeeks, 9));
		
		if (m_BigCalendar.SetVisibleWeeks(nWeeks))
			m_cbNumWeeks.SetCurSel(nWeeks-1);

		// make sure 'today' is visible
		COleDateTime dtToday = COleDateTime::GetCurrentTime();

		m_MiniCalendar.SetDate(dtToday);
		m_BigCalendar.GotoToday();
	}
}

void CCalendarWnd::UpdateCalendarCtrlPreferences()
{
	m_BigCalendar.SetOption(TCCO_CALCMISSINGSTARTASCREATION, m_dlgPrefs.GetCalcMissingStartAsCreation());
	m_BigCalendar.SetOption(TCCO_CALCMISSINGSTARTASDUE, m_dlgPrefs.GetCalcMissingStartAsDue());
	m_BigCalendar.SetOption(TCCO_CALCMISSINGSTARTASEARLIESTDUEANDTODAY, m_dlgPrefs.GetCalcMissingStartAsEarliestDueAndToday());
	m_BigCalendar.SetOption(TCCO_CALCMISSINGDUEASSTART, m_dlgPrefs.GetCalcMissingDueAsStart());
	m_BigCalendar.SetOption(TCCO_CALCMISSINGDUEASLATESTSTARTANDTODAY, m_dlgPrefs.GetCalcMissingDueAsLatestStartAndToday());
	m_BigCalendar.SetOption(TCCO_DISPLAYCONTINUOUS, m_dlgPrefs.GetDisplayAsContinuous());
	m_BigCalendar.SetOption(TCCO_DISPLAYSTART, m_dlgPrefs.GetDisplayStart());
	m_BigCalendar.SetOption(TCCO_DISPLAYDUE, m_dlgPrefs.GetDisplayDue());
	m_BigCalendar.SetOption(TCCO_DISPLAYDONE, m_dlgPrefs.GetDisplayDone());
	m_BigCalendar.SetOption(TCCO_DISPLAYCALCSTART, m_dlgPrefs.GetDisplayCalcStart());
	m_BigCalendar.SetOption(TCCO_DISPLAYCALCDUE, m_dlgPrefs.GetDisplayCalcDue());
	m_BigCalendar.SetOption(TCCO_ADJUSTTASKHEIGHTS, m_dlgPrefs.GetAdjustTaskHeights());
}

void CCalendarWnd::SetUITheme(const UITHEME* pTheme)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	GraphicsMisc::VerifyDeleteObject(m_brBack);

	if (CThemed::IsAppThemed() && pTheme)
	{
		m_theme = *pTheme;
		m_brBack.CreateSolidBrush(pTheme->crAppBackLight);

		m_BigCalendar.SetThemeColour(pTheme->crAppBackDark);
		m_MiniCalendar.SetHighlightToday(TRUE, pTheme->crAppLinesDark);
		
		// intentionally set background colours to be same as ours
		m_toolbar.SetBackgroundColors(m_theme.crAppBackLight, m_theme.crAppBackLight, FALSE, FALSE);
		m_toolbar.SetHotColor(m_theme.crToolbarHot);
	}
}

LPCTSTR CCalendarWnd::GetMenuText() const
{
	return _T("Calendar");
}

bool CCalendarWnd::ProcessMessage(MSG* pMsg) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (!IsWindowEnabled())
		return false;

	if (m_BigCalendar.ProcessMessage(pMsg))
		return true;

	return false;
}

bool CCalendarWnd::DoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra) 
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
	case IUI_RESIZEATTRIBCOLUMNS:
	case IUI_GETNEXTTASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTASK:
	case IUI_GETPREVTOPLEVELTASK:
		// not handled
		break;
		
	case IUI_SELECTTASK:
		return SelectTask(dwExtra);
		
	case IUI_SETFOCUS:
		m_BigCalendar.SetFocus();
		return true;
	}

	return false;
}

bool CCalendarWnd::CanDoAppCommand(IUI_APPCOMMAND nCmd, DWORD /*dwExtra*/) const 
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
	case IUI_RESIZEATTRIBCOLUMNS:
	case IUI_GETNEXTTASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTASK:
	case IUI_GETPREVTOPLEVELTASK:
		// not handled
		break;

	case IUI_SETFOCUS:
	case IUI_SELECTTASK:
		return true;
	}
	
	return false;
}

bool CCalendarWnd::GetLabelEditRect(LPRECT pEdit)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	DWORD dwSelTask = m_BigCalendar.GetSelectedTaskID();
	m_BigCalendar.EnsureVisible(dwSelTask, FALSE);

	CRect rLabel;

	if (m_BigCalendar.GetTaskLabelRect(dwSelTask, rLabel))
	{
		m_BigCalendar.ClientToScreen(rLabel);

		// make sure it has a minimum height
		rLabel.bottom = (rLabel.top + max(rLabel.Height(), (m_BigCalendar.GetDefaultTaskHeight() - 1)));
		*pEdit = rLabel;

		return true;
	}

	// else
	return false;
}

IUI_HITTEST CCalendarWnd::HitTest(const POINT& ptScreen) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// try header
	// 6.9: disable header click because it changes the 
	// tree/list columns not the calendar columns
	if (m_BigCalendar.PtInHeader(ptScreen))
		return IUI_NOWHERE;//IUI_COLUMNHEADER;

	// then specific task
	CPoint ptBigCal(ptScreen);
	m_BigCalendar.ScreenToClient(&ptBigCal);

	if (m_BigCalendar.HitTest(ptBigCal))
		return IUI_TASK;

	// else try rest of big cal
	CRect rCal;
	m_BigCalendar.GetClientRect(rCal);

	return (rCal.PtInRect(ptBigCal) ? IUI_TASKLIST : IUI_NOWHERE);
}

bool CCalendarWnd::SelectTask(DWORD dwTaskID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DWORD dwPrevTaskID = m_BigCalendar.GetSelectedTaskID();
	
	bool bSet = (m_BigCalendar.SelectTask(dwTaskID) != FALSE);

	if (bSet)
	{
		m_BigCalendar.ScrollToSelectedTask();

		UpdateSelectedTaskDates();
		SyncMiniCalendar(TRUE);
	}

	return bSet;
}

bool CCalendarWnd::SelectTasks(const DWORD* /*pdwTaskIDs*/, int /*nTaskCount*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return false; // only support single selection
}

bool CCalendarWnd::WantEditUpdate(IUI_ATTRIBUTE nAttribute) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return (CTaskCalendarCtrl::WantEditUpdate(nAttribute) != FALSE);
}

bool CCalendarWnd::WantSortUpdate(IUI_ATTRIBUTE nAttribute) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return (CTaskCalendarCtrl::WantSortUpdate(nAttribute) != FALSE);
}

bool CCalendarWnd::PrepareNewTask(ITaskList* pTask) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return m_BigCalendar.PrepareNewTask(pTask);
}

void CCalendarWnd::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const IUI_ATTRIBUTE* pAttributes, int nNumAttributes)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_BigCalendar.UpdateTasks(pTasks, nUpdate, CSet<IUI_ATTRIBUTE>(pAttributes, nNumAttributes));

	UpdateSelectedTaskDates();
}

void CCalendarWnd::Release()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (GetSafeHwnd())
		DestroyWindow();
	
	delete this;
}

void CCalendarWnd::OnSetFocus(CWnd* /*pOldWnd*/)
{
	//set focus to big calendar (seems the focus gets lost after switching back to the Calendar window from another app)
	m_BigCalendar.SetFocus();
}

void CCalendarWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if (m_BigCalendar.GetSafeHwnd())
		ResizeControls(cx, cy);
}

BOOL CALLBACK CCalendarWnd::IsMiniCalSpecialDateCallback(COleDateTime &dt, DWORD dwUserData)
{
	ASSERT(dwUserData);

	CCalendarWnd* pThis = (CCalendarWnd*)dwUserData;

	return pThis->m_BigCalendar.IsSpecialDate(dt);
}


HBRUSH CCalendarWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CCalendarWnd::OnEraseBkgnd(CDC* pDC)
{
	// clip out children
	CDialogHelper::ExcludeCtrl(this, IDC_CALENDAR_FRAME, pDC);

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

void CCalendarWnd::OnPreferences()
{
	if (m_dlgPrefs.DoModal() == IDOK)
	{
		// show/hide the minicalendar as necessary
		if (m_dlgPrefs.GetShowMiniCalendar())
			m_MiniCalendar.ShowWindow(SW_SHOW);
		else
			m_MiniCalendar.ShowWindow(SW_HIDE);

		// forward options to calendar ctrl
		UpdateCalendarCtrlPreferences();

		// call ResizeControls which will resize both calendars
		CRect rcFrame;
		GetClientRect(rcFrame);
		ResizeControls(rcFrame.Width(), rcFrame.Height());
	}

	//set focus to big calendar (seems the focus gets lost after the dialog is shown)
	m_BigCalendar.SetFocus();
}

void CCalendarWnd::ResizeControls(int cx, int cy)
{
	// selected task dates takes available space
	int nOffset = cx - CDialogHelper::GetCtrlRect(this, IDC_SELECTEDTASKDATES).right;
	CDialogHelper::ResizeCtrl(this, IDC_SELECTEDTASKDATES, nOffset, 0);

	// calendar frame
	CRect rFrame = CDialogHelper::GetCtrlRect(this, IDC_CALENDAR_FRAME);

	rFrame.right = cx;
	rFrame.bottom = cy;
	GetDlgItem(IDC_CALENDAR_FRAME)->MoveWindow(rFrame);

	// mini-cal
	rFrame.DeflateRect(1, 1);
	int nMiniCalWidth = (m_dlgPrefs.GetShowMiniCalendar() ? m_MiniCalendar.GetMinWidth() : 0);

	CRect rItem(rFrame);
	rItem.right = rItem.left + nMiniCalWidth;

	m_MiniCalendar.MoveWindow(rItem);

	// big-cal
	rItem.left = rItem.right;
	rItem.right = rFrame.right;

	m_BigCalendar.MoveWindow(rItem);

	Invalidate(TRUE);
}

void CCalendarWnd::SyncMiniCalendar(BOOL bScroll)
{
	if (bScroll)
		m_MiniCalendar.SetCurrentMonthAndYear(m_BigCalendar.GetMinDate());
	else
		m_MiniCalendar.SetCurrentMonthAndYear(m_BigCalendar.GetFirstSelectedItem());
}

void CCalendarWnd::SyncBigCalendar(BOOL bScroll)
{
	m_BigCalendar.Goto(m_MiniCalendar.GetCurrentMonthAndYear(), !bScroll);
}

void CCalendarWnd::OnBigCalendarNotifyClick(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	SyncMiniCalendar(FALSE);

	*pResult = 0;
}

void CCalendarWnd::OnBigCalendarNotifyDblClk(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	SyncMiniCalendar(FALSE);
	
	*pResult = 0;
}

void CCalendarWnd::OnMiniCalendarNotifyClick(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	SyncBigCalendar(FALSE);

	*pResult = 0;
}

void CCalendarWnd::OnMiniCalendarNotifyDblClk(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	SyncBigCalendar(FALSE);
	
	*pResult = 0;
}

void CCalendarWnd::OnGototoday() 
{
	COleDateTime dtToday = COleDateTime::GetCurrentTime();

	m_BigCalendar.GotoToday(true);
	SyncMiniCalendar(FALSE);
}

LRESULT CCalendarWnd::OnBigCalendarNotifyDateChange(WPARAM wp, LPARAM /*lp*/)
{
	COleDateTime dtStart, dtDue;

	if (m_BigCalendar.GetSelectedTaskDates(dtStart, dtDue))
	{
		IUITASKMOD mod = { IUI_NONE, 0 };
		
		switch (wp)
		{
		case TCCHT_BEGIN:
			if (CDateHelper::GetTimeT64(dtStart, mod.tValue))
				mod.nAttrib = IUI_STARTDATE;
			break;
			
		case TCCHT_MIDDLE:
			if (CDateHelper::GetTimeT64(dtStart, mod.tValue))
				mod.nAttrib = IUI_OFFSETTASK;
			break;
			
		case TCCHT_END:
			if (CDateHelper::GetTimeT64(dtDue, mod.tValue))
				mod.nAttrib = IUI_DUEDATE;
			break;
		}
		
		if (mod.nAttrib != IUI_NONE)
			GetParent()->SendMessage(WM_IUI_MODIFYSELECTEDTASK, 1, (LPARAM)&mod);
	}

	return 0L;
}

LRESULT CCalendarWnd::OnBigCalendarNotifySelectionChange(WPARAM /*wp*/, LPARAM lp)
{
	if (lp)
	{
		UpdateSelectedTaskDates();
		GetParent()->SendMessage(WM_IUI_SELECTTASK, 0, lp);
	}

	return 0L;
}

void CCalendarWnd::OnCancel()
{
	m_BigCalendar.CancelDrag();
}

void CCalendarWnd::UpdateSelectedTaskDates()
{
	COleDateTime dtStart, dtDue;
	
	if (m_BigCalendar.GetSelectedTaskDates(dtStart, dtDue))
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

LRESULT CCalendarWnd::OnBigCalendarNotifyDragChange(WPARAM wp, LPARAM /*lp*/)
{
	CDialogHelper::SelectItemByData(m_cbSnapModes, wp);
	UpdateSelectedTaskDates();

	return 0L;
}

LRESULT CCalendarWnd::OnBigCalendarNotifyVisibleWeekChange(WPARAM /*wp*/, LPARAM /*lp*/)
{
	m_cbNumWeeks.SetCurSel(m_BigCalendar.GetVisibleWeeks() - 1);

	return 0L;
}

void CCalendarWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetSafeHwnd() == m_BigCalendar.GetSafeHwnd())
	{
		SyncMiniCalendar(TRUE);
		return;
	}
	
	// else
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CCalendarWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetSafeHwnd() == m_MiniCalendar.GetSafeHwnd())
	{
		ASSERT((nSBCode == SB_LEFT) || (nSBCode == SB_RIGHT));

// 		// Move current date to start of month
// 		int nYear = m_MiniCalendar.GetCurrentYear();
// 		int nMonth = m_MiniCalendar.GetCurrentMonth();
// 
// 		COleDateTime date(nYear, nMonth, 1, 0, 0, 0);
// 		m_MiniCalendar.SetDate(date);

		SyncBigCalendar(TRUE);
		return;
	}
	
	// else
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}
