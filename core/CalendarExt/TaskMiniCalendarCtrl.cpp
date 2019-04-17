// TaskCalendarCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TaskMiniCalendarCtrl.h"
#include "CalMsg.h"
#include "resource.h"

#include "..\Shared\GraphicsMisc.h"
#include "..\Shared\DateHelper.h"
#include "..\Shared\misc.h"
#include "..\Shared\dialoghelper.h"
#include "..\Shared\enstring.h"

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
	m_nHeatMapAttribute(TDCA_NONE)
{
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

void CTaskMiniCalendarCtrl::SetOptions(DWORD dwOptions)
{
	if (m_dwOptions != dwOptions)
	{
		m_dwOptions = dwOptions;

		if (dwOptions & (TCCO_DISPLAYCONTINUOUS | 
						TCCO_DISPLAYSTART | 
						TCCO_DISPLAYDUE | 
						TCCO_DISPLAYDONE | 
						TCCO_DISPLAYCALCSTART | 
						TCCO_DISPLAYCALCDUE))
		{
			RecalcSpecialDates();
		}
	
		RecalcHeatMap();
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

		if (HasOption(TCCO_DISPLAYDONE) || !pTCI->IsDone(TRUE))
		{
			if (pTCI->IsStartDateSet())
				m_setSpecialDates.Add(CDateHelper::GetDateOnly(pTCI->GetAnyStartDate()).m_dt);

			if (pTCI->IsEndDateSet())
				m_setSpecialDates.Add(CDateHelper::GetDateOnly(pTCI->GetAnyEndDate()).m_dt);
		}
	}
}

void CTaskMiniCalendarCtrl::RecalcHeatMap()
{
	if (m_mapHeatMap.Recalculate(m_mapData, m_nHeatMapAttribute, m_dwOptions))
		Invalidate();
}

BOOL CTaskMiniCalendarCtrl::IsSpecialDate(const COleDateTime& dt) const
{
	return m_setSpecialDates.Has(CDateHelper::GetDateOnly(dt.m_dt));
}

void CTaskMiniCalendarCtrl::GetDateCellColors(const COleDateTime& dt, BOOL bSelected, BOOL bSpecial, BOOL bActiveMonth, COLORREF& crText, COLORREF& crBkgnd) const
{
	CMiniCalendarCtrl::GetDateCellColors(dt, bSelected, bSpecial, bActiveMonth, crText, crBkgnd);

	// Handle heat map
 	if (!bSelected && bActiveMonth && m_mapHeatMap.HasHeat())
	{
		crBkgnd = m_mapHeatMap.GetColor(dt);

		if (crBkgnd != CLR_NONE)
			crText = GraphicsMisc::GetBestTextColor(crBkgnd);
	}
}

void CTaskMiniCalendarCtrl::EnableHeatMap(const CDWordArray& aPalette, TDC_ATTRIBUTE nAttrib)
{
	ASSERT(aPalette.GetSize());
	ASSERT(nAttrib != TDCA_NONE);

	if (m_mapHeatMap.SetColorPalette(aPalette))
		Invalidate();

	if (nAttrib != m_nHeatMapAttribute)
	{
		m_nHeatMapAttribute = nAttrib;
		RecalcHeatMap();
	}

	if (!m_tooltip.GetSafeHwnd())
		m_tooltip.Create(this, TTS_ALWAYSTIP);
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

BOOL CTaskMiniCalendarCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (m_tooltip.GetSafeHwnd())
		m_tooltip.FilterToolTipMessage(pMsg);

	return CMiniCalendarCtrl::PreTranslateMessage(pMsg);
}

int CTaskMiniCalendarCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	const CFPSMiniCalendarCtrlFontHotSpot* pSpot = HitTest(point);

	if (pSpot)
	{
		int nHeat = m_mapHeatMap.GetHeat(pSpot->m_dt);

		if (nHeat > 0)
		{
			pTI->hwnd = GetSafeHwnd();
			pTI->uId = (DWORD)(int)pSpot->m_dt;
			pTI->uFlags |= TTF_TRANSPARENT;
			pTI->rect = pSpot->m_rect;

			CEnString sTooltip(IDS_MINICAL_TOOLTIP, nHeat);
			pTI->lpszText = _tcsdup(sTooltip); // MFC will free the duplicated string

			return (int)pTI->uId;
		}
	}

	return -1;
}
