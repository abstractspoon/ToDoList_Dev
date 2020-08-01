#if !defined(AFX_CTRLTEXTHIGHLIGHTER_H__19D0237F_28DC_4B45_9762_DF9B4F7D8492__INCLUDED_)
#define AFX_CTRLTEXTHIGHLIGHTER_H__19D0237F_28DC_4B45_9762_DF9B4F7D8492__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Subclass.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CCtrlTextSearcher

class CCtrlTextHighlighter : CSubclassWnd
{
public:
	CCtrlTextHighlighter();
	virtual ~CCtrlTextHighlighter();
	
	BOOL HighlightUIText(CWnd* pWnd, const CStringArray& aSearch, COLORREF crHighlight, const CWnd* pWndIgnore = NULL);
	void ClearHighlights();
	CWnd* GetFirstHighlightedItem() const;
	
	static BOOL TextContainsOneOf(const CString& sUIText, const CStringArray& aSearch);

protected:
	CArray<HWND, HWND&> m_aHighlightedCtrls;
	HBRUSH m_brHighlight;
	COLORREF m_crHighlight;

protected:
	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);

	BOOL GetHighlightRect(HWND hwnd, CRect& rHighlight) const;
	
	// This searches 'pWnd', its children, grand-children, etc
	static int FindMatchingCtrls(const CWnd* pWnd, const CStringArray& aSearch, CArray<HWND, HWND&>& aMatching, const CWnd* pWndIgnore = NULL);

	// This checks 'pWnd only'
	static BOOL TextContainsOneOf(const CWnd* pWnd, const CStringArray& aSearch);
};


#endif // !defined(AFX_CTRLTEXTHIGHLIGHTER_H__19D0237F_28DC_4B45_9762_DF9B4F7D8492__INCLUDED_)
