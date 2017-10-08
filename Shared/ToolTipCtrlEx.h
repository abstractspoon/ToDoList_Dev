#if !defined(AFX_TOOLTIPCTRLEX_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_)
#define AFX_TOOLTIPCTRLEX_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaskCalendarCtrl.h : header file

/////////////////////////////////////////////////////////////////////////////

// For MFC non-extension Dlls using FilterToolTipMessage
const UINT WM_TTC_TOOLHITTEST = ::RegisterWindowMessage(_T("WM_TTC_TOOLHITTEST")); // WPARAM = Point, LPARAM = LPTOOLINFO

/////////////////////////////////////////////////////////////////////////////

class CToolTipCtrlEx : public CToolTipCtrl
{
	DECLARE_DYNAMIC(CToolTipCtrlEx)

public:
	CToolTipCtrlEx();
	virtual ~CToolTipCtrlEx();

	void RelayEvent(LPMSG lpMsg);
	void FilterToolTipMessage(MSG* pMsg);
	BOOL AdjustRect(LPRECT lprc, BOOL bLarger /*= TRUE*/) const;
	void Activate(BOOL bActivate);

	const TOOLINFO& GetLastHitToolInfo() const;

	static BOOL WantMessage(const MSG* pMsg);

protected:
	BOOL m_bUsingRelayEvent;
	int m_nLastHit;
	TOOLINFO m_tiLast;

protected:
	DECLARE_MESSAGE_MAP()

	static void InitToolInfo(TOOLINFO& ti, BOOL bInitSize);
	static int DoToolHitTest(CWnd* pOwner, CPoint point, TOOLINFO& ti);
	static BOOL IsTopParentActive(CWnd* pOwner);
	static HWND GetParentOwner(HWND hWnd);

};

#endif // AFX_TOOLTIPCTRLEX_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_