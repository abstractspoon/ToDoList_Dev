// TDImportExportMgr.cpp: implementation of the CTDLImportExportMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDCImportExportMgr.h"
#include "tasklisthtmlexporter.h"
#include "tasklisttxtexporter.h"
#include "tasklistcsvexporter.h"
#include "tasklistcsvimporter.h"
#include "tasklisttdlexporter.h"
#include "tasklisttdlimporter.h"
#include "tasklistoutlookimporter.h"
#include "MultiTaskFile.h"

#include "..\shared\filemisc.h"
#include "..\shared\datehelper.h"

#include "..\Interfaces\Preferences.h"
#include "..\Interfaces\TasklistSchemaDef.h"

///////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const LPCTSTR PREF_KEY = _T("Preferences");
const LPCTSTR EMPTY_STR = _T("");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCImportExportMgr::CTDCImportExportMgr()
{

}

CTDCImportExportMgr::~CTDCImportExportMgr()
{

}

void CTDCImportExportMgr::Initialize() const
{
	if (!m_bInitialized)
	{
		CImportExportMgr::Initialize();
		
		// we need a non-const pointer to update the array
		CTDCImportExportMgr* pMgr = const_cast<CTDCImportExportMgr*>(this);

		// exporters
		pMgr->AddExporter(new CTaskListHtmlExporter, TDCET_HTML);
		pMgr->AddExporter(new CTaskListTxtExporter, TDCET_TXT);
		pMgr->AddExporter(new CTaskListCsvExporter, TDCET_CSV);
		pMgr->AddExporter(new CTaskListTdlExporter, TDCET_TDL);

		// importers
		pMgr->AddImporter(new CTaskListCsvImporter, TDCIT_CSV);
		pMgr->AddImporter(new CTaskListTdlImporter, TDCIT_TDL);
		pMgr->AddImporter(new CTaskListOutlookImporter, TDCIT_OUTLOOK);
	}
}

BOOL CTDCImportExportMgr::ImportTaskList(LPCTSTR szSrcFile, CTaskFile& tasksDest, TDC_IMPORT_TASKLIST nFormat, DWORD dwFlags) const
{
	return ImportTaskList(szSrcFile, tasksDest, GetTypeID(nFormat), dwFlags);
}

BOOL CTDCImportExportMgr::ExportTaskList(const CTaskFile& tasksSrc, LPCTSTR szDestFile, TDC_EXPORT_TASKLIST nFormat, DWORD dwFlags) const
{
	return ExportTaskList(tasksSrc, szDestFile, GetTypeID(nFormat), dwFlags);
}

BOOL CTDCImportExportMgr::ExportTaskLists(const CMultiTaskFile& tasksSrc, LPCTSTR szDestFile, TDC_EXPORT_TASKLIST nFormat, DWORD dwFlags) const
{
	return ExportTaskLists(tasksSrc, szDestFile, GetTypeID(nFormat), dwFlags);
}

CString CTDCImportExportMgr::ExportTaskList(const CTaskFile& tasksSrc, TDC_EXPORT_TASKLIST nFormat, DWORD dwFlags) const
{
	return ExportTaskList(tasksSrc, GetTypeID(nFormat), dwFlags);
}

CString CTDCImportExportMgr::ExportTaskLists(const CMultiTaskFile& tasksSrc, TDC_EXPORT_TASKLIST nFormat, DWORD dwFlags) const
{
	return ExportTaskLists(tasksSrc, GetTypeID(nFormat), dwFlags);
}

CString CTDCImportExportMgr::ExportTaskList(const CTaskFile& tasksSrc, LPCTSTR szTypeID, DWORD dwFlags) const
{
	CString sTempFile = FileMisc::GetTempFilePath(), sFileContents;															

	if (ExportTaskList(tasksSrc, sTempFile, szTypeID, dwFlags) == IIER_SUCCESS)
		VERIFY(FileMisc::LoadFile(sTempFile, sFileContents));
	
	// Cleanup
	FileMisc::DeleteFile(sTempFile, TRUE);

	return sFileContents;
}

CString CTDCImportExportMgr::ExportTaskLists(const CMultiTaskFile& tasksSrc, LPCTSTR szTypeID, DWORD dwFlags) const
{
	CString sTempFile = FileMisc::GetTempFilePath(), sFileContents;															

	if (ExportTaskLists(tasksSrc, sTempFile, szTypeID, dwFlags) == IIER_SUCCESS)
		VERIFY(FileMisc::LoadFile(sTempFile, sFileContents));
	
	// Cleanup
	FileMisc::DeleteFile(sTempFile, TRUE);

	return sFileContents;
}

int CTDCImportExportMgr::GetImporter(TDC_IMPORT_TASKLIST nFormat) const
{
	return FindImporterByType(GetTypeID(nFormat));
}

int CTDCImportExportMgr::GetExporter(TDC_EXPORT_TASKLIST nFormat) const
{
	return FindExporterByType(GetTypeID(nFormat));
}

LPCTSTR CTDCImportExportMgr::GetTypeID(TDC_IMPORT_TASKLIST nFormat)
{
	switch (nFormat)
	{
	case TDCIT_CSV:		return CSVIMPORT_TYPEID;
	case TDCIT_TDL:		return TDLIMPORT_TYPEID;
	case TDCIT_OUTLOOK:	return OUTLOOKIMPORT_TYPEID;
	}

	return EMPTY_STR;
}

LPCTSTR CTDCImportExportMgr::GetTypeID(TDC_EXPORT_TASKLIST nFormat)
{
	switch (nFormat)
	{
	case TDCET_HTML:	return HTMLEXPORT_TYPEID;
	case TDCET_CSV:		return CSVEXPORT_TYPEID;
	case TDCET_TXT:		return TXTEXPORT_TYPEID;
	case TDCET_TDL:		return TDLEXPORT_TYPEID;
	}

	return EMPTY_STR;
}

BOOL CTDCImportExportMgr::IsFormat(LPCTSTR szTypeID, TDC_IMPORT_TASKLIST nFormat)
{
	return (CString(szTypeID).CompareNoCase(GetTypeID(nFormat)) == 0);
}

BOOL CTDCImportExportMgr::IsFormat(LPCTSTR szTypeID, TDC_EXPORT_TASKLIST nFormat)
{
	return (CString(szTypeID).CompareNoCase(GetTypeID(nFormat)) == 0);
}

IIMPORTEXPORT_RESULT CTDCImportExportMgr::ExportTaskList(const CTaskFile& tasksSrc, LPCTSTR szDestFile, int nByExporter, DWORD dwFlags, IPreferences* pPrefs) const
{
	CPreferences prefs;

	if (pPrefs == NULL)
		pPrefs = prefs;
	
	return CImportExportMgr::ExportTaskList(&tasksSrc, szDestFile, nByExporter, dwFlags, pPrefs);
}

IIMPORTEXPORT_RESULT CTDCImportExportMgr::ExportTaskLists(const CMultiTaskFile& tasksSrc, LPCTSTR szDestFile, int nByExporter, DWORD dwFlags, IPreferences* pPrefs) const
{
	CPreferences prefs;

	if (pPrefs == NULL)
		pPrefs = prefs;
	
	return CImportExportMgr::ExportTaskLists(&tasksSrc, szDestFile, nByExporter, dwFlags, pPrefs);
}

IIMPORTEXPORT_RESULT CTDCImportExportMgr::ImportTaskList(LPCTSTR szSrcFile, CTaskFile& tasksDest, int nByImporter, DWORD dwFlags) const
{
	CPreferences prefs;

	return CImportExportMgr::ImportTaskList(szSrcFile, &tasksDest, nByImporter, dwFlags, prefs);
}

IIMPORTEXPORT_RESULT CTDCImportExportMgr::ExportTaskList(const CTaskFile& tasksSrc, LPCTSTR szDestFile, LPCTSTR szTypeID, DWORD dwFlags, IPreferences* pPrefs) const
{
	int nByExporter = FindExporterByType(szTypeID);
	ASSERT(nByExporter != -1);

	return ExportTaskList(tasksSrc, szDestFile, nByExporter, dwFlags, pPrefs);
}

IIMPORTEXPORT_RESULT CTDCImportExportMgr::ExportTaskLists(const CMultiTaskFile& tasksSrc, LPCTSTR szDestFile, LPCTSTR szTypeID, DWORD dwFlags, IPreferences* pPrefs) const
{
	int nByExporter = FindExporterByType(szTypeID);
	ASSERT(nByExporter != -1);

	return ExportTaskLists(tasksSrc, szDestFile, nByExporter, dwFlags, pPrefs);
}

IIMPORTEXPORT_RESULT CTDCImportExportMgr::ImportTaskList(LPCTSTR szSrcFile, CTaskFile& tasksDest, LPCTSTR szTypeID, DWORD dwFlags) const
{
	int nByImporter = FindImporterByType(szTypeID);
	ASSERT(nByImporter != -1);

	return ImportTaskList(szSrcFile, tasksDest, nByImporter, dwFlags);
}

