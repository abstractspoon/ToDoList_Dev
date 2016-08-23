#if !defined(AFX_TDLCOLUMNSELECTIONDLG_H__B8AF67E2_DDBA_4D08_B930_397D338F9D43__INCLUDED_)
#define AFX_TDLCOLUMNSELECTIONDLG_H__B8AF67E2_DDBA_4D08_B930_397D338F9D43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLColumnSelectionDlg.h : header file
//

#include "tdlcolumnlistbox.h"
#include "tdlcolumnattribVisibilitylistctrl.h"
#include "TDLDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLColumnSelectionDlg dialog

class CTDLColumnSelectionDlg : public CTDLDialog
{
// Construction
public:
	CTDLColumnSelectionDlg(const TDCCOLEDITFILTERVISIBILITY& vis, 
							const TDCCOLEDITFILTERVISIBILITY& visDefault, 
							CWnd* pParent = NULL);   // standard constructor

	void GetColumnEditFilterVisibility(TDCCOLEDITFILTERVISIBILITY& vis) const;
	BOOL GetApplyActiveTasklist() const { return m_bActiveTasklist; }
	BOOL GetUpdatePreferences() const { return m_bUpdatePrefs; }

protected:
// Dialog Data
	//{{AFX_DATA(CTDLColumnSelectionDlg)
	enum { IDD = IDD_COLUMNSELECTION_DIALOG };
	CTDLColumnAttribVisibilityListCtrl	m_lcVisibility;
	int		m_bActiveTasklist;
	BOOL	m_bUpdatePrefs;
	//}}AFX_DATA
	TDCCOLEDITFILTERVISIBILITY m_visColAttrib, m_visDefault;
	TDL_SHOWATTRIB m_nAttribShow;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLColumnSelectionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLColumnSelectionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectallcols();
	afx_msg void OnClearallcols();
	afx_msg void OnDefaultcols();
	afx_msg void OnChangeAttribShow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLCOLUMNSELECTIONDLG_H__B8AF67E2_DDBA_4D08_B930_397D338F9D43__INCLUDED_)
