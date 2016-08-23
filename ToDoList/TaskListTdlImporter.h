// TaskListCsvImporter.h: interface for the CTaskListTdlImporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKLISTTDLIMPORTER_H__ADF211CB_FBD2_42A2_AD51_DFF58E566753__INCLUDED_)
#define AFX_TASKLISTTDLIMPORTER_H__ADF211CB_FBD2_42A2_AD51_DFF58E566753__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Interfaces\Itasklist.h"
#include "..\Interfaces\IImportExport.h"

class CTaskListTdlImporter : public IImportTasklist  
{
public:
	CTaskListTdlImporter();
	virtual ~CTaskListTdlImporter();

    void Release() { delete this; }
	void SetLocalizer(ITransText* /*pTT*/) {}

	LPCTSTR GetMenuText() const { return _T("ToDoList"); }
	LPCTSTR GetFileFilter() const { return _T("Tasklists (*.tdl)|*.tdl||"); }
	LPCTSTR GetFileExtension() const { return _T("tdl"); }
	HICON GetIcon() const { return NULL; }

	IIMPORT_RESULT Import(LPCTSTR szSrcFilePath, ITaskList* pDestTaskFile, BOOL bSilent, IPreferences* pPrefs, LPCTSTR szKey);

};

#endif // !defined(AFX_TASKLISTCSVImPORTER_H__ADF211CB_FBD2_42A2_AD51_DFF58E566753__INCLUDED_)
