// MLOExporter.cpp: implementation of the CMLOExporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MLOImport.h"
#include "MLOExporter.h"

#include "..\shared\xmlfileex.h"
#include "..\shared\datehelper.h"
#include "..\shared\timehelper.h"
#include "..\shared\misc.h"

#include "..\3rdParty\T64Utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMLOExporter::CMLOExporter()
{
	m_icon.Load(IDI_MYLIFEORGANISED);
}

CMLOExporter::~CMLOExporter() 
{
}

void CMLOExporter::SetLocalizer(ITransText* /*pTT*/)
{
	//CLocalizer::Initialize(pTT);
}

IIMPORTEXPORT_RESULT CMLOExporter::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD /*dwFlags*/, IPreferences* /*pPrefs*/, LPCTSTR /*szKey*/)
{
	const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pSrcTaskFile, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return IIER_BADINTERFACE;
	}
	
	CXmlFile fileDest(_T("MyLifeOrganized-xml"));
	fileDest.SetXmlHeader(DEFAULT_UTF8_HEADER);
	fileDest.AddItem(_T("ver"), _T("1.2"));

	// export tasks
	CXmlItem* pXITasks = fileDest.AddItem(_T("TaskTree"));
	
	if (!ExportTask(pTasks, pSrcTaskFile->GetFirstTask(), pXITasks, TRUE))
		return IIER_SOMEFAILED;
	
	// export resource allocations
	ExportPlaces(pTasks, fileDest.Root());
	
	// save result
	if (!fileDest.Save(szDestFilePath, SFEF_UTF8WITHOUTBOM))
		return IIER_BADFILE;

	return IIER_SUCCESS;
}

IIMPORTEXPORT_RESULT CMLOExporter::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD /*dwFlags*/, IPreferences* /*pPrefs*/, LPCTSTR /*szKey*/)
{
	CXmlFile fileDest(_T("MyLifeOrganized-xml"));
	fileDest.SetXmlHeader(DEFAULT_UTF8_HEADER);
	fileDest.AddItem(_T("ver"), _T("1.2"));

	// export tasks
	CXmlItem* pXITasks = fileDest.AddItem(_T("TaskTree"));
	
	for (int nTaskList = 0; nTaskList < pSrcTaskFile->GetTaskListCount(); nTaskList++)
	{
		const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pSrcTaskFile->GetTaskList(nTaskList), IID_TASKLISTBASE);
		
		if (pTasks == NULL)
		{
			ASSERT(0);
			return IIER_BADINTERFACE;
		}

		// export tasks
		if (!ExportTask(pTasks, pTasks->GetFirstTask(), pXITasks, TRUE))
			return IIER_SOMEFAILED;
			
		// export resource allocations
		ExportPlaces(pTasks, fileDest.Root());
	}
	
	// save result
	if (!fileDest.Save(szDestFilePath, SFEF_UTF8WITHOUTBOM))
		return IIER_BADFILE;

	return IIER_SUCCESS;
}

CString CMLOExporter::FormatDestID(const CString& sTitle, DWORD dwID) const
{
	unsigned long nTaskNameCode = sTitle.GetLength();
	ASSERT(nTaskNameCode);

	if (nTaskNameCode > 1)
		nTaskNameCode *= int(sTitle[1]);

	return Misc::Format(_T("{%08lu-0000-1111-0000-%012lu}"), dwID, nTaskNameCode);
}

bool CMLOExporter::ExportTask(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask, 
							  CXmlItem* pXIDestParent, BOOL bAndSiblings)
{
	if (!hTask)
		return true;
	
	// Create a new item corresponding to pXITask at the dest
	CXmlItem* pXIDestItem = pXIDestParent->AddItem(_T("TaskNode"));
	
	if (!pXIDestItem)
		return false;
	
	// Copy across the appropriate attributes
	pXIDestItem->AddItem(_T("Caption"), pSrcTaskFile->GetTaskTitle(hTask));
	
	// Priority
	int nPriority = pSrcTaskFile->GetTaskPriority(hTask, FALSE);
	int nImportance = max(nPriority, 0) * 20; // The scale of MyLifeOrganized is over 200, not over 100.
	
	pXIDestItem->AddItem(_T("Importance"), nImportance, XIT_ELEMENT);

	// Dates
	time64_t tDate = T64Utils::T64_NULL;
	
	if (pSrcTaskFile->GetTaskCreationDate64(hTask, tDate))
		pXIDestItem->AddItem(_T("Created"), FormatDate(tDate), XIT_ELEMENT);
	
	if (pSrcTaskFile->GetTaskDoneDate64(hTask, tDate))
		pXIDestItem->AddItem(_T("CompletionDateTime"), FormatDate(tDate), XIT_ELEMENT);

	if (pSrcTaskFile->GetTaskDueDate64(hTask, false, tDate))
		pXIDestItem->AddItem(_T("DueDateTime"), FormatDate(tDate), XIT_ELEMENT);
	
	// Time estimate
	TDC_UNITS nUnits;
	double dTimeEst = pSrcTaskFile->GetTaskTimeEstimate(hTask, nUnits, FALSE);
	
	if (dTimeEst > 0.0)
	{
		TH_UNITS nTHUnits = MapUnitsToTHUnits(nUnits);
		pXIDestItem->AddItem(_T("EstimateMax"), CTimeHelper().Convert(dTimeEst, nTHUnits, THU_DAYS), XIT_ELEMENT);
	}

	// Flag
	if (pSrcTaskFile->IsTaskFlagged(hTask, false))
		pXIDestItem->AddItem(_T("Flag"), _T("Red Flag"), XIT_ELEMENT);

	// Colours and Font
	CXmlItem* pXIDestFormat = pXIDestItem->AddItem(_T("CustomFormat"), _T(""), XIT_ELEMENT);

	if (pXIDestFormat)
	{
		// Top-level tasks are bold
		if (pSrcTaskFile->GetTaskParentID(hTask) == 0)
			pXIDestFormat->AddItem(_T("Bold"), _T("1"), XIT_ELEMENT);

		// Task Color
		COLORREF crTaskColor = pSrcTaskFile->GetTaskColor(hTask);

		pXIDestFormat->AddItem(_T("FontColor"), Misc::Format(crTaskColor), XIT_ELEMENT);
		pXIDestFormat->AddItem(_T("SideBarColor"), Misc::Format(crTaskColor), XIT_ELEMENT);
	}
	
	// Note (Comments and other attributes not natively supported by MLO)
	CString sNote;

	sNote += FormatFileLinks(pSrcTaskFile, hTask);
	sNote += FormatDependencies(pSrcTaskFile, hTask, pXIDestItem);
	sNote += FormatComments(pSrcTaskFile, hTask);

	// Notes must be added as elements now
	if (!sNote.IsEmpty())
		pXIDestItem->AddItem(_T("Note"), sNote, XIT_ELEMENT);

	// Folders and projects
	// These are not available in ToDoList, but we set a criteria for setting those in MyLifeOrganized:
	if (pSrcTaskFile->IsTaskParent(hTask)) // If they have sub-tasks...
	{
		// We consider it a folder if it has no notes, otherwise it is a project:
		if (sNote.IsEmpty())
			pXIDestItem->AddItem(_T("HideInToDoThisTask"), -1, XIT_ELEMENT); // Folders are marked with: HideInToDoThisTask
		else
			pXIDestItem->AddItem(_T("IsProject"), -1, XIT_ELEMENT); // Projects are marked with: IsProject
	}

	// copy across first child
	if (!ExportTask(pSrcTaskFile, pSrcTaskFile->GetFirstTask(hTask), pXIDestItem, TRUE))
		return false;
	
	// handle sibling tasks WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pSrcTaskFile->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == don't recurse on siblings
			if (!ExportTask(pSrcTaskFile, hSibling, pXIDestParent, FALSE))
				return false;

			hSibling = pSrcTaskFile->GetNextTask(hSibling);
		}
	}

	return true;
}

CString CMLOExporter::FormatComments(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask) const
{
	CString sComments;

	// Remove parts of HTML that MyLifeOrganized cannot process/recognize (when generated from rich text):
	// Sample of what is actually generated from rich text:
	//      <div class=WordSection1>
	//      <p class=MsoNormal style='margin-top:0cm;margin-bottom:.0001pt;line-height:
	//      normal;text-autospace:none'><span lang=EN-GB style='font-size:8.0pt;font-family:
	//      "Arial","sans-serif"'>This is a test in bold, <span style='color:red;
	//      background:yellow'>colored</span>, Underlined.</span></p>
	//      <p class=MsoNormal style='margin-top:0cm;margin-bottom:.0001pt;line-height:
	//      normal;text-autospace:none'><span style='font-size:8.0pt;font-family:"Arial","sans-serif"'> </span></p>
	//      </div>
	//
	CString sHtmlComments = pSrcTaskFile->GetTaskAttribute(hTask, TDCA_HTMLCOMMENTS), sHtmlSource;

	if (!sHtmlComments.IsEmpty())
	{
		if (sHtmlComments.Replace(_T("<div class=WordSection1>"), _T("")))
		{
			sHtmlComments.Replace(_T("</div>"), _T(""));
			sHtmlComments.Replace(_T("style='font-size:8.0pt;font-family:"), _T(""));
			sHtmlComments.Replace(_T("\"Arial\",\"sans-serif\"'"), _T(""));
			sHtmlComments.Replace(_T("lang=EN-GB"), _T(""));
			sHtmlComments.Replace(_T("class=MsoNormal style='margin-top:0cm;margin-bottom:.0001pt;line-height:"), _T(""));
			sHtmlComments.Replace(_T("normal;text-autospace:none'"), _T(""));
			sHtmlComments.Replace(_T("<p\r\n>"), _T("<p>"));
			sHtmlComments.Replace(_T("<p\n>"), _T("<p>"));
			sHtmlComments.Replace(_T("<span\r\n>"), _T("<span>"));
			sHtmlComments.Replace(_T("<span\n>"), _T("<span>"));

			sHtmlSource = _T("RTF"); // Type of comment in ToDoList
		}
		else
		{
			sHtmlSource = _T("HTML");
		}

		// Warn about the format issue in MyLifeOrganized:
		sComments += Misc::Format(_T("ToDoList Comments (from %s): *** To see format in notes, activate \"Use Markdown format in notes\" in the MyLifeOrganized options ***\r\n\r\n"), sHtmlSource);
		sComments += _T("\r\n\r\n");
	}

	// Plain text comments
	CString sPlainText = pSrcTaskFile->GetTaskComments(hTask);

	if (!sPlainText.IsEmpty())
	{
		if (!sHtmlComments.IsEmpty())
			sComments += _T("----- *** ToDoList Plain Text (HTML below) *** -----\r\n\r\n");

		sComments += sPlainText;
		sComments += _T("\r\n\r\n");
	}

	// HTML comments
	if (!sHtmlComments.IsEmpty())
	{
		sComments += Misc::Format(_T("----- *** ToDoList HTML (from %s) *** (MyLifeOrganized supports only basic HTML formatting and Markdown) -----"), sHtmlSource);
		sComments += _T("\r\n\r\n");
		sComments += sHtmlComments;
	}

	return sComments;
}

CString CMLOExporter::FormatDependencies(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask, CXmlItem* pXIDestItem) const
{
	CString sDepends;
	int nNumDepends = pSrcTaskFile->GetTaskDependencyCount(hTask);

	if (nNumDepends > 0)
	{
		// Dependency
		CXmlItem* pXIDestDepends = pXIDestItem->AddItem(_T("Dependency"), _T(""), XIT_ELEMENT);

		for (int nDependency = 0; nDependency < nNumDepends; ++nDependency)
		{
			CString sDependency(pSrcTaskFile->GetTaskDependency(hTask, nDependency));
			sDepends += Misc::Format(_T("ToDoList Dependency (%d): [ID %s] "), (nDependency + 1), sDependency);

			DWORD dwID = _wtoi(sDependency);

			if (dwID)
			{
				HTASKITEM hTaskDependency = pSrcTaskFile->FindTask(dwID);

				if (hTaskDependency)
				{
					CString sDependsTitle = pSrcTaskFile->GetTaskTitle(hTaskDependency);
					sDepends += sDependsTitle;

					CString sID = FormatDestID(sDependsTitle, pSrcTaskFile->GetTaskID(hTaskDependency));
					pXIDestDepends->AddItem(_T("UID"), sID, XIT_ELEMENT);
				}
			}

			sDepends += _T("\r\n\r\n");
		}
	}

	return sDepends;
}

CString CMLOExporter::FormatFileLinks(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask) const
{
	static const CString urlProtocol = _T("://");
	static const CString fileProtocol = _T("file://");

	CString sLinks;
	int nLinks = pSrcTaskFile->GetTaskFileLinkCount(hTask);

	if (nLinks == 1)
	{
		CString sLink = pSrcTaskFile->GetTaskFileLinkPath(hTask);
		sLinks += _T("ToDoList Link: <");

		// Is it a local file link without an URL-protocol? Then add the fileProtocol...
		if (sLink.Find(urlProtocol) < 0)
			sLinks += fileProtocol;

		sLinks += sLink;
		sLinks += _T(">\r\n\r\n");
	}
	else if (nLinks > 1)
	{
		for (int nLinkCount = 0; nLinkCount < nLinks; ++nLinkCount)
		{
			CString sLink = pSrcTaskFile->GetTaskFileLink(hTask, nLinkCount);
			sLinks += Misc::Format(_T("ToDoList Link (%d): <"), (nLinkCount + 1));

			// Is it a local file link without an URL-protocol? Then add the fileProtocol...
			if (sLink.Find(urlProtocol) < 0)
				sLinks += fileProtocol;

			sLinks += sLink;
			sLinks += _T(">\r\n\r\n");
		}
	}

	return sLinks;
}

CString CMLOExporter::FormatDate(time64_t tDate)
{
	COleDateTime date = CDateHelper::GetDate(tDate);

	return CDateHelper::FormatDate(date, DHFD_ISO | DHFD_TIME, 'T');
}

void CMLOExporter::BuildPlacesMap(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask, 
								  CMapStringToString& mapPlaces, BOOL bAndSiblings)
{
	if (!hTask)
		return;
	
	int nCat = pSrcTaskFile->GetTaskCategoryCount(hTask);
	
	while (nCat--)
	{
		CString sCat = pSrcTaskFile->GetTaskCategory(hTask, nCat);
		CString sCatUpper(sCat);
		sCat.MakeUpper();
		
		mapPlaces[sCatUpper] = sCat;
	}
	
	// children
	BuildPlacesMap(pSrcTaskFile, pSrcTaskFile->GetFirstTask(hTask), mapPlaces, TRUE);
	
	// handle sibling tasks WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pSrcTaskFile->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == don't recurse on siblings
			BuildPlacesMap(pSrcTaskFile, hSibling, mapPlaces, FALSE);
			hSibling = pSrcTaskFile->GetNextTask(hSibling);
		}
	}
}

void CMLOExporter::ExportPlaces(const ITASKLISTBASE* pSrcTaskFile, CXmlItem* pDestPrj)
{
	CMapStringToString mapPlaces;
	BuildPlacesMap(pSrcTaskFile, pSrcTaskFile->GetFirstTask(), mapPlaces, TRUE);
	
	if (mapPlaces.GetCount())
	{
		CXmlItem* pXIResources = pDestPrj->AddItem(_T("PlacesList"));
		CString sPlace, sPlaceUpper;
		
		POSITION pos = mapPlaces.GetStartPosition();
		
		while (pos)
		{
			mapPlaces.GetNextAssoc(pos, sPlaceUpper, sPlace);
			
			CXmlItem* pXIRes = pXIResources->AddItem(_T("TaskPlace"));
			
			if (pXIRes)
				pXIRes->AddItem(_T("Caption"), sPlace);
		}
	}
}

TH_UNITS CMLOExporter::MapUnitsToTHUnits(TDC_UNITS nUnits)
{
	switch (nUnits)
	{
	case TDCU_MINS:		return THU_MINS;
	case TDCU_HOURS:	return THU_HOURS;
	case TDCU_DAYS:		return THU_DAYS;
	case TDCU_WEEKDAYS:	return THU_WEEKDAYS;
	case TDCU_WEEKS:	return THU_WEEKS;
	case TDCU_MONTHS:	return THU_MONTHS;
	case TDCU_YEARS:	return THU_YEARS;
	}
	
	// all else
	ASSERT(0);
	return THU_NULL;
}