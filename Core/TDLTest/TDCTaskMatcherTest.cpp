// ToDoCtrlDataTest.cpp: implementation of the CTDCTaskMatcherTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDCTaskMatcherTest.h"

#include "..\todolist\todoctrldata.h"
#include "..\todolist\todoctrldatautils.h"
#include "..\todolist\tdcstruct.h"
#include "..\todolist\todoitem.h"
#include "..\todolist\taskfile.h"

#include "..\shared\datehelper.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const LPCTSTR ID_DATE = _T("Date");
const LPCTSTR ID_INTEGER = _T("Integer");

//////////////////////////////////////////////////////////////////////

// Mocks
const CWnd callback;
const CTDCReminderHelper reminders(callback);
const CContentMgr contentMgr;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCTaskMatcherTest::CTDCTaskMatcherTest(const CTestUtils& utils) 
	: 
	CTDLTestBase(utils)
{
	// Simple custom attributes
	TDCCUSTOMATTRIBUTEDEFINITION attribDef;

	attribDef.sUniqueID = attribDef.sLabel = ID_INTEGER;
	attribDef.SetAttributeType(TDCCA_INTEGER);
	attribDef.dwFeatures = TDCCAF_ACCUMULATE;

	m_aCustomAttribDefs.Add(attribDef);

	attribDef.sUniqueID = attribDef.sLabel = ID_DATE;
	attribDef.SetAttributeType(TDCCA_DATE);
	attribDef.dwFeatures = TDCCAF_MAXIMIZE;

	m_aCustomAttribDefs.Add(attribDef);
}

CTDCTaskMatcherTest::~CTDCTaskMatcherTest()
{

}

TESTRESULT CTDCTaskMatcherTest::Run()
{
	ClearTotals();

	CToDoCtrlData data(m_aStyles, m_aCustomAttribDefs);
	PopulateDataModel(data);

	TestSimpleMatching(data);
	TestComplexMatching(data);

	return GetTotals();
}

void CTDCTaskMatcherTest::TestSimpleMatching(const CToDoCtrlData& data)
{
	CTDCScopedTest test(*this, _T("CTDCTaskMatcherTest::SimpleMatching"));


	// Simple text
	TestSimpleTextMatch(data, TDCA_TASKNAME, _T("Task"));
	TestSimpleTextMatch(data, TDCA_COMMENTS, _T("Comments"));
	TestSimpleTextMatch(data, TDCA_ALLOCBY, _T("AllocBy"));
	TestSimpleTextMatch(data, TDCA_STATUS, _T("Stat"));
	TestSimpleTextMatch(data, TDCA_VERSION, _T("Ver"));
	TestSimpleTextMatch(data, TDCA_EXTERNALID, _T("ExtID"));

	// Special case
// 	TestSimpleTextMatch(data, TDCA_TASKNAMEORCOMMENTS, _T("Task"));
// 	TestSimpleTextMatch(data, TDCA_TASKNAMEORCOMMENTS, _T("Comments"));

	// Array of strings
	TestSimpleTextArrayMatch(data, TDCA_CATEGORY, _T("Cat"));
	TestSimpleTextArrayMatch(data, TDCA_ALLOCTO, _T("AllocTo"));
	TestSimpleTextArrayMatch(data, TDCA_TAGS, _T("Tag"));
	TestSimpleTextArrayMatch(data, TDCA_FILELINK, _T("File"));
}

void CTDCTaskMatcherTest::TestSimpleTextMatch(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, const CString& sPrefix)
{
	CTDCTaskMatcher match(data, reminders, contentMgr);
	CDWordArray aTaskIDs; // get overwritten by each test

	// Equals
	{
		ExpectEQ(1, match.FindTasks(nAttibID, FOP_EQUALS, (sPrefix + _T("2")), FALSE, aTaskIDs));
		ExpectEQ(2UL, aTaskIDs[0]);

		ExpectEQ(0, match.FindTasks(nAttibID, FOP_EQUALS, _T("Equals"), FALSE, aTaskIDs));
		ExpectEQ(0, aTaskIDs.GetSize());
	}

	// Not equals
	{
		ExpectEQ(5, match.FindTasks(nAttibID, FOP_NOT_EQUALS, (sPrefix + _T("5")), FALSE, aTaskIDs));
		ExpectEQ(1UL, aTaskIDs[0]);
		ExpectEQ(2UL, aTaskIDs[1]);
		ExpectEQ(3UL, aTaskIDs[2]);
		ExpectEQ(4UL, aTaskIDs[3]);
		// Task5 matches
		ExpectEQ(6UL, aTaskIDs[4]);

		ExpectEQ(6, match.FindTasks(nAttibID, FOP_NOT_EQUALS, _T("NotEquals"), FALSE, aTaskIDs));
		ExpectEQ(1UL, aTaskIDs[0]);
		ExpectEQ(2UL, aTaskIDs[1]);
		ExpectEQ(3UL, aTaskIDs[2]);
		ExpectEQ(4UL, aTaskIDs[3]);
		ExpectEQ(5UL, aTaskIDs[4]);
		ExpectEQ(6UL, aTaskIDs[5]);
	}

	// Includes
	{
		ExpectEQ(6, match.FindTasks(nAttibID, FOP_INCLUDES, sPrefix, FALSE, aTaskIDs));
		ExpectEQ(1UL, aTaskIDs[0]);
		ExpectEQ(2UL, aTaskIDs[1]);
		ExpectEQ(3UL, aTaskIDs[2]);
		ExpectEQ(4UL, aTaskIDs[3]);
		ExpectEQ(5UL, aTaskIDs[4]);
		ExpectEQ(6UL, aTaskIDs[5]);

		ExpectEQ(0, match.FindTasks(nAttibID, FOP_INCLUDES, _T("Includes"), FALSE, aTaskIDs));
		ExpectEQ(0, aTaskIDs.GetSize());

	}

	// Not includes
	{
		ExpectEQ(0, match.FindTasks(nAttibID, FOP_NOT_INCLUDES, sPrefix, FALSE, aTaskIDs));
		ExpectEQ(0, aTaskIDs.GetSize());

		ExpectEQ(6, match.FindTasks(nAttibID, FOP_NOT_INCLUDES, _T("NotIncludes"), FALSE, aTaskIDs));
		ExpectEQ(1UL, aTaskIDs[0]);
		ExpectEQ(2UL, aTaskIDs[1]);
		ExpectEQ(3UL, aTaskIDs[2]);
		ExpectEQ(4UL, aTaskIDs[3]);
		ExpectEQ(5UL, aTaskIDs[4]);
		ExpectEQ(6UL, aTaskIDs[5]);
	}

	// Set
	{
		ExpectEQ(6, match.FindTasks(nAttibID, FOP_SET, _T(""), FALSE, aTaskIDs));
		ExpectEQ(1UL, aTaskIDs[0]);
		ExpectEQ(2UL, aTaskIDs[1]);
		ExpectEQ(3UL, aTaskIDs[2]);
		ExpectEQ(4UL, aTaskIDs[3]);
		ExpectEQ(5UL, aTaskIDs[4]);
		ExpectEQ(6UL, aTaskIDs[5]);
	}

	// Not set
	{
		ExpectEQ(0, match.FindTasks(nAttibID, FOP_NOT_SET, _T(""), FALSE, aTaskIDs));
		ExpectEQ(0, aTaskIDs.GetSize());
	}
}

void CTDCTaskMatcherTest::TestSimpleTextArrayMatch(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, const CString& sPrefix)
{
	CTDCTaskMatcher match(data, reminders, contentMgr);
	CDWordArray aTaskIDs; // get overwritten by each test

	CStringArray aValues;
	CString sCompare;

	// Equals
	{
		{
			sCompare.Format(_T("%s2.0, %s2.1, %s2.2"), sPrefix, sPrefix, sPrefix);

			ExpectEQ(1, match.FindTasks(nAttibID, FOP_EQUALS, sCompare, FALSE, aTaskIDs));
			ExpectEQ(2UL, aTaskIDs[0]);
		}

		// Not order-sensitive
		{
			sCompare.Format(_T("%s2.1, %s2.2, %s2.0"), sPrefix, sPrefix, sPrefix);

			ExpectEQ(1, match.FindTasks(nAttibID, FOP_EQUALS, sCompare, FALSE, aTaskIDs));
			ExpectEQ(2UL, aTaskIDs[0]);
		}
	}

	// Not equals
	{
		{
			sCompare.Format(_T("%s4.0"), sPrefix);

			ExpectEQ(6, match.FindTasks(nAttibID, FOP_NOT_EQUALS, sCompare, FALSE, aTaskIDs));
			ExpectEQ(1UL, aTaskIDs[0]);
			ExpectEQ(2UL, aTaskIDs[1]);
			ExpectEQ(3UL, aTaskIDs[2]);
			ExpectEQ(4UL, aTaskIDs[3]);
			ExpectEQ(5UL, aTaskIDs[4]);
			ExpectEQ(6UL, aTaskIDs[5]);
		}

		{
			sCompare.Format(_T("%s2.0, %s2.1, %s2.2"), sPrefix, sPrefix, sPrefix);

			ExpectEQ(5, match.FindTasks(nAttibID, FOP_NOT_EQUALS, sCompare, FALSE, aTaskIDs));
			ExpectEQ(1UL, aTaskIDs[0]);
			// Task2 matches
			ExpectEQ(3UL, aTaskIDs[1]);
			ExpectEQ(4UL, aTaskIDs[2]);
			ExpectEQ(5UL, aTaskIDs[3]);
			ExpectEQ(6UL, aTaskIDs[4]);
		}

		{
			sCompare = _T("Cat3.0, Cat3.1, Cat3.2");

			int nNumMatches = match.FindTasks(nAttibID, FOP_NOT_EQUALS, sCompare, FALSE, aTaskIDs);

			if (nNumMatches == 6)
			{
				ExpectEQ(1UL, aTaskIDs[0]);
				ExpectEQ(2UL, aTaskIDs[1]);
				ExpectEQ(3UL, aTaskIDs[2]);
				ExpectEQ(4UL, aTaskIDs[3]);
				ExpectEQ(5UL, aTaskIDs[4]);
				ExpectEQ(6UL, aTaskIDs[5]);
			}
			else
			{
				ExpectEQ(5, nNumMatches);
				ExpectEQ(TDCA_CATEGORY, nAttibID);

				ExpectEQ(1UL, aTaskIDs[0]);
				ExpectEQ(2UL, aTaskIDs[1]);
				// Task3 matches
				ExpectEQ(4UL, aTaskIDs[2]);
				ExpectEQ(5UL, aTaskIDs[3]);
				ExpectEQ(6UL, aTaskIDs[4]);
			}
		}
	}

	// Includes
	{
// 		ExpectEQ(6, match.FindTasks(nAttibID, FOP_INCLUDES, _T("Cat"), FALSE, aTaskIDs));
// 		ExpectEQ(1UL, aTaskIDs[0]);
// 		ExpectEQ(2UL, aTaskIDs[1]);
// 		ExpectEQ(3UL, aTaskIDs[2]);
// 		ExpectEQ(4UL, aTaskIDs[3]);
// 		ExpectEQ(5UL, aTaskIDs[4]);
// 		ExpectEQ(6UL, aTaskIDs[5]);
	}

	// Not includes
	{
// 		ExpectEQ(0, match.FindTasks(nAttibID, FOP_NOT_INCLUDES, _T("Comments"), FALSE, aTaskIDs));
// 		ExpectEQ(0, aTaskIDs.GetSize());
	}

	// Set
	{
// 		ExpectEQ(6, match.FindTasks(nAttibID, FOP_SET, _T(""), FALSE, aTaskIDs));
// 		ExpectEQ(1UL, aTaskIDs[0]);
// 		ExpectEQ(2UL, aTaskIDs[1]);
// 		ExpectEQ(3UL, aTaskIDs[2]);
// 		ExpectEQ(4UL, aTaskIDs[3]);
// 		ExpectEQ(5UL, aTaskIDs[4]);
// 		ExpectEQ(6UL, aTaskIDs[5]);
	}

	// Not set
	{
// 		ExpectEQ(0, match.FindTasks(nAttibID, FOP_NOT_SET, _T(""), FALSE, aTaskIDs));
// 		ExpectEQ(0, aTaskIDs.GetSize());
	}
}

void CTDCTaskMatcherTest::TestComplexMatching(const CToDoCtrlData& data)
{
}

void CTDCTaskMatcherTest::InitialiseStyles()
{
	m_aStyles.RemoveAll();
}

// ---------------------------------------------------------------------------------

void CTDCTaskMatcherTest::PopulateDataModel(CToDoCtrlData& data) const
{
	CTaskFile tasks;
	tasks.SetCustomAttributeDefs(m_aCustomAttribDefs);

	HTASKITEM hTasks[6] = 
	{
		tasks.NewTask(_T("Task1"), NULL, 1, 0),
		tasks.NewTask(_T("Task2"), NULL, 2, 0),
		tasks.NewTask(_T("Task3"), NULL, 3, 0),
		tasks.NewTask(_T("Task4"), NULL, 4, 0),
		tasks.NewTask(_T("Task5"), NULL, 5, 0),
		tasks.NewTask(_T("Task6"), NULL, 6, 0)
	};
	
	for (int i = 0, v = 1; i < 6; i++, v++)
	{
		tasks.SetTaskComments(hTasks[i], Misc::Format(_T("Comments%d"), v));
		tasks.SetTaskCustomComments(hTasks[i], Misc::Format(_T("Comments%d"), v), _T("Type"));

		tasks.SetTaskStatus(hTasks[i], Misc::Format(_T("Stat%d"), v));
		tasks.SetTaskAllocatedBy(hTasks[i], Misc::Format(_T("AllocBy%d"), v));
		tasks.SetTaskVersion(hTasks[i], Misc::Format(_T("Ver%d"), v));
		tasks.SetTaskExternalID(hTasks[i], Misc::Format(_T("ExtID%d"), v));

		tasks.SetTaskCategory(hTasks[i], Misc::Format(_T("Cat%d"), v));
		tasks.SetTaskAllocatedTo(hTasks[i], Misc::Format(_T("AllocTo%d"), v));

#define POPULATE_ARRAY(pre, func)                        \
{ CStringArray aValues;                                  \
for (int s = 0; s < 3; s++)                              \
	aValues.Add(Misc::Format(_T("%s%d.%d"), pre, v, s)); \
tasks.func(hTasks[i], aValues); }

		POPULATE_ARRAY(_T("Tag"), SetTaskTags);
		POPULATE_ARRAY(_T("Cat"), SetTaskCategories);
		POPULATE_ARRAY(_T("AllocTo"), SetTaskAllocatedTo);
		POPULATE_ARRAY(_T("File"), SetTaskFileLinks);

		tasks.SetTaskStartDate(hTasks[i], COleDateTime(45000.0 + v));
		tasks.SetTaskDueDate(hTasks[i], COleDateTime(45001.0 + v));
		tasks.SetTaskLastModified(hTasks[i], COleDateTime(45002.0 + i), Misc::Format(_T("User%d"), v));

		tasks.SetTaskPriority(hTasks[i], (5 + v));
		tasks.SetTaskRisk(hTasks[i], (6 + v));
		tasks.SetTaskPercentDone(hTasks[i], (10 + v));

		tasks.SetTaskCost(hTasks[i], (20 + v));
		tasks.SetTaskTimeEstimate(hTasks[i], (30 + v), TDCU_DAYS);
		tasks.SetTaskTimeSpent(hTasks[i], (40 + v), TDCU_DAYS);

		tasks.SetTaskFlag(hTasks[i], ((v % 2) == 1));
		tasks.SetTaskLock(hTasks[i], ((v % 2) == 0));

		tasks.SetTaskCustomAttributeData(hTasks[i], ID_INTEGER, TDCCADATA(-37 + v));
		tasks.SetTaskCustomAttributeData(hTasks[i], ID_DATE, TDCCADATA(45020.0 + v));
	}

	data.BuildDataModel(tasks);

#ifdef _DEBUG
// 	tasks.SetXmlHeader(DEFAULT_UNICODE_HEADER);
// 	tasks.Save(_T(" CTDCTaskMatcherTest_PopulateDataModel.txt"), SFEF_UTF16);
#endif
}
