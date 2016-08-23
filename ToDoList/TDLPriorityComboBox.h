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

// replacement DDX routine
void AFXAPI DDX_CBPriority(CDataExchange* pDX, int nIDC, int& nPriority);

class CTDLPriorityComboBox : public CColorComboBox
{
// Construction
public:
	CTDLPriorityComboBox();
	
	BOOL SetColors(const CDWordArray& aColors); // must have 11 elements

	// not currently used
//	void SetReverseOrder(BOOL bReverse = TRUE);

	int IncrementPriority(int nAmount);
	int GetSelectedPriority() const; // -2 -> 10
	void SetSelectedPriority(int nPriority); // -2 -> 10

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLPriorityComboBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
//	BOOL m_bReverse;
	CDWordArray m_aColors;

// Implementation
public:
	virtual ~CTDLPriorityComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLPriorityComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
   void BuildCombo();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLPRIORITYCOMBOBOX_H__94985E3D_C2CA_44C0_B3AD_E55C110AEE45__INCLUDED_)
