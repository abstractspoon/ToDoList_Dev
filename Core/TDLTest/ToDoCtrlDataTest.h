// ToDoCtrlDataTest.h: interface for the CToDoCtrlDataTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TODOCTRLDATATEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_)
#define AFX_TODOCTRLDATATEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDLTestBase.h"

#include "..\todolist\taskfile.h"
#include "..\todolist\tdcstruct.h"

class CToDoCtrlData;
class CTaskFile;

class CToDoCtrlDataTest : public CTDLTestBase  
{
public:
	CToDoCtrlDataTest(const CTestUtils& utils);
	virtual ~CToDoCtrlDataTest();
	
	TESTRESULT Run();

protected:
	CTDCStyleMap m_aStyles;
	CTDCCustomAttribDefinitionArray m_aCustomAttribDefs;

protected:
	void BeginTest(LPCTSTR szFunction, BOOL bWithAttributes);

	void TestHierarchyDataModelPerformance();
	void TestFlatListDataModelPerformance();
	void TestDataModelCreationPerformance(const CTaskFile& tasks, CToDoCtrlData& data, LPCTSTR szTaskType);
	void TestDataModelCalculationPerformance(const CToDoCtrlData& data, LPCTSTR szTaskType);
	void TestDataModelFormattingPerformance(const CToDoCtrlData& data, LPCTSTR szTaskType);
	void TestDataModelExporterPerformance(const CToDoCtrlData& data, LPCTSTR szTaskType);
	void TestDataModelGetTaskPerformance(const CToDoCtrlData& data, LPCTSTR szTaskType);
	void TestDataModelGetTaskPositionPerformance(const CToDoCtrlData& data, LPCTSTR szTaskType);

};

#endif // !defined(AFX_TODOCTRLDATATEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_)
