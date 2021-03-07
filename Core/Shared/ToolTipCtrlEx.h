#if !defined(AFX_TOOLTIPCTRLEX_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_)
#define AFX_TOOLTIPCTRLEX_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaskCalendarCtrl.h : header file

#include "Subclass.h"

/////////////////////////////////////////////////////////////////////////////

// For MFC non-extension Dlls using FilterToolTipMessage
const UINT WM_TTC_TOOLHITTEST = ::RegisterWindowMessage(_T("WM_TTC_TOOLHITTEST")); // WPARAM = Point, LPARAM = LPTOOLINFO

/////////////////////////////////////////////////////////////////////////////

class CToolTipCtrlEx : public CToolTipCtrl, protected CSubclasser
{
	DECLARE_DYNAMIC(CToolTipCtrlEx)

public:
	CToolTipCtrlEx();
	virtual ~CToolTipCtrlEx();

	void RelayEvent(LPMSG lpMsg);
	void FilterToolTipMessage(MSG* pMsg, BOOL bSendHitTestMessage = FALSE);
	BOOL AdjustRect(LPRECT lprc, BOOL bLarger /*= TRUE*/) const;
	void Activate(BOOL bActivate);
	void EnableTracking(BOOL bTracking = TRUE, int nXOffset = 0, int nYOffset = 0);
	BOOL IsTracking() const;

	const TOOLINFO& GetLastHitToolInfo() const;

	static BOOL IsKeypress(UINT nMsgID);
	static BOOL IsMouseDown(UINT nMsgID);
	static BOOL WantMessage(UINT nMsgID);

	static int SetToolInfo(TOOLINFO& ti, const CWnd* pWnd, const CString sTooltip, int nID, LPCRECT pBounds = NULL, UINT nFlags = TTF_TRANSPARENT | TTF_NOTBUTTON);
	static int SetToolInfo(TOOLINFO& ti, HWND hWnd, const CString sTooltip, int nID, LPCRECT pBounds = NULL, UINT nFlags = TTF_TRANSPARENT | TTF_NOTBUTTON);

	static UINT GetCtrlID(const TOOLTIPTEXT* pTTT);

protected:
	BOOL m_bUsingRelayEvent;
	CPoint m_ptTrackingOffset;
	CSubclassWnd m_scTracking;

	int m_nLastHit;
	TOOLINFO m_tiLast;
	CSize m_sizeTooltip;

protected:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()

protected:
	virtual LRESULT ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);

	CPoint FitTooltipRectToScreen(const CRect& rTooltip) const;

	static void InitToolInfo(TOOLINFO& ti, BOOL bInitSize);
	static int DoToolHitTest(CWnd* pOwner, CPoint point, TOOLINFO& ti, BOOL bSendHitTestMessage);
	static BOOL IsTopParentActive(CWnd* pOwner);
	static HWND GetParentOwner(HWND hWnd);
};

#endif // AFX_TOOLTIPCTRLEX_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_