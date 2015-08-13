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
CSampleBridge::CSampleBridge()
{
	return;
}

void CSampleBridge::Release()
{
	delete this;
}

void CSampleBridge::SetLocalizer(ITransText* /*pTT*/)
{
	// TODO
}

LPCTSTR CSampleBridge::GetMenuText() const
{
	return L"Sample";
}

LPCTSTR CSampleBridge::GetFileFilter() const
{
	return L"smp";
}

LPCTSTR CSampleBridge::GetFileExtension() const
{
	return L"smp";
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool CSampleBridge::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, BOOL bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	// call into out sibling C# module to do the actual work
	msclr::auto_gcroot<SampleImpExpCore^> expCore = gcnew SampleImpExpCore();
	msclr::auto_gcroot<CPreferences^> prefs = gcnew CPreferences(pPrefs);
	msclr::auto_gcroot<CTaskList^> srcTasks = gcnew CTaskList(GetITLInterface<ITaskList14>(pSrcTaskFile, IID_TASKLIST14));
	
	// do the export
	return expCore->Export(srcTasks.get(), gcnew String(szDestFilePath), (bSilent != FALSE), prefs.get(), gcnew String(szKey));
}

bool CSampleBridge::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, BOOL bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	// TODO
	return false;
}
