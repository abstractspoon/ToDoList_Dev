// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "TodoTxtImpExpBridge.h"

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include <Interfaces\ITasklist.h>
#include <Interfaces\ITransText.h>
#include <Interfaces\IPreferences.h>

////////////////////////////////////////////////////////////////////////////////////////////////

#using <PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace TodoTxtImpExp;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CTodoTxtImpExpBridge::CTodoTxtImpExpBridge()
	:
	m_hIcon(NULL),
	m_pTT(nullptr)
{
	//m_hIcon = Win32::LoadHIcon(L"TodoTxtImpExpBridge.dll", IDI_TODOTXT, 16, true);
}

void CTodoTxtImpExpBridge::Release()
{
	delete this;
}

void CTodoTxtImpExpBridge::SetLocalizer(ITransText* pTT)
{
	if (m_pTT == nullptr)
		m_pTT = pTT;
}

HICON CTodoTxtImpExpBridge::GetIcon() const
{
	return m_hIcon;
}

LPCWSTR CTodoTxtImpExpBridge::GetMenuText() const
{
	return L"todo.txt";
}

LPCWSTR CTodoTxtImpExpBridge::GetFileFilter() const
{
	return L"txt";
}

LPCWSTR CTodoTxtImpExpBridge::GetFileExtension() const
{
	return L"txt";
}

LPCWSTR CTodoTxtImpExpBridge::GetTypeID() const
{
	return L"E18EBEBB-4F06-476B-B3C9-069A6B25D521";
}

////////////////////////////////////////////////////////////////////////////////////////////////

// IExportTasklist
IIMPORTEXPORT_RESULT CTodoTxtImpExpBridge::Export(const ITaskList* pSrcTaskFile, LPCWSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey)
{
	auto exporter = gcnew TodoTxtExporter(gcnew Translator(m_pTT));

	if (exporter->Export(gcnew TaskList(pSrcTaskFile),
						 gcnew String(szDestFilePath),
						 ((dwFlags & IIEF_SILENT) != 0),
						 gcnew Preferences(pPrefs),
						 gcnew String(szKey)))
	{
		return IIER_SUCCESS;
	}

	// else
	return IIER_OTHER;
}

IIMPORTEXPORT_RESULT CTodoTxtImpExpBridge::Export(const IMultiTaskList* pSrcTaskFile, LPCWSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey)
{
	// TODO
	return IIER_OTHER;
}

// IImportTasklist
IIMPORTEXPORT_RESULT CTodoTxtImpExpBridge::Import(LPCWSTR szSrcFilePath, ITaskList* pDestTaskFile, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey)
{
	auto importer = gcnew TodoTxtImporter(gcnew Translator(m_pTT));
	
	if (importer->Import(gcnew String(szSrcFilePath),
						 gcnew TaskList(pDestTaskFile),
						 ((dwFlags & IIEF_SILENT) != 0),
						 gcnew Preferences(pPrefs),
						 gcnew String(szKey)))
	{
		return IIER_SUCCESS;
	}

	// else
	return IIER_OTHER;
}
