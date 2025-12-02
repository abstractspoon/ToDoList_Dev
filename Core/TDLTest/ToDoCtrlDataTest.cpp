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
#include "..\todolist\tdcmapping.h"

#include "..\shared\treectrlhelper.h"
#include "..\shared\datehelper.h"

#include "..\interfaces\contentmgr.h"
#include "..\interfaces\IEnums.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToDoCtrlDataTest::CToDoCtrlDataTest(const CTestUtils& utils) 
	: 
	CTDLTestBase(_T("CToDoCtrlDataTest"), utils)
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

	TestSetTaskPriorityRisk();
	TestOffsetTaskPriorityRisk();
	TestAdjustNewRecurringTasksDates();

	TestCanOffsetTaskDate();
	TestOffsetTaskDate();

	// Performance
	TestHierarchyDataModelPerformance();
	TestFlatListDataModelPerformance();

	return GetTotals();
}

void CToDoCtrlDataTest::BeginPerformanceTest(LPCTSTR szFunction, BOOL bWithAttributes)
{
	CString sTest;
	
	sTest += _T("CToDoCtrlDataTest::");
	sTest += szFunction;
	
	if (bWithAttributes)
		sTest += _T("(WITH attributes)");
	else
		sTest += _T("(WITHOUT attributes)");
	
	BeginTest(sTest);
}

void CToDoCtrlDataTest::TestHierarchyDataModelPerformance()
{
	if (!m_utils.GetWantPerformanceTests())
	{
		_tprintf(_T("Add '-p' to run CToDoCtrlDataTest::HierarchyDataModelCreationPerformance\n"));
		return;
	}

	CTaskFileTest tasksTest(m_utils);

	BeginPerformanceTest(_T("HierarchyDataModelCreationPerformance"), tasksTest.WantPerformanceAttributes());

	for (int nNumLevels = 2; nNumLevels <= tasksTest.NUM_PERFTESTLEVELS; nNumLevels++)
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
	if (!m_utils.GetWantPerformanceTests())
	{
		_tprintf(_T("Add '-p' to run CToDoCtrlDataTest::FlatListDataModelPerformance\n"));
		return;
	}

	CTaskFileTest tasksTest(m_utils);
	
	BeginPerformanceTest(_T("FlatListDataModelPerformance"), tasksTest.WantPerformanceAttributes());

	for (int nNumLevels = 2, nNumTasks = 10; nNumLevels <= tasksTest.NUM_PERFTESTLEVELS; nNumLevels++)
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
	ASSERT(m_utils.GetWantPerformanceTests());

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
	ASSERT(m_utils.GetWantPerformanceTests());

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
		calc.GetTaskTimeRemaining(dwTaskID, nUnits);

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
	ASSERT(m_utils.GetWantPerformanceTests());

	DWORD dwTickStart = GetTickCount();

	CContentMgr mgrContent;
	CTDCTaskFormatter formatter(data, mgrContent);

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
	ASSERT(m_utils.GetWantPerformanceTests());

	DWORD dwTickStart = GetTickCount();

	CContentMgr mgrContent;
	CTDCTaskFormatter formatter(data, mgrContent);

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
	ASSERT(m_utils.GetWantPerformanceTests());

	// Mocks ----------------------------------------
	const CTDCImageList ilIcons;
	const TDCAUTOLISTDATA tld;
	const CTDCColumnIDMap mapVisibleCols;
	const CContentMgr mgrContent;

	const CTDLTaskTreeCtrl colors(ilIcons,
								  data,
								  m_aStyles,
								  tld,
								  mapVisibleCols,
								  m_aCustomAttribDefs,
								  mgrContent);
	// ----------------------------------------------
	
	DWORD dwTickStart = GetTickCount();
	CTaskFile tasks;
	
	CTDCTaskExporter exporter(data, colors, mgrContent);
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
	ASSERT(m_utils.GetWantPerformanceTests());

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

void CToDoCtrlDataTest::TestAdjustNewRecurringTasksDates()
{
	CTDCScopedTest test(*this, _T("CToDoCtrlData::AdjustNewRecurringTasksDates"));

	TestAdjustNewRecurringTasksDates(TDIRO_STARTDATE);
	TestAdjustNewRecurringTasksDates(TDIRO_DUEDATE);
	TestAdjustNewRecurringTasksDates(TDIRO_DONEDATE);
}

////////////////////////////////////////////////////////////////////////////

void CToDoCtrlDataTest::TestAdjustNewRecurringTasksDates(TDC_RECURFROMOPTION nRecalcFrom)
{
	DWORD PRIMES[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
	int NUM_PRIMES = (sizeof(PRIMES) / sizeof(PRIMES[0])), i;

	//  nRegularity								dwSpecific1				dwSpecific2
	//  -------------------------------------|-----------------------|-------------------

	//	TDIR_DAY_EVERY_NDAYS					every 'n' days			--- (0)
	for (i = 0; i < NUM_PRIMES; i++)
	{
		TestAdjustNewRecurringTasksDates(TDIR_DAY_EVERY_NDAYS, PRIMES[i], 0, nRecalcFrom);
	}

	//  -------------------------------------|-----------------------|-------------------
	//	TDIR_DAY_EVERY_WEEKDAY					--- (0)					--- (0)
	TestAdjustNewRecurringTasksDates(TDIR_DAY_EVERY_WEEKDAY, 0, 0, nRecalcFrom);

	//  -------------------------------------|-----------------------|-------------------
	//	TDIR_DAY_EVERY_NWEEKDAYS				every 'n' days			--- (0)
	for (i = 0; i < NUM_PRIMES; i++)
	{
		TestAdjustNewRecurringTasksDates(TDIR_DAY_EVERY_NWEEKDAYS, PRIMES[i], 0, nRecalcFrom);
	}

	//  -------------------------------------|-----------------------|-------------------
	//	TDIR_WEEK_SPECIFIC_DOWS_NWEEKS			every 'n' weeks			weekdays (DHW_...)
	DWORD DOWS[] =
	{
		DHW_MONDAY,
		DHW_TUESDAY | DHW_WEDNESDAY,
		DHW_MONDAY | DHW_WEDNESDAY | DHW_FRIDAY,
		DHW_SATURDAY | DHW_SUNDAY,
		DHW_THURSDAY | DHW_FRIDAY | DHW_SATURDAY,
		DHW_TUESDAY | DHW_SUNDAY,
		DHW_EVERYDAY,
	};
	int NUM_DOWS = (sizeof(DOWS) / sizeof(DOWS[0]));

	for (i = 0; i < NUM_PRIMES; i++)
	{
		for (int k = 0; k < NUM_DOWS; k++)
		{
			TestAdjustNewRecurringTasksDates(TDIR_WEEK_SPECIFIC_DOWS_NWEEKS, PRIMES[i], DOWS[k], nRecalcFrom);
		}
	}

	//  -------------------------------------|-----------------------|-------------------
	//	TDIR_WEEK_EVERY_NWEEKS					every 'n' weeks			--- (0)
	for (i = 0; i < NUM_PRIMES; i++)
	{
		TestAdjustNewRecurringTasksDates(TDIR_WEEK_EVERY_NWEEKS, PRIMES[i], 0, nRecalcFrom);
	}

	//  -------------------------------------|-----------------------|-------------------
	//	TDIR_MONTH_EVERY_NMONTHS				every 'n' months		preserve weekday (BOOL)
	for (i = 0; i < NUM_PRIMES; i++)
	{
		// Don't preserve weekday
		TestAdjustNewRecurringTasksDates(TDIR_MONTH_EVERY_NMONTHS, PRIMES[i], FALSE, nRecalcFrom);
	}
	for (i = 0; i < NUM_PRIMES; i++)
	{
		// Preserve weekday
		TestAdjustNewRecurringTasksDates(TDIR_MONTH_EVERY_NMONTHS, PRIMES[i], TRUE, nRecalcFrom);
	}

	//  -------------------------------------|-----------------------|-------------------
	//	TDIR_MONTH_SPECIFIC_DAY_NMONTHS			every 'n' months		day of month (1-31)
	for (i = 0; i < NUM_PRIMES; i++)
	{
		for (int k = 0; k < NUM_PRIMES; k++)
		{
			TestAdjustNewRecurringTasksDates(TDIR_MONTH_SPECIFIC_DAY_NMONTHS, PRIMES[i], PRIMES[k], nRecalcFrom);
		}
	}

	//  -------------------------------------|-----------------------|-------------------
	//	TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS		first(0), last(!0)		every 'n' months
	for (i = 0; i < NUM_PRIMES; i++)
	{
		// First weekday
		TestAdjustNewRecurringTasksDates(TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS, FALSE, PRIMES[i], nRecalcFrom);
	}
	for (i = 0; i < NUM_PRIMES; i++)
	{
		// Last weekday
		TestAdjustNewRecurringTasksDates(TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS, TRUE, PRIMES[i], nRecalcFrom);
	}

	//  -------------------------------------|-----------------------|-------------------
	//	TDIR_MONTH_SPECIFIC_DOW_NMONTHS			LOWORD = which (1-5)	every 'n' months
	//                                          HIWORD = DOW   (1-7)		
	for (i = 0; i < NUM_PRIMES; i++)
	{
		for (DWORD dwWhich = 1; dwWhich <= 5; dwWhich++)
		{
			for (DWORD dwDow = 1; dwDow <= 5; dwDow++)
			{
				TestAdjustNewRecurringTasksDates(TDIR_MONTH_SPECIFIC_DOW_NMONTHS, MAKELONG(dwWhich, dwDow), PRIMES[i], nRecalcFrom);
			}
		}
	}

	//  -------------------------------------|-----------------------|-------------------
	//	TDIR_YEAR_SPECIFIC_DAY_MONTHS			month (1-12)			day of month (1-31)
	for (i = 0; i < NUM_PRIMES; i++)
	{
		for (DWORD dwMonth = 1; dwMonth < 12; dwMonth++)
		{
			TestAdjustNewRecurringTasksDates(TDIR_YEAR_SPECIFIC_DAY_MONTHS, dwMonth, PRIMES[i], nRecalcFrom);
		}
	}

	//  -------------------------------------|-----------------------|-------------------
	//	TDIR_YEAR_EVERY_NYEARS					every 'n' years			preserve weekday (BOOL)
	for (i = 0; i < NUM_PRIMES; i++)
	{
		// Don't preserve weekday
		TestAdjustNewRecurringTasksDates(TDIR_YEAR_EVERY_NYEARS, PRIMES[i], FALSE, nRecalcFrom);
	}
	for (i = 0; i < NUM_PRIMES; i++)
	{
		// Preserve weekday
		TestAdjustNewRecurringTasksDates(TDIR_YEAR_EVERY_NYEARS, PRIMES[i], TRUE, nRecalcFrom);
	}

	//  -------------------------------------|-----------------------|-------------------
	//  TDIR_YEAR_SPECIFIC_DOW_MONTHS			LOWORD = which (1-5)	specific month (1-12)
	//                                          HIWORD = DOW   (1-7)		
	for (DWORD dwMonth = 1; dwMonth < 12; dwMonth++)
	{
		for (DWORD dwWhich = 1; dwWhich <= 5; dwWhich++)
		{
			for (DWORD dwDow = 1; dwDow <= 5; dwDow++)
			{
				TestAdjustNewRecurringTasksDates(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(dwWhich, dwDow), dwMonth, nRecalcFrom);
			}
		}
	}
}

void CToDoCtrlDataTest::TestAdjustNewRecurringTasksDates(TDC_REGULARITY nRegularity, DWORD dwSpecific1, DWORD dwSpecific2, TDC_RECURFROMOPTION nRecalcFrom)
{
	CTDCScopedSubTest subtest(*this, GetRegularityText(nRegularity, dwSpecific1, dwSpecific2, nRecalcFrom));

	CToDoCtrlData data(m_aStyles, m_aCustomAttribDefs);
	CUndoAction undo(data, TDCUAT_ADD, FALSE); // Otherwise CToDoCtrlData will assert

	const DWORD dwTaskID = 1;
	TODOITEM* pTDI = data.NewTask(TODOITEM());

	ExpectTrue(data.AddTask(dwTaskID, pTDI, 0, 0));
	ExpectTrue(pTDI->trRecurrence.SetRegularity(nRegularity, dwSpecific1, dwSpecific2));

	pTDI->trRecurrence.nRecalcFrom = nRecalcFrom;

	switch (nRecalcFrom)
	{
	case TDIRO_STARTDATE:
		pTDI->dateStart = COleDateTime(40000.0);
		break;

	case TDIRO_DUEDATE:
		pTDI->dateStart = COleDateTime(40000.0);
		pTDI->dateDue = COleDateTime(40002.0);
		break;

	case TDIRO_DONEDATE:
		pTDI->dateDue = COleDateTime(40002.0);
		break;
	}

	// A completion date is always required by GetNextTaskOccurrence
	pTDI->dateDone = COleDateTime(40010.0);

	for (int i = 0; i < 10; i++)
	{
		COleDateTime dtNext;
		BOOL bDueDate = FALSE;

		ExpectTrue(data.GetNextTaskOccurrence(dwTaskID, dtNext, bDueDate));
		ExpectTrue(CDateHelper::IsDateSet(dtNext));

		ExpectEQ(SET_CHANGE, data.AdjustNewRecurringTasksDates(dwTaskID, dwTaskID, dtNext, bDueDate));
		ExpectEQ(data.GetTaskDate(dwTaskID, (bDueDate ? TDCD_DUE : TDCD_START)), dtNext);

		// Keep bumping the completion date else AdjustNewRecurringTasksDates
		// will assert because it keeps producing the same outcome and so the 
		// offset will be zero
		if (nRecalcFrom == TDIRO_DONEDATE)
			data.SetTaskDate(dwTaskID, TDCD_DONE, dtNext);
	}
}

CString CToDoCtrlDataTest::GetRegularityText(TDC_REGULARITY nRegularity, DWORD dwSpecific1, DWORD dwSpecific2, TDC_RECURFROMOPTION nRecalcFrom)
{
	LPCTSTR szFrom = NULL;

	switch (nRecalcFrom)
	{
	case TDIRO_STARTDATE: szFrom = _T("From Start"); break;
	case TDIRO_DUEDATE:   szFrom = _T("From Due"); break;
	case TDIRO_DONEDATE:  szFrom = _T("From Done"); break;
	}
	ASSERT(szFrom);

	LPCTSTR szRegularity = NULL;

	switch (nRegularity)
	{
	case TDIR_DAY_EVERY_NDAYS:					szRegularity = _T("TDIR_DAY_EVERY_NDAYS");					break;
	case TDIR_DAY_EVERY_WEEKDAY:				szRegularity = _T("TDIR_DAY_EVERY_WEEKDAY");				break;
	case TDIR_DAY_EVERY_NWEEKDAYS:				szRegularity = _T("TDIR_DAY_EVERY_NWEEKDAYS");				break;
	case TDIR_WEEK_SPECIFIC_DOWS_NWEEKS:		szRegularity = _T("TDIR_WEEK_SPECIFIC_DOWS_NWEEKS");		break;
	case TDIR_WEEK_EVERY_NWEEKS:				szRegularity = _T("TDIR_WEEK_EVERY_NWEEKS");				break;
	case TDIR_MONTH_EVERY_NMONTHS:				szRegularity = _T("TDIR_MONTH_EVERY_NMONTHS");				break;
	case TDIR_MONTH_SPECIFIC_DAY_NMONTHS:		szRegularity = _T("TDIR_MONTH_SPECIFIC_DAY_NMONTHS");		break;
	case TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS:	szRegularity = _T("TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS");	break;
	case TDIR_MONTH_SPECIFIC_DOW_NMONTHS:		szRegularity = _T("TDIR_MONTH_SPECIFIC_DOW_NMONTHS");		break;
	case TDIR_YEAR_SPECIFIC_DAY_MONTHS:			szRegularity = _T("TDIR_YEAR_SPECIFIC_DAY_MONTHS");			break;
	case TDIR_YEAR_EVERY_NYEARS:				szRegularity = _T("TDIR_YEAR_EVERY_NYEARS");				break;
	case TDIR_YEAR_SPECIFIC_DOW_MONTHS:			szRegularity = _T("TDIR_YEAR_SPECIFIC_DOW_MONTHS");			break;
	}
	ASSERT(szRegularity);

	return Misc::Format(_T("%s(%ld, %ld, %s)"), szRegularity, dwSpecific1, dwSpecific2, szFrom);
}

////////////////////////////////////////////////////////////////////////////

void CToDoCtrlDataTest::TestSetTaskPriorityRisk()
{
	{
		CTDCScopedTest test(*this, _T("CToDoCtrlData::SetTaskPriority"));

		TestSetTaskPriorityRisk(TRUE);
	}

	{
		CTDCScopedTest test(*this, _T("CToDoCtrlData::SetTaskRisk"));

		TestSetTaskPriorityRisk(FALSE);
	}
}

void CToDoCtrlDataTest::TestOffsetTaskPriorityRisk()
{
	{
		CTDCScopedTest test(*this, _T("CToDoCtrlData::OffsetTaskPriority"));

		for (int nNumLevels = TDC_PRIORITYORRISK_MINLEVELS;
				 nNumLevels <= TDC_PRIORITYORRISK_MAXLEVELS; nNumLevels++)
		{
			TestOffsetTaskPriorityRisk(TRUE, nNumLevels);
		}
	}

	{
		CTDCScopedTest test(*this, _T("CToDoCtrlData::OffsetTaskRisk"));

		for (int nNumLevels = TDC_PRIORITYORRISK_MINLEVELS;
			 nNumLevels <= TDC_PRIORITYORRISK_MAXLEVELS; nNumLevels++)
		{
			TestOffsetTaskPriorityRisk(FALSE, nNumLevels);
		}
	}
}

// -----------------------------------------------

#define GET_PRIORITYRISK() \
	(bPriority ? data.GetTaskPriority(dwTaskID) : data.GetTaskRisk(dwTaskID))

#define SET_PRIORITYRISK(value) \
	(bPriority ? data.SetTaskPriority(dwTaskID, value, FALSE) : data.SetTaskRisk(dwTaskID, value, FALSE))

#define OFFSET_PRIORITYRISK(offset) \
	(bPriority ? data.SetTaskPriority(dwTaskID, offset, TRUE) : data.SetTaskRisk(dwTaskID, offset, TRUE))

// -----------------------------------------------

void CToDoCtrlDataTest::TestSetTaskPriorityRisk(BOOL bPriority)
{
	ASSERT(IsTestActive());

	CTDCStyleMap aStyles;
	CTDCCustomAttribDefinitionArray aCustAttrib;

	CToDoCtrlData data(aStyles, aCustAttrib);
	CUndoAction undo(data, TDCUAT_ADD, FALSE); // Otherwise CToDoCtrlData will assert

	const DWORD dwTaskID = 1;
	TODOITEM* pTDI = data.NewTask(TODOITEM());

	ExpectTrue(data.AddTask(dwTaskID, pTDI, 0, 0));
	ExpectEQ(GET_PRIORITYRISK(), TDC_PRIORITYORRISK_NONE); // default value
	
	for (int nValue = (TDC_PRIORITYORRISK_MIN - 10);
			(nValue <= TDC_PRIORITYORRISK_MAX + 10); nValue++)
	{
		int nCurValue = GET_PRIORITYRISK();
		TDC_SET nSet = SET_PRIORITYRISK(nValue);

		int nNewValue = GET_PRIORITYRISK();

		if (nValue == TDC_PRIORITYORRISK_NONE)
		{
			ExpectEQ(nSet, SET_NOCHANGE); // it started off as this value
			ExpectEQ(nNewValue, nValue);
		}
		else if ((nValue >= TDC_PRIORITYORRISK_MIN) && (nValue <= TDC_PRIORITYORRISK_MAX))
		{
			ExpectEQ(nSet, SET_CHANGE);
			ExpectEQ(GET_PRIORITYRISK(), nValue);
		}
		else
		{
			ExpectEQ(nSet, SET_FAILED);
			ExpectEQ(GET_PRIORITYRISK(), nCurValue);
		}
	}
}

void CToDoCtrlDataTest::TestOffsetTaskPriorityRisk(BOOL bPriority, int nNumLevels)
{
	CTDCStyleMap aStyles;
	CTDCCustomAttribDefinitionArray aCustAttrib;

	CToDoCtrlData data(aStyles, aCustAttrib);
	data.SetNumPriorityRiskLevels(nNumLevels);

	CUndoAction undo(data, TDCUAT_ADD, FALSE); // Otherwise CToDoCtrlData will assert

	const DWORD dwTaskID = 1;
	TODOITEM* pTDI = data.NewTask(TODOITEM());

	VERIFY(data.AddTask(dwTaskID, pTDI, 0, 0));

	const int nMaxLevel = (nNumLevels - 1);

	// Offset < 0
	{
		const int OFFSETS[] = { -1, -3, -5, -7 };
		const int NUM_OFFSETS = (sizeof(OFFSETS), sizeof(OFFSETS[0]));

		for (int i = 0; i < NUM_OFFSETS; i++)
		{
			const int nOffset = OFFSETS[i];

			for (int nStart = TDC_PRIORITYORRISK_NONE; nStart <= TDC_PRIORITYORRISK_MAX; nStart++)
			{
				if (nStart == -1)
					nStart = TDC_PRIORITYORRISK_MIN;

				CTDCScopedSubTest subtest(*this, Misc::Format(_T("Num Levels = %d, Offset = %d, Start = %d"), nNumLevels, nOffset, nStart));

				SET_PRIORITYRISK(nStart);
				ExpectEQ(GET_PRIORITYRISK(), nStart);

				for (int k = 0; k < 10; k++)
				{
					int nCurValue = GET_PRIORITYRISK();
					ASSERT(TODOITEM::IsValidPriorityOrRisk(nCurValue));

					TDC_SET nSet = OFFSET_PRIORITYRISK(nOffset);
					int nNewValue = GET_PRIORITYRISK();

					int nAttempt = (nCurValue + nOffset);

					if (nCurValue <= 0)
					{
						ExpectEQ(nNewValue, TDC_PRIORITYORRISK_NONE);
					}
					else if (nAttempt <= 0)
					{
						ExpectEQ(nNewValue, TDC_PRIORITYORRISK_MIN);
					}
					else if (nAttempt > nMaxLevel)
					{
						ExpectEQ(nNewValue, nMaxLevel);
					}
					else
					{
						ExpectEQ(nNewValue, nAttempt);
					}
				}
			}
		}
	}

	// Offset > 0
	{
		const int OFFSETS[] = { 1, 3, 5, 7 };
		const int NUM_OFFSETS = (sizeof(OFFSETS), sizeof(OFFSETS[0]));

		for (int i = 0; i < NUM_OFFSETS; i++)
		{
			const int nOffset = OFFSETS[i];

			for (int nStart = TDC_PRIORITYORRISK_NONE; nStart <= TDC_PRIORITYORRISK_MAX; nStart++)
			{
				if (nStart == -1)
					nStart = TDC_PRIORITYORRISK_MIN;

				CTDCScopedSubTest subtest(*this, Misc::Format(_T("Num Levels = %d, Offset = %d, Start = %d"), nNumLevels, nOffset, nStart));

				SET_PRIORITYRISK(nStart);
				ExpectEQ(GET_PRIORITYRISK(), nStart);

				for (int j = 0; j < 10; j++)
				{
					int nCurValue = GET_PRIORITYRISK();
					ASSERT(TODOITEM::IsValidPriorityOrRisk(nCurValue));

					TDC_SET nSet = OFFSET_PRIORITYRISK(nOffset);
					int nNewValue = GET_PRIORITYRISK();

					int nAttempt = (nCurValue + nOffset);

					if (nCurValue == TDC_PRIORITYORRISK_NONE)
					{
						ExpectEQ(nNewValue, TDC_PRIORITYORRISK_MIN);
					}
					else if (nCurValue > nMaxLevel)
					{
						ExpectEQ(nNewValue, nCurValue);
					}
					else if (nAttempt > nMaxLevel)
					{
						ExpectEQ(nNewValue, nMaxLevel);
					}
					else
					{
						ExpectEQ(nNewValue, nAttempt);
					}
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////

void CToDoCtrlDataTest::TestCanOffsetTaskDate()
{
	CTDCScopedTest test(*this, _T("CToDoCtrlData::CanOffsetTaskDate"));

	CTDCStyleMap aStyles;
	CTDCCustomAttribDefinitionArray aCustAttrib;

	CToDoCtrlData data(aStyles, aCustAttrib);
	CUndoAction undo(data, TDCUAT_ADD, FALSE); // Otherwise CToDoCtrlData will assert

	const DWORD dwTaskID = 1;
	TODOITEM* pTDI = data.NewTask(TODOITEM());

	ExpectTrue(data.AddTask(dwTaskID, pTDI, 0, 0));

	const TDC_DATE DATES[] = { TDCD_CREATE, TDCD_START, TDCD_STARTDATE,	TDCD_STARTTIME,	TDCD_DUE, TDCD_DUEDATE, TDCD_DUETIME, TDCD_DONE, TDCD_DONEDATE, TDCD_DONETIME, TDCD_LASTMOD, TDCD_CUSTOM, TDCD_REMINDER };
	const int NUM_DATES = (sizeof(DATES) / sizeof(DATES[0]));

	const TDC_UNITS UNITS[] = { TDCU_MINS, TDCU_HOURS, TDCU_DAYS, TDCU_WEEKDAYS, TDCU_WEEKS, TDCU_MONTHS, TDCU_YEARS };
	const int NUM_UNITS = (sizeof(UNITS) / sizeof(UNITS[0]));

	const int AMOUNTS[] = { 1, 3, 5, 7, 11 };
	const int NUM_AMOUNTS = (sizeof(AMOUNTS), sizeof(AMOUNTS[0]));

	for (int i = 0; i < NUM_DATES; i++)
	{
		const TDC_DATE nDate = DATES[i];

		for (int j = 0; j < NUM_UNITS; j++)
		{
			const TDC_UNITS nUnits = UNITS[j];

			CTDCScopedSubTest subtest(*this, Misc::Format(_T("Uninitialised Task, %s, %s"), GetDateTypeText(nDate), GetUnitsText(nUnits)));

			for (int k = 0; k < NUM_AMOUNTS; k++)
			{
				const int nAmount = AMOUNTS[k];

				TestCanOffsetTaskDate(data, dwTaskID, nDate, nAmount, nUnits, 0);
				TestCanOffsetTaskDate(data, dwTaskID, nDate, -nAmount, nUnits, 0);

				TestCanOffsetTaskDate(data, dwTaskID, nDate, nAmount, nUnits, TDCOTD_OFFSETFROMTODAY);
				TestCanOffsetTaskDate(data, dwTaskID, nDate, -nAmount, nUnits, TDCOTD_OFFSETFROMTODAY);
			}
		}
	}
}

void CToDoCtrlDataTest::TestCanOffsetTaskDate(const CToDoCtrlData& data, DWORD dwTaskID, TDC_DATE nDate,
											  int nAmount, TDC_UNITS nUnits, DWORD dwFlags)
{
	BOOL bOffsetFromToday = Misc::HasFlag(dwFlags, TDCOTD_OFFSETFROMTODAY);
	BOOL bHasValidUnits = IsValidUnits(nUnits);
	BOOL bUnitsAreTime = ((nUnits == TDCU_HOURS) || (nUnits == TDCU_MINS));
	BOOL bUnitsAreDate = (bHasValidUnits && !bUnitsAreTime);

	BOOL bExpectedResult = -1;

	switch (nDate)
	{
	case TDCD_CREATE:	
	case TDCD_LASTMOD:
	case TDCD_NONE:
		// Never offsetable
		bExpectedResult = FALSE; 
		break;

	case TDCD_REMINDER:	
		// Not offsetable by CToDoCtrlData
		bExpectedResult = FALSE; 
		break;

	case TDCD_CUSTOM:
		// Not currently offsetable by CToDoCtrlData
		bExpectedResult = FALSE; 
		break;

	case TDCD_START:	
	case TDCD_STARTDATE:
	case TDCD_DUE:		
	case TDCD_DUEDATE:	
		// Start and Due dates only support date units 
		// and Start/Due dates can be uninitialised if offsetting from today
		bExpectedResult = (bUnitsAreDate && (bOffsetFromToday || data.TaskHasDate(dwTaskID, nDate)));
		break;

	case TDCD_STARTTIME:
		// Start time only supports time units 
		// and does not support offsetting from today
		// and Start date must already be initialised
		bExpectedResult = (bUnitsAreTime && !bOffsetFromToday && data.TaskHasDate(dwTaskID, TDCD_START));
		break;

	case TDCD_DUETIME:	
		// Done time only supports time units 
		// and does not support offsetting from today
		// and Due date must already be initialised
		bExpectedResult = (bUnitsAreTime && !bOffsetFromToday && data.TaskHasDate(dwTaskID, TDCD_DUE));
		break;

	case TDCD_DONE:
	case TDCD_DONEDATE:
		// Done date only support date units 
		// and does not support offsetting from today
		// and Done date must already be initialised
		bExpectedResult = (bUnitsAreDate && data.TaskHasDate(dwTaskID, nDate));
		break;

	case TDCD_DONETIME:
		// Done time only support time units 
		// and Done date must already be initialised
		bExpectedResult = (bUnitsAreTime && !bOffsetFromToday && data.TaskHasDate(dwTaskID, TDCD_DONE));
		break;
	}

	ExpectEQ(data.CanOffsetTaskDate(dwTaskID, nDate, nAmount, nUnits, dwFlags), bExpectedResult);
}

// ------------------------------------------------------------

void CToDoCtrlDataTest::TestOffsetTaskDate()
{
	CTDCScopedTest test(*this, _T("CToDoCtrlData::OffsetTaskDate"));

	CTDCStyleMap aStyles;
	CTDCCustomAttribDefinitionArray aCustAttrib;

	CToDoCtrlData data(aStyles, aCustAttrib);
	CUndoAction undo(data, TDCUAT_ADD, FALSE); // Otherwise CToDoCtrlData will assert

	const DWORD dwTaskID = 1;
	TODOITEM* pTDI = data.NewTask(TODOITEM());

	ExpectTrue(data.AddTask(dwTaskID, pTDI, 0, 0));

	// No dates
	{
		pTDI->dateStart = CDateHelper::NullDate();
		pTDI->dateDue   = CDateHelper::NullDate();
		pTDI->dateDone  = CDateHelper::NullDate();

		TestOffsetTaskDate(_T("No dates"), data, dwTaskID);
	}

	// Dates no Times
	{
		pTDI->dateStart = COleDateTime(40000.0);
		pTDI->dateDue   = COleDateTime(40001.0);
		pTDI->dateDone  = COleDateTime(40002.0);

		TestOffsetTaskDate(_T("Dates no Times"), data, dwTaskID);
	}

	// Dates and Times
	{
		pTDI->dateStart = COleDateTime(40000.25);
		pTDI->dateDue   = COleDateTime(40001.50);
		pTDI->dateDone  = COleDateTime(40002.75);

		TestOffsetTaskDate(_T("Dates and Times"), data, dwTaskID);
	}
}

void CToDoCtrlDataTest::TestOffsetTaskDate(LPCTSTR szSubTest, CToDoCtrlData& data, DWORD dwTaskID)
{
	const TDC_DATE DATES[] = { TDCD_CREATE, TDCD_START, TDCD_STARTDATE, TDCD_STARTTIME, TDCD_DUE, TDCD_DUEDATE, TDCD_DUETIME, TDCD_DONE, TDCD_DONEDATE, TDCD_DONETIME, TDCD_LASTMOD, TDCD_CUSTOM, TDCD_REMINDER };
	const int NUM_DATES = (sizeof(DATES) / sizeof(DATES[0]));

	const TDC_UNITS UNITS[] = { TDCU_MINS, TDCU_HOURS, TDCU_DAYS, TDCU_WEEKDAYS, TDCU_WEEKS, TDCU_MONTHS, TDCU_YEARS };
	const int NUM_UNITS = (sizeof(UNITS) / sizeof(UNITS[0]));

	const int AMOUNTS[] = { 1, 3, 5, 7, 11 };
	const int NUM_AMOUNTS = (sizeof(AMOUNTS), sizeof(AMOUNTS[0]));

	for (int i = 0; i < NUM_DATES; i++)
	{
		const TDC_DATE nDate = DATES[i];

		for (int j = 0; j < NUM_UNITS; j++)
		{
			const TDC_UNITS nUnits = UNITS[j];

			CTDCScopedSubTest subtest(*this, Misc::Format(_T("%s, %s, %s"), szSubTest, GetDateTypeText(nDate), GetUnitsText(nUnits)));

			for (int k = 0; k < NUM_AMOUNTS; k++)
			{
				const int nAmount = AMOUNTS[k];

				TestOffsetTaskDate(data, dwTaskID, nDate, nAmount, nUnits, 0);
				TestOffsetTaskDate(data, dwTaskID, nDate, -nAmount, nUnits, 0);

				TestOffsetTaskDate(data, dwTaskID, nDate, nAmount, nUnits, TDCOTD_OFFSETFROMTODAY);
				TestOffsetTaskDate(data, dwTaskID, nDate, -nAmount, nUnits, TDCOTD_OFFSETFROMTODAY);
			}
		}
	}
}

// -------------------------------------

#define DATEOFFSET_NOCHANGE \
	ExpectEQ(SET_NOCHANGE, data.OffsetTaskDate(dwTaskID, nDate, nAmount, nUnits, dwFlags, aModTaskIDs)); \
	ExpectEQ(0, aModTaskIDs.GetSize())

#define DATEOFFSET_CHANGE(amount) \
	aModTaskIDs.RemoveAll();      \
	ExpectEQ(SET_CHANGE, data.OffsetTaskDate(dwTaskID, nDate, amount, nUnits, dwFlags, aModTaskIDs)); \
	ExpectEQ(1, aModTaskIDs.GetSize()); \
	ExpectEQ(1UL, aModTaskIDs[0])

// -------------------------------------

void CToDoCtrlDataTest::TestOffsetTaskDate(CToDoCtrlData& data, DWORD dwTaskID, TDC_DATE nDate,
										   int nAmount, TDC_UNITS nUnits, DWORD dwFlags)
{
	BOOL bOffsetFromToday = Misc::HasFlag(dwFlags, TDCOTD_OFFSETFROMTODAY);
	BOOL bZeroAmount = (nAmount == 0);
	BOOL bHasValidUnits = IsValidUnits(nUnits);
	BOOL bUnitsAreTime = ((nUnits == TDCU_HOURS) || (nUnits == TDCU_MINS));
	BOOL bUnitsAreDate = (bHasValidUnits && !bUnitsAreTime);

	CDWordArray aModTaskIDs;

	switch (nDate)
	{
	case TDCD_CREATE:
	case TDCD_LASTMOD:
	case TDCD_NONE:
	case TDCD_REMINDER:
	case TDCD_CUSTOM:
		DATEOFFSET_NOCHANGE;
		break;

	case TDCD_START:
	case TDCD_STARTDATE:
	case TDCD_DUE:
	case TDCD_DUEDATE:
	case TDCD_DONE:
	case TDCD_DONEDATE:
		{
			// Dates only support date units 
			// Start and Due date can be uninitialised if offsetting from today
			// Done date cannot be uninitialised if offsetting from today
			BOOL bMustBeInitialised = ((nDate == TDCD_DONE) || (nDate == TDCD_DONEDATE) || !bOffsetFromToday);

			if (!bUnitsAreDate || (bMustBeInitialised && !data.TaskHasDate(dwTaskID, nDate)) || (bZeroAmount && !bOffsetFromToday))
			{
				DATEOFFSET_NOCHANGE;
			}
			else
			{
				const COleDateTime dtTaskOrg = data.GetTaskDate(dwTaskID, nDate);
				const COleDateTime dtFrom = (bOffsetFromToday ? CDateHelper::GetDate(DHD_TODAY) : dtTaskOrg);
				
				DATEOFFSET_CHANGE(nAmount);

				COleDateTime dtTaskNew = data.GetTaskDate(dwTaskID, nDate);

				if (nAmount > 0)
					ExpectTrue(dtTaskNew > dtFrom);
				else
					ExpectTrue(dtTaskNew < dtFrom);

				// Reverse offset to get original date and time
				DATEOFFSET_CHANGE(-nAmount);

				dtTaskNew = data.GetTaskDate(dwTaskID, nDate);

				if (bOffsetFromToday)
				{
					if (-nAmount > 0)
						ExpectTrue(dtTaskNew > dtFrom);
					else
						ExpectTrue(dtTaskNew < dtFrom);

					// Restore original task date
					data.SetTaskDate(dwTaskID, nDate, dtTaskOrg);
				}
				else
				{
					ExpectTrue(dtTaskNew == dtFrom);
				}
			}
		}
		break;

	case TDCD_STARTTIME:
	case TDCD_DUETIME:
	case TDCD_DONETIME:
		{
			// Times only support time units and
			// and do not support offsetting from today
			// and date must already be initialised
			TDC_DATE nMappedDate = TDC::MapTimeToDate(nDate);

			if (!bUnitsAreTime || bZeroAmount || bOffsetFromToday || !data.TaskHasDate(dwTaskID, nMappedDate))
			{
				DATEOFFSET_NOCHANGE;
			}
			else
			{
				const COleDateTime dtTaskOrg = data.GetTaskDate(dwTaskID, nMappedDate);

				// A negative offset
				if ((nAmount < 0) && !CDateHelper::DateHasTime(dtTaskOrg))
				{
					DATEOFFSET_NOCHANGE;
				}
				else
				{
					// CToDoCtrlData rejects offsets which would result in overflow
					if (nUnits == TDCU_HOURS)
					{
						if (!CDateHelper::IsSameDay(dtTaskOrg, dtTaskOrg.m_dt + (nAmount / 24.0)))
						{
							DATEOFFSET_NOCHANGE;
							return;
						}
					}
					else
					{
						ASSERT(nUnits == TDCU_MINS);

						if (!CDateHelper::IsSameDay(dtTaskOrg, dtTaskOrg.m_dt + (nAmount / (60 * 24.0))))
						{
							DATEOFFSET_NOCHANGE;
							return;
						}
					}

					DATEOFFSET_CHANGE(nAmount);

					COleDateTime dtTaskNew = data.GetTaskDate(dwTaskID, nMappedDate);

					if (nAmount > 0)
						ExpectTrue(dtTaskNew > dtTaskOrg);
					else
						ExpectTrue(dtTaskNew < dtTaskOrg);

					// Reverse offset to get original date and time
					DATEOFFSET_CHANGE(-nAmount);

					dtTaskNew = data.GetTaskDate(dwTaskID, nMappedDate);

					ExpectTrue(dtTaskNew == dtTaskOrg);

					// Restore original task date
					data.SetTaskDate(dwTaskID, nDate, dtTaskOrg);
				}

			}
		}
		break;
	}
}

CString CToDoCtrlDataTest::GetDateTypeText(TDC_DATE nDate)
{
	switch (nDate)
	{
	case TDCD_CREATE:		return _T("TDCD_CREATE");
	case TDCD_START:		return _T("TDCD_START");
	case TDCD_STARTDATE:	return _T("TDCD_STARTDATE");
	case TDCD_STARTTIME:	return _T("TDCD_STARTTIME");
	case TDCD_DUE:			return _T("TDCD_DUE");
	case TDCD_DUEDATE:		return _T("TDCD_DUEDATE");
	case TDCD_DUETIME:		return _T("TDCD_DUETIME");
	case TDCD_DONE:			return _T("TDCD_DONE");
	case TDCD_DONEDATE:		return _T("TDCD_DONEDATE");
	case TDCD_DONETIME:		return _T("TDCD_DONETIME");
	case TDCD_LASTMOD:		return _T("TDCD_LASTMOD");
	case TDCD_CUSTOM:		return _T("TDCD_CUSTOM");
	case TDCD_REMINDER:		return _T("TDCD_REMINDER");
	case TDCD_NONE:			return _T("TDCD_NONE");
	}

	return _T("UNKNOWN");
}

CString CToDoCtrlDataTest::GetUnitsText(TDC_UNITS nUnits)
{
	switch (nUnits)
	{
	case TDCU_MINS:		return _T("TDCU_MINS");
	case TDCU_HOURS:	return _T("TDCU_HOURS");
	case TDCU_DAYS:		return _T("TDCU_DAYS");
	case TDCU_WEEKDAYS:	return _T("TDCU_WEEKDAYS");
	case TDCU_WEEKS:	return _T("TDCU_WEEKS");
	case TDCU_MONTHS:	return _T("TDCU_MONTHS");
	case TDCU_YEARS:	return _T("TDCU_YEARS");
	}

	return _T("UNKNOWN");
}
