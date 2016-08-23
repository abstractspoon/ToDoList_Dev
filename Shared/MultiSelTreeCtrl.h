#if !defined(AFX_MULTISELECTIONTREECTRL_H__663332A2_9568_11D3_B705_0000E842E6CE__INCLUDED_)
#define AFX_MULTISELECTIONTREECTRL_H__663332A2_9568_11D3_B705_0000E842E6CE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MultiSelectionTreeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMultiSelTreeCtrl window
// Code based on Zafir Anjum free code

class CMultiSelTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CMultiSelTreeCtrl)
		// Construction
public:
	CMultiSelTreeCtrl();
	
	// Attributes
public:
	
	// Operations
public:
	virtual void EnableMultSelection(BOOL bAllow = TRUE);
	BOOL IsMultSelectionEnabled() const			{ return m_bAllowMultSel; }
	
	virtual void ClearSelection();
	
	virtual BOOL SelectItems(HTREEITEM hItemFrom, HTREEITEM hItemTo);
	void SelectItem(HTREEITEM htItem) {	CTreeCtrl::SelectItem(htItem); }
	void SelectItem(HTREEITEM htItem, BOOL bSelect);
	BOOL IsItemSelected(HTREEITEM htItem);
	int	GetSelectedCount();
	
	HTREEITEM GetPrevSelectedItem(HTREEITEM hItem);
	HTREEITEM GetNextSelectedItem(HTREEITEM hItem);
	HTREEITEM GetFirstSelectedItem();
	
	HTREEITEM GetNextItem(HTREEITEM hItem, UINT nCode) const { return CTreeCtrl::GetNextItem(hItem, nCode); }
	HTREEITEM GetNextItem(HTREEITEM hItem);
	HTREEITEM GetPrevItem(HTREEITEM hItem);
	
	void SelectAllVisibleItems();
	
	HTREEITEM GetFirstSiblingItem(HTREEITEM hItem);
	HTREEITEM GetLastSiblingItem(HTREEITEM hItem);
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiSelTreeCtrl)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CMultiSelTreeCtrl();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CMultiSelTreeCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	
	BOOL m_bAllowMultSel;
	HTREEITEM m_hItemFirstSel;		// Init to NULL in constructor
	
	DECLARE_MESSAGE_MAP()
		
protected:
	void NotifySelChange();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTISELECTIONTREECTRL_H__663332A2_9568_11D3_B705_0000E842E6CE__INCLUDED_)
