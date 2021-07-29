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

	for (int nNumLevels = 2; nNumLevels <= 5; nNumLevels++)
	{
		CTaskFile tasks;
		DWORD dwTickStart = GetTickCount();

		PopulateHierarchy(tasks, nNumLevels);
		
		DWORD dwDuration = (GetTickCount() - dwTickStart);
		_tprintf(_T("Test took %ld ms to build a tasklist with %d nested tasks\n"), dwDuration, tasks.GetTaskCount());
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

	for (int nNumLevels = 2, nNumTasks = 10; nNumLevels <= 4/*5*/; nNumLevels++)
	{
		// Numbers to match hierarchical test
		nNumTasks += (int)pow(10, nNumLevels);

		CTaskFile tasks;
		DWORD dwTickStart = GetTickCount();

		PopulateFlatList(tasks, nNumTasks);

		DWORD dwDuration = (GetTickCount() - dwTickStart);
		_tprintf(_T("Test took %ld ms to build a tasklist with %d flat tasks\n"), dwDuration, tasks.GetTaskCount());
	}

	EndTest();
}

void CTaskFileTest::PopulateHierarchy(CTaskFile& tasks, int nNumLevels)
{
	ASSERT(nNumLevels > 0 && nNumLevels <= 5);

	for (int i = 0; i < 10; i++)
	{
		HTASKITEM hTask_I = tasks.NewTask(Misc::Format(_T("Task_%d"), i), NULL, 0, 0, TRUE);
		PopulateTaskAttributes(tasks, hTask_I);

		if (nNumLevels < 2)
			continue;

		DWORD dwTaskID_I = tasks.GetTaskID(hTask_I);
		DWORD dwPrevTaskID_J = 0;

		for (int j = 0; j < 10; j++)
		{
			HTASKITEM hTask_J = tasks.NewTask(Misc::Format(_T("Task_%d"), j), hTask_I, 0, dwTaskID_I, TRUE);
			PopulateTaskAttributes(tasks, hTask_J);

			if (nNumLevels < 3)
				continue;

			DWORD dwTaskID_J = tasks.GetTaskID(hTask_J);

			for (int k = 0; k < 10; k++)
			{
				HTASKITEM hTask_K = tasks.NewTask(Misc::Format(_T("Task_%d"), k), hTask_J, 0, dwTaskID_J, TRUE);
				PopulateTaskAttributes(tasks, hTask_K);

				if (nNumLevels < 4)
					continue;

				DWORD dwTaskID_K = tasks.GetTaskID(hTask_K);

				for (int m = 0; m < 10; m++)
				{
					HTASKITEM hTask_M = tasks.NewTask(Misc::Format(_T("Task_%d"), m), hTask_K, 0, dwTaskID_K, TRUE);
					PopulateTaskAttributes(tasks, hTask_M);

					if (nNumLevels < 5)
						continue;

					DWORD dwTaskID_M = tasks.GetTaskID(hTask_M);

					for (int n = 0; n < 10; n++)
					{
						HTASKITEM hTask_N = tasks.NewTask(Misc::Format(_T("Task_%d"), n), hTask_M, 0, dwTaskID_M, TRUE);
						PopulateTaskAttributes(tasks, hTask_N);
					}
				}
			}
		}
	}
}

void CTaskFileTest::PopulateFlatList(CTaskFile& tasks, int nNumTasks)
{
	for (int i = 0; i < nNumTasks; i++)
	{
		HTASKITEM hTask = tasks.NewTask(Misc::Format(_T("Task_%d"), i), NULL, 0, 0, TRUE);
		PopulateTaskAttributes(tasks, hTask);
	}
}

void CTaskFileTest::PopulateTaskAttributes(CTaskFile& tasks, HTASKITEM hTask)
{
	COleDateTime dtNow = COleDateTime::GetCurrentTime();

	tasks.SetTaskPercentDone(hTask, rand() % 100);
	tasks.SetTaskPriority(hTask, rand() % 10);
	tasks.SetTaskRisk(hTask, rand() % 10);
	tasks.SetTaskCost(hTask, rand() / 1000.0);
	tasks.SetTaskDueDate(hTask, COleDateTime(dtNow.m_dt + (rand() % 100)));
	tasks.SetTaskStartDate(hTask, COleDateTime(dtNow.m_dt + (rand() % 100)));
	tasks.SetTaskLastModified(hTask, COleDateTime(dtNow.m_dt + (rand() % 100)), _T(""));

	if ((rand() % 10) == 0)
		tasks.SetTaskDoneDate(hTask, COleDateTime(dtNow.m_dt + (rand() % 100)));
}
