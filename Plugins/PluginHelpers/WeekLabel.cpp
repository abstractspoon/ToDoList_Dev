
#include "stdafx.h"
#include "WeekLabel.h"
#include "DateUtil.h"
#include "Win32.h"

#include <shared\Clipboard.h>
#include <shared\Misc.h>
#include <shared\GraphicsMisc.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Diagnostics;
using namespace System::Drawing;
using namespace System::Windows::Forms;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

WeekLabel::WeekLabel(Translator^ trans)
	:
	m_Trans(trans),
	m_NumDays(1)
{
	Win32::SetRTLReading(Handle, DateUtil::WantRTLDates());
}

int WeekLabel::NumDays::get()
{
	return m_NumDays;
}

void WeekLabel::NumDays::set(int value)
{
	if (value != m_NumDays)
	{
		m_NumDays = value;
		StartDate = m_StartDate;
	}
}

void WeekLabel::StartDate::set(DateTime value)
{
	m_StartDate = value;
	DateTime endDate = m_StartDate.AddDays(m_NumDays - 1);

	int startYear, endYear, startMonth, endMonth, unused;
	DateUtil::FromDate(m_StartDate, startYear, startMonth, unused);
	DateUtil::FromDate(endDate, endYear, endMonth, unused);

	String^ MonthYearFormat = (DateUtil::WantRTLDates() ? "yyyy MMM" : "MMM yyyy");

	if (endYear == startYear)
	{
		if (endMonth == startMonth)
		{
			Text = DateUtil::FormatDateOnly(m_StartDate, MonthYearFormat);
		}
		else
		{
			Text = String::Format("{0} - {1} ",
								 DateUtil::GetMonthName(startMonth, true),
								 DateUtil::FormatDateOnly(endDate, MonthYearFormat));
		}
	}
	else
	{
		Text = DateUtil::FormatDateOnlyRange(m_StartDate, endDate, MonthYearFormat);
	}

	int startWeek = DateUtil::WeekOfYear(m_StartDate);
	int numWeeks = (NumDays / 7);

	if (numWeeks <= 1)
		Text = (Text + String::Format(m_Trans->Translate("(Week {0}) ", Translator::Type::Label), startWeek));
	else
		Text = (Text + String::Format(m_Trans->Translate("(Weeks {0}-{1}) ", Translator::Type::Label), startWeek, (startWeek + numWeeks - 1)));

	Invalidate();
}

void WeekLabel::OnPaint(System::Windows::Forms::PaintEventArgs^ pe)
{
    pe->Graphics->TextRenderingHint = Drawing::Text::TextRenderingHint::SystemDefault;
    Label::OnPaint(pe);
}
