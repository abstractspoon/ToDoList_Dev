// RangeSliderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "RangeSliderCtrl.h"

#include "Themed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRangeSliderCtrl

CRangeSliderCtrl::CRangeSliderCtrl(UINT nThumbStyle) : m_crParentBkgnd(CLR_NONE), m_nThumbStyle(nThumbStyle)
{
}

CRangeSliderCtrl::~CRangeSliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CRangeSliderCtrl, CRangeSlider)
	//{{AFX_MSG_MAP(CRangeSliderCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
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

void CRangeSliderCtrl::DrawRegion(CDC& dc, RS_DRAWREGION nRegion, const CRect& rRegion) const
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
				th.DrawBackground(&dc, TKP_TRACK, TKS_NORMAL, rTrack);
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

	case RSDR_MIDDLE:
		if (CThemed::AreControlsThemed() || (m_crParentBkgnd != CLR_NONE))
			return;
		break;

	case RSDR_FOCUS:
		return;
	}

	// all else
	CRangeSlider::DrawRegion(dc, nRegion, rRegion);
}

void CRangeSliderCtrl::DrawButton(CDC& dc, const CRect& rButton, const CString& sText, BOOL bPressed) const
{
	if (CThemed::AreControlsThemed())
	{
		CThemed th(this, _T("TRACKBAR"));
		int nStyle = GetStyle(), nPart = TKP_THUMBBOTTOM;

		if (nStyle & TBS_BOTH)
		{
		}
		else if (nStyle & TBS_TOP)
		{
		}
		else
		{
		}

		th.DrawBackground(&dc, nPart, (bPressed ? TUS_HOT : TUS_NORMAL), rButton);
	}
	else
	{
		CRangeSlider::DrawButton(dc, rButton, sText, bPressed);
	}
}

void CRangeSliderCtrl::RegionToTrack(CRect& rRegion) const
{
	int nStyle = GetStyle();

	if (nStyle & TBS_BOTH)
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
