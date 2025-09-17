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

const int BASE_PRIORITY = 3;
const int BASE_RISK = 4;
const int BASE_PERCENT = 10;

const double BASE_COST = 20.0;
const double BASE_TIMEEST = 30.0;
const double BASE_TIMESPENT = 40.0;

const double BASE_CREATION = 45000.0;
const double BASE_START = 45001.0;
const double BASE_DUE = 45002.0;
//const double BASE_DONE = 45003.0;
const double BASE_LASTMOD = 45004.0;

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

	TestIndividualRules(data);
	TestMultipleRules(data);

	return GetTotals();
}

void CTDCTaskMatcherTest::TestIndividualRules(const CToDoCtrlData& data)
{
	// These tests are just for single rules
	CTDCScopedTest test(*this, _T("CTDCTaskMatcherTest::IndividualRules"));

	// Simple text
	{
		CTDCScopedSubTest subtest(*this, _T("Text"));

		TestTextAttributeRules(data, TDCA_TASKNAME, _T("Task"));
		TestTextAttributeRules(data, TDCA_COMMENTS, _T("Comments"));
		TestTextAttributeRules(data, TDCA_ALLOCBY, _T("AllocBy"));
		TestTextAttributeRules(data, TDCA_STATUS, _T("Stat"));
		TestTextAttributeRules(data, TDCA_VERSION, _T("Ver"));
		TestTextAttributeRules(data, TDCA_EXTERNALID, _T("ExtID"));
	}

	// Array of strings
	{
		CTDCScopedSubTest subtest(*this, _T("Text Array"));

		TestTextArrayAttributeRules(data, TDCA_CATEGORY, _T("Cat"));
		TestTextArrayAttributeRules(data, TDCA_ALLOCTO, _T("AllocTo"));
		TestTextArrayAttributeRules(data, TDCA_TAGS, _T("Tag"));
		TestTextArrayAttributeRules(data, TDCA_FILELINK, _T("File"));
	}

	// Integers
	{
		CTDCScopedSubTest subtest(*this, _T("Integers"));

		TestIntegerAttributeRules(data, TDCA_PERCENT, BASE_PERCENT);
		TestIntegerAttributeRules(data, TDCA_PRIORITY, BASE_PRIORITY);
		TestIntegerAttributeRules(data, TDCA_RISK, BASE_RISK);
	}

	// Doubles
	{
		CTDCScopedSubTest subtest(*this, _T("Doubles"));

		TestDoubleAttributeRules(data, TDCA_COST, BASE_COST);
	}

	// Time Periods
	{
		CTDCScopedSubTest subtest(*this, _T("Time Periods"));

		TestTimePeriodAttributeRules(data, TDCA_TIMEESTIMATE, BASE_TIMEEST);
		TestTimePeriodAttributeRules(data, TDCA_TIMESPENT, BASE_TIMESPENT);
	}

	// Dates
	{
		CTDCScopedSubTest subtest(*this, _T("Dates"));

		TestDateAttributeRules(data, TDCA_CREATIONDATE, BASE_CREATION);
		TestDateAttributeRules(data, TDCA_STARTDATE, BASE_START);
		TestDateAttributeRules(data, TDCA_DUEDATE, BASE_DUE);
		//	TestSimpleDateMatching(data, TDCA_DONEDATE, BASE_DONE);
		TestDateAttributeRules(data, TDCA_LASTMODDATE, BASE_LASTMOD);
	}
}

void CTDCTaskMatcherTest::TestTextAttributeRules(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, const CString& sPrefix)
{
	CTDCTaskMatcher matcher(data, reminders, contentMgr);
	CDWordArray aTaskIDs; // get overwritten by each test

	SEARCHPARAM rule(nAttibID);

	// Equals
	{
		rule.SetOperator(FOP_EQUALS);

		{
			rule.SetValue(sPrefix + _T("2"));

			ExpectEQ(1, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 2));
		}

		{
			rule.SetValue(_T("Equals"));

			ExpectEQ(0, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
		}
	}

	// Not equals
	{
		rule.SetOperator(FOP_NOT_EQUALS);

		{
			rule.SetValue(sPrefix + _T("5"));

			ExpectEQ(5, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 5));
		}

		{
			rule.SetValue(_T("NotEquals"));

			ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}
	}

	// Includes
	{
		rule.SetOperator(FOP_INCLUDES);

		{
			rule.SetValue(sPrefix);

			ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(_T("Includes"));

			ExpectEQ(0, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
		}
	}

	// Not includes
	{
		rule.SetOperator(FOP_NOT_INCLUDES);

		{
			rule.SetValue(sPrefix);

			ExpectEQ(0, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(_T("NotIncludes"));

			ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}
	}

	// Set
	{
		rule.SetOperator(FOP_SET);
		// value ignored

		ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
		ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
	}

	// Not set
	{
		rule.SetOperator(FOP_NOT_SET);
		// value ignored

		ExpectEQ(0, matcher.FindTasks(rule, FALSE, aTaskIDs));
		ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
	}
}

void CTDCTaskMatcherTest::TestTextArrayAttributeRules(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, const CString& sPrefix)
{
	CTDCTaskMatcher matcher(data, reminders, contentMgr);
	CDWordArray aTaskIDs; // get overwritten by each test

	SEARCHPARAM rule(nAttibID);

	// Equals
	{
		rule.SetOperator(FOP_EQUALS);

		{
			rule.SetValue(Misc::Format(_T("%s2.0, %s2.1, %s2.2"), sPrefix, sPrefix, sPrefix));

			ExpectEQ(1, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 2));
		}

		// Not order-sensitive
		{
			rule.SetValue(Misc::Format(_T("%s3.1, %s3.2, %s3.0"), sPrefix, sPrefix, sPrefix));

			ExpectEQ(1, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 3));
		}
	}

	// Not equals
	{
		rule.SetOperator(FOP_NOT_EQUALS);

		{
			rule.SetValue(Misc::Format(_T("%s4.0"), sPrefix));

			ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(Misc::Format(_T("%s2.0, %s2.1, %s2.2"), sPrefix, sPrefix, sPrefix));

			ExpectEQ(5, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 2));
		}

		{
			rule.SetValue(_T("Cat3.0, Cat3.1, Cat3.2"));

			int nNumMatches = matcher.FindTasks(rule, FALSE, aTaskIDs);

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

			ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(_T(".2"));

			ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(sPrefix + _T("3"));

			ExpectEQ(1, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 3));
		}

		{
			rule.SetValue(_T("5"));

			ExpectEQ(1, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 5));
		}

		{
			rule.SetValue(_T("Includes"));

			ExpectEQ(0, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
		}
	}

	// Not includes
	{
		rule.SetOperator(FOP_NOT_INCLUDES);

		{
			rule.SetValue(sPrefix);

			ExpectEQ(0, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(sPrefix + _T("1"));

			ExpectEQ(5, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 1));
		}

		{
			rule.SetValue(_T("0"));

			ExpectEQ(0, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(_T(".4"));

			ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(_T("NotIncludes"));

			ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}
	}

	// Set
	{
		rule.SetOperator(FOP_SET);
		// value ignored

		ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
		ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
	}

	// Not set
	{
		rule.SetOperator(FOP_NOT_SET);
		// value ignored

		ExpectEQ(0, matcher.FindTasks(rule, FALSE, aTaskIDs));
		ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
	}
}

void CTDCTaskMatcherTest::TestIntegerAttributeRules(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, int nBase)
{
	SEARCHPARAM rule(nAttibID);

	TestSimpleNumberMatching(data, rule, nBase);
}

void CTDCTaskMatcherTest::TestDoubleAttributeRules(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, double dBase)
{
	SEARCHPARAM rule(nAttibID);

	TestSimpleNumberMatching(data, rule, dBase);
}

void CTDCTaskMatcherTest::TestTimePeriodAttributeRules(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, double dBase)
{
	SEARCHPARAM rule(nAttibID);
	rule.SetTimeUnits(TDCU_DAYS);

	TestSimpleNumberMatching(data, rule, dBase);
}

void CTDCTaskMatcherTest::TestDateAttributeRules(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, double dBase)
{
	CTDCTaskMatcher matcher(data, reminders, contentMgr);
	CDWordArray aTaskIDs; // get overwritten by each test

	SEARCHPARAM rule(nAttibID);

	// Equals
	{
		rule.SetOperator(FOP_EQUALS);

		{
			rule.SetValue(dBase + 3);

			ExpectEQ(1, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 3));
		}
	}

	// Not equals
	{
		rule.SetOperator(FOP_NOT_EQUALS);

		{
			rule.SetValue(dBase + 5);

			ExpectEQ(5, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 5));
		}

		{
			rule.SetValue(dBase + 10);

			ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}
	}

	// After
	{
		rule.SetOperator(FOP_AFTER);

		{
			rule.SetValue(dBase);

			ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(dBase + 1);

			ExpectEQ(5, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 1));
		}

		{
			rule.SetValue(dBase + 5);

			ExpectEQ(1, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 6));
		}
	}

	// On or After
	{
		rule.SetOperator(FOP_ON_OR_AFTER);

		{
			rule.SetValue(dBase + 6);

			ExpectEQ(1, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 6));
		}

		{
			rule.SetValue(dBase + 1);

			ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(dBase + 2);

			ExpectEQ(5, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 1));
		}
	}

	// Before
	{
		rule.SetOperator(FOP_BEFORE);

		{
			rule.SetValue(dBase);

			ExpectEQ(0, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(dBase + 10);

			ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(dBase + 6);

			ExpectEQ(5, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 6));
		}

		{
			rule.SetValue(dBase + 2);

			ExpectEQ(1, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 1));
		}
	}

	// On or Before
	{
		rule.SetOperator(FOP_ON_OR_BEFORE);

		{
			rule.SetValue(dBase);

			ExpectEQ(0, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(dBase + 10);

			ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(dBase + 6);

			ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
		}

		{
			rule.SetValue(dBase + 5);

			ExpectEQ(5, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 6));
		}

		{
			rule.SetValue(dBase + 1);

			ExpectEQ(1, matcher.FindTasks(rule, FALSE, aTaskIDs));
			ExpectTrue(ContainsOneTaskID(aTaskIDs, 1));
		}
	}

	// Set
	{
		rule.SetOperator(FOP_SET);
		// value ignored

		ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
		ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
	}

	// Not set
	{
		rule.SetOperator(FOP_NOT_SET);
		// value ignored

		ExpectEQ(0, matcher.FindTasks(rule, FALSE, aTaskIDs));
		ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
	}
}

void CTDCTaskMatcherTest::TestMultipleRules(const CToDoCtrlData& data)
{
	// These tests are testing the logical aspects of a ruleset
	// ie. We presume that the individual rules work correctly
	CTDCScopedTest test(*this, _T("CTDCTaskMatcherTest::MultipleRules"));

	CTDCTaskMatcher matcher(data, reminders, contentMgr);
	CDWordArray aTaskIDs;

	const BOOL AND(1), OR(0);
		
	{
		CTDCScopedSubTest subtest(*this, _T("Name OR Name"));
		SEARCHPARAMS query;

		query.aRules.Add(SEARCHPARAM(TDCA_TASKNAME, FOP_EQUALS, _T("Task1"), OR));	// -> Task 1
		query.aRules.Add(SEARCHPARAM(TDCA_TASKNAME, FOP_EQUALS, _T("Task4")));		// -> Task 4

		ExpectEQ(2, matcher.FindTasks(query, FALSE, aTaskIDs));

		Misc::SortArray(aTaskIDs);
		ExpectTrue((aTaskIDs[0] == 1) && (aTaskIDs[1] == 4));
	}

	{
		CTDCScopedSubTest subtest(*this, _T("Name AND Name"));
		SEARCHPARAMS query;

		query.aRules.Add(SEARCHPARAM(TDCA_TASKNAME, FOP_EQUALS, _T("Task1"), AND)); // -> Task 1
		query.aRules.Add(SEARCHPARAM(TDCA_TASKNAME, FOP_EQUALS, _T("Task4")));		// -> Task 4

		ExpectEQ(0, matcher.FindTasks(query, FALSE, aTaskIDs));
	}

	{
		CTDCScopedSubTest subtest(*this, _T("Name OR Name OR Priority"));
		SEARCHPARAMS query;

		query.aRules.Add(SEARCHPARAM(TDCA_TASKNAME, FOP_EQUALS, _T("Task2"), OR));	// -> Task 2
		query.aRules.Add(SEARCHPARAM(TDCA_TASKNAME, FOP_EQUALS, _T("Task3"), OR));	// -> Task 3
		query.aRules.Add(SEARCHPARAM(TDCA_PRIORITY, FOP_EQUALS, 4));				// -> Task 1

		ExpectEQ(3, matcher.FindTasks(query, FALSE, aTaskIDs));

		Misc::SortArray(aTaskIDs);
		ExpectTrue((aTaskIDs[0] == 1) && (aTaskIDs[1] == 2) && (aTaskIDs[2] == 3));
	}

	{
		CTDCScopedSubTest subtest(*this, _T("Name OR Name AND Category"));
		SEARCHPARAMS query;

		query.aRules.Add(SEARCHPARAM(TDCA_TASKNAME, FOP_EQUALS, _T("Task"), OR));	// -> None
		query.aRules.Add(SEARCHPARAM(TDCA_TASKNAME, FOP_EQUALS, _T("Task5"), AND)); // -> Task 5
		query.aRules.Add(SEARCHPARAM(TDCA_CATEGORY, FOP_INCLUDES, _T("Cat5.2")));	// -> Task 5

		ExpectEQ(1, matcher.FindTasks(query, FALSE, aTaskIDs));
		ExpectTrue(aTaskIDs[0] == 5);
	}

	{
		CTDCScopedSubTest subtest(*this, _T("Risk AND Name Or Priority AND Risk"));
		SEARCHPARAMS query;

		query.aRules.Add(SEARCHPARAM(TDCA_RISK, FOP_LESS, 7, AND));					// -> Tasks 1/2
		query.aRules.Add(SEARCHPARAM(TDCA_TASKNAME, FOP_INCLUDES, _T("Task"), OR)); // -> All

		query.aRules.Add(SEARCHPARAM(TDCA_PRIORITY, FOP_GREATER_OR_EQUAL, 7, AND)); // -> Tasks 4/5/6
		query.aRules.Add(SEARCHPARAM(TDCA_RISK, FOP_GREATER, 8));					// -> Tasks 5/6

		ExpectEQ(4, matcher.FindTasks(query, FALSE, aTaskIDs));

		Misc::SortArray(aTaskIDs);
		ExpectTrue((aTaskIDs[0] == 1) && (aTaskIDs[1] == 2) && (aTaskIDs[2] == 5) && (aTaskIDs[3] == 6));
	}

	{
		CTDCScopedSubTest subtest(*this, _T("Risk AND Name Or Priority AND Risk OR Cost AND Cost"));

		SEARCHPARAMS query;
		query.aRules.Add(SEARCHPARAM(TDCA_RISK, FOP_LESS, 7, AND));					// -> Tasks 1/2
		query.aRules.Add(SEARCHPARAM(TDCA_TASKNAME, FOP_INCLUDES, _T("Task"), OR)); // -> All

		query.aRules.Add(SEARCHPARAM(TDCA_PRIORITY, FOP_GREATER_OR_EQUAL, 7, AND)); // -> Tasks 4/5/6
		query.aRules.Add(SEARCHPARAM(TDCA_RISK, FOP_GREATER, 8, OR));				// -> Tasks 5/6

		query.aRules.Add(SEARCHPARAM(TDCA_COST, FOP_GREATER, 22.0, AND));			// -> Tasks 3/4/5/6
		query.aRules.Add(SEARCHPARAM(TDCA_COST, FOP_LESS, 25.0));					// -> Tasks 1/2/3/4

		Misc::SortArray(aTaskIDs);
		ExpectEQ(6, matcher.FindTasks(query, FALSE, aTaskIDs));
		ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
	}
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

		tasks.SetTaskCreationDate(hTasks[i], COleDateTime(BASE_CREATION + v));
		tasks.SetTaskStartDate(hTasks[i], COleDateTime(BASE_START + v));
		tasks.SetTaskDueDate(hTasks[i], COleDateTime(BASE_DUE + v));
//		tasks.SetTaskDoneDate(hTasks[i], COleDateTime(BASE_DONE + v));
		tasks.SetTaskLastModified(hTasks[i], COleDateTime(BASE_LASTMOD + v), Misc::Format(_T("User%d"), v));

		tasks.SetTaskPriority(hTasks[i], (BASE_PRIORITY + v));
		tasks.SetTaskRisk(hTasks[i], (BASE_RISK + v));
		tasks.SetTaskPercentDone(hTasks[i], (BASE_PERCENT + v));

		tasks.SetTaskCost(hTasks[i], (BASE_COST + v));
		tasks.SetTaskTimeEstimate(hTasks[i], (BASE_TIMEEST + v), TDCU_DAYS);
		tasks.SetTaskTimeSpent(hTasks[i], (BASE_TIMESPENT + v), TDCU_DAYS);

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
