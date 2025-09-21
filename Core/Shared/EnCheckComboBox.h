#if !defined(AFX_ENCHECKCOMBOBOX_H__23DEAFA7_C661_477D_B583_FDB36C11FDC3__INCLUDED_)
#define AFX_ENCHECKCOMBOBOX_H__23DEAFA7_C661_477D_B583_FDB36C11FDC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnCheckComboBox.h : header file
//

#include "checkcombobox.h"

/////////////////////////////////////////////////////////////////////////////
// CEnCheckComboBox window

class CEnCheckComboBox : public CCheckComboBox
{
	DECLARE_DYNAMIC(CEnCheckComboBox);

public:
	CEnCheckComboBox(BOOL bMulti = TRUE, UINT nIDNoneString = 0, UINT nIDAnyString = 0);
	virtual ~CEnCheckComboBox();
	
	BOOL EnableMultiSelection(BOOL bEnable = TRUE);
	BOOL IsMultiSelectionEnabled() const { return m_bMultiSel; }
	void ClearMultiSelectionHistory();

	int SelectString(int nStartAfter, LPCTSTR lpszString);
	BOOL GetCheck(int nIndex) const;
	int GetChecked(CStringArray& aItems, CCB_CHECKSTATE nCheck = CCBC_CHECKED) const;
	BOOL SetChecked(const CStringArray& aChecked);
	int SetCheck(int nIndex, CCB_CHECKSTATE nCheck = CCBC_CHECKED); 

	virtual int SetStrings(const CStringArray& aItems);
	virtual int GetChecked(CStringArray& aChecked, CStringArray& aMixed) const;
	virtual BOOL SetChecked(const CStringArray& aChecked, const CStringArray& aMixed);

protected:
	BOOL m_bMultiSel;
	CString m_sNone, m_sAny;
	
protected:
	afx_msg void OnLBSelChange();
	afx_msg BOOL OnSelEndOK();
	afx_msg LRESULT OnGetTextLen(WPARAM wParam, LPARAM lParam);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()
	
	// pseudo handlers
	virtual LRESULT OnListboxMessage(UINT msg, WPARAM wp, LPARAM lp);
	virtual LRESULT OnEditboxMessage(UINT msg, WPARAM wp, LPARAM lp);

	virtual void OnCheckChange(int nIndex);
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);	
	virtual void DrawCheckBox(CDC& dc, const CRect& rect, int nItem, UINT nItemState, DWORD dwItemData, BOOL bDisabled) const;
	virtual CString FormatCheckedItems(LPCTSTR szSep = NULL) const;
	virtual int CalcMinItemHeight(BOOL bList) const;
	virtual int GetExtraListboxWidth() const;

	void FixupEmptyStringsAtStart();
	int CalcNumRequiredEmptyStrings() const;
	BOOL AddEmptyString() { return CCheckComboBox::AddEmptyString(); }
	CString GetItemText(int nItem, const CString& sHint = _T("")) const;
	
	BOOL HasItemNone() const { return (GetItemNoneIndex() != CB_ERR); }
	int GetItemNoneIndex() const;
	BOOL IsItemNoneIndex(int nItem) const;
	BOOL IsItemNoneChecked() const;

	BOOL HasItemAny() const { return (GetItemAnyIndex() != CB_ERR); }
	int GetItemAnyIndex() const;
	BOOL IsItemAnyIndex(int nItem) const;
	BOOL IsItemAnyChecked() const;

#ifdef _DEBUG
	void TraceCheckStates() const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENCHECKCOMBOBOX_H__23DEAFA7_C661_477D_B583_FDB36C11FDC3__INCLUDED_)
