// RangeSliderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "RangeSliderCtrl.h"

#include "Themed.h"
#include "Misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRangeSliderCtrl

CRangeSliderCtrl::CRangeSliderCtrl(UINT nThumbStyle) : m_crParentBkgnd(CLR_NONE), m_nSliderDrawStyles(nThumbStyle)
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

void CRangeSliderCtrl::SetParentBackgroundColor(COLORREF crBkgnd)
{
	if (crBkgnd != m_crParentBkgnd)
	{
		m_crParentBkgnd = crBkgnd;

		if (GetSafeHwnd())
			Invalidate(FALSE);
	}
}

void CRangeSliderCtrl::DrawRegion(CDC& dc, RS_DRAWREGION nRegion, const CRect& rRegion)
{
	switch (nRegion)
	{
	case RSDR_BACKGROUND:
		{
			if (m_crParentBkgnd != CLR_NONE)
				dc.FillSolidRect(rRegion, m_crParentBkgnd);

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
	case RSDR_RIGHT:
		if (CThemed::AreControlsThemed() || (m_crParentBkgnd != CLR_NONE))
			return;

	case RSDR_FOCUS:
		return;
	}

	// all else
	CRangeSlider::DrawRegion(dc, nRegion, rRegion);
}

BOOL CRangeSliderCtrl::IsHot(BUTTON_ID nBtn) const
{
	return (!m_bTracking && m_hotTrack.IsHot(nBtn));
}

void CRangeSliderCtrl::DrawButton(CDC& dc, BUTTON_ID nBtn, const CRect& rButton, const CString& sText)
{
	VERIFY(m_hotTrack.UpdateRect(BUTTON_MIDDLE, rButton));

	if (CThemed::AreControlsThemed())
	{
		if ((nBtn == BUTTON_LEFT) || (nBtn == BUTTON_RIGHT))
		{
			CThemed th(this, _T("TRACKBAR"));
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

			int nState = 0;
			
			if (IsPressed(nBtn))
			{
				nState = TUS_PRESSED;
			}
			else if (IsHot(nBtn))
			{
				nState = TUS_HOT;
			}
			else
			{
				nState = TUS_NORMAL;
			}
			ASSERT(nState != 0);

			th.DrawBackground(&dc, nPart, nState, rButton);
		}
		else
		{
			ASSERT(nBtn = BUTTON_MIDDLE);
			CThemed th(this, _T("PROGRESS"));

			CRect rTrack(rButton);
			RegionToTrack(rTrack);
			
			th.DrawBackground(&dc, (m_bHorizontal ? PP_CHUNK : PP_CHUNKVERT), 0, rTrack);

			if ((m_nSliderDrawStyles & TBS_NOTICKS) == 0)
			{
				int nNumTick = Misc::Round((m_Right - m_Left) / m_Step);
				double dTickSpacing = ((double)rTrack.Width() / nNumTick);

				// Skip first and last ticks
				for (int nTick = 1; nTick < nNumTick; nTick++)
				{
					int nTickPos = (rButton.left + (int)(nTick * dTickSpacing));
					CRect rTick(nTickPos, rTrack.bottom, (nTickPos + 1), rTrack.bottom + 4);

					dc.FillSolidRect(rTick, GetSysColor(COLOR_3DSHADOW));
				}
			}
		}
	}
	else
	{
		CRangeSlider::DrawButton(dc, nBtn, rButton, sText);
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
		
		if (!m_hotTrack.Initialize(this))
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
