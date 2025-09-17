// ToDoCtrlDataTest.h: interface for the CToDoCtrlDataTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCTASKMATCHERTEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_)
#define AFX_TDCTASKMATCHERTEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "TDLTestBase.h"

#include "..\todolist\tdcstruct.h"

//////////////////////////////////////////////////////////////////////

class CToDoCtrlData;

//////////////////////////////////////////////////////////////////////

class CTDCTaskMatcherTest : public CTDLTestBase
{
public:
	CTDCTaskMatcherTest(const CTestUtils& utils);
	virtual ~CTDCTaskMatcherTest();
	
	TESTRESULT Run();

protected:
	CTDCStyleMap m_aStyles;
	CTDCCustomAttribDefinitionArray m_aCustomAttribDefs;

protected:
	void TestIndividualRules(const CToDoCtrlData& data);
	void TestMultipleRules(const CToDoCtrlData& data);

	void TestTextAttributeRules(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, const CString& sPrefix);
	void TestTextArrayAttributeRules(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, const CString& sPrefix);
	
	void TestIntegerAttributeRules(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, int nBase);
	void TestDoubleAttributeRules(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, double dBase);
	void TestTimePeriodAttributeRules(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, double dBase);
	void TestDateAttributeRules(const CToDoCtrlData& data, TDC_ATTRIBUTE nAttibID, double dBase);

	void PopulateDataModel(CToDoCtrlData& data) const;
	void InitialiseStyles();

	static BOOL ContainsAllTaskIDs(const CDWordArray& aTasks);
	static BOOL ContainsNoTaskIDs(const CDWordArray& aTasks);
	static BOOL ContainsAllTaskIDsButOne(const CDWordArray& aTasks, DWORD dwTaskID);
	static BOOL ContainsOneTaskID(const CDWordArray& aTasks, DWORD dwTaskID);

	template <class T>
	void TestSimpleNumberMatching(const CToDoCtrlData& data, SEARCHPARAM& rule, T tBase) const
	{
		CTDCTaskMatcher matcher(data, reminders, contentMgr);
		CDWordArray aTaskIDs; // get overwritten by each test

		// Equals
		{
			rule.SetOperator(FOP_EQUALS);

			{
				rule.SetValue(tBase + 3);

 				ExpectEQ(1, matcher.FindTasks(rule, FALSE, aTaskIDs));
 				ExpectTrue(ContainsOneTaskID(aTaskIDs, 3));
			}
		}

		// Not equals
		{
			rule.SetOperator(FOP_NOT_EQUALS);

			{
				rule.SetValue(tBase + 5);

				ExpectEQ(5, matcher.FindTasks(rule, FALSE, aTaskIDs));
				ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 5));
			}

			{
				rule.SetValue(tBase + 10);

				ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
				ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
			}

		}

		// Greater Than
		{
			rule.SetOperator(FOP_GREATER);

			{
				rule.SetValue(tBase);

				ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
				ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
			}

			{
				rule.SetValue(tBase + 1);

				ExpectEQ(5, matcher.FindTasks(rule, FALSE, aTaskIDs));
				ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 1));
			}

			{
				rule.SetValue(tBase + 5);

				ExpectEQ(1, matcher.FindTasks(rule, FALSE, aTaskIDs));
				ExpectTrue(ContainsOneTaskID(aTaskIDs, 6));
			}
		}

		// Greater Than or Equal To
		{
			rule.SetOperator(FOP_GREATER_OR_EQUAL);

			{
				rule.SetValue(tBase + 6);

				ExpectEQ(1, matcher.FindTasks(rule, FALSE, aTaskIDs));
				ExpectTrue(ContainsOneTaskID(aTaskIDs, 6));
			}

			{
				rule.SetValue(tBase + 1);

				ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
				ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
			}

			{
				rule.SetValue(tBase + 2);

				ExpectEQ(5, matcher.FindTasks(rule, FALSE, aTaskIDs));
				ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 1));
			}
		}

		// Less Than
		{
			rule.SetOperator(FOP_LESS);

			{
				rule.SetValue(tBase);

				ExpectEQ(0, matcher.FindTasks(rule, FALSE, aTaskIDs));
				ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
			}

			{
				rule.SetValue(tBase + 10);

				ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
				ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
			}

			{
				rule.SetValue(tBase + 6);

				ExpectEQ(5, matcher.FindTasks(rule, FALSE, aTaskIDs));
				ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 6));
			}

			{
				rule.SetValue(tBase + 2);

				ExpectEQ(1, matcher.FindTasks(rule, FALSE, aTaskIDs));
				ExpectTrue(ContainsOneTaskID(aTaskIDs, 1));
			}
		}

		// Less Than or Equal To
		{
			rule.SetOperator(FOP_LESS_OR_EQUAL);

			{
				rule.SetValue(tBase);

				ExpectEQ(0, matcher.FindTasks(rule, FALSE, aTaskIDs));
				ExpectTrue(ContainsNoTaskIDs(aTaskIDs));
			}

			{
				rule.SetValue(tBase + 10);

				ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
				ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
			}

			{
				rule.SetValue(tBase + 6);

				ExpectEQ(6, matcher.FindTasks(rule, FALSE, aTaskIDs));
				ExpectTrue(ContainsAllTaskIDs(aTaskIDs));
			}

			{
				rule.SetValue(tBase + 5);

				ExpectEQ(5, matcher.FindTasks(rule, FALSE, aTaskIDs));
				ExpectTrue(ContainsAllTaskIDsButOne(aTaskIDs, 6));
			}

			{
				rule.SetValue(tBase + 1);

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

};

#endif // !defined(AFX_TDCTASKMATCHERTEST_H__DD08C7F5_76D6_4587_8325_41F964BFB927__INCLUDED_)

