#if !defined(AFX_AUTOCOMBOBOX_H__34DD95FF_CCAE_4D8E_9162_D860B65CD448__INCLUDED_)
#define AFX_AUTOCOMBOBOX_H__34DD95FF_CCAE_4D8E_9162_D860B65CD448__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// autocombobox.h : header file
//

#include "Subclass.h"
#include "maskedit.h"
#include "misc.h"
#include "ownerdrawcomboboxbase.h"

/////////////////////////////////////////////////////////////////////////////

const UINT WM_ACBN_ITEMADDED = ::RegisterWindowMessage(_T("WM_ACBN_ITEMADDED"));
const UINT WM_ACBN_ITEMDELETED = ::RegisterWindowMessage(_T("WM_ACBN_ITEMDELETED"));


/////////////////////////////////////////////////////////////////////////////

enum 
{
	ACBS_ALLOWDELETE	= 0x01,
	ACBS_CASESENSITIVE	= 0x02,
	ACBS_ADDTOSTART		= 0x04,
	ACBS_AUTOCOMPLETE	= 0x08,
};

/////////////////////////////////////////////////////////////////////////////
// CAutoComboBox window

class CAutoComboBox : public COwnerdrawComboBoxBase, private CSubclasser
{
	DECLARE_DYNAMIC(CAutoComboBox)

public:
	CAutoComboBox(DWORD dwFlags = 0);
	virtual ~CAutoComboBox();
	
public:
    virtual int AddString(LPCTSTR szItem) { return AddUniqueItem(szItem); }
    virtual int InsertString(int nIndex, LPCTSTR szItem) { return InsertUniqueItem(nIndex, szItem); }

    virtual int AddStrings(const CStringArray& aItems) { return AddUniqueItems(aItems); }
    virtual int SetStrings(const CStringArray& aItems);

	virtual int DeleteString(UINT nIndex) { return COwnerdrawComboBoxBase::DeleteString(nIndex);	}
	virtual int DeleteString(LPCTSTR szItem, BOOL bCaseSensitive = FALSE);

	virtual int DeleteItems(const CStringArray& aItems, BOOL bCaseSensitive = FALSE); // returns deleted count
	
	virtual int AddUniqueItem(const CString& sItem); // returns index or CB_ERR
    virtual int InsertUniqueItem(int nIndex, const CString& sItem); // returns index or CB_ERR
	
    virtual int AddUniqueItems(const CStringArray& aItems); // returns num items added
    virtual int AddUniqueItems(const CAutoComboBox& cbItems); // returns num items added
	
    virtual int GetItems(CStringArray& aItems) const; // returns item count
    virtual int SelectString(int nStartAfter, LPCTSTR lpszString) { return COwnerdrawComboBoxBase::SelectString(nStartAfter, lpszString); }
    
	BOOL AddEmptyString();
	void SetEditMask(LPCTSTR szMask, DWORD dwMaskFlags = 0);
	void Flush();
	int GetCurSel() const;
	void ModifyFlags(DWORD dwRemove, DWORD dwAdd);
	CString GetItemText(int nItem) const;

	void DDX(CDataExchange* pDX, CString& value);

protected:
	DWORD m_dwFlags;
	CMaskEdit m_eMask;
	CFont m_fontClose;

	CSubclassWnd m_scEdit;
	CSubclassWnd m_scList;

	BOOL m_bEditChange;
	BOOL m_bNotifyingParent;
	BOOL m_bSkipAutoComplete;

	int m_nHotSimpleListItem;

	// It seems that both the edit and listbox cannot
	// have visible selection at the same time so we
	// need to manage it ourselves
	int m_nAutoCompleteMatch;

	mutable BOOL m_bDrawing;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoComboBox)
	//}}AFX_VIRTUAL
	
private:
	virtual LRESULT ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);

	// for deletion
	int m_nDeleteItem;
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CAutoComboBox)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor); // for subclassing
	//}}AFX_MSG
	afx_msg BOOL OnSelEndCancel();
	afx_msg BOOL OnSelEndOK();
	afx_msg BOOL OnSelChange();
	afx_msg BOOL OnDropDown();
	afx_msg BOOL OnCloseUp();
	afx_msg BOOL OnEditChange();
	afx_msg BOOL OnEditUpdate();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()

	// pseudo handlers
	virtual LRESULT OnListboxMessage(UINT msg, WPARAM wp, LPARAM lp);
	virtual LRESULT OnEditboxMessage(UINT msg, WPARAM wp, LPARAM lp);
		
protected:
	virtual void GetItemColors(int nItem, UINT nItemState, DWORD dwItemData,
							   COLORREF& crText, COLORREF& crBack) const;
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);
	virtual int GetExtraListboxWidth() const;
	virtual DWORD GetNewItemData() const { return 0; }

	BOOL GetListDeleteButtonRect(const CRect& rItem, CRect& rBtn) const;
	BOOL DoDeleteListItem(const CPoint& ptList);
	int HitTestListDeleteBtn(const CPoint& ptList) const;
	int HitTestList(const CPoint& ptList) const;
	void RedrawListItem(int nItem) const;
	BOOL IsAutoCompleteMatch(int nItem) const;

	BOOL AllowDelete() const;
	BOOL CaseSensitive() const { return Misc::HasFlag(m_dwFlags, ACBS_CASESENSITIVE); }
	BOOL AddToStart() const { return Misc::HasFlag(m_dwFlags, ACBS_ADDTOSTART); }
	int AddUniqueItem(const CString& sItem, BOOL bAddToStart);
	CString GetEditText() const;

	inline HWND GetEdit() const { return m_scEdit.GetHwnd(); }
	inline HWND GetListbox() const { return m_scList.GetHwnd(); }

	void ParentCBNotify(UINT nIDNotify);
	void ParentACNotify(UINT nMsgNotify, int nIndex, LPCTSTR szItem);

	virtual void HandleReturnKey();
	virtual CString GetSelectedItemText() const;
	virtual BOOL DeleteLBItem(int nItem);
	virtual int UpdateEditAutoComplete(const CString& sText, int nCaretPos);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOCOMBOBOX_H__34DD95FF_CCAE_4D8E_9162_D860B65CD448__INCLUDED_)
