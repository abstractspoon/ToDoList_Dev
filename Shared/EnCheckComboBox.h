#if !defined(AFX_TDLCATEGORYCOMBOBOX_H__23DEAFA7_C661_477D_B583_FDB36C11FDC3__INCLUDED_)
#define AFX_TDLCATEGORYCOMBOBOX_H__23DEAFA7_C661_477D_B583_FDB36C11FDC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLCategoryComboBox.h : header file
//

#include "checkcombobox.h"
#include "enstring.h"

/////////////////////////////////////////////////////////////////////////////
// CEnCheckComboBox window

class CEnCheckComboBox : public CCheckComboBox
{
	DECLARE_DYNAMIC(CEnCheckComboBox);

	// Construction
public:
	CEnCheckComboBox(BOOL bMulti = TRUE, UINT nIDNoneString = 0, UINT nIDAnyString = 0);
	
	BOOL EnableMultiSelection(BOOL bEnable = TRUE);
	int SetStrings(const CStringArray& aItems);

	BOOL GetCheck(int nIndex) const;
	int GetChecked(CStringArray& aItems, CCB_CHECKSTATE nCheck = CCBC_CHECKED) const;
	BOOL SetChecked(const CStringArray& aChecked);
	BOOL SetChecked(const CStringArray& aChecked, const CStringArray& aMixed);
	int SetCheck(int nIndex, CCB_CHECKSTATE nCheck = CCBC_CHECKED); 

	CString FormatCheckedItems(LPCTSTR szSep = NULL) const;

protected:
	BOOL m_bMultiSel;
	CEnString m_sNone, m_sAny;
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnCheckComboBox)
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CEnCheckComboBox(); 
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CEnCheckComboBox)
	//}}AFX_MSG
	afx_msg void OnLBSelChange();
	afx_msg BOOL OnSelEndOK();
	
	DECLARE_MESSAGE_MAP()
	
	// pseudo handlers
	virtual LRESULT OnListboxMessage(UINT msg, WPARAM wp, LPARAM lp);
	virtual LRESULT OnEditboxMessage(UINT msg, WPARAM wp, LPARAM lp);

	virtual void OnCheckChange(int nIndex);
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);	
	virtual BOOL DrawCheckBox(CDC& dc, const CRect& rect, int nItem, UINT nItemState, DWORD dwItemData, BOOL bDisabled) const;

	void FixupEmptyStringsAtStart();
	int CalcNumRequiredEmptyStrings() const;
	BOOL AddEmptyString() { return CCheckComboBox::AddEmptyString(); }
	void GetChecked(CStringArray& aChecked, CStringArray& aMixed) const;
	CString GetItemText(int nItem, const CString& sHint = _T("")) const;

	int GetNoneIndex() const;
	BOOL IsNoneIndex(int nItem) const;
	BOOL IsNoneChecked() const;

	int GetAnyIndex() const;
	BOOL IsAnyIndex(int nItem) const;
	BOOL IsAnyChecked() const;

#ifdef _DEBUG
	void TraceCheckStates() const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLCATEGORYCOMBOBOX_H__23DEAFA7_C661_477D_B583_FDB36C11FDC3__INCLUDED_)
