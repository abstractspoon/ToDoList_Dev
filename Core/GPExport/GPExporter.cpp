// GPExporter.cpp: implementation of the CGPExporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GPExport.h"
#include "GPExporter.h"

#include "..\shared\xmlfileex.h"
#include "..\shared\misc.h"
#include "..\shared\datehelper.h"
#include "..\shared\localizer.h"
#include "..\shared\enstring.h"

#include "..\3rdParty\T64Utils.h"

#include "..\interfaces\ipreferences.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const UINT ONEDAY = 24 * 60 * 60;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGPExporter::CGPExporter()
{
	m_icon.Load(IDI_GANTTPROJECT);
}

CGPExporter::~CGPExporter() 
{
}

void CGPExporter::SetLocalizer(ITransText* pTT)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CLocalizer::Initialize(pTT);
}

LPCTSTR CGPExporter::GetMenuText() const
{
	return GP_MENUTEXT;
}

LPCTSTR CGPExporter::GetFileFilter() const
{
	return GP_FILEFILTER;
}

LPCTSTR CGPExporter::GetFileExtension() const
{
	return GP_FILEEXT;
}

IIMPORTEXPORT_RESULT CGPExporter::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pSrcTaskFile, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return IIER_BADINTERFACE;
	}

	if (!InitConsts(pTasks, dwFlags, pPrefs, szKey))
		return IIER_CANCELLED;

	CXmlFile fileDest(_T("project"));
	fileDest.SetXmlHeader(DEFAULT_UTF8_HEADER);

	// export resource allocations
	ExportResources(pTasks, fileDest.Root());

	// clear the task map that will be populated in ExportTask
	m_mapTasks.RemoveAll();

	// export tasks
	CXmlItem* pXITasks = fileDest.AddItem(_T("tasks"));
	CXmlItem* pXIAllocations = fileDest.AddItem(_T("allocations"));

	if (!ExportTask(pTasks, pSrcTaskFile->GetFirstTask(), pXITasks, pXIAllocations, TRUE))
		return IIER_SOMEFAILED;

	ExportDependencies(pTasks, pTasks->GetFirstTask(), pXITasks, TRUE);

	// important display stuff for GP
	SetupDisplay(fileDest.Root());
	SetupCalendar(fileDest.Root());

	// save result
	if (!fileDest.Save(szDestFilePath, SFEF_UTF8WITHOUTBOM))
		return IIER_BADFILE;

	return IIER_SUCCESS;
}

IIMPORTEXPORT_RESULT CGPExporter::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	const ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pSrcTaskFile->GetTaskList(0), IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return IIER_BADINTERFACE;
	}

	if (!InitConsts(pTasks, dwFlags, pPrefs, szKey))
		return IIER_CANCELLED;

	CXmlFile fileDest(_T("project"));
	fileDest.SetXmlHeader(DEFAULT_UTF8_HEADER);

	// placeholders for tasks
	CXmlItem* pXITasks = fileDest.AddItem(_T("tasks"));
	CXmlItem* pXIAllocations = fileDest.AddItem(_T("allocations"));
	
	for (int nTaskList = 0; nTaskList < pSrcTaskFile->GetTaskListCount(); nTaskList++)
	{
		pTasks = GetITLInterface<ITASKLISTBASE>(pSrcTaskFile->GetTaskList(nTaskList), IID_TASKLISTBASE);

		if (pTasks)
		{
			// export resource allocations
			ExportResources(pTasks, fileDest.Root());
			
			// clear the task map that will be populated in ExportTask
			m_mapTasks.RemoveAll();

			// export tasks
			if (!ExportTask(pTasks, pTasks->GetFirstTask(), pXITasks, pXIAllocations, TRUE))
				return IIER_SOMEFAILED;

			ExportDependencies(pTasks, pTasks->GetFirstTask(), pXITasks, TRUE);
		}
	}

	// important display stuff for GP
	SetupDisplay(fileDest.Root());
	SetupCalendar(fileDest.Root());

	// save result
	if (!fileDest.Save(szDestFilePath, SFEF_UTF8WITHOUTBOM))
		return IIER_BADFILE;

	return IIER_SUCCESS;
}

void CGPExporter::SetupDisplay(CXmlItem* pDestPrj)
{
	CXmlItem* pXIDisplay = pDestPrj->AddItem(_T("taskdisplaycolumns"));

	CXmlItem* pXIColumn = pXIDisplay->AddItem(_T("displaycolumn"));
	pXIColumn->SetItemValue(_T("property-id"), _T("tpd3"));
	pXIColumn->SetItemValue(_T("order"), _T("0"));
	pXIColumn->SetItemValue(_T("width"), _T("75"));

	pXIColumn = pXIDisplay->AddItem(_T("displaycolumn"));
	pXIColumn->SetItemValue(_T("property-id"), _T("tpd4"));
	pXIColumn->SetItemValue(_T("order"), _T("1"));
	pXIColumn->SetItemValue(_T("width"), _T("75"));

	pXIColumn = pXIDisplay->AddItem(_T("displaycolumn"));
	pXIColumn->SetItemValue(_T("property-id"), _T("tpd5"));
	pXIColumn->SetItemValue(_T("order"), _T("2"));
	pXIColumn->SetItemValue(_T("width"), _T("75"));
}

void CGPExporter::SetupCalendar(CXmlItem* pDestPrj)
{
	/*
	<calendars>
	<day-types>
	<day-type id="0"/> 
	<day-type id="1"/> 
	<calendar id="1" name="default">
	<default-week sun="1" mon="0" tue="0" wed="0"
	thu="0" fri="0" sat="1"/>
	</calendar>
	</day-types>
	</calendars>
	*/
	
	CXmlItem* pXICals = pDestPrj->AddItem(_T("calendars"));
	CXmlItem* pXIDayTypes = pXICals->AddItem(_T("day-types"));
	
	CXmlItem* pXIDay = pXICals->AddItem(_T("day-type"));
	pXIDay->SetItemValue(_T("id"), _T("0")); // weekday
	
	pXIDay = pXICals->AddItem(_T("day-type"));
	pXIDay->SetItemValue(_T("id"), _T("1")); // weekend
	
	CXmlItem* pXICal = pXIDayTypes->AddItem(_T("calendar"));
	
	pXICal->SetItemValue(_T("id"), _T("1"));
	pXICal->SetItemValue(_T("name"), _T("default"));
	
	CXmlItem* pXIWeek = pXICal->AddItem(_T("default-week"));
	
	pXIWeek->SetItemValue(_T("sun"), _T("1"));
	pXIWeek->SetItemValue(_T("mon"), _T("0"));
	pXIWeek->SetItemValue(_T("tue"), _T("0"));
	pXIWeek->SetItemValue(_T("wed"), _T("0"));
	pXIWeek->SetItemValue(_T("thu"), _T("0"));
	pXIWeek->SetItemValue(_T("fri"), _T("0"));
	pXIWeek->SetItemValue(_T("sat"), _T("1"));
}


bool CGPExporter::ExportTask(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask, 
							 CXmlItem* pXIDestParent, CXmlItem* pXIAllocations, BOOL bAndSiblings)
{
	if (!hTask)
		return true;

	// create a new item corresponding to pXITask at the dest
	CXmlItem* pXIDestItem = pXIDestParent->AddItem(_T("task"));

	if (!pXIDestItem)
		return false;

	// copy across the appropriate attributes
	int nTaskID = GetGPTaskID(pSrcTaskFile->GetTaskID(hTask));
	pXIDestItem->AddItem(_T("id"), nTaskID);
	pXIDestItem->AddItem(_T("name"), pSrcTaskFile->GetTaskTitle(hTask));

	// map the id to item for dependency
	m_mapTasks[nTaskID] = pXIDestItem;

	// colour
	if (pSrcTaskFile->GetTaskTextColor(hTask) > 0)
	{
		CString sColor = ((ITaskList*)pSrcTaskFile)->GetTaskAttribute(hTask, _T("TEXTWEBCOLOR"));
		ASSERT(!sColor.IsEmpty() && sColor[0] == '#');

		pXIDestItem->AddItem(_T("color"), sColor);
	}

	// Dates
	time64_t tStart = 0, tDue = 0, tDone = 0;
	GetTaskDates(pSrcTaskFile, hTask, tStart, tDue, tDone);
	
	if (tStart != T64Utils::T64_NULL)
	{
		// Milestone
		BOOL bMilestone = FALSE;

		if (!MILESTONETAG.IsEmpty())
		{
			int nTag = pSrcTaskFile->GetTaskTagCount(hTask);

			while (nTag--)
			{
				if (MILESTONETAG.CompareNoCase(pSrcTaskFile->GetTaskTag(hTask, nTag)) == 0)
				{
					pXIDestItem->AddItem(_T("meeting"), _T("true"));
					bMilestone = TRUE;
					break;
				}
			}
		}

		COleDateTime dtStart(CDateHelper::GetDate(tStart));
		pXIDestItem->AddItem(_T("start"), CDateHelper::FormatDate(dtStart, DHFD_ISO));

		// Duration
		if (bMilestone)
		{
			pXIDestItem->AddItem(_T("duration"), 0);
		}
		else if (tDue || tDone)
		{
			if (tDone >= tStart) // completion date takes precedence
			{
				COleDateTime dtDone = CDateHelper::GetDate(tDone);
				int nDays = CDateHelper().CalcDaysFromTo(dtStart, dtDone, TRUE);

				pXIDestItem->AddItem(_T("duration"), nDays);
			}
			else if (tDue >= tStart)
			{
				COleDateTime dtDue = CDateHelper::GetDate(tDue);
				int nDays = CDateHelper().CalcDaysFromTo(dtStart, dtDue, TRUE);

				pXIDestItem->AddItem(_T("duration"), nDays);
			}
		}
	}

	// completion
	if (tDone)
	{
		pXIDestItem->AddItem(_T("complete"), _T("100"));
		pXIDestItem->AddItem(_T("priority"), _T("-1"));
	}
	else
	{
		pXIDestItem->AddItem(_T("complete"), pSrcTaskFile->GetTaskPercentDone(hTask, TRUE));

		int nPriority = pSrcTaskFile->GetTaskPriority(hTask, TRUE);
		
		if (nPriority < 0)
			pXIDestItem->AddItem(_T("priority"), _T("-1"));
		else if (nPriority <= 3)
			pXIDestItem->AddItem(_T("priority"), 0); // low
		else if (nPriority <= 6)
			pXIDestItem->AddItem(_T("priority"), 1); // medium
		else
			pXIDestItem->AddItem(_T("priority"), 2); // high
	}
	
	// dependencies done afterwards

	// file/weblink
	CString sFileLink = pSrcTaskFile->GetTaskFileLinkPath(hTask);
	sFileLink.TrimLeft();

	if (!sFileLink.IsEmpty())
	{
		// web or file link?
		if (sFileLink.Find(_T(":\\")) == 1 || sFileLink.Find(_T("\\\\")) == 0)
			sFileLink = _T("file://") + sFileLink;

		sFileLink.Replace(_T(" "), _T("%20"));

		pXIDestItem->AddItem(_T("webLink"), sFileLink);
	}

	// comments
	LPCTSTR szComments = pSrcTaskFile->GetTaskComments(hTask);

	if (!Misc::IsEmpty(szComments))
		pXIDestItem->AddItem(_T("notes"), szComments, XIT_CDATA);

	// resource allocation
	int nAllocTo = pSrcTaskFile->GetTaskAllocatedToCount(hTask);

	while (nAllocTo--)
	{
		CString sAllocTo = pSrcTaskFile->GetTaskAllocatedTo(hTask, nAllocTo);
		int nResID;

		if (m_mapResources.Lookup(sAllocTo, nResID))
		{
			CXmlItem* pXIAllocTo = pXIAllocations->AddItem(_T("allocation"));

			if (pXIAllocTo)
			{
				pXIAllocTo->AddItem(_T("task-id"), nTaskID);
				pXIAllocTo->AddItem(_T("resource-id"), nResID);
				pXIAllocTo->AddItem(_T("responsible"), _T("true"));
				pXIAllocTo->AddItem(_T("load"), _T("100.0"));
				pXIAllocTo->AddItem(_T("function"), _T("Default:0"));
			}
		}
	}
	
	// copy across first child
	if (!ExportTask(pSrcTaskFile, pSrcTaskFile->GetFirstTask(hTask), pXIDestItem, pXIAllocations, TRUE))
		return false;

	// handle sibling tasks WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pSrcTaskFile->GetNextTask(hTask);

		while (hSibling)
		{
			// FALSE == don't recurse on siblings
			if (!ExportTask(pSrcTaskFile, hSibling, pXIDestParent, pXIAllocations, FALSE))
				return false;

			hSibling = pSrcTaskFile->GetNextTask(hSibling);
		}
	}

	return true;
}

void CGPExporter::BuildResourceMap(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask, CXmlItem* pDestPrj, BOOL bAndSiblings)
{
	if (!hTask)
		return;

	int nAllocTo = pSrcTaskFile->GetTaskAllocatedToCount(hTask);
	
	while (nAllocTo--)
	{
		CString sAllocTo = pSrcTaskFile->GetTaskAllocatedTo(hTask, nAllocTo);
		int nResID = 0;
		
		if (!m_mapResources.Lookup(sAllocTo, nResID))
		{
			nResID = m_mapResources.GetCount();
			m_mapResources[sAllocTo] = nResID;
		}
	}

	// children
	BuildResourceMap(pSrcTaskFile, pSrcTaskFile->GetFirstTask(hTask), pDestPrj, TRUE);

	// handle sibling tasks WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pSrcTaskFile->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == don't recurse on siblings
			BuildResourceMap(pSrcTaskFile, hSibling, pDestPrj, FALSE);
			hSibling = pSrcTaskFile->GetNextTask(hSibling);
		}
	}
}

void CGPExporter::ExportResources(const ITASKLISTBASE* pSrcTaskFile, CXmlItem* pDestPrj)
{
	BuildResourceMap(pSrcTaskFile, pSrcTaskFile->GetFirstTask(), pDestPrj, TRUE);

	if (m_mapResources.GetCount())
	{
		CXmlItem* pXIResources = pDestPrj->AddItem(_T("resources"));
		CString sRes;
		int nResID;

		POSITION pos = m_mapResources.GetStartPosition();

		while (pos)
		{
			m_mapResources.GetNextAssoc(pos, sRes, nResID);
			
			CXmlItem* pXIRes = pXIResources->AddItem(_T("resource"));

			if (pXIRes)
			{
				pXIRes->AddItem(_T("name"), sRes);
				pXIRes->AddItem(_T("id"), nResID);
				pXIRes->AddItem(_T("function"), _T("Default:0")); // crucial
			}
		}

		// set up view characteristics
		CXmlItem* pXIResView = pDestPrj->AddItem(_T("view"));
		pXIResView->AddItem(_T("id"), _T("resource-table"));
		
		CXmlItem* pXIResField = pXIResView->AddItem(_T("field"));
		pXIResField->AddItem(_T("id"), 0);
		pXIResField->AddItem(_T("name"), _T("Name"));
		pXIResField->AddItem(_T("width"), 60);
		pXIResField->AddItem(_T("order"), 0);
		
		pXIResField = pXIResView->AddItem(_T("field"));
		pXIResField->AddItem(_T("id"), 1);
		pXIResField->AddItem(_T("name"), _T("Default role"));
		pXIResField->AddItem(_T("width"), 30);
		pXIResField->AddItem(_T("order"), 10);	
	}
}

void CGPExporter::ExportDependencies(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask, CXmlItem* pDestPrj, BOOL bAndSiblings)
{
	if (!hTask)
		return;

	// Dependency of 'B' on 'A' is stored in 'A's definition
	// GP does dependencies the opposite to TDL, specifically GP records the link
	// of 'B's dependence on 'A', in 'A's definition.
	int nNumDepends = pSrcTaskFile->GetTaskDependencyCount(hTask);

	for (int nDepend = 0; nDepend < nNumDepends; nDepend++)
	{
		int nDaysLeadIn = 0;
		CString sDepends(pSrcTaskFile->GetTaskDependency(hTask, nDepend, &nDaysLeadIn));

		if (!sDepends.IsEmpty())
		{
			int nDelim = sDepends.Find('?');

			if (nDelim == -1) // must be a dependency in the same tasklist
			{
				// find the GP task having this ID
				// note: this task may not exist if pSrcTaskFile points to a partial list
				CXmlItem* pXIDependTask = NULL;
				int nDependID = GetGPTaskID(_ttoi(sDepends));

				m_mapTasks.Lookup(nDependID, pXIDependTask);

				if (pXIDependTask)
				{
					CXmlItem* pXIDepends = pXIDependTask->AddItem(_T("depend"));
					ASSERT(pXIDepends);
					
					int nDependeeID = GetGPTaskID(pSrcTaskFile->GetTaskID(hTask));
					
					pXIDepends->AddItem(_T("id"), nDependeeID);
					pXIDepends->AddItem(_T("type"), 2);

					if (nDaysLeadIn != 0)
						pXIDepends->AddItem(_T("difference"), nDaysLeadIn);

				}
			}
		}
	}

	// children
	ExportDependencies(pSrcTaskFile, pSrcTaskFile->GetFirstTask(hTask), pDestPrj, TRUE);

	// handle sibling tasks WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pSrcTaskFile->GetNextTask(hTask);
		
		while (hSibling)
		{
			// FALSE == don't recurse on siblings
			ExportDependencies(pSrcTaskFile, hSibling, pDestPrj, FALSE);
			hSibling = pSrcTaskFile->GetNextTask(hSibling);
		}
	}
}

int CGPExporter::GetGPTaskID(DWORD dwTDLTaskID)
{
	// we deliberately deduct 1 from the task ID because GP has zero-indexed
	// task IDs and TDL has one-based IDs. Then in the importer we will
	// add one to each task ID to reverse the process.
	return ((int)dwTDLTaskID - 1);
}

void CGPExporter::GetTaskDates(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask, time64_t& tEarliestStart, time64_t& tLatestDue, time64_t& tLatestDone)
{
	tEarliestStart = INT_MAX;
	tLatestDue = tLatestDone = 0;

	time64_t tTaskStart = T64Utils::T64_NULL, tTaskDue = T64Utils::T64_NULL, tTaskDone = T64Utils::T64_NULL;
	
	BOOL bHasStart = pSrcTaskFile->GetTaskStartDate64(hTask, false, tTaskStart);
	BOOL bHasDue = pSrcTaskFile->GetTaskDueDate64(hTask, false, tTaskDue);
	BOOL bHasDone = pSrcTaskFile->GetTaskDoneDate64(hTask, tTaskDone);

	// if we are _not_ a parent make up what we don't have
	HTASKITEM hTaskChild = pSrcTaskFile->GetFirstTask(hTask);

	if (hTaskChild == NULL)
	{
		if (!bHasStart)
		{
			if (bHasDue)
			{
				tTaskStart = tTaskDue;
			}
			else if (bHasDone)
			{
				tTaskStart = tTaskDone;
			}
			// else there are no dates so leave as-is
		}
		else if (bHasStart && !bHasDue && !bHasDone)
		{
			// if no due date then make something up
			tTaskDue = tTaskStart;
		}
	}

	if (tTaskStart != T64Utils::T64_NULL)
		tEarliestStart = tTaskStart;

	if (tTaskDue != T64Utils::T64_NULL)
		tLatestDue = tTaskDue;

	if (tTaskDone != T64Utils::T64_NULL)
		tLatestDone = tTaskDone;

	// check children
	while (hTaskChild)
	{
		GetTaskDates(pSrcTaskFile, hTaskChild, tTaskStart, tTaskDue, tTaskDone); // RECURSIVE call

		if (tTaskStart != T64Utils::T64_NULL)
			tEarliestStart = min(tTaskStart, tEarliestStart);

		if (tTaskDue != T64Utils::T64_NULL)
			tLatestDue = max(tTaskDue, tLatestDue);

		if (tTaskDone != T64Utils::T64_NULL)
			tLatestDone = max(tTaskDone, tLatestDone);

		// next
		hTaskChild = pSrcTaskFile->GetNextTask(hTaskChild);
	}
}

bool CGPExporter::InitConsts(const ITASKLISTBASE* pTaskFile, DWORD /*dwFlags*/, const IPreferences* pPrefs, LPCTSTR /*szKey*/)
{
	MILESTONETAG = CEnString(_T("MileStone")); // default

	// Hack to get Gantt View's tag for milestones
	CString sFileName = pTaskFile->GetAttribute(_T("FILENAME"));

	if (!sFileName.IsEmpty())
	{
		CString sPrefKey;
		sPrefKey.Format(_T("FileStates\\%s\\UIExtensions\\%s"), sFileName, GANTTVIEW_ID);
		
		if (pPrefs->GetProfileInt(sPrefKey, _T("UseTagForMilestone")))
			MILESTONETAG = pPrefs->GetProfileString(sPrefKey, _T("MileStoneTag"), MILESTONETAG);
	}

	return true;
}
