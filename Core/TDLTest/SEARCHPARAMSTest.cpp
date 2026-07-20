// TaskFileTest.cpp: implementation of the CTaskFileTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "SEARCHPARAMSTest.h"

#include "..\ToDoList\TDCStruct.h"

#include "..\Shared\DateHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

CSEARCHPARAMSTest::CSEARCHPARAMSTest(const CTestUtils& utils) 
	: 
	CTDLTestBase(_T("CSEARCHPARAMSTest"), utils)
{

}

CSEARCHPARAMSTest::~CSEARCHPARAMSTest()
{

}

TESTRESULT CSEARCHPARAMSTest::Run()
{
	ClearTotals();

	TestSEARCHPARAM();
	//TestSearchParamArray();
	//TestSEARCHPARAMS();

	return GetTotals();
}

void CSEARCHPARAMSTest::TestSEARCHPARAM()
{
	CTDCScopedTest test(*this, _T("SEARCHPARAM"));

	// FT_NONE
	{
		CTDCScopedSubTest subTest(*this, _T("FT_NONE"));

		SEARCHPARAM rule;

		ExpectEQ(rule.GetAttribute(), TDCA_NONE);
		ExpectEQ(rule.GetAttribType(), FT_NONE);
		ExpectEQ(rule.GetOperator(), FOP_NONE);
		ExpectEQ(rule.GetAnd(), TRUE);

		rule.SetAnd(FALSE);
		ExpectEQ(rule.GetAnd(), FALSE);

#ifndef _DEBUG
		// These checks will assert
		ExpectEQ(rule.ValueAsString(), _T(""));
		ExpectEQ(rule.ValueAsInteger(), 0);
		ExpectEQ(rule.ValueAsDouble(), 0.0);
		ExpectEQ(rule.ValueAsDate(), CDateHelper::NullDate());

		rule.SetValue(_T("Fail"));
		ExpectEQ(rule.ValueAsString(), _T(""));

		rule.SetValue(5);
		ExpectEQ(rule.ValueAsString(), _T(""));

		rule.SetValue(7.0);
		ExpectEQ(rule.ValueAsString(), _T(""));

		rule.SetValue(COleDateTime::GetCurrentTime());
		ExpectEQ(rule.ValueAsString(), _T(""));

		rule.SetMatchWholeWord(TRUE);
		ExpectEQ(rule.GetMatchWholeWord(), FALSE);
#endif
	}

	// Text rules
	{
		// FT_STRING
		{
			CTDCScopedSubTest subTest(*this, _T("FT_STRING"));

			const TDC_ATTRIBUTE ATTRIBIDS[] =
			{
				TDCA_TASKNAME,
				TDCA_TASKNAMEORCOMMENTS,
				TDCA_ANYTEXTATTRIBUTE,
				TDCA_ALLOCTO,
				TDCA_ALLOCBY,
				TDCA_STATUS,
				TDCA_CATEGORY,
				TDCA_VERSION,
				TDCA_COMMENTS,
				TDCA_FILELINK,
				TDCA_CREATEDBY,
				TDCA_EXTERNALID,
				TDCA_TAGS,
				TDCA_PATH,
				TDCA_LASTMODBY,
				TDCA_COMMENTSFORMAT,
			};
			const int NUM_IDS = (sizeof(ATTRIBIDS) / sizeof(ATTRIBIDS[0]));

			for (int i = 0; i < NUM_IDS; i++)
			{
				TestTextParam(ATTRIBIDS[i], FT_STRING);

				// FT_STRING specific
				SEARCHPARAM rule(ATTRIBIDS[i]);

				rule.SetMatchWholeWord(TRUE);
				ExpectEQ(rule.GetMatchWholeWord(), TRUE);
			}
		}

		// FT_ICON
		{
			CTDCScopedSubTest subTest(*this, _T("FT_ICON"));

			TestTextParam(TDCA_ICON, FT_ICON);
		}

		// FT_DEPENDENCY:
		{
			CTDCScopedSubTest subTest(*this, _T("FT_DEPENDENCY"));

			TestTextParam(TDCA_DEPENDENCY, FT_DEPENDENCY);
		}
	}

	// Integer types
	{
		// FT_INTEGER
		{
			CTDCScopedSubTest subTest(*this, _T("FT_INTEGER"));

			const TDC_ATTRIBUTE ATTRIBIDS[] =
			{
				TDCA_PRIORITY,
				TDCA_PERCENT,
				TDCA_RISK,
				TDCA_ID,
				TDCA_PARENTID,
				TDCA_POSITION,
			};
			const int NUM_IDS = (sizeof(ATTRIBIDS) / sizeof(ATTRIBIDS[0]));

			for (int i = 0; i < NUM_IDS; i++)
				TestIntegerParam(ATTRIBIDS[i], FT_INTEGER);
		}

		// FT_BOOL
		{
			CTDCScopedSubTest subTest(*this, _T("FT_BOOL"));

			const TDC_ATTRIBUTE ATTRIBIDS[] =
			{
				TDCA_FLAG,
				TDCA_LOCK,
				TDCA_RECENTMODIFIED,
				TDCA_REMINDER,
			};
			const int NUM_IDS = (sizeof(ATTRIBIDS) / sizeof(ATTRIBIDS[0]));

			for (int i = 0; i < NUM_IDS; i++)
				TestIntegerParam(ATTRIBIDS[i], FT_BOOL);
		}

		// FT_RECURRENCE
		{
			CTDCScopedSubTest subTest(*this, _T("FT_RECURRENCE"));

			TestIntegerParam(TDCA_RECURRENCE, FT_RECURRENCE);
		}

		// FT_COLOR
		{
			CTDCScopedSubTest subTest(*this, _T("FT_COLOR"));

			TestIntegerParam(TDCA_COLOR, FT_COLOR);
		}
	}

	// FT_DOUBLE
	{
		CTDCScopedSubTest subTest(*this, _T("FT_DOUBLE"));

		const TDC_ATTRIBUTE ATTRIBIDS[] =
		{
			TDCA_COST,
			TDCA_SUBTASKDONE,
			TDCA_COMMENTSSIZE,
		};
		const int NUM_IDS = (sizeof(ATTRIBIDS) / sizeof(ATTRIBIDS[0]));

		for (int i = 0; i < NUM_IDS; i++)
			TestDecimalParam(ATTRIBIDS[i], FT_DOUBLE);
	}
	
	// FT_TIMEPERIOD
	{
		CTDCScopedSubTest subTest(*this, _T("FT_TIMEPERIOD"));

		const TDC_ATTRIBUTE ATTRIBIDS[] =
		{
			TDCA_TIMEESTIMATE,
			TDCA_TIMESPENT,
		};
		const int NUM_IDS = (sizeof(ATTRIBIDS) / sizeof(ATTRIBIDS[0]));

		for (int i = 0; i < NUM_IDS; i++)
			TestTimePeriodParam(ATTRIBIDS[i]);
	}

	// Dates
	{
		CTDCScopedSubTest subTest(*this, _T(""));

		const TDC_ATTRIBUTE ATTRIBIDS[] =
		{
			TDCA_STARTDATE,
			TDCA_STARTTIME,
			TDCA_CREATIONDATE,
			TDCA_DONEDATE,
			TDCA_DUEDATE,
			TDCA_DUETIME,
			TDCA_LASTMODDATE,
		};
		const int NUM_IDS = (sizeof(ATTRIBIDS) / sizeof(ATTRIBIDS[0]));

		// FT_DATE
		{
			CTDCScopedSubTest subTest(*this, _T("FT_DATE"));

			for (int i = 0; i < NUM_IDS; i++)
				TestDateParam(ATTRIBIDS[i], FT_DATE);
		}

		// FT_DATERELATIVE
		{
			CTDCScopedSubTest subTest(*this, _T("FT_DATERELATIVE"));

			for (int i = 0; i < NUM_IDS; i++)
				TestDateParam(ATTRIBIDS[i], FT_DATERELATIVE);
		}
	}

	// FT_GROUP
	{
		CTDCScopedSubTest subTest(*this, _T("FT_GROUP"));

		const TDC_ATTRIBUTE ATTRIBIDS[] =
		{
			TDCA_MATCHGROUPSTART,
			TDCA_MATCHGROUPEND
		};
		const int NUM_IDS = (sizeof(ATTRIBIDS) / sizeof(ATTRIBIDS[0]));

		for (int i = 0; i < NUM_IDS; i++)
			TestGroupParam(ATTRIBIDS[i]);
	}
}

// --------------------------------------------------------------

void CSEARCHPARAMSTest::TestTextParam(TDC_ATTRIBUTE nAttribID, FIND_ATTRIBTYPE nType)
{
	SEARCHPARAM rule(nAttribID);

	ExpectEQ(rule.GetAttribute(), nAttribID);
	ExpectEQ(rule.GetAttribType(), nType);
	ExpectEQ(rule.GetOperator(), FOP_NONE);
	ExpectEQ(rule.GetAnd(), TRUE);
	ExpectEQ(rule.ValueAsString(), _T(""));
	ExpectEQ(rule.GetMatchWholeWord(), FALSE);

	ExpectTrue(rule.SetValue(_T("Success")));
	ExpectEQ(rule.ValueAsString(), _T("Success"));

	rule.ClearValue();
	ExpectEQ(rule.ValueAsString(), _T(""));
	
	ExpectTrue(rule.SetValue(_T("Success")));
	ExpectEQ(rule.ValueAsString(), _T("Success"));

#ifndef _DEBUG
	// These checks will assert
	ExpectEQ(rule.ValueAsInteger(), 0);
	ExpectEQ(rule.ValueAsDouble(), 0.0);
	ExpectEQ(rule.ValueAsDate(), CDateHelper::NullDate());

	ExpectFalse(rule.SetValue(5));
	ExpectEQ(rule.ValueAsString(), _T("Success"));

	ExpectFalse(rule.SetValue(7.0));
	ExpectEQ(rule.ValueAsString(), _T("Success"));

	ExpectFalse(rule.SetValue(COleDateTime::GetCurrentTime()));
	ExpectEQ(rule.ValueAsString(), _T("Success"));
#endif
}

void CSEARCHPARAMSTest::TestIntegerParam(TDC_ATTRIBUTE nAttribID, FIND_ATTRIBTYPE nType)
{
	SEARCHPARAM rule(nAttribID);

	ExpectEQ(rule.GetAttribute(), nAttribID);
	ExpectEQ(rule.GetAttribType(), nType);
	ExpectEQ(rule.GetOperator(), FOP_NONE);
	ExpectEQ(rule.GetAnd(), TRUE);

	ExpectEQ(rule.ValueAsString(), _T("0"));
	ExpectEQ(rule.ValueAsInteger(), 0);
	ExpectEQ(rule.ValueAsDouble(), 0.0);

	ExpectTrue(rule.SetValue(5));
	ExpectEQ(rule.ValueAsString(), _T("5"));
	ExpectEQ(rule.ValueAsInteger(), 5);
	ExpectEQ(rule.ValueAsDouble(), 5.0);

	rule.ClearValue();
	ExpectEQ(rule.ValueAsString(), _T("0"));
	ExpectEQ(rule.ValueAsInteger(), 0);
	ExpectEQ(rule.ValueAsDouble(), 0.0);

	ExpectTrue(rule.SetValue(_T("-5")));
	ExpectEQ(rule.ValueAsInteger(), -5);
	ExpectEQ(rule.ValueAsDouble(), -5.0);

#ifndef _DEBUG
	// These checks will assert
	ExpectEQ(rule.ValueAsDate(), CDateHelper::NullDate());

	ExpectFalse(rule.SetValue(_T("Fail")));
	ExpectEQ(rule.ValueAsString(), _T("-5"));
	ExpectEQ(rule.ValueAsInteger(), -5);
	ExpectEQ(rule.ValueAsDouble(), -5.0);

	ExpectFalse(rule.SetValue(10.6));
	ExpectEQ(rule.ValueAsString(), _T("-5"));
	ExpectEQ(rule.ValueAsInteger(), -5);
	ExpectEQ(rule.ValueAsDouble(), -5.0);

	ExpectFalse(rule.SetValue(COleDateTime::GetCurrentTime()));
	ExpectEQ(rule.ValueAsString(), _T("-5"));
	ExpectEQ(rule.ValueAsInteger(), -5);
	ExpectEQ(rule.ValueAsDouble(), -5.0);

	ExpectFalse(rule.SetMatchWholeWord(TRUE));
	ExpectEQ(rule.GetMatchWholeWord(), FALSE);
#endif
}

void CSEARCHPARAMSTest::TestDecimalParam(TDC_ATTRIBUTE nAttribID, FIND_ATTRIBTYPE nType)
{
	SEARCHPARAM rule(nAttribID);

	ExpectEQ(rule.GetAttribute(), nAttribID);
	ExpectEQ(rule.GetAttribType(), nType);
	ExpectEQ(rule.GetOperator(), FOP_NONE);
	ExpectEQ(rule.GetAnd(), TRUE);
	ExpectEQ(rule.ValueAsString(), _T("0.000"));
	ExpectEQ(rule.ValueAsInteger(), 0);
	ExpectEQ(rule.ValueAsDouble(), 0.0);

	ExpectTrue(rule.SetValue(5.3));
	ExpectEQ(rule.ValueAsString(), _T("5.300"));
	ExpectEQ(rule.ValueAsInteger(), 5);
	ExpectEQ(rule.ValueAsDouble(), 5.3);

	rule.ClearValue();
	ExpectEQ(rule.ValueAsString(), _T("0.000"));
	ExpectEQ(rule.ValueAsInteger(), 0);
	ExpectEQ(rule.ValueAsDouble(), 0.0);

	ExpectTrue(rule.SetValue(_T("-5.7")));
	ExpectEQ(rule.ValueAsString(), _T("-5.700"));
	ExpectEQ(rule.ValueAsInteger(), -5);
	ExpectEQ(rule.ValueAsDouble(), -5.7);

#ifndef _DEBUG
	// These tests will assert
	ExpectEQ(rule.ValueAsDate(), CDateHelper::NullDate());

	ExpectFalse(rule.SetValue(_T("Fail")));
	ExpectEQ(rule.ValueAsString(), _T("-5.700"));

	ExpectFalse(rule.SetValue(COleDateTime::GetCurrentTime()));
	ExpectEQ(rule.ValueAsString(), _T("-5.700"));

	ExpectFalse(rule.SetMatchWholeWord(TRUE));
	ExpectEQ(rule.GetMatchWholeWord(), FALSE);
#endif
}

void CSEARCHPARAMSTest::TestDateParam(TDC_ATTRIBUTE nAttribID, FIND_ATTRIBTYPE nType)
{
	SEARCHPARAM rule;
	rule.SetAttribute(nAttribID, nType);

	ExpectEQ(rule.GetAttribute(), nAttribID);
	ExpectEQ(rule.GetAttribType(), nType);

	ExpectEQ(rule.GetOperator(), FOP_NONE);
	ExpectEQ(rule.GetAnd(), TRUE);
	ExpectEQ(rule.ValueAsDate(), CDateHelper::NullDate());

	if (rule.IsRelativeDate(FALSE))
	{
		ExpectTrue(rule.SetValue(_T("t")));
		ExpectEQ(rule.ValueAsString(), _T("t"));
		ExpectEQ(rule.ValueAsDate(), CDateHelper::GetDate(DHD_TODAY));

		rule.ClearValue();
		ExpectEQ(rule.ValueAsString(), _T(""));
		ExpectEQ(rule.ValueAsDate(), CDateHelper::NullDate());

		ExpectTrue(rule.SetValue(_T("M+10D")));
		ExpectEQ(rule.ValueAsString(), _T("M+10D"));
		ExpectEQ(rule.ValueAsDate(), (CDateHelper::GetDate(DHD_ENDTHISMONTH) + COleDateTimeSpan(10)));

#ifndef _DEBUG
		// These checks will assert
		ExpectTrue(rule.SetValue(_T("Success")));
		ExpectEQ(rule.ValueAsString(), _T("Success"));
		ExpectEQ(rule.ValueAsDate(), CDateHelper::NullDate());
		ExpectEQ(rule.ValueAsInteger(), 0);
		ExpectEQ(rule.ValueAsDouble(), 0.0);
		ExpectEQ(rule.GetTimeUnits(), TDCU_NULL);

		ExpectTrue(rule.SetValue(_T("-5.7")));
		ExpectEQ(rule.ValueAsString(), _T("-5.7"));
		ExpectEQ(rule.ValueAsDate(), CDateHelper::NullDate());
		ExpectEQ(rule.ValueAsInteger(), 0);
		ExpectEQ(rule.ValueAsDouble(), 0.0);
		ExpectEQ(rule.GetTimeUnits(), TDCU_NULL);

		ExpectFalse(rule.SetTimeUnits(TDCU_DAYS));
		ExpectEQ(rule.GetTimeUnits(), TDCU_NULL);

		ExpectFalse(rule.SetMatchWholeWord(TRUE));
		ExpectEQ(rule.GetMatchWholeWord(), FALSE);
#endif
	}
	else
	{
		ExpectTrue(rule.SetValue(_T("45678.123")));
		ExpectEQ(rule.ValueAsString(), _T("45678.123"));
		ExpectEQ(rule.ValueAsDate(), COleDateTime(45678.123));

		rule.ClearValue();
		ExpectEQ(rule.ValueAsString(), _T(""));
		ExpectEQ(rule.ValueAsDate(), CDateHelper::NullDate());

		ExpectTrue(rule.SetValue(_T("-45678.123")));
		ExpectEQ(rule.ValueAsString(), _T("-45678.123"));
		ExpectEQ(rule.ValueAsDate(), COleDateTime(-45678.123));

#ifndef _DEBUG
		// These checks will assert
		ExpectFalse(rule.SetValue(_T("Fail")));
		ExpectEQ(rule.ValueAsString(), _T("-45678.123"));
		ExpectEQ(rule.ValueAsInteger(), 0);
		ExpectEQ(rule.ValueAsDouble(), 0.0);
		ExpectEQ(rule.GetTimeUnits(), TDCU_NULL);

		ExpectFalse(rule.SetValue(-5.7));
		ExpectEQ(rule.ValueAsString(), _T("-45678.123"));
		ExpectEQ(rule.ValueAsInteger(), 0);
		ExpectEQ(rule.ValueAsDouble(), 0.0);
		ExpectEQ(rule.GetTimeUnits(), TDCU_NULL);

		ExpectFalse(rule.SetValue(5));
		ExpectEQ(rule.ValueAsString(), _T("-45678.123"));
		ExpectEQ(rule.ValueAsInteger(), 0);
		ExpectEQ(rule.ValueAsDouble(), 0.0);
		ExpectEQ(rule.GetTimeUnits(), TDCU_NULL);

		ExpectFalse(rule.SetTimeUnits(TDCU_DAYS));
		ExpectEQ(rule.GetTimeUnits(), TDCU_NULL);

		ExpectFalse(rule.SetMatchWholeWord(TRUE));
		ExpectEQ(rule.GetMatchWholeWord(), FALSE);
#endif
	}
}

void CSEARCHPARAMSTest::TestTimePeriodParam(TDC_ATTRIBUTE nAttribID)
{
	// Base test
	TestDecimalParam(nAttribID, FT_TIMEPERIOD);

	// Extra tests
	SEARCHPARAM rule(nAttribID);
	ExpectEQ(rule.GetTimeUnits(), TDCU_HOURS);

	ExpectTrue(rule.SetTimeUnits(TDCU_DAYS));
	ExpectEQ(rule.GetTimeUnits(), TDCU_DAYS);

	rule.ClearValue();
	ExpectEQ(rule.GetTimeUnits(), TDCU_HOURS);
}

void CSEARCHPARAMSTest::TestGroupParam(TDC_ATTRIBUTE nAttribID)
{
	SEARCHPARAM rule(nAttribID);

	ExpectEQ(rule.GetAttribute(), nAttribID);
	ExpectEQ(rule.GetAttribType(), FT_GROUP);
	ExpectEQ(rule.GetOperator(), FOP_NONE);
	ExpectEQ(rule.GetAnd(), TRUE);

#ifndef _DEBUG
	// These checks will assert
	ExpectEQ(rule.ValueAsDate(), CDateHelper::NullDate());

	ExpectFalse(rule.SetValue(_T("Fail")));
	ExpectEQ(rule.ValueAsString(), _T(""));
	ExpectEQ(rule.ValueAsInteger(), 0);
	ExpectEQ(rule.ValueAsDouble(), 0.0);

	ExpectFalse(rule.SetValue(10.6));
	ExpectEQ(rule.ValueAsString(), _T(""));
	ExpectEQ(rule.ValueAsInteger(), 0);
	ExpectEQ(rule.ValueAsDouble(), 0.0);

	ExpectFalse(rule.SetValue(COleDateTime::GetCurrentTime()));
	ExpectEQ(rule.ValueAsString(), _T(""));
	ExpectEQ(rule.ValueAsInteger(), 0);
	ExpectEQ(rule.ValueAsDouble(), 0.0);

	ExpectFalse(rule.SetMatchWholeWord(TRUE));
	ExpectEQ(rule.GetMatchWholeWord(), FALSE);
#endif
}

////////////////////////////////////////////////////////////////////////////////

/*
void CSEARCHPARAMSTest::TestSearchParamArray()
{
	CTDCScopedTest test(*this, _T("CSearchParamArray"));

	{
		CSearchParamArray aRules;

		// TODO
	}

	{
		CSearchParamArray aRules;

		// TODO
	}
	
	{
		CSearchParamArray aRules;

		// TODO
	}

	{
		CSearchParamArray aRules;

		// TODO
	}
}
*/

////////////////////////////////////////////////////////////////////////////////

/*
void CSEARCHPARAMSTest::TestSEARCHPARAMS()
{
	CTDCScopedTest test(*this, _T("SEARCHPARAMS"));

	// TODO
}
*/

