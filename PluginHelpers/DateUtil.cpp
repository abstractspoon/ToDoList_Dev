// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DateUtil.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////


/// <summary>
/// Get the Week number of the year
/// (In the range 1..53)
/// This conforms to ISO 8601 specification for week number.
/// </summary>
/// <param name="date"></param>
/// <returns>Week of year</returns>
int DateUtil::WeekOfYear(DateTime^ date)
{
	const int moveByDays[] = { 6, 7, 8, 9, 10, 4, 5 };

	DateTime^ startOfYear = gcnew DateTime(date->Year, 1, 1);
	DateTime^ endOfYear = gcnew DateTime(date->Year, 12, 31);

	// ISO 8601 weeks start with Monday 
	// The first week of a year includes the first Thursday 
	// This means that Jan 1st could be in week 51, 52, or 53 of the previous year...
	int numberDays = date->Subtract(*startOfYear).Days + moveByDays[(int)startOfYear->DayOfWeek];
	int weekNumber = numberDays / 7;

	switch (weekNumber)
	{
	case 0:
		// Before start of first week of this year - in last week of previous year
		weekNumber = WeekOfYear(startOfYear->AddDays(-1));
		break;
	case 53:
		// In first week of next year.
		if (endOfYear->DayOfWeek < DayOfWeek::Thursday)
		{
			weekNumber = 1;
		}
		break;
	}

	return weekNumber;
}
