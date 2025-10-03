// TaskFileTest.cpp: implementation of the CTaskFileTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "TaskFileTest.h"

#include "..\todolist\tdcstruct.h"
#include "..\todolist\todoitem.h"

#include "..\shared\DateHelper.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const LPCTSTR NO_ATTRIBUTES = _T("na");

//////////////////////////////////////////////////////////////////////

const int MAX_GLOBAL_STRINGS = 100;
const int MAX_TASK_STRINGS = 10;

//////////////////////////////////////////////////////////////////////

static const CTDCCustomAttribDefinitionArray EMPTY_CUSTATTRIB;

//////////////////////////////////////////////////////////////////////

#define TESTMERGE(from, cmp, att, val, flag) \
			tasksSrc.MergeTaskAttributes(from, tdiDestCopy, att, CTDCCustomAttribDefinitionArray(), flag); \
			ExpectTrue(tdiDestCopy.val == cmp.val);

#define TESTMERGEARR(from, cmp, att, arr, match, flag) \
			tasksSrc.MergeTaskAttributes(from, tdiDestCopy, att, CTDCCustomAttribDefinitionArray(), flag); \
			ExpectTrue(Misc::match(tdiDestCopy.arr, cmp.arr, FALSE));

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaskFileTest::CTaskFileTest(const CTestUtils& utils) 
	: 
	CTDLTestBase(_T("CTaskFileTest"), utils),
	NUM_PERFTESTLEVELS(utils.HasCommandlineFlag(NO_ATTRIBUTES) ? 5 : 4),
	m_bWantPerformanceAttributes(!utils.HasCommandlineFlag(NO_ATTRIBUTES))
{
	CoInitialize(NULL);
}

CTaskFileTest::~CTaskFileTest()
{

}

TESTRESULT CTaskFileTest::Run()
{
	ClearTotals();

	TestMergeTaskAttributesOverwriteAll();
	TestMergeTaskAttributesExcludingEmptySrcValues();
	TestMergeTaskAttributesPreservingNonEmptyDestValues();
	TestMergeTaskAttributesPreservingNonEmptyDestValuesAndExcludingEmptySrcValues();
	TestSetGetCustomComments();

	// -----------------------------------------------------

	TestHierarchyConstructionPerformance();
	TestFlatListConstructionPerformance();

	return GetTotals();
}

void CTaskFileTest::TestMergeTaskAttributesOverwriteAll()
{
	CTDCScopedTest test(*this, _T("CTaskFile::MergeTaskAttributes(OverwriteAll)"));

	CTaskFile tasksSrc;
	HTASKITEM hSrcEmpty = NULL, hSrcFull = NULL;
	TODOITEM tdiSrcEmpty, tdiSrcFull, tdiDestFull;

	PrepareMergeTestTasks(tasksSrc, hSrcEmpty, hSrcFull, tdiSrcFull, tdiDestFull);

	// Test merging of all attributes -----------------------------------------------------
	{
		// Empty source values into non-empty destination -> destination == source
		{
			TODOITEM tdiDestCopy(tdiDestFull);
			tasksSrc.MergeTaskAttributes(hSrcEmpty, tdiDestCopy, TDLMTA_OVERWRITEALL);

			ExpectTrue(tdiDestCopy == tdiSrcEmpty);
		}

		// Full source values into non-empty destination -> destination == source
		{
			TODOITEM tdiDestCopy(tdiDestFull);
			tasksSrc.MergeTaskAttributes(hSrcFull, tdiDestCopy, TDLMTA_OVERWRITEALL);

			ExpectTrue(tdiDestCopy == tdiSrcFull);
		}
	}

	// Test merging of some attributes -----------------------------------------------------
	{
		CTDCAttributeMap mapMerge, mapRest;
		PopulateMergeAttributeMaps(mapMerge, mapRest);

		// Empty source values into non-empty destination -> destination == source
		{
			TODOITEM tdiDestCopy(tdiDestFull);
			tasksSrc.MergeTaskAttributes(hSrcEmpty, tdiDestCopy, mapMerge, EMPTY_CUSTATTRIB, TDLMTA_OVERWRITEALL);

			ExpectTrue(tdiDestCopy.MatchAll(tdiSrcEmpty, mapMerge));
			ExpectTrue(tdiDestCopy.MatchAll(tdiDestFull, mapRest));
		}

		// Full source values into non-empty destination
		{
			TODOITEM tdiDestCopy(tdiDestFull);
			tasksSrc.MergeTaskAttributes(hSrcFull, tdiDestCopy, mapRest, EMPTY_CUSTATTRIB, TDLMTA_OVERWRITEALL);

			ExpectTrue(tdiDestCopy.MatchAll(tdiSrcFull, mapRest));
			ExpectTrue(tdiDestCopy.MatchAll(tdiDestFull, mapMerge));
		}
	}
}

void CTaskFileTest::TestMergeTaskAttributesExcludingEmptySrcValues()
{
	CTDCScopedTest test(*this, _T("CTaskFile::MergeTaskAttributes(ExcludeEmptySrcValues)"));

	CTaskFile tasksSrc;
	HTASKITEM hSrcEmpty = NULL, hSrcFull = NULL;
	TODOITEM tdiSrcFull, tdiDestFull;

	PrepareMergeTestTasks(tasksSrc, hSrcEmpty, hSrcFull, tdiSrcFull, tdiDestFull);

	// Test merging of all attributes -----------------------------------------------------
	{
		// Empty source values into non-empty destination -> No change
		{
			TODOITEM tdiDestCopy(tdiDestFull);
			tasksSrc.MergeTaskAttributes(hSrcEmpty, tdiDestCopy, TDLMTA_EXCLUDEEMPTYSOURCEVALUES);

			ExpectTrue(tdiDestCopy == tdiDestFull);
		}

		// Full source values into non-empty destination -> destination == source
		{
			TODOITEM tdiDestCopy(tdiDestFull);
			tasksSrc.MergeTaskAttributes(hSrcFull, tdiDestCopy, TDLMTA_EXCLUDEEMPTYSOURCEVALUES);

			ExpectTrue(tdiDestCopy == tdiSrcFull);
		}
	}

	// Test merging of some attributes -----------------------------------------------------
	{
		CTDCAttributeMap mapMerge, mapRest;
		PopulateMergeAttributeMaps(mapMerge, mapRest);

		// Empty source values into non-empty destination -> No change
		{
			TODOITEM tdiDestCopy(tdiDestFull);
			tasksSrc.MergeTaskAttributes(hSrcEmpty, tdiDestCopy, mapRest, EMPTY_CUSTATTRIB, TDLMTA_EXCLUDEEMPTYSOURCEVALUES);

			ExpectTrue(tdiDestCopy == tdiDestFull);
		}

		// Full source values into non-empty destination
		{
			TODOITEM tdiDestCopy(tdiDestFull);
			tasksSrc.MergeTaskAttributes(hSrcFull, tdiDestCopy, mapMerge, EMPTY_CUSTATTRIB, TDLMTA_EXCLUDEEMPTYSOURCEVALUES);

			ExpectTrue(tdiDestCopy.MatchAll(tdiSrcFull, mapMerge));
			ExpectTrue(tdiDestCopy.MatchAll(tdiDestFull, mapRest));
		}
	}
}

void CTaskFileTest::TestMergeTaskAttributesPreservingNonEmptyDestValues()
{
	CTDCScopedTest test(*this, _T("CTaskFile::MergeTaskAttributes(PreserveNonEmptyDestValues)"));

	CTaskFile tasksSrc;
	HTASKITEM hSrcEmpty = NULL, hSrcFull = NULL;
	TODOITEM tdiSrcFull, tdiDestEmpty, tdiDestFull;

	PrepareMergeTestTasks(tasksSrc, hSrcEmpty, hSrcFull, tdiSrcFull, tdiDestFull);
	tdiDestEmpty.dateCreated = tdiDestFull.dateCreated; // else the comparisons will fail incorrectly

	// Test merging of all attributes -----------------------------------------------------
	{
		// Empty source values into non-empty destination -> No change
		{
			TODOITEM tdiDestCopy(tdiDestFull);
			tasksSrc.MergeTaskAttributes(hSrcEmpty, tdiDestCopy, TDLMTA_PRESERVENONEMPTYDESTVALUES);

			ExpectTrue(tdiDestCopy == tdiDestFull);
		}

		// Full source values into non-empty destination -> no change
		{
			TODOITEM tdiDestCopy(tdiDestFull);
			tasksSrc.MergeTaskAttributes(hSrcFull, tdiDestCopy, TDLMTA_PRESERVENONEMPTYDESTVALUES);

			ExpectTrue(tdiDestCopy == tdiDestFull);
		}

		// Full source values into empty destination -> destination == source
		{
			TODOITEM tdiDestCopy(tdiDestEmpty);
			tasksSrc.MergeTaskAttributes(hSrcFull, tdiDestCopy, TDLMTA_PRESERVENONEMPTYDESTVALUES);

			ExpectTrue(tdiDestCopy == tdiSrcFull);
		}
	}

	// Test merging of some attributes -----------------------------------------------------
	{
		CTDCAttributeMap mapMerge, mapRest;
		PopulateMergeAttributeMaps(mapMerge, mapRest);

		// Empty source values into non-empty destination -> No change
		{
			TODOITEM tdiDestCopy(tdiDestFull);
			tasksSrc.MergeTaskAttributes(hSrcEmpty, tdiDestCopy, mapRest, EMPTY_CUSTATTRIB, TDLMTA_PRESERVENONEMPTYDESTVALUES);

			ExpectTrue(tdiDestCopy == tdiDestFull);
		}

		// Full source values into non-empty destination -> no change
		{
			TODOITEM tdiDestCopy(tdiDestFull);
			tasksSrc.MergeTaskAttributes(hSrcFull, tdiDestCopy, mapMerge, EMPTY_CUSTATTRIB, TDLMTA_PRESERVENONEMPTYDESTVALUES);

			ExpectTrue(tdiDestCopy == tdiDestFull);
		}

		// Full source values into empty destination -> destination == source
		{
			TODOITEM tdiDestCopy(tdiDestEmpty);
			tasksSrc.MergeTaskAttributes(hSrcFull, tdiDestCopy, mapMerge, EMPTY_CUSTATTRIB, TDLMTA_PRESERVENONEMPTYDESTVALUES);

			ExpectTrue(tdiDestCopy.MatchAll(tdiSrcFull, mapMerge));
			ExpectTrue(tdiDestCopy.MatchAll(tdiDestEmpty, mapRest));
		}
	}
}

void CTaskFileTest::TestMergeTaskAttributesPreservingNonEmptyDestValuesAndExcludingEmptySrcValues()
{
	CTDCScopedTest test(*this, _T("CTaskFile::MergeTaskAttributes(PreserveNonEmptyDestValuesAndExcludeEmptySrcValues)"));

	CTaskFile tasksSrc;
	HTASKITEM hSrcEmpty = NULL, hSrcFull = NULL;
	TODOITEM tdiSrcFull, tdiDestEmpty, tdiDestFull;

	PrepareMergeTestTasks(tasksSrc, hSrcEmpty, hSrcFull, tdiSrcFull, tdiDestFull);
	tdiDestEmpty.dateCreated = tdiDestFull.dateCreated; // else the comparisons will fail incorrectly

	// Test merging of all attributes -----------------------------------------------------
	{
		// Test merging empty source values into non-empty destination -> No change
		{
			TODOITEM tdiDestCopy(tdiDestFull);
			tasksSrc.MergeTaskAttributes(hSrcEmpty, tdiDestCopy, TDLMTA_PRESERVENONEMPTYDESTVALUES | TDLMTA_EXCLUDEEMPTYSOURCEVALUES);

			ExpectTrue(tdiDestCopy == tdiDestFull);
		}

		// Test merging non-empty source values into non-empty destination -> no change
		{
			TODOITEM tdiDestCopy(tdiDestFull);
			tasksSrc.MergeTaskAttributes(hSrcFull, tdiDestCopy, TDLMTA_PRESERVENONEMPTYDESTVALUES | TDLMTA_EXCLUDEEMPTYSOURCEVALUES);

			ExpectTrue(tdiDestCopy == tdiDestFull);
		}

		// Test merging non-empty source values into empty destination -> destination == source
		{
			TODOITEM tdiDestCopy(tdiDestEmpty);
			tasksSrc.MergeTaskAttributes(hSrcFull, tdiDestCopy, TDLMTA_PRESERVENONEMPTYDESTVALUES | TDLMTA_EXCLUDEEMPTYSOURCEVALUES);

			ExpectTrue(tdiDestCopy == tdiSrcFull);
		}
	}
}

void CTaskFileTest::TestSetGetCustomComments()
{
	// A very specific test when custom comments of odd character lengths
	// because the conversion to Unicode and back will add a character
	// to the original byte length
	CTDCScopedTest test(*this, _T("CTaskFile::SetGetCustomComments"));

	CTaskFile tasks;
	HTASKITEM hTask = tasks.NewTask(_T(""), NULL, 0, 0);

	ExpectTrue(hTask != NULL);

	const LPCSTR szContent =
		"[ColouredParagraphs.rtf](file:///C:/Users/Daniel%20G/Documents/ColouredParagraphs.rtf)\n\n"
		"![singleclickediting.png](file:///C:/Users/Daniel%20G/Pictures/singleclickediting.png)\n";

	const int nLenContent = strlen(szContent);
	ExpectEQ((nLenContent % 2), 1);

	CBinaryData dataIn((PBYTE)szContent, nLenContent);
	ExpectTrue(tasks.SetTaskCustomComments(hTask, dataIn, _T("Test")));

	CBinaryData dataOut;
	CString sType;

	ExpectTrue(tasks.GetTaskCustomComments(hTask, dataOut, sType));
	ExpectEQ(dataOut.AsString(), dataIn.AsString());
}

void CTaskFileTest::PrepareMergeTestTasks(CTaskFile& tasksSrc, HTASKITEM& hSrcEmpty, HTASKITEM& hSrcFull, 
										   TODOITEM& tdiSrcFull, TODOITEM& tdiDestFull)
{
	// Initialise empty and full source tasks
	hSrcEmpty = tasksSrc.NewTask(_T(""), NULL, 0, 0);
	hSrcFull = tasksSrc.NewSiblingTask(_T(""), hSrcEmpty, 1, TRUE);

	tasksSrc.SetTaskTitle(hSrcFull, _T("Src.FullTask"));
	tasksSrc.SetTaskComments(hSrcFull, _T("Src.Comments"));
	tasksSrc.SetTaskAllocatedBy(hSrcFull, _T("Src.AllocBy"));
	tasksSrc.SetTaskStatus(hSrcFull, _T("Src.Status"));
	tasksSrc.SetTaskVersion(hSrcFull, _T("Src.Version"));
	tasksSrc.SetTaskExternalID(hSrcFull, _T("Src.ExternalID"));

	tasksSrc.SetTaskPriority(hSrcFull, 5);
	tasksSrc.SetTaskRisk(hSrcFull, 3);
	tasksSrc.SetTaskColor(hSrcFull, RGB(255, 0, 0));
	tasksSrc.SetTaskCost(hSrcFull, 5.67);
	tasksSrc.SetTaskTimeEstimate(hSrcFull, 2.34, TDCU_WEEKS);
	tasksSrc.SetTaskTimeSpent(hSrcFull, 1.23, TDCU_HOURS);

	tasksSrc.AddTaskAllocatedTo(hSrcFull, _T("Src.AllocTo"));
	tasksSrc.AddTaskCategory(hSrcFull, _T("Src.Category"));
	tasksSrc.AddTaskTag(hSrcFull, _T("Src.Tag"));
	tasksSrc.AddTaskDependency(hSrcFull, _T("Src.Dependency"));
	tasksSrc.AddTaskFileLink(hSrcFull, _T("Src.FileLink"));

	tasksSrc.SetTaskCreationDate(hSrcFull, COleDateTime(CDateHelper::GetDate(DHD_NOW)));
	tasksSrc.SetTaskStartDate(hSrcFull, COleDateTime(CDateHelper::GetDate(DHD_YESTERDAY)));
	tasksSrc.SetTaskDueDate(hSrcFull, COleDateTime(CDateHelper::GetDate(DHD_TOMORROW)));
	tasksSrc.SetTaskDoneDate(hSrcFull, COleDateTime(CDateHelper::GetDate(DHD_TODAY)));
	tasksSrc.SetTaskLastModified(hSrcFull, COleDateTime(CDateHelper::GetDate(DHD_TODAY)), _T("Src.ModifiedBy"));

	tasksSrc.MergeTaskAttributes(hSrcFull, tdiSrcFull, TDLMTA_OVERWRITEALL);
	
	// Initialise full destination task
	tdiDestFull.sTitle = _T("Dest.FullTask");
	tdiDestFull.sComments = _T("Dest.Comments");
	tdiDestFull.sAllocBy = _T("Dest.AllocBy");
	tdiDestFull.sStatus = _T("Dest.Status");
	tdiDestFull.sVersion = _T("Dest.Version");
	tdiDestFull.sExternalID = _T("Dest.ExternalID");
	tdiDestFull.sLastModifiedBy = _T("Dest.ModifiedBy");

	tdiDestFull.nPriority = 4;
	tdiDestFull.nRisk = 7;
	tdiDestFull.color = RGB(0, 255, 0);
	tdiDestFull.cost.dAmount = 3.45;
	tdiDestFull.timeEstimate = TDCTIMEPERIOD(8.90, TDCU_MINS);
	tdiDestFull.timeSpent = TDCTIMEPERIOD(7.89, TDCU_MONTHS);

	tdiDestFull.aAllocTo.Add(_T("Dest.AllocTo"));
	tdiDestFull.aCategories.Add(_T("Dest.Category"));
	tdiDestFull.aTags.Add(_T("Dest.Tag"));
	tdiDestFull.aDependencies.Add(_T("Dest.Dependency"));
	tdiDestFull.aFileLinks.Add(_T("Dest.FileLink"));

	tdiDestFull.dateCreated = tdiSrcFull.dateCreated;
	tdiDestFull.dateStart = CDateHelper::GetDate(DHD_YESTERDAY);
	tdiDestFull.dateDue = CDateHelper::GetDate(DHD_TOMORROW);
	tdiDestFull.dateDone = CDateHelper::GetDate(DHD_TODAY);
	tdiDestFull.dateLastMod = CDateHelper::GetDate(DHD_TODAY);
}

void CTaskFileTest::PopulateMergeAttributeMaps(CTDCAttributeMap& mapMerge, CTDCAttributeMap& mapRest)
{
	mapMerge.RemoveAll();
	mapRest.RemoveAll();

	mapMerge.Add(TDCA_TASKNAME);
	mapMerge.Add(TDCA_ALLOCBY);
	mapMerge.Add(TDCA_VERSION);
	mapMerge.Add(TDCA_LASTMODBY);
	mapMerge.Add(TDCA_RISK);
	mapMerge.Add(TDCA_COLOR);
	mapMerge.Add(TDCA_TIMEESTIMATE);
	mapMerge.Add(TDCA_ALLOCTO);
	mapMerge.Add(TDCA_TAGS);
	mapMerge.Add(TDCA_FILELINK);
	mapMerge.Add(TDCA_STARTDATE);
	mapMerge.Add(TDCA_DONEDATE);

	mapRest.Add(TDCA_COMMENTS);
	mapRest.Add(TDCA_STATUS);
	mapRest.Add(TDCA_EXTERNALID);
	mapRest.Add(TDCA_PRIORITY);
	mapRest.Add(TDCA_COST);
	mapRest.Add(TDCA_TIMESPENT);
	mapRest.Add(TDCA_CATEGORY);
	mapRest.Add(TDCA_DEPENDENCY);
	mapRest.Add(TDCA_CREATIONDATE);
	mapRest.Add(TDCA_DUEDATE);
	mapRest.Add(TDCA_LASTMODDATE);
}

void CTaskFileTest::BeginPerformanceTest(LPCTSTR szFunction)
{
	CString sTest;

	sTest += _T("CTaskFileTest::");
	sTest += szFunction;

	if (m_bWantPerformanceAttributes)
		sTest += _T("(WITH attributes)");
	else
		sTest += _T("(WITHOUT attributes)");

	CTDLTestBase::BeginTest(sTest);
}

void CTaskFileTest::TestHierarchyConstructionPerformance()
{
	if (!m_utils.GetWantPerformanceTests())
	{
		_tprintf(_T("Add '-p' to run CTaskFileTest::HierarchyConstructionPerformance\n"));
		return;
	}

	BeginPerformanceTest(_T("HierarchyConstructionPerformance"));

	for (int nNumLevels = 2; nNumLevels <= NUM_PERFTESTLEVELS; nNumLevels++)
	{
		DWORD dwTickStart = GetTickCount();

		// Populate
		CTaskFile tasks;
		
		PopulateHierarchy(tasks, nNumLevels, (m_bWantPerformanceAttributes ? TDCA_ALL : TDCA_NONE));
		OutputElapsedTime(tasks, dwTickStart, _T("build"), _T("nested"));

		// Save and reload
		CString sFilePath = Misc::Format(_T("TestTasklist_Tree_%d.tdl"), tasks.GetTaskCount());

		TestSaveTasklist(tasks, sFilePath, _T("nested"));
		TestLoadTasklist(sFilePath, _T("nested"));
	}
	
	EndTest();
}

void CTaskFileTest::TestFlatListConstructionPerformance()
{
	if (!m_utils.GetWantPerformanceTests())
	{
		_tprintf(_T("Add '-p' to run CTaskFileTest::FlatListConstructionPerformance\n"));
		return;
	}

	BeginPerformanceTest(_T("TestFlatListConstructionPerformance"));

	for (int nNumLevels = 2, nNumTasks = 10; nNumLevels <= NUM_PERFTESTLEVELS; nNumLevels++)
	{
		DWORD dwTickStart = GetTickCount();

		// Numbers to match hierarchical test
		nNumTasks += (int)pow(10, nNumLevels);

		// Populate
		CTaskFile tasks;

		PopulateFlatList(tasks, nNumTasks, (m_bWantPerformanceAttributes ? TDCA_ALL : TDCA_NONE));
		OutputElapsedTime(tasks, dwTickStart, _T("build"), _T("flat"));

		// Save and reload
		CString sFilePath = Misc::Format(_T("TestTasklist_Flat_%d.tdl"), tasks.GetTaskCount());

		TestSaveTasklist(tasks, sFilePath, _T("flat"));
		TestLoadTasklist(sFilePath, _T("flat"));
	}

	EndTest();
}

void CTaskFileTest::OutputElapsedTime(const CTaskFile& tasks, DWORD dwTickStart, LPCTSTR szOperation, LPCTSTR szType)
{
	DWORD dwDuration = (GetTickCount() - dwTickStart);

	_tprintf(_T("Test took %ld ms to %s a tasklist with %d %s tasks (%.1f ms/100)"),
			 dwDuration,
			 szOperation,
			 tasks.GetTaskCount(),
			 szType,
			 (dwDuration * 100.0) / tasks.GetTaskCount());
	_tprintf(_T("\n"));
}

void CTaskFileTest::TestSaveTasklist(CTaskFile& tasks, LPCTSTR szFilePath, LPCTSTR szType)
{
	DWORD dwTickStart = GetTickCount();
	
	tasks.SetXmlHeader(DEFAULT_UNICODE_HEADER);
	tasks.Save(szFilePath, SFEF_UTF16);
	
	OutputElapsedTime(tasks, dwTickStart, _T("save"), szType);
}

void CTaskFileTest::TestLoadTasklist(LPCTSTR szFilePath, LPCTSTR szType)
{
	DWORD dwTickStart = GetTickCount();
	
	CTaskFile tasks;
	tasks.Load(szFilePath);
				
	OutputElapsedTime(tasks, dwTickStart, _T("load"), szType);
}

void CTaskFileTest::PopulateHierarchy(CTaskFile& tasks, int nNumLevels, const CTDCAttributeMap& mapAttrib) const
{
	ASSERT(nNumLevels > 0 && nNumLevels <= NUM_PERFTESTLEVELS);

	tasks.Reset();
	AddGlobalsToTasklist(tasks, mapAttrib);
	
	Add10TasksToHierarchy(tasks, NULL, 1, nNumLevels, mapAttrib);
}

void CTaskFileTest::Add10TasksToHierarchy(CTaskFile& tasks, HTASKITEM hParentTask, int nLevel, int nNumLevels, const CTDCAttributeMap& mapAttrib)
{
	if (nLevel > nNumLevels)
		return;

	HTASKITEM hPrevSiblingTask = NULL, hTask = NULL;

	for (int i = 0; i < 10; i++)
	{
		if (hPrevSiblingTask == NULL)
			hTask = tasks.NewTask(Misc::Format(_T("Task_%d"), i), hParentTask, 0, 0, TRUE);
		else
			hTask = tasks.NewSiblingTask(Misc::Format(_T("Task_%d"), i), hPrevSiblingTask, 0, TRUE);

		PopulateNumericTaskAttributes(tasks, hTask, mapAttrib);
		PopulateStringTaskAttributes(tasks, hTask, MAX_TASK_STRINGS, mapAttrib);

		// Add next level of tasks
		Add10TasksToHierarchy(tasks, hTask, nLevel + 1, nNumLevels, mapAttrib);

		hPrevSiblingTask = hTask;
	}
}

void CTaskFileTest::PopulateFlatList(CTaskFile& tasks, int nNumTasks, const CTDCAttributeMap& mapAttrib) const
{
	tasks.Reset();
	AddGlobalsToTasklist(tasks, mapAttrib);
	
	// Create tasks
	HTASKITEM hPrevSiblingTask = NULL, hTask = NULL;

	for (int i = 0; i < nNumTasks; i++)
	{
		if (hPrevSiblingTask == NULL)
			hTask = tasks.NewTask(Misc::Format(_T("Task_%d"), i), NULL, 0, 0, TRUE);
		else
			hTask = tasks.NewSiblingTask(Misc::Format(_T("Task_%d"), i), hPrevSiblingTask, 0, TRUE);

		PopulateNumericTaskAttributes(tasks, hTask, mapAttrib);
		PopulateStringTaskAttributes(tasks, hTask, MAX_TASK_STRINGS, mapAttrib);

		hPrevSiblingTask = hTask;
	}
}

BOOL CTaskFileTest::HasAttribute(const CTDCAttributeMap& mapAttrib, TDC_ATTRIBUTE nAttribID)
{
	return (mapAttrib.Has(TDCA_ALL) || mapAttrib.Has(nAttribID));
}

void CTaskFileTest::AddGlobalsToTasklist(CTaskFile& tasks, const CTDCAttributeMap& mapAttrib)
{
	if (mapAttrib.Has(TDCA_NONE))
		return;

	TDCAUTOLISTDATA tld;

	if (HasAttribute(mapAttrib, TDCA_ALLOCBY))
	{
		tld.aAllocBy.SetSize(MAX_GLOBAL_STRINGS);

		for (int i = 0; i < MAX_GLOBAL_STRINGS; i++)
			tld.aAllocBy[i] = Misc::Format(_T("AllocBy_%d"), i + 1);
	}

	if (HasAttribute(mapAttrib, TDCA_ALLOCTO))
	{
		tld.aAllocTo.SetSize(MAX_GLOBAL_STRINGS);

		for (int i = 0; i < MAX_GLOBAL_STRINGS; i++)
			tld.aAllocTo[i] = Misc::Format(_T("AllocTo_%d"), i + 1);
	}

	if (HasAttribute(mapAttrib, TDCA_CATEGORY))
	{
		tld.aCategory.SetSize(MAX_GLOBAL_STRINGS);

		for (int i = 0; i < MAX_GLOBAL_STRINGS; i++)
			tld.aCategory[i] = Misc::Format(_T("Category_%d"), i + 1);
	}

	if (HasAttribute(mapAttrib, TDCA_STATUS))
	{
		tld.aStatus.SetSize(MAX_GLOBAL_STRINGS);

		for (int i = 0; i < MAX_GLOBAL_STRINGS; i++)
			tld.aStatus[i] = Misc::Format(_T("Status_%d"), i + 1);
	}

	if (HasAttribute(mapAttrib, TDCA_TAGS))
	{
		tld.aTags.SetSize(MAX_GLOBAL_STRINGS);

		for (int i = 0; i < MAX_GLOBAL_STRINGS; i++)
			tld.aTags[i] = Misc::Format(_T("Tag_%d"), i + 1);
	}

	if (HasAttribute(mapAttrib, TDCA_VERSION))
	{
		tld.aVersion.SetSize(MAX_GLOBAL_STRINGS);

		for (int i = 0; i < MAX_GLOBAL_STRINGS; i++)
			tld.aVersion[i] = Misc::Format(_T("Version_%d"), i + 1);
	}

	tasks.SetAutoListData(tld);
}

void CTaskFileTest::PopulateNumericTaskAttributes(CTaskFile& tasks, HTASKITEM hTask, const CTDCAttributeMap& mapAttrib)
{
	if (mapAttrib.Has(TDCA_NONE))
		return;
	
	COleDateTime dtNow = COleDateTime::GetCurrentTime();

	if (HasAttribute(mapAttrib, TDCA_PERCENT))
		tasks.SetTaskPercentDone(hTask, rand() % 100);
		
	if (HasAttribute(mapAttrib, TDCA_PRIORITY))
		tasks.SetTaskPriority(hTask, rand() % 10);
	
	if (HasAttribute(mapAttrib, TDCA_RISK))
		tasks.SetTaskRisk(hTask, rand() % 10);
	
	if (HasAttribute(mapAttrib, TDCA_COST))
		tasks.SetTaskCost(hTask, rand() / 1000.0);
	
	if (HasAttribute(mapAttrib, TDCA_DUEDATE))
		tasks.SetTaskDueDate(hTask, COleDateTime(dtNow.m_dt + (rand() % 100)));
	
	if (HasAttribute(mapAttrib, TDCA_STARTDATE))
		tasks.SetTaskStartDate(hTask, COleDateTime(dtNow.m_dt + (rand() % 100)));
	
	if (HasAttribute(mapAttrib, TDCA_LASTMODDATE))
		tasks.SetTaskLastModified(hTask, COleDateTime(dtNow.m_dt + (rand() % 100)), _T(""));
	
	if (HasAttribute(mapAttrib, TDCA_TIMEESTIMATE))
		tasks.SetTaskTimeEstimate(hTask, rand() % 20, TDCU_DAYS);
	
	if (HasAttribute(mapAttrib, TDCA_TIMESPENT))
		tasks.SetTaskTimeSpent(hTask, rand() % 20, TDCU_DAYS);

	if (HasAttribute(mapAttrib, TDCA_DONEDATE) && ((rand() % 10) == 0))
		tasks.SetTaskDoneDate(hTask, COleDateTime(dtNow.m_dt + (rand() % 100)));
}

void CTaskFileTest::PopulateStringTaskAttributes(CTaskFile& tasks, HTASKITEM hTask, int nNumMultiAttrib, const CTDCAttributeMap& mapAttrib)
{
	if (mapAttrib.Has(TDCA_NONE))
		return;
	
	if (HasAttribute(mapAttrib, TDCA_ALLOCTO))
	{
		CStringArray aValues;
		PopulateArrayWithRandomStrings(aValues, nNumMultiAttrib, _T("AllocTo_%d"));
		tasks.SetTaskAllocatedTo(hTask, aValues);
	}

	if (HasAttribute(mapAttrib, TDCA_CATEGORY))
	{
		CStringArray aValues;
		PopulateArrayWithRandomStrings(aValues, nNumMultiAttrib, _T("Category_%d"));
		tasks.SetTaskCategories(hTask, aValues);
	}

	if (HasAttribute(mapAttrib, TDCA_TAGS))
	{
		CStringArray aValues;
		PopulateArrayWithRandomStrings(aValues, nNumMultiAttrib, _T("Tags_%d"));
		tasks.SetTaskTags(hTask, aValues);
	}

	int i = (rand() % MAX_GLOBAL_STRINGS);

	if (HasAttribute(mapAttrib, TDCA_ALLOCBY))
		tasks.SetTaskAllocatedBy(hTask, Misc::Format(_T("AllocBy_%d"), i + 1));
	
	if (HasAttribute(mapAttrib, TDCA_STATUS))
		tasks.SetTaskStatus(hTask, Misc::Format(_T("Status_%d"), i + 1));

	if (HasAttribute(mapAttrib, TDCA_VERSION))
		tasks.SetTaskVersion(hTask, Misc::Format(_T("Version_%d"), i + 1));

	if (HasAttribute(mapAttrib, TDCA_LASTMODBY))
		tasks.SetTaskLastModifiedBy(hTask, Misc::Format(_T("LastModBy_%d"), i + 1));
}

void CTaskFileTest::PopulateArrayWithRandomStrings(CStringArray& aValues, int nCount, LPCTSTR szFormat)
{
	aValues.SetSize(nCount);

	while (nCount--)
	{
		int i = (rand() % MAX_GLOBAL_STRINGS);
		aValues[nCount] = Misc::Format(szFormat, i + 1);
	}
}
