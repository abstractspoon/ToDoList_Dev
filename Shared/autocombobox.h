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
};

/////////////////////////////////////////////////////////////////////////////
// CAutoComboBox window

// replacement DDX routine
void AFXAPI DDX_AutoCBString(CDataExchange* pDX, int nIDC, CString& value);

class CAutoComboBox : public COwnerdrawComboBoxBase, private CSubclasser
{
	DECLARE_DYNAMIC(CAutoComboBox)

		// Construction
public:
	CAutoComboBox(DWORD dwFlags = 0);
	virtual ~CAutoComboBox();
	
	// Operations
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

    int FindStringExact(int nIndexStart, const CString& sItem, BOOL bCaseSensitive) const;
    int FindStringExact(int nIndexStart, LPCTSTR lpszFind) const;

	operator HWND() const { return GetSafeHwnd(); }

	// resolve ambiguities between CWnd and CSubclassWnd
	inline CWnd* GetParent() const { return COwnerdrawComboBoxBase::GetParent(); }
	inline DWORD GetExStyle() const { return COwnerdrawComboBoxBase::GetExStyle(); }
	inline DWORD GetStyle() const { return COwnerdrawComboBoxBase::GetStyle(); }

	inline void GetClientRect(LPRECT pRect) const { COwnerdrawComboBoxBase::GetClientRect(pRect); }
	inline void GetWindowRect(LPRECT pRect) const { COwnerdrawComboBoxBase::GetWindowRect(pRect); }
	inline void Invalidate(BOOL bErase = TRUE) { COwnerdrawComboBoxBase::Invalidate(bErase); }
	inline BOOL IsWindowEnabled() const { return COwnerdrawComboBoxBase::IsWindowEnabled(); }
	inline BOOL IsWindowVisible() const { return COwnerdrawComboBoxBase::IsWindowVisible(); }
	inline UINT GetDlgCtrlID() const { return COwnerdrawComboBoxBase::GetDlgCtrlID(); }
	inline void SetRedraw(BOOL bRedraw = TRUE) { COwnerdrawComboBoxBase::SetRedraw(bRedraw); }

protected:
	DWORD m_dwFlags;
	BOOL m_bEditChange;
	BOOL m_bClosingUp;
	CMaskEdit m_eMask;
	CFont m_fontClose;

	CSubclassWnd m_scEdit;
	CSubclassWnd m_scList;

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
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	afx_msg BOOL OnSelEndCancel();
	afx_msg BOOL OnSelEndOK();
	afx_msg BOOL OnSelChange();
	afx_msg BOOL OnDropDown();
	afx_msg BOOL OnCloseUp();
	afx_msg BOOL OnEditChange();
	DECLARE_MESSAGE_MAP()

	// pseudo handlers
	virtual LRESULT OnListboxMessage(UINT msg, WPARAM wp, LPARAM lp);
	virtual LRESULT OnEditboxMessage(UINT msg, WPARAM wp, LPARAM lp);
		
protected:
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								DWORD dwItemData, const CString& sItem, BOOL bList);
	virtual int GetExtraListboxWidth() const;
	virtual DWORD GetNewItemData() const { return 0; }

	BOOL GetListDeleteButtonRect(const CRect& rItem, CRect& rBtn) const;
	BOOL DoDeleteListItem(const CPoint& ptList);
	int HitTestListDeleteBtn(const CPoint& ptList) const;
	int HitTestList(const CPoint& ptList) const;

	BOOL AllowDelete() const;
	BOOL CaseSensitive() const { return Misc::HasFlag(m_dwFlags, ACBS_CASESENSITIVE); }
	BOOL AddToStart() const { return Misc::HasFlag(m_dwFlags, ACBS_ADDTOSTART); }
	int AddUniqueItem(const CString& sItem, BOOL bAddToStart);

	inline HWND GetEdit() const { return m_scEdit.GetHwnd(); }
	inline HWND GetListbox() const { return m_scList.GetHwnd(); }

	void ParentCBNotify(UINT nIDNotify);
	void ParentACNotify(UINT nMsgNotify, int nIndex, LPCTSTR szItem);

	virtual void HandleReturnKey();
	virtual CString GetSelectedItem() const;
	virtual BOOL DeleteLBItem(int nItem);

	BOOL FixupListBoxPosition(const WINDOWPOS& wpos);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOCOMBOBOX_H__34DD95FF_CCAE_4D8E_9162_D860B65CD448__INCLUDED_)
