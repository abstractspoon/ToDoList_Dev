// ToDoCtrlData.cpp: implementation of the CToDoCtrlData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToDoCtrlData.h"
#include "TDCCustomAttributeHelper.h"
#include "resource.h"

#include "..\shared\xmlfile.h"
#include "..\shared\timehelper.h"
#include "..\shared\datehelper.h"
#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\enstring.h"
#include "..\shared\treectrlhelper.h"
#include "..\shared\treedragdrophelper.h"

#include <float.h>
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

// what got changed as a result of AdjustTaskDates
enum 
{
	ADJUSTED_NONE	= 0x00,
	ADJUSTED_START	= 0x01,
	ADJUSTED_DUE	= 0x02,
	ADJUSTED_DONE	= 0x04,
};

//////////////////////////////////////////////////////////////////////

#define EDIT_GET_TDI(id, tdi)	\
{								\
	GET_TDI(id, tdi, SET_FAILED)\
}					

#define GET_TDI(id, tdi, ret)	\
{								\
	if (id == 0)				\
		return ret;				\
	tdi = GetTask(id);			\
	ASSERT(tdi);				\
	if (tdi == NULL)			\
		return ret;				\
}

#define GET_TDI_TDS(id, tdi, tds, ret)	\
{										\
	if (id == 0)						\
		return ret;						\
	VERIFY(GetTask(id, tdi, tds));		\
	ASSERT(tdi && tds);					\
	if (tdi == NULL || tds == NULL)		\
		return ret;						\
}

//////////////////////////////////////////////////////////////////////

CUndoAction::CUndoAction(CToDoCtrlData& data, TDCUNDOACTIONTYPE nType, BOOL bExtendLastAction) 
	: m_data(data), m_bActive(FALSE)
{
	if (bExtendLastAction)
		m_bActive = m_data.ExtendLastUndoAction(nType);
	else
		m_bActive = m_data.BeginNewUndoAction(nType);
}

CUndoAction::~CUndoAction()
{
	if (m_bActive)
	{
		VERIFY(m_data.EndCurrentUndoAction());
		m_bActive = FALSE;
	}
}

//////////////////////////////////////////////////////////////////////
// static variables

TDC_UNITS CToDoCtrlData::s_nDefTimeEstUnits		= TDCU_HOURS;
TDC_UNITS CToDoCtrlData::s_nDefTimeSpentUnits	= TDCU_HOURS;
BOOL CToDoCtrlData::s_bUpdateInheritAttrib		= FALSE; 

CTDCAttributeMap CToDoCtrlData::s_mapParentAttribs; 
CString CToDoCtrlData::s_cfDefault;

//////////////////////////////////////////////////////////////////////

static const CString EMPTY_STR;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToDoCtrlData::CToDoCtrlData(const CWordArray& aStyles)	
	: 
	m_aStyles(aStyles)
{
	m_mapID2TDI.InitHashTable(1991); // prime number closest to 2000
}

CToDoCtrlData::~CToDoCtrlData()
{
	DeleteAllTasks();
}

void CToDoCtrlData::SetInheritedParentAttributes(const CTDCAttributeMap& mapAttribs, BOOL bUpdateAttrib)
{
	s_mapParentAttribs.CopyAttributes(mapAttribs);
	s_bUpdateInheritAttrib = bUpdateAttrib;
}

BOOL CToDoCtrlData::WantUpdateInheritedAttibute(TDC_ATTRIBUTE nAttrib)
{
	return (s_bUpdateInheritAttrib && s_mapParentAttribs.HasAttribute(nAttrib));
}

int CToDoCtrlData::BuildDataModel(const CTaskFile& tasks)
{
	m_undo.ResetAll();
	DeleteAllTasks();

	// add top-level items
	VERIFY(AddTaskToDataModel(tasks, NULL, &m_struct));

	return GetTaskCount();
}

BOOL CToDoCtrlData::AddTaskToDataModel(const CTaskFile& tasks, HTASKITEM hTask, TODOSTRUCTURE* pTDSParent)
{
	ASSERT(pTDSParent);

	if (!pTDSParent)
		return FALSE;

	if (hTask)	
	{
		DWORD dwTaskID = tasks.GetTaskID(hTask);

		// Duplicates should never occur but we've had one
		// reported before without establishing what caused it
		if (HasTask(dwTaskID))
		{
			ASSERT(0);
			
			// Don't add this task but simply merge the new task's
			// children into the existing task.
			
			// Note: If the subtasks are also duplicates then this
			// approach will just filter them out too.
			
			// Existing task becomes the parent
			pTDSParent = m_struct.FindTask(dwTaskID);
		}
		else
		{
			TODOITEM* pTDI = NewTask(tasks, hTask);
			ASSERT(pTDI);
			
			m_mapID2TDI[dwTaskID] = pTDI;
			
			// New task becomes the parent
			pTDSParent = m_struct.AddTask(dwTaskID, pTDSParent);
		}
		
		ASSERT(pTDSParent);
	}

	// tasks children
	HTASKITEM hSubtask = tasks.GetFirstTask(hTask);

	while (hSubtask)
	{
		VERIFY(AddTaskToDataModel(tasks, hSubtask, pTDSParent));

		// next task
		hSubtask = tasks.GetNextTask(hSubtask);
	}

	return TRUE;
}

TODOITEM* CToDoCtrlData::NewTask() const
{
	return new TODOITEM();
}

TODOITEM* CToDoCtrlData::NewTask(const TODOITEM& tdiRef, DWORD dwParentTaskID) const
{
	TODOITEM* pTDI = new TODOITEM(tdiRef);

	// copy over parent attribs
	if (dwParentTaskID && !s_mapParentAttribs.IsEmpty())
		CopyTaskAttributes(pTDI, dwParentTaskID, s_mapParentAttribs);

	return pTDI;
}

TODOITEM* CToDoCtrlData::NewTask(const CTaskFile& tasks, HTASKITEM hTask) const
{
	ASSERT(hTask);

	if (!hTask)
		return NULL;

	TODOITEM* pTDI = NewTask();
	ASSERT(pTDI);

	if (!pTDI)
		return NULL;
	
	tasks.GetTaskAttributes(hTask, pTDI);
	
	// make sure incomplete tasks are not 100% and vice versa
	if (pTDI->IsDone())
		pTDI->nPercentDone = 100;
	else
		pTDI->nPercentDone = max(0, min(99, pTDI->nPercentDone));
	
	// set comments type if not set
	if (pTDI->sCommentsTypeID.IsEmpty()) 
		pTDI->sCommentsTypeID = s_cfDefault;

	return pTDI;
}

BOOL CToDoCtrlData::HasTask(DWORD dwTaskID) const
{
	return (GetTask(dwTaskID, FALSE) != NULL);
}

// external const version
const TODOITEM* CToDoCtrlData::GetTask(DWORD& dwTaskID, BOOL bTrue) const
{
	return const_cast<CToDoCtrlData*>(this)->GetTask(dwTaskID, bTrue);
}

// internal non-const version
TODOITEM* CToDoCtrlData::GetTask(DWORD& dwTaskID, BOOL bTrue)
{
	TODOITEM* pTDI = NULL;

	if (dwTaskID && m_mapID2TDI.Lookup(dwTaskID, pTDI))
	{
		// check for reference task
		if (bTrue && pTDI->dwTaskRefID)
		{
			DWORD dwRefID = pTDI->dwTaskRefID;
			
			if (dwRefID && m_mapID2TDI.Lookup(dwRefID, pTDI))
			{
				dwTaskID = dwRefID;
			}
		}
	}
	
	return pTDI;
}

// internal non-const version
TODOITEM* CToDoCtrlData::GetTask(const TODOSTRUCTURE* pTDS, BOOL bTrue) const
{
	if (!pTDS || !pTDS->GetTaskID())
		return NULL;
	
	// else
	DWORD dwTaskID = pTDS->GetTaskID();
	const TODOITEM* pTDI = GetTask(dwTaskID, bTrue);

	return const_cast<TODOITEM*>(pTDI);
}

BOOL CToDoCtrlData::Locate(DWORD dwParentID, DWORD dwPrevSiblingID, TODOSTRUCTURE*& pTDSParent, int& nPos) const
{
	pTDSParent = NULL;
	nPos = -1;
	
	if (dwPrevSiblingID)
		m_struct.FindTask(dwPrevSiblingID, pTDSParent, nPos);
	
	else if (dwParentID)
		pTDSParent = m_struct.FindTask(dwParentID);
	else
		pTDSParent = const_cast<CToDoCtrlStructure*>(&m_struct); // root
	
	ASSERT (pTDSParent);
	return (pTDSParent != NULL);
}

BOOL CToDoCtrlData::LocateTask(DWORD dwTaskID, TODOSTRUCTURE*& pTDSParent, int& nPos) const
{
	return m_struct.FindTask(dwTaskID, pTDSParent, nPos);
}

const TODOSTRUCTURE* CToDoCtrlData::LocateTask(DWORD dwTaskID) const
{
	// special case
	if (dwTaskID == 0)
		return &m_struct;
	
	// else
	return m_struct.FindTask(dwTaskID);
}

void CToDoCtrlData::AddTask(DWORD dwTaskID, TODOITEM* pTDI, DWORD dwParentID, DWORD dwPrevSiblingID) 
{ 
	if (dwTaskID && pTDI)
	{
		// must delete duplicates else we'll get a memory leak
		TODOITEM* pExist = GetTask(dwTaskID, FALSE);
		
		if (pExist)
		{
			m_mapID2TDI.RemoveKey(dwTaskID);
			delete pExist;
		}
	}
	
	// add to structure
	TODOSTRUCTURE* pTDSParent = NULL;
	int nPrevSibling = -1;
	
	if (!Locate(dwParentID, dwPrevSiblingID, pTDSParent, nPrevSibling))
		return;
	
	m_struct.InsertTask(dwTaskID, pTDSParent, nPrevSibling + 1);
	m_mapID2TDI.SetAt(dwTaskID, pTDI); 
	
	AddUndoElement(TDCUEO_ADD, dwTaskID, dwParentID, dwPrevSiblingID);
}

void CToDoCtrlData::DeleteAllTasks()
{
	if (m_undo.CurrentAction() == TDCUAT_NONE)
	{
		// delete the data
		DWORD dwID;
		TODOITEM* pTDI;
		POSITION pos = m_mapID2TDI.GetStartPosition();

		while (pos)
		{
			m_mapID2TDI.GetNextAssoc(pos, dwID, pTDI);
			delete pTDI;
		}

		m_mapID2TDI.RemoveAll();

		// delete the structure
		m_struct.DeleteAll();
	}
	else
	{
		ASSERT(m_undo.CurrentAction() == TDCUAT_DELETE);

		// delete all top-level tasks
		while (m_struct.GetSubTaskCount())
			DeleteTask(&m_struct, 0);
	}

	ASSERT(GetTaskCount() == 0);
}

CString CToDoCtrlData::GetTaskTitle(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, EMPTY_STR);
	
	return pTDI->sTitle;
}

CString CToDoCtrlData::GetTaskIcon(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, EMPTY_STR);
	
	return pTDI->sIcon;
}

CString CToDoCtrlData::GetTaskComments(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, EMPTY_STR);
	
	return pTDI->sComments;
}

const CBinaryData& CToDoCtrlData::GetTaskCustomComments(DWORD dwTaskID, CString& sCommentsTypeID) const
{
	static CBinaryData content;
	sCommentsTypeID.Empty();

	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, content);
	
	sCommentsTypeID = pTDI->sCommentsTypeID;
	return pTDI->customComments;
}

double CToDoCtrlData::GetTaskCost(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0);
	
	return pTDI->dCost;
}

double CToDoCtrlData::GetTaskTimeEstimate(DWORD dwTaskID, TDC_UNITS& nUnits) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0);

	nUnits = pTDI->nTimeEstUnits;
	return pTDI->dTimeEstimate;
}

double CToDoCtrlData::GetTaskTimeSpent(DWORD dwTaskID, TDC_UNITS& nUnits) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0);
	
	nUnits = pTDI->nTimeSpentUnits;
	return pTDI->dTimeSpent;
}

int CToDoCtrlData::GetTaskAllocTo(DWORD dwTaskID, CStringArray& aAllocTo) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0);
	
	aAllocTo.Copy(pTDI->aAllocTo);
	return aAllocTo.GetSize();
}

CString CToDoCtrlData::GetTaskAllocBy(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, EMPTY_STR);
	
	return pTDI->sAllocBy;
}

CString CToDoCtrlData::GetTaskVersion(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, EMPTY_STR);
	
	return pTDI->sVersion;
}

CString CToDoCtrlData::GetTaskStatus(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, EMPTY_STR);
	
	return pTDI->sStatus;
}

int CToDoCtrlData::GetTaskCategories(DWORD dwTaskID, CStringArray& aCategories) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0);
	
	aCategories.Copy(pTDI->aCategories);
	return aCategories.GetSize();
}

int CToDoCtrlData::GetTaskTags(DWORD dwTaskID, CStringArray& aTags) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0);
	
	aTags.Copy(pTDI->aTags);
	return aTags.GetSize();
}

CString CToDoCtrlData::GetTaskPositionString(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, EMPTY_STR);
	
	return GetTaskPositionString(pTDI, pTDS);
}

CString CToDoCtrlData::GetTaskPath(DWORD dwTaskID, int nMaxLen) const
{ 
	if (nMaxLen == 0)
		return EMPTY_STR;
	
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, EMPTY_STR);

	CString sPath = GetTaskPath(pTDI, pTDS);

	if (nMaxLen == -1 || sPath.IsEmpty() || sPath.GetLength() <= nMaxLen)
		return sPath;

	CStringArray aElements;

	int nNumElm = Misc::Split(sPath, aElements, '\\', TRUE);
	ASSERT (nNumElm >= 2);

	int nTrimElm = ((sPath.GetLength() - nMaxLen) / nNumElm) + 2;

	for (int nElm = 0; nElm < nNumElm; nElm++)
	{
		CString& sElm = aElements[nElm];
		sElm = sElm.Left(sElm.GetLength() - nTrimElm) + "...";
	}

	return Misc::FormatArray(aElements, _T("\\"));
}

int CToDoCtrlData::GetTaskDependencies(DWORD dwTaskID, CStringArray& aDependencies) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0);
	
	aDependencies.Copy(pTDI->aDependencies);
	return aDependencies.GetSize();
}

CString CToDoCtrlData::GetTaskDependency(DWORD dwTaskID, int nDepends) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, EMPTY_STR);
	
	if (nDepends < pTDI->aDependencies.GetSize())
		return pTDI->aDependencies[nDepends];

	return EMPTY_STR;
}

BOOL CToDoCtrlData::IsTaskDependent(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);
	
	return (pTDI->aDependencies.GetSize() > 0);
}

BOOL CToDoCtrlData::IsTaskLocallyDependentOn(DWORD dwTaskID, DWORD dwOtherID, BOOL bImmediateOnly) const
{
	ASSERT(dwOtherID);

	if (!dwOtherID)
		return FALSE;

	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);
	
	if (bImmediateOnly)
		return pTDI->IsLocallyDependentOn(dwOtherID);

	CDWordArray aDependIDs;
	int nDepend = pTDI->GetLocalDependencies(aDependIDs);

	while (nDepend--)
	{
		DWORD dwDependID = aDependIDs[nDepend];
		ASSERT(dwDependID);

		if (dwDependID == dwOtherID)
			return TRUE;

		// else check dependents of dwDependID
		if (IsTaskLocallyDependentOn(dwDependID, dwOtherID, FALSE)) // RECURSIVE
			return TRUE;
	}


	// all else
	return FALSE;
}

// within the same task list only
int CToDoCtrlData::GetTaskLocalDependencies(DWORD dwTaskID, CDWordArray& aDependencies) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0);

	int nDepends = pTDI->GetLocalDependencies(aDependencies);

	// weed out 'unknown' tasks
	while (nDepends--)
	{
		if (!HasTask(aDependencies[nDepends]))
			aDependencies.RemoveAt(nDepends);
	}

	return aDependencies.GetSize();
}

BOOL CToDoCtrlData::RemoveTaskLocalDependency(DWORD dwTaskID, DWORD dwDependID)
{
	TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);

	return pTDI->RemoveLocalDependency(dwDependID);
}

BOOL CToDoCtrlData::TaskHasDependents(DWORD dwTaskID) const
{
	ASSERT(dwTaskID);

	if (!dwTaskID)
		return FALSE;

	POSITION pos = m_mapID2TDI.GetStartPosition();
	CString sTaskID = Misc::Format(dwTaskID);
		
	while (pos)
	{
		TODOITEM* pTDI = NULL;
		DWORD dwDependsID;
			
		m_mapID2TDI.GetNextAssoc(pos, dwDependsID, pTDI);
		ASSERT (pTDI);
			
		if (pTDI && (dwDependsID != dwTaskID) && Misc::Find(pTDI->aDependencies, sTaskID) != -1)
			return TRUE;
	}	

	// else
	return FALSE;
}

// only interested in dependents within the same task list
int CToDoCtrlData::GetTaskLocalDependents(DWORD dwTaskID, CDWordArray& aDependents) const
{
	aDependents.RemoveAll();

	if (dwTaskID)
	{
		POSITION pos = m_mapID2TDI.GetStartPosition();
		
		while (pos)
		{
			TODOITEM* pTDI = NULL;
			DWORD dwDependsID;
			
			m_mapID2TDI.GetNextAssoc(pos, dwDependsID, pTDI);
			ASSERT (pTDI);
			
			if (pTDI && (dwDependsID != dwTaskID))
			{
				CDWordArray aDependIDs;
				int nDepend = GetTaskLocalDependencies(dwDependsID, aDependIDs);

				while (nDepend--)
				{
					if (aDependIDs[nDepend] == dwTaskID)
						aDependents.Add(dwDependsID);
				}
			}
		}
	}	
	
	return aDependents.GetSize();
}

void CToDoCtrlData::FixupTaskLocalDependentsIDs(DWORD dwTaskID, DWORD dwPrevTaskID)
{
	CDWordArray aDependents;
	
	if (GetTaskLocalDependents(dwPrevTaskID, aDependents))
	{
		CString sPrevTaskID = Misc::Format(dwPrevTaskID);
		CString sTaskID = Misc::Format(dwTaskID);

		int nTask = aDependents.GetSize();

		while (nTask--)
		{
			DWORD dwDependentID = aDependents[nTask];
			CStringArray aDepends;
	
			// delete existing dependency
			if (GetTaskDependencies(dwDependentID, aDepends))
				Misc::RemoveItem(sPrevTaskID, aDepends);

			// add new dependency
			aDepends.Add(sTaskID);

			// update dependencies
			SetTaskDependencies(dwDependentID, aDepends, FALSE);
		}
	}
}

BOOL CToDoCtrlData::TaskHasLocalCircularDependencies(DWORD dwTaskID) const
{
	if (!dwTaskID)
		return FALSE;

	// trace each of this tasks dependencies to see 
	// if it ever comes back to itself.
	CID2IDMap mapVisited;

	// we only check 'same file' links
	CDWordArray aDependIDs;
	int nDepends = GetTaskLocalDependencies(dwTaskID, aDependIDs);
	
	while (nDepends--)
	{
		if (FindTaskLocalDependency(aDependIDs[nDepends], dwTaskID, mapVisited))
			return TRUE;
	}
	
	// else
	return FALSE;
}

BOOL CToDoCtrlData::FindTaskLocalDependency(DWORD dwTaskID, DWORD dwDependsID, CID2IDMap& mapVisited) const
{
	// simple checks
	if (!dwTaskID || !HasTask(dwTaskID))
		return FALSE; // no such task == not found
	
	if (dwTaskID == dwDependsID)
		return TRUE; // same task == circular
	
	// if we have been here before, we can stop
	DWORD dwDummy;
	
	if (mapVisited.Lookup(dwTaskID, dwDummy))
		return TRUE; // circular
	
	// else mark this task as having been visited
	mapVisited[dwTaskID] = TRUE;
	
	// and process its 'same file' dependencies
	CDWordArray aDependIDs;
	int nDepends = GetTaskLocalDependencies(dwTaskID, aDependIDs);
	
	while (nDepends--)
	{
		if (FindTaskLocalDependency(aDependIDs[nDepends], dwDependsID, mapVisited))
			return TRUE;
	}
	
	// else not found
	return FALSE;
}

CString CToDoCtrlData::GetTaskExtID(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, EMPTY_STR);
	
	return pTDI->sExternalID;
}

CString CToDoCtrlData::GetTaskCreatedBy(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, EMPTY_STR);
	
	return pTDI->sCreatedBy;
}

COLORREF CToDoCtrlData::GetTaskColor(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0);
	
	return pTDI->color;
}

int CToDoCtrlData::GetTaskPriority(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FM_NOPRIORITY);
	
	return pTDI->nPriority;
}

int CToDoCtrlData::GetTaskRisk(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FM_NORISK);
	
	return pTDI->nRisk;
}

CString CToDoCtrlData::GetTaskCustomAttributeData(DWORD dwTaskID, const CString& sAttribID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, EMPTY_STR);
	
	return pTDI->GetCustomAttribValue(sAttribID);
}

BOOL CToDoCtrlData::CalcTaskCustomAttributeData(DWORD dwTaskID, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, double& dValue) const
{
	if (dwTaskID && attribDef.SupportsCalculation())
	{
		const TODOITEM* pTDI = NULL;
		const TODOSTRUCTURE* pTDS = NULL;
		
		if (GetTask(dwTaskID, pTDI, pTDS))
			return CalcTaskCustomAttributeData(pTDI, pTDS, attribDef, dValue);

		ASSERT(0);
	}
	
	// all else
	return FALSE;
}

BOOL CToDoCtrlData::CalcTaskCustomAttributeData(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, double& dValue) const
{
	if (!attribDef.SupportsCalculation())
		return FALSE;

	const CString& sAttribID = attribDef.sUniqueID;
	CString sTaskVal = pTDI->GetCustomAttribValue(sAttribID);
	DWORD dwDataType = attribDef.GetDataType();

	// easier to handle by feature 
	// -----------------------------------------------------------
	if (attribDef.HasFeature(TDCCAF_ACCUMULATE))
	{
		ASSERT(dwDataType == TDCCA_DOUBLE || 
				dwDataType == TDCCA_INTEGER);

		// our value
		dValue = Misc::Atof(sTaskVal);
		
		// our children's values
		for (int i = 0; i < pTDS->GetSubTaskCount(); i++)
		{
			double dSubtaskVal;
			DWORD dwSubtaskID = pTDS->GetSubTaskID(i);
			
			// ignore references else risk of infinite loop
			if (!IsTaskReference(dwSubtaskID) && CalcTaskCustomAttributeData(dwSubtaskID, attribDef, dSubtaskVal))
				dValue += dSubtaskVal;
		}
	}
	// -----------------------------------------------------------
	else if (attribDef.HasFeature(TDCCAF_MAXIMIZE))
	{
		ASSERT(dwDataType == TDCCA_DOUBLE || 
				dwDataType == TDCCA_INTEGER ||
				dwDataType == TDCCA_DATE);

		if (sTaskVal.IsEmpty())
			dValue = -DBL_MAX;
		else
			dValue = Misc::Atof(sTaskVal);

		// our children's values
		for (int i = 0; i < pTDS->GetSubTaskCount(); i++)
		{
			double dSubtaskVal;
			DWORD dwSubtaskID = pTDS->GetSubTaskID(i);
			
			// ignore references else risk of infinite loop
			if (!IsTaskReference(dwSubtaskID) && CalcTaskCustomAttributeData(dwSubtaskID, attribDef, dSubtaskVal))
				dValue = max(dSubtaskVal, dValue);
		}

		if (dValue <= -DBL_MAX)
			dValue = TODOITEM::NULL_VALUE;
	}
	// -----------------------------------------------------------
	else if (attribDef.HasFeature(TDCCAF_MINIMIZE))
	{
		ASSERT(dwDataType == TDCCA_DOUBLE || 
			dwDataType == TDCCA_INTEGER ||
			dwDataType == TDCCA_DATE);
		
		if (sTaskVal.IsEmpty())
			dValue = DBL_MAX;
		else
			dValue = Misc::Atof(sTaskVal);

		// our children's values
		for (int i = 0; i < pTDS->GetSubTaskCount(); i++)
		{
			double dSubtaskVal;
			DWORD dwSubtaskID = pTDS->GetSubTaskID(i);
			
			// ignore references else risk of infinite loop
			if (!IsTaskReference(dwSubtaskID) && CalcTaskCustomAttributeData(dwSubtaskID, attribDef, dSubtaskVal))
				dValue = min(dSubtaskVal, dValue);
		}

		if (dValue >= DBL_MAX)
			dValue = TODOITEM::NULL_VALUE;
	}
	// -----------------------------------------------------------
	else
	{
		return FALSE;
	}

	return (dValue != TODOITEM::NULL_VALUE);
}

BOOL CToDoCtrlData::IsTaskFlagged(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);
	
	return pTDI->bFlagged;
}

BOOL CToDoCtrlData::TaskHasRecurringParent(const TODOSTRUCTURE* pTDS) const
{
	return IsTaskRecurring(pTDS->GetParentTaskID(), TRUE);
}

BOOL CToDoCtrlData::TaskHasFileRef(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);
	
	return (pTDI->aFileLinks.GetSize() > 0);
}

BOOL CToDoCtrlData::CanTaskRecur(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);
	
	return pTDI->CanRecur();
}

BOOL CToDoCtrlData::IsTaskRecurring(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bCheckParent) const
{
	ASSERT(pTDI && (pTDS || !bCheckParent));
	
	if (!pTDI || (bCheckParent && !pTDS))
		return FALSE;

	BOOL bRecurring = pTDI->IsRecurring();

	if (!bRecurring && bCheckParent)
		bRecurring = IsTaskRecurring(pTDS->GetParentTaskID(), TRUE);

	return bRecurring;
}

BOOL CToDoCtrlData::IsTaskRecurring(DWORD dwTaskID, BOOL bCheckParent) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);

	const TODOSTRUCTURE* pTDS = (bCheckParent ? LocateTask(dwTaskID) : NULL);
	ASSERT(pTDS || !bCheckParent);
	
	if (bCheckParent && !pTDS)
		return FALSE;

	return IsTaskRecurring(pTDI, pTDS, bCheckParent);
}

BOOL CToDoCtrlData::GetTaskRecurrence(DWORD dwTaskID, TDCRECURRENCE& tr) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);
	
	tr = pTDI->trRecurrence;
	return TRUE;
}

BOOL CToDoCtrlData::GetTaskNextOccurrence(DWORD dwTaskID, COleDateTime& dtNext, BOOL& bDue)
{
	TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);
	
	return pTDI->GetNextOccurence(dtNext, bDue);
}

COleDateTime CToDoCtrlData::GetTaskDate(DWORD dwTaskID, TDC_DATE nDate) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0.0);
	
	return pTDI->GetDate(nDate);
}

BOOL CToDoCtrlData::TaskHasDate(DWORD dwTaskID, TDC_DATE nDate) const
{
	return CDateHelper::IsDateSet(GetTaskDate(dwTaskID, nDate));
}

int CToDoCtrlData::GetTaskPercent(DWORD dwTaskID, BOOL bCheckIfDone) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0);
	
	if (bCheckIfDone)
		return pTDI->IsDone() ? 100 : pTDI->nPercentDone;
	
	// else
	return pTDI->nPercentDone;
}

CString CToDoCtrlData::GetTaskFileRef(DWORD dwTaskID, int nFileRef) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, EMPTY_STR);
	
	return pTDI->GetFileRef(nFileRef);
}

int CToDoCtrlData::GetTaskFileRefs(DWORD dwTaskID, CStringArray& aFiles) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0);
	
	aFiles.Copy(pTDI->aFileLinks);
	return aFiles.GetSize();
}

int CToDoCtrlData::GetTaskFileRefCount(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0);
	
	return pTDI->aFileLinks.GetSize();
}

DWORD CToDoCtrlData::GetTaskParentID(DWORD dwTaskID) const
{
	if (dwTaskID)
	{
		const TODOSTRUCTURE* pTDS = LocateTask(dwTaskID);
		ASSERT(pTDS);

		if (pTDS)
			return pTDS->GetParentTaskID();
	}

	// else
	return 0;
}

DWORD CToDoCtrlData::GetTaskReferenceID(DWORD dwTaskID) const
{
	// NOTE: don't use GET_TDI macro here because 
	// that'll give us the 'true' task
	if (dwTaskID == 0)
		return 0;

	const TODOITEM* pTDI = GetTask(dwTaskID, FALSE);
	ASSERT(pTDI);

	return (pTDI ? pTDI->dwTaskRefID : 0);
}

BOOL CToDoCtrlData::CanMoveTask(DWORD /*dwTaskID*/, DWORD /*dwDestParentID*/) const
{
	return TRUE;
}

BOOL CToDoCtrlData::IsTaskReference(DWORD dwTaskID) const
{
	return (GetTaskReferenceID(dwTaskID) != 0);
}

BOOL CToDoCtrlData::IsReferenceToTask(DWORD dwTestID, DWORD dwTaskID) const
{
	ASSERT(dwTestID && dwTaskID);

	if (!dwTestID || !dwTaskID)
		return FALSE;

	return (GetTaskReferenceID(dwTestID) == dwTaskID);
}

BOOL CToDoCtrlData::DeleteTask(DWORD dwTaskID)
{
	if (dwTaskID)
	{
		TODOSTRUCTURE* pTDSParent = NULL;
		int nPos = -1;
		
		if (!m_struct.FindTask(dwTaskID, pTDSParent, nPos))
		{
			ASSERT(0);
			return FALSE;
		}

		// delete the task itself so that we don't have to worry
		// about checking if it has a reference to itself
		return DeleteTask(pTDSParent, nPos);
	}

	// else
	return FALSE;
}

BOOL CToDoCtrlData::DeleteTask(TODOSTRUCTURE* pTDSParent, int nPos)
{
	TODOSTRUCTURE* pTDS = pTDSParent->GetSubTask(nPos);
	ASSERT(pTDS);
	
	// do children first to ensure entire branch is deleted
	while (pTDS->GetSubTaskCount() > 0)
		DeleteTask(pTDS, 0);

	// is this task a reference?
	DWORD dwTaskID = pTDS->GetTaskID();
	BOOL bRef = IsTaskReference(dwTaskID);

	// save undo 
	DWORD dwParentID = pTDSParent->GetTaskID();
	DWORD dwPrevSiblingID = nPos ? pTDSParent->GetSubTaskID(nPos - 1) : 0;
	
	AddUndoElement(TDCUEO_DELETE, dwTaskID, dwParentID, dwPrevSiblingID);
	
	// then this item
	delete GetTask(dwTaskID, FALSE);
	m_mapID2TDI.RemoveKey(dwTaskID);
	m_struct.DeleteTask(dwTaskID);

	// then update it's referees and dependents unless it was a reference itself
	if (!bRef)
	{
		RemoveOrphanTaskReferences(&m_struct, dwTaskID);
		RemoveOrphanTaskLocalDependencies(&m_struct, dwTaskID);
	}
	
	return TRUE;
}

BOOL CToDoCtrlData::RemoveOrphanTaskReferences(TODOSTRUCTURE* pTDSParent, DWORD dwTaskID)
{
	ASSERT(pTDSParent && dwTaskID);
	
	if (!pTDSParent || !dwTaskID)
		return FALSE;
	
	int nChild = pTDSParent->GetSubTaskCount();
	BOOL bRemoved = FALSE;
	
	while (nChild--)
	{
		TODOSTRUCTURE* pTDSChild = pTDSParent->GetSubTask(nChild);
		
		// children's children first
		if (RemoveOrphanTaskReferences(pTDSChild, dwTaskID))
			bRemoved = TRUE;
		
		// then child
		TODOITEM* pTDIChild = GetTask(pTDSChild, FALSE);
		ASSERT(pTDIChild);
		
		if (pTDIChild)
		{
			// references
			if (pTDIChild->dwTaskRefID == dwTaskID)
			{
				DeleteTask(pTDSParent, nChild);
				bRemoved = TRUE;
			}
		}
	}
	
	return bRemoved;
}

BOOL CToDoCtrlData::RemoveOrphanTaskLocalDependencies(TODOSTRUCTURE* pTDSParent, DWORD dwDependID)
{
	ASSERT(pTDSParent && dwDependID);
	
	if (!pTDSParent || !dwDependID)
		return FALSE;
	
	int nChild = pTDSParent->GetSubTaskCount();
	BOOL bRemoved = FALSE;
	
	while (nChild--)
	{
		TODOSTRUCTURE* pTDSChild = pTDSParent->GetSubTask(nChild);
		
		// children's children first
		if (RemoveOrphanTaskLocalDependencies(pTDSChild, dwDependID))
			bRemoved = TRUE;
		
		// then child itself
		TODOITEM* pTDIChild = NULL;
		DWORD dwTaskID = pTDSChild->GetTaskID();
		
		GET_TDI(dwTaskID, pTDIChild, FALSE);

		if (pTDIChild->IsLocallyDependentOn(dwDependID))
		{
			AddUndoElement(TDCUEO_EDIT, dwTaskID);
			VERIFY(pTDIChild->RemoveLocalDependency(dwDependID));

			bRemoved = TRUE;
		}
		
	}
	
	return bRemoved;
}

TDC_SET CToDoCtrlData::CopyTaskAttributes(TODOITEM* pToTDI, DWORD dwFromTaskID, const CTDCAttributeMap& mapAttribs) const
{
	if (!pToTDI)
		return SET_FAILED;
	
	const TODOITEM* pFromTDI = NULL;
	EDIT_GET_TDI(dwFromTaskID, pFromTDI);
	
	TDC_SET nRes = SET_NOCHANGE;
	
	// helper macros
#define COPYATTRIB(a) if (pToTDI->a != pFromTDI->a) { pToTDI->a = pFromTDI->a; nRes = SET_CHANGE; }
#define COPYATTRIBARR(a) if (!Misc::MatchAll(pToTDI->a, pFromTDI->a)) { pToTDI->a.Copy(pFromTDI->a); nRes = SET_CHANGE; }
	
	// note: we don't use the public SetTask* methods purely so we can
	// capture all the edits as a single atomic change that can be undone
	for (int nAttrib = TDCA_FIRSTATTRIBUTE; nAttrib < TDCA_ATTRIBUTECOUNT; nAttrib++)
	{
		if (mapAttribs.HasAttribute((TDC_ATTRIBUTE)nAttrib))
		{
			switch (nAttrib)
			{
			case TDCA_DUEDATE:
			case TDCA_DUETIME:		COPYATTRIB(dateDue); break;
			case TDCA_STARTDATE:
			case TDCA_STARTTIME:	COPYATTRIB(dateStart); break;

			case TDCA_TASKNAME:		COPYATTRIB(sTitle); break;
			case TDCA_DONEDATE:		COPYATTRIB(dateDone); break;
			case TDCA_PRIORITY:		COPYATTRIB(nPriority); break;
			case TDCA_RISK:			COPYATTRIB(nRisk); break;
			case TDCA_COLOR:		COPYATTRIB(color); break;
			case TDCA_ALLOCBY:		COPYATTRIB(sAllocBy); break;
			case TDCA_STATUS:		COPYATTRIB(sStatus); break;
			case TDCA_PERCENT:		COPYATTRIB(nPercentDone); break;
			case TDCA_VERSION:		COPYATTRIB(sVersion); break;
			case TDCA_EXTERNALID:	COPYATTRIB(sExternalID); break;
			case TDCA_FLAG:			COPYATTRIB(bFlagged); break;
			
			case TDCA_TIMEEST:		COPYATTRIB(dTimeEstimate); 
									COPYATTRIB(nTimeEstUnits); break;
			case TDCA_TIMESPENT:	COPYATTRIB(dTimeSpent);	
									COPYATTRIB(nTimeSpentUnits); break;
			
			case TDCA_COMMENTS:		COPYATTRIB(sComments); 
									COPYATTRIB(customComments); 
									COPYATTRIB(sCommentsTypeID); break;
			
			case TDCA_FILEREF:		COPYATTRIBARR(aFileLinks); break;
			case TDCA_ALLOCTO:		COPYATTRIBARR(aAllocTo); break;
			case TDCA_CATEGORY:		COPYATTRIBARR(aCategories); break;
			case TDCA_TAGS:			COPYATTRIBARR(aTags); break;
			case TDCA_DEPENDENCY:	COPYATTRIBARR(aDependencies); break;

			default:
				ASSERT(0);
			}
		}
	}
	
	return nRes;
}

TDC_SET CToDoCtrlData::ClearTaskAttribute(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, BOOL bAndChildren)
{
	TDC_SET nRes = SET_NOCHANGE;

	switch (nAttrib)
	{
	case TDCA_DONEDATE:		
		nRes = SetTaskDate(dwTaskID, TDCD_DONE, 0.0);
		break;

	case TDCA_DUEDATE:		
		nRes = SetTaskDate(dwTaskID, TDCD_DUEDATE, 0.0);
		break;

	case TDCA_DUETIME:		
		nRes = SetTaskDate(dwTaskID, TDCD_DUETIME, 0.0);
		break;

	case TDCA_STARTDATE:	
		nRes = SetTaskDate(dwTaskID, TDCD_START, 0.0);
		break;

	case TDCA_STARTTIME:	
		nRes = SetTaskDate(dwTaskID, TDCD_STARTTIME, 0.0);
		break;
		
	case TDCA_PRIORITY:		
		nRes = SetTaskPriority(dwTaskID, FM_NOPRIORITY);
		break;

	case TDCA_RISK:			
		nRes = SetTaskRisk(dwTaskID, FM_NORISK);
		break;
		
	case TDCA_ALLOCTO:		
		nRes = SetTaskAllocTo(dwTaskID, CStringArray(), FALSE);
		break;

	case TDCA_CATEGORY:		
		nRes = SetTaskCategories(dwTaskID, CStringArray(), FALSE);
		break;

	case TDCA_TAGS:			
		nRes = SetTaskTags(dwTaskID, CStringArray(), FALSE);
		break;

	case TDCA_DEPENDENCY:	
		nRes = SetTaskDependencies(dwTaskID, CStringArray(), FALSE);
		break;

	case TDCA_FILEREF:		
		nRes = SetTaskFileRefs(dwTaskID, CStringArray(), FALSE);
		break;
		
	case TDCA_ALLOCBY:		
		nRes = SetTaskAllocBy(dwTaskID, EMPTY_STR);
		break;

	case TDCA_STATUS:		
		nRes = SetTaskStatus(dwTaskID, EMPTY_STR);
		break;

	case TDCA_VERSION:		
		nRes = SetTaskVersion(dwTaskID, EMPTY_STR);
		break;

	case TDCA_EXTERNALID:	
		nRes = SetTaskExtID(dwTaskID, EMPTY_STR);
		break;

	case TDCA_ICON:			
		nRes = SetTaskIcon(dwTaskID, EMPTY_STR);
		break;
		
	case TDCA_PERCENT:		
		nRes = SetTaskPercent(dwTaskID, 0);
		break;

	case TDCA_FLAG:			
		nRes = SetTaskFlag(dwTaskID, FALSE);
		break;

	case TDCA_COST:			
		nRes = SetTaskCost(dwTaskID, 0.0);
		break;

	case TDCA_COLOR:		
		nRes = SetTaskColor(dwTaskID, 0);
		break;

	case TDCA_RECURRENCE:	
		nRes = SetTaskRecurrence(dwTaskID, TDCRECURRENCE());
		break;
		
	case TDCA_TIMEEST:		
		{
			// preserve existing units
			TDC_UNITS nUnits;
			GetTaskTimeEstimate(dwTaskID, nUnits);

			nRes = SetTaskTimeEstimate(dwTaskID, 0.0, nUnits);
		}
		break;

	case TDCA_TIMESPENT:
		{ 
			// preserve existing units
			TDC_UNITS nUnits;
			GetTaskTimeSpent(dwTaskID, nUnits);

			nRes = SetTaskTimeSpent(dwTaskID, 0.0, nUnits);
		}
		break;

	case TDCA_COMMENTS:
		{
			TODOITEM* pTDI = NULL;
			EDIT_GET_TDI(dwTaskID, pTDI);

			pTDI->sComments.Empty();
			pTDI->customComments.Empty();
		}
		break;

	default:
		ASSERT (0);
		return SET_FAILED;
	}

	// children?
	if (bAndChildren)
	{
		const TODOSTRUCTURE* pTDS = m_struct.FindTask(dwTaskID);
		
		for (int nSubtask = 0; nSubtask < pTDS->GetSubTaskCount(); nSubtask++)
		{
			DWORD dwSubtaskID = pTDS->GetSubTaskID(nSubtask);
			
			if (ClearTaskAttribute(dwSubtaskID, nAttrib, TRUE) == SET_CHANGE)
				nRes = SET_CHANGE;
		}
	}
	
	return nRes;
}

TDC_SET CToDoCtrlData::ClearTaskCustomAttribute(DWORD dwTaskID, const CString& sAttribID, BOOL bAndChildren)
{
	TDC_SET nRes = SetTaskCustomAttributeData(dwTaskID, sAttribID, EMPTY_STR);

	// children?
	if (bAndChildren)
	{
		const TODOSTRUCTURE* pTDS = m_struct.FindTask(dwTaskID);
		
		for (int nSubtask = 0; nSubtask < pTDS->GetSubTaskCount(); nSubtask++)
		{
			DWORD dwSubtaskID = pTDS->GetSubTaskID(nSubtask);
			
			if (ClearTaskCustomAttribute(dwSubtaskID, sAttribID, TRUE) == SET_CHANGE)
				nRes = SET_CHANGE;
		}
	}
	
	return nRes;
}

void CToDoCtrlData::ApplyLastInheritedChangeToSubtasks(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib)
{
	// special case: 
	if (nAttrib == TDCA_ALL)
	{
		POSITION pos = s_mapParentAttribs.GetStartPosition();

		while (pos)
		{
			// FALSE means do not apply if parent is blank
			TDC_ATTRIBUTE nAttrib = s_mapParentAttribs.GetNextAttribute(pos);
			ApplyLastChangeToSubtasks(dwTaskID, nAttrib, FALSE);
		}
	}
	else if (WantUpdateInheritedAttibute(nAttrib))
	{
		ApplyLastChangeToSubtasks(dwTaskID, nAttrib);
	}
}

BOOL CToDoCtrlData::ApplyLastChangeToSubtasks(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, BOOL bIncludeBlank)
{
	if (dwTaskID)
	{
		const TODOITEM* pTDI = NULL;
		const TODOSTRUCTURE* pTDS = NULL;

		if (GetTask(dwTaskID, pTDI, pTDS))
			return ApplyLastChangeToSubtasks(pTDI, pTDS, nAttrib, bIncludeBlank);
	}

	// else
	return FALSE;
}

BOOL CToDoCtrlData::ApplyLastChangeToSubtasks(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, 
											  TDC_ATTRIBUTE nAttrib, BOOL bIncludeBlank)
{
	ASSERT(pTDI && pTDS);
	
	if (!pTDI || !pTDS)
		return FALSE;
	
	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		DWORD dwChildID = pTDS->GetSubTaskID(nSubTask);
		TODOITEM* pTDIChild = GetTask(dwChildID);
		ASSERT(pTDIChild);

		if (!pTDIChild)
			return FALSE;

		// save undo data
		SaveEditUndo(dwChildID, pTDIChild, nAttrib);
		
		// apply the change based on nAttrib
		switch (nAttrib)
		{
		case TDCA_DONEDATE:
			if (bIncludeBlank || pTDI->IsDone())
				pTDIChild->dateDone = pTDI->dateDone;
			break;
			
		case TDCA_DUEDATE:
		case TDCA_DUETIME:
			if (bIncludeBlank || pTDI->HasDue())
				pTDIChild->dateDue = pTDI->dateDue;
			break;
			
		case TDCA_STARTDATE:
		case TDCA_STARTTIME:
			if (bIncludeBlank || pTDI->HasStart())
				pTDIChild->dateStart = pTDI->dateStart;
			break;
			
		case TDCA_PRIORITY:
			if (bIncludeBlank || pTDI->nPriority != FM_NOPRIORITY)
				pTDIChild->nPriority = pTDI->nPriority;
			break;
			
		case TDCA_RISK:
			if (bIncludeBlank || pTDI->nRisk != FM_NORISK)
				pTDIChild->nRisk = pTDI->nRisk;
			break;
			
		case TDCA_COLOR:
			if (bIncludeBlank || pTDI->color != 0)
				pTDIChild->color = pTDI->color;
			break;
			
		case TDCA_ALLOCTO:
			if (bIncludeBlank || pTDI->aAllocTo.GetSize())
				pTDIChild->aAllocTo.Copy(pTDI->aAllocTo);
			break;
			
		case TDCA_ALLOCBY:
			if (bIncludeBlank || !pTDI->sAllocBy.IsEmpty())
				pTDIChild->sAllocBy = pTDI->sAllocBy;
			break;
			
		case TDCA_STATUS:
			if (bIncludeBlank || !pTDI->sStatus.IsEmpty())
				pTDIChild->sStatus = pTDI->sStatus;
			break;
			
		case TDCA_CATEGORY:
			if (bIncludeBlank || pTDI->aCategories.GetSize())
				pTDIChild->aCategories.Copy(pTDI->aCategories);
			break;
			
		case TDCA_TAGS:
			if (bIncludeBlank || pTDI->aTags.GetSize())
				pTDIChild->aTags.Copy(pTDI->aTags);
			break;
			
		case TDCA_PERCENT:
			if (bIncludeBlank || pTDI->nPercentDone)
				pTDIChild->nPercentDone = pTDI->nPercentDone;
			break;
			
		case TDCA_TIMEEST:
			if (bIncludeBlank || pTDI->dTimeEstimate > 0)
				pTDIChild->dTimeEstimate = pTDI->dTimeEstimate;
			break;
			
		case TDCA_FILEREF:
			if (bIncludeBlank || pTDI->aFileLinks.GetSize())
				pTDIChild->aFileLinks.Copy(pTDI->aFileLinks);
			break;
			
		case TDCA_VERSION:
			if (bIncludeBlank || !pTDI->sVersion.IsEmpty())
				pTDIChild->sVersion = pTDI->sVersion;
			break;
			
		case TDCA_FLAG:
			if (bIncludeBlank || pTDI->bFlagged)
				pTDIChild->bFlagged = pTDI->bFlagged;
			break;
			
		case TDCA_EXTERNALID:
			if (bIncludeBlank || !pTDI->sExternalID.IsEmpty())
				pTDIChild->sExternalID = pTDI->sExternalID;
			break;
			
		default:
			ASSERT (0);
			return FALSE;
		}

		// and its children too
		ApplyLastChangeToSubtasks(pTDIChild, pTDS->GetSubTask(nSubTask), nAttrib, bIncludeBlank);
	}
	
	return TRUE;
}

TDC_SET CToDoCtrlData::SetTaskColor(DWORD dwTaskID, COLORREF color)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);
	
	// if the color is 0 then add 1 to discern from unset
	if (color == 0)
	{
		color = 1;
	}
	else if (color == CLR_NONE) 
	{
		color = 0;
	}
			
	return EditTaskAttributeT(dwTaskID, pTDI, TDCA_COLOR, pTDI->color, color);
}

TDC_SET CToDoCtrlData::SetTaskComments(DWORD dwTaskID, const CString& sComments, const CBinaryData& customComments, const CString& sCommentsTypeID)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);
	
	BOOL bCommentsChange = (pTDI->sComments != sComments);
	BOOL bCustomCommentsChange = (bCommentsChange || (pTDI->sCommentsTypeID != sCommentsTypeID) || (pTDI->customComments != customComments));
	
	if (bCommentsChange || bCustomCommentsChange)
	{
		// save undo data
		SaveEditUndo(dwTaskID, pTDI, TDCA_COMMENTS);
		
		if (bCommentsChange)
			pTDI->sComments = sComments;
		
		if (bCustomCommentsChange)
		{
			// if we're changing comments type we clear the custom comments
			if (pTDI->sCommentsTypeID != sCommentsTypeID)
				pTDI->customComments.Empty();
			else
				pTDI->customComments = customComments;
			
			pTDI->sCommentsTypeID = sCommentsTypeID;
		}
		
		pTDI->SetModified();
		
		return SET_CHANGE;
	}
	
	// else
	return SET_NOCHANGE;
}

TDC_SET CToDoCtrlData::SetTaskCommentsType(DWORD dwTaskID, const CString& sCommentsTypeID)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);
	
	return EditTaskAttributeT(dwTaskID, pTDI, TDCA_COMMENTS, pTDI->sCommentsTypeID, sCommentsTypeID);
}

TDC_SET CToDoCtrlData::SetTaskTitle(DWORD dwTaskID, const CString& sTitle)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);
	
	return EditTaskAttributeT(dwTaskID, pTDI, TDCA_TASKNAME, pTDI->sTitle, sTitle);
}

TDC_SET CToDoCtrlData::SetTaskIcon(DWORD dwTaskID, const CString& sIcon)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);
	
	return EditTaskAttributeT(dwTaskID, pTDI, TDCA_ICON, pTDI->sIcon, sIcon);
}

TDC_SET CToDoCtrlData::SetTaskFlag(DWORD dwTaskID, BOOL bFlagged)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);
	
	return EditTaskAttributeT(dwTaskID, pTDI, TDCA_FLAG, pTDI->bFlagged, bFlagged);
}

TDC_SET CToDoCtrlData::SetTaskCustomAttributeData(DWORD dwTaskID, const CString& sAttribID, const CString& sData)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);
	
	CString sOldData = GetTaskCustomAttributeData(dwTaskID, sAttribID);
	
	if (sOldData != sData)
	{
		// save undo data
		SaveEditUndo(dwTaskID, pTDI, TDCA_CUSTOMATTRIB);
		
		// make changes
		pTDI->SetCustomAttribValue(sAttribID, sData);
		
		return SET_CHANGE;
	}
	return SET_NOCHANGE;
}

TDC_SET CToDoCtrlData::SetTaskRecurrence(DWORD dwTaskID, const TDCRECURRENCE& tr)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	if (pTDI->IsDone())
		return SET_FAILED;

	if (pTDI->trRecurrence != tr)
	{
		// save undo data
		SaveEditUndo(dwTaskID, pTDI, TDCA_RECURRENCE);
		
		// make changes
		pTDI->trRecurrence = tr;
		pTDI->SetModified();
		
		return SET_CHANGE;
	}
	return SET_NOCHANGE;
}

TDC_SET CToDoCtrlData::SetTaskPriority(DWORD dwTaskID, int nPriority)
{
	if (!(nPriority == FM_NOPRIORITY || (nPriority >= 0 && nPriority <= 10)))
		return SET_FAILED;

	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);
	
	return EditTaskAttributeT(dwTaskID, pTDI, TDCA_PRIORITY, pTDI->nPriority, nPriority);
}

TDC_SET CToDoCtrlData::SetTaskRisk(DWORD dwTaskID, int nRisk)
{
	if (!(nRisk == FM_NORISK || (nRisk >= 0 && nRisk <= 10)))
		return SET_FAILED;

	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	return EditTaskAttributeT(dwTaskID, pTDI, TDCA_RISK, pTDI->nRisk, nRisk);
}

// external version
TDC_SET CToDoCtrlData::SetTaskDate(DWORD dwTaskID, TDC_DATE nDate, const COleDateTime& date)
{
	return SetTaskDate(dwTaskID, NULL, nDate, date);
}

// internal version
TDC_SET CToDoCtrlData::SetTaskDate(DWORD dwTaskID, TODOITEM* pTDI, TDC_DATE nDate, const COleDateTime& date, BOOL bRecalcTimeEstimate)
{
	if (!pTDI)
		EDIT_GET_TDI(dwTaskID, pTDI);

	COleDateTime dtDate(date);
	BOOL bDateIsSet = CDateHelper::IsDateSet(dtDate);
	
	// whole days only for creation or 'end of day'
	if ((nDate == TDCD_CREATE) || (bDateIsSet && IsEndOfDay(dtDate)))
	{
		dtDate = CDateHelper::GetDateOnly(dtDate);
	}
	
	const COleDateTime dtCur = pTDI->GetDate(nDate);
	
	if (pTDI->GetDate(nDate) != dtDate)
	{
		// save undo data
		SaveEditUndo(dwTaskID, pTDI, TDC::MapDateToAttribute(nDate));
		
		switch (nDate)
		{
		case TDCD_CREATE:
			pTDI->dateCreated = dtDate;	

			bRecalcTimeEstimate = FALSE;
			break;
			
		case TDCD_START:	
			pTDI->dateStart = dtDate;		
			break;
			
		case TDCD_STARTDATE:		
			// Add date to existing time component unless existing date is 0.0
			if (!bDateIsSet || !pTDI->HasStart())
				pTDI->dateStart = CDateHelper::GetDateOnly(dtDate);
			else
				pTDI->dateStart = CDateHelper::MakeDate(dtDate, pTDI->dateStart);	

			// If the task does NOT have a due date but does have a time estimate
			// then calculate an appropriate due date
			if (HasStyle(TDCS_SYNCTIMEESTIMATESANDDATES) && 
				CalcMissingDueDateFromStart(pTDI))
			{
				nDate = TDCD_DUE; // to update dependencies
				bRecalcTimeEstimate = FALSE;
			}
			break;
			
		case TDCD_STARTTIME:		
			// add time to date component only if it exists
			if (pTDI->HasStart())
				pTDI->dateStart = CDateHelper::MakeDate(pTDI->dateStart, dtDate);		
			break;
			
		case TDCD_DUE:		
			pTDI->dateDue = dtDate;		
			break;
			
		case TDCD_DUEDATE:		
			// add date to existing time component unless existing date is 0.0
			if (!bDateIsSet || !pTDI->HasDue())
				pTDI->dateDue = CDateHelper::GetDateOnly(dtDate);
			else
				pTDI->dateDue = CDateHelper::MakeDate(dtDate, pTDI->dateDue);		

			// If the task does NOT have a start date but does have a time estimate
			// then back-calculate an appropriate start date
			if (HasStyle(TDCS_SYNCTIMEESTIMATESANDDATES) &&
				CalcMissingStartDateFromDue(pTDI))
			{
				bRecalcTimeEstimate = FALSE;
			}
			break;
			
		case TDCD_DUETIME:		
			// add time to date component only if it exists
			if (pTDI->HasDue())
				pTDI->dateDue = CDateHelper::MakeDate(pTDI->dateDue, dtDate);		
			break;
			
		case TDCD_DONE:		
			{
				BOOL bWasDone = pTDI->IsDone();
				pTDI->dateDone = dtDate;
				
				// reset % completion if going from done to not-done
				if (bWasDone && !pTDI->IsDone() && pTDI->nPercentDone == 100)
					pTDI->nPercentDone = 0;
	
				bRecalcTimeEstimate = FALSE;
			}
			break;
			
		case TDCD_DONEDATE:		
			// add date to existing time component unless date is 0.0
			if (!bDateIsSet || !pTDI->IsDone())
				pTDI->dateDone = CDateHelper::GetDateOnly(dtDate);
			else
				pTDI->dateDone = CDateHelper::MakeDate(dtDate, pTDI->dateDone);		
			
			bRecalcTimeEstimate = FALSE;
			break;
			
		case TDCD_DONETIME:		
			// add time to date component only if it exists
			if (pTDI->IsDone())
				pTDI->dateDone = CDateHelper::MakeDate(pTDI->dateDone, dtDate);		
			
			bRecalcTimeEstimate = FALSE;
			break;
			
		default:
			ASSERT(0);
			return SET_FAILED;
		}
		
		pTDI->SetModified();
		
		// update dependent dates
		FixupTaskLocalDependentsDates(dwTaskID, nDate);

		// and time estimates
		if (bRecalcTimeEstimate)
			RecalcTaskTimeEstimate(dwTaskID, pTDI, nDate);
		
		return SET_CHANGE;
	}
	return SET_NOCHANGE;
}

TDC_SET CToDoCtrlData::OffsetTaskDate(DWORD dwTaskID, TDC_DATE nDate, int nAmount, TDC_UNITS nUnits, 
										BOOL bAndSubtasks, BOOL bFitToRecurringScheme)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	DH_UNITS nDHUnits = TDC::MapUnitsToDHUnits(nUnits);
	COleDateTime date = pTDI->GetDate(nDate);

	if (!CDateHelper::IsDateSet(date))
		return SET_FAILED;

	CDateHelper::OffsetDate(date, nAmount, nDHUnits);

	// Special case: Task is recurring and therefore must fall on a valid date
	if (pTDI->IsRecurring() && bFitToRecurringScheme)
		pTDI->trRecurrence.FitDayToScheme(date);

	TDC_SET nRes = SetTaskDate(dwTaskID, pTDI, nDate, date);
	
	// children
	if (bAndSubtasks)
	{
		const TODOSTRUCTURE* pTDS = LocateTask(dwTaskID);

		if (!pTDS)
		{
			ASSERT(0);
			return SET_FAILED;
		}
		
		for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
		{
			DWORD dwChildID = pTDS->GetSubTaskID(nSubTask);
			
			if (OffsetTaskDate(dwChildID, nDate, nAmount, nUnits, TRUE, bFitToRecurringScheme) == SET_CHANGE)
				nRes = SET_CHANGE;
		}
	}
	
	return nRes;
}

TDC_SET CToDoCtrlData::MoveTaskDates(DWORD dwTaskID, const COleDateTime& dtNewStart)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	if (!(pTDI->HasStart() && (pTDI->HasDue() || (pTDI->dTimeEstimate > 0.0))))
	{
		ASSERT(0);
		return SET_FAILED;
	}

	if (dtNewStart == pTDI->dateStart)
	{
		ASSERT(0);
		return SET_NOCHANGE;
	}

	// Cache current time estimate before doing anything
	double dDuration = pTDI->dTimeEstimate;
	
	if (dDuration == 0.0)
		dDuration = CalcDuration(pTDI->dateStart, pTDI->dateDue, pTDI->nTimeEstUnits);

	// recalc due date
	COleDateTime dtStart(dtNewStart);
	COleDateTime dtNewDue = AddDuration(dtStart, dDuration, pTDI->nTimeEstUnits);

	TDC_SET nRes = SetTaskDate(dwTaskID, pTDI, TDCD_START, dtStart, TRUE);
	ASSERT(nRes == SET_CHANGE);

	if (nRes == SET_CHANGE)
		SetTaskDate(dwTaskID, pTDI, TDCD_DUE, dtNewDue);

	return nRes;
}

TDC_SET CToDoCtrlData::InitMissingTaskDate(DWORD dwTaskID, TDC_DATE nDate, const COleDateTime& date, BOOL bAndSubtasks)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);
	
	COleDateTime dtTask = pTDI->GetDate(nDate);
	TDC_SET nRes = SET_NOCHANGE;

	if (!CDateHelper::IsDateSet(dtTask))
		nRes = SetTaskDate(dwTaskID, pTDI, nDate, date);
	
	// children
	if (bAndSubtasks)
	{
		TODOSTRUCTURE* pTDS = m_struct.FindTask(dwTaskID);
		ASSERT(pTDS);

		if (pTDS)
		{
			for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
			{
				DWORD dwChildID = pTDS->GetSubTaskID(nSubTask);
				
				if (InitMissingTaskDate(dwChildID, nDate, date, TRUE) == SET_CHANGE)
					nRes = SET_CHANGE;
			}
		}
	}
	
	return nRes;

}

TDC_SET CToDoCtrlData::SetTaskPercent(DWORD dwTaskID, int nPercent)
{
	if (nPercent < 0 || nPercent > 100)
		return SET_FAILED;
	
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	return EditTaskAttributeT(dwTaskID, pTDI, TDCA_PERCENT, pTDI->nPercentDone, nPercent);
}

TDC_SET CToDoCtrlData::SetTaskCost(DWORD dwTaskID, double dCost)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	return EditTaskAttributeT(dwTaskID, pTDI, TDCA_COST, pTDI->dCost, dCost);
}

TDC_SET CToDoCtrlData::SetTaskTimeEstimate(DWORD dwTaskID, double dTime, TDC_UNITS nUnits)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	double dOrgEstimate = pTDI->dTimeEstimate;
	TDC_UNITS nOrgUnits = pTDI->nTimeEstUnits;
	
	TDC_SET nRes = EditTaskTimeAttribute(dwTaskID, pTDI, TDCA_TIMEEST, pTDI->dTimeEstimate, dTime, pTDI->nTimeEstUnits, nUnits);

	if ((nRes == SET_CHANGE) && HasStyle(TDCS_SYNCTIMEESTIMATESANDDATES))
	{
		// recalc due date but only if the duration 'really' changed
		// and the task has either a due date or a start date
		BOOL bTimeChange = ((dOrgEstimate != dTime) || ((dTime != 0.0) && (nOrgUnits != nUnits)));
		
		if (bTimeChange && (pTDI->HasStart() || pTDI->HasDue()))
		{
			// Make sure the task has a start date
			CalcMissingStartDateFromDue(pTDI);

			COleDateTime dtNewDue = AddDuration(pTDI->dateStart, dTime, nUnits);
			SetTaskDate(dwTaskID, pTDI, TDCD_DUE, dtNewDue, FALSE);
		}
	}

	return nRes;
}

BOOL CToDoCtrlData::CalcMissingStartDateFromDue(TODOITEM* pTDI) const
{
	if (pTDI->HasStart() || !pTDI->HasDue() || (pTDI->dTimeEstimate <= 0.0))
		return FALSE;

	// Subtract time estimate from due date
	pTDI->dateStart = AddDuration(pTDI->dateDue, -pTDI->dTimeEstimate, pTDI->nTimeEstUnits);
	return TRUE;
}

BOOL CToDoCtrlData::CalcMissingDueDateFromStart(TODOITEM* pTDI) const
{
	if (!pTDI->HasStart() || pTDI->HasDue() || (pTDI->dTimeEstimate <= 0.0))
		return FALSE;

	// Add time estimate to start date
	pTDI->dateDue = AddDuration(pTDI->dateStart, pTDI->dTimeEstimate, pTDI->nTimeEstUnits);
	return TRUE;
}

TDC_SET CToDoCtrlData::RecalcTaskTimeEstimate(DWORD dwTaskID, TODOITEM* pTDI, TDC_DATE nDate)
{
	if (HasStyle(TDCS_SYNCTIMEESTIMATESANDDATES))
	{
		// only applies to due and start dates
		switch (nDate)
		{
		case TDCD_STARTDATE:
		case TDCD_START:
		case TDCD_STARTTIME:
		case TDCD_DUEDATE:	
		case TDCD_DUE:
		case TDCD_DUETIME:
			{
				ASSERT(pTDI);
				
				COleDateTime dateStart = pTDI->dateStart, dateDue = pTDI->dateDue;
				
				if (pTDI->HasStart() && pTDI->HasDue() && (dateDue >= dateStart)) // both exist
				{
					double dDuration = CalcDuration(dateStart, dateDue, pTDI->nTimeEstUnits);
					ASSERT(dDuration > 0.0);

					return EditTaskTimeAttribute(dwTaskID, pTDI, TDCA_TIMEEST, pTDI->dTimeEstimate, dDuration, pTDI->nTimeEstUnits, pTDI->nTimeEstUnits);
				}
			}
			break;
		}
	}

	// else
	return SET_NOCHANGE;
}

TDC_SET CToDoCtrlData::SetTaskTimeSpent(DWORD dwTaskID, double dTime, TDC_UNITS nUnits)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);
	
	return EditTaskTimeAttribute(dwTaskID, pTDI, TDCA_TIMESPENT, pTDI->dTimeSpent, dTime, pTDI->nTimeSpentUnits, nUnits);
}

void CToDoCtrlData::ResetRecurringSubtaskOcurrences(DWORD dwTaskID)
{
	const TODOSTRUCTURE* pTDS = m_struct.FindTask(dwTaskID);

	if (!pTDS)
	{
		ASSERT(0);
		return;
	}
	
	for (int nSubtask = 0; nSubtask < pTDS->GetSubTaskCount(); nSubtask++)
	{
		DWORD dwSubtaskID = pTDS->GetSubTaskID(nSubtask);

		TODOITEM* pTDI = GetTask(dwSubtaskID);
		ASSERT(pTDI);
		
		if (pTDI)
		{
			if (pTDI->IsRecurring())
			{
				int nNumOccur = pTDI->trRecurrence.GetOccurrenceCount();
				pTDI->trRecurrence.SetOccurrenceCount(nNumOccur, nNumOccur);
			}

			// then its subtasks
			ResetRecurringSubtaskOcurrences(dwSubtaskID);
		}
	}
}

TDC_SET CToDoCtrlData::SetTaskArray(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, const CStringArray& aItems, BOOL bAppend)
{
	switch (nAttrib)
	{
	case TDCA_CATEGORY:		return SetTaskCategories(dwTaskID, aItems, bAppend);
	case TDCA_TAGS:			return SetTaskTags(dwTaskID, aItems, bAppend);
	case TDCA_ALLOCTO:		return SetTaskAllocTo(dwTaskID, aItems, bAppend);
	case TDCA_DEPENDENCY:	return SetTaskDependencies(dwTaskID, aItems, bAppend);
	case TDCA_FILEREF:		return SetTaskFileRefs(dwTaskID, aItems, bAppend);
		break;
	}

	// all else
	ASSERT(0);
	return SET_FAILED;
}

int CToDoCtrlData::GetTaskArray(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, CStringArray& aItems)
{
	switch (nAttrib)
	{
	case TDCA_CATEGORY:		return GetTaskCategories(dwTaskID, aItems);
	case TDCA_TAGS:			return GetTaskTags(dwTaskID, aItems);
	case TDCA_ALLOCTO:		return GetTaskAllocTo(dwTaskID, aItems);
	case TDCA_DEPENDENCY:	return GetTaskDependencies(dwTaskID, aItems);
	case TDCA_FILEREF:		return GetTaskFileRefs(dwTaskID, aItems);
		break;
	}

	// all else
	ASSERT(0);
	return 0;
}

TDC_SET CToDoCtrlData::SetTaskAllocTo(DWORD dwTaskID, const CStringArray& aAllocTo, BOOL bAppend)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);
	
	return EditTaskArrayAttribute(dwTaskID, pTDI, TDCA_ALLOCTO, pTDI->aAllocTo, aAllocTo, bAppend);
}

TDC_SET CToDoCtrlData::SetTaskAllocBy(DWORD dwTaskID, const CString& sAllocBy)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	return EditTaskAttributeT(dwTaskID, pTDI, TDCA_ALLOCBY, pTDI->sAllocBy, sAllocBy);
}

TDC_SET CToDoCtrlData::SetTaskVersion(DWORD dwTaskID, const CString& sVersion)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	return EditTaskAttributeT(dwTaskID, pTDI, TDCA_VERSION, pTDI->sVersion, sVersion);
}

TDC_SET CToDoCtrlData::SetTaskStatus(DWORD dwTaskID, const CString& sStatus)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);
	
	return EditTaskAttributeT(dwTaskID, pTDI, TDCA_STATUS, pTDI->sStatus, sStatus);
}

TDC_SET CToDoCtrlData::SetTaskCategories(DWORD dwTaskID, const CStringArray& aCategories, BOOL bAppend)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);
	
	return EditTaskArrayAttribute(dwTaskID, pTDI, TDCA_CATEGORY, pTDI->aCategories, aCategories, bAppend);
}

TDC_SET CToDoCtrlData::SetTaskTags(DWORD dwTaskID, const CStringArray& aTags, BOOL bAppend)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);
	
	return EditTaskArrayAttribute(dwTaskID, pTDI, TDCA_TAGS, pTDI->aTags, aTags, bAppend);
}

TDC_SET CToDoCtrlData::SetTaskDependencies(DWORD dwTaskID, const CStringArray& aDepends, BOOL bAppend)
{
	// weed out 'unknown' tasks and parent tasks
	const TODOSTRUCTURE* pTDS = LocateTask(dwTaskID);
	ASSERT(pTDS);
	
	if (!pTDS)
		return SET_FAILED;

	CStringArray aWeeded;
	aWeeded.Copy(aDepends);

	int nDepends = aWeeded.GetSize();

	while (nDepends--)
	{
		DWORD dwDepends = _ttol(aWeeded[nDepends]);

		if (dwDepends)
		{
			if (!HasTask(dwDepends)) // check for existence
			{
				aWeeded.RemoveAt(nDepends);
			}
			else // check for parent
			{
				while (pTDS)
				{
					if (dwDepends == pTDS->GetParentTaskID())
					{
						aWeeded.RemoveAt(nDepends);
						break;
					}

					// next parent up
					pTDS = pTDS->GetParentTask();
				}
			}
		}
	}

	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	TDC_SET nRes = EditTaskArrayAttribute(dwTaskID, pTDI, TDCA_DEPENDENCY, pTDI->aDependencies, aWeeded, bAppend);

	if (nRes == SET_CHANGE)
	{
		// make sure our start date matches our dependents due date
		if (HasStyle(TDCS_AUTOADJUSTDEPENDENCYDATES))
		{
			UINT nAdjusted = UpdateTaskLocalDependencyDates(dwTaskID, TDCD_DUE);

			if (nAdjusted == ADJUSTED_NONE)
				nAdjusted = UpdateTaskLocalDependencyDates(dwTaskID, TDCD_DONE);

			// and then fix up our dependents
			if (Misc::HasFlag(nAdjusted, ADJUSTED_DUE))
				FixupTaskLocalDependentsDates(dwTaskID, TDCD_DUE);
		}
	}

	return nRes;
}

TDC_SET CToDoCtrlData::SetTaskExtID(DWORD dwTaskID, const CString& sID)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);
	
	return EditTaskAttributeT(dwTaskID, pTDI, TDCA_EXTERNALID, pTDI->sExternalID, sID);
}

BOOL CToDoCtrlData::SaveEditUndo(DWORD dwTaskID, TODOITEM* pTDI, TDC_ATTRIBUTE nAttrib)
{
	if (m_undo.IsActive())										
		return m_undo.SaveElement(TDCUEO_EDIT, dwTaskID, 0, 0, (WORD)nAttrib, pTDI);

	// else
	return FALSE;
}

TDC_SET CToDoCtrlData::EditTaskArrayAttribute(DWORD dwTaskID, TODOITEM* pTDI, TDC_ATTRIBUTE nAttrib, CStringArray& aValues, 
										 	const CStringArray& aNewValues, BOOL bAppend, BOOL bOrderSensitive)
{
	ASSERT(dwTaskID);
	ASSERT(pTDI);
	ASSERT(nAttrib != TDCA_NONE);
	
	// test for actual change
	if (Misc::MatchAll(aValues, aNewValues, bOrderSensitive))
		return SET_NOCHANGE;
	
	// save undo data
	SaveEditUndo(dwTaskID, pTDI, nAttrib);
	
	// make the change
	if (bAppend)
		Misc::AddUniqueItems(aNewValues, aValues);
	else
		aValues.Copy(aNewValues);

	pTDI->SetModified();
				
	// Update subtasks
	ApplyLastInheritedChangeToSubtasks(dwTaskID, nAttrib);
	
	return SET_CHANGE;
}

TDC_SET CToDoCtrlData::EditTaskTimeAttribute(DWORD dwTaskID, TODOITEM* pTDI, TDC_ATTRIBUTE nAttrib, 
											 double& dValue, double dNewValue, TDC_UNITS& nUnits, TDC_UNITS nNewUnits)
{
	ASSERT(dwTaskID);
	ASSERT(pTDI);
	ASSERT((nAttrib == TDCA_TIMEEST) || (nAttrib == TDCA_TIMESPENT));
	
	// test for actual change
	if ((dValue == dNewValue) && (nUnits == nNewUnits))
		return SET_NOCHANGE;
	
	// save undo data
	SaveEditUndo(dwTaskID, pTDI, nAttrib);
	
	// make the change
	dValue = dNewValue;
	nUnits = nNewUnits;

	pTDI->SetModified();
				
	// Update subtasks
	ApplyLastInheritedChangeToSubtasks(dwTaskID, nAttrib);
	
	return SET_CHANGE;
}

TDC_SET CToDoCtrlData::SetTaskFileRefs(DWORD dwTaskID, const CStringArray& aFileRefs, BOOL bAppend)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);
	
	return EditTaskArrayAttribute(dwTaskID, pTDI, TDCA_FILEREF, pTDI->aFileLinks, aFileRefs, bAppend, TRUE);
}

BOOL CToDoCtrlData::BeginNewUndoAction(TDCUNDOACTIONTYPE nType)
{
	return m_undo.BeginNewAction(nType);
}

BOOL CToDoCtrlData::EndCurrentUndoAction()
{
	return m_undo.EndCurrentAction();
}

BOOL CToDoCtrlData::ExtendLastUndoAction(TDCUNDOACTIONTYPE nType)
{
	return m_undo.ExtendLastAction(nType);
}

BOOL CToDoCtrlData::AddUndoElement(TDCUNDOELMOP nOp, DWORD dwTaskID, DWORD dwParentID, DWORD dwPrevSiblingID, WORD wFlags)
{
	if (!m_undo.IsActive())
		return FALSE;
	
	const TODOITEM* pTDI = GetTask(dwTaskID, FALSE);
	ASSERT (pTDI);
	
	if (!pTDI)
		return FALSE;
	
	// save task state
	VERIFY (m_undo.SaveElement(nOp, dwTaskID, dwParentID, dwPrevSiblingID, wFlags, pTDI));
	
	// save children RECURSIVELY if an add
	if (nOp == TDCUEO_ADD)
	{
		const TODOSTRUCTURE* pTDS = m_struct.FindTask(dwTaskID);
		dwPrevSiblingID = 0; // reuse
		dwParentID = dwTaskID; // reuse
		
		for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
		{
			TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
			ASSERT(pTDSChild);
			
			dwTaskID = pTDSChild->GetTaskID(); // reuse
			VERIFY (AddUndoElement(nOp, dwTaskID, dwParentID, dwPrevSiblingID));
			
			dwPrevSiblingID = dwTaskID;
		}
	}
	
	return TRUE;
}

int CToDoCtrlData::GetLastUndoActionTaskIDs(BOOL bUndo, CDWordArray& aIDs) const 
{
	return bUndo ? m_undo.GetLastUndoActionTaskIDs(aIDs) : m_undo.GetLastRedoActionTaskIDs(aIDs);
}

TDCUNDOACTIONTYPE CToDoCtrlData::GetLastUndoActionType(BOOL bUndo) const
{
	return (bUndo ? m_undo.GetLastUndoType() : m_undo.GetLastRedoType());
}

BOOL CToDoCtrlData::DeleteLastUndoAction()
{
	return m_undo.DeleteLastUndoAction();
}

BOOL CToDoCtrlData::UndoLastAction(BOOL bUndo, CArrayUndoElements& aElms)
{
	aElms.RemoveAll();
	TDCUNDOACTION* pAction = bUndo ? m_undo.UndoLastAction() : m_undo.RedoLastAction();
	
	if (!pAction)
		return FALSE;
	
	// restore elements
	int nNumElm = pAction->aElements.GetSize();
	
	// note: if we are undoing then we need to undo in the reverse order
	// of the initial edits unless it was a move because moves always
	// work off the first item.
	int nStart = 0, nEnd = nNumElm, nInc = 1;
	
	if (bUndo && pAction->nType != TDCUAT_MOVE)
	{
		nStart = nNumElm - 1;
		nEnd = -1;
		nInc = -1;
	}
	
	// copy the structre because we're going to be changing it and we need 
	// to be able to lookup the original previous sibling IDs for undo info
	CToDoCtrlStructure tdsCopy(m_struct);
	
	// now undo
	for (int nElm = nStart; nElm != nEnd; nElm += nInc)
	{
		TDCUNDOELEMENT& elm = pAction->aElements[nElm];
		
		if (elm.nOp == TDCUEO_EDIT)
		{
			TODOITEM* pTDI = NULL;
			GET_TDI(elm.dwTaskID, pTDI, FALSE);
			ASSERT(pTDI);
			
			// copy current task state so we can update redo info
			TODOITEM tdiRedo = *pTDI;
			*pTDI = elm.tdi;
			elm.tdi = tdiRedo;
			
			// no need to return this item nothing to be done
		}
		else if ((elm.nOp == TDCUEO_ADD && bUndo) || (elm.nOp == TDCUEO_DELETE && !bUndo))
		{
			// this is effectively a delete so make the returned elem that way
			TDCUNDOELEMENT elmRet(TDCUEO_DELETE, elm.dwTaskID);
			aElms.Add(elmRet);
			
			DeleteTask(elm.dwTaskID);
		}
		else if ((elm.nOp == TDCUEO_DELETE && bUndo) || (elm.nOp == TDCUEO_ADD && !bUndo))
		{
			// this is effectively an add so make the returned elem that way
			TDCUNDOELEMENT elmRet(TDCUEO_ADD, elm.dwTaskID, elm.dwParentID, elm.dwPrevSiblingID);
			aElms.Add(elmRet);
			
			// restore task
			TODOITEM* pTDI = GetTask(elm.dwTaskID, FALSE);
			ASSERT(pTDI == NULL);
			
			if (!pTDI)
			{
				pTDI = NewTask(elm.tdi);
				AddTask(elm.dwTaskID, pTDI, elm.dwParentID, elm.dwPrevSiblingID);
			}
		}
		else if (elm.nOp == TDCUEO_MOVE)
		{
			TDCUNDOELEMENT elmRet(TDCUEO_MOVE, elm.dwTaskID, elm.dwParentID, elm.dwPrevSiblingID);
			aElms.Add(elmRet);
			
			MoveTask(elm.dwTaskID, elm.dwParentID, elm.dwPrevSiblingID);
			
			// adjust undo element so these changes can be undone
			elm.dwParentID = tdsCopy.GetParentTaskID(elm.dwTaskID);
			elm.dwPrevSiblingID = tdsCopy.GetPreviousTaskID(elm.dwTaskID);
		}
		else
			return FALSE;
	}
	
	return TRUE;
}

BOOL CToDoCtrlData::CanUndoLastAction(BOOL bUndo) const
{
	return bUndo ? m_undo.CanUndo() : m_undo.CanRedo();
}

BOOL CToDoCtrlData::MoveTask(DWORD dwTaskID, DWORD dwDestParentID, DWORD dwDestPrevSiblingID)
{
	// get source location
	TODOSTRUCTURE* pTDSSrcParent = NULL;
	int nSrcPos = 0;
	
	if (!LocateTask(dwTaskID, pTDSSrcParent, nSrcPos))
	{
		ASSERT(0);
		return FALSE;
	}
	
	DWORD dwPrevSiblingID = pTDSSrcParent->GetPreviousSubTaskID(nSrcPos);
	
	// get destination
	TODOSTRUCTURE* pTDSDestParent = NULL;
	int nDestPos = -1;
	
	if (Locate(dwDestParentID, dwDestPrevSiblingID, pTDSDestParent, nDestPos))
	{
		// we want the location after the dest previous sibling
		nDestPos++;
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}

	// check that a move is actually happening
	if ((pTDSDestParent == pTDSSrcParent) && (nDestPos == nSrcPos))
		return FALSE;

	return (MoveTask(pTDSSrcParent, nSrcPos, dwPrevSiblingID, pTDSDestParent, nDestPos) != -1);
}

int CToDoCtrlData::MoveTask(TODOSTRUCTURE* pTDSSrcParent, int nSrcPos, DWORD dwSrcPrevSiblingID,
							TODOSTRUCTURE* pTDSDestParent, int nDestPos)
{
	DWORD dwTaskID = pTDSSrcParent->GetSubTaskID(nSrcPos);
	DWORD dwSrcParentID = pTDSSrcParent->GetTaskID();

	// check if there's anyhting to do
	if ((pTDSSrcParent == pTDSDestParent) && (nSrcPos == nDestPos))
		return -1;
	
	// save undo info
	AddUndoElement(TDCUEO_MOVE, dwTaskID, dwSrcParentID, dwSrcPrevSiblingID);
	
	int nPos = pTDSSrcParent->MoveSubTask(nSrcPos, pTDSDestParent, nDestPos);
	
	if (nPos != -1)
	{
		// mark affected tasks as modified
		SetTaskModified(dwTaskID);
		SetTaskModified(dwSrcParentID);
		
		// mark dest if not same as source
		DWORD dwDestParentID = pTDSDestParent->GetTaskID();
		
		if (dwDestParentID != dwSrcParentID)
			SetTaskModified(dwDestParentID);
	}
	
	return nPos;
}

void CToDoCtrlData::FixupParentCompletion(DWORD dwParentID)
{
	if (!dwParentID) // top level item 
		return;
		
	// if the parent was marked as done and it has any incomplete
	// subtasks then mark the parent as incomplete too
	const TODOITEM* pTDI = GetTask(dwParentID);
	ASSERT(pTDI);
	
	if (pTDI && pTDI->IsDone())
	{
		if (TaskHasIncompleteSubtasks(dwParentID, FALSE))
		{
			// work our way up the chain setting parent to incomplete
			const TODOSTRUCTURE* pTDS = LocateTask(dwParentID);
			ASSERT(pTDS);

			do 
			{
				SetTaskDate(pTDS->GetTaskID(), TDCD_DONE, 0.0);
				pTDS = pTDS->GetParentTask();
			} 
			while(pTDS);
		}
	}
}

BOOL CToDoCtrlData::MoveTasks(const CDWordArray& aTaskIDs, DWORD dwDestParentID, DWORD dwDestPrevSiblingID)
{
	if (aTaskIDs.GetSize() == 0) 
	{
		return FALSE;
	}
	else if (aTaskIDs.GetSize() == 1)
	{
		return MoveTask(aTaskIDs[0], dwDestParentID, dwDestPrevSiblingID);
	}
	
	// copy the structure because we're going to be changing it and we need 
	// to be able to lookup the original previous sibling IDs for undo info
	CToDoCtrlStructure tdsCopy(m_struct);
	
	// get destination location
	TODOSTRUCTURE* pTDSDestParent = NULL;
	int nDestPos = -1;
	
	if (!Locate(dwDestParentID, dwDestPrevSiblingID, pTDSDestParent, nDestPos))
	{
		ASSERT(0);
		return FALSE;
	}
	else // we want the location after the dest previous sibling
	{
		nDestPos++;
	}
	
	// move source tasks 
	int nMoved = 0;

	for (int nTask = 0; nTask < aTaskIDs.GetSize(); nTask++, nDestPos++)
	{
		TODOSTRUCTURE* pTDSSrcParent = NULL;
		int nSrcPos = -1;

		DWORD dwTaskID = aTaskIDs[nTask];

		if (!LocateTask(dwTaskID, pTDSSrcParent, nSrcPos))
		{
			ASSERT(0);
			return FALSE;
		}
		
		// get previous subtask ID, using our copy of the task structure 
		// because the original task structure may already have been altered
		TODOSTRUCTURE* pTDSDummy = NULL;
		int nDummyPos = -1;

		if (tdsCopy.FindTask(dwTaskID, pTDSDummy, nDummyPos))
		{
			DWORD dwPrevSiblingID = pTDSDummy->GetPreviousSubTaskID(nDummyPos);
			nDestPos = MoveTask(pTDSSrcParent, nSrcPos, dwPrevSiblingID, pTDSDestParent, nDestPos);
			
			if (nDestPos != -1)
				nMoved++;
		}
		else
		{
			ASSERT(0);
			return FALSE;
		}
	}

	if (nMoved)
		ApplyLastInheritedChangeToSubtasks(dwDestParentID, TDCA_ALL);

	return nMoved;
}

BOOL CToDoCtrlData::SetTaskModified(DWORD dwTaskID)
{
	if (dwTaskID == 0)
		return TRUE; // not an error

	TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);
	
	pTDI->SetModified();
	return TRUE;
}

BOOL CToDoCtrlData::TaskHasIncompleteSubtasks(DWORD dwTaskID, BOOL bExcludeRecurring) const
{
	const TODOSTRUCTURE* pTDS = LocateTask(dwTaskID);

	if (pTDS)
		return TaskHasIncompleteSubtasks(pTDS, bExcludeRecurring);

	ASSERT(0);
	return FALSE;
}

BOOL CToDoCtrlData::TaskHasIncompleteSubtasks(const TODOSTRUCTURE* pTDS, BOOL bExcludeRecurring) const
{
	// process its subtasks
	int nPos = pTDS->GetSubTaskCount();

	while (nPos--)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nPos);
		const TODOITEM* pTDIChild = GetTask(pTDSChild);
		
		// ignore recurring tasks and their children
		if (bExcludeRecurring && IsTaskRecurring(pTDIChild, pTDSChild, TRUE))
			continue;

		// ignore completed tasks and their children
		if (pTDIChild->IsDone())
			continue;

		// test for leaf-tasks or parents that do not depend
		// on their children's completed state
		if (!pTDSChild->HasSubTasks() || !HasStyle(TDCS_TREATSUBCOMPLETEDASDONE))
			return TRUE;
		
		// test its subtasks
		if (TaskHasIncompleteSubtasks(pTDSChild, bExcludeRecurring)) // RECURSIVE call
			return TRUE;
	}

	return FALSE;
}

BOOL CToDoCtrlData::IsParentTaskDone(DWORD dwTaskID) const
{
	const TODOSTRUCTURE* pTDS = LocateTask(dwTaskID);
	return IsParentTaskDone(pTDS);
}

BOOL CToDoCtrlData::IsParentTaskDone(const TODOSTRUCTURE* pTDS) const
{
	ASSERT (pTDS);
	
	if (!pTDS || pTDS->ParentIsRoot())
		return FALSE;
	
	const TODOSTRUCTURE* pTDSParent = pTDS->GetParentTask();
	const TODOITEM* pTDIParent = GetTask(pTDSParent);
	
	ASSERT (pTDIParent && pTDSParent);
	
	if (!pTDIParent || !pTDSParent)
		return FALSE;
	
	if (pTDIParent->IsDone())
		return TRUE;
	
	// else check parent's parent
	return IsParentTaskDone(pTDSParent);
}

BOOL CToDoCtrlData::GetTaskSubtaskTotals(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS,
									 int& nSubtasksCount, int& nSubtasksDone) const
{
	ASSERT (pTDS && pTDI);
	
	if (!pTDS || !pTDS->HasSubTasks() || !pTDI)
		return FALSE;
	
	nSubtasksDone = nSubtasksCount = 0;
	
	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		nSubtasksCount++;
		
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
		const TODOITEM* pTDIChild = GetTask(pTDSChild);
		
		if (IsTaskDone(pTDIChild, pTDSChild, TDCCHECKCHILDREN))
			nSubtasksDone++;
	}
	
	return (nSubtasksCount > 0);
}

CString CToDoCtrlData::GetTaskPositionString(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	ASSERT (pTDI && pTDS && pTDS->GetParentTask());
	
	if (!pTDI || !pTDS)
		return EMPTY_STR;
	
	const TODOSTRUCTURE* pTDSParent = pTDS->GetParentTask();

	if (!pTDSParent)
		return EMPTY_STR;

	CString sPosition;
	
	if (pTDSParent->IsRoot())
	{
		sPosition = Misc::Format(pTDS->GetPosition() + 1);
	}
	else
	{
		TODOITEM* pTDIParent = GetTask(pTDSParent);
		ASSERT (pTDIParent);
		
		if (!pTDIParent)
			return EMPTY_STR;
		
		CString sParentPos = GetTaskPositionString(pTDIParent, pTDSParent);
		sPosition.Format(_T("%s.%d"), sParentPos, pTDS->GetPosition() + 1);
	}

	return sPosition;
}

CString CToDoCtrlData::GetTaskPath(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	ASSERT (pTDS && pTDI);
	
	if (!pTDS || !pTDI)
		return EMPTY_STR;
	
	const TODOSTRUCTURE* pTDSParent = pTDS->GetParentTask();
	
	if (!pTDSParent)
		return EMPTY_STR;

	CString sPath;

	while (pTDSParent && !pTDSParent->IsRoot())
	{
		TODOITEM* pTDIParent = GetTask(pTDSParent);
		ASSERT (pTDIParent);
		
		if (!pTDIParent)
			return EMPTY_STR;
		
		sPath = (pTDIParent->sTitle + "\\" + sPath);
		
		pTDSParent = pTDSParent->GetParentTask();
	}

	return sPath;
}

int CToDoCtrlData::CalcTaskPercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	ASSERT (pTDS && pTDI);
	
	if (!pTDS || !pTDI)
		return 0;
	
	int nPercent = 0;
	
	if (!HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION) || !pTDS->HasSubTasks())
	{
		if (pTDI->IsDone())
		{
			nPercent = 100;
		}
		else if(HasStyle(TDCS_AUTOCALCPERCENTDONE))
		{
			nPercent = CalcPercentFromTime(pTDI, pTDS);
		}
		else
		{
			nPercent = pTDI->nPercentDone;
		}
	}
	else if (HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION)) // has subtasks and we must average their completion
	{
		// note: we have separate functions for weighted/unweighted
		// just to keep the logic for each as clear as possible
		if (HasStyle(TDCS_WEIGHTPERCENTCALCBYNUMSUB))
		{
			nPercent = (int)Misc::Round(SumWeightedPercentDone(pTDI, pTDS));
		}
		else
		{
			nPercent = (int)Misc::Round(SumPercentDone(pTDI, pTDS));
		}
	}
		
	return nPercent;
}

int CToDoCtrlData::CalcPercentFromTime(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	ASSERT (pTDS && pTDI);
	
	if (!pTDS || !pTDI)
		return 0;
	
	ASSERT (HasStyle(TDCS_AUTOCALCPERCENTDONE)); // sanity check
	
	double dSpent = CalcTaskTimeSpent(pTDI, pTDS, TDCU_HOURS);
	double dUnused, dEstimate = CalcTaskTimeEstimate(pTDI, pTDS, TDCU_HOURS, dUnused);
	
	if (dSpent > 0 && dEstimate > 0)
		return min(100, (int)(100 * dSpent / dEstimate));
	else
		return 0;
}

double CToDoCtrlData::SumPercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	ASSERT (pTDS && pTDI);
	
	if (!pTDS || !pTDI)
		return 0;
	
	ASSERT (HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION)); // sanity check
	
	if (!pTDS->HasSubTasks() || pTDI->IsDone())
	{
		// base percent
		if (pTDI->IsDone())
		{
			return 100;
		}
		else if(HasStyle(TDCS_AUTOCALCPERCENTDONE))
		{
			return CalcPercentFromTime(pTDI, pTDS);
		}
		else
		{
			return pTDI->nPercentDone;
		}
	}

	// else aggregate child percentages
	// optionally ignoring completed tasks
	BOOL bIncludeDone = HasStyle(TDCS_INCLUDEDONEINAVERAGECALC);
	int nNumSubtasks = 0, nNumDoneSubtasks = 0;

	if (bIncludeDone)
	{
		nNumSubtasks = pTDS->GetSubTaskCount();
		ASSERT(nNumSubtasks);
	}
	else
	{
		GetTaskSubtaskTotals(pTDI, pTDS, nNumSubtasks, nNumDoneSubtasks);

		if (nNumDoneSubtasks == nNumSubtasks) // all subtasks are completed
			return 0;
	}
	
	// Get default done value for each child (ex.4 child = 25, 3 child = 33.33, etc.)
	double dSplitDoneValue = (1.0 / (nNumSubtasks - nNumDoneSubtasks)); 
	double dTotalPercentDone = 0;
	
	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
		ASSERT(pTDSChild);
		
		const TODOITEM* pTDIChild = GetTask(pTDSChild);
		ASSERT(pTDIChild);

		if (pTDSChild && pTDIChild)
		{
			if (HasStyle(TDCS_INCLUDEDONEINAVERAGECALC) || !IsTaskDone(pTDIChild, pTDSChild, TDCCHECKALL))
			{
				// add percent per child(eg. 2 child = 50 each if 1st child 
				// has 75% completed then will add 50*75/100 = 37.5)
				dTotalPercentDone += dSplitDoneValue * SumPercentDone(pTDIChild, pTDSChild);
			}
		}
	}

	return dTotalPercentDone;
}

int CToDoCtrlData::GetTaskLeafCount(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bIncludeDone) const
{
	// sanity check
	ASSERT(pTDS && pTDI);

	if (!pTDS || !pTDI)
		return 0;

	if (bIncludeDone)
	{
		return pTDS->GetLeafCount();
	}
	else if (pTDI->IsDone())
	{
		return 0;
	}
	else if (!pTDS->HasSubTasks())
	{
		return 1;
	}

	// else traverse sub items
	int nLeafCount = 0;

	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
		const TODOITEM* pTDIChild = GetTask(pTDSChild); 

		nLeafCount += GetTaskLeafCount(pTDIChild, pTDSChild, bIncludeDone);
	}

	ASSERT(nLeafCount);
	return nLeafCount;
}

double CToDoCtrlData::SumWeightedPercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	// sanity check
	ASSERT (pTDS && pTDI);
	
	if (!pTDS || !pTDI)
		return 0;
	
	ASSERT (HasStyle(TDCS_AVERAGEPERCENTSUBCOMPLETION)); // sanity check
	ASSERT (HasStyle(TDCS_WEIGHTPERCENTCALCBYNUMSUB)); // sanity check
	
	if (!pTDS->HasSubTasks() || pTDI->IsDone())
	{
		if (pTDI->IsDone())
		{
			return 100;
		}
		else if(HasStyle(TDCS_AUTOCALCPERCENTDONE))
		{
			return CalcPercentFromTime(pTDI, pTDS);
		}
		else
		{
			return pTDI->nPercentDone;
		}
	}

	// calculate the total number of task leaves for this task
	// we will proportion our children percentages against these values
	int nTotalNumSubtasks = GetTaskLeafCount(pTDI, pTDS, HasStyle(TDCS_INCLUDEDONEINAVERAGECALC));
	double dTotalPercentDone = 0;

	// process the children multiplying the split percent by the 
	// proportion of this subtask's subtasks to the whole
	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
		const TODOITEM* pTDIChild = GetTask(pTDSChild);

		int nChildNumSubtasks = GetTaskLeafCount(pTDIChild, pTDSChild, HasStyle(TDCS_INCLUDEDONEINAVERAGECALC));

		if (HasStyle(TDCS_INCLUDEDONEINAVERAGECALC) || !IsTaskDone(pTDIChild, pTDSChild, TDCCHECKCHILDREN))
		{
			double dChildPercent = SumWeightedPercentDone(pTDIChild, pTDSChild);

			dTotalPercentDone += dChildPercent * ((double)nChildNumSubtasks / (double)nTotalNumSubtasks);
		}
	}

	return dTotalPercentDone;
}

double CToDoCtrlData::CalcTaskCost(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	// sanity check
	ASSERT (pTDS && pTDI);
	
	if (!pTDS || !pTDI)
		return 0.0;
	
	double dCost = pTDI->dCost; // own cost
		
	if (pTDS->HasSubTasks())
	{
		for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
		{
			const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
			const TODOITEM* pTDIChild = GetTask(pTDSChild);
			
			dCost += CalcTaskCost(pTDIChild, pTDSChild);
		}
	}
		
	return dCost;
}

void CToDoCtrlData::SetDefaultCommentsFormat(const CString& format) 
{ 
	s_cfDefault = format; 
}

void CToDoCtrlData::SetDefaultTimeUnits(TDC_UNITS nTimeEstUnits, TDC_UNITS nTimeSpentUnits)
{
	s_nDefTimeEstUnits = nTimeEstUnits;
	s_nDefTimeSpentUnits = nTimeSpentUnits;
}

TDC_UNITS CToDoCtrlData::GetBestCalcTimeEstUnits(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	// sanity check
	ASSERT (pTDS && pTDI);
	
	if (!pTDS || !pTDI)
		return s_nDefTimeEstUnits;

	TDC_UNITS nUnits = s_nDefTimeEstUnits;

	if (pTDI->dTimeEstimate > 0)
	{
		nUnits = pTDI->nTimeEstUnits;
	}
	else if (pTDS->HasSubTasks())
	{
		DWORD dwID = pTDS->GetSubTaskID(0);

		if (GetTask(dwID, pTDI, pTDS, FALSE))
			nUnits = GetBestCalcTimeEstUnits(pTDI, pTDS); // RECURSIVE CALL
	}

	return nUnits;
}

TDC_UNITS CToDoCtrlData::GetBestCalcTimeSpentUnits(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	// sanity check
	ASSERT (pTDS && pTDI);
	
	if (!pTDS || !pTDI)
		return s_nDefTimeSpentUnits;

	TDC_UNITS nUnits = s_nDefTimeSpentUnits;

	if (pTDI->dTimeSpent > 0)
	{
		nUnits = pTDI->nTimeSpentUnits;
	}
	else if (pTDS->HasSubTasks())
	{
		DWORD dwID = pTDS->GetSubTaskID(0);

		if (GetTask(dwID, pTDI, pTDS, FALSE))
			nUnits = GetBestCalcTimeSpentUnits(pTDI, pTDS); // RECURSIVE CALL
	}

	return nUnits;
}

// external version
double CToDoCtrlData::CalcTaskTimeEstimate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS nUnits) const
{
	BOOL bReturnWeighted = HasStyle(TDCS_USEPERCENTDONEINTIMEEST);
	
	double dWeightedEstimate;
	double dEstimate = CalcTaskTimeEstimate(pTDI, pTDS, nUnits, dWeightedEstimate);

	return (bReturnWeighted ? dWeightedEstimate : dEstimate);
}

// internal version
double CToDoCtrlData::CalcTaskTimeEstimate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, 
										TDC_UNITS nUnits, double& dWeightedEstimate) const
{
	// sanity check
	ASSERT (pTDS && pTDI);
	
	if (!pTDS || !pTDI)
		return 0.0;
	
	double dEstimate = 0.0;
	dWeightedEstimate = 0.0;

	BOOL bIsParent = pTDS->HasSubTasks();
	
	if (!bIsParent || HasStyle(TDCS_ALLOWPARENTTIMETRACKING))
	{
		dEstimate = CTimeHelper().GetTime(pTDI->dTimeEstimate, pTDI->GetTHTimeUnits(TRUE), THU_HOURS);
		
		// DON'T WEIGHT BY PERCENT if we are auto-calculating
		// percent-done, because that will recurse back into here
		if (!HasStyle(TDCS_AUTOCALCPERCENTDONE))
		{
			int nPercent = CalcTaskPercentDone(pTDI, pTDS);
			dWeightedEstimate = (dEstimate * ((100 - nPercent) / 100.0));
		}
		else
		{
			dWeightedEstimate = dEstimate;
		}
	}
	
	if (bIsParent) // children's time
	{
		for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
		{
			const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
			const TODOITEM* pTDIChild = GetTask(pTDSChild);

			double dChildWeightedEstimate = 0.0;
			
			dEstimate += CalcTaskTimeEstimate(pTDIChild, pTDSChild, TDCU_HOURS, dChildWeightedEstimate);
			dWeightedEstimate += dChildWeightedEstimate;
		}
	}
	
	// Estimate is calculated internally in hours so we need to convert it to nUnits
	CTimeHelper th;
	
	dEstimate = th.GetTime(dEstimate, THU_HOURS, TDC::MapUnitsToTHUnits(nUnits));
	dWeightedEstimate = th.GetTime(dWeightedEstimate, THU_HOURS, TDC::MapUnitsToTHUnits(nUnits));

	return dEstimate;
}

double CToDoCtrlData::CalcTaskRemainingTime(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS& nUnits) const
{
	// sanity check
	ASSERT (pTDS && pTDI);
	
	if (!pTDS || !pTDI)
		return 0.0;

	double dRemain = 0.0, dWeightedEstimate;

	if (HasStyle(TDCS_CALCREMAININGTIMEBYDUEDATE))
	{
		COleDateTime date = CalcTaskDueDate(pTDI, pTDS);
		nUnits = TDCU_DAYS; // always
		
		if (CDateHelper::IsDateSet(date)) 
			dRemain = TODOITEM::GetRemainingDueTime(date);
	}
	else
	{
		nUnits = pTDI->nTimeEstUnits;
		dRemain = CalcTaskTimeEstimate(pTDI, pTDS, nUnits, dWeightedEstimate);

		if (dRemain > 0)
		{
			if (HasStyle(TDCS_CALCREMAININGTIMEBYPERCENT))
			{
				dRemain = dWeightedEstimate;
			}
			else if (HasStyle(TDCS_CALCREMAININGTIMEBYSPENT))
			{
				dRemain -= CalcTaskTimeSpent(pTDI, pTDS, nUnits);
			}
		}
	}

	return dRemain;
}

double CToDoCtrlData::CalcTaskTimeSpent(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS nUnits) const
{
	// sanity check
	ASSERT (pTDS && pTDI);
	
	if (!pTDS || !pTDI)
		return 0.0;
	
	double dSpent = 0;
	BOOL bIsParent = pTDS->HasSubTasks();
	
	// task's own time
	if (!bIsParent || HasStyle(TDCS_ALLOWPARENTTIMETRACKING))
		dSpent = CTimeHelper().GetTime(pTDI->dTimeSpent, pTDI->GetTHTimeUnits(FALSE), THU_HOURS);
	
	if (bIsParent) // children's time
	{
		for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
		{
			const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
			const TODOITEM* pTDIChild = GetTask(pTDSChild);
			
			dSpent += CalcTaskTimeSpent(pTDIChild, pTDSChild, TDCU_HOURS);
		}
	}
	
	// convert it back from hours to nUnits
	return CTimeHelper().GetTime(dSpent, THU_HOURS, TDC::MapUnitsToTHUnits(nUnits));
}

BOOL CToDoCtrlData::IsTaskStarted(DWORD dwTaskID, BOOL bToday) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, FALSE);

	return IsTaskStarted(pTDI, pTDS, bToday);
}

BOOL CToDoCtrlData::IsTaskStarted(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bToday) const
{
	// sanity check
	ASSERT (pTDS && pTDI);
	
	if (!pTDS || !pTDI)
		return FALSE;
	
	double dStarted = CalcTaskStartDate(pTDI, pTDS);

	if (bToday)
		return CDateHelper::IsToday(dStarted);

	// else
	return ((dStarted > 0) && (dStarted < COleDateTime::GetCurrentTime()));
}

BOOL CToDoCtrlData::IsTaskDue(DWORD dwTaskID, BOOL bToday) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, FALSE);

	return IsTaskDue(pTDI, pTDS, bToday);
}

BOOL CToDoCtrlData::IsTaskOverDue(DWORD dwTaskID) const
{
	return IsTaskDue(dwTaskID, FALSE);
}

BOOL CToDoCtrlData::IsTaskOverDue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	// we need to check that it's due BUT not due today
	return IsTaskDue(pTDI, pTDS, FALSE) && !IsTaskDue(pTDI, pTDS, TRUE);
}

BOOL CToDoCtrlData::HasOverdueTasks() const
{
	if (GetTaskCount() == 0)
		return FALSE;
	
	COleDateTime dtToday = CDateHelper::GetDate(DHD_TODAY);
	double dEarliest = GetEarliestDueDate();

	return ((dEarliest > 0.0) && (dEarliest < dtToday));
}

BOOL CToDoCtrlData::HasDueTodayTasks() const
{
	if (GetTaskCount() == 0)
		return FALSE;
	
	// search all tasks for first due today
	return HasDueTodayTasks(GetStructure());
}

BOOL CToDoCtrlData::HasDueTodayTasks(const TODOSTRUCTURE* pTDS) const
{
	// sanity check
	ASSERT(pTDS);

	if (!pTDS)
		return FALSE;

	if (pTDS->GetTaskID())
	{
		const TODOITEM* pTDI = NULL;
		DWORD dwTaskID = pTDS->GetTaskID();

		GET_TDI(dwTaskID, pTDI, FALSE);

		if (IsTaskDue(pTDI, pTDS, TRUE))
			return TRUE;
	}

	// subtasks
	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
		
		if (HasDueTodayTasks(pTDSChild))
			return TRUE;
	}

	// no due todays found
	return FALSE;
}

double CToDoCtrlData::GetEarliestDueDate() const
{
	double dEarliest = DBL_MAX;
	
	// traverse top level items
	for (int nSubTask = 0; nSubTask < m_struct.GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDS = m_struct.GetSubTask(nSubTask);
		const TODOITEM* pTDI = GetTask(pTDS);
		
		double dTaskEarliest = CalcStartDueDate(pTDI, pTDS, TRUE, TRUE, TRUE);
		
		if (dTaskEarliest > 0.0)
			dEarliest = min(dTaskEarliest, dEarliest);
	}
	
	return ((dEarliest == DBL_MAX) ? 0.0 : dEarliest);
}

BOOL CToDoCtrlData::GetTask(DWORD& dwTaskID, const TODOITEM*& pTDI, const TODOSTRUCTURE*& pTDS, BOOL bTrue) const
{
	// get item and structure
	pTDI = GetTask(dwTaskID, bTrue);
	pTDS = LocateTask(dwTaskID);

	// we only need to assert if one could be found but not the other
	ASSERT((pTDI && pTDS) || (!pTDI && (!pTDS || pTDS->IsRoot())));

	if (!pTDI || !pTDS)
	{
		pTDI = NULL;
		pTDS = NULL;
		return FALSE;
	}
	
	return TRUE;
}

DWORD CToDoCtrlData::GetTrueTaskID(DWORD dwTaskID) const
{
	DWORD dwTaskRefID = GetTaskReferenceID(dwTaskID);
	
	if (dwTaskRefID)
		return dwTaskRefID;
	
	// all else
	return dwTaskID;
}

void CToDoCtrlData::GetTrueTaskIDs(CDWordArray& aTaskIDs) const
{
	int nID = aTaskIDs.GetSize();

	while (nID--)
		aTaskIDs[nID] = GetTrueTaskID(aTaskIDs[nID]);
}

BOOL CToDoCtrlData::IsTaskReferenced(DWORD dwTaskID) const
{
	// sanity check
	ASSERT(dwTaskID);

	// if it is already a reference then it cannot have refs itself
	if (dwTaskID == 0 || IsTaskReference(dwTaskID))
		return FALSE;

	return IsTaskReferenced(dwTaskID, &m_struct);
}

BOOL CToDoCtrlData::IsTaskReferenced(DWORD dwTaskID, const TODOSTRUCTURE* pTDS) const
{
	// sanity check
	ASSERT(dwTaskID && pTDS);

	if (!dwTaskID || !pTDS)
		return FALSE;
	
	// test this task
	if (GetTaskReferenceID(pTDS->GetTaskID()) == dwTaskID)
		return TRUE;

	// then its children
	for (int nChild = 0; nChild < pTDS->GetSubTaskCount(); nChild++)
	{
		if (IsTaskReferenced(dwTaskID, pTDS->GetSubTask(nChild)))
			return TRUE;
	}

	// else
	return FALSE;
}

int CToDoCtrlData::GetReferencesToTask(DWORD dwTaskID, CDWordArray& aRefIDs) const
{
	// sanity check
	ASSERT(dwTaskID);

	aRefIDs.RemoveAll();

	if (dwTaskID)
		return GetReferencesToTask(dwTaskID, &m_struct, aRefIDs);

	// else
	return 0;
}

int CToDoCtrlData::GetReferencesToTask(DWORD dwTaskID, const TODOSTRUCTURE* pTDS, CDWordArray& aRefIDs) const
{
	// sanity check
	ASSERT(dwTaskID && pTDS);

	if (!dwTaskID || !pTDS)
		return 0;
	
	// test this task
	if (GetTaskReferenceID(pTDS->GetTaskID()) == dwTaskID)
		aRefIDs.Add(pTDS->GetTaskID());

	// then its children
	for (int nChild = 0; nChild < pTDS->GetSubTaskCount(); nChild++)
	{
		GetReferencesToTask(dwTaskID, pTDS->GetSubTask(nChild), aRefIDs);
	}

	return aRefIDs.GetSize();
}

int CToDoCtrlData::GetTaskHighestPriority(DWORD dwTaskID, BOOL bIncludeDue) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, FM_NOPRIORITY);
		
	return GetTaskHighestPriority(pTDI, pTDS, bIncludeDue);
}

int CToDoCtrlData::GetTaskHighestRisk(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, FM_NORISK);
		
	return GetTaskHighestRisk(pTDI, pTDS);
}

double CToDoCtrlData::CalcTaskDueDate(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, 0);
		
	return CalcTaskDueDate(pTDI, pTDS);
}

double CToDoCtrlData::CalcTaskStartDate(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, 0);
		
	return CalcTaskStartDate(pTDI, pTDS);
}

CString CToDoCtrlData::FormatTaskAllocTo(DWORD dwTaskID) const
{
	return FormatTaskAllocTo(GetTask(dwTaskID));
}

CString CToDoCtrlData::FormatTaskCategories(DWORD dwTaskID) const
{
	return FormatTaskCategories(GetTask(dwTaskID));
}

CString CToDoCtrlData::FormatTaskTags(DWORD dwTaskID) const
{
	return FormatTaskTags(GetTask(dwTaskID));
}

double CToDoCtrlData::CalcTaskCost(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, 0.0);
		
	return CalcTaskCost(pTDI, pTDS);
}

int CToDoCtrlData::CalcTaskPercentDone(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, 0);
		
	return CalcTaskPercentDone(pTDI, pTDS);
}

double CToDoCtrlData::CalcTaskTimeEstimate(DWORD dwTaskID, TDC_UNITS nUnits) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, 0.0);
		
	return CalcTaskTimeEstimate(pTDI, pTDS, nUnits);
}

double CToDoCtrlData::CalcTaskTimeSpent(DWORD dwTaskID, TDC_UNITS nUnits) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, 0.0);
		
	return CalcTaskTimeSpent(pTDI, pTDS, nUnits);
}

double CToDoCtrlData::CalcTaskRemainingTime(DWORD dwTaskID, TDC_UNITS& nUnits) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, 0.0);
		
	return CalcTaskRemainingTime(pTDI, pTDS, nUnits);
}

BOOL CToDoCtrlData::GetTaskSubtaskTotals(DWORD dwTaskID, int& nSubtasksTotal, int& nSubtasksDone) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, FALSE);

	return GetTaskSubtaskTotals(pTDI, pTDS, nSubtasksTotal, nSubtasksDone);
}

BOOL CToDoCtrlData::IsTaskDone(DWORD dwTaskID, DWORD dwExtraCheck) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, FALSE);

	return IsTaskDone(pTDI, pTDS, dwExtraCheck);
}

BOOL CToDoCtrlData::IsTaskDue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bToday) const
{
	// sanity check
	ASSERT (pTDS && pTDI);
	
	if (!pTDS || !pTDI)
		return FALSE;
	
 	double dDue = CalcTaskDueDate(pTDI, pTDS);

	if (bToday)
		return CDateHelper::IsToday(dDue);

	// else
	return ((dDue > 0) && (dDue < COleDateTime::GetCurrentTime()));
}

double CToDoCtrlData::CalcTaskDueDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	if (HasStyle(TDCS_USEEARLIESTDUEDATE))
	{
		return CalcStartDueDate(pTDI, pTDS, TRUE, TRUE, TRUE);
	}
	else if (HasStyle(TDCS_USELATESTDUEDATE))
	{
		return CalcStartDueDate(pTDI, pTDS, TRUE, TRUE, FALSE);
	}

	// else
	return CalcStartDueDate(pTDI, pTDS, FALSE, TRUE, FALSE);
}

double CToDoCtrlData::CalcTaskStartDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	if (HasStyle(TDCS_USEEARLIESTSTARTDATE))
	{
		return CalcStartDueDate(pTDI, pTDS, TRUE, FALSE, TRUE);
	}
	else if (HasStyle(TDCS_USELATESTSTARTDATE))
	{
		return CalcStartDueDate(pTDI, pTDS, TRUE, FALSE, FALSE);
	}

	// else
	return CalcStartDueDate(pTDI, pTDS, FALSE, FALSE, FALSE);
}

double CToDoCtrlData::CalcStartDueDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bCheckChildren, BOOL bDue, BOOL bEarliest) const
{
	// sanity check
	ASSERT (pTDS && pTDI);
	
	if (!pTDS || !pTDI)
		return 0.0;
	
	BOOL bDone = IsTaskDone(pTDI, pTDS, TDCCHECKCHILDREN);
	double dBest = 0;
	
	if (bDone)
	{
		// nothing to do
	}
	else if (bCheckChildren && pTDS->HasSubTasks())
	{
		// initialize dBest to Parent's dates
		if (bDue)
			dBest = GetBestDate(dBest, pTDI->dateDue.m_dt, FALSE); // FALSE = latest
		else
			dBest = GetBestDate(dBest, pTDI->dateStart.m_dt, FALSE); // FALSE = latest

		// handle pTDI not having dates
		if (dBest == 0.0)
			dBest = (bEarliest ? DBL_MAX : -DBL_MAX);
		
		// check children
		for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
		{
			const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
			const TODOITEM* pTDIChild = GetTask(pTDSChild);

			ASSERT (pTDSChild && pTDIChild);
			
			double dChildDate = CalcStartDueDate(pTDIChild, pTDSChild, TRUE, bDue, bEarliest);
			dBest = GetBestDate(dBest, dChildDate, bEarliest);
		}
		
		if (fabs(dBest) == DBL_MAX) // no children had dates
			dBest = 0;
	}
	else // leaf task
	{
		dBest = (bDue ? pTDI->dateDue : pTDI->dateStart).m_dt;
	}
	
	if (bDue)
	{
		// finally if no date set then use today or start whichever is later
		if ((dBest == 0) && !bDone && HasStyle(TDCS_NODUEDATEISDUETODAYORSTART))
		{
			COleDateTime dtDue(CDateHelper::GetDate(DHD_TODAY));

			if (CDateHelper::Max(dtDue, pTDI->dateStart))
				dBest = dtDue.m_dt;
		}
	}
	
	return dBest;
}

double CToDoCtrlData::GetBestDate(double dBest, double dDate, BOOL bEarliest)
{
	if (dDate == 0.0)
	{
		return dBest;
	}
	else if (bEarliest)
	{
		return min(dDate, dBest);
	}
	
	// else
	return max(dDate, dBest);
}

int CToDoCtrlData::GetTaskHighestPriority(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bIncludeDue) const
{
	// sanity check
	ASSERT (pTDS && pTDI);
	
	if (!pTDS || !pTDI)
		return -1;
	
	int nHighest = pTDI->nPriority;
	
	if (pTDI->IsDone() && !HasStyle(TDCS_INCLUDEDONEINPRIORITYCALC) && HasStyle(TDCS_DONEHAVELOWESTPRIORITY))
	{
		nHighest = min(nHighest, MIN_TDPRIORITY);
	}
	else if (nHighest < MAX_TDPRIORITY)
	{
		if (bIncludeDue && HasStyle(TDCS_DUEHAVEHIGHESTPRIORITY) && IsTaskDue(pTDI, pTDS))
		{
			nHighest = MAX_TDPRIORITY;
		}
		else if (HasStyle(TDCS_USEHIGHESTPRIORITY) && pTDS->HasSubTasks())
		{
			// check children
			for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
			{
				const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
				const TODOITEM* pTDIChild = GetTask(pTDSChild);

				ASSERT (pTDSChild && pTDIChild);
				
				if (HasStyle(TDCS_INCLUDEDONEINPRIORITYCALC) || !IsTaskDone(pTDIChild, pTDSChild, TDCCHECKALL))
				{
					int nChildHighest = GetTaskHighestPriority(pTDIChild, pTDSChild, bIncludeDue);
					
					// optimization
					if (nChildHighest == MAX_TDPRIORITY)
					{
						nHighest = MAX_TDPRIORITY;
						break;
					}
					else
						nHighest = max(nChildHighest, nHighest);
				}
			}
		}
	}
	
	return nHighest;
}

int CToDoCtrlData::GetTaskHighestRisk(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const
{
	// sanity check
	ASSERT (pTDS && pTDI);
	
	if (!pTDS || !pTDI)
		return -1;
	
	// some optimizations
	int nHighest = pTDI->nRisk;
	
	if (pTDI->IsDone() && !HasStyle(TDCS_INCLUDEDONEINRISKCALC) && HasStyle(TDCS_DONEHAVELOWESTRISK))
	{
		nHighest = min(nHighest, MIN_TDRISK);
	}
	else if (nHighest < MAX_TDRISK)
	{
		if (HasStyle(TDCS_USEHIGHESTRISK) && pTDS->HasSubTasks())
		{
			// check children
			nHighest = max(nHighest, FM_NORISK);//MIN_TDRISK;
			
			for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
			{
				const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
				const TODOITEM* pTDIChild = GetTask(pTDSChild);

				ASSERT (pTDSChild && pTDIChild);
				
				if (HasStyle(TDCS_INCLUDEDONEINRISKCALC) || !IsTaskDone(pTDIChild, pTDSChild, TDCCHECKALL))
				{
					  int nChildHighest = GetTaskHighestRisk(pTDIChild, pTDSChild);
					  
					  // optimization
					  if (nChildHighest == MAX_TDRISK)
					  {
						  nHighest = MAX_TDRISK;
						  break;
					  }
					  else
						  nHighest = max(nChildHighest, nHighest);
				}
			}
		}
	}
	
	return nHighest;
}

CString CToDoCtrlData::FormatTaskAllocTo(const TODOITEM* pTDI) const
{
	// sanity check
	ASSERT(pTDI);
	
	if (!pTDI)
		return EMPTY_STR;

	return Misc::FormatArray(pTDI->aAllocTo);
}

CString CToDoCtrlData::FormatTaskCategories(const TODOITEM* pTDI) const
{
	// sanity check
	ASSERT(pTDI);
	
	if (!pTDI)
		return EMPTY_STR;
	
	return Misc::FormatArray(pTDI->aCategories);
}

CString CToDoCtrlData::FormatTaskTags(const TODOITEM* pTDI) const
{
	// sanity check
	ASSERT(pTDI);
	
	if (!pTDI)
		return EMPTY_STR;
	
	return Misc::FormatArray(pTDI->aTags);
}

BOOL CToDoCtrlData::IsTaskDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, DWORD dwExtraCheck) const
{
	// sanity check
	ASSERT (pTDS && pTDI);
	
	if (!pTDS || !pTDI)
	{
		return FALSE;
	}

	// simple checks
	if (pTDS->IsRoot())
	{
		return FALSE;
	}
	else if (pTDI->IsDone())
	{
		return TRUE;
	}
	else if (dwExtraCheck == 0)
	{
		return FALSE;
	}

	// extra checking
	BOOL bDone = FALSE;
	
	// check parent first because it's quicker
	if (dwExtraCheck & TDCCHECKPARENT)
		bDone = IsParentTaskDone(pTDS);
	
	// else check children for 'good-as-done'
	BOOL bCheckChildren = (!bDone && 
							(dwExtraCheck & TDCCHECKCHILDREN) && 
							HasStyle(TDCS_TREATSUBCOMPLETEDASDONE) &&
							pTDS->HasSubTasks());

	if (bCheckChildren)
		bDone = (FALSE == TaskHasIncompleteSubtasks(pTDS, FALSE));
	
	return bDone;
}

BOOL CToDoCtrlData::IsTaskTimeTrackable(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, FALSE);
	
	// not trackable if complete
	if (pTDI->IsDone())
		return FALSE;

	// not trackable if a container
	return (HasStyle(TDCS_ALLOWPARENTTIMETRACKING) || !pTDS->HasSubTasks());
}

int CToDoCtrlData::CompareTasks(DWORD dwTask1ID, DWORD dwTask2ID, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bAscending) const
{
	const TODOITEM* pTDI1 = NULL, *pTDI2 = NULL;
	const TODOSTRUCTURE* pTDS1 = NULL, *pTDS2 = NULL;

	GET_TDI_TDS(dwTask1ID, pTDI1, pTDS1, 0);
	GET_TDI_TDS(dwTask2ID, pTDI2, pTDS2, 0);

	// handle 'sort done below'
	BOOL bSortDoneBelow = HasStyle(TDCS_SORTDONETASKSATBOTTOM);
		
	if (bSortDoneBelow)
	{
		BOOL bDone1 = IsTaskDone(pTDI1, pTDS1, TDCCHECKALL);
		BOOL bDone2 = IsTaskDone(pTDI2, pTDS2, TDCCHECKALL);
			
		if (bDone1 != bDone2)
			return bDone1 ? 1 : -1;
	}

	// else compare actual data if 
	int nCompare = 0;
	double dVal1, dVal2;

	if (CalcTaskCustomAttributeData(pTDI1, pTDS1, attribDef, dVal1) &&
		CalcTaskCustomAttributeData(pTDI2, pTDS2, attribDef, dVal2))
	{
		nCompare = Compare(dVal1, dVal2);
	}
	else
	{
		nCompare = Compare(pTDI1->GetCustomAttribValue(attribDef.sUniqueID), 
							pTDI2->GetCustomAttribValue(attribDef.sUniqueID));
	}

	return bAscending ? nCompare : -nCompare;
}


int CToDoCtrlData::CompareTasks(DWORD dwTask1ID, DWORD dwTask2ID, TDC_COLUMN nSortBy, BOOL bAscending, 
								BOOL bSortDueTodayHigh, BOOL bIncStartTime, BOOL bIncDueTime, BOOL bIncDoneTime) const
{
	// sanity check
	ASSERT(dwTask1ID && dwTask2ID && (dwTask1ID != dwTask2ID));

	if (!dwTask1ID  || !dwTask2ID || (dwTask1ID == dwTask2ID))
		return 0;

	int nCompare = 0;

	// special case: sort by ID can be optimized
	if (nSortBy == TDCC_ID)
	{
		nCompare = (dwTask1ID - dwTask2ID);
	}
	else
	{
		const TODOITEM* pTDI1 = NULL, *pTDI2 = NULL;
		const TODOSTRUCTURE* pTDS1 = NULL, *pTDS2 = NULL;

		GET_TDI_TDS(dwTask1ID, pTDI1, pTDS1, 0);
		GET_TDI_TDS(dwTask2ID, pTDI2, pTDS2, 0);

		// special case: 'unsorted' == (sort by position + ascending)
		if (nSortBy == TDCC_NONE)
		{
			nSortBy = TDCC_POSITION;
			bAscending = TRUE;
		}

		// figure out if either or both tasks are completed
		// but only if the user has specified to sort these differently
		BOOL bHideDone = HasStyle(TDCS_HIDESTARTDUEFORDONETASKS);
		BOOL bSortDoneBelow = HasStyle(TDCS_SORTDONETASKSATBOTTOM);
		
		BOOL bDone1 = IsTaskDone(pTDI1, pTDS1, TDCCHECKALL);
		BOOL bDone2 = IsTaskDone(pTDI2, pTDS2, TDCCHECKALL);

		// can also do a partial optimization
		if (bSortDoneBelow && 
			(nSortBy != TDCC_DONE) && 
			(nSortBy != TDCC_DONEDATE) && 
			(nSortBy != TDCC_POSITION))
		{
			if (bDone1 != bDone2)
				return bDone1 ? 1 : -1;
		}

		// else default attribute
		switch (nSortBy)
		{
		case TDCC_POSITION:
			nCompare = Compare(GetTaskPositionString(pTDI1, pTDS1), GetTaskPositionString(pTDI2, pTDS2));
			break;
			
		case TDCC_PATH:
			nCompare = Compare(GetTaskPath(pTDI1, pTDS1), GetTaskPath(pTDI2, pTDS2));
			break;
			
		case TDCC_CLIENT:
			nCompare = Compare(pTDI1->sTitle, pTDI2->sTitle);
			break;
			
		case TDCC_DONE:
			nCompare = Compare(bDone1, bDone2);
			break;
			
		case TDCC_FLAG:
			nCompare = Compare(pTDI1->bFlagged, pTDI2->bFlagged);
			break;
			
		case TDCC_RECURRENCE:
			nCompare = Compare(pTDI1->trRecurrence.GetRegularity(), pTDI2->trRecurrence.GetRegularity());
			break;
			
		case TDCC_VERSION:
			nCompare = FileMisc::CompareVersions(pTDI1->sVersion, pTDI2->sVersion);
			break;
			
		case TDCC_CREATIONDATE:
			nCompare = Compare(pTDI1->dateCreated, pTDI2->dateCreated, FALSE, TDCD_CREATE);
			break;
			
		case TDCC_LASTMOD:
			nCompare = Compare(pTDI1->dateLastMod, pTDI2->dateLastMod, TRUE, TDCD_LASTMOD);
			break;
			
		case TDCC_DONEDATE:
			{
				COleDateTime date1(pTDI1->dateDone);
				COleDateTime date2(pTDI2->dateDone);
				
				// sort tasks 'good as done' between done and not-done
				if (bDone1 && !CDateHelper::IsDateSet(date1))
					date1 = 0.1;

				if (bDone2 && !CDateHelper::IsDateSet(date2))
					date2 = 0.1;
				
				nCompare = Compare(date1, date2, bIncDoneTime, TDCD_DONE);
			}
			break;
			
		case TDCC_DUEDATE:
			{
				COleDateTime date1, date2;

				if (bDone1 && !bHideDone)
					date1 = pTDI1->dateDue;
				else
					date1 = CalcTaskDueDate(pTDI1, pTDS1);
				
				if (bDone2 && !bHideDone)
					date2 = pTDI2->dateDue;
				else
					date2 = CalcTaskDueDate(pTDI2, pTDS2);
								
				nCompare = Compare(date1, date2, bIncDueTime, TDCD_DUE);
			}
			break;
			
		case TDCC_REMAINING:
			{
				COleDateTime date1 = CalcTaskDueDate(pTDI1, pTDS1);
				COleDateTime date2 = CalcTaskDueDate(pTDI2, pTDS2);
								
				if (!CDateHelper::IsDateSet(date1) || 
					!CDateHelper::IsDateSet(date2))
				{
					return nCompare = Compare(date1, date2, TRUE, TDCD_DUE);
				}

				// Both dates set => calc remaining time
				COleDateTime dtCur = COleDateTime::GetCurrentTime();
					
				double dRemain1 = date1 - dtCur;
				double dRemain2 = date2 - dtCur;
					
				nCompare = Compare(dRemain1, dRemain2);
			}
			break;

		case TDCC_STARTDATE:
			{
				COleDateTime date1, date2; 
								
				if (bDone1 && !bHideDone)
					date1 = pTDI1->dateStart;
				else
					date1 = CalcTaskStartDate(pTDI1, pTDS1);
				
				if (bDone2 && !bHideDone)
					date2 = pTDI2->dateStart;
				else
					date2 = CalcTaskStartDate(pTDI2, pTDS2);
								
				nCompare = Compare(date1, date2, bIncStartTime, TDCD_START);
			}
			break;
			
		case TDCC_PRIORITY:
			{
				// done items have even less than zero priority!
				// and due items have greater than the highest priority
				int nPriority1 = pTDI1->nPriority; // default
				int nPriority2 = pTDI2->nPriority; // default
				
				BOOL bUseHighestPriority = HasStyle(TDCS_USEHIGHESTPRIORITY);
				
				// item1
				if (bDone1)
				{
					nPriority1 = -1;
				}
				else if (HasStyle(TDCS_DUEHAVEHIGHESTPRIORITY) &&
						IsTaskDue(pTDI1, pTDS1) && 
						(bSortDueTodayHigh || !IsTaskDue(pTDI1, pTDS1, TRUE)))
				{
					nPriority1 = pTDI1->nPriority + 11;
				}
				else if (bUseHighestPriority)
				{
					nPriority1 = GetTaskHighestPriority(pTDI1, pTDS1);
				}
				
				// item2
				if (bDone2)
				{
					nPriority2 = -1;
				}
				else if (HasStyle(TDCS_DUEHAVEHIGHESTPRIORITY) && 
						IsTaskDue(pTDI2, pTDS2) && 
						(bSortDueTodayHigh || !IsTaskDue(pTDI2, pTDS2, TRUE)))
				{
					nPriority2 = pTDI2->nPriority + 11;
				}
				else if (bUseHighestPriority)
				{
					nPriority2 = GetTaskHighestPriority(pTDI2, pTDS2);
				}
				
				nCompare = Compare(nPriority1, nPriority2);
			}
			break;
			
		case TDCC_RISK:
			{
				// done items have even less than zero priority!
				// and due items have greater than the highest priority
				int nRisk1 = pTDI1->nRisk; // default
				int nRisk2 = pTDI2->nRisk; // default
				
				BOOL bUseHighestRisk = HasStyle(TDCS_USEHIGHESTRISK);
				
				// item1
				if (bDone1)
				{
					nRisk1 = -1;
				}
				else if (bUseHighestRisk)
				{
					nRisk1 = GetTaskHighestRisk(pTDI1, pTDS1);
				}
				
				// item2
				if (bDone2)
				{
					nRisk2 = -1;
				}
				else if (bUseHighestRisk)
				{
					nRisk2 = GetTaskHighestRisk(pTDI2, pTDS2);
				}
				
				nCompare = Compare(nRisk1, nRisk2);
			}
			break;
			
		case TDCC_COLOR:
			nCompare = Compare((int)pTDI1->color, (int)pTDI2->color);
			break;
			
		case TDCC_ALLOCTO:
			nCompare = Compare(Misc::FormatArray(pTDI1->aAllocTo), 
								Misc::FormatArray(pTDI2->aAllocTo), TRUE);
			break;
			
		case TDCC_ALLOCBY:
			nCompare = Compare(pTDI1->sAllocBy, pTDI2->sAllocBy, TRUE);
			break;
			
		case TDCC_CREATEDBY:
			nCompare = Compare(pTDI1->sCreatedBy, pTDI2->sCreatedBy, TRUE);
			break;
			
		case TDCC_STATUS:
			nCompare = Compare(pTDI1->sStatus, pTDI2->sStatus, TRUE);
			break;
			
		case TDCC_EXTERNALID:
			nCompare = Compare(pTDI1->sExternalID, pTDI2->sExternalID, TRUE);
			break;
			
		case TDCC_CATEGORY:
			nCompare = Compare(Misc::FormatArray(pTDI1->aCategories), 
								Misc::FormatArray(pTDI2->aCategories), TRUE);
			break;
			
		case TDCC_TAGS:
			nCompare = Compare(Misc::FormatArray(pTDI1->aTags), 
								Misc::FormatArray(pTDI2->aTags), TRUE);
			break;
			
		case TDCC_FILEREF:
			nCompare = Compare(pTDI1->aFileLinks.GetSize(), pTDI2->aFileLinks.GetSize());
			break;
			
		case TDCC_PERCENT:
			{
				int nPercent1 = CalcTaskPercentDone(pTDI1, pTDS1);
				int nPercent2 = CalcTaskPercentDone(pTDI2, pTDS2);
				
				nCompare = Compare(nPercent1, nPercent2);
			}
			break;

		case TDCC_ICON:
			{
				CString sIcon1 = pTDI1->sIcon; 
				CString sIcon2 = pTDI2->sIcon; 

				nCompare = Compare(sIcon1, sIcon2);
			}
			break;

		case TDCC_PARENTID:
			{
				DWORD dwPID1 = (pTDS1 ? pTDS1->GetParentTaskID() : 0);
				DWORD dwPID2 = (pTDS2 ? pTDS2->GetParentTaskID() : 0);

				nCompare = (dwPID1 - dwPID2);
			}
			break;
			
		case TDCC_COST:
			{
				double dCost1 = CalcTaskCost(pTDI1, pTDS1);
				double dCost2 = CalcTaskCost(pTDI2, pTDS2);
				
				nCompare = Compare(dCost1, dCost2);
			}
			break;
			
		case TDCC_TIMEEST:
			{
				double dTime1 = CalcTaskTimeEstimate(pTDI1, pTDS1, TDCU_HOURS);
				double dTime2 = CalcTaskTimeEstimate(pTDI2, pTDS2, TDCU_HOURS);
				
				nCompare = Compare(dTime1, dTime2);
			}
			break;
			
		case TDCC_TIMESPENT:
			{
				double dTime1 = CalcTaskTimeSpent(pTDI1, pTDS1, TDCU_HOURS);
				double dTime2 = CalcTaskTimeSpent(pTDI2, pTDS2, TDCU_HOURS);
				
				nCompare = Compare(dTime1, dTime2);
			}
			break;
			
		case TDCC_RECENTEDIT:
			{
				BOOL bRecent1 = pTDI1->IsRecentlyEdited();
				BOOL bRecent2 = pTDI2->IsRecentlyEdited();
				
				nCompare = Compare(bRecent1, bRecent2);
			}
			break;

		case TDCC_DEPENDENCY:
			{
				// If Task2 is dependent on Task1 then Task1 comes first
				if (IsTaskLocallyDependentOn(dwTask2ID, dwTask1ID, FALSE))
				{
					TRACE(_T("Sort(Task %d depends on Task %d. Task %d sorts higher\n"), dwTask2ID, dwTask1ID, dwTask1ID);
					nCompare = -1;
				}
				// else if Task1 is dependent on Task2 then Task2 comes first
				else if (IsTaskLocallyDependentOn(dwTask1ID, dwTask2ID, FALSE))
				{
					TRACE(_T("Sort(Task %d depends on Task %d. Task %d sorts higher\n"), dwTask1ID, dwTask2ID, dwTask2ID);
					nCompare = 1;
				}
// 				// else if both have dependencies sort by ID
// 				else if (dwDepends1 && dwDepends2)
// 				{
// 					nCompare = (dwDepends1 - dwDepends2);
// 				}
// 				// else the non-zero dependency always sorts high
// 				else
// 				{
// 					nCompare = (dwDepends1 ? -1 : (dwDepends2 ? 1 : 0));
// 				}
			}
			break;

		case TDCC_SUBTASKDONE:
			{
				int nSubtasksCount1 = -1, nSubtasksDone1 = -1;
				int nSubtasksCount2 = -1, nSubtasksDone2 = -1;
				double dPercent1 = -1.0, dPercent2 = -1.0;
				
				// compare first by % completion
				if (GetTaskSubtaskTotals(pTDI1, pTDS1, nSubtasksCount1, nSubtasksDone1))
					dPercent1 = ((double)nSubtasksDone1 / nSubtasksCount1);
				
				if (GetTaskSubtaskTotals(pTDI2, pTDS2, nSubtasksCount2, nSubtasksDone2))
					dPercent2 = ((double)nSubtasksDone2 / nSubtasksCount2);
				
				nCompare = Compare(dPercent1, dPercent2);

				if (nCompare == 0)
				{
					// compare by number of subtasks
					nCompare = -Compare(nSubtasksCount1, nSubtasksCount2);
				}
			}
			break;
			
		default:
			ASSERT(0);
			return 0;
		}
	}
	
	return bAscending ? nCompare : -nCompare;
}

int CToDoCtrlData::Compare(const COleDateTime& date1, const COleDateTime& date2, BOOL bIncTime, TDC_DATE nDate)
{
	// Sort Non-dates below others
	BOOL bHas1 = CDateHelper::IsDateSet(date1);
	BOOL bHas2 = CDateHelper::IsDateSet(date2);

	if (bHas1 != bHas2)
	{
		return (bHas1 ? -1 : 1);
	}
	else if (!bHas1 && !bHas2)
	{
		return 0;
	}

	// Both dates are valid
	COleDateTime dateTime1(date1), dateTime2(date2);
	
	if (bIncTime)
	{
		BOOL bHasTime1 = CDateHelper::DateHasTime(date1);
		BOOL bHasTime2 = CDateHelper::DateHasTime(date2);
		
		if (bHasTime1 != bHasTime2)
		{
			switch (nDate)
			{
			case TDCD_START:
				// Start dates default to the beginning of the day,
				// so nothing to do because it's already zero
				break;
				
			case TDCD_DUE:
			case TDCD_DONE:
				if (!bHasTime1)
					dateTime1 = CDateHelper::GetEndOfDay(dateTime1);
				else
					dateTime2 = CDateHelper::GetEndOfDay(dateTime2);
				break;
				
			case TDCD_CREATE:
			case TDCD_LASTMOD:
				// Should never end up here
			default: 
				// No other values permissible
				ASSERT(0);
				return 0;
			}
		}
	}
	else
	{
		dateTime1 = CDateHelper::GetDateOnly(date1);
		dateTime2 = CDateHelper::GetDateOnly(date2);
	}

	
	return ((dateTime1 < dateTime2) ? -1 : (dateTime1 > dateTime2) ? 1 : 0);
}

int CToDoCtrlData::Compare(const CString& sText1, const CString& sText2, BOOL bCheckEmpty)
{
	if (bCheckEmpty)
	{
		BOOL bEmpty1 = sText1.IsEmpty();
		BOOL bEmpty2 = sText2.IsEmpty();
		
		if (bEmpty1 != bEmpty2)
			return (bEmpty1 ? 1 : -1);
	}
	
	return Misc::NaturalCompare(sText1, sText2);
}

int CToDoCtrlData::Compare(int nNum1, int nNum2)
{
    return (nNum1 - nNum2);
}

int CToDoCtrlData::Compare(double dNum1, double dNum2)
{
    return (dNum1 < dNum2) ? -1 : (dNum1 > dNum2) ? 1 : 0;
}

int CToDoCtrlData::FindTasks(const SEARCHPARAMS& params, CResultArray& aResults) const
{
	// sanity check
	if (!GetTaskCount())
		return 0;
	
	for (int nSubTask = 0; nSubTask < m_struct.GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDS = m_struct.GetSubTask(nSubTask);
		ASSERT(pTDS);

		const TODOITEM* pTDI = GetTask(pTDS);
		ASSERT(pTDI);
		
		FindTasks(pTDI, pTDS, params, aResults);
	}
	
	return aResults.GetSize();
}

int CToDoCtrlData::FindTasks(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const SEARCHPARAMS& params, CResultArray& aResults) const
{
	// sanity check
	ASSERT(pTDI && pTDS);

	if (!pTDI || !pTDS)
		return 0;

	SEARCHRESULT result;
	int nResults = aResults.GetSize();
	
	// if the item is done and we're ignoring completed tasks 
	// (and by corollary their children) then we can stop right-away
	if (params.bIgnoreDone && IsTaskDone(pTDI, pTDS, TDCCHECKALL))
		return 0;
	
	if (TaskMatches(pTDI, pTDS, params, result))
	{
		aResults.Add(result);
	}
	
	// process children
	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nSubTask);
		ASSERT(pTDSChild);

		const TODOITEM* pTDIChild = GetTask(pTDSChild);
		ASSERT(pTDIChild);
		
		FindTasks(pTDIChild, pTDSChild, params, aResults);
	}
	
	return (aResults.GetSize() - nResults);
}

BOOL CToDoCtrlData::TaskMatches(DWORD dwTaskID, const SEARCHPARAMS& params, SEARCHRESULT& result) const
{
	// sanity check
	if (!dwTaskID)
		return FALSE;

	// snapshot task ID so we can test for 'reference'
	DWORD dwOrgTaskID(dwTaskID);
	
	const TODOITEM* pTDI = NULL; 
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDI_TDS(dwTaskID, pTDI, pTDS, FALSE);

	if (TaskMatches(pTDI, pTDS, params, result))
	{
		// test for 'reference'
		if (dwOrgTaskID != dwTaskID)
			result.dwFlags |= RF_REFERENCE;

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CToDoCtrlData::TaskMatches(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, 
								const SEARCHPARAMS& params, SEARCHRESULT& result) const
{
	// sanity check
	ASSERT(pTDI && pTDS);
	
	if (!pTDI || !pTDS)
		return FALSE;
	
	BOOL bIsDone = IsTaskDone(pTDI, pTDS, TDCCHECKALL);
	
	// if the item is done and we're ignoring completed tasks 
	// (and by corollary their children) then we can stop right-away
	if (bIsDone && params.bIgnoreDone)
		return FALSE;
	
	BOOL bMatches = TRUE;
	
	int nNumRules = params.aRules.GetSize();
	
	for (int nRule = 0; nRule < nNumRules && bMatches; nRule++)
	{
		SEARCHRESULT resTask;
		const SEARCHPARAM& sp = params.aRules[nRule];
		BOOL bMatch = TRUE, bLastRule = (nRule == nNumRules - 1);
		
		switch (sp.GetAttribute())
		{
		case TDCA_TASKNAME:
			bMatch = TaskMatches(pTDI->sTitle, sp, resTask, FALSE);
			break;
			
		case TDCA_TASKNAMEORCOMMENTS:
			bMatch = TaskMatches(pTDI->sTitle, sp, resTask, FALSE) ||
					TaskCommentsMatch(pTDI, sp, resTask);
			break;
			
		case TDCA_COMMENTS:
			bMatch = TaskCommentsMatch(pTDI, sp, resTask);
			break;
			
		case TDCA_ALLOCTO:
			bMatch = TaskMatches(pTDI->aAllocTo, sp, resTask);
			break;
			
		case TDCA_ALLOCBY:
			bMatch = TaskMatches(pTDI->sAllocBy, sp, resTask, TRUE);
			break;
			
		case TDCA_PATH:
			{
				CString sPath = GetTaskPath(pTDI, pTDS);

				// needs care in the handling of trailing back-slashes 
				// when testing for equality
				if ((sp.GetOperator() == FOP_EQUALS) || (sp.GetOperator() == FOP_NOT_EQUALS))
				{
					FileMisc::TerminatePath(sPath, FileMisc::IsPathTerminated(sp.ValueAsString()));
				}
				
				bMatch = TaskMatches(sPath, sp, resTask, FALSE);
			}
			break;
			
		case TDCA_CREATEDBY:
			bMatch = TaskMatches(pTDI->sCreatedBy, sp, resTask, TRUE);
			break;
			
		case TDCA_STATUS:
			bMatch = TaskMatches(pTDI->sStatus, sp, resTask, TRUE);
			break;
			
		case TDCA_CATEGORY:
			bMatch = TaskMatches(pTDI->aCategories, sp, resTask);
			break;
			
		case TDCA_TAGS:
			bMatch = TaskMatches(pTDI->aTags, sp, resTask);
			break;
			
		case TDCA_EXTERNALID:
			bMatch = TaskMatches(pTDI->sExternalID, sp, resTask, TRUE);
			break;

		case TDCA_RECENTMODIFIED:
			bMatch = pTDI->IsRecentlyEdited();

			if (bMatch)
				resTask.aMatched.Add(FormatResultDate(pTDI->dateLastMod));
			break;
			
		case TDCA_CREATIONDATE:
			bMatch = TaskMatches(pTDI->dateCreated, sp, resTask, FALSE, TDCD_CREATE);
			break;
			
		case TDCA_STARTDATE:
		case TDCA_STARTTIME:
			{
				BOOL bIncTime = (sp.GetAttribute() == TDCA_STARTTIME);

				// CalcStartDate will ignore completed tasks so we have
				// to handle that specific situation
				if (bIsDone && !params.bIgnoreDone)
				{
					bMatch = TaskMatches(pTDI->dateStart, sp, resTask, bIncTime, TDCD_START);
				}
				else
				{
					COleDateTime dtStart = CalcTaskStartDate(pTDI, pTDS);
					bMatch = TaskMatches(dtStart, sp, resTask, bIncTime, TDCD_START);
				}
			}
			break;
			
		case TDCA_DUEDATE:
		case TDCA_DUETIME:
			{
				BOOL bIncTime = (sp.GetAttribute() == TDCA_DUETIME);

				// CalcDueDate will ignore completed tasks so we have
				// to handle that specific situation
				if (bIsDone && !params.bIgnoreDone)
				{
					bMatch = TaskMatches(pTDI->dateDue, sp, resTask, bIncTime, TDCD_DUE);
				}
				else
				{
					COleDateTime dtDue = CalcTaskDueDate(pTDI, pTDS);
					bMatch = TaskMatches(dtDue, sp, resTask, bIncTime, TDCD_DUE);
					
					// handle overdue tasks
					if (bMatch && params.bIgnoreOverDue && IsTaskOverDue(pTDI, pTDS))
					{
						bMatch = FALSE;
					}
				}
			}
			break;
			
		case TDCA_DONEDATE:
			// there's a special case here where if the parent
			// is completed then the task is also treated as completed
			if (sp.OperatorIs(FOP_SET))
			{
				bMatch = bIsDone;
			}
			else if (sp.OperatorIs(FOP_NOT_SET))
			{
				bMatch = !bIsDone;
			}
			else
			{
				bMatch = TaskMatches(pTDI->dateDone, sp, resTask, TRUE, TDCD_DONE);
			}
			break;
			
		case TDCA_LASTMOD:
			bMatch = TaskMatches(pTDI->dateLastMod, sp, resTask, TRUE, TDCD_LASTMOD);
			break;
			
		case TDCA_PRIORITY:
			{
				int nPriority = GetTaskHighestPriority(pTDI, pTDS);
				bMatch = TaskMatches(nPriority, sp, resTask);

				// Replace '-2' with 'not set'
				if (bMatch && (nPriority == FM_NOPRIORITY))
					Misc::ReplaceLastT(resTask.aMatched, CEnString(IDS_TDC_NONE));
			}
			break;
			
		case TDCA_RISK:
			{
				int nRisk = GetTaskHighestRisk(pTDI, pTDS);
				bMatch = TaskMatches(nRisk, sp, resTask);

				// Replace '-2' with 'not set'
				if (bMatch && (nRisk == FM_NORISK))
					Misc::ReplaceLastT(resTask.aMatched, CEnString(IDS_TDC_NONE));
			}
			break;
			
		case TDCA_ID:
			bMatch = TaskMatches((int)pTDS->GetTaskID(), sp, resTask);
			break;
			
		case TDCA_PARENTID:
			bMatch = TaskMatches((int)pTDS->GetParentTaskID(), sp, resTask);
			break;
			
		case TDCA_PERCENT:
			{
				int nPercent = CalcTaskPercentDone(pTDI, pTDS);
				bMatch = TaskMatches(nPercent, sp, resTask);
			}
			break;
			
		case TDCA_TIMEEST:
			{
				double dTime = CalcTaskTimeEstimate(pTDI, pTDS, TDCU_HOURS);
				bMatch = TaskMatches(dTime, sp, resTask);
			}
			break;
			
		case TDCA_TIMESPENT:
			{
				double dTime = CalcTaskTimeSpent(pTDI, pTDS, TDCU_HOURS);
				bMatch = TaskMatches(dTime, sp, resTask);
			}
			break;
			
		case TDCA_COST:
			{
				double dCost = CalcTaskCost(pTDI, pTDS);
				bMatch = TaskMatches(dCost, sp, resTask);
			}
			break;
			
		case TDCA_FLAG:
			bMatch = (sp.OperatorIs(FOP_SET) ? pTDI->bFlagged : !pTDI->bFlagged);
				
			if (bMatch)
			{
				resTask.aMatched.Add(CEnString(sp.OperatorIs(FOP_SET) ? IDS_FLAGGED : IDS_UNFLAGGED));
			}
			break;
			
		case TDCA_VERSION:
			bMatch = TaskMatches(pTDI->sVersion, sp, resTask, TRUE);
			break;
			
		case TDCA_ICON:
			bMatch = TaskMatches(pTDI->sIcon, sp, resTask, FALSE);
			break;

		case TDCA_FILEREF:
			bMatch = TaskMatches(pTDI->aFileLinks, sp, resTask);
			break;

		case TDCA_DEPENDENCY:
			bMatch = TaskMatches(pTDI->aDependencies, sp, resTask);
			break;

		case TDCA_POSITION:
			// Position is 1-based in the UI, but 0-based 'here' 
			bMatch = TaskMatches((pTDS->GetPosition() + 1), sp, resTask);

			if (bMatch)
			{
				// replace the default 'int' with full position path
				Misc::ReplaceLastT(resTask.aMatched, GetTaskPositionString(pTDI, pTDS));
			}
			break;
			
		case TDCA_ANYTEXTATTRIBUTE:
			bMatch = (TaskMatches(pTDI->sTitle, sp, resTask, FALSE) ||
						TaskMatches(pTDI->sComments, sp, resTask, FALSE) ||
						TaskMatches(pTDI->aAllocTo, sp, resTask) ||
						TaskMatches(pTDI->sAllocBy, sp, resTask, TRUE) ||
						TaskMatches(pTDI->aCategories, sp, resTask) ||
						TaskMatches(pTDI->sStatus, sp, resTask, TRUE) ||
						TaskMatches(pTDI->sVersion, sp, resTask, TRUE) ||
						TaskMatches(pTDI->sExternalID, sp, resTask, TRUE) ||
						TaskMatches(pTDI->aFileLinks, sp, resTask) ||
						TaskMatches(pTDI->aTags, sp, resTask) ||
						TaskMatches(pTDI->sCreatedBy, sp, resTask, TRUE));
			break;

		case TDCA_SELECTION:
			break; // ignore

		default:
			// test for custom attributes
			if (sp.IsCustomAttribute())
			{
				CString sUniqueID = sp.GetCustomAttributeID();
				ASSERT (!sUniqueID.IsEmpty());

				TDCCUSTOMATTRIBUTEDEFINITION attribDef;

				if (CTDCCustomAttributeHelper::GetAttributeDef(sUniqueID, params.aAttribDefs, attribDef))
				{
					TDCCADATA data(pTDI->GetCustomAttribValue(attribDef.sUniqueID));
					bMatch = TaskMatches(data, attribDef.GetAttributeType(), sp, resTask);
				}
				else
				{
					ASSERT (0);
				}
			}
			else
			{
				ASSERT (0);
			}
			break;
		}
		
		// save off result
		if (bMatch)
			result.aMatched.Append(resTask.aMatched);
		
		// handle this result
		bMatches &= bMatch;
		
		// are we at the end of this group?
		if ((sp.GetAnd() == FALSE) || bLastRule) // == 'OR' or end of aRules
		{
			// if the group result is a match then we're done because
			// whatever may come after this is 'ORed' and so cannot change 
			// the result
			if (bMatches || bLastRule)
			{
				break;
			}
			else // we're not at the end so we reset bMatches and keep going
			{
				bMatches = TRUE;
			}
		}
		// or is there another group ahead of us ?
		else if (!bMatches) 
		{
			int nNext = nRule + 1;
			
			while (nNext < nNumRules)
			{
				const SEARCHPARAM& spNext = params.aRules[nNext];
				bLastRule = (nNext == nNumRules - 1);
				
				if ((spNext.GetAnd() == FALSE) && !bLastRule)
				{
					nRule = nNext; // start of next group
					bMatches = TRUE;
					break;
				}
				
				// keep looking
				nNext++;
			}
		}	
	}

	// check for parent match if user wants all subtasks
	if (!bMatches && params.bWantAllSubtasks)
	{
		const TODOSTRUCTURE* pTDSParent = pTDS->GetParentTask();

		while (pTDSParent)
		{
			const TODOITEM* pTDIParent = GetTask(pTDSParent);

			if (pTDIParent)
			{
				if (TaskMatches(pTDIParent, pTDSParent, params, result))
				{
					bMatches = TRUE;
					break;
				}
				
				// next parent
				pTDSParent = pTDSParent->GetParentTask();
			}
			else
				break;
		}
	}
	
	if (bMatches)
	{
		if (bIsDone)
		{
			if (pTDI->IsDone())
				result.dwFlags |= RF_DONE;
			else
				result.dwFlags |= RF_GOODASDONE;
		}
		
		if (IsTaskReference(pTDS->GetTaskID()))
			result.dwFlags |= RF_REFERENCE;

		if (pTDS->HasSubTasks())
			result.dwFlags |= RF_PARENT;

		if (pTDS->GetParentTaskID() == 0)
			result.dwFlags |= RF_TOPMOST;
		
		result.dwTaskID = pTDS->GetTaskID();
	}
	
	return bMatches;
}

BOOL CToDoCtrlData::TaskCommentsMatch(const TODOITEM* pTDI, const SEARCHPARAM& sp, SEARCHRESULT& result) const
{
	BOOL bMatch = TaskMatches(pTDI->sComments, sp, result, FALSE);
				
	// handle custom comments for 'SET' and 'NOT SET'
	if (!bMatch)
	{
		if (sp.OperatorIs(FOP_SET))
		{
			bMatch = !pTDI->customComments.IsEmpty();
		}
		else if (sp.OperatorIs(FOP_NOT_SET))
		{
			bMatch = pTDI->customComments.IsEmpty();
		}
	}

	return bMatch;
}

BOOL CToDoCtrlData::TaskMatches(const COleDateTime& dtTask, const SEARCHPARAM& sp, 
								SEARCHRESULT& result, BOOL bIncludeTime, TDC_DATE nDate) const
{
	double dTaskDate = dtTask.m_dt, dSearch = sp.ValueAsDate().m_dt;
	BOOL bMatch = FALSE;

	if (!bIncludeTime)
	{
		// Truncate to start of day
		dTaskDate = CDateHelper::GetDateOnly(dTaskDate);
		dSearch = CDateHelper::GetDateOnly(dSearch);
	}
	else
	{
		// Handle those tasks having no (ie. Default) times
		if (CDateHelper::IsDateSet(dtTask) && IsEndOfDay(dtTask))
		{
			switch (nDate)
			{
			case TDCD_START:
				// Start dates default to the beginning of the day,
				// so nothing to do because it's already zero
				break;

			case TDCD_DUE:
			case TDCD_DONE:
			case TDCD_CUSTOM:
				// Due/Done dates default to the end of the day
				dTaskDate = CDateHelper::GetEndOfDay(dtTask);
				break;

			case TDCD_CREATE:
			case TDCD_LASTMOD:
				// Should never end up here
			default:
				// No other values permissible
				ASSERT(0);
				return FALSE;
			}
		}
	}
	
	switch (sp.GetOperator())
	{
	case FOP_EQUALS:
		bMatch = (dTaskDate != 0.0) && (dTaskDate == dSearch);
		break;
		
	case FOP_NOT_EQUALS:
		bMatch = (dTaskDate != 0.0) && (dTaskDate != dSearch);
		break;
		
	case FOP_ON_OR_AFTER:
		bMatch = (dTaskDate != 0.0) && (dTaskDate >= dSearch);
		break;
		
	case FOP_AFTER:
		bMatch = (dTaskDate != 0.0) && (dTaskDate > dSearch);
		break;
		
	case FOP_ON_OR_BEFORE:
		bMatch = (dTaskDate != 0.0) && (dTaskDate <= dSearch);
		break;
		
	case FOP_BEFORE:
		bMatch = (dTaskDate != 0.0) && (dTaskDate < dSearch);
		break;
		
	case FOP_SET:
		bMatch = (dTaskDate != 0.0);
		break;
		
	case FOP_NOT_SET:
		bMatch = (dTaskDate == 0.0);
		break;
	}
	
	if (bMatch)
		result.aMatched.Add(FormatResultDate(dtTask));
	
	return bMatch;
}

CString CToDoCtrlData::FormatResultDate(const COleDateTime& date) const
{
	CString sDate;

	if (CDateHelper::IsDateSet(date))
	{
		DWORD dwFmt = 0;
		
		if (HasStyle(TDCS_SHOWWEEKDAYINDATES))
			dwFmt |= DHFD_DOW;
		
		if (HasStyle(TDCS_SHOWDATESINISO))
			dwFmt |= DHFD_ISO;
		
		sDate = CDateHelper::FormatDate(date, dwFmt);
	}

	return sDate;
}

BOOL CToDoCtrlData::TaskMatches(const CString& sText, const SEARCHPARAM& sp, SEARCHRESULT& result, BOOL bMatchAsArray) const
{
	// special case: search param may hold multiple delimited items
	if (bMatchAsArray && (!sText.IsEmpty() || sp.HasString()))
	{
		CStringArray aText;
		Misc::Split(sText, aText);

		return TaskMatches(aText, sp, result);
	}

	// all else normal text search
	BOOL bMatch = FALSE;
	
	switch (sp.GetOperator())
	{
	case FOP_EQUALS:
		bMatch = (Misc::NaturalCompare(sText, sp.ValueAsString()) == 0);
		break;
		
	case FOP_NOT_EQUALS:
		bMatch = (Misc::NaturalCompare(sText, sp.ValueAsString()) != 0);
		break;
		
	case FOP_INCLUDES:
	case FOP_NOT_INCLUDES:
		{
			CStringArray aWords;
			
			if (!Misc::ParseSearchString(sp.ValueAsString(), aWords))
				return FALSE;
			
			// cycle all the words
			for (int nWord = 0; nWord < aWords.GetSize() && !bMatch; nWord++)
			{
				CString sWord = aWords.GetAt(nWord);
				bMatch = Misc::FindWord(sWord, sText, FALSE, FALSE);
			}
			
			// handle !=
			if (sp.OperatorIs(FOP_NOT_INCLUDES))
				bMatch = !bMatch;
		}
		break;
		
	case FOP_SET:
		bMatch = !sText.IsEmpty();
		break;
		
	case FOP_NOT_SET:
		bMatch = sText.IsEmpty();
		break;
	}
	
	if (bMatch)
		result.aMatched.Add(sText);
	
	return bMatch;
}

BOOL CToDoCtrlData::TaskMatches(const CStringArray& aItems, const SEARCHPARAM& sp, SEARCHRESULT& result) const
{
	// special cases
	if (sp.OperatorIs(FOP_SET) && aItems.GetSize())
	{
		int nItem = aItems.GetSize();
		
		while (nItem--)
			result.aMatched.Add(aItems[nItem]);
		
		return TRUE;
	}
	else if (sp.OperatorIs(FOP_NOT_SET) && !aItems.GetSize())
	{
		return TRUE;
	}
	
	// general case
	BOOL bMatch = FALSE;
	BOOL bMatchAll = (sp.OperatorIs(FOP_EQUALS) || sp.OperatorIs(FOP_NOT_EQUALS));
	
	CStringArray aSearchItems;
	Misc::Split(sp.ValueAsString(), aSearchItems, EMPTY_STR, TRUE);
	
	if (bMatchAll)
	{
		bMatch = Misc::MatchAll(aItems, aSearchItems);
	}
	else // only one match is required
	{
		BOOL bPartialOK = (sp.OperatorIs(FOP_INCLUDES) || sp.OperatorIs(FOP_NOT_INCLUDES));

		if (aItems.GetSize())
		{
			bMatch = Misc::MatchAny(aSearchItems, aItems, FALSE, bPartialOK);
		}
		else
		{
			// special case: task has no item and param.aItems contains an empty item
			bMatch = (Misc::Find(aSearchItems, EMPTY_STR) != -1);
		}
	}
	
	// handle !=
	if (sp.OperatorIs(FOP_NOT_EQUALS) || sp.OperatorIs(FOP_NOT_INCLUDES))
		bMatch = !bMatch;
	
	if (bMatch)
		result.aMatched.Add(Misc::FormatArray(aItems));
	
	return bMatch;
}

BOOL CToDoCtrlData::TaskMatches(const TDCCADATA& data, DWORD dwAttribType, const SEARCHPARAM& sp, SEARCHRESULT& result) const
{
	DWORD dwdataType = (dwAttribType & TDCCA_DATAMASK);
	BOOL bIsList = (dwAttribType & TDCCA_LISTMASK);
	BOOL bMatch = FALSE;

	if (bIsList)
	{
		CStringArray aData;
		data.AsArray(aData);
		
		bMatch = TaskMatches(aData, sp, result);
	}
	else
	{
		// Optimisation: Handle not-set by checking if string is empty
		if (sp.OperatorIs(FOP_NOT_SET) && data.IsEmpty())
			dwdataType = TDCCA_STRING;
        
		switch (dwdataType)
		{
		case TDCCA_STRING:	
			bMatch = TaskMatches(data.AsString(), sp, result, FALSE);
			break;
			
		case TDCCA_INTEGER:	
			bMatch = TaskMatches(data.AsInteger(), sp, result);
			break;
			
		case TDCCA_DOUBLE:	
			bMatch = TaskMatches(data.AsDouble(), sp, result);
			break;
			
		case TDCCA_DATE:	
			bMatch = TaskMatches(data.AsDate(), sp, result, FALSE, TDCD_CUSTOM);
			break;
			
		case TDCCA_BOOL:	
			bMatch = TaskMatches(data.AsBool(), sp, result);
			break;
			
		default:
			ASSERT(0);
			break;
		}
	}

	return bMatch;
}


BOOL CToDoCtrlData::TaskMatches(double dValue, const SEARCHPARAM& sp, SEARCHRESULT& result) const
{
	BOOL bMatch = FALSE;
	BOOL bTime = (sp.AttributeIs(TDCA_TIMEEST) || sp.AttributeIs(TDCA_TIMESPENT));
	double dSearchVal = sp.ValueAsDouble();
	
	if (bTime)
	{
		TH_UNITS nTHUints = TDC::MapUnitsToTHUnits((TDC_UNITS)sp.GetFlags());
		dSearchVal = CTimeHelper().GetTime(dSearchVal, nTHUints, THU_HOURS);
	}
	
	switch (sp.GetOperator())
	{
	case FOP_EQUALS:
		bMatch = (dValue == dSearchVal);
		break;
		
	case FOP_NOT_EQUALS:
		bMatch = (dValue != dSearchVal);
		break;
		
	case FOP_GREATER_OR_EQUAL:
		bMatch = (dValue >= dSearchVal);
		break;
		
	case FOP_GREATER:
		bMatch = (dValue > dSearchVal);
		break;
		
	case FOP_LESS_OR_EQUAL:
		bMatch = (dValue <= dSearchVal);
		break;
		
	case FOP_LESS:
		bMatch = (dValue < dSearchVal);
		break;
		
	case FOP_SET:
		bMatch = (dValue > 0.0);
		break;
		
	case FOP_NOT_SET:
		bMatch = (dValue == 0.0);
		break;
	}
	
	if (bMatch)
	{
		CString sMatch;
		
		if (bTime)
			sMatch = Misc::Format(dValue, 3, _T(" H"));
		else
			sMatch = Misc::Format(dValue, 3);
		
		result.aMatched.Add(sMatch);
	}
	
	return bMatch;
}

BOOL CToDoCtrlData::TaskMatches(int nValue, const SEARCHPARAM& sp, SEARCHRESULT& result) const
{
	BOOL bMatch = FALSE;
	BOOL bPriorityRisk = (sp.AttributeIs(TDCA_PRIORITY) || sp.AttributeIs(TDCA_RISK));
	int nSearchVal = sp.ValueAsInteger();
	
	switch (sp.GetOperator())
	{
	case FOP_EQUALS:
		bMatch = (nValue == nSearchVal);
		break;
		
	case FOP_NOT_EQUALS:
		bMatch = (nValue != nSearchVal);
		break;
		
	case FOP_GREATER_OR_EQUAL:
		bMatch = (nValue >= nSearchVal);
		break;
		
	case FOP_GREATER:
		bMatch = (nValue > nSearchVal);
		break;
		
	case FOP_LESS_OR_EQUAL:
		bMatch = (nValue <= nSearchVal);
		break;
		
	case FOP_LESS:
		bMatch = (nValue < nSearchVal);
		break;
		
	case FOP_SET:
		if (sp.AttributeIs(TDCA_PRIORITY))
		{
			bMatch = (nValue != FM_NOPRIORITY);
		}
		else if (sp.AttributeIs(TDCA_RISK))
		{
			bMatch = (nValue != FM_NORISK);
		}
		else
		{
			bMatch = (nValue > 0);
		}
		break;
		
	case FOP_NOT_SET:
		if (sp.AttributeIs(TDCA_PRIORITY))
		{
			bMatch = (nValue == FM_NOPRIORITY);
		}
		else if (sp.AttributeIs(TDCA_RISK))
		{
			bMatch = (nValue == FM_NORISK);
		}
		else
		{
			bMatch = (nValue == 0);
		}
		break;
	}
	
	if (bMatch)
		result.aMatched.Add(Misc::Format(nValue));
	
	return bMatch;
}

BOOL CToDoCtrlData::CalcNewTaskDependencyStartDate(DWORD dwTaskID, DWORD dwDependencyID, 
												   TDC_DATE nDate, COleDateTime& dtNewStart) const
{
	CDateHelper::ClearDate(dtNewStart);

	// if we're already completed then no adjustment is necessary
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);

	if (pTDI->IsDone())
		return FALSE;

	const TODOITEM* pTDIDepends = NULL;
	GET_TDI(dwDependencyID, pTDIDepends, FALSE);

	switch (nDate)
	{
	case TDCD_DONE:
		if (pTDIDepends->IsDone())
		{
			// start dates match the preceding task's end date 
			dtNewStart = pTDIDepends->dateDone;

			// if we're on a day/night boundary move to next day
			if (IsEndOfDay(dtNewStart))
				dtNewStart += 1.0;
		}
		break;

	case TDCD_DUE:
	case TDCD_DUEDATE:
	case TDCD_DUETIME:
		// or its due date and it's not yet finished
		if (!pTDIDepends->IsDone() && pTDIDepends->HasDue())
		{	
			// start dates match the preceding task's due date 
			dtNewStart = pTDIDepends->dateDue;

			// if we're on a day/night boundary move to next day
			if (IsEndOfDay(dtNewStart))
				dtNewStart += 1.0;
		}
		break;

	case TDCD_START:
	case TDCD_STARTDATE:
	case TDCD_STARTTIME:
		// start is not affected by changes to dependency start dates
		dtNewStart = pTDI->dateStart;
		break;

	default:
		ASSERT(0);
		return FALSE;
	}
	
	if (CDateHelper::IsDateSet(dtNewStart))
	{
		if (pTDI->nTimeEstUnits == TDCU_WEEKDAYS)
			CDateHelper::MakeWeekday(dtNewStart);

		return TRUE;
	}

	return FALSE;
}

UINT CToDoCtrlData::SetNewTaskDependencyStartDate(DWORD dwTaskID, const COleDateTime& dtNewStart)
{
	TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, ADJUSTED_NONE);

	// check we have something to do
	if (dtNewStart == pTDI->dateStart)
		return ADJUSTED_NONE;
	
	// bump the due date too if present but before
	// we set the start date
	COleDateTime dtStart(dtNewStart); // start can change too
	UINT nAdjusted = ADJUSTED_NONE;

	if (pTDI->HasDue() && pTDI->HasStart())
	{
		double dDuration = CalcDuration(pTDI->dateStart, pTDI->dateDue, pTDI->nTimeEstUnits);

		COleDateTime dtNewDue = AddDuration(dtStart, dDuration, pTDI->nTimeEstUnits);

		if (dtNewDue != pTDI->dateDue)
		{
			// save undo info
			SaveEditUndo(dwTaskID, pTDI, TDCA_DUEDATE);

			// make the change
			pTDI->dateDue = dtNewDue;
			nAdjusted |= ADJUSTED_DUE;
		}
	}

	// save undo info
	SaveEditUndo(dwTaskID, pTDI, TDCA_STARTDATE);

	// always set the start date
	pTDI->dateStart = dtStart;
	nAdjusted |= ADJUSTED_START;

	return nAdjusted;
}

BOOL CToDoCtrlData::CalcNewTaskDependencyStartDate(DWORD dwTaskID, TDC_DATE nDate, COleDateTime& dtNewStart) const
{
	CDateHelper::ClearDate(dtNewStart);

	// calculate the latest start date possible for this task's dependencies
	CDWordArray aDepends;

	int nDepend = GetTaskLocalDependencies(dwTaskID, aDepends);

	while (nDepend--)
	{
		COleDateTime dtStart;

		if (CalcNewTaskDependencyStartDate(dwTaskID, aDepends[nDepend], nDate, dtStart))
			VERIFY(CDateHelper::Max(dtNewStart, dtStart));
	}

	return CDateHelper::IsDateSet(dtNewStart);
}

UINT CToDoCtrlData::UpdateTaskLocalDependencyDates(DWORD dwTaskID, TDC_DATE nDate)
{
	// calculate the latest start date possible for this task's dependencies
	COleDateTime dtNewStart;

	if (CalcNewTaskDependencyStartDate(dwTaskID, nDate, dtNewStart))
		return SetNewTaskDependencyStartDate(dwTaskID, dtNewStart);

	return ADJUSTED_NONE;
}

BOOL CToDoCtrlData::IsEndOfDay(const COleDateTime& date)
{
	ASSERT(CDateHelper::IsDateSet(date));
	
	return (!CDateHelper::DateHasTime(date) || (date >= CDateHelper::GetEndOfDay(date)));
}

COleDateTime CToDoCtrlData::AddDuration(COleDateTime& dateStart, double dDuration, TDC_UNITS nUnits)
{
	if (!CDateHelper::IsDateSet(dateStart) || (dDuration == 0.0) || (nUnits == TDCU_NULL))
	{
		ASSERT(0);
		return dateStart;
	}

	COleDateTime dateEnd(dateStart);
	
	switch (nUnits)
	{
	case TDCU_MINS:
	case TDCU_HOURS:
	case TDCU_WEEKS:
	case TDCU_MONTHS:
	case TDCU_YEARS:
		{
			CTimeHelper thAllDay(24.0, 7.0);

			double dDays = thAllDay.GetTime(dDuration, TDC::MapUnitsToTHUnits(nUnits), THU_DAYS);
			dateEnd.m_dt += dDays;
		}
		break;

	case TDCU_DAYS:
		dateEnd.m_dt += dDuration;
		break;

	case TDCU_WEEKDAYS:
		{
			// handle workdays
			if (CDateHelper::HasWeekend())
			{
				// Adjust start date if it falls on a weekend
				BOOL bForward = (dDuration > 0.0);
				CDateHelper::MakeWeekday(dateStart, bForward);

				// Adjust one day at a time
				double dDaysLeft = fabs(dDuration);
				int nDir = (bForward ? 1 : -1);

				dateEnd = dateStart;

				while (dDaysLeft > 0.0)
				{
					dDaysLeft--;
					dateEnd.m_dt += nDir;

					// adjust for partial day overrun
					if (dDaysLeft < 0.0)
						dateEnd.m_dt += (nDir * dDaysLeft);

					// step over weekends
					if ((dDaysLeft > 0.0) || CDateHelper::DateHasTime(dateEnd))
					{
						// FALSE -> Don't truncate time
						CDateHelper::MakeWeekday(dateEnd, bForward, FALSE);
					}
				}
			}
			else
			{
				dateEnd.m_dt += dDuration;
			}
		}
		break;
	}

	// If date falls on the beginning of a day, move to end of previous day
	if (dDuration > 0.0)
	{
		if (!CDateHelper::DateHasTime(dateEnd))
			dateEnd.m_dt--;
	}
	else
	{
		if (!CDateHelper::DateHasTime(dateStart))
			dateEnd.m_dt++;
	}
	
	// sanity check
#ifdef _DEBUG
	if (nUnits != TDCU_WEEKDAYS)
	{
		double dCheck = 0.0;
		
		if (dDuration > 0.0)
			dCheck = CalcDuration(dateStart, dateEnd, nUnits);
		else
			dCheck = -CalcDuration(dateEnd, dateStart, nUnits);

		ASSERT(fabs(dCheck - dDuration) < 1e-3);
	}
#endif

	return dateEnd;
}

double CToDoCtrlData::CalcDuration(const COleDateTime& dateStart, const COleDateTime& dateEnd, TDC_UNITS nUnits)
{
	if (!CDateHelper::IsDateSet(dateStart) || !CDateHelper::IsDateSet(dateEnd) || (dateEnd < dateStart))
	{
		ASSERT(0);
		return 0.0;
	}

	double dDuration = (dateEnd.m_dt - dateStart.m_dt); // in days
	
	switch (nUnits)
	{
	case TDCU_MINS:
	case TDCU_HOURS:
	case TDCU_WEEKS:
	case TDCU_MONTHS:
	case TDCU_YEARS:
		{
			CTimeHelper thAllDay(24.0, 7.0);
			dDuration = thAllDay.GetTime(dDuration, THU_DAYS, TDC::MapUnitsToTHUnits(nUnits));

			// handle 'whole' of due date
			if (IsEndOfDay(dateEnd))
				dDuration += 1.0;

		}
		break;

	case TDCU_DAYS:
		// handle 'whole' of due date
		if (IsEndOfDay(dateEnd))
			dDuration += 1.0;
		break;

	case TDCU_WEEKDAYS:
		{
			if (CDateHelper::HasWeekend())
			{
				// process each whole or part day  
				double dDayStart(dateStart);
				dDuration = 0.0;

				while (dDayStart < dateEnd)
				{
					// determine the end of this day
					double dDayEnd = (CDateHelper::GetDateOnly(dDayStart).m_dt + 1.0);

					if (!CDateHelper::IsWeekend(dDayStart))
					{
						dDuration += (min(dDayEnd, dateEnd) - dDayStart); // in days
					}

					// next day
					dDayStart = dDayEnd;
				}
			}

			// handle 'whole' of due date
			if (IsEndOfDay(dateEnd))
				dDuration += 1.0;
		}
		break;
	}

	return dDuration;
}

void CToDoCtrlData::FixupTaskLocalDependentsDates(DWORD dwTaskID, TDC_DATE nDate)
{
	if (!HasStyle(TDCS_AUTOADJUSTDEPENDENCYDATES))
		return;
	
	// check for circular dependency before continuing
	if (TaskHasLocalCircularDependencies(dwTaskID))
		return;
	
	// who is dependent on us -> GetTaskDependents
	CDWordArray aDependents;
	int nDepend = GetTaskLocalDependents(dwTaskID, aDependents);

	while (nDepend--)
	{
		DWORD dwIDDependent = aDependents[nDepend];
		UINT nAdjusted = UpdateTaskLocalDependencyDates(dwIDDependent, nDate);
		
		// then process this task's dependents
		// if the due date was actually modified
		if (Misc::HasFlag(nAdjusted, ADJUSTED_DUE))
		{
			FixupTaskLocalDependentsDates(dwIDDependent, TDCD_DUE);
		}
	}
}

