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

TESTRESULT CDateHelperTest::Run()
{
	ClearTotals();

	TestRelativeDateDecoding();

	return GetTotals();
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

	CTwentyFourSevenWeek week;
	CDateHelper dh(week);

	COleDateTime date;
	
	// -----------------------------------------------------------------------

	ExpectFalse(dh.DecodeRelativeDate(_T("0"), date, MUSTHAVESIGN));
	ExpectTrue(dh.DecodeRelativeDate(_T("+0"), date, MUSTHAVESIGN));
	ExpectTrue(dh.DecodeRelativeDate(_T("0"), date, DONTNEEDSIGN));
	ExpectTrue(dh.DecodeRelativeDate(_T("+0"), date, DONTNEEDSIGN));
	ExpectEQ(date, TODAY);
	
	ExpectFalse(dh.DecodeRelativeDate(_T("4"), date, MUSTHAVESIGN));
	ExpectTrue(dh.DecodeRelativeDate(_T("+4"), date, MUSTHAVESIGN));
	ExpectTrue(dh.DecodeRelativeDate(_T("4"), date, DONTNEEDSIGN));
	ExpectEQ(date, (TODAY + 4));
	
	// -----------------------------------------------------------------------

	ExpectTrue(dh.DecodeRelativeDate(_T("t"), date));
	ExpectEQ(date, TODAY);
	
	ExpectTrue(dh.DecodeRelativeDate(_T("T"), date));
	ExpectEQ(date, TODAY);
	
	// -----------------------------------------------------------------------

	ExpectFalse(dh.DecodeRelativeDate(_T("w"), date));
	ExpectTrue(dh.DecodeRelativeDate(_T("W"), date));
	ExpectEQ(date, ENDTHISWEEK);
	
	ExpectFalse(dh.DecodeRelativeDate(_T("w"), date));
	ExpectTrue(dh.DecodeRelativeDate(_T("M"), date));
	ExpectEQ(date, ENDTHISMONTH);
	
	ExpectFalse(dh.DecodeRelativeDate(_T("y"), date));
	ExpectTrue(dh.DecodeRelativeDate(_T("Y"), date));
	ExpectEQ(date, ENDTHISYEAR);
	
	// -----------------------------------------------------------------------

	ExpectTrue(dh.DecodeRelativeDate(_T("t+3"), date));
	ExpectEQ(date, (TODAY + 3));
	
	ExpectTrue(dh.DecodeRelativeDate(_T("T-21"), date));
	ExpectEQ(date, (TODAY - 21));
	
	// -----------------------------------------------------------------------

	ExpectTrue(dh.DecodeRelativeDate(_T("W+11"), date));
	ExpectEQ(date, (ENDTHISWEEK + (11 * 7)));

	ExpectTrue(dh.DecodeRelativeDate(_T("M+7"), date));
	{
		COleDateTime dtExpect(ENDTHISMONTH);
		ExpectTrue(dh.OffsetDate(dtExpect, 7, DHU_MONTHS));
		ExpectEQ(date, dtExpect);
	}

	ExpectTrue(dh.DecodeRelativeDate(_T("Y-4"), date));
	{
		COleDateTime dtExpect(ENDTHISYEAR);
		ExpectTrue(dh.OffsetDate(dtExpect, -4, DHU_YEARS));
		ExpectEQ(date, dtExpect);
	}
	
	// -----------------------------------------------------------------------
	
	EndTest();
}
