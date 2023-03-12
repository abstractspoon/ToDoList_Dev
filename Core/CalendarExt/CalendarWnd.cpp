// CalendarFrameWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "CalendarWnd.h"
#include "CalMsg.h"

#include "..\Shared\DialogHelper.h"
#include "..\Shared\DateHelper.h"
#include "..\Shared\themed.h"
#include "..\Shared\dlgunits.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\localizer.h"
#include "..\shared\WorkingWeek.h"

#include "..\3rdparty\GdiPlus.h"
#include "..\3rdparty\XNamedColors.h"

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
const COLORREF DEF_GRIDLINECOLOR	= RGB(192, 192, 192);
const COLORREF DEF_ALTLINECOLOR		= RGB(230, 230, 255);

/////////////////////////////////////////////////////////////////////////////
// CCalendarWnd

IMPLEMENT_DYNAMIC(CCalendarWnd, CDialog)

CCalendarWnd::CCalendarWnd()
	:	
	m_bReadOnly(FALSE),
	m_stSelectedTaskDates(TRUE), // we handle the click
	m_MiniCalendar(m_BigCalendar.Data()),
	m_dlgPrefs(this)
{
	m_icon.Load(IDR_CALENDAR);
}

CCalendarWnd::~CCalendarWnd()
{
}

void CCalendarWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_NUMWEEKS, m_cbNumWeeks);
	DDX_Control(pDX, IDC_SNAPMODES, m_cbSnapModes);
	DDX_Control(pDX, IDC_SELECTEDTASKDATES, m_stSelectedTaskDates);
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
	ON_CONTROL(STN_CLICKED, IDC_SELECTEDTASKDATES, OnClickSelectedTaskDates)
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
	ON_REGISTERED_MESSAGE(WM_CALENDAR_GETTASKICON, OnBigCalendarGetTaskIcon)
	ON_REGISTERED_MESSAGE(WM_CALENDAR_GETTASKFUTUREDATES, OnBigCalendarGetTaskFutureDates)
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalendarWnd message handlers

void CCalendarWnd::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	
	delete this;
}

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

	m_BigCalendar.SetReadOnly(bReadOnly ? TRUE : FALSE);
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
		VERIFY(m_toolbar.LoadToolBar(IDR_TOOLBAR, IDB_TOOLBAR_STD, colorMagenta));
		VERIFY(m_tbHelper.Initialize(&m_toolbar, this));
		
		CRect rToolbar = CDialogHelper::GetCtrlRect(this, IDC_TB_PLACEHOLDER);
		m_toolbar.Resize(rToolbar.Width(), rToolbar.TopLeft());
		m_toolbar.RefreshButtonStates(TRUE);
	}

	// create mini-calendar ctrl
	m_MiniCalendar.SetShowWeekNumbers(TRUE);
	m_MiniCalendar.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | FMC_NO3DBORDER, 
                          CRect(0,0,0,0), this, IDC_MINI_CALENDAR);

    m_MiniCalendar.SetDate(COleDateTime::GetCurrentTime());
    m_MiniCalendar.SetRowsAndColumns(3, 1);

	// create big-calendar ctrl
	m_BigCalendar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, CRect(0,0,0,0), this, IDC_BIG_CALENDAR);
	m_BigCalendar.EnableMultiSelection(FALSE);

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

	CDialogHelper::SelectItemByData(m_cbSnapModes, m_BigCalendar.GetDefaultSnapMode());
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
	m_BigCalendar.SetDefaultSnapMode(CDialogHelper::GetSelectedItemData(m_cbSnapModes, TCCSM_FREE));
}

void CCalendarWnd::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_dlgPrefs.SavePreferences(pPrefs, szKey);

	pPrefs->WriteProfileInt(szKey, _T("NumWeeks"), m_BigCalendar.GetVisibleWeeks());
	pPrefs->WriteProfileInt(szKey, _T("SnapMode"), m_BigCalendar.GetDefaultSnapMode());
}

void CCalendarWnd::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey, bool bAppOnly) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// app preferences
	DWORD dwPrefs = m_BigCalendar.GetOptions(); // preserve calendar-specific prefs

	Misc::SetFlag(dwPrefs, TCCO_TASKTEXTCOLORISBKGND, pPrefs->GetProfileInt(_T("Preferences"), _T("ColorTaskBackground"), FALSE));
	Misc::SetFlag(dwPrefs, TCCO_STRIKETHRUDONETASKS, pPrefs->GetProfileInt(_T("Preferences"), _T("StrikethroughDone"), TRUE));
	Misc::SetFlag(dwPrefs, TCCO_PREVENTDEPENDENTDRAGGING, pPrefs->GetProfileInt(_T("Preferences"), _T("AutoAdjustDependents"), TRUE));
	Misc::SetFlag(dwPrefs, TCCO_SHOWPARENTTASKSASFOLDER, pPrefs->GetProfileInt(_T("Preferences"), _T("ShowParentsAsFolders"), TRUE));
	Misc::SetFlag(dwPrefs, TCCO_ENABLELABELTIPS, !pPrefs->GetProfileInt(_T("Preferences"), _T("ShowInfoTips"), FALSE));

	m_BigCalendar.SetOptions(dwPrefs);

	DWORD dwWeekends = pPrefs->GetProfileInt(_T("Preferences"), _T("Weekends"), (DHW_SATURDAY | DHW_SUNDAY));
	CWeekend::Initialise(dwWeekends);

	// get grid line color from app prefs
	COLORREF crGrid = DEF_GRIDLINECOLOR;

	if (pPrefs->GetProfileInt(_T("Preferences"), _T("SpecifyGridColor"), TRUE))
		crGrid = pPrefs->GetProfileInt(_T("Preferences\\Colors"), _T("GridLines"), DEF_GRIDLINECOLOR);

	m_BigCalendar.SetGridLineColor(crGrid);
	m_MiniCalendar.SetBorderColor(crGrid);

/*
	// And alternate day colour which we will use for alternate weeks
	COLORREF crAlt = CLR_NONE;

	if (pPrefs->GetProfileInt(_T("Preferences"), _T("AlternateLineColor"), TRUE))
		crAlt = pPrefs->GetProfileInt(_T("Preferences\\Colors"), _T("AlternateLines"), DEF_ALTLINECOLOR);

	m_BigCalendar.SetAlternateWeekColor(crAlt);
*/

	// calendar specific preferences
	if (!bAppOnly)
	{
		CString sKey(szKey);

		m_dlgPrefs.LoadPreferences(pPrefs, sKey);
		UpdateCalendarCtrlPreferences();

		int nWeeks = pPrefs->GetProfileInt(sKey, _T("NumWeeks"), 2);
		nWeeks = max(0, min(nWeeks, 9));
		
		if (m_BigCalendar.SetVisibleWeeks(nWeeks))
			m_cbNumWeeks.SetCurSel(nWeeks-1);

		TCC_SNAPMODE nSnap = (TCC_SNAPMODE)pPrefs->GetProfileInt(szKey, _T("SnapMode"), TCCSM_NEARESTHOUR);
		m_BigCalendar.SetDefaultSnapMode(nSnap);
		CDialogHelper::SelectItemByData(m_cbSnapModes, nSnap);
	
		// make sure 'today' is visible
		COleDateTime dtToday = COleDateTime::GetCurrentTime();

		m_MiniCalendar.SetDate(dtToday);
		m_BigCalendar.GotoToday();
	}
}

void CCalendarWnd::UpdateCalendarCtrlPreferences()
{
	DWORD dwOptions = 0;

	Misc::SetFlag(dwOptions, TCCO_CALCMISSINGSTARTASCREATION,		m_dlgPrefs.GetCalcMissingStartAsCreation());
	Misc::SetFlag(dwOptions, TCCO_CALCMISSINGSTARTASDUE,			m_dlgPrefs.GetCalcMissingStartAsDue());
	Misc::SetFlag(dwOptions, TCCO_CALCMISSINGSTARTASEARLIESTDUEANDTODAY, m_dlgPrefs.GetCalcMissingStartAsEarliestDueAndToday());
	Misc::SetFlag(dwOptions, TCCO_CALCMISSINGDUEASSTART,			m_dlgPrefs.GetCalcMissingDueAsStart());
	Misc::SetFlag(dwOptions, TCCO_CALCMISSINGDUEASLATESTSTARTANDTODAY, m_dlgPrefs.GetCalcMissingDueAsLatestStartAndToday());
	Misc::SetFlag(dwOptions, TCCO_DISPLAYCONTINUOUS,				m_dlgPrefs.GetDisplayAsContinuous());
	Misc::SetFlag(dwOptions, TCCO_DISPLAYSTART,						m_dlgPrefs.GetDisplayStart());
	Misc::SetFlag(dwOptions, TCCO_DISPLAYDUE,						m_dlgPrefs.GetDisplayDue());
	Misc::SetFlag(dwOptions, TCCO_DISPLAYDONE,						m_dlgPrefs.GetDisplayDone());
	Misc::SetFlag(dwOptions, TCCO_DISPLAYCALCSTART,					m_dlgPrefs.GetDisplayCalcStart());
	Misc::SetFlag(dwOptions, TCCO_DISPLAYCALCDUE,					m_dlgPrefs.GetDisplayCalcDue());
	Misc::SetFlag(dwOptions, TCCO_ADJUSTTASKHEIGHTS,				m_dlgPrefs.GetAdjustTaskHeights());
	Misc::SetFlag(dwOptions, TCCO_TREATOVERDUEASDUETODAY,			m_dlgPrefs.GetTreatOverdueAsDueToday());
	Misc::SetFlag(dwOptions, TCCO_SHOWFUTUREITEMS,					m_dlgPrefs.GetDisplayFutureOcurrences());
	Misc::SetFlag(dwOptions, TCCO_SHOWDATEINEVERYCELL,				m_dlgPrefs.GetDisplayDateInEveryCell());
	Misc::SetFlag(dwOptions, TCCO_SHOWWEEKNUMINCELLDATE,			m_dlgPrefs.GetDisplayWeekNumberInCell());

	// Preserve app preferences
	Misc::SetFlag(dwOptions, TCCO_TASKTEXTCOLORISBKGND,				m_BigCalendar.HasOption(TCCO_TASKTEXTCOLORISBKGND));
	Misc::SetFlag(dwOptions, TCCO_STRIKETHRUDONETASKS,				m_BigCalendar.HasOption(TCCO_STRIKETHRUDONETASKS));
	Misc::SetFlag(dwOptions, TCCO_PREVENTDEPENDENTDRAGGING,			m_BigCalendar.HasOption(TCCO_PREVENTDEPENDENTDRAGGING));
	Misc::SetFlag(dwOptions, TCCO_SHOWPARENTTASKSASFOLDER,			m_BigCalendar.HasOption(TCCO_SHOWPARENTTASKSASFOLDER));
	Misc::SetFlag(dwOptions, TCCO_ENABLELABELTIPS,					m_BigCalendar.HasOption(TCCO_ENABLELABELTIPS));

	m_BigCalendar.SetOptions(dwOptions);
	m_MiniCalendar.SetOptions(dwOptions);

	CString sHideParentTag;
	m_BigCalendar.SetHideParentTasks(m_dlgPrefs.GetHideParentTasks(sHideParentTag), sHideParentTag);

	CDWordArray aHeatMapPalette;
	TDC_ATTRIBUTE nHeatMapAttrib = TDCA_NONE;

	if (m_dlgPrefs.GetEnableHeatMap(aHeatMapPalette, nHeatMapAttrib))
		m_MiniCalendar.EnableHeatMap(aHeatMapPalette, nHeatMapAttrib);
	else
		m_MiniCalendar.DisableHeatMap();
}

void CCalendarWnd::SetUITheme(const UITHEME* pTheme)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	GraphicsMisc::VerifyDeleteObject(m_brBack);

	if (CThemed::IsAppThemed() && pTheme)
	{
		m_theme = *pTheme;
		m_brBack.CreateSolidBrush(pTheme->crAppBackLight);

		m_dlgPrefs.SetThemeBkgndColors(pTheme->crAppBackLight, pTheme->crAppBackDark);

		m_BigCalendar.SetUITheme(*pTheme);
		m_MiniCalendar.SetUITheme(*pTheme);

		// intentionally set background colours to be same as ours
		m_toolbar.SetBackgroundColors(m_theme.crAppBackLight, m_theme.crAppBackLight, FALSE, FALSE);
		m_toolbar.SetHotColor(m_theme.crToolbarHot);

		// Rescale images because background colour has changed
		if (GraphicsMisc::WantDPIScaling())
			m_toolbar.SetImage(IDB_TOOLBAR_STD, colorMagenta);

		m_stSelectedTaskDates.SetBkColor(m_theme.crAppBackLight);
		m_stSelectedTaskDates.SetTextColor(m_theme.crAppText);
	}
}

LPCTSTR CCalendarWnd::GetMenuText() const
{
	return _T("Calendar");
}

void CCalendarWnd::FilterToolTipMessage(MSG* pMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_BigCalendar.FilterToolTipMessage(pMsg);
	m_MiniCalendar.FilterToolTipMessage(pMsg);
}

bool CCalendarWnd::ProcessMessage(MSG* pMsg) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (!IsWindowEnabled())
		return false;

	if (m_BigCalendar.ProcessMessage(pMsg))
		return true;

	if (m_MiniCalendar.PreTranslateMessage(pMsg))
		return true;

	return false;
}

bool CCalendarWnd::DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData) 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	switch (nCmd)
	{
	case IUI_EXPANDALL:
	case IUI_COLLAPSEALL:
	case IUI_EXPANDSELECTED:
	case IUI_COLLAPSESELECTED:
	case IUI_RESIZEATTRIBCOLUMNS:
	case IUI_GETNEXTTASK:
	case IUI_GETNEXTVISIBLETASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTASK:
	case IUI_GETPREVVISIBLETASK:
	case IUI_GETPREVTOPLEVELTASK:
		// not handled
		break;

	case IUI_SORT:
		if (pData)
			return (m_BigCalendar.SortBy(pData->nSortBy, (pData->bSortAscending ? TRUE : FALSE)) != FALSE);
		break;
		
	case IUI_SETFOCUS:
		m_BigCalendar.SetFocus();
		return true;

	case IUI_SAVETOIMAGE:
		if (pData)
		{
			CBitmap bmImage;

			if (m_BigCalendar.SaveToImage(bmImage))
				return (CGdiPlusBitmap(bmImage).SaveAsFile(pData->szFilePath) != FALSE);
		}
	}

	return false;
}

void CCalendarWnd::SetTaskFont(HFONT hFont)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_BigCalendar.SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);
}

bool CCalendarWnd::CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData) const 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	switch (nCmd)
	{
	case IUI_EXPANDALL:
	case IUI_COLLAPSEALL:
	case IUI_EXPANDSELECTED:
	case IUI_COLLAPSESELECTED:
	case IUI_RESIZEATTRIBCOLUMNS:
	case IUI_GETNEXTVISIBLETASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVVISIBLETASK:
	case IUI_GETPREVTOPLEVELTASK:
		// not handled
		break;

	case IUI_SETFOCUS:
		return (CDialogHelper::IsChildOrSame(this, GetFocus()) == FALSE);

	case IUI_SORT:
		if (pData)
			return (CTaskCalendarCtrl::WantSortUpdate(pData->nSortBy) != FALSE);
		break;

	case IUI_SAVETOIMAGE:
		return (m_BigCalendar.CanSaveToImage() != FALSE);
	}
	
	return false;
}

bool CCalendarWnd::GetLabelEditRect(LPRECT pEdit)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_BigCalendar.EnsureSelectionVisible();

	CRect rLabel;

	if (m_BigCalendar.GetSelectedTaskLabelRect(rLabel))
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

IUI_HITTEST CCalendarWnd::HitTest(POINT ptScreen) const
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

	BOOL bCustomDate = FALSE;

	if (m_BigCalendar.HitTestTask(ptBigCal, bCustomDate))
		return (bCustomDate ? IUI_NOWHERE : IUI_TASK);

	// else try rest of big cal
	CRect rCal;
	m_BigCalendar.GetClientRect(rCal);

	return (rCal.PtInRect(ptBigCal) ? IUI_TASKLIST : IUI_NOWHERE);
}

DWORD CCalendarWnd::HitTestTask(POINT ptScreen, bool /*bTitleColumnOnly*/) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CPoint ptBigCal(ptScreen);
	m_BigCalendar.ScreenToClient(&ptBigCal);

	BOOL bUnused = FALSE;

	return m_BigCalendar.HitTestTask(ptBigCal, bUnused);
}

bool CCalendarWnd::SelectTask(DWORD dwTaskID, bool bTaskLink)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_BigCalendar.SelectTask(dwTaskID, TRUE))
	{
		UpdateSelectedTaskDates();
		SyncMiniCalendar(TRUE);

		return true;
	}

	return false;
}

bool CCalendarWnd::SelectTasks(const DWORD* /*pdwTaskIDs*/, int /*nTaskCount*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return false; // only support single selection
}

bool CCalendarWnd::WantTaskUpdate(TDC_ATTRIBUTE nAttribute) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return (CTaskCalendarCtrl::WantEditUpdate(nAttribute) != FALSE);
}

bool CCalendarWnd::PrepareNewTask(ITaskList* pTask) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return (m_BigCalendar.PrepareNewTask(pTask) != FALSE);
}

void CCalendarWnd::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (m_BigCalendar.UpdateTasks(pTasks, nUpdate))
		m_MiniCalendar.OnUpdateTasks();

	UpdateSelectedTaskDates();
}

void CCalendarWnd::OnSetFocus(CWnd* /*pOldWnd*/)
{
	m_BigCalendar.SetFocus();
}

void CCalendarWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if (m_BigCalendar.GetSafeHwnd())
		ResizeControls(cx, cy);
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
	{
		m_MiniCalendar.SetCurrentMonthAndYear(m_BigCalendar.GetMinDate());
	}
	else
	{
		time_t tSel =  m_BigCalendar.GetFirstSelectedItem();
		COleDateTime dtSel(tSel);
		
		m_MiniCalendar.SetDateSel(dtSel, dtSel);
		m_MiniCalendar.SetCurrentMonthAndYear(dtSel);
	}

}

void CCalendarWnd::SyncBigCalendar(BOOL bScroll)
{
	if (bScroll)
		m_BigCalendar.Goto(m_MiniCalendar.GetCurrentMonthAndYear(), FALSE);
	else
		m_BigCalendar.SelectDate(m_MiniCalendar.GetDate(), TRUE);
}

void CCalendarWnd::OnBigCalendarNotifyClick(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	SyncMiniCalendar(FALSE);

	*pResult = 0;
}

void CCalendarWnd::OnBigCalendarNotifyDblClk(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	SyncMiniCalendar(FALSE);

	CPoint ptBigCal(GetMessagePos());
	m_BigCalendar.ScreenToClient(&ptBigCal);

	BOOL bUnused = FALSE;
	DWORD dwTaskID = m_BigCalendar.HitTestTask(ptBigCal, bUnused);

	if (dwTaskID)
		GetParent()->SendMessage(WM_IUI_EDITSELECTEDTASKTITLE, dwTaskID);
	
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
	TCINOTIFYDATECHANGE* notify = (TCINOTIFYDATECHANGE*)wp;
	IUITASKMOD mod = { TDCA_NONE, 0 };
	CString sCustAttribValue; // must persist for duration of parent callback

	if (notify->sCustAttribID.IsEmpty())
	{
		COleDateTime dtStart, dtDue;

		if (m_BigCalendar.GetSelectedTaskDates(dtStart, dtDue))
		{
			switch (notify->nHit)
			{
			case TCCHT_BEGIN:
				if (CDateHelper::GetTimeT64(dtStart, mod.tValue))
					mod.nAttrib = TDCA_STARTDATE;
				break;

			case TCCHT_MIDDLE:
				if (CDateHelper::GetTimeT64(dtStart, mod.tValue))
					mod.nAttrib = TDCA_OFFSETTASK;
				break;

			case TCCHT_END:
				if (CDateHelper::GetTimeT64(dtDue, mod.tValue))
					mod.nAttrib = TDCA_DUEDATE;
				break;
			}
		}
	}
	else
	{
		COleDateTime date;
		BOOL bHasDate = m_BigCalendar.GetSelectedTaskCustomDate(notify->sCustAttribID, date);

		ASSERT(!bHasDate || (notify->nHit == TCCHT_MIDDLE));

		mod.szCustomAttribID = notify->sCustAttribID;
		mod.nAttrib = TDCA_CUSTOMATTRIB;

		if (bHasDate)		
			sCustAttribValue = CDateHelper::FormatDate(date, DHFD_TIME);

		mod.szValue = sCustAttribValue;
	}

	if (mod.nAttrib != TDCA_NONE)
	{
		if (GetParent()->SendMessage(WM_IUI_MODIFYSELECTEDTASK, 1, (LPARAM)&mod))
		{
			m_MiniCalendar.OnUpdateTasks();
			return TRUE;
		}
	}

	return 0L;
}

LRESULT CCalendarWnd::OnBigCalendarNotifySelectionChange(WPARAM /*wp*/, LPARAM lp)
{
	UpdateSelectedTaskDates();
	GetParent()->SendMessage(WM_IUI_SELECTTASK, 0, lp);

	return 0L;
}

void CCalendarWnd::OnCancel()
{
	m_BigCalendar.CancelDrag();
}

void CCalendarWnd::UpdateSelectedTaskDates()
{
	CString sSelectedTaskDates;
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

		CString sDateRange;
		sDateRange.Format(_T("%s - %s"), sStart, sDue);

		sSelectedTaskDates.Format(_T("%s: <a href=%s>%s</a>"), CEnString(IDS_SELTASKDATES_LABEL), _T(""), sDateRange);
	}

	m_stSelectedTaskDates.SetWindowText(sSelectedTaskDates);
}

void CCalendarWnd::OnClickSelectedTaskDates()
{
	m_BigCalendar.EnsureSelectionVisible();
}

LRESULT CCalendarWnd::OnBigCalendarNotifyDragChange(WPARAM /*wp*/, LPARAM /*lp*/)
{
	//CDialogHelper::SelectItemByData(m_cbSnapModes, wp);
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

		SyncBigCalendar(TRUE);
		return;
	}
	
	// else
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CCalendarWnd::OnBigCalendarGetTaskIcon(WPARAM wp, LPARAM lp)
{
	return GetParent()->SendMessage(WM_IUI_GETTASKICON, wp, lp);
}


LRESULT CCalendarWnd::OnBigCalendarGetTaskFutureDates(WPARAM wp, LPARAM lp)
{
	if (!wp || !lp)
	{
		ASSERT(0);
		return 0L;
	}

	TASKCALFUTUREDATES* pFutureDates = (TASKCALFUTUREDATES*)lp;
	IUINEXTTASKOCCURRENCES iuiNext;

	// Package up range
	if (!CDateHelper::GetTimeT64(pFutureDates->dtRange.GetStart(), iuiNext.tRangeStart))
	{
		ASSERT(0);
		return 0L;
	}

	if (!CDateHelper::GetTimeT64(pFutureDates->dtRange.GetEndInclusive(), iuiNext.tRangeEnd))
	{
		ASSERT(0);
		return 0L;
	}

	if (!GetParent()->SendMessage(WM_IUI_GETNEXTTASKOCCURRENCES, wp, (LPARAM)&iuiNext) ||
		!iuiNext.nNumOccurrences)
	{
		// No occurrences
		return 0L;
	}
	 
	// Unpack occurrences
	int nNumSrc = iuiNext.nNumOccurrences, nNumDest = 0;

	for (int nItem = 0; nItem < nNumSrc; nItem++)
	{
		COleDateTime dtStart = CDateHelper::GetDate(iuiNext.occurrences[nItem].tStart);
		COleDateTime dtEnd = CDateHelper::GetDate(iuiNext.occurrences[nItem].tEnd);

		if (pFutureDates->dtOccurrences[nNumDest].Set(dtStart, dtEnd, TRUE))
			nNumDest++;
	}

	ASSERT(nNumDest);
	pFutureDates->nNumOccurrences = nNumDest;
	
	return nNumDest;
}

