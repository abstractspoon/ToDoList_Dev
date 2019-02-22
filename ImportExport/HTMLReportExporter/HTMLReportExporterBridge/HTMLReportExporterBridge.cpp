// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include "HTMLReportExporterBridge.h"

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include "..\..\..\..\ToDoList_Dev\Interfaces\ITasklist.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\ITransText.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\IPreferences.h"

////////////////////////////////////////////////////////////////////////////////////////////////

#using <PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace HTMLReportExporter;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CHTMLReportExporterBridge::CHTMLReportExporterBridge()
{
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
	return NULL;
}

LPCWSTR CHTMLReportExporterBridge::GetMenuText() const
{
	return L"HTML Reporter";
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
	return L"95E6E2D4-2301-461A-80FB-C2863E788F37";
}

////////////////////////////////////////////////////////////////////////////////////////////////

IIMPORTEXPORT_RESULT CHTMLReportExporterBridge::Export(const ITaskList* pSrcTaskFile, LPCWSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCWSTR szKey)
{
	// call into out sibling C# module to do the actual work
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<TaskList^> srcTasks = gcnew TaskList(pSrcTaskFile);
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);
	msclr::auto_gcroot<HTMLReportExporterCore^> expCore = gcnew HTMLReportExporterCore(trans.get());
	
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
