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
using namespace System::IO;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

using namespace MySqlStorage;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CMySqlStorageBridge::CMySqlStorageBridge()
{
	m_hIcon = Win32::LoadHIcon(L"MySqlStorageBridge.dll", IDI_MYSQL, 16, true);
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
	msclr::auto_gcroot<String^> tasklistId = gcnew String(pFInfo->szTasklistID);
	msclr::auto_gcroot<String^> password = gcnew String(pFInfo->szPassword);
	msclr::auto_gcroot<String^> destPath = Path::GetTempFileName();

	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);
	msclr::auto_gcroot<MySqlStorageCore^> mysql = gcnew MySqlStorageCore(trans.get());
	
	msclr::auto_gcroot<ConnectionDefinition^> def = mysql->RetrieveTasklist(tasklistId.get(),
																			password.get(),
																			destPath.get(),
																			bSilent,
																			prefs.get(),
																			gcnew String(szKey));
	if (def.get() == nullptr)
		return false;

	CopyInfo(def.get(), pFInfo);
	lstrcpy(pFInfo->szLocalFileName, MarshalledString(destPath.get()));

	return true;
}

bool CMySqlStorageBridge::StoreTasklist(ITS_TASKLISTINFO* pFInfo, const ITaskList* pSrcTaskFile, IPreferences* pPrefs, LPCWSTR szKey, bool bSilent)
{
	msclr::auto_gcroot<String^> tasklistId = gcnew String(pFInfo->szTasklistID);
	msclr::auto_gcroot<String^> tasklistName = gcnew String(pFInfo->szTasklistName);
	msclr::auto_gcroot<String^> password = gcnew String(pFInfo->szPassword);
	msclr::auto_gcroot<String^> srcPath = gcnew String(pFInfo->szLocalFileName);

	msclr::auto_gcroot<TaskList^> srcTasks = gcnew TaskList(pSrcTaskFile);
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);
	msclr::auto_gcroot<MySqlStorageCore^> mysql = gcnew MySqlStorageCore(trans.get());

	msclr::auto_gcroot<ConnectionDefinition^> def = mysql->StoreTasklist(tasklistId.get(),
																		 tasklistName.get(),
																		 password.get(),
																		 srcPath.get(),
																		 bSilent,
																		 prefs.get(),
																		 gcnew String(szKey));
	if (def.get() == nullptr)
		return false;

	CopyInfo(def.get(), pFInfo);
	lstrcpy(pFInfo->szLocalFileName, MarshalledString(srcPath.get()));

	return true;
}

void CMySqlStorageBridge::CopyInfo(ConnectionDefinition^ fromDef, ITS_TASKLISTINFO* pToFInfo)
{
	lstrcpy(pToFInfo->szTasklistID, MarshalledString(fromDef->Encode()));
	lstrcpy(pToFInfo->szTasklistName, MarshalledString(fromDef->TasklistName));
	lstrcpy(pToFInfo->szPassword, MarshalledString(fromDef->Password));

	String^ displayName = String::Format(L"{0}@{1}/{2}", fromDef->Username, fromDef->Server, fromDef->Database);
	lstrcpy(pToFInfo->szDisplayName, MarshalledString(displayName));
}
