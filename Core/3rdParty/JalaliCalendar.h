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
	static void GetCurrentDate(int *JYear, int *JMonth, int *JDay, int *JDayOfWeek = NULL);

	static CString GetDayOfWeekName(int JDayOfWeek); // 1 = Saturday
	static CString GetMonthName(int JMonth);
	
	static void GregorianToJalali(const COleDateTime& dtGregorian, COleDateTime& dtJalali, int *JDayOfWeek = NULL);
	static void JalaliToGregorian(const COleDateTime& dtJalali, COleDateTime& dtGregorian, int *GDayOfWeek = NULL);

	static void GregorianToJalali(int GYear, int GMonth, int GDay, int *JYear, int *JMonth, int *JDay, int *JDayOfWeek = NULL);
	static void JalaliToGregorian(int JYear, int JMonth, int JDay, int *GYear, int *GMonth, int *GDay, int *GDayOfWeek = NULL);
	
	static void GetJalaliDayOfWeek(int JYear, int JMonth, int JDay, int *DayOfWeek);
	static void GetGregorianDayOfWeek(int GYear, int GMonth, int GDay, int *DayOfWeek);

	// To get difference of two dates
	static int GetGregorianDayDifference(int GYear1, int GMonth1, int GDay1, int GYear2, int GMonth2, int GDay2);
	static int GetJalaliDayDifference(int JYear1, int JMonth1, int JDay1, int JYear2, int JMonth2, int JDay2);

	// To calculate a date after/before another date
	static void GetJalaliDateWithOffset(int JYear, int JMonth, int JDay, int Offset, int *Year, int *Month, int *Day, int *DayOfWeek = NULL);
	static void GetGregorianDateWithOffset(int GYear, int GMonth, int GDay, int Offset, int *Year, int *Month, int *Day, int *DayOfWeek = NULL);

private:
	static int GetGregorianOffset(int GYear, int GMonth, int GDay);
	static int GetJalaliOffset(int JYear, int JMonth, int JDay);
};

//////////////////////////////////////////////////////////////////////
