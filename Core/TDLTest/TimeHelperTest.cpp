// DateHelperTest.cpp: implementation of the CTimeHelperTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "TimeHelperTest.h"

#include "..\shared\Timehelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeHelperTest::CTimeHelperTest(const CTestUtils& utils) : CTDLTestBase(utils)
{

}

CTimeHelperTest::~CTimeHelperTest()
{

}

TESTRESULT CTimeHelperTest::Run()
{
	ClearTotals();

	TestGetTime();

	return GetTotals();
}

void CTimeHelperTest::TestGetTime()
{
	BeginTest(_T("CTimeHelperTest::GetTime"));

	// -----------------------------------------------------------------------

	{
		// Use default statically defined internal values
		const CTimeHelper th;

		const double DAYSINWEEK = 5.0;
		const double HOURSINDAY = 8.0;

		// -----------------------------------------------------------------------

		ExpectEQ(th.Convert(1.0, THU_DAYS,		THU_HOURS), HOURSINDAY);
		ExpectEQ(th.Convert(1.0, THU_WEEKDAYS,	THU_HOURS), HOURSINDAY);
		ExpectEQ(th.Convert(1.0, THU_WEEKS,		THU_WEEKDAYS), DAYSINWEEK);
		ExpectEQ(th.Convert(1.0, THU_WEEKS,		THU_DAYS), 7.0);

		// -----------------------------------------------------------------------

		ExpectEQ(th.Convert(1.0, THU_HOURS,		THU_MINS), 60.0);
		ExpectEQ(th.Convert(1.0, THU_DAYS,		THU_MINS), (60.0 * HOURSINDAY));
		ExpectEQ(th.Convert(1.0, THU_WEEKDAYS,	THU_MINS), (60.0 * HOURSINDAY));
		ExpectEQ(th.Convert(1.0, THU_WEEKS,		THU_MINS), (60.0 * HOURSINDAY * DAYSINWEEK));

		// -----------------------------------------------------------------------

		ExpectEQ(th.Convert(1.0, THU_MINS,		THU_HOURS)	, (1 / 60.0));
		ExpectEQ(th.Convert(1.0, THU_MINS,		THU_DAYS)	, (1 / (60.0 * HOURSINDAY)));
		ExpectEQ(th.Convert(1.0, THU_MINS,		THU_WEEKDAYS), (1 / (60.0 * HOURSINDAY)));
		ExpectEQ(th.Convert(1.0, THU_MINS,		THU_WEEKS)	, (1 / (60.0 * HOURSINDAY * DAYSINWEEK)));
	}
		
	// -----------------------------------------------------------------------

	{
		// Temporarily override default statically defined internal values
		const double DAYSINWEEK = 7.0;
		const double HOURSINDAY = 24.0;

		const CTimeHelper th(CWorkingWeek(0, HOURSINDAY));

		// -----------------------------------------------------------------------

		ExpectEQ(th.Convert(1.0, THU_DAYS,		THU_HOURS),		HOURSINDAY);
		ExpectEQ(th.Convert(1.0, THU_WEEKDAYS,	THU_HOURS),		HOURSINDAY);
		ExpectEQ(th.Convert(1.0, THU_WEEKS,		THU_WEEKDAYS),	DAYSINWEEK);
		ExpectEQ(th.Convert(1.0, THU_WEEKS,		THU_DAYS),		DAYSINWEEK);

		// -----------------------------------------------------------------------

		ExpectEQ(th.Convert(1.0, THU_HOURS,		THU_MINS),		60.0);
		ExpectEQ(th.Convert(1.0, THU_DAYS,		THU_MINS),		(60.0 * HOURSINDAY));
		ExpectEQ(th.Convert(1.0, THU_WEEKDAYS,	THU_MINS),		(60.0 * HOURSINDAY));
		ExpectEQ(th.Convert(1.0, THU_WEEKS,		THU_MINS),		(60.0 * HOURSINDAY * DAYSINWEEK));

		// -----------------------------------------------------------------------

		ExpectEQ(th.Convert(1.0, THU_MINS,		THU_HOURS),		(1 / 60.0));
		ExpectEQ(th.Convert(1.0, THU_MINS,		THU_DAYS),		(1 / (60.0 * HOURSINDAY)));
		ExpectEQ(th.Convert(1.0, THU_MINS,		THU_WEEKDAYS),	(1 / (60.0 * HOURSINDAY)));
		ExpectEQ(th.Convert(1.0, THU_MINS,		THU_WEEKS),		(1 / (60.0 * HOURSINDAY * DAYSINWEEK)));
	}
		
	// -----------------------------------------------------------------------

	{
		// Permanently override default statically defined internal values
		const double DAYSINWEEK = 6.0;
		const double HOURSINDAY = 16.0;

		const CTimeHelper th(CWorkingWeek(DHW_SUNDAY, HOURSINDAY));

		// -----------------------------------------------------------------------

		ExpectEQ(th.Convert(1.0, THU_DAYS,		THU_HOURS),		HOURSINDAY);
		ExpectEQ(th.Convert(1.0, THU_WEEKDAYS,	THU_HOURS),		HOURSINDAY);
		ExpectEQ(th.Convert(1.0, THU_WEEKS,		THU_WEEKDAYS),	DAYSINWEEK);
		ExpectEQ(th.Convert(1.0, THU_WEEKS,		THU_DAYS),		7.0);

		// -----------------------------------------------------------------------

		ExpectEQ(th.Convert(1.0, THU_HOURS,		THU_MINS),		60.0);
		ExpectEQ(th.Convert(1.0, THU_DAYS,		THU_MINS),		(60.0 * HOURSINDAY));
		ExpectEQ(th.Convert(1.0, THU_WEEKDAYS,	THU_MINS),		(60.0 * HOURSINDAY));
		ExpectEQ(th.Convert(1.0, THU_WEEKS,		THU_MINS),		(60.0 * HOURSINDAY * DAYSINWEEK));

		// -----------------------------------------------------------------------

		ExpectEQ(th.Convert(1.0, THU_MINS,		THU_HOURS)	,	(1 / 60.0));
		ExpectEQ(th.Convert(1.0, THU_MINS,		THU_DAYS)	,	(1 / (60.0 * HOURSINDAY)));
		ExpectEQ(th.Convert(1.0, THU_MINS,		THU_WEEKDAYS),	(1 / (60.0 * HOURSINDAY)));
		ExpectEQ(th.Convert(1.0, THU_MINS,		THU_WEEKS)	,	(1 / (60.0 * HOURSINDAY * DAYSINWEEK)));
	}
		
	// -----------------------------------------------------------------------
	
	EndTest();
}
