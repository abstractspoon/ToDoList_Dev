// TaskFileTest.cpp: implementation of the CTaskFileTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "TaskFileTest.h"

#include "..\todolist\tdcstruct.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

int CTaskFileTest::NUM_TESTLEVELS = 5;
int CTaskFileTest::MAX_TESTLEVELS = 5;

//////////////////////////////////////////////////////////////////////

const int NUM_GLOBAL_STRINGS = 100;
const int NUM_TASK_STRINGS = 10;

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

	tasks.Reset();
	AddGlobalsToTasklist(tasks);
	
	Add10TasksToHierarchy(tasks, NULL, 1, nNumLevels);
}

void CTaskFileTest::Add10TasksToHierarchy(CTaskFile& tasks, HTASKITEM hParentTask, int nLevel, int nNumLevels)
{
	if (nLevel > nNumLevels)
		return;

	// Note: there's no benefit to using 'NewSiblingTask' for so few tasks
	for (int i = 0; i < 10; i++)
	{
		HTASKITEM hTask = tasks.NewTask(Misc::Format(_T("Task_%d"), i), hParentTask, 0, 0, TRUE);

		PopulateNumericTaskAttributes(tasks, hTask);
		PopulateStringTaskAttributes(tasks, hTask, NUM_TASK_STRINGS);

		// Add next level of tasks
		Add10TasksToHierarchy(tasks, hTask, nLevel + 1, nNumLevels);
	}
}

void CTaskFileTest::PopulateFlatList(CTaskFile& tasks, int nNumTasks)
{
	tasks.Reset();
	AddGlobalsToTasklist(tasks);
	
	// Create tasks
	for (int i = 0; i < nNumTasks; i++)
	{
		HTASKITEM hTask = tasks.NewTask(Misc::Format(_T("Task_%d"), i), NULL, 0, 0, TRUE);

		PopulateNumericTaskAttributes(tasks, hTask);
		PopulateStringTaskAttributes(tasks, hTask, NUM_TASK_STRINGS);
	}
}

void CTaskFileTest::AddGlobalsToTasklist(CTaskFile& tasks)
{
	TDCAUTOLISTDATA tld;

	tld.aAllocBy.SetSize(NUM_GLOBAL_STRINGS);
	tld.aAllocTo.SetSize(NUM_GLOBAL_STRINGS);
	tld.aCategory.SetSize(NUM_GLOBAL_STRINGS);
	tld.aStatus.SetSize(NUM_GLOBAL_STRINGS);
	tld.aTags.SetSize(NUM_GLOBAL_STRINGS);
	tld.aVersion.SetSize(NUM_GLOBAL_STRINGS);

	for (int i = 0; i < NUM_GLOBAL_STRINGS; i++)
	{
		tld.aAllocBy[i] = Misc::Format(_T("AllocBy_%d"), i + 1);
		tld.aAllocTo[i] = Misc::Format(_T("AllocTo_%d"), i + 1);
		tld.aCategory[i] = Misc::Format(_T("Category_%d"), i + 1);
		tld.aStatus[i] = Misc::Format(_T("Status_%d"), i + 1);
		tld.aTags[i] = Misc::Format(_T("Tag_%d"), i + 1);
		tld.aVersion[i] = Misc::Format(_T("Version_%d"), i + 1);
	}

	tasks.SetAutoListData(tld);
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

void CTaskFileTest::PopulateStringTaskAttributes(CTaskFile& tasks, HTASKITEM hTask, int nNumMultiAttrib)
{
	{
		CStringArray aValues;
		PopulateArrayWithRandomStrings(aValues, nNumMultiAttrib, _T("AllocTo_%d"));
		tasks.SetTaskAllocatedTo(hTask, aValues);
	}

	{
		CStringArray aValues;
		PopulateArrayWithRandomStrings(aValues, nNumMultiAttrib, _T("Category_%d"));
		tasks.SetTaskCategories(hTask, aValues);
	}

	{
		CStringArray aValues;
		PopulateArrayWithRandomStrings(aValues, nNumMultiAttrib, _T("Tags_%d"));
		tasks.SetTaskTags(hTask, aValues);
	}

	int i = (rand() % NUM_GLOBAL_STRINGS);

	tasks.SetTaskAllocatedBy(hTask, Misc::Format(_T("AllocBy_%d"), i + 1));
	tasks.SetTaskStatus(hTask, Misc::Format(_T("Status_%d"), i + 1));
	tasks.SetTaskVersion(hTask, Misc::Format(_T("Version_%d"), i + 1));
}

void CTaskFileTest::PopulateArrayWithRandomStrings(CStringArray& aValues, int nCount, LPCTSTR szFormat)
{
	aValues.SetSize(nCount);

	while (nCount--)
	{
		int i = (rand() % NUM_GLOBAL_STRINGS);
		aValues[nCount] = Misc::Format(szFormat, i + 1);
	}
}

