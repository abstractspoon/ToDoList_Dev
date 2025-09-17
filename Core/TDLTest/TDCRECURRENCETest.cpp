// TDCRECURRENCETest.cpp: implementation of the CTDCRECURRENCETest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "TDCRECURRENCETest.h"

#include "..\todolist\tdcrecurrence.h"

#include "..\shared\datehelper.h"
#include "..\shared\misc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCRECURRENCETest::CTDCRECURRENCETest(const CTestUtils& utils) 
	: 
	CTDLTestBase(_T("CTDCRECURRENCETest"), utils)
{

}

CTDCRECURRENCETest::~CTDCRECURRENCETest()
{

}

TESTRESULT CTDCRECURRENCETest::Run()
{
	ClearTotals();

	TestSetRegularity();
	TestCalcNextOccurrencesPerformance();

	return GetTotals();
}

void CTDCRECURRENCETest::TestSetRegularity()
{
	CTDCScopedTest test(*this, _T("CTDCRECURRENCE::SetRegularity"));
	
	//  nRegularity										dwSpecific1				dwSpecific2
	
	//	TDIR_DAY_EVERY_NDAYS							every 'n' days			--- (0)
	//	TDIR_DAY_EVERY_WEEKDAY							--- (0)					--- (0)
	//	TDIR_DAY_EVERY_NWEEKDAYS						every 'n' days			--- (0)
	
	//	TDIR_WEEK_SPECIFIC_DOWS_NWEEKS					every 'n' weeks			weekdays (TDIW_...)
	//	TDIR_WEEK_EVERY_NWEEKS							every 'n' weeks			--- (0)
	
	//	TDIR_MONTH_EVERY_NMONTHS						every 'n' months		--- (0)
	//	TDIR_MONTH_SPECIFIC_DAY_NMONTHS					every 'n' months		day of month (1-31)
	//	TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS				first(0), last(!0)		every 'n' months
	//	TDIR_MONTH_SPECIFIC_DOW_NMONTHS					LOWORD = which (1-5)	every 'n' months
	//													HIWORD = DOW (1-7)		
	
	//	TDIR_YEAR_SPECIFIC_DAY_MONTH					month (1-12)			day of month (1-31)
	//	TDIR_YEAR_EVERY_NYEARS							every 'n' years			--- (0)
	//  TDIR_YEAR_SPECIFIC_DOW_MONTH					LOWORD = which (1-5)	specific months (1-12, or DHM_...)
	//													HIWORD = DOW (1-7)		
	TDCRECURRENCE tr;

	//	TDIR_DAY_EVERY_NDAYS ---------------------------------------
	ExpectTrue(tr.SetRegularity(TDIR_DAY_EVERY_NDAYS, 5, 0));

	ExpectFalse(tr.SetRegularity(TDIR_DAY_EVERY_NDAYS, 5, 3));  // dwSpecific2 != 0
	ExpectFalse(tr.SetRegularity(TDIR_DAY_EVERY_NDAYS, -5, 0)); // 'n' days < 0
	ExpectFalse(tr.SetRegularity(TDIR_DAY_EVERY_NDAYS, 0, 0));  // 'n' days == 0
		
	//	TDIR_DAY_EVERY_WEEKDAY ---------------------------------------
	ExpectTrue(tr.SetRegularity(TDIR_DAY_EVERY_WEEKDAY, 0, 0));

	ExpectFalse(tr.SetRegularity(TDIR_DAY_EVERY_WEEKDAY, 5, 0));  // dwSpecific1 != 0
	ExpectFalse(tr.SetRegularity(TDIR_DAY_EVERY_WEEKDAY, -5, 0)); // dwSpecific1 != 0
	ExpectFalse(tr.SetRegularity(TDIR_DAY_EVERY_WEEKDAY, 0, 5));  // dwSpecific2 != 0

	//	TDIR_DAY_EVERY_NWEEKDAYS ---------------------------------------
	ExpectTrue(tr.SetRegularity(TDIR_DAY_EVERY_NWEEKDAYS, 5, 0));

	ExpectFalse(tr.SetRegularity(TDIR_DAY_EVERY_NWEEKDAYS, 5, 3));  // dwSpecific2 != 0
	ExpectFalse(tr.SetRegularity(TDIR_DAY_EVERY_NWEEKDAYS, -5, 0)); // 'n' weekdays < 0
	ExpectFalse(tr.SetRegularity(TDIR_DAY_EVERY_NWEEKDAYS, 0, 0));  // 'n' weekdays == 0
	
	//	TDIR_WEEK_SPECIFIC_DOWS_NWEEKS ---------------------------------------
	ExpectTrue(tr.SetRegularity(TDIR_WEEK_SPECIFIC_DOWS_NWEEKS, 5, DHW_EVERYDAY));

	ExpectFalse(tr.SetRegularity(TDIR_WEEK_SPECIFIC_DOWS_NWEEKS, 5, 0));             // DOWs == 0
	ExpectFalse(tr.SetRegularity(TDIR_WEEK_SPECIFIC_DOWS_NWEEKS, -5, DHW_EVERYDAY)); // 'n' weeks < 0
	ExpectFalse(tr.SetRegularity(TDIR_WEEK_SPECIFIC_DOWS_NWEEKS, 0, DHW_EVERYDAY));  // 'n' weeks == 0

	//	TDIR_WEEK_EVERY_NWEEKS ---------------------------------------
	ExpectTrue(tr.SetRegularity(TDIR_WEEK_EVERY_NWEEKS, 5, 0));

	ExpectFalse(tr.SetRegularity(TDIR_WEEK_EVERY_NWEEKS, 5, 3));  // dwSpecific2 != 0
	ExpectFalse(tr.SetRegularity(TDIR_WEEK_EVERY_NWEEKS, -5, 0)); // 'n' weeks < 0
	ExpectFalse(tr.SetRegularity(TDIR_WEEK_EVERY_NWEEKS, 0, 0));  // 'n' weeks == 0
		
	//	TDIR_MONTH_EVERY_NMONTHS ---------------------------------------
	ExpectTrue(tr.SetRegularity(TDIR_MONTH_EVERY_NMONTHS, 5, FALSE)); // Don't preserve weekdays
	ExpectTrue(tr.SetRegularity(TDIR_MONTH_EVERY_NMONTHS, 5, TRUE));  // Preserve weekdays

	ExpectFalse(tr.SetRegularity(TDIR_MONTH_EVERY_NMONTHS, -5, 0)); // 'n' Months < 0
	ExpectFalse(tr.SetRegularity(TDIR_MONTH_EVERY_NMONTHS, 0, 0));  // 'n' Months == 0

	//	TDIR_MONTH_SPECIFIC_DAY_NMONTHS ---------------------------------------
	ExpectTrue(tr.SetRegularity(TDIR_MONTH_SPECIFIC_DAY_NMONTHS, 5, 1));
	ExpectTrue(tr.SetRegularity(TDIR_MONTH_SPECIFIC_DAY_NMONTHS, 5, 31));

	ExpectFalse(tr.SetRegularity(TDIR_MONTH_SPECIFIC_DAY_NMONTHS, -5, 1)); // 'n' Months < 0
	ExpectFalse(tr.SetRegularity(TDIR_MONTH_SPECIFIC_DAY_NMONTHS, 0, 31)); // 'n' Months == 0
	ExpectFalse(tr.SetRegularity(TDIR_MONTH_SPECIFIC_DAY_NMONTHS, 5, 0));  // Day < 1
	ExpectFalse(tr.SetRegularity(TDIR_MONTH_SPECIFIC_DAY_NMONTHS, 5, -1)); // Day < 1
	ExpectFalse(tr.SetRegularity(TDIR_MONTH_SPECIFIC_DAY_NMONTHS, 5, 32)); // Day > 31

	//	TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS ---------------------------------------
	ExpectTrue(tr.SetRegularity(TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS, 0, 5)); // 1st weekday of May
	ExpectTrue(tr.SetRegularity(TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS, 1, 5)); // Last weekday of May

	ExpectTrue(tr.SetRegularity(TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS, 100, 5)); // Last weekday of May
	ExpectFalse(tr.SetRegularity(TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS, 0, 0)); // 'n' Months == 0
	ExpectFalse(tr.SetRegularity(TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS, 0, -1)); // 'n' Months < 0
	
	//	TDIR_MONTH_SPECIFIC_DOW_NMONTHS ---------------------------------------
	ExpectTrue(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(1, 1), 5)); // 1st Sunday
	ExpectTrue(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(5, 7), 5)); // Last Saturday

	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(1, 1), -1)); // 'n' Months < 0
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(1, 1), 0));  // 'n' Months == 0

	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(0, 1), 5)); // 'which' < 1
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(6, 7), 5)); // 'which' > 5 
	
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(1, 0), 5)); // 'DOW' < 1 
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(1, 8), 5)); // 'DOW' > 7
	
	//	TDIR_YEAR_SPECIFIC_DAY_MONTHS ---------------------------------------
	ExpectTrue(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DAY_MONTHS, 5, 1));
	ExpectTrue(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DAY_MONTHS, 5, 11));
	ExpectTrue(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DAY_MONTHS, 5, 31));
	ExpectTrue(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DAY_MONTHS, DHM_ALL, 31));
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DAY_MONTHS, 13, 1)); // 'Month' > 12 and 'Month' < DHM_JANUARY
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DAY_MONTHS, DHM_ALL + 1, 1)); // 'Month' > DHM_ALL
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DAY_MONTHS, -1, 1)); // 'Month' < 1
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DAY_MONTHS, 0, 1)); // 'Month' < 1
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DAY_MONTHS, 5, 41)); // 'Day' > 31
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DAY_MONTHS, 5, 0)); // 'Day' < 1

	//	TDIR_YEAR_EVERY_NYEARS ---------------------------------------
	ExpectTrue(tr.SetRegularity(TDIR_YEAR_EVERY_NYEARS, 5, FALSE)); // Don't preserve weekdays
	ExpectTrue(tr.SetRegularity(TDIR_YEAR_EVERY_NYEARS, 5, TRUE));  // Preserve weekdays

	ExpectFalse(tr.SetRegularity(TDIR_YEAR_EVERY_NYEARS, -5, 0)); // 'n' years < 0
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_EVERY_NYEARS, 0, 0));  // 'n' years == 0

	//  TDIR_YEAR_SPECIFIC_DOW_MONTHS ---------------------------------------
	ExpectTrue(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(1, 1), 5)); // 1st Sunday of May
	ExpectTrue(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(5, 7), 5)); // Last Saturday of May
	ExpectTrue(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(5, 7), DHM_ALL)); // Last Saturday of All months

	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(1, 1), 13)); // 'Month' > 12 and 'Month' < DHM_JANUARY
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(1, 1), 0));  // 'Month' < 1
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(1, 1), DHM_ALL + 1)); // 'Month' > DHM_ALL

	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(0, 1), 5)); // 'which' < 1
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(6, 7), 5)); // 'which' > 5 
	
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(1, 0), 5)); // 'DOW' < 1 
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(1, 8), 5)); // 'DOW' > 7
}

void CTDCRECURRENCETest::TestCalcNextOccurrencesPerformance()
{
	if (!m_utils.GetWantPerformanceTests())
	{
		_tprintf(_T("Add '-p' to run CTDCRECURRENCETest::CalcNextOccurrencesPerformance\n"));
		return;
	}

	CTDCScopedTest test(*this, _T("CTDCRECURRENCETest::CalcNextOccurrencesPerformance"));

	const TDCRECURRENCE RECURRENCES[] = 
	{
		//	TDIR_DAY_EVERY_NDAYS 
		TDCRECURRENCE(TDIR_DAY_EVERY_NDAYS, 2, 0),

		//	TDIR_DAY_EVERY_WEEKDAY 
		TDCRECURRENCE(TDIR_DAY_EVERY_WEEKDAY, 0, 0),

		//	TDIR_DAY_EVERY_NWEEKDAYS 
		TDCRECURRENCE(TDIR_DAY_EVERY_NWEEKDAYS, 2, 0),

		//	TDIR_WEEK_SPECIFIC_DOWS_NWEEKS 
		TDCRECURRENCE(TDIR_WEEK_SPECIFIC_DOWS_NWEEKS, 2, DHW_EVERYDAY),

		//	TDIR_WEEK_EVERY_NWEEKS 
		TDCRECURRENCE(TDIR_WEEK_EVERY_NWEEKS, 1, 0),

		//	TDIR_MONTH_EVERY_NMONTHS 
		TDCRECURRENCE(TDIR_MONTH_EVERY_NMONTHS, 1, 0),

		//	TDIR_MONTH_SPECIFIC_DAY_NMONTHS 
		TDCRECURRENCE(TDIR_MONTH_SPECIFIC_DAY_NMONTHS, 1, 1),
		TDCRECURRENCE(TDIR_MONTH_SPECIFIC_DAY_NMONTHS, 1, 31),

		//	TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS 
		TDCRECURRENCE(TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS, 0, 1), // 1st weekday 
		TDCRECURRENCE(TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS, 1, 1), // Last weekday

		//	TDIR_MONTH_SPECIFIC_DOW_NMONTHS 
		TDCRECURRENCE(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(1, 1), 1), // 1st Sunday
		TDCRECURRENCE(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(5, 7), 1), // Last Saturday

		//	TDIR_YEAR_SPECIFIC_DAY_MONTH 
		TDCRECURRENCE(TDIR_YEAR_SPECIFIC_DAY_MONTHS, 5, 1),
		TDCRECURRENCE(TDIR_YEAR_SPECIFIC_DAY_MONTHS, 5, 11),
		TDCRECURRENCE(TDIR_YEAR_SPECIFIC_DAY_MONTHS, 5, 31),

		//	TDIR_YEAR_EVERY_NYEARS 
		TDCRECURRENCE(TDIR_YEAR_EVERY_NYEARS, 1, 0),

		//  TDIR_YEAR_SPECIFIC_DOW_MONTH 
		TDCRECURRENCE(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(1, 1), 5), // 1st Sunday of May
		TDCRECURRENCE(TDIR_YEAR_SPECIFIC_DOW_MONTHS, MAKELONG(5, 7), 5), // Last Saturday of May
	};
	const int NUM_RECURRENCE = (sizeof(RECURRENCES) / sizeof(RECURRENCES[0]));

	COleDateTime date(CDateHelper::GetDate(DHD_TODAY));
	CArray<double, double&> aRecurrences;

#ifdef _DEBUG
	COleDateTimeRange range(DHD_ENDTHISYEAR, 1, DHU_YEARS);
#else
	COleDateTimeRange range(DHD_ENDNEXTYEAR, 4, DHU_YEARS);
#endif

	for (int nRecur = 0; nRecur < NUM_RECURRENCE; nRecur++)
	{
		// Run test
		DWORD dwTickStart = GetTickCount();
		const TDCRECURRENCE& tr = RECURRENCES[nRecur];

		int nNumRecur = tr.CalcNextOccurrences(date, range, aRecurrences);

		DWORD dwDuration = (GetTickCount() - dwTickStart);

		// Output results
		if (nNumRecur == 0)
		{
			_tprintf(_T("Recurrence (%s) failed to produce any recurrences for the range: %s\n"), (LPCTSTR)FormatRecurrenceRegularity(tr), (LPCTSTR)range.Format());
		}
		else
		{
			_tprintf(_T("Recurrence (%s) took %ld ms to calculate %d recurrences for the range: %s\n"), (LPCTSTR)FormatRecurrenceRegularity(tr), dwDuration, nNumRecur, (LPCTSTR)range.Format());
		}
	}
}

CString CTDCRECURRENCETest::FormatRecurrenceRegularity(const TDCRECURRENCE& tr)
{
	DWORD dwSpecific1 = 0, dwSpecific2 = 0;
	TDC_REGULARITY nRegularity = tr.GetRegularity(dwSpecific1, dwSpecific2);

	CString sRegularity = TDCRECURRENCE::GetRegularityText(nRegularity, FALSE);

	if (dwSpecific1 > 0xffff)
		return Misc::Format(_T("%s, (%ld, %ld), %ld"), sRegularity, LOWORD(dwSpecific1), HIWORD(dwSpecific1), dwSpecific2);

	// else
	return Misc::Format(_T("%s, %ld, %ld"), sRegularity, dwSpecific1, dwSpecific2);
}
