// MonthCalCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "MonthCalCtrlEx.h"

#include "datehelper.h"
#include "misc.h"
#include "graphicsmisc.h"

/////////////////////////////////////////////////////////////////////////////////////

const UINT WM_REDRAWWEEKNUMBERS = (WM_USER+1);

/////////////////////////////////////////////////////////////////////////////////////
// CMonthCalCtrlEx

// IMPLEMENT_DYNAMIC(CMonthCalCtrlEx, CMonthCalCtrl)

CMonthCalCtrlEx::CMonthCalCtrlEx() 
	: 
	m_rWeekNumbers(0, 0, 0, 0), 
	m_bWeekNumbers(FALSE)
{

}

CMonthCalCtrlEx::~CMonthCalCtrlEx()
{
}

BEGIN_MESSAGE_MAP(CMonthCalCtrlEx, CMonthCalCtrl)
	ON_WM_SIZE()
	ON_WM_STYLECHANGED()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_MESSAGE(WM_REDRAWWEEKNUMBERS, OnRedrawWeekNumbers)
END_MESSAGE_MAP()

// CMonthCalCtrlEx message handlers
const CRect& CMonthCalCtrlEx::GetWeekNumbersRect()
{
	ASSERT(m_bWeekNumbers);

	if (m_bWeekNumbers && m_rWeekNumbers.IsRectNull())
	{
		GetClientRect(m_rWeekNumbers);

		// Brute force method for left and right
		MCHITTESTINFO mchi = { sizeof(mchi), 0 };

		mchi.pt.x = 0;
		mchi.pt.y = m_rWeekNumbers.CenterPoint().y;
		
		BOOL bFoundLeft = FALSE;

		while (mchi.pt.x < m_rWeekNumbers.right)
		{
			DWORD dwHit = HitTest(&mchi);

			if (!bFoundLeft && (dwHit == MCHT_CALENDARWEEKNUM))
			{
				m_rWeekNumbers.left = mchi.pt.x;
				bFoundLeft = TRUE;
			}
			else if (bFoundLeft && (dwHit != MCHT_CALENDARWEEKNUM))
			{
				m_rWeekNumbers.right = (mchi.pt.x - 1);
				break;
			}

			// else keep going
			mchi.pt.x++;
		}

		if (bFoundLeft)
		{
			mchi.pt.x = m_rWeekNumbers.CenterPoint().x;
			mchi.pt.y = 0;

			// Brute force method for top and bottom
			BOOL bFoundTop = FALSE;

			while (mchi.pt.y < m_rWeekNumbers.bottom)
			{
				DWORD dwHit = HitTest(&mchi);

				if (!bFoundTop && (dwHit == MCHT_CALENDARWEEKNUM))
				{
					m_rWeekNumbers.top = (mchi.pt.y + 1);
					bFoundTop = TRUE;
				}
				else if (bFoundTop && (dwHit != MCHT_CALENDARWEEKNUM))
				{
					m_rWeekNumbers.bottom = mchi.pt.y;
					break;
				}

				// else keep going
				mchi.pt.y++;
			}
			ASSERT(bFoundTop);

			if (bFoundTop)
			{
				// Make sure height is divisible by 6 (weeks)
				int nMod = (m_rWeekNumbers.Height() % 6);

				if (nMod)
					m_rWeekNumbers.bottom += (6 - nMod);

				ASSERT(!m_rWeekNumbers.IsRectEmpty());
				ASSERT((m_rWeekNumbers.Height() % 6) == 0);
			}
			else
			{
				m_rWeekNumbers.SetRectEmpty();
			}
		}
		else
		{
			m_rWeekNumbers.SetRectEmpty();
		}
	}

	return m_rWeekNumbers;
}

BOOL CMonthCalCtrlEx::IsMonthView() const
{
	COleDateTime dtStart, dtEnd;
	GetMonthRange(dtStart, dtEnd, GMR_VISIBLE);

	COleDateTimeSpan dtSpan = (dtEnd - dtStart);

	return (dtSpan.GetDays() <= 42); // (6 * 7) day grid
}

void CMonthCalCtrlEx::OnSize(UINT nType, int cx, int cy)
{
	CMonthCalCtrl::OnSize(nType, cx, cy);

	// Force recalculation of Week numbers rect
	m_rWeekNumbers.SetRect(0, 0, 0, 0);
}

void CMonthCalCtrlEx::DrawWeekNumbers(CDC* pDC)
{
	ASSERT(m_bWeekNumbers);

	if (m_bWeekNumbers)
	{
		CRect rClip;
		pDC->GetClipBox(rClip);
		
		CRect rWeekNumbers(GetWeekNumbersRect());

		if (!CRect().IntersectRect(rWeekNumbers, rClip))
			return;

		HFONT hFont = (HFONT)SendMessage(WM_GETFONT);
		HGDIOBJ hOldFont = pDC->SelectObject(hFont);

		COleDateTime dtStart, dtEnd;
		GetMonthRange(dtStart, dtEnd, GMR_DAYSTATE);

		pDC->FillSolidRect(rWeekNumbers, GetSysColor(COLOR_WINDOW));
		pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));

		int nDayHeight = (rWeekNumbers.Height() / 6);

		CRect rWeek(rWeekNumbers);
		rWeek.bottom = (rWeek.top + nDayHeight); // first row

		COleDateTime dtWeek = dtStart; // first row

		for (int nWeek = 0; nWeek < 6; nWeek++)
		{
			int nWeekNum = CDateHelper::GetWeekofYear(dtWeek);
			pDC->DrawText(Misc::Format(nWeekNum), rWeek, (DT_BOTTOM | DT_CENTER));

			// next row
			rWeek.OffsetRect(0, nDayHeight);
			dtWeek.m_dt += 7.0;
		}

		pDC->SelectObject(hOldFont);
	}
}

void CMonthCalCtrlEx::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	CMonthCalCtrl::OnStyleChanged(nStyleType, lpStyleStruct);
	
	// Force redraw of week numbers if they become visible
	if ((nStyleType == GWL_STYLE) && !m_bWeekNumbers && IsMonthView())
	{
		m_bWeekNumbers = ((lpStyleStruct->styleNew && MCS_WEEKNUMBERS) != 0);

		if (m_bWeekNumbers)
			InvalidateRect(GetWeekNumbersRect());
	}
}

void CMonthCalCtrlEx::PreSubclassWindow()
{
	m_bWeekNumbers = ((GetStyle() && MCS_WEEKNUMBERS) != 0);
	m_rWeekNumbers.SetRect(0, 0, 0, 0);

	CMonthCalCtrl::PreSubclassWindow();
}

void CMonthCalCtrlEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (m_bWeekNumbers && IsMonthView())
	{
		CDC dcTemp;

		if (dcTemp.CreateCompatibleDC(&dc))
		{
			CBitmap bmTemp;
			CRect rClient;

			GetClientRect(rClient);

			if (bmTemp.CreateCompatibleBitmap(&dc, rClient.right, rClient.bottom))
			{
				CBitmap* pOld = dcTemp.SelectObject(&bmTemp);

				dcTemp.FillSolidRect(rClient, GetSysColor(COLOR_WINDOW));

				// default draw to temp dc
				DefWindowProc(WM_PAINT, (WPARAM)(HDC)dcTemp, 0);

				// extra draw
				DrawWeekNumbers(&dcTemp);

				// blit to screen
				dc.BitBlt(0, 0, rClient.right, rClient.bottom, &dcTemp, 0, 0, SRCCOPY);

				// cleanup
				dcTemp.SelectObject(pOld);

				return;
			}
		}
	}

	// else draw to default dc
	DefWindowProc(WM_PAINT, (WPARAM)(HDC)dc, 0);
}

BOOL CMonthCalCtrlEx::PreTranslateMessage(MSG* pMsg)
{
	// Redraw week numbers every time a timer message fires
	if (m_bWeekNumbers && (pMsg->hwnd == *this) && (pMsg->message == WM_TIMER))
	{
		PostMessage(WM_REDRAWWEEKNUMBERS);
	}

	return CMonthCalCtrl::PreTranslateMessage(pMsg);
}

LRESULT CMonthCalCtrlEx::OnRedrawWeekNumbers(WPARAM /*wp*/, LPARAM /*lp*/)
{
	InvalidateRect(GetWeekNumbersRect());

	return 0L;
}
