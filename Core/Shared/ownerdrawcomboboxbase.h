#if !defined(AFX_OWNERDRAWCOMBOBOXBASE_H__9FFD5D25_60B4_49B8_A05B_D61AEBC8D936__INCLUDED_)
#define AFX_OWNERDRAWCOMBOBOXBASE_H__9FFD5D25_60B4_49B8_A05B_D61AEBC8D936__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ownerdrawcomboboxbase.h : header file
//

/////////////////////////////////////////////////////////////////////////////

struct ODCB_ITEMDATA
{
	ODCB_ITEMDATA() : dwItemData(0), bHeading(FALSE), bDisabled(FALSE) {}
	virtual ~ODCB_ITEMDATA() {}
	
	DWORD dwItemData;
	BOOL bHeading;
	BOOL bDisabled;
};

/////////////////////////////////////////////////////////////////////////////
// COwnerdrawComboBoxBase window

class COwnerdrawComboBoxBase : public CComboBox
{
	DECLARE_DYNAMIC(COwnerdrawComboBoxBase)

// Construction
public:
	COwnerdrawComboBoxBase(int nDefMinVisible = 30);
	virtual ~COwnerdrawComboBoxBase();

	void RefreshDropWidth();
	int SetCurSel(int nSel, BOOL bValidate = TRUE);

    int FindStringExact(int nIndexStart, const CString& sItem, BOOL bCaseSensitive) const;
    int FindStringExact(int nIndexStart, LPCTSTR lpszFind) const;

	BOOL SelectNextItem(const CString& sText, BOOL bForward);
	int FindNextItem(const CString& sText, int nFrom, BOOL bForward) const;

	int SetDisabledItem(int nItem, BOOL bDisabled = TRUE);
	int SetHeadingItem(int nItem, BOOL bHeading = TRUE);
	
	BOOL IsHeadingItem(int nItem) const;
	BOOL IsDisabledItem(int nItem) const;

// Attributes
protected:
	int m_nMaxTextWidth;
	int m_nDefMinVisible;
	int m_nNumHeadings;
	BOOL m_bHasExtItemData;

	static const int IMAGESIZE;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COwnerdrawComboBoxBase)
protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct); 

	// Generated message map functions
protected:
	//{{AFX_MSG(COwnerdrawComboBoxBase)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSetFont(WPARAM , LPARAM);
	afx_msg BOOL OnSelEndOK();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();

	// These are for extending the item data
	afx_msg LRESULT OnCBGetItemData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCBSetItemData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCBDeleteString(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCBResetContent(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

protected:
	virtual void GetItemColors(int nItem, UINT nItemState, DWORD dwItemData, 
								COLORREF& crText, COLORREF& crBack) const;	
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);	
	virtual BOOL HasIcon() const { return FALSE; }
	virtual UINT GetDrawEllipsis() const { return DT_END_ELLIPSIS; }
	virtual int GetMaxDropWidth() const { return -1; } // no limit
	virtual int GetExtraListboxWidth() const;
	virtual int CalcMinItemHeight(BOOL bList) const;
	virtual BOOL IsSelectableItem(int nItem) const;

	void InitItemHeight();
	BOOL IsType(UINT nComboType) const;
	void RefreshDropWidth(BOOL bRecalc);
	int GetMinVisible() const;

	BOOL WantDrawFocusRect(LPDRAWITEMSTRUCT lpDrawItemStruct) const;
	BOOL ValidateSelection(int& nSel, BOOL bForward) const;
	BOOL IsValidIndex(int nItem) const { return ((nItem >= 0) && (nItem < GetCount())); }
	BOOL FindNextItem(const CString& sText, int nFrom, int nTo, int nIncrement, int& nNext) const;

protected:
	void DeleteAllExtItemData();

	virtual ODCB_ITEMDATA* NewExtItemData() const { return new ODCB_ITEMDATA(); }

	ODCB_ITEMDATA* GetAddExtItemData(int nItem);
	ODCB_ITEMDATA* GetExtItemData(int nItem) const;

private:
	LRESULT GetRawItemData(int nItem) const;
	LRESULT SetRawItemData(int nItem, ODCB_ITEMDATA*& pItemData);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OWNERDRAWCOMBOBOXBASE_H__9FFD5D25_60B4_49B8_A05B_D61AEBC8D936__INCLUDED_)
