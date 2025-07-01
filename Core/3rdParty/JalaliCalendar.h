// Original code (c) Ali Tavakol, CodeProject, Sep 24, 2007

#pragma once

//////////////////////////////////////////////////////////////////////

#ifndef LANG_PERSIAN
#	define LANG_PERSIAN 0x29
#endif

//////////////////////////////////////////////////////////////////////

class CJalaliCalendar
{
public:
	static BOOL IsActive();

	static CString GetDayOfWeekName(int JDayOfWeek);
	static CString GetMonthName(int JMonth);
	
	static int GetDaysInMonth(int JYear, int JMonth);
	static BOOL IsLeapYear(int JYear);

	static void GetCurrentDate(int *JYear, int *JMonth, int *JDay);
	
	static void FromGregorian(const COleDateTime& dtGregorian, int *JYear, int *JMonth, int *JDay);
	static void ToGregorian(int JYear, int JMonth, int JDay, COleDateTime& dtGregorian);

	static void FromGregorian(int GYear, int GMonth, int GDay, int *JYear, int *JMonth, int *JDay);
	static void ToGregorian(int JYear, int JMonth, int JDay, int *GYear, int *GMonth, int *GDay);
};

//////////////////////////////////////////////////////////////////////
