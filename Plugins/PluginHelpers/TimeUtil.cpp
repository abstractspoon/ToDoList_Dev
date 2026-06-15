// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "TimeUtil.h"
#include "DateUtil.h"
#include "PluginHelpers.h"

#include <Shared\TimeHelper.h>
#include <Shared\Misc.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Diagnostics;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

double TimeUtil::Convert(double amount, Task::TimeUnits fromUnits, Task::TimeUnits toUnits, WorkingWeek^ week)
{
	if (toUnits == fromUnits)
		return amount;

	CWorkingWeek ww(DateUtil::MapDaysOfWeek(week->WeekendDays()),
					week->WorkDay()->DayLengthInHours(false),
					week->WorkDay()->StartOfDayInHours(),
					week->WorkDay()->StartOfLunchInHours(),
					week->WorkDay()->EndOfDayInHours());

	auto nFromUnits = (TH_UNITS)MapToTHUnits(fromUnits);
	auto nToUnits = (TH_UNITS)MapToTHUnits(toUnits);

	return CTimeHelper(ww).Convert(amount, nFromUnits, nToUnits);
}

int TimeUtil::MapToTHUnits(Task::TimeUnits units)
{
	switch (units)
	{
	case Task::TimeUnits::Minutes:	return THU_MINS;
	case Task::TimeUnits::Hours:	return THU_HOURS;
	case Task::TimeUnits::Days:		return THU_DAYS;
	case Task::TimeUnits::Weekdays:	return THU_WEEKDAYS;
	case Task::TimeUnits::Weeks:	return THU_WEEKS;
	case Task::TimeUnits::Months:	return THU_MONTHS;
	case Task::TimeUnits::Years:	return THU_YEARS;
	}

	Debug::Assert(false);
	return THU_NULL;
}

