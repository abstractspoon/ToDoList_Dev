// ToDoCtrlDataTest.cpp: implementation of the CToDoCtrlDataTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToDoCtrlDataTest.h"

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

}

CToDoCtrlDataTest::~CToDoCtrlDataTest()
{

}

TESTRESULT CToDoCtrlDataTest::Run()
{
	ClearTotals();

	CTDCStyleMap styles;
	CTDCCustomAttribDefinitionArray aCustomAttribDefs;
	// TODO
	
	CTaskFile tasks;
	InitialiseTasks(tasks, 4);

	CToDoCtrlData data(styles, aCustomAttribDefs);

	TestDataModelCreationPerformance(tasks, data);
	TestDataModelCalculationPerformance(data);

	return GetTotals();
}

void CToDoCtrlDataTest::TestDataModelCreationPerformance(const CTaskFile& tasks, CToDoCtrlData& data)
{
	if (!m_utils.HasCommandlineFlag('p'))
	{
		_tprintf(_T("Add '-p' to run CToDoCtrlDataTest::DataModelCreationPerformance\n"));
		return;
	}

	BeginTest(_T("CToDoCtrlDataTest::DataModelCreationPerformance"));

	DWORD dwTickStart = GetTickCount();

	data.BuildDataModel(tasks);

	DWORD dwDuration = (GetTickCount() - dwTickStart);
	_tprintf(_T("Test took %ld ms to build data model with %d nested tasks\n"), dwDuration, data.GetTaskCount());

	EndTest();
}

void CToDoCtrlDataTest::TestDataModelCalculationPerformance(const CToDoCtrlData& data)
{
	if (!m_utils.HasCommandlineFlag('p'))
	{
		_tprintf(_T("Add '-p' to run CToDoCtrlDataTest::DataModelCalculationPerformance\n"));
		return;
	}

	BeginTest(_T("CToDoCtrlDataTest::DataModelCalculationPerformance"));

	DWORD dwTickStart = GetTickCount();

	CTDCTaskCalculator calc(data);
	DWORD dwTaskID = (data.GetTaskCount() + 1);

	while (dwTaskID--)
	{
		// TODO
	}

	DWORD dwDuration = (GetTickCount() - dwTickStart);
	_tprintf(_T("Test took %ld ms to perform calculations on %d nested tasks\n"), dwDuration, data.GetTaskCount());

	EndTest();
}


void CToDoCtrlDataTest::InitialiseTasks(CTaskFile& tasks, int nNumLevels)
{
	ASSERT(nNumLevels > 0 && nNumLevels <= 5);

	BeginTest(_T("CToDoCtrlDataTest::InitialiseTasks"));

	DWORD dwTickStart = GetTickCount();

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

	DWORD dwDuration = (GetTickCount() - dwTickStart);
	_tprintf(_T("Test took %ld ms to create a taskfile with %d nested tasks\n"), dwDuration, tasks.GetTaskCount());

	EndTest();
}

void CToDoCtrlDataTest::PopulateTaskAttributes(CTaskFile& tasks, HTASKITEM hTask)
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
