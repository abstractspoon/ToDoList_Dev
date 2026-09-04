// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ToDoTxtImpExpBridge.h"

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include <Interfaces\ITasklist.h>
#include <Interfaces\ITransText.h>
#include <Interfaces\IPreferences.h>

////////////////////////////////////////////////////////////////////////////////////////////////

#using <PluginHelpers.dll> as_friend

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace ToDoTxtImpExp;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CToDoTxtImpExpBridge::CToDoTxtImpExpBridge()
	:
	m_hIcon(NULL),
	m_pTT(nullptr)
{
	//m_hIcon = Win32::LoadHIcon(L"ToDoTxtImpExpBridge.dll", IDI_TODOTXT, 16, true);
}

void CToDoTxtImpExpBridge::Release()
{
	delete this;
}

void CToDoTxtImpExpBridge::SetLocalizer(ITransText* pTT)
{
	if (m_pTT == nullptr)
		m_pTT = pTT;
}

HICON CToDoTxtImpExpBridge::GetIcon() const
{
	return m_hIcon;
}

LPCWSTR CToDoTxtImpExpBridge::GetMenuText() const
{
	return L"todo.txt";
}

LPCWSTR CToDoTxtImpExpBridge::GetFileFilter() const
{
	return L"txt";
}

LPCWSTR CToDoTxtImpExpBridge::GetFileExtension() const
{
	return L"txt";
}

LPCWSTR CToDoTxtImpExpBridge::GetTypeID() const
{
	return L"E18EBEBB-4F06-476B-B3C9-069A6B25D521";
}

////////////////////////////////////////////////////////////////////////////////////////////////

// IExportTasklist
IIMPORTEXPORT_RESULT CToDoTxtImpExpBridge::Export(const ITaskList* pSrcTaskFile, LPCWSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey)
{
	auto exporter = gcnew ToDoTxtExporter(gcnew Translator(m_pTT));

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

IIMPORTEXPORT_RESULT CToDoTxtImpExpBridge::Export(const IMultiTaskList* pSrcTaskFile, LPCWSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey)
{
	// TODO
	return IIER_OTHER;
}

// IImportTasklist
IIMPORTEXPORT_RESULT CToDoTxtImpExpBridge::Import(LPCWSTR szSrcFilePath, ITaskList* pDestTaskFile, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey)
{
	auto importer = gcnew ToDoTxtImporter(gcnew Translator(m_pTT));
	
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
