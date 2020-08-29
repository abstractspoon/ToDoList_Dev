// TaskFileTest.h: interface for the CTaskFileTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCDEPENDENCYTEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_)
#define AFX_TDCDEPENDENCYTEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDLTestBase.h"

class CTDCDEPENDENCYTest : public CTDLTestBase
{
public:
	CTDCDEPENDENCYTest(const CTestUtils& utils);
	virtual ~CTDCDEPENDENCYTest();

	TESTRESULT Run();

protected:
	void TestTDCDEPENDENCY();
	void TestTDCDependencyArray();
};

#endif // !defined(AFX_TDCDEPENDENCYTEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_)
