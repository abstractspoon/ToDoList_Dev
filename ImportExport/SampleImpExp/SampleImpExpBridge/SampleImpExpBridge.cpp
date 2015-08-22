// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include <unknwn.h>
#include <tchar.h>

#include "stdafx.h"
#include "SampleImpExpBridge.h"

#include "..\..\..\Interfaces\ITasklist.h"
#include "..\..\..\Interfaces\ITransText.h"
#include "..\..\..\Interfaces\IPreferences.h"

////////////////////////////////////////////////////////////////////////////////////////////////

#using <..\Debug\SampleImpExpCore.dll>
#include <msclr\auto_gcroot.h>

#using <..\Debug\PluginHelpers.dll> as_friend

using namespace SampleImpExp;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CSampleImpExpBridge::CSampleImpExpBridge()
{
	return;
}

void CSampleImpExpBridge::Release()
{
	delete this;
}

void CSampleImpExpBridge::SetLocalizer(ITransText* /*pTT*/)
{
	// TODO
}

LPCTSTR CSampleImpExpBridge::GetMenuText() const
{
	return L"Sample";
}

LPCTSTR CSampleImpExpBridge::GetFileFilter() const
{
	return L"smp";
}

LPCTSTR CSampleImpExpBridge::GetFileExtension() const
{
	return L"smp";
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool CSampleImpExpBridge::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, BOOL bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	// call into out sibling C# module to do the actual work
	msclr::auto_gcroot<SampleImpExpCore^> expCore = gcnew SampleImpExpCore();
	msclr::auto_gcroot<TDLPreferences^> prefs = gcnew TDLPreferences(pPrefs);
	msclr::auto_gcroot<TDLTaskList^> srcTasks = gcnew TDLTaskList(pSrcTaskFile);
	
	// do the export
	return expCore->Export(srcTasks.get(), gcnew String(szDestFilePath), (bSilent != FALSE), prefs.get(), gcnew String(szKey));
}

bool CSampleImpExpBridge::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, BOOL bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	// TODO
	return false;
}
