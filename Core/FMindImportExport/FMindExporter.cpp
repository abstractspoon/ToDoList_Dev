// FMindExporter.cpp: implementation of the CFMindExporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FMindImportExport.h"
#include "FMindExporter.h"
#include "FMdefines.h"

#include "..\shared\xmlfileex.h"
#include "..\shared\misc.h"
#include "..\shared\filemisc.h"

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

#define ADDCUSTOMATTRIB(name, value)									\
{																		\
	CXmlItem* pXIAttribs = pXIDestItem->AddItem(_T("attribute"));		\
	pXIAttribs->AddItem(_T("NAME"), name);								\
	pXIAttribs->AddItem(_T("VALUE"), value);							\
}

#define ADDCUSTOMATTRIBARRAY(name, fnCount, fn)								\
{																			\
	int nCount = pSrcTaskFile->fnCount(hTask);								\
	CStringArray aItems;													\
	while (nCount--) aItems.InsertAt(0, pSrcTaskFile->fn(hTask, nCount));	\
	if (aItems.GetSize())													\
	{																		\
		CXmlItem* pXIAttribs = pXIDestItem->AddItem(_T("attribute"));		\
		pXIAttribs->AddItem(_T("NAME"), name);								\
		pXIAttribs->AddItem(_T("VALUE"), Misc::FormatArray(aItems, _T(",")));\
	}																		\
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFMindExporter::CFMindExporter()
{
	m_icon.Load(IDI_FREEMIND);
}

CFMindExporter::~CFMindExporter()
{
}

void CFMindExporter::SetLocalizer(ITransText* /*pTT*/)
{
	//CLocalizer::Initialize(pTT);
}

IIMPORTEXPORT_RESULT CFMindExporter::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD /*dwFlags*/, IPreferences* /*pPrefs*/, LPCTSTR /*szKey*/)
{
	const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pSrcTaskFile, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return IIER_BADINTERFACE;
	}

	CXmlFile fileDest(_T("map"));
	fileDest.SetItemValue(_T("version"), _T("0.9.0"));

	CXmlItem *firstItem = fileDest.AddItem(_T("node"), _T(""));
	firstItem->AddItem(_T("TEXT"), pSrcTaskFile->GetProjectName());	

	CXmlItem * hookItem = firstItem->AddItem(_T("hook"), _T(""));
	hookItem->AddItem(_T("NAME"), _T("accessories/plugins/AutomaticLayout.properties"));

	// Attrib Manager settings
	// This will make the attribs not to be shown as a list view at every node;	
	CXmlItem *attribManItem = firstItem->AddItem(_T("attribute_registry"),_T(""));	
	attribManItem->AddItem(_T("SHOW_ATTRIBUTES"), _T("hide"));

	// export first task
	ExportTask(pTasks, pTasks->GetFirstTask(), firstItem , 0, TRUE);

	// save output manually to restore non-escaping of & and <>
	CString sOutput = Export(fileDest);

	if (!FileMisc::SaveFile(szDestFilePath, sOutput, SFEF_UTF8WITHOUTBOM))
		return IIER_BADFILE;

	return IIER_SUCCESS;
}

IIMPORTEXPORT_RESULT CFMindExporter::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD /*dwFlags*/, IPreferences* /*pPrefs*/, LPCTSTR /*szKey*/)
{
	CXmlFile fileDest(_T("map"));
	fileDest.SetItemValue(_T("version"), _T("0.9.0"));

	for (int nTaskList = 0; nTaskList < pSrcTaskFile->GetTaskListCount(); nTaskList++)
	{
		const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pSrcTaskFile->GetTaskList(nTaskList), IID_TASKLISTBASE);
		
		if (pTasks == NULL)
		{
			ASSERT(0);
			return IIER_BADINTERFACE;
		}

		CXmlItem *firstItem = fileDest.AddItem(_T("node"), _T(""));
		firstItem->AddItem(_T("TEXT"), pTasks->GetProjectName());	

		CXmlItem * hookItem = firstItem->AddItem(_T("hook"), _T(""));
		hookItem->AddItem(_T("NAME"), _T("accessories/plugins/AutomaticLayout.properties"));

		//Attrib Manager settings
		//This will make the attribs not to be shown as a list view at every node;	
		CXmlItem *attribManItem = firstItem->AddItem(_T("attribute_registry"),_T(""));	
		attribManItem->AddItem(_T("SHOW_ATTRIBUTES"), _T("hide"));

		// export first task
		ExportTask(pTasks, pTasks->GetFirstTask(), firstItem, 0, TRUE);
	}

	// save output manually to restore non-escaping of & and <>
	CString sOutput = Export(fileDest);

	if (!FileMisc::SaveFile(szDestFilePath, sOutput, SFEF_UTF8WITHOUTBOM))
		return IIER_BADFILE;

	return IIER_SUCCESS;
}

CString CFMindExporter::Export(const CXmlFile& file)
{
	CString sOutput;
	file.Export(sOutput);
	
	sOutput.Replace(_T("&amp;#"), _T("&#"));
	sOutput.Replace(_T("&lt;br&gt;"), _T("<br>"));

	// and strip off the xml header (FM doesn't like it)
	int nHeaderEnd = sOutput.Find(_T("?>"));
	ASSERT(nHeaderEnd != -1);

	if (nHeaderEnd != -1)
		sOutput = sOutput.Mid(nHeaderEnd + 2);

	return sOutput;
}

CString CFMindExporter::Translate(LPCTSTR szText)
{
	CString sTranslated;
	int nLen = lstrlen(szText);
	
#ifdef _UNICODE
	WCHAR* wszText = (LPTSTR)szText;
#else
	WCHAR* wszText = Misc::MultiByteToWide(szText, nLen);
#endif
	
	for (int i = 0; i < nLen; i++)
	{
		CString str;
		str.Format(_T("&#%d;"), wszText[i]);
		sTranslated += str;
	}
	
#ifndef _UNICODE
	delete [] wszText;
#endif

	return sTranslated;
}

void CFMindExporter::ExportTask(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask, 
								CXmlItem* pXIDestParent, int LEVEL, BOOL bAndSiblings)
{
	if (!hTask)
		return;

	// create a new item corresponding to pXITask at the dest
	CXmlItem* pXIDestItem = pXIDestParent->AddItem(_T("node"));

	// copy across the appropriate attributes
	pXIDestItem->AddItem(_T("TEXT"), Translate(pSrcTaskFile->GetTaskTitle(hTask)));

	time_t tLastMod = pSrcTaskFile->GetTaskLastModified(hTask);
	pXIDestItem->AddItem(_T("MODIFIED"), Misc::Format((DWORD)tLastMod, _T("000")));

	time_t tCreation = pSrcTaskFile->GetTaskCreationDate(hTask);
	pXIDestItem->AddItem(_T("CREATED"), Misc::Format((DWORD)tCreation, _T("000")));

	// FM specific attributes not handled natively by ToDoList
	pXIDestItem->AddItem(_T("BACKGROUND_COLOR"), pSrcTaskFile->GetTaskMetaData(hTask, _T("FM_BKGNDCOLOR")));
	pXIDestItem->AddItem(_T("ID"), pSrcTaskFile->GetTaskMetaData(hTask, _T("FM_ID")));

	// try first for our previous position and if that is empty
	// and we are a top-level task then alternate sides
	CString sPosition = pSrcTaskFile->GetTaskMetaData(hTask, _T("FM_POSITION"));

	if (sPosition.IsEmpty() && LEVEL == 0)
	{
		static bool bRight = true;

		sPosition = bRight ? _T("right") : _T("left");
		bRight = !bRight; 
	}

	if (!sPosition.IsEmpty())
		pXIDestItem->AddItem(_T("POSITION"), sPosition);

	int nTaskPrior = pSrcTaskFile->GetTaskPriority(hTask,FALSE);
	time_t tDone = pSrcTaskFile->GetTaskDoneDate(hTask);
	
	if (tDone)
	{		
		CXmlItem* pXIIcons = pXIDestItem->AddItem(_T("icon"));
		pXIIcons->AddItem(_T("BUILTIN"),_T("button_ok"));
	}

	if (nTaskPrior > 0)
	{		
		CString sIconName;
		sIconName.Format(_T("full-%d"),nTaskPrior);		
		CXmlItem* pXIIcons = pXIDestItem->AddItem(_T("icon"));
		pXIIcons->AddItem(_T("BUILTIN"),sIconName);
	}
	
	// comments
	CString sComments = pSrcTaskFile->GetTaskComments(hTask);
	
	if (!sComments.IsEmpty())
	{
		// for version 0.9 we export comments as rich NOTE
		// now we need to export it as html
		CXmlItem* pXIComments = pXIDestItem->AddItem(_T("richcontent"));
		pXIComments->AddItem(_T("TYPE"), _T("NOTE"));
		
		CXmlItem* pXIHtml = pXIComments->AddItem(_T("html"), _T(""), XIT_ELEMENT);
		/*CXmlItem* pXIHead =*/ pXIHtml->AddItem(_T("head"), _T(""), XIT_ELEMENT);
		CXmlItem* pXIBody = pXIHtml->AddItem(_T("body"), _T(""), XIT_ELEMENT);

		// split the comments by newlines
		CStringArray aParas;
		int nNumPara = Misc::Split(sComments, aParas, '\n', TRUE);

		for (int nPara = 0; nPara < nNumPara; nPara++)
		{
			CString sPara = Translate(aParas[nPara]);
			/*CXmlItem* pXIPara =*/ pXIBody->AddItem(_T("p"), sPara, XIT_ELEMENT);
		}
	}
	
	// Start of Attrib List not Supported by FreeMind

	// arrays
	ADDCUSTOMATTRIBARRAY(FM_CUSTOMALLOCTO, GetTaskAllocatedToCount, GetTaskAllocatedTo)
	ADDCUSTOMATTRIBARRAY(FM_CUSTOMCATEGORIES, GetTaskCategoryCount, GetTaskCategory)
	ADDCUSTOMATTRIBARRAY(FM_CUSTOMDEPENDS, GetTaskDependencyCount, GetTaskDependency)
	ADDCUSTOMATTRIBARRAY(FM_CUSTOMTAGS, GetTaskTagCount, GetTaskTag)

	// simple attribs
	ADDCUSTOMATTRIB(FM_CUSTOMTASKID, (int)pSrcTaskFile->GetTaskID(hTask))
	ADDCUSTOMATTRIB(FM_CUSTOMALLOCBY, pSrcTaskFile->GetTaskAllocatedBy(hTask))
	ADDCUSTOMATTRIB(FM_CUSTOMSTATUS, pSrcTaskFile->GetTaskStatus(hTask))
	ADDCUSTOMATTRIB(FM_CUSTOMFILELINK, pSrcTaskFile->GetTaskFileLinkPath(hTask))
	ADDCUSTOMATTRIB(FM_CUSTOMCOLOR, (int)pSrcTaskFile->GetTaskColor(hTask))
	ADDCUSTOMATTRIB(FM_CUSTOMPRIORITY, pSrcTaskFile->GetTaskPriority(hTask, false))
	ADDCUSTOMATTRIB(FM_CUSTOMRISK, pSrcTaskFile->GetTaskRisk(hTask, false))
	ADDCUSTOMATTRIB(FM_CUSTOMPERCENT, pSrcTaskFile->GetTaskPercentDone(hTask, false))
	ADDCUSTOMATTRIB(FM_CUSTOMPOS, (int)pSrcTaskFile->GetTaskPosition(hTask))
	ADDCUSTOMATTRIB(FM_CUSTOMFLAG, pSrcTaskFile->IsTaskFlagged(hTask, false))
	ADDCUSTOMATTRIB(FM_CUSTOMCREATEDBY, pSrcTaskFile->GetTaskCreatedBy(hTask))
	ADDCUSTOMATTRIB(FM_CUSTOMEXTID, pSrcTaskFile->GetTaskExternalID(hTask))
	ADDCUSTOMATTRIB(FM_CUSTOMCOST, pSrcTaskFile->GetTaskAttribute(hTask, TDCA_COST))
	ADDCUSTOMATTRIB(FM_CUSTOMVERSION, pSrcTaskFile->GetTaskVersion(hTask))

	// dates
	ADDCUSTOMATTRIB(FM_CUSTOMDONEDATE, pSrcTaskFile->GetTaskDoneDateString(hTask))
	ADDCUSTOMATTRIB(FM_CUSTOMDUEDATE, pSrcTaskFile->GetTaskDueDateString(hTask, false))
	ADDCUSTOMATTRIB(FM_CUSTOMSTARTDATE, pSrcTaskFile->GetTaskStartDateString(hTask, false))

	// times
	TDC_UNITS nUnits;
	ADDCUSTOMATTRIB(FM_CUSTOMTIMEEST, pSrcTaskFile->GetTaskTimeEstimate(hTask, nUnits, false))
	ADDCUSTOMATTRIB(FM_CUSTOMTIMEESTUNITS, nUnits)
	ADDCUSTOMATTRIB(FM_CUSTOMTIMESPENT, pSrcTaskFile->GetTaskTimeSpent(hTask, nUnits, false))
	ADDCUSTOMATTRIB(FM_CUSTOMTIMESPENTUNITS, nUnits)

	// recurrence
	TDC_REGULARITY nRegularity;
	TDC_RECURREUSEOPTION nReuse;
	TDC_RECURFROMOPTION nRecalcFrom;

	DWORD dwSpecific1 = 0, dwSpecific2 = 0;
	int nNumOccur = -1, nRemainingOccur = -1;
	bool bPreserveComments = false;

	if (pSrcTaskFile->GetTaskRecurrence(hTask, nRegularity, dwSpecific1, dwSpecific2, 
										nRecalcFrom, nReuse, nNumOccur, nRemainingOccur, bPreserveComments))
	{
		ADDCUSTOMATTRIB(FM_CUSTOMREGULARITY, nRegularity)
		ADDCUSTOMATTRIB(FM_CUSTOMRECURSPEC1, (int)dwSpecific1)
		ADDCUSTOMATTRIB(FM_CUSTOMRECURSPEC2, (int)dwSpecific2)
		ADDCUSTOMATTRIB(FM_CUSTOMRECURRECALCFROM, nRecalcFrom)
		ADDCUSTOMATTRIB(FM_CUSTOMRECURREUSE, nReuse)
		ADDCUSTOMATTRIB(FM_CUSTOMNUMOCCUR, nNumOccur)
		ADDCUSTOMATTRIB(FM_CUSTOMREMAININGOCCUR, nRemainingOccur)
		ADDCUSTOMATTRIB(FM_CUSTOMPRESERVECOMMENTS, bPreserveComments)
	}
	
	// End of Attrib List not Supported by FreeMind

	// copy across first child
	ExportTask(pSrcTaskFile, pSrcTaskFile->GetFirstTask(hTask), pXIDestItem, LEVEL + 1, TRUE);

	// copy sibling tasks WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pSrcTaskFile->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == don't recurse on siblings
			ExportTask(pSrcTaskFile, hSibling, pXIDestParent, LEVEL, FALSE);
			hSibling = pSrcTaskFile->GetNextTask(hSibling);
		}
	}
}



