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

CString CIncompleteTasksGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_INCOMPLETE);
}

void CIncompleteTasksGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	datasets[0].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[0].SetLineColor(COLOR_GREENLINE);
	datasets[0].SetFillColor(COLOR_GREENFILL);
	datasets[0].SetSize(LINE_THICKNESS);
	datasets[0].SetMin(0.0);

	// build the graph
	int nNumDays = calculator.GetTotalDays();

	if (nNumDays)
	{
		COleDateTime dtStart = calculator.GetStartDate();
		int nItemFrom = 0;
	
		for (int nDay = 0; nDay <= nNumDays; nDay++)
		{
			COleDateTime date(dtStart.m_dt + nDay);
		
			if (dtStart > date)
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

CString CIncompleteTasksGraph::GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const
{
	ASSERT(nHit != -1);

	CString sTooltip;
	double dNumTasks;

	if (datasets[0].GetData(nHit, dNumTasks))
	{
		double dDate = (calculator.GetStartDate().m_dt + nHit);
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

CString CRemainingDaysGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_REMAINING);
}

void CRemainingDaysGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
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
	int nNumDays = calculator.GetTotalDays();
	
	if (nNumDays > 0)
	{
		COleDateTime dtStart = calculator.GetStartDate();
		double dTotalEst = calculator.GetTotalTimeEstimateInDays();

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

CString CRemainingDaysGraph::GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const
{
	ASSERT(nHit != -1);

	double dDate = (calculator.GetStartDate().m_dt + nHit), dNumEst, dNumSpent;
	CString sTooltip;

	if (datasets[REMAINING_SPENT].GetData(nHit, dNumSpent) &&
		datasets[REMAINING_ESTIMATE].GetData(nHit, dNumEst))
	{
		sTooltip.Format(CEnString(IDS_TOOLTIP_REMAINING), CDateHelper::FormatDate(dDate), (int)dNumEst, (int)dNumSpent);
	}

	return sTooltip;
}

//////////////////////////////////////////////////////////////////////////////////////

enum
{
	STARTED_TASKS,
	ENDED_TASKS
};

CString CStartedCompletedTasksGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_STARTEDCOMPLETED);
}

void CStartedCompletedTasksGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	datasets[STARTED_TASKS].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[STARTED_TASKS].SetLineColor(COLOR_GREENLINE);
	datasets[STARTED_TASKS].SetFillColor(COLOR_GREENFILL);
	datasets[STARTED_TASKS].SetSize(LINE_THICKNESS);
	datasets[STARTED_TASKS].SetMin(0.0);

	datasets[ENDED_TASKS].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[ENDED_TASKS].SetLineColor(COLOR_REDLINE);
	datasets[ENDED_TASKS].SetFillColor(COLOR_REDFILL);
	datasets[ENDED_TASKS].SetSize(LINE_THICKNESS);
	datasets[ENDED_TASKS].SetMin(0.0);

	// build the graph
	int nNumDays = calculator.GetTotalDays();

	if (nNumDays > 0)
	{
		COleDateTime dtStart = calculator.GetStartDate();

		for (int nDay = 0; nDay <= nNumDays; nDay++)
		{
			COleDateTime date(dtStart.m_dt + nDay);
			int nNumStarted = 0, nNumDone = 0;

			if (date > dtStart)
				calculator.GetStartedCompletedTaskCounts(date, nNumStarted, nNumDone);

			datasets[STARTED_TASKS].AddData(nNumStarted);
			datasets[ENDED_TASKS].AddData(nNumDone);
		}

		// Set the maximum Y value to be something 'nice'
		double dMin, dMax;

		// Note: started count is always greater than completed count
		// so we only need to get the max/min of the started tasks
		if (HMXUtils::GetMinMax(datasets, 1, dMin, dMax, true))
		{
			ASSERT(dMin == 0.0);

			dMax = HMXUtils::CalcMaxYAxisValue(dMax, 10);

			datasets[STARTED_TASKS].SetMax(dMax);
			datasets[ENDED_TASKS].SetMax(dMax);
		}
	}
}

CString CStartedCompletedTasksGraph::GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const
{
	ASSERT(nHit != -1);

	double dDate = (calculator.GetStartDate().m_dt + nHit), dNumStarted, nNumDone;
	CString sTooltip;

	if (datasets[STARTED_TASKS].GetData(nHit, dNumStarted) &&
		datasets[ENDED_TASKS].GetData(nHit, nNumDone))
	{
		sTooltip.Format(CEnString(IDS_TOOLTIP_STARTEDCOMPLETED), CDateHelper::FormatDate(dDate), (int)dNumStarted, (int)nNumDone);
	}

	return sTooltip;
}

/////////////////////////////////////////////////////////////////////////////
