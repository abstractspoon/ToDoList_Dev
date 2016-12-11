#if !defined(AFX_CHECKCOMBOBOX_H__4DDD3C3D_BDB7_4F07_9C50_75495F6E7F66__INCLUDED_)
#define AFX_CHECKCOMBOBOX_H__4DDD3C3D_BDB7_4F07_9C50_75495F6E7F66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// checkcombobox.h : header file
//

#include "autocombobox.h"
#include "subclass.h"

/////////////////////////////////////////////////////////////////////////////
// CCheckComboBox window

struct CCB_CHECK_DATA;

class CCheckComboBox : public CAutoComboBox
{
// Construction
public:
	CCheckComboBox(DWORD dwFlags = 0);

	BOOL GetCheck(int nIndex) const;
	int SetCheck(int nIndex, BOOL bCheck = TRUE);
	BOOL GetCheckByData(DWORD dwItemData) const;
	int SetCheckByData(DWORD dwItemData, BOOL bCheck = TRUE);
	void CheckAll(BOOL bCheck = TRUE);
	int GetCheckedCount() const;

	virtual int GetChecked(CStringArray& aItems) const;
	virtual BOOL SetChecked(const CStringArray& aItems);

	BOOL IsAnyChecked() const;
	BOOL IsAnyUnchecked() const;

	CString FormatCheckedItems(LPCTSTR szSep = NULL) const;
	CString GetTooltip() const;

	virtual int AddUniqueItem(const CString& sItem); // returns index or CB_ERR
    virtual int SelectString(int nStartAfter, LPCTSTR lpszString);

protected:
	CString m_sText;
	BOOL m_bTextFits;
	BOOL m_bChecking;
	int m_nCheckItem;

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
								DWORD dwItemData, const CString& sItem, BOOL bList);	
	virtual BOOL DeleteLBItem(int nItem);
	virtual int GetExtraListboxWidth() const;
	virtual void HandleReturnKey();
	virtual CString GetSelectedItem() const;

protected:
	void RecalcText(BOOL bUpdate = TRUE, BOOL bNotify = TRUE);
	BOOL ParseText(BOOL bAutoAdd = TRUE);

	static int CalcCheckBoxWidth(HDC hdc = NULL, HWND hwndRef = NULL);
	int SetCheck(int nIndex, BOOL bCheck, BOOL bUpdate);
	BOOL DrawCheckBox(CDC& dc, const CRect& rect, int nItem, DWORD dwItemData) const;
	void CheckAll(BOOL bCheck, BOOL bUpdate);

private:
	CCB_CHECK_DATA* GetAddItemCheckData(int nItem);
	CCB_CHECK_DATA* GetItemCheckData(int nItem) const;
	void DeleteAllCheckData();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKCOMBOBOX_H__4DDD3C3D_BDB7_4F07_9C50_75495F6E7F66__INCLUDED_)
