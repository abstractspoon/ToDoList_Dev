// TDCRECURRENCETest.cpp: implementation of the CTDCRECURRENCETest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "TDCRECURRENCETest.h"

#include "..\todolist\tdcrecurrence.h"

#include "..\shared\datehelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCRECURRENCETest::CTDCRECURRENCETest(const CTestUtils& utils) : CTDLTestBase(utils)
{

}

CTDCRECURRENCETest::~CTDCRECURRENCETest()
{

}

void CTDCRECURRENCETest::Run()
{
	TestSetRegularity();
}

void CTDCRECURRENCETest::TestSetRegularity()
{
	BeginTest(_T("CTDCRECURRENCETest::SetRegularity"));
	
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
	//  TDIR_YEAR_SPECIFIC_DOW_MONTH					LOWORD = which (1-5)	specific month (1-12)
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
	ExpectTrue(tr.SetRegularity(TDIR_MONTH_EVERY_NMONTHS, 5, 0));

	ExpectFalse(tr.SetRegularity(TDIR_MONTH_EVERY_NMONTHS, 5, 3));  // dwSpecific2 != 0
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
	ExpectTrue(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTH, MAKELONG(1, 1), 5)); // 1st Sunday
	ExpectTrue(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTH, MAKELONG(5, 7), 5)); // Last Saturday

	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTH, MAKELONG(1, 1), -1)); // 'n' Months < 0
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTH, MAKELONG(1, 1), 0));  // 'n' Months == 0

	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTH, MAKELONG(0, 1), 5)); // 'which' < 1
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTH, MAKELONG(6, 7), 5)); // 'which' > 5 
	
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTH, MAKELONG(1, 0), 5)); // 'DOW' < 1 
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTH, MAKELONG(1, 8), 5)); // 'DOW' > 7
	
	//	TDIR_YEAR_SPECIFIC_DAY_MONTH ---------------------------------------
	ExpectTrue(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DAY_MONTH, 5, 1));
	ExpectTrue(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DAY_MONTH, 5, 11));
	ExpectTrue(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DAY_MONTH, 5, 31));
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DAY_MONTH, 13, 1));
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DAY_MONTH, -1, 1));
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DAY_MONTH, 0, 1));
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DAY_MONTH, 5, 41));
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DAY_MONTH, 5, 0));

	//	TDIR_YEAR_EVERY_NYEARS ---------------------------------------
	ExpectTrue(tr.SetRegularity(TDIR_YEAR_EVERY_NYEARS, 5, 0));
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_EVERY_NYEARS, 5, 3));  // dwSpecific2 != 0
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_EVERY_NYEARS, -5, 0)); // 'n' years < 0
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_EVERY_NYEARS, 0, 0));  // 'n' years == 0

	//  TDIR_YEAR_SPECIFIC_DOW_MONTH ---------------------------------------
	ExpectTrue(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTH, MAKELONG(1, 1), 5)); // 1st Sunday of May
	ExpectTrue(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTH, MAKELONG(5, 7), 5)); // Last Saturday of May

	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTH, MAKELONG(1, 1), 13)); // 'Month' > 12
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTH, MAKELONG(1, 1), 0));  // 'Month' < 1

	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTH, MAKELONG(0, 1), 5)); // 'which' < 1
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTH, MAKELONG(6, 7), 5)); // 'which' > 5 
	
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTH, MAKELONG(1, 0), 5)); // 'DOW' < 1 
	ExpectFalse(tr.SetRegularity(TDIR_YEAR_SPECIFIC_DOW_MONTH, MAKELONG(1, 8), 5)); // 'DOW' > 7
		
	//  ---------------------------------------

	EndTest();
}
