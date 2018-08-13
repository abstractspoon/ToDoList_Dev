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
	: m_mapData(mapData)
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

void CTaskMiniCalendarCtrl::RecalcSpecialDates()
{
	m_setSpecialDates.RemoveAll();

	POSITION pos = m_mapData.GetStartPosition();
	DWORD dwTaskID = 0;
	TASKCALITEM* pTCI = NULL;

	while (pos)
	{
		m_mapData.GetNextAssoc(pos, dwTaskID, pTCI);

		// process item for special dates
		if (HasOption(TCCO_DISPLAYDONE) || !pTCI->IsDone(TRUE))
		{
			if (pTCI->IsStartDateSet())
				m_setSpecialDates.Add(CDateHelper::GetDateOnly(pTCI->GetAnyStartDate()).m_dt);

			if (pTCI->IsEndDateSet())
				m_setSpecialDates.Add(CDateHelper::GetDateOnly(pTCI->GetAnyEndDate()).m_dt);
		}
	}
}

BOOL CTaskMiniCalendarCtrl::IsSpecialDate(const COleDateTime& dt) const
{
	return m_setSpecialDates.Has(dt.m_dt);
}

COLORREF CTaskMiniCalendarCtrl::GetDateBkgndColor(const COleDateTime& dt, BOOL bSelected, BOOL bSpecial, BOOL bActiveMonth) const
{
// 	if (!bSelected && bActiveMonth && ((int)dt.m_dt % 2))
// 		return 255;

	return CFPSMiniCalendarCtrl::GetDateBkgndColor(dt, bSelected, bSpecial, bActiveMonth);
}

COLORREF CTaskMiniCalendarCtrl::GetDateTextColor(const COleDateTime& dt, BOOL bSelected, BOOL bSpecial, BOOL bActiveMonth) const
{
	return CFPSMiniCalendarCtrl::GetDateTextColor(dt, bSelected, bSpecial, bActiveMonth);
}
