#if !defined(AFX_TODOCTRLVIEWTABCONTROL_H__4829AF79_C5C1_4D5F_8AB1_A6D0FFD5793E__INCLUDED_)
#define AFX_TODOCTRLVIEWTABCONTROL_H__4829AF79_C5C1_4D5F_8AB1_A6D0FFD5793E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToDoCtrlViewTabControl.h : header file
//

#include <afxtempl.h>

#include "tdcenum.h"

#include "..\shared\tabctrlex.h"
#include "..\shared\enstring.h"

class CDeferWndMove;

/////////////////////////////////////////////////////////////////////////////
// CTDCViewTabControl window

class CTDLViewTabControl : public CTabCtrlEx
{
// Construction
public:
	CTDLViewTabControl(DWORD dwStyles = 0);

public:
	BOOL AttachView(HWND hWnd, FTC_VIEW nView, LPCTSTR szLabel, HICON hIcon, void* pData = NULL);
	BOOL DetachView(HWND hWnd);
	BOOL DetachView(FTC_VIEW nView);

	void Resize(const CRect& rect, CDeferWndMove* pDWM = NULL, LPRECT prcView = NULL);

	BOOL SetActiveView(CWnd* pWnd, BOOL bNotify = FALSE);
	BOOL SetActiveView(FTC_VIEW nView, BOOL bNotify = FALSE);
	CWnd* GetActiveWnd() const;
	FTC_VIEW GetActiveView() const;
	void ActivateNextView(BOOL bNext = TRUE);
	BOOL ShowViewTab(FTC_VIEW nView);
	
	HWND GetViewHwnd(FTC_VIEW nView) const;
	BOOL SetViewHwnd(FTC_VIEW nView, HWND hWnd);
	CString GetViewName(FTC_VIEW nView) const;

	void* GetActiveViewData() const;
	void* GetViewData(FTC_VIEW nView) const;

	DWORD ModifyStyles(DWORD dwStyle, BOOL bAdd = TRUE);
	void ShowTabControl(BOOL bShow = TRUE);

	BOOL ShowViewTab(FTC_VIEW nView, BOOL bShow = TRUE);
	BOOL IsViewTabShowing(FTC_VIEW nView) const;

protected:
	struct TDCVIEW
	{
		TDCVIEW(HWND hWnd = NULL, FTC_VIEW view = FTCV_UNSET, LPCTSTR szLabel = NULL, int nImg = -1, void* data = NULL) 
			: 
			hwndView(hWnd), 
			nView(view), 
			sViewLabel(szLabel), 
			pData(data),
			nImage(nImg)
		{
			// add space for close button
			sViewLabel += "   ";
		}

		HWND hwndView;
		FTC_VIEW nView;
		CString sViewLabel;
		int nImage;
		void* pData;
	};

	CArray<TDCVIEW, TDCVIEW&> m_aViews;
	DWORD m_dwStyles;
	int m_nSelTab;
	CImageList m_ilTabs;
	BOOL m_bShowingTabs;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDCViewTabControl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTDLViewTabControl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDCViewTabControl)
	//}}AFX_MSG
	afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCloseTab(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

	int FindView(HWND hWnd) const;
	int FindView(FTC_VIEW nView) const;
	int FindTab(FTC_VIEW nView) const;
	CWnd* GetViewWnd(TDCVIEW& view) const;
	FTC_VIEW GetView(int nIndex) const;
	FTC_VIEW GetTabView(int nTab) const;
	void GetViewRect(TDCVIEW& view, CRect& rPos) const;
	BOOL SwitchToTab(int nNewIndex);
	BOOL CalcTabViewRects(const CRect& rPos, CRect& rTabs, CRect& rView);

	int IndexToTab(int nIndex) const;
	int TabToIndex(int nTab) const;

	BOOL DoTabChange(int nOldTab, int nNewTab, BOOL bNotify);
	BOOL WantTabCloseButton(int nTab) const; // virtual
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TODOCTRLVIEWTABCONTROL_H__4829AF79_C5C1_4D5F_8AB1_A6D0FFD5793E__INCLUDED_)
