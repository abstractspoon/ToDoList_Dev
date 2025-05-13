
#include "stdafx.h"
#include "YearComboBoxEx.h"
#include "Win32.h"

#include <shared\DateHelper.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Diagnostics;
using namespace System::Drawing;
using namespace System::Windows::Forms;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

void YearComboBoxEx::WndProc(Message% m)
{
	YearComboBox::WndProc(m);

	switch (m.Msg)
	{
	case WM_CTLCOLORLISTBOX:
		if (!m_ListBoxHandled)
		{ 
			// Clear first to prevent re-entrancy
			m_ListBoxHandled = true;

			if (CDateHelper::WantRTLDates())
				Win32::SetRTLReading(m.LParam, true);
		}
		break;
	}
}

void YearComboBoxEx::BuildCombo()
{
	YearComboBox::BuildCombo();

	if (CDateHelper::WantRTLDates())
		Win32::SetRTLReading(Handle, true);
}

