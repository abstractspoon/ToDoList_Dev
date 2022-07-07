// TDCTreeListCtrl.h: interface for the CTDCListListCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCLISTLISTCTRL_H__155791A3_22AC_4083_B933_F39E9EBDADEF__INCLUDED_)
#define AFX_TDCLISTLISTCTRL_H__155791A3_22AC_4083_B933_F39E9EBDADEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

#include "tdcmsg.h"
#include "tdcstruct.h"
#include "todoctrldata.h"
#include "tdltaskctrlbase.h"

/////////////////////////////////////////////////////////////////////////////

#include "..\Shared\EnHeaderCtrl.h"
#include "..\Shared\TreeListSyncer.h"

/////////////////////////////////////////////////////////////////////////////

#include <afxtempl.h>
typedef CArray<int, int> CIntArray;

/////////////////////////////////////////////////////////////////////////////

class CToDoCtrlData;

/////////////////////////////////////////////////////////////////////////////

enum TTC_NEXTTASK
{
	TTCNT_NEXT,
	TTCNT_PREV,
	TTCNT_NEXTVISIBLE,
	TTCNT_PREVVISIBLE,
	TTCNT_NEXTTOPLEVEL,
	TTCNT_PREVTOPLEVEL,
};

/////////////////////////////////////////////////////////////////////////////

class CTDLTaskListCtrl : public CTDLTaskCtrlBase  
{
	DECLARE_DYNAMIC(CTDLTaskListCtrl);
	
public:
	CTDLTaskListCtrl(const CTDCImageList& ilIcons,
					 const CToDoCtrlData& data,
					 const CToDoCtrlFind& find,
					 const CTDCStyleMap& styles,
					 const TDCAUTOLISTDATA& tld,
					 const CTDCColumnIDMap& mapVisibleCols,
					 const CTDCCustomAttribDefinitionArray& aCustAttribDefs);

	virtual ~CTDLTaskListCtrl();
	operator HWND() const { return GetSafeHwnd(); }
	
	inline CListCtrl& List() { return m_lcTasks; }
	inline const CListCtrl& List() const { return m_lcTasks; }
	
	inline const TODOITEM* GetTask(DWORD dwTaskID) const { return m_data.GetTrueTask(dwTaskID); }
	inline const TODOITEM* GetTask(int nItem) const { return GetTask(GetTaskID(nItem)); }
	inline const BOOL HasTask(int nItem) const { return m_data.HasTask(GetTaskID(nItem)); }
	inline UINT GetTaskCount() const { return m_data.GetTaskCount(); }
	inline int GetSelectedCount() const { return m_lcTasks.GetSelectedCount(); }
	inline int GetItemCount() const { return m_lcTasks.GetItemCount(); }
	inline int GetVisibleItemCount() const { return m_lcTasks.GetItemCount(); }
	inline int HitTestItem(POINT point, UINT* pFlags = NULL) const { return m_lcTasks.HitTest(point, pFlags); }

	DWORD GetTaskID(int nItem) const;
	DWORD GetSelectedTaskID() const;
	DWORD GetTrueTaskID(int nItem) const;
	int GetSelectedTaskIDs(CDWordArray& aTaskIDs, BOOL bTrue) const;
	int GetSelectedTaskIDs(CDWordArray& aTaskIDs, DWORD& dwFocusedTaskID) const;
	BOOL SelectTask(DWORD dwTaskID);
	BOOL SelectTasks(const CDWordArray& aTaskIDs);
	int CacheSelection(TDCSELECTIONCACHE& cache, BOOL bIncBreadcrumbs = TRUE) const;
	int RestoreSelection(const TDCSELECTIONCACHE& cache, BOOL bEnsureSelection);
	BOOL IsTaskSelected(DWORD dwTaskID, BOOL bSingly = FALSE) const;
	BOOL EnsureSelectionVisible(BOOL bPartialOK);
	BOOL GetSelectionBoundingRect(CRect& rSelection) const;

	DWORD GetNextTaskID(DWORD dwTaskID, TTC_NEXTTASK nNext, BOOL bExcludeSelected) const;

	// list related
	int GetSelectedItem() const;
	BOOL SelectItem(int nItem);
	BOOL IsItemSelected(int nItem) const;
	BOOL SelectAll();
	BOOL InvalidateSelection(BOOL bUpdate = FALSE);
	BOOL InvalidateItem(int nItem, BOOL bUpdate = FALSE);
	DWORD GetFocusedListTaskID() const;
	int GetFocusedListItem() const;
	int FindTaskItem(DWORD dwTaskID) const;
	int InsertItem(DWORD dwTaskID, int nPos = -1);
	DWORD GetNextSelectedTaskID(POSITION& pos) const;

	BOOL GetLabelEditRect(CRect& rLabel) const;
	void GetWindowRect(CRect& rWindow) const { CWnd::GetWindowRect(rWindow); }
	void DeleteAll();
	void RemoveDeletedItems();
	void SetModified(const CTDCAttributeMap& mapAttribIDs, BOOL bAllowResort);

	BOOL SetGroupBy(TDC_COLUMN nGroupBy, BOOL bSortGroupsAscending = -1);
	BOOL CanGroupBy(TDC_COLUMN nGroupBy, BOOL bCheckVisibility) const;
	UINT GetGroupCount() const;
	void SetSortGroupsAscending(BOOL bAscending = TRUE);
	BOOL TaskHasGroupValue(DWORD dwTaskID) const;
	BOOL IsGroupHeaderTask(DWORD dwTaskID) const;
	BOOL IsGroupHeaderItem(int nItem) const;
	void SetGroupHeaderBackgroundColor(COLORREF color);

	void OnStylesUpdated(const CTDCStyleMap& styles, BOOL bAllowResort);
	void OnBuildComplete();
	void Resort(BOOL bAllowToggle = FALSE);
	BOOL ModsNeedResort(const CTDCAttributeMap& attribIDs) const;

protected:
	CListCtrl m_lcTasks;
	TDC_COLUMN m_nGroupBy;
	BOOL m_bSortGroupsAscending;
	BOOL m_bDeletingGroupHeaders;
	COLORREF m_crGroupHeaderBkgnd;

	typedef CMap<DWORD, DWORD, CString, CString&> CGroupHeaderMap;
	CGroupHeaderMap m_mapGroupHeaders;
	
	mutable TDSORTCOLUMN m_aGroupSortCols[4];
	
protected:
	// Virtual function overrides
	virtual BOOL PrepareSort(TDSORTPARAMS& ss) const;
	virtual int CompareTasks(LPARAM lParam1,
							 LPARAM lParam2,
							 const TDSORTCOLUMN& sort,
							 const TDSORTFLAGS& flags) const;

protected:
	// Message map functions
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()

protected:
	// base-class overrides
	LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT ScWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	
 	LRESULT OnListCustomDraw(NMLVCUSTOMDRAW* pLVCD);
	LRESULT OnListGetDispInfo(NMLVDISPINFO* pLVDI);

	void OnListSelectionChange(NMLISTVIEW* pNMLV);
	void OnNotifySplitterChange(int nSplitPos);

	// pure virtual overrides
	void NotifyParentSelChange(SELCHANGE_ACTION nAction = SC_UNKNOWN);
	BOOL CreateTasksWnd(CWnd* pParentWnd, const CRect& rect, BOOL bVisible);
	BOOL BuildColumns();
	void Release();
	DWORD GetColumnItemTaskID(int nItem) const;
	DWORD HitTestTasksTask(const CPoint& ptScreen) const;
	void SetTasksImageList(HIMAGELIST hil, BOOL bState, BOOL bOn = TRUE);
	HWND Tasks() const { return m_lcTasks; }
	GM_ITEMSTATE GetColumnItemState(int nItem) const;
	void DeselectAll();
	DWORD GetHelpID() const;
	BOOL DoSaveToImage(CBitmap& bmImage, COLORREF crDivider);
	LPCTSTR GetDebugName() const { return _T("ListView"); }

	BOOL GetItemTitleRect(const NMCUSTOMDRAW& nmcd, TDC_LABELRECT nArea, CRect& rect, CDC* pDC = NULL, LPCTSTR szTitle = NULL) const;
	GM_ITEMSTATE GetItemTitleState(const NMCUSTOMDRAW& nmcd) const;
	BOOL IsAlternateTitleLine(const NMCUSTOMDRAW& nmcd) const;

protected:
	BOOL IsListItemSelected(HWND hwnd, int nItem) const;
	BOOL InvalidateTask(DWORD dwTaskID, BOOL bUpdate);
	void GetWindowRect(CRect& rWindow, BOOL bWithHeader) const;
	inline HWND TasksHeader() const { return m_hdrTasks; }
	int GetTopIndex() const;
	BOOL SetTopIndex(int nIndex);
	BOOL NeedDrawTaskSelection() { return (HasFocus() && (GetFocus() != &m_lcTasks)); }
	void SetSelectedTasks(const CDWordArray& aTaskIDs, DWORD dwFocusedTaskID);
	BOOL HandleClientColumnClick(const CPoint& pt, BOOL bDblClk);
	BOOL GetItemTitleRect(int nItem, TDC_LABELRECT nArea, CRect& rect, CDC* pDC = NULL, LPCTSTR szTitle = NULL) const;
	int CalcRequiredTitleColumnWidthForImage();
	GM_ITEMSTATE GetListItemState(int nItem) const;
	BOOL WantNextItem(int nItem, BOOL bTopLevelOnly, BOOL bExcludeSelected) const;

	BOOL UpdateGroupHeaders();
	CString GetTaskGroupValue(DWORD dwTaskID) const;
	CString FormatTaskGroupHeaderText(DWORD dwTaskID) const;
	CString GetGroupByColumnName() const;
	BOOL IsGrouped() const { return (m_nGroupBy != TDCC_NONE); }
	int CalcGroupHeaders(CStringSet& mapNewHeaders, CStringSet& mapOldHeaders, CIntArray& aOldHeaderItems) const;
	void GetGroupHeaderColors(COLORREF& crBack, COLORREF& crText);

	static BOOL HasHitTestFlag(UINT nFlags, UINT nFlag);

};

#endif // !defined(AFX_TDCLISTLISTCTRL_H__155791A3_22AC_4083_B933_F39E9EBDADEF__INCLUDED_)
