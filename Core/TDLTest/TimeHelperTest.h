// DateHelperTest.h: interface for the CTimeHelperTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEHELPERTEST_H__6A11464D_D4DA_4ABA_8FDB_310C9A5252E5__INCLUDED_)
#define AFX_TIMEHELPERTEST_H__6A11464D_D4DA_4ABA_8FDB_310C9A5252E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDLTestBase.h"

class CTimeHelperTest : public CTDLTestBase  
{
public:
	CTimeHelperTest(const CTestUtils& utils);
	virtual ~CTimeHelperTest();

	TESTRESULT Run();

protected:
	void TestGetTime();
};

#endif // !defined(AFX_TIMEHELPERTEST_H__6A11464D_D4DA_4ABA_8FDB_310C9A5252E5__INCLUDED_)
