#if !defined(AFX_TEXTDIAGRAMCTRL_H__DB16232E_C9BD_483B_86CA_5858C77045FC__INCLUDED_)
#define AFX_TEXTDIAGRAMCTRL_H__DB16232E_C9BD_483B_86CA_5858C77045FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextDiagramCtrl.h : header file
//

#include "textdiagram.h"
#include "maskedit.h"

/////////////////////////////////////////////////////////////////////////////
// CTextDiagramCtrl window

// WM_NOTIFY codes
enum 
{
	TDN_SELCHANGE = 1,
	TDN_SELRECTMOVE,
	TDN_SELRECTDELETE,
	TDN_SELRECTSIZE,
	TDN_DBLCLK,
	TDN_SELRECTTEXTCHANGE,
	TDN_IMPLCONNNEW, // host must implement this functionality

	TDN_INTERNALERROR,
};

struct TDNHDR
{
	NMHDR nmhdr;
	POINT ptCursor; // client coords
};

//{{DIAGRAM_START
//                                     
//           ----------------          
//          |CTextDiagramCtrl|         
//          |                |         
//           #--------------#          
//           |              |          
//   --------*-----     ----*--------- 
//  |CWnd          |   |CTextDiagram  |
//  |              |   |              |
//  |              |   |              |
//  |              |   |              |
//  |              |   |              |
//   --------------     -------------- 
//
//}}DIAGRAM_END
class CTextDiagramCtrl : public CWnd, public CTextDiagram
{
// Construction
public:
	CTextDiagramCtrl(BOOL bWarnOnDeletion = TRUE);
	virtual ~CTextDiagramCtrl();

// Operations
public:
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParent, UINT nID);

	void SetDiagram(const CTextDiagram& diagram);
	void SetDiagram(const CStringArray& diagram);
	void SetDiagram(LPCTSTR szDiagram);
	BOOL ResetDiagram();

	int AddRect(LPCRECT pRect, LPCTSTR szText, BOOL bEditText);
	BOOL DeleteRect(int nRect);
	BOOL SetRect(int nRect, const CTDRect& rect);
	BOOL SetText(int nRect, LPCTSTR szText);

	void EditSelectedText(BOOL bResize = FALSE);
	inline BOOL EditingText() { return (m_editText.GetSafeHwnd() && m_editText.IsWindowVisible()); }

	int AddConnection(int nRectFrom, int nRectTo, int nSideFrom);
	BOOL DeleteConnection(int nConn);
	BOOL SetConnection(int nConn, const CTDConnection& conn);

	int GetSelRect() { return m_nSelRect; }
	int HitTestConn(CPoint point); // client coords
	int HitTestRect(CPoint point); // client coords

	void SetPageWidth(int nPageWidth, BOOL bRedraw = TRUE);

protected:
	int m_nSelRect;
	CPoint m_ptScrollOffset; // for scrolling
	BOOL m_bMoving;
	int m_nSizing;
	CPoint m_ptDragStart, m_ptDragPrev;
	CMaskEdit m_editText;
	BOOL m_bDblClick;
	CPoint m_ptContextMenu; // client coords
	BOOL m_bResizeOnEdit;
	BOOL m_bWarn;
	int m_nPageWidth;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextDiagramCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CTextDiagramCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	afx_msg void OnEndTextEdit();
	afx_msg void OnChangeText();
	afx_msg void OnRectNew();
	afx_msg void OnRectDelete();
	afx_msg void OnRectEditText();
	afx_msg void OnConnNew();
	afx_msg void OnConnModify();
	afx_msg void OnConnFlip();
	afx_msg void OnConnDelete();
	afx_msg void OnReset();
	DECLARE_MESSAGE_MAP()

protected:
	void DrawArrow(CDC* pDC, CPoint point, int nDirection);
	void DrawRect(CDC* pDC, const CTDRect& rect, BOOL bSelected);
	void DrawConn(CDC* pDC, const CTDConnection& conn);
	void DrawGrid(CDC* pDC);
	void RenderText(CDC* pDC, const CRect& rect, const CString& sText);

	void UpdateScrollbars();
	CRect GetBoundingRect();
	BOOL RebuildConnections();
	void SnapToGrid(CRect& rect);
	void ClientToLogical(CPoint& point);
	void LogicalToClient(CRect& rect);
	CRect GetVisiblePageRect();
	void EndTextEdit(BOOL bCancel);
	void EnsureVisible(int nRect);
	void SendNotification(UINT nCode, CPoint point = CPoint(-1, -1));
	int AddRect(LPCRECT pRect = NULL, LPCTSTR szText = NULL);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTDIAGRAMCTRL_H__DB16232E_C9BD_483B_86CA_5858C77045FC__INCLUDED_)
