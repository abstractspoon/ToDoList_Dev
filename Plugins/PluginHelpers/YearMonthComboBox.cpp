
#include "stdafx.h"
#include "YearMonthComboBox.h"
#include "Win32.h"
#include "DateUtil.h"

#include <shared\DateHelper.h>

//////////////////////////////////////////////////////////////////////////////

using namespace System::Diagnostics;
using namespace System::Drawing;
using namespace System::Windows::Forms;

using namespace Abstractspoon::Tdl::PluginHelpers;

//////////////////////////////////////////////////////////////////////////////

MonthComboBox::MonthComboBox()
{
}

void MonthComboBox::BuildCombo()
{
	DropDownStyle = ComboBoxStyle::DropDownList;

	for (int month = 1; month <= 12; month++)
		Items->Add(DateUtil::GetMonthName(month, false));

	SelectedMonth = DateUtil::GetMonth(DateTime::Now);
}

int MonthComboBox::SelectedMonth::get()
{
	return (SelectedIndex + 1);
}

void MonthComboBox::SelectedMonth::set(int month)
{
	if ((month >= 1) && (month <= 12))
	{
		if (Items->Count == 0)
			BuildCombo();

		SelectedIndex = (month - 1);
	}
}

//////////////////////////////////////////////////////////////////////////////

YearComboBox::YearComboBox() : m_ListBoxHandled(false)
{
	SelectedYear = DateUtil::GetYear(DateTime::Now);

	Win32::SetRTLReading(Handle, DateUtil::WantRTLDates());
}

void YearComboBox::WndProc(Message% m)
{
	ComboBox::WndProc(m);

	switch (m.Msg)
	{
	case WM_CTLCOLORLISTBOX:
		if (!m_ListBoxHandled)
		{ 
			// Clear first to prevent re-entrancy
			m_ListBoxHandled = true;

			Win32::SetRTLReading(m.LParam, DateUtil::WantRTLDates());
		}
		break;
	}
}

void YearComboBox::BuildCombo()
{
	Items->Clear();

	for (int year = m_MinYear; year <= m_MaxYear; year++)
	{
 		DateTime date = DateUtil::ToDate(year, 1, 1);
		Items->Add(DateUtil::FormatDateOnly(date, "yyyy"));
	}
}

int YearComboBox::SelectedYear::get()
{
	return (SelectedIndex + m_MinYear);
}

void YearComboBox::SelectedYear::set(int year)
{
	if ((year < m_MinYear) || (year > m_MaxYear))
	{
		m_MinYear = year - 10;
		m_MaxYear = year + 10;

		BuildCombo();
	}

	SelectedIndex = (year - m_MinYear);
}

//////////////////////////////////////////////////////////////////////////////
