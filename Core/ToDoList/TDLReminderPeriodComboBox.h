#pragma once

/////////////////////////////////////////////////////////////////////////////

#include "TDCEnum.h"

#include "..\Shared\ownerdrawcomboboxbase.h"

/////////////////////////////////////////////////////////////////////////////

enum 
{
	TDLRPC_SHOWZERO	= 0x01,
	TDLRPC_SHOWNONE	= 0x02,
};

/////////////////////////////////////////////////////////////////////////////
// CTDLReminderleadinComboBox

class CTDLReminderPeriodComboBox : public COwnerdrawComboBoxBase
{
	DECLARE_DYNAMIC(CTDLReminderPeriodComboBox)

public:
	CTDLReminderPeriodComboBox(DWORD dwShow = 0);
	virtual ~CTDLReminderPeriodComboBox();

	BOOL SetSelectedPeriod(TDC_REMINDERPERIOD nPeriod);
	TDC_REMINDERPERIOD GetSelectedPeriod() const;

	void DDX(CDataExchange* pDX, TDC_REMINDERPERIOD& nPeriod);

protected:
	DWORD m_dwShow;

protected:
	DECLARE_MESSAGE_MAP()

protected:
	void ValidatePeriod(TDC_REMINDERPERIOD& nPeriod);

	virtual void BuildCombo();
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);
};


