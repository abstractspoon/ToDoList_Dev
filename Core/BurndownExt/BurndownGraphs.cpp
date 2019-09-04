// BurndownChart.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BurndownGraphs.h"
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

const int    LINE_THICKNESS			= 1;

/////////////////////////////////////////////////////////////////////////////

void CIncompleteDaysGraph::BuildGraph(const CStatsItemCalculator& calculator, BURNDOWN_CHARTSCALE nScale, CHMXDataset datasets[HMX_MAX_DATASET])
{
	datasets[0].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[0].SetLineColor(COLOR_GREENLINE);
	datasets[0].SetFillColor(COLOR_GREENFILL);
	datasets[0].SetSize(LINE_THICKNESS);
	datasets[0].SetMin(0.0);

	// build the graph
	const COleDateTimeRange& dtExtents = calculator.GetDateRange();

	COleDateTime dtStart = GetGraphStartDate(dtExtents, nScale);
	COleDateTime dtEnd = GetGraphEndDate(dtExtents, nScale);
	
	int nNumDays = ((int)dtEnd.m_dt - (int)dtStart.m_dt);

	if (nNumDays)
	{
		int nItemFrom = 0;
	
		for (int nDay = 0; nDay <= nNumDays; nDay++)
		{
			COleDateTime date(dtStart.m_dt + nDay);
		
			if (dtExtents.GetStart() > date)
			{
				datasets[0].AddData(0);
			}
			else
			{
				int nNumNotDone = calculator.GetIncompleteTaskCount(date, nItemFrom, nItemFrom);
				datasets[0].AddData(nNumNotDone);
			}
		}

		// Set the maximum Y value to be something 'nice'
		double dMin, dMax;

		if (HMXUtils::GetMinMax(datasets, 1, dMin, dMax, true))
		{
			ASSERT(dMin == 0.0);

			dMax = HMXUtils::CalcMaxYAxisValue(dMax, 10);
			datasets[0].SetMax(dMax);
		}
	}
}

COleDateTime CIncompleteDaysGraph::GetGraphStartDate(const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE nScale)
{
	return dtExtents.GetStart();

	COleDateTime dtStart(dtExtents.GetStart());

	// back up a bit to always show first completion
	dtStart -= COleDateTimeSpan(7.0);

	SYSTEMTIME st = { 0 };
	VERIFY(dtStart.GetAsSystemTime(st));

	switch (nScale)
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

COleDateTime CIncompleteDaysGraph::GetGraphEndDate(const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE nScale)
{
	return dtExtents.GetEnd();

	COleDateTime dtEnd = (dtExtents.GetEnd() + COleDateTimeSpan(7.0));

	// avoid unnecessary call to GetAsSystemTime()
	if (nScale == BCS_DAY)
		return dtEnd;

	SYSTEMTIME st = { 0 };
	VERIFY(dtEnd.GetAsSystemTime(st));

	switch (nScale)
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

CString CIncompleteDaysGraph::GetTooltip(const CHMXDataset datasets[HMX_MAX_DATASET], const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE nScale, int nHit)
{
	ASSERT(nHit != -1);

	CString sTooltip;
	double dNumTasks;

	if (datasets[0].GetData(nHit, dNumTasks))
	{
		double dDate = (GetGraphStartDate(dtExtents, nScale).m_dt + nHit);
		sTooltip.Format(CEnString(IDS_TOOLTIP_INCOMPLETE), CDateHelper::FormatDate(dDate), (int)dNumTasks);
	}

	return sTooltip;
}

//////////////////////////////////////////////////////////////////////////////////////

enum
{
	REMAINING_ESTIMATE,
	REMAINING_SPENT
};

void CRemainingDaysGraph::BuildGraph(const CStatsItemCalculator& calculator, BURNDOWN_CHARTSCALE nScale, CHMXDataset datasets[HMX_MAX_DATASET])
{
	datasets[REMAINING_ESTIMATE].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[REMAINING_ESTIMATE].SetLineColor(COLOR_BLUELINE);
	datasets[REMAINING_ESTIMATE].SetFillColor(COLOR_BLUEFILL);
	datasets[REMAINING_ESTIMATE].SetSize(LINE_THICKNESS);
	datasets[REMAINING_ESTIMATE].SetMin(0.0);
	
	datasets[REMAINING_SPENT].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[REMAINING_SPENT].SetLineColor(COLOR_YELLOWLINE);
	datasets[REMAINING_SPENT].SetFillColor(COLOR_YELLOWFILL);
	datasets[REMAINING_SPENT].SetSize(LINE_THICKNESS);
	datasets[REMAINING_SPENT].SetMin(0.0);
	
	// build the graph
	const COleDateTimeRange& dtExtents = calculator.GetDateRange();

	COleDateTime dtStart = GetGraphStartDate(dtExtents, nScale);
	COleDateTime dtEnd = GetGraphEndDate(dtExtents, nScale);
	
	double dTotalEst = calculator.GetTotalTimeEstimateInDays();
	
	int nNumDays = ((int)dtEnd.m_dt - (int)dtStart.m_dt);
	
	if (nNumDays > 0)
	{
		for (int nDay = 0; nDay <= nNumDays; nDay++)
		{
			// Time Estimate
			double dEst = ((nDay * dTotalEst) / nNumDays);

			// last value is always zero
			if (nDay == nNumDays)
				datasets[REMAINING_ESTIMATE].AddData(0.0);
			else
				datasets[REMAINING_ESTIMATE].AddData(dTotalEst - dEst);
		
			// Time Spent
			COleDateTime date(dtStart.m_dt + nDay);
			double dSpent = calculator.GetTimeSpentInDays(date);
		
			datasets[REMAINING_SPENT].AddData(dTotalEst - dSpent);
		}
	
		// Set the maximum Y value to be something 'nice'
		double dMin, dMax;

		if (HMXUtils::GetMinMax(datasets, 2, dMin, dMax, true))
		{
			ASSERT(dMin == 0.0);

			dMax = HMXUtils::CalcMaxYAxisValue(dMax, 10);

			datasets[REMAINING_ESTIMATE].SetMax(dMax);
			datasets[REMAINING_SPENT].SetMax(dMax);
		}
	}
}

COleDateTime CRemainingDaysGraph::GetGraphStartDate(const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE /*nScale*/)
{
	return dtExtents.GetStart();
}

COleDateTime CRemainingDaysGraph::GetGraphEndDate(const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE /*nScale*/)
{
	return dtExtents.GetEnd();
}

CString CRemainingDaysGraph::GetTooltip(const CHMXDataset datasets[HMX_MAX_DATASET], const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE nScale, int nHit)
{
	ASSERT(nHit != -1);

	double dDate = (GetGraphStartDate(dtExtents, nScale).m_dt + nHit), dNumEst, dNumSpent;
	CString sTooltip;

	if (datasets[REMAINING_SPENT].GetData(nHit, dNumSpent) &&
		datasets[REMAINING_ESTIMATE].GetData(nHit, dNumEst))
	{
		sTooltip.Format(CEnString(IDS_TOOLTIP_REMAINING), CDateHelper::FormatDate(dDate), (int)dNumEst, (int)dNumSpent);
	}

	return sTooltip;
}

//////////////////////////////////////////////////////////////////////////////////////


