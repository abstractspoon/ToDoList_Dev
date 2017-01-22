// FMindExporter.cpp: implementation of the CFMindExporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FMindImportExport.h"
#include "FMindExporter.h"
#include "FMdefines.h"

#include "..\shared\xmlfileex.h"
#include "..\shared\datehelper.h"
#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
//#include "..\shared\localizer.h"

//#include "..\todolist\tdlschemadef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

#define ADDCUSTOMATTRIB(name, value)									\
{																		\
	pXIAttribs = pXIDestItem->AddItem(_T("attribute"));					\
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

CFMindExporter::CFMindExporter() : m_hIcon(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_FREEMIND);
}

CFMindExporter::~CFMindExporter()
{
	::DestroyIcon(m_hIcon);
}

void CFMindExporter::SetLocalizer(ITransText* /*pTT*/)
{
	//CLocalizer::Initialize(pTT);
}

bool CFMindExporter::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool /*bSilent*/, IPreferences* /*pPrefs*/, LPCTSTR /*szKey*/)
{
	CXmlFile fileDest(_T("map"));
	fileDest.SetItemValue(_T("version"), _T("0.9.0"));

	CXmlItem *firstItem = fileDest.AddItem(_T("node"), _T(""));
	firstItem->AddItem(_T("TEXT"), pSrcTaskFile->GetProjectName());	

	CXmlItem * hookItem = firstItem->AddItem(_T("hook"), _T(""));
	hookItem->AddItem(_T("NAME"), _T("accessories/plugins/AutomaticLayout.properties"));

	//Attrib Manager settings
	//This will make the attribs not to be shown as a list view at every node;	
	CXmlItem *attribManItem = firstItem->AddItem(_T("attribute_registry"),_T(""));	
	attribManItem->AddItem(_T("SHOW_ATTRIBUTES"), _T("hide"));

	// export first task
	const ITaskList10* pITL10 = GetITLInterface<ITaskList10>(pSrcTaskFile, IID_TASKLIST10);
	ASSERT (pITL10);

	ExportTask(pITL10, pITL10->GetFirstTask(), firstItem , 0, TRUE);

	// save output manually to restore non-escaping of & and <>
	CString sOutput = Export(fileDest);

	return (FileMisc::SaveFile(szDestFilePath, sOutput, SFEF_UTF8WITHOUTBOM) != FALSE);
}

bool CFMindExporter::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool /*bSilent*/, IPreferences* /*pPrefs*/, LPCTSTR /*szKey*/)
{
	CXmlFile fileDest(_T("map"));
	fileDest.SetItemValue(_T("version"), _T("0.9.0"));

	for (int nTaskList = 0; nTaskList < pSrcTaskFile->GetTaskListCount(); nTaskList++)
	{
		const ITaskList10* pITL10 = GetITLInterface<ITaskList10>(pSrcTaskFile->GetTaskList(nTaskList), IID_TASKLIST8);
		
		if (pITL10)
		{
			CXmlItem *firstItem = fileDest.AddItem(_T("node"), _T(""));

			CString sProjectName = pITL10->GetProjectName();

// 			if (sProjectName.IsEmpty())
// 				sProjectName = FileMisc::GetFileNameFromPath(pITL8->GetAttribute(TDL_FILENAME), FALSE);

			firstItem->AddItem(_T("TEXT"), sProjectName);	

			CXmlItem * hookItem = firstItem->AddItem(_T("hook"), _T(""));
			hookItem->AddItem(_T("NAME"), _T("accessories/plugins/AutomaticLayout.properties"));

			//Attrib Manager settings
			//This will make the attribs not to be shown as a list view at every node;	
			CXmlItem *attribManItem = firstItem->AddItem(_T("attribute_registry"),_T(""));	
			attribManItem->AddItem(_T("SHOW_ATTRIBUTES"), _T("hide"));

			// export first task
			ExportTask(pITL10, pITL10->GetFirstTask(), firstItem, 0, TRUE);
		}
	}

	// save output manually to restore non-escaping of & and <>
	CString sOutput = Export(fileDest);

	return (FileMisc::SaveFile(szDestFilePath, sOutput, SFEF_UTF8WITHOUTBOM) != FALSE);
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

void CFMindExporter::ExportTask(const ITaskList10* pSrcTaskFile, HTASKITEM hTask, 
								CXmlItem* pXIDestParent, int LEVEL, BOOL bAndSiblings)
{
	if (!hTask)
		return;

	// create a new item corresponding to pXITask at the dest
	CXmlItem* pXIDestItem = pXIDestParent->AddItem(_T("node"));

	// copy across the appropriate attributes
	pXIDestItem->AddItem(_T("TEXT"), Translate(pSrcTaskFile->GetTaskTitle(hTask)));
	pXIDestItem->AddItem(_T("MODIFIED"), (int)(pSrcTaskFile->GetTaskLastModified(hTask) * 1000));
	pXIDestItem->AddItem(_T("CREATED"), (int)(pSrcTaskFile->GetTaskCreationDate(hTask) * 1000));

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
	CXmlItem* pXIAttribs = NULL; 
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
	ADDCUSTOMATTRIB(FM_CUSTOMFILEREF, pSrcTaskFile->GetTaskFileLinkPath(hTask))
	ADDCUSTOMATTRIB(FM_CUSTOMCOLOR, (int)pSrcTaskFile->GetTaskColor(hTask))
	ADDCUSTOMATTRIB(FM_CUSTOMPRIORITY, pSrcTaskFile->GetTaskPriority(hTask, FALSE))
	ADDCUSTOMATTRIB(FM_CUSTOMRISK, pSrcTaskFile->GetTaskRisk(hTask, FALSE))
	ADDCUSTOMATTRIB(FM_CUSTOMPERCENT, pSrcTaskFile->GetTaskPercentDone(hTask, FALSE))
	ADDCUSTOMATTRIB(FM_CUSTOMPOS, (int)pSrcTaskFile->GetTaskPosition(hTask))
	ADDCUSTOMATTRIB(FM_CUSTOMFLAG, pSrcTaskFile->IsTaskFlagged(hTask))
	ADDCUSTOMATTRIB(FM_CUSTOMCREATEDBY, pSrcTaskFile->GetTaskCreatedBy(hTask))
	ADDCUSTOMATTRIB(FM_CUSTOMEXTID, pSrcTaskFile->GetTaskExternalID(hTask))
	ADDCUSTOMATTRIB(FM_CUSTOMCOST, pSrcTaskFile->GetTaskCost(hTask, FALSE))
	ADDCUSTOMATTRIB(FM_CUSTOMVERSION, pSrcTaskFile->GetTaskVersion(hTask))

	// dates
	ADDCUSTOMATTRIB(FM_CUSTOMDONEDATE, pSrcTaskFile->GetTaskDoneDateString(hTask))
	ADDCUSTOMATTRIB(FM_CUSTOMDUEDATE, pSrcTaskFile->GetTaskDueDateString(hTask, FALSE))
	ADDCUSTOMATTRIB(FM_CUSTOMSTARTDATE, pSrcTaskFile->GetTaskStartDateString(hTask))

	// times
	TDC_UNITS nUnits;
	ADDCUSTOMATTRIB(FM_CUSTOMTIMEEST, pSrcTaskFile->GetTaskTimeEstimate(hTask, nUnits, FALSE))
	ADDCUSTOMATTRIB(FM_CUSTOMTIMEESTUNITS, nUnits)
	ADDCUSTOMATTRIB(FM_CUSTOMTIMESPENT, pSrcTaskFile->GetTaskTimeSpent(hTask, nUnits, FALSE))
	ADDCUSTOMATTRIB(FM_CUSTOMTIMESPENTUNITS, nUnits)

	// recurrence
	int nRegularity = 0, nReuse = 0;
	DWORD dwSpecific1 = 0, dwSpecific2 = 0;
	int nRecalcFrom = 0;

	if (pSrcTaskFile->GetTaskRecurrence(hTask, nRegularity, dwSpecific1, dwSpecific2, nRecalcFrom, nReuse))
	{
		ADDCUSTOMATTRIB(FM_CUSTOMREGULARITY, nRegularity)
		ADDCUSTOMATTRIB(FM_CUSTOMRECURSPEC1, (int)dwSpecific1)
		ADDCUSTOMATTRIB(FM_CUSTOMRECURSPEC2, (int)dwSpecific2)
		ADDCUSTOMATTRIB(FM_CUSTOMRECURRECALCFROM, nRecalcFrom)
		ADDCUSTOMATTRIB(FM_CUSTOMRECURREUSE, nReuse)
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

CString CFMindExporter::FormatDate(time_t tDate)
{
	if (tDate)
	{
		COleDateTime date(tDate);
		return CDateHelper::FormatDate(date, DHFD_ISO | DHFD_TIME);
	}

	// else
	return "";
}


