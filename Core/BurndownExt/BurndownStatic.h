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
};

static const int NUM_GRAPHTYPES = (sizeof(GRAPHTYPES) / sizeof(GRAPHTYPES[0]));

/////////////////////////////////////////////////////////////////////////////

static BURNDOWN_GRAPHTYPE GetGraphType(BURNDOWN_GRAPH nGraph)
{
	if (nGraph == BCT_UNKNOWNGRAPH)
		return BCT_UNKNOWNTYPE;

	for (int nType = 0; nType < NUM_GRAPHTYPES; nType++)
	{
		int nRangeStart = GRAPHTYPES[nType].nType, nRangeEnd;

		if (nType == (NUM_GRAPHTYPES - 1))
			nRangeEnd = BCT_NUMGRAPHS;
		else
			nRangeEnd = GRAPHTYPES[nType + 1].nType;

		if ((nGraph >= nRangeStart) && (nGraph < nRangeEnd))
			return GRAPHTYPES[nType].nType;
	}

	ASSERT(0);
	return BCT_UNKNOWNTYPE;
}

static BOOL IsValidGraph(BURNDOWN_GRAPH nGraph)
{
	return (GetGraphType(nGraph) != BCT_UNKNOWNTYPE);
}

/////////////////////////////////////////////////////////////////////////////

static BOOL IsValidOption(DWORD dwOption, BURNDOWN_GRAPHTYPE nType)
{
	if (nType == BCT_UNKNOWNTYPE)
		return FALSE;

	DWORD dwValidOptions = 0;

	switch (nType)
	{
	case BCT_TIMESERIES:
		dwValidOptions = BGO_TIMESERIES_TREND_MASK;
		break;

	case BCT_FREQUENCY:
		dwValidOptions = BGO_FREQUENCY_STYLE_MASK;
		break;
	}
	ASSERT(dwValidOptions);

	return ((dwOption & dwValidOptions) == dwOption);
}

static DWORD GetDefaultOption(BURNDOWN_GRAPHTYPE nType)
{
	switch (nType)
	{
	case BCT_TIMESERIES:	return BGO_TIMESERIES_TREND_BESTFIT;
	case BCT_FREQUENCY:		return BGO_FREQUENCY_STYLE_BAR;
		break;
	}

	ASSERT(0);
	return 0;
}

static DWORD GetDefaultOption(BURNDOWN_GRAPH nGraph)
{
	return GetDefaultOption(GetGraphType(nGraph));
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

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_BURNDOWNSTATIC_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
