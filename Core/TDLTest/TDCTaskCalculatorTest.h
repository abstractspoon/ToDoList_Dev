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
	void TestGetTaskStartDate(const CToDoCtrlData& data);
	void TestGetTaskDueDate(const CToDoCtrlData& data);
	void TestGetTaskLastModifiedDateAndUser(const CToDoCtrlData& data);
	void TestGetTaskPriority(const CToDoCtrlData& data);
	void TestGetTaskRisk(const CToDoCtrlData& data);
	void TestGetTaskPercentDone(const CToDoCtrlData& data);
	void TestGetTaskCost(const CToDoCtrlData& data);
	void TestGetTaskTimeEstimate(const CToDoCtrlData& data);
	void TestGetTaskTimeSpent(const CToDoCtrlData& data);
	void TestGetTaskTimeRemaining(const CToDoCtrlData& data);
	void TestGetTaskFlag(const CToDoCtrlData& data);
	void TestGetTaskLock(const CToDoCtrlData& data);

	void PopulateData(CToDoCtrlData& data) const;
	//void Test(CToDoCtrlData& data);
};

#endif // !defined(AFX_TDCTASKCALCULATORTEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_)

