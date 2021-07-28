// TaskFileTest.cpp: implementation of the CTaskFileTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "TDCCUSTOMATTRIBUTECALCULATIONTest.h"

#include "..\ToDoList\TDCCustomAttributeDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const LPCTSTR NONE = _T("");
const LPCTSTR ID_STRING = _T("String");
const LPCTSTR ID_INTEGER_MULTILIST = _T("IntegerMultiList");
const LPCTSTR ID_DATE = _T("Date");
const LPCTSTR ID_TIMEPERIOD = _T("TimePeriod");
const LPCTSTR ID_DOUBLE = _T("Double");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCCUSTOMATTRIBUTECALCULATIONTest::CTDCCUSTOMATTRIBUTECALCULATIONTest(const CTestUtils& utils) 
	: 
	CTDLTestBase(utils)
{

}

CTDCCUSTOMATTRIBUTECALCULATIONTest::~CTDCCUSTOMATTRIBUTECALCULATIONTest()
{

}

TESTRESULT CTDCCUSTOMATTRIBUTECALCULATIONTest::Run()
{
	ClearTotals();

	TestTDCCUSTOMATTRIBUTECALCULATION();

	return GetTotals();
}

void CTDCCUSTOMATTRIBUTECALCULATIONTest::InitCustomAttributes(CTDCCustomAttribDefinitionArray& aAttribDef)
{
	aAttribDef.RemoveAll();

	TDCCUSTOMATTRIBUTEDEFINITION attribDef;

	attribDef.sUniqueID = ID_STRING;
	attribDef.SetAttributeType(TDCCA_STRING); // invalid
	aAttribDef.Add(attribDef);

	attribDef.sUniqueID = ID_INTEGER_MULTILIST;
	attribDef.SetAttributeType(TDCCA_INTEGER | TDCCA_AUTOMULTILIST); // invalid
	aAttribDef.Add(attribDef);

	attribDef.sUniqueID = ID_DATE;
	attribDef.SetAttributeType(TDCCA_DATE);
	aAttribDef.Add(attribDef);

	attribDef.sUniqueID = ID_TIMEPERIOD;
	attribDef.SetAttributeType(TDCCA_TIMEPERIOD);
	aAttribDef.Add(attribDef);

	attribDef.sUniqueID = ID_DOUBLE;
	attribDef.SetAttributeType(TDCCA_DOUBLE);
	aAttribDef.Add(attribDef);
}

void CTDCCUSTOMATTRIBUTECALCULATIONTest::TestTDCCUSTOMATTRIBUTECALCULATION()
{
	BeginTest(_T("TDCCUSTOMATTRIBUTECALCULATION"));

	CTDCCustomAttribDefinitionArray aAttribDef;
	InitCustomAttributes(aAttribDef);

	{
		// NONE calculation
		TDCCUSTOMATTRIBUTECALCULATION calc;

		ExpectFalse(calc.IsValid(FALSE));
		ExpectTrue(calc.IsValid());
		ExpectTrue(TDCCA_INVALID == aAttribDef.GetResultDataType(calc));
	}

	{
		TDCCUSTOMATTRIBUTECALCULATION calc;

		ExpectTrue(calc.Set(TDCA_PRIORITY, 
							NONE, 
							TDCCAC_ADD, 
							TDCA_RISK, 
							NONE));

		ExpectTrue(calc.IsValid(FALSE));
		ExpectTrue(aAttribDef.IsValidCalculation(calc));
		ExpectTrue(TDCCA_INTEGER == aAttribDef.GetResultDataType(calc));
	}

	{
		TDCCUSTOMATTRIBUTECALCULATION calc;

		ExpectTrue(calc.Set(TDCA_PRIORITY, 
							NONE, 
							TDCCAC_ADD, 
							50.0));

		ExpectTrue(calc.IsValid(FALSE));
		ExpectTrue(aAttribDef.IsValidCalculation(calc));
		ExpectTrue(TDCCA_DOUBLE == aAttribDef.GetResultDataType(calc));
	}

	{
		TDCCUSTOMATTRIBUTECALCULATION calc;

		ExpectTrue(calc.Set(TDCA_TIMEESTIMATE, 
							NONE, 
							TDCCAC_MULTIPLY, 
							3.0));

		ExpectTrue(calc.IsValid(FALSE));
		ExpectTrue(aAttribDef.IsValidCalculation(calc));
		ExpectTrue(TDCCA_TIMEPERIOD == aAttribDef.GetResultDataType(calc));
	}

	{
		TDCCUSTOMATTRIBUTECALCULATION calc;

		ExpectFalse(calc.Set(TDCA_RISK, // Invalid for custom attribute type
							 ID_DATE, 
							 TDCCAC_ADD, 
							 3.0));

		ExpectFalse(calc.IsValid(FALSE));
		ExpectFalse(aAttribDef.IsValidCalculation(calc));
		ExpectTrue(TDCCA_INVALID == aAttribDef.GetResultDataType(calc));
	}

	{
		TDCCUSTOMATTRIBUTECALCULATION calc;

		ExpectTrue(calc.Set(TDCA_CUSTOMATTRIB, 
							ID_STRING, // Invalid
							TDCCAC_ADD, 
							3.0));
		
		ExpectTrue(calc.IsValid(FALSE));
		ExpectFalse(aAttribDef.IsValidCalculation(calc));
		ExpectTrue(TDCCA_INVALID == aAttribDef.GetResultDataType(calc));
	}

	{
		TDCCUSTOMATTRIBUTECALCULATION calc;

		ExpectTrue(calc.Set(TDCA_CUSTOMATTRIB, 
							ID_DATE, 
							TDCCAC_ADD, 
							3.0));

		ExpectTrue(calc.IsValid(FALSE));
		ExpectTrue(aAttribDef.IsValidCalculation(calc));
		ExpectTrue(TDCCA_DATE == aAttribDef.GetResultDataType(calc));
	}

	{
		TDCCUSTOMATTRIBUTECALCULATION calc;

		ExpectTrue(calc.Set(TDCA_CUSTOMATTRIB,
							ID_INTEGER_MULTILIST, // Invalid
							TDCCAC_ADD,
							3.0));

		ExpectTrue(calc.IsValid(FALSE));
		ExpectFalse(aAttribDef.IsValidCalculation(calc));
		ExpectTrue(TDCCA_INVALID == aAttribDef.GetResultDataType(calc));
	}

	{
		TDCCUSTOMATTRIBUTECALCULATION calc;

		ExpectTrue(calc.Set(TDCA_CUSTOMATTRIB, 
							ID_DATE, 
							TDCCAC_MULTIPLY, // Multiply invalid for dates
							3.0));

		ExpectTrue(calc.IsValid(FALSE));
		ExpectFalse(aAttribDef.IsValidCalculation(calc)); 
		ExpectTrue(TDCCA_INVALID == aAttribDef.GetResultDataType(calc));
	}

	{
		TDCCUSTOMATTRIBUTECALCULATION calc;

		// Add time-period to date
		ExpectTrue(calc.Set(TDCA_CUSTOMATTRIB, 
							ID_DATE, 
							TDCCAC_ADD, 
							TDCA_CUSTOMATTRIB, 
							ID_TIMEPERIOD));

		ExpectTrue(calc.IsValid(FALSE));
		ExpectTrue(aAttribDef.IsValidCalculation(calc));
		ExpectTrue(TDCCA_DATE == aAttribDef.GetResultDataType(calc));
	}

	{
		TDCCUSTOMATTRIBUTECALCULATION calc;

		// Add time-period to time-period
		ExpectTrue(calc.Set(TDCA_CUSTOMATTRIB, 
							ID_TIMEPERIOD, 
							TDCCAC_ADD, 
							TDCA_CUSTOMATTRIB, 
							ID_TIMEPERIOD));

		ExpectTrue(calc.IsValid(FALSE));
		ExpectTrue(aAttribDef.IsValidCalculation(calc));
		ExpectTrue(TDCCA_TIMEPERIOD == aAttribDef.GetResultDataType(calc));
	}

	{
		// Compare the result of initialising the calculation via a Set() function
		// with the result of constructing it manually and then checking its validity
		//
		// Built-in attributes and Built-in attributes
		for (int nAttrib1 = TDCA_FIRST_ATTRIBUTE; nAttrib1 <= TDCA_LAST_REALATTRIBUTE; nAttrib1++)
		{
			TDC_ATTRIBUTE nAttrib1ID = (TDC_ATTRIBUTE)nAttrib1;

			for (int nAttrib2 = TDCA_FIRST_ATTRIBUTE; nAttrib2 <= TDCA_LAST_REALATTRIBUTE; nAttrib2++)
			{
				TDC_ATTRIBUTE nAttrib2ID = (TDC_ATTRIBUTE)nAttrib2;

				TDCCUSTOMATTRIBUTECALCULATION calcSet;
				bool bSetSuccess = (FALSE != calcSet.Set(nAttrib1ID, 
														 NONE, 
														 TDCCAC_ADD, 
														 nAttrib2ID, 
														 NONE));

				TDCCUSTOMATTRIBUTECALCULATION calcManual;
				calcManual.opFirst.nAttribID			= nAttrib1ID;
				calcManual.opFirst.sCustAttribID		= NONE;
				calcManual.nOperator					= TDCCAC_ADD;
				calcManual.opSecond.nAttribID			= nAttrib2ID;
				calcManual.opSecond.sCustAttribID		= NONE;

				bool bManualSuccess = (FALSE != calcManual.IsValid(FALSE));

				if (!ExpectEQ(bSetSuccess, bManualSuccess))
				{
					int breakpoint = 0;
				}

				bool bValidResult = (TDCCA_INVALID != aAttribDef.GetResultDataType(calcManual));

				if (!ExpectEQ(bManualSuccess, bValidResult))
				{
					int breakpoint = 0;
				}
			}
		}
	}

	{
		// Compare the result of initialising the calculation via a Set() function
		// with the result of constructing it manually and then checking its validity
		//
		// Built-in attributes and custom attributes
		for (int nAttrib2 = TDCA_FIRST_ATTRIBUTE; nAttrib2 <= TDCA_LAST_REALATTRIBUTE; nAttrib2++)
		{
			TDC_ATTRIBUTE nAttrib1ID = (TDC_ATTRIBUTE)nAttrib2;

			for (int nDef = 0; nDef < aAttribDef.GetSize(); nDef++)
			{
				TDCCUSTOMATTRIBUTECALCULATION calcSet;
				bool bSetSuccess = (FALSE != calcSet.Set(nAttrib1ID, 
														 NONE, 
														 TDCCAC_SUBTRACT, 
														 TDCA_CUSTOMATTRIB, 
														 aAttribDef[nDef].sUniqueID));

				TDCCUSTOMATTRIBUTECALCULATION calcManual;
				calcManual.opFirst.nAttribID			= nAttrib1ID;
				calcManual.opFirst.sCustAttribID		= NONE;
				calcManual.nOperator					= TDCCAC_SUBTRACT;
				calcManual.opSecond.nAttribID			= TDCA_CUSTOMATTRIB;
				calcManual.opSecond.sCustAttribID		= aAttribDef[nDef].sUniqueID;

				bool bManualSuccess = (FALSE != calcManual.IsValid(FALSE));

				if (!ExpectEQ(bSetSuccess, bManualSuccess))
				{
					int breakpoint = 0;
				}

				bManualSuccess = (FALSE != aAttribDef.IsValidCalculation(calcManual));
				bool bValidResult = (TDCCA_INVALID != aAttribDef.GetResultDataType(calcManual));

				if (!ExpectEQ(bManualSuccess, bValidResult))
				{
					int breakpoint = 0;
				}
			}
		}
	}

	{
		// Compare the result of initialising the calculation via a Set() function
		// with the result of constructing it manually and then checking its validity
		//
		// Custom attributes and custom attributes
		for (int nDef1 = 0; nDef1 < aAttribDef.GetSize(); nDef1++)
		{
			for (int nDef2 = 0; nDef2 < aAttribDef.GetSize(); nDef2++)
			{
				TDCCUSTOMATTRIBUTECALCULATION calcSet;
				bool bSetSuccess = (FALSE != calcSet.Set(TDCA_CUSTOMATTRIB, 
														 aAttribDef[nDef1].sUniqueID, 
														 TDCCAC_MULTIPLY, 
														 TDCA_CUSTOMATTRIB, 
														 aAttribDef[nDef2].sUniqueID));

				TDCCUSTOMATTRIBUTECALCULATION calcManual;
				calcManual.opFirst.nAttribID			= TDCA_CUSTOMATTRIB;
				calcManual.opFirst.sCustAttribID		= aAttribDef[nDef1].sUniqueID;
				calcManual.nOperator					= TDCCAC_MULTIPLY;
				calcManual.opSecond.nAttribID			= TDCA_CUSTOMATTRIB;
				calcManual.opSecond.sCustAttribID		= aAttribDef[nDef2].sUniqueID;

				bool bManualSuccess = (FALSE != calcManual.IsValid(FALSE));

				if (!ExpectEQ(bSetSuccess, bManualSuccess))
				{
					int breakpoint = 0;
				}

				bManualSuccess = (FALSE != aAttribDef.IsValidCalculation(calcManual));
				bool bValidResult = (TDCCA_INVALID != aAttribDef.GetResultDataType(calcManual));

				if (!ExpectEQ(bManualSuccess, bValidResult))
				{
					int breakpoint = 0;
				}
			}
		}
	}

	{
		// Compare the result of initialising the calculation via a Set() function
		// with the result of constructing it manually and then checking its validity
		//
		// Custom attributes and built-in attributes
		for (int nAttrib = TDCA_FIRST_ATTRIBUTE; nAttrib <= TDCA_LAST_REALATTRIBUTE; nAttrib++)
		{
			TDC_ATTRIBUTE nAttribID = (TDC_ATTRIBUTE)nAttrib;

			for (int nDef = 0; nDef < aAttribDef.GetSize(); nDef++)
			{
				TDCCUSTOMATTRIBUTECALCULATION calcSet;
				bool bSetSuccess = (FALSE != calcSet.Set(TDCA_CUSTOMATTRIB, 
														 aAttribDef[nDef].sUniqueID, 
														 TDCCAC_SUBTRACT, 
														 nAttribID, 
														 NONE));

				TDCCUSTOMATTRIBUTECALCULATION calcManual;
				calcManual.opFirst.nAttribID			= TDCA_CUSTOMATTRIB;
				calcManual.opFirst.sCustAttribID		= aAttribDef[nDef].sUniqueID;
				calcManual.nOperator					= TDCCAC_SUBTRACT;
				calcManual.opSecond.nAttribID			= nAttribID;
				calcManual.opSecond.sCustAttribID		= NONE;

				bool bManualSuccess = (FALSE != calcManual.IsValid(FALSE));

				if (!ExpectEQ(bSetSuccess, bManualSuccess))
				{
					int breakpoint = 0;
				}

				bManualSuccess = (FALSE != aAttribDef.IsValidCalculation(calcManual));
				bool bValidResult = (TDCCA_INVALID != aAttribDef.GetResultDataType(calcManual));

				if (!ExpectEQ(bManualSuccess, bValidResult))
				{
					int breakpoint = 0;
				}
			}
		}
	}

	EndTest();
}

