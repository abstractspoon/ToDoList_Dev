#pragma once


// CMonthCalCtrlEx

class CMonthCalCtrlEx : public CMonthCalCtrl
{
// 	DECLARE_DYNAMIC(CMonthCalCtrlEx)

public:
	CMonthCalCtrlEx();
	virtual ~CMonthCalCtrlEx();

protected:
	CRect m_rWeekNumbers;
	BOOL m_bWeekNumbers;

protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	afx_msg void OnPaint();

	afx_msg LRESULT OnRedrawWeekNumbers(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

protected:
	const CRect& GetWeekNumbersRect();
	void DrawWeekNumbers(CDC* pDC);
	BOOL IsMonthView() const;
};


