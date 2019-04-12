#pragma once


// CTDLReminderleadinComboBox

enum 
{
	TDLRPC_SHOWZERO	= 0x01,
	TDLRPC_SHOWNONE	= 0x02,
};

const UINT TDLRPC_NOREMINDER = (UINT)-1; // for SetSelectedLeadin

class CTDLReminderPeriodComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CTDLReminderPeriodComboBox)

public:
	CTDLReminderPeriodComboBox(DWORD dwShow = 0);
	virtual ~CTDLReminderPeriodComboBox();

	BOOL SetSelectedPeriod(UINT nMinutes);
	UINT GetSelectedPeriod() const;

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
};


