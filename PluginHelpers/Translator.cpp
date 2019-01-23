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
	// TODO

	Translate(window->Controls);
}

void Translator::Translate(System::Windows::Forms::ToolStripItemCollection^ items)
{
	// TODO
}

void Translator::Translate(System::Windows::Forms::Control::ControlCollection^ items)
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////////////////////

