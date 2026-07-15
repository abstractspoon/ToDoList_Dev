#if !defined(AFX_TDLWEBUPDATEDLG_H__C551EDC9_69C8_40C3_B5D9_CA44A02C339D__INCLUDED_)
#define AFX_TDLWEBUPDATEDLG_H__C551EDC9_69C8_40C3_B5D9_CA44A02C339D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLWebUpdateDlg.h : header file
//

#include "tdcenum.h"
#include "TDLWizard.h"

#include "..\Shared\entoolbar.h"
#include "..\Shared\toolbarhelper.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLWebUpdatePromptDlg dialog

class CTDLWebUpdatePromptPage : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CTDLWebUpdatePromptPage)

public:
	CTDLWebUpdatePromptPage();

	void SetInfo(LPCTSTR szExeVer, const CStringArray& aChanges);

protected:
	CString	m_sPrompt;
	CString	m_sChanges;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();

protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CTDLWebUpdatePromptDlg

class CTDLWebUpdatePromptDlg : public CTDLWizard
{
	DECLARE_DYNAMIC(CTDLWebUpdatePromptDlg)
		
public:
	static TDL_WEBUPDATE_CHECK CheckForUpdates();

#ifdef _DEBUG
	static void ShowDialog();
#endif

protected:
	CTDLWebUpdatePromptDlg(LPCTSTR szExeVer, const CStringArray& aChanges);
	virtual ~CTDLWebUpdatePromptDlg();

	// Attributes
protected:
	CTDLWebUpdatePromptPage m_page;
	CEnToolBar m_toolbar;
	CToolbarHelper m_tbHelper;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	
protected:
	DECLARE_MESSAGE_MAP()
	
};

#endif // !defined(AFX_TDLWEBUPDATEDLG_H__C551EDC9_69C8_40C3_B5D9_CA44A02C339D__INCLUDED_)
