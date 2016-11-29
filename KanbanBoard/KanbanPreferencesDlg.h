#if !defined(AFX_KANBANPREFERENCESDLG_H__4BEDF571_7002_4C0D_B355_1334515CA1F9__INCLUDED_)
#define AFX_KANBANPREFERENCESDLG_H__4BEDF571_7002_4C0D_B355_1334515CA1F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KanbanPreferencesDlg.h : header file
//

#include "KanbanStruct.h"
#include "KanbanAttributeComboBox.h"
#include "KanbanFixedColumnListCtrl.h"

#include "..\shared\groupline.h"
#include "..\shared\preferencesbase.h"
#include "..\shared\inputlistctrl.h"
#include "..\Shared\entoolbar.h"
#include "..\Shared\toolbarhelper.h"
#include "..\Shared\checklistboxex.h"

#include "..\Interfaces\ipreferences.h"
#include "..\Interfaces\iuiextension.h"

/////////////////////////////////////////////////////////////////////////////
// CKanbanPreferencesPage dialog

class CKanbanPreferencesPage : public CPreferencesPageBase
{
// Construction
public:
	CKanbanPreferencesPage(CWnd* pParent = NULL);   // standard constructor

	BOOL HasFixedColumns() const { return m_aFixedColumnDefs.GetSize(); }
	int GetFixedColumnDefinitions(CKanbanColumnArray& aColumnDefs) const;
	BOOL GetSortSubtasksBelowParents() const { return m_bSortSubtaskBelowParent; }
	BOOL GetAlwaysShowBacklog() const { return m_bAlwaysShowBacklog; }
	BOOL GetShowTaskColorAsBar() const { return m_bShowTaskColorAsBar; }
	BOOL GetColorBarByPriority() const { return m_bColorBarByPriority; }
	IUI_ATTRIBUTE GetFixedAttributeToTrack(CString& sCustomID) const;
	int GetDisplayAttributes(CKanbanAttributeArray& aAttrib) const;
	int SetDisplayAttributes(const CKanbanAttributeArray& aAttrib);

	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);

	void SetCustomAttributeIDs(const CStringArray& aCustomAttribIDs);
	void SetAttributeValues(const CKanbanAttributeValueMap& mapValues);

protected:
// Dialog Data
	//{{AFX_DATA(CKanbanPreferencesPage)
	BOOL	m_bShowTaskColorAsBar;
	//}}AFX_DATA
	CKanbanAttributeComboBox	m_cbAttributes;
	CKanbanFixedColumnListCtrl	m_lcFixedColumnDefs;
	CGroupLineManager m_mgrGroupLines;
	CEnToolBar m_toolbar;
	CToolbarHelper m_tbHelper;
	CComboBox m_cbCustomAttributes;
	CCheckListBoxEx	m_lbDisplayAttrib;

	BOOL	m_bSortSubtaskBelowParent;
	BOOL	m_bColorBarByPriority;
	BOOL	m_bAlwaysShowBacklog;
	CString	m_sFixedCustomAttribID;
	IUI_ATTRIBUTE m_nFixedAttrib;

	CKanbanColumnArray m_aFixedColumnDefs;
	CStringArray m_aCustomAttribIDs;
	CKanbanAttributeValueMap m_mapAttribValues;
	CKanbanAttributeArray m_aDisplayAttrib;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKanbanPreferencesPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
	virtual void OnOK();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKanbanPreferencesPage)
	afx_msg void OnChangeColumnType();
	afx_msg void OnSelchangeAttribute();
	afx_msg void OnMoveFixedColDown();
	afx_msg void OnUpdateFixedMoveColDown(CCmdUI* pCmdUI);
	afx_msg void OnMoveFixedColUp();
	afx_msg void OnUpdateMoveFixedColUp(CCmdUI* pCmdUI);
	afx_msg void OnItemchangedColumndefs(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnShowColorAsBar();
	afx_msg void OnPopulateFixedColumns();
	afx_msg void OnUpdatePopulateColumns(CCmdUI* pCmdUI);
	afx_msg void OnSelchangeCustomAttribute();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	DECLARE_MESSAGE_MAP()

protected:
	void EnableDisableControls();
	void UpdateAttributeValueCombo();
	CString GetFixedAttributeID() const;
	void BuildDisplayAttributeListBox();
};

/////////////////////////////////////////////////////////////////////////////
// CKanbanPreferencesDlg dialog

class CKanbanPreferencesDlg : public CPreferencesDlgBase
{
// Construction
public:
	CKanbanPreferencesDlg(CWnd* pParent);   // standard constructor

	BOOL HasFixedColumns() const { return m_page.HasFixedColumns(); }
	int GetFixedColumnDefinitions(CKanbanColumnArray& aColumnDefs) const { return m_page.GetFixedColumnDefinitions(aColumnDefs); }
	BOOL GetSortSubtasksBelowParents() const { return m_page.GetSortSubtasksBelowParents(); }
	BOOL GetAlwaysShowBacklog() const { return m_page.GetAlwaysShowBacklog(); }
	BOOL GetShowTaskColorAsBar() const { return m_page.GetShowTaskColorAsBar(); }
	BOOL GetColorBarByPriority() const { return m_page.GetColorBarByPriority(); }
	IUI_ATTRIBUTE GetFixedAttributeToTrack(CString& sCustomID) const { return m_page.GetFixedAttributeToTrack(sCustomID); }
	int GetDisplayAttributes(CKanbanAttributeArray& aAttrib) const { return m_page.GetDisplayAttributes(aAttrib); }

	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const { m_page.SavePreferences(pPrefs, szKey); }
	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey) { m_page.LoadPreferences(pPrefs, szKey); }

	int DoModal(const CStringArray& aCustomAttribIDs, const CKanbanAttributeValueMap& mapValues, const CKanbanAttributeArray& aDisplayAttrib);

protected:
	CKanbanPreferencesPage m_page;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoHelp();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CKanbanPreferencesDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KanbanPREFERENCESDLG_H__4BEDF571_7002_4C0D_B355_1334515CA1F9__INCLUDED_)
