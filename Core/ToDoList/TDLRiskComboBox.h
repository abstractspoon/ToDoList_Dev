#if !defined(AFX_TDLRISKCOMBOBOX_H__94985E3D_C2CA_44C0_B3AD_E55C110AEE45__INCLUDED_)
#define AFX_TDLRISKCOMBOBOX_H__94985E3D_C2CA_44C0_B3AD_E55C110AEE45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLRiskComboBox.h : header file
//

#include "..\Shared\ownerdrawcomboboxbase.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLRiskComboBox window

class CTDLRiskComboBox : public COwnerdrawComboBoxBase
{
// Construction
public:
	CTDLRiskComboBox(BOOL bIncludeAny, BOOL bIncludeNone = TRUE);
	virtual ~CTDLRiskComboBox();

	void SetNumLevels(int nNumLevels); // 2 - 11
	int GetNumLevels() const { return m_nNumLevels; }

	int GetSelectedRisk() const; // -2 -> 10
	void SetSelectedRisk(int nRisk); // -2 -> 10

	void DDX(CDataExchange* pDX, int& nRisk);

protected:
	BOOL m_bIncludeAny, m_bIncludeNone;
	int m_nNumLevels;

protected:
	DECLARE_MESSAGE_MAP()

protected:
   virtual void BuildCombo();
   virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
	   DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLRISKCOMBOBOX_H__94985E3D_C2CA_44C0_B3AD_E55C110AEE45__INCLUDED_)
