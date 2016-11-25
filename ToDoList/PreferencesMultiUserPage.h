#if !defined(AFX_PREFERENCESMULTIUSERPAGE_H__D7484AFF_704B_4FA9_94DA_9AB2F5364816__INCLUDED_)
#define AFX_PREFERENCESMULTIUSERPAGE_H__D7484AFF_704B_4FA9_94DA_9AB2F5364816__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesMultiUserPage.h : header file
//

#include "..\shared\groupline.h"
#include "..\shared\preferencesbase.h"

/////////////////////////////////////////////////////////////////////////////
// CPreferencesMultiUserPage dialog

enum // source control
{
	STSS_NONE,
	STSS_LANONLY,
	STSS_ALL,
};

enum // reload
{
	RO_NO,
	RO_ASK,
	RO_AUTO,
	RO_NOTIFY,
};

class CPreferencesMultiUserPage : public CPreferencesPageBase
{
	DECLARE_DYNCREATE(CPreferencesMultiUserPage)

// Construction
public:
	CPreferencesMultiUserPage();
	~CPreferencesMultiUserPage();

	int GetReadonlyReloadOption() const;
	int GetTimestampReloadOption() const;
	BOOL GetAutoCheckOut() const { return m_bAutoCheckOut; }
	BOOL GetKeepTryingToCheckout() const { return m_bKeepTryingToCheckout; }
	BOOL GetCheckinOnClose() const { return m_bCheckinOnClose; }
	BOOL GetIncludeUserNameInCheckout() const { return m_bIncludeUserNameInCheckout; }
	UINT GetRemoteFileCheckFrequency() const { return m_nRemoteFileCheckFreq; }
	UINT GetAutoCheckinFrequency() const { return (m_bCheckinNoChange ? m_nCheckinNoEditTime : 0); }
	BOOL GetUsing3rdPartySourceControl() const { return m_bUse3rdPartySourceControl; }

protected:
// Dialog Data
	//{{AFX_DATA(CPreferencesMultiUserPage)
	enum { IDD = IDD_PREFMULTIUSER_PAGE };
	CComboBox	m_cbNoEditTime;
	BOOL	m_bCheckinNoChange;
	BOOL	m_bUse3rdPartySourceControl;
	BOOL	m_bIncludeUserNameInCheckout;
	//}}AFX_DATA
	CComboBox	m_cbRemoteFileCheck;
	BOOL	m_bAutoCheckOut;
	BOOL	m_bKeepTryingToCheckout;
	int		m_nReadonlyReloadOption;
	int		m_nTimestampReloadOption;
	BOOL	m_bCheckinOnClose;
	UINT	m_nRemoteFileCheckFreq;
	BOOL	m_bPromptReloadOnWritable;
	BOOL	m_bPromptReloadOnTimestamp;
	UINT    m_nCheckinNoEditTime;
	CGroupLineManager m_mgrGroupLines;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesMultiUserPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPreferencesMultiUserPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckinonnoedit();
	afx_msg void OnUse3rdpartysourcectrl();
	//}}AFX_MSG
	afx_msg void OnPromptreloadonwritable();
	afx_msg void OnPromptreloadontimestamp();
	DECLARE_MESSAGE_MAP()

   virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
   virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESMULTIUSERPAGE_H__D7484AFF_704B_4FA9_94DA_9AB2F5364816__INCLUDED_)
