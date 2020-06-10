// FMindImporter.cpp: implementation of the CFMindImporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FMindImportExport.h"
#include "FMindImporter.h"
#include "FMdefines.h"

#include "../Shared/Misc.h"
#include "../Shared/datehelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

#define ADDCUSTOMATTRIBARRAY(name, fn)								\
{																	\
	CStringArray aItems;											\
	int nItem = GetTaskArrayItems(pFMTask, name, aItems);			\
	while (nItem--)													\
		pDestTaskFile->fn(hTask, aItems[nItem]);					\
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFMindImporter::CFMindImporter()
{
	m_icon.Load(IDI_FREEMIND);
}

CFMindImporter::~CFMindImporter()
{
}

void CFMindImporter::SetLocalizer(ITransText* /*pTT*/)
{
	//CLocalizer::Initialize(pTT);
}

IIMPORTEXPORT_RESULT CFMindImporter::Import(LPCTSTR szSrcFilePath, ITaskList* pDestTaskFile, bool /*bSilent*/, IPreferences* /*pPrefs*/, LPCTSTR /*szKey*/)
{
	ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pDestTaskFile, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return IIER_BADINTERFACE;
	}

	CXmlFile file;

	if (!file.Load(szSrcFilePath, _T("map")))
	{
		switch (file.GetLastFileError())
		{
		case XFL_BADMSXML:
		case XFL_MISSINGROOT:
			return IIER_BADFORMAT;
		}

		// else
		return IIER_BADFILE;
	}

	const CXmlItem* pXIMLOTree = file.Root();

	if (!pXIMLOTree)
		return IIER_BADFORMAT;

	// Get the Main Node
	const CXmlItem* pFMTask = pXIMLOTree->GetItem(_T("node"));

	if (pFMTask)
	{
		CString sTitle = pFMTask->GetItemValue(_T("TEXT"));
		pDestTaskFile->SetProjectName(sTitle);

		// Get first child Node
		pFMTask = pFMTask->GetItem(_T("node"));

		if (pFMTask)
			ImportTask(pFMTask, pTasks, NULL, TRUE); // NULL ==  root
		// else it was an empty tasklist
	}
	// else it was an empty tasklist
	
	return IIER_SUCCESS;
}

bool CFMindImporter::ImportTask(const CXmlItem* pFMTask, ITASKLISTBASE* pDestTaskFile, HTASKITEM hParent, BOOL bAndSiblings)
{
	ASSERT (pFMTask);
	
	CString sTask = GetTaskRichContent(pFMTask, _T("NODE"), _T("TEXT"));
	DWORD dwID = GetAttribValueI(pFMTask, FM_CUSTOMTASKID);

	HTASKITEM hTask = pDestTaskFile->NewTask(sTask, hParent, dwID);
		
	if (!hTask)
		return false;

	// first restore any custom TDL attributes

	// arrays
	ADDCUSTOMATTRIBARRAY(FM_CUSTOMALLOCTO, AddTaskAllocatedTo)
	ADDCUSTOMATTRIBARRAY(FM_CUSTOMCATEGORIES, AddTaskCategory)
	ADDCUSTOMATTRIBARRAY(FM_CUSTOMDEPENDS, AddTaskDependency)
	ADDCUSTOMATTRIBARRAY(FM_CUSTOMTAGS, AddTaskTag)

	// simple attribs
	pDestTaskFile->SetTaskStatus(hTask, GetAttribValueS(pFMTask, FM_CUSTOMSTATUS));
	pDestTaskFile->SetTaskAllocatedBy(hTask, GetAttribValueS(pFMTask, FM_CUSTOMALLOCBY));
	pDestTaskFile->SetTaskFileLinkPath(hTask, GetAttribValueS(pFMTask, FM_CUSTOMFILELINK));
	pDestTaskFile->SetTaskColor(hTask, GetAttribValueI(pFMTask, FM_CUSTOMCOLOR));
	pDestTaskFile->SetTaskPriority(hTask, GetAttribValueI(pFMTask, FM_CUSTOMPRIORITY));
	pDestTaskFile->SetTaskPercentDone(hTask, (unsigned char)GetAttribValueI(pFMTask, FM_CUSTOMPERCENT));
	pDestTaskFile->SetTaskFlag(hTask, GetAttribValueB(pFMTask, FM_CUSTOMFLAG));
	pDestTaskFile->SetTaskCreatedBy(hTask, GetAttribValueS(pFMTask, FM_CUSTOMCREATEDBY));
	pDestTaskFile->SetTaskRisk(hTask, GetAttribValueI(pFMTask, FM_CUSTOMRISK));
	pDestTaskFile->SetTaskExternalID(hTask, GetAttribValueS(pFMTask, FM_CUSTOMEXTID));
	pDestTaskFile->SetTaskCost(hTask, GetAttribValueD(pFMTask, FM_CUSTOMCOST), false);
	pDestTaskFile->SetTaskVersion(hTask, GetAttribValueS(pFMTask, FM_CUSTOMVERSION));
	pDestTaskFile->SetTaskComments(hTask, GetTaskComments(pFMTask));

	// dates
	time64_t tDate;

	if (GetAttribValueT(pFMTask, FM_CUSTOMDONEDATE, tDate))
		pDestTaskFile->SetTaskDoneDate64(hTask, tDate);

	if (GetAttribValueT(pFMTask, FM_CUSTOMDUEDATE, tDate))
		pDestTaskFile->SetTaskDueDate64(hTask, tDate);

	if (GetAttribValueT(pFMTask, FM_CUSTOMSTARTDATE, tDate))
		pDestTaskFile->SetTaskStartDate64(hTask, tDate);

	// times
	TDC_UNITS cUnits = (TDC_UNITS)GetAttribValueI(pFMTask, FM_CUSTOMTIMEESTUNITS);
	pDestTaskFile->SetTaskTimeEstimate(hTask, GetAttribValueD(pFMTask, FM_CUSTOMTIMEEST), cUnits);

	cUnits = (TDC_UNITS)GetAttribValueI(pFMTask, FM_CUSTOMTIMESPENTUNITS);
	pDestTaskFile->SetTaskTimeSpent(hTask, GetAttribValueD(pFMTask, FM_CUSTOMTIMESPENT), cUnits);

	// recurrence
	TDC_REGULARITY nRegularity = (TDC_REGULARITY)GetAttribValueI(pFMTask, FM_CUSTOMREGULARITY);
	TDC_RECURFROMOPTION nRecalcFrom = (TDC_RECURFROMOPTION)GetAttribValueI(pFMTask, FM_CUSTOMRECURRECALCFROM);
	TDC_RECURREUSEOPTION nReuse = (TDC_RECURREUSEOPTION)GetAttribValueI(pFMTask, FM_CUSTOMRECURREUSE);

	DWORD dwSpecific1 = GetAttribValueI(pFMTask, FM_CUSTOMRECURSPEC1);
	DWORD dwSpecific2 = GetAttribValueI(pFMTask, FM_CUSTOMRECURSPEC2);
	int nNumOccur = GetAttribValueI(pFMTask, FM_CUSTOMNUMOCCUR);
	int nRemainingOccurs = GetAttribValueI(pFMTask, FM_CUSTOMREMAININGOCCUR);
	bool bPreserveComments = (GetAttribValueI(pFMTask, FM_CUSTOMPRESERVECOMMENTS) != FALSE);

	pDestTaskFile->SetTaskRecurrence(hTask, nRegularity, dwSpecific1, dwSpecific2, 
									nRecalcFrom, nReuse, nNumOccur, nRemainingOccurs, bPreserveComments);

	// then overwrite with FreeMind native attributes
	pDestTaskFile->SetTaskCreationDate(hTask, GetFMDate(pFMTask, _T("CREATED")));
	pDestTaskFile->SetTaskLastModified(hTask, GetFMDate(pFMTask, _T("MODIFIED")));
	pDestTaskFile->SetTaskColor(hTask, GetFMColor(pFMTask, _T("COLOR")));

	// FM specific attributes not handled natively by ToDoList
	pDestTaskFile->SetTaskMetaData(hTask, _T("FM_BKGNDCOLOR"), pFMTask->GetItemValue(_T("BACKGROUND_COLOR")));
	pDestTaskFile->SetTaskMetaData(hTask, _T("FM_ID"), pFMTask->GetItemValue(_T("ID")));
	pDestTaskFile->SetTaskMetaData(hTask, _T("FM_POSITION"), pFMTask->GetItemValue(_T("POSITION")));

	// Completion Status (== icon)
	const CXmlItem* pXIIcon = pFMTask->GetItem(_T("icon"));
	
	while (pXIIcon)
	{		
		// For Completion Status - <icon BUILTIN="button_ok"/>
		if (pXIIcon->FindItem(_T("BUILTIN"),_T("button_ok"),TRUE))
		{
			pDestTaskFile->SetTaskDoneDate(hTask, pDestTaskFile->GetTaskLastModified(hTask));
			pDestTaskFile->SetTaskPercentDone(hTask, 100);
		}
	
		// For Task priority - <icon BUILTIN="full-n"/>
		for (int nItem = 1; nItem <= 7; nItem++)
		{
			CString sItem;
			sItem.Format(_T("full-%d"), nItem);

			if (pXIIcon->FindItem(_T("BUILTIN"), sItem, TRUE))
			{
				pDestTaskFile->SetTaskPriority(hTask, nItem);
				break;
			}
		}

		pXIIcon = pXIIcon->GetSibling();
	}
	
	// children
	const CXmlItem* pXIMLOSubTask = pFMTask->GetItem(_T("node"));

	if (pXIMLOSubTask)
	{
		if (!ImportTask(pXIMLOSubTask, pDestTaskFile, hTask, TRUE))
			return false;
	}

	// handle sibling tasks WITHOUT RECURSION
	if (bAndSiblings)
	{
		pFMTask = pFMTask->GetSibling();
		
		while (pFMTask)
		{
			// FALSE == don't recurse on siblings
			if (!ImportTask(pFMTask, pDestTaskFile, hParent, FALSE))
				return false;
			
			pFMTask = pFMTask->GetSibling();
		}
	}

	return true;
}

CString CFMindImporter::GetTaskComments(const CXmlItem* pFMTask)
{
	CString sComments = GetTaskRichContent(pFMTask, _T("NOTE"), _T("Comments"));

	if (sComments.IsEmpty())
	{
		// handle export from XMind
		sComments = pFMTask->GetItemValue(_T("Hook"));
	}

	return sComments;
}

CString CFMindImporter::GetTaskRichContent(const CXmlItem* pFMTask, LPCTSTR szRichType, LPCTSTR szFallback)
{
	const CXmlItem* pXIRich = pFMTask->GetItem(_T("richcontent"));
	CString sContent;
	
	while (pXIRich)
	{
		if (pXIRich->GetItemValue(_T("TYPE")) == szRichType)
		{
			sContent = pXIRich->GetValue();
			sContent.TrimRight();
			
			// extract HTML if present
			if (sContent.IsEmpty())
			{
				const CXmlItem* pXIBody = pXIRich->GetItem(_T("html"), _T("body"));
				
				if (pXIBody)
					sContent = ExtractContentFromItem(pXIBody, TRUE);
			}			
			break;
		}
		
		pXIRich = pXIRich->GetSibling();
	}
	
	// else
	if (sContent.IsEmpty() && szFallback)
	{
		const CXmlItem* pXIFallback = pFMTask->GetItem(szFallback);

		if (pXIFallback)
			sContent = ExtractContentFromItem(pXIFallback, TRUE);
	}
	
	return sContent;
}

CString CFMindImporter::ExtractContentFromItem(const CXmlItem* pFMItem, BOOL bAndSiblings)
{
	if (pFMItem == NULL)
		return _T("");
	
	CString sContent = pFMItem->GetValue();
	Misc::Trim(sContent);
	
	// add spacer
	if (!sContent.IsEmpty())
		sContent += ' ';
	
	// handle children first
	POSITION pos = pFMItem->GetFirstItemPos();
	
	while (pos)
	{
		const CXmlItem* pXINode = pFMItem->GetNextItem(pos);
		ASSERT(pXINode);
		
		sContent += ExtractContentFromItem(pXINode, TRUE);
	}
	
	// handle sibling tasks WITHOUT RECURSION
	if (bAndSiblings)
	{
		pFMItem = pFMItem->GetSibling();
		
		while (pFMItem)
		{
			// FALSE == don't recurse on siblings
			sContent += ExtractContentFromItem(pFMItem, FALSE);
			pFMItem = pFMItem->GetSibling();
		}
	}
	
	return sContent;
}

// Java Time date to CTime
time_t CFMindImporter::GetFMDate(const CXmlItem* pFMTask, LPCTSTR szDateField)
{
	// Date is Milliseconds from Jan 1, 1970.
	CString sDate = pFMTask->GetItemValue(szDateField);

	// So cull the last 3 digits to get seconds
	sDate = sDate.Left(sDate.GetLength() - 3);

	return (time_t)_ttol(sDate);
}

// Color Manipulation Code borrowed from - http://www.codeproject.com/gdi/ccolor.asp
COLORREF CFMindImporter::GetFMColor(const CXmlItem* pFMTask, LPCTSTR szColorField)
{
	CString sColor = pFMTask->GetItemValue(szColorField);

	if (sColor.GetLength() < 2)
		return 0;

	if (sColor[0] == _T('#')) 
	{
		LPTSTR szStop;
		ULONG ret = (ULONG)_tcstoul(sColor, &szStop, 16);
		return RGB(GetBValue(ret), GetGValue(ret), GetRValue(ret));
	}

	return 0;
}

bool CFMindImporter::GetAttribValueT(const CXmlItem* pFMTask, LPCTSTR szAttribName, time64_t& tDate)
{
	if (CDateHelper::DecodeDate(GetAttribValueS(pFMTask, szAttribName), tDate, TRUE))
		return true;

	// else
	return false;
}

CString CFMindImporter::GetAttribValueS( const CXmlItem *pFMTask, LPCTSTR szAttribName)
{
	// Completion Status (== icon)
	const CXmlItem* pXIAttrib = pFMTask->GetItem(_T("attribute"));

	if (pXIAttrib == NULL) 
		return "";	

	while (pXIAttrib)
	{		
		const CXmlItem* pXIAttribItemName = pXIAttrib->GetItem(_T("NAME"));

		if (pXIAttribItemName != NULL)
		{
			if (pXIAttribItemName->ValueMatches(szAttribName))
			{
				const CXmlItem* pXIAttribItemValue = pXIAttrib->GetItem(_T("VALUE"));
				
				if (pXIAttribItemValue != NULL)
					return pXIAttribItemValue->GetValue();
			}
		}		
		
		pXIAttrib = pXIAttrib->GetSibling();
	}
	return "";
}

int CFMindImporter::GetTaskArrayItems(const CXmlItem* pFMTask, LPCTSTR szAttribName, CStringArray& aItems)
{
	CString sItems = GetAttribValueS(pFMTask, szAttribName);

	aItems.RemoveAll();
	Misc::Split(sItems, aItems, ',');

	return aItems.GetSize();
}

int CFMindImporter::GetAttribValueI(const CXmlItem *pFMTask, LPCTSTR szAttribName)
{
	CString Result = GetAttribValueS(pFMTask, szAttribName);
	return _ttoi(Result); 
}

bool CFMindImporter::GetAttribValueB(const CXmlItem *pFMTask, LPCTSTR szAttribName)
{
	return (GetAttribValueI(pFMTask, szAttribName) != 0);
}

double CFMindImporter::GetAttribValueD(const CXmlItem *pFMTask, LPCTSTR szAttribName)
{
	CString Result = GetAttribValueS(pFMTask, szAttribName);
	return _ttof(Result); 
}