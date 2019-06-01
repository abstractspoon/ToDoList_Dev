// DateHelperTest.h: interface for the CDateHelperTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATEHELPERTEST_H__6A11464D_D4DA_4ABA_8FDB_310C9A5252E5__INCLUDED_)
#define AFX_DATEHELPERTEST_H__6A11464D_D4DA_4ABA_8FDB_310C9A5252E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDLTestBase.h"

class CDateHelperTest : public CTDLTestBase  
{
public:
	CDateHelperTest(const CTestUtils& utils);
	virtual ~CDateHelperTest();

	void Run();

protected:
	void TestRelativeDateDecoding();
};

#endif // !defined(AFX_DATEHELPERTEST_H__6A11464D_D4DA_4ABA_8FDB_310C9A5252E5__INCLUDED_)
