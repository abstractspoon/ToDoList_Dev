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

// replacement DDX routine
void AFXAPI DDX_CBRisk(CDataExchange* pDX, int nIDC, int& nRisk);

class CTDLRiskComboBox : public COwnerdrawComboBoxBase
{
// Construction
public:
	CTDLRiskComboBox();

	int GetSelectedRisk() const; // -2 -> 10
	void SetSelectedRisk(int nRisk); // -2 -> 10

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLRiskComboBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:

	// Implementation
public:
	virtual ~CTDLRiskComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLRiskComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
   void BuildCombo();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLRISKCOMBOBOX_H__94985E3D_C2CA_44C0_B3AD_E55C110AEE45__INCLUDED_)
