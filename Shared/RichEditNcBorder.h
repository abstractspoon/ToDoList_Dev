// RichEditNcBorder.h: interface for the CRichEditNcBorder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RICHEDITNCBORDER_H__AB98DDF6_FE73_4518_B805_DCF65A954860__INCLUDED_)
#define AFX_RICHEDITNCBORDER_H__AB98DDF6_FE73_4518_B805_DCF65A954860__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Subclass.h"

class CRichEditNcBorder : public CSubclassWnd  
{
public:
	CRichEditNcBorder();
	virtual ~CRichEditNcBorder();

	BOOL Initialize(HWND hwndRichEdit);

protected:
	BOOL m_bThemedBorder;

protected:
	virtual LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);

	void NcDrawBorder();
	void VerifyThemedBorderState(/*BOOL bRecheck*/);

};

#endif // !defined(AFX_RICHEDITNCBORDER_H__AB98DDF6_FE73_4518_B805_DCF65A954860__INCLUDED_)
