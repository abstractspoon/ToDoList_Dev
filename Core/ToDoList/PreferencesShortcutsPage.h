#if !defined(AFX_PREFERENCESSHORTCUTSPAGE_H__DA5D005D_C6CC_453A_A431_A2B85A920CE5__INCLUDED_)
#define AFX_PREFERENCESSHORTCUTSPAGE_H__DA5D005D_C6CC_453A_A431_A2B85A920CE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrefererencesShortcutsPage.h : header file
//

#include "..\shared\shortcutManager.h"
#include "..\shared\orderedtreectrl.h"
#include "..\shared\hotkeyctrlex.h"
#include "..\shared\preferencesbase.h"
#include "..\shared\enstring.h"

/////////////////////////////////////////////////////////////////////////////
// CPreferencesShortcutsPage dialog

class CPreferencesShortcutsPage : public CPreferencesPageBase 
{
// Construction
public:
	CPreferencesShortcutsPage(CShortcutManager* pMgr);
	~CPreferencesShortcutsPage();

	UINT GetShortcutCmdID(DWORD dwShortcut) const { return m_pShortcutMgr->GetCommandID(dwShortcut); }
	BOOL RemapMenuItemIDs(const CMap<UINT, UINT, UINT, UINT&>& mapCmdIDs);

protected:
// Dialog Data
	//{{AFX_DATA(CPreferencesShortcutsPage)
	enum { IDD = IDD_PREFSHORTCUTS_PAGE };
	CHotKeyCtrlEx	m_hkCur;
	COrderedTreeCtrl	m_tcCommands;
	CHotKeyCtrlEx	m_hkNew;
	CEnString	m_sOtherCmdID;
	BOOL	m_bShowCommandIDs;
	//}}AFX_DATA

	CShortcutManager* m_pShortcutMgr;
	CMap<UINT, UINT, DWORD, DWORD&> m_mapID2Shortcut;
	CMap<DWORD, DWORD, HTREEITEM, HTREEITEM&> m_mapShortcut2HTI;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesShortcutsPage)
	//}}AFX_VIRTUAL

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnFirstShow();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPreferencesShortcutsPage)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg void OnSelchangedShortcuts(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAssignshortcut();
	afx_msg void OnShowCmdIDs();
	afx_msg void OnCopyall();
	afx_msg void OnChangeShortcut();
	afx_msg LRESULT OnGutterDrawItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterPostDrawItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGutterRecalcColWidth(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTreeCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_MESSAGE_MAP()

protected:
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
	virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

	HTREEITEM AddMenuItem(HTREEITEM htiParent, const CMenu* pMenu, int nPos);
	int GetLongestShortcutText(HTREEITEM hti, CDC* pDC);
	void AddMiscShortcuts();
	BOOL CopyItem(HTREEITEM hti, CString& sOutput);

	static BOOL IsMiscCommandID(UINT nCmdID);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERERENCESSHORTCUTSPAGE_H__DA5D005D_C6CC_453A_A431_A2B85A920CE5__INCLUDED_)
