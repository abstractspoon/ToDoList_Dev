// CalendarCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "CalendarCtrlEx.h"

#include "DateHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalendarCtrlEx

CCalendarCtrlEx::CCalendarCtrlEx()
{
	SetFirstWeekDay(CDateHelper::GetFirstDayOfWeek());
}

CCalendarCtrlEx::~CCalendarCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CCalendarCtrlEx, CCalendarCtrl)
	//{{AFX_MSG_MAP(CCalendarCtrlEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCalendarCtrlEx message handlers

CString CCalendarCtrlEx::GetMonthName(const COleDateTime& date, BOOL bShort) const
{
	return CDateHelper::GetMonthName(date.GetMonth(), bShort);
}

CString CCalendarCtrlEx::GetDayOfWeekName(const COleDateTime& date, BOOL bShort) const
{
	return CDateHelper::GetDayOfWeekName(CDateHelper::GetDayOfWeek(date), bShort);
}

void CCalendarCtrlEx::DrawHeader(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	rc.bottom = rc.top + m_nHeaderHeight;

	CRect rcLine(rc);
	rcLine.top = rcLine.bottom-1;

	int i;

	for(i=0; i<m_nHeaderHeight; i++)
	{
		pDC->FillSolidRect(rcLine, GetFadedThemeColour(i*4));
		rcLine.bottom--;
		rcLine.top = rcLine.bottom-1;
	}

	CFont* pOldFont = pDC->SelectObject(&m_DefaultFont);
	int nWidth = rc.Width()/CALENDAR_NUM_COLUMNS;
	bool bShort = (CDateHelper::GetMaxDayOfWeekNameWidth(pDC) > nWidth);

	for(i=0 ; i<CALENDAR_NUM_COLUMNS; i++)
	{
		CRect txtRect(i*nWidth, 2, (i+1)*nWidth, m_nHeaderHeight);
		int nDOW = GetDayOfWeek(i);

		CString sDOW = CDateHelper::GetDayOfWeekName((OLE_DAYOFWEEK)nDOW, bShort);

		pDC->DrawText(sDOW, txtRect, DT_CENTER|DT_VCENTER);
	}
	pDC->SelectObject(pOldFont);
}

