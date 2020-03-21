// RangeSliderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "RangeSliderCtrl.h"

#include "Themed.h"
#include "Misc.h"
#include "GraphicsMisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRangeSliderCtrl

CRangeSliderCtrl::CRangeSliderCtrl(UINT nThumbStyle) 
	: 
	m_crParentBkgnd(CLR_NONE), 
	m_nSliderDrawStyles(nThumbStyle),
	m_dMinRangeWidth(0.0),
	m_dMaxRangeWidth(-1.0),
	m_nMinTickSpacing(3)
{
}

CRangeSliderCtrl::~CRangeSliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CRangeSliderCtrl, CRangeSlider)
	//{{AFX_MSG_MAP(CRangeSliderCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(WM_HTHOTCHANGE, OnHotChange)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRangeSliderCtrl message handlers

BOOL CRangeSliderCtrl::SetMinMaxRangeWidths(double dMinWidth, double dMaxWidth)
{
	if (dMinWidth < 0.0)
	{
		ASSERT(0);
		return FALSE;
	}
		
	if (dMaxWidth != -1)
	{
		if ((dMaxWidth < 0.0) || (dMaxWidth < dMinWidth))
		{
			ASSERT(0);
			return FALSE;
		}
	}

	m_dMinRangeWidth = dMinWidth;
	m_dMaxRangeWidth = dMaxWidth;

	return TRUE;
}

BOOL CRangeSliderCtrl::SetMinTickSpacing(int nPixels)
{
	if (nPixels < 3 || nPixels > 100)
	{
		ASSERT(0);
		return FALSE;
	}

	m_nMinTickSpacing = nPixels;

	if (GetSafeHwnd())
		Invalidate();

	return TRUE;
}

BOOL CRangeSliderCtrl::IsValidMove(double dLeft, double dRight) const
{
	if ((dRight - dLeft) < m_dMinRangeWidth)
		return FALSE;

	if (m_dMaxRangeWidth != -1)
	{
		if ((dRight - dLeft) > m_dMaxRangeWidth)
			return FALSE;
	}

	return TRUE;
}

void CRangeSliderCtrl::SetParentBackgroundColor(COLORREF crBkgnd)
{
	ASSERT(crBkgnd != CLR_NONE);

	if (crBkgnd != m_crParentBkgnd)
	{
		m_crParentBkgnd = crBkgnd;

		if (GetSafeHwnd())
			Invalidate(FALSE);
	}
}

BOOL CRangeSliderCtrl::HasSelectedRange() const
{
	return ((m_Left > m_Min) || (m_Right < m_Max));
}

void CRangeSliderCtrl::DrawRegion(CDC& dc, RS_DRAWREGION nRegion, const CRect& rRegion)
{
	switch (nRegion)
	{
	case RSDR_BACKGROUND:
		{
			if (m_crParentBkgnd != CLR_NONE)
				dc.FillSolidRect(rRegion, m_crParentBkgnd);
			else
				dc.FillSolidRect(rRegion, GetSysColor(COLOR_3DFACE));

			CRect rTrack(rRegion);
			RegionToTrack(rTrack);
				
			if (CThemed::AreControlsThemed())
			{
				CThemed th(this, _T("TRACKBAR"));
				th.DrawBackground(&dc, (m_bHorizontal ? TKP_TRACK : TKP_TRACKVERT), TKS_NORMAL, rTrack);
			}
			else
			{
				CRangeSlider::DrawRegion(dc, nRegion, rTrack);
			}
		}
		return;

	case RSDR_LEFT:
		if (m_Left > m_Min)
		{
			CRect rTrack(rRegion);
			RegionToTrack(rTrack);

			if (!CThemed::AreControlsThemed() && (m_crParentBkgnd == CLR_NONE))
				CRangeSlider::DrawRegion(dc, nRegion, rTrack);

			DrawTicks(dc, rTrack, m_Min, m_Left);
		}
		return;

	case RSDR_RIGHT:
		if (m_Right < m_Max)
		{
			CRect rTrack(rRegion);
			RegionToTrack(rTrack);

			if (!CThemed::AreControlsThemed() && (m_crParentBkgnd == CLR_NONE))
				CRangeSlider::DrawRegion(dc, nRegion, rTrack);

			DrawTicks(dc, rTrack, m_Right, m_Max);
		}
		return;

	case RSDR_FOCUS:
		return;
	}

	// all else
	CRangeSlider::DrawRegion(dc, nRegion, rRegion);
}

BOOL CRangeSliderCtrl::IsHot(BUTTON_ID nBtn) const
{
	return (!m_bTracking && m_hotTrack.IsRectHot(nBtn));
}

void CRangeSliderCtrl::DrawButton(CDC& dc, BUTTON_ID nBtn, const CRect& rButton, const CString& sText)
{
	VERIFY(!m_hotTrack.IsInitialized() || m_hotTrack.UpdateRect(nBtn, rButton));

	BOOL bPressed = IsPressed(nBtn);
	BOOL bHot = IsHot(nBtn);

	if (CThemed::AreControlsThemed())
	{
		switch (nBtn)
		{
		case BUTTON_LEFT:
		case BUTTON_RIGHT:
			{
				int nPart = 0;

				if (m_nSliderDrawStyles & TBS_TOP)
				{
					if (m_bHorizontal)
						nPart = TKP_THUMBTOP;
					else
						nPart = TKP_THUMBLEFT;
				}
				else if (m_nSliderDrawStyles & TBS_BOTH)
				{
					nPart = TKP_THUMB;
				}
				else
				{
					if (m_bHorizontal)
						nPart = TKP_THUMBBOTTOM;
					else
						nPart = TKP_THUMBRIGHT;
				}
				ASSERT(nPart != 0);

				int nState = TUS_DISABLED;

				if (IsWindowEnabled())
				{
					if (bPressed)
					{
						nState = TUS_PRESSED;
					}
					else if (bHot)
					{
						nState = TUS_HOT;
					}
					else
					{
						nState = TUS_NORMAL;
					}
				}
				ASSERT(nState != 0);

				CThemed th(this, _T("TRACKBAR"));
				th.DrawBackground(&dc, nPart, nState, rButton);
			}
			break;

		case BUTTON_MIDDLE:
			{
				ASSERT(nBtn = BUTTON_MIDDLE);

				CRect rTrack(rButton);
				RegionToTrack(rTrack);
				rTrack.InflateRect(1, 0);

				if (IsWindowEnabled())
				{
					CThemed th(this, _T("PROGRESS"));
					th.DrawBackground(&dc, (m_bHorizontal ? PP_FILL : PP_FILLVERT), 0, rTrack);

					// Don't display hotness if at full range
					if (bHot)
						bHot = !((m_Left == m_Min) && (m_Right == m_Max));

					BOOL bTrackingEnds = (m_bTracking && !bPressed);

					// Fade the colour using semi-transparency
					if (!bPressed && (bTrackingEnds || !bHot))
						GraphicsMisc::DrawRect(&dc, rTrack, RGB(255, 255, 255), CLR_NONE, 0, GMDR_NONE, (bTrackingEnds ? 96 : 128));
				}

				DrawTicks(dc, rTrack, m_Left, m_Right);
			}
			break;
		}

		return;
	}

	// else
	CRangeSlider::DrawButton(dc, nBtn, rButton, sText);
}

void CRangeSliderCtrl::DrawTicks(CDC& dc, const CRect& rTrack, double dFrom, double dTo)
{
	if ((m_nSliderDrawStyles & TBS_NOTICKS) == 0)
	{
		double dNumTick = ((dTo - dFrom) / m_Step);
		double dTickSpacing = (rTrack.Width() / dNumTick);

		// Skip first and last ticks
		int nNumTick = Misc::Round(dNumTick);
		int nLastTickPos = rTrack.left;

		for (int nTick = 1; nTick < nNumTick; nTick++)
		{
			int nTickPos = (rTrack.left + (int)(nTick * dTickSpacing));

			if ((nTickPos - nLastTickPos) > m_nMinTickSpacing)
			{
				CRect rTick(nTickPos, rTrack.bottom, (nTickPos + 1), rTrack.bottom + 4);
				dc.FillSolidRect(rTick, GetSysColor(COLOR_3DSHADOW));

				nLastTickPos = nTickPos;
			}
		}
	}
}

void CRangeSliderCtrl::RegionToTrack(CRect& rRegion) const
{
	int nStyle = GetStyle();

	if (nStyle & TBS_BOTH) // == NO ARROW
	{
		if (m_bHorizontal)
			rRegion.DeflateRect(0, 3, 0, 3);
		else
			rRegion.DeflateRect(3, 0, 3, 0);
	}
	else if (nStyle & TBS_TOP)
	{
		if (m_bHorizontal)
			rRegion.DeflateRect(0, 6, 0, 3);
		else
			rRegion.DeflateRect(6, 0, 3, 0);
	}
	else
	{
		if (m_bHorizontal)
			rRegion.DeflateRect(0, 3, 0, 6);
		else
			rRegion.DeflateRect(3, 0, 6, 0);
	}
}

int CRangeSliderCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CRangeSlider::OnCreate(lpCreateStruct) != 0)
		return -1;

	VERIFY (Initialize());

	return 0;
}

BOOL CRangeSliderCtrl::Initialize()
{
	if (CThemed::AreControlsThemed() && !m_hotTrack.IsInitialized())
	{
		ASSERT(m_hotTrack.GetRectCount() == 0);
		
		if (!m_hotTrack.Initialize(this, FALSE))
			return FALSE;

		VERIFY(BUTTON_LEFT == m_hotTrack.AddRect(m_RectLeft));
		VERIFY(BUTTON_RIGHT == m_hotTrack.AddRect(m_RectRight));
		VERIFY(BUTTON_MIDDLE == m_hotTrack.AddRect(CRect(m_RectLeft.right, m_RectLeft.top, m_RectRight.left, m_RectRight.bottom)));
	}

	return TRUE;
}

void CRangeSliderCtrl::PreSubclassWindow()
{
	CRangeSlider::PreSubclassWindow();

	VERIFY (Initialize());
}

LRESULT CRangeSliderCtrl::OnHotChange(WPARAM wp, LPARAM lp) 
{ 
	Invalidate(FALSE); 
	UpdateWindow();

	return 0L; 
}
