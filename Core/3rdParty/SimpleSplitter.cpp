//
//  CSimpleSplitter
//
//  Splitter window with CWnd-derived panes
//  (C) Robert A. T. K�ldy <kaldy@matfyz.cz>
//  last updated on 24.3.2004

#include "stdafx.h"
#include "SimpleSplitter.h"


#define FULL_SIZE 32768


inline int MulDivRound(int x, int mul, int div)
{
	return (x * mul + div / 2) / div;
}


BEGIN_MESSAGE_MAP(CSimpleSplitter, CWnd)
	//{{AFX_MSG_MAP(CSimpleSplitter)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_NCCREATE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_REGISTERED_MESSAGE(WM_SS_DRAWSPLITBAR, OnDrawSplitBar)
	ON_REGISTERED_MESSAGE(WM_SS_NOTIFYSPLITCHANGE, OnSplitChange)
END_MESSAGE_MAP()

CSimpleSplitter::CSimpleSplitter(int nNumPanes, SS_ORIENTATION nOrientation, int nMinSize, int nBarThickness)
	:
	m_nOrientation(nOrientation),
	m_nMinSize(nMinSize), 
	m_nBarThickness(nBarThickness),
	m_crBkgnd(::GetSysColor(COLOR_BTNFACE)),
	m_crBar(::GetSysColor(COLOR_BTNFACE))
{
	ASSERT(nNumPanes >= 0);
	ASSERT(nOrientation == SSP_HORZ || nOrientation == SSP_VERT);
	ASSERT(nMinSize >= 0);
	ASSERT(nBarThickness >= 0);
		
	InitialisePanes(nNumPanes);
}

CSimpleSplitter::~CSimpleSplitter()
{
}

void CSimpleSplitter::InitialisePanes(int nNumPanes)
{
	m_aPanes.SetSize(nNumPanes);
	m_aPaneSizes.SetSize(nNumPanes);
	m_aOrgPaneSizes.SetSize(nNumPanes + 1);
	
	if (nNumPanes > 0)
	{
		int total = 0;
		for (int i = 0; i < nNumPanes - 1; i++)			// default, set equal size to all panes
		{
			m_aPaneSizes[i] = ((FULL_SIZE + (nNumPanes / 2)) / nNumPanes);
			total += m_aPaneSizes[i];
		}
		m_aPaneSizes[nNumPanes - 1] = FULL_SIZE - total;
	}
	else
	{
		m_aOrgPaneSizes[0] = 0;
	}
}

BOOL CSimpleSplitter::Create(CWnd* pParent, UINT nID)
{
	return Create(m_nOrientation, pParent, nID);
}

BOOL CSimpleSplitter::Create(SS_ORIENTATION nOrientation, CWnd* pParent, UINT nID)
{
	ASSERT(pParent);

	m_nOrientation = nOrientation;

	HCURSOR crsResize = AfxGetApp()->LoadCursor(IsHorz() ? AFX_IDC_HSPLITBAR : AFX_IDC_VSPLITBAR);

	if (crsResize == NULL)
		crsResize = ::LoadCursor(0, IsHorz() ? IDC_SIZEWE : IDC_SIZENS);

	CRect rClient;
	pParent->GetClientRect(rClient);

	return CreateEx(0, AfxRegisterWndClass(CS_DBLCLKS, crsResize, NULL, NULL), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rClient, pParent, nID, NULL);
}

BOOL CSimpleSplitter::CreatePane(int nIndex, CWnd* pPaneWnd, DWORD dwStyle, DWORD dwExStyle, LPCTSTR lpszClassName)
{
	ASSERT((nIndex >= 0) && (nIndex < m_aPanes.GetSize()));

	CRect rPane;
	GetPaneRect(nIndex, rPane);
	
	dwStyle |= (WS_CHILD | WS_VISIBLE);

	if (!pPaneWnd->CreateEx(dwExStyle, lpszClassName, NULL, dwStyle, rPane, this, AFX_IDW_PANE_FIRST + nIndex))
		return FALSE;

	m_aPanes[nIndex] = pPaneWnd;
	return TRUE;
}

void CSimpleSplitter::SetPaneCount(int nCount)
{
	InitialisePanes(nCount);
}

void CSimpleSplitter::ClearPanes()
{
	InitialisePanes(0);
}

void CSimpleSplitter::SetPane(int nIndex, CWnd* pPaneWnd)
{
	ASSERT((nIndex >= 0) && (nIndex < m_aPanes.GetSize()));
	ASSERT(!pPaneWnd || ::IsWindow(pPaneWnd->m_hWnd));

	m_aPanes[nIndex] = pPaneWnd;
	
	ResizePaneWindow(nIndex);
}

CWnd* CSimpleSplitter::GetPane(int nIndex) const
{
	ASSERT((nIndex >= 0) && (nIndex < m_aPanes.GetSize()));
	return m_aPanes[nIndex];
}

void CSimpleSplitter::SetActivePane(int nIndex)
{
	ASSERT((nIndex >= 0) && (nIndex < m_aPanes.GetSize()));
	m_aPanes[nIndex]->SetFocus();
}

CWnd* CSimpleSplitter::GetActivePane(int* pIndex) const
{
	for (int i = 0; i < m_aPanes.GetSize(); i++)
	{
		if (CWnd::GetFocus() == m_aPanes[i])
		{
			*pIndex = i;
			return m_aPanes[i];
		}
	}
	return NULL;
}

void CSimpleSplitter::SetRelativePaneSizes(const int sizes[])
{
	int i, total = 0, total_in = 0;
	
	for (i = 0; i < m_aPanes.GetSize(); i++)
	{
		ASSERT(sizes[i] >= 0);
		total += sizes[i];
	}
	for (i = 0; i < m_aPanes.GetSize() - 1; i++)
	{
		m_aPaneSizes[i] = MulDivRound(sizes[i], FULL_SIZE, total);
		total_in += m_aPaneSizes[i];
	}
	m_aPaneSizes[m_aPanes.GetSize() - 1] = FULL_SIZE - total_in;
	
	RecalcLayout();
}

void CSimpleSplitter::SetRelativePaneSizes(const CArray<int, int&>& aSizes)
{
	ASSERT(aSizes.GetSize() == m_aPaneSizes.GetSize());

	SetRelativePaneSizes(aSizes.GetData());
}

void CSimpleSplitter::GetPaneRect(int nIndex, CRect& rPane, const CWnd* pWndRelativeTo) const
{
	ASSERT(nIndex >= 0 && nIndex < m_aPanes.GetSize());
	GetClientRect(&rPane);

	if (IsHorz())
	{
		rPane.left = m_aOrgPaneSizes[nIndex];
		rPane.right = m_aOrgPaneSizes[nIndex + 1] - m_nBarThickness;
	}
	else
	{
		rPane.top = m_aOrgPaneSizes[nIndex];
		rPane.bottom = m_aOrgPaneSizes[nIndex + 1] - m_nBarThickness;
	}

	if (pWndRelativeTo)
	{
		ClientToScreen(rPane);
		pWndRelativeTo->ScreenToClient(rPane);
	}
}

void CSimpleSplitter::GetBarRect(int nIndex, CRect& rBar, const CWnd* pWndRelativeTo) const
{
	ASSERT(nIndex >= 0 && nIndex < m_aPanes.GetSize());
	GetPaneRect(nIndex, rBar, pWndRelativeTo);

	if (IsHorz())
	{
		rBar.left = rBar.right;
		rBar.right += m_nBarThickness;
	}
	else
	{
		rBar.top = rBar.bottom;
		rBar.bottom += m_nBarThickness;
	}
}

void CSimpleSplitter::SetColors(COLORREF crBkgnd, COLORREF crBar)
{
	if ((crBkgnd != m_crBkgnd) || (crBar != m_crBar))
	{
		m_crBkgnd = crBkgnd;
		m_crBar = crBar;

		if (GetSafeHwnd())
			Invalidate();
	}
}

// CSimpleSplitter protected

void CSimpleSplitter::RecalcLayout()
{
	int i, size_sum, remain, remain_new = 0;
	bool bGrow = true;
	CRect rClient;

	GetClientRect(rClient);
	size_sum = IsHorz() ? rClient.Width() : rClient.Height();
	size_sum -= (m_aPanes.GetSize() - 1) * m_nBarThickness;

	// adjust sizes on the beginning
	// and while we have grow-ed something	
	while (bGrow)									
	{												
		bGrow = false;
		remain = remain_new = FULL_SIZE;

		// grow small panes to minimal size
		for (i = 0; i < m_aPanes.GetSize(); i++)				
		{
			if (MulDivRound(m_aPaneSizes[i], size_sum, FULL_SIZE) <= m_nMinSize)
			{
				remain -= m_aPaneSizes[i];
				if (MulDivRound(m_aPaneSizes[i], size_sum, FULL_SIZE) < m_nMinSize)
				{
					if (m_nMinSize > size_sum)
						m_aPaneSizes[i] = FULL_SIZE;
					else
						m_aPaneSizes[i] = MulDivRound(m_nMinSize, FULL_SIZE, size_sum);
					bGrow = true;
				}
				remain_new -= m_aPaneSizes[i];
			}
		}
		
		// if there isn't place to all panes
		// set the minimal size to the leftmost/topmost							
		// and set zero size to the remaining		
		if (remain_new <= 0)		
		{										
			remain = FULL_SIZE;					
		
			for (i = 0; i < m_aPanes.GetSize(); i++)
			{
				if (size_sum == 0)
					m_aPaneSizes[i] = 0;
				else
					m_aPaneSizes[i] = MulDivRound(m_nMinSize, FULL_SIZE, size_sum);
			
				if (m_aPaneSizes[i] > remain)
					m_aPaneSizes[i] = remain;
				
				remain -= m_aPaneSizes[i];
			}
			break;
		}
		
		// adjust other pane sizes, if we have growed some
		if (remain_new != FULL_SIZE)				
		{
			for (i = 0; i < m_aPanes.GetSize(); i++)
			{
				if (MulDivRound(m_aPaneSizes[i], size_sum, FULL_SIZE) != m_nMinSize)
					m_aPaneSizes[i] = MulDivRound(m_aPaneSizes[i], remain_new, remain);
			}
		}
	}

	// calculate positions (in pixels) from relative sizes
	m_aOrgPaneSizes[0] = 0;									

	for (i = 0; i < m_aPanes.GetSize() - 1; i++)
		m_aOrgPaneSizes[i + 1] = m_aOrgPaneSizes[i] + MulDivRound(m_aPaneSizes[i], size_sum, FULL_SIZE) + m_nBarThickness;

	m_aOrgPaneSizes[m_aPanes.GetSize()] = size_sum + m_nBarThickness * m_aPanes.GetSize();

	ResizePaneWindows();
}

void CSimpleSplitter::ResizePaneWindows()
{
	CRect rClient;
	GetClientRect(rClient);

	for (int i = 0; i < m_aPanes.GetSize(); i++)
		ResizePaneWindow(i);

	Invalidate(TRUE);
}

void CSimpleSplitter::ResizePaneWindow(int nPane)
{
	if (m_aPanes[nPane])
	{
		CRect rPane;
		GetPaneRect(nPane, rPane, m_aPanes[nPane]->GetParent());

		m_aPanes[nPane]->MoveWindow(rPane);
		m_aPanes[nPane]->UpdateWindow();
	}
	else
	{
		GetParent()->SendMessage(WM_SS_NOTIFYSPLITCHANGE, (WPARAM)GetSafeHwnd(), nPane);
	}
}

// CSimpleSplitter messages
LRESULT CSimpleSplitter::OnSplitChange(WPARAM wp, LPARAM lp)
{
	// Forward on
	return GetParent()->SendMessage(WM_SS_NOTIFYSPLITCHANGE, wp, lp);
}

LRESULT CSimpleSplitter::OnDrawSplitBar(WPARAM wp, LPARAM lp)
{
	// Forward on
	return GetParent()->SendMessage(WM_SS_DRAWSPLITBAR, wp, lp);
}

BOOL CSimpleSplitter::OnEraseBkgnd(CDC* pDC)
{
	if (m_crBkgnd != CLR_NONE)
	{
		// Clip out all pane rects having an attached CWnd*
		for (int i = 0; i < m_aPanes.GetSize(); i++)
		{
			if (m_aPanes[i])
			{
				CRect rPane;
				GetPaneRect(i, rPane);

				pDC->ExcludeClipRect(rPane);
			}

			// And any bar rects if the bar colour varies from the background colour
	// 		if (m_crBar != m_crBkgnd)
	// 		{
	// 			CRect rBar;
	// 			GetBarRect(i, rBar);
	// 
	// 			pDC->ExcludeClipRect(rBar);
	// 		}
		}

		CRect rClient;
		GetClientRect(rClient);

		pDC->FillSolidRect(rClient, m_crBkgnd);
		return TRUE;
	}

	// else
	return CWnd::OnEraseBkgnd(pDC);
}

void CSimpleSplitter::OnPaint() 
{
	if (GetPaneCount() && (m_crBar != CLR_NONE) && (m_crBar != m_crBkgnd))
	{
		CPaintDC dc(this);

		// Draw the bars
		for (int i = 0; i < m_aPanes.GetSize() - 1; i++)
		{
			CRect rBar;
			GetBarRect(i, rBar);

			if (GetParent()->SendMessage(WM_SS_DRAWSPLITBAR, (WPARAM)dc.GetSafeHdc(), i) == 0)
			{
				dc.FillSolidRect(rBar, m_crBar);
			}
		}
	}
	else
	{
		Default();
	}
}

void CSimpleSplitter::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	RecalcLayout();
}

void CSimpleSplitter::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);

	if (GetPaneCount())
	{
		int mouse_pos = (IsHorz() ? point.x : point.y);
		
		for (m_nTrackIndex = 1; m_nTrackIndex < m_aPanes.GetSize(); m_nTrackIndex++)
		{
			CRect rBar;
			GetBarRect(m_nTrackIndex - 1, rBar);

			if (rBar.PtInRect(point))
			{
				SetCapture();
				break;
			}
		}
		
		m_nTracker = m_aOrgPaneSizes[m_nTrackIndex];
		m_nTrackerMouseOffset = mouse_pos - m_nTracker;
		
		CRect rcClient;
		GetClientRect(&rcClient);
		
		m_nTrackerLength = IsHorz() ? rcClient.Height() : rcClient.Width();
	}
}

void CSimpleSplitter::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);

	if (GetCapture() != this) 
		return;
	
	ASSERT(GetPaneCount());

	ReleaseCapture();
	Invalidate(FALSE);
}

void CSimpleSplitter::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);
	
	if (GetCapture() != this) 
		return;

	ASSERT(GetPaneCount());

	m_nTracker = (IsHorz() ? point.x : point.y) - m_nTrackerMouseOffset;
	
	m_nTracker = min(m_nTracker, m_aOrgPaneSizes[m_nTrackIndex + 1] - m_nBarThickness - m_nMinSize);
	m_nTracker = max(m_nTracker, m_aOrgPaneSizes[m_nTrackIndex - 1] + m_nBarThickness + m_nMinSize);

	CRect rClient;
	int size_sum;
	
	GetClientRect(rClient);
	size_sum = IsHorz() ? rClient.Width() : rClient.Height();
	size_sum -= (m_aPanes.GetSize() - 1) * m_nBarThickness;
	
	m_aOrgPaneSizes[m_nTrackIndex] = m_nTracker;
	m_aPaneSizes[m_nTrackIndex - 1] = MulDivRound(m_aOrgPaneSizes[m_nTrackIndex] - m_aOrgPaneSizes[m_nTrackIndex - 1] - m_nBarThickness, FULL_SIZE, size_sum);
	m_aPaneSizes[m_nTrackIndex]     = MulDivRound(m_aOrgPaneSizes[m_nTrackIndex + 1] - m_aOrgPaneSizes[m_nTrackIndex] - m_nBarThickness, FULL_SIZE, size_sum);

	ResizePaneWindows();
	UpdateWindow();
}

BOOL CSimpleSplitter::OnNcCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (!CWnd::OnNcCreate(lpCreateStruct))
		return FALSE;

	CWnd* pParent = GetParent();
	ASSERT_VALID(pParent);
	pParent->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME);

	return TRUE;
}

void CSimpleSplitter::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	lpwndpos->flags |= SWP_NOCOPYBITS;
	CWnd::OnWindowPosChanging(lpwndpos);
}
