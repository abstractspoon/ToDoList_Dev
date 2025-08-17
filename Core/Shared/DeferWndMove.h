// MultiWndMove.h: interface for the CDeferWndMove class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEFERWNDMOVE_H__4D6EF0E2_B8BF_11D3_ADD8_0000E8425C3E__INCLUDED_)
#define AFX_DEFERWNDMOVE_H__4D6EF0E2_B8BF_11D3_ADD8_0000E8425C3E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDeferWndMove  
{
public:
	CDeferWndMove(int nNumWindows = 10);
	virtual ~CDeferWndMove();

	BOOL BeginMove(int nNumWindows);
	BOOL EndMove();

	BOOL MoveWindow(CWnd* pWnd, int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	BOOL MoveWindow(CWnd* pWnd, LPCRECT lpRect, BOOL bRepaint = TRUE);

	CRect OffsetCtrl(const CWnd* pParent, UINT nCtrlID, int x, int y);
	CRect ResizeCtrl(const CWnd* pParent, UINT nCtrlID, int cx, int cy);
	CRect MoveCtrl(const CWnd* pParent, UINT nCtrlID, int x, int y);

	CRect OffsetChild(CWnd* pChild, int x = 0, int y = 0);
	CRect ResizeChild(CWnd* pChild, int cx = 0, int cy = 0);
	CRect MoveChild(CWnd* pChild, int x, int y);

protected:
	HDWP m_hdwp;

protected:
	CRect OffsetCtrl(const CWnd* pParent, CWnd* pChild, int x, int y);
	CRect ResizeCtrl(const CWnd* pParent, CWnd* pChild, int cx, int cy);
	CRect MoveCtrl(const CWnd* pParent, CWnd* pChild, int x, int y);
};

#endif // !defined(AFX_MULTIWNDMOVE_H__4D6EF0E2_B8BF_11D3_ADD8_0000E8425C3E__INCLUDED_)
