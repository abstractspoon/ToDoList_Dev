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
	CTDLViewTabControl();

public:
	BOOL AttachView(HWND hWnd, FTC_VIEW nView, LPCTSTR szLabel, HICON hIcon, void* pData = NULL, int nVertOffset = 0);
	BOOL DetachView(HWND hWnd);
	BOOL DetachView(FTC_VIEW nView);

	void Resize(const CRect& rect, CDeferWndMove* pDWM = NULL);

	BOOL SetActiveView(CWnd* pWnd, BOOL bNotify = FALSE);
	BOOL SetActiveView(FTC_VIEW nView, BOOL bNotify = FALSE);
	CWnd* GetActiveWnd() const;
	FTC_VIEW GetActiveView() const;
	void ActivateNextView(BOOL bNext = TRUE);
	
	HWND GetViewHwnd(FTC_VIEW nView) const;
	BOOL SetViewHwnd(FTC_VIEW nView, HWND hWnd);
	CString GetViewName(FTC_VIEW nView) const;

	void* GetActiveViewData() const;
	void* GetViewData(FTC_VIEW nView) const;

	void ShowTabControl(BOOL bShow = TRUE);

	BOOL ShowViewTab(FTC_VIEW nView, BOOL bShow = TRUE);
	BOOL IsViewTabShowing(FTC_VIEW nView) const;

protected:
	struct TDCVIEW
	{
		TDCVIEW(HWND hWnd = NULL, FTC_VIEW view = FTCV_UNSET, LPCTSTR szLabel = NULL, HICON icon = NULL, void* data = NULL, int vertOffset = 0) 
			: 
			hwndView(hWnd), 
			nView(view), 
			sViewLabel(szLabel), 
			pData(data),
			hIcon(NULL),
			nVertOffset(vertOffset)
		{
			// add space for close button
			sViewLabel += "   ";

			if (icon)
				hIcon = ::CopyIcon(icon);
		}

		HWND hwndView;
		FTC_VIEW nView;
		CString sViewLabel;
		HICON hIcon;
		void* pData;
		int nVertOffset;
	};

	CArray<TDCVIEW, TDCVIEW&> m_aViews;
	int m_nSelTab;
	BOOL m_bShowingTabs;
	CRect m_rOverall; // tabs plus views

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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

	int FindView(HWND hWnd) const;
	int FindView(FTC_VIEW nView) const;
	int FindTab(FTC_VIEW nView) const;
	CWnd* GetViewWnd(const TDCVIEW& view) const;
	FTC_VIEW GetView(int nIndex) const;
	FTC_VIEW GetTabView(int nTab) const;
	void GetViewRect(const TDCVIEW& view, CRect& rView) const;
	BOOL SwitchToTab(int nNewIndex);
	BOOL CalcTabViewRects(const CRect& rPos, CRect& rTabs, CRect& rView) const;
	BOOL DoTabChange(int nOldTab, int nNewTab, BOOL bNotify);

	int IndexToTab(int nIndex) const;
	int TabToIndex(int nTab) const;

	// Virtual overrides
	BOOL WantTabCloseButton(int nTab) const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TODOCTRLVIEWTABCONTROL_H__4829AF79_C5C1_4D5F_8AB1_A6D0FFD5793E__INCLUDED_)
