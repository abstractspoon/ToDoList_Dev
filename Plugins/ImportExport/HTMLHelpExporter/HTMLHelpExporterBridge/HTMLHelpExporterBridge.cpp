// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include "HTMLHelpExporterBridge.h"

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include <Interfaces\ITasklist.h>
#include <Interfaces\ITransText.h>
#include <Interfaces\IPreferences.h>

////////////////////////////////////////////////////////////////////////////////////////////////

#using <PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace HTMLHelpExporter;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

const LPCWSTR HTMLHELP_GUID = L"ABBC23CD-AE9C-4BEA-AB96-66D8090F470F";
const LPCWSTR HTMLREPORTER_NAME = L"Compiled Help";

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CHTMLHelpExporterBridge::CHTMLHelpExporterBridge() : m_pTT(nullptr), m_hIcon(NULL)
{
	HMODULE hMod = LoadLibrary(L"HTMLHelpExporterBridge.dll"); // us

	m_hIcon = (HICON)::LoadImage(hMod, MAKEINTRESOURCE(IDI_HTMLHELP), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS);
}

void CHTMLHelpExporterBridge::Release()
{
	delete this;
}

void CHTMLHelpExporterBridge::SetLocalizer(ITransText* pTT)
{
	if (m_pTT == nullptr)
		m_pTT = pTT;
}

HICON CHTMLHelpExporterBridge::GetIcon() const
{
	return m_hIcon;
}

LPCWSTR CHTMLHelpExporterBridge::GetMenuText() const
{
	return HTMLREPORTER_NAME;
}

LPCWSTR CHTMLHelpExporterBridge::GetFileFilter() const
{
	return _T("Compiled Help Files (*.chm)|*.chm||");
}

LPCWSTR CHTMLHelpExporterBridge::GetFileExtension() const
{
	return _T("chm");
}

LPCWSTR CHTMLHelpExporterBridge::GetTypeID() const
{
	return HTMLHELP_GUID;
}

////////////////////////////////////////////////////////////////////////////////////////////////

IIMPORTEXPORT_RESULT CHTMLHelpExporterBridge::Export(const ITaskList* pSrcTaskFile, LPCWSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCWSTR szKey)
{
	// call into out sibling C# module to do the actual work
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<TaskList^> srcTasks = gcnew TaskList(pSrcTaskFile);
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);
	msclr::auto_gcroot<String^> typeID = gcnew String(HTMLHELP_GUID);

	msclr::auto_gcroot<HTMLHelpExporterCore^> expCore = gcnew HTMLHelpExporterCore(typeID.get(), trans.get());
	
	// do the export
	if (expCore->Export(srcTasks.get(), gcnew String(szDestFilePath), bSilent, prefs.get(), gcnew String(szKey)))
		return IIER_SUCCESS;

	return IIER_OTHER;
}

IIMPORTEXPORT_RESULT CHTMLHelpExporterBridge::Export(const IMultiTaskList* pSrcTaskFile, LPCWSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCWSTR szKey)
{
	// TODO
	return IIER_OTHER;
}
