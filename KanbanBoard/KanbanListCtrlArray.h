#if !defined(AFX_KANBANLISTCTRLARRAY_H__059495EC_3D8D_4607_A4CF_20C142F8A294__INCLUDED_)
#define AFX_KANBANLISTCTRLARRAY_H__059495EC_3D8D_4607_A4CF_20C142F8A294__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KanbanListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////

class CKanbanListCtrl;

/////////////////////////////////////////////////////////////////////////////

class CKanbanListCtrlArray : public CArray<CKanbanListCtrl*, CKanbanListCtrl*&>
{
public:
	virtual ~CKanbanListCtrlArray();
	
	void RemoveAll();
	BOOL RemoveAt(int nList);

	int Find(DWORD dwTaskID) const;
	int Find(DWORD dwTaskID, HTREEITEM& hti) const;
	int Find(HWND hWnd) const;
	int Find(const CString& sAttribValue) const;
	int Find(const CKanbanListCtrl* pList) const;

	CKanbanListCtrl* Get(DWORD dwTaskID) const;
	CKanbanListCtrl* Get(DWORD dwTaskID, HTREEITEM& hti) const;
	CKanbanListCtrl* Get(HWND hWnd) const;
	CKanbanListCtrl* Get(const CString& sAttribValue) const;
	CKanbanListCtrl* GetFirstNonEmpty() const;
	CKanbanListCtrl* GetLastNonEmpty() const;
	CKanbanListCtrl* GetBacklog() const;

	void SetTextColorIsBackground(BOOL bSet = TRUE);
	void SetShowTaskColorAsBar(BOOL bSet = TRUE);
	void SetStrikeThruDoneTasks(BOOL bSet = TRUE);
	void SetColorTaskBarByPriority(BOOL bSet = TRUE);
	void SetShowCompletionCheckboxes(BOOL bShow = TRUE);
	void SetIndentSubtasks(BOOL bIndent = TRUE);
	void SetHideEmptyAttributes(BOOL bHide = TRUE);
	
	int GetVisibleTaskCount() const;
	float GetAverageCharWidth();
	DWORD HitTestTask(const CPoint& ptScreen) const;
	void SetAttributeLabelVisibility(KBC_ATTRIBLABELS nLabelVis);
	
	CSize CalcRequiredColumnSizeForImage() const;
	BOOL CanSaveToImage() const;
	BOOL SaveToImage(CBitmap& bmImage);

	CKanbanListCtrl* GetNext(const CKanbanListCtrl* pList, BOOL bNext, BOOL bExcludeEmpty, BOOL bFixedColumns) const;
	CKanbanListCtrl* HitTest(const CPoint& ptScreen, HTREEITEM* pHit = NULL, UINT* pHitFlags = NULL) const;

	void OnDisplayAttributeChanged();
	void OnSetFont(HFONT hFont);

	void Sort();
	void SortItems(IUI_ATTRIBUTE nBy, BOOL bAscending, BOOL bSubtasksBelowParent);

	void SetSelectedList(const CKanbanListCtrl* pSelList);
	void SetDropTarget(const CKanbanListCtrl* pTarget);
	void DeleteTaskFromOthers(DWORD dwTaskID, const CKanbanListCtrl* pIgnore);

	void Exclude(CDC* pDC);
	void Redraw(BOOL bErase, BOOL bUpdate = FALSE);
	void RemoveDeletedTasks(const CDWordSet& mapCurIDs);
	void RefreshItemLineHeights();

protected:
	static int ListSortProc(const void* pV1, const void* pV2);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KANBANLISTCTRLARRAY_H__059495EC_3D8D_4607_A4CF_20C142F8A294__INCLUDED_)
