// MiscTest.h: interface for the CMiscTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MISCTEST_H__96F4BFF1_37C7_4AB6_95D6_FEE032D025DA__INCLUDED_)
#define AFX_MISCTEST_H__96F4BFF1_37C7_4AB6_95D6_FEE032D025DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDLTestBase.h"

class CMiscTest : public CTDLTestBase  
{
public:
	CMiscTest(const CTestUtils& utils);
	virtual ~CMiscTest();
	
	TESTRESULT Run();

protected:
	void TestGetFormattedLength();
	void TestFormatArray();
	void TestHasPrefix();
	void TestHasSuffix();

	void TestRegionalSettingsRetrievalPerformance();

protected:
	static BOOL ActualLengthMatchesCalculation(const CStringArray& aValues, LPCTSTR szSep, BOOL bIncEmpty = FALSE);
};

#endif // !defined(AFX_MISCTEST_H__96F4BFF1_37C7_4AB6_95D6_FEE032D025DA__INCLUDED_)
