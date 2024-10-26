#if !defined(AFX_BURNDOWNSTATIC_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
#define AFX_BURNDOWNSTATIC_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "BurndownStruct.h"
#include "BurndownEnum.h"

/////////////////////////////////////////////////////////////////////////////

static const GRAPHTYPE GRAPHTYPES[] = 
{
	{ BCT_TIMESERIES, IDS_TIMESERIES },
	{ BCT_FREQUENCY,  IDS_FREQUENCYDIST },
	{ BCT_MINMAX,     IDS_MINMAX },
};

static const int NUM_GRAPHTYPES = (sizeof(GRAPHTYPES) / sizeof(GRAPHTYPES[0]));

/////////////////////////////////////////////////////////////////////////////

static const GRAPHOPTION GRAPHOPTIONS[] = 
{
	{ BGO_TREND_BESTFIT,		BCT_TIMESERIES,	},	// default
	{ BGO_TREND_7DAYAVERAGE,	BCT_TIMESERIES, },
	{ BGO_TREND_30DAYAVERAGE,	BCT_TIMESERIES, },
	{ BGO_TREND_90DAYAVERAGE,	BCT_TIMESERIES, },
	{ BGO_TREND_NONE,			BCT_TIMESERIES	},

	{ BGO_FREQUENCY_BAR,		BCT_FREQUENCY, },	// default
	{ BGO_FREQUENCY_LINE,		BCT_FREQUENCY, },
	{ BGO_FREQUENCY_PIE,		BCT_FREQUENCY, },
	{ BGO_FREQUENCY_DONUT,		BCT_FREQUENCY, },

	{ BGO_MINMAX_NONE,			BCT_MINMAX, },		// default
};

static const int NUM_OPTIONS = sizeof(GRAPHOPTIONS) / sizeof(GRAPHOPTION);

// -------------------------------------------------------------------

static BURNDOWN_GRAPHTYPE GetGraphType(BURNDOWN_GRAPHOPTION nOption)
{
	int nOpt = NUM_OPTIONS;

	while (nOpt--)
	{
		if (GRAPHOPTIONS[nOpt].nOption == nOption)
			return GRAPHOPTIONS[nOpt].nType;
	}

	return BCT_UNKNOWNTYPE;
}

static BOOL IsCustomAttributeGraph(BURNDOWN_GRAPH nGraph)
{
	return ((nGraph >= BCG_CUSTOMATTRIB_FIRST) && (nGraph <= BCG_CUSTOMATTRIB_LAST));
}

static BOOL IsValidOption(BURNDOWN_GRAPHOPTION nOption, BURNDOWN_GRAPHTYPE nType)
{
	if ((nType == BCT_UNKNOWNTYPE) || (nOption == BGO_INVALID))
		return FALSE;

	return (GetGraphType(nOption) == nType);
}

static BURNDOWN_GRAPHOPTION GetDefaultOption(BURNDOWN_GRAPHTYPE nType)
{
	for (int nItem = 0; nItem < NUM_OPTIONS; nItem++)
	{
		if (nType == GRAPHOPTIONS[nItem].nType)
			return GRAPHOPTIONS[nItem].nOption;
	}

	ASSERT(0);
	return BGO_INVALID;
}

/////////////////////////////////////////////////////////////////////////////

static BURNDOWN_GRAPHSCALE SCALES[] =
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

// ---------------------------------------------------------

static BURNDOWN_GRAPHSCALE CalculateRequiredScale(int nAvailSpace, int nNumDays, int nMinSpacing)
{
	// work thru the available scales until we find a suitable one
	for (int nScale = 0; nScale < NUM_SCALES; nScale++)
	{
		int nSpacing = MulDiv(SCALES[nScale], nAvailSpace, nNumDays);

		if (nSpacing > nMinSpacing)
			return SCALES[nScale];
	}

	return BCS_YEAR;
}

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_BURNDOWNSTATIC_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
