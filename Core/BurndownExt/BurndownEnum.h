
#pragma once

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

enum BURNDOWN_GRAPHTYPE
{
	BCT_UNKNOWNTYPE	= -1,
	BCT_TIMESERIES	= 0,
	BCT_FREQUENCY	= 100,
	// BCT_NEXT		= 200,

	// Always the last
	BCT_NUMTYPES
};

/////////////////////////////////////////////////////////////////////////////

enum BURNDOWN_GRAPH
{
	BCT_UNKNOWNGRAPH = -1,

	BCT_TIMESERIES_INCOMPLETETASKS		= BCT_TIMESERIES + 1,
	BCT_TIMESERIES_REMAININGDAYS,
	BCT_TIMESERIES_STARTEDENDEDTASKS,
	BCT_TIMESERIES_ESTIMATEDSPENTDAYS,
	BCT_TIMESERIES_ESTIMATEDSPENTCOST,
	// new graphs here

	BCT_FREQUENCY_CATEGORY				= BCT_FREQUENCY + 1,
	BCT_FREQUENCY_STATUS, 
	BCT_FREQUENCY_ALLOCTO, 
	BCT_FREQUENCY_ALLOCBY, 
	BCT_FREQUENCY_PRIORITY, 
	BCT_FREQUENCY_RISK, 
	BCT_FREQUENCY_TAGS, 
	BCT_FREQUENCY_VERSION,
	// new graphs here

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

enum // BURNDOWN_GRAPH_OPTIONS
{
	BGO_TIMESERIES_TREND_NONE			= 0x00,
	BGO_TIMESERIES_TREND_BESTFIT		= 0x01,
	BGO_TIMESERIES_TREND_7DAYAVERAGE	= 0x02,
	BGO_TIMESERIES_TREND_30DAYAVERAGE	= 0x03,
	BGO_TIMESERIES_TREND_90DAYAVERAGE	= 0x04,
	BGO_TIMESERIES_TREND_MASK			= 0x0f,

	BGO_FREQUENCY_STYLE_BAR				= 0x00,
	BGO_FREQUENCY_STYLE_LINE			= 0x10,
	BGO_FREQUENCY_STYLE_PIE				= 0x20,
	BGO_FREQUENCY_STYLE_DONUT			= 0x30,
	BGO_FREQUENCY_STYLE_MASK			= 0xf0,

	// new options here
};

/////////////////////////////////////////////////////////////////////////////



