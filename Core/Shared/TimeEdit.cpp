// TimeEdit.cpp: implementation of the CTimeEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TimeEdit.h"
#include "stringres.h"
#include "misc.h"
#include "localizer.h"
#include "dialoghelper.h"

#include <math.h>

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////

CString CTimeEdit::s_sUnitsBtnTip;
BOOL CTimeEdit::s_bUnitsTranslated = FALSE;

//////////////////////////////////////////////////////////////////////

const int BTNWIDTHDLU = 11;
const int LABELLEN = 30;
const int TEBTN_UNITS = 1;

//////////////////////////////////////////////////////////////////////

struct TIMEUNIT
{
	TH_UNITS nUnits;
	TCHAR szLabel[LABELLEN];
	TCHAR cAbbrLabel;
	UINT nMenuID;
};

enum
{
	ID_MINS = 0x8000,
	ID_HOURS,
	ID_WEEKDAYS,
	ID_DAYS,
	ID_WEEKS,
	ID_MONTHS,
	ID_YEARS,
};

static TIMEUNIT TIMEUNITS[] = 
{
	{ THU_MINS,		_T(""),	TIME_MIN_ABBREV,	ID_MINS },
	{ THU_HOURS,	_T(""),	TIME_HOUR_ABBREV,	ID_HOURS },
	{ THU_WEEKDAYS,	_T(""),	TIME_WEEKDAY_ABBREV,ID_WEEKDAYS },
	{ THU_DAYS,		_T(""),	TIME_DAY_ABBREV,	ID_DAYS },
	{ THU_WEEKS,	_T(""),	TIME_WEEK_ABBREV,	ID_WEEKS },
	{ THU_MONTHS,	_T(""), TIME_MONTH_ABBREV,	ID_MONTHS },
	{ THU_YEARS,	_T(""),	TIME_YEAR_ABBREV,	ID_YEARS },
};

static LPCTSTR UNITLABELS[] = 
{
	TIME_MINS,	
	TIME_HOURS,	
	TIME_WEEKDAYS,
	TIME_DAYS,	
	TIME_WEEKS,	
	TIME_MONTHS,
	TIME_YEARS	
};

const int NUM_UNITS = sizeof(TIMEUNITS) / sizeof (TIMEUNIT);

const TIMEUNIT& GetTimeUnit(TH_UNITS nUnits)
{
	int nItem = NUM_UNITS;

	while (nItem--)
	{
		if (TIMEUNITS[nItem].nUnits == nUnits)
			return TIMEUNITS[nItem];
	}

	return TIMEUNITS[0]; // hours
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////////////////////

CTimeEdit::CTimeEdit(TH_UNITS nUnits, int nMaxDecPlaces) : m_nUnits(nUnits), m_nMaxDecPlaces(nMaxDecPlaces)
{
	// init static units
	for (int nUnit = 0; nUnit < NUM_UNITS; nUnit++)
	{
		TIMEUNIT& tu = TIMEUNITS[nUnit];

		if (Misc::IsEmpty(tu.szLabel))
			lstrcpyn(tu.szLabel, UNITLABELS[nUnit], (LABELLEN - 1));
	}

	SetMask(_T(".0123456789"), ME_LOCALIZEDECIMAL);

	AddButton(TEBTN_UNITS, _T(""), _T(""), CALC_BTNWIDTH);
	SetDropMenuButton(TEBTN_UNITS);

	UpdateButtonText(nUnits);
}

CTimeEdit::~CTimeEdit()
{

}

BEGIN_MESSAGE_MAP(CTimeEdit, CEnEdit)
	//{{AFX_MSG_MAP(CTimeEdit)
	//}}AFX_MSG_MAP
	ON_WM_CHAR()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void CTimeEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	VERIFY(CreateUnitsPopupMenu(FALSE));

	// first check for exact match with units
	// then try again without case
	for (int bIgnoreCase = 0; bIgnoreCase <= 1; bIgnoreCase++)
	{
		for (int nUnit = 0; nUnit < NUM_UNITS; nUnit++)
		{
			TIMEUNIT& tu = TIMEUNITS[nUnit];
			BOOL bMatch = FALSE;

			if (bIgnoreCase)
			{
				bMatch = (Misc::ToUpper(tu.cAbbrLabel) == Misc::ToUpper((TCHAR)nChar));
			}
			else // case sensitive
			{
				bMatch = (tu.cAbbrLabel == nChar);
			}
			
			if (bMatch)
			{
				CheckSetUnits(tu.nUnits, FALSE);
				return;
			}
		}
	}
	
	CEnEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CTimeEdit::SetDefaultButtonTip(LPCTSTR szUnits)
{
	if (!Misc::IsEmpty(szUnits))
		s_sUnitsBtnTip = szUnits;
}

void CTimeEdit::CheckSetUnits(TH_UNITS nUnits, BOOL bQueryUnits)
{
	if (nUnits != m_nUnits)
	{
		// update the units
		TH_UNITS nPrevUnits = m_nUnits;
		SetUnits(nUnits);
		
		// inform parent and check for cancel
		if (bQueryUnits)
		{
			if (GetParent()->SendMessage(WM_TEN_UNITSCHANGE, (WPARAM)GetDlgCtrlID(), nPrevUnits))
				SetUnits(nPrevUnits);
		}
		else // fake a time change
		{
			GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), EN_CHANGE), (LPARAM)GetSafeHwnd());
		}
	}
}

void CTimeEdit::EnableNegativeTimes(BOOL bEnable)
{
	if (bEnable)
		SetMask(_T("-.0123456789"), ME_LOCALIZEDECIMAL);
	else
		SetMask(_T(".0123456789"), ME_LOCALIZEDECIMAL);
}

void CTimeEdit::EnableUnitsChanges(BOOL bEnable)
{
	EnableButton(TEBTN_UNITS, bEnable);
}

void CTimeEdit::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (bShow)
	{
		UpdateButtonText(m_nUnits);
		SetTime(Convert());
	}
}

void CTimeEdit::UpdateButtonText(TH_UNITS nUnits)
{
	if (nUnits != THU_NULL)
	{
		const TIMEUNIT& tu = GetTimeUnit(nUnits);
		
		CString sLabel;
		sLabel.Format(_T("%c"), tu.cAbbrLabel);
		
		SetButtonCaption(TEBTN_UNITS, sLabel);
		SetButtonTip(TEBTN_UNITS, tu.szLabel);
	}
	else
	{
		SetButtonCaption(TEBTN_UNITS, _T(""));
		SetButtonTip(TEBTN_UNITS, (s_sUnitsBtnTip.IsEmpty() ? TIME_UNITS : s_sUnitsBtnTip));
	}
}

double CTimeEdit::Convert() const
{
	CString sTime;
	GetWindowText(sTime);
	return Misc::Atof(sTime);
}

void CTimeEdit::SetTime(double dTime)
{
	CString sTime = CTimeHelper::FormatTime(dTime, m_nMaxDecPlaces);
	RemoveTrailingZeros(sTime);

	SetWindowText(sTime);
}

void CTimeEdit::SetTime(double dTime, TH_UNITS nUnits)
{
	if (dTime != Convert())
		SetTime(dTime);

	SetUnits(nUnits);
}

void CTimeEdit::SetUnits(TH_UNITS nUnits)
{
	if (nUnits != m_nUnits)
	{
		m_nUnits = nUnits;
		UpdateButtonText(m_nUnits);
	}
}

void CTimeEdit::SetMaxDecimalPlaces(int nMaxDecPlaces)
{
	if (m_nMaxDecPlaces != nMaxDecPlaces)
	{
		m_nMaxDecPlaces = nMaxDecPlaces;

		SetTime(Convert());
	}
}

BOOL CTimeEdit::CreateUnitsPopupMenu(BOOL bUpdateState)
{
	ASSERT(GetSafeHwnd());

	// create once only
	if (!m_menuUnits.GetSafeHmenu() && m_menuUnits.CreatePopupMenu())
	{
		CString sMenuItem;
		
		for (int nUnit = 0; nUnit < NUM_UNITS; nUnit++)
		{
			const TIMEUNIT& tu = TIMEUNITS[nUnit];
			sMenuItem.Format(_T("%c\t%s"), tu.cAbbrLabel, tu.szLabel);
			
			m_menuUnits.AppendMenu(MF_STRING, tu.nMenuID, sMenuItem);
		}

		CLocalizer::EnableTranslation(m_menuUnits, FALSE);
	}

	// state
	if (bUpdateState)
		m_menuUnits.CheckMenuRadioItem(ID_MINS, ID_YEARS, MapUnitsToMenuID(), MF_BYCOMMAND);

	return (m_menuUnits.GetSafeHmenu() != NULL);
}

UINT CTimeEdit::MapUnitsToMenuID() const
{
	switch (m_nUnits)
	{
	case THU_NULL:		return 0;
	case THU_MINS:		return ID_MINS;
	case THU_HOURS:		return ID_HOURS;
	case THU_WEEKDAYS:	return ID_WEEKDAYS;
	case THU_DAYS:		return ID_DAYS;
	case THU_WEEKS:		return ID_WEEKS;
	case THU_MONTHS:	return ID_MONTHS;
	case THU_YEARS:		return ID_YEARS;
	}

	ASSERT(0);
	return 0;
}

void CTimeEdit::OnBtnClick(UINT nID)
{
	if (nID != TEBTN_UNITS)
		return;

	if (CreateUnitsPopupMenu(TRUE))
	{			
		UINT nCmdID = TrackPopupMenu(nID, &m_menuUnits, EETPM_RETURNCMD);

		if (nCmdID <= 0)
			return; // cancelled

		// handle result
		for (int nUnit = 0; nUnit < NUM_UNITS; nUnit++)
		{
			const TIMEUNIT& tu = TIMEUNITS[nUnit];

			if (tu.nMenuID == nCmdID)
			{
				// update the units
				CheckSetUnits(tu.nUnits, TRUE);
				break;
			}
		}
	}
}

double CTimeEdit::Convert(TH_UNITS nUnits) const
{
	return CTimeHelper().Convert(Convert(), m_nUnits, nUnits);
}

CString CTimeEdit::FormatTime(BOOL bUnits) const
{
	return CTimeHelper().FormatTime(Convert(), (bUnits ? m_nUnits : THU_NULL), m_nMaxDecPlaces);
}

CString CTimeEdit::FormatTimeHMS() const
{
	return CTimeHelper().FormatTimeHMS(Convert(), GetUnits(), TRUE); 
}

void CTimeEdit::OnSetReadOnly(BOOL bReadOnly)
{
	EnableButton(1, !bReadOnly && IsWindowEnabled());
}

void CTimeEdit::SetUnits(TH_UNITS nUnits, LPCTSTR szLongUnits, LPCTSTR szAbbrevUnits)
{
	for (int nUnit = 0; nUnit < NUM_UNITS; nUnit++)
	{
		TIMEUNIT& tu = TIMEUNITS[nUnit];

		if (tu.nUnits == nUnits)
		{
			if (!Misc::IsEmpty(szLongUnits))
			{
				lstrcpyn(tu.szLabel, szLongUnits, LABELLEN - 1);
				tu.szLabel[LABELLEN - 1] = 0;
			}

			if (!Misc::IsEmpty(szAbbrevUnits))
				tu.cAbbrLabel = szAbbrevUnits[0];
		}
	}

	// abbrev units
	CTimeHelper::SetUnits(nUnits, szAbbrevUnits);
}

void CTimeEdit::RemoveTrailingZeros(CString& sTime)
{
	sTime.TrimRight();

	while (!sTime.IsEmpty())
	{
		int nLen = sTime.GetLength();

		if (sTime[nLen - 1] == '0')
		{
			sTime = sTime.Left(nLen - 1);
		}
		else if (sTime[nLen - 1] == '.')
		{
			sTime = sTime.Left(nLen - 1);
			break;
		}
		else
		{
			break;
		}
	}
}

void CTimeEdit::DDX(CDataExchange* pDX, double& value, TH_UNITS& units, int nDecimals)
{
	CDialogHelper::DDX_Text(pDX, GetDlgCtrlID(), value, nDecimals);

	if (pDX->m_bSaveAndValidate)
		units = GetUnits();
	else
		SetUnits(units);
}

