#if !defined(AFX_PREFERENCESBASE_H__19D0237F_28DC_4B45_9762_DF9B4F7D8492__INCLUDED_)
#define AFX_PREFERENCESBASE_H__19D0237F_28DC_4B45_9762_DF9B4F7D8492__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "scrollingpropertypagehost.h"
#include "dialoghelper.h"
#include "winhelpbutton.h"
#include "icon.h"
#include "Groupline.h"
#include "SizeGrip.h"
#include "CtrlTextHighlighter.h"
#include "WndPrompt.h"

#include "..\Interfaces\IPreferences.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

const UINT WM_PPB_CTRLCHANGE = ::RegisterWindowMessage(_T("WM_PPB_CTRLCHANGE"));

/////////////////////////////////////////////////////////////////////////////

class IPreferences;
class CDeferWndMove;

/////////////////////////////////////////////////////////////////////////////
// CPreferencesPageBase dialog

class CPreferencesPageBase : public CPropertyPage, protected CDialogHelper
{
	DECLARE_DYNAMIC(CPreferencesPageBase);

	// Construction
public:
	CPreferencesPageBase(UINT nDlgTemplateID);
	virtual ~CPreferencesPageBase();
	
	virtual void LoadPreferences(const IPreferences* /*pPrefs*/, LPCTSTR /*szKey*/) = 0;
	virtual void SavePreferences(IPreferences* /*pPrefs*/, LPCTSTR /*szKey*/) const = 0;

	void SetBackgroundColor(COLORREF color);
	CWnd* GetDlgItem(UINT nID) const;
	UINT GetHelpID() const { return m_nHelpID; }

	virtual int HighlightUIText(const CStringArray& aSearch, COLORREF crHighlight);
	virtual void ClearHighlights();
	virtual CWnd* GetFirstHighlightedItem() const;

protected:
	HBRUSH m_brBack;
	COLORREF m_crback;
	BOOL m_bFirstShow;
	UINT m_nHelpID;

	CCtrlTextHighlighter m_ctrlHighlighter;
	CGroupLineManager m_mgrGroupLines;
	CWndPromptManager m_mgrPrompts;

protected:
	virtual BOOL OnInitDialog();
	virtual void OnFirstShow();
	
protected:
	afx_msg void OnControlChange(UINT nID = -1);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

protected:
	BOOL AddGroupLine(UINT nIDStatic);
};

/////////////////////////////////////////////////////////////////////////////
// CPreferencesDlgBase dialog

class CPreferencesDlgBase : public CDialog, protected CDialogHelper
{
	// Construction
public:
	CPreferencesDlgBase(UINT nDlgTemplateID, 
						UINT nPPHostFrameCtrlID,
						UINT nDialogIconID,
						UINT nHelpBtnIconID,
						CWnd* pParent);
	virtual ~CPreferencesDlgBase();
	
	int DoModal(IPreferences* prefs = NULL, LPCTSTR szKey = NULL, int nInitPage = -1, UINT nInitCtrlID = 0);
	void SetPageBackgroundColor(COLORREF color);

	virtual void LoadPreferences(const IPreferences* prefs, LPCTSTR szKey);
	virtual void SavePreferences(IPreferences* prefs, LPCTSTR szKey) const;
	
protected:
	CScrollingPropertyPageHost m_ppHost;
	CWinHelpButton m_btnHelp;
	CIcon m_icon;
	CSizeGrip m_sizer;

	UINT m_nDlgTemplateID;
	UINT m_nPPHostFrameCtrlID;
	UINT m_nHelpBtnIconID;
	UINT m_nDlgIconID;
	UINT m_nInitCtrlID;

	int m_nInitPage;
	CSize m_sizeOrgWindow, m_sizeCurWindow;
	CSize m_sizeCurClient;

	// Temporary pointer only
	IPreferences* m_pDoModalPrefs;
	CString m_sDoModalKey;
	
protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnClickHelpButton();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_MESSAGE_MAP()
		
protected:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	
	void OnApply(); // called from derived class
	
	BOOL EnsurePageCreated(int nPage) { return m_ppHost.EnsurePageCreated(nPage); }
	BOOL EnsurePageCreated(const CPreferencesPageBase* pPage) { return m_ppHost.EnsurePageCreated(pPage); }

	virtual BOOL SetActivePage(int nPage);
	virtual BOOL SetActivePage(CPreferencesPageBase* pPage);
	virtual BOOL AddPage(CPreferencesPageBase* pPage);
	virtual void ReposContents(CDeferWndMove& dwm, int nDX, int nDY);
	virtual void DoHelp();

	BOOL CreatePPHost(UINT nHostID);
	BOOL CreatePPHost(LPRECT pRect);
	void Resize(int cx = 0, int cy = 0);

	int GetActiveIndex() const { return m_ppHost.GetActiveIndex(); }
	CPreferencesPageBase* GetActivePage() { return (CPreferencesPageBase*)m_ppHost.GetActivePage(); }
	int FindPage(CPreferencesPageBase* pPage) { return m_ppHost.FindPage(pPage); }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESBASE_H__19D0237F_28DC_4B45_9762_DF9B4F7D8492__INCLUDED_)
