// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include "MarkdeepExporterBridge.h"

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

using namespace MarkdeepExporter;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

const LPCWSTR MARKDEEPEXPORTER_GUID = L"49A52D2D-7661-49AF-949A-E60066B300FC";
const LPCWSTR MARKDEEPEXPORTER_NAME = L"Markdeep";

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CMarkdeepExporterBridge::CMarkdeepExporterBridge() : m_hIcon(NULL), m_pTT(NULL)
{
	m_hIcon = Win32::LoadHIcon(L"MarkdeepExporterBridge.dll", IDI_MARKDEEP, 16, true);
}

void CMarkdeepExporterBridge::Release()
{
	delete this;
}

void CMarkdeepExporterBridge::SetLocalizer(ITransText* pTT)
{
	if (m_pTT == nullptr)
		m_pTT = pTT;
}

HICON CMarkdeepExporterBridge::GetIcon() const
{
	return m_hIcon;
}

LPCWSTR CMarkdeepExporterBridge::GetMenuText() const
{
	return MARKDEEPEXPORTER_NAME;
}

LPCWSTR CMarkdeepExporterBridge::GetFileFilter() const
{
	return _T("Markdeep Files (*.md.html)|*.md.html||");
}

LPCWSTR CMarkdeepExporterBridge::GetFileExtension() const
{
	return L"md.html";
}

LPCWSTR CMarkdeepExporterBridge::GetTypeID() const
{
	return MARKDEEPEXPORTER_GUID;
}

////////////////////////////////////////////////////////////////////////////////////////////////

IIMPORTEXPORT_RESULT CMarkdeepExporterBridge::Export(const ITaskList* pSrcTaskFile, LPCWSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey)
{
	// call into out sibling C# module to do the actual work
	auto prefs = gcnew Preferences(pPrefs);
	auto key = gcnew String(szKey);
	auto srcTasks = gcnew TaskList(pSrcTaskFile);
	auto trans = gcnew Translator(m_pTT);
	auto typeID = gcnew String(MARKDEEPEXPORTER_GUID);
	auto destPath = gcnew String(szDestFilePath);

	auto exporter = gcnew MarkdeepExporterCore(typeID, trans);

	// do the export
	bool bSilent = ((dwFlags & IIEF_SILENT) != 0);

	if (exporter->Export(srcTasks, destPath, bSilent, prefs, key))
		return IIER_SUCCESS;

	return IIER_OTHER;
}

IIMPORTEXPORT_RESULT CMarkdeepExporterBridge::Export(const IMultiTaskList* pSrcTaskFile, LPCWSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey)
{
	// call into out sibling C# module to do the actual work
	auto prefs = gcnew Preferences(pPrefs);
	auto key = gcnew String(szKey);
	auto srcTasks = gcnew MultiTaskList(pSrcTaskFile);
	auto trans = gcnew Translator(m_pTT);
	auto typeID = gcnew String(MARKDEEPEXPORTER_GUID);
	auto destPath = gcnew String(szDestFilePath);

	auto exporter = gcnew MarkdeepExporterCore(typeID, trans);

	// do the export
	bool bSilent = ((dwFlags & IIEF_SILENT) != 0);

	if (exporter->Export(srcTasks, destPath, bSilent, prefs, key))
		return IIER_SUCCESS;

	return IIER_OTHER;
}
