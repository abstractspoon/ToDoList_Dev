#if !defined(AFX_WINHELPBUTTON_H__4FA393FA_20E7_4C07_A682_5AA7A8306DF4__INCLUDED_)
#define AFX_WINHELPBUTTON_H__4FA393FA_20E7_4C07_A682_5AA7A8306DF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WinHelpButton.h : header file
//

#include "iconbutton.h"

/////////////////////////////////////////////////////////////////////////////
// CWinHelpButton window

class CWinHelpButton : public CIconButton
{
public:
	CWinHelpButton(UINT nHelpID = 0, BOOL bAutoHandleClick = TRUE);
	virtual ~CWinHelpButton();

	BOOL Create(UINT nID, CWnd* pParent, const CRect& rPos = CRect(0, 0, 0, 0));
	void UpdatePosition();

	UINT GetHelpID() const { return m_nHelpID; }
	void SetHelpID(UINT nHelpID) { m_nHelpID = nHelpID; }

	static BOOL SetDefaultIcon(HICON hIcon);
	static BOOL SetDefaultTooltip(LPCTSTR szTooltip);

protected:
	UINT m_nHelpID;
	BOOL m_bAutoHandleClick;
	BOOL m_bAutoPosition;

	static HICON s_hDefIcon;
	static CString s_sDefTooltip;

protected:
	virtual BOOL DoAction();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

protected:
	static BOOL GetAutoPosition(const CWnd* pParent, CRect& rPos);
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_WINHELPBUTTON_H__4FA393FA_20E7_4C07_A682_5AA7A8306DF4__INCLUDED_)
