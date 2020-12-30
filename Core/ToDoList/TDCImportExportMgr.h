// TDImportExportMgr.h: interface for the CTDLImportExportMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDIMPORTEXPORTMGR_H__0810C7FB_A895_4035_BEFA_B7097ABC2A7B__INCLUDED_)
#define AFX_TDIMPORTEXPORTMGR_H__0810C7FB_A895_4035_BEFA_B7097ABC2A7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "..\Interfaces\ImportExportMgr.h"

//////////////////////////////////////////////////////////////////////

class CTaskFile;
class CMultiTaskFile;

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

enum // Extended export flags
{
	TDCEF_HTMLWRAP	= 0x00010000,
	TDCEF_HTMLTABLE	= 0X00020000,
	TDCEF_HTMLPARA	= 0X00040000,
};

/////////////////////////////////////////////////////////////////////

class CTDCImportExportMgr : public CImportExportMgr  
{
public:
	CTDCImportExportMgr();
	virtual ~CTDCImportExportMgr();

	BOOL ImportTaskList(LPCTSTR szSrcFile, CTaskFile& tasksDest, TDC_IMPORT_TASKLIST nFormat, DWORD dwFlags = 0) const;
	BOOL ExportTaskList(const CTaskFile& tasksSrc, LPCTSTR szDestFile, TDC_EXPORT_TASKLIST nFormat, DWORD dwFlags = 0) const;
	BOOL ExportTaskLists(const CMultiTaskFile& tasksSrc, LPCTSTR szDestFile, TDC_EXPORT_TASKLIST nFormat, DWORD dwFlags = 0) const;
	CString ExportTaskList(const CTaskFile& tasksSrc, TDC_EXPORT_TASKLIST nFormat, DWORD dwFlags = 0) const;
	CString ExportTaskLists(const CMultiTaskFile& tasksSrc, TDC_EXPORT_TASKLIST nFormat, DWORD dwFlags = 0) const;

	IIMPORTEXPORT_RESULT ImportTaskList(LPCTSTR szSrcFile, CTaskFile& tasksDest, int nByImporter, DWORD dwFlags) const; 
	IIMPORTEXPORT_RESULT ExportTaskList(const CTaskFile& tasksSrc, LPCTSTR szDestFile, int nByExporter, DWORD dwFlags, IPreferences* pPrefs = NULL) const;
	IIMPORTEXPORT_RESULT ExportTaskLists(const CMultiTaskFile& tasksSrc, LPCTSTR szDestFile, int nByExporter, DWORD dwFlags, IPreferences* pPrefs = NULL) const;

	IIMPORTEXPORT_RESULT ImportTaskList(LPCTSTR szSrcFile, CTaskFile& tasksDest, LPCTSTR szTypeID, DWORD dwFlags) const; 
	IIMPORTEXPORT_RESULT ExportTaskList(const CTaskFile& tasksSrc, LPCTSTR szDestFile, LPCTSTR szTypeID, DWORD dwFlags, IPreferences* pPrefs = NULL) const;
	IIMPORTEXPORT_RESULT ExportTaskLists(const CMultiTaskFile& tasksSrc, LPCTSTR szDestFile, LPCTSTR szTypeID, DWORD dwFlags, IPreferences* pPrefs = NULL) const;

	int GetImporter(TDC_IMPORT_TASKLIST nFormat) const;
	int GetExporter(TDC_EXPORT_TASKLIST nFormat) const;

	static LPCTSTR GetTypeID(TDC_IMPORT_TASKLIST nFormat);
	static LPCTSTR GetTypeID(TDC_EXPORT_TASKLIST nFormat);

	static BOOL IsFormat(LPCTSTR szTypeID, TDC_IMPORT_TASKLIST nFormat);
	static BOOL IsFormat(LPCTSTR szTypeID, TDC_EXPORT_TASKLIST nFormat);

protected:
	void Initialize() const;

	CString ExportTaskList(const CTaskFile& tasksSrc, LPCTSTR szTypeID, DWORD dwFlags) const;
	CString ExportTaskLists(const CMultiTaskFile& tasksSrc, LPCTSTR szTypeID, DWORD dwFlags) const;
};

#endif // !defined(AFX_TDIMPORTEXPORTMGR_H__0810C7FB_A895_4035_BEFA_B7097ABC2A7B__INCLUDED_)
