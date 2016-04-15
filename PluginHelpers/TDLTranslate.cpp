// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "TDLTranslate.h"

#include "..\..\Interfaces\ITransText.h"

using namespace TDLPluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

TDLTranslate::TDLTranslate(ITransText* pTransText) : m_pTransText(pTransText) 
{
   int breakpoint = 0;
} 

// private constructor
TDLTranslate::TDLTranslate() : m_pTransText(NULL)
{

}

// TODO

////////////////////////////////////////////////////////////////////////////////////////////////

