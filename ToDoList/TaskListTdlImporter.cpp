// TaskListCsvImporter.cpp: implementation of the CTaskListTdlImporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaskListTdlImporter.h"
#include "TDLTasklistImportDlg.h"
#include "Taskfile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaskListTdlImporter::CTaskListTdlImporter()
{

}

CTaskListTdlImporter::~CTaskListTdlImporter()
{

}

IIMPORT_RESULT CTaskListTdlImporter::Import(LPCTSTR szSrcFilePath, ITaskList* pDestTaskFile, BOOL bSilent, IPreferences* /*pPrefs*/, LPCTSTR /*szKey*/)
{
	ITaskList8* pTasks8 = GetITLInterface<ITaskList8>(pDestTaskFile, IID_TASKLIST8);

	if (!pTasks8)
	{
		ASSERT(0);
		return IIR_BADINTERFACE;
	}
	
	if (!bSilent)
	{
		CTDLTasklistImportDlg dialog(szSrcFilePath);
		
		if (dialog.DoModal() != IDOK)
			return IIR_CANCELLED;

		// else
		return dialog.GetSelectedTasks(pTasks8);
	}

	// else 
	CTaskFile tasks;

	if (!tasks.Load(szSrcFilePath))
	{
		switch (tasks.GetLastFileError())
		{
		case XFL_CANCELLED: 
			return IIR_CANCELLED;

		case XFL_BADMSXML:
		case XFL_MISSINGROOT: 
			return IIR_BADFORMAT;
		}

		return IIR_OTHER;
	}

	VERIFY(tasks.CopyTasksTo(pTasks8));
	return IIR_SUCCESS;
}

