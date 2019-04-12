// StatusBarProgress.h: interface for the CStatusBarProgress class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUSBARPROGRESS_H__DA091770_47D8_4A2D_AE9B_25A0853A6FF3__INCLUDED_)
#define AFX_STATUSBARPROGRESS_H__DA091770_47D8_4A2D_AE9B_25A0853A6FF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProgressThread;

class CStatusBarProgress  
{
	friend class CStatusBarProgressProxy;

public:
	CStatusBarProgress(HWND hwndStatusBar = NULL, const CString& sPrompt = _T(""), int nPane = 0);
	virtual ~CStatusBarProgress();

	BOOL BeginProgress(HWND hwndStatusBar, const CString& sPrompt = _T(""), int nPane = 0);
	void EndProgress();
	
	BOOL IsActive() const { return (m_pThread != NULL); }

protected:
	CString m_sPrevPrompt;
	HWND m_hwndStatusBar;
	int m_nSBPane;
	CProgressThread* m_pThread;
};

class CStatusBarProgressProxy
{
public:
	CStatusBarProgressProxy(CStatusBarProgress* pProxy, HWND hwndStatusBar, const CString& sPrompt = _T(""), int nPane = 0, BOOL bIgnoreIfActive = TRUE);
	virtual ~CStatusBarProgressProxy();

protected:
	CStatusBarProgress* m_pProxy;
};



#endif // !defined(AFX_STATUSBARPROGRESS_H__DA091770_47D8_4A2D_AE9B_25A0853A6FF3__INCLUDED_)
