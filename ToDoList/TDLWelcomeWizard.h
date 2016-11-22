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

/////////////////////////////////////////////////////////////////////////////

struct TDCCOLEDITFILTERVISIBILITY;

/////////////////////////////////////////////////////////////////////////////
// CWelcomePage1 dialog

class CTDLWelcomePage1 : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CTDLWelcomePage1)

// Construction
public:
	CTDLWelcomePage1();
	~CTDLWelcomePage1();

	void AttachFont(HFONT hFont) { m_hFont = hFont; }
	BOOL GetUseIniFile() const { return m_bUseIniFile; }
	BOOL GetShareTasklists() const { return m_bShareTasklists; }

protected:
// Dialog Data
	//{{AFX_DATA(CWelcomePage1)
	enum { IDD = IDD_WELCOME_PAGE1 };
	int		m_bShareTasklists;
	int		m_bUseIniFile;
	//}}AFX_DATA
	HFONT m_hFont;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWelcomePage1)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWelcomePage1)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CWelcomePage2 dialog

class CTDLWelcomePage2 : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CTDLWelcomePage2)

// Construction
public:
	CTDLWelcomePage2();
	~CTDLWelcomePage2();

	void AttachFont(HFONT hFont) { m_hFont = hFont; }
	void GetColumnVisibility(TDCCOLEDITFILTERVISIBILITY& vis) const;

protected:
// Dialog Data
	//{{AFX_DATA(CWelcomePage2)
	enum { IDD = IDD_WELCOME_PAGE2 };
	CTDLColumnListBox	m_lbColumns;
	//}}AFX_DATA
	HFONT m_hFont;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWelcomePage2)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWelcomePage2)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CWelcomePage3 dialog

class CTDLWelcomePage3 : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CTDLWelcomePage3)

// Construction
public:
	CTDLWelcomePage3();
	~CTDLWelcomePage3();

	void AttachFont(HFONT hFont) { m_hFont = hFont; }
	BOOL GetHideAttributes() const { return m_bHideAttrib; }
	CString GetSampleFilePath() const;

protected:
// Dialog Data
	//{{AFX_DATA(CWelcomePage3)
	enum { IDD = IDD_WELCOME_PAGE3 };
	CFileEdit	m_eSampleTasklist;
	CString	m_sSampleTaskList;
	int		m_bHideAttrib;
	int		m_bViewSample;
	//}}AFX_DATA
	HFONT m_hFont;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWelcomePage3)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWelcomePage3)
	virtual BOOL OnInitDialog();
	afx_msg void OnNosample();
	afx_msg void OnSample();
	//}}AFX_MSG
	afx_msg LRESULT OnGetFileIcon(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CWelcomeWizard

class CTDLWelcomeWizard : public CPropertySheetEx
{
	DECLARE_DYNAMIC(CTDLWelcomeWizard)

// Construction
public:
	CTDLWelcomeWizard();

// Operations
public:
	BOOL GetUseIniFile() const { return m_page1.GetUseIniFile(); }
	BOOL GetShareTasklists() const { return m_page1.GetShareTasklists(); }
	BOOL GetHideAttributes() const { return m_page3.GetHideAttributes(); }
	CString GetSampleFilePath() const { return m_page3.GetSampleFilePath(); }
	void GetColumnVisibility(TDCCOLEDITFILTERVISIBILITY& vis) const;

// Attributes
protected:
	CTDLWelcomePage1 m_page1;
	CTDLWelcomePage2 m_page2;
	CTDLWelcomePage3 m_page3;
	HFONT m_hFont;
	CWinHelpButton m_btnHelp;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWelcomeWizard)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTDLWelcomeWizard();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWelcomeWizard)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnWizFinish();
	afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);
	DECLARE_MESSAGE_MAP()

protected:
	void InitSheet();

};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WELCOMEWIZARD_H__089919DB_8CBF_4F53_BFDF_6BB1C1C63929__INCLUDED_)
