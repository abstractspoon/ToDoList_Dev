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

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const int PADDING = 3;

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
	m_dwOptions(TCCO_DISPLAYCONTINUOUS),
	m_bReadOnly(FALSE),
	m_nCellVScrollPos(0)
{
	GraphicsMisc::CreateFont(m_DefaultFont, _T("Tahoma"));
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

BEGIN_MESSAGE_MAP(CTaskCalendarCtrl, CCalendarCtrl)
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
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEWHEEL()
	ON_NOTIFY(TTN_SHOW, 0, OnShowTooltip)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskCalendarCtrl message handlers

int CTaskCalendarCtrl::GetDefaultTaskHeight()
{
	return DEF_TASK_HEIGHT;
}

void CTaskCalendarCtrl::SetOption(DWORD dwOption, BOOL bSet)
{
	if (dwOption)
	{
		DWORD dwPrev = m_dwOptions;

		if (bSet)
			m_dwOptions |= dwOption;
		else
			m_dwOptions &= ~dwOption;

		// specific handling
		if (m_dwOptions != dwPrev)
		{
			RecalcTaskDates();
		}
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

bool CTaskCalendarCtrl::PrepareNewTask(ITaskList* pTask) const
{
	// give the task a date that will make it appear in the calendar
	time_t tDate = DateToSeconds((GetMaxDate().m_dt + GetMinDate().m_dt) / 2);

	int nRow, nCol;

	if (GetLastSelectedGridCell(nRow, nCol))
	{
		const CCalendarCell* pCell = GetCell(nRow, nCol);
		ASSERT(pCell);

		tDate = DateToSeconds(pCell->date);
	}

	if (tDate <= 0)
		return false;

	HTASKITEM hNewTask = pTask->GetFirstTask();
	ASSERT(hNewTask);

	pTask->SetTaskStartDate(hNewTask, tDate);
	pTask->SetTaskDueDate(hNewTask, tDate);

	return true;
}

BOOL CTaskCalendarCtrl::WantEditUpdate(IUI_ATTRIBUTE nEditAttrib)
{
	switch (nEditAttrib)
	{
	case IUI_TASKNAME:
	case IUI_DONEDATE:
	case IUI_DUEDATE:
	case IUI_STARTDATE:
	case IUI_COLOR:
		return true;
	}
	
	// all else 
	return FALSE;
}

BOOL CTaskCalendarCtrl::WantSortUpdate(IUI_ATTRIBUTE nEditAttrib)
{
	switch (nEditAttrib)
	{
	case IUI_TASKNAME:
// 	case IUI_DONEDATE:
// 	case IUI_DUEDATE:
// 	case IUI_STARTDATE:
		return true;
	}
	
	// all else 
	return FALSE;
}

void CTaskCalendarCtrl::UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib)
{
	const ITaskList15* pTasks14 = GetITLInterface<ITaskList15>(pTasks, IID_TASKLIST15);
	BOOL bChange = FALSE;

	switch (nUpdate)
	{
	case IUI_ALL:
		DeleteData();
		BuildData(pTasks14, pTasks->GetFirstTask(), attrib, TRUE);
		bChange = TRUE;
		break;

	case IUI_NEW:
		BuildData(pTasks14, pTasks->GetFirstTask(), attrib, TRUE);
		bChange = TRUE;
		break;
		
	case IUI_EDIT:
		bChange = UpdateTask(pTasks14, pTasks14->GetFirstTask(), nUpdate, attrib, TRUE);
		break;
		
	case IUI_DELETE:
		bChange = RemoveDeletedTasks(pTasks14);
		break;
		
	case IUI_MOVE:
		ASSERT(0);
		break;
		
	default:
		ASSERT(0);
	}
	
	if (bChange)
		Invalidate(FALSE);
}

void CTaskCalendarCtrl::BuildTaskMap(const ITaskList15* pTasks, HTASKITEM hTask, 
							   CSet<DWORD>& mapIDs, BOOL bAndSiblings)
{
	if (hTask == NULL)
		return;
	
	mapIDs.AddKey(pTasks->GetTaskID(hTask));
	
	// children
	BuildTaskMap(pTasks, pTasks->GetFirstTask(hTask), mapIDs, TRUE);
	
	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			BuildTaskMap(pTasks, hSibling, mapIDs, FALSE);
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
}

BOOL CTaskCalendarCtrl::RemoveDeletedTasks(const ITaskList15* pTasks)
{
	CSet<DWORD> mapIDs;
	BuildTaskMap(pTasks, pTasks->GetFirstTask(NULL), mapIDs, TRUE);

	// traverse the data looking for items that do not 
	// exist in pTasks and delete them
	POSITION pos = m_mapData.GetStartPosition();
	DWORD dwTaskID = 0;
	TASKCALITEM* pTCI = NULL;
	BOOL bChange = FALSE;

	while (pos)
	{
		m_mapData.GetNextAssoc(pos, dwTaskID, pTCI);
		ASSERT(pTCI);
		ASSERT(pTCI->GetTaskID() == dwTaskID);

		if (!mapIDs.HasKey(dwTaskID))
		{
			delete pTCI;
			m_mapData.RemoveKey(dwTaskID);

			// clear selection if necessary
			if (m_dwSelectedTaskID == dwTaskID)
				m_dwSelectedTaskID = 0;

			bChange = TRUE;
		}
	}

	return bChange;
}

BOOL CTaskCalendarCtrl::UpdateTask(const ITaskList15* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, 
									const CSet<IUI_ATTRIBUTE>& attrib, BOOL bAndSiblings)
{
	if (hTask == NULL)
		return FALSE;

	ASSERT(nUpdate == IUI_EDIT);

	// Not interested in references
	if (pTasks->IsTaskReference(hTask))
		return FALSE;

	DWORD dwTaskID = pTasks->GetTaskID(hTask);
	BOOL bChange = FALSE;

	if (HasTask(dwTaskID)) 
	{
		TASKCALITEM* pTCI = GetTaskCalItem(dwTaskID);
		bChange = pTCI->UpdateTask(pTasks, hTask, attrib, m_dwOptions);
	}
	else // must be a parent or a new task
	{
		if (!pTasks->IsTaskParent(hTask))
		{
			BuildData(pTasks, hTask, attrib, FALSE);
		}
		else
		{
			HTASKITEM hSubtask = pTasks->GetFirstTask(hTask);
			ASSERT(hSubtask);
			
			bChange = UpdateTask(pTasks, hSubtask, nUpdate, attrib, TRUE);
		}
	}
	
	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			if (UpdateTask(pTasks, hSibling, nUpdate, attrib, FALSE))
				bChange = TRUE;
			
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}

	return bChange;
}

void CTaskCalendarCtrl::NotifyParentDateChange(TCC_HITTEST nHit)
{
	ASSERT(!m_bReadOnly);
	ASSERT(m_dwSelectedTaskID);

	if (nHit != TCCHT_NOWHERE)
		GetParent()->SendMessage(WM_CALENDAR_DATECHANGE, (WPARAM)nHit, m_dwSelectedTaskID);
}

void CTaskCalendarCtrl::NotifyParentDragChange()
{
	ASSERT(m_dwSelectedTaskID);

	GetParent()->SendMessage(WM_CALENDAR_DRAGCHANGE, (WPARAM)GetSnapMode(), m_dwSelectedTaskID);
}

BOOL CTaskCalendarCtrl::IsSpecialDate(const COleDateTime& date) const
{
	BOOL bDummy;
	return m_mapSpecial.Lookup(CDateHelper::GetDateOnly(date).m_dt, bDummy);
}

void CTaskCalendarCtrl::BuildData(const ITaskList15* pTasks, HTASKITEM hTask, const CSet<IUI_ATTRIBUTE>& attrib, BOOL bAndSiblings)
{
	if (hTask == NULL)
		return;

	// Not interested in references
	if (pTasks->IsTaskReference(hTask))
		return;

	// We are only interested in leaf (non-parent) tasks
	if (!pTasks->IsTaskParent(hTask))
	{
		// sanity check
		DWORD dwTaskID = pTasks->GetTaskID(hTask);
		ASSERT(!HasTask(dwTaskID));

		TASKCALITEM* pTCI = new TASKCALITEM(pTasks, hTask, attrib, m_dwOptions);
		m_mapData[dwTaskID] = pTCI;

		// process item for special dates
		if (pTCI->IsStartDateSet())
			m_mapSpecial[CDateHelper::GetDateOnly(pTCI->GetAnyStartDate())] = TRUE;

		if (pTCI->IsEndDateSet())
			m_mapSpecial[CDateHelper::GetDateOnly(pTCI->GetAnyEndDate())] = TRUE;
	}
	else // process children
	{
		BuildData(pTasks, pTasks->GetFirstTask(hTask), attrib, TRUE);
	}

	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);

		while (hSibling)
		{
			// FALSE == not siblings
			BuildData(pTasks, hSibling, attrib, FALSE);

			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
}

BOOL CTaskCalendarCtrl::SetVisibleWeeks(int nWeeks)
{
	if (CCalendarCtrl::SetVisibleWeeks(nWeeks))
	{
		// delete text font so it can be recreated
		// at the right size
		if (HasOption(TCCO_ADJUSTTASKHEIGHTS))
			GraphicsMisc::VerifyDeleteObject(m_fontAltText);

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
	POSITION pos = m_mapData.GetStartPosition();
	DWORD dwTaskID = 0;
	TASKCALITEM* pTCI = NULL;

	while (pos)
	{
		m_mapData.GetNextAssoc(pos, dwTaskID, pTCI);
		delete pTCI;
	}

	m_mapData.RemoveAll();
	m_mapSpecial.RemoveAll();
}

void CTaskCalendarCtrl::DrawHeader(CDC* pDC)
{
	CThemed th;
	BOOL bThemed = (th.AreControlsThemed() && th.Open(this, _T("HEADER")));
	
	CRect rc;
	GetClientRect(&rc);
	
	rc.bottom = CALENDAR_HEADER_HEIGHT;
	int nWidth = (rc.Width() / CALENDAR_NUM_COLUMNS);
	
	CFont* pOldFont = pDC->SelectObject(&m_DefaultFont);
	bool bShort = (CDateHelper::CalcLongestDayOfWeekName(pDC) > nWidth);
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
		}
		else
		{
			pDC->FillSolidRect(rCol, GetSysColor(COLOR_3DFACE));
			pDC->Draw3dRect(rCol, GetSysColor(COLOR_3DHIGHLIGHT), GetSysColor(COLOR_3DSHADOW));
		}
		
		CString csTitle = CDateHelper::GetDayOfWeekName(nDOW, bShort);
		CRect rText(rCol);
		rText.DeflateRect(0, 2, 0, 0);
		
		pDC->DrawText(csTitle, rText, DT_CENTER|DT_VCENTER);
		
		// next column
		rCol.left = rCol.right;
	}
	pDC->SelectObject(pOldFont);
}

void CTaskCalendarCtrl::DrawGrid(CDC* pDC)
{
	CCalendarCtrl::DrawGrid(pDC);
}

void CTaskCalendarCtrl::DrawCells(CDC* pDC)
{
	// rebuild build display
	m_nMaxDayTaskCount = 0;
	m_mapVertPos.RemoveAll();
	m_mapTextOffset.RemoveAll();

	if (m_mapData.GetCount())
	{
		for(int i=0; i<CALENDAR_MAX_ROWS ; i++)
		{
			for(int u=0; u<CALENDAR_NUM_COLUMNS ; u++)
			{
				CCalendarCell* pCell = GetCell(i, u);
				ASSERT(pCell);

				CTaskCalItemArray* pTasks = static_cast<CTaskCalItemArray*>(pCell->pUserData);

				if (pTasks == NULL)
				{
					pTasks = new CTaskCalItemArray();
					pCell->pUserData = pTasks;
				}

				ASSERT(pTasks);
				GetCellTasks(pCell->date, *pTasks, TRUE);
			}
		}
	}

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

	CCalendarCtrl::DrawCells(pDC);
}

void CTaskCalendarCtrl::DrawCell(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell, 
							 BOOL bSelected, BOOL bToday, BOOL bShowMonth)
{
	CCalendarCtrl::DrawCell(pDC, pCell, rCell, bSelected, bToday, bShowMonth);
}

void CTaskCalendarCtrl::DrawCellContent(CDC* pDC, const CCalendarCell* pCell, const CRect& rCell, 
										BOOL bSelected, BOOL bToday)
{
	// default drawing
	CCalendarCtrl::DrawCellContent(pDC, pCell, rCell, bSelected, bToday);

	UpdateCellScrollBarVisibility();

	// then ours
	if (!m_nMaxDayTaskCount)
		return;
	
	CTaskCalItemArray* pTasks = static_cast<CTaskCalItemArray*>(pCell->pUserData);
	ASSERT(pTasks);
	
	int nNumTasks = (pTasks ? pTasks->GetSize() : 0);

	if (!nNumTasks)
		return;
	
	// adjust cell rect for scrollbar
	CRect rCellTrue(rCell);
	BOOL bShowScroll = (IsCellScrollBarActive() && IsGridCellSelected(pCell));

	if (bShowScroll)
		rCellTrue.right -= GetSystemMetrics(SM_CXVSCROLL);
	
	BOOL bVScrolled = (bShowScroll || HasOption(TCCO_DISPLAYCONTINUOUS));
	BOOL bFocused = CDialogHelper::IsChildOrSame(this, GetFocus());
	BOOL bTextColorIsBkgnd = HasOption(TCCO_TASKTEXTCOLORISBKGND);
	
	int nTaskHeight = GetTaskHeight();
	int nStart = (bVScrolled ? m_nCellVScrollPos : 0);
	
	for (int nTask = nStart; nTask < nNumTasks; nTask++)
	{
		const TASKCALITEM* pTCI = (*pTasks)[nTask];
		ASSERT(pTCI);
		
		CRect rTask;
		
		if (!CalcTaskCellRect(pTCI->GetTaskID(), pCell, rCellTrue, rTask))
			continue;

		int nSaveDC = pDC->SaveDC();

		// draw selection
		BOOL bSelTask = (pTCI->GetTaskID() == m_dwSelectedTaskID);
		COLORREF crText = pTCI->GetTextColor(bSelTask, bTextColorIsBkgnd);

		if (bSelTask)
		{
 			DWORD dwSelFlags = GMIB_THEMECLASSIC;
 			CRect rClip(rCell);

			if (rTask.left <= rCellTrue.left)
				dwSelFlags |= GMIB_CLIPLEFT;

			if (rTask.right >= rCellTrue.right)
				dwSelFlags |= GMIB_CLIPRIGHT;

			GM_ITEMSTATE nState = (bFocused ? GMIS_SELECTED : GMIS_SELECTEDNOTFOCUSED);
			crText = GraphicsMisc::GetExplorerItemTextColor(crText, nState, GMIB_THEMECLASSIC);

			GraphicsMisc::DrawExplorerItemBkgnd(pDC, *this, nState, rTask, dwSelFlags, rClip);
		}
		else // draw task
		{
			DWORD dwFlags = GMDR_TOP;
			
			if (rTask.left > rCellTrue.left)
				dwFlags |= GMDR_LEFT;
			
			if (rTask.right < rCellTrue.right)
				dwFlags |= GMDR_RIGHT;
			
			if (rTask.bottom < rCellTrue.bottom)
				dwFlags |= GMDR_BOTTOM;
			
			COLORREF crFill = pTCI->GetFillColor(bTextColorIsBkgnd);
			COLORREF crBorder = pTCI->GetBorderColor(bTextColorIsBkgnd);
						
			GraphicsMisc::DrawRect(pDC, rTask, crFill, crBorder, 0, dwFlags);
		}
		
		// draw text if there is enough space
		if (nTaskHeight >= MIN_TASK_HEIGHT)
		{
			int nOffset = GetTaskTextOffset(pTCI->GetTaskID());
			CFont* pOldFont = NULL;
			
			if (nOffset != -1)
			{
				if (nOffset == 0)
					rTask.left += PADDING; // initial pos
				
				int nLeft = (rTask.left - nOffset);
				int nTop = (rTask.top + 1);
				
				if (m_fontAltText.GetSafeHandle())
				{
					pOldFont = pDC->SelectObject(&m_fontAltText);
				}
				else
				{
					if (pTCI->IsDone(FALSE) && m_fontDone.GetSafeHandle())
						pOldFont = pDC->SelectObject(&m_fontDone);

					nTop++;
				}

				COLORREF crOld = pDC->SetTextColor(crText);
				CString sTitle = pTCI->GetName();
				
				pDC->ExtTextOut(nLeft, nTop, ETO_CLIPPED, rTask, sTitle, NULL);
				pDC->SetTextColor(crOld);
				
				// update text pos
				nOffset += rTask.Width();
				
				// if the offset now exceeds the text extent we can stop
				int nExtent = pDC->GetTextExtent(sTitle).cx;
				
				if (nOffset >= nExtent)
					nOffset = -1;
				
				m_mapTextOffset[pTCI->GetTaskID()] = nOffset;

				// restore font
				pDC->SelectObject(pOldFont);
			}
						
			if (rTask.bottom >= rCellTrue.bottom)
				break;
		}
	}
}

void CTaskCalendarCtrl::SetStrikeThruDoneTasks(BOOL bStrikeThru)
{
	if (bStrikeThru)
	{
		VERIFY(GraphicsMisc::CreateFont(m_fontDone, m_DefaultFont, GMFS_STRIKETHRU, GMFS_STRIKETHRU));
	}
	else
	{
		GraphicsMisc::VerifyDeleteObject(m_fontDone);
	}
}

void CTaskCalendarCtrl::OnSetFocus(CWnd* pFocus)
{
	CCalendarCtrl::OnSetFocus(pFocus);

	UpdateCellScrollBarVisibility();
	Invalidate(FALSE);
}

void CTaskCalendarCtrl::OnKillFocus(CWnd* pFocus)
{
	CCalendarCtrl::OnKillFocus(pFocus);

	UpdateCellScrollBarVisibility();
	Invalidate(FALSE);
}

void CTaskCalendarCtrl::DrawCellFocus(CDC* /*pDC*/, const CCalendarCell* /*pCell*/, const CRect& /*rCell*/)
{
	// we handle the focus during drawing
	// CCalendarCtrl::DrawCellFocus(pDC, pCell, rCell);
}

BOOL CTaskCalendarCtrl::UpdateCellScrollBarVisibility()
{
	CRect rCell;
	int nRow = -1, nCol = -1;
	int nNumCellTasks = 0;

	BOOL bShowSB = (CDialogHelper::IsChildOrSame(this, GetFocus()) && 
					GetLastSelectedGridCell(nRow, nCol));
	BOOL bSuccess = TRUE;

	if (bShowSB)
	{
		bShowSB = GetCellRect(nRow, nCol, rCell, TRUE);

		if (bShowSB)
		{
			const CCalendarCell* pCell = GetCell(nRow, nCol);
			ASSERT(pCell);
			
			const CTaskCalItemArray* pTasks = static_cast<CTaskCalItemArray*>(pCell->pUserData);

			if (pTasks)
			{
				nNumCellTasks = pTasks->GetSize();
				bShowSB = ((nNumCellTasks * GetTaskHeight()) > rCell.Height());
			}
			else
			{
				bShowSB = FALSE;
			}
		}
	}

	if (!bShowSB)
	{
		if (m_sbCellVScroll.GetSafeHwnd())
		{
			if (m_sbCellVScroll.IsWindowVisible())
			{
				m_sbCellVScroll.ShowWindow(SW_HIDE);
				Invalidate(FALSE);
			}

			m_sbCellVScroll.EnableWindow(FALSE);
		}

		m_nCellVScrollPos = 0;
	}
	else
	{
		rCell.left = (rCell.right - GetSystemMetrics(SM_CXVSCROLL));

		if (!m_sbCellVScroll.GetSafeHwnd())
			bSuccess = m_sbCellVScroll.Create(WS_CHILD | SBS_VERT, rCell, this, (UINT)IDC_STATIC);
		else
			m_sbCellVScroll.MoveWindow(rCell);

		// update scrollbar info
		SCROLLINFO si = { sizeof(si), (SIF_PAGE | SIF_POS | SIF_RANGE) };

		si.nMin = 0;
		si.nMax = (nNumCellTasks - 1);
		si.nPage = min ((rCell.Height() / GetTaskHeight()), nNumCellTasks);
		
		if ((m_nCellVScrollPos < 0) || (m_nCellVScrollPos > (nNumCellTasks - (int)si.nPage)))
			m_nCellVScrollPos = 0;

		si.nPos = m_nCellVScrollPos;

		m_sbCellVScroll.EnableWindow(TRUE);
		m_sbCellVScroll.SetScrollInfo(&si, m_sbCellVScroll.IsWindowVisible());
		m_sbCellVScroll.ShowWindow(SW_SHOW);
	}

	return bSuccess;
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
	
	if (GetGridCellFromPoint(pt, nRow, nCol) && 
		IsGridCellSelected(nRow, nCol) &&
		IsCellScrollBarActive())
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
		return CCalendarCtrl::OnMouseWheel(nFlags, zDelta, pt);
#else
		CCalendarCtrl::OnMouseWheel(nFlags, zDelta, pt);
#endif
}

void CTaskCalendarCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == &m_sbCellVScroll)
	{
		CRect rCell;
		VERIFY(GetCellRect(0, 0, rCell, TRUE));

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

	// else
	CCalendarCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

int CTaskCalendarCtrl::GetTaskHeight() const
{
	int nHeight = DEF_TASK_HEIGHT;

	if (m_nMaxDayTaskCount && HasOption(TCCO_ADJUSTTASKHEIGHTS))
	{
		CRect rCell;
		GetCellRect(0, 0, rCell, TRUE);

		nHeight = (rCell.Height() / m_nMaxDayTaskCount);
	}

	return max(MIN_TASK_HEIGHT, min(nHeight, DEF_TASK_HEIGHT));
}

int CTaskCalendarCtrl::GetCellTasks(const COleDateTime& dtCell, CTaskCalItemArray& aTasks, BOOL bOrdered) const
{
	ASSERT(dtCell);

#ifdef _DEBUG
	int nDay = dtCell.GetDay();
	int nMonth = dtCell.GetMonth();
#endif

	aTasks.RemoveAll();

	double dCellStart = dtCell, dCellEnd = dCellStart + 1.0;
	POSITION pos = m_mapData.GetStartPosition();
	DWORD dwTaskID = 0;
	TASKCALITEM* pTCI = NULL;

	while (pos)
	{
		m_mapData.GetNextAssoc(pos, dwTaskID, pTCI);

		// ignore tasks with both start and end dates calculated
		if (!pTCI->IsValid())
			continue;

		// ignore completed tasks as required
		if (pTCI->IsDone(TRUE) && !HasOption(TCCO_DISPLAYDONE))
			continue;

		// draw continuous either if the flag is set or the item is selected
		if (HasOption(TCCO_DISPLAYCONTINUOUS))
		{
			if ((pTCI->GetAnyStartDate().m_dt < dCellEnd) && 
				(pTCI->GetAnyEndDate().m_dt >= dCellStart))
			{
				aTasks.Add(pTCI);
			}
		}
		else
		{
			BOOL bAdded = FALSE; // only add once

			if (HasOption(TCCO_DISPLAYCALCSTART) || (HasOption(TCCO_DISPLAYSTART) && pTCI->IsStartDateSet()))
			{
				if (CDateHelper::GetDateOnly(pTCI->GetAnyStartDate()) == dtCell)
				{
					aTasks.Add(pTCI);
					bAdded = TRUE;
				}
			}

			// only test for due/done if start not added
			if (!bAdded)
			{
				if (HasOption(TCCO_DISPLAYCALCDUE) || 
					((HasOption(TCCO_DISPLAYDUE) || HasOption(TCCO_DISPLAYDONE)) && pTCI->IsEndDateSet()))
				{
					if (CDateHelper::GetDateOnly(pTCI->GetAnyEndDate()) == dtCell)
						aTasks.Add(pTCI);
				}
			}
		}
	}

	if (bOrdered && aTasks.GetSize() > 1)
		qsort(aTasks.GetData(), aTasks.GetSize(), sizeof(TASKCALITEM*), CompareTCItems);

	// now go thru the list and set the position of each item 
	// if not already done
	int nMaxPos = 0;

	for (int nTask = 0; nTask < aTasks.GetSize(); nTask++)
	{
		const TASKCALITEM* pTCI = aTasks[nTask];
		ASSERT(pTCI);

		int nPos;

		if (!m_mapVertPos.Lookup(pTCI->GetTaskID(), nPos))
		{
			nPos = max(nMaxPos, nTask);
			m_mapVertPos[pTCI->GetTaskID()] = nPos;
		}

		nMaxPos = max(nMaxPos, nPos+1);
	}

	m_nMaxDayTaskCount = max(m_nMaxDayTaskCount, nMaxPos);

	return aTasks.GetSize();
}

int CTaskCalendarCtrl::CompareTCItems(const void* pV1, const void* pV2)
{
	typedef TASKCALITEM* PTASKCALITEM;

	const TASKCALITEM* pTCI1 = *(static_cast<const PTASKCALITEM*>(pV1));
	const TASKCALITEM* pTCI2 = *(static_cast<const PTASKCALITEM*>(pV2));

	// special case: Not drawing tasks continuous means that
	// the same task can appear twice
	if (pTCI1->GetTaskID() == pTCI2->GetTaskID())
	{
		//ASSERT(!HasOption(TCCO_DISPLAYCONTINUOUS));
		return 0;
	}
	
	// earlier start date
	if (pTCI1->GetAnyStartDate() < pTCI2->GetAnyStartDate())
		return -1;

	if (pTCI1->GetAnyStartDate() > pTCI2->GetAnyStartDate())
		return 1;

	// equal so test for later end date
	if (pTCI1->GetAnyEndDate() > pTCI2->GetAnyEndDate())
		return -1;

	if (pTCI1->GetAnyEndDate() < pTCI2->GetAnyEndDate())
		return 1;

	// equal so test for task ID
	if (pTCI1->GetTaskID() < pTCI2->GetTaskID())
		return -1;

	ASSERT(pTCI1->GetTaskID() > pTCI2->GetTaskID());
	return 1;
}

DWORD CTaskCalendarCtrl::HitTest(const CPoint& ptCursor) const
{
	TCC_HITTEST nHit = TCCHT_NOWHERE;
	return HitTest(ptCursor, nHit);
}

DWORD CTaskCalendarCtrl::HitTest(const CPoint& ptCursor, TCC_HITTEST& nHit) const
{
	nHit = TCCHT_NOWHERE;

	if (!m_nMaxDayTaskCount)
		return 0;

	int nRow, nCol;

	if (!GetGridCellFromPoint(ptCursor, nRow, nCol))
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
	GetCellRect(nRow, nCol, rCell, TRUE);
	
	// handle clicking above tasks
	if (ptCursor.y < rCell.top)
		return 0;
	
	int nTaskHeight = GetTaskHeight();
	int nPos = ((ptCursor.y - rCell.top) / nTaskHeight);
	
	// look thru the tasks for this pos
	for (int nTask = 0; nTask < pTasks->GetSize(); nTask++)
	{
		const TASKCALITEM* pTCI = (*pTasks)[nTask];
		ASSERT(pTCI);

		BOOL bVScrolled = (IsGridCellSelected(pCell) || HasOption(TCCO_DISPLAYCONTINUOUS));
		int nTaskPos = GetTaskVertPos(pTCI->GetTaskID(), bVScrolled);
		
		if (nTaskPos == nPos)
		{
			// now we figure out where on the item we hit
			COleDateTime dtHit;
			VERIFY(GetDateFromPoint(ptCursor, dtHit));
			
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
			
			return (nHit == TCCHT_NOWHERE) ? 0 : pTCI->GetTaskID();
		}
	}

	// nothing hit
	return 0;
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

void CTaskCalendarCtrl::EnsureVisible(DWORD dwTaskID, BOOL bShowStart)
{
	if (!bShowStart) // partial visibility ok
	{
		// is the task already visible to some degree
		int nRow, nCol;

		if (GetGridCellFromTask(dwTaskID, nRow, nCol))
 			return;
	}

	// else make it visible
	TASKCALITEM* pTCI = GetTaskCalItem(dwTaskID);
	ASSERT(pTCI);

	if (!pTCI)
		return;

	COleDateTime dtMin = GetMinDate(), dtMax = GetMaxDate();

	if (bShowStart)
	{
		if (pTCI->GetAnyStartDate() < dtMin || pTCI->GetAnyStartDate() > dtMax)
		{
			// need to scroll
			Goto(pTCI->GetAnyStartDate());
		}
	}
	else // allow any visibility
	{
		if (pTCI->GetAnyEndDate() <= dtMin || pTCI->GetAnyStartDate() >= dtMax)
		{
			// need to scroll
			Goto(pTCI->GetAnyStartDate());
		}
	}
}

bool CTaskCalendarCtrl::GetGridCellFromTask(DWORD dwTaskID, int &nRow, int &nCol) const
{
	// iterate the visible cells for the specified task
	CTaskCalItemArray aTasks;

	for(int i=0; i < GetVisibleWeeks() ; i++)
	{
		for(int u=0; u<CALENDAR_NUM_COLUMNS; u++)
		{
			const CCalendarCell* pCell = GetCell(i, u);
			
			int nTask = GetCellTasks(pCell->date, aTasks, FALSE);

			while (nTask--)
			{
				if (aTasks[nTask]->GetTaskID() == dwTaskID)
				{
					nRow = i;
					nCol = u;

					return true;
				}
			}
		}
	}

	return false;
}

BOOL CTaskCalendarCtrl::GetTaskLabelRect(DWORD dwTaskID, CRect& rLabel) const
{
	int nRow, nCol;

	// start with visibility check
	if (!GetGridCellFromTask(dwTaskID, nRow, nCol))
 		return FALSE;

	const CCalendarCell* pCell = GetCell(nRow, nCol);
	ASSERT(pCell);

	CRect rCell;
	VERIFY(GetCellRect(nRow, nCol, rCell, TRUE));

	return CalcTaskCellRect(dwTaskID, pCell, rCell, rLabel);
}

BOOL CTaskCalendarCtrl::CalcTaskCellRect(DWORD dwTaskID, const CCalendarCell* pCell, const CRect& rCell, CRect& rTask) const
{
	// check horizontal (date) intersection first
	TASKCALITEM* pTCI = GetTaskCalItem(dwTaskID);
	ASSERT(pTCI);

	double dCellStart = pCell->date;
	double dCellEnd = CDateHelper::GetEndOfDay(dCellStart).m_dt;

	if ((pTCI->GetAnyEndDate().m_dt < dCellStart) || (pTCI->GetAnyStartDate().m_dt > dCellEnd))
		return FALSE;

	// check vertical (pos) intersection next
	BOOL bVScrolled = (IsGridCellSelected(pCell) || HasOption(TCCO_DISPLAYCONTINUOUS));
	int nPos = GetTaskVertPos(dwTaskID, bVScrolled);

	ASSERT(nPos >= 0 && nPos < m_nMaxDayTaskCount);

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

int CTaskCalendarCtrl::GetTaskVertPos(DWORD dwTaskID, BOOL bVScrolled) const
{
	ASSERT(dwTaskID);
	int nPos = -1;

	m_mapVertPos.Lookup(dwTaskID, nPos);
	ASSERT(nPos >= 0 && nPos < m_nMaxDayTaskCount);

	if (bVScrolled)
		nPos -= m_nCellVScrollPos;

	return nPos;
}

int CTaskCalendarCtrl::GetTaskTextOffset(DWORD dwTaskID) const
{
	ASSERT(dwTaskID);
	int nPos = 0;

	// special case: Always return zero if NOT drawing continuous
	// and this task is NOT selected
	if (!HasOption(TCCO_DISPLAYCONTINUOUS)/* && (m_dwSelectedTaskID != dwTaskID)*/)
		return 0;

	if (!m_mapTextOffset.Lookup(dwTaskID, nPos))
		m_mapTextOffset[dwTaskID] = nPos;

	return nPos;
}

TASKCALITEM* CTaskCalendarCtrl::GetTaskCalItem(DWORD dwTaskID) const
{
	ASSERT(dwTaskID);
	TASKCALITEM* pTCI = NULL;

	m_mapData.Lookup(dwTaskID, pTCI);
	ASSERT(pTCI);

	return pTCI;
}

BOOL CTaskCalendarCtrl::HasTask(DWORD dwTaskID) const
{
	if (dwTaskID == 0)
		return FALSE;

	TASKCALITEM* pTCI = NULL;
	return m_mapData.Lookup(dwTaskID, pTCI);
}

// external version
BOOL CTaskCalendarCtrl::SelectTask(DWORD dwTaskID)
{
	return SelectTask(dwTaskID, FALSE);
}

// internal version
BOOL CTaskCalendarCtrl::SelectTask(DWORD dwTaskID, BOOL bNotify)
{
	if (!HasTask(dwTaskID))
		return FALSE;

	if (dwTaskID != m_dwSelectedTaskID)
	{
		m_dwSelectedTaskID = dwTaskID;

		if (bNotify)
			GetParent()->SendMessage(WM_CALENDAR_SELCHANGE, 0, dwTaskID);

		Invalidate(FALSE);
		UpdateWindow();
	}

	return TRUE;
}

void CTaskCalendarCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	DWORD dwSelID = HitTest(point);
	
	if (dwSelID)
	{
		SetFocus();
		SelectTask(dwSelID, TRUE);

		if (!m_bReadOnly)
			StartDragging(point);
	}
	else
	{
		CCalendarCtrl::OnLButtonDown(nFlags, point);
		UpdateWindow();
	}
}

BOOL CTaskCalendarCtrl::StartDragging(const CPoint& ptCursor)
{
	ASSERT(!m_bReadOnly);

	TCC_HITTEST nHit = TCCHT_NOWHERE;
	DWORD dwTaskID = HitTest(ptCursor, nHit);

	ASSERT((nHit == TCCHT_NOWHERE) || (dwTaskID != 0));

	if (nHit == TCCHT_NOWHERE)
		return FALSE;

	// when not drawing tasks continuously, it's possible
	// for the act of selecting a task to change its
	// position and thus its hit-test result
	if (dwTaskID != m_dwSelectedTaskID)
		return FALSE;

	if (!::DragDetect(GetSafeHwnd(), ptCursor))
		return FALSE;
	
	switch (nHit)
	{
	case TCCHT_BEGIN:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		m_bDraggingStart = TRUE;
		break;
		
	case TCCHT_END:
		m_bDraggingEnd = TRUE;
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		break;
		
	case TCCHT_MIDDLE:
		m_bDragging = TRUE;
		break;
		
	default:
		ASSERT(0);
		return FALSE;
	}
	
	m_tciPreDrag = *(GetTaskCalItem(dwTaskID));
	m_ptDragOrigin = ptCursor;

	SetCapture();

	// keep parent informed
	NotifyParentDragChange();

	return TRUE;
}

BOOL CTaskCalendarCtrl::GetValidDragDate(const CPoint& ptCursor, COleDateTime& dtDrag) const
{
	CPoint ptDrag(ptCursor);

	if (!ValidateDragPoint(ptDrag))
		return FALSE;

	if (!GetDateFromPoint(ptDrag, dtDrag))
		return FALSE;

	// if dragging the whole task, then we calculate
	// dtDrag as TASKCALITEM::dtStart/dtEnd offset by the
	// difference between the current drag pos and the
	// initial drag pos
	if (m_bDragging)
	{
		COleDateTime dtOrg;
		GetDateFromPoint(m_ptDragOrigin, dtOrg);
		
		// offset from pre-drag position
		double dOffset = dtDrag.m_dt - dtOrg.m_dt;

		if (m_tciPreDrag.IsStartDateSet())
			dtDrag = m_tciPreDrag.GetAnyStartDate().m_dt + dOffset;
		else
		{
			ASSERT(m_tciPreDrag.IsEndDateSet());
			dtDrag = m_tciPreDrag.GetAnyEndDate().m_dt + dOffset;
		}
		
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
	}
	
	// adjust date depending on snap mode
	switch (GetSnapMode())
	{
	case TCCSM_NEARESTHOUR:
		dtDrag = CDateHelper::GetNearestHour(dtDrag, m_bDraggingEnd);
		break;

	case TCCSM_NEARESTDAY:
		dtDrag = CDateHelper::GetNearestDay(dtDrag, m_bDraggingEnd);
		break;

	case TCCSM_NEARESTHALFDAY:
		dtDrag = CDateHelper::GetNearestHalfDay(dtDrag, m_bDraggingEnd);
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

	CCalendarCtrl::OnMouseMove(nFlags, point);
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
					pTCI->SetStartDate(min(dtDrag.m_dt, pTCI->GetAnyEndDate().m_dt - ONE_HOUR));
				}
				else if (m_bDraggingEnd)
				{
					pTCI->SetEndDate(max(dtDrag.m_dt, pTCI->GetAnyStartDate().m_dt + ONE_HOUR));
				}
				else // m_bDragging
				{
					if (pTCI->IsStartDateSet() && pTCI->IsEndDateSet())
					{
						double dDuration = (pTCI->GetAnyEndDate().m_dt - pTCI->GetAnyStartDate().m_dt);
						
						pTCI->SetStartDate(dtDrag);
						pTCI->SetEndDate(dtDrag.m_dt + dDuration);
					}
					else if (pTCI->IsStartDateSet())
					{
						pTCI->SetStartDate(dtDrag);
					}
					else if (pTCI->IsEndDateSet())
					{
						pTCI->SetEndDate(dtDrag);
					}
				}
			}
		}
		else
		{
			*pTCI = m_tciPreDrag;
		}

		// always recalc dates
		if (pTCI->IsEndDateSet() && !pTCI->IsStartDateSet() && (pTCI->GetAnyEndDate() < pTCI->GetAnyStartDate()))
		{
			int breakpoint = 0;
		}
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
	
	CCalendarCtrl::OnLButtonUp(nFlags, point);
}

BOOL CTaskCalendarCtrl::EndDragging(const CPoint& ptCursor)
{
	if (IsDragging())
	{
		TASKCALITEM* pTCI = GetTaskCalItem(m_dwSelectedTaskID);
		ASSERT(pTCI);

		// dropping outside the calendar is a cancel
		CRect rClient;
		GetClientRect(rClient);
		rClient.top += CALENDAR_HEADER_HEIGHT;

		TCC_HITTEST nDragWhat = TCCHT_NOWHERE;

		if (!rClient.PtInRect(ptCursor) || !IsValidDrag(ptCursor))
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
		Invalidate(FALSE);

		// keep parent informed
		NotifyParentDateChange(nDragWhat);
		NotifyParentDragChange();

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CTaskCalendarCtrl::GetSelectedTaskDates(COleDateTime& dtStart, COleDateTime& dtDue) const
{
	if ((m_dwSelectedTaskID == 0) || !HasTask(m_dwSelectedTaskID))
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

	if (GetGridCellFromPoint(ptCursor, nRow, nCol))
	{
		const CCalendarCell* pCell = GetCell(nRow, nCol);
		ASSERT(pCell);

		// calc proportion of day by 'x' coordinate
		CRect rCell;
		VERIFY(GetCellRect(nRow, nCol, rCell));

		double dTime = ((ptCursor.x - rCell.left) / (double)rCell.Width());
		date = pCell->date.m_dt + dTime;

		return TRUE;
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
		DWORD dwHitID = HitTest(ptCursor, nHit);

		switch (nHit)
		{
		case TCCHT_BEGIN:
		case TCCHT_END:
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
			return TRUE;

		case TCCHT_MIDDLE:
			//SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
			break;
		}
	}
	
	// else
	return CCalendarCtrl::OnSetCursor(pWnd, nHitTest, message);
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

BOOL CTaskCalendarCtrl::ValidateDragPoint(CPoint& ptDrag) const
{
	if (!IsDragging())
		return FALSE;

	CRect rClient;
	GetClientRect(rClient);

	rClient.top += CALENDAR_HEADER_HEIGHT;

	ptDrag.x = max(ptDrag.x, rClient.left);
	ptDrag.x = min(ptDrag.x, rClient.right);
	ptDrag.y = max(ptDrag.y, rClient.top);
	ptDrag.y = min(ptDrag.y, rClient.bottom);

	return TRUE;
}

void CTaskCalendarCtrl::OnCaptureChanged(CWnd *pWnd) 
{
	// if someone else grabs the capture we cancel any drag
	if (pWnd && (pWnd != this) && IsDragging())
		CancelDrag(FALSE);
	
	CCalendarCtrl::OnCaptureChanged(pWnd);
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
	
	CCalendarCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
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
	SelectTask(dwTaskID, TRUE);
	
	CCalendarCtrl::OnRButtonDown(nFlags, point);
}

int CTaskCalendarCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CCalendarCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_tooltip.Create(this))
	{
		m_tooltip.ModifyStyleEx(0, WS_EX_TRANSPARENT);
		m_tooltip.SetFont(&m_DefaultFont);
		m_tooltip.SetDelayTime(TTDT_INITIAL, 50);
		m_tooltip.SetDelayTime(TTDT_AUTOPOP, 10000);
	}
	
	return 0;
}

bool CTaskCalendarCtrl::ProcessMessage(MSG* pMsg) 
{
	m_tooltip.FilterToolTipMessage(pMsg);

	return false;
}

int CTaskCalendarCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	// perform a hit-test
	DWORD dwTaskID = HitTest(point);

	if (dwTaskID)
	{
		int nTextOffset = GetTaskTextOffset(dwTaskID);

		if ((nTextOffset > 0) || 
			!HasOption(TCCO_DISPLAYCONTINUOUS) ||
			(GetTaskHeight() < MIN_TASK_HEIGHT))
		{
			const TASKCALITEM* pTCI = GetTaskCalItem(dwTaskID);
			ASSERT(pTCI);

			CRect rLabel;
			VERIFY(GetTaskLabelRect(dwTaskID, rLabel));

			if (rLabel.PtInRect(point))
			{
				pTI->hwnd = GetSafeHwnd();
				pTI->uId = dwTaskID;
				pTI->uFlags |= TTF_TRANSPARENT;
				pTI->rect = rLabel;
				
				// MFC will free the duplicated string
				pTI->lpszText = _tcsdup(pTCI->GetName());

				return (int)dwTaskID;
			}
		}
	}

	// else
	return CCalendarCtrl::OnToolHitTest(point, pTI);
}

void CTaskCalendarCtrl::OnShowTooltip(NMHDR* pNMHDR, LRESULT* pResult)
{
	DWORD dwTaskID = m_tooltip.GetToolInfo().uId;

	if (dwTaskID == 0)
	{
		ASSERT(0);
		return;
	}

	*pResult = TRUE; // we do the positioning

	CRect rLabel;
	VERIFY(GetTaskLabelRect(dwTaskID, rLabel));
	ClientToScreen(rLabel);

	// Calculate exact width required
	const TASKCALITEM* pTCI = GetTaskCalItem(dwTaskID);
	ASSERT(pTCI);

	rLabel.right = (rLabel.left + GraphicsMisc::GetTextWidth(pTCI->GetName(), pNMHDR->hwndFrom));

	CRect rTip(rLabel);
	m_tooltip.AdjustRect(rTip, TRUE);
	rTip.OffsetRect(PADDING, 0);

	rTip.top = rLabel.top;
	rTip.bottom = rLabel.bottom;

	m_tooltip.SetWindowPos(NULL, rTip.left, rTip.top, rTip.Width(), rTip.Height(), 
							(SWP_NOACTIVATE | SWP_NOZORDER));
}
