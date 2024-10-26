// ToDoCtrlDataTest.h: interface for the CToDoCtrlDataTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCTASKCALCULATORTEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_)
#define AFX_TDCTASKCALCULATORTEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "TDLTestBase.h"

#include "..\todolist\tdcstruct.h"

//////////////////////////////////////////////////////////////////////

class CToDoCtrlData;

//////////////////////////////////////////////////////////////////////

class CTDCTaskCalculatorTest : public CTDLTestBase
{
public:
	CTDCTaskCalculatorTest(const CTestUtils& utils);
	virtual ~CTDCTaskCalculatorTest();
	
	TESTRESULT Run();

protected:
	CTDCStyleMap m_aStyles;
	CTDCCustomAttribDefinitionArray m_aCustomAttribDefs;

protected:
	void TestCalcs(const CToDoCtrlData& data, BOOL bIncludeRefs);

	void TestGetTaskStartDate(const CToDoCtrlData& data, BOOL bIncludeRefs);
	void TestGetTaskDueDate(const CToDoCtrlData& data, BOOL bIncludeRefs);
	void TestGetTaskLastModifiedDateAndUser(const CToDoCtrlData& data, BOOL bIncludeRefs);
	void TestGetTaskPriority(const CToDoCtrlData& data, BOOL bIncludeRefs);
	void TestGetTaskRisk(const CToDoCtrlData& data, BOOL bIncludeRefs);
	void TestGetTaskPercentDone(const CToDoCtrlData& data, BOOL bIncludeRefs);
	void TestGetTaskCost(const CToDoCtrlData& data, BOOL bIncludeRefs);
	void TestGetTaskTimeEstimate(const CToDoCtrlData& data, BOOL bIncludeRefs);
	void TestGetTaskTimeSpent(const CToDoCtrlData& data, BOOL bIncludeRefs);
	void TestGetTaskTimeRemaining(const CToDoCtrlData& data, BOOL bIncludeRefs);
	void TestGetTaskFlag(const CToDoCtrlData& data, BOOL bIncludeRefs);
	void TestGetTaskLock(const CToDoCtrlData& data, BOOL bIncludeRefs);
	void TestGetTaskCustomAttributeData(const CToDoCtrlData& data, BOOL bIncludeRefs);

	void PopulateDataModel(CToDoCtrlData& data) const;
	void InitialiseStyles(BOOL bIncludeRefs);
};

#endif // !defined(AFX_TDCTASKCALCULATORTEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_)

