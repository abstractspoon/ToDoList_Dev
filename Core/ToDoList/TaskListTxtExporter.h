// TaskListTxtExporter.h: interface for the CTaskListTxtExporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKLISTTXTEXPORTER_H__CF68988D_FBBD_431D_BB56_464E8737D993__INCLUDED_)
#define AFX_TASKLISTTXTEXPORTER_H__CF68988D_FBBD_431D_BB56_464E8737D993__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TaskListExporterBase.h"

#include "..\Interfaces\Itasklist.h"
#include "..\Interfaces\IImportExport.h"

//////////////////////////////////////////////////////////////////////

static const LPCTSTR TXTEXPORT_TYPEID = _T("CF68988D_FBBD_431D_BB56_464E8737D993");

//////////////////////////////////////////////////////////////////////

class CTaskListTxtExporter : public IExportTasklist, protected CTaskListExporterBase  
{
public:
	CTaskListTxtExporter();
	virtual ~CTaskListTxtExporter();

    void Release() { delete this; }
	void SetLocalizer(ITransText* /*pTT*/) {}
	bool SupportsHtmlComments() const { return false; }
	HICON GetIcon() const { return NULL; }

	LPCTSTR GetMenuText() const { return _T("Plain Text"); }
	LPCTSTR GetFileFilter() const { return _T("Text Files (*.txt)|*.txt||"); }
	LPCTSTR GetFileExtension() const { return _T("txt"); }
	LPCTSTR GetTypeID() const { return TXTEXPORT_TYPEID; }

	IIMPORTEXPORT_RESULT Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);
   	IIMPORTEXPORT_RESULT Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	CString TEXTNOTES, INDENT;

protected:
	// base-class overrides
	virtual CString ExportTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int nDepth) const;

	virtual CString FormatAttribute(TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel, const CString& sValue) const;
	virtual CString FormatAttribute(const ITASKLISTBASE* pTasks, HTASKITEM hTask, int nDepth, TDC_ATTRIBUTE nAttrib, const CString& sAttribLabel) const;

	virtual bool InitConsts(const ITASKLISTBASE* pTasks, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);
	virtual CString GetSpaceForNotes() const { return TEXTNOTES; }
	virtual CString FormatTitle(const ITASKLISTBASE* pTasks) const;
};

#endif // !defined(AFX_TASKLISTTXTEXPORTER_H__CF68988D_FBBD_431D_BB56_464E8737D993__INCLUDED_)
