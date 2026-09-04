// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ToDoTxtImpExpBridge.h"

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include <Interfaces\ITasklist.h>
#include <Interfaces\ITransText.h>
#include <Interfaces\IPreferences.h>

////////////////////////////////////////////////////////////////////////////////////////////////

#using <PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace ToDoTxtImpExp;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CToDoTxtImpExpBridge::CToDoTxtImpExpBridge()
{
	//m_hIcon = Win32::LoadHIcon(L"ToDoTxtImpExpBridge.dll", IDI_SAMPLE, 16, true);
}

void CToDoTxtImpExpBridge::Release()
{
	delete this;
}

void CToDoTxtImpExpBridge::SetLocalizer(ITransText* pTT)
{
	if (m_pTT == nullptr)
		m_pTT = pTT;
}

HICON CToDoTxtImpExpBridge::GetIcon() const
{
	return m_hIcon;
}

LPCWSTR CToDoTxtImpExpBridge::GetMenuText() const
{
	return L"ToDoTxt";
}

LPCWSTR CToDoTxtImpExpBridge::GetFileFilter() const
{
	return L"smp";
}

LPCWSTR CToDoTxtImpExpBridge::GetFileExtension() const
{
	return L"smp";
}

LPCWSTR CToDoTxtImpExpBridge::GetTypeID() const
{
	return L"F0D68A9A-EEA3-4D2A-9242-86CF893CE8B0";
}

////////////////////////////////////////////////////////////////////////////////////////////////

IIMPORTEXPORT_RESULT CToDoTxtImpExpBridge::Export(const ITaskList* pSrcTaskFile, LPCWSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey)
{
	// call into out sibling C# module to do the actual work
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<TaskList^> srcTasks = gcnew TaskList(pSrcTaskFile);
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);
	msclr::auto_gcroot<ToDoTxtImpExpCore^> expCore = gcnew ToDoTxtImpExpCore(trans.get());
	
	// do the export
	bool bSilent = ((dwFlags & IIEF_SILENT) != 0);

	if (expCore->Export(srcTasks.get(), gcnew String(szDestFilePath), bSilent, prefs.get(), gcnew String(szKey)))
		return IIER_SUCCESS;

	// else
	return IIER_OTHER;
}

IIMPORTEXPORT_RESULT CToDoTxtImpExpBridge::Export(const IMultiTaskList* pSrcTaskFile, LPCWSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey)
{
	// TODO
	return IIER_OTHER;
}
