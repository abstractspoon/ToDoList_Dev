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

}

CToDoCtrlDataTest::~CToDoCtrlDataTest()
{

}

TESTRESULT CToDoCtrlDataTest::Run()
{
	ClearTotals();

	// Initialise styles and custom attributes
	// TODO

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

	for (int nNumLevels = 2; nNumLevels <= 4; nNumLevels++)
	{
		CTaskFile tasks;
		CTaskFileTest(m_utils).PopulateHierarchy(tasks, nNumLevels);

		CToDoCtrlData data(m_aStyles, m_aCustomAttribDefs);

		TestDataModelCreationPerformance(tasks, data, _T("nested"));
		//TestDataModelCalculationPerformance(data, _T("nested"));
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

	for (int nNumLevels = 2, nNumTasks = 10; nNumLevels <= 4; nNumLevels++)
	{
		// Numbers to match hierarchical test
		nNumTasks += (int)pow(10, nNumLevels);

		CTaskFile tasks;
		CTaskFileTest(m_utils).PopulateFlatList(tasks, nNumTasks);

		CToDoCtrlData data(m_aStyles, m_aCustomAttribDefs);

		TestDataModelCreationPerformance(tasks, data, _T("flat"));
		//TestDataModelCalculationPerformance(data, _T("flat"));
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

	BeginTest(_T("CToDoCtrlDataTest::DataModelCalculationPerformance"));

	DWORD dwTickStart = GetTickCount();

	CTDCTaskCalculator calc(data);
	DWORD dwTaskID = (data.GetTaskCount() + 1);

	while (dwTaskID--)
	{
		// TODO
	}

	DWORD dwDuration = (GetTickCount() - dwTickStart);
	_tprintf(_T("Test took %ld ms to perform calculations on %d %s tasks\n"), dwDuration, data.GetTaskCount(), szTaskType);

	EndTest();
}


