#if !defined(AFX_TDLCLEANUPPREFERENCESDLG_H__F2C8506D_F58D_4EB0_93FE_4F50522B11A4__INCLUDED_)
#define AFX_TDLCLEANUPPREFERENCESDLG_H__F2C8506D_F58D_4EB0_93FE_4F50522B11A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLCleanupPreferencesDlg.h : header file
//

#include "TDLDialog.h"

#include "..\shared\checklistboxex.h"
#include "..\Shared\EnRecentFileList.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLCleanupPreferencesDlg dialog

class CTDLCleanupIniPreferencesDlg : public CTDLDialog
{
// Construction
public:
	CTDLCleanupIniPreferencesDlg(const CStringArray& aExclusions, CWnd* pParent = NULL);   // standard constructor

protected:
// Dialog Data
	//{{AFX_DATA(CTDLCleanupPreferencesDlg)
	CCheckListBoxEx	m_lbTasklists;
	CString	m_sPrefsFile;
	//}}AFX_DATA
	CStringArray m_aExclusions, m_aMRU, m_aLastOpen;
	CEnRecentFileList m_mru;
	CString m_sLastActiveFile;

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLCleanupPreferencesDlg)
	//}}AFX_VIRTUAL
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual BOOL OnInitDialog();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLCleanupPreferencesDlg)
	//}}AFX_MSG
	afx_msg void OnSelectAll();
	afx_msg void OnClearAll();

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLCLEANUPPREFERENCESDLG_H__F2C8506D_F58D_4EB0_93FE_4F50522B11A4__INCLUDED_)
