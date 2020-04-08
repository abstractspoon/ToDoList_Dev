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

void FormsUtil::RecalcDropWidth(ComboBox^ combo)
{
	int maxWidth = 0;
	int nItem = combo->Items->Count;

	while (nItem--)
	{
		auto item = ASTYPE(combo->Items[nItem], String);

		if (item == nullptr)
			return;

		int itemWidth = TextRenderer::MeasureText(item, combo->Font).Width;
		maxWidth = Math::Max(itemWidth, maxWidth);
	}

	combo->DropDownWidth = (maxWidth + SystemInformation::VerticalScrollBarWidth);
}

