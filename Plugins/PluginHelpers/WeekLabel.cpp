
#include "stdafx.h"
#include "WeekLabel.h"
#include "DateUtil.h"

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
	DateTime endDate = m_StartDate.AddDays(m_NumDays).AddSeconds(-1);

	if (endDate.Year == m_StartDate.Year)
	{
		if (endDate.Month == m_StartDate.Month)
		{
			Text = m_StartDate.ToString("MMM yyyy ");
		}
		else
		{
			Text = String::Format("{0} - {1} ",
								 m_StartDate.ToString("MMM"),
								 endDate.ToString("MMM yyyy"));
		}
	}
	else
	{
		Text = String::Format("{0} - {1} ",
							 m_StartDate.ToString("MMM yyyy"),
							 endDate.ToString("MMM yyyy"),
							 m_StartDate.Year);
	}

	int startWeek = DateUtil::WeekOfYear(m_StartDate);
	int numWeeks = (NumDays / 7);

	if (numWeeks <= 1)
		Text = (Text + String::Format(m_Trans->Translate("(Week {0})", Translator::Type::Label), startWeek));
	else
		Text = (Text + String::Format(m_Trans->Translate("(Weeks {0}-{1})", Translator::Type::Label), startWeek, (startWeek + numWeeks - 1)));

	Invalidate();
}

void WeekLabel::OnPaint(System::Windows::Forms::PaintEventArgs^ pe)
{
    pe->Graphics->TextRenderingHint = Drawing::Text::TextRenderingHint::SystemDefault;
    Label::OnPaint(pe);
}
