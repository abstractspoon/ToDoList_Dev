#if !defined(AFX_TODOCTRL_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define AFX_TODOCTRL_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToDoCtrl.h : header file
//

#include "TaskListDropTarget.h"
#include "todoctrldata.h"
#include "todoctrlfind.h"
#include "taskfile.h"
#include "tdcstruct.h"
#include "tdlprioritycombobox.h"
#include "tdlriskcombobox.h"
#include "tdlrecurringtaskedit.h"
#include "tdcimagelist.h"
#include "tdltasktreectrl.h"
#include "tdlcontenttypecombobox.h"

#include "..\shared\runtimedlg.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\orderedtreectrl.h"
#include "..\shared\filecombobox.h"
#include "..\shared\urlricheditctrl.h"
#include "..\shared\colorcombobox.h"
#include "..\shared\autocombobox.h"
#include "..\shared\maskedit.h"
#include "..\shared\timeedit.h"
#include "..\shared\TreeDragDropHelper.h"
#include "..\shared\wndPrompt.h"
#include "..\shared\contentmgr.h"
#include "..\shared\encheckcombobox.h"
#include "..\shared\timecombobox.h"
#include "..\shared\popupeditctrl.h"
#include "..\shared\uithemefile.h"
#include "..\shared\datetimectrlex.h"

#include "..\3rdparty\colourpicker.h"

/////////////////////////////////////////////////////////////////////////////

// predeclarations
class CXmlItem;
class CTaskFile;
class CDeferWndMove;
class CDlgUnits;
class CSpellCheckDlg;
class CPreferences;
class CMSOutlookItemDataMap;
class CTDCAttributeMapping;

struct CTRLITEM;
struct TDCCOLUMN;

/////////////////////////////////////////////////////////////////////////////

namespace OutlookAPI
{
	class _MailItem;
	class Selection;
}

/////////////////////////////////////////////////////////////////////////////
// CToDoCtrl dialog

class CToDoCtrl : public CRuntimeDlg
{
// Construction
public:
	CToDoCtrl(CContentMgr& mgr, const CONTENTFORMAT& cfDefault, const TDCCOLEDITFILTERVISIBILITY& visDefault);
	virtual ~CToDoCtrl();

	BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID, BOOL bVisible = TRUE, BOOL bEnabled = TRUE);

	TDC_FILE Save(const CString& sFilePath = _T(""));
	TDC_FILE Save(CTaskFile& tasks/*out*/, const CString& sFilePath = _T(""));

	TDC_FILE Load(const CString& sFilePath);
	TDC_FILE Load(const CString& sFilePath, CTaskFile& tasks/*out*/);

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

	void SetMaximizeState(TDC_MAXSTATE nState);
	virtual BOOL WantTaskContextMenu() const { return TRUE; }

	TDC_FILE CheckIn();
	TDC_FILE CheckOut();
	TDC_FILE CheckOut(CString& sCheckedOutTo);
	BOOL IsCheckedOut() const;
	BOOL IsSourceControlled() const;
	BOOL AddToSourceControl(BOOL bAdd = TRUE);

	void Flush(BOOL bEndTimeTracking = FALSE); // called to end current editing actions
	BOOL IsModified() const;
	void SetModified(BOOL bMod = TRUE);

	CString GetFilePath() const { return m_sLastSavePath; }
	void ClearFilePath() { m_sLastSavePath.Empty(); }
	CString GetProjectName() const { return m_sProjectName; }
	void SetProjectName(const CString& sProjectName);
	CString GetFriendlyProjectName(int nUntitledIndex = -1) const;
	void SetFilePath(const CString& sPath) { m_sLastSavePath = sPath; }
	CString GetStylesheetPath() const;

	const CTDCCustomAttribDefinitionArray& GetCustomAttributeDefs() const { return m_aCustomAttribDefs; }
	int GetCustomAttributeDefs(CTDCCustomAttribDefinitionArray& aAttrib) const;
	void SetCustomAttributeDefs(const CTDCCustomAttribDefinitionArray& aAttrib);

	BOOL DeleteAllTasks();
	void NewList();

	int GetTasks(CTaskFile& tasks, const TDCGETTASKS& filter = TDCGT_ALL) const;
	int GetSelectedTasks(CTaskFile& tasks, const TDCGETTASKS& filter = TDCGT_ALL, DWORD dwFlags = 0) const;

	BOOL InsertTasks(const CTaskFile& tasks, TDC_INSERTWHERE nWhere, BOOL bSelectAll = TRUE);

	void SetReadonly(BOOL bReadOnly) { SetStyle(TDCS_READONLY, bReadOnly); }
	BOOL IsReadOnly() const { return HasStyle(TDCS_READONLY); }

	BOOL SetStyles(const CTDCStylesMap& styles);
	BOOL SetStyle(TDC_STYLE nStyle, BOOL bOn = TRUE); // one style at a time only 
	BOOL HasStyle(TDC_STYLE nStyle) const; // one style at a time only 
	
	virtual BOOL IsColumnShowing(TDC_COLUMN nColumn) const;
	BOOL IsEditFieldShowing(TDC_ATTRIBUTE nAttrib) const;
	BOOL IsColumnOrEditFieldShowing(TDC_COLUMN nColumn, TDC_ATTRIBUTE nAttrib) const;
	void SetColumnEditFilterVisibility(const TDCCOLEDITFILTERVISIBILITY& vis);
	void GetColumnEditFilterVisibility(TDCCOLEDITFILTERVISIBILITY& vis) const;
	const CTDCColumnIDMap& GetVisibleColumns() const;
	const CTDCAttributeMap& GetVisibleEditFields() const;
	const CTDCAttributeMap& GetVisibleFilterFields() const;

	void SetPriorityColors(const CDWordArray& aColors);
	void SetDueTaskColors(COLORREF crDue, COLORREF crDueToday);
    void SetGridlineColor(COLORREF color) { m_taskTree.SetGridlineColor(color); }
	void SetCompletedTaskColor(COLORREF color) { m_taskTree.SetCompletedTaskColor(color); }
	void SetFlaggedTaskColor(COLORREF color) { m_taskTree.SetFlaggedTaskColor(color); }
	void SetReferenceTaskColor(COLORREF color) { m_taskTree.SetReferenceTaskColor(color); }
	void SetAttributeColors(TDC_ATTRIBUTE nAttrib, const CTDCColorMap& colors) { m_taskTree.SetAttributeColors(nAttrib, colors); }
	void SetStartedTaskColors(COLORREF crStarted, COLORREF crStartedToday) { m_taskTree.SetStartedTaskColors(crStarted, crStartedToday); }
	void SetAlternateLineColor(COLORREF color) { m_taskTree.SetAlternateLineColor(color); }

	COLORREF GetPriorityColor(int nPriority) const { return m_taskTree.GetPriorityColor(nPriority); }
	void GetStartedTaskColors(COLORREF& crStarted, COLORREF& crStartedToday) { m_taskTree.GetStartedTaskColors(crStarted, crStartedToday); }
	void GetDueTaskColors(COLORREF& crDue, COLORREF& crDueToday) { m_taskTree.GetDueTaskColors(crDue, crDueToday); }

	void SetUITheme(const CUIThemeFile& theme);
	void ResizeAttributeColumnsToFit();

	// these return the full list of items in each droplist
	int GetAutoListData(TDCAUTOLISTDATA& tld) const;
	void SetDefaultAutoListData(const TDCAUTOLISTDATA& tld);
	BOOL SetAutoListContentReadOnly(TDC_ATTRIBUTE nListAttribID, BOOL bReadOnly = TRUE);

	BOOL CreateNewTask(const CString& sText, TDC_INSERTWHERE nWhere, BOOL bEditText = TRUE, DWORD dwDependency = 0);
	BOOL CanCreateNewTask(TDC_INSERTWHERE nInsertWhere) const;
	BOOL CanCreateNewTask(TDC_INSERTWHERE nWhere, const CString& sText) const;

	void SetSubtaskDragDropPos(BOOL bTop = TRUE) { m_bDragDropSubtasksAtTop = bTop; }
	BOOL SplitSelectedTask(int nNumSubtasks = 2);
	BOOL CanSplitSelectedTask() const { return m_taskTree.CanSplitSelectedTask(); }

	inline DWORD GetSelectedTaskID() const { return m_taskTree.GetSelectedTaskID(); }
	int GetSelectedTaskIDs(CDWordArray& aTaskIDs, BOOL bTrue = FALSE) const;
	int GetSelectedTaskIDs(CDWordArray& aTaskIDs, DWORD& dwFocusedTaskID, BOOL bRemoveChildDupes) const;

	BOOL SelectTask(DWORD dwTaskID) { return SelectTask(dwTaskID, TRUE); }
	BOOL SelectTasks(const CDWordArray& aTaskIDs) { return SelectTasks(aTaskIDs, TRUE); }

	int CacheTreeSelection(TDCSELECTIONCACHE& cache, BOOL bIncBreadcrumbs = TRUE) const;
	BOOL RestoreTreeSelection(const TDCSELECTIONCACHE& cache);

	BOOL IsTaskDone(DWORD dwTaskID, DWORD dwExtraCheck = TDCCHECKNONE) const { return m_data.IsTaskDone(dwTaskID, dwExtraCheck); }
	BOOL IsTaskRecurring(DWORD dwTaskID) const;
	BOOL CanTaskRecur(DWORD dwTaskID) const;

	BOOL DeleteSelectedTask();
	BOOL EditSelectedTask(BOOL bTaskIsNew = FALSE); 
	void SpellcheckSelectedTask(BOOL bTitle); // else comments
	BOOL CanSpellcheckSelectedTaskComments();
	
	BOOL GotoSelectedTaskDependency(); 
	BOOL GotoSelectedReferenceTaskTarget();
	BOOL GotoSelectedTaskReferences();

	BOOL EditSelectedTaskRecurrence(); 
	BOOL SetSelectedTaskRecurrence(const TDCRECURRENCE& tr);

	BOOL EditSelectedTaskIcon(); 
	BOOL ClearSelectedTaskIcon(); 

	BOOL SetSelectedTaskDone(BOOL bDone = TRUE);
	BOOL IsSelectedTaskDone() const { return m_taskTree.IsSelectedTaskDone(); }
	BOOL IsSelectedTaskDue() const { return m_taskTree.IsSelectedTaskDue(); }
	BOOL OffsetSelectedTaskDate(TDC_DATE nDate, int nAmount, TDC_OFFSET nUnits, BOOL bAndSubtasks);
	BOOL OffsetSelectedTaskDates(int nAmount, TDC_OFFSET nUnits, BOOL bAndSubtasks);
	COleDateTime GetEarliestDueDate() const { return m_data.GetEarliestDueDate(); } // entire tasklist

	COLORREF GetSelectedTaskColor() const { return m_taskTree.GetSelectedTaskColor(); }
	CString GetSelectedTaskIcon() const { return m_taskTree.GetSelectedTaskIcon(); }
	CString GetSelectedTaskComments() const { return m_taskTree.GetSelectedTaskComments(); }
	const CBinaryData& GetSelectedTaskCustomComments(CString& sCommentsTypeID) const { return m_taskTree.GetSelectedTaskCustomComments(sCommentsTypeID); }
	CString GetSelectedTaskTitle() const { return m_taskTree.GetSelectedTaskTitle(); }
	double GetSelectedTaskTimeEstimate(TDC_UNITS& nUnits) const { return m_taskTree.GetSelectedTaskTimeEstimate(nUnits); }
	double GetSelectedTaskTimeSpent(TDC_UNITS& nUnits) const { return m_taskTree.GetSelectedTaskTimeSpent(nUnits); }
	int GetSelectedTaskAllocTo(CStringArray& aAllocTo) const { return m_taskTree.GetSelectedTaskAllocTo(aAllocTo); }
	CString GetSelectedTaskAllocBy() const { return m_taskTree.GetSelectedTaskAllocBy(); }
	CString GetSelectedTaskStatus() const { return m_taskTree.GetSelectedTaskStatus(); }
	int GetSelectedTaskCategories(CStringArray& aCats) const { return m_taskTree.GetSelectedTaskCategories(aCats); }
	int GetSelectedTaskDependencies(CStringArray& aDepends) const { return m_taskTree.GetSelectedTaskDependencies(aDepends); }
	int GetSelectedTaskTags(CStringArray& aTags) const { return m_taskTree.GetSelectedTaskTags(aTags); }
	CString GetSelectedTaskFileRef(int nFile) const;
	int GetSelectedTaskFileRefCount() const { return m_taskTree.GetSelectedTaskFileRefCount(); }
	int GetSelectedTaskFileRefs(CStringArray& aFiles) const;
	BOOL GotoSelectedTaskFileRef(int nFile);
	CString GetSelectedTaskExtID() const { return m_taskTree.GetSelectedTaskExtID(); }
	int GetSelectedTaskPercent() const { return m_taskTree.GetSelectedTaskPercent(); }
	int GetSelectedTaskPriority() const { return m_taskTree.GetSelectedTaskPriority(); }
	int GetSelectedTaskRisk() const { return m_taskTree.GetSelectedTaskRisk(); }
	double GetSelectedTaskCost() const { return m_taskTree.GetSelectedTaskCost(); }
	BOOL IsSelectedTaskFlagged() const { return m_taskTree.IsSelectedTaskFlagged(); }
	BOOL GetSelectedTaskRecurrence(TDCRECURRENCE& tr) const;
	CString GetSelectedTaskVersion() const { return m_taskTree.GetSelectedTaskVersion(); }
	BOOL SelectedTaskHasDate(TDC_DATE nDate) const { return m_taskTree.SelectedTaskHasDate(nDate); }
	CString GetSelectedTaskPath(BOOL bIncludeTaskName, int nMaxLen = -1) const { return m_taskTree.GetSelectedTaskPath(bIncludeTaskName, nMaxLen); }
	COleDateTime GetSelectedTaskDate(TDC_DATE nDate) const { return m_taskTree.GetSelectedTaskDate(nDate); }
	CString GetSelectedTaskCustomAttributeData(const CString& sAttribID, BOOL bFormatted) const { return m_taskTree.GetSelectedTaskCustomAttributeData(sAttribID, bFormatted); }
	int GetSelectedTaskCustomAttributeData(CMapStringToString& mapData, BOOL bFormatted) const;
	BOOL IsSelectedTaskReference() const { return m_taskTree.IsSelectedTaskReference(); }
	DWORD GetSelectedTaskParentID() const { return m_taskTree.GetSelectedTaskParentID(); }

	CString GetTaskPath(DWORD dwTaskID, int nMaxLen = -1) const { return m_data.GetTaskPath(dwTaskID, nMaxLen); }
	CString GetTaskTitle(DWORD dwTaskID) const { return m_data.GetTaskTitle(dwTaskID); }
	CString GetTaskComments(DWORD dwTaskID) const { return m_data.GetTaskComments(dwTaskID); }
	COleDateTime GetTaskDate(DWORD dwID, TDC_DATE nDate) const;
	BOOL GetTaskTimes(DWORD dwTaskID, double& dTimeEst, TDC_UNITS& nEstUnits, double& dTimeSpent, TDC_UNITS& nSpentUnits) const;

	double CalcSelectedTaskTimeEstimate(TDC_UNITS nUnits = TDCU_HOURS) const { return m_taskTree.CalcSelectedTaskTimeEstimate(nUnits); }
	double CalcSelectedTaskTimeSpent(TDC_UNITS nUnits = TDCU_HOURS) const { return m_taskTree.CalcSelectedTaskTimeSpent(nUnits); }
	double CalcSelectedTaskCost() const { return m_taskTree.CalcSelectedTaskCost(); }

	BOOL SetSelectedTaskColor(COLORREF color);
	BOOL ClearSelectedTaskColor() { return SetSelectedTaskColor(CLR_NONE); }
	BOOL SetSelectedTaskTitle(const CString& sTitle);
	BOOL SetSelectedTaskPercentDone(int nPercent);
	BOOL SetSelectedTaskTimeEstimate(double dTime, TDC_UNITS nUnits = TDCU_HOURS);
	BOOL SetSelectedTaskTimeSpent(double dTime, TDC_UNITS nUnits = TDCU_HOURS);
	BOOL SetSelectedTaskAllocBy(const CString& sAllocBy);
	BOOL SetSelectedTaskStatus(const CString& sStatus);
	BOOL SetSelectedTaskAllocTo(const CStringArray& aAllocTo);
	BOOL SetSelectedTaskCategories(const CStringArray& aCats);
	BOOL SetSelectedTaskDependencies(const CStringArray& aDepends);
	BOOL SetSelectedTaskTags(const CStringArray& aTags);
	BOOL SetSelectedTaskFileRef(const CString& sFilePath);
	BOOL SetSelectedTaskFileRefs(const CStringArray& aFilePaths);
	BOOL AppendSelectedTaskAllocTo(const CStringArray& aAllocTo);
	BOOL AppendSelectedTaskCategories(const CStringArray& aCats);
	BOOL AppendSelectedTaskDependencies(const CStringArray& aDepends);
	BOOL AppendSelectedTaskTags(const CStringArray& aTags);
	BOOL AppendSelectedTaskFileRef(const CString& sFilePath);
	BOOL AppendSelectedTaskFileRefs(const CStringArray& aFilePaths);
	BOOL SetSelectedTaskPriority(int nPriority); // 0-10 (10 is highest)
	BOOL SetSelectedTaskRisk(int nRisk); // 0-10 (10 is highest)
	BOOL SetSelectedTaskExtID(const CString& sID);
	BOOL SetSelectedTaskFlag(BOOL bFlagged);
	BOOL SetSelectedTaskCost(double dCost);
	BOOL SetSelectedTaskVersion(const CString& sVersion);
	BOOL SetSelectedTaskComments(const CString& sComments, const CBinaryData& customComments);
	BOOL SetSelectedTaskIcon(const CString& sIcon); 
	BOOL SetSelectedTaskDate(TDC_DATE nDate, const COleDateTime& date);
	BOOL SetSelectedTaskCustomAttributeData(const CString& sAttribID, const CString& sData);

	BOOL CanClearSelectedTaskFocusedAttribute() const;
	BOOL ClearSelectedTaskFocusedAttribute();
	BOOL CanClearSelectedTaskAttribute(TDC_ATTRIBUTE nAttrib) const;
	BOOL ClearSelectedTaskAttribute(TDC_ATTRIBUTE nAttrib);

	BOOL IncrementSelectedTaskPercentDone(BOOL bUp = TRUE); // +ve or -ve
	BOOL IncrementSelectedTaskPriority(BOOL bUp = TRUE); // +ve or -ve
	void SetPercentDoneIncrement(int nAmount);

	// time tracking
	void PauseTimeTracking(BOOL bPause = TRUE);
	BOOL TimeTrackSelectedTask();
	BOOL CanTimeTrackSelectedTask() const;
	BOOL IsSelectedTaskBeingTimeTracked() const;
	BOOL IsActivelyTimeTracking() const; // this instant
	DWORD GetTimeTrackTaskID(BOOL bActive = TRUE) const;
	CString GetSelectedTaskTimeLogPath() const;
	void EndTimeTracking(BOOL bAllowConfirm);
	void BeginTimeTracking(DWORD dwTaskID);
	BOOL DoAddTimeToLogFile();

	static void SetInheritedParentAttributes(const CTDCAttributeMap& mapAttribs, BOOL bUpdateAttrib);
	void SetDefaultTaskAttributes(const TODOITEM& tdi);

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
	virtual void ResortSelectedTaskParents() { m_taskTree.ResortSelectedTaskParents(); }

	// move functions
	virtual BOOL MoveSelectedTask(TDC_MOVETASK nDirection);
	virtual BOOL CanMoveSelectedTask(TDC_MOVETASK nDirection) const { return m_taskTree.CanMoveSelection(nDirection); }

	virtual BOOL GotoNextTask(TDC_GOTO nDirection); 
	virtual BOOL CanGotoNextTask(TDC_GOTO nDirection) const;
	virtual BOOL GotoNextTopLevelTask(TDC_GOTO nDirection); 
	virtual BOOL CanGotoNextTopLevelTask(TDC_GOTO nDirection) const;

	virtual BOOL CanExpandTasks(TDC_EXPANDCOLLAPSE nWhat, BOOL bExpand) const;
	virtual void ExpandTasks(TDC_EXPANDCOLLAPSE nWhat, BOOL bExpand = TRUE);

	// copy/paste functions
	BOOL CanPasteText(); // into focused control
	BOOL PasteText(const CString& sText); // into focused control
	BOOL CutSelectedTask();
	BOOL CopySelectedTask() const;
	void ClearCopiedItem() const;
	BOOL PasteTasks(TDC_PASTE nWhere, BOOL bAsRef);
	BOOL CanPasteTasks(TDC_PASTE nWhere, BOOL bAsRef) const;

	void ResetFileVersion(unsigned int nTo = 0) { m_nFileVersion = max(nTo, 0); }
	DWORD GetFileVersion() const { return m_nFileVersion == 0 ? 1 : m_nFileVersion; }
    TDC_FILEFMT CompareFileFormat() const; // older, same, newer
	
	// tree related
	inline HTREEITEM GetSelectedItem() const { return m_taskTree.GetSelectedItem(); }
	inline UINT GetTaskCount() const { return m_taskTree.GetTaskCount(); }
	inline BOOL ItemHasChildren(HTREEITEM hti) const { return m_taskTree.ItemHasChildren(hti); }
	inline BOOL ItemHasParent(HTREEITEM hti) const { return (NULL != m_taskTree.ItemHasParent(hti)); }
	inline int GetSelectedCount() const { return m_taskTree.GetSelectedCount(); }
	inline BOOL IsItemSelected(HTREEITEM hti) const { return m_taskTree.IsItemSelected(hti); }
	inline BOOL HasSelection() const { return m_taskTree.HasSelection(); }
	BOOL IsTaskLabelEditing() const;

	virtual BOOL TasksHaveFocus() const { return m_taskTree.HasFocus(); }
	virtual void SetFocusToTasks();
	virtual void SetFocusToComments();
	virtual CString GetControlDescription(const CWnd* pCtrl) const;
	virtual BOOL GetSelectionBoundingRect(CRect& rSelection) const;

	void SelectItem(HTREEITEM hti);
	void SelectAll();
	BOOL SelectTask(CString sPart, TDC_SELECTTASK nSlect);
	BOOL SelectedTasksHaveChildren() const { return m_taskTree.SelectionHasSubtasks(); }
	BOOL SelectedTasksHaveIcons() const { return m_taskTree.SelectionHasIcons(); }
	BOOL SelectedTasksAreAllDone() const { return m_taskTree.SelectionAreAllDone(); }

	BOOL CanSelectTasksInHistory(BOOL bForward) const { return m_taskTree.CanSelectTasksInHistory(bForward); }
	void SelectTasksInHistory(BOOL bForward);

	BOOL SetTreeFont(HFONT hFont); // setter responsible for deleting
	BOOL SetCommentsFont(HFONT hFont); // setter responsible for deleting

	const CImageList& GetCheckImageList() const { return m_taskTree.GetCheckImageList(); }
	const CTDCImageList& GetTaskIconImageList() const { return m_ilTaskIcons; }

	int FindTasks(const SEARCHPARAMS& params, CResultArray& aResults) const;
	BOOL HasTask(DWORD dwTaskID) const { return m_data.HasTask(dwTaskID); }
	BOOL HasOverdueTasks() const;
	BOOL HasDueTodayTasks() const;

	// undo/redo
	BOOL UndoLastAction(BOOL bUndo);
	BOOL CanUndoLastAction(BOOL bUndo) const;

	// misc
	void Spellcheck();
	void SetMaxInfotipCommentsLength(int nLength) { m_taskTree.SetMaxInfotipCommentsLength(nLength); }
	COleDateTime GetLastTaskModified() const { return m_dtLastTaskMod; }
	void RedrawReminders();
	void SetLayoutPositions(TDC_UILOCATION nControlsPos, TDC_UILOCATION nCommentsPos, BOOL bResize);
	void SetCompletionStatus(const CString& sStatus);

	BOOL ParseTaskLink(const CString& sLink, DWORD& dwTaskID, CString& sFile) const;
	static BOOL ParseTaskLink(const CString& sLink, const CString& sFolder, DWORD& dwTaskID, CString& sFile);

	void SetAlternatePreferencesKey(const CString& sKey) { m_sAltPrefsKey = sKey; }
	CString GetPreferencesKey(const CString& sSubKey = _T("")) const;

	virtual void NotifyBeginPreferencesUpdate() {}
	virtual void NotifyEndPreferencesUpdate() {}
	virtual void UpdateVisibleColumns();
	virtual TDC_HITTEST HitTest(const CPoint& ptScreen) const;
	virtual TDC_COLUMN ColumnHitTest(const CPoint& ptScreen) const;

	static BOOL IsReservedShortcut(DWORD dwShortcut);
	static void EnableExtendedSelection(BOOL bCtrl, BOOL bShift);
	static void SetRecentlyModifiedPeriod(const COleDateTimeSpan& dtSpan);

protected:
	enum  // visible for derived classes
	{ 
		TIMER_TRACK = 1, 
		TIMER_MIDNIGHT,
		TIMER_LAST
	};
	
protected:
	CAutoComboBox m_cbAllocBy;
	CAutoComboBox m_cbStatus;
	CAutoComboBox m_cbVersion;
	CCheckComboBox m_cbCategory, m_cbAllocTo, m_cbTags;
	CColourPicker m_cpColour;
	CContentCtrl m_ctrlComments;
	CTDLContentTypeComboBox m_cbCommentsType;
	CDateTimeCtrlEx m_dtcStart, m_dtcDue, m_dtcDone;
	CEnEdit m_eExternalID, m_eDependency;
	CFileComboBox m_cbFileRef;
	CMaskEdit m_ePercentDone, m_eCost;
	CPopupEditCtrl m_eTaskName;
	CSpinButtonCtrl m_spinPercent;
	CTDLPriorityComboBox m_cbPriority;
	CTDLRecurringTaskEdit m_eRecurrence;
	CTDLRiskComboBox m_cbRisk;
	CTimeComboBox m_cbTimeDue, m_cbTimeStart, m_cbTimeDone;
	CTimeEdit m_eTimeEstimate, m_eTimeSpent;
	
	CTDLTaskTreeCtrl m_taskTree;

	HFONT m_hFontTree, m_hFontComments;
	CTDCImageList m_ilTaskIcons;
	CBrush m_brUIBack;
	CUIThemeFile m_theme;
	HICON m_hIconClock, m_hIconLink;

	CWordArray m_aStyles;
	CString m_sXmlHeader, m_sXslHeader;
	CTaskListDropTarget m_dtTree, m_dtFileRef;
	CString m_sLastSavePath;
	CString m_sAltPrefsKey;
	int m_nCommentsSize;
	CString m_sPassword;
	CString m_sCompletionStatus;
	CTreeDragDropHelper m_treeDragDrop;
	CWndPromptManager m_mgrPrompts;
	COleDateTime m_dtLastTaskMod;
	CContentMgr& m_mgrContent;
	TDCAUTOLISTDATA m_tldDefault;
	TDC_MAXSTATE m_nMaxState;
	TDC_UILOCATION m_nControlsPos, m_nCommentsPos;
	int m_nPercentIncrement;
	TDCCOLEDITFILTERVISIBILITY m_visColAttrib;
	TODOITEM m_tdiDefault;
	TDI_RECURFROMOPTION m_nDefRecurFrom;
	TDI_RECURREUSEOPTION m_nDefRecurReuse;
	CDWordArray m_aRecreateTaskIDs;

	enum COMMENTS_STATE
	{
		CS_CLEAN,
		CS_PENDING,
		CS_CHANGED,
	};
	COMMENTS_STATE m_nCommentsState;

	CToDoCtrlData m_data;

	CStringArray m_aFileRefs;
	CString m_sTextComments;
	CString m_sAllocBy;
	CString m_sStatus;
	CStringArray m_aCategory, m_aAllocTo, m_aTags;
	CString m_sProjectName;
	CString m_sExternalID, m_sDepends;
	CString m_sOccurrence;
	CString m_sVersion;
	CBinaryData m_customComments;
	double m_dTimeEstimate, m_dTimeSpent;
	double m_dCost;
	double m_dLogTime; // in hours
	int m_nPriority;
	int m_nRisk;
	int m_nPercentDone;
	TDC_UNITS m_nTimeEstUnits, m_nTimeSpentUnits;
	CONTENTFORMAT m_cfComments, m_cfDefault;
	TDCRECURRENCE m_tRecurrence;
	COLORREF m_crColour;
	CMapStringToString m_mapMetaData;
	CMapStringToString m_mapCustomCtrlData;

	CTDCCustomAttribDefinitionArray m_aCustomAttribDefs;
	CTDCCustomControlArray m_aCustomControls;

	DWORD m_dwNextUniqueID;
	DWORD m_nFileVersion;
	DWORD m_nFileFormat;
	DWORD m_dwTimeTrackTaskID;
	DWORD m_dwTickLast; // time tracking
	DWORD m_dwLastAddedID;
	DWORD m_dwEditTitleTaskID;

	BOOL m_bModified;
	BOOL m_bArchive;
	BOOL m_bCheckedOut; // intentionally not a style
	BOOL m_bSplitting; // dragging comments splitter
	BOOL m_bTimeTrackingPaused;
	BOOL m_bSourceControlled;
	BOOL m_bDragDropSubtasksAtTop;
	BOOL m_bDelayLoaded;
	BOOL m_bFirstLoadCommentsPrefs;
//	BOOL m_bIsUnicode;

	static int s_nCommentsSize; // TDCS_SHAREDCOMMENTSHEIGHT

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToDoCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual BOOL OnInitDialog();
	virtual void OnTimerMidnight();

	// Implementation
protected:
	void UpdateComments(BOOL bSaveAndValidate); 
	
	// private CToDoCtrl messages
	static UINT WM_TDC_FIXUPPOSTDROPSELECTION;
	static UINT WM_TDC_REFRESHPERCENTSPINVISIBILITY;
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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStartDatechange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDueDatechange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCompletionDatechange(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnTreeSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeChangeFocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeGetInfoTip(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg LRESULT OnColumnEditClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTreeDragDrop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTreeDragOver(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnCommentsChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsDoHelp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsKillFocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnApplyAddLoggedTime(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCGetTaskReminder(WPARAM wp, LPARAM lp);

	afx_msg void OnChangePriority();
	afx_msg void OnChangePercent();
	afx_msg void OnChangeTimeEstimate();
	afx_msg void OnChangeTimeSpent();
	afx_msg void OnSelChangeDueTime();
	afx_msg void OnSelChangeDoneTime();
	afx_msg void OnSelChangeStartTime();
	afx_msg void OnSelChangeAllocTo();
	afx_msg void OnSelChangeAllocBy();
	afx_msg void OnSelChangeStatus();
	afx_msg void OnSelChangeVersion();
	afx_msg void OnSelChangeCategory();
	afx_msg void OnSelChangeFileRefPath();
	afx_msg void OnCancelChangeFileRefPath();
	afx_msg void OnSelChangeTag();
	afx_msg void OnSelCancelAllocTo();
	afx_msg void OnSelCancelCategory();
	afx_msg void OnSelCancelTag();
	afx_msg void OnChangeRisk();
	afx_msg void OnChangeProjectName();
	afx_msg void OnChangeCost();
	afx_msg void OnChangeDependency();
	afx_msg void OnChangeExternalID();
	afx_msg void OnChangeRecurrence();
 	afx_msg LRESULT OnGutterNotifyScroll(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEEBtnClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCustomUrl(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTimeUnitsChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDropObject(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFileEditWantIcon(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFileEditDisplayFile(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGotoFileRef();
	afx_msg LRESULT OnTDCHasClipboard(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCGetClipboard(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCTaskIsDone(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCDoTaskLink(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTDCFailedLink(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAutoComboAddDelete(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetFont(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsWantSpellCheck(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelChangeCommentsType();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnFixupPostDropSelection(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnRefreshPercentSpinVisibility(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnChangeColour(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnEditEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditCancel(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecreateRecurringTask(WPARAM wParam, LPARAM lParam);

	// custom data notifications
	afx_msg void OnCustomAttributeChange(UINT nCtrlID, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomAttributeChange(UINT nCtrlID);
	DECLARE_MESSAGE_MAP()

	inline const TODOITEM* GetTask(DWORD dwTaskID) const { return m_taskTree.GetTask(dwTaskID); }
	inline DWORD GetTaskID(HTREEITEM hti) const { return m_taskTree.GetTaskID(hti); }
	inline DWORD GetTrueTaskID(HTREEITEM hti) const { return m_taskTree.GetTrueTaskID(hti); }
	inline BOOL IsTaskReference(DWORD dwTaskID) const { return m_data.IsTaskReference(dwTaskID); }
	
	inline const CToDoCtrlFind& TCF() const { return m_taskTree.Find(); }
	
	inline CTreeCtrlHelper& TCH() { return m_taskTree.TCH(); }
	inline const CTreeCtrlHelper& TCH() const { return m_taskTree.TCH(); }
	
	inline CTreeSelectionHelper& TSH() { return m_taskTree.TSH(); }
	inline const CTreeSelectionHelper& TSH() const { return m_taskTree.TSH(); }

	virtual void InvalidateItem(HTREEITEM hti, BOOL bUpdate = FALSE) { m_taskTree.InvalidateItem(hti, bUpdate); }
	virtual void UpdateSelectedTaskPath();
	virtual BOOL SetStyle(TDC_STYLE nStyle, BOOL bOn, BOOL bWantUpdate);
	virtual void EndLabelEdit(BOOL bCancel);
	virtual BOOL GetLabelEditRect(CRect& rScreen);
	virtual void SetEditTitleTaskID(DWORD dwTaskID);
	virtual void EndTimeTracking(BOOL bAllowConfirm, BOOL bNotify);
	virtual void BeginTimeTracking(DWORD dwTaskID, BOOL bNotify);

	void ResetTimeTracking() { m_dwTickLast = GetTickCount(); }
	void UpdateTask(TDC_ATTRIBUTE nAttrib, DWORD dwFlags = 0);
	void UpdateControls(BOOL bIncComments = TRUE, HTREEITEM hti = NULL);
	void SetCtrlDate(CDateTimeCtrl& ctrl, const COleDateTime& date, const COleDateTime& dateMin = 0.0);
	BOOL CreateContentControl(BOOL bResendComments);
	int GetMaxTaskCategoryWidth(CDC* pDC, BOOL bVisibleOnly = TRUE);
	int GetMaxTaskTagWidth(CDC* pDC, BOOL bVisibleOnly = TRUE);
	void IncrementTrackedTime(BOOL bEnding);
	int CalcMaxDateColWidth(TDC_DATE nDate, CDC* pDC);
	int GetNextPercentDone(int nPercent, BOOL bUp);
	BOOL ShowLabelEdit(const CRect& rPos);
	BOOL UpdateCommentsFont(BOOL bResendComments);

	// internal versions so we can tell how we've been called
	BOOL SetSelectedTaskComments(const CString& sComments, const CBinaryData& customComments, BOOL bInternal);
	BOOL SetSelectedTaskDependencies(const CStringArray& aDepends, BOOL bAppends);
	int SetTaskDone(HTREEITEM hti, const COleDateTime& date, BOOL bAndSubtasks, BOOL bUpdateAllSubtaskDates);
	BOOL SetSelectedTaskDone(const COleDateTime& date, BOOL bDateEdited);
	int CheckWantSubtasksCompleted();
	BOOL MoveSelectedTaskDates(const COleDateTime& dtNewStart, BOOL bFailOnNoChange);
	BOOL SetSelectedTaskDate(TDC_DATE nDate, const COleDateTime& date, BOOL bDateEdited);
	BOOL SetSelectedTaskCustomAttributeData(const CString& sAttribID, const CString& sData, BOOL bCtrlEdited);
	int GetSelectedTaskFileRefs(CStringArray& aFiles, BOOL bFullPath) const;
	CString GetSelectedTaskFileRef(int nFile, BOOL bFullPath) const;
	BOOL SetSelectedTaskTimeEstimateUnits(TDC_UNITS nUnits, BOOL bRecalcTime);
	BOOL SetSelectedTaskTimeSpentUnits(TDC_UNITS nUnits, BOOL bRecalcTime);
	BOOL SetSelectedTaskFileRefs(const CStringArray& aFilePaths, BOOL bAppend, BOOL bCtrlEdited);
	BOOL SetSelectedTaskFileRef(const CString& sFilePath, BOOL bAppend);

	virtual TODOITEM* CreateNewTask(HTREEITEM htiParent); // overridable
	virtual BOOL DeleteSelectedTask(BOOL bWarnUser, BOOL bResetSel = FALSE);

	virtual void SetModified(BOOL bMod, TDC_ATTRIBUTE nAttrib, DWORD dwModTaskID = 0);

	void SaveGlobals(CTaskFile& file) const;
	void LoadGlobals(const CTaskFile& file);

	void SaveCustomAttributeDefinitions(CTaskFile& file) const;
	void LoadCustomAttributeDefinitions(const CTaskFile& file);
	virtual void RebuildCustomAttributeUI();

	virtual BOOL ModNeedsResort(TDC_ATTRIBUTE nModType) const;

	BOOL HandleCustomColumnClick(TDC_COLUMN nColID);
	UINT MapColumnToCtrlID(TDC_COLUMN nColID) const;
	TDC_COLUMN MapCtrlIDToColumn(UINT nCtrlID) const;
	CString GetSourceControlID(BOOL bAlternate = FALSE) const;
	BOOL MatchesSourceControlID(const CString& sID) const;

	virtual BOOL CopyCurrentSelection() const;
	BOOL IsClipboardEmpty(BOOL bCheckID = FALSE) const;
	CString GetClipboardID() const;
	BOOL GetClipboardID(CString& sClipID, BOOL bArchive) const;

	virtual void Resize(int cx = 0, int cy = 0, BOOL bSplitting = FALSE);
	virtual void UpdateTasklistVisibility();
	virtual void OnStylesUpdated() { m_taskTree.OnStylesUpdated(); }
	virtual void OnTaskIconsChanged() { m_taskTree.OnImageListChange(); }

	int GetControls(CTDCControlArray& aControls, BOOL bVisible) const;
	BOOL IsCtrlShowing(const CTRLITEM& ctrl) const;
	void ShowHideControls();
	void ShowHideControl(const CTRLITEM& ctrl);
	void EnableDisableControls(HTREEITEM hti);
	void EnableDisableControl(const CTRLITEM& ctrl, DWORD dwTaskID, BOOL bEnable, BOOL bReadOnly, BOOL bIsParent, BOOL bEditTime);
	void EnableDisableCustomControl(const CUSTOMATTRIBCTRLITEM& ctrl, DWORD dwTaskID, BOOL bEnable, BOOL bReadOnly);
	BOOL GetColumnAttribAndCtrl(TDC_COLUMN nCol, TDC_ATTRIBUTE& nAttrib, CWnd*& pWnd) const;
	CWnd* GetAttributeCtrl(TDC_ATTRIBUTE nAttrib) const;

	void ReposControl(const CTRLITEM& ctrl, CDeferWndMove* pDWM, const CDlgUnits* pDLU, 
						const CRect& rItem, int nClientRight);
	void ReposControls(CDeferWndMove* pDWM, CRect& rAvailable /*in/out*/, BOOL bSplitting);
	void ReposComments(CDeferWndMove* pDWM, CRect& rAvailable /*in/out*/);
	BOOL IsCommentsVisible(BOOL bActually = FALSE) const;
	void ReposProjectName(CDeferWndMove* pDWM, CRect& rAvailable /*in/out*/);
	virtual void ReposTaskTree(CDeferWndMove* pDWM, const CRect& rAvailable /*in*/);
	BOOL CalcRequiredControlsRect(const CRect& rAvailable, CRect& rRequired, int& nCols, int& nRows, BOOL bPreserveSplitPos) const;
	BOOL GetStackCommentsAndControls() const;
	int CalcMinCommentSize() const;
	int CalcMaxCommentSize() const;
	CRect GetSplitterRect() const;
	BOOL IsSplitterVisible() const;
	void ValidateCommentsSize();
	COLORREF GetTaskTextColor(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS) const;

	int AddTreeChildrenToTaskFile(HTREEITEM hti, CTaskFile& file, HTASKITEM hTask, const TDCGETTASKS& filter) const;
	BOOL AddTreeItemToTaskFile(HTREEITEM hti, CTaskFile& file, HTASKITEM hParentTask, const TDCGETTASKS& filter, BOOL bWantSubtasks = TRUE, DWORD dwParentID = 0) const;
	BOOL AddItemAndParentToTaskFile(HTREEITEM hti, CTaskFile& tasks, const TDCGETTASKS& filter, BOOL bAllParents, BOOL bWantSubtasks) const;
	BOOL SetTaskAttributes(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CTaskFile& file, HTASKITEM hTask, const TDCGETTASKS& filter, BOOL bTitleCommentsOnly) const;

//	typedef BOOL (CALLBACK *PFNWANTADDTASK)(const CToDoCtrlData&, DWORD);

	BOOL AddTaskToTaskFile(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CTaskFile& tasks, HTASKITEM hParentTask, BOOL bIncDuplicateCompletedRecurringSubtasks) const;
	BOOL AddSubTasksToTaskFile(const TODOSTRUCTURE* pTDSParent, CTaskFile& tasks, HTASKITEM hParentTask, BOOL bIncDuplicateCompletedRecurringSubtasks) const;
	BOOL SetAllTaskAttributes(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, CTaskFile& file, HTASKITEM hTask) const;

	HTREEITEM AddTaskToTreeItem(const CTaskFile& file, HTASKITEM hTask, HTREEITEM htiParent = NULL, HTREEITEM htiAfter = TVI_LAST, TDC_RESETIDS nResetID = TDCR_NO);
	virtual BOOL AddTasksToTree(const CTaskFile& tasks, HTREEITEM htiDest, HTREEITEM htiDestAfter, TDC_RESETIDS nResetID, BOOL bSelectAll, TDC_ATTRIBUTE nModAttrib);
	HTREEITEM InsertItem(const CString& sText, HTREEITEM htiParent, HTREEITEM htiAfter, BOOL bEdit, DWORD dwDependency);

	virtual BOOL LoadTasks(const CTaskFile& file);
	BOOL CheckRestoreBackupFile(const CString& sFilePath);

	void SaveSplitPos(CPreferences& prefs) const;
	void LoadSplitPos(const CPreferences& prefs);
	void SaveDefaultRecurrence(CPreferences& prefs) const;
	void LoadDefaultRecurrence(const CPreferences& prefs);

	virtual void SaveTasksState(CPreferences& prefs, BOOL bRebuildingTree = FALSE) const; // keyed by last filepath
	virtual HTREEITEM LoadTasksState(const CPreferences& prefs, BOOL bRebuildingTree = FALSE); // returns the previously selected item if any

	void LoadAttributeVisibility(const CPreferences& prefs);
	void SaveAttributeVisibility(CPreferences& prefs) const;

	void TimeTrackTask(HTREEITEM hti);
	BOOL AddTimeToTaskLogFile(DWORD dwTaskID, double dHours, const COleDateTime& dtWhen, const CString& sComment, BOOL bTracked, BOOL bAddToTimeSpent = FALSE);
	BOOL DoAddTimeToLogFile(DWORD dwTaskID, double dHours, BOOL bShowDialog);

	BOOL SetTextChange(int nChange, CString& sItem, const CString& sNewItem, TDC_ATTRIBUTE nAttrib, UINT nIDC, DWORD dwTaskID, CAutoComboBox* pCombo = NULL);
	TDC_SET SetSelectedTaskArray(TDC_ATTRIBUTE nAttrib, const CStringArray& aItems, BOOL bAppend, DWORD& dwRefTaskID);
	BOOL SetSelectedTaskArray(TDC_ATTRIBUTE nAttrib, const CStringArray& aItems, BOOL bAppend, CCheckComboBox& combo);

	BOOL SpellcheckItem(HTREEITEM hti, CSpellCheckDlg* pSpellChecker, BOOL bTitle, BOOL bNotifyNoErrors);
	BOOL SpellcheckItem(HTREEITEM hti, CSpellCheckDlg* pSpellChecker);
	BOOL CanSpellcheckComments();

	BOOL GotoFile(const CString& sFile, BOOL bShellExecute = TRUE);
	BOOL ShowTaskLink(const CString& sLink);
	void MakeRelativePaths(CStringArray& aFilePaths) const;
	void MakeFullPaths(CStringArray& aFilePaths) const;
	CString GetLastSaveFolder() const;

	inline const TODOITEM* GetSelectedTask() const { return m_taskTree.GetSelectedTask(); }

	void HandleUnsavedComments();
	BOOL UndoLastActionItems(const CArrayUndoElements& aElms);
	void LoadTaskIcons();
	void InitEditPrompts();
	BOOL ConfirmDeleteAllTasks(BOOL bSelected = FALSE) const;

	void ShowTaskHasIncompleteDependenciesError(const CString& sIncomplete);
	void ShowTaskHasCircularDependenciesError(const CDWordArray& aTaskIDs) const;

	BOOL MoveSelection(TDC_MOVETASK nDirection);

	typedef CMap<DWORD, DWORD, DWORD, DWORD&> CMapID2ID;
	void PrepareTasksForPaste(CTaskFile& tasks, TDC_RESETIDS nResetID) const;
	void BuildTaskIDMapForPaste(CTaskFile& tasks, HTASKITEM hTask, DWORD& dwNextID, 
								CMapID2ID& mapID, TDC_RESETIDS nResetID, BOOL bAndSiblings) const;
	void PrepareTasksForPaste(CTaskFile& tasks, HTASKITEM hTask, const CMapID2ID& mapID, BOOL bAndSiblings) const;
	BOOL PrepareTaskLinkForPaste(CString& sLink, const CMapID2ID& mapID) const;
	void PrepareTaskIDsForPasteAsRef(CTaskFile& tasks) const;
	void RemoveNonSelectedTasks(CTaskFile& tasks, HTASKITEM hTask) const;

	virtual int GetArchivableTasks(CTaskFile& tasks, BOOL bSelectedOnly = FALSE) const;
	void RemoveArchivedTasks(const CTaskFile& tasks, TDC_ARCHIVE nRemove, BOOL bRemoveFlagged);
	BOOL RemoveArchivedTask(const CTaskFile& tasks, HTASKITEM hTask, TDC_ARCHIVE nRemove, BOOL bRemoveFlagged);
	virtual BOOL RemoveArchivedTask(DWORD dwTaskID);
	BOOL ArchiveTasks(const CString& sArchivePath, const CTaskFile& tasks); // helper to avoid code dupe
	void PrepareTaskfileForTasks(CTaskFile& tasks, const TDCGETTASKS& filter) const;

	void SearchAndExpand(const SEARCHPARAMS& params, BOOL bExpand);
	void AppendTaskFileHeader(CTaskFile& tasks) const;

	void DrawSplitter(CDC* pDC);

	// used for building/creating the tree for saving/loading
	// not for overriding
	int GetAllTasks(CTaskFile& tasks) const;
	HTREEITEM SetAllTasks(const CTaskFile& tasks);

	virtual BOOL SelectTask(DWORD dwTaskID, BOOL bTrue);
	virtual BOOL SelectTasks(const CDWordArray& aTaskIDs, BOOL bTrue);

	virtual HTREEITEM RebuildTree(const void* pContext = NULL);
	virtual BOOL BuildTreeItem(HTREEITEM hti, const TODOSTRUCTURE* pTDS, const void* pContext);
	virtual BOOL WantAddTask(const TODOITEM* pTDI, const TODOSTRUCTURE* pTDS, const void* pContext) const;
	HTREEITEM InsertTreeItem(const TODOITEM* pTDI, DWORD dwID, HTREEITEM htiParent, HTREEITEM htiAfter);

	void AdjustNewRecurringTasksDates(DWORD dwPrevTaskID, DWORD dwNewTaskID, const COleDateTime& dtNext, BOOL bDueDate);
	void InitialiseNewRecurringTask(DWORD dwPrevTaskID, DWORD dwNewTaskID, HTREEITEM hti,
									const COleDateTime& dtNext, BOOL bDueDate);

	int CreateTasksFromOutlookObjects(const TLDT_DATA* pData);

	TDC_ATTRIBUTE GetFocusedControlAttribute() const;
	void SetDefaultComboNames(CAutoComboBox& combo, const CStringArray& aNewNames, 
								CStringArray& aDefNames, BOOL bReadOnly, BOOL bAddEmpty);
	void BuildTasksForSave(CTaskFile& tasks, BOOL bFirstSave);
	BOOL SetComboReadOnly(CAutoComboBox& combo, BOOL bReadOnly, const CStringArray& aDefContent, BOOL bAddEmpty);
	
	static void SetDefaultListContent(CAutoComboBox& combo, const CStringArray& aNewDefs, const CStringArray& aOldDefs, BOOL bAddEmpty = FALSE);
	static void AddUserListContent(CAutoComboBox& combo, const CStringArray& aItems);
	static TDC_FILE MapTaskfileError(int nFileErr);
	static BOOL XMLHeaderIsUnicode(LPCTSTR szXmlHeader);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TODOCTRL_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
