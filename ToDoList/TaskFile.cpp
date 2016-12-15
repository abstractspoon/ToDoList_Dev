// TaskFile.cpp: implementation of the CTaskFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaskFile.h"
#include "tdlschemadef.h"
#include "tdcmergetasklist.h"
#include "tdcstruct.h"
#include "todoitem.h"

#include "..\shared\timeedit.h"
#include "..\shared\datehelper.h"
#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\binarydata.h"

#include "..\3rdparty\Base64Coder.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

static const CString NULLSTRING;

//////////////////////////////////////////////////////////////////////

#define STR_MATCH(a, b) (lstrcmp(a, b) == 0)

//////////////////////////////////////////////////////////////////////

// private class to load header
class CTFHeaderParse : public IXmlParse
{
public:
	CTFHeaderParse() {}
	
	virtual BOOL Continue(LPCTSTR szItem, LPCTSTR /*szValue*/) const
	{
		if (TDL_TASK.CompareNoCase(szItem) == 0)
			return FALSE; // stop
		
#ifndef NO_TL_ENCRYPTDECRYPT
		if (CXmlFileEx::XFE_ENCODEDDATA.CompareNoCase(szItem) == 0)
			return FALSE; // stop
#endif

		return TRUE; // continue
	}
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define GET_TASK(t, h, r) { t = TaskFromHandle(h); if (!t) return r; }

CTaskFile::CTaskFile(LPCTSTR szPassword) 
	: 
	m_dwNextUniqueID(1), 
	m_bISODates(FALSE), 
	m_bHideParentID(FALSE),

#ifdef NO_TL_ENCRYPTDECRYPT
	CXmlFile(TDL_ROOT)
#else
	CXmlFileEx(TDL_ROOT, szPassword)
#endif
{
}

CTaskFile::CTaskFile(const CTaskFile& tasks, LPCTSTR szPassword)
	: 
	m_dwNextUniqueID(1), 
	m_bISODates(FALSE), 
	m_bHideParentID(FALSE),

#ifdef NO_TL_ENCRYPTDECRYPT
	CXmlFile(TDL_ROOT)
#else
	CXmlFileEx(TDL_ROOT, szPassword)
#endif
{
	CopyFrom(tasks);
}

CTaskFile::CTaskFile(const ITaskList* pTasks, LPCTSTR szPassword)
	: 
	m_dwNextUniqueID(1), 
	m_bISODates(FALSE), 
	m_bHideParentID(FALSE),

#ifdef NO_TL_ENCRYPTDECRYPT
	CXmlFile(TDL_ROOT)
#else
	CXmlFileEx(TDL_ROOT, szPassword)
#endif
{
	CopyFrom(pTasks);
}

CTaskFile::~CTaskFile()
{
}

HRESULT CTaskFile::QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
{
	*ppvObject = NULL;

	// compare riid against our supported version numbers
	if (IsEqualIID(riid, IID_TASKLIST2))
	{
		*ppvObject = reinterpret_cast<ITaskList2*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_TASKLIST3))
	{
		*ppvObject = reinterpret_cast<ITaskList3*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_TASKLIST4))
	{
		*ppvObject = reinterpret_cast<ITaskList4*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_TASKLIST5))
	{
		*ppvObject = reinterpret_cast<ITaskList5*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_TASKLIST6))
	{
		*ppvObject = reinterpret_cast<ITaskList6*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_TASKLIST7))
	{
		*ppvObject = reinterpret_cast<ITaskList7*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_TASKLIST8))
	{
		*ppvObject = reinterpret_cast<ITaskList8*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_TASKLIST9))
	{
		*ppvObject = reinterpret_cast<ITaskList9*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_TASKLIST10))
	{
		*ppvObject = reinterpret_cast<ITaskList10*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_TASKLIST11))
	{
		*ppvObject = reinterpret_cast<ITaskList11*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_TASKLIST12))
	{
		*ppvObject = reinterpret_cast<ITaskList12*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_TASKLIST13))
	{
		*ppvObject = reinterpret_cast<ITaskList13*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_TASKLIST14))
	{
		*ppvObject = reinterpret_cast<ITaskList14*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_TASKLIST15))
	{
		*ppvObject = reinterpret_cast<ITaskList15*>(this);
		AddRef();
	}
	else
	{
		ASSERT(0);
	}
	
	return (*ppvObject ? S_OK : E_NOTIMPL);
}

#ifndef NO_TL_ENCRYPTDECRYPT

BOOL CTaskFile::Decrypt(LPCTSTR szPassword)
{
	BOOL bWasEncrypted = IsEncrypted();
	BOOL bResult = CXmlFileEx::Decrypt(szPassword);

	if (bResult && bWasEncrypted)
	{
		// fix corrupted tasklist where the root item has an ID
		CXmlItem* pXI = GetItem(TDL_TASKID);

		while (pXI)
		{
			DeleteItem(pXI);
			pXI = GetItem(TDL_TASKID);
		}

		m_dwNextUniqueID = (DWORD)GetItemValueI(TDL_NEXTUNIQUEID);

		if (m_dwNextUniqueID <= 0)
			m_dwNextUniqueID = 1; // always > 0

		ClearHandleMap();
	}

	return bResult;
}

#endif

BOOL CTaskFile::Load(LPCTSTR szFilePath, IXmlParse* pCallback, BOOL bDecrypt)
{
#ifdef NO_TL_ENCRYPTDECRYPT
	BOOL bRes = CXmlFile::Load(szFilePath, TDL_ROOT, pCallback);
#else
	BOOL bRes = CXmlFileEx::Load(szFilePath, TDL_ROOT, pCallback, bDecrypt);
#endif
	
	if (bRes)
	{
		m_dwNextUniqueID = (DWORD)GetItemValueI(TDL_NEXTUNIQUEID);
		
		if (m_dwNextUniqueID <= 0)
			m_dwNextUniqueID = 1; // always > 0
		
		ClearHandleMap();
		UpgradeArrays();
		CleanUp();
	}
	
	return bRes;
}

BOOL CTaskFile::LoadContent(const CString& sContent)
{
	if (XMLBASE::LoadContent(sContent, TDL_ROOT))
	{
		ClearHandleMap();
		return TRUE;
	}

	return FALSE;
}

BOOL CTaskFile::LoadHeader(LPCTSTR szFilePath, TASKFILE_HEADER* pHeader)
{
	CTFHeaderParse tfhp;

	if (Load(szFilePath, &tfhp, FALSE))
	{
		if (pHeader)
			GetHeader(*pHeader);

		return TRUE;
	}

	// else
	return FALSE;
}

void CTaskFile::SetHeader(const TASKFILE_HEADER& header)
{
	if (!header.sXmlHeader.IsEmpty())
		VERIFY(SetXmlHeader(header.sXmlHeader));

	if (!header.sXslHeader.IsEmpty())
		VERIFY(SetXslHeader(header.sXslHeader));

	// add other taskfile related stuff
	if (!header.sProjectName.IsEmpty())
		VERIFY(SetProjectName(header.sProjectName));

	if (header.bArchive != -1)
		VERIFY(SetArchive(header.bArchive));

	if (header.nFileFormat != -1)
		VERIFY(SetFileFormat(header.nFileFormat));

	if (header.dtEarliestDue != 0)
		VERIFY(SetEarliestDueDate(header.dtEarliestDue));

	if (header.dwNextID > 0)
		VERIFY(SetNextUniqueID(header.dwNextID));
	else 
		VERIFY(SetNextUniqueID(m_dwNextUniqueID));

	if (!header.sFileName.IsEmpty())
		VERIFY(SetFileName(header.sFileName));

	if (header.nFileVersion != -1)
		VERIFY(SetFileVersion(header.nFileVersion));
}

void CTaskFile::GetHeader(TASKFILE_HEADER& header) const
{
	header.sXmlHeader = m_sXmlHeader;
	header.sXslHeader = GetXslHeader();
	header.sProjectName = GetProjectName();
	header.sFileName = GetItemValue(TDL_FILENAME);
	header.sCheckedOutTo = GetCheckOutTo();
	header.bArchive = IsArchive();
	header.bUnicode = (GetFormat() == SFEF_UTF16);
	header.dtEarliestDue = GetItemValueF(TDL_EARLIESTDUEDATE);
	header.dwNextID = m_dwNextUniqueID;
	header.nFileFormat = GetFileFormat();
	header.nFileVersion = GetFileVersion();
}

BOOL CTaskFile::LoadEx(IXmlParse* pCallback)
{
	BOOL bResult = XMLBASE::LoadEx(TDL_ROOT, pCallback);

#ifdef NO_TL_ENCRYPTDECRYPT
	if (bResult)
#else
	if (bResult && !IsEncrypted())
#endif
	{
		// initialize 
		m_dwNextUniqueID = (DWORD)GetItemValueI(TDL_NEXTUNIQUEID);

		if (m_dwNextUniqueID <= 0)
			m_dwNextUniqueID = 1; // always > 0

		ClearHandleMap();
		UpgradeArrays();
		CleanUp();
	}

	return bResult;
}

void CTaskFile::CleanUp(HTASKITEM hTask)
{
	// Tasks should have no 'value' just attributes
	// but MSXML does something that I do not yet understand
	if (hTask != NULL)
		TaskFromHandle(hTask)->ClearValue();
	
	// process children
	HTASKITEM hChild = GetFirstTask(hTask);
	
	while (hChild)
	{
		CleanUp(hChild);
		hChild = GetNextTask(hChild);
	}
}

void CTaskFile::UpgradeArrays(HTASKITEM hTask)
{
	CStringArray aItems;

	if (hTask == NULL) // root
	{
		// test for existence of 'old' arrays
		BOOL bHasOld = FALSE;

		if (LegacyGetArray(TDL_TASKCATEGORY, aItems) > 1)
		{
			bHasOld = TRUE;

			LegacyDeleteArray(TDL_TASKCATEGORY);
			SetArray(TDL_TASKCATEGORY, aItems);
		}
		
		if (LegacyGetArray(TDL_TASKTAG, aItems) > 1)
		{
			bHasOld = TRUE;
			
			LegacyDeleteArray(TDL_TASKTAG);
			SetArray(TDL_TASKTAG, aItems);
		}
		
		if (LegacyGetArray(TDL_TASKALLOCTO, aItems) > 1)
		{
			bHasOld = TRUE;
			
			LegacyDeleteArray(TDL_TASKALLOCTO);
			SetArray(TDL_TASKALLOCTO, aItems);
		}
		
		// these don't affect tasks
		if (LegacyGetArray(TDL_TASKSTATUS, aItems) > 1)
		{
			LegacyDeleteArray(TDL_TASKSTATUS);
			SetArray(TDL_TASKSTATUS, aItems);
		}

		if (LegacyGetArray(TDL_TASKALLOCBY, aItems) > 1)
		{
			LegacyDeleteArray(TDL_TASKALLOCBY);
			SetArray(TDL_TASKALLOCBY, aItems);
		}
		
		if (LegacyGetArray(TDL_TASKVERSION, aItems) > 1)
		{
			LegacyDeleteArray(TDL_TASKVERSION);
			SetArray(TDL_TASKVERSION, aItems);
		}

		// process top-level children if any old arrays were found
		if (!bHasOld)
			return; // nothing else to do
	}
	else
	{
		// test for existence of 'old' arrays
		if (LegacyGetTaskArray(hTask, TDL_TASKNUMCATEGORY_DEP, TDL_TASKCATEGORY, aItems) > 1)
		{
			LegacyDeleteTaskArray(hTask, TDL_TASKNUMCATEGORY_DEP, TDL_TASKCATEGORY);
			SetTaskCategories(hTask, aItems);
		}

		if (LegacyGetTaskArray(hTask, TDL_TASKNUMTAGS_DEP, TDL_TASKTAG, aItems) > 1)
		{
			LegacyDeleteTaskArray(hTask, TDL_TASKNUMTAGS_DEP, TDL_TASKTAG);
			SetTaskTags(hTask, aItems);
		}
		
		if (LegacyGetTaskArray(hTask, TDL_TASKNUMALLOCTO_DEP, TDL_TASKALLOCTO, aItems) > 1)
		{
			LegacyDeleteTaskArray(hTask, TDL_TASKNUMALLOCTO_DEP, TDL_TASKALLOCTO);
			SetTaskAllocatedTo(hTask, aItems);
		}
		
		if (LegacyGetTaskArray(hTask, TDL_TASKNUMDEPENDENCY_DEP, TDL_TASKDEPENDENCY, aItems) > 1)
		{
			LegacyDeleteTaskArray(hTask, TDL_TASKNUMDEPENDENCY_DEP, TDL_TASKDEPENDENCY);
			SetTaskDependencies(hTask, aItems);
		}
	}

	// process children
	HTASKITEM hChild = GetFirstTask(hTask);
	
	while (hChild)
	{
		UpgradeArrays(hChild);
		hChild = GetNextTask(hChild);
	}
}

BOOL CTaskFile::SaveEx()
{
	// insert application version
	SetItemValue(TDL_APPVER, FileMisc::GetAppVersion());

	return XMLBASE::SaveEx();
}

BOOL CTaskFile::CopyFrom(const CTaskFile& tasks)
{
	XMLBASE::CopyFrom(tasks);

	m_dwNextUniqueID = tasks.GetNextUniqueID();
	ClearHandleMap();

	return TRUE;
}

BOOL CTaskFile::CopyFrom(const ITaskList* pTasks)
{
	Reset();
	
	ASSERT(GetTaskCount() == 0);

	// copy header
	SetProjectName(pTasks->GetProjectName());
	SetFileVersion(pTasks->GetFileVersion());
	
	CTDCCustomAttribDefinitionArray aAttrib;
	
	if (GetCustomAttributeDefs(pTasks, aAttrib))
		SetCustomAttributeDefs(aAttrib);
	
	// copy tasks
	return CopyTaskFrom(pTasks, pTasks->GetFirstTask(), NULL);
}

BOOL CTaskFile::CopyTasksTo(ITaskList* pTasks)
{
	ITaskList9* pTL9 = GetITLInterface<ITaskList9>(pTasks, IID_TASKLIST9);
						
	if (pTL9)
	{
		pTL9->Reset();

		ASSERT(pTL9->GetTaskCount() == 0);
	}

	// copy tasks
	return CopyTaskTo(GetFirstTask(), pTasks, NULL);
}

void CTaskFile::Reset()
{
	XMLBASE::Reset();

	ClearHandleMap();

	m_dwNextUniqueID = 1;
	m_bISODates = FALSE;
	m_sHtmlImgFolder.Empty();
}

BOOL CTaskFile::CopyTaskFrom(const ITaskList* pSrcTasks, HTASKITEM hSrcTask, HTASKITEM hDestParent, 
							 BOOL bResetID, CID2IDMap* pMapID2ID)
{
	return CopyTask(pSrcTasks, hSrcTask, this, hDestParent, bResetID, pMapID2ID, TRUE);
}

BOOL CTaskFile::CopyTaskTo(HTASKITEM hSrcTask, ITaskList* pDestTasks, HTASKITEM hDestParent, 
						   BOOL bResetID, CID2IDMap* pMapID2ID)
{
	return CopyTask(this, hSrcTask, pDestTasks, hDestParent, bResetID, pMapID2ID, TRUE);
}

BOOL CTaskFile::CopyTask(const ITaskList* pTasksSrc, HTASKITEM hTaskSrc, 
						 ITaskList* pTasksDest, HTASKITEM hTaskDestParent, 
						 BOOL bResetID, CID2IDMap* pMapID2ID, BOOL bAndSiblings)
{
	if (hTaskSrc == NULL)
		return TRUE;

	HTASKITEM hTaskDest = NULL;
	
	const ITaskList8* pTL8Src = GetITLInterface<ITaskList8>(pTasksSrc, IID_TASKLIST8);
	ITaskList8* pTL8Dest = GetITLInterface<ITaskList8>(pTasksDest, IID_TASKLIST8);
							
	if (pTL8Src && pTL8Dest && !bResetID)
	{
		hTaskDest = pTL8Dest->NewTask(pTL8Src->GetTaskTitle(hTaskSrc), hTaskDestParent, pTL8Src->GetTaskID(hTaskSrc));
	}
	else
	{
		hTaskDest = pTasksDest->NewTask(pTasksSrc->GetTaskTitle(hTaskSrc), hTaskDestParent);

		if (pMapID2ID)
			pMapID2ID->SetAt(pTL8Dest->GetTaskID(hTaskDest), pTL8Src->GetTaskID(hTaskSrc));
	}

	if (!hTaskDest)
		return FALSE;

	// simple attributes
	pTasksDest->SetTaskComments(hTaskDest, pTasksSrc->GetTaskComments(hTaskSrc));
	pTasksDest->SetTaskAllocatedTo(hTaskDest, pTasksSrc->GetTaskAllocatedTo(hTaskSrc));
	pTasksDest->SetTaskAllocatedBy(hTaskDest, pTasksSrc->GetTaskAllocatedBy(hTaskSrc));
	pTasksDest->SetTaskCategory(hTaskDest, pTasksSrc->GetTaskCategory(hTaskSrc));
	pTasksDest->SetTaskStatus(hTaskDest, pTasksSrc->GetTaskStatus(hTaskSrc));
	pTasksDest->SetTaskFileLinkPath(hTaskDest, pTasksSrc->GetTaskFileLinkPath(hTaskSrc));
	pTasksDest->SetTaskColor(hTaskDest, pTasksSrc->GetTaskColor(hTaskSrc));
	pTasksDest->SetTaskPriority(hTaskDest, pTasksSrc->GetTaskPriority(hTaskSrc, FALSE));
	pTasksDest->SetTaskPercentDone(hTaskDest, pTasksSrc->GetTaskPercentDone(hTaskSrc, FALSE));
	pTasksDest->SetTaskLastModified(hTaskDest, pTasksSrc->GetTaskLastModified(hTaskSrc));
	pTasksDest->SetTaskDoneDate(hTaskDest, pTasksSrc->GetTaskDoneDate(hTaskSrc));
	pTasksDest->SetTaskDueDate(hTaskDest, pTasksSrc->GetTaskDueDate(hTaskSrc));
	pTasksDest->SetTaskStartDate(hTaskDest, pTasksSrc->GetTaskStartDate(hTaskSrc));
	pTasksDest->SetTaskFlag(hTaskDest, pTasksSrc->IsTaskFlagged(hTaskSrc));
	
	// times
	TDC_UNITS nUnits;
	double dTime;
	
	dTime = pTasksSrc->GetTaskTimeEstimate(hTaskSrc, nUnits, FALSE);
	pTasksDest->SetTaskTimeEstimate(hTaskDest, dTime, nUnits);

	dTime = pTasksSrc->GetTaskTimeSpent(hTaskSrc, nUnits, FALSE);
	pTasksDest->SetTaskTimeSpent(hTaskDest, dTime, nUnits);
	
	// later interfaces
	do 
	{
	// ---------------------------------------------------------------------------
		const ITaskList2* pTL2Src = GetITLInterface<ITaskList2>(pTasksSrc, IID_TASKLIST2);
		ITaskList2* pTL2Dest = GetITLInterface<ITaskList2>(pTasksDest, IID_TASKLIST2);
		
		if (!(pTL2Src && pTL2Dest))
			break;
		
		pTL2Dest->SetTaskCreatedBy(hTaskDest, pTL2Src->GetTaskCreatedBy(hTaskSrc));
		pTL2Dest->SetTaskCreationDate(hTaskDest, pTL2Src->GetTaskCreationDate(hTaskSrc));
		
		// ---------------------------------------------------------------------------
		const ITaskList3* pTL3Src = GetITLInterface<ITaskList3>(pTasksSrc, IID_TASKLIST3);
		ITaskList3* pTL3Dest = GetITLInterface<ITaskList3>(pTasksDest, IID_TASKLIST3);
		
		if (!(pTL3Src && pTL3Dest))
			break;
		
		pTL3Dest->SetTaskRisk(hTaskDest, pTL3Src->GetTaskRisk(hTaskSrc, FALSE));
		pTL3Dest->SetTaskExternalID(hTaskDest, pTL3Src->GetTaskExternalID(hTaskSrc));
		
		// ---------------------------------------------------------------------------
		const ITaskList4* pTL4Src = GetITLInterface<ITaskList4>(pTasksSrc, IID_TASKLIST4);
		ITaskList4* pTL4Dest = GetITLInterface<ITaskList4>(pTasksDest, IID_TASKLIST4);
		
		if (!(pTL4Src && pTL4Dest))
			break;
		
		pTL4Dest->SetTaskDependency(hTaskDest, pTL4Src->GetTaskDependency(hTaskSrc));
		pTL4Dest->SetTaskCost(hTaskDest, pTL4Src->GetTaskCost(hTaskSrc, FALSE));
		
		// ---------------------------------------------------------------------------
		const ITaskList5* pTL5Src = GetITLInterface<ITaskList5>(pTasksSrc, IID_TASKLIST5);
		ITaskList5* pTL5Dest = GetITLInterface<ITaskList5>(pTasksDest, IID_TASKLIST5);
		
		if (!(pTL5Src && pTL5Dest))
			break;
		
		int nNumCat = pTL5Src->GetTaskCategoryCount(hTaskSrc);
		
		for (int nCat = 0; nCat < nNumCat; nCat++)
			pTL5Dest->AddTaskCategory(hTaskDest, pTL5Src->GetTaskCategory(hTaskSrc, nCat));
		
		// ---------------------------------------------------------------------------
		const ITaskList6* pTL6Src = GetITLInterface<ITaskList6>(pTasksSrc, IID_TASKLIST6);
		ITaskList6* pTL6Dest = GetITLInterface<ITaskList6>(pTasksDest, IID_TASKLIST6);
		
		if (!(pTL6Src && pTL6Dest))
			break;
		
		pTL6Dest->SetTaskVersion(hTaskDest, pTL6Src->GetTaskVersion(hTaskSrc));
		
		// ---------------------------------------------------------------------------
		const ITaskList7* pTL7Src = GetITLInterface<ITaskList7>(pTasksSrc, IID_TASKLIST7);
		ITaskList7* pTL7Dest = GetITLInterface<ITaskList7>(pTasksDest, IID_TASKLIST7);
		
		if (!(pTL7Src && pTL7Dest))
			break;
		
		int nNumDepends = pTL7Src->GetTaskDependencyCount(hTaskSrc);
		
		for (int nDepend = 0; nDepend < nNumDepends; nDepend++)
			pTL7Dest->AddTaskDependency(hTaskDest, pTL7Src->GetTaskDependency(hTaskSrc, nDepend));
		
		int nNumAllocTo = pTL7Src->GetTaskAllocatedToCount(hTaskSrc);
		
		for (int nAlloc = 0; nAlloc < nNumAllocTo; nAlloc++)
			pTL7Dest->AddTaskAllocatedTo(hTaskDest, pTL7Src->GetTaskAllocatedTo(hTaskSrc, nAlloc));
		
		// ---------------------------------------------------------------------------
		if (!(pTL8Src && pTL8Dest)) // already got these earlier
			break;
		
		// custom comments
		pTL8Dest->SetTaskAttribute(hTaskDest, TDL_TASKCUSTOMCOMMENTS, pTL8Src->GetTaskAttribute(hTaskSrc, TDL_TASKCUSTOMCOMMENTS));
		pTL8Dest->SetTaskAttribute(hTaskDest, TDL_TASKCOMMENTSTYPE, pTL8Src->GetTaskAttribute(hTaskSrc, TDL_TASKCOMMENTSTYPE));
		
		// ---------------------------------------------------------------------------
		const ITaskList9* pTL9Src = GetITLInterface<ITaskList9>(pTasksSrc, IID_TASKLIST9);
		ITaskList9* pTL9Dest = GetITLInterface<ITaskList9>(pTasksDest, IID_TASKLIST9);
		
		if (!(pTL9Src && pTL9Dest))
			break;
		
		int nNumTag = pTL9Src->GetTaskTagCount(hTaskSrc);
		
		for (int nTag = 0; nTag < nNumTag; nTag++)
			pTL9Dest->AddTaskTag(hTaskDest, pTL9Src->GetTaskTag(hTaskSrc, nTag));
		
		pTL9Dest->SetTaskPosition(hTaskDest, pTL9Src->GetTaskPositionString(hTaskSrc));
		
		// ---------------------------------------------------------------------------
		const ITaskList10* pTL10Src = GetITLInterface<ITaskList10>(pTasksSrc, IID_TASKLIST10);
		ITaskList10* pTL10Dest = GetITLInterface<ITaskList10>(pTasksDest, IID_TASKLIST10);
		
		if (!(pTL10Src && pTL10Dest))
			break;
		
		int nCustom = pTL10Src->GetCustomAttributeCount();
		
		while (nCustom--)
		{
			CString sCustomID = pTL10Src->GetCustomAttributeID(nCustom);
			CString sCustomData = pTL10Src->GetTaskCustomAttributeData(hTaskSrc, sCustomID);
			
			pTL10Dest->SetTaskCustomAttributeData(hTaskDest, sCustomID, sCustomData);
		}
		
		// ---------------------------------------------------------------------------
		const ITaskList11* pTL11Src = GetITLInterface<ITaskList11>(pTasksSrc, IID_TASKLIST11);
		ITaskList11* pTL11Dest = GetITLInterface<ITaskList11>(pTasksDest, IID_TASKLIST11);
		
		if (!(pTL11Src && pTL11Dest))
			break;
		
		pTL11Dest->SetTaskIcon(hTaskDest, pTL11Src->GetTaskIcon(hTaskSrc));
		
		// ---------------------------------------------------------------------------
		const ITaskList12* pTL12Src = GetITLInterface<ITaskList12>(pTasksSrc, IID_TASKLIST12);
		ITaskList12* pTL12Dest = GetITLInterface<ITaskList12>(pTasksDest, IID_TASKLIST12);
		
		if (!(pTL12Src && pTL12Dest))
			break;
		
		time64_t tDate = 0;
		
		if (pTL12Src->GetTaskCreationDate64(hTaskSrc, tDate))
			pTL12Dest->SetTaskCreationDate64(hTaskDest, tDate);
		
		if (pTL12Src->GetTaskStartDate64(hTaskSrc, FALSE, tDate))
			pTL12Dest->SetTaskStartDate64(hTaskDest, tDate);
		
		if (pTL12Src->GetTaskDueDate64(hTaskSrc, FALSE, tDate))
			pTL12Dest->SetTaskDueDate64(hTaskDest, tDate);
		
		if (pTL12Src->GetTaskDoneDate64(hTaskSrc, tDate))
			pTL12Dest->SetTaskDoneDate64(hTaskDest, tDate);
		
		if (pTL12Src->GetTaskLastModified64(hTaskSrc, tDate))
			pTL12Dest->SetTaskLastModified64(hTaskDest, tDate);
		
		// ---------------------------------------------------------------------------
		const ITaskList13* pTL13Src = GetITLInterface<ITaskList13>(pTasksSrc, IID_TASKLIST13);
		ITaskList13* pTL13Dest = GetITLInterface<ITaskList13>(pTasksDest, IID_TASKLIST13);
		
		if (!(pTL13Src && pTL13Dest))
			break;

		int nRegularity, nReuse, nNumOccur;
		DWORD dwSpecific1, dwSpecific2;
		BOOL bRecalcFromDue;
		
		if (pTL13Src->GetTaskRecurrence(hTaskSrc, nRegularity, dwSpecific1, dwSpecific2, bRecalcFromDue, nReuse, nNumOccur))
			pTL13Dest->SetTaskRecurrence(hTaskDest, nRegularity, dwSpecific1, dwSpecific2, bRecalcFromDue, nReuse, nNumOccur);
		
		// ---------------------------------------------------------------------------
		const ITaskList14* pTL14Src = GetITLInterface<ITaskList14>(pTasksSrc, IID_TASKLIST14);
		ITaskList14* pTL14Dest = GetITLInterface<ITaskList14>(pTasksDest, IID_TASKLIST14);
		
		if (!(pTL14Src && pTL14Dest))
			break;

		int nNumFile = pTL14Src->GetTaskFileLinkCount(hTaskSrc);
		
		for (int nFile = 0; nFile < nNumFile; nFile++)
			pTL14Dest->AddTaskFileLink(hTaskDest, pTL14Src->GetTaskFileLink(hTaskSrc, nFile));
		// ---------------------------------------------------------------------------
		const ITaskList15* pTL15Src = GetITLInterface<ITaskList15>(pTasksSrc, IID_TASKLIST15);
		ITaskList15* pTL15Dest = GetITLInterface<ITaskList15>(pTasksDest, IID_TASKLIST15);
		
		if (!(pTL15Src && pTL15Dest))
			break;

		// Note: we don't deliberately don't expose SetTaskReferenceID to clients
		CString sRefID = Misc::Format(pTL15Src->GetTaskReferenceID(hTaskSrc));
		pTL15Dest->SetTaskAttribute(hTaskDest, TDL_TASKREFID, sRefID);
		// ---------------------------------------------------------------------------
	} 
	while (0);

	// first child task
	CopyTask(pTasksSrc, pTasksSrc->GetFirstTask(hTaskSrc), pTasksDest, hTaskDest, bResetID, pMapID2ID, TRUE);

	// handle sibling tasks WITHOUT RECURSION
	if (bAndSiblings)
	{
		HTASKITEM hSibling = pTasksSrc->GetNextTask(hTaskSrc);
		
		while (hSibling)
		{
			// FALSE == don't recurse on siblings
			CopyTask(pTasksSrc, hSibling, pTasksDest, hTaskDestParent, bResetID, pMapID2ID, FALSE);
			hSibling = pTasksSrc->GetNextTask(hSibling);
		}
	}

	return TRUE;
}

#ifndef NO_TL_MERGE
int CTaskFile::Merge(const CTaskFile& tasks, BOOL bByID, BOOL bMoveExist)
{
	CTDCMergeTasklist mtdl(bByID ? TDLM_BYID : TDLM_BYTITLE, 
						bMoveExist ? TDLM_MOVE : TDLM_LEAVE);

	int nMerge = mtdl.Merge(tasks.Root(), Root());
	BuildHandleMap();

	// update next uniqueID
	SetNextUniqueID(mtdl.GetNextID());

	return nMerge;
}

int CTaskFile::Merge(LPCTSTR szTaskFilePath, BOOL bByID, BOOL bMoveExist)
{
	CTDCMergeTasklist mtdl(bByID ? TDLM_BYID : TDLM_BYTITLE, 
						bMoveExist ? TDLM_MOVE : TDLM_LEAVE);
	
	int nMerge = mtdl.Merge(szTaskFilePath, Root());
	BuildHandleMap();

	// update next uniqueID
	SetNextUniqueID(mtdl.GetNextID());

	return nMerge;
}
#endif

DWORD CTaskFile::GetNextUniqueID() const
{
	return m_dwNextUniqueID;
}

void CTaskFile::BuildHandleMap() const
{
	ClearHandleMap();
	AddTaskToMap(GetItem(TDL_TASK), TRUE, TRUE); // first top level item
}

void CTaskFile::ClearHandleMap() const
{
	m_mapHandles.RemoveAll();
}

void CTaskFile::AddTaskToMap(const CXmlItem* pXITask, BOOL bAndSiblings, BOOL bAndSubTasks) const
{
	if (pXITask)
	{
		// sanity check
		ASSERT (pXITask->NameIs(TDL_TASK));
		
		m_mapHandles[(HTASKITEM)pXITask] = const_cast<CXmlItem*>(pXITask);

		// children
		// note: we only need do the first child
		if (bAndSubTasks)
			AddTaskToMap(pXITask->GetItem(TDL_TASK), TRUE, TRUE);

		// handle sibling tasks WITHOUT RECURSION
		if (bAndSiblings)
		{
			pXITask = pXITask->GetSibling();
			
			while (pXITask)
			{
				// FALSE == don't recurse on siblings
				AddTaskToMap(pXITask, FALSE, bAndSubTasks);
				pXITask = pXITask->GetSibling();
			}
		}
	}
}

void CTaskFile::RemoveTaskFromMap(const CXmlItem* pXITask) const
{
	if (pXITask)
	{
		// sanity check
		ASSERT (pXITask->NameIs(TDL_TASK));
		
		m_mapHandles.RemoveKey((HTASKITEM)pXITask);

		// children
		const CXmlItem* pXIChild = pXITask->GetItem(TDL_TASK);

		while (pXIChild)
		{
			RemoveTaskFromMap(pXIChild);
			pXIChild = pXIChild->GetSibling();
		}
	}
}

CXmlItem* CTaskFile::TaskFromHandle(HTASKITEM hTask) const
{
#ifdef _DEBUG
	CXmlItem* pXITask = NULL;

	if (hTask)
	{
		if (m_mapHandles.GetCount() == 0)
			BuildHandleMap();

		m_mapHandles.Lookup(hTask, pXITask);
	}

	return pXITask;
#else
	return static_cast<CXmlItem*>(hTask);
#endif
}

bool CTaskFile::IsArchive() const
{
	return (NULL != GetItem(TDL_ARCHIVE));
}

BOOL CTaskFile::IsCheckedOutTo(const CString& sCheckedOutTo) const
{
	return (sCheckedOutTo.CompareNoCase(GetCheckOutTo()) == 0);
}	

bool CTaskFile::IsSourceControlled() const
{
	return (NULL != GetItem(TDL_CHECKEDOUTTO));
}

LPCTSTR CTaskFile::GetProjectName() const
{
	return GetItemValue(TDL_PROJECTNAME);
}

BOOL CTaskFile::SetMetaData(const CMapStringToString& mapMetaData)
{
	return SetMetaData(Root(), mapMetaData);
}

int CTaskFile::GetMetaData(CMapStringToString& mapMetaData) const
{
	return GetMetaData(Root(), mapMetaData);
}

BOOL CTaskFile::SetAttributeVisibility(const TDCCOLEDITFILTERVISIBILITY& vis)
{
	// delete old visibility settings
	DeleteItem(TDL_ATTRIBVIS);

	CXmlItem *pXIVis = AddItem(TDL_ATTRIBVIS, _T(""), XIT_ELEMENT);
	ASSERT(pXIVis);

	// columns
	const CTDCColumnIDMap& mapCols = vis.GetVisibleColumns();
	POSITION pos = mapCols.GetStartPosition();
	
	while (pos)
	{
		TDC_COLUMN nColID = mapCols.GetNextColumn(pos);
		pXIVis->AddItem(TDL_ATTRIBVISCOL, nColID, XIT_ELEMENT);
	}

	TDL_SHOWATTRIB nShow = vis.GetShowEditsAndFilters();
	pXIVis->AddItem(TDL_ATTRIBVISSHOWEDITFILTER, nShow);

	if (nShow == TDLSA_ANY)
	{
		// edit fields
		const CTDCAttributeMap& mapEdit = vis.GetVisibleEditFields();
		pos = mapEdit.GetStartPosition();

		while (pos)
		{
			TDC_ATTRIBUTE nAttrib = mapEdit.GetNextAttribute(pos);
			pXIVis->AddItem(TDL_ATTRIBVISEDIT, nAttrib, XIT_ELEMENT);
		}

		// filter fields
		const CTDCAttributeMap& mapFilter = vis.GetVisibleFilterFields();
		pos = mapFilter.GetStartPosition();

		while (pos)
		{
			TDC_ATTRIBUTE nAttrib = mapFilter.GetNextAttribute(pos);
			pXIVis->AddItem(TDL_ATTRIBVISFILTER, nAttrib, XIT_ELEMENT);
		}
	}

	return TRUE;
}

BOOL CTaskFile::GetAttributeVisibility(TDCCOLEDITFILTERVISIBILITY& vis) const
{
	const CXmlItem *pXIVis = GetItem(TDL_ATTRIBVIS);

	if (!pXIVis)
		return FALSE;

	// else
	vis.Clear();

	// columns
	const CXmlItem* pXICol = pXIVis->GetItem(TDL_ATTRIBVISCOL);
	CTDCColumnIDMap mapCols;

	while (pXICol)
	{
		mapCols.AddColumn((TDC_COLUMN)pXICol->GetValueI());
		pXICol = pXICol->GetSibling();
	}

	vis.SetVisibleColumns(mapCols);
	vis.SetShowEditsAndFilters(TDL_SHOWATTRIB(pXIVis->GetItemValueI(TDL_ATTRIBVISSHOWEDITFILTER)));

	if (vis.GetShowEditsAndFilters() == TDLSA_ANY)
	{
		// edit fields
		const CXmlItem* pXIEdit = pXIVis->GetItem(TDL_ATTRIBVISEDIT);
		CTDCAttributeMap mapEdit;

		while (pXIEdit)
		{
			mapEdit.AddAttribute((TDC_ATTRIBUTE)pXIEdit->GetValueI());
			pXIEdit = pXIEdit->GetSibling();
		}

		// filter fields
		const CXmlItem* pXIFilter = pXIVis->GetItem(TDL_ATTRIBVISFILTER);
		CTDCAttributeMap mapFilter;

		while (pXIFilter)
		{
			mapFilter.AddAttribute((TDC_ATTRIBUTE)pXIFilter->GetValueI());
			pXIFilter = pXIFilter->GetSibling();
		}

		vis.SetVisibleEditFields(mapEdit);
		vis.SetVisibleFilterFields(mapFilter);
	}

	return TRUE;
}

BOOL CTaskFile::SetCustomAttributeDefs(const CTDCCustomAttribDefinitionArray& aAttribs)
{
	// delete old attribDef
	DeleteItem(const_cast<CXmlItem*>(GetCustomAttribDefs()));

	int nNumAttrib = aAttribs.GetSize();

	for (int nAttrib = 0; nAttrib < nNumAttrib; nAttrib++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aAttribs.GetData()[nAttrib];

		ASSERT(!attribDef.sUniqueID.IsEmpty());
		ASSERT(!attribDef.sLabel.IsEmpty() || !attribDef.sColumnTitle.IsEmpty());

		CXmlItem* pXIAttribDef = AddItem(TDL_CUSTOMATTRIBDEFS);

		pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBID, attribDef.sUniqueID);
		pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBLABEL, attribDef.sLabel);
		pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBCOLTITLE, attribDef.sColumnTitle);
		pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBTYPE, (int)attribDef.GetAttributeType());
		pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBCOLALIGN, (int)attribDef.nTextAlignment);
		pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBFEATURES, (int)attribDef.dwFeatures);
		pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBENABLED, (int)attribDef.bEnabled);
		
		// format auto list data to follow default list data
		if (attribDef.IsList())
			pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBLISTDATA, attribDef.EncodeListData());
	}

	return TRUE;
}

int CTaskFile::GetCustomAttributeDefs(const ITaskList* pTasks, CTDCCustomAttribDefinitionArray& aAttribs)
{
	aAttribs.RemoveAll(); 

	const ITaskList10* pTasks10 = GetITLInterface<ITaskList10>(pTasks, IID_TASKLIST10);
	
	if (pTasks10)
	{
		// copy custom attribute definitions
		int nNumCustom = pTasks10->GetCustomAttributeCount();
		
		for (int nCustom = 0; nCustom < nNumCustom; nCustom++)
		{
			TDCCUSTOMATTRIBUTEDEFINITION attribDef;
			CString sAttribPath;
			
			attribDef.SetAttributeType(_ttoi(pTasks10->GetCustomAttributeValue(nCustom, TDL_CUSTOMATTRIBTYPE)));
			attribDef.sUniqueID = pTasks10->GetCustomAttributeValue(nCustom, TDL_CUSTOMATTRIBID);
			attribDef.sLabel = pTasks10->GetCustomAttributeValue(nCustom, TDL_CUSTOMATTRIBLABEL);
			attribDef.sColumnTitle = pTasks10->GetCustomAttributeValue(nCustom, TDL_CUSTOMATTRIBCOLTITLE);
			attribDef.nTextAlignment = _ttoi(pTasks10->GetCustomAttributeValue(nCustom, TDL_CUSTOMATTRIBCOLALIGN));
			attribDef.dwFeatures = _ttoi(pTasks10->GetCustomAttributeValue(nCustom, TDL_CUSTOMATTRIBFEATURES));

			// list data can contain default-data and/or auto-data
			if (attribDef.IsList())
				attribDef.DecodeListData(pTasks10->GetCustomAttributeValue(nCustom, TDL_CUSTOMATTRIBLISTDATA));
			
			aAttribs.Add(attribDef);
		}
	}
	
	return aAttribs.GetSize();
}

BOOL CTaskFile::HasCustomAttribute(const CString& sCustID) const
{
	return (GetCustomAttributeDef(sCustID) != NULL);
}

CXmlItem* CTaskFile::GetCustomAttributeDef(const CString& sCustID)
{
	CXmlItem* pXIAttribDef = GetItem(TDL_CUSTOMATTRIBDEFS);
	
	while (pXIAttribDef)
	{
		if (sCustID.CompareNoCase(pXIAttribDef->GetItemValue(TDL_CUSTOMATTRIBID)) == 0)
			return pXIAttribDef;
		
		// next sibling
		pXIAttribDef = pXIAttribDef->GetSibling();
	}
	
	// not found 
	return NULL;
}

const CXmlItem* CTaskFile::GetCustomAttributeDef(const CString& sCustID) const
{
	return const_cast<CTaskFile*>(this)->GetCustomAttributeDef(sCustID);
// 	const CXmlItem* pXIAttribDef = GetItem(TDL_CUSTOMATTRIBDEFS);
// 	
// 	while (pXIAttribDef)
// 	{
// 		if (sCustID == pXIAttribDef->GetItemValue(TDL_CUSTOMATTRIBID))
// 			return pXIAttribDef;
// 		
// 		// next sibling
// 		pXIAttribDef = pXIAttribDef->GetSibling();
// 	}
// 	
// 	// not found 
// 	return NULL;
}

// native version
int CTaskFile::GetCustomAttributeDefs(CTDCCustomAttribDefinitionArray& aAttribs) const
{
	aAttribs.RemoveAll();

	const CXmlItem* pXIAttribDef = GetCustomAttribDefs();

	while (pXIAttribDef)
	{
		TDCCUSTOMATTRIBUTEDEFINITION attribDef;

		attribDef.SetAttributeType(pXIAttribDef->GetItemValueI(TDL_CUSTOMATTRIBTYPE));
		attribDef.sUniqueID = pXIAttribDef->GetItemValue(TDL_CUSTOMATTRIBID);
		attribDef.sLabel = pXIAttribDef->GetItemValue(TDL_CUSTOMATTRIBLABEL);
		attribDef.sColumnTitle = pXIAttribDef->GetItemValue(TDL_CUSTOMATTRIBCOLTITLE);
		attribDef.nTextAlignment = pXIAttribDef->GetItemValueI(TDL_CUSTOMATTRIBCOLALIGN);
		attribDef.bEnabled = pXIAttribDef->GetItemValueI(TDL_CUSTOMATTRIBENABLED);
		attribDef.dwFeatures = pXIAttribDef->GetItemValueI(TDL_CUSTOMATTRIBFEATURES);

		// list data can contain default-data and/or auto-data
		if (attribDef.IsList())
			attribDef.DecodeListData(pXIAttribDef->GetItemValue(TDL_CUSTOMATTRIBLISTDATA));

		// good to go
		aAttribs.Add(attribDef);

		// next sibling
		pXIAttribDef = pXIAttribDef->GetSibling();
	}

	return aAttribs.GetSize();
}

const CXmlItem* CTaskFile::GetCustomAttribDefs(int nIndex) const
{
	const CXmlItem* pXIAttribDef = GetItem(TDL_CUSTOMATTRIBDEFS, nIndex);

	// backwards compatibility
	if (!pXIAttribDef)
		pXIAttribDef = GetItem(TDL_CUSTOMATTRIBDEFS_DEP, nIndex);

	return pXIAttribDef;
}

BOOL CTaskFile::IsCustomDateAttribute(const CString& sTypeID) const
{
	const CXmlItem* pXIAttribDef = GetCustomAttribDefs();

	while (pXIAttribDef)
	{
		if (pXIAttribDef->GetItemValue(TDL_CUSTOMATTRIBID) == sTypeID)
		{
			DWORD dwAttribType = (DWORD)pXIAttribDef->GetItemValueI(TDL_CUSTOMATTRIBTYPE);
			return ((dwAttribType & TDCCA_DATAMASK) == TDCCA_DATE);
		}

		// next sibling
		pXIAttribDef = pXIAttribDef->GetSibling();
	}

	// not a date
	return FALSE;
}

bool CTaskFile::AddCustomAttribute(LPCTSTR szID, LPCTSTR szLabel, LPCWSTR szColumn, bool bList)
{
	return (AddCustomAttributeDef(szID, szLabel, szColumn, bList) != NULL);
}

CXmlItem* CTaskFile::AddCustomAttributeDef(LPCTSTR szID, LPCTSTR szLabel, LPCTSTR szColumn, BOOL bList)
{
	if (Misc::IsEmpty(szID) || Misc::IsEmpty(szLabel))
	{
		ASSERT(0);
		return NULL;
	}

	// check for existing item with same ID
	if (HasCustomAttribute(szID))
		return NULL;
		
	// not found so we can proceed
	CXmlItem* pXINewAttribDef = AddItem(TDL_CUSTOMATTRIBDEFS);
	
	pXINewAttribDef->SetItemValue(TDL_CUSTOMATTRIBID, szID);
	pXINewAttribDef->SetItemValue(TDL_CUSTOMATTRIBLABEL, szLabel);
	pXINewAttribDef->SetItemValue(TDL_CUSTOMATTRIBENABLED, TRUE);
	pXINewAttribDef->SetItemValue(TDL_CUSTOMATTRIBTYPE, (bList ? TDCCA_AUTOLIST : TDCCA_STRING));

	// optionals
	if (!Misc::IsEmpty(szColumn))
		pXINewAttribDef->SetItemValue(TDL_CUSTOMATTRIBCOLTITLE, szColumn);

	return pXINewAttribDef;
}

int CTaskFile::GetCustomAttributeCount() const
{
	int nCount = GetItemCount(TDL_CUSTOMATTRIBDEFS);

	// backwards compatibility
	if (!nCount)
		nCount = GetItemCount(TDL_CUSTOMATTRIBDEFS_DEP);

	return nCount;
}

LPCTSTR CTaskFile::GetCustomAttributeLabel(int nIndex) const
{
	return GetCustomAttributeValue(nIndex, TDL_CUSTOMATTRIBLABEL);
}

LPCTSTR CTaskFile::GetCustomAttributeID(int nIndex) const
{
	return GetCustomAttributeValue(nIndex, TDL_CUSTOMATTRIBID);
}

unsigned long CTaskFile::GetCustomAttributeType(int nIndex) const
{
	return _ttol(GetCustomAttributeValue(nIndex, TDL_CUSTOMATTRIBTYPE));
}

LPCTSTR CTaskFile::GetCustomAttributeListData(int nIndex) const
{
	return GetCustomAttributeValue(nIndex, TDL_CUSTOMATTRIBLISTDATA);
}

bool CTaskFile::IsCustomAttributeEnabled(int nIndex) const
{
	return (_ttoi(GetCustomAttributeValue(nIndex, TDL_CUSTOMATTRIBENABLED)) != 0);
}

LPCTSTR CTaskFile::GetCustomAttributeValue(int nIndex, LPCTSTR szItem) const
{
	ASSERT(nIndex >= 0);

	if (nIndex < 0)
		return _T("");

	const CXmlItem* pXIAttribDef = GetCustomAttribDefs(nIndex);

	if (pXIAttribDef)
		return pXIAttribDef->GetItemValue(szItem);

	// else
	ASSERT(0);
	return _T("");
}

LPCTSTR CTaskFile::GetMetaData(LPCTSTR szKey) const
{
	if (Misc::IsEmpty(szKey))
	{
		ASSERT(0);
		return _T("");
	}

	// else
	return GetItemValue(TDL_METADATA, szKey);
}

LPCTSTR CTaskFile::GetReportTitle() const
{
	return GetItemValue(TDL_REPORTTITLE);
}

LPCTSTR CTaskFile::GetHtmlCharSet() const
{
	return GetItemValue(TDL_CHARSET);
}

LPCTSTR CTaskFile::GetAttribute(LPCTSTR szAttrib) const
{
	return GetItemValue(szAttrib);
}

int CTaskFile::GetAttributeCount(LPCTSTR szAttrib) const
{
	return GetItemCount(szAttrib);
}

LPCTSTR CTaskFile::GetAttribute(LPCTSTR szAttrib, int nIndex) const
{
	const CXmlItem* pXI = GetItem(szAttrib, nIndex);

	if (pXI)
		return pXI->GetValue();

	// else
	return EMPTY_STR;
}

LPCTSTR CTaskFile::GetReportDate() const
{
	return GetItemValue(TDL_REPORTDATE);
}

LPCTSTR CTaskFile::GetCheckOutTo() const
{
	return GetItemValue(TDL_CHECKEDOUTTO);
}	

unsigned long CTaskFile::GetFileFormat() const
{
	return GetItemValueI(TDL_FILEFORMAT);
}

unsigned long CTaskFile::GetFileVersion() const
{
	return GetItemValueI(TDL_FILEVERSION);
}

BOOL CTaskFile::SetEarliestDueDate(const COleDateTime& date)
{
	return (NULL != SetItemValue(TDL_EARLIESTDUEDATE, date));
}

BOOL CTaskFile::GetEarliestDueDate(COleDateTime& date) const
{
	const CXmlItem* pXItem = GetItem(TDL_EARLIESTDUEDATE);

	if (!pXItem)
		return FALSE;

	date = pXItem->GetValueF();
	return TRUE;
}

CString CTaskFile::GetCommentsType() const
{
	return GetItemValue(TDL_COMMENTSTYPE);
}

bool CTaskFile::SetProjectName(LPCTSTR szName)
{
	return (NULL != SetItemValue(TDL_PROJECTNAME, szName));
}

bool CTaskFile::SetMetaData(LPCTSTR szKey, LPCTSTR szMetaData)
{
	if (Misc::IsEmpty(szMetaData))
	{
		return ClearMetaData(szKey);
	}
	else if (Misc::IsEmpty(szKey))
	{
		ASSERT(0);
		return false;
	}

	// else
	CXmlItem* pXItem = GetAddItem(TDL_METADATA);
	ASSERT(pXItem);

	return (NULL != pXItem->SetItemValue(szKey, szMetaData));
}

bool CTaskFile::SetFileVersion(unsigned long nVersion)
{
	return (NULL != SetItemValue(TDL_FILEVERSION, (int)nVersion));
}

BOOL CTaskFile::SetCheckedOutTo(const CString& sCheckedOutTo)
{
	return (NULL != SetItemValue(TDL_CHECKEDOUTTO, sCheckedOutTo));
}

bool CTaskFile::ClearMetaData(LPCTSTR szKey)
{
	if (Misc::IsEmpty(szKey))
	{
		ASSERT(0);
		return false;
	}

	return (FALSE != DeleteItem(TDL_METADATA, szKey));
}

BOOL CTaskFile::SetArchive(BOOL bArchive)
{
	if (bArchive)
		return (NULL != SetItemValue(TDL_ARCHIVE, (int)1));
	
	// else
	DeleteItem(TDL_ARCHIVE);
	return TRUE;
}

BOOL CTaskFile::SetFileName(LPCTSTR szFilename)
{
	if (!Misc::IsEmpty(szFilename))
		return (NULL != SetItemValue(TDL_FILENAME, szFilename));

	// else
	DeleteItem(TDL_FILENAME);
	return TRUE;
}

BOOL CTaskFile::SetLastModified(const COleDateTime& tLastMod)
{
	if (SetItemValue(TDL_LASTMOD, tLastMod))
	{
		DWORD dwFmt = (DHFD_TIME | DHFD_NOSEC | (m_bISODates ? DHFD_ISO : 0));
		return (NULL != SetItemValue(TDL_LASTMODSTRING, CDateHelper::FormatDate(tLastMod, dwFmt)));
	}

	// else
	return FALSE;
}

COleDateTime CTaskFile::GetLastModifiedOle() const
{
	return GetItemValueF(TDL_LASTMOD);
}

BOOL CTaskFile::SetAutoListData(const TDCAUTOLISTDATA& tld)
{
	return (SetArray(TDL_TASKCATEGORY, tld.aCategory) &&
			SetArray(TDL_TASKTAG, tld.aTags) &&
			SetArray(TDL_TASKSTATUS, tld.aStatus) &&
			SetArray(TDL_TASKALLOCTO, tld.aAllocTo) &&
			SetArray(TDL_TASKALLOCBY, tld.aAllocBy) &&
			SetArray(TDL_TASKVERSION, tld.aVersion));
}

int CTaskFile::GetAutoListData(TDCAUTOLISTDATA& tld) const
{
	return (GetArray(TDL_TASKCATEGORY, tld.aCategory) +
			GetArray(TDL_TASKTAG, tld.aTags) +
			GetArray(TDL_TASKSTATUS, tld.aStatus) +
			GetArray(TDL_TASKALLOCTO, tld.aAllocTo) +
			GetArray(TDL_TASKALLOCBY, tld.aAllocBy) +
			GetArray(TDL_TASKVERSION, tld.aVersion));
}

BOOL CTaskFile::SetFileFormat(unsigned long lFormat)
{
	return (NULL != SetItemValue(TDL_FILEFORMAT, (int)lFormat));
}

BOOL CTaskFile::SetNextUniqueID(DWORD dwNextID)
{
	ASSERT (dwNextID >= m_dwNextUniqueID);

	BOOL bResult = FALSE;
	CXmlItem* pXItem = GetItem(TDL_NEXTUNIQUEID);

	if (pXItem)
	{
		pXItem->SetValue((int)dwNextID);
		bResult = TRUE;
	}
	else
		bResult = (NULL != AddItem(TDL_NEXTUNIQUEID, (int)dwNextID));

	if (bResult)
		m_dwNextUniqueID = dwNextID;

	return bResult;
}

BOOL CTaskFile::SetReportAttributes(LPCTSTR szTitle, const COleDateTime& date)
{
	BOOL bRes = TRUE;

	CXmlItem* pXItem = GetItem(TDL_REPORTTITLE);

	if (pXItem)
		pXItem->SetValue(szTitle);
	else
		bRes &= (NULL != AddItem(TDL_REPORTTITLE, szTitle));

	if (CDateHelper::IsDateSet(date))
	{
		// date value
		pXItem = GetItem(TDL_REPORTDATEOLE);

		if (pXItem)
			pXItem->SetValue(date);
		else
			bRes &= (NULL != AddItem(TDL_REPORTDATE, date));

		// date string
		CString sDate = date.Format(VAR_DATEVALUEONLY);
		pXItem = GetItem(TDL_REPORTDATE);

		if (pXItem)
			pXItem->SetValue(sDate);
		else
			bRes &= (NULL != AddItem(TDL_REPORTDATE, sDate));
	}

	return bRes;
}

BOOL CTaskFile::SetTaskAttributes(HTASKITEM hTask, const TODOITEM* pTDI)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, FALSE);

	SetTaskReferenceID(hTask, pTDI->dwTaskRefID);

	// set string items via lower-level call to prevent string copying
	SetTaskString(hTask, TDL_TASKTITLE, pTDI->sTitle);
	
	// Save attributes _only_ if this is a 'real' task
	if (pTDI->dwTaskRefID == 0)
	{
		if (!pTDI->sComments.IsEmpty())
			SetTaskString(hTask, TDL_TASKCOMMENTS, pTDI->sComments, XIT_ELEMENT);

		SetTaskCustomComments(hTask, pTDI->customComments, pTDI->sCommentsTypeID);

		if (!pTDI->sAllocBy.IsEmpty())
			SetTaskString(hTask, TDL_TASKALLOCBY, pTDI->sAllocBy);
		
		if (!pTDI->sStatus.IsEmpty())
			SetTaskString(hTask, TDL_TASKSTATUS, pTDI->sStatus);
		
		if (!pTDI->sVersion.IsEmpty())
			SetTaskString(hTask, TDL_TASKVERSION, pTDI->sVersion);
		
		if (!pTDI->sCreatedBy.IsEmpty())
			SetTaskString(hTask, TDL_TASKCREATEDBY, pTDI->sCreatedBy);
		
		if (!pTDI->sExternalID.IsEmpty())
			SetTaskString(hTask, TDL_TASKEXTERNALID, pTDI->sExternalID);
		
		if (!pTDI->sIcon.IsEmpty())
			SetTaskIcon(hTask, pTDI->sIcon);
		
		// rest of non-string attributes
		SetTaskPriority(hTask, pTDI->nPriority);
		SetTaskRisk(hTask, pTDI->nRisk);
		
		if (pTDI->bFlagged)
			SetTaskFlag(hTask, pTDI->bFlagged != FALSE);
		
		if (pTDI->IsRecurring())
			SetTaskRecurrence(hTask, pTDI->trRecurrence);
		
		if (pTDI->aAllocTo.GetSize())
			SetTaskAllocatedTo(hTask, pTDI->aAllocTo);
		
		if (pTDI->aCategories.GetSize())
			SetTaskCategories(hTask, pTDI->aCategories);
		
		if (pTDI->aTags.GetSize())
			SetTaskTags(hTask, pTDI->aTags);
		
		if (pTDI->aDependencies.GetSize())
			SetTaskDependencies(hTask, pTDI->aDependencies);
			
		if (pTDI->aFileLinks.GetSize())
			SetTaskFileLinks(hTask, pTDI->aFileLinks);
		
		if (pTDI->dCost != 0)
			SetTaskCost(hTask, pTDI->dCost);
		
		if ((pTDI->dTimeEstimate > 0) || (pTDI->nTimeEstUnits != TDCU_HOURS))
			SetTaskTimeEstimate(hTask, pTDI->dTimeEstimate, pTDI->nTimeEstUnits);
		
		if ((pTDI->dTimeSpent != 0.0) || (pTDI->nTimeSpentUnits != TDCU_HOURS))
			SetTaskTimeSpent(hTask, pTDI->dTimeSpent, pTDI->nTimeSpentUnits);
		
		// done date and percent
		if (pTDI->IsDone())
		{
			SetTaskDoneDate(hTask, pTDI->dateDone);
			SetTaskPercentDone(hTask, 100);
		}
		//////////////////////////////////////////////////////////////////////////
		// SAVE PERCENT DONE REGARDLESS OF VALUE, ELSE GANTTVIEWER SPITS THE DUMMY
		else// if (pTDI->nPercentDone > 0)
		{
			SetTaskPercentDone(hTask, (unsigned char)min(99, pTDI->nPercentDone));
		}
		//////////////////////////////////////////////////////////////////////////

		if (pTDI->HasDue())
			SetTaskDueDate(hTask, pTDI->dateDue);
		
		if (pTDI->HasStart())
			SetTaskStartDate(hTask, pTDI->dateStart);
		
		if (pTDI->HasCreation())
			SetTaskCreationDate(hTask, pTDI->dateCreated);
		
		if (pTDI->HasLastMod())
			SetTaskLastModified(hTask, pTDI->dateLastMod);
		
		if (pTDI->color)
			SetTaskColor(hTask, pTDI->color);

		// meta data
		SetTaskMetaData(hTask, pTDI->mapMetaData);

		// custom data
		SetTaskCustomAttributeData(hTask, pTDI->mapCustomData);
	}

	return TRUE;
}

BOOL CTaskFile::GetTaskAttributes(HTASKITEM hTask, TODOITEM* pTDI) const
{
	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, FALSE);

	pTDI->dwTaskRefID = GetTaskReferenceID(hTask);

	// Load attributes _only_ if this is a 'real' task
	if (pTDI->dwTaskRefID == 0)
	{
		// Call GetTaskString directly wherever possible to avoid string copying
		pTDI->dateLastMod = GetTaskLastModifiedOle(hTask);
		pTDI->sTitle = GetTaskString(hTask, TDL_TASKTITLE);
		pTDI->sComments = GetTaskString(hTask, TDL_TASKCOMMENTS);
		pTDI->sAllocBy = GetTaskString(hTask, TDL_TASKALLOCBY);
		pTDI->sStatus = GetTaskString(hTask, TDL_TASKSTATUS);
		pTDI->sCreatedBy = GetTaskString(hTask, TDL_TASKCREATEDBY);
		pTDI->bFlagged = IsTaskFlagged(hTask);
		pTDI->color = (COLORREF)GetTaskColor(hTask);
		pTDI->nPercentDone = (int)GetTaskPercentDone(hTask, FALSE);
		pTDI->dTimeEstimate = GetTaskTimeEstimate(hTask, pTDI->nTimeEstUnits, FALSE);
		pTDI->dTimeSpent = GetTaskTimeSpent(hTask, pTDI->nTimeSpentUnits, FALSE);
		pTDI->nPriority = (int)GetTaskPriority(hTask, FALSE);
		pTDI->dateDue = GetTaskDueDateOle(hTask);
		pTDI->dateStart = GetTaskStartDateOle(hTask);
		pTDI->dateDone = GetTaskDoneDateOle(hTask);
		pTDI->dateCreated = GetTaskCreationDateOle(hTask);
		pTDI->nRisk = GetTaskRisk(hTask, FALSE);
		pTDI->sExternalID = GetTaskString(hTask, TDL_TASKEXTERNALID);
		pTDI->dCost = GetTaskCost(hTask, FALSE);
		pTDI->sVersion = GetTaskVersion(hTask);
		pTDI->sIcon = GetTaskIcon(hTask);

		GetTaskCategories(hTask, pTDI->aCategories);
		GetTaskTags(hTask, pTDI->aTags);
		GetTaskAllocatedTo(hTask, pTDI->aAllocTo);
		GetTaskRecurrence(hTask, pTDI->trRecurrence);
		GetTaskDependencies(hTask, pTDI->aDependencies);
		GetTaskFileLinks(hTask, pTDI->aFileLinks);
		GetTaskCustomComments(hTask, pTDI->customComments, pTDI->sCommentsTypeID);

		// meta data
		GetTaskMetaData(hTask, pTDI->mapMetaData);

		// custom data
		GetTaskCustomAttributeData(hTask, pTDI->mapCustomData);
	}

	return TRUE;
}

BOOL CTaskFile::DeleteTaskAttributes(HTASKITEM hTask)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, FALSE);

	POSITION pos = pXITask->GetFirstItemPos();

	while (pos)
	{
		CXmlItem* pXIChild = pXITask->GetNextItem(pos);

		if (!pXIChild->NameMatches(TDL_TASK))
			pXITask->DeleteItem(pXIChild);
	}

	return TRUE;
}

unsigned long CTaskFile::GetTaskParentID(HTASKITEM hTask) const
{
	HTASKITEM hParent = GetTaskParent(hTask);
	unsigned long dwParentID = GetTaskID(hParent);

	// else try backup 
	if (dwParentID == 0)
		dwParentID = GetTaskULong(hTask, TDL_TASKPARENTID);

	return dwParentID;
}

HTASKITEM CTaskFile::FindTask(unsigned long dwTaskID) const
{
	if (dwTaskID <= 0)
		return NULL;

	// find taskID attribute
	const CXmlItem* pXI = FindItem(TDL_TASKID, (int)dwTaskID);

	// then take it's parent as the task itself
	return pXI ? (HTASKITEM)(pXI->GetParent()) : NULL;
}

bool CTaskFile::DeleteTask(HTASKITEM hTask)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, FALSE);

	CXmlItem* pXIParent = pXITask->GetParent();
	ASSERT (pXIParent);

	// remove task from map first
	RemoveTaskFromMap(pXITask);

	return (pXIParent->DeleteItem(pXITask) != FALSE);
}

BOOL CTaskFile::SetTaskCustomComments(HTASKITEM hTask, const CBinaryData& content, const CString& sType)
{
	if (!TaskFromHandle(hTask))
		return FALSE;

	BOOL bRes = TRUE;

	if (!content.IsEmpty())
	{
		Base64Coder b64;
		b64.Encode((const PBYTE)content.Get(), content.GetLength());

 		// Base64Coder will convert ansi to unicode as required
		bRes = SetTaskString(hTask, TDL_TASKCUSTOMCOMMENTS, b64.EncodedMessage(), XIT_ELEMENT);
	}

	if (bRes)
		bRes = SetTaskString(hTask, TDL_TASKCOMMENTSTYPE, sType);

	return bRes;
}

BOOL CTaskFile::SetTaskHtmlComments(HTASKITEM hTask, const CString& sContent, BOOL bForTransform)
{
	if (!TaskFromHandle(hTask))
		return FALSE;

	XI_TYPE nType = bForTransform ? XIT_ELEMENT : XIT_CDATA;

	return SetTaskString(hTask, TDL_TASKHTMLCOMMENTS, sContent, nType);
}

BOOL CTaskFile::GetTaskCustomComments(HTASKITEM hTask, CBinaryData& content, CString& sType) const
{
	// comments type
	sType = GetTaskString(hTask, TDL_TASKCOMMENTSTYPE);

	// custom comments
	CString sTemp = GetTaskString(hTask, TDL_TASKCUSTOMCOMMENTS);

	if (sTemp.IsEmpty())
		return FALSE;

	Base64Coder b64;

#ifdef _UNICODE

	// if text is unicode then we need to convert it back to multibyte
	// to read the binary stream as unsigned chars
	int nLen = sTemp.GetLength();
	unsigned char* pBinary = (unsigned char*)Misc::WideToMultiByte((LPCTSTR)sTemp, nLen);
	b64.Decode(pBinary, nLen);
	delete [] pBinary;

#else

	b64.Decode(sTemp);

#endif


	unsigned long nLenContent;
	PBYTE pContent = b64.DecodedMessage(nLenContent);

	PBYTE szContent = (PBYTE)content.GetBuffer(nLenContent);
	CopyMemory(szContent, pContent, nLenContent);
	content.ReleaseBuffer(nLenContent);
	
	return (nLenContent > 0);
}

BOOL CTaskFile::SetTaskCategories(HTASKITEM hTask, const CStringArray& aCategories)
{
	return SetTaskArray(hTask, TDL_TASKCATEGORY, aCategories);
}

BOOL CTaskFile::SetTaskTags(HTASKITEM hTask, const CStringArray& aTags)
{
	return SetTaskArray(hTask, TDL_TASKTAG, aTags);
}

BOOL CTaskFile::SetTaskFileLinks(HTASKITEM hTask, const CStringArray& aFiles)
{
	return SetTaskArray(hTask, TDL_TASKFILEREFPATH, aFiles);
}

BOOL CTaskFile::SetTaskDependencies(HTASKITEM hTask, const CStringArray& aDepends)
{
	return SetTaskArray(hTask, TDL_TASKDEPENDENCY, aDepends);
}

BOOL CTaskFile::SetTaskMetaData(HTASKITEM hTask, const CMapStringToString& mapMetaData)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, FALSE);

	return SetMetaData(pXITask, mapMetaData);
}

int CTaskFile::GetTaskMetaData(HTASKITEM hTask, CMapStringToString& mapMetaData) const
{
	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, 0);

	return GetMetaData(pXITask, mapMetaData);
}

int CTaskFile::GetTaskCustomAttributeData(HTASKITEM hTask, CMapStringToString& mapData) const
{
	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, 0);

	mapData.RemoveAll();

	const CXmlItem* pXICustData = pXITask->GetItem(TDL_TASKCUSTOMATTRIBDATA);

	while (pXICustData)
	{
		CString sTypeID = pXICustData->GetItemValue(TDL_TASKCUSTOMATTRIBID);
		CString sData = pXICustData->GetItemValue(TDL_TASKCUSTOMATTRIBVALUE);

		if (!sTypeID.IsEmpty() && !sData.IsEmpty())
		{
			mapData[Misc::MakeUpper(sTypeID)] = sData;
		}

		pXICustData = pXICustData->GetSibling();
	}

	return mapData.GetCount();
}

BOOL CTaskFile::SetTaskCustomAttributeData(HTASKITEM hTask, const CMapStringToString& mapData)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, FALSE);

	pXITask->DeleteItem(TDL_TASKCUSTOMATTRIBDATA);

	POSITION pos = mapData.GetStartPosition();

	while (pos)
	{
		CString sTypeID, sData;
		mapData.GetNextAssoc(pos, sTypeID, sData);

		if (sTypeID.IsEmpty() || sData.IsEmpty())
			continue;

		if (!HasCustomAttribute(sTypeID))
			continue;

		CXmlItem* pXICustData = pXITask->AddItem(TDL_TASKCUSTOMATTRIBDATA);
		ASSERT(pXICustData);

		pXICustData->AddItem(TDL_TASKCUSTOMATTRIBID, Misc::MakeUpper(sTypeID));
		pXICustData->AddItem(TDL_TASKCUSTOMATTRIBVALUE, sData);

		// add human readable date format
		if (IsCustomDateAttribute(sTypeID))
		{
			COleDateTime date(_ttof(sData));
			DWORD dwFmt = m_bISODates ? DHFD_ISO : 0;

			pXICustData->AddItem(TDL_TASKCUSTOMATTRIBDATESTRING, CDateHelper::FormatDate(date, dwFmt));
		}
	}

	return TRUE;
}

BOOL CTaskFile::SetTaskAllocatedTo(HTASKITEM hTask, const CStringArray& aAllocTo)
{
	return SetTaskArray(hTask, TDL_TASKALLOCTO, aAllocTo);
}

bool CTaskFile::AddTaskDependency(HTASKITEM hTask, LPCTSTR szDepends)
{
	return AddTaskArrayItem(hTask, TDL_TASKDEPENDENCY, szDepends);
}

bool CTaskFile::AddTaskDependency(HTASKITEM hTask, unsigned long dwID)
{
	CString sID;
	sID.Format(_T("%lu"), dwID);

	return AddTaskDependency(hTask, sID);
}

bool CTaskFile::AddTaskAllocatedTo(HTASKITEM hTask, LPCTSTR szAllocTo)
{
	return AddTaskArrayItem(hTask, TDL_TASKALLOCTO, szAllocTo);
}

bool CTaskFile::AddTaskFileLink(HTASKITEM hTask, LPCTSTR szFileRef)
{
	return AddTaskArrayItem(hTask, TDL_TASKFILEREFPATH, szFileRef);
}

bool CTaskFile::AddTaskCategory(HTASKITEM hTask, LPCTSTR szCategory)
{
	return AddTaskArrayItem(hTask, TDL_TASKCATEGORY, szCategory);
}

bool CTaskFile::AddTaskTag(HTASKITEM hTask, LPCTSTR szTag)
{
	return AddTaskArrayItem(hTask, TDL_TASKTAG, szTag);
}

int CTaskFile::GetTaskCategories(HTASKITEM hTask, CStringArray& aCategories) const
{
	return GetTaskArray(hTask, TDL_TASKCATEGORY, aCategories);
}

int CTaskFile::GetTaskTags(HTASKITEM hTask, CStringArray& aTags) const
{
	return GetTaskArray(hTask, TDL_TASKTAG, aTags);
}

int CTaskFile::GetTaskFileLinks(HTASKITEM hTask, CStringArray& aFiles) const
{
	return GetTaskArray(hTask, TDL_TASKFILEREFPATH, aFiles);
}

int CTaskFile::GetTaskDependencies(HTASKITEM hTask, CStringArray& aDepends) const
{
	return GetTaskArray(hTask, TDL_TASKDEPENDENCY, aDepends);
}

int CTaskFile::GetTaskAllocatedTo(HTASKITEM hTask, CStringArray& aAllocTo) const
{
	return GetTaskArray(hTask, TDL_TASKALLOCTO, aAllocTo);
}

CXmlItem* CTaskFile::NewItem(const CString& sName)
{
	return new CXmlItem(NULL, sName);
}

///////////////////////////////////////////////////////////////////////

HTASKITEM CTaskFile::NewTask(LPCTSTR szTitle, HTASKITEM hParent)
{
	return NewTask(szTitle, hParent, 0); // 0 means 'allocate new ID'
}

HTASKITEM CTaskFile::NewTask(LPCTSTR szTitle, HTASKITEM hParent, DWORD dwID)
{
	ASSERT((dwID == 0) || (FindTask(dwID) == 0));

	CXmlItem* pXIParent = hParent ? TaskFromHandle(hParent) : Root();

	if (!pXIParent)
	{
		// handle the fact that hParent might already point to Root()
		if (hParent == (HTASKITEM)Root())
			pXIParent = Root();
		else
			return NULL;
	}

	CXmlItem* pXINew = NewItem(TDL_TASK);

	if (pXINew)
	{
		pXIParent->AddItem(pXINew);
		AddTaskToMap(pXINew, FALSE, FALSE);

		// set ID and name
		SetTaskTitle((HTASKITEM)pXINew, szTitle);

		if (dwID <= 0)
			dwID = m_dwNextUniqueID++;
		else
			m_dwNextUniqueID = max(m_dwNextUniqueID, dwID + 1);

		SetTaskID((HTASKITEM)pXINew, dwID);
	}

	return (HTASKITEM)pXINew;
}

// new wrapper that lets us set the parent ID if the parent task is NULL
HTASKITEM CTaskFile::NewTask(LPCTSTR szTitle, HTASKITEM hParent, DWORD dwID, DWORD dwParentID)
{
	HTASKITEM hNewTask = NewTask(szTitle, hParent, dwID);
	
	// set parent ID
	if (hNewTask && dwParentID)
	{
		// sanity check
		ASSERT((hParent == NULL) || (GetTaskID(hParent) == dwParentID));
		
		if (hParent == NULL)
			SetTaskULong(hNewTask, TDL_TASKPARENTID, dwParentID);
	}
	
	return hNewTask;
}

///////////////////////////////////////////////////////////////////////

int CTaskFile::GetTaskCount() const 
{ 
	if ((m_mapHandles.GetCount() == 0) && (Root()->HasItem(TDL_TASK)))
		BuildHandleMap();

	return m_mapHandles.GetCount(); 
}

int CTaskFile::GetTopLevelTaskCount() const
{
	const CXmlItem* pXI = Root()->GetItem(TDL_TASK);
	int nCount = 0;

	if (pXI)
		nCount = (pXI->GetSiblingCount() + 1);

	return nCount;
}

HTASKITEM CTaskFile::GetFirstTask(HTASKITEM hParent) const
{
	const CXmlItem* pXIParent = hParent ? TaskFromHandle(hParent) : Root();

	if (!pXIParent)
	{
		// handle the fact that hParent might already point to Root()
		if (hParent == (HTASKITEM)Root())
			pXIParent = Root();
		else
			return NULL;
	}

	return (HTASKITEM)pXIParent->GetItem(TDL_TASK);
}

HTASKITEM CTaskFile::GetNextTask(HTASKITEM hTask) const
{
	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, NULL);

	return (HTASKITEM)pXITask->GetSibling();
}

LPCTSTR CTaskFile::GetTaskTitle(HTASKITEM hTask) const
{
	return GetTaskString(hTask, TDL_TASKTITLE);
}

LPCTSTR CTaskFile::GetTaskIcon(HTASKITEM hTask) const
{
	return GetTaskString(hTask, TDL_TASKICONINDEX);
}

LPCTSTR CTaskFile::GetTaskSubtaskCompletion(HTASKITEM hTask) const
{
	return GetTaskString(hTask, TDL_TASKSUBTASKDONE);
}

LPCTSTR CTaskFile::GetTaskComments(HTASKITEM hTask) const
{
	return GetTaskString(hTask, TDL_TASKCOMMENTS);
}

LPCTSTR CTaskFile::GetTaskAllocatedTo(HTASKITEM hTask) const
{
	return GetTaskString(hTask, TDL_TASKALLOCTO);
}

LPCTSTR CTaskFile::GetTaskMetaData(HTASKITEM hTask, LPCTSTR szKey) const
{
	if (Misc::IsEmpty(szKey))
	{
		ASSERT(0);
		return _T("");
	}

	return GetTaskAttribute(hTask, TDL_TASKMETADATA, szKey);
}

const CXmlItem* CTaskFile::GetTaskCustomAttribute(HTASKITEM hTask, LPCTSTR szID) const
{
	if (Misc::IsEmpty(szID))
	{
		ASSERT(0);
		return NULL;
	}

	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, NULL);

	// find the item having name == szID
	CXmlItem* pXICustData = pXITask->GetItem(TDL_TASKCUSTOMATTRIBDATA);

	while (pXICustData)
	{
		if (pXICustData->GetItemValue(TDL_TASKCUSTOMATTRIBID).CompareNoCase(szID) == 0)
			return pXICustData;

		pXICustData = pXICustData->GetSibling();
	}

	// no match
	return NULL;
}

LPCTSTR CTaskFile::GetTaskCustomAttributeData(HTASKITEM hTask, LPCTSTR szID) const
{
	const CXmlItem* pXICustData = GetTaskCustomAttribute(hTask, szID);

	if (pXICustData)
		return pXICustData->GetItemValue(TDL_TASKCUSTOMATTRIBVALUE);

	// no match
	return NULLSTRING;
}

LPCTSTR CTaskFile::GetTaskCustomDateString(HTASKITEM hTask, LPCTSTR szID) const
{
	const CXmlItem* pXICustData = GetTaskCustomAttribute(hTask, szID);

	if (pXICustData)
		return pXICustData->GetItemValue(TDL_TASKCUSTOMATTRIBDATESTRING);

	// no match
	return NULLSTRING;
}

LPCTSTR CTaskFile::GetTaskAllocatedBy(HTASKITEM hTask) const
{
	return GetTaskString(hTask, TDL_TASKALLOCBY);
}

LPCTSTR CTaskFile::GetTaskCategory(HTASKITEM hTask) const
{
	return GetTaskString(hTask, TDL_TASKCATEGORY);
}

int CTaskFile::GetTaskCategoryCount(HTASKITEM hTask) const
{
	return GetTaskArraySize(hTask, TDL_TASKCATEGORY);
}

LPCTSTR CTaskFile::GetTaskCategory(HTASKITEM hTask, int nIndex) const
{
	return GetTaskArrayItem(hTask, TDL_TASKCATEGORY, nIndex);
}

int CTaskFile::GetTaskTagCount(HTASKITEM hTask) const
{
	return GetTaskArraySize(hTask, TDL_TASKTAG);
}

LPCTSTR CTaskFile::GetTaskTag(HTASKITEM hTask, int nIndex) const
{
	return GetTaskArrayItem(hTask, TDL_TASKTAG, nIndex);
}

int CTaskFile::GetTaskDependencyCount(HTASKITEM hTask) const
{
	return GetTaskArraySize(hTask, TDL_TASKDEPENDENCY);
}

LPCTSTR CTaskFile::GetTaskDependency(HTASKITEM hTask, int nIndex) const
{
	return GetTaskArrayItem(hTask, TDL_TASKDEPENDENCY, nIndex);
}

int CTaskFile::GetTaskAllocatedToCount(HTASKITEM hTask) const
{
	return GetTaskArraySize(hTask, TDL_TASKALLOCTO);
}

LPCTSTR CTaskFile::GetTaskAllocatedTo(HTASKITEM hTask, int nIndex) const
{
	return GetTaskArrayItem(hTask, TDL_TASKALLOCTO, nIndex);
}

int CTaskFile::GetTaskFileLinkCount(HTASKITEM hTask) const
{
	return GetTaskArraySize(hTask, TDL_TASKFILEREFPATH);
}

LPCTSTR CTaskFile::GetTaskFileLink(HTASKITEM hTask, int nIndex) const
{
	return GetTaskArrayItem(hTask, TDL_TASKFILEREFPATH, nIndex);
}

bool CTaskFile::IsTaskGoodAsDone(HTASKITEM hTask) const
{
	if (GetTaskUChar(hTask, TDL_TASKGOODASDONE) != 0)
		return true;

	// else
	return IsTaskDone(hTask);
}

bool CTaskFile::IsTaskParent(HTASKITEM hTask) const
{
	if (GetTaskUChar(hTask, TDL_TASKISPARENT) != 0)
		return true;

	// else
	return (GetFirstTask(hTask) != NULL);
}

LPCTSTR CTaskFile::GetTaskDependency(HTASKITEM hTask) const
{
	return GetTaskString(hTask, TDL_TASKDEPENDENCY);
}

LPCTSTR CTaskFile::GetTaskStatus(HTASKITEM hTask) const
{
	return GetTaskString(hTask, TDL_TASKSTATUS);
}

LPCTSTR CTaskFile::GetTaskFileLinkPath(HTASKITEM hTask) const
{
	return GetTaskString(hTask, TDL_TASKFILEREFPATH);
}

LPCTSTR CTaskFile::GetTaskCreatedBy(HTASKITEM hTask) const
{
	return GetTaskString(hTask, TDL_TASKCREATEDBY);
}

unsigned long CTaskFile::GetTaskID(HTASKITEM hTask) const
{
	return GetTaskULong(hTask, TDL_TASKID);
}

bool CTaskFile::IsTaskFlagged(HTASKITEM hTask) const
{
	return (GetTaskUChar(hTask, TDL_TASKFLAG) > 0);
}

unsigned long CTaskFile::GetTaskColor(HTASKITEM hTask) const
{
	return GetTaskULong(hTask, TDL_TASKCOLOR);
}

LPCTSTR CTaskFile::GetTaskWebColor(HTASKITEM hTask) const
{
	DWORD color = 0;

	if (TaskHasAttribute(hTask, TDL_TASKTEXTCOLOR))
		color = GetTaskULong(hTask, TDL_TASKTEXTCOLOR);
	else
		color = GetTaskULong(hTask, TDL_TASKCOLOR);
	
	return GraphicsMisc::GetWebColor((COLORREF)color);
}

LPCTSTR CTaskFile::GetTaskPriorityWebColor(HTASKITEM hTask) const
{
	static CString sColor;
	sColor = GraphicsMisc::GetWebColor(GetTaskPriorityColor(hTask));

	return sColor;
}

unsigned long CTaskFile::GetTaskPriorityColor(HTASKITEM hTask) const
{
	DWORD color = RGB(255, 255, 255);

	if (TaskHasAttribute(hTask, TDL_TASKPRIORITYCOLOR))
		color = GetTaskULong(hTask, TDL_TASKPRIORITYCOLOR);

	return color;
}

int CTaskFile::GetTaskPriority(HTASKITEM hTask, BOOL bHighest) const
{
	if (bHighest && TaskHasAttribute(hTask, TDL_TASKHIGHESTPRIORITY))
		return GetTaskInt(hTask, TDL_TASKHIGHESTPRIORITY);

	return GetTaskInt(hTask, TDL_TASKPRIORITY);
}

unsigned char CTaskFile::GetTaskPercentDone(HTASKITEM hTask, BOOL bCalc) const
{
	if (bCalc && TaskHasAttribute(hTask, TDL_TASKCALCCOMPLETION))
		return GetTaskUChar(hTask, TDL_TASKCALCCOMPLETION);

	return GetTaskUChar(hTask, TDL_TASKPERCENTDONE);
}

double CTaskFile::GetTaskCost(HTASKITEM hTask, BOOL bCalc) const
{
	if (bCalc && TaskHasAttribute(hTask, TDL_TASKCALCCOST))
		return GetTaskDouble(hTask, TDL_TASKCALCCOST);

	// else
	return GetTaskDouble(hTask, TDL_TASKCOST);
}

BOOL CTaskFile::GetTaskRecurrence(HTASKITEM hTask, TDCRECURRENCE& tr) const
{
	// NOTE: we don't restore 'tr.sRegularity' because this was always
	// just a handy human-readable string for display purposes
	TDI_REGULARITY nRegularity;
	DWORD dwSpecific1;
	DWORD dwSpecific2;
	TDI_RECURFROMOPTION nRecalcFrom; 
	TDI_RECURREUSEOPTION nReuse;
	int nNumOccur;
	int nRemainingOccur;

	if (!GetTaskRecurrence(hTask, 
							(int&)nRegularity, 
							dwSpecific1, 
							dwSpecific2, 
							(int&)nRecalcFrom, 
							(int&)nReuse,
							nNumOccur,
							nRemainingOccur))
	{
		return FALSE;
	}

	if (!tr.SetRegularity(nRegularity, dwSpecific1, dwSpecific2))
		return FALSE;

	if (!tr.SetOccurrenceCount(nNumOccur, nRemainingOccur))
		return FALSE;

	tr.nRecalcFrom = nRecalcFrom;
	tr.nReuse = nReuse;

	return TRUE;
}

// // Internal
bool CTaskFile::GetTaskRecurrence(HTASKITEM hTask, int& nRegularity, DWORD& dwSpecific1, 
								  DWORD& dwSpecific2, int& nRecalcFrom, 
								  int& nReuse, int& nNumOccur, int& nRemainingOccur) const
{
	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);
	
	const CXmlItem* pXIRecur = pXITask->GetItem(TDL_TASKRECURRENCE);
	
	if (pXIRecur)
	{
		// backwards compatibility
		if (pXIRecur->HasItem(TDL_TASKRECURRENCEREG_DEP))
		{
			nRegularity = pXIRecur->GetItemValueI(TDL_TASKRECURRENCEREG_DEP);
			dwSpecific1 = pXIRecur->GetItemValueI(TDL_TASKRECURRENCESPEC1_DEP);
			dwSpecific2 = pXIRecur->GetItemValueI(TDL_TASKRECURRENCESPEC2_DEP);
		}
		else
		{
			nRegularity = pXIRecur->GetItemValueI(TDL_TASKRECURRENCEFREQ);
			dwSpecific1 = pXIRecur->GetItemValueI(TDL_TASKRECURRENCESPEC1);
			dwSpecific2 = pXIRecur->GetItemValueI(TDL_TASKRECURRENCESPEC2);
		}
		
		nReuse = pXIRecur->GetItemValueI(TDL_TASKRECURRENCEREUSE);
		
		if (pXIRecur->HasItem(TDL_TASKRECURRENCEFROM))
		{
			nRecalcFrom = pXIRecur->GetItemValueI(TDL_TASKRECURRENCEFROM);
		}
		else // fallback
		{
			// really a boolean
			nRecalcFrom = (pXIRecur->GetItemValueI(TDL_TASKRECURRENCEFROMDUE_DEP) ? TRUE : FALSE);
		}
		
		if (pXIRecur->HasItem(TDL_TASKRECURRENCENUM))
		{
			nNumOccur = pXIRecur->GetItemValueI(TDL_TASKRECURRENCENUM);
			
			if (pXIRecur->HasItem(TDL_TASKRECURRENCEREMAINING))
			{
				nRemainingOccur = pXIRecur->GetItemValueI(TDL_TASKRECURRENCEREMAINING);
			}
			else
			{
				nRemainingOccur = nNumOccur;
			}
		}
		else // fallback
		{
			nNumOccur = -1;
			nRemainingOccur = -1;
		}
		
		// Enforce TDIRO_DONEDATE for deprecated 'recreate after...' options
		switch (nRegularity)
		{
		case TDIR_DAY_RECREATEAFTERNDAYS_DEP:
			nRegularity = TDIR_DAY_EVERY_NDAYS;
			nRecalcFrom = TDIRO_DONEDATE;
			break;
			
		case TDIR_WEEK_RECREATEAFTERNWEEKS_DEP:
			nRegularity = TDIR_WEEK_EVERY_NWEEKS;
			nRecalcFrom = TDIRO_DONEDATE;
			break;
			
		case TDIR_MONTH_RECREATEAFTERNMONTHS_DEP:
			nRegularity = TDIR_MONTH_EVERY_NMONTHS;
			nRecalcFrom = TDIRO_DONEDATE;
			break;
			
		case TDIR_YEAR_RECREATEAFTERNYEARS_DEP:
			nRegularity = TDIR_YEAR_EVERY_NYEARS;
			nRecalcFrom = TDIRO_DONEDATE;
			break;
		}
		
		return true;
	}
	
	return false;
}

// ITaskList6 interface
bool CTaskFile::GetTaskRecurrence(HTASKITEM hTask, int& nRegularity, DWORD& dwSpecific1, 
									DWORD& dwSpecific2, BOOL& bRecalcFromDue, int& nReuse) const
{
	int nDummy1, nDummy2;

	return GetTaskRecurrence(hTask, nRegularity, dwSpecific1, dwSpecific2, 
							bRecalcFromDue, nReuse, nDummy1, nDummy2);
}

// ITaskList13 interface
bool CTaskFile::GetTaskRecurrence(HTASKITEM hTask, int& nRegularity, DWORD& dwSpecific1, 
									DWORD& dwSpecific2, int& nRecalcFrom, 
									int& nReuse, int& nNumOccur) const
{
	int nDummy;

	// Note: for backwards compatibility nNumOccur maps to the remaining occurences
	return GetTaskRecurrence(hTask, nRegularity, dwSpecific1, dwSpecific2, 
							nRecalcFrom, nReuse, nDummy, nNumOccur);
}

LPCTSTR CTaskFile::GetTaskVersion(HTASKITEM hTask) const
{
	return GetTaskString(hTask, TDL_TASKVERSION);
}

TDC_UNITS CTaskFile::GetTaskTimeUnits(HTASKITEM hTask, const CString& sUnitsItem) const
{
	TDC_UNITS nUnits = TDCU_HOURS;
	CString sUnits = GetTaskString(hTask, sUnitsItem);

	if (!sUnits.IsEmpty())
		nUnits = (TDC_UNITS)sUnits[0];

	return nUnits;
}

double CTaskFile::GetTaskTimeEstimate(HTASKITEM hTask, TDC_UNITS& nUnits, BOOL bCalc) const
{
	if (bCalc && TaskHasAttribute(hTask, TDL_TASKCALCTIMEESTIMATE))
	{
		nUnits = GetTaskTimeUnits(hTask, TDL_TASKCALCTIMEESTUNITS);
		return GetTaskDouble(hTask, TDL_TASKCALCTIMEESTIMATE);
	}
	
	// else
	nUnits = GetTaskTimeUnits(hTask, TDL_TASKTIMEESTUNITS);
	return GetTaskDouble(hTask, TDL_TASKTIMEESTIMATE);
}

double CTaskFile::GetTaskTimeSpent(HTASKITEM hTask, TDC_UNITS& nUnits, BOOL bCalc) const
{
	if (bCalc && TaskHasAttribute(hTask, TDL_TASKCALCTIMESPENT))
	{
		nUnits = GetTaskTimeUnits(hTask, TDL_TASKCALCTIMESPENTUNITS);
		return GetTaskDouble(hTask, TDL_TASKCALCTIMESPENT);
	}

	nUnits = GetTaskTimeUnits(hTask, TDL_TASKTIMESPENTUNITS);
	return GetTaskDouble(hTask, TDL_TASKTIMESPENT);
}

time_t CTaskFile::GetTaskLastModified(HTASKITEM hTask) const
{
	return GetTaskDate(hTask, TDL_TASKLASTMOD, TRUE);
}

time_t CTaskFile::GetTaskCreationDate(HTASKITEM hTask) const
{
	return GetTaskDate(hTask, TDL_TASKCREATIONDATE, FALSE);
}

time_t CTaskFile::GetTaskDoneDate(HTASKITEM hTask) const
{
	return GetTaskDate(hTask, TDL_TASKDONEDATE, TRUE);
}

time_t CTaskFile::GetTaskDueDate(HTASKITEM hTask) const
{
	return GetTaskDueDate(hTask, FALSE);
}

time_t CTaskFile::GetTaskDueDate(HTASKITEM hTask, BOOL bCalc) const
{
	if (bCalc && TaskHasAttribute(hTask, TDL_TASKCALCDUEDATE))
		return GetTaskDate(hTask, TDL_TASKCALCDUEDATE, TRUE);

	return GetTaskDate(hTask, TDL_TASKDUEDATE, TRUE);
}

LPCTSTR CTaskFile::GetTaskDueDateString(HTASKITEM hTask, BOOL bCalc) const
{
	if (bCalc && TaskHasAttribute(hTask, TDL_TASKCALCDUEDATESTRING))
		return GetTaskString(hTask, TDL_TASKCALCDUEDATESTRING);

	return GetTaskString(hTask, TDL_TASKDUEDATESTRING);
}

time_t CTaskFile::GetTaskStartDate(HTASKITEM hTask, BOOL bCalc) const
{
	if (bCalc && TaskHasAttribute(hTask, TDL_TASKCALCSTARTDATE))
		return GetTaskDate(hTask, TDL_TASKCALCSTARTDATE, TRUE);

	return GetTaskDate(hTask, TDL_TASKSTARTDATE, TRUE);
}

LPCTSTR CTaskFile::GetTaskStartDateString(HTASKITEM hTask, BOOL bCalc) const
{
	if (bCalc && TaskHasAttribute(hTask, TDL_TASKCALCSTARTDATESTRING))
		return GetTaskString(hTask, TDL_TASKCALCSTARTDATESTRING);

	return GetTaskString(hTask, TDL_TASKSTARTDATESTRING);
}

time_t CTaskFile::GetTaskStartDate(HTASKITEM hTask) const
{
	return GetTaskDate(hTask, TDL_TASKSTARTDATE, TRUE);
}

bool CTaskFile::GetTaskCreationDate64(HTASKITEM hTask, time64_t& timeT) const
{
	COleDateTime date = GetTaskCreationDateOle(hTask);

	return (CDateHelper::GetTimeT64(date, timeT) != FALSE);
}

bool CTaskFile::GetTaskStartDate64(HTASKITEM hTask, BOOL bCalc, time64_t& timeT) const
{
	COleDateTime date = GetTaskStartDateOle(hTask);

	if (bCalc && TaskHasAttribute(hTask, TDL_TASKCALCSTARTDATE))
		date = GetTaskDateOle(hTask, TDL_TASKCALCSTARTDATE, TRUE);
	
	return (CDateHelper::GetTimeT64(date, timeT) != FALSE);
}

bool CTaskFile::GetTaskDueDate64(HTASKITEM hTask, BOOL bCalc, time64_t& timeT) const
{
	COleDateTime date = GetTaskDueDateOle(hTask);
	
	if (bCalc && TaskHasAttribute(hTask, TDL_TASKCALCDUEDATE))
		date = GetTaskDateOle(hTask, TDL_TASKCALCDUEDATE, TRUE);

	return (CDateHelper::GetTimeT64(date, timeT) != FALSE);
}

bool CTaskFile::GetTaskDoneDate64(HTASKITEM hTask, time64_t& timeT) const
{
	COleDateTime date = GetTaskDoneDateOle(hTask);

	return (CDateHelper::GetTimeT64(date, timeT) != FALSE);
}

bool CTaskFile::GetTaskLastModified64(HTASKITEM hTask, time64_t& timeT) const
{
	COleDateTime date = GetTaskLastModifiedOle(hTask);

	return (CDateHelper::GetTimeT64(date, timeT) != FALSE);
}

COleDateTime CTaskFile::GetTaskLastModifiedOle(HTASKITEM hTask) const
{
	return GetTaskDateOle(hTask, TDL_TASKLASTMOD, TRUE);
}

COleDateTime CTaskFile::GetTaskDoneDateOle(HTASKITEM hTask) const
{
	return GetTaskDateOle(hTask, TDL_TASKDONEDATE, TRUE);
}

COleDateTime CTaskFile::GetTaskDueDateOle(HTASKITEM hTask) const
{
	return GetTaskDateOle(hTask, TDL_TASKDUEDATE, TRUE);
}

COleDateTime CTaskFile::GetTaskStartDateOle(HTASKITEM hTask) const
{
	return GetTaskDateOle(hTask, TDL_TASKSTARTDATE, TRUE);
}

COleDateTime CTaskFile::GetTaskCreationDateOle(HTASKITEM hTask) const
{
	return GetTaskDateOle(hTask, TDL_TASKCREATIONDATE, TRUE);
}

LPCTSTR CTaskFile::GetTaskDoneDateString(HTASKITEM hTask) const
{
	return GetTaskString(hTask, TDL_TASKDONEDATESTRING);
}

LPCTSTR CTaskFile::GetTaskCreationDateString(HTASKITEM hTask) const
{
	return GetTaskString(hTask, TDL_TASKCREATIONDATESTRING);
}

LPCTSTR CTaskFile::GetTaskDueDateString(HTASKITEM hTask) const
{
	return GetTaskDueDateString(hTask, FALSE);
}

LPCTSTR CTaskFile::GetTaskStartDateString(HTASKITEM hTask) const
{
	return GetTaskString(hTask, TDL_TASKSTARTDATESTRING);
}

unsigned long CTaskFile::GetTaskPosition(HTASKITEM hTask) const
{
	return GetTaskULong(hTask, TDL_TASKPOS);
}

LPCTSTR CTaskFile::GetTaskPositionString(HTASKITEM hTask) const
{
	return GetTaskString(hTask, TDL_TASKPOSSTRING);
}

bool CTaskFile::IsTaskDone(HTASKITEM hTask) const
{
	return (TaskHasAttribute(hTask, TDL_TASKDONEDATE, FALSE) ||
			(GetTaskPercentDone(hTask, FALSE) == 100));
}

bool CTaskFile::IsTaskDue(HTASKITEM hTask) const
{
	double dDue = GetTaskDueDateOle(hTask);

	if (dDue > 0.0)
	{
		double dToday = floor(COleDateTime::GetCurrentTime());
		return dDue <= dToday;
	}

	return false;
}

// interface version
bool CTaskFile::TaskHasAttribute(HTASKITEM hTask, LPCTSTR szAttrib) const
{
	return TaskHasAttribute(hTask, szAttrib, TRUE);
}

// internal version
bool CTaskFile::TaskHasAttribute(HTASKITEM hTask, LPCTSTR szAttrib, BOOL bOmitHidden) const
{
	// special case: parent task ID because m_bHideParentID
	// operates at a tasklist level
	if (bOmitHidden && m_bHideParentID && STR_MATCH(szAttrib, TDL_TASKPARENTID))
		return FALSE;

	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);
	
	const CXmlItem* pXIAttrib = pXITask->GetItem(szAttrib);
	
	if (!pXIAttrib || !pXIAttrib->HasValue()) // not found
	{
		// some fallbacks
		if (STR_MATCH(szAttrib, TDL_TASKCOLOR))
		{
			return TaskHasAttribute(hTask, TDL_TASKTEXTCOLOR, bOmitHidden); // RECURSIVE
		}
		else if (STR_MATCH(szAttrib, TDL_TASKWEBCOLOR))
		{
			return TaskHasAttribute(hTask, TDL_TASKTEXTWEBCOLOR, bOmitHidden); // RECURSIVE
		}
		else if (STR_MATCH(szAttrib, TDL_TASKPARENTID))
		{
			return TaskHasAttribute(hTask, TDL_TASKID, bOmitHidden); // RECURSIVE
		}
		else if (pXIAttrib && STR_MATCH(szAttrib, TDL_TASKCUSTOMATTRIBDATA))
		{
			return (pXIAttrib->HasItem(TDL_TASKCUSTOMATTRIBID) != FALSE);
		}

		// else
		return FALSE;
	}
	
	// finally check for hidden attribute
	return (!bOmitHidden || (pXIAttrib->GetItemValueI(_T("HIDE")) == 0));
}

LPCTSTR CTaskFile::GetTaskAttribute(HTASKITEM hTask, LPCTSTR szAttrib) const
{
	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, NULLSTRING);

	CString sValue = pXITask->GetItemValue(szAttrib);

	// special case: Parent ID
	if (sValue.IsEmpty() && STR_MATCH(szAttrib, TDL_TASKPARENTID) && TaskHasAttribute(hTask, TDL_TASKID))
	{
		static CString sPID;
		sPID.Format(_T("%lu"), GetTaskParentID(hTask));
		sValue = sPID;
	}

	return sValue;
}

HTASKITEM CTaskFile::GetTaskParent(HTASKITEM hTask) const
{
	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, NULL);

	return (HTASKITEM)pXITask->GetParent();
}

COLORREF CTaskFile::GetTaskTextColor(HTASKITEM hTask) const
{
	if (TaskHasAttribute(hTask, TDL_TASKTEXTCOLOR))
	{
		return GetTaskULong(hTask, TDL_TASKTEXTCOLOR);
	}
	else if (TaskHasAttribute(hTask, TDL_TASKCOLOR))
	{
		return GetTaskULong(hTask, TDL_TASKCOLOR);
	}

	// else
	return 0;
}

COLORREF CTaskFile::GetTaskBkgndColor(HTASKITEM /*hTask*/) const
{
	// DEPRECATED
// 	if (TaskHasAttribute(hTask, TDL_TASKBACKCOLOR))
// 		return GetTaskULong(hTask, TDL_TASKBACKCOLOR);

	// else
	return CLR_NONE;
}

int CTaskFile::GetTaskRisk(HTASKITEM hTask, BOOL bHighest) const
{
	if (bHighest && TaskHasAttribute(hTask, TDL_TASKHIGHESTRISK))
		return GetTaskInt(hTask, TDL_TASKHIGHESTRISK);

	return GetTaskInt(hTask, TDL_TASKRISK);
}

LPCTSTR CTaskFile::GetTaskExternalID(HTASKITEM hTask) const
{
	return GetTaskString(hTask, TDL_TASKEXTERNALID);
}

///////////////////////////////////////////////

bool CTaskFile::SetTaskTitle(HTASKITEM hTask, LPCTSTR szTitle)
{
	return SetTaskString(hTask, TDL_TASKTITLE, szTitle);
}

bool CTaskFile::SetTaskComments(HTASKITEM hTask, LPCTSTR szComments)
{
	return SetTaskString(hTask, TDL_TASKCOMMENTS, szComments, XIT_ELEMENT);
}

bool CTaskFile::SetTaskAllocatedTo(HTASKITEM hTask, LPCTSTR szAllocTo)
{
	return SetTaskString(hTask, TDL_TASKALLOCTO, szAllocTo);
}

bool CTaskFile::SetTaskMetaData(HTASKITEM hTask, LPCTSTR szKey, LPCTSTR szMetaData)
{
	if (Misc::IsEmpty(szMetaData))
	{
		return ClearTaskMetaData(hTask, szKey);
	}
	else if (Misc::IsEmpty(szKey))
	{
		ASSERT(0);
		return false;
	}

	// else
	return SetTaskAttribute(hTask, TDL_TASKMETADATA, szKey, szMetaData);
}

bool CTaskFile::ClearTaskMetaData(HTASKITEM hTask, LPCTSTR szKey)
{
	if (Misc::IsEmpty(szKey))
	{
		ASSERT(0);
		return false;
	}

	return DeleteTaskAttribute(hTask, TDL_TASKMETADATA, szKey);
}

bool CTaskFile::SetTaskCustomAttributeData(HTASKITEM hTask, LPCTSTR szID, LPCTSTR szData)
{
	if (Misc::IsEmpty(szData))
	{
		return ClearTaskCustomAttributeData(hTask, szID);
	}
	else if (Misc::IsEmpty(szID))
	{
		ASSERT(0);
		return false;
	}

	// first check that custom attribute exists
	CXmlItem* pXICustDef = GetCustomAttributeDef(szID);

	if (pXICustDef == NULL)
	{
		// treat as simple text type
		pXICustDef = AddCustomAttributeDef(szID, szID, szID, FALSE);

		if (!pXICustDef)
		{
			ASSERT(0);
			return false;
		}
	}

	// add data to task
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);

	CXmlItem* pXICustData = pXITask->AddItem(TDL_TASKCUSTOMATTRIBDATA);
	ASSERT(pXICustData);

	pXICustData->AddItem(TDL_TASKCUSTOMATTRIBID, Misc::ToUpper(szID));
	pXICustData->AddItem(TDL_TASKCUSTOMATTRIBVALUE, szData);

	// If the type is 'LIST' append the data to the definition
	// and if the data contains multiple values then change the
	// list definition to 'MULTI-LIST'
	TDCCUSTOMATTRIBUTEDEFINITION temp;
	temp.SetListType(pXICustDef->GetItemValueI(TDL_CUSTOMATTRIBTYPE));

	if (temp.IsList())
	{
		temp.DecodeListData(pXICustDef->GetItemValue(TDL_CUSTOMATTRIBLISTDATA));

		CStringArray aData;
		VERIFY(Misc::Split(szData, aData, '\n'));

		if (aData.GetSize() > 1)
		{
			temp.SetListType(TDCCA_AUTOMULTILIST);
			pXICustDef->SetItemValue(TDL_CUSTOMATTRIBTYPE, (int)temp.GetAttributeType());
		}

		if (Misc::AddUniqueItems(aData, temp.aDefaultListData))
		{
			pXICustDef->SetItemValue(TDL_CUSTOMATTRIBLISTDATA, temp.EncodeListData());
		}
	}

	return true;
}

bool CTaskFile::ClearTaskCustomAttributeData(HTASKITEM hTask, LPCTSTR szID)
{
	if (Misc::IsEmpty(szID))
	{
		ASSERT(0);
		return false;
	}

	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);

	// find the item having name == szID
	CXmlItem* pXICustData = pXITask->GetItem(TDL_TASKCUSTOMATTRIBDATA);

	while (pXICustData)
	{
		if (pXICustData->GetItemValue(TDL_TASKCUSTOMATTRIBID).CompareNoCase(szID) == 0)
			return (pXICustData->DeleteItem(pXICustData) != FALSE);

		pXICustData = pXICustData->GetSibling();
	}

	// not found
	return false;
}

bool CTaskFile::SetTaskDependency(HTASKITEM hTask, LPCTSTR szDepends)
{
	return SetTaskString(hTask, TDL_TASKDEPENDENCY, szDepends);
}

bool CTaskFile::SetTaskAllocatedBy(HTASKITEM hTask, LPCTSTR szAllocBy)
{
	return SetTaskString(hTask, TDL_TASKALLOCBY, szAllocBy);
}

bool CTaskFile::SetTaskCategory(HTASKITEM hTask, LPCTSTR szCategory)
{
	return SetTaskString(hTask, TDL_TASKCATEGORY, szCategory);
}

bool CTaskFile::SetTaskStatus(HTASKITEM hTask, LPCTSTR szStatus)
{
	return SetTaskString(hTask, TDL_TASKSTATUS, szStatus);
}

bool CTaskFile::SetTaskFlag(HTASKITEM hTask, bool bFlag)
{
	return SetTaskUChar(hTask, TDL_TASKFLAG, (unsigned char)(bFlag ? 1 : 0));
}

bool CTaskFile::SetTaskFileLinkPath(HTASKITEM hTask, LPCTSTR szFileRefpath)
{
	return SetTaskString(hTask, TDL_TASKFILEREFPATH, szFileRefpath);
}

bool CTaskFile::SetTaskCreatedBy(HTASKITEM hTask, LPCTSTR szCreatedBy)
{
	return SetTaskString(hTask, TDL_TASKCREATEDBY, szCreatedBy);
}

BOOL CTaskFile::SetTaskID(HTASKITEM hTask, unsigned long nID, BOOL bVisible)
{
	if (SetTaskULong(hTask, TDL_TASKID, nID))
	{
		// update m_dwNextUniqueID
		m_dwNextUniqueID = max(m_dwNextUniqueID, nID + 1);

		return HideAttribute(hTask, TDL_TASKID, !bVisible);
	}
	
	return FALSE;
}

BOOL CTaskFile::SetTaskReferenceID(HTASKITEM hTask, unsigned long nRefID, BOOL bVisible)
{
	if (SetTaskULong(hTask, TDL_TASKREFID, nRefID))
	{
		return HideAttribute(hTask, TDL_TASKREFID, !bVisible);
	}
	
	return false;
}

DWORD CTaskFile::GetTaskReferenceID(HTASKITEM hTask) const
{
	return GetTaskULong(hTask, TDL_TASKREFID);
}

bool CTaskFile::IsTaskReference(HTASKITEM hTask) const
{
	return (GetTaskReferenceID(hTask) > 0);
}

BOOL CTaskFile::HideAttribute(HTASKITEM hTask, LPCTSTR szAttrib, BOOL bHide)
{
	// Hiding parent ID works at file level
	if (STR_MATCH(szAttrib, TDL_TASKPARENTID))
	{
		m_bHideParentID = bHide;
		return TRUE;
	}

	// else hiding works at a task level
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, FALSE);

	CXmlItem* pXIAttrib = pXITask->GetItem(szAttrib);

	if (pXIAttrib)
	{
		CXmlItem* pXIHide = pXIAttrib->GetItem(_T("HIDE"));

		// if not visible add 'HIDE' attribute
		if (bHide)
		{
			if (pXIHide)
				pXIHide->SetValue(1);
			else
				pXIAttrib->AddItem(_T("HIDE"), 1);
		}
		else if (pXIHide)
			pXIAttrib->DeleteItem(pXIHide);
		
		return TRUE;
	}
	
	return FALSE;
}

bool CTaskFile::SetTaskColor(HTASKITEM hTask, unsigned long nColor)
{
	if ((nColor == 0) || (nColor == CLR_NONE))
	{
		DeleteTaskAttribute(hTask, TDL_TASKCOLOR);
		DeleteTaskAttribute(hTask, TDL_TASKWEBCOLOR);

		return true;
	}

	// else
	return SetTaskULong(hTask, TDL_TASKCOLOR, nColor) &&
			SetTaskString(hTask, TDL_TASKWEBCOLOR, GraphicsMisc::GetWebColor((COLORREF)nColor));
}

bool CTaskFile::SetTaskPriority(HTASKITEM hTask, int nPriority)
{
	if (nPriority != FM_NOPRIORITY)
		nPriority = (char)max(0, min(10, nPriority));

	return SetTaskInt(hTask, TDL_TASKPRIORITY, nPriority);
}

bool CTaskFile::SetTaskPercentDone(HTASKITEM hTask, unsigned char nPercent)
{
	return SetTaskUChar(hTask, TDL_TASKPERCENTDONE, nPercent);
}

bool CTaskFile::SetTaskCost(HTASKITEM hTask, double dCost)
{
	return SetTaskDouble(hTask, TDL_TASKCOST, dCost);
}

bool CTaskFile::SetTaskTimeEstimate(HTASKITEM hTask, double dTimeEst, TDC_UNITS cUnits)
{
	return SetTaskTime(hTask, TDL_TASKTIMEESTIMATE, dTimeEst, TDL_TASKTIMEESTUNITS, cUnits);
}

bool CTaskFile::SetTaskTimeSpent(HTASKITEM hTask, double dTimeSpent, TDC_UNITS cUnits)
{
	return SetTaskTime(hTask, TDL_TASKTIMESPENT, dTimeSpent, TDL_TASKTIMESPENTUNITS, cUnits);
}

bool CTaskFile::SetTaskLastModified(HTASKITEM hTask, time_t tLastMod)
{
	return SetTaskDate(hTask, TDL_TASKLASTMOD, tLastMod);
}

bool CTaskFile::SetTaskDoneDate(HTASKITEM hTask, time_t tDoneDate)
{
	return SetTaskDate(hTask, TDL_TASKDONEDATE, tDoneDate);
}

bool CTaskFile::SetTaskDueDate(HTASKITEM hTask, time_t tDueDate)
{
	return SetTaskDate(hTask, TDL_TASKDUEDATE, tDueDate);
}

bool CTaskFile::SetTaskStartDate(HTASKITEM hTask, time_t tStartDate)
{
	return SetTaskDate(hTask, TDL_TASKSTARTDATE, tStartDate);
}

bool CTaskFile::SetTaskCreationDate(HTASKITEM hTask, time_t tCreationDate)
{
	return SetTaskDate(hTask, TDL_TASKCREATIONDATE, tCreationDate);
}

bool CTaskFile::SetTaskCreationDate64(HTASKITEM hTask, time64_t tDate)
{
	COleDateTime date = CDateHelper::GetDate(tDate);

	return SetTaskDate(hTask, TDL_TASKCREATIONDATE, date);
}

bool CTaskFile::SetTaskLastModified64(HTASKITEM hTask, time64_t tDate)
{
	COleDateTime date = CDateHelper::GetDate(tDate);
	
	return SetTaskDate(hTask, TDL_TASKLASTMOD, date);
}

bool CTaskFile::SetTaskDoneDate64(HTASKITEM hTask, time64_t tDate)
{
	COleDateTime date = CDateHelper::GetDate(tDate);
	
	return SetTaskDate(hTask, TDL_TASKDONEDATE, date);
}

bool CTaskFile::SetTaskDueDate64(HTASKITEM hTask, time64_t tDate)
{
	COleDateTime date = CDateHelper::GetDate(tDate);
	
	return SetTaskDate(hTask, TDL_TASKDUEDATE, date);
}

bool CTaskFile::SetTaskStartDate64(HTASKITEM hTask, time64_t tDate)
{
	COleDateTime date = CDateHelper::GetDate(tDate);
	
	return SetTaskDate(hTask, TDL_TASKSTARTDATE, date);
}

BOOL CTaskFile::SetTaskLastModified(HTASKITEM hTask, const COleDateTime& tLastMod)
{
	return (BOOL)SetTaskDate(hTask, TDL_TASKLASTMOD, tLastMod, TDL_TASKLASTMODSTRING);
}

BOOL CTaskFile::SetTaskCreationDate(HTASKITEM hTask, const COleDateTime& date)
{
	return (BOOL)SetTaskDate(hTask, TDL_TASKCREATIONDATE, date, TDL_TASKCREATIONDATESTRING);
}

BOOL CTaskFile::SetTaskDoneDate(HTASKITEM hTask, const COleDateTime& date)
{
	return (BOOL)SetTaskDate(hTask, TDL_TASKDONEDATE, date, TDL_TASKDONEDATESTRING);
}

BOOL CTaskFile::SetTaskDueDate(HTASKITEM hTask, const COleDateTime& date)
{
	return (BOOL)SetTaskDate(hTask, TDL_TASKDUEDATE, date, TDL_TASKDUEDATESTRING);
}

BOOL CTaskFile::SetTaskStartDate(HTASKITEM hTask, const COleDateTime& date)
{
	return (BOOL)SetTaskDate(hTask, TDL_TASKSTARTDATE, date, TDL_TASKSTARTDATESTRING);
}

BOOL CTaskFile::SetTaskRecurrence(HTASKITEM hTask, const TDCRECURRENCE& tr)
{
	DWORD dwSpecific1 = 0, dwSpecific2 = 0;
	TDI_REGULARITY nRegularity = tr.GetRegularity(dwSpecific1, dwSpecific2);

	int nNumOccur = tr.GetOccurrenceCount();
	int nRemainingOccur = tr.GetRemainingOccurrenceCount();

	if (SetTaskRecurrence(hTask, 
							nRegularity, 
							dwSpecific1, 
							dwSpecific2, 
							tr.nRecalcFrom, 
							tr.nReuse, 
							nNumOccur,
							nRemainingOccur))
	{
		SetTaskString(hTask, TDL_TASKRECURRENCE, tr.GetRegularityText(), XIT_ELEMENT); // human readable
		return true;
	}
	
	return false;
}

// Internal version
bool CTaskFile::SetTaskRecurrence(HTASKITEM hTask, int nRegularity, DWORD dwSpecific1, 
									DWORD dwSpecific2, int nRecalcFrom, int nReuse, 
									int nNumOccur, int nRemainingOccur)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);

	CXmlItem* pXIRecur = pXITask->GetAddItem(TDL_TASKRECURRENCE);
	ASSERT(pXIRecur);

	if (pXIRecur)
	{
		// Enforce TDIRO_DONEDATE for 'recreate after...' options
		switch (nRegularity)
		{
		case TDIR_DAY_RECREATEAFTERNDAYS_DEP:
		case TDIR_WEEK_RECREATEAFTERNWEEKS_DEP:
		case TDIR_MONTH_RECREATEAFTERNMONTHS_DEP:
		case TDIR_YEAR_RECREATEAFTERNYEARS_DEP:
			ASSERT(0);
			nRecalcFrom = TDIRO_DONEDATE;
			break;
		}

		pXIRecur->SetItemValue(TDL_TASKRECURRENCEFREQ, nRegularity);
		pXIRecur->SetItemValue(TDL_TASKRECURRENCESPEC1, (int)dwSpecific1);
		pXIRecur->SetItemValue(TDL_TASKRECURRENCESPEC2, (int)dwSpecific2);
		pXIRecur->SetItemValue(TDL_TASKRECURRENCEREUSE, nReuse);
		pXIRecur->SetItemValue(TDL_TASKRECURRENCEFROM, nRecalcFrom);
		pXIRecur->SetItemValue(TDL_TASKRECURRENCENUM, nNumOccur);
		pXIRecur->SetItemValue(TDL_TASKRECURRENCEREMAINING, nRemainingOccur);
		
		return true;
	}
	
	return false;
}

// ITaskList6 interface
bool CTaskFile::SetTaskRecurrence(HTASKITEM hTask, int nRegularity, DWORD dwSpecific1, 
									DWORD dwSpecific2, BOOL bRecalcFromDue, int nReuse)
{
	return SetTaskRecurrence(hTask, nRegularity, dwSpecific1, dwSpecific2, 
							(bRecalcFromDue ? TDIRO_DUEDATE : TDIRO_DONEDATE), nReuse, -1);
}

// ITaskList13 interface
bool CTaskFile::SetTaskRecurrence(HTASKITEM hTask, int nRegularity, DWORD dwSpecific1, 
									DWORD dwSpecific2, int nRecalcFrom, int nReuse, int nNumOccur)
{
	return SetTaskRecurrence(hTask, nRegularity, dwSpecific1, dwSpecific2, 
							nRecalcFrom, nReuse, nNumOccur, nNumOccur);
}

bool CTaskFile::SetTaskAttribute(HTASKITEM hTask, LPCTSTR szAttrib, LPCTSTR szValue)
{
	return SetTaskString(hTask, szAttrib, szValue);
}

bool CTaskFile::SetTaskVersion(HTASKITEM hTask, LPCTSTR szVersion)
{
	return SetTaskString(hTask, TDL_TASKVERSION, szVersion);
}

BOOL CTaskFile::SetTaskTextColor(HTASKITEM hTask, COLORREF color)
{
	// Text colour can be zero
	if (color == CLR_NONE)
	{
		DeleteTaskAttribute(hTask, TDL_TASKTEXTCOLOR);
		DeleteTaskAttribute(hTask, TDL_TASKTEXTWEBCOLOR);
		
		return true;
	}
	
	return (SetTaskULong(hTask, TDL_TASKTEXTCOLOR, color) &&
			SetTaskString(hTask, TDL_TASKTEXTWEBCOLOR, GraphicsMisc::GetWebColor(color)));
}

bool CTaskFile::SetTaskIcon(HTASKITEM hTask, LPCTSTR szIcon)
{
	return SetTaskString(hTask, TDL_TASKICONINDEX, szIcon);
}

BOOL CTaskFile::SetTaskSubtaskCompletion(HTASKITEM hTask, LPCTSTR szSubtaskDone)
{
	return SetTaskString(hTask, TDL_TASKSUBTASKDONE, szSubtaskDone);
}

BOOL CTaskFile::SetTaskGoodAsDone(HTASKITEM hTask, BOOL bDone)
{
	if (!bDone)
	{
		DeleteTaskAttribute(hTask, TDL_TASKGOODASDONE);
		return TRUE;
	}

	// else
	return SetTaskUChar(hTask, TDL_TASKGOODASDONE, 1);
}

BOOL CTaskFile::SetTaskPriorityColor(HTASKITEM hTask, COLORREF color)
{
	if ((color == 0) || (color == CLR_NONE))
	{
		DeleteTaskAttribute(hTask, TDL_TASKPRIORITYCOLOR);
		DeleteTaskAttribute(hTask, TDL_TASKPRIORITYWEBCOLOR);
		
		return true;
	}
	
	return SetTaskULong(hTask, TDL_TASKPRIORITYCOLOR, color) &&
			SetTaskString(hTask, TDL_TASKPRIORITYWEBCOLOR, GraphicsMisc::GetWebColor(color));
}

BOOL CTaskFile::SetTaskCalcTimeEstimate(HTASKITEM hTask, double dTime, TDC_UNITS cUnits)
{
	return SetTaskTime(hTask, TDL_TASKCALCTIMEESTIMATE, dTime, TDL_TASKCALCTIMEESTUNITS, cUnits);
}

BOOL CTaskFile::SetTaskCalcTimeSpent(HTASKITEM hTask, double dTime, TDC_UNITS cUnits)
{
	return SetTaskTime(hTask, TDL_TASKCALCTIMESPENT, dTime, TDL_TASKCALCTIMESPENTUNITS, cUnits);
}

BOOL CTaskFile::SetTaskCalcDueDate(HTASKITEM hTask, const COleDateTime& date)
{
	return SetTaskDate(hTask, TDL_TASKCALCDUEDATE, date, TDL_TASKCALCDUEDATESTRING, TRUE);
}

BOOL CTaskFile::SetTaskCalcStartDate(HTASKITEM hTask, const COleDateTime& date)
{
	return SetTaskDate(hTask, TDL_TASKCALCSTARTDATE, date, TDL_TASKCALCSTARTDATESTRING, TRUE);
}

BOOL CTaskFile::SetTaskCalcCompletion(HTASKITEM hTask, int nPercent)
{
	return SetTaskUChar(hTask, TDL_TASKCALCCOMPLETION, (unsigned char)nPercent);
}

BOOL CTaskFile::SetTaskHighestPriority(HTASKITEM hTask, int nPriority)
{
	if (nPriority != FM_NOPRIORITY)
		nPriority = (char)max(0, min(10, nPriority));

	return SetTaskInt(hTask, TDL_TASKHIGHESTPRIORITY, nPriority);
}

BOOL CTaskFile::SetTaskHighestRisk(HTASKITEM hTask, int nRisk)
{
	if (nRisk != FM_NORISK)
		nRisk = (char)max(0, min(10, nRisk));

	return SetTaskInt(hTask, TDL_TASKHIGHESTRISK, nRisk);
}

BOOL CTaskFile::SetTaskCalcCost(HTASKITEM hTask, double dCost)
{
	return SetTaskDouble(hTask, TDL_TASKCALCCOST, dCost);
}

bool CTaskFile::SetTaskPosition(HTASKITEM hTask, LPCTSTR szPos)
{
	return SetTaskString(hTask, TDL_TASKPOSSTRING, szPos);
}

bool CTaskFile::SetTaskPosition(HTASKITEM hTask, unsigned long nPos)
{
	return SetTaskULong(hTask, TDL_TASKPOS, nPos);
}

bool CTaskFile::SetTaskRisk(HTASKITEM hTask, int nRisk)
{
	if (nRisk != FM_NORISK)
		nRisk = (char)max(-1, min(10, nRisk));

	return SetTaskInt(hTask, TDL_TASKRISK, nRisk);
}

bool CTaskFile::SetTaskExternalID(HTASKITEM hTask, LPCTSTR szID)
{
	return SetTaskString(hTask, TDL_TASKEXTERNALID, szID);
}

bool CTaskFile::SetTaskIsParent(HTASKITEM hTask)
{
	return SetTaskUChar(hTask, TDL_TASKISPARENT, 1);
}

////////////////////////////////////////////////
// utility functions

time_t CTaskFile::GetTaskDate(HTASKITEM hTask, const CString& sDateItem, BOOL bIncTime) const
{
	COleDateTime date = GetTaskDateOle(hTask, sDateItem, bIncTime);
	time_t timeT;

	return (CDateHelper::GetTimeT(date, timeT) ? timeT : 0);
}

COleDateTime CTaskFile::GetTaskDateOle(HTASKITEM hTask, const CString& sDateItem, BOOL bIncTime) const
{
	COleDateTime date;

	if (TaskHasAttribute(hTask, sDateItem))
	{
		date = GetTaskDouble(hTask, sDateItem);

		if (!bIncTime)
			date = CDateHelper::GetDateOnly(date);
	}
	else
	{
		CDateHelper::ClearDate(date);
	}

	return date;
}

unsigned char CTaskFile::GetTaskUChar(HTASKITEM hTask, const CString& sUCharItem) const
{
	unsigned long lVal = GetTaskULong(hTask, sUCharItem);

	return (unsigned char)min(lVal, 255);
}

unsigned long CTaskFile::GetTaskULong(HTASKITEM hTask, const CString& sULongItem) const
{
	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, 0);

	return (unsigned long)pXITask->GetItemValueI(sULongItem);
}

int CTaskFile::GetTaskInt(HTASKITEM hTask, const CString& sIntItem) const
{
	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, 0);

	return pXITask->GetItemValueI(sIntItem);
}

CString CTaskFile::GetTaskString(HTASKITEM hTask, const CString& sStringItem) const
{
	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, NULLSTRING);

	return pXITask->GetItemValue(sStringItem);
}

double CTaskFile::GetTaskDouble(HTASKITEM hTask, const CString& sDoubleItem) const
{
	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, 0.0);

	return pXITask->GetItemValueF(sDoubleItem);
}

double CTaskFile::GetTaskTime(HTASKITEM hTask, const CString& sTimeItem) const
{
	return GetTaskDouble(hTask, sTimeItem);
}

CString CTaskFile::GetTaskAttribute(HTASKITEM hTask, const CString& sAttrib, const CString& sKey) const
{
	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, NULLSTRING);

	return pXITask->GetItemValue(sAttrib, sKey);
}

////////////////////////////////////////////////////////////////////

bool CTaskFile::DeleteTaskAttribute(HTASKITEM hTask, const CString& sAttrib, const CString& sKey)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);

	return (pXITask->DeleteItem(sAttrib, sKey) != FALSE);
}

bool CTaskFile::SetTaskAttribute(HTASKITEM hTask, const CString& sAttrib, const CString& sKey, const CString& sValue)
{
	ASSERT(!sAttrib.IsEmpty());

	if (sAttrib.IsEmpty())
		return false;

	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);

	if (sKey.IsEmpty())
		return (NULL != pXITask->SetItemValue(sAttrib, sValue));

	// else
	CXmlItem* pXItem = pXITask->GetAddItem(sAttrib);
	ASSERT (pXItem);

	return (NULL != pXItem->SetItemValue(sKey, sValue));
}

bool CTaskFile::SetTaskDate(HTASKITEM hTask, const CString& sDateItem, const COleDateTime& date, 
							const CString& sDateStringItem, BOOL bCalculated)
{
	if (SetTaskDouble(hTask, sDateItem, date))
	{
		if (!sDateStringItem.IsEmpty())
		{
			DWORD dwFmt = m_bISODates ? DHFD_ISO : 0;
			
			if (!bCalculated && CDateHelper::DateHasTime(date))
				dwFmt |= DHFD_TIME | DHFD_NOSEC;
			
			return SetTaskString(hTask, sDateStringItem, CDateHelper::FormatDate(date, dwFmt));
		}
		
		return true;
	}
	
	// else
	return false;
}

bool CTaskFile::SetTaskDate(HTASKITEM hTask, const CString& sDateItem, time_t tVal)
{
	if (tVal == 0) 
        return SetTaskDate(hTask, sDateItem, COleDateTime(0.0));

	//fabio_2005
#if _MSC_VER >= 1400
    errno_t err;
	tm time;
	tm* pTime = &time;
        // Get time as 64-bit integer.
        // Convert to local time
	err= localtime_s(pTime ,&tVal  );
#else
	tm* pTime = localtime(&tVal);
#endif

	if (!pTime)
		return false;

	// else
	COleDateTime date(pTime->tm_year + 1900,
					  pTime->tm_mon + 1,
					  pTime->tm_mday,
					  pTime->tm_hour,
					  pTime->tm_min,
					  pTime->tm_sec);
	
	return SetTaskDate(hTask, sDateItem, date);
}

bool CTaskFile::SetTaskUChar(HTASKITEM hTask, const CString& sUCharItem, unsigned char cVal)
{
	return SetTaskULong(hTask, sUCharItem, cVal);
}

bool CTaskFile::SetTaskULong(HTASKITEM hTask, const CString& sULongItem, unsigned long lVal)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);
	
	return (pXITask->SetItemValue(sULongItem, (int)lVal) != NULL);
}

bool CTaskFile::SetTaskInt(HTASKITEM hTask, const CString& sIntItem, int iVal)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);
	
	return (pXITask->SetItemValue(sIntItem, iVal) != NULL);
}

bool CTaskFile::SetTaskString(HTASKITEM hTask, const CString& sStringItem, const CString& sVal, XI_TYPE nType)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);
	
	return (pXITask->SetItemValue(sStringItem, sVal, nType) != NULL);
}

bool CTaskFile::SetTaskDouble(HTASKITEM hTask, const CString& sDoubleItem, double dVal)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);
	
	return (pXITask->SetItemValue(sDoubleItem, dVal) != NULL);
}

bool CTaskFile::SetTaskTime(HTASKITEM hTask, const CString& sTimeItem, double dTime,
							const CString& sUnitsItem, TDC_UNITS nUnits)
{
	LPCTSTR szUnits;

	switch (nUnits)
	{
	case TDCU_MINS:		szUnits = _T("I"); break;
	case TDCU_HOURS:	szUnits = _T("H"); break;
	case TDCU_DAYS:		szUnits = _T("D"); break;
	case TDCU_WEEKDAYS: szUnits = _T("K"); break;
	case TDCU_WEEKS:	szUnits = _T("W"); break;
	case TDCU_MONTHS:	szUnits = _T("M"); break;
	case TDCU_YEARS:	szUnits = _T("Y"); break;

	default: // all else
		ASSERT(0);
		return false;
	}

	return SetTaskDouble(hTask, sTimeItem, dTime) &&
		  	SetTaskString(hTask, sUnitsItem, szUnits);
}

BOOL CTaskFile::SetMetaData(CXmlItem* pXIParent, const CMapStringToString& mapMetaData)
{
	// delete existing meta data
	pXIParent->DeleteItem(TDL_METADATA);

	// re-add
	CString sKey, sValue;
	POSITION pos = mapMetaData.GetStartPosition();

	while (pos)
	{
		mapMetaData.GetNextAssoc(pos, sKey, sValue);

		if (sKey.IsEmpty() || sValue.IsEmpty())
			continue;

		CXmlItem* pXIMetaData = pXIParent->GetAddItem(TDL_METADATA);
		ASSERT(pXIMetaData);

		pXIMetaData->AddItem(sKey, sValue);
	}

	// else
	return TRUE;
}

int CTaskFile::GetMetaData(const CXmlItem* pXIParent, CMapStringToString& mapMetaData)
{
	mapMetaData.RemoveAll();

	const CXmlItem* pXIMetaData = pXIParent->GetItem(TDL_METADATA);

	if (pXIMetaData)
	{
		// read all the children
		POSITION pos = pXIMetaData->GetFirstItemPos();

		while (pos)
		{
			const CXmlItem* pXIData = pXIMetaData->GetNextItem(pos);

			if (pXIData && pXIData->HasName() && pXIData->HasValue())
			{
				mapMetaData[pXIData->GetName()] = pXIData->GetValue();
			}
		}
	}

	return mapMetaData.GetCount();
}

CString CTaskFile::GetTaskArrayItem(HTASKITEM hTask, const CString& sItemTag, int nIndex) const
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, _T(""));
	
	const CXmlItem* pXI = pXITask->GetItem(sItemTag);

	while (pXI)
	{
		if (nIndex == 0) // this is the item we want
			return pXI->GetValue();

		// else continue
		nIndex--;
		pXI = pXI->GetSibling();
	}

	// not found
	return _T("");
}

bool CTaskFile::LegacyDeleteTaskArray(HTASKITEM hTask, const CString& sNumItemTag, 
								const CString& sItemTag)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);
	
	// delete any existing items
	pXITask->DeleteItem(sItemTag);

	int nItem, nNumExists = pXITask->GetItemValueI(sNumItemTag);

	for (nItem = 1; nItem < nNumExists; nItem++)
	{
		CString sItem;
		sItem.Format(_T("%s%d"), sItemTag, nItem);
		pXITask->DeleteItem(sItem);
	}

	pXITask->DeleteItem(sNumItemTag);

	return true;
}

bool CTaskFile::AddTaskArrayItem(HTASKITEM hTask, const CString& sItemTag, const CString& sItem)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);

	CXmlItem* pXI = pXITask->GetItem(sItemTag);

	// check existence before adding
	if (!pXI || !pXI->FindItem(sItemTag, sItem, FALSE))
		pXITask->AddItem(sItemTag, sItem, XIT_ELEMENT);
	
	return true;
}

int CTaskFile::GetTaskArray(HTASKITEM hTask, const CString& sItemTag, CStringArray& aItems) const
{
	aItems.RemoveAll();

	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, 0);

	// first item
	const CXmlItem* pXI = pXITask->GetItem(sItemTag);
	
	while (pXI)
	{
		aItems.Add(pXI->GetValue());
		pXI = pXI->GetSibling();
	}

	return aItems.GetSize();
}

int CTaskFile::LegacyGetTaskArray(HTASKITEM hTask, const CString& sNumItemTag,
				  	 const CString& sItemTag, CStringArray& aItems) const
{
	aItems.RemoveAll();

	// first item
	CString sItem = GetTaskString(hTask, sItemTag);

	if (!sItem.IsEmpty())
	{
		aItems.Add(sItem);

		// rest
		int nCount = GetTaskUChar(hTask, sNumItemTag);

		for (int nItem = 1; nItem < nCount; nItem++)
		{
			sItem.Format(_T("%s%d"), sItemTag, nItem);
			aItems.Add(GetTaskString(hTask, sItem));
		}
	}

	return aItems.GetSize();

}

int CTaskFile::GetTaskArraySize(HTASKITEM hTask, const CString& sItemTag) const
{
	CStringArray aTemp;
	return GetTaskArray(hTask, sItemTag, aTemp);
}

bool CTaskFile::DeleteTaskArray(HTASKITEM hTask, const CString& sItemTag)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, FALSE);

	// delete any existing items
	return (pXITask->DeleteItem(sItemTag) == TRUE);
}

BOOL CTaskFile::SetTaskArray(HTASKITEM hTask, const CString& sItemTag, const CStringArray& aItems)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, FALSE);

	// delete any existing items
	DeleteTaskArray(hTask, sItemTag);

	// then add these
	int nCount = aItems.GetSize();
	
	for (int nItem = 0; nItem < nCount; nItem++)
		AddTaskArrayItem(hTask, sItemTag, aItems[nItem]);

	return TRUE;
}

int CTaskFile::GetArray(const CString& sItemTag, CStringArray& aItems) const
{
	aItems.RemoveAll();

	// first item
	const CXmlItem* pXI = GetItem(sItemTag);

	while (pXI)
	{
		aItems.Add(pXI->GetValue());
		pXI = pXI->GetSibling();
	}

	return aItems.GetSize();
}

bool CTaskFile::LegacyDeleteArray(const CString& sItemTag)
{
	return (DeleteItem(sItemTag) == TRUE);
}

int CTaskFile::LegacyGetArray(const CString& sItemTag, CStringArray& aItems) const
{
	aItems.RemoveAll();

	// first item
	const CXmlItem* pXI = GetItem(sItemTag);

	if (pXI)
	{
		int nCount = pXI->GetItemCount();

		for (int nItem = 0; nItem < nCount; nItem++)
		{
			CString sItem;
			sItem.Format(_T("%s%d"), sItemTag, nItem);

			aItems.Add(pXI->GetItemValue(sItem));
		}
	}

	return aItems.GetSize();
}

BOOL CTaskFile::SetArray(const CString& sItemTag, const CStringArray& aItems)
{
	// delete existing items
	DeleteItem(sItemTag);

	// and start again
	int nCount = aItems.GetSize();
	
	for (int nItem = 0; nItem < nCount; nItem++)
		AddItem(sItemTag, aItems[nItem], XIT_ELEMENT);

	return TRUE;
}


