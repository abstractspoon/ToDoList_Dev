// TaskListCsvExporter.cpp: implementation of the CTaskListTdlExporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaskListtdlExporter.h"
#include "TaskFile.h"
#include "tdcstruct.h"
#include "todoitem.h"

#include "..\shared\filemisc.h"

#include "..\Interfaces\TasklistSchemaDef.h"

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaskListTdlExporter::CTaskListTdlExporter()
{
	m_icon.Load(IDR_MAINFRAME_STD);
}

CTaskListTdlExporter::~CTaskListTdlExporter()
{

}

IIMPORTEXPORT_RESULT CTaskListTdlExporter::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	if (pSrcTaskFile->GetTaskListCount() == 1)
		return Export(pSrcTaskFile->GetTaskList(0), szDestFilePath, dwFlags, pPrefs, szKey);

	// create a single tasklist out of the many
	CTaskFile tasks;
	int nTaskListCount = pSrcTaskFile->GetTaskListCount();

	// accumulate custom task attribute definitions
	CTDCCustomAttribDefinitionArray aAttribDefs;

	for (int nTaskList = 0; nTaskList < nTaskListCount; nTaskList++)
	{
		const ITaskList* pTasks = pSrcTaskFile->GetTaskList(nTaskList);
		const ITaskList17* pTasks17 = GetITLInterface<ITaskList17>(pTasks, IID_TASKLIST17);
		
		if (pTasks17) // minimum requirement
		{
			// do not export the destination file if it also appears in the list
			CString sFilePath = pTasks17->GetFileName(true);

			if (FileMisc::IsSamePath(sFilePath, szDestFilePath))
				continue;

			// save off custom attribute definitions
			CTDCCustomAttribDefinitionArray aDefs; 

			if (CTaskFile::GetCustomAttributeDefs(pTasks17, aDefs))
				aAttribDefs.Append(aDefs);

			// create a top-level task for each tasklist
			CString sProjectName = pTasks17->GetProjectName();

			if (sProjectName.IsEmpty())
				sProjectName = FileMisc::GetFileNameFromPath(sFilePath, FALSE);

			HTASKITEM hFileTask = tasks.NewTask(sProjectName, NULL, 0, 0);
			ASSERT(hFileTask);

			// copy tasklist into that top-level task resetting IDs
			// and keeping track of the ID mapping as we go
			CID2IDMap mapIDs;

			tasks.CopyTaskFrom(pTasks17, pTasks17->GetFirstTask(), hFileTask, TRUE, &mapIDs);

			// verify task count is as expected
			ASSERT(mapIDs.GetCount() == pTasks17->GetTaskCount());

			// create file links back to original tasks
			HTASKITEM hFirstTask = tasks.GetFirstTask(hFileTask);
			CreateReverseFileLinks(tasks, hFirstTask, mapIDs, FileMisc::GetFileNameFromPath(sFilePath), TRUE);

			// fixup dependency IDs
			CID2IDMap mapReverseIDs;
			BuildReverseIDMap(mapIDs, mapReverseIDs);

			FixupInternalDependencies(tasks, hFirstTask, mapReverseIDs, TRUE);
		}
	}

	// inject custom attribute defs in one hit
	tasks.SetCustomAttributeDefs(aAttribDefs);

	if (!tasks.Save(szDestFilePath, SFEF_UTF16))
		return IIER_BADFILE;

	return IIER_SUCCESS;
}

IIMPORTEXPORT_RESULT CTaskListTdlExporter::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD /*dwFlags*/, IPreferences* /*pPrefs*/, LPCTSTR /*szKey*/)
{
	CTaskFile tasks(pSrcTaskFile);

	if (!tasks.Save(szDestFilePath, SFEF_UTF16))
		return IIER_BADFILE;

	return IIER_SUCCESS;
}

void CTaskListTdlExporter::CreateReverseFileLinks(CTaskFile& tasks, HTASKITEM hTask, const CID2IDMap& mapIDs, const CString& sFileName, BOOL bAndSiblings)
{
	if (!hTask)
		return;

	DWORD dwID = tasks.GetTaskID(hTask), dwOrgID = 0;
	CString sFileLink;

	VERIFY (mapIDs.Lookup(dwID, dwOrgID));
	ASSERT (dwOrgID > 0);

	sFileLink.Format(_T("tdl://%s?%lu"), sFileName, dwOrgID);
	tasks.AddTaskFileLink(hTask, sFileLink);

	// first child
	CreateReverseFileLinks(tasks, tasks.GetFirstTask(hTask), mapIDs, sFileName, TRUE);

	// handle sibling tasks WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = tasks.GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == don't recurse on siblings
			CreateReverseFileLinks(tasks, hSibling, mapIDs, sFileName, FALSE);
			
			hSibling = tasks.GetNextTask(hSibling);
		}
	}
}

void CTaskListTdlExporter::FixupInternalDependencies(CTaskFile& tasks, HTASKITEM hTask, const CID2IDMap& mapIDs, BOOL bAndSiblings)
{
	if (!hTask)
		return;

	CTDCDependencyArray aDepends;
	int nNumDepends = tasks.GetTaskDependencies(hTask, aDepends);

	for (int nDepend = 0; nDepend < nNumDepends; nDepend++)
	{
		TDCDEPENDENCY& depend = aDepends[nDepend];

		// only handle  simple numeric dependencies
		if (depend.IsLocal())
		{
			DWORD dwNewID = 0;

			VERIFY (mapIDs.Lookup(depend.dwTaskID, dwNewID));
			ASSERT (dwNewID > 0);

			depend.dwTaskID = dwNewID;
		}
	}

	// reassign new dependencies
	tasks.SetTaskDependencies(hTask, aDepends);

	// first child
	FixupInternalDependencies(tasks, tasks.GetFirstTask(hTask), mapIDs, TRUE);

	// handle sibling tasks WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = tasks.GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == don't recurse on siblings
			FixupInternalDependencies(tasks, hSibling, mapIDs, FALSE);
			
			hSibling = tasks.GetNextTask(hSibling);
		}
	}
}

void CTaskListTdlExporter::BuildReverseIDMap(const CID2IDMap& mapIDs, CID2IDMap& mapReverseIDs)
{
	mapReverseIDs.RemoveAll();

	POSITION pos = mapIDs.GetStartPosition();
	DWORD dwID, dwOrgID;

	while (pos)
	{
		mapIDs.GetNextAssoc(pos, dwID, dwOrgID);
		ASSERT(dwID > 0 && dwOrgID > 0);

		mapReverseIDs[dwOrgID] = dwID;
	}
}
