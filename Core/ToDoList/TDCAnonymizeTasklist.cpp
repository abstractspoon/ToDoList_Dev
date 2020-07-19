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

CTDCAnonymizeTasklist::CTDCAnonymizeTasklist()
{
	// Use a different seed value so that we don't get same
	// result each time we run this program
	srand((unsigned int)time(NULL));

}

BOOL CTDCAnonymizeTasklist::Anonymize(LPCTSTR szTaskfile)
{
	CString sUnused;
	return Anonymize(szTaskfile, sUnused);
}

BOOL CTDCAnonymizeTasklist::Anonymize(LPCTSTR szTaskfile, CString& sAnonFilePath)
{
	CTaskFile tasks;

	if (!tasks.Load(szTaskfile))
		return FALSE;

	if (!CTDCAnonymizeTasklist().AnonymizeTasklist(tasks))
		return FALSE;

	// else
	sAnonFilePath = tasks.GetFilePath();
	return TRUE;
}

BOOL CTDCAnonymizeTasklist::AnonymizeTasklist(CTaskFile& tasks)
{
	if (!BuildContent(tasks))
		return FALSE;

	// Walk the tasks randomising their titles, comments and string attributes
	AnonymizeTask(tasks, NULL);
		
	// Save to alternate filename
	CString sFilePath(tasks.GetFilePath());
	FileMisc::AddToFileName(sFilePath, _T(".rnd"));

	return tasks.Save(sFilePath, SFEF_UTF16);
}

CString CTDCAnonymizeTasklist::GetContent(const CTaskFile& tasks, HTASKITEM hTask) const
{
	CString sContent;

	if (hTask)
	{
		TODOITEM tdi;
		VERIFY(tasks.GetTaskAttributes(hTask, tdi));
		
		sContent += tasks.GetTaskTitle(hTask);
		sContent += tasks.GetTaskComments(hTask);
	}
	
	// Subtasks
	HTASKITEM hSubTask = tasks.GetFirstTask(hTask);
	
	while (hSubTask)
	{
		sContent += GetContent(tasks, hSubTask);
		hSubTask = tasks.GetNextTask(hSubTask);
	}

	return sContent;
}

BOOL CTDCAnonymizeTasklist::BuildContent(const CTaskFile& tasks)
{
	// Get all task titles and comments
	m_sContent = GetContent(tasks, NULL);

	// Convert all whitespace and punctuation into spaces
	int nChar = m_sContent.GetLength();

	while (nChar--)
	{
		if (IsWhiteSpaceOrNumberOrPunctuation(m_sContent[nChar]))
			m_sContent.SetAt(nChar, 32);
	}

	// Remove all spaces
	m_sContent.Remove(32);

	return (m_sContent.GetLength() > 0);
}

BOOL CTDCAnonymizeTasklist::IsWhiteSpaceOrNumberOrPunctuation(TCHAR c)
{
	if (c > 127)
		return FALSE;

	if ((c >= 'A') && (c <= 'Z'))
		return FALSE;

	if ((c >= 'a') && (c <= 'z'))
		return FALSE;

	return TRUE;
}

void CTDCAnonymizeTasklist::AnonymizeTask(CTaskFile& tasks, HTASKITEM hTask)
{
	if (hTask)
	{
		TODOITEM tdi;
		VERIFY(tasks.GetTaskAttributes(hTask, tdi));
		
		tasks.SetTaskTitle(hTask, AnonymizeText(tdi.sTitle));
		tasks.SetTaskComments(hTask, AnonymizeText(tdi.sComments));

		if (!tdi.customComments.IsEmpty())
			tasks.SetTaskCustomComments(hTask, CBinaryData(_T("__")), tdi.cfComments); // Too hard
		
		AnonymizeListItems(tdi.aAllocTo,	*m_mapListData.GetAddMapping(_T("ALLOCTO")));
		AnonymizeListItems(tdi.aCategories,	*m_mapListData.GetAddMapping(_T("CATEGORY")));
		AnonymizeListItems(tdi.aTags,		*m_mapListData.GetAddMapping(_T("TAGS")));
		
		AnonymizeListItem(tdi.sAllocBy,		*m_mapListData.GetAddMapping(_T("ALLOCBY")));
		AnonymizeListItem(tdi.sStatus,		*m_mapListData.GetAddMapping(_T("STATUS")));
		AnonymizeListItem(tdi.sVersion,		*m_mapListData.GetAddMapping(_T("VERSION")));
		
		AnonymizeListItem(tdi.sExternalID,	*m_mapListData.GetAddMapping(_T("EXTERNALID")));
		AnonymizeListItem(tdi.sCreatedBy,	*m_mapListData.GetAddMapping(_T("CREATEDBY")));
		
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
		AnonymizeTask(tasks, hSubTask);
		
		hSubTask = tasks.GetNextTask(hSubTask);
	}
}

void CTDCAnonymizeTasklist::AnonymizeListItems(CStringArray& aItems, CMapStringToString& mapTLD)
{
	int nItem = aItems.GetSize();
	
	while (nItem--)
		AnonymizeListItem(aItems[nItem], mapTLD);
}

void CTDCAnonymizeTasklist::AnonymizeListItem(CString& sItem, CMapStringToString& mapTLD)
{
	if (!sItem.IsEmpty())
	{
		CString sRandom;
		
		if (!mapTLD.Lookup(sItem, sRandom))
		{
			// add to map
			sRandom = AnonymizeText(sItem);
			mapTLD[sItem] = sRandom;
		}
		
		sItem = sRandom;
		ASSERT(!sItem.IsEmpty());
	}
}

CString CTDCAnonymizeTasklist::AnonymizeText(const CString& sItem)
{
	CString sRandom(sItem);
	int nChar = sRandom.GetLength();

	while (nChar--)
	{
		TCHAR c = sRandom[nChar], cRnd = c;

		if (!IsWhiteSpaceOrNumberOrPunctuation(c))
		{
			while (cRnd == c)
			{
				int nRnd = (rand() % (m_sContent.GetLength() + 1));
				cRnd = m_sContent[nRnd];
			}
		}

		sRandom.SetAt(nChar, cRnd);
	}

	return sRandom;
}


