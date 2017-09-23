// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include "OutlookExporterBridge.h"

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include "..\..\..\..\ToDoList_Dev\Interfaces\ITasklist.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\ITransText.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\IPreferences.h"

////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#	using <..\..\..\Debug\SampleImpExpCore.dll>
#	using <..\..\..\Debug\PluginHelpers.dll> as_friend
#else
#	using <..\..\..\Release\SampleImpExpCore.dll>
#	using <..\..\..\Release\PluginHelpers.dll> as_friend
#endif

using namespace SampleImpExp;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
OutlookExporterCore::OutlookExporterCore() : m_hIcon(NULL)
{
	HMODULE hMod = LoadLibrary(L"OutlookExporterBridge.dll"); // us

	m_hIcon = ::LoadIcon(hMod, MAKEINTRESOURCE(IDI_OUTLOOK));
}

void OutlookExporterCore::Release()
{
	delete this;
}

void OutlookExporterCore::SetLocalizer(ITransText* /*pTT*/)
{
	// TODO
}

HICON OutlookExporterCore::GetIcon(void) const
{
	return m_hIcon;
}

LPCWSTR OutlookExporterCore::GetMenuText() const
{
	return L"Microsoft Outlook";
}

LPCWSTR OutlookExporterCore::GetFileFilter() const
{
	return NULL; // Outlook has no file
}

LPCWSTR OutlookExporterCore::GetFileExtension() const
{
	return NULL; // Outlook has no file
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool OutlookExporterCore::Export(const ITaskList* pSrcTaskFile, LPCWSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCWSTR szKey)
{
	// call into out sibling C# module to do the actual work
	msclr::auto_gcroot<SampleImpExpCore^> expCore = gcnew SampleImpExpCore();
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<TaskList^> srcTasks = gcnew TaskList(pSrcTaskFile);
	
	// do the export
	return expCore->Export(srcTasks.get(), gcnew String(szDestFilePath), (bSilent != FALSE), prefs.get(), gcnew String(szKey));
}

bool OutlookExporterCore::Export(const IMultiTaskList* pSrcTaskFile, LPCWSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCWSTR szKey)
{
	// TODO
	return false;
}
