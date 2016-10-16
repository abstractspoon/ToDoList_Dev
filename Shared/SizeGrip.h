#if !defined(AFX_SIZEGRIP_H__00B91A71_B758_4BF6_88AA_9B3860864023__INCLUDED_)
#define AFX_SIZEGRIP_H__00B91A71_B758_4BF6_88AA_9B3860864023__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SizeGrip.h : header file
//

#include "subclass.h"
#include "enbitmap.h"

/////////////////////////////////////////////////////////////////////////////
// CSizeGrip window

class CSizeGrip : protected CScrollBar, protected CSubclasser
{
// Construction
public:
	CSizeGrip();

	BOOL Initialize(UINT nCtrlID, CWnd* pParent, UINT nBitmapID = 0);

// Attributes
protected:
	CEnBitmap m_bm;
	CSubclassWnd m_scParent;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSizeGrip)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSizeGrip();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSizeGrip)
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	virtual LRESULT ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIZEGRIP_H__00B91A71_B758_4BF6_88AA_9B3860864023__INCLUDED_)
