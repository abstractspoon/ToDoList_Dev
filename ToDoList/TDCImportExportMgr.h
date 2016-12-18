// TDImportExportMgr.h: interface for the CTDLImportExportMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDIMPORTEXPORTMGR_H__0810C7FB_A895_4035_BEFA_B7097ABC2A7B__INCLUDED_)
#define AFX_TDIMPORTEXPORTMGR_H__0810C7FB_A895_4035_BEFA_B7097ABC2A7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToDoitem.h"

#include "..\SHARED\ImportExportMgr.h"

enum { EXPTOHTML, EXPTOTXT, EXPTOCSV, EXPTOTDL };
enum { IMPFROMCSV, IMPFROMTDL, IMPFROMOUTLOOK };

class CTDCImportExportMgr : public CImportExportMgr  
{
public:
	CTDCImportExportMgr();
	virtual ~CTDCImportExportMgr();

	BOOL ExportTaskListToHtml(const ITaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent = FALSE, IPreferences* pPrefs = NULL) const;
	BOOL ExportTaskListsToHtml(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent = FALSE, IPreferences* pPrefs = NULL) const;
	CString ExportTaskListToHtml(const ITaskList* pSrcTasks, BOOL bSilent = FALSE, IPreferences* pPrefs = NULL) const;
	CString ExportTaskListsToHtml(const IMultiTaskList* pSrcTasks, BOOL bSilent = FALSE, IPreferences* pPrefs = NULL) const;

	BOOL ExportTaskListToText(const ITaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent = FALSE, IPreferences* pPrefs = NULL) const;
	BOOL ExportTaskListsToText(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent = FALSE, IPreferences* pPrefs = NULL) const;
	CString ExportTaskListToText(const ITaskList* pSrcTasks, BOOL bSilent = FALSE, IPreferences* pPrefs = NULL) const;
	CString ExportTaskListsToText(const IMultiTaskList* pSrcTasks, BOOL bSilent = FALSE, IPreferences* pPrefs = NULL) const;

	BOOL ExportTaskListToCsv(const ITaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent = FALSE, IPreferences* pPrefs = NULL) const;
	BOOL ExportTaskListsToCsv(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent = FALSE, IPreferences* pPrefs = NULL) const;
	CString ExportTaskListToCsv(const ITaskList* pSrcTasks, BOOL bSilent = FALSE, IPreferences* pPrefs = NULL) const;
	CString ExportTaskListsToCsv(const IMultiTaskList* pSrcTasks, BOOL bSilent = FALSE, IPreferences* pPrefs = NULL) const;

	BOOL ExportTaskListToTdl(const ITaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent = FALSE, IPreferences* pPrefs = NULL) const;
	BOOL ExportTaskListsToTdl(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent = FALSE, IPreferences* pPrefs = NULL) const;
	CString ExportTaskListToTdl(const ITaskList* pSrcTasks, BOOL bSilent = FALSE, IPreferences* pPrefs = NULL) const;
	CString ExportTaskListsToTdl(const IMultiTaskList* pSrcTasks, BOOL bSilent = FALSE, IPreferences* pPrefs = NULL) const;

	BOOL ImportTaskListFromCsv(LPCTSTR szSrcFile, ITaskList* pDestTasks, BOOL bSilent = FALSE, IPreferences* pPrefs = NULL) const;
	BOOL ImportTaskListFromTdl(LPCTSTR szSrcFile, ITaskList* pDestTasks, BOOL bSilent = FALSE, IPreferences* pPrefs = NULL) const;
	BOOL ImportTaskListFromOutlook(LPCTSTR szSrcFile, ITaskList* pDestTasks, BOOL bSilent = FALSE, IPreferences* pPrefs = NULL) const;

	// override base class so we can set default task attributes
	IIMPORT_RESULT ImportTaskList(LPCTSTR szSrcFile, ITaskList* pDestTasks, int nByImporter, BOOL bSilent = FALSE, IPreferences* pPrefs = NULL) const; 
	BOOL ExportTaskList(const ITaskList* pSrcTasks, LPCTSTR szDestFile, int nByExporter, BOOL bSilent, IPreferences* pPrefs = NULL) const;
	BOOL ExportTaskLists(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, int nByExporter, BOOL bSilent, IPreferences* pPrefs = NULL) const;

	void SetDefaultTaskAttributes(const TODOITEM& tdi) { m_tdiDefault = tdi; }

protected:
	TODOITEM m_tdiDefault; // default attributes

protected:
	void SetTaskAttributesToDefaults(ITaskList* pTasks, HTASKITEM hTask, BOOL bAndSiblings) const;
	void Initialize() const;

};

#endif // !defined(AFX_TDIMPORTEXPORTMGR_H__0810C7FB_A895_4035_BEFA_B7097ABC2A7B__INCLUDED_)
