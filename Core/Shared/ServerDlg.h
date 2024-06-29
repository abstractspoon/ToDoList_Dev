#if !defined(AFX_SERVERDLG_H__104A903D_A221_47B3_B6CC_52755637D9B0__INCLUDED_)
#define AFX_SERVERDLG_H__104A903D_A221_47B3_B6CC_52755637D9B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerDlg.h : header file
//

#include "runtimedlg.h"
#include "WndPrompt.h"

#include "..\Interfaces\Ipreferences.h"

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

enum
{
	SD_TITLE = 0,
	// don't put anything here else it will clash with IDOK, etc

	IDC_SD_SERVER = 1001,
	IDC_SD_SERVERLABEL,
	IDC_SD_USERNAME,
	IDC_SD_USERNAMELABEL,
	IDC_SD_PASSWORD,
	IDC_SD_PASSWORDLABEL,
	IDC_SD_ANONLOGIN,
	IDC_SD_PROXY,
	IDC_SD_PROXYLABEL,
	IDC_SD_PROXYPORT,
	IDC_SD_PROXYPORTLABEL,
};

enum AL_TYPE
{
	ANONLOGIN_HIDE	= -2,
	ANONLOGIN_AUTO	= -1,
	ANONLOGIN_NO	= 0,
	ANONLOGIN_YES	= 1,
};

class CServerDlg : public CRuntimeDlg
{
// Construction
public:
	CServerDlg(LPCTSTR szServer = NULL, 
			   LPCTSTR szUsername = NULL, 
			   LPCTSTR szPassword = NULL, 
			   AL_TYPE nAnonymousLogin = ANONLOGIN_AUTO,
			   HICON hIcon = NULL);

	CString GetServer() const { return m_sServer; }
	CString GetUsername() const { return m_sUsername; }
	CString GetPassword() const { return m_sPassword; }
	BOOL GetAnonymousLogin() const { return (m_nAnonLogin == ANONLOGIN_YES); }
	CString GetProxy(UINT& nPort) const { nPort = m_sProxy.IsEmpty() ? 0 : m_nProxyPort; return m_sProxy; }

	int DoModal(IPreferences* pPrefs, LPCTSTR szKey);

	// for overriding the default text for translating
	static void SetItemText(UINT nIDItem, UINT nIDText);

protected:
// Dialog Data
	CString	m_sServer;
	CString	m_sUsername;
	CString	m_sPassword;
	CString m_sProxy;
	UINT m_nProxyPort;
	AL_TYPE m_nAnonLogin;

	CComboBox m_cbServers;
	CComboBox m_cbUsernames;
	CWndPrompt m_wpProxy;
	HICON m_hIcon;
	
	IPreferences* m_pPrefs;
	CString m_sPrefKey;

	static CMap<UINT, UINT, CString, CString&> s_mapText;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual CString OverrideItemText(UINT nIDItem) { return GetItemText(nIDItem, NULL); }

protected:
	afx_msg void OnChangeServer();
	afx_msg void OnAnonLogin();
	afx_msg void OnChangeProxy();
	DECLARE_MESSAGE_MAP()

protected:
	CString GetItemText(UINT nIDItem, LPCTSTR szDefault);
	void EnableDisableProxy();
	BOOL HasProxySettings() const;
	
	void PopulateComboHistory(CComboBox& combo, LPCTSTR szCountKey, LPCTSTR szItemTemplate, LPCTSTR szCurrentValue) const;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__104A903D_A221_47B3_B6CC_52755637D9B0__INCLUDED_)
