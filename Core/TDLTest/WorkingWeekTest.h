// WorkingWeekTest.h: interface for the CWorkingWeekTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKINGWEEKTEST_H__6A11464D_D4DA_4ABA_8FDB_310C9A5252E5__INCLUDED_)
#define AFX_WORKINGWEEKTEST_H__6A11464D_D4DA_4ABA_8FDB_310C9A5252E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDLTestBase.h"

//////////////////////////////////////////////////////////////////////

class CWorkingWeekTest : public CTDLTestBase  
{
public:
	CWorkingWeekTest(const CTestUtils& utils);
	virtual ~CWorkingWeekTest();

	TESTRESULT Run();

protected:
	void TestAddDuration();
	void TestCalculateDuration();
};

//////////////////////////////////////////////////////////////////////

class CWorkingDayTest : public CTDLTestBase
{
public:
	CWorkingDayTest(const CTestUtils& utils);
	virtual ~CWorkingDayTest();

	TESTRESULT Run();

protected:
	void TestAddDurationInHours();
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_WORKINGWEEKTEST_H__6A11464D_D4DA_4ABA_8FDB_310C9A5252E5__INCLUDED_)
