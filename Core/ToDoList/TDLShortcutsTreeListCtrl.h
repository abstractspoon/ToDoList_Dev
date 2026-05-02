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
	afx_msg void OnSize(UINT nType, int cx, int cy);
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

#endif // !defined(AFX_TDLSHORTCUTSTREELISTCTRL_H__DA5D005D_C6CC_453A_A431_A2B85A920CE5__INCLUDED_)
