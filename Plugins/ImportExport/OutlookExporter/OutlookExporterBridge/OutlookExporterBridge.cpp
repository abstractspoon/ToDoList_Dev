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
	m_hIcon = Win32::LoadHIcon(L"OutlookExporterBridge.dll", IDI_OUTLOOK, 16, true);
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
	auto prefs = gcnew Preferences(pPrefs);
	auto key = gcnew String(szKey);
	auto srcTasks = gcnew TaskList(pSrcTaskFile);
	auto trans = gcnew Translator(m_pTT);
	auto destPath = gcnew String(szDestFilePath);

	auto exporter = gcnew OutlookExporterCore(trans);
	
	// do the export
	bool bSilent = ((dwFlags & IIEF_SILENT) != 0);

	if (exporter->Export(srcTasks, destPath, bSilent, prefs, key))
		return IIER_SUCCESS;

	return IIER_OTHER;
}

IIMPORTEXPORT_RESULT COutlookExporterBridge::Export(const IMultiTaskList* pSrcTaskFile, LPCWSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey)
{
	// TODO
	return IIER_OTHER;
}
