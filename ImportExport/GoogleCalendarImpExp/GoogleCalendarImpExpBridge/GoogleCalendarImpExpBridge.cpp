// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "GoogleCalendarImpExpBridge.h"

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include "..\..\..\..\ToDoList_Dev\Interfaces\ITasklist.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\ITransText.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\IPreferences.h"

////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#	using <..\..\..\Debug\GoogleCalendarImpExpCore.dll>
#	using <..\..\..\Debug\PluginHelpers.dll> as_friend
#else
#	using <..\..\..\Release\GoogleCalendarImpExpCore.dll>
#	using <..\..\..\Release\PluginHelpers.dll> as_friend
#endif

using namespace GoogleCalendarImpExp;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CGoogleCalendarImportBridge::CGoogleCalendarImportBridge()
{
	return;
}

void CGoogleCalendarImportBridge::Release()
{
	delete this;
}

void CGoogleCalendarImportBridge::SetLocalizer(ITransText* /*pTT*/)
{
	// TODO
}

HICON CGoogleCalendarImportBridge::GetIcon(void) const
{
	// TODO
	return NULL;
}

LPCWSTR CGoogleCalendarImportBridge::GetMenuText() const
{
	return L"Google Calendar";
}

LPCWSTR CGoogleCalendarImportBridge::GetFileFilter() const
{
	return L"";
}

LPCWSTR CGoogleCalendarImportBridge::GetFileExtension() const
{
	return L"";
}

////////////////////////////////////////////////////////////////////////////////////////////////

IIMPORT_RESULT CGoogleCalendarImportBridge::Import(LPCWSTR szSrcFilePath, ITaskList* pDestTaskFile, bool bSilent, IPreferences* pPrefs, LPCWSTR szKey)
{
	// call into out sibling C# module to do the actual work
	msclr::auto_gcroot<GoogleCalendarImportCore^> impCore = gcnew GoogleCalendarImportCore();
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<TaskList^> destTasks = gcnew TaskList(pDestTaskFile);
	
	// do the import
	if (impCore->Import(gcnew String(szSrcFilePath), destTasks.get(), (bSilent != FALSE), prefs.get(), gcnew String(szKey)))
		return IIR_SUCCESS;

	// else
	return IIR_OTHER;
}

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CGoogleCalendarExportBridge::CGoogleCalendarExportBridge()
{
	return;
}

void CGoogleCalendarExportBridge::Release()
{
	delete this;
}

void CGoogleCalendarExportBridge::SetLocalizer(ITransText* /*pTT*/)
{
	// TODO
}

HICON CGoogleCalendarExportBridge::GetIcon(void) const
{
	// TODO
	return NULL;
}

LPCWSTR CGoogleCalendarExportBridge::GetMenuText() const
{
	return L"Google Calendar";
}

LPCWSTR CGoogleCalendarExportBridge::GetFileFilter() const
{
	return L"";
}

LPCWSTR CGoogleCalendarExportBridge::GetFileExtension() const
{
	return L"";
}

bool CGoogleCalendarExportBridge::Export(const ITaskList* pSrcTaskFile, LPCWSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCWSTR szKey)
{
	// call into out sibling C# module to do the actual work
	msclr::auto_gcroot<GoogleCalendarExportCore^> expCore = gcnew GoogleCalendarExportCore();
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<TaskList^> srcTasks = gcnew TaskList(pSrcTaskFile);

	// do the export
	return expCore->Export(srcTasks.get(), gcnew String(szDestFilePath), (bSilent != FALSE), prefs.get(), gcnew String(szKey));
}

bool CGoogleCalendarExportBridge::Export(const IMultiTaskList* pSrcTaskFile, LPCWSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCWSTR szKey)
{
	// TODO
	return false;
}
