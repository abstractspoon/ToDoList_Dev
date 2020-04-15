// TDCTreeListCtrl.cpp: implementation of the CTDCTaskCtrlBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDlTaskCtrlBase.h"
#include "todoctrldata.h"
#include "tdcstatic.h"
#include "tdcmsg.h"
#include "tdcimagelist.h"
#include "resource.h"

#include "..\shared\graphicsmisc.h"
#include "..\shared\autoflag.h"
#include "..\shared\holdredraw.h"
#include "..\shared\timehelper.h"
#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\themed.h"
#include "..\shared\wndprompt.h"
#include "..\shared\osversion.h"
#include "..\shared\webmisc.h"
#include "..\shared\enbitmap.h"
#include "..\shared\msoutlookhelper.h"
#include "..\shared\ScopedTimer.h"
#include "..\shared\FileIcons.h"

#include "..\3rdparty\colordef.h"

#include "..\Interfaces\Preferences.h"

#include <math.h>

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const int LV_COLPADDING			= GraphicsMisc::ScaleByDPIFactor(3);
const int HD_COLPADDING			= GraphicsMisc::ScaleByDPIFactor(6);
const int ICON_SIZE				= GraphicsMisc::ScaleByDPIFactor(16); 
const int MIN_RESIZE_WIDTH		= (ICON_SIZE + 3); 
const int COL_ICON_SIZE			= ICON_SIZE; 
const int COL_ICON_SPACING		= GraphicsMisc::ScaleByDPIFactor(2); 
const int MIN_COL_WIDTH			= GraphicsMisc::ScaleByDPIFactor(6);
const int MIN_TASKS_WIDTH		= GraphicsMisc::ScaleByDPIFactor(200);

const COLORREF COMMENTSCOLOR	= RGB(98, 98, 98);
const COLORREF ALTCOMMENTSCOLOR = RGB(164, 164, 164);

const UINT TIMER_BOUNDINGSEL	= 100;

const LPCTSTR APP_ICON			= _T("TDL_APP_ICON");

//////////////////////////////////////////////////////////////////////

enum
{
	IDC_TASKTREE = 100,		
	IDC_TASKTREECOLUMNS,		
	IDC_TASKTREEHEADER,		
};

//////////////////////////////////////////////////////////////////////

CTDCColumnMap	CTDLTaskCtrlBase::s_mapColumns;
short			CTDLTaskCtrlBase::s_nExtendedSelection = HOTKEYF_CONTROL | HOTKEYF_SHIFT;
double			CTDLTaskCtrlBase::s_dRecentModPeriod = 0.0;												

//////////////////////////////////////////////////////////////////////

const CString EMPTY_STR(_T(""));

//////////////////////////////////////////////////////////////////////
// Private classes

CTDLTaskCtrlBase::TDSORTFLAGS::TDSORTFLAGS()
	:
	bSortChildren(TRUE),
	bSortDueTodayHigh(FALSE),
	dwTimeTrackID(0),
	bIncStartTime(FALSE),
	bIncCreateTime(FALSE),
	bIncDueTime(FALSE),
	bIncDoneTime(FALSE)
{
}

BOOL CTDLTaskCtrlBase::TDSORTFLAGS::WantIncludeTime(TDC_COLUMN nColID) const
{
	switch (nColID)
	{
	case TDCC_CREATIONDATE:
	case TDCC_CREATIONTIME:
		return bIncCreateTime;

	case TDCC_STARTDATE:
	case TDCC_STARTTIME:
		return bIncStartTime;

	case TDCC_DUEDATE:
	case TDCC_DUETIME:
		return bIncDueTime;

	case TDCC_DONEDATE:
	case TDCC_DONETIME:
		return bIncDoneTime;

	case TDCC_LASTMODDATE:
		return TRUE;
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////

CTDLTaskCtrlBase::TDSORTPARAMS::TDSORTPARAMS(const CTDLTaskCtrlBase& tcb) 
	: 
	base(tcb)
{
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTDLTaskCtrlBase, CWnd)

//////////////////////////////////////////////////////////////////////

CTDLTaskCtrlBase::CTDLTaskCtrlBase(BOOL bSyncSelection,
								   const CTDCImageList& ilIcons,
								   const CToDoCtrlData& data, 
								   const CToDoCtrlFind& find,
								   const CTDCStyleMap& styles,
								   const TDCAUTOLISTDATA& tld,
								   const CTDCColumnIDMap& mapVisibleCols,
								   const CTDCCustomAttribDefinitionArray& aCustAttribDefs) 
	: 
	CTreeListSyncer(TLSF_SYNCFOCUS | TLSF_BORDER | TLSF_SYNCDATA | TLSF_SPLITTER | (bSyncSelection ? TLSF_SYNCSELECTION : 0)),
	m_data(data),
	m_find(find),
	m_styles(styles),
	m_tld(tld),
	m_ilTaskIcons(ilIcons),
	m_mapVisibleCols(mapVisibleCols),
	m_aCustomAttribDefs(aCustAttribDefs),
	m_crDone(CLR_NONE),
	m_crDue(CLR_NONE), 
	m_crDueToday(CLR_NONE),
	m_crFlagged(CLR_NONE),
	m_crStarted(CLR_NONE), 
	m_crStartedToday(CLR_NONE),
	m_crReference(CLR_NONE),
	m_crAltLine(CLR_NONE),
	m_crGridLine(CLR_NONE),
	m_nSortColID(TDCC_NONE),
	m_nSortDir(TDC_SORTNONE),
	m_dwTimeTrackTaskID(0), 
	m_dwEditTitleTaskID(0),
	m_dwNextUniqueTaskID(100),
	m_bSortingColumns(FALSE),
	m_nColorByAttrib(TDCA_NONE),
	m_bBoundSelecting(FALSE),
	m_comparer(data),
	m_calculator(data),
	m_formatter(data),
	m_bAutoFitSplitter(TRUE),
	m_imageIcons(16, 16),
	m_bEnableRecalcColumns(TRUE),
	m_bReadOnly(FALSE)
{
	// build one time column map
	if (s_mapColumns.IsEmpty())
	{
		// add all columns
		int nCol = NUM_COLUMNS;

		while (nCol--)
		{
			const TDCCOLUMN& tdcc = COLUMNS[nCol];
			s_mapColumns[tdcc.nColID] = &tdcc;
		}
	}
}

CTDLTaskCtrlBase::~CTDLTaskCtrlBase()
{
	Release();
}

///////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTDLTaskCtrlBase, CWnd)
//{{AFX_MSG_MAP(CTDCTaskCtrlBase)
//}}AFX_MSG_MAP
ON_WM_DESTROY()
ON_WM_SIZE()
ON_WM_CREATE()
ON_WM_SETCURSOR()
ON_WM_TIMER()
ON_WM_HELPINFO()

END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////

BOOL CTDLTaskCtrlBase::Create(CWnd* pParentWnd, const CRect& rect, UINT nID, BOOL bVisible)
{
	DWORD dwStyle = (WS_CHILD | (bVisible ? WS_VISIBLE : 0) | WS_TABSTOP);
	
	// create ourselves
	return CWnd::CreateEx(WS_EX_CONTROLPARENT, NULL, NULL, dwStyle, rect, pParentWnd, nID);
}

int CTDLTaskCtrlBase::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rect(0, 0, lpCreateStruct->cx, lpCreateStruct->cy);
	
	if (!CreateTasksWnd(this, rect, TRUE))
		return -1;

	DWORD dwStyle = (WS_CHILD | WS_VISIBLE);

	// Tasks Header ---------------------------------------------------------------------
	if (!m_hdrTasks.Create((dwStyle | HDS_BUTTONS), rect, this, IDC_TASKTREEHEADER))
		return FALSE;

	// Column List ---------------------------------------------------------------------
	rect.OffsetRect(rect.Width(), 0);

	if (!m_lcColumns.Create((dwStyle | WS_TABSTOP),	rect, this, IDC_TASKTREECOLUMNS))
		return FALSE;

	// extended styles
	ListView_SetExtendedListViewStyleEx(m_lcColumns, LVS_EX_HEADERDRAGDROP, LVS_EX_HEADERDRAGDROP);
	ListView_SetImageList(m_lcColumns, m_ilTaskIcons, LVSIL_SMALL);
	
	// subclass the tree and list
	if (HasStyle(TDCS_RIGHTSIDECOLUMNS))
	{
		if (!Sync(Tasks(), m_lcColumns, TLSL_RIGHTDATA_IS_LEFTITEM, m_hdrTasks))
			return FALSE;
	}
	else // left side
	{
		if (!Sync(m_lcColumns, Tasks(), TLSL_LEFTDATA_IS_RIGHTITEM, m_hdrTasks))
			return FALSE;
	}
		
	// Column Header ---------------------------------------------------------------------
	if (!m_hdrColumns.SubclassWindow(ListView_GetHeader(m_lcColumns)))
		return FALSE;

	m_hdrColumns.EnableToolTips();
	
	// set header font and calc char width
	CFont* pFont = m_hdrColumns.GetFont();
	m_hdrTasks.SetFont(pFont);

	CClientDC dc(&m_hdrTasks);
	m_fAveHeaderCharWidth = GraphicsMisc::GetAverageCharWidth(&dc, pFont);

	// Add some padding to the right of the checkbox for tree/list
	// so that the checkboxes, icons and labels have consistent positioning
	if (IsTreeList())
		VERIFY(GraphicsMisc::InitCheckboxImageList(*this, m_ilCheckboxes, IDB_CHECKBOXES, 255, CRect(0, 0, 3, 0)));
	else
		VERIFY(GraphicsMisc::InitCheckboxImageList(*this, m_ilCheckboxes, IDB_CHECKBOXES, 255));

	BuildColumns();
	OnColumnVisibilityChange(CTDCColumnIDMap());
	PostResize();

	// Tooltips for columns
	if (m_tooltipColumns.Create(this))
	{
		m_tooltipColumns.ModifyStyleEx(0, WS_EX_TRANSPARENT);
		m_tooltipColumns.SetDelayTime(TTDT_INITIAL, 50);
		m_tooltipColumns.SetDelayTime(TTDT_AUTOPOP, 10000);
		m_tooltipColumns.SetMaxTipWidth((UINT)(WORD)-1);

		// Disable columns own tooltips
		HWND hwndTooltips = (HWND)m_lcColumns.SendMessage(LVM_GETTOOLTIPS);

		if (hwndTooltips)
			::SendMessage(hwndTooltips, TTM_ACTIVATE, FALSE, 0);
	}

	return 0;
}

int CTDLTaskCtrlBase::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
{
	CWnd::ClientToScreen(&point);

	CString sTooltip;
	int nHitTest = GetTaskColumnTooltip(point, sTooltip);
	
	if ((nHitTest != -1) && !sTooltip.IsEmpty())
		return CToolTipCtrlEx::SetToolInfo(*pTI, m_lcColumns, sTooltip, nHitTest);

	return CWnd::OnToolHitTest(point, pTI);
}

int CTDLTaskCtrlBase::GetUniqueToolTipID(DWORD dwTaskID, TDC_COLUMN nColID, int nIndex)
{
	ASSERT(nIndex < 100);
	ASSERT(nColID < TDCC_COUNT);

	return (int)((((dwTaskID * TDCC_COUNT) + nColID) * 100) + nIndex);
}

int CTDLTaskCtrlBase::GetTaskColumnTooltip(const CPoint& ptScreen, CString& sTooltip) const
{
	TDC_COLUMN nColID = TDCC_NONE;
	DWORD dwTaskID = 0;
	
	if (HitTestColumnsItem(ptScreen, FALSE, nColID, &dwTaskID) == -1)
		return -1;

	ASSERT(nColID != TDCC_NONE);
	ASSERT(dwTaskID);

	const TODOITEM* pTDI = m_data.GetTrueTask(dwTaskID);

	if (!pTDI)
	{
		ASSERT(0);
		return -1;
	}

	switch (nColID)
	{
	case TDCC_RECURRENCE:
		break;

	case TDCC_RECENTEDIT:
		{
			COleDateTime dtLastMod = m_calculator.GetTaskLastModifiedDate(dwTaskID);

			if (TODOITEM::IsRecentlyModified(dtLastMod))
				sTooltip = CDateHelper::FormatDate(dtLastMod, (DHFD_DOW | DHFD_TIME | DHFD_NOSEC));

			return GetUniqueToolTipID(dwTaskID, nColID);
		}
		break;

	case TDCC_DEPENDENCY:
		if (pTDI->aDependencies.GetSize())
		{
			// Build list of Names and IDs
			for (int nDepend = 0; nDepend < pTDI->aDependencies.GetSize(); nDepend++)
			{
				if (nDepend > 0)
					sTooltip += '\n';

				const CString& sDepends = Misc::GetItem(pTDI->aDependencies, nDepend);
				DWORD dwDependID = (DWORD)_ttol(sDepends);
				
				sTooltip += sDepends; // always

				// If local, append task name
				if ((dwDependID != 0) && m_data.HasTask(dwDependID))
				{
					sTooltip += ' ';
					sTooltip += '(';
					sTooltip += m_data.GetTaskTitle(dwDependID);
					sTooltip += ')';
				}
			}			
			return GetUniqueToolTipID(dwTaskID, nColID);
		}
		break;

	case TDCC_DONE:
		if (pTDI->IsDone())
		{
			sTooltip = CDateHelper::FormatDate(pTDI->dateDone, (DHFD_DOW | DHFD_TIME | DHFD_NOSEC));
			return GetUniqueToolTipID(dwTaskID, nColID);
		}
		break;

	case TDCC_TRACKTIME:
		break;

	case TDCC_REMINDER:
		if (!HasStyle(TDCS_SHOWREMINDERSASDATEANDTIME))
		{
			time_t tRem = GetTaskReminder(dwTaskID);
			
			if (tRem == -1)
			{
				sTooltip = CEnString(IDS_REMINDER_DATENOTSET);
			}
			else if (tRem > 0)
			{
				sTooltip = CDateHelper::FormatDate(tRem, (DHFD_DOW | DHFD_TIME | DHFD_NOSEC));
			}

			if (!sTooltip.IsEmpty())
				return GetUniqueToolTipID(dwTaskID, nColID);
		}
		break;

	case TDCC_FILEREF:
		{
			int nIndex = HitTestFileLinkColumn(ptScreen);

			if (nIndex != -1)
			{
				sTooltip = FileMisc::GetFullPath(pTDI->aFileLinks[nIndex], m_sTasklistFolder);
				return GetUniqueToolTipID(dwTaskID, nColID, nIndex);
			}
		}
		break;

	case TDCC_NONE:
	case TDCC_CLIENT:
		ASSERT(0);
		return -1;

	default:
		break;
	}
	
	return -1;
}

void CTDLTaskCtrlBase::UpdateSelectedTaskPath()
{
	CEnString sHeader(IDS_TDC_COLUMN_TASK);
	
	// add the item path to the header
	if (HasStyle(TDCS_SHOWPATHINHEADER) && m_hdrTasks.GetItemCount())
	{
		if (GetSelectedCount() == 1)
		{
			CRect rHeader;
			::GetClientRect(m_hdrTasks, rHeader);
			
			int nColWidthInChars = (int)(rHeader.Width() / m_fAveHeaderCharWidth);
			CString sPath = m_formatter.GetTaskPath(GetSelectedTaskID(), nColWidthInChars);
			
			if (!sPath.IsEmpty())
				sHeader.Format(_T("%s [%s]"), CEnString(IDS_TDC_COLUMN_TASK), sPath);
		}
	}
	
	m_hdrTasks.SetItemText(0, sHeader);
	m_hdrTasks.Invalidate(FALSE);
}

void CTDLTaskCtrlBase::SetTimeTrackTaskID(DWORD dwTaskID)
{
	if (m_dwTimeTrackTaskID != dwTaskID)
	{
		m_dwTimeTrackTaskID = dwTaskID;
		
		// resort if appropriate
		if (m_sort.IsSortingBy(TDCC_TRACKTIME, FALSE))
		{
			Sort(TDCC_TRACKTIME, FALSE);
		}
		else
		{
			RedrawColumn(TDCC_TRACKTIME);
			RedrawColumn(TDCC_TIMESPENT);
		}
	}
}

void CTDLTaskCtrlBase::SetEditTitleTaskID(DWORD dwTaskID)
{
	if (m_dwEditTitleTaskID != dwTaskID)
	{
		m_dwEditTitleTaskID = dwTaskID;
	}
}

void CTDLTaskCtrlBase::SetNextUniqueTaskID(DWORD dwTaskID)
{
	if (m_dwNextUniqueTaskID != dwTaskID)
	{
		m_dwNextUniqueTaskID = dwTaskID;

		if (GetSafeHwnd())
			RecalcUntrackedColumnWidths(CTDCColumnIDMap(TDCC_ID));
	}
}

void CTDLTaskCtrlBase::OnDestroy() 
{
	Release();
	
	CWnd::OnDestroy();
}

void CTDLTaskCtrlBase::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	
	if (cx && cy)
	{
		CRect rect(0, 0, cx, cy);
		CTreeListSyncer::Resize(rect);

		if (m_bAutoFitSplitter)
			AdjustSplitterToFitAttributeColumns();

		// Delay-loaded to avoid resource leaks
		//VERIFY(m_ilFileRef.Initialize());
	}
}

BOOL CTDLTaskCtrlBase::IsListItemSelected(HWND hwnd, int nItem) const
{
	return (ListView_GetItemState(hwnd, nItem, LVIS_SELECTED) & LVIS_SELECTED);
}

void CTDLTaskCtrlBase::SetTasksWndStyle(DWORD dwStyles, BOOL bSet, BOOL bExStyle)
{
	CTreeListSyncer::ModifyStyle(Tasks(), (bSet ? 0 : dwStyles), (bSet ? dwStyles : 0), bExStyle);
}

void CTDLTaskCtrlBase::OnStylesUpdated(const CTDCStyleMap& styles, BOOL bAllowResort)
{
	BOOL bRecalcUntrackedCols = FALSE;
	BOOL bInvalidateAll = FALSE;
	BOOL bResort = FALSE;
	BOOL bSwapSides = FALSE;

	for (int nItem = TDCS_FIRST; nItem < TDCS_LAST; nItem++)
	{
		TDC_STYLE nStyle = (TDC_STYLE)nItem;

		if (!styles.HasStyle(nStyle))
			continue;

		BOOL bEnable = styles.IsStyleEnabled(nStyle);

		switch (nStyle)
		{
		case TDCS_NODUEDATEISDUETODAYORSTART:
		case TDCS_USEEARLIESTDUEDATE:
		case TDCS_USELATESTDUEDATE:
		case TDCS_USEEARLIESTSTARTDATE:
		case TDCS_USELATESTSTARTDATE:
		case TDCS_SHOWCOMMENTSINLIST:
		case TDCS_SHOWFIRSTCOMMENTLINEINLIST:
		case TDCS_STRIKETHOUGHDONETASKS:
		case TDCS_TASKCOLORISBACKGROUND:
		case TDCS_CALCREMAININGTIMEBYDUEDATE:
		case TDCS_CALCREMAININGTIMEBYSPENT:
		case TDCS_CALCREMAININGTIMEBYPERCENT:
		case TDCS_COLORTEXTBYATTRIBUTE:
			bInvalidateAll = TRUE;
			break;

		case TDCS_SHOWDATESINISO:
			m_dateTimeWidths.SetIsoFormat(bEnable);
			bInvalidateAll = TRUE;
			break;

		case TDCS_SORTDONETASKSATBOTTOM:
			bResort = TRUE;
			break;

		case TDCS_DUEHAVEHIGHESTPRIORITY:
		case TDCS_DONEHAVELOWESTPRIORITY:
			if (IsSortingBy(TDCC_PRIORITY))
				bResort = TRUE;
			break;

		case TDCS_DONEHAVELOWESTRISK:
			if (IsSortingBy(TDCC_RISK))
				bResort = TRUE;
			break;

		case TDCS_RIGHTSIDECOLUMNS:
			bSwapSides = (bEnable != IsShowingColumnsOnRight());
			break;

		case TDCS_DISPLAYHMSTIMEFORMAT:
		case TDCS_TREATSUBCOMPLETEDASDONE:
			bRecalcUntrackedCols = TRUE;
			break;

		case TDCS_USEHIGHESTPRIORITY:
		case TDCS_INCLUDEDONEINPRIORITYCALC:
		case TDCS_HIDEPRIORITYNUMBER:
			if (IsColumnShowing(TDCC_PRIORITY))
				bInvalidateAll = TRUE;
			break;

		case TDCS_USEHIGHESTRISK:
		case TDCS_INCLUDEDONEINRISKCALC:
			if (IsColumnShowing(TDCC_RISK))
				bInvalidateAll = TRUE;
			break;

		case TDCS_USEPERCENTDONEINTIMEEST:
			if (IsColumnShowing(TDCC_TIMEEST))
				bRecalcUntrackedCols = TRUE;
			break;

		case TDCS_SHOWREMINDERSASDATEANDTIME:
			if (IsColumnShowing(TDCC_REMINDER))
			{
				// Reset 'tracked' flag to ensure correct resize
				int nCol = GetColumnIndex(TDCC_REMINDER);
				ASSERT(nCol != -1);

				m_hdrColumns.SetItemTracked(nCol, FALSE);

				bRecalcUntrackedCols = TRUE;
			}
			break;

		case TDCS_HIDEZEROTIMECOST:
			if (IsColumnShowing(TDCC_TIMEEST) ||
				IsColumnShowing(TDCC_TIMESPENT) ||
				IsColumnShowing(TDCC_COST))
			{
				bRecalcUntrackedCols = TRUE;
			}
			break;

		case TDCS_ROUNDTIMEFRACTIONS:
			if (IsColumnShowing(TDCC_TIMEEST) ||
				IsColumnShowing(TDCC_TIMESPENT))
			{
				bRecalcUntrackedCols = TRUE;
			}
			break;

		case TDCS_HIDEPERCENTFORDONETASKS:
		case TDCS_INCLUDEDONEINAVERAGECALC:
		case TDCS_WEIGHTPERCENTCALCBYNUMSUB:
		case TDCS_SHOWPERCENTASPROGRESSBAR:
		case TDCS_HIDEZEROPERCENTDONE:
			if (IsColumnShowing(TDCC_PERCENT))
				bInvalidateAll = TRUE;
			break;

		case TDCS_AVERAGEPERCENTSUBCOMPLETION:
		case TDCS_AUTOCALCPERCENTDONE:
			if (IsColumnShowing(TDCC_PERCENT))
				bRecalcUntrackedCols = TRUE;
			else
				bInvalidateAll = TRUE;
			break;

		case TDCS_HIDESTARTDUEFORDONETASKS:
			if (IsColumnShowing(TDCC_STARTDATE) ||
				IsColumnShowing(TDCC_DUEDATE))
			{
				bRecalcUntrackedCols = TRUE;
			}
			break;

		case TDCS_SHOWWEEKDAYINDATES:
			if (IsColumnShowing(TDCC_STARTDATE) ||
				IsColumnShowing(TDCC_LASTMODDATE) ||
				IsColumnShowing(TDCC_DUEDATE) ||
				IsColumnShowing(TDCC_DONEDATE))
			{
				bRecalcUntrackedCols = TRUE;
			}
			break;

		case TDCS_SHOWPATHINHEADER:
			UpdateSelectedTaskPath();
			break;

		case TDCS_HIDEPANESPLITBAR:
			CTreeListSyncer::SetSplitBarWidth(bEnable ? 0 : 10);
			break;

		case TDCS_ALLOWTREEITEMCHECKBOX:
			SetTasksImageList(m_ilCheckboxes, TRUE, (bEnable && !IsColumnShowing(TDCC_DONE)));
			break;

		case TDCS_COLUMNHEADERSORTING:
			{
				DWORD dwAdd(bEnable ? HDS_BUTTONS : 0), dwRemove(bEnable ? 0 : HDS_BUTTONS);

				if (m_hdrTasks.GetSafeHwnd())
					m_hdrTasks.ModifyStyle(dwRemove, dwAdd);

				if (m_hdrColumns)
					m_hdrColumns.ModifyStyle(dwRemove, dwAdd);
			}
			break;
		}
	}

	if (bResort && bAllowResort)
	{
		Resort();
		bInvalidateAll = FALSE;
	}

	if (bRecalcUntrackedCols)
	{
		RecalcUntrackedColumnWidths();
		bInvalidateAll = FALSE;
	}

	if (bSwapSides)
	{
		SwapSides();
		bInvalidateAll = FALSE;
	}

	if (bInvalidateAll && IsVisible())
		InvalidateAll();

	PostResize();
}

BOOL CTDLTaskCtrlBase::InvalidateColumnItem(int nItem, BOOL bUpdate)
{
	return InvalidateItem(m_lcColumns, nItem, bUpdate);
}

BOOL CTDLTaskCtrlBase::InvalidateColumnSelection(BOOL bUpdate)
{
	return InvalidateSelection(m_lcColumns, bUpdate);
}

BOOL CTDLTaskCtrlBase::InvalidateSelection(CListCtrl& lc, BOOL bUpdate)
{
	BOOL bInvalidated = FALSE;
	POSITION pos = lc.GetFirstSelectedItemPosition();

	while (pos)
	{
		int nItem = lc.GetNextSelectedItem(pos);
		bInvalidated |= InvalidateItem(lc, nItem, FALSE); // don't update until the end
	}
	
	if (bUpdate && bInvalidated)
		lc.UpdateWindow();

	return bInvalidated;
}

BOOL CTDLTaskCtrlBase::InvalidateItem(CListCtrl& lc, int nItem, BOOL bUpdate)
{
	ASSERT(nItem != -1);
	
	if (nItem == -1)
		return FALSE;

	BOOL bInvalidated = FALSE;
	CRect rItem;

	if (lc.GetItemRect(nItem, rItem, LVIR_BOUNDS))
	{
		lc.InvalidateRect(rItem);
		bInvalidated = TRUE;
	}
	
	if (bUpdate && bInvalidated)
		lc.UpdateWindow();

	return bInvalidated;
}

void CTDLTaskCtrlBase::InvalidateAll(BOOL bErase, BOOL bUpdate) 
{ 
	CTreeListSyncer::InvalidateAll(bErase, bUpdate); 
}

BOOL CTDLTaskCtrlBase::IsShowingColumnsOnRight() const
{
	return (Right() == m_lcColumns);
}

void CTDLTaskCtrlBase::OnUndoRedo(BOOL /*bUndo*/)
{
	// resync scroll pos
	PostResync(m_lcColumns, FALSE);

	// resync scrollbars
	PostResize(TRUE); 
}

void CTDLTaskCtrlBase::OnColumnVisibilityChange(const CTDCColumnIDMap& mapChanges)
{
	CTLSHoldResync hr(*this);
	CHoldRedraw hr2(m_lcColumns);
	
	int nNumCols = m_hdrColumns.GetItemCount();
	
	for (int nItem = 1; nItem < nNumCols; nItem++)
	{		
		m_hdrColumns.ShowItem(nItem, IsColumnShowing(GetColumnID(nItem)));
	}

	UpdateAttributePaneVisibility();

	if (mapChanges.Has(TDCC_ICON) || mapChanges.Has(TDCC_DONE))
		OnImageListChange();

	RecalcUntrackedColumnWidths(mapChanges);

	if (m_bAutoFitSplitter)
		AdjustSplitterToFitAttributeColumns();
	else
		PostResize(TRUE); // resync scrollbars
}

void CTDLTaskCtrlBase::UpdateAttributePaneVisibility()
{
	// we only need to find one visible column
	BOOL bShow = FALSE;
	int nItem = m_hdrColumns.GetItemCount();
	
	while (nItem-- && !bShow)
		bShow = IsColumnShowing(GetColumnID(nItem));
	
	if (bShow)
		SetHidden(TLSH_NONE);
	else
		SetHidden(IsLeft(m_lcColumns) ? TLSH_LEFT : TLSH_RIGHT);
}

void CTDLTaskCtrlBase::OnImageListChange()
{
	if (Tasks())
	{
		// Always force the task icon imagelist on the columns to ensure
		// the item height don't changes when hiding label icons on the Tasks
		ListView_SetImageList(m_lcColumns, m_ilTaskIcons, LVSIL_SMALL);

		SetTasksImageList(m_ilTaskIcons, FALSE, !IsColumnShowing(TDCC_ICON));
		SetTasksImageList(m_ilCheckboxes, TRUE, (!IsColumnShowing(TDCC_DONE) && HasStyle(TDCS_ALLOWTREEITEMCHECKBOX)));

		if (IsVisible())
			::InvalidateRect(Tasks(), NULL, FALSE);
	}
}

BOOL CTDLTaskCtrlBase::IsVisible() const
{
	HWND hwnd = GetSafeHwnd();

	return (hwnd && ::IsWindowVisible(::GetParent(hwnd)) && ::IsWindowVisible(hwnd));
}

void CTDLTaskCtrlBase::OnCustomAttributeChange()
{
	for (int nAttrib = 0; nAttrib < m_aCustomAttribDefs.GetSize(); nAttrib++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aCustomAttribDefs.GetData()[nAttrib];
		
		int nItem = GetColumnIndex(attribDef.GetColumnID());
		ASSERT(nItem != -1);

		m_hdrColumns.EnableItemTracking(nItem, attribDef.bEnabled);
		m_hdrColumns.ShowItem(nItem, attribDef.bEnabled);

		if (attribDef.bEnabled)
		{
			m_hdrColumns.SetItemText(nItem, attribDef.GetColumnTitle());
			m_hdrColumns.SetItemToolTip(nItem, attribDef.GetToolTip());

			LVCOLUMN lvc = { 0 };
			lvc.mask = LVCF_FMT;
			lvc.fmt = attribDef.GetColumnHeaderAlignment();

			m_lcColumns.SetColumn(nItem, &lvc);
		}
		else
		{
			m_hdrColumns.SetItemText(nItem, EMPTY_STR);
			m_hdrColumns.SetItemToolTip(nItem, EMPTY_STR);
		}
	}

	UpdateAttributePaneVisibility();
	RecalcUntrackedColumnWidths(TRUE); // Only custom columns

	if (m_bAutoFitSplitter)
		AdjustSplitterToFitAttributeColumns();
	else
		PostResize(TRUE); // resync horizontal scrollbars
}

BOOL CTDLTaskCtrlBase::IsColumnShowing(TDC_COLUMN nColID) const
{
	// Some columns are always visible
	if (nColID == TDCC_CLIENT)
	{
		return TRUE;
	}
	else if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nColID))
	{
		return m_aCustomAttribDefs.IsColumnEnabled(nColID);
	}

	return m_mapVisibleCols.Has(nColID);
}

int CTDLTaskCtrlBase::GetColumnIndex(TDC_COLUMN nColID) const
{
	int nItem = m_hdrColumns.FindItem(nColID);

	if (nItem == -1)
	{
		// Some fallbacks
		switch (nColID)
		{
		case TDCC_STARTTIME:	nItem = m_hdrColumns.FindItem(TDCC_STARTDATE);		break;
		case TDCC_DUETIME:		nItem = m_hdrColumns.FindItem(TDCC_DUEDATE);		break;
		case TDCC_DONETIME:		nItem = m_hdrColumns.FindItem(TDCC_DONEDATE);		break;
		case TDCC_CREATIONTIME: nItem = m_hdrColumns.FindItem(TDCC_CREATIONDATE);	break;
		}
	}

	ASSERT(nItem != -1);
	return nItem;
}

BOOL CTDLTaskCtrlBase::CanCopyTaskColumnValues(TDC_COLUMN nColID, BOOL bSelectedTasksOnly) const
{
	switch (nColID)
	{
	case TDCC_NONE:
		return FALSE;

	case TDCC_PRIORITY:
	case TDCC_PERCENT:
	case TDCC_TIMEEST:
	case TDCC_TIMESPENT:
	case TDCC_STARTDATE:
	case TDCC_DUEDATE:
	case TDCC_DONEDATE:
	case TDCC_ALLOCTO:
	case TDCC_ALLOCBY:
	case TDCC_STATUS:
	case TDCC_CATEGORY:
	case TDCC_FILEREF:
	case TDCC_POSITION:
	case TDCC_ID:
	case TDCC_CREATIONDATE:
	case TDCC_CREATEDBY:
	case TDCC_LASTMODDATE:
	case TDCC_RISK:
	case TDCC_EXTERNALID:
	case TDCC_COST:
	case TDCC_DEPENDENCY:
	case TDCC_RECURRENCE:
	case TDCC_VERSION:
	case TDCC_REMAINING:
	case TDCC_REMINDER:
	case TDCC_PARENTID:
	case TDCC_PATH:
	case TDCC_TAGS:
	case TDCC_SUBTASKDONE:
	case TDCC_STARTTIME:
	case TDCC_DUETIME:
	case TDCC_DONETIME:
	case TDCC_CREATIONTIME:
	case TDCC_LASTMODBY:
	case TDCC_COMMENTSSIZE:
	case TDCC_CLIENT:
		break;

	case TDCC_ICON:
	case TDCC_RECENTEDIT:
	case TDCC_LOCK:
	case TDCC_COLOR:
	case TDCC_DONE:
	case TDCC_TRACKTIME:
	case TDCC_FLAG:
		return FALSE;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nColID))
		{
			TDCCUSTOMATTRIBUTEDEFINITION attribDef;
			VERIFY(m_aCustomAttribDefs.GetAttributeDef(nColID, attribDef));

			switch (attribDef.GetAttributeType())
			{
			case TDCCA_BOOL:
			case TDCCA_ICON:
				return FALSE;
			}
		}
		else
		{
			// Missed values
			ASSERT(0);
			return FALSE;
		}
		break;
	}

	return (bSelectedTasksOnly ? HasSelection() : m_lcColumns.GetItemCount());
}

int CTDLTaskCtrlBase::CopyTaskColumnValues(TDC_COLUMN nColID, BOOL bSelectedTasksOnly, CStringArray& aValues) const
{
	if (!CanCopyTaskColumnValues(nColID, bSelectedTasksOnly))
		return 0;

	CDWordArray aTaskIDs;

	if (bSelectedTasksOnly)
	{
		GetSelectedTaskIDs(aTaskIDs, FALSE);
	}
	else
	{
		int nItem = m_lcColumns.GetItemCount();
		aTaskIDs.SetSize(nItem);

		while (nItem--)
			aTaskIDs[nItem] = GetColumnItemTaskID(nItem);
	}

	int nItem = aTaskIDs.GetSize();
	ASSERT(nItem);
	
	aValues.SetSize(nItem);

	while (nItem--)
	{
		const TODOITEM* pTDI = NULL;
		const TODOSTRUCTURE* pTDS = NULL;

		DWORD dwTaskID = aTaskIDs[nItem];
		VERIFY(m_data.GetTrueTask(dwTaskID, pTDI, pTDS));

		aValues[nItem] = GetTaskColumnText(dwTaskID, pTDI, pTDS, nColID, FALSE);
	}

	return aValues.GetSize();
}

CString CTDLTaskCtrlBase::GetColumnName(TDC_COLUMN nColID) const
{
	if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nColID))
	{
		TDCCUSTOMATTRIBUTEDEFINITION attribDef;
		VERIFY(m_aCustomAttribDefs.GetAttributeDef(nColID, attribDef));

		return attribDef.sLabel;
	}

	// else
	const TDCCOLUMN* pCol = GetColumn(nColID);
	ASSERT(pCol);

	return (pCol ? CEnString(pCol->nIDLongName) : EMPTY_STR);
}

BOOL CTDLTaskCtrlBase::SetColumnOrder(const CDWordArray& aColumns)
{
	CIntArray aOrder;
	aOrder.SetSize(aColumns.GetSize() + 1);

	// hidden column is always first
	aOrder[0] = 0;
	
	// convert columns IDs to indices
	int nNumCols = aColumns.GetSize();
	
	for (int nCol = 0; nCol < nNumCols; nCol++)
	{		
		int nItem = GetColumnIndex((TDC_COLUMN)aColumns[nCol]);
		ASSERT(nItem != -1);

		aOrder[nCol + 1] = nItem;
	}
	
	return m_lcColumns.SetColumnOrderArray(aOrder.GetSize(), aOrder.GetData());
}

BOOL CTDLTaskCtrlBase::GetColumnOrder(CDWordArray& aColumnIDs) const
{
	CIntArray aOrder;
	int nNumCols = m_hdrColumns.GetItemOrder(aOrder);

	if (nNumCols)
	{
		// ignore first column because that's our dummy column
		aColumnIDs.SetSize(nNumCols - 1); 

		for (int nItem = 1; nItem < nNumCols; nItem++)
			aColumnIDs[nItem - 1] = GetColumnID(aOrder[nItem]);
	
		return TRUE;
	}
	
	return FALSE;
}

void CTDLTaskCtrlBase::SetColumnWidths(const CDWordArray& aWidths)
{
	int nNumCols = aWidths.GetSize();
	
	// omit first column because that's our dummy column
	for (int nCol = 0; nCol < nNumCols; nCol++)
		m_hdrColumns.SetItemWidth(nCol + 1, aWidths[nCol]);
}

void CTDLTaskCtrlBase::GetColumnWidths(CDWordArray& aWidths) const
{
	CIntArray aIntWidths;
	int nNumCols = m_hdrColumns.GetItemWidths(aIntWidths);

	// omit first column because that's our dummy column
	aWidths.SetSize(nNumCols - 1);
	
	for (int nCol = 1; nCol < nNumCols; nCol++)
		aWidths[nCol - 1] = aIntWidths[nCol];
}

void CTDLTaskCtrlBase::SetTrackedColumns(const CDWordArray& aTracked)
{
	int nNumCols = aTracked.GetSize();
	
	// omit first column because that's our dummy column
	for (int nCol = 0; nCol < nNumCols; nCol++)
		m_hdrColumns.SetItemTracked(nCol + 1, (int)aTracked[nCol]);
}

void CTDLTaskCtrlBase::GetTrackedColumns(CDWordArray& aTracked) const
{
	CIntArray aIntTracked;
	int nNumCols = m_hdrColumns.GetTrackedItems(aIntTracked);

	// omit first column because that's our dummy column
	aTracked.SetSize(nNumCols - 1);
	
	for (int nCol = 1; nCol < nNumCols; nCol++)
		aTracked[nCol - 1] = aIntTracked[nCol];
}

BOOL CTDLTaskCtrlBase::BuildColumns()
{
	if (m_hdrColumns.GetItemCount())
		return FALSE;

	// Create imagelist for columns using symbols
	ASSERT(m_ilColSymbols.GetSafeHandle() == NULL);

	if (!m_ilColSymbols.Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 1))
		return FALSE;

	CBitmap bmp;

	if (!bmp.LoadBitmap(IDB_COLUMN_SYMBOLS) || (m_ilColSymbols.Add(&bmp, RGB(255, 0, 255)) == -1))
		return FALSE;
	
	m_ilColSymbols.ScaleByDPIFactor(RGB(253, 253, 253));

	// primary header
	const TDCCOLUMN* pClient = GetColumn(TDCC_CLIENT);
	ASSERT(pClient);
		
	// add empty column as placeholder so we can easily replace the 
	// other columns without losing all our items too
	m_lcColumns.InsertColumn(0, EMPTY_STR);
	m_hdrColumns.ShowItem(0, FALSE);
	m_hdrColumns.SetItemWidth(0, 0);
	
	// add all columns in two stages because m_lcColumns 
	// doesn't immediately update the header control
	int nCol = 0;
	
	for (nCol = 0; nCol < NUM_COLUMNS; nCol++)
	{
		const TDCCOLUMN& tdcc = COLUMNS[nCol];
		
		if (tdcc.nColID != TDCC_CLIENT)
		{
			m_lcColumns.InsertColumn((nCol + 1), CEnString(tdcc.nIDName), tdcc.GetColumnHeaderAlignment(), 100);
		}
		else
		{
			ASSERT(!m_hdrTasks.GetItemCount());

			m_hdrTasks.AppendItem(150, CEnString(pClient->nIDName), HDF_LEFT);
			m_hdrTasks.SetItemData(0, TDCC_CLIENT);
			m_hdrTasks.EnableItemTracking(0, FALSE); // always
		}
	}
	
	// Add column IDs to header
	for (nCol = 0; nCol < NUM_COLUMNS; nCol++)
	{
		const TDCCOLUMN& tdcc = COLUMNS[nCol];
		
		if (tdcc.nColID != TDCC_CLIENT)
		{
			int nItem = (nCol + 1); // zero'th column ignored

			m_hdrColumns.SetItemData(nItem, tdcc.nColID);
			m_hdrColumns.SetItemToolTip(nItem, CEnString(tdcc.nIDLongName));
		}
	}
	
	// add custom columns
	int nNumCols = (TDCC_CUSTOMCOLUMN_LAST - TDCC_CUSTOMCOLUMN_FIRST + 1);

	for (nCol = 0; nCol < nNumCols; nCol++)
	{
		m_lcColumns.InsertColumn((NUM_COLUMNS + nCol), EMPTY_STR, LVCFMT_LEFT, 0);
	}

	// and their IDs
	for (nCol = 0; nCol < nNumCols; nCol++)
	{
		int nItem = (NUM_COLUMNS + nCol);

		m_hdrColumns.SetItemData(nItem, (TDCC_CUSTOMCOLUMN_FIRST + nCol));
		m_hdrColumns.EnableItemTracking(nItem, FALSE);
		m_hdrColumns.ShowItem(nItem, TRUE);
	}

	RecalcUntrackedColumnWidths();

	return TRUE;
}

void CTDLTaskCtrlBase::EnableRecalcColumns(BOOL bEnable)
{
	if ((bEnable && !m_bEnableRecalcColumns) || (!bEnable && m_bEnableRecalcColumns))
	{
		m_bEnableRecalcColumns = bEnable;

		if (bEnable)
			RecalcUntrackedColumnWidths();
	}
}

void CTDLTaskCtrlBase::RecalcAllColumnWidths()
{
	ASSERT(m_bEnableRecalcColumns);

	m_hdrColumns.ClearAllTracked();

	CWaitCursor cursor;
	RecalcUntrackedColumnWidths();
}

void CTDLTaskCtrlBase::RecalcUntrackedColumnWidths()
{
	if (m_bEnableRecalcColumns)
		RecalcUntrackedColumnWidths(FALSE); // Standard and Custom cols
}

void CTDLTaskCtrlBase::RecalcUntrackedColumnWidths(BOOL bCustomOnly)
{
	// Get a list of all the visible column attributes
	CTDCColumnIDMap mapCols;

	if (!bCustomOnly)
		mapCols.Copy(m_mapVisibleCols);
	
	m_aCustomAttribDefs.GetVisibleColumnIDs(mapCols, TRUE); // append

	RecalcUntrackedColumnWidths(mapCols, TRUE, bCustomOnly);
}

void CTDLTaskCtrlBase::RecalcUntrackedColumnWidths(const CTDCColumnIDMap& aColIDs, BOOL bZeroOthers, BOOL bCustomOnly)
{
	if (!bZeroOthers && !aColIDs.GetCount())
		return;

	// Weed out all the tracked columns
	CTDCColumnIDMap mapCols(aColIDs);
	int nNumCols = m_hdrColumns.GetItemCount();

	for (int nItem = 1; nItem < nNumCols; nItem++)
	{
		if (m_hdrColumns.IsItemTracked(nItem))
		{
			ASSERT(m_hdrColumns.IsItemVisible(nItem));
			mapCols.RemoveKey(GetColumnID(nItem));
		}
	}

	if (!bZeroOthers && !mapCols.GetCount())
		return;

	CHoldRedraw hr(m_lcColumns);
	CClientDC dc(&m_lcColumns);

	int nPrevTotalWidth = m_hdrColumns.CalcTotalItemWidth();

	CFont* pOldFont = GraphicsMisc::PrepareDCFont(&dc, m_lcColumns);
	BOOL bVisibleTasksOnly = IsTreeList();

	// Optimise for single columns
	if (!bZeroOthers && (mapCols.GetCount() == 1))
	{
		int nCol = GetColumnIndex(mapCols.GetFirst());
		ASSERT(nCol != -1);

		int nColWidth = CalcColumnWidth(nCol, &dc, bVisibleTasksOnly);
		m_hdrColumns.SetItemWidth(nCol, nColWidth);
	}
	else
	{
		// Get the longest task values for the remaining attributes
		CTDCLongestItemMap mapLongest;
		m_find.GetLongestValues(mapCols, m_aCustomAttribDefs, mapLongest, bVisibleTasksOnly);

		m_hdrColumns.SetItemWidth(0, 0); // always

		for (int nItem = 1; nItem < nNumCols; nItem++)
		{
			TDC_COLUMN nColID = GetColumnID(nItem);
			int nColWidth = 0;

			if (mapCols.Has(nColID))
			{
				if (mapLongest.HasColumn(nColID))
				{
					CString sLongest = mapLongest.GetLongestValue(nColID);

					if (sLongest.GetLength())
						nColWidth = GraphicsMisc::GetAverageMaxStringWidth(sLongest, &dc);

					if (nColWidth == 0)
						nColWidth = MIN_RESIZE_WIDTH;
					else
						nColWidth = max((nColWidth + (2 * LV_COLPADDING)), MIN_RESIZE_WIDTH);

					// take max of this and column title
					int nTitleWidth = (m_hdrColumns.GetItemTextWidth(nItem, &dc) + (2 * HD_COLPADDING));

					nColWidth = max(nTitleWidth, nColWidth);
				}
				else
				{
					nColWidth = CalcColumnWidth(nItem, &dc, bVisibleTasksOnly);
				}
			}
			else if (!bZeroOthers || m_hdrColumns.IsItemTracked(nItem))
			{
				continue;
			}
			else if (bCustomOnly && !TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nColID))
			{
				continue;
			}

			m_hdrColumns.SetItemWidth(nItem, nColWidth);
		}
	}

	// cleanup
	dc.SelectObject(pOldFont);

	// Resync horizontal scrollbars
	if (m_hdrColumns.CalcTotalItemWidth() != nPrevTotalWidth)
		PostResize();
}

void CTDLTaskCtrlBase::SaveState(CPreferences& prefs, const CString& sKey) const
{
	ASSERT (GetSafeHwnd());
	ASSERT (!sKey.IsEmpty());
	
	CDWordArray aOrder, aWidths, aTracked;

	GetColumnOrder(aOrder);
	GetColumnWidths(aWidths);
	GetTrackedColumns(aTracked);

	prefs.WriteProfileArray((sKey + _T("\\ColumnOrder")), aOrder);
	prefs.WriteProfileArray((sKey + _T("\\ColumnWidth")), aWidths);
	prefs.WriteProfileArray((sKey + _T("\\ColumnTracked")), aTracked);

	if (!m_bAutoFitSplitter)
		prefs.WriteProfileInt(sKey, _T("SplitPos"), GetSplitPos());
	else
		prefs.DeleteProfileEntry(sKey, _T("SplitPos"));
	
	m_sort.SaveState(prefs, sKey);
}

void CTDLTaskCtrlBase::LoadState(const CPreferences& prefs, const CString& sKey)
{
	ASSERT (GetSafeHwnd());
	ASSERT (!sKey.IsEmpty());

	// make sure columns are configured right
	OnColumnVisibilityChange(CTDCColumnIDMap());
	
	// load column customisations
	CDWordArray aOrder, aWidths, aTracked;
	
	if (prefs.GetProfileArray((sKey + _T("\\ColumnOrder")), aOrder))
		SetColumnOrder(aOrder);

	if (prefs.GetProfileArray((sKey + _T("\\ColumnWidth")), aWidths))
		SetColumnWidths(aWidths);
	
	if (prefs.GetProfileArray((sKey + _T("\\ColumnTracked")), aTracked))
		SetTrackedColumns(aTracked);

	int nSplitPos = prefs.GetProfileInt(sKey, _T("SplitPos"), -1);

	if (nSplitPos > 0)
	{
		m_bAutoFitSplitter = FALSE;
		SetSplitPos(nSplitPos);
	}

	m_sort.LoadState(prefs, sKey);

	PostResize();
}

int CALLBACK CTDLTaskCtrlBase::SortFuncMulti(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	TDSORTPARAMS* pSS = (TDSORTPARAMS*)lParamSort;
	ASSERT (pSS && pSS->sort.bMulti && pSS->sort.multi.IsSorting());

	int nCompare = 0;
	const TDSORTCOLUMN* pCols = pSS->sort.multi.Cols();
	
	for (int nCol = 0; ((nCol < 3) && (nCompare == 0)); nCol++)
	{
		if (!pCols[nCol].IsSorting())
			break;

		nCompare = CompareTasks(lParam1, lParam2, 
								pSS->base, 
								pCols[nCol], 
								pSS->flags);
	}

	// finally, if the items are equal we sort by raw
	// position so that the sort is stable
	if (nCompare == 0)
	{
		static TDSORTCOLUMN nullCol(TDCC_NONE, FALSE);
		static TDSORTFLAGS nullFlags;

		nCompare = CompareTasks(lParam1, lParam2, pSS->base, nullCol, nullFlags);
	}
	
	return nCompare;
}

int CALLBACK CTDLTaskCtrlBase::SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	TDSORTPARAMS* pSS = (TDSORTPARAMS*)lParamSort;
	
	ASSERT (!pSS->sort.bMulti);
	
	int nCompare = CompareTasks(lParam1, lParam2, 
								pSS->base, 
								pSS->sort.single, 
								pSS->flags);
	
	// finally, if the items are equal we sort by raw
	// position so that the sort is stable
	if (nCompare == 0)
	{
		static TDSORTCOLUMN nullCol(TDCC_NONE, FALSE);
		static TDSORTFLAGS nullFlags;

		nCompare = CompareTasks(lParam1, lParam2, pSS->base, nullCol, nullFlags);
	}
	
	return nCompare;
}

int CTDLTaskCtrlBase::CompareTasks(LPARAM lParam1, 
									LPARAM lParam2, 
									const CTDLTaskCtrlBase& base, 
									const TDSORTCOLUMN& sort, 
									const TDSORTFLAGS& flags)
{
	ASSERT(sort.bAscending != -1);

	DWORD dwTaskID1 = lParam1, dwTaskID2 = lParam2;
	
	// special cases first
	if (sort.IsSortingBy(TDCC_TRACKTIME))
	{
		BOOL bTracked1 = ((dwTaskID1 == flags.dwTimeTrackID) ? 1 : 0);
		BOOL bTracked2 = ((dwTaskID2 == flags.dwTimeTrackID) ? 1 : 0);
		ASSERT(!(bTracked1 && bTracked2));
		
		return (sort.bAscending ? (bTracked1 - bTracked2) : (bTracked2 - bTracked1));
	}
	else if (sort.IsSortingBy(TDCC_REMINDER))
	{
		COleDateTime dtRem1, dtRem2;

		BOOL bHasReminder1 = base.GetTaskReminder(dwTaskID1, dtRem1);
		BOOL bHasReminder2 = base.GetTaskReminder(dwTaskID2, dtRem2);

		int nCompare = 0;

		if (bHasReminder1 && bHasReminder2)
		{
			nCompare = ((dtRem1 < dtRem2) ? -1 : (dtRem1 > dtRem2) ? 1 : 0);
		}
		else if (bHasReminder1)
		{
			nCompare = 1;
		}
		else if (bHasReminder2)
		{
			nCompare = -1;
		}
		
		return (sort.bAscending ? nCompare : -nCompare);
	}
	else if (sort.IsSortingByCustom())
	{
		TDCCUSTOMATTRIBUTEDEFINITION attribDef;
		
		// this can still fail
		if (!base.m_aCustomAttribDefs.GetAttributeDef(sort.nBy, attribDef))
			return 0;
		
		return base.m_comparer.CompareTasks(dwTaskID1, dwTaskID2, attribDef, sort.bAscending);
	}
	
	// else default attribute
	return base.m_comparer.CompareTasks(dwTaskID1, 
										dwTaskID2, 
										sort.nBy, 
										sort.bAscending, 
										flags.bSortDueTodayHigh,
										flags.WantIncludeTime(sort.nBy));
}

DWORD CTDLTaskCtrlBase::HitTestTask(const CPoint& ptScreen, BOOL bTitleColumnOnly) const
{
	DWORD dwTaskID = HitTestTasksTask(ptScreen);
	
	if (!dwTaskID && !bTitleColumnOnly)
		dwTaskID = HitTestColumnsTask(ptScreen);

	return dwTaskID;
}

int CTDLTaskCtrlBase::HitTestColumnsItem(const CPoint& pt, BOOL bClient, TDC_COLUMN& nColID, DWORD* pTaskID, LPRECT pRect) const
{
	LVHITTESTINFO lvHit = { 0 };
	lvHit.pt = pt;

	if (!bClient)
		m_lcColumns.ScreenToClient(&lvHit.pt);
	
	ListView_SubItemHitTest(m_lcColumns, &lvHit);

	if ((lvHit.iItem < 0) || (lvHit.iSubItem < 0))
		return -1;

	nColID = GetColumnID(lvHit.iSubItem);
	ASSERT(nColID != TDCC_NONE);

	if (pTaskID)
	{
		*pTaskID = GetColumnItemTaskID(lvHit.iItem);
		ASSERT(*pTaskID);
	}

	if (pRect)
	{
		ListView_GetSubItemRect(m_lcColumns, lvHit.iItem, lvHit.iSubItem, LVIR_BOUNDS, pRect);
	}
		
	return lvHit.iItem;
}

int CTDLTaskCtrlBase::HitTestFileLinkColumn(const CPoint& ptScreen) const
{
	TDC_COLUMN nColID = TDCC_NONE;
	DWORD dwTaskID = 0;
	CRect rSubItem;
	
	if (HitTestColumnsItem(ptScreen, FALSE, nColID, &dwTaskID, &rSubItem) == -1)
	{
		ASSERT(0);
		return -1;
	}
	ASSERT(nColID == TDCC_FILEREF);

	const TODOITEM* pTDI = m_data.GetTrueTask(dwTaskID);
	
	if (!pTDI)
	{
		ASSERT(0);
		return -1;
	}
	
	int nNumFiles = pTDI->aFileLinks.GetSize();
	
	if (nNumFiles == 1)
	{
		return 0;
	}
	else
	{
		CPoint ptList(ptScreen);
		m_lcColumns.ScreenToClient(&ptList);
		
		for (int nFile = 0; nFile < nNumFiles; nFile++)
		{
			CRect rIcon;
			
			if (!CalcFileIconRect(rSubItem, rIcon, nFile, nNumFiles))
				break;

			if (rIcon.PtInRect(ptList))
				return nFile;
		}
	}

	return -1;
}

TDC_HITTEST CTDLTaskCtrlBase::HitTest(const CPoint& ptScreen) const
{
	if (PtInClientRect(ptScreen, m_hdrTasks, TRUE)) // task header
	{
		return TDCHT_COLUMNHEADER;
	}
	else if (PtInClientRect(ptScreen, m_hdrColumns, TRUE))	// column header
	{
		return TDCHT_COLUMNHEADER;
	}
	else if (PtInClientRect(ptScreen, Tasks(), TRUE))
	{
		// see if we hit a task
		if (HitTestTasksTask(ptScreen))
		{
			return TDCHT_TASK;
		}
		else if (PtInClientRect(ptScreen, Tasks(), TRUE))
		{
			return TDCHT_TASKLIST;
		}
	}
	else if (PtInClientRect(ptScreen, m_lcColumns, TRUE))
	{
		// see if we hit a task
		if (HitTestColumnsTask(ptScreen))
		{
			return TDCHT_TASK;
		}
		else if (PtInClientRect(ptScreen, m_lcColumns, TRUE))
		{
			return TDCHT_TASKLIST;
		}
	}
	
	// all else
	return TDCHT_NOWHERE;
}

DWORD CTDLTaskCtrlBase::HitTestColumnsTask(const CPoint& ptScreen) const
{
	// see if we hit a task in the list
	CPoint ptClient(ptScreen);
	m_lcColumns.ScreenToClient(&ptClient);
	
	int nItem = m_lcColumns.HitTest(ptClient);

	if (nItem != -1)
		return GetColumnItemTaskID(nItem);

	// all else
	return 0;
}

TDC_COLUMN CTDLTaskCtrlBase::HitTestColumn(const CPoint& ptScreen) const
{
	if (PtInClientRect(ptScreen, m_hdrTasks, TRUE) || // tree header
		PtInClientRect(ptScreen, Tasks(), TRUE)) // tree
	{
		return TDCC_CLIENT;
	}
	else if (PtInClientRect(ptScreen, m_hdrColumns, TRUE))	// column header
	{
		CPoint ptHeader(ptScreen);
		m_hdrColumns.ScreenToClient(&ptHeader);
		
		int nCol = m_hdrColumns.HitTest(ptHeader);
		
		if (nCol >= 0)
			return GetColumnID(nCol);
	}
	else if (PtInClientRect(ptScreen, m_lcColumns, TRUE)) // columns
	{
		TDC_COLUMN nColID = TDCC_NONE;
		
		if (HitTestColumnsItem(ptScreen, FALSE, nColID) != -1)
			return nColID;
	}

	// else
	return TDCC_NONE;
}

BOOL CTDLTaskCtrlBase::PtInClientRect(POINT point, HWND hWnd, BOOL bScreenCoords)
{
	CRect rect;
	::GetClientRect(hWnd, rect);
	
	if (bScreenCoords)
	{
		::ClientToScreen(hWnd, &rect.TopLeft());
		::ClientToScreen(hWnd, &rect.BottomRight());
	}
	
	return rect.PtInRect(point);
}

void CTDLTaskCtrlBase::Release() 
{ 
	m_imageIcons.Clear();
	m_fonts.Release();
	m_ilCheckboxes.DeleteImageList();
	
	GraphicsMisc::VerifyDeleteObject(m_brDue);
	GraphicsMisc::VerifyDeleteObject(m_brDueToday);

	if (::IsWindow(m_hdrColumns))
		m_hdrColumns.UnsubclassWindow();
	
	Unsync(); 
}

BOOL CTDLTaskCtrlBase::SetFont(HFONT hFont)
{	
	ASSERT(Tasks() != NULL);
	ASSERT(hFont);

	HFONT hTaskFont = GetFont();
	BOOL bChange = !GraphicsMisc::SameFontNameSize(hFont, hTaskFont);
	
	if (bChange)
	{
		m_fonts.Clear();

		CHoldRedraw hr(*this);
		::SendMessage(Tasks(), WM_SETFONT, (WPARAM)hFont, TRUE);

		RecalcUntrackedColumnWidths();
		m_dateTimeWidths.ResetWidths();
	}
	
	return bChange;
}

HFONT CTDLTaskCtrlBase::GetFont() const
{
	return (HFONT)::SendMessage(m_lcColumns, WM_GETFONT, 0, 0);
}

BOOL CTDLTaskCtrlBase::IsAlternateColumnLine(int nItem) const
{
	return (HasColor(m_crAltLine) && ((nItem % 2) == 0));
}

BOOL CTDLTaskCtrlBase::IsSorting() const
{
	return m_sort.IsSorting();
}

BOOL CTDLTaskCtrlBase::IsSortingBy(TDC_COLUMN nSortBy) const
{
	return (m_sort.IsSortingBy(nSortBy, TRUE));
}

void CTDLTaskCtrlBase::Resort(BOOL bAllowToggle) 
{ 
	if (IsMultiSorting())
	{
		TDSORTCOLUMNS sort;

		GetSortBy(sort);
		MultiSort(sort);
	}
	else
	{
		Sort(GetSortBy(), bAllowToggle); 
	}
}

BOOL CTDLTaskCtrlBase::IsMultiSorting() const
{
	return (m_sort.bMulti && m_sort.multi.IsSorting());
}

void CTDLTaskCtrlBase::Sort(TDC_COLUMN nBy, BOOL bAllowToggle)
{
	// special case
	if (nBy == TDCC_NONE && !m_sort.IsSorting())
		return; // nothing to do

	BOOL bAscending = m_sort.single.bAscending;

	if (nBy != TDCC_NONE)
	{
		// first time?
		if ((bAscending == -1) || !m_sort.single.IsSortingBy(nBy))
		{
			const TDCCOLUMN* pTDCC = GetColumn(nBy);

			if (pTDCC)
			{
				bAscending = pTDCC->bSortAscending;
			}
			else if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nBy))
			{
				// TODO
				bAscending = FALSE;//(m_ctrlTreeList.Tree().GetGutterColumnSort(nBy) != NCGSORT_DOWN);
			}
		}
		// if there's been a mod since last sorting then its reasonable to assume
		// that the user is not toggling direction but wants to simply resort
		// in the same direction, otherwise we toggle the direction.
		else if (!m_sort.bModSinceLastSort && bAllowToggle)
		{
			bAscending = !bAscending; // toggle 
		}
		
		// update the column header
		SetSortColumn(nBy, (bAscending ? TDC_SORTUP : TDC_SORTDOWN));
	}
	else
	{
		ClearSortColumn();
		bAscending = TRUE;
	}

	m_sort.SetSortBy(nBy, bAscending);
	m_sort.bModSinceLastSort = FALSE;
	
	if (m_data.GetTaskCount() > 1)
		DoSort();
}

PFNTLSCOMPARE CTDLTaskCtrlBase::PrepareSort(TDSORTPARAMS& ss) const
{
	ss.sort = m_sort;
	ss.flags.bSortChildren = TRUE;
	ss.flags.bSortDueTodayHigh = HasColor(m_crDueToday);
	ss.flags.dwTimeTrackID = m_dwTimeTrackTaskID;
	ss.flags.bIncCreateTime = IsColumnShowing(TDCC_CREATIONTIME);
	ss.flags.bIncStartTime = IsColumnShowing(TDCC_STARTTIME);
	ss.flags.bIncDueTime = IsColumnShowing(TDCC_DUETIME);
	ss.flags.bIncDoneTime = IsColumnShowing(TDCC_DONETIME);
	
	return (ss.sort.bMulti ? SortFuncMulti : SortFunc);
}

void CTDLTaskCtrlBase::DoSort()
{
	// Scope the hold to have finished before resyncing
	{
		CHoldListVScroll hold(m_lcColumns);

		TDSORTPARAMS ss(*this);
		CTreeListSyncer::Sort(PrepareSort(ss), (LPARAM)&ss);

		ResyncSelection(m_lcColumns, Tasks(), FALSE);
	}

	ResyncScrollPos(Tasks(), m_lcColumns);
	EnsureSelectionVisible();
}

void CTDLTaskCtrlBase::GetSortBy(TDSORTCOLUMNS& sort) const
{
	sort = m_sort.multi;

	// initialise multisort if first time
	if (!sort.IsSorting())
		sort.SetSortBy(0, m_sort.single.nBy, (m_sort.single.bAscending ? TRUE : FALSE));
}

void CTDLTaskCtrlBase::MultiSort(const TDSORTCOLUMNS& sort)
{
	if (!sort.IsSorting())
	{
		ASSERT(0);
		return;
	}

	m_sort.SetSortBy(sort);
	m_sort.bModSinceLastSort = FALSE;

	if (m_data.GetTaskCount() > 1)
		DoSort();

	// clear sort direction on header
	ClearSortColumn();
}

void CTDLTaskCtrlBase::Resize(const CRect& rect)
{
	if (GetSafeHwnd())
		MoveWindow(rect);
	else
		CTreeListSyncer::Resize(rect);
}

POSITION CTDLTaskCtrlBase::GetFirstSelectedTaskPos() const
{
	return m_lcColumns.GetFirstSelectedItemPosition();
}

DWORD CTDLTaskCtrlBase::GetNextSelectedTaskID(POSITION& pos) const
{
	if (pos == NULL)
		return 0;

	int nSel = m_lcColumns.GetNextSelectedItem(pos);

	if (nSel == -1)
		return 0;

	return GetColumnItemTaskID(nSel);
}

void CTDLTaskCtrlBase::OnNotifySplitterChange(int /*nSplitPos*/)
{
	if (IsSplitting())
		m_bAutoFitSplitter = FALSE;

	InvalidateAll(TRUE);
}

void CTDLTaskCtrlBase::DrawSplitBar(CDC* pDC, const CRect& rSplitter, COLORREF crSplitBar)
{
	GraphicsMisc::DrawSplitBar(pDC, rSplitter, crSplitBar);
}

BOOL CTDLTaskCtrlBase::GetTaskTextColors(DWORD dwTaskID, COLORREF& crText, COLORREF& crBack, BOOL bRef) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;

	DWORD dwOrgTaskID(dwTaskID);
	
	if (!m_data.GetTrueTask(dwTaskID, pTDI, pTDS))
		return FALSE;
	
	// else
	if (bRef == -1)
		bRef = (dwTaskID != dwOrgTaskID);

	return GetTaskTextColors(pTDI, pTDS, crText, crBack, bRef, FALSE);
}

BOOL CTDLTaskCtrlBase::GetTaskTextColors(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS,
										COLORREF& crText, COLORREF& crBack, BOOL bRef) const
{
	return GetTaskTextColors(pTDI, pTDS, crText, crBack, bRef, FALSE);
}

BOOL CTDLTaskCtrlBase::GetTaskTextColors(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, COLORREF& crText, 
										COLORREF& crBack, BOOL bRef, BOOL bSelected) const
{
	ASSERT(pTDI && pTDS);

	if (!(pTDI && pTDS))
		return FALSE;

	if (bRef == -1)
	{
		bRef = pTDI->IsReference();

		if (bRef)
		{
			DWORD dwTaskID(pTDI->dwTaskRefID);
			m_data.GetTrueTask(dwTaskID, pTDI, pTDS);
		}
	}

	// all else
	crBack = CLR_NONE;
	crText = GetSysColor(COLOR_WINDOWTEXT);

	BOOL bDone = m_calculator.IsTaskDone(pTDI, pTDS);

	if (bDone)
	{
		if (HasColor(m_crDone))
		{
			crText = m_crDone; // parent and/or item is done
		}
		else if (bRef && HasColor(m_crReference))
		{
			crText = m_crReference;
		}
		else
		{
			crText = pTDI->color; 
		}
	}
	else // all incomplete tasks
	{
		do
		{
			// if it's a ref task just return the ref colour
			if (bRef && HasColor(m_crReference))
			{
				crText = m_crReference;
				break;
			}

			// else
			BOOL bDueToday = m_calculator.IsTaskDueToday(pTDI, pTDS);
			BOOL bOverDue = m_calculator.IsTaskOverDue(pTDI, pTDS);

			// overdue takes priority
			if (HasColor(m_crDue) && bOverDue)
			{
				crText = m_crDue;
				break;
			}
			else if (HasColor(m_crDueToday) && bDueToday)
			{
				crText = m_crDueToday;
				break;
			}

			// started 'by now' takes priority
			if (HasColor(m_crStarted) && m_calculator.IsTaskStarted(pTDI, pTDS))
			{
				crText = m_crStarted;
				break;
			}
			else if (HasColor(m_crStartedToday) && m_calculator.IsTaskStarted(pTDI, pTDS, TRUE))
			{
				crText = m_crStartedToday;
				break;
			}

			// else
			if (HasColor(m_crFlagged) && pTDI->bFlagged)
			{
				crText = m_crFlagged;
				break;
			}

			if (HasStyle(TDCS_COLORTEXTBYPRIORITY))
			{
				int nPriority = FM_NOPRIORITY;

				if (bDueToday)
				{
					nPriority = m_calculator.GetTaskHighestPriority(pTDI, pTDS, FALSE); // ignore due tasks
				}
				else if (bOverDue && HasStyle(TDCS_DUEHAVEHIGHESTPRIORITY))
				{
					nPriority = 10;
				}
				else
				{
					nPriority = m_calculator.GetTaskHighestPriority(pTDI, pTDS);
				}

				if (nPriority != FM_NOPRIORITY)
				{
					crText = GetPriorityColor(nPriority); 
					break;
				}
			}
			else if (HasStyle(TDCS_COLORTEXTBYATTRIBUTE))
			{
				switch (m_nColorByAttrib)
				{
				case TDCA_CATEGORY:
					GetAttributeColor(pTDI->GetCategory(0), crText);
					break;

				case TDCA_ALLOCBY:
					GetAttributeColor(pTDI->sAllocBy, crText);
					break;

				case TDCA_ALLOCTO:
					GetAttributeColor(pTDI->GetAllocTo(0), crText);
					break;

				case TDCA_STATUS:
					GetAttributeColor(pTDI->sStatus, crText);
					break;

				case TDCA_VERSION:
					GetAttributeColor(pTDI->sVersion, crText);
					break;

				case TDCA_EXTERNALID:
					GetAttributeColor(pTDI->sExternalID, crText);
					break;

				case TDCA_TAGS:
					GetAttributeColor(pTDI->GetTag(0), crText);
					break;

				default:
					ASSERT(0);
					break;
				}
			}
			else if (!HasStyle(TDCS_COLORTEXTBYNONE) && pTDI->color)
			{
				crText = pTDI->color; 
			}
		}
		while (false); // break
	}
	ASSERT(HasColor(crText));
	
	if (bSelected && !m_bSavingToImage)
	{
		crText = GraphicsMisc::GetExplorerItemSelectionTextColor(crText, GMIS_SELECTED, GMIB_THEMECLASSIC);
	}
	else if (HasStyle(TDCS_TASKCOLORISBACKGROUND) &&
			 !bSelected && !bDone && (crText != GetSysColor(COLOR_WINDOWTEXT)))
	{
		crBack = crText;
		crText = GraphicsMisc::GetBestTextColor(crBack);
	}

	return TRUE;
}

COLORREF CTDLTaskCtrlBase::GetTaskCommentsTextColor(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	COLORREF crText = COMMENTSCOLOR;

	if (HasColor(m_crDone) && m_calculator.IsTaskDone(pTDI, pTDS, TDCCHECKALL))
		crText = m_crDone;

	return crText;
}

BOOL CTDLTaskCtrlBase::SetPriorityColors(const CDWordArray& aColors)
{
	ASSERT (aColors.GetSize() == 11);
	
	if ((aColors.GetSize() == 11) && !Misc::MatchAllT(aColors, m_aPriorityColors, FALSE))
	{
		m_aPriorityColors.Copy(aColors);
			
		if (GetSafeHwnd())
			InvalidateAll();
			
		return TRUE;
	}
	
	// else
	return FALSE; // invalid combination or no change
}

BOOL CTDLTaskCtrlBase::SetStartedTaskColors(COLORREF crStarted, COLORREF crStartedToday)
{
	if ((m_crStarted != crStarted) || (m_crStartedToday != crStartedToday))
	{
		m_crStarted = crStarted;
		m_crStartedToday = crStartedToday;
		
		if (GetSafeHwnd())
			InvalidateAll();

		return TRUE;
	}

	// else no change
	return FALSE;
}

BOOL CTDLTaskCtrlBase::CheckUpdateDueBrushColor(COLORREF crNew, COLORREF& crCur, CBrush& brCur)
{
	if (crCur != crNew)
	{
		GraphicsMisc::VerifyDeleteObject(brCur);

		if (HasColor(crNew))
			brCur.CreateSolidBrush(crNew);

		crCur = crNew;
		return TRUE;
	}

	return FALSE;
}

BOOL CTDLTaskCtrlBase::SetDueTaskColors(COLORREF crDue, COLORREF crDueToday)
{
	BOOL bResort = (IsSortingBy(TDCC_PRIORITY) && HasStyle(TDCS_DUEHAVEHIGHESTPRIORITY) && (HasColor(crDueToday) != HasColor(m_crDueToday)));

	BOOL bChange = CheckUpdateDueBrushColor(crDue, m_crDue, m_brDue);
	bChange |= CheckUpdateDueBrushColor(crDueToday, m_crDueToday, m_brDueToday);

	if (bChange)
	{
		if (GetSafeHwnd())
		{
			if (bResort)
				Resort();
			else
				InvalidateAll();
		}

		return TRUE;
	}

	// else no change
	return FALSE;
}

BOOL CTDLTaskCtrlBase::SetCompletedTaskColor(COLORREF color)
{
	if (m_crDone != color)
	{
		m_crDone = color;
		
		if (GetSafeHwnd())
			InvalidateAll();

		return TRUE;
	}

	// else no change
	return FALSE;
}

BOOL CTDLTaskCtrlBase::SetFlaggedTaskColor(COLORREF color)
{
	if (m_crFlagged != color)
	{
		m_crFlagged = color;
		
		if (GetSafeHwnd())
			InvalidateAll();

		return TRUE;
	}

	// else no change
	return FALSE;
}

BOOL CTDLTaskCtrlBase::SetReferenceTaskColor(COLORREF color)
{
	if (m_crReference != color)
	{
		m_crReference = color;
			
		if (GetSafeHwnd())
			InvalidateAll();

		return TRUE;
	}

	// else no change
	return FALSE;
}

BOOL CTDLTaskCtrlBase::SetAttributeColors(TDC_ATTRIBUTE nAttrib, const CTDCColorMap& colors)
{
	// see if there is any change
	if ((m_nColorByAttrib == nAttrib) && m_mapAttribColors.MatchAll(colors))
	{
		return FALSE; // no change
	}

	m_nColorByAttrib = nAttrib;
	m_mapAttribColors.Copy(colors);
	
	if (GetSafeHwnd() && HasStyle(TDCS_COLORTEXTBYATTRIBUTE))
		InvalidateAll();

	return TRUE;
}

BOOL CTDLTaskCtrlBase::GetAttributeColor(const CString& sAttrib, COLORREF& color) const
{
	return m_mapAttribColors.GetColor(sAttrib, color);
}

COLORREF CTDLTaskCtrlBase::GetPriorityColor(int nPriority) const
{
	if (nPriority < 0 || nPriority >= m_aPriorityColors.GetSize())
		return 0;
	
	return (COLORREF)m_aPriorityColors[nPriority];
}

void CTDLTaskCtrlBase::DrawCommentsText(CDC* pDC, const CRect& rRow, const CRect& rLabel, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS)
{
	// Avoid drawing wherever possible
	if (m_bSavingToImage || IsEditingTask(pTDS->GetTaskID()) || pTDI->sComments.IsEmpty())
	{
		return;
	}

	CRect rClip;
	pDC->GetClipBox(rClip);
	
 	CRect rComments((rLabel.right + 10), (rRow.top + 1), min(rRow.right, m_hdrTasks.GetItemWidth(0)), rRow.bottom);

	if ((rClip.right <= rComments.left) || (rComments.Width() <= 0))
	{
		return;
	}

	// Draw the minimum necessary
	COLORREF crText = GetTaskCommentsTextColor(pTDI, pTDS);

	if (HasStyle(TDCS_SHOWCOMMENTSINLIST))
	{
		int nFind = pTDI->sComments.FindOneOf(_T("\n\r")); 

		if (HasStyle(TDCS_SHOWFIRSTCOMMENTLINEINLIST))
		{
			if (nFind == 0) 
				return; // comments start with a newline -> show nothing

			// else
			DrawColumnText(pDC, pTDI->sComments, rComments, DT_LEFT, crText, TRUE, nFind);
		}
		else
		{
			// Calculate the max length of comments we are likely to show
			int nShow = ((int)(rComments.Width() / GraphicsMisc::GetAverageCharWidth(pDC)) * 2);
			nShow = min(nShow, pTDI->sComments.GetLength());

			CString sShow;
			LPTSTR szBuffer = sShow.GetBuffer(nShow);
			
			for (int nChar = 0; nChar < nShow; nChar++)
			{
				TCHAR cChar = pTDI->sComments[nChar];

				switch (cChar)
				{
				case '\r':
				case '\n':
				case '\t':
					cChar = ' ';
					// fall thru
				}

				szBuffer[nChar] = cChar;
			}
			sShow.ReleaseBuffer(nShow);
			sShow.TrimRight();

			DrawColumnText(pDC, sShow, rComments, DT_LEFT, crText, TRUE, nShow);
		}
	}
	else
	{
		DrawColumnText(pDC, _T("[...]"), rComments, DT_LEFT, crText, FALSE, 5);
	}
}

CFont* CTDLTaskCtrlBase::GetTaskFont(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bColumns)
{
	if (!m_fonts.GetHwnd() && !m_fonts.Initialise(Tasks()))
		return NULL;

	BOOL bStrikeThru = (HasStyle(TDCS_STRIKETHOUGHDONETASKS) && pTDI->IsDone());
	BOOL bBold = (pTDS->ParentIsRoot() && !bColumns);

	if (bBold || bStrikeThru)
		return m_fonts.GetFont(bBold, FALSE, FALSE, bStrikeThru);

	// else
	return NULL;
}

BOOL CTDLTaskCtrlBase::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if ((pWnd == &m_lcColumns) && Misc::ModKeysArePressed(0))
	{
		CPoint ptScreen(::GetMessagePos());
		
		if (PtInClientRect(ptScreen, m_lcColumns, TRUE))
		{
			TDC_COLUMN nColID = TDCC_NONE;
			CPoint ptCursor(::GetMessagePos());

			int nHit = HitTestColumnsItem(ptCursor, FALSE, nColID);
			
			if (ItemColumnSupportsClickHandling(nHit, nColID, &ptCursor))
			{
				GraphicsMisc::SetHandCursor();
				return TRUE;
			}
		}
	}
	
	// else
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CTDLTaskCtrlBase::TaskHasReminder(DWORD dwTaskID) const
{
	return (GetTaskReminder(dwTaskID) > 0);
}

BOOL CTDLTaskCtrlBase::GetTaskReminder(DWORD dwTaskID, COleDateTime& dtRem) const
{
	time_t tRem = GetTaskReminder(dwTaskID);

	if (tRem > 0)
	{
		dtRem = tRem;
		return CDateHelper::IsDateSet(dtRem);
	}
	
	// else
	CDateHelper::ClearDate(dtRem);
	return FALSE;
}

time_t CTDLTaskCtrlBase::GetTaskReminder(DWORD dwTaskID) const
{
	return CTDCReminderHelper(*this).GetTaskReminder(dwTaskID);
}

void CTDLTaskCtrlBase::DrawGridlines(CDC* pDC, const CRect& rect, BOOL bSelected, BOOL bHorz, BOOL bVert)
{
	if (HasColor(m_crGridLine))
	{
		if (bHorz)
			GraphicsMisc::DrawHorzLine(pDC, rect.left, rect.right, (rect.bottom - 1), m_crGridLine);

		if (bVert)
		{
			CRect rGridline(rect);
			COLORREF crGridline(m_crGridLine);

			if (bSelected)
			{
				// Make color a little darker
				crGridline = GraphicsMisc::Darker(crGridline, 0.1);
			}

			GraphicsMisc::DrawVertLine(pDC, rGridline.top, rGridline.bottom, rect.right - 1, crGridline);
		}
	}
}

LRESULT CTDLTaskCtrlBase::OnListCustomDraw(NMLVCUSTOMDRAW* pLVCD)
{
	ASSERT(pLVCD->nmcd.hdr.hwndFrom == m_lcColumns);

	if (pLVCD->nmcd.hdr.hwndFrom == m_lcColumns)
	{
		switch (pLVCD->nmcd.dwDrawStage)
		{
		case CDDS_PREPAINT:
			return CDRF_NOTIFYITEMDRAW;
			
		case CDDS_ITEMPREPAINT:
			{
				// task data
				int nItem = (int)pLVCD->nmcd.dwItemSpec;
				DWORD dwTaskID = GetColumnItemTaskID(nItem), dwTrueID(dwTaskID);
				
				const TODOITEM* pTDI = NULL;
				const TODOSTRUCTURE* pTDS = NULL;
				
				if (m_data.GetTrueTask(dwTrueID, pTDI, pTDS))
				{
					CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);

					// draw gridlines and row colour full width of list
					BOOL bAlternate = IsAlternateColumnLine(nItem);
					COLORREF crRowBack = (bAlternate ? m_crAltLine : GetSysColor(COLOR_WINDOW));
					
					CRect rItem;
					m_lcColumns.GetItemRect(nItem, rItem, LVIR_BOUNDS);

					// this call will update rFullWidth to full client width
					CRect rFullWidth(rItem);
					GraphicsMisc::FillItemRect(pDC, rFullWidth, crRowBack, m_lcColumns);

					// selection state
					GM_ITEMSTATE nState = GetColumnItemState(nItem);
					
					if ((nState == GMIS_SELECTEDNOTFOCUSED) && m_dwEditTitleTaskID)
						nState = GMIS_SELECTED;
					
					BOOL bSelected = (nState != GMIS_NONE);
					BOOL bRef = (dwTaskID != dwTrueID);
					
					// colors
					COLORREF crText, crBack;
					GetTaskTextColors(pTDI, pTDS, crText, crBack, bRef, bSelected);

					// draw task background
					// Note: using the non-full width item rect
					if (!bSelected && HasColor(crBack))
						pDC->FillSolidRect(rItem, crBack);

					// draw horz gridline before selection
					DrawGridlines(pDC, rFullWidth, FALSE, TRUE, FALSE);

					// Draw selection using the non-full width item rect
					if (bSelected)
					{
						DWORD dwFlags = GMIB_THEMECLASSIC;

						// if the columns are on the right we clip the column selection
						// on the left so it looks continuous with the title selection
						if (HasStyle(TDCS_RIGHTSIDECOLUMNS))
							dwFlags |= GMIB_CLIPLEFT;

						crText = GraphicsMisc::GetExplorerItemSelectionTextColor(crText, nState, GMIB_THEMECLASSIC);
						GraphicsMisc::DrawExplorerItemSelection(pDC, m_lcColumns, nState, rItem, dwFlags | GMIB_PREDRAW | GMIB_POSTDRAW);
					}

					// draw row text and column dividers
					CFont* pOldFont = pDC->SelectObject(GetTaskFont(pTDI, pTDS));
					
					DrawColumnsRowText(pDC, nItem, dwTaskID, pTDI, pTDS, crText, bSelected);

					pDC->SelectObject(pOldFont);
				}
			}
			return CDRF_SKIPDEFAULT;
		}
	}
	
	return CDRF_DODEFAULT;
}

DWORD CTDLTaskCtrlBase::OnPrePaintTaskTitle(const NMCUSTOMDRAW& nmcd, BOOL bFillRow, COLORREF& crText, COLORREF& crBkgnd)
{
	// Fill the item background with the 'unselected' colour.
	// Although we fill fill the entire row, we are really only
	// interested in the bit to left of the task title where the
	// completion checkbox and task icon will be drawn by Windows
	CDC* pDC = CDC::FromHandle(nmcd.hdc);
	COLORREF crRowBack = (IsAlternateTitleLine(nmcd) ? m_crAltLine : GetSysColor(COLOR_WINDOW));

	if (HasStyle(TDCS_TASKCOLORISBACKGROUND))
	{
		VERIFY(GetTaskTextColors(nmcd.lItemlParam, crText, crBkgnd));

		if (crBkgnd != CLR_NONE)
			crRowBack = crBkgnd;
	}
	crBkgnd = crText = crRowBack;

	if (bFillRow)
		GraphicsMisc::FillItemRect(pDC, &nmcd.rc, crRowBack, Tasks());

	return (CDRF_NOTIFYPOSTPAINT | CDRF_NEWFONT); // always
}

DWORD CTDLTaskCtrlBase::OnPostPaintTaskTitle(const NMCUSTOMDRAW& nmcd)
{
	// Check row is visible
	CRect rClient, rRow(nmcd.rc);
	::GetClientRect(Tasks(), rClient);

	if ((rRow.bottom > 0) && (rRow.top <= rClient.bottom))
	{
		const TODOITEM* pTDI = NULL;
		const TODOSTRUCTURE* pTDS = NULL;

		DWORD dwTaskID(nmcd.lItemlParam), dwTrueID(dwTaskID);

		if (m_data.GetTrueTask(dwTrueID, pTDI, pTDS))
		{
			CDC* pDC = CDC::FromHandle(nmcd.hdc);
			GM_ITEMSTATE nState = GetItemTitleState(nmcd);

			COLORREF crText = 0, crBack = GetSysColor(COLOR_WINDOW);
			VERIFY(GetTaskTextColors(pTDI, pTDS, crText, crBack, (dwTaskID != dwTrueID), (nState != GMIS_NONE)));

			if (!HasColor(crBack))
				crBack = (IsAlternateTitleLine(nmcd) ? m_crAltLine : GetSysColor(COLOR_WINDOW));

			// Set font before getting text length
			CFont* pOldFont = pDC->SelectObject(GetTaskFont(pTDI, pTDS, FALSE));

			// draw label background only
			CRect rLabel;
			GetItemTitleRect(nmcd, TDCTR_BKGND, rLabel, pDC, pTDI->sTitle);

			rRow.right = (rClient.right + GetSystemMetrics(SM_CXVSCROLL));
			rLabel.right = rRow.right; // else overwriting with comments produces artifacts

			pDC->FillSolidRect(rLabel, crBack);

			// draw horz gridline
			DrawGridlines(pDC, rRow, FALSE, TRUE, FALSE);

			// Draw selection before text
			if (!m_bSavingToImage)
			{
				DWORD dwFlags = (GMIB_THEMECLASSIC | GMIB_EXTENDRIGHT | GMIB_PREDRAW | GMIB_POSTDRAW);

				// if the columns are on the right we clip the title selection
				// so it looks continuous with the column selection
				if (HasStyle(TDCS_RIGHTSIDECOLUMNS))
					dwFlags |= GMIB_CLIPRIGHT;

				GraphicsMisc::DrawExplorerItemSelection(pDC, Tasks(), nState, rLabel, dwFlags);
			}

			// draw text
			CRect rText;
			GetItemTitleRect(nmcd, TDCTR_TEXT, rText, pDC, pTDI->sTitle);
			DrawColumnText(pDC, pTDI->sTitle, rText, DT_LEFT, crText, TRUE);

			pDC->SelectObject(pOldFont);

			// draw shortcut for references
			if (dwTaskID != dwTrueID)
			{
				CRect rIcon(rLabel);

				// Draw over icon if icon column NOT visible
				if (!IsColumnShowing(TDCC_ICON))
					rIcon.left -= (ICON_SIZE + 2);
				
				GraphicsMisc::DrawShortcutOverlay(pDC, rIcon);
			}

			// render comment text
			DrawCommentsText(pDC, rRow, rText, pTDI, pTDS);
		}
	}

	return CDRF_SKIPDEFAULT; // always
}

void CTDLTaskCtrlBase::DrawColumnsRowText(CDC* pDC, int nItem, DWORD dwTaskID, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS,
										  COLORREF crText, BOOL bSelected)
{
	DWORD dwTrueID = pTDS->GetTaskID();

	// draw each column separately
	int nNumCol = m_hdrColumns.GetItemCount();

	CRect rSubItem, rClient, rClip;
	m_lcColumns.GetClientRect(rClient);

	pDC->GetClipBox(rClip);
	rClient.IntersectRect(rClient, rClip);

	if (rClient.IsRectEmpty())
		return;

	for (int nCol = 1; nCol < nNumCol; nCol++)
	{
		if (!m_hdrColumns.IsItemVisible(nCol))
			continue;

		if (!m_lcColumns.GetSubItemRect(nItem, nCol, LVIR_BOUNDS, rSubItem))
			continue;

		// don't draw columns outside of client rect
		if (rSubItem.IsRectEmpty())
			continue;

		if (rSubItem.right <= rClient.left)
			continue;

		if (rSubItem.left >= rClient.right)
			continue;

		// vertical gridlines
		DrawGridlines(pDC, rSubItem, bSelected, FALSE, TRUE);

		// don't draw min sized columns
		if (rSubItem.Width() <= MIN_COL_WIDTH)
			continue;
		
		TDC_COLUMN nColID = GetColumnID(nCol);

		// Note: we pass dwTaskID NOT dwTrueID here so that references 
		// can be handled correctly
		CString sTaskColText = GetTaskColumnText(dwTaskID, pTDI, pTDS, nColID, TRUE);
		
		const TDCCOLUMN* pCol = GetColumn(nColID);

		switch (nColID)
		{
		case TDCC_POSITION:
		case TDCC_RISK:
		case TDCC_RECURRENCE:
		case TDCC_ID:
		case TDCC_PARENTID:
		case TDCC_RECENTEDIT:
		case TDCC_COST:
		case TDCC_EXTERNALID:
		case TDCC_VERSION:
		case TDCC_ALLOCTO:
		case TDCC_ALLOCBY:
		case TDCC_STATUS:
		case TDCC_CATEGORY:
		case TDCC_TAGS:
		case TDCC_CREATEDBY:
		case TDCC_PATH:
		case TDCC_REMAINING:
		case TDCC_SUBTASKDONE:
		case TDCC_TIMEEST:
		case TDCC_LASTMODBY:
		case TDCC_COMMENTSSIZE:
			DrawColumnText(pDC, sTaskColText, rSubItem, pCol->nTextAlignment, crText);
			break;
			
		case TDCC_TIMESPENT:
			if (!sTaskColText.IsEmpty())
			{
				// show text in red if we're currently tracking
				COLORREF crTemp = ((m_dwTimeTrackTaskID == dwTrueID) ? 255 : crText);

				DrawColumnText(pDC, sTaskColText, rSubItem, pCol->nTextAlignment, crTemp);
			}
			break;
			
		case TDCC_PRIORITY:
			// priority color
			if (!HasStyle(TDCS_DONEHAVELOWESTPRIORITY) || !m_calculator.IsTaskDone(pTDI, pTDS))
			{
				rSubItem.DeflateRect(2, 1, 3, 2);
				
				// first draw the priority colour
				int nPriority = m_calculator.GetTaskHighestPriority(pTDI, pTDS, FALSE);
				BOOL bHasPriority = (nPriority != FM_NOPRIORITY);
				
				if (bHasPriority)
				{
					COLORREF crFill = GetPriorityColor(nPriority);
					COLORREF crBorder = GraphicsMisc::Darker(crFill, 0.5);
					
					GraphicsMisc::DrawRect(pDC, rSubItem, crFill, crBorder);//, nCornerRadius);
				}
				
				// then, if the task is also due, draw a small tag
				// unless it's due today and the user doesn't want today's tasks marked as due
				// or there's no due color 
				HBRUSH brTag = NULL;
				
				if (HasColor(m_crDue) && m_calculator.IsTaskOverDue(pTDI, pTDS))
				{
					brTag = m_brDue;
				}
				else if (HasColor(m_crDueToday) && m_calculator.IsTaskDueToday(pTDI, pTDS))
				{
					brTag = m_brDueToday;
				}

				if (brTag != NULL)
				{
					POINT pt[3] = 
					{ 
						{ rSubItem.left, rSubItem.top + 7 }, 
						{ rSubItem.left, rSubItem.top }, 
						{ rSubItem.left + 7, rSubItem.top } 
					};
					
					HGDIOBJ hOldBr = pDC->SelectObject(brTag);
					pDC->SelectStockObject(NULL_PEN);
					
					pDC->Polygon(pt, 3);
					pDC->SelectObject(hOldBr);
					
					// a black line between the two
					pDC->SelectStockObject(BLACK_PEN);
					pDC->MoveTo(rSubItem.left, rSubItem.top + 6);
					pDC->LineTo(rSubItem.left + 7, rSubItem.top - 1);
				}
				
				// draw priority number over the top
				if (bHasPriority && !HasStyle(TDCS_HIDEPRIORITYNUMBER))
				{
					COLORREF crTemp = GraphicsMisc::GetBestTextColor(GetPriorityColor(nPriority));
					DrawColumnText(pDC, sTaskColText, rSubItem, pCol->nTextAlignment, crTemp);
				}
			}
			break;
			
		case TDCC_PERCENT:
			if (!sTaskColText.IsEmpty())
			{
				rSubItem.DeflateRect(2, 1, 3, 2);

				// draw default text first
				DrawColumnText(pDC, sTaskColText, rSubItem, pCol->nTextAlignment, crText);

				if (HasStyle(TDCS_SHOWPERCENTASPROGRESSBAR))
				{
					// if the task is done then draw in 'done' colour else priority colour
					BOOL bDone = m_calculator.IsTaskDone(pTDI, pTDS);
					
					COLORREF crBar(m_crDone);
					
					if (!bDone || !HasStyle(TDCS_DONEHAVELOWESTPRIORITY)) // determine appropriate priority
					{
						int nPriority = m_calculator.GetTaskHighestPriority(pTDI, pTDS, FALSE);
						crBar = GetPriorityColor(nPriority);
						
						// check for due
						if (m_calculator.IsTaskOverDue(pTDI, pTDS))
						{
							if (HasColor(m_crDueToday) && m_calculator.IsTaskDueToday(pTDI, pTDS))
							{
								crBar = m_crDueToday;
							}
							else if (HasColor(m_crDue))
							{
								crBar = m_crDue;
							}
						}
					}
					
					if (HasColor(crBar))
					{
						CRect rProgress(rSubItem);
						
						// draw border
						COLORREF crBorder = GraphicsMisc::Darker(crBar, 0.5);
						GraphicsMisc::DrawRect(pDC, rProgress, CLR_NONE, crBorder);
						
						// draw fill
						int nPercent = m_calculator.GetTaskPercentDone(pTDI, pTDS);

						rProgress.DeflateRect(1, 1);
						rProgress.right = rProgress.left + MulDiv(rProgress.Width(), nPercent, 100);
						
						if (rProgress.Width() > 0)
						{
							pDC->FillSolidRect(rProgress, crBar); 
							
							// Exclude the 'unfilled' part so that we do not
							// overwrite the text
							CRect rUnfilled(rSubItem);
							rUnfilled.left = rProgress.right;

							pDC->ExcludeClipRect(rUnfilled);
							
							// draw text in colour to suit progress bar
							COLORREF crTemp = GraphicsMisc::GetBestTextColor(crBar);
							DrawColumnText(pDC, sTaskColText, rSubItem, pCol->nTextAlignment, crTemp);
						}
					}
				}
				
			}
			break;
			
		case TDCC_TRACKTIME:
			if (m_dwTimeTrackTaskID == dwTrueID)
				DrawColumnImage(pDC, nColID, rSubItem);
			break;
			
		case TDCC_FLAG:
			if (pTDI->bFlagged)
			{
				DrawColumnImage(pDC, nColID, rSubItem);
			}
			else if (m_calculator.IsTaskFlagged(pTDI, pTDS))
			{
				DrawColumnImage(pDC, nColID, rSubItem, TRUE);
			}
			break;
			
		case TDCC_LOCK:
			if (pTDI->bLocked)
			{
				DrawColumnImage(pDC, nColID, rSubItem);
			}
			else if (m_calculator.IsTaskLocked(pTDI, pTDS))
			{
				DrawColumnImage(pDC, nColID, rSubItem, TRUE);
			}
			break;

		case TDCC_REMINDER:
			{
				time_t tRem = GetTaskReminder(dwTrueID);

				// Reminder must be set and start/due date must be set
				if (tRem != 0)
				{
					if (tRem != -1)
					{
						if (HasStyle(TDCS_SHOWREMINDERSASDATEANDTIME))
						{
							COleDateTime dtRem(tRem);

							if (CDateHelper::IsDateSet(dtRem))
							{
								DrawColumnDate(pDC, dtRem, TDCD_REMINDER, rSubItem, crText);
							}
						}
						else
						{
							DrawColumnImage(pDC, nColID, rSubItem);
						}
					}
					else
					{
						DrawColumnImage(pDC, nColID, rSubItem, TRUE);
					}
				}
			}
			break;
			
		case TDCC_STARTDATE:
			{
				COleDateTime date;
				BOOL bDone = m_calculator.IsTaskDone(pTDI, pTDS);
				BOOL bCalculated = FALSE;
				
				if (bDone && !HasStyle(TDCS_HIDESTARTDUEFORDONETASKS))
				{
					date = pTDI->dateStart;
				}
				else if (!bDone)
				{
					date = m_calculator.GetTaskStartDate(pTDI, pTDS);
					bCalculated = (date != pTDI->dateStart);
				}
				
				DrawColumnDate(pDC, date, TDCD_START, rSubItem, crText, bCalculated);
			}
			break;
			
		case TDCC_DUEDATE:
			{
				COleDateTime date;
				BOOL bDone = m_calculator.IsTaskDone(pTDI, pTDS);
				BOOL bCalculated = FALSE;
				
				if (bDone && !HasStyle(TDCS_HIDESTARTDUEFORDONETASKS))
				{
					date = pTDI->dateDue;
				}
				else if (!bDone)
				{
					date = m_calculator.GetTaskDueDate(pTDI, pTDS);
					bCalculated = (date != pTDI->dateDue);
				}
				
				DrawColumnDate(pDC, date, TDCD_DUE, rSubItem, crText, bCalculated);
			}
			break;
			
			
		case TDCC_DONEDATE:
			DrawColumnDate(pDC, pTDI->dateDone, TDCD_DONE, rSubItem, crText);
			break;
			
		case TDCC_CREATIONDATE:
			DrawColumnDate(pDC, pTDI->dateCreated, TDCD_CREATE, rSubItem, crText);
			break;
			
		case TDCC_LASTMODDATE:
			DrawColumnDate(pDC, m_calculator.GetTaskLastModifiedDate(pTDI, pTDS), TDCD_LASTMOD, rSubItem, crText);
			break;
			
		case TDCC_ICON:
			{
				int nIcon = GetTaskIconIndex(pTDI, pTDS);
												
				if (nIcon >= 0)
				{
					int nImageSize = m_ilTaskIcons.GetImageSize();
					CPoint pt(CalcColumnIconTopLeft(rSubItem, nImageSize));

					m_ilTaskIcons.Draw(pDC, nIcon, pt, ILD_TRANSPARENT);
				}
			}
			break;
			
		case TDCC_DEPENDENCY:
			if (pTDI->aDependencies.GetSize())
				DrawColumnImage(pDC, nColID, rSubItem);
			break;
			
		case TDCC_FILEREF:
			DrawColumnFileLinks(pDC, pTDI->aFileLinks, rSubItem);
			break;
			
		case TDCC_DONE:
			{
				TTCB_CHECK nCheck = (pTDI->IsDone() ? TTCBC_CHECKED : TTCNC_UNCHECKED);

				if ((nCheck == TTCNC_UNCHECKED) && m_data.TaskHasCompletedSubtasks(pTDS))
					nCheck = TTCBC_MIXED;

				DrawColumnCheckBox(pDC, rSubItem, nCheck);
			}
			break;
			
		default:
			// custom attribute columns
			VERIFY (DrawItemCustomColumn(pTDI, pTDS, nColID, pDC, rSubItem, crText));
			break;
		}
	}
}

int CTDLTaskCtrlBase::GetTaskIconIndex(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;

	if (!m_data.GetTrueTask(dwTaskID, pTDI, pTDS))
		return -1;

	return GetTaskIconIndex(pTDI, pTDS);
}

int CTDLTaskCtrlBase::GetTaskIconIndex(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	int nIcon = m_ilTaskIcons.GetImageIndex(pTDI->sIcon);

	if ((nIcon == -1) && pTDS->HasSubTasks() && HasStyle(TDCS_SHOWPARENTSASFOLDERS))
		nIcon = 0;

	return nIcon;
}

void CTDLTaskCtrlBase::DrawColumnFileLinks(CDC* pDC, const CStringArray& aFileLinks, const CRect& rect)
{
	int nNumFiles = aFileLinks.GetSize();

	for (int nFile = 0; nFile < nNumFiles; nFile++)
	{
		CRect rIcon;

		if (!CalcFileIconRect(rect, rIcon, nFile, nNumFiles))
			break; // out of bounds

		// first check for a tdl://
		CString sFileRef = aFileLinks[nFile];

		if (TODOITEM::IsTaskLink(sFileRef, TRUE))
		{
			// draw our app icon 
			if (m_imageIcons.HasIcon(APP_ICON) ||
				m_imageIcons.Add(APP_ICON, GraphicsMisc::GetAppWindowIcon(FALSE)))
			{
				m_imageIcons.Draw(pDC, APP_ICON, rIcon.TopLeft());
			}
		}
		else
		{
			// get the associated image, failing if necessary
			sFileRef.Remove('\"'); // remove double-quotes
			FileMisc::MakeFullPath(sFileRef, m_sTasklistFolder);

			if (m_imageIcons.HasIcon(sFileRef) ||
				(CEnBitmap::IsSupportedImageFile(sFileRef) &&
					FileMisc::PathExists(sFileRef) &&
					m_imageIcons.Add(sFileRef, sFileRef)))
			{
				m_imageIcons.Draw(pDC, sFileRef, rIcon.TopLeft());
			}
			else
			{
				CFileIcons::Draw(pDC, sFileRef, rIcon.TopLeft());
			}
		}
	}
}

void CTDLTaskCtrlBase::DrawColumnImage(CDC* pDC, TDC_COLUMN nColID, const CRect& rect, BOOL bAlternate)
{
	const TDCCOLUMN* pCol = GetColumn(nColID);
	ASSERT(pCol);

	if (pCol)
	{
		TDCC_IMAGE iImage = (bAlternate ? pCol->iAlternateImage : pCol->iImage);
		ASSERT(iImage != TDCC_NONE);
	
		if (iImage != TDCC_NONE)
		{
			int nImageSize = m_ilColSymbols.GetImageSize();
			CPoint ptDraw(CalcColumnIconTopLeft(rect, nImageSize, iImage));

			m_ilColSymbols.Draw(pDC, iImage, ptDraw, ILD_TRANSPARENT);
		}
	}
}

void CTDLTaskCtrlBase::DrawColumnCheckBox(CDC* pDC, const CRect& rSubItem, TTCB_CHECK nCheck)
{
	int nImageSize = m_ilCheckboxes.GetImageSize();
	CPoint pt(CalcColumnIconTopLeft(rSubItem, nImageSize));
				
	// if the line height is odd, move one pixel down
	// to avoid collision with selection rect
	pt.y += (rSubItem.Height() % 2);

	int nImage = (nCheck + 1); // first image is blank
	m_ilCheckboxes.Draw(pDC, nImage, pt, ILD_TRANSPARENT);
}

CPoint CTDLTaskCtrlBase::CalcColumnIconTopLeft(const CRect& rSubItem, int nImageSize, int nImage, int nCount) const
{
	CRect rImage(rSubItem.TopLeft(), CSize(nImageSize, nImageSize));
	GraphicsMisc::CentreRect(rImage, rSubItem, (nCount == 1), TRUE);
	
	if (nCount > 1)
		rImage.OffsetRect((nImage * (nImageSize + 1)), 0);

	return rImage.TopLeft();
}

BOOL CTDLTaskCtrlBase::CalcFileIconRect(const CRect& rSubItem, CRect& rIcon, int nImage, int nCount) const
{
	int nImageSize = CFileIcons::GetImageSize();

	rIcon = CRect(CalcColumnIconTopLeft(rSubItem, nImageSize, nImage, nCount), CSize(nImageSize, nImageSize));

	// we always draw the first icon
	if ((nImage == 0) || (rIcon.right <= rSubItem.right))
		return TRUE;

	// else
	rIcon.SetRectEmpty();
	return FALSE;
}

BOOL CTDLTaskCtrlBase::DrawItemCustomColumn(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_COLUMN nColID, 
											CDC* pDC, const CRect& rSubItem, COLORREF crText)
{
	if (!TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nColID))
		return FALSE;

	TDCCUSTOMATTRIBUTEDEFINITION attribDef;
	
	if (!m_aCustomAttribDefs.GetAttributeDef(nColID, attribDef) || !attribDef.bEnabled)
		return TRUE;

	TDCCADATA data;
	pTDI->GetCustomAttributeValue(attribDef.sUniqueID, data);

	CRect rCol(rSubItem);
	DWORD dwDataType = attribDef.GetDataType();
	
	switch (dwDataType)
	{
	case TDCCA_DATE:
		{
			double dDate = 0.0;
			m_calculator.GetTaskCustomAttributeData(pTDI, pTDS, attribDef, dDate);

			DrawColumnDate(pDC, dDate, TDCD_CUSTOM, rCol, crText, FALSE, 
							attribDef.HasFeature(TDCCAF_SHOWTIME), attribDef.nTextAlignment);
		}
		break;
		
	case TDCCA_ICON:
		if (!data.IsEmpty() && (rCol.Width() >= CalcRequiredIconColumnWidth(1)))
		{
			CStringArray aImages;
			int nNumImage = data.AsArray(aImages);

			int nReqWidth = CalcRequiredIconColumnWidth(nNumImage);
			int nAvailWidth = rCol.Width();

			if (nAvailWidth < nReqWidth)
			{
				nNumImage = min(nNumImage, ((nAvailWidth + COL_ICON_SPACING - (LV_COLPADDING * 2)) / (COL_ICON_SIZE + COL_ICON_SPACING)));
				nReqWidth = CalcRequiredIconColumnWidth(nNumImage);
			}

			CString sName;
			
			if (nNumImage == 1)
				sName = attribDef.GetImageName(data.AsString());

			rCol.bottom = (rCol.top + COL_ICON_SIZE);
			GraphicsMisc::CentreRect(rCol, rSubItem, FALSE, TRUE); // centre vertically

			int nTextAlign = attribDef.nTextAlignment;
			
			switch (nTextAlign)
			{
			case DT_RIGHT:
				// We still draw from the left just like text
				rCol.left = (rCol.right - nReqWidth);
				break;
				
			case DT_CENTER:
				// if there is associated text then we align left
				if (sName.IsEmpty())
				{
					rCol.right = (rCol.left + nReqWidth);
					GraphicsMisc::CentreRect(rCol, rSubItem, TRUE, FALSE);
				}
				else 
				{
					nTextAlign = DT_LEFT;
				}
				break;
				
			case DT_LEFT:
			default:
				break;
			}

			BOOL bOverrun = FALSE;
			rCol.left += LV_COLPADDING;

			for (int nImg = 0; ((nImg < nNumImage) && !bOverrun); nImg++)
			{
				CString sImage, sDummy;

				if (TDCCUSTOMATTRIBUTEDEFINITION::DecodeImageTag(aImages[nImg], sImage, sDummy))
				{
					m_ilTaskIcons.Draw(pDC, sImage, rCol.TopLeft(), ILD_TRANSPARENT);
					rCol.left += (COL_ICON_SIZE + COL_ICON_SPACING);

					bOverrun = ((rCol.left + COL_ICON_SIZE) > rCol.right);
				}
			}
			
			// optional text for single list images
			if (!bOverrun && (nNumImage == 1) && attribDef.IsList() && !sName.IsEmpty())
			{
				DrawColumnText(pDC, sName, rCol, nTextAlign, crText);
			}
		}
		break;
		
	case TDCCA_BOOL:
		DrawColumnCheckBox(pDC, rSubItem, (data.AsBool() ? TTCBC_CHECKED : TTCNC_UNCHECKED));
		break;

	case TDCCA_FILELINK:
		{
			CStringArray aItems;
			
			if (data.AsArray(aItems))
				DrawColumnFileLinks(pDC, aItems, rSubItem);
		}
		break;

	default:
		{
			CString sData = m_formatter.GetTaskCustomAttributeData(pTDI, pTDS, attribDef);
			DrawColumnText(pDC, sData, rCol, attribDef.nTextAlignment, crText);
		}
		break;
	}

	return TRUE; // we handled it
}

int CTDLTaskCtrlBase::CalcRequiredIconColumnWidth(int nNumImage)
{
	return ((nNumImage * (COL_ICON_SIZE + COL_ICON_SPACING)) - COL_ICON_SPACING + (LV_COLPADDING * 2));
}

BOOL CTDLTaskCtrlBase::FormatDate(const COleDateTime& date, TDC_DATE nDate, CString& sDate, CString& sTime, CString& sDow, BOOL bCustomWantsTime) const
{
	sDate = m_formatter.GetDateOnly(date, TRUE);

	if (sDate.IsEmpty())
		return FALSE;

	if (WantDrawColumnTime(nDate, bCustomWantsTime))
		sTime = m_formatter.GetTimeOnly(date, nDate);
	else
		sTime.Empty();

	if (HasStyle(TDCS_SHOWWEEKDAYINDATES))
		sDow = CDateHelper::GetDayOfWeekName(CDateHelper::GetDayOfWeek(date), TRUE);
	else
		sDow.Empty();

	return TRUE;
}

void CTDLTaskCtrlBase::DrawColumnDate(CDC* pDC, const COleDateTime& date, TDC_DATE nDate, const CRect& rect,
									  COLORREF crText, BOOL bCalculated, BOOL bCustomWantsTime, int nAlign)
{
	CString sDate, sTime, sDow;

	if (!FormatDate(date, nDate, sDate, sTime, sDow, bCustomWantsTime))
		return; // nothing to do

	BOOL bHasTime = !sTime.IsEmpty();
	BOOL bHasDow = !sDow.IsEmpty();
	BOOL bTimeIsReal = CDateHelper::DateHasTime(date);

  	ASSERT((nDate != TDCD_REMINDER) || HasStyle(TDCS_SHOWREMINDERSASDATEANDTIME));
	ASSERT((nDate != TDCD_REMINDER) || bHasTime);

	// Because the column width can be changed by the user we need
	// a strategy for deciding what to cull as the width reduces
	// +--------+-----------------------+-----------------------+
	// |   DoW  |        Date           |        Time           |
	// +--------+-----------------------+-----------------------+

	m_dateTimeWidths.Initialise(pDC);

	int nSpace = pDC->GetTextExtent(_T(" ")).cx;

	int nMaxDateWidth = (m_dateTimeWidths.nMaxDateWidth + nSpace);
	int nMinDateWidth = (m_dateTimeWidths.nMinDateWidth + nSpace);
	int nMaxTimeWidth = (bHasTime ? (m_dateTimeWidths.nMaxTimeWidth + nSpace) : 0);
	int nMaxDayWidth = (bHasDow ? (m_dateTimeWidths.nMaxDowNameWidth + nSpace) : 0);

	// Work out what we can draw
	BOOL bDrawDate = FALSE;
	BOOL bDrawTime = FALSE;
	BOOL bDrawDow = FALSE;

	const int nAvailWidth = (rect.Width() - LV_COLPADDING);
	int nReqWidth = 0;

	do // easy exit
	{
		// Enough room for everything 
		nReqWidth = (nMaxDayWidth + nMaxDateWidth + nMaxTimeWidth);

		if (nAvailWidth >= nReqWidth)
		{
			bDrawDow = bHasDow;
			bDrawDate = TRUE;
			bDrawTime = bHasTime;

			break;
		}

		// Sacrifice wide date for narrow date
		nReqWidth = (nMaxDayWidth + nMinDateWidth + nMaxTimeWidth);

		if (nAvailWidth >= nReqWidth)
		{
			bDrawDow = bHasDow;
			bDrawDate = TRUE;
			bDrawTime = bHasTime;

			nMaxDateWidth = nMinDateWidth;
			sDate = m_formatter.GetDateOnly(date, FALSE);

			break;
		}

		// Sacrifice time component if it's not 'real'
		nReqWidth = (nMaxDayWidth + nMinDateWidth);

		if (bHasDow && !bTimeIsReal && (nAvailWidth >= nReqWidth))
		{
			bDrawDow = TRUE;
			bDrawDate = TRUE;
			bDrawTime = FALSE;

			nMaxDateWidth = nMinDateWidth;
			sDate = m_formatter.GetDateOnly(date, FALSE);

			break;
		}

		// Sacrifice the date if it falls within 7 days
		BOOL bWithin7Days = IsDateWithin7DaysOfToday(date, nDate);

		nReqWidth = (nMaxDayWidth + nMaxTimeWidth);

		if (bHasDow && bWithin7Days && (nAvailWidth >= nReqWidth))
		{
			bDrawDow = TRUE;
			bDrawDate = FALSE;
			bDrawTime = bHasTime;

			break;
		}

		// Sacrifice day of week
		nReqWidth = (nMinDateWidth + nMaxTimeWidth);

		if (nAvailWidth >= nReqWidth)
		{
			bDrawDow = FALSE;
			bDrawTime = bHasTime;
			bDrawDate = TRUE;

			nMaxDateWidth = nMinDateWidth;
			sDate = m_formatter.GetDateOnly(date, FALSE);

			break;
		}

		// Sacrifice date and day of week if date is today and time is 'real'
		if (bTimeIsReal && CDateHelper::IsToday(date))
		{
			bDrawDow = FALSE;
			bDrawTime = TRUE;
			bDrawDate = FALSE;

			nReqWidth = nMaxTimeWidth;
			break;
		}

		// Sacrifice date and time if date is within 7 days
		if (bHasDow && bWithin7Days)
		{
			bDrawDow = TRUE;
			bDrawTime = FALSE;
			bDrawDate = FALSE;

			nReqWidth = nMaxDayWidth;
			break;
		}

		// else display narrow date
		bDrawDow = FALSE;
		bDrawTime = FALSE;
		bDrawDate = TRUE;

		nMaxDateWidth = nMinDateWidth;
		sDate = m_formatter.GetDateOnly(date, FALSE);

		nReqWidth = nMinDateWidth;
	}
	while (false); // always end

	ASSERT((nReqWidth > 0) && (bDrawDow || bDrawDate || bDrawTime));

	// Draw calculated dates in a lighter colour
	if (bCalculated && !Misc::IsHighContrastActive())
	{
		crText = (HasColor(crText) ? crText : pDC->GetTextColor());
		crText = GraphicsMisc::Lighter(crText, 0.5);
	}

	// We always draw FROM THE RIGHT and with each component 
	// aligned to the right
	CRect rDraw(rect);

	switch (nAlign)
	{
	case DT_LEFT:
		ASSERT(nDate == TDCD_CUSTOM);
		rDraw.right = min(rDraw.right, (rDraw.left + nReqWidth));
		break;

	case DT_RIGHT:
		break;

	case DT_CENTER:
		ASSERT(nDate == TDCD_CUSTOM);
		rDraw.right = min(rDraw.right, (rDraw.CenterPoint().x + (nReqWidth / 2)));
		break;
	}

	// draw time first
	if (bDrawTime)
	{
		ASSERT(!sTime.IsEmpty());

		// if NO time component, render 'default' start and due time 
		// in a lighter colour to indicate it wasn't user-set
		COLORREF crTime(crText);

		if (!bTimeIsReal && !bCalculated && !Misc::IsHighContrastActive())
		{
			// Note: If we've already calculated it above we need not again
			if (!HasColor(crTime))
				crTime = pDC->GetTextColor();

			crTime = GraphicsMisc::Lighter(crTime, 0.5);
		}

		// draw and adjust rect
		DrawColumnText(pDC, sTime, rDraw, DT_RIGHT, crTime);
		rDraw.right -= nMaxTimeWidth;
	}

	if (bDrawDate)
	{
		DrawColumnText(pDC, sDate, rDraw, DT_RIGHT, crText);
		rDraw.right -= nMaxDateWidth;
	}
	
	// Finally day of week
	if (bDrawDow)
	{
		ASSERT(!sDow.IsEmpty());

		DrawColumnText(pDC, sDow, rDraw, DT_RIGHT, crText);
	}
}

BOOL CTDLTaskCtrlBase::IsDateWithin7DaysOfToday(const COleDateTime& date, TDC_DATE nDate)
{
	ASSERT(CDateHelper::IsDateSet(date));

	COleDateTime dtToday = CDateHelper::GetDate(DHD_TODAY);

	switch (nDate)
	{
	case TDCD_CREATE:
	case TDCD_DONE:
	case TDCD_LASTMOD:
		// Previous 6 days
		dtToday.m_dt++;
		return ((date < dtToday) && ((dtToday.m_dt - date.m_dt) < 7));

	case TDCD_START:
	case TDCD_DUE:
	case TDCD_REMINDER:
	case TDCD_CUSTOM:
		// Next 6 days
		return ((date >= dtToday) && ((date.m_dt - dtToday.m_dt) < 7));
	}

	ASSERT(0);
	return FALSE;
}

void CTDLTaskCtrlBase::DrawColumnText(CDC* pDC, const CString& sText, const CRect& rect, int nAlign, 
										COLORREF crText, BOOL bTaskTitle, int nTextLen)
{
	ASSERT(crText != CLR_NONE);

	if (sText.IsEmpty())
		return;

	if (nTextLen == -1)
		nTextLen = sText.GetLength();
	
	CRect rText(rect);
	CPoint ptText(0, rText.top);
	
	if (!bTaskTitle)
	{
		switch (nAlign)
		{
		case DT_LEFT:
			rText.left += LV_COLPADDING;
			break;
			
		case DT_RIGHT:
			rText.right -= LV_COLPADDING;
			break;
			
		case DT_CENTER:
			break;
		}
	}
	
	UINT nFlags = (nAlign | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | GraphicsMisc::GetRTLDrawTextFlags(Tasks()));

	if (!m_bSavingToImage && bTaskTitle)
		nFlags |= DT_END_ELLIPSIS;

	COLORREF crOld = pDC->SetTextColor(crText);
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(sText, nTextLen, rText, nFlags);
	pDC->SetTextColor(crOld);
}

LRESULT CTDLTaskCtrlBase::OnHeaderCustomDraw(NMCUSTOMDRAW* pNMCD)
{
	switch (pNMCD->dwDrawStage)
	{
	case CDDS_PREPAINT:
		return CDRF_NOTIFYITEMDRAW;
		
	case CDDS_ITEMPREPAINT:
		{
			// don't draw columns having min width
			CRect rItem(pNMCD->rc);

			if (rItem.Width() > MIN_COL_WIDTH)
				return CDRF_NOTIFYPOSTPAINT;
		}
		break;

	case CDDS_ITEMPOSTPAINT:
		{
			// don't draw columns having min width
			CRect rItem(pNMCD->rc);

			if (rItem.Width() > MIN_COL_WIDTH)
			{
				CDC* pDC = CDC::FromHandle(pNMCD->hdc);

				// draw sort direction
				int nCol = (int)pNMCD->dwItemSpec;
				TDC_COLUMN nColID = (TDC_COLUMN)pNMCD->lItemlParam;

				if (nColID == m_nSortColID)
				{
					BOOL bUp = (m_nSortDir == TDC_SORTUP);
					GetColumnHeaderCtrl(nColID).DrawItemSortArrow(pDC, nCol, bUp);
				}

				const TDCCOLUMN* pTDCC = GetColumn(nColID);
				int nAlignment = DT_LEFT;
				
				if (pTDCC)
				{
					nAlignment = pTDCC->GetColumnHeaderAlignment();

					// handle symbol images
					if (pTDCC->iImage != -1)
					{
						CRect rImage(0, 0, COL_ICON_SIZE, COL_ICON_SIZE);
						GraphicsMisc::CentreRect(rImage, rItem, TRUE, TRUE);

 						m_ilColSymbols.Draw(pDC, pTDCC->iImage, rImage.TopLeft(), ILD_TRANSPARENT);
						return CDRF_SKIPDEFAULT;
					}
				}

				// Handle RTL text column headers
				if (GraphicsMisc::GetRTLDrawTextFlags(pNMCD->hdr.hwndFrom) == DT_RTLREADING)
				{
					CString sColumn(GetColumnHeaderCtrl(nColID).GetItemText(nCol));
					DrawColumnText(pDC, sColumn, pNMCD->rc, nAlignment, GetSysColor(COLOR_WINDOWTEXT));
					
					return CDRF_SKIPDEFAULT;
				}
			}
		}
		break;
	}
	
	return CDRF_DODEFAULT;
}

const CEnHeaderCtrl& CTDLTaskCtrlBase::GetColumnHeaderCtrl(TDC_COLUMN nColID) const
{
	return ((nColID == TDCC_CLIENT) ? m_hdrTasks : m_hdrColumns);
}

const TDCCOLUMN* CTDLTaskCtrlBase::GetColumn(TDC_COLUMN nColID)
{
	if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nColID))
		return NULL;

	ASSERT(!s_mapColumns.IsEmpty());

	const TDCCOLUMN* pCol = NULL;
	VERIFY(s_mapColumns.Lookup(nColID, pCol));

	ASSERT(pCol);
	return pCol;
}

TDC_COLUMN CTDLTaskCtrlBase::GetColumnID(int nCol) const
{
	if (nCol == 0)
		return TDCC_CLIENT;

	if (nCol > 0)
		return (TDC_COLUMN)m_hdrColumns.GetItemData(nCol);

	ASSERT(0);
	return TDCC_NONE;
}

void CTDLTaskCtrlBase::SetCompletionStatus(const CString& sStatus) 
{ 
	if (sStatus != m_sCompletionStatus)
	{
		m_sCompletionStatus = sStatus; 

		if (IsColumnShowing(TDCC_STATUS))
			RedrawColumn(TDCC_STATUS);
	}
}

CString CTDLTaskCtrlBase::FormatTaskDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_DATE nDate) const
{
	COleDateTime date;

	switch (nDate)
	{
	case TDCD_CREATE:	return FormatDate(pTDI->GetDate(nDate), nDate);
	case TDCD_DONE:		return FormatDate(pTDI->GetDate(nDate), nDate);
	case TDCD_START:	return FormatDate(m_calculator.GetTaskStartDate(pTDI, pTDS), nDate);
	case TDCD_DUE:		return FormatDate(m_calculator.GetTaskDueDate(pTDI, pTDS), nDate);
	case TDCD_LASTMOD:	return FormatDate(m_calculator.GetTaskLastModifiedDate(pTDI, pTDS), nDate);

	default:
		ASSERT(0);
		break;
	}

	return EMPTY_STR;
}

CString CTDLTaskCtrlBase::FormatDate(const COleDateTime& date, TDC_DATE nDate) const
{
	switch (nDate)
	{
	case TDCD_CREATE:
	case TDCD_DONE:
	case TDCD_START:
	case TDCD_DUE:
	case TDCD_LASTMOD:
	case TDCD_REMINDER:
		{
			CString sDate, sTime, sDow;

			if (FormatDate(date, nDate, sDate, sTime, sDow))
				return (sDow + ' ' + sDate + ' ' + sTime);
		}
		break;

	default:
		ASSERT(0);
		break;
	}

	return EMPTY_STR;
}

CString CTDLTaskCtrlBase::GetTaskColumnText(DWORD dwTaskID, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_COLUMN nColID, BOOL bDrawing) const
{
	if (!pTDS || !pTDI || !dwTaskID || (nColID == TDCC_NONE))
	{
		ASSERT(0);
		return EMPTY_STR;
	}

	switch (nColID)
	{
	case TDCC_CLIENT:		return pTDI->sTitle;
	case TDCC_EXTERNALID:	return pTDI->sExternalID;
	case TDCC_VERSION:		return pTDI->sVersion;
	case TDCC_LASTMODBY:	return pTDI->sLastModifiedBy;
	case TDCC_ALLOCBY:		return pTDI->sAllocBy;
	case TDCC_CREATEDBY:	return pTDI->sCreatedBy;

	case TDCC_POSITION:		return m_formatter.GetTaskPosition(pTDS);
	case TDCC_RISK:			return m_formatter.GetTaskRisk(pTDI, pTDS);
	case TDCC_RECURRENCE:	return m_formatter.GetTaskRecurrence(pTDI);
	case TDCC_RECENTEDIT:	return m_formatter.GetTaskRecentlyModified(pTDI, pTDS);
	case TDCC_COST:			return m_formatter.GetTaskCost(pTDI, pTDS);
	case TDCC_ALLOCTO:		return m_formatter.GetTaskAllocTo(pTDI);
	case TDCC_STATUS:		return m_formatter.GetTaskStatus(pTDI, pTDS, m_sCompletionStatus);
	case TDCC_CATEGORY:		return m_formatter.GetTaskCategories(pTDI);
	case TDCC_TAGS:			return m_formatter.GetTaskTags(pTDI);
	case TDCC_PERCENT:		return m_formatter.GetTaskPercentDone(pTDI, pTDS);
	case TDCC_REMAINING:	return m_formatter.GetTaskTimeRemaining(pTDI, pTDS);
	case TDCC_TIMEEST:		return m_formatter.GetTaskTimeEstimate(pTDI, pTDS);
	case TDCC_TIMESPENT:	return m_formatter.GetTaskTimeSpent(pTDI, pTDS);
	case TDCC_PATH:			return m_formatter.GetTaskPath(pTDI, pTDS);
	case TDCC_SUBTASKDONE:	return m_formatter.GetTaskSubtaskCompletion(pTDI, pTDS);
	case TDCC_COMMENTSSIZE:	return m_formatter.GetTaskCommentSize(pTDI);

	case TDCC_ID:			return m_formatter.GetID(dwTaskID, pTDS->GetTaskID());
	case TDCC_PARENTID:		return m_formatter.GetID(pTDS->GetParentTaskID());

		// items having no text
	case TDCC_ICON:
	case TDCC_DONE:
	case TDCC_FLAG:
	case TDCC_LOCK:
	case TDCC_TRACKTIME:
		return EMPTY_STR;

		// items rendered differently
	case TDCC_STARTDATE:
	case TDCC_DUEDATE:
	case TDCC_DONEDATE:
	case TDCC_CREATIONDATE:
	case TDCC_LASTMODDATE:
		if (!bDrawing)
			return FormatTaskDate(pTDI, pTDS, TDC::MapColumnToDate(nColID));
		break;

	case TDCC_DEPENDENCY:
		if (!bDrawing)
			return Misc::FormatArray(pTDI->aDependencies, '+');
		break;

	case TDCC_REMINDER:
		if (!bDrawing)
		{
			time_t tRem = GetTaskReminder(m_data.GetTrueTaskID(dwTaskID));

			// Reminder must be set and start/due date must be set
			if ((tRem != 0) && (tRem != -1))
				return FormatDate(COleDateTime(tRem), TDCD_REMINDER);
		}
		break;

	case TDCC_FILEREF:
		if (!bDrawing)
			return Misc::FormatArray(pTDI->aFileLinks, '+');
		break;

	case TDCC_PRIORITY:
		if (!bDrawing || !HasStyle(TDCS_HIDEPRIORITYNUMBER))
			return m_formatter.GetTaskPriority(pTDI, pTDS);
		break;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nColID))
		{
			if (!bDrawing)
			{
				TDCCUSTOMATTRIBUTEDEFINITION attribDef;

				if (m_aCustomAttribDefs.GetAttributeDef(nColID, attribDef))
				{
					switch (attribDef.GetDataType())
					{
					case TDCCA_BOOL:
					case TDCCA_ICON:
						return EMPTY_STR;
					}

					TDCCADATA data;

					if (pTDI->GetCustomAttributeValue(attribDef.sUniqueID, data))
						return attribDef.FormatData(data, HasStyle(TDCS_SHOWDATESINISO));
				}
			}
			return EMPTY_STR;
		}
		else
		{
			ASSERT(0);
		}
		break;
	}

	return EMPTY_STR;
}

// message and notifications for 'us'
LRESULT CTDLTaskCtrlBase::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (!IsResyncEnabled())
		return CTreeListSyncer::WindowProc(hRealWnd, msg, wp, lp);
	
	ASSERT(hRealWnd == GetHwnd()); // 'us'

	switch (msg)
	{
	case WM_CONTEXTMENU:
		if ((HWND)wp == m_scLeft.GetHwnd() || 
			(HWND)wp == m_scRight.GetHwnd() ||
			(HWND)wp == m_hdrTasks.GetSafeHwnd() ||
			(HWND)wp == m_hdrColumns.GetSafeHwnd())
		{
			// pass on to parent
			::SendMessage(GetHwnd(), WM_CONTEXTMENU, (WPARAM)hRealWnd, lp);

			return 0L; // eat
		}
		break;
		
	case WM_LBUTTONDBLCLK:
		{
			CPoint ptCursor(lp);
			CRect rSplitter;
			
			if (GetSplitterRect(rSplitter) && rSplitter.PtInRect(ptCursor))
			{
				AdjustSplitterToFitAttributeColumns();
				m_bAutoFitSplitter = TRUE;

				return 0L; // eat
			}
		}
		break;
		
	case WM_NOTIFY:
		// only interested in notifications from the tree/list pair to their parent
		if (wp == m_scLeft.GetDlgCtrlID() || 
			wp == m_scRight.GetDlgCtrlID() ||
			wp == (UINT)m_hdrTasks.GetDlgCtrlID() ||
			wp == (UINT)m_hdrColumns.GetDlgCtrlID())
		{
			// let base class have its turn first
			LRESULT lr = CTreeListSyncer::WindowProc(hRealWnd, msg, wp, lp);
			
			// our extra handling
			LPNMHDR pNMHDR = (LPNMHDR)lp;
			HWND hwnd = pNMHDR->hwndFrom;
			
			switch (pNMHDR->code)
			{
			case HDN_ITEMCLICK:
				// column header clicks handled in ScWindowProc
				if (hwnd == m_hdrTasks)
				{
					NMHEADER* pHDN = (NMHEADER*)pNMHDR;
					
					if (pHDN->iButton == 0)
						OnHeaderClick(TDCC_CLIENT);

					return 0L; // eat
				}
				break;
				
			case NM_RCLICK:
				// headers don't generate their own WM_CONTEXTMENU
				if (hwnd == m_hdrTasks)
				{
					// pass on to parent
					::SendMessage(GetHwnd(), WM_CONTEXTMENU, (WPARAM)hwnd, (LPARAM)::GetMessagePos());
				}
				break;

			case NM_CLICK:
				if (hwnd == m_lcColumns)
				{
					const NMITEMACTIVATE* pNMIA = (NMITEMACTIVATE*)lp;

					if (pNMIA->iItem != -1)// valid items only
					{
						DWORD dwTaskID = GetColumnItemTaskID(pNMIA->iItem); // task ID
						TDC_COLUMN nColID = GetColumnID(pNMIA->iSubItem);
						
						if (ItemColumnSupportsClickHandling(pNMIA->iItem, nColID))
						{
							if (nColID == TDCC_FILEREF)
								HandleFileLinkColumnClick(pNMIA->iItem, dwTaskID, pNMIA->ptAction);
							else
								NotifyParentOfColumnEditClick(nColID, dwTaskID);
						}
					}
				}
				break;
			}
				
			return lr;
		}
		break;
	}
	
	return CTreeListSyncer::WindowProc(hRealWnd, msg, wp, lp);
}

int CTDLTaskCtrlBase::CalcSplitterPosToFitListColumns() const
{
	int nFirst = m_hdrColumns.GetFirstVisibleItem();
	int nLast = m_hdrColumns.GetLastVisibleItem();

	if ((nFirst == -1) && (nLast == -1))
		return -1;

	CRect rFirst, rLast;
	VERIFY(m_hdrColumns.GetItemRect(nFirst, rFirst) && m_hdrColumns.GetItemRect(nLast, rLast));
	
	CRect rClient;
	CWnd::GetClientRect(rClient);

	int nNewSplitPos = 0;
	int nColsWidth = ((rLast.right - rFirst.left) + 10/*GetSplitterWidth()*/);

	if (IsRight(m_lcColumns))
	{
		if (HasVScrollBar(m_lcColumns))
			nColsWidth += GetSystemMetrics(SM_CXVSCROLL);

		nNewSplitPos = max(MIN_TASKS_WIDTH, (rClient.right - nColsWidth));
	}
	else // cols on left
	{
		nNewSplitPos = min(nColsWidth, (rClient.right - MIN_TASKS_WIDTH));
	}

	return nNewSplitPos;
}

void CTDLTaskCtrlBase::AdjustSplitterToFitAttributeColumns()
{
	int nNewSplitPos = CalcSplitterPosToFitListColumns();

	if ((nNewSplitPos != -1) && (nNewSplitPos != GetSplitPos()))
	{
		CRect rClient;
		CWnd::GetClientRect(rClient);
		
		CTreeListSyncer::Resize(rClient, nNewSplitPos);
	}
}

void CTDLTaskCtrlBase::RepackageAndSendToParent(UINT msg, WPARAM /*wp*/, LPARAM lp)
{
	// sanity check
	ASSERT(msg == WM_NOTIFY);

	if (msg == WM_NOTIFY)
	{
		NMHDR* pNMHDR = (NMHDR*)lp;
		NMHDR hdrOrg = *pNMHDR; // so we can restore after

		pNMHDR->hwndFrom = GetSafeHwnd();
		pNMHDR->idFrom = CWnd::GetDlgCtrlID();
		
		CWnd::GetParent()->SendMessage(msg, pNMHDR->idFrom, lp);

		// restore
		pNMHDR->hwndFrom = hdrOrg.hwndFrom;
		pNMHDR->idFrom = hdrOrg.idFrom;
	}
}

LRESULT CTDLTaskCtrlBase::ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (!IsResyncEnabled())
		return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);
	
	if (hRealWnd == Tasks())
	{
		switch (msg)
		{
		case WM_PRINT:
			if (!m_lcColumns.GetItemCount() && !m_sTasksWndPrompt.IsEmpty())
			{
				LRESULT lr = ScDefault(hRealWnd);
				CWndPrompt::DrawPrompt(hRealWnd, m_sTasksWndPrompt, (HDC)wp, TRUE);

				return lr;
			}
			break;
			
		case WM_PAINT:
			if (!m_lcColumns.GetItemCount() && !m_sTasksWndPrompt.IsEmpty())
			{
				LRESULT lr = ScDefault(hRealWnd);
				CWndPrompt::DrawPrompt(hRealWnd, m_sTasksWndPrompt, (HDC)wp, TRUE);

				return lr;
			}
			break;
			
		case WM_HSCROLL:
			// Windows only invalidates the item labels but
			// we need the whole row because we render the 
			// comments after the task text
			::InvalidateRect(hRealWnd, NULL, FALSE);
			break;

		case WM_MOUSEWHEEL:
			// Windows only invalidates the item labels but
			// we need the whole row because we render the 
			// comments after the task text
			if (HasHScrollBar(hRealWnd) && !HasVScrollBar(hRealWnd))
				::InvalidateRect(hRealWnd, NULL, FALSE);
			break;
		}
	}
	else if (hRealWnd == m_lcColumns)
	{
		switch (msg)
		{
		case WM_TIMER:
			{
				switch (wp)
				{
				case 0x2A:
				case 0x2B:
					// These are timers internal to the list view associated
					// with editing labels and which cause unwanted selection
					// changes. Given that we have disabled label editing for 
					// the attribute columns we can safely kill these timers
					::KillTimer(hRealWnd, wp);
					return TRUE;
				}
			}
			break;
		
		case WM_NOTIFY:
			{
				LPNMHDR pNMHDR = (LPNMHDR)lp;
				HWND hwnd = pNMHDR->hwndFrom;
				
				switch (pNMHDR->code)
				{
				case NM_RCLICK:
					// headers don't generate their own WM_CONTEXTMENU
					if (hwnd == m_hdrColumns)
					{
						// pass on to parent
						::SendMessage(GetHwnd(), WM_CONTEXTMENU, (WPARAM)hwnd, ::GetMessagePos());
					}
					break;

				case HDN_DIVIDERDBLCLICK:
					if (hwnd == m_hdrColumns)
					{
						// resize just that column
						int nItem = ((NMHEADER*)pNMHDR)->iItem;
						
						if (m_hdrColumns.IsItemVisible(nItem))
						{
							CClientDC dc(&m_lcColumns);
							CFont* pOldFont = GraphicsMisc::PrepareDCFont(&dc, m_lcColumns);
							int nColWidth = CalcColumnWidth(nItem, &dc, IsTreeList());
							
							m_hdrColumns.SetItemWidth(nItem, nColWidth);
							m_hdrColumns.SetItemTracked(nItem, FALSE); // width now auto-calc'ed
							
							dc.SelectObject(pOldFont);
						}
						return 0L;
					}

				case HDN_ITEMCLICK:
					if (hwnd == m_hdrColumns)
					{
						NMHEADER* pHDN = (NMHEADER*)pNMHDR;

						// forward on to our parent
						if ((pHDN->iButton == 0) && m_hdrColumns.IsItemVisible(pHDN->iItem))
						{
							OnHeaderClick(GetColumnID(pHDN->iItem));
						}
						return 0L;
					}
					break;
				}
			}
			break;

 		case WM_ERASEBKGND:
			if (COSVersion() == OSV_LINUX)
			{
				CRect rClient;
				m_lcColumns.GetClientRect(rClient);

				CDC::FromHandle((HDC)wp)->FillSolidRect(rClient, GetSysColor(COLOR_WINDOW));
			}
			return TRUE;

		case WM_KEYUP:
			switch (wp)
			{
			case VK_NEXT:  
			case VK_DOWN:
			case VK_UP:
			case VK_PRIOR: 
			case VK_HOME:
			case VK_END: 
			case VK_SHIFT: 
			case VK_CONTROL: 
				// force a parent notification
				NotifyParentSelChange(SC_BYKEYBOARD);
				break;
			}
			break;
			
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
			{
				// let parent handle any focus changes first
				m_lcColumns.SetFocus();

				// don't let the selection to be set to -1
				// when clicking below the last item
				if (m_lcColumns.HitTest(lp) == -1)
					return 0L; // eat it
			}
			break;

		case WM_RBUTTONDOWN:
			// Don't let the selection to be set to -1 when clicking below the last item
			// BUT NOT ON Linux because it interferes with context menu handling
			if (COSVersion() != OSV_LINUX)
			{
				// let parent handle any focus changes first
				m_lcColumns.SetFocus();

				// don't let the selection to be set to -1
				// when clicking below the last item
				if (m_lcColumns.HitTest(lp) == -1)
				{
					CPoint pt(lp);
					::ClientToScreen(hRealWnd, &pt);

					// we don't want to disable drag selecting
					if (!::DragDetect(m_lcColumns, pt))
					{
						TRACE(_T("Ate Listview ButtonDown\n"));
						return 0L; // eat it
					}
				}
			}
			break;
				
		case WM_LBUTTONDOWN:
			if (HandleListLBtnDown(m_lcColumns, lp))
				return 0L; // eat it
			break;
		}
	}
	
	return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);
}

BOOL CTDLTaskCtrlBase::OnHeaderItemWidthChanging(NMHEADER* pHDN, int nMinWidth)
{
	if (pHDN->hdr.hwndFrom == m_hdrColumns)
	{
		// Prevent changing the first hidden column
		if (pHDN->iItem == 0)
		{
			pHDN->pitem->cxy = 0;
		}
		else
		{
			TDC_COLUMN nColID = GetColumnID(pHDN->iItem);

			if (IsColumnShowing(nColID))
				nMinWidth = MIN_COL_WIDTH;
		}
	}
	else if (pHDN->hdr.hwndFrom == m_hwndPrimaryHeader)
	{
		if (pHDN->iItem == 0)
			nMinWidth = MIN_TASKS_WIDTH;
		else
			nMinWidth = MIN_COL_WIDTH;
	}

	return CTreeListSyncer::OnHeaderItemWidthChanging(pHDN, nMinWidth);
}

BOOL CTDLTaskCtrlBase::OnListHeaderBeginTracking(NMHEADER* pHDN)
{
	// Prevent tracking the first hidden column
	if (pHDN->iItem == 0)
		return FALSE;

	// else
	return m_hdrColumns.IsItemTrackable(pHDN->iItem);
}

void CTDLTaskCtrlBase::HandleFileLinkColumnClick(int nItem, DWORD dwTaskID, CPoint pt)
{
	const TODOITEM* pTDI = GetTask(dwTaskID);
	ASSERT(pTDI);
	
	if (pTDI == NULL)
		return;
	
	int nNumFiles = pTDI->aFileLinks.GetSize();
	
	switch (nNumFiles)
	{
	case 0:
		break;
		
	case 1:
		ShowFileLink(pTDI->aFileLinks[0]);
		break;
		
	default: // > 1 file links
		{
			int nCol = GetColumnIndex(TDCC_FILEREF);
			ASSERT(nCol != -1);
			ASSERT(m_hdrColumns.IsItemVisible(nCol));
			
			CRect rSubItem;
			VERIFY(m_lcColumns.GetSubItemRect(nItem, nCol, LVIR_BOUNDS, rSubItem));
			
			ASSERT(rSubItem.PtInRect(pt));
			
			for (int nFile = 0; nFile < nNumFiles; nFile++)
			{
				CRect rIcon;
				
				if (!CalcFileIconRect(rSubItem, rIcon, nFile, nNumFiles))
					break; // outside the subitem
				
				if (rIcon.PtInRect(pt))
				{
					ShowFileLink(pTDI->aFileLinks[nFile]);
					break;
				}
			}
		}
		break; 
	}
}

void CTDLTaskCtrlBase::ShowFileLink(LPCTSTR szFilePath) const
{
	// Handle Outlook manually because under Windows 10 ShellExecute 
	// will succeed even if Outlook is not installed
	if (CMSOutlookHelper::IsOutlookUrl(szFilePath))
	{
		if (CMSOutlookHelper::HandleUrl(*this, szFilePath))
			return;
	}
	else if (FileMisc::Run(GetSafeHwnd(), szFilePath, NULL, SW_SHOWNORMAL, m_sTasklistFolder) >= SE_ERR_SUCCESS)
	{
		return;
	}

	// else forward to our parent
	::SendMessage(::GetParent(GetSafeHwnd()), WM_TDCM_FAILEDLINK, (WPARAM)GetSafeHwnd(), (LPARAM)szFilePath);
}

BOOL CTDLTaskCtrlBase::HandleListLBtnDown(CListCtrl& lc, CPoint pt)
{
	m_bBoundSelecting = FALSE;

	// let parent handle any focus changes first
	lc.SetFocus();

	int nHit = -1;
	TDC_COLUMN nColID = TDCC_NONE;

	if (lc == m_lcColumns)
	{
		DWORD dwTaskID = 0;
		nHit = HitTestColumnsItem(pt, TRUE, nColID, &dwTaskID);

		if (nColID != TDCC_NONE)
		{
			if (Misc::ModKeysArePressed(MKS_ALT))
			{
				NMITEMACTIVATE nmia = { 0 };

				nmia.hdr.hwndFrom = GetSafeHwnd();
				nmia.hdr.code = NM_CLICK;
				nmia.hdr.idFrom = CWnd::GetDlgCtrlID();

				nmia.iItem = nHit;
				nmia.iSubItem = nColID;

				CWnd::GetParent()->SendMessage(WM_NOTIFY, nmia.hdr.idFrom, (LPARAM)&nmia);
				return TRUE; // eat it
			}
			else
			{
				// if the user clicked on a column that allows direct input
				// AND multi items are selected and the item clicked is 
				// already selected then we generate a NM_CLICK and eat the 
				// message to prevent a selection change
				BOOL bMultiSelection = (m_lcColumns.GetSelectedCount() > 1);
				BOOL bTaskSelected = IsListItemSelected(m_lcColumns, nHit);

				if (bMultiSelection && bTaskSelected && 
					ItemColumnSupportsClickHandling(nHit, nColID))
				{
					// special case
					if (nColID == TDCC_FILEREF)
						HandleFileLinkColumnClick(nHit, dwTaskID, pt);
					else
						NotifyParentOfColumnEditClick(nColID, dwTaskID);

					TRACE(_T("Ate Listview LButtonDown\n"));
					return TRUE; // eat it
				}
			}
		}
	}
	else
	{
		nHit = lc.HitTest(pt);
	}

	// De-selecting a lot of items can be slow because 
	// OnListSelectionChange is called once for each.
	// So we try to detect big changes here and handle 
	// them ourselves.
	lc.ClientToScreen(&pt);

	if (nHit != -1)
	{
		BOOL bHitSelected = IsListItemSelected(m_lcColumns, nHit);

		if (Misc::ModKeysArePressed(0) && 
			(!bHitSelected || !ItemColumnSupportsClickHandling(nHit, nColID)))
		{
			DeselectAll();
		}
	}
	else if (::DragDetect(lc, pt))
	{
		m_bBoundSelecting = -1;

		if (!Misc::IsKeyPressed(VK_CONTROL))
		{
			DeselectAll();
		}

		// there's no reliable to way to detect the end of a
		// bounding-box selection especially if the mouse 
		// cursor ends up outside the window so we use a timer
		SetTimer(TIMER_BOUNDINGSEL, 50, NULL);
	}
	else // prevent deselection
	{
		TRACE(_T("Ate Listview ButtonDown\n"));
		return TRUE; // eat it
	}

	return FALSE;
}

void CTDLTaskCtrlBase::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case TIMER_BOUNDINGSEL:
		if (m_bBoundSelecting && !Misc::IsKeyPressed(VK_LBUTTON))
		{
			m_bBoundSelecting = FALSE;

			KillTimer(TIMER_BOUNDINGSEL);
			NotifyParentSelChange(SC_BYMOUSE);
		}
		break;
	}

	CWnd::OnTimer(nIDEvent);
}

void CTDLTaskCtrlBase::NotifyParentOfColumnEditClick(TDC_COLUMN nColID, DWORD dwTaskID)
{
	ASSERT(nColID != TDCC_FILEREF); // handled by HandleFileLinkColumnClick

	CWnd::GetParent()->SendMessage(WM_TDCN_COLUMNEDITCLICK, nColID, dwTaskID);
}

void CTDLTaskCtrlBase::OnHeaderClick(TDC_COLUMN nColID)
{
	TDC_COLUMN nPrev = m_sort.single.nBy;
	TDC_COLUMN nSortBy = TDCC_NONE;
	
	// check for default attribute
	const TDCCOLUMN* pTDCC = GetColumn(nColID);
	
	// could also be a custom column
	if (pTDCC)
	{
		nSortBy = nColID;
	}	
	else if (m_aCustomAttribDefs.IsColumnSortable(nColID))
	{
		nSortBy = nColID;
	}
	
	// do the sort
	if (nSortBy != TDCC_NONE)
	{
		Sort(nSortBy);
		
		// notify parent
		CWnd::GetParent()->SendMessage(WM_TDCN_SORT, CWnd::GetDlgCtrlID(), MAKELPARAM((WORD)nPrev, (WORD)nSortBy));
	}
}

BOOL CTDLTaskCtrlBase::ItemColumnSupportsClickHandling(int nItem, TDC_COLUMN nColID, const CPoint* pCursor) const
{
	if ((nItem == -1) || !Misc::ModKeysArePressed(0))
		return FALSE;

	DWORD dwTaskID = GetColumnItemTaskID(nItem);
	ASSERT(dwTaskID);

	BOOL bNoModifiers = Misc::ModKeysArePressed(0);
	BOOL bSingleSelection = (GetSelectedCount() == 1);
	BOOL bTaskSelected = IsListItemSelected(m_lcColumns, nItem);
	BOOL bReadOnly = IsReadOnly();
	BOOL bLocked = m_calculator.IsTaskLocked(dwTaskID);

	// Edit operations
	if (!bReadOnly)
	{
		switch (nColID)
		{
		case TDCC_DONE:
		case TDCC_FLAG:
		case TDCC_RECURRENCE:
		case TDCC_ICON:
			return !bLocked;

		case TDCC_LOCK:
			// Prevent editing of subtasks inheriting parent lock state
			if (HasStyle(TDCS_SUBTASKSINHERITLOCK))
				return !m_calculator.IsTaskLocked(m_data.GetTaskParentID(dwTaskID));

			// else
			return TRUE;
			
		case TDCC_TRACKTIME:
			// check tasklist is editable, task is trackable and 
			// neither the ctrl not shift keys are pressed (ie => multiple selection)
			// and either the task is not selected or it's only singly selected
			return (bNoModifiers && !bLocked &&
					m_data.IsTaskTimeTrackable(dwTaskID) &&
					(!bTaskSelected || bSingleSelection));

		default: // try custom columns
			if (!bLocked && TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nColID))
			{
				TDCCUSTOMATTRIBUTEDEFINITION attribDef;
			
				if (m_aCustomAttribDefs.GetAttributeDef(nColID, attribDef))
				{
					switch (attribDef.GetDataType())
					{
					case TDCCA_BOOL:
						return TRUE;
					
					case TDCCA_ICON:
						switch (attribDef.GetListType())
						{
						case TDCCA_FIXEDLIST:
						case TDCCA_NOTALIST:
							return TRUE;
						}
						break;
					
					default: // Allow item cycling for fixed lists
						return (attribDef.GetListType() == TDCCA_FIXEDLIST);
					}
				}
			}
			break;
		}
	}

	// Non-edit operations
	switch (nColID)
	{
	case TDCC_REMINDER:
		return !m_calculator.IsTaskDone(dwTaskID);

	case TDCC_FILEREF:
		if (pCursor)
			return (HitTestFileLinkColumn(*pCursor) != -1);
		
		// else
		return m_data.TaskHasFileRef(dwTaskID);
			
	case TDCC_DEPENDENCY:
		return m_data.IsTaskDependent(dwTaskID);
			
	default: // try custom columns
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nColID))
		{
			TDCCUSTOMATTRIBUTEDEFINITION attribDef;
			
			if (m_aCustomAttribDefs.GetAttributeDef(nColID, attribDef))
			{
				switch (attribDef.GetDataType())
				{
				case TDCCA_FILELINK:
					// TODO
					return TRUE;
				}
			}
		}
		break;
	}

	return FALSE;
}

void CTDLTaskCtrlBase::SetModified(const CTDCAttributeMap& mapAttribIDs, BOOL bAllowResort)
{
	if (AttribsMatchSort(mapAttribIDs))
		m_sort.bModSinceLastSort = TRUE;

	// Recalc or redraw columns as required
	BOOL bRedrawCols = FALSE, bRedrawTasks = ModsCauseTaskTextColorChange(mapAttribIDs);
	
	CTDCColumnIDMap aColIDs;
	POSITION pos = mapAttribIDs.GetStartPosition();

	while (pos)
	{
		TDC_ATTRIBUTE nAttribID = mapAttribIDs.GetNext(pos);
		TDC_COLUMN nColID = TDC::MapAttributeToColumn(nAttribID);

		switch (nAttribID)
		{
		case TDCA_CREATIONDATE:
			// this can only be modified for new tasks via the commandline
			// so nothing needs to be done
			break;

		case TDCA_DONEDATE:
		case TDCA_DONETIME:
			{
				bRedrawTasks |= (HasStyle(TDCS_STRIKETHOUGHDONETASKS) ||
								(HasStyle(TDCS_ALLOWTREEITEMCHECKBOX) && !IsColumnShowing(TDCC_DONE)));

				AccumulateRecalcColumn(TDCC_DONEDATE, aColIDs);
				AccumulateRecalcColumn(TDCC_DUEDATE, aColIDs);
				AccumulateRecalcColumn(TDCC_DONE, aColIDs);

				if (HasStyle(TDCS_USEPERCENTDONEINTIMEEST))
					AccumulateRecalcColumn(TDCC_TIMEEST, aColIDs);

				if (!m_sCompletionStatus.IsEmpty())
					AccumulateRecalcColumn(TDCC_STATUS, aColIDs);
			}
			break;

		case TDCA_DUEDATE:
		case TDCA_DUETIME:
			{
				if (!AccumulateRecalcColumn(TDCC_DUEDATE, aColIDs))
					bRedrawCols = IsColumnShowing(TDCC_PRIORITY);
			}
			break;

		case TDCA_PRIORITY:
			if (!bRedrawTasks)
			{
				bRedrawCols = IsColumnShowing(TDCC_PRIORITY);

				if (!bRedrawCols && HasStyle(TDCS_SHOWPERCENTASPROGRESSBAR))
					bRedrawCols = IsColumnShowing(TDCC_PERCENT);
			}
			break;

		case TDCA_ALLOCTO:
		case TDCA_ALLOCBY:
		case TDCA_STATUS:
		case TDCA_VERSION:
		case TDCA_CATEGORY:
		case TDCA_TAGS:
		case TDCA_COST:
		case TDCA_STARTDATE:
		case TDCA_STARTTIME:
		case TDCA_EXTERNALID:
		case TDCA_RECURRENCE:
		case TDCA_FILEREF:
		case TDCA_SUBTASKDONE:
			AccumulateRecalcColumn(nColID, aColIDs);
			break;

		case TDCA_OFFSETTASK:
			AccumulateRecalcColumn(TDCC_STARTDATE, aColIDs);
			AccumulateRecalcColumn(TDCC_DUEDATE, aColIDs);
			break;

		case TDCA_TIMEEST:
			{
				bRedrawCols |= !AccumulateRecalcColumn(TDCC_TIMEEST, aColIDs);

				if (HasStyle(TDCS_CALCREMAININGTIMEBYSPENT))
					bRedrawCols |= !AccumulateRecalcColumn(TDCC_REMAINING, aColIDs);

				if (bRedrawCols)
					bRedrawCols = HasStyle(TDCS_AUTOCALCPERCENTDONE);
			}
			break;

		case TDCA_TIMESPENT:
			{
				bRedrawCols |= !AccumulateRecalcColumn(TDCC_TIMESPENT, aColIDs);

				if (HasStyle(TDCS_CALCREMAININGTIMEBYSPENT))
					bRedrawCols |= !AccumulateRecalcColumn(TDCC_REMAINING, aColIDs);

				if (bRedrawCols)
					bRedrawCols = HasStyle(TDCS_AUTOCALCPERCENTDONE);
			}
			break;

		case TDCA_DEPENDENCY:
		case TDCA_RISK:
		case TDCA_FLAG:
		case TDCA_LOCK:
		case TDCA_PERCENT:
			{
				if (!bRedrawTasks)
					bRedrawCols = IsColumnShowing(nColID);
			}
			break;

		case TDCA_ICON:
			{
				if (IsColumnShowing(TDCC_ICON))
					bRedrawCols = TRUE;
				else
					bRedrawTasks = TRUE;
			}
			break;

		case TDCA_TASKNAME:
			bRedrawCols = IsColumnShowing(TDCC_PATH);
			break;

		case TDCA_PROJECTNAME:
		case TDCA_COMMENTS:
		case TDCA_ENCRYPT:
		case TDCA_METADATA:
			break;

		case TDCA_COLOR:
			bRedrawTasks = TRUE;
			break;

		case TDCA_POSITION:
			ASSERT(0);
			// fallthru

		case TDCA_POSITION_DIFFERENTPARENT:
			{
				AccumulateRecalcColumn(TDCC_POSITION, aColIDs);

				// Add all auto-calculated attributes
				AccumulateRecalcColumn(TDCC_COST, aColIDs);
				AccumulateRecalcColumn(TDCC_TIMEEST, aColIDs);
				AccumulateRecalcColumn(TDCC_TIMESPENT, aColIDs);

				for (int nAttrib = 0; nAttrib < m_aCustomAttribDefs.GetSize(); nAttrib++)
				{
					const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aCustomAttribDefs.GetData()[nAttrib];

					if (attribDef.IsCalculated())
						AccumulateRecalcColumn(attribDef.GetColumnID(), aColIDs);
				}
			}
			break;

		case TDCA_POSITION_SAMEPARENT:		// moved within the same parent
			bRedrawCols = IsColumnShowing(TDCC_POSITION);
			break;

		case TDCA_PASTE:
		case TDCA_MERGE:
		case TDCA_DELETE:
		case TDCA_ARCHIVE:
		case TDCA_UNDO:
		case TDCA_NEWTASK:
		case TDCA_ALL:
			aColIDs.Copy(m_mapVisibleCols);
			break;

		case TDCA_CUSTOMATTRIBDEFS:
			// Handled in OnCustomAttributeChange
			break;

		case TDCA_NONE:
		case TDCA_TASKNAMEORCOMMENTS:
		case TDCA_ANYTEXTATTRIBUTE:
			ASSERT(0);
			break;

		default:
			if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nColID))
				aColIDs.Add(nColID);
			else
				ASSERT(0);
			break;
		}
	}

	if (bAllowResort && ModsNeedResort(mapAttribIDs))
	{
		Resort();
	}

	RecalcUntrackedColumnWidths(aColIDs);
	
	if (bRedrawTasks)
	{
		InvalidateAll();
	}
	else if (bRedrawCols || !aColIDs.IsEmpty())
	{
		m_lcColumns.Invalidate();
	}
}

int CTDLTaskCtrlBase::GetColumnIndices(const CTDCColumnIDMap& aColIDs, CIntArray& aCols) const
{
	aCols.RemoveAll();

	if (!aColIDs.GetCount())
		return 0;

	int nNumCols = m_hdrColumns.GetItemCount();

	for (int nCol = 0; nCol < nNumCols; nCol++)
	{
		if (aColIDs.Has(GetColumnID(nCol)))
		{
			aCols.Add(nCol);

			// can stop when we have everything
			if (aCols.GetSize() == aColIDs.GetCount())
				break;
		}
	}

	return aCols.GetSize();
}

BOOL CTDLTaskCtrlBase::ModsCauseTaskTextColorChange(const CTDCAttributeMap& mapAttribIDs) const
{
	POSITION pos = mapAttribIDs.GetStartPosition();

	while (pos)
	{
		if (ModCausesTaskTextColorChange(mapAttribIDs.GetNext(pos)))
			return TRUE;
	}

	return FALSE;
}

BOOL CTDLTaskCtrlBase::ModCausesTaskTextColorChange(TDC_ATTRIBUTE nModType) const
{
	switch (nModType)
	{
	case TDCA_COLOR:
		return !HasStyle(TDCS_COLORTEXTBYPRIORITY) &&
				!HasStyle(TDCS_COLORTEXTBYATTRIBUTE) &&
				!HasStyle(TDCS_COLORTEXTBYNONE);

	case TDCA_CATEGORY:
	case TDCA_ALLOCBY:
	case TDCA_ALLOCTO:
	case TDCA_STATUS:
	case TDCA_VERSION:
	case TDCA_EXTERNALID:
	case TDCA_TAGS:
		return (HasStyle(TDCS_COLORTEXTBYATTRIBUTE) && (GetColorByAttribute() == nModType));

	case TDCA_DONEDATE:
		return (GetCompletedTaskColor() != CLR_NONE);

	case TDCA_DUEDATE:
		{
			COLORREF crDue, crDueToday;
			GetDueTaskColors(crDue, crDueToday);

			return ((crDue != CLR_NONE) || (crDueToday != CLR_NONE));
		}

	case TDCA_PRIORITY:
		return HasStyle(TDCS_COLORTEXTBYPRIORITY);
	}

	// all else
	return FALSE;
}

BOOL CTDLTaskCtrlBase::AccumulateRecalcColumn(TDC_COLUMN nColID, CSet<TDC_COLUMN>& aColIDs) const
{
	if (aColIDs.Has(nColID))
	{
		return TRUE;
	}
	else if (IsColumnShowing(nColID))
	{
		aColIDs.Add(nColID);
		return TRUE;
	}
	
	// else
	return FALSE;
}

BOOL CTDLTaskCtrlBase::ModsNeedResort(const CTDCAttributeMap& mapAttribIDs) const
{
	if (!HasStyle(TDCS_RESORTONMODIFY))
		return FALSE;
	
	return AttribsMatchSort(mapAttribIDs);
}

BOOL CTDLTaskCtrlBase::AttribsMatchSort(const CTDCAttributeMap& mapAttribIDs) const
{
	POSITION pos = mapAttribIDs.GetStartPosition();

	while (pos)
	{
		if (AttribMatchesSort(mapAttribIDs.GetNext(pos)))
			return TRUE;
	}

	return FALSE;
}

BOOL CTDLTaskCtrlBase::AttribMatchesSort(TDC_ATTRIBUTE nAttrib) const
{
	if (!m_sort.IsSorting())
		return FALSE;

	if (nAttrib == TDCA_ALL)
		return TRUE;
	
	if (m_sort.bMulti)
	{
		for (int nCol = 0; nCol < 3; nCol++)
		{
			if (!m_sort.multi.IsSorting(nCol))
				break;

			if (ModNeedsResort(nAttrib, m_sort.multi.GetSortBy(nCol)))
				return TRUE;
		}

		return FALSE;
	}

	// else
	return ModNeedsResort(nAttrib, m_sort.single.nBy);
}

BOOL CTDLTaskCtrlBase::ModNeedsResort(TDC_ATTRIBUTE nModType, TDC_COLUMN nSortBy) const
{
	if (nSortBy == TDCC_NONE)
		return FALSE;

	TDC_COLUMN nModCol = TDC::MapAttributeToColumn(nModType);

	switch (nModType)
	{
	case TDCA_TASKNAME:		
	case TDCA_STARTDATE:	
	case TDCA_PRIORITY:		
	case TDCA_ALLOCTO:		
	case TDCA_ALLOCBY:		
	case TDCA_STATUS:		
	case TDCA_CATEGORY:		
	case TDCA_TAGS:			
	case TDCA_TIMEEST:		
	case TDCA_RISK:			
	case TDCA_EXTERNALID:	
	case TDCA_VERSION:		
	case TDCA_RECURRENCE:	
	case TDCA_ICON:			
	case TDCA_COLOR:		
	case TDCA_FLAG:			
	case TDCA_DUEDATE:
	case TDCA_PERCENT:
	case TDCA_TIMESPENT:
	case TDCA_DEPENDENCY:
	case TDCA_COST:
	case TDCA_FILEREF:
	case TDCA_POSITION:
	case TDCA_POSITION_SAMEPARENT:
	case TDCA_POSITION_DIFFERENTPARENT:
	case TDCA_LOCK:
		{
			ASSERT(nModCol != TDCC_NONE);

			if (nModCol == nSortBy)
				return TRUE;
		}
		break;

	case TDCA_OFFSETTASK:
		return (ModNeedsResort(TDCA_STARTDATE, nSortBy) ||
				ModNeedsResort(TDCA_DUEDATE, nSortBy)); // RECURSIVE calls

	case TDCA_DONEDATE:
		{
			ASSERT(nModCol != TDCC_NONE);

			if (nModCol == nSortBy)
				return TRUE;

			if (HasStyle(TDCS_SORTDONETASKSATBOTTOM))
			{
				// some sort columns are unaffected by completed tasks
				switch (nSortBy)
				{
				case TDCC_ID:
					return FALSE;
				}

				// all else
				return TRUE;
			}
		}
		break;

	case TDCA_UNDO:
	case TDCA_PASTE:
	case TDCA_MERGE:
		ASSERT(nModCol == TDCC_NONE);
		return TRUE;

	case TDCA_NEWTASK:
	case TDCA_DELETE:
	case TDCA_ARCHIVE:
	case TDCA_ENCRYPT:
	case TDCA_NONE:
	case TDCA_PROJECTNAME:
	case TDCA_COMMENTS:
		ASSERT(nModCol == TDCC_NONE);
		return FALSE;

	case TDCA_METADATA:
		return FALSE;

	case TDCA_CUSTOMATTRIBDEFS:	// Resort all custom columns
		ASSERT(nModCol == TDCC_NONE);
		return TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nSortBy);

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nModType))
		{
			ASSERT(m_aCustomAttribDefs.IsColumnSortable(nModCol));
			return (nModCol == nSortBy);
		}
		// else unhandled attribute
		ASSERT(0);
		return FALSE;
	}

	ASSERT(nModCol != TDCC_NONE);

	// Attribute interdependencies
	switch (nSortBy)
	{
	case TDCC_DONE:
	case TDCC_STARTDATE:
	case TDCC_DUEDATE:
	case TDCC_PERCENT:
		return (nModType == TDCA_DONEDATE);
		
	case TDCC_RISK:
		return ((nModType == TDCA_DONEDATE) && HasStyle(TDCS_DONEHAVELOWESTRISK));
		
	case TDCC_PRIORITY:
		return (((nModType == TDCA_DONEDATE) && HasStyle(TDCS_DONEHAVELOWESTPRIORITY)) ||
				((nModType == TDCA_DUEDATE) && HasStyle(TDCS_DUEHAVEHIGHESTPRIORITY)));
		
	case TDCC_REMAINING: 
		return (((nModType == TDCA_DUEDATE) && HasStyle(TDCS_CALCREMAININGTIMEBYDUEDATE)) ||
				((nModType == TDCA_TIMESPENT) && HasStyle(TDCS_CALCREMAININGTIMEBYSPENT)) ||
				((nModType == TDCA_PERCENT) && HasStyle(TDCS_CALCREMAININGTIMEBYPERCENT)));
	}

	// all else
	return FALSE;
}

void CTDLTaskCtrlBase::OnReminderChange()
{
	if (IsColumnShowing(TDCC_REMINDER))
	{
		if (IsSortingBy(TDCC_REMINDER))
			Resort();
		else
			RedrawColumn(TDCC_REMINDER);
	}
}

void CTDLTaskCtrlBase::SetAlternateLineColor(COLORREF crAltLine)
{
	SetColor(m_crAltLine, crAltLine);
}

void CTDLTaskCtrlBase::SetGridlineColor(COLORREF crGridLine)
{
	SetColor(m_crGridLine, crGridLine);
}

void CTDLTaskCtrlBase::SetColor(COLORREF& color, COLORREF crNew)
{
	if (IsHooked() && (crNew != color))
		InvalidateAll(FALSE);
	
	color = crNew;
}

void CTDLTaskCtrlBase::RedrawColumns(BOOL bErase) const
{
	::InvalidateRect(m_lcColumns, NULL, bErase);
	::UpdateWindow(m_lcColumns);
}

void CTDLTaskCtrlBase::RedrawColumn(TDC_COLUMN nColID) const
{
	int nCol = GetColumnIndex(nColID);

	if (m_hdrColumns.IsItemVisible(nCol))
	{
		CRect rCol, rClient;
		
		m_lcColumns.GetClientRect(rClient);
		m_hdrColumns.GetItemRect(nCol, rCol);

		// Adjust header rect for list scrollpos
		m_hdrColumns.ClientToScreen(rCol);
		m_lcColumns.ScreenToClient(rCol);
		
		rCol.top = rClient.top;
		rCol.bottom = rClient.bottom;
		
		::InvalidateRect(m_lcColumns, rCol, TRUE);
		::UpdateWindow(m_lcColumns);
	}
}

void CTDLTaskCtrlBase::RedrawTasks(BOOL bErase) const
{
	::InvalidateRect(Tasks(), NULL, bErase);
	::UpdateWindow(Tasks());
}

int CTDLTaskCtrlBase::CalcMaxDateColWidth(TDC_DATE nDate, CDC* pDC, BOOL bCustomWantsTime) const
{
	COleDateTime dateMax(2000, 12, 31, 23, 59, 0);
	CString sDateMax, sTimeMax, sDow;

	VERIFY(FormatDate(dateMax, nDate, sDateMax, sTimeMax, sDow, bCustomWantsTime));

	// Always want date
	int nSpace = pDC->GetTextExtent(_T(" ")).cx;
	int nWidth = pDC->GetTextExtent(sDateMax).cx;

	if (!sTimeMax.IsEmpty())
	{
		nWidth += nSpace;
		nWidth += pDC->GetTextExtent(sTimeMax).cx;
	}

	if (!sDow.IsEmpty())
	{
		nWidth += nSpace;
		nWidth += CDateHelper::GetMaxDayOfWeekNameWidth(pDC, TRUE);
	}

	return nWidth;
}

BOOL CTDLTaskCtrlBase::WantDrawColumnTime(TDC_DATE nDate, BOOL bCustomWantsTime) const
{
	switch (nDate)
	{
	case TDCD_CREATE:
		return IsColumnShowing(TDCC_CREATIONTIME);

	case TDCD_START:		
	case TDCD_STARTDATE:	
	case TDCD_STARTTIME:
		return IsColumnShowing(TDCC_STARTTIME);
		
	case TDCD_DUE:		
	case TDCD_DUEDATE:	
	case TDCD_DUETIME:	
		return IsColumnShowing(TDCC_DUETIME);
		
	case TDCD_DONE:		
	case TDCD_DONEDATE:	
	case TDCD_DONETIME:	
		return IsColumnShowing(TDCC_DONETIME);
		
	case TDCD_CUSTOM:
		return bCustomWantsTime;
		
	case TDCD_LASTMOD:
	case TDCD_REMINDER:
		return TRUE; // always
	}
	
	// all else
	ASSERT(0);
	return FALSE;
}

void CTDLTaskCtrlBase::ClearSortColumn()
{
	if ((m_nSortColID != TDCC_NONE) && GetSafeHwnd())
	{
		::InvalidateRect(m_hdrTasks, NULL, FALSE);
		m_hdrColumns.Invalidate(FALSE);
	}

	m_nSortColID = TDCC_NONE;
	m_nSortDir = TDC_SORTNONE;
}

void CTDLTaskCtrlBase::SetSortColumn(TDC_COLUMN nColID, TDC_SORTDIR nSortDir)
{
	m_nSortDir = nSortDir;

	if (m_nSortColID != nColID)
	{
		if ((m_nSortColID == TDCC_CLIENT) || (nColID == TDCC_CLIENT))
			m_hdrTasks.Invalidate(FALSE);

		m_nSortColID = nColID;
	}
}

TDC_COLUMN CTDLTaskCtrlBase::GetSortColumn(TDC_SORTDIR& nSortDir) const
{
	nSortDir = m_nSortDir;
	return m_nSortColID;
}

int CTDLTaskCtrlBase::CalcColumnWidth(int nCol, CDC* pDC, BOOL bVisibleTasksOnly) const
{
	TDC_COLUMN nColID = GetColumnID(nCol);

	// handle hidden columns
	if (!IsColumnShowing(nColID))
 		return 0;
	
	int nColWidth = 0; // equivalent to MINCOLWIDTH
	
	switch (nColID)
	{
	case TDCC_TRACKTIME:
	case TDCC_FLAG:
	case TDCC_LOCK:
	case TDCC_RECENTEDIT:
	case TDCC_DEPENDENCY:
	case TDCC_ICON:
	case TDCC_DONE:
		// use MINCOLWIDTH
		break; 
		
	case TDCC_REMINDER:
		if (HasStyle(TDCS_SHOWREMINDERSASDATEANDTIME))
			nColWidth = CalcMaxDateColWidth(TDCD_REMINDER, pDC);
		// else use MINCOLWIDTH
		break; 
		
	case TDCC_ID:
		{
			DWORD dwRefID = m_find.GetLargestReferenceID(bVisibleTasksOnly);
			nColWidth = GraphicsMisc::GetTextWidth(pDC, m_formatter.GetID(m_dwNextUniqueTaskID - 1, dwRefID));
		}
		break; 

	case TDCC_PARENTID:
		nColWidth = GraphicsMisc::GetTextWidth(pDC, m_formatter.GetID(m_dwNextUniqueTaskID - 1));
		break; 

	case TDCC_POSITION:
	case TDCC_RECURRENCE:
	case TDCC_EXTERNALID:
	case TDCC_CREATEDBY:
	case TDCC_PATH:
	case TDCC_SUBTASKDONE:
	case TDCC_LASTMODBY:
	case TDCC_COMMENTSSIZE:
		{
			// determine the longest visible string
			CString sLongest = m_find.GetLongestValue(nColID, bVisibleTasksOnly);
			nColWidth = GraphicsMisc::GetAverageMaxStringWidth(sLongest, pDC);
		}
		break;
		
	case TDCC_COST:
		{
			// determine the longest visible string
			CString sLongest = m_find.GetLongestCost();
			nColWidth = GraphicsMisc::GetTextWidth(pDC, sLongest);
		}
		break;
		
	case TDCC_ALLOCTO:
		{
			// determine the longest visible string
			CString sLongest = m_find.GetLongestValue(nColID, m_tld.aAllocTo, bVisibleTasksOnly);
			nColWidth = GraphicsMisc::GetAverageMaxStringWidth(sLongest, pDC);
		}
		break;
		
	case TDCC_ALLOCBY:
		{
			// determine the longest visible string
			CString sLongest = m_find.GetLongestValue(nColID, m_tld.aAllocBy, bVisibleTasksOnly);
			nColWidth = GraphicsMisc::GetAverageMaxStringWidth(sLongest, pDC);
		}
		break;
		
	case TDCC_CATEGORY:
		{
			// determine the longest visible string
			CString sLongest = m_find.GetLongestValue(nColID, m_tld.aCategory, bVisibleTasksOnly);
			nColWidth = GraphicsMisc::GetAverageMaxStringWidth(sLongest, pDC);
		}
		break;
		
	case TDCC_TAGS:
		{
			// determine the longest visible string
			CString sLongest = m_find.GetLongestValue(nColID, m_tld.aTags, bVisibleTasksOnly);
			nColWidth = GraphicsMisc::GetAverageMaxStringWidth(sLongest, pDC);
		}
		break;
		
	case TDCC_STATUS:
		{
			// determine the longest visible string
			CString sLongest = m_find.GetLongestValue(nColID, m_tld.aStatus, bVisibleTasksOnly);
			nColWidth = GraphicsMisc::GetAverageMaxStringWidth(sLongest, pDC);
		}
		break;
		
	case TDCC_VERSION:
		{
			// determine the longest visible string
			CString sLongest = m_find.GetLongestValue(nColID, m_tld.aVersion, bVisibleTasksOnly);
			nColWidth = GraphicsMisc::GetAverageMaxStringWidth(sLongest, pDC);
		}
		break;
		
	case TDCC_PRIORITY:
		if (!HasStyle(TDCS_HIDEPRIORITYNUMBER))
			nColWidth = pDC->GetTextExtent("10").cx;
		break; 
		
	case TDCC_RISK:
		nColWidth = pDC->GetTextExtent("10").cx;
		break; 
		
	case TDCC_FILEREF:
		{
			int nMaxCount = m_find.GetLargestFileLinkCount(bVisibleTasksOnly);

			if (nMaxCount >= 1)
			{
				nColWidth = ((nMaxCount * (CFileIcons::GetImageSize() + COL_ICON_SPACING)) - COL_ICON_SPACING);

				// compensate for extra padding we don't want 
				nColWidth -= (2 * LV_COLPADDING);
			}
			// else use MINCOLWIDTH
		}
		break; 
		
	case TDCC_TIMEEST:
	case TDCC_TIMESPENT:
	case TDCC_REMAINING:
		if (HasStyle(TDCS_DISPLAYHMSTIMEFORMAT))
		{
			nColWidth = pDC->GetTextExtent("-12m4w").cx;
		}
		else
		{
			CString sLongest;

			switch (nColID)
			{
			case TDCC_TIMEEST:		sLongest = m_find.GetLongestTimeEstimate(bVisibleTasksOnly);	break;
			case TDCC_TIMESPENT:	sLongest = m_find.GetLongestTimeSpent(bVisibleTasksOnly);		break;
			case TDCC_REMAINING: 	sLongest = m_find.GetLongestTimeRemaining(bVisibleTasksOnly);	break;
			}

			nColWidth = (pDC->GetTextExtent(sLongest).cx + 4); // add a bit to handle different time unit widths
		}
		break;
		
	case TDCC_PERCENT:
		nColWidth = pDC->GetTextExtent("100%").cx;
		break;
		
	case TDCC_LASTMODDATE:
	case TDCC_DUEDATE:
	case TDCC_CREATIONDATE:
	case TDCC_STARTDATE:
	case TDCC_DONEDATE:
		nColWidth = CalcMaxDateColWidth(TDC::MapColumnToDate(nColID), pDC);
		break;

	default:
		nColWidth = CalcMaxCustomAttributeColWidth(nColID, pDC, bVisibleTasksOnly);
		break;
	}

	if (nColWidth == 0)
		nColWidth = MIN_RESIZE_WIDTH;
	else
		nColWidth = max((nColWidth + (2 * LV_COLPADDING)), MIN_RESIZE_WIDTH);
	
	// take max of this and column title
	int nTitleWidth = (m_hdrColumns.GetItemTextWidth(nCol, pDC) + (2 * HD_COLPADDING));

	return max(nTitleWidth, nColWidth);
}

int CTDLTaskCtrlBase::CalcMaxCustomAttributeColWidth(TDC_COLUMN nColID, CDC* pDC, BOOL bVisibleTasksOnly) const
{
	if (!TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nColID))
	{
		ASSERT(0);
		return 0;
	}

	// determine the longest visible string depending on type
	TDCCUSTOMATTRIBUTEDEFINITION attribDef;

	if (m_aCustomAttribDefs.GetAttributeDef(nColID, attribDef))
	{
		if (!attribDef.bEnabled)
		{
			return 0; // hidden
		}
		else
		{
			switch (attribDef.GetDataType())
			{
			case TDCCA_DATE:
				return CalcMaxDateColWidth(TDCD_CUSTOM, pDC, attribDef.HasFeature(TDCCAF_SHOWTIME));

			case TDCCA_ICON:
				if (attribDef.IsList())
				{
					switch (attribDef.GetListType())
					{
					case TDCCA_FIXEDLIST:
						return attribDef.CalcLongestListItem(pDC);

					case TDCCA_FIXEDMULTILIST:
						{
							int nNumIcons = m_find.GetLargestCustomAttributeArraySize(attribDef, bVisibleTasksOnly);
							return ((nNumIcons * (COL_ICON_SIZE + COL_ICON_SPACING)) - COL_ICON_SPACING);
						}
					}
				}
				// else single icon, no text: use MINCOLWIDTH
				return COL_ICON_SIZE;

			case TDCCA_FRACTION:
			case TDCCA_DOUBLE:
			case TDCCA_INTEGER:
				{
					// numerals are always the same width so we don't need average width
					CString sLongest = m_find.GetLongestValue(attribDef, bVisibleTasksOnly);
					return pDC->GetTextExtent(sLongest).cx;
				}
				break;

			case TDCCA_BOOL:
			case TDCCA_FILELINK:
				return COL_ICON_SIZE;

			default:
				{
					CString sLongest = m_find.GetLongestValue(attribDef, bVisibleTasksOnly);
					return GraphicsMisc::GetAverageMaxStringWidth(sLongest, pDC);
				}
				break;
			}
		}
	}

	// all else
	return 0;
}

BOOL CTDLTaskCtrlBase::SelectionHasIncompleteDependencies(CString& sIncomplete) const
{
	POSITION pos = GetFirstSelectedTaskPos();
	
	while (pos)
	{
		DWORD dwTaskID = GetNextSelectedTaskID(pos);

		if (TaskHasIncompleteDependencies(dwTaskID, sIncomplete))
			return TRUE;
	}

	return FALSE;
}

BOOL CTDLTaskCtrlBase::SelectionHasDependencies() const
{
	POSITION pos = GetFirstSelectedTaskPos();
	CString sUnused;
	
	while (pos)
	{
		DWORD dwTaskID = GetNextSelectedTaskID(pos);
		
		if (m_data.TaskHasDependencies(dwTaskID))
			return TRUE;
	}
	
	return FALSE;
}

BOOL CTDLTaskCtrlBase::TaskHasIncompleteDependencies(DWORD dwTaskID, CString& sIncomplete) const
{
	CStringArray aDepends;
	int nNumDepends = m_data.GetTaskDependencies(dwTaskID, aDepends);
	
	for (int nDepends = 0; nDepends < nNumDepends; nDepends++)
	{
		CString sFile;
		DWORD dwDependID;
		
		VERIFY(TODOITEM::ParseTaskLink(aDepends[nDepends], FALSE, m_sTasklistFolder, dwDependID, sFile));
		
		// see if dependent is one of 'our' tasks
		if (dwDependID && sFile.IsEmpty())
		{
			if (m_data.HasTask(dwDependID) && !m_data.IsTaskDone(dwDependID))
			{
				sIncomplete = aDepends[nDepends];
				return TRUE;
			}
		}
		else if (!sFile.IsEmpty()) // pass to parent if we can't handle
		{
			BOOL bDependentIsDone = CWnd::GetParent()->SendMessage(WM_TDCM_ISTASKDONE, dwDependID, (LPARAM)(LPCTSTR)sFile);
			
			if (!bDependentIsDone)
			{
				sIncomplete = aDepends[nDepends];
				return TRUE;
			}
		}
	}
	
	// check this tasks subtasks
	const TODOSTRUCTURE* pTDS = m_data.LocateTask(dwTaskID);
	ASSERT(pTDS);
	
	if (pTDS && pTDS->HasSubTasks())
	{
		int nPos = pTDS->GetSubTaskCount();
		
		while (nPos--)
		{
			if (TaskHasIncompleteDependencies(pTDS->GetSubTaskID(nPos), sIncomplete))
				return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CTDLTaskCtrlBase::SelectionHasDates(TDC_DATE nDate, BOOL bAll) const
{
	POSITION pos = GetFirstSelectedTaskPos();
	
	// traverse the selected items looking for the first one
	// who has a due date or the first that doesn't
	while (pos)
	{
		DWORD dwTaskID = GetNextSelectedTaskID(pos);
		double dDate = m_data.GetTaskDate(dwTaskID, nDate);

		if (!bAll && dDate > 0)
		{
			return TRUE;
		}
		else if (bAll && dDate == 0)
		{
			return FALSE;
		}
	}

	return bAll;
}

BOOL CTDLTaskCtrlBase::SelectionHasIcons() const
{
	POSITION pos = GetFirstSelectedTaskPos();
	
	while (pos)
	{
		DWORD dwTaskID = GetNextSelectedTaskID(pos);

		if (!m_data.GetTaskIcon(dwTaskID).IsEmpty())
			return TRUE;
	}

	return FALSE;
}

BOOL CTDLTaskCtrlBase::SelectionHasUnlocked(BOOL bTreatRefsAsUnlocked) const
{
	POSITION pos = GetFirstSelectedTaskPos();
	
	while (pos)
	{
		DWORD dwTaskID = GetNextSelectedTaskID(pos);

		if (bTreatRefsAsUnlocked && m_data.IsTaskReference(dwTaskID))
			return TRUE;

		if (!m_calculator.IsTaskLocked(dwTaskID))
			return TRUE;
	}

	return FALSE;
}

BOOL CTDLTaskCtrlBase::SelectionHasLocked(BOOL bTreatRefsAsUnlocked) const
{
	POSITION pos = GetFirstSelectedTaskPos();
	
	while (pos)
	{
		DWORD dwTaskID = GetNextSelectedTaskID(pos);

		if (bTreatRefsAsUnlocked && m_data.IsTaskReference(dwTaskID))
			continue;

		if (m_calculator.IsTaskLocked(dwTaskID))
			return TRUE;
	}

	return FALSE;
}

BOOL CTDLTaskCtrlBase::SelectionHasLockedParent(BOOL bTreatRefsAsUnlocked) const
{
	POSITION pos = GetFirstSelectedTaskPos();

	while (pos)
	{
		DWORD dwTaskID = GetNextSelectedTaskID(pos);
		DWORD dwParentID = m_data.GetTaskParentID(dwTaskID);

		// Root is always unlocked
		if (!dwParentID)
			continue;

		if (bTreatRefsAsUnlocked && m_data.IsTaskReference(dwParentID))
			continue;

		if (m_calculator.IsTaskLocked(dwParentID))
			return TRUE;
	}

	return FALSE;
}

BOOL CTDLTaskCtrlBase::SelectionAreAllDone() const
{
	POSITION pos = GetFirstSelectedTaskPos();
	
	while (pos)
	{
		DWORD dwTaskID = GetNextSelectedTaskID(pos);

		if (!m_data.IsTaskDone(dwTaskID))
			return FALSE;
	}

	return TRUE;
}

int CTDLTaskCtrlBase::SelectionHasCircularDependencies(CDWordArray& aTaskIDs) const
{
	aTaskIDs.RemoveAll();

	POSITION pos = GetFirstSelectedTaskPos();

	while (pos)
	{
		DWORD dwTaskID = GetNextSelectedTaskID(pos);

		if (m_data.TaskHasLocalCircularDependencies(dwTaskID))
		{
			aTaskIDs.Add(dwTaskID);
			return TRUE;
		}
	}

	return aTaskIDs.GetSize();
}

BOOL CTDLTaskCtrlBase::SelectionHasDependents() const
{
	POSITION pos = GetFirstSelectedTaskPos();
	
	while (pos)
	{
		DWORD dwTaskID = GetNextSelectedTaskID(pos);
		
		if (m_data.TaskHasDependents(dwTaskID))
			return TRUE;
	}
	
	return FALSE;
}

BOOL CTDLTaskCtrlBase::SelectionHasRecurring() const
{
	POSITION pos = GetFirstSelectedTaskPos();
	
	while (pos)
	{
		DWORD dwTaskID = GetNextSelectedTaskID(pos);
		
		if (m_data.IsTaskRecurring(dwTaskID))
			return TRUE;
	}
	
	return FALSE;
}

BOOL CTDLTaskCtrlBase::SelectionHasReferences() const
{
	POSITION pos = GetFirstSelectedTaskPos();
	
	while (pos)
	{
		DWORD dwTaskID = GetNextSelectedTaskID(pos);
		
		if (m_data.IsTaskReference(dwTaskID))
			return TRUE;
	}
	
	return FALSE;
}

BOOL CTDLTaskCtrlBase::SelectionHasTask(DWORD dwTaskID, BOOL bIncludeRefs) const
{
	BOOL bSelected = IsTaskSelected(dwTaskID);

	if (bSelected || !bIncludeRefs)
		return bSelected;

	POSITION pos = GetFirstSelectedTaskPos();
	dwTaskID = m_data.GetTrueTaskID(dwTaskID);

	while (pos)
	{
		DWORD dwSelTaskID = GetNextSelectedTaskID(pos);

		if (m_data.GetTrueTaskID(dwSelTaskID) == dwTaskID)
			return TRUE;
	}

	return FALSE;
}

BOOL CTDLTaskCtrlBase::SelectionHasSameParent() const
{
	switch (GetSelectedCount())
	{
	case 0:
		ASSERT(0);
		return FALSE;

	case 1:
		return TRUE;

	default:
		{
			DWORD dwFirstParent = (DWORD)-1;
			POSITION pos = GetFirstSelectedTaskPos();

			while (pos)
			{
				DWORD dwTaskID = GetNextSelectedTaskID(pos);

				if (dwFirstParent == -1)
				{
					dwFirstParent = m_data.GetTaskParentID(dwTaskID);
				}
				else if (dwFirstParent != m_data.GetTaskParentID(dwTaskID))
				{
					return FALSE;
				}
			}
		}
		break;
	}

	return TRUE;
}

BOOL CTDLTaskCtrlBase::SelectionHasNonReferences() const
{
	POSITION pos = GetFirstSelectedTaskPos();
	
	while (pos)
	{
		DWORD dwTaskID = GetNextSelectedTaskID(pos);
		
		if (!m_data.IsTaskReference(dwTaskID))
			return TRUE;
	}
	
	return FALSE;
}

BOOL CTDLTaskCtrlBase::SelectionHasIncompleteSubtasks(BOOL bExcludeRecurring) const
{
	POSITION pos = GetFirstSelectedTaskPos();
	
	while (pos)
	{
		DWORD dwTaskID = GetNextSelectedTaskID(pos);
		
		if (m_data.TaskHasIncompleteSubtasks(dwTaskID, bExcludeRecurring))
			return TRUE;
	}
	
	return FALSE;
}

BOOL CTDLTaskCtrlBase::SelectionHasSubtasks() const
{
	POSITION pos = GetFirstSelectedTaskPos();
	
	while (pos)
	{
		DWORD dwTaskID = GetNextSelectedTaskID(pos);

		const TODOSTRUCTURE* pTDS = m_data.LocateTask(dwTaskID);
		ASSERT(pTDS);

		if (pTDS && pTDS->HasSubTasks())
			return TRUE;
	}
	
	return FALSE;
}

void CTDLTaskCtrlBase::EnableExtendedSelection(BOOL bCtrl, BOOL bShift)
{
	if (bCtrl)
		s_nExtendedSelection |= HOTKEYF_CONTROL;
	else
		s_nExtendedSelection &= ~HOTKEYF_CONTROL;
	
	if (bShift)
		s_nExtendedSelection |= HOTKEYF_SHIFT;
	else
		s_nExtendedSelection &= ~HOTKEYF_SHIFT;
}

BOOL CTDLTaskCtrlBase::IsReservedShortcut(DWORD dwShortcut)
{
	// check this is not a reserved shortcut used by the tree or a.n.other ctrl
	switch (dwShortcut)
	{
	case MAKELONG(VK_UP, HOTKEYF_EXT):
	case MAKELONG(VK_PRIOR, HOTKEYF_EXT):
	case MAKELONG(VK_DOWN, HOTKEYF_EXT):
	case MAKELONG(VK_NEXT, HOTKEYF_EXT):
		
	case MAKELONG(VK_SPACE, HOTKEYF_CONTROL):
	case MAKELONG(VK_DELETE, HOTKEYF_CONTROL | HOTKEYF_EXT):
		return TRUE;
		
	case MAKELONG(VK_UP, HOTKEYF_CONTROL | HOTKEYF_EXT):
	case MAKELONG(VK_PRIOR, HOTKEYF_CONTROL | HOTKEYF_EXT):
	case MAKELONG(VK_DOWN, HOTKEYF_CONTROL | HOTKEYF_EXT):
	case MAKELONG(VK_NEXT, HOTKEYF_CONTROL | HOTKEYF_EXT):
		return (s_nExtendedSelection & HOTKEYF_CONTROL);
		
	case MAKELONG(VK_UP, HOTKEYF_SHIFT | HOTKEYF_EXT):
	case MAKELONG(VK_PRIOR, HOTKEYF_SHIFT | HOTKEYF_EXT):
	case MAKELONG(VK_DOWN, HOTKEYF_SHIFT | HOTKEYF_EXT):
	case MAKELONG(VK_NEXT, HOTKEYF_SHIFT | HOTKEYF_EXT):
		return (s_nExtendedSelection & HOTKEYF_SHIFT);
		
	case MAKELONG(VK_UP, HOTKEYF_SHIFT | HOTKEYF_CONTROL | HOTKEYF_EXT):
	case MAKELONG(VK_PRIOR, HOTKEYF_SHIFT | HOTKEYF_CONTROL | HOTKEYF_EXT):
	case MAKELONG(VK_DOWN, HOTKEYF_SHIFT | HOTKEYF_CONTROL | HOTKEYF_EXT):
	case MAKELONG(VK_NEXT, HOTKEYF_SHIFT | HOTKEYF_CONTROL | HOTKEYF_EXT):
		return (s_nExtendedSelection & (HOTKEYF_CONTROL | HOTKEYF_SHIFT)); // both
	}
	
	// all else
	return FALSE;
}

CString CTDLTaskCtrlBase::GetSelectedTaskComments() const
{
	if (GetSelectedCount() == 1)
		return m_data.GetTaskComments(GetSelectedTaskID());
	
	// else
	return EMPTY_STR;
}

const CBinaryData& CTDLTaskCtrlBase::GetSelectedTaskCustomComments(CONTENTFORMAT& cfComments) const
{
	if (GetSelectedCount() == 1)
		return m_data.GetTaskCustomComments(GetSelectedTaskID(), cfComments);
	
	// else
	cfComments.Empty();
    POSITION pos = GetFirstSelectedTaskPos();
	
    while (pos)
    {
		DWORD dwTaskID = GetNextSelectedTaskID(pos);
		
		const TODOITEM* pTDI = m_data.GetTrueTask(dwTaskID);
		ASSERT (pTDI);
		
		if (pTDI)
		{
			if (cfComments.IsEmpty())
			{
				cfComments = pTDI->cfComments;
			}
			else if (cfComments != pTDI->cfComments)
			{
				cfComments.Empty();
				break;
			}
		}
	}
	
	static CBinaryData content;
	return content;
}

CString CTDLTaskCtrlBase::GetSelectedTaskTitle() const
{
	if (GetSelectedCount() == 1)
		return m_data.GetTaskTitle(GetSelectedTaskID());
	
	// else
	return EMPTY_STR;
}

int CTDLTaskCtrlBase::GetSelectedTaskPriority() const
{
	int nPriority = -1;
	POSITION pos = GetFirstSelectedTaskPos();

	while (pos)
	{
		DWORD dwTaskID = GetNextSelectedTaskID(pos);
		int nTaskPriority = m_data.GetTaskPriority(dwTaskID);

		if (nPriority == -1)
		{
			nPriority = nTaskPriority;
		}
		else if (nPriority != nTaskPriority)
		{
			return -1;
		}
	}

	return nPriority;
}

DWORD CTDLTaskCtrlBase::GetSelectedTaskParentID() const
{
	// If multiple tasks are selected they must all
	// have the same parent else we return 0
	POSITION pos = GetFirstSelectedTaskPos();

	if (!pos)
		return 0;
	
	DWORD dwParentID = (DWORD)-1;

	while (pos)
	{
		DWORD dwTaskID = GetNextSelectedTaskID(pos);
		DWORD dwTaskParentID = m_data.GetTaskParentID(dwTaskID);
		
		if (dwParentID == (DWORD)-1)
		{
			dwParentID = dwTaskParentID;
		}
		else if (dwParentID != dwTaskParentID)
		{
			return 0;
		}
	}
	
	return dwParentID;
}

int CTDLTaskCtrlBase::GetSelectedTaskRisk() const
{
	int nRisk = -1;
	POSITION pos = GetFirstSelectedTaskPos();

	while (pos)
	{
		DWORD dwTaskID = GetNextSelectedTaskID(pos);
		int nTaskRisk = m_data.GetTaskRisk(dwTaskID);

		if (nRisk == -1)
		{
			nRisk = nTaskRisk;
		}
		else if (nRisk != nTaskRisk)
		{
			return -1; // == various
		}
	}

	return nRisk;
}

CString CTDLTaskCtrlBase::GetSelectedTaskIcon() const
{
	CString sIcon;
	
	if (GetSelectedCount())
	{
		// get first item's value as initial
		POSITION pos = GetFirstSelectedTaskPos();
		DWORD dwTaskID = GetNextSelectedTaskID(pos);

		sIcon = m_data.GetTaskIcon(dwTaskID);
		
		while (pos)
		{
			dwTaskID = GetNextSelectedTaskID(pos);
			CString sTaskIcon = m_data.GetTaskIcon(dwTaskID);
			
			if (sIcon != sTaskIcon)
				return EMPTY_STR;
		}
	}
	
	return sIcon;
}

BOOL CTDLTaskCtrlBase::SelectedTaskHasDate(TDC_DATE nDate) const
{
	return CDateHelper::IsDateSet(GetSelectedTaskDate(nDate));
}

COleDateTime CTDLTaskCtrlBase::GetSelectedTaskDate(TDC_DATE nDate) const
{
	COleDateTime date; // == 0
	
	if (GetSelectedCount())
	{
		// get first item's value as initial
		POSITION pos = GetFirstSelectedTaskPos();
		DWORD dwTaskID = GetNextSelectedTaskID(pos);

		date = m_data.GetTaskDate(dwTaskID, nDate);
		
		while (pos)
		{
			dwTaskID = GetNextSelectedTaskID(pos);
			COleDateTime dateTask = m_data.GetTaskDate(dwTaskID, nDate);
			
			// first check if both dates are not set
			if (!CDateHelper::IsDateSet(date) && !CDateHelper::IsDateSet(dateTask))
				continue;

			if (!CDateHelper::IsDateSet(date)) // means dateTask.m_dt != 0
				return dateTask;

			// else 
			return date;
		}
		// if we get here then no dates were set
	}
	
	return date;
}

int CTDLTaskCtrlBase::IsSelectedTaskFlagged() const
{
	return m_data.IsTaskFlagged(GetSelectedTaskID());
}

int CTDLTaskCtrlBase::IsSelectedTaskLocked() const
{
	return m_data.IsTaskLocked(GetSelectedTaskID());
}

BOOL CTDLTaskCtrlBase::IsSelectedTaskReference() const
{
	return m_data.IsTaskReference(GetSelectedTaskID());
}

BOOL CTDLTaskCtrlBase::GetSelectedTaskTimeEstimate(TDCTIMEPERIOD& timeEst) const
{
	if (!GetSelectedCount())
		return FALSE;

	// get first item's value as initial
	POSITION pos = GetFirstSelectedTaskPos();
	DWORD dwTaskID = GetNextSelectedTaskID(pos);

	m_data.GetTaskTimeEstimate(dwTaskID, timeEst);

	while (pos)
	{
		dwTaskID = GetNextSelectedTaskID(pos);

		TDCTIMEPERIOD time;
		m_data.GetTaskTimeEstimate(dwTaskID, time);

		if (time != timeEst)
		{
			timeEst.dAmount = 0.0;
			return FALSE;
		}
	}
	
	return TRUE;
}

BOOL CTDLTaskCtrlBase::GetSelectedTaskTimeSpent(TDCTIMEPERIOD& timeSpent) const
{
	if (!GetSelectedCount())
		return FALSE;

	// get first item's value as initial
	POSITION pos = GetFirstSelectedTaskPos();
	DWORD dwTaskID = GetNextSelectedTaskID(pos);

	m_data.GetTaskTimeSpent(dwTaskID, timeSpent);

	while (pos)
	{
		dwTaskID = GetNextSelectedTaskID(pos);

		TDCTIMEPERIOD time;
		m_data.GetTaskTimeSpent(dwTaskID, time);

		if (time != timeSpent)
		{
			timeSpent.dAmount = 0.0;
			return FALSE;
		}
	}

	return TRUE;
}

COLORREF CTDLTaskCtrlBase::GetSelectedTaskColor() const
{
	return m_data.GetTaskColor(GetSelectedTaskID());
}

BOOL CTDLTaskCtrlBase::GetSelectedTaskRecurrence(TDCRECURRENCE& tr) const
{
	if (GetSelectedCount())
	{
		// get first item's value as initial
		POSITION pos = GetFirstSelectedTaskPos();
		DWORD dwTaskID = GetNextSelectedTaskID(pos);
		
		m_data.GetTaskRecurrence(dwTaskID, tr);
		
		while (pos)
		{
			dwTaskID = GetNextSelectedTaskID(pos);
			
			TDCRECURRENCE trTask;
			m_data.GetTaskRecurrence(dwTaskID, trTask);
			
			if (tr != trTask)
				tr = TDCRECURRENCE();
		}
	}
	
	return tr.IsRecurring();
}

int CTDLTaskCtrlBase::GetSelectedTaskPercent() const
{
	int nPercent = 0;
	
	if (GetSelectedCount())
	{
		// get first item's value as initial
		POSITION pos = GetFirstSelectedTaskPos();
		DWORD dwTaskID = GetNextSelectedTaskID(pos);

		nPercent = m_data.GetTaskPercent(dwTaskID, FALSE);
		
		while (pos)
		{
			dwTaskID = GetNextSelectedTaskID(pos);
			int nTaskPercent = m_data.GetTaskPercent(dwTaskID, FALSE);
			
			if (nPercent != nTaskPercent)
				return -1;
		}
	}
	
	return nPercent;
}

CString CTDLTaskCtrlBase::GetSelectedTaskPath(BOOL bIncludeTaskName, int nMaxLen) const
{
	CString sPath;

	if (GetSelectedCount() == 1)
	{
		DWORD dwTaskID = GetSelectedTaskID();
		CString sTaskTitle = bIncludeTaskName ? m_data.GetTaskTitle(dwTaskID) : EMPTY_STR;

		if (bIncludeTaskName && nMaxLen != -1)
			nMaxLen -= sTaskTitle.GetLength();

		sPath = m_formatter.GetTaskPath(dwTaskID, nMaxLen);
	
		if (bIncludeTaskName)
			sPath += sTaskTitle;
	}

	return sPath;
}

BOOL CTDLTaskCtrlBase::GetSelectedTaskCost(TDCCOST& cost) const
{
	if (GetSelectedCount())
	{
		// get first item's value as initial
		POSITION pos = GetFirstSelectedTaskPos();

		if (pos)
		{
			DWORD dwTaskID = GetNextSelectedTaskID(pos);

			VERIFY(m_data.GetTaskCost(dwTaskID, cost));
		
			while (pos)
			{
				dwTaskID = GetNextSelectedTaskID(pos);

				TDCCOST taskCost;
				VERIFY(m_data.GetTaskCost(dwTaskID, taskCost));

				if (cost != taskCost)
				{
					cost.dAmount = 0;
					return FALSE;
				}
			}

			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CTDLTaskCtrlBase::GetSelectedTaskCustomAttributeData(const CString& sAttribID, TDCCADATA& data, BOOL bFormatted) const
{
	data.Clear();

	int nSelCount = GetSelectedCount();

	if (nSelCount)
	{
		TDCCUSTOMATTRIBUTEDEFINITION attribDef;
		VERIFY(m_aCustomAttribDefs.GetAttributeDef(sAttribID, attribDef));

		// Multi-selection check lists need special handling
		if (attribDef.IsMultiList())
		{
			CMap<CString, LPCTSTR, int, int&> mapCounts;
			POSITION pos = GetFirstSelectedTaskPos();

			while (pos)
			{
				DWORD dwTaskID = GetNextSelectedTaskID(pos);
				
				if (m_data.GetTaskCustomAttributeData(dwTaskID, sAttribID, data))
				{
					CStringArray aTaskItems;
					int nItem = data.AsArray(aTaskItems);

					while (nItem--)
						Misc::IncrementItemStrT<int>(mapCounts, aTaskItems[nItem]);
				}
			}

			CStringArray aMatched, aMixed;
			SplitSelectedTaskArrayMatchCounts(mapCounts, nSelCount, aMatched, aMixed);

			data.Set(aMatched, aMixed);
		}
		else
		{
			// get first item's value as initial
			POSITION pos = GetFirstSelectedTaskPos();
			DWORD dwTaskID = GetNextSelectedTaskID(pos);
		
			m_data.GetTaskCustomAttributeData(dwTaskID, sAttribID, data);
		
			while (pos)
			{
				dwTaskID = GetNextSelectedTaskID(pos);
			
				TDCCADATA dataNext;
				m_data.GetTaskCustomAttributeData(dwTaskID, sAttribID, dataNext);

				if (data != dataNext)
				{
					data.Clear();
					return FALSE;
				}
			}
		}

		if (bFormatted && !data.IsEmpty())
			data.Set(attribDef.FormatData(data, HasStyle(TDCS_SHOWDATESINISO)));
	}
	
	return !data.IsEmpty();
}

int CTDLTaskCtrlBase::GetSelectedTaskAllocTo(CStringArray& aAllocTo) const
{
	return GetSelectedTaskArray(TDCA_ALLOCTO, aAllocTo);
}

int CTDLTaskCtrlBase::GetSelectedTaskAllocTo(CStringArray& aMatched, CStringArray& aMixed) const
{
	return GetSelectedTaskArray(TDCA_ALLOCTO, aMatched, aMixed);
}

CString CTDLTaskCtrlBase::GetSelectedTaskAllocBy() const
{
	CString sAllocBy;
	
	if (GetSelectedCount())
	{
		// get first item's value as initial
		POSITION pos = GetFirstSelectedTaskPos();
		DWORD dwTaskID = GetNextSelectedTaskID(pos);

		sAllocBy = m_data.GetTaskAllocBy(dwTaskID);
		
		while (pos)
		{
			dwTaskID = GetNextSelectedTaskID(pos);
			CString sTaskAllocBy = m_data.GetTaskAllocBy(dwTaskID);
			
			if (sAllocBy != sTaskAllocBy)
				return EMPTY_STR;
		}
	}
	
	return sAllocBy;
}

CString CTDLTaskCtrlBase::GetSelectedTaskVersion() const
{
	CString sVersion;
	
	if (GetSelectedCount())
	{
		// get first item's value as initial
		POSITION pos = GetFirstSelectedTaskPos();
		DWORD dwTaskID = GetNextSelectedTaskID(pos);

		sVersion = m_data.GetTaskVersion(dwTaskID);
		
		while (pos)
		{
			dwTaskID = GetNextSelectedTaskID(pos);
			CString sTaskVersion = m_data.GetTaskVersion(dwTaskID);
			
			if (sVersion != sTaskVersion)
				return EMPTY_STR;
		}
	}
	
	return sVersion;
}

CString CTDLTaskCtrlBase::GetSelectedTaskStatus() const
{
	CString sStatus;
	
	if (GetSelectedCount())
	{
		// get first item's value as initial
		POSITION pos = GetFirstSelectedTaskPos();
		DWORD dwTaskID = GetNextSelectedTaskID(pos);

		sStatus = m_data.GetTaskStatus(dwTaskID);
		
		while (pos)
		{
			dwTaskID = GetNextSelectedTaskID(pos);
			CString sTaskStatus = m_data.GetTaskStatus(dwTaskID);
			
			if (sStatus != sTaskStatus)
				return EMPTY_STR;
		}
	}
	
	return sStatus;
}

int CTDLTaskCtrlBase::GetSelectedTaskArray(TDC_ATTRIBUTE nAttrib, CStringArray& aItems) const
{
	aItems.RemoveAll();

	if (GetSelectedCount())
	{
		// get first item's value as initial
		POSITION pos = GetFirstSelectedTaskPos();
		DWORD dwTaskID = GetNextSelectedTaskID(pos);
		
		m_data.GetTaskArray(dwTaskID, nAttrib, aItems);
		
		while (pos)
		{
			dwTaskID = GetNextSelectedTaskID(pos);

			CStringArray aTaskItems;
			m_data.GetTaskArray(dwTaskID, nAttrib, aTaskItems);
			
			if (!Misc::MatchAll(aItems, aTaskItems))
			{
				aItems.RemoveAll();
				break;
			}
		}
	}
	
	return aItems.GetSize();
}

int CTDLTaskCtrlBase::GetSelectedTaskArray(TDC_ATTRIBUTE nAttrib, CStringArray& aMatched, CStringArray& aMixed) const
{
	int nSelCount = GetSelectedCount();
	CMap<CString, LPCTSTR, int, int&> mapCounts;

	POSITION pos = GetFirstSelectedTaskPos();

	while (pos)
	{
		DWORD dwTaskID = GetNextSelectedTaskID(pos);

		CStringArray aTaskItems;
		int nItem = m_data.GetTaskArray(dwTaskID, nAttrib, aTaskItems);

		while (nItem--)
			Misc::IncrementItemStrT<int>(mapCounts, aTaskItems[nItem]);
	}

	return SplitSelectedTaskArrayMatchCounts(mapCounts, nSelCount, aMatched, aMixed);
}

int CTDLTaskCtrlBase::SplitSelectedTaskArrayMatchCounts(const CMap<CString, LPCTSTR, int, int&>& mapCounts, int nNumTasks, CStringArray& aMatched, CStringArray& aMixed)
{
	aMatched.RemoveAll();
	aMixed.RemoveAll();

	POSITION pos = mapCounts.GetStartPosition();

	while (pos)
	{
		CString sItem;
		int nCount = 0;

		mapCounts.GetNextAssoc(pos, sItem, nCount);

		if (nCount == nNumTasks)
		{
			aMatched.Add(sItem);
		}
		else if (nCount > 0)
		{
			aMixed.Add(sItem);
		}
	}

	return aMatched.GetSize();
}

int CTDLTaskCtrlBase::GetSelectedTaskCategories(CStringArray& aCats) const
{
	return GetSelectedTaskArray(TDCA_CATEGORY, aCats);
}

int CTDLTaskCtrlBase::GetSelectedTaskCategories(CStringArray& aMatched, CStringArray& aMixed) const
{
	return GetSelectedTaskArray(TDCA_CATEGORY, aMatched, aMixed);
}

int CTDLTaskCtrlBase::GetSelectedTaskTags(CStringArray& aTags) const
{
	return GetSelectedTaskArray(TDCA_TAGS, aTags);
}

int CTDLTaskCtrlBase::GetSelectedTaskTags(CStringArray& aMatched, CStringArray& aMixed) const
{
	return GetSelectedTaskArray(TDCA_TAGS, aMatched, aMixed);
}

int CTDLTaskCtrlBase::GetSelectedTaskDependencies(CStringArray& aDepends) const
{
	return GetSelectedTaskArray(TDCA_DEPENDENCY, aDepends);
}

CString CTDLTaskCtrlBase::GetSelectedTaskFileRef(int nFile) const
{
	if (GetSelectedCount() == 1)
		return m_data.GetTaskFileRef(GetSelectedTaskID(), nFile);
	
	// else
	return EMPTY_STR;
}

int CTDLTaskCtrlBase::GetSelectedTaskFileRefs(CStringArray& aFiles) const
{
	if (GetSelectedCount() == 1)
		return m_data.GetTaskFileRefs(GetSelectedTaskID(), aFiles);
	
	// else
	aFiles.RemoveAll();
	return 0;
}

int CTDLTaskCtrlBase::GetSelectedTaskFileRefCount() const
{
	if (GetSelectedCount() == 1)
		return m_data.GetTaskFileRefCount(GetSelectedTaskID());
	
	// else
	return 0;
}

CString CTDLTaskCtrlBase::GetSelectedTaskExtID() const
{
	if (GetSelectedCount() == 1)
		return m_data.GetTaskExtID(GetSelectedTaskID());
	
	// else
	return EMPTY_STR;
}

BOOL CTDLTaskCtrlBase::CanSplitSelectedTask() const
{
	if (IsReadOnly())
		return FALSE;
	
	if (SelectionHasReferences())
		return FALSE;
	
	int nSelCount = GetSelectedCount();
	
	if (nSelCount == 1)
	{
		if (IsSelectedTaskDone() || SelectionHasSubtasks())
			return FALSE;
	}
	
	return (nSelCount > 0);
}

BOOL CTDLTaskCtrlBase::IsSelectedTaskDone() const
{
	return m_data.IsTaskDone(GetSelectedTaskID());
}

BOOL CTDLTaskCtrlBase::IsSelectedTaskDue() const
{
	return m_calculator.IsTaskOverDue(GetSelectedTaskID());
}

BOOL CTDLTaskCtrlBase::PreTranslateMessage(MSG* pMsg)
{
	m_tooltipColumns.FilterToolTipMessage(pMsg);

	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		// Do our custom column resizing because Windows own
		// does not understand how we do things!
		switch (pMsg->wParam)
		{
		case VK_ADD:
			if ((IsLeft(pMsg->hwnd) || IsRight(pMsg->hwnd)) && Misc::ModKeysArePressed(MKS_CTRL))
			{
				RecalcAllColumnWidths();
				return TRUE;
			}
			break;

		case VK_ESCAPE:
			if (IsSplitting())
			{
				VERIFY(::ReleaseCapture());
				ASSERT(IsSplitting() == FALSE);

				return TRUE;
			}
			break;
		}
	}
	
	// all else
	return CWnd::PreTranslateMessage(pMsg);
}

BOOL CTDLTaskCtrlBase::OnHelpInfo(HELPINFO* /*lpHelpInfo*/)
{
	AfxGetApp()->WinHelp(GetHelpID());
	return TRUE;
}

BOOL CTDLTaskCtrlBase::SaveToImage(CBitmap& bmImage)
{
	if (!CanSaveToImage())
		return FALSE;

	CLockUpdates lock(GetSafeHwnd());

	// Allow derived classes to get involved
	BOOL bRes = DoSaveToImage(bmImage, m_crGridLine);

	ResyncScrollPos(Tasks(), m_lcColumns);

	return bRes;
}

BOOL CTDLTaskCtrlBase::CanSaveToImage() const
{
	return (GetTaskCount() > 0);
}

BOOL CTDLTaskCtrlBase::DoSaveToImage(CBitmap& bmImage, COLORREF crDivider)
{
	return CTreeListSyncer::SaveToImage(bmImage, crDivider);
}
