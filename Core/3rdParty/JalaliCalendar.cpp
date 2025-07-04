// Original code (c) Ali Tavakol, CodeProject, Sep 24, 2007
// largely replaced with online code

#include "StdAfx.h"
#include "JalaliCalendar.h"

////////////////////////////////////////////////////////////////////

// Day of week names in Persian script
TCHAR DOWNAME_1[] = { 0x0634, 0x0646, 0x0628, 0x0647, 0x0009, 0x0009, 0x0 };								// sanbe
TCHAR DOWNAME_2[] = { 0x06CC, 0x06A9, 0x200C, 0x0634, 0x0646, 0x0628, 0x0647, 0x0009, 0x0 };				// yeksanbe
TCHAR DOWNAME_3[] = { 0x062F, 0x0648, 0x200C, 0x0634, 0x0646, 0x0628, 0x0647, 0x0009, 0x0 };				// dosanbe
TCHAR DOWNAME_4[] = { 0x0633, 0x0647, 0x200C, 0x0634, 0x0646, 0x0628, 0x0647, 0x0009, 0x0 };				// sesanbe
TCHAR DOWNAME_5[] = { 0x0686, 0x0647, 0x0627, 0x0631, 0x200C, 0x0634, 0x0646, 0x0628, 0x0647, 0x0009, 0x0 }; // čaharsanbe
TCHAR DOWNAME_6[] = { 0x067E, 0x0646, 0x062C, 0x200C, 0x0634, 0x0646, 0x0628, 0x0647, 0x0009, 0x0 };		// panjsanbe
TCHAR DOWNAME_7[] = { 0x062C, 0x0645, 0x0639, 0x0647, 0x0009, 0x0009, 0x0 };								// Jom-e

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
	// One of a number of ways apparently
	// From https://github.com/MenoData/Time4J/issues/631

	return (((((((JYear - ((JYear > 0) ? 474 : 473)) % 2820) + 474) + 38) * 682) % 2816) < 682);
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

CString CJalaliCalendar::GetDayOfWeekName(int JDayOfWeek)
{
	switch (JDayOfWeek)
	{
	case 1:  return DOWNAME_1; // sanbe
	case 2:  return DOWNAME_2; // yeksanbe
	case 3:  return DOWNAME_3; // dosanbe
	case 4:  return DOWNAME_4; // sesanbe
	case 5:  return DOWNAME_5; // čaharsanbe
	case 6:  return DOWNAME_6; // panjsanbe
	case 7:  return DOWNAME_7; // Jom-e
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

void CJalaliCalendar::ToGregorian(int JYear, int JMonth, int JDay, COleDateTime& dtGregorian)
{
	int GYear, GMonth, GDay;
	ToGregorian(JYear, JMonth, JDay, &GYear, &GMonth, &GDay);

	dtGregorian.SetDate(GYear, GMonth, GDay);
}

void CJalaliCalendar::GetCurrentDate(int *JYear, int *JMonth, int *JDay)
{
	SYSTEMTIME st;

	GetLocalTime(&st);
	FromGregorian(st.wYear, st.wMonth, st.wDay, JYear, JMonth, JDay);
}

void CJalaliCalendar::FromGregorian(int GYear, int GMonth, int GDay, int *JYear, int *JMonth, int *JDay)
{
	/**
	*
	* @Name : ConvertCalendar.c
	* @Version : 1.0
	* @Programmer : Max
	* @Date : 2019-03-31
	* @Released under : https://github.com/BaseMax/ConvertCalendar/blob/master/LICENSE
	* @Repository : https://github.com/BaseMax/ConvertCalendar
	*
	**/

	const int GMONTHOFFSETS[12] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

	const int iMonth = (GMonth - 1);

	if (GYear <= 1600)
	{
		GYear -= 621;
		*JYear = 0;
	}
	else
	{
		GYear -= 1600;
		*JYear = 979;
	}

	int temp = ((GYear > 2) ? (GYear + 1) : GYear);
	int days = (((int)((temp + 3) / 4)) +
				(365 * GYear) -
				((int)((temp + 99) / 100)) -
				80 +
				GMONTHOFFSETS[iMonth] +
				((int)((temp + 399) / 400)) +
				GDay);

	*JYear += 33 * ((int)(days / 12053));
	days %= 12053;

	*JYear += 4 * ((int)(days / 1461));
	days %= 1461;

	if (days > 365)
	{
		*JYear += (int)((days - 1) / 365);
		days = (days - 1) % 365;
	}

	*JMonth = ((days < 186) ? 1 + (int)(days / 31) : 7 + (int)((days - 186) / 30));
	*JDay = (1 + ((days < 186) ? (days % 31) : ((days - 186) % 30)));
}

void CJalaliCalendar::ToGregorian(int JYear, int JMonth, int JDay, int *GYear, int *GMonth, int *GDay)
{
	/**
	*
	* @Name : ConvertCalendar.c
	* @Version : 1.0
	* @Programmer : Max
	* @Date : 2019-03-31
	* @Released under : https://github.com/BaseMax/ConvertCalendar/blob/master/LICENSE
	* @Repository : https://github.com/BaseMax/ConvertCalendar
	*
	**/

	if (JYear <= 979)
	{
		*GYear = 621;
	}
	else
	{
		JYear -= 979;
		*GYear = 1600;
	}

	int temp = (((int)(JYear / 33)) * 8) + ((int)(((JYear % 33) + 3) / 4)) + (365 * JYear) + 78 + JDay + ((JMonth < 7) ? (JMonth - 1) * 31 : ((JMonth - 7) * 30) + 186);

	*GYear += 400 * ((int)(temp / 146097));
	temp %= 146097;

	if (temp > 36524)
	{
		*GYear += 100 * ((int)(--temp / 36524));
		temp %= 36524;

		if (temp >= 365)
			temp++;
	}

	*GYear += 4 * ((int)(temp / 1461));
	temp %= 1461;

	if (temp > 365)
	{
		*GYear += (int)((temp - 1) / 365);
		temp = (temp - 1) % 365;
	}

	*GDay = temp + 1; // 1 -> 31

	bool leapYear = ((*GYear % 4 == 0 && *GYear % 100 != 0) || (*GYear % 400 == 0));
	int DAYSINMONTH[12] = { 31, (leapYear ? 29 : 28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	for (int index = 0; index < 12; index++)
	{
		*GMonth = (index + 1); // 1 -> 12

		if (*GDay <= DAYSINMONTH[index])
			break;

		*GDay -= DAYSINMONTH[index];
	}
}

/*
// These original functions were found to occasionally 
// convert consecutive days to the same date

void CJalaliCalendar::GregorianToJalali(int GYear, int GMonth, int GDay, int *JYear, int *JMonth, int *JDay, int *JDayOfWeek)
{
	int TotalDays;

	TotalDays = GetGregorianOffset(GYear, GMonth, GDay);
	
	if(JDayOfWeek)
		*JDayOfWeek = (TotalDays + 5) % 7;

	*JYear = TotalDays / (7 * 1461 + 4 * 365 + 366);
	if((*JYear) * (7 * 1461 + 4 * 365 + 366) == TotalDays)
		(*JYear)--;
	*JDay = TotalDays - (*JYear) * (7 * 1461 + 4 * 365 + 366);
	(*JYear) *= 33;

	(*JYear) += 1337;

	if((*JDay) > 366)
	{
		(*JYear)++;
		(*JDay) -= 366;
	}
	if((*JDay) > 365)
	{
		(*JYear)++;
		(*JDay) -= 365;
	}
	if((*JDay) > 365)
	{
		(*JYear)++;
		(*JDay) -= 365;
	}
	if((*JDay) > 365)
	{
		(*JYear)++;
		(*JDay) -= 365;
	}
	if((*JDay) > 365)
	{
		(*JYear)++;
		(*JDay) -= 365;
	}

	while((*JDay) > 366)
	{
		(*JYear)++;
		(*JDay) -= 366;

		if((*JDay) > 365)
		{
			(*JYear)++;
			(*JDay) -= 365;
		}
		if((*JDay) > 365)
		{
			(*JYear)++;
			(*JDay) -= 365;
		}
		if((*JDay) > 365)
		{
			(*JYear)++;
			(*JDay) -= 365;
		}
	}

	if((*JDay) < 187)
	{
		(*JMonth) = (*JDay) / 31;
		(*JDay) -= (*JMonth) * 31;
		if(!(*JDay))
		{
			(*JMonth)--;
			(*JDay) = 31;
		}
	}
	else
	{
		(*JDay) -= 186;
		(*JMonth) = (*JDay) / 30;
		(*JDay) -= (*JMonth) * 30;
		if(!(*JDay))
		{
			(*JMonth)--;
			(*JDay) = 30;
		}
		(*JMonth) += 6;
	}

	(*JMonth)++;
}

void CJalaliCalendar::JalaliToGregorian(int JYear, int JMonth, int JDay, int *GYear, int *GMonth, int *GDay, int *GDayOfWeek)
{
	int TotalDays;

	TotalDays = GetJalaliOffset(JYear, JMonth, JDay);

	if(GDayOfWeek)
		*GDayOfWeek = (TotalDays) % 7;

	*GYear = TotalDays / 1461;
	if((*GYear) * 1461 == TotalDays)
		(*GYear)--;
	*GDay = TotalDays - (*GYear) * 1461;
	(*GYear) *= 4;

	(*GYear)++;
	if((*GDay) > 365)
	{
		(*GYear)++;
		(*GDay) -= 365;
	}
	if((*GDay) > 365)
	{
		(*GYear)++;
		(*GDay) -= 365;
	}
	if((*GDay) > 365)
	{
		(*GYear)++;
		(*GDay) -= 365;
	}

	*GMonth = 1;
	if((*GDay) > 31)
	{
		(*GMonth)++;
		(*GDay) -= 31;
		if((*GDay) > (((*GYear) % 4)? 28 : 29))
		{
			(*GMonth)++;
			(*GDay) -= (((*GYear) % 4)? 28 : 29);
		}
	}
	if((*GDay) > 31)
	{
		(*GMonth)++;
		(*GDay) -= 31;
		if((*GDay) > 30)
		{
			(*GMonth)++;
			(*GDay) -= 30;
		}
	}
	if((*GDay) > 31)
	{
		(*GMonth)++;
		(*GDay) -= 31;
		if((*GDay) > 30)
		{
			(*GMonth)++;
			(*GDay) -= 30;
		}
	}
	if((*GDay) > 31)
	{
		(*GMonth)++;
		(*GDay) -= 31;
	}
	if((*GDay) > 31)
	{
		(*GMonth)++;
		(*GDay) -= 31;
		if((*GDay) > 30)
		{
			(*GMonth)++;
			(*GDay) -= 30;
		}
	}
	if((*GDay) > 31)
	{
		(*GMonth)++;
		(*GDay) -= 31;
		if((*GDay) > 30)
		{
			(*GMonth)++;
			(*GDay) -= 30;
		}
	}
	if((*GDay) > 31)
	{
		(*GMonth)++;
		(*GDay) -= 31;
	}
}
*/

