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

#ifndef TVGN_LASTVISIBLE
#	define TVGN_LASTVISIBLE 0x000A
#endif

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

//////////////////////////////////////////////////////////////////////

typedef CMap<HTREEITEM, HTREEITEM, int, int&> CMapIndices;

class CHTIMap : public CMap<DWORD, DWORD, HTREEITEM, HTREEITEM&>
{
public:
	HTREEITEM GetItem(DWORD dwItemID) const;
	BOOL HasItem(DWORD dwItemID) const;
	BOOL HasItems(const CDWordArray& aItemIDs) const;

	void AddItem(const CTreeCtrl& tree, HTREEITEM hti, BOOL bVisibleChildrenOnly = FALSE);
	BOOL RemoveItem(const CTreeCtrl& tree, HTREEITEM hti);
	
#ifdef _DEBUG
	void Trace(CTreeCtrl& tree) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

class CDisableTreeTips
{
public:
	CDisableTreeTips(CTreeCtrl& tree, BOOL bToolTips = TRUE, BOOL bInfoTips = TRUE);
	~CDisableTreeTips();
	
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
	void ExpandItem(HTREEITEM hti, BOOL bExpand = TRUE, BOOL bChildren = FALSE, BOOL bParents = TRUE);
	void ToggleItemExpansion(HTREEITEM hti, BOOL bChildren = FALSE);
	BOOL IsParentItemExpanded(HTREEITEM hti, BOOL bRecursive = FALSE) const;
	BOOL ItemHasParent(HTREEITEM hti, HTREEITEM htiParent) const;
	BOOL IsAnyItemExpanded() const;
	BOOL IsAnyItemCollapsed() const;
	int GetItemData(CDWordArray& aItemData, BOOL bIncParents = TRUE, BOOL bInCollapsedChildren = TRUE) const;

	BOOL IsItemFullyVisible(HTREEITEM hti) const;
	BOOL IsItemVisible(HTREEITEM hti, BOOL bVertPartialOK = TRUE, BOOL bHorzPartialOK = TRUE) const;
	BOOL IsItemVisible(HTREEITEM hti, BOOL bVertPartialOK, BOOL bHorzPartialOK, BOOL& bVertVisible, BOOL& bHorzVisible) const;
	void EnsureItemVisible(HTREEITEM hti, BOOL bVertPartialOK = TRUE, BOOL bHorzPartialOK = TRUE);

	void SetItemIntegral(HTREEITEM hti, int iIntegral);
	int GetItemIntegral(HTREEITEM hti) const;
	
	void SetItemChecked(HTREEITEM hti, BOOL bChecked); // 2 state
	void SetItemChecked(HTREEITEM hti, TCH_CHECK nChecked); // 3 state
	TCH_CHECK GetItemCheckState(HTREEITEM hti) const;

	BOOL SelectItem(HTREEITEM hti); // won't auto edit if item already selected
	BOOL IsSelectedItem(HTREEITEM hti) const;
	inline void EndLabelEdit(BOOL bCancel) { SendMessage(m_tree, TVM_ENDEDITLABELNOW, bCancel, 0); }

	BOOL InvalidateItem(HTREEITEM hti, BOOL bChildren = TRUE);
	void GetClientRect(LPRECT lpRect, HTREEITEM htiFrom);
	int GetItemTop(HTREEITEM hti) const;

	int GetItemPos(HTREEITEM hti, HTREEITEM htiParent);
	int GetItemLevel(HTREEITEM hti);

	HTREEITEM FindItem(DWORD dwItemData, HTREEITEM htiStart = NULL) const;
 	const CTreeCtrl& TreeCtrl() const { return m_tree; }

	BOOL IsItemBold(HTREEITEM hti) const;
	void SetItemBold(HTREEITEM hti, BOOL bBold = TRUE);
	void SetTopLevelItemsBold(BOOL bBold = TRUE);

	HTREEITEM GetLastVisibleItem() const;
	HTREEITEM GetLastVisibleChildItem(HTREEITEM hti) const;
	HTREEITEM GetLastChildItem(HTREEITEM hti) const;
	HTREEITEM GetLastItem() const;
	HTREEITEM GetFirstItem() const;
	HTREEITEM GetFirstChildItem(HTREEITEM hti) const;

	void SetItemStateEx(HTREEITEM hti, UINT nState, UINT nMask, BOOL bChildren = FALSE);
	BOOL HasStyle(DWORD dwStyle) const { return (dwStyle != 0) && ((m_tree.GetStyle() & dwStyle) == dwStyle); }

	HTREEITEM GetTopLevelItem(HTREEITEM hti) const;
	HTREEITEM GetNextTopLevelItem(HTREEITEM hti, BOOL bNext = TRUE) const;

	// returns the top level item whose child is the first visible item (or itself)
	HTREEITEM GetFirstVisibleTopLevelItem(int& nPos); // return 0 if no items
	HTREEITEM GetFirstVisibleTopLevelItem(); // return 0 if no items

	int BuildVisibleIndexMap() const;
	BOOL ItemLineIsOdd(HTREEITEM hti) const;
	void ResetIndexMap() const;

	// return increments of item height
	int GetDistanceToEdge(HTREEITEM htiFrom, TCH_EDGE nToEdge) const;

	// returns TRUE if the view moved
	BOOL SetMinDistanceToEdge(HTREEITEM htiFrom, TCH_EDGE nToEdge, int nItems);

	// get next/prev selectable items, NULL if none
	HTREEITEM GetNextPageVisibleItem(HTREEITEM hti) const;
	HTREEITEM GetPrevPageVisibleItem(HTREEITEM hti) const;

	HTREEITEM GetNextVisibleItem(HTREEITEM hti, BOOL bAllowChildren = TRUE) const;
	HTREEITEM GetPrevVisibleItem(HTREEITEM hti, BOOL bAllowChildren = TRUE) const;

	HTREEITEM FindFirstParent() const;
	int CompareItemPositions(HTREEITEM htiFind, HTREEITEM htiStart); // return -1 for above, 1 for below, 0 if same

	HTREEITEM GetNextItem(HTREEITEM hti, BOOL bAllowChildren = TRUE) const; // next item as-if all items were expanded
	HTREEITEM GetPrevItem(HTREEITEM hti, BOOL bAllowChildren = TRUE) const; // prev item as-if all items were expanded


	HTREEITEM MoveTree(HTREEITEM hti, HTREEITEM htiDestParent, HTREEITEM htiDestPrevSibling, 
						BOOL bUsesTextCallback = FALSE, BOOL bUsesImageCallback = FALSE);
	HTREEITEM CopyTree(HTREEITEM hti, HTREEITEM htiDestParent, HTREEITEM htiDestPrevSibling, 
						BOOL bUsesTextCallback = FALSE, BOOL bUsesImageCallback = FALSE);

protected:
	CTreeCtrl& m_tree;
	mutable CMapIndices m_mapVisibleIndices;

protected:
	void AddVisibleItemToIndex(HTREEITEM hti) const;
	void AddItemData(HTREEITEM hti, CDWordArray& aItemData, BOOL bIncParents, BOOL bInCollapsedChildren) const;
	int GetPageCount() const;

	TCH_WHERE GetMoveTarget(HTREEITEM htiDestParent, HTREEITEM htiDestPrevSibling, HTREEITEM& htiTarget) const;
	HTREEITEM CopyTree(HTREEITEM hDest, HTREEITEM hSrc, TCH_WHERE nWhere, BOOL bUsesTextCallback = FALSE, BOOL bUsesImageCallback = FALSE);

	void BuildCopy(const HTREEITEM hti, TCHHCOPY* pCopy) const;
	HTREEITEM CopyTree(HTREEITEM hDest, const TCHHCOPY* pSrc, TCH_WHERE nWhere, BOOL bUsesTextCallback, BOOL bUsesImageCallback);
};

#endif // !defined(AFX_TREECTRLHELPER_H__F6652DBE_3770_4E1C_A246_057AD6AD16B7__INCLUDED_)
