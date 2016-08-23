// TreeCtrlHelper.h: interface for the CTreeCtrlHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREECTRLHELPER_H__F6652DBE_3770_4E1C_A246_057AD6AD16B7__INCLUDED_)
#define AFX_TREECTRLHELPER_H__F6652DBE_3770_4E1C_A246_057AD6AD16B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

#ifndef TVM_SETEXTENDEDSTYLE
#	define TVM_SETEXTENDEDSTYLE (TV_FIRST + 44)
#	define TreeView_SetExtendedStyle(hwnd, dw, mask) (DWORD)SNDMSG((hwnd), TVM_SETEXTENDEDSTYLE, mask, dw)
#endif

#ifndef TVM_GETEXTENDEDSTYLE
#	define TVM_GETEXTENDEDSTYLE (TV_FIRST + 45)
#	define TreeView_GetExtendedStyle(hwnd) (DWORD)SNDMSG((hwnd), TVM_GETEXTENDEDSTYLE, 0, 0)
#endif

//////////////////////////////////////////////////////////////////////

typedef CMap<HTREEITEM, HTREEITEM, int, int&> CMapIndices;

class CHTIMap : public CMap<DWORD, DWORD, HTREEITEM, HTREEITEM&>
{
public:
	inline HTREEITEM GetItem(DWORD dwItemID)
	{
		HTREEITEM hti = NULL;
		Lookup(dwItemID, hti);
		return hti;
	}
};

//////////////////////////////////////////////////////////////////////

enum TCH_EDGE 
{ 
	TCHE_TOP, 
	TCHE_BOTTOM 
};

enum TCH_CHECK 
{ 
	TCHC_NONE		= 0, 
	TCHC_UNCHECKED	= INDEXTOSTATEIMAGEMASK(1), 
	TCHC_CHECKED	= INDEXTOSTATEIMAGEMASK(2), 
	TCHC_MIXED		= INDEXTOSTATEIMAGEMASK(3) 
};

enum TCH_WHERE
{
	TCHW_ABOVE = -1,
	TCHW_ON,
	TCHW_BELOW
};

/////////////////////////////////////////////////////////////////////////////

class CHoldHScroll
{
public:
	CHoldHScroll(HWND hwnd, int nInitialPos = -1) : m_hwnd(hwnd)
	{
		// it's acceptable to pass no HWND -> nothing happens
		if (m_hwnd)
		{
			if (nInitialPos < 0)
				m_nOrgHScrollPos = ::GetScrollPos(hwnd, SB_HORZ);
			else
				m_nOrgHScrollPos = nInitialPos;
		}
	}
	
	~CHoldHScroll()
	{
		if (m_hwnd)
		{
			if (::GetScrollPos(m_hwnd, SB_HORZ) != m_nOrgHScrollPos)
			{
				::SendMessage(m_hwnd, WM_HSCROLL, MAKEWPARAM(SB_THUMBPOSITION, m_nOrgHScrollPos), 0L);
				::UpdateWindow(m_hwnd);
			}
		}
	}
	
protected:
	HWND m_hwnd;
	int m_nOrgHScrollPos;
};

/////////////////////////////////////////////////////////////////////////////

class CDisableTreeTips
{
public:
	CDisableTreeTips(CTreeCtrl& tree, BOOL bToolTips = TRUE, BOOL bInfoTips = TRUE) 
		: m_tree(tree), m_bToolTips(FALSE), m_bInfoTips(FALSE)
	{
		DWORD dwStyle = m_tree.GetStyle();
		
		m_bToolTips = (bToolTips && !(dwStyle & TVS_NOTOOLTIPS));
		m_bInfoTips = (bInfoTips && (dwStyle & TVS_INFOTIP));
		
		m_tree.ModifyStyle((m_bInfoTips ? TVS_INFOTIP : 0), (m_bToolTips ? TVS_NOTOOLTIPS : 0));
	}
	
	~CDisableTreeTips()
	{
		m_tree.ModifyStyle((m_bToolTips ? TVS_NOTOOLTIPS : 0), (m_bInfoTips ? TVS_INFOTIP : 0));
		
		if (m_bToolTips)
		{
			m_tree.Invalidate(FALSE);
			m_tree.UpdateWindow();
		}
	}
	
protected:
	CTreeCtrl& m_tree;
	BOOL m_bToolTips, m_bInfoTips;
};

//////////////////////////////////////////////////////////////////////

// predec
struct TCHHCOPY; 

//////////////////////////////////////////////////////////////////////

class CTreeCtrlHelper  
{
public:
	CTreeCtrlHelper(CTreeCtrl& tree);
	virtual ~CTreeCtrlHelper();

	HTREEITEM InsertItem(LPCTSTR lpszItem, int nImage, int nSelImage, 
						LPARAM lParam, HTREEITEM htiParent, HTREEITEM htiAfter,
						BOOL bBold, BOOL bChecked);

	HTREEITEM InsertItem(LPCTSTR lpszItem, int nImage, int nSelImage, 
						LPARAM lParam, HTREEITEM htiParent, HTREEITEM htiAfter,
						BOOL bBold, TCH_CHECK nCheck = TCHC_NONE);
	
	BOOL HasFocus(BOOL bIncEditing = TRUE);
	int IsItemExpanded(HTREEITEM hti, BOOL bFully = FALSE) const; // TRUE, FALSE, -1 if no children
	void ExpandAll(BOOL bExpand = TRUE);
	void ExpandItem(HTREEITEM hti, BOOL bExpand = TRUE, BOOL bChildren = FALSE);
	void ToggleItemExpansion(HTREEITEM hti, BOOL bChildren = FALSE);
	BOOL IsParentItemExpanded(HTREEITEM hti, BOOL bRecursive = FALSE) const;
	BOOL IsItemVisible(HTREEITEM hti) const;
	BOOL ItemHasParent(HTREEITEM hti, HTREEITEM htiParent) const;

	void SetItemIntegral(HTREEITEM hti, int iIntegral);
	
	void SetItemChecked(HTREEITEM hti, BOOL bChecked); // 2 state
	void SetItemChecked(HTREEITEM hti, TCH_CHECK nChecked); // 3 state
	TCH_CHECK GetItemCheckState(HTREEITEM hti) const;

	BOOL SelectItem(HTREEITEM hti); // won't auto edit if item already selected
	inline void EndLabelEdit(BOOL bCancel) { SendMessage(m_tree, TVM_ENDEDITLABELNOW, bCancel, 0); }

	BOOL InvalidateItem(HTREEITEM hti, BOOL bChildren = TRUE);
	void GetClientRect(LPRECT lpRect, HTREEITEM htiFrom);
	int GetItemHeight(HTREEITEM hti = NULL);

	int GetItemPos(HTREEITEM hti, HTREEITEM htiParent);
	int GetItemLevel(HTREEITEM hti);

	HTREEITEM FindItem(DWORD dwItemData, HTREEITEM htiStart = NULL) const;
 	const CTreeCtrl& TreeCtrl() const { return m_tree; }

	BOOL IsItemBold(HTREEITEM hti);
	void SetItemBold(HTREEITEM hti, BOOL bBold = TRUE);
	void SetTopLevelItemsBold(BOOL bBold = TRUE);

	HTREEITEM GetLastVisibleChildItem(HTREEITEM hti) const;
	HTREEITEM GetLastChildItem(HTREEITEM hti) const;
	HTREEITEM GetLastItem() const;

	void SetItemStateEx(HTREEITEM hti, UINT nState, UINT nMask, BOOL bChildren = FALSE);

	HTREEITEM GetTopLevelItem(HTREEITEM hti) const;
	HTREEITEM GetNextTopLevelItem(HTREEITEM hti, BOOL bNext = TRUE) const;

	// returns the top level item whose child is the first visible item (or itself)
	HTREEITEM GetFirstVisibleTopLevelItem(int& nPos); // return 0 if no items
	HTREEITEM GetFirstVisibleTopLevelItem(); // return 0 if no items

	int BuildVisibleIndexMap() const;
	BOOL ItemLineIsOdd(HTREEITEM hti) const;
	void ResetIndexMap() const;

	// return increments of item height
	void SetMinDistanceToEdge(HTREEITEM htiFrom, TCH_EDGE nToEdge, int nItems);
	int GetDistanceToEdge(HTREEITEM htiFrom, TCH_EDGE nToEdge) const;

	// get next/prev selectable items, NULL if none
	HTREEITEM GetNextPageVisibleItem(HTREEITEM hti) const;
	HTREEITEM GetPrevPageVisibleItem(HTREEITEM hti) const;

	HTREEITEM GetNextVisibleItem(HTREEITEM hti, BOOL bAllowChildren = TRUE) const;
	HTREEITEM GetPrevVisibleItem(HTREEITEM hti, BOOL bAllowChildren = TRUE) const;

	HTREEITEM FindFirstParent() const;

	HTREEITEM GetNextItem(HTREEITEM hti, BOOL bAllowChildren = TRUE) const; // next item as-if all items were expanded
	HTREEITEM GetPrevItem(HTREEITEM hti, BOOL bAllowChildren = TRUE) const; // prev item as-if all items were expanded

	int FindItem(HTREEITEM htiFind, HTREEITEM htiStart); // return -1 for above, 1 for below, 0 if same
	BOOL IsFullyVisible(HTREEITEM hti) const;

	HTREEITEM FindDeepestItem(int& nDepth, BOOL bExpandedOnly = FALSE) const;
	HTREEITEM FindDeepestItem(BOOL bExpandedOnly = FALSE) const;
	HTREEITEM FindWidestItem(int& nWidth, BOOL bExpandedOnly = FALSE, CDC* pDC = NULL) const;
	HTREEITEM FindWidestItem(BOOL bExpandedOnly = FALSE, CDC* pDC = NULL) const;

	void EnsureVisibleEx(HTREEITEM hti, BOOL bVPartialOK = TRUE, BOOL bHPartialOK = TRUE);
	int BuildHTIMap(CHTIMap& mapHTI, BOOL bVisibleOnly = FALSE) const;
	
	HTREEITEM CopyTree(HTREEITEM hDest, HTREEITEM hSrc, TCH_WHERE nWhere, BOOL bUsesTextCallback = FALSE, BOOL bUsesImageCallback = FALSE);
	HTREEITEM MoveTree(HTREEITEM hDest, HTREEITEM hSrc, TCH_WHERE nWhere, BOOL bUsesTextCallback = FALSE, BOOL bUsesImageCallback = FALSE);

protected:
	CTreeCtrl& m_tree;
	mutable CMapIndices m_mapVisibleIndices;

protected:
	void AddVisibleItemToIndex(HTREEITEM hti) const;
	void UpdateHTIMapEntry(CHTIMap& mapHTI, HTREEITEM hti, BOOL bVisibleOnly) const;
	HTREEITEM FindDeepestItem(HTREEITEM htiParent, int& nDepth, BOOL bExpandedOnly) const;
	HTREEITEM FindWidestItem(HTREEITEM htiParent, int& nWidth, BOOL bExpandedOnly, CDC* pDC) const;

	void BuildCopy(const HTREEITEM hti, TCHHCOPY* pCopy) const;
	HTREEITEM CopyTree(HTREEITEM hDest, const TCHHCOPY* pSrc, TCH_WHERE nWhere, BOOL bUsesTextCallback, BOOL bUsesImageCallback);
	void ExpandItem(HTREEITEM hti, BOOL bExpand, BOOL bChildren, BOOL bParents);

};

#endif // !defined(AFX_TREECTRLHELPER_H__F6652DBE_3770_4E1C_A246_057AD6AD16B7__INCLUDED_)
