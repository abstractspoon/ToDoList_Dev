// TDLReminderleadinComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "TDLReminderPeriodComboBox.h"

#include "..\shared\DialogHelper.h"
#include "..\shared\Wndprompt.h"

/////////////////////////////////////////////////////////////////////////////

struct PERIODDATA
{
	LPCTSTR szItemText;
	TDC_REMINDERPERIOD nPeriod;
};

// --------------------------------------

static PERIODDATA data[] = 
{
	{ _T("<none>"),				TDCRP_NOREMINDER },
	{ _T("0 minutes"),			TDCRP_0_MINS },
	{ _T("5 minutes"),			TDCRP_5_MINS },
	{ _T("10 minutes"),			TDCRP_10_MINS },
	{ _T("15 minutes"),			TDCRP_15_MINS },
	{ _T("20 minutes"),			TDCRP_20_MINS },
	{ _T("30 minutes"),			TDCRP_30_MINS },
	{ _T("45 minutes"),			TDCRP_45_MINS },
	{ _T("60 minutes / 1 hour"),TDCRP_1_HOUR },
	{ _T("2 hours"),			TDCRP_2_HOURS },
	{ _T("3 hours"),			TDCRP_3_HOURS },
	{ _T("4 hours"),			TDCRP_4_HOURS },
	{ _T("5 hours"),			TDCRP_5_HOURS },
	{ _T("6 hours"),			TDCRP_6_HOURS },
	{ _T("12 hours"),			TDCRP_12_HOURS },
	{ _T("24 hours / 1 day"),	TDCRP_1_DAY },
	{ _T("2 days"),				TDCRP_2_DAYS },
	{ _T("3 days"),				TDCRP_3_DAYS },
	{ _T("4 days"),				TDCRP_4_DAYS },
	{ _T("5 days"),				TDCRP_5_DAYS },
	{ _T("6 days"),				TDCRP_6_DAYS },
	{ _T("7 days / 1 week"),	TDCRP_1_WEEK },
	{ _T("2 weeks"),			TDCRP_2_WEEKS },
	{ _T("3 weeks"),			TDCRP_3_WEEKS },
	{ _T("4 weeks"),			TDCRP_4_WEEKS },
	{ _T("1 month"),			TDCRP_1_MONTH },
	{ _T("2 months"),			TDCRP_2_MONTHS } ,
	{ _T("3 months"),			TDCRP_3_MONTHS },
	{ _T("4 months"),			TDCRP_4_MONTHS },
	{ _T("5 months"),			TDCRP_5_MONTHS },
	{ _T("6 months"),			TDCRP_6_MONTHS },
	{ _T("9 months"),			TDCRP_9_MONTHS },
	{ _T("12 months / 1 year"),	TDCRP_1_YEAR },
};
const UINT NUM_DATA = (sizeof(data) / sizeof(PERIODDATA));

/////////////////////////////////////////////////////////////////////////////
// CTDLReminderleadinComboBox

IMPLEMENT_DYNAMIC(CTDLReminderPeriodComboBox, COwnerdrawComboBoxBase)

CTDLReminderPeriodComboBox::CTDLReminderPeriodComboBox(DWORD dwShow)
	:
	m_dwShow(dwShow)
{

}

CTDLReminderPeriodComboBox::~CTDLReminderPeriodComboBox()
{
}

BEGIN_MESSAGE_MAP(CTDLReminderPeriodComboBox, COwnerdrawComboBoxBase)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CTDLReminderPeriodComboBox::BuildCombo()
{
	ASSERT(GetSafeHwnd());
	ASSERT(GetCount() == 0);
	ASSERT(!HasStyle(CBS_SORT));

	for (int nData = 0; nData < NUM_DATA; nData++)
	{
		if (!(m_dwShow & TDLRPC_SHOWNONE) && (data[nData].nPeriod == TDCRP_NOREMINDER))
			continue;

		if (!(m_dwShow & TDLRPC_SHOWZERO) && (data[nData].nPeriod == TDCRP_0_MINS))
			continue;

		VERIFY(CDialogHelper::AddStringT(*this,
										 data[nData].szItemText,
										 data[nData].nPeriod) != CB_ERR);
	}
}

BOOL CTDLReminderPeriodComboBox::SetSelectedPeriod(TDC_REMINDERPERIOD nPeriod)
{
	CheckBuildCombo();
	ValidatePeriod(nPeriod);

	return (CB_ERR != CDialogHelper::SelectItemByDataT(*this, nPeriod));
}

TDC_REMINDERPERIOD CTDLReminderPeriodComboBox::GetSelectedPeriod() const
{
	return CDialogHelper::GetSelectedItemDataT(*this, TDCRP_15_MINS);
}

void CTDLReminderPeriodComboBox::ValidatePeriod(TDC_REMINDERPERIOD& nPeriod)
{
	ASSERT ((m_dwShow & TDLRPC_SHOWNONE) || (nPeriod != TDCRP_NOREMINDER));
	ASSERT ((m_dwShow & TDLRPC_SHOWZERO) || (nPeriod != TDCRP_0_MINS));

	// ignore 'none'
	if ((m_dwShow & TDLRPC_SHOWNONE) && (nPeriod == TDCRP_NOREMINDER))
		return;

	for (int nData = 1; nData < NUM_DATA; nData++)
	{
		if (data[nData].nPeriod >= nPeriod)
		{
			nPeriod = data[nData].nPeriod;
			break;
		}
	}
}

void CTDLReminderPeriodComboBox::DDX(CDataExchange* pDX, TDC_REMINDERPERIOD& nPeriod)
{
	if (pDX->m_bSaveAndValidate)
		nPeriod = GetSelectedPeriod();
	else
		SetSelectedPeriod(nPeriod);
}

void CTDLReminderPeriodComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
	DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	// Draw <none> in window prompt color
	if ((dwItemData == TDCRP_NOREMINDER) && !(nItemState & ODS_SELECTED) && !bList)
	{
		crText = CWndPrompt::GetTextColor();
	}

	// all else
	COwnerdrawComboBoxBase::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sItem, bList, crText);
}
