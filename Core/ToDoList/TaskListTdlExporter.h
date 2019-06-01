// TaskListCsvExporter.h: interface for the CTaskListTdlExporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKLISTTDLEXPORTER_H__1FA1C0DA_B381_4F41_BCAB_CD1BB6FF3FC6__INCLUDED_)
#define AFX_TASKLISTTDLEXPORTER_H__1FA1C0DA_B381_4F41_BCAB_CD1BB6FF3FC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tdcenum.h"
#include "taskfile.h"

#include "..\Interfaces\Itasklist.h"
#include "..\Interfaces\IImportExport.h"

//////////////////////////////////////////////////////////////////////

static const LPCTSTR TDLEXPORT_TYPEID = _T("1FA1C0DA-B381-4F41-BCAB-CD1BB6FF3FC6");

//////////////////////////////////////////////////////////////////////

class CTaskListTdlExporter : public IExportTasklist  
{
public:
	CTaskListTdlExporter();
	virtual ~CTaskListTdlExporter();

    void Release() { delete this; }
	void SetLocalizer(ITransText* /*pTT*/) {}

	LPCTSTR GetMenuText() const { return _T("ToDoList"); }
	LPCTSTR GetFileFilter() const { return _T("Tasklists (*.tdl)|*.tdl||"); }
	LPCTSTR GetFileExtension() const { return _T("tdl"); }
	LPCTSTR GetTypeID() const { return TDLEXPORT_TYPEID; }
	HICON GetIcon() const { return NULL; }

	IIMPORTEXPORT_RESULT Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey);
	IIMPORTEXPORT_RESULT Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	static void CreateReverseFileLinks(CTaskFile& tasks, HTASKITEM hTask, const CID2IDMap& mapIDs, const CString& sFileName, BOOL bAndSiblings);
	static void FixupInternalDependencies(CTaskFile& tasks, HTASKITEM hTask, const CID2IDMap& mapIDs, BOOL bAndSiblings);
	static void BuildReverseIDMap(const CID2IDMap& mapIDs, CID2IDMap& mapReverseIDs);
};

#endif // !defined(AFX_TASKLISTTDLEXPORTER_H__1FA1C0DA_B381_4F41_BCAB_CD1BB6FF3FC6__INCLUDED_)
