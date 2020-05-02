// FileMiscTest.h: interface for the CFileMiscTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEMISCTEST_H__96F4BFF1_37C7_4AB6_95D6_FEE032D025DA__INCLUDED_)
#define AFX_FILEMISCTEST_H__96F4BFF1_37C7_4AB6_95D6_FEE032D025DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDLTestBase.h"

class CFileMiscTest : public CTDLTestBase  
{
public:
	CFileMiscTest(const CTestUtils& utils);
	virtual ~CFileMiscTest();
	
	TESTRESULT Run();

protected:
	void TestGetVirtualStorePath();
	void TestGetExtension();
	void TestSplitPath();
};

#endif // !defined(AFX_FILEMISCTEST_H__96F4BFF1_37C7_4AB6_95D6_FEE032D025DA__INCLUDED_)
