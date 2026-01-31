// TDCFileBackupTest.h: interface for the CTDCFileBackupTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCFILEBACKUPTEST_H__96F4BFF1_37C7_4AB6_95D6_FEE032D025DA__INCLUDED_)
#define AFX_TDCFILEBACKUPTEST_H__96F4BFF1_37C7_4AB6_95D6_FEE032D025DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDLTestBase.h"

class CTDCFileBackupTest : public CTDLTestBase  
{
public:
	CTDCFileBackupTest(const CTestUtils& utils);
	virtual ~CTDCFileBackupTest();
	
	TESTRESULT Run();

protected:
	void TestCreateBackup();
};

#endif // !defined(AFX_TDCFILEBACKUPTEST_H__96F4BFF1_37C7_4AB6_95D6_FEE032D025DA__INCLUDED_)
