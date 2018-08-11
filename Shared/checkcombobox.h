#if !defined(AFX_CHECKCOMBOBOX_H__4DDD3C3D_BDB7_4F07_9C50_75495F6E7F66__INCLUDED_)
#define AFX_CHECKCOMBOBOX_H__4DDD3C3D_BDB7_4F07_9C50_75495F6E7F66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// checkcombobox.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#include "autocombobox.h"
#include "subclass.h"

/////////////////////////////////////////////////////////////////////////////

struct CCB_CHECK_DATA;

/////////////////////////////////////////////////////////////////////////////

enum CCB_CHECKSTATE
{ 
	CCBC_UNCHECKED = 0, 
	CCBC_CHECKED, 
	CCBC_MIXED 
};

/////////////////////////////////////////////////////////////////////////////
// CCheckComboBox window

class CCheckComboBox : public CAutoComboBox
{
	DECLARE_DYNAMIC(CCheckComboBox);

// Construction
public:
	CCheckComboBox(DWORD dwFlags = 0);

	CCB_CHECKSTATE GetCheck(int nIndex) const;
	int SetCheck(int nIndex, CCB_CHECKSTATE nCheck = CCBC_CHECKED);
	CCB_CHECKSTATE GetCheckByData(DWORD dwItemData) const;
	int SetCheckByData(DWORD dwItemData, CCB_CHECKSTATE nCheck = CCBC_CHECKED);
	void CheckAll(CCB_CHECKSTATE nCheck = CCBC_CHECKED);
	int GetCheckedCount(CCB_CHECKSTATE nCheck = CCBC_CHECKED) const;
	BOOL IsAnyChecked(CCB_CHECKSTATE nCheck = CCBC_CHECKED) const;
	int GetCheckStates(CArray<CCB_CHECKSTATE, CCB_CHECKSTATE> aStates) const;

	virtual int GetChecked(CStringArray& aItems, CCB_CHECKSTATE nCheck = CCBC_CHECKED) const;
	virtual BOOL SetChecked(const CStringArray& aItems);
	virtual BOOL SetChecked(const CStringArray& aChecked, const CStringArray& aMixed);
	
	CString GetTooltip() const;
	virtual CString FormatCheckedItems(LPCTSTR szSep = NULL) const;

	virtual int AddUniqueItem(const CString& sItem); // returns index or CB_ERR
    virtual int SelectString(int nStartAfter, LPCTSTR lpszString);

protected:
	CString m_sText;
	BOOL m_bTextFits;
	BOOL m_bChecking;
	int m_nClickedItem;
	CStringArray m_aMixedItems;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckComboBox)
	//}}AFX_VIRTUAL


// Implementation
public:
	virtual ~CCheckComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCheckComboBox)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg void OnDestroy();
	afx_msg BOOL OnEditchange();
	afx_msg BOOL OnDropdown();
	afx_msg BOOL OnCloseUp();
	afx_msg void OnLBSelChange();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnCBGetItemData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCBSetItemData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCBDeleteString(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCBResetContent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetTextLen(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	// pseudo handlers
	virtual LRESULT OnListboxMessage(UINT msg, WPARAM wp, LPARAM lp);
	virtual LRESULT OnEditboxMessage(UINT msg, WPARAM wp, LPARAM lp);
	virtual void OnCheckChange(int /*nIndex*/) {}
	
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);	
	virtual BOOL DrawCheckBox(CDC& dc, const CRect& rect, int nItem, UINT nItemState, DWORD dwItemData, BOOL bDisabled) const;

	virtual BOOL DeleteLBItem(int nItem);
	virtual int GetExtraListboxWidth() const;
	virtual void HandleReturnKey();
	virtual CString GetSelectedItemText() const;
	virtual BOOL HasIcon() const { return TRUE; }

protected:
	void RecalcText(BOOL bUpdate = TRUE, BOOL bNotify = TRUE);
	BOOL ParseText(BOOL bAutoAdd = TRUE);

	static int CalcCheckBoxWidth(HDC hdc = NULL, HWND hwndRef = NULL);
	int SetCheck(int nIndex, CCB_CHECKSTATE nCheck, BOOL bUpdate);
	void CheckAll(CCB_CHECKSTATE nCheck, BOOL bUpdate);
	BOOL ToggleCheck(int nItem);
	BOOL ModifyChecked(const CStringArray& aItems, CCB_CHECKSTATE nCheck, BOOL bUpdate);

private:
	CCB_CHECK_DATA* GetAddItemCheckData(int nItem);
	CCB_CHECK_DATA* GetItemCheckData(int nItem) const;
	void DeleteAllCheckData();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKCOMBOBOX_H__4DDD3C3D_BDB7_4F07_9C50_75495F6E7F66__INCLUDED_)
