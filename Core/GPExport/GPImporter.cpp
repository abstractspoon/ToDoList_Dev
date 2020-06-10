// GPImporter.cpp: implementation of the CGPImporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GPExport.h"
#include "GPImporter.h"

#include "..\shared\xmlfileex.h"
#include "..\shared\datehelper.h"
#include "..\shared\enstring.h"
#include "..\shared\localizer.h"
#include "..\shared\WorkingWeek.h"

#include "..\Interfaces\IPreferences.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const UINT ONEDAY = 24 * 60 * 60;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGPImporter::CGPImporter()
{
	m_icon.Load(IDI_GANTTPROJECT);
}

CGPImporter::~CGPImporter()
{
}

void CGPImporter::SetLocalizer(ITransText* pTT)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CLocalizer::Initialize(pTT);
}

LPCTSTR CGPImporter::GetMenuText() const
{
	return GP_MENUTEXT;
}

LPCTSTR CGPImporter::GetFileFilter() const
{
	return GP_FILEFILTER;
}

LPCTSTR CGPImporter::GetFileExtension() const
{
	return GP_FILEEXT;
}

IIMPORTEXPORT_RESULT CGPImporter::Import(LPCTSTR szSrcFilePath, ITaskList* pDestTaskFile, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pDestTaskFile, IID_TASKLISTBASE);

	if (pTasks == NULL)
	{
		ASSERT(0);
		return IIER_BADINTERFACE;
	}

	if (!InitConsts(bSilent, pPrefs, szKey))
		return IIER_CANCELLED;
	
	CXmlFile fileSrc;

	if (!fileSrc.Load(szSrcFilePath, _T("project")))
	{
		switch (fileSrc.GetLastFileError())
		{
		case XFL_BADMSXML:
		case XFL_MISSINGROOT:
			return IIER_BADFORMAT;
		}
		
		// else
		return IIER_BADFILE;
	}

	InitWeekends(fileSrc.Root());

	const CXmlItem* pXISrcTasks = fileSrc.GetItem(_T("tasks"));

	if (!pXISrcTasks) // must exist
		return IIER_BADFORMAT;

	const CXmlItem* pXISrcTask = pXISrcTasks->GetItem(_T("task"));

	if (!pXISrcTask) // must exist
		return IIER_BADFORMAT;

	if (ImportTask(pXISrcTask, pTasks, NULL, TRUE))
	{
		// fix up resource allocations
		FixupResourceAllocations(fileSrc.Root(), pTasks);

		// and dependencies
		FixupDependencies(pXISrcTask, pTasks, TRUE);
	}

	return IIER_SUCCESS; 
}

bool CGPImporter::ImportTask(const CXmlItem* pXISrcTask, ITASKLISTBASE* pDestTaskFile, HTASKITEM htDestParent, BOOL bAndSiblings)
{
	if (!pXISrcTask)
		return true;

	CString sName = pXISrcTask->GetItemValue(_T("name"));
	DWORD dwID = GetTDLTaskID(pXISrcTask->GetItemValueI(_T("id")));
	ASSERT(dwID);

	HTASKITEM hTask = pDestTaskFile->NewTask(sName, htDestParent, dwID);
	ASSERT (hTask);

	if (!hTask)
		return false;

	// completion
	int nPercentDone = pXISrcTask->GetItemValueI(_T("complete"));
	pDestTaskFile->SetTaskPercentDone(hTask, (unsigned char)nPercentDone);

	// dates
	time64_t tStart;
	
	if (CDateHelper::DecodeDate(pXISrcTask->GetItemValue(_T("start")), tStart, FALSE)) // exclude time
	{
		pDestTaskFile->SetTaskStartDate64(hTask, tStart);

		// only add duration to leaf tasks else it'll double up
		if (!pXISrcTask->HasItem(_T("task")))
		{
			int nDuration = pXISrcTask->GetItemValueI(_T("duration"));
			
			if (pXISrcTask->GetItemValue(_T("meeting")) == _T("true"))
			{
				if (!MILESTONETAG.IsEmpty())
					pDestTaskFile->AddTaskTag(hTask, MILESTONETAG);

				pDestTaskFile->SetTaskDueDate64(hTask, tStart);
			}
			else if (nDuration > 0)
			{
				COleDateTime dtEnd = CDateHelper::GetDate(tStart);
				CDateHelper().OffsetDate(dtEnd, (nDuration - 1), DHU_WEEKDAYS); // gp dates are inclusive

				time64_t tEnd = 0;
				VERIFY(CDateHelper::GetTimeT64(dtEnd, tEnd));

				if (nPercentDone == 100)
				{
					pDestTaskFile->SetTaskDoneDate64(hTask, tEnd);
					pDestTaskFile->SetTaskTimeSpent(hTask, nDuration, TDCU_WEEKDAYS);
				}
				else
				{
					pDestTaskFile->SetTaskDueDate64(hTask, tEnd);
					pDestTaskFile->SetTaskTimeEstimate(hTask, nDuration, TDCU_WEEKDAYS);
				}
			}
		}
	}

	// priority
	int nPriority = pXISrcTask->GetItemValueI(_T("priority"));
	pDestTaskFile->SetTaskPriority(hTask, (unsigned char)(nPriority * 3 + 2)); // 2, 5, 8

	// file ref
	CString sFileLink = pXISrcTask->GetItemValue(_T("webLink"));
	sFileLink.TrimLeft();

	if (!sFileLink.IsEmpty())
	{
		// decode file paths
		if (sFileLink.Find(_T("file://")) == 0)
		{
			sFileLink = sFileLink.Mid(7);
			sFileLink.Replace(_T("%20"), _T(""));
		}

		pDestTaskFile->SetTaskFileLinkPath(hTask, sFileLink);
	}

	
	// comments
	pDestTaskFile->SetTaskComments(hTask, pXISrcTask->GetItemValue(_T("notes")));

	// dependency
	// do this after we've imported all the tasks because GP does it 
	// the opposite way round to TDL

	// children
	if (!ImportTask(pXISrcTask->GetItem(_T("task")), pDestTaskFile, hTask, TRUE))
		return false;

	// handle sibling tasks WITHOUT RECURSION
	if (bAndSiblings)
	{
		pXISrcTask = pXISrcTask->GetSibling();
		
		while (pXISrcTask)
		{
			// FALSE == don't recurse on siblings
			if (!ImportTask(pXISrcTask, pDestTaskFile, htDestParent, FALSE))
				return false;
			
			pXISrcTask = pXISrcTask->GetSibling();
		}
	}
	
	return true;
}

DWORD CGPImporter::GetTDLTaskID(int nGPTaskID)
{
	// we deliberately add 1 to the task ID because GP has zero-indexed
	// task IDs and TDL has one-based IDs. This reverses the 
	// process in the exporter that deducted one from each task ID
	return ((DWORD)nGPTaskID + 1);
}

void CGPImporter::FixupResourceAllocations(const CXmlItem* pXISrcPrj, ITASKLISTBASE* pDestTaskFile)
{
	BuildResourceMap(pXISrcPrj);
			
	const CXmlItem* pXIAllocations = pXISrcPrj->GetItem(_T("allocations"));

	if (pXIAllocations && m_mapResources.GetCount())
	{
		const CXmlItem* pXIAlloc = pXIAllocations->GetItem(_T("allocation"));

		while (pXIAlloc)
		{
			DWORD dwTaskID = GetTDLTaskID(pXIAlloc->GetItemValueI(_T("task-id")));
			int nResID = pXIAlloc->GetItemValueI(_T("resource-id"));

			// look up task
			HTASKITEM hTask = pDestTaskFile->FindTask(dwTaskID);

			if (hTask)
			{
				CString sRes;

				if (m_mapResources.Lookup(nResID, sRes) && !sRes.IsEmpty())
					pDestTaskFile->AddTaskAllocatedTo(hTask, sRes);
			}
			
			pXIAlloc = pXIAlloc->GetSibling();
		}
	}
}

void CGPImporter::FixupDependencies(const CXmlItem* pXISrcTask, ITASKLISTBASE* pDestTaskFile, BOOL bAndSiblings)
{
	if (!pXISrcTask)
		return;

	const CXmlItem* pXIDepends = pXISrcTask->GetItem(_T("depend"));

	while (pXIDepends)
	{
		if (pXIDepends->GetItemValueI(_T("type")) == 2)
		{
			// GP records dependencies in reverse to TDL
			// so what appears to be the dependency target is actually the source
			DWORD dwSrcDependID = GetTDLTaskID(pXIDepends->GetItemValueI(_T("id")));
			HTASKITEM hTask = pDestTaskFile->FindTask(dwSrcDependID);

			if (hTask)
			{
				DWORD dwDestDependID = GetTDLTaskID(pXISrcTask->GetItemValueI(_T("id")));
				pDestTaskFile->AddTaskDependency(hTask, dwDestDependID);
			}
		}

		// next dependency
		pXIDepends = pXIDepends->GetSibling();
	}

	// children
	FixupDependencies(pXISrcTask->GetItem(_T("task")), pDestTaskFile, TRUE);

	// handle sibling tasks WITHOUT RECURSION
	if (bAndSiblings)
	{
		pXISrcTask = pXISrcTask->GetSibling();
		
		while (pXISrcTask)
		{
			// FALSE == don't recurse on siblings
			FixupDependencies(pXISrcTask, pDestTaskFile, FALSE);
			pXISrcTask = pXISrcTask->GetSibling();
		}
	}
}

void CGPImporter::InitWeekends(const CXmlItem* pXISrcPrj)
{
	const CXmlItem* pXIDayTypes = pXISrcPrj->GetItem(_T("calendars"), _T("day-types"));
	DWORD dwWeekends = (DHW_SATURDAY | DHW_SUNDAY);

	if (pXIDayTypes)
	{
		const CXmlItem* pXIWeek = pXIDayTypes->GetItem(_T("default-week"));

		if (pXIWeek == NULL)
			pXIWeek = pXIDayTypes->GetItem(_T("calendar"), _T("default-week"));

		if (pXIWeek)
		{
			dwWeekends = 0;

			if (pXIWeek->GetItemValueI(_T("sun")) != 0)
				dwWeekends |= DHW_SUNDAY;

			if (pXIWeek->GetItemValueI(_T("mon")) != 0)
				dwWeekends |= DHW_MONDAY;

			if (pXIWeek->GetItemValueI(_T("tue")) != 0)
				dwWeekends |= DHW_TUESDAY;

			if (pXIWeek->GetItemValueI(_T("wed")) != 0)
				dwWeekends |= DHW_WEDNESDAY;

			if (pXIWeek->GetItemValueI(_T("thu")) != 0)
				dwWeekends |= DHW_THURSDAY;

			if (pXIWeek->GetItemValueI(_T("fri")) != 0)
				dwWeekends |= DHW_FRIDAY;

			if (pXIWeek->GetItemValueI(_T("sat")) != 0)
				dwWeekends |= DHW_SATURDAY;
		}
	}

	CWeekend::Initialise(dwWeekends);
}

void CGPImporter::BuildResourceMap(const CXmlItem* pXISrcPrj)
{
	m_mapResources.RemoveAll();

	const CXmlItem* pXIResources = pXISrcPrj->GetItem(_T("resources"));

	if (pXIResources)
	{
		const CXmlItem* pXIRes = pXIResources->GetItem(_T("resource"));

		while (pXIRes)
		{
			m_mapResources[pXIRes->GetItemValueI(_T("id"))] = pXIRes->GetItemValue(_T("name"));
			pXIRes = pXIRes->GetSibling();
		}
	}
}

bool CGPImporter::InitConsts(bool /*bSilent*/, const IPreferences* pPrefs, LPCTSTR /*szKey*/)
{
	CString sPrefKey;
	sPrefKey.Format(_T("FileStates\\Introduction.tdl\\UIExtensions\\%s"), GANTTVIEW_ID);

	MILESTONETAG = pPrefs->GetProfileString(sPrefKey, _T("MileStoneTag"), CEnString(_T("MileStone")));

	return true;
}
