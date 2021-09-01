// TabbedPropertyPageHost.h: interface for the CScrollingPropertyPageHost class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCROLLINGPROPERTYPAGEHOST_H__4BC7E883_D652_462C_BB3A_6102BA978FBC__INCLUDED_)
#define AFX_SCROLLINGPROPERTYPAGEHOST_H__4BC7E883_D652_462C_BB3A_6102BA978FBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PropertyPageHost.h"

class CScrollingPropertyPageHost : public CPropertyPageHost  
{
public:
	CScrollingPropertyPageHost();
	virtual ~CScrollingPropertyPageHost();

	int GetScrollPos() const;
	BOOL ScrollTo(CWnd* pCtrl);
	BOOL ScrollToTop();

protected:
	CScrollBar m_scroll;

	// Generated message map functions
protected:
	//{{AFX_MSG(CScrollingPropertyPageHost)
	//}}AFX_MSG
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	DECLARE_MESSAGE_MAP()

protected:
	BOOL ConstructScrollbar();
	void UpdatePageSize(int nPage, BOOL bPageChange); // virtual
	BOOL ScrollTo(LONG nPos);

};

#endif // !defined(AFX_SCROLLINGPROPERTYPAGEHOST_H__4BC7E883_D652_462C_BB3A_6102BA978FBC__INCLUDED_)
