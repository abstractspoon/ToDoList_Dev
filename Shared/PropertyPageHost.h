#if !defined(AFX_PROPERTYPAGEHOST_H__43CF5AE7_C70B_443D_BC8B_7DA1D0E082DD__INCLUDED_)
#define AFX_PROPERTYPAGEHOST_H__43CF5AE7_C70B_443D_BC8B_7DA1D0E082DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPageHost.h : header file
//

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

struct PAGEITEM
{
	PAGEITEM(CPropertyPage* _pPage = NULL, LPCTSTR szTitle = NULL, DWORD dwData = 0) : 
			pPage(_pPage), sTitle(szTitle), dwItemData(dwData) {}

	CPropertyPage* pPage;
	CString sTitle;
	DWORD dwItemData;
	CSize sizeOrg;
};

/////////////////////////////////////////////////////////////////////////////

class CCmdNotifyPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCmdNotifyPropertyPage)

	// Construction
public:
	CCmdNotifyPropertyPage(UINT nIDTemplate, UINT nIDCaption = 0);
	CCmdNotifyPropertyPage(LPCTSTR lpszTemplateName, UINT nIDCaption = 0);

	virtual ~CCmdNotifyPropertyPage();

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageHost window

class CPropertyPageHost : public CWnd
{
// Construction
public:
	CPropertyPageHost();
	virtual ~CPropertyPageHost();

	BOOL Create(LPRECT lpRect, CWnd* pParent, UINT uCtrlID = AFX_IDW_PANE_FIRST);
	BOOL Create(UINT nRefCtrlID, CWnd* pParent, UINT uCtrlID = AFX_IDW_PANE_FIRST);
	void OnOK();
	void OnApply();
	
	int GetActiveIndex() const;
	int FindPage(const CPropertyPage* pPage) const;
	BOOL IsPage(HWND hWnd) const;

	CPropertyPage* GetActivePage();
	int GetPageIndex(const CPropertyPage* pPage) const;
	CPropertyPage* GetPage(int nIndex);
	const CPropertyPage* GetPage(int nIndex) const;
	CPropertyPage* FindPage(DWORD dwItemData);

	BOOL AddPage(CPropertyPage* pPage, LPCTSTR szTitle = NULL, DWORD dwItemData = 0);
	BOOL SetActivePage(int nIndex, BOOL bAndFocus = TRUE);
	BOOL SetActivePage(CPropertyPage* pPage, BOOL bAndFocus = TRUE);
	int GetPageCount() const { return m_aPages.GetSize(); }
	CString GetPageTitle(int nIndex) const;
	DWORD GetPageItemData(int nIndex) const;

	BOOL GetPageRect(int nIndex, LPRECT pRect) const;
	BOOL GetOrgPageSize(int nIndex, LPSIZE pSize) const;

	BOOL CreateAllPages();

	BOOL EnsurePageCreated(int nIndex);
	BOOL EnsurePageCreated(const CPropertyPage* pPage);
	BOOL IsPageCreated(int nIndex) const;

	void ForwardMessage(UINT nMsg) { m_aForwardMsgs.Add(nMsg); }

protected:
	CArray<PAGEITEM, PAGEITEM&> m_aPages;
	int m_nSelIndex;
	CUIntArray m_aForwardMsgs;
	BOOL m_bCmdNotifyParent;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyPageHost)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropertyPageHost)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnGetCurrentPageHwnd(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wp, LPARAM lp, LRESULT* pResult);
	virtual BOOL OnCommand(WPARAM wp, LPARAM lp);
	virtual void UpdatePageSize(int nPage);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGEHOST_H__43CF5AE7_C70B_443D_BC8B_7DA1D0E082DD__INCLUDED_)
