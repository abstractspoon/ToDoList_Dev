#if !defined(AFX_ICONBUTTON_H__4FA393FA_20E7_4C07_A682_5AA7A8306DF4__INCLUDED_)
#define AFX_ICONBUTTON_H__4FA393FA_20E7_4C07_A682_5AA7A8306DF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// colorbutton.h : header file
//

#include "custombutton.h"
#include "enimagelist.h"
#include "icon.h"

/////////////////////////////////////////////////////////////////////////////
// CIconButton window

class CIconButton : public CCustomButton
{
public:
	CIconButton(int nSize = 16);
	virtual ~CIconButton();

	void SetIcon(HICON hIcon, BOOL bCleanup = TRUE);

protected:
	CIcon m_icon;
	int m_nSize;

protected:
	virtual void DoExtraPaint(CDC* pDC, const CRect& rExtra);
	virtual void CalcExtraSpace(const CRect& rClient, CRect& rExtra) const;

protected:
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONBUTTON_H__4FA393FA_20E7_4C07_A682_5AA7A8306DF4__INCLUDED_)
