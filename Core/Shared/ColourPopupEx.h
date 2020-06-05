#if !defined(AFX_COLOURPOPUPEX_H__B494C4B6_C416_4556_A7FC_283C6DE7744B__INCLUDED_)
#define AFX_COLOURPOPUPEX_H__B494C4B6_C416_4556_A7FC_283C6DE7744B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColourPopupEx.h : header file
//

#include "Subclass.h"

#include "..\3rdParty\ColourPopup.h"

/////////////////////////////////////////////////////////////////////////////
// CColourPopupEx window

class CColourPopupEx : public CColourPopup, public CSubclasser
{
// Construction
public:
	CColourPopupEx();
	CColourPopupEx(CPoint p, COLORREF crColour, CWnd* pParentWnd, UINT nID = 0, 
				LPCTSTR szDefaultText = NULL, LPCTSTR szCustomText = NULL);

protected:
	CSubclassWnd   m_scParent;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColourPopupEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColourPopupEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColourPopupEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()

protected:
	virtual LRESULT ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOURPOPUPEX_H__B494C4B6_C416_4556_A7FC_283C6DE7744B__INCLUDED_)
