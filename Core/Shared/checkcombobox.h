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
#include "tooltipctrlex.h"

/////////////////////////////////////////////////////////////////////////////

enum CCB_CHECKSTATE
{ 
	CCBC_UNCHECKED = 0, 
	CCBC_CHECKED, 
	CCBC_MIXED 
};

/////////////////////////////////////////////////////////////////////////////

class CCheckComboBox;

void DDX_CheckItemData(CDataExchange* pDX, CCheckComboBox& combo, DWORD& dwItems);

/////////////////////////////////////////////////////////////////////////////
// CCheckComboBox window

class CCheckComboBox : public CAutoComboBox
{
	DECLARE_DYNAMIC(CCheckComboBox);

// Construction
public:
	CCheckComboBox(DWORD dwFlags = 0);
	virtual ~CCheckComboBox();

	CCB_CHECKSTATE GetCheck(int nIndex) const;
	int SetCheck(int nIndex, CCB_CHECKSTATE nCheck = CCBC_CHECKED);
	CCB_CHECKSTATE GetCheckByData(DWORD dwItemData) const;
	void CheckAll(CCB_CHECKSTATE nCheck = CCBC_CHECKED);
	int GetCheckedCount(CCB_CHECKSTATE nCheck = CCBC_CHECKED) const;
	BOOL IsAnyChecked(CCB_CHECKSTATE nCheck = CCBC_CHECKED) const;
	int GetCheckStates(CArray<CCB_CHECKSTATE, CCB_CHECKSTATE>& aStates) const;

	int SetCheckByItemData(DWORD dwItemData, CCB_CHECKSTATE nCheck = CCBC_CHECKED);
	int SetCheckedByItemData(DWORD dwFlags);
	DWORD GetCheckedItemData() const;

	virtual int GetChecked(CStringArray& aItems, CCB_CHECKSTATE nCheck = CCBC_CHECKED) const;
	virtual BOOL SetChecked(const CStringArray& aItems);
	virtual BOOL SetChecked(const CStringArray& aChecked, const CStringArray& aMixed);
	
	void EnableTooltip(BOOL bEnable = TRUE);
	CString GetTooltip() const;
	void FilterToolTipMessage(MSG* pMsg);

	virtual int AddUniqueItem(const CString& sItem); // returns index or CB_ERR
    virtual int SelectString(int nStartAfter, LPCTSTR lpszString);

protected:
	CString m_sText;
	BOOL m_bTextFits;
	BOOL m_bChecking;
	int m_nClickedItem;
	CStringArray m_aMixedItems;
	CToolTipCtrlEx m_tooltip;

	const static int CHECKBOX_SIZE;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckComboBox)
	//}}AFX_VIRTUAL

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
	afx_msg LRESULT OnGetTextLen(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	// pseudo handlers
	virtual LRESULT OnListboxMessage(UINT msg, WPARAM wp, LPARAM lp);
	virtual LRESULT OnEditboxMessage(UINT msg, WPARAM wp, LPARAM lp);
	virtual void OnCheckChange(int /*nIndex*/) {}
	
	virtual CString FormatCheckedItems(LPCTSTR szSep = NULL) const;
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);	
	virtual void DrawCheckBox(CDC& dc, const CRect& rect, int nItem, UINT nItemState, DWORD dwItemData, BOOL bDisabled) const;

	virtual BOOL DeleteLBItem(int nItem);
	virtual int GetExtraListboxWidth() const;
	virtual int CalcMinItemHeight(BOOL bList) const;
	virtual void HandleReturnKey();
	virtual CString GetSelectedItemText() const;
	virtual int UpdateEditAutoComplete(const CString& sText, int nCaretPos);

	BOOL PreTranslateMessage(MSG* pMsg);
	int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

protected:
	void RecalcText(BOOL bUpdate = TRUE, BOOL bNotify = TRUE);
	BOOL ParseText(BOOL bAutoAdd = TRUE);

	int SetCheck(int nIndex, CCB_CHECKSTATE nCheck, BOOL bUpdate);
	void CheckAll(CCB_CHECKSTATE nCheck, BOOL bUpdate);
	BOOL ToggleCheck(int nItem);
	BOOL ModifyChecked(const CStringArray& aItems, CCB_CHECKSTATE nCheck, BOOL bUpdate);

protected:
	struct CCB_ITEMDATA : public ODCB_ITEMDATA
	{
		CCB_ITEMDATA() : ODCB_ITEMDATA(), nCheck(CCBC_UNCHECKED) {}

		CCB_CHECKSTATE nCheck;
	};

	virtual ODCB_ITEMDATA* NewExtItemData() const { return new CCB_ITEMDATA; }
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKCOMBOBOX_H__4DDD3C3D_BDB7_4F07_9C50_75495F6E7F66__INCLUDED_)
