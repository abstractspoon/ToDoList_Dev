// ToDoCtrlData.h: interface for the CToDoCtrlData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TODOCTRLDATA_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
#define AFX_TODOCTRLDATA_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tdcstruct.h"
#include "tdcenum.h"
#include "todoctrlundo.h"
#include "todoitem.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

class CToDoCtrlData;
class CBinaryData;
class CDWordSet;

//////////////////////////////////////////////////////////////////////

// class to help start and end undo actions
// CToDoCtrlData only allows a single action to be active at any one time
class CUndoAction
{
public:
	CUndoAction(CToDoCtrlData& data, TDC_UNDOACTIONTYPE nType, BOOL bExtendLastAction);
	~CUndoAction();

protected:
	CToDoCtrlData& m_data;
	BOOL m_bActive;
};

//////////////////////////////////////////////////////////////////////

class CToDoCtrlData  
{
	friend class CTDCTaskMatcher;
	friend class CTDCTaskComparer;
	friend class CTDCTaskCalculator;
	friend class CTDCTaskFormatter;
	friend class CTDCTaskExporter;

public:
	CToDoCtrlData(const CTDCStyleMap& styles, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs);
	virtual ~CToDoCtrlData();

	int BuildDataModel(const CTaskFile& tasks);
	
	inline int GetTaskCount() const { return m_items.GetCount(); }
	
	TODOITEM* NewTask(const TODOITEM& tdiRef, DWORD dwParentTaskID = 0) const;
	TODOITEM* NewTask(const CTaskFile& tasks, HTASKITEM hTask, const TODOITEM* pTDIRef = NULL) const;

	BOOL AddTask(DWORD dwTaskID, TODOITEM* pTDI, DWORD dwParentID, DWORD dwPrevSiblingID);
	BOOL DeleteTask(DWORD dwTaskID, BOOL bWithUndo);
	void DeleteAllTasks();

	const TODOSTRUCTURE* LocateTask(DWORD dwTaskID) const;
	const TODOSTRUCTURE* GetStructure() const { return &m_struct; }
	
	BOOL HasTask(DWORD dwTaskID) const;
	BOOL TaskHasSibling(DWORD dwTaskID, DWORD dwSiblingID, BOOL bImmediate = TRUE) const;
	BOOL TaskHasParent(DWORD dwTaskID, DWORD dwParentID, BOOL bImmediate = TRUE) const;

	const TODOITEM* GetTask(DWORD dwTaskID) const;
	const TODOITEM* GetTrueTask(DWORD& dwTaskID) const;
	BOOL GetTask(DWORD dwTaskID, const TODOITEM*& pTDI, const TODOSTRUCTURE*& pTDS) const;
	BOOL GetTrueTask(DWORD& dwTaskID, const TODOITEM*& pTDI, const TODOSTRUCTURE*& pTDS) const;

	POSITION GetFirstTaskPosition() const;
	DWORD GetNextTask(POSITION& pos, const TODOITEM*& pTDI) const;
	DWORD GetNextTaskID(POSITION& pos) const;

	BOOL CanMoveTask(DWORD dwTaskID, DWORD dwDestParentID) const;
	BOOL MoveTask(DWORD dwTaskID, DWORD dwDestParentID, DWORD dwDestPrevSiblingID);
	BOOL MoveTasks(const CDWordArray& aTaskIDs, DWORD dwDestParentID, DWORD dwDestPrevSiblingID);
	BOOL FixupParentCompletion(DWORD dwParentID, BOOL bClearStatus);
	BOOL CanOffsetTaskDate(DWORD dwTaskID, TDC_DATE nDate, int nAmount, TDC_UNITS nUnits);

	// undo/redo
	BOOL BeginNewUndoAction(TDC_UNDOACTIONTYPE nType);
	BOOL EndCurrentUndoAction();
	BOOL ExtendLastUndoAction(TDC_UNDOACTIONTYPE nType = TDCUAT_NONE);
	BOOL UndoLastAction(BOOL bUndo, CArrayUndoElements& aElms);
	BOOL CanUndoLastAction(BOOL bUndo) const;
	int GetLastUndoActionTaskIDs(BOOL bUndo, CDWordArray& aIDs) const;
	TDC_UNDOACTIONTYPE GetLastUndoActionType(BOOL bUndo) const;
	BOOL DeleteLastUndoAction();

	// Gets
	BOOL GetTaskAttributes(DWORD dwTaskID, TODOITEM& tdi) const;
	int GetTaskSubtaskIDs(DWORD dwTaskID, CDWordArray& aSubtaskIDs, BOOL bAppend = FALSE) const;

	DWORD GetTaskParentID(DWORD dwTaskID) const;
	CString GetTaskTitle(DWORD dwTaskID) const;
	CString GetTaskIcon(DWORD dwTaskID) const;
	COleDateTime GetTaskDate(DWORD dwTaskID, TDC_DATE nDate) const;
	BOOL TaskHasDate(DWORD dwTaskID, TDC_DATE nDate) const;
	BOOL GetTaskStartDueDates(DWORD dwTaskID, COleDateTimeRange& dtRange) const;
	BOOL TaskHasStartDueDates(DWORD dwTaskID) const;
	COLORREF GetTaskColor(DWORD dwTaskID) const; // -1 on no item selected
	CString GetTaskComments(DWORD dwTaskID) const;
	const CBinaryData& GetTaskCustomComments(DWORD dwTaskID, CONTENTFORMAT& cfComments) const;
	int GetTaskPercent(DWORD dwTaskID, BOOL bCheckIfDone) const;
	BOOL GetTaskTimeEstimate(DWORD dwTaskID, TDCTIMEPERIOD& timeEst) const;
	BOOL GetTaskTimeSpent(DWORD dwTaskID, TDCTIMEPERIOD& timeSpent) const;
	BOOL GetTaskCost(DWORD dwTaskID, TDCCOST& cost) const;
	CString GetTaskAllocBy(DWORD dwTaskID) const;
	CString GetTaskCreatedBy(DWORD dwTaskID) const;
	CString GetTaskStatus(DWORD dwTaskID) const;
	CString GetTaskDependency(DWORD dwTaskID, int nDepends) const;
	CString GetTaskFileLink(DWORD dwTaskID, int nFileLink) const;
	CString GetTaskExtID(DWORD dwTaskID) const;
	CString GetTaskLastModifiedBy(DWORD dwTaskID) const;

	int GetTaskPriority(DWORD dwTaskID) const;
	int GetTaskRisk(DWORD dwTaskID) const;
	BOOL IsTaskFlagged(DWORD dwTaskID) const;
	BOOL IsTaskLocked(DWORD dwTaskID) const;
	BOOL IsTaskDone(DWORD dwTaskID) const;
	BOOL GetTaskRecurrence(DWORD dwTaskID, TDCRECURRENCE& tr) const;
	BOOL IsTaskRecurring(DWORD dwTaskID, BOOL bCheckRemaining) const;
	BOOL GetTaskNextOccurrence(DWORD dwTaskID, COleDateTime& dtNext, BOOL& bDue);
	BOOL ResetRecurringSubtaskOccurrences(DWORD dwTaskID);
	BOOL IsTaskRecurring(DWORD dwTaskID) const;
	BOOL CanTaskRecur(DWORD dwTaskID) const;
	CString GetTaskVersion(DWORD dwTaskID) const;
	BOOL GetTaskCustomAttributeData(DWORD dwTaskID, const CString& sAttribID, TDCCADATA& data) const;
	CString GetTaskMetaData(DWORD dwTaskID, const CString& sKey) const;

	int GetTaskAllocTo(DWORD dwTaskID, CStringArray& aAllocTo) const;
	int GetTaskCategories(DWORD dwTaskID, CStringArray& aCategories) const;
	int GetTaskTags(DWORD dwTaskID, CStringArray& aTags) const;
	int GetTaskDependencies(DWORD dwTaskID, CStringArray& aDepends) const;
	int GetTaskFileLinks(DWORD dwTaskID, CStringArray& aFiles) const;
	int GetTaskFileLinkCount(DWORD dwTaskID) const;
	int GetTaskArray(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, CStringArray& aItems) const;

	DWORD GetTrueTaskID(DWORD dwTaskID) const;
	void GetTrueTaskIDs(CDWordArray& aTaskIDs) const;
	BOOL IsTaskReference(DWORD dwTaskID) const;
	DWORD GetTaskReferenceID(DWORD dwTaskID) const;
	BOOL IsTaskReferenced(DWORD dwTaskID) const;
	int GetReferencesToTask(DWORD dwTaskID, CDWordArray& aRefIDs) const;
	BOOL IsReferenceToTask(DWORD dwTestID, DWORD dwTaskID) const;
	BOOL IsTaskTimeTrackable(DWORD dwTaskID) const;
	BOOL IsTaskParent(DWORD dwTaskID) const;

	BOOL TaskHasDependencies(DWORD dwTaskID) const;
	BOOL TaskHasDependents(DWORD dwTaskID) const;
	int GetTaskLocalDependents(DWORD dwTaskID, CDWordArray& aDependents) const;
	int GetTaskLocalDependencies(DWORD dwTaskID, CDWordArray& aDependencies) const;
	BOOL TaskHasLocalCircularDependencies(DWORD dwTaskID) const;
	void FixupTaskLocalDependentsIDs(DWORD dwTaskID, DWORD dwPrevTaskID);
	BOOL RemoveTaskLocalDependency(DWORD dwTaskID, DWORD dwDependID);
	BOOL IsTaskLocallyDependentOn(DWORD dwTaskID, DWORD dwOtherID, BOOL bImmediateOnly) const;
	BOOL IsTaskDependent(DWORD dwTaskID) const;

	BOOL TaskHasIncompleteSubtasks(DWORD dwTaskID, BOOL bExcludeRecurring) const;
	BOOL TaskHasCompletedSubtasks(DWORD dwTaskID) const;
	BOOL TaskHasSubtasks(DWORD dwTaskID) const;
	BOOL TaskHasFileLink(DWORD dwTaskID) const;

	BOOL TaskHasIncompleteSubtasks(const TODOSTRUCTURE* pTDS, BOOL bExcludeRecurring) const;
	BOOL TaskHasCompletedSubtasks(const TODOSTRUCTURE* pTDS) const;

	// Sets
	TDC_SET SetTaskAttributes(DWORD dwTaskID, const TODOITEM& tdi);
	
	TDC_SET SetTaskDate(DWORD dwTaskID, TDC_DATE nDate, const COleDateTime& date);
	TDC_SET SetTaskColor(DWORD dwTaskID, COLORREF color);
	TDC_SET SetTaskIcon(DWORD dwTaskID, const CString& sIcon);
	TDC_SET SetTaskComments(DWORD dwTaskID, const CString& sComments, const CBinaryData& customComments = _T(""));
	TDC_SET SetTaskCommentsType(DWORD dwTaskID, const CONTENTFORMAT& cfComments);
	TDC_SET SetTaskAllocBy(DWORD dwTaskID, const CString& sAllocBy);
	TDC_SET SetTaskStatus(DWORD dwTaskID, const CString& sStatus);
	TDC_SET SetTaskExternalID(DWORD dwTaskID, const CString& sID);
	TDC_SET SetTaskTitle(DWORD dwTaskID, const CString& sTitle);
	TDC_SET SetTaskFlag(DWORD dwTaskID, BOOL bFlagged);
	TDC_SET SetTaskLock(DWORD dwTaskID, BOOL bLocked);
	TDC_SET SetTaskRecurrence(DWORD dwTaskID, const TDCRECURRENCE& tr);
	TDC_SET SetTaskVersion(DWORD dwTaskID, const CString& sVersion);
	TDC_SET SetTaskCustomAttributeData(DWORD dwTaskID, const CString& sAttribID, const TDCCADATA& data);
	TDC_SET SetTaskMetaData(DWORD dwTaskID, const CString& sKey, const CString& sMetaData);
	
	TDC_SET SetTaskPercent(DWORD dwTaskID, int nPercent, BOOL bOffset = FALSE);
	TDC_SET SetTaskPriority(DWORD dwTaskID, int nPriority, BOOL bOffset = FALSE); // 0-10 (10 is highest)
	TDC_SET SetTaskRisk(DWORD dwTaskID, int nRisk, BOOL bOffset = FALSE); // 0-10 (10 is highest)
	TDC_SET SetTaskTimeEstimate(DWORD dwTaskID, const TDCTIMEPERIOD& timeEst, BOOL bOffset = FALSE);
	TDC_SET SetTaskTimeSpent(DWORD dwTaskID, const TDCTIMEPERIOD& timeEst, BOOL bOffset = FALSE);
	TDC_SET SetTaskCost(DWORD dwTaskID, const TDCCOST& cost, BOOL bOffset = FALSE);
	
	TDC_SET SetTaskAllocTo(DWORD dwTaskID, const CStringArray& aAllocTo, BOOL bAppend = FALSE);
	TDC_SET SetTaskCategories(DWORD dwTaskID, const CStringArray& aCategories, BOOL bAppend = FALSE);
	TDC_SET SetTaskTags(DWORD dwTaskID, const CStringArray& aTags, BOOL bAppend = FALSE);
	TDC_SET SetTaskDependencies(DWORD dwTaskID, const CStringArray& aDepends, BOOL bAppend = FALSE);
	TDC_SET SetTaskFileLinks(DWORD dwTaskID, const CStringArray& aFileLinks, BOOL bAppend = FALSE);
	TDC_SET SetTaskArray(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, const CStringArray& aItems, BOOL bAppend = FALSE);

	TDC_SET ClearTaskColor(DWORD dwTaskID) { SetTaskColor(dwTaskID, CLR_NONE); }
	TDC_SET OffsetTaskDate(DWORD dwTaskID, TDC_DATE nDate, int nAmount, TDC_UNITS nUnits, BOOL bAndSubtasks, BOOL bFitToRecurringScheme);
	TDC_SET InitMissingTaskDate(DWORD dwTaskID, TDC_DATE nDate, const COleDateTime& date, BOOL bAndSubtasks);
	TDC_SET MoveTaskStartAndDueDates(DWORD dwTaskID, const COleDateTime& dtNewStart);

	TDC_SET ClearTaskAttribute(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, BOOL bAndChildren = FALSE);
	TDC_SET ClearTaskCustomAttribute(DWORD dwTaskID, const CString& sAttribID, BOOL bAndChildren = FALSE);

	TDC_SET CopyTaskAttributeValues(DWORD dwTaskID, TDC_ATTRIBUTE nFromAttrib, TDC_ATTRIBUTE nToAttrib);
	TDC_SET CopyTaskAttributeValues(DWORD dwTaskID, TDC_ATTRIBUTE nFromAttrib, const CString& sToCustomAttribID);
	TDC_SET CopyTaskAttributeValues(DWORD dwTaskID, const CString& sFromCustomAttribID, TDC_ATTRIBUTE nToAttrib);
	TDC_SET CopyTaskAttributeValues(DWORD dwTaskID, const CString& sFromCustomAttribID, const CString& sToCustomAttribID);

	TDC_SET RenameTasksAttributeValue(TDC_ATTRIBUTE nAttrib, const CString& sFrom, const CString& sTo, BOOL bCaseSensitive, BOOL bWholeWord);
	TDC_SET RenameTasksAttributeValue(const CString& sAttribID, const CString& sFrom, const CString& sTo, BOOL bCaseSensitive, BOOL bWholeWord);

	BOOL ApplyLastChangeToSubtasks(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, BOOL bIncludeBlank = TRUE);
	BOOL ApplyLastInheritedChangeToSubtasks(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib);
	BOOL ApplyLastInheritedChangeFromParent(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib);
	
	inline BOOL HasStyle(TDC_STYLE nStyle) const { return m_styles.IsStyleEnabled(nStyle); }
	
	void SetDefaultCommentsFormat(const CString& format);
	void SetDefaultTimeUnits(TDC_UNITS nTimeEstUnits, TDC_UNITS nTimeSpentUnits);
	TDC_UNITS GetDefaultTimeEstimateUnits() const { return m_nDefTimeEstUnits; }
	TDC_UNITS GetDefaultTimeSpentUnits() const { return m_nDefTimeSpentUnits; }
	void SetInheritedParentAttributes(const CTDCAttributeMap& mapAttribs, BOOL bUpdateAttrib);
	BOOL WantUpdateInheritedAttibute(TDC_ATTRIBUTE nAttrib) const;

protected:
	CToDoCtrlDataItems m_items; // the real data
	CToDoCtrlUndo m_undo;
	CToDoCtrlDataStructure m_struct;
	BOOL m_bUndoRedoing;

	const CTDCStyleMap& m_styles;
	const CTDCCustomAttribDefinitionArray& m_aCustomAttribDefs;

	CString m_cfDefault;
	TDC_UNITS m_nDefTimeEstUnits, m_nDefTimeSpentUnits;
	CTDCAttributeMap m_mapParentAttribs; // inheritable attribs
	BOOL m_bUpdateInheritAttrib; // update as changes are made to parents

protected:
	BOOL DeleteTask(TODOSTRUCTURE* pTDSParent, int nPos, BOOL bWithUndo);
	BOOL AddTaskToDataModel(const CTaskFile& tasks, HTASKITEM hTask, TODOSTRUCTURE* pTDSParent);
	int GetReferencesToTask(DWORD dwTaskID, const TODOSTRUCTURE* pTDS, CDWordArray& aRefIDs) const;
	BOOL IsTaskReferenced(DWORD dwTaskID, const TODOSTRUCTURE* pTDS) const;
	BOOL RemoveOrphanTaskReferences(TODOSTRUCTURE* pTDSParent, DWORD dwMatchID = 0);
	
	BOOL FindTaskLocalDependency(DWORD dwTaskID, DWORD dwDependsID, CDWordSet& mapVisited) const;
	void FixupTaskLocalDependentsDates(DWORD dwTaskID, TDC_DATE nDate);
	UINT UpdateTaskLocalDependencyDates(DWORD dwTaskID, TDC_DATE nDate);
	UINT SetNewTaskDependencyStartDate(DWORD dwTaskID, const COleDateTime& dtNewStart);
	BOOL CalcNewTaskDependencyStartDate(DWORD dwTaskID, DWORD dwDependencyID, TDC_DATE nDate, COleDateTime& dtNewStart) const;
	BOOL CalcNewTaskDependencyStartDate(DWORD dwTaskID, TDC_DATE nDate, COleDateTime& dtNewStart) const;
	BOOL RemoveOrphanTaskLocalDependencies(TODOSTRUCTURE* pTDSParent, DWORD dwDependID);
	TDC_SET RecalcTaskTimeEstimate(DWORD dwTaskID, TODOITEM* pTDI, TDC_DATE nDate);
	TDC_SET SetTaskDate(DWORD dwTaskID, TODOITEM* pTDI, TDC_DATE nDate, const COleDateTime& date, BOOL bRecalcTimeEstimate = TRUE);
	BOOL CalcMissingStartDateFromDue(TODOITEM* pTDI) const;
	BOOL CalcMissingDueDateFromStart(TODOITEM* pTDI) const;

	BOOL AddUndoElement(TDC_UNDOELMOP nOp, DWORD dwTaskID, DWORD dwParentID = 0, 
						DWORD dwPrevSiblingID = 0, WORD wFlags = 0);
	BOOL SaveEditUndo(DWORD dwTaskID, TODOITEM* pTDI, TDC_ATTRIBUTE nAttrib);

	BOOL LocateTask(DWORD dwTaskID, TODOSTRUCTURE*& pTDSParent, int& nPos) const;

	// internal versions
	TODOITEM* GetTrueTask(const TODOSTRUCTURE* pTDS) const;
	TODOITEM* GetTask(const TODOSTRUCTURE* pTDS) const;
	TODOITEM* GetTask(DWORD& dwTaskID, BOOL bTrue) const;
	BOOL GetTask(DWORD& dwTaskID, const TODOITEM*& pTDI, const TODOSTRUCTURE*& pTDS, BOOL bTrue) const;

	BOOL ApplyLastChangeToSubtasks(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_ATTRIBUTE nAttrib, BOOL bIncludeBlank);
	BOOL ApplyLastChangeToSubtask(const TODOITEM* pTDIParent, const TODOSTRUCTURE* pTDSParent, int nChildPos, TDC_ATTRIBUTE nAttrib, BOOL bIncludeBlank);

	TDC_SET CopyTaskAttributes(TODOITEM* pToTDI, DWORD dwFromTaskID, const CTDCAttributeMap& mapAttribs) const;

	template <class T>
	TDC_SET EditTaskAttributeT(DWORD dwTaskID, TODOITEM* pTDI, TDC_ATTRIBUTE nAttrib, T& val, const T& newValue)
	{
		// test for actual change
		if (val == newValue)
			return SET_NOCHANGE;
		
		return DoEditTaskAttribute(dwTaskID, pTDI, nAttrib, val, newValue);
	}
	TDC_SET EditTaskArrayAttribute(DWORD dwTaskID, TODOITEM* pTDI, TDC_ATTRIBUTE nAttrib, CStringArray& aValues, const CStringArray& aNewValues, BOOL bAppend, BOOL bOrderSensitive = FALSE);
	TDC_SET EditTaskTimeAttribute(DWORD dwTaskID, TODOITEM* pTDI, TDC_ATTRIBUTE nAttrib, TDCTIMEPERIOD& time, const TDCTIMEPERIOD& newTime);
	
	template <class T>
	TDC_SET DoEditTaskAttribute(DWORD dwTaskID, TODOITEM* pTDI, TDC_ATTRIBUTE nAttrib, T& val, const T& newValue)
	{
		ASSERT(dwTaskID);
		ASSERT(pTDI);
		ASSERT(nAttrib != TDCA_NONE);

		// save undo data
		SaveEditUndo(dwTaskID, pTDI, nAttrib);
		
		// make the change
		val = newValue;
		pTDI->SetModified();
		
		// update subtasks
		ApplyLastInheritedChangeToSubtasks(dwTaskID, nAttrib);
		
		return SET_CHANGE;
	}

	BOOL Locate(DWORD dwParentID, DWORD dwPrevSiblingID, TODOSTRUCTURE*& pTDSParent, int& nPos) const;
	int MoveTask(TODOSTRUCTURE* pTDSSrcParent, int nSrcPos, DWORD dwSrcPrevSiblingID, TODOSTRUCTURE* pTDSDestParent, int nDestPos);
	BOOL IsValidMoveDestination(DWORD dwTaskID, DWORD dwDestParentID) const;
	BOOL IsValidMoveDestination(const CDWordArray& aTaskIDs, DWORD dwDestParentID) const;
	BOOL SetTaskModified(DWORD dwTaskID);

	BOOL GetTaskAttributeValues(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, TDCCADATA& data) const;
	TDC_SET SetTaskAttributeValues(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, const TDCCADATA& data);
	BOOL TaskHasAttributeValue(TODOITEM* pTDI, TDC_ATTRIBUTE nAttrib, const CString& sText, BOOL bCaseSensitive, BOOL bWholeWord);

	static double CalcDuration(const COleDateTime& dateStart, const COleDateTime& dateDue, TDC_UNITS nUnits);
	static COleDateTime AddDuration(COleDateTime& dateStart, double dDuration, TDC_UNITS nUnits, BOOL bAllowUpdateStart);
	static COleDateTime CalcNewDueDate(const COleDateTime& dtCurStart, const COleDateTime& dtCurDue, TDC_UNITS nUnits, COleDateTime& dtNewStart);
	static int GetNextValue(int nValue, int nIncrement);
	static BOOL CanEditPriorityRisk(int nValue, int nNoValue, BOOL bOffset);

};

#endif // !defined(AFX_TODOCTRLDATA_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
