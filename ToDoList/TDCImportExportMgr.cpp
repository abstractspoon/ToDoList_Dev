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
	if (FUNCTION(pSrcTasks, szTempFile, bSilent, pPrefs))					\
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

BOOL CTDCImportExportMgr::ExportTaskListToHtml(const ITaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent, IPreferences* pPrefs) const
{
	return ExportTaskList(pSrcTasks, szDestFile, EXPTOHTML, bSilent, pPrefs);
}

CString CTDCImportExportMgr::ExportTaskListToHtml(const ITaskList* pSrcTasks, BOOL bSilent, IPreferences* pPrefs) const
{
	ExportTaskListTo(ExportTaskListToHtml)
	return sFile;
}

BOOL CTDCImportExportMgr::ExportTaskListsToHtml(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent, IPreferences* pPrefs) const
{
	return ExportTaskLists(pSrcTasks, szDestFile, EXPTOHTML, bSilent, pPrefs);
}

CString CTDCImportExportMgr::ExportTaskListsToHtml(const IMultiTaskList* pSrcTasks, BOOL bSilent, IPreferences* pPrefs) const
{
	ExportTaskListTo(ExportTaskListsToHtml)
	return sFile;
}

BOOL CTDCImportExportMgr::ExportTaskListToText(const ITaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent, IPreferences* pPrefs) const
{
	return ExportTaskList(pSrcTasks, szDestFile, EXPTOTXT, bSilent, pPrefs);
}

CString CTDCImportExportMgr::ExportTaskListToText(const ITaskList* pSrcTasks, BOOL bSilent, IPreferences* pPrefs) const
{
	ExportTaskListTo(ExportTaskListToText)
	return sFile;
}

BOOL CTDCImportExportMgr::ExportTaskListsToText(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent, IPreferences* pPrefs) const
{
	return ExportTaskLists(pSrcTasks, szDestFile, EXPTOTXT, bSilent, pPrefs);
}

CString CTDCImportExportMgr::ExportTaskListsToText(const IMultiTaskList* pSrcTasks, BOOL bSilent, IPreferences* pPrefs) const
{
	ExportTaskListTo(ExportTaskListsToText)
	return sFile;
}

BOOL CTDCImportExportMgr::ExportTaskListToCsv(const ITaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent, IPreferences* pPrefs) const
{
	return ExportTaskList(pSrcTasks, szDestFile, EXPTOCSV, bSilent, pPrefs);
}

CString CTDCImportExportMgr::ExportTaskListToCsv(const ITaskList* pSrcTasks, BOOL bSilent, IPreferences* pPrefs) const
{
	ExportTaskListTo(ExportTaskListToCsv)
	return sFile;
}

BOOL CTDCImportExportMgr::ExportTaskListsToCsv(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent, IPreferences* pPrefs) const
{
	return ExportTaskLists(pSrcTasks, szDestFile, EXPTOCSV, bSilent, pPrefs);
}

CString CTDCImportExportMgr::ExportTaskListsToCsv(const IMultiTaskList* pSrcTasks, BOOL bSilent, IPreferences* pPrefs) const
{
	ExportTaskListTo(ExportTaskListsToCsv)
	return sFile;
}

BOOL CTDCImportExportMgr::ImportTaskListFromCsv(LPCTSTR szSrcFile, ITaskList* pDestTasks, BOOL bSilent, IPreferences* pPrefs) const
{
	return ImportTaskList(szSrcFile, pDestTasks, IMPFROMCSV, bSilent, pPrefs);
}

BOOL CTDCImportExportMgr::ExportTaskListToTdl(const ITaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent, IPreferences* pPrefs) const
{
	return ExportTaskList(pSrcTasks, szDestFile, EXPTOTDL, bSilent, pPrefs);
}

CString CTDCImportExportMgr::ExportTaskListToTdl(const ITaskList* pSrcTasks, BOOL bSilent, IPreferences* pPrefs) const
{
	ExportTaskListTo(ExportTaskListToTdl)
	return sFile;
}

BOOL CTDCImportExportMgr::ExportTaskListsToTdl(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent, IPreferences* pPrefs) const
{
	return ExportTaskLists(pSrcTasks, szDestFile, EXPTOTDL, bSilent, pPrefs);
}

CString CTDCImportExportMgr::ExportTaskListsToTdl(const IMultiTaskList* pSrcTasks, BOOL bSilent, IPreferences* pPrefs) const
{
	ExportTaskListTo(ExportTaskListsToTdl)
	return sFile;
}

BOOL CTDCImportExportMgr::ImportTaskListFromTdl(LPCTSTR szSrcFile, ITaskList* pDestTasks, BOOL bSilent, IPreferences* pPrefs) const
{
	return ImportTaskList(szSrcFile, pDestTasks, IMPFROMTDL, bSilent, pPrefs);
}

BOOL CTDCImportExportMgr::ImportTaskListFromOutlook(LPCTSTR szSrcFile, ITaskList* pDestTasks, BOOL bSilent, IPreferences* pPrefs) const
{
	return ImportTaskList(szSrcFile, pDestTasks, IMPFROMOUTLOOK, bSilent, pPrefs);
}

BOOL CTDCImportExportMgr::ExportTaskList(const ITaskList* pSrcTasks, LPCTSTR szDestFile, int nByExporter, BOOL bSilent, IPreferences* pPrefs) const
{
	CPreferences prefs;

	if (pPrefs == NULL)
		pPrefs = prefs;
	
	return CImportExportMgr::ExportTaskList(pSrcTasks, szDestFile, nByExporter, bSilent, pPrefs);
}

BOOL CTDCImportExportMgr::ExportTaskLists(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, int nByExporter, BOOL bSilent, IPreferences* pPrefs) const
{
	CPreferences prefs;

	if (pPrefs == NULL)
		pPrefs = prefs;
	
	return CImportExportMgr::ExportTaskLists(pSrcTasks, szDestFile, nByExporter, bSilent, pPrefs);
}

IIMPORT_RESULT CTDCImportExportMgr::ImportTaskList(LPCTSTR szSrcFile, ITaskList* pDestTasks, int nByImporter, BOOL bSilent, IPreferences* pPrefs) const
{
	CPreferences prefs;

	if (pPrefs == NULL)
		pPrefs = prefs;

	// call base class and then fixup default attributes
	IIMPORT_RESULT nRes = CImportExportMgr::ImportTaskList(szSrcFile, pDestTasks, nByImporter, bSilent, pPrefs);

	if (nRes == IIR_SUCCESS)
		SetTaskAttributesToDefaults(pDestTasks, pDestTasks->GetFirstTask(), TRUE);

	return nRes;
}

void CTDCImportExportMgr::SetTaskAttributesToDefaults(ITaskList* pTasks, HTASKITEM hTask, BOOL bAndSiblings) const
{
	if (!hTask)
		return;

	ASSERT(pTasks);
	ITaskList9* pTasks9 = GetITLInterface<ITaskList9>(pTasks, IID_TASKLIST9);

	if (!pTasks9->TaskHasAttribute(hTask, TDL_TASKCOLOR))
		pTasks9->SetTaskColor(hTask, m_tdiDefault.color);

	if (!pTasks9->TaskHasAttribute(hTask, TDL_TASKSTARTDATE) && CDateHelper::IsDateSet(m_tdiDefault.dateStart))
	{
		SYSTEMTIME st;
		m_tdiDefault.dateStart.GetAsSystemTime(st);

		pTasks9->SetTaskStartDate(hTask, CTime(st).GetTime());
	}
	
	if (!pTasks9->TaskHasAttribute(hTask, TDL_TASKALLOCBY))
		pTasks9->SetTaskAllocatedBy(hTask, m_tdiDefault.sAllocBy);
	
	if (!pTasks9->TaskHasAttribute(hTask, TDL_TASKSTATUS))
		pTasks9->SetTaskStatus(hTask, m_tdiDefault.sStatus);
	
	if (!pTasks9->TaskHasAttribute(hTask, TDL_TASKTIMEESTIMATE))
		pTasks9->SetTaskTimeEstimate(hTask, m_tdiDefault.dTimeEstimate, m_tdiDefault.nTimeEstUnits);
	
	if (!pTasks9->TaskHasAttribute(hTask, TDL_TASKTIMESPENT))
		pTasks9->SetTaskTimeSpent(hTask, m_tdiDefault.dTimeSpent, m_tdiDefault.nTimeSpentUnits);
	
	if (!pTasks9->TaskHasAttribute(hTask, TDL_TASKCREATEDBY))
		pTasks9->SetTaskCreatedBy(hTask, m_tdiDefault.sCreatedBy);
	
	if (!pTasks9->TaskHasAttribute(hTask, TDL_TASKCOST))
		pTasks9->SetTaskCost(hTask, m_tdiDefault.dCost);
	
	if (!pTasks9->TaskHasAttribute(hTask, TDL_TASKCATEGORY))
	{
		int nCat = m_tdiDefault.aCategories.GetSize();

		while (nCat--)
			pTasks9->AddTaskCategory(hTask, m_tdiDefault.aCategories[nCat]);
	}
	
	if (!pTasks9->TaskHasAttribute(hTask, TDL_TASKTAG))
	{
		int nTag = m_tdiDefault.aTags.GetSize();

		while (nTag--)
			pTasks9->AddTaskTag(hTask, m_tdiDefault.aTags[nTag]);
	}
	
	if (!pTasks9->TaskHasAttribute(hTask, TDL_TASKALLOCTO))
	{
		int nAlloc = m_tdiDefault.aAllocTo.GetSize();

		while (nAlloc--)
			pTasks9->AddTaskAllocatedTo(hTask, m_tdiDefault.aAllocTo[nAlloc]);
	}
	
	if (!pTasks9->TaskHasAttribute(hTask, TDL_TASKCOMMENTSTYPE))
		pTasks9->SetTaskAttribute(hTask, TDL_TASKCOMMENTSTYPE, m_tdiDefault.sCommentsTypeID);
	
	if (!pTasks9->TaskHasAttribute(hTask, TDL_TASKPRIORITY))
		pTasks9->SetTaskPriority(hTask, m_tdiDefault.nPriority);
	
	if (!pTasks9->TaskHasAttribute(hTask, TDL_TASKRISK))
		pTasks9->SetTaskRisk(hTask, m_tdiDefault.nRisk);

	// first child
	SetTaskAttributesToDefaults(pTasks9, pTasks9->GetFirstTask(hTask), TRUE);

	// handle sibling tasks WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasks9->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == don't recurse on siblings
			SetTaskAttributesToDefaults(pTasks9, hSibling, FALSE);
			
			hSibling = pTasks9->GetNextTask(hSibling);
		}
	}
}

