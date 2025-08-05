#if !defined(AFX_TDLPRIORITYCOMBOBOX_H__94985E3D_C2CA_44C0_B3AD_E55C110AEE45__INCLUDED_)
#define AFX_TDLPRIORITYCOMBOBOX_H__94985E3D_C2CA_44C0_B3AD_E55C110AEE45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLPriorityComboBox.h : header file
//

#include "..\shared\colorcombobox.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLPriorityComboBox window

class CTDLPriorityComboBox : public CColorComboBox
{
// Construction
public:
	CTDLPriorityComboBox(BOOL bIncludeAny, BOOL bIncludeNone = TRUE);
	virtual ~CTDLPriorityComboBox();

	BOOL SetColors(const CDWordArray& aColors); // 2 - 11
	void SetNumLevels(int nLevels); // 2 - 11
	int GetNumLevels() const { return m_nNumLevels; }

	int IncrementPriority(int nAmount);
	int GetSelectedPriority() const; // -2 -> Num Levels - 1
	void SetSelectedPriority(int nPriority); // -2 -> Num Levels - 1

	void DDX(CDataExchange* pDX, int& nPriority);

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLPriorityComboBox)
// 	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	CDWordArray m_aColors;
	BOOL m_bIncludeAny, m_bIncludeNone;
	int m_nNumLevels;

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLPriorityComboBox)
// 	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	virtual void OnPopulate();
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState, 
							 DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLPRIORITYCOMBOBOX_H__94985E3D_C2CA_44C0_B3AD_E55C110AEE45__INCLUDED_)
