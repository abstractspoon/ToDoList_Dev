// TaskFileTest.cpp: implementation of the CTaskFileTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "TaskFileTest.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaskFileTest::CTaskFileTest(const CTestUtils& utils) : CTDLTestBase(utils)
{

}

CTaskFileTest::~CTaskFileTest()
{

}

TESTRESULT CTaskFileTest::Run()
{
	ClearTotals();

	TestHierarchyConstructionPerformance();
	TestFlatListConstructionPerformance();

	return GetTotals();
}

void CTaskFileTest::TestHierarchyConstructionPerformance()
{
	if (!m_utils.HasCommandlineFlag('p'))
	{
		_tprintf(_T("Add '-p' to run CTaskFileTest::HierarchyConstructionPerformance\n"));
		return;
	}

	BeginTest(_T("CTaskFileTest::HierarchyConstructionPerformance"));

	for (int nNumLevels = 2; nNumLevels <= max(NUM_TESTLEVELS, MAX_TESTLEVELS); nNumLevels++)
	{
		DWORD dwTickStart = GetTickCount();

		CTaskFile tasks;
		PopulateHierarchy(tasks, nNumLevels);
		
		DWORD dwDuration = (GetTickCount() - dwTickStart);
		_tprintf(_T("Test took %ld ms to build a tasklist with %d nested tasks (%.1f ms/100)\n"), 
				 dwDuration, 
				 tasks.GetTaskCount(),
				 (dwDuration * 100.0) / tasks.GetTaskCount());
	}
	
	EndTest();
}

void CTaskFileTest::TestFlatListConstructionPerformance()
{
	if (!m_utils.HasCommandlineFlag('p'))
	{
		_tprintf(_T("Add '-p' to run CTaskFileTest::FlatListConstructionPerformance\n"));
		return;
	}

	BeginTest(_T("CTaskFileTest::FlatListConstructionPerformance"));

	for (int nNumLevels = 2, nNumTasks = 10; nNumLevels <= max(NUM_TESTLEVELS, MAX_TESTLEVELS); nNumLevels++)
	{
		DWORD dwTickStart = GetTickCount();

		// Numbers to match hierarchical test
		nNumTasks += (int)pow(10, nNumLevels);

		CTaskFile tasks;
		PopulateFlatList(tasks, nNumTasks);

		DWORD dwDuration = (GetTickCount() - dwTickStart);
		_tprintf(_T("Test took %ld ms to build a tasklist with %d flat tasks (%.1f ms/100)\n"), 
				 dwDuration, 
				 tasks.GetTaskCount(),
				 (dwDuration * 100.0) / tasks.GetTaskCount());
	}

	EndTest();
}

void CTaskFileTest::PopulateHierarchy(CTaskFile& tasks, int nNumLevels)
{
	ASSERT(nNumLevels > 0 && nNumLevels <= MAX_TESTLEVELS);

	Add10TasksToHierarchy(tasks, NULL, 1, nNumLevels);
}

void CTaskFileTest::Add10TasksToHierarchy(CTaskFile& tasks, HTASKITEM hParentTask, int nLevel, int nNumLevels)
{
	if (nLevel > nNumLevels)
		return;

	HTASKITEM hPrevSiblingTask = NULL, hTask = NULL;

	for (int i = 0; i < 10; i++)
	{
//		if (hPrevSiblingTask == NULL)
			hTask = tasks.NewTask(Misc::Format(_T("Task_%d"), i), NULL, 0, 0, TRUE);
// 		else
// 			hTask = tasks.NewSiblingTask(Misc::Format(_T("Task_%d"), i), hPrevSiblingTask, 0, TRUE);

		hPrevSiblingTask = hTask;

		PopulateNumericTaskAttributes(tasks, hTask);

		// Add next level of tasks
		Add10TasksToHierarchy(tasks, hTask, nLevel + 1, nNumLevels);
	}
}

void CTaskFileTest::PopulateFlatList(CTaskFile& tasks, int nNumTasks)
{
	HTASKITEM hPrevSiblingTask = NULL, hTask = NULL;

	for (int i = 0; i < nNumTasks; i++)
	{
		if (hPrevSiblingTask == NULL)
			hTask = tasks.NewTask(Misc::Format(_T("Task_%d"), i), NULL, 0, 0, TRUE);
		else
			hTask = tasks.NewSiblingTask(Misc::Format(_T("Task_%d"), i), hPrevSiblingTask, 0, TRUE);

		hPrevSiblingTask = hTask;

		PopulateNumericTaskAttributes(tasks, hTask);
	}
}

void CTaskFileTest::PopulateNumericTaskAttributes(CTaskFile& tasks, HTASKITEM hTask)
{
	COleDateTime dtNow = COleDateTime::GetCurrentTime();

	tasks.SetTaskPercentDone(hTask, rand() % 100);
	tasks.SetTaskPriority(hTask, rand() % 10);
	tasks.SetTaskRisk(hTask, rand() % 10);
	tasks.SetTaskCost(hTask, rand() / 1000.0);
	tasks.SetTaskDueDate(hTask, COleDateTime(dtNow.m_dt + (rand() % 100)));
	tasks.SetTaskStartDate(hTask, COleDateTime(dtNow.m_dt + (rand() % 100)));
	tasks.SetTaskLastModified(hTask, COleDateTime(dtNow.m_dt + (rand() % 100)), _T(""));
	tasks.SetTaskTimeEstimate(hTask, rand() % 20, TDCU_DAYS);
	tasks.SetTaskTimeSpent(hTask, rand() % 20, TDCU_DAYS);

	if ((rand() % 10) == 0)
		tasks.SetTaskDoneDate(hTask, COleDateTime(dtNow.m_dt + (rand() % 100)));
}
