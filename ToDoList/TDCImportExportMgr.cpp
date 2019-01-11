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
#include "tdlschemadef.h"

#include "..\shared\filemisc.h"
#include "..\shared\preferences.h"
#include "..\shared\datehelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const LPCTSTR PREF_KEY = _T("Preferences");
const LPCTSTR EMPTY_STR = _T("");

///////////////////////////////////////////////////////////////////////

#define ExportTaskListTo(FUNCTION)											\
																			\

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

BOOL CTDCImportExportMgr::ImportTaskList(LPCTSTR szSrcFile, ITaskList* pDestTasks, TDC_IMPORT_TASKLIST nFormat, BOOL bSilent) const
{
	return ImportTaskList(szSrcFile, pDestTasks, GetTypeID(nFormat), bSilent);
}

BOOL CTDCImportExportMgr::ExportTaskList(const ITaskList* pSrcTasks, LPCTSTR szDestFile, TDC_EXPORT_TASKLIST nFormat, BOOL bSilent) const
{
	return ExportTaskList(pSrcTasks, szDestFile, GetTypeID(nFormat), bSilent);
}

BOOL CTDCImportExportMgr::ExportTaskLists(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, TDC_EXPORT_TASKLIST nFormat, BOOL bSilent) const
{
	return ExportTaskLists(pSrcTasks, szDestFile, GetTypeID(nFormat), bSilent);
}

CString CTDCImportExportMgr::ExportTaskList(const ITaskList* pSrcTasks, TDC_EXPORT_TASKLIST nFormat, BOOL bSilent) const
{
	return ExportTaskList(pSrcTasks, GetTypeID(nFormat), bSilent);
}

CString CTDCImportExportMgr::ExportTaskLists(const IMultiTaskList* pSrcTasks, TDC_EXPORT_TASKLIST nFormat, BOOL bSilent) const
{
	return ExportTaskLists(pSrcTasks, GetTypeID(nFormat), bSilent);
}

CString CTDCImportExportMgr::ExportTaskList(const ITaskList* pSrcTasks, LPCTSTR szTypeID, BOOL bSilent) const
{
	CString sTempFile = FileMisc::GetTempFilePath(), sFileContents;															

	if (ExportTaskList(pSrcTasks, sTempFile, szTypeID, bSilent) == IIER_SUCCESS)
		VERIFY(FileMisc::LoadFile(sTempFile, sFileContents));
	
	// Cleanup
	FileMisc::DeleteFile(sTempFile, TRUE);

	return sFileContents;
}

CString CTDCImportExportMgr::ExportTaskLists(const IMultiTaskList* pSrcTasks, LPCTSTR szTypeID, BOOL bSilent) const
{
	CString sTempFile = FileMisc::GetTempFilePath(), sFileContents;															

	if (ExportTaskLists(pSrcTasks, sTempFile, szTypeID, bSilent) == IIER_SUCCESS)
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
	return CString(szTypeID).CompareNoCase(GetTypeID(nFormat));
}

BOOL CTDCImportExportMgr::IsFormat(LPCTSTR szTypeID, TDC_EXPORT_TASKLIST nFormat)
{
	return CString(szTypeID).CompareNoCase(GetTypeID(nFormat));
}

IIMPORTEXPORT_RESULT CTDCImportExportMgr::ExportTaskList(const ITaskList* pSrcTasks, LPCTSTR szDestFile, int nByExporter, BOOL bSilent, IPreferences* pPrefs) const
{
	CPreferences prefs;

	if (pPrefs == NULL)
		pPrefs = prefs;
	
	return CImportExportMgr::ExportTaskList(pSrcTasks, szDestFile, nByExporter, bSilent, pPrefs);
}

IIMPORTEXPORT_RESULT CTDCImportExportMgr::ExportTaskLists(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, int nByExporter, BOOL bSilent, IPreferences* pPrefs) const
{
	CPreferences prefs;

	if (pPrefs == NULL)
		pPrefs = prefs;
	
	return CImportExportMgr::ExportTaskLists(pSrcTasks, szDestFile, nByExporter, bSilent, pPrefs);
}

IIMPORTEXPORT_RESULT CTDCImportExportMgr::ImportTaskList(LPCTSTR szSrcFile, ITaskList* pDestTasks, int nByImporter, BOOL bSilent) const
{
	CPreferences prefs;

	return CImportExportMgr::ImportTaskList(szSrcFile, pDestTasks, nByImporter, bSilent, prefs);
}

IIMPORTEXPORT_RESULT CTDCImportExportMgr::ExportTaskList(const ITaskList* pSrcTasks, LPCTSTR szDestFile, LPCTSTR szTypeID, BOOL bSilent, IPreferences* pPrefs) const
{
	int nByExporter = FindExporterByType(szTypeID);
	ASSERT(nByExporter != -1);

	return ExportTaskList(pSrcTasks, szDestFile, nByExporter, bSilent, pPrefs);
}

IIMPORTEXPORT_RESULT CTDCImportExportMgr::ExportTaskLists(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, LPCTSTR szTypeID, BOOL bSilent, IPreferences* pPrefs) const
{
	int nByExporter = FindExporterByType(szTypeID);
	ASSERT(nByExporter != -1);

	return ExportTaskLists(pSrcTasks, szDestFile, nByExporter, bSilent, pPrefs);
}

IIMPORTEXPORT_RESULT CTDCImportExportMgr::ImportTaskList(LPCTSTR szSrcFile, ITaskList* pDestTasks, LPCTSTR szTypeID, BOOL bSilent) const
{
	int nByImporter = FindImporterByType(szTypeID);
	ASSERT(nByImporter != -1);

	return ImportTaskList(szSrcFile, pDestTasks, nByImporter, bSilent);
}

