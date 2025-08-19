//
//  CSimpleSplitter
//
//  Splitter window with CWnd-derived panes
//  (C) Robert A. T. Káldy <kaldy@matfyz.cz>
//  last updated on 24.3.2004
//
///////////////////////////////////////////////////////////////////////////////////////

#if !defined(SIMPLE_SPLITTER)
#define SIMPLE_SPLITTER

#include <afxtempl.h>

///////////////////////////////////////////////////////////////////////////////////////

const UINT WM_SS_NOTIFYSPLITCHANGE	= ::RegisterWindowMessage(_T("WM_SS_NOTIFYSPLITCHANGE"));	// wParam = HWND, lParam = LPCRECT relative to parent wnd
const UINT WM_SS_DRAWSPLITBAR		= ::RegisterWindowMessage(_T("WM_SS_DRAWSPLITBAR"));		// wParam = HDC,  lParam = LPCRECT relative to splitter

///////////////////////////////////////////////////////////////////////////////////////

enum SS_ORIENTATION
{
	SSP_HORZ,
	SSP_VERT,
};

///////////////////////////////////////////////////////////////////////////////////////

class CSimpleSplitter : public CWnd
{
public:
	CSimpleSplitter(int nNumPanes = 0, SS_ORIENTATION nOrientation = SSP_HORZ, int nMinSize = 30, int nBarThickness = 3);
	~CSimpleSplitter();

	BOOL Create(CWnd* pParent, UINT nID = AFX_IDW_PANE_FIRST);
	BOOL Create(SS_ORIENTATION nOrientation, CWnd* pParent, UINT nID = AFX_IDW_PANE_FIRST);
	BOOL CreatePane(int nIndex, CWnd* pPaneWnd, DWORD dwStyle, DWORD dwExStyle, LPCTSTR lpszClassName = NULL);
	
	int GetPaneCount() const { return m_aPanes.GetSize(); }
	int FindPane(CWnd* pPaneWnd) const;
	CWnd* GetPane(int nIndex) const;
	CWnd* GetActivePane(int* pIndex) const;

	void GetPaneRect(int nIndex, CRect& rcPane, const CWnd* pWndRelativeTo = NULL) const;
	void GetBarRect(int nIndex, CRect& rcBar, const CWnd* pWndRelativeTo = NULL) const;
	int GetRelativePaneSizes(CArray<int, int&>& aSizes) const;

	void SetPaneCount(int nCount);
	void SetPane(int nIndex, CWnd* pPaneWnd);
	void SetActivePane(int nIndex);
	void SetRelativePaneSizes(const CArray<int, int&>& aSizes, BOOL bRecalcLayout = TRUE);
	void SetRelativePaneSizes(const int sizes[], BOOL bRecalcLayout = TRUE);
	void SetBarColor(COLORREF crBar);

	void ClearPanes();
	void RecalcLayout();
	
	BOOL IsVert() const { return (m_nOrientation == SSP_VERT); }
	BOOL IsHorz() const { return (m_nOrientation == SSP_HORZ); }

protected:
	void InitialisePanes(int nNumPanes);
	void ResizePaneWindow(int nPane);
	void ResizePaneWindows();
	int HitTestBar(CPoint ptClient) const;

	//{{AFX_MSG(CSimpleSplitter)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg LRESULT OnSplitChange(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnDrawSplitBar(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

protected:
	const int m_nMinSize, m_nBarThickness;

	SS_ORIENTATION m_nOrientation;
	int m_nTrackIndex, m_nTracker, m_nTrackerMouseOffset;
	COLORREF m_crBar;
	
	CArray<CWnd*, CWnd*&> m_aPanes;
	CArray<int, int&> m_aPaneSizes, m_aOrgPaneSizes;
};


#endif
