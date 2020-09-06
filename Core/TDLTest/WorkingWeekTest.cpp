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
}

