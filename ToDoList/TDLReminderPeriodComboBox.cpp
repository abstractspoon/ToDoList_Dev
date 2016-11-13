// TDLReminderleadinComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "TDLReminderPeriodComboBox.h"

#include "..\shared\DialogHelper.h"

/////////////////////////////////////////////////////////////////////////////

// CTDLReminderleadinComboBox
struct LEADINDATA
{
	LPCTSTR szItemText;
	UINT nItemData; // minutes
};

const UINT ONE_HOUR		= 60;
const UINT ONE_DAY		= ONE_HOUR * 24;
const UINT ONE_WEEK		= ONE_DAY * 7;
const UINT ONE_MONTH	= ONE_DAY * 30;
const UINT ONE_YEAR		= ONE_DAY * 365;

// We can get away with non-resourced strings here because
// ToDoList uses a dynamic runtime translation system
static LEADINDATA data[] = 
{
	{ _T("<none>"),				TDLRPC_NOREMINDER },
	{ _T("0 minutes"),			0 },
	{ _T("5 minutes"),			5 },
	{ _T("10 minutes"),			10 },
	{ _T("15 minutes"),			15 },
	{ _T("20 minutes"),			20 },
	{ _T("30 minutes"),			30 },
	{ _T("45 minutes"),			45 },
	{ _T("60 minutes / 1 hour"),ONE_HOUR },
	{ _T("2 hours"),			2 * ONE_HOUR },
	{ _T("3 hours"),			3 * ONE_HOUR },
	{ _T("4 hours"),			4 * ONE_HOUR },
	{ _T("5 hours"),			5 * ONE_HOUR },
	{ _T("6 hours"),			6 * ONE_HOUR },
	{ _T("12 hours"),			12 * ONE_HOUR },
	{ _T("24 hours / 1 day"),	ONE_DAY },
	{ _T("2 days"),				2 * ONE_DAY },
	{ _T("3 days"),				3 * ONE_DAY },
	{ _T("4 days"),				4 * ONE_DAY },
	{ _T("5 days"),				5 * ONE_DAY },
	{ _T("6 days"),				6 * ONE_DAY },
	{ _T("7 days / 1 week"),	ONE_WEEK },
	{ _T("2 weeks"),			2 * ONE_WEEK },
	{ _T("3 weeks"),			3 * ONE_WEEK },
	{ _T("4 weeks"),			4 * ONE_WEEK },
	{ _T("1 month"),			ONE_MONTH },
	{ _T("2 months"),			2 * ONE_MONTH } ,
	{ _T("3 months"),			3 * ONE_MONTH },
	{ _T("4 months"),			4 * ONE_MONTH },
	{ _T("5 months"),			5 * ONE_MONTH },
	{ _T("6 months"),			6 * ONE_MONTH },
	{ _T("9 months"),			9 * ONE_MONTH },
	{ _T("12 months / 1 year"),	ONE_YEAR 	},
};
const UINT NUM_DATA = (sizeof(data) / sizeof(LEADINDATA));

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTDLReminderPeriodComboBox, CComboBox)

CTDLReminderPeriodComboBox::CTDLReminderPeriodComboBox(DWORD dwShow)
	:
	m_dwShow(dwShow)
{

}

CTDLReminderPeriodComboBox::~CTDLReminderPeriodComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLReminderPeriodComboBox, CComboBox)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CTDLReminderleadinComboBox message handlers

void CTDLReminderPeriodComboBox::PreSubclassWindow()
{
	CComboBox::PreSubclassWindow();

	// Remove CBS_SORT before populating
	ModifyStyle(CBS_SORT, 0, 0);

	BuildCombo();
}

int CTDLReminderPeriodComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// Remove CBS_SORT
	lpCreateStruct->style &= ~CBS_SORT;

	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// else
	BuildCombo();
	return 0;
}

void CTDLReminderPeriodComboBox::BuildCombo()
{
	ASSERT(GetSafeHwnd());

	// once only
	if (GetCount() == 0)
	{
		for (int nData = 0; nData < NUM_DATA; nData++)
		{
			if (!(m_dwShow & TDLRPC_SHOWNONE) && (data[nData].nItemData == TDLRPC_NOREMINDER))
				continue;

			if (!(m_dwShow & TDLRPC_SHOWZERO) && (data[nData].nItemData == 0))
				continue;
			
			VERIFY(CDialogHelper::AddString(*this, 
											data[nData].szItemText, 
											data[nData].nItemData) != CB_ERR);
		}
	}
}

BOOL CTDLReminderPeriodComboBox::SetSelectedPeriod(UINT nMinutes)
{
	ValidateLeadin(nMinutes);

	return CDialogHelper::SelectItemByData(*this, nMinutes);
}

UINT CTDLReminderPeriodComboBox::GetSelectedPeriod() const
{
	return CDialogHelper::GetSelectedItemData(*this);
}

void CTDLReminderPeriodComboBox::ValidateLeadin(UINT& nMinutes)
{
	ASSERT ((m_dwShow & TDLRPC_SHOWNONE) || (nMinutes != TDLRPC_NOREMINDER));
	ASSERT ((m_dwShow & TDLRPC_SHOWZERO) || (nMinutes != 0));

	// ignore 'none'
	if ((m_dwShow & TDLRPC_SHOWNONE) && (nMinutes == TDLRPC_NOREMINDER))
		return;

	for (int nData = 1; nData < NUM_DATA; nData++)
	{
		if (data[nData].nItemData >= nMinutes)
		{
			nMinutes = data[nData].nItemData;
			break;
		}
	}
}
