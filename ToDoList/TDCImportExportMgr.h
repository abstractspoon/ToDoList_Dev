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

	BOOL ExportTaskListToHtml(const ITaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent = FALSE) const;
	BOOL ExportTaskListsToHtml(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent = FALSE) const;
	CString ExportTaskListToHtml(const ITaskList* pSrcTasks, BOOL bSilent = FALSE) const;
	CString ExportTaskListsToHtml(const IMultiTaskList* pSrcTasks, BOOL bSilent = FALSE) const;

	BOOL ExportTaskListToText(const ITaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent = FALSE) const;
	BOOL ExportTaskListsToText(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent = FALSE) const;
	CString ExportTaskListToText(const ITaskList* pSrcTasks, BOOL bSilent = FALSE) const;
	CString ExportTaskListsToText(const IMultiTaskList* pSrcTasks, BOOL bSilent = FALSE) const;

	BOOL ExportTaskListToCsv(const ITaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent = FALSE) const;
	BOOL ExportTaskListsToCsv(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent = FALSE) const;
	CString ExportTaskListToCsv(const ITaskList* pSrcTasks, BOOL bSilent = FALSE) const;
	CString ExportTaskListsToCsv(const IMultiTaskList* pSrcTasks, BOOL bSilent = FALSE) const;

	BOOL ExportTaskListToTdl(const ITaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent = FALSE) const;
	BOOL ExportTaskListsToTdl(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, BOOL bSilent = FALSE) const;
	CString ExportTaskListToTdl(const ITaskList* pSrcTasks, BOOL bSilent = FALSE) const;
	CString ExportTaskListsToTdl(const IMultiTaskList* pSrcTasks, BOOL bSilent = FALSE) const;

	BOOL ImportTaskListFromCsv(LPCTSTR szSrcFile, ITaskList* pDestTasks, BOOL bSilent = FALSE) const;
	BOOL ImportTaskListFromTdl(LPCTSTR szSrcFile, ITaskList* pDestTasks, BOOL bSilent = FALSE) const;
	BOOL ImportTaskListFromOutlook(LPCTSTR szSrcFile, ITaskList* pDestTasks, BOOL bSilent = FALSE) const;

	// override base class so we can set default task attributes
	IIMPORT_RESULT ImportTaskList(LPCTSTR szSrcFile, ITaskList* pDestTasks, int nByImporter, BOOL bSilent = FALSE) const; 
	BOOL ExportTaskList(const ITaskList* pSrcTasks, LPCTSTR szDestFile, int nByExporter, BOOL bSilent) const;
	BOOL ExportTaskLists(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, int nByExporter, BOOL bSilent) const;

	void SetDefaultTaskAttributes(const TODOITEM& tdi) { m_tdiDefault = tdi; }

protected:
	TODOITEM m_tdiDefault; // default attributes

protected:
	void SetTaskAttributesToDefaults(ITaskList* pTasks, HTASKITEM hTask, BOOL bAndSiblings) const;
	void Initialize() const;

};

#endif // !defined(AFX_TDIMPORTEXPORTMGR_H__0810C7FB_A895_4035_BEFA_B7097ABC2A7B__INCLUDED_)
