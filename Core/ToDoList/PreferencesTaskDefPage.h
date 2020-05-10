#if !defined(AFX_PREFERENCESTASKDEFPAGE_H__852964E3_4ABD_4B66_88BA_F553177616F2__INCLUDED_)
#define AFX_PREFERENCESTASKDEFPAGE_H__852964E3_4ABD_4B66_88BA_F553177616F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesTaskDefPage.h : header file
//

#include "tdcenum.h"
#include "tdlprioritycombobox.h"
#include "tdlriskcombobox.h"
#include "tdcimagelist.h"
#include "tdlreminderperiodcombobox.h"
#include "tdcenumcontainers.h"
#include "tdlcommentsctrl.h"
#include "todoitem.h"

#include "..\shared\preferencesbase.h"
#include "..\shared\colorbutton.h"
#include "..\shared\timeedit.h"
#include "..\shared\wndPrompt.h"
#include "..\shared\autocombobox.h"
#include "..\shared\maskedit.h"
#include "..\Shared\iconbutton.h"
#include "..\Shared\binarydata.h"

#include "..\Interfaces\contentMgr.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CPreferencesTaskDefPage dialog

struct TODOITEM;

/////////////////////////////////////////////////////////////////////////////

class CPreferencesTaskDefPage : public CPreferencesPageBase
{
	DECLARE_DYNCREATE(CPreferencesTaskDefPage)

// Construction
public:
	CPreferencesTaskDefPage(const CTDLContentMgr* pMgrContent = NULL);
	~CPreferencesTaskDefPage();

	void SetPriorityColors(const CDWordArray& aColors);
	void SetDefaultCommentsFont(const CString& sFaceName, int nPointSize);

	void GetTaskAttributes(TODOITEM& tdiDefault) const;
	BOOL GetReminder(UINT& nMinutes, BOOL& bBeforeDue) const;

protected:
// Dialog Data
	//{{AFX_DATA(CPreferencesTaskDefPage)
	enum { IDD = IDD_PREFTASKDEF_PAGE };
	//}}AFX_DATA

	CTDLReminderPeriodComboBox	m_cbDefReminder;
	CTDLRiskComboBox	m_cbDefRisk;
	CTDLPriorityComboBox	m_cbDefPriority;
	CTimeEdit	m_eTimeEst;
	CTimeEdit	m_eTimeSpent;
	CMaskEdit m_eCost;
	CColourButton	m_btDefColor;
	CIconButton		m_btDefIcon;

	CString	m_sDefCreatedBy;
	BOOL	m_bReminderBeforeDue;
	int		m_nDefPriority;
	int		m_nDefRisk;
	TDCCOST	m_defCost;
	TDCTIMEPERIOD m_defTimeEst, m_defTimeSpent;
	CString	m_sDefAllocTo;
	CString	m_sDefAllocBy;
	CString	m_sDefStatus;
	CString	m_sDefTags;
	CString	m_sDefCategory;
	CString m_sDefIcon;
	COLORREF m_crDef;
	BOOL	m_bUseCreationDateForDefStartDate;
	BOOL	m_bUseCreationTimeForDefStartDate;
	BOOL	m_bUseCreationDateForDefDueDate;
	CWndPromptManager m_mgrPrompts;
	CTDCImageList m_ilTaskIcons;
	int m_nDefReminderLeadin;

	const CContentMgr* m_pMgrContent;
	CTDLCommentsCtrl m_ctrlComments;
	CONTENTFORMAT m_cfDefault;
	CBinaryData m_defCustomComments;
	CString m_sDefTextComments;
	CFont m_fontDefComments;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesTaskDefPage)
	//}}AFX_VIRTUAL

protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnFirstShow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPreferencesTaskDefPage)
	afx_msg void OnSetdefaulticon();
	afx_msg void OnSelchangeReminder();
	afx_msg void OnUseCreationDateForDefStartDate();
	//}}AFX_MSG
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetdefaultcolor();
	afx_msg void OnSelchangeCommentsformat();
	afx_msg LRESULT OnCommentsChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInitComments(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
		
protected:
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
	virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
	
	void UpdateCommentsSize();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESTASKDEFPAGE_H__852964E3_4ABD_4B66_88BA_F553177616F2__INCLUDED_)
