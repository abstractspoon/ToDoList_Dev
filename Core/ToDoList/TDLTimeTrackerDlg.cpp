// TDCTimeTrackerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLTimeTrackerDlg.h"
#include "taskfile.h"
#include "ToDoCtrl.h"

#include "..\shared\HoldRedraw.h"
#include "..\shared\dlgunits.h"
#include "..\shared\autoflag.h"
#include "..\shared\localizer.h"
#include "..\shared\mapex.h"
#include "..\shared\Misc.h"

#include "..\3rdparty\XNamedColors.h"

/////////////////////////////////////////////////////////////////////////

enum // btns
{
	BTN_STARTENABLED,
	BTN_STOPENABLED,
	BTN_STARTDISABLED,
	BTN_STOPDISABLED,
};

/////////////////////////////////////////////////////////////////////////

const int ID_RESET_ELAPSED = 1;

/////////////////////////////////////////////////////////////////////////

CTDLTimeTrackerDlg::CTDLTimeTrackerDlg()
	: 
	CDialog(CTDLTimeTrackerDlg::IDD), 
	m_pWndNotify(NULL),
	m_btnStart(32),
	m_sizeMin(0, 0),
	m_sizeMax(32000, 32000),
	m_nUncollapsedHeight(-1),
	m_bCollapsed(FALSE),
	m_bRecreating(FALSE),
	m_bCentreOnShow(FALSE),
	m_dwOptions(0),
	m_dwStartStopShortcut(0)
{
	m_sOrgCaption = CEnString(IDS_FOCUS_TIMETRACKER);
}

CTDLTimeTrackerDlg::~CTDLTimeTrackerDlg()
{
}

void CTDLTimeTrackerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TASKLISTS, m_cbTasklists);
	DDX_Control(pDX, IDC_TASKS, m_cbTasks);
	DDX_Control(pDX, IDC_STARTSTOP, m_btnStart);
	DDX_Control(pDX, IDC_ELAPSEDTIME, m_eElapsedTime);
	DDX_Text(pDX, IDC_TASKTIME, m_sTaskTimes);
	DDX_Text(pDX, IDC_ELAPSEDTIME, m_sElapsedTime);
	DDX_Text(pDX, IDC_QUICKFIND, m_sQuickFind);
}

/////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTDLTimeTrackerDlg, CDialog)
	ON_BN_CLICKED(IDC_STARTSTOP, OnStartStopTracking)
	ON_WM_CTLCOLOR()
	ON_WM_NCHITTEST()
	ON_CBN_SELCHANGE(IDC_TASKLISTS, OnSelchangeTasklist)
	ON_CBN_SELCHANGE(IDC_TASKS, OnSelchangeTask)
	ON_EN_CHANGE(IDC_QUICKFIND, OnChangeQuickFind)
	ON_WM_ERASEBKGND()
	ON_WM_ACTIVATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipNotify)
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	ON_WM_NCLBUTTONDBLCLK()
	ON_COMMAND(ID_TIMETRACKER_ONTOP, OnToggleTopMost)
	ON_COMMAND(ID_TIMETRACK_HELP, OnHelp)
	ON_WM_HELPINFO()
	ON_REGISTERED_MESSAGE(WM_EE_BTNCLICK, OnEEBtnClick)
	ON_REGISTERED_MESSAGE(WM_TTC_TOOLHITTEST, OnToolHitTest)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////

void CTDLTimeTrackerDlg::OnHelp()
{
	AfxGetApp()->WinHelp(IDD_TIMETRACK_DIALOG);
}

BOOL CTDLTimeTrackerDlg::OnHelpInfo(HELPINFO* /*lpHelpInfo*/)
{
	OnHelp();
	return TRUE;
}

BOOL CTDLTimeTrackerDlg::Create(CWnd* pNotify, DWORD dwOptions)
{
	// Called externally so get topmost state from prefs
	m_bAlwaysOnTop = CPreferences().GetProfileInt(_T("TimeTracker"), _T("AlwaysOnTop"), TRUE);
	m_dwOptions = dwOptions;

	if (!Create())
		return FALSE;

	m_pWndNotify = pNotify;
	return TRUE;
}

BOOL CTDLTimeTrackerDlg::Create()
{
	CWnd* pParent = (m_bAlwaysOnTop ? GetDesktopWindow() : AfxGetMainWnd());

	if (!CDialog::Create(IDD_TIMETRACK_DIALOG, pParent))
	{
		ASSERT(0);
		m_pWndNotify = NULL;

		return FALSE;
	}

	if (m_bAlwaysOnTop)
		SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, (SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE));

	return TRUE;
}

BOOL CTDLTimeTrackerDlg::Recreate()
{
	CAutoFlag af(m_bRecreating, TRUE);
	
	// Cache current state
	const CToDoCtrl* pTDC = GetSelectedTasklist();
	DWORD dwTaskID = GetSelectedTaskID();
	
	CRect rPrev;
	GetWindowRect(rPrev);
	
	DestroyWindow();
	
	if (!Create()) // invisible
		return FALSE;
	
	if (!RebuildTasklistCombo())
		return FALSE;
	
	if (!SelectTaskList(pTDC))
		return FALSE;
	
	m_cbTasks.SelectTask(dwTaskID);
	
	// restore position
	MoveWindow(rPrev);
	ShowWindow(SW_SHOW);

	return TRUE;
}

BOOL CTDLTimeTrackerDlg::OnInitDialog()
{	
	CDialog::OnInitDialog();

	CLocalizer::EnableTranslation(m_cbTasks, FALSE);
	CLocalizer::EnableTranslation(m_cbTasklists, FALSE);

	// Note: Titlebar tooltip is created on demand in OnEraseBkgnd
	
	if (m_ilBtns.Create(32, 32, (ILC_COLOR32 | ILC_MASK), 0, 0))
	{
		CBitmap bmp;
		
		if (bmp.LoadBitmap(IDB_TIMETRACK_BTNS))
			m_ilBtns.Add(&bmp, colorMagenta);
	}

	m_toolbar.SetBackgroundColors(m_theme.crAppBackLight, CLR_NONE, FALSE, FALSE);
	m_toolbar.SetHotColor(m_theme.crToolbarHot);

	if (m_toolbar.CreateEx(this))
	{
		VERIFY(m_toolbar.LoadToolBar(IDR_TIMETRACKER_TOOLBAR, IDB_TIMETRACK_TOOLBAR_STD, colorMagenta));
		VERIFY(m_tbHelper.Initialize(&m_toolbar, this));

		m_toolbar.SetDlgCtrlID(IDC_TOOLBAR);
		m_toolbar.MoveWindow(GetCtrlRect(this, IDC_TOOLBAR));
		m_toolbar.GetToolBarCtrl().CheckButton(ID_TIMETRACKER_ONTOP, m_bAlwaysOnTop);
	}

	m_mgrPrompts.SetEditPrompt(IDC_QUICKFIND, *this, IDS_QUICKTASKFIND);

	m_iconResetElapsed.Load(IDI_RESET, 16, FALSE);
	m_eElapsedTime.AddButton(ID_RESET_ELAPSED, m_iconResetElapsed, CEnString(IDS_RESET_ELAPSED), 15);
		
	m_iconDlg.Load(IDR_MAINFRAME_STD);
	SetIcon(m_iconDlg, TRUE);

	EnableToolTips(TRUE);
	CalcMinMaxSizes();
	LoadSettings();
	
	return TRUE;
}

BOOL CTDLTimeTrackerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (m_tipCaption.GetSafeHwnd() && m_tipCaption.WantMessage(pMsg->message))
	{
		m_tipCaption.FilterToolTipMessage(pMsg, TRUE);
	}

	// we need to check for <return> in quick find
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((::GetDlgCtrlID(pMsg->hwnd) == IDC_QUICKFIND) &&
			(::GetFocus() == pMsg->hwnd))
		{
			switch (pMsg->wParam)
			{
			case VK_RETURN:
			case VK_F3:
				{
					BOOL bForward = !Misc::ModKeysArePressed(MKS_SHIFT);

					if (m_cbTasks.SelectNextItem(m_sQuickFind, bForward))
					{
						UpdatePlayButton();
						UpdateTaskTime(GetSelectedTasklist());
					
						return TRUE;
					}
				}
				break;
			}
		}
		else if (m_dwStartStopShortcut != 0)
		{
			BOOL bExtKey = (pMsg->lParam & 0x01000000);
			DWORD dwShortcut = Misc::GetShortcut((WORD)pMsg->wParam, bExtKey);

			if (dwShortcut == m_dwStartStopShortcut)
			{
				OnStartStopTracking();
				return TRUE;
			}
		}
	}
	
	// else default handling
	return CDialog::PreTranslateMessage(pMsg);
}

void CTDLTimeTrackerDlg::SetUITheme(const CUIThemeFile& theme)
{
	CUIThemeFile oldTheme = m_theme;
	m_theme = theme;
	
	if (m_theme != oldTheme)
	{
		m_brBack.DeleteObject();

		// Use crAppBackLight so the toolbar merges with the bkgnd
		m_toolbar.SetBackgroundColors(m_theme.crAppBackLight, m_theme.crAppBackLight, m_theme.HasGradient(), m_theme.HasGlass());
		m_toolbar.SetHotColor(m_theme.crToolbarHot);

		// Rescale images because background colour has changed
		if (GraphicsMisc::WantDPIScaling())
			m_toolbar.SetImage(IDB_TIMETRACK_TOOLBAR_STD, colorMagenta);

		Invalidate(TRUE);
		SendMessage(WM_NCPAINT);
	}
}

DWORD CTDLTimeTrackerDlg::GetTasklistTrackID(const CToDoCtrl* pTDC) const
{
	const TRACKTASKLIST* pTTL = m_aTasklists.GetTasklist(pTDC);
	ASSERT(pTTL);
	
	return (pTTL ? pTTL->GetTrackedTaskID() : 0);
}

int CTDLTimeTrackerDlg::GetTasklistCBIndex(const CToDoCtrl* pTDC) const
{
	ASSERT(m_cbTasklists.GetSafeHwnd());
	
	return CDialogHelper::FindItemByData(m_cbTasklists, (DWORD)pTDC);
}

BOOL CTDLTimeTrackerDlg::HasTasklist(const CToDoCtrl* pTDC) const
{
	return (m_aTasklists.FindTasklist(pTDC) != -1);
}

BOOL CTDLTimeTrackerDlg::AddTasklist(const CToDoCtrl* pTDC, const CTaskFile& tasks)
{
	if (m_aTasklists.AddTasklist(pTDC, tasks) == -1)
	{
		ASSERT(0);
		return FALSE;
	}
	
	// else
	CEnString sTitle;
	
	if (pTDC->IsDelayLoaded())
		sTitle.Format(IDS_TIMETRACKER_DELAYLOADED, pTDC->GetFriendlyProjectName());
	else
		sTitle = pTDC->GetFriendlyProjectName();

	int nTDC = AddString(m_cbTasklists, sTitle, (DWORD)pTDC);
	
	if (nTDC == CB_ERR)
	{
		ASSERT(0);
		return FALSE;
	}
	
	// Select first non-delay-loaded tasklist
	if ((m_cbTasklists.GetCurSel() == CB_ERR) && !pTDC->IsDelayLoaded())
	{
		m_cbTasklists.SetCurSel(nTDC);
		m_cbTasks.Rebuild(m_aTasklists.GetTasklist(pTDC));
		
		UpdatePlayButton();
	}
	
	RefreshMaxDropWidth(m_cbTasklists);
	
	return TRUE;
}

BOOL CTDLTimeTrackerDlg::SetTasks(const CToDoCtrl* pTDC, const CTaskFile& tasks)
{
	TRACKTASKLIST* pTTL = m_aTasklists.GetTasklist(pTDC);

	if (!pTTL)
	{
		ASSERT(0);
		return FALSE;
	}
	pTTL->SetTasks(tasks);

	if (IsSelectedTasklist(pTDC))
		m_cbTasks.Rebuild(pTTL);
	
	UpdateTasklistName(pTDC);
	UpdateTaskTime(pTDC);
	UpdatePlayButton();
	
	return TRUE;
}

void CTDLTimeTrackerDlg::UpdateTasklistName(const CToDoCtrl* pTDC)
{
	int nTDC = FindItemByData(m_cbTasklists, (DWORD)pTDC);

	if (nTDC == CB_ERR)
	{
		ASSERT(0);
		return;
	}

	CString sCurName = GetItem(m_cbTasklists, nTDC);
	CString sNewName = pTDC->GetFriendlyProjectName();

	if (sNewName != sCurName)
	{
		BOOL bSelected = IsSelectedTasklist(pTDC);

		m_cbTasklists.DeleteString(nTDC);

		int nTDC = AddString(m_cbTasklists, sNewName, (DWORD)pTDC);
		ASSERT(nTDC != CB_ERR);

		// Restore 
		if (bSelected)
			m_cbTasklists.SetCurSel(nTDC);
	}
}

BOOL CTDLTimeTrackerDlg::UpdateAllTasks(const CToDoCtrl* pTDC)
{
	if (m_aTasklists.FindTasklist(pTDC) == -1)
	{
		ASSERT(0);
		return FALSE;
	}

	TDCGETTASKS filter(TDCGT_NOTDONE);
	
	filter.mapAttribs.Add(TDCA_TASKNAME);
	filter.mapAttribs.Add(TDCA_ICON);
	
	CTaskFile tasks;
	pTDC->GetTasks(tasks, filter);

	return SetTasks(pTDC, tasks);
}

BOOL CTDLTimeTrackerDlg::UpdateSelectedTasks(const CToDoCtrl* pTDC, const CTDCAttributeMap& mapAttrib)
{
	// Operations which introduce new tasks or undelete existing tasks
	// require a complete refresh
	if (mapAttrib.Has(TDCA_PASTE) ||
		mapAttrib.Has(TDCA_POSITION_SAMEPARENT) ||
		mapAttrib.Has(TDCA_POSITION_DIFFERENTPARENT) ||
		(mapAttrib.Has(TDCA_DONEDATE) && !pTDC->SelectedTasksAreAllDone()) ||
		(mapAttrib.Has(TDCA_NEWTASK) && !pTDC->IsTaskLabelEditing()))
	{
		return UpdateAllTasks(pTDC);
	}

	// else
	TRACKTASKLIST* pTTL = m_aTasklists.GetTasklist(pTDC);

	if (!pTTL)
	{
		ASSERT(0);
		return FALSE;
	}

	BOOL bUpdateSel = FALSE;
	CDWordArray aModTaskIDs;

	if (mapAttrib.Has(TDCA_TASKNAME) || mapAttrib.Has(TDCA_ICON))
	{
		CTaskFile tasks;
		TDCGETTASKS filter(TDCGT_NOTDONE);

		filter.mapAttribs.Add(TDCA_TASKNAME);
		filter.mapAttribs.Add(TDCA_ICON);

		bUpdateSel = (pTDC->GetSelectedTasks(tasks, filter) && pTTL->UpdateTasks(tasks, aModTaskIDs));
	}

	DWORD dwRemoveTasks = 0;

	if (mapAttrib.Has(TDCA_DONEDATE))
		dwRemoveTasks |= TTL_REMOVEDONE;

	if (mapAttrib.Has(TDCA_DELETE))
		dwRemoveTasks |= TTL_REMOVEDELETED;

	BOOL bUpdateAll = RemoveTasks(pTDC, dwRemoveTasks);

	if (IsSelectedTasklist(pTDC))
	{
		if (bUpdateAll)
		{
			m_cbTasks.Rebuild(pTTL);
		}
		else if (bUpdateSel)
		{
			m_cbTasks.Update(pTTL, aModTaskIDs);
		}

		UpdatePlayButton();
		UpdateTaskTime(pTDC);
	}

	return TRUE;
}

BOOL CTDLTimeTrackerDlg::RemoveTasks(const CToDoCtrl* pTDC, DWORD dwToRemove)
{
	if (dwToRemove == 0)
		return FALSE;

	TRACKTASKLIST* pTTL = m_aTasklists.GetTasklist(pTDC);

	if (!pTTL)
	{
		ASSERT(0);
		return FALSE;
	}

	return pTTL->RemoveTasks(dwToRemove);
}

BOOL CTDLTimeTrackerDlg::SelectTaskList(const CToDoCtrl* pTDC)
{
	// Select the tasklist
	if (CB_ERR == SelectItemByData(m_cbTasklists, (DWORD)pTDC))
		return FALSE;

	// Update UI
	OnSelchangeTasklist();

	return TRUE;
}

const CToDoCtrl* CTDLTimeTrackerDlg::GetSelectedTasklist() const
{
	return (const CToDoCtrl*)GetSelectedItemData(m_cbTasklists);
}

BOOL CTDLTimeTrackerDlg::IsSelectedTasklist(const CToDoCtrl* pTDC) const
{
	return (pTDC && (pTDC == GetSelectedTasklist()));
}

BOOL CTDLTimeTrackerDlg::RebuildTasklistCombo()
{
	int nTasklist = m_aTasklists.GetNumTasklists();

	while (nTasklist--)
	{
		const TRACKTASKLIST* pTTL = m_aTasklists.GetTasklist(nTasklist);

		if (!pTTL)
		{
			ASSERT(0);
			return FALSE;
		}

		CEnString sTitle;
		
		if (pTTL->pTDC->IsDelayLoaded())
			sTitle.Format(IDS_TIMETRACKER_DELAYLOADED, pTTL->pTDC->GetFriendlyProjectName());
		else
			sTitle = pTTL->pTDC->GetFriendlyProjectName();
		
		if (AddString(m_cbTasklists, sTitle, (DWORD)pTTL->pTDC) == CB_ERR)
		{
			ASSERT(0);
			return FALSE;
		}
	}
	
	RefreshMaxDropWidth(m_cbTasklists);
	return TRUE;
}

BOOL CTDLTimeTrackerDlg::RemoveTasklist(const CToDoCtrl* pTDC)
{
	ASSERT(m_pWndNotify && GetSafeHwnd());
	
	int nCBTasklist = GetTasklistCBIndex(pTDC);
	int nTasklist = m_aTasklists.FindTasklist(pTDC);
	
	if ((nTasklist == CB_ERR) || (nTasklist == -1))
	{
		ASSERT(0);
		return FALSE;
	}
	
	// else
	if (IsSelectedTasklist(pTDC))
		m_cbTasks.ResetContent();
	
	m_cbTasklists.DeleteString(nCBTasklist);
	m_aTasklists.DeleteTasklist(pTDC);

	return TRUE;
}

void CTDLTimeTrackerDlg::RemoveAllTasklists()
{
	ASSERT(m_pWndNotify && GetSafeHwnd());
	
	m_cbTasklists.ResetContent();
	m_cbTasks.ResetContent();
	m_aTasklists.DeleteAllTasklists();

	m_sTaskTimes.Empty();
	m_sElapsedTime.Empty();

	UpdateData(FALSE);
	UpdatePlayButton();
}

BOOL CTDLTimeTrackerDlg::UpdateTracking(const CToDoCtrl* pTDC)
{
	ASSERT(m_pWndNotify && GetSafeHwnd());
	ASSERT(pTDC);

	// Update data struct first
	TRACKTASKLIST* pTTL = m_aTasklists.GetTasklist(pTDC);

	if (!pTTL)
	{
		ASSERT(0);
		return FALSE;
	}

	BOOL bWasTracking = pTTL->IsTracking();
	DWORD dwWasTrackingTaskID = (bWasTracking ? pTTL->GetTrackedTaskID() : 0);

	VERIFY(pTTL->UpdateTracking());

	BOOL bIsTracking = pTTL->IsTracking();
	DWORD dwIsTrackingTaskID = (bIsTracking ? pTTL->GetTrackedTaskID() : 0);

	ASSERT(Misc::StateChanged(bWasTracking, bIsTracking) ||
			(bWasTracking && bIsTracking && (dwIsTrackingTaskID != dwWasTrackingTaskID)));

	// If we've just started tracking, switch to that tasklist
	// and show the dialog if required
	if (bIsTracking && !bWasTracking)
	{
		SelectTaskList(pTDC);

		if (HasOption(TTDO_SHOWONBEGINTRACKING))
		{
			ShowWindow(SW_SHOWNORMAL);
			SetForegroundWindow();
		}
	}
	else
	{
		UpdatePlayButton();
		UpdateTaskTime(pTDC);
	}

	m_cbTasks.UpdateRecentlyTrackedTasks(pTTL);
	RefreshCaptionText();
	
	return TRUE;
}

BOOL CTDLTimeTrackerDlg::IsTrackingSelectedTasklistAndTask() const
{
	const CToDoCtrl* pTDC = GetSelectedTasklist();

	if (!pTDC)
		return FALSE;

	const TRACKTASKLIST* pTTL = m_aTasklists.GetTasklist(pTDC);

	if (!pTTL)
	{
		ASSERT(0);
		return FALSE;
	}

	return pTTL->IsTracking(GetSelectedTaskID());
}

CString CTDLTimeTrackerDlg::GetSelectedTaskTitle() const
{
	const CToDoCtrl* pTDC = GetSelectedTasklist();
	DWORD dwSelID = GetSelectedTaskID();

	return ((pTDC && dwSelID) ? pTDC->GetTaskTitle(dwSelID) : _T(""));
}

CString CTDLTimeTrackerDlg::GetSelectedTasklistName() const
{
	const CToDoCtrl* pTDC = GetSelectedTasklist();

	return (pTDC ? pTDC->GetFriendlyProjectName() : _T(""));
}

void CTDLTimeTrackerDlg::UpdatePlayButton(BOOL bCheckVisibility)
{
	if (bCheckVisibility && (!IsWindowVisible() || m_bCollapsed))
		return;

	BOOL bEnable = ((m_cbTasklists.GetCurSel() != CB_ERR) &&
					(m_cbTasks.GetCurSel() != CB_ERR));
	
	if (bEnable)
	{
		BOOL bTracking = IsTrackingSelectedTasklistAndTask();

		m_btnStart.SetIcon(m_ilBtns.ExtractIcon(bTracking ? BTN_STOPENABLED : BTN_STARTENABLED));
		m_btnStart.SetTooltip(CEnString(bTracking ? IDS_STOP_TRACKING : IDS_START_TRACKING));
	}
	else
	{
		m_btnStart.SetIcon(m_ilBtns.ExtractIcon(BTN_STARTDISABLED));
		m_btnStart.SetTooltip(NULL);
	}

	m_btnStart.EnableWindow(bEnable);
}

// External version
void CTDLTimeTrackerDlg::UpdateTaskTime(const CToDoCtrl* pTDC)
{
	UpdateTaskTime(pTDC, TRUE);
}

// Internal version
void CTDLTimeTrackerDlg::UpdateTaskTime(const CToDoCtrl* pTDC, BOOL bCheckVisibility)
{
	if ((bCheckVisibility && !IsWindowVisible()) || !IsSelectedTasklist(pTDC))
		return;

	BOOL bTrackingSelTask = IsTrackingSelectedTasklistAndTask();

	if (bTrackingSelTask)
		m_sElapsedTime = pTDC->FormatTimeTrackingElapsedTime();
	else
		m_sElapsedTime.Empty();

	DWORD dwSelTaskID = GetSelectedTaskID();
	CTimeHelper th;
	TDCTIMEPERIOD timeEst, timeSpent;
	
	if (dwSelTaskID)
		pTDC->GetTaskTimes(dwSelTaskID, timeEst, timeSpent);

	if (HasOption(TTDO_FORMATTIMESASHMS))
	{
		m_sTaskTimes.Format(_T("%s : %s"),
			th.FormatTimeHMS(timeEst.dAmount, timeEst.GetTHUnits(), (HMS_ALLOWZERO | HMS_DECIMALPLACES)),
			th.FormatTimeHMS(timeSpent.dAmount, timeSpent.GetTHUnits(), (HMS_ALLOWZERO | HMS_DECIMALPLACES)));
	}
	else
	{
		m_sTaskTimes.Format(_T("%s : %s"),
			th.FormatTime(timeEst.dAmount, timeEst.GetTHUnits(), 2),
			th.FormatTime(timeSpent.dAmount, timeSpent.GetTHUnits(), 2));
	}
		
	if (m_bCollapsed)
		RefreshCaptionText();

	UpdateData(FALSE);

	if (bTrackingSelTask)
	{
		GetDlgItem(IDC_TASKTIME)->Invalidate(FALSE);
		GetDlgItem(IDC_ELAPSEDTIME)->Invalidate(FALSE);
	}
}

void CTDLTimeTrackerDlg::OnStartStopTracking()
{
	ASSERT(m_cbTasklists.GetCurSel() != CB_ERR);
	
	TRACKTASKLIST* pTTL = m_aTasklists.GetTasklist(GetSelectedTasklist());

	if (!pTTL)
	{
		ASSERT(0);
		return;
	}
	
	DWORD dwSelTaskID = GetSelectedTaskID();

	if (pTTL->IsTracking(dwSelTaskID))
	{
		// notify parent to STOP tracking
		SendNotifyMessage(WM_TDLTTN_STOPTRACKING, pTTL->pTDC, 0);
	}
	else
	{
		// notify parent to START tracking
		SendNotifyMessage(WM_TDLTTN_STARTTRACKING, pTTL->pTDC, dwSelTaskID);
	}

	UpdateTracking(pTTL->pTDC);
	m_cbTasks.UpdateRecentlyTrackedTasks(pTTL);

	RefreshCaptionText();

	// redraw text colour
	GetDlgItem(IDC_TASKTIME)->Invalidate(FALSE);
	GetDlgItem(IDC_ELAPSEDTIME)->Invalidate(FALSE);
}

LRESULT CTDLTimeTrackerDlg::SendNotifyMessage(UINT message, const CToDoCtrl* pTDC, DWORD dwTaskID) const
{
	ASSERT(pTDC && pTDC->GetSafeHwnd());

	return m_pWndNotify->SendMessage(message, (WPARAM)pTDC->GetSafeHwnd(), dwTaskID);
}

HBRUSH CTDLTimeTrackerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (!Misc::IsHighContrastActive() && !m_bRecreating)
	{
		if (nCtlColor == CTLCOLOR_STATIC)
		{
			pDC->SetBkMode(TRANSPARENT);

			if (!m_brBack.GetSafeHandle())
				m_brBack.CreateSolidBrush(GetBkgndColor());

			hbr = (HBRUSH)m_brBack.GetSafeHandle();

			pDC->SetTextColor(m_theme.crAppText);
		}

		if (IsTrackingSelectedTasklistAndTask())
		{
			switch (pWnd->GetDlgCtrlID())
			{
			case IDC_TASKTIME:
			case IDC_ELAPSEDTIME:
				pDC->SetTextColor(255);
				break;
			}
		}
	}

	return hbr;
}

#if _MSC_VER >= 1400
afx_msg LRESULT CTDLTimeTrackerDlg::OnNcHitTest(CPoint point)
#else
afx_msg UINT CTDLTimeTrackerDlg::OnNcHitTest(CPoint point)
#endif
{
	LRESULT nHitTest = CDialog::OnNcHitTest(point);
	
	if (nHitTest == HTCLIENT)
		nHitTest = HTCAPTION;
	
	return nHitTest;
}

void CTDLTimeTrackerDlg::OnSelchangeTasklist()
{
	const CToDoCtrl* pTDC = GetSelectedTasklist();
	ASSERT(pTDC);

	BOOL bTasklistChange = ((pTDC != m_cbTasks.GetToDoCtrl()) || !m_cbTasks.GetCount());
	
	if (bTasklistChange && pTDC->IsDelayLoaded())
	{
		int nSel = m_cbTasklists.GetCurSel();

		if (SendNotifyMessage(WM_TDLTTN_LOADDELAYEDTASKLIST, pTDC, 0) == FALSE)
			return;

		UpdateTasklistName(pTDC);
	}

	// Select the tasklist
	const TRACKTASKLIST* pTTL = m_aTasklists.GetTasklist(pTDC);

	if (!pTTL)
	{
		ASSERT(0);
		return;
	}
	
	// Build task combo and select the tracked task if any
	if (bTasklistChange)
		m_cbTasks.Rebuild(pTTL);
	else
		m_cbTasks.SelectTask(pTTL);
	
	UpdatePlayButton();
	UpdateTaskTime(pTDC);
	RefreshCaptionText();
}

void CTDLTimeTrackerDlg::OnSelchangeTask()
{
	UpdatePlayButton();
	UpdateTaskTime(GetSelectedTasklist());
	RefreshCaptionText();
}

BOOL CTDLTimeTrackerDlg::OnEraseBkgnd(CDC* pDC)
{
	if (!m_tipCaption.GetSafeHwnd() && m_tipCaption.Create(this))
	{
		m_tipCaption.ModifyStyleEx(0, WS_EX_TRANSPARENT);
	}

	if (!Misc::IsHighContrastActive())
	{
		CRect rClient;
		GetClientRect(rClient);
		
		pDC->FillSolidRect(rClient, GetBkgndColor());
		
		return TRUE;
	}

	// else
	return CDialog::OnEraseBkgnd(pDC);
}

COLORREF CTDLTimeTrackerDlg::GetBkgndColor() const
{
	return m_theme.crAppBackLight;
}

void CTDLTimeTrackerDlg::OnChangeQuickFind()
{
	UpdateData();

	const CToDoCtrl* pTDC = GetSelectedTasklist();

	if (pTDC && m_cbTasks.SelectNextItem(m_sQuickFind, TRUE))
	{
		UpdatePlayButton();
		UpdateTaskTime(pTDC);
	}
}

LRESULT CTDLTimeTrackerDlg::OnToolHitTest(WPARAM wp, LPARAM lp)
{
	ASSERT(wp && lp);

	// Don't show a tip if it's just 'Timer Tracker'
	if (m_sCaption != m_sOrgCaption)
	{
		CPoint point(wp);
		TOOLINFO* pTI = (TOOLINFO*)lp;

		// Since we override WM_NCHITTEST we first need to 
		// limit our hit-test to the actual caption bar
		CRect rWindow;
		GetWindowRect(rWindow);
		ScreenToClient(rWindow);

		rWindow.bottom = 0; // top of client rect
		
		if (rWindow.PtInRect(point))
		{
			ClientToScreen(&point);

			int nHit = ::SendMessage(*this, WM_NCHITTEST, 0, MAKELPARAM(point.x, point.y));

			switch (nHit)
			{
			case HTCAPTION:
			case HTSYSMENU:
				return m_tipCaption.SetToolInfo(*pTI, this, m_sCaption, 1, rWindow, TTF_ALWAYSTIP | TTF_TRANSPARENT | TTF_NOTBUTTON);
			}
		}
	}

	return -1;
}

BOOL CTDLTimeTrackerDlg::OnToolTipNotify(UINT /*id*/, NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT*)pNMHDR;
	
	static CString sTooltip;
		
	switch (CToolTipCtrlEx::GetCtrlID(pTTT))
	{
	case IDC_TASKLISTS:
		sTooltip = GetSelectedItem(m_cbTasklists);
		break;
		
	case IDC_TASKS:
		sTooltip = GetSelectedItem(m_cbTasks);
		break;

	default:
		return FALSE;
	}
	
	if (!sTooltip.IsEmpty())
	{
		Misc::Trim(sTooltip);
		pTTT->lpszText = (LPTSTR)(LPCTSTR)sTooltip;
	}
	
	return TRUE; // handled
}

void CTDLTimeTrackerDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		if (m_bCentreOnShow)
		{
			CenterWindow(m_pWndNotify);
			m_bCentreOnShow = FALSE;
		}

		UpdatePlayButton(FALSE);
		UpdateTaskTime(GetSelectedTasklist(), FALSE); // ignore visible state
		RefreshCaptionText();
	}
}

void CTDLTimeTrackerDlg::OnDestroy()
{
	SaveSettings();

	m_ilBtns.DeleteImageList();
	m_iconResetElapsed.Destroy();
	m_eElapsedTime.DeleteAllButtons();

	CDialog::OnDestroy();
}

void CTDLTimeTrackerDlg::LoadSettings()
{
	if (!m_bRecreating) // ie. first time only
	{
		CPreferences prefs;
		
		m_bCentreOnShow = TRUE; // default

		int nWidth = prefs.GetProfileInt(_T("TimeTracker"), _T("Width"), 0);
		int nHeight = prefs.GetProfileInt(_T("TimeTracker"), _T("Height"), 0);

		if ((nWidth > 0) && (nHeight > 0))
		{
			CRect rWindow;
			GetWindowRect(rWindow);

			int nPos = prefs.GetProfileInt(_T("TimeTracker"), _T("Position"), -1);

			if (nPos != -1)
			{
				CRect rTemp(rWindow), rScreen;

				rTemp.OffsetRect((GET_X_LPARAM(nPos) - rTemp.left),
								(GET_Y_LPARAM(nPos) - rTemp.top));

				if (GraphicsMisc::GetAvailableScreenSpace(rTemp, rScreen))
				{
					rWindow = rTemp;
					m_bCentreOnShow = FALSE;
				}
			}

			rWindow.right = (rWindow.left + nWidth);
			rWindow.bottom = (rWindow.top + nHeight);

			MoveWindow(rWindow);
			Resize();

			ASSERT(m_bCollapsed == FALSE);

			if (prefs.GetProfileInt(_T("TimeTracker"), _T("Collapsed"), FALSE))
				CollapseWindow(TRUE);
		}
	}
}

void CTDLTimeTrackerDlg::SaveSettings() const
{
	CPreferences prefs;
	
	CRect rWindow;
	GetWindowRect(rWindow);
	
	if (m_bCollapsed)
		prefs.WriteProfileInt(_T("TimeTracker"), _T("Height"), m_nUncollapsedHeight);
	else
		prefs.WriteProfileInt(_T("TimeTracker"), _T("Height"), rWindow.Height());

	prefs.WriteProfileInt(_T("TimeTracker"), _T("Width"), rWindow.Width());
	prefs.WriteProfileInt(_T("TimeTracker"), _T("Collapsed"), m_bCollapsed);
	prefs.WriteProfileInt(_T("TimeTracker"), _T("Position"), MAKELPARAM(rWindow.left, rWindow.top));
	prefs.WriteProfileInt(_T("TimeTracker"), _T("AlwaysOnTop"), m_bAlwaysOnTop);
}

void CTDLTimeTrackerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	
	Resize(cx, cy);
}

void CTDLTimeTrackerDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// Limit minimum size
	lpMMI->ptMinTrackSize.x = m_sizeMin.cx;

	if (!m_bCollapsed)
		lpMMI->ptMinTrackSize.y = m_sizeMin.cy;
	
	// Prevent height changing
	lpMMI->ptMaxTrackSize.y = m_sizeMax.cy;
}

void CTDLTimeTrackerDlg::Resize(int cx, int cy)
{
	if (m_btnStart.GetSafeHwnd())
	{
		CRect rClient(0, 0, cx, cy);

		// Parameter-less call
		if ((cx == 0) && (cy == 0))
			GetClientRect(rClient);

		if (rClient.IsRectEmpty())
			return;

		// work out how many rows we can display
		int nRows = CalcAvailableRows(rClient.Height());

		// remove borders
		CDlgUnits dlu(this);
		rClient.DeflateRect(dlu.ToPixelsX(4), dlu.ToPixelsY(4));

		// We _always_ display the button bottom-right
		// together with the task combo and time displays
		CRect rCurBtn = GetChildRect(&m_btnStart), rNewBtn;

		rNewBtn.right = rClient.right;
		rNewBtn.bottom = rClient.bottom;
		rNewBtn.left = (rNewBtn.right - rCurBtn.Width());
		rNewBtn.top = (rNewBtn.bottom - rCurBtn.Height());
		
		m_btnStart.MoveWindow(rNewBtn);

		int nXOffset = (rNewBtn.right - rCurBtn.right);
		int nYOffset = (rNewBtn.bottom - rCurBtn.bottom);

		OffsetCtrl(this, IDC_TASKTIME, 0, nYOffset);
		ResizeCtrl(this, IDC_TASKTIME, nXOffset, 0);
		OffsetCtrl(this, IDC_TASKTIME_LABEL, 0, nYOffset);
		ResizeCtrl(this, IDC_TASKTIME_LABEL, nXOffset, 0);

		OffsetCtrl(this, IDC_ELAPSEDTIME, 0, nYOffset);
		ResizeCtrl(this, IDC_ELAPSEDTIME, nXOffset, 0);
		OffsetCtrl(this, IDC_ELAPSEDTIME_LABEL, 0, nYOffset);
		ResizeCtrl(this, IDC_ELAPSEDTIME_LABEL, nXOffset, 0);

		// Then the rest if there is space
		BOOL bShowTasks = (nRows > 2);
		BOOL bShowTasklists = ((nRows == 5) || ((nRows == 4) && (m_aTasklists.GetNumTasklists() > 1)));
		BOOL bShowToolbar = ((nRows == 5) || ((nRows == 4) && !bShowTasklists));

		ShowCtrl(&m_toolbar, bShowToolbar);
		ShowCtrl(this, IDC_QUICKFIND, bShowToolbar);
		ShowCtrl(this, IDC_TASKLISTS, bShowTasklists);
		ShowCtrl(this, IDC_TASKLISTS_LABEL, bShowTasklists);
		ShowCtrl(this, IDC_TASKS, bShowTasks);
		ShowCtrl(this, IDC_TASKS_LABEL, bShowTasks);

		OffsetCtrl(this, IDC_TASKS, 0, nYOffset);
		ResizeCtrl(this, IDC_TASKS, nXOffset, 0);
		OffsetCtrl(this, IDC_TASKS_LABEL, 0, nYOffset);
		ResizeCtrl(this, IDC_TASKS_LABEL, nXOffset, 0);

		OffsetCtrl(this, IDC_TASKLISTS, 0, nYOffset);
		ResizeCtrl(this, IDC_TASKLISTS, nXOffset, 0);
		OffsetCtrl(this, IDC_TASKLISTS_LABEL, 0, nYOffset);
		ResizeCtrl(this, IDC_TASKLISTS_LABEL, nXOffset, 0);
		ResizeCtrl(this, IDC_QUICKFIND, nXOffset, 0);
		
		Invalidate();
		RefreshCaptionText();
	}
}

int CTDLTimeTrackerDlg::CalcAvailableRows(int nHeight) const
{
	int nRowHeight = CDlgUnits(this).ToPixelsY(22);

	return (nHeight / nRowHeight);
}

void CTDLTimeTrackerDlg::CalcMinMaxSizes()
{
	if (m_btnStart.GetSafeHwnd())
	{
		CRect rWindow;
		GetWindowRect(rWindow);

		m_sizeMax.cy = rWindow.Height();
		m_sizeMin.cx = rWindow.Width();

		CRect rClient, rLabel = GetCtrlRect(this, IDC_TASKTIME_LABEL);
		GetClientRect(rClient);

		CDlgUnits dlu(this);
		m_sizeMin.cy = ((rWindow.Height() - rClient.Height()) + (rClient.bottom - rLabel.top) + 4);
	}
}

void CTDLTimeTrackerDlg::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	CDialog::OnNcLButtonDblClk(nHitTest, point);

	if (nHitTest == HTCAPTION)
		CollapseWindow(!m_bCollapsed);
}

void CTDLTimeTrackerDlg::CollapseWindow(BOOL bCollapse)
{
	if (Misc::StateChanged(m_bCollapsed, bCollapse))
	{
		m_bCollapsed = bCollapse;

		CRect rWindow;
		GetWindowRect(rWindow);
		
		if (m_bCollapsed)
		{
			m_nUncollapsedHeight = rWindow.Height();
			
			CRect rClient;
			GetClientRect(rClient);
			
			rWindow.bottom -= rClient.Height();
		}
		else
		{
			rWindow.bottom = (rWindow.top + m_nUncollapsedHeight);
		}
		
		MoveWindow(rWindow);
		UpdatePlayButton();
		RefreshCaptionText();
	}
}

void CTDLTimeTrackerDlg::RefreshCaptionText()
{
	CString sTaskTitle = GetSelectedTaskTitle(), sPrevCaption = m_sCaption;

	if (m_bCollapsed && !m_sTaskTimes.IsEmpty())
	{
		ASSERT(!sTaskTitle.IsEmpty());

		if (!m_sElapsedTime.IsEmpty())
		{
			m_sCaption.Format(_T("%s (%s) - %s"), m_sElapsedTime, m_sTaskTimes, sTaskTitle);
		}
		else
		{
			m_sCaption.Format(_T("%s - %s"), m_sTaskTimes, sTaskTitle);
		}
	}
	else
	{
		// work out how many rows we can display
		CRect rClient;
		GetClientRect(rClient);

		int nRows = CalcAvailableRows(rClient.Height());

		if (nRows < 3) // task combo hidden
		{
			m_sCaption.Format(_T("%s - %s"), sTaskTitle, m_sOrgCaption);
		}
		else if ((nRows == 3) || ((nRows == 4) && (m_aTasklists.GetNumTasklists() == 1))) // tasklist combo hidden
		{
			m_sCaption.Format(_T("%s - %s"), GetSelectedTasklistName(), m_sOrgCaption);
		}
		else
		{
			m_sCaption = m_sOrgCaption;
		}
	}

	SetWindowText(m_sCaption);

	if (m_tipCaption.GetSafeHwnd() && (sPrevCaption != m_sCaption))
		m_tipCaption.Activate(FALSE);
}

void CTDLTimeTrackerDlg::OnToggleTopMost()
{
	SetAlwaysOnTop(!m_bAlwaysOnTop);
}

void CTDLTimeTrackerDlg::SetAlwaysOnTop(BOOL bTopMost)
{
	if (bTopMost == m_bAlwaysOnTop)
		return;

	m_bAlwaysOnTop = bTopMost;
	VERIFY(Recreate());
}

void CTDLTimeTrackerDlg::SetOption(DWORD dwOption, BOOL bEnable)
{
	if (Misc::ModifyFlags(m_dwOptions, (bEnable ? 0 : dwOption), (bEnable ? dwOption : 0)))
	{
		if (Misc::HasFlag(dwOption, TTDO_ALLOWPARENTTRACKING))
			m_cbTasks.EnableParentTasks(bEnable);

		if (Misc::HasFlag(dwOption, TTDO_SHOWPARENTSASFOLDERS))
			m_cbTasks.SetShowParentTasksAsFolders(bEnable);

		if (Misc::HasFlag(dwOption, TTDO_FORMATTIMESASHMS))
			UpdateTaskTime(GetSelectedTasklist());
	}
}

BOOL CTDLTimeTrackerDlg::HasOption(DWORD dwOption) const
{
	return Misc::HasFlag(m_dwOptions, dwOption);
}

void CTDLTimeTrackerDlg::SetWindowIcons(HICON hIconBig, HICON hIconSmall)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	SetIcon(hIconBig, TRUE);
	SetIcon(hIconSmall, FALSE);
}

LRESULT CTDLTimeTrackerDlg::OnEEBtnClick(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IDC_ELAPSEDTIME:
		switch (lParam)
		{
		case ID_RESET_ELAPSED:
			{
				const CToDoCtrl* pTDC = GetSelectedTasklist();

				SendNotifyMessage(WM_TDLTTN_RESETELAPSEDTIME, pTDC, 0);
				UpdateTaskTime(pTDC);
			}
			break;
		}
		break;
	}

	return 0L;
}

