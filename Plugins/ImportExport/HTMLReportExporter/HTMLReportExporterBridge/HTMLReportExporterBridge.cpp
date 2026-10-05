// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include "HTMLReportExporterBridge.h"

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

using namespace HTMLReportExporter;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

const LPCWSTR HTMLREPORTER_GUID = L"95E6E2D4-2301-461A-80FB-C2863E788F37";
const LPCWSTR HTMLREPORTER_NAME = L"Report Builder";

////////////////////////////////////////////////////////////////////////////////////////////////

IExportTasklist* CreateExportInterface()
{
	// Disable this module on Linux because it requires IE and will otherwise crash
	if (OSVersion::IsLinux())
		return NULL;

	return new CHTMLReportExporterBridge();
}

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CHTMLReportExporterBridge::CHTMLReportExporterBridge() : m_pTT(nullptr), m_hIcon(NULL)
{
	m_hIcon = Win32::LoadHIcon(L"HTMLReportExporterBridge.dll", IDI_HTMLREPORTER, 16, true);
}

void CHTMLReportExporterBridge::Release()
{
	delete this;
}

void CHTMLReportExporterBridge::SetLocalizer(ITransText* pTT)
{
	if (m_pTT == nullptr)
		m_pTT = pTT;
}

HICON CHTMLReportExporterBridge::GetIcon() const
{
	return m_hIcon;
}

LPCWSTR CHTMLReportExporterBridge::GetMenuText() const
{
	return HTMLREPORTER_NAME;
}

LPCWSTR CHTMLReportExporterBridge::GetFileFilter() const
{
	return _T("Report Files (*.html)|*.html||");
}

LPCWSTR CHTMLReportExporterBridge::GetFileExtension() const
{
	return _T("html");
}

LPCWSTR CHTMLReportExporterBridge::GetTypeID() const
{
	return HTMLREPORTER_GUID;
}

////////////////////////////////////////////////////////////////////////////////////////////////

IIMPORTEXPORT_RESULT CHTMLReportExporterBridge::Export(const ITaskList* pSrcTaskFile, LPCWSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey)
{
	// call into out sibling C# module to do the actual work
	auto prefs = gcnew Preferences(pPrefs);
	auto key = gcnew String(szKey);
	auto srcTasks = gcnew TaskList(pSrcTaskFile);
	auto trans = gcnew Translator(m_pTT);
	auto typeID = gcnew String(HTMLREPORTER_GUID);
	auto destPath = gcnew String(szDestFilePath);

	auto exporter = gcnew HTMLReportExporterCore(typeID, trans);
	
	// do the export
	bool bSilent = ((dwFlags & IIEF_SILENT) != 0);
	bool bPrinting = ((dwFlags & IIEF_PRINTING) != 0);

	if (exporter->Export(srcTasks, destPath, bSilent, bPrinting, prefs, key))
		return IIER_SUCCESS;

	return IIER_OTHER;
}

IIMPORTEXPORT_RESULT CHTMLReportExporterBridge::Export(const IMultiTaskList* pSrcTaskFile, LPCWSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey)
{
	// call into out sibling C# module to do the actual work
	auto prefs = gcnew Preferences(pPrefs);
	auto key = gcnew String(szKey);
	auto srcTasks = gcnew MultiTaskList(pSrcTaskFile);
	auto trans = gcnew Translator(m_pTT);
	auto typeID = gcnew String(HTMLREPORTER_GUID);
	auto destPath = gcnew String(szDestFilePath);

	auto exporter = gcnew HTMLReportExporterCore(typeID, trans);

	// do the export
	bool bSilent = ((dwFlags & IIEF_SILENT) != 0);
	bool bPrinting = ((dwFlags & IIEF_PRINTING) != 0);

	if (exporter->Export(srcTasks, destPath, bSilent, bPrinting, prefs, key))
		return IIER_SUCCESS;

	return IIER_OTHER;
}
