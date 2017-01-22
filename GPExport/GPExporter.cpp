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

#include "..\interfaces\ipreferences.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const UINT ONEDAY = 24 * 60 * 60;
const LPCTSTR UTF8_HEADER = _T("version=\"1.0\" encoding=\"UTF-8\"");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGPExporter::CGPExporter() : m_hIcon(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_GANTTPROJECT);
}

CGPExporter::~CGPExporter() 
{
	::DestroyIcon(m_hIcon);
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

bool CGPExporter::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	const ITaskList9* pITL9 = GetITLInterface<ITaskList9>(pSrcTaskFile, IID_TASKLIST9);
	ASSERT (pITL9);

	if (!InitConsts(pITL9, bSilent, pPrefs, szKey))
		return false;

	CXmlFile fileDest(_T("project"));
	fileDest.SetXmlHeader(UTF8_HEADER);

	// export resource allocations
	ExportResources(pITL9, fileDest.Root());

	// clear the task map that will be populated in ExportTask
	m_mapTasks.RemoveAll();

	// export tasks
	CXmlItem* pXITasks = fileDest.AddItem(_T("tasks"));
	CXmlItem* pXIAllocations = fileDest.AddItem(_T("allocations"));

	if (!ExportTask(pITL9, pSrcTaskFile->GetFirstTask(), pXITasks, pXIAllocations, TRUE))
		return false;

	ExportDependencies(pITL9, pITL9->GetFirstTask(), pXITasks, TRUE);

	// important display stuff for GP
	SetupDisplay(fileDest.Root());
	SetupCalendar(fileDest.Root());

	// save result
	return (fileDest.Save(szDestFilePath, SFEF_UTF8WITHOUTBOM) != FALSE);
}

bool CGPExporter::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CXmlFile fileDest(_T("project"));
	fileDest.SetXmlHeader(UTF8_HEADER);

	// placeholders for tasks
	CXmlItem* pXITasks = fileDest.AddItem(_T("tasks"));
	CXmlItem* pXIAllocations = fileDest.AddItem(_T("allocations"));
	
	for (int nTaskList = 0; nTaskList < pSrcTaskFile->GetTaskListCount(); nTaskList++)
	{
		const ITaskList9* pITL9 = GetITLInterface<ITaskList9>(pSrcTaskFile->GetTaskList(nTaskList), IID_TASKLIST9);

		if ((nTaskList == 0) && !InitConsts(pITL9, bSilent, pPrefs, szKey))
			return false;
			
		if (pITL9)
		{
			// export resource allocations
			ExportResources(pITL9, fileDest.Root());
			
			// clear the task map that will be populated in ExportTask
			m_mapTasks.RemoveAll();

			// export tasks
			if (!ExportTask(pITL9, pITL9->GetFirstTask(), pXITasks, pXIAllocations, TRUE))
				return false;

			ExportDependencies(pITL9, pITL9->GetFirstTask(), pXITasks, TRUE);
		}
	}

	// important display stuff for GP
	SetupDisplay(fileDest.Root());
	SetupCalendar(fileDest.Root());

	// save result
	return (fileDest.Save(szDestFilePath, SFEF_UTF8WITHOUTBOM) != FALSE);
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


bool CGPExporter::ExportTask(const ITaskList9* pSrcTaskFile, HTASKITEM hTask, 
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
		CString sColor = pSrcTaskFile->GetTaskAttribute(hTask, _T("TEXTWEBCOLOR"));
		ASSERT(!sColor.IsEmpty() && sColor[0] == '#');

		pXIDestItem->AddItem(_T("color"), sColor);
	}

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

	// dates
	time_t tStart = 0, tDue = 0, tDone = 0;
	GetTaskDates(pSrcTaskFile, hTask, tStart, tDue, tDone);

	if (tStart && (tDue || tDone))
	{
		COleDateTime start(tStart);
		pXIDestItem->AddItem(_T("start"), CDateHelper::FormatDate(start, DHFD_ISO));

		// Duration
		if (bMilestone)
		{
			pXIDestItem->AddItem(_T("duration"), 0);
		}
		else
		{
			if (tDone >= tStart) // completion date takes precedence
			{
				int nDays = CDateHelper::CalcDaysFromTo(start, tDone, TRUE, TRUE);
				pXIDestItem->AddItem(_T("duration"), nDays);
			}
			else if (tDue >= tStart)
			{
				int nDays = CDateHelper::CalcDaysFromTo(start, tDue, TRUE, TRUE);
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
	CString sFileRef = pSrcTaskFile->GetTaskFileLinkPath(hTask);
	sFileRef.TrimLeft();

	if (!sFileRef.IsEmpty())
	{
		// web or file link?
		if (sFileRef.Find(_T(":\\")) == 1 || sFileRef.Find(_T("\\\\")) == 0)
			sFileRef = _T("file://") + sFileRef;

		sFileRef.Replace(_T(" "), _T("%20"));

		pXIDestItem->AddItem(_T("webLink"), sFileRef);
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

void CGPExporter::BuildResourceMap(const ITaskList9* pSrcTaskFile, HTASKITEM hTask, CXmlItem* pDestPrj, BOOL bAndSiblings)
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

void CGPExporter::ExportResources(const ITaskList9* pSrcTaskFile, CXmlItem* pDestPrj)
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

void CGPExporter::ExportDependencies(const ITaskList9* pSrcTaskFile, HTASKITEM hTask, CXmlItem* pDestPrj, BOOL bAndSiblings)
{
	if (!hTask)
		return;

	// Dependency of 'B' on 'A' is stored in 'A's definition
	// GP does dependencies the opposite to TDL, specifically GP records the link
	// of 'B's dependence on 'A', in 'A's definition.
	int nNumDepends = pSrcTaskFile->GetTaskDependencyCount(hTask);

	for (int nDepend = 0; nDepend < nNumDepends; nDepend++)
	{
		CString sDepends(pSrcTaskFile->GetTaskDependency(hTask, nDepend));

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
					pXIDepends->AddItem(_T("type"), _T("2"));
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

void CGPExporter::GetTaskDates(const ITaskList9* pSrcTaskFile, HTASKITEM hTask, time_t& tEarliestStart, time_t& tLatestDue, time_t& tLatestDone)
{
	tEarliestStart = INT_MAX;
	tLatestDue = tLatestDone = 0;

	time_t tTaskStart = pSrcTaskFile->GetTaskStartDate(hTask);
	time_t tTaskDue = pSrcTaskFile->GetTaskDueDate(hTask, FALSE);
	time_t tTaskDone = pSrcTaskFile->GetTaskDoneDate(hTask);

	// if we are _not_ a parent make up what we don't have
	HTASKITEM hTaskChild = pSrcTaskFile->GetFirstTask(hTask);

	if (hTaskChild == NULL)
	{
		if (!tTaskStart)
		{
			if (tTaskDue)
				tTaskStart = tTaskDue;
			else if (tTaskDone)
				tTaskStart = tTaskDone;

			// else there are no dates so leave as-is
		}

		// if no due date then make something up
		if (tTaskStart && !tTaskDue && !tTaskDone)
			tTaskDue = tTaskStart;
	}

	if (tTaskStart)
		tEarliestStart = tTaskStart;

	if (tTaskDue)
		tLatestDue = tTaskDue;

	if (tTaskDone)
		tLatestDone = tTaskDone;

	// check children
	while (hTaskChild)
	{
		GetTaskDates(pSrcTaskFile, hTaskChild, tTaskStart, tTaskDue, tTaskDone); // RECURSIVE call

		if (tTaskStart)
			tEarliestStart = min(tTaskStart, tEarliestStart);

		if (tTaskDue)
			tLatestDue = max(tTaskDue, tLatestDue);

		if (tTaskDone)
			tLatestDone = max(tTaskDone, tLatestDone);

		// next
		hTaskChild = pSrcTaskFile->GetNextTask(hTaskChild);
	}
}

bool CGPExporter::InitConsts(const ITaskList9* pTaskFile, bool /*bSilent*/, const IPreferences* pPrefs, LPCTSTR /*szKey*/)
{
	// Hack to get Gantt View's tag for milestones
	CString sFileName = pTaskFile->GetAttribute(_T("FILENAME"));

	CString sPrefKey;
	sPrefKey.Format(_T("FileStates\\%s\\UIExtensions\\%s"), sFileName, GANTTVIEW_ID);

	if (pPrefs->GetProfileInt(sPrefKey, _T("UseTagForMilestone")))
		MILESTONETAG = pPrefs->GetProfileString(sPrefKey, _T("MileStoneTag"));

	return true;
}
