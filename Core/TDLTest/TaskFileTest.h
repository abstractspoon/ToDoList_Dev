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
struct TODOITEM;

//////////////////////////////////////////////////////////////////////

class CTaskFileTest : public CTDLTestBase  
{
public:
	CTaskFileTest(const CTestUtils& utils);
	virtual ~CTaskFileTest();

	TESTRESULT Run();

	// Helpers accessible to other tests
	void PopulateHierarchy(CTaskFile& tasks, int nNumLevels, const CTDCAttributeMap& mapAttrib = TDCA_ALL) const;
	void PopulateFlatList(CTaskFile& tasks, int nNumTasks, const CTDCAttributeMap& mapAttrib = TDCA_ALL) const;

	BOOL WantPerformanceAttributes() const { return m_bWantPerformanceAttributes; }
	
	const int NUM_PERFTESTLEVELS;

protected:
	BOOL m_bWantPerformanceAttributes;

protected:
	void TestMergeTaskAttributesOverwriteAll();
	void TestMergeTaskAttributesExcludingEmptySrcValues();
	void TestMergeTaskAttributesPreservingNonEmptyDestValues();
	void TestMergeTaskAttributesPreservingNonEmptyDestValuesAndExcludingEmptySrcValues();
	void TestSetGetCustomComments();

	void TestHierarchyConstructionPerformance();
	void TestFlatListConstructionPerformance();

	// Merge related functions
	static void PrepareMergeTestTasks(CTaskFile& tasks, HTASKITEM& hSrcEmpty, HTASKITEM& hSrcFull, TODOITEM& tdiSrcFull, TODOITEM& tdiDestFull);
	static void PopulateMergeAttributeMaps(CTDCAttributeMap& mapMerge, CTDCAttributeMap& mapRest);

	// Performance related functions
	void BeginPerformanceTest(LPCTSTR szFunction);

	static void TestSaveTasklist(CTaskFile& tasks, LPCTSTR szFilePath, LPCTSTR szType);
	static void TestLoadTasklist(LPCTSTR szFilePath, LPCTSTR szType);

	static void Add10TasksToHierarchy(CTaskFile& tasks, HTASKITEM hParentTask, int nLevel, int nNumLevels, const CTDCAttributeMap& mapAttrib);
	static void PopulateNumericTaskAttributes(CTaskFile& tasks, HTASKITEM hTask, const CTDCAttributeMap& mapAttrib);
	static void PopulateStringTaskAttributes(CTaskFile& tasks, HTASKITEM hTask, int nNumMultiAttrib, const CTDCAttributeMap& mapAttrib);
	static void PopulateArrayWithRandomStrings(CStringArray& aValues, int nCount, LPCTSTR szFormat);
	static void AddGlobalsToTasklist(CTaskFile& tasks, const CTDCAttributeMap& mapAttrib);
	static BOOL HasAttribute(const CTDCAttributeMap& mapAttrib, TDC_ATTRIBUTE nAttribID);
	static void OutputElapsedTime(const CTaskFile& tasks, DWORD dwTickStart, LPCTSTR szOperation, LPCTSTR szType);

};

#endif // !defined(AFX_TASKFILETEST_H__21479206_861C_4C47_9837_75F9B9171F90__INCLUDED_)
