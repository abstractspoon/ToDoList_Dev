
#include "StdAfx.h"
#include "JalaliCalendar.h"

//////////////////////////////////////////////////////////////////////

#ifndef LANG_PERSIAN
#	define LANG_PERSIAN 0x29
#endif

////////////////////////////////////////////////////////////////////

// Day of week names in Persian script
TCHAR DOWNAME_LONG_1[] = { 0x06CC, 0x06A9, 0x200C, 0x0634, 0x0646, 0x0628, 0x0647, 0x0009, 0x0 };					// yeksanbe,	sunday
TCHAR DOWNAME_LONG_2[] = { 0x062F, 0x0648, 0x200C, 0x0634, 0x0646, 0x0628, 0x0647, 0x0009, 0x0 };					// dosanbe,		monday
TCHAR DOWNAME_LONG_3[] = { 0x0633, 0x0647, 0x200C, 0x0634, 0x0646, 0x0628, 0x0647, 0x0009, 0x0 };					// sesanbe,		tuesday
TCHAR DOWNAME_LONG_4[] = { 0x0686, 0x0647, 0x0627, 0x0631, 0x200C, 0x0634, 0x0646, 0x0628, 0x0647, 0x0009, 0x0 };	// čaharsanbe,	wednesday
TCHAR DOWNAME_LONG_5[] = { 0x067E, 0x0646, 0x062C, 0x200C, 0x0634, 0x0646, 0x0628, 0x0647, 0x0009, 0x0 };			// panjsanbe,	thursday
TCHAR DOWNAME_LONG_6[] = { 0x062C, 0x0645, 0x0639, 0x0647, 0x0009, 0x0009, 0x0 };									// Jom-e,		friday
TCHAR DOWNAME_LONG_7[] = { 0x0634, 0x0646, 0x0628, 0x0647, 0x0009, 0x0009, 0x0 };									// sanbe,		saturday

// Day of week names in Persian script
TCHAR DOWNAME_SHORT_1[] = { 0x06CC, 0x06A9, 0x0 };					// yeksanbe,	sunday
TCHAR DOWNAME_SHORT_2[] = { 0x062F, 0x0648, 0x0 };					// dosanbe,		monday
TCHAR DOWNAME_SHORT_3[] = { 0x0633, 0x0647, 0x0 };					// sesanbe,		tuesday
TCHAR DOWNAME_SHORT_4[] = { 0x0686, 0x0647, 0x0627, 0x0631, 0x0 };	// čaharsanbe,	wednesday
TCHAR DOWNAME_SHORT_5[] = { 0x067E, 0x0646, 0x062C, 0x0 };			// panjsanbe,	thursday
TCHAR DOWNAME_SHORT_6[] = { 0x062C, 0x0645, 0x0639, 0x0647, 0x0 };	// Jom-e,		friday
TCHAR DOWNAME_SHORT_7[] = { 0x0634, 0x0646, 0x0628, 0x0647, 0x0 };	// sanbe,		saturday

////////////////////////////////////////////////////////////////////

// Month names in Persian script
TCHAR MONTHNAME_1[] =  { 0x0641, 0x0631, 0x0648, 0x0631, 0x062F, 0x06CC, 0x0646, 0x0  };		// Farvardin
TCHAR MONTHNAME_2[] =  { 0x0627, 0x0631, 0x062F, 0x06CC, 0x0628, 0x0647, 0x0634, 0x062A, 0x0 }; // Ordibehesht
TCHAR MONTHNAME_3[] =  { 0x062E, 0x0631, 0x062F, 0x0627, 0x062F, 0x0 };							// Khordad
TCHAR MONTHNAME_4[] =  { 0x062A, 0x06CC, 0x0631, 0x0 };											// Tir
TCHAR MONTHNAME_5[] =  { 0x0645, 0x0631, 0x062F, 0x0627, 0x062F, 0x0};							// Mordad
TCHAR MONTHNAME_6[] =  { 0x0634, 0x0647, 0x0631, 0x06CC, 0x0648, 0x0631, 0x0};					// Shahrivar
TCHAR MONTHNAME_7[] =  { 0x0645, 0x0647, 0x0631, 0x0};											// Mehr
TCHAR MONTHNAME_8[] =  { 0x0622, 0x0628, 0x0627, 0x0646, 0x0};									// Aban
TCHAR MONTHNAME_9[] =  { 0x0622, 0x0630, 0x0631, 0x0};											// Azar
TCHAR MONTHNAME_10[] = { 0x062F, 0x06CC, 0x0};													// Dey
TCHAR MONTHNAME_11[] = { 0x0628, 0x0647, 0x0645, 0x0646, 0x0};									// Bahman
TCHAR MONTHNAME_12[] = { 0x0627, 0x0633, 0x0641, 0x0646, 0x062F, 0x0};							// Esfand

////////////////////////////////////////////////////////////////////
// https://github.com/RayanFar/mysql-jalali-date-converter-plugin/blob/master/gregorian_to_jalali.c

typedef struct { int year, month, day; } date_struct;

static int jalali_month_days(int m)
{
	return (m <= 6) ? 31 : ((m <= 11) ? 30 : 29);
}

static int gregorian_month_days(int y, int m)
{
	static const int md[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (m == 2)
	{
		if ((y % 400 == 0) || ((y % 4 == 0) && (y % 100 != 0)))
			return 29;
	}
	if (m >= 1 && m <= 12)
		return md[m - 1];
	return 0;
}

static date_struct gregorian_to_jalali_core(int gy, int gm, int gd)
{
	date_struct res;
	int gy2 = gy - 1600;
	int gm2 = gm - 1;
	int gd2 = gd - 1;

	long g_day_no = 365L * gy2 + (gy2 + 3) / 4 - (gy2 + 99) / 100 + (gy2 + 399) / 400;
	{
		for (int i = 0; i < gm2; ++i)
			g_day_no += gregorian_month_days(gy, i + 1);
	}
	g_day_no += gd2;

	long j_day_no = g_day_no - 79;
	long j_np = j_day_no / 12053;
	j_day_no %= 12053;

	long jy = 979 + 33 * j_np + 4 * (j_day_no / 1461);
	j_day_no %= 1461;

	if (j_day_no >= 366)
	{
		jy += (j_day_no - 1) / 365;
		j_day_no = (j_day_no - 1) % 365;
	}

	int jm = 0;
	int jd = 0;
	{
		for (int i = 1; i <= 12; ++i)
		{
			int dim = jalali_month_days(i);

			if ((j_day_no < dim) || ((i == 12) && (j_day_no == dim)))
			{
				jm = i;
				jd = j_day_no + 1;
				break;
			}
			j_day_no -= dim;
		}
	}

	res.year = (int)jy;
	res.month = jm;
	res.day = jd;
	return res;
}

static date_struct jalali_to_gregorian_core(int jy, int jm, int jd)
{
	date_struct res;
	int jy2 = jy - 979;
	int jm2 = jm - 1;
	int jd2 = jd - 1;

	long j_day_no = 365L * jy2 + (jy2 / 33) * 8 + ((jy2 % 33 + 3) / 4);
	{
		for (int i = 0; i < jm2; ++i)
			j_day_no += jalali_month_days(i + 1);
	}
	j_day_no += jd2;

	long g_day_no = j_day_no + 79;
	long gy = 1600 + 400 * (g_day_no / 146097);
	g_day_no %= 146097;

	int leap = 1;
	if (g_day_no >= 36525)
	{
		g_day_no--;
		gy += 100 * (g_day_no / 36524);
		g_day_no %= 36524;
		if (g_day_no >= 365)
			g_day_no++;
		else
			leap = 0;
	}

	gy += 4 * (g_day_no / 1461);
	g_day_no %= 1461;

	if (g_day_no >= 366)
	{
		leap = 0;
		g_day_no--;
		gy += g_day_no / 365;
		g_day_no %= 365;
	}

	int gm = 0;
	int gd = 0;
	{
		for (int i = 1; i <= 12; ++i)
		{
			int dim = gregorian_month_days((int)gy, i);
			if (i == 2 && !leap)
				dim = 28;
			else if (i == 2 && leap)
				dim = 29;
			if (g_day_no < dim)
			{
				gm = i;
				gd = g_day_no + 1;
				break;
			}
			g_day_no -= dim;
		}
	}

	res.year = (int)gy;
	res.month = gm;
	res.day = gd;
	return res;
}

////////////////////////////////////////////////////////////////////

BOOL CJalaliCalendar::IsActive()
{
	LCID lcid = ::GetThreadLocale();
	LANGID lid = LANGIDFROMLCID(lcid);

	switch (PRIMARYLANGID(lid))
	{
	case LANG_ARABIC:
	case LANG_PERSIAN:
		return TRUE;
	}

	return FALSE;
}

int CJalaliCalendar::GetDaysInMonth(int JYear, int JMonth)
{
	switch (JMonth)
	{
	case 1:  return 31; // Farvardin
	case 2:  return 31; // Ordibehesht
	case 3:  return 31; // Khordad
	case 4:  return 31; // Tir
	case 5:  return 31; // Mordad
	case 6:  return 31; // Shahrivar
	case 7:  return 30; // Mehr
	case 8:  return 30; // Aban
	case 9:  return 30; // Azar
	case 10: return 30; // Dey
	case 11: return 30; // Bahman

	case 12: return (IsLeapYear(JYear) ? 30 : 29); // Esfand
	}

	// all else
	return 0;
}

BOOL CJalaliCalendar::IsEndOfMonth(int JYear, int JMonth, int JDay)
{
	return (GetDaysInMonth(JYear, JMonth) == JDay);
}

BOOL CJalaliCalendar::IsLeapYear(int JYear)
{
	COleDateTime dt29_12 = ToGregorian(JYear, 12, 29);

	int JCheckYear, JCheckMonth, JCheckDay;
	FromGregorian((dt29_12.m_dt + 1), &JCheckYear, &JCheckMonth, &JCheckDay);
	
	BOOL bLeapYear = (JCheckYear == JYear);

	if (bLeapYear)
	{
		ASSERT(JCheckMonth == 12);
		ASSERT(JCheckDay == 30);
	}
	else
	{
		ASSERT(JCheckYear == JYear + 1);
		ASSERT(JCheckMonth == 1);
		ASSERT(JCheckDay == 1);
	}

	return bLeapYear;
}

int CJalaliCalendar::GetDayOfYear(int JYear, int JMonth, int JDay)
{
	int nDayOfYear = 0;

	for (int nTemp = 1; nTemp < JMonth; nTemp++)
		nDayOfYear += GetDaysInMonth(JYear, nTemp);

	return (nDayOfYear + JDay);
}

int CJalaliCalendar::GetWeekOfYear(int JYear, int JMonth, int JDay)
{
	// Use the US method
	// Note: We assume that the DOW for the first day
	// of the year is the same FOR NOW
	COleDateTime dtGreg = ToGregorian(JYear, 1, 1);
	int nStartDOW = dtGreg.GetDayOfWeek();

	int nDayOfYear = GetDayOfYear(JYear, JMonth, JDay);
	int nWeek = (((nDayOfYear + nStartDOW - 1) / 7) + 1);

	switch (nWeek)
	{
	case 53:
		{
			// Since week 53 could be week 1 of the next year
			// we check the week number a week later
			if (GetWeekOfYear(dtGreg.m_dt + 7) == 2) // RECURSIVE CALL
				nWeek = 1;
		}
		break;

	case 54:
		nWeek = 1;
		break;
	}
	ASSERT((nWeek >= 1) && (nWeek <= 53));

	return nWeek;
}

CString CJalaliCalendar::GetMonthName(int JMonth)
{
	switch (JMonth)
	{
	case 1:  return MONTHNAME_1;  // Farvardin
	case 2:  return MONTHNAME_2;  // Ordibehesht
	case 3:  return MONTHNAME_3;  // Khordad
	case 4:  return MONTHNAME_4;  // Tir
	case 5:  return MONTHNAME_5;  // Mordad
	case 6:  return MONTHNAME_6;  // Shahrivar
	case 7:  return MONTHNAME_7;  // Mehr
	case 8:  return MONTHNAME_8;  // Aban
	case 9:  return MONTHNAME_9;  // Azar
	case 10: return MONTHNAME_10; // Dey
	case 11: return MONTHNAME_11; // Bahman
	case 12: return MONTHNAME_12; // Esfand
	}

	// all else
	return "";
}

CString CJalaliCalendar::GetDayOfWeekName(int JDayOfWeek, BOOL bShort)
{
	switch (JDayOfWeek)
	{
	case 1:  return (bShort ? DOWNAME_SHORT_1: DOWNAME_LONG_1); // sanbe
	case 2:  return (bShort ? DOWNAME_SHORT_2: DOWNAME_LONG_2); // yeksanbe
	case 3:  return (bShort ? DOWNAME_SHORT_3: DOWNAME_LONG_3); // dosanbe
	case 4:  return (bShort ? DOWNAME_SHORT_4: DOWNAME_LONG_4); // sesanbe
	case 5:  return (bShort ? DOWNAME_SHORT_5: DOWNAME_LONG_5); // čaharsanbe
	case 6:  return (bShort ? DOWNAME_SHORT_6: DOWNAME_LONG_6); // panjsanbe
	case 7:  return (bShort ? DOWNAME_SHORT_7: DOWNAME_LONG_7); // Jom-e
	}

	// all else
	return "";
}

void CJalaliCalendar::FromGregorian(const COleDateTime& dtGregorian, int *JYear, int *JMonth, int *JDay)
{
	SYSTEMTIME stGregorian = { 0 };
	dtGregorian.GetAsSystemTime(stGregorian);

	FromGregorian((int)stGregorian.wYear, (int)stGregorian.wMonth, (int)stGregorian.wDay, JYear, JMonth, JDay);
}

COleDateTime CJalaliCalendar::ToGregorian(int JYear, int JMonth, int JDay)
{
	int GYear, GMonth, GDay;
	ToGregorian(JYear, JMonth, JDay, &GYear, &GMonth, &GDay);

	return COleDateTime(GYear, GMonth, GDay, 0, 0, 0);
}

void CJalaliCalendar::GetCurrentDate(int *JYear, int *JMonth, int *JDay)
{
	SYSTEMTIME st;

	GetLocalTime(&st);
	FromGregorian(st.wYear, st.wMonth, st.wDay, JYear, JMonth, JDay);
}

void CJalaliCalendar::FromGregorian(int GYear, int GMonth, int GDay, int *JYear, int *JMonth, int *JDay)
{
	date_struct dtJalali = gregorian_to_jalali_core(GYear, GMonth, GDay);

	*JYear = dtJalali.year;
	*JMonth = dtJalali.month;
	*JDay = dtJalali.day;

#ifdef _DEBUG
	// Consistency check
	date_struct dtGregCheck = jalali_to_gregorian_core(*JYear, *JMonth, *JDay);

	ASSERT(dtGregCheck.year == GYear);
	ASSERT(dtGregCheck.month == GMonth);
	ASSERT(dtGregCheck.day == GDay);
#endif
}

void CJalaliCalendar::ToGregorian(int JYear, int JMonth, int JDay, int *GYear, int *GMonth, int *GDay)
{
	date_struct dtGreg = jalali_to_gregorian_core(JYear, JMonth, JDay);

	*GYear = dtGreg.year;
	*GMonth = dtGreg.month;
	*GDay = dtGreg.day;

#ifdef _DEBUG
	// Consistency check
	date_struct dtJalaliCheck = gregorian_to_jalali_core(*GYear, *GMonth, *GDay);

	ASSERT(dtJalaliCheck.year == JYear);
	ASSERT(dtJalaliCheck.month == JMonth);
	ASSERT(dtJalaliCheck.day == JDay);
#endif
}

