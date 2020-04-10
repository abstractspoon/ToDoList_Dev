// ToDoCtrlData.h: interface for the CToDoCtrlData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TODOCTRLDATAUTILS_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
#define AFX_TODOCTRLDATAUTILS_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tdcreminder.h"
#include "tdcstruct.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

class CToDoCtrlData;
class CBinaryData;
class CToDoCtrl;
class CTaskFile;
class CContentMgr;
class CTDLTaskCtrlBase;

class TODOSTRUCTURE;
struct TODOITEM;

typedef void* HTASKITEM;

//////////////////////////////////////////////////////////////////////

class CTDCTaskCalculator
{
public:
	CTDCTaskCalculator(const CToDoCtrlData& data);

	BOOL HasOverdueTasks() const;
	BOOL HasDueTodayTasks() const;
	double GetEarliestDueDate() const;
	BOOL HasLockedTasks() const;

	BOOL IsTaskRecentlyModified(DWORD dwTaskID) const;
	BOOL IsTaskRecurring(DWORD dwTaskID) const;
	BOOL IsTaskFlagged(DWORD dwTaskID) const;
	BOOL IsTaskLocked(DWORD dwTaskID) const;
	BOOL IsTaskDone(DWORD dwTaskID, DWORD dwExtraCheck = TDCCHECKALL) const;
	BOOL IsTaskStarted(DWORD dwTaskID, BOOL bToday = FALSE) const;
	BOOL IsTaskDueToday(DWORD dwTaskID) const;
	BOOL IsTaskOverDue(DWORD dwTaskID) const;

	double GetTaskDueDate(DWORD dwTaskID) const;
	double GetTaskStartDate(DWORD dwTaskID) const;
	double GetTaskLastModifiedDate(DWORD dwTaskID) const;
	CString GetTaskLastModifiedBy(DWORD dwTaskID) const;
	int GetTaskHighestPriority(DWORD dwTaskID, BOOL bIncludeDue = TRUE) const;
	int GetTaskHighestRisk(DWORD dwTaskID) const;
	int GetTaskPercentDone(DWORD dwTaskID) const;
	double GetTaskCost(DWORD dwTaskID) const;
	double GetTaskTimeEstimate(DWORD dwTaskID, TDC_UNITS nUnits) const;
	double GetTaskTimeSpent(DWORD dwTaskID, TDC_UNITS nUnits) const;
	double GetTaskRemainingTime(DWORD dwTaskID, TDC_UNITS& nUnits) const;
	BOOL GetTaskCustomAttributeData(DWORD dwTaskID, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, double& dValue, TDC_UNITS nUnits = TDCU_NULL) const;
	BOOL GetTaskSubtaskTotals(DWORD dwTaskID, int& nSubtasksTotal, int& nSubtasksDone) const;
	double GetTaskSubtaskCompletion(DWORD dwTaskID) const;

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
	CString GetTaskLastModifiedBy(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	int GetTaskHighestPriority(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bIncludeDue = TRUE) const;
	int GetTaskHighestRisk(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double GetTaskCost(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double GetTaskTimeEstimate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS nUnits) const;
	double GetTaskRemainingTime(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS& nUnits) const;
	TDC_UNITS GetBestTimeEstUnits(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double GetTaskTimeSpent(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS nUnits) const;
	TDC_UNITS GetBestTimeSpentUnits(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	int GetTaskPercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	int GetPercentFromTime(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const; // spent / estimate
	BOOL GetTaskSubtaskTotals(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, int& nSubtasksTotal, int& nSubtasksDone) const;
	double GetTaskSubtaskCompletion(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	BOOL GetTaskCustomAttributeData(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, double& dValue, TDC_UNITS nUnits = TDCU_NULL) const;

	BOOL IsCalculatedAttribute(TDC_ATTRIBUTE nAttrib, const CTDCCustomAttribDefinitionArray& aAttribDefs) const;
	BOOL HasCalculatedAttribute(const CTDCAttributeMap& mapAttribIDs, const CTDCCustomAttribDefinitionArray& aAttribDefs) const;

protected:
	const CToDoCtrlData& m_data;

protected:
	double GetPercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double GetWeightedPercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	double GetTaskTimeEstimate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_UNITS nUnits, double& dWeightedEstimate) const;
	double GetStartDueDate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bCheckChildren, BOOL bDue, BOOL bEarliest) const;
	int GetTaskLeafCount(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bIncludeDone) const;
	BOOL IsTaskReference(DWORD dwTaskID) const;
	BOOL HasDueTodayTasks(const TODOSTRUCTURE* pTDS) const;
	BOOL HasLockedTasks(const TODOSTRUCTURE* pTDS) const;
	BOOL IsParentTaskDone(const TODOSTRUCTURE* pTDS) const;
	BOOL IsTaskDue(DWORD dwTaskID, BOOL bToday) const;
	BOOL IsTaskDue(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, BOOL bToday) const;
	
	const TODOITEM* GetLastModifiedTask(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;

	static double GetBestDate(double dBest, double dDate, BOOL bEarliest);
	static BOOL GetCalculationValue(const TDCCADATA& data, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, double& dValue, TDC_UNITS nUnits);

};

//////////////////////////////////////////////////////////////////////

class CTDCTaskFormatter
{
public:
	CTDCTaskFormatter(const CToDoCtrlData& data);
	
	CString GetCommentSize(float fSize) const;
	CString GetPriority(int nPriority) const;
	CString GetRisk(int nRisk) const;
	CString GetID(DWORD dwTaskID, DWORD dwRefID = 0) const;

	CString GetTaskAllocTo(DWORD dwTaskID) const;
	CString GetTaskCategories(DWORD dwTaskID) const;
	CString GetTaskTags(DWORD dwTaskID) const;
	CString GetTaskSubtaskCompletion(DWORD dwTaskID) const;
	CString GetTaskPath(DWORD dwTaskID, int nMaxLen = -1) const; 
	CString GetTaskPosition(DWORD dwTaskID) const; 
	CString GetTaskTimeEstimate(DWORD dwTaskID) const;
	CString GetTaskTimeSpent(DWORD dwTaskID) const;
	CString GetTaskTimeRemaining(DWORD dwTaskID) const;
	CString GetTaskPercentDone(DWORD dwTaskID) const;
	CString GetTaskCommentSize(DWORD dwTaskID) const;
	CString GetTaskCost(DWORD dwTaskID) const;
	CString GetTaskPriority(DWORD dwTaskID) const;
	CString GetTaskRisk(DWORD dwTaskID) const;
	CString GetTaskStatus(DWORD dwTaskID, const CString& sCompletionStatus) const;
	CString GetTaskRecentlyModified(DWORD dwTaskID) const;
	CString GetTaskRecurrence(DWORD dwTaskID) const;
	CString GetTaskCustomAttributeData(DWORD dwTaskID, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef) const;

	CString GetTaskAllocTo(const TODOITEM* pTDI) const;
	CString GetTaskCategories(const TODOITEM* pTDI) const;
	CString GetTaskTags(const TODOITEM* pTDI) const;
	CString GetTaskSubtaskCompletion(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskPath(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskPosition(const TODOSTRUCTURE* pTDS) const;
	CString GetTaskTimeEstimate(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskTimeSpent(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskTimeRemaining(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskPercentDone(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskCommentSize(const TODOITEM* pTDI) const;
	CString GetTaskCost(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskPriority(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskRisk(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskStatus(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const CString& sCompletionStatus) const;
	CString GetTaskRecentlyModified(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	CString GetTaskRecurrence(const TODOITEM* pTDI) const;
	CString GetTaskCustomAttributeData(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef) const;

	CString GetTaskTimePeriod(DWORD dwTaskID, TDC_COLUMN nColID) const;
	CString GetTaskTimePeriod(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, TDC_COLUMN nColID) const;
	CString GetTimePeriod(double dTime, TDC_UNITS nUnits, BOOL bAllowNegative) const;

	CString GetDateOnly(const COleDateTime& date, BOOL bWantYear) const;
	CString GetTimeOnly(const COleDateTime& date, TDC_DATE nDate) const;

protected:
	const CToDoCtrlData& m_data;
	CTDCTaskCalculator m_calculator;
};

//////////////////////////////////////////////////////////////////////

class CTDCTaskMatcher
{
public:
	CTDCTaskMatcher(const CToDoCtrlData& data, const CTDCReminderHelper& reminders);

	int FindTasks(TDC_ATTRIBUTE nAttrib, FIND_OPERATOR nOp, CString sValue, CResultArray& aResults, BOOL bCheckDueToday) const;
	int FindTasks(const SEARCHPARAMS& query, CResultArray& aResults, BOOL bCheckDueToday) const;
	int FindTasks(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const SEARCHPARAMS& query, CResultArray& aResults, BOOL bCheckDueToday) const;

	int FindTasks(TDC_ATTRIBUTE nAttrib, FIND_OPERATOR nOp, CString sValue, CDWordArray& aTaskIDs, BOOL bCheckDueToday) const;
	int FindTasks(const SEARCHPARAMS& query, CDWordArray& aTaskIDs, BOOL bCheckDueToday) const;
	int FindTasks(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const SEARCHPARAMS& query, CDWordArray& aTaskIDs, BOOL bCheckDueToday) const;

	BOOL TaskMatches(DWORD dwTaskID, const SEARCHPARAMS& query, SEARCHRESULT& result, BOOL bCheckDueToday) const;
	BOOL TaskMatches(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const SEARCHPARAMS& query, SEARCHRESULT& result, BOOL bCheckDueToday) const;

	static int Convert(const CResultArray& aResults, CDWordArray& aTaskIDs);

protected:
	const CToDoCtrlData& m_data;
	const CTDCReminderHelper& m_reminders;

	CTDCTaskCalculator m_calculator;
	CTDCTaskFormatter m_formatter;

protected:
	BOOL AnyTaskParentMatches(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const SEARCHPARAMS& query, SEARCHRESULT& result, BOOL bCheckDueToday) const;

	BOOL ValueMatches(const COleDateTime& date, const SEARCHPARAM& rule, SEARCHRESULT& result, BOOL bIncludeTime, TDC_DATE nDate) const;
	BOOL ValueMatches(double dValue, const SEARCHPARAM& rule, SEARCHRESULT& result) const;
	BOOL ValueMatches(int nValue, const SEARCHPARAM& rule, SEARCHRESULT& result) const;
	BOOL ValueMatches(const CString& sComments, const CBinaryData& customComments, const SEARCHPARAM& rule, SEARCHRESULT& result) const;
	BOOL ValueMatches(const TDCRECURRENCE& trRecurrence, const SEARCHPARAM& rule, SEARCHRESULT& result) const;

	BOOL ValueMatches(const CString& sText, const SEARCHPARAM& rule, SEARCHRESULT& result, BOOL bCaseSensitive) const;
	BOOL ValueMatches(const TDCCADATA& data, DWORD dwAttribType, const SEARCHPARAM& rule, SEARCHRESULT& result, BOOL bCaseSensitive) const;

	BOOL ArrayMatches(const CStringArray& aItems, const SEARCHPARAM& rule, SEARCHRESULT& result, BOOL bCaseSensitive) const;
	BOOL ValueMatchesAsArray(const CString& sText, const SEARCHPARAM& rule, SEARCHRESULT& result, BOOL bCaseSensitive) const;

	CString FormatResultDate(const COleDateTime& date) const;

};

//////////////////////////////////////////////////////////////////////

class CTDCTaskComparer
{
public:
	CTDCTaskComparer(const CToDoCtrlData& data);

	int CompareTasks(DWORD dwTask1ID, DWORD dwTask2ID, TDC_COLUMN nSortBy, BOOL bAscending, BOOL bCheckDueToday, BOOL bIncTime = FALSE) const;
	int CompareTasks(DWORD dwTask1ID, DWORD dwTask2ID, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bAscending) const;

protected:
	const CToDoCtrlData& m_data;
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

class CTDCTaskExporter
{
public:
	CTDCTaskExporter(const CToDoCtrlData& data, 
		const CTDLTaskCtrlBase& colors,
		const CContentMgr& comments);

	int ExportAllTasks(CTaskFile& tasks, BOOL bIncDuplicateCompletedRecurringSubtasks = TRUE) const;
	int ExportCompletedTasks(CTaskFile& tasks) const;

	BOOL ExportTask(DWORD dwTaskID, CTaskFile& tasks, HTASKITEM hParentTask, BOOL bIncDuplicateCompletedRecurringSubtasks = TRUE) const;
	BOOL ExportTask(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CTaskFile& tasks, HTASKITEM hParentTask, BOOL bIncDuplicateCompletedRecurringSubtasks = TRUE) const;
	BOOL ExportSubTasks(const TODOSTRUCTURE* pTDSParent, CTaskFile& tasks, HTASKITEM hParentTask, BOOL bIncDuplicateCompletedRecurringSubtasks = TRUE) const;

	BOOL ExportAllTaskAttributes(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CTaskFile& tasks, HTASKITEM hTask) const;
	BOOL ExportTaskAttributes(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CTaskFile& tasks, HTASKITEM hTask, const TDCGETTASKS& filter) const;

protected:
	const CToDoCtrlData& m_data;
	const CTDLTaskCtrlBase& m_colors;
	const CContentMgr& m_comments;

	CTDCTaskCalculator m_calculator;
	CTDCTaskFormatter m_formatter;

protected:
	COLORREF GetTaskTextColor(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;
	COLORREF GetPriorityColor(int nPriority) const;

	void ExportCompletedTasks(const TODOSTRUCTURE* pTDS, CTaskFile& tasks, HTASKITEM hTaskParent) const;
};


#endif // !defined(AFX_TODOCTRLDATAUTILS_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
