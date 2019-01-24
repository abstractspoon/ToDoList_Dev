// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "Translator.h"

#include "..\..\ToDoList_Dev\Interfaces\ITransText.h"

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

	LPWSTR szTemp = NULL;

	if (!m_pTransText->TranslateText(MS(sText), szTemp))
		return sText;

	String^ sTextOut = gcnew String(szTemp);
	m_pTransText->FreeTextBuffer(szTemp);

	return sTextOut;
}

void Translator::Translate(System::Windows::Forms::Form^ window)
{
	// Window title
	window->Text = Translate(window->Text);

	// children
	Translate(window->Controls);
}

void Translator::Translate(System::Windows::Forms::ToolStripItemCollection^ items)
{
	int nItem = items->Count;

	while (nItem--)
	{
		System::Windows::Forms::ToolStripItem^ tsi = items[nItem];

		tsi->Text = Translate(tsi->Text);
		tsi->ToolTipText = Translate(tsi->ToolTipText);

		// children
		System::Windows::Forms::ToolStripMenuItem^ tsmi = 
			dynamic_cast<System::Windows::Forms::ToolStripMenuItem^>(tsi);

		if ((tsmi != nullptr) && tsmi->HasDropDownItems)
		{
			Translate(tsmi->DropDownItems);
		}
	}
}

void Translator::Translate(System::Windows::Forms::Control::ControlCollection^ items)
{
	int nItem = items->Count;

	while (nItem--)
	{
		System::Windows::Forms::Control^ ctrl = items[nItem];

		// Avoid translating dynamic content
		// TODO

		ctrl->Text = Translate(ctrl->Text);

		// children
		Translate(ctrl->Controls);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

