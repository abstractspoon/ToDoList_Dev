#if !defined(AFX_PREFERENCESUIPAGE_H__5AE787F2_44B0_4A48_8D75_24C6C16B45DF__INCLUDED_)
#define AFX_PREFERENCESUIPAGE_H__5AE787F2_44B0_4A48_8D75_24C6C16B45DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesUIPage.h : header file
//

#include "TDLTaskViewListBox.h"
#include "tdlcontenttypecombobox.h"
#include "tdlthemecombobox.h"

#include "..\shared\contentMgr.h"
#include "..\shared\groupline.h"
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
	CPreferencesUIPage(const CContentMgr* pMgrContent = NULL, const CUIExtensionMgr* pMgrUIExt = NULL);
	~CPreferencesUIPage();

	BOOL GetShowEditMenuAsColumns() const { return m_bShowEditMenuAsColumns; }
	BOOL GetShowCommentsAlways() const { return m_bShowCommentsAlways; }
	BOOL GetAutoReposCtrls() const { return m_bAutoReposCtrls; }
	BOOL GetSharedCommentsHeight() const { return m_bSharedCommentsHeight; }
	BOOL GetAutoHideTabbar() const { return m_bAutoHideTabbar; }
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
	CONTENTFORMAT GetDefaultCommentsFormat() const { return m_cfDefault; }
	BOOL GetMultiSelFilters() const { return m_bMultiSelFilters; }
	BOOL GetRestoreTasklistFilters() const { return m_bRestoreTasklistFilters; }
	BOOL GetReFilterOnModify() const { return m_bAutoRefilter; }
	BOOL GetReSortOnModify() const { return m_bAutoResort; }
	CString GetUITheme() const;
	PUIP_MATCHTITLE GetTitleFilterOption() const { return m_nTitleFilterOption; }
	BOOL GetShowDefaultTaskIcons() const { return m_bShowDefaultTaskIcons; }
	BOOL GetShowDefaultFilters() const { return m_bShowDefaultFilters; }
	BOOL GetEnableColumnHeaderSorting() const { return m_bEnableColumnHeaderSorting; }
	int GetDefaultTaskViews(CStringArray& aTypeIDs) const;
	BOOL GetStackEditFieldsAndComments() const { return m_bStackEditFieldsAndComments; }
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
	BOOL	m_bSharedCommentsHeight;
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
	BOOL	m_bStackEditFieldsAndComments;
	//}}AFX_DATA
//	BOOL	m_bShowCtrlsAsColumns;
	BOOL	m_bIncludeWebLinksInCommentsPaste;
	PUIP_NEWTASKPOS	m_nNewTaskPos;
	PUIP_NEWTASKPOS	m_nNewSubtaskPos;
	PUIP_MATCHTITLE m_nTitleFilterOption;
	PUIP_LOCATION	m_nCommentsPos;
	PUIP_LOCATION	m_nCtrlsPos;
	CTDLContentTypeComboBox	m_cbCommentsFmt;
	BOOL	m_bSortDoneTasksAtBottom;
	const CContentMgr* m_pMgrContent;
	const CUIExtensionMgr* m_pMgrUIExt;
	CGroupLineManager m_mgrGroupLines;
	CONTENTFORMAT m_cfDefault;
	int m_nDefaultCommentsFormat;
	CTDLThemeComboBox m_cbThemes;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesUIPage)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPreferencesUIPage)
	afx_msg void OnUseuitheme();
	//}}AFX_MSG
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCommentsformat();
	DECLARE_MESSAGE_MAP()

protected:
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
	virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESUIPAGE_H__5AE787F2_44B0_4A48_8D75_24C6C16B45DF__INCLUDED_)
