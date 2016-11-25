#if !defined(AFX_PREFERENCESUIVISIBILITYPAGE_H__220CB618_3CFF_47A0_850C_6E43668E8F6C__INCLUDED_)
#define AFX_PREFERENCESUIVISIBILITYPAGE_H__220CB618_3CFF_47A0_850C_6E43668E8F6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesUIVisibilityPage.h : header file
//

#include "tdlcolumnattribVisibilitylistctrl.h"

#include "..\shared\preferencesbase.h"

/////////////////////////////////////////////////////////////////////////////
// CPreferencesUIVisibilityPage dialog

class CPreferencesUIVisibilityPage : public CPreferencesPageBase
{
	DECLARE_DYNCREATE(CPreferencesUIVisibilityPage)

// Construction
public:
	CPreferencesUIVisibilityPage();
	~CPreferencesUIVisibilityPage();

	void GetColumnAttributeVisibility(TDCCOLEDITFILTERVISIBILITY& vis) const;
	void SetColumnAttributeVisibility(const TDCCOLEDITFILTERVISIBILITY& vis);

protected:
// Dialog Data
	//{{AFX_DATA(CPreferencesUIVisibilityPage)
	CTDLColumnAttribVisibilityListCtrl	m_lcVisibility;
	//}}AFX_DATA
	TDL_SHOWATTRIB m_nAttribShow;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesUIVisibilityPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	BOOL OnInitDialog();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPreferencesUIVisibilityPage)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChangeAttribShow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
protected:
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
	virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESUIVISIBILITYPAGE_H__220CB618_3CFF_47A0_850C_6E43668E8F6C__INCLUDED_)
