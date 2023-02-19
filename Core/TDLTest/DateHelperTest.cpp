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

	Test64BitDates();
	TestDecodeRelativeDate();
	TestTruncateSeconds();
	TestGetDateOnly();
	TestGetTimeOnly();
	TestMakeDate();
	TestCompare();
	
	return GetTotals();
}

void CDateHelperTest::TestDecodeRelativeDate()
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

void CDateHelperTest::TestTruncateSeconds()
{
	BeginTest(_T("CDateHelperTest::TruncateSeconds"));
	
	const COleDateTime dtPositive(45678.123456);
	const COleDateTime dtNegative(-45678.123456);

	const COleDateTime dtPositiveNoSeconds = CDateHelper::TruncateSeconds(dtPositive);
	const COleDateTime dtNegativeNoSeconds = CDateHelper::TruncateSeconds(dtNegative);

	ExpectEQ(dtPositive.GetYear(), dtPositiveNoSeconds.GetYear());
	ExpectEQ(dtPositive.GetMonth(), dtPositiveNoSeconds.GetMonth());
	ExpectEQ(dtPositive.GetDay(), dtPositiveNoSeconds.GetDay());
	ExpectEQ(dtPositive.GetHour(), dtPositiveNoSeconds.GetHour());
	ExpectEQ(dtPositive.GetMinute(), dtPositiveNoSeconds.GetMinute());
	ExpectNE(dtPositive.GetSecond(), dtPositiveNoSeconds.GetSecond());

	ExpectTrue(dtPositive.GetSecond() > 0);
	ExpectTrue(dtPositiveNoSeconds.GetSecond() == 0);
	
	// -----------------------------------------------------------------------
	
	EndTest();
}

void CDateHelperTest::TestGetDateOnly()
{
	BeginTest(_T("CDateHelperTest::GetDateOnly"));

	ExpectEQ(CDateHelper::GetDateOnly(44000.125).m_dt, 44000.0);
	ExpectEQ(CDateHelper::GetDateOnly(44000.0).m_dt, 44000.0);
	ExpectEQ(CDateHelper::GetDateOnly(-44000.125).m_dt, -44000.0);
	ExpectEQ(CDateHelper::GetDateOnly(-44000.0).m_dt, -44000.0);

	// -----------------------------------------------------------------------
	
	EndTest();
}

void CDateHelperTest::TestGetTimeOnly()
{
	BeginTest(_T("CDateHelperTest::GetTimeOnly"));

	// Note: time component is always positive
	ExpectEQ(CDateHelper::GetTimeOnly(44000.125).m_dt, 0.125);
	ExpectEQ(CDateHelper::GetTimeOnly(44000.0).m_dt, 0.0);
	ExpectEQ(CDateHelper::GetTimeOnly(-44000.125).m_dt, 0.125);
	ExpectEQ(CDateHelper::GetTimeOnly(-44000.0).m_dt, 0.0);

	// -----------------------------------------------------------------------
	
	EndTest();
}

void CDateHelperTest::TestMakeDate()
{
	BeginTest(_T("CDateHelperTest::MakeDate"));

	{
		COleDateTime dt1(44000.125), dt2(34000.375);
		
		ExpectEQ(CDateHelper::MakeDate(dt1, dt2).m_dt, 44000.375);
		ExpectEQ(CDateHelper::MakeDate(dt2, dt1).m_dt, 34000.125);
	}

	{
		COleDateTime dt1(44000.125), dt2(-34000.375);

		ExpectEQ(CDateHelper::MakeDate(dt1, dt2).m_dt, 44000.375);
		ExpectEQ(CDateHelper::MakeDate(dt2, dt1).m_dt, -34000.125);
	}

	{
		COleDateTime dt1(-44000.125), dt2(-34000.375);

		ExpectEQ(CDateHelper::MakeDate(dt1, dt2).m_dt, -44000.375);
		ExpectEQ(CDateHelper::MakeDate(dt2, dt1).m_dt, -34000.125);
	}

	// -----------------------------------------------------------------------
	
	EndTest();
}

void CDateHelperTest::TestCompare()
{
	BeginTest(_T("CDateHelperTest::Compare"));

	{
		COleDateTime dt1(44000.125), dt2(44000.25);
		
		ExpectTrue(CDateHelper::Compare(dt1, dt2, 0) == 0);
		ExpectTrue(CDateHelper::Compare(dt1, dt2, DHC_COMPARETIME) < 0);
	}

	{
		COleDateTime dt1(44000.125), dt2(44000.0);
		
		ExpectTrue(CDateHelper::Compare(dt1, dt2, 0) == 0);
		ExpectTrue(CDateHelper::Compare(dt1, dt2, DHC_COMPARETIME) > 0);
		ExpectTrue(CDateHelper::Compare(dt1, dt2, DHC_COMPARETIME | DHC_NOTIMEISENDOFDAY) < 0);
		ExpectTrue(CDateHelper::Compare(dt1, dt2, DHC_NOTIMEISENDOFDAY) == 0);
	}

	{
		// Note: time is treated positive
		COleDateTime dt1(-44000.125), dt2(-44000.25);
		
		ExpectTrue(CDateHelper::Compare(dt1, dt2, 0) == 0);
		ExpectTrue(CDateHelper::Compare(dt1, dt2, DHC_COMPARETIME) < 0);
	}

	{
		COleDateTime dt1(-44000.125), dt2(-44000.0);

		ExpectTrue(CDateHelper::Compare(dt1, dt2, 0) == 0);
		ExpectTrue(CDateHelper::Compare(dt1, dt2, DHC_COMPARETIME) > 0);
		ExpectTrue(CDateHelper::Compare(dt1, dt2, DHC_COMPARETIME | DHC_NOTIMEISENDOFDAY) < 0);
		ExpectTrue(CDateHelper::Compare(dt1, dt2, DHC_NOTIMEISENDOFDAY) == 0);
	}

	// -----------------------------------------------------------------------
	
	EndTest();
}

void CDateHelperTest::Test64BitDates()
{
	BeginTest(_T("CDateHelperTest::64BitDates"));

	COleDateTime dtNow = COleDateTime::GetCurrentTime();

	time64_t tNow;
	ExpectTrue(CDateHelper::GetTimeT64(dtNow, tNow));

	COleDateTime dtNowCheck = CDateHelper::GetDate(tNow);
	ExpectEQ(dtNow, dtNowCheck);

	EndTest();
}
