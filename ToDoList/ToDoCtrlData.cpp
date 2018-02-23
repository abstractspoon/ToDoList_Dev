// ToDoCtrlData.cpp: implementation of the CToDoCtrlData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToDoCtrlData.h"
#include "ToDoCtrlDataDefines.h"
#include "TDCCustomAttributeHelper.h"
#include "resource.h"

#include "..\shared\timehelper.h"
#include "..\shared\datehelper.h"
#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\enstring.h"
#include "..\shared\autoflag.h"
#include "..\shared\mapex.h"

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
	if (tdi->bLocked)           \
		return SET_FAILED;		\
}					

//////////////////////////////////////////////////////////////////////

#define GET_TDI(id, tdi, ret)	\
	GET_DATA_TDI((*this), id, tdi, ret)

#define GET_TDS(id, tds, ret)	\
	GET_DATA_TDS((*this), id, tds, ret)

#define GET_TDI_TDS(id, tdi, tds, ret)	\
	GET_DATA_TDI_TDS((*this), id, tdi, tds, ret)

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
	m_aStyles(aStyles),
	m_bUndoRedoing(FALSE)
{
}

CToDoCtrlData::~CToDoCtrlData()
{
	DeleteAllTasks();
}

void CToDoCtrlData::SetInheritedParentAttributes(const CTDCAttributeMap& mapAttribs, BOOL bUpdateAttrib)
{
	s_mapParentAttribs.Copy(mapAttribs);
	s_bUpdateInheritAttrib = bUpdateAttrib;
}

BOOL CToDoCtrlData::WantUpdateInheritedAttibute(TDC_ATTRIBUTE nAttrib)
{
	return (s_bUpdateInheritAttrib && s_mapParentAttribs.Has(nAttrib));
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
			
			m_items.AddTask(dwTaskID, pTDI);
			
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

TODOITEM* CToDoCtrlData::NewTask(const CTaskFile& tasks, HTASKITEM hTask, const TODOITEM* pTDIRef) const
{
	ASSERT(hTask);

	if (!hTask)
		return NULL;

	TODOITEM* pTDI = (pTDIRef ? NewTask(*pTDIRef) : NewTask());

	if (!pTDI)
	{
		ASSERT(0);
		return NULL;
	}
	
	// Don't overwrite default attributes except with actual values
	if (pTDIRef)
		tasks.MergeTaskAttributes(hTask, *pTDI);
	else
		tasks.GetTaskAttributes(hTask, *pTDI);
	
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
	return m_items.HasTask(dwTaskID);
}

BOOL CToDoCtrlData::TaskHasSubtask(DWORD dwTaskID, DWORD dwSubtaskID, BOOL bImmediate) const
{
	if (dwTaskID == 0)
		return m_struct.HasSubTask(dwSubtaskID, bImmediate);

	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDS(dwTaskID, pTDS, FALSE);

	return pTDS->HasSubTask(dwSubtaskID, bImmediate);
}

BOOL CToDoCtrlData::TaskHasSibling(DWORD dwTaskID, DWORD dwSiblingID, BOOL bImmediate) const
{
	if (dwTaskID == 0)
	{
		ASSERT(0);
		return FALSE;
	}

	// Check they have the same parent
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDS(dwTaskID, pTDS, FALSE);

	const TODOSTRUCTURE* pTDSParent = pTDS->GetParentTask();
	BOOL bSameParent = (pTDSParent->HasSubTask(dwSiblingID, TRUE));

	if (!bSameParent || bImmediate)
	{
		return bSameParent;
	}

	// Check they have adjacent positions
	int nPos = pTDSParent->GetSubTaskPosition(dwTaskID);
	int nSiblingPos = pTDSParent->GetSubTaskPosition(dwSiblingID);

	return (abs(nPos - nSiblingPos) == 1);
}

POSITION CToDoCtrlData::GetFirstTaskPosition() const
{
	return m_items.GetStartPosition();
}

DWORD CToDoCtrlData::GetNextTask(POSITION& pos, const TODOITEM*& pTDI) const
{
	DWORD dwTaskID = 0;
	TODOITEM* pTemp = NULL;

	m_items.GetNextAssoc(pos, dwTaskID, pTemp);

	pTDI = pTemp;
	return dwTaskID;
}

DWORD CToDoCtrlData::GetNextTaskID(POSITION& pos) const
{
	const TODOITEM* pUnused;
	return GetNextTask(pos, pUnused);
}

// external version returning const
const TODOITEM* CToDoCtrlData::GetTask(DWORD dwTaskID) const
{
	return GetTask(dwTaskID, FALSE);
}

// external version returning const
const TODOITEM* CToDoCtrlData::GetTrueTask(DWORD& dwTaskID) const
{
	return GetTask(dwTaskID, TRUE);
}

// internal version returning non-const
TODOITEM* CToDoCtrlData::GetTask(DWORD& dwTaskID, BOOL bTrue) const
{
	TODOITEM* pTDI = m_items.GetTask(dwTaskID);

	if (pTDI && bTrue && pTDI->dwTaskRefID)
	{
		DWORD dwRefID = pTDI->dwTaskRefID;
		pTDI = m_items.GetTask(dwRefID);
		dwTaskID = dwRefID;
	}
	
	return pTDI;
}

// internal version returning non-const
TODOITEM* CToDoCtrlData::GetTask(const TODOSTRUCTURE* pTDS) const
{
	if (!pTDS || !pTDS->GetTaskID())
	{
		ASSERT(0);
		return NULL;
	}

	// else
	DWORD dwTaskID = pTDS->GetTaskID();
	return GetTask(dwTaskID, FALSE);
}

// internal version returning non-const
TODOITEM* CToDoCtrlData::GetTrueTask(const TODOSTRUCTURE* pTDS) const
{
	if (!pTDS || !pTDS->GetTaskID())
	{
		ASSERT(0);
		return NULL;
	}
	
	// else
	DWORD dwTaskID = pTDS->GetTaskID();
	return GetTask(dwTaskID, TRUE);
}

BOOL CToDoCtrlData::Locate(DWORD dwParentID, DWORD dwPrevSiblingID, TODOSTRUCTURE*& pTDSParent, int& nPos) const
{
	pTDSParent = NULL;
	nPos = -1;
	
	if (dwPrevSiblingID)
	{
		m_struct.FindTask(dwPrevSiblingID, pTDSParent, nPos);
	}
	else if (dwParentID)
	{
		pTDSParent = m_struct.FindTask(dwParentID);
	}
	else
	{
		pTDSParent = const_cast<CToDoCtrlDataStructure*>(&m_struct); // root
	}
	
	ASSERT (pTDSParent);
	return (pTDSParent != NULL);
}

BOOL CToDoCtrlData::LocateTask(DWORD dwTaskID, TODOSTRUCTURE*& pTDSParent, int& nPos) const
{
	return m_struct.FindTask(dwTaskID, pTDSParent, nPos);
}

const TODOSTRUCTURE* CToDoCtrlData::LocateTask(DWORD dwTaskID) const
{
	return m_struct.FindTask(dwTaskID);
}

BOOL CToDoCtrlData::AddTask(DWORD dwTaskID, TODOITEM* pTDI, DWORD dwParentID, DWORD dwPrevSiblingID, BOOL bWantUndo) 
{ 
	if (!dwTaskID || !pTDI)
	{
		ASSERT(0);
		return FALSE;
	}

	// must delete duplicates else we'll get a memory leak
	m_items.DeleteTask(dwTaskID);
	
	// add to structure
	TODOSTRUCTURE* pTDSParent = NULL;
	int nPrevSibling = -1;
	
	if (!Locate(dwParentID, dwPrevSiblingID, pTDSParent, nPrevSibling))
		return FALSE;
	
	if (!m_struct.InsertTask(dwTaskID, pTDSParent, nPrevSibling + 1))
		return FALSE;

	if (!m_items.AddTask(dwTaskID, pTDI))
	{
		// remove from structure
		m_struct.DeleteTask(dwTaskID);
		return FALSE;
	}
	
	VERIFY(!bWantUndo || AddUndoElement(TDCUEO_ADD, dwTaskID, dwParentID, dwPrevSiblingID));

	return TRUE;
}

BOOL CToDoCtrlData::AddTask(DWORD dwTaskID, TODOITEM* pTDI, DWORD dwParentID, DWORD dwPrevSiblingID) 
{ 
	return AddTask(dwTaskID, pTDI, dwParentID, dwPrevSiblingID, TRUE);
}

void CToDoCtrlData::DeleteAllTasks()
{
	if (m_undo.CurrentAction() == TDCUAT_NONE)
	{
		m_items.DeleteAll();
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

BOOL CToDoCtrlData::TaskHasDependencies(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0);
	
	return (pTDI->aDependencies.GetSize() > 0);
}

BOOL CToDoCtrlData::TaskHasDependents(DWORD dwTaskID) const
{
	ASSERT(dwTaskID);

	if (!dwTaskID)
		return FALSE;

	// Search the entire tasklist for tasks having 'dwTaskID'
	// in their list of dependencies
	POSITION pos = m_items.GetStartPosition();
	CString sTaskID = Misc::Format(dwTaskID);
		
	while (pos)
	{
		TODOITEM* pTDI = NULL;
		DWORD dwDependsID;
			
		m_items.GetNextAssoc(pos, dwDependsID, pTDI);
		ASSERT (pTDI);
			
		if (pTDI && (dwDependsID != dwTaskID) && Misc::Contains(pTDI->aDependencies, sTaskID))
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
		POSITION pos = m_items.GetStartPosition();
		
		while (pos)
		{
			TODOITEM* pTDI = NULL;
			DWORD dwDependsID;
			
			m_items.GetNextAssoc(pos, dwDependsID, pTDI);
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

	// A circular dependency is where any one of a task's dependencies
	// ultimately traces a path back to the task

	// we only check dependencies within the same tasklist
	CDWordArray aDependIDs;
	int nDepends = GetTaskLocalDependencies(dwTaskID, aDependIDs);
	
	while (nDepends--)
	{
		// Keep each path separate
		CDWordSet mapVisited;

		if (FindTaskLocalDependency(aDependIDs[nDepends], dwTaskID, mapVisited))
			return TRUE;
	}
	
	// else
	return FALSE;
}

BOOL CToDoCtrlData::FindTaskLocalDependency(DWORD dwTaskID, DWORD dwDependID, CDWordSet& mapVisited) const
{
	// simple checks
	if (!dwTaskID || !HasTask(dwTaskID))
	{
		// no such task == not found
		return FALSE; 
	}
	
	if (dwTaskID == dwDependID)
	{
		// same task == circular
		return TRUE; 
	}
	
	// if we have been here before, we can stop
	if (mapVisited.Has(dwTaskID))
	{
		// Means some part of the dependency path is circular 
		// but not including this task so we just don't continue
		return FALSE;
	}
	
	// else mark this task as having been visited
	mapVisited.Add(dwTaskID);
	
	// and process its 'same file' dependents
	CDWordArray aDependIDs;
	int nDepends = GetTaskLocalDependencies(dwTaskID, aDependIDs);
	
	while (nDepends--)
	{
		// Continue to keep each path separate
		CDWordSet mapNextVisited(mapVisited);

		if (FindTaskLocalDependency(aDependIDs[nDepends], dwDependID, mapNextVisited))
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

CString CToDoCtrlData::GetTaskLastModifiedBy(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, EMPTY_STR);
	
	return pTDI->sLastModifiedBy;
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

BOOL CToDoCtrlData::GetTaskCustomAttributeData(DWORD dwTaskID, const CString& sAttribID, TDCCADATA& data) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);
	
	return pTDI->GetCustomAttributeValue(sAttribID, data);
}

BOOL CToDoCtrlData::IsTaskLocked(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);

	return pTDI->bLocked;
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

BOOL CToDoCtrlData::IsTaskRecurring(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);

	return pTDI->IsRecurring();
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
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDS(dwTaskID, pTDS, 0);

	return pTDS->GetParentTaskID();
}

DWORD CToDoCtrlData::GetTaskReferenceID(DWORD dwTaskID) const
{
	if (dwTaskID)
	{
		// NOTE: don't use GET_TDI macro here because 
		// that'll give us the 'true' task
		const TODOITEM* pTDI = GetTask(dwTaskID, FALSE);
		ASSERT(pTDI);

		return (pTDI ? pTDI->dwTaskRefID : 0);
	}

	// else
	return 0;
}

BOOL CToDoCtrlData::CanMoveTask(DWORD /*dwTaskID*/, DWORD /*dwDestParentID*/) const
{
	return TRUE;
}

BOOL CToDoCtrlData::IsTaskFlagged(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);

	return pTDI->bFlagged;
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
	m_items.DeleteTask(dwTaskID);
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
		const TODOITEM* pTDIChild = GetTask(pTDSChild);
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
	if (!pToTDI || pToTDI->bLocked)
		return SET_FAILED;
	
	const TODOITEM* pFromTDI = NULL;
	GET_TDI(dwFromTaskID, pFromTDI, SET_FAILED);
	
	TDC_SET nRes = SET_NOCHANGE;
	
	// helper macros
#define COPYATTRIB(a) if (pToTDI->a != pFromTDI->a) { pToTDI->a = pFromTDI->a; nRes = SET_CHANGE; }
#define COPYATTRIBARR(a) if (!Misc::MatchAll(pToTDI->a, pFromTDI->a)) { pToTDI->a.Copy(pFromTDI->a); nRes = SET_CHANGE; }
	
	// note: we don't use the public SetTask* methods purely so we can
	// capture all the edits as a single atomic change that can be undone
	for (int nAttrib = TDCA_FIRSTATTRIBUTE; nAttrib < TDCA_ATTRIBUTECOUNT; nAttrib++)
	{
		if (mapAttribs.Has((TDC_ATTRIBUTE)nAttrib))
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
			case TDCA_LOCK:			COPYATTRIB(bLocked); break;
			
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

TDC_SET CToDoCtrlData::SetTaskAttributes(DWORD dwTaskID, const TODOITEM& tdi)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	if (*pTDI != tdi)
	{
		*pTDI = tdi;
		return SET_CHANGE;
	}

	// else
	return SET_NOCHANGE;
}

BOOL CToDoCtrlData::GetTaskAttributes(DWORD dwTaskID, TODOITEM& tdi) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);

	tdi = *pTDI;
	return TRUE;
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
		nRes = SetTaskExternalID(dwTaskID, EMPTY_STR);
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

	case TDCA_LOCK:			
		nRes = SetTaskLock(dwTaskID, FALSE);
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
			TDC_ATTRIBUTE nAttrib = s_mapParentAttribs.GetNext(pos);
			ApplyLastChangeToSubtasks(dwTaskID, nAttrib, FALSE);
		}
	}
	else if (WantUpdateInheritedAttibute(nAttrib))
	{
		ApplyLastChangeToSubtasks(dwTaskID, nAttrib);
	}
}

void CToDoCtrlData::ApplyLastInheritedChangeFromParent(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib)
{
	// special case: 
	if (nAttrib == TDCA_ALL)
	{
		POSITION pos = s_mapParentAttribs.GetStartPosition();

		while (pos)
		{
			TDC_ATTRIBUTE nAttrib = s_mapParentAttribs.GetNext(pos);
			ApplyLastInheritedChangeFromParent(dwTaskID, nAttrib);
		}
	}
	else if (WantUpdateInheritedAttibute(nAttrib))
	{
		const TODOSTRUCTURE* pTDS = LocateTask(dwTaskID);
		ASSERT(pTDS);

		if (pTDS && !pTDS->IsRoot() && !pTDS->ParentIsRoot())
		{
			DWORD dwParentID = pTDS->GetParentTaskID();

			const TODOSTRUCTURE* pTDSParent = NULL;
			const TODOITEM* pTDIParent = NULL;

			if (!GetTask(dwParentID, pTDIParent, pTDSParent))
			{
				ASSERT(0);
				return;
			}

			int nPos = pTDSParent->GetSubTaskPosition(dwTaskID);
			ASSERT(nPos != -1);

			ApplyLastChangeToSubtask(pTDIParent, pTDSParent, nPos, nAttrib, FALSE);
		}
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
		if (!ApplyLastChangeToSubtask(pTDI, pTDS, nSubTask, nAttrib, bIncludeBlank))
			return FALSE;
	}
	
	return TRUE;
}

BOOL CToDoCtrlData::ApplyLastChangeToSubtask(const TODOITEM* pTDIParent, const TODOSTRUCTURE* pTDSParent, 
											int nChildPos, TDC_ATTRIBUTE nAttrib, BOOL bIncludeBlank)
{
	
	if (!pTDIParent || !pTDSParent || (nChildPos < 0) || (nChildPos >= pTDSParent->GetSubTaskCount()))
	{
		ASSERT(0);
		return FALSE;
	}

	DWORD dwSubtaskID = pTDSParent->GetSubTaskID(nChildPos);
	TODOITEM* pTDIChild = NULL;

	GET_TDI(dwSubtaskID, pTDIChild, FALSE);

	// save undo data
	SaveEditUndo(dwSubtaskID, pTDIChild, nAttrib);

	// apply the change based on nAttrib
	switch (nAttrib)
	{
	case TDCA_DONEDATE:
		if (bIncludeBlank || pTDIParent->IsDone())
			pTDIChild->dateDone = pTDIParent->dateDone;
		break;

	case TDCA_DUEDATE:
	case TDCA_DUETIME:
		if (bIncludeBlank || pTDIParent->HasDue())
			pTDIChild->dateDue = pTDIParent->dateDue;
		break;

	case TDCA_STARTDATE:
	case TDCA_STARTTIME:
		if (bIncludeBlank || pTDIParent->HasStart())
			pTDIChild->dateStart = pTDIParent->dateStart;
		break;

	case TDCA_PRIORITY:
		if (bIncludeBlank || pTDIParent->nPriority != FM_NOPRIORITY)
			pTDIChild->nPriority = pTDIParent->nPriority;
		break;

	case TDCA_RISK:
		if (bIncludeBlank || pTDIParent->nRisk != FM_NORISK)
			pTDIChild->nRisk = pTDIParent->nRisk;
		break;

	case TDCA_COLOR:
		if (bIncludeBlank || pTDIParent->color != 0)
			pTDIChild->color = pTDIParent->color;
		break;

	case TDCA_ALLOCTO:
		if (bIncludeBlank || pTDIParent->aAllocTo.GetSize())
			pTDIChild->aAllocTo.Copy(pTDIParent->aAllocTo);
		break;

	case TDCA_ALLOCBY:
		if (bIncludeBlank || !pTDIParent->sAllocBy.IsEmpty())
			pTDIChild->sAllocBy = pTDIParent->sAllocBy;
		break;

	case TDCA_STATUS:
		if (bIncludeBlank || !pTDIParent->sStatus.IsEmpty())
			pTDIChild->sStatus = pTDIParent->sStatus;
		break;

	case TDCA_CATEGORY:
		if (bIncludeBlank || pTDIParent->aCategories.GetSize())
			pTDIChild->aCategories.Copy(pTDIParent->aCategories);
		break;

	case TDCA_TAGS:
		if (bIncludeBlank || pTDIParent->aTags.GetSize())
			pTDIChild->aTags.Copy(pTDIParent->aTags);
		break;

	case TDCA_PERCENT:
		if (bIncludeBlank || pTDIParent->nPercentDone)
			pTDIChild->nPercentDone = pTDIParent->nPercentDone;
		break;

	case TDCA_TIMEEST:
		if (bIncludeBlank || pTDIParent->dTimeEstimate > 0)
			pTDIChild->dTimeEstimate = pTDIParent->dTimeEstimate;
		break;

	case TDCA_FILEREF:
		if (bIncludeBlank || pTDIParent->aFileLinks.GetSize())
			pTDIChild->aFileLinks.Copy(pTDIParent->aFileLinks);
		break;

	case TDCA_VERSION:
		if (bIncludeBlank || !pTDIParent->sVersion.IsEmpty())
			pTDIChild->sVersion = pTDIParent->sVersion;
		break;

	case TDCA_FLAG:
		if (bIncludeBlank || pTDIParent->bFlagged)
			pTDIChild->bFlagged = pTDIParent->bFlagged;
		break;

	case TDCA_LOCK:
		if (bIncludeBlank || pTDIParent->bLocked)
			pTDIChild->bLocked = pTDIParent->bLocked;
		break;

	case TDCA_EXTERNALID:
		if (bIncludeBlank || !pTDIParent->sExternalID.IsEmpty())
			pTDIChild->sExternalID = pTDIParent->sExternalID;
		break;

	default:
		ASSERT (0);
		return FALSE;
	}

	// and its children too
	return ApplyLastChangeToSubtasks(pTDIChild, pTDSParent->GetSubTask(nChildPos), nAttrib, bIncludeBlank);
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

TDC_SET CToDoCtrlData::SetTaskComments(DWORD dwTaskID, const CString& sComments, const CBinaryData& customComments)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);
	
	BOOL bCommentsChange = (pTDI->sComments != sComments);
	BOOL bCustomCommentsChange = (bCommentsChange || (pTDI->customComments != customComments));
	
	if (bCommentsChange || bCustomCommentsChange)
	{
		// save undo data
		SaveEditUndo(dwTaskID, pTDI, TDCA_COMMENTS);
		
		if (bCommentsChange)
			pTDI->sComments = sComments;
		
		if (bCustomCommentsChange)
			pTDI->customComments = customComments;
		
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

	// test for actual change
	if (pTDI->sCommentsTypeID.CompareNoCase(sCommentsTypeID) == 0)
		return SET_NOCHANGE;

	TDC_SET nRes = DoEditTaskAttribute(dwTaskID, pTDI, TDCA_COMMENTS, pTDI->sCommentsTypeID, sCommentsTypeID);

	if (nRes == SET_CHANGE)
		pTDI->customComments.Empty();

	return nRes;
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

TDC_SET CToDoCtrlData::SetTaskLock(DWORD dwTaskID, BOOL bLocked)
{
	// Can't use EDIT_GET_TDI here because that fails if this task is Locked!
	TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, SET_FAILED);

	return EditTaskAttributeT(dwTaskID, pTDI, TDCA_LOCK, pTDI->bLocked, bLocked);
}

TDC_SET CToDoCtrlData::SetTaskCustomAttributeData(DWORD dwTaskID, const CString& sAttribID, const TDCCADATA& data)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);
	
	TDCCADATA dataOld;
	GetTaskCustomAttributeData(dwTaskID, sAttribID, dataOld);
	
	if (data != dataOld)
	{
		// save undo data
		SaveEditUndo(dwTaskID, pTDI, TDCA_CUSTOMATTRIB);
		
		// make changes
		pTDI->SetCustomAttributeValue(sAttribID, data);
		pTDI->SetModified();
		
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

		// And subtasks
		ApplyLastInheritedChangeToSubtasks(dwTaskID, TDC::MapDateToAttribute(nDate));
		
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

	TDC_SET nRes = SET_NOCHANGE;
	
	if (CDateHelper::IsDateSet(date))
	{
		CDateHelper::OffsetDate(date, nAmount, nDHUnits);

		// Special case: Task is recurring and therefore must fall on a valid date
		if (pTDI->IsRecurring() && bFitToRecurringScheme)
			pTDI->trRecurrence.FitDayToScheme(date);

		nRes = SetTaskDate(dwTaskID, pTDI, nDate, date);
	}

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

TDC_SET CToDoCtrlData::MoveTaskStartAndDueDates(DWORD dwTaskID, const COleDateTime& dtNewStart)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	// Sanity checks
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

	// Ignore tasks with dependencies where their dates 
	// are automatically calculated
	if (pTDI->aDependencies.GetSize() && HasStyle(TDCS_AUTOADJUSTDEPENDENCYDATES))
		return SET_NOCHANGE;
	
	// Cache current time estimate before doing anything
	double dDuration = pTDI->dTimeEstimate;
	
	if (dDuration == 0.0)
		dDuration = CalcDuration(pTDI->dateStart, pTDI->dateDue, pTDI->nTimeEstUnits);

	// recalc due date
	COleDateTime dtStart(dtNewStart);
	COleDateTime dtNewDue = AddDuration(dtStart, dDuration, pTDI->nTimeEstUnits);

	// FALSE -> don't recalc time estimate until due date is set
	TDC_SET nRes = SetTaskDate(dwTaskID, pTDI, TDCD_START, dtStart, FALSE);
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

			// FALSE = don't recalc time estimate
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

int CToDoCtrlData::IsValidDateRange(const COleDateTime& dateStart, const COleDateTime& dateDue)
{
	if (CDateHelper::DateHasTime(dateDue))
		return (dateStart <= dateDue);

	// else
	return (dateStart < CDateHelper::GetEndOfDay(dateDue));
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
				
				if (pTDI->HasStart() && pTDI->HasDue()) // both exist
				{
					// End date must be greater then start date
					if (!IsValidDateRange(pTDI->dateStart, pTDI->dateDue))
					{
						ASSERT(0);
						break;
					}

					double dDuration = CalcDuration(pTDI->dateStart, pTDI->dateDue, pTDI->nTimeEstUnits);
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

BOOL CToDoCtrlData::ResetRecurringSubtaskOccurrences(DWORD dwTaskID)
{
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDS(dwTaskID, pTDS, FALSE);
	
	for (int nSubtask = 0; nSubtask < pTDS->GetSubTaskCount(); nSubtask++)
	{
		DWORD dwSubtaskID = pTDS->GetSubTaskID(nSubtask);

		TODOITEM* pTDI = NULL;
		GET_TDI(dwSubtaskID, pTDI, FALSE);

		if (pTDI->IsRecurring())
		{
			int nNumOccur = pTDI->trRecurrence.GetOccurrenceCount();
			pTDI->trRecurrence.SetOccurrenceCount(nNumOccur, nNumOccur);
		}

		// then its subtasks
		ResetRecurringSubtaskOccurrences(dwSubtaskID);
	}

	return TRUE;
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

int CToDoCtrlData::GetTaskArray(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, CStringArray& aItems) const
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
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDS(dwTaskID, pTDS, SET_FAILED);

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

TDC_SET CToDoCtrlData::SetTaskExternalID(DWORD dwTaskID, const CString& sID)
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
	
	const TODOITEM* pTDI = GetTask(dwTaskID);
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
	CAutoFlag af(m_bUndoRedoing, TRUE);

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
	CToDoCtrlDataStructure tdsCopy(m_struct);
	
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
			if (HasTask(elm.dwTaskID))
			{
				// Should not exist
				ASSERT(0);
			}
			else
			{
				TODOITEM* pTDI = NewTask(elm.tdi);
				AddTask(elm.dwTaskID, pTDI, elm.dwParentID, elm.dwPrevSiblingID, FALSE);
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
		{
			SetTaskModified(dwDestParentID);

			// And update inherited attributes unless we are undoing
			if (!m_bUndoRedoing)
				ApplyLastInheritedChangeFromParent(dwTaskID, TDCA_ALL);
		}
	}
	
	return nPos;
}

BOOL CToDoCtrlData::FixupParentCompletion(DWORD dwParentID, BOOL bClearStatus)
{
	if (!dwParentID) // top level item 
		return TRUE;
		
	// if the parent was marked as done and it has any incomplete
	// subtasks then mark the parent as incomplete too
	const TODOITEM* pTDIParent = NULL;
	const TODOSTRUCTURE* pTDSParent = NULL;
	GET_TDI_TDS(dwParentID, pTDIParent, pTDSParent, FALSE);

	if (pTDIParent->IsDone() && TaskHasIncompleteSubtasks(pTDSParent, FALSE))
	{
		SetTaskDate(dwParentID, TDCD_DONE, 0.0);

		if (bClearStatus)
			ClearTaskAttribute(dwParentID, TDCA_STATUS);
	}

	// and this parent's immediate parent
	FixupParentCompletion(pTDSParent->GetParentTaskID(), bClearStatus); // RECURSIVE call

	return TRUE;
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
	CToDoCtrlDataStructure tdsCopy(m_struct);
	
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

BOOL CToDoCtrlData::TaskHasSubtasks(DWORD dwTaskID) const
{
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDS(dwTaskID, pTDS, FALSE);

	return pTDS->HasSubTasks();
}

BOOL CToDoCtrlData::TaskHasCompletedSubtasks(DWORD dwTaskID) const
{
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDS(dwTaskID, pTDS, FALSE);

	return TaskHasCompletedSubtasks(pTDS);
}

BOOL CToDoCtrlData::TaskHasCompletedSubtasks(const TODOSTRUCTURE* pTDS) const
{
	if (!pTDS)
	{
		ASSERT(0);
		return FALSE;
	}

	// process its subtasks
	int nPos = pTDS->GetSubTaskCount();
	
	while (nPos--)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nPos);
		
		if (IsTaskDone(pTDSChild->GetTaskID()))
			return TRUE;
		
		// Grandchildren
		if (TaskHasCompletedSubtasks(pTDSChild))
			return TRUE;
	}
	
	// else
	return FALSE;
}

BOOL CToDoCtrlData::TaskHasIncompleteSubtasks(DWORD dwTaskID, BOOL bExcludeRecurring) const
{
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDS(dwTaskID, pTDS, FALSE);

	return TaskHasIncompleteSubtasks(pTDS, bExcludeRecurring);
}

BOOL CToDoCtrlData::TaskHasIncompleteSubtasks(const TODOSTRUCTURE* pTDS, BOOL bExcludeRecurring) const
{
	if (!pTDS)
	{
		ASSERT(0);
		return FALSE;
	}

	// process its subtasks
	int nPos = pTDS->GetSubTaskCount();

	while (nPos--)
	{
		const TODOSTRUCTURE* pTDSChild = pTDS->GetSubTask(nPos);
		const TODOITEM* pTDIChild = GetTrueTask(pTDSChild);

		if (!pTDIChild || !pTDSChild)
		{
			ASSERT(0);
			return FALSE;
		}
		
		// ignore recurring tasks and their children
		if (bExcludeRecurring && pTDIChild->IsRecurring())
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

void CToDoCtrlData::SetDefaultCommentsFormat(const CString& format) 
{ 
	s_cfDefault = format; 
}

void CToDoCtrlData::SetDefaultTimeUnits(TDC_UNITS nTimeEstUnits, TDC_UNITS nTimeSpentUnits)
{
	s_nDefTimeEstUnits = nTimeEstUnits;
	s_nDefTimeSpentUnits = nTimeSpentUnits;
}

// external version returning const
BOOL CToDoCtrlData::GetTask(DWORD dwTaskID, const TODOITEM*& pTDI, const TODOSTRUCTURE*& pTDS) const
{
	return GetTask(dwTaskID, pTDI, pTDS, FALSE);
}

// external version returning const
BOOL CToDoCtrlData::GetTrueTask(DWORD& dwTaskID, const TODOITEM*& pTDI, const TODOSTRUCTURE*& pTDS) const
{
	return GetTask(dwTaskID, pTDI, pTDS, TRUE);
}

// internal version returning const
BOOL CToDoCtrlData::GetTask(DWORD& dwTaskID, const TODOITEM*& pTDI, const TODOSTRUCTURE*& pTDS, BOOL bTrue) const
{
	// get item and structure
	pTDI = GetTask(dwTaskID, bTrue);
	pTDS = LocateTask(dwTaskID);

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
	if (!pTDS->IsRoot() && (GetTaskReferenceID(pTDS->GetTaskID()) == dwTaskID))
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

BOOL CToDoCtrlData::IsTaskDone(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);

	return pTDI->IsDone();
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

	COleDateTime dateDue(dateStart);
	
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
			dateDue.m_dt += dDays;
		}
		break;

	case TDCU_DAYS:
		dateDue.m_dt += dDuration;
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

				dateDue = dateStart;

				while (dDaysLeft > 0.0)
				{
					dDaysLeft--;
					dateDue.m_dt += nDir;

					// adjust for partial day overrun
					if (dDaysLeft < 0.0)
						dateDue.m_dt += (nDir * dDaysLeft);

					// step over weekends
					if ((dDaysLeft > 0.0) || CDateHelper::DateHasTime(dateDue))
					{
						// FALSE -> Don't truncate time
						CDateHelper::MakeWeekday(dateDue, bForward, FALSE);
					}
				}
			}
			else
			{
				dateDue.m_dt += dDuration;
			}
		}
		break;
	}

	// If date falls on the beginning of a day, move to end of previous day
	if (dDuration > 0.0)
	{
		if (!CDateHelper::DateHasTime(dateDue))
			dateDue.m_dt--;
	}
	else
	{
		if (!CDateHelper::DateHasTime(dateStart))
			dateDue.m_dt++;
	}
	
	// sanity check
#ifdef _DEBUG
	if (nUnits != TDCU_WEEKDAYS)
	{
		double dCheck = 0.0;
		
		if (dDuration > 0.0)
			dCheck = CalcDuration(dateStart, dateDue, nUnits);
		else
			dCheck = -CalcDuration(dateDue, dateStart, nUnits);

		ASSERT(fabs(dCheck - dDuration) < 1e-3);
	}
#endif

	return dateDue;
}

double CToDoCtrlData::CalcDuration(const COleDateTime& dateStart, const COleDateTime& dateDue, TDC_UNITS nUnits)
{
	// Sanity checks
	if (!CDateHelper::IsDateSet(dateStart) || !CDateHelper::IsDateSet(dateDue))
	{
		ASSERT(0);
		return 0.0;
	}

	// End date must be greater then start date
	if (!IsValidDateRange(dateStart, dateDue))
	{
		ASSERT(0);
		return 0.0;
	}

	double dDuration = (dateDue.m_dt - dateStart.m_dt); // in days
	
	switch (nUnits)
	{
	case TDCU_MINS:
	case TDCU_HOURS:
	case TDCU_WEEKS:
	case TDCU_MONTHS:
	case TDCU_YEARS:
		{
			// handle 'whole' of due date
			if (IsEndOfDay(dateDue))
				dDuration += 1.0;

			CTimeHelper thAllDay(24.0, 7.0);
			dDuration = thAllDay.GetTime(dDuration, THU_DAYS, TDC::MapUnitsToTHUnits(nUnits));
		}
		break;

	case TDCU_WEEKDAYS:
		if (CDateHelper::HasWeekend())
		{
			// process each whole or part day  
			double dDayStart(dateStart);
			dDuration = 0.0;

			while (dDayStart < dateDue)
			{
				// determine the end of this day
				double dDayEnd = (CDateHelper::GetDateOnly(dDayStart).m_dt + 1.0);

				if (!CDateHelper::IsWeekend(dDayStart))
				{
					dDuration += (min(dDayEnd, dateDue) - dDayStart); // in days
				}

				// next day
				dDayStart = dDayEnd;
			}

			// handle 'whole' of due date
			if (CDateHelper::IsWeekend(dateDue) || !IsEndOfDay(dateDue))
				break;
		}
		// else fall thru to handle 'whole' of due date

	case TDCU_DAYS:
		// handle 'whole' of due date
		if (IsEndOfDay(dateDue))
			dDuration += 1.0;
		break;
	}

	return dDuration;
}

void CToDoCtrlData::FixupTaskLocalDependentsDates(DWORD dwTaskID, TDC_DATE nDate)
{
	if (!HasStyle(TDCS_AUTOADJUSTDEPENDENCYDATES))
		return;
	
	// who is dependent on us -> GetTaskDependents
	CDWordArray aDependents;
	int nDepend = GetTaskLocalDependents(dwTaskID, aDependents);

	while (nDepend--)
	{
		DWORD dwIDDependent = aDependents[nDepend];

		// check for circular dependency before continuing
		if (TaskHasLocalCircularDependencies(dwIDDependent))
			return;
	
		UINT nAdjusted = UpdateTaskLocalDependencyDates(dwIDDependent, nDate);
		
		// then process this task's dependents
		// if the due date was actually modified
		if (Misc::HasFlag(nAdjusted, ADJUSTED_DUE))
		{
			FixupTaskLocalDependentsDates(dwIDDependent, TDCD_DUE);
		}
	}
}

TDC_SET CToDoCtrlData::CopyTaskAttributeData(DWORD dwTaskID, TDC_ATTRIBUTE nFromAttrib, TDC_ATTRIBUTE nToAttrib)
{
	TDCCADATA data;

	if (!GetTaskAttributeData(dwTaskID, nFromAttrib, data))
		return SET_FAILED;

	// else
	return SetTaskAttributeData(dwTaskID, nToAttrib, data);
}

TDC_SET CToDoCtrlData::CopyTaskAttributeData(DWORD dwTaskID, TDC_ATTRIBUTE nFromAttrib, const CString& sToCustomAttribID)
{
	TDCCADATA data;

	if (!GetTaskAttributeData(dwTaskID, nFromAttrib, data))
		return SET_FAILED;

	// else
	return SetTaskCustomAttributeData(dwTaskID, sToCustomAttribID, data);
}

BOOL CToDoCtrlData::GetTaskAttributeData(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, TDCCADATA& data) const
{
	data.Clear();

	switch (nAttrib)
	{
	case TDCA_VERSION:		data.Set(GetTaskVersion(dwTaskID));			break;
	case TDCA_ALLOCBY:		data.Set(GetTaskAllocBy(dwTaskID));			break;	
	case TDCA_CREATEDBY:	data.Set(GetTaskCreatedBy(dwTaskID));		break;
	case TDCA_EXTERNALID:	data.Set(GetTaskExtID(dwTaskID));			break;	
	case TDCA_STATUS:		data.Set(GetTaskStatus(dwTaskID));			break;	
	case TDCA_TASKNAME:		data.Set(GetTaskTitle(dwTaskID));			break;
	case TDCA_COMMENTS:		data.Set(GetTaskComments(dwTaskID));		break;
	case TDCA_LASTMODBY:	data.Set(GetTaskLastModifiedBy(dwTaskID));	break;

	case TDCA_COLOR:		data.Set((int)GetTaskColor(dwTaskID));		break;	
	case TDCA_PRIORITY:		data.Set(GetTaskPriority(dwTaskID));		break;	
	case TDCA_RISK:			data.Set(GetTaskRisk(dwTaskID));			break;	
	case TDCA_PERCENT:		data.Set(GetTaskPercent(dwTaskID, FALSE));	break;	
	case TDCA_COST:			data.Set(GetTaskCost(dwTaskID));			break;	
	case TDCA_FLAG:			data.Set(IsTaskFlagged(dwTaskID));			break;	
	case TDCA_ICON:			data.Set(GetTaskIcon(dwTaskID));			break;	
	case TDCA_LOCK:			data.Set(IsTaskLocked(dwTaskID));			break;	

	case TDCA_FILEREF:	
	case TDCA_DEPENDENCY:
	case TDCA_ALLOCTO:			
	case TDCA_CATEGORY:			
	case TDCA_TAGS:	
		{
			CStringArray aValues;
			GetTaskArray(dwTaskID, nAttrib, aValues);

			data.Set(aValues);
		}
		break;

	case TDCA_CREATIONDATE:		
	case TDCA_DONEDATE:			
	case TDCA_DUEDATE:			
	case TDCA_LASTMODDATE:			
	case TDCA_STARTDATE:		
	case TDCA_DONETIME:			
	case TDCA_DUETIME:			
	case TDCA_STARTTIME:
		{
			TDC_DATE nDate = TDC::MapAttributeToDate(nAttrib);
			COleDateTime date = GetTaskDate(dwTaskID, nDate);

			data.Set(date);
		}
		break;

	case TDCA_TIMEEST:			
		{
			TDC_UNITS nUnits;
			double dTime = GetTaskTimeEstimate(dwTaskID, nUnits);

			data.Set(dTime, nUnits);
		}
		break;

	case TDCA_TIMESPENT:	
		{
			TDC_UNITS nUnits;
			double dTime = GetTaskTimeSpent(dwTaskID, nUnits);

			data.Set(dTime, nUnits);
		}
		break;
	}

	return !data.IsEmpty();
}

TDC_SET CToDoCtrlData::CopyTaskAttributeData(DWORD dwTaskID, const CString& sFromCustomAttribID, TDC_ATTRIBUTE nToAttrib)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	TDCCADATA data;

	if (!pTDI->GetCustomAttributeValues().Lookup(sFromCustomAttribID, data))
		return SET_FAILED;

	return SetTaskAttributeData(dwTaskID, nToAttrib, data);
}

TDC_SET CToDoCtrlData::SetTaskAttributeData(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, const TDCCADATA& data)
{
	switch (nAttrib)
	{
	// TDCA_CREATEDBY not supported
	// TDCA_CREATIONDATE not supported
	// TDCA_LASTMOD not supported
	// TDCA_LASTMODBY not supported

	case TDCA_VERSION:		return SetTaskVersion(dwTaskID, data.AsString());
	case TDCA_ALLOCBY:		return SetTaskAllocBy(dwTaskID, data.AsString());
	case TDCA_EXTERNALID:	return SetTaskExternalID(dwTaskID, data.AsString());
	case TDCA_STATUS:		return SetTaskStatus(dwTaskID, data.AsString());
	case TDCA_TASKNAME:		return SetTaskTitle(dwTaskID, data.AsString());
	case TDCA_COMMENTS:		return SetTaskComments(dwTaskID, data.AsString());
	case TDCA_ICON:			return SetTaskIcon(dwTaskID, data.AsString());

	case TDCA_COLOR:		return SetTaskColor(dwTaskID, data.AsInteger());
	case TDCA_PRIORITY:		return SetTaskPriority(dwTaskID, data.AsInteger());
	case TDCA_RISK:			return SetTaskRisk(dwTaskID, data.AsInteger());
	case TDCA_PERCENT:		return SetTaskPercent(dwTaskID, data.AsInteger());
	case TDCA_COST:			return SetTaskCost(dwTaskID, data.AsDouble());
	case TDCA_FLAG:			return SetTaskFlag(dwTaskID, data.AsBool());
	case TDCA_LOCK:			return SetTaskLock(dwTaskID, data.AsBool());

	case TDCA_FILEREF:	
	case TDCA_DEPENDENCY:
	case TDCA_ALLOCTO:			
	case TDCA_CATEGORY:			
	case TDCA_TAGS:	
		{
			CStringArray aValues;
			data.AsArray(aValues);

			return SetTaskArray(dwTaskID, nAttrib, aValues, TRUE);
		}
		break;

	case TDCA_DONEDATE:			
	case TDCA_DUEDATE:			
	case TDCA_STARTDATE:		
	case TDCA_DONETIME:			
	case TDCA_DUETIME:			
	case TDCA_STARTTIME:
		{
			COleDateTime date = data.AsDate();
			TDC_DATE nDate = TDC::MapAttributeToDate(nAttrib);

			return SetTaskDate(dwTaskID, nDate, date);
		}
		break;

	case TDCA_TIMEEST:			
		{
			TDC_UNITS nUnits;
			double dTime = data.AsTimePeriod(nUnits);
			
			return SetTaskTimeEstimate(dwTaskID, dTime, nUnits);
		}
		break;

	case TDCA_TIMESPENT:	
		{
			TDC_UNITS nUnits;
			double dTime = data.AsTimePeriod(nUnits);

			return SetTaskTimeSpent(dwTaskID, dTime, nUnits);
		}
		break;
	}

	return SET_FAILED;
}

TDC_SET CToDoCtrlData::CopyTaskAttributeData(DWORD dwTaskID, const CString& sFromCustomAttribID, const CString& sToCustomAttribID)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	TDCCADATA data;

	if (!pTDI->GetCustomAttributeValue(sFromCustomAttribID, data))
		return SET_FAILED;

	return SetTaskCustomAttributeData(dwTaskID, sToCustomAttribID, data);
}
