#if !defined(AFX_TDLTHEMECOMBOBOX_H__94985E3D_C2CA_44C0_B3AD_E55C110AEE45__INCLUDED_)
#define AFX_TDLTHEMECOMBOBOX_H__94985E3D_C2CA_44C0_B3AD_E55C110AEE45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLPriorityComboBox.h : header file
//

#include "..\shared\colorcombobox.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLThemeComboBox window

class CTDLThemeComboBox : public CColorComboBox
{
// Construction
public:
	CTDLThemeComboBox();

	CString GetThemePath() const;
	BOOL SetThemePath(LPCTSTR szThemePath);

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLThemeComboBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTDLThemeComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLThemeComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
   void BuildCombo();

   static CString ExtractThemeName(LPCTSTR szThemePath);
   static CString FormatThemePath(LPCTSTR szThemeName);
   static CString GetThemeFolder();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTHEMECOMBOBOX_H__94985E3D_C2CA_44C0_B3AD_E55C110AEE45__INCLUDED_)
