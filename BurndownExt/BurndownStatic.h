#if !defined(AFX_BURNDOWNSTATIC_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
#define AFX_BURNDOWNSTATIC_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "BurndownStruct.h"
#include "BurndownEnum.h"

/////////////////////////////////////////////////////////////////////////////

static DISPLAYITEM STATSDISPLAY[] = 
{
	{ IDS_DISPLAY_INCOMPLETE, BCT_INCOMPLETETASKS	},
	{ IDS_DISPLAY_REMAINING, BCT_REMAININGDAYS },
};
static int NUM_DISPLAY = sizeof(STATSDISPLAY) / sizeof(DISPLAYITEM);


#endif // !defined(AFX_BURNDOWNSTATIC_H__F2F5ABDC_CDB2_4197_A8E1_6FE134F95A20__INCLUDED_)
