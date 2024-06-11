// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include "MySqlStorageBridge.h"

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include <Interfaces\ITasklist.h>
#include <Interfaces\ITransText.h>
#include <Interfaces\IPreferences.h>

////////////////////////////////////////////////////////////////////////////////////////////////

#using <PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

using namespace MySqlStorage;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CMySqlStorageBridge::CMySqlStorageBridge()
{
	HMODULE hMod = LoadLibrary(L"MySqlStorageBridge.dll"); // us

	m_hIcon = (HICON)::LoadImage(hMod, MAKEINTRESOURCE(IDI_MYSQL), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS);
}

void CMySqlStorageBridge::Release()
{
	delete this;
}

void CMySqlStorageBridge::SetLocalizer(ITransText* pTT)
{
	if (m_pTT == nullptr)
		m_pTT = pTT;
}

HICON CMySqlStorageBridge::GetIcon() const
{
	return m_hIcon;
}

LPCWSTR CMySqlStorageBridge::GetMenuText() const
{
	return L"MySQL Database";
}

LPCWSTR CMySqlStorageBridge::GetTypeID() const
{
	return L"ABEE8308-7109-482D-9381-5674967F5CAF";
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool CMySqlStorageBridge::RetrieveTasklist(ITS_TASKLISTINFO* pFInfo, ITaskList* pDestTaskFile, IPreferences* pPrefs, LPCWSTR szKey, bool bSilent)
{
	// call into out sibling C# module to do the actual work
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<TaskList^> destTasks = gcnew TaskList(pDestTaskFile);
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);
	msclr::auto_gcroot<MySqlStorageCore^> expCore = gcnew MySqlStorageCore(trans.get());
	
// 	if (expCore->Export(destTasks.get(), gcnew String(szDestFilePath), bSilent, prefs.get(), gcnew String(szKey)))
// 		return true;

	// else
	return false;
}

bool CMySqlStorageBridge::StoreTasklist(ITS_TASKLISTINFO* pFInfo, const ITaskList* pSrcTaskFile, IPreferences* pPrefs, LPCWSTR szKey, bool bSilent)
{
	// TODO
	return false;
}
