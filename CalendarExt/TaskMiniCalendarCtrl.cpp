// TaskCalendarCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TaskMiniCalendarCtrl.h"
#include "CalMsg.h"

#include "..\Shared\GraphicsMisc.h"
#include "..\Shared\themed.h"
#include "..\Shared\DateHelper.h"
#include "..\Shared\TimeHelper.h"
#include "..\Shared\misc.h"
#include "..\Shared\dialoghelper.h"
#include "..\Shared\holdredraw.h"
#include "..\Shared\autoflag.h"
#include "..\Shared\enimagelist.h"

#include <math.h>

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
	m_nHeatMapAttribute(IUI_NONE), 
	m_nMaxHeat(0)
{
}

CTaskMiniCalendarCtrl::~CTaskMiniCalendarCtrl()
{
}

BEGIN_MESSAGE_MAP(CTaskMiniCalendarCtrl, CFPSMiniCalendarCtrl)
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
	if (m_mapData.GetCount() == 0)
		return;

	m_mapHeatMap.RemoveAll();

	POSITION pos = m_mapData.GetStartPosition();
	DWORD dwTaskID = 0;
	TASKCALITEM* pTCI = NULL;

	while (pos)
	{
		m_mapData.GetNextAssoc(pos, dwTaskID, pTCI);

		switch (m_nHeatMapAttribute)
		{
		case IUI_DONEDATE:
			if (pTCI->IsDone(FALSE))
				IncrementDateHeat(pTCI->GetDoneDate());
			break;
		}
	}

	Invalidate();
}

void CTaskMiniCalendarCtrl::IncrementDateHeat(const COleDateTime& dt)
{
	int nNewHeat = (GetDateHeat(dt) + 1);

	m_mapHeatMap[CDateHelper::GetDateOnly(dt)] = nNewHeat;
	m_nMaxHeat = max(nNewHeat, m_nMaxHeat);
}

int CTaskMiniCalendarCtrl::GetDateHeat(const COleDateTime& dt) const
{
	int nHeat = 0;
	m_mapHeatMap.Lookup(CDateHelper::GetDateOnly(dt.m_dt), nHeat);

	return nHeat;
}

BOOL CTaskMiniCalendarCtrl::IsSpecialDate(const COleDateTime& dt) const
{
	return m_setSpecialDates.Has(CDateHelper::GetDateOnly(dt.m_dt));
}

void CTaskMiniCalendarCtrl::GetDateCellColors(const COleDateTime& dt, BOOL bSelected, BOOL bSpecial, BOOL bActiveMonth, COLORREF& crText, COLORREF& crBkgnd) const
{
	CFPSMiniCalendarCtrl::GetDateCellColors(dt, bSelected, bSpecial, bActiveMonth, crText, crBkgnd);

	// Handle heat map
 	if (!bSelected && bActiveMonth && m_mapHeatMap.GetCount())
	{
		int nHeat = GetDateHeat(dt);

		if (nHeat)
		{
			int nColor = ((m_aPalette.GetSize() * nHeat) / m_nMaxHeat);
			nColor = min(nColor, m_aPalette.GetSize() - 1);

			if (nColor >= 0)
			{
				crBkgnd = m_aPalette[nColor];
				crText = GraphicsMisc::GetBestTextColor(crBkgnd);
			}
		}
	}
}

void CTaskMiniCalendarCtrl::EnableHeatMap(const CDWordArray& aPalette, IUI_ATTRIBUTE nAttrib)
{
	ASSERT(aPalette.GetSize());
	ASSERT(nAttrib != IUI_NONE);

	if (nAttrib != m_nHeatMapAttribute)
	{
		m_nHeatMapAttribute = nAttrib;
		RecalcHeatMap();
	}
	else if (!Misc::MatchAll(aPalette, m_aPalette, TRUE))
	{
		Invalidate();
	}

	m_aPalette.Copy(aPalette);
}

void CTaskMiniCalendarCtrl::DisableHeatMap()
{
	if (m_mapHeatMap.GetCount())
	{
		m_aPalette.RemoveAll();
		m_mapHeatMap.RemoveAll();
		m_nHeatMapAttribute = IUI_NONE;
		m_nMaxHeat = 0;

		Invalidate();
	}
}
