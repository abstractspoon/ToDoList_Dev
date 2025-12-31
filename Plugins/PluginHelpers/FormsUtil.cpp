// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "FormsUtil.h"
#include "PluginHelpers.h"
#include "win32.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Windows::Forms;
using namespace System::Drawing;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

void FormsUtil::SetFont(Form^ form, Font^ font)
{
	form->Font = font;

	SetFont(form->Controls, font);
}

void FormsUtil::SetFont(Control^ ctrl, Font^ font)
{
	ctrl->Font = font;

	SetFont(ctrl->Controls, font);
}

void FormsUtil::SetFont(Control::ControlCollection^ ctrls, Font^ font)
{
	int nCtrl = ctrls->Count;

	while (nCtrl--)
		SetFont(ctrls[nCtrl], font); // RECURSIVE CALL
}

Control^ FormsUtil::Find(Control^ parent, String^ childName, bool recursive)
{
	auto ctrls = parent->Controls->Find(childName, recursive);

	return ((ctrls != nullptr) ? ctrls[0] : nullptr);
}

bool FormsUtil::SetEditCue(Control^ ctrl, String^ sCueText)
{
	if ((ctrl != nullptr) && (sCueText != nullptr))
		return Win32::SetEditCue(ctrl->Handle, sCueText);

	// else
	return false;
}

bool FormsUtil::SetEditCue(Control^ parent, String^ childName, String^ sCueText, bool recursiveSearch)
{
	return SetEditCue(Find(parent, childName, recursiveSearch), sCueText);
}

bool FormsUtil::SetComboBoxCue(Windows::Forms::ComboBox^ combo, String^ sCueText)
{
	if ((combo != nullptr) && (sCueText != nullptr))
		return Win32::SetComboBoxCue(combo->Handle, sCueText);

	// else
	return false;
}

bool FormsUtil::SetComboBoxCue(Windows::Forms::Control^ parent, String^ childName, String^ sCueText, bool recursiveSearch)
{
	auto combo = ASTYPE(Find(parent, childName, recursiveSearch), Windows::Forms::ComboBox);

	return SetComboBoxCue(combo, sCueText);
}

void FormsUtil::RecalcDropWidth(ComboBox^ combo)
{
	int maxWidth = CalcWidestItem(combo);

	combo->DropDownWidth = (maxWidth + SystemInformation::VerticalScrollBarWidth);
}

int FormsUtil::CalcWidestItem(Windows::Forms::ComboBox^ combo)
{
	int maxWidth = 0;
	int nItem = combo->Items->Count;

	while (nItem--)
	{
		auto item = combo->Items[nItem]->ToString();

		if (item != nullptr)
		{
			int itemWidth = TextRenderer::MeasureText(item, combo->Font).Width;
			maxWidth = Math::Max(itemWidth, maxWidth);
		}
	}

	return maxWidth;
}

void FormsUtil::FixThumbScrolling(Message% m)
{
	// When 'show window contents while dragging' is DISABLED
	// WinForms also disables 'live' scrollbar thumb dragging
	// and only performs the scroll when the tumb is released.
	//
	// This is contrary to how Windows itself operates, which
	// ignores this setting when scrolling all native controls.
	//
	// To restore native Windows behaviour for ScrollableControls,
	// so as to be consistent with the rest of the app we detect the
	// situation and then replace SB_THUMBTRACK with SB_THUMBPOSITION.
	switch (m.Msg)
	{
	case WM_HSCROLL:
	case WM_VSCROLL:
		if (!SystemInformation::DragFullWindows)
		{
			if (Win32::LoWord(m.WParam) == SB_THUMBTRACK)
			{
				// Must be a scrollable control
				if (ISTYPE(Control::FromHandle(m.HWnd), ScrollableControl))
					m.WParam = (IntPtr)Win32::MakeWParam(SB_THUMBPOSITION, Win32::HiWord(m.WParam));
			}
		}
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

void PanelEx::WndProc(Message% m)
{
	FormsUtil::FixThumbScrolling(m);

	Panel::WndProc(m);
}

////////////////////////////////////////////////////////////////////////////////////////////////

