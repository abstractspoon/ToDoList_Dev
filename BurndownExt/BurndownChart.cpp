// BurndownChart.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BurndownChart.h"
#include "BurndownStatic.h"

#include "..\shared\datehelper.h"
#include "..\shared\holdredraw.h"
#include "..\shared\enstring.h"
#include "..\shared\graphicsmisc.h"

////////////////////////////////////////////////////////////////////////////////

 
const COLORREF COLOR_GREEN		= RGB(122, 204,   0); 
const COLORREF COLOR_GREENLINE	= GraphicsMisc::Darker(COLOR_GREEN, 0.05, FALSE);
const COLORREF COLOR_GREENFILL	= GraphicsMisc::Lighter(COLOR_GREEN, 0.25, FALSE);

const COLORREF COLOR_RED		= RGB(204,   0,   0); 
const COLORREF COLOR_REDLINE	= GraphicsMisc::Darker(COLOR_RED, 0.05, FALSE);
const COLORREF COLOR_REDFILL	= GraphicsMisc::Lighter(COLOR_RED, 0.25, FALSE);

const COLORREF COLOR_YELLOW		= RGB(204, 164,   0); 
const COLORREF COLOR_YELLOWLINE	= GraphicsMisc::Darker(COLOR_YELLOW, 0.05, FALSE);
const COLORREF COLOR_YELLOWFILL	= GraphicsMisc::Lighter(COLOR_YELLOW, 0.25, FALSE);

const COLORREF COLOR_BLUE		= RGB(0,     0, 244); 
const COLORREF COLOR_BLUELINE	= GraphicsMisc::Darker(COLOR_BLUE, 0.05, FALSE);
const COLORREF COLOR_BLUEFILL	= GraphicsMisc::Lighter(COLOR_BLUE, 0.25, FALSE);

const COLORREF COLOR_PINK		= RGB(234,  28,  74); 
const COLORREF COLOR_PINKLINE	= GraphicsMisc::Darker(COLOR_PINK, 0.05, FALSE);
const COLORREF COLOR_PINKFILL	= GraphicsMisc::Lighter(COLOR_PINK, 0.25, FALSE);

const COLORREF COLOR_ORANGE		= RGB(255,  91,  21);
const COLORREF COLOR_ORANGELINE	= GraphicsMisc::Darker(COLOR_ORANGE, 0.05, FALSE);
const COLORREF COLOR_ORANGEFILL	= GraphicsMisc::Lighter(COLOR_ORANGE, 0.25, FALSE);

const int    DEF_DAYSINWEEK			= 5;
const double DEF_HOURSINDAY			= 8.0;

const int    LINE_THICKNESS			= 1;
const double MIN_SUBINTERVAL_HEIGHT	= GraphicsMisc::ScaleByDPIFactor(10);


/////////////////////////////////////////////////////////////////////////////

const int MIN_XSCALE_SPACING = 50; // pixels

static int SCALES[] = 
{
	BCS_DAY,		
	BCS_WEEK,	
	BCS_MONTH,	
	BCS_2MONTH,	
	BCS_QUARTER,	
	BCS_HALFYEAR,
	BCS_YEAR,	
};
static int NUM_SCALES = sizeof(SCALES) / sizeof(int);

////////////////////////////////////////////////////////////////////////////////
// CBurndownChart

CBurndownChart::CBurndownChart(const CStatsItemArray& data) 
	: 
	m_data(data),
	m_nScale(1),
	m_dHoursInDay(DEF_HOURSINDAY),
	m_nDaysInWeek(DEF_DAYSINWEEK),
	m_nChartType(BCT_INCOMPLETETASKS)
{
}

CBurndownChart::~CBurndownChart()
{
}

////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CBurndownChart, CHMXChart)
	ON_WM_SIZE()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// CBurndownChart message handlers

BOOL CBurndownChart::SetChartType(BURNDOWN_CHARTTYPE nType)
{
	if (nType != m_nChartType)
	{
		m_nChartType = nType;
		RebuildGraph(FALSE);

		return TRUE;
	}

	return FALSE;
}

BOOL CBurndownChart::SetTimeIntervals(int nDaysInWeek, double dHoursInDay)
{
	if ((nDaysInWeek != m_nDaysInWeek) || (dHoursInDay != m_dHoursInDay))
	{
		m_dHoursInDay = dHoursInDay;
		m_nDaysInWeek = nDaysInWeek;

		return TRUE;
	}

	return FALSE;
}

BOOL CBurndownChart::SaveToImage(CBitmap& bmImage)
{
	CClientDC dc(this);
	CDC dcImage;

	if (dcImage.CreateCompatibleDC(&dc))
	{
		CRect rClient;
		GetClientRect(rClient);

		if (bmImage.CreateCompatibleBitmap(&dc, rClient.Width(), rClient.Height()))
		{
			CBitmap* pOldImage = dcImage.SelectObject(&bmImage);

			SendMessage(WM_PRINTCLIENT, (WPARAM)dcImage.GetSafeHdc(), PRF_ERASEBKGND);
			Invalidate(TRUE);

			dcImage.SelectObject(pOldImage);
		}
	}

	return (bmImage.GetSafeHandle() != NULL);
}

void CBurndownChart::BuildBurndownGraph()
{
	ClearData();
		
	SetDatasetStyle(0, HMX_DATASET_STYLE_AREALINE);
	SetDatasetLineColor(0, COLOR_GREENLINE);
	SetDatasetFillColor(0, COLOR_GREENFILL);
	SetDatasetSizeFactor(0, LINE_THICKNESS);
	SetDatasetMin(0, 0.0);
	SetDrawDatasetOverGrid(0, false);

	// build the graph
	COleDateTime dtStart = GetGraphStartDate();
	COleDateTime dtEnd = GetGraphEndDate();
	
	int nNumDays = ((int)dtEnd.m_dt - (int)dtStart.m_dt);
	int nItemFrom = 0;
	
	for (int nDay = 0; nDay <= nNumDays; nDay++)
	{
		COleDateTime date(dtStart.m_dt + nDay);
		
		if (m_dtExtents.GetStart() > date)
		{
			AddData(0, 0);
		}
		else
		{
			int nNumNotDone = m_data.CalculateIncompleteTaskCount(date, nItemFrom, nItemFrom);
			AddData(0, nNumNotDone);
		}
	}

	// Set the maximum Y value to be something 'nice'
	double dMin, dMax;

	if (GetMinMax(dMin, dMax, true))
	{
		ASSERT(dMin == 0.0);

		dMax = CalcMaxYAxisValue(dMax);
		SetDatasetMax(0, dMax);
	}
	
	CalcDatas();
}

double CBurndownChart::CalcMaxYAxisValue(double dDataMax) const
{
	return (GetYTicks() * CalcYAxisInterval(dDataMax));
}

double CBurndownChart::CalcYAxisInterval(double dDataMax) const
{
	const double INTERVALS[] = { 1, 5, 10, 20, 25, 50, 100, 200, 250, 500, 1000, 2000, 5000 };
	const int NUM_INT = (sizeof(INTERVALS) / sizeof(INTERVALS[0]));

	int nNumTicks = GetYTicks();

	// Find the first tick increment that gives us a range
	// greater than or equal to dDataMax
	for (int nInc = 0; nInc < NUM_INT; nInc++)
	{
		double dMaxYAxis = (nNumTicks * INTERVALS[nInc]);

		if (dDataMax <= dMaxYAxis)
			return INTERVALS[nInc];
	}

	return 10000;
}

int CBurndownChart::GetYSubTicks(double dInterval) const
{
	if (dInterval != (int)dInterval)
	{
		ASSERT(0);
		return 1;
	}
	else if (dInterval == 1.0)
	{
		return 1;
	}
	
	const int SUB_TICKS[] = { 2, 4, 5 };
	const int NUM_SUBTICKS = (sizeof(SUB_TICKS) / sizeof(SUB_TICKS[0]));

	int nSubTick = NUM_SUBTICKS;
	int nNumTicks = GetYTicks();

	while (nSubTick--)
	{
		double dSubInterval = (dInterval / SUB_TICKS[nSubTick]);

		if (dSubInterval == (int)dSubInterval)
		{
			double dSubIntervalInPixels = (m_rectData.Height() / (nNumTicks * SUB_TICKS[nSubTick]));

			if (dSubIntervalInPixels >= MIN_SUBINTERVAL_HEIGHT)
				return SUB_TICKS[nSubTick];
		}
	}

	// else
	return 1;
}

void CBurndownChart::BuildSprintGraph()
{
	ClearData();

	enum 
	{ 
		SPRINT_EST,
		SPRINT_SPENT
	};
	
	SetDatasetStyle(SPRINT_EST, HMX_DATASET_STYLE_LINE);
	SetDatasetMarker(SPRINT_EST, HMX_DATASET_MARKER_NONE);
	SetDatasetLineColor(SPRINT_EST,  COLOR_REDLINE);
	SetDatasetSizeFactor(SPRINT_EST, LINE_THICKNESS);
	SetDatasetMin(SPRINT_EST, 0.0);
	SetDrawDatasetOverGrid(SPRINT_EST, true);
	
	SetDatasetStyle(SPRINT_SPENT, HMX_DATASET_STYLE_AREALINE);
	SetDatasetLineColor(SPRINT_SPENT, COLOR_YELLOWLINE);
	SetDatasetFillColor(SPRINT_SPENT, COLOR_YELLOWFILL);
	SetDatasetSizeFactor(SPRINT_SPENT, LINE_THICKNESS);
	SetDatasetMin(SPRINT_SPENT, 0.0);
	SetDrawDatasetOverGrid(SPRINT_SPENT, false);
	
	// build the graph
	COleDateTime dtStart = GetGraphStartDate();
	COleDateTime dtEnd = GetGraphEndDate();
	
	double dTotalEst = m_data.CalcTotalTimeEstimateInDays(m_nDaysInWeek, m_dHoursInDay);
	
	int nNumDays = ((int)dtEnd.m_dt - (int)dtStart.m_dt);
	
	for (int nDay = 0; nDay <= nNumDays; nDay++)
	{
		// Time Estimate
		double dEst = ((nDay * dTotalEst) / nNumDays);
		AddData(SPRINT_EST, (dTotalEst - dEst));
		
		// Time Spent
		COleDateTime date(dtStart.m_dt + nDay);
		double dSpent = m_data.CalcTimeSpentInDays(date, m_nDaysInWeek, m_dHoursInDay);
		
		AddData(SPRINT_SPENT, (dTotalEst - dSpent));
	}
	
	// Set the maximum Y value to be something 'nice'
	double dMin, dMax;

	if (GetMinMax(dMin, dMax, true))
	{
		ASSERT(dMin == 0.0);

		dMax = CalcMaxYAxisValue(dMax);
		SetDatasetMax(SPRINT_EST, dMax);
		SetDatasetMax(SPRINT_SPENT, dMax);
	}
	
	CalcDatas();
}

int CBurndownChart::CalculateRequiredXScale() const
{
	// calculate new x scale
	int nDataWidth = GetDataArea().cx;
	int nNumDays = m_dtExtents.GetDayCount();

	// work thru the available scales until we find a suitable one
	for (int nScale = 0; nScale < NUM_SCALES; nScale++)
	{
		int nSpacing = MulDiv(SCALES[nScale], nDataWidth, nNumDays);

		if (nSpacing > MIN_XSCALE_SPACING)
			return SCALES[nScale];
	}

	return BCS_YEAR;
}

void CBurndownChart::RebuildXScale()
{
	ClearXScaleLabels();

	// calc new scale
	m_nScale = CalculateRequiredXScale();
	SetXLabelStep(m_nScale);

	// Get new start and end dates
	COleDateTime dtStart = GetGraphStartDate();
	COleDateTime dtEnd = GetGraphEndDate();

	// build ticks
	int nNumDays = ((int)dtEnd.m_dt - (int)dtStart.m_dt);
	COleDateTime dtTick = dtStart;
	CString sTick;

	for (int nDay = 0; nDay <= nNumDays; nDay += m_nScale)
	{
		sTick = CDateHelper::FormatDate(dtTick);
		SetXScaleLabel(nDay, sTick);

		// next Tick date
		switch (m_nScale)
		{
		case BCS_DAY:
			dtTick.m_dt += 1.0;
			break;
			
		case BCS_WEEK:
			CDateHelper::OffsetDate(dtTick, 1, DHU_WEEKS);
			break;
			
		case BCS_MONTH:
			CDateHelper::OffsetDate(dtTick, 1, DHU_MONTHS);
			break;
			
		case BCS_2MONTH:
			CDateHelper::OffsetDate(dtTick, 2, DHU_MONTHS);
			break;
			
		case BCS_QUARTER:
			CDateHelper::OffsetDate(dtTick, 3, DHU_MONTHS);
			break;
			
		case BCS_HALFYEAR:
			CDateHelper::OffsetDate(dtTick, 6, DHU_MONTHS);
			break;
			
		case BCS_YEAR:
			CDateHelper::OffsetDate(dtTick, 1, DHU_YEARS);
			break;
			
		default:
			ASSERT(0);
		}
	}
}

COleDateTime CBurndownChart::GetGraphStartDate() const
{
	if (m_nChartType == BCT_REMAININGDAYS)
		return m_dtExtents.GetStart();

	// else
	COleDateTime dtStart(m_dtExtents.GetStart());

	// back up a bit to always show first completion
	dtStart -= COleDateTimeSpan(7.0);

	SYSTEMTIME st = { 0 };
	VERIFY(dtStart.GetAsSystemTime(st));

	switch (m_nScale)
	{
	case BCS_DAY:
	case BCS_WEEK:
		// make sure we start at the beginning of a week
		dtStart.m_dt -= st.wDayOfWeek;
		return dtStart;
		
	case BCS_MONTH:
		st.wDay = 1; // start of month;
		break;
		
	case BCS_2MONTH:
		st.wDay = 1; // start of month;
		st.wMonth = (WORD)(st.wMonth - ((st.wMonth - 1) % 2)); // previous even month
		break;

	case BCS_QUARTER:
		st.wDay = 1; // start of month;
		st.wMonth = (WORD)(st.wMonth - ((st.wMonth - 1) % 3)); // previous quarter
		break;
		
	case BCS_HALFYEAR:
		st.wDay = 1; // start of month;
		st.wMonth = (WORD)(st.wMonth - ((st.wMonth - 1) % 6)); // previous half-year
		break;
		
	case BCS_YEAR:
		st.wDay = 1; // start of month;
		st.wMonth = 1; // start of year
		break;

	default:
		ASSERT(0);
	}

	return COleDateTime(st.wYear, st.wMonth, st.wDay, 0, 0, 0);
}

COleDateTime CBurndownChart::GetGraphEndDate() const
{
	if (m_nChartType == BCT_REMAININGDAYS)
		return m_dtExtents.GetEnd();

	COleDateTime dtEnd = (m_dtExtents.GetEnd() + COleDateTimeSpan(7.0));

	// avoid unnecessary call to GetAsSystemTime()
	if (m_nScale == BCS_DAY)
		return dtEnd;

	SYSTEMTIME st = { 0 };
	VERIFY(dtEnd.GetAsSystemTime(st));

	switch (m_nScale)
	{
	case BCS_DAY:
		ASSERT(0); // handled above
		break;
		
	case BCS_WEEK:
		break;
		
	case BCS_MONTH:
		st.wDay = (WORD)CDateHelper::GetDaysInMonth(st.wMonth, st.wYear); // end of month;
		break;
		
	case BCS_2MONTH:
		CDateHelper::IncrementMonth(st, ((st.wMonth - 1) % 2)); // next even month
		st.wDay = (WORD)CDateHelper::GetDaysInMonth(st.wMonth, st.wYear); // end of month;
		break;

	case BCS_QUARTER:
		CDateHelper::IncrementMonth(st, ((st.wMonth - 1) % 3)); // next quarter
		st.wDay = (WORD)CDateHelper::GetDaysInMonth(st.wMonth, st.wYear); // end of month;
		break;
		
	case BCS_HALFYEAR:
		CDateHelper::IncrementMonth(st, ((st.wMonth - 1) % 6)); // next half-year
		st.wDay = (WORD)CDateHelper::GetDaysInMonth(st.wMonth, st.wYear); // end of month;
		break;
		
	case BCS_YEAR:
		st.wDay = 31;
		st.wMonth = 12;
		break;

	default:
		ASSERT(0);
	}

	return COleDateTime(st.wYear, st.wMonth, st.wDay, 0, 0, 0);
}

void CBurndownChart::OnSize(UINT nType, int cx, int cy) 
{
	CHMXChart::OnSize(nType, cx, cy);
	
	int nOldScale = m_nScale;
	RebuildXScale();
		
	// handle scale change
	if (m_nScale != nOldScale)
		RebuildGraph(FALSE);
}

void CBurndownChart::RebuildGraph(BOOL bUpdateExtents)
{
	CWaitCursor cursor;
	CHoldRedraw hr(*this);
	
	if (bUpdateExtents || (m_dtExtents.GetDayCount() == 0))
		m_data.GetDataExtents(m_dtExtents);
	
	ClearData();
	SetYText(CEnString(STATSDISPLAY[m_nChartType].nYAxisID));
	
	if (!m_data.IsEmpty())
		RebuildXScale();
	
	switch (STATSDISPLAY[m_nChartType].nDisplay)
	{
	case BCT_INCOMPLETETASKS:
		BuildBurndownGraph();
		break;
		
	case BCT_REMAININGDAYS:
		BuildSprintGraph();
		break;
	}
}

void CBurndownChart::PreSubclassWindow()
{
	SetBkGnd(GetSysColor(COLOR_WINDOW));
	SetXLabelsAreTicks(true);
	SetXLabelAngle(45);
	SetYTicks(10);
}

bool CBurndownChart::DrawHorzLine(CDC& dc)
{
	double dInterval = CalcYAxisInterval(m_nYMax);
	int nNumSubTicks = GetYSubTicks(dInterval);

	if (nNumSubTicks > 1)
	{
		CPen penSubGrid(PS_SOLID, 1, GraphicsMisc::Lighter(GetGridColor(), 0.6));
		CPen* pPenOld = dc.SelectObject(&penSubGrid);

		int nTotalTicks = (GetYTicks() * nNumSubTicks);
		double nY = ((m_nYMax - m_nYMin)/nTotalTicks);
	
		for(int f=0; f<=nTotalTicks; f++) 
		{
			if (f % nNumSubTicks)
			{
				double nTemp = m_rectData.bottom - (nY*f) * m_rectData.Height()/(m_nYMax-m_nYMin);

				dc.MoveTo(m_rectData.left , (int)nTemp);
				dc.LineTo(m_rectData.right, (int)nTemp);
			}
		}

		dc.SelectObject(pPenOld);
	}

	return CHMXChart::DrawHorzLine(dc);
}
