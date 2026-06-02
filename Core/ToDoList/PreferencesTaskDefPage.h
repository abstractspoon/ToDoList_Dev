#if !defined(AFX_PREFERENCESTASKDEFPAGE_H__852964E3_4ABD_4B66_88BA_F553177616F2__INCLUDED_)
#define AFX_PREFERENCESTASKDEFPAGE_H__852964E3_4ABD_4B66_88BA_F553177616F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesTaskDefPage.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#include "tdcenum.h"
#include "tdlprioritycombobox.h"
#include "tdlriskcombobox.h"
#include "tdcimagelist.h"
#include "tdlreminderperiodcombobox.h"
#include "tdcenumcontainers.h"
#include "tdlcommentsctrl.h"
#include "todoitem.h"
#include "tdcreminder.h"

#include "..\shared\preferencesbase.h"
#include "..\shared\colorbutton.h"
#include "..\shared\timeedit.h"
#include "..\shared\autocombobox.h"
#include "..\shared\maskedit.h"
#include "..\Shared\iconbutton.h"
#include "..\Shared\binarydata.h"
#include "..\Shared\SoundEdit.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CPreferencesTaskDefPage dialog

struct TODOITEM;

class CTDCContentMgr;

/////////////////////////////////////////////////////////////////////////////

class CPreferencesTaskDefPage : public CPreferencesPageBase
{
public:
	CPreferencesTaskDefPage(const CTDCContentMgr* pMgrContent = NULL);
	~CPreferencesTaskDefPage();

	void SetPriorityColors(const CDWordArray& aColors);
	void SetDefaultCommentsFont(const CString& sFaceName, int nPointSize);
	void SetNumPriorityRiskLevels(int nNumLevels);

	void GetTaskAttributes(TODOITEM& tdiDefault) const;
	BOOL GetReminder(TDCREMINDER& rem) const;

protected:
	CTDLReminderPeriodComboBox m_cbDefReminder;
	CTDLRiskComboBox m_cbDefRisk;
	CTDLPriorityComboBox m_cbDefPriority;
	CTimeEdit m_eTimeEst;
	CTimeEdit m_eTimeSpent;
	CMaskEdit m_eCost;
	CColourButton m_btDefColor;
	CIconButton m_btDefIcon;
	CSoundEdit m_eReminderSound;
	CTDLCommentsCtrl m_ctrlComments;
	CTDCImageList m_ilTaskIcons;

	CString	m_sDefCreatedBy;
	BOOL m_bReminderBeforeDue;
	int m_nDefPriority;
	int m_nDefRisk;
	TDCCOST	m_defCost;
	TDCTIMEPERIOD m_defTimeEst, m_defTimeSpent;
	CString	m_sDefAllocTo;
	CString	m_sDefAllocBy;
	CString	m_sDefStatus;
	CString	m_sDefTags;
	CString	m_sDefCategory;
	CString m_sDefIcon;
	CString	m_sReminderSound;
	COLORREF m_crDef;
	BOOL m_bUseCreationDateForDefStartDate;
	BOOL m_bUseCreationTimeForDefStartDate;
	BOOL m_bUseCreationDateForDefDueDate;
	TDC_REMINDERPERIOD m_nDefReminderLeadin;

	const CTDCContentMgr* m_pMgrContent;

	CONTENTFORMAT m_cfDefault;
	CBinaryData m_defCustomComments;
	CString m_sDefTextComments;
	CFont m_fontDefComments;

protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnFirstShow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg void OnSetdefaulticon();
	afx_msg void OnSelchangeReminder();
	afx_msg void OnUseCreationDateForDefStartDate();
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

#endif // !defined(AFX_PREFERENCESTASKDEFPAGE_H__852964E3_4ABD_4B66_88BA_F553177616F2__INCLUDED_)
