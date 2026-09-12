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

const LPCTSTR NONE							= _T("");

const LPCTSTR ID_CUSTSTRING					= _T("CustomString");
const LPCTSTR ID_CUSTINTEGERMULTILIST		= _T("CustomIntegerMultiList");
const LPCTSTR ID_CUSTDATE					= _T("CustomDate");
const LPCTSTR ID_CUSTTIMEPERIOD				= _T("CustomTimePeriod");
const LPCTSTR ID_CUSTDOUBLE					= _T("CustomDouble");

const LPCTSTR ID_PRORITYMULRISK				= _T("Priority * Risk");
const LPCTSTR ID_TIMEESTMINUSDOUBLE			= _T("TimeEst - Double");
const LPCTSTR ID_CUSTDATEPLUSCUSTDOUBLE		= _T("CustomDate + CustomDouble");

const LPCTSTR ID_CUSTDATEMINUSSTARTDATE		= _T("CustomDate - StartDate");
const LPCTSTR ID_DUEDATEMINUSSTARTDATE		= _T("DueDate - StartDate");
const LPCTSTR ID_CUSTDATEMINUSDUEDATE		= _T("CustomDate - DueDate");

//////////////////////////////////////////////////////////////////////

const DWORD ID_ROOT = 0;
const DWORD ID_PARENT = 1;
const DWORD ID_CHILD1 = 2;
const DWORD ID_CHILD2 = 3;

//////////////////////////////////////////////////////////////////////

class CTempCustAttribDef
{
public:
	CTempCustAttribDef(LPCTSTR szAttribID, CTDCCustomAttribDefinitionArray& aAttribDefs, DWORD dwFeature)
		:
		m_sAttribID(szAttribID),
		m_aAttribDefs(aAttribDefs),
		m_nAttrib(aAttribDefs.Find(m_sAttribID))
	{
		m_dwOrgFeatures = m_aAttribDefs[m_nAttrib].dwFeatures;
		m_aAttribDefs[m_nAttrib].dwFeatures = dwFeature;
	}

	~CTempCustAttribDef()
	{
		m_aAttribDefs[m_nAttrib].dwFeatures = m_dwOrgFeatures;
	}

	operator const TDCCUSTOMATTRIBUTEDEFINITION()
	{
		return m_aAttribDefs[m_nAttrib];
	}

protected:
	CString m_sAttribID;
	CTDCCustomAttribDefinitionArray& m_aAttribDefs;
	DWORD m_dwOrgFeatures;
	int m_nAttrib;
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCCUSTOMATTRIBUTECALCULATIONTest::CTDCCUSTOMATTRIBUTECALCULATIONTest(const CTestUtils& utils) 
	: 
	CTDLTestBase(_T("CTDCCUSTOMATTRIBUTECALCULATIONTest"), utils)
{

}

CTDCCUSTOMATTRIBUTECALCULATIONTest::~CTDCCUSTOMATTRIBUTECALCULATIONTest()
{

}

TESTRESULT CTDCCUSTOMATTRIBUTECALCULATIONTest::Run()
{
	ClearTotals();

	TestTDCCUSTOMATTRIBUTECALCULATIONValidity();
	TestTDCCUSTOMATTRIBUTECALCULATIONResults();

	return GetTotals();
}

void CTDCCUSTOMATTRIBUTECALCULATIONTest::InitCustomAttributes(CTDCCustomAttribDefinitionArray& aAttribDef)
{
	aAttribDef.RemoveAll();

	// Simple attributes
	TDCCUSTOMATTRIBUTEDEFINITION attribDef;

	attribDef.sUniqueID = ID_CUSTSTRING;
	attribDef.SetAttributeType(TDCCA_STRING); // invalid
	aAttribDef.Add(attribDef);

	attribDef.sUniqueID = ID_CUSTINTEGERMULTILIST;
	attribDef.SetAttributeType(TDCCA_INTEGER | TDCCA_AUTOMULTILIST); // invalid
	aAttribDef.Add(attribDef);

	attribDef.sUniqueID = ID_CUSTDATE;
	attribDef.SetAttributeType(TDCCA_DATE);
	aAttribDef.Add(attribDef);

	attribDef.sUniqueID = ID_CUSTTIMEPERIOD;
	attribDef.SetAttributeType(TDCCA_TIMEPERIOD);
	aAttribDef.Add(attribDef);

	attribDef.sUniqueID = ID_CUSTDOUBLE;
	attribDef.SetAttributeType(TDCCA_DOUBLE);
	aAttribDef.Add(attribDef);

	// Calculations
	{
		TDCCUSTOMATTRIBUTECALCULATION calc;

		// Priority * Risk
		{
			attribDef.sUniqueID = ID_PRORITYMULRISK;
			calc.Set(TDCA_PRIORITY,
					 NONE,
					 TDCCAC_MULTIPLY,
					 TDCA_RISK,
					 NONE);
			attribDef.SetCalculation(calc);
			aAttribDef.Add(attribDef);
		}

		// Time Estimate - 1.75
		{
			attribDef.sUniqueID = ID_TIMEESTMINUSDOUBLE;
			calc.Set(TDCA_TIMEESTIMATE,
					 NONE,
					 TDCCAC_SUBTRACT,
					 1.75);
			attribDef.SetCalculation(calc);
			aAttribDef.Add(attribDef);
		}

		// Custom date - Custom double
		{
			attribDef.sUniqueID = ID_CUSTDATEPLUSCUSTDOUBLE;
			calc.Set(TDCA_CUSTOMATTRIB,
					 ID_CUSTDATE,
					 TDCCAC_ADD,
					 TDCA_CUSTOMATTRIB,
					 ID_CUSTDOUBLE);
			attribDef.SetCalculation(calc);
			aAttribDef.Add(attribDef);
		}

		// Subtracting dates from dates
		{
			// Due date - Start date
			{
				attribDef.sUniqueID = ID_DUEDATEMINUSSTARTDATE;
				calc.Set(TDCA_DUEDATE,
						 NONE,
						 TDCCAC_SUBTRACT,
						 TDCA_STARTDATE,
						 NONE);
				attribDef.SetCalculation(calc);
				aAttribDef.Add(attribDef);
			}

			// Custom date - Start date
			{
				attribDef.sUniqueID = ID_CUSTDATEMINUSSTARTDATE;
				calc.Set(TDCA_CUSTOMATTRIB,
						 ID_CUSTDATE,
						 TDCCAC_SUBTRACT,
						 TDCA_STARTDATE,
						 NONE);
				attribDef.SetCalculation(calc);
				aAttribDef.Add(attribDef);
			}

			// Custom date - Due date
			{
				attribDef.sUniqueID = ID_CUSTDATEMINUSDUEDATE;
				calc.Set(TDCA_CUSTOMATTRIB,
						 ID_CUSTDATE,
						 TDCCAC_SUBTRACT,
						 TDCA_DUEDATE,
						 NONE);
				attribDef.SetCalculation(calc);
				aAttribDef.Add(attribDef);
			}
		}
	}
}

void CTDCCUSTOMATTRIBUTECALCULATIONTest::TestTDCCUSTOMATTRIBUTECALCULATIONValidity()
{
	CTDCScopedTest test(*this, _T("TDCCUSTOMATTRIBUTECALCULATION"));

	CTDCCustomAttribDefinitionArray aAttribDef;
	InitCustomAttributes(aAttribDef);

	// Testing validity of various operand/operator combinations
	{
		{
			// Undefined calculation
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
								 ID_CUSTDATE,
								 TDCCAC_ADD,
								 3.0));

			ExpectFalse(calc.IsValid(FALSE));
			ExpectFalse(aAttribDef.IsValidCalculation(calc));
			ExpectTrue(TDCCA_INVALID == aAttribDef.GetCalculationResultDataType(calc));
		}

		{
			TDCCUSTOMATTRIBUTECALCULATION calc;

			ExpectTrue(calc.Set(TDCA_CUSTOMATTRIB,
								ID_CUSTSTRING, // Invalid
								TDCCAC_ADD,
								3.0));

			ExpectTrue(calc.IsValid(FALSE));
			ExpectFalse(aAttribDef.IsValidCalculation(calc));
			ExpectTrue(TDCCA_INVALID == aAttribDef.GetCalculationResultDataType(calc));
		}

		{
			TDCCUSTOMATTRIBUTECALCULATION calc;

			ExpectTrue(calc.Set(TDCA_CUSTOMATTRIB,
								ID_CUSTDATE,
								TDCCAC_ADD,
								3.0));

			ExpectTrue(calc.IsValid(FALSE));
			ExpectTrue(aAttribDef.IsValidCalculation(calc));
			ExpectTrue(TDCCA_DATE == aAttribDef.GetCalculationResultDataType(calc));
		}

		{
			TDCCUSTOMATTRIBUTECALCULATION calc;

			ExpectTrue(calc.Set(TDCA_CUSTOMATTRIB,
								ID_CUSTINTEGERMULTILIST, // Invalid
								TDCCAC_ADD,
								3.0));

			ExpectTrue(calc.IsValid(FALSE));
			ExpectFalse(aAttribDef.IsValidCalculation(calc));
			ExpectTrue(TDCCA_INVALID == aAttribDef.GetCalculationResultDataType(calc));
		}

		{
			TDCCUSTOMATTRIBUTECALCULATION calc;

			ExpectTrue(calc.Set(TDCA_CUSTOMATTRIB,
								ID_CUSTDATE,
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
								ID_CUSTDATE,
								TDCCAC_ADD,
								TDCA_CUSTOMATTRIB,
								ID_CUSTTIMEPERIOD));

			ExpectTrue(calc.IsValid(FALSE));
			ExpectTrue(aAttribDef.IsValidCalculation(calc));
			ExpectTrue(TDCCA_DATE == aAttribDef.GetCalculationResultDataType(calc));
		}

		{
			TDCCUSTOMATTRIBUTECALCULATION calc;

			// Add time-period to time-period
			ExpectTrue(calc.Set(TDCA_CUSTOMATTRIB,
								ID_CUSTTIMEPERIOD,
								TDCCAC_ADD,
								TDCA_CUSTOMATTRIB,
								ID_CUSTTIMEPERIOD));

			ExpectTrue(calc.IsValid(FALSE));
			ExpectTrue(aAttribDef.IsValidCalculation(calc));
			ExpectTrue(TDCCA_TIMEPERIOD == aAttribDef.GetCalculationResultDataType(calc));
		}

		{
			// Compare the result of initialising the calculation via a Set() function
			// with the result of constructing it manually and then checking its validity
			//
			// Built-in attributes and Built-in attributes
			for (int nAtt1 = TDCA_FIRST_ATTRIBUTE; nAtt1 <= TDCA_LAST_REALATTRIBUTE; nAtt1++)
			{
				TDC_ATTRIBUTE nAttrib1ID = (TDC_ATTRIBUTE)nAtt1;

				for (int nAtt2 = TDCA_FIRST_ATTRIBUTE; nAtt2 <= TDCA_LAST_REALATTRIBUTE; nAtt2++)
				{
					TDC_ATTRIBUTE nAttrib2ID = (TDC_ATTRIBUTE)nAtt2;

					TDCCUSTOMATTRIBUTECALCULATION calcSet;
					bool bSetSuccess = (FALSE != calcSet.Set(nAttrib1ID,
															 NONE,
															 TDCCAC_ADD,
															 nAttrib2ID,
															 NONE));

					TDCCUSTOMATTRIBUTECALCULATION calcManual;
					calcManual.opFirst.nAttributeID = nAttrib1ID;
					calcManual.opFirst.sCustAttribID = NONE;
					calcManual.nOperator = TDCCAC_ADD;
					calcManual.opSecond.nAttributeID = nAttrib2ID;
					calcManual.opSecond.sCustAttribID = NONE;

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
	}

	// Compare the result of initialising the calculation via a Set() function
	// with the result of constructing it manually and then checking its validity
	{
		// Built-in attributes and custom attributes
		for (int nAtt2 = TDCA_FIRST_ATTRIBUTE; nAtt2 <= TDCA_LAST_REALATTRIBUTE; nAtt2++)
		{
			TDC_ATTRIBUTE nAttrib1ID = (TDC_ATTRIBUTE)nAtt2;

			for (int nDef = 0; nDef < aAttribDef.GetSize(); nDef++)
			{
				TDCCUSTOMATTRIBUTECALCULATION calcSet;
				bool bSetSuccess = (FALSE != calcSet.Set(nAttrib1ID, 
														 NONE, 
														 TDCCAC_SUBTRACT, 
														 TDCA_CUSTOMATTRIB, 
														 aAttribDef[nDef].sUniqueID));

				TDCCUSTOMATTRIBUTECALCULATION calcManual;
				calcManual.opFirst.nAttributeID			= nAttrib1ID;
				calcManual.opFirst.sCustAttribID		= NONE;
				calcManual.nOperator					= TDCCAC_SUBTRACT;
				calcManual.opSecond.nAttributeID		= TDCA_CUSTOMATTRIB;
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

	// Compare the result of initialising the calculation via a Set() function
	// with the result of constructing it manually and then checking its validity
	{
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
				calcManual.opFirst.nAttributeID			= TDCA_CUSTOMATTRIB;
				calcManual.opFirst.sCustAttribID		= aAttribDef[nDef1].sUniqueID;
				calcManual.nOperator					= TDCCAC_MULTIPLY;
				calcManual.opSecond.nAttributeID		= TDCA_CUSTOMATTRIB;
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

	// Compare the result of initialising the calculation via a Set() function
	// with the result of constructing it manually and then checking its validity
	{
		// Custom attributes and built-in attributes
		for (int nAtt = TDCA_FIRST_ATTRIBUTE; nAtt <= TDCA_LAST_REALATTRIBUTE; nAtt++)
		{
			TDC_ATTRIBUTE nAttribID = (TDC_ATTRIBUTE)nAtt;

			for (int nDef = 0; nDef < aAttribDef.GetSize(); nDef++)
			{
				TDCCUSTOMATTRIBUTECALCULATION calcSet;
				bool bSetSuccess = (FALSE != calcSet.Set(TDCA_CUSTOMATTRIB, 
														 aAttribDef[nDef].sUniqueID, 
														 TDCCAC_SUBTRACT, 
														 nAttribID, 
														 NONE));

				TDCCUSTOMATTRIBUTECALCULATION calcManual;
				calcManual.opFirst.nAttributeID			= TDCA_CUSTOMATTRIB;
				calcManual.opFirst.sCustAttribID		= aAttribDef[nDef].sUniqueID;
				calcManual.nOperator					= TDCCAC_SUBTRACT;
				calcManual.opSecond.nAttributeID		= nAttribID;
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
}

/////////////////////////////////////////////////////////////////////////

void CTDCCUSTOMATTRIBUTECALCULATIONTest::TestTDCCUSTOMATTRIBUTECALCULATIONResults()
{
	CTDCScopedTest test(*this, _T("TDCCUSTOMATTRIBUTECALCULATION (Results)"));

	CTDCCustomAttribDefinitionArray aAttribDefs;
	InitCustomAttributes(aAttribDefs);

	CToDoCtrlData data(CTDCStyleMap(), aAttribDefs); // No calculation styles
	InitDataModel(data);

	CTDCTaskCalculator calc(data);
	double dValue = 0.0;

	// Priority * Risk
	{
		CTempCustAttribDef tempCalcDef(ID_PRORITYMULRISK, aAttribDefs, 0);

		data.SetTaskPriority(ID_PARENT, 3);
		data.SetTaskPriority(ID_CHILD1, 5);
		data.SetTaskPriority(ID_CHILD2, 7);

		data.SetTaskRisk(ID_PARENT, 4);
		data.SetTaskRisk(ID_CHILD1, 6);
		data.SetTaskRisk(ID_CHILD2, 8);

		ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
		ExpectEQ(dValue, 12.0);

		ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
		ExpectEQ(dValue, 30.0);

		ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
		ExpectEQ(dValue, 56.0);
	}

	// Time Estimate - Double (1.75)
	{
		CTempCustAttribDef tempCalcDef(ID_TIMEESTMINUSDOUBLE, aAttribDefs, 0);

		data.SetTaskTimeEstimate(ID_PARENT, TDCTIMEPERIOD(8, TDCU_DAYS));
		data.SetTaskTimeEstimate(ID_CHILD1, TDCTIMEPERIOD(6, TDCU_DAYS));
		data.SetTaskTimeEstimate(ID_CHILD2, TDCTIMEPERIOD(4, TDCU_DAYS));

		ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue, TDCU_DAYS));
		ExpectEQ(dValue, 6.25);

		ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue, TDCU_DAYS));
		ExpectEQ(dValue, 4.25);

		ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue, TDCU_DAYS));
		ExpectEQ(dValue, 2.25);
	}

	// Custom date + Custom double
	{
		CTempCustAttribDef tempCalcDef(ID_CUSTDATEPLUSCUSTDOUBLE, aAttribDefs, 0);

		// Null Dates
		{
			data.SetTaskCustomAttributeData(ID_PARENT, ID_CUSTDATE, _T(""));
			data.SetTaskCustomAttributeData(ID_CHILD1, ID_CUSTDATE, _T(""));
			data.SetTaskCustomAttributeData(ID_CHILD2, ID_CUSTDATE, _T(""));

			data.SetTaskCustomAttributeData(ID_PARENT, ID_CUSTDOUBLE, _T("4.25"));
			data.SetTaskCustomAttributeData(ID_CHILD1, ID_CUSTDOUBLE, _T("8.50"));
			data.SetTaskCustomAttributeData(ID_CHILD2, ID_CUSTDOUBLE, _T("2.75"));

			ExpectFalse(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
			ExpectFalse(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
			ExpectFalse(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
		}

		// 'Zero Hour' - December 30, 1899, midnight
		{
			CTDCScopedSubTest subtest(*this, Misc::Format(_T("%s (%s)"), ID_CUSTDATEPLUSCUSTDOUBLE, _T("Zero Hour")));

			// ID_CUSTDATEPLUSCUSTDOUBLE.dwFeatures = 0 (no TDCCAF_SHOWTIMEOFDAY)
			{
				CTempCustAttribDef tempCalcDef(ID_CUSTDATEPLUSCUSTDOUBLE, aAttribDefs, 0);
				ExpectFalse(aAttribDefs.AttributeHasFeature(tempCalcDef, TDCCAF_SHOWTIMEOFDAY));

				data.SetTaskCustomAttributeData(ID_PARENT, ID_CUSTDATE, _T("0.0"));
				data.SetTaskCustomAttributeData(ID_CHILD1, ID_CUSTDATE, _T("0.0"));
				data.SetTaskCustomAttributeData(ID_CHILD2, ID_CUSTDATE, _T("0.0"));

				data.SetTaskCustomAttributeData(ID_PARENT, ID_CUSTDOUBLE, _T("4.25"));
				data.SetTaskCustomAttributeData(ID_CHILD1, ID_CUSTDOUBLE, _T("-8.50"));
				data.SetTaskCustomAttributeData(ID_CHILD2, ID_CUSTDOUBLE, _T("2.75"));

				ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
				ExpectEQ(dValue, 4.0);

				ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
				ExpectEQ(dValue, -8.0);

				ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
				ExpectEQ(dValue, 2.0);
			}

			// ID_CUSTDATEPLUSCUSTDOUBLE.dwFeatures = TDCCAF_SHOWTIMEOFDAY
			{
				CTempCustAttribDef tempCalcDef(ID_CUSTDATEPLUSCUSTDOUBLE, aAttribDefs, TDCCAF_SHOWTIMEOFDAY);
				ExpectTrue(aAttribDefs.AttributeHasFeature(tempCalcDef, TDCCAF_SHOWTIMEOFDAY));

				data.SetTaskCustomAttributeData(ID_PARENT, ID_CUSTDATE, _T("0.0"));
				data.SetTaskCustomAttributeData(ID_CHILD1, ID_CUSTDATE, _T("0.0"));
				data.SetTaskCustomAttributeData(ID_CHILD2, ID_CUSTDATE, _T("0.0"));

				data.SetTaskCustomAttributeData(ID_PARENT, ID_CUSTDOUBLE, _T("4.25"));
				data.SetTaskCustomAttributeData(ID_CHILD1, ID_CUSTDOUBLE, _T("-8.50"));
				data.SetTaskCustomAttributeData(ID_CHILD2, ID_CUSTDOUBLE, _T("2.75"));

				ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
				ExpectEQ(dValue, 4.25);

				ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
				ExpectEQ(dValue, -8.50);

				ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
				ExpectEQ(dValue, 2.75);
			}
		}

		// Positive dates
		{
			CTDCScopedSubTest subtest(*this, Misc::Format(_T("%s (%s)"), ID_CUSTDATEPLUSCUSTDOUBLE, _T("Positive")));

			// ID_CUSTDATEPLUSCUSTDOUBLE.dwFeatures = 0 (no TDCCAF_SHOWTIMEOFDAY)
			{
				CTempCustAttribDef tempCalcDef(ID_CUSTDATEPLUSCUSTDOUBLE, aAttribDefs, 0);
				ExpectFalse(aAttribDefs.AttributeHasFeature(tempCalcDef, TDCCAF_SHOWTIMEOFDAY));

				data.SetTaskCustomAttributeData(ID_PARENT, ID_CUSTDATE, _T("41254.25"));
				data.SetTaskCustomAttributeData(ID_CHILD1, ID_CUSTDATE, _T("41258.50"));
				data.SetTaskCustomAttributeData(ID_CHILD2, ID_CUSTDATE, _T("41262.75"));

				data.SetTaskCustomAttributeData(ID_PARENT, ID_CUSTDOUBLE, _T("4.8"));
				data.SetTaskCustomAttributeData(ID_CHILD1, ID_CUSTDOUBLE, _T("8.5"));
				data.SetTaskCustomAttributeData(ID_CHILD2, ID_CUSTDOUBLE, _T("2.3"));

				// ID_CUSTDATE.dwFeatures = 0 (no TDCCAF_SHOWTIMEOFDAY)
				{
					CTempCustAttribDef tempDateDef(ID_CUSTDATE, aAttribDefs, 0);
					ExpectFalse(aAttribDefs.AttributeHasFeature(tempDateDef, TDCCAF_SHOWTIMEOFDAY));

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
					ExpectEQ(dValue, 41258.0);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
					ExpectEQ(dValue, 41266.0);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
					ExpectEQ(dValue, 41264.0);
				}

				// ID_CUSTDATE.dwFeatures = TDCCAF_SHOWTIMEOFDAY
				{
					CTempCustAttribDef tempDateDef(ID_CUSTDATE, aAttribDefs, TDCCAF_SHOWTIMEOFDAY);
					ExpectTrue(aAttribDefs.AttributeHasFeature(tempDateDef, TDCCAF_SHOWTIMEOFDAY));

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
					ExpectEQ(dValue, 41259.0);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
					ExpectEQ(dValue, 41267.0);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
					ExpectEQ(dValue, 41265.0);
				}
			}

			// .dwFeatures = TDCCAF_SHOWTIMEOFDAY
			{
				CTempCustAttribDef tempCalcDef(ID_CUSTDATEPLUSCUSTDOUBLE, aAttribDefs, TDCCAF_SHOWTIMEOFDAY);
				ExpectTrue(aAttribDefs.AttributeHasFeature(tempCalcDef, TDCCAF_SHOWTIMEOFDAY));

				data.SetTaskCustomAttributeData(ID_PARENT, ID_CUSTDATE, _T("41254.25"));
				data.SetTaskCustomAttributeData(ID_CHILD1, ID_CUSTDATE, _T("41258.50"));
				data.SetTaskCustomAttributeData(ID_CHILD2, ID_CUSTDATE, _T("41262.75"));

				data.SetTaskCustomAttributeData(ID_PARENT, ID_CUSTDOUBLE, _T("4.1"));
				data.SetTaskCustomAttributeData(ID_CHILD1, ID_CUSTDOUBLE, _T("8.2"));
				data.SetTaskCustomAttributeData(ID_CHILD2, ID_CUSTDOUBLE, _T("2.3"));

				// ID_CUSTDATE.dwFeatures = 0 (no TDCCAF_SHOWTIMEOFDAY)
				{
					CTempCustAttribDef tempDateDef(ID_CUSTDATE, aAttribDefs, 0);
					ExpectFalse(aAttribDefs.AttributeHasFeature(tempDateDef, TDCCAF_SHOWTIMEOFDAY));

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
					ExpectEQ(dValue, 41258.1);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
					ExpectEQ(dValue, 41266.2);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
					ExpectEQ(dValue, 41264.3);
				}

				// ID_CUSTDATE.dwFeatures = TDCCAF_SHOWTIMEOFDAY
				{
					CTempCustAttribDef tempDateDef(ID_CUSTDATE, aAttribDefs, TDCCAF_SHOWTIMEOFDAY);
					ExpectTrue(aAttribDefs.AttributeHasFeature(tempDateDef, TDCCAF_SHOWTIMEOFDAY));

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
					ExpectEQ(dValue, 41258.35);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
					ExpectEQ(dValue, 41266.70);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
					ExpectEQ(dValue, 41265.05);
				}
			}
		}

		// Negative dates
		{
			CTDCScopedSubTest subtest(*this, Misc::Format(_T("%s (%s)"), ID_CUSTDATEPLUSCUSTDOUBLE, _T("Negative")));

			// ID_CUSTDATEPLUSCUSTDOUBLE.dwFeatures = 0 (no TDCCAF_SHOWTIMEOFDAY)
			{
				CTempCustAttribDef tempCalcDef(ID_CUSTDATEPLUSCUSTDOUBLE, aAttribDefs, 0);
				ExpectFalse(aAttribDefs.AttributeHasFeature(tempCalcDef, TDCCAF_SHOWTIMEOFDAY));

				data.SetTaskCustomAttributeData(ID_PARENT, ID_CUSTDATE, _T("-41254.25"));
				data.SetTaskCustomAttributeData(ID_CHILD1, ID_CUSTDATE, _T("-41258.50"));
				data.SetTaskCustomAttributeData(ID_CHILD2, ID_CUSTDATE, _T("-41262.75"));

				data.SetTaskCustomAttributeData(ID_PARENT, ID_CUSTDOUBLE, _T("4.4"));
				data.SetTaskCustomAttributeData(ID_CHILD1, ID_CUSTDOUBLE, _T("8.5"));
				data.SetTaskCustomAttributeData(ID_CHILD2, ID_CUSTDOUBLE, _T("2.6"));

				// ID_CUSTDATE.dwFeatures = 0 (no TDCCAF_SHOWTIMEOFDAY)
				{
					CTempCustAttribDef tempDateDef(ID_CUSTDATE, aAttribDefs, 0);
					ExpectFalse(aAttribDefs.AttributeHasFeature(tempDateDef, TDCCAF_SHOWTIMEOFDAY));

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
					ExpectEQ(dValue, -41249.0);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
					ExpectEQ(dValue, -41249.0);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
					ExpectEQ(dValue, -41259.0);
				}

				// ID_CUSTDATE.dwFeatures = TDCCAF_SHOWTIMEOFDAY
				{
					CTempCustAttribDef tempDateDef(ID_CUSTDATE, aAttribDefs, TDCCAF_SHOWTIMEOFDAY);
					ExpectTrue(aAttribDefs.AttributeHasFeature(tempDateDef, TDCCAF_SHOWTIMEOFDAY));

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
					ExpectEQ(dValue, -41249.0);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
					ExpectEQ(dValue, -41250.0);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
					ExpectEQ(dValue, -41260.0);
				}
			}

			// ID_CUSTDATEPLUSCUSTDOUBLE.dwFeatures = TDCCAF_SHOWTIMEOFDAY
			{
				CTempCustAttribDef tempCalcDef(ID_CUSTDATEPLUSCUSTDOUBLE, aAttribDefs, TDCCAF_SHOWTIMEOFDAY);
				ExpectTrue(aAttribDefs.AttributeHasFeature(tempCalcDef, TDCCAF_SHOWTIMEOFDAY));

				data.SetTaskCustomAttributeData(ID_PARENT, ID_CUSTDATE, _T("-41254.25"));
				data.SetTaskCustomAttributeData(ID_CHILD1, ID_CUSTDATE, _T("-41258.50"));
				data.SetTaskCustomAttributeData(ID_CHILD2, ID_CUSTDATE, _T("-41262.75"));

				data.SetTaskCustomAttributeData(ID_PARENT, ID_CUSTDOUBLE, _T("4.4"));
				data.SetTaskCustomAttributeData(ID_CHILD1, ID_CUSTDOUBLE, _T("8.5"));
				data.SetTaskCustomAttributeData(ID_CHILD2, ID_CUSTDOUBLE, _T("2.6"));

				// ID_CUSTDATE.dwFeatures = 0 (no TDCCAF_SHOWTIMEOFDAY)
				{
					CTempCustAttribDef tempDateDef(ID_CUSTDATE, aAttribDefs, 0);
					ExpectFalse(aAttribDefs.AttributeHasFeature(tempDateDef, TDCCAF_SHOWTIMEOFDAY));

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
					ExpectEQ(dValue, -41249.6);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
					ExpectEQ(dValue, -41249.5);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
					ExpectEQ(dValue, -41259.4);
				}

				// ID_CUSTDATE.dwFeatures = TDCCAF_SHOWTIMEOFDAY
				{
					CTempCustAttribDef tempDateDef(ID_CUSTDATE, aAttribDefs, TDCCAF_SHOWTIMEOFDAY);
					ExpectTrue(aAttribDefs.AttributeHasFeature(tempDateDef, TDCCAF_SHOWTIMEOFDAY));

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
					ExpectEQ(dValue, -41249.85);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
					ExpectEQ(dValue, -41250.0);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
					ExpectEQ(dValue, -41260.15);
				}
			}
		}
	}

	// Subtracting dates from dates
	{
		// Due date - Start date
		{
			CTDCScopedSubTest subtest(*this, ID_DUEDATEMINUSSTARTDATE);

			// dwFeatures = 0 (no TDCCAF_IGNORETIMEOFYDAY)
			{
				CTempCustAttribDef tempCalcDef(ID_DUEDATEMINUSSTARTDATE, aAttribDefs, 0);
				ExpectFalse(aAttribDefs.AttributeHasFeature(tempCalcDef, TDCCAF_IGNORETIMEOFDAY));

				data.SetTaskDate(ID_PARENT, TDCD_DUE, COleDateTime(41254.0));	// end of day
				data.SetTaskDate(ID_CHILD1, TDCD_DUE, COleDateTime(-41258.0));	// end of day
				data.SetTaskDate(ID_CHILD2, TDCD_DUE, COleDateTime(0.0));		// end of day

				data.SetTaskDate(ID_PARENT, TDCD_START, COleDateTime(41249.50));
				data.SetTaskDate(ID_CHILD1, TDCD_START, COleDateTime(-41247.75));
				data.SetTaskDate(ID_CHILD2, TDCD_START, COleDateTime(41273.25));

				ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
				ExpectEQ(dValue, 5.50);

				ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
				ExpectEQ(dValue, -9.25);

				ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
				ExpectEQ(dValue, -41272.25);
			}

			// dwFeatures = TDCCAF_IGNORETIMEOFDAY
			{
				CTempCustAttribDef tempCalcDef(ID_DUEDATEMINUSSTARTDATE, aAttribDefs, TDCCAF_IGNORETIMEOFDAY);
				ExpectTrue(aAttribDefs.AttributeHasFeature(tempCalcDef, TDCCAF_IGNORETIMEOFDAY));

				data.SetTaskDate(ID_PARENT, TDCD_DUE, COleDateTime(41254.0));	// start of day
				data.SetTaskDate(ID_CHILD1, TDCD_DUE, COleDateTime(-41258.0));	// start of day
				data.SetTaskDate(ID_CHILD2, TDCD_DUE, COleDateTime(0.0));		// start of day

				data.SetTaskDate(ID_PARENT, TDCD_START, COleDateTime(41249.50));
				data.SetTaskDate(ID_CHILD1, TDCD_START, COleDateTime(-41247.75));
				data.SetTaskDate(ID_CHILD2, TDCD_START, COleDateTime(41273.25));

				ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
				ExpectEQ(dValue, 5.0);

				ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
				ExpectEQ(dValue, -11.0);

				ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
				ExpectEQ(dValue, -41273.0);
			}
		}

		// Custom date - Start date
		{
			CTDCScopedSubTest subtest(*this, ID_CUSTDATEMINUSSTARTDATE);

			// ID_CUSTDATEMINUSSTARTDATE.dwFeatures = 0 (no TDCCAF_IGNORETIMEOFYDAY)
			{
				CTempCustAttribDef tempCalcDef(ID_CUSTDATEMINUSSTARTDATE, aAttribDefs, 0);
				ExpectFalse(aAttribDefs.AttributeHasFeature(tempCalcDef, TDCCAF_IGNORETIMEOFDAY));

				data.SetTaskCustomAttributeData(ID_PARENT, ID_CUSTDATE, _T("41254.25"));
				data.SetTaskCustomAttributeData(ID_CHILD1, ID_CUSTDATE, _T("41258.50"));
				data.SetTaskCustomAttributeData(ID_CHILD2, ID_CUSTDATE, _T("41262.75"));

				data.SetTaskDate(ID_PARENT, TDCD_START, COleDateTime(41249.50));
				data.SetTaskDate(ID_CHILD1, TDCD_START, COleDateTime(41247.75));
				data.SetTaskDate(ID_CHILD2, TDCD_START, COleDateTime(41273.25));

				// ID_CUSTDATE.dwFeatures = 0 (no TDCCAF_SHOWTIMEOFDAY)
				{
					CTempCustAttribDef tempDateDef(ID_CUSTDATE, aAttribDefs, 0);
					ExpectFalse(aAttribDefs.AttributeHasFeature(tempDateDef, TDCCAF_SHOWTIMEOFDAY));

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
					ExpectEQ(dValue, 4.50);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
					ExpectEQ(dValue, 10.25);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
					ExpectEQ(dValue, -11.25);
				}

				// ID_CUSTDATE.dwFeatures = TDCCAF_SHOWTIMEOFDAY
				{
					CTempCustAttribDef tempDateDef(ID_CUSTDATE, aAttribDefs, TDCCAF_SHOWTIMEOFDAY);
					ExpectTrue(aAttribDefs.AttributeHasFeature(tempDateDef, TDCCAF_SHOWTIMEOFDAY));

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
					ExpectEQ(dValue, 4.75);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
					ExpectEQ(dValue, 10.75);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
					ExpectEQ(dValue, -10.50);
				}
			}

			// ID_CUSTDATEMINUSSTARTDATE.dwFeatures = TDCCAF_IGNORETIMEOFDAY
			{
				CTempCustAttribDef tempCalcDef(ID_CUSTDATEMINUSSTARTDATE, aAttribDefs, TDCCAF_IGNORETIMEOFDAY);
				ExpectTrue(aAttribDefs.AttributeHasFeature(tempCalcDef, TDCCAF_IGNORETIMEOFDAY));

				data.SetTaskCustomAttributeData(ID_PARENT, ID_CUSTDATE, _T("41254.25"));
				data.SetTaskCustomAttributeData(ID_CHILD1, ID_CUSTDATE, _T("41258.50"));
				data.SetTaskCustomAttributeData(ID_CHILD2, ID_CUSTDATE, _T("41262.75"));

				data.SetTaskDate(ID_PARENT, TDCD_START, COleDateTime(41249.50));
				data.SetTaskDate(ID_CHILD1, TDCD_START, COleDateTime(41247.75));
				data.SetTaskDate(ID_CHILD2, TDCD_START, COleDateTime(41273.25));

				// ID_CUSTDATE.dwFeatures = 0 (no TDCCAF_SHOWTIMEOFDAY)
				{
					CTempCustAttribDef tempDateDef(ID_CUSTDATE, aAttribDefs, 0);
					ExpectFalse(aAttribDefs.AttributeHasFeature(tempDateDef, TDCCAF_SHOWTIMEOFDAY));

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
					ExpectEQ(dValue, 5.0);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
					ExpectEQ(dValue, 11.0);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
					ExpectEQ(dValue, -11.0);
				}

				// ID_CUSTDATE.dwFeatures = TDCCAF_SHOWTIMEOFDAY
				{
					CTempCustAttribDef tempDateDef(ID_CUSTDATE, aAttribDefs, TDCCAF_SHOWTIMEOFDAY);
					ExpectTrue(aAttribDefs.AttributeHasFeature(tempDateDef, TDCCAF_SHOWTIMEOFDAY));

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
					ExpectEQ(dValue, 5.0);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
					ExpectEQ(dValue, 11.0);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
					ExpectEQ(dValue, -11.0);
				}
			}
		}

		// Custom date - Due date
		{
			CTDCScopedSubTest subtest(*this, ID_CUSTDATEMINUSDUEDATE);

			// ID_CUSTDATEMINUSDUEDATE.dwFeatures = 0 (no TDCCAF_IGNORETIMEOFYDAY)
			{
				CTempCustAttribDef tempCalcDef(ID_CUSTDATEMINUSDUEDATE, aAttribDefs, 0);
				ExpectFalse(aAttribDefs.AttributeHasFeature(tempCalcDef, TDCCAF_IGNORETIMEOFDAY));

				data.SetTaskCustomAttributeData(ID_PARENT, ID_CUSTDATE, _T("41254.25"));
				data.SetTaskCustomAttributeData(ID_CHILD1, ID_CUSTDATE, _T("41258.50"));
				data.SetTaskCustomAttributeData(ID_CHILD2, ID_CUSTDATE, _T("41262.75"));

				data.SetTaskDate(ID_PARENT, TDCD_DUE, COleDateTime(41254.0));	// end of day
				data.SetTaskDate(ID_CHILD1, TDCD_DUE, COleDateTime(-41258.0));	// end of day
				data.SetTaskDate(ID_CHILD2, TDCD_DUE, COleDateTime(0.0));		// end of day

				// ID_CUSTDATE.dwFeatures = 0 (no TDCCAF_SHOWTIMEOFDAY)
				{
					CTempCustAttribDef tempDateDef(ID_CUSTDATE, aAttribDefs, 0);
					ExpectFalse(aAttribDefs.AttributeHasFeature(tempDateDef, TDCCAF_SHOWTIMEOFDAY));

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
					ExpectEQ(dValue, -1.0);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
					ExpectEQ(dValue, 82515.0);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
					ExpectEQ(dValue, 41261.0);
				}

				{
					CTempCustAttribDef tempDateDef(ID_CUSTDATE, aAttribDefs, TDCCAF_SHOWTIMEOFDAY);
					ExpectTrue(aAttribDefs.AttributeHasFeature(tempDateDef, TDCCAF_SHOWTIMEOFDAY));

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
					ExpectEQ(dValue, -0.75);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
					ExpectEQ(dValue, 82515.50);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
					ExpectEQ(dValue, 41261.75);
				}
			}

			// ID_CUSTDATEMINUSDUEDATE.dwFeatures = TDCCAF_IGNORETIMEOFDAY
			{
				CTempCustAttribDef tempCalcDef(ID_CUSTDATEMINUSDUEDATE, aAttribDefs, TDCCAF_IGNORETIMEOFDAY);
				ExpectTrue(aAttribDefs.AttributeHasFeature(tempCalcDef, TDCCAF_IGNORETIMEOFDAY));

				data.SetTaskCustomAttributeData(ID_PARENT, ID_CUSTDATE, _T("41254.25"));
				data.SetTaskCustomAttributeData(ID_CHILD1, ID_CUSTDATE, _T("41258.50"));
				data.SetTaskCustomAttributeData(ID_CHILD2, ID_CUSTDATE, _T("41262.75"));

				data.SetTaskDate(ID_PARENT, TDCD_DUE, COleDateTime(41254.0));	// start of day
				data.SetTaskDate(ID_CHILD1, TDCD_DUE, COleDateTime(-41258.0));	// start of day
				data.SetTaskDate(ID_CHILD2, TDCD_DUE, COleDateTime(0.0));		// start of day

				// ID_CUSTDATE.dwFeatures = 0 (no TDCCAF_SHOWTIMEOFDAY)
				{
					CTempCustAttribDef tempDateDef(ID_CUSTDATE, aAttribDefs, 0);
					ExpectFalse(aAttribDefs.AttributeHasFeature(tempDateDef, TDCCAF_SHOWTIMEOFDAY));

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
					ExpectEQ(dValue, 0.0);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
					ExpectEQ(dValue, 82516.0);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
					ExpectEQ(dValue, 41262.0);
				}

				// ID_CUSTDATE.dwFeatures = TDCCAF_SHOWTIMEOFDAY
				{
					CTempCustAttribDef tempDateDef(ID_CUSTDATE, aAttribDefs, TDCCAF_SHOWTIMEOFDAY);
					ExpectTrue(aAttribDefs.AttributeHasFeature(tempDateDef, TDCCAF_SHOWTIMEOFDAY));

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_PARENT, tempCalcDef, dValue));
					ExpectEQ(dValue, 0.0);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD1, tempCalcDef, dValue));
					ExpectEQ(dValue, 82516.0);

					ExpectTrue(calc.GetTaskCustomAttributeData(ID_CHILD2, tempCalcDef, dValue));
					ExpectEQ(dValue, 41262.0);
				}
			}
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