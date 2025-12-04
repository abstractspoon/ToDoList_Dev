// DateHelperTest.cpp: implementation of the CDateHelperTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "DateHelperTest.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDateHelperTest::CDateHelperTest(const CTestUtils& utils) 
	: 
	CTDLTestBase(_T("CDateHelperTest"), utils)
{

}

CDateHelperTest::~CDateHelperTest()
{

}

TESTRESULT CDateHelperTest::Run()
{
	ClearTotals();

	TestDecodeDate();
	TestDecodeRelativeDate();
	TestTruncateSeconds();
	TestGetDateOnly();
	TestGetTimeOnly();
	TestMakeDate();
	TestCompare();
	Test64BitDates();
	TestOffsetDate();

	return GetTotals();
}

void CDateHelperTest::TestDecodeDate()
{
	CTDCScopedTest test(*this, _T("CDateHelper::DecodeDate"));

	const double DATE_TOL = COleDateTimeSpan(0, 0, 0, 1).m_span;
	const COleDateTime dtNow = COleDateTime::GetCurrentTime();

	// empty string -> Null date
	{
		COleDateTime dtNull;
		ExpectFalse(CDateHelper::DecodeDate(_T(""), dtNull, TRUE));
		ExpectTrue(dtNull == CDateHelper::NullDate());
	}

	// CDateHelper::FormatDate(DHFD_TIME)
	{
		CString sNow = CDateHelper::FormatDate(dtNow, DHFD_TIME);

		COleDateTime dtCheck;
		ExpectTrue(CDateHelper::DecodeDate(sNow, dtCheck, TRUE));
		ExpectEQ(dtCheck, dtNow, DATE_TOL);

		ExpectTrue(CDateHelper::DecodeDate(sNow, dtCheck, FALSE));
		ExpectEQ(dtCheck.m_dt, (double)(int)dtNow.m_dt);
	}

	// CDateHelper::FormatDate() - no time
	{
		CString sNow = CDateHelper::FormatDate(dtNow);

		COleDateTime dtCheck;
		ExpectTrue(CDateHelper::DecodeDate(sNow, dtCheck, TRUE));
		ExpectEQ(dtCheck.m_dt, (double)(int)dtNow.m_dt);

		ExpectTrue(CDateHelper::DecodeDate(sNow, dtCheck, FALSE));
		ExpectEQ(dtCheck.m_dt, (double)(int)dtNow.m_dt);
	}

	// CDateHelper::FormatDate(DHFD_ISO | DHFD_TIME)
	{
		CString sNow = CDateHelper::FormatDate(dtNow, DHFD_ISO | DHFD_TIME);

		COleDateTime dtCheck;
		ExpectTrue(CDateHelper::DecodeDate(sNow, dtCheck, TRUE));
		ExpectEQ(dtCheck, dtNow, DATE_TOL);

		ExpectTrue(CDateHelper::DecodeDate(sNow, dtCheck, FALSE));
		ExpectEQ(dtCheck.m_dt, (double)(int)dtNow.m_dt);
	}

	// CDateHelper::FormatDate(DHFD_ISO) - no time
	{
		CString sNow = CDateHelper::FormatDate(dtNow, DHFD_ISO);

		COleDateTime dtCheck;
		ExpectTrue(CDateHelper::DecodeDate(sNow, dtCheck, TRUE));
		ExpectEQ(dtCheck.m_dt, (double)(int)dtNow.m_dt);

		ExpectTrue(CDateHelper::DecodeDate(sNow, dtCheck, FALSE));
		ExpectEQ(dtCheck.m_dt, (double)(int)dtNow.m_dt);
	}
}

void CDateHelperTest::TestDecodeRelativeDate()
{
	CTDCScopedTest test(*this, _T("CDateHelper::DecodeRelativeDate"));
	
	const COleDateTime TODAY(CDateHelper::GetDate(DHD_TODAY));
	const COleDateTime ENDTHISWEEK(CDateHelper::GetDate(DHD_ENDTHISWEEK));
	const COleDateTime ENDTHISMONTH(CDateHelper::GetDate(DHD_ENDTHISMONTH));
	const COleDateTime ENDTHISYEAR(CDateHelper::GetDate(DHD_ENDTHISYEAR));

	const BOOL MUSTHAVESIGN = TRUE;
	const BOOL DONTNEEDSIGN = FALSE;

	CTwentyFourSevenWeek week;
	CDateHelper dh(week);

	COleDateTime date;

	// empty string -> Null date
	{
		ExpectFalse(dh.DecodeRelativeDate(_T(""), date, MUSTHAVESIGN));
		ExpectTrue(date == CDateHelper::NullDate());

		ExpectFalse(dh.DecodeRelativeDate(_T(""), date, DONTNEEDSIGN));
		ExpectTrue(date == CDateHelper::NullDate());
	}
	
	// Numeric
	{
		ExpectFalse(dh.DecodeRelativeDate(_T("0"), date, MUSTHAVESIGN));

		ExpectTrue(dh.DecodeRelativeDate(_T("+0"), date, MUSTHAVESIGN));
		ExpectEQ(date, TODAY);

		ExpectTrue(dh.DecodeRelativeDate(_T("0"), date, DONTNEEDSIGN));
		ExpectEQ(date, TODAY);

		ExpectTrue(dh.DecodeRelativeDate(_T("+0"), date, DONTNEEDSIGN));
		ExpectEQ(date, TODAY);

		ExpectFalse(dh.DecodeRelativeDate(_T("4"), date, MUSTHAVESIGN));

		ExpectTrue(dh.DecodeRelativeDate(_T("+4"), date, MUSTHAVESIGN));
		ExpectEQ(date, COleDateTime(TODAY.m_dt + 4));

		ExpectTrue(dh.DecodeRelativeDate(_T("-4"), date, DONTNEEDSIGN));
		ExpectEQ(date, COleDateTime(TODAY.m_dt - 4));
	}
	
	// Today
	{
		ExpectTrue(dh.DecodeRelativeDate(_T("t"), date));
		ExpectEQ(date, TODAY);

		ExpectTrue(dh.DecodeRelativeDate(_T("T"), date));
		ExpectEQ(date, TODAY);
	}
	
	// End of this Week/Month/Year
	{
		ExpectFalse(dh.DecodeRelativeDate(_T("w"), date));
		ExpectTrue(dh.DecodeRelativeDate(_T("W"), date));
		ExpectEQ(date, ENDTHISWEEK);

		ExpectFalse(dh.DecodeRelativeDate(_T("w"), date));
		ExpectTrue(dh.DecodeRelativeDate(_T("M"), date));
		ExpectEQ(date, ENDTHISMONTH);

		ExpectFalse(dh.DecodeRelativeDate(_T("y"), date));
		ExpectTrue(dh.DecodeRelativeDate(_T("Y"), date));
		ExpectEQ(date, ENDTHISYEAR);
	}
	
	// Today + offset
	{
		ExpectTrue(dh.DecodeRelativeDate(_T("t+3"), date));
		ExpectEQ(date, COleDateTime(TODAY.m_dt + 3));

		ExpectTrue(dh.DecodeRelativeDate(_T("T-21"), date));
		ExpectEQ(date, COleDateTime(TODAY.m_dt - 21));
	}
	
	// End of this Week/Month/Year + offset
	{
		ExpectTrue(dh.DecodeRelativeDate(_T("W+11"), date));
		ExpectEQ(date, COleDateTime(ENDTHISWEEK.m_dt + (11 * 7)));

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
	}
}

void CDateHelperTest::TestTruncateSeconds()
{
	CTDCScopedTest test(*this, _T("CDateHelper::TruncateSeconds"));
	
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
}

void CDateHelperTest::TestGetDateOnly()
{
	CTDCScopedTest test(*this, _T("CDateHelper::GetDateOnly"));

	// Null dates
	{
#ifndef _DEBUG // because CDateHelper asserts on Null dates

		ExpectEQ(CDateHelper::GetDateOnly(CDateHelper::NullDate()), CDateHelper::NullDate());

#endif
	}

	// 'Zero Hour' - December 30, 1899
	{
		ExpectEQ(CDateHelper::GetDateOnly(0.125).m_dt, 0.0);
		ExpectEQ(CDateHelper::GetDateOnly(-0.125).m_dt, 0.0);

		ExpectEQ(CDateHelper::GetDateOnly(0.125).m_dt, 0.0);
		ExpectEQ(CDateHelper::GetDateOnly(-0.125).m_dt, 0.0);
	}

	// Positive/Negative dates
	{
		ExpectEQ(CDateHelper::GetDateOnly(44000.125).m_dt, 44000.0);
		ExpectEQ(CDateHelper::GetDateOnly(44000.0).m_dt, 44000.0);
		ExpectEQ(CDateHelper::GetDateOnly(-44000.125).m_dt, -44000.0);
		ExpectEQ(CDateHelper::GetDateOnly(-44000.0).m_dt, -44000.0);
	}
}

void CDateHelperTest::TestGetTimeOnly()
{
	CTDCScopedTest test(*this, _T("CDateHelper::GetTimeOnly"));

	// Null dates
	{
#ifndef _DEBUG // because CDateHelper asserts on Null dates

		ExpectEQ(CDateHelper::GetTimeOnly(CDateHelper::NullDate()), CDateHelper::NullDate());

#endif
	}
	
	// Both dates are 'Zero Day' - December 30, 1899, midnight
	// Note: time component is always positive
	{
		ExpectEQ(CDateHelper::GetTimeOnly(0.125).m_dt, 0.125);
		ExpectEQ(CDateHelper::GetTimeOnly(-0.125).m_dt, 0.125);
	}

	// Positive dates
	{
		ExpectEQ(CDateHelper::GetTimeOnly(44000.125).m_dt, 0.125);
		ExpectEQ(CDateHelper::GetTimeOnly(44000.0).m_dt, 0.0);
	}

	// Negative dates
	{
		ExpectEQ(CDateHelper::GetTimeOnly(-44000.125).m_dt, 0.125);
		ExpectEQ(CDateHelper::GetTimeOnly(-44000.0).m_dt, 0.0);
	}
}

void CDateHelperTest::TestMakeDate()
{
	CTDCScopedTest test(*this, _T("CDateHelper::MakeDate"));

	// One and/or other date is null
	{
#ifndef _DEBUG // because CDateHelper will assert on Null dates

		COleDateTime dt1(CDateHelper::NullDate()), dt2(34000.375);

		ExpectEQ(CDateHelper::MakeDate(dt1, dt2), CDateHelper::NullDate());
		ExpectEQ(CDateHelper::MakeDate(dt2, dt1), CDateHelper::NullDate());
		ExpectEQ(CDateHelper::MakeDate(dt1, dt2), CDateHelper::NullDate());

#endif
	}


	// One and/or other date is 'Zero Hour' - December 30, 1899, midnight
	{
		{
			COleDateTime dt1(0.0), dt2(34000.375);

			ExpectEQ(CDateHelper::MakeDate(dt1, dt2).m_dt, 0.375);
			ExpectEQ(CDateHelper::MakeDate(dt2, dt1).m_dt, 34000.0);
		}

		{
			COleDateTime dt1(44000.125), dt2(0.0);

			ExpectEQ(CDateHelper::MakeDate(dt1, dt2).m_dt, 44000.0);
			ExpectEQ(CDateHelper::MakeDate(dt2, dt1).m_dt, 0.125);
		}

		{
			COleDateTime dt1(0.0), dt2(0.0);

			ExpectEQ(CDateHelper::MakeDate(dt2, dt1).m_dt, 0.0);
		}
	}


	// Positive/Negative dates
	{
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
	}
}

void CDateHelperTest::TestCompare()
{
	CTDCScopedTest test(*this, _T("CDateHelper::Compare"));

	// One and/or other dates is 'Null'
	{
		// CDateHelper intentionally won't assert here
		{
			COleDateTime dt1(CDateHelper::NullDate()), dt2(0.25);

			ExpectTrue(CDateHelper::Compare(dt1, dt2, 0) > 0);
			ExpectTrue(CDateHelper::Compare(dt1, dt2, DHC_COMPARETIME) > 0);
		}

		{
			COleDateTime dt1(0.125), dt2(CDateHelper::NullDate());

			ExpectTrue(CDateHelper::Compare(dt1, dt2, 0) < 0);
			ExpectTrue(CDateHelper::Compare(dt1, dt2, DHC_COMPARETIME) < 0);
		}

		{
			COleDateTime dt1(CDateHelper::NullDate()), dt2(CDateHelper::NullDate());

			ExpectTrue(CDateHelper::Compare(dt1, dt2, 0) == 0);
			ExpectTrue(CDateHelper::Compare(dt1, dt2, DHC_COMPARETIME) == 0);
		}
	}

	// Both dates are 'Zero Day' - December 30, 1899
	{
		{
			COleDateTime dt1(0.125), dt2(0.25);

			ExpectTrue(CDateHelper::Compare(dt1, dt2, 0) == 0);
			ExpectTrue(CDateHelper::Compare(dt1, dt2, DHC_COMPARETIME) < 0);
		}
	}

	// Positive dates
	{
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
			COleDateTime dt1(44000.125000001), dt2(44000.125);

			ExpectTrue(CDateHelper::Compare(dt1, dt2, DHC_COMPARETIME | DHC_COMPARESECONDS) == 0);
		}
	}

	// Negative dates
	// Note: COleDateTime always treats time as positive
	{
		{
			COleDateTime dt1(-0.125), dt2(-0.25);

			ExpectTrue(CDateHelper::Compare(dt1, dt2, 0) == 0);
			ExpectTrue(CDateHelper::Compare(dt1, dt2, DHC_COMPARETIME) < 0);
		}

		{
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
	}
}

void CDateHelperTest::Test64BitDates()
{
	CTDCScopedTest test(*this, _T("CDateHelper::64BitDates"));

	// Null dates
	{
#ifndef _DEBUG // because CDateHelper will assert on Null dates

		COleDateTime dtNull(CDateHelper::NullDate());

		time64_t tNull;
		ExpectFalse(CDateHelper::GetTimeT64(dtNull, tNull));

#endif
	}

	// 'Zero Hour' - December 30, 1899, midnight
	{
		COleDateTime dtZero(0.0);

		time64_t tZero;
		ExpectTrue(CDateHelper::GetTimeT64(dtZero, tZero));

		COleDateTime dtZeroCheck = CDateHelper::GetDate(tZero);
		ExpectEQ(dtZero, dtZeroCheck);
	}

	// Now
	{
		COleDateTime dtNow = COleDateTime::GetCurrentTime();

		time64_t tNow;
		ExpectTrue(CDateHelper::GetTimeT64(dtNow, tNow));

		COleDateTime dtNowCheck = CDateHelper::GetDate(tNow);
		ExpectEQ(dtNow, dtNowCheck);
	}
}

void CDateHelperTest::TestOffsetDate()
{
	CTDCScopedTest test(*this, _T("CDateHelper::OffsetDate"));
 
	// 24/7 working week
	{
		CTwentyFourSevenWeek week;

		CDateHelper dh(week);
		ExpectTrue(dh.WorkingWeek().GetLengthInDays() == 7);

		// Don't preserve end of month
		{
			TestOffsetDate(dh, 1, FALSE);  // Forwards
			TestOffsetDate(dh, -1, FALSE); // Backwards
		}

		// Preserve end of month
		{
			TestOffsetDate(dh, 1, TRUE);   // Forwards
			TestOffsetDate(dh, -1, TRUE);  // Backwards
		}
	}
 
	// 'Standard' working week
	// Note: Make it 24 hour day to simplify checks
	{
		CWorkingWeek week((DWORD)(DHW_SATURDAY | DHW_SUNDAY), 24);

		CDateHelper dh(week);
		ExpectTrue(dh.WorkingWeek().GetLengthInDays() == 5);

		// Don't preserve end of month
		{
			TestOffsetDate(dh, 1, FALSE);  // Forwards
			TestOffsetDate(dh, -1, FALSE); // Backwards
		}

		// Preserve end of month
		{
 			TestOffsetDate(dh, 1, TRUE);   // Forwards
 			TestOffsetDate(dh, -1, TRUE);  // Backwards
		}
	}
}

void CDateHelperTest::TestOffsetDate(const CDateHelper& dh, int nDir, BOOL bPreserveEndOfMonth)
{
	// Invalid arguments
	{
		CTDCScopedSubTest test(*this, _T("Invalid Arguments"));

		// Date (CDateHelper will intentionally assert the Null dates)
#ifndef _DEBUG
		COleDateTime dtNull(CDateHelper::NullDate());
		ExpectFalse(CDateHelper::IsDateSet(dtNull));

		ExpectFalse(dh.OffsetDate(dtNull, 10, DHU_DAYS, bPreserveEndOfMonth));
		ExpectFalse(dh.OffsetDate(dtNull, -10, DHU_DAYS, bPreserveEndOfMonth));

		// dtNull should still be null
		ExpectFalse(CDateHelper::IsDateSet(dtNull));
#endif
		// Amount
		ExpectFalse(dh.OffsetDate(COleDateTime::GetCurrentTime(), 0, DHU_DAYS, bPreserveEndOfMonth));
		ExpectFalse(dh.OffsetDate(COleDateTime::GetCurrentTime(), 0, DHU_DAYS, bPreserveEndOfMonth));

		// Units
		ExpectFalse(dh.OffsetDate(COleDateTime::GetCurrentTime(), 10, DHU_NULL, bPreserveEndOfMonth));
		ExpectFalse(dh.OffsetDate(COleDateTime::GetCurrentTime(), -10, DHU_NULL, bPreserveEndOfMonth));
	}

	// ------------------------------------------------------------

	// The dates have a time component so we can show that
	// time is preserved during offsets
	const COleDateTime DATES[] =
	{
		// leap year
		COleDateTime(2024, 1,   1, 1, 8, 2),
		COleDateTime(2024, 1,  31, 2, 7, 4),
		COleDateTime(2024, 2,  28, 3, 6, 6),
		COleDateTime(2024, 2,  29, 4, 5, 8),
		COleDateTime(2024, 6,  15, 5, 4, 1),
		COleDateTime(2024, 9,  30, 6, 3, 3),
		COleDateTime(2024, 11, 11, 7, 2, 5),
		COleDateTime(2024, 12, 31, 8, 1, 7),

		// Arbitrary past non-leap year
		COleDateTime(1039, 1,   1, 1, 8, 2),
		COleDateTime(1039, 1,  31, 2, 7, 4),
		COleDateTime(1039, 2,  28, 3, 6, 6),
		COleDateTime(1039, 6,  15, 5, 4, 1),
		COleDateTime(1039, 9,  30, 6, 3, 3),
		COleDateTime(1039, 11, 11, 7, 2, 5),
		COleDateTime(1039, 12, 31, 8, 1, 7),

		// Arbitrary future year
		COleDateTime(3039, 1,   1, 1, 8, 2),
		COleDateTime(3039, 1,  31, 2, 7, 4),
		COleDateTime(3039, 2,  28, 3, 6, 6),
		COleDateTime(3039, 6,  15, 5, 4, 1),
		COleDateTime(3039, 9,  30, 6, 3, 3),
		COleDateTime(3039, 11, 11, 7, 2, 5),
		COleDateTime(3039, 12, 31, 8, 1, 7),

	};
	const int NUM_DATES = (sizeof(DATES) / sizeof(DATES[0]));

	// ---------------------------------------

	const DH_UNITS UNITS[] = { DHU_WEEKDAYS, DHU_DAYS, DHU_WEEKS, DHU_MONTHS, DHU_YEARS };
	const int NUM_UNITS = (sizeof(UNITS) / sizeof(UNITS[0]));

	// ---------------------------------------

	const int OFFSETS[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
	const int NUM_OFFSETS = (sizeof(OFFSETS) / sizeof(OFFSETS[0]));

	// ---------------------------------------

	for (int i = 0; i < NUM_DATES; i++)
	{
		const COleDateTime& date = DATES[i];

		for (int j = 0; j < NUM_UNITS; j++)
		{
			const DH_UNITS nUnits = UNITS[j];

			for (int k = 0; k < NUM_OFFSETS; k++)
			{
				const int nOffset = (nDir * OFFSETS[k]);

				TestOffsetDate(dh, date, nUnits, nOffset, bPreserveEndOfMonth);
			}
		}
	}
}

void CDateHelperTest::TestOffsetDate(const CDateHelper& dh, const COleDateTime& date, DH_UNITS nUnits, int nOffset, BOOL bPreserveEndOfMonth)
{
	CTDCScopedSubTest test(*this, Misc::Format(_T("%f, %d %s, %d-day week, %s"),
											   date.m_dt,
											   nOffset,
											   GetUnitsText(nUnits),
											   dh.WorkingWeek().GetLengthInDays(),
											   bPreserveEndOfMonth ? _T("Preserve EOM") : _T("Don't preserve EOM")));
	COleDateTime dtOffset(date);

	// Common tests
	ExpectTrue(dh.OffsetDate(dtOffset, nOffset, nUnits, bPreserveEndOfMonth));
	ExpectEQ(CDateHelper::GetTimeOnly(date), CDateHelper::GetTimeOnly(dtOffset));

	// Custom tests
	switch (nUnits)
	{
	case DHU_DAYS:		
		ExpectEQ((double)nOffset, (dtOffset.m_dt - date.m_dt));
		break;

	case DHU_WEEKDAYS:	
		if (dh.Weekend().IsWeekend(date) && (nOffset > 0))
		{
			ExpectEQ(nOffset, dh.CalcDaysFromTo(date, dtOffset, FALSE) + 1);
		}
		else
		{
			ExpectEQ(nOffset, dh.CalcDaysFromTo(date, dtOffset, FALSE));
		}
		break;

	case DHU_WEEKS:		
		ExpectEQ((nOffset * 7.0), (dtOffset.m_dt - date.m_dt));
		break;

	case DHU_MONTHS:	
	case DHU_YEARS:
		{
			if (nUnits == DHU_MONTHS)
				ExpectEQ(nOffset, (CDateHelper::GetDateInMonths(dtOffset) - CDateHelper::GetDateInMonths(date)));
			else
				ExpectEQ(nOffset * 12, (CDateHelper::GetDateInMonths(dtOffset) - CDateHelper::GetDateInMonths(date)));

			// Common tests
			if (bPreserveEndOfMonth)
			{
				ExpectTrue(!CDateHelper::IsEndOfMonth(date) || CDateHelper::IsEndOfMonth(dtOffset));
			}
			else
			{
				if (date.GetDay() > CDateHelper::GetDaysInMonth(dtOffset))
					ExpectTrue(CDateHelper::IsEndOfMonth(dtOffset));
				else
					ExpectEQ(date.GetDay(), dtOffset.GetDay());
			}
		}
		break;
	}
}

CString CDateHelperTest::GetUnitsText(DH_UNITS nUnits)
{
	switch (nUnits)
	{
	case DHU_DAYS:		return _T("Days");
	case DHU_WEEKDAYS:	return _T("Weekdays");
	case DHU_WEEKS:		return _T("Weeks");
	case DHU_MONTHS:	return _T("Months");
	case DHU_YEARS:		return _T("Years");
	}
	
	return _T("");
}
