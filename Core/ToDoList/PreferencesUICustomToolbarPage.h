#if !defined(AFX_PREFERENCESUICUSTOMTOOLBARPAGE_H__6B9BF0B2_D6DE_4868_B97E_8F6288C77778__INCLUDED_)
#define AFX_PREFERENCESUICUSTOMTOOLBARPAGE_H__6B9BF0B2_D6DE_4868_B97E_8F6288C77778__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesToolPage.h : header file
//

#include "TDLToolbarButtonListCtrl.h"

#include "..\shared\preferencesbase.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CPreferencesToolbarPage dialog

class CPreferencesUICustomToolbarPage : public CPreferencesPageBase
{
	DECLARE_DYNCREATE(CPreferencesUICustomToolbarPage)

// Construction
public:
	CPreferencesUICustomToolbarPage();
	~CPreferencesUICustomToolbarPage();

	BOOL HasToolbarButtons() const;
	int GetToolbarButtons(CToolbarButtonArray& aButtons) const;
	UINT GetLastCustomToolbarButtonID() const;

	BOOL RemapMenuItemIDs(const CMap<UINT, UINT, UINT, UINT&>& mapCmdIDs);

protected:

// Dialog Data
	//{{AFX_DATA(CPreferencesToolbarPage)
	enum { IDD = IDD_PREFTOOLBAR_PAGE };
	//}}AFX_DATA

	CTDLToolbarButtonListCtrl m_ilcButtons;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesToolbarPage)
	//}}AFX_VIRTUAL

protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnFirstShow();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPreferencesToolbarPage)
	//}}AFX_MSG
	afx_msg void OnMoveButtonUp();
	afx_msg void OnMoveButtonDown();
	afx_msg void OnDeleteButton();
	afx_msg void OnDuplicateButton();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnListSelChange(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
		
protected:
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
	virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

	void EnableDisableButtons();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESUICUSTOMTOOLBARPAGE_H__6B9BF0B2_D6DE_4868_B97E_8F6288C77778__INCLUDED_)
