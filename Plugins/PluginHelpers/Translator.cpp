// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "FormsUtil.h"
#include "Translator.h"

#include <Interfaces\ITransText.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

Translator::Translator(ITransText* pTransText) : m_pTransText(pTransText) 
{
} 

// private constructor
Translator::Translator() : m_pTransText(nullptr)
{

}

String^ Translator::Translate(String^ sText)
{
	if (String::IsNullOrWhiteSpace(sText))
		return String::Empty;

	// Allow pre-translate to operate regardless of
	// the embedded translator pointer
	if (m_mapPreTranslate != nullptr)
	{
		String^ sPreTrans;

		if (m_mapPreTranslate->TryGetValue(sText, sPreTrans))
			sText = sPreTrans;
	}

	if (m_pTransText == nullptr)
		return sText;

	LPWSTR szTemp = NULL;

	if (!m_pTransText->TranslateText(MS(sText), szTemp))
		return sText;

	String^ sTextOut = gcnew String(szTemp);
	m_pTransText->FreeTextBuffer(szTemp);

	return sTextOut;
}

void Translator::Translate(Form^ window)
{
	// Window title
	window->Text = Translate(window->Text);

	// children
	Translate(window->Controls);
}

void Translator::Translate(Form^ window, ToolTip^ tooltips)
{
	Translate(window);

	if (tooltips != nullptr)
	{
		int nItem = window->Controls->Count;

		while (nItem--)
		{
			auto ctrl = window->Controls[nItem];
			auto toolText = tooltips->GetToolTip(ctrl);

			if (!String::IsNullOrEmpty(toolText))
				tooltips->SetToolTip(ctrl, Translate(toolText));
		}
	}
}

void Translator::Translate(ToolStripItemCollection^ items)
{
	int nItem = items->Count;

	while (nItem--)
	{
		auto item = items[nItem];

		item->Text = Translate(item->Text);
		item->ToolTipText = Translate(item->ToolTipText);

		// children
		auto menu = ASTYPE(item, ToolStripMenuItem);

		if ((menu != nullptr) && menu->HasDropDownItems)
		{
			Translate(menu->DropDownItems); // RECURSIVE CALL
		}
	}
}

void Translator::Translate(Control::ControlCollection^ items)
{
	int nItem = items->Count;

	while (nItem--)
	{
		auto ctrl = items[nItem];

		// Special cases
		if (ISTYPE(ctrl, Windows::Forms::WebBrowser) ||
			ISTYPE(ctrl, TextBox) ||
			ISTYPE(ctrl, RichTextBox))
		{
			continue;
		}

		if (ISTYPE(ctrl, ToolStrip))
		{
			Translate(ASTYPE(ctrl, ToolStrip)->Items);
		}
		else if (ISTYPE(ctrl, ComboBox))
		{
			Translate(ASTYPE(ctrl, ComboBox));
		}
		else
		{
			ctrl->Text = Translate(ctrl->Text);

			// children
			Translate(ctrl->Controls);
		}
	}
}

void Translator::Translate(ComboBox^ combo)
{
	// Don't translate dynamic content
	if (combo->DropDownStyle != ComboBoxStyle::DropDownList)
		return;

	int nItem = combo->Items->Count;

	while (nItem--)
	{
		auto item = ASTYPE(combo->Items[nItem], String);

		if (item == nullptr)
			return;

		combo->Items[nItem] = Translate(item);
	}

	combo->Text = Translate(combo->Text);
	
	FormsUtil::RecalcDropWidth(combo);
}

void Translator::AddPreTranslation(String^ sText, String^ sTranslation)
{
	if (m_mapPreTranslate == nullptr)
		m_mapPreTranslate = gcnew Dictionary<String^, String^>();

	m_mapPreTranslate[sText] = sTranslation;
}

////////////////////////////////////////////////////////////////////////////////////////////////

