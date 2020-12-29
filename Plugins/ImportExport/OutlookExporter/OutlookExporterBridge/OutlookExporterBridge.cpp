// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include "OutlookExporterBridge.h"

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include <Interfaces\ITasklist.h>
#include <Interfaces\ITransText.h>
#include <Interfaces\IPreferences.h>

////////////////////////////////////////////////////////////////////////////////////////////////

#using <PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace OutlookExporter;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

const LPCWSTR OUTLOOKEXPORTER_GUID = L"85D6AC7D-2D7D-4ACE-B776-C215FA181C33";
const LPCWSTR OUTLOOKEXPORTER_NAME = L"Microsoft Outlook";

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
COutlookExporterBridge::COutlookExporterBridge() : m_hIcon(NULL), m_pTT(nullptr)
{
	HMODULE hMod = LoadLibrary(L"OutlookExporterBridge.dll"); // us

	m_hIcon = (HICON)::LoadImage(hMod, MAKEINTRESOURCE(IDI_OUTLOOK), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS);
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

HICON COutlookExporterBridge::GetIcon() const
{
	return m_hIcon;
}

LPCWSTR COutlookExporterBridge::GetMenuText() const
{
	return OUTLOOKEXPORTER_NAME;
}

LPCWSTR COutlookExporterBridge::GetFileFilter() const
{
	return NULL; // Outlook has no file
}

LPCWSTR COutlookExporterBridge::GetFileExtension() const
{
	return NULL; // Outlook has no file
}

LPCWSTR COutlookExporterBridge::GetTypeID() const
{
	return OUTLOOKEXPORTER_GUID;
}

////////////////////////////////////////////////////////////////////////////////////////////////

IIMPORTEXPORT_RESULT COutlookExporterBridge::Export(const ITaskList* pSrcTaskFile, LPCWSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey)
{
	// call into out sibling C# module to do the actual work
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<TaskList^> srcTasks = gcnew TaskList(pSrcTaskFile);
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);
	msclr::auto_gcroot<OutlookExporterCore^> expCore = gcnew OutlookExporterCore(trans.get());
	
	// do the export
	bool bSilent = ((dwFlags & IIEF_SILENT) != 0);

	if (expCore->Export(srcTasks.get(), gcnew String(szDestFilePath), bSilent, prefs.get(), gcnew String(szKey)))
		return IIER_SUCCESS;

	return IIER_OTHER;
}

IIMPORTEXPORT_RESULT COutlookExporterBridge::Export(const IMultiTaskList* pSrcTaskFile, LPCWSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey)
{
	// TODO
	return IIER_OTHER;
}
