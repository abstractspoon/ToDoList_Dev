#if !defined(AFX_OWNERDRAWCOMBOBOXBASE_H__9FFD5D25_60B4_49B8_A05B_D61AEBC8D936__INCLUDED_)
#define AFX_OWNERDRAWCOMBOBOXBASE_H__9FFD5D25_60B4_49B8_A05B_D61AEBC8D936__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ownerdrawcomboboxbase.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COwnerdrawComboBoxBase window

class COwnerdrawComboBoxBase : public CComboBox
{
	DECLARE_DYNAMIC(COwnerdrawComboBoxBase)

// Construction
public:
	COwnerdrawComboBoxBase(int nMinDLUHeight = 9);

	BOOL SetMinDLUHeight(int nMinDLUHeight);
	int GetMinDluHeight() const { return m_nMinDLUHeight; }
	void RefreshDropWidth();

    int FindStringExact(int nIndexStart, const CString& sItem, BOOL bCaseSensitive) const;
    int FindStringExact(int nIndexStart, LPCTSTR lpszFind) const;

// Attributes
protected:
	BOOL m_bItemHeightSet;

private:
	int m_nMinDLUHeight;
	int m_nMaxTextWidth;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COwnerdrawComboBoxBase)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct); 

// Implementation
public:
	virtual ~COwnerdrawComboBoxBase();

	// Generated message map functions
protected:
	//{{AFX_MSG(COwnerdrawComboBoxBase)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnSetFont(WPARAM , LPARAM);

	DECLARE_MESSAGE_MAP()

protected:
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								DWORD dwItemData, const CString& sItem, BOOL bList);	
	virtual BOOL HasIcon() const { return FALSE; }
	virtual UINT GetDrawEllipsis() const { return DT_END_ELLIPSIS; }
	virtual int GetMaxDropWidth() const { return -1; } // no limit
	virtual int GetExtraListboxWidth() const;

	void InitItemHeight();
	int CalcMinItemHeight(BOOL bList) const;
	BOOL IsType(UINT nComboType) const;
	void RefreshDropWidth(BOOL bRecalc);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OWNERDRAWCOMBOBOXBASE_H__9FFD5D25_60B4_49B8_A05B_D61AEBC8D936__INCLUDED_)
