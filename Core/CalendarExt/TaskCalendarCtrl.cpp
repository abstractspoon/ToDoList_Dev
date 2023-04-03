// TaskCalendarCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TaskCalendarCtrl.h"
#include "CalMsg.h"

#include "..\Shared\GraphicsMisc.h"
#include "..\Shared\themed.h"
#include "..\Shared\DateHelper.h"
#include "..\Shared\dialoghelper.h"
#include "..\Shared\holdredraw.h"
#include "..\Shared\autoflag.h"
#include "..\Shared\enimagelist.h"
#include "..\Shared\WorkingWeek.h"
#include "..\Shared\ScopedTimer.h"

#include "..\Interfaces\UITheme.h"

#include <math.h>

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const int TIP_PADDING		= 3;
const int TEXT_PADDING		= GraphicsMisc::ScaleByDPIFactor(2);
const int HEADER_PADDING	= GraphicsMisc::ScaleByDPIFactor(3);
const int IMAGE_SIZE		= GraphicsMisc::ScaleByDPIFactor(16);
const int DEF_TASK_HEIGHT	= (IMAGE_SIZE + 3); // Effective height is 1 less
const int MIN_TASK_HEIGHT	= (DEF_TASK_HEIGHT - 6);
const int OVERFLOWBTN_TIPID = INT_MAX;

/////////////////////////////////////////////////////////////////////////////
// CTaskCalendarCtrl

CTaskCalendarCtrl::CTaskCalendarCtrl()
	:
	m_nMaxDayTaskCount(0),
	m_dwSelectedTaskID(0),
	m_bDraggingStart(FALSE),
	m_bDraggingEnd(FALSE),
	m_bDragging(FALSE),
	m_ptDragOrigin(0),
	m_nDefSnapMode(TCCSM_NEARESTHOUR),
	m_dwOptions(TCCO_DISPLAYCONTINUOUS | TCCO_ENABLELABELTIPS),
	m_bReadOnly(FALSE),
	m_nCellVScrollPos(0),
	m_bStrikeThruDone(FALSE),
	m_bSavingToImage(FALSE),
	m_nTaskHeight(DEF_TASK_HEIGHT),
	m_nSortBy(TDCA_NONE),
	m_bSortAscending(-1),
	m_crWeekend(RGB(224, 224, 224)),
	m_crToday(255),
	m_crAltWeek(CLR_NONE),
	m_sCellDateFormat(_T("%#d"))
{
	GraphicsMisc::CreateFont(m_DefaultFont, _T("Tahoma"));

	m_bDrawGridOverCells = FALSE; // always
	m_crTheme = RGB(160, 215, 255);
}

CTaskCalendarCtrl::~CTaskCalendarCtrl()
{
	DeleteData();

	// clean up other heap allocations
	for(int i=0; i<CALENDAR_MAX_ROWS ; i++)
	{
		for(int u=0; u<CALENDAR_NUM_COLUMNS ; u++)
		{
			CCalendarCell* pCell = GetCell(i, u);

			delete static_cast<CTaskCalItemArray*>(pCell->pUserData);
			pCell->pUserData = NULL;
		}
	}
}

BEGIN_MESSAGE_MAP(CTaskCalendarCtrl, CCalendarCtrlEx)
	//{{AFX_MSG_MAP(CTaskCalendarCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_VSCROLL()
	ON_WM_SETFOCUS()
	ON_WM_SETCURSOR()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEWHEEL()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(TTN_SHOW, 0, OnShowTooltip)
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_REGISTERED_MESSAGE(WM_MIDNIGHT, OnMidnight)
	ON_WM_SIZE()

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskCalendarCtrl message handlers

LRESULT CTaskCalendarCtrl::OnGetFont(WPARAM /*wp*/, LPARAM /*lp*/)
{
	return (LRESULT)m_DefaultFont.GetSafeHandle();
}

LRESULT CTaskCalendarCtrl::OnSetFont(WPARAM wp, LPARAM /*lp*/)
{
	m_fonts.Clear();

	GraphicsMisc::VerifyDeleteObject(m_DefaultFont);
	GraphicsMisc::CreateFont(m_DefaultFont, (HFONT)wp);

	// Recalc the task height
	m_nTaskHeight = (GraphicsMisc::GetFontPixelSize((HFONT)wp) + 6);

	if (!HasOption(TCCO_ADJUSTTASKHEIGHTS))
		m_nTaskHeight = max(m_nTaskHeight, DEF_TASK_HEIGHT);

	SetHeaderHeight(m_nTaskHeight + 2);
	SetDayHeaderHeight(m_nTaskHeight);
	RecalcCellHeaderDateFormats();

	return 0L;
}

int CTaskCalendarCtrl::GetDefaultTaskHeight()
{
	return DEF_TASK_HEIGHT;
}

BOOL CTaskCalendarCtrl::HasSameDateDisplayOptions(DWORD dwOld, DWORD dwNew)
{
	return ((dwOld & TCCO_DATEDISPLAYOPTIONS) == (dwNew & TCCO_DATEDISPLAYOPTIONS));
}

void CTaskCalendarCtrl::SetOptions(DWORD dwNewOptions)
{
	// Now handled by 'SetHideParentTasks'
	dwNewOptions &= ~TCCO_HIDEPARENTTASKS;

	DWORD dwCurOptions = (m_dwOptions & ~TCCO_HIDEPARENTTASKS);

	if (dwCurOptions != dwNewOptions)
	{
		DWORD dwPrev = m_dwOptions;
		m_dwOptions = dwNewOptions | (dwPrev & TCCO_HIDEPARENTTASKS); // preserve parent status

		RecalcCellHeaderDateFormats();
		RecalcTaskDates();
		RebuildCellTasks();

		BOOL bScrollToTask = !HasSameDateDisplayOptions(m_dwOptions, dwPrev);
		FixupSelection(bScrollToTask);

		EnableLabelTips(HasOption(TCCO_ENABLELABELTIPS));

		if (HasOption(TCCO_CALCMISSINGSTARTASEARLIESTDUEANDTODAY) ||
			HasOption(TCCO_CALCMISSINGDUEASLATESTSTARTANDTODAY))
		{
			m_timerMidnight.Enable(*this);
		}
		else
		{
			m_timerMidnight.Disable();
		}
	}
}

void CTaskCalendarCtrl::SetHideParentTasks(BOOL bHide, const CString& sTag)
{
	BOOL bIsHidden = HasOption(TCCO_HIDEPARENTTASKS);
	BOOL bChange = (Misc::StateChanged(bHide, bIsHidden) || 
					(bHide && (m_sHideParentTag != sTag)));

	if (bChange)
	{
		if (bHide)
		{
			m_dwOptions |= TCCO_HIDEPARENTTASKS;
			m_sHideParentTag = sTag;
		}
		else
		{
			m_dwOptions &= ~TCCO_HIDEPARENTTASKS;
			m_sHideParentTag.Empty();
		}

		RecalcTaskDates();
		RebuildCellTasks();

		// Scroll to task is hidden parent is being reshown
		BOOL bScrollToTask = (!bHide && m_mapData.IsParentTask(GetSelectedTaskID()));
		FixupSelection(bScrollToTask);
	}
}

void CTaskCalendarCtrl::RecalcTaskDates()
{
	POSITION pos = m_mapData.GetStartPosition();
	DWORD dwTaskID = 0;
	TASKCALITEM* pTCI = NULL;

	while (pos)
	{
		m_mapData.GetNextAssoc(pos, dwTaskID, pTCI);
		ASSERT(pTCI);
		ASSERT(pTCI->GetTaskID() == dwTaskID);

		pTCI->RecalcDates(m_dwOptions);
	}
}

BOOL CTaskCalendarCtrl::PrepareNewTask(ITaskList* pTaskList) const
{
	// give the task a date that will make it appear in the calendar
	COleDateTime date = ((GetMaxDate().m_dt + GetMinDate().m_dt) / 2);

	int nRow, nCol;

	if (GetLastSelectedGridCell(nRow, nCol))
	{
		const CCalendarCell* pCell = GetCell(nRow, nCol);
		ASSERT(pCell);

		date = pCell->date;
	}

	time64_t tDate;
	
	if (!CDateHelper::GetTimeT64(date, tDate))
		return false;

	ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pTaskList, IID_TASKLISTBASE);
	
	if (pTasks == NULL)
	{
		ASSERT(0);
		return false;
	}

	HTASKITEM hNewTask = pTasks->GetFirstTask();
	ASSERT(hNewTask);

	pTasks->SetTaskStartDate64(hNewTask, tDate);
	pTasks->SetTaskDueDate64(hNewTask, tDate);

	return true;
}

BOOL CTaskCalendarCtrl::WantEditUpdate(TDC_ATTRIBUTE nEditAttrib)
{
	switch (nEditAttrib)
	{
	case TDCA_TASKNAME:
	case TDCA_DONEDATE:
	case TDCA_DUEDATE:
	case TDCA_STARTDATE:
	case TDCA_COLOR:
	case TDCA_DEPENDENCY:
	case TDCA_ICON:
	case TDCA_RECURRENCE:
	case TDCA_TAGS:
	case TDCA_CUSTOMATTRIB:
		return true;
	}
	
	// all else 
	return FALSE;
}

BOOL CTaskCalendarCtrl::WantSortUpdate(TDC_ATTRIBUTE nEditAttrib)
{
	switch (nEditAttrib)
	{
	case TDCA_TASKNAME:
	case TDCA_ID:
	case TDCA_NONE:
// 	case IUI_DONEDATE:
// 	case IUI_DUEDATE:
// 	case IUI_STARTDATE:
		return true;
	}
	
	// all else 
	return FALSE;
}

BOOL CTaskCalendarCtrl::UpdateTasks(const ITaskList* pTaskList, IUI_UPDATETYPE nUpdate)
{
	const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pTaskList, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return FALSE;
	}

	// Make sure the selected task remains visible
	// after any changes if it was visible to start with
	BOOL bSelTaskWasVisible = IsTaskVisible(m_dwSelectedTaskID);
	BOOL bChange = UpdateCustomDateAttributes(pTasks);

	switch (nUpdate)
	{
	case IUI_ALL:
		DeleteData();
		BuildData(pTasks, pTasks->GetFirstTask(), TRUE);
		bChange = TRUE;
		break;

	case IUI_NEW:
		BuildData(pTasks, pTasks->GetFirstTask(), TRUE);
		bChange = TRUE;
		break;
		
	case IUI_EDIT:
		bChange |= (UpdateTask(pTasks, pTasks->GetFirstTask(), nUpdate, TRUE) ||
					pTasks->IsAttributeAvailable(TDCA_RECURRENCE));
		break;
		
	case IUI_DELETE:
		bChange |= RemoveDeletedTasks(pTasks);
		break;
		
	default:
		ASSERT(0);
	}
	
	if (bChange)
	{
		// clear selection if necessary
		if (!HasTask(m_dwSelectedTaskID, FALSE))
		{
			m_dwSelectedTaskID = 0;
			bSelTaskWasVisible = FALSE;
		}

		RecalcDataRange();
		RebuildCellTasks();

		if (bSelTaskWasVisible && !IsTaskVisible(m_dwSelectedTaskID))
			EnsureSelectionVisible();
		else
			Invalidate(FALSE);
	}

	return bChange;
}

BOOL CTaskCalendarCtrl::UpdateCustomDateAttributes(const ITASKLISTBASE* pTasks)
{
	CMapStringToString mapPrevAttrib;
	Misc::Copy(m_mapCustomDateAttrib, mapPrevAttrib);

	m_mapCustomDateAttrib.RemoveAll();

	int nAttrib = pTasks->GetCustomAttributeCount();

	while (nAttrib--)
	{
		if (pTasks->IsCustomAttributeEnabled(nAttrib) && 
			((pTasks->GetCustomAttributeType(nAttrib) & TDCCA_DATAMASK) == TDCCA_DATE))
		{
			CString sAttribID = pTasks->GetCustomAttributeID(nAttrib);
			m_mapCustomDateAttrib[Misc::ToUpper(sAttribID)] = pTasks->GetCustomAttributeLabel(nAttrib);
		}
	}

	return !Misc::MatchAll(m_mapCustomDateAttrib, mapPrevAttrib);
}

void CTaskCalendarCtrl::FixupSelection(BOOL bScrollToTask)
{
	if (m_dwSelectedTaskID)
	{
		DWORD dwPrevSelTaskID = m_dwSelectedTaskID;
		m_dwSelectedTaskID = 0;

		SelectTask(dwPrevSelTaskID, bScrollToTask, TRUE);
	}
}

void CTaskCalendarCtrl::BuildTaskMap(const ITASKLISTBASE* pTasks, HTASKITEM hTask, 
							   CSet<DWORD>& mapIDs, BOOL bAndSiblings)
{
	if (hTask == NULL)
		return;
	
	mapIDs.Add(pTasks->GetTaskID(hTask));
	
	// children
	BuildTaskMap(pTasks, pTasks->GetFirstTask(hTask), mapIDs, TRUE);  // RECURSIVE CALL
	
	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			BuildTaskMap(pTasks, hSibling, mapIDs, FALSE);  // RECURSIVE CALL
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
}

BOOL CTaskCalendarCtrl::RemoveDeletedTasks(const ITASKLISTBASE* pTasks)
{
	CSet<DWORD> mapIDs;
	BuildTaskMap(pTasks, pTasks->GetFirstTask(NULL), mapIDs, TRUE);

	// traverse the data looking for items that do not 
	// exist in pTasks and delete them
	BOOL bChange = FALSE;
	POSITION pos = m_mapData.GetStartPosition();

	while (pos)
	{
		DWORD dwTaskID = m_mapData.GetNextTaskID(pos);

		if (!mapIDs.Has(dwTaskID))
		{
			m_mapData.RemoveKey(dwTaskID);
			bChange = TRUE;
		}
	}

	return bChange;
}

BOOL CTaskCalendarCtrl::UpdateTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, BOOL bAndSiblings)
{
	if (hTask == NULL)
		return FALSE;

	ASSERT(nUpdate == IUI_EDIT);

	BOOL bChange = FALSE;

	DWORD dwTaskID = pTasks->GetTaskID(hTask);
	m_dwMaximumTaskID = max(m_dwMaximumTaskID, dwTaskID);

	// Not interested in references or their subtasks
	// which can also only be references
	if (!pTasks->IsTaskReference(hTask))
	{
		if (HasTask(dwTaskID, FALSE)) // don't exclude hidden tasks
		{
			TASKCALITEM* pTCI = GetTaskCalItem(dwTaskID);

			bChange = pTCI->UpdateTask(pTasks, hTask, m_mapCustomDateAttrib, m_dwOptions);

			// Update our list of recurring tasks
			if (pTasks->IsAttributeAvailable(TDCA_RECURRENCE))
			{
				if (pTCI->bRecurring)
					m_mapRecurringTaskIDs.Add(dwTaskID);
				else
					m_mapRecurringTaskIDs.Remove(dwTaskID);
			}
			
			// subtasks
			HTASKITEM hSubtask = pTasks->GetFirstTask(hTask);

			if (hSubtask)
				bChange |= UpdateTask(pTasks, hSubtask, nUpdate, TRUE);
		}
		else // must be new task
		{
			BuildData(pTasks, hTask, FALSE);
		}
	}
	
	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			if (UpdateTask(pTasks, hSibling, nUpdate, FALSE))
				bChange = TRUE;
			
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}

	return bChange;
}

BOOL CTaskCalendarCtrl::NotifyParentDateChange(TCC_HITTEST nHit, const CString& sCustAttribID)
{
	ASSERT(!m_bReadOnly);
	ASSERT(m_dwSelectedTaskID);

	if (nHit != TCCHT_NOWHERE)
	{
		TCINOTIFYDATECHANGE notify;

		notify.nHit = nHit;
		notify.sCustAttribID = sCustAttribID;

		return GetParent()->SendMessage(WM_CALENDAR_DATECHANGE, (WPARAM)&notify, m_dwSelectedTaskID);
	}

	// else
	return FALSE;
}

void CTaskCalendarCtrl::NotifyParentDragChange()
{
	ASSERT(m_dwSelectedTaskID);

	GetParent()->SendMessage(WM_CALENDAR_DRAGCHANGE, (WPARAM)GetSnapMode(), m_dwSelectedTaskID);
}

void CTaskCalendarCtrl::NotifyParentSelChange()
{
	GetParent()->SendMessage(WM_CALENDAR_SELCHANGE, 0, GetRealTaskID(GetSelectedTaskID()));
}

void CTaskCalendarCtrl::BuildData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL bAndSiblings)
{
	if (hTask == NULL)
		return;

	DWORD dwTaskID = pTasks->GetTaskID(hTask);
	m_dwMaximumTaskID = max(m_dwMaximumTaskID, dwTaskID);
	
	// Not interested in references
	if (pTasks->IsTaskReference(hTask))
		return;

	if (!HasTask(dwTaskID, FALSE)) // don't exclude hidden tasks
	{
		// Only interested in new tasks
		TASKCALITEM* pTCI = new TASKCALITEM(pTasks, hTask, m_mapCustomDateAttrib, m_dwOptions);
		m_mapData[dwTaskID] = pTCI;

		// Keep track of recurring tasks
		if (pTCI->bRecurring)
			m_mapRecurringTaskIDs.Add(dwTaskID);
	}
	
	// process children
	BuildData(pTasks, pTasks->GetFirstTask(hTask), TRUE);

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

BOOL CTaskCalendarCtrl::SetVisibleWeeks(int nWeeks)
{
	if (CCalendarCtrlEx::SetVisibleWeeks(nWeeks))
	{
		// delete text font so it can be recreated
		// at the right size
		if (HasOption(TCCO_ADJUSTTASKHEIGHTS))
			GraphicsMisc::VerifyDeleteObject(m_fontAltText);

		FixupSelection(TRUE);
		UpdateCellScrollBarVisibility(TRUE); // scroll to selected task
		return TRUE;
	}

	// else
	return FALSE;
}

int CTaskCalendarCtrl::CalcRequiredTaskFontPointSize() const
{
	if (!HasOption(TCCO_ADJUSTTASKHEIGHTS))
		return -1;

	int nTaskHeight = GetTaskHeight();

	if (nTaskHeight == DEF_TASK_HEIGHT)
		return -1;

	if (nTaskHeight < MIN_TASK_HEIGHT)
		return -1;

	return GraphicsMisc::PixelToPoint(nTaskHeight - 6);
}

void CTaskCalendarCtrl::DeleteData()
{
	m_mapData.RemoveAll();
	m_mapRecurringTaskIDs.RemoveAll();
	m_dwMaximumTaskID = 0;
}

void CTaskCalendarCtrl::DrawHeader(CDC* pDC)
{
	CThemed th;
	BOOL bThemed = (th.AreControlsThemed() && th.Open(this, _T("HEADER")));
	
	CRect rc;
	GetClientRect(&rc);
	
	rc.bottom = m_nHeaderHeight;
	int nWidth = (rc.Width() / CALENDAR_NUM_COLUMNS);
	
	CFont* pOldFont = pDC->SelectObject(&m_DefaultFont);
	CSize sizeDOW = CDateHelper::GetMaxDayOfWeekNameExtent(pDC);
	BOOL bShort = (sizeDOW.cx > nWidth);

	// Overlay with 'today' colour 
	int nTodayCol = -1;

	if (bThemed && (m_crToday != CLR_NONE))
		nTodayCol = GetGridCellColumn(COleDateTime::GetCurrentTime());
	
	CRect rCol(rc);
	
	for(int i = 0 ; i < CALENDAR_NUM_COLUMNS; i++)
	{
		int nDOW = GetDayOfWeek(i);
		
		if (i == (CALENDAR_NUM_COLUMNS - 1))
		{
			rCol.right = rc.right;
		}
		else if (i == 0)
		{
			rCol.right = nWidth + 1;
		}
		else
		{
			rCol.right = rCol.left + nWidth;
		}
		
		// draw background
		if (bThemed)
		{
			th.DrawBackground(pDC, HP_HEADERITEM, HIS_NORMAL, rCol);

			if (i == nTodayCol)
				GraphicsMisc::DrawRect(pDC, rCol, m_crToday, CLR_NONE, 0, GMDR_NONE, 64);
		}
		else
		{
			pDC->FillSolidRect(rCol, GetSysColor(COLOR_3DFACE));
			pDC->Draw3dRect(rCol, GetSysColor(COLOR_3DHIGHLIGHT), GetSysColor(COLOR_3DSHADOW));
		}
		
		CString csTitle = CDateHelper::GetDayOfWeekName((OLE_DAYOFWEEK)nDOW, bShort);
		CRect rText(rCol);
		rText.DeflateRect(HEADER_PADDING, (m_nHeaderHeight - sizeDOW.cy) / 2, 0, 0);
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(csTitle, rText, DT_LEFT|DT_TOP);
		
		// next column
		rCol.left = rCol.right;
	}
	pDC->SelectObject(pOldFont);
}

void CTaskCalendarCtrl::DrawCells(CDC* pDC)
{
	UpdateCellScrollBarVisibility(FALSE);
	RebuildCellTaskDrawInfo();

	// create alternate text font as required
	int nSize = CalcRequiredTaskFontPointSize();

	if (nSize == -1)
	{
		GraphicsMisc::VerifyDeleteObject(m_fontAltText);
	}
	else
	{
		int nCurSize = GraphicsMisc::GetFontPointSize(m_fontAltText);

		if (nSize != nCurSize)
		{
			GraphicsMisc::VerifyDeleteObject(m_fontAltText);
			VERIFY(GraphicsMisc::CreateFont(m_fontAltText, _T("Tahoma"), nSize));
		}
	}

	CCalendarCtrlEx::DrawCells(pDC);
}

void CTaskCalendarCtrl::SetGridLineColor(COLORREF crGrid)
{
	if (crGrid != m_crGrid)
	{
		m_crGrid = crGrid;

		if (GetSafeHwnd())
			Invalidate();
	}
}

void CTaskCalendarCtrl::SetAlternateWeekColor(COLORREF crAltWeek)
{
	if (crAltWeek != m_crAltWeek)
	{
		m_crAltWeek = crAltWeek;

		if (GetSafeHwnd())
			Invalidate();
	}
}

void CTaskCalendarCtrl::SetUITheme(const UITHEME& theme)
{
	m_crWeekend = theme.crWeekend;
	m_crToday = theme.crToday;

	if (GetSafeHwnd())
		Invalidate();
}

void CTaskCalendarCtrl::DrawCellBkgnd(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell, BOOL bSelected, BOOL bToday)
{
	if (HasColor(m_crAltWeek) && (CDateHelper::GetWeekofYear(pCell->date) % 2))
	{
		CRect rWeek(rCell);

		// Don't overdraw gridlines
		rWeek.top++;
		
		if (rWeek.left > 0)
			rWeek.left++;

		pDC->FillSolidRect(rWeek, m_crAltWeek);
	}

	BOOL bWeekend = (HasColor(m_crWeekend) && CWeekend().IsWeekend(pCell->date));

	bToday &= HasColor(m_crToday);

	if (bToday)
	{
		CRect rToday(rCell);

		// Don't overdraw gridline
		if (rToday.left > 0)
			rToday.left++;

		GraphicsMisc::DrawRect(pDC, rToday, m_crToday, CLR_NONE, 0, GMDR_NONE, 128);
	}
	else if (bWeekend)
	{
		GraphicsMisc::DrawRect(pDC, rCell, m_crWeekend, CLR_NONE, 0, GMDR_NONE, 128);
	}

	if (bSelected && !m_bSavingToImage)
	{
		GraphicsMisc::DrawRect(pDC, rCell, m_crTheme, CLR_NONE, 0, GMDR_NONE, (BYTE)(bToday ? 48 : 128));
	}
}

void CTaskCalendarCtrl::OnSize(UINT nType, int cx, int cy)
{
	CCalendarCtrlEx::OnSize(nType, cx, cy);

	RecalcCellHeaderDateFormats();
}

void CTaskCalendarCtrl::RecalcCellHeaderDateFormats()
{
	CRect rCells;
	GetClientRect(rCells);

	int nAvailWidth = ((rCells.Width() / CALENDAR_NUM_COLUMNS) - (2 * HEADER_PADDING));

	// Note: We use a bold font and the first day of the month 
	// for our calculations because experience tells that it 
	// always produces wider text than double digit non-bold text
	CClientDC dc(this);
	CFont *pOldFont = dc.SelectObject(m_fonts.GetFont(GMFS_BOLD));
	
	BOOL bMonthBeforeDay = Misc::ShortDateFormatHasMonthBeforeDay();
	int nMaxDayWidth = dc.GetTextExtent(_T("1 ")).cx;
	int nMaxLongMonthWidth = CDateHelper::GetMaxMonthNameWidth(&dc, FALSE);
	int nLongYearWidth = dc.GetTextExtent(_T(" 2020")).cx;

	int nWeekWidth = 0;

	if (HasOption(TCCO_SHOWWEEKNUMINCELLDATE))
	{
		m_sCellDateWeekNumFormat = Misc::Format(_T(" %s%%d "), CEnString(IDS_SHORTWEEK));
		nWeekWidth = dc.GetTextExtent(Misc::Format(m_sCellDateWeekNumFormat, 52)).cx;
	}
	else
	{
		m_sCellDateWeekNumFormat.Empty();
	}

	m_sCellDateFormat = _T("%#d"); // Day only

	// Note: we include the week number width in the calculation
	// but leave it out of the format string because we need to
	// use our corrected week number not Windows'
	if ((nMaxDayWidth + nMaxLongMonthWidth + nLongYearWidth + nWeekWidth) <= nAvailWidth)
	{
		m_sCellDateFormat = (bMonthBeforeDay ? _T("%B %#d %Y") : _T("%#d %B %Y"));
	}
	else
	{
		int nMaxShortMonthWidth = CDateHelper::GetMaxMonthNameWidth(&dc, TRUE);

		if ((nMaxDayWidth + nMaxShortMonthWidth + nLongYearWidth + nWeekWidth) <= nAvailWidth)
		{
			m_sCellDateFormat = (bMonthBeforeDay ? _T("%b %#d %Y") : _T("%#d %b %Y"));
		}
		else
		{
			CString sDateSep = Misc::GetDateSeparator();
			CString sDayMonthFormat;

			if (bMonthBeforeDay)
				sDayMonthFormat = _T("%m") + sDateSep + _T("%#d");
			else
				sDayMonthFormat = _T("%#d") + sDateSep + _T("%m");

			if ((dc.GetTextExtent(_T("1/12/2020")).cx + nWeekWidth) <= nAvailWidth)
			{
				m_sCellDateFormat = sDayMonthFormat + sDateSep + _T("%Y");
			}
			else if ((dc.GetTextExtent(_T("1/12/20")).cx + nWeekWidth) <= nAvailWidth)
			{
				m_sCellDateFormat = sDayMonthFormat + sDateSep + _T("%y");
			}
			else if ((dc.GetTextExtent(_T("1/12")).cx + nWeekWidth) <= nAvailWidth)
			{
				m_sCellDateFormat = sDayMonthFormat;
			}
			else if ((nMaxDayWidth + nWeekWidth) <= nAvailWidth)
			{
				// Day Only + week
			}
			else if (dc.GetTextExtent(_T("1/12")).cx <= nAvailWidth)
			{
				m_sCellDateFormat = sDayMonthFormat;
				m_sCellDateWeekNumFormat.Empty();
			}
			else 
			{
				// Day only
				m_sCellDateWeekNumFormat.Empty();
			}
		}
	}
	dc.SelectObject(pOldFont);
}

CString CTaskCalendarCtrl::FormatCellDate(const COleDateTime& date, BOOL bShowMonth, CString& sWeekNum) const
{
	ASSERT(m_sCellDateFormat);

	CString sDate = date.Format(bShowMonth ? m_sCellDateFormat : _T("%#d"));

	// Show the week number on the first of the week -> First column
	sWeekNum.Empty();

	if (!m_sCellDateWeekNumFormat.IsEmpty())
	{
		int nUnused = -1, nCol = -1;
		
		if (CCalendarCtrl::GetGridCell(date, nUnused, nCol) && (nCol == 0))
			sWeekNum = Misc::Format(m_sCellDateWeekNumFormat, CDateHelper::GetWeekofYear(date));
	}

	return sDate;
}

void CTaskCalendarCtrl::DrawCellHeader(CDC* pDC, const CCalendarCell* pCell, const CRect& rHeader, BOOL bShowMonth)
{
	bShowMonth |= HasOption(TCCO_SHOWDATEINEVERYCELL);

	CString sWeekNum, sDate = FormatCellDate(pCell->date, bShowMonth, sWeekNum);

	pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	pDC->SetBkMode(TRANSPARENT);

	CRect rDate(rHeader);
	rDate.DeflateRect(HEADER_PADDING, 3);

	if (pCell->date.GetDay() == 1)
	{
		// Draw the first of any month in bold
		CFont* pOldFont = pDC->SelectObject(m_fonts.GetFont(GMFS_BOLD));

		pDC->DrawText(sDate, &rDate, DT_LEFT | DT_VCENTER);
		pDC->SelectObject(pOldFont);
	}
	else
	{
		pDC->DrawText(sDate, &rDate, DT_LEFT | DT_VCENTER);
	}

	// Draw week always in non-bold font
	if (!sWeekNum.IsEmpty())
		pDC->DrawText(sWeekNum, &rDate, DT_RIGHT | DT_VCENTER);
}

void CTaskCalendarCtrl::DrawCellContent(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell, 
										BOOL bSelected, BOOL bToday)
{
	// default drawing
	CCalendarCtrlEx::DrawCellContent(pDC, pCell, rCell, bSelected, bToday);

	// then ours
	if (!m_nMaxDayTaskCount)
		return;
	
#ifdef _DEBUG
	int nDay = pCell->date.GetDay();
	int nMonth = pCell->date.GetMonth();
#endif

	const CTaskCalItemArray* pTasks = GetCellTasks(pCell);
	ASSERT(pTasks);
	
	int nNumTasks = (pTasks ? pTasks->GetSize() : 0);

	if (!nNumTasks)
		return;

	// Exclude scrollbar rect from drawing
	CRect rAvailCell(rCell);
	BOOL bShowScroll = (IsCellScrollBarActive() && IsGridCellSelected(pCell));
	
	if (bShowScroll)
	{
		CRect rScrollbar;
		CalcScrollBarRect(rCell, rScrollbar);

		rAvailCell.right = rScrollbar.left;
	}
	
	BOOL bContinuous = HasOption(TCCO_DISPLAYCONTINUOUS);
	BOOL bVScrolled = (bShowScroll || bContinuous);
	BOOL bFocused = CDialogHelper::IsChildOrSame(this, GetFocus());
	BOOL bTextColorIsBkgnd = HasOption(TCCO_TASKTEXTCOLORISBKGND);
	
	int nTaskHeight = GetTaskHeight();
	
	for (int nTask = 0; nTask < nNumTasks; nTask++)
	{
		CRect rTask;
		
		if (!CalcTaskCellRect(nTask, pCell, rAvailCell, rTask))
			continue;

		const TASKCALITEM* pTCI = pTasks->GetAt(nTask);
		ASSERT(pTCI);

		DWORD dwTaskID = pTCI->GetTaskID();
		ASSERT(dwTaskID);

		// draw selection
		BOOL bSelTask = WantDrawTaskSelected(pTCI);
		COLORREF crText = pTCI->GetTextColor(bSelTask, bTextColorIsBkgnd);

		if (bSelTask)
		{
 			DWORD dwSelFlags = GMIB_THEMECLASSIC;
 			CRect rClip(rAvailCell);

			if (rTask.left <= rAvailCell.left)
			{
				if (bContinuous)
				{
					// draw over gridline
					rTask.left--; 
					rClip.left--;
				}

				dwSelFlags |= GMIB_CLIPLEFT;
			}

			if (rTask.right >= rAvailCell.right)
				dwSelFlags |= GMIB_CLIPRIGHT;

			GM_ITEMSTATE nState = GMIS_SELECTED;
			
			if (!bFocused)
			{
				nState = GMIS_SELECTEDNOTFOCUSED;
			}
			else if (IsFutureOccurrence(pTCI))
			{
				nState = GMIS_DROPHILITED;
			}

			crText = GraphicsMisc::GetExplorerItemSelectionTextColor(crText, nState, GMIB_THEMECLASSIC);

			GraphicsMisc::DrawExplorerItemSelection(pDC, *this, nState, rTask, dwSelFlags, rClip);
		}
		else // draw task border/background
		{
			DWORD dwBorders = GMDR_TOP;
			
			if (rTask.left > rAvailCell.left)
			{
				dwBorders |= GMDR_LEFT;
			}
			else if (bContinuous)
			{
				rTask.left--; // draw over gridline
			}
			
			if (rTask.right < rAvailCell.right)
				dwBorders |= GMDR_RIGHT;
			
			if (rTask.bottom < rAvailCell.bottom)
				dwBorders |= GMDR_BOTTOM;
			
			COLORREF crFill = pTCI->GetFillColor(bTextColorIsBkgnd);
			COLORREF crBorder = pTCI->GetBorderColor(bTextColorIsBkgnd);
						
			int nBorderStyle = PS_SOLID;
			
			if (IsFutureOccurrence(pTCI))
			{
				// For some unknown reason PS_DOT style draws 1 pixel wider
				nBorderStyle = PS_DOT;
				rTask.right--;
			}

			GraphicsMisc::DrawRect(pDC, rTask, crFill, crBorder, 0, dwBorders, 255, nBorderStyle);
		}

		// draw icon and text
		CONTINUOUSDRAWINFO& cdi = GetTaskContinuousDrawInfo(dwTaskID);

		if (cdi.nTextOffset == -1) // text (and icon) have been drawn
			continue;

		if ((nTaskHeight >= DEF_TASK_HEIGHT) && pTCI->HasIcon(HasOption(TCCO_SHOWPARENTTASKSASFOLDER)))
		{
			// draw at the start only
			const int nReqWidth = (IMAGE_SIZE + 1);

			if ((cdi.nIconOffset >= 0) && (cdi.nIconOffset < nReqWidth))
			{
				DWORD dwRealTaskID = GetRealTaskID(dwTaskID);

				int iImageIndex = -1;
				HIMAGELIST hilTask = (HIMAGELIST)GetParent()->SendMessage(WM_CALENDAR_GETTASKICON, dwRealTaskID, (LPARAM)&iImageIndex);

				if (hilTask && (iImageIndex != -1))
				{
					CRect rIcon(rTask), rClip(rTask);

					rClip.DeflateRect(1, 1);
					rIcon.OffsetRect(-cdi.nIconOffset, 0);

					if (bContinuous)
					{
						if (cdi.nIconOffset != 0)
							rClip.left--; // draw over gridline

						if (rTask.right >= rAvailCell.right)
							rClip.right++; // draw over gridline
					}
					
					CSaveDC sdc(pDC);
					pDC->IntersectClipRect(rClip);

					ImageList_Draw(hilTask, iImageIndex, *pDC, rIcon.left + 1, rIcon.top + 1, ILD_TRANSPARENT);

					if (rIcon.Width() < nReqWidth)
					{
						cdi.nIconOffset = rTask.Width();
					}
					else
					{
						cdi.nIconOffset = -1;
						rTask.left = (rIcon.left + nReqWidth);
					}
				}
				else
				{
					TASKCALITEM* pTCIReal = GetTaskCalItem(dwRealTaskID);
					ASSERT(pTCIReal);

					if (pTCIReal)
						pTCIReal->DisableIcon();
				}
			}
		}

		// draw text if there is enough space and either there 
		// is no icon or it's been completely drawn
		if ((nTaskHeight >= MIN_TASK_HEIGHT) && !rTask.IsRectEmpty() && (cdi.nIconOffset <= 0))
		{
			int nLeft = rTask.left, nTop = (rTask.top + 2);

			if (cdi.nTextOffset == 0)
			{
				rTask.left += TEXT_PADDING; // initial pos
				nLeft = rTask.left;
			}
			else
			{
				nLeft -= cdi.nTextOffset;
			}

			pDC->SelectObject(GetTaskFont(pTCI));
			pDC->SetTextColor(crText);

			CString sTitle = pTCI->GetName();
#ifdef _DEBUG
			sTitle += Misc::Format(_T(" (%ld)"), dwTaskID);
#endif
			pDC->ExtTextOut(nLeft, nTop, ETO_CLIPPED, rTask, sTitle, NULL);

			// update text pos
			cdi.nTextOffset += rTask.Width();

			// if the offset now exceeds the text extent we can stop
			int nExtent = pDC->GetTextExtent(sTitle).cx;

			if (cdi.nTextOffset >= nExtent)
				cdi.nTextOffset = -1;
						
			if (rTask.bottom >= rAvailCell.bottom)
				break;
		}
	}
	
	// Show an overflow button if not showing the scrollbar
	if (!bShowScroll)
	{
		int nNumItems = CalcEffectiveCellContentItemCount(pCell);

		if ((nNumItems * nTaskHeight) > rCell.Height())
		{
			CRect rOverflow;
			CalcOverflowBtnRect(rCell, rOverflow);

			CThemed::DrawFrameControl(this, pDC, rOverflow, DFC_SCROLL, DFCS_SCROLLDOWN, rCell);
		}
	}
}

BOOL CTaskCalendarCtrl::WantDrawTaskSelected(const TASKCALITEM* pTCI) const
{
	if (m_bSavingToImage)
		return FALSE;

	if (m_dwSelectedTaskID == 0)
		return FALSE;

	DWORD dwTaskID = pTCI->GetTaskID();

	if (dwTaskID == m_dwSelectedTaskID)
		return TRUE;

	// Show real task as selected when a future item 
	// is selected and vice versa
	const TASKCALITEM* pTCISel = GetTaskCalItem(m_dwSelectedTaskID);

	if (!IsCustomDate(pTCISel))
	{
		DWORD dwRealID = GetRealTaskID(dwTaskID);
		DWORD dwSelRealID = GetRealTaskID(m_dwSelectedTaskID);

		if (IsFutureOccurrence(pTCI))
			return (dwSelRealID == dwRealID);

		if (IsFutureOccurrence(pTCISel))
			return (dwSelRealID == dwTaskID);
	}

	// all else
	return FALSE;
}

CFont* CTaskCalendarCtrl::GetTaskFont(const TASKCALITEM* pTCI)
{
	if (m_fontAltText.GetSafeHandle())
		return &m_fontAltText;

	DWORD dwFlags = 0;
		
	if (pTCI->IsDone(FALSE) && m_bStrikeThruDone)
		dwFlags |= GMFS_STRIKETHRU;
		
	if (pTCI->bTopLevel)
		dwFlags |= GMFS_BOLD;
		
	return m_fonts.GetFont(dwFlags);
}

void CTaskCalendarCtrl::SetStrikeThruDoneTasks(BOOL bStrikeThru)
{
	m_bStrikeThruDone = bStrikeThru;
}

void CTaskCalendarCtrl::OnSetFocus(CWnd* pFocus)
{
	CCalendarCtrlEx::OnSetFocus(pFocus);

	UpdateCellScrollBarVisibility(TRUE); // scroll to selected task
	Invalidate(FALSE);
}

void CTaskCalendarCtrl::OnKillFocus(CWnd* pFocus)
{
	CCalendarCtrlEx::OnKillFocus(pFocus);

	UpdateCellScrollBarVisibility(FALSE); // don't scroll to selected task
	Invalidate(FALSE);
}

void CTaskCalendarCtrl::DrawCellFocus(CDC* /*pDC*/, const CCalendarCell* /*pCell*/, const CRect& /*rCell*/)
{
	// we handle the focus during drawing
	// CCalendarCtrlEx::DrawCellFocus(pDC, pCell, rCell);
}

int CTaskCalendarCtrl::CalcEffectiveCellContentItemCount(const CCalendarCell* pCell) const
{
	const CTaskCalItemArray* pTasks = GetCellTasks(pCell);
	int nNumItems = 0;

	if (pTasks && pTasks->GetSize())
	{
		// Get the max task pos range
		int nLastTask = (pTasks->GetSize() - 1);
		const TASKCALITEM* pTCILast = pTasks->GetAt(nLastTask);

		int nMaxPos = GetTaskVertPos(pTCILast->GetTaskID(), nLastTask, pCell, FALSE);
		nNumItems = (nMaxPos + 1);
	}

	return nNumItems;
}

BOOL CTaskCalendarCtrl::UpdateCellScrollBarVisibility(BOOL bEnsureSelVisible)
{
	// First determine if we need to show a vertical scrollbar
	CRect rCell;
	const CCalendarCell* pCell = NULL;
	int  nRow = -1, nCol = -1, nNumItems = 0, nMaxVisItems = 0;
	BOOL bShowScrollbar = FALSE;
	
	if (CDialogHelper::IsChildOrSame(this, GetFocus()) && 
		GetLastSelectedGridCell(nRow, nCol) &&
		GetGridCellRect(nRow, nCol, rCell, TRUE))
	{
		pCell = GetCell(nRow, nCol);
		ASSERT(pCell);

		nMaxVisItems = (rCell.Height() / GetTaskHeight());
		nNumItems = CalcEffectiveCellContentItemCount(pCell);
		bShowScrollbar = (nNumItems > nMaxVisItems);

		// And scroll selected task into view
		if (bEnsureSelVisible && m_dwSelectedTaskID)
		{
			const CTaskCalItemArray* pTasks = GetCellTasks(pCell);

			if (pTasks)
			{
				int nFind = pTasks->FindItem(m_dwSelectedTaskID);

				if (nFind != -1)
				{
					int nPos = GetTaskVertPos(m_dwSelectedTaskID, nFind, pCell, FALSE);

					if (bShowScrollbar)
						m_nCellVScrollPos = min(nPos, (nNumItems - nMaxVisItems));
					else
						m_nCellVScrollPos = 0;
				}
			}
		}
	}

	// Then update the state of the scrollbar appropriately
	BOOL bWasShowingScrollbar = IsCellScrollBarActive();
	BOOL bSuccess = TRUE;

	if (!bShowScrollbar)
	{
		if (m_sbCellVScroll.GetSafeHwnd())
		{
			m_sbCellVScroll.ShowWindow(SW_HIDE);
			m_sbCellVScroll.EnableWindow(FALSE);
			m_nCellVScrollPos = 0;
		}
	}
	else // showing scrollbar
	{
		ASSERT(pCell);
		ASSERT(!rCell.IsRectEmpty());

		const CCalendarCell* pOldCell = NULL;

		CRect rNewPos;
		CalcScrollBarRect(rCell, rNewPos);

		if (!m_sbCellVScroll.GetSafeHwnd())
		{
			bSuccess = m_sbCellVScroll.Create(WS_CHILD | SBS_VERT, rNewPos, this, (UINT)IDC_STATIC);
		}
		else
		{
			CRect rPrevPos = CDialogHelper::GetChildRect(&m_sbCellVScroll);
			int nOldRow, nOldCol;

			if (HitTestGridCell(rPrevPos.CenterPoint(), nOldRow, nOldCol))
			{
				pOldCell = GetCell(nOldRow, nOldCol);
				ASSERT(pOldCell);
			}

			m_sbCellVScroll.MoveWindow(rNewPos);
		}
		
		// update scrollbar info if previously hidden or 
		// moved to a different cell
		if (bSuccess && (!bWasShowingScrollbar || (pCell != pOldCell)))
		{
			SCROLLINFO si = { sizeof(si), (SIF_PAGE | SIF_POS | SIF_RANGE) };

			si.nMin = 0;
			si.nMax = (nNumItems - 1);
			si.nPage = nMaxVisItems;

			m_nCellVScrollPos = min(m_nCellVScrollPos, (nNumItems - nMaxVisItems));
			si.nPos = m_nCellVScrollPos;

			m_sbCellVScroll.EnableWindow(TRUE);
			m_sbCellVScroll.SetScrollInfo(&si, m_sbCellVScroll.IsWindowVisible());
			m_sbCellVScroll.ShowWindow(SW_SHOW);
		}
	}

	ASSERT(bSuccess);
	return bSuccess;
}

void CTaskCalendarCtrl::CalcScrollBarRect(const CRect& rCell, CRect& rScrollbar) const
{
	rScrollbar = rCell;
	rScrollbar.left = (rCell.right - GetSystemMetrics(SM_CXVSCROLL));
}

void CTaskCalendarCtrl::CalcOverflowBtnRect(const CRect& rCell, CRect& rOverflowBtn) const
{
	rOverflowBtn = rCell;
	rOverflowBtn.left = (rCell.right - GetSystemMetrics(SM_CXVSCROLL));
	rOverflowBtn.top = (rCell.bottom - GetSystemMetrics(SM_CYVSCROLL));
}

BOOL CTaskCalendarCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (m_tooltip.GetSafeHwnd())
		m_tooltip.Pop();

	if (zDelta != 0)
	{
		int nLine = max(1, (abs(zDelta) / 120));
		UINT nDir = ((zDelta < 0) ? SB_LINEDOWN : SB_LINEUP);

		// if the mouse is over the cell with the scrollbar
		// then pass to the scrollbar instead
		ScreenToClient(&pt);

		int nRow, nCol;

		if (HitTestGridCell(pt, nRow, nCol) &&
			IsGridCellSelected(nRow, nCol) &&
			IsCellScrollBarActive() &&
			!Misc::IsKeyPressed(VK_CONTROL))
		{
			while (nLine--)
				SendMessage(WM_VSCROLL, nDir, (LPARAM)m_sbCellVScroll.GetSafeHwnd());
		}
		else
		{
			while (nLine--)
				SendMessage(WM_VSCROLL, nDir);
		}
		
		return TRUE; // eat
	}

	return CCalendarCtrlEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CTaskCalendarCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == &m_sbCellVScroll)
	{
		CRect rCell;
		VERIFY(GetGridCellRect(0, 0, rCell, TRUE));

		SCROLLINFO si = { sizeof(si), 0 };
		m_sbCellVScroll.GetScrollInfo(&si, (SIF_PAGE | SIF_POS | SIF_RANGE));

		switch (nSBCode)
		{
		case SB_BOTTOM:			m_nCellVScrollPos = si.nMax; break;
		case SB_TOP:			m_nCellVScrollPos = 0; break;

		case SB_LINEDOWN:		m_nCellVScrollPos += 1; break;
		case SB_LINEUP:			m_nCellVScrollPos -= 1; break;

		case SB_PAGEDOWN:		m_nCellVScrollPos += si.nPage; break;
		case SB_PAGEUP:			m_nCellVScrollPos -= si.nPage; break;

		case SB_THUMBPOSITION:	m_nCellVScrollPos = nPos; break;
		case SB_THUMBTRACK:		m_nCellVScrollPos = nPos; break;

		default:
			return;
		}

		m_nCellVScrollPos = min(m_nCellVScrollPos, (si.nMax - (int)si.nPage + 1));
		m_nCellVScrollPos = max(m_nCellVScrollPos, 0);

		if (m_nCellVScrollPos != si.nPos)
		{
			m_sbCellVScroll.SetScrollPos(m_nCellVScrollPos);
			Invalidate();
		}
		
		return;
	}

	int nCurPos = GetScrollPos(SB_VERT);
	
	CCalendarCtrlEx::OnVScroll(nSBCode, nPos, pScrollBar);
	
	if (GetScrollPos(SB_VERT) != nCurPos)
	{
		// Notify Parent
		GetParent()->SendMessage(WM_VSCROLL, nPos, (LPARAM)GetSafeHwnd());

		UpdateCellScrollBarVisibility(TRUE);
	}
}

int CTaskCalendarCtrl::GetTaskHeight() const
{
	int nHeight = m_nTaskHeight;

	if (m_nMaxDayTaskCount && HasOption(TCCO_ADJUSTTASKHEIGHTS))
	{
		CRect rCell;
		GetGridCellRect(0, 0, rCell, TRUE);

		nHeight = (rCell.Height() / m_nMaxDayTaskCount);
	}

	return max(MIN_TASK_HEIGHT, min(nHeight, m_nTaskHeight));
}

BOOL CTaskCalendarCtrl::SortBy(TDC_ATTRIBUTE nSortBy, BOOL bAscending)
{
	if (!WantSortUpdate(nSortBy))
		return FALSE;

	m_nSortBy = nSortBy;
	m_bSortAscending = bAscending;

	if (GetSafeHwnd())
		ResortCellTasks();

	return TRUE;
}

const CTaskCalItemArray* CTaskCalendarCtrl::GetCellTasks(const CCalendarCell* pCell) const
{
	if (!pCell)
	{
		ASSERT(0);
		return NULL;
	}
	
	return static_cast<CTaskCalItemArray*>(pCell->pUserData);
}

CTaskCalItemArray* CTaskCalendarCtrl::GetCellTasks(CCalendarCell* pCell)
{
	if (!pCell)
	{
		ASSERT(0);
		return NULL;
	}
	
	if (pCell->pUserData == NULL)
		pCell->pUserData = new CTaskCalItemArray();
	
	return static_cast<CTaskCalItemArray*>(pCell->pUserData);
}

const CTaskCalItemArray* CTaskCalendarCtrl::GetCellTasks(int nRow, int nCol) const
{
	return GetCellTasks(GetCell(nRow, nCol));
}

CTaskCalItemArray* CTaskCalendarCtrl::GetCellTasks(int nRow, int nCol)
{
	return GetCellTasks(GetCell(nRow, nCol));
}

int CTaskCalendarCtrl::RebuildCellTasks(BOOL bIncExtItems)
{
//	CScopedTraceTimer timing(_T("CTaskCalendarCtrl::RebuildCellTasks()"));

	// Rebuild extension items first because they are
	// needed for building the cell tasks
	if (bIncExtItems)
	{
		// Cache before clearing the map
		BOOL bSelItemIsCustomDate = IsCustomDate(m_dwSelectedTaskID);

		m_mapExtensionItems.RemoveAll();
		DWORD dwNextExtID = (((m_dwMaximumTaskID / 1000) + 1) * 1000);

		RebuildFutureOccurrences(dwNextExtID);
		RebuildCustomDates(dwNextExtID);

		// Check for disappearing selected 'custom date'
		if (bSelItemIsCustomDate &&	!HasTask(m_dwSelectedTaskID, FALSE))
		{
			m_dwSelectedTaskID = 0;
			NotifyParentSelChange();
		}
	}
	
	// Now rebuild the cell tasks
	int nTotal = 0;

	for (int i = 0; i < CALENDAR_MAX_ROWS; i++)
	{
		for (int u = 0; u < CALENDAR_NUM_COLUMNS; u++)
		{
			CCalendarCell* pCell = GetCell(i, u);
			ASSERT(pCell);

			nTotal += RebuildCellTasks(pCell);
		}
	}

	// Finally rebuild the temporary cell draw info
	RebuildCellTaskDrawInfo();

	return nTotal;
}

void CTaskCalendarCtrl::ResortCellTasks()
{
	int nTotal = 0;

	for (int i = 0; i < CALENDAR_MAX_ROWS; i++)
	{
		for (int u = 0; u < CALENDAR_NUM_COLUMNS; u++)
		{
			CTaskCalItemArray* pTasks = GetCellTasks(i, u);
			pTasks->SortItems(m_nSortBy, m_bSortAscending);
		}
	}

	Invalidate(TRUE);
}

void CTaskCalendarCtrl::RebuildFutureOccurrences(DWORD& dwNextExtID)
{
	if (HasOption(TCCO_SHOWFUTUREITEMS) && m_mapRecurringTaskIDs.GetCount())
	{
		// Go through all the known recurring tasks to see if any 
		// of them have future occurrences within the visible cells
		TASKCALFUTUREDATES dtFuture;
		VERIFY(dtFuture.dtRange.Set(GetMinDate(), GetMaxDate()));

		POSITION pos = m_mapRecurringTaskIDs.GetStartPosition();

		while (pos)
		{
			DWORD dwTaskID = m_mapRecurringTaskIDs.GetNext(pos);
			ASSERT(dwTaskID);

			TASKCALITEM* pTCIReal = GetTaskCalItem(dwTaskID); // exclude extension items

			if (!pTCIReal)
			{
				ASSERT(0);
				continue;
			}

			if (pTCIReal->IsDone(FALSE))
				continue;

			// This is the potentially expensive bit
			if (!GetParent()->SendMessage(WM_CALENDAR_GETTASKFUTUREDATES, dwTaskID, (LPARAM)&dtFuture))
				continue;

			// Create new temporary tasks for these future items
			int nItem = dtFuture.nNumOccurrences;

			while (nItem--)
			{
				TASKCALITEM* pTCIFuture = new TASKCALFUTUREOCURRENCE(*pTCIReal, dwNextExtID, dtFuture.dtOccurrences[nItem]);
				m_mapExtensionItems[dwNextExtID++] = pTCIFuture;
			}
		}
	}
}

void CTaskCalendarCtrl::RebuildCustomDates(DWORD& dwNextExtID)
{
	double dStart = GetMinDate(), dEnd = (GetMaxDate().m_dt + 1.0);
	POSITION pos = m_mapData.GetStartPosition();

	while (pos)
	{
		TASKCALITEM* pTCI = m_mapData.GetNextTask(pos);
		ASSERT(pTCI);

		if (pTCI->IsParent() && (HasOption(TCCO_HIDEPARENTTASKS)))
			continue;

		// ignore completed tasks as required
		if (pTCI->IsDone(TRUE) && !HasOption(TCCO_DISPLAYDONE))
			continue;

		POSITION posDate = pTCI->Dates().Custom().GetStartPosition();

		while (posDate)
		{
			CString sCustAttribID;
			COleDateTime date;

			pTCI->Dates().Custom().GetNextAssoc(posDate, sCustAttribID, date);

			if ((date.m_dt >= dStart) && (date.m_dt < dEnd))
			{
				TASKCALCUSTOMDATE* pTCIDate = new TASKCALCUSTOMDATE(*pTCI, dwNextExtID, sCustAttribID, date);
				m_mapExtensionItems[dwNextExtID++] = pTCIDate;
			}
		}
	}
}

void CTaskCalendarCtrl::RebuildCellTaskDrawInfo()
{
	m_nMaxDayTaskCount = 0;
	m_ContinuousDrawInfo.RemoveAll();

	if (m_mapData.GetCount() == 0)
		return;

	for (int i = 0; i < CALENDAR_MAX_ROWS; i++)
	{
		for (int u = 0; u < CALENDAR_NUM_COLUMNS; u++)
		{
			CTaskCalItemArray* pTasks = GetCellTasks(i, u);
			ASSERT(pTasks);

			if (pTasks && pTasks->GetSize())
			{
				if (HasOption(TCCO_DISPLAYCONTINUOUS))
				{
					// Determine which slots are already used
					CTaskCalItemArray aUsedSlots;
					int nTask;

					for (nTask = 0; nTask < pTasks->GetSize(); nTask++)
					{
						TASKCALITEM* pTCI = pTasks->GetAt(nTask);
						ASSERT(pTCI);

						DWORD dwTaskID = pTCI->GetTaskID();
						const CONTINUOUSDRAWINFO& cdi = GetTaskContinuousDrawInfo(dwTaskID);

						if (cdi.nVertPos != -1)
						{
							if (cdi.nVertPos >= aUsedSlots.GetSize())
								aUsedSlots.SetSize(cdi.nVertPos + 1);

							aUsedSlots[cdi.nVertPos] = pTCI;
						}
					}

					// Now go thru the list and set the position of each item 
					// (if not already done) keeping track of the next available slot
					int nNextSlot = Misc::FindT((TASKCALITEM*)NULL, aUsedSlots);
					BOOL bHadEmptySlots = (nNextSlot != -1);

					if (nNextSlot == -1)
						nNextSlot = aUsedSlots.GetSize();

					for (nTask = 0; nTask < pTasks->GetSize(); nTask++)
					{
						TASKCALITEM* pTCI = pTasks->GetAt(nTask);
						ASSERT(pTCI);

						DWORD dwTaskID = pTCI->GetTaskID();
						CONTINUOUSDRAWINFO& cdi = GetTaskContinuousDrawInfo(dwTaskID);

						if (cdi.nVertPos == -1)
						{
							cdi.nVertPos = nNextSlot;

							if (nNextSlot == aUsedSlots.GetSize())
							{
								// We've passed the end of the allocated slots
								// so just keep adding to the end
								aUsedSlots.Add(pTCI);
								nNextSlot++;
							}
							else
							{
								ASSERT(aUsedSlots[nNextSlot] == NULL);

								aUsedSlots[nNextSlot] = pTCI;

								int nNextAvailSlot = Misc::FindT((TASKCALITEM*)NULL, aUsedSlots, nNextSlot + 1);

								// If no further slots are available we start adding to the end
								if (nNextAvailSlot == -1)
									nNextSlot = aUsedSlots.GetSize();
								else
									nNextSlot = nNextAvailSlot;
							}
						}
					}

					m_nMaxDayTaskCount = max(m_nMaxDayTaskCount, aUsedSlots.GetSize());

					// Copy the reordered tasks after removing any remaining empty slots
					if (bHadEmptySlots)
					{
						if (aUsedSlots.GetSize() > pTasks->GetSize())
							Misc::RemoveAllT((TASKCALITEM*)NULL, aUsedSlots);
						else
							ASSERT(Misc::FindT((TASKCALITEM*)NULL, aUsedSlots) == -1);

						pTasks->Copy(aUsedSlots);
					}
				}
				else
				{
					// else pos is just task index
					m_nMaxDayTaskCount = max(m_nMaxDayTaskCount, pTasks->GetSize());
				}
			}
		}
	}
}

int CTaskCalendarCtrl::RebuildCellTasks(CCalendarCell* pCell)
{
	ASSERT(pCell);

	CTaskCalItemArray* pTasks = GetCellTasks(pCell);
	pTasks->RemoveAll();

	// 'Real' tasks
	AddTasksToCell(m_mapData, pCell->date, pTasks);

	// Extension tasks
	AddTasksToCell(m_mapExtensionItems, pCell->date, pTasks);

	// Sort them in order of task start date
	pTasks->SortItems(m_nSortBy, m_bSortAscending);

	return pTasks->GetSize();
}

void CTaskCalendarCtrl::AddTasksToCell(const CTaskCalItemMap& mapTasks, const COleDateTime& dtCell, CTaskCalItemArray* pTasks)
{
#ifdef _DEBUG
	int nDay = dtCell.GetDay();
	int nMonth = dtCell.GetMonth();
#endif

	double dCellStart = dtCell, dCellEnd = (dCellStart + 1.0);
	POSITION pos = mapTasks.GetStartPosition();

	while (pos)
	{
		TASKCALITEM* pTCI = mapTasks.GetNextTask(pos);
		ASSERT(pTCI);

		// Task Visibility
		if (IsHiddenTask(pTCI, TRUE))
			continue;

		if (HasOption(TCCO_DISPLAYCONTINUOUS))
		{
			if ((pTCI->GetAnyStartDate().m_dt < dCellEnd) && 
				(pTCI->GetAnyEndDate().m_dt >= dCellStart))
			{
				pTasks->Add(pTCI);
			}
		}
		else
		{
			BOOL bAdded = FALSE; // only add once

			if (HasOption(TCCO_DISPLAYCALCSTART) || (HasOption(TCCO_DISPLAYSTART) && pTCI->IsStartDateSet()))
			{
				if (CDateHelper::GetDateOnly(pTCI->GetAnyStartDate()).m_dt == dCellStart)
				{
					pTasks->Add(pTCI);
					bAdded = TRUE;
				}
			}

			// only test for due/done if start not added
			if (!bAdded)
			{
				if (HasOption(TCCO_DISPLAYCALCDUE) || (HasOption(TCCO_DISPLAYDUE) && pTCI->IsEndDateSet()))
				{
					if (CDateHelper::GetDateOnly(pTCI->GetAnyEndDate()).m_dt == dCellStart)
					{
						pTasks->Add(pTCI);
						bAdded = TRUE;
					}
				}

				if (!bAdded && HasOption(TCCO_DISPLAYDONE) && pTCI->IsDone(FALSE))
				{
					if (CDateHelper::GetDateOnly(pTCI->GetDoneDate()).m_dt == dCellStart)
						pTasks->Add(pTCI);
				}
			}
		}
	}
}

DWORD CTaskCalendarCtrl::HitTestTask(const CPoint& ptClient, BOOL& bCustomDate) const
{
	TCC_HITTEST nHit = TCCHT_NOWHERE;
	DWORD dwTaskID = HitTestTask(ptClient, nHit);

	if (nHit == TCCHT_NOWHERE)
		return 0;

	bCustomDate = IsCustomDate(dwTaskID);

	if (!bCustomDate)
		dwTaskID = GetRealTaskID(dwTaskID);

	return dwTaskID;
}

DWORD CTaskCalendarCtrl::HitTestTask(const CPoint& ptClient, TCC_HITTEST& nHit, LPRECT pRect) const
{
	nHit = TCCHT_NOWHERE;

	if (!m_nMaxDayTaskCount)
		return 0;

	int nRow, nCol;

	if (!HitTestGridCell(ptClient, nRow, nCol))
		return 0;

	const CCalendarCell* pCell = GetCell(nRow, nCol);
	ASSERT(pCell);
	
	if (pCell == NULL)
		return 0;

	// handle clicking above tasks
	CRect rCell;
	GetGridCellRect(nRow, nCol, rCell, TRUE);

	if (ptClient.y < rCell.top)
		return 0;

	// exclude visible scrollbar
	if (IsCellScrollBarActive() && IsGridCellSelected(pCell))
	{
		CRect rScroll;
		CalcScrollBarRect(rCell, rScroll);

		if (rScroll.PtInRect(ptClient))
			return 0;
	}
	else // exclude 'More Tasks...' button
	{
		int nNumItems = CalcEffectiveCellContentItemCount(pCell);

		if ((nNumItems * GetTaskHeight()) > rCell.Height())
		{
			CRect rOverflow;
			CalcOverflowBtnRect(rCell, rOverflow);

			if (rOverflow.PtInRect(ptClient))
				return 0;
		}
	}

	const CTaskCalItemArray* pTasks = static_cast<CTaskCalItemArray*>(pCell->pUserData);
	ASSERT(pTasks);

	if (!pTasks || !pTasks->GetSize())
		return 0;

	// Find the task beneath the mouse
	for (int nTask = 0; nTask < pTasks->GetSize(); nTask++)
	{
		CRect rTask;

		if (!CalcTaskCellRect(nTask, pCell, rCell, rTask))
			continue;

		if (rTask.PtInRect(ptClient))
		{
			// now check for closeness to ends
			COleDateTime dtHit;
			VERIFY(GetDateFromPoint(ptClient, dtHit));

			double dDateTol = CalcDateDragTolerance();
			const TASKCALITEM* pTCI = pTasks->GetAt(nTask);

			if (fabs(dtHit.m_dt - pTCI->GetAnyStartDate().m_dt) < dDateTol)
			{
				nHit = TCCHT_BEGIN;
			}
			else if (fabs(dtHit.m_dt - pTCI->GetAnyEndDate().m_dt) < dDateTol)
			{
				nHit = TCCHT_END;
			}
			else if (dtHit > pTCI->GetAnyStartDate() && dtHit < pTCI->GetAnyEndDate())
			{
				nHit = TCCHT_MIDDLE;
			}
			else
			{
				ASSERT(0);
				break;
			}

			if (pRect)
				*pRect = rTask;

			return pTCI->GetTaskID();
		}
	}

	// nothing hit
	return 0;
}

BOOL CTaskCalendarCtrl::HitTestCellOverflowBtn(const CPoint& ptClient) const
{
	CRect rBtn;
	return HitTestCellOverflowBtn(ptClient, rBtn);
}

BOOL CTaskCalendarCtrl::HitTestCellOverflowBtn(const CPoint& ptClient, CRect& rBtn) const
{
	int nRow, nCol;

	if (!HitTestGridCell(ptClient, nRow, nCol))
		return FALSE;

	const CCalendarCell* pCell = GetCell(nRow, nCol);
	ASSERT(pCell);

	if (IsGridCellSelected(pCell))
		return FALSE;

	CRect rCell;
	VERIFY(GetGridCellRect(nRow, nCol, rCell, TRUE)); // without header

	int nNumCellItems = CalcEffectiveCellContentItemCount(pCell);

	if ((nNumCellItems * GetTaskHeight()) <= rCell.Height())
		return FALSE;

	CalcOverflowBtnRect(rCell, rBtn);

	if (rBtn.PtInRect(ptClient))
		return TRUE;

	rBtn.SetRectEmpty();
	return FALSE;
}

int CTaskCalendarCtrl::GetTaskVertPos(DWORD dwTaskID, int nTask, const CCalendarCell* pCell, BOOL bScrolled) const
{
	if (!dwTaskID || !pCell)
	{
		ASSERT(0);
		return nTask;
	}

	int nPos = nTask;

	if (HasOption(TCCO_DISPLAYCONTINUOUS))
	{
		nPos = GetTaskContinuousDrawInfo(dwTaskID).nVertPos;
	}
	else if (nPos == -1)
	{
		const CTaskCalItemArray* pTasks = GetCellTasks(pCell);
		ASSERT(pTasks);

		if (pTasks)
			nPos = pTasks->FindItem(dwTaskID);
	}

	BOOL bVScrolled = (bScrolled && (IsGridCellSelected(pCell) || HasOption(TCCO_DISPLAYCONTINUOUS)));

	if (bVScrolled)
		nPos -= m_nCellVScrollPos;

	return nPos;
}

double CTaskCalendarCtrl::CalcDateDragTolerance() const
{
	CRect rClient;
	GetClientRect(rClient);

	// calc equivalent of DRAG_WIDTH in days
	double dOneDay = ((double)rClient.Width() / CALENDAR_NUM_COLUMNS);
	double dDragTol = (GetSystemMetrics(SM_CXSIZEFRAME) / dOneDay);

	return min(dDragTol, 1.0);
}

BOOL CTaskCalendarCtrl::EnsureSelectionVisible()
{
	if (!HasTask(m_dwSelectedTaskID, TRUE)) // exclude hidden tasks
		return FALSE;

	// Does the selected cell already have this task?
	int nRow, nCol;

	if (GetLastSelectedGridCell(nRow, nCol) && (nRow < m_nVisibleWeeks))
	{
		const CTaskCalItemArray* pTasks = GetCellTasks(nRow, nCol);
		ASSERT(pTasks);

		if (pTasks && (pTasks->FindItem(m_dwSelectedTaskID) != -1))
		{ 
			UpdateCellScrollBarVisibility(TRUE); // scroll to selected task
			return TRUE;
		}
	}
	
	// else is the task already visible to some degree?
	if (GetGridCell(m_dwSelectedTaskID, nRow, nCol))
	{
		VERIFY(SelectGridCell(nRow, nCol));
	}
	else // scroll selected task date into view
	{
		const TASKCALITEM* pTCI = GetTaskCalItem(m_dwSelectedTaskID);

		if (!pTCI)
		{
			ASSERT(0);
			return FALSE;
		}

		COleDateTime dtCellsMin = GetMinDate();
		COleDateTime dtCellsMax = GetMaxDate();

		COleDateTime dtStart = pTCI->GetAnyStartDate();
		COleDateTime dtEnd = pTCI->GetAnyEndDate();

		if (dtEnd < dtCellsMin)
		{
			Goto(dtEnd, true); // select day
		}
		else if (dtStart > dtCellsMax)
		{
			Goto(dtStart, true); // select day
		}
		else if (!HasOption(TCCO_DISPLAYCONTINUOUS))
		{
			BOOL bHasStart = (HasOption(TCCO_DISPLAYSTART) && pTCI->IsStartDateSet());
			BOOL bHasCalcStart = (HasOption(TCCO_DISPLAYCALCSTART) && !pTCI->IsStartDateSet() && pTCI->HasAnyStartDate());

			BOOL bHasEnd = (HasOption(TCCO_DISPLAYDUE) && pTCI->IsEndDateSet());
			BOOL bHasCalcEnd = (HasOption(TCCO_DISPLAYCALCDUE) && !pTCI->IsEndDateSet() && pTCI->HasAnyEndDate());

			if ((dtStart < dtCellsMin) && (bHasStart || bHasCalcStart))
			{
				Goto(dtStart, true); // select day
			}
			else if ((dtEnd > dtCellsMax) && (bHasEnd || bHasCalcEnd))
			{
				Goto(dtEnd, true); // select day
			}
		}
		else
		{
			//ASSERT(0);
		}

		ASSERT(GetGridCell(m_dwSelectedTaskID, nRow, nCol));
	}
	
	UpdateCellScrollBarVisibility(TRUE); // scroll to selected task
	return TRUE;
}

void CTaskCalendarCtrl::OnVisibleDateRangeChanged()
{
	if (RebuildCellTasks() == 0)
		return;

	UpdateCellScrollBarVisibility(TRUE);// scroll to selected task
}

BOOL CTaskCalendarCtrl::SelectGridCell(int nRow, int nCol)
{
	int nPrevRow, nPrevCol;

	if (!GetLastSelectedGridCell(nPrevRow, nPrevCol) ||
		(nRow != nPrevRow) || (nCol != nPrevCol))
	{
		if (!CCalendarCtrl::SelectGridCell(nRow, nCol))
			return FALSE;

		// Notify our parent of the grid cell change
		NotifyParentClick();
	}

	return TRUE;
}

BOOL CTaskCalendarCtrl::IsTaskVisible(DWORD dwTaskID) const
{
	int nUnused;
	return GetGridCell(dwTaskID, nUnused, nUnused, nUnused);
}

BOOL CTaskCalendarCtrl::GetGridCell(DWORD dwTaskID, int &nRow, int &nCol) const
{
	int nUnused;
	return GetGridCell(dwTaskID, nRow, nCol, nUnused);
}

BOOL CTaskCalendarCtrl::GetGridCell(DWORD dwTaskID, int &nRow, int &nCol, int& nTask) const
{
	nRow = nCol = nTask = -1;

	if (!dwTaskID)
		return FALSE;

	// iterate the visible cells for the specified task
	for(int i=0; i < GetVisibleWeeks() ; i++)
	{
		for(int u=0; u<CALENDAR_NUM_COLUMNS; u++)
		{
			const CTaskCalItemArray* pTasks = GetCellTasks(i, u);
			ASSERT(pTasks);

			if (pTasks)
			{
				int nTemp = pTasks->FindItem(dwTaskID);

				if (nTemp >= 0)
				{
					nRow = i;
					nCol = u;
					nTask = nTemp;

					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

BOOL CTaskCalendarCtrl::GetSelectedTaskLabelRect(CRect& rLabel) const
{
	return GetTaskLabelRect(GetRealTaskID(m_dwSelectedTaskID), rLabel);
}

BOOL CTaskCalendarCtrl::GetTaskLabelRect(DWORD dwTaskID, CRect& rLabel) const
{
	int nRow, nCol, nTask;

	// start with visibility check
	if (!GetGridCell(dwTaskID, nRow, nCol, nTask))
 		return FALSE;

	const CCalendarCell* pCell = GetCell(nRow, nCol);
	ASSERT(pCell);

	CRect rCell;
	VERIFY(GetGridCellRect(nRow, nCol, rCell, TRUE));

	if (IsGridCellSelected(pCell))
		rCell.DeflateRect(2, 0);

	if (!CalcTaskCellRect(nTask, pCell, rCell, rLabel))
		return FALSE;

	DWORD dwRealID = GetRealTaskID(dwTaskID);
	const TASKCALITEM* pTCI = GetTaskCalItem(dwRealID);
	
	if (!pTCI)
	{
		ASSERT(0);
		return FALSE;
	}

	if (pTCI->HasIcon(HasOption(TCCO_SHOWPARENTTASKSASFOLDER)))
		rLabel.left += (IMAGE_SIZE + TIP_PADDING);

	return TRUE;
}

BOOL CTaskCalendarCtrl::CalcTaskCellRect(int nTask, const CCalendarCell* pCell, const CRect& rCell, CRect& rTask) const
{
	const CTaskCalItemArray* pTasks = GetCellTasks(pCell);

	// Sanity checks
	if (!pTasks || (nTask < 0) || (nTask >= pTasks->GetSize()))
	{
		ASSERT(0);
		return FALSE;
	}

	// check horizontal (date) intersection first
	const TASKCALITEM* pTCI = pTasks->GetAt(nTask);
	ASSERT(pTCI);

	double dCellStart = pCell->date;
	double dCellEnd = CDateHelper::GetEndOfDay(dCellStart).m_dt;

	if ((pTCI->GetAnyEndDate().m_dt < dCellStart) || (pTCI->GetAnyStartDate().m_dt > dCellEnd))
		return FALSE;

	// check vertical (pos) intersection next
	int nPos = GetTaskVertPos(pTCI->GetTaskID(), nTask, pCell, IsCellScrollBarActive());

	if (nPos < 0)
	{
		ASSERT(m_nCellVScrollPos > 0);
		return FALSE;
	}
	ASSERT(nPos < m_nMaxDayTaskCount);

	// Check bottom of task against cell boundary
	int nTaskHeight = GetTaskHeight();

	if ((nPos * nTaskHeight) >= rCell.bottom)
		return FALSE;

	// calc rest of sides
	rTask = rCell;

	rTask.top += (nPos * nTaskHeight);
	rTask.bottom = min(rCell.bottom, (rTask.top + nTaskHeight - 1));

	// left edge
	if (pTCI->GetAnyStartDate().m_dt == dCellStart)
	{ 
		// whole day
		rTask.left++;
	}
	else if (pTCI->GetAnyStartDate().m_dt > dCellStart)
	{ 
		// partial day
		rTask.left += (int)((pTCI->GetAnyStartDate().m_dt - dCellStart) * rCell.Width());
	}	
			
	// right edge
	if (pTCI->GetAnyEndDate().m_dt == dCellEnd) // whole day
	{
		rTask.right--;
	}
	else if (pTCI->GetAnyEndDate().m_dt < dCellEnd)
	{
		// partial day
		rTask.right -= (int)((dCellEnd - pTCI->GetAnyEndDate().m_dt) * rCell.Width());
	}

	return TRUE;
}

CONTINUOUSDRAWINFO& CTaskCalendarCtrl::GetTaskContinuousDrawInfo(DWORD dwTaskID) const
{
	ASSERT(dwTaskID);

	if (!HasOption(TCCO_DISPLAYCONTINUOUS) || !dwTaskID)
	{
		static CONTINUOUSDRAWINFO nullDrawInfo;
		nullDrawInfo.Reset();

		return nullDrawInfo;
	}

	return m_ContinuousDrawInfo.GetTaskInfo(dwTaskID);
}

TASKCALITEM* CTaskCalendarCtrl::GetTaskCalItem(DWORD dwTaskID) const
{
	ASSERT(dwTaskID);
	TASKCALITEM* pTCI = m_mapData.GetTaskItem(dwTaskID);

	if (!pTCI && IsExtensionItem(dwTaskID))
	{
		pTCI = m_mapExtensionItems.GetTaskItem(dwTaskID);
		ASSERT(pTCI);
	}

	return pTCI;
}

BOOL CTaskCalendarCtrl::IsHiddenTask(const TASKCALITEM* pTCI, BOOL bCheckValid) const
{
	if (bCheckValid && !pTCI->IsValid())
		return TRUE;

	if (pTCI->IsParent() && (HasOption(TCCO_HIDEPARENTTASKS)))
	{
		if (m_sHideParentTag.IsEmpty() || pTCI->HasTag(m_sHideParentTag))
			return TRUE;
	}

	if (pTCI->IsDone(TRUE) && !HasOption(TCCO_DISPLAYDONE))
		return TRUE;

	return FALSE;
}

BOOL CTaskCalendarCtrl::HasTask(DWORD dwTaskID, BOOL bExcludeHidden) const
{
	if (dwTaskID == 0)
		return FALSE;

	TASKCALITEM* pTCI = NULL;
	
	if (!m_mapData.Lookup(dwTaskID, pTCI) && 
		!m_mapExtensionItems.Lookup(dwTaskID, pTCI))
	{
		return FALSE;
	}

	if (bExcludeHidden && IsHiddenTask(pTCI, TRUE))
		return FALSE;

	return TRUE;
}

// external version
BOOL CTaskCalendarCtrl::SelectTask(DWORD dwTaskID, BOOL bEnsureVisible)
{
	SelectTask(dwTaskID, bEnsureVisible, FALSE);

	return (GetSelectedTaskID() != 0);
}

// internal version
BOOL CTaskCalendarCtrl::SelectTask(DWORD dwTaskID, BOOL bEnsureVisible, BOOL bNotify)
{
	if (!HasTask(dwTaskID, FALSE)) // Don't exclude hidden tasks
		return FALSE;

	DWORD dwSelTaskID = GetSelectedTaskID();

	if (dwTaskID != dwSelTaskID)
	{
		m_dwSelectedTaskID = dwTaskID;

		if (bNotify && (dwTaskID != GetRealTaskID(dwSelTaskID)))
			GetParent()->SendMessage(WM_CALENDAR_SELCHANGE, 0, GetRealTaskID(GetSelectedTaskID()));

		if (bEnsureVisible)
			EnsureSelectionVisible();

		Invalidate(FALSE);
		UpdateWindow();
	}

	return TRUE;
}

DWORD CTaskCalendarCtrl::GetSelectedTaskID() const 
{ 
	if (!m_dwSelectedTaskID)
		return 0;

	// Check visibility
	const TASKCALITEM* pTCI = GetTaskCalItem(m_dwSelectedTaskID);

	if (!pTCI || IsHiddenTask(pTCI, TRUE)) // Exclude hidden tasks
		return 0;

	// Does it have the necessary dates
	if (!HasOption(TCCO_DISPLAYCONTINUOUS))
	{
		BOOL bHasStart = (HasOption(TCCO_DISPLAYSTART) && pTCI->IsStartDateSet());
		BOOL bHasCalcStart = (HasOption(TCCO_DISPLAYCALCSTART) && !pTCI->IsStartDateSet() && pTCI->HasAnyStartDate());

		BOOL bHasEnd = (HasOption(TCCO_DISPLAYDUE) && pTCI->IsEndDateSet());
		BOOL bHasCalcEnd = (HasOption(TCCO_DISPLAYCALCDUE) && !pTCI->IsEndDateSet() && pTCI->HasAnyEndDate());

		if (!bHasStart && !bHasCalcStart &&	!bHasEnd && !bHasCalcEnd)
			return 0;
	}
	
	return m_dwSelectedTaskID;
}

void CTaskCalendarCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	BOOL bCustomDate = FALSE;
	DWORD dwSelID = HitTestTask(point, bCustomDate);
	
	if (dwSelID)
	{
		m_tooltip.Pop();

		SetFocus();
		SelectTask(dwSelID, FALSE, TRUE);

		const CCalendarCell* pCell = HitTestGridCell(point);
		ASSERT(pCell);

		if (pCell)
			SelectDate(pCell->date, FALSE);
		
		if (!m_bReadOnly && StartDragging(point))
			return;
	}

	// else
	CCalendarCtrl::OnLButtonDown(nFlags, point);
	UpdateWindow();
}

DWORD CTaskCalendarCtrl::GetRealTaskID(DWORD dwTaskID) const
{
	if (IsExtensionItem(dwTaskID))
		return m_mapExtensionItems.GetRealTaskID(dwTaskID);

	// else
	return dwTaskID;
}

BOOL CTaskCalendarCtrl::IsExtensionItem(DWORD dwTaskID) const
{
	if (dwTaskID > m_dwMaximumTaskID)
	{
		ASSERT(m_mapExtensionItems.HasTask(dwTaskID));
		return TRUE;
	}

	ASSERT(dwTaskID <= m_dwMaximumTaskID);
	return FALSE;
}

BOOL CTaskCalendarCtrl::IsFutureOccurrence(DWORD dwTaskID) const
{
	if (!IsExtensionItem(dwTaskID))
		return FALSE;

	return IsFutureOccurrence(GetTaskCalItem(dwTaskID));
}

BOOL CTaskCalendarCtrl::IsCustomDate(DWORD dwTaskID) const
{
	if (!IsExtensionItem(dwTaskID))
		return FALSE;

	return IsCustomDate(GetTaskCalItem(dwTaskID));
}

BOOL CTaskCalendarCtrl::IsExtensionItem(const TASKCALITEM* pTCI)
{
	return (dynamic_cast<const TASKCALEXTENSIONITEM*>(pTCI) != NULL);
}

BOOL CTaskCalendarCtrl::IsFutureOccurrence(const TASKCALITEM* pTCI)
{
	return (dynamic_cast<const TASKCALFUTUREOCURRENCE*>(pTCI) != NULL);
}

BOOL CTaskCalendarCtrl::IsCustomDate(const TASKCALITEM* pTCI)
{
	return (dynamic_cast<const TASKCALCUSTOMDATE*>(pTCI) != NULL);
}

BOOL CTaskCalendarCtrl::StartDragging(const CPoint& ptCursor)
{
	ASSERT(!m_bReadOnly);

	TCC_HITTEST nHit = TCCHT_NOWHERE;
	DWORD dwTaskID = HitTestTask(ptCursor, nHit);
	
	if (!CanDragTask(dwTaskID, nHit))
		return FALSE;

	const TASKCALITEM* pTCI = GetTaskCalItem(dwTaskID);
	ASSERT(pTCI);

	// when not drawing tasks continuously, it's possible
	// for the act of selecting a task to change its
	// position and thus its hit-test result
	if ((dwTaskID != m_dwSelectedTaskID) && !IsCustomDate(pTCI))
		return FALSE;

	if (nHit == TCCHT_BEGIN || nHit == TCCHT_END)
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
	
	if (!::DragDetect(GetSafeHwnd(), ptCursor))
		return FALSE;
	
	SetCapture();

	switch (nHit)
	{
	case TCCHT_BEGIN:	m_bDraggingStart = TRUE;	break;
	case TCCHT_END:		m_bDraggingEnd = TRUE;		break;
	case TCCHT_MIDDLE:	m_bDragging = TRUE;			break;
		
	default:			
		ASSERT(0);
		return FALSE;
	}
	
	m_tcidPreDrag = pTCI->Dates();
	m_ptDragOrigin = ptCursor;

	VERIFY(GetDateFromPoint(m_ptDragOrigin, m_dtDragOrigin));

	// keep parent informed
	NotifyParentDragChange();

	TRACE(_T("\nDragStart (date: %f.0, vpos: %d, scrollpos: %d)\n"), m_DateCurrent.m_dt, m_nVscrollPos, GetScrollPos(SB_VERT));

	return TRUE;
}

BOOL CTaskCalendarCtrl::GetValidDragDate(const CPoint& ptCursor, COleDateTime& dtDrag) const
{
	CPoint ptDrag(ptCursor);

	if (!ValidateDragPoint(ptDrag) || !GetDateFromPoint(ptDrag, dtDrag))
	{
		GraphicsMisc::SetDragDropCursor(GMOC_NO);
		return FALSE;
	}

	int nRow = HitTestGridRow(ptDrag);

	if (nRow == -1)
	{
		ASSERT(0);
		return FALSE;
	}

	COleDateTime dtRowMin = GetMinDate(nRow);
	COleDateTime dtRowMax = GetMaxDate(nRow);

	double dSnap = GetSnapIncrement();

	// if dragging the whole task, then we calculate
	// dtDrag as TASKCALITEM::dtStart/dtEnd offset by the
	// difference between the current drag pos and the
	// initial drag pos
	BOOL bEndOfDay = m_bDraggingEnd;

	if (m_bDragging)
	{
		// offset from pre-drag position
		double dOffset = (dtDrag.m_dt - m_dtDragOrigin.m_dt);

		if (m_tcidPreDrag.IsStartSet())
		{
			dtDrag = (m_tcidPreDrag.GetAnyStart().m_dt + dOffset);
			bEndOfDay = FALSE;
		}
		else
		{
			ASSERT(m_tcidPreDrag.IsEndSet());

			dtDrag = (m_tcidPreDrag.GetAnyEnd().m_dt + dOffset);
			bEndOfDay = TRUE;
		}
		
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
	}
	else
	{
		if (m_bDraggingStart)
		{
			dtDrag.m_dt = min(dtDrag.m_dt, (dtRowMax.m_dt + 1.0 - dSnap));
		}
		else // if (m_bDraggingEnd)
		{
			dtDrag.m_dt = max(dtDrag.m_dt, (dtRowMin.m_dt + dSnap));
		}

		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
	}
	
	// adjust date depending on snap mode
	switch (GetSnapMode())
	{
	case TCCSM_NEARESTHOUR:
		dtDrag = CDateHelper::GetNearestHour(dtDrag, bEndOfDay);
		break;

	case TCCSM_NEARESTDAY:
		dtDrag = CDateHelper::GetNearestDay(dtDrag, bEndOfDay);
		break;

	case TCCSM_NEARESTHALFDAY:
		dtDrag = CDateHelper::GetNearestHalfDay(dtDrag, bEndOfDay);
		break;

	case TCCSM_FREE:
		if (m_bDraggingEnd)
		{
			COleDateTime dtEndOfDay(CDateHelper::GetEndOfDay(dtDrag));

			if (dtDrag > dtEndOfDay)
				dtDrag = dtEndOfDay;
		}
		break;

	case TCCSM_NONE:
	default:
		ASSERT(0);
		return FALSE;
	}
	
	return TRUE;
}

double CTaskCalendarCtrl::GetSnapIncrement() const
{
	switch (GetSnapMode())
	{
	case TCCSM_NEARESTDAY:		return 1.0;
	case TCCSM_NEARESTHALFDAY:	return TWELVE_HOURS;
	case TCCSM_NEARESTHOUR:		return ONE_HOUR;
	case TCCSM_FREE:			return ONE_MINUTE;
	}

	ASSERT(0);
	return 0.0;
}

TCC_SNAPMODE CTaskCalendarCtrl::GetSnapMode() const
{
	if (IsDragging())
	{
		TASKCALITEM* pTCI = GetTaskCalItem(m_dwSelectedTaskID);
		ASSERT(pTCI && !IsFutureOccurrence(pTCI));

		if (IsCustomDate(pTCI))
			return TCCSM_NEARESTDAY;
		
		// else active keys override
		if (Misc::ModKeysArePressed(MKS_CTRL))
			return TCCSM_NEARESTHOUR;

		if (Misc::ModKeysArePressed(MKS_SHIFT))
			return TCCSM_NEARESTDAY;

		if (Misc::ModKeysArePressed(MKS_CTRL | MKS_SHIFT))
			return TCCSM_NEARESTHALFDAY;
	}

	return m_nDefSnapMode; // default
}

void CTaskCalendarCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bReadOnly && UpdateDragging(point))
		return;

	CCalendarCtrlEx::OnMouseMove(nFlags, point);
}

BOOL CTaskCalendarCtrl::UpdateDragging(const CPoint& ptCursor)
{
	if (!IsDragging())
		return FALSE;

	TASKCALITEM* pTCI = GetTaskCalItem(m_dwSelectedTaskID);
	ASSERT(pTCI && !IsFutureOccurrence(pTCI));

	if (IsValidDrag(ptCursor))
	{
		COleDateTime dtDrag;

		if (GetValidDragDate(ptCursor, dtDrag))
		{
			// prevent the start and end dates from overlapping
			if (m_bDraggingStart)
			{
				// but only if the due date is set
				if (pTCI->IsEndDateSet())
					pTCI->SetStartDate(min(dtDrag.m_dt, pTCI->GetAnyEndDate().m_dt - ONE_HOUR));
				else
					pTCI->SetStartDate(dtDrag);
			}
			else if (m_bDraggingEnd)
			{
				// but only if the start date is set
				if (pTCI->IsStartDateSet())
					pTCI->SetDueDate(max(dtDrag.m_dt, pTCI->GetAnyStartDate().m_dt + ONE_HOUR));
				else
					pTCI->SetDueDate(dtDrag);
			}
			else // m_bDragging
			{
				if (pTCI->IsStartDateSet() && pTCI->IsEndDateSet())
				{
					COleDateTime dtStart = pTCI->GetAnyStartDate();
					COleDateTime dtEnd = pTCI->GetAnyEndDate();

					// If the end date is currently at 'end of day'
					// then bump it to the start of the next day so
					// that the calculated duration is accurate
					if (CDateHelper::GetEndOfDay(dtEnd) == dtEnd)
						dtEnd = CDateHelper::GetStartOfNextDay(dtEnd);

					double dDuration = (dtEnd.m_dt - dtStart.m_dt);

					pTCI->SetStartDate(dtDrag);

					// If end date would fall on the start of a new day
					// move it to the previous day
					dtEnd = (dtDrag.m_dt + dDuration);

					if (!CDateHelper::DateHasTime(dtEnd))
						dtEnd = CDateHelper::GetEndOfPreviousDay(dtEnd);

					pTCI->SetDueDate(dtEnd);
				}
				else if (pTCI->IsStartDateSet())
				{
					pTCI->SetStartDate(dtDrag);
				}
				else if (pTCI->IsEndDateSet())
				{
					pTCI->SetDueDate(dtDrag);
				}
			}
		}

		// Rebuild the cell tasks because the task may have moved cells
		// but without rebuilding extension tasks because we do that at the 
		// end of the drag only
		RebuildCellTasks(FALSE);

		// Select the cell under the mouse because we know it must contain the dragged task
		int nRow, nCol;

		if (HitTestGridCell(ptCursor, nRow, nCol))
			SelectGridCell(nRow, nCol);

		EnsureSelectionVisible();
	}
	else
	{
		*pTCI = m_tcidPreDrag;
	}

	// Recalc dates if either start/end is not set
	if (!pTCI->IsStartDateSet() || !pTCI->IsEndDateSet())
		pTCI->RecalcDates(m_dwOptions);

	Invalidate();
	UpdateWindow();

	// keep parent informed
	NotifyParentDragChange();

	return TRUE;
}

void CTaskCalendarCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_bReadOnly && EndDragging(point))
		return;
	
	CCalendarCtrlEx::OnLButtonUp(nFlags, point);
}

BOOL CTaskCalendarCtrl::EndDragging(const CPoint& ptCursor)
{
	if (!IsDragging())
		return FALSE;

	// dropping outside the calendar is a cancel
	CRect rLimits;
	GetAllowableDragLimits(rLimits);

	TCC_HITTEST nDragWhat = TCCHT_NOWHERE;
	CString sCustAttribID;

	TASKCALITEM* pTCI = GetTaskCalItem(m_dwSelectedTaskID);
	ASSERT(pTCI);

	BOOL bExtItem = IsExtensionItem(pTCI);

	if (!rLimits.PtInRect(ptCursor) || !IsValidDrag(ptCursor))
	{
		*pTCI = m_tcidPreDrag;
	}
	else if (bExtItem)
	{
		const TASKCALCUSTOMDATE* pTCICust = dynamic_cast<const TASKCALCUSTOMDATE*>(pTCI);
		ASSERT(pTCICust);

		if (pTCICust)
		{
			sCustAttribID = pTCICust->sCustomAttribID;
			nDragWhat = TCCHT_MIDDLE; // always

			// copy changes back to real task
			TASKCALITEM* pTCIReal = GetTaskCalItem(pTCICust->dwRealTaskID);
			ASSERT(pTCIReal);
 
			pTCIReal->SetCustomDate(pTCICust->sCustomAttribID, pTCICust->GetDate());
		}
	}
	else // real task
	{
		if (m_bDraggingStart)
		{
			nDragWhat = TCCHT_BEGIN;
		}
		else if (m_bDraggingEnd)
		{
			nDragWhat = TCCHT_END;
		}
		else
		{
			ASSERT(m_bDragging);

			// if the start is calculated, treat this like an end move
			if (!pTCI->IsStartDateSet())
			{
				nDragWhat = TCCHT_END;
			}
			else if (!pTCI->IsEndDateSet())
			{
				nDragWhat = TCCHT_BEGIN;
			}
			else
			{
				ASSERT(pTCI->IsStartDateSet() && pTCI->IsEndDateSet());
				nDragWhat = TCCHT_MIDDLE;
			}
		}
	}

	// cleanup
	m_bDraggingStart = m_bDraggingEnd = m_bDragging = FALSE;
	ReleaseCapture();

	TRACE(_T("\nDragEnd (date: %f.0, vpos: %d, scrollpos: %d)\n"), m_DateCurrent.m_dt, m_nVscrollPos, GetScrollPos(SB_VERT));

	// keep parent informed
	if (!NotifyParentDateChange(nDragWhat, sCustAttribID))
	{
		*pTCI = m_tcidPreDrag;
		RebuildCellTasks();
	}
	else if (m_mapRecurringTaskIDs.Has(m_dwSelectedTaskID) || bExtItem)
	{
		RebuildCellTasks();
	}

	Invalidate(FALSE);
	NotifyParentDragChange();

	return TRUE;
}

BOOL CTaskCalendarCtrl::GetSelectedTaskDates(COleDateTime& dtStart, COleDateTime& dtDue) const
{
	if (GetSelectedTaskID() == 0)
		return FALSE;

	DWORD dwRealTaskID = GetRealTaskID(m_dwSelectedTaskID);

	TASKCALITEM* pTCI = GetTaskCalItem(dwRealTaskID);
	ASSERT(pTCI);

	if (!pTCI)
		return FALSE;

	dtStart = pTCI->GetAnyStartDate();
	dtDue = pTCI->GetAnyEndDate();

	// handle 'End of Day'
	COleDateTime dtEndOfDueDay(CDateHelper::GetEndOfDay(dtDue));

	if (dtDue > dtEndOfDueDay)
		dtDue = dtEndOfDueDay;

	return TRUE;
}

BOOL CTaskCalendarCtrl::GetSelectedTaskCustomDate(const CString& sCustAttribID, COleDateTime& date) const
{
	if (GetSelectedTaskID() == 0)
		return FALSE;

	TASKCALITEM* pTCI = GetTaskCalItem(GetRealTaskID(m_dwSelectedTaskID));
	ASSERT(pTCI);

	if (!pTCI)
		return FALSE;

	date = pTCI->GetCustomDate(sCustAttribID);
	return CDateHelper::IsDateSet(date);
}

BOOL CTaskCalendarCtrl::GetDateFromPoint(const CPoint& ptCursor, COleDateTime& date) const
{
	int nRow, nCol;

	if (HitTestGridCell(ptCursor, nRow, nCol))
	{
		const CCalendarCell* pCell = GetCell(nRow, nCol);
		ASSERT(pCell);

		// calc proportion of day by 'x' coordinate
		CRect rCell;
		VERIFY(GetGridCellRect(nRow, nCol, rCell));

		double dTime = ((ptCursor.x - rCell.left) / (double)rCell.Width());
		date = pCell->date.m_dt + dTime;

		return TRUE;
	}

	// all else
	return FALSE;
}

BOOL CTaskCalendarCtrl::CanDragTask(DWORD dwTaskID, TCC_HITTEST nHit) const
{
	ASSERT((nHit == TCCHT_NOWHERE) || (dwTaskID != 0));

	if (nHit == TCCHT_NOWHERE)
		return FALSE;
	
	const TASKCALITEM* pTCI = GetTaskCalItem(dwTaskID);

	// Sanity check
	if (!pTCI)
	{
		ASSERT(0);
		return FALSE;
	}

	if (pTCI->IsDone(FALSE))
		return FALSE;

	if (pTCI->bLocked)
		return FALSE;

	if (IsFutureOccurrence(pTCI))
		return FALSE;

	BOOL bCustomDate = IsCustomDate(pTCI);
	BOOL bHasDepends = (!bCustomDate && HasOption(TCCO_PREVENTDEPENDENTDRAGGING) && pTCI->bHasDepends);
			
	switch (nHit)
	{
	case TCCHT_BEGIN:
		return !bCustomDate && !bHasDepends;

	case TCCHT_MIDDLE:
		return !bHasDepends;

	case TCCHT_END:
		return !bCustomDate;
	}

	// all else
	return FALSE;
}

BOOL CTaskCalendarCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// this is only for when we are NOT yet dragging
	ASSERT(!IsDragging());

	if (!m_bReadOnly &&
		!m_bSelectionStarted && 
		(nHitTest == HTCLIENT) && 
		(message == WM_MOUSEMOVE) &&
		!Misc::IsKeyPressed(VK_MBUTTON) &&
		!Misc::IsKeyPressed(VK_RBUTTON))
	{
		CPoint ptCursor = CWnd::GetCurrentMessage()->pt;
		ScreenToClient(&ptCursor);

		TCC_HITTEST nHit = TCCHT_NOWHERE;
		DWORD dwHitID = HitTestTask(ptCursor, nHit);

		if (SetTaskCursor(dwHitID, nHit))
			return TRUE;
	}
	
	// else
	return CCalendarCtrlEx::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CTaskCalendarCtrl::SetTaskCursor(DWORD dwTaskID, TCC_HITTEST nHit) const
{
	if ((dwTaskID != 0) && (nHit != TCCHT_NOWHERE))
	{
		if (!CanDragTask(dwTaskID, nHit))
		{
			const TASKCALITEM* pTCI = GetTaskCalItem(dwTaskID); 
			ASSERT(pTCI);

			if (pTCI && pTCI->bLocked)
				return GraphicsMisc::SetAppCursor(_T("Locked"), _T("Resources\\Cursors"));

			// else
			return GraphicsMisc::SetAppCursor(_T("NoDrag"), _T("Resources\\Cursors"));
		}
		else
		{
			switch (nHit)
			{
			case TCCHT_BEGIN:
			case TCCHT_END:
				return GraphicsMisc::SetStandardCursor(IDC_SIZEWE);
			}
		}
	}

	return FALSE;
}

BOOL CTaskCalendarCtrl::IsDragging() const
{
	return (m_bDragging || m_bDraggingStart || m_bDraggingEnd);
}

BOOL CTaskCalendarCtrl::IsValidDrag(const CPoint& ptDrag) const
{
	if (!IsDragging())
		return FALSE;

	CSize size = (m_ptDragOrigin - ptDrag);

	int nCxDrag = (GetSystemMetrics(SM_CXDRAG) / 2);
	int nCyDrag = (GetSystemMetrics(SM_CYDRAG) / 2);

	return ((abs(size.cx) > nCxDrag) || 
			(abs(size.cy) > nCyDrag));
}

void CTaskCalendarCtrl::GetAllowableDragLimits(CRect& rLimits) const
{
	GetClientRect(rLimits);
	rLimits.top += m_nHeaderHeight;

	// Allow a border all the way round
	rLimits.InflateRect(50, 50);
}

BOOL CTaskCalendarCtrl::ValidateDragPoint(CPoint& ptDrag) const
{
	if (!IsDragging())
		return FALSE;

	CRect rLimits;
	GetAllowableDragLimits(rLimits);

	if (!rLimits.PtInRect(ptDrag))
		return FALSE;

	// Validate against client rect
	CRect rClient;
	GetClientRect(rClient);

	ptDrag.x = max(ptDrag.x, rClient.left);
	ptDrag.x = min(ptDrag.x, rClient.right);
	ptDrag.y = max(ptDrag.y, rClient.top);
	ptDrag.y = min(ptDrag.y, rClient.bottom);

	return TRUE;
}

void CTaskCalendarCtrl::OnCaptureChanged(CWnd *pWnd) 
{
	// if something grabs the capture we cancel any drag
	if (IsDragging() && (pWnd != this))
		CancelDrag(FALSE);
	
	CCalendarCtrlEx::OnCaptureChanged(pWnd);
}

void CTaskCalendarCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
	case VK_ESCAPE:
		if (IsDragging())
		{
			CancelDrag(TRUE);
			return;
		}
		break;

	case VK_CONTROL:
	case VK_SHIFT:
		if (IsDragging())
			NotifyParentDragChange();
		break;

	case VK_DOWN:
		if (IsCellScrollBarActive())
		{
			SendMessage(WM_VSCROLL, SB_LINEDOWN, (LPARAM)m_sbCellVScroll.GetSafeHwnd());
			return;
		}
		break;

	case VK_UP:
		if (IsCellScrollBarActive())
		{
			SendMessage(WM_VSCROLL, SB_LINEUP, (LPARAM)m_sbCellVScroll.GetSafeHwnd());
			return;
		}
		break;
	}
	
	CCalendarCtrlEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTaskCalendarCtrl::OnContextMenu(CWnd* pWnd, CPoint pos)
{
	if (GetSelectedTaskID() == 0)
		return ;

	TASKCALITEM* pTCI = GetTaskCalItem(m_dwSelectedTaskID);
	ASSERT(pTCI);

	if (pTCI && IsCustomDate(pTCI))
	{
		CMenu menu;

		if (menu.LoadMenu(IDR_CUSTOMDATE_POPUP))
		{
			CMenu* pPopup = menu.GetSubMenu(0);

			if (pPopup)
			{
				// check pos
				if (pos.x == -1 && pos.y == -1)
				{
					pos = GetCaretPos();
					::ClientToScreen(*this, &pos);
				}

				UINT nCmdID = pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, pos.x, pos.y, this);

				switch (nCmdID)
				{
				case ID_CAL_CLEARCUSTOMDATE:
					ClearSelectedCustomDate();
					break;
				}
			}
		}

		return;
	}

	CCalendarCtrlEx::OnContextMenu(pWnd, pos);
}

BOOL CTaskCalendarCtrl::IsCellScrollBarActive() const
{
	return (m_sbCellVScroll.GetSafeHwnd() && 
			m_sbCellVScroll.IsWindowVisible() && 
			CDialogHelper::IsChildOrSame(this, GetFocus()));
}

// external version
BOOL CTaskCalendarCtrl::CancelDrag()
{
	if (IsDragging())
	{
		CancelDrag(TRUE);
		return TRUE;
	}

	// else
	return FALSE;
}

// internal version
void CTaskCalendarCtrl::CancelDrag(BOOL bReleaseCapture)
{
	ASSERT(IsDragging());

	// cancel drag, restoring original task dates
	TASKCALITEM* pTCI = GetTaskCalItem(m_dwSelectedTaskID); 
	ASSERT(pTCI);

	*pTCI = m_tcidPreDrag;
	m_bDragging = m_bDraggingStart = m_bDraggingEnd = FALSE;

	if (bReleaseCapture)
		ReleaseCapture();

	RebuildCellTasks(IsExtensionItem(m_dwSelectedTaskID));
	Invalidate(FALSE);
	UpdateWindow();

	// keep parent informed
	NotifyParentDragChange();
}

void CTaskCalendarCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	BOOL bUnused;
	DWORD dwTaskID = HitTestTask(point, bUnused);

	SelectTask(dwTaskID, FALSE, TRUE);
	
	CCalendarCtrlEx::OnRButtonDown(nFlags, point);
}

int CTaskCalendarCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CCalendarCtrlEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_fonts.Initialise(*this);
	m_timerMidnight.Enable(*this);

	EnableLabelTips(HasOption(TCCO_ENABLELABELTIPS));
	
	return 0;
}

BOOL CTaskCalendarCtrl::EnableLabelTips(BOOL bEnable)
{
	if (bEnable)
	{
		if (!m_tooltip.GetSafeHwnd())
		{
			if (!m_tooltip.Create(this))
				return FALSE;

			m_tooltip.ModifyStyleEx(0, WS_EX_TRANSPARENT);
			m_tooltip.SetFont(&m_DefaultFont);
			m_tooltip.SetDelayTime(TTDT_INITIAL, 50);
			m_tooltip.SetDelayTime(TTDT_AUTOPOP, 10000);
		}
	}
	else if (m_tooltip.GetSafeHwnd()) 
	{
		m_tooltip.DestroyWindow();
	}

	return TRUE;
}

BOOL CTaskCalendarCtrl::ProcessMessage(MSG* pMsg) 
{
	// Handle 'Delete' on a custom date
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		if (pMsg->wParam == VK_DELETE)
			return ClearSelectedCustomDate();
		break;
	}
	
	// All else
	return false;
}

BOOL CTaskCalendarCtrl::ClearSelectedCustomDate()
{
	TASKCALITEM* pTCI = GetTaskCalItem(m_dwSelectedTaskID);

	if (IsCustomDate(pTCI))
	{
		const TASKCALCUSTOMDATE* pTCIDate = dynamic_cast<TASKCALCUSTOMDATE*>(pTCI);
		ASSERT(pTCIDate);

		pTCI = GetTaskCalItem(GetRealTaskID(m_dwSelectedTaskID));
		ASSERT(pTCI);

		pTCI->ClearCustomDate(pTCIDate->sCustomAttribID);
		NotifyParentDateChange(TCCHT_MIDDLE, pTCIDate->sCustomAttribID);

		// Move the selection to the real task
		m_dwSelectedTaskID = 0;
		SelectTask(pTCI->GetTaskID(), TRUE, TRUE);

		RebuildCellTasks();
		Invalidate(TRUE);

		return TRUE;
	}

	// else
	return FALSE;
}

void CTaskCalendarCtrl::FilterToolTipMessage(MSG* pMsg) 
{
	if (m_tooltip.GetSafeHwnd())
		m_tooltip.FilterToolTipMessage(pMsg);
}

int CTaskCalendarCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	CRect rOverflowBtn;

	if (HitTestCellOverflowBtn(point, rOverflowBtn))
	{
		return CToolTipCtrlEx::SetToolInfo(*pTI, this, CEnString(IDS_OVERFLOWBTN_TIP), OVERFLOWBTN_TIPID, rOverflowBtn);
	}

	TCC_HITTEST nHit;
	CRect rHit;
	DWORD dwTaskID = HitTestTask(point, nHit, rHit);

	// Don't show tooltip when hovering over 'ends'
	if (dwTaskID && (nHit == TCCHT_MIDDLE))
	{
		const TASKCALITEM* pTCI = GetTaskCalItem(dwTaskID); 
		ASSERT(pTCI);

		CEnString sTooltip;

		if (IsExtensionItem(pTCI))
		{
			// NOTE: - Must match 'Week Planner' View in 'Plugins' project
			if (IsFutureOccurrence(pTCI))
			{
				sTooltip.LoadString(IDS_FUTUREOCCURRENCE_TOOLTIP);
			}
			else if (IsCustomDate(pTCI))
			{
				const TASKCALCUSTOMDATE* pTCIDate = dynamic_cast<const TASKCALCUSTOMDATE*>(pTCI);
				ASSERT(pTCIDate);

				CString sAttribLabel;
				VERIFY(m_mapCustomDateAttrib.Lookup(pTCIDate->sCustomAttribID, sAttribLabel));

				sTooltip.Format(IDS_CUSTOMDATE_TOOLTIP, sAttribLabel);
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			BOOL bWantTooltip = (GetTaskHeight() < MIN_TASK_HEIGHT);

			if (!bWantTooltip)
			{
				if (HasOption(TCCO_DISPLAYCONTINUOUS))
				{
					int nTextOffset = GetTaskContinuousDrawInfo(dwTaskID).nTextOffset;
					bWantTooltip = (nTextOffset > 0);
				}
				else
				{
					CFontCache& fonts = const_cast<CFontCache&>(m_fonts);

					int nTextLen = GraphicsMisc::GetTextWidth(pTCI->GetName(), m_tooltip, fonts.GetHFont(pTCI->bTopLevel ? GMFS_BOLD : 0));
					bWantTooltip = (nTextLen > rHit.Width());
				}
			}

			if (bWantTooltip)
				sTooltip = pTCI->GetName();
		}

		if (!sTooltip.IsEmpty())
			return CToolTipCtrlEx::SetToolInfo(*pTI, this, sTooltip, dwTaskID, rHit);
	}

	// else
	return CCalendarCtrlEx::OnToolHitTest(point, pTI);
}

void CTaskCalendarCtrl::OnShowTooltip(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	// Only handle our tooltips
	if (pNMHDR->hwndFrom != m_tooltip)
		return;

	if (IsDragging())
		return;

	DWORD dwTaskID = m_tooltip.GetLastHitToolInfo().uId;

	if (dwTaskID == 0)
	{
		ASSERT(0);
		return;
	}
	else if (dwTaskID == OVERFLOWBTN_TIPID)
	{
		m_tooltip.SetFont(m_fonts.GetFont());
		return;
	}
	
	// Calculate label rect for real tasks only
	if (!IsExtensionItem(dwTaskID))
	{
		// Set the font first, bold for top level items
		const TASKCALITEM* pTCI = GetTaskCalItem(dwTaskID);

		if (!pTCI)
		{
			ASSERT(0);
			return;
		}

		m_tooltip.SetFont(m_fonts.GetFont(pTCI->bTopLevel ? GMFS_BOLD : 0));

		CRect rLabel;

		if (HasOption(TCCO_DISPLAYCONTINUOUS))
		{
			VERIFY(GetTaskLabelRect(dwTaskID, rLabel));
		}
		else
		{
			CPoint ptTip(::GetMessagePos());
			ScreenToClient(&ptTip);

			TCC_HITTEST nUnused;
			VERIFY(HitTestTask(ptTip, nUnused, rLabel));
		}

		ClientToScreen(rLabel);

		CRect rTip(rLabel);
		m_tooltip.AdjustRect(rTip, TRUE);
		rTip.OffsetRect(TIP_PADDING, 0);

		m_tooltip.SetWindowPos(NULL, rTip.left, rLabel.top, 0, 0, (SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE));

		*pResult = TRUE; // we do the positioning
	}
	else
	{
		// Never embolden extension item tooltips
		m_tooltip.SetFont(m_fonts.GetFont());
	}
}

BOOL CTaskCalendarCtrl::SaveToImage(CBitmap& bmImage)
{
	if (!CanSaveToImage())
		return FALSE;

	CAutoFlag af(m_bSavingToImage, TRUE);
	CLockUpdates lock(GetSafeHwnd());
	CClientDC dc(this);

	// Resize the client rect to fully 'unroll' any cell scrollbars
	CFont* pOldFont = dc.SelectObject(m_fonts.GetFont());

	CRect rCell, rWindowOrg(0, 0, 0, 0);
	GetGridCellRect(0, 0, rCell);

	int nReqCellHeight = (m_nHeaderHeight + (m_nMaxDayTaskCount * m_nTaskHeight));
	int nReqCellWidth = (CDateHelper::GetMaxDayOfWeekNameWidth(&dc) + GraphicsMisc::ScaleByDPIFactor(10 + 10));

	dc.SelectObject(pOldFont);
	
	if ((rCell.Height() < nReqCellHeight) || (rCell.Width() < nReqCellWidth))
	{
		CRect rWindow;
		GetWindowRect(rWindow);

		GetParent()->ScreenToClient(rWindow);
		rWindowOrg = rWindow;

		if (rCell.Height() < nReqCellHeight)
		{
			rWindow.bottom = rWindow.top;
			rWindow.bottom += m_nDayHeaderHeight;
			rWindow.bottom += (m_nVisibleWeeks * nReqCellHeight);
		}

		if (rCell.Width() < nReqCellWidth)
		{
			rWindow.left = rWindow.right;
			rWindow.right += (7 * nReqCellWidth);
		}

		MoveWindow(rWindow);
	}
	
	CDC dcImage;

	if (dcImage.CreateCompatibleDC(&dc))
	{
		CRect rClient;
		GetClientRect(rClient);

		if (bmImage.CreateCompatibleBitmap(&dc, rClient.Width(), rClient.Height()))
		{
			CBitmap* pOldImage = dcImage.SelectObject(&bmImage);
			dcImage.FillSolidRect(rClient, GetSysColor(COLOR_WINDOW));

			DrawHeader(&dcImage);
			DrawCells(&dcImage);
			DrawGrid(&dcImage);

			dcImage.SelectObject(pOldImage);
		}
	}

	if (!rWindowOrg.IsRectNull())
		MoveWindow(rWindowOrg);

	return (bmImage.GetSafeHandle() != NULL);
}

BOOL CTaskCalendarCtrl::CanSaveToImage() const
{
	return ((m_mapData.GetCount() > 0) && ((m_dtMax.m_dt - m_dtMin.m_dt) > 0));
}

void CTaskCalendarCtrl::RecalcDataRange()
{
	CDateHelper::ClearDate(m_dtMin);
	CDateHelper::ClearDate(m_dtMax);

	POSITION pos = m_mapData.GetStartPosition();

	while (pos)
	{
		DWORD dwTaskID = 0;
		TASKCALITEM* pTCI = NULL;

		m_mapData.GetNextAssoc(pos, dwTaskID, pTCI);
		pTCI->MinMax(m_dtMin, m_dtMax);
	}
}

LRESULT CTaskCalendarCtrl::OnMidnight(WPARAM /*wp*/, LPARAM /*lp*/)
{
	ASSERT(HasOption(TCCO_CALCMISSINGSTARTASEARLIESTDUEANDTODAY) ||
			HasOption(TCCO_CALCMISSINGDUEASLATESTSTARTANDTODAY));

	RecalcTaskDates();
	RebuildCellTasks();

	Invalidate(FALSE);

	return 0L;
}

