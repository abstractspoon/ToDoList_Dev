#if !defined(AFX_PREFERENCESSHORTCUTSPAGE_H__DA5D005D_C6CC_453A_A431_A2B85A920CE5__INCLUDED_)
#define AFX_PREFERENCESSHORTCUTSPAGE_H__DA5D005D_C6CC_453A_A431_A2B85A920CE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// PrefererencesShortcutsPage.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#include "..\shared\shortcutManager.h"
#include "..\shared\hotkeyctrlex.h"
#include "..\shared\preferencesbase.h"
#include "..\shared\enstring.h"
#include "..\shared\FontCache.h"
#include "..\shared\TreeListCtrl.h"

/////////////////////////////////////////////////////////////////////////////

class CMenuIconMgr;

/////////////////////////////////////////////////////////////////////////////

class CTDLShortcutsTreeListCtrl : public CTreeListCtrl
{
public:
	CTDLShortcutsTreeListCtrl(const CCtrlTextHighlighter& ctrlHighlighter,
							  const CMenuIconMgr& mgrIcons,
							  CWndPromptManager& mgrPrompts,
							  CShortcutManager* pMgrShortcuts);

	void BuildMenuTree();
	BOOL AssignShortcut(UINT nCmdID, DWORD dwShortcut);
	BOOL SaveToShortcutMgr();
	void ShowCommandIDs(BOOL bShow = TRUE);
	void RecalcColumnsToFit();

	BOOL MatchesSearch(const CString& sItem) const;
	UINT GetSelectedCmdID() const;
	UINT GetSelectedShortcut() const;
	void CopyAllToClipboard() const;

protected:
	const CMenuIconMgr& m_mgrMenuIcons;
	const CCtrlTextHighlighter& m_ctrlHighlighter;

	CWndPromptManager& m_mgrPrompts;
	CShortcutManager* m_pMgrShortcuts;

	CMap<UINT, UINT, DWORD, DWORD&> m_mapID2Shortcut;
 	CMap<DWORD, DWORD, HTREEITEM, HTREEITEM&> m_mapShortcut2HTI;
	CFontCache m_fonts;
	BOOL m_bShowCommandIDs;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

protected:
	HTREEITEM AddMenuItem(HTREEITEM htiParent, const CMenu* pMenu, int nPos, BOOL bForceAdd);
	int GetLongestShortcutText(CDC* pDC, HTREEITEM hti = NULL);
	void AddMiscShortcuts();
	void RemoveUnusedDefaultFilterItems(CMenu& menu) const;
	BOOL CopyItem(HTREEITEM hti, CString& sOutput) const;
	HTREEITEM InsertItem(const CString& sItem, UINT nCmdID, HTREEITEM htiParent, BOOL bForceAdd);
	BOOL WantKeepSubmenu(HTREEITEM hti) const;
	BOOL WantItem(const CString& sItem) const;

	// base class overrides
	LRESULT OnListCustomDraw(NMLVCUSTOMDRAW* pLVCD, const CIntArray& aColOrder, const CIntArray& aColWidths);
	COLORREF GetTreeItemBackColor(HTREEITEM hti, DWORD dwItemData, BOOL bSelected) const;
	void DrawTreeSubItemText(CDC* pDC, HTREEITEM hti, DWORD dwItemData, int nCol, const CRect& rSubItem, BOOL bSelected);
	void DrawTreeItemIcon(CDC* pDC, HTREEITEM hti, DWORD dwItemData, const CRect& rLabel);

	static BOOL IsMiscCommandID(UINT nCmdID);
};

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
	CTDLShortcutsTreeListCtrl m_ctrlCommands;

	CHotKeyCtrlEx m_hkCur;
	CHotKeyCtrlEx m_hkNew;
	CEnString m_sOtherCmdID;
	BOOL m_bShowCommandIDs;

	CShortcutManager* m_pMgrShortcuts;

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
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_MESSAGE_MAP()

protected:
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
	virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

	virtual int HighlightUIText(const CStringArray& aSearch, COLORREF crHighlight);
	virtual void ClearHighlights();

	static BOOL IsMiscCommandID(UINT nCmdID);

};

#endif // !defined(AFX_PREFERERENCESSHORTCUTSPAGE_H__DA5D005D_C6CC_453A_A431_A2B85A920CE5__INCLUDED_)
