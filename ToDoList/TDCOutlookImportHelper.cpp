// TDCOutlookImportHelper.cpp: implementation of the CTDCOutlookImportHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "tdcenum.h"
#include "todoitem.h"
#include "taskfile.h"
#include "TDCOutlookImportHelper.h"
#include "TDLImportOutlookObjectsDlg.h"
#include "TaskListDropTarget.h"

#include "..\shared\Misc.h"
#include "..\shared\datehelper.h"
#include "..\shared\MSOutlookHelper.h"

#include "..\3rdparty\msoutl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CTDCOutlookImportHelper::ImportTasks(const TLDT_DATA* pData, UINT nIDMappingError, ITaskList* pTasks)
{
	// If the data contains files which are not from Outlook
	// then quit before starting up outlook
	if (!pData->pOutlookSelection && pData->pFilePaths)
	{
		if ((pData->pFilePaths->GetSize() == 0) ||
			!CMSOutlookHelper::IsOutlookObject(pData->pFilePaths->GetAt(0)))
		{
			return 0;
		}
	}

	// first thing to do is to get the first outlook object so we 
	// can prime the dialog

	// NOTE: we've got to be a bit clever here because if the dialog
	// has been displayed before, the user may have elected to hide
	// confidential fields in which case we need to know this so as not
	// to ask for that data which in turn will trigger the outlook
	// security popup.
	CMSOutlookHelper outlook;
	OutlookAPI::_Item* pItem = NULL;
	
	if (pData->pOutlookSelection)
		pItem = outlook.GetFirstObject(pData->pOutlookSelection);
	
	if (!pItem && pData->pFilePaths)
		pItem = outlook.GetFirstFileObject(*pData->pFilePaths);
	
	if (pItem == NULL)
		return 0; // nothing looking like an outlook object
	
	// display the mapping dialog and create the tasks
	CTDLImportOutlookObjectsDlg dialog(*pItem);
	CTDCAttributeMapping aMapping;
	
	while (true)
	{
		if (dialog.DoModal() != IDOK)
			return false;
		
		// valid mapping must include title
		if (!dialog.GetColumnMapping(aMapping) || !aMapping.IsAttributeMapped(TDCA_TASKNAME))
		{
			AfxMessageBox(CEnString(nIDMappingError));
			// try again
		}
		else
		{
			break; // all good
		}
	} 
	
	// cleanup the temporary file object
	delete pItem;
	
	// accumulate the new tasks
	BOOL bWantConfidential = dialog.GetWantConfidentialAttributes();
	int nTaskCount = 0;
	
	if (pData->pOutlookSelection)
	{
		// 1-based indexing
		for (short nSel = 1; nSel <= pData->pOutlookSelection->GetCount(); nSel++)
		{
			OutlookAPI::_Item item(pData->pOutlookSelection->Item(COleVariant(nSel)));
			
			if (CTDCOutlookImportHelper::ImportTask(aMapping, &item, bWantConfidential, pTasks))
				nTaskCount++;
			else
				ASSERT(0);
		}
	}
	else if (pData->pFilePaths)
	{
		// 0-based indexing
		for (int nFile = 0; nFile < pData->GetFileCount(); nFile++)
		{
			OutlookAPI::_Item* pItem = outlook.GetFileObject(pData->GetFile(nFile));
			
			if (pItem)
			{
				if (CTDCOutlookImportHelper::ImportTask(aMapping, pItem, bWantConfidential, pTasks))
					nTaskCount++;
				else
					ASSERT(0);
				
				// cleanup
				delete pItem;
			}
		}
	}
	
	return nTaskCount;
}

BOOL CTDCOutlookImportHelper::ImportTask(const CTDCAttributeMapping& aMapping, OutlookAPI::_Item* pItem, BOOL bWantConfidential, ITaskList* pTasks, BOOL bWantAttrib)
{
	TODOITEM tdi;

	if (ImportTask(aMapping, pItem, bWantConfidential, tdi))
	{
		HTASKITEM hTask = pTasks->NewTask(tdi.sTitle);
		ASSERT(hTask);
		
		if (hTask)
		{
			if (!bWantAttrib || SetTaskAttributes(pTasks, hTask, tdi))
				return TRUE;
		}
	}

	ASSERT(0);
	return FALSE;
}

BOOL CTDCOutlookImportHelper::ImportTask(const CTDCAttributeMapping& aMapping, OutlookAPI::_Item* pItem, BOOL bWantConfidential, TODOITEM& tdi)
{
	ASSERT(pItem);

	CMSOutlookItemDataMap mapData;
	
	if (CMSOutlookHelper::GetItemData(*pItem, mapData, bWantConfidential) == 0)
		return NULL;

	for (int nAttrib = 0; nAttrib < aMapping.GetSize(); nAttrib++)
	{
		const TDCATTRIBUTEMAPPING& attrib = aMapping[nAttrib];

		OUTLOOK_FIELDTYPE oaType = (OUTLOOK_FIELDTYPE)attrib.dwItemData;
		CString sData;
		CStringArray aData;
		
		if (attrib.nTDCAttrib == TDCA_NONE || !mapData.Lookup(oaType, sData) || sData.IsEmpty())
			continue;
		
		switch(attrib.nTDCAttrib)
		{
		case TDCA_TASKNAME:
			if (!tdi.sTitle.IsEmpty())
				tdi.sTitle += _T(", ");

			tdi.sTitle += sData;
			break;
			
		case TDCA_CATEGORY: 
			Misc::Split(sData, aData);
			tdi.aCategories.Append(aData);
			break;
			
		case TDCA_TAGS: 
			Misc::Split(sData, aData);
			tdi.aTags.Append(aData);
			break;
			
		case TDCA_STATUS: 
			tdi.sStatus = sData;
			break;
			
		case TDCA_EXTERNALID: 
			tdi.sExternalID = sData;
			break;
			
		case TDCA_ALLOCBY: 
			tdi.sAllocBy = sData;
			break;
			
		case TDCA_ALLOCTO: 
			Misc::Split(sData, aData);
			tdi.aAllocTo.Append(aData);
			break;
			
		case TDCA_VERSION: 
			tdi.sVersion = sData;
			break;
			
		case TDCA_FILEREF: 
			// special case:
			if (oaType == OA_ENTRYID)
			{
				CString sFileRef;
				sFileRef.Format(_T("outlook:%s"), sData);
				sData = sFileRef;
			}
			tdi.aFileLinks.Add(sData);
			break;
			
		case TDCA_DEPENDENCY: 
			Misc::Split(sData, aData);
			tdi.aDependencies.Append(aData);
			break;
			
		case TDCA_COMMENTS:
			if (!tdi.sComments.IsEmpty())
				tdi.sComments += '\n';
// 			else
// 				tdi.sCommentsTypeID = m_cfDefault;

			// special case:
			if (oaType == OA_ENTRYID)
			{
				CString sLink;
				sLink.Format(_T("outlook:%s"), sData);
				tdi.sComments += sLink;
			}
			else
			{
				tdi.sComments += CTDLImportOutlookObjectsDlg::GetOutlookFieldName(oaType);
				tdi.sComments += _T(": ");
				tdi.sComments += sData;
			}
			break;
			
		case TDCA_STARTDATE: 
			CDateHelper::DecodeDate(sData, tdi.dateStart);
			break;
			
		case TDCA_DUEDATE: 
			CDateHelper::DecodeDate(sData, tdi.dateDue);
			break;
			
		case TDCA_DONEDATE: 
			CDateHelper::DecodeDate(sData, tdi.dateDone);
			break;
			
		case TDCA_LASTMOD: 
			CDateHelper::DecodeDate(sData, tdi.dateLastMod);
			break;
			
		case TDCA_CREATIONDATE: 
			CDateHelper::DecodeDate(sData, tdi.dateCreated);
			break;
			
		case TDCA_PRIORITY: 
			// map 0->0, 1->5, 2->10
			tdi.nPriority = (_ttoi(sData) * 5);
			break;
			
		case TDCA_RISK: 
			tdi.nRisk = _ttoi(sData);
			break;
			
		case TDCA_FLAG: 
			tdi.bFlagged = !sData.IsEmpty();
			break;
			
		case TDCA_COST: 
			tdi.dCost = _ttof(sData);
			break;
			
		case TDCA_TIMEEST: 
			tdi.dTimeEstimate = _ttof(sData);
			break;
			
		case TDCA_TIMESPENT: 
			tdi.dTimeSpent = _ttof(sData);
			break;
		}
	}

	// handle attachments
// 	OutlookAPI::Attachments attachments(pItem->GetAttachments());
// 
// 	for (short nAttach = 1; nAttach <= attachments.GetCount(); nAttach++)
// 	{
// 		OutlookAPI::Attachment item(attachments.Item(COleVariant(nAttach)));
// 		item.SaveAsFile()
// 	}

	return TRUE;
}

BOOL CTDCOutlookImportHelper::SetTaskAttributes(ITaskList* pTasks, HTASKITEM hTask, const TODOITEM& tdi)
{
	ITaskList15* pTasks15 = GetITLInterface<ITaskList15>(pTasks, IID_TASKLIST15);
	ASSERT(pTasks15);
	
	if (!pTasks15)
		return FALSE;
	
	// ------------------------------------------------------------
	pTasks15->SetTaskComments(hTask, tdi.sComments);
	pTasks15->SetTaskAllocatedBy(hTask, tdi.sAllocBy);
	pTasks15->SetTaskStatus(hTask, tdi.sStatus);
	pTasks15->SetTaskCreatedBy(hTask, tdi.sCreatedBy);
	pTasks15->SetTaskExternalID(hTask, tdi.sExternalID);
	pTasks15->SetTaskVersion(hTask, tdi.sVersion);
	pTasks15->SetTaskIcon(hTask, tdi.sIcon);

	// ------------------------------------------------------------
	int nItem;

	for (nItem = 0; nItem < tdi.aAllocTo.GetSize(); nItem++)
		pTasks15->AddTaskAllocatedTo(hTask, tdi.aAllocTo[nItem]);
	
	for (nItem = 0; nItem < tdi.aCategories.GetSize(); nItem++)
		pTasks15->AddTaskCategory(hTask, tdi.aCategories[nItem]);
	
	for (nItem = 0; nItem < tdi.aDependencies.GetSize(); nItem++)
		pTasks15->AddTaskDependency(hTask, tdi.aDependencies[nItem]);
	
	for (nItem = 0; nItem < tdi.aTags.GetSize(); nItem++)
		pTasks15->AddTaskTag(hTask, tdi.aTags[nItem]);
	
	for (nItem = 0; nItem < tdi.aFileLinks.GetSize(); nItem++)
		pTasks15->AddTaskFileLink(hTask, tdi.aFileLinks[nItem]);

	// ------------------------------------------------------------
	time64_t time;

	if (CDateHelper::GetTimeT64(tdi.dateStart, time))
		pTasks15->SetTaskStartDate64(hTask, time);

	if (CDateHelper::GetTimeT64(tdi.dateDue, time))
		pTasks15->SetTaskStartDate64(hTask, time);
	
	if (CDateHelper::GetTimeT64(tdi.dateDone, time))
		pTasks15->SetTaskStartDate64(hTask, time);
	
	if (CDateHelper::GetTimeT64(tdi.dateCreated, time))
		pTasks15->SetTaskCreationDate64(hTask, time);
	
	if (CDateHelper::GetTimeT64(tdi.dateLastMod, time))
		pTasks15->SetTaskLastModified64(hTask, time);
	
	// ------------------------------------------------------------
	pTasks15->SetTaskPriority(hTask, tdi.nPriority);
	pTasks15->SetTaskRisk(hTask, tdi.nRisk);
	pTasks15->SetTaskPercentDone(hTask, (unsigned char)tdi.nPercentDone);
	pTasks15->SetTaskCost(hTask, tdi.dCost);
	pTasks15->SetTaskFlag(hTask, (tdi.bFlagged == TRUE));
	pTasks15->SetTaskTimeEstimate(hTask, tdi.dTimeEstimate, tdi.nTimeEstUnits);
	pTasks15->SetTaskTimeSpent(hTask, tdi.dTimeSpent, tdi.nTimeSpentUnits);

	return TRUE;
}
