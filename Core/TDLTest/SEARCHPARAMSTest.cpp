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
	TestSearchParamArray();
	TestSEARCHPARAMS();

	return GetTotals();
}

void CSEARCHPARAMSTest::TestSEARCHPARAM()
{
	CTDCScopedTest test(*this, _T("SEARCHPARAM"));

	// Undefined type
	{
		SEARCHPARAM rule;

		ExpectEQ(rule.GetAttribute(), TDCA_NONE);
		ExpectEQ(rule.GetAttribType(), FT_NONE);
		ExpectEQ(rule.GetOperator(), FOP_NONE);
		ExpectEQ(rule.GetAnd(), TRUE);

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
#endif
	}

	// String Type
	{
		SEARCHPARAM rule(TDCA_ALLOCBY);

		ExpectEQ(rule.GetAttribute(), TDCA_ALLOCBY);
		ExpectEQ(rule.GetAttribType(), FT_STRING);
		ExpectEQ(rule.GetOperator(), FOP_NONE);
		ExpectEQ(rule.GetAnd(), TRUE);
		ExpectEQ(rule.ValueAsString(), _T(""));

		rule.SetValue(_T("Success"));
		ExpectEQ(rule.ValueAsString(), _T("Success"));

		rule.ClearValue();
		ExpectEQ(rule.ValueAsString(), _T(""));

#ifndef _DEBUG
		// These checks will assert
		ExpectEQ(rule.ValueAsInteger(), 0);
		ExpectEQ(rule.ValueAsDouble(), 0.0);
		ExpectEQ(rule.ValueAsDate(), CDateHelper::NullDate());

		rule.SetValue(_T("Success"));
		ExpectEQ(rule.ValueAsString(), _T("Success"));

		rule.SetValue(5);
		ExpectEQ(rule.ValueAsString(), _T("Success"));

		rule.SetValue(7.0);
		ExpectEQ(rule.ValueAsString(), _T("Success"));

		rule.SetValue(COleDateTime::GetCurrentTime());
		ExpectEQ(rule.ValueAsString(), _T("Success"));
#endif
	}

	// Integer type
	{
		SEARCHPARAM rule(TDCA_PRIORITY);

		ExpectEQ(rule.GetAttribute(), TDCA_PRIORITY);
		ExpectEQ(rule.GetAttribType(), FT_INTEGER);
		ExpectEQ(rule.GetOperator(), FOP_NONE);
		ExpectEQ(rule.GetAnd(), TRUE);

		ExpectEQ(rule.ValueAsString(), _T("0"));
		ExpectEQ(rule.ValueAsInteger(), 0);
		ExpectEQ(rule.ValueAsDouble(), 0.0);

		rule.SetValue(5);
		ExpectEQ(rule.ValueAsString(), _T("5"));
		ExpectEQ(rule.ValueAsInteger(), 5);
		ExpectEQ(rule.ValueAsDouble(), 5.0);

		rule.ClearValue();
		ExpectEQ(rule.ValueAsString(), _T("0"));
		ExpectEQ(rule.ValueAsInteger(), 0);
		ExpectEQ(rule.ValueAsDouble(), 0.0);

		rule.SetValue(_T("-5"));
		ExpectEQ(rule.ValueAsInteger(), -5);
		ExpectEQ(rule.ValueAsDouble(), -5.0);

#ifndef _DEBUG
		// These checks will assert
		ExpectEQ(rule.ValueAsDate(), CDateHelper::NullDate());

		rule.SetValue(_T("Fail"));
		ExpectEQ(rule.ValueAsString(), _T("-5"));
		ExpectEQ(rule.ValueAsInteger(), -5);
		ExpectEQ(rule.ValueAsDouble(), -5.0);

		rule.SetValue(10.6);
		ExpectEQ(rule.ValueAsString(), _T("-5"));
		ExpectEQ(rule.ValueAsInteger(), -5);
		ExpectEQ(rule.ValueAsDouble(), -5.0);

		rule.SetValue(COleDateTime::GetCurrentTime());
		ExpectEQ(rule.ValueAsString(), _T("-5"));
		ExpectEQ(rule.ValueAsInteger(), -5);
		ExpectEQ(rule.ValueAsDouble(), -5.0);
#endif
	}

	// Double types
	{
		// Plain double
		{
			SEARCHPARAM rule(TDCA_COST);

			ExpectEQ(rule.GetAttribute(), TDCA_COST);
			ExpectEQ(rule.GetAttribType(), FT_DOUBLE);
			ExpectEQ(rule.GetOperator(), FOP_NONE);
			ExpectEQ(rule.GetAnd(), TRUE);
			ExpectEQ(rule.ValueAsString(), _T("0.000"));
			ExpectEQ(rule.ValueAsInteger(), 0);
			ExpectEQ(rule.ValueAsDouble(), 0.0);

			rule.SetValue(5.3);
			ExpectEQ(rule.ValueAsString(), _T("5.300"));
			ExpectEQ(rule.ValueAsInteger(), 5);
			ExpectEQ(rule.ValueAsDouble(), 5.3);

			rule.ClearValue();
			ExpectEQ(rule.ValueAsString(), _T("0.000"));
			ExpectEQ(rule.ValueAsInteger(), 0);
			ExpectEQ(rule.ValueAsDouble(), 0.0);

			rule.SetValue(_T("-5.7"));
			ExpectEQ(rule.ValueAsString(), _T("-5.700"));
			ExpectEQ(rule.ValueAsInteger(), -5);
			ExpectEQ(rule.ValueAsDouble(), -5.7);

#ifndef _DEBUG
			// These tests will assert
			ExpectEQ(rule.ValueAsDate(), CDateHelper::NullDate());

			rule.SetValue(_T("Fail"));
			ExpectEQ(rule.ValueAsString(), _T("-5.700"));

			rule.SetValue(COleDateTime::GetCurrentTime());
			ExpectEQ(rule.ValueAsString(), _T("-5.700"));
#endif
		}

		// Time period
		{
			SEARCHPARAM rule(TDCA_TIMEESTIMATE);

			ExpectEQ(rule.GetAttribute(), TDCA_TIMEESTIMATE);
			ExpectEQ(rule.GetAttribType(), FT_TIMEPERIOD);
			ExpectEQ(rule.GetOperator(), FOP_NONE);
			ExpectEQ(rule.GetAnd(), TRUE);
			ExpectEQ(rule.ValueAsString(), _T("0.000"));
			ExpectEQ(rule.ValueAsInteger(), 0);
			ExpectEQ(rule.ValueAsDouble(), 0.0);
			ExpectEQ(rule.GetTimeUnits(), TDCU_NULL);

			rule.SetTimeUnits(TDCU_DAYS);
			ExpectEQ(rule.GetTimeUnits(), TDCU_DAYS);

			rule.SetValue(5.6);
			ExpectEQ(rule.ValueAsString(), _T("5.600"));
			ExpectEQ(rule.ValueAsInteger(), 5);
			ExpectEQ(rule.ValueAsDouble(), 5.6);
			ExpectEQ(rule.GetTimeUnits(), TDCU_DAYS);

			rule.ClearValue();
			ExpectEQ(rule.ValueAsString(), _T("0.000"));
			ExpectEQ(rule.ValueAsInteger(), 0);
			ExpectEQ(rule.ValueAsDouble(), 0.0);
			ExpectEQ(rule.GetTimeUnits(), TDCU_HOURS); // Current quirk
			
			rule.SetValue(_T("-5.7"));
			ExpectEQ(rule.ValueAsString(), _T("-5.700"));
			ExpectEQ(rule.ValueAsInteger(), -5);
			ExpectEQ(rule.ValueAsDouble(), -5.7);

#ifndef _DEBUG
			// These checks will assert
			ExpectEQ(rule.ValueAsDate(), CDateHelper::NullDate());

			rule.SetValue(_T("Fail"));
			ExpectEQ(rule.ValueAsString(), _T("-5.700"));

			rule.SetValue(COleDateTime::GetCurrentTime());
			ExpectEQ(rule.ValueAsString(), _T("-5.700"));
#endif
		}
	}

}

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

void CSEARCHPARAMSTest::TestSEARCHPARAMS()
{
	CTDCScopedTest test(*this, _T("SEARCHPARAMS"));

	// TODO
}

