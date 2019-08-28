#pragma once

#include "BurndownEnum.h"

/////////////////////////////////////////////////////////////////////////////

class COleDateTimeRange;
class CBurndownChart;
class CStatsItemArray;

/////////////////////////////////////////////////////////////////////////////

class CIncompleteDaysGraph
{
public:
	static void BuildGraph(const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE nScale, const CStatsItemArray& data, CBurndownChart& chart);
	static CString GetTooltip(const CBurndownChart& chart, const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE nScale, int nHit);

	static COleDateTime GetGraphStartDate(const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE nScale);
	static COleDateTime GetGraphEndDate(const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE nScale);
};

/////////////////////////////////////////////////////////////////////////////

class CRemainingDaysGraph
{
public:
	static void BuildGraph(const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE nScale, const CStatsItemArray& data, CBurndownChart& chart);
	static CString GetTooltip(const CBurndownChart& chart, const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE nScale, int nHit);

	static COleDateTime GetGraphStartDate(const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE nScale);
	static COleDateTime GetGraphEndDate(const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE nScale);
};

/////////////////////////////////////////////////////////////////////////////

