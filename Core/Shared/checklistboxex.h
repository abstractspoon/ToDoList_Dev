#if !defined(AFX_CHECKLISTBOXEX_H__3862911F_2AC1_41DC_822D_CA68941B6FDC__INCLUDED_)
#define AFX_CHECKLISTBOXEX_H__3862911F_2AC1_41DC_822D_CA68941B6FDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// checklistboxex.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCheckListBoxEx window

class CCheckListBoxEx : public CCheckListBox
{
// Construction
public:
	CCheckListBoxEx();

	void SetAllChecked(BOOL bChecked = TRUE);

	int GetNumChecked() const;
	int GetCheckedItems(CStringArray& aChecked) const;

	BOOL GetCheckByData(DWORD dwItemData) const;
	int SetCheckByData(DWORD dwItemData, BOOL bChecked = TRUE);

// Attributes
protected:
	CImageList m_ilCheck;
	int m_nCheckHeight;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckListBoxEx)
	public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	//}}AFX_VIRTUAL
	virtual void PreSubclassWindow();

// Implementation
public:
	virtual ~CCheckListBoxEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCheckListBoxEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnDestroy();
	afx_msg LRESULT OnSetFont(WPARAM , LPARAM);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnable(BOOL bEnable);
	DECLARE_MESSAGE_MAP()

	virtual void PreDrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreMeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	BOOL DrawCheckbox(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void GetItemColors(LPDRAWITEMSTRUCT lpDrawItemStruct, COLORREF& crText, COLORREF& crBackgnd);
	BOOL GetItemCheckRect(int nItem, CRect& rCheck) const;
	void GetItemCheckRect(const CRect& rItem, CRect& rCheck) const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKLISTBOXEX_H__3862911F_2AC1_41DC_822D_CA68941B6FDC__INCLUDED_)
