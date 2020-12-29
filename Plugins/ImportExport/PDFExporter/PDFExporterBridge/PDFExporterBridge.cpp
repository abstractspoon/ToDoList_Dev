// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include "PDFExporterBridge.h"

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include <Interfaces\ITasklist.h>
#include <Interfaces\ITransText.h>
#include <Interfaces\IPreferences.h>

////////////////////////////////////////////////////////////////////////////////////////////////

#using <PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace PDFExporter;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

const LPCWSTR PDFEXPORTER_GUID = L"E6CEB38F-B834-4135-B56C-203E554058AE";
const LPCWSTR PDFEXPORTER_NAME = L"PDF Tree View";

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CPDFExporterBridge::CPDFExporterBridge() : m_pTT(nullptr), m_hIcon(NULL)
{
	HMODULE hMod = LoadLibrary(L"PDFExporterBridge.dll"); // us

	m_hIcon = (HICON)::LoadImage(hMod, MAKEINTRESOURCE(IDI_PDF), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS);
}

void CPDFExporterBridge::Release()
{
	delete this;
}

void CPDFExporterBridge::SetLocalizer(ITransText* pTT)
{
	if (m_pTT == nullptr)
		m_pTT = pTT;
}

HICON CPDFExporterBridge::GetIcon() const
{
	return m_hIcon;
}

LPCWSTR CPDFExporterBridge::GetMenuText() const
{
	return PDFEXPORTER_NAME;
}

LPCWSTR CPDFExporterBridge::GetFileFilter() const
{
	return _T("PDF Files (*.pdf)|*.pdf||");
}

LPCWSTR CPDFExporterBridge::GetFileExtension() const
{
	return _T("pdf");
}

LPCWSTR CPDFExporterBridge::GetTypeID() const
{
	return PDFEXPORTER_GUID;
}

////////////////////////////////////////////////////////////////////////////////////////////////

IIMPORTEXPORT_RESULT CPDFExporterBridge::Export(const ITaskList* pSrcTaskFile, LPCWSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey)
{
	// call into out sibling C# module to do the actual work
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<TaskList^> srcTasks = gcnew TaskList(pSrcTaskFile);
	msclr::auto_gcroot<Translator^> trans = gcnew Translator(m_pTT);
	msclr::auto_gcroot<String^> typeID = gcnew String(PDFEXPORTER_GUID);

	msclr::auto_gcroot<PDFExporterCore^> expCore = gcnew PDFExporterCore(typeID.get(), trans.get());
	
	// do the export
	bool bSilent = ((dwFlags & IIEF_SILENT) != 0);

	if (expCore->Export(srcTasks.get(), gcnew String(szDestFilePath), bSilent, prefs.get(), gcnew String(szKey)))
		return IIER_SUCCESS;

	return IIER_OTHER;
}

IIMPORTEXPORT_RESULT CPDFExporterBridge::Export(const IMultiTaskList* pSrcTaskFile, LPCWSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey)
{
	// TODO
	return IIER_OTHER;
}
