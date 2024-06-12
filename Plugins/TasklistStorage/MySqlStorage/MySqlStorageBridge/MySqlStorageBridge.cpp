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
	msclr::auto_gcroot<String^> tasklistId = gcnew String(pFInfo->szTasklistID);
	msclr::auto_gcroot<String^> password = gcnew String(pFInfo->szPassword);
	msclr::auto_gcroot<String^> destPath = Path::GetTempFileName();

	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);
	msclr::auto_gcroot<MySqlStorageCore^> mysql = gcnew MySqlStorageCore(trans.get());
	
	MySqlStorageDefinition^ def = mysql->RetrieveTasklist(tasklistId.get(),
														  password.get(),
														  destPath.get(),
														  bSilent,
														  prefs.get(),
														  gcnew String(szKey));
	if (def == nullptr)
		return false;

	lstrcpy(pFInfo->szLocalFileName, MarshalledString(destPath.get()));
	lstrcpy(pFInfo->szTasklistID, MarshalledString(def->Encode()));
	lstrcpy(pFInfo->szPassword, MarshalledString(def->Password));
	lstrcpy(pFInfo->szDisplayName, MarshalledString(def->DisplayName));

	return true;
}

bool CMySqlStorageBridge::StoreTasklist(ITS_TASKLISTINFO* pFInfo, const ITaskList* pSrcTaskFile, IPreferences* pPrefs, LPCWSTR szKey, bool bSilent)
{
	msclr::auto_gcroot<String^> tasklistId = gcnew String(pFInfo->szTasklistID);
	msclr::auto_gcroot<String^> password = gcnew String(pFInfo->szPassword);
	msclr::auto_gcroot<String^> srcPath = gcnew String(pFInfo->szLocalFileName);

	msclr::auto_gcroot<TaskList^> srcTasks = gcnew TaskList(pSrcTaskFile);
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);
	msclr::auto_gcroot<MySqlStorageCore^> mysql = gcnew MySqlStorageCore(trans.get());

	MySqlStorageDefinition^ def = mysql->StoreTasklist(tasklistId.get(),
													   srcTasks->GetProjectName(),
													   password.get(),
													   srcPath.get(),
													   bSilent,
													   prefs.get(),
													   gcnew String(szKey));
	if (def == nullptr)
		return false;

	lstrcpy(pFInfo->szLocalFileName, MarshalledString(srcPath.get()));
	lstrcpy(pFInfo->szTasklistID, MarshalledString(def->Encode()));
	lstrcpy(pFInfo->szPassword, MarshalledString(def->Password));
	lstrcpy(pFInfo->szDisplayName, MarshalledString(def->DisplayName));

	return true;
}
