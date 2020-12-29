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

bool CMLOExporter::ExportTask(const ITASKLISTBASE* pSrcTaskFile, HTASKITEM hTask, 
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
	time64_t tDate = T64Utils::T64_NULL;
	
	if (pSrcTaskFile->GetTaskCreationDate64(hTask, tDate))
		pXIDestItem->AddItem(_T("Created"), FormatDate(tDate));
	
	if (pSrcTaskFile->GetTaskDoneDate64(hTask, tDate))
		pXIDestItem->AddItem(_T("CompletionDateTime"), FormatDate(tDate));

	if (pSrcTaskFile->GetTaskDueDate64(hTask, false, tDate))
		pXIDestItem->AddItem(_T("DueDateTime"), FormatDate(tDate));
	
	// time estimate
	TDC_UNITS nUnits;
	double dTimeEst = pSrcTaskFile->GetTaskTimeEstimate(hTask, nUnits, FALSE);
	
	if (dTimeEst > 0.0)
	{
		TH_UNITS nTHUnits = MapUnitsToTHUnits(nUnits);
		pXIDestItem->AddItem(_T("EstimateMax"), CTimeHelper().Convert(dTimeEst, nTHUnits, THU_DAYS));
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
