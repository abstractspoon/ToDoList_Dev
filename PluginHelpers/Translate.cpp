// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "Translate.h"

#include <msclr\auto_gcroot.h>

#include "..\..\ToDoList_Dev\Interfaces\ITransText.h"

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

Translate::Translate(ITransText* pTransText) : m_pTransText(pTransText) 
{
} 

// private constructor
Translate::Translate() : m_pTransText(nullptr)
{

}

String^ Translate::TranslateText(String^ sText)
{
	if (m_pTransText == nullptr)
		return sText;

	LPWSTR szTemp = NULL;

	if (!m_pTransText->TranslateText(MS(sText), szTemp))
		return sText;

	msclr::auto_gcroot<String^> sTextOut = gcnew String(szTemp);
	m_pTransText->FreeTextBuffer(szTemp);

	return sTextOut;
}

////////////////////////////////////////////////////////////////////////////////////////////////

