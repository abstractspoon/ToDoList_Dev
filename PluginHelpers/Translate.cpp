// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "Translate.h"

#include "..\..\ToDoList_Dev\Interfaces\ITransText.h"

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

Translate::Translate(ITransText* pTransText) : m_pTransText(pTransText) 
{
   int breakpoint = 0;
} 

// private constructor
Translate::Translate() : m_pTransText(NULL)
{

}

// TODO

////////////////////////////////////////////////////////////////////////////////////////////////

