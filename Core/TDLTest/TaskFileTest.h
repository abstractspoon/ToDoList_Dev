// TaskFileTest.h: interface for the CTaskFileTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKFILETEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_)
#define AFX_TASKFILETEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDLTestBase.h"

#include "..\todolist\taskfile.h"

//////////////////////////////////////////////////////////////////////

const int NUM_TESTLEVELS = 4;
const int MAX_TESTLEVELS = 5;

//////////////////////////////////////////////////////////////////////

class CTaskFileTest : public CTDLTestBase  
{
public:
	CTaskFileTest(const CTestUtils& utils);
	virtual ~CTaskFileTest();

	TESTRESULT Run();

	void PopulateHierarchy(CTaskFile& tasks, int nNumLevels);
	void PopulateFlatList(CTaskFile& tasks, int nNumTasks);
	
	void TestHierarchyConstructionPerformance();
	void TestFlatListConstructionPerformance();

protected:
	static void PopulateTaskAttributes(CTaskFile& tasks, HTASKITEM hTask);

};

#endif // !defined(AFX_TASKFILETEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_)
