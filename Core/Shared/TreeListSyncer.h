#pragma once

#include "subclass.h"

#include <wtypes.h>
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

enum TLS_LINKAGE
{
	TLSL_LEFTDATA_IS_RIGHTITEM,
	TLSL_RIGHTDATA_IS_LEFTITEM,
	TLSL_LEFTDATA_IS_RIGHTDATA,
	TLSL_RIGHTDATA_IS_LEFTDATA,
};

enum TLS_HIDE
{
	TLSH_NONE,
	TLSH_LEFT,
	TLSH_RIGHT,
};

enum
{
	TLSF_SYNCSELECTION		= 0x0001,
	TLSF_SYNCFOCUS			= 0x0002,
	TLSF_SYNCDATA			= 0x0004,
	TLSF_SPLITTER			= 0x0008,
	TLSF_BORDER				= 0x0010,
	TLSF_NOHOLDTREEHSCROLL	= 0x0020,
};

/////////////////////////////////////////////////////////////////////////////

// private enum for type checking
enum TLS_TYPE {	TLST_NONE, TLST_TREE, TLST_LIST, TLST_HEADER };

/////////////////////////////////////////////////////////////////////////////

typedef CArray<HTREEITEM, HTREEITEM> CHTIArray;
typedef CArray<int, int> CIntArray;

/////////////////////////////////////////////////////////////////////////////

typedef int (CALLBACK *PFNTLSCOMPARE)(LPARAM, LPARAM, LPARAM);

/////////////////////////////////////////////////////////////////////////////

class CTreeListSyncer;

class CTLSHoldResync
{
public:
	CTLSHoldResync(CTreeListSyncer& tls);
	~CTLSHoldResync();

protected:
	CTreeListSyncer& m_tls;
	BOOL m_bResyncHeld;
};

/////////////////////////////////////////////////////////////////////////////

class CHoldListVScroll
{
public:
	CHoldListVScroll(HWND hwndList);
	~CHoldListVScroll();
	
protected:
	HWND m_hwndList;
	int m_nOrgVScrollPos;
};

/////////////////////////////////////////////////////////////////////////////

class CHoldHScroll
{
public:
	CHoldHScroll(HWND hwnd, int nInitialPos = -1);
	~CHoldHScroll();

protected:
	HWND m_hwnd;
	int m_nOrgHScrollPos;

private:
	static HWND s_hwndGlobal;

private:
	static int WINAPI MySetScrollPos(HWND hWnd, int nBar, int nPos, BOOL bRedraw);
	static int WINAPI MyScrollWindowEx(HWND hWnd, int dx, int dy, const RECT *prcScroll, const RECT *prcClip, HRGN hrgnUpdate, LPRECT prcUpdate, UINT flags);
};

/////////////////////////////////////////////////////////////////////////////

class CTreeListSyncer : protected CSubclassWnd, protected CSubclasser 
{
	friend class CTLSHoldResync;

public:
	CTreeListSyncer(DWORD dwFlags = TLSF_SYNCSELECTION | TLSF_SYNCFOCUS);
	virtual ~CTreeListSyncer();
	
	BOOL Sync(HWND hwndLeft, HWND hwndRight, TLS_LINKAGE nLink, HWND hwndMainHeader = NULL);
	BOOL IsSyncing() const;
	void Unsync();
	HWND GetHwnd() const { return CSubclassWnd::GetHwnd(); }
	BOOL SwapSides();

	inline TLS_LINKAGE GetLinkage() const { return m_nLinkage; }
	
	void Resize(const CRect& rect);
	void Resize(const CRect& rect, float fLeftProportion);
	void Resize(const CRect& rect, int nLeftWidth);

	virtual void GetBoundingRect(CRect& rect) const;
	virtual BOOL HandleEraseBkgnd(CDC* pDC);

	int GetSplitPos() const { return m_nSplitPos; }
	void SetSplitPos(int nPos);
	int GetSplitBarWidth() const { return m_nSplitBarWidth; }
	void SetSplitBarWidth(int nWidth);
	void SetSplitBarColor(COLORREF crSplitBar);

	BOOL SetHidden(TLS_HIDE nHide);
	TLS_HIDE GetHidden() const { return m_nHidden; }
	BOOL IsHiding() const { return (m_nHidden != TLSH_NONE); }
	BOOL IsHiding(TLS_HIDE nHide) const { return (m_nHidden == nHide); }
	
	void InvalidateAll(BOOL bErase = FALSE, BOOL bUpdate = FALSE);
	void UpdateAll();

	BOOL HasFocus() const;
	void SetFocus();
	void Show(BOOL bShow = TRUE);
	BOOL SaveToImage(CBitmap& bmImage, COLORREF crDivider = CLR_NONE);
	BOOL SaveToImage(CBitmap& bmImage, int nOtherFrom, int nOtherTo, COLORREF crDivider = CLR_NONE);

	HTREEITEM GetTreeSelItem() const;
	void SelectTreeItem(HTREEITEM hti, BOOL bClear = TRUE);

	void Sort(PFNTLSCOMPARE pfnCompare, LPARAM lParamSort, HTREEITEM hti = NULL);
	
protected:
	// all tree/list window hooking gets routed to this function
	LRESULT ScWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT ScDefault(HWND hwnd); // helper
	
	// parent of tree/list gets routed here
	LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
		
protected:
	HWND m_hwndPrimaryHeader;
	TLS_LINKAGE m_nLinkage;
	BOOL m_bTreeExpanding;
	CSubclassWnd m_scLeft, m_scRight;
	DWORD m_dwFlags;
	BOOL m_bSplitting;
	COLORREF m_crSplitBar;
	HIMAGELIST m_hilSize;
	BOOL m_bSavingToImage;

private:
	int m_nSplitPos, m_nSplitBarWidth;
	BOOL m_bNeedInitItemHeight;
	BOOL m_bResyncing, m_bResyncPending, m_bResizePending;
	BOOL m_bResyncEnabled;
	TLS_HIDE m_nHidden;
	HWND m_hwndTrackedHeader;
	int m_nTrackedColumn;
	HWND m_hwndIgnoreNcCalcSize;

	CIntArray m_aListDrawColWidths, m_aListDrawColOrder;

protected:
	inline BOOL CanResync() const { return (m_bResyncEnabled && !m_bResyncing); }
	inline BOOL IsResyncEnabled() const { return m_bResyncEnabled; }
	inline BOOL IsResyncing() const { return m_bResyncing; }

	BOOL Resync(HWND hwnd, HWND hwndTo);
	void PostResync(HWND hwnd);
	void EnableResync(BOOL bEnable, HWND hwnd = NULL);
	void WindowNeedsScrollBars(HWND hwnd, const CRect& rAvail, BOOL& bNeedHScroll, BOOL& bNeedVScroll, BOOL bCheckForLeftListVScroll) const;
	void WindowNeedsScrollBars(HWND hwnd, const CRect& rAvail, const CSize& sizeContent, BOOL& bNeedHScroll, BOOL& bNeedVScroll) const;
	CSize GetContentSize(HWND hwnd, BOOL bAddLeftListVScrollZone) const;
	
#ifdef _DEBUG
	static int GetListSelItem(HWND hwnd, CString& sText);
	static HTREEITEM GetTreeSelItem(HWND hwnd, CString& sText);
	static CString GetTreeItemText(HWND hwndTree, HTREEITEM hti);

	int GetListItem(HWND hwndList, HWND hwndTree, HTREEITEM hti, CString& sText) const;
	HTREEITEM GetTreeItem(HWND hwndTree, HWND hwndList, int nItem, CString& sText) const;

	void TraceListSelection(HWND hwnd, LPCTSTR szLocation = NULL) const;
	void TraceResync(HWND hwnd, HWND hwndTo) const;
#endif
	
	static BOOL IsList(HWND hwnd);
	static BOOL IsTree(HWND hwnd);
	static BOOL IsHeader(HWND hwnd);
	static void ResyncListHeader(HWND hwnd);
	static int GetItemHeight(HWND hwnd);
	static BOOL HasVScrollBar(HWND hwnd);
	static BOOL HasHScrollBar(HWND hwnd);
	static BOOL HasScrollBars(HWND hwnd, BOOL bHScroll, BOOL bVScroll);
	static BOOL CanScroll(HWND hwnd, int nScrollbar, BOOL bLeftUp);
	static DWORD GetTreeItemData(HWND hwnd, HTREEITEM hti);
	static DWORD GetListItemData(HWND hwnd, int nItem);
	static BOOL SetListItemData(HWND hwnd, int nItem, DWORD dwItemData);
	static int GetListSelItem(HWND hwnd);
	static int GetListFocusItem(HWND hwnd);
	static HTREEITEM GetTreeSelItem(HWND hwnd);
	static HTREEITEM GetTreeFocusItem(HWND hwnd);
	static HTREEITEM FindTreeItem(HWND hwndTree, DWORD dwItemData);
	static HTREEITEM FindTreeItem(HWND hwndTree, HTREEITEM hti, DWORD dwItemData);
	static HTREEITEM FindVisibleTreeItem(HWND hwndTree, DWORD dwItemData);
	static int FindListItem(HWND hwnd, DWORD dwItemData);
	static BOOL IsTreeFullRowSelect(HWND hwnd);
	static BOOL IsListFullRowSelect(HWND hwnd);
	static BOOL IsTreeItemExpanded(HWND hwnd, HTREEITEM hti);
	static BOOL IsTreeItemVisible(HWND hwnd, HTREEITEM hti);
	static BOOL TreeItemHasState(HWND hwnd, HTREEITEM hti, UINT nStateMask);
	static void SetTreeItemState(HWND hwnd, HTREEITEM hti, UINT nState, UINT nStateMask);
	static BOOL ListItemHasState(HWND hwnd, int nItem, UINT nStateMask);
	static void InvalidateTreeItem(HWND hwnd, HTREEITEM hti);
	static void InvalidateListItem(HWND hwnd, int nItem);
	static int InsertListItem(HWND hwndList, int nInsertPos, DWORD dwItemData);
 	static void SelectTreeItem(HWND hwnd, HTREEITEM hti, BOOL bClear = TRUE);
	static BOOL OsIsXP();
	static BOOL OsIsLinux();
	static int CalcTotalHeaderItemWidth(HWND hwndHeader);
	static int CalcMaxVisibleTreeItemWidth(HWND hwnd);

	static DWORD GetStyle(HWND hwnd, BOOL bExStyle);
	static BOOL HasStyle(HWND hwnd, DWORD dwStyle, BOOL bExStyle);
	static BOOL HasFocus(HWND hwnd);
	static BOOL ModifyStyle(HWND hwnd, DWORD dwRemove, DWORD dwAdd, BOOL bExStyle);
	static BOOL SetStyle(HWND hwnd, DWORD dwStyle, BOOL bExStyle);
	
	HWND GetTree() const;

	inline HWND Left() const { return m_scLeft.GetHwnd(); }
	inline HWND Right() const { return m_scRight.GetHwnd(); }
	inline HWND OtherWnd(HWND hwnd) const { ASSERT(IsLeft(hwnd) || IsRight(hwnd)); return (IsLeft(hwnd) ? Right() : Left()); }
	inline BOOL IsLeft(HWND hwnd) const { return ((hwnd == Left()) ? TRUE : FALSE); }
	inline BOOL IsRight(HWND hwnd) const { return ((hwnd == Right()) ? TRUE : FALSE); }
	inline BOOL IsPrimaryHeader(HWND hwnd) const { return (m_hwndPrimaryHeader == hwnd); }
	inline BOOL IsHeaderTracking() const { return (m_hwndTrackedHeader != NULL); }
		
	void Resize(const CRect& rLeft, const CRect& rRight);
	void RefreshSize();
	void PostResize(BOOL bForce = FALSE);
	BOOL IsHiding(HWND hwnd) const;

	virtual BOOL ResyncScrollPos(HWND hwnd, HWND hwndTo);
	virtual BOOL ResyncSelection(HWND hwnd, HWND hwndTo, BOOL bClearTreeSel = TRUE);
	void PostResync(HWND hwnd, BOOL bIncSelection);

	int GetListItem(HWND hwndList, HWND hwndTree, HTREEITEM hti) const;
	int GetListItem(HWND hwndList1, HWND hwndList2, int nItem2) const;
	HTREEITEM GetTreeItem(HWND hwndTree, HWND hwndList, int nItem) const;
	BOOL ShowVScrollBar(HWND hwnd, BOOL bShow = TRUE, BOOL bRefreshSize = TRUE);
	BOOL HasVScrollBar() const;
	void FixupLeftVScrollbar(BOOL bRightHadVScroll = -1, BOOL bRefreshSize = TRUE);
	void FixupLeftVScrollbar(HWND hwndLeft, BOOL bRightHadVScroll, BOOL bRefreshSize);
	BOOL HasFlag(DWORD dwFlag) const { return ((m_dwFlags & dwFlag) == dwFlag); }
	void InitItemHeights();
	void RecalcItemHeights();
	DWORD GetRequiredLinkData(HWND hwndList, HWND hwndTree, HTREEITEM hti);
	DWORD GetRequiredLinkData(HWND hwndList1, HWND hwndList2, int nItem);
	void InitialiseStyles(HWND hwndLeft, HWND hwndRight);
	HWND PrimaryWnd() const;
	int GetSelectedListItems(HWND hwndList, CIntArray& aItems);
	BOOL ResyncListToTreeSelection(HWND hwndTree, const CList<HTREEITEM, HTREEITEM>& htItems, HTREEITEM htiFocused);
	BOOL WantHoldHScroll(HWND hWnd) const;
	BOOL IsHeaderTracking(HWND hwndHeader, int nCol = -1) const;
	HWND HitTestHeader(const CPoint& ptScreen, int& nCol) const;

	void ExpandList(HWND hwndList, HWND hwndTree, HTREEITEM hti, int& nNextIndex);
	void CollapseList(HWND hwndList, HWND hwndTree, HTREEITEM hti);
	void ExpandList(HWND hwndList, HWND hwndTree);
	void CollapseList(HWND hwndList, HWND hwndTree);

	void GetBoundingRect(HWND hwnd, CRect& rect) const;
	BOOL GetHeaderRect(HWND hwnd, CRect& rect, LPCRECT prcBounds) const;
	BOOL GetSplitterRect(CRect& rSplitter) const;
	BOOL GetHScrollDeadSpace(CRect& rDead) const;
	void AdjustForBorder(CRect& rLeft, CRect& rRight) const;
	BOOL PtInSplitter(const CPoint& pt, BOOL bScreen = FALSE) const;
	BOOL IsSplitting() { return m_bSplitting; }
	BOOL CheckBottomAlignment() const;
	int GetBoundingWidth() const;
	
	// callbacks for derived classes
	virtual BOOL IsTreeItemSelected(HWND hwnd, HTREEITEM hti) const;
	virtual BOOL IsListItemSelected(HWND hwnd, int nItem) const;
	virtual void OnNotifySplitterChange(int nSplitPos);
	virtual void DrawSplitBar(CDC* pDC, const CRect& rSplitter, COLORREF crSplitBar);
	virtual void HandleItemExpanded(HWND hwndTree, HTREEITEM hti, BOOL bExpand);

	// pseudo message handlers
	virtual LRESULT OnTreeCustomDraw(NMTVCUSTOMDRAW* pTVCD);
	virtual LRESULT OnListCustomDraw(NMLVCUSTOMDRAW* pLVCD, const CIntArray& aColOrder, const CIntArray& aColWidths);
	virtual LRESULT OnHeaderCustomDraw(NMCUSTOMDRAW* pNMCD);

	virtual LRESULT OnListGetDispInfo(NMLVDISPINFO* pLVDI);
	virtual LRESULT OnTreeGetDispInfo(NMTVDISPINFO* pTVDI);

	virtual void OnListSelectionChange(NMLISTVIEW* pNMLV);
	virtual void OnTreeSelectionChange(NMTREEVIEW* pNMTV);

	virtual BOOL OnHeaderItemWidthChanging(NMHEADER* pHDN, int nMinWidth = 0);

	virtual BOOL OnListHeaderBeginTracking(NMHEADER* /*pHDN*/) { return TRUE; }
	virtual BOOL OnListHeaderTrackItem(NMHEADER* pHDN, int nMinWidth = 0);
	virtual void OnListHeaderEndTracking(NMHEADER* pHDN);
	virtual BOOL OnPrimaryHeaderBeginTracking(NMHEADER* /*pHDN*/) { return TRUE; }
	virtual BOOL OnPrimaryHeaderTrackItem(NMHEADER* pHDN, int nMinWidth = 0);
	virtual void OnPrimaryHeaderEndTracking(NMHEADER* pHDN);

private:
	static TLS_TYPE GetType(HWND hwnd);
	static void RemoveType(HWND hwnd);

	void PreDetachWindow();

	// for sorting list from tree
	typedef CMap<DWORD, DWORD, int, int> CSortMap;
	
	void BuildTreeListSortMap(HWND hwndTree, HWND hwndList, CSortMap& map);
	void BuildTreeListSortMap(HWND hwndTree, HWND hwndList, HTREEITEM hti, CSortMap& map, int& nIndex);
	void SortTreeItem(HWND hwndTree, HTREEITEM hti, PFNTLSCOMPARE pfnCompare, LPARAM lParamSort, BOOL bRecursive);
	void BuildListListSortMap(HWND hwndPrimary, HWND hwndList, CSortMap& map);
	BOOL HandleMouseWheel(HWND hWnd, WPARAM wp, LPARAM lp);
	void FixupListListItemIsDataLinkage(int nFrom = 0);
	void RefreshListDrawColAttributes(HWND hwndList);

	static int CALLBACK SortListProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static BOOL ConvertNonClientToClientMouseMsg(HWND hWnd, UINT& nMsg, WPARAM& wParam, LPARAM& lParam);
	static BOOL SaveToImage(HWND hWnd, CBitmap& bmImage, const CRect& rFromTo = CRect(0, 0, -1, -1));
};
