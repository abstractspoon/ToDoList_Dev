
#pragma once

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

enum BURNDOWN_GRAPHTYPE
{
	BCT_UNKNOWNTYPE	= -1,

	BCT_TIMESERIES	= 0,
	BCT_FREQUENCY	= 100,
	BCT_MINMAX		= 200,
	// BCT_NEXT		= 300,

	// Always the last
	BCT_NUMTYPES
};

/////////////////////////////////////////////////////////////////////////////

enum BURNDOWN_GRAPH
{
	BCG_UNKNOWNGRAPH = -1,

	BCG_TIMESERIES_INCOMPLETETASKS		= BCT_TIMESERIES + 1,
	BCG_TIMESERIES_REMAININGDAYS,
	BCG_TIMESERIES_STARTEDENDEDTASKS,
	BCG_TIMESERIES_ESTIMATEDSPENTDAYS,
	BCG_TIMESERIES_ESTIMATEDSPENTCOST,
	// new graphs here

	// --------------------------------------------------------

	BCG_FREQUENCY_CATEGORY				= BCT_FREQUENCY + 1,
	BCG_FREQUENCY_STATUS, 
	BCG_FREQUENCY_ALLOCTO, 
	BCG_FREQUENCY_ALLOCBY, 
	BCG_FREQUENCY_PRIORITY, 
	BCG_FREQUENCY_RISK, 
	BCG_FREQUENCY_TAGS, 
	BCG_FREQUENCY_VERSION,
	// new graphs here

	// --------------------------------------------------------

	BCG_MINMAX_ESTIMATEDSPENTDAYS		= BCT_MINMAX + 1,
	BCG_MINMAX_DUEDONEDATES,
	// new graphs here

	// --------------------------------------------------------

	BCG_CUSTOMATTRIB_FIRST			= 1000,
	BCG_CUSTOMATTRIB_LAST			= 1063,

	// Always the last
	BCT_NUMGRAPHS
};

typedef CArray<BURNDOWN_GRAPH, BURNDOWN_GRAPH&> CGraphArray;

/////////////////////////////////////////////////////////////////////////////

enum BURNDOWN_GRAPHSCALE
{
	BCS_DAY			= 1,
	BCS_WEEK		= 7,
	BCS_MONTH		= 30,
	BCS_2MONTH		= 60,
	BCS_QUARTER		= 91,
	BCS_HALFYEAR	= 182,
	BCS_YEAR		= 365,
};

/////////////////////////////////////////////////////////////////////////////

enum BURNDOWN_GRAPHOPTION
{
	BGO_INVALID = -1,

	BGO_TREND_NONE,
	BGO_TREND_BESTFIT,
	BGO_TREND_7DAYAVERAGE,
	BGO_TREND_30DAYAVERAGE,
	BGO_TREND_90DAYAVERAGE,

	BGO_FREQUENCY_BAR,
	BGO_FREQUENCY_LINE,
	BGO_FREQUENCY_PIE,
	BGO_FREQUENCY_DONUT,

	BGO_MINMAX_NONE,

	// new options here
};

/////////////////////////////////////////////////////////////////////////////



