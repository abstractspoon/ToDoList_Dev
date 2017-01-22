// TaskListTxtExporter.h: interface for the CTaskListCsvExporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKLISTCSVEXPORTER2_H__CF68988D_FBBD_431D_BB56_464E8737D993__INCLUDED_)
#define AFX_TASKLISTCSVEXPORTER2_H__CF68988D_FBBD_431D_BB56_464E8737D993__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TaskListExporterBase.h"
#include "TDLCsvImportExportDlg.h"

#include "..\Interfaces\Itasklist.h"
#include "..\Interfaces\IImportExport.h"

class CTaskListCsvExporter : public IExportTasklist, protected CTaskListExporterBase  
{
public:
	CTaskListCsvExporter();
	virtual ~CTaskListCsvExporter();

    void Release() { delete this; }
	void SetLocalizer(ITransText* /*pTT*/) {}

	LPCTSTR GetMenuText() const { return _T("Spreadsheet"); }
	LPCTSTR GetFileFilter() const { return _T("Spreadsheet Files (*.csv)|*.csv||"); }
	LPCTSTR GetFileExtension() const { return _T("csv"); }
	HICON GetIcon() const { return NULL; }

	bool Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey);
   	bool Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	CString DELIM, INDENT;
	CTDCAttributeMapping m_aColumnMapping;
	BOOL m_bExportingForExcel;

protected:
	// base-class overrides
	virtual CString ExportTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int nDepth) const;
	virtual bool ExportOutput(LPCTSTR szDestFilePath, const CString& sOutput) const;

	virtual CString FormatAttribute(TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel, const CString& sValue) const;
	virtual CString FormatAttribute(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int nDepth, TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel) const;
	virtual CString FormatHeaderItem(TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel) const;
	virtual CString FormatHeader(const ITASKLISTBASE* pTasks) const;

	virtual bool InitConsts(const ITASKLISTBASE* pTasks, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	void CheckAddIDField(TDC_ATTRIBUTE nAttrib);
};

#endif // !defined(AFX_TASKLISTTXTEXPORTER_H__CF68988D_FBBD_431D_BB56_464E8737D993__INCLUDED_)
