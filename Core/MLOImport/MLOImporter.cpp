// MLOImporter.cpp: implementation of the CMLOImporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MLOImporter.h"
#include "resource.h"

#include <time.h>
#include <unknwn.h>

#include "..\SHARED\xmlfile.h"
#include "..\SHARED\datehelper.h"

#include "..\Interfaces\iTasklist.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMLOImporter::CMLOImporter()
{
	m_icon.Load(IDI_MYLIFEORGANISED);
}

CMLOImporter::~CMLOImporter()
{
}

void CMLOImporter::SetLocalizer(ITransText* /*pTT*/)
{
	//CLocalizer::Initialize(pTT);
}

IIMPORTEXPORT_RESULT CMLOImporter::Import(LPCTSTR szSrcFilePath, ITaskList* pDestTaskFile, DWORD /*dwFlags*/, IPreferences* /*pPrefs*/, LPCTSTR /*szKey*/)
{
	ITASKLISTBASE* pTasks = GetITLInterface<ITASKLISTBASE>(pDestTaskFile, IID_TASKLISTBASE);
	
	if (pTasks == NULL)
	{
		ASSERT(0);
		return IIER_BADINTERFACE;
	}
	
	CXmlFile file;

	if (!file.Load(szSrcFilePath, _T("MyLifeOrganized-xml")))
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

	const CXmlItem* pXIMLOTree = file.GetItem(_T("TaskTree"));

	if (!pXIMLOTree)
		return IIER_BADFORMAT;

	const CXmlItem* pXIMLOTask = pXIMLOTree->GetItem(_T("TaskNode"));

	if (!pXIMLOTask)
		return IIER_BADFORMAT;

	// this first node always seems to be untitled 
	// so we get the first subnode
	// If that is not present it's just an empty file
	pXIMLOTask = pXIMLOTask->GetItem(_T("TaskNode"));

	if (pXIMLOTask)
		ImportTask(pXIMLOTask, pTasks, NULL, TRUE); // NULL ==  root

	return IIER_SUCCESS;
}

bool CMLOImporter::ImportTask(const CXmlItem* pXIMLOTask, ITASKLISTBASE* pDestTaskFile, HTASKITEM hParent, BOOL bAndSiblings) const
{
	ASSERT (pXIMLOTask);

	HTASKITEM hTask = pDestTaskFile->NewTask(pXIMLOTask->GetItemValue(_T("Caption")), hParent, 0);
	ASSERT(hTask);

	if (!hTask)
		return false;

	// priority (== Importance)
	int nPriority = (pXIMLOTask->GetItemValueI(_T("Importance")) * 10) / 100;

	pDestTaskFile->SetTaskPriority(hTask, (unsigned char)nPriority);

	// categories (== places)
	const CXmlItem* pXIPlace = pXIMLOTask->GetItem(_T("Places"), _T("Place"));

	while (pXIPlace)
	{
		pDestTaskFile->AddTaskCategory(hTask, pXIPlace->GetValue());
		pXIPlace = pXIPlace->GetSibling();
	}

	// estimated time (== EstimateMax)
	double dTimeEst = pXIMLOTask->GetItemValueF(_T("EstimateMax")); // in days

	pDestTaskFile->SetTaskTimeEstimate(hTask, dTimeEst, TDCU_DAYS);

	// due date (== DueDateTime)
	pDestTaskFile->SetTaskDueDate64(hTask, GetDate(pXIMLOTask, _T("DueDateTime")));

	// completion (== CompletionDateTime)
	pDestTaskFile->SetTaskDoneDate64(hTask, GetDate(pXIMLOTask, _T("CompletionDateTime")));

	// comments (== Note)
	CString sComments = pXIMLOTask->GetItemValue(_T("Note"));

	pDestTaskFile->SetTaskComments(hTask, sComments);

	// children
	const CXmlItem* pXIMLOSubTask = pXIMLOTask->GetItem(_T("TaskNode"));

	if (pXIMLOSubTask)
	{
		if (!ImportTask(pXIMLOSubTask, pDestTaskFile, hTask, TRUE))
			return false;
	}

	// handle sibling tasks WITHOUT RECURSION
	if (bAndSiblings)
	{
		pXIMLOTask = pXIMLOTask->GetSibling();
		
		while (pXIMLOTask)
		{
			// FALSE == don't recurse on siblings
			if (!ImportTask(pXIMLOTask, pDestTaskFile, hParent, FALSE))
				return false;
			
			pXIMLOTask = pXIMLOTask->GetSibling();
		}
	}

	return true;
}

time64_t CMLOImporter::GetDate(const CXmlItem* pXIMLOTask, LPCTSTR szDateField) const
{
	CString sDate = pXIMLOTask->GetItemValue(szDateField);
	time64_t date = 0;
	
	if (!CDateHelper::DecodeDate(sDate, date, TRUE)) // include time
		date = 0;
	
	return date;
}
