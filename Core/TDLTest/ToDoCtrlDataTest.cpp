// ToDoCtrlDataTest.cpp: implementation of the CToDoCtrlDataTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToDoCtrlDataTest.h"
#include "TaskFileTest.h"

#include "..\todolist\todoctrldata.h"
#include "..\todolist\todoctrldatautils.h"
#include "..\todolist\tdcimagelist.h"
#include "..\todolist\todoctrlfind.h"
#include "..\todolist\tdcstruct.h"
#include "..\todolist\tdltasktreectrl.h"

#include "..\shared\treectrlhelper.h"

#include "..\interfaces\contentmgr.h"

#include <math.h>

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

void CToDoCtrlDataTest::BeginTest(LPCTSTR szFunction, BOOL bWithAttributes)
{
	CString sTest;
	
	sTest += _T("CToDoCtrlDataTest::");
	sTest += szFunction;
	
	if (bWithAttributes)
		sTest += _T("(WITH attributes)");
	else
		sTest += _T("(WITHOUT attributes)");
	
	CTDLTestBase::BeginTest(sTest);
}

void CToDoCtrlDataTest::TestHierarchyDataModelPerformance()
{
	if (!m_utils.HasCommandlineFlag('p'))
	{
		_tprintf(_T("Add '-p' to run CToDoCtrlDataTest::HierarchyDataModelCreationPerformance\n"));
		return;
	}

	CTaskFileTest tasksTest(m_utils);

	BeginTest(_T("HierarchyDataModelCreationPerformance"), tasksTest.WantPopulateAttributes());

	for (int nNumLevels = 2; nNumLevels <= tasksTest.NUM_TESTLEVELS; nNumLevels++)
	{
		CTaskFile tasks;
		tasksTest.PopulateHierarchy(tasks, nNumLevels);

		CToDoCtrlData data(m_aStyles, m_aCustomAttribDefs);

 		TestDataModelCreationPerformance(tasks, data, _T("nested"));
		TestDataModelCalculationPerformance(data, _T("nested"));
		TestDataModelFormattingPerformance(data, _T("nested"));
		TestDataModelExporterPerformance(data, _T("nested"));
		TestDataModelGetTaskPositionPerformance(data, _T("nested"));
		TestDataModelGetTaskPerformance(data, _T("nested"));

		printf("\n");
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

	CTaskFileTest tasksTest(m_utils);
	
	BeginTest(_T("FlatListDataModelPerformance"), tasksTest.WantPopulateAttributes());

	for (int nNumLevels = 2, nNumTasks = 10; nNumLevels <= tasksTest.NUM_TESTLEVELS; nNumLevels++)
	{
		// Numbers to match hierarchical test
		nNumTasks += (int)pow(10, nNumLevels);

		CTaskFile tasks;
		tasksTest.PopulateFlatList(tasks, nNumTasks);

		CToDoCtrlData data(m_aStyles, m_aCustomAttribDefs);

 		TestDataModelCreationPerformance(tasks, data, _T("flat"));
		TestDataModelCalculationPerformance(data, _T("flat"));
		TestDataModelFormattingPerformance(data, _T("flat"));
		TestDataModelExporterPerformance(data, _T("flat"));
		TestDataModelGetTaskPositionPerformance(data, _T("flat"));
		TestDataModelGetTaskPerformance(data, _T("flat"));

		printf("\n");
	}

	EndTest();
}

void CToDoCtrlDataTest::TestDataModelCreationPerformance(const CTaskFile& tasks, CToDoCtrlData& data, LPCTSTR szTaskType)
{
	ASSERT(m_utils.HasCommandlineFlag('p'));

	DWORD dwTickStart = GetTickCount();

	data.BuildDataModel(tasks);

	DWORD dwDuration = (GetTickCount() - dwTickStart);
	_tprintf(_T("Test took %ld ms to build data model with %d %s tasks (%.1f ms/100)\n"), 
			 dwDuration, 
			 data.GetTaskCount(), 
			 szTaskType,
			 (dwDuration * 100.0) / data.GetTaskCount());
}

void CToDoCtrlDataTest::TestDataModelCalculationPerformance(const CToDoCtrlData& data, LPCTSTR szTaskType)
{
	ASSERT(m_utils.HasCommandlineFlag('p'));

	DWORD dwTickStart = GetTickCount();

	CTDCTaskCalculator calc(data);
	DWORD dwMaxTaskID = data.GetTaskCount();

	for (DWORD dwTaskID = 1; dwTaskID <= dwMaxTaskID; dwTaskID++)
	{
		calc.GetTaskSubtaskCompletion(dwTaskID);
		calc.GetTaskTimeEstimate(dwTaskID, TDCU_DAYS);
		calc.GetTaskTimeSpent(dwTaskID, TDCU_DAYS);
		calc.GetTaskPercentDone(dwTaskID);
		calc.GetTaskCost(dwTaskID);
		calc.GetTaskPriority(dwTaskID, TRUE);
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
	_tprintf(_T("Test took %ld ms to perform calculations on %d %s tasks (%.1f ms/100)\n"), 
			 dwDuration, 
			 data.GetTaskCount(), 
			 szTaskType,
			 (dwDuration * 100.0) / data.GetTaskCount());
}

void CToDoCtrlDataTest::TestDataModelFormattingPerformance(const CToDoCtrlData& data, LPCTSTR szTaskType)
{
	ASSERT(m_utils.HasCommandlineFlag('p'));

	DWORD dwTickStart = GetTickCount();

	CTDCTaskFormatter formatter(data);
	DWORD dwMaxTaskID = data.GetTaskCount();

	for (DWORD dwTaskID = 1; dwTaskID <= dwMaxTaskID; dwTaskID++)
	{
		formatter.GetTaskSubtaskCompletion(dwTaskID);
		formatter.GetTaskPath(dwTaskID);
		formatter.GetTaskPosition(dwTaskID);
		formatter.GetTaskTimeEstimate(dwTaskID);
		formatter.GetTaskTimeSpent(dwTaskID);
		formatter.GetTaskTimeRemaining(dwTaskID);
		formatter.GetTaskPercentDone(dwTaskID);
		formatter.GetTaskCost(dwTaskID);
		formatter.GetTaskPriority(dwTaskID, FALSE);
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
	_tprintf(_T("Test took %ld ms to format attributes for %d %s tasks (%.1f ms/100)\n"), 
			 dwDuration, 
			 data.GetTaskCount(), 
			 szTaskType,
			 (dwDuration * 100.0) / data.GetTaskCount());
}

void CToDoCtrlDataTest::TestDataModelGetTaskPositionPerformance(const CToDoCtrlData& data, LPCTSTR szTaskType)
{
	ASSERT(m_utils.HasCommandlineFlag('p'));

	DWORD dwTickStart = GetTickCount();

	CTDCTaskFormatter formatter(data);
	DWORD dwMaxTaskID = data.GetTaskCount();

	for (DWORD dwTaskID = 1; dwTaskID <= dwMaxTaskID; dwTaskID++)
	{
		CArray<int, int> aPositions;
		data.GetTaskPositions(dwTaskID, aPositions);

		formatter.GetTaskPosition(dwTaskID);
	}

	DWORD dwDuration = (GetTickCount() - dwTickStart);
	_tprintf(_T("Test took %ld ms to get task position for %d %s tasks (%.1f ms/100)\n"), 
			 dwDuration, 
			 data.GetTaskCount(), 
			 szTaskType,
			 (dwDuration * 100.0) / data.GetTaskCount());
}

void CToDoCtrlDataTest::TestDataModelExporterPerformance(const CToDoCtrlData& data, LPCTSTR szTaskType)
{
	ASSERT(m_utils.HasCommandlineFlag('p'));

	// Mocks ----------------------------------------
	CTreeCtrl tree;
	const CTreeCtrlHelper tch(tree);

	const CTDCImageList ilIcons;
	const TDCAUTOLISTDATA tld;
	const CTDCColumnIDMap mapVisibleCols;

	const CTDLTaskTreeCtrl colors(ilIcons,
								  data,
								  m_aStyles,
								  tld,
								  mapVisibleCols,
								  m_aCustomAttribDefs);
	CContentMgr comments;
	// ----------------------------------------------
	
	DWORD dwTickStart = GetTickCount();
	CTaskFile tasks;
	
	CTDCTaskExporter exporter(data, colors, comments);
	exporter.ExportAllTasks(tasks);
	
	DWORD dwDuration = (GetTickCount() - dwTickStart);
	_tprintf(_T("Test took %ld ms to export %d %s tasks (%.1f ms/100)\n"), 
			 dwDuration, 
			 data.GetTaskCount(), 
			 szTaskType,
			 (dwDuration * 100.0) / data.GetTaskCount());
}

void CToDoCtrlDataTest::TestDataModelGetTaskPerformance(const CToDoCtrlData& data, LPCTSTR szTaskType)
{
	ASSERT(m_utils.HasCommandlineFlag('p'));

	DWORD dwTickStart = GetTickCount();
	DWORD dwMaxTaskID = data.GetTaskCount();

	for (DWORD dwTaskID = 1; dwTaskID <= dwMaxTaskID; dwTaskID++)
	{
		const TODOITEM* pTDI = NULL;
		const TODOSTRUCTURE* pTDS = NULL;

		data.GetTask(dwTaskID, pTDI, pTDS);
	}

	DWORD dwDuration = (GetTickCount() - dwTickStart);
	_tprintf(_T("Test took %ld ms to locate %d %s tasks (%.1f ms/100)\n"),
			 dwDuration,
			 data.GetTaskCount(),
			 szTaskType,
			 (dwDuration * 100.0) / data.GetTaskCount());
}
