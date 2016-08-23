// TreeDragDropHelper.h: interface for the CTreeDragDropHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREEDRAGDROPHELPER_H__06381648_F0F3_4791_8204_6A0A8798F29A__INCLUDED_)
#define AFX_TREEDRAGDROPHELPER_H__06381648_F0F3_4791_8204_6A0A8798F29A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\3rdparty\dragdrop.h"
#include "..\shared\treeselectionhelper.h"

enum DDWHERE
{
	DD_ABOVE = -1,
	DD_ON = 0,
	DD_BELOW = 1
};

class CTreeDragDropHelper
{
public:
	CTreeDragDropHelper(CTreeSelectionHelper& selection, CTreeCtrl& tree);
	virtual ~CTreeDragDropHelper();

	BOOL Initialize(CWnd* pOwner, BOOL bEnabled = TRUE, BOOL bAllowNcDrag = TRUE);
	void EnableDragDrop(BOOL bEnable) { m_bEnabled = bEnable; }
	UINT ProcessMessage(const MSG* pMsg);

	BOOL AddTargetWnd(CWnd* pWnd);
	
	BOOL GetDropTarget(HTREEITEM& htiDrop, HTREEITEM& htiAfter);
	BOOL IsDragging() { return m_ddMgr.IsDragging(); }
	void CancelDrag() { m_ddMgr.CancelDrag(); }

protected:
	CTreeSelectionHelper& m_selection;
	CTreeCtrl& m_tree;
	CDragDropMgr m_ddMgr;
	BOOL m_bEnabled;
	HTREEITEM m_htiDropTarget, m_htiDropAfter;
	UINT m_nScrollTimer, m_nExpandTimer;
	BOOL m_bAllowNcDrag;

	static CTreeDragDropHelper* s_pTDDH;

	struct DROPPOSITION
	{
		HTREEITEM htiDrop;
		DDWHERE nWhere;
	};

	DROPPOSITION m_dropPos;

protected:
	HTREEITEM HitTest(CPoint pt) const { DDWHERE nWhere; return HitTest(pt, nWhere); }
	HTREEITEM HitTest(CPoint pt, DDWHERE& nWhere) const;
	HTREEITEM HighlightDropTarget(CPoint point);
	HTREEITEM HighlightDropTarget(); // get current cursor pos

	enum { TIMER_SCROLL, TIMER_EXPAND };
	void SetTimer(int nTimer, UINT nPeriod);
	static VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);

	// pseudo message handlers
	void OnTimer(UINT nIDEvent);
	BOOL OnDragEnter(DRAGDROPINFO* pDDI); // not const arg because we set pData
	BOOL OnDragPreMove(const DRAGDROPINFO* pDDI);
	UINT OnDragOver(const DRAGDROPINFO* pDDI);
	BOOL OnDragDrop(const DRAGDROPINFO* pDDI);
	BOOL OnDragAbort();
};

#endif // !defined(AFX_TREEDRAGDROPHELPER_H__06381648_F0F3_4791_8204_6A0A8798F29A__INCLUDED_)
