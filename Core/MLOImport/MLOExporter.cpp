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
#include "..\shared\filemisc.h"

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
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CITaskListArray aTasklists;
	aTasklists.Add(GetITLInterface<ITASKLISTBASE>(pSrcTaskFile, IID_TASKLISTBASE));

	return ExportTasklists(aTasklists, szDestFilePath);
}

IIMPORTEXPORT_RESULT CMLOExporter::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD /*dwFlags*/, IPreferences* /*pPrefs*/, LPCTSTR /*szKey*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CITaskListArray aTasklists;

	for (int nTaskList = 0; nTaskList < pSrcTaskFile->GetTaskListCount(); nTaskList++)
		aTasklists.Add(GetITLInterface<ITASKLISTBASE>(pSrcTaskFile->GetTaskList(nTaskList), IID_TASKLISTBASE));

	return ExportTasklists(aTasklists, szDestFilePath);
}

IIMPORTEXPORT_RESULT CMLOExporter::ExportTasklists(const CITaskListArray& aTasklists, LPCTSTR szDestFilePath) const
{
	CXmlFile fileDest(_T("MyLifeOrganized-xml"));
	BOOL bMulti = (aTasklists.GetSize() > 1), bSomeFailed = FALSE;

	fileDest.SetXmlHeader(DEFAULT_UTF8_HEADER);
	fileDest.AddItem(_T("ver"), _T("1.2"));

	// export tasks
	CXmlItem* pXIAllTasks = fileDest.AddItem(_T("TaskTree"));

	for (int nTaskList = 0; nTaskList < aTasklists.GetSize(); nTaskList++)
	{
		const ITASKLISTBASE* pTasks = aTasklists[nTaskList];
		
		if (pTasks == NULL)
		{
			ASSERT(0);
			return IIER_BADINTERFACE;
		}

		CXmlItem* pXITasks = pXIAllTasks;
		
		// For a multi-file export create a top-level node for each tasklist
		if (bMulti)
		{
			CString sTitle = FormatTitle(pTasks->GetReportTitle(), pTasks->GetReportDate());
			pXITasks = CreateTaskNode(sTitle, (nTaskList * SHORT_MAX), pXIAllTasks);
		}

		// export tasks
		if (!pXITasks || !ExportTask(pTasks, pTasks->GetFirstTask(), pXITasks, TRUE))
			bSomeFailed = TRUE;

		// export resource allocations
		ExportPlaces(pTasks, fileDest.Root());
	}
	
	// save result
	if (!fileDest.Save(szDestFilePath, SFEF_UTF8WITHOUTBOM))
		return IIER_BADFILE;

	return (bSomeFailed ? IIER_SOMEFAILED : IIER_SUCCESS);
}

CString CMLOExporter::FormatTitle(LPCTSTR szReportTitle, LPCTSTR szReportDate)
{
	if (Misc::IsEmpty(szReportDate))
		return szReportTitle;

	if (Misc::IsEmpty(szReportTitle))
		return szReportDate;

	return Misc::Format(_T("%s (%s)"), szReportTitle, szReportDate);
}

CXmlItem* CMLOExporter::CreateTaskNode(LPCTSTR szTitle, DWORD dwID, CXmlItem* pXIDestParent) const
{
	CXmlItem* pXITask = pXIDestParent->AddItem(_T("TaskNode"));
	ASSERT(pXITask);

	if (pXITask)
	{
		pXITask->AddItem(_T("Caption"), szTitle);
		pXITask->AddItem(_T("IDD"), FormatDestID(szTitle, dwID), XIT_ELEMENT);
	}

	return pXITask;
}

CString CMLOExporter::FormatDestID(const CString& sTitle, DWORD dwID)
{
	unsigned long nTaskNameCode = sTitle.GetLength();
	ASSERT(nTaskNameCode);

	if (nTaskNameCode > 1)
		nTaskNameCode *= int(sTitle[1]);

	return Misc::Format(_T("{%08lu-0000-1111-0000-%012lu}"), dwID, nTaskNameCode);
}

bool CMLOExporter::ExportTask(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask, 
							  CXmlItem* pXIDestParent, BOOL bAndSiblings) const
{
	if (!hTask)
		return true;
	
	CXmlItem* pXIDestItem = CreateTaskNode(pSrcTaskFile->GetTaskTitle(hTask), 
										   pSrcTaskFile->GetTaskID(hTask),
										   pXIDestParent);

	if (!pXIDestItem)
	{
		ASSERT(0);
		return false;
	}
	
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

	AddSpacedContent(FormatFileLinks(pSrcTaskFile, hTask), sNote);
	AddSpacedContent(FormatDependencies(pSrcTaskFile, hTask, pXIDestItem), sNote);
	AddSpacedContent(FormatComments(pSrcTaskFile, hTask), sNote);

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

void CMLOExporter::AddSpacedContent(const CString& sSrc, CString& sDest)
{
	if (!sSrc.IsEmpty())
	{
		if (!sDest.IsEmpty())
			sDest += _T("\r\n\r\n");

		sDest += sSrc;
	}
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
	Misc::Trim(sHtmlComments);

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
		AddSpacedContent(Misc::Format(_T("ToDoList Comments (from %s): *** To see format in notes, activate \"Use Markdown format in notes\" in the MyLifeOrganized options ***\r\n\r\n"), sHtmlSource), sComments);
	}

	// Plain text comments
	CString sPlainText = pSrcTaskFile->GetTaskComments(hTask);
	Misc::Trim(sPlainText);

	if (!sPlainText.IsEmpty())
	{
		if (!sHtmlComments.IsEmpty())
			AddSpacedContent(_T("----- *** ToDoList Plain Text (HTML below) *** -----"), sComments);

		AddSpacedContent(sPlainText, sComments);
	}

	// HTML comments
	if (!sHtmlComments.IsEmpty())
	{
		AddSpacedContent(Misc::Format(_T("----- *** ToDoList HTML (from %s) *** (MyLifeOrganized supports only basic HTML formatting and Markdown) -----"), sHtmlSource), sComments);
		AddSpacedContent(sHtmlComments, sComments);
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

		for (int nDepend = 0; nDepend < nNumDepends; nDepend++)
		{
			CString sDepend(pSrcTaskFile->GetTaskDependency(hTask, nDepend));
			DWORD dwDependsID = _wtoi(sDepend);

			sDepend = Misc::Format(_T("ToDoList Dependency (%d): [ID %s] "), (nDepend + 1), sDepend);

			if (dwDependsID) // Local dependency
			{
				HTASKITEM hTaskDependency = pSrcTaskFile->FindTask(dwDependsID);

				if (hTaskDependency)
				{
					CString sDependTitle = pSrcTaskFile->GetTaskTitle(hTaskDependency);
					sDepend += sDependTitle;

					CString sID = FormatDestID(sDependTitle, pSrcTaskFile->GetTaskID(hTaskDependency));
					pXIDestDepends->AddItem(_T("UID"), sID, XIT_ELEMENT);
				}
			}

			AddSpacedContent(sDepend, sDepends);
		}
	}

	return sDepends;
}

CString CMLOExporter::FormatFileLinks(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask) const
{
	static const CString urlProtocol = _T("://");
	static const CString fileProtocol = _T("file://");

	CString sLinks;
	int nNumLinks = pSrcTaskFile->GetTaskFileLinkCount(hTask);

	if (nNumLinks == 1)
	{
		CString sLink = pSrcTaskFile->GetTaskFileLinkPath(hTask);

		// Is it a local file link without an URL-protocol? Then add the fileProtocol...
		if (sLink.Find(urlProtocol) == -1)
			sLink = fileProtocol + sLink;

		sLink = Misc::Format(_T("ToDoList Link: <%s>"), sLink);
		AddSpacedContent(sLink, sLinks);
	}
	else if (nNumLinks > 1)
	{
		for (int nLink = 0; nLink < nNumLinks; nLink)
		{
			CString sLink = pSrcTaskFile->GetTaskFileLink(hTask, nLink);

			// Is it a local file link without an URL-protocol? Then add the fileProtocol...
			if (sLink.Find(urlProtocol) == -1)
				sLink = fileProtocol + sLink;
			
			sLink = Misc::Format(_T("ToDoList Link (%d): <%s>"), (nLink + 1), sLink);
			AddSpacedContent(sLink, sLinks);
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
								  CMapStringToString& mapPlaces, BOOL bAndSiblings) const
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

void CMLOExporter::ExportPlaces(const ITASKLISTBASE* pSrcTaskFile, CXmlItem* pXIDestProj) const
{
	CMapStringToString mapPlaces;
	BuildPlacesMap(pSrcTaskFile, pSrcTaskFile->GetFirstTask(), mapPlaces, TRUE);
	
	if (mapPlaces.GetCount())
	{
		CXmlItem* pXIResources = pXIDestProj->AddItem(_T("PlacesList"));
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