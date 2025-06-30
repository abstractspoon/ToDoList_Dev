// Original code (c) Ali Tavakol, CodeProject, Sep 24, 2007

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

void CJalaliCalendar::GregorianToJalali(const COleDateTime& dtGregorian, COleDateTime& dtJalali, int *JDayOfWeek)
{
	SYSTEMTIME stGregorian = { 0 };
	dtGregorian.GetAsSystemTime(stGregorian);

	int JYear, JMonth, JDay;
	GregorianToJalali((int)stGregorian.wYear, (int)stGregorian.wMonth, (int)stGregorian.wDay, &JYear, &JMonth, &JDay, JDayOfWeek);

	SYSTEMTIME stJalali = stGregorian;
	stJalali.wYear = (WORD)JYear;
	stJalali.wMonth = (WORD)JMonth;
	stJalali.wDay = (WORD)JDay;

	dtJalali = stJalali;
}

void CJalaliCalendar::JalaliToGregorian(const COleDateTime& dtJalali, COleDateTime& dtGregorian, int *GDayOfWeek)
{
	SYSTEMTIME stJalali = { 0 };
	dtJalali.GetAsSystemTime(stJalali);

	int GYear, GMonth, GDay;
	JalaliToGregorian((int)stJalali.wYear, (int)stJalali.wMonth, (int)stJalali.wDay, &GYear, &GMonth, &GDay, GDayOfWeek);

	SYSTEMTIME stGregorian = stJalali;
	stGregorian.wYear = (WORD)GYear;
	stGregorian.wMonth = (WORD)GMonth;
	stGregorian.wDay = (WORD)GDay;

	dtGregorian = stGregorian;
}

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

void CJalaliCalendar::GetCurrentDate(int *JYear, int *JMonth, int *JDay, int *JDayOfWeek)
{
	SYSTEMTIME st;

	GetLocalTime(&st);
	GregorianToJalali(st.wYear, st.wMonth, st.wDay, JYear, JMonth, JDay, JDayOfWeek);
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

void CJalaliCalendar::GetJalaliDayOfWeek(int JYear, int JMonth, int JDay, int *DayOfWeek)
{
	int TotalDays;

	TotalDays = GetJalaliOffset(JYear, JMonth, JDay);

	if(DayOfWeek)
		*DayOfWeek = (TotalDays) % 7;
}

void CJalaliCalendar::GetGregorianDayOfWeek(int GYear, int GMonth, int GDay, int *DayOfWeek)
{
	int TotalDays;

	TotalDays = GetGregorianOffset(GYear, GMonth, GDay);

	if(DayOfWeek)
		*DayOfWeek = (TotalDays + 5) % 7;
}

int CJalaliCalendar::GetGregorianOffset(int GYear, int GMonth, int GDay)
{
	int TotalDays;
	int i, j;

	i = GYear / 4;
	if(i * 4 == GYear)
		i--;
	TotalDays = i * 1461;
	i *= 4;

	if(GYear > i + 1)
	{
		i++;
		TotalDays += 365;
	}
	if(GYear > i + 1)
	{
		i++;
		TotalDays += 365;
	}
	if(GYear > i + 1)
		TotalDays += 365;

	i = (GYear - 2000) / 100 - (GYear - 2000) / 400;
	TotalDays -= i;

	switch(GMonth)
	{
	case 1:
		i = 0;
		break;
	case 2:
	case 3:
		i = 1;
		break;
	case 4:
	case 5:
		i = 2;
		break;
	case 6:
	case 7:
		i = 3;
		break;
	case 8:
		i = 4;
		break;
	case 9:
	case 10:
		i = 5;
		break;
	case 11:
	case 12:
		i = 6;
	}

	switch(GMonth)
	{
	case 1:
	case 2:
	case 3:
	case 4:
		j = 0;
		break;
	case 5:
	case 6:
		j = 1;
		break;
	case 7:
	case 8:
	case 9:
		j = 2;
		break;
	case 10:
	case 11:
		j = 3;
		break;
	case 12:
		j = 4;
	}

	TotalDays += GDay + i * 31 + j * 30;

	if(GMonth > 2)
		if(GYear % 4)
			TotalDays += 28;
		else
			TotalDays += 29;

	// Date should be greater than 1337/1/1
//	ASSERT(TotalDays > (726926 - (7 * 1461 + 4 * 365 + 366)));
	TotalDays -= (726926 - (7 * 1461 + 4 * 365 + 366));

	return TotalDays;
}

int CJalaliCalendar::GetJalaliOffset(int JYear, int JMonth, int JDay)
{
	int TotalDays;
	int i, j;
	
	// Date should be greater than 1337/1/1
//	ASSERT(JYear > 1337);
	TotalDays = 726926 - (7 * 1461 + 4 * 365 + 366);

	i = (JYear - 1337) / 33;
	TotalDays += (7 * 1461 + 4 * 365 + 366) * i;

	i = i * 33 + 1336;

	if(JYear > i + 1)
	{
		i++;
		TotalDays += 366;
	}
	if(JYear > i + 1)
	{
		i++;
		TotalDays += 365;
	}
	if(JYear > i + 1)
	{
		i++;
		TotalDays += 365;
	}
	if(JYear > i + 1)
	{
		i++;
		TotalDays += 365;
	}
	if(JYear > i + 1)
	{
		i++;
		TotalDays += 365;
	}

	while(JYear > i + 1)
	{
		if(JYear > i + 1)
		{
			i++;
			TotalDays += 366;
		}
		if(JYear > i + 1)
		{
			i++;
			TotalDays += 365;
		}
		if(JYear > i + 1)
		{
			i++;
			TotalDays += 365;
		}
		if(JYear > i + 1)
		{
			i++;
			TotalDays += 365;
		}
	}

	i = min(JMonth - 1, 6);
	j = max(JMonth - 7, 0);
	TotalDays += JDay + i * 31 + j * 30;

	return TotalDays;
}

int CJalaliCalendar::GetGregorianDayDifference(int GYear1, int GMonth1, int GDay1, int GYear2, int GMonth2, int GDay2)
{
	int Offset1, Offset2;

	Offset1 = GetGregorianOffset(GYear1, GMonth1, GDay1);
	Offset2 = GetGregorianOffset(GYear2, GMonth2, GDay2);

	return Offset2 - Offset1;
}

int CJalaliCalendar::GetJalaliDayDifference(int GYear1, int GMonth1, int GDay1, int GYear2, int GMonth2, int GDay2)
{
	int Offset1, Offset2;

	Offset1 = GetJalaliOffset(GYear1, GMonth1, GDay1);
	Offset2 = GetJalaliOffset(GYear2, GMonth2, GDay2);

	return Offset2 - Offset1;
}

void CJalaliCalendar::GetJalaliDateWithOffset(int JYear, int JMonth, int JDay, int Offset, int *Year, int *Month, int *Day, int *DayOfWeek)
{
	int TotalDays;

	TotalDays = GetJalaliOffset(JYear, JMonth, JDay) + Offset - 726926 + (7 * 1461 + 4 * 365 + 366);

	if(DayOfWeek)
		*DayOfWeek = (TotalDays + 5) % 7;

	*Year = TotalDays / (7 * 1461 + 4 * 365 + 366);
	if((*Year) * (7 * 1461 + 4 * 365 + 366) == TotalDays)
		(*Year)--;
	*Day = TotalDays - (*Year) * (7 * 1461 + 4 * 365 + 366);
	(*Year) *= 33;

	(*Year) += 1337;

	if((*Day) > 366)
	{
		(*Year)++;
		(*Day) -= 366;
	}
	if((*Day) > 365)
	{
		(*Year)++;
		(*Day) -= 365;
	}
	if((*Day) > 365)
	{
		(*Year)++;
		(*Day) -= 365;
	}
	if((*Day) > 365)
	{
		(*Year)++;
		(*Day) -= 365;
	}
	if((*Day) > 365)
	{
		(*Year)++;
		(*Day) -= 365;
	}

	while((*Day) > 366)
	{
		(*Year)++;
		(*Day) -= 366;

		if((*Day) > 365)
		{
			(*Year)++;
			(*Day) -= 365;
		}
		if((*Day) > 365)
		{
			(*Year)++;
			(*Day) -= 365;
		}
		if((*Day) > 365)
		{
			(*Year)++;
			(*Day) -= 365;
		}
	}

	if((*Day) < 187)
	{
		(*Month) = (*Day) / 31;
		(*Day) -= (*Month) * 31;
		if(!(*Day))
		{
			(*Month)--;
			(*Day) = 31;
		}
	}
	else
	{
		(*Day) -= 186;
		(*Month) = (*Day) / 30;
		(*Day) -= (*Month) * 30;
		if(!(*Day))
		{
			(*Month)--;
			(*Day) = 30;
		}
		(*Month) += 6;
	}

	(*Month)++;
}

void CJalaliCalendar::GetGregorianDateWithOffset(int GYear, int GMonth, int GDay, int Offset, int *Year, int *Month, int *Day, int *DayOfWeek /*, TCHAR *DayName , TCHAR *MonthName */)
{
	GregorianToJalali(GYear, GMonth, GDay, &GYear, &GMonth, &GDay);
	GetJalaliDateWithOffset(GYear, GMonth, GDay, Offset, &GYear, &GMonth, &GDay);
	JalaliToGregorian(GYear, GMonth, GDay, Year, Month, Day, DayOfWeek/*, DayName, MonthName*/);
}
