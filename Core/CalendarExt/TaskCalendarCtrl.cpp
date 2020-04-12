// TaskCalendarCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TaskCalendarCtrl.h"
#include "CalMsg.h"

#include "..\Shared\GraphicsMisc.h"
#include "..\Shared\themed.h"
#include "..\Shared\DateHelper.h"
#include "..\Shared\TimeHelper.h"
#include "..\Shared\misc.h"
#include "..\Shared\dialoghelper.h"
#include "..\Shared\holdredraw.h"
#include "..\Shared\autoflag.h"
#include "..\Shared\enimagelist.h"
#include "..\Shared\WorkingWeek.h"
#include "..\Shared\ScopedTimer.h"

#include "..\Interfaces\UITheme.h"

#include "..\3rdParty\colordef.h"

#include <math.h>

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const int TIP_PADDING = 3;
const int TEXT_PADDING = 2;
const int IMAGE_SIZE = GraphicsMisc::ScaleByDPIFactor(16);
const int DEF_TASK_HEIGHT = (IMAGE_SIZE + 3); // Effective height is 1 less
const int MIN_TASK_HEIGHT = (DEF_TASK_HEIGHT - 6);

/////////////////////////////////////////////////////////////////////////////

CTaskCalendarCtrl::CONTINUOUSDRAWINFO::CONTINUOUSDRAWINFO(DWORD dwID) : dwTaskID(dwID) 
{ 
	Reset();
}

void CTaskCalendarCtrl::CONTINUOUSDRAWINFO::Reset() 
{ 
	nIconOffset = 0;
	nTextOffset = 0;
	nVertPos = -1; 
}

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
	m_nSnapMode(TCCSM_NEARESTHOUR),
	m_dwOptions(TCCO_DISPLAYCONTINUOUS | TCCO_ENABLELABELTIPS),
	m_bReadOnly(FALSE),
	m_nCellVScrollPos(0),
	m_bStrikeThruDone(FALSE),
	m_bSavingToImage(FALSE),
	m_nTaskHeight(DEF_TASK_HEIGHT),
	m_nSortBy(TDCA_NONE),
	m_bSortAscending(-1),
	m_crWeekend(RGB(224, 224, 224)),
	m_crToday(255) 
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
	ON_NOTIFY(TTN_SHOW, 0, OnShowTooltip)
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_MESSAGE(WM_SETFONT, OnSetFont)

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

void CTaskCalendarCtrl::SetOptions(DWORD dwOptions)
{
	if (m_dwOptions != dwOptions)
	{
		DWORD dwPrev = m_dwOptions;
		m_dwOptions = dwOptions;

		RecalcTaskDates();
		RebuildCellTasks();

		BOOL bScrollToTask = !HasSameDateDisplayOptions(m_dwOptions, dwPrev);
		FixupSelection(bScrollToTask);

		EnableLabelTips(HasOption(TCCO_ENABLELABELTIPS));
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

	BOOL bChange = FALSE;

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
		bChange = (UpdateTask(pTasks, pTasks->GetFirstTask(), nUpdate, TRUE) ||
					pTasks->IsAttributeAvailable(TDCA_RECURRENCE));
		break;
		
	case IUI_DELETE:
		bChange = RemoveDeletedTasks(pTasks);
		break;
		
	default:
		ASSERT(0);
	}
	
	if (bChange)
	{
		RecalcDataRange();
		RebuildCellTasks();

		Invalidate(FALSE);
	}

	return bChange;
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

			// clear selection if necessary
			if (m_dwSelectedTaskID == dwTaskID)
				m_dwSelectedTaskID = 0;

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

			bChange = pTCI->UpdateTask(pTasks, hTask, m_dwOptions);

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

BOOL CTaskCalendarCtrl::NotifyParentDateChange(TCC_HITTEST nHit)
{
	ASSERT(!m_bReadOnly);
	ASSERT(m_dwSelectedTaskID);

	if (nHit != TCCHT_NOWHERE)
		return GetParent()->SendMessage(WM_CALENDAR_DATECHANGE, (WPARAM)nHit, m_dwSelectedTaskID);

	// else
	return FALSE;
}

void CTaskCalendarCtrl::NotifyParentDragChange()
{
	ASSERT(m_dwSelectedTaskID);

	GetParent()->SendMessage(WM_CALENDAR_DRAGCHANGE, (WPARAM)GetSnapMode(), m_dwSelectedTaskID);
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
		TASKCALITEM* pTCI = new TASKCALITEM(pTasks, hTask, m_dwOptions);
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
		rText.DeflateRect(0, (m_nHeaderHeight - sizeDOW.cy) / 2, 0, 0);
		
		pDC->DrawText(csTitle, rText, DT_CENTER|DT_TOP);
		
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

void CTaskCalendarCtrl::SetUITheme(const UITHEME& theme)
{
	m_crWeekend = theme.crWeekend;
	m_crToday = theme.crToday;

	if (GetSafeHwnd())
		Invalidate();
}

void CTaskCalendarCtrl::DrawCell(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell, BOOL bSelected, BOOL bToday, BOOL bShowMonth)
{
	CCalendarCtrlEx::DrawCell(pDC, pCell, rCell, bSelected, bToday, bShowMonth);

	if (bSelected && (m_crTheme != CLR_NONE))
	{
		CRect rSel(rCell);

		if (rSel.left)
			rSel.left++;

		GraphicsMisc::DrawRect(pDC, rSel, CLR_NONE, m_crTheme);

		rSel.DeflateRect(1, 1);
		GraphicsMisc::DrawRect(pDC, rSel, CLR_NONE, m_crTheme);
	}
}

void CTaskCalendarCtrl::DrawCellBkgnd(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell, BOOL /*bSelected*/, BOOL bToday)
{
	if (HasColor(m_crWeekend) && CWeekend().IsWeekend(pCell->date))
	{
		// If dtDay is also 'today' and we have a today colour
		// then render using a grey so that that it does overly
		// affect the today colour which will be overlaid on top
		COLORREF crWeekend = m_crWeekend;

		if (bToday && HasColor(m_crToday))
			crWeekend = RGBX(crWeekend).Gray();

		GraphicsMisc::DrawRect(pDC, rCell, crWeekend, CLR_NONE, 0, GMDR_NONE, 128);
	}

	if (bToday && HasColor(m_crToday))
	{
		CRect rToday(rCell);

		// Don't overdraw gridline
		if (rToday.left > 0)
			rToday.left++;

		GraphicsMisc::DrawRect(pDC, rToday, m_crToday, CLR_NONE, 0, GMDR_NONE, 128);
	}
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

	// Exclude selection border from drawing
	CRect rAvailCell(rCell);

	if (bSelected)
		rAvailCell.DeflateRect(2, 0); // not top/bottom
	
	// Exclude scrollbar rect from drawing
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

		DWORD dwRealTaskID = GetRealTaskID(dwTaskID);
		ASSERT(dwRealTaskID);
		
		// draw selection
		BOOL bSelTask = (!m_bSavingToImage && (dwRealTaskID == m_dwSelectedTaskID));

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
			else if (dwTaskID != dwRealTaskID)
			{
				nState = GMIS_DROPHILITED;
			}

			crText = GraphicsMisc::GetExplorerItemSelectionTextColor(crText, nState, GMIB_THEMECLASSIC);

			GraphicsMisc::DrawExplorerItemSelection(pDC, *this, nState, rTask, dwSelFlags, rClip);
		}
		else // draw task border/background
		{
			DWORD dwFlags = GMDR_TOP;
			
			if (rTask.left > rAvailCell.left)
			{
				dwFlags |= GMDR_LEFT;
			}
			else if (bContinuous)
			{
				rTask.left--; // draw over gridline
			}
			
			if (rTask.right < rAvailCell.right)
				dwFlags |= GMDR_RIGHT;
			
			if (rTask.bottom < rAvailCell.bottom)
				dwFlags |= GMDR_BOTTOM;
			
			COLORREF crFill = pTCI->GetFillColor(bTextColorIsBkgnd);
			COLORREF crBorder = pTCI->GetBorderColor(bTextColorIsBkgnd);
						
			GraphicsMisc::DrawRect(pDC, rTask, crFill, crBorder, 0, dwFlags);
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

BOOL CTaskCalendarCtrl::UpdateCellScrollBarVisibility(BOOL bEnsureSelVisible)
{
	// First determine if we need to show a vertical scrollbar
	int nRow = -1, nCol = -1;
	BOOL bShowScrollbar = (CDialogHelper::IsChildOrSame(this, GetFocus()) &&
							GetLastSelectedGridCell(nRow, nCol));

	CRect rCell;
	const CCalendarCell* pCell = NULL;
	int nNumItems = 0;

	if (bShowScrollbar)
	{
		bShowScrollbar = GetGridCellRect(nRow, nCol, rCell, TRUE);

		if (bShowScrollbar)
		{
			pCell = GetCell(nRow, nCol);

			const CTaskCalItemArray* pTasks = GetCellTasks(pCell);

			if (pTasks && pTasks->GetSize())
			{
				// Get the max task pos range
				int nLastTask = (pTasks->GetSize() - 1);
				const TASKCALITEM* pTCILast = pTasks->GetAt(nLastTask);

				int nMaxPos = GetTaskVertPos(pTCILast->GetTaskID(), nLastTask, pCell, FALSE);

				nNumItems = (nMaxPos + 1);
				bShowScrollbar = ((nNumItems * GetTaskHeight()) > rCell.Height());

				// And scroll selected task into view
				if (bEnsureSelVisible && m_dwSelectedTaskID)
				{
					int nFind = pTasks->FindItem(m_dwSelectedTaskID);

					if (nFind != -1)
						m_nCellVScrollPos = GetTaskVertPos(m_dwSelectedTaskID, nFind, pCell, FALSE);
				}
			}
			else
			{
				bShowScrollbar = FALSE;
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
			int nRow, nCol;

			if (HitTestGridCell(rPrevPos.CenterPoint(), nRow, nCol))
			{
				pOldCell = GetCell(nRow, nCol);
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
			si.nPage = min((rCell.Height() / GetTaskHeight()), nNumItems);

			m_nCellVScrollPos = min(m_nCellVScrollPos, (nNumItems - (int)si.nPage));
			si.nPos = m_nCellVScrollPos;

			m_sbCellVScroll.EnableWindow(TRUE);
			m_sbCellVScroll.SetScrollInfo(&si, m_sbCellVScroll.IsWindowVisible());
			m_sbCellVScroll.ShowWindow(SW_SHOW);
		}
	}

	return bSuccess;
}

void CTaskCalendarCtrl::CalcScrollBarRect(const CRect& rCell, CRect& rScrollbar) const
{
	rScrollbar = rCell;

	if (m_crTheme != CLR_NONE)
		rScrollbar.DeflateRect(0, 0, 2, 2);

	rScrollbar.left = (rScrollbar.right - GetSystemMetrics(SM_CXVSCROLL));
}

#if _MSC_VER >= 1400
BOOL CTaskCalendarCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
#else
void CTaskCalendarCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
#endif
{
	// if the mouse is over the cell with the scrollbar
	// then pass to the scrollbar instead
	int nRow, nCol;
	
	// pt is in screen coords
	ScreenToClient(&pt);
	
	if (HitTestGridCell(pt, nRow, nCol) &&
		IsGridCellSelected(nRow, nCol) &&
		IsCellScrollBarActive() &&
		!Misc::IsKeyPressed(VK_CONTROL))
	{
		int nLine = (abs(zDelta) / 120);
		UINT nDir = ((zDelta < 0) ? SB_LINEDOWN : SB_LINEUP);

		while (nLine--)
			SendMessage(WM_VSCROLL, nDir, (LPARAM)m_sbCellVScroll.GetSafeHwnd());

#if _MSC_VER >= 1400
		return TRUE; // eat
#else
		return; // eat
#endif
	}

#if _MSC_VER >= 1400
		return CCalendarCtrlEx::OnMouseWheel(nFlags, zDelta, pt);
#else
		CCalendarCtrlEx::OnMouseWheel(nFlags, zDelta, pt);
#endif
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
		Invalidate(FALSE);

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

int CTaskCalendarCtrl::RebuildCellTasks(BOOL bIncFutureItems)
{
	CScopedTraceTimer timing(_T("CTaskCalendarCtrl::RebuildCellTasks()"));

	// Rebuild future occurrences first because these are
	// needed for building the cell tasks
	if (bIncFutureItems)
		RebuildFutureOccurrences();
	
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

void CTaskCalendarCtrl::RebuildFutureOccurrences()
{
	m_mapFutureOccurrences.RemoveAll();

	if (m_mapRecurringTaskIDs.GetCount())
	{
		// Go through all the known recurring tasks to see if any 
		// of them have future occurrences within the visible cells
		TASKCALFUTUREDATES dtFuture;
		VERIFY(dtFuture.dtRange.Set(GetMinDate(), GetMaxDate()));

		POSITION pos = m_mapRecurringTaskIDs.GetStartPosition();
		DWORD dwNextFutureTaskID = (((m_dwMaximumTaskID / 1000) + 1) * 1000);

		while (pos)
		{
			DWORD dwTaskID = m_mapRecurringTaskIDs.GetNext(pos);
			ASSERT(dwTaskID);

			TASKCALITEM* pTCIReal = GetTaskCalItem(dwTaskID, FALSE); // exclude future items

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
				TASKCALITEM* pTCIFuture = new TASKCALFUTUREITEM(*pTCIReal, dwNextFutureTaskID, dtFuture.dtOccurrences[nItem]);
				m_mapFutureOccurrences[dwNextFutureTaskID] = pTCIFuture;

				dwNextFutureTaskID++;
			}
		}
	}
}

void CTaskCalendarCtrl::RebuildCellTaskDrawInfo()
{
	m_nMaxDayTaskCount = 0;
	m_aContinuousDrawInfo.RemoveAll();

	if (m_mapData.GetCount() == 0)
		return;

	for (int i = 0; i < CALENDAR_MAX_ROWS; i++)
	{
		for (int u = 0; u < CALENDAR_NUM_COLUMNS; u++)
		{
			CTaskCalItemArray* pTasks = GetCellTasks(i, u);
			ASSERT(pTasks);

			if (pTasks)
			{
				// now go thru the list and set the position of each item 
				// if not already done
				if (HasOption(TCCO_DISPLAYCONTINUOUS))
				{
					int nMaxPos = 0;

					for (int nTask = 0; nTask < pTasks->GetSize(); nTask++)
					{
						const TASKCALITEM* pTCI = pTasks->GetAt(nTask);
						ASSERT(pTCI);

						DWORD dwTaskID = pTCI->GetTaskID();
						CONTINUOUSDRAWINFO& cdi = GetTaskContinuousDrawInfo(dwTaskID);

						if (cdi.nVertPos == -1)
							cdi.nVertPos = max(nMaxPos, nTask);

						nMaxPos = max(nMaxPos, (cdi.nVertPos + 1));
					}

					m_nMaxDayTaskCount = max(m_nMaxDayTaskCount, nMaxPos);
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

	// Future task occurrences
	AddTasksToCell(m_mapFutureOccurrences, pCell->date, pTasks);

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

		// ignore tasks with both start and end dates calculated
		if (!pTCI->IsValid())
			continue;

		if (pTCI->IsParent() && (HasOption(TCCO_HIDEPARENTTASKS)))
			continue;

		// ignore completed tasks as required
		if (pTCI->IsDone(TRUE) && !HasOption(TCCO_DISPLAYDONE))
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

DWORD CTaskCalendarCtrl::HitTest(const CPoint& ptClient) const
{
	TCC_HITTEST nHit = TCCHT_NOWHERE;
	return HitTest(ptClient, nHit);
}

DWORD CTaskCalendarCtrl::HitTest(const CPoint& ptClient, TCC_HITTEST& nHit) const
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

	CTaskCalItemArray* pTasks = static_cast<CTaskCalItemArray*>(pCell->pUserData);
	ASSERT(pTasks);
	
	if (!pTasks || !pTasks->GetSize())
		return 0;
	
	// determine the vertical 'task pos' of the cursor
	CRect rCell;
	GetGridCellRect(nRow, nCol, rCell, TRUE);
	
	// handle clicking above tasks
	if (ptClient.y < rCell.top)
		return 0;
	
	int nTaskHeight = GetTaskHeight();
	int nPos = ((ptClient.y - rCell.top) / nTaskHeight);
	
	// look thru the tasks for this pos
	for (int nTask = 0; nTask < pTasks->GetSize(); nTask++)
	{
		const TASKCALITEM* pTCI = (*pTasks)[nTask];
		ASSERT(pTCI);

		DWORD dwTaskID = pTCI->GetTaskID();
		ASSERT(dwTaskID);

		int nTaskPos = GetTaskVertPos(dwTaskID, nTask, pCell, IsCellScrollBarActive());
		
		if (nTaskPos == nPos)
		{
			// now we figure out where on the item we hit
			COleDateTime dtHit;
			VERIFY(GetDateFromPoint(ptClient, dtHit));
			
			// now check for closeness to ends
			double dDateTol = CalcDateDragTolerance();
			
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
			
			return ((nHit == TCCHT_NOWHERE) ? 0 : dwTaskID);
		}
	}

	// nothing hit
	return 0;
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
	{
		ASSERT(0);
		return FALSE;
	}

	// Does the selected cell already have this task?
	int nRow, nCol;

	if (GetLastSelectedGridCell(nRow, nCol))
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

bool CTaskCalendarCtrl::SelectGridCell(int nRow, int nCol)
{
	int nPrevRow, nPrevCol;

	if (!GetLastSelectedGridCell(nPrevRow, nPrevCol) ||
		(nRow != nPrevRow) || (nCol != nPrevCol))
	{
		if (!CCalendarCtrl::SelectGridCell(nRow, nCol))
			return false;

		// Notify our parent of the grid cell change
		NotifyParentClick();
	}

	return true;
}

BOOL CTaskCalendarCtrl::GetGridCell(DWORD dwTaskID, int &nRow, int &nCol) const
{
	int nUnused;
	return GetGridCell(dwTaskID, nRow, nCol, nUnused);
}

BOOL CTaskCalendarCtrl::GetGridCell(DWORD dwTaskID, int &nRow, int &nCol, int& nTask) const
{
	nRow = nCol = nTask = -1;

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

	return false;
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

	if (!CalcTaskCellRect(nTask, pCell, rCell, rLabel))
		return FALSE;

	const TASKCALITEM* pTCI = m_mapData.GetTaskItem(GetRealTaskID(dwTaskID));
	
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

CTaskCalendarCtrl::CONTINUOUSDRAWINFO& CTaskCalendarCtrl::GetTaskContinuousDrawInfo(DWORD dwTaskID) const
{
	ASSERT(dwTaskID);

	if (!HasOption(TCCO_DISPLAYCONTINUOUS))
	{
		static CONTINUOUSDRAWINFO nullDrawInfo;
		nullDrawInfo.Reset();

		return nullDrawInfo;
	}

	int nItem, nNumItem = m_aContinuousDrawInfo.GetSize();

	for (nItem = 0; nItem < nNumItem; nItem++)
	{
		if (m_aContinuousDrawInfo[nItem].dwTaskID == dwTaskID)
			break;
	}

	if (nItem == nNumItem)
	{
		CONTINUOUSDRAWINFO cdi(dwTaskID);
		nItem = m_aContinuousDrawInfo.Add(cdi);
	}

	return m_aContinuousDrawInfo[nItem];
}

TASKCALITEM* CTaskCalendarCtrl::GetTaskCalItem(DWORD dwTaskID, BOOL bIncFutureItems) const
{
	ASSERT(dwTaskID);
	TASKCALITEM* pTCI = m_mapData.GetTaskItem(dwTaskID);

	if (!pTCI && bIncFutureItems && (dwTaskID > m_dwMaximumTaskID))
	{
		pTCI = m_mapFutureOccurrences.GetTaskItem(dwTaskID);
		ASSERT(pTCI);
	}

	return pTCI;
}

BOOL CTaskCalendarCtrl::IsTaskLocked(DWORD dwTaskID) const
{
	const TASKCALITEM* pTCI = GetTaskCalItem(dwTaskID);

	return (pTCI && pTCI->bLocked);
}

BOOL CTaskCalendarCtrl::IsTaskDone(DWORD dwTaskID, BOOL bIncGoodAs) const
{
	const TASKCALITEM* pTCI = GetTaskCalItem(dwTaskID);
	
	return (pTCI && pTCI->IsDone(bIncGoodAs));
}

BOOL CTaskCalendarCtrl::TaskHasDependencies(DWORD dwTaskID) const
{
	const TASKCALITEM* pTCI = GetTaskCalItem(dwTaskID);
	
	return (pTCI && pTCI->bHasDepends);
}

BOOL CTaskCalendarCtrl::HasTask(DWORD dwTaskID, BOOL bExcludeHidden) const
{
	if (dwTaskID == 0)
		return FALSE;

	TASKCALITEM* pTCI = NULL;
	
	if (!m_mapData.Lookup(dwTaskID, pTCI))
		return FALSE;

	if (bExcludeHidden)
	{
		if (!pTCI->IsValid())
			return FALSE;

		if (pTCI->IsParent() && (HasOption(TCCO_HIDEPARENTTASKS)))
			return FALSE;

		if (pTCI->IsDone(TRUE) && !HasOption(TCCO_DISPLAYDONE))
			return FALSE;
	}

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
	if (!HasTask(dwTaskID, FALSE)) // don't exclude hidden tasks
		return FALSE;

	if (dwTaskID != GetSelectedTaskID())
	{
		m_dwSelectedTaskID = dwTaskID;

		if (bNotify)
			GetParent()->SendMessage(WM_CALENDAR_SELCHANGE, 0, GetSelectedTaskID());

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

	if (!pTCI)
		return 0;

	if (pTCI->IsParent() && HasOption(TCCO_HIDEPARENTTASKS))
		return 0;

	if (pTCI->IsDone(TRUE) && !HasOption(TCCO_DISPLAYDONE))
		return 0;

	if (!pTCI->HasAnyStartDate() && !pTCI->HasAnyEndDate())
		return 0;

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
	DWORD dwSelID = HitTest(point);
	
	if (dwSelID)
	{
		SetFocus();
		SelectTask(GetRealTaskID(dwSelID), FALSE, TRUE);

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
	if (m_mapFutureOccurrences.HasTask(dwTaskID))
		return m_mapFutureOccurrences.GetRealTaskID(dwTaskID);

	// else
	return dwTaskID;
}

BOOL CTaskCalendarCtrl::StartDragging(const CPoint& ptCursor)
{
	ASSERT(!m_bReadOnly);

	TCC_HITTEST nHit = TCCHT_NOWHERE;
	DWORD dwTaskID = HitTest(ptCursor, nHit);
	
	BOOL bCanDrag = CanDragTask(dwTaskID, nHit);

	if (bCanDrag <= 0)
		return FALSE;

	// when not drawing tasks continuously, it's possible
	// for the act of selecting a task to change its
	// position and thus its hit-test result
	if (dwTaskID != m_dwSelectedTaskID)
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
	
	m_tciPreDrag = *(GetTaskCalItem(dwTaskID));
	m_ptDragOrigin = ptCursor;

	// keep parent informed
	NotifyParentDragChange();

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
		COleDateTime dtOrg;
		GetDateFromPoint(m_ptDragOrigin, dtOrg);
		
		// offset from pre-drag position
		double dOffset = (dtDrag.m_dt - dtOrg.m_dt);

		if (m_tciPreDrag.IsStartDateSet())
		{
			dtDrag = (m_tciPreDrag.GetAnyStartDate().m_dt + dOffset);

			dtDrag.m_dt = min(dtDrag.m_dt, (dtRowMax.m_dt + 1.0 - dSnap));
			dtDrag.m_dt = max(dtDrag.m_dt, dtRowMin.m_dt);

			bEndOfDay = FALSE;
		}
		else
		{
			ASSERT(m_tciPreDrag.IsEndDateSet());

			dtDrag = (m_tciPreDrag.GetAnyEndDate().m_dt + dOffset);

			dtDrag.m_dt = min(dtDrag.m_dt, (dtRowMax.m_dt + 1.0));
			dtDrag.m_dt = max(dtDrag.m_dt, (dtRowMin.m_dt + dSnap));

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
		// active keys override
		if (Misc::ModKeysArePressed(MKS_CTRL))
		{
			m_nSnapMode = TCCSM_NEARESTHOUR;
		}
		else if (Misc::ModKeysArePressed(MKS_SHIFT))
		{
			m_nSnapMode = TCCSM_NEARESTDAY;
		}
		else if (Misc::ModKeysArePressed(MKS_CTRL | MKS_SHIFT))
		{
			m_nSnapMode = TCCSM_NEARESTHALFDAY;
		}
	}

	return m_nSnapMode;
}

void CTaskCalendarCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bReadOnly && UpdateDragging(point))
		return;

	CCalendarCtrlEx::OnMouseMove(nFlags, point);
}

BOOL CTaskCalendarCtrl::UpdateDragging(const CPoint& ptCursor)
{
	if (IsDragging())
	{
		TASKCALITEM* pTCI = GetTaskCalItem(m_dwSelectedTaskID);
		ASSERT(pTCI);
			
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
			// but without rebuilding future tasks because we do that at the 
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
			*pTCI = m_tciPreDrag;
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

	// else
	return FALSE;
}

void CTaskCalendarCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_bReadOnly && EndDragging(point))
		return;
	
	CCalendarCtrlEx::OnLButtonUp(nFlags, point);
}

BOOL CTaskCalendarCtrl::EndDragging(const CPoint& ptCursor)
{
	if (IsDragging())
	{
		TASKCALITEM* pTCI = GetTaskCalItem(m_dwSelectedTaskID);
		ASSERT(pTCI);

		// dropping outside the calendar is a cancel
		CRect rLimits;
		GetAllowableDragLimits(rLimits);

		TCC_HITTEST nDragWhat = TCCHT_NOWHERE;

		if (!rLimits.PtInRect(ptCursor) || !IsValidDrag(ptCursor))
		{
			*pTCI = m_tciPreDrag;
		}
		else if (m_bDraggingStart)
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

		// cleanup
		m_bDraggingStart = m_bDraggingEnd = m_bDragging = FALSE;
		ReleaseCapture();

		// keep parent informed
		if (!NotifyParentDateChange(nDragWhat))
		{
			*pTCI = m_tciPreDrag;
		}
		else if (m_mapRecurringTaskIDs.Has(m_dwSelectedTaskID))
		{
			RebuildCellTasks();
		}

		Invalidate(FALSE);
		NotifyParentDragChange();

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CTaskCalendarCtrl::GetSelectedTaskDates(COleDateTime& dtStart, COleDateTime& dtDue) const
{
	if (GetSelectedTaskID() == 0)
		return FALSE;

	TASKCALITEM* pTCI = GetTaskCalItem(m_dwSelectedTaskID);
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
	
	if (dwTaskID)
	{
		if (m_mapFutureOccurrences.HasTask(dwTaskID))
			return FALSE;

		if (IsTaskLocked(dwTaskID))
			return FALSE;

		if (IsTaskDone(dwTaskID, FALSE))
			return FALSE;
		
		BOOL bCanDrag = !HasOption(TCCO_PREVENTDEPENDENTDRAGGING) ||
						!TaskHasDependencies(dwTaskID);
			
		switch (nHit)
		{
		case TCCHT_BEGIN:
			return bCanDrag;
			
		case TCCHT_END:
			return TRUE; // always
			
		case TCCHT_MIDDLE:
			return bCanDrag;
		}
	}

	// all else
	return -1;
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
		DWORD dwHitID = HitTest(ptCursor, nHit);

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
			if (IsTaskLocked(dwTaskID))
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
	
	*pTCI = m_tciPreDrag;
	m_bDragging = m_bDraggingStart = m_bDraggingEnd = FALSE;
	
	if (bReleaseCapture)
		ReleaseCapture();

	Invalidate(FALSE);
	UpdateWindow();

	// keep parent informed
	NotifyParentDragChange();
}

void CTaskCalendarCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	DWORD dwTaskID = HitTest(point);
	SelectTask(dwTaskID, FALSE, TRUE);
	
	CCalendarCtrlEx::OnRButtonDown(nFlags, point);
}

int CTaskCalendarCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CCalendarCtrlEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_fonts.Initialise(*this);

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

BOOL CTaskCalendarCtrl::ProcessMessage(MSG* /*pMsg*/) 
{
	return false;
}

void CTaskCalendarCtrl::FilterToolTipMessage(MSG* pMsg) 
{
	if (m_tooltip.GetSafeHwnd())
		m_tooltip.FilterToolTipMessage(pMsg);
}

int CTaskCalendarCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	// perform a hit-test
	DWORD dwTaskID = HitTest(point);

	if (dwTaskID)
	{
		int nTextOffset = GetTaskContinuousDrawInfo(dwTaskID).nTextOffset;

		if ((nTextOffset > 0) || 
			!HasOption(TCCO_DISPLAYCONTINUOUS) ||
			(GetTaskHeight() < MIN_TASK_HEIGHT))
		{
			const TASKCALITEM* pTCI = GetTaskCalItem(dwTaskID, TRUE); // include future items
			ASSERT(pTCI);

			CRect rLabel;
			VERIFY(GetTaskLabelRect(dwTaskID, rLabel));

			if (rLabel.PtInRect(point))
			{
				return CToolTipCtrlEx::SetToolInfo(*pTI, this, pTCI->GetName(), dwTaskID, rLabel);
			}
		}
	}

	// else
	return CCalendarCtrlEx::OnToolHitTest(point, pTI);
}

void CTaskCalendarCtrl::OnShowTooltip(NMHDR* pNMHDR, LRESULT* pResult)
{
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

	// Set the font first, bold for top level items
	const TASKCALITEM* pTCI = GetTaskCalItem(dwTaskID, TRUE); // include future items

	if (!pTCI)
	{
		ASSERT(0);
		return;
	}

	m_tooltip.SetFont(m_fonts.GetFont(pTCI->bTopLevel ? GMFS_BOLD : 0));

	// Calculate exact position required
	CRect rLabel;
	VERIFY(GetTaskLabelRect(dwTaskID, rLabel));
	ClientToScreen(rLabel);

	CRect rTip(rLabel);
	m_tooltip.AdjustRect(rTip, TRUE);
	rTip.OffsetRect(TIP_PADDING, 0);

	rTip.top = rLabel.top;
	rTip.bottom = rLabel.bottom;

	m_tooltip.SetWindowPos(NULL, rTip.left, rTip.top, 0, 0, (SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE));

	*pResult = TRUE; // we do the positioning
}

BOOL CTaskCalendarCtrl::SaveToImage(CBitmap& bmImage)
{
	if (!CanSaveToImage())
		return FALSE;

	CAutoFlag af(m_bSavingToImage, TRUE);
	CLockUpdates lock(GetSafeHwnd());
	CClientDC dc(this);
	CDC dcImage, dcClient;

	if (dcImage.CreateCompatibleDC(&dc) && dcClient.CreateCompatibleDC(&dc))
	{
		CBitmap bmClient;
		CRect rClient;
		GetClientRect(rClient);

		CRect rData(rClient), rCell;
		GetGridCellRect(0, 0, rCell);

		Goto(m_dtMin);
		COleDateTime dtStart(CDateHelper::GetStartOfWeek(GetMinDate()));

		int nDataWeeks = ((int)((m_dtMax.m_dt - dtStart.m_dt) / 7)) + 1;
		rData.bottom = (nDataWeeks * rCell.Height());

		if (bmImage.CreateCompatibleBitmap(&dc, rData.Width(), rData.Height()) &&
			bmClient.CreateCompatibleBitmap(&dc, rClient.Width(), rClient.Height()))
		{
			CBitmap* pOldImage = dcImage.SelectObject(&bmImage);
			CBitmap* pOldClient = dcClient.SelectObject(&bmClient);

			dcImage.FillSolidRect(rData, GetSysColor(COLOR_WINDOW));

			BOOL bFirst = TRUE;
			int nVImageOffset = 0;
			int nClientWidth = rClient.Width(), nClientHeight = rClient.Height();
			int nHeaderOffset = 0;

			do 
			{
				dcClient.FillSolidRect(rClient, GetSysColor(COLOR_WINDOW));

				// Draw days of week header once only
				if (bFirst)
					DrawHeader(&dcClient);

				DrawCells(&dcClient);	
				DrawGrid(&dcClient);

				// Draw gridline at top of cells for subsequent
				if (!bFirst)
					dcClient.FillSolidRect(0, m_nHeaderHeight, nClientWidth, 1, m_crGrid);

				dcImage.BitBlt(0, nVImageOffset, nClientWidth, nClientHeight, &dcClient, 0, nHeaderOffset, SRCCOPY);

				nVImageOffset += rClient.Height();

				if (bFirst)
				{
					nHeaderOffset = m_nHeaderHeight;
					nClientHeight -= nHeaderOffset;
				
					bFirst = FALSE;
				}
				else
				{
					nVImageOffset -= nHeaderOffset;
				}

				dtStart.m_dt += (m_nVisibleWeeks * 7.0);
				Goto(dtStart);
			}
			while (dtStart < m_dtMax);
		}
	}

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
