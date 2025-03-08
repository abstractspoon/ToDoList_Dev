// FileMiscTest.h: interface for the CToDoCtrlMgrTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TODOCTRLMGRTEST_H__96F4BFF1_37C7_4AB6_95D6_FEE032D025DA__INCLUDED_)
#define AFX_TODOCTRLMGRTEST_H__96F4BFF1_37C7_4AB6_95D6_FEE032D025DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDLTestBase.h"

class CToDoCtrlMgrTest : public CTDLTestBase  
{
public:
	CToDoCtrlMgrTest(const CTestUtils& utils);
	virtual ~CToDoCtrlMgrTest();
	
	TESTRESULT Run();

protected:
	void TestCreateBackup();
};

#endif // !defined(AFX_TODOCTRLMGRTEST_H__96F4BFF1_37C7_4AB6_95D6_FEE032D025DA__INCLUDED_)
