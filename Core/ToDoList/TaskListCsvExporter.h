// TaskListTxtExporter.h: interface for the CTaskListCsvExporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKLISTCSVEXPORTER_H__7B199F98_746F_49DE_9C1D_3141281996F5__INCLUDED_)
#define AFX_TASKLISTCSVEXPORTER_H__7B199F98_746F_49DE_9C1D_3141281996F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "TaskListExporterBase.h"
#include "TDLCsvImportExportDlg.h"

#include "..\Interfaces\Itasklist.h"
#include "..\Interfaces\IImportExport.h"

//////////////////////////////////////////////////////////////////////

static const LPCTSTR CSVEXPORT_TYPEID = _T("7B199F98-746F-49DE-9C1D-3141281996F5");

//////////////////////////////////////////////////////////////////////

class CTaskListCsvExporter : public IExportTasklist, protected CTaskListExporterBase  
{
public:
	CTaskListCsvExporter();
	virtual ~CTaskListCsvExporter();

    void Release() { delete this; }
	void SetLocalizer(ITransText* /*pTT*/) {}
	bool SupportsHtmlComments() const { return false; }
	HICON GetIcon() const { return NULL; }

	LPCTSTR GetMenuText() const { return _T("Spreadsheet"); }
	LPCTSTR GetFileFilter() const { return _T("Spreadsheet Files (*.csv)|*.csv||"); }
	LPCTSTR GetFileExtension() const { return _T("csv"); }
	LPCTSTR GetTypeID() const { return CSVEXPORT_TYPEID; }

	IIMPORTEXPORT_RESULT Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);
   	IIMPORTEXPORT_RESULT Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	CString DELIM, INDENT;
	BOOL ISODATES;
	CTDCAttributeMapping m_aColumnMapping;
	BOOL m_bExportingForExcel;
	
	mutable BOOL m_bFirstHeader;

protected:
	// base-class overrides
	virtual CString ExportTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int nDepth) const;
	virtual IIMPORTEXPORT_RESULT ExportOutput(LPCTSTR szDestFilePath, const CString& sOutput) const;

	virtual CString FormatAttribute(TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel, const CString& sValue) const;
	virtual CString FormatAttribute(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int nDepth, TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel) const;
	virtual CString FormatHeaderItem(TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel) const;
	virtual CString FormatHeader(const ITASKLISTBASE* pTasks) const;

	virtual bool InitConsts(const ITASKLISTBASE* pTasks, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	void CheckAddIDFields();
	CString FormatDateWithSeconds(time64_t timeT) const;
};

#endif // !defined(AFX_TASKLISTCSVEXPORTER_H__7B199F98_746F_49DE_9C1D_3141281996F5__INCLUDED_)
