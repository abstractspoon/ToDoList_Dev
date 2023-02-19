#if !defined(AFX_TDLREGULARITYCOMBOBOX_H__FEBCAD06_B7B7_46A9_ABA2_911DF0E1AF55__INCLUDED_)
#define AFX_TDLREGULARITYCOMBOBOX_H__FEBCAD06_B7B7_46A9_ABA2_911DF0E1AF55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLRegularityComboBox.h : header file
//

#include "..\shared\OwnerdrawComboBoxBase.h"

#include "..\Interfaces\IEnums.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLRegularityComboBox window

class CTDLRegularityComboBox : public COwnerdrawComboBoxBase
{
// Construction
public:
	CTDLRegularityComboBox(BOOL bIncludeAny);
	virtual ~CTDLRegularityComboBox();

	TDC_REGULARITY GetSelectedRegularity() const;
	int SetSelectedRegularity(TDC_REGULARITY nRegularity);

	static CString GetRegularity(TDC_REGULARITY nRegularity);

protected:
	BOOL m_bIncludeAny;

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLRegularityComboBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLRegularityComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	void BuildCombo();

	void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
					  DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLREGULARITYCOMBOBOX_H__FEBCAD06_B7B7_46A9_ABA2_911DF0E1AF55__INCLUDED_)
