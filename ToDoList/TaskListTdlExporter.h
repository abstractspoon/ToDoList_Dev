// TaskListCsvExporter.h: interface for the CTaskListTdlExporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKLISTTDLEXPORTER_H__ADF211CB_FBD2_42A2_AD51_DFF58E566753__INCLUDED_)
#define AFX_TASKLISTTDLEXPORTER_H__ADF211CB_FBD2_42A2_AD51_DFF58E566753__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tdcenum.h"
#include "taskfile.h"

#include "..\Interfaces\Itasklist.h"
#include "..\Interfaces\IImportExport.h"

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
	HICON GetIcon() const { return NULL; }

	bool Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey);
	bool Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	static void CreateReverseFileLinks(CTaskFile& tasks, HTASKITEM hTask, const CID2IDMap& mapIDs, const CString& sFileName, BOOL bAndSiblings);
	static void CopyCustomDataDefinitions(const ITaskList* pTasks, CTDCCustomAttribDefinitionArray& aAttribDefs);
	static void FixupInternalDependencies(CTaskFile& tasks, HTASKITEM hTask, const CID2IDMap& mapIDs, BOOL bAndSiblings);
	static void BuildReverseIDMap(const CID2IDMap& mapIDs, CID2IDMap& mapReverseIDs);
};

#endif // !defined(AFX_TASKLISTCSVEXPORTER_H__ADF211CB_FBD2_42A2_AD51_DFF58E566753__INCLUDED_)
