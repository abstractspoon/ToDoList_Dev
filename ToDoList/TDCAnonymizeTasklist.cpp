// TDCAnonymizeTasklist.cpp: implementation of the CTDCAnonymizeTasklist class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDCAnonymizeTasklist.h"
#include "todoitem.h"

#include "..\Shared\Misc.h"
#include "..\Shared\fileMisc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CTDCAnonymizeTasklist::Anonymize(LPCTSTR szTaskfile)
{
	CTaskFile tasks;
	
	if (!tasks.Load(szTaskfile))
		return FALSE;

	// Walk the tasks randomising their titles, comments and string attributes
	CMapStringToStringMap mapTLD;
	
	Anonymize(tasks, NULL, mapTLD);
		
	// Save to alternate filename
	CString sFilePath(szTaskfile);
	FileMisc::AddToFileName(sFilePath, _T(".rnd"));

	return tasks.Save(sFilePath, SFEF_UTF16);
}

void CTDCAnonymizeTasklist::Anonymize(CTaskFile& tasks, HTASKITEM hTask, CMapStringToStringMap& mapTLD)
{
	if (hTask)
	{
		TODOITEM tdi;
		VERIFY(tasks.GetTaskAttributes(hTask, &tdi));
		
		tasks.SetTaskTitle(hTask, Misc::Shuffle(tdi.sTitle));
		tasks.SetTaskComments(hTask, Misc::Shuffle(tdi.sComments));

		if (!tdi.customComments.IsEmpty())
			tasks.SetTaskCustomComments(hTask, CBinaryData(_T("__")), tdi.sCommentsTypeID); // Too hard
		
		Anonymize(tdi.aAllocTo,		*mapTLD.GetAddMapping(_T("ALLOCTO")));
		Anonymize(tdi.aCategories,	*mapTLD.GetAddMapping(_T("CATEGORY")));
		Anonymize(tdi.aTags,		*mapTLD.GetAddMapping(_T("TAGS")));
		
		Anonymize(tdi.sAllocBy,		*mapTLD.GetAddMapping(_T("ALLOCBY")));
		Anonymize(tdi.sStatus,		*mapTLD.GetAddMapping(_T("STATUS")));
		Anonymize(tdi.sVersion,		*mapTLD.GetAddMapping(_T("VERSION")));
		
		Anonymize(tdi.sExternalID,	*mapTLD.GetAddMapping(_T("EXTERNALID")));
		Anonymize(tdi.sCreatedBy,	*mapTLD.GetAddMapping(_T("CREATEDBY")));
		
		VERIFY(tasks.SetTaskAllocatedTo(hTask, tdi.aAllocTo));
		VERIFY(tasks.SetTaskCategories(hTask, tdi.aCategories));
		VERIFY(tasks.SetTaskTags(hTask, tdi.aTags));
		VERIFY(tasks.SetTaskAllocatedBy(hTask, tdi.sAllocBy));
		VERIFY(tasks.SetTaskStatus(hTask, tdi.sStatus));
		VERIFY(tasks.SetTaskVersion(hTask, tdi.sVersion));
		VERIFY(tasks.SetTaskExternalID(hTask, tdi.sExternalID));
		VERIFY(tasks.SetTaskCreatedBy(hTask, tdi.sCreatedBy));
	}
	
	// Subtasks
	HTASKITEM hSubTask = tasks.GetFirstTask(hTask);
	
	while (hSubTask)
	{
		Anonymize(tasks, hSubTask, mapTLD);
		
		hSubTask = tasks.GetNextTask(hSubTask);
	}
}

void CTDCAnonymizeTasklist::Anonymize(CStringArray& aItems, CMapStringToString& mapTLD)
{
	int nItem = aItems.GetSize();
	
	while (nItem--)
		Anonymize(aItems[nItem], mapTLD);
}

void CTDCAnonymizeTasklist::Anonymize(CString& sItem, CMapStringToString& mapTLD)
{
	if (!sItem.IsEmpty())
	{
		CString sRandom;
		
		if (!mapTLD.Lookup(sItem, sRandom))
		{
			// add to map
			sRandom = Misc::Shuffle(sItem);
			mapTLD[sItem] = sRandom;
		}
		
		sItem = sRandom;
		ASSERT(!sItem.IsEmpty());
	}
}

