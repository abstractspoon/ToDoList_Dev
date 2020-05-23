// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include "HTMLReportExporterBridge.h"

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include <Interfaces\ITasklist.h>
#include <Interfaces\ITransText.h>
#include <Interfaces\IPreferences.h>

////////////////////////////////////////////////////////////////////////////////////////////////

#using <PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace HTMLReportExporter;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

const LPCWSTR HTMLREPORTER_GUID = L"95E6E2D4-2301-461A-80FB-C2863E788F37";
const LPCWSTR HTMLREPORTER_NAME = L"Report Builder";

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CHTMLReportExporterBridge::CHTMLReportExporterBridge() : m_pTT(nullptr), m_hIcon(NULL)
{
	HMODULE hMod = LoadLibrary(L"HTMLReportExporterBridge.dll"); // us

	m_hIcon = (HICON)::LoadImage(hMod, MAKEINTRESOURCE(IDI_HTMLREPORTER), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS);
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

IIMPORTEXPORT_RESULT CHTMLReportExporterBridge::Export(const ITaskList* pSrcTaskFile, LPCWSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCWSTR szKey)
{
	// call into out sibling C# module to do the actual work
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<TaskList^> srcTasks = gcnew TaskList(pSrcTaskFile);
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);
	msclr::auto_gcroot<String^> typeID = gcnew String(HTMLREPORTER_GUID);

	msclr::auto_gcroot<HTMLReportExporterCore^> expCore = gcnew HTMLReportExporterCore(typeID.get(), trans.get());
	
	// do the export
	if (expCore->Export(srcTasks.get(), gcnew String(szDestFilePath), bSilent, prefs.get(), gcnew String(szKey)))
		return IIER_SUCCESS;

	return IIER_OTHER;
}

IIMPORTEXPORT_RESULT CHTMLReportExporterBridge::Export(const IMultiTaskList* pSrcTaskFile, LPCWSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCWSTR szKey)
{
	// TODO
	return IIER_OTHER;
}
