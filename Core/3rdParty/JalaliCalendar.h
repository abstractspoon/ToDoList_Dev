#pragma once

//////////////////////////////////////////////////////////////////////

class CJalaliCalendar
{
public:
	static BOOL IsActive();

	static CString GetDayOfWeekName(int JDayOfWeek);
	static CString GetMonthName(int JMonth);
	
	static BOOL IsEndOfMonth(int JYear, int JMonth, int JDay);
	static int GetDaysInMonth(int JYear, int JMonth);
	static BOOL IsLeapYear(int JYear);

	static void GetCurrentDate(int *JYear, int *JMonth, int *JDay);
	
	static void FromGregorian(const COleDateTime& dtGregorian, int *JYear, int *JMonth, int *JDay);
	static void ToGregorian(int JYear, int JMonth, int JDay, COleDateTime& dtGregorian);

	static void FromGregorian(int GYear, int GMonth, int GDay, int *JYear, int *JMonth, int *JDay);
	static void ToGregorian(int JYear, int JMonth, int JDay, int *GYear, int *GMonth, int *GDay);
};

//////////////////////////////////////////////////////////////////////
