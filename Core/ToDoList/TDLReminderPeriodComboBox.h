#pragma once

#include "..\Shared\ownerdrawcomboboxbase.h"

// CTDLReminderleadinComboBox

enum 
{
	TDLRPC_SHOWZERO	= 0x01,
	TDLRPC_SHOWNONE	= 0x02,
};

const UINT TDLRPC_NOREMINDER = (UINT)-1; // for SetSelectedLeadin

class CTDLReminderPeriodComboBox : public COwnerdrawComboBoxBase
{
	DECLARE_DYNAMIC(CTDLReminderPeriodComboBox)

public:
	CTDLReminderPeriodComboBox(DWORD dwShow = 0);
	virtual ~CTDLReminderPeriodComboBox();

	BOOL SetSelectedPeriod(UINT nMinutes);
	int GetSelectedPeriod() const;

	void DDX(CDataExchange* pDX, int& nMinutes);

protected:
	DWORD m_dwShow;

protected:
	void PreSubclassWindow();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

protected:
	void BuildCombo();
	void ValidateLeadin(UINT& nMinutes);

	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
		DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);
};


