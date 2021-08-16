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
#include "..\todolist\tdcenum.h"

//////////////////////////////////////////////////////////////////////

struct TDCAUTOLISTDATA;

//////////////////////////////////////////////////////////////////////

class CTaskFileTest : public CTDLTestBase  
{
public:
	CTaskFileTest(const CTestUtils& utils);
	virtual ~CTaskFileTest();

	TESTRESULT Run();

	// Helpers accessible to other tests
	void PopulateHierarchy(CTaskFile& tasks, int nNumLevels, const CTDCAttributeMap& mapAtrrib = TDCA_ALL) const;
	void PopulateFlatList(CTaskFile& tasks, int nNumTasks, const CTDCAttributeMap& mapAtrrib = TDCA_ALL) const;
	
	static int NUM_TESTLEVELS;
	static int MAX_TESTLEVELS;

protected:
	void TestHierarchyConstructionPerformance();
	void TestFlatListConstructionPerformance();

	static void TestSaveTasklist(CTaskFile& tasks, LPCTSTR szFilePath, LPCTSTR szType);
	static void TestLoadTasklist(LPCTSTR szFilePath, LPCTSTR szType);

	static void Add10TasksToHierarchy(CTaskFile& tasks, HTASKITEM hParentTask, int nLevel, int nNumLevels, const CTDCAttributeMap& mapAtrrib);
	static void PopulateNumericTaskAttributes(CTaskFile& tasks, HTASKITEM hTask, const CTDCAttributeMap& mapAtrrib);
	static void PopulateStringTaskAttributes(CTaskFile& tasks, HTASKITEM hTask, int nNumMultiAttrib, const CTDCAttributeMap& mapAtrrib);
	static void PopulateArrayWithRandomStrings(CStringArray& aValues, int nCount, LPCTSTR szFormat);
	static void AddGlobalsToTasklist(CTaskFile& tasks, const CTDCAttributeMap& mapAtrrib);
};

#endif // !defined(AFX_TASKFILETEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_)
