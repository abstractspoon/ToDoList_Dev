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
	CTDLTestBase(_T("CTDCTaskMatcherTest"), utils)
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
	TestSimpleTextMatching(data, TDCA_TASKNAME, _T("Task"));
	TestSimpleTextMatching(data, TDCA_COMMENTS, _T("Comments"));
	TestSimpleTextMatching(data, TDCA_ALLOCBY, _T("AllocBy"));
	TestSimpleTextMatching(data, TDCA_STATUS, _T("Stat"));
	TestSimpleTextMatching(data, TDCA_VERSION, _T("Ver"));
	TestSimpleTextMatching(data, TDCA_EXTERNALID, _T("ExtID"));

	// Special case
// 	TestSimpleTextMatch(data, TDCA_TASKNAMEORCOMMENTS, _T("Task"));
// 	TestSimpleTextMatch(data, TDCA_TASKNAMEORCOMMENTS, _T("Comments"));

	// Array of strings
	TestSimpleTextArrayMatching(data, TDCA_CATEGORY, _T("Cat"));
	TestSimpleTextArrayMatching(data, TDCA_ALLOCTO, _T("AllocTo"));
	TestSimpleTextArrayMatching(data, TDCA_TAGS, _T("Tag"));
	TestSimpleTextArrayMatching(data, TDCA_FILELINK, _T("File"));

	// Integers
	TestSimpleIntegerMatching(data, TDCA_PERCENT, 10);
	TestSimpleIntegerMatching(data, TDCA_PRIORITY, 3);
	TestSimpleIntegerMatching(data, TDCA_RISK, 4);

	// Doubles
	TestSimpleDoubleMatching(data, TDCA_COST, 20.0);

	// Time Periods
	TestSimpleTimePeriodMatching(data, TDCA_TIMEESTIMATE, 30.0);
	TestSimpleTimePeriodMatching(data, TDCA_TIMESPENT, 40.0);

	// Dates
	TestSimpleDateMatching(data, TDCA_CREATIONDATE, 45000.0);
	TestSimpleDateMatching(data, TDCA_STARTDATE, 45001.0);
	TestSimpleDateMatching(data, TDCA_DUEDATE, 45002.0);
//	TestSimpleDateMatching(data, TDCA_DONEDATE, 45003.0);
	TestSimpleDateMatching(data, TDCA_LASTMODDATE, 45004.0);

// 	TestSimplePriorityRiskMatching(data, TDCA_PRIORITY);
// 	TestSimplePriorityRiskMatching(data, TDCA_RISK);

}

void CTDCTaskMatcherTest::TestSimpleTextMatching(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, const CString& sPrefix)
{
	CTDCTaskMatcher match(data, reminders, contentMgr);
	CDWordArray aTaskIDs; // get overwritten by each test

	SEARCHPARAM rule(nAttibID);

	// Equals
	{
		rule.SetOperator(FOP_EQUALS);

		{
			rule.SetValue(sPrefix + _T("2"));

			ExpectEQ(1, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 2));
		}

		{
			rule.SetValue(_T("Equals"));

			ExpectEQ(0, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
		}
	}

	// Not equals
	{
		rule.SetOperator(FOP_NOT_EQUALS);

		{
			rule.SetValue(sPrefix + _T("5"));

			ExpectEQ(5, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 5));
		}

		{
			rule.SetValue(_T("NotEquals"));

			ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}
	}

	// Includes
	{
		rule.SetOperator(FOP_INCLUDES);

		{
			rule.SetValue(sPrefix);

			ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(_T("Includes"));

			ExpectEQ(0, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
		}
	}

	// Not includes
	{
		rule.SetOperator(FOP_NOT_INCLUDES);

		{
			rule.SetValue(sPrefix);

			ExpectEQ(0, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(_T("NotIncludes"));

			ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}
	}

	// Set
	{
		rule.SetOperator(FOP_SET);
		// value ignored

		ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
		ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
	}

	// Not set
	{
		rule.SetOperator(FOP_NOT_SET);
		// value ignored

		ExpectEQ(0, match.FindTasks(rule, FALSE, aTaskIDs));
		ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
	}
}

void CTDCTaskMatcherTest::TestSimpleTextArrayMatching(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, const CString& sPrefix)
{
	CTDCTaskMatcher match(data, reminders, contentMgr);
	CDWordArray aTaskIDs; // get overwritten by each test

	SEARCHPARAM rule(nAttibID);

	// Equals
	{
		rule.SetOperator(FOP_EQUALS);

		{
			rule.SetValue(Misc::Format(_T("%s2.0, %s2.1, %s2.2"), sPrefix, sPrefix, sPrefix));

			ExpectEQ(1, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 2));
		}

		// Not order-sensitive
		{
			rule.SetValue(Misc::Format(_T("%s3.1, %s3.2, %s3.0"), sPrefix, sPrefix, sPrefix));

			ExpectEQ(1, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 3));
		}
	}

	// Not equals
	{
		rule.SetOperator(FOP_NOT_EQUALS);

		{
			rule.SetValue(Misc::Format(_T("%s4.0"), sPrefix));

			ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(Misc::Format(_T("%s2.0, %s2.1, %s2.2"), sPrefix, sPrefix, sPrefix));

			ExpectEQ(5, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 2));
		}

		{
			rule.SetValue(_T("Cat3.0, Cat3.1, Cat3.2"));

			int nNumMatches = match.FindTasks(rule, FALSE, aTaskIDs);

			if (nNumMatches == 6)
			{
				ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
			}
			else
			{
				ExpectEQ(5, nNumMatches);
				ExpectEQ(TDCA_CATEGORY, nAttibID);

				ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 3));
			}
		}
	}

	// Includes
	{
		rule.SetOperator(FOP_INCLUDES);

		{
			rule.SetValue(sPrefix);

			ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(_T(".2"));

			ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(sPrefix + _T("3"));

			ExpectEQ(1, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 3));
		}

		{
			rule.SetValue(_T("5"));

			ExpectEQ(1, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 5));
		}

		{
			rule.SetValue(_T("Includes"));

			ExpectEQ(0, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
		}
	}

	// Not includes
	{
		rule.SetOperator(FOP_NOT_INCLUDES);

		{
			rule.SetValue(sPrefix);

			ExpectEQ(0, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(sPrefix + _T("1"));

			ExpectEQ(5, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 1));
		}

		{
			rule.SetValue(_T("0"));

			ExpectEQ(0, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(_T(".4"));

			ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(_T("NotIncludes"));

			ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}
	}

	// Set
	{
		rule.SetOperator(FOP_SET);
		// value ignored

		ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
		ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
	}

	// Not set
	{
		rule.SetOperator(FOP_NOT_SET);
		// value ignored

		ExpectEQ(0, match.FindTasks(rule, FALSE, aTaskIDs));
		ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
	}
}

template <class T>
void CTDCTaskMatcherTest::TestSimpleNumberMatching(const CToDoCtrlData& data, SEARCHPARAM& rule, T tOffset) const
{
	CTDCTaskMatcher match(data, reminders, contentMgr);
	CDWordArray aTaskIDs; // get overwritten by each test

	// Equals
	{
		rule.SetOperator(FOP_EQUALS);

		{
			rule.SetValue(tOffset + 3);

 			ExpectEQ(1, match.FindTasks(rule, FALSE, aTaskIDs));
 			ExpectTrue(ContainsOneTaskID(aTaskIDs, 3));
		}
	}

	// Not equals
	{
		rule.SetOperator(FOP_NOT_EQUALS);

		{
			rule.SetValue(tOffset + 5);

			ExpectEQ(5, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 5));
		}

		{
			rule.SetValue(tOffset + 10);

			ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

	}

	// Greater Than
	{
		rule.SetOperator(FOP_GREATER);

		{
			rule.SetValue(tOffset);

			ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(tOffset + 1);

			ExpectEQ(5, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 1));
		}

		{
			rule.SetValue(tOffset + 5);

			ExpectEQ(1, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 6));
		}
	}

	// Greater Than or Equal To
	{
		rule.SetOperator(FOP_GREATER_OR_EQUAL);

		{
			rule.SetValue(tOffset + 6);

			ExpectEQ(1, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 6));
		}

		{
			rule.SetValue(tOffset + 1);

			ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(tOffset + 2);

			ExpectEQ(5, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 1));
		}
	}

	// Less Than
	{
		rule.SetOperator(FOP_LESS);

		{
			rule.SetValue(tOffset);

			ExpectEQ(0, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(tOffset + 10);

			ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(tOffset + 6);

			ExpectEQ(5, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 6));
		}

		{
			rule.SetValue(tOffset + 2);

			ExpectEQ(1, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 1));
		}
	}

	// Less Than or Equal To
	{
		rule.SetOperator(FOP_LESS_OR_EQUAL);

		{
			rule.SetValue(tOffset);

			ExpectEQ(0, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(tOffset + 10);

			ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(tOffset + 6);

			ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(tOffset + 5);

			ExpectEQ(5, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 6));
		}

		{
			rule.SetValue(tOffset + 1);

			ExpectEQ(1, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 1));
		}
	}

	// Set
	{
		rule.SetOperator(FOP_SET);
		// value ignored

		ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
		ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
	}

	// Not set
	{
		rule.SetOperator(FOP_NOT_SET);
		// value ignored

		ExpectEQ(0, match.FindTasks(rule, FALSE, aTaskIDs));
		ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
	}
}

void CTDCTaskMatcherTest::TestSimpleIntegerMatching(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, int nOffset)
{
	SEARCHPARAM rule(nAttibID);

	TestSimpleNumberMatching(data, rule, nOffset);
}

void CTDCTaskMatcherTest::TestSimpleDoubleMatching(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, double dOffset)
{
	SEARCHPARAM rule(nAttibID);

	TestSimpleNumberMatching(data, rule, dOffset);
}

void CTDCTaskMatcherTest::TestSimpleTimePeriodMatching(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, double dOffset)
{
	SEARCHPARAM rule(nAttibID);
	rule.SetTimeUnits(TDCU_DAYS);

	TestSimpleNumberMatching(data, rule, dOffset);
}

void CTDCTaskMatcherTest::TestSimpleDateMatching(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, double dOffset)
{
	CTDCTaskMatcher match(data, reminders, contentMgr);
	CDWordArray aTaskIDs; // get overwritten by each test

	SEARCHPARAM rule(nAttibID);

	// Equals
	{
		rule.SetOperator(FOP_EQUALS);

		{
			rule.SetValue(dOffset + 3);

			ExpectEQ(1, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 3));
		}
	}

	// Not equals
	{
		rule.SetOperator(FOP_NOT_EQUALS);

		{
			rule.SetValue(dOffset + 5);

			ExpectEQ(5, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 5));
		}

		{
			rule.SetValue(dOffset + 10);

			ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}
	}

	// After
	{
		rule.SetOperator(FOP_AFTER);

		{
			rule.SetValue(dOffset);

			ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(dOffset + 1);

			ExpectEQ(5, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 1));
		}

		{
			rule.SetValue(dOffset + 5);

			ExpectEQ(1, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 6));
		}
	}

	// On or After
	{
		rule.SetOperator(FOP_ON_OR_AFTER);

		{
			rule.SetValue(dOffset + 6);

			ExpectEQ(1, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 6));
		}

		{
			rule.SetValue(dOffset + 1);

			ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(dOffset + 2);

			ExpectEQ(5, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 1));
		}
	}

	// Before
	{
		rule.SetOperator(FOP_BEFORE);

		{
			rule.SetValue(dOffset);

			ExpectEQ(0, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(dOffset + 10);

			ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(dOffset + 6);

			ExpectEQ(5, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 6));
		}

		{
			rule.SetValue(dOffset + 2);

			ExpectEQ(1, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 1));
		}
	}

	// On or Before
	{
		rule.SetOperator(FOP_ON_OR_BEFORE);

		{
			rule.SetValue(dOffset);

			ExpectEQ(0, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(dOffset + 10);

			ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(dOffset + 6);

			ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(dOffset + 5);

			ExpectEQ(5, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 6));
		}

		{
			rule.SetValue(dOffset + 1);

			ExpectEQ(1, match.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 1));
		}
	}

	// Set
	{
		rule.SetOperator(FOP_SET);
		// value ignored

		ExpectEQ(6, match.FindTasks(rule, FALSE, aTaskIDs));
		ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
	}

	// Not set
	{
		rule.SetOperator(FOP_NOT_SET);
		// value ignored

		ExpectEQ(0, match.FindTasks(rule, FALSE, aTaskIDs));
		ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
	}
}

void CTDCTaskMatcherTest::TestComplexMatching(const CToDoCtrlData& data)
{
}

// ---------------------------------------------------------------------------------

void CTDCTaskMatcherTest::InitialiseStyles()
{
	m_aStyles.RemoveAll();
}

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

		tasks.SetTaskCreationDate(hTasks[i], COleDateTime(45000.0 + v));
		tasks.SetTaskStartDate(hTasks[i], COleDateTime(45001.0 + v));
		tasks.SetTaskDueDate(hTasks[i], COleDateTime(45002.0 + v));
//		tasks.SetTaskDoneDate(hTasks[i], COleDateTime(45003.0 + v));
		tasks.SetTaskLastModified(hTasks[i], COleDateTime(45004.0 + v), Misc::Format(_T("User%d"), v));

		tasks.SetTaskPriority(hTasks[i], (3 + v));
		tasks.SetTaskRisk(hTasks[i], (4 + v));
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

// ---------------------------------------------------------------------------------

BOOL CTDCTaskMatcherTest::ContainsAllTaskIDs(const CDWordArray& aTaskIDs)
{
	if (aTaskIDs.GetSize() != 6)
		return FALSE;

	for (int t = 0; t < 6; t++)
	{
		if (aTaskIDs[t] != (t + 1))
			return FALSE;
	}

	return TRUE;
}

BOOL CTDCTaskMatcherTest::ContainsAllTaskIDsButOne(const CDWordArray& aTaskIDs, DWORD dwTaskID)
{
	ASSERT((dwTaskID >= 1) && (dwTaskID <= 6));

	if (aTaskIDs.GetSize() != 5)
		return FALSE;

	CDWordSet mapTaskIDs(aTaskIDs);

	if (mapTaskIDs.Has(dwTaskID))
		return FALSE;

	for (DWORD t = 1; t <= 6; t++)
	{
		if ((t != dwTaskID) && !mapTaskIDs.Has(t))
			return FALSE;
	}

	return TRUE;
}

BOOL CTDCTaskMatcherTest::ContainsOneTaskID(const CDWordArray& aTaskIDs, DWORD dwTaskID)
{
	ASSERT((dwTaskID >= 1) && (dwTaskID <= 6));

	return ((aTaskIDs.GetSize() == 1) && (aTaskIDs[0] == dwTaskID));
}

BOOL CTDCTaskMatcherTest::ContainsNoTaskIDs(const CDWordArray& aTaskIDs)
{
	return (aTaskIDs.GetSize() == 0);
}
