// ToDoCtrlData.h: interface for the CToDoCtrlData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TODOCTRLDATAUTILS_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
#define AFX_TODOCTRLDATAUTILS_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tdcreminder.h"
#include "ToDoCtrlData.h"

#include "..\Interfaces\ContentMgr.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

class CBinaryData;
class CToDoCtrl;
class CTaskFile;
class CContentMgr;
class CTDLTaskCtrlBase;

class TODOSTRUCTURE;
struct TODOITEM;

typedef void* HTASKITEM;

//////////////////////////////////////////////////////////////////////

class CTDCDataHelperBase
{
protected:
	CTDCDataHelperBase(const CToDoCtrlData& data) : m_data(data) {}

protected:
	const CToDoCtrlData& m_data;

protected:
	const CTDCCustomAttribDefinitionArray& CustomAttribDefs() const { return m_data.m_aCustomAttribDefs; }

	BOOL HasStyle(TDC_STYLE nStyle) const { return m_data.HasStyle(nStyle); }
};

//////////////////////////////////////////////////////////////////////

class CTDCTaskCalculator : protected CTDCDataHelperBase
{
public:
	CTDCTaskCalculator(const CToDoCtrlData& data);

	BOOL HasOverdueTasks() const;
	BOOL HasDueTodayTasks() const;
	BOOL HasLockedTasks() const;
	double GetEarliestDueDate() const;

	BOOL IsTaskRecentlyModified(DWORD dwTaskID) const;
	BOOL IsTaskRecurring(DWORD dwTaskID) const;
	BOOL IsTaskFlagged(DWORD dwTaskID) const;
	BOOL IsTaskLocked(DWORD dwTaskID) const;
	BOOL IsTaskDone(DWORD dwTaskID, DWORD dwExtraCheck = TDCCHECKALL) const;
	BOOL IsTaskStarted(DWORD dwTaskID, BOOL bToday = FALSE) const;
	BOOL IsTaskDueToday(DWORD dwTaskID) const;
	BOOL IsTaskOverDue(DWORD dwTaskID) const;
	BOOL IsTaskTimeTrackable(DWORD dwTaskID) const;

	double GetTaskDueDate(DWORD dwTaskID) const;
	double GetTaskStartDate(DWORD dwTaskID) const;
	double GetTaskLastModifiedDate(DWORD dwTaskID) const;
	double GetTaskCost(DWORD dwTaskID) const;
	double GetTaskTimeEstimate(DWORD dwTaskID, TDC_UNITS nUnits) const;
	double GetTaskTimeSpent(DWORD dwTaskID, TDC_UNITS nUnits) const;
	double GetTaskTimeRemaining(DWORD dwTaskID, TDC_UNITS& nUnits) const;
	double GetTaskSubtaskCompletion(DWORD dwTaskID) const;
	CString GetTaskLastModifiedBy(DWORD dwTaskID) const;
	int GetTaskPriority(DWORD dwTaskID, BOOL bCheckOverdue) const;
	int GetTaskRisk(DWORD dwTaskID) const;
	int GetTaskPercentDone(DWORD dwTaskID) const;
	BOOL GetTaskCustomAttributeData(DWORD dwTaskID, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, double& dValue, TDC_UNITS nUnits = TDCU_NULL) const;
	BOOL GetTaskSubtaskTotals(DWORD dwTaskID, int& nSubtasksTotal, int& nSubtasksDone) const;

	BOOL IsParentTaskDone(DWORD dwTaskID) const;
	BOOL IsTaskRecentlyModified(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	BOOL IsTaskFlagged(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	BOOL IsTaskLocked(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	BOOL IsTaskStarted(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bToday = FALSE) const;
	BOOL IsTaskDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, DWORD dwExtraCheck = TDCCHECKALL) const;
	BOOL IsTaskDueToday(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	BOOL IsTaskOverDue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;

	double GetTaskDueDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double GetTaskStartDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double GetTaskLastModifiedDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double GetTaskCost(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double GetTaskTimeEstimate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS nUnits) const;
	double GetTaskTimeRemaining(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS& nUnits) const;
	double GetTaskTimeSpent(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS nUnits) const;
	double GetTaskSubtaskCompletion(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskLastModifiedBy(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	int GetTaskPriority(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bCheckOverdue) const;
	int GetTaskRisk(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	int GetTaskPercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	int GetPercentFromTime(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const; // spent / estimate
	BOOL GetTaskSubtaskTotals(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, int& nSubtasksTotal, int& nSubtasksDone) const;
	BOOL GetTaskCustomAttributeData(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, double& dValue, TDC_UNITS nUnits = TDCU_NULL) const;
	TDC_UNITS GetBestTimeEstUnits(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	TDC_UNITS GetBestTimeSpentUnits(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	TDC_UNITS GetTaskCustomAttributeUnits(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef) const;

	BOOL IsAggregatedAttribute(TDC_ATTRIBUTE nAttribID) const;
	BOOL HasAggregatedAttribute(const CTDCAttributeMap& mapAttribIDs) const;

protected:
	BOOL IsParentTaskDone(const TODOSTRUCTURE* pTDS) const;
	BOOL IsTaskDue(DWORD dwTaskID, BOOL bToday) const;
	BOOL IsTaskDue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bToday) const;
	BOOL GetSubtask(const TODOSTRUCTURE* pTDSParent, int nSubtask, const TODOITEM*& pTDIChild, const TODOSTRUCTURE*& pTDSChild) const;

	BOOL DoCustomAttributeCalculation(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, double& dResult, TDC_UNITS nUnits, BOOL bAggregated) const;
	BOOL GetFirstCustomAttributeOperandValue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTECALCULATION& calc, double& dValue, TDC_UNITS nUnits, BOOL bAggregated) const;
	BOOL GetSecondCustomAttributeOperandValue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTECALCULATION& calc, double& dValue, TDC_UNITS nUnits, BOOL bAggregated) const;
	BOOL CustomAttributeOperandDerivesFromDueDate(const TDCCUSTOMATTRIBUTECALCULATIONOPERAND& op) const;
	
	BOOL GetTaskCustomAttributeOperandValue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTECALCULATIONOPERAND& op, double& dValue, TDC_UNITS nUnits, BOOL bAggregated) const;
	BOOL GetTaskCustomAttributeOperandValue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_ATTRIBUTE nAttribID, double& dValue, TDC_UNITS nUnits, BOOL bAggregated) const;
	BOOL GetTaskCustomAttributeOperandValue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, double& dValue, TDC_UNITS nUnits, BOOL bAggregated) const;

	// Recursive functions hardened against stack overflows
	BOOL IsTaskFlagged(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CDWordSet& mapProcessedIDs) const;
	BOOL IsTaskRecentlyModified(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CDWordSet& mapProcessedIDs) const;
	BOOL HasDueTodayTasks(const TODOSTRUCTURE* pTDS, CDWordSet& mapProcessedIDs) const;
	BOOL HasLockedTasks(const TODOSTRUCTURE* pTDS, CDWordSet& mapProcessedIDs) const;
	BOOL GetTaskSubtaskTotals(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, int& nSubtasksTotal, int& nSubtasksDone, CDWordSet& mapProcessedIDs) const;
	BOOL GetTaskCustomAttributeData(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, double& dValue, TDC_UNITS nUnits, CDWordSet& mapProcessedIDs) const;

	double GetTaskCost(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CDWordSet& mapProcessedIDs) const;
	double GetTaskTimeEstimate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS nUnits, double& dWeightedEstimate, CDWordSet& mapProcessedIDs) const;
	double GetTaskTimeSpent(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS nUnits, CDWordSet& mapProcessedIDs) const;
	double GetAveragePercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CDWordSet& mapProcessedIDs) const;
	double GetWeightedAveragePercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CDWordSet& mapProcessedIDs) const;
	double GetStartDueDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bCheckChildren, BOOL bDue, BOOL bEarliest, CDWordSet& mapProcessedIDs) const;
	int GetTaskPriority(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bCheckOverdue, CDWordSet& mapProcessedIDs) const;
	int GetTaskRisk(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CDWordSet& mapProcessedIDs) const;
	int GetTaskLeafCount(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bIncludeDone, CDWordSet& mapProcessedIDs) const;
	const TODOITEM* GetLastModifiedTask(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CDWordSet& mapProcessedIDs) const;

	static double GetLatestDate(double dDate1, double dDate2, BOOL bNoTimeIsEndOfDay);
	static double GetEarliestDate(double dDate1, double dDate2, BOOL bNoTimeIsEndOfDay);
};

//////////////////////////////////////////////////////////////////////

enum // GetTaskTitlePaths
{
	TDCTF_TITLEANDPATH	= 0x0,
	TDCTF_TITLEONLY		= 0x1,
	TDCTF_PATHONLY		= 0x2,
	TDCTF_TRAILINGID	= 0x4,
};

// -------------------------------------------------------------------

class CTDCTaskFormatter : protected CTDCDataHelperBase
{
public:
	CTDCTaskFormatter(const CToDoCtrlData& data, const CContentMgr& mgrContent);
	
	CString GetTaskTitlePath(DWORD dwTaskID, DWORD dwFlags) const;
	CString GetTaskPath(DWORD dwTaskID, int nMaxLen = -1) const;
	CString GetTaskSubtaskCompletion(DWORD dwTaskID) const;
	CString GetTaskPosition(DWORD dwTaskID) const; 
	CString GetTaskTimeEstimate(DWORD dwTaskID) const;
	CString GetTaskTimeSpent(DWORD dwTaskID) const;
	CString GetTaskTimeRemaining(DWORD dwTaskID) const;
	CString GetTaskPercentDone(DWORD dwTaskID) const;
	CString GetTaskCommentsSizeInKB(DWORD dwTaskID) const;
	CString GetTaskCommentsFormat(DWORD dwTaskID, BOOL bEmptyIsBlank = TRUE) const;
	CString GetTaskCost(DWORD dwTaskID) const;
	CString GetTaskPriority(DWORD dwTaskID, BOOL bCheckOverdue) const;
	CString GetTaskRisk(DWORD dwTaskID) const;
	CString GetTaskStatus(DWORD dwTaskID, const CString& sCompletionStatus) const;
	CString GetTaskRecentlyModified(DWORD dwTaskID) const;
	CString GetTaskRecurrence(DWORD dwTaskID) const;
	CString GetTaskCustomAttributeData(DWORD dwTaskID, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef) const;
	CString GetTaskDoneDate(DWORD dwTaskID) const;
	CString GetTaskDueDate(DWORD dwTaskID) const;
	CString GetTaskStartDate(DWORD dwTaskID) const;
	CString GetTaskCreationDate(DWORD dwTaskID) const;
	CString GetTaskLastModDate(DWORD dwTaskID) const;

	CString GetTaskAllocTo(DWORD dwTaskID, TCHAR cSep = 0) const;
	CString GetTaskCategories(DWORD dwTaskID, TCHAR cSep = 0) const;
	CString GetTaskTags(DWORD dwTaskID, TCHAR cSep = 0) const;
	CString GetTaskDependencies(DWORD dwTaskID, TCHAR cSep = 0) const;
	CString GetTaskDependents(DWORD dwTaskID, TCHAR cSep = 0) const;
	CString GetTaskFileLinks(DWORD dwTaskID, TCHAR cSep = 0) const;

	CString GetTaskTitlePath(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, DWORD dwFlags) const;
	CString GetTaskSubtaskCompletion(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskPath(const TODOSTRUCTURE* pTDS) const;
	CString GetTaskPosition(const TODOSTRUCTURE* pTDS) const;
	CString GetTaskTimeEstimate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskTimeSpent(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskTimeRemaining(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskPercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskCommentsSizeInKB(const TODOITEM* pTDI) const;
	CString GetTaskCommentsFormat(const TODOITEM* pTDI, BOOL bEmptyIsBlank = TRUE) const;
	CString GetTaskCost(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskPriority(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bCheckOverdue) const;
	CString GetTaskRisk(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskStatus(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const CString& sCompletionStatus) const;
	CString GetTaskRecentlyModified(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskRecurrence(const TODOITEM* pTDI) const;
	CString GetTaskCustomAttributeData(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef) const;
	CString GetTaskDoneDate(const TODOITEM* pTDI) const;
	CString GetTaskDueDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskStartDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskCreationDate(const TODOITEM* pTDI) const;
	CString GetTaskLastModDate(const TODOITEM* pTDI) const;

	CString GetTaskAllocTo(const TODOITEM* pTDI, TCHAR cSep = 0) const;
	CString GetTaskCategories(const TODOITEM* pTDI, TCHAR cSep = 0) const;
	CString GetTaskTags(const TODOITEM* pTDI, TCHAR cSep = 0) const;
	CString GetTaskDependencies(const TODOITEM* pTDI, TCHAR cSep = 0) const;
	CString GetTaskFileLinks(const TODOITEM* pTDI, TCHAR cSep = 0) const;

	CString GetTaskTimePeriod(DWORD dwTaskID, TDC_COLUMN nColID) const;
	CString GetTaskTimePeriod(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_COLUMN nColID) const;

	CString GetTaskDate(DWORD dwTaskID, TDC_COLUMN nColID) const;
	CString GetTaskDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_COLUMN nColID) const;

	void GetTaskTitlePaths(const CDWordArray& aTaskIDs, DWORD dwFlags, CStringArray& aTitlePaths) const;
	CString GetTaskTitlePaths(const CDWordArray& aTaskIDs, DWORD dwFlags, TCHAR cSep = 0) const;

	// Helpers
	CString GetDateTime(const COleDateTime& date, BOOL bAllowTime = TRUE) const;
	CString GetDateOnly(const COleDateTime& date, BOOL bWantYear) const;
	CString GetTimeOnly(const COleDateTime& date, TDC_DATE nDate) const;
	CString GetCost(double dCost) const;
	CString GetDependencies(const CTDCDependencyArray& aDepends, TCHAR cSep = 0) const;
	CString GetTimePeriod(double dTime, TDC_UNITS nUnits, BOOL bAllowNegative) const;
	
	CString GetCommentSizeInKB(float fSize) const;
	CString GetID(DWORD dwTaskID, DWORD dwRefID = 0) const;

	BOOL WantFormatValue(double dValue, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef) const;
	
protected:
	const CContentMgr& m_mgrContent;

	CTDCTaskCalculator m_calculator;
};

//////////////////////////////////////////////////////////////////////

class CTDCTaskMatcher : protected CTDCDataHelperBase
{
public:
	CTDCTaskMatcher(const CToDoCtrlData& data, const CTDCReminderHelper& reminders, const CContentMgr& mgrContent);

	int FindTasks(TDC_ATTRIBUTE nAttribID, FIND_OPERATOR nOp, CString sValue, BOOL bCheckDueToday, CResultArray& aResults) const;
	int FindTasks(const SEARCHPARAMS& query, BOOL bCheckDueToday, CResultArray& aResults) const;
	int FindTasks(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const SEARCHPARAMS& query, BOOL bCheckDueToday, CResultArray& aResults) const;

	int FindTasks(TDC_ATTRIBUTE nAttribID, FIND_OPERATOR nOp, CString sValue, BOOL bCheckDueToday, CDWordArray& aTaskIDs) const;
	int FindTasks(const SEARCHPARAMS& query, BOOL bCheckDueToday, CDWordArray& aTaskIDs) const;
	int FindTasks(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const SEARCHPARAMS& query, BOOL bCheckDueToday, CDWordArray& aTaskIDs) const;

	BOOL TaskMatches(DWORD dwTaskID, const SEARCHPARAMS& query, BOOL bCheckDueToday, SEARCHRESULT& result) const;
	BOOL TaskMatches(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const SEARCHPARAMS& query, BOOL bCheckDueToday, SEARCHRESULT& result) const;

	static int Convert(const CResultArray& aResults, CDWordArray& aTaskIDs);

protected:
	const CTDCReminderHelper& m_reminders;
	const CContentMgr& m_mgrContent;

	CTDCTaskCalculator m_calculator;
	CTDCTaskFormatter m_formatter;

protected:
	BOOL TitleOrCommentsMatches(const TODOITEM* pTDI, const SEARCHPARAM& rule, BOOL bCaseSensitive, CString& sWhatMatched, TDC_ATTRIBUTE &nWhatMatched) const;
	BOOL AnyTextAttributeMatches(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const SEARCHPARAM& rule,
								 const CTDCCustomAttribDefinitionArray& aAttribDefs, BOOL bCaseSensitive,
								 CString& sWhatMatched, TDC_ATTRIBUTE& nWhatMatched) const;
	BOOL AnyTaskParentMatches(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const SEARCHPARAMS& query, BOOL bCheckDueToday, SEARCHRESULT& result) const;

	BOOL ValueMatches(const COleDateTime& date, const SEARCHPARAM& rule, BOOL bIncludeTime, TDC_DATE nDate, CString& sWhatMatched) const;
	BOOL ValueMatches(double dValue, const SEARCHPARAM& rule, CString& sWhatMatched) const;
	BOOL ValueMatches(int nValue, const SEARCHPARAM& rule, CString& sWhatMatched) const;
	BOOL ValueMatches(const CString& sComments, const CBinaryData& customComments, const SEARCHPARAM& rule, CString& sWhatMatched) const;
	BOOL ValueMatches(const TDCRECURRENCE& trRecurrence, const SEARCHPARAM& rule, CString& sWhatMatched) const;
	BOOL PriorityRiskValueMatches(int nValue, const SEARCHPARAM& rule, CString& sWhatMatched) const;

	BOOL ValueMatches(const CString& sText, const SEARCHPARAM& rule, BOOL bCaseSensitive, CString& sWhatMatched) const;
	BOOL ValueMatches(const TDCCADATA& data, DWORD dwAttribType, const SEARCHPARAM& rule, BOOL bCaseSensitive, CString& sWhatMatched) const;

	BOOL ArrayMatches(const CStringArray& aItems, const SEARCHPARAM& rule, BOOL bCaseSensitive, CString& sWhatMatched) const;
	BOOL ValueMatchesAsArray(const CString& sText, const SEARCHPARAM& rule, BOOL bCaseSensitive, CString& sWhatMatched) const;

	CString FormatResultDate(const COleDateTime& date) const;

};

//////////////////////////////////////////////////////////////////////

class CTDCTaskComparer : protected CTDCDataHelperBase
{
public:
	CTDCTaskComparer(const CToDoCtrlData& data, const CContentMgr& mgrContent);

	int CompareTasks(DWORD dwTask1ID, DWORD dwTask2ID, TDC_COLUMN nSortBy, BOOL bAscending, BOOL bCheckDueToday, BOOL bIncTime = FALSE) const;
	int CompareTasks(DWORD dwTask1ID, DWORD dwTask2ID, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bAscending) const;

protected:
	CTDCTaskCalculator m_calculator;
	CTDCTaskFormatter m_formatter;

protected:
	BOOL CheckGetIsTaskDone(BOOL bDone, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	COleDateTime CalcTaskDueDate(BOOL bDone, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	COleDateTime CalcTaskStartDate(BOOL bDone, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	COleDateTime CalcTaskDoneDate(BOOL bDone, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	int CalcTaskPriority(BOOL bCheckDueToday, BOOL bDone, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	int CalcTaskRisk(BOOL bDone, const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;

	static int Compare(const COleDateTime& date1, const COleDateTime& date2, BOOL bIncTime, TDC_DATE nDate);
	static int Compare(const CString& sText1, const CString& sText2, BOOL bCheckEmpty = FALSE);
	static int Compare(int nNum1, int nNum2);
	static int Compare(double dNum1, double dNum2);
	static int Compare(DWORD dwNum1, DWORD dwNum2);
};

//////////////////////////////////////////////////////////////////////

class CTDCTaskExporter : protected CTDCDataHelperBase
{
public:
	CTDCTaskExporter(const CToDoCtrlData& data,
					 const CTDLTaskCtrlBase& colors,
					 const CContentMgr& comments);

	int ExportAllTasks(CTaskFile& tasks, BOOL bIncDuplicateCompletedRecurringSubtasks = TRUE) const;

	HTASKITEM ExportTask(DWORD dwTaskID, CTaskFile& tasks, HTASKITEM hParentTask, BOOL bIncDuplicateCompletedRecurringSubtasks = TRUE) const;
	HTASKITEM ExportTrueTask(DWORD dwTaskID, CTaskFile& tasks, HTASKITEM hParentTask, BOOL bIncDuplicateCompletedRecurringSubtasks = TRUE) const;
	HTASKITEM ExportTask(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CTaskFile& tasks, HTASKITEM hParentTask, BOOL bIncDuplicateCompletedRecurringSubtasks = TRUE) const;
	HTASKITEM ExportSiblingTask(DWORD dwTaskID, CTaskFile& tasks, HTASKITEM hPrevSiblingTask, BOOL bIncDuplicateCompletedRecurringSubtasks = TRUE) const;
	BOOL ExportSubTasks(const TODOSTRUCTURE* pTDSParent, CTaskFile& tasks, HTASKITEM hParentTask, BOOL bIncDuplicateCompletedRecurringSubtasks = TRUE) const;

	BOOL ExportAllTaskAttributes(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CTaskFile& tasks, HTASKITEM hTask) const;
	BOOL ExportMatchingTaskAttributes(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CTaskFile& tasks, HTASKITEM hTask, const TDCGETTASKS& filter) const;
	BOOL ExportMatchingTaskAttributes(DWORD dwTaskID, CTaskFile& tasks, HTASKITEM hTask, const TDCGETTASKS& filter) const;

protected:
	const CTDLTaskCtrlBase& m_colors;
	const CContentMgr& m_mgrContent;

	CTDCTaskCalculator m_calculator;
	CTDCTaskFormatter m_formatter;

protected:
	COLORREF GetTaskTextColor(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	COLORREF GetPriorityColor(int nPriority) const;

	HTASKITEM ExportTaskEx(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CTaskFile& tasks, HTASKITEM hParentTask, HTASKITEM hPrevSiblingTask, BOOL bIncDuplicateCompletedRecurringSubtasks = TRUE) const;

	void ExportMatchingTaskCustomAttributes(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CTaskFile& tasks, HTASKITEM hTask, const TDCGETTASKS& filter) const;
	void ExportAllCalculatedTaskCustomAttributes(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CTaskFile& tasks, HTASKITEM hTask) const;
	void ExportCalculatedTaskCustomAttribute(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, CTaskFile& tasks, HTASKITEM hTask) const;
};

//////////////////////////////////////////////////////////////////////

class CTDCMultiTasker : protected CTDCDataHelperBase
{
public:
	CTDCMultiTasker(const CToDoCtrlData& data, const CContentMgr& mgrContent);

	// These functions return FALSE if two or more tasks have different values
	BOOL GetTasksDate(const CDWordArray& aTaskIDs, TDC_DATE nDate, COleDateTime& dtValue) const;

	BOOL GetTasksTitle(const CDWordArray& aTaskIDs, CString& sValue) const;
	BOOL GetTasksLastModifiedBy(const CDWordArray& aTaskIDs, CString& sValue) const;
	BOOL GetTasksCreatedBy(const CDWordArray& aTaskIDs, CString& sValue) const;
	BOOL GetTasksAllocatedBy(const CDWordArray& aTaskIDs, CString& sValue) const;
	BOOL GetTasksVersion(const CDWordArray& aTaskIDs, CString& sValue) const;
	BOOL GetTasksExternalID(const CDWordArray& aTaskIDs, CString& sValue) const;
	BOOL GetTaskPosition(const CDWordArray& aTaskIDs, CString& sValue) const;
	BOOL GetTasksStatus(const CDWordArray& aTaskIDs, CString& sValue) const;
	BOOL GetTasksIcon(const CDWordArray& aTaskIDs, CString& sValue) const;
	BOOL GetTasksPath(const CDWordArray& aTaskIDs, CString& sValue, BOOL bWithTaskName = FALSE) const;
	BOOL GetTasksCommentsFormat(const CDWordArray& aTaskIDs, CString& sValue) const;
	BOOL GetTasksCommentsSize(const CDWordArray& aTaskIDs, CString& sValue) const;
	BOOL GetTasksSubtaskCompletion(const CDWordArray& aTaskIDs, CString& sValue) const;

	BOOL GetTasksParentID(const CDWordArray& aTaskIDs, DWORD& dwValue) const;
	BOOL GetTasksPriority(const CDWordArray& aTaskIDs, int& nValue) const;
	BOOL GetTasksCommentLength(const CDWordArray& aTaskIDs, int& nValue) const;
	BOOL GetTasksRisk(const CDWordArray& aTaskIDs, int& nValue) const;
	BOOL GetTasksPercentDone(const CDWordArray& aTaskIDs, int& nValue) const;
	BOOL GetTasksFlagState(const CDWordArray& aTaskIDs, BOOL& bValue) const;
	BOOL GetTasksLockState(const CDWordArray& aTaskIDs, BOOL& bValue) const;
	BOOL GetTasksColor(const CDWordArray& aTaskIDs, COLORREF& crValue) const;

	BOOL GetTasksTimeEstimate(const CDWordArray& aTaskIDs, TDCTIMEPERIOD& period) const;
	BOOL GetTasksTimeSpent(const CDWordArray& aTaskIDs, TDCTIMEPERIOD& period) const;
	BOOL GetTasksTimeRemaining(const CDWordArray& aTaskIDs, TDCTIMEPERIOD& period) const;
	BOOL GetTasksCost(const CDWordArray& aTaskIDs, TDCCOST& cost) const;
	BOOL GetTasksRecurrence(const CDWordArray& aTaskIDs, TDCRECURRENCE& recurs) const;

	BOOL GetTasksCategories(const CDWordArray& aTaskIDs, CStringArray& aValues) const;
	BOOL GetTasksTags(const CDWordArray& aTaskIDs, CStringArray& aValues) const;
	BOOL GetTasksFileLinks(const CDWordArray& aTaskIDs, CStringArray& aValues) const;
	BOOL GetTasksAllocatedTo(const CDWordArray& aTaskIDs, CStringArray& aValues) const;
	BOOL GetTasksDependencies(const CDWordArray& aTaskIDs, CTDCDependencyArray& aDepends) const;

	BOOL GetTasksCustomAttributeData(const CDWordArray& aTaskIDs, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, TDCCADATA& data) const;
	BOOL GetTasksReminder(const CDWordArray& aTaskIDs, const CTDCReminderHelper& rems, time_t& tValue) const;

	// These functions return the number of matching values
	int GetTasksCategories(const CDWordArray& aTaskIDs, CStringArray& aMatched, CStringArray& aMixed) const;
	int GetTasksTags(const CDWordArray& aTaskIDs, CStringArray& aMatched, CStringArray& aMixed) const;
	int GetTasksAllocatedTo(const CDWordArray& aTaskIDs, CStringArray& aMatched, CStringArray& aMixed) const;

	// Simple query functions
	BOOL AnyTaskHasDependencies(const CDWordArray& aTaskIDs) const;
	BOOL AnyTaskHasLocalCircularDependencies(const CDWordArray& aTaskIDs) const;
	BOOL AnyTaskHasDate(const CDWordArray& aTaskIDs, TDC_DATE nDate) const;
	BOOL AnyTaskHasDependents(const CDWordArray& aTaskIDs) const;
	BOOL AnyTaskHasIcon(const CDWordArray& aTaskIDs) const;
	BOOL AnyTaskHasLockedParent(const CDWordArray& aTaskIDs, BOOL bTreatRefsAsUnlocked = FALSE) const;
	BOOL AnyTaskHasColor(const CDWordArray& aTaskIDs) const;
	BOOL AnyTaskHasID(const CDWordArray& aTaskIDs, DWORD dwTaskID, BOOL bIncludeRefs) const;
	BOOL AnyTaskIsReference(const CDWordArray& aTaskIDs) const;
	BOOL AnyTaskIsParent(const CDWordArray& aTaskIDs) const;
	BOOL AnyTaskIsRecurring(const CDWordArray& aTaskIDs) const;
	BOOL AnyTaskIsLocked(const CDWordArray& aTaskIDs, BOOL bTreatRefsAsUnlocked = FALSE) const;
	BOOL AnyTaskIsUnlocked(const CDWordArray& aTaskIDs, BOOL bTreatRefsAsUnlocked = FALSE) const;
	BOOL AnyTaskIsFlagged(const CDWordArray& aTaskIDs) const;
	BOOL AnyTaskIsDone(const CDWordArray& aTaskIDs, BOOL bIncGoodAsDone) const;

	BOOL AllTasksAreReferences(const CDWordArray& aTaskIDs) const;
	BOOL AllTasksAreDone(const CDWordArray& aTaskIDs, BOOL bIncGoodAsDone) const;
	BOOL AllTasksHaveDate(const CDWordArray& aTaskIDs, TDC_DATE nDate) const;
	BOOL AllTasksAreParents(const CDWordArray& aTaskIDs) const;
	BOOL AllTasksHaveSameParent(const CDWordArray& aTaskIDs) const;
	BOOL AllTasksHaveDependencies(const CDWordArray& aTaskIDs) const;

	// Returns -1 if attribute not handled
	BOOL CanEditAnyTask(const CDWordArray& aTaskIDs, TDC_ATTRIBUTE nAttribID) const;

protected:
	CTDCTaskFormatter m_formatter;
	CTDCTaskCalculator m_calculator;

protected:
	static int SplitSelectedTaskArrayMatchCounts(const CMap<CString, LPCTSTR, int, int&>& mapCounts, int nNumTasks,
												 CStringArray& aMatched, CStringArray& aMixed);
};

//////////////////////////////////////////////////////////////////////

class CTDCLongestItemMap : public CMap<TDC_COLUMN, TDC_COLUMN, CString, LPCTSTR>
{
public:
	BOOL Initialise(const CTDCColumnIDMap& mapCols, const CTDCCustomAttribDefinitionArray& aCustAttribDefs);
	BOOL CheckUpdateValue(TDC_COLUMN nColID, const CString& sValue);
	BOOL CheckUpdateValue(TDC_COLUMN nColID, const CStringArray& aValues);
	BOOL UpdateValue(TDC_COLUMN nColID, const CString& sValue);
	BOOL UpdateValue(TDC_COLUMN nColID, int nValue);
	BOOL HasColumn(TDC_COLUMN nColID) const;
	CString GetLongestValue(TDC_COLUMN nColID) const;

	static BOOL IsSupported(TDC_COLUMN nColID);
	static BOOL IsSupported(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef);
};

// --------------------------------------------------------------------

class CTDCTaskColumnSizer : protected CTDCDataHelperBase
{
public:
	CTDCTaskColumnSizer(const CToDoCtrlData& data,
						const CContentMgr& mgrContent);
	// general
	int GetLongestValues(const CTDCColumnIDMap& mapCols, const CDWordArray& aTaskIDs, CTDCLongestItemMap& mapLongest) const;

	CString GetLongestValue(TDC_COLUMN nColID, const CDWordArray& aTaskIDs) const;
	CString GetLongestValue(TDC_COLUMN nColID, const CStringArray& aPossible, const CDWordArray& aTaskIDs) const;
	CString GetLongestTime(TDC_COLUMN nColID, const CDWordArray& aTaskIDs) const;
	CString GetLongestValue(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, const CDWordArray& aTaskIDs) const;

	// specific
	CString GetLongestTimeEstimate(const CDWordArray& aTaskIDs) const;
	CString GetLongestTimeSpent(const CDWordArray& aTaskIDs) const;
	CString GetLongestTimeRemaining(const CDWordArray& aTaskIDs) const;
	CString GetLargestCommentsSizeInKB(const CDWordArray& aTaskIDs) const;

	DWORD GetLargestReferenceID(const CDWordArray& aTaskIDs) const;
	int GetLargestFileLinkCount(const CDWordArray& aTaskIDs) const;
	int GetLargestCustomAttributeArraySize(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, const CDWordArray& aTaskIDs) const;

protected:
	const CContentMgr& m_mgrContent;

	CTDCTaskFormatter m_formatter;
	CTDCTaskCalculator m_calculator;

protected:
	// general
	CString GetLongestValue(TDC_COLUMN nColID, const CString& sLongestPossible, const CDWordArray& aTaskIDs) const;

	// specific
	CString GetLongestSubtaskDone(const CDWordArray& aTaskIDs) const;
	CString GetLongestPosition(const CDWordArray& aTaskIDs) const;
	CString GetLongestPath(const CDWordArray& aTaskIDs) const;
	CString GetLongestCost(const CDWordArray& aTaskIDs) const;

	BOOL GetLongestAggregatedValue(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, const CDWordArray& aTaskIDs, CString& sLongest) const;

	static CString GetLongerString(const CString& str1, const CString& str2);
	static BOOL EqualsLongestPossible(const CString& sValue, const CString& sLongestPossible);
	static CString GetLongestRecurrenceOption();

	static void GetLongestValues(const TODOITEM* pTDI,
								 const TODOSTRUCTURE* pTDS,
								 const CTDCCustomAttribDefinitionArray& aCustAttribDefs,
								 const CTDCTaskFormatter& formatter,
								 CTDCLongestItemMap& mapLongest);

};


//////////////////////////////////////////////////////////////////////

class CTDCTaskAttributeCopier : protected CTDCDataHelperBase
{
public:
	CTDCTaskAttributeCopier(const CToDoCtrlData& data,
							const CContentMgr& mgrContent);

	BOOL CanCopyAttributeValues(TDC_ATTRIBUTE nFromAttribID, TDC_ATTRIBUTE nToAttribID, BOOL bSameTasklist) const;
	BOOL CopyAttributeValue(const TODOITEM& tdiFrom, TDC_ATTRIBUTE nFromAttribID, TODOITEM& tdiTo, TDC_ATTRIBUTE nToAttribID) const;
	
	BOOL CanCopyColumnValues(TDC_COLUMN nColID) const;
	BOOL CanCopyColumnValues(TDC_COLUMN nFromColID, TDC_COLUMN nToColID, BOOL bSameTasklist) const;
	BOOL CopyColumnValue(const TODOITEM& tdiFrom, TDC_COLUMN nFromColID, TODOITEM& tdiTo, TDC_COLUMN nToColID) const;
	
	TDC_ATTRIBUTEGROUP GetAttributeGroup(TDC_ATTRIBUTE nAttribID, BOOL bResolveCustomCols = TRUE) const;

protected:
	CTDCTaskFormatter m_formatter;

protected:
	static void CopyDate(const COleDateTime& dtFrom, COleDateTime& dtTo);
};

//////////////////////////////////////////////////////////////////////


#endif // !defined(AFX_TODOCTRLDATAUTILS_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
