// MiniCalendarCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MiniCalendarCtrl.h"

#include "datehelper.h"
#include "misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef LOCALE_SNAME
#	define LOCALE_SNAME 0x0000005c
#endif

const UINT DEFTEXTFLAGS = (DT_VCENTER | DT_CENTER | DT_SINGLELINE);

const int WEEKNUMBERPADDING = 8;

/////////////////////////////////////////////////////////////////////////////

#ifndef LOCALE_SNAME
#	define LOCALE_SNAME 0x0000005c 
#endif

/////////////////////////////////////////////////////////////////////////////
// CMiniCalendarCtrl

CMiniCalendarCtrl::CMiniCalendarCtrl() : m_bShowWeekNumbers(FALSE), m_nWeekNumberWidth(0)
{
}

CMiniCalendarCtrl::~CMiniCalendarCtrl()
{
}


BEGIN_MESSAGE_MAP(CMiniCalendarCtrl, CFPSMiniCalendarCtrl)
	//{{AFX_MSG_MAP(CMiniCalendarCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMiniCalendarCtrl message handlers

int CMiniCalendarCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	for (int nMonth = 1; nMonth <= 12; nMonth++)
		SetMonthName(nMonth, CDateHelper::GetMonthName(nMonth));

	// Set first day of week before weekday names because
	// Otherwise it will make an incorrect adjustment
	SetFirstDayOfWeek(CDateHelper::GetFirstDayOfWeek());

	// The base class is going to draw only the very first character
	// of the weekday name which works in 99% of cases but not for 
	// (Simplified) Chinese whose unique character comes at the end.
	// So we handle that specific case here:
	CString sRegion = Misc::GetLocaleInfo(LOCALE_SNAME);
	BOOL bIsChinese = (sRegion.Find(_T("zh")) != -1);

	for (int nDay = 0; nDay < 7; nDay++)
	{
		CString sDowName = CDateHelper::GetDayOfWeekName(OLE_DAYSOFWEEK[nDay], TRUE); // short name
		SetDayOfWeekName(nDay + 1, (bIsChinese ? sDowName.Right(1) : sDowName));
	}

	return CFPSMiniCalendarCtrl::OnCreate(lpCreateStruct);
}

CSize CMiniCalendarCtrl::ComputeSize()
{
	CSize size = CFPSMiniCalendarCtrl::ComputeSize();

	size.cx += ComputeWeekNumberWidth();
	m_szMonthSize = size;

	return size;
}

int CMiniCalendarCtrl::ComputeWeekNumberWidth()
{
	if (!CDateHelper::WantRTLDates() && m_bShowWeekNumbers && (m_nWeekNumberWidth == 0))
	{
		if (!m_bFontsCreated)
			CreateFontObjects();

		// allocate a DC to use when testing sizes, etc
		CClientDC dc(m_hWnd ? this : AfxGetMainWnd());

		// get current font and save for later restoration
		CFont* pOldFont = dc.SelectObject(m_FontInfo[FMC_FONT_DAYSOFWEEK].m_pFont);

		m_nWeekNumberWidth = (dc.GetTextExtent("52").cx + WEEKNUMBERPADDING);

		// cleanup DC
		dc.SelectObject(pOldFont);
	}

	return m_nWeekNumberWidth;
}

int CMiniCalendarCtrl::DrawDaysOfWeek(CDC &dc, int iY, int iLeftX, int iRow, int iCol)
{
	int nWeekWidth = ComputeWeekNumberWidth();
	iLeftX += (nWeekWidth / 2);

	return CFPSMiniCalendarCtrl::DrawDaysOfWeek(dc, iY, iLeftX, iRow, iCol);
}

int CMiniCalendarCtrl::DrawDays(CDC& dc, int iY, int iLeftX, int iRow, int iCol, int iMonth, int iYear)
{
	int nWeekWidth = ComputeWeekNumberWidth();

	if (nWeekWidth)
	{
		COleDateTime dtStart(iYear, iMonth, 1, 0, 0, 0), dt(dtStart);

		while (dtStart.GetDayOfWeek() != m_iFirstDayOfWeek)
			dtStart -= 1;

		int iRowY = iY;

		for (int nRow = 1; nRow <= 6; nRow++)
		{
			// Draw week number
			int nWeek = CDateHelper::GetWeekofYear(dtStart);
			CRect rect((iLeftX + WEEKNUMBERPADDING), iRowY, (iLeftX + nWeekWidth), iRowY+m_iDaysHeight);

			dc.SelectObject(m_FontInfo[FMC_FONT_DAYS].m_pFont);
			dc.SetTextColor(GetSysColor(COLOR_3DDKSHADOW));
			dc.DrawText(CStr(nWeek), rect, DEFTEXTFLAGS);

			dtStart += 7;
			iRowY += (2 + m_iDaysHeight);
		}

		// Draw divider between week numbers and days
		dc.FillSolidRect(iLeftX + nWeekWidth + WEEKNUMBERPADDING, iY, 1, (6 * (2 + m_iDaysHeight)), GetSysColor(COLOR_3DDKSHADOW));

		// Base class
		iLeftX += (nWeekWidth / 2);
	}

	return CFPSMiniCalendarCtrl::DrawDays(dc, iY, iLeftX, iRow, iCol, iMonth, iYear);
}
