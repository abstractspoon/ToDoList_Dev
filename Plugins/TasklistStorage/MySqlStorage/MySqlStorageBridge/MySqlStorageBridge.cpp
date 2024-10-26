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

	msclr::auto_gcroot<String^> tasklistId = gcnew String(pFInfo->szTasklistID);
	msclr::auto_gcroot<String^> password = gcnew String(pFInfo->szPassword);
	msclr::auto_gcroot<String^> destPath = Path::GetTempFileName();

	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);
	msclr::auto_gcroot<MySqlStorageCore^> mysql = gcnew MySqlStorageCore(trans.get());
	
	msclr::auto_gcroot<TasklistConnectionInfo^> info = mysql->RetrieveTasklist(tasklistId.get(),
																			   password.get(),
																			   destPath.get(),
																			   bPrompt,
																			   prefs.get(),
																			   gcnew String(szKey));
	if (info.get() == nullptr)
		return false;

	CopyInfo(info.get(), pFInfo);
	lstrcpy(pFInfo->szLocalFileName, MarshalledString(destPath.get()));

	// Cache the password for next time
	lstrcpy(szCachedPassword, pFInfo->szPassword);

	return true;
}

bool CMySqlStorageBridge::StoreTasklist(ITS_TASKLISTINFO* pFInfo, const ITaskList* pSrcTaskFile, IPreferences* pPrefs, LPCWSTR szKey, bool bPrompt)
{
	if (pFInfo->szPassword[0] == 0)
		lstrcpy(pFInfo->szPassword, szCachedPassword);

	msclr::auto_gcroot<String^> tasklistId = gcnew String(pFInfo->szTasklistID);
	msclr::auto_gcroot<String^> tasklistName = gcnew String(pFInfo->szTasklistName);
	msclr::auto_gcroot<String^> password = gcnew String(pFInfo->szPassword);
	msclr::auto_gcroot<String^> srcPath = gcnew String(pFInfo->szLocalFileName);

	msclr::auto_gcroot<TaskList^> srcTasks = gcnew TaskList(pSrcTaskFile);
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);
	msclr::auto_gcroot<MySqlStorageCore^> mysql = gcnew MySqlStorageCore(trans.get());

	msclr::auto_gcroot<TasklistConnectionInfo^> info = mysql->StoreTasklist(tasklistId.get(),
																			tasklistName.get(),
																			password.get(),
																			srcPath.get(),
																			bPrompt,
																			prefs.get(),
																			gcnew String(szKey));
	if (info.get() == nullptr)
		return false;

	CopyInfo(info.get(), pFInfo);
	lstrcpy(pFInfo->szLocalFileName, MarshalledString(srcPath.get()));

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
