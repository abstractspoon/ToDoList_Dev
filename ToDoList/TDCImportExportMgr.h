// TDImportExportMgr.h: interface for the CTDLImportExportMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDIMPORTEXPORTMGR_H__0810C7FB_A895_4035_BEFA_B7097ABC2A7B__INCLUDED_)
#define AFX_TDIMPORTEXPORTMGR_H__0810C7FB_A895_4035_BEFA_B7097ABC2A7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "ToDoitem.h"

#include "..\SHARED\ImportExportMgr.h"

//////////////////////////////////////////////////////////////////////

enum TDC_EXPORT_TASKLIST
{
	TDCET_HTML,
	TDCET_TXT,
	TDCET_CSV,
	TDCET_TDL,
};

enum TDC_IMPORT_TASKLIST
{
	TDCIT_CSV,
	TDCIT_TDL,
	TDCIT_OUTLOOK,
};

/////////////////////////////////////////////////////////////////////

class CTDCImportExportMgr : public CImportExportMgr  
{
public:
	CTDCImportExportMgr();
	virtual ~CTDCImportExportMgr();

	BOOL ImportTaskList(LPCTSTR szSrcFile, ITaskList* pDestTasks, TDC_IMPORT_TASKLIST nFormat, BOOL bSilent = FALSE) const;
	BOOL ExportTaskList(const ITaskList* pSrcTasks, LPCTSTR szDestFile, TDC_EXPORT_TASKLIST nFormat, BOOL bSilent = FALSE) const;
	BOOL ExportTaskLists(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, TDC_EXPORT_TASKLIST nFormat, BOOL bSilent = FALSE) const;
	CString ExportTaskList(const ITaskList* pSrcTasks, TDC_EXPORT_TASKLIST nFormat, BOOL bSilent = FALSE) const;
	CString ExportTaskLists(const IMultiTaskList* pSrcTasks, TDC_EXPORT_TASKLIST nFormat, BOOL bSilent = FALSE) const;

	IIMPORTEXPORT_RESULT ImportTaskList(LPCTSTR szSrcFile, ITaskList* pDestTasks, int nByImporter, BOOL bSilent = FALSE) const; 
	IIMPORTEXPORT_RESULT ExportTaskList(const ITaskList* pSrcTasks, LPCTSTR szDestFile, int nByExporter, BOOL bSilent, IPreferences* pPrefs = NULL) const;
	IIMPORTEXPORT_RESULT ExportTaskLists(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, int nByExporter, BOOL bSilent, IPreferences* pPrefs = NULL) const;

	IIMPORTEXPORT_RESULT ImportTaskList(LPCTSTR szSrcFile, ITaskList* pDestTasks, LPCTSTR szTypeID, BOOL bSilent = FALSE) const; 
	IIMPORTEXPORT_RESULT ExportTaskList(const ITaskList* pSrcTasks, LPCTSTR szDestFile, LPCTSTR szTypeID, BOOL bSilent, IPreferences* pPrefs = NULL) const;
	IIMPORTEXPORT_RESULT ExportTaskLists(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, LPCTSTR szTypeID, BOOL bSilent, IPreferences* pPrefs = NULL) const;

	int GetImporter(TDC_IMPORT_TASKLIST nFormat) const;
	int GetExporter(TDC_EXPORT_TASKLIST nFormat) const;

	static LPCTSTR GetTypeID(TDC_IMPORT_TASKLIST nFormat);
	static LPCTSTR GetTypeID(TDC_EXPORT_TASKLIST nFormat);

	static BOOL IsFormat(LPCTSTR szTypeID, TDC_IMPORT_TASKLIST nFormat);
	static BOOL IsFormat(LPCTSTR szTypeID, TDC_EXPORT_TASKLIST nFormat);

protected:
	void Initialize() const;

	CString ExportTaskList(const ITaskList* pSrcTasks, LPCTSTR szTypeID, BOOL bSilent) const;
	CString ExportTaskLists(const IMultiTaskList* pSrcTasks, LPCTSTR szTypeID, BOOL bSilent) const;
};

#endif // !defined(AFX_TDIMPORTEXPORTMGR_H__0810C7FB_A895_4035_BEFA_B7097ABC2A7B__INCLUDED_)
