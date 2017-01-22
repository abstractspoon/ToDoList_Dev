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

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMLOExporter::CMLOExporter() : m_hIcon(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MYLIFEORGANISED);
}

CMLOExporter::~CMLOExporter() 
{
	::DestroyIcon(m_hIcon);
}

void CMLOExporter::SetLocalizer(ITransText* /*pTT*/)
{
	//CLocalizer::Initialize(pTT);
}

bool CMLOExporter::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool /*bSilent*/, IPreferences* /*pPrefs*/, LPCTSTR /*szKey*/)
{
	CXmlFile fileDest(_T("MyLifeOrganized-xml"));
	
	const ITaskList7* pITL7 = GetITLInterface<ITaskList7>(pSrcTaskFile, IID_TASKLIST7);
	ASSERT (pITL7);
	
	// export tasks
	CXmlItem* pXITasks = fileDest.AddItem(_T("TaskTree"));
	
	if (!ExportTask(pITL7, pSrcTaskFile->GetFirstTask(), pXITasks, TRUE))
		return false;
	
	// export resource allocations
	ExportPlaces(pITL7, fileDest.Root());
	
	// save result
	return (fileDest.Save(szDestFilePath, SFEF_UTF8WITHOUTBOM) != FALSE);
}

bool CMLOExporter::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool /*bSilent*/, IPreferences* /*pPrefs*/, LPCTSTR /*szKey*/)
{
	CXmlFile fileDest(_T("MyLifeOrganized-xml"));
	
	// export tasks
	CXmlItem* pXITasks = fileDest.AddItem(_T("TaskTree"));
	
	for (int nTaskList = 0; nTaskList < pSrcTaskFile->GetTaskListCount(); nTaskList++)
	{
		const ITaskList7* pITL7 = GetITLInterface<ITaskList7>(pSrcTaskFile->GetTaskList(nTaskList), IID_TASKLIST7);
		
		if (pITL7)
		{
			// export tasks
			if (!ExportTask(pITL7, pITL7->GetFirstTask(), pXITasks, TRUE))
				return false;
			
			// export resource allocations
			ExportPlaces(pITL7, fileDest.Root());
		}
	}
	
	// save result
	return (fileDest.Save(szDestFilePath, SFEF_UTF8WITHOUTBOM) != FALSE);
}

bool CMLOExporter::ExportTask(const ITaskList7* pSrcTaskFile, HTASKITEM hTask, 
							  CXmlItem* pXIDestParent, BOOL bAndSiblings)
{
	if (!hTask)
		return true;
	
	// create a new item corresponding to pXITask at the dest
	CXmlItem* pXIDestItem = pXIDestParent->AddItem(_T("TaskNode"));
	
	if (!pXIDestItem)
		return false;
	
	// copy across the appropriate attributes
	pXIDestItem->AddItem(_T("Caption"), pSrcTaskFile->GetTaskTitle(hTask));
	
	// priority
	int nPriority = pSrcTaskFile->GetTaskPriority(hTask, FALSE);
	int nImportance = max(nPriority, 0) * 10;
	
	pXIDestItem->AddItem(_T("Importance"), nImportance);
	
	// dates
	time_t tDue = pSrcTaskFile->GetTaskDueDate(hTask, FALSE);
	time_t tDone = pSrcTaskFile->GetTaskDoneDate(hTask);
	
	if (tDone)
		pXIDestItem->AddItem(_T("CompletionDateTime"), CDateHelper::FormatDate(tDone, DHFD_ISO));
	
	if (tDue)
		pXIDestItem->AddItem(_T("DueDateTime"), CDateHelper::FormatDate(tDue, DHFD_ISO));
	
	// time estimate
	TDC_UNITS nUnits;
	double dTimeEst = pSrcTaskFile->GetTaskTimeEstimate(hTask, nUnits, FALSE);
	
	if (dTimeEst > 0.0)
	{
		TH_UNITS nTHUnits = MapUnitsToTHUnits(nUnits);
		pXIDestItem->AddItem(_T("EstimateMax"), CTimeHelper().GetTime(dTimeEst, nTHUnits, THU_DAYS));
	}
	
	// comments
	LPCTSTR szComments = pSrcTaskFile->GetTaskComments(hTask);
	
	if (!Misc::IsEmpty(szComments))
		pXIDestItem->AddItem(_T("Note"), szComments);
	
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

void CMLOExporter::BuildPlacesMap(const ITaskList7* pSrcTaskFile, HTASKITEM hTask, 
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

void CMLOExporter::ExportPlaces(const ITaskList7* pSrcTaskFile, CXmlItem* pDestPrj)
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
