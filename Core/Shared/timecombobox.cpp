// timecombobox.cpp : implementation file
//

#include "stdafx.h"
#include "timecombobox.h"
#include "timehelper.h"
#include "datehelper.h"
#include "misc.h"
#include "localizer.h"
#include "holdredraw.h"
#include "workingweek.h"

#include <math.h>

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTimeComboBox

CTimeComboBox::CTimeComboBox(DWORD dwStyles) : m_dwStyle(dwStyles) 
{
}

CTimeComboBox::~CTimeComboBox()
{
}

BEGIN_MESSAGE_MAP(CTimeComboBox, COwnerdrawComboBoxBase)
	ON_WM_CREATE()
	ON_WM_CAPTURECHANGED()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimeComboBox message handlers

void CTimeComboBox::PreSubclassWindow() 
{
	BuildCombo();

	COwnerdrawComboBoxBase::PreSubclassWindow();
}

int CTimeComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COwnerdrawComboBoxBase::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	BuildCombo();
	return 0;
}

void CTimeComboBox::BuildCombo(BOOL bReset)
{
	CLocalizer::EnableTranslation(*this, FALSE);
	
	ASSERT(bReset || GetCount() == 0);
	
	if (!bReset && GetCount())
		return;
	
	ResetContent();
	
	for (int nHour = 0; nHour < 24; nHour++)
	{
		CString sTime;
		
		if (!(m_dwStyle & TCB_NOTIME) || (nHour > 0))
			sTime = CTimeHelper::FormatClockTime(nHour, 0, 0, FALSE, (m_dwStyle & TCB_ISO));
		
		AddString(sTime);
		
		if (m_dwStyle & TCB_HALFHOURS)
		{
			sTime = CTimeHelper::FormatClockTime(nHour, 30, 0, FALSE, (m_dwStyle & TCB_ISO));
			AddString(sTime);
		}
	}
}

void CTimeComboBox::DDX(CDataExchange* pDX, double& dHours)
{
	if (pDX->m_bSaveAndValidate)
		dHours = Get24HourTime();
	else
		Set24HourTime(dHours);
}

void CTimeComboBox::DDX(CDataExchange* pDX, COleDateTime& dtTime)
{
	double dTime = CDateHelper::GetTimeOnly(dtTime);

	if (pDX->m_bSaveAndValidate)
	{
		double dDays = CDateHelper::GetDateOnly(dtTime);
		double dTime = GetOleTime();

		dtTime = CDateHelper::MakeDate(dDays, dTime);
	}
	else
	{
		SetOleTime(CDateHelper::GetTimeOnly(dtTime));
	}
}

double CTimeComboBox::GetOleTime() const
{
	return (Get24HourTime() / 24.0);
}

BOOL CTimeComboBox::SetOleTime(double dTime)
{
	// truncate to extract the time only component if it has one
	dTime = fabs(dTime - (int)dTime);

	if (dTime <= 0)
	{
		if (m_dwStyle & TCB_NOTIME)
			SetCurSel(0);
		else
			SetCurSel(CB_ERR);

		return TRUE;
	}
	
	// else
	return Set24HourTime(dTime * 24.0);
}

void CTimeComboBox::SetISOFormat(BOOL bISO)
{
	DWORD dwNewStyle = m_dwStyle;
	Misc::SetFlag(dwNewStyle, TCB_ISO, bISO);

	if (dwNewStyle != m_dwStyle)
	{
		m_dwStyle = dwNewStyle;

		if (GetSafeHwnd())
		{
			double date = GetOleTime();

			BuildCombo(TRUE);
			SetOleTime(date);
		}
	}
}

CString CTimeComboBox::GetCurrentTime() const
{
	// is this a hack? I'm not sure
	// but we look at the current Windows message being handled and if it's a
	// combo sel change then we use the current selection else we use the
	// window text (it may be an edit change notification)
	CString sTime;
	const MSG* pMsg = CWnd::GetCurrentMessage();

	if ((pMsg->message == WM_COMMAND) && 
		(pMsg->lParam == (LPARAM)GetSafeHwnd()) &&
		((HIWORD(pMsg->wParam) == CBN_SELCHANGE) || (HIWORD(pMsg->wParam) == CBN_SELENDOK)))
	{
		GetLBText(GetCurSel(), sTime);
	}
	else
	{
		GetWindowText(sTime);
	}
	
	return Misc::Trim(sTime);
}

double CTimeComboBox::Get24HourTime() const
{
	CString sTime = GetCurrentTime();

	if (sTime.IsEmpty())
		return 0.0; // no time

	// else
	return CTimeHelper::DecodeClockTime(sTime);
}

double CTimeComboBox::Get24HourTime(int nItem) const
{
	if (nItem < 0) // 'no time'
		return 0.0;

	// since the items in the combo are ordered from 1am to 11pm
	// we can use the selection index as a direct link to the hour
	if (m_dwStyle & TCB_HALFHOURS)
		return min(24.0, (nItem * 0.5));

	// else
	return min(24.0, nItem);
}

BOOL CTimeComboBox::Set24HourTime(double dTime)
{
	if (dTime < 0)
	{
		SetCurSel(0);
		return TRUE;
	}
	
	// else
	if (dTime >= 24.0)
		return FALSE;

	// add on half a minute to handle floating point inaccuracies
	dTime += (0.5 / (24 * 60));

	int nHour = (int)dTime;
	int nMin = (int)((dTime - nHour) * 60); // nearest 1 minute

	CString sTime = CTimeHelper::FormatClockTime(nHour, nMin, 0, FALSE, (m_dwStyle & TCB_ISO));

	if (SelectString(-1, sTime) == CB_ERR)
		SetWindowText(sTime);

	return TRUE;
}

LRESULT CTimeComboBox::OnEditboxMessage(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CHAR:
		// Convert comma and period to time separator
		if ((wp == ',') || (wp == '.'))
		{
			CString sSep = Misc::GetTimeSeparator();

			if (!sSep.IsEmpty())
				wp = sSep[0];
		}
		break;
	}

	return COwnerdrawComboBoxBase::OnEditboxMessage(msg, wp, lp);
}

void CTimeComboBox::OnCaptureChanged(CWnd* pWnd)
{
	// Receiving this message tells us that
	// the listbox is now fully visible
	COwnerdrawComboBoxBase::OnCaptureChanged(pWnd);

	ScrollListBox();
}

void CTimeComboBox::ScrollListBox()
{
	// Scroll to beginning of working day unless the
	// current time is earlier than that
	const int STARTOFWORKDAY = 7;

	double dCurTime = Get24HourTime();

	if ((dCurTime == 0.0) || (dCurTime > STARTOFWORKDAY))
	{
		int nCurScrollPos = ::GetScrollPos(GetListbox(), SB_VERT);
		int nMinScrollPos = STARTOFWORKDAY; 

		if (m_dwStyle & TCB_HALFHOURS)
			nMinScrollPos *= 2;

		if (nMinScrollPos > nCurScrollPos)
		{
			CHoldRedraw hr(GetListbox());
			m_scList.SendMessage(WM_VSCROLL, MAKEWPARAM(SB_THUMBPOSITION, nMinScrollPos), 0);
		}
	}
}

void CTimeComboBox::GetItemColors(int nItem, UINT nItemState, DWORD dwItemData, 
								COLORREF& crText, COLORREF& crBack) const
{
	COwnerdrawComboBoxBase::GetItemColors(nItem, nItemState, dwItemData, crText, crBack);

	if ((m_dwStyle & TCB_HOURSINDAY) && 
		(nItem != CB_ERR) && 
		!(nItemState & (ODS_SELECTED | ODS_GRAYED | ODS_DISABLED)) &&
		!Misc::IsHighContrastActive())
	{
		double dTime = Get24HourTime(nItem);
		CWorkingDay wd;

		if ((dTime >= 0) &&
			((dTime < wd.GetStartOfDayInHours()) ||	(dTime > wd.GetEndOfDayInHours())))
		{
			crBack = GetSysColor(COLOR_3DFACE);
			// leave text colour as-is
		}
	}
}
