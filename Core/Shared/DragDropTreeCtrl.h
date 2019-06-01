#if !defined(AFX_DRAGDROPTREECTRL_H__63AC05AD_E0DC_11D1_8E53_006008A82731__INCLUDED_)
#define AFX_DRAGDROPTREECTRL_H__63AC05AD_E0DC_11D1_8E53_006008A82731__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DragDropTreeCtrl.h : header file

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CDragDropTreeCtrl window

enum DDTC_MOVE	
{
	MOVE_DOWN,
	MOVE_UP,
	MOVE_LEFT,
	MOVE_RIGHT,
};

static UINT WM_DDTC_ENDDRAG = ::RegisterWindowMessage("DRAGDROPTREECTRL_ENDDRAG");
static UINT WM_DDTC_BEGINDRAG = ::RegisterWindowMessage("DRAGDROPTREECTRL_BEGINDRAG");

class CDragDropTreeCtrl : public CTreeCtrl
{
	// Construction
public:
	CDragDropTreeCtrl();
	
	// Operations
public:
	void EnableDragDrop(BOOL bEnable);
	
	HTREEITEM MoveItem(HTREEITEM hti, DDTC_MOVE nDirection);
	BOOL CanMoveItem(HTREEITEM hti, DDTC_MOVE nDirection) const;

	virtual BOOL IsItemSelected(HTREEITEM hti) const;
	BOOL CopyItem(HTREEITEM hti);
	void ClearCopiedItem() { m_htc.Clear(); }
	BOOL PasteCopiedItem(HTREEITEM htiDest);
	BOOL CanPaste() const { return m_htc.IsValid(); }

	void EndDrag(BOOL bCancel, CPoint* pMouse = NULL, BOOL bCopy = FALSE);
	BOOL IsItemExpanded (HTREEITEM hItem) const;
	BOOL IsChildOf (HTREEITEM hItem1, HTREEITEM hItem2) const;
	BOOL IsDragging() const {return m_bDragging; }

	// because we have another version
	HTREEITEM HitTest(CPoint pt, UINT* pFlags = NULL) { return CTreeCtrl::HitTest(pt, pFlags); }

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDragDropTreeCtrl)
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	// Implementation
public:
	virtual ~CDragDropTreeCtrl();
	
protected:
	const int DELAY_INTERVAL;
	const int SCROLL_MARGIN;
	const int SCROLL_INTERVAL;
	const int EXPAND_INTERVAL;

	BOOL m_bDragging;
	CImageList* m_pImageList;
	HTREEITEM m_hDragItem;
	DWORD m_dwTipStyle; // so we can disable and renenable during dragging

	// Generated message map functions
	//{{AFX_MSG(CDragDropTreeCtrl)
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg BOOL OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	
	DECLARE_MESSAGE_MAP()

protected:
	HTREEITEM HighlightDropTarget (CPoint point);
	BOOL BeginDrag(HTREEITEM hti, CPoint point); // will return if LButton not down

	virtual void OnEndDrag(HTREEITEM hti) {}
	virtual void OnCancelDrag() {}

public:
	// helper for copying
	struct HTREECOPY
	{
		HTREECOPY() { hti = NULL; dwItemData = 0; }
		HTREECOPY(const HTREECOPY& htc) { *this = htc; }

		HTREEITEM hti;
		DWORD dwItemData;
		CArray<HTREECOPY, HTREECOPY&> childItems;

		const HTREECOPY& operator=(const HTREECOPY& htc)
		{
			hti = htc.hti;
			dwItemData = htc.dwItemData;
			childItems.Copy(htc.childItems);

			return *this;
		}

		void Clear()
		{
			dwItemData = 0;
			hti = NULL;
			childItems.RemoveAll();
		}

		BOOL IsValid() const { return (NULL != hti); }
	};

	void BuildCopy(const HTREEITEM hti, HTREECOPY& htc) const;

protected:
	HTREECOPY m_htc;

	enum HTWHERE
	{
		HT_ABOVE = -1,
		HT_ON = 0,
		HT_BELOW = 1
	};

	HTREEITEM HitTest(CPoint pt, HTWHERE& nWhere);
	HTREEITEM CopyTree (HTREEITEM hDest, HTREEITEM hSrc, HTWHERE nWhere);
	HTREEITEM MoveTree (HTREEITEM hDest, HTREEITEM hSrc, HTWHERE nWhere);

	HTREEITEM CopyTree(HTREEITEM hDest, const HTREECOPY& htcSrc, HTWHERE nWhere);

	HTREEITEM MoveItemDown(HTREEITEM hti);
	HTREEITEM MoveItemUp(HTREEITEM hti);
	BOOL CanMoveItemDown(HTREEITEM hti) const;
	BOOL CanMoveItemUp(HTREEITEM hti) const;

	HTREEITEM MoveItemLeft(HTREEITEM hti); // up a level
	HTREEITEM MoveItemRight(HTREEITEM hti); // down a level
	BOOL CanMoveItemLeft(HTREEITEM hti) const; // up a level
	BOOL CanMoveItemRight(HTREEITEM hti) const; // down a level
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately
// before the previous line.

#endif
// !defined(AFX_DRAGDROPTREECTRL_H__63AC05AD_E0DC_11D1_8E53_006008A82731__INCLUDED_)
