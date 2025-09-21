// TaskCalendarCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TaskMiniCalendarCtrl.h"
#include "CalEnum.h"
#include "resource.h"

#include "..\Shared\GraphicsMisc.h"
#include "..\Shared\DateHelper.h"
#include "..\Shared\misc.h"
#include "..\Shared\enstring.h"

#include "..\Interfaces\UITHEME.h"

#include <math.h>

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaskMiniCalendarCtrl

CTaskMiniCalendarCtrl::CTaskMiniCalendarCtrl(const CTaskCalItemMap& mapData) 
	: 
	m_mapData(mapData), 
	m_nHeatMapAttribute(TDCA_NONE),
	m_crBorder(CLR_NONE),
	m_crWeekend(CLR_NONE),
	m_crSelected(RGB(160, 215, 255))
{
	m_bHighlightToday = FALSE; // we handle it ourselves
}

CTaskMiniCalendarCtrl::~CTaskMiniCalendarCtrl()
{
}

BEGIN_MESSAGE_MAP(CTaskMiniCalendarCtrl, CMiniCalendarCtrl)
	//{{AFX_MSG_MAP(CTaskMiniCalendarCtrl)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskMiniCalendarCtrl message handlers

void CTaskMiniCalendarCtrl::SetOptions(DWORD dwOptions, LPCTSTR szHideParentTag)
{
	if ((m_dwOptions != dwOptions) || (Misc::HasFlag(m_dwOptions, TCCO_HIDEPARENTTASKS) && (m_sHideParentTag != szHideParentTag)))
	{
		m_dwOptions = dwOptions;
		m_sHideParentTag = szHideParentTag;

		if (dwOptions & TCCO_DATEDISPLAYOPTIONS)
			RecalcSpecialDates();
	
		RecalcHeatMap();
	}
}

void CTaskMiniCalendarCtrl::SetBorderColor(COLORREF crBorder)
{
	if (crBorder != m_crBorder)
	{
		m_crBorder = crBorder;

		if (GetSafeHwnd())
			Invalidate(FALSE);
	}
}

void CTaskMiniCalendarCtrl::SetUITheme(const UITHEME& theme)
{
	if ((theme.crWeekend != m_crWeekend) || (theme.crToday != m_crToday))
	{
		m_crWeekend = theme.crWeekend;
		m_crToday = theme.crToday;

		if (GetSafeHwnd())
			Invalidate(FALSE);
	}
}

void CTaskMiniCalendarCtrl::OnUpdateTasks()
{
	RecalcSpecialDates();
	RecalcHeatMap();
}

void CTaskMiniCalendarCtrl::RecalcSpecialDates()
{
	m_setSpecialDates.RemoveAll();

	POSITION pos = m_mapData.GetStartPosition();
	DWORD dwTaskID = 0;
	TASKCALITEM* pTCI = NULL;

	while (pos)
	{
		m_mapData.GetNextAssoc(pos, dwTaskID, pTCI);

		if (CTaskCalItemMap::WantHideTask(pTCI, m_dwOptions, m_sHideParentTag))
			continue;

		// else
		if (pTCI->IsStartDateSet())
			m_setSpecialDates.Add(CDateHelper::GetDateOnly(pTCI->GetAnyStartDate()).m_dt);

		if (pTCI->IsEndDateSet())
			m_setSpecialDates.Add(CDateHelper::GetDateOnly(pTCI->GetAnyEndDate()).m_dt);
	}
}

void CTaskMiniCalendarCtrl::RecalcHeatMap()
{
	if (m_mapHeatMap.Recalculate(m_mapData, m_nHeatMapAttribute, m_dwOptions, m_sHideParentTag))
		Invalidate();
}

BOOL CTaskMiniCalendarCtrl::IsSpecialDate(const COleDateTime& dt) const
{
	return m_setSpecialDates.Has(CDateHelper::GetDateOnly(dt.m_dt));
}

COLORREF CTaskMiniCalendarCtrl::GetCellBkgndColor(const COleDateTime& dt, BOOL bSelected, BOOL /*bSpecial*/, BOOL bActiveMonth) const
{
	if (bSelected && Misc::IsHighContrastActive())
		return GetSysColor(COLOR_HIGHLIGHT);

	if (bActiveMonth && m_mapHeatMap.HasHeat())
		return m_mapHeatMap.GetColor(dt);

	// Ignore base class totally
	return CLR_NONE;
}

COLORREF CTaskMiniCalendarCtrl::GetCellTextColor(const COleDateTime& dt, BOOL bSelected, BOOL bSpecial, BOOL bActiveMonth) const
{
	if (bSelected && Misc::IsHighContrastActive())
		return GetSysColor(COLOR_HIGHLIGHTTEXT);

	if (bActiveMonth && m_mapHeatMap.HasHeat())
	{
		COLORREF crBkgnd = m_mapHeatMap.GetColor(dt);

		if (crBkgnd != CLR_NONE)
			return GraphicsMisc::GetBestTextColor(crBkgnd);
	}

	// We handle selection as an overlay so we always pass FALSE
	return CMiniCalendarCtrl::GetCellTextColor(dt, FALSE, bSpecial, bActiveMonth);;
}

void CTaskMiniCalendarCtrl::DrawCellBkgnd(CDC& dc, const CRect& rCell, const COleDateTime& dt, BOOL bSelected, BOOL bSpecial, BOOL bActiveMonth)
{
	COLORREF crBack = GetCellBkgndColor(dt, bSelected, bSpecial, bActiveMonth);

	BOOL bBkgnd = (crBack != CLR_NONE);
	BOOL bWeekend = (!bBkgnd && (m_crWeekend != CLR_NONE) && CWeekend().IsWeekend(dt));
	BOOL bToday = ((m_crToday != CLR_NONE) && IsToday(dt));
	
	if (bToday)
	{
		GraphicsMisc::DrawRect(&dc, rCell, m_crToday, CLR_NONE, 0, GMDR_NONE, 128);
	}
	else if (bBkgnd)
	{
		dc.FillSolidRect(rCell, crBack);
	}
	else if (bWeekend)
	{
		GraphicsMisc::DrawRect(&dc, rCell, m_crWeekend, CLR_NONE, 0, GMDR_NONE, 128);
	}

	if (bSelected && !Misc::IsHighContrastActive())
		GraphicsMisc::DrawRect(&dc, rCell, m_crSelected, CLR_NONE, 0, GMDR_NONE, (BYTE)(bToday ? 80 : 160));
}

void CTaskMiniCalendarCtrl::EnableHeatMap(const CDWordArray& aPalette, TDC_ATTRIBUTE nAttribID)
{
	ASSERT(aPalette.GetSize());
	ASSERT(nAttribID != TDCA_NONE);

	if (m_mapHeatMap.SetColorPalette(aPalette))
		Invalidate();

	if (nAttribID != m_nHeatMapAttribute)
	{
		m_nHeatMapAttribute = nAttribID;
		RecalcHeatMap();
	}

	if (!m_tooltip.GetSafeHwnd())
		m_tooltip.Create(this, TTS_ALWAYSTIP | TTS_NOPREFIX);
}

void CTaskMiniCalendarCtrl::DisableHeatMap()
{
	if (m_mapHeatMap.HasHeat())
	{
		m_tooltip.DestroyWindow();
		m_mapHeatMap.ClearHeat();
		m_nHeatMapAttribute = TDCA_NONE;

		Invalidate();
	}
}

void CTaskMiniCalendarCtrl::FilterToolTipMessage(MSG* pMsg)
{
	m_tooltip.FilterToolTipMessage(pMsg);
}

int CTaskMiniCalendarCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	const CFPSMiniCalendarCtrlFontHotSpot* pSpot = HitTest(point);

	if (pSpot)
	{
		int nHeat = m_mapHeatMap.GetHeat(pSpot->m_dt);

		if (nHeat > 0)
		{
			return CToolTipCtrlEx::SetToolInfo(*pTI, 
												this, 
												CEnString().Format(IDS_MINICAL_TOOLTIP, nHeat),
												(int)pSpot->m_dt, 
												pSpot->m_rect);
		}
	}

	return CMiniCalendarCtrl::OnToolHitTest(point, pTI);
}

void CTaskMiniCalendarCtrl::Draw(CDC& dc, const CRect& rDraw)
{
	// Draw a border on our RHS
	if (m_crBorder != CLR_NONE)
	{
		CRect rBorder(rDraw);
		rBorder.left = (rBorder.right - 1);

		dc.FillSolidRect(rBorder, m_crBorder);
	}

	// Then default
	CMiniCalendarCtrl::Draw(dc, rDraw);
}
