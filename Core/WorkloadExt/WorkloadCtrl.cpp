// WorkloadTreeList.cpp: implementation of the CWorkloadTreeList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "WorkloadExt.h" // for WORKLOAD_TYPEID
#include "WorkloadCtrl.h"
#include "WorkloadMsg.h"
#include "WorkloadStatic.h"

#include "..\shared\DialogHelper.h"
#include "..\shared\DateHelper.h"
#include "..\shared\timeHelper.h"
#include "..\shared\holdredraw.h"
#include "..\shared\graphicsMisc.h"
#include "..\shared\TreeCtrlHelper.h"
#include "..\shared\autoflag.h"
#include "..\shared\misc.h"
#include "..\shared\enstring.h"
#include "..\shared\localizer.h"
#include "..\shared\themed.h"
#include "..\shared\osversion.h"
#include "..\shared\enbitmap.h"
#include "..\shared\copywndcontents.h"
#include "..\shared\wclassdefines.h"
#include "..\shared\mousewheelmgr.h"
#include "..\shared\CopyWndContents.h"

#include "..\3rdparty\shellicons.h"

#include "..\Interfaces\iuiextension.h"

#include <float.h> // for DBL_MAX
#include <math.h>  // for fabs()

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

#ifndef CDRF_SKIPPOSTPAINT
#	define CDRF_SKIPPOSTPAINT	(0x00000100)
#endif

#ifndef LVS_EX_DOUBLEBUFFER
#define LVS_EX_DOUBLEBUFFER 0x00010000
#endif

//////////////////////////////////////////////////////////////////////

const int MIN_COL_WIDTH			= GraphicsMisc::ScaleByDPIFactor(6);
const int MIN_LABEL_EDIT_WIDTH	= GraphicsMisc::ScaleByDPIFactor(200);
const int TREE_TITLE_MIN_WIDTH	= GraphicsMisc::ScaleByDPIFactor(75); 
const int LV_COLPADDING			= GraphicsMisc::ScaleByDPIFactor(3);
const int TV_TIPPADDING			= GraphicsMisc::ScaleByDPIFactor(3);
const int HD_COLPADDING			= GraphicsMisc::ScaleByDPIFactor(6);
const int IMAGE_SIZE			= GraphicsMisc::ScaleByDPIFactor(16);

//////////////////////////////////////////////////////////////////////

#define GET_WI_RET(id, wi, ret)	\
{								\
	if (id == 0) return ret;	\
	wi = GetWorkloadItem(id);		\
	ASSERT(wi);					\
	if (wi == NULL) return ret;	\
}

#define GET_WI(id, wi)		\
{							\
	if (id == 0) return;	\
	wi = GetWorkloadItem(id);	\
	ASSERT(wi);				\
	if (wi == NULL)	return;	\
}

//////////////////////////////////////////////////////////////////////

class CWorkloadLockUpdates : public CLockUpdates
{
public:
	CWorkloadLockUpdates(CWorkloadCtrl* pCtrl, BOOL bTree, BOOL bAndSync) 
		: 
	CLockUpdates(bTree ? pCtrl->m_tcTasks.GetSafeHwnd() : pCtrl->m_lcColumns.GetSafeHwnd()),
		m_bAndSync(bAndSync), 
		m_pCtrl(pCtrl)
	{
		ASSERT(m_pCtrl);
		
		if (m_bAndSync)
			m_pCtrl->EnableResync(FALSE);
	}
	
	~CWorkloadLockUpdates()
	{
		ASSERT(m_pCtrl);
		
		if (m_bAndSync)
			m_pCtrl->EnableResync(TRUE, m_hWnd);
	}
	
private:
	BOOL m_bAndSync;
	CWorkloadCtrl* m_pCtrl;
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkloadCtrl::CWorkloadCtrl() 
	:
	CTreeListSyncer(TLSF_SYNCSELECTION | TLSF_SYNCFOCUS | TLSF_BORDER | TLSF_SYNCDATA | TLSF_SPLITTER),
	m_dwOptions(WLCF_SHOWSPLITTERBAR),
	m_crAltLine(CLR_NONE),
	m_crGridLine(CLR_NONE),
	m_crBkgnd(GetSysColor(COLOR_3DFACE)),
	m_crAllocation(RGB(255, 180, 180)),
	m_crOverlap(RGB(255, 0, 0)),
	m_dwMaxTaskID(0),
	m_bReadOnly(FALSE),
	m_bMovingTask(FALSE),
	m_nPrevDropHilitedItem(-1),
	m_tshDragDrop(m_tcTasks),
	m_treeDragDrop(m_tshDragDrop, m_tcTasks),
	m_mapTotalDays(FALSE),
	m_mapTotalTasks(FALSE),
	m_mapPercentLoad(TRUE), // average
	m_barChart(m_aAllocTo, m_mapPercentLoad),
	m_dtPeriod(DHD_BEGINTHISMONTH, DHD_ENDTHISMONTH, TRUE)
{
}

CWorkloadCtrl::~CWorkloadCtrl()
{
	//CTreeListSyncer::Release();
}

BEGIN_MESSAGE_MAP(CWorkloadCtrl, CWnd)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_TASKTREE, OnBeginEditTreeLabel)
	ON_NOTIFY(HDN_ENDDRAG, IDC_TASKHEADER, OnEndDragTreeHeader)
	ON_NOTIFY(HDN_ITEMCLICK, IDC_TASKHEADER, OnClickTreeHeader)
	ON_NOTIFY(HDN_ITEMCHANGING, IDC_TASKHEADER, OnItemChangingTreeHeader)
	ON_NOTIFY(HDN_ITEMCHANGED, IDC_TASKHEADER, OnItemChangedTreeHeader)
	ON_NOTIFY(HDN_DIVIDERDBLCLICK, IDC_TASKHEADER, OnDblClickTreeHeaderDivider)
	ON_NOTIFY(NM_RCLICK, IDC_TASKHEADER, OnRightClickTreeHeader)
	ON_NOTIFY(TVN_GETDISPINFO, IDC_TASKTREE, OnTreeGetDispInfo)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TASKTREE, OnTreeItemExpanded)
	ON_NOTIFY(TVN_KEYUP, IDC_TASKTREE, OnTreeKeyUp)
	ON_NOTIFY(NM_CLICK, IDC_ALLOCATIONCOLUMNS, OnClickColumns)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TOTALSLABELS, OnTotalsListsCustomDraw)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ALLOCATIONTOTALS, OnTotalsListsCustomDraw)

	ON_REGISTERED_MESSAGE(WM_DD_DRAGENTER, OnTreeDragEnter)
	ON_REGISTERED_MESSAGE(WM_DD_PREDRAGMOVE, OnTreePreDragMove)
	ON_REGISTERED_MESSAGE(WM_DD_DRAGOVER, OnTreeDragOver)
	ON_REGISTERED_MESSAGE(WM_DD_DRAGDROP, OnTreeDragDrop)
	ON_REGISTERED_MESSAGE(WM_DD_DRAGABORT, OnTreeDragAbort)

	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


BOOL CWorkloadCtrl::Create(CWnd* pParentWnd, const CRect& rect, UINT nID, BOOL bVisible)
{
	DWORD dwStyle = (WS_CHILD | (bVisible ? WS_VISIBLE : 0) | WS_TABSTOP);
	
	// create ourselves
	return CWnd::CreateEx(WS_EX_CONTROLPARENT, NULL, NULL, dwStyle, rect, pParentWnd, nID);
}

int CWorkloadCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	BOOL bVisible = (lpCreateStruct->style & WS_VISIBLE);
	CRect rect(0, 0, lpCreateStruct->cx, lpCreateStruct->cy);
	
	DWORD dwStyle = (WS_CHILD | (bVisible ? WS_VISIBLE : 0));
	
	if (!m_tcTasks.Create((dwStyle | WS_TABSTOP | TVS_SHOWSELALWAYS | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_NONEVENHEIGHT | TVS_NOTOOLTIPS | TVS_EDITLABELS),
							rect, 
							this, 
							IDC_TASKTREE))
	{
		return -1;
	}
	
	// Tasks Header ---------------------------------------------------------------------
	if (!m_hdrTasks.Create((dwStyle | HDS_HOTTRACK | HDS_BUTTONS | HDS_DRAGDROP | HDS_FULLDRAG), rect, this, IDC_TASKHEADER))
	{
		return -1;
	}
	
	// Column List ---------------------------------------------------------------------
	if (!m_lcColumns.Create((dwStyle | WS_TABSTOP),	rect, this, IDC_ALLOCATIONCOLUMNS))
	{
		return -1;
	}
	
	ListView_SetExtendedListViewStyleEx(m_lcColumns, LVS_EX_HEADERDRAGDROP, LVS_EX_HEADERDRAGDROP);
	ListView_SetExtendedListViewStyleEx(m_lcColumns, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	// subclass the tree and list
	if (!Sync(m_tcTasks, m_lcColumns, TLSL_RIGHTDATA_IS_LEFTITEM, m_hdrTasks))
	{
		return -1;
	}
	
	// Column Header ---------------------------------------------------------------------
	if (!m_hdrColumns.SubclassWindow(ListView_GetHeader(m_lcColumns)))
	{
		return FALSE;
	}
	m_hdrColumns.EnableToolTips();
	m_hdrColumns.EnableTracking(FALSE);

	// Totals are disabled so they can't grab the focus
	dwStyle |= LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_SINGLESEL | LVS_NOSCROLL | WS_TABSTOP | WS_DISABLED;

	if (!m_lcTotalsLabels.Create(dwStyle, rect, this, IDC_TOTALSLABELS))
	{
		return -1;
	}

	m_lcTotalsLabels.SetBkColor(m_crBkgnd);
	m_lcTotalsLabels.SetTextBkColor(m_crBkgnd);
	ListView_SetExtendedListViewStyleEx(m_lcTotalsLabels, LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);
	
	if (!m_lcColumnTotals.Create(dwStyle, rect, this, IDC_ALLOCATIONTOTALS))
	{
		return -1;
	}
	
	m_lcColumnTotals.ModifyStyleEx(0, WS_EX_CLIENTEDGE, 0);
	ListView_SetExtendedListViewStyleEx(m_lcColumnTotals, LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);

	if (m_themeHeader.AreControlsThemed())
		VERIFY(m_themeHeader.Open(*this, _T("HEADER")));

	// Bar Chart ------------------------------------------------------------------------
	// Initialise graph
	VERIFY(m_barChart.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_BARCHART));

	m_barChart.SetBkGnd(GetSysColor(COLOR_WINDOW));
	m_barChart.SetXLabelsAreTicks(true);
	m_barChart.SetXLabelAngle(45);
	m_barChart.SetYTicks(10);
	
	BuildTaskTreeColumns();
	BuildListColumns();
	PopulateTotalsLists();
	
	// prevent column reordering on columns
	m_hdrColumns.ModifyStyle(HDS_DRAGDROP, 0);

	// prevent translation of the list header
	CLocalizer::EnableTranslation(m_hdrColumns, FALSE);

	// Initialise tree drag and drop
	m_treeDragDrop.Initialize(m_tcTasks.GetParent(), TRUE, FALSE);

	// misc
	m_tcTasks.ModifyStyle(TVS_SHOWSELALWAYS, 0, 0);
	m_lcColumns.ModifyStyle(LVS_SHOWSELALWAYS, 0, 0);

 	PostResize();
	
	return 0;
}

BOOL CWorkloadCtrl::SetCurrentPeriod(const COleDateTimeRange& dtPeriod)
{
	m_dtPeriod = dtPeriod;
	m_dWorkDaysInPeriod = dtPeriod.GetWeekdayCount();

	if (GetSafeHwnd())
	{
		UpdateTotalsDateRangeLabel();
		RecalcAllocationTotals();
	}

	return TRUE;
}

void CWorkloadCtrl::UpdateTotalsDateRangeLabel()
{
	if (m_lcTotalsLabels.GetSafeHwnd())
	{
		CString sLabel;

		if (m_dtPeriod.IsValid())
			sLabel.Format(CEnString(IDS_TOTALSDATERANGE), m_dtPeriod.Format());

		m_lcTotalsLabels.SetItemText(ID_TOTALCOLUMNHEADER, 0, sLabel);
	}
}

void CWorkloadCtrl::OnLButtonDblClk(UINT /*nFlags*/, CPoint point)
{
	CRect rSplitter;
	
	if (GetSplitterRect(rSplitter) && rSplitter.PtInRect(point))
		AdjustSplitterToFitAttributeColumns();
}

void CWorkloadCtrl::AdjustSplitterToFitAttributeColumns()
{
	int nColsWidth = m_hdrColumns.CalcTotalItemWidth();

	if (HasVScrollBar(m_lcColumns))
		nColsWidth += GetSystemMetrics(SM_CXVSCROLL);
	
	CRect rClient;
	CWnd::GetClientRect(rClient);

	// adjust for graph
	rClient.right = ((rClient.Width() * 2) / 3);
	
	int nNewSplitPos = (rClient.right - nColsWidth - GetSplitBarWidth() - LV_COLPADDING);
	nNewSplitPos = max(MIN_LABEL_EDIT_WIDTH, nNewSplitPos);
	
	CTreeListSyncer::SetSplitPos(nNewSplitPos);
}

void CWorkloadCtrl::InitItemHeights()
{
	CTreeListSyncer::InitItemHeights();
}

HTREEITEM CWorkloadCtrl::GetSelectedItem() const
{
	return GetTreeSelItem();
}

DWORD CWorkloadCtrl::GetSelectedTaskID() const
{
	HTREEITEM hti = GetTreeSelItem();

	return (hti ? GetTaskID(hti) : 0);
}

BOOL CWorkloadCtrl::GetSelectedTask(WORKLOADITEM& wi) const
{
	DWORD dwTaskID = GetSelectedTaskID();
	const WORKLOADITEM* pWI = NULL;

	GET_WI_RET(dwTaskID, pWI, FALSE);
	
	wi = *pWI;
	return TRUE;
}

BOOL CWorkloadCtrl::SetSelectedTask(const WORKLOADITEM& wi)
{
	DWORD dwTaskID = GetSelectedTaskID();
	WORKLOADITEM* pWI = NULL;

	GET_WI_RET(dwTaskID, pWI, FALSE);

	*pWI = wi;

	RecalcAllocationTotals();

	return TRUE;
}

BOOL CWorkloadCtrl::SelectTask(DWORD dwTaskID)
{
	HTREEITEM hti = FindTreeItem(m_tcTasks, dwTaskID);

	return SelectItem(hti);
}

BOOL CWorkloadCtrl::SelectItem(HTREEITEM hti)
{
	if (hti == NULL)
		return FALSE;

	BOOL bWasVisible = IsTreeItemVisible(m_tcTasks, hti);

	SelectTreeItem(hti, FALSE);
	ResyncSelection(m_lcColumns, m_tcTasks, FALSE);

	if (!bWasVisible)
		ExpandList();

	return TRUE;
}

BOOL CWorkloadCtrl::SelectTask(IUI_APPCOMMAND nCmd, const IUISELECTTASK& select)
{
	HTREEITEM htiStart = NULL;
	BOOL bForwards = TRUE;

	switch (nCmd)
	{
	case IUI_SELECTFIRSTTASK:
		htiStart = m_tcTasks.TCH().GetFirstItem();
		break;

	case IUI_SELECTNEXTTASK:
		htiStart = m_tcTasks.TCH().GetNextItem(GetSelectedItem());
		break;
		
	case IUI_SELECTNEXTTASKINCLCURRENT:
		htiStart = GetSelectedItem();
		break;

	case IUI_SELECTPREVTASK:
		htiStart = m_tcTasks.TCH().GetPrevItem(GetSelectedItem());

		if (htiStart == NULL) // we were on the first task
			htiStart = m_tcTasks.TCH().GetLastItem();
		
		bForwards = FALSE;
		break;

	case IUI_SELECTLASTTASK:
		htiStart = m_tcTasks.TCH().GetLastItem();
		bForwards = FALSE;
		break;

	default:
		return FALSE;
	}

	return SelectTask(htiStart, select, bForwards);
}

BOOL CWorkloadCtrl::SelectTask(HTREEITEM hti, const IUISELECTTASK& select, BOOL bForwards)
{
	if (!hti)
		return FALSE;

	CString sTitle = m_tcTasks.GetItemText(hti);

	if (Misc::Find(select.szWords, sTitle, select.bCaseSensitive, select.bWholeWord) != -1)
	{
		if (SelectItem(hti))
			return TRUE;

		ASSERT(0);
	}

	if (bForwards)
		return SelectTask(m_tcTasks.TCH().GetNextItem(hti), select, TRUE);

	// else
	return SelectTask(m_tcTasks.TCH().GetPrevItem(hti), select, FALSE);
}

int CWorkloadCtrl::GetExpandedState(CDWordArray& aExpanded, HTREEITEM hti) const
{
	int nStart = 0;

	if (hti == NULL)
	{
		// guestimate initial size
		aExpanded.SetSize(0, m_tcTasks.GetCount() / 4);
	}
	else if (TCH().IsItemExpanded(hti) <= 0)
	{
		return 0; // nothing added
	}
	else // expanded
	{
		nStart = aExpanded.GetSize();
		aExpanded.Add(GetTaskID(hti));
	}

	// process children
	HTREEITEM htiChild = m_tcTasks.GetChildItem(hti);

	while (htiChild)
	{
		GetExpandedState(aExpanded, htiChild);
		htiChild = m_tcTasks.GetNextItem(htiChild, TVGN_NEXT);
	}

	return (aExpanded.GetSize() - nStart);
}

void CWorkloadCtrl::SetExpandedState(const CDWordArray& aExpanded)
{
	int nNumExpanded = aExpanded.GetSize();

	if (nNumExpanded)
	{
		for (int nItem = 0; nItem < nNumExpanded; nItem++)
		{
			HTREEITEM hti = GetTreeItem(aExpanded[nItem]);

			if (hti)
				m_tcTasks.Expand(hti, TVE_EXPAND);
		}

		ExpandList();
	}
}

BOOL CWorkloadCtrl::EditWantsResort(const ITASKLISTBASE* pTasks, IUI_UPDATETYPE nUpdate) const
{
	switch (nUpdate)
	{
	case IUI_ALL:
		// Note: Tasks should arrive 'unsorted' so we only need to
		// resort if an attribute is set
		return m_sort.IsSorting();

	case IUI_NEW:
		// Don't sort new tasks because it's confusing
		return FALSE;

	case IUI_EDIT:
		if (m_sort.IsSorting())
		{
			if (!m_sort.bMultiSort)
				return pTasks->IsAttributeAvailable(MapColumnToAttribute(m_sort.single.nBy));

			// else
			for (int nCol = 0; nCol < 3; nCol++)
			{
				if (pTasks->IsAttributeAvailable(MapColumnToAttribute(m_sort.multi.cols[nCol].nBy)))
					return TRUE;
			}
		}
		break;

	case IUI_DELETE:
		break;

	default:
		ASSERT(0);
	}

	return FALSE;
}

void CWorkloadCtrl::UpdateTasks(const ITaskList* pTaskList, IUI_UPDATETYPE nUpdate)
{
	// we must have been initialized already
	ASSERT(m_lcColumns.GetSafeHwnd() && m_tcTasks.GetSafeHwnd());

	// always cancel any ongoing operation
	CancelOperation();

	const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pTaskList, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return;
	}

	// Two stage update to avoid redrawing when in
	// partially updated state
	CDWordArray aExpanded;
	GetExpandedState(aExpanded);

	DWORD dwSelID = GetSelectedTaskID();

	// Stage 1: Update the data structures
	switch (nUpdate)
	{
	case IUI_ALL:
		LockWindowUpdate();
		EnableResync(FALSE);

		RebuildTree(pTasks);
		break;
		
	case IUI_NEW:
	case IUI_EDIT:
		{
			m_tcTasks.SetRedraw(FALSE);
			m_lcColumns.SetRedraw(FALSE);
			
			// update the task(s)
			if (UpdateTask(pTasks, pTasks->GetFirstTask(), nUpdate, TRUE))
			{
				if (pTasks->IsAttributeAvailable(TDCA_DUEDATE) || pTasks->IsAttributeAvailable(TDCA_STARTDATE))
					m_data.RecalculateOverlaps();
			}
		}
		break;
		
	case IUI_DELETE:
		{
			m_tcTasks.SetRedraw(FALSE);
			m_lcColumns.SetRedraw(FALSE);
		
			RemoveDeletedTasks(pTasks);
		}
		break;
		
	default:
		ASSERT(0);
		return;
	}

	InitItemHeights();
	UpdateListColumns();
	FixupListSortColumn();
	RecalcTreeColumns(TRUE);
	RecalcAllocationTotals();
	RecalcDataDateRange();

	// Stage 2: Restore rendering
	switch (nUpdate)
	{
	case IUI_ALL:
		{
			// Odd bug: The very last tree item will sometimes not scroll into view. 
			// Expanding and collapsing an item is enough to resolve the issue. 
			// First time only though.
			if (aExpanded.GetSize() == 0)
				PreFixVScrollSyncBug();

			SetExpandedState(aExpanded);
			SelectTask(dwSelID);

			UnlockWindowUpdate();
			EnableResync(TRUE, m_tcTasks);
		}
		break;

	case IUI_NEW:
	case IUI_EDIT:
	case IUI_DELETE:
		{
			m_tcTasks.SetRedraw(TRUE);
			m_lcColumns.SetRedraw(TRUE);
		}
		break;

	default:
		ASSERT(0);
		return;
	}
		
	if (EditWantsResort(pTasks, nUpdate))
	{
		ASSERT(m_sort.IsSorting());

		CHoldRedraw hr(m_tcTasks);

		if (m_sort.bMultiSort)
			CTreeListSyncer::Sort(MultiSortProc, (DWORD)this);
		else
			CTreeListSyncer::Sort(SortProc, (DWORD)this);
	}
}

void CWorkloadCtrl::RecalcAllocationTotals()
{
	m_data.CalculateTotals(m_dtPeriod, m_mapTotalDays, m_mapTotalTasks);

	// Individual loading
	m_mapPercentLoad.RemoveAll();

	if (m_dWorkDaysInPeriod > 0.0)
	{
		for (int nAllocTo = 0; nAllocTo < m_aAllocTo.GetSize(); nAllocTo++)
		{
			const CString& sAllocTo = m_aAllocTo[nAllocTo];
			double dTotalDays = m_mapTotalDays.Get(sAllocTo);

			m_mapPercentLoad.Set(sAllocTo, (dTotalDays * 100 / m_dWorkDaysInPeriod));
		}
	}

	m_lcColumnTotals.Invalidate();
	m_barChart.RebuildChart();
}

void CWorkloadCtrl::PreFixVScrollSyncBug()
{
	// Odd bug: The very last tree item will not scroll into view. 
	// Expanding and collapsing an item is enough to resolve the issue.
	HTREEITEM hti = TCH().FindFirstParent();
		
	if (hti)
	{
		TCH().ExpandItem(hti, TRUE);
		TCH().ExpandItem(hti, FALSE);
	}
}

int CWorkloadCtrl::GetTaskAllocTo(const ITASKLISTBASE* pTasks, HTASKITEM hTask, CStringArray& aAllocTo)
{
	aAllocTo.RemoveAll();
	int nAllocTo = pTasks->GetTaskAllocatedToCount(hTask);
	
	while (nAllocTo--)
		aAllocTo.InsertAt(0, pTasks->GetTaskAllocatedTo(hTask, nAllocTo));
	
	return aAllocTo.GetSize();
}

double CWorkloadCtrl::GetTaskTimeEstimate(const ITASKLISTBASE* pTasks, HTASKITEM hTask)
{
	TDC_UNITS nUnits = TDCU_NULL;
	double dTimeEst = pTasks->GetTaskTimeEstimate(hTask, nUnits, true);

	TH_UNITS nTHUnits = THU_NULL;

	switch (nUnits)
	{
	case TDCU_MINS:		nTHUnits = THU_MINS;	break;
	case TDCU_HOURS:	nTHUnits = THU_HOURS;	break;
	case TDCU_WEEKS:	nTHUnits = THU_WEEKS;	break;
	case TDCU_MONTHS:	nTHUnits = THU_MONTHS;	break;
	case TDCU_YEARS:	nTHUnits = THU_YEARS;	break;

	case TDCU_DAYS:
	case TDCU_WEEKDAYS:
		return dTimeEst;

	default:
		ASSERT(0);
		return 0.0;
	}

	return CTimeHelper().GetTime(dTimeEst, nTHUnits, THU_WEEKDAYS);
}

BOOL CWorkloadCtrl::WantEditUpdate(TDC_ATTRIBUTE nAttrib)
{
	switch (nAttrib)
	{
	case TDCA_ALLOCTO:
	case TDCA_COLOR:
	case TDCA_DONEDATE:
	case TDCA_DUEDATE:
	case TDCA_ICON:
	case TDCA_ID:
	case TDCA_NONE:
	case TDCA_PERCENT:
	case TDCA_STARTDATE:
	case TDCA_SUBTASKDONE:
	case TDCA_TASKNAME:
	case TDCA_TIMEEST:
		return TRUE;
	}
	
	// all else 
	return (nAttrib == IUI_ALL);
}

BOOL CWorkloadCtrl::WantSortUpdate(TDC_ATTRIBUTE nAttrib)
{
	switch (nAttrib)
	{
	case TDCA_ALLOCTO:
	case TDCA_DUEDATE:
	case TDCA_ID:
	case TDCA_PERCENT:
	case TDCA_STARTDATE:
	case TDCA_TASKNAME:
		return (MapAttributeToColumn(nAttrib) != WLCC_NONE);

	case TDCA_NONE:
		return TRUE;
	}
	
	// all else 
	return FALSE;
}

TDC_ATTRIBUTE CWorkloadCtrl::MapColumnToAttribute(WLC_COLUMNID nCol)
{
	switch (nCol)
	{
	case WLCC_TITLE:		return TDCA_TASKNAME;
	case WLCC_DUEDATE:		return TDCA_DUEDATE;
	case WLCC_STARTDATE:	return TDCA_STARTDATE;
	case WLCC_PERCENT:		return TDCA_PERCENT;
	case WLCC_TASKID:		return TDCA_ID;
	case WLCC_ALLOCTO:		return TDCA_ALLOCTO;
	case WLCC_DURATION:		return TDCA_TIMEEST;
	}
	
	// all else 
	return TDCA_NONE;
}

WLC_COLUMNID CWorkloadCtrl::MapAttributeToColumn(TDC_ATTRIBUTE nAttrib)
{
	switch (nAttrib)
	{
	case TDCA_TASKNAME:		return WLCC_TITLE;		
	case TDCA_DUEDATE:		return WLCC_DUEDATE;		
	case TDCA_STARTDATE:	return WLCC_STARTDATE;	
	case TDCA_PERCENT:		return WLCC_PERCENT;		
	case TDCA_ID:			return WLCC_TASKID;		
	case TDCA_ALLOCTO:		return WLCC_ALLOCTO;		
	case TDCA_TIMEEST:		return WLCC_DURATION;		
	}
	
	// all else 
	return WLCC_NONE;
}

BOOL CWorkloadCtrl::UpdateTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, BOOL bAndSiblings)
{
	if (hTask == NULL)
		return FALSE;

	ASSERT((nUpdate == IUI_EDIT) || (nUpdate == IUI_NEW));

	// handle task if not NULL (== root)
	DWORD dwTaskID = pTasks->GetTaskID(hTask);
	m_dwMaxTaskID = max(m_dwMaxTaskID, dwTaskID);

	// This can be a new task
	if (!m_data.HasItem(dwTaskID))
	{
		ASSERT(nUpdate == IUI_NEW);

		// Parent must exist or be NULL
		DWORD dwParentID = pTasks->GetTaskParentID(hTask);
		HTREEITEM htiParent = NULL;

		if (dwParentID)
		{
			if (!m_data.HasItem(dwParentID))
			{
				ASSERT(0);
				return FALSE;
			}

			htiParent = GetTreeItem(dwParentID);

			if (!htiParent)
			{
				ASSERT(0);
				return FALSE;
			}
		}

		// Before anything else we increment the position of 
		// any tasks having the same position of this new task 
		// or greater within the parent
		int nPos = pTasks->GetTaskPosition(hTask);
		IncrementItemPositions(htiParent, nPos);

		BuildTreeItem(pTasks, hTask, htiParent, FALSE, FALSE);
		RefreshTreeItemMap();

		return TRUE;
	}
	
	WORKLOADITEM* pWI = NULL;
	GET_WI_RET(dwTaskID, pWI, FALSE);

	// update taskID to refID 
	if (pWI->dwOrgRefID)
	{
		dwTaskID = pWI->dwOrgRefID;
		pWI->dwOrgRefID = 0;
	}

	// take a snapshot we can check changes against
	// Note: No need to check meta-data because we're 
	// the only one who can change it
	WORKLOADITEM giOrg = *pWI;

	// can't use a switch here because we also need to check for IUI_ALL
	time64_t tDate = 0;
	BOOL bRecalcMissingAllocations = FALSE;
	
	if (pTasks->IsAttributeAvailable(TDCA_TASKNAME))
		pWI->sTitle = pTasks->GetTaskTitle(hTask);
	
 	if (pTasks->IsAttributeAvailable(TDCA_ALLOCTO))
	{
 		GetTaskAllocTo(pTasks, hTask, pWI->aAllocTo);
		Misc::AddUniqueItems(pWI->aAllocTo, m_aAllocTo);
	}
	
	if (pTasks->IsAttributeAvailable(TDCA_ICON))
		pWI->bHasIcon = !Misc::IsEmpty(pTasks->GetTaskIcon(hTask));

	if (pTasks->IsAttributeAvailable(TDCA_PERCENT))
		pWI->nPercent = pTasks->GetTaskPercentDone(hTask, TRUE);
		
	if (pTasks->IsAttributeAvailable(TDCA_STARTDATE))
	{
		if (pTasks->GetTaskStartDate64(hTask, pWI->bParent, tDate))
			pWI->dtRange.m_dtStart = CDateHelper::GetDate(tDate);
		else
			CDateHelper::ClearDate(pWI->dtRange.m_dtStart);

		bRecalcMissingAllocations = HasOption(WLCF_CALCMISSINGALLOCATIONS);
	}
	
	if (pTasks->IsAttributeAvailable(TDCA_DUEDATE))
	{
		if (pTasks->GetTaskDueDate64(hTask, pWI->bParent, tDate))
			pWI->dtRange.m_dtEnd = CDateHelper::GetDate(tDate);
		else
			CDateHelper::ClearDate(pWI->dtRange.m_dtEnd);

		bRecalcMissingAllocations = HasOption(WLCF_CALCMISSINGALLOCATIONS);
	}

	if (pTasks->IsAttributeAvailable(TDCA_TIMEEST))
	{
		pWI->dTimeEst = GetTaskTimeEstimate(pTasks, hTask);

		bRecalcMissingAllocations = HasOption(WLCF_CALCMISSINGALLOCATIONS);
	}
	
	if (pTasks->IsAttributeAvailable(TDCA_DONEDATE))
		pWI->bDone = pTasks->IsTaskDone(hTask);

	if (pTasks->IsAttributeAvailable(TDCA_SUBTASKDONE))
	{
		LPCWSTR szSubTaskDone = pTasks->GetTaskSubtaskCompletion(hTask);
		pWI->bSomeSubtaskDone = (!Misc::IsEmpty(szSubTaskDone) && (szSubTaskDone[0] != '0'));
	}

	// always update lock states
	pWI->bLocked = pTasks->IsTaskLocked(hTask, true);

	// always update colour because it can change for so many reasons
	pWI->color = pTasks->GetTaskTextColor(hTask);

	// likewise 'Good as Done'
	pWI->bGoodAsDone = pTasks->IsTaskGoodAsDone(hTask);

	// detect update
	BOOL bChange = !(*pWI == giOrg);
		
	// children
	if (UpdateTask(pTasks, pTasks->GetFirstTask(hTask), nUpdate, TRUE))
		bChange = TRUE;

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

	if (bRecalcMissingAllocations && pWI->mapAllocatedDays.IsAutoCalculated())
		pWI->AutoCalculateAllocations(HasOption(WLCF_PREFERTIMEESTFORCALCS));
	
	return bChange;
}

void CWorkloadCtrl::BuildTaskMap(const ITASKLISTBASE* pTasks, HTASKITEM hTask, 
									  CSet<DWORD>& mapIDs, BOOL bAndSiblings)
{
	if (hTask == NULL)
		return;

	mapIDs.Add(pTasks->GetTaskID(hTask));

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

void CWorkloadCtrl::RemoveDeletedTasks(const ITASKLISTBASE* pTasks)
{
	CSet<DWORD> mapIDs;
	BuildTaskMap(pTasks, pTasks->GetFirstTask(), mapIDs, TRUE);

	if (RemoveDeletedTasks(NULL, pTasks, mapIDs))
	{
		m_data.RecalculateOverlaps();
		RefreshTreeItemMap();
	}
}

BOOL CWorkloadCtrl::RemoveDeletedTasks(HTREEITEM hti, const ITASKLISTBASE* pTasks, const CSet<DWORD>& mapIDs)
{
	// traverse the tree looking for items that do not 
	// exist in pTasks and delete them
	if (hti && !mapIDs.Has(GetTaskID(hti)))
	{
		DeleteTreeItem(hti);
		return TRUE;
	}

	// check its children
	BOOL bSomeRemoved = FALSE;
	HTREEITEM htiChild = m_tcTasks.GetChildItem(hti);
	
	while (htiChild)
	{
		// get next sibling before we (might) delete this one
		HTREEITEM htiNext = m_tcTasks.GetNextItem(htiChild, TVGN_NEXT);
		
		bSomeRemoved |= RemoveDeletedTasks(htiChild, pTasks, mapIDs);
		htiChild = htiNext;
	}

	return bSomeRemoved;
}

WORKLOADITEM* CWorkloadCtrl::GetWorkloadItem(DWORD dwTaskID, BOOL bCopyRefID) const
{
	WORKLOADITEM* pWI = m_data.GetItem(dwTaskID);
	ASSERT(pWI);
	
	if (pWI)
	{
		// For references we use the 'real' task but with the 
		// original reference reference ID copied over
		DWORD dwRefID = pWI->dwRefID;
		
		if (dwRefID && (dwRefID != dwTaskID) && m_data.Lookup(dwRefID, pWI))
		{
			// copy over the reference id so that the caller can still detect it
			if (bCopyRefID)
			{
				ASSERT((pWI->dwRefID == 0) || (pWI->dwRefID == dwRefID));
				pWI->dwOrgRefID = dwRefID;
			}
		}
		else
		{
			pWI->dwOrgRefID = 0;
		}
	}
	
	return pWI;
}

void CWorkloadCtrl::RebuildTree(const ITASKLISTBASE* pTasks)
{
	m_tcTasks.DeleteAllItems();
	m_lcColumns.DeleteAllItems();

	m_aAllocTo.RemoveAll();
	m_data.RemoveAll();

	m_dwMaxTaskID = 0;

	BuildTreeItem(pTasks, pTasks->GetFirstTask(), NULL, TRUE);

	m_data.RecalculateOverlaps();

	RefreshTreeItemMap();
	ExpandList();
	RefreshItemBoldState();
}

void CWorkloadCtrl::RecalcDataDateRange()
{
	COleDateTimeRange dtRange;

	if (!m_data.CalcDateRange(dtRange))
		dtRange.Add(COleDateTime::GetCurrentTime());

	m_dtDataRange.Set(CDateHelper::GetStartOfMonth(dtRange.GetStart()), 
					  CDateHelper::GetEndOfMonth(dtRange.GetEnd()));
}

void CWorkloadCtrl::RefreshTreeItemMap()
{
	m_mapHTItems.BuildMap(m_tcTasks);
}

void CWorkloadCtrl::PopulateTotalsLists()
{
	// Build Task totals once only
	if (m_lcTotalsLabels.GetItemCount() == 0)
	{
		for (int nTotal = 0; nTotal < NUM_TOTALS; nTotal++)
		{
			int nItem = m_lcTotalsLabels.InsertItem(nTotal, CEnString(WORKLOADTOTALS[nTotal].nTextResID));
			m_lcTotalsLabels.SetItemData(nItem, WORKLOADTOTALS[nTotal].nTotal);

			nItem = m_lcColumnTotals.InsertItem(nTotal, _T(""));
			m_lcColumnTotals.SetItemData(nItem, WORKLOADTOTALS[nTotal].nTotal);
		}

		UpdateTotalsDateRangeLabel();
	}
}

void CWorkloadCtrl::BuildTreeItem(const ITASKLISTBASE* pTasks, HTASKITEM hTask, 
										HTREEITEM htiParent, BOOL bAndSiblings, BOOL bInsertAtEnd)
{
	if (hTask == NULL)
		return;

	DWORD dwTaskID = pTasks->GetTaskID(hTask);
	ASSERT(!m_data.HasItem(dwTaskID));

	m_dwMaxTaskID = max(m_dwMaxTaskID, dwTaskID);

	// map the data
	WORKLOADITEM* pWI = new WORKLOADITEM(dwTaskID);
	m_data[dwTaskID] = pWI;
	
	pWI->dwRefID = pTasks->GetTaskReferenceID(hTask);

	// Except for position
	pWI->nPosition = pTasks->GetTaskPosition(hTask);

	// Only save data for non-references
	if (pWI->dwRefID == 0)
	{
		pWI->sTitle = pTasks->GetTaskTitle(hTask);
		pWI->color = pTasks->GetTaskTextColor(hTask);
		pWI->bDone = pTasks->IsTaskDone(hTask);
		pWI->bGoodAsDone = pTasks->IsTaskGoodAsDone(hTask);
		pWI->bParent = pTasks->IsTaskParent(hTask);
		pWI->nPercent = pTasks->GetTaskPercentDone(hTask, TRUE);
		pWI->bLocked = pTasks->IsTaskLocked(hTask, true);
		pWI->bHasIcon = !Misc::IsEmpty(pTasks->GetTaskIcon(hTask));

		GetTaskAllocTo(pTasks, hTask, pWI->aAllocTo);
		Misc::AddUniqueItems(pWI->aAllocTo, m_aAllocTo);
		
		LPCWSTR szSubTaskDone = pTasks->GetTaskSubtaskCompletion(hTask);
		pWI->bSomeSubtaskDone = (!Misc::IsEmpty(szSubTaskDone) && (szSubTaskDone[0] != '0'));

		time64_t tDate = 0;

		if (pTasks->GetTaskStartDate64(hTask, pWI->bParent, tDate))
			pWI->dtRange.m_dtStart = CDateHelper::GetDate(tDate);

		if (pTasks->GetTaskDueDate64(hTask, pWI->bParent, tDate))
			pWI->dtRange.m_dtEnd = CDateHelper::GetDate(tDate);

		pWI->dTimeEst = GetTaskTimeEstimate(pTasks, hTask);

		// This wants to be last so that time estimate and date range are up to date
		if (!pWI->mapAllocatedDays.Decode(pTasks->GetTaskMetaData(hTask, WORKLOAD_TYPEID)))
			pWI->AutoCalculateAllocations(HasOption(WLCF_PREFERTIMEESTFORCALCS));
	}
	
	// add item to tree
	HTREEITEM htiAfter = TVI_LAST; // default

	if (!bInsertAtEnd)
	{
		// Find the sibling task whose position is one less
		HTREEITEM htiSibling = m_tcTasks.GetChildItem(htiParent);

		while (htiSibling)
		{
			DWORD dwSiblingID = m_tcTasks.GetItemData(htiSibling);
			const WORKLOADITEM* pWISibling = GetWorkloadItem(dwSiblingID);
			ASSERT(pWISibling);

			if (pWISibling && (pWISibling->nPosition == (pWI->nPosition - 1)))
			{
				htiAfter = htiSibling;
				break;
			}

			htiSibling = m_tcTasks.GetNextItem(htiSibling, TVGN_NEXT);
		}
	}

	HTREEITEM hti = m_tcTasks.TCH().InsertItem(LPSTR_TEXTCALLBACK, 
											I_IMAGECALLBACK, 
											I_IMAGECALLBACK, 
											dwTaskID, // lParam
											htiParent, 
											htiAfter,
											FALSE,
											FALSE);
	
	// add first child which will add all the rest
	BuildTreeItem(pTasks, pTasks->GetFirstTask(hTask), hti, TRUE);
	
	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			BuildTreeItem(pTasks, hSibling, htiParent, FALSE);
			
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
}

void CWorkloadCtrl::IncrementItemPositions(HTREEITEM htiParent, int nFromPos)
{
	HTREEITEM htiChild = m_tcTasks.GetChildItem(htiParent);

	while (htiChild)
	{
		DWORD dwTaskID = GetTaskID(htiChild);
		WORKLOADITEM* pWI = NULL;

		GET_WI(dwTaskID, pWI);

		if (pWI->nPosition >= nFromPos)
			pWI->nPosition++;
		
		htiChild = m_tcTasks.GetNextItem(htiChild, TVGN_NEXT);
	}
}

void CWorkloadCtrl::SetOption(DWORD dwOption, BOOL bSet)
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
			switch (dwOption)
			{
			case WLCF_STRIKETHRUDONETASKS:
				m_tcTasks.Fonts().Clear();
				CWnd::Invalidate(FALSE);
				break;

			case WLCF_SHOWSPLITTERBAR:
				CTreeListSyncer::SetSplitBarWidth(bSet ? 10 : 0);
				break;

			case WLCF_DISPLAYISODATES:
				CWnd::Invalidate(FALSE);
				break;

			case WLCF_SHOWTREECHECKBOXES:
				m_tcTasks.ShowCheckboxes(bSet);
				break;

			case WLCF_CALCMISSINGALLOCATIONS:
				RefreshMissingAllocations();
				break;

			case WLCF_PREFERTIMEESTFORCALCS:
				if (HasOption(WLCF_CALCMISSINGALLOCATIONS))
					RefreshMissingAllocations();
				else
					m_tcTasks.Invalidate();
				break;
			}

			if (IsSyncing())
				RedrawList();
		}
	}
}

void CWorkloadCtrl::RefreshMissingAllocations()
{
	BOOL bAutoCalc = HasOption(WLCF_CALCMISSINGALLOCATIONS);
	BOOL bPreferTimeEst = HasOption(WLCF_PREFERTIMEESTFORCALCS);

	POSITION pos = m_data.GetStartPosition();

	while (pos)
	{
		WORKLOADITEM* pWI = m_data.GetNextItem(pos);

		if (pWI && pWI->mapAllocatedDays.IsAutoCalculated())
		{
			if (bAutoCalc)
				pWI->AutoCalculateAllocations(bPreferTimeEst);
			else
				pWI->ClearAllocations();
		}
	}
}

int CWorkloadCtrl::GetRequiredListColumnCount() const
{
	int nNumCols = m_aAllocTo.GetSize();

	nNumCols++; // first hidden column 
	nNumCols++; // spacer before last 'total' column
	nNumCols++; // last 'total' column

	return nNumCols;
}

void CWorkloadCtrl::BuildTaskTreeColumns()
{
	// add columns
	m_hdrTasks.InsertItem(0, 0, _T("Task"), (HDF_LEFT | HDF_STRING), 0, WLCC_TITLE);
	m_hdrTasks.EnableItemDragging(0, FALSE);

	for (int nCol = 0; nCol < NUM_TREECOLUMNS; nCol++)
	{
		m_hdrTasks.InsertItem(nCol + 1, 
								0, 
								CEnString(WORKLOADTREECOLUMNS[nCol].nIDColName), 
								(WORKLOADTREECOLUMNS[nCol].nColAlign | HDF_STRING),
								0,
								WORKLOADTREECOLUMNS[nCol].nColID);
	}

	// Build Task totals once only
	m_lcTotalsLabels.InsertColumn(0, _T(""), LVCFMT_RIGHT, GetSplitPos());

	// Align right
	LV_COLUMN lvc = { LVCF_FMT, LVCFMT_RIGHT, 0 };
	m_lcTotalsLabels.SetColumn(0, &lvc);
}

BOOL CWorkloadCtrl::IsTreeItemLineOdd(HTREEITEM hti) const
{
	int nItem = GetListItem(hti);

	return IsListItemLineOdd(nItem);
}

BOOL CWorkloadCtrl::IsListItemLineOdd(int nItem) const
{
	return ((nItem % 2) == 1);
}

void CWorkloadCtrl::SetFocus()
{
	if (!HasFocus())
		m_tcTasks.SetFocus();
}

void CWorkloadCtrl::Resize(int cx, int cy)
{
	if (!cx || !cy)
	{
		CRect rClient;
		CWnd::GetClientRect(rClient);

		cx = rClient.Width();
		cy = rClient.Height();
	}

	if (cx && cy)
	{
		CRect rTreeList(0, 0, ((cx * 2) / 3), cy);

		rTreeList.bottom = cy;
		rTreeList.bottom -= (GetItemHeight(m_lcTotalsLabels) * NUM_TOTALS); // rows
		rTreeList.bottom -= LV_COLPADDING; // spacing
		rTreeList.bottom -= 2; // totals border

		CRect rChart(rTreeList.right + LV_COLPADDING, 0, cx, cy);
		m_barChart.MoveWindow(rChart);
				
		CTreeListSyncer::Resize(rTreeList, GetSplitPos());

		if (m_hdrTasks.GetItemCount())
			m_tcTasks.SetTitleColumnWidth(m_hdrTasks.GetItemWidth(0));
	}
}

void CWorkloadCtrl::ResyncTotalsPositions()
{
	CRect rClient;
	CWnd::GetClientRect(rClient);

	CRect rTreeTotals = CDialogHelper::GetChildRect(&m_tcTasks);
	CRect rColumnTotals = CDialogHelper::GetChildRect(&m_lcColumns);

	rTreeTotals.top = rColumnTotals.top = (max(rTreeTotals.bottom, rColumnTotals.bottom) + LV_COLPADDING);
	rTreeTotals.bottom = rColumnTotals.bottom = rClient.bottom;

	// Adjust for splitter border
	rColumnTotals.left--;

	// Adjust for border drawn by CTreeListSyncer
	rColumnTotals.right++;

	m_lcColumnTotals.MoveWindow(rColumnTotals);
	m_lcTotalsLabels.MoveWindow(rTreeTotals);
	m_lcTotalsLabels.SetColumnWidth(0, rTreeTotals.Width());
}

void CWorkloadCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	
	Resize(cx, cy);
}

void CWorkloadCtrl::ExpandAll(BOOL bExpand)
{
	ExpandItem(NULL, bExpand, TRUE);

	RecalcTreeColumns(TRUE);
}

BOOL CWorkloadCtrl::CanExpandAll() const
{
	return TCH().IsAnyItemCollapsed();
}

BOOL CWorkloadCtrl::CanCollapseAll() const
{
	return TCH().IsAnyItemExpanded();
}

void CWorkloadCtrl::ExpandItem(HTREEITEM hti, BOOL bExpand, BOOL bAndChildren)
{
	// avoid unnecessary processing
	if (hti && !CanExpandItem(hti, bExpand))
		return;

	CAutoFlag af(m_bTreeExpanding, TRUE);
	EnableResync(FALSE);

	CHoldRedraw hr(*this);

	TCH().ExpandItem(hti, bExpand, bAndChildren);

	if (bExpand)
	{
		if (hti)
		{
			int nNextIndex = (GetListItem(hti) + 1);
			ExpandList(hti, nNextIndex);
		}
		else
			ExpandList(); // all
	}
	else
	{
		CollapseList(hti);
	}
	
	m_tcTasks.EnsureVisible(hti);

	EnableResync(TRUE, m_tcTasks);
	RecalcTreeColumns(TRUE);
}

BOOL CWorkloadCtrl::CanExpandItem(HTREEITEM hti, BOOL bExpand) const
{
	int nFullyExpanded = TCH().IsItemExpanded(hti, TRUE);
			
	if (nFullyExpanded == -1)	// item has no children
	{
		return FALSE; // can neither expand nor collapse
	}
	else if (bExpand)
	{
		return !nFullyExpanded;
	}
			
	// else
	return TCH().IsItemExpanded(hti, FALSE);
}

LRESULT CWorkloadCtrl::OnTreeCustomDraw(NMTVCUSTOMDRAW* pTVCD)
{
	HTREEITEM hti = (HTREEITEM)pTVCD->nmcd.dwItemSpec;
	
	switch (pTVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		return CDRF_NOTIFYITEMDRAW;
								
	case CDDS_ITEMPREPAINT:
		{
			DWORD dwTaskID = pTVCD->nmcd.lItemlParam;
			WORKLOADITEM* pWI = NULL;

			GET_WI_RET(dwTaskID, pWI, 0L);
				
 			CDC* pDC = CDC::FromHandle(pTVCD->nmcd.hdc);
			CRect rItem(pTVCD->nmcd.rc);

			COLORREF crBack = DrawTreeItemBackground(pDC, hti, *pWI, rItem, rItem, FALSE);
				
			// hide text because we will draw it later
			pTVCD->clrTextBk = pTVCD->clrText = crBack;
		
		}
		return (CDRF_NOTIFYPOSTPAINT | CDRF_NEWFONT); // always
								
	case CDDS_ITEMPOSTPAINT:
		{
			// check row is visible
			CRect rItem;
			GetTreeItemRect(hti, WLCC_TITLE, rItem);

			CRect rClient;
			m_tcTasks.GetClientRect(rClient);
			
			if ((rItem.bottom > 0) && (rItem.top < rClient.bottom))
			{
				DWORD dwTaskID = pTVCD->nmcd.lItemlParam;
				WORKLOADITEM* pWI = NULL;

				GET_WI_RET(dwTaskID, pWI, 0L);
				
				CDC* pDC = CDC::FromHandle(pTVCD->nmcd.hdc);

				GM_ITEMSTATE nState = GetItemState(hti);
				BOOL bSelected = (nState != GMIS_NONE);

				// draw horz gridline before selection
				DrawItemDivider(pDC, pTVCD->nmcd.rc, DIV_HORZ, bSelected);

				// Draw icon
				if (pWI->bHasIcon || pWI->bParent)
				{
					int iImageIndex = -1;
					HIMAGELIST hilTask = m_tcTasks.GetTaskIcon(pWI->dwTaskID, iImageIndex);

					if (hilTask && (iImageIndex != -1))
					{
						CRect rItem;
						m_tcTasks.GetItemRect(hti, rItem, TRUE);

						CRect rIcon(rItem);
						rIcon.left -= (IMAGE_SIZE + 2);
						rIcon.bottom = (rIcon.top + IMAGE_SIZE);
						GraphicsMisc::CentreRect(rIcon, rItem, FALSE, TRUE);

						ImageList_Draw(hilTask, iImageIndex, *pDC, rIcon.left, rIcon.top, ILD_TRANSPARENT);
					}
				}
				
				// draw background
				COLORREF crBack = DrawTreeItemBackground(pDC, hti, *pWI, rItem, rClient, bSelected);
				
				// draw Workload item attribute columns
				DrawTreeItem(pDC, hti, *pWI, bSelected, crBack);
			}			
	
		}
		return CDRF_SKIPDEFAULT;
	}

	return CDRF_DODEFAULT;
}

COLORREF CWorkloadCtrl::DrawTreeItemBackground(CDC* pDC, HTREEITEM hti, const WORKLOADITEM& wi, const CRect& rItem, const CRect& rClient, BOOL bSelected)
{
	BOOL bAlternate = (HasAltLineColor() && !IsTreeItemLineOdd(hti));
	COLORREF crBack = GetTreeTextBkColor(wi, bSelected, bAlternate);

	if (!bSelected)
	{
		// redraw item background else tooltips cause overwriting
		CRect rBack(rItem);
		rBack.bottom--;
		rBack.right = rClient.right;

		pDC->FillSolidRect(rBack, crBack);
	}
	else
	{
		DWORD dwFlags = (GMIB_THEMECLASSIC | GMIB_EXTENDRIGHT | GMIB_CLIPRIGHT);
		GraphicsMisc::DrawExplorerItemBkgnd(pDC, m_tcTasks, GetItemState(hti), rItem, dwFlags);
	}

	return crBack;
}

GM_ITEMSTATE CWorkloadCtrl::GetItemState(int nItem) const
{
	if (!m_bSavingToImage)
	{
		if (IsListItemSelected(m_lcColumns, nItem))
		{
			if (HasFocus())
				return GMIS_SELECTED;
			else
				return GMIS_SELECTEDNOTFOCUSED;
		}
		else if (ListItemHasState(m_lcColumns, nItem, LVIS_DROPHILITED))
		{
			return GMIS_DROPHILITED;
		}
	}

	// else
	return GMIS_NONE;
}

GM_ITEMSTATE CWorkloadCtrl::GetItemState(HTREEITEM hti) const
{
	if (!m_bSavingToImage)
	{
		if (IsTreeItemSelected(m_tcTasks, hti))
		{
			if (HasFocus())
				return GMIS_SELECTED;
			else
				return GMIS_SELECTEDNOTFOCUSED;
		}
		else if (TreeItemHasState(m_tcTasks, hti, TVIS_DROPHILITED))
		{
			return GMIS_DROPHILITED;
		}
	}

	// else
	return GMIS_NONE;
}

LRESULT CWorkloadCtrl::OnListCustomDraw(NMLVCUSTOMDRAW* pLVCD)
{
	ASSERT(pLVCD->nmcd.hdr.idFrom == IDC_ALLOCATIONCOLUMNS);

	return OnAllocationsListCustomDraw(pLVCD);
}

void CWorkloadCtrl::OnTotalsListsCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	switch (pNMHDR->idFrom)
	{
	case IDC_ALLOCATIONCOLUMNS:
		ASSERT(0);
		break;

	case IDC_TOTALSLABELS:
		*pResult = OnTotalsLabelsListCustomDraw((NMLVCUSTOMDRAW*)pNMHDR);
		break;

	case IDC_ALLOCATIONTOTALS:
		*pResult = OnAllocationsTotalsListCustomDraw((NMLVCUSTOMDRAW*)pNMHDR);
		break;
	}
}

LRESULT CWorkloadCtrl::OnTotalsLabelsListCustomDraw(NMLVCUSTOMDRAW* pLVCD)
{
	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		{
			CRect rClient;
			m_lcTotalsLabels.GetClientRect(rClient);

			CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
			pDC->FillSolidRect(rClient, (m_bSavingToImage ? GetSysColor(COLOR_WINDOW) : m_crBkgnd));
		}
		return CDRF_NOTIFYITEMDRAW;

	case CDDS_ITEMPREPAINT:
		{
			pLVCD->clrTextBk = (m_bSavingToImage ? GetSysColor(COLOR_WINDOW) : m_crBkgnd);

			CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);

			if (pLVCD->nmcd.lItemlParam == ID_TOTALCOLUMNHEADER)
				pDC->SelectObject(m_tcTasks.Fonts().GetFont(GMFS_BOLD));
			else
				pDC->SelectObject(m_tcTasks.Fonts().GetFont());
		}
		return CDRF_NEWFONT;
	}

	// else
	return CDRF_DODEFAULT;
}

LRESULT CWorkloadCtrl::OnAllocationsTotalsListCustomDraw(NMLVCUSTOMDRAW* pLVCD)
{
	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		return CDRF_NOTIFYITEMDRAW;
								
	case CDDS_ITEMPREPAINT:
		{
			HWND hwndList = pLVCD->nmcd.hdr.hwndFrom;
			int nItem = (int)pLVCD->nmcd.dwItemSpec;
			CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
			
			CRect rItem, rClient;
			m_lcColumnTotals.GetItemRect(nItem, rItem, LVIR_BOUNDS);
			m_lcColumnTotals.GetClientRect(rClient);

			// draw item bkgnd and gridlines full width of list
			CRect rFullWidth(rItem);
			rFullWidth.right = rClient.right + 2;

			switch (nItem)
			{
			case ID_TOTALCOLUMNHEADER:
				if (m_themeHeader.AreControlsThemed())
					m_themeHeader.DrawBackground(pDC, HP_HEADERITEM, HIS_NORMAL, rFullWidth);
				else
					pDC->FillSolidRect(rFullWidth, ::GetSysColor(COLOR_3DFACE));
				break;
				
			case ID_TOTALDAYSPERPERSON:
			case ID_TOTALTASKSPERPERSON:
			case ID_PERCENTLOADPERPERSON:
				{
					pDC->FillSolidRect(rFullWidth, GetRowColor(nItem + 1));
					
					if (m_bSavingToImage || ((nItem + 1) != ID_LASTTOTAL))
						DrawItemDivider(pDC, rFullWidth, DIV_HORZ, FALSE);
				}
				break;
				
			default:
				ASSERT(0);
			}
			
			// Draw content
			switch (nItem)
			{
			case ID_TOTALCOLUMNHEADER:
				DrawTotalsHeader(pDC);
				break;

			case ID_TOTALDAYSPERPERSON:
				DrawTotalsListItem(pDC, nItem, m_mapTotalDays, 2);
				break;
				
			case ID_TOTALTASKSPERPERSON:
				DrawTotalsListItem(pDC, nItem, m_mapTotalTasks, 0);
				break;
				
			case ID_PERCENTLOADPERPERSON:
				DrawTotalsListItem(pDC, nItem, m_mapPercentLoad, 1);
				break;

			default:
				ASSERT(0);
			}
		}
		return CDRF_SKIPDEFAULT;
	}

	return CDRF_DODEFAULT;
}

LRESULT CWorkloadCtrl::OnAllocationsListCustomDraw(NMLVCUSTOMDRAW* pLVCD)
{
	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		return CDRF_NOTIFYITEMDRAW;
								
	case CDDS_ITEMPREPAINT:
		{
			HWND hwndList = pLVCD->nmcd.hdr.hwndFrom;
			int nItem = (int)pLVCD->nmcd.dwItemSpec;
			
			DWORD dwTaskID = GetTaskID(nItem);

			WORKLOADITEM* pWI = NULL;
			GET_WI_RET(dwTaskID, pWI, 0L);

			CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
			
			// draw item bkgnd and gridlines full width of list
			CRect rItem;
			m_lcColumns.GetItemRect(nItem, rItem, LVIR_BOUNDS);

			COLORREF crBack = GetRowColor(nItem);
			pLVCD->clrTextBk = pLVCD->clrText = crBack;
			
			GraphicsMisc::FillItemRect(pDC, rItem, crBack, m_lcColumns);

			DrawItemDivider(pDC, rItem, DIV_HORZ, FALSE);
			
			// draw background
			GM_ITEMSTATE nState = GetItemState(nItem);
			BOOL bSelected = (nState != GMIS_NONE);
			DWORD dwFlags = (GMIB_THEMECLASSIC | GMIB_CLIPLEFT);

			GraphicsMisc::DrawExplorerItemBkgnd(pDC, m_lcColumns, nState, rItem, dwFlags);

			// draw row
			COLORREF crText = GetTreeTextColor(*pWI, bSelected, FALSE);
			pDC->SetTextColor(crText);
			
			DrawAllocationListItem(pDC, nItem, *pWI, bSelected);
		}
		return CDRF_SKIPDEFAULT;
	}

	return CDRF_DODEFAULT;
}

COLORREF CWorkloadCtrl::GetRowColor(int nItem) const
{
	BOOL bAlternate = (!m_bSavingToImage && !IsListItemLineOdd(nItem) && HasAltLineColor());
	COLORREF crBack = (bAlternate ? m_crAltLine : GetSysColor(COLOR_WINDOW));

	return crBack;
}

LRESULT CWorkloadCtrl::OnHeaderCustomDraw(NMCUSTOMDRAW* pNMCD)
{
	if (pNMCD->hdr.hwndFrom == m_hdrColumns)
	{
		switch (pNMCD->dwDrawStage)
		{
		case CDDS_PREPAINT:
			// only need handle drawing for single sorting or double row height
			if (m_sort.IsSingleSortingBy(WLCC_ALLOCTO) || (m_hdrColumns.GetRowCount() > 1))
			{
				return CDRF_NOTIFYITEMDRAW;
			}
			break;
							
		case CDDS_ITEMPREPAINT:
			// only need handle drawing for double row height
			if (m_hdrColumns.GetRowCount() > 1)
			{
				CDC* pDC = CDC::FromHandle(pNMCD->hdc);
				int nCol = (int)pNMCD->dwItemSpec;

				DrawListHeaderItem(pDC, nCol);
				return CDRF_SKIPDEFAULT;
			}
			// else alloc to selection
			return CDRF_NOTIFYPOSTPAINT;

		case CDDS_ITEMPOSTPAINT:
			{
				int nCol = (int)pNMCD->dwItemSpec;

				if (nCol == m_nSortByAllocToCol)
				{
					CDC* pDC = CDC::FromHandle(pNMCD->hdc);
					m_hdrColumns.DrawItemSortArrow(pDC, nCol, m_sort.single.bAscending);
				}
			}
			break;
		}
	}
	else if (pNMCD->hdr.hwndFrom == m_hdrTasks)
	{
		switch (pNMCD->dwDrawStage)
		{
		case CDDS_PREPAINT:
			return CDRF_NOTIFYITEMDRAW;
			
		case CDDS_ITEMPREPAINT:
			{
				// don't draw columns having min width
				CRect rItem(pNMCD->rc);
				
				if (rItem.Width() <= MIN_COL_WIDTH)
					return CDRF_DODEFAULT;
			}
			return CDRF_NOTIFYPOSTPAINT;
			
		case CDDS_ITEMPOSTPAINT:
			{
				// draw sort direction
				int nCol = (int)pNMCD->dwItemSpec;
				WLC_COLUMNID nColID = GetTreeColumnID(nCol);
				CDC* pDC = CDC::FromHandle(pNMCD->hdc);
				
				if (m_sort.IsSingleSortingBy(nColID))
				{
					m_hdrTasks.DrawItemSortArrow(pDC, nCol, m_sort.single.bAscending);
				}
			}
			break;
		}
	}
	
	return CDRF_DODEFAULT;
}

void CWorkloadCtrl::DrawSplitBar(CDC* pDC, const CRect& rSplitter, COLORREF crSplitBar)
{
	GraphicsMisc::DrawSplitBar(pDC, rSplitter, crSplitBar);
}

// Called by parent
void CWorkloadCtrl::Sort(WLC_COLUMNID nBy, BOOL bAscending)
{
	ASSERT(bAscending != -1);

	FixupListSortColumn();

	Sort(nBy, FALSE, bAscending, FALSE);
}

void CWorkloadCtrl::SetSortByAllocTo(LPCTSTR szAllocTo)
{
	m_sSortByAllocTo = szAllocTo;
}

void CWorkloadCtrl::FixupListSortColumn(LPCTSTR szAllocTo)
{
	if (!Misc::IsEmpty(szAllocTo))
	{
		m_sSortByAllocTo = szAllocTo;
	}
	else if (m_sSortByAllocTo.IsEmpty() && m_aAllocTo.GetSize())
	{
		m_sSortByAllocTo = m_aAllocTo[0];
	}

	if (m_sSortByAllocTo == ALLOCTO_TOTALID)
	{
		m_nSortByAllocToCol = (m_hdrColumns.GetItemCount() - 1);
	}
	else
	{
		m_nSortByAllocToCol = Misc::Find(m_sSortByAllocTo, m_aAllocTo);

		if (m_nSortByAllocToCol >= 0)
			m_nSortByAllocToCol++; // first column is hidden
	}
}

void CWorkloadCtrl::Sort(WLC_COLUMNID nBy, BOOL bAllowToggle, BOOL bAscending, BOOL bNotifyParent)
{
	ASSERT((nBy != WLCC_ALLOCTO) || !m_sSortByAllocTo.IsEmpty());

	m_sort.Sort(nBy, bAllowToggle, bAscending);

	// do the sort
	CHoldRedraw hr(m_tcTasks);
	CTreeListSyncer::Sort(SortProc, (DWORD)this);

	// update sort arrow
	if (nBy == WLCC_ALLOCTO)
		m_hdrColumns.Invalidate(FALSE);
	else
		m_hdrTasks.Invalidate(FALSE);

	if (bNotifyParent)
		CWnd::GetParent()->PostMessage(WM_WLCN_SORTCHANGE, m_sort.single.bAscending, m_sort.single.nBy);
}

void CWorkloadCtrl::Sort(const WORKLOADSORTCOLUMNS& multi)
{
	FixupListSortColumn();

	m_sort.Sort(multi);

	// do the sort
	CHoldRedraw hr(m_tcTasks);
	CTreeListSyncer::Sort(MultiSortProc, (DWORD)this);

	// hide sort arrow
	m_hdrTasks.Invalidate(FALSE);
}

void CWorkloadCtrl::OnBeginEditTreeLabel(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	*pResult = TRUE; // cancel our edit
	
	// notify app to edit
	if (!m_bReadOnly)
	{
		CPoint point(GetMessagePos());
		CWnd::ScreenToClient(&point);

		if (TreeHitTestItemColumn(point, FALSE) == WLCC_TITLE)
		{
			CWnd::GetParent()->SendMessage(WM_WLC_EDITTASKTITLE);
		}
	}
}

void CWorkloadCtrl::OnEndDragTreeHeader(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	m_tcTasks.InvalidateRect(NULL, TRUE);
}

void CWorkloadCtrl::OnClickTreeHeader(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	HD_NOTIFY *pHDN = (HD_NOTIFY *) pNMHDR;
	
	if (pHDN->iButton == 0) // left button
	{
		WLC_COLUMNID nColID = GetTreeColumnID(pHDN->iItem);
		Sort(nColID, TRUE, -1, TRUE);
	}
}

void CWorkloadCtrl::OnItemChangingTreeHeader(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMHEADER* pHDN = (NMHEADER*) pNMHDR;
	*pResult = 0;
	
	if (pHDN->iButton == 0) // left button
	{
		if (pHDN->pitem->mask & HDI_WIDTH)
		{
			// don't allow columns get too small
			WLC_COLUMNID nColID = GetTreeColumnID(pHDN->iItem);
			
			switch (nColID)
			{
			case WLCC_TITLE:
				if (pHDN->pitem->cxy < TREE_TITLE_MIN_WIDTH)
					*pResult = TRUE; // prevent change
				break;
				
			case WLCC_STARTDATE:
			case WLCC_DUEDATE:
			case WLCC_PERCENT:
			case WLCC_TASKID:
			case WLCC_DURATION:
				if (m_hdrTasks.IsItemVisible(pHDN->iItem))
				{
					if (pHDN->pitem->cxy < MIN_COL_WIDTH)
						pHDN->pitem->cxy = MIN_COL_WIDTH;
				}
				break;
			}
		}
	}
}

void CWorkloadCtrl::OnItemChangedTreeHeader(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	SetSplitPos(m_hdrTasks.CalcTotalItemWidth());
	Resize();
	
	m_tcTasks.UpdateWindow();
	m_lcColumns.UpdateWindow();
}

void CWorkloadCtrl::OnDblClickTreeHeaderDivider(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	CClientDC dc(&m_tcTasks);
	NMHEADER* pHDN = (NMHEADER*)pNMHDR;

	int nCol = pHDN->iItem;
	ASSERT(nCol != -1);

	RecalcTreeColumnWidth(nCol, &dc, TRUE);
	SetSplitPos(m_hdrTasks.CalcTotalItemWidth());

	Resize();
}

void CWorkloadCtrl::OnRightClickTreeHeader(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	// pass on to parent
	CWnd::GetParent()->SendMessage(WM_CONTEXTMENU, (WPARAM)GetSafeHwnd(), (LPARAM)::GetMessagePos());
}

void CWorkloadCtrl::OnTreeKeyUp(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMTVKEYDOWN* pTVKD = (NMTVKEYDOWN*)pNMHDR;
	
	switch (pTVKD->wVKey)
	{
	case VK_UP:
	case VK_DOWN:
	case VK_PRIOR:
	case VK_NEXT:
		//UpdateSelectedTaskDates();
		//SendParentSelectionUpdate();
		break;
	}
	
	*pResult = 0;
}

void CWorkloadCtrl::OnClickColumns(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	//UpdateSelectedTaskDates();
	//SendParentSelectionUpdate();
	
	*pResult = 0;
}

void CWorkloadCtrl::OnTreeGetDispInfo(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	TV_DISPINFO* pDispInfo = (TV_DISPINFO*)pNMHDR;
	DWORD dwTaskID = pDispInfo->item.lParam;
	
	const WORKLOADITEM* pWI = NULL;
	GET_WI(dwTaskID, pWI);
	
	if (pDispInfo->item.mask & TVIF_TEXT)
	{
		pDispInfo->item.pszText = (LPTSTR)(LPCTSTR)pWI->sTitle;
	}
	
	if (pDispInfo->item.mask & (TVIF_SELECTEDIMAGE | TVIF_IMAGE))
	{
		// checkbox image
		pDispInfo->item.mask |= TVIF_STATE;
		pDispInfo->item.stateMask = TVIS_STATEIMAGEMASK;
		
		if (pWI->bDone)
		{
			pDispInfo->item.state = TCHC_CHECKED;
		}
		else if (pWI->bSomeSubtaskDone)
		{
			pDispInfo->item.state = TCHC_MIXED;
		}
		else 
		{
			pDispInfo->item.state = TCHC_UNCHECKED;
		}
	}
}

void CWorkloadCtrl::OnTreeItemExpanded(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	RecalcTreeColumns(TRUE);
}

LRESULT CWorkloadCtrl::OnTreeDragEnter(WPARAM /*wp*/, LPARAM /*lp*/)
{
	// Make sure the selection helper is synchronised
	// with the tree's current selection
	m_tshDragDrop.ClearHistory();
	m_tshDragDrop.RemoveAll(TRUE, FALSE);
	m_tshDragDrop.AddItem(m_tcTasks.GetSelectedItem(), FALSE);
	
	return m_treeDragDrop.ProcessMessage(CWnd::GetCurrentMessage());
}

LRESULT CWorkloadCtrl::OnTreePreDragMove(WPARAM /*wp*/, LPARAM /*lp*/)
{
	return m_treeDragDrop.ProcessMessage(CWnd::GetCurrentMessage());
}

LRESULT CWorkloadCtrl::OnTreeDragOver(WPARAM /*wp*/, LPARAM /*lp*/)
{
	// We currently DON'T support 'linking'
	UINT nCursor = m_treeDragDrop.ProcessMessage(CWnd::GetCurrentMessage());
	
	if (nCursor == DD_DROPEFFECT_LINK)
		nCursor = DD_DROPEFFECT_NONE;
	
	return nCursor;
}

LRESULT CWorkloadCtrl::OnTreeDragDrop(WPARAM /*wp*/, LPARAM /*lp*/)
{
	if (m_treeDragDrop.ProcessMessage(CWnd::GetCurrentMessage()))
	{
		HTREEITEM htiDropTarget = NULL, htiAfterSibling = NULL;
		
		if (m_treeDragDrop.GetDropTarget(htiDropTarget, htiAfterSibling))
		{
			// Notify parent of move
			HTREEITEM htiSel = GetSelectedItem();
			ASSERT(htiSel);
			
			IUITASKMOVE move = { 0 };
			
			move.dwSelectedTaskID = GetTaskID(htiSel);
			move.dwParentID = GetTaskID(htiDropTarget);
			move.dwAfterSiblingID = GetTaskID(htiAfterSibling);
			move.bCopy = (Misc::ModKeysArePressed(MKS_CTRL) != FALSE);
			
			// If copying a task, app will send us a full update 
			// so we do not need to perform the move ourselves
			if (CWnd::GetParent()->SendMessage(WM_WLC_MOVETASK, 0, (LPARAM)&move) && !move.bCopy)
			{
				MoveSelectedItem(move);
			}
		}
	}

	return 0L;
}

LRESULT CWorkloadCtrl::OnTreeDragAbort(WPARAM /*wp*/, LPARAM /*lp*/)
{
	return m_treeDragDrop.ProcessMessage(CWnd::GetCurrentMessage());
}

void CWorkloadCtrl::OnTreeSelectionChange(NMTREEVIEW* pNMTV)
{
	if (m_bMovingTask)
		return;
	
	// Ignore setting selection to 'NULL' unless there are no tasks at all
	// because we know it's temporary only
	if ((pNMTV->itemNew.hItem == NULL) && (m_tcTasks.GetCount() != 0))
		return;
	
	// we're only interested in non-keyboard changes
	// because keyboard gets handled in OnKeyUpWorkload
	if (pNMTV->action != TVC_BYKEYBOARD)
		CWnd::GetParent()->SendMessage(WM_WLCN_SELCHANGE, 0, GetTaskID(pNMTV->itemNew.hItem));
}

LRESULT CWorkloadCtrl::ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (!IsResyncEnabled())
		return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);

	if (hRealWnd == m_lcColumns)
	{
		switch (msg)
		{
		case WM_HSCROLL:
			CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);

			// Keep totals synced
			m_lcColumnTotals.Invalidate(FALSE);
			m_lcColumnTotals.UpdateWindow();
			return 0L;

		case WM_MOUSEWHEEL:
			CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);

			if (HasHScrollBar(hRealWnd) && !HasVScrollBar(hRealWnd))
			{
				// Keep totals synced
				m_lcColumnTotals.Invalidate(FALSE);
				m_lcColumnTotals.UpdateWindow();
			}
			return 0L;

		case WM_TIMER:
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
			break;
			
		case WM_NOTIFY:
			{
				LPNMHDR pNMHDR = (LPNMHDR)lp;
				HWND hwnd = pNMHDR->hwndFrom;
				
				// let base class have its turn first
				LRESULT lr = CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);

				switch (pNMHDR->code)
				{
				case NM_RCLICK:
					if (hwnd == m_hdrColumns)
					{
						// pass on to parent
						::SendMessage(GetHwnd(), WM_CONTEXTMENU, (WPARAM)hwnd, (LPARAM)::GetMessagePos());
					}
					break;

				case HDN_ITEMCLICK:
					if (hwnd == m_hdrColumns)
					{
						OnListHeaderClick((NMHEADER*)pNMHDR);
					}
					break;

				case HDN_ITEMCHANGING:
					if (hwnd == m_hdrColumns)
					{
						NMHEADER* pHDN = (NMHEADER*)pNMHDR;
						
						// don't let user drag column too narrow
						if ((pHDN->iButton == 0) && (pHDN->pitem->mask & HDI_WIDTH))
						{
							if (m_hdrColumns.IsItemTrackable(pHDN->iItem) && (pHDN->pitem->cxy < MIN_COL_WIDTH))
								pHDN->pitem->cxy = MIN_COL_WIDTH;

							m_lcColumns.Invalidate(FALSE);
						}
					}
					break;

				}
				return lr;
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

		case WM_SETCURSOR:
			break;

		case WM_LBUTTONDBLCLK:
			if (OnListLButtonDblClk(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_LBUTTONDOWN:
			if (OnListLButtonDown(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_LBUTTONUP:
			if (OnListLButtonUp(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_MOUSEMOVE:
			if (OnListMouseMove(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_CAPTURECHANGED:
			break;

		case WM_KEYDOWN:
			break;

		case WM_RBUTTONDOWN:
			{
				DWORD dwTaskID = ListHitTestTask(lp, FALSE);

				if (dwTaskID != 0)
					SelectTask(dwTaskID);
			}
			break;

		case WM_SETFOCUS:
			m_tcTasks.SetFocus();
			break;
		}
	}
	else if (hRealWnd == m_tcTasks)
	{
		switch (msg)
		{
		case WM_RBUTTONDOWN:
			{
				DWORD dwTaskID = TreeHitTestTask(lp, FALSE);

				if (dwTaskID)
					SelectTask(dwTaskID);
			}
			break;

		case WM_LBUTTONDOWN:
			if (OnTreeLButtonDown(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_LBUTTONUP:
			if (OnTreeLButtonUp(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_LBUTTONDBLCLK:
			if (OnTreeLButtonDblClk(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_MOUSEMOVE:
			if (OnTreeMouseMove(wp, lp))
			{
				return FALSE; // eat
			}
			break;

		case WM_MOUSELEAVE:
			// Remove any drophilighting from the list
			if (m_nPrevDropHilitedItem != -1)
			{
				m_lcColumns.SetItemState(m_nPrevDropHilitedItem, 0, LVIS_DROPHILITED);
				m_nPrevDropHilitedItem = -1;
			}
			break;

		case WM_MOUSEWHEEL:
			// if we have a horizontal scrollbar but NOT a vertical scrollbar
			// then we need to redraw the whole tree to prevent artifacts
			if (HasHScrollBar(hRealWnd) && !HasVScrollBar(hRealWnd))
			{
				CHoldRedraw hr(hRealWnd, NCR_PAINT | NCR_UPDATE);

				return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);
			}
			break;

		case WM_HSCROLL:
			{
				CHoldRedraw hr(hRealWnd, NCR_PAINT | NCR_UPDATE);

				return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);
			}
			break;

		case WM_SETCURSOR:
			if (!m_bReadOnly)
			{
				CPoint ptCursor(GetMessagePos());
				DWORD dwTaskID = TreeHitTestTask(ptCursor, TRUE);

				if (dwTaskID && m_data.ItemIsLocked(dwTaskID))
					return GraphicsMisc::SetAppCursor(_T("Locked"), _T("Resources\\Cursors"));
			}
			break;
		}
	}

	// else tree or list
	switch (msg)
	{
	case WM_MOUSEWHEEL:
		{
			int zDelta = GET_WHEEL_DELTA_WPARAM(wp);

			if (zDelta != 0)
			{
				WORD wKeys = LOWORD(wp);
				
				if (wKeys == MK_CONTROL)
				{
					// TODO
				}
				else
				{
					CHoldHScroll hhs(m_tcTasks);
					
					return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);
				}
			}
		}
		break;

	case WM_KEYUP:
		{
			LRESULT lr = CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);

			NMTVKEYDOWN tvkd = { 0 };

			tvkd.hdr.hwndFrom = hRealWnd;
			tvkd.hdr.idFrom = ::GetDlgCtrlID(hRealWnd);
			tvkd.hdr.code = TVN_KEYUP;
			tvkd.wVKey = LOWORD(wp);
			tvkd.flags = lp;

			CWnd::SendMessage(WM_NOTIFY, ::GetDlgCtrlID(hRealWnd), (LPARAM)&tvkd);
			return lr;
		}
		
	case WM_VSCROLL:
		{
			CHoldHScroll hhs(m_tcTasks);
			
			return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);
		}
		break;
	}
	
	return CTreeListSyncer::ScWindowProc(hRealWnd, msg, wp, lp);
}

BOOL CWorkloadCtrl::OnMouseWheel(UINT /*nFlags*/, short zDelta, CPoint pt)
{
	// We have to handle mouse wheel over the totals because we have disabled it
	if (HasHScrollBar(m_lcColumns))
	{
		CWnd::ScreenToClient(&pt);

		if (ChildWindowFromPoint(pt) == &m_lcColumnTotals)
		{
			BOOL bScrollRight = (zDelta < 0);
			int nScroll = 3;

			while (nScroll--)
				m_lcColumns.SendMessage(WM_HSCROLL, (bScrollRight ? SB_LINERIGHT : SB_LINELEFT), 0L);

			return TRUE;
		}
	}

	return FALSE;
}

void CWorkloadCtrl::OnListHeaderClick(NMHEADER* pHDN)
{
	if (pHDN->iButton == 0) // left button
	{
		CString sSortByAllocTo;

		switch (GetListColumnType(pHDN->iItem))
		{
		case WLCT_TOTAL:
			sSortByAllocTo = ALLOCTO_TOTALID;
			break;

		case WLCT_VALUE:
			sSortByAllocTo = m_hdrColumns.GetItemText(pHDN->iItem);
			break;

		default:
			return;
		}

		BOOL bAscending = -1, bAllowToggle = TRUE;

		if (m_sort.IsSingleSortingBy(WLCC_ALLOCTO))
		{
			bAllowToggle = (m_sSortByAllocTo == sSortByAllocTo);
			bAscending = m_sort.single.bAscending;
		}

		m_sSortByAllocTo = sSortByAllocTo;
		m_nSortByAllocToCol = pHDN->iItem;

		Sort(WLCC_ALLOCTO, bAllowToggle, bAscending, TRUE);
	}
}

void CWorkloadCtrl::SetDropHilite(HTREEITEM hti, int nItem)
{
	if (m_nPrevDropHilitedItem != -1)
		m_lcColumns.SetItemState(m_nPrevDropHilitedItem, 0, LVIS_DROPHILITED);
	
	m_tcTasks.SelectDropTarget(hti);
	
	if (nItem != -1)
		m_lcColumns.SetItemState(nItem, LVIS_DROPHILITED, LVIS_DROPHILITED);
	
	m_nPrevDropHilitedItem = nItem;
}

BOOL CWorkloadCtrl::OnTreeMouseMove(UINT /*nFlags*/, CPoint /*point*/)
{
// 	if (!m_bReadOnly)
// 	{
// 	}

	// not handled
	return FALSE;
}

BOOL CWorkloadCtrl::OnTreeLButtonDown(UINT nFlags, CPoint point)
{
	HTREEITEM hti = m_tcTasks.HitTest(point, &nFlags);
	
	// Don't process if expanding an item
	if (nFlags & TVHT_ONITEMBUTTON)
		return FALSE;

// 	if (!m_bReadOnly)
// 	{
// 	}

	if (!(nFlags & TVHT_ONITEMBUTTON))
	{
		if (hti && (hti != GetTreeSelItem(m_tcTasks)))
		{
			SelectTreeItem(m_tcTasks, hti);
			return TRUE;
		}
	}

	// not handled
	return FALSE;
}

BOOL CWorkloadCtrl::OnTreeLButtonUp(UINT nFlags, CPoint point)
{
	HTREEITEM hti = m_tcTasks.HitTest(point, &nFlags);

	if (!m_bReadOnly && (nFlags & TVHT_ONITEMSTATEICON))
	{
		DWORD dwTaskID = GetTaskID(hti);
		const WORKLOADITEM* pWI = m_data.GetItem(dwTaskID);
		ASSERT(pWI);
		
		if (pWI)
			CWnd::GetParent()->SendMessage(WM_WLCN_COMPLETIONCHANGE, (WPARAM)m_tcTasks.GetSafeHwnd(), !pWI->bDone);
		
		return TRUE; // eat
	}

	// not handled
	return FALSE;
}

BOOL CWorkloadCtrl::OnTreeLButtonDblClk(UINT nFlags, CPoint point)
{
	// For reasons I don't understand, the resource context is
	// wrong when handling the double-click
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HTREEITEM hti = m_tcTasks.HitTest(point, &nFlags);
				
	if (!(nFlags & (TVHT_ONITEM | TVHT_ONITEMRIGHT)))
		return FALSE;
	
	if (!TCH().TreeCtrl().ItemHasChildren(hti))
	{
		if (TreeHitTestItemColumn(point, FALSE) == WLCC_TITLE)
		{
			m_tcTasks.EditLabel(hti);
			return TRUE;
		}
	}
	else
	{
		// Kill any built-in timers for label editing
		m_tcTasks.KillTimer(0x2A);
		m_tcTasks.KillTimer(0x2B);

		ExpandItem(hti, !TCH().IsItemExpanded(hti), TRUE);
		return TRUE;
	}

	// not handled
	return FALSE;
}

BOOL CWorkloadCtrl::OnListMouseMove(UINT /*nFlags*/, CPoint /*point*/)
{
	if (!m_bReadOnly)
	{
	}

	// not handled
	return FALSE;
}

BOOL CWorkloadCtrl::OnListLButtonDown(UINT /*nFlags*/, CPoint point)
{
	if (!m_bReadOnly)
	{
	}
	
	// don't let the selection to be set to -1
	{
		CPoint ptScreen(point);
		m_lcColumns.ClientToScreen(&ptScreen);
		
		if (HitTestTask(ptScreen) == 0)
		{
			SetFocus();
			return TRUE; // eat
		}
	}

	// not handled
	return FALSE;
}

BOOL CWorkloadCtrl::OnListLButtonUp(UINT /*nFlags*/, CPoint /*point*/)
{
	// not handled
	return FALSE;
}

BOOL CWorkloadCtrl::OnListLButtonDblClk(UINT /*nFlags*/, CPoint point)
{
	int nCol = -1;
	int nHit = ListHitTestItem(point, FALSE, nCol);
	
	if (nHit == -1)
		return TRUE; // prevent null selection

	HTREEITEM hti = CTreeListSyncer::GetTreeItem(m_tcTasks, m_lcColumns, nHit);
	ASSERT(hti == GetTreeSelItem(m_tcTasks));

	if (TCH().TreeCtrl().ItemHasChildren(hti))
	{
		ExpandItem(hti, !TCH().IsItemExpanded(hti));
		return TRUE;
	}

	// else
	if (m_bReadOnly || (GetListColumnType(nCol) != WLCT_VALUE))
		return FALSE;

	CString sAllocTo(m_hdrColumns.GetItemText(nCol));

	return CWnd::GetParent()->SendMessage(WM_WLC_EDITTASKALLOCATIONS, (WPARAM)(LPCTSTR)sAllocTo, GetTaskID(nHit));
}

BOOL CWorkloadCtrl::GetLabelEditRect(LPRECT pEdit) const
{
	HTREEITEM htiSel = GetSelectedItem();
	
	// scroll into view first
	const_cast<CWorkloadTreeCtrl&>(m_tcTasks).EnsureVisible(htiSel);

	if (m_tcTasks.GetItemRect(htiSel, pEdit, TRUE)) // label only
	{
		// make width of tree column or 200 whichever is larger
		int nWidth = (m_hdrTasks.GetItemWidth(0) - pEdit->left);
		nWidth = max(nWidth, MIN_LABEL_EDIT_WIDTH);

		pEdit->right = (pEdit->left + nWidth);

		// convert from tree to 'our' coords
		m_tcTasks.ClientToScreen(pEdit);
		CWnd::ScreenToClient(pEdit);

		return true;
	}
	
	return false;
}


void CWorkloadCtrl::SetAlternateLineColor(COLORREF crAltLine)
{
	SetColor(m_crAltLine, crAltLine);
}

void CWorkloadCtrl::SetGridLineColor(COLORREF crGridLine)
{
	SetColor(m_crGridLine, crGridLine);
}

void CWorkloadCtrl::SetOverlapColor(COLORREF crOverlap)
{
	SetColor(m_crOverlap, crOverlap);
}

void CWorkloadCtrl::SetAllocationColor(COLORREF crAllocation)
{
	SetColor(m_crAllocation, crAllocation);
}

void CWorkloadCtrl::EnableOverload(BOOL bEnable, double dOverloadValue, COLORREF crOverload)
{
	m_barChart.EnableOverload(bEnable, dOverloadValue, crOverload);
	m_lcColumnTotals.Invalidate();		
}

void CWorkloadCtrl::EnableUnderload(BOOL bEnable, double dUnderloadValue, COLORREF crUnderload)
{
	m_barChart.EnableUnderload(bEnable, dUnderloadValue, crUnderload);
	m_lcColumnTotals.Invalidate();
}

void CWorkloadCtrl::SetSplitBarColor(COLORREF crSplitBar) 
{ 
	CTreeListSyncer::SetSplitBarColor(crSplitBar); 
}

void CWorkloadCtrl::SetBackgroundColor(COLORREF crBkgnd)
{
	if (SetColor(m_crBkgnd, crBkgnd) && m_lcTotalsLabels.GetSafeHwnd())
	{
		m_lcTotalsLabels.SetBkColor(m_crBkgnd);
		m_lcTotalsLabels.SetTextBkColor(m_crBkgnd);
	}
}

BOOL CWorkloadCtrl::SetColor(COLORREF& color, COLORREF crNew)
{
	if (crNew == color)
		return FALSE;

	color = crNew;

	if (GetSafeHwnd())
		InvalidateAll();

	return TRUE;
}

CString CWorkloadCtrl::FormatDate(const COleDateTime& date, DWORD dwFlags) const
{
	dwFlags &= ~DHFD_ISO;
	dwFlags |= (HasOption(WLCF_DISPLAYISODATES) ? DHFD_ISO : 0);

	return CDateHelper::FormatDate(date, dwFlags);
}

int CWorkloadCtrl::GetLongestVisibleDuration(HTREEITEM hti) const
{
	int nLongest = 0;

	if (hti)
	{
		DWORD dwTaskID = GetTaskID(hti);

		const WORKLOADITEM* pWI = NULL;
		GET_WI_RET(dwTaskID, pWI, 0);

		if (pWI->HasValidDates())
			nLongest = pWI->dtRange.GetWeekdayCount();
	}

	// children
	if (!hti || TCH().IsItemExpanded(hti))
	{
		HTREEITEM htiChild = m_tcTasks.GetChildItem(hti);

		while (htiChild)
		{
			int nLongestChild = GetLongestVisibleDuration(htiChild);
			nLongest = max(nLongest, nLongestChild);

			htiChild = m_tcTasks.GetNextItem(htiChild, TVGN_NEXT);
		}
	}

	return nLongest;
}

double CWorkloadCtrl::GetLargestVisibleTimeEstimate(HTREEITEM hti) const
{
	double dLargest = 0.0;

	if (hti)
	{
		DWORD dwTaskID = GetTaskID(hti);

		const WORKLOADITEM* pWI = NULL;
		GET_WI_RET(dwTaskID, pWI, 0);

		dLargest = pWI->dTimeEst;
	}

	// children
	if (!hti || TCH().IsItemExpanded(hti))
	{
		HTREEITEM htiChild = m_tcTasks.GetChildItem(hti);

		while (htiChild)
		{
			double dLargestChild = GetLongestVisibleDuration(htiChild);
			dLargest = max(dLargest, dLargestChild);

			htiChild = m_tcTasks.GetNextItem(htiChild, TVGN_NEXT);
		}
	}

	return dLargest;
}

CString CWorkloadCtrl::GetTreeItemColumnText(const WORKLOADITEM& wi, WLC_COLUMNID nCol) const
{
	CString sItem;

	switch (nCol)
	{
		case WLCC_TITLE:
			sItem = wi.sTitle;
			break;

		case WLCC_TASKID:
			sItem.Format(_T("%ld"), wi.dwTaskID);
			break;
			
		case WLCC_STARTDATE:
			if (wi.HasStartDate())
				sItem = FormatDate(wi.dtRange.m_dtStart);
			break;

		case WLCC_DUEDATE:
			if (wi.HasDueDate())
				sItem = FormatDate(wi.dtRange.m_dtEnd);
			break;

		case WLCC_TIMEEST:
			if (wi.dTimeEst > 0)
				sItem.Format(CEnString(IDS_TIMEEST_FORMAT), wi.dTimeEst);
			break;

		case WLCC_DURATION:
			if (wi.HasValidDates())
				sItem.Format(CEnString(IDS_ALLOCATION_FORMAT), wi.dtRange.GetWeekdayCount());
			break;

		case WLCC_PERCENT:
			sItem.Format(_T("%d%%"), wi.nPercent);
			break;
	}

	return sItem;
}

void CWorkloadCtrl::DrawTreeItem(CDC* pDC, HTREEITEM hti, const WORKLOADITEM& wi, BOOL bSelected, COLORREF crBack)
{
	int nNumCol = m_hdrTasks.GetItemCount();

	for (int nCol = 0; nCol < nNumCol; nCol++)
		DrawTreeItemText(pDC, hti, nCol, wi, bSelected, crBack);
}

void CWorkloadCtrl::DrawTreeItemText(CDC* pDC, HTREEITEM hti, int nCol, const WORKLOADITEM& wi, BOOL bSelected, COLORREF crBack)
{
	CRect rItem;
	GetTreeItemRect(hti, nCol, rItem);

	if (rItem.Width() == 0)
		return;

	DrawItemDivider(pDC, rItem, DIV_VERT_LIGHT, bSelected);

	WLC_COLUMNID nColID = GetTreeColumnID(nCol);
	BOOL bTitleCol = (nColID == WLCC_TITLE);

	// draw item background colour
	if (!bSelected && (crBack != CLR_NONE))
	{
		CRect rBack(rItem);

		if (bTitleCol)
			GetTreeItemRect(hti, WLCC_TITLE, rBack, TRUE); // label only
		
		// don't overwrite gridlines
		if (m_crGridLine != CLR_NONE)
			rBack.DeflateRect(0, 0, 1, 1);
		
		pDC->FillSolidRect(rBack, crBack);
	}
	
	if (rItem.Width() <= MIN_COL_WIDTH)
		return;

	// draw text
	CString sItem = GetTreeItemColumnText(wi, nColID);

	if (!sItem.IsEmpty())
	{
		if (bTitleCol)
			rItem.DeflateRect(2, 2, 1, 0);
		else
			rItem.DeflateRect(LV_COLPADDING, 2, LV_COLPADDING, 0);

		// text color and alignment
		BOOL bLighter = FALSE; 
		UINT nFlags = (DT_LEFT | DT_VCENTER | DT_NOPREFIX | GraphicsMisc::GetRTLDrawTextFlags(m_tcTasks));

		switch (nColID)
		{
		case WLCC_TITLE:
			nFlags |= DT_END_ELLIPSIS;
			break;

		case  WLCC_TASKID:
		case WLCC_DURATION:
			nFlags |= DT_RIGHT;
			break;
			
		case WLCC_STARTDATE:
		case WLCC_DUEDATE:
			{
				// Right-align if the column width can show the entire date
				// else keep left align to ensure day and month remain visible
				if (rItem.Width() >= pDC->GetTextExtent(sItem).cx)
					nFlags |= DT_RIGHT;
			}
			break;
			
		case WLCC_PERCENT:
			nFlags |= DT_CENTER;
			break;
		}

		COLORREF crText = GetTreeTextColor(wi, bSelected, bLighter);
		COLORREF crOldColor = pDC->SetTextColor(crText);
		HGDIOBJ hFontOld = pDC->SelectObject(GetTreeItemFont(hti, wi, nColID));
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(sItem, rItem, nFlags);
		pDC->SetTextColor(crOldColor);
		pDC->SelectObject(hFontOld);
	}

	// special case: drawing shortcut icon for reference tasks
	if (bTitleCol && wi.dwOrgRefID)
	{
		GetTreeItemRect(hti, nCol, rItem, TRUE);
		CPoint ptIcon(rItem.left, rItem.bottom - 32);

		ShellIcons::DrawIcon(pDC, ShellIcons::SI_SHORTCUT, ptIcon, true);
	}
}

BOOL CWorkloadCtrl::IsVerticalDivider(DIV_TYPE nType)
{
	switch (nType)
	{
	case DIV_VERT_LIGHT:
	case DIV_VERT_MID:
	case DIV_VERT_DARK:
		return TRUE;
	}
	
	// else
	return FALSE;
}

void CWorkloadCtrl::DrawItemDivider(CDC* pDC, const CRect& rItem, DIV_TYPE nType, BOOL bSelected)
{
	if (!HasGridlines() || (nType == DIV_NONE) || (IsVerticalDivider(nType) && (rItem.right < 0)))
		return;

	COLORREF color = m_crGridLine;

	switch (nType)
	{
	case DIV_VERT_LIGHT:
	case DIV_HORZ:
		break;
		
	case DIV_VERT_MID:
		color = GraphicsMisc::Darker(m_crGridLine, 0.25);
		break;
		
	case DIV_VERT_DARK:
		color = GraphicsMisc::Darker(m_crGridLine, 0.5);
		break;
	}

	CRect rDiv(rItem);

	if (nType == DIV_HORZ)
	{
		rDiv.top = (rDiv.bottom - 1);
	}
	else
	{
		rDiv.left = (rDiv.right - 1);
		
		if (bSelected)
			rDiv.DeflateRect(0, 1);
	}

	COLORREF crOld = pDC->GetBkColor();

	pDC->FillSolidRect(rDiv, color);
	pDC->SetBkColor(crOld);
}


HFONT CWorkloadCtrl::GetTreeItemFont(HTREEITEM hti, const WORKLOADITEM& wi, WLC_COLUMNID nCol)
{
	BOOL bStrikThru = (HasOption(WLCF_STRIKETHRUDONETASKS) && wi.bDone);
	BOOL bBold = ((nCol == WLCC_TITLE) && (m_tcTasks.GetParentItem(hti) == NULL));
	
	return m_tcTasks.Fonts().GetHFont(bBold, FALSE, FALSE, bStrikThru);
}

void CWorkloadCtrl::GetTreeItemRect(HTREEITEM hti, int nCol, CRect& rItem, BOOL bText) const
{
	rItem.SetRectEmpty();

	if (m_tcTasks.GetItemRect(hti, rItem, TRUE)) // text rect only
	{
		WLC_COLUMNID nColID = GetTreeColumnID(nCol);

		switch (nColID)
		{
		case WLCC_TITLE:
			{
				int nColWidth = m_hdrTasks.GetItemWidth(0); // always
	
				if (!bText)
					rItem.right = nColWidth;
				else
					rItem.right = min(rItem.right, nColWidth);
			}
			break;

		case WLCC_TASKID:
		case WLCC_STARTDATE:
		case WLCC_DUEDATE:
		case WLCC_PERCENT:
		case WLCC_DURATION:
		case WLCC_TIMEEST:
			{
				CRect rHdrItem;
				m_hdrTasks.GetItemRect(nCol, rHdrItem);
				
				rItem.left = rHdrItem.left;
				rItem.right = rHdrItem.right;
			}
			break;

		case WLCC_NONE:
		default:
			ASSERT(0);
			break;
		}
	}

	if (m_bSavingToImage)
		rItem.OffsetRect(-1, 0);
}

CString CWorkloadCtrl::GetListItemColumnText(const WORKLOADITEM& wi, int nCol, int nDecimals, BOOL bSelected, COLORREF& crBack) const
{
	crBack = CLR_NONE;

	switch (GetListColumnType(nCol))
	{
	case WLCT_TOTAL:
		return wi.mapAllocatedDays.FormatTotalDays(nDecimals);
		
	case WLCT_VALUE:
		{
			int nAllocTo = (nCol - 1);
			ASSERT(nAllocTo < m_aAllocTo.GetSize());
			
			CString sAllocTo = m_aAllocTo[nAllocTo];
			CString sDays = wi.mapAllocatedDays.FormatDays(sAllocTo, nDecimals);

			if (!sDays.IsEmpty())
			{
				if ((m_crOverlap != CLR_NONE) && wi.mapAllocatedDays.IsOverlapping(sAllocTo))
				{
					crBack = m_crOverlap;
				}
				else if (!bSelected && (m_crAllocation != CLR_NONE))
				{
					crBack = m_crAllocation;
				}
			}

			return sDays;
		}
		break;
	}

	return _T("");
}

CString CWorkloadCtrl::GetListItemColumnTotal(const CMapAllocationTotals& mapTotals, int nCol, int nDecimals) const
{
	switch (GetListColumnType(nCol))
	{
	case WLCT_TOTAL:
		return mapTotals.FormatTotal(nDecimals);

	case WLCT_VALUE:
		{
			int nAllocTo = (nCol - 1);
			ASSERT(nAllocTo < m_aAllocTo.GetSize());

			CString sAllocTo = m_aAllocTo[nAllocTo];
			return mapTotals.Format(sAllocTo, nDecimals);
		}
		break;
	}

	return _T("");
}

void CWorkloadCtrl::DrawAllocationListItem(CDC* pDC, int nItem, const WORKLOADITEM& wi, BOOL bSelected)
{
	ASSERT(nItem != -1);
	int nNumCol = GetRequiredListColumnCount();

	for (int nCol = 1; nCol < nNumCol; nCol++)
	{
		CRect rColumn;
		m_lcColumns.GetSubItemRect(nItem, nCol, LVIR_BOUNDS, rColumn);
		
		DrawItemDivider(pDC, rColumn, DIV_VERT_LIGHT, bSelected);
		rColumn.right--;
		rColumn.bottom--;

		COLORREF crBack = CLR_NONE;
		CString sDays = GetListItemColumnText(wi, nCol, 2, bSelected, crBack);
		
		if (!sDays.IsEmpty())
		{
			if (crBack != CLR_NONE)
			{
				if (bSelected)
				{
					GraphicsMisc::DrawRect(pDC, rColumn, CLR_NONE, crBack);
				}
				else
				{
					pDC->FillSolidRect(rColumn, crBack);
					pDC->SetTextColor(GraphicsMisc::GetBestTextColor(crBack));
				}
			}
			else
			{
				pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
			}

			rColumn.DeflateRect(LV_COLPADDING, 1, LV_COLPADDING, 0);
			pDC->DrawText(sDays, (LPRECT)(LPCRECT)rColumn, DT_CENTER);
		}
	}
}

void CWorkloadCtrl::DrawTotalsListItem(CDC* pDC, int nItem, const CMapAllocationTotals& mapTotals, int nDecimals)
{
	ASSERT(nItem != -1);
	int nNumCol = GetRequiredListColumnCount();
	
	for (int nCol = 1; nCol < nNumCol; nCol++)
	{
		CRect rColumn;
		m_lcColumnTotals.GetSubItemRect(nItem, nCol, LVIR_BOUNDS, rColumn);
			
		// Offset for allocation label horz scroll
		rColumn.OffsetRect(-m_lcColumns.GetScrollPos(SB_HORZ) - 1, 0);

		CString sValue = GetListItemColumnTotal(mapTotals, nCol, nDecimals);
			
		if (!sValue.IsEmpty())
		{
			if (nItem == ID_PERCENTLOADPERPERSON)
			{
				COLORREF crBack = CLR_NONE;

				switch (GetListColumnType(nCol))
				{
				case WLCT_VALUE:
					crBack = m_barChart.GetBkgndColor(mapTotals.Get(m_aAllocTo[nCol - 1]));
					break;
					
				case WLCT_TOTAL:
					crBack = m_barChart.GetBkgndColor(mapTotals.GetTotal());
					break;
				}

				if (crBack != CLR_NONE)
				{
					pDC->FillSolidRect(rColumn, crBack);
					pDC->SetTextColor(GraphicsMisc::GetBestTextColor(crBack));
				}
	
				sValue += '%';
			}
		
			CRect rText(rColumn);
			rText.DeflateRect(LV_COLPADDING, 1, LV_COLPADDING, 0);

			pDC->DrawText(sValue, (LPRECT)(LPCRECT)rText, DT_CENTER);
		}
			
		DrawItemDivider(pDC, rColumn, DIV_VERT_LIGHT, FALSE);
	}
}

void CWorkloadCtrl::DrawTotalsHeader(CDC* pDC)
{
	int nNumCol = GetRequiredListColumnCount();
	
	for (int nCol = 1; nCol < nNumCol; nCol++)
	{
		CRect rColumn;
		m_lcColumnTotals.GetSubItemRect(ID_TOTALCOLUMNHEADER, nCol, LVIR_BOUNDS, rColumn);

		// Offset for allocation label horz scroll
		rColumn.OffsetRect(-m_lcColumns.GetScrollPos(SB_HORZ), 0);
		rColumn.right--;

		DrawListHeaderRect(pDC, rColumn, m_hdrColumns.GetItemText(nCol));
	}
}

void CWorkloadCtrl::DrawListHeaderItem(CDC* /*pDC*/, int nCol)
{
	CRect rItem;
	m_hdrColumns.GetItemRect(nCol, rItem);

	if (nCol == 0)
		return;

	// TODO
}

void CWorkloadCtrl::DrawListHeaderRect(CDC* pDC, const CRect& rItem, const CString& sItem)
{
	if (m_themeHeader.AreControlsThemed())
	{
		m_themeHeader.DrawBackground(pDC, HP_HEADERITEM, HIS_NORMAL, rItem);
	}
	else
	{
		pDC->FillSolidRect(rItem, ::GetSysColor(COLOR_3DFACE));
		pDC->Draw3dRect(rItem, ::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DSHADOW));
	}
	
	// text
	if (!sItem.IsEmpty())
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));

		const UINT nFlags = (DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_CENTER | GraphicsMisc::GetRTLDrawTextFlags(m_hdrColumns));
		pDC->DrawText(sItem, (LPRECT)(LPCRECT)rItem, nFlags);
	}
}

COLORREF CWorkloadCtrl::GetTreeTextBkColor(const WORKLOADITEM& wi, BOOL bSelected, BOOL bAlternate) const
{
	COLORREF crTextBk = wi.GetTextBkColor(bSelected, HasOption(WLCF_TASKTEXTCOLORISBKGND));

	if (crTextBk == CLR_NONE)
	{
		if (!m_bSavingToImage && bAlternate && HasAltLineColor())
			crTextBk = m_crAltLine;
		else
			crTextBk = GetSysColor(COLOR_WINDOW);
	}
	
	return crTextBk;
}

COLORREF CWorkloadCtrl::GetTreeTextColor(const WORKLOADITEM& wi, BOOL bSelected, BOOL bLighter) const
{
	COLORREF crText = wi.GetTextColor(bSelected, HasOption(WLCF_TASKTEXTCOLORISBKGND));
	ASSERT(crText != CLR_NONE);

	if (!m_bSavingToImage)
	{
		if (bSelected)
		{
			crText = GraphicsMisc::GetExplorerItemTextColor(crText, GMIS_SELECTED, GMIB_THEMECLASSIC);
		}
		else if (bLighter)
		{
			crText = GraphicsMisc::Lighter(crText, 0.5);
		}
	}

	return crText;
}

HTREEITEM CWorkloadCtrl::GetTreeItem(DWORD dwTaskID) const
{
	HTREEITEM hti = NULL;
	m_mapHTItems.Lookup(dwTaskID, hti);
	
	return hti;
}

int CWorkloadCtrl::GetListItem(DWORD dwTaskID) const
{
	HTREEITEM hti = GetTreeItem(dwTaskID);

	return (hti ? GetListItem(hti) : -1);
}

COLORREF CWorkloadCtrl::GetColor(COLORREF crBase, double dLighter, BOOL bSelected)
{
	COLORREF crResult(crBase);

	if (dLighter > 0.0)
		crResult = GraphicsMisc::Lighter(crResult, dLighter);

	if (bSelected)
		crResult = GraphicsMisc::Darker(crResult, 0.15);

	return crResult;
}

int CWorkloadCtrl::GetListItem(HTREEITEM htiFrom) const
{
	if (!htiFrom)
		return -1;

	return CTreeListSyncer::FindListItem(m_lcColumns, (DWORD)htiFrom);
}

void CWorkloadCtrl::ExpandList()
{
	int nNextIndex = 0;
	ExpandList(NULL, nNextIndex);
}

void CWorkloadCtrl::ExpandList(HTREEITEM htiFrom, int& nNextIndex)
{
	CTreeListSyncer::ExpandList(m_lcColumns, m_tcTasks, htiFrom, nNextIndex);
}

void CWorkloadCtrl::CollapseList(HTREEITEM htiFrom)
{
	CTreeListSyncer::CollapseList(m_lcColumns, m_tcTasks, htiFrom);
}

void CWorkloadCtrl::DeleteTreeItem(HTREEITEM htiFrom)
{
	ASSERT(htiFrom);

	DWORD dwTaskID = GetTaskID(htiFrom);

	m_tcTasks.DeleteItem(htiFrom);
	VERIFY(m_data.RemoveKey(dwTaskID));
}

WLC_COLUMNID CWorkloadCtrl::GetTreeColumnID(int nCol) const
{
	return (WLC_COLUMNID)m_hdrTasks.GetItemData(nCol);
}

WLC_LISTCOLUMNTYPE CWorkloadCtrl::GetListColumnType(int nCol) const
{
	return (WLC_LISTCOLUMNTYPE)m_hdrColumns.GetItemData(nCol);
}

void CWorkloadCtrl::ResizeColumnsToFit()
{
	// tree columns
	CClientDC dc(&m_tcTasks);
	int nNumCol = m_hdrTasks.GetItemCount();

	for (int nCol = 1; nCol < nNumCol; nCol++)
		RecalcTreeColumnWidth(nCol, &dc, TRUE);

	// list columns
	RecalcListColumnsToFit();
}

void CWorkloadCtrl::RecalcListColumnsToFit()
{
	// Calc widest column first
	CClientDC dc(&m_lcColumns);
	CFont* pOldFont = GraphicsMisc::PrepareDCFont(&dc, m_lcColumns);

	int nMaxHeaderWidth = dc.GetTextExtent("100.0%").cx;
	int nCol = GetRequiredListColumnCount();

	while (nCol--)
	{
		CString sAllocTo = m_hdrColumns.GetItemText(nCol);

		int nHeaderWidth = dc.GetTextExtent(sAllocTo).cx;
		nMaxHeaderWidth = max(nMaxHeaderWidth, nHeaderWidth);

		int nTotalWidth = dc.GetTextExtent(m_mapTotalDays.Format(sAllocTo, 2)).cx;
		nMaxHeaderWidth = max(nMaxHeaderWidth, nTotalWidth);
	}
	nMaxHeaderWidth += (2 * HD_COLPADDING);

	// cleanup
	dc.SelectObject(pOldFont);

	// Resize all columns to that width (except first column)
	int nNumCols = GetRequiredListColumnCount();

	for (nCol = 1; nCol < nNumCols; nCol++)
	{
		if (GetListColumnType(nCol) == WLCT_EMPTY)
		{
			int nWidth = GraphicsMisc::ScaleByDPIFactor(10);

			m_hdrColumns.SetItemWidth(nCol, nWidth);
			m_lcColumnTotals.SetColumnWidth(nCol, nWidth);
		}
		else
		{
			m_hdrColumns.SetItemWidth(nCol, nMaxHeaderWidth);
			m_lcColumnTotals.SetColumnWidth(nCol, nMaxHeaderWidth);
		}
	}

	Resize();
	RemoveTotalsScrollbars();
}

void CWorkloadCtrl::RemoveTotalsScrollbars()
{
	if (m_lcColumnTotals.GetStyle() & (WS_VSCROLL | WS_HSCROLL))
		m_lcColumnTotals.ModifyStyle(WS_VSCROLL | WS_HSCROLL, 0, SWP_FRAMECHANGED);

	if (m_lcTotalsLabels.GetStyle() & (WS_VSCROLL | WS_HSCROLL))
		m_lcTotalsLabels.ModifyStyle(WS_VSCROLL | WS_HSCROLL, 0, SWP_FRAMECHANGED);
}

void CWorkloadCtrl::OnNotifySplitterChange(int nSplitPos)
{
	CTreeListSyncer::OnNotifySplitterChange(nSplitPos);

	// Adjust 'Title' column to suit
	int nRestOfColsWidth = m_hdrTasks.CalcTotalItemWidth(0);

	CClientDC dc(this);
	GraphicsMisc::PrepareDCFont(&dc, m_tcTasks);

	int nMinColWidth = CalcWidestItemTitle(NULL, &dc, FALSE);
	int nTitleColWidth = max(nMinColWidth, (nSplitPos - nRestOfColsWidth));

	m_hdrTasks.SetItemWidth(WLCC_TITLE, nTitleColWidth);

	if (m_bSplitting)
		m_hdrTasks.SetItemTracked(WLCC_TITLE, TRUE);

	m_hdrTasks.UpdateWindow();

	m_tcTasks.SetTitleColumnWidth(nTitleColWidth);

	ResyncTotalsPositions();
	UpdateWindow();
}

BOOL CWorkloadCtrl::HandleEraseBkgnd(CDC* pDC)
{
	CTreeListSyncer::HandleEraseBkgnd(pDC);

	CDialogHelper::ExcludeChild(&m_lcTotalsLabels, pDC);
	CDialogHelper::ExcludeChild(&m_lcColumnTotals, pDC);
	CDialogHelper::ExcludeChild(&m_barChart, pDC);
	
	CRect rClient;
	CWnd::GetClientRect(rClient);
	
	pDC->FillSolidRect(rClient, m_crBkgnd);
	return TRUE;
}

BOOL CWorkloadCtrl::RecalcTreeColumns(BOOL bResize)
{
	CClientDC dc(&m_tcTasks);

	int nNumCols = m_hdrTasks.GetItemCount();
	BOOL bChange = FALSE;

	for (int nCol = 0; nCol < nNumCols; nCol++)
	{
		switch (GetTreeColumnID(nCol))
		{
		case WLCC_TITLE:
		case WLCC_TASKID:
		case WLCC_DURATION:
			if (!m_hdrTasks.IsItemTracked(nCol))
			{
				int nCurWidth = m_hdrTasks.GetItemWidth(nCol);

				if (RecalcTreeColumnWidth(nCol, &dc, FALSE) != nCurWidth)
					bChange = TRUE;
			}
			break;
		}
	}

	if (bChange && bResize)
		Resize();

	return bChange;
}

int CWorkloadCtrl::RecalcTreeColumnWidth(int nCol, CDC* pDC, BOOL bForce)
{
	if (!bForce && m_hdrTasks.IsItemTracked(nCol))
		return m_hdrTasks.GetItemWidth(nCol);

	int nColWidth = CalcTreeColumnWidth(nCol, pDC);
	m_hdrTasks.SetItemWidth(nCol, nColWidth);

	return nColWidth;
}

int CWorkloadCtrl::CalcTreeColumnWidth(int nCol, CDC* pDC) const
{
	ASSERT(pDC);
	CFont* pOldFont = GraphicsMisc::PrepareDCFont(pDC, m_tcTasks);

	int nColWidth = 0;
	WLC_COLUMNID nColID = GetTreeColumnID(nCol);

	switch (nColID)
	{
	case WLCC_TITLE:
		nColWidth = CalcWidestItemTitle(NULL, pDC, TRUE);
		nColWidth = max(nColWidth, TREE_TITLE_MIN_WIDTH);
		break;

	case WLCC_TASKID:
		nColWidth = pDC->GetTextExtent(Misc::Format(m_dwMaxTaskID)).cx;
		break;
		
	// rest of attributes are fixed width
	case WLCC_STARTDATE:
	case WLCC_DUEDATE: 
		{
			COleDateTime date(2015, 12, 31, 23, 59, 59);
			nColWidth = GraphicsMisc::GetAverageMaxStringWidth(FormatDate(date), pDC);
		}
		break;

	case WLCC_DURATION:
		nColWidth = pDC->GetTextExtent(CEnString(IDS_ALLOCATION_FORMAT, GetLongestVisibleDuration(NULL))).cx;
		break;

	case WLCC_TIMEEST:
		nColWidth = pDC->GetTextExtent(CEnString(IDS_TIMEEST_FORMAT, GetLargestVisibleTimeEstimate(NULL))).cx;
		break;
		
	case WLCC_PERCENT: 
		nColWidth = GraphicsMisc::GetAverageMaxStringWidth(_T("100%"), pDC);
		break;

	default:
		ASSERT(0);
	}

	if (nColWidth < MIN_COL_WIDTH)
		nColWidth = MIN_COL_WIDTH;
	else
		nColWidth += (2 * LV_COLPADDING);
	
	// take max of this and column title
	int nTitleWidth = (m_hdrTasks.GetItemTextWidth(nCol, pDC) + (2 * HD_COLPADDING));
	ASSERT(nTitleWidth);

	pDC->SelectObject(pOldFont);

	return max(nTitleWidth, nColWidth);
}

int CWorkloadCtrl::CalcWidestItemTitle(HTREEITEM htiParent, CDC* pDC, BOOL bEnd) const
{
	// we only want parents
	HTREEITEM htiChild = m_tcTasks.GetChildItem(htiParent);
	
	if (htiChild == NULL)
		return 0;
	
	// or we only want expanded items
	if (htiParent && !TCH().IsItemExpanded(htiParent, FALSE))
		return 0;
	
	// Prepare font
	HFONT hFont = NULL, hOldFont = NULL;
	
	if (bEnd)
	{
		CFontCache& fonts = const_cast<CFontCache&>(m_tcTasks.Fonts());

		hFont = fonts.GetHFont((htiParent == NULL) ? GMFS_BOLD : 0);
		hOldFont = (HFONT)pDC->SelectObject(hFont);
	}
	
	// else try children
	int nWidest = 0;
	
	while (htiChild)
	{
		CRect rChild;
		
		if (m_tcTasks.GetItemRect(htiChild, rChild, TRUE)) // text rect only
		{
			int nWidth = 0;

			if (bEnd)
			{
				DWORD dwTaskID = GetTaskID(htiChild);
				const WORKLOADITEM* pWI = NULL;
			
				GET_WI_RET(dwTaskID, pWI, 0);
			
				int nTextWidth = pDC->GetTextExtent(pWI->sTitle).cx;
				nWidth = max(nTextWidth, (rChild.left + nTextWidth));
			}
			else
			{
				nWidth = (rChild.left + TREE_TITLE_MIN_WIDTH);
			}
			
			int nWidestChild = CalcWidestItemTitle(htiChild, pDC, bEnd); // RECURSIVE CALL
			nWidest = max(max(nWidest, nWidth), nWidestChild);
		}
		
		htiChild = m_tcTasks.GetNextItem(htiChild, TVGN_NEXT);
	}
	
	pDC->SelectObject(hOldFont);
	
	return nWidest;
}

void CWorkloadCtrl::BuildListColumns()
{
	// once only
	if (m_hdrColumns.GetItemCount())
		return;

	// add empty column as placeholder so we can
	// easily replace the other columns without
	// losing all our items too
	LVCOLUMN lvc = { LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, 0 };
	
	m_lcColumns.InsertColumn(0, &lvc);
	m_hdrColumns.SetItemData(0, WLCT_EMPTY);
	m_lcColumnTotals.InsertColumn(0, &lvc);

	UpdateListColumns();
}

void CWorkloadCtrl::UpdateListColumns()
{
	Misc::SortArray(m_aAllocTo);

	int nNumCols = m_hdrColumns.GetItemCount();
	int nReqCols = GetRequiredListColumnCount();
	int nDiffCols = (nReqCols - nNumCols);

	if (nDiffCols > 0)
	{
		// add other columns
		LVCOLUMN lvc = { LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, 0 };

		lvc.cx = 100;
		lvc.fmt = LVCFMT_CENTER | HDF_STRING;
		lvc.pszText = _T("");
		lvc.cchTextMax = 50;
		
		for (int i = 0, nCol = nNumCols; i < nDiffCols; i++, nCol++)
		{
			m_lcColumns.InsertColumn(nCol, &lvc);
			m_lcColumnTotals.InsertColumn(nCol, &lvc);
		}
	}
	else if (nDiffCols < 0)
	{
		// don't delete first (hidden) column
		int i = nNumCols;

		while (i-- > nReqCols)
		{
			m_lcColumns.DeleteColumn(i);
			m_lcColumnTotals.DeleteColumn(i);
		}
	}
	ASSERT(m_hdrColumns.GetItemCount() == nReqCols);

	if (nDiffCols != 0)
		PostResize();

	// Update column header text and type
	for (int nCol = 1; nCol < nReqCols; nCol++)
	{
		if (nCol == (nReqCols - 2))
		{
			m_hdrColumns.SetItemData(nCol, WLCT_EMPTY);
			m_hdrColumns.SetItemText(nCol, _T(""));
		}
		else if (nCol == (nReqCols - 1))
		{
			m_hdrColumns.SetItemData(nCol, WLCT_TOTAL);
			m_hdrColumns.SetItemText(nCol, CEnString(IDS_COL_TOTAL));
		}
		else
		{
			m_hdrColumns.SetItemData(nCol, WLCT_VALUE);
			m_hdrColumns.SetItemText(nCol, m_aAllocTo[nCol - 1]);
		}
	}

	RecalcListColumnsToFit();
}

int CALLBACK CWorkloadCtrl::MultiSortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	const CWorkloadCtrl* pThis = (CWorkloadCtrl*)lParamSort;
	const WORKLOADSORTCOLUMNS& sort = pThis->m_sort.multi;

	int nCompare = 0;

	for (int nCol = 0; ((nCol < 3) && (nCompare == 0)); nCol++)
	{
		if (sort.cols[nCol].nBy == TDCA_NONE)
			break;

		nCompare = pThis->CompareTasks(lParam1, lParam2, sort.cols[nCol]);
	}

	return nCompare;
}

int CALLBACK CWorkloadCtrl::SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	const CWorkloadCtrl* pThis = (CWorkloadCtrl*)lParamSort;
	
	return pThis->CompareTasks(lParam1, lParam2, pThis->m_sort.single);
}

int CWorkloadCtrl::CompareTasks(DWORD dwTaskID1, DWORD dwTaskID2, const WORKLOADSORTCOLUMN& col) const
{
	int nCompare = 0;

	// Optimise for task ID
	if (col.nBy == WLCC_TASKID)
	{
		nCompare = (dwTaskID1 - dwTaskID2);
	}
	else
	{
		const WORKLOADITEM* pWI1 = GetWorkloadItem(dwTaskID1);
		const WORKLOADITEM* pWI2 = GetWorkloadItem(dwTaskID2);

		if (!pWI1 || !pWI2)
		{
			ASSERT(0);
			return 0;
		}

		switch (col.nBy)
		{
		case WLCC_TITLE:
			nCompare = Compare(pWI1->sTitle, pWI2->sTitle);
			break;

		case WLCC_STARTDATE:
			nCompare = CDateHelper::Compare(pWI1->dtRange.m_dtStart, pWI2->dtRange.m_dtStart, DHC_COMPARETIME);
			break;

		case WLCC_DUEDATE:
			nCompare = CDateHelper::Compare(pWI1->dtRange.m_dtEnd, pWI2->dtRange.m_dtEnd, DHC_COMPARETIME | DHC_NOTIMEISENDOFDAY);
			break;

		case WLCC_DURATION:
			nCompare = (pWI1->dtRange.GetWeekdayCount() - pWI2->dtRange.GetWeekdayCount());
			break;

		case WLCC_TIMEEST:
			{
				double dDiff = pWI1->dTimeEst - pWI2->dTimeEst;
				
				nCompare = ((dDiff > 0.0) ? 1 : ((dDiff < 0.0) ? -1 : 0));
			}
			break;

		case WLCC_PERCENT:
			nCompare = (pWI1->nPercent - pWI2->nPercent);
			break;

		case WLCC_NONE:
			nCompare = (pWI1->nPosition - pWI2->nPosition);
			break;

		case WLCC_ALLOCTO:
			{
				ASSERT(!m_sSortByAllocTo.IsEmpty());

				double dDays1 = pWI1->mapAllocatedDays.GetDays(m_sSortByAllocTo);
				double dDays2 = pWI2->mapAllocatedDays.GetDays(m_sSortByAllocTo);

				nCompare = (int)(dDays1 - dDays2);
			}
			break;

		default:
			ASSERT(0);
			break;
		}
	}

	return (col.bAscending ? nCompare : -nCompare);
}

int CWorkloadCtrl::Compare(const CString& sText1, const CString& sText2)
{
	BOOL bEmpty1 = sText1.IsEmpty();
	BOOL bEmpty2 = sText2.IsEmpty();
		
	if (bEmpty1 != bEmpty2)
		return (bEmpty1 ? 1 : -1);
	
	return Misc::NaturalCompare(sText1, sText2);
}

BOOL CWorkloadCtrl::GetListColumnRect(int nCol, CRect& rColumn, BOOL bScrolled) const
{
	if (ListView_GetSubItemRect(m_lcColumns, 0, nCol, LVIR_BOUNDS, &rColumn))
	{
		if (!bScrolled)
		{
			int nScroll = m_lcColumns.GetScrollPos(SB_HORZ);
			rColumn.OffsetRect(nScroll, 0);
		}

		return TRUE;
	}

	return FALSE;
}

bool CWorkloadCtrl::PrepareNewTask(ITaskList* pTaskList) const
{
	ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pTaskList, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return false;
	}

	// Set the start date to today and of duration 1 day
	HTASKITEM hNewTask = pTasks->GetFirstTask();
	ASSERT(hNewTask);

	COleDateTime dt = CDateHelper::GetDate(DHD_TODAY);
	time64_t tDate = 0;

	if (CDateHelper::GetTimeT64(dt, tDate))
	{
		pTasks->SetTaskStartDate64(hNewTask, tDate);
		pTasks->SetTaskDueDate64(hNewTask, tDate);
	}

	return true;
}

DWORD CWorkloadCtrl::HitTestTask(const CPoint& ptScreen) const
{
	// try list first
	DWORD dwTaskID = ListHitTestTask(ptScreen, TRUE);

	// then tree
	if (!dwTaskID)
		dwTaskID = TreeHitTestTask(ptScreen, TRUE);

	return dwTaskID;
}

DWORD CWorkloadCtrl::ListHitTestTask(const CPoint& ptScreen, BOOL bScreen) const
{
	int nUnused = -1;
	int nItem = ListHitTestItem(ptScreen, bScreen, nUnused);
	
	if (nItem != -1)
		return GetTaskID(nItem);

	return 0;
}

DWORD CWorkloadCtrl::TreeHitTestTask(const CPoint& ptScreen, BOOL bScreen) const
{
	HTREEITEM htiHit = TreeHitTestItem(ptScreen, bScreen);
	
	if (htiHit)
		return GetTaskID(htiHit);
	
	return 0;
}

HTREEITEM CWorkloadCtrl::TreeHitTestItem(const CPoint& point, BOOL bScreen) const
{
	CPoint ptTree(point);

	if (bScreen)
		m_tcTasks.ScreenToClient(&ptTree);
	
	return m_tcTasks.HitTest(ptTree);
}

WLC_COLUMNID CWorkloadCtrl::TreeHitTestItemColumn(const CPoint& point, BOOL bScreen) const
{
	if (TreeHitTestItem(point, bScreen) == NULL)
		return WLCC_NONE;

	CPoint ptHeader(point);

	if (bScreen)
		m_tcTasks.ScreenToClient(&ptHeader);

	ptHeader.y = 4;

	int nCol = m_hdrTasks.HitTest(ptHeader);

	if (nCol == -1)
		return WLCC_NONE;

	// else
	return (WLC_COLUMNID)m_hdrTasks.GetItemData(nCol);
}

HTREEITEM CWorkloadCtrl::GetItem(CPoint ptScreen) const
{
	return TreeHitTestItem(ptScreen, TRUE);
}

CString CWorkloadCtrl::GetItemTip(CPoint ptScreen) const
{
	HTREEITEM htiHit = GetItem(ptScreen);

	if (htiHit)
	{
		CRect rItem;

		if (m_tcTasks.GetItemRect(htiHit, rItem, TRUE))
		{
			int nColWidth = m_hdrTasks.GetItemWidth(0);

			rItem.left = max(rItem.left, 0);
			rItem.right = nColWidth;

			CPoint ptClient(ptScreen);
			m_tcTasks.ScreenToClient(&ptClient);

			if (rItem.PtInRect(ptClient))
			{
				DWORD dwTaskID = GetTaskID(htiHit);
				WORKLOADITEM* pWI = NULL;

				GET_WI_RET(dwTaskID, pWI, _T(""));

				int nTextLen = GraphicsMisc::GetTextWidth(pWI->sTitle, m_tcTasks);
				rItem.DeflateRect(TV_TIPPADDING, 0);

				if (nTextLen > rItem.Width())
					return pWI->sTitle;
			}
		}
	}

	// else
	return _T("");
}

BOOL CWorkloadCtrl::PointInHeader(const CPoint& ptScreen) const
{
	CRect rHeader;

	// try tree
	m_hdrTasks.GetWindowRect(rHeader);

	if (rHeader.PtInRect(ptScreen))
		return TRUE;

	// then list
	m_hdrColumns.GetWindowRect(rHeader);

	return rHeader.PtInRect(ptScreen);
}

void CWorkloadCtrl::GetWindowRect(CRect& rWindow, BOOL bWithHeader) const
{
	CRect rTree, rList;

	m_tcTasks.GetWindowRect(rTree);
	m_lcColumns.GetWindowRect(rList);

	if (bWithHeader)
		rWindow = rList; // height will include header
	else
		rWindow = rTree; // height will not include header

	rWindow.left  = min(rTree.left, rList.left);
	rWindow.right = max(rTree.right, rList.right);
}

int CWorkloadCtrl::ListHitTestItem(const CPoint& point, BOOL bScreen, int& nCol) const
{
	if (m_data.GetCount() == 0)
		return -1;

	// convert to list coords
	LVHITTESTINFO lvht = { 0 };
	lvht.pt = point;

	if (bScreen)
		m_lcColumns.ScreenToClient(&(lvht.pt));

	CRect rClient;
	CWnd::GetClientRect(rClient);

	if (!rClient.PtInRect(lvht.pt))
		return -1;

	if ((ListView_SubItemHitTest(m_lcColumns, &lvht) != -1) &&	(lvht.iSubItem > 0))
	{
		ASSERT(lvht.iItem != -1);

		nCol = lvht.iSubItem;
		return lvht.iItem;
	}

	// all else
	return -1;
}

DWORD CWorkloadCtrl::GetTaskID(HTREEITEM htiFrom) const
{
	if ((htiFrom == NULL) || (htiFrom == TVI_FIRST) || (htiFrom == TVI_ROOT))
		return 0;

	return GetTreeItemData(m_tcTasks, htiFrom);
}

DWORD CWorkloadCtrl::GetTaskID(int nItem) const
{
	return GetListTaskID(GetListItemData(m_lcColumns, nItem));
}

DWORD CWorkloadCtrl::GetListTaskID(DWORD dwItemData) const
{
	return GetTaskID((HTREEITEM)dwItemData);
}

void CWorkloadCtrl::RedrawList(BOOL bErase)
{
	m_lcColumns.InvalidateRect(NULL, bErase);
	m_lcColumns.UpdateWindow();
}

void CWorkloadCtrl::RedrawTree(BOOL bErase)
{
	m_tcTasks.InvalidateRect(NULL, bErase);
	m_tcTasks.UpdateWindow();
}

void CWorkloadCtrl::SetReadOnly(bool bReadOnly) 
{ 
	m_bReadOnly = bReadOnly;

	m_treeDragDrop.EnableDragDrop(!bReadOnly);
}

// external version
BOOL CWorkloadCtrl::CancelOperation()
{
	if (m_treeDragDrop.IsDragging())
	{
		m_treeDragDrop.CancelDrag();
		return TRUE;
	}
	
	// else 
	return FALSE;
}

int CWorkloadCtrl::GetTreeColumnOrder(CIntArray& aOrder) const
{
	return m_hdrTasks.GetItemOrder(aOrder);
}

void CWorkloadCtrl::SetTreeColumnVisibility(const CDWordArray& aColumnVis)
{
	int nNumCols = aColumnVis.GetSize();

	for (int nColID = 1; nColID < nNumCols; nColID++)
	{
		int nCol = m_hdrTasks.FindItem(nColID);
		m_hdrTasks.ShowItem(nCol, aColumnVis[nColID]);
	}

	Resize();
}

BOOL CWorkloadCtrl::SetTreeColumnOrder(const CIntArray& aOrder)
{
	if (!(aOrder.GetSize() && (aOrder[0] == 0)))
	{
		ASSERT(0);
		return FALSE;
	}

	return m_hdrTasks.SetItemOrder(aOrder);
}

void CWorkloadCtrl::GetTreeColumnWidths(CIntArray& aWidths) const
{
	m_hdrTasks.GetItemWidths(aWidths);
}

BOOL CWorkloadCtrl::SetTreeColumnWidths(const CIntArray& aWidths)
{
	if (aWidths.GetSize() != (NUM_TREECOLUMNS + 1))
		return FALSE;

	m_hdrTasks.SetItemWidths(aWidths);
	return TRUE;
}

void CWorkloadCtrl::GetTreeTrackedColumns(CIntArray& aTracked) const
{
	m_hdrTasks.GetTrackedItems(aTracked);
}

BOOL CWorkloadCtrl::SetTrackedTreeColumns(const CIntArray& aTracked)
{
	if (aTracked.GetSize() != (NUM_TREECOLUMNS + 1))
		return FALSE;
	
	m_hdrTasks.SetTrackedItems(aTracked); 
	return TRUE;
}

DWORD CWorkloadCtrl::GetNextTask(DWORD dwTaskID, IUI_APPCOMMAND nCmd) const
{
	HTREEITEM hti = FindTreeItem(m_tcTasks, dwTaskID);
	
	if (!hti)
	{
		ASSERT(0);
		return 0L;
	}

	DWORD dwNextID(dwTaskID);

	switch (nCmd)
	{
	case IUI_GETNEXTTASK:
		{
			HTREEITEM htiNext = TCH().GetNextVisibleItem(hti);
			
			if (htiNext)
				dwNextID = GetTreeItemData(m_tcTasks, htiNext);
		}
		break;
		
	case IUI_GETPREVTASK:
		{
			HTREEITEM htiPrev = TCH().GetPrevVisibleItem(hti);
			
			if (htiPrev)
				dwNextID = GetTreeItemData(m_tcTasks, htiPrev);
		}
		break;
		
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTOPLEVELTASK:
		{
			HTREEITEM htiNext = TCH().GetNextTopLevelItem(hti, (nCmd == IUI_GETNEXTTOPLEVELTASK));
			
			if (htiNext)
				dwNextID = GetTreeItemData(m_tcTasks, htiNext);
		}
		break;
		
	default:
		ASSERT(0);
	}
	
	return dwNextID;
}

BOOL CWorkloadCtrl::SaveToImage(CBitmap& bmImage)
{
	if (m_tcTasks.GetCount() == 0)
		return FALSE;

	CClientDC dc(&m_tcTasks);

	BOOL bTracked = m_hdrTasks.IsItemTracked(0);

	// Resize tree width to suit title text width
	int nPrevTitleWidth = m_hdrTasks.GetItemWidth(0);
	int nPrevTreeWidth = m_hdrTasks.CalcTotalItemWidth();

	int nReqTitleWidth = CalcTreeColumnWidth(0, &dc);
	int nReqTreeWidth = (nReqTitleWidth + (nPrevTreeWidth - nPrevTitleWidth));

	m_hdrTasks.SetItemWidth(0, nReqTitleWidth);
	Resize();

	CAutoFlag af(m_bSavingToImage, TRUE);
	CEnBitmap bmBase;

	BOOL bRes = CTreeListSyncer::SaveToImage(bmBase);

	if (bRes)
	{
		// Add totals and graph
		CEnBitmap bmLabels, bmTotals, bmChart;

		if (!CCopyListCtrlContents(m_lcTotalsLabels).DoCopy(bmLabels))
			return FALSE;

		// Manually resize the totals full width because it doesn't scroll
		int nReqWidth = m_hdrColumns.CalcTotalItemWidth();

		CRect rTotals = CDialogHelper::GetChildRect(&m_lcColumnTotals), rTemp(rTotals);
		rTemp.right = rTemp.left + nReqWidth;
		rTemp.bottom += 2; // else the bottom border is not always drawn

		m_lcColumnTotals.MoveWindow(rTemp);
		
		if (!CCopyListCtrlContents(m_lcColumnTotals).DoCopy(bmTotals))
			return FALSE;

		m_lcColumnTotals.MoveWindow(rTotals);

		if (!m_barChart.SaveToImage(bmChart))
			return FALSE;

		// Join them all the bits together
		CDC dcImage, dcParts;

		if (dcImage.CreateCompatibleDC(&dc) && dcParts.CreateCompatibleDC(&dc))
		{
			CSize sizeBase = bmBase.GetSize();
			CSize sizeLabels = bmLabels.GetSize();
			CSize sizeTotals = bmTotals.GetSize();
			CSize sizeChart = bmChart.GetSize();

			CSize sizeImage;

			sizeImage.cx = (sizeBase.cx + sizeChart.cx);
			sizeImage.cy = max((sizeBase.cy + sizeTotals.cy), sizeChart.cy);

			if (bmImage.CreateCompatibleBitmap(&dc, sizeImage.cx, sizeImage.cy))
			{
				CBitmap* pOldImage = dcImage.SelectObject(&bmImage);
				dcImage.FillSolidRect(0, 0, sizeImage.cx, sizeImage.cy, GetSysColor(COLOR_WINDOW));

				// Base-class
				CBitmap* pOldPart = dcParts.SelectObject(&bmBase);
				dcImage.BitBlt(0, 0, sizeBase.cx, sizeBase.cy, &dcParts, 0, 0, SRCCOPY);

				// Totals labels
				dcParts.SelectObject(bmLabels);
				dcImage.BitBlt(0, sizeBase.cy, sizeLabels.cx, sizeLabels.cy, &dcParts, 0, 0, SRCCOPY);

				// Column Totals
				dcParts.SelectObject(bmTotals);
				dcImage.BitBlt(sizeLabels.cx + 2, sizeBase.cy, sizeTotals.cx, sizeTotals.cy, &dcParts, 0, 0, SRCCOPY);

				// Draw vertical divider between labels and totals
				CRect rDivider(0, sizeBase.cy, (sizeLabels.cx + 1), (sizeBase.cy + sizeTotals.cy));
				DrawItemDivider(&dcImage, rDivider, DIV_VERT_LIGHT, FALSE);

				// Draw vertical divider at end of totals
				rDivider.right = sizeBase.cx;
				DrawItemDivider(&dcImage, rDivider, DIV_VERT_LIGHT, FALSE);

				// Bar chart
				dcParts.SelectObject(bmChart);
				dcImage.BitBlt(sizeBase.cx, 0, sizeChart.cx, sizeChart.cy, &dcParts, 0, 0, SRCCOPY);

				dcParts.SelectObject(pOldPart);
				dcImage.SelectObject(pOldImage);
			}
		}
	}
	
	// Restore title column width
	m_hdrTasks.SetItemWidth(0, nPrevTitleWidth);
	m_hdrTasks.SetItemTracked(0, bTracked);

	Resize();
	
	return bRes;
}

void CWorkloadCtrl::RefreshItemBoldState(HTREEITEM htiFrom, BOOL bAndChildren)
{
	if (htiFrom && (htiFrom != TVI_ROOT))
	{
		TCH().SetItemBold(htiFrom, (m_tcTasks.GetParentItem(htiFrom) == NULL));
	}
	
	// children
	if (bAndChildren)
	{
		HTREEITEM htiChild = m_tcTasks.GetChildItem(htiFrom);
		
		while (htiChild)
		{
			RefreshItemBoldState(htiChild);
			htiChild = m_tcTasks.GetNextItem(htiChild, TVGN_NEXT);
		}
	}
}

BOOL CWorkloadCtrl::SetFont(HFONT hFont, BOOL bRedraw)
{
	if (!hFont || !m_tcTasks.GetSafeHwnd() || !m_lcColumns.GetSafeHwnd())
	{
		ASSERT(0);
		return FALSE;
	}

	CFont* pFont = CFont::FromHandle(hFont);
	
	m_tcTasks.SetFont(pFont, bRedraw);
	m_lcTotalsLabels.SetFont(pFont, bRedraw);
	m_lcColumnTotals.SetFont(pFont, bRedraw);
	m_hdrColumns.SetFont(pFont, bRedraw);

	ResizeColumnsToFit();

	CString sFontName;
	int nFontSize = GraphicsMisc::GetFontNameAndPointSize(hFont, sFontName);

	m_barChart.SetFont(sFontName, nFontSize);

	return TRUE;
}

void CWorkloadCtrl::FilterToolTipMessage(MSG* pMsg)
{
	m_tcTasks.FilterToolTipMessage(pMsg);
	m_hdrTasks.FilterToolTipMessage(pMsg);
	m_hdrColumns.FilterToolTipMessage(pMsg);
	m_barChart.FilterToolTipMessage(pMsg);
}

bool CWorkloadCtrl::ProcessMessage(MSG* pMsg) 
{
	return (m_treeDragDrop.ProcessMessage(pMsg) != FALSE);
}

BOOL CWorkloadCtrl::CanMoveSelectedItem(const IUITASKMOVE& move) const
{
	return (GetSelectedTaskID() && 
			((move.dwParentID == 0) || m_data.HasItem(move.dwParentID)) &&
			((move.dwAfterSiblingID == 0) || m_data.HasItem(move.dwAfterSiblingID)));
}

BOOL CWorkloadCtrl::MoveSelectedItem(const IUITASKMOVE& move)
{
	if (!CanMoveSelectedItem(move))
		return FALSE;

	CAutoFlag af(m_bMovingTask, TRUE);
	
	HTREEITEM htiSel = GetSelectedItem(), htiNew = NULL;
	HTREEITEM htiDestParent = GetTreeItem(move.dwParentID);
	HTREEITEM htiDestAfterSibling = GetTreeItem(move.dwAfterSiblingID);

	CLockUpdates lu(*this);
	CHoldRedraw hr(*this);

	htiNew = TCH().MoveTree(htiSel, htiDestParent, htiDestAfterSibling, TRUE, TRUE);

	if (htiNew)
	{
		RefreshTreeItemMap();
		SelectItem(htiNew);

		return TRUE;
	}

	ASSERT(0);
	return FALSE;
}
