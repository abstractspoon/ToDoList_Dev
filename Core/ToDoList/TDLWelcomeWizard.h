#if !defined(AFX_WELCOMEWIZARD_H__089919DB_8CBF_4F53_BFDF_6BB1C1C63929__INCLUDED_)
#define AFX_WELCOMEWIZARD_H__089919DB_8CBF_4F53_BFDF_6BB1C1C63929__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WelcomeWizard.h : header file
//

#include "TDLColumnListBox.h"

#include "..\shared\fileedit.h"
#include "..\shared\winhelpbutton.h"
#include "..\shared\icon.h"

/////////////////////////////////////////////////////////////////////////////

struct TDCCOLEDITFILTERVISIBILITY;

/////////////////////////////////////////////////////////////////////////////
// CWelcomePage1 dialog

class CTDLWelcomePage1 : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CTDLWelcomePage1)

public:
	CTDLWelcomePage1();
	~CTDLWelcomePage1();

	void AttachFont(HFONT hFont) { m_hFont = hFont; }
	BOOL GetUseIniFile() const { return m_bUseIniFile; }
	BOOL GetShareTasklists() const { return m_bShareTasklists; }

protected:
	int		m_bShareTasklists;
	int		m_bUseIniFile;
	HFONT m_hFont;

protected:
	virtual BOOL OnSetActive();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CWelcomePage2 dialog

class CTDLWelcomePage2 : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CTDLWelcomePage2)

public:
	CTDLWelcomePage2();
	~CTDLWelcomePage2();

	void AttachFont(HFONT hFont) { m_hFont = hFont; }
	void GetColumnVisibility(TDCCOLEDITFILTERVISIBILITY& vis) const;

protected:
	CTDLColumnListBox	m_lbColumns;
	HFONT m_hFont;

protected:
	virtual BOOL OnSetActive();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CWelcomePage3 dialog

class CTDLWelcomePage3 : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CTDLWelcomePage3)

public:
	CTDLWelcomePage3();
	~CTDLWelcomePage3();

	void AttachFont(HFONT hFont) { m_hFont = hFont; }
	BOOL GetHideAttributes() const { return m_bHideAttrib; }
	CString GetSampleFilePath() const;

protected:
	CFileEdit	m_eSampleTasklist;
	CString	m_sSampleTaskList;
	int		m_bHideAttrib;
	int		m_bViewSample;
	HFONT m_hFont;
	CIcon m_iconTDL;

protected:
	virtual BOOL OnSetActive();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnNosample();
	afx_msg void OnSample();
	afx_msg LRESULT OnGetFileIcon(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CWelcomeWizard

class CTDLWelcomeWizard : public CPropertySheetEx
{
	DECLARE_DYNAMIC(CTDLWelcomeWizard)

public:
	CTDLWelcomeWizard(LPCTSTR szAppVer);
	virtual ~CTDLWelcomeWizard();

public:
	BOOL GetUseIniFile() const { return m_page1.GetUseIniFile(); }
	BOOL GetShareTasklists() const { return m_page1.GetShareTasklists(); }
	BOOL GetHideAttributes() const { return m_page3.GetHideAttributes(); }
	CString GetSampleFilePath() const { return m_page3.GetSampleFilePath(); }
	void GetColumnVisibility(TDCCOLEDITFILTERVISIBILITY& vis) const;

protected:
	CTDLWelcomePage1 m_page1;
	CTDLWelcomePage2 m_page2;
	CTDLWelcomePage3 m_page3;
	HFONT m_hFont;
	CWinHelpButton m_btnHelp;
	CString m_sTitle;
	HBITMAP m_hbmHeader;

protected:
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnWizFinish();
	afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);
	DECLARE_MESSAGE_MAP()

protected:
	void InitSheet();
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_WELCOMEWIZARD_H__089919DB_8CBF_4F53_BFDF_6BB1C1C63929__INCLUDED_)
