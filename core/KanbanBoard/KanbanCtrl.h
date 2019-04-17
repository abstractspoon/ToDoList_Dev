// KanbanTreeList.h: interface for the CKanbanTreeList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANBANCTRL_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
#define AFX_KANBANCTRL_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Kanbanstruct.h"
#include "Kanbanenum.h"
#include "KanbanlistctrlArray.h"

#include "..\shared\graphicsmisc.h"
#include "..\shared\fontcache.h"
#include "..\shared\mapex.h"
#include "..\shared\enheaderctrl.h"

#include "..\Interfaces\itasklist.h"
#include "..\Interfaces\iuiextension.h"

#include <AFXTEMPL.H>

/////////////////////////////////////////////////////////////////////////////

class CThemed;
class CDeferWndMove;

/////////////////////////////////////////////////////////////////////////////

class CKanbanCtrl : public CWnd  
{
public:
	CKanbanCtrl();
	virtual ~CKanbanCtrl();

	BOOL Create(DWORD dwStyle, const RECT &rect, CWnd* pParentWnd, UINT nID);

	void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const CSet<TDC_ATTRIBUTE>& attrib);
	bool PrepareNewTask(ITaskList* pTask) const;

	DWORD GetSelectedTaskID() const;
	BOOL SelectTask(DWORD dwTaskID);
	BOOL SelectTask(IUI_APPCOMMAND nCmd, const IUISELECTTASK& select);

	DWORD GetNextTask(DWORD dwTaskID, IUI_APPCOMMAND nCmd) const;
	DWORD HitTestTask(const CPoint& ptScreen) const;
	void ScrollToSelectedTask();

	BOOL SaveToImage(CBitmap& bmImage);
	BOOL CanSaveToImage() const;

	bool ProcessMessage(MSG* pMsg);
	void FilterToolTipMessage(MSG* pMsg);

 	void Sort(TDC_ATTRIBUTE nBy, BOOL bAscending);

	void SetOptions(DWORD dwOptions);
	DWORD GetOptions() const { return m_dwOptions; }
	BOOL HasOption(DWORD dwOption) const { return (m_dwOptions & dwOption); }

	void SetDisplayAttributes(const CKanbanAttributeArray& aAttrib);
	BOOL CancelOperation();
	void SetReadOnly(bool bReadOnly);
	BOOL GetLabelEditRect(LPRECT pEdit);
	void SetPriorityColors(const CDWordArray& aColors);

	int GetVisibleColumnCount() const;
	int GetVisibleTaskCount() const { return m_aColumns.GetVisibleTaskCount(); }

	int GetTaskTrackedAttributeValues(DWORD dwTaskID, CStringArray& aValues) const;
	int GetAttributeValues(TDC_ATTRIBUTE nAttrib, CStringArray& aValues) const;
	void LoadDefaultAttributeListValues(const IPreferences* pPrefs);
	BOOL TrackAttribute(TDC_ATTRIBUTE nAttrib, const CString& sCustomAttribID, const CKanbanColumnArray& aColumnDefs);
	TDC_ATTRIBUTE GetTrackedAttribute() const { return m_nTrackAttribute; }
	TDC_ATTRIBUTE GetTrackedAttribute(CString& sCustomAttrib) const;

	const CKanbanCustomAttributeDefinitionArray& GetCustomAttributeDefinitions() const { return m_aCustomAttribDefs; }
	int GetAttributeValues(CKanbanAttributeValueMap& mapValues) const;

	BOOL WantSortUpdate(TDC_ATTRIBUTE nAttribute) const;
	BOOL WantEditUpdate(TDC_ATTRIBUTE nAttribute) const;

protected:
	BOOL m_bReadOnly;
	BOOL m_bSortAscending;
	BOOL m_bSelectTasks;
	BOOL m_bSettingColumnFocus;
	BOOL m_bResizingHeader;
	BOOL m_bSavingToImage;

	DWORD m_dwOptions;
	UINT m_nNextColor;
	TDC_ATTRIBUTE m_nTrackAttribute, m_nSortBy;
	CString m_sTrackAttribID;
	CDWordArray m_aPriorityColors;
	CFontCache m_fonts;

	CKanbanColumnCtrl* m_pSelectedColumn;
	CKanbanColumnCtrlArray m_aColumns;
	CEnHeaderCtrl m_header;

	CKanbanItemMap m_data;
	CKanbanAttributeValueMap m_mapAttributeValues;
	CKanbanAttributeValueMap m_mapGlobalAttributeValues;
	CKanbanAttributeArray m_aDisplayAttrib;
	CKanbanColumnArray m_aColumnDefs;
	CKanbanCustomAttributeDefinitionArray m_aCustomAttribDefs;

protected:
	// Generated message map functions
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnColumnItemSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnEditLabel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginDragColumnItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnColumnToggleTaskDone(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnColumnEditTaskIcon(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnColumnToggleTaskFlag(WPARAM wp, LPARAM lp);
	afx_msg void OnColumnSetFocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnColumnGetTaskIcon(WPARAM wp, LPARAM lp);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* );
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnSetFont(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSelectTask(WPARAM wp, LPARAM lp);
	afx_msg void OnHeaderCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHeaderItemChanging(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

	void RemoveDeletedTasks(const ITASKLISTBASE* pTasks);
	void Resize(int cx = 0, int cy = 0);
	void ResizeHeader(CDeferWndMove& dwm, CRect& rAvail);

	void RebuildColumns(BOOL bRebuildData, BOOL bTaskUpdate);
	void RebuildDynamicColumns(const CKanbanItemArrayMap& mapKIArray);
	void RebuildFixedColumns(const CKanbanItemArrayMap& mapKIArray);
	int RemoveOldDynamicColumns(const CKanbanItemArrayMap& mapKIArray);
	int AddMissingDynamicColumns(const CKanbanItemArrayMap& mapKIArray);
	BOOL CheckAddBacklogColumn();
	void RebuildColumnsData(const CKanbanItemArrayMap& mapKIArray);
	void RebuildColumnHeader();

	KBC_ATTRIBLABELS GetColumnAttributeLabelVisibility(int nCol, int nColWidth);
	float GetAverageColumnCharWidth();
	BOOL CanFitAttributeLabels(int nAvailWidth, float fAveCharWidth, KBC_ATTRIBLABELS nLabelVis) const;

	KANBANITEM* GetKanbanItem(DWORD dwTaskID) const;
	BOOL HasKanbanItem(DWORD dwTaskID) const;

	CKanbanColumnCtrl* LocateTask(DWORD dwTaskID, HTREEITEM& hti, BOOL bForward) const;
	CKanbanColumnCtrl* AddNewColumn(const KANBANCOLUMN& colDef);
	CKanbanColumnCtrl* GetSelColumn();
	CKanbanColumnCtrl* GetNextColumn(const CKanbanColumnCtrl* pCol, BOOL bNext, BOOL bExcludeEmpty);

	const CKanbanColumnCtrl* GetSelColumn() const;
	const CKanbanColumnCtrl* GetNextColumn(const CKanbanColumnCtrl* pCol, BOOL bNext, BOOL bExcludeEmpty) const;

	BOOL SelectColumn(CKanbanColumnCtrl* pCol, BOOL bNotifyParent = TRUE);
	BOOL IsSelectedColumn(HWND hWnd) const;
	void FixupSelectedColumn();
	void FixupColumnFocus();
	BOOL DeleteColumn(int nCol);
	BOOL HasFocus() const;
	BOOL SelectClosestAdjacentItemToSelection(int nAdjacentCol);

	inline BOOL UsingFixedColumns() const { return m_aColumnDefs.GetSize(); }
	inline BOOL UsingDynamicColumns() const { return !UsingFixedColumns(); }

	BOOL IsDragging() const;
	BOOL EndDragItem(CKanbanColumnCtrl* pSrcCol, DWORD dwTaskID, CKanbanColumnCtrl* pDestCol, const CString& sDestAttribValue);
	BOOL HandleKeyDown(WPARAM wp, LPARAM lp);
	
	BOOL NotifyParentAttibuteChange(DWORD dwTaskID);
	void NotifyParentSelectionChange();
	BOOL GetColumnAttributeValue(const CKanbanColumnCtrl* pDestCol, const CPoint& ptScreen, CString& sValue) const;
	BOOL UpdateTrackableTaskAttribute(KANBANITEM* pKI, TDC_ATTRIBUTE nAttrib, const CString& sNewValue);
	BOOL UpdateTrackableTaskAttribute(KANBANITEM* pKI, TDC_ATTRIBUTE nAttrib, int nNewValue);
	BOOL UpdateTrackableTaskAttribute(KANBANITEM* pKI, TDC_ATTRIBUTE nAttrib, const CStringArray& aNewValues);
	BOOL UpdateTrackableTaskAttribute(KANBANITEM* pKI, const CString& sAttribID, const CStringArray& aNewValues);
	void LoadDefaultAttributeListValues(const IPreferences* pPrefs, LPCTSTR szAttribID, LPCTSTR szSubKey);
	BOOL IsTrackedAttributeMultiValue() const;
	BOOL IsTracking(const CString& sAttribID) const;
	BOOL CanDrag(const CKanbanColumnCtrl* pSrcCol, const CKanbanColumnCtrl* pDestCol) const;
	BOOL UpdateNeedsItemHeightRefresh(const CSet<TDC_ATTRIBUTE>& attrib) const;

	BOOL UpdateData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CSet<TDC_ATTRIBUTE>& attrib, BOOL bAndSiblings);
	BOOL RebuildData(const ITASKLISTBASE* pTasks, const CSet<TDC_ATTRIBUTE>& attrib);
	BOOL AddTaskToData(const ITASKLISTBASE* pTasks, HTASKITEM hTask, DWORD dwParentID, const CSet<TDC_ATTRIBUTE>& attrib, BOOL bAndSiblings);
	BOOL UpdateGlobalAttributeValues(const ITASKLISTBASE* pTasks, const CSet<TDC_ATTRIBUTE>& attrib);
	BOOL UpdateGlobalAttributeValues(const ITASKLISTBASE* pTasks, TDC_ATTRIBUTE nAttribute);
	BOOL UpdateGlobalAttributeValues(LPCTSTR szAttribID, const CStringArray& aValues);

	BOOL WantShowColumn(LPCTSTR szAttribID, const CKanbanItemArrayMap& mapKIArray) const;
	BOOL WantShowColumn(const CKanbanColumnCtrl* pCol) const;

	static int GetTaskAllocTo(const ITASKLISTBASE* pTasks, HTASKITEM hTask, CStringArray& aValues);
	static int GetTaskCategories(const ITASKLISTBASE* pTasks, HTASKITEM hTask, CStringArray& aValues);
	static int GetTaskTags(const ITASKLISTBASE* pTasks, HTASKITEM hTask, CStringArray& aValues);
	static BOOL RebuildColumnContents(CKanbanColumnCtrl* pCol, const CKanbanItemArrayMap& mapKIArray, BOOL bShowParents);
	static CString GetXMLTag(TDC_ATTRIBUTE nAttrib);
	static BOOL HasNonParentTasks(const CKanbanItemArray* pItems);
	static void UpdateItemDisplayAttributes(KANBANITEM* pKI, const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CSet<TDC_ATTRIBUTE>& attrib);
	static void BuildTaskIDMap(const ITASKLISTBASE* pTasks, HTASKITEM hTask, CDWordSet& mapIDs, BOOL bAndSiblings);
};

#endif // !defined(AFX_KANBANCTRL_H__016B94F3_1D28_4532_97EF_95F1D9D5CE55__INCLUDED_)
