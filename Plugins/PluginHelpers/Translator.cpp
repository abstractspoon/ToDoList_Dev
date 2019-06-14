// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "Translator.h"

#include <Interfaces\ITransText.h>

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
	if (m_pTransText == nullptr)
		return sText;

	if (m_mapPreTranslate != nullptr)
	{
		String^ sPreTrans;

		if (m_mapPreTranslate->TryGetValue(sText, sPreTrans))
			sText = sPreTrans;
	}

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

void Translator::Translate(ToolStripItemCollection^ items)
{
	int nItem = items->Count;

	while (nItem--)
	{
		auto tsi = items[nItem];

		tsi->Text = Translate(tsi->Text);
		tsi->ToolTipText = Translate(tsi->ToolTipText);

		// children
		auto tsmi = dynamic_cast<ToolStripMenuItem^>(tsi);

		if ((tsmi != nullptr) && tsmi->HasDropDownItems)
		{
			Translate(tsmi->DropDownItems); // RECURSIVE CALL
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
		if ((dynamic_cast<WebBrowser^>(ctrl) != nullptr) ||
			(dynamic_cast<TextBox^>(ctrl) != nullptr) ||
			(dynamic_cast<RichTextBox^>(ctrl) != nullptr))
		{
			continue;
		}

		if (dynamic_cast<ToolStrip^>(ctrl) != nullptr)
		{
			Translate(dynamic_cast<ToolStrip^>(ctrl)->Items);
		}
		else
		{
			ctrl->Text = Translate(ctrl->Text);

			// children
			Translate(ctrl->Controls);
		}
	}
}

void Translator::AddPreTranslate(String^ sText, String^ sTranslation)
{
	if (m_mapPreTranslate == nullptr)
		m_mapPreTranslate = gcnew Dictionary<String^, String^>();

	m_mapPreTranslate->Add(sText, sTranslation);
}

////////////////////////////////////////////////////////////////////////////////////////////////

