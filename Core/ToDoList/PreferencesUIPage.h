#if !defined(AFX_PREFERENCESUIPAGE_H__5AE787F2_44B0_4A48_8D75_24C6C16B45DF__INCLUDED_)
#define AFX_PREFERENCESUIPAGE_H__5AE787F2_44B0_4A48_8D75_24C6C16B45DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesUIPage.h : header file
//

#include "TDLTaskViewListBox.h"
#include "tdlthemecombobox.h"

#include "..\shared\preferencesbase.h"

/////////////////////////////////////////////////////////////////////////////

enum PUIP_NEWTASKPOS
{ 
	PUIP_TOP,
	PUIP_BOTTOM,
	PUIP_ABOVE,
	PUIP_BELOW,
};

enum PUIP_LOCATION
{
	PUIP_LOCATEBOTTOM,
	PUIP_LOCATERIGHT,
	PUIP_LOCATELEFT,
};

enum PUIP_MATCHTITLE
{
	PUIP_MATCHONTITLE,
	PUIP_MATCHONTITLECOMMENTS,
	PUIP_MATCHONANYTEXT,
};

/////////////////////////////////////////////////////////////////////////////
// CPreferencesUIPage dialog

class CUIExtensionMgr;

class CPreferencesUIPage : public CPreferencesPageBase
{
	DECLARE_DYNCREATE(CPreferencesUIPage)

// Construction
public:
	CPreferencesUIPage(const CUIExtensionMgr* pMgrUIExt = NULL);
	~CPreferencesUIPage();

	BOOL GetShowEditMenuAsColumns() const { return m_bShowEditMenuAsColumns; }
	BOOL GetShowSortMenuAsColumns() const { return GetShowEditMenuAsColumns(); }
	BOOL GetShowCommentsAlways() const { return m_bShowCommentsAlways; }
	BOOL GetAutoReposCtrls() const { return m_bAutoReposCtrls; }
	BOOL GetShareCommentsSize() const { return m_bShareCommentsSize; }
	BOOL GetAutoHideTabbar() const { return m_bAutoHideTabbar; }
	BOOL SetAutoHideTabbar(BOOL bAutoHide);
	BOOL GetStackTabbarItems() const { return m_bStackTabbarItems; }
	BOOL GetFocusTreeOnEnter() const { return m_bFocusTreeOnEnter; }
	PUIP_NEWTASKPOS GetNewTaskPos() const { return m_nNewTaskPos; }
	PUIP_NEWTASKPOS GetNewSubtaskPos() const { return m_nNewSubtaskPos; }
	BOOL GetKeepTabsOrdered() const { return m_bKeepTabsOrdered; }
	BOOL GetShowTasklistTabCloseButton() const { return m_bShowTasklistTabCloseButton; }
	BOOL GetSortDoneTasksAtBottom() const { return m_bSortDoneTasksAtBottom; }
	BOOL GetRTLComments() const { return m_bRTLComments; }
	PUIP_LOCATION GetCommentsPos() const { return m_nCommentsPos; }
	PUIP_LOCATION GetControlsPos() const { return m_nCtrlsPos; }
	BOOL GetMultiSelFilters() const { return m_bMultiSelFilters; }
	BOOL GetRestoreTasklistFilters() const { return m_bRestoreTasklistFilters; }
	BOOL GetReFilterOnModify() const { return m_bAutoRefilter; }
	BOOL GetReSortOnModify() const { return m_bAutoResort; }
	CString GetUITheme() const;
	PUIP_MATCHTITLE GetTitleFilterOption() const { return m_nTitleFilterOption; }
	BOOL GetShowDefaultTaskIcons() const { return m_bShowDefaultTaskIcons; }
	BOOL GetShowDefaultFiltersInFilterBar() const { return m_bShowDefaultFilters; }
	BOOL GetEnableColumnHeaderSorting() const { return m_bEnableColumnHeaderSorting; }
	int GetDefaultTaskViews(CStringArray& aTypeIDs) const;
	BOOL GetStackEditFieldsAndComments() const { return m_bStackEditFieldsAndComments; }
	BOOL GetStackCommentsAboveEditFields() const { return (m_bStackEditFieldsAndComments && m_bStackCommentsAboveEditFields); }
	BOOL GetIncludeWebLinksInCommentsPaste() const { return m_bIncludeWebLinksInCommentsPaste; }
//	BOOL Get() const { return ; }

protected:
// Dialog Data
	//{{AFX_DATA(CPreferencesUIPage)
	enum { IDD = IDD_PREFUI_PAGE };
	CTDLTaskViewListBox	m_lbTaskViews;
	BOOL	m_bShowCommentsAlways;
	BOOL	m_bAutoReposCtrls;
	BOOL	m_bSpecifyToolbarImage;
	BOOL	m_bShareCommentsSize;
	BOOL	m_bAutoHideTabbar;
	BOOL	m_bStackTabbarItems;
	BOOL	m_bFocusTreeOnEnter;
	BOOL	m_bKeepTabsOrdered;
	BOOL	m_bShowTasklistTabCloseButton;
	BOOL	m_bRTLComments;
	BOOL	m_bShowEditMenuAsColumns;
	BOOL	m_bMultiSelFilters;
	BOOL	m_bRestoreTasklistFilters;
	BOOL	m_bAutoRefilter;
	CString	m_sUIThemeFile;
	BOOL	m_bUseUITheme;
	BOOL	m_bShowDefaultTaskIcons;
	BOOL	m_bShowDefaultFilters;
	BOOL	m_bAutoResort;
	BOOL	m_bEnableColumnHeaderSorting;
	//}}AFX_DATA
	BOOL	m_bStackEditFieldsAndComments;
	BOOL	m_bStackCommentsAboveEditFields;
	BOOL	m_bIncludeWebLinksInCommentsPaste;
	BOOL	m_bSortDoneTasksAtBottom;

	PUIP_NEWTASKPOS	m_nNewTaskPos;
	PUIP_NEWTASKPOS	m_nNewSubtaskPos;
	PUIP_MATCHTITLE m_nTitleFilterOption;
	PUIP_LOCATION	m_nCommentsPos;
	PUIP_LOCATION	m_nCtrlsPos;

	const CUIExtensionMgr* m_pMgrUIExt;
	CTDLThemeComboBox m_cbThemes;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesUIPage)
	//}}AFX_VIRTUAL

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnFirstShow();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPreferencesUIPage)
	afx_msg void OnUseuitheme();
	//}}AFX_MSG
	afx_msg void OnStackEditFieldsAndComments();
	DECLARE_MESSAGE_MAP()

protected:
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
	virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESUIPAGE_H__5AE787F2_44B0_4A48_8D75_24C6C16B45DF__INCLUDED_)
