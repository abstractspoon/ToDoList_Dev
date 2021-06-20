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

	{
		CWorkingWeek week(0,		// no weekend (hard to test)
						  8,		// 8 hour day
						  9,		// 9 am - 6pm (0.375 - 0.75)
						  12, 13);	// 12-1pm lunch

		COleDateTime date(44000.0);

		ExpectEQ(week.AddDurationInMinutes(date, 30), CWorkingDay::GetDateAtTimeInHours(44000.0, 9.5));
		ExpectEQ(week.AddDurationInMinutes(date, -30), CWorkingDay::GetDateAtTimeInHours(43999.0, 17.5));

		ExpectEQ(week.AddDurationInHours(date, 9), CWorkingDay::GetDateAtTimeInHours(44001.0, 10));
		ExpectEQ(week.AddDurationInHours(date, -9), CWorkingDay::GetDateAtTimeInHours(43998.0, 17));

		ExpectEQ(week.AddDurationInDays(date, 9).m_dt, 44008.75);   // end of 9th day
		ExpectEQ(week.AddDurationInDays(date, -9).m_dt, 43991.375); // start of 9th previous day

		ExpectEQ(week.AddDurationInWeeks(date, 2).m_dt, 44013.75);  // end of 14th day
		ExpectEQ(week.AddDurationInWeeks(date, -2).m_dt, 43986.375);// start of 14th previous day
	}

	// -----------------------------------------------------------------------

	{
		CWorkingWeek week(0,		// no weekend (hard to test)
						  8,		// 8 hour day
						  9,		// 9 am - 6pm (0.375 - 0.75)
						  12, 13);	// 12-1pm lunch

		COleDateTime date(-44000.0);

		ExpectEQ(week.AddDurationInMinutes(date, 30).m_dt, CWorkingDay::GetDateAtTimeInHours(-44000.0, 9.5));
		ExpectEQ(week.AddDurationInMinutes(date, -30).m_dt, CWorkingDay::GetDateAtTimeInHours(-44001.0, 17.5));

		ASSERT(date.m_dt == -44000.0);
		ExpectEQ(week.AddDurationInHours(date, 9).m_dt, CWorkingDay::GetDateAtTimeInHours(-43999.0, 10));
		ExpectEQ(week.AddDurationInHours(date, -9).m_dt, CWorkingDay::GetDateAtTimeInHours(-44002.0, 17));

		ASSERT(date.m_dt == -44000.0);
		ExpectEQ(week.AddDurationInDays(date, 9).m_dt, -43992.75);   // end of 9th day
		ExpectEQ(week.AddDurationInDays(date, -9).m_dt, -44009.375); // start of 9th previous day

		ASSERT(date.m_dt == -44000.0);
		ExpectEQ(week.AddDurationInWeeks(date, 2).m_dt, -43987.75);  // end of 14th day
		ExpectEQ(week.AddDurationInWeeks(date, -2).m_dt, -44014.375);// start of 14th previous day
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

	{
		// Add hours to positive date
		CWorkingDay day(8,			// 8 hour day
						9,			// 9 am - 6pm (0.375 - 0.75)
						12, 13);	// 12-1pm lunch
		COleDateTime date(44000.0);

		double dHours = 3.0;
		day.AddDurationInHours(date, dHours);

		ExpectEQ(date.m_dt, 44000.5);
		ExpectEQ(dHours, 0.0);

		dHours = 3.0;
		day.AddDurationInHours(date, dHours);

		ExpectEQ(date, CWorkingDay::GetDateAtTimeInHours(44000.0, 16));
		ExpectEQ(dHours, 0.0);

		dHours = 60.0;
		day.AddDurationInHours(date, dHours);

		ExpectEQ(date.m_dt, 44000.75); // end of day
		ExpectEQ(dHours, 58.0, 1e-9);
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

