// BurndownWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BurndownExt.h"
#include "BurndownWnd.h"
#include "BurndownStatic.h"

#include "..\shared\mapex.h"
#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\themed.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\datehelper.h"
#include "..\shared\enstring.h"
#include "..\shared\WorkingWeek.h"
#include "..\shared\AutoFlag.h"

#include "..\3rdparty\dibdata.h"
#include "..\3rdparty\GdiPlus.h"

#include "..\Interfaces\ipreferences.h"

#include <float.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const COLORREF COLOR_GREEN	= RGB(122, 204,   0); 
const COLORREF COLOR_RED	= RGB(204,   0,   0); 
const COLORREF COLOR_YELLOW = RGB(204, 164,   0); 
const COLORREF COLOR_BLUE	= RGB(0,     0, 244); 
const COLORREF COLOR_PINK	= RGB(234,  28,  74); 
const COLORREF COLOR_ORANGE	= RGB(255,  91,  21); 

/////////////////////////////////////////////////////////////////////////////

const UINT WM_REBUILDGRAPH = (WM_APP+1);

/////////////////////////////////////////////////////////////////////////////

enum // m_dwUpdateGraphOnShow
{
	REBUILD_GRAPH	= 0x01,
	RESORT_DATA		= 0x02,
	UPDATE_EXTENTS	= 0x04
};

/////////////////////////////////////////////////////////////////////////////
// CBurndownWnd dialog

CBurndownWnd::CBurndownWnd(CWnd* pParent /*=NULL*/)
	: 
	CDialog(IDD_STATISTICS_DLG, pParent),
	m_nGraphType(BCT_REMAININGDAYS),
	m_dwUpdateGraphOnShow(0),
	m_nTrendLine(BTL_NONE),
	m_dtDataRange(DHD_BEGINTHISMONTH, DHD_ENDTHISMONTH),
	m_graph(m_data),
	m_bUpdatingSlider(FALSE),
	m_sliderDateRange(TBS_BOTTOM)
{
	//{{AFX_DATA_INIT(CBurndownWnd)
	//}}AFX_DATA_INIT
	m_icon.LoadIcon(IDR_STATISTICS);
	m_sliderDateRange.SetMinMaxRangeWidths(1.0); // min range = 1 month
}

CBurndownWnd::~CBurndownWnd()
{

}

void CBurndownWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBurndownWnd)
	DDX_Control(pDX, IDC_FRAME, m_stFrame);
	DDX_Control(pDX, IDC_DISPLAY, m_cbGraphs);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TRENDLINES, m_cbTrends);
	DDX_Control(pDX, IDC_ACTIVEDATERANGE, m_sliderDateRange);

	CDialogHelper::DDX_CBData(pDX, m_cbGraphs, m_nGraphType, BCT_REMAININGDAYS);
	CDialogHelper::DDX_CBData(pDX, m_cbTrends, m_nTrendLine, BTL_NONE);

}


BEGIN_MESSAGE_MAP(CBurndownWnd, CDialog)
	//{{AFX_MSG_MAP(CBurndownWnd)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, OnHelp)
	ON_WM_HELPINFO()
	ON_CBN_SELCHANGE(IDC_DISPLAY, OnSelchangeDisplay)
	ON_CBN_SELCHANGE(IDC_TRENDLINES, OnTrendsChanged)
	ON_WM_SHOWWINDOW()
	ON_WM_ERASEBKGND()
	ON_WM_NCDESTROY()

	ON_MESSAGE(WM_REBUILDGRAPH, OnRebuildGraph)
	ON_REGISTERED_MESSAGE(RANGE_CHANGED, OnActiveDateRangeChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBurndownWnd message handlers

void CBurndownWnd::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	
	delete this;
}

void CBurndownWnd::OnHelp()
{
	GetParent()->SendMessage(WM_IUI_DOHELP, 0, (LPARAM)GetTypeID());
}

BOOL CBurndownWnd::OnHelpInfo(HELPINFO* /*lpHelpInfo*/)
{
	OnHelp();
	return TRUE;
}

BOOL CBurndownWnd::Create(DWORD dwStyle, const RECT &/*rect*/, CWnd* pParentWnd, UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	UINT nSize = sizeof(CPlex);

	if (CDialog::Create(IDD_STATISTICS_DLG, pParentWnd))
	{
		SetWindowLong(*this, GWL_STYLE, dwStyle);
		SetDlgCtrlID(nID);

		return TRUE;
	}

	return FALSE;
}

BOOL CBurndownWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();

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

	// Initialise graph
	CRect rFrame;
	m_stFrame.GetWindowRect(rFrame);
	ScreenToClient(rFrame);
	rFrame.DeflateRect(1, 1);

	VERIFY(m_graph.SubclassDlgItem(IDC_GRAPH, this));

	BuildCombos();
	RebuildGraph(FALSE, FALSE, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBurndownWnd::BuildCombos()
{
	ASSERT(m_cbGraphs.GetCount() == 0);

	for (int nGraph = 0; nGraph < BCT_NUMGRAPHS; nGraph++)
		CDialogHelper::AddString(m_cbGraphs, m_graph.GetGraphTitle((BURNDOWN_GRAPHTYPE)nGraph), nGraph);

	ASSERT(m_cbTrends.GetCount() == 0);

	for (int nTrend = 0; nTrend < NUM_TRENDS; nTrend++)
		CDialogHelper::AddString(m_cbTrends, TRENDS[nTrend].nTitleStrID, TRENDS[nTrend].nTrend);
}

void CBurndownWnd::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	//CString sKey(szKey);

	pPrefs->WriteProfileInt(szKey, _T("GraphType"), m_nGraphType);
	pPrefs->WriteProfileInt(szKey, _T("TrendLine"), m_nTrendLine);

	// Active date range
	pPrefs->DeleteProfileSection(_T("ActiveRange"));

	COleDateTimeRange dtActiveRange;

	if (m_sliderDateRange.HasSelectedRange())
	{
		VERIFY(GetSliderDateRange(dtActiveRange));

		pPrefs->WriteProfileDouble(_T("ActiveRange"), _T("Start"), dtActiveRange.GetStart().m_dt);
		pPrefs->WriteProfileDouble(_T("ActiveRange"), _T("End"), dtActiveRange.GetEnd().m_dt);
	}
	else
	{
		pPrefs->DeleteProfileSection(_T("ActiveRange"));
	}
}

void CBurndownWnd::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey, bool bAppOnly) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// burn down specific options
	if (!bAppOnly)
	{
		//CString sKey(szKey);
		m_nGraphType = (BURNDOWN_GRAPHTYPE)pPrefs->GetProfileInt(szKey, _T("GraphType"), BCT_INCOMPLETETASKS);
		m_nTrendLine = (BURNDOWN_TRENDTYPE)pPrefs->GetProfileInt(szKey, _T("TrendLine"), BTL_BEST_FIT);

		if (m_nGraphType >= BCT_NUMGRAPHS)
			m_nGraphType = BCT_INCOMPLETETASKS;

		if (m_nTrendLine >= NUM_TRENDS)
			m_nTrendLine = BTL_NONE;

		m_graph.SetActiveGraph(m_nGraphType);
		m_graph.ShowTrendLine(m_nTrendLine);

		// Active range
		m_dtPrevActiveRange.Reset();

		double dStart = pPrefs->GetProfileDouble(_T("ActiveRange"), _T("Start"), 0);
		double dEnd = pPrefs->GetProfileDouble(_T("ActiveRange"), _T("End"), -1);

		if (dEnd > dStart)
			VERIFY(m_dtPrevActiveRange.Set(dStart, dEnd));

		// TODO
		m_graph.SetTodayColour(255);

		UpdateData(FALSE);
	}

	// application preferences
	DWORD dwWeekends = pPrefs->GetProfileInt(_T("Preferences"), _T("Weekends"), (DHW_SATURDAY | DHW_SUNDAY));
	double dHoursInDay = _ttof(pPrefs->GetProfileString(_T("Preferences"), _T("HoursInDay"), _T("8")));

	CWorkingWeek::Initialise(dwWeekends, dHoursInDay);
}

void CBurndownWnd::SetUITheme(const UITHEME* pTheme)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	GraphicsMisc::VerifyDeleteObject(m_brBack);
	
	if (CThemed::IsAppThemed() && pTheme)
	{
		m_theme = *pTheme;
		m_brBack.CreateSolidBrush(pTheme->crAppBackLight);
		
		// intentionally set background colours to be same as ours
		m_toolbar.SetBackgroundColors(m_theme.crAppBackLight, m_theme.crAppBackLight, FALSE, FALSE);
		m_toolbar.SetHotColor(m_theme.crToolbarHot);

		m_sliderDateRange.SetParentBackgroundColor(m_theme.crAppBackLight);
	}
}

void CBurndownWnd::SetTaskFont(HFONT hFont)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString sFontName;
	int nFontSize = GraphicsMisc::GetFontNameAndPointSize(hFont, sFontName);

	m_graph.SetFont(sFontName, nFontSize);
}

bool CBurndownWnd::ProcessMessage(MSG* /*pMsg*/) 
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
//	if (!IsWindowEnabled())
//		return false;

	// process editing shortcuts
	// TODO

	return false;
}

void CBurndownWnd::FilterToolTipMessage(MSG* pMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_graph.FilterToolTipMessage(pMsg);
}

bool CBurndownWnd::GetLabelEditRect(LPRECT /*pEdit*/)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return false;
}

bool CBurndownWnd::PrepareNewTask(ITaskList* /*pTask*/) const 
{ 
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return false; 
}

IUI_HITTEST CBurndownWnd::HitTest(const POINT& /*ptScreen*/) const
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return IUI_NOWHERE;
}

bool CBurndownWnd::SelectTask(DWORD /*dwTaskID*/)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// because we can't change the selection
	// in this plugin we don't care what it's set to
	return false;
}

bool CBurndownWnd::SelectTasks(const DWORD* /*pdwTaskIDs*/, int /*nTaskCount*/)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// because we can't change the selection
	// in this plugin we don't care what it's set to
	return false;
}

bool CBurndownWnd::WantTaskUpdate(TDC_ATTRIBUTE nAttribute) const
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	switch (nAttribute)
	{
	case TDCA_DONEDATE:
	case TDCA_STARTDATE:
	case TDCA_CREATIONDATE:
	case TDCA_TIMEEST:
	case TDCA_TIMESPENT:
	case TDCA_COST:
		return true;
	}

	// all else 
	return false;
}

void CBurndownWnd::BuildData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, BOOL bAndSiblings, BOOL bCheckExist)
{
	if (hTask == NULL)
		return;

	// Not interested in references
	if (!pTasks->IsTaskReference(hTask))
	{
		// only interested in leaf tasks
		if (!pTasks->IsTaskParent(hTask))
		{
			STATSITEM* pSI = m_data.AddItem(pTasks->GetTaskID(hTask));

			if (pSI) // means it's new
				pSI->Set(pTasks, hTask);
		}
		else // Process children
		{
			BuildData(pTasks, pTasks->GetFirstTask(hTask), TRUE, bCheckExist);
		}
	}

	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			BuildData(pTasks, hSibling, FALSE, bCheckExist);
			
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
}

void CBurndownWnd::UpdateTasks(const ITaskList* pTaskList, IUI_UPDATETYPE nUpdate)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pTaskList, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return;
	}

	// we must have been initialized already
	ASSERT(m_graph.GetSafeHwnd());

	switch (nUpdate)
	{
	case IUI_ALL:
		m_data.RemoveAll();

		BuildData(pTasks, pTasks->GetFirstTask(), TRUE, FALSE);
		RebuildGraph(TRUE, TRUE, TRUE);
		break;
		
	case IUI_NEW:
		BuildData(pTasks, pTasks->GetFirstTask(), TRUE, TRUE);
		RebuildGraph(TRUE, TRUE, TRUE);
		break;
		
	case IUI_EDIT:
		UpdateTask(pTasks, pTasks->GetFirstTask(), nUpdate, TRUE);
		RebuildGraph(TRUE, TRUE, TRUE);
		break;
		
	case IUI_DELETE:
		if (RemoveDeletedTasks(pTasks))
			RebuildGraph(FALSE, TRUE, TRUE);
		break;
		
	default:
		ASSERT(0);
	}
}

void CBurndownWnd::UpdateTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, IUI_UPDATETYPE nUpdate, BOOL bAndSiblings)
{
	// handle task if not NULL (== root)
	if (hTask == NULL)
		return;

	// Not interested in references
	if (pTasks->IsTaskReference(hTask))
		return;
	
	ASSERT(nUpdate == IUI_EDIT);

	// only interested in leaf tasks
	if (!pTasks->IsTaskParent(hTask))
	{
		DWORD dwTaskID = pTasks->GetTaskID(hTask);

		// if the task we've been given is actually a parent task
		// then we'll have no record of it, so the following
		// lookup has a legitimate reason to fail
		STATSITEM* pSI = m_data.GetItem(dwTaskID);
		ASSERT(pSI);
		
		if (pSI)
		{
			pSI->Update(pTasks, hTask);
		}
		else
		{
			int breakpoint = 0;
		}
	}
	
	// children
	UpdateTask(pTasks, pTasks->GetFirstTask(hTask), nUpdate, TRUE);
	
	// handle siblings WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == not siblings
			UpdateTask(pTasks, hSibling, nUpdate, FALSE);
			
			hSibling = pTasks->GetNextTask(hSibling);
		}
	}
}

BOOL CBurndownWnd::RemoveDeletedTasks(const ITASKLISTBASE* pTasks)
{
	// iterating sorted data is quickest
	int nOrgCount = m_data.GetSize();
	int nItem = nOrgCount;

	while (nItem--)
	{
		const STATSITEM* pSI = m_data[nItem];

		if (!pTasks->FindTask(pSI->dwTaskID))
			m_data.RemoveAt(nItem);
	}

	return (m_data.GetSize() != nOrgCount);
}

bool CBurndownWnd::DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData) 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	switch (nCmd)
	{
	case IUI_SAVETOIMAGE:
		if (pData && !m_data.IsEmpty())
		{
			CBitmap bmImage;

			if (m_graph.SaveToImage(bmImage))
			{
				CString sPngPath(pData->szFilePath);
				FileMisc::ReplaceExtension(sPngPath, _T(".png"));
				
				if (CGdiPlusBitmap(bmImage).SaveAsPNG(sPngPath))
				{
					lstrcpyn(pData->szFilePath, sPngPath, MAX_PATH);
					return true;
				}
				
				// Fallback
				CDibData dib;

				if (dib.CreateDIB(bmImage) && dib.SaveDIB(pData->szFilePath))
					return true;
			}
		}
		break;

	case IUI_EXPANDALL:
	case IUI_COLLAPSEALL:
	case IUI_EXPANDSELECTED:
	case IUI_COLLAPSESELECTED:
	case IUI_SORT:
	case IUI_SETFOCUS:
	case IUI_RESIZEATTRIBCOLUMNS:
	case IUI_GETNEXTTASK:
	case IUI_GETNEXTVISIBLETASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVTASK:
	case IUI_GETPREVVISIBLETASK:
	case IUI_GETPREVTOPLEVELTASK:
		// not handled
		break;
	}

	return false;
}

bool CBurndownWnd::CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* /*pData*/) const 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	switch (nCmd)
	{
	case IUI_SAVETOIMAGE:
		return !m_data.IsEmpty();

	case IUI_EXPANDALL:
	case IUI_COLLAPSEALL:
	case IUI_EXPANDSELECTED:
	case IUI_COLLAPSESELECTED:
	case IUI_SORT:
	case IUI_SETFOCUS:
	case IUI_RESIZEATTRIBCOLUMNS:
	case IUI_GETNEXTVISIBLETASK:
	case IUI_GETNEXTTOPLEVELTASK:
	case IUI_GETPREVVISIBLETASK:
	case IUI_GETPREVTOPLEVELTASK:
	case IUI_SELECTTASK:
		// not handled
		break;
	}

	return false;
}

BOOL CBurndownWnd::OnEraseBkgnd(CDC* pDC) 
{
	CDialogHelper::ExcludeChild(&m_graph, pDC);

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

HBRUSH CBurndownWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CBurndownWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (m_stFrame.GetSafeHwnd())
	{
		CRect rFrame;
		m_stFrame.GetWindowRect(rFrame);
		
		ScreenToClient(rFrame);
		rFrame.right = cx;
		rFrame.bottom = cy;

		m_stFrame.MoveWindow(rFrame);

		rFrame.DeflateRect(1, 1);
		m_graph.MoveWindow(rFrame);

		// selected task dates takes available space
		CRect rSlider = CDialogHelper::GetChildRect(&m_sliderDateRange);

		CDialogHelper::ResizeChild(&m_sliderDateRange, (cx - 10 - rSlider.right), 0);
		CDialogHelper::ResizeCtrl(this, IDC_ACTIVEDATERANGE_LABEL, (cx - 10 - rSlider.right), 0);

		CAutoFlag af(m_bUpdatingSlider, TRUE);
		UpdateRangeSliderStep();
	}
}

void CBurndownWnd::RebuildGraph(BOOL bSortData, BOOL bUpdateExtents, BOOL bCheckVisibility)
{
	if (bCheckVisibility && !m_graph.IsWindowVisible())
	{
		m_dwUpdateGraphOnShow = REBUILD_GRAPH;
		m_dwUpdateGraphOnShow |= (bSortData ? RESORT_DATA : 0);
		m_dwUpdateGraphOnShow |= (bUpdateExtents ? UPDATE_EXTENTS : 0);
		
		return;
	}

	if (bSortData)
		m_data.Sort();
	
	if (bUpdateExtents)
		m_data.GetDataExtents(m_dtDataRange);

	// Only restore previous range if it's wholly contained within the data
	COleDateTimeRange dtActiveRange(m_dtDataRange);

	if (m_dtPrevActiveRange.IsValid() && m_dtDataRange.IsValid())
	{
		COleDateTime dtDataStart = CDateHelper::GetStartOfMonth(m_dtDataRange.GetStart());
		COleDateTime dtDataEnd = CDateHelper::GetEndOfMonth(m_dtDataRange.GetEnd());
		
		COleDateTimeRange dtDataRange;

		if (dtDataRange.Set(dtDataStart, dtDataEnd) && dtDataRange.Contains(m_dtPrevActiveRange))
			dtActiveRange = m_dtPrevActiveRange;

		m_dtPrevActiveRange.Reset(); // always
	}

	m_graph.RebuildGraph(dtActiveRange);

	UpdateRangeSlider(dtActiveRange);
	UpdateActiveRangeLabel(dtActiveRange);
}

void CBurndownWnd::OnSelchangeDisplay()
{
	UpdateData();

	m_graph.SetActiveGraph(m_nGraphType);
}

void CBurndownWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow && m_dwUpdateGraphOnShow)
	{
		ASSERT(m_dwUpdateGraphOnShow & REBUILD_GRAPH);
		SendMessage(WM_REBUILDGRAPH);
	}
}

LRESULT CBurndownWnd::OnRebuildGraph(WPARAM /*wp*/, LPARAM /*lp*/)
{
	ASSERT(m_dwUpdateGraphOnShow & REBUILD_GRAPH);

	BOOL bSortData = (m_dwUpdateGraphOnShow & RESORT_DATA);
	BOOL bUpdateExtents = (m_dwUpdateGraphOnShow & UPDATE_EXTENTS);

	m_dwUpdateGraphOnShow = 0;

	RebuildGraph(bSortData, bUpdateExtents, FALSE);

	return 0L;
}

BOOL CBurndownWnd::GetSliderDateRange(COleDateTimeRange& dtActiveRange) const
{
	COleDateTime dtActiveStart(CDateHelper::GetDateFromMonths((int)m_sliderDateRange.GetLeft()));
	COleDateTime dtActiveEnd(CDateHelper::GetDateFromMonths((int)m_sliderDateRange.GetRight() - 1));

	return dtActiveRange.Set(dtActiveStart, dtActiveEnd);
}

LRESULT CBurndownWnd::OnActiveDateRangeChange(WPARAM /*wp*/, LPARAM /*lp*/)
{
	if (!m_bUpdatingSlider)
	{
		COleDateTimeRange dtActiveRange;
		
		if (GetSliderDateRange(dtActiveRange))
		{
			UpdateActiveRangeLabel(dtActiveRange);
			m_graph.RebuildGraph(dtActiveRange);
		}
	}

	return 0L;
}

void CBurndownWnd::UpdateRangeSliderStep()
{
	// Set the tick spacing to match the current scale
	int nStep = 1;

// 	switch (m_graph.GetCurrentScale())
// 	{
// 	case BCS_DAY:
// 	case BCS_WEEK:
// 	case BCS_MONTH:		break;
// 
// 	case BCS_2MONTH:	nStep = 2;	break;
// 	case BCS_QUARTER:	nStep = 3;	break;
// 	case BCS_HALFYEAR:	nStep = 6;	break;
// 	case BCS_YEAR:		nStep = 12;	break;
// 
// 	default:			ASSERT(0);	break;
// 	}

	m_sliderDateRange.SetStep(nStep);
}

void CBurndownWnd::UpdateRangeSlider(const COleDateTimeRange& dtActiveRange)
{
	CAutoFlag af(m_bUpdatingSlider, TRUE);

	if (m_dtDataRange.IsValid())
	{
		int nDataStart = CDateHelper::GetDateInMonths(m_dtDataRange.GetStart());
		int nDataEnd = CDateHelper::GetDateInMonths(m_dtDataRange.GetEnd()) + 1;
		nDataEnd = max(nDataEnd, nDataStart + 1);

		m_sliderDateRange.SetMinMax(nDataStart, nDataEnd);
		
		int nActiveStart = CDateHelper::GetDateInMonths(dtActiveRange.GetStart());
		int nActiveEnd = CDateHelper::GetDateInMonths(dtActiveRange.GetEnd()) + 1;
		nActiveEnd = max(nActiveEnd, nActiveStart + 1);

		nActiveStart = max(nActiveStart, nDataStart);
		nActiveEnd = min(nActiveEnd, nDataEnd);

		m_sliderDateRange.SetRange(nActiveStart, nActiveEnd);
		m_sliderDateRange.EnableWindow(TRUE);
	}
	else
	{
		m_sliderDateRange.SetMinMax(0, 1.0);
		m_sliderDateRange.SetRange(0.0, 1.0);
		m_sliderDateRange.EnableWindow(FALSE);
	}

	UpdateRangeSliderStep();
}

void CBurndownWnd::UpdateActiveRangeLabel(const COleDateTimeRange& dtActiveRange)
{
	CString sRange;

	if (dtActiveRange.IsValid())
	{
		COleDateTime dtStart = dtActiveRange.GetStart();

		if (m_sliderDateRange.GetRangeWidth() == 1.0)
		{
			sRange.Format(_T("%s %d"), CDateHelper::GetMonthName(dtStart.GetMonth(), TRUE), dtStart.GetYear());
		}
		else
		{
			COleDateTime dtEnd = dtActiveRange.GetEnd();
			CString sStart, sEnd;

			sStart.Format(_T("%s %d"), CDateHelper::GetMonthName(dtStart.GetMonth(), TRUE), dtStart.GetYear());
			sEnd.Format(_T("%s %d"), CDateHelper::GetMonthName(dtEnd.GetMonth(), TRUE), dtEnd.GetYear());

			sRange.Format(_T("%s - %s"), sStart, sEnd);
		}
	}
	
	GetDlgItem(IDC_ACTIVEDATERANGE_LABEL)->SetWindowText(CEnString(IDS_ACTIVEDATERANGE, sRange));
}

void CBurndownWnd::OnTrendsChanged()
{
	BURNDOWN_TRENDTYPE nPrevTrend = m_nTrendLine;

	UpdateData();

	if (m_nTrendLine != nPrevTrend)
		m_graph.ShowTrendLine(m_nTrendLine);
}
