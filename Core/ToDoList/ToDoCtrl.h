#if !defined(AFX_TODOCTRL_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define AFX_TODOCTRL_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToDoCtrl.h : header file
//

#include "TDCTaskListDropTarget.h"
#include "todoctrldata.h"
#include "todoctrldatautils.h"
#include "todoctrlfind.h"
#include "todoctrlLayout.h"
#include "tdcstruct.h"
#include "tdcimagelist.h"
#include "tdltasktreectrl.h"
#include "tdlcommentsctrl.h"
#include "tdcTimeTracking.h"
#include "tdcSourceControl.h"
#include "tdcFindReplace.h"
#include "tdcdialoghelper.h"
#include "tdlinfotipctrl.h"
#include "tdltaskattributectrl.h"

#include "..\shared\runtimedlg.h"
#include "..\shared\TreeDragDropHelper.h"
#include "..\shared\popupeditctrl.h"
#include "..\shared\mapex.h"
#include "..\shared\icon.h"
#include "..\shared\FindReplace.h"
#include "..\shared\midnighttimer.h"

#include "..\Interfaces\uithemefile.h"
#include "..\Interfaces\contentmgr.h"

/////////////////////////////////////////////////////////////////////////////

class CTaskFile;
class CSpellCheckDlg;
class CPreferences;
class CTDCTaskCompletionArray;
class CTDCContentMgr;

struct TDCTASKCOMPLETION;

/////////////////////////////////////////////////////////////////////////////
// CToDoCtrl dialog

class CToDoCtrl : public CRuntimeDlg
{
private:
	struct IDLETASKS
	{
		IDLETASKS(CToDoCtrl& tdc);

		void RefreshAttributeValues(const CTDCAttributeMap& aAttribIDs = TDCA_ALL);
		void UpdateSelectedTaskPath() { m_bUpdateSelectedTaskPath = TRUE; }
		BOOL Process();

	protected:
		CToDoCtrl& m_tdc;

		CTDCAttributeMap m_mapRefreshAttribIDs;
		BOOL m_bUpdateSelectedTaskPath;

	protected:
		BOOL HasTasks() const;
	};

	friend struct IDLETASKS;

	// ----------------------------------------------------

	friend class CTDCSourceControl;
	friend class CTDCFindReplace;

	// ----------------------------------------------------

// Construction
public:
	CToDoCtrl(const CTDCContentMgr& mgrContent, 
			  const CShortcutManager& mgrShortcuts, 
			  const CONTENTFORMAT& cfDefault, 
			  const TDCCOLEDITFILTERVISIBILITY& visDefault);

	virtual ~CToDoCtrl();

	BOOL Create(const CRect& rect, CWnd* pParentWnd, UINT nID, BOOL bVisible = TRUE, BOOL bEnabled = TRUE);

	TDC_FILE Save(const CString& sFilePath = _T(""), BOOL bFlush = TRUE);
	TDC_FILE Save(CTaskFile& tasks/*out*/, const CString& sFilePath = _T(""), BOOL bFlush = TRUE);

	BOOL SaveTaskViewToImage(const CString& sFilePath);
	BOOL CanSaveTaskViewToImage() const { return m_taskTree.CanSaveToImage(); }

	TDC_FILE Load(const CString& sFilePath, LPCTSTR szDefaultPassword /*= NULL*/);
	TDC_FILE Load(const CString& sFilePath, CTaskFile& tasks/*out*/, LPCTSTR szDefaultPassword /*= NULL*/);

	BOOL DelayLoad(const CString& sFilePath, COleDateTime& dtEarliestDue);
	BOOL IsDelayLoaded() const { return m_bDelayLoaded; }

	BOOL IsArchive() const { return m_bArchive; }
	BOOL GetArchivePath(CString& sArchivePath) const;
	BOOL ArchiveDoneTasks(TDC_ARCHIVE nFlags, BOOL bRemoveFlagged); // returns true if any tasks were removed
	BOOL ArchiveSelectedTasks(BOOL bRemove); // returns true if any tasks were removed

	void EnableEncryption(BOOL bEnable = TRUE);
	BOOL IsEncrypted() const { return (!m_sPassword.IsEmpty()); }
	static BOOL CanEncrypt(); // returns true only if the required encryption capabilities are present
	CString GetPassword() const { return m_sPassword; }
	BOOL VerifyPassword(const CString& sExplanation = _T("")) const;
	BOOL ModifyPassword();
	BOOL WantPasswordReprompting() const;

	TDC_FILE CheckIn();
	TDC_FILE CheckOut(CString& sCheckedOutTo, BOOL bForce = FALSE);
	BOOL IsCheckedOut() const;
	BOOL IsSourceControlled() const;
	BOOL AddToSourceControl(BOOL bAdd = TRUE);
	BOOL CanAddToSourceControl(BOOL bAdd = TRUE) const;

	BOOL Flush(); // called to end current editing actions
	BOOL IsModified() const;
	void SetModified(BOOL bMod = TRUE);
	BOOL IsPristine() const;

	CString GetFilePath() const { return m_sLastSavePath; }
	BOOL HasFilePath() const { return !m_sLastSavePath.IsEmpty(); }
	void ClearFilePath() { m_sLastSavePath.Empty(); }
	CString GetProjectName() const { return m_sProjectName; }
	BOOL HasProjectName() const { return !m_sProjectName.IsEmpty(); }
	void SetProjectName(const CString& sProjectName);
	CString GetFriendlyProjectName(int nUntitledIndex = -1) const;
	void SetFilePath(const CString& sPath);
	CString GetStylesheetPath() const;
	int GetCommentsSize() const { return m_nCommentsSize; }
	void SetCommentsSize(int nSize);

	const CTDCCustomAttribDefinitionArray& GetCustomAttributeDefs() const { return m_aCustomAttribDefs; }
	int GetCustomAttributeDefs(CTDCCustomAttribDefinitionArray& aAttrib) const;
	BOOL SetCustomAttributeDefs(const CTDCCustomAttribDefinitionArray& aAttrib);

	BOOL DeleteAllTasks();
	void NewList();

	int GetTasks(CTaskFile& tasks, const TDCGETTASKS& filter = TDCGT_ALL) const;
	int GetSelectedTasks(CTaskFile& tasks, const TDCGETTASKS& filter = TDCGT_ALL) const;

	BOOL PasteTaskAttributeValues(const CTaskFile& tasks, HTASKITEM hTask, const CTDCAttributeMap& mapAttribs, DWORD dwFlags);
	BOOL PasteTasks(const CTaskFile& tasks, TDC_INSERTWHERE nWhere, BOOL bSelectAll = TRUE);
	BOOL MergeTasks(const CTaskFile& tasks, BOOL bMergeByID);

	void SetReadonly(BOOL bReadOnly);
	BOOL IsReadOnly() const { return HasStyle(TDCS_READONLY); }

	BOOL ModifyStyles(const CTDCStyleMap& styles);
	BOOL HasStyle(TDC_STYLE nStyle) const;
	
	BOOL IsColumnShowing(TDC_COLUMN nColumn) const;
	BOOL IsEditFieldShowing(TDC_ATTRIBUTE nAttribID) const;
	BOOL IsColumnOrEditFieldShowing(TDC_COLUMN nColumn, TDC_ATTRIBUTE nAttribID) const;
	void SetColumnFieldVisibility(const TDCCOLEDITVISIBILITY& vis);
	void GetColumnFieldVisibility(TDCCOLEDITVISIBILITY& vis) const;
	int GetSortableColumns(CTDCColumnIDMap& mapColIDs) const;

	const CTDCColumnIDMap& GetVisibleColumns() const;
	const CTDCAttributeMap& GetVisibleEditFields() const;

	void SetPriorityColors(const CDWordArray& aColors);
	void SetDueTaskColors(COLORREF crDue, COLORREF crDueToday);
    void SetGridlineColor(COLORREF color) { m_taskTree.SetGridlineColor(color); }
	void SetCompletedTaskColor(COLORREF color) { m_taskTree.SetCompletedTaskColor(color); }
	void SetFlaggedTaskColor(COLORREF color) { m_taskTree.SetFlaggedTaskColor(color); }
	void SetReferenceTaskColor(COLORREF color) { m_taskTree.SetReferenceTaskColor(color); }
	void SetAttributeColors(TDC_ATTRIBUTE nAttribID, const CTDCColorMap& colors) { m_taskTree.SetAttributeColors(nAttribID, colors); }
	void SetStartedTaskColors(COLORREF crStarted, COLORREF crStartedToday) { m_taskTree.SetStartedTaskColors(crStarted, crStartedToday); }
	void SetAlternateLineColor(COLORREF color) { m_taskTree.SetAlternateLineColor(color); }

	void GetDueTaskColors(COLORREF& crDue, COLORREF& crDueToday) { m_taskTree.GetDueTaskColors(crDue, crDueToday); }
	BOOL HasDueTodayColor() const { return m_taskTree.HasDueTodayColor(); }

	void SetUITheme(const CUIThemeFile& theme);
	void SetNumPriorityRiskLevels(int nNumLevels);
	void ResizeAttributeColumnsToFit();

	// these return the full list of items in each droplist
	int GetAutoListData(TDC_ATTRIBUTE nAttribID, TDCAUTOLISTDATA& tld) const;
	void SetDefaultAutoListData(const TDCAUTOLISTDATA& tld);
	void SetAutoListContentReadOnly(TDC_ATTRIBUTE nListAttribID, BOOL bReadOnly = TRUE);

	BOOL CreateNewTask(const CString& sText, TDC_INSERTWHERE nWhere, BOOL bEditLabel = TRUE, DWORD dwDependency = 0);
	BOOL CanCreateNewTask(TDC_INSERTWHERE nInsertWhere) const;
	BOOL CanCreateNewTask(TDC_INSERTWHERE nWhere, const CString& sText) const;

	void SetSubtaskDragDropPos(BOOL bTop = TRUE) { m_bDragDropSubtasksAtTop = bTop; }
	BOOL SplitSelectedTask(int nNumSubtasks = 2);
	BOOL CanSplitSelectedTask() const;

	inline DWORD GetSelectedTaskID() const { return m_taskTree.GetSelectedTaskID(); }
	int GetSubTaskIDs(DWORD dwTaskID, CDWordArray& aSubtaskIDs) const;

	virtual BOOL SelectTask(DWORD dwTaskID, BOOL bTaskLink);
	virtual BOOL SelectTasks(const CDWordArray& aTaskIDs);
	virtual int GetSelectedTaskIDs(CDWordArray& aTaskIDs, BOOL bTrue, BOOL bOrdered = FALSE) const;
	virtual int GetSelectedTaskIDs(CDWordArray& aTaskIDs, DWORD& dwFocusedTaskID, BOOL bRemoveChildDupes, BOOL bOrdered = FALSE) const;

	BOOL SelectNextTask(const CString& sPart, TDC_SELECTNEXTTASK nSelect);
	BOOL ScrollToSelectedTask() { return m_taskTree.EnsureSelectionVisible(FALSE); }
	
	int CacheTreeSelection(TDCSELECTIONCACHE& cache, BOOL bIncBreadcrumbs = TRUE) const;
	BOOL RestoreTreeSelection(const TDCSELECTIONCACHE& cache);

	BOOL IsTaskParent(DWORD dwTaskID) const { return m_data.IsTaskParent(dwTaskID); }
	BOOL IsTaskDone(DWORD dwTaskID) const { return m_data.IsTaskDone(dwTaskID); }
	BOOL IsTaskGoodAsDone(DWORD dwTaskID) const { return m_calculator.IsTaskDone(dwTaskID); }
	BOOL IsTaskRecurring(DWORD dwTaskID) const { return m_data.IsTaskRecurring(dwTaskID); }
	BOOL CanTaskRecur(DWORD dwTaskID) const { return m_data.CanTaskRecur(dwTaskID); }

	BOOL DeleteSelectedTask();
	BOOL EditSelectedTaskTitle(BOOL bTaskIsNew = FALSE); 
	void SpellcheckSelectedTask(BOOL bTitle); // else comments
	BOOL CanSpellcheckSelectedTaskComments();
	BOOL DoFindReplace(TDC_ATTRIBUTE nAttribID = TDCA_TASKNAME);
	BOOL CanDoFindReplace(TDC_ATTRIBUTE nAttribID = TDCA_TASKNAME) const;
	BOOL RenameTaskAttributeValues(TDC_ATTRIBUTE nListAttribID, const CString& sFrom, const CString& sTo, BOOL bCaseSensitive, BOOL bWholeWord);
	
	BOOL EditSelectedTaskDependency();
	BOOL GotoSelectedTaskDependency(); 
	BOOL GotoSelectedReferenceTaskTarget();
	BOOL GotoSelectedTaskReferences();
	BOOL GotoSelectedTaskLocalDependencies();
	BOOL GotoSelectedTaskLocalDependents();

	BOOL EditSelectedTaskRecurrence(); 
	BOOL SetSelectedTaskRecurrence(const TDCRECURRENCE& tr);

	BOOL EditSelectedTaskIcon(); 
	BOOL ClearSelectedTaskIcon(); 

	BOOL SetSelectedTaskCompletion(TDC_TASKCOMPLETION nCompletion);

	BOOL OffsetSelectedTaskDates(const CTDCDateSet& mapDates, int nAmount, TDC_UNITS nUnits, DWORD dwFlags = 0);
	BOOL CanOffsetSelectedTaskDates(const CTDCDateSet& mapDates) const;

	CString GetSelectedTaskComments() const { return m_taskTree.GetSelectedTaskComments(); }
	const CBinaryData& GetSelectedTaskCustomComments(CONTENTFORMAT& cfComments) const;
	BOOL GotoSelectedTaskFileLink(int nFile);
	CString GetSelectedTaskPath(BOOL bIncludeTaskName, int nMaxLen = -1) const { return m_taskTree.GetSelectedTaskPath(bIncludeTaskName, nMaxLen); }
	COLORREF GetSelectedTaskColor() const { return m_taskTree.GetSelectedTaskColor(); }
	int GetSelectedTaskCustomAttributeData(CTDCCustomAttributeDataMap& mapData, BOOL bFormatted = FALSE) const;
	
	int GetSelectedTaskDependencies(CTDCDependencyArray& aDepends) const { return m_ctrlAttributes.GetDependencies(aDepends); }
	CString GetSelectedTaskExternalID() const { return m_ctrlAttributes.GetExternalID(); }
	int GetSelectedTaskPriority() const { return m_ctrlAttributes.GetPriority(); }
	int GetSelectedTaskAllocTo(CStringArray& aAllocTo) const { CStringArray aUnused; return m_ctrlAttributes.GetAllocTo(aAllocTo, aUnused); }
	CString GetSelectedTaskAllocBy() const { return m_ctrlAttributes.GetAllocBy(); }
	int GetSelectedTaskFileLinks(CStringArray& aFiles) const { return m_ctrlAttributes.GetFileLinks(aFiles); }

	BOOL SelectedTasksHaveIcon() const { return m_taskTree.SelectionHasIcon(); }
	BOOL SelectedTasksAreAllDone(BOOL bIncGoodAsDone) const { return m_taskTree.SelectionAreAllDone(bIncGoodAsDone); }
	BOOL SelectedTasksHaveDependencies() const { return m_taskTree.SelectionHasDependencies(); }
	BOOL SelectedTasksHaveDependents() const { return m_taskTree.SelectionHasDependents(); }
	BOOL SelectedTasksHaveFlagged() const { return m_taskTree.SelectionHasFlagged(); }
	BOOL SelectedTasksHaveLocked() const { return m_taskTree.SelectionHasLocked(FALSE); }
	BOOL SelectedTasksHaveColor() const { return m_taskTree.SelectionHasTaskColor(); }

	CString GetTaskPath(DWORD dwTaskID, int nMaxLen = -1) const { return m_formatter.GetTaskPath(dwTaskID, nMaxLen); }
	CString GetTaskTitle(DWORD dwTaskID) const { return m_data.GetTaskTitle(dwTaskID); }
	CString GetTaskComments(DWORD dwTaskID) const { return m_data.GetTaskComments(dwTaskID); }
	CString GetParentTaskTitle(DWORD dwTaskID) const { return m_data.GetTaskTitle(m_data.GetTaskParentID(dwTaskID)); }
	DWORD GetParentTaskID(DWORD dwTaskID) const { return m_data.GetTaskParentID(dwTaskID); }
	BOOL GetTaskTimes(DWORD dwTaskID, TDCTIMEPERIOD& timeEst, TDCTIMEPERIOD& timeSpent) const;
	BOOL GetTaskTextColors(DWORD dwTaskID, COLORREF& crText, COLORREF& crBack, BOOL bSelected = FALSE) const;
	int GetTaskIconIndex(DWORD dwTaskID) const { return m_taskTree.GetTaskIconIndex(dwTaskID); }
	COleDateTime GetTaskDate(DWORD dwTaskID, TDC_DATE nDate) const;

	double CalcSelectedTaskTimeEstimate(TDC_UNITS nUnits = TDCU_HOURS) const { return m_taskTree.CalcSelectedTaskTimeEstimate(nUnits); }
	double CalcSelectedTaskTimeSpent(TDC_UNITS nUnits = TDCU_HOURS) const { return m_taskTree.CalcSelectedTaskTimeSpent(nUnits); }
	double CalcSelectedTaskCost() const { return m_taskTree.CalcSelectedTaskCost(); }

	BOOL CanSetSelectedTaskPercentDoneToToday() const;

	BOOL EditSelectedTaskColor();
	BOOL ClearSelectedTaskColor() { return SetSelectedTaskColor(CLR_NONE); }
	BOOL SetSelectedTaskTitle(const CString& sTitle, BOOL bAllowMultiple);
	BOOL SetSelectedTaskPercentDoneToToday();
	BOOL SetSelectedTaskAllocBy(const CString& sAllocBy);
	BOOL SetSelectedTaskStatus(const CString& sStatus);
	BOOL SetSelectedTaskExternalID(const CString& sID);
	BOOL SetSelectedTaskFlag(BOOL bFlagged);
	BOOL SetSelectedTaskLock(BOOL bLocked);
	BOOL SetSelectedTaskVersion(const CString& sVersion);
	BOOL SetSelectedTaskComments(const CString& sComments, const CBinaryData& customComments = _T(""));
	BOOL SetSelectedTaskIcon(const CString& sIcon); 
	BOOL SetSelectedTaskDate(TDC_DATE nDate, const COleDateTime& date);
	BOOL SetSelectedTaskCustomAttributeData(const CString& sAttribID, const TDCCADATA& data);
	BOOL SetSelectedTaskMetaData(const CString& sKey, const CString& sMetaData);

	BOOL SetSelectedTaskAllocTo(const CStringArray& aAllocTo, BOOL bAppend = FALSE);
	BOOL SetSelectedTaskCategories(const CStringArray& aCats, BOOL bAppend = FALSE);
	BOOL SetSelectedTaskDependencies(const CTDCDependencyArray& aDepends, BOOL bAppend = FALSE);
	BOOL SetSelectedTaskTags(const CStringArray& aTags, BOOL bAppend = FALSE);
	BOOL SetSelectedTaskFileLinks(const CStringArray& aFilePaths, BOOL bAppend = FALSE);

	BOOL SetSelectedTaskPercentDone(int nPercent, BOOL bOffset = FALSE);
	BOOL SetSelectedTaskPriority(int nPriority, BOOL bOffset = FALSE);
	BOOL SetSelectedTaskRisk(int nRisk, BOOL bOffset = FALSE);
	BOOL SetSelectedTaskTimeEstimate(const TDCTIMEPERIOD& timeEst, BOOL bOffset = FALSE);
	BOOL SetSelectedTaskTimeSpent(const TDCTIMEPERIOD& timeSpent, BOOL bOffset = FALSE);
	BOOL SetSelectedTaskCost(const TDCCOST& cost, BOOL bOffset = FALSE);

	BOOL CopySelectedTaskAttributeValue(TDC_ATTRIBUTE nFromAttribID, TDC_ATTRIBUTE nToAttribID);
	BOOL CopySelectedTaskAttributeValue(TDC_ATTRIBUTE nFromAttribID, const CString& sToCustomAttribID);
	BOOL CopySelectedTaskAttributeValue(const CString& sFromCustomAttribID, TDC_ATTRIBUTE nToAttribID);
	BOOL CopySelectedTaskAttributeValue(const CString& sFromCustomAttribID, const CString& sToCustomAttribID);

	CString GetColumnName(TDC_COLUMN nColID) const { return m_taskTree.GetColumnName(nColID); }

	BOOL CanClearSelectedTaskFocusedAttribute() const;
	BOOL ClearSelectedTaskFocusedAttribute();

	BOOL IncrementSelectedTaskPercentDone(BOOL bUp = TRUE); // +ve or -ve
	BOOL IncrementSelectedTaskPriority(BOOL bUp = TRUE); // +ve or -ve
	void SetPercentDoneIncrement(int nAmount);

	void BeginSelectedTaskEdit();
	void EndSelectedTaskEdit();

	// time tracking
	void PauseTimeTracking(BOOL bPause = TRUE);
	BOOL TimeTrackSelectedTask();
	BOOL CanTimeTrackSelectedTask() const;
	BOOL IsSelectedTaskBeingTimeTracked() const;
	BOOL IsActivelyTimeTracking() const; // this instant
	DWORD GetTimeTrackTaskID(BOOL bActive = TRUE) const;
	CString GetSelectedTaskTimeLogPath() const;
	void EndTimeTracking(BOOL bAllowConfirm);
	BOOL BeginTimeTracking(DWORD dwTaskID);
	BOOL DoAddTimeToLogFile();
	void SetTimeTrackingReminderInterval(int nMinutes);
	CString FormatTimeTrackingElapsedTime() const;
	void ResetTimeTrackingElapsedMinutes();

	void SetInheritedParentAttributes(const CTDCAttributeMap& mapAttribs, BOOL bUpdateAttrib);
	void SetDefaultTaskAttributeValues(const TODOITEM& tdi);

	// sort functions
	virtual void Sort(TDC_COLUMN nBy, BOOL bAllowToggle = TRUE) { m_taskTree.Sort(nBy, bAllowToggle); }
	virtual BOOL CanSortBy(TDC_COLUMN nBy) const { return m_taskTree.CanSortBy(nBy); }
	virtual BOOL CanMultiSort() const { return TRUE; }
	virtual void MultiSort(const TDSORTCOLUMNS& sort) { m_taskTree.MultiSort(sort); }
	virtual TDC_COLUMN GetSortBy() const { return m_taskTree.GetSortBy(); }
	virtual void GetSortBy(TDSORTCOLUMNS& sort) const { m_taskTree.GetSortBy(sort); }
	virtual BOOL IsSorting() const { return m_taskTree.IsSorting(); }
	virtual BOOL IsMultiSorting() const { return m_taskTree.IsMultiSorting(); }
	virtual BOOL IsSortingBy(TDC_COLUMN nBy) const { return m_taskTree.IsSortingBy(nBy); }
	virtual void Resort(BOOL bAllowToggle = FALSE) { m_taskTree.Resort(bAllowToggle); }

	// selection functions
	virtual BOOL MoveSelectedTask(TDC_MOVETASK nDirection);
	virtual BOOL CanMoveSelectedTask(TDC_MOVETASK nDirection) const { return m_taskTree.CanMoveSelection(nDirection); }

	virtual BOOL GotoNextTask(TDC_GOTO nDirection); 
	virtual BOOL CanGotoNextTask(TDC_GOTO nDirection) const;
	virtual BOOL GotoNextTopLevelTask(TDC_GOTO nDirection); 
	virtual BOOL CanGotoNextTopLevelTask(TDC_GOTO nDirection) const;

	virtual BOOL CanExpandTasks(TDC_EXPANDCOLLAPSE nWhat, BOOL bExpand) const;
	virtual void ExpandTasks(TDC_EXPANDCOLLAPSE nWhat, BOOL bExpand = TRUE);

	// copy/paste functions
	BOOL CanPasteText() const; // into focused control
	BOOL CanPasteDateTime() const; // into focused control, excludes numeric fields
	BOOL PasteText(const CString& sText); // into focused control
	BOOL CutSelectedTask();
	BOOL CopySelectedTask() const;
	void ClearCopiedItem() const;
	BOOL PasteTasks(TDC_PASTE nWhere, BOOL bAsRef);
	BOOL CanPasteTasks(TDC_PASTE nWhere, BOOL bAsRef) const;
	BOOL CanCopyAttributeColumnValues(TDC_COLUMN nColID, BOOL bSelectedTasksOnly) const;
	BOOL CopyAttributeColumnValues(TDC_COLUMN nColID, BOOL bSelectedTasksOnly) const;
	BOOL CanPasteAttributeColumnValues(TDC_COLUMN nToColID, BOOL bSelectedTasksOnly, TDC_COLUMN& nFromColID, int& nNumFrom) const;
	BOOL PasteAttributeColumnValues(TDC_COLUMN nToColID, BOOL bSelectedTasksOnly);

	void ResetFileVersion(unsigned int nTo = 0) { m_nFileVersion = max(nTo, 0); }
	DWORD GetFileVersion() const { return m_nFileVersion == 0 ? 1 : m_nFileVersion; }
    TDC_FILEFMT CompareFileFormat() const; // older, same, newer
	
	inline UINT GetTaskCount() const { return m_data.GetTaskCount(); }
	inline int GetSelectedTaskCount() const { return m_taskTree.GetSelectedCount(); }
	inline BOOL HasSelection() const { return m_taskTree.HasSelection(); }
	BOOL IsTaskLabelEditing() const;
	void NotifyParentSelectionChange() const;

	virtual void SetFocus(TDC_SETFOCUSTO nLocation);
	virtual BOOL HasFocus(TDC_SETFOCUSTO nLocation) const;
	virtual CString GetControlDescription(const CWnd* pCtrl) const;
	virtual BOOL GetSelectionBoundingRect(CRect& rSelection) const;
	virtual BOOL CanEditTask(DWORD dwTaskID, TDC_ATTRIBUTE nAttribID) const;
	virtual CString FormatSelectedTaskTitles(BOOL bFullPath, TCHAR cSep = 0, int nMaxTasks = -1) const;
	virtual BOOL DoIdleProcessing();
	virtual void SetMaximizeState(TDC_MAXSTATE nState);
	virtual BOOL WantTaskContextMenu() const { return TRUE; }

	TDC_MAXSTATE GetMaximizeState() const { return m_layout.GetMaximiseState(); }
	BOOL CanSelectTasksInHistory(BOOL bForward) const { return m_taskTree.CanSelectTasksInHistory(bForward); }
	BOOL SelectTasksInHistory(BOOL bForward);
	void SelectAll(BOOL bVisibleOnly = TRUE);
	BOOL CanSelectAll() const { return (GetTaskCount() > 0); }
	BOOL CanEditSelectedTask(TDC_ATTRIBUTE nAttribID) const { return CanEditSelectedTask(nAttribID, 0); }

	BOOL SetTreeFont(HFONT hFont); // setter responsible for deleting
	BOOL SetCommentsFont(HFONT hFont); // setter responsible for deleting

	const CTDCImageList& GetTaskIconImageList() const;

	int FindTasks(const SEARCHPARAMS& params, CResultArray& aResults) const;
	BOOL HasTask(DWORD dwTaskID) const { return m_data.HasTask(dwTaskID); }
	BOOL HasOverdueTasks() const;
	BOOL HasDueTodayTasks() const;
	BOOL HasLockedTasks() const;

	// undo/redo
	BOOL UndoLastAction(BOOL bUndo);
	BOOL CanUndoLastAction(BOOL bUndo) const;
	int GetLastModifiedTaskIDs(CDWordArray& aTaskIDs) const { return m_data.GetLastUndoActionTaskIDs(TRUE, aTaskIDs); }

	// misc
	void Spellcheck();
	void SetMaxInfotipCommentsLength(int nLength) { m_nMaxInfotipCommentsLength = max(-1, nLength); } // -1 to switch off
	COleDateTime GetLastTaskModified() const { return m_dtLastTaskMod; }
	void RefreshReminders();
	void SetLayoutPositions(TDC_UILOCATION nControlsPos, TDC_UILOCATION nCommentsPos);
	void SetCompletionStatus(const CString& sStatus);
	COleDateTime GetEarliestDueDate() const { return m_calculator.GetEarliestDueDate(); } // entire tasklist

	CString FormatTaskLink(DWORD dwTaskID, BOOL bFull) const;
	CString FormatTaskDependency(DWORD dwTaskID, BOOL bFull) const;
	BOOL ParseTaskLink(const CString& sLink, BOOL bURL, DWORD& dwTaskID, CString& sFile) const;
	static BOOL ParseTaskLink(const CString& sLink, BOOL bURL, const CString& sFolder, DWORD& dwTaskID, CString& sFile);
	
	void SetAlternatePreferencesKey(const CString& sKey) { m_sAltPrefsKey = sKey; }
	CString GetPreferencesKey(const CString& sSubKey = _T("")) const;

	virtual void NotifyBeginPreferencesUpdate() { /* do nothing */ }
	virtual void NotifyEndPreferencesUpdate();
	virtual void UpdateVisibleColumns(const CTDCColumnIDMap& mapChanges);
	virtual TDC_HITTEST HitTest(const CPoint& ptScreen, TDC_HITTESTREASON nReason) const;
	virtual DWORD HitTestTask(const CPoint& ptScreen, TDC_HITTESTREASON nReason) const;
	virtual TDC_COLUMN HitTestColumn(const CPoint& ptScreen) const;

	static BOOL IsReservedShortcut(DWORD dwShortcut);
	static void EnableExtendedSelection(BOOL bCtrl, BOOL bShift);
	static void SetRecentlyModifiedPeriod(const COleDateTimeSpan& dtSpan);
	static void SetDialogIcons(HICON hIconIconDlg, HICON hIconDependsDlg, HICON hIconRecursIcon, HICON hIconAddLogDlg);

protected:
	enum  // visible for derived classes
	{ 
		TIMER_TRACK = 1, 
		TIMER_LAST
	};
	
protected:
	// NOTE: SHARED ATTRIBUTES MUST BE INITIALISED IN STRICT ORDER

	// Attributes shared with CToDoCtrlData
	CTDCStyleMap m_styles;
	CTDCCustomAttribDefinitionArray m_aCustomAttribDefs;

	// Further attributes shared with CTDLTaskAttributeListCtrl
	CToDoCtrlData m_data;
	TDCCOLEDITVISIBILITY m_visColEdit;
	CTDCImageList m_ilTaskIcons;

	// Further attributes shared with CToDoCtrlLayout
	CTDLCommentsCtrl m_ctrlComments;
	CTDLTaskAttributeCtrl m_ctrlAttributes;

	// Further attributes shared with CTreeDragDropHelper
	CTDLTaskTreeCtrl m_taskTree;

	// -----------------------------------------------

	CPopupEditCtrl m_eTaskName;
	CTDLInfoTipCtrl m_infoTip;
	CToDoCtrlLayout m_layout;

	HFONT m_hFontTree, m_hFontComments;
	CBrush m_brUIBack;
	CUIThemeFile m_theme;
	CMidnightTimer m_timerMidnight;
	
	static HICON s_hIconIconDlg, s_hIconDependsDlg, s_hIconRecurDlg, s_hIconAddLogDlg;

	CString m_sXmlHeader, m_sXslHeader;
	CString m_sLastSavePath;
	CString m_sAltPrefsKey;
	int m_nCommentsSize;
	CString m_sPassword;
	CString m_sCompletionStatus;
	CTreeDragDropHelper m_treeDragDrop;
	CTDCTaskListDropTarget m_treeDropTarget;
	CWndPromptManager m_mgrPrompts;
	COleDateTime m_dtLastTaskMod;
	TDCAUTOLISTDATA m_tldDefault, m_tldAll;
	int m_nPercentIncrement;
	TODOITEM m_tdiDefault;
	TDC_RECURFROMOPTION m_nDefRecurFrom;
	TDC_RECURREUSEOPTION m_nDefRecurReuse;
	CDWordArray m_aRecreateTaskIDs;
	int m_nMaxInfotipCommentsLength;

	const CContentMgr& m_mgrContent;

	enum COMMENTS_STATE
	{
		CS_CLEAN,
		CS_PENDING,
		CS_CHANGED,
	};
	COMMENTS_STATE m_nCommentsState;

	CTDCTaskMatcher m_matcher;
	CTDCTaskCalculator m_calculator;
	CTDCTaskFormatter m_formatter;
	CTDCTaskExporter m_exporter;
	CTDCTimeTracking m_timeTracking;
	CTDCTaskAttributeCopier m_attribCopier;
	CTDCSourceControl m_sourceControl;
	CTDCFindReplace m_findReplace;
	CTDCReminderHelper m_reminders;
	CTDCMultiTasker m_multitasker;

	CString m_sProjectName;
	CONTENTFORMAT m_cfComments, m_cfDefault;
	CMapStringToString m_mapMetaData;
	double m_dTrackedTimeElapsedHours;

	DWORD m_dwNextUniqueID;
	DWORD m_nFileFormat;
	DWORD m_dwLastAddedID;
	DWORD m_dwEditTitleTaskID;

	mutable DWORD m_nFileVersion;

	BOOL m_bModified;
	BOOL m_bArchive;
	BOOL m_bDragDropSubtasksAtTop;
	BOOL m_bDelayLoaded;
	BOOL m_bDeletingTasks;
	BOOL m_bInSelectedTaskEdit;
	BOOL m_bPendingUpdateControls;

private:
	IDLETASKS m_idleTasks;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToDoCtrl)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int OnToolHitTest(CPoint point, TOOLINFO * pTI) const;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	// Implementation
protected:
	void UpdateComments(const CString& sTextComments, const CBinaryData& customComments);
	
	// private CToDoCtrl messages
	static UINT WM_TDC_FIXUPPOSTDROPSELECTION;
	static UINT WM_TDC_RECREATERECURRINGTASK;
	
	// Generated message map functions
	//{{AFX_MSG(CToDoCtrl)
	//}}AFX_MSG
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);

	afx_msg void OnTreeSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeChangeFocus(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg LRESULT OnTreeDragAbort(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTreeDragEnter(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTreeDragPreMove(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTreeDragDrop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTreeDragOver(WPARAM wParam, LPARAM lParam);

	afx_msg void OnChangeProjectName();
	afx_msg void OnSelChangeCommentsType();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	afx_msg LRESULT OnTDCHasClipboard(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCGetClipboard(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCDoTaskLink(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCFailedLink(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCGetTaskReminder(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnTDCGetLinkTooltip(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnTDCNotifyTaskAttributeEdited(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCNotifyColumnEditClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCEditTaskAttribute(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCEditTaskReminder(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCClearTaskReminder(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCClearTaskAttribute(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCNotifyAutoComboAddDelete(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCToggleTimeTracking(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCAddTimeToLogFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCSelectDependencies(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCDisplayLink(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCCanCopyAttributeValue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCCanPasteAttributeValue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCCopyAttributeValue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCPasteAttributeValue(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnGetFileIcon(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCustomUrl(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDropObject(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCanDropObject(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetFont(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsWantSpellCheck(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFixupPostDropSelection(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnLabelEditEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLabelEditCancel(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecreateRecurringTask(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsDoHelp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsKillFocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnApplyAddLoggedTime(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsGetTooltip(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsGetAttributeList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTaskIconDlgReloadIcons(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMidnight(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSplitChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnDrawSplitBar(WPARAM wp, LPARAM lp);

	afx_msg LRESULT OnFindReplaceMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFindReplaceSelectNextTask(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFindReplaceSelectedTask(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFindReplaceAllTasks(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFindReplaceGetExclusionRect(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	// Pseudo message handler
	LRESULT OnGetLinkTooltip(WPARAM wp, LPARAM lp);

	// -------------------------------------------------------------------------------

	inline const TODOITEM* GetTask(DWORD dwTaskID) const { return m_taskTree.GetTask(dwTaskID); }
	inline DWORD GetTaskID(HTREEITEM hti) const { return m_taskTree.GetTaskID(hti); }
	inline DWORD GetTrueTaskID(HTREEITEM hti) const { return m_taskTree.GetTrueTaskID(hti); }
	inline HTREEITEM GetSelectedItem() const { return m_taskTree.GetSelectedItem(); }
	inline BOOL ItemHasChildren(HTREEITEM hti) const { return m_taskTree.ItemHasChildren(hti); }
	inline BOOL ItemHasParent(HTREEITEM hti) const { return (NULL != m_taskTree.ItemHasParent(hti)); }
	inline BOOL IsItemSelected(HTREEITEM hti) const { return m_taskTree.IsItemSelected(hti); }

	inline const CToDoCtrlFind& TCF() const { return m_taskTree.Find(); }
	
	inline CTreeCtrlHelper& TCH() { return m_taskTree.TCH(); }
	inline const CTreeCtrlHelper& TCH() const { return m_taskTree.TCH(); }
	
	inline CTreeSelectionHelper& TSH() { return m_taskTree.TSH(); }
	inline const CTreeSelectionHelper& TSH() const { return m_taskTree.TSH(); }

	// -------------------------------------------------------------------------------

	virtual void InvalidateItem(HTREEITEM hti, BOOL bUpdate = FALSE) { m_taskTree.InvalidateItem(hti, bUpdate); }
	virtual void UpdateSelectedTaskPath();
	virtual void EndLabelEdit(BOOL bCancel);
	virtual BOOL GetLabelEditRect(CRect& rScreen);
	virtual void SetEditTitleTaskID(DWORD dwTaskID);
	virtual void EndTimeTracking(BOOL bAllowConfirm, BOOL bNotify);
	virtual BOOL BeginTimeTracking(DWORD dwTaskID, BOOL bNotify);
	virtual DWORD GetNextNonSelectedTaskID() const;
	virtual int GetAllTaskIDs(CDWordArray& aTaskIDs, BOOL bIncParents, BOOL bIncCollapsedChildren) const;

	enum
	{
		TDCSS_WANTRESIZE = 0x01,
	};
	virtual DWORD SetStyle(TDC_STYLE nStyle, BOOL bEnable);

	virtual TODOITEM* CreateNewTask(HTREEITEM htiParent);
	virtual BOOL DeleteSelectedTask(BOOL bWarnUser, BOOL bResetSel = FALSE);
	virtual DWORD RecreateRecurringTaskInTree(const CTaskFile& task, const COleDateTime& dtNext, BOOL bDueDate);
	virtual void SetModified(const CTDCAttributeMap& mapAttribIDs, const CDWordArray& aModTaskIDs, BOOL bAllowResort);
	virtual int GetColumnTaskIDs(CDWordArray& aTaskIDs, int nFrom = 0, int nTo = -1) const;

	virtual void LoadAttributeVisibility(const CTaskFile& tasks, const CPreferences& prefs);
	virtual void SaveAttributeVisibility(CTaskFile& tasks) const;
	virtual void SaveAttributeVisibility(CPreferences& prefs) const;
	
	virtual void Resize(int cx = 0, int cy = 0);
	virtual void ShowTaskCtrl(BOOL bShow);
	virtual void ReposTaskCtrl(const CRect& rect);

	virtual void OnStylesUpdated(const CTDCStyleMap& styles) { m_taskTree.OnStylesUpdated(styles, TRUE); }
	virtual void OnTaskIconsChanged() { m_taskTree.OnImageListChange(); }
	virtual void OnCustomAttributesChanged();
	virtual void OnFirstSave(const CTaskFile& /*tasks*/) {}
	
	virtual void SaveTasksState(CPreferences& prefs, BOOL bRebuildingTree = FALSE) const; // keyed by last filepath
	virtual HTREEITEM LoadTasksState(const CPreferences& prefs, BOOL bRebuildingTree = FALSE); // returns the previously selected item if any

	virtual int GetArchivableTasks(CTaskFile& tasks, BOOL bSelectedOnly = FALSE) const;
	virtual BOOL RemoveArchivedTask(DWORD dwTaskID);
	virtual HTREEITEM RebuildTree(const void* pContext = NULL);
	virtual BOOL WantAddTaskToTree(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const void* pContext) const;
	virtual DWORD MergeNewTaskIntoTree(const CTaskFile& tasks, HTASKITEM hTask, DWORD dwParentTaskID, BOOL bAndSubtasks);

	virtual HTREEITEM GetUpdateControlsItem() const { return GetSelectedItem(); }
	virtual BOOL CopySelectedTasks() const;
	virtual BOOL LoadTasks(const CTaskFile& tasks);
	virtual BOOL SelectNextTask(const CString& sPart, TDC_SELECTNEXTTASK nSelect, TDC_ATTRIBUTE nAttribID, BOOL bCaseSensitive, BOOL bWholeWord, BOOL bFindReplace);
	virtual void DeselectAll() { m_taskTree.DeselectAll(); }

	// -------------------------------------------------------------------------------
	
	BOOL UpdateTask(TDC_ATTRIBUTE nAttribID, DWORD dwFlags = 0);
	void UpdateControls(BOOL bIncComments = TRUE);
	void IncrementTrackedTime(BOOL bEnding);
	BOOL FindReplaceSelectedTaskAttribute(BOOL bReplacingAllTasks);

	// internal versions so we can tell how we've been called
	BOOL SetSelectedTaskComments(const CString& sComments, const CBinaryData& customComments, BOOL bInternal);
	BOOL SetSelectedTaskTimeEstimate(const TDCTIMEPERIOD& timeEst, BOOL bOffset, BOOL bRecalcTime);
	BOOL SetSelectedTaskTimeSpent(const TDCTIMEPERIOD& timeSpent, BOOL bOffset, BOOL bRecalcTime);
	HTREEITEM InsertNewTask(const CString& sText, HTREEITEM htiParent, HTREEITEM htiAfter, BOOL bEditLabel, DWORD dwDependency);
	BOOL SetSelectedTaskPercentDone(int nPercent, BOOL bOffset, const COleDateTime& date);
	BOOL SetSelectedTaskColor(COLORREF color);

	int GetAllSelectedTaskDependencies(CDWordArray& aLocalDepends, CStringArray& aOtherDepends) const;
	BOOL GetSelectedTaskRecurrence(TDCRECURRENCE& tr) const { return m_taskTree.GetSelectedTaskRecurrence(tr); }
	DWORD GetSelectedTaskParentID() const { return m_taskTree.GetSelectedTaskParentID(); }
	BOOL GetSelectedTaskTimeEstimate(TDCTIMEPERIOD& timeEst) const { return m_ctrlAttributes.GetTimeEstimate(timeEst); }
	BOOL GetSelectedTaskTimeSpent(TDCTIMEPERIOD& timeSpent) const { return m_ctrlAttributes.GetTimeSpent(timeSpent); }
	CString GetSelectedTaskIcon() const { return m_taskTree.GetSelectedTaskIcon(); }
	BOOL GetSelectedTaskTimePeriod(TDC_ATTRIBUTE nAttribID, TDCTIMEPERIOD& tp) const;
	BOOL SelectedTaskHasDate(TDC_DATE nDate) const { return m_taskTree.SelectedTaskHasDate(nDate); }
	BOOL GetSelectedTaskCustomAttributeData(const CString& sAttribID, TDCCADATA& data, BOOL bFormatted = FALSE) const { return m_ctrlAttributes.GetCustomAttributeData(sAttribID, data, bFormatted); }

	BOOL CanEditSelectedTask(TDC_ATTRIBUTE nAttribID, DWORD dwTaskID) const;
	BOOL CanEditSelectedTask(const CTDCAttributeMap& mapAttribs) const;

	BOOL CanSetSelectedTaskPercentDone(BOOL bToToday) const;
	BOOL CanClearSelectedTaskAttribute(TDC_ATTRIBUTE nAttribID) const;
	BOOL ClearSelectedTaskAttribute(TDC_ATTRIBUTE nAttribID);

	BOOL SetSelectedTaskCompletion(const COleDateTime& date);
	BOOL SetSelectedTaskCompletion(const CTDCTaskCompletionArray& aTasks);
	BOOL SetSelectedTaskCompletion(const TDCTASKCOMPLETION& task, BOOL bAndSubtasks);
	BOOL CanSetSelectedTasksDone(const CTDCTaskCompletionArray& aTasks, BOOL& bAndSubtasks) /*const*/;
	BOOL TaskHasIncompleteDependencies(DWORD dwTaskID, CString& sIncomplete) const;
	BOOL CheckWantTaskSubtasksCompleted(const CDWordArray& aTaskIDs) const;

	void LoadGlobals(const CTaskFile& tasks);
	void SaveCustomAttributeDefinitions(CTaskFile& tasks, const TDCGETTASKS& filter = TDCGETTASKS()) const;
	void LoadCustomAttributeDefinitions(const CTaskFile& tasks);

	BOOL IsClipboardEmpty(BOOL bCheckID = FALSE) const;
	CString GetClipboardID() const;
	BOOL GetClipboardID(CString& sClipID, BOOL bArchive) const;

	void ShowHideControls();
	void EnableDisableControls(BOOL bHasSelection = -1);
	void EnableDisableComments(BOOL bHasSelection = -1);
	void ReposProjectName(CRect& rAvailable);
	BOOL HandleCustomColumnClick(TDC_COLUMN nColID);

	int AddTasksToTaskFile(const CHTIList& listHTI, const TDCGETTASKS& filter, CTaskFile& tasks) const;
	int AddTreeChildrenToTaskFile(HTREEITEM hti, CTaskFile& tasks, HTASKITEM hTask, const TDCGETTASKS& filter) const;
	BOOL AddTreeItemToTaskFile(HTREEITEM hti, DWORD dwTaskID, CTaskFile& tasks, HTASKITEM hParentTask, const TDCGETTASKS& filter, BOOL bWantSubtasks = TRUE, DWORD dwParentID = 0) const;
	BOOL AddTreeItemAndParentToTaskFile(HTREEITEM hti, CTaskFile& tasks, const TDCGETTASKS& filter, BOOL bAllParents, BOOL bWantSubtasks) const;
	void AddSelectedTaskReferencesToTaskFile(const TDCGETTASKS& filter, CTaskFile& tasks) const;
	void AddSelectedTaskDependentsToTaskFile(const TDCGETTASKS& filter, CTaskFile& tasks) const;

	HTREEITEM PasteTaskToTree(const CTaskFile& tasks, HTASKITEM hTask, HTREEITEM htiParent, HTREEITEM htiAfter, TDC_RESETIDS nResetID, BOOL bAndSubtasks);
	BOOL PasteTasksToTree(const CTaskFile& tasks, HTREEITEM htiDest, HTREEITEM htiDestAfter, TDC_RESETIDS nResetID, BOOL bSelectAll);
	BOOL MergeTaskWithTree(const CTaskFile& tasks, HTASKITEM hTask, DWORD dwParentTaskID, BOOL bMergeByID, CDWordArray& aNewTaskIDs);

	void SaveSplitPos(CPreferences& prefs) const;
	void LoadSplitPos(const CPreferences& prefs);
	void SaveDefaultRecurrence(CPreferences& prefs) const;
	void LoadDefaultRecurrence(const CPreferences& prefs);

	void ToggleTimeTracking(HTREEITEM hti);
	BOOL AddTimeToTaskLogFile(DWORD dwTaskID, double dHours, const COleDateTime& dtWhen, const CString& sComment, BOOL bTracked);
	BOOL DoAddTimeToLogFile(DWORD dwTaskID, double dHours, BOOL bShowDialog);
	BOOL AdjustTaskTimeSpent(DWORD dwTaskID, double dHours);

	BOOL SetSelectedTaskArray(TDC_ATTRIBUTE nAttribID, const CStringArray& aItems, BOOL bAppend);
	BOOL SetSelectedTaskArray(TDC_ATTRIBUTE nAttribID, const CStringArray& aAll, const CStringArray& aChecked, const CStringArray& aMixed);

	BOOL SpellcheckItem(HTREEITEM hti, CSpellCheckDlg* pSpellChecker, BOOL bTitle, BOOL bNotifyNoErrors);
	BOOL SpellcheckItem(HTREEITEM hti, CSpellCheckDlg* pSpellChecker);
	BOOL CanSpellcheckComments();

	BOOL GotoFile(const CString& sFile, BOOL bShellExecute = TRUE);
	BOOL ShowTaskLink(const CString& sLink, BOOL bURL);
	void MakeRelativePaths(CStringArray& aFilePaths) const;
	void MakeFullPaths(CStringArray& aFilePaths) const;
	CString GetFullPath(const CString& sFile) const;
	CString GetLastSaveFolder() const;
	BOOL CheckRestoreBackupFile(const CString& sFilePath);

	BOOL HandleUnsavedComments();
	BOOL UndoLastActionItems(const CArrayUndoElements& aElms);
	void LoadTaskIcons();
	BOOL ConfirmDeleteAllTasks(BOOL bSelected = FALSE) const;

	typedef CMap<DWORD, DWORD, DWORD, DWORD&> CMapID2ID;

	void PrepareTasksForPaste(CTaskFile& tasks, TDC_RESETIDS nResetID, BOOL bResetCreation) const;
	void BuildTaskIDMapForPaste(CTaskFile& tasks, HTASKITEM hTask, DWORD& dwNextID, 
								CMapID2ID& mapID, TDC_RESETIDS nResetID, BOOL bAndSiblings) const;
	void PrepareTasksForPaste(CTaskFile& tasks, HTASKITEM hTask, BOOL bResetCreation, const CMapID2ID& mapID, BOOL bAndSiblings) const;
	BOOL PrepareTaskLinkForPaste(CString& sLink, const CMapID2ID& mapID) const;
	BOOL PrepareTaskLinkForPaste(TDCDEPENDENCY& depends, const CMapID2ID& mapID) const;
	void PrepareTasksForPasteAsRef(CTaskFile& tasks, const CDWordArray& aSelTaskIDs = CDWordArray()) const;

	void RemoveArchivedTasks(const CTaskFile& tasks, TDC_ARCHIVE nRemove, BOOL bRemoveFlagged);
	BOOL RemoveArchivedTask(const CTaskFile& tasks, HTASKITEM hTask, TDC_ARCHIVE nRemove, BOOL bRemoveFlagged);
	BOOL ArchiveTasks(const CString& sArchivePath, const CTaskFile& tasks); // helper to avoid code dupe
	void PrepareTaskfileForTasks(CTaskFile& tasks, const TDCGETTASKS& filter) const;
	
	BOOL IsValidSelectedTaskMoveTarget(DWORD dwTargetID, DD_DROPEFFECT nDrop) const { return m_taskTree.IsValidSelectedTaskMoveTarget(dwTargetID, nDrop); }
	DD_DROPEFFECT GetSelectedTasksDropEffect(DWORD dwTargetID, BOOL bLeftDrag) const { return m_taskTree.GetSelectedTaskDropEffect(dwTargetID, bLeftDrag); }
	BOOL IsSelectedTaskMoveEnabled(TDC_MOVEMETHOD nMethod) const { return m_taskTree.IsSelectedTaskMoveEnabled(nMethod); }
	BOOL DropSelectedTasks(DD_DROPEFFECT nDrop, HTREEITEM htiDropTarget, HTREEITEM htiDropAfter);
	BOOL CanDropSelectedTasks(DD_DROPEFFECT nDrop, HTREEITEM htiDropTarget) const;

	void SearchAndExpand(const SEARCHPARAMS& params, BOOL bExpand);
	void AppendTaskFileHeader(CTaskFile& tasks) const;
	void FixupParentCompletion(DWORD dwParentID);

	// used for building/creating the tree for saving/loading
	// NOT FOR OVERRIDING
	int GetAllTasks(CTaskFile& tasks) const;
	HTREEITEM SetAllTasks(const CTaskFile& tasks);

	void SelectItem(HTREEITEM hti);
	BOOL BuildTreeItem(HTREEITEM hti, const TODOSTRUCTURE* pTDS, const void* pContext);
	HTREEITEM InsertTreeItem(const TODOITEM* pTDI, DWORD dwID, HTREEITEM htiParent, HTREEITEM htiAfter, BOOL bAddToCombos);

	void InitialiseNewRecurringTask(DWORD dwPrevTaskID, DWORD dwNewTaskID, const COleDateTime& dtNext, BOOL bDueDate);
	int CreateTasksFromOutlookObjects(const TLDT_DATA* pData);
	BOOL CopyColumnValue(const TODOITEM& tdiFrom, TDC_COLUMN nFromColID, DWORD dwToTaskID, TDC_COLUMN nToColID);

	TDC_ATTRIBUTE GetFocusedControlAttribute() const;
	void BuildTasksForSave(CTaskFile& tasks) const;
	void UpdateAutoListData(TDC_ATTRIBUTE nAttribID = TDCA_ALL);
	void UpdateDefaultTaskCustomAttributeValues();
	void SetModified(TDC_ATTRIBUTE nAttribID, const CDWordArray& aModTaskIDs = CDWordArray());
	BOOL IsNewTaskMod(const CTDCAttributeMap& mapAttribIDs, const CDWordArray& aModTaskIDs) const;
	BOOL IsNewTaskTitleEditMod(const CTDCAttributeMap& mapAttribIDs, const CDWordArray& aModTaskIDs) const;

	static BOOL HandleModResult(DWORD dwTaskID, TDC_SET nRes, CDWordArray& aModTaskIDs);
	static BOOL XMLHeaderIsUnicode(LPCTSTR szXmlHeader);
	static TDC_FILE SaveTaskfile(CTaskFile& tasks, const CString& sSavePath);
	static int RemoveNonSelectedTasks(const CDWordSet& mapSelTaskIDs, CTaskFile& tasks, HTASKITEM hTask);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TODOCTRL_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
