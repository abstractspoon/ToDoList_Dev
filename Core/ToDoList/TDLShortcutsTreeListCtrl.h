#if !defined(AFX_TDLSHORTCUTSTREELISTCTRL_H__DA5D005D_C6CC_453A_A431_A2B85A920CE5__INCLUDED_)
#define AFX_TDLSHORTCUTSTREELISTCTRL_H__DA5D005D_C6CC_453A_A431_A2B85A920CE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// TDLShortcutsTreeListCtrl.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#include "..\shared\enstring.h"
#include "..\shared\FontCache.h"
#include "..\shared\TreeListCtrl.h"

/////////////////////////////////////////////////////////////////////////////

class CMenuIconMgr;
class CShortcutManager;
class CWndPromptManager;
class CCtrlTextHighlighter;

/////////////////////////////////////////////////////////////////////////////

class CTDLShortcutsTreeListCtrl : public CTreeListCtrl
{
public:
	CTDLShortcutsTreeListCtrl(const CCtrlTextHighlighter& ctrlHighlighter,
							  const CMenuIconMgr& mgrIcons,
							  CWndPromptManager& mgrPrompts,
							  CShortcutManager* pMgrShortcuts);

	void BuildMenuTree();
	BOOL SaveToShortcutMgr();
	void ShowCommandIDs(BOOL bShow = TRUE);
	void RecalcColumnsToFit();

	BOOL AssignShortcut(UINT nCmdID, DWORD dwShortcut);
	BOOL DeleteShortcut(UINT nCmdID);
	DWORD GetShortcut(UINT nCmdID) const;
	BOOL HasShorcut(DWORD dwShortcut) const;
	UINT GetCmdID(DWORD dwShortcut) const;
	CString GetMenuText(UINT nCmdID) const;

	BOOL MatchesSearch(const CString& sItem) const;
	UINT GetSelectedCmdID() const;
	DWORD GetSelectedShortcut() const;
	void CopyAllToClipboard() const;

	static BOOL IsMiscCommandID(UINT nCmdID);

protected:
	const CMenuIconMgr& m_mgrMenuIcons;
	const CCtrlTextHighlighter& m_ctrlHighlighter;

	CWndPromptManager& m_mgrPrompts;
	CShortcutManager* m_pMgrShortcuts;

	CMap<UINT, UINT, DWORD, DWORD&> m_mapID2Shortcut;
 	CMap<DWORD, DWORD, UINT, UINT&> m_mapShortcut2ID;
	CFontCache m_fonts;
	BOOL m_bShowCommandIDs;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

protected:
	HTREEITEM AddMenuItem(HTREEITEM htiParent, const CMenu* pMenu, int nPos, BOOL bForceAdd, UINT& nNextSubMenuCmdID);
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

	static BOOL IsSubMenu(UINT nCmdID) { return ((int)nCmdID < 0); }
};

#endif // !defined(AFX_TDLSHORTCUTSTREELISTCTRL_H__DA5D005D_C6CC_453A_A431_A2B85A920CE5__INCLUDED_)
