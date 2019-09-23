
#pragma once

/////////////////////////////////////////////////////////////////////////////

enum BURNDOWN_GRAPHTYPE
{
	BCT_TIMESERIES,
	BCT_FREQUENCY,
};

/////////////////////////////////////////////////////////////////////////////

enum BURNDOWN_GRAPH
{
	BCT_TIMESERIES_INCOMPLETETASKS,
	BCT_TIMESERIES_REMAININGDAYS,
	BCT_TIMESERIES_STARTEDENDEDTASKS,
	BCT_TIMESERIES_ESTIMATEDSPENTDAYS,
	//BCT_TIMESERIES_ESTIMATEDSPENTCOST,

	BCT_FREQUENCY_CATEGORY,
	//BCT_FREQUENCY_STATUS, 
	//BCT_FREQUENCY_ALLOCTO, 
	//BCT_FREQUENCY_ALLOCBY, 
	//BCT_FREQUENCY_PRIORITY, 
	//BCT_FREQUENCY_RISK, 
	//BCT_FREQUENCY_TAGS, 
	//BCT_FREQUENCY_VERSION,
	// new graphs here

	BCT_NUMGRAPHS // always last
};

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

enum BURNDOWN_TREND
{
	BTL_NONE = -1,
	BTL_BEST_FIT,
	BTL_7DAY_ROLLING_AVERAGE,
	BTL_30DAY_ROLLING_AVERAGE,
};

/////////////////////////////////////////////////////////////////////////////

const int MIN_XSCALE_SPACING = 50; // pixels

/////////////////////////////////////////////////////////////////////////////



