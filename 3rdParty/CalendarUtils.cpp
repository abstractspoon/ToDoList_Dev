//Written for the ToDoList (http://www.codeproject.com/KB/applications/todolist2.aspx)
//Design and latest version - http://www.codeproject.com/KB/applications/TDL_Calendar.aspx
//by demon.code.monkey@googlemail.com

#include "stdafx.h"
#include "..\..\Shared\DateHelper.h"
#include "CalendarUtils.h"

#define DATE_RANGE_YEAR_MIN	100
#define DATE_RANGE_YEAR_MAX	9999


CString CCalendarUtils::LongToString(long lValue)
{
	char szValue[20];
	_ltoa(lValue, szValue, 10);
	return szValue;
}

time_t CCalendarUtils::DateToTime(const COleDateTime& _dt)
{
	tm time = { 0,
				0,
				0,
				_dt.GetDay(),
				_dt.GetMonth() - 1,
				_dt.GetYear() - 1900,
				_dt.GetDayOfWeek() - 1,
				_dt.GetDayOfYear(),
				-1 };

	time_t timet = mktime(&time);

	if (timet != -1)
		return timet;
	else
		return 0;
}

#ifdef _DEBUG
//
CString CCalendarUtils::DateToString(const COleDateTime& _dt)
{
	CString strReturn;

	if (IsDateValid(_dt))
	{
		int nYear = _dt.GetYear();
		int nMonth = _dt.GetMonth();
		int nDay = _dt.GetDay();
		strReturn.Format(_T("%d/%d/%d"), nDay, nMonth, nYear);
	}
	else
	{
		ASSERT(FALSE);
	}

	return strReturn;
}
//
#endif

//returns -1 if _dt1 < _dt2      returns 0 if _dt1 == _dt2      returns 1 if _dt1 > _dt2   
int CCalendarUtils::CompareDates(const COleDateTime& _dt1,
								 const COleDateTime& _dt2)
{
	int nReturn = 0;

	//check years
	int nYear1 = _dt1.GetYear();
	int nYear2 = _dt2.GetYear();
	if (nYear1 < nYear2)
	{
		nReturn = -1;
	}
	else if (nYear1 > nYear2)
	{
		nReturn = 1;
	}
	else
	{
		//years match, check months
		int nMonth1 = _dt1.GetMonth();
		int nMonth2 = _dt2.GetMonth();
		if (nMonth1 < nMonth2)
		{
			nReturn = -1;
		}
		else if (nMonth1 > nMonth2)
		{
			nReturn = 1;
		}
		else
		{
			//months match, check days
			int nDay1 = _dt1.GetDay();
			int nDay2 = _dt2.GetDay();
			if (nDay1 < nDay2)
			{
				nReturn = -1;
			}
			else if (nDay1 > nDay2)
			{
				nReturn = 1;
			}
			else
			{
				//date matches
			}
		}
	}

	return nReturn;
}

BOOL CCalendarUtils::IsDateValid(const COleDateTime& _dt)
{
	if ((_dt.GetYear() > DATE_RANGE_YEAR_MIN) &&
		(_dt.GetYear() < DATE_RANGE_YEAR_MAX))
	{
		return TRUE;
	}
	return FALSE;
}

int CCalendarUtils::GetDaysInMonth(int _iMonth, int _iYear)
{
	int nDays = -1;
	switch (_iMonth)
	{
		case 1:		//jan
		case 3:		//mar
		case 5:		//may
		case 7:		//jul
		case 8:		//aug
		case 10:	//oct
		case 12:	//dec
		{
			nDays = 31;
			break;
		}
		case 4:		//apr
		case 9:		//sep
		case 6:		//jun
		case 11:	//nov
		{
			nDays = 30;
			break;
		}
		case 2:		//feb
		{
			if (_iYear%4 == 0)
			{
				nDays = 29;
			}
			else
			{
				nDays = 28;
			}
			break;
		}
		default:
		{
			ASSERT(FALSE);
			break;
		}
	}
	return nDays;
}

void CCalendarUtils::AddDay(COleDateTime& _dt, int _nNumDays/*=1*/)
{
	if (_nNumDays == 0)
	{
		return;
	}

	if (IsDateValid(_dt))
	{
		COleDateTime dtLocal(_dt);
		dtLocal += COleDateTimeSpan(_nNumDays, 0, 0, 0);

		if (IsDateValid(dtLocal))
		{
			_dt = dtLocal;
		}
	}
}

void CCalendarUtils::SubtractDay(COleDateTime& _dt, int _nNumDays/*=1*/)
{
	if (_nNumDays == 0)
	{
		return;
	}

	if (IsDateValid(_dt))
	{
		COleDateTime dtLocal(_dt);
		dtLocal -= COleDateTimeSpan(_nNumDays, 0, 0, 0);

		if (IsDateValid(dtLocal))
		{
			_dt = dtLocal;
		}
	}
}

void CCalendarUtils::AddMonth(COleDateTime& _dt, int _nNumMonths/*=1*/)
{
	if (_nNumMonths == 0)
	{
		return;
	}

	if (IsDateValid(_dt))
	{
		int nYear = _dt.GetYear();
		int nMonth = _dt.GetMonth();
		int nDay = _dt.GetDay();

		for (int nLoop = 0; nLoop < _nNumMonths; nLoop++)
		{
			//increase month, and year if rolled over
			nMonth++;
			if (nMonth > 12)
			{
				nMonth = 1;
				nYear++;
			}

			//adjust day if isn't valid in that month
			int nDaysInNewMonth = GetDaysInMonth(nMonth, nYear);
			if (nDay > nDaysInNewMonth)
			{
				nDay = nDaysInNewMonth;
			}
		}

		COleDateTime dtLocal(nYear, nMonth, nDay, 0, 0, 0);
		if (IsDateValid(dtLocal))
		{
			_dt = dtLocal;
		}
	}
}

void CCalendarUtils::SubtractMonth(COleDateTime& _dt, int _nNumMonths/*=1*/)
{
	if (_nNumMonths == 0)
	{
		return;
	}

	if (IsDateValid(_dt))
	{
		int nYear = _dt.GetYear();
		int nMonth = _dt.GetMonth();
		int nDay = _dt.GetDay();

		for (int nLoop = 0; nLoop < _nNumMonths; nLoop++)
		{
			//decrease month, and year if rolled over
			nMonth--;
			if (nMonth < 1)
			{
				nMonth = 12;
				nYear--;
			}

			//adjust day if isn't valid in that month
			int nDaysInNewMonth = GetDaysInMonth(nMonth, nYear);
			if (nDay > nDaysInNewMonth)
			{
				nDay = nDaysInNewMonth;
			}
		}

		COleDateTime dtLocal(nYear, nMonth, nDay, 0, 0, 0);
		if (IsDateValid(dtLocal))
		{
			_dt = dtLocal;
		}
	}
}

//sets time to midnight
void CCalendarUtils::GetDateOnly(COleDateTime& _dt)
{
	_dt.SetDate(_dt.GetYear(), _dt.GetMonth(), _dt.GetDay());
}

//gets today's date (midnight)
void CCalendarUtils::GetToday(COleDateTime& _dt)
{
	_dt = COleDateTime::GetCurrentTime();
	GetDateOnly(_dt);
}

CString CCalendarUtils::GetWeekday(int _iDayOfWeek,
								   BOOL _bShort)
{
	CString strReturn;
	if ((_iDayOfWeek >= 0) && (_iDayOfWeek < 7))
	{
		//_iDayOfWeek is zero-based, MFC days of week are 1-based
		int nDay = _iDayOfWeek + 1;

		int iFirstDayOfWeek = CDateHelper::GetFirstDayOfWeek();

		//_iDayOfWeek of 0 must map to iFirstDayOfWeek (which is also 1-based)
		nDay += iFirstDayOfWeek-1;

		//avoid overflow
		if (nDay > 7)
		{
			nDay = 1;
		}

		strReturn = CDateHelper::GetWeekdayName(nDay, _bShort);
	}
	else
	{
		ASSERT(FALSE);
	}
	return strReturn;
}

void CCalendarUtils::GetWeekdays(CStringList& _listDays,
								 BOOL _bShort,
								 BOOL _bWeekendsHidden)
{
	_listDays.RemoveAll();

	int nCurrentDay = CDateHelper::GetFirstDayOfWeek();	//start with the first day of the week

	for (int i = 0; i < 7; i++)
	{
		if (nCurrentDay > 7)
		{
			nCurrentDay = 1;
		}

		if (_bWeekendsHidden && ((nCurrentDay == 1) || (nCurrentDay == 7)))
		{
			//skip - this day is a weekend-day, and weekends are hidden
		}
		else
		{
			CString strDay = CDateHelper::GetWeekdayName(nCurrentDay, _bShort);
			_listDays.AddTail(strDay);
		}

		nCurrentDay++;
	}
}
