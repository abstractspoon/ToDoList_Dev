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
	CString GetImporterFileExtension(int nImporter) const;
	BOOL ImporterHasFileExtension(int nImporter) const;
	CString GetImporterFileFilter(int nImporter) const;
	HICON GetImporterIcon(int nImporter) const;
	
	CString GetExporterMenuText(int nExporter) const;
	CString GetExporterFileExtension(int nExporter) const;
	BOOL ExporterHasFileExtension(int nExporter) const;
	CString GetExporterFileFilter(int nExporter) const;
	HICON GetExporterIcon(int nExporter) const;
	
	IIMPORT_RESULT ImportTaskList(LPCTSTR szSrcFile, ITaskList* pDestTasks, int nByImporter, BOOL bSilent, IPreferences* pPrefs) const;
	BOOL ExportTaskList(const ITaskList* pSrcTasks, LPCTSTR szDestFile, int nByExporter, BOOL bSilent, IPreferences* pPrefs) const;
	BOOL ExportTaskLists(const IMultiTaskList* pSrcTasks, LPCTSTR szDestFile, int nByExporter, BOOL bSilent, IPreferences* pPrefs) const;
	
	int FindImporter(LPCTSTR szFilePath);
	
protected:
	BOOL m_bInitialized;
	CArray<IImportTasklist*, IImportTasklist*> m_aImporters;
	CArray<IExportTasklist*, IExportTasklist*> m_aExporters;
	
protected:
	virtual void Initialize() const;

};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_IMPORTEXPORTMGR_H__C258D849_69ED_46A7_A2F0_351C5C9FB3B3__INCLUDED_)
