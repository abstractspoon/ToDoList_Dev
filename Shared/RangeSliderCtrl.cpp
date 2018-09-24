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

enum HOTRECTINDEX
{
	HRI_LEFT,
	HRI_MIDDLE,
	HRI_RIGHT,
};

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
		{
			VERIFY(m_hotTrack.UpdateRect(HRI_MIDDLE, rRegion));

			if (CThemed::AreControlsThemed() || (m_crParentBkgnd != CLR_NONE))
				return;
		}
		break;

	case RSDR_LEFTBUTTON:
		VERIFY(m_hotTrack.UpdateRect(HRI_LEFT, rRegion));
		break;

	case RSDR_RIGHTBUTTON:
		VERIFY(m_hotTrack.UpdateRect(HRI_RIGHT, rRegion));
		break;

	case RSDR_FOCUS:
		return;
	}

	// all else
	CRangeSlider::DrawRegion(dc, nRegion, rRegion);
}

void CRangeSliderCtrl::DrawButton(CDC& dc, RS_DRAWREGION nRegion, const CRect& rButton, const CString& sText, BOOL bPressed)
{
	if (CThemed::AreControlsThemed())
	{
		CThemed th(this, _T("TRACKBAR"));

		int nPart = TKP_THUMB;

		if (m_nThumbStyle & TBS_TOP)
		{
			if (m_bHorizontal)
				nPart = TKP_THUMBTOP;
			else
				nPart = TKP_THUMBLEFT;
		}
		else if (!(m_nThumbStyle & TBS_BOTH))
		{
			if (m_bHorizontal)
				nPart = TKP_THUMBBOTTOM;
			else
				nPart = TKP_THUMBRIGHT;
		}

		int nState = (bPressed ? TUS_PRESSED : TUS_NORMAL);

		switch (nRegion)
		{
		case RSDR_MIDDLE:
			if (!bPressed && m_hotTrack.IsHot(HRI_MIDDLE))
				nState = TUS_HOT;
			break;

		case RSDR_LEFTBUTTON:
			if (!bPressed && m_hotTrack.IsHot(HRI_LEFT))
				nState = TUS_HOT;
			break;

		case RSDR_RIGHTBUTTON:
			if (!bPressed && m_hotTrack.IsHot(HRI_RIGHT))
				nState = TUS_HOT;
			break;
		}

		th.DrawBackground(&dc, nPart, nState, rButton);
	}
	else
	{
		CRangeSlider::DrawButton(dc, nRegion, rButton, sText, bPressed);
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

		VERIFY(HRI_LEFT == m_hotTrack.AddRect(m_RectLeft));
		VERIFY(HRI_MIDDLE == m_hotTrack.AddRect(CRect(m_RectLeft.right, m_RectLeft.top, m_RectRight.left, m_RectRight.bottom)));
		VERIFY(HRI_RIGHT == m_hotTrack.AddRect(m_RectRight));
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
