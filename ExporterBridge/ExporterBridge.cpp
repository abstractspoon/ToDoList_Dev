// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include <unknwn.h>
#include <tchar.h>

#include "stdafx.h"
#include "ExporterBridge.h"

#include "..\shared\ITasklist.h"
#include "..\shared\ITransText.h"
#include "..\shared\IPreferences.h"

////////////////////////////////////////////////////////////////////////////////////////////////

#using <..\exporter\Debug\ExporterCore.dll>
#include <msclr\auto_gcroot.h>

#using <..\exporter\Debug\PluginHelpers.dll> as_friend

using namespace ExporterCore;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CExporterBridge::CExporterBridge()
{
	return;
}

void CExporterBridge::Release()
{
   delete this;
}

void CExporterBridge::SetLocalizer(ITransText* /*pTT*/)
{

}

LPCTSTR CExporterBridge::GetMenuText() const
{
   return L"Bridge";
}

LPCTSTR CExporterBridge::GetFileFilter() const
{
   return L"zzz";
}

LPCTSTR CExporterBridge::GetFileExtension() const
{
   return L"zzz";
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool CExporterBridge::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, BOOL bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
   // call into out sibling C# module to do the actual work
   msclr::auto_gcroot<Class1^> expCore = gcnew Class1();
   msclr::auto_gcroot<CPreferences^> prefs = gcnew CPreferences(pPrefs);
   msclr::auto_gcroot<CTaskList^> srcTasks = gcnew CTaskList(GetITLInterface<ITaskList13>(pSrcTaskFile, IID_TASKLIST13));
      
   // do the export
   return expCore->Export(srcTasks.get(), gcnew String(szDestFilePath), (bSilent != FALSE), prefs.get(), gcnew String(szKey));
}

bool CExporterBridge::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, BOOL bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
   return false;
}
