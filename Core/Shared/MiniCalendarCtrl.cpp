// MiniCalendarCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MiniCalendarCtrl.h"

#include "datehelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const UINT DEFTEXTFLAGS = (DT_VCENTER | DT_CENTER | DT_SINGLELINE/* | DT_RTLREADING*/);
const int WEEKNUMBERPADDING = 8;

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

	for (int nDay = 0; nDay < 7; nDay++)
		SetDayOfWeekName(nDay + 1, CDateHelper::GetDayOfWeekName(OLE_DAYSOFWEEK[nDay]));

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
	if (m_bShowWeekNumbers && (m_nWeekNumberWidth == 0))
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
	}

	// Base class
	iLeftX += (nWeekWidth / 2);

	return CFPSMiniCalendarCtrl::DrawDays(dc, iY, iLeftX, iRow, iCol, iMonth, iYear);
}
