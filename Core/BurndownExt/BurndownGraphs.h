#pragma once

#include "BurndownEnum.h"

#include "..\3rdParty\HMXChart.h"

/////////////////////////////////////////////////////////////////////////////

class COleDateTimeRange;
class CStatsItemCalculator;

/////////////////////////////////////////////////////////////////////////////

class CIncompleteDaysGraph
{
public:
	static void BuildGraph(const CStatsItemCalculator& calculator, const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE nScale, CHMXDataset datasets[HMX_MAX_DATASET]);
	static CString GetTooltip(const CHMXDataset datasets[HMX_MAX_DATASET], const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE nScale, int nHit);

	static COleDateTime GetGraphStartDate(const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE nScale);
	static COleDateTime GetGraphEndDate(const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE nScale);
};

/////////////////////////////////////////////////////////////////////////////

class CRemainingDaysGraph
{
public:
	static void BuildGraph(const CStatsItemCalculator& calculator, const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE nScale, CHMXDataset datasets[HMX_MAX_DATASET]);
	static CString GetTooltip(const CHMXDataset datasets[HMX_MAX_DATASET], const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE nScale, int nHit);

	static COleDateTime GetGraphStartDate(const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE nScale);
	static COleDateTime GetGraphEndDate(const COleDateTimeRange& dtExtents, BURNDOWN_CHARTSCALE nScale);
};

/////////////////////////////////////////////////////////////////////////////

