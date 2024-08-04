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
	void TestCalcs(const CToDoCtrlData& data, BOOL bEnableRefs);

	void TestGetTaskStartDate(const CToDoCtrlData& data, BOOL bEnableRefs);
	void TestGetTaskDueDate(const CToDoCtrlData& data, BOOL bEnableRefs);
	void TestGetTaskLastModifiedDateAndUser(const CToDoCtrlData& data, BOOL bEnableRefs);
	void TestGetTaskPriority(const CToDoCtrlData& data, BOOL bEnableRefs);
	void TestGetTaskRisk(const CToDoCtrlData& data, BOOL bEnableRefs);
	void TestGetTaskPercentDone(const CToDoCtrlData& data, BOOL bEnableRefs);
	void TestGetTaskCost(const CToDoCtrlData& data, BOOL bEnableRefs);
	void TestGetTaskTimeEstimate(const CToDoCtrlData& data, BOOL bEnableRefs);
	void TestGetTaskTimeSpent(const CToDoCtrlData& data, BOOL bEnableRefs);
	void TestGetTaskTimeRemaining(const CToDoCtrlData& data, BOOL bEnableRefs);
	void TestGetTaskFlag(const CToDoCtrlData& data, BOOL bEnableRefs);
	void TestGetTaskLock(const CToDoCtrlData& data, BOOL bEnableRefs);
	//void Test(const CToDoCtrlData& data, BOOL bEnableRefs);

	void PopulateData(CToDoCtrlData& data) const;
	void InitialiseStyles(BOOL bEnableRefs);
};

#endif // !defined(AFX_TDCTASKCALCULATORTEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_)

