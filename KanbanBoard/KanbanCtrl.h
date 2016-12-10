// KanbanTreeList.h: interface for the CKanbanTreeList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANBANTREELISTCTRL_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
#define AFX_KANBANTREELISTCTRL_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Kanbanstruct.h"
#include "Kanbanenum.h"
#include "Kanbanlistctrl.h"

#include "..\shared\graphicsmisc.h"
#include "..\shared\fontcache.h"
#include "..\shared\mapex.h"

#include "..\Interfaces\itasklist.h"
#include "..\Interfaces\iuiextension.h"

#include <AFXTEMPL.H>

/////////////////////////////////////////////////////////////////////////////

class CThemed;

/////////////////////////////////////////////////////////////////////////////

class CKanbanCtrl : public CWnd  
{
public:
	CKanbanCtrl(CFontCache& fonts);
	virtual ~CKanbanCtrl();

	BOOL Create(DWORD dwStyle, const RECT &rect, CWnd* pParentWnd, UINT nID);

	void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const CSet<IUI_ATTRIBUTE>& attrib);
	bool PrepareNewTask(ITaskList* pTask) const;

	int GetSelectedTaskIDs(CDWordArray& aTaskIDs) const;
	BOOL SelectTask(DWORD dwTaskID);
	BOOL SelectTasks(const CDWordArray& aTaskIDs);

	DWORD GetNextTask(DWORD dwTaskID, IUI_APPCOMMAND nCmd) const;
	DWORD HitTestTask(const CPoint& ptScreen) const;

	void ResizeColumnsToFit();
	void ScrollToSelectedTask();

	bool ProcessMessage(MSG* pMsg);

 	void Sort(IUI_ATTRIBUTE nBy, BOOL bAllowToggle, BOOL bAscending = -1);
 	BOOL IsSorted() const;
// 	GTLC_COLUMN GetSortColumn() const { return m_nSortBy; }
// 	BOOL GetSortAscending() const { return m_bSortAscending; }

	void SetOption(DWORD dwOption, BOOL bSet = TRUE);
	BOOL HasOption(DWORD dwOption) const { return (m_dwOptions & dwOption); }

	void SetDisplayAttributes(const CKanbanAttributeArray& aAttrib);
	int GetDisplayAttributes(CKanbanAttributeArray& aAttrib) const;

	BOOL CancelOperation();
	void SetReadOnly(bool bReadOnly);
	BOOL GetLabelEditRect(LPRECT pEdit);
	void SetPriorityColors(const CDWordArray& aColors);

 	int GetColumnOrder(CStringArray& aOrder) const;
 	BOOL SetColumnOrder(const CStringArray& aOrder);

	int GetAttributeValues(IUI_ATTRIBUTE nAttrib, CStringArray& aValues) const;
	void LoadDefaultAttributeValues(const IPreferences* pPrefs);
	BOOL TrackAttribute(IUI_ATTRIBUTE nAttrib, const CString& sCustomAttribID, const CKanbanColumnArray& aColumnDefs);
	IUI_ATTRIBUTE GetTrackedAttribute() const { return m_nTrackAttribute; }
	IUI_ATTRIBUTE GetTrackedAttribute(CString& sCustomAttrib) const;

	int GetCustomAttributeIDs(CStringArray& aCustAttribIDs) const;
	int GetAttributeValues(CKanbanAttributeValueMap& mapValues) const;

	static BOOL WantEditUpdate(IUI_ATTRIBUTE nAttribute);
	static BOOL WantSortUpdate(IUI_ATTRIBUTE nAttribute);

protected:
	BOOL m_bDragging;
	BOOL m_bReadOnly;
	BOOL m_bSortAscending;
	BOOL m_bStrikeThruDone;

	DWORD m_dwOptions;
	UINT m_nNextColor;
	IUI_ATTRIBUTE m_nTrackAttribute, m_nSortBy;
	CString m_sTrackAttribID;
	CFontCache& m_fonts;
	CImageList m_ilHeight;
	CStringArray m_aCustomAttribIDs;
	CDWordArray m_aPriorityColors;

	CArray<int, int> m_aDragItems;
	CKanbanListCtrl* m_pDragFromList;
	CKanbanListCtrl* m_pSelectedList;
	CKanbanListCtrlArray m_aListCtrls;

	CKanbanItemMap m_data;
	CKanbanAttributeValueMap m_mapAttributeValues;
	CKanbanAttributeValueMap m_mapDefAttributeValues;
	CKanbanAttributeArray m_aDisplayAttrib;
	CKanbanColumnArray m_aColumnDefs;

protected:
	// Generated message map functions
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListItemChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginDragListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListSetFocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* );
	
	DECLARE_MESSAGE_MAP()

	BOOL DrawListItemColumn(CDC* pDC, int nItem, int nCol, DWORD dwTaskID, BOOL bSelected);
	GM_ITEMSTATE GetItemState(int nItem) const;
	
	void RedrawListCtrls(BOOL bErase = FALSE);
	void RemoveDeletedTasks(const ITaskList15* pTasks);
	void Resize();
	void Resize(const CRect& rect);
	void ClearOtherListSelections(const CKanbanListCtrl* pList);

	void RebuildListCtrls(BOOL bRebuildData);
	void RebuildDynamicListCtrls(const CKanbanItemArrayMap& mapKIArray);
	void RebuildFixedListCtrls(const CKanbanItemArrayMap& mapKIArray);
	void RemoveOldDynamicListCtrls(const CKanbanItemArrayMap& mapKIArray);
	void AddMissingDynamicListCtrls(const CKanbanItemArrayMap& mapKIArray);
	void CheckAddBacklogListCtrl();
	void RebuildListCtrlData(const CKanbanItemArrayMap& mapKIArray);
	int GetVisibleListCtrlCount() const;

	KANBANITEM* GetKanbanItem(DWORD dwTaskID) const;
	BOOL HasKanbanItem(DWORD dwTaskID) const;

	CKanbanListCtrl* LocateTask(DWORD dwTaskID) const;
	CKanbanListCtrl* LocateTask(DWORD dwTaskID, int& nItem) const;
	CKanbanListCtrl* GetListCtrl(const CString& sAttribValue) const;
	CKanbanListCtrl* GetListCtrl(HWND hwnd) const;
	CKanbanListCtrl* HitTestListCtrl(const CPoint& ptScreen, BOOL* pbHeader = NULL) const;

	CKanbanListCtrl* NewListCtrl(const KANBANCOLUMN& colDef);
	CKanbanListCtrl* GetSelListCtrl();
	const CKanbanListCtrl* GetSelListCtrl() const;
	const CKanbanListCtrl* GetNextListCtrl(const CKanbanListCtrl* pList, BOOL bNext, BOOL bExcludeEmpty) const;

	BOOL SelectListCtrl(CKanbanListCtrl* pList);
	BOOL IsSelectedListCtrl(HWND hWnd) const;
	void FixupSelection();
	BOOL DeleteListCtrl(int nList);

	DWORD GetTaskID(HTREEITEM hti) const;
	DWORD GetTaskID(int nItem) const;
	DWORD GetListTaskID(DWORD nItemData) const;

	inline BOOL UsingFixedColumns() const { return m_aColumnDefs.GetSize(); }
	inline BOOL UsingDynamicColumns() const { return !UsingFixedColumns(); }

	BOOL IsDragging() const;
	BOOL NotifyParentAttibuteChange(LPCTSTR szValue);
	void NotifyParentSelectionChange();
	BOOL GetListCtrlAttributeValue(CKanbanListCtrl* pDestList, const CPoint& ptScreen, CString& sValue);
	BOOL UpdateTrackableTaskAttribute(KANBANITEM* pKI, IUI_ATTRIBUTE nAttrib, const CString& sNewValue);
	BOOL UpdateTrackableTaskAttribute(KANBANITEM* pKI, IUI_ATTRIBUTE nAttrib, int nNewValue);
	BOOL UpdateTrackableTaskAttribute(KANBANITEM* pKI, const CString& sAttribID, const CString& sNewValue);
	void LoadDefaultAttributeValues(const IPreferences* pPrefs, LPCTSTR szAttribID, LPCTSTR szSubKey);

	BOOL UpdateData(const ITaskList15* pTasks, HTASKITEM hTask, const CSet<IUI_ATTRIBUTE>& attrib, BOOL bAndSiblings);
	BOOL RebuildData(const ITaskList15* pTasks, const CSet<IUI_ATTRIBUTE>& attrib);
	BOOL AddTaskToData(const ITaskList15* pTasks, HTASKITEM hTask, DWORD dwParentID, const CSet<IUI_ATTRIBUTE>& attrib, BOOL bAndSiblings);
	BOOL UpdateGlobalAttributeValues(const ITaskList15* pTasks, const CSet<IUI_ATTRIBUTE>& attrib);
	BOOL UpdateGlobalAttributeValues(const ITaskList15* pTasks, IUI_ATTRIBUTE nAttribute);
	BOOL UpdateGlobalAttributeValues(const ITaskList15* pTasks, LPCTSTR szXMLTag, LPCTSTR szAttribID);

	BOOL WantShowColumn(LPCTSTR szAttribID, const CKanbanItemArrayMap& mapKIArray) const;
	BOOL WantShowColumn(const CKanbanListCtrl* pList) const;

	static COLORREF GetColor(COLORREF crBase, double dLighter, BOOL bSelected);
	static CString GetTaskAllocTo(const ITaskList15* pTasks, HTASKITEM hTask);
	static int Compare(const CString& sText1, const CString& sText2);
	static BOOL RebuildListContents(CKanbanListCtrl* pList, const CKanbanItemArrayMap& mapKIArray, BOOL bShowParents);
	static CString GetXMLTag(IUI_ATTRIBUTE nAttrib);
	static BOOL HasNonParentTasks(const CKanbanItemArray* pItems);
	static void UpdateItemDisplayAttributes(KANBANITEM* pKI, const ITaskList15* pTasks, HTASKITEM hTask, const CSet<IUI_ATTRIBUTE>& attrib);
	static void BuildTaskMap(const ITaskList15* pTasks, HTASKITEM hTask, CSet<DWORD>& mapIDs, BOOL bAndSiblings);

	static int ListSortProc(const void* pV1, const void* pV2);

};

#endif // !defined(AFX_KANBANTREELISTCTRL_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
