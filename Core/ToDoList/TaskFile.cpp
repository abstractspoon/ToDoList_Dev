// TaskFile.cpp: implementation of the CTaskFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaskFile.h"
#include "tdcmergetasklist.h"
#include "tdcstruct.h"
#include "todoitem.h"

#include "..\shared\timeedit.h"
#include "..\shared\datehelper.h"
#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\binarydata.h"
#include "..\shared\xslfile.h"

#include "..\3rdparty\Base64Coder.h"

#include "..\Interfaces\TasklistSchemaDef.h"

#include <math.h>

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

static const CString NULLSTRING;
static const LPCTSTR METADATA_SELECTED = _T("__selected__");

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
		if (lstrcmpi(TDL_TASK, szItem) == 0)
			return FALSE; // stop
		
		if (CXmlFileEx::XFE_ENCODEDDATA.CompareNoCase(szItem) == 0)
			return FALSE; // stop

		return TRUE; // continue
	}
};

//////////////////////////////////////////////////////////////////////

#define COPYSTRATTRIB(pFrom, hFrom, fFrom, pTo, hTo, fTo) \
	{ LPCTSTR szVal = pFrom->fFrom(hFrom); \
	if (!Misc::IsEmpty(szVal)) pTo->fTo(hTo, szVal); }

//////////////////////////////////////////////////////////////////////

#define GET_TASK(t, h, r) { t = TaskFromHandle(h); if (!t) return r; }

//////////////////////////////////////////////////////////////////////

TASKFILE_HEADER::TASKFILE_HEADER() 
	: 
	bArchive(-1), 
	bUnicode(-1), 
	dwNextID(0), 
	nFileFormat(TDL_FILEFORMAT_CURRENT), 
	nFileVersion(1) 
{
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaskFile::CTaskFile(LPCTSTR szPassword) 
	: 
	CXmlFileEx(TDL_ROOT, szPassword),
	m_dwNextUniqueID(1), 
	m_bISODates(FALSE)
{
	m_mapHandles.InitHashTable(ITASK_HASHTABLE_SIZE, FALSE);
}

CTaskFile::CTaskFile(const CTaskFile& tasks, LPCTSTR szPassword)
	: 
	CXmlFileEx(TDL_ROOT, szPassword),
	m_dwNextUniqueID(1), 
	m_bISODates(FALSE)
{
	m_mapHandles.InitHashTable(ITASK_HASHTABLE_SIZE, FALSE);

	CopyFrom(tasks);
}

CTaskFile::CTaskFile(const ITaskList* pTasks, LPCTSTR szPassword)
	: 
	CXmlFileEx(TDL_ROOT, szPassword),
	m_dwNextUniqueID(1), 
	m_bISODates(FALSE)
{
	m_mapHandles.InitHashTable(ITASK_HASHTABLE_SIZE, FALSE);

	CopyFrom(pTasks);
}

CTaskFile::~CTaskFile()
{
}

HRESULT CTaskFile::QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
{
	// Default interface
	*ppvObject = reinterpret_cast<ITaskList*>(this);

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
	else if (IsEqualIID(riid, IID_TASKLIST16))
	{
		*ppvObject = reinterpret_cast<ITaskList16*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_TASKLIST17))
	{
		*ppvObject = reinterpret_cast<ITaskList17*>(this);
		AddRef();
	}
	else if (IsEqualIID(riid, IID_TASKLIST18))
	{
		*ppvObject = reinterpret_cast<ITaskList18*>(this);
		AddRef();
	}
	else
	{
		ASSERT(0);
	}
	
	return (*ppvObject ? S_OK : E_NOTIMPL);
}


BOOL CTaskFile::TransformToFile(const CString& sTransformPath, const CString& sOutputPath, SFE_FORMAT nFormat) const
{
	if (nFormat == SFEF_AUTODETECT)
	{
		CXslFile xsl;
		
		if (!xsl.Load(sTransformPath))
		{
			ASSERT(0);
			return FALSE;
		}

		nFormat = xsl.GetOutputFileEncoding();
	}
	
	return CXmlFile::TransformToFile(sTransformPath, sOutputPath, nFormat);
}

BOOL CTaskFile::Decrypt(LPCTSTR szPassword)
{
	BOOL bWasEncrypted = IsEncrypted();
	BOOL bResult = CXmlFileEx::Decrypt(szPassword);

	if (bResult && bWasEncrypted)
	{
		// fix corrupted tasklist where the root item has an ID
		DeleteItem(TDL_TASKID);

		m_dwNextUniqueID = (DWORD)GetItemValueI(TDL_NEXTUNIQUEID);

		if (m_dwNextUniqueID <= 0)
			m_dwNextUniqueID = 1; // always > 0

		ClearHandleMap();
	}

	return bResult;
}

void CTaskFile::SetDisablePasswordPrompting()
{
	VERIFY(SetItemValue(TDL_DISABLEPASSWORDPROMPTING, (int)TRUE));
}

BOOL CTaskFile::IsPasswordPromptingDisabled() const
{
	return (GetItemValueI(TDL_DISABLEPASSWORDPROMPTING) != FALSE);
}

BOOL CTaskFile::Load(LPCTSTR szFilePath, IXmlParse* pCallback, BOOL bDecrypt)
{
	BOOL bRes = CXmlFileEx::Load(szFilePath, TDL_ROOT, pCallback, bDecrypt);
	
	if (bRes)
	{
		m_dwNextUniqueID = (DWORD)GetItemValueI(TDL_NEXTUNIQUEID);
		
		if (m_dwNextUniqueID <= 0)
			m_dwNextUniqueID = 1; // always > 0
		
		ClearHandleMap();
		CleanUp();
	}
	
	return bRes;
}

BOOL CTaskFile::LoadContent(const CString& sContent)
{
	if (CXmlFileEx::LoadContent(sContent, TDL_ROOT))
	{
		ClearHandleMap();
		return TRUE;
	}

	return FALSE;
}

BOOL CTaskFile::LoadHeader(LPCTSTR szFilePath)
{
	CTFHeaderParse tfhp;

	return Load(szFilePath, &tfhp, FALSE);
}

void CTaskFile::SetHeader(const TASKFILE_HEADER& header)
{
	// Note: TASKFILE_HEADER::nFileFormat is always set to 
	// TDL_FILEFORMAT_CURRENT when saving so we ignore it here

	if (!header.sXmlHeader.IsEmpty())
		VERIFY(SetXmlHeader(header.sXmlHeader));

	if (!header.sXslHeader.IsEmpty())
		VERIFY(SetXslHeader(header.sXslHeader));

	// add other taskfile related stuff
	if (!header.sProjectName.IsEmpty())
		VERIFY(SetProjectName(header.sProjectName));

	if (header.bArchive != -1)
		VERIFY(SetArchive(header.bArchive));

	if (header.dtEarliestDue != 0)
		VERIFY(SetEarliestTaskDueDate(header.dtEarliestDue));

	if (header.dwNextID > 0)
		VERIFY(SetNextUniqueID(header.dwNextID));
	else 
		VERIFY(SetNextUniqueID(m_dwNextUniqueID));

	if (!header.sFilePath.IsEmpty())
		SetFilePath(header.sFilePath);

	if (header.nFileVersion != -1)
		VERIFY(SetFileVersion(header.nFileVersion));
}

void CTaskFile::GetHeader(TASKFILE_HEADER& header) const
{
	header.sXmlHeader = m_sXmlHeader;
	header.sXslHeader = GetXslHeader();
	header.sProjectName = GetProjectName();
	header.sFilePath = GetItemValue(TDL_FILENAME);
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
	BOOL bResult = CXmlFileEx::LoadEx(TDL_ROOT, pCallback);

	if (bResult && !IsEncrypted())
	{
		// initialize 
		m_dwNextUniqueID = (DWORD)GetItemValueI(TDL_NEXTUNIQUEID);

		if (m_dwNextUniqueID <= 0)
			m_dwNextUniqueID = 1; // always > 0

		ClearHandleMap();
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

BOOL CTaskFile::SaveEx()
{
	SetItemValue(TDL_APPVER, FileMisc::GetAppVersion());
	SetItemValue(TDL_FILEFORMAT, TDL_FILEFORMAT_CURRENT);
	
	return CXmlFileEx::SaveEx();
}

BOOL CTaskFile::CopyFrom(const CTaskFile& tasks)
{
	CXmlFileEx::CopyFrom(tasks);

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
	// TODO

	SetXmlHeader(DEFAULT_UNICODE_HEADER);
	
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
	CXmlFileEx::Reset();

	ClearHandleMap();

	m_dwNextUniqueID = 1;
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
	{
		ASSERT(0);
		return FALSE;
	}

	// simple attributes
	pTasksDest->SetTaskColor(hTaskDest, pTasksSrc->GetTaskColor(hTaskSrc));
	pTasksDest->SetTaskPriority(hTaskDest, pTasksSrc->GetTaskPriority(hTaskSrc, FALSE));
	pTasksDest->SetTaskPercentDone(hTaskDest, pTasksSrc->GetTaskPercentDone(hTaskSrc, FALSE));
	pTasksDest->SetTaskLastModified(hTaskDest, pTasksSrc->GetTaskLastModified(hTaskSrc));
	pTasksDest->SetTaskDoneDate(hTaskDest, pTasksSrc->GetTaskDoneDate(hTaskSrc));
	pTasksDest->SetTaskDueDate(hTaskDest, pTasksSrc->GetTaskDueDate(hTaskSrc));
	pTasksDest->SetTaskStartDate(hTaskDest, pTasksSrc->GetTaskStartDate(hTaskSrc));
	pTasksDest->SetTaskFlag(hTaskDest, pTasksSrc->IsTaskFlagged(hTaskSrc));
	
	// string attributes
	COPYSTRATTRIB(pTasksSrc, hTaskSrc, GetTaskComments, pTasksDest, hTaskDest, SetTaskComments);
	COPYSTRATTRIB(pTasksSrc, hTaskSrc, GetTaskAllocatedTo, pTasksDest, hTaskDest, SetTaskAllocatedTo);
	COPYSTRATTRIB(pTasksSrc, hTaskSrc, GetTaskAllocatedBy, pTasksDest, hTaskDest, SetTaskAllocatedBy);
	COPYSTRATTRIB(pTasksSrc, hTaskSrc, GetTaskCategory, pTasksDest, hTaskDest, SetTaskCategory);
	COPYSTRATTRIB(pTasksSrc, hTaskSrc, GetTaskStatus, pTasksDest, hTaskDest, SetTaskStatus);
	COPYSTRATTRIB(pTasksSrc, hTaskSrc, GetTaskFileLinkPath, pTasksDest, hTaskDest, SetTaskFileLinkPath);

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
		
		COPYSTRATTRIB(pTL3Src, hTaskSrc, GetTaskExternalID, pTL3Dest, hTaskDest, SetTaskExternalID);
		pTL3Dest->SetTaskRisk(hTaskDest, pTL3Src->GetTaskRisk(hTaskSrc, FALSE));
	
		// ---------------------------------------------------------------------------

		const ITaskList4* pTL4Src = GetITLInterface<ITaskList4>(pTasksSrc, IID_TASKLIST4);
		ITaskList4* pTL4Dest = GetITLInterface<ITaskList4>(pTasksDest, IID_TASKLIST4);
		
		if (!(pTL4Src && pTL4Dest))
			break;
		
		COPYSTRATTRIB(pTL4Src, hTaskSrc, GetTaskDependency, pTL4Dest, hTaskDest, SetTaskDependency);
		pTL4Dest->SetTaskCost(hTaskDest, pTL4Src->GetTaskCost(hTaskSrc, FALSE));
		
		// ---------------------------------------------------------------------------

		const ITaskList5* pTL5Src = GetITLInterface<ITaskList5>(pTasksSrc, IID_TASKLIST5);
		ITaskList5* pTL5Dest = GetITLInterface<ITaskList5>(pTasksDest, IID_TASKLIST5);
		
		if (!(pTL5Src && pTL5Dest))
			break;
		
		int nNumCat = pTL5Src->GetTaskCategoryCount(hTaskSrc);
		
		// First value already added by default
		for (int nCat = 1; nCat < nNumCat; nCat++)
			pTL5Dest->AddTaskCategory(hTaskDest, pTL5Src->GetTaskCategory(hTaskSrc, nCat));
		
		// ---------------------------------------------------------------------------

		const ITaskList6* pTL6Src = GetITLInterface<ITaskList6>(pTasksSrc, IID_TASKLIST6);
		ITaskList6* pTL6Dest = GetITLInterface<ITaskList6>(pTasksDest, IID_TASKLIST6);
		
		if (!(pTL6Src && pTL6Dest))
			break;
		
		COPYSTRATTRIB(pTL6Src, hTaskSrc, GetTaskVersion, pTL6Dest, hTaskDest, SetTaskVersion);
		
		// ---------------------------------------------------------------------------

		const ITaskList7* pTL7Src = GetITLInterface<ITaskList7>(pTasksSrc, IID_TASKLIST7);
		ITaskList7* pTL7Dest = GetITLInterface<ITaskList7>(pTasksDest, IID_TASKLIST7);
		
		if (!(pTL7Src && pTL7Dest))
			break;
		
		int nNumDepends = pTL7Src->GetTaskDependencyCount(hTaskSrc);
		
		for (int nDepend = 0; nDepend < nNumDepends; nDepend++)
			pTL7Dest->AddTaskDependency(hTaskDest, pTL7Src->GetTaskDependency(hTaskSrc, nDepend));
		
		int nNumAllocTo = pTL7Src->GetTaskAllocatedToCount(hTaskSrc);
		
		// First value already added by default
		for (int nAlloc = 1; nAlloc < nNumAllocTo; nAlloc++)
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
		
		COPYSTRATTRIB(pTL11Src, hTaskSrc, GetTaskIcon, pTL11Dest, hTaskDest, SetTaskIcon);
		
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

		TDC_REGULARITY nRegularity;
		TDC_RECURREUSEOPTION nReuse;
		int nNumOccur;
		DWORD dwSpecific1, dwSpecific2;
		TDC_RECURFROMOPTION nRecalcFrom;
		
		if (pTL13Src->GetTaskRecurrence(hTaskSrc, nRegularity, dwSpecific1, dwSpecific2, nRecalcFrom, nReuse, nNumOccur))
			pTL13Dest->SetTaskRecurrence(hTaskDest, nRegularity, dwSpecific1, dwSpecific2, nRecalcFrom, nReuse, nNumOccur);
		
		// ---------------------------------------------------------------------------

		const ITaskList14* pTL14Src = GetITLInterface<ITaskList14>(pTasksSrc, IID_TASKLIST14);
		ITaskList14* pTL14Dest = GetITLInterface<ITaskList14>(pTasksDest, IID_TASKLIST14);
		
		if (!(pTL14Src && pTL14Dest))
			break;

		int nNumFile = pTL14Src->GetTaskFileLinkCount(hTaskSrc);
		
		// First value already added by default
		for (int nFile = 1; nFile < nNumFile; nFile++)
			pTL14Dest->AddTaskFileLink(hTaskDest, pTL14Src->GetTaskFileLink(hTaskSrc, nFile));

		// ---------------------------------------------------------------------------

		const ITaskList15* pTL15Src = GetITLInterface<ITaskList15>(pTasksSrc, IID_TASKLIST15);
		ITaskList15* pTL15Dest = GetITLInterface<ITaskList15>(pTasksDest, IID_TASKLIST15);
		
		if (!(pTL15Src && pTL15Dest))
			break;

		// Note: we don't deliberately don't expose SetTaskReferenceID to clients
		DWORD dwRefID = pTL15Src->GetTaskReferenceID(hTaskSrc);

		if (dwRefID)
			pTL15Dest->SetTaskAttribute(hTaskDest, TDL_TASKREFID, Misc::Format(dwRefID));

		// ---------------------------------------------------------------------------

		const ITaskList16* pTL16Src = GetITLInterface<ITaskList16>(pTasksSrc, IID_TASKLIST16);
		ITaskList16* pTL16Dest = GetITLInterface<ITaskList16>(pTasksDest, IID_TASKLIST16);
		
		if (!(pTL16Src && pTL16Dest))
			break;

		pTL16Dest->SetTaskLock(hTaskDest, pTL16Src->IsTaskLocked(hTaskSrc, false));

		if (pTL16Src->GetTaskLastModified64(hTaskSrc, tDate))
			pTL16Dest->SetTaskLastModified64(hTaskDest, tDate);
		
		pTL16Dest->SetTaskLastModifiedBy(hTaskDest, pTL16Src->GetTaskLastModifiedBy(hTaskSrc));
		
		int nRemainingOccur;
		bool bPreserveComments;

		if (pTL16Src->GetTaskRecurrence(hTaskSrc, nRegularity, dwSpecific1, dwSpecific2, 
										nRecalcFrom, nReuse, nNumOccur, nRemainingOccur, bPreserveComments))
		{
			pTL16Dest->SetTaskRecurrence(hTaskDest, nRegularity, dwSpecific1, dwSpecific2, 
										nRecalcFrom, nReuse, nNumOccur, nRemainingOccur, bPreserveComments);
		}

		// ---------------------------------------------------------------------------

		const ITaskList17* pTL17Src = GetITLInterface<ITaskList17>(pTasksSrc, IID_TASKLIST17);
		ITaskList17* pTL17Dest = GetITLInterface<ITaskList17>(pTasksDest, IID_TASKLIST17);
		
		if (!(pTL17Src && pTL17Dest))
			break;

		bool bCostIsRate;
		double dCost = pTL17Src->GetTaskCost(hTaskSrc, false, bCostIsRate);

		pTL17Dest->SetTaskCost(hTaskDest, dCost, bCostIsRate);

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

		DWORD dwTaskID = pXITask->GetItemValueI(TDL_TASKID);
		ASSERT (dwTaskID);
		
		m_mapHandles[dwTaskID] = (HTASKITEM)pXITask;

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
		
		DWORD dwTaskID = pXITask->GetItemValueI(TDL_TASKID);
		ASSERT (dwTaskID);

		m_mapHandles.RemoveKey(dwTaskID);

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
	return static_cast<CXmlItem*>(hTask);
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

int CTaskFile::SetSelectedTaskIDs(const CDWordArray& aSelTaskIDs, BOOL bSelect)
{
	int nSel = aSelTaskIDs.GetSize(), nNumModified = 0;

	while (nSel--)
	{
		if (SelectTask(aSelTaskIDs[nSel], bSelect))
			nNumModified++;
	}

	return nNumModified;
}

BOOL CTaskFile::SelectTask(DWORD dwTaskID, BOOL bSelect)
{
	ASSERT(dwTaskID);

	return SelectTask(FindTask(dwTaskID), bSelect);
}

BOOL CTaskFile::SelectTask(HTASKITEM hTask, BOOL bSelect)
{
	if (!hTask)
	{
		ASSERT(0);
		return FALSE;
	}

	return (SetTaskMetaData(hTask, METADATA_SELECTED, (bSelect ? _T("1") : NULL)) ? TRUE : FALSE);
}

BOOL CTaskFile::IsTaskSelected(DWORD dwTaskID) const
{
	ASSERT(dwTaskID);

	return IsTaskSelected(FindTask(dwTaskID));
}

BOOL CTaskFile::IsTaskSelected(HTASKITEM hTask) const
{
	if (!hTask)
	{
		ASSERT(0);
		return FALSE;
	}

	LPCTSTR szSelected = GetTaskMetaData(hTask, METADATA_SELECTED);

	return (szSelected && (szSelected[0] == '1'));
}

int CTaskFile::GetSelectedTaskIDs(CDWordArray& aTaskIDs) const
{
	aTaskIDs.RemoveAll();

	return GetSelectedTaskIDs(GetFirstTask(), aTaskIDs);
}

int CTaskFile::GetSelectedTaskIDs(HTASKITEM hTask, CDWordArray& aTaskIDs) const
{
	if (hTask)
	{
		if (IsTaskSelected(hTask))
			aTaskIDs.Add(GetTaskID(hTask));

		// siblings
		GetSelectedTaskIDs(GetNextTask(hTask), aTaskIDs);

		// children
		GetSelectedTaskIDs(GetFirstTask(hTask), aTaskIDs);
	}

	return aTaskIDs.GetSize();
}

void CTaskFile::RemoveNonSelectedTasks()
{
	RemoveNonSelectedTasks(GetFirstTask());
}

void CTaskFile::RemoveNonSelectedTasks(HTASKITEM hTask)
{
	if (hTask)
	{
		// siblings first ie. before we might delete it
		RemoveNonSelectedTasks(GetNextTask(hTask));

		if (!IsTaskSelected(hTask))
		{
			DeleteTask(hTask); // will delete children
			return;
		}

		// check children
		RemoveNonSelectedTasks(GetFirstTask(hTask));
	}
}

BOOL CTaskFile::SetAttributeVisibility(const TDCCOLEDITVISIBILITY& vis)
{
	// delete old visibility settings
	DeleteItem(TDL_ATTRIBVIS);

	CXmlItem *pXIVis = AddItem(TDL_ATTRIBVIS, EMPTY_STR, XIT_ELEMENT);
	ASSERT(pXIVis);

	// columns
	const CTDCColumnIDMap& mapCols = vis.GetVisibleColumns();
	POSITION pos = mapCols.GetStartPosition();
	
	while (pos)
	{
		TDC_COLUMN nColID = mapCols.GetNext(pos);
		pXIVis->AddItem(TDL_ATTRIBVISCOL, nColID, XIT_ELEMENT);
	}

	TDL_SHOWATTRIB nShow = vis.GetShowFields();
	pXIVis->AddItem(TDL_ATTRIBVISSHOWFIELDS, nShow);

	if (nShow == TDLSA_ANY)
	{
		// edit fields
		const CTDCAttributeMap& mapEdit = vis.GetVisibleEditFields();
		pos = mapEdit.GetStartPosition();

		while (pos)
		{
			TDC_ATTRIBUTE nAttrib = mapEdit.GetNext(pos);
			pXIVis->AddItem(TDL_ATTRIBVISEDIT, nAttrib, XIT_ELEMENT);
		}
	}

	return TRUE;
}

BOOL CTaskFile::GetAttributeVisibility(TDCCOLEDITVISIBILITY& vis) const
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
		mapCols.Add((TDC_COLUMN)pXICol->GetValueI());
		pXICol = pXICol->GetSibling();
	}

	vis.SetVisibleColumns(mapCols);
	vis.SetShowFields(TDL_SHOWATTRIB(pXIVis->GetItemValueI(TDL_ATTRIBVISSHOWFIELDS)));

	if (vis.GetShowFields() == TDLSA_ANY)
	{
		// edit fields
		const CXmlItem* pXIEdit = pXIVis->GetItem(TDL_ATTRIBVISEDIT);
		CTDCAttributeMap mapEdit;

		while (pXIEdit)
		{
			mapEdit.Add((TDC_ATTRIBUTE)pXIEdit->GetValueI());
			pXIEdit = pXIEdit->GetSibling();
		}
		vis.SetVisibleEditFields(mapEdit);
	}

	return TRUE;
}

BOOL CTaskFile::SetAttributeVisibility(const TDCCOLEDITFILTERVISIBILITY& vis)
{
	if (!SetAttributeVisibility((const TDCCOLEDITVISIBILITY&)vis))
		return FALSE;

		// filter fields
	if (vis.GetShowFields() == TDLSA_ANY)
	{
		CXmlItem *pXIVis = GetItem(TDL_ATTRIBVIS);
		ASSERT(pXIVis);

		const CTDCAttributeMap& mapFilter = vis.GetVisibleFilterFields();
		POSITION pos = mapFilter.GetStartPosition();

		while (pos)
		{
			TDC_ATTRIBUTE nAttrib = mapFilter.GetNext(pos);
			pXIVis->AddItem(TDL_ATTRIBVISFILTER, nAttrib, XIT_ELEMENT);
		}
	}

	return TRUE;
}

BOOL CTaskFile::GetAttributeVisibility(TDCCOLEDITFILTERVISIBILITY& vis) const
{
	if (!GetAttributeVisibility((TDCCOLEDITVISIBILITY&)vis))
		return FALSE;

	// filter fields
	if (vis.GetShowFields() == TDLSA_ANY)
	{
		const CXmlItem *pXIVis = GetItem(TDL_ATTRIBVIS);
		ASSERT(pXIVis);

		const CXmlItem* pXIFilter = pXIVis->GetItem(TDL_ATTRIBVISFILTER);
		CTDCAttributeMap mapFilter;

		while (pXIFilter)
		{
			mapFilter.Add((TDC_ATTRIBUTE)pXIFilter->GetValueI());
			pXIFilter = pXIFilter->GetSibling();
		}

		vis.SetVisibleFilterFields(mapFilter);
	}

	return TRUE;
}

BOOL CTaskFile::SetCustomAttributeDefs(const CTDCCustomAttribDefinitionArray& aAttribs)
{
	// delete old attribDef
	if (!DeleteItem(TDL_CUSTOMATTRIBDEFS))
		DeleteItem(TDL_CUSTOMATTRIBDEFS_DEP);

	ASSERT(GetCustomAttribDefs() == NULL);

	int nNumAttrib = aAttribs.GetSize();

	for (int nAttrib = 0; nAttrib < nNumAttrib; nAttrib++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aAttribs.GetData()[nAttrib];

		ASSERT(!attribDef.sUniqueID.IsEmpty());
		ASSERT(!attribDef.sLabel.IsEmpty() || !attribDef.sColumnTitle.IsEmpty());

		CXmlItem* pXIAttribDef = AddItem(TDL_CUSTOMATTRIBDEFS);

		pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBID, Misc::ToUpper(attribDef.sUniqueID));
		pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBLABEL, attribDef.sLabel);
		pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBCOLTITLE, attribDef.sColumnTitle);
		pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBTYPE, (int)attribDef.GetAttributeType());
		pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBCOLALIGN, (int)attribDef.nTextAlignment);
		pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBFEATURES, (int)attribDef.dwFeatures);
		pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBENABLED, (int)attribDef.bEnabled);
		
		// format auto list data to follow default list data
		if (attribDef.IsList())
		{
			ASSERT(!attribDef.Calculation().IsValid(FALSE));

			pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBLISTDATA, attribDef.EncodeListData());
		}
		else if (attribDef.IsDataType(TDCCA_CALCULATION))
		{
			ASSERT(attribDef.Calculation().IsValid());

			const TDCCUSTOMATTRIBUTECALCULATION& calc = attribDef.Calculation();

			if (calc.IsFirstOperandCustom())
			{
				pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBCALCFIRSTOPERAND, calc.opFirst.sCustAttribID);
			}
			else
			{
				ASSERT(!TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(calc.opFirst.nAttribID));
				pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBCALCFIRSTOPERAND, calc.opFirst.nAttribID);
			}

			pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBCALCOPERATOR, calc.nOperator);

			if (calc.IsSecondOperandCustom())
			{
				pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBCALCSECONDOPERANDATTRIB, calc.opSecond.sCustAttribID);
			}
			else if (calc.opSecond.nAttribID != TDCA_NONE)
			{
				ASSERT(!TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(calc.opSecond.nAttribID));
				pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBCALCSECONDOPERANDATTRIB, calc.opSecond.nAttribID);
			}
			else
			{
				pXIAttribDef->SetItemValue(TDL_CUSTOMATTRIBCALCSECONDOPERANDVALUE, calc.dSecondOperandValue);
			}
		}
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
			attribDef.bEnabled = _ttoi(pTasks10->GetCustomAttributeValue(nCustom, TDL_CUSTOMATTRIBENABLED));
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
		CString sAttribDefID(pXIAttribDef->GetItemValue(TDL_CUSTOMATTRIBID));

		if (sCustID.CompareNoCase(sAttribDefID) == 0)
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
		{
			attribDef.DecodeListData(pXIAttribDef->GetItemValue(TDL_CUSTOMATTRIBLISTDATA));

			// Add 'Filterable' feature first time only
			if (GetFileFormat() < TDL_FILEFORMAT_LOCKING)
				attribDef.dwFeatures |= TDCCAF_FILTER;
		}
		else if (attribDef.IsDataType(TDCCA_CALCULATION))
		{
			CString sFirstOp = pXIAttribDef->GetItemValue(TDL_CUSTOMATTRIBCALCFIRSTOPERAND);
			CString sOperator = pXIAttribDef->GetItemValue(TDL_CUSTOMATTRIBCALCOPERATOR);
			CString sSecondOpAttrib = pXIAttribDef->GetItemValue(TDL_CUSTOMATTRIBCALCSECONDOPERANDATTRIB);
			CString sSecondOpValue = pXIAttribDef->GetItemValue(TDL_CUSTOMATTRIBCALCSECONDOPERANDVALUE);

			TDCCUSTOMATTRIBUTECALCULATION calc;

			if (Misc::IsNumber(sFirstOp))
			{
				calc.opFirst.nAttribID = (TDC_ATTRIBUTE)_ttoi(sFirstOp);
			}
			else if (!sFirstOp.IsEmpty())
			{
				calc.opFirst.nAttribID = TDCA_CUSTOMATTRIB;
				calc.opFirst.sCustAttribID = sFirstOp;
			}

			if (sOperator.IsEmpty())
				calc.nOperator = TDCCAC_ADD;
			else
				calc.nOperator = (TDCCA_CALC_OPERATOR)_ttoi(sOperator);

			if (Misc::IsNumber(sSecondOpAttrib))
			{
				calc.opSecond.nAttribID = (TDC_ATTRIBUTE)_ttoi(sSecondOpAttrib);
			}
			else if (!sSecondOpAttrib.IsEmpty())
			{
				ASSERT(sSecondOpValue.IsEmpty());

				calc.opSecond.nAttribID = TDCA_CUSTOMATTRIB;
				calc.opSecond.sCustAttribID = sSecondOpAttrib;
				calc.dSecondOperandValue = 0.0;
			}
			else
			{
				calc.opSecond.nAttribID = TDCA_NONE;
				calc.opSecond.sCustAttribID.Empty();
				calc.dSecondOperandValue = _ttof(sSecondOpValue); // defaults to 0.0
			}

			VERIFY(attribDef.SetCalculation(calc));
		}

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
	pXINewAttribDef->SetItemValue(TDL_CUSTOMATTRIBFEATURES, (TDCCAF_SORT | (bList ? TDCCAF_FILTER : 0)));

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
		return EMPTY_STR;

	const CXmlItem* pXIAttribDef = GetCustomAttribDefs(nIndex);

	if (pXIAttribDef)
		return pXIAttribDef->GetItemValue(szItem);

	// else
	ASSERT(0);
	return EMPTY_STR;
}

LPCTSTR CTaskFile::GetMetaData(LPCTSTR szKey) const
{
	if (Misc::IsEmpty(szKey))
	{
		ASSERT(0);
		return EMPTY_STR;
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

LPCTSTR CTaskFile::GetAttributeItem(LPCTSTR szAttrib, int nIndex) const
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

BOOL CTaskFile::SetEarliestTaskDueDate(const COleDateTime& date)
{
	return (NULL != SetItemValue(TDL_EARLIESTDUEDATE, date));
}

BOOL CTaskFile::GetEarliestTaskDueDate(COleDateTime& date) const
{
	const CXmlItem* pXItem = GetItem(TDL_EARLIESTDUEDATE);

	if (!pXItem)
		return FALSE;

	date = pXItem->GetValueF();
	return TRUE;
}

BOOL CTaskFile::GetEarliestTaskStartDate(COleDateTime& date) const
{
	date = GetEarliestTaskStartDate(GetFirstTask());

	return CDateHelper::IsDateSet(date);
}

BOOL CTaskFile::OffsetDates(int nNumDays)
{
	if (!OffsetTaskDates(GetFirstTask(), nNumDays))
		return FALSE;

	COleDateTime date;
		
	if (GetEarliestTaskDueDate(date) && OffsetDate(date, nNumDays))
		SetEarliestTaskDueDate(date);

	date = GetLastModifiedOle();

	if (OffsetDate(date, nNumDays))
		SetLastModified(date);

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

BOOL CTaskFile::RemoveFromSourceControl()
{
	return DeleteItem(TDL_CHECKEDOUTTO);
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

void CTaskFile::SetFilePath(LPCTSTR szFilePath)
{
	CStdioFileEx::SetFilePath(szFilePath);

	if (!Misc::IsEmpty(szFilePath))
		SetItemValue(TDL_FILENAME, FileMisc::GetFileNameFromPath(szFilePath));
	else
		DeleteItem(TDL_FILENAME);
}

BOOL CTaskFile::SetLastModified(const COleDateTime& tLastMod)
{
	if (SetItemValue(TDL_LASTMOD, tLastMod))
		return (NULL != SetItemValue(TDL_LASTMODSTRING, FormatDate(tLastMod)));

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

BOOL CTaskFile::SetReportDetails(LPCTSTR szTitle, const COleDateTime& date)
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
			bRes &= (NULL != AddItem(TDL_REPORTDATEOLE, date));

		// date string
		CString sDate = CDateHelper::FormatDate(date);
		pXItem = GetItem(TDL_REPORTDATE);

		if (pXItem)
			pXItem->SetValue(sDate);
		else
			bRes &= (NULL != AddItem(TDL_REPORTDATE, sDate));
	}

	return bRes;
}

void CTaskFile::SetAvailableAttributes(const CTDCAttributeMap& mapAttrib)
{
	m_mapReadableAttrib.Copy(mapAttrib);
}

bool CTaskFile::IsAttributeAvailable(TDC_ATTRIBUTE nAttrib) const
{
	if (!m_mapReadableAttrib.GetCount() || m_mapReadableAttrib.Has(TDCA_ALL))
		return (nAttrib != TDCA_NONE);

	if (m_mapReadableAttrib.Has(TDCA_NONE))
		return (nAttrib == TDCA_NONE);

	return (m_mapReadableAttrib.Has(nAttrib) != FALSE);
}

bool CTaskFile::SetTaskCost(HTASKITEM hTask, double dCost, bool bIsRate)
{
	return SetTaskAttribute(hTask, TDL_TASKCOST, TDCCOST::Format(dCost, (bIsRate ? TRUE : FALSE)));
}

double CTaskFile::GetTaskCost(HTASKITEM hTask, bool bCalc, bool& bIsRate) const
{
	if (bCalc)
	{
		bIsRate = false;
		return GetTaskCost(hTask, bCalc);
	}

	// else
	CString sCost;

	if (!GetTaskAttribute(hTask, TDL_TASKCOST, sCost))
		return false;

	TDCCOST cost(sCost);

	bIsRate = (cost.bIsRate != FALSE);
	return cost.dAmount;
}

BOOL CTaskFile::SetTaskAttributes(HTASKITEM hTask, const TODOITEM& tdi)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, FALSE);

	SetTaskReferenceID(hTask, tdi.dwTaskRefID);

	// set string items via lower-level call to prevent string copying
	SetTaskString(hTask, TDL_TASKTITLE, tdi.sTitle);
	
	// Save attributes _only_ if this is a 'real' task
	if (tdi.dwTaskRefID == 0)
	{
		if (!tdi.sComments.IsEmpty())
			SetTaskString(hTask, TDL_TASKCOMMENTS, tdi.sComments, XIT_ELEMENT);

		SetTaskCustomComments(hTask, tdi.customComments, tdi.cfComments);

		if (!tdi.sAllocBy.IsEmpty())
			SetTaskString(hTask, TDL_TASKALLOCBY, tdi.sAllocBy);
		
		if (!tdi.sStatus.IsEmpty())
			SetTaskString(hTask, TDL_TASKSTATUS, tdi.sStatus);
		
		if (!tdi.sVersion.IsEmpty())
			SetTaskString(hTask, TDL_TASKVERSION, tdi.sVersion);
		
		if (!tdi.sCreatedBy.IsEmpty())
			SetTaskString(hTask, TDL_TASKCREATEDBY, tdi.sCreatedBy);
		
		if (!tdi.sExternalID.IsEmpty())
			SetTaskString(hTask, TDL_TASKEXTERNALID, tdi.sExternalID);
		
		if (!tdi.sIcon.IsEmpty())
		{
			ASSERT(tdi.sIcon != _T("-1")); // Historical bug
			SetTaskString(hTask, TDL_TASKICON, tdi.sIcon);
		}
		
		// rest of non-string attributes
		SetTaskPriority(hTask, tdi.nPriority);
		SetTaskRisk(hTask, tdi.nRisk);
		
		if (tdi.bFlagged)
			SetTaskFlag(hTask, tdi.bFlagged != FALSE);
		
		if (tdi.bLocked)
			SetTaskLock(hTask, tdi.bLocked != FALSE);

		if (tdi.IsRecurring())
			SetTaskRecurrence(hTask, tdi.trRecurrence);
		
		if (tdi.aAllocTo.GetSize())
			SetTaskAllocatedTo(hTask, tdi.aAllocTo);
		
		if (tdi.aCategories.GetSize())
			SetTaskCategories(hTask, tdi.aCategories);
		
		if (tdi.aTags.GetSize())
			SetTaskTags(hTask, tdi.aTags);
		
		if (tdi.aDependencies.GetSize())
			SetTaskDependencies(hTask, tdi.aDependencies);
			
		if (tdi.aFileLinks.GetSize())
			SetTaskFileLinks(hTask, tdi.aFileLinks);
		
		if ((tdi.cost.dAmount != 0) || tdi.cost.bIsRate)
			SetTaskCost(hTask, tdi.cost.dAmount, (tdi.cost.bIsRate != FALSE));
		
		if ((tdi.timeEstimate.dAmount > 0) || (tdi.timeEstimate.nUnits != TDCU_HOURS))
			SetTaskTimeEstimate(hTask, tdi.timeEstimate.dAmount, tdi.timeEstimate.nUnits);
		
		if ((tdi.timeSpent.dAmount != 0.0) || (tdi.timeSpent.nUnits != TDCU_HOURS))
			SetTaskTimeSpent(hTask, tdi.timeSpent.dAmount, tdi.timeSpent.nUnits);
		
		// done date and percent
		if (tdi.IsDone())
		{
			SetTaskDoneDate(hTask, tdi.dateDone);
			SetTaskPercentDone(hTask, 100);
		}
		else
		{
			SetTaskPercentDone(hTask, (unsigned char)min(99, tdi.nPercentDone));
		}

		if (tdi.HasDue())
			SetTaskDueDate(hTask, tdi.dateDue);
		
		if (tdi.HasStart())
			SetTaskStartDate(hTask, tdi.dateStart);
		
		if (tdi.HasCreation())
			SetTaskCreationDate(hTask, tdi.dateCreated);
		
		if (tdi.HasLastMod())
			SetTaskLastModified(hTask, tdi.dateLastMod, tdi.sLastModifiedBy);
		
		if (tdi.color)
			SetTaskColor(hTask, tdi.color);

		// meta data
		SetTaskMetaData(hTask, tdi.GetMetaData());

		// custom data
		SetTaskCustomAttributeData(hTask, tdi.GetCustomAttributeValues());
	}

	return TRUE;
}

BOOL CTaskFile::GetTaskAttributes(HTASKITEM hTask, TODOITEM& tdi) const
{
	return GetTaskAttributes(hTask, tdi, TRUE); // overwrite
}

BOOL CTaskFile::MergeTaskAttributes(HTASKITEM hTask, TODOITEM& tdi) const
{
	return GetTaskAttributes(hTask, tdi, FALSE); // merge
}

BOOL CTaskFile::GetTaskAttributes(HTASKITEM hTask, TODOITEM& tdi, BOOL bOverwrite) const
{
	ASSERT(m_mapReadableAttrib.GetCount() == 0);

	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, FALSE);

	tdi.dwTaskRefID = GetTaskReferenceID(hTask);

	// Load attributes _only_ if this is a 'real' task
	if (tdi.dwTaskRefID == 0)
	{
#define WANTATTRIB(att) (bOverwrite || TaskHasAttribute(hTask, att))
#define GETATTRIB(att, expr) if WANTATTRIB(att) expr

		// Call GetTaskString wherever possible to avoid string copying
		GETATTRIB(TDL_TASKTITLE,				tdi.sTitle = GetTaskString(hTask, TDL_TASKTITLE));
		GETATTRIB(TDL_TASKALLOCBY,				tdi.sAllocBy = GetTaskString(hTask, TDL_TASKALLOCBY));
		GETATTRIB(TDL_TASKSTATUS,				tdi.sStatus = GetTaskString(hTask, TDL_TASKSTATUS));
		GETATTRIB(TDL_TASKCREATEDBY,			tdi.sCreatedBy = GetTaskString(hTask, TDL_TASKCREATEDBY));
		GETATTRIB(TDL_TASKEXTERNALID,			tdi.sExternalID = GetTaskString(hTask, TDL_TASKEXTERNALID));
		GETATTRIB(TDL_TASKVERSION,				tdi.sVersion = GetTaskString(hTask, TDL_TASKVERSION));
		GETATTRIB(TDL_TASKICON,					tdi.sIcon = GetTaskString(hTask, TDL_TASKICON));
		GETATTRIB(TDL_TASKLASTMODBY,			tdi.sLastModifiedBy = GetTaskString(hTask, TDL_TASKLASTMODBY));

		// Historical bug
		if ((tdi.sIcon.GetLength() == 2) && (tdi.sIcon == _T("-1")))
			tdi.sIcon.Empty();
		
		GETATTRIB(TDL_TASKFLAG,					tdi.bFlagged = IsTaskFlagged(hTask, false));
		GETATTRIB(TDL_TASKLOCK,					tdi.bLocked = IsTaskLocked(hTask, false));

		GETATTRIB(TDL_TASKCOLOR,				tdi.color = (COLORREF)GetTaskColor(hTask));
		GETATTRIB(TDL_TASKPERCENTDONE,			tdi.nPercentDone = (int)GetTaskPercentDone(hTask, false));
		GETATTRIB(TDL_TASKTIMEESTIMATE,			tdi.timeEstimate.dAmount = GetTaskTimeEstimate(hTask, tdi.timeEstimate.nUnits, false));
		GETATTRIB(TDL_TASKTIMESPENT,			tdi.timeSpent.dAmount = GetTaskTimeSpent(hTask, tdi.timeSpent.nUnits, false));
		GETATTRIB(TDL_TASKPRIORITY,				tdi.nPriority = (int)GetTaskPriority(hTask, false));
		GETATTRIB(TDL_TASKRISK,					tdi.nRisk = GetTaskRisk(hTask, false));
		GETATTRIB(TDL_TASKCOST,					tdi.cost.Parse(GetTaskString(hTask, TDL_TASKCOST)));

		GETATTRIB(TDL_TASKDUEDATE,				tdi.dateDue = GetTaskDueDateOle(hTask));
		GETATTRIB(TDL_TASKSTARTDATE,			tdi.dateStart = GetTaskStartDateOle(hTask));
		GETATTRIB(TDL_TASKDONEDATE,				tdi.dateDone = GetTaskDoneDateOle(hTask));
		GETATTRIB(TDL_TASKCREATIONDATE,			tdi.dateCreated = GetTaskCreationDateOle(hTask));
		GETATTRIB(TDL_TASKLASTMOD,				tdi.dateLastMod = GetTaskLastModifiedOle(hTask));

		GETATTRIB(TDL_TASKCATEGORY,				GetTaskCategories(hTask, tdi.aCategories));
		GETATTRIB(TDL_TASKTAG,					GetTaskTags(hTask, tdi.aTags));
		GETATTRIB(TDL_TASKALLOCTO,				GetTaskAllocatedTo(hTask, tdi.aAllocTo));
		GETATTRIB(TDL_TASKRECURRENCE,			GetTaskRecurrence(hTask, tdi.trRecurrence));
		GETATTRIB(TDL_TASKDEPENDENCY,			GetTaskDependencies(hTask, tdi.aDependencies));
		GETATTRIB(TDL_TASKFILELINKPATH,			GetTaskFileLinks(hTask, tdi.aFileLinks));

		// Comments are special
		if (bOverwrite)
		{
			tdi.sComments = GetTaskString(hTask, TDL_TASKCOMMENTS);
			GetTaskCustomComments(hTask, tdi.customComments, tdi.cfComments);
		}
		else // merge
		{
			// To replace what we already have, the 'other' task must have
			// the full set of text comments, comments type and custom comments 
			// OR
			// it must have text comments and the comments type must of 'text' type
			// OR
			// 'we' have no comments and the 'other' task has a different comments type
			CString sOtherTextComments = GetTaskString(hTask, TDL_TASKCOMMENTS);

			CONTENTFORMAT cfOtherComments;
			CBinaryData otherCustomComments;
			GetTaskCustomComments(hTask, otherCustomComments, cfOtherComments);

			BOOL bOtherIsTextFormat = cfOtherComments.FormatIsText();

			if (!sOtherTextComments.IsEmpty())
			{
				if (!otherCustomComments.IsEmpty() && !bOtherIsTextFormat)
				{
					tdi.sComments = sOtherTextComments;
					tdi.cfComments = cfOtherComments;
					tdi.customComments = otherCustomComments;
				}
				else if (bOtherIsTextFormat)
				{
					tdi.sComments = sOtherTextComments;
					tdi.cfComments = cfOtherComments;
					tdi.customComments.Empty();
				}
			}
			else if (tdi.sComments.IsEmpty() &&
					 tdi.customComments.IsEmpty() &&
					 !cfOtherComments.IsEmpty() &&
					 !bOtherIsTextFormat &&
					 (cfOtherComments != tdi.cfComments))
			{
				tdi.cfComments = cfOtherComments;
			}
		}

		// custom data
		if (WANTATTRIB(TDL_TASKCUSTOMATTRIBDATA))
		{
			CTDCCustomAttributeDataMap mapData;

			GetTaskCustomAttributeData(hTask, mapData); 
			tdi.SetCustomAttributeValues(mapData);
		}

		// meta data
		if (WANTATTRIB(TDL_TASKMETADATA))
		{
			CTDCMetaDataMap mapData;

			GetTaskMetaData(hTask, mapData); 
			tdi.SetMetaData(mapData);
		}
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

	if ((m_mapHandles.GetCount() == 0) && (Root()->HasItem(TDL_TASK)))
		BuildHandleMap();

	HTASKITEM hTask = NULL;
	VERIFY(!m_mapHandles.Lookup(dwTaskID, hTask) || (hTask != NULL));

	return hTask;
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
		return TRUE; // not an error

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
	return SetTaskArray(hTask, TDL_TASKCATEGORY, aCategories, FALSE);
}

BOOL CTaskFile::SetTaskTags(HTASKITEM hTask, const CStringArray& aTags)
{
	return SetTaskArray(hTask, TDL_TASKTAG, aTags, FALSE);
}

BOOL CTaskFile::SetTaskFileLinks(HTASKITEM hTask, const CStringArray& aFiles)
{
	return SetTaskArray(hTask, TDL_TASKFILELINKPATH, aFiles, FALSE);
}

BOOL CTaskFile::SetTaskDependencies(HTASKITEM hTask, const CTDCDependencyArray& aDepends)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, FALSE);

	// delete any existing items
	DeleteTaskArray(hTask, TDL_TASKDEPENDENCY);

	// then add these
	int nCount = aDepends.GetSize();

	for (int nDepend = 0; nDepend < nCount; nDepend++)
	{
		const TDCDEPENDENCY& depend = aDepends[nDepend];

		// No need to check for duplicates because CTDCDependencyArray handles that
		CXmlItem* pXIDepend = pXITask->AddItem(TDL_TASKDEPENDENCY, depend.Format(), XIT_ELEMENT);

		// Attributes
		if (depend.nDaysLeadIn)
			pXIDepend->AddItem(TDL_TASKDEPENDENCYLEADIN, depend.nDaysLeadIn, XIT_ATTRIB);
	}

	return TRUE;
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

int CTaskFile::GetTaskCustomAttributeData(HTASKITEM hTask, CTDCCustomAttributeDataMap& mapData) const
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

BOOL CTaskFile::SetTaskCustomAttributeData(HTASKITEM hTask, const CTDCCustomAttributeDataMap& mapData)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, FALSE);

	pXITask->DeleteItem(TDL_TASKCUSTOMATTRIBDATA);

	POSITION pos = mapData.GetStartPosition();

	while (pos)
	{
		CString sTypeID;
		TDCCADATA data;
		mapData.GetNextAssoc(pos, sTypeID, data);

		SetTaskCustomAttributeData(hTask, sTypeID, data);
	}

	return TRUE;
}

BOOL CTaskFile::SetTaskCustomAttributeData(HTASKITEM hTask, const CString& sCustAttribID, const TDCCADATA& data)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, FALSE);

	return SetTaskCustomAttributeData(pXITask, sCustAttribID, data);
}

BOOL CTaskFile::SetTaskCustomAttributeData(CXmlItem* pXITask, const CString& sCustAttribID, const TDCCADATA& data)
{
	if (sCustAttribID.IsEmpty() || data.IsEmpty())
		return FALSE;

	if (!HasCustomAttribute(sCustAttribID))
	{
		ASSERT(0);
		return FALSE;
	}

	CXmlItem* pXICustData = pXITask->AddItem(TDL_TASKCUSTOMATTRIBDATA);
	ASSERT(pXICustData);

	pXICustData->AddItem(TDL_TASKCUSTOMATTRIBID, Misc::ToUpper(sCustAttribID));
	pXICustData->AddItem(TDL_TASKCUSTOMATTRIBVALUE, data.AsString());

	// add human readable format
	DWORD dwAttribType = GetCustomAttributeTypeByID(sCustAttribID);

	if (dwAttribType & TDCCA_LISTMASK)
	{
		if (dwAttribType & (TDCCA_AUTOMULTILIST | TDCCA_FIXEDMULTILIST))
		{
			if (data.IsArray())
				pXICustData->AddItem(TDL_TASKCUSTOMATTRIBDISPLAYSTRING, data.FormatAsArray());
		}
	}
	else
	{
		switch (dwAttribType & TDCCA_DATAMASK)
		{
		case TDCCA_DATE:
			pXICustData->AddItem(TDL_TASKCUSTOMATTRIBDISPLAYSTRING, data.FormatAsDate(m_bISODates, TRUE));
			break;

		case TDCCA_TIMEPERIOD:
			pXICustData->AddItem(TDL_TASKCUSTOMATTRIBDISPLAYSTRING, data.FormatAsTimePeriod());
			break;
		}
	}

	return TRUE;
}

BOOL CTaskFile::SetTaskAllocatedTo(HTASKITEM hTask, const CStringArray& aAllocTo)
{
	return SetTaskArray(hTask, TDL_TASKALLOCTO, aAllocTo, FALSE);
}

bool CTaskFile::AddTaskDependency(HTASKITEM hTask, LPCTSTR szDepends)
{
	return AddTaskDependency(hTask, szDepends, 0);
}

bool CTaskFile::AddTaskDependency(HTASKITEM hTask, unsigned long dwID)
{
	return AddTaskDependency(hTask, dwID, 0);
}

bool CTaskFile::AddTaskDependency(HTASKITEM hTask, LPCTSTR szDepends, int nDaysLeadIn)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);

	CXmlItem* pXI = pXITask->GetItem(TDL_TASKDEPENDENCY);

	// check existence before adding
	if (!pXI || !pXI->FindItem(TDL_TASKDEPENDENCY, szDepends, FALSE))
	{
		CXmlItem* pXIDepend = pXITask->AddItem(TDL_TASKDEPENDENCY, szDepends, XIT_ELEMENT);
		ASSERT(pXIDepend);

		// Attributes
		if (nDaysLeadIn)
			pXIDepend->AddItem(TDL_TASKDEPENDENCYLEADIN, nDaysLeadIn, XIT_ATTRIB);

		return true;
	}

	return false;
}

bool CTaskFile::AddTaskDependency(HTASKITEM hTask, unsigned long dwID, int nDaysLeadIn)
{
	CString sID;
	sID.Format(_T("%lu"), dwID);

	return AddTaskDependency(hTask, sID, nDaysLeadIn);
}

bool CTaskFile::AddTaskAllocatedTo(HTASKITEM hTask, LPCTSTR szAllocTo)
{
	return AddTaskArrayItem(hTask, TDL_TASKALLOCTO, szAllocTo, FALSE);
}

bool CTaskFile::AddTaskFileLink(HTASKITEM hTask, LPCTSTR szFileLink)
{
	return AddTaskArrayItem(hTask, TDL_TASKFILELINKPATH, szFileLink, FALSE);
}

bool CTaskFile::AddTaskCategory(HTASKITEM hTask, LPCTSTR szCategory)
{
	return AddTaskArrayItem(hTask, TDL_TASKCATEGORY, szCategory, FALSE);
}

bool CTaskFile::AddTaskTag(HTASKITEM hTask, LPCTSTR szTag)
{
	return AddTaskArrayItem(hTask, TDL_TASKTAG, szTag, FALSE);
}

int CTaskFile::GetTaskCategories(HTASKITEM hTask, CStringArray& aCategories) const
{
	return GetTaskArray(hTask, TDL_TASKCATEGORY, aCategories, FALSE);
}

int CTaskFile::GetTaskTags(HTASKITEM hTask, CStringArray& aTags) const
{
	return GetTaskArray(hTask, TDL_TASKTAG, aTags, FALSE);
}

int CTaskFile::GetTaskFileLinks(HTASKITEM hTask, CStringArray& aFiles) const
{
	return GetTaskArray(hTask, TDL_TASKFILELINKPATH, aFiles, FALSE);
}

int CTaskFile::GetTaskDependencies(HTASKITEM hTask, CTDCDependencyArray& aDepends) const
{
	aDepends.RemoveAll();

	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, 0);

	// first item
	const CXmlItem* pXIDepend = pXITask->GetItem(TDL_TASKDEPENDENCY);
	TDCDEPENDENCY depend;

	while (pXIDepend)
	{
		if (depend.Parse(pXIDepend->GetValue()))
		{
			depend.nDaysLeadIn = pXIDepend->GetItemValueI(TDL_TASKDEPENDENCYLEADIN);
			
			aDepends.Add(depend);
		}

		pXIDepend = pXIDepend->GetSibling();
	}

	return aDepends.GetSize();
}

int CTaskFile::GetTaskAllocatedTo(HTASKITEM hTask, CStringArray& aAllocTo) const
{
	return GetTaskArray(hTask, TDL_TASKALLOCTO, aAllocTo, FALSE);
}

///////////////////////////////////////////////////////////////////////

HTASKITEM CTaskFile::NewTask(LPCTSTR szTitle, HTASKITEM hParent)
{
	return NewTask(szTitle, hParent, 0); // Zero mean 'allocate ID'
}

HTASKITEM CTaskFile::NewTask(LPCTSTR szTitle, HTASKITEM hParent, DWORD dwID)
{
	return NewTask(szTitle, hParent, dwID, 0, TRUE);
}

// New wrapper that lets us set the parent ID if the parent task is NULL
// And optionally initialise the creation date
HTASKITEM CTaskFile::NewTask(const CString& sTitle, HTASKITEM hParent, DWORD dwID, DWORD dwParentID, BOOL bInitCreationDate)
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

	CXmlItem* pXINew = pXIParent->AddItem(TDL_TASK, NULL, XIT_ELEMENT);

	if (pXINew)
	{
		// Must set ID before adding to handle map
		if (dwID <= 0)
			dwID = m_dwNextUniqueID++;
		else
			m_dwNextUniqueID = max(m_dwNextUniqueID, dwID + 1);

		VERIFY(pXINew->SetItemValue(TDL_TASKID, (int)dwID));
		
		// Add to map
		AddTaskToMap(pXINew, FALSE, FALSE);

		// Set name, parent ID and creation date
		HTASKITEM hTask = (HTASKITEM)pXINew;
		SetTaskString(hTask, TDL_TASKTITLE, sTitle);

		if (dwParentID)
		{
			// sanity check
			ASSERT((hParent == NULL) || (GetTaskID(hParent) == dwParentID));
		
			if (hParent == NULL)
				SetTaskULong(hTask, TDL_TASKPARENTID, dwParentID);
		}

		if (bInitCreationDate)
			SetTaskCreationDate(hTask, COleDateTime::GetCurrentTime());
	}

	return (HTASKITEM)pXINew;
}

HTASKITEM CTaskFile::NewSiblingTask(const CString& sTitle, HTASKITEM hPrevSiblingTask, DWORD dwID, BOOL bInitCreationDate)
{
	if (hPrevSiblingTask == NULL)
	{
		ASSERT(0);
		return NULL;
	}
	
	CXmlItem* pXIPrevSibling = TaskFromHandle(hPrevSiblingTask);
	CXmlItem* pXINew = pXIPrevSibling->AddSibling();

	if (pXINew)
	{
		// Must set ID before adding to handle map
		if (dwID <= 0)
			dwID = m_dwNextUniqueID++;
		else
			m_dwNextUniqueID = max(m_dwNextUniqueID, dwID + 1);

		VERIFY(pXINew->SetItemValue(TDL_TASKID, (int)dwID));
		
		// Add to map
		AddTaskToMap(pXINew, FALSE, FALSE);

		// Set name, parent ID and creation date
		HTASKITEM hTask = (HTASKITEM)pXINew;
		SetTaskString(hTask, TDL_TASKTITLE, sTitle);

		if (bInitCreationDate)
			SetTaskCreationDate(hTask, COleDateTime::GetCurrentTime());
	}

	return (HTASKITEM)pXINew;
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
	const CString& sIcon = GetTaskString(hTask, TDL_TASKICON);

	// Historical bug
	if ((sIcon.GetLength() == 2) && (sIcon == _T("-1")))
		return NULLSTRING;

	// else
	return sIcon;
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
		return EMPTY_STR;
	}

	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, NULLSTRING);

	return pXITask->GetItemValue(TDL_TASKMETADATA, szKey);
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

unsigned long CTaskFile::GetCustomAttributeTypeByID(LPCTSTR szID) const
{
	const CXmlItem* pXIAttribDef = GetCustomAttributeDef(szID);
	
	if (pXIAttribDef)
		return pXIAttribDef->GetItemValueI(TDL_CUSTOMATTRIBTYPE);

	// else
	return 0;
}

LPCTSTR CTaskFile::GetTaskCustomAttributeData(HTASKITEM hTask, LPCTSTR szID, bool bDisplay) const
{
	if (bDisplay)
	{
		const CXmlItem* pXICustData = GetTaskCustomAttribute(hTask, szID);
	
		if (pXICustData)
		{
			LPCTSTR szValue = pXICustData->GetItemValue(TDL_TASKCUSTOMATTRIBDISPLAYSTRING);

			if (!Misc::IsEmpty(szValue))
				return szValue;
		}
	}

	// all else
	return GetTaskCustomAttributeData(hTask, szID);
}

// DEPRECATED
LPCTSTR CTaskFile::GetTaskCustomDateString(HTASKITEM hTask, LPCTSTR szID) const
{
	if ((GetCustomAttributeTypeByID(szID) & TDCCA_DATAMASK) == TDCCA_DATE)
		return GetTaskCustomAttributeData(hTask, szID, true);

	// else
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
	return GetTaskArrayItemValue(hTask, TDL_TASKCATEGORY, nIndex);
}

int CTaskFile::GetTaskTagCount(HTASKITEM hTask) const
{
	return GetTaskArraySize(hTask, TDL_TASKTAG);
}

LPCTSTR CTaskFile::GetTaskTag(HTASKITEM hTask, int nIndex) const
{
	return GetTaskArrayItemValue(hTask, TDL_TASKTAG, nIndex);
}

int CTaskFile::GetTaskDependencyCount(HTASKITEM hTask) const
{
	return GetTaskArraySize(hTask, TDL_TASKDEPENDENCY);
}

LPCTSTR CTaskFile::GetTaskDependency(HTASKITEM hTask, int nIndex) const
{
	return GetTaskDependency(hTask, nIndex, NULL);
}

LPCTSTR CTaskFile::GetTaskDependency(HTASKITEM hTask, int nIndex, int* pDaysLeadIn) const
{
	const CXmlItem* pXI = GetTaskArrayItem(hTask, TDL_TASKDEPENDENCY, nIndex);

	if (pDaysLeadIn)
	{
		if (pXI)
			*pDaysLeadIn = pXI->GetItemValueI(TDL_TASKDEPENDENCYLEADIN);
		else
			*pDaysLeadIn = 0;
	}

	return (pXI ? pXI->GetValue() : EMPTY_STR);
}

int CTaskFile::GetTaskAllocatedToCount(HTASKITEM hTask) const
{
	return GetTaskArraySize(hTask, TDL_TASKALLOCTO);
}

LPCTSTR CTaskFile::GetTaskAllocatedTo(HTASKITEM hTask, int nIndex) const
{
	return GetTaskArrayItemValue(hTask, TDL_TASKALLOCTO, nIndex);
}

int CTaskFile::GetTaskFileLinkCount(HTASKITEM hTask) const
{
	return GetTaskArraySize(hTask, TDL_TASKFILELINKPATH);
}

LPCTSTR CTaskFile::GetTaskFileLink(HTASKITEM hTask, int nIndex) const
{
	return GetTaskArrayItemValue(hTask, TDL_TASKFILELINKPATH, nIndex);
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
	return GetTaskString(hTask, TDL_TASKFILELINKPATH);
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
	return IsTaskFlagged(hTask, false);
}

bool CTaskFile::IsTaskFlagged(HTASKITEM hTask, bool bCalc) const
{
	if (bCalc && (GetTaskUChar(hTask, TDL_TASKCALCFLAG) > 0))
		return TRUE;

	return (GetTaskUChar(hTask, TDL_TASKFLAG) > 0);
}

LPCTSTR CTaskFile::GetFileName(bool bFullPath) const
{
	if (bFullPath)
	{
		static CString sFilePath;
		sFilePath = CXmlFile::GetFilePath();

		return sFilePath;
	}

	// else
	return GetAttribute(TDL_FILENAME);
}

bool CTaskFile::IsTaskLocked(HTASKITEM hTask, bool bCalc) const
{
	if (bCalc && (GetTaskUChar(hTask, TDL_TASKCALCLOCK) > 0))
		return TRUE;

	return (GetTaskUChar(hTask, TDL_TASKLOCK) > 0);
}

bool CTaskFile::IsTaskRecurring(HTASKITEM hTask) const
{
	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);

	return (pXITask->GetItem(TDL_TASKRECURRENCE) != NULL);
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

int CTaskFile::GetTaskPriority(HTASKITEM hTask, bool bHighest) const
{
	if (bHighest && TaskHasAttribute(hTask, TDL_TASKHIGHESTPRIORITY))
		return GetTaskInt(hTask, TDL_TASKHIGHESTPRIORITY);

	return GetTaskInt(hTask, TDL_TASKPRIORITY);
}

unsigned char CTaskFile::GetTaskPercentDone(HTASKITEM hTask, bool bCalc) const
{
	if (bCalc && TaskHasAttribute(hTask, TDL_TASKCALCCOMPLETION))
		return GetTaskUChar(hTask, TDL_TASKCALCCOMPLETION);

	return GetTaskUChar(hTask, TDL_TASKPERCENTDONE);
}

double CTaskFile::GetTaskCost(HTASKITEM hTask, bool bCalc) const
{
	if (bCalc && TaskHasAttribute(hTask, TDL_TASKCALCCOST))
		return GetTaskDouble(hTask, TDL_TASKCALCCOST);

	// else
	// Note: If someone is calling this old version and the
	// cost is actually a rate then this will return 0.0 due
	// to the leading '@' which is exactly what we want
	return GetTaskDouble(hTask, TDL_TASKCOST);
}

BOOL CTaskFile::GetTaskRecurrence(HTASKITEM hTask, TDCRECURRENCE& tr) const
{
	// NOTE: we don't restore 'tr.sRegularity' because this was always
	// just a handy human-readable string for display purposes
	TDC_REGULARITY nRegularity;
	TDC_RECURFROMOPTION nRecalcFrom; 
	TDC_RECURREUSEOPTION nReuse;

	DWORD dwSpecific1;
	DWORD dwSpecific2;
	int nNumOccur;
	int nRemainingOccur;
	bool bPreserveComments;

	if (!GetTaskRecurrence(hTask, 
							nRegularity, 
							dwSpecific1, 
							dwSpecific2, 
							nRecalcFrom, 
							nReuse,
							nNumOccur,
							nRemainingOccur,
							bPreserveComments))
	{
		return FALSE;
	}

	if (!tr.SetRegularity(nRegularity, dwSpecific1, dwSpecific2))
		return FALSE;

	if (!tr.SetOccurrenceCount(nNumOccur, nRemainingOccur))
		return FALSE;

	tr.nRecalcFrom = nRecalcFrom;
	tr.nReuse = nReuse;
	tr.bPreserveComments = (bPreserveComments ? TRUE : FALSE);

	return TRUE;
}

// ITaskList6 interface
bool CTaskFile::GetTaskRecurrence(HTASKITEM hTask, TDC_REGULARITY& nRegularity, 
									DWORD& dwSpecific1, DWORD& dwSpecific2, 
									bool& bRecalcFromDue, TDC_RECURREUSEOPTION& nReuse) const
{
	int nDummy1, nDummy2;
	TDC_RECURFROMOPTION nRecalcFrom;
	bool bDummy;

	if (GetTaskRecurrence(hTask, nRegularity, dwSpecific1, dwSpecific2, 
							nRecalcFrom, nReuse, nDummy1, nDummy2, bDummy))
	{
		bRecalcFromDue = (nRecalcFrom == TDIRO_DUEDATE);
		return true;
	}

	return false;
}

// ITaskList13 interface
bool CTaskFile::GetTaskRecurrence(HTASKITEM hTask, TDC_REGULARITY& nRegularity, 
									DWORD& dwSpecific1, DWORD& dwSpecific2, 
									TDC_RECURFROMOPTION& nRecalcFrom, TDC_RECURREUSEOPTION& nReuse, 
									int& nNumOccur) const
{
	int nDummy;
	bool bDummy;

	// Note: for backwards compatibility nNumOccur maps to the remaining occurrences
	return GetTaskRecurrence(hTask, nRegularity, dwSpecific1, dwSpecific2, 
							nRecalcFrom, nReuse, nDummy, nNumOccur, bDummy);
}

// ITaskList16 interface
bool CTaskFile::GetTaskRecurrence(HTASKITEM hTask, TDC_REGULARITY& nRegularity, 
									DWORD& dwSpecific1, DWORD& dwSpecific2, 
									TDC_RECURFROMOPTION& nRecalcFrom,  TDC_RECURREUSEOPTION& nReuse, 
									int& nNumOccur, int& nRemainingOccur, bool& bPreserveComments) const
{
	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);
	
	const CXmlItem* pXIRecur = pXITask->GetItem(TDL_TASKRECURRENCE);
	
	if (pXIRecur)
	{
		// backwards compatibility
		if (pXIRecur->HasItem(TDL_TASKRECURRENCEREG_DEP))
		{
			nRegularity = (TDC_REGULARITY)pXIRecur->GetItemValueI(TDL_TASKRECURRENCEREG_DEP);
			dwSpecific1 = pXIRecur->GetItemValueI(TDL_TASKRECURRENCESPEC1_DEP);
			dwSpecific2 = pXIRecur->GetItemValueI(TDL_TASKRECURRENCESPEC2_DEP);
		}
		else
		{
			nRegularity = (TDC_REGULARITY)pXIRecur->GetItemValueI(TDL_TASKRECURRENCEFREQ);
			dwSpecific1 = pXIRecur->GetItemValueI(TDL_TASKRECURRENCESPEC1);
			dwSpecific2 = pXIRecur->GetItemValueI(TDL_TASKRECURRENCESPEC2);
		}
		
		nReuse = (TDC_RECURREUSEOPTION)pXIRecur->GetItemValueI(TDL_TASKRECURRENCEREUSE);
		
		if (pXIRecur->HasItem(TDL_TASKRECURRENCEFROM))
		{
			nRecalcFrom = (TDC_RECURFROMOPTION)pXIRecur->GetItemValueI(TDL_TASKRECURRENCEFROM);
		}
		else // fallback
		{
			// really a boolean
			nRecalcFrom = (pXIRecur->GetItemValueI(TDL_TASKRECURRENCEFROMDUE_DEP) ? TDIRO_DUEDATE : TDIRO_DONEDATE);
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

		if (pXIRecur->HasItem(TDL_TASKRECURRENCEPRESERVECOMMENTS))
		{
			bPreserveComments = (pXIRecur->GetItemValueI(TDL_TASKRECURRENCEPRESERVECOMMENTS) != FALSE);
		}
		else
		{
			// Backward compatibility
			bPreserveComments = (nReuse == TDIRO_REUSE);
		}
	
		return true;
	}
	
	return false;
}

LPCTSTR CTaskFile::GetTaskVersion(HTASKITEM hTask) const
{
	return GetTaskString(hTask, TDL_TASKVERSION);
}

LPCTSTR CTaskFile::GetTaskLastModifiedBy(HTASKITEM hTask) const
{
	return GetTaskString(hTask, TDL_TASKLASTMODBY);
}

TDC_UNITS CTaskFile::GetTaskTimeUnits(HTASKITEM hTask, const CString& sUnitsItem) const
{
	TDC_UNITS nUnits = TDCU_HOURS;
	CString sUnits = GetTaskString(hTask, sUnitsItem);

	if (!sUnits.IsEmpty())
		nUnits = (TDC_UNITS)sUnits[0];

	return nUnits;
}

double CTaskFile::GetTaskTimeEstimate(HTASKITEM hTask, TDC_UNITS& nUnits, bool bCalc) const
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

double CTaskFile::GetTaskTimeSpent(HTASKITEM hTask, TDC_UNITS& nUnits, bool bCalc) const
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

time_t CTaskFile::GetTaskDueDate(HTASKITEM hTask, bool bCalc) const
{
	if (bCalc && TaskHasAttribute(hTask, TDL_TASKCALCDUEDATE))
		return GetTaskDate(hTask, TDL_TASKCALCDUEDATE, TRUE);

	return GetTaskDate(hTask, TDL_TASKDUEDATE, TRUE);
}

LPCTSTR CTaskFile::GetTaskDueDateString(HTASKITEM hTask, bool bCalc) const
{
	if (bCalc && TaskHasAttribute(hTask, TDL_TASKCALCDUEDATESTRING))
		return GetTaskString(hTask, TDL_TASKCALCDUEDATESTRING);

	return GetTaskString(hTask, TDL_TASKDUEDATESTRING);
}

time_t CTaskFile::GetTaskStartDate(HTASKITEM hTask, bool bCalc) const
{
	if (bCalc && TaskHasAttribute(hTask, TDL_TASKCALCSTARTDATE))
		return GetTaskDate(hTask, TDL_TASKCALCSTARTDATE, TRUE);

	return GetTaskDate(hTask, TDL_TASKSTARTDATE, TRUE);
}

LPCTSTR CTaskFile::GetTaskStartDateString(HTASKITEM hTask, bool bCalc) const
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

bool CTaskFile::GetTaskStartDate64(HTASKITEM hTask, bool bCalc, time64_t& timeT) const
{
	COleDateTime date = GetTaskStartDateOle(hTask);

	if (bCalc && TaskHasAttribute(hTask, TDL_TASKCALCSTARTDATE))
		date = GetTaskDateOle(hTask, TDL_TASKCALCSTARTDATE, TRUE);
	
	return (CDateHelper::GetTimeT64(date, timeT) != FALSE);
}

bool CTaskFile::GetTaskDueDate64(HTASKITEM hTask, bool bCalc, time64_t& timeT) const
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

COleDateTime CTaskFile::GetEarliestTaskStartDate(HTASKITEM hTask) const
{
	COleDateTime dtEarliest = CDateHelper::NullDate();

	if (hTask)
	{
		//CDateHelper::Min(dtEarliest, GetTaskCreationDateOle(hTask));
		CDateHelper::Min(dtEarliest, GetTaskStartDateOle(hTask));
		//CDateHelper::Min(dtEarliest, GetTaskDueDateOle(hTask));
		//CDateHelper::Min(dtEarliest, GetTaskDoneDateOle(hTask));
		//CDateHelper::Min(dtEarliest, GetTaskLastModifiedOle(hTask));

		// First child
		CDateHelper::Min(dtEarliest, GetEarliestTaskStartDate(GetFirstTask(hTask)));

		// First sibling
		CDateHelper::Min(dtEarliest, GetEarliestTaskStartDate(GetNextTask(hTask)));
	}

	return dtEarliest;
}

BOOL CTaskFile::OffsetTaskDates(HTASKITEM hTask, int nNumDays)
{
	BOOL bChanged = FALSE;

	if (hTask && (nNumDays > 0))
	{
		COleDateTime date = GetTaskCreationDateOle(hTask);

		if (OffsetDate(date, nNumDays))
			bChanged |= SetTaskCreationDate(hTask, date);

		date = GetTaskStartDateOle(hTask);

		if (OffsetDate(date, nNumDays))
			bChanged |= SetTaskStartDate(hTask, date);

		date = GetTaskDueDateOle(hTask);

		if (OffsetDate(date, nNumDays))
			bChanged |= SetTaskDueDate(hTask, date);

		date = GetTaskDoneDateOle(hTask);

		if (OffsetDate(date, nNumDays))
			bChanged |= SetTaskDoneDate(hTask, date);

		date = GetTaskLastModifiedOle(hTask);

		if (OffsetDate(date, nNumDays))
			bChanged |= SetTaskLastModified(hTask, date, GetTaskLastModifiedBy(hTask));

		// First child
		bChanged |= OffsetTaskDates(GetFirstTask(hTask), nNumDays);

		// Next sibling
		bChanged |= OffsetTaskDates(GetNextTask(hTask), nNumDays);
	}

	return bChanged;
}

BOOL CTaskFile::OffsetDate(COleDateTime& date, int nNumDays)
{
	if (!CDateHelper::IsDateSet(date))
		return FALSE;

	// else
	date.m_dt += nNumDays;
	return TRUE;
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
	return GetTaskULong(hTask, TDL_TASKPOSITION);
}

LPCTSTR CTaskFile::GetTaskPositionString(HTASKITEM hTask) const
{
	return GetTaskString(hTask, TDL_TASKPOSSTRING);
}

bool CTaskFile::IsTaskDone(HTASKITEM hTask) const
{
	if (GetTaskPercentDone(hTask, FALSE) == 100)
		return true;

	return (CDateHelper::IsDateSet(GetTaskDateOle(hTask, TDL_TASKDONEDATE, FALSE)) != FALSE);
}

bool CTaskFile::IsTaskDue(HTASKITEM hTask) const
{
	if (!IsTaskDone(hTask))
	{
		COleDateTime dtDue = GetTaskDateOle(hTask, TDL_TASKDUEDATE, FALSE); // date only

		if (CDateHelper::IsDateSet(dtDue))
		{
			return (dtDue.m_dt <= CDateHelper::GetDate(DHD_TODAY));
		}
	}

	return false;
}

// interface version
bool CTaskFile::TaskHasAttribute(HTASKITEM hTask, LPCTSTR szAttrib) const
{
	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);
	
	const CXmlItem* pXIAttrib = pXITask->GetItem(szAttrib);
	
	if (pXIAttrib && pXIAttrib->HasValue())
	{
		// extra checks
		if (STR_MATCH(szAttrib, TDL_TASKPRIORITY) || STR_MATCH(szAttrib, TDL_TASKRISK))
		{
			if (pXIAttrib->GetValueI() < 0)
				return false;
		}

		return true;
	}

	// some fallbacks
	if (STR_MATCH(szAttrib, TDL_TASKCOLOR))
		return (pXITask->ItemHasValue(TDL_TASKTEXTCOLOR) != FALSE);

	if (STR_MATCH(szAttrib, TDL_TASKWEBCOLOR))
		return (pXITask->ItemHasValue(TDL_TASKTEXTWEBCOLOR) != FALSE);

	if (STR_MATCH(szAttrib, TDL_TASKPARENTID))
		return (pXITask->ItemHasValue(TDL_TASKID) != FALSE);

	if (pXIAttrib)
	{
		if (STR_MATCH(szAttrib, TDL_TASKCUSTOMATTRIBDATA))
			return (pXIAttrib->HasItem(TDL_TASKCUSTOMATTRIBID) != FALSE);

		if (STR_MATCH(szAttrib, TDL_TASKRECURRENCE))
			return (pXIAttrib->HasItem(TDL_TASKRECURRENCEFREQ) != FALSE);
	}

	// all else
	return false;
}

bool CTaskFile::TaskHasAttribute(HTASKITEM hTask, TDC_ATTRIBUTE nAttrib, bool bCalc, bool bDisplay) const
{
	if (!IsAttributeAvailable(nAttrib))
		return false;

	LPCTSTR szAttrib = GetAttribTag(nAttrib, bCalc, bDisplay);

	if (Misc::IsEmpty(szAttrib))
		return false;

	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);

	if (pXITask->ItemHasValue(szAttrib))
		return true;

	// Fallback in special case
	if ((nAttrib == TDCA_PARENTID) && IsAttributeAvailable(TDCA_ID))
		return (GetTaskParent(hTask) != NULL);

	return false;
}

LPCTSTR CTaskFile::GetAttribTag(TDC_ATTRIBUTE nAttrib, bool bCalc, bool bDisplay)
{
	switch (nAttrib)
	{
	case TDCA_ALLOCBY:		return TDL_TASKALLOCBY;
	case TDCA_ALLOCTO:		return TDL_TASKALLOCTO;
	case TDCA_CATEGORY:		return TDL_TASKCATEGORY;
	case TDCA_COLOR:		return TDL_TASKCOLOR;
	case TDCA_COMMENTS:		return TDL_TASKCOMMENTS;
	case TDCA_CREATEDBY:	return TDL_TASKCREATEDBY;
	case TDCA_CUSTOMATTRIB:	return TDL_TASKCUSTOMATTRIBDATA;
	case TDCA_DEPENDENCY:	return TDL_TASKDEPENDENCY;
	case TDCA_EXTERNALID:	return TDL_TASKEXTERNALID;
	case TDCA_FILELINK:		return TDL_TASKFILELINKPATH;
	case TDCA_FLAG:			return TDL_TASKFLAG;
	case TDCA_HTMLCOMMENTS: return TDL_TASKHTMLCOMMENTS;
	case TDCA_ICON:			return TDL_TASKICON;
	case TDCA_ID:			return TDL_TASKID;
	case TDCA_LASTMODBY:	return TDL_TASKLASTMODBY;
	case TDCA_METADATA:		return TDL_TASKMETADATA;
	case TDCA_PARENTID:		return TDL_TASKPARENTID;
	case TDCA_PATH:			return TDL_TASKPATH;
	case TDCA_RECURRENCE:	return TDL_TASKRECURRENCE;
	case TDCA_STATUS:		return TDL_TASKSTATUS;
	case TDCA_TAGS:			return TDL_TASKTAG;
	case TDCA_TASKNAME:		return TDL_TASKTITLE;
	case TDCA_VERSION:		return TDL_TASKVERSION;
	case TDCA_SUBTASKDONE:	return TDL_TASKSUBTASKDONE;

	case TDCA_COST:			return (bCalc ? TDL_TASKCALCCOST : TDL_TASKCOST);
	case TDCA_LOCK:			return (bCalc ? TDL_TASKCALCLOCK : TDL_TASKLOCK);
	case TDCA_PERCENT:		return (bCalc ? TDL_TASKCALCCOMPLETION : TDL_TASKPERCENTDONE);
	case TDCA_PRIORITY:		return (bCalc ? TDL_TASKHIGHESTPRIORITY : TDL_TASKPRIORITY);
	case TDCA_RISK:			return (bCalc ? TDL_TASKHIGHESTRISK : TDL_TASKRISK);
	case TDCA_TIMEESTIMATE:		return (bCalc ? TDL_TASKCALCTIMEESTIMATE : TDL_TASKTIMEESTIMATE);
	case TDCA_TIMESPENT:	return (bCalc ? TDL_TASKCALCTIMESPENT : TDL_TASKTIMESPENT);

	case TDCA_CREATIONDATE:	return (bDisplay ? TDL_TASKCREATIONDATESTRING : TDL_TASKCREATIONDATE);
	case TDCA_DONEDATE:		return (bDisplay ? TDL_TASKDONEDATESTRING : TDL_TASKDONEDATE);
	case TDCA_LASTMODDATE:	return (bDisplay ? TDL_TASKLASTMODSTRING : TDL_TASKLASTMOD);
	case TDCA_POSITION:		return (bDisplay ? TDL_TASKPOSSTRING : TDL_TASKPOSITION);

	case TDCA_DUEDATE:		return (bCalc ? (bDisplay ? TDL_TASKCALCDUEDATESTRING : TDL_TASKDUEDATESTRING) : (bDisplay ? TDL_TASKDUEDATESTRING : TDL_TASKDUEDATE));
	case TDCA_STARTDATE:	return (bCalc ? (bDisplay ? TDL_TASKCALCSTARTDATESTRING : TDL_TASKSTARTDATESTRING) : (bDisplay ? TDL_TASKSTARTDATESTRING : TDL_TASKSTARTDATE));
	}
	
	// all else
	ASSERT(0);
	return NULLSTRING;
}

LPCTSTR CTaskFile::GetTaskAttribute(HTASKITEM hTask, TDC_ATTRIBUTE nAttrib, bool bCalc, bool bDisplay) const
{
	if (!IsAttributeAvailable(nAttrib))
		return NULLSTRING;

	if (!TaskHasAttribute(hTask, nAttrib, bCalc, bDisplay))
	{
		if (bCalc && TaskHasAttribute(hTask, nAttrib, !bCalc, bDisplay))
			bCalc = false;
		else
			return NULLSTRING;
	}

	LPCTSTR szAttrib = GetAttribTag(nAttrib, bCalc, bDisplay);

	if (bDisplay)
	{
		// Customisations
		static CString DISPLAYSTRING;

		switch (nAttrib)
		{
		case TDCA_ALLOCTO:
		case TDCA_CATEGORY:
		case TDCA_DEPENDENCY:
		case TDCA_FILELINK:
		case TDCA_TAGS:
			{
				DISPLAYSTRING = FormatTaskArray(hTask, szAttrib);
			}
			return DISPLAYSTRING;

		case TDCA_TIMEESTIMATE:
		case TDCA_TIMESPENT:
			{
				TDCTIMEPERIOD time;
				time.dAmount = (nAttrib == TDCA_TIMEESTIMATE) ?
								GetTaskTimeEstimate(hTask, time.nUnits, bCalc) :
								GetTaskTimeSpent(hTask, time.nUnits, bCalc);

				if (time.dAmount == 0)
					DISPLAYSTRING.Empty();
				else
					DISPLAYSTRING = time.Format(2);
			}
			return DISPLAYSTRING;

		case TDCA_PRIORITY:
		case TDCA_RISK:
			{
				DISPLAYSTRING = GetTaskAttribute(hTask, szAttrib);

				if (DISPLAYSTRING == _T("-2")) // None
					DISPLAYSTRING.Empty();
			}
			return DISPLAYSTRING;

		case TDCA_COST:
			{
				TDCCOST cost(GetTaskAttribute(hTask, szAttrib));

				if (cost.dAmount == 0)
					DISPLAYSTRING.Empty();
				else
					DISPLAYSTRING = cost.Format(2);
			}
			return DISPLAYSTRING;
		}
	}

	// all else
	return GetTaskAttribute(hTask, szAttrib);
}

LPCTSTR CTaskFile::GetTaskAttribute(HTASKITEM hTask, LPCTSTR szAttrib) const
{
	if (Misc::IsEmpty(szAttrib))
		return NULLSTRING;
	
	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, NULLSTRING);

	LPCTSTR szValue = pXITask->GetItemValue(szAttrib);

	if (!Misc::IsEmpty(szValue))
		return szValue;
	
 	// Fallback in special case
	if (STR_MATCH(szAttrib, TDL_TASKPARENTID) && IsAttributeAvailable(TDCA_ID))
	{
		const CXmlItem* pXIParent = pXITask->GetParent();

		if (pXIParent)
			return pXIParent->GetItemValue(TDL_TASKID);
	}

	// all else
	return NULLSTRING;
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
	return CLR_NONE;
}

int CTaskFile::GetTaskRisk(HTASKITEM hTask, bool bHighest) const
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
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);

	// else
	CXmlItem* pXItem = pXITask->GetAddItem(TDL_TASKMETADATA);
	ASSERT (pXItem);

	return (NULL != pXItem->SetItemValue(szKey, szMetaData));
}

bool CTaskFile::ClearTaskMetaData(HTASKITEM hTask, LPCTSTR szKey)
{
	if (Misc::IsEmpty(szKey))
	{
		ASSERT(0);
		return false;
	}

	return (DeleteTaskAttribute(hTask, TDL_TASKMETADATA, szKey) != FALSE);
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

bool CTaskFile::SetTaskLastModifiedBy(HTASKITEM hTask, LPCWSTR szModifiedBy)
{
	return SetTaskString(hTask, TDL_TASKLASTMODBY, szModifiedBy);
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
	return (SetTaskFlag(hTask, bFlag, FALSE) != FALSE);
}

bool CTaskFile::SetTaskLock(HTASKITEM hTask, bool bLocked)
{
	return (SetTaskLock(hTask, bLocked, FALSE) != FALSE);
}

bool CTaskFile::SetTaskFileLinkPath(HTASKITEM hTask, LPCTSTR szFileLink)
{
	return SetTaskString(hTask, TDL_TASKFILELINKPATH, szFileLink);
}

bool CTaskFile::SetTaskCreatedBy(HTASKITEM hTask, LPCTSTR szCreatedBy)
{
	return SetTaskString(hTask, TDL_TASKCREATEDBY, szCreatedBy);
}

BOOL CTaskFile::SetTaskID(HTASKITEM hTask, unsigned long nID)
{
	if (SetTaskULong(hTask, TDL_TASKID, nID))
	{
		m_dwNextUniqueID = max(m_dwNextUniqueID, (nID + 1));
		return TRUE;
	}
	
	return FALSE;
}

int CTaskFile::GetTaskIDs(CDWordArray& aTaskIDs, BOOL bIncParents) const
{
	aTaskIDs.RemoveAll();
	AddTaskIDs(GetFirstTask(), bIncParents, aTaskIDs);

	return aTaskIDs.GetSize();
}

void CTaskFile::AddTaskIDs(HTASKITEM hTask, BOOL bIncParents, CDWordArray& aTaskIDs) const
{
	if (!hTask)
		return;

	HTASKITEM hSubtask = GetFirstTask(hTask);

	if (bIncParents || (hSubtask == NULL))
		aTaskIDs.Add(GetTaskID(hTask));

	// subtasks
	AddTaskIDs(hSubtask, bIncParents, aTaskIDs);

	// siblings
	AddTaskIDs(GetNextTask(hTask), bIncParents, aTaskIDs);
}

BOOL CTaskFile::SetTaskReferenceID(HTASKITEM hTask, unsigned long nRefID)
{
	return SetTaskULong(hTask, TDL_TASKREFID, nRefID);
}

DWORD CTaskFile::GetTaskReferenceID(HTASKITEM hTask) const
{
	return GetTaskULong(hTask, TDL_TASKREFID);
}

bool CTaskFile::IsTaskReference(HTASKITEM hTask) const
{
	return (GetTaskReferenceID(hTask) > 0);
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

bool CTaskFile::SetTaskPriorityOrRisk(HTASKITEM hTask, const CString& sIntItem, int iVal)
{
	if (!TODOITEM::IsValidPriorityOrRisk(iVal))
		iVal = (int)max(TDC_MINPRIORITYORISK, min(TDC_MAXPRIORITYORISK, iVal));

	if (!SetTaskInt(hTask, sIntItem, iVal))
		return false;

	return true;
}

bool CTaskFile::SetTaskPriority(HTASKITEM hTask, int nPriority)
{
	return SetTaskPriorityOrRisk(hTask, TDL_TASKPRIORITY, nPriority);
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
	if (!IsValidUnits(cUnits))
	{
		ASSERT(0);
		return false;
	}

	return SetTaskTime(hTask, TDL_TASKTIMEESTIMATE, dTimeEst, TDL_TASKTIMEESTUNITS, cUnits);
}

bool CTaskFile::SetTaskTimeSpent(HTASKITEM hTask, double dTimeSpent, TDC_UNITS cUnits)
{
	if (!IsValidUnits(cUnits))
	{
		ASSERT(0);
		return false;
	}

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

BOOL CTaskFile::SetTaskLastModified(HTASKITEM hTask, const COleDateTime& tLastMod, const CString& sModifiedBy)
{
	return (BOOL)(SetTaskDate(hTask, TDL_TASKLASTMOD, tLastMod, TDL_TASKLASTMODSTRING) &&
					SetTaskString(hTask, TDL_TASKLASTMODBY, sModifiedBy, XIT_ATTRIB));
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
	TDC_REGULARITY nRegularity = tr.GetRegularity(dwSpecific1, dwSpecific2);

	int nNumOccur = tr.GetOccurrenceCount();
	int nRemainingOccur = tr.GetRemainingOccurrenceCount();

	if (SetTaskRecurrence(hTask, 
							nRegularity, 
							dwSpecific1, 
							dwSpecific2, 
							tr.nRecalcFrom, 
							tr.nReuse, 
							nNumOccur,
							nRemainingOccur,
							(tr.bPreserveComments != FALSE)))
	{
		SetTaskString(hTask, TDL_TASKRECURRENCE, tr.GetRegularityText(), XIT_ELEMENT); // human readable
		return true;
	}
	
	return false;
}

// ITaskList6 interface
bool CTaskFile::SetTaskRecurrence(HTASKITEM hTask, TDC_REGULARITY nRegularity, DWORD dwSpecific1, 
									DWORD dwSpecific2, bool bRecalcFromDue, TDC_RECURREUSEOPTION nReuse)
{
	return SetTaskRecurrence(hTask, nRegularity, dwSpecific1, dwSpecific2, 
							(bRecalcFromDue ? TDIRO_DUEDATE : TDIRO_DONEDATE), nReuse, -1);
}

// ITaskList13 interface
bool CTaskFile::SetTaskRecurrence(HTASKITEM hTask, TDC_REGULARITY nRegularity, 
									DWORD dwSpecific1, DWORD dwSpecific2, 
									TDC_RECURFROMOPTION nRecalcFrom, TDC_RECURREUSEOPTION nReuse, 
									int nNumOccur)
{
	return SetTaskRecurrence(hTask, nRegularity, dwSpecific1, dwSpecific2, 
							nRecalcFrom, nReuse, nNumOccur, nNumOccur, (nReuse == TDIRO_REUSE));
}

// ITaskList16 interface
bool CTaskFile::SetTaskRecurrence(HTASKITEM hTask, TDC_REGULARITY nRegularity, 
									DWORD dwSpecific1, DWORD dwSpecific2, 
									TDC_RECURFROMOPTION nRecalcFrom, TDC_RECURREUSEOPTION nReuse, 
									int nNumOccur, int nRemainingOccur, bool bPreserveComments)
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
		pXIRecur->SetItemValue(TDL_TASKRECURRENCEPRESERVECOMMENTS, bPreserveComments);
		
		return true;
	}
	
	return false;
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
	ASSERT(lstrcmp(szIcon, _T("-1")) != 0); // Historical bug
	return SetTaskString(hTask, TDL_TASKICON, szIcon);
}

BOOL CTaskFile::SetTaskSubtaskCompletion(HTASKITEM hTask, const CString& sSubtaskDone)
{
	return SetTaskString(hTask, TDL_TASKSUBTASKDONE, sSubtaskDone);
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
	return SetTaskDate(hTask, TDL_TASKCALCDUEDATE, date, TDL_TASKCALCDUEDATESTRING);
}

BOOL CTaskFile::SetTaskCalcStartDate(HTASKITEM hTask, const COleDateTime& date)
{
	return SetTaskDate(hTask, TDL_TASKCALCSTARTDATE, date, TDL_TASKCALCSTARTDATESTRING);
}

BOOL CTaskFile::SetTaskCalcCompletion(HTASKITEM hTask, int nPercent)
{
	return SetTaskUChar(hTask, TDL_TASKCALCCOMPLETION, (unsigned char)nPercent);
}

BOOL CTaskFile::SetTaskFlag(HTASKITEM hTask, BOOL bFlag, BOOL bCalc)
{
	if (bCalc)
		return SetTaskUChar(hTask, TDL_TASKCALCFLAG, (unsigned char)(bFlag ? 1 : 0));

	// else
	return SetTaskUChar(hTask, TDL_TASKFLAG, (unsigned char)(bFlag ? 1 : 0));
}

BOOL CTaskFile::SetTaskLock(HTASKITEM hTask, BOOL bLock, BOOL bCalc)
{
	if (bCalc)
		return SetTaskUChar(hTask, TDL_TASKCALCLOCK, (unsigned char)(bLock ? 1 : 0));

	// else
	return SetTaskUChar(hTask, TDL_TASKLOCK, (unsigned char)(bLock ? 1 : 0));
}

BOOL CTaskFile::SetTaskHighestPriority(HTASKITEM hTask, int nPriority)
{
	return SetTaskPriorityOrRisk(hTask, TDL_TASKHIGHESTPRIORITY, nPriority);
}

BOOL CTaskFile::SetTaskHighestRisk(HTASKITEM hTask, int nRisk)
{
	return SetTaskPriorityOrRisk(hTask, TDL_TASKHIGHESTRISK, nRisk);
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
	return SetTaskULong(hTask, TDL_TASKPOSITION, nPos);
}

bool CTaskFile::SetTaskRisk(HTASKITEM hTask, int nRisk)
{
	return SetTaskPriorityOrRisk(hTask, TDL_TASKRISK, nRisk);
}

bool CTaskFile::SetTaskExternalID(HTASKITEM hTask, LPCTSTR szID)
{
	return SetTaskString(hTask, TDL_TASKEXTERNALID, szID);
}

BOOL CTaskFile::SetTaskIsParent(HTASKITEM hTask)
{
	return SetTaskUChar(hTask, TDL_TASKISPARENT, 1);
}

BOOL CTaskFile::SetTaskPath(HTASKITEM hTask, const CString& sPath)
{
	return SetTaskString(hTask, TDL_TASKPATH, sPath);
}

BOOL CTaskFile::GetTaskAttribute(HTASKITEM hTask, const CString& sAttrib, CString& sValue) const
{
	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, FALSE);

	const CXmlItem* pXItem = pXITask->GetItem(sAttrib);

	if (!pXItem)
		return FALSE;

	sValue = pXItem->GetValue();
	return TRUE;
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

const CString& CTaskFile::GetTaskString(HTASKITEM hTask, const CString& sStringItem) const
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

////////////////////////////////////////////////////////////////////

CString CTaskFile::FormatDate(const COleDateTime& date) const
{
	DWORD dwFmt = (m_bISODates ? DHFD_ISO : 0);

	if (CDateHelper::DateHasTime(date))
		dwFmt |= DHFD_TIME | DHFD_NOSEC;

	return CDateHelper::FormatDate(date, dwFmt);
}

BOOL CTaskFile::DeleteTaskAttribute(HTASKITEM hTask, const CString& sAttrib, const CString& sKey)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);

	return pXITask->DeleteItem(sAttrib, sKey);
}

bool CTaskFile::SetTaskDate(HTASKITEM hTask, const CString& sDateItem, const COleDateTime& date, 
							const CString& sDateStringItem)
{
	if (SetTaskDouble(hTask, sDateItem, date))
	{
		if (!sDateStringItem.IsEmpty())
			return SetTaskString(hTask, sDateStringItem, FormatDate(date));
		
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

CXmlItem* CTaskFile::GetTaskArrayItem(HTASKITEM hTask, const CString& sItemTag, int nIndex) const
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, NULL);
	
	CXmlItem* pXI = pXITask->GetItem(sItemTag);

	while (pXI)
	{
		if (nIndex == 0) // this is the item we want
			return pXI;

		// else continue
		nIndex--;
		pXI = pXI->GetSibling();
	}

	// not found
	return NULL;
}

CString CTaskFile::GetTaskArrayItemValue(HTASKITEM hTask, const CString& sItemTag, int nIndex) const
{
	CXmlItem* pXI = GetTaskArrayItem(hTask, sItemTag, nIndex);

	return (pXI ? pXI->GetValue() : EMPTY_STR);
}

bool CTaskFile::AddTaskArrayItem(HTASKITEM hTask, const CString& sItemTag, const CString& sItem, BOOL bAllowEmpty)
{
	if (!bAllowEmpty && sItem.IsEmpty())
		return false;

	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, false);

	CXmlItem* pXI = pXITask->GetItem(sItemTag);

	// check existence before adding
	if (!pXI || !pXI->FindItem(sItemTag, sItem, FALSE))
		pXITask->AddItem(sItemTag, sItem, XIT_ELEMENT);
	
	return true;
}

int CTaskFile::GetTaskArray(HTASKITEM hTask, const CString& sItemTag, CStringArray& aItems, BOOL bAllowEmpty) const
{
	aItems.RemoveAll();

	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, 0);

	// first item
	const CXmlItem* pXI = pXITask->GetItem(sItemTag);
	
	while (pXI)
	{
		if (bAllowEmpty || pXI->HasValue())
			aItems.Add(pXI->GetValue());

		pXI = pXI->GetSibling();
	}

	return aItems.GetSize();
}

int CTaskFile::GetTaskArraySize(HTASKITEM hTask, const CString& sItemTag) const
{
	const CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, 0);

	int nSize = 0;
	const CXmlItem* pXI = pXITask->GetItem(sItemTag);

	while (pXI)
	{
		nSize++;

		pXI = pXI->GetSibling();
	}

	return nSize;
}

bool CTaskFile::DeleteTaskArray(HTASKITEM hTask, const CString& sItemTag)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, FALSE);

	// delete any existing items
	return (pXITask->DeleteItem(sItemTag) == TRUE);
}

CString CTaskFile::FormatTaskArray(HTASKITEM hTask, const CString& sItemTag) const
{
	CStringArray aItems;

	if (!GetTaskArray(hTask, sItemTag, aItems, FALSE))
		return NULLSTRING;

	return Misc::FormatArray(aItems);
}

BOOL CTaskFile::SetTaskArray(HTASKITEM hTask, const CString& sItemTag, const CStringArray& aItems, BOOL bAllowEmpty)
{
	CXmlItem* pXITask = NULL;
	GET_TASK(pXITask, hTask, FALSE);

	// delete any existing items
	DeleteTaskArray(hTask, sItemTag);

	// then add these
	int nCount = aItems.GetSize();
	
	for (int nItem = 0; nItem < nCount; nItem++)
		AddTaskArrayItem(hTask, sItemTag, aItems[nItem], bAllowEmpty);

	return TRUE;
}

int CTaskFile::GetArray(const CString& sItemTag, CStringArray& aItems) const
{
	aItems.RemoveAll();

	// first item
	const CXmlItem* pXI = GetItem(sItemTag);

	while (pXI)
	{
		if (pXI->HasValue())
			aItems.Add(pXI->GetValue());

		pXI = pXI->GetSibling();
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
	{
		if (!aItems[nItem].IsEmpty())
			AddItem(sItemTag, aItems[nItem], XIT_ELEMENT);
	}

	return TRUE;
}

