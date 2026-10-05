// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include "MySqlStorageBridge.h"

#include <unknwn.h>
#include <tchar.h>

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
CMySqlStorageBridge::CMySqlStorageBridge() : m_pTT(nullptr)
{
	szCachedPassword[0] = 0;

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

bool CMySqlStorageBridge::RetrieveTasklist(ITS_TASKLISTINFO* pFInfo, ITaskList* pDestTaskFile, IPreferences* pPrefs, LPCWSTR szKey, bool bPrompt)
{
	if (pFInfo->szPassword[0] == 0)
		lstrcpy(pFInfo->szPassword, szCachedPassword);

	auto tasklistId = gcnew String(pFInfo->szTasklistID);
	auto password = gcnew String(pFInfo->szPassword);
	auto destPath = Path::GetTempFileName();

	auto prefs = gcnew Preferences(pPrefs);
	auto key = gcnew String(szKey);
	auto trans = gcnew Translator(m_pTT);
	auto mysql = gcnew MySqlStorageCore(trans);
	
	auto info = mysql->RetrieveTasklist(tasklistId,
										password,
										destPath,
										bPrompt,
										prefs,
										key);
	if (info == nullptr)
		return false;

	CopyInfo(info, pFInfo);
	lstrcpy(pFInfo->szLocalFileName, MarshalledString(destPath));

	// Cache the password for next time
	lstrcpy(szCachedPassword, pFInfo->szPassword);

	return true;
}

bool CMySqlStorageBridge::StoreTasklist(ITS_TASKLISTINFO* pFInfo, const ITaskList* pSrcTaskFile, IPreferences* pPrefs, LPCWSTR szKey, bool bPrompt)
{
	if (pFInfo->szPassword[0] == 0)
		lstrcpy(pFInfo->szPassword, szCachedPassword);

	auto tasklistId = gcnew String(pFInfo->szTasklistID);
	auto tasklistName = gcnew String(pFInfo->szTasklistName);
	auto password = gcnew String(pFInfo->szPassword);
	auto srcPath = gcnew String(pFInfo->szLocalFileName);

	auto srcTasks = gcnew TaskList(pSrcTaskFile);
	auto prefs = gcnew Preferences(pPrefs);
	auto key = gcnew String(szKey);
	auto trans = gcnew Translator(m_pTT);
	auto mysql = gcnew MySqlStorageCore(trans);

	auto info = mysql->StoreTasklist(tasklistId,
									 tasklistName,
									 password,
									 srcPath,
									 bPrompt,
									 prefs,
									 key);
	if (info == nullptr)
		return false;

	CopyInfo(info, pFInfo);
	lstrcpy(pFInfo->szLocalFileName, MarshalledString(srcPath));

	// Cache the password for next time
	lstrcpy(szCachedPassword, pFInfo->szPassword);

	return true;
}

void CMySqlStorageBridge::CopyInfo(TasklistConnectionInfo^ fromInfo, ITS_TASKLISTINFO* toInfo)
{
	lstrcpy(toInfo->szTasklistID, MarshalledString(fromInfo->TasklistId));
	lstrcpy(toInfo->szTasklistName, MarshalledString(fromInfo->Tasklist->Name));
	lstrcpy(toInfo->szPassword, MarshalledString(fromInfo->Connection->Password));

	String^ displayPath = String::Format(L"{0}/{1}/{2}", 
										 fromInfo->Connection->Server,
										 fromInfo->Connection->DatabaseName,
										 fromInfo->Tasklist->Name);

	lstrcpy(toInfo->szDisplayPath, MarshalledString(displayPath));
}
