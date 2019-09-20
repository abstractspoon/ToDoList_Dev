#if !defined(AFX_BURNDOWNSTATIC_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
#define AFX_BURNDOWNSTATIC_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "BurndownStruct.h"
#include "BurndownEnum.h"

/////////////////////////////////////////////////////////////////////////////

static const TRENDLINE TRENDS[] = 
{
	{ IDS_TREND_NONE,			BTL_NONE	},
	{ IDS_TREND_BESTFIT,		BTL_BEST_FIT	},
	{ IDS_TREND_7DAYAVERAGE,	BTL_7DAY_ROLLING_AVERAGE },
	{ IDS_TREND_30DAYAVERAGE,	BTL_30DAY_ROLLING_AVERAGE },
};

static const int NUM_TRENDS = sizeof(TRENDS) / sizeof(TRENDLINE);

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_BURNDOWNSTATIC_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
