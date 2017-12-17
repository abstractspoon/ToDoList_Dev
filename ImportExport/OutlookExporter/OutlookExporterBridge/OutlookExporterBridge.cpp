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
#	using <..\..\..\Debug\OutlookExporterCore.dll>
#	using <..\..\..\Debug\PluginHelpers.dll> as_friend
#else
#	using <..\..\..\Release\OutlookExporterCore.dll>
#	using <..\..\..\Release\PluginHelpers.dll> as_friend
#endif

using namespace OutlookExporter;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
COutlookExporterBridge::COutlookExporterBridge() : m_hIcon(NULL)
{
	HMODULE hMod = LoadLibrary(L"OutlookExporterBridge.dll"); // us

	m_hIcon = ::LoadIcon(hMod, MAKEINTRESOURCE(IDI_OUTLOOK));
}

void COutlookExporterBridge::Release()
{
	delete this;
}

void COutlookExporterBridge::SetLocalizer(ITransText* pTT)
{
	if (m_pTT == nullptr)
		m_pTT = pTT;
}

HICON COutlookExporterBridge::GetIcon(void) const
{
	return m_hIcon;
}

LPCWSTR COutlookExporterBridge::GetMenuText() const
{
	return L"Microsoft Outlook";
}

LPCWSTR COutlookExporterBridge::GetFileFilter() const
{
	return NULL; // Outlook has no file
}

LPCWSTR COutlookExporterBridge::GetFileExtension() const
{
	return NULL; // Outlook has no file
}

////////////////////////////////////////////////////////////////////////////////////////////////

IIMPORTEXPORT_RESULT COutlookExporterBridge::Export(const ITaskList* pSrcTaskFile, LPCWSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCWSTR szKey)
{
	// call into out sibling C# module to do the actual work
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<TaskList^> srcTasks = gcnew TaskList(pSrcTaskFile);
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);
	msclr::auto_gcroot<OutlookExporterCore^> expCore = gcnew OutlookExporterCore(trans.get());
	
	// do the export
	if (expCore->Export(srcTasks.get(), gcnew String(szDestFilePath), (bSilent != FALSE), prefs.get(), gcnew String(szKey)))
		return IIER_SUCCESS;

	return IIER_OTHER;
}

IIMPORTEXPORT_RESULT COutlookExporterBridge::Export(const IMultiTaskList* pSrcTaskFile, LPCWSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCWSTR szKey)
{
	// TODO
	return IIER_OTHER;
}
