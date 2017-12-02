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

///////////////////////////////////////////////////////////////////////

#define ExportTaskListTo(FUNCTION)											\
																			\
	TCHAR szTempFile[MAX_PATH+1] = { 0 }, szTempPath[MAX_PATH+1] = { 0 };	\
																			\
	if (!::GetTempPath(MAX_PATH, szTempPath) ||								\
		!::GetTempFileName(szTempPath, _T("tdl"), 0, szTempFile))			\
		return _T("");														\
																			\
	CString sFile;															\
																			\
	if (FUNCTION(pSrcTasks, szTempFile, bSilent) == IIER_SUCCESS)			\
		FileMisc::LoadFile(szTempFile, sFile);								\
																			\
	FileMisc::DeleteFile(szTempFile, TRUE);									

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
	// add html and text exporters first
	if (!m_bInitialized)
	{
		CImportExportMgr::Initialize();
		
		// we need a non-const pointer to update the array
		CTDCImportExportMgr* pMgr = const_cast<CTDCImportExportMgr*>(this);

		// exporters
		pMgr->m_aExporters.InsertAt(EXPTOHTML, new CTaskListHtmlExporter);
		pMgr->m_aExporters.InsertAt(EXPTOTXT, new CTaskListTxtExporter);
		pMgr->m_aExporters.InsertAt(EXPTOCSV, new CTaskListCsvExporter);
		pMgr->m_aExporters.InsertAt(EXPTOTDL, new CTaskListTdlExporter);

		// importers
		pMgr->m_aImporters.InsertAt(IMPFROMCSV, new CTaskListCsvImporter);
		pMgr->m_aImporters.InsertAt(IMPFROMTDL, new CTaskListTdlImporter);
		pMgr->m_aImporters.InsertAt(IMPFROMOUTLOOK, new CTaskListOutlookImporter);
	}
}

BOOL CTDCImportExportMgr::ExportTaskListToHtml(const ITaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent) const
{
	return (ExportTaskList(pSrcTasks, szDestFile, EXPTOHTML, bSilent == IIER_SUCCESS));
}

CString CTDCImportExportMgr::ExportTaskListToHtml(const ITaskList* pSrcTasks, BOOL bSilent) const
{
	ExportTaskListTo(ExportTaskListToHtml)
	return sFile;
}

BOOL CTDCImportExportMgr::ExportTaskListsToHtml(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent) const
{
	return (ExportTaskLists(pSrcTasks, szDestFile, EXPTOHTML, bSilent == IIER_SUCCESS));
}

CString CTDCImportExportMgr::ExportTaskListsToHtml(const IMultiTaskList* pSrcTasks, BOOL bSilent) const
{
	ExportTaskListTo(ExportTaskListsToHtml)
	return sFile;
}

BOOL CTDCImportExportMgr::ExportTaskListToText(const ITaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent) const
{
	return (ExportTaskList(pSrcTasks, szDestFile, EXPTOTXT, bSilent == IIER_SUCCESS));
}

CString CTDCImportExportMgr::ExportTaskListToText(const ITaskList* pSrcTasks, BOOL bSilent) const
{
	ExportTaskListTo(ExportTaskListToText)
	return sFile;
}

BOOL CTDCImportExportMgr::ExportTaskListsToText(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent) const
{
	return (ExportTaskLists(pSrcTasks, szDestFile, EXPTOTXT, bSilent) == IIER_SUCCESS);
}

CString CTDCImportExportMgr::ExportTaskListsToText(const IMultiTaskList* pSrcTasks, BOOL bSilent) const
{
	ExportTaskListTo(ExportTaskListsToText)
	return sFile;
}

BOOL CTDCImportExportMgr::ExportTaskListToCsv(const ITaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent) const
{
	return (ExportTaskList(pSrcTasks, szDestFile, EXPTOCSV, bSilent) == IIER_SUCCESS);
}

CString CTDCImportExportMgr::ExportTaskListToCsv(const ITaskList* pSrcTasks, BOOL bSilent) const
{
	ExportTaskListTo(ExportTaskListToCsv)
	return sFile;
}

BOOL CTDCImportExportMgr::ExportTaskListsToCsv(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent) const
{
	return (ExportTaskLists(pSrcTasks, szDestFile, EXPTOCSV, bSilent) == IIER_SUCCESS);
}

CString CTDCImportExportMgr::ExportTaskListsToCsv(const IMultiTaskList* pSrcTasks, BOOL bSilent) const
{
	ExportTaskListTo(ExportTaskListsToCsv)
	return sFile;
}

BOOL CTDCImportExportMgr::ImportTaskListFromCsv(LPCTSTR szSrcFile, ITaskList* pDestTasks, BOOL bSilent) const
{
	return (ImportTaskList(szSrcFile, pDestTasks, IMPFROMCSV, bSilent) == IIER_SUCCESS);
}

BOOL CTDCImportExportMgr::ExportTaskListToTdl(const ITaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent) const
{
	return (ExportTaskList(pSrcTasks, szDestFile, EXPTOTDL, bSilent) == IIER_SUCCESS);
}

CString CTDCImportExportMgr::ExportTaskListToTdl(const ITaskList* pSrcTasks, BOOL bSilent) const
{
	ExportTaskListTo(ExportTaskListToTdl)
	return sFile;
}

BOOL CTDCImportExportMgr::ExportTaskListsToTdl(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent) const
{
	return (ExportTaskLists(pSrcTasks, szDestFile, EXPTOTDL, bSilent) == IIER_SUCCESS);
}

CString CTDCImportExportMgr::ExportTaskListsToTdl(const IMultiTaskList* pSrcTasks, BOOL bSilent) const
{
	ExportTaskListTo(ExportTaskListsToTdl)
	return sFile;
}

BOOL CTDCImportExportMgr::ImportTaskListFromTdl(LPCTSTR szSrcFile, ITaskList* pDestTasks, BOOL bSilent) const
{
	return (ImportTaskList(szSrcFile, pDestTasks, IMPFROMTDL, bSilent) == IIER_SUCCESS);
}

BOOL CTDCImportExportMgr::ImportTaskListFromOutlook(LPCTSTR szSrcFile, ITaskList* pDestTasks, BOOL bSilent) const
{
	return (ImportTaskList(szSrcFile, pDestTasks, IMPFROMOUTLOOK, bSilent) == IIER_SUCCESS);
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

