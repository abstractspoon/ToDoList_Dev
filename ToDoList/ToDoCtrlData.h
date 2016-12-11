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
#include "taskfile.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

class CToDoCtrlData;
class CBinaryData;

//////////////////////////////////////////////////////////////////////

typedef CMap<DWORD, DWORD, TODOITEM*, TODOITEM*&> CMapIDToTDI;

//////////////////////////////////////////////////////////////////////

// class to help start and end undo actions
// only one can be active at one time
class CUndoAction
{
public:
	CUndoAction(CToDoCtrlData& data, TDCUNDOACTIONTYPE nType = TDCUAT_EDIT, BOOL bExtendLastAction = FALSE);
	~CUndoAction();

protected:
	CToDoCtrlData& m_data;
	BOOL m_bActive;
};

//////////////////////////////////////////////////////////////////////

class CToDoCtrlData  
{
public:
	CToDoCtrlData(const CWordArray& aStyles);
	virtual ~CToDoCtrlData();

	int BuildDataModel(const CTaskFile& tasks);
	
	inline int GetTaskCount() const { return m_mapID2TDI.GetCount(); }
	
	TODOITEM* NewTask() const;
	TODOITEM* NewTask(const TODOITEM& tdiRef, DWORD dwParentTaskID = 0) const;
	TODOITEM* NewTask(const CTaskFile& tasks, HTASKITEM hTask) const;

	void AddTask(DWORD dwTaskID, TODOITEM* pTDI, DWORD dwParentID, DWORD dwPrevSiblingID);
	BOOL DeleteTask(DWORD dwTaskID);
	void DeleteAllTasks();

	const TODOSTRUCTURE* LocateTask(DWORD dwTaskID) const;
	const TODOSTRUCTURE* GetStructure() const { return &m_struct; }

	const TODOITEM* GetTask(DWORD& dwTaskID, BOOL bTrue = TRUE) const;
	BOOL GetTask(DWORD& dwTaskID, const TODOITEM*& pTDI, const TODOSTRUCTURE*& pTDS, BOOL bTrue = TRUE) const;
	BOOL HasTask(DWORD dwTaskID) const;

	BOOL HasOverdueTasks() const;
	BOOL HasDueTodayTasks() const;
	double GetEarliestDueDate() const;

	BOOL CanMoveTask(DWORD dwTaskID, DWORD dwDestParentID) const;
	BOOL MoveTask(DWORD dwTaskID, DWORD dwDestParentID, DWORD dwDestPrevSiblingID);
	BOOL MoveTasks(const CDWordArray& aTaskIDs, DWORD dwDestParentID, DWORD dwDestPrevSiblingID);
	void FixupParentCompletion(DWORD dwParentID);

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
	int GetTaskPriority(DWORD dwTaskID) const;
	int GetTaskRisk(DWORD dwTaskID) const;
	BOOL IsTaskFlagged(DWORD dwTaskID) const;
	BOOL GetTaskRecurrence(DWORD dwTaskID, TDCRECURRENCE& tr) const;
	BOOL GetTaskNextOccurrence(DWORD dwTaskID, COleDateTime& dtNext, BOOL& bDue);
	BOOL IsTaskRecurring(DWORD dwTaskID, BOOL bCheckParent = FALSE) const;
	BOOL CanTaskRecur(DWORD dwTaskID) const;
	CString GetTaskVersion(DWORD dwTaskID) const;
	CString GetTaskPath(DWORD dwTaskID, int nMaxLen = -1) const; 
	CString GetTaskPositionString(DWORD dwTaskID) const; 
	CString GetTaskCustomAttributeData(DWORD dwTaskID, const CString& sAttribID) const;

	int GetTaskAllocTo(DWORD dwTaskID, CStringArray& aAllocTo) const;
	int GetTaskCategories(DWORD dwTaskID, CStringArray& aCategories) const;
	int GetTaskTags(DWORD dwTaskID, CStringArray& aTags) const;
	int GetTaskDependencies(DWORD dwTaskID, CStringArray& aDepends) const;
	int GetTaskFileRefs(DWORD dwTaskID, CStringArray& aFiles) const;
	int GetTaskFileRefCount(DWORD dwTaskID) const;
	int GetTaskArray(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, CStringArray& aItems);

	DWORD GetTrueTaskID(DWORD dwTaskID) const;
	void GetTrueTaskIDs(CDWordArray& aTaskIDs) const;
	BOOL IsTaskReference(DWORD dwTaskID) const;
	DWORD GetTaskReferenceID(DWORD dwTaskID) const;
	BOOL IsTaskReferenced(DWORD dwTaskID) const;
	int GetReferencesToTask(DWORD dwTaskID, CDWordArray& aRefIDs) const;
	BOOL IsReferenceToTask(DWORD dwTestID, DWORD dwTaskID) const;

	BOOL TaskHasDependents(DWORD dwTaskID) const;
	int GetTaskLocalDependents(DWORD dwTaskID, CDWordArray& aDependents) const;
	int GetTaskLocalDependencies(DWORD dwTaskID, CDWordArray& aDependencies) const;
	BOOL TaskHasLocalCircularDependencies(DWORD dwTaskID) const;
	void FixupTaskLocalDependentsIDs(DWORD dwTaskID, DWORD dwPrevTaskID);
	BOOL RemoveTaskLocalDependency(DWORD dwTaskID, DWORD dwDependID);
	BOOL IsTaskLocallyDependentOn(DWORD dwTaskID, DWORD dwOtherID, BOOL bImmediateOnly) const;
	BOOL IsTaskDependent(DWORD dwTaskID) const;

	BOOL IsTaskDone(DWORD dwTaskID, DWORD dwExtraCheck = TDCCHECKNONE) const;
	BOOL IsTaskStarted(DWORD dwTaskID, BOOL bToday = FALSE) const;
	BOOL IsTaskDue(DWORD dwTaskID, BOOL bToday = FALSE) const;
	BOOL IsTaskOverDue(DWORD dwTaskID) const;
	double CalcTaskDueDate(DWORD dwTaskID) const;
	double CalcTaskStartDate(DWORD dwTaskID) const;
	int GetTaskHighestPriority(DWORD dwTaskID, BOOL bIncludeDue = TRUE) const;
	int GetTaskHighestRisk(DWORD dwTaskID) const;
	int CalcTaskPercentDone(DWORD dwTaskID) const;
	double CalcTaskCost(DWORD dwTaskID) const;
	double CalcTaskTimeEstimate(DWORD dwTaskID, TDC_UNITS nUnits) const;
	double CalcTaskTimeSpent(DWORD dwTaskID, TDC_UNITS nUnits) const;
	double CalcTaskRemainingTime(DWORD dwTaskID, TDC_UNITS& nUnits) const;
	CString FormatTaskAllocTo(DWORD dwTaskID) const;
	CString FormatTaskCategories(DWORD dwTaskID) const;
	CString FormatTaskTags(DWORD dwTaskID) const;
	BOOL CalcTaskCustomAttributeData(DWORD dwTaskID, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, double& dValue) const;
	BOOL GetTaskSubtaskTotals(DWORD dwTaskID, int& nSubtasksTotal, int& nSubtasksDone) const;
	BOOL TaskHasIncompleteSubtasks(DWORD dwTaskID, BOOL bExcludeRecurring) const;
	BOOL TaskHasRecurringParent(DWORD dwTaskID) const;
	BOOL TaskHasFileRef(DWORD dwTaskID) const;

	BOOL IsTaskStarted(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bToday = FALSE) const;
	BOOL IsTaskDue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bToday = FALSE) const;
	BOOL IsTaskOverDue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	BOOL IsTaskRecurring(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS = NULL, BOOL bCheckParent = FALSE) const;
	double CalcTaskDueDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double CalcTaskStartDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	int GetTaskHighestPriority(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bIncludeDue = TRUE) const;
	int GetTaskHighestRisk(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double CalcTaskCost(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double CalcTaskTimeEstimate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS nUnits) const;
	double CalcTaskRemainingTime(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS& nUnits) const;
	TDC_UNITS GetBestCalcTimeEstUnits(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double CalcTaskTimeSpent(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS nUnits) const;
	TDC_UNITS GetBestCalcTimeSpentUnits(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	int CalcTaskPercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	int CalcPercentFromTime(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const; // spent / estimate
	BOOL GetTaskSubtaskTotals(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, 
							int& nSubtasksTotal, int& nSubtasksDone) const;
	BOOL TaskHasIncompleteSubtasks(const TODOSTRUCTURE* pTDS, BOOL bExcludeRecurring) const;
	BOOL TaskHasRecurringParent(const TODOSTRUCTURE* pTDS) const;
	BOOL IsTaskDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, DWORD dwExtraCheck) const;
	CString GetTaskPath(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskPositionString(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString FormatTaskAllocTo(const TODOITEM* pTDI) const;
	CString FormatTaskCategories(const TODOITEM* pTDI) const;
	CString FormatTaskTags(const TODOITEM* pTDI) const;
	CString GetTaskCustomAttributeData(const TODOITEM* pTDI, const CString& sAttribID) const;
	BOOL CalcTaskCustomAttributeData(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, double& dValue) const;

	// Sets
	TDC_SET SetTaskDate(DWORD dwTaskID, TDC_DATE nDate, const COleDateTime& date);
	TDC_SET SetTaskColor(DWORD dwTaskID, COLORREF color);
	TDC_SET SetTaskIcon(DWORD dwTaskID, const CString& sIcon);
	TDC_SET SetTaskComments(DWORD dwTaskID, const CString& sComments, const CBinaryData& customComments = _T(""), const CString& sCommentsTypeID = _T(""));
	TDC_SET SetTaskCommentsType(DWORD dwTaskID, const CString& sCommentsTypeID);
	TDC_SET SetTaskPercent(DWORD dwTaskID, int nPercent);
	TDC_SET SetTaskTimeEstimate(DWORD dwTaskID, double dTime, TDC_UNITS nUnits);
	TDC_SET SetTaskTimeSpent(DWORD dwTaskID, double dTime, TDC_UNITS nUnits);
	TDC_SET SetTaskCost(DWORD dwTaskID, double dCost);
	TDC_SET SetTaskAllocBy(DWORD dwTaskID, const CString& sAllocBy);
	TDC_SET SetTaskStatus(DWORD dwTaskID, const CString& sStatus);
	TDC_SET SetTaskExtID(DWORD dwTaskID, const CString& sID);
	TDC_SET SetTaskPriority(DWORD dwTaskID, int nPriority); // 0-10 (10 is highest)
	TDC_SET SetTaskRisk(DWORD dwTaskID, int nRisk); // 0-10 (10 is highest)
	TDC_SET SetTaskTitle(DWORD dwTaskID, const CString& sTitle);
	TDC_SET SetTaskFlag(DWORD dwTaskID, BOOL bFlagged);
	TDC_SET SetTaskRecurrence(DWORD dwTaskID, const TDCRECURRENCE& tr);
	TDC_SET SetTaskVersion(DWORD dwTaskID, const CString& sVersion);
	TDC_SET SetTaskCustomAttributeData(DWORD dwTaskID, const CString& sAttribID, const CString& sData);

	TDC_SET SetTaskAllocTo(DWORD dwTaskID, const CStringArray& aAllocTo, BOOL bAppend);
	TDC_SET SetTaskCategories(DWORD dwTaskID, const CStringArray& aCategories, BOOL bAppend);
	TDC_SET SetTaskTags(DWORD dwTaskID, const CStringArray& aTags, BOOL bAppend);
	TDC_SET SetTaskDependencies(DWORD dwTaskID, const CStringArray& aDepends, BOOL bAppend);
	TDC_SET SetTaskFileRefs(DWORD dwTaskID, const CStringArray& aFileRefs, BOOL bAppend);
	TDC_SET SetTaskArray(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, const CStringArray& aItems, BOOL bAppend);

	TDC_SET ClearTaskColor(DWORD dwTaskID) { SetTaskColor(dwTaskID, CLR_NONE); }
	TDC_SET OffsetTaskDate(DWORD dwTaskID, TDC_DATE nDate, int nAmount, TDC_UNITS nUnits, BOOL bAndSubtasks);
	TDC_SET InitMissingTaskDate(DWORD dwTaskID, TDC_DATE nDate, const COleDateTime& date, BOOL bAndSubtasks);
	TDC_SET MoveTaskDates(DWORD dwTaskID, const COleDateTime& dtNewStart);

	BOOL TaskMatches(DWORD dwTaskID, const SEARCHPARAMS& params, SEARCHRESULT& result) const;
	BOOL TaskMatches(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const SEARCHPARAMS& params, SEARCHRESULT& result) const;
	
	BOOL IsTaskTimeTrackable(DWORD dwTaskID) const;
	BOOL IsParentTaskDone(DWORD dwTaskID) const;
	BOOL IsParentTaskDone(const TODOSTRUCTURE* pTDS) const;

	TDC_SET ClearTaskAttribute(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, BOOL bAndChildren = FALSE);
	TDC_SET ClearTaskCustomAttribute(DWORD dwTaskID, const CString& sAttribID, BOOL bAndChildren = FALSE);

	BOOL ApplyLastChangeToSubtasks(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib, BOOL bIncludeBlank = TRUE);
	void ApplyLastInheritedChangeToSubtasks(DWORD dwTaskID, TDC_ATTRIBUTE nAttrib);
	void ResetRecurringSubtaskOcurrences(DWORD dwTaskID);
	
	int CompareTasks(DWORD dwTask1ID, DWORD dwTask2ID, TDC_COLUMN nSortBy, BOOL bAscending, 
					BOOL bSortDueTodayHigh, BOOL bIncStartTime = FALSE, BOOL bIncDueTime = FALSE, BOOL bIncDoneTime = FALSE) const;
	int CompareTasks(DWORD dwTask1ID, DWORD dwTask2ID, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bAscending) const;

	int FindTasks(const SEARCHPARAMS& params, CResultArray& aResults) const;
	int FindTasks(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const SEARCHPARAMS& params, CResultArray& aResults) const;
	inline BOOL HasStyle(int nStyle) const { return m_aStyles[nStyle] ? TRUE : FALSE; }
	
	static void SetDefaultCommentsFormat(const CString& format);
	static void SetDefaultTimeUnits(TDC_UNITS nTimeEstUnits, TDC_UNITS nTimeSpentUnits);
	static void SetInheritedParentAttributes(const CTDCAttributeMap& mapAttribs, BOOL bUpdateAttrib);
	static BOOL WantUpdateInheritedAttibute(TDC_ATTRIBUTE nAttrib);
	static DH_UNITS MapUnitsToDHUnits();

protected:
	CMapIDToTDI m_mapID2TDI; // the real data
	const CWordArray& m_aStyles; // CToDoCtrl styles
	CToDoCtrlUndo m_undo;
	CToDoCtrlStructure m_struct;

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

	BOOL FindTaskLocalDependency(DWORD dwTaskID, DWORD dwDependsID, CID2IDMap& mapVisited) const;
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

	BOOL TaskMatches(const COleDateTime& date, const SEARCHPARAM& sp, SEARCHRESULT& result, BOOL bIncludeTime, TDC_DATE nDate) const;
	BOOL TaskMatches(const CString& sText, const SEARCHPARAM& sp, SEARCHRESULT& result, BOOL bMatchAsArray) const;
	BOOL TaskMatches(double dValue, const SEARCHPARAM& sp, SEARCHRESULT& result) const;
	BOOL TaskMatches(int nValue, const SEARCHPARAM& sp, SEARCHRESULT& result) const;
	BOOL TaskMatches(const CStringArray& aItems, const SEARCHPARAM& sp, SEARCHRESULT& result) const;
	BOOL TaskMatches(const TDCCADATA& data, DWORD dwAttribType, const SEARCHPARAM& sp, SEARCHRESULT& result) const;
	BOOL TaskCommentsMatch(const TODOITEM* pTDI, const SEARCHPARAM& sp, SEARCHRESULT& result) const;

	double SumPercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double SumWeightedPercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double CalcTaskTimeEstimate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS nUnits, double& dWeightedEstimate) const;
	double CalcStartDueDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bCheckChildren, BOOL bDue, BOOL bEarliest) const;
	BOOL CalcMissingStartDateFromDue(TODOITEM* pTDI) const;
	BOOL CalcMissingDueDateFromStart(TODOITEM* pTDI) const;

	int GetTaskLeafCount(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bIncludeDone) const;
	BOOL LocateTask(DWORD dwTaskID, TODOSTRUCTURE*& pTDSParent, int& nPos) const;

	// internal non-const versions
	TODOITEM* GetTask(const TODOSTRUCTURE* pTDS, BOOL bTrue = TRUE) const;
	TODOITEM* GetTask(DWORD& dwTaskID, BOOL bTrue = TRUE);

	BOOL ApplyLastChangeToSubtasks(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_ATTRIBUTE nAttrib, BOOL bIncludeBlank);

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

	BOOL HasDueTodayTasks(const TODOSTRUCTURE* pTDS) const;
	BOOL Locate(DWORD dwParentID, DWORD dwPrevSiblingID, TODOSTRUCTURE*& pTDSParent, int& nPos) const;
	int MoveTask(TODOSTRUCTURE* pTDSSrcParent, int nSrcPos, DWORD dwSrcPrevSiblingID,
							 TODOSTRUCTURE* pTDSDestParent, int nDestPos);
	BOOL SetTaskModified(DWORD dwTaskID);
	CString FormatResultDate(const COleDateTime& date) const;

	static int Compare(const COleDateTime& date1, const COleDateTime& date2, BOOL bIncTime, TDC_DATE nDate);
	static int Compare(const CString& sText1, const CString& sText2, BOOL bCheckEmpty = FALSE);
	static int Compare(int nNum1, int nNum2);
	static int Compare(double dNum1, double dNum2);

	static double GetBestDate(double dBest, double dDate, BOOL bEarliest);
	static double CalcDuration(const COleDateTime& dateStart, const COleDateTime& dateDue, TDC_UNITS nUnits);
	static COleDateTime AddDuration(COleDateTime& dateStart, double dDuration, TDC_UNITS nUnits);
	static BOOL IsEndOfDay(const COleDateTime& date);

};

#endif // !defined(AFX_TODOCTRLDATA_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
