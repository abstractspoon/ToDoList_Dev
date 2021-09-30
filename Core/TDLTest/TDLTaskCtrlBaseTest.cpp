// TDLTaskCtrlBaseTest.cpp: implementation of the CTDLTaskCtrlBaseTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTaskCtrlBaseTest.h"
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

CTDLTaskCtrlBaseTest::CTDLTaskCtrlBaseTest(const CTestUtils& utils) : CTDLTestBase(utils)
{
	// Calculation styles
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

	// Visible Columns
	m_mapColumns.Add(TDCC_ALLOCTO);
	m_mapColumns.Add(TDCC_CATEGORY);
	m_mapColumns.Add(TDCC_TAGS);
	m_mapColumns.Add(TDCC_ALLOCBY);
	m_mapColumns.Add(TDCC_STATUS);
	m_mapColumns.Add(TDCC_VERSION);
	m_mapColumns.Add(TDCC_EXTERNALID);
	m_mapColumns.Add(TDCC_CREATEDBY);
	m_mapColumns.Add(TDCC_LASTMODBY);
	m_mapColumns.Add(TDCC_RECURRENCE);
	m_mapColumns.Add(TDCC_PATH);
	m_mapColumns.Add(TDCC_POSITION);
	m_mapColumns.Add(TDCC_SUBTASKDONE);
	m_mapColumns.Add(TDCC_COST);
	m_mapColumns.Add(TDCC_TIMEESTIMATE);
	m_mapColumns.Add(TDCC_TIMESPENT);
	m_mapColumns.Add(TDCC_TIMEREMAINING);
	m_mapColumns.Add(TDCC_ID);
}

CTDLTaskCtrlBaseTest::~CTDLTaskCtrlBaseTest()
{

}

TESTRESULT CTDLTaskCtrlBaseTest::Run()
{
	ClearTotals();

	TestColumnRecalculationPerformance();

	return GetTotals();
}

void CTDLTaskCtrlBaseTest::TestColumnRecalculationPerformance()
{
	if (!m_utils.HasCommandlineFlag('p'))
	{
		_tprintf(_T("Add '-p' to run CTDLTaskCtrlBaseTest::ColumnRecalculationPerformance\n"));
		return;
	}

	BeginTest(_T("CTDLTaskCtrlBaseTest::ColumnRecalculationPerformance"));
	
	TestTreeColumnRecalculationPerformance();
	TestListColumnRecalculationPerformance();

	EndTest();
}

void CTDLTaskCtrlBaseTest::TestTreeColumnRecalculationPerformance()
{
	ASSERT(m_utils.HasCommandlineFlag('p'));

	CTaskFile tasks;
	CTaskFileTest(m_utils).PopulateHierarchy(tasks, 4);

	CToDoCtrlData data(m_aStyles, m_aCustAttribDefs);
	data.BuildDataModel(tasks);

	DWORD dwTickStart = GetTickCount();

	// We can't easily test the actual column calculation because it
	// relies on there being a valid CTreeCtrl which isn't possible here.
	// So instead we simulate the internals of CToDoCtrlFind::GetLongestValues
	CTDCLongestItemMap mapLongest;
	mapLongest.Initialise(m_mapColumns, m_aCustAttribDefs);

	PopulateLongestMap(data, mapLongest);

	DWORD dwDuration = (GetTickCount() - dwTickStart);
	_tprintf(_T("Test took %ld ms to recalc columns with %d tree tasks (%.1f ms/100)\n"),
			 dwDuration,
			 data.GetTaskCount(),
			 (dwDuration * 100.0) / data.GetTaskCount());
}

void CTDLTaskCtrlBaseTest::TestListColumnRecalculationPerformance()
{
	ASSERT(m_utils.HasCommandlineFlag('p'));

	CTaskFile tasks;
	CTaskFileTest(m_utils).PopulateFlatList(tasks, 10000);

	CToDoCtrlData data(m_aStyles, m_aCustAttribDefs);
	data.BuildDataModel(tasks);

	DWORD dwTickStart = GetTickCount();

	// We can't easily test the actual column calculation because it
	// relies on there being a valid CTreeCtrl which isn't possible here.
	// So instead we simulate the internals of CToDoCtrlFind::GetLongestValues
	CTDCLongestItemMap mapLongest;
	mapLongest.Initialise(m_mapColumns, m_aCustAttribDefs);

	PopulateLongestMap(data, mapLongest);

	DWORD dwDuration = (GetTickCount() - dwTickStart);
	_tprintf(_T("Test took %ld ms to recalc columns with %d list tasks (%.1f ms/100)\n"),
			 dwDuration,
			 data.GetTaskCount(),
			 (dwDuration * 100.0) / data.GetTaskCount());
}

void CTDLTaskCtrlBaseTest::PopulateLongestMap(const CToDoCtrlData& data, CTDCLongestItemMap& mapLongest) const
{
	CTDCTaskFormatter formatter(data);

	DWORD dwMaxTaskID = data.GetTaskCount();

	for (DWORD dwTaskID = 1; dwTaskID <= dwMaxTaskID; dwTaskID++)
	{
		const TODOITEM* pTDI = NULL;
		const TODOSTRUCTURE* pTDS = NULL;
		VERIFY(data.GetTask(dwTaskID, pTDI, pTDS));

		// Attributes not affected by subtasks
		mapLongest.CheckUpdateValue(TDCC_ALLOCTO, pTDI->aAllocTo);
		mapLongest.CheckUpdateValue(TDCC_CATEGORY, pTDI->aCategories);
		mapLongest.CheckUpdateValue(TDCC_TAGS, pTDI->aTags);
		mapLongest.CheckUpdateValue(TDCC_ALLOCBY, pTDI->sAllocBy);
		mapLongest.CheckUpdateValue(TDCC_STATUS, pTDI->sStatus);
		mapLongest.CheckUpdateValue(TDCC_VERSION, pTDI->sVersion);
		mapLongest.CheckUpdateValue(TDCC_EXTERNALID, pTDI->sExternalID);
		mapLongest.CheckUpdateValue(TDCC_CREATEDBY, pTDI->sCreatedBy);
		mapLongest.CheckUpdateValue(TDCC_LASTMODBY, pTDI->sLastModifiedBy);
		mapLongest.CheckUpdateValue(TDCC_RECURRENCE, pTDI->trRecurrence.GetRegularityText(FALSE));

		// Attributes dependent on subtask values
		mapLongest.UpdateValue(TDCC_PATH, formatter.GetTaskPath(pTDI, pTDS));
		mapLongest.UpdateValue(TDCC_POSITION, formatter.GetTaskPosition(pTDS));
		mapLongest.UpdateValue(TDCC_SUBTASKDONE, formatter.GetTaskSubtaskCompletion(pTDI, pTDS));
		mapLongest.UpdateValue(TDCC_COST, formatter.GetTaskCost(pTDI, pTDS));
		mapLongest.UpdateValue(TDCC_TIMEESTIMATE, formatter.GetTaskTimeEstimate(pTDI, pTDS));
		mapLongest.UpdateValue(TDCC_TIMESPENT, formatter.GetTaskTimeSpent(pTDI, pTDS));
		mapLongest.UpdateValue(TDCC_TIMEREMAINING, formatter.GetTaskTimeRemaining(pTDI, pTDS));

		// Note: Priority, Risk and ALL Dates have fixed width columns and 
		// so do not depend on the tasks themselves
	}
}

