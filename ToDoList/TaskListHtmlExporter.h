// TaskFileHtmlExporter.h: interface for the CTaskListHtmlExporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKFILEHTMLEXPORTER_H__E4FD92AB_2BF2_40E3_9C8E_5018A72AEA89__INCLUDED_)
#define AFX_TASKFILEHTMLEXPORTER_H__E4FD92AB_2BF2_40E3_9C8E_5018A72AEA89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TaskListExporterBase.h"

#include "..\Interfaces\Itasklist.h"
#include "..\Interfaces\IImportExport.h"

class CTaskListHtmlExporter : public IExportTasklist, protected CTaskListExporterBase  
{
public:
	CTaskListHtmlExporter();
	virtual ~CTaskListHtmlExporter();

	void Release() { delete this; }
	void SetLocalizer(ITransText* /*pTT*/) {}

	LPCTSTR GetMenuText() const { return _T("Web Page"); }
	LPCTSTR GetFileFilter() const { return _T("Web Pages (*.html)|*.html||"); }
	LPCTSTR GetFileExtension() const { return _T("html"); }
	HICON GetIcon() const { return NULL; }

	bool Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey);
   	bool Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	// Pseudo-const variables
	CString CHARSET;
	CString DEFAULTFONT, HTMLNOTES;
	BOOL STRIKETHRUDONE;
	int EXPORTSTYLE;
	mutable BOOL ROOT;

protected:
	// base-class overrides
	virtual bool ExportOutput(LPCTSTR szDestFilePath, const CString& sOutput) const;
	virtual CString ExportTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int nDepth) const;
	virtual CString ExportSubtasks(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int nDepth) const;

	virtual CString FormatAttribute(TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel, const CString& sValue) const;
	virtual CString FormatAttribute(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int nDepth, TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel) const;

	virtual bool InitConsts(const ITASKLISTBASE* pTasks, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey);
	virtual CString GetSpaceForNotes() const { return HTMLNOTES; }

	virtual CString FormatTitle(const ITASKLISTBASE* pTasks) const;
	virtual CString FormatHeaderItem(TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel) const;
	virtual CString FormatHeader(const ITASKLISTBASE* pTasks) const;

	// non-virtual internal helper
	CString FormatAttribute(TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel, const CString& sValue, BOOL bEncodeVal) const;
};

#endif // !defined(AFX_TASKFILEHTMLEXPORTER_H__E4FD92AB_2BF2_40E3_9C8E_5018A72AEA89__INCLUDED_)
