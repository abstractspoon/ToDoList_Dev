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

struct TDCAUTOLISTDATA;

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

	static int NUM_TESTLEVELS;
	static int MAX_TESTLEVELS;

protected:
	static void Add10TasksToHierarchy(CTaskFile& tasks, HTASKITEM hParentTask, int nLevel, int nNumLevels);
	static void PopulateNumericTaskAttributes(CTaskFile& tasks, HTASKITEM hTask);
	static void PopulateStringTaskAttributes(CTaskFile& tasks, HTASKITEM hTask, int nNumMultiAttrib);
	static void PopulateArrayWithRandomStrings(CStringArray& aValues, int nCount, LPCTSTR szFormat);
	static void AddGlobalsToTasklist(CTaskFile& tasks);
};

#endif // !defined(AFX_TASKFILETEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_)
