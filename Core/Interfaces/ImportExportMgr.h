// ImportExportMgr.h: interface for the CImportExportMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMPORTEXPORTMGR_H__C258D849_69ED_46A7_A2F0_351C5C9FB3B3__INCLUDED_)
#define AFX_IMPORTEXPORTMGR_H__C258D849_69ED_46A7_A2F0_351C5C9FB3B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "..\Interfaces\itasklist.h"
#include "..\Interfaces\iimportexport.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

class CImportExportMgr  
{
public:
	CImportExportMgr();
	virtual ~CImportExportMgr();
	
	virtual void Release();
	void UpdateLocalizer();
	BOOL IsInitialised() const { return m_bInitialized; }

	int GetNumImporters() const;
	int GetNumExporters() const;
	
	CString GetImporterMenuText(int nImporter) const;
	CString GetImporterFileExtension(int nImporter, BOOL bWithDot) const;
	BOOL ImporterHasFileExtension(int nImporter) const;
	CString GetImporterFileFilter(int nImporter) const;
	CString GetImporterTypeID(int nImporter) const;
	HICON GetImporterIcon(int nImporter) const;
	
	CString GetExporterMenuText(int nExporter) const;
	CString GetExporterFileExtension(int nExporter, BOOL bWithDot) const;
	BOOL ExporterHasFileExtension(int nExporter, LPCTSTR szExt = NULL) const;
	CString GetExporterFileFilter(int nExporter) const;
	CString GetExporterTypeID(int nExporter) const;
	HICON GetExporterIcon(int nExporter) const;
	
	IIMPORTEXPORT_RESULT ImportTaskList(LPCTSTR szSrcFile, ITaskList* pDestTasks, int nByImporter, BOOL bSilent, IPreferences* pPrefs) const;
	IIMPORTEXPORT_RESULT ExportTaskList(const ITaskList* pSrcTasks, LPCTSTR szDestFile, int nByExporter, BOOL bSilent, IPreferences* pPrefs) const;
	IIMPORTEXPORT_RESULT ExportTaskLists(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, int nByExporter, BOOL bSilent, IPreferences* pPrefs) const;
	
	int FindImporterByPath(LPCTSTR szFilePath) const;
	int FindExporterByPath(LPCTSTR szFilePath) const;
	int FindImporterByType(LPCTSTR szTypeID) const;
	int FindExporterByType(LPCTSTR szTypeID) const;
	
protected:
	BOOL m_bInitialized;
	CArray<IImportTasklist*, IImportTasklist*> m_aImporters;
	CArray<IExportTasklist*, IExportTasklist*> m_aExporters;
	
protected:
	virtual void Initialize() const;

	BOOL AddImporter(IImportTasklist* pImporter, int nPos = -1);
	BOOL AddExporter(IExportTasklist* pExporter, int nPos = -1);

	CString GetExporterPreferenceKey(int nExporter) const;
	CString GetImporterPreferenceKey(int nImporter) const;
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_IMPORTEXPORTMGR_H__C258D849_69ED_46A7_A2F0_351C5C9FB3B3__INCLUDED_)
