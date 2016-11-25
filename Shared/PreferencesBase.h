#if !defined(AFX_PREFERENCESBASE_H__19D0237F_28DC_4B45_9762_DF9B4F7D8492__INCLUDED_)
#define AFX_PREFERENCESBASE_H__19D0237F_28DC_4B45_9762_DF9B4F7D8492__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "scrollingpropertypagehost.h"
#include "dialoghelper.h"

#include "..\Interfaces\IPreferences.h"

/////////////////////////////////////////////////////////////////////////////

const UINT WM_PPB_CTRLCHANGE = ::RegisterWindowMessage(_T("WM_PPB_CTRLCHANGE"));

/////////////////////////////////////////////////////////////////////////////

class IPreferences;

/////////////////////////////////////////////////////////////////////////////
// CPreferencesPageBase dialog

class CPreferencesPageBase : public CPropertyPage, protected CDialogHelper
{
	DECLARE_DYNAMIC(CPreferencesPageBase);

	// Construction
public:
	CPreferencesPageBase(UINT nID);
	~CPreferencesPageBase();
	
	virtual void LoadPreferences(const IPreferences* /*pPrefs*/) {}
	virtual void SavePreferences(IPreferences* /*pPrefs*/) {}

	void SetBackgroundColor(COLORREF color);
	CWnd* GetDlgItem(UINT nID) const;
	UINT GetHelpID() const { return m_nHelpID; }

protected:
	HBRUSH m_brush;
	COLORREF m_crback;
	BOOL m_bFirstShow;
	UINT m_nHelpID;

protected:
	afx_msg void OnControlChange(UINT nID = -1);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CPreferencesDlgBase dialog

class CPreferencesDlgBase : public CDialog
{
	// Construction
public:
	CPreferencesDlgBase(UINT nID = 0, CWnd* pParent = NULL);   // standard constructor
	
	int DoModal(IPreferences* prefs, int nInitPage = -1);
	void SetPageBackgroundColor(COLORREF color);
	
protected:
	int m_nInitPage;
	CScrollingPropertyPageHost m_pphost;

	// Temporary pointer only
	IPreferences* m_pPrefs;
	
protected:
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
		
protected:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	
	void OnApply(); // called from derived class
	
	BOOL EnsurePageCreated(int nPage) { return m_pphost.EnsurePageCreated(nPage); }
	BOOL EnsurePageCreated(const CPreferencesPageBase* pPage) { return m_pphost.EnsurePageCreated(pPage); }

	virtual BOOL SetActivePage(int nPage);
	virtual BOOL SetActivePage(CPreferencesPageBase* pPage);
	virtual BOOL AddPage(CPreferencesPageBase* pPage);
	
	BOOL CreatePPHost(UINT nHostID);
	BOOL CreatePPHost(LPRECT pRect);

	int GetActiveIndex() const { return m_pphost.GetActiveIndex(); }
	CPreferencesPageBase* GetActivePage() { return (CPreferencesPageBase*)m_pphost.GetActivePage(); }
	int FindPage(CPreferencesPageBase* pPage) { return m_pphost.FindPage(pPage); }
	
	void ForwardMessage(UINT nMsg) { m_pphost.ForwardMessage(nMsg); }
	
	void LoadPreferences(const IPreferences* prefs);
	void SavePreferences(IPreferences* prefs);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESBASE_H__19D0237F_28DC_4B45_9762_DF9B4F7D8492__INCLUDED_)
