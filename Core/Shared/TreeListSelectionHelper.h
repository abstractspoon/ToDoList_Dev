// TreeListSelectionHelper.h: interface for the CTreeListSelectionHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREELISTSELECTIONHELPER_H__098294B4_8B41_4369_8522_FE1637BA7EA1__INCLUDED_)
#define AFX_TREELISTSELECTIONHELPER_H__098294B4_8B41_4369_8522_FE1637BA7EA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TreeSelectionHelper.h"

//////////////////////////////////////////////////////////////////////

class CTreeListSelectionHelper : public CTreeSelectionHelper
{
public:
	CTreeListSelectionHelper(CTreeCtrl& tree, CListCtrl& list);
	virtual ~CTreeListSelectionHelper();

	void DeselectAll();

	virtual BOOL HasFocus() const;

	// Pseudo List message handlers
	void OnListLButtonDown(WPARAM wp, LPARAM lp, BOOL& bSelChange);
	void OnListRButtonDown(WPARAM wp, LPARAM lp, BOOL& bSelChange);
	void OnListNotifyParentSelChange(NMLISTVIEW* pNMLV, BOOL& bSelChange);

	// Base class tree overrides
	virtual void OnTreeLButtonDown(WPARAM wp, LPARAM lp, BOOL& bSelChange);
	virtual void OnTreeRButtonDown(WPARAM wp, LPARAM lp, BOOL& bSelChange);

	virtual void OnTreeKeyDown(WPARAM wp, LPARAM lp, BOOL& bSelChange);
	virtual void OnTreeKeyUp(WPARAM wp, LPARAM lp, BOOL& bSelChange);
	virtual void OnTreeNotifyParentKeyDown(NMTVKEYDOWN* pTVKD);
	virtual void OnTreeNotifyParentSelChange(NMTREEVIEW* pNMTV, BOOL& bSelChange);

protected:
	CListCtrl& m_list;

protected:
	int GetListItem(HTREEITEM hti) const;
	HTREEITEM GetTreeItem(int nItem) const;
};

#endif // !defined(AFX_TREELISTSELECTIONHELPER_H__098294B4_8B41_4369_8522_FE1637BA7EA1__INCLUDED_)
