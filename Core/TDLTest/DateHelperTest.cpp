// DateHelperTest.cpp: implementation of the CDateHelperTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "DateHelperTest.h"

#include "..\shared\Datehelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDateHelperTest::CDateHelperTest(const CTestUtils& utils) : CTDLTestBase(utils)
{

}

CDateHelperTest::~CDateHelperTest()
{

}

void CDateHelperTest::Run()
{
	TestRelativeDateDecoding();
}

void CDateHelperTest::TestRelativeDateDecoding()
{
	BeginTest(_T("CDateHelperTest::DecodeRelativeDate"));
	
	const double TODAY(CDateHelper::GetDate(DHD_TODAY));
	const double ENDTHISWEEK(CDateHelper::GetDate(DHD_ENDTHISWEEK));
	const double ENDTHISMONTH(CDateHelper::GetDate(DHD_ENDTHISMONTH));
	const double ENDTHISYEAR(CDateHelper::GetDate(DHD_ENDTHISYEAR));

	const BOOL MUSTHAVESIGN = TRUE;
	const BOOL DONTNEEDSIGN = FALSE;

	COleDateTime date;
	
	// -----------------------------------------------------------------------

	ExpectFalse(CDateHelper::DecodeRelativeDate(_T("0"), date, FALSE, MUSTHAVESIGN));
	ExpectTrue(CDateHelper::DecodeRelativeDate(_T("+0"), date, FALSE, MUSTHAVESIGN));
	ExpectTrue(CDateHelper::DecodeRelativeDate(_T("0"), date, FALSE, DONTNEEDSIGN));
	ExpectTrue(CDateHelper::DecodeRelativeDate(_T("+0"), date, FALSE, DONTNEEDSIGN));
	ExpectEQ(date, TODAY);
	
	ExpectFalse(CDateHelper::DecodeRelativeDate(_T("4"), date, FALSE, MUSTHAVESIGN));
	ExpectTrue(CDateHelper::DecodeRelativeDate(_T("+4"), date, FALSE, MUSTHAVESIGN));
	ExpectTrue(CDateHelper::DecodeRelativeDate(_T("4"), date, FALSE, DONTNEEDSIGN));
	ExpectEQ(date, (TODAY + 4));
	
	// -----------------------------------------------------------------------

	ExpectTrue(CDateHelper::DecodeRelativeDate(_T("t"), date, FALSE, TRUE));
	ExpectEQ(date, TODAY);
	
	ExpectTrue(CDateHelper::DecodeRelativeDate(_T("T"), date, FALSE, TRUE));
	ExpectEQ(date, TODAY);
	
	// -----------------------------------------------------------------------

	ExpectFalse(CDateHelper::DecodeRelativeDate(_T("w"), date, FALSE, TRUE));
	ExpectTrue(CDateHelper::DecodeRelativeDate(_T("W"), date, FALSE, TRUE));
	ExpectEQ(date, ENDTHISWEEK);
	
	ExpectFalse(CDateHelper::DecodeRelativeDate(_T("w"), date, FALSE, TRUE));
	ExpectTrue(CDateHelper::DecodeRelativeDate(_T("M"), date, FALSE, TRUE));
	ExpectEQ(date, ENDTHISMONTH);
	
	ExpectFalse(CDateHelper::DecodeRelativeDate(_T("y"), date, FALSE, TRUE));
	ExpectTrue(CDateHelper::DecodeRelativeDate(_T("Y"), date, FALSE, TRUE));
	ExpectEQ(date, ENDTHISYEAR);
	
	// -----------------------------------------------------------------------

	ExpectTrue(CDateHelper::DecodeRelativeDate(_T("t+3"), date, FALSE, TRUE));
	ExpectEQ(date, (TODAY + 3));
	
	ExpectTrue(CDateHelper::DecodeRelativeDate(_T("T-21"), date, FALSE, TRUE));
	ExpectEQ(date, (TODAY - 21));
	
	// -----------------------------------------------------------------------

	ExpectTrue(CDateHelper::DecodeRelativeDate(_T("W+11"), date, FALSE, TRUE));
	ExpectEQ(date, (ENDTHISWEEK + (11 * 7)));

	ExpectTrue(CDateHelper::DecodeRelativeDate(_T("M+7"), date, FALSE, TRUE));
	{
		COleDateTime dtExpect(ENDTHISMONTH);
		ExpectTrue(CDateHelper::OffsetDate(dtExpect, 7, DHU_MONTHS));
		ExpectEQ(date, dtExpect);
	}

	ExpectTrue(CDateHelper::DecodeRelativeDate(_T("Y-4"), date, FALSE, TRUE));
	{
		COleDateTime dtExpect(ENDTHISYEAR);
		ExpectTrue(CDateHelper::OffsetDate(dtExpect, -4, DHU_YEARS));
		ExpectEQ(date, dtExpect);
	}
	
	// -----------------------------------------------------------------------
	
	EndTest();
}
