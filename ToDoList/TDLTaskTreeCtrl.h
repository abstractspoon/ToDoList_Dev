// TDCTreeListCtrl.h: interface for the CTDCTreeListCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCTREELISTCTRL_H__155791A3_22AC_4083_B933_F39E9EBDADEF__INCLUDED_)
#define AFX_TDCTREELISTCTRL_H__155791A3_22AC_4083_B933_F39E9EBDADEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

#include "tdcmsg.h"
#include "tdcstruct.h"
#include "todoctrlfind.h"
#include "todoctrldata.h"
#include "tdltaskctrlbase.h"

/////////////////////////////////////////////////////////////////////////////

#include "..\Shared\EnHeaderCtrl.h"
#include "..\Shared\TreeListSyncer.h"
#include "..\Shared\Treeselectionhelper.h"
#include "..\Shared\runtimedlg.h"
#include "..\shared\sysimagelist.h"

/////////////////////////////////////////////////////////////////////////////

#include <afxtempl.h>
typedef CArray<int, int> CIntArray;

/////////////////////////////////////////////////////////////////////////////

class CTreeCtrlHelper;
class CToDoCtrlData;
class CToDoCtrlFind;
class CPreferences;

/////////////////////////////////////////////////////////////////////////////

class CTDLTaskTreeCtrl : public CTDLTaskCtrlBase  
{
	DECLARE_DYNAMIC(CTDLTaskTreeCtrl);

public:
	CTDLTaskTreeCtrl(const CTDCImageList& ilIcons,
					const CToDoCtrlData& data, 
					const CWordArray& aStyles,
					const CTDCColumnIDMap& mapVisibleCols,
					const CTDCCustomAttribDefinitionArray& aCustAttribDefs);

	virtual ~CTDLTaskTreeCtrl();
	operator HWND() const { return GetSafeHwnd(); }
	
	inline CTreeCtrl& Tree() { return m_tcTasks; }
	inline const CTreeCtrl& Tree() const { return m_tcTasks; }
	
	inline CTreeCtrlHelper& TCH() { return m_tch; }
	inline const CTreeCtrlHelper& TCH() const { return m_tch; }
	
	inline CTreeSelectionHelper& TSH() { return m_tsh; }
	inline const CTreeSelectionHelper& TSH() const { return m_tsh; }
	
	inline const CToDoCtrlFind& Find() const { return m_find; }

	inline const TODOITEM* GetSelectedTask() const { return m_find.GetTask(GetSelectedItem()); }
	inline const TODOITEM* GetTask(HTREEITEM hti, BOOL bTrue = TRUE) const { return m_find.GetTask(hti, bTrue); }
	inline const TODOITEM* GetTask(DWORD dwTaskID) const { return CTDLTaskCtrlBase::GetTask(dwTaskID); }
	inline DWORD GetTaskID(HTREEITEM hti) const { return m_find.GetTaskID(hti); }
	inline DWORD GetTrueTaskID(HTREEITEM hti) const { return m_data.GetTrueTaskID(m_find.GetTaskID(hti)); }
	inline DWORD GetSelectedTaskID() const { return GetTaskID(GetSelectedItem()); }
	inline int GetSelectedCount() const { return TSH().GetCount(); }
	inline BOOL IsItemSelected(HTREEITEM hti) const { return TSH().HasItem(hti); }
	inline BOOL IsTaskSelected(DWORD dwTaskID) const { return TSH().HasItem(dwTaskID); }

	int GetSelectedTaskIDs(CDWordArray& aTaskIDs, BOOL bTrue = FALSE) const;
	int GetSelectedTaskIDs(CDWordArray& aTaskIDs, DWORD& dwFocusedTaskID, BOOL bRemoveChildDupes) const;
	int CacheSelection(TDCSELECTIONCACHE& cache, BOOL bIncBreadcrumbs = TRUE) const;
	BOOL RestoreSelection(const TDCSELECTIONCACHE& cache);
	BOOL SelectTasks(const CDWordArray& aTaskIDs, BOOL bTrue = FALSE);
	BOOL MultiSelectTask(DWORD dwTaskID, BOOL bSelected);
	BOOL EnsureSelectionVisible();
	BOOL SelectTasksInHistory(BOOL bForward);
	BOOL CanSelectTasksInHistory(BOOL bForward) const;

	double CalcSelectedTaskTimeEstimate(TDC_UNITS nUnits = TDCU_HOURS) const;
	double CalcSelectedTaskTimeSpent(TDC_UNITS nUnits = TDCU_HOURS) const;
	double CalcSelectedTaskCost() const;

	BOOL SelectItem(HTREEITEM hti);
	BOOL SelectAll();
	BOOL RemoveOrphanTreeItemReferences(HTREEITEM hti = NULL);
	BOOL InvalidateItem(HTREEITEM hti, BOOL bUpdate = FALSE);
	BOOL InvalidateTask(DWORD dwTaskID, BOOL bUpdate = FALSE);
	BOOL InvalidateSelection(BOOL bUpdate = FALSE);
	DWORD GetTaskParentID(HTREEITEM hti) const;
	void SaveState(CPreferences& prefs, const CString& sKey, BOOL bExpandedOnly = FALSE) const; 
	HTREEITEM LoadState(const CPreferences& prefs, const CString& sKey, BOOL bExpandedOnly = FALSE);
	BOOL SelectDropTarget(HTREEITEM hti) { return m_tcTasks.SelectDropTarget(hti); }
	void ResortSelectedTaskParents();

	BOOL GetInsertLocation(TDC_INSERTWHERE nWhere, HTREEITEM& htiDest, HTREEITEM& htiDestAfter) const;
	BOOL GetInsertLocation(TDC_MOVETASK nDirection, HTREEITEM& htiDest, HTREEITEM& htiDestAfter) const;
	BOOL GetInsertLocation(TDC_MOVETASK nDirection, DWORD& dwDest, DWORD& dwDestAfter) const;
	HTREEITEM InsertItem(DWORD dwTaskID, HTREEITEM htiParent, HTREEITEM htiAfter);

	HTREEITEM MoveItem(HTREEITEM hti, HTREEITEM htiDestParent, HTREEITEM htiDestPrevSibling);
	void MoveSelection(HTREEITEM htiDestParent, HTREEITEM htiDestPrevSibling);
	BOOL MoveSelection(TDC_MOVETASK nDirection);
	BOOL CanMoveSelection(TDC_MOVETASK nDirection) const;
	
	inline HTREEITEM GetSelectedItem() const { return TSH().GetFirstItem(); }
	inline HTREEITEM GetTreeSelectedItem() const { return m_tcTasks.GetSelectedItem(); }
	inline HTREEITEM GetItem(DWORD dwTaskID) const { return m_find.GetItem(dwTaskID); }
	inline int GetItemCount() const { return m_tcTasks.GetCount(); }
	inline int GetVisibleItemCount() const { return m_tcTasks.GetVisibleCount(); }
	inline BOOL ItemHasChildren(HTREEITEM hti) const { return m_tcTasks.ItemHasChildren(hti); }
	inline BOOL ItemHasParent(HTREEITEM hti) const { return (GetParentItem(hti) != NULL); }
	inline BOOL DeleteItem(HTREEITEM hti) { return m_tcTasks.DeleteItem(hti); }
	inline HTREEITEM GetChildItem(HTREEITEM htiParent = NULL) const { return m_tcTasks.GetChildItem(htiParent); }
	inline HTREEITEM GetNextItem(HTREEITEM hti, BOOL bNext = TRUE) const { return m_tcTasks.GetNextItem(hti, (bNext ? TVGN_NEXT : TVGN_PREVIOUS)); }
	inline HTREEITEM GetParentItem(HTREEITEM hti) const { return m_tcTasks.GetParentItem(hti); }
	inline HTREEITEM HitTestItem(POINT point, UINT* pFlags = NULL) const { return m_tcTasks.HitTest(point, pFlags); }

	void GetWindowRect(CRect& rWindow) const { CWnd::GetWindowRect(rWindow); }
	BOOL GetSelectionBoundingRect(CRect& rSelection) const;
	BOOL GetLabelEditRect(CRect& rLabel) const;
	
	void ExpandAll(BOOL bExpand = TRUE);
	void ExpandItem(HTREEITEM hti, BOOL bExpand = TRUE, BOOL bAndChildren = FALSE);
	BOOL CanExpandItem(HTREEITEM hti, BOOL bExpand = TRUE) const;
	int GetExpandedTasks(CDWordArray& aExpanded) const;
	void SetExpandedTasks(const CDWordArray& aExpanded);

	void DeleteAll();
	void DeselectAll();
	BOOL CancelOperation();
	BOOL PreTranslateMessage(MSG* pMsg);

	void OnStyleUpdated(TDC_STYLE nStyle, BOOL bOn, BOOL bDoUpdate);
	void OnStylesUpdated();
	
#ifdef _DEBUG
	void Trace(LPCTSTR szComment);
#else
	inline void Trace(LPCTSTR /*szComment*/) {}
#endif
	
protected:
	CTreeCtrl m_tcTasks;
	CToDoCtrlFind m_find;

	CTreeSelectionHelper m_tsh;
	CTreeCtrlHelper m_tch;
	HTREEITEM m_htiLastHandledLBtnDown;
	WORD m_wKeyPress;
	BOOL m_bMovingItem;
	BOOL m_bEditLabelTimerStarted;

protected:
	// Virtual function overrides
	
protected:
	// Message map functions
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()

protected:
	// base-class overrides
	LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT ScWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	
 	LRESULT OnTreeCustomDraw(NMTVCUSTOMDRAW* pTVCD);

	void OnListSelectionChange(NMLISTVIEW* pNMLV);
	void OnTreeSelectionChange(NMTREEVIEW* pNMTV);

	BOOL IsTreeItemSelected(HWND hwnd, HTREEITEM hti) const;
	int RecalcColumnWidth(int nCol, CDC* pDC) const;
	DWORD GetColumnItemTaskID(int nItem) const;
	void SetTasksImageList(HIMAGELIST hil, BOOL bState, BOOL bOn = TRUE);
	BOOL IsColumnShowing(TDC_COLUMN nColID) const;
	void NotifyParentSelChange(SELCHANGE_ACTION nAction = SC_UNKNOWN);

protected:
	// pseudo-message handlers
	void OnTreeKeyDown(NMTVKEYDOWN* pTVKD);
	LRESULT OnTreeGetDispInfo(NMTVDISPINFO* pTVDI);

protected:
	int GetExpandedTasks(CDWordArray& aExpanded, HTREEITEM hti) const;
	void ExpandList(HTREEITEM hti, int& nNextIndex);
	void CollapseList(HTREEITEM hti);
	void ExpandList(HTREEITEM hti = NULL);
	DWORD GetHelpID() const;

	inline HWND Tasks() const { return m_tcTasks; }
	inline HWND TasksHeader() const { return m_hdrTasks; }
	
	void RedrawTree(BOOL bErase = FALSE);
	void GetWindowRect(CRect& rWindow, BOOL bWithHeader) const;
	BOOL GetItemTitleRect(HTREEITEM hti, TDC_TITLERECT nArea, CRect& rect, CDC* pDC = NULL, LPCTSTR szTitle = NULL) const;

	BOOL IsTreeItemLineOdd(HTREEITEM hti) const;
	BOOL BuildColumns();
	void Release();
	BOOL CreateTasksWnd(CWnd* pParentWnd, const CRect& rect, BOOL bVisible);
	void SyncColumnSelectionToTasks();
	int SaveTreeExpandedState(CPreferences& prefs, const CString& sKey, HTREEITEM hti, int nStart) const; 
	BOOL SelectItem(HTREEITEM hti, BOOL bSyncAndNotify, SELCHANGE_ACTION nBy); // internal version
	int GetListItem(HTREEITEM hti) const;
	DWORD HitTestTasksTask(const CPoint& ptScreen) const;
	BOOL MultiSelectItem(HTREEITEM hti, TSH_SELECT nState = TSHS_SELECT, BOOL bRedraw = TRUE);
	BOOL MultiSelectItems(HTREEITEM htiFrom, HTREEITEM htiTo, TSH_SELECT nState = TSHS_SELECT, BOOL bRedraw = TRUE);
	BOOL HandleClientColumnClick(const CPoint& pt, BOOL bDblClk);
	BOOL CanMoveItem(HTREEITEM hti, TDC_MOVETASK nDirection) const;
	void BeginLabelEditTimer();
	void EndLabelEditTimer();

	GM_ITEMSTATE GetTreeItemState(HTREEITEM hti) const;
	GM_ITEMSTATE GetColumnItemState(int nItem) const;

#ifdef _DEBUG
	void Trace(HTREEITEM hti, int nLevel);
	void Trace(LPCTSTR szComment, HTREEITEM hti);
	LPCTSTR GetDebugName() const { return _T("TaskTree"); }
#endif

};

#endif // !defined(AFX_TDCTREELISTCTRL_H__155791A3_22AC_4083_B933_F39E9EBDADEF__INCLUDED_)
