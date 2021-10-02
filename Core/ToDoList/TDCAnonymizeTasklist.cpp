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
	// Use a different seed value so that we don't get same
	// result each time we run this method
	srand((unsigned int)time(NULL));

	if (!BuildContent(tasks))
		return FALSE;

	AnonymizeAutoListData(tasks);
	AnonymizeCustomAttributeDefs(tasks);

	// Walk the tasks randomising their titles, comments,
	// string and custom attributes
	AnonymizeTask(tasks, NULL);
		
	// Save to alternate filename
	CString sFilePath(tasks.GetFilePath());
	FileMisc::AddToFileName(sFilePath, _T(".rnd"));

	return tasks.Save(sFilePath, SFEF_UTF16);
}

void CTDCAnonymizeTasklist::AnonymizeCustomAttributeDefs(CTaskFile& tasks)
{
	// We need attributes to persist because we need them
	// when anonymising the tasks
	int nDef = tasks.GetCustomAttributeDefs(m_aAttribDefs);

	if (nDef)
	{
		// We keep a mapping of custom attribute IDs for when
		// we need to anonymise the task custom attribute data
		CMapStringToString* pMap = m_mapSharedData.GetAddMapping(_T("CUSTOMATTRIBID"));
		ASSERT(pMap);

		while (nDef--)
		{
			TDCCUSTOMATTRIBUTEDEFINITION& def = m_aAttribDefs[nDef];

			CString sCustID = def.sUniqueID;
			CString sRndID = AnonymizeText(sCustID);

			pMap->SetAt(sCustID, sRndID); // for task lookups
			def.sUniqueID = sRndID;

			def.sLabel = AnonymizeText(def.sLabel);

			if (def.sColumnTitle.GetLength() > 1)
				def.sColumnTitle = AnonymizeText(def.sColumnTitle);

			// Anonymise list content
			if (def.IsDataType(TDCCA_STRING) && def.IsList())
				AnonymizeListItems(def.aDefaultListData, *m_mapSharedData.GetAddMapping(sRndID));
		}

		tasks.SetCustomAttributeDefs(m_aAttribDefs);
	}
}

void CTDCAnonymizeTasklist::AnonymizeAutoListData(CTaskFile& tasks)
{
	TDCAUTOLISTDATA tld;

	if (tasks.GetAutoListData(tld))
	{
		AnonymizeListItems(tld.aAllocTo,	*m_mapSharedData.GetAddMapping(_T("ALLOCTO")));
		AnonymizeListItems(tld.aCategory,	*m_mapSharedData.GetAddMapping(_T("CATEGORY")));
		AnonymizeListItems(tld.aTags,		*m_mapSharedData.GetAddMapping(_T("TAGS")));
		AnonymizeListItems(tld.aAllocBy,	*m_mapSharedData.GetAddMapping(_T("ALLOCBY")));
		AnonymizeListItems(tld.aStatus,		*m_mapSharedData.GetAddMapping(_T("STATUS")));
		AnonymizeListItems(tld.aVersion,	*m_mapSharedData.GetAddMapping(_T("VERSION")));

		tasks.SetAutoListData(tld);
	}
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
		
		AnonymizeListItems(tdi.aAllocTo,		*m_mapSharedData.GetAddMapping(_T("ALLOCTO")));
		AnonymizeListItems(tdi.aCategories,		*m_mapSharedData.GetAddMapping(_T("CATEGORY")));
		AnonymizeListItems(tdi.aTags,			*m_mapSharedData.GetAddMapping(_T("TAGS")));
		AnonymizeListItems(tdi.aFileLinks,		*m_mapSharedData.GetAddMapping(_T("FILELINK")));
		
		AnonymizeListItem(tdi.sAllocBy,			*m_mapSharedData.GetAddMapping(_T("ALLOCBY")));
		AnonymizeListItem(tdi.sStatus,			*m_mapSharedData.GetAddMapping(_T("STATUS")));
		AnonymizeListItem(tdi.sVersion,			*m_mapSharedData.GetAddMapping(_T("VERSION")));

		AnonymizeListItem(tdi.sExternalID,		*m_mapSharedData.GetAddMapping(_T("EXTERNALID")));
		AnonymizeListItem(tdi.sCreatedBy,		*m_mapSharedData.GetAddMapping(_T("CREATEDBY")));
		AnonymizeListItem(tdi.sLastModifiedBy,	*m_mapSharedData.GetAddMapping(_T("LASTMODBY")));
		
		VERIFY(tasks.SetTaskAllocatedTo(hTask, tdi.aAllocTo));
		VERIFY(tasks.SetTaskCategories(hTask, tdi.aCategories));
		VERIFY(tasks.SetTaskTags(hTask, tdi.aTags));
		VERIFY(tasks.SetTaskFileLinks(hTask, tdi.aFileLinks));
		VERIFY(tasks.SetTaskAllocatedBy(hTask, tdi.sAllocBy));
		VERIFY(tasks.SetTaskStatus(hTask, tdi.sStatus));
		VERIFY(tasks.SetTaskVersion(hTask, tdi.sVersion));
		VERIFY(tasks.SetTaskExternalID(hTask, tdi.sExternalID));
		VERIFY(tasks.SetTaskCreatedBy(hTask, tdi.sCreatedBy));
		VERIFY(tasks.SetTaskLastModifiedBy(hTask, tdi.sLastModifiedBy));

		if (tdi.HasCustomAttributeValues())
		{
			CTDCCustomAttributeDataMap mapRndData;
			
			AnonymizeCustomAttributeData(tdi.GetCustomAttributeValues(), mapRndData);
			ASSERT(mapRndData.GetCount());

			VERIFY(tasks.SetTaskCustomAttributeData(hTask, mapRndData));
		}
	}
	
	// Subtasks
	HTASKITEM hSubTask = tasks.GetFirstTask(hTask);
	
	while (hSubTask)
	{
		AnonymizeTask(tasks, hSubTask);
		
		hSubTask = tasks.GetNextTask(hSubTask);
	}
}

void CTDCAnonymizeTasklist::AnonymizeCustomAttributeData(const CTDCCustomAttributeDataMap& mapCustomData,
														CTDCCustomAttributeDataMap& mapRndData)
{
	CMapStringToString* pMapIDs = m_mapSharedData.GetAddMapping(_T("CUSTOMATTRIBID"));
	POSITION pos = mapCustomData.GetStartPosition();

	while (pos)
	{
		CString sCustID, sRndID;
		TDCCADATA data;
		mapCustomData.GetNextAssoc(pos, sCustID, data);

		// Could possibly not be in the map
		if (!pMapIDs->Lookup(sCustID, sRndID))
		{
			ASSERT(0);

			sRndID = AnonymizeText(sCustID);
			pMapIDs->SetAt(sCustID, sRndID);
		}

		const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
		GET_DEF_ALT(m_aAttribDefs, sRndID, pDef, continue);

		// Anonymise list content
		if (pDef->IsDataType(TDCCA_STRING) || pDef->IsDataType(TDCCA_FILELINK))
		{
			if (pDef->IsList())
			{
				CStringArray aListData;

				if (data.AsArray(aListData))
				{
					AnonymizeListItems(aListData, *m_mapSharedData.GetAddMapping(sRndID));
					data.Set(aListData);
				}
			}
			else
			{
				data.Set(AnonymizeText(data.AsString()));
			}
		}

		mapRndData[sRndID] = data;
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


