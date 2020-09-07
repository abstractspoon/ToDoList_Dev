// WorkingWeekTest.cpp: implementation of the CWorkingWeekTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "WorkingWeekTest.h"

#include "..\shared\WorkingWeek.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkingWeekTest::CWorkingWeekTest(const CTestUtils& utils) : CTDLTestBase(utils)
{

}

CWorkingWeekTest::~CWorkingWeekTest()
{

}

TESTRESULT CWorkingWeekTest::Run()
{
	ClearTotals();

	TestAddDuration();
	TestCalculateDuration();

	return GetTotals();
}

void CWorkingWeekTest::TestAddDuration()
{
	BeginTest(_T("CWorkingWeekTest::AddDuration"));

	// -----------------------------------------------------------------------

	{
		CTwentyFourSevenWeek week;
		COleDateTime date(44000.125);

		ExpectEQ(week.AddDurationInMinutes(date, 30).m_dt, 44000.125 + (30.0 / (60 * 24)));
		ExpectEQ(week.AddDurationInMinutes(date, -30).m_dt, 44000.125 - (30.0 / (60 * 24)));

		ExpectEQ(week.AddDurationInHours(date, 9).m_dt, 44000.5);
		ExpectEQ(week.AddDurationInHours(date, -9).m_dt, 43999.75);

		ExpectEQ(week.AddDurationInDays(date, 9).m_dt, 44009.125);
		ExpectEQ(week.AddDurationInDays(date, -9).m_dt, 43991.125);

		ExpectEQ(week.AddDurationInWeeks(date, 2).m_dt, 44014.125);
		ExpectEQ(week.AddDurationInWeeks(date, -2).m_dt, 43986.125);
	}

	// -----------------------------------------------------------------------

	EndTest();
}

void CWorkingWeekTest::TestCalculateDuration()
{
	BeginTest(_T("CWorkingWeekTest::CalculateDuration"));

	// -----------------------------------------------------------------------


	// -----------------------------------------------------------------------

	EndTest();
}

//////////////////////////////////////////////////////////////////////

CWorkingDayTest::CWorkingDayTest(const CTestUtils& utils) : CTDLTestBase(utils)
{

}

CWorkingDayTest::~CWorkingDayTest()
{

}

TESTRESULT CWorkingDayTest::Run()
{
	ClearTotals();

	TestAddDurationInHours();

	return GetTotals();
}

void CWorkingDayTest::TestAddDurationInHours()
{
	BeginTest(_T("CWorkingDayTest::AddDurationInHours"));

	// -----------------------------------------------------------------------

	{
		// Add hours to positive date
		CWorkingDay day(24);
		COleDateTime date(44000.0);

		double dHours = 24;
		day.AddDurationInHours(date, dHours);

		ExpectEQ(date.m_dt, 44001.0);
		ExpectEQ(dHours, 0.0);

		dHours = 60.0;
		day.AddDurationInHours(date, dHours);

		ExpectEQ(date.m_dt, 44002.0);
		ExpectEQ(dHours, 36.0);

		dHours = 3.0;
		day.AddDurationInHours(date, dHours);

		ExpectEQ(date.m_dt, 44002.125);
		ExpectEQ(dHours, 0.0);
	}

	// -----------------------------------------------------------------------

	{
		// Subtract hours from positive date 
		CWorkingDay day(24);
		COleDateTime date(44000.0);

		double dHours = -24;
		day.AddDurationInHours(date, dHours);

		ExpectEQ(date.m_dt, 43999.0);
		ExpectEQ(dHours, 0.0);

		dHours = -51.0;
		day.AddDurationInHours(date, dHours);

		ExpectEQ(date.m_dt, 43998.0);
		ExpectEQ(dHours, -27.0);

		dHours = -9.0;
		day.AddDurationInHours(date, dHours);

		ExpectEQ(date.m_dt, 43997.625);
		ExpectEQ(dHours, 0.0);
	}

	// -----------------------------------------------------------------------

	{
		// Add hours to negative date 
		CWorkingDay day(24);
		COleDateTime date(-44000.0);

		double dHours = 24;
		day.AddDurationInHours(date, dHours);

		ExpectEQ(date.m_dt, -43999.0);
		ExpectEQ(dHours, 0.0);

		dHours = 60.0;
		day.AddDurationInHours(date, dHours);

		ExpectEQ(date.m_dt, -43998.0);
		ExpectEQ(dHours, 36.0);

		dHours = 3.0;
		day.AddDurationInHours(date, dHours);

		ExpectEQ(date.m_dt, -43998.125);
		ExpectEQ(dHours, 0.0);
	}

	// -----------------------------------------------------------------------

	{
		// Subtract hours from negative date 
		CWorkingDay day(24);
		COleDateTime date(-44000.0);

 		double dHours = -24;
		day.AddDurationInHours(date, dHours);

		ExpectEQ(date.m_dt, -44001.0);
		ExpectEQ(dHours, 0.0);

		dHours = -51.0;
		day.AddDurationInHours(date, dHours);

		ExpectEQ(date.m_dt, -44002.0);
		ExpectEQ(dHours, -27.0);

		dHours = -9.0;
		day.AddDurationInHours(date, dHours);

		ExpectEQ(date.m_dt, -44003.625);
		ExpectEQ(dHours, 0.0);
	}

	// -----------------------------------------------------------------------

	EndTest();
}

