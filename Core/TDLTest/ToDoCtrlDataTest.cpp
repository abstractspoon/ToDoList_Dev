// ToDoCtrlDataTest.cpp: implementation of the CToDoCtrlDataTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToDoCtrlDataTest.h"
#include "TaskFileTest.h"

#include "..\todolist\todoctrldata.h"
#include "..\todolist\todoctrldatautils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToDoCtrlDataTest::CToDoCtrlDataTest(const CTestUtils& utils) : CTDLTestBase(utils)
{
	// Initialise styles and custom attributes
	m_aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = TRUE;
//	m_aStyles[TDCS_USEEARLIESTDUEDATE] = TRUE;
	m_aStyles[TDCS_USELATESTDUEDATE] = TRUE;
//	m_aStyles[TDCS_USEEARLIESTSTARTDATE] = TRUE;
	m_aStyles[TDCS_USELATESTSTARTDATE] = TRUE;
	m_aStyles[TDCS_USELATESTLASTMODIFIED] = TRUE;
	m_aStyles[TDCS_USEHIGHESTPRIORITY] = TRUE;
	m_aStyles[TDCS_USEHIGHESTRISK] = TRUE;
	m_aStyles[TDCS_AVERAGEPERCENTSUBCOMPLETION] = TRUE;
	m_aStyles[TDCS_INCLUDEDONEINAVERAGECALC] = TRUE;
	m_aStyles[TDCS_DONEHAVELOWESTPRIORITY] = TRUE;
	m_aStyles[TDCS_INCLUDEDONEINRISKCALC] = TRUE;
	m_aStyles[TDCS_INCLUDEDONEINPRIORITYCALC] = TRUE;
	m_aStyles[TDCS_DONEHAVELOWESTRISK] = TRUE;
	m_aStyles[TDCS_DUEHAVEHIGHESTPRIORITY] = TRUE;
}

CToDoCtrlDataTest::~CToDoCtrlDataTest()
{

}

TESTRESULT CToDoCtrlDataTest::Run()
{
	ClearTotals();

	TestHierarchyDataModelPerformance();
	TestFlatListDataModelPerformance();

	return GetTotals();
}

void CToDoCtrlDataTest::TestHierarchyDataModelPerformance()
{
	if (!m_utils.HasCommandlineFlag('p'))
	{
		_tprintf(_T("Add '-p' to run CToDoCtrlDataTest::HierarchyDataModelCreationPerformance\n"));
		return;
	}

	BeginTest(_T("CToDoCtrlDataTest::HierarchyDataModelCreationPerformance"));

	for (int nNumLevels = 2; nNumLevels <= NUM_TESTLEVELS; nNumLevels++)
	{
		CTaskFile tasks;
		CTaskFileTest(m_utils).PopulateHierarchy(tasks, nNumLevels);

		CToDoCtrlData data(m_aStyles, m_aCustomAttribDefs);

		TestDataModelCreationPerformance(tasks, data, _T("nested"));
		TestDataModelCalculationPerformance(data, _T("nested"));
		TestDataModelFormattingPerformance(data, _T("nested"));
	}

	EndTest();
}

void CToDoCtrlDataTest::TestFlatListDataModelPerformance()
{
	if (!m_utils.HasCommandlineFlag('p'))
	{
		_tprintf(_T("Add '-p' to run CToDoCtrlDataTest::FlatListDataModelPerformance\n"));
		return;
	}

	// Initialise styles and custom attributes
	// TODO

	BeginTest(_T("CToDoCtrlDataTest::FlatListDataModelPerformance"));

	for (int nNumLevels = 2, nNumTasks = 10; nNumLevels <= NUM_TESTLEVELS; nNumLevels++)
	{
		// Numbers to match hierarchical test
		nNumTasks += (int)pow(10, nNumLevels);

		CTaskFile tasks;
		CTaskFileTest(m_utils).PopulateFlatList(tasks, nNumTasks);

		CToDoCtrlData data(m_aStyles, m_aCustomAttribDefs);

		TestDataModelCreationPerformance(tasks, data, _T("flat"));
		TestDataModelCalculationPerformance(data, _T("flat"));
		TestDataModelFormattingPerformance(data, _T("flat"));
	}

	EndTest();
}

void CToDoCtrlDataTest::TestDataModelCreationPerformance(const CTaskFile& tasks, CToDoCtrlData& data, LPCTSTR szTaskType)
{
	ASSERT(m_utils.HasCommandlineFlag('p'));

	DWORD dwTickStart = GetTickCount();

	data.BuildDataModel(tasks);

	DWORD dwDuration = (GetTickCount() - dwTickStart);
	_tprintf(_T("Test took %ld ms to build data model with %d %s tasks\n"), dwDuration, data.GetTaskCount(), szTaskType);
}

void CToDoCtrlDataTest::TestDataModelCalculationPerformance(const CToDoCtrlData& data, LPCTSTR szTaskType)
{
	ASSERT(m_utils.HasCommandlineFlag('p'));

	DWORD dwTickStart = GetTickCount();

	CTDCTaskCalculator calc(data);
	DWORD dwMaxTaskID = (data.GetTaskCount() + 1);

	for (DWORD dwTaskID = 1; dwTaskID < dwMaxTaskID; dwTaskID++)
	{
		calc.GetTaskSubtaskCompletion(dwTaskID);
		calc.GetTaskTimeEstimate(dwTaskID, TDCU_DAYS);
		calc.GetTaskTimeSpent(dwTaskID, TDCU_DAYS);
		calc.GetTaskPercentDone(dwTaskID);
		calc.GetTaskCost(dwTaskID);
		calc.GetTaskPriority(dwTaskID);
		calc.GetTaskRisk(dwTaskID);
		calc.GetTaskDueDate(dwTaskID);
		calc.GetTaskStartDate(dwTaskID);
		calc.GetTaskLastModifiedDate(dwTaskID);
		calc.GetTaskSubtaskCompletion(dwTaskID);

		calc.IsTaskOverDue(dwTaskID);
		calc.IsTaskDueToday(dwTaskID);
		calc.IsTaskRecentlyModified(dwTaskID);

		TDC_UNITS nUnits;
		calc.GetTaskRemainingTime(dwTaskID, nUnits);

		int nSubtasksTotal, nSubtasksDone;
		calc.GetTaskSubtaskTotals(dwTaskID, nSubtasksTotal, nSubtasksDone);
	}

	DWORD dwDuration = (GetTickCount() - dwTickStart);
	_tprintf(_T("Test took %ld ms to perform calculations on %d %s tasks\n"), dwDuration, data.GetTaskCount(), szTaskType);
}

void CToDoCtrlDataTest::TestDataModelFormattingPerformance(const CToDoCtrlData& data, LPCTSTR szTaskType)
{
	ASSERT(m_utils.HasCommandlineFlag('p'));

	DWORD dwTickStart = GetTickCount();

	CTDCTaskFormatter formatter(data);
	DWORD dwMaxTaskID = (data.GetTaskCount() + 1);

	for (DWORD dwTaskID = 1; dwTaskID < dwMaxTaskID; dwTaskID++)
	{
		formatter.GetTaskSubtaskCompletion(dwTaskID);
		formatter.GetTaskPath(dwTaskID);
		formatter.GetTaskPosition(dwTaskID);
		formatter.GetTaskTimeEstimate(dwTaskID);
		formatter.GetTaskTimeSpent(dwTaskID);
		formatter.GetTaskTimeRemaining(dwTaskID);
		formatter.GetTaskPercentDone(dwTaskID);
		formatter.GetTaskCost(dwTaskID);
		formatter.GetTaskPriority(dwTaskID);
		formatter.GetTaskRisk(dwTaskID);
		formatter.GetTaskRecentlyModified(dwTaskID);
		formatter.GetTaskDoneDate(dwTaskID);
		formatter.GetTaskDueDate(dwTaskID);
		formatter.GetTaskStartDate(dwTaskID);
		formatter.GetTaskCreationDate(dwTaskID);
		formatter.GetTaskLastModDate(dwTaskID);

// 		formatter.GetTaskAllocTo(dwTaskID);
// 		formatter.GetTaskCategories(dwTaskID);
// 		formatter.GetTaskTags(dwTaskID);
//		formatter.GetTaskCommentSize(dwTaskID);
// 		formatter.GetTaskStatus(dwTaskID, CString& sCompletionStatus);
//		formatter.GetTaskRecurrence(dwTaskID);
//		formatter.GetTaskCustomAttributeData(dwTaskID, TDCCUSTOMATTRIBUTEDEFINITION& attribDef);
	}

	DWORD dwDuration = (GetTickCount() - dwTickStart);
	_tprintf(_T("Test took %ld ms to format attributes for %d %s tasks\n"), dwDuration, data.GetTaskCount(), szTaskType);
}


