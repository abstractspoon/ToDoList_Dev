#if !defined(AFX_TOOLTIPCTRLEX_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_)
#define AFX_TOOLTIPCTRLEX_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaskCalendarCtrl.h : header file

/////////////////////////////////////////////////////////////////////////////

class CToolTipCtrlEx : public CToolTipCtrl
{
	DECLARE_DYNAMIC(CToolTipCtrlEx)

public:
	CToolTipCtrlEx();
	virtual ~CToolTipCtrlEx();

	void RelayEvent(LPMSG lpMsg);
	void FilterToolTipMessage(MSG* pMsg);
	BOOL AdjustRect(LPRECT lprc, BOOL bLarger /*= TRUE*/);
	void Activate(BOOL bActivate);

	const TOOLINFO& GetLastHitToolInfo() const;

protected:
	BOOL m_bUsingRelayEvent;
	int m_nLastHit;
	TOOLINFO m_tiLast;

protected:
	DECLARE_MESSAGE_MAP()

	static void InitToolInfo(TOOLINFO& ti, BOOL bInitSize);
};

#endif // AFX_TOOLTIPCTRLEX_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_