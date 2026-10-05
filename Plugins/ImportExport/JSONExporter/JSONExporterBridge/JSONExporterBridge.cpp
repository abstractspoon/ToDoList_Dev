// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include "JSONExporterBridge.h"

#include <unknwn.h>
#include <tchar.h>

#include <Interfaces\ITasklist.h>
#include <Interfaces\ITransText.h>
#include <Interfaces\IPreferences.h>

////////////////////////////////////////////////////////////////////////////////////////////////

#using <PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

using namespace Abstractspoon::Tdl::PluginHelpers;

using namespace JSONExporterPlugin;

////////////////////////////////////////////////////////////////////////////////////////////////

const LPCWSTR JSONEXPORTER_GUID = L"FE1CB6A5-D206-499F-81E0-76B3BB5825FE";
const LPCWSTR JSONEXPORTER_NAME = L"JSON";

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CJSONExporterBridge::CJSONExporterBridge() : m_hIcon(NULL), m_pTT(NULL)
{
	m_hIcon = Win32::LoadHIcon(L"JSONExporterBridge.dll", IDI_JSON, 16, true);
}

void CJSONExporterBridge::Release()
{
	delete this;
}

void CJSONExporterBridge::SetLocalizer(ITransText* pTT)
{
	if (m_pTT == nullptr)
		m_pTT = pTT;
}

HICON CJSONExporterBridge::GetIcon() const
{
	return m_hIcon;
}

LPCWSTR CJSONExporterBridge::GetMenuText() const
{
	return JSONEXPORTER_NAME;
}

LPCWSTR CJSONExporterBridge::GetFileFilter() const
{
	return _T("JSON Files (*.json)|*.json||");
}

LPCWSTR CJSONExporterBridge::GetFileExtension() const
{
	return L"json";
}

LPCWSTR CJSONExporterBridge::GetTypeID() const
{
	return JSONEXPORTER_GUID;
}

////////////////////////////////////////////////////////////////////////////////////////////////

IIMPORTEXPORT_RESULT CJSONExporterBridge::Export(const ITaskList* pSrcTaskFile, LPCWSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey)
{
	// call into out sibling C# module to do the actual work
	auto prefs = gcnew Preferences(pPrefs);
	auto key = gcnew String(szKey);
	auto srcTasks = gcnew TaskList(pSrcTaskFile);
	auto trans = gcnew Translator(m_pTT);
	auto typeID = gcnew String(JSONEXPORTER_GUID);
	auto destPath = gcnew String(szDestFilePath);

	auto exporter = gcnew JSONExporterCore(typeID, trans);

	// do the export
	bool bSilent = ((dwFlags & IIEF_SILENT) != 0);

	if (exporter->Export(srcTasks, destPath, bSilent, prefs, key))
		return IIER_SUCCESS;

	return IIER_OTHER;
}

IIMPORTEXPORT_RESULT CJSONExporterBridge::Export(const IMultiTaskList* pSrcTaskFile, LPCWSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey)
{
	// call into out sibling C# module to do the actual work
	auto prefs = gcnew Preferences(pPrefs);
	auto key = gcnew String(szKey);
	auto srcTasks = gcnew MultiTaskList(pSrcTaskFile);
	auto trans = gcnew Translator(m_pTT);
	auto typeID = gcnew String(JSONEXPORTER_GUID);
	auto destPath = gcnew String(szDestFilePath);

	auto exporter = gcnew JSONExporterCore(typeID, trans);

	// do the export
	bool bSilent = ((dwFlags & IIEF_SILENT) != 0);

	if (exporter->Export(srcTasks, destPath, bSilent, prefs, key))
		return IIER_SUCCESS;

	return IIER_OTHER;
}
