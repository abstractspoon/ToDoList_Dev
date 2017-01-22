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

CMLOImporter::CMLOImporter() : m_hIcon(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MYLIFEORGANISED);
}

CMLOImporter::~CMLOImporter()
{
	::DestroyIcon(m_hIcon);
}

void CMLOImporter::SetLocalizer(ITransText* /*pTT*/)
{
	//CLocalizer::Initialize(pTT);
}

IIMPORT_RESULT CMLOImporter::Import(LPCTSTR szSrcFilePath, ITaskList* pDestTaskFile, bool /*bSilent*/, IPreferences* /*pPrefs*/, LPCTSTR /*szKey*/)
{
	ITaskList5* pTL5 = GetITLInterface<ITaskList5>(pDestTaskFile, IID_TASKLIST5);
	
	if (!pTL5)
	{
		ASSERT(0);
		return IIR_BADINTERFACE;
	}
	
	CXmlFile file;

	if (!file.Load(szSrcFilePath, _T("MyLifeOrganized-xml")))
	{
		switch (file.GetLastFileError())
		{
		case XFL_BADMSXML:
		case XFL_MISSINGROOT:
			return IIR_BADFORMAT;
		}

		// else
		return IIR_BADFILE;
	}

	const CXmlItem* pXIMLOTree = file.GetItem(_T("TaskTree"));

	if (!pXIMLOTree)
		return IIR_BADFORMAT;

	const CXmlItem* pXIMLOTask = pXIMLOTree->GetItem(_T("TaskNode"));

	if (!pXIMLOTask)
		return IIR_BADFORMAT;

	// this first node always seems to be untitled 
	// so we get the first subnode
	// If that is not present it's just an empty file
	pXIMLOTask = pXIMLOTask->GetItem(_T("TaskNode"));

	if (pXIMLOTask)
		ImportTask(pXIMLOTask, pTL5, NULL, TRUE); // NULL ==  root

	return IIR_SUCCESS;
}

bool CMLOImporter::ImportTask(const CXmlItem* pXIMLOTask, ITaskList5* pDestTaskFile, HTASKITEM hParent, BOOL bAndSiblings) const
{
	ASSERT (pXIMLOTask);

	HTASKITEM hTask = pDestTaskFile->NewTask(pXIMLOTask->GetItemValue(_T("Caption")), hParent);

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
	pDestTaskFile->SetTaskDueDate(hTask, GetDate(pXIMLOTask, _T("DueDateTime")));

	// completion (== CompletionDateTime)
	pDestTaskFile->SetTaskDoneDate(hTask, GetDate(pXIMLOTask, _T("CompletionDateTime")));

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

time_t CMLOImporter::GetDate(const CXmlItem* pXIMLOTask, LPCTSTR szDateField) const
{
	CString sDate = pXIMLOTask->GetItemValue(szDateField);
	time_t date = 0;
	
	if (!CDateHelper::DecodeDate(sDate, date))
		date = 0;
	
	return date;
}
