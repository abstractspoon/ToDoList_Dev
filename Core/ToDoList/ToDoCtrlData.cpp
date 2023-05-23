// ToDoCtrlData.cpp: implementation of the CToDoCtrlData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToDoCtrlData.h"
#include "ToDoCtrlDataDefines.h"
#include "resource.h"

#include "..\shared\timehelper.h"
#include "..\shared\datehelper.h"
#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\enstring.h"
#include "..\shared\autoflag.h"
#include "..\shared\mapex.h"
#include "..\shared\WorkingWeek.h"

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

CUndoAction::CUndoAction(CToDoCtrlData& data, TDC_UNDOACTIONTYPE nType, BOOL bExtendLastAction) 
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

static const CString EMPTY_STR;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToDoCtrlData::CToDoCtrlData(const CTDCStyleMap& styles, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs)
	: 
	m_styles(styles),
	m_aCustomAttribDefs(aCustomAttribDefs),
	m_bUndoRedoing(FALSE),
	m_nDefTimeEstUnits(TDCU_DAYS),
	m_nDefTimeSpentUnits(TDCU_DAYS)
{
}

CToDoCtrlData::~CToDoCtrlData()
{
	ASSERT(!m_bUndoRedoing);

	DeleteAllTasks();
}

void CToDoCtrlData::SetInheritedParentAttributes(const CTDCAttributeMap& mapAttribs, BOOL bUpdateAttrib)
{
	m_mapParentAttribs.Copy(mapAttribs);
	m_bUpdateInheritAttrib = bUpdateAttrib;
}

BOOL CToDoCtrlData::WantUpdateInheritedAttibute(TDC_ATTRIBUTE nAttribID) const
{
	if (m_bUpdateInheritAttrib)
	{
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID) &&
			m_mapParentAttribs.Has(TDCA_CUSTOMATTRIB))
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_DEF_RET(m_aCustomAttribDefs, nAttribID, pDef, FALSE);

			return pDef->HasFeature(TDCCAF_INHERITPARENTCHANGES);
		}
		else if ((nAttribID == TDCA_ALL) && !m_mapParentAttribs.IsEmpty())
		{
			return TRUE;
		}

		// else
		return m_mapParentAttribs.Has(nAttribID);
	}

	// All else
	return FALSE;
}

int CToDoCtrlData::BuildDataModel(const CTaskFile& tasks)
{
	m_undo.ResetAll();
	DeleteAllTasks();

	// add top-level items
	VERIFY(AddTaskToDataModel(tasks, NULL, &m_struct));

	RemoveOrphanTaskReferences(&m_struct, FALSE); // no undo

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

TODOITEM* CToDoCtrlData::NewTask(const TODOITEM& tdiRef, DWORD dwParentTaskID) const
{
	TODOITEM* pTDI = new TODOITEM(tdiRef);

	// copy over parent attribs
	if (dwParentTaskID && !m_mapParentAttribs.IsEmpty())
		CopyTaskAttributes(pTDI, dwParentTaskID, m_mapParentAttribs);
	
	return pTDI;
}

TODOITEM* CToDoCtrlData::NewTask(const CTaskFile& tasks, HTASKITEM hTask, const TODOITEM* pTDIRef) const
{
	ASSERT(hTask);

	if (!hTask)
		return NULL;

	TODOITEM* pTDI = (pTDIRef ? NewTask(*pTDIRef) : (new TODOITEM()));

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
	if (pTDI->cfComments.IsEmpty()) 
		pTDI->cfComments = m_cfDefault;

	return pTDI;
}

BOOL CToDoCtrlData::HasTask(DWORD dwTaskID) const
{
	return m_items.HasTask(dwTaskID);
}

BOOL CToDoCtrlData::HasTasks(const CDWordArray& aTaskIDs) const
{
	int nID = aTaskIDs.GetSize();

	while (nID--)
	{
		if (!m_items.HasTask(aTaskIDs[nID]))
			break;
	}

	return (nID == -1);
}

BOOL CToDoCtrlData::TaskHasParent(DWORD dwTaskID, DWORD dwParentID, BOOL bImmediate) const
{
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDS(dwTaskID, pTDS, FALSE);

	return pTDS->HasParent(dwParentID, bImmediate);
}

BOOL CToDoCtrlData::TaskHasSibling(DWORD dwTaskID, DWORD dwSiblingID, BOOL bImmediate) const
{
	if ((dwTaskID == 0) || (dwSiblingID == 0))
	{
		ASSERT(0);
		return FALSE;
	}

	// Check they have the same parent
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDS(dwTaskID, pTDS, FALSE);

	const TODOSTRUCTURE* pTDSParent = pTDS->GetParentTask();

	if (!TaskHasParent(dwSiblingID, pTDS->GetParentTaskID()))
		return FALSE;

	if (!bImmediate)
		return TRUE;

	// Check they have adjacent positions
	int nPos = GetTaskPosition(pTDS);
	ASSERT((nPos != -1) && (pTDSParent->GetSubTaskID(nPos) == dwTaskID));

	return ((pTDSParent->GetPreviousSubTaskID(nPos) == dwSiblingID) ||
			(pTDSParent->GetNextSubTaskID(nPos) == dwSiblingID));
}

// for friend classes
int CToDoCtrlData::GetTaskPosition(const TODOSTRUCTURE* pTDS, BOOL bZeroBased) const
{
	return m_struct.GetTaskPosition(pTDS->GetTaskID(), bZeroBased);
}

int CToDoCtrlData::GetTaskPositions(DWORD dwTaskID, CArray<int, int>& aPositions, BOOL bZeroBased) const
{
	return m_struct.GetTaskPositions(dwTaskID, aPositions, bZeroBased);
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

BOOL CToDoCtrlData::AddTask(DWORD dwTaskID, TODOITEM* pTDI, DWORD dwParentID, DWORD dwPrevSiblingID) 
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
	
	VERIFY(m_bUndoRedoing || AddUndoElement(TDCUEO_ADD, dwTaskID, dwParentID, dwPrevSiblingID));

	return TRUE;
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
			DeleteTask(&m_struct, 0, TRUE); // TRUE == with undo
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

int CToDoCtrlData::GetTaskCommentsLength(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0);
	
	return pTDI->sComments.GetLength();
}

const CBinaryData& CToDoCtrlData::GetTaskCustomComments(DWORD dwTaskID, CONTENTFORMAT& cfComments) const
{
	static CBinaryData content;
	cfComments.Empty();

	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, content);
	
	cfComments = pTDI->cfComments;
	return pTDI->customComments;
}

BOOL CToDoCtrlData::GetTaskCost(DWORD dwTaskID, TDCCOST& cost) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);
	
	cost = pTDI->cost;
	return TRUE;
}

BOOL CToDoCtrlData::GetTaskTimeEstimate(DWORD dwTaskID, TDCTIMEPERIOD& timeEst) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);

	timeEst = pTDI->timeEstimate;
	return TRUE;
}

BOOL CToDoCtrlData::GetTaskTimeSpent(DWORD dwTaskID, TDCTIMEPERIOD& timeSpent) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);
	
	timeSpent = pTDI->timeSpent;
	return TRUE;
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

CString CToDoCtrlData::GetTaskMetaData(DWORD dwTaskID, const CString& sKey) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, EMPTY_STR);

	return pTDI->GetMetaData(sKey);
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

int CToDoCtrlData::GetTaskDependencies(DWORD dwTaskID, CTDCDependencyArray& aDependencies) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0);
	
	aDependencies.Copy(pTDI->aDependencies);
	return aDependencies.GetSize();
}

int CToDoCtrlData::GetTaskDependencies(DWORD dwTaskID, CStringArray& aDepends) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0);

	return pTDI->aDependencies.Format(aDepends);
}

int CToDoCtrlData::GetTaskDependencies(DWORD dwTaskID, CDWordArray& aLocalDepends, CStringArray& aOtherDepends) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0);

	return pTDI->aDependencies.GetDependencies(aLocalDepends, aOtherDepends);
}

BOOL CToDoCtrlData::IsTaskLocallyDependentOn(DWORD dwTaskID, DWORD dwOtherID, BOOL bImmediateOnly) const
{
	ASSERT(dwOtherID);

	if (!dwOtherID)
		return FALSE;

	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);
	
	if (bImmediateOnly)
		return pTDI->HasLocalDependency(dwOtherID);

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
	// in their list of local dependencies
	const TODOITEM* pTDI = NULL;
	POSITION pos = m_items.GetStart();
		
	while (pos)
	{
		DWORD dwDependsID = m_items.GetNextTask(pos, pTDI);
		ASSERT (dwDependsID && pTDI);

		if (pTDI && (dwDependsID != dwTaskID) && pTDI->HasLocalDependency(dwTaskID))
			return TRUE;
	}	

	// else
	return FALSE;
}

int CToDoCtrlData::GetAllTaskDependents(CDWordSet& mapDependents) const
{
	mapDependents.RemoveAll();

	POSITION pos = m_items.GetStart();

	while (pos)
	{
		TODOITEM* pTDI = NULL;
		DWORD dwTaskID = 0;

		m_items.GetNext(pos, dwTaskID, pTDI);

		if (pTDI && pTDI->aDependencies.GetSize())
			mapDependents.Add(dwTaskID);
	}

	return mapDependents.GetCount();
}

int CToDoCtrlData::GetTaskLocalDependents(DWORD dwTaskID, CDWordArray& aDependents, BOOL bImmediateOnly) const
{
	aDependents.RemoveAll();

	CDWordSet mapAllDependents;

	if (!GetAllTaskDependents(mapAllDependents))
		return 0;

	CDWordSet mapTaskDependents;

	if (GetTaskLocalDependents(dwTaskID, mapAllDependents, mapTaskDependents, bImmediateOnly))
		mapTaskDependents.CopyTo(aDependents);

	return aDependents.GetSize();
}

int CToDoCtrlData::GetTaskLocalDependents(const CDWordArray& aTaskIDs, CDWordArray& aDependents, BOOL bImmediateOnly) const
{
	aDependents.RemoveAll();

	CDWordSet mapAllDependents;

	if (!GetAllTaskDependents(mapAllDependents))
		return 0;

	int nID = aTaskIDs.GetSize();
	CDWordSet mapDependents;

	while (nID--)
	{
		CDWordSet mapTaskDependents;

		if (GetTaskLocalDependents(aTaskIDs[nID], mapAllDependents, mapTaskDependents, bImmediateOnly))
			mapDependents.Append(mapTaskDependents);
	}

	mapDependents.CopyTo(aDependents);

	return aDependents.GetSize();
}

int CToDoCtrlData::GetTaskLocalDependents(DWORD dwTaskID, const CDWordSet& mapAllDependents, CDWordSet& mapDependents, BOOL bImmediateOnly) const
{
	if (bImmediateOnly)
		mapDependents.RemoveAll();

	if (dwTaskID)
	{
		// Find all tasks dependent on 'dwTaskID' within the same task list
		const TODOITEM* pTDI = NULL;
		POSITION pos = mapAllDependents.GetStartPosition();
		
		while (pos)
		{
			DWORD dwDependentID = mapAllDependents.GetNext(pos);

			if (!mapDependents.Has(dwDependentID))
			{
				TODOITEM* pTDI = m_items.GetTask(dwDependentID);

				if (pTDI && pTDI->HasLocalDependency(dwTaskID))
				{
					mapDependents.Add(dwDependentID);

					if (!bImmediateOnly)
						GetTaskLocalDependents(dwDependentID, mapAllDependents, mapDependents, FALSE); // RECURSIVE CALL
				}
			}
		}
	}

	return mapDependents.GetCount();
}

void CToDoCtrlData::FixupTaskLocalDependentsIDs(DWORD dwTaskID, DWORD dwPrevTaskID)
{
	CDWordArray aDependents;
	
	if (GetTaskLocalDependents(dwPrevTaskID, aDependents, FALSE))
	{
		int nTask = aDependents.GetSize();

		while (nTask--)
		{
			DWORD dwDependentID = aDependents[nTask];
			CStringArray aDepends;
	
			TODOITEM* pTDIDepends = GetTask(dwDependentID, TRUE);
			ASSERT(pTDIDepends);

			if (pTDIDepends)
				VERIFY(pTDIDepends->ReplaceLocalDependency(dwPrevTaskID, dwTaskID));
		}
	}
}

BOOL CToDoCtrlData::InsertTaskIntoDependencyChain(DWORD dwTaskID, DWORD dwAfterID)
{
	// Sanity checks
	if ((!dwTaskID || IsTaskReference(dwTaskID)) ||
		(!dwAfterID || IsTaskReference(dwAfterID)))
	{
		ASSERT(0);
		return FALSE;
	}

	if (IsTaskLocallyDependentOn(dwTaskID, dwAfterID, FALSE) ||
		IsTaskLocallyDependentOn(dwAfterID, dwTaskID, FALSE))
	{
		ASSERT(0);
		return FALSE;
	}

	// Get the current immediate dependents of 'dwDependency' 
	// before changing them
	CDWordArray aDependentIDs;
	GetTaskLocalDependents(dwAfterID, aDependentIDs, TRUE);

	// Set the new task to be dependent on 'dwDependency'
	CTDCDependencyArray aDepends;
	aDepends.Add(dwAfterID);

	if (SetTaskDependencies(dwTaskID, aDepends) != SET_CHANGE)
	{
		ASSERT(0);
		return FALSE;
	}

	// Fixup the previous dependents to point to the new task
	int nID = aDependentIDs.GetSize();

	if (nID > 0)
	{
		// Make sure the new task has a due date
		if (!TaskHasDate(dwTaskID, TDCD_DUE) && TaskHasDate(dwTaskID, TDCD_START))
		{
			TODOITEM* pTDI = m_items.GetTask(dwTaskID);

			if (!CalcMissingDueDateFromStart(pTDI))
				pTDI->dateDue = CDateHelper::GetDateOnly(pTDI->dateStart);
		}

		while (nID--)
		{
			DWORD dwDependentID = aDependentIDs[nID];

			CTDCDependencyArray aTaskDepends;
			GetTaskDependencies(dwDependentID, aTaskDepends);

			VERIFY(aTaskDepends.ReplaceLocalDependency(dwAfterID, dwTaskID));
			SetTaskDependencies(dwDependentID, aTaskDepends);
		}
	}

	return TRUE;
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

CString CToDoCtrlData::GetTaskCustomAttributeData(DWORD dwTaskID, const CString& sAttribID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, EMPTY_STR);

	TDCCADATA data;
	pTDI->GetCustomAttributeValue(sAttribID, data);

	return data.AsString();
}

BOOL CToDoCtrlData::IsTaskLocked(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);

	return pTDI->bLocked;
}

BOOL CToDoCtrlData::TaskHasFileLink(DWORD dwTaskID) const
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

BOOL CToDoCtrlData::IsTaskReusableRecurring(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);

	return (pTDI->IsRecurring() && (pTDI->trRecurrence.nReuse == TDIRO_REUSE));
}

BOOL CToDoCtrlData::GetTaskRecurrence(DWORD dwTaskID, TDCRECURRENCE& tr) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);
	
	tr = pTDI->trRecurrence;
	return TRUE;
}

BOOL CToDoCtrlData::GetNextTaskOccurrence(DWORD dwTaskID, COleDateTime& dtNext, BOOL& bDue)
{
	TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);
	
	return pTDI->GetNextOccurence(dtNext, bDue);
}

BOOL CToDoCtrlData::CalcNextTaskOccurences(DWORD dwTaskID, const COleDateTimeRange& dtRange, CArray<double, double&>& aDates, BOOL& bDue) const
{
	TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);

	return pTDI->CalcNextOccurences(dtRange, aDates, bDue);
}

COleDateTime CToDoCtrlData::GetTaskDate(DWORD dwTaskID, TDC_DATE nDate) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0.0);
	
	return pTDI->GetDate(nDate);
}

BOOL CToDoCtrlData::GetTaskStartDueDates(DWORD dwTaskID, COleDateTimeRange& dtRange) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);
	
	return pTDI->GetStartDueDates(dtRange);
}

BOOL CToDoCtrlData::TaskHasDate(DWORD dwTaskID, TDC_DATE nDate) const
{
	return CDateHelper::IsDateSet(GetTaskDate(dwTaskID, nDate));
}

int CToDoCtrlData::GetTaskPercent(DWORD dwTaskID) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0);
	
	return pTDI->nPercentDone;
}

CString CToDoCtrlData::GetTaskFileLink(DWORD dwTaskID, int nFileLink) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, EMPTY_STR);
	
	return pTDI->GetFileLink(nFileLink);
}

int CToDoCtrlData::GetTaskFileLinks(DWORD dwTaskID, CStringArray& aFiles) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, 0);
	
	aFiles.Copy(pTDI->aFileLinks);
	return aFiles.GetSize();
}

int CToDoCtrlData::GetTaskFileLinkCount(DWORD dwTaskID) const
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

int CToDoCtrlData::GetTaskParentIDs(DWORD dwTaskID, CDWordArray& aTaskIDs) const
{
	aTaskIDs.RemoveAll();

	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDS(dwTaskID, pTDS, 0);

	while (!pTDS->ParentIsRoot())
	{
		Misc::AddUniqueItemT(pTDS->GetParentTaskID(), aTaskIDs);
		pTDS = pTDS->GetParentTask();
	}

	return aTaskIDs.GetSize();
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
	if (!dwTestID || !dwTaskID)
	{
		ASSERT(0);
		return FALSE;
	}

	return (GetTaskReferenceID(dwTestID) == dwTaskID);
}

BOOL CToDoCtrlData::DeleteTask(DWORD dwTaskID, BOOL bWithUndo)
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
		return DeleteTask(pTDSParent, nPos, bWithUndo);
	}

	// else
	return FALSE;
}

BOOL CToDoCtrlData::DeleteTask(TODOSTRUCTURE* pTDSParent, int nPos, BOOL bWithUndo)
{
	ASSERT(!bWithUndo || !m_bUndoRedoing);

	TODOSTRUCTURE* pTDS = pTDSParent->GetSubTask(nPos);
	ASSERT(pTDS);
	
	// do children first to ensure entire branch is deleted
	while (pTDS->GetSubTaskCount() > 0)
		DeleteTask(pTDS, 0, bWithUndo); // RECURSIVE CALL

	// is this task a reference?
	DWORD dwTaskID = pTDS->GetTaskID();
	BOOL bRef = IsTaskReference(dwTaskID);

	// save undo 
	DWORD dwParentID = pTDSParent->GetTaskID();
	DWORD dwPrevSiblingID = pTDSParent->GetPreviousSubTaskID(nPos);
	
	VERIFY (!bWithUndo || AddUndoElement(TDCUEO_DELETE, dwTaskID, dwParentID, dwPrevSiblingID));
	
	// then this item
	m_items.DeleteTask(dwTaskID);
	m_struct.DeleteTask(dwTaskID);

	// then update it's referees and dependents unless it was a reference itself
	if (!bRef && !m_bUndoRedoing)
	{
		RemoveOrphanTaskReferences(&m_struct, bWithUndo, dwTaskID);
		RemoveOrphanTaskLocalDependencies(&m_struct, bWithUndo, dwTaskID);
	}
	
	return TRUE;
}

BOOL CToDoCtrlData::RemoveOrphanTaskReferences(TODOSTRUCTURE* pTDSParent, BOOL bWithUndo, DWORD dwMatchID)
{
	// Undo/redo takes care of this itself because it has
	// a precise list of what to delete already
	ASSERT(!m_bUndoRedoing);

	if (!pTDSParent)
	{
		ASSERT(0);
		return FALSE;
	}
	
	int nChild = pTDSParent->GetSubTaskCount();
	BOOL bRemoved = FALSE;
	
	while (nChild--)
	{
		TODOSTRUCTURE* pTDSChild = pTDSParent->GetSubTask(nChild);
		
		// children's children first
		if (RemoveOrphanTaskReferences(pTDSChild, bWithUndo, dwMatchID))
			bRemoved = TRUE;
		
		// then child
		const TODOITEM* pTDIChild = GetTask(pTDSChild);
		ASSERT(pTDIChild);
		
		if (pTDIChild && pTDIChild->IsReference())
		{
			BOOL bDeleteRef = (dwMatchID && (pTDIChild->dwTaskRefID == dwMatchID));

			if (!bDeleteRef)
				bDeleteRef = (!dwMatchID && !HasTask(pTDIChild->dwTaskRefID)); // undo = false

			if (bDeleteRef)
			{
				VERIFY(DeleteTask(pTDSParent, nChild, bWithUndo));
				bRemoved = TRUE;
			}
		}
	}
	
	return bRemoved;
}

BOOL CToDoCtrlData::RemoveOrphanTaskLocalDependencies(TODOSTRUCTURE* pTDSParent, BOOL bWithUndo, DWORD dwDependID)
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
		if (RemoveOrphanTaskLocalDependencies(pTDSChild, bWithUndo, dwDependID))
			bRemoved = TRUE;
		
		// then child itself
		TODOITEM* pTDIChild = NULL;
		DWORD dwTaskID = pTDSChild->GetTaskID();
		
		GET_TDI(dwTaskID, pTDIChild, FALSE);

		if (pTDIChild->HasLocalDependency(dwDependID))
		{
			VERIFY(!bWithUndo || AddUndoElement(TDCUEO_EDIT, dwTaskID));
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
	for (int nAttrib = TDCA_FIRST_ATTRIBUTE; nAttrib <= TDCA_LAST_REALATTRIBUTE; nAttrib++)
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
			
			case TDCA_TIMEESTIMATE:	COPYATTRIB(timeEstimate.dAmount); 
									COPYATTRIB(timeEstimate.nUnits); break;
			case TDCA_TIMESPENT:	COPYATTRIB(timeSpent.dAmount);	
									COPYATTRIB(timeSpent.nUnits); break;
			case TDCA_COST:			COPYATTRIB(cost.dAmount);	
									COPYATTRIB(cost.bIsRate); break;
			
			case TDCA_COMMENTS:		COPYATTRIB(sComments); 
									COPYATTRIB(customComments); 
									COPYATTRIB(cfComments); break;
			
			case TDCA_FILELINK:		COPYATTRIBARR(aFileLinks); break;
			case TDCA_ALLOCTO:		COPYATTRIBARR(aAllocTo); break;
			case TDCA_CATEGORY:		COPYATTRIBARR(aCategories); break;
			case TDCA_TAGS:			COPYATTRIBARR(aTags); break;

			case TDCA_DEPENDENCY:
				if (!pToTDI->aDependencies.MatchAll(pFromTDI->aDependencies)) 
				{ 
					pToTDI->aDependencies.Copy(pFromTDI->aDependencies); 
					nRes = SET_CHANGE; 
				}
				break;

			default:
				ASSERT(0);
			}
		}
	}

	if (mapAttribs.Has(TDCA_CUSTOMATTRIB))
	{
		// Copy those custom attributes that have the 'Inherit' feature enabled
		for (int nDef = 0; nDef < m_aCustomAttribDefs.GetSize(); nDef++)
		{
			const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aCustomAttribDefs.GetData()[nDef];

			if (attribDef.HasFeature(TDCCAF_INHERITPARENTCHANGES))
			{
				TDCCADATA dataFrom, dataTo;
				pFromTDI->GetCustomAttributeValue(attribDef.sUniqueID, dataFrom);
				pToTDI->GetCustomAttributeValue(attribDef.sUniqueID, dataTo);

				if (dataFrom != dataTo)
				{
					pToTDI->SetCustomAttributeValue(attribDef.sUniqueID, dataFrom);
					nRes = SET_CHANGE;
				}
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
		SaveEditUndo(dwTaskID, pTDI, TDCA_ALL);

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
		nRes = SetTaskAllocTo(dwTaskID, CStringArray());
		break;

	case TDCA_CATEGORY:		
		nRes = SetTaskCategories(dwTaskID, CStringArray());
		break;

	case TDCA_TAGS:			
		nRes = SetTaskTags(dwTaskID, CStringArray());
		break;

	case TDCA_DEPENDENCY:	
		nRes = SetTaskDependencies(dwTaskID, CTDCDependencyArray());
		break;

	case TDCA_FILELINK:		
		nRes = SetTaskFileLinks(dwTaskID, CStringArray());
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
		{
			// preserve 'IsRate'
			TDCCOST cost;
			GetTaskCost(dwTaskID, cost);
			
			cost.dAmount = 0.0;
			nRes = SetTaskCost(dwTaskID, cost);
		}
		break;

	case TDCA_COLOR:		
		nRes = SetTaskColor(dwTaskID, 0);
		break;

	case TDCA_RECURRENCE:	
		nRes = SetTaskRecurrence(dwTaskID, TDCRECURRENCE());
		break;
		
	case TDCA_TIMEESTIMATE:		
		{
			// preserve existing units
			TDCTIMEPERIOD time;
			GetTaskTimeEstimate(dwTaskID, time);

			time.dAmount = 0.0;
			nRes = SetTaskTimeEstimate(dwTaskID, time);
		}
		break;

	case TDCA_TIMESPENT:
		{ 
			// preserve existing units
			TDCTIMEPERIOD time;
			GetTaskTimeSpent(dwTaskID, time);

			time.dAmount = 0.0;
			nRes = SetTaskTimeSpent(dwTaskID, time);
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

BOOL CToDoCtrlData::ApplyLastInheritedChangeToSubtasks(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib)
{
	// special case: 
	if (nAttrib == TDCA_ALL)
	{
		POSITION pos = m_mapParentAttribs.GetStartPosition();

		while (pos)
		{
			// FALSE means do not apply if parent is blank
			TDC_ATTRIBUTE nAttrib = m_mapParentAttribs.GetNext(pos);
			
			if (!ApplyLastChangeToSubtasks(dwTaskID, nAttrib, FALSE))
				return FALSE;
		}
	}
	else if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttrib) &&
			WantUpdateInheritedAttibute(TDCA_CUSTOMATTRIB))
	{
		return ApplyLastChangeToSubtasks(dwTaskID, nAttrib);
	}
	else if (WantUpdateInheritedAttibute(nAttrib))
	{
		return ApplyLastChangeToSubtasks(dwTaskID, nAttrib);
	}

	return TRUE; // not an error
}

BOOL CToDoCtrlData::ApplyLastInheritedChangeFromParent(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib)
{
	// Exclude references and undo/redo operations
	if (m_bUndoRedoing || IsTaskReference(dwTaskID))
		return TRUE; // not an error

	// special case: 
	if (nAttrib == TDCA_ALL)
	{
		POSITION pos = m_mapParentAttribs.GetStartPosition();

		while (pos)
		{
			nAttrib = m_mapParentAttribs.GetNext(pos);

			if (nAttrib == TDCA_CUSTOMATTRIB)
			{
				for (int nCust = 0; nCust < m_aCustomAttribDefs.GetSize(); nCust++)
				{
					nAttrib = m_aCustomAttribDefs[nCust].GetAttributeID();

					if (!ApplyLastInheritedChangeFromParent(dwTaskID, nAttrib)) // RECURSIVE CALL
						return FALSE;
				}
			}
			else if (!ApplyLastInheritedChangeFromParent(dwTaskID, nAttrib)) // RECURSIVE CALL
			{
				return FALSE;
			}
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
				return FALSE;
			}

			int nPos = GetTaskPosition(pTDS);
			ASSERT((nPos != -1) && (pTDSParent->GetSubTaskID(nPos) == dwTaskID));

			if (!ApplyLastChangeToSubtask(pTDIParent, pTDSParent, nPos, nAttrib, FALSE))
				return FALSE;
		}
	}

	return TRUE;
}

BOOL CToDoCtrlData::ApplyLastChangeToSubtasks(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, BOOL bIncludeBlank)
{
	// Exclude references
	if (dwTaskID && !IsTaskReference(dwTaskID))
	{
		const TODOITEM* pTDI = NULL;
		const TODOSTRUCTURE* pTDS = NULL;

		if (GetTask(dwTaskID, pTDI, pTDS))
			return ApplyLastChangeToSubtasks(pTDI, pTDS, nAttrib, bIncludeBlank);
		else
			ASSERT(0);
	}

	// else
	return FALSE;
}

BOOL CToDoCtrlData::ApplyLastChangeToSubtasks(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, 
											  TDC_ATTRIBUTE nAttrib, BOOL bIncludeBlank)
{
	ASSERT(m_undo.IsActive());

	// Exclude references
	if (!pTDI || pTDI->dwTaskRefID || !pTDS)
	{
		ASSERT(0);
		return FALSE;
	}
	
	for (int nSubTask = 0; nSubTask < pTDS->GetSubTaskCount(); nSubTask++)
	{
		if (!ApplyLastChangeToSubtask(pTDI, pTDS, nSubTask, nAttrib, bIncludeBlank))
			return FALSE;
	}
	
	return TRUE;
}

BOOL CToDoCtrlData::ApplyLastChangeToSubtask(const TODOITEM* pTDIParent, const TODOSTRUCTURE* pTDSParent, 
											int nChildPos, TDC_ATTRIBUTE nAttribID, BOOL bIncludeBlank)
{
	ASSERT(m_undo.IsActive());

	if (!pTDIParent || !pTDSParent || (nChildPos < 0) || (nChildPos >= pTDSParent->GetSubTaskCount()))
	{
		ASSERT(0);
		return FALSE;
	}

	DWORD dwSubtaskID = pTDSParent->GetSubTaskID(nChildPos);

	// Exclude references
	if (IsTaskReference(dwSubtaskID))
		return TRUE; // not an error

	TODOITEM* pTDIChild = NULL;
	GET_TDI(dwSubtaskID, pTDIChild, FALSE);

	// save undo data
	SaveEditUndo(dwSubtaskID, pTDIChild, nAttribID);

	// apply the change based on nAttrib
	switch (nAttribID)
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

	case TDCA_TIMEESTIMATE:
		if (bIncludeBlank || pTDIParent->timeEstimate.dAmount > 0)
			pTDIChild->timeEstimate = pTDIParent->timeEstimate;
		break;

	case TDCA_TIMESPENT:
		if (bIncludeBlank || pTDIParent->timeSpent.dAmount > 0)
			pTDIChild->timeSpent = pTDIParent->timeSpent;
		break;

	case TDCA_FILELINK:
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
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_DEF_RET(m_aCustomAttribDefs, nAttribID, pDef, FALSE);

			if (pDef->HasFeature(TDCCAF_INHERITPARENTCHANGES))
			{
				if (bIncludeBlank || pTDIParent->HasCustomAttributeValue(pDef->sUniqueID))
				{
					TDCCADATA data;
					pTDIParent->GetCustomAttributeValue(pDef->sUniqueID, data);

					pTDIChild->SetCustomAttributeValue(pDef->sUniqueID, data);
				}
			}
		}
		else
		{
			ASSERT(0);
			return FALSE;
		}
	}

	// and its children too
	return ApplyLastChangeToSubtasks(pTDIChild, pTDSParent->GetSubTask(nChildPos), nAttribID, bIncludeBlank);
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

TDC_SET CToDoCtrlData::SetTaskCommentsType(DWORD dwTaskID, const CONTENTFORMAT& cfComments)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	// test for actual change
	if (pTDI->cfComments.CompareNoCase(cfComments) == 0)
		return SET_NOCHANGE;

	TDC_SET nRes = DoEditTaskAttribute(dwTaskID, pTDI, TDCA_COMMENTS, pTDI->cfComments, cfComments);

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
	const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
	GET_DEF_RET(m_aCustomAttribDefs, sAttribID, pDef, SET_FAILED);


	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	TDCCADATA dataOld;
	pTDI->GetCustomAttributeValue(sAttribID, dataOld);

	CStringArray aOldItems, aNewItems;
	dataOld.AsArray(aOldItems);

	// Mixed state multi-selection lists need special handling
	if (pDef->IsMultiList() && data.HasExtra())
	{
		CStringArray aMatched, aMixed;
		data.AsArrays(aMatched, aMixed);

		// Starting with the 'old' items, we remove any 
		// that are neither 'matched' nor 'mixed', and then
		// add any that matched
		aNewItems.Copy(aOldItems);
		
		int nItem = aNewItems.GetSize();

		while (nItem--)
		{
			if (!Misc::HasT(aNewItems[nItem], aMatched) &&
				!Misc::HasT(aNewItems[nItem], aMixed))
			{
				aNewItems.RemoveAt(nItem);
			}
		}

		Misc::AddUniqueItems(aMatched, aNewItems);
	}
	else
	{
		data.AsArray(aNewItems);
	}
		
	if (!Misc::MatchAll(aOldItems, aNewItems))
	{
		// save undo data
		SaveEditUndo(dwTaskID, pTDI, TDCA_CUSTOMATTRIB);
		
		// make changes
		pTDI->SetCustomAttributeValue(sAttribID, aNewItems);
		pTDI->SetModified();

		// Update subtasks
		ApplyLastInheritedChangeToSubtasks(dwTaskID, pDef->GetAttributeID());
		
		return SET_CHANGE;
	}

	// else
	return SET_NOCHANGE;
}

TDC_SET CToDoCtrlData::SetTaskMetaData(DWORD dwTaskID, const CString& sKey, const CString& sMetaData)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	if (sMetaData != pTDI->GetMetaData(sKey))
	{
		// save undo data
		SaveEditUndo(dwTaskID, pTDI, TDCA_METADATA);
		
		// make changes
		pTDI->SetMetaData(sKey, sMetaData);
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

BOOL CToDoCtrlData::CanEditPriorityRisk(int nValue, int nNoValue, BOOL bOffset)
{
	if (bOffset)
		return ((nValue >= -10) && (nValue <= 10));

	// else
	return (nValue == nNoValue || (nValue >= 0 && nValue <= 10));
}

TDC_SET CToDoCtrlData::SetTaskPriority(DWORD dwTaskID, int nPriority, BOOL bOffset)
{
	if (!CanEditPriorityRisk(nPriority, FM_NOPRIORITY, bOffset))
		return SET_FAILED;
	
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	if (bOffset && (pTDI->nPriority != FM_NOPRIORITY))
	{
		nPriority += pTDI->nPriority;
		nPriority = max(0, min(10, nPriority));
	}
	
	return EditTaskAttributeT(dwTaskID, pTDI, TDCA_PRIORITY, pTDI->nPriority, nPriority);
}

TDC_SET CToDoCtrlData::SetTaskRisk(DWORD dwTaskID, int nRisk, BOOL bOffset)
{
	if (!CanEditPriorityRisk(nRisk, FM_NORISK, bOffset))
		return SET_FAILED;
	
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	if (bOffset && (pTDI->nRisk != FM_NOPRIORITY))
	{
		nRisk += pTDI->nRisk;
		nRisk = max(0, min(10, nRisk));
	}

	return EditTaskAttributeT(dwTaskID, pTDI, TDCA_RISK, pTDI->nRisk, nRisk);
}

// external version
TDC_SET CToDoCtrlData::SetTaskDate(DWORD dwTaskID, TDC_DATE nDate, const COleDateTime& date)
{
	return SetTaskDate(dwTaskID, NULL, nDate, date, TRUE); // Recalc time estimate
}

// internal version
TDC_SET CToDoCtrlData::SetTaskDate(DWORD dwTaskID, TODOITEM* pTDI, TDC_DATE nDate, const COleDateTime& date, BOOL bRecalcTimeEstimate)
{
	if (!pTDI)
		EDIT_GET_TDI(dwTaskID, pTDI);

	// Convert 'end of day' to whole days
	COleDateTime dtDate(date);
	BOOL bDateIsSet = CDateHelper::IsDateSet(dtDate);

	if (bDateIsSet && CDateHelper::IsEndOfDay(dtDate, FALSE))
		dtDate = CDateHelper::GetDateOnly(dtDate);
	
	BOOL bWasDone = pTDI->IsDone();
	
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
			if (HasStyle(TDCS_SYNCTIMEESTIMATESANDDATES) && CalcMissingDueDateFromStart(pTDI))
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
			if (HasStyle(TDCS_SYNCTIMEESTIMATESANDDATES) &&	CalcMissingStartDateFromDue(pTDI))
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
			pTDI->dateDone = dtDate;
			bRecalcTimeEstimate = FALSE;
			break;
			
		case TDCD_DONEDATE:		
			// add date to existing time component unless date is 0.0
			if (!bDateIsSet || !bWasDone)
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
		
		// update dependent dates and time estimates
		if (CDateHelper::IsDateSet(date))
		{
			FixupTaskLocalDependentsDates(dwTaskID, nDate);

			if (bRecalcTimeEstimate)
				RecalcTaskTimeEstimate(dwTaskID, pTDI, nDate);
		}

		// And subtasks
		ApplyLastInheritedChangeToSubtasks(dwTaskID, TDC::MapDateToAttribute(nDate));
		
		return SET_CHANGE;
	}
	return SET_NOCHANGE;
}

BOOL CToDoCtrlData::CanOffsetTaskDate(DWORD dwTaskID, TDC_DATE nDate, int nAmount, TDC_UNITS nUnits, BOOL bFromToday) const
{
	if (!nAmount && !bFromToday)
		return FALSE;

	if (!HasTask(dwTaskID))
		return FALSE;

	switch (nDate)
	{
	case TDCD_START:
	case TDCD_STARTDATE:
	case TDCD_DUE:
	case TDCD_DUEDATE:
	case TDCD_DONE:
	case TDCD_DONEDATE:
		{
			// This fails on hours and minutes
			if (TDC::MapUnitsToDHUnits(nUnits) == DHU_NULL)
				return FALSE;
		}
		break;

	case TDCD_STARTTIME:
		{
			if ((nUnits != TDCU_HOURS) && (nUnits != TDCU_MINS))
				return FALSE;

			nDate = TDCD_START;
		}
		break;

	case TDCD_DUETIME:
		{
			if ((nUnits != TDCU_HOURS) && (nUnits != TDCU_MINS))
				return FALSE;

			nDate = TDCD_DUE;
		}
		break;

	case TDCD_DONETIME:
		{
			if ((nUnits != TDCU_HOURS) && (nUnits != TDCU_MINS))
				return FALSE;

			nDate = TDCD_DONE;
		}
		break;

	default: // All the rest
		return FALSE;
	}

	if (TaskHasDate(dwTaskID, nDate))
		return TRUE;

	// Allow start and due dates to be created by offsetting from today
	if (bFromToday)
	{
		switch (nDate)
		{
		case TDCD_START:
		case TDCD_STARTDATE:
		case TDCD_DUE:
		case TDCD_DUEDATE:
			return TRUE;
		}
	}

	// all else
	return FALSE;
}

TDC_SET CToDoCtrlData::OffsetTaskDate(DWORD dwTaskID, TDC_DATE nDate, int nAmount, TDC_UNITS nUnits, DWORD dwFlags)
{
	BOOL bFitToRecurringScheme = Misc::HasFlag(dwFlags, TDCOTD_FITTORECURRINGSCHEME);
	BOOL bAndSubtasks = Misc::HasFlag(dwFlags, TDCOTD_OFFSETSUBTASKS);
	BOOL bFromToday = Misc::HasFlag(dwFlags, TDCOTD_OFFSETFROMTODAY);

	TDC_SET nRes = SET_NOCHANGE;

	if (CanOffsetTaskDate(dwTaskID, nDate, nAmount, nUnits, bFromToday))
	{
		TODOITEM* pTDI = NULL;
		EDIT_GET_TDI(dwTaskID, pTDI);

		COleDateTime date = (bFromToday ? CDateHelper::GetDate(DHD_TODAY) : pTDI->GetDate(nDate));
		BOOL bModTimeOnly = ((nUnits == TDCU_HOURS) || (nUnits == TDCU_MINS));

		if (nAmount != 0)
		{
			if (bModTimeOnly)
			{
				// Modify time only
				ASSERT(date.m_dt < 1.0);

				switch (TDC::MapUnitsToTHUnits(nUnits))
				{
				case THU_HOURS:
					date.m_dt += (nAmount / 24.0);
					break;

				case THU_MINS:
					date.m_dt += (nAmount / (24.0 * 60));
					break;

				default:
					ASSERT(0);
				}
			}
			else // Modify date AND time
			{
				VERIFY(CDateHelper().OffsetDate(date, nAmount, TDC::MapUnitsToDHUnits(nUnits)));
			}
		}

		// Special case: Task is recurring and the date was changed -> must fall on a valid date
		if (bFitToRecurringScheme && pTDI->IsRecurring() && !bModTimeOnly)
		{
			pTDI->trRecurrence.FitDayToScheme(date);
		}

		nRes = SetTaskDate(dwTaskID, pTDI, nDate, date, TRUE); // Recalc time estimate
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
			
			if (OffsetTaskDate(dwChildID, nDate, nAmount, nUnits, dwFlags) == SET_CHANGE) // RECURSIVE CALL
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
	if (!COleDateTimeRange::IsValid(pTDI->dateStart, pTDI->dateDue))
	{
		ASSERT(0);
		return SET_FAILED;
	}
	else if (dtNewStart == pTDI->dateStart)
	{
		ASSERT(0);
		return SET_NOCHANGE;
	}

	// Ignore tasks with dependencies where their dates 
	// are automatically calculated
	if (pTDI->aDependencies.GetSize() && HasStyle(TDCS_AUTOADJUSTDEPENDENCYDATES))
	{
		return SET_NOCHANGE;
	}
	
	// recalc due date
	COleDateTime dtStart(dtNewStart); // may get modified
	COleDateTime dtNewDue = CalcNewDueDate(pTDI->dateStart, pTDI->dateDue, pTDI->timeEstimate.nUnits, dtStart);

	// FALSE -> don't recalc time estimate until due date is set
	TDC_SET nRes = SetTaskDate(dwTaskID, pTDI, TDCD_START, dtStart, FALSE);
	ASSERT(nRes != SET_FAILED);

	if (nRes == SET_CHANGE)
		SetTaskDate(dwTaskID, pTDI, TDCD_DUE, dtNewDue, TRUE); // Recalc time estimate

	return nRes;
}

COleDateTime CToDoCtrlData::CalcNewDueDate(const COleDateTime& dtCurStart, const COleDateTime& dtCurDue, TDC_UNITS nUnits, COleDateTime& dtNewStart)
{
	double dSimpleDuration = CalcDuration(dtCurStart, dtCurDue, TDCU_DAYS);
	ASSERT(dSimpleDuration > 0.0);

	double dRealDuration = CalcDuration(dtCurStart, dtCurDue, nUnits);

	if (dRealDuration == 0.0)
	{
		// If the real duration is zero then it means that the task
		// falls between the end of one weekday and the start of the next
		// which means that the user has performed an action to avoid our
		// checks, so we fall back on the simple duration instead
		ASSERT((nUnits == TDCU_MINS) ||
				(nUnits == TDCU_HOURS) ||
				(nUnits == TDCU_WEEKDAYS) ||
				(nUnits == TDCU_WEEKS));

		ASSERT(CDateHelper().WorkingWeek().HasWeekend() ||
				(CDateHelper().WorkingDay().GetLengthInHours(TRUE) < 24));

		// Recalculate the simple duration in weekday-hours because this
		// seems most likely to produce a coherent outcome ie. Avoiding 
		// unit-mashing weirdness
		int nWholeDays = (int)dSimpleDuration;
		double dRemainingTimeInHours = CTimeHelper::RoundHoursToNearestSecond((dSimpleDuration - nWholeDays) * 24);

		double dDurationInWeekdayHours = ((nWholeDays * CDateHelper().WorkingDay().GetLengthInHours()) + dRemainingTimeInHours);

		return AddDuration(dtNewStart, dDurationInWeekdayHours, TDCU_HOURS, TRUE);
	}

	// Tasks whose current and new dates fall wholly within a single day 
	// and where at least one of dates has a time component are kept simple
	if (CDateHelper::DateHasTime(dtCurStart) || CDateHelper::DateHasTime(dtCurDue))
	{
		COleDateTime dtSimpleDue = AddDuration(dtNewStart, dSimpleDuration, TDCU_DAYS, FALSE); // Does not update dtNewStart

		if (CDateHelper::IsSameDay(dtCurStart, dtCurDue) &&
			CDateHelper::IsSameDay(dtNewStart, dtSimpleDue))
		{
			return AddDuration(dtNewStart, dSimpleDuration, TDCU_DAYS, TRUE);
		}
	}

	// else
	return AddDuration(dtNewStart, dRealDuration, nUnits, TRUE);
}

TDC_SET CToDoCtrlData::InitMissingTaskDate(DWORD dwTaskID, TDC_DATE nDate, const COleDateTime& date)
{
	if (TaskHasDate(dwTaskID, nDate))
		return SET_NOCHANGE;

	return SetTaskDate(dwTaskID, NULL, nDate, date, TRUE); // Recalc time estimate
}

TDC_SET CToDoCtrlData::SetTaskPercent(DWORD dwTaskID, int nPercent)
{
	if ((nPercent < 0) || (nPercent > 100))
		return SET_FAILED;
	
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	return EditTaskAttributeT(dwTaskID, pTDI, TDCA_PERCENT, pTDI->nPercentDone, nPercent);
}

TDC_SET CToDoCtrlData::SetTaskCost(DWORD dwTaskID, const TDCCOST& cost, BOOL bOffset)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	if (cost == pTDI->cost)
		return SET_NOCHANGE;

	// save undo data
	SaveEditUndo(dwTaskID, pTDI, TDCA_COST);

	// make the change
	if (bOffset)
		pTDI->cost.AddCost(cost);
	else
		pTDI->cost = cost;

	pTDI->SetModified();

	// update subtasks
	ApplyLastInheritedChangeToSubtasks(dwTaskID, TDCA_COST);

	return SET_CHANGE;
}

TDC_SET CToDoCtrlData::SetTaskTimeEstimate(DWORD dwTaskID, const TDCTIMEPERIOD& timeEst, BOOL bOffset)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	TDCTIMEPERIOD newEst(timeEst);
	
	if (bOffset)
		newEst.AddTime(pTDI->timeEstimate);

	TDC_SET nRes = EditTaskTimeAttribute(dwTaskID, pTDI, TDCA_TIMEESTIMATE, pTDI->timeEstimate, newEst);

	if ((nRes == SET_CHANGE) && HasStyle(TDCS_SYNCTIMEESTIMATESANDDATES))
	{
		if ((newEst.dAmount != 0) && (pTDI->HasStart() || pTDI->HasDue()))
		{
			// Make sure the task has a start date
			CalcMissingStartDateFromDue(pTDI);

			COleDateTime dtNewDue = AddDuration(pTDI->dateStart, newEst.dAmount, newEst.nUnits, FALSE); // Don't modify start date

			// FALSE = don't recalc time estimate
			SetTaskDate(dwTaskID, pTDI, TDCD_DUE, dtNewDue, FALSE); 
		}
	}

	return nRes;
}

TDC_SET CToDoCtrlData::SetTaskTimeSpent(DWORD dwTaskID, const TDCTIMEPERIOD& timeSpent, BOOL bOffset)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	TDCTIMEPERIOD newSpent(bOffset ? pTDI->timeSpent : timeSpent);

	if (bOffset)
		newSpent.AddTime(timeSpent);
	
	return EditTaskTimeAttribute(dwTaskID, pTDI, TDCA_TIMESPENT, pTDI->timeSpent, newSpent);
}

BOOL CToDoCtrlData::CalcMissingStartDateFromDue(TODOITEM* pTDI) const
{
	if (pTDI->HasStart() || !pTDI->HasDue() || (pTDI->timeEstimate.dAmount <= 0.0))
		return FALSE;

	// Subtract time estimate from due date
	pTDI->dateStart = AddDuration(pTDI->dateDue, -pTDI->timeEstimate.dAmount, pTDI->timeEstimate.nUnits, FALSE); // don't modify due date

	return TRUE;
}

BOOL CToDoCtrlData::CalcMissingDueDateFromStart(TODOITEM* pTDI) const
{
	if (!pTDI->HasStart() || pTDI->HasDue() || (pTDI->timeEstimate.dAmount <= 0.0))
		return FALSE;

	// Add time estimate to start date
	COleDateTime dtStart(pTDI->dateStart);
	pTDI->dateDue = AddDuration(pTDI->dateStart, pTDI->timeEstimate.dAmount, pTDI->timeEstimate.nUnits, FALSE); // don't modify start date

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
				
				if (pTDI->HasStart() && pTDI->HasDue()) // both exist
				{
					// End date must be greater then start date
					if (!COleDateTimeRange::IsValid(pTDI->dateStart, pTDI->dateDue))
					{
						ASSERT(0);
						break;
					}

					TDCTIMEPERIOD time = pTDI->timeEstimate;
					time.dAmount = CalcDuration(pTDI->dateStart, pTDI->dateDue, time.nUnits);

					return EditTaskTimeAttribute(dwTaskID, pTDI, TDCA_TIMEESTIMATE, pTDI->timeEstimate, time);
				}
			}
			break;
		}
	}

	// else
	return SET_NOCHANGE;
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
	case TDCA_FILELINK:		return SetTaskFileLinks(dwTaskID, aItems, bAppend);
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
	case TDCA_FILELINK:		return GetTaskFileLinks(dwTaskID, aItems);
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

	CString sPrevStatus = pTDI->sStatus;
	
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

TDC_SET CToDoCtrlData::SetTaskDependencies(DWORD dwTaskID, const CTDCDependencyArray& aDepends, BOOL bAppend)
{
	// weed out 'unknown' tasks, dependencies on self and own parent tasks
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDS(dwTaskID, pTDS, SET_FAILED);

	CTDCDependencyArray aWeeded;
	aWeeded.Copy(aDepends);

	TDC_SET nRes = SET_NOCHANGE;
	int nDepend = aDepends.GetSize();

	while (nDepend--)
	{
		const TDCDEPENDENCY& depend = aWeeded[nDepend];

		if (!depend.dwTaskID)
		{
			aWeeded.RemoveAt(nDepend);
		}
		else if (depend.IsLocal())
		{
			if (!HasTask(depend.dwTaskID)) // check for existence
			{
				aWeeded.RemoveAt(nDepend);
			}
			else if ((depend.dwTaskID == dwTaskID) || IsReferenceToTask(depend.dwTaskID, dwTaskID))
			{
				aWeeded.RemoveAt(nDepend);
			}
			else // check for parent
			{
				while (pTDS)
				{
					if (depend.dwTaskID == pTDS->GetParentTaskID())
					{
						aWeeded.RemoveAt(nDepend);
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

	// test for actual change
	if (bAppend)
	{
		aWeeded.Remove(pTDI->aDependencies);

		if (aWeeded.GetSize() == 0)
			return SET_NOCHANGE;
	}
	else if (pTDI->aDependencies.MatchAll(aWeeded))
	{
		return SET_NOCHANGE;
	}
	
	// save undo data
	SaveEditUndo(dwTaskID, pTDI, TDCA_DEPENDENCY);

	// make the change
	if (bAppend)
		VERIFY(pTDI->aDependencies.Append(aWeeded));
	else
		pTDI->aDependencies.Copy(aWeeded);

	pTDI->SetModified();

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

	return SET_CHANGE;
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
											 TDCTIMEPERIOD& time, const TDCTIMEPERIOD& newTime)
{
	ASSERT(dwTaskID);
	ASSERT(pTDI);
	ASSERT((nAttrib == TDCA_TIMEESTIMATE) || (nAttrib == TDCA_TIMESPENT));
	
	// test for actual change
	if (time == newTime)
		return SET_NOCHANGE;
	
	// save undo data
	SaveEditUndo(dwTaskID, pTDI, nAttrib);
	
	// make the change
	time = newTime;
	pTDI->SetModified();
				
	// Update subtasks
	ApplyLastInheritedChangeToSubtasks(dwTaskID, nAttrib);
	
	return SET_CHANGE;
}

TDC_SET CToDoCtrlData::SetTaskFileLinks(DWORD dwTaskID, const CStringArray& aFileLinks, BOOL bAppend)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);
	
	return EditTaskArrayAttribute(dwTaskID, pTDI, TDCA_FILELINK, pTDI->aFileLinks, aFileLinks, bAppend, TRUE);
}

BOOL CToDoCtrlData::BeginNewUndoAction(TDC_UNDOACTIONTYPE nType)
{
	return m_undo.BeginNewAction(nType);
}

BOOL CToDoCtrlData::EndCurrentUndoAction()
{
	return m_undo.EndCurrentAction();
}

BOOL CToDoCtrlData::ExtendLastUndoAction(TDC_UNDOACTIONTYPE nType)
{
	return m_undo.ExtendLastAction(nType);
}

BOOL CToDoCtrlData::AddUndoElement(TDC_UNDOELMOP nOp, DWORD dwTaskID, DWORD dwParentID, DWORD dwPrevSiblingID, WORD wFlags)
{
	if (!m_undo.IsActive())
	{
		ASSERT(0);
		return FALSE;
	}
	
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

TDC_UNDOACTIONTYPE CToDoCtrlData::GetLastUndoActionType(BOOL bUndo) const
{
	return (bUndo ? m_undo.GetLastUndoType() : m_undo.GetLastRedoType());
}

BOOL CToDoCtrlData::UndoLastAction(BOOL bUndo, CArrayUndoElements& aElms)
{
	CAutoFlag af(m_bUndoRedoing, TRUE);

	aElms.RemoveAll();
	TDCUNDOACTION* pAction = bUndo ? m_undo.UndoLastAction() : m_undo.RedoLastAction();
	
	if (!pAction)
		return FALSE;
	
	int nNumElm = pAction->aElements.GetSize();

	// copy the structure because we're going to be changing it and we need 
	// to be able to lookup the original previous sibling IDs for undo info
	CToDoCtrlDataStructure tdsCopy(m_struct);
	
	if (bUndo)
	{
		// If we are undoing then we need to undo in the reverse order
		// of the initial edits.
		// Unless it was a move which needs special handling because the
		// undoing of the moves MUST occur in forward-order
		if (pAction->nType == TDCUAT_MOVE)
		{
			// Do the undo in 2 passes:
			// 1. The 'non-move' elements in reverse order
			int nElm = nNumElm;

			while (nElm--)
			{
				TDCUNDOELEMENT& elm = pAction->aElements[nElm];

				if ((elm.nOp != TDCUEO_MOVE) && !ProcessUndoElement(bUndo, elm, aElms, tdsCopy))
					return FALSE;
			}

			// 2. The 'move' elements in forward order
			for (nElm = 0; nElm < nNumElm; nElm++)
			{
				TDCUNDOELEMENT& elm = pAction->aElements[nElm];

				if ((elm.nOp == TDCUEO_MOVE) && !ProcessUndoElement(bUndo, elm, aElms, tdsCopy))
					return FALSE;
			}
		}
		else
		{
			// All operations in reverse order
			int nElm = nNumElm;

			while (nElm--)
			{
				TDCUNDOELEMENT& elm = pAction->aElements[nElm];

				if (!ProcessUndoElement(bUndo, elm, aElms, tdsCopy))
					return FALSE;
			}
		}
	}
	else
	{
		// All operations in forward order
		for (int nElm = 0; nElm < nNumElm; nElm++)
		{
			TDCUNDOELEMENT& elm = pAction->aElements[nElm];
	
			if (!ProcessUndoElement(bUndo, elm, aElms, tdsCopy))
				return FALSE;
		}
	}
	
	return TRUE;
}

BOOL CToDoCtrlData::ProcessUndoElement(BOOL bUndo, TDCUNDOELEMENT& srcElement, CArrayUndoElements& aReturnedElms, const CToDoCtrlDataStructure& tdsCopy)
{
	if (srcElement.nOp == TDCUEO_EDIT)
	{
		TODOITEM* pTDI = NULL;
		GET_TDI(srcElement.dwTaskID, pTDI, FALSE);

		// copy current task state so we can update redo info
		TODOITEM tdiRedo = *pTDI;
		*pTDI = srcElement.tdi;
		srcElement.tdi = tdiRedo;

		// no need to return this item nothing to be done
	}
	else if ((srcElement.nOp == TDCUEO_ADD && bUndo) || (srcElement.nOp == TDCUEO_DELETE && !bUndo))
	{
		// this is effectively a delete so make the returned elem that way
		TDCUNDOELEMENT elmRet(TDCUEO_DELETE, srcElement.dwTaskID);
		aReturnedElms.Add(elmRet);

		DeleteTask(srcElement.dwTaskID, FALSE); // FALSE == no undo
	}
	else if ((srcElement.nOp == TDCUEO_DELETE && bUndo) || (srcElement.nOp == TDCUEO_ADD && !bUndo))
	{
		// this is effectively an add so make the returned elem that way
		TDCUNDOELEMENT elmRet(TDCUEO_ADD, srcElement.dwTaskID, srcElement.dwParentID, srcElement.dwPrevSiblingID);
		aReturnedElms.Add(elmRet);

		// restore task
		if (HasTask(srcElement.dwTaskID))
		{
			// Should not exist
			ASSERT(0);
		}
		else
		{
			TODOITEM* pTDI = NewTask(srcElement.tdi);
			AddTask(srcElement.dwTaskID, pTDI, srcElement.dwParentID, srcElement.dwPrevSiblingID);
		}
	}
	else if (srcElement.nOp == TDCUEO_MOVE)
	{
		// We DON'T want the TRUE task
		TODOITEM* pTDI = GetTask(srcElement.dwTaskID, FALSE);

		if (!pTDI)
		{
			ASSERT(0);
			return FALSE;
		}

		TDCUNDOELEMENT elmRet(TDCUEO_MOVE, srcElement.dwTaskID, srcElement.dwParentID, srcElement.dwPrevSiblingID, 0, pTDI);
		aReturnedElms.Add(elmRet);

		MoveTask(srcElement.dwTaskID, srcElement.dwParentID, srcElement.dwPrevSiblingID);

		// adjust undo element so these changes can be undone
		srcElement.dwParentID = tdsCopy.GetParentTaskID(srcElement.dwTaskID);
		srcElement.dwPrevSiblingID = tdsCopy.GetPreviousTaskID(srcElement.dwTaskID);

		TODOITEM tdiRedo = *pTDI;
		*pTDI = srcElement.tdi;
		srcElement.tdi = tdiRedo;
	}
	else
	{
		// What else could it be?
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

BOOL CToDoCtrlData::CanUndoLastAction(BOOL bUndo) const
{
	return bUndo ? m_undo.CanUndo() : m_undo.CanRedo();
}

BOOL CToDoCtrlData::IsValidMoveDestination(DWORD dwTaskID, DWORD dwDestParentID) const
{
	// Sanity checks
	if (!dwTaskID || (dwDestParentID && !HasTask(dwDestParentID)))
	{
		ASSERT(0);
		return FALSE;
	}

	// Parent can always be the root
	if (!dwDestParentID)
		return TRUE;

	// Parent can't be the task being moved
	if (dwDestParentID == dwTaskID)
		return FALSE;

	// Parent can't be a child of the task being moved
	const TODOSTRUCTURE* pTDSDestParent = LocateTask(dwDestParentID);
	ASSERT(pTDSDestParent);

	return (pTDSDestParent && !pTDSDestParent->HasParent(dwTaskID, FALSE));
}

BOOL CToDoCtrlData::IsValidMoveDestination(const CDWordArray& aTaskIDs, DWORD dwDestParentID) const
{
	// Sanity checks
	if (!aTaskIDs.GetSize() || (dwDestParentID && !HasTask(dwDestParentID)))
	{
		ASSERT(0);
		return FALSE;
	}

	// Parent can always be the root
	if (!dwDestParentID)
		return TRUE;

	const TODOSTRUCTURE* pTDSDestParent = LocateTask(dwDestParentID);
	ASSERT(pTDSDestParent);

	if (pTDSDestParent)
	{
		int nID = aTaskIDs.GetSize();

		while (nID--)
		{
			DWORD dwTaskID = aTaskIDs[nID];

			// Parent can't be one of the tasks being moved
			if (dwDestParentID == dwTaskID)
				return FALSE;

			// Or a child of one of the tasks being moved
			if (pTDSDestParent->HasParent(dwTaskID, FALSE))
				return FALSE;
		}
	}

	return TRUE;
}

BOOL CToDoCtrlData::MoveTask(DWORD dwTaskID, DWORD dwDestParentID, DWORD dwDestPrevSiblingID)
{
	// Sanity check
	if (!IsValidMoveDestination(dwTaskID, dwDestParentID))
	{
		ASSERT(0);
		return FALSE;
	}

	// get source location
	TODOSTRUCTURE* pTDSSrcParent = NULL;
	int nSrcPos = 0;
	
	if (!LocateTask(dwTaskID, pTDSSrcParent, nSrcPos))
	{
		ASSERT(0);
		return FALSE;
	}
	
	DWORD dwSrcPrevSiblingID = pTDSSrcParent->GetPreviousSubTaskID(nSrcPos);
	
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

	return (MoveTask(pTDSSrcParent, nSrcPos, dwSrcPrevSiblingID, pTDSDestParent, nDestPos) != -1);
}

// Internal: Assume that move destination has already been validated
int CToDoCtrlData::MoveTask(TODOSTRUCTURE* pTDSSrcParent, int nSrcPos, DWORD dwSrcPrevSiblingID,
							TODOSTRUCTURE* pTDSDestParent, int nDestPos)
{
	// check if there's anything to do
	if (!m_struct.CanMoveSubTask(pTDSSrcParent, nSrcPos, pTDSDestParent, nDestPos))
		return -1;
	
	// save undo info
	DWORD dwTaskID = pTDSSrcParent->GetSubTaskID(nSrcPos);
	DWORD dwSrcParentID = pTDSSrcParent->GetTaskID();

	VERIFY(m_bUndoRedoing || AddUndoElement(TDCUEO_MOVE, dwTaskID, dwSrcParentID, dwSrcPrevSiblingID));
	
	int nPos = m_struct.MoveSubTask(pTDSSrcParent, nSrcPos, pTDSDestParent, nDestPos);
	ASSERT(nPos != -1);

	// sanity check
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
	switch (aTaskIDs.GetSize())
	{
		case 0: return FALSE;
		case 1: return MoveTask(aTaskIDs[0], dwDestParentID, dwDestPrevSiblingID);
	}
	
	// Sanity check
	if (!IsValidMoveDestination(aTaskIDs, dwDestParentID))
	{
		ASSERT(0);
		return FALSE;
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
			DWORD dwSrcPrevSiblingID = pTDSDummy->GetPreviousSubTaskID(nDummyPos);
			nDestPos = MoveTask(pTDSSrcParent, nSrcPos, dwSrcPrevSiblingID, pTDSDestParent, nDestPos);
			
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
	return TaskHasCompletedSubtasks(LocateTask(dwTaskID));
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
	return TaskHasIncompleteSubtasks(LocateTask(dwTaskID), bExcludeRecurring);
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

BOOL CToDoCtrlData::TaskHasRecurringSubtasks(DWORD dwTaskID) const
{
	return TaskHasRecurringSubtasks(LocateTask(dwTaskID));
}

BOOL CToDoCtrlData::TaskHasRecurringSubtasks(const TODOSTRUCTURE* pTDS) const
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
		
		if (IsTaskRecurring(pTDSChild->GetTaskID()))
			return TRUE;
		
		// Grandchildren
		if (TaskHasRecurringSubtasks(pTDSChild))
			return TRUE;
	}
	
	// else
	return FALSE;
}

void CToDoCtrlData::SetDefaultCommentsFormat(const CString& format) 
{ 
	m_cfDefault = format; 
}

void CToDoCtrlData::SetDefaultTimeUnits(TDC_UNITS nTimeEstUnits, TDC_UNITS nTimeSpentUnits)
{
	m_nDefTimeEstUnits = nTimeEstUnits;
	m_nDefTimeSpentUnits = nTimeSpentUnits;
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
	// Not trackable if complete
	if (IsTaskDone(dwTaskID))
		return FALSE;

	// Not trackable if task is parent
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDS(dwTaskID, pTDS, FALSE);
	
	if (pTDS->HasSubTasks() && !HasStyle(TDCS_ALLOWPARENTTIMETRACKING))
		return FALSE;

	// Not trackable if any of its parents are complete
	pTDS = pTDS->GetParentTask();

	while (pTDS && !pTDS->IsRoot())
	{
		if (IsTaskDone(pTDS->GetTaskID()))
			return FALSE;

		pTDS = pTDS->GetParentTask();
	}

	return TRUE;
}

BOOL CToDoCtrlData::IsTaskParent(DWORD dwTaskID) const
{
	const TODOSTRUCTURE* pTDS = NULL;
	GET_TDS(dwTaskID, pTDS, FALSE);
	
	return pTDS->HasSubTasks();
}

BOOL CToDoCtrlData::CalcTaskDependencyStartDate(DWORD dwTaskID, const TDCDEPENDENCY& depend, 
												   TDC_DATE nDate, COleDateTime& dtNewStart) const
{
	CDateHelper::ClearDate(dtNewStart);

	// if we're already completed then no adjustment is necessary
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);

	if (pTDI->IsDone())
		return FALSE;

	const TODOITEM* pTDIDepends = NULL;
	DWORD dwDependencyID = depend.dwTaskID;

	GET_TDI(dwDependencyID, pTDIDepends, FALSE);

	switch (nDate)
	{
	case TDCD_DONE:
		if (pTDIDepends->IsDone())
		{
			// New start date matched the preceding task's end date 
			// Ignore completion time if due date has no time
			if (pTDIDepends->HasDue() && !pTDIDepends->HasDueTime())
				dtNewStart = CDateHelper::GetDateOnly(pTDIDepends->dateDone);
			else
				dtNewStart = pTDIDepends->dateDone;

			// if we're on a day/night boundary move to next day
			if (CDateHelper::IsEndOfDay(dtNewStart, TRUE))
				dtNewStart += 1.0;
		}
		break;

	case TDCD_DUE:
	case TDCD_DUEDATE:
	case TDCD_DUETIME:
		// or its due date and it's not yet finished
		if (!pTDIDepends->IsDone() && pTDIDepends->HasDue())
		{	
			// New start date matches the preceding task's due date 
			dtNewStart = pTDIDepends->dateDue;

			// if we're on a day/night boundary move to next day
			if (CDateHelper::IsEndOfDay(dtNewStart, TRUE))
				dtNewStart += 1.0;
		}
		break;

	case TDCD_START:
	case TDCD_STARTDATE:
	case TDCD_STARTTIME:
		// start is not affected by changes to dependency start dates
 		break;

	default:
		ASSERT(0);
		break;
	}
	
	if (CDateHelper::IsDateSet(dtNewStart))
	{
		DH_UNITS nDHUnits = DHU_WEEKDAYS;

		switch (pTDI->timeEstimate.nUnits)
		{
		case TDCU_DAYS:
		case TDCU_MONTHS:
		case TDCU_YEARS:
			nDHUnits = DHU_DAYS;
			break;
		}

		if (nDHUnits = DHU_WEEKDAYS)
			CWorkingWeek().MakeWeekday(dtNewStart);

		// Add lead-in time
		if (depend.nDaysLeadIn)
			VERIFY(CDateHelper().OffsetDate(dtNewStart, depend.nDaysLeadIn, nDHUnits));

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
	COleDateTime dtNewDue;

	UINT nAdjusted = ADJUSTED_NONE;

	if (pTDI->HasDue() && pTDI->HasStart())
	{
		dtNewDue = CalcNewDueDate(pTDI->dateStart, pTDI->dateDue, pTDI->timeEstimate.nUnits, dtStart);
	}
	else if ((pTDI->timeEstimate.dAmount > 0.0) && HasStyle(TDCS_SYNCTIMEESTIMATESANDDATES))
	{
		dtNewDue = AddDuration(dtStart, pTDI->timeEstimate.dAmount, pTDI->timeEstimate.nUnits, TRUE);
	}

	if (CDateHelper::IsDateSet(dtNewDue) && (dtNewDue != pTDI->dateDue))
	{
		// save undo info
		SaveEditUndo(dwTaskID, pTDI, TDCA_DUEDATE);

		// make the change
		pTDI->dateDue = dtNewDue;
		nAdjusted |= ADJUSTED_DUE;
	}
	
	// save undo info
	SaveEditUndo(dwTaskID, pTDI, TDCA_STARTDATE);

	// always set the start date
	pTDI->dateStart = dtStart;
	nAdjusted |= ADJUSTED_START;

	return nAdjusted;
}

BOOL CToDoCtrlData::CalcTaskDependencyStartDate(DWORD dwTaskID, TDC_DATE nDate, COleDateTime& dtNewStart) const
{
	TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);

	// calculate the latest start date possible for this task's dependencies
	CDateHelper::ClearDate(dtNewStart);

	int nDepend = pTDI->aDependencies.GetSize();

	while (nDepend--)
	{
		const TDCDEPENDENCY& depend = pTDI->aDependencies[nDepend];

		if (depend.IsLocal())
		{
			COleDateTime dtStart;

			if (CalcTaskDependencyStartDate(dwTaskID, depend, nDate, dtStart))
				VERIFY(CDateHelper::Max(dtNewStart, dtStart));
		}
	}

	return CDateHelper::IsDateSet(dtNewStart);
}

UINT CToDoCtrlData::UpdateTaskLocalDependencyDates(DWORD dwTaskID, TDC_DATE nDate)
{
	// calculate the latest start date possible for this task's dependencies
	COleDateTime dtNewStart;

	if (CalcTaskDependencyStartDate(dwTaskID, nDate, dtNewStart))
		return SetNewTaskDependencyStartDate(dwTaskID, dtNewStart);

	return ADJUSTED_NONE;
}

COleDateTime CToDoCtrlData::AddDuration(COleDateTime& dateStart, double dDuration, TDC_UNITS nUnits, BOOL bAllowUpdateStart)
{
	if (!CDateHelper::IsDateSet(dateStart) || (dDuration == 0.0) || (nUnits == TDCU_NULL))
	{
		ASSERT(0);
		return dateStart;
	}

	COleDateTime dtOrgStart(dateStart);
	COleDateTime dateEnd(dateStart);
	
	switch (nUnits)
	{
	case TDCU_DAYS:
	case TDCU_MONTHS:
	case TDCU_YEARS:
		{
			// work in days
			if (nUnits != TDCU_DAYS)
			{
				CTwentyFourSevenWeek week;

				dDuration = CTimeHelper(week).Convert(dDuration, TDC::MapUnitsToTHUnits(nUnits), THU_DAYS);
				nUnits = TDCU_DAYS;
			}

			dateEnd.m_dt += dDuration;

			// If date falls on the beginning of a day, move to end of previous day
			if (dDuration > 0.0)
			{
				if (!CDateHelper::DateHasTime(dateEnd))
					dateEnd.m_dt--;
			}
			else
			{
				// End date comes before start date, so 'dateStart' is logically the end date
				ASSERT(dateEnd < dateStart);

				if (!CDateHelper::DateHasTime(dateStart))
					dateEnd.m_dt++;
			}
		}
		break;

	case TDCU_MINS:
	case TDCU_HOURS:
	case TDCU_WEEKDAYS:
	case TDCU_WEEKS:
		{
			// Note: 
			CWorkingWeek week;
			dateEnd = week.AddDuration(dateStart, dDuration, TDC::MapUnitsToWWUnits(nUnits));

			// If date falls on the beginning of a day, move to end of previous day
			if (dDuration > 0.0)
			{
				if (week.WorkingDay().IsEndOfDay(dateEnd))
					dateEnd = CDateHelper::GetDateOnly(dateEnd);
			}
			else
			{
				// End date comes before start date, so 'dateStart' is logically the end date
				ASSERT(dateEnd < dateStart);

				if (week.WorkingDay().IsEndOfDay(dateStart))
					dateEnd = (CDateHelper::GetDateOnly(dateEnd).m_dt + 1.0);
			}
		}
		break;
	}

	// sanity check
#ifdef _DEBUG
	double dCheck = 0.0;

	if (dDuration > 0.0)
		dCheck = CalcDuration(dateStart, dateEnd, nUnits);
	else
		dCheck = -CalcDuration(dateEnd, dateStart, nUnits);

	ASSERT(fabs(dCheck - dDuration) < 1e-3);
#endif

	if (!bAllowUpdateStart)
		dateStart = dtOrgStart;
	
	return dateEnd;
}

double CToDoCtrlData::CalcDuration(const COleDateTime& dateStart, const COleDateTime& dateDue, TDC_UNITS nUnits)
{
	// Sanity check
	if (!COleDateTimeRange::IsValid(dateStart, dateDue))
	{
		ASSERT(0);
		return 0.0;
	}

	// Handle due date 'end of day'
	COleDateTime dateEnd(dateDue);

	if (CDateHelper::IsEndOfDay(dateEnd, TRUE))
		dateEnd = CDateHelper::GetStartOfNextDay(dateEnd);
	
	switch (nUnits)
	{
	case TDCU_DAYS:
		return (dateEnd.m_dt - dateStart.m_dt);

	case TDCU_MONTHS:
	case TDCU_YEARS:
		{
			CTwentyFourSevenWeek week;
			double dDuration = (dateEnd.m_dt - dateStart.m_dt); // in days

			return CTimeHelper(week).Convert(dDuration, THU_DAYS, TDC::MapUnitsToTHUnits(nUnits));
		}
		break;

	case TDCU_MINS:
	case TDCU_HOURS:
	case TDCU_WEEKDAYS:
	case TDCU_WEEKS:
		return CWorkingWeek().CalculateDuration(dateStart, dateEnd, TDC::MapUnitsToWWUnits(nUnits));
	}

	ASSERT(0);
	return 0.0;
}

void CToDoCtrlData::FixupTaskLocalDependentsDates(DWORD dwTaskID, TDC_DATE nDate)
{
	if (!HasStyle(TDCS_AUTOADJUSTDEPENDENCYDATES))
		return;
	
	// who is dependent on us -> GetTaskDependents
	CDWordArray aDependents;
	int nDepend = GetTaskLocalDependents(dwTaskID, aDependents, FALSE);

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
			FixupTaskLocalDependentsDates(dwIDDependent, TDCD_DUE); // RECURSIVE CALL
		}
	}
}

TDC_SET CToDoCtrlData::CopyTaskAttributeValues(DWORD dwTaskID, TDC_ATTRIBUTE nFromAttrib, TDC_ATTRIBUTE nToAttrib)
{
	TDCCADATA data;

	if (!GetTaskAttributeValues(dwTaskID, nFromAttrib, data))
		return SET_FAILED;

	// else
	return SetTaskAttributeValues(dwTaskID, nToAttrib, data);
}

TDC_SET CToDoCtrlData::CopyTaskAttributeValues(DWORD dwTaskID, TDC_ATTRIBUTE nFromAttrib, const CString& sToCustomAttribID)
{
	TDCCADATA data;

	if (!GetTaskAttributeValues(dwTaskID, nFromAttrib, data))
		return SET_FAILED;

	// else
	return SetTaskCustomAttributeData(dwTaskID, sToCustomAttribID, data);
}

BOOL CToDoCtrlData::GetTaskAttributeValues(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, TDCCADATA& data) const
{
	const TODOITEM* pTDI = NULL;
	GET_TDI(dwTaskID, pTDI, FALSE);

	return pTDI->GetAttributeValues(nAttrib, data);
}

TDC_SET CToDoCtrlData::CopyTaskAttributeValues(DWORD dwTaskID, const CString& sFromCustomAttribID, TDC_ATTRIBUTE nToAttrib)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	TDCCADATA data;

	if (!pTDI->GetCustomAttributeValues().Lookup(sFromCustomAttribID, data))
		return SET_FAILED;

	return SetTaskAttributeValues(dwTaskID, nToAttrib, data);
}

TDC_SET CToDoCtrlData::SetTaskAttributeValues(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, const TDCCADATA& data)
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
	case TDCA_PRIORITY:		return SetTaskPriority(dwTaskID, data.AsInteger(), FALSE);
	case TDCA_RISK:			return SetTaskRisk(dwTaskID, data.AsInteger(), FALSE);
	case TDCA_PERCENT:		return SetTaskPercent(dwTaskID, data.AsInteger());
	case TDCA_FLAG:			return SetTaskFlag(dwTaskID, data.AsBool());
	case TDCA_LOCK:			return SetTaskLock(dwTaskID, data.AsBool());

	case TDCA_FILELINK:	
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

	case TDCA_TIMEESTIMATE:			
		{
			TDCTIMEPERIOD time;
			
			if (data.AsTimePeriod(time))
				return SetTaskTimeEstimate(dwTaskID, time, FALSE);
		}
		break;

	case TDCA_TIMESPENT:	
		{
			TDCTIMEPERIOD time;

			if (data.AsTimePeriod(time))
				return SetTaskTimeSpent(dwTaskID, time, FALSE);
		}
		break;

	case TDCA_COST:
		{
			TDCCOST cost;
			
			if (data.AsCost(cost))
				return SetTaskCost(dwTaskID, cost, FALSE);
		}
		break;
	}

	return SET_FAILED;
}

TDC_SET CToDoCtrlData::CopyTaskAttributeValues(DWORD dwTaskID, const CString& sFromCustomAttribID, const CString& sToCustomAttribID)
{
	TODOITEM* pTDI = NULL;
	EDIT_GET_TDI(dwTaskID, pTDI);

	TDCCADATA data;

	if (!pTDI->GetCustomAttributeValue(sFromCustomAttribID, data))
		return SET_FAILED;

	return SetTaskCustomAttributeData(dwTaskID, sToCustomAttribID, data);
}

TDC_SET CToDoCtrlData::RenameTasksAttributeValue(const CString& sAttribID, const CString& sFrom, const CString& sTo, BOOL bCaseSensitive, BOOL bWholeWord)
{
	TDC_SET nRes = SET_NOCHANGE;
	TDCCADATA data;

	DWORD dwTaskID = 0;
	TODOITEM* pTDI = NULL;
	POSITION pos = m_items.GetStart();
	
	while (pos && (nRes != SET_FAILED))
	{
		m_items.GetNext(pos, dwTaskID, pTDI);

		if (pTDI->GetCustomAttributeValue(sAttribID, data))
		{
			if (Misc::Find(sFrom, data.AsString(), bCaseSensitive, bWholeWord) != -1)
			{
				// save undo data
				SaveEditUndo(dwTaskID, pTDI, TDCA_CUSTOMATTRIB);

				CString sValue(data.AsString());
				VERIFY(Misc::Replace(sFrom, sTo, sValue, bCaseSensitive, bWholeWord));

				pTDI->SetCustomAttributeValue(sAttribID, sValue);
				pTDI->SetModified();

				nRes = SET_CHANGE;
			}
		}
	}

	return nRes;
}

TDC_SET CToDoCtrlData::RenameTasksAttributeValue(TDC_ATTRIBUTE nAttrib, const CString& sFrom, const CString& sTo, BOOL bCaseSensitive, BOOL bWholeWord)
{
	TDC_SET nRes = SET_NOCHANGE;
	DWORD dwTaskID = 0;
	TODOITEM* pTDI = NULL;
	POSITION pos = m_items.GetStart();
	
	while (pos && (nRes != SET_FAILED))
	{
		m_items.GetNext(pos, dwTaskID, pTDI);

		if (TaskHasAttributeValue(pTDI, nAttrib, sFrom, bCaseSensitive, bWholeWord))
		{
			// save undo data
			SaveEditUndo(dwTaskID, pTDI, nAttrib);

			switch (nAttrib)
			{
			case TDCA_VERSION:		VERIFY(Misc::Replace(sFrom, sTo, pTDI->sVersion, bCaseSensitive, bWholeWord));
			case TDCA_ALLOCBY:		VERIFY(Misc::Replace(sFrom, sTo, pTDI->sAllocBy, bCaseSensitive, bWholeWord));
			case TDCA_EXTERNALID:	VERIFY(Misc::Replace(sFrom, sTo, pTDI->sExternalID, bCaseSensitive, bWholeWord));
			case TDCA_STATUS:		VERIFY(Misc::Replace(sFrom, sTo, pTDI->sStatus, bCaseSensitive, bWholeWord));

			case TDCA_ALLOCTO:		VERIFY(Misc::Replace(sFrom, sTo, pTDI->aAllocTo, bCaseSensitive, bWholeWord));
			case TDCA_CATEGORY:		VERIFY(Misc::Replace(sFrom, sTo, pTDI->aCategories, bCaseSensitive, bWholeWord));
			case TDCA_TAGS:			VERIFY(Misc::Replace(sFrom, sTo, pTDI->aTags, bCaseSensitive, bWholeWord));
			}

			pTDI->SetModified();
			nRes = SET_CHANGE;
		}
	}

	return nRes;
}

BOOL CToDoCtrlData::TaskHasAttributeValue(TODOITEM* pTDI, TDC_ATTRIBUTE nAttrib, const CString& sText, BOOL bCaseSensitive, BOOL bWholeWord)
{
	switch (nAttrib)
	{
	case TDCA_VERSION:		return (Misc::Find(sText, pTDI->sVersion, bCaseSensitive, bWholeWord) != -1);
	case TDCA_ALLOCBY:		return (Misc::Find(sText, pTDI->sAllocBy, bCaseSensitive, bWholeWord) != -1);
	case TDCA_EXTERNALID:	return (Misc::Find(sText, pTDI->sExternalID, bCaseSensitive, bWholeWord) != -1);
	case TDCA_STATUS:		return (Misc::Find(sText, pTDI->sStatus, bCaseSensitive, bWholeWord) != -1);
		
	case TDCA_ALLOCTO:		return Misc::Contains(sText, pTDI->aAllocTo, bCaseSensitive, bWholeWord);
	case TDCA_CATEGORY:		return Misc::Contains(sText, pTDI->aCategories, bCaseSensitive, bWholeWord);		
	case TDCA_TAGS:			return Misc::Contains(sText, pTDI->aTags, bCaseSensitive, bWholeWord);
		
	default:
		ASSERT(0);
	}

	return FALSE;
}

TDC_SET CToDoCtrlData::AdjustNewRecurringTasksDates(DWORD dwPrevTaskID, DWORD dwNewTaskID,
													 const COleDateTime& dtNext, BOOL bDueDate)
{
	TDC_SET nRes = SET_NOCHANGE;

	// we need to move both the due date and the start date forward
	// so we first cache the old dates
	COleDateTime dtStart = GetTaskDate(dwPrevTaskID, TDCD_START);
	COleDateTime dtDue = GetTaskDate(dwPrevTaskID, TDCD_DUE);

	BOOL bHasStart = CDateHelper::IsDateSet(dtStart);
	BOOL bHasDue = CDateHelper::IsDateSet(dtDue);

	BOOL bWantInheritStart = WantUpdateInheritedAttibute(TDCA_STARTDATE);
	BOOL bWantInheritDue = WantUpdateInheritedAttibute(TDCA_DUEDATE);

	if (bDueDate) // dtNext is the new due date
	{
		int nOffsetDays = (bHasDue ? ((int)dtNext - (int)dtDue) : 0);

		if (bWantInheritDue)
		{
			if (SetTaskDate(dwNewTaskID, TDCD_DUE, dtNext) == SET_CHANGE)
			{
				ApplyLastChangeToSubtasks(dwNewTaskID, TDCA_DUEDATE);
				nRes = SET_CHANGE;
			}
		}
		else // bump dates by required amount
		{
			if (bHasDue)
			{
				// Make sure the new date fits the recurring scheme
				if (OffsetTaskDate(dwNewTaskID, TDCD_DUEDATE, nOffsetDays, TDCU_DAYS, TDCOTD_FITTORECURRINGSCHEME | TDCOTD_OFFSETSUBTASKS) == SET_CHANGE)
					nRes = SET_CHANGE;
			}
			else
			{
				nRes = InitMissingTaskDate(dwNewTaskID, TDCD_DUE, dtNext);
			}
		}

		// adjust start dates similarly
		if (bHasStart)
		{
			// BUT DON'T FIT THE NEW DATE TO THE RECURRING SCHEME
			if (bWantInheritStart)
			{
				// don't offset children
				if (OffsetTaskDate(dwNewTaskID, TDCD_STARTDATE, nOffsetDays, TDCU_DAYS, 0) == SET_CHANGE)
				{
					ApplyLastChangeToSubtasks(dwNewTaskID, TDCA_STARTDATE);
					nRes = SET_CHANGE;
				}
			}
			else // offset children
			{
				if (OffsetTaskDate(dwNewTaskID, TDCD_STARTDATE, nOffsetDays, TDCU_DAYS, TDCOTD_OFFSETSUBTASKS) == SET_CHANGE)
					nRes = SET_CHANGE;
			}
		}
	}
	else // dtNext is the new start date
	{
		int nOffsetDays = (bHasStart ? ((int)dtNext - (int)dtStart) : 0);

		if (bWantInheritStart)
		{
			if (SetTaskDate(dwNewTaskID, TDCD_START, dtNext) == SET_CHANGE)
			{
				ApplyLastChangeToSubtasks(dwNewTaskID, TDCA_STARTDATE);
				nRes = SET_CHANGE;
			}
		}
		else // bump dates by required amount
		{
			if (bHasStart)
			{
				// Make sure the new date fits the recurring scheme
				if (OffsetTaskDate(dwNewTaskID, TDCD_STARTDATE, nOffsetDays, TDCU_DAYS, TDCOTD_FITTORECURRINGSCHEME | TDCOTD_OFFSETSUBTASKS) == SET_CHANGE)
					nRes = SET_CHANGE;
			}
			else
			{
				nRes = InitMissingTaskDate(dwNewTaskID, TDCD_START, dtNext);
			}
		}

		// adjust due dates similarly
		if (bHasDue)
		{
			// BUT DON'T FIT THE NEW DATE TO THE RECURRING SCHEME
			if (bWantInheritDue)
			{
				// don't update children
				if (OffsetTaskDate(dwNewTaskID, TDCD_DUEDATE, nOffsetDays, TDCU_DAYS, 0) == SET_CHANGE)
				{
					ApplyLastChangeToSubtasks(dwNewTaskID, TDCA_DUEDATE);
					nRes = SET_CHANGE;
				}
			}
			else // bump
			{
				if (OffsetTaskDate(dwNewTaskID, TDCD_DUEDATE, nOffsetDays, TDCU_DAYS, TDCOTD_OFFSETSUBTASKS) == SET_CHANGE)
					nRes = SET_CHANGE;
			}
		}
	}

	return nRes;
}

BOOL CToDoCtrlData::InitialiseNewRecurringTask(DWORD dwPrevTaskID, DWORD dwNewTaskID, const COleDateTime& dtNext, BOOL bDueDate)
{
	if (!HasTask(dwPrevTaskID) || !HasTask(dwNewTaskID))
	{
		ASSERT(0);
		return FALSE;
	}

	// reset new task state to 'undone' including all children
	SetTaskDone(dwNewTaskID, 0.0, TRUE, TRUE);
	
	// we need to move both the due date and the start date forward
	AdjustNewRecurringTasksDates(dwPrevTaskID, dwNewTaskID, dtNext, bDueDate);

	// Clear certain attributes
	ClearTaskAttribute(dwNewTaskID, TDCA_TIMESPENT, TRUE);
	ClearTaskAttribute(dwNewTaskID, TDCA_PERCENT, TRUE);

	// Set some defaults
	SetTaskStatus(dwNewTaskID, m_sDefaultStatus);
	ApplyLastChangeToSubtasks(dwNewTaskID, TDCA_STATUS);

	// Reset number of occurrences
	ResetRecurringSubtaskOccurrences(dwNewTaskID);

	// Special handling for recreated tasks
	if (dwNewTaskID != dwPrevTaskID)
	{
		SetTaskDate(dwNewTaskID, TDCD_CREATE, COleDateTime::GetCurrentTime());

		// the task ID has effectively changed so fix up those
		// tasks that previously had a dependency
		FixupTaskLocalDependentsIDs(dwNewTaskID, dwPrevTaskID);

		// Restore previous comments format
		CONTENTFORMAT cfComments;
		const CBinaryData& customComments = GetTaskCustomComments(dwPrevTaskID, cfComments);

		SetTaskCommentsType(dwNewTaskID, cfComments);
	}

	// optionally clear the comments
	TDCRECURRENCE tr;

	if (!GetTaskRecurrence(dwNewTaskID, tr) || !tr.bPreserveComments)
		ClearTaskAttribute(dwNewTaskID, TDCA_COMMENTS, TRUE);

	return TRUE;
}

// External version
TDC_SET CToDoCtrlData::SetTaskDone(DWORD dwTaskID, const COleDateTime& date,
								   BOOL bAndSubtasks, BOOL bUpdateAllSubtaskDates)
{
	return SetTaskDone(dwTaskID, date, bAndSubtasks, bUpdateAllSubtaskDates, FALSE);
}

// Internal version
TDC_SET CToDoCtrlData::SetTaskDone(DWORD dwTaskID, const COleDateTime& date,
							   BOOL bAndSubtasks, BOOL bUpdateAllSubtaskDates, BOOL bIsSubtask)
{
	ASSERT(bAndSubtasks || !bIsSubtask);
	ASSERT(!CDateHelper::IsDateSet(date) || !bUpdateAllSubtaskDates);

	TDC_SET nRes = SET_NOCHANGE;

	// If bUpdateAllSubtaskDates == FALSE, we only update a subtask's 
	// completion date if its completion state has also changed
	BOOL bDone = CDateHelper::IsDateSet(date);
	BOOL bWasDone = IsTaskDone(dwTaskID);
	BOOL bStateChange = Misc::StateChanged(bDone, bWasDone);
	BOOL bDateChange = bStateChange;

	if (!bDateChange && bDone && bWasDone)
		bDateChange = (date != GetTaskDate(dwTaskID, TDCD_DONE));

	if (bDateChange && (!bIsSubtask || bUpdateAllSubtaskDates || bStateChange))
	{
		if (SetTaskDate(dwTaskID, TDCD_DONE, date) == SET_CHANGE)
			nRes = SET_CHANGE;
	}

	if (bAndSubtasks && TaskHasSubtasks(dwTaskID))
	{
		const TODOSTRUCTURE* pTDS = LocateTask(dwTaskID);
		ASSERT(pTDS);

		for (int nSubtask = 0; nSubtask < pTDS->GetSubTaskCount(); nSubtask++)
		{
			DWORD dwSubtaskID = pTDS->GetSubTaskID(nSubtask);

			if (SetTaskDone(dwSubtaskID, date, TRUE, bUpdateAllSubtaskDates, TRUE) == SET_CHANGE)
				nRes = SET_CHANGE;
		}
	}

	return nRes;
}

