#if !defined(AFX_PREFERENCESSHORTCUTSPAGE_H__DA5D005D_C6CC_453A_A431_A2B85A920CE5__INCLUDED_)
#define AFX_PREFERENCESSHORTCUTSPAGE_H__DA5D005D_C6CC_453A_A431_A2B85A920CE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// PrefererencesShortcutsPage.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#include "..\shared\shortcutManager.h"
#include "..\shared\orderedtreectrl.h"
#include "..\shared\hotkeyctrlex.h"
#include "..\shared\preferencesbase.h"
#include "..\shared\enstring.h"
#include "..\shared\FontCache.h"

/////////////////////////////////////////////////////////////////////////////

class CMenuIconMgr;

/////////////////////////////////////////////////////////////////////////////
// CPreferencesShortcutsPage dialog

class CPreferencesShortcutsPage : public CPreferencesPageBase 
{
public:
	CPreferencesShortcutsPage(const CMenuIconMgr& mgrIcons, CShortcutManager* pMgrShortcuts);
	~CPreferencesShortcutsPage();

	UINT GetShortcutCmdID(DWORD dwShortcut) const { return m_pMgrShortcuts->GetCommandID(dwShortcut); }
	BOOL RemapMenuItemIDs(const CMap<UINT, UINT, UINT, UINT&>& mapCmdIDs);

protected:
	CHotKeyCtrlEx	m_hkCur;
	COrderedTreeCtrl	m_tcCommands;
	CHotKeyCtrlEx	m_hkNew;
	CEnString	m_sOtherCmdID;
	BOOL	m_bShowCommandIDs;
	CFontCache m_fonts;

	const CMenuIconMgr& m_mgrMenuIcons;
	CShortcutManager* m_pMgrShortcuts;

	CMap<UINT, UINT, DWORD, DWORD&> m_mapID2Shortcut;
	CMap<DWORD, DWORD, HTREEITEM, HTREEITEM&> m_mapShortcut2HTI;

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnFirstShow();

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
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

	virtual int HighlightUIText(const CStringArray& aSearch, COLORREF crHighlight);
	virtual void ClearHighlights();

	HTREEITEM AddMenuItem(HTREEITEM htiParent, const CMenu* pMenu, int nPos, BOOL bForceAdd);
	int GetLongestShortcutText(HTREEITEM hti, CDC* pDC);
	void AddMiscShortcuts();
	void RemoveUnusedDefaultFilterItems(CMenu& menu) const;
	BOOL CopyItem(HTREEITEM hti, CString& sOutput);
	void BuildMenuTree();
	HTREEITEM InsertItem(const CString& sItem, UINT nCmdID, HTREEITEM htiParent, BOOL bForceAdd);
	BOOL WantKeepSubmenu(HTREEITEM hti) const;
	BOOL WantItem(const CString& sItem) const;
	BOOL MatchesSearch(const CString& sItem) const;

	static BOOL IsMiscCommandID(UINT nCmdID);

};

#endif // !defined(AFX_PREFERERENCESSHORTCUTSPAGE_H__DA5D005D_C6CC_453A_A431_A2B85A920CE5__INCLUDED_)
