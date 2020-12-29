// TaskListCsvImporter.h: interface for the CTaskListTdlImporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKLISTTDLIMPORTER_H__81861E57_83AA_4618_8C75_AA7AF34F8F23__INCLUDED_)
#define AFX_TASKLISTTDLIMPORTER_H__81861E57_83AA_4618_8C75_AA7AF34F8F23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Shared\icon.h"

#include "..\Interfaces\Itasklist.h"
#include "..\Interfaces\IImportExport.h"

//////////////////////////////////////////////////////////////////////

static const LPCTSTR TDLIMPORT_TYPEID = _T("81861E57-83AA-4618-8C75-AA7AF34F8F23");

//////////////////////////////////////////////////////////////////////

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
	LPCTSTR GetTypeID() const { return TDLIMPORT_TYPEID; }
	HICON GetIcon() const { return m_icon; }

	IIMPORTEXPORT_RESULT Import(LPCTSTR szSrcFilePath, ITaskList* pDestTaskFile, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	CIcon m_icon;

};

#endif // !defined(AFX_TASKLISTTDLIMPORTER_H__81861E57_83AA_4618_8C75_AA7AF34F8F23__INCLUDED_)
