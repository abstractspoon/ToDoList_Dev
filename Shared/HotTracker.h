// HotTracker.h: interface for the CHotTracker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOTTRACKER_H__5F3B72B7_716D_47A0_B2A3_8815CE3FEBBE__INCLUDED_)
#define AFX_HOTTRACKER_H__5F3B72B7_716D_47A0_B2A3_8815CE3FEBBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Subclass.h"
#include <afxtempl.h>

const UINT WM_HTHOTCHANGE = ::RegisterWindowMessage(_T("WM_HTHOTCHANGE"));

class CHotTracker : public CSubclassWnd  
{
public:
	CHotTracker();
	virtual ~CHotTracker();

	BOOL Initialize(CWnd* pWnd);
	BOOL IsInitialized() const { return IsHooked(); }
	void Reset();

	int AddRect(const CRect& rect);
	int AddRect(); // adds a placeholder
	BOOL UpdateRect(int nRect, const CRect& rect);
	int GetRectCount() { return m_aRects.GetSize(); }
	BOOL GetRect(int nRect, CRect& rect);

protected:
	CArray<CRect, CRect&> m_aRects;
	int m_nHotRect;

protected:
	int HitTest(CPoint ptScreen);
	virtual LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);
	void InitTracking();

};

#endif // !defined(AFX_HOTTRACKER_H__5F3B72B7_716D_47A0_B2A3_8815CE3FEBBE__INCLUDED_)
