// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SampleImpExpBridge.h"

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include "..\..\..\..\ToDoList_Dev\Interfaces\ITasklist.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\ITransText.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\IPreferences.h"

////////////////////////////////////////////////////////////////////////////////////////////////

#using <PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace SampleImpExp;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CSampleImpExpBridge::CSampleImpExpBridge()
{
// 	HMODULE hMod = LoadLibrary(L"SampleImpExpBridge.dll"); // us
// 
// 	m_hIcon = ::LoadIcon(hMod, MAKEINTRESOURCE(IDI_ICON));
}

void CSampleImpExpBridge::Release()
{
	delete this;
}

void CSampleImpExpBridge::SetLocalizer(ITransText* pTT)
{
	if (m_pTT == nullptr)
		m_pTT = pTT;
}

HICON CSampleImpExpBridge::GetIcon() const
{
	return m_hIcon;
}

LPCWSTR CSampleImpExpBridge::GetMenuText() const
{
	return L"Sample";
}

LPCWSTR CSampleImpExpBridge::GetFileFilter() const
{
	return L"smp";
}

LPCWSTR CSampleImpExpBridge::GetFileExtension() const
{
	return L"smp";
}

LPCWSTR CSampleImpExpBridge::GetTypeID() const
{
	return L"F0D68A9A-EEA3-4D2A-9242-86CF893CE8B0";
}

////////////////////////////////////////////////////////////////////////////////////////////////

IIMPORTEXPORT_RESULT CSampleImpExpBridge::Export(const ITaskList* pSrcTaskFile, LPCWSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCWSTR szKey)
{
	// call into out sibling C# module to do the actual work
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<TaskList^> srcTasks = gcnew TaskList(pSrcTaskFile);
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);
	msclr::auto_gcroot<SampleImpExpCore^> expCore = gcnew SampleImpExpCore(trans.get());
	
	// do the export
	if (expCore->Export(srcTasks.get(), gcnew String(szDestFilePath), (bSilent != FALSE), prefs.get(), gcnew String(szKey)))
		return IIER_SUCCESS;

	// else
	return IIER_OTHER;
}

IIMPORTEXPORT_RESULT CSampleImpExpBridge::Export(const IMultiTaskList* pSrcTaskFile, LPCWSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCWSTR szKey)
{
	// TODO
	return IIER_OTHER;
}
