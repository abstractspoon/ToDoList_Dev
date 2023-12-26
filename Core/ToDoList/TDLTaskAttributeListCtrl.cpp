// TDLTaskAttributeListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLTaskAttributeListCtrl.h"
#include "TDLTaskCtrlBase.h"

#include "tdcstatic.h"
#include "tdcstruct.h"

#include "..\shared\GraphicsMisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

enum 
{
	ATTRIB_COL,
	VALUE_COL
};

/////////////////////////////////////////////////////////////////////////////

static CContentMgr s_mgrContent;

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeListCtrl

CTDLTaskAttributeListCtrl::CTDLTaskAttributeListCtrl(const CTDLTaskCtrlBase& taskCtrl,
													 const CToDoCtrlData& data,
													 const TDCCOLEDITVISIBILITY& vis)
	:
	m_taskCtrl(taskCtrl),
	m_data(data),
	m_formatter(data, s_mgrContent),
	m_vis(vis)
{
}

CTDLTaskAttributeListCtrl::~CTDLTaskAttributeListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTDLTaskAttributeListCtrl, CInputListCtrl)
	//{{AFX_MSG_MAP(CTDLTaskAttributeListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DROPFILES()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeListCtrl message handlers

int CTDLTaskAttributeListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CInputListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetView(LVS_REPORT);
	EnableSorting(TRUE);
	ShowGrid(TRUE, TRUE);
	
	// Add columns
	AddCol(_T("Attribute"));
	AddCol(_T("Value"));

	// Add attributes
	Populate();
	
	return 0;
}

void CTDLTaskAttributeListCtrl::SetAttributeVisibility(const TDCCOLEDITVISIBILITY& vis)
{
	BOOL bColChange = FALSE, bEditChange = FALSE;
	BOOL bChange = m_vis.HasDifferences(vis, bColChange, bEditChange);

	if (!bChange)
		return;

	m_vis = vis;

	if (bEditChange || (bColChange && (vis.GetShowFields() == TDLSA_ASCOLUMN)))
	{
		DeleteAllItems();
		Populate();
	}
}

void CTDLTaskAttributeListCtrl::CheckAddAttribute(TDC_ATTRIBUTE nAttribID, UINT nAttribResID)
{
	if (m_vis.IsEditFieldVisible(nAttribID) ||
		m_vis.IsColumnVisible(TDC::MapAttributeToColumn(nAttribID)))
	{
		int nItem = AddRow(CEnString(nAttribResID));
		SetItemData(nItem, nAttribID);
	}
}

void CTDLTaskAttributeListCtrl::Populate()
{
	for (int nAttrib = 1; nAttrib < ATTRIB_COUNT; nAttrib++)
		CheckAddAttribute(ATTRIBUTES[nAttrib].nAttribID, ATTRIBUTES[nAttrib].nAttribResID);

	// Dependent time fields
	CheckAddAttribute(TDCA_STARTTIME, IDS_TDLBC_STARTTIME);
	CheckAddAttribute(TDCA_DUETIME, IDS_TDLBC_DUETIME);
	CheckAddAttribute(TDCA_DONETIME, IDS_TDLBC_DONETIME);

	RefreshSelectedTaskAttributeValues();
	Sort();
}

void CTDLTaskAttributeListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CInputListCtrl::OnSize(nType, cx, cy);
	
	if (GetColumnCount())
	{
		SetColumnWidth(ATTRIB_COL, (cx / 2));
		SetColumnWidth(VALUE_COL, (cx / 2) - 1);
	}
}

void CTDLTaskAttributeListCtrl::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
	CInputListCtrl::OnDropFiles(hDropInfo);
}

BOOL CTDLTaskAttributeListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CInputListCtrl::OnEraseBkgnd(pDC);
}

BOOL CTDLTaskAttributeListCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CInputListCtrl::OnSetCursor(pWnd, nHitTest, message);
}

IL_COLUMNTYPE CTDLTaskAttributeListCtrl::GetCellType(int nRow, int nCol) const
{
	if (nCol == ATTRIB_COL)
		return ILCT_TEXT;

	// else
	switch (GetItemData(nRow))
	{
		// Text fields
		case TDCA_TASKNAME:
		case TDCA_COST:
		case TDCA_EXTERNALID:
		case TDCA_PERCENT:
		case TDCA_TIMEESTIMATE:
		case TDCA_TIMESPENT:
			return ILCT_TEXT;

		// Date fields
		case TDCA_DONEDATE:
		case TDCA_DUEDATE:
		case TDCA_STARTDATE:
			return ILCT_DATE;

		// Drop lists
		case TDCA_PRIORITY:
		case TDCA_COLOR:
		case TDCA_ALLOCTO:
		case TDCA_ALLOCBY:
		case TDCA_STATUS:
		case TDCA_CATEGORY:
		case TDCA_TAGS:
		case TDCA_FILELINK:
		case TDCA_RISK:
		case TDCA_VERSION:
			return ILCT_DROPLIST;

		// Browse-like fields
		case TDCA_ICON:
		case TDCA_RECURRENCE:
		case TDCA_DEPENDENCY:
			return ILCT_BROWSE;

		// Checkbox fields
		case TDCA_FLAG:
		case TDCA_LOCK:
			return ILCT_CHECK;

		// Read-only fields
		case TDCA_CREATEDBY:
		case TDCA_PATH:
		case TDCA_POSITION:
		case TDCA_CREATIONDATE:
		case TDCA_LASTMODDATE:
		case TDCA_COMMENTSSIZE:
		case TDCA_COMMENTSFORMAT:
		case TDCA_SUBTASKDONE:
		case TDCA_LASTMODBY:
		case TDCA_ID:
		case TDCA_PARENTID:
			return ILCT_TEXT;
	}

	// All else
	return ILCT_TEXT;
}

BOOL CTDLTaskAttributeListCtrl::CanEditCell(int nRow, int nCol) const
{
	if (nCol == ATTRIB_COL)
		return FALSE;

	// else
	switch (GetItemData(nRow))
	{
		// Read-only fields
		case TDCA_CREATEDBY:
		case TDCA_PATH:
		case TDCA_POSITION:
		case TDCA_CREATIONDATE:
		case TDCA_LASTMODDATE:
		case TDCA_COMMENTSSIZE:
		case TDCA_COMMENTSFORMAT:
		case TDCA_SUBTASKDONE:
		case TDCA_LASTMODBY:
		case TDCA_ID:
		case TDCA_PARENTID:
			return FALSE;
	}

	// All else
	return TRUE;
}

COLORREF CTDLTaskAttributeListCtrl::GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	if (nCol == VALUE_COL)
	{
		if (!CanEditCell(nItem, nCol))
			return GetSysColor(COLOR_3DFACE);

		if (GetItemData(nItem) == TDCA_COLOR)
		{
			COLORREF color = m_taskCtrl.GetSelectedTaskColor();

			if (color != CLR_NONE)
			{
				if (m_data.HasStyle(TDCS_TASKCOLORISBACKGROUND))
					return color;
			}
		}
	}

	// All else
	return CInputListCtrl::GetItemBackColor(nItem, nCol, bSelected, bDropHighlighted, bWndFocus);
}

COLORREF CTDLTaskAttributeListCtrl::GetItemTextColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	if (nCol == VALUE_COL)
	{
		if (!CanEditCell(nItem, nCol))
			return GetSysColor(COLOR_GRAYTEXT);

		if (GetItemData(nItem) == TDCA_COLOR)
		{
			COLORREF color = m_taskCtrl.GetSelectedTaskColor();

			if (color != CLR_NONE)
			{
				if (m_data.HasStyle(TDCS_TASKCOLORISBACKGROUND))
					return GraphicsMisc::GetBestTextColor(color);
				else
					return color;
			}
		}
	}

	// All else
	return CInputListCtrl::GetItemTextColor(nItem, nCol, bSelected, bDropHighlighted, bWndFocus);
}

void CTDLTaskAttributeListCtrl::RefreshSelectedTaskAttributeValues(BOOL bForceClear)
{
	int nRow = GetItemCount();

	while (nRow--)
	{
		if (bForceClear)
		{
			SetItemText(nRow, VALUE_COL, _T(""));
		}
		else
		{
			CString sValue;
			CStringArray aValues;

			DWORD dwSelTaskID = ((m_taskCtrl.GetSelectedCount() == 1) ? m_taskCtrl.GetSelectedTaskID() : 0);
			TDC_ATTRIBUTE nAttrib = (TDC_ATTRIBUTE)GetItemData(nRow);

			switch (nAttrib)
			{
				// Simple text
			case TDCA_EXTERNALID: sValue = m_taskCtrl.GetSelectedTaskExtID(); break;
			case TDCA_ALLOCBY: sValue = m_taskCtrl.GetSelectedTaskAllocBy(); break;
			case TDCA_STATUS: sValue = m_taskCtrl.GetSelectedTaskStatus(); break;
			case TDCA_VERSION: sValue = m_taskCtrl.GetSelectedTaskVersion(); break;
			case TDCA_ICON: sValue = m_taskCtrl.GetSelectedTaskIcon(); break;

				// Arrays
			case TDCA_ALLOCTO: m_taskCtrl.GetSelectedTaskAllocTo(aValues); break;
			case TDCA_CATEGORY: m_taskCtrl.GetSelectedTaskCategories(aValues); break;
			case TDCA_TAGS: m_taskCtrl.GetSelectedTaskTags(aValues); break;
			case TDCA_FILELINK: m_taskCtrl.GetSelectedTaskFileLinks(aValues, FALSE); break;

				// Checkboxes
			case TDCA_FLAG: sValue = m_taskCtrl.IsSelectedTaskFlagged() ? _T("+") : _T(""); break;
			case TDCA_LOCK: sValue = m_taskCtrl.IsSelectedTaskLocked() ? _T("+") : _T(""); break;

			case TDCA_COST: 
				{
					TDCCOST cost;
					if (m_taskCtrl.GetSelectedTaskCost(cost))
						sValue = cost.Format(2);
				}
				break;

				// Simple values
			case TDCA_PERCENT:
				{
					int nValue = m_taskCtrl.GetSelectedTaskPercent();

					if (nValue != -1)
						sValue = Misc::Format(nValue);
				}
				break;

			case TDCA_PRIORITY: 
				{
					int nPriority = m_taskCtrl.GetSelectedTaskPriority(); 
					
					if (nPriority != TDC_NOPRIORITYORISK)
						sValue = Misc::Format(nPriority);
				}
				break;

			case TDCA_RISK:
				{
					int nRisk = m_taskCtrl.GetSelectedTaskRisk();

					if (nRisk != TDC_NOPRIORITYORISK)
						sValue = Misc::Format(nRisk);
				}
				break;

			case TDCA_TIMEESTIMATE:
				{
					TDCTIMEPERIOD tp;
					if (m_taskCtrl.GetSelectedTaskTimeEstimate(tp))
						sValue = tp.Format(2);
				}
				break;

			case TDCA_TIMESPENT:
				{
					TDCTIMEPERIOD tp;
					if (m_taskCtrl.GetSelectedTaskTimeSpent(tp))
						sValue = tp.Format(2);
				}
				break;

				// Date fields
			case TDCA_DONEDATE:
			case TDCA_DUEDATE:
			case TDCA_STARTDATE:
				{
					COleDateTime date = m_taskCtrl.GetSelectedTaskDate(TDC::MapAttributeToDate(nAttrib));
					sValue = CDateHelper::FormatDate(date);
				}
				break;

			case TDCA_LASTMODDATE:
			case TDCA_CREATIONDATE:
				{
					COleDateTime date = m_taskCtrl.GetSelectedTaskDate(TDC::MapAttributeToDate(nAttrib));
					sValue = CDateHelper::FormatDate(date, DHFD_TIME);
				}
				break;

				// Time fields
			case TDCA_DONETIME:
			case TDCA_DUETIME:
			case TDCA_STARTTIME:
				{
					COleDateTime date = m_taskCtrl.GetSelectedTaskDate(TDC::MapAttributeToDate(nAttrib));

					if (CDateHelper::DateHasTime(date))
						sValue = CTimeHelper::FormatClockTime(date);
				}
				break;

			case TDCA_COLOR:
				if (m_taskCtrl.GetSelectedTaskColor() != CLR_NONE)
					sValue = CEnString(IDS_COLOR_SAMPLETEXT);
				break;

			case TDCA_RECURRENCE:
				{
					TDCRECURRENCE recurs;

					if (m_taskCtrl.GetSelectedTaskRecurrence(recurs))
						sValue = recurs.GetRegularityText();
				}
				break;

			case TDCA_DEPENDENCY:
				{
					CTDCDependencyArray aDepends;

					if (m_taskCtrl.GetSelectedTaskDependencies(aDepends))
						sValue = aDepends.Format();
				}
				break;

			case TDCA_POSITION:
				if (dwSelTaskID)
					sValue = m_formatter.GetTaskPosition(dwSelTaskID);
				break;


			case TDCA_ID:
				if (dwSelTaskID)
					sValue = Misc::Format(dwSelTaskID);
				break;

			case TDCA_PARENTID:
				{
					DWORD dwID = m_taskCtrl.GetSelectedTaskParentID();

					if (dwID)
						sValue = Misc::Format(dwID);
				}
				break;

			case TDCA_PATH:
				if (dwSelTaskID)
				{
					sValue = m_formatter.GetTaskPath(dwSelTaskID);
				}
				else 
				{
					DWORD dwID = m_taskCtrl.GetSelectedTaskParentID();

					if (dwID)
						sValue = m_formatter.GetTaskPath(m_taskCtrl.GetSelectedTaskID());
				}
				break;

			case TDCA_TASKNAME:			break; // TODO
			case TDCA_CREATEDBY:		break; // TODO
			case TDCA_COMMENTSSIZE:		break; // TODO
			case TDCA_COMMENTSFORMAT:	break; // TODO
			case TDCA_SUBTASKDONE:		break; // TODO
			case TDCA_LASTMODBY:		break; // TODO
			}

			if (aValues.GetSize())
				sValue = Misc::FormatArray(aValues, _T(", "));

			SetItemText(nRow, VALUE_COL, sValue);
		}

	}
}


