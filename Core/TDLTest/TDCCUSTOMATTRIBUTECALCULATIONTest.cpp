// TaskFileTest.cpp: implementation of the CTaskFileTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "TDCCUSTOMATTRIBUTECALCULATIONTest.h"

#include "..\ToDoList\TDCStruct.h"
#include "..\ToDoList\ToDoCtrlData.h"
#include "..\ToDoList\ToDoCtrlDataUtils.h"
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

const LPCTSTR ID_PRORITYMULTRISK = _T("Priority*Risk");
const LPCTSTR ID_TIMEESTMINUSVALUE = _T("Time-Value");
const LPCTSTR ID_DATEPLUSDOUBLE = _T("Date+Double");

//////////////////////////////////////////////////////////////////////

const DWORD ID_ROOT = 0;
const DWORD ID_PARENT = 1;
const DWORD ID_CHILD1 = 2;
const DWORD ID_CHILD2 = 3;

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
	TestTDCCUSTOMATTRIBUTECALCULATIONSimple();

	return GetTotals();
}

void CTDCCUSTOMATTRIBUTECALCULATIONTest::InitCustomAttributes(CTDCCustomAttribDefinitionArray& aAttribDef)
{
	aAttribDef.RemoveAll();

	// Simple attributes
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

	// Calculations
	TDCCUSTOMATTRIBUTECALCULATION calc;

	{
		attribDef.sUniqueID = ID_PRORITYMULTRISK;
		calc.Set(TDCA_PRIORITY,
				 NONE,
				 TDCCAC_MULTIPLY,
				 TDCA_RISK,
				 NONE);
		attribDef.SetCalculation(calc);
		aAttribDef.Add(attribDef);
	}

	{
		attribDef.sUniqueID = ID_TIMEESTMINUSVALUE;
		calc.Set(TDCA_TIMEESTIMATE,
				 NONE,
				 TDCCAC_SUBTRACT,
				 1.75);
		attribDef.SetCalculation(calc);
		aAttribDef.Add(attribDef);
	}

	{
		attribDef.sUniqueID = ID_DATEPLUSDOUBLE;
		calc.Set(TDCA_CUSTOMATTRIB,
				 ID_DATE,
				 TDCCAC_ADD,
				 TDCA_CUSTOMATTRIB,
				 ID_DOUBLE);
		attribDef.SetCalculation(calc);
		aAttribDef.Add(attribDef);
	}
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
		ExpectTrue(TDCCA_INVALID == aAttribDef.GetCalculationResultDataType(calc));
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
		ExpectTrue(TDCCA_INTEGER == aAttribDef.GetCalculationResultDataType(calc));
	}

	{
		TDCCUSTOMATTRIBUTECALCULATION calc;

		ExpectTrue(calc.Set(TDCA_PRIORITY, 
							NONE, 
							TDCCAC_ADD, 
							50.0));

		ExpectTrue(calc.IsValid(FALSE));
		ExpectTrue(aAttribDef.IsValidCalculation(calc));
		ExpectTrue(TDCCA_DOUBLE == aAttribDef.GetCalculationResultDataType(calc));
	}

	{
		TDCCUSTOMATTRIBUTECALCULATION calc;

		ExpectTrue(calc.Set(TDCA_TIMEESTIMATE, 
							NONE, 
							TDCCAC_MULTIPLY, 
							3.0));

		ExpectTrue(calc.IsValid(FALSE));
		ExpectTrue(aAttribDef.IsValidCalculation(calc));
		ExpectTrue(TDCCA_TIMEPERIOD == aAttribDef.GetCalculationResultDataType(calc));
	}

	{
		TDCCUSTOMATTRIBUTECALCULATION calc;

		ExpectFalse(calc.Set(TDCA_RISK, // Invalid for custom attribute type
							 ID_DATE, 
							 TDCCAC_ADD, 
							 3.0));

		ExpectFalse(calc.IsValid(FALSE));
		ExpectFalse(aAttribDef.IsValidCalculation(calc));
		ExpectTrue(TDCCA_INVALID == aAttribDef.GetCalculationResultDataType(calc));
	}

	{
		TDCCUSTOMATTRIBUTECALCULATION calc;

		ExpectTrue(calc.Set(TDCA_CUSTOMATTRIB, 
							ID_STRING, // Invalid
							TDCCAC_ADD, 
							3.0));
		
		ExpectTrue(calc.IsValid(FALSE));
		ExpectFalse(aAttribDef.IsValidCalculation(calc));
		ExpectTrue(TDCCA_INVALID == aAttribDef.GetCalculationResultDataType(calc));
	}

	{
		TDCCUSTOMATTRIBUTECALCULATION calc;

		ExpectTrue(calc.Set(TDCA_CUSTOMATTRIB, 
							ID_DATE, 
							TDCCAC_ADD, 
							3.0));

		ExpectTrue(calc.IsValid(FALSE));
		ExpectTrue(aAttribDef.IsValidCalculation(calc));
		ExpectTrue(TDCCA_DATE == aAttribDef.GetCalculationResultDataType(calc));
	}

	{
		TDCCUSTOMATTRIBUTECALCULATION calc;

		ExpectTrue(calc.Set(TDCA_CUSTOMATTRIB,
							ID_INTEGER_MULTILIST, // Invalid
							TDCCAC_ADD,
							3.0));

		ExpectTrue(calc.IsValid(FALSE));
		ExpectFalse(aAttribDef.IsValidCalculation(calc));
		ExpectTrue(TDCCA_INVALID == aAttribDef.GetCalculationResultDataType(calc));
	}

	{
		TDCCUSTOMATTRIBUTECALCULATION calc;

		ExpectTrue(calc.Set(TDCA_CUSTOMATTRIB, 
							ID_DATE, 
							TDCCAC_MULTIPLY, // Multiply invalid for dates
							3.0));

		ExpectTrue(calc.IsValid(FALSE));
		ExpectFalse(aAttribDef.IsValidCalculation(calc)); 
		ExpectTrue(TDCCA_INVALID == aAttribDef.GetCalculationResultDataType(calc));
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
		ExpectTrue(TDCCA_DATE == aAttribDef.GetCalculationResultDataType(calc));
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
		ExpectTrue(TDCCA_TIMEPERIOD == aAttribDef.GetCalculationResultDataType(calc));
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

				bool bValidResult = (TDCCA_INVALID != aAttribDef.GetCalculationResultDataType(calcManual));

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
				bool bValidResult = (TDCCA_INVALID != aAttribDef.GetCalculationResultDataType(calcManual));

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
				bool bValidResult = (TDCCA_INVALID != aAttribDef.GetCalculationResultDataType(calcManual));

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
				bool bValidResult = (TDCCA_INVALID != aAttribDef.GetCalculationResultDataType(calcManual));

				if (!ExpectEQ(bManualSuccess, bValidResult))
				{
					int breakpoint = 0;
				}
			}
		}
	}

	EndTest();
}

void CTDCCUSTOMATTRIBUTECALCULATIONTest::TestTDCCUSTOMATTRIBUTECALCULATIONSimple()
{
	CTDCCustomAttribDefinitionArray aAttribDefs;
	InitCustomAttributes(aAttribDefs);

	CTDCStyleMap aStyles; // Simple == No calculation styles

	CToDoCtrlData data(aStyles, aAttribDefs);
	InitDataModel(data);

	CTDCTaskCalculator calc(data);

	{
		int nAttrib = aAttribDefs.Find(ID_PRORITYMULTRISK);
		ExpectTrue(nAttrib != -1);

		if (nAttrib != -1)
		{
			const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aAttribDefs[nAttrib];
			double dValue = 0.0;

			data.SetTaskPriority(ID_PARENT, 3);
			data.SetTaskPriority(ID_CHILD1, 5);
			data.SetTaskPriority(ID_CHILD2, 7);

			data.SetTaskRisk(ID_PARENT, 4);
			data.SetTaskRisk(ID_CHILD1, 6);
			data.SetTaskRisk(ID_CHILD2, 8);

			ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, attribDef, dValue));
			ExpectEQ(dValue, 12.0);

			ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, attribDef, dValue));
			ExpectEQ(dValue, 30.0);

			ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, attribDef, dValue));
			ExpectEQ(dValue, 56.0);
		}

	}

	{
		int nAttrib = aAttribDefs.Find(ID_TIMEESTMINUSVALUE);
		ExpectTrue(nAttrib != -1);

		if (nAttrib != -1)
		{
			const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aAttribDefs[nAttrib];
			double dValue = 0.0;

			data.SetTaskTimeEstimate(ID_PARENT, TDCTIMEPERIOD(8, TDCU_DAYS));
			data.SetTaskTimeEstimate(ID_CHILD1, TDCTIMEPERIOD(6, TDCU_DAYS));
			data.SetTaskTimeEstimate(ID_CHILD2, TDCTIMEPERIOD(4, TDCU_DAYS));

			ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, attribDef, dValue, TDCU_DAYS));
			ExpectEQ(dValue, 6.25);

			ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, attribDef, dValue, TDCU_DAYS));
			ExpectEQ(dValue, 4.25);

			ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, attribDef, dValue, TDCU_DAYS));
			ExpectEQ(dValue, 2.25);
		}
	}

	{
		int nAttrib = aAttribDefs.Find(ID_DATEPLUSDOUBLE);
		ExpectTrue(nAttrib != -1);

		if (nAttrib != -1)
		{
			const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aAttribDefs[nAttrib];
			double dValue = 0.0;

			data.SetTaskCustomAttributeData(ID_PARENT, ID_DATE, _T("41254.0"));
			data.SetTaskCustomAttributeData(ID_CHILD1, ID_DATE, _T("41258.0"));
			data.SetTaskCustomAttributeData(ID_CHILD2, ID_DATE, _T("41262.0"));

			data.SetTaskCustomAttributeData(ID_PARENT, ID_DOUBLE, _T("4.0"));
			data.SetTaskCustomAttributeData(ID_CHILD1, ID_DOUBLE, _T("8.0"));
			data.SetTaskCustomAttributeData(ID_CHILD2, ID_DOUBLE, _T("2.0"));

			ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, attribDef, dValue));
			ExpectEQ(dValue, 41258.0);

			ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, attribDef, dValue));
			ExpectEQ(dValue, 41266.0);

			ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, attribDef, dValue));
			ExpectEQ(dValue, 41264.0);
		}
	}
}

void CTDCCUSTOMATTRIBUTECALCULATIONTest::InitDataModel(CToDoCtrlData& data)
{
	data.DeleteAllTasks();
	
	CUndoAction ua(data, TDCUAT_ADD, FALSE);
	TODOITEM tdiRef; // empty
	
	VERIFY(data.AddTask(ID_PARENT, data.NewTask(tdiRef), ID_ROOT, 0));
	VERIFY(data.AddTask(ID_CHILD1, data.NewTask(tdiRef), ID_PARENT, 0));
	VERIFY(data.AddTask(ID_CHILD2, data.NewTask(tdiRef), ID_PARENT, ID_CHILD1));
	
}

void CTDCCUSTOMATTRIBUTECALCULATIONTest::InitStyles(CTDCStyleMap& aStyles)
{
	// Initialise styles and custom attributes
	aStyles[TDCS_TREATSUBCOMPLETEDASDONE] = TRUE;
//	aStyles[TDCS_USEEARLIESTDUEDATE] = TRUE;
	aStyles[TDCS_USELATESTDUEDATE] = TRUE;
//	aStyles[TDCS_USEEARLIESTSTARTDATE] = TRUE;
	aStyles[TDCS_USELATESTSTARTDATE] = TRUE;
	aStyles[TDCS_USELATESTLASTMODIFIED] = TRUE;
	aStyles[TDCS_USEHIGHESTPRIORITY] = TRUE;
	aStyles[TDCS_USEHIGHESTRISK] = TRUE;
	aStyles[TDCS_AVERAGEPERCENTSUBCOMPLETION] = TRUE;
	aStyles[TDCS_INCLUDEDONEINAVERAGECALC] = TRUE;
	aStyles[TDCS_DONEHAVELOWESTPRIORITY] = TRUE;
	aStyles[TDCS_INCLUDEDONEINRISKCALC] = TRUE;
	aStyles[TDCS_INCLUDEDONEINPRIORITYCALC] = TRUE;
	aStyles[TDCS_DONEHAVELOWESTRISK] = TRUE;
	aStyles[TDCS_DUEHAVEHIGHESTPRIORITY] = TRUE;
}