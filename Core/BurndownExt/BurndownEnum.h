
#pragma once

/////////////////////////////////////////////////////////////////////////////

enum BURNDOWN_GRAPHTYPE
{
	BCT_INCOMPLETETASKS,
	BCT_REMAININGDAYS,
	BCT_STARTEDENDEDTASKS,
	BCT_ESTIMATEDSPENTDAYS,
	//BCT_ESTIMATEDSPENTCOST,
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

enum BURNDOWN_TRENDTYPE
{
	BTL_NONE = -1,
	BTL_BEST_FIT,
	BTL_7DAY_ROLLING_AVERAGE,
	BTL_30DAY_ROLLING_AVERAGE,
};

/////////////////////////////////////////////////////////////////////////////



