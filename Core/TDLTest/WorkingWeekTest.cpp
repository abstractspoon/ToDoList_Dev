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
}

void CWorkingWeekTest::TestCalculateDuration()
{

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
	// Add hours to positive date
	{
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

		dHours = 6.0;
		day.AddDurationInHours(date, dHours);

		ExpectEQ(date.m_dt, 44002.25);
		ExpectEQ(dHours, 0.0);
	}

	// Subtract hours from positive date
	{
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

	// Add hours to negative date
	{
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

		dHours = 6.0;
		day.AddDurationInHours(date, dHours);

		ExpectEQ(date.m_dt, -43997.75);
		ExpectEQ(dHours, 0.0);
	}

	// Subtract hours from negative date
	{
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
}

