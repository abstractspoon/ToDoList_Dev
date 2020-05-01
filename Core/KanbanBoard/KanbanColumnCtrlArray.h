#if !defined(AFX_KANBANCOLUMNCTRLARRAY_H__059495EC_3D8D_4607_A4CF_20C142F8A294__INCLUDED_)
#define AFX_KANBANCOLUMNCTRLARRAY_H__059495EC_3D8D_4607_A4CF_20C142F8A294__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KanbanListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////

class CKanbanColumnCtrl;

/////////////////////////////////////////////////////////////////////////////

class CKanbanColumnCtrlArray : public CArray<CKanbanColumnCtrl*, CKanbanColumnCtrl*&>
{
public:
	virtual ~CKanbanColumnCtrlArray();
	
	void RemoveAll();
	BOOL RemoveAt(int nList);

	int Find(DWORD dwTaskID) const;
	int Find(DWORD dwTaskID, HTREEITEM& hti) const;
	int Find(HWND hWnd) const;
	int Find(const CString& sAttribValue) const;
	int Find(const CKanbanColumnCtrl* pList) const;

	CKanbanColumnCtrl* Get(DWORD dwTaskID) const;
	CKanbanColumnCtrl* Get(DWORD dwTaskID, HTREEITEM& hti) const;
	CKanbanColumnCtrl* Get(HWND hWnd) const;
	CKanbanColumnCtrl* Get(const CString& sAttribValue) const;
	CKanbanColumnCtrl* GetFirstNonEmpty() const;
	CKanbanColumnCtrl* GetLastNonEmpty() const;
	CKanbanColumnCtrl* GetBacklog() const;

	void SetOptions(DWORD dwOptions);
	int GetVisibleTaskCount() const;
	float GetAverageCharWidth();
	DWORD HitTestTask(const CPoint& ptScreen) const;
	void SetAttributeLabelVisibility(KBC_ATTRIBLABELS nLabelVis);
	
	CSize CalcRequiredColumnSizeForImage() const;
	BOOL CanSaveToImage() const;
	BOOL SaveToImage(CBitmap& bmImage);

	CKanbanColumnCtrl* GetNext(const CKanbanColumnCtrl* pList, BOOL bNext, BOOL bExcludeEmpty, BOOL bFixedColumns) const;
	CKanbanColumnCtrl* HitTest(const CPoint& ptScreen, HTREEITEM* pHit = NULL, UINT* pHitFlags = NULL) const;

	void OnDisplayAttributeChanged();

	void SortColumns();
	void SortItems(TDC_ATTRIBUTE nBy, BOOL bAscending);

	void SetSelectedColumn(const CKanbanColumnCtrl* pSelList);
	void SetDropTarget(const CKanbanColumnCtrl* pTarget);
	void DeleteTaskFromOthers(DWORD dwTaskID, const CKanbanColumnCtrl* pIgnore);

	void Exclude(CDC* pDC);
	void Redraw(BOOL bErase, BOOL bUpdate = FALSE);
	int RemoveDeletedTasks(const CDWordSet& mapCurIDs);
	void RefreshItemLineHeights();
	void SetFont(HFONT hFont);

protected:
	static int SortProc(const void* pV1, const void* pV2);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KANBANCOLUMNCTRLARRAY_H__059495EC_3D8D_4607_A4CF_20C142F8A294__INCLUDED_)
