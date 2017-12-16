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
	CUndoAction(CToDoCtrlData& data, TDCUNDOACTIONTYPE nType, BOOL bExtendLastAction);
	~CUndoAction();

protected:
	CToDoCtrlData& m_data;
	BOOL m_bActive;
};

//////////////////////////////////////////////////////////////////////

#define IMPLEMENT_UNDO(data, type)					CUndoAction ua(m_data, type, FALSE)
#define IMPLEMENT_UNDO_EDIT(data)					CUndoAction ua(m_data, TDCUAT_EDIT, FALSE)
#define IMPLEMENT_UNDO_EXTEND(data, type, extend)	CUndoAction ua(m_data, type, extend)

//////////////////////////////////////////////////////////////////////

class CToDoCtrlData  
{
	friend class CTDCTaskMatcher;
	friend class CTDCTaskComparer;

public:
	CToDoCtrlData(const CWordArray& aStyles);
	virtual ~CToDoCtrlData();

	int BuildDataModel(const CTaskFile& tasks);
	
	inline int GetTaskCount() const { return m_items.GetCount(); }
	
	TODOITEM* NewTask() const;
	TODOITEM* NewTask(const TODOITEM& tdiRef, DWORD dwParentTaskID = 0) const;
	TODOITEM* NewTask(const CTaskFile& tasks, HTASKITEM hTask, const TODOITEM* pTDIRef = NULL) const;

	BOOL AddTask(DWORD dwTaskID, TODOITEM* pTDI, DWORD dwParentID, DWORD dwPrevSiblingID);
	BOOL DeleteTask(DWORD dwTaskID);
	void DeleteAllTasks();

	const TODOSTRUCTURE* LocateTask(DWORD dwTaskID) const;
	const TODOSTRUCTURE* GetStructure() const { return &m_struct; }
	BOOL HasTask(DWORD dwTaskID) const;

	const TODOITEM* GetTask(DWORD dwTaskID) const;
	const TODOITEM* GetTrueTask(DWORD& dwTaskID) const;
	BOOL GetTask(DWORD dwTaskID, const TODOITEM*& pTDI, const TODOSTRUCTURE*& pTDS) const;
	BOOL GetTrueTask(DWORD& dwTaskID, const TODOITEM*& pTDI, const TODOSTRUCTURE*& pTDS) const;

	BOOL HasOverdueTasks() const;
	BOOL HasDueTodayTasks() const;
	double GetEarliestDueDate() const;
	BOOL HasLockedTasks() const;

	BOOL CanMoveTask(DWORD dwTaskID, DWORD dwDestParentID) const;
	BOOL MoveTask(DWORD dwTaskID, DWORD dwDestParentID, DWORD dwDestPrevSiblingID);
	BOOL MoveTasks(const CDWordArray& aTaskIDs, DWORD dwDestParentID, DWORD dwDestPrevSiblingID);
	BOOL FixupParentCompletion(DWORD dwParentID, BOOL bClearStatus);

	// undo/redo
	BOOL BeginNewUndoAction(TDCUNDOACTIONTYPE nType);
	BOOL EndCurrentUndoAction();
	BOOL ExtendLastUndoAction(TDCUNDOACTIONTYPE nType = TDCUAT_NONE);
	BOOL UndoLastAction(BOOL bUndo, CArrayUndoElements& aElms);
	BOOL CanUndoLastAction(BOOL bUndo) const;
	int GetLastUndoActionTaskIDs(BOOL bUndo, CDWordArray& aIDs) const;
	TDCUNDOACTIONTYPE GetLastUndoActionType(BOOL bUndo) const;
	BOOL DeleteLastUndoAction();

	// Gets
	BOOL GetTaskAttributes(DWORD dwTaskID, TODOITEM& tdi) const;

	DWORD GetTaskParentID(DWORD dwTaskID) const;
	CString GetTaskTitle(DWORD dwTaskID) const;
	CString GetTaskIcon(DWORD dwTaskID) const;
	COleDateTime GetTaskDate(DWORD dwTaskID, TDC_DATE nDate) const;
	BOOL TaskHasDate(DWORD dwTaskID, TDC_DATE nDate) const;
	COLORREF GetTaskColor(DWORD dwTaskID) const; // -1 on no item selected
	CString GetTaskComments(DWORD dwTaskID) const;
	const CBinaryData& GetTaskCustomComments(DWORD dwTaskID, CString& sCommentsTypeID) const;
	int GetTaskPercent(DWORD dwTaskID, BOOL bCheckIfDone) const;
	double GetTaskTimeEstimate(DWORD dwTaskID, TDC_UNITS& nUnits) const;
	double GetTaskTimeSpent(DWORD dwTaskID, TDC_UNITS& nUnits) const;
	double GetTaskCost(DWORD dwTaskID) const;
	CString GetTaskAllocBy(DWORD dwTaskID) const;
	CString GetTaskCreatedBy(DWORD dwTaskID) const;
	CString GetTaskStatus(DWORD dwTaskID) const;
	CString GetTaskDependency(DWORD dwTaskID, int nDepends) const;
	CString GetTaskFileRef(DWORD dwTaskID, int nFileRef) const;
	CString GetTaskExtID(DWORD dwTaskID) const;
	CString GetTaskLastModifiedBy(DWORD dwTaskID) const;
	int GetTaskPriority(DWORD dwTaskID) const;
	int GetTaskRisk(DWORD dwTaskID) const;
	BOOL IsTaskFlagged(DWORD dwTaskID) const;
	BOOL IsTaskLocked(DWORD dwTaskID) const;
	BOOL IsTaskDone(DWORD dwTaskID) const;
	BOOL GetTaskRecurrence(DWORD dwTaskID, TDCRECURRENCE& tr) const;
	BOOL GetTaskNextOccurrence(DWORD dwTaskID, COleDateTime& dtNext, BOOL& bDue);
	BOOL IsTaskRecurring(DWORD dwTaskID) const;
	BOOL CanTaskRecur(DWORD dwTaskID) const;
	CString GetTaskVersion(DWORD dwTaskID) const;
	CString FormatTaskPath(DWORD dwTaskID, int nMaxLen = -1) const; 
	CString FormatTaskPosition(DWORD dwTaskID) const; 
	BOOL GetTaskCustomAttributeData(DWORD dwTaskID, const CString& sAttribID, TDCCADATA& data) const;

	int GetTaskAllocTo(DWORD dwTaskID, CStringArray& aAllocTo) const;
	int GetTaskCategories(DWORD dwTaskID, CStringArray& aCategories) const;
	int GetTaskTags(DWORD dwTaskID, CStringArray& aTags) const;
	int GetTaskDependencies(DWORD dwTaskID, CStringArray& aDepends) const;
	int GetTaskFileRefs(DWORD dwTaskID, CStringArray& aFiles) const;
	int GetTaskFileRefCount(DWORD dwTaskID) const;
	int GetTaskArray(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, CStringArray& aItems) const;

	DWORD GetTrueTaskID(DWORD dwTaskID) const;
	void GetTrueTaskIDs(CDWordArray& aTaskIDs) const;
	BOOL IsTaskReference(DWORD dwTaskID) const;
	DWORD GetTaskReferenceID(DWORD dwTaskID) const;
	BOOL IsTaskReferenced(DWORD dwTaskID) const;
	int GetReferencesToTask(DWORD dwTaskID, CDWordArray& aRefIDs) const;
	BOOL IsReferenceToTask(DWORD dwTestID, DWORD dwTaskID) const;
	BOOL IsTaskTimeTrackable(DWORD dwTaskID) const;
	BOOL IsParentTaskDone(DWORD dwTaskID) const;

	BOOL TaskHasDependencies(DWORD dwTaskID) const;
	BOOL TaskHasDependents(DWORD dwTaskID) const;
	int GetTaskLocalDependents(DWORD dwTaskID, CDWordArray& aDependents) const;
	int GetTaskLocalDependencies(DWORD dwTaskID, CDWordArray& aDependencies) const;
	BOOL TaskHasLocalCircularDependencies(DWORD dwTaskID) const;
	void FixupTaskLocalDependentsIDs(DWORD dwTaskID, DWORD dwPrevTaskID);
	BOOL RemoveTaskLocalDependency(DWORD dwTaskID, DWORD dwDependID);
	BOOL IsTaskLocallyDependentOn(DWORD dwTaskID, DWORD dwOtherID, BOOL bImmediateOnly) const;
	BOOL IsTaskDependent(DWORD dwTaskID) const;

	BOOL CalcIsTaskRecurring(DWORD dwTaskID) const;
	BOOL CalcIsTaskFlagged(DWORD dwTaskID) const;
	BOOL CalcIsTaskLocked(DWORD dwTaskID) const;
	BOOL CalcIsTaskDone(DWORD dwTaskID, DWORD dwExtraCheck = TDCCHECKALL) const;
	BOOL CalcIsTaskStarted(DWORD dwTaskID, BOOL bToday = FALSE) const;
	BOOL CalcIsTaskDue(DWORD dwTaskID, BOOL bToday = FALSE) const;
	BOOL CalcIsTaskOverDue(DWORD dwTaskID) const;
	double CalcTaskDueDate(DWORD dwTaskID) const;
	double CalcTaskStartDate(DWORD dwTaskID) const;
	int CalcTaskHighestPriority(DWORD dwTaskID, BOOL bIncludeDue = TRUE) const;
	int CalcTaskHighestRisk(DWORD dwTaskID) const;
	int CalcTaskPercentDone(DWORD dwTaskID) const;
	double CalcTaskCost(DWORD dwTaskID) const;
	double CalcTaskTimeEstimate(DWORD dwTaskID, TDC_UNITS nUnits) const;
	double CalcTaskTimeSpent(DWORD dwTaskID, TDC_UNITS nUnits) const;
	double CalcTaskRemainingTime(DWORD dwTaskID, TDC_UNITS& nUnits) const;
	BOOL CalcTaskCustomAttributeData(DWORD dwTaskID, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, double& dValue, TDC_UNITS nUnits = TDCU_NULL) const;
	BOOL CalcTaskSubtaskTotals(DWORD dwTaskID, int& nSubtasksTotal, int& nSubtasksDone) const;
	double CalcTaskSubtaskCompletion(DWORD dwTaskID) const;

	CString FormatTaskAllocTo(DWORD dwTaskID) const;
	CString FormatTaskCategories(DWORD dwTaskID) const;
	CString FormatTaskTags(DWORD dwTaskID) const;
	CString FormatTaskSubtaskCompletion(DWORD dwTaskID) const;

	BOOL TaskHasIncompleteSubtasks(DWORD dwTaskID, BOOL bExcludeRecurring) const;
	BOOL TaskHasCompletedSubtasks(DWORD dwTaskID) const;
	BOOL TaskHasSubtasks(DWORD dwTaskID) const;
	BOOL TaskHasFileRef(DWORD dwTaskID) const;

	BOOL CalcIsTaskFlagged(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	BOOL CalcIsTaskLocked(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	BOOL CalcIsTaskStarted(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bToday = FALSE) const;
	BOOL CalcIsTaskDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, DWORD dwExtraCheck = TDCCHECKALL) const;
	BOOL CalcIsTaskDue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bToday = FALSE) const;
	BOOL CalcIsTaskOverDue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double CalcTaskDueDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double CalcTaskStartDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	int CalcTaskHighestPriority(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bIncludeDue = TRUE) const;
	int CalcTaskHighestRisk(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double CalcTaskCost(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double CalcTaskTimeEstimate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS nUnits) const;
	double CalcTaskRemainingTime(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS& nUnits) const;
	TDC_UNITS CalcBestTimeEstUnits(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double CalcTaskTimeSpent(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS nUnits) const;
	TDC_UNITS CalcBestTimeSpentUnits(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	int CalcTaskPercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	int CalcPercentFromTime(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const; // spent / estimate
	BOOL CalcTaskSubtaskTotals(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, 
							int& nSubtasksTotal, int& nSubtasksDone) const;
	double CalcTaskSubtaskCompletion(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	BOOL CalcTaskCustomAttributeData(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, double& dValue, TDC_UNITS nUnits = TDCU_NULL) const;

	CString FormatTaskSubtaskCompletion(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString FormatTaskPath(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString FormatTaskPosition(const TODOSTRUCTURE* pTDS) const;
	CString FormatTaskAllocTo(const TODOITEM* pTDI) const;
	CString FormatTaskCategories(const TODOITEM* pTDI) const;
	CString FormatTaskTags(const TODOITEM* pTDI) const;

	BOOL TaskHasIncompleteSubtasks(const TODOSTRUCTURE* pTDS, BOOL bExcludeRecurring) const;
	BOOL TaskHasCompletedSubtasks(const TODOSTRUCTURE* pTDS) const;

	// Sets
	TDC_SET SetTaskAttributes(DWORD dwTaskID, const TODOITEM& tdi);
	
	TDC_SET SetTaskDate(DWORD dwTaskID, TDC_DATE nDate, const COleDateTime& date);
	TDC_SET SetTaskColor(DWORD dwTaskID, COLORREF color);
	TDC_SET SetTaskIcon(DWORD dwTaskID, const CString& sIcon);
	TDC_SET SetTaskComments(DWORD dwTaskID, const CString& sComments, const CBinaryData& customComments = _T(""));
	TDC_SET SetTaskCommentsType(DWORD dwTaskID, const CString& sCommentsTypeID);
	TDC_SET SetTaskPercent(DWORD dwTaskID, int nPercent);
	TDC_SET SetTaskTimeEstimate(DWORD dwTaskID, double dTime, TDC_UNITS nUnits);
	TDC_SET SetTaskTimeSpent(DWORD dwTaskID, double dTime, TDC_UNITS nUnits);
	TDC_SET SetTaskCost(DWORD dwTaskID, double dCost);
	TDC_SET SetTaskAllocBy(DWORD dwTaskID, const CString& sAllocBy);
	TDC_SET SetTaskStatus(DWORD dwTaskID, const CString& sStatus);
	TDC_SET SetTaskExternalID(DWORD dwTaskID, const CString& sID);
	TDC_SET SetTaskPriority(DWORD dwTaskID, int nPriority); // 0-10 (10 is highest)
	TDC_SET SetTaskRisk(DWORD dwTaskID, int nRisk); // 0-10 (10 is highest)
	TDC_SET SetTaskTitle(DWORD dwTaskID, const CString& sTitle);
	TDC_SET SetTaskFlag(DWORD dwTaskID, BOOL bFlagged);
	TDC_SET SetTaskLock(DWORD dwTaskID, BOOL bLocked);
	TDC_SET SetTaskRecurrence(DWORD dwTaskID, const TDCRECURRENCE& tr);
	TDC_SET SetTaskVersion(DWORD dwTaskID, const CString& sVersion);
	TDC_SET SetTaskCustomAttributeData(DWORD dwTaskID, const CString& sAttribID, const TDCCADATA& data);

	TDC_SET SetTaskAllocTo(DWORD dwTaskID, const CStringArray& aAllocTo, BOOL bAppend);
	TDC_SET SetTaskCategories(DWORD dwTaskID, const CStringArray& aCategories, BOOL bAppend);
	TDC_SET SetTaskTags(DWORD dwTaskID, const CStringArray& aTags, BOOL bAppend);
	TDC_SET SetTaskDependencies(DWORD dwTaskID, const CStringArray& aDepends, BOOL bAppend);
	TDC_SET SetTaskFileRefs(DWORD dwTaskID, const CStringArray& aFileRefs, BOOL bAppend);
	TDC_SET SetTaskArray(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, const CStringArray& aItems, BOOL bAppend);

	TDC_SET ClearTaskColor(DWORD dwTaskID) { SetTaskColor(dwTaskID, CLR_NONE); }
	TDC_SET OffsetTaskDate(DWORD dwTaskID, TDC_DATE nDate, int nAmount, TDC_UNITS nUnits, BOOL bAndSubtasks, BOOL bFitToRecurringScheme);
	TDC_SET InitMissingTaskDate(DWORD dwTaskID, TDC_DATE nDate, const COleDateTime& date, BOOL bAndSubtasks);
	TDC_SET MoveTaskStartAndDueDates(DWORD dwTaskID, const COleDateTime& dtNewStart);

	TDC_SET ClearTaskAttribute(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, BOOL bAndChildren = FALSE);
	TDC_SET ClearTaskCustomAttribute(DWORD dwTaskID, const CString& sAttribID, BOOL bAndChildren = FALSE);

	TDC_SET CopyTaskAttributeData(DWORD dwTaskID, TDC_ATTRIBUTE nFromAttrib, TDC_ATTRIBUTE nToAttrib);
	TDC_SET CopyTaskAttributeData(DWORD dwTaskID, TDC_ATTRIBUTE nFromAttrib, const CString& sToCustomAttribID);
	TDC_SET CopyTaskAttributeData(DWORD dwTaskID, const CString& sFromCustomAttribID, TDC_ATTRIBUTE nToAttrib);
	TDC_SET CopyTaskAttributeData(DWORD dwTaskID, const CString& sFromCustomAttribID, const CString& sToCustomAttribID);

	BOOL ApplyLastChangeToSubtasks(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, BOOL bIncludeBlank = TRUE);
	void ApplyLastInheritedChangeToSubtasks(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib);
	BOOL ResetRecurringSubtaskOccurrences(DWORD dwTaskID);
	void ApplyLastInheritedChangeFromParent(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib);
	
	inline BOOL HasStyle(int nStyle) const { return m_aStyles[nStyle] ? TRUE : FALSE; }
	
	static void SetDefaultCommentsFormat(const CString& format);
	static void SetDefaultTimeUnits(TDC_UNITS nTimeEstUnits, TDC_UNITS nTimeSpentUnits);
	static void SetInheritedParentAttributes(const CTDCAttributeMap& mapAttribs, BOOL bUpdateAttrib);
	static BOOL WantUpdateInheritedAttibute(TDC_ATTRIBUTE nAttrib);
	static DH_UNITS MapUnitsToDHUnits();

protected:
	CToDoCtrlDataItems m_items; // the real data
	const CWordArray& m_aStyles; // CToDoCtrl styles
	CToDoCtrlUndo m_undo;
	CToDoCtrlDataStructure m_struct;
	BOOL m_bUndoRedoing;

	static CString s_cfDefault;
	static TDC_UNITS s_nDefTimeEstUnits, s_nDefTimeSpentUnits;
	static CTDCAttributeMap s_mapParentAttribs; // inheritable attribs
	static BOOL s_bUpdateInheritAttrib; // update as changes are made to parents

protected:
	BOOL DeleteTask(TODOSTRUCTURE* pTDSParent, int nPos);
	BOOL AddTaskToDataModel(const CTaskFile& tasks, HTASKITEM hTask, TODOSTRUCTURE* pTDSParent);
	BOOL RemoveOrphanTaskReferences(TODOSTRUCTURE* pTDSParent, DWORD dwTaskID);
	int GetReferencesToTask(DWORD dwTaskID, const TODOSTRUCTURE* pTDS, CDWordArray& aRefIDs) const;
	BOOL IsTaskReferenced(DWORD dwTaskID, const TODOSTRUCTURE* pTDS) const;

	BOOL FindTaskLocalDependency(DWORD dwTaskID, DWORD dwDependsID, CDWordSet& mapVisited) const;
	void FixupTaskLocalDependentsDates(DWORD dwTaskID, TDC_DATE nDate);
	UINT UpdateTaskLocalDependencyDates(DWORD dwTaskID, TDC_DATE nDate);
	UINT SetNewTaskDependencyStartDate(DWORD dwTaskID, const COleDateTime& dtNewStart);
	BOOL CalcNewTaskDependencyStartDate(DWORD dwTaskID, DWORD dwDependencyID, TDC_DATE nDate, COleDateTime& dtNewStart) const;
	BOOL CalcNewTaskDependencyStartDate(DWORD dwTaskID, TDC_DATE nDate, COleDateTime& dtNewStart) const;
	BOOL RemoveOrphanTaskLocalDependencies(TODOSTRUCTURE* pTDSParent, DWORD dwDependID);
	TDC_SET RecalcTaskTimeEstimate(DWORD dwTaskID, TODOITEM* pTDI, TDC_DATE nDate);
	TDC_SET SetTaskDate(DWORD dwTaskID, TODOITEM* pTDI, TDC_DATE nDate, const COleDateTime& date, BOOL bRecalcTimeEstimate = TRUE);

	BOOL AddUndoElement(TDCUNDOELMOP nOp, DWORD dwTaskID, DWORD dwParentID = 0, 
						DWORD dwPrevSiblingID = 0, WORD wFlags = 0);
	BOOL SaveEditUndo(DWORD dwTaskID, TODOITEM* pTDI, TDC_ATTRIBUTE nAttrib);

	double CalcPercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double CalcWeightedPercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double CalcTaskTimeEstimate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS nUnits, double& dWeightedEstimate) const;
	double CalcStartDueDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bCheckChildren, BOOL bDue, BOOL bEarliest) const;
	BOOL CalcMissingStartDateFromDue(TODOITEM* pTDI) const;
	BOOL CalcMissingDueDateFromStart(TODOITEM* pTDI) const;
	int CalcTaskLeafCount(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bIncludeDone) const;

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
	TDC_SET EditTaskTimeAttribute(DWORD dwTaskID, TODOITEM* pTDI, TDC_ATTRIBUTE nAttrib, double& dValue, double dNewValue, TDC_UNITS& nUnits, TDC_UNITS nNewUnits);
	
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

	BOOL IsParentTaskDone(const TODOSTRUCTURE* pTDS) const;
	BOOL HasDueTodayTasks(const TODOSTRUCTURE* pTDS) const;
	BOOL HasLockedTasks(const TODOSTRUCTURE* pTDS) const;
	BOOL Locate(DWORD dwParentID, DWORD dwPrevSiblingID, TODOSTRUCTURE*& pTDSParent, int& nPos) const;
	int MoveTask(TODOSTRUCTURE* pTDSSrcParent, int nSrcPos, DWORD dwSrcPrevSiblingID,
							 TODOSTRUCTURE* pTDSDestParent, int nDestPos);
	BOOL SetTaskModified(DWORD dwTaskID);
	BOOL AddTask(DWORD dwTaskID, TODOITEM* pTDI, DWORD dwParentID, DWORD dwPrevSiblingID, BOOL bWantUndo);

	BOOL GetTaskAttributeData(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, TDCCADATA& data) const;
	TDC_SET SetTaskAttributeData(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, const TDCCADATA& data);

	static double GetBestDate(double dBest, double dDate, BOOL bEarliest);
	static double CalcDuration(const COleDateTime& dateStart, const COleDateTime& dateDue, TDC_UNITS nUnits);
	static COleDateTime AddDuration(COleDateTime& dateStart, double dDuration, TDC_UNITS nUnits);
	static BOOL IsEndOfDay(const COleDateTime& date);
	static BOOL IsValidDateRange(const COleDateTime& dateStart, const COleDateTime& dateDue);
	static double GetCalculationValue(const TDCCADATA& data, TDC_UNITS nUnits);

};

#endif // !defined(AFX_TODOCTRLDATA_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
