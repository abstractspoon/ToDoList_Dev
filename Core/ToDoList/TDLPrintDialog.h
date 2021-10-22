#if !defined(AFX_TDLPRINTDIALOG_H__1A62F94F_687F_421C_97D2_300BAC4A3E7C__INCLUDED_)
#define AFX_TDLPRINTDIALOG_H__1A62F94F_687F_421C_97D2_300BAC4A3E7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLPrintDialog.h : header file
//

#include "TaskSelectionDlg.h"
#include "TDLDialog.h"

#include "..\shared\fileedit.h"
#include "..\shared\historycombobox.h"
#include "..\shared\TabbedPropertyPageHost.h"

#include "..\interfaces\ImportExportComboBox.h"

/////////////////////////////////////////////////////////////////////////////

class CTDLHtmlStyleStatic : public CStatic
{
public:
	void SetStyle(TDLPD_STYLE nFormat);
};

/////////////////////////////////////////////////////////////////////////////

class CTDLHtmlStyleComboBox : public CComboBox
{
	// Construction
public:
	void DDX(CDataExchange* pDX, TDLPD_STYLE& value);

protected:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLLanguageComboBox)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLLanguageComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	void BuildCombo();
};

/////////////////////////////////////////////////////////////////////////////

class CTDLPrintStylePage : public CPropertyPage
{
// Construction
public:
	CTDLPrintStylePage(LPCTSTR szStylesheet, 
					   const CTDCImportExportMgr& mgrImpExp,
					   LPCTSTR szPrefsKey,
					   LPCTSTR szExportToImageView);
	~CTDLPrintStylePage();
	
	TDLPD_STYLE GetExportStyle() const;
	BOOL GetStylesheet(CString& sStylesheet) const;
	BOOL GetOtherExporterTypeID(CString& sExporterID) const;

protected:
// Dialog Data
	//{{AFX_DATA(CTDLPrintStylePage)
	enum { IDD = IDD_PRINT_STYLE_PAGE };
	//}}AFX_DATA

	CTDLHtmlStyleStatic m_stSimpleIcon;
	CTDLHtmlStyleComboBox m_cbSimpleOptions;
	CImportExportComboBox m_cbOtherExporters;
	CFileEdit m_eStylesheet;

	CString	m_sStylesheet;
	int m_nStyleOption;
	CString m_sExportToImageView;
	CString m_sPrefsKey;
	CString m_sOtherExporterTypeID;
	TDLPD_STYLE m_nSimpleStyle;

	const CTDCImportExportMgr& m_mgrImpExp;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTDLPrintStylePage)
protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTDLPrintStylePage)
	afx_msg void OnSelchangeSimplePageOption();
	//}}AFX_MSG
	afx_msg void OnChangeStylesheet();
	afx_msg void OnChangeStyle();
	afx_msg void OnConfigureStylesheet();
	afx_msg void OnUsestylesheet();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

protected:
	void InitStylesheet(LPCTSTR szStylesheet);
	void EnableDisableControls();

	CString GetBaseStylesheetPath() const;
};

/////////////////////////////////////////////////////////////////////////////
// CTDLPrintTaskSelectionPage dialog

class CTDLPrintTaskSelectionPage : public CPropertyPage
{
// Construction
public:
	CTDLPrintTaskSelectionPage(const CTDCCustomAttribDefinitionArray& aAttribDefs, 
							   LPCTSTR szRegKey, BOOL bEnableSubtaskSelection);
	~CTDLPrintTaskSelectionPage();

	const CTaskSelectionDlg& GetTaskSelection() const { return m_dlgTaskSel; }
	void SetOutputStyle(TDLPD_STYLE nStyle);

protected:
// Dialog Data
	//{{AFX_DATA(CTDLPrintTaskSelectionPage)
	enum { IDD = IDD_PRINT_TASKSEL_PAGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CTaskSelectionDlg m_dlgTaskSel;
	TDLPD_STYLE	m_nExportStyle;

// Overrides
protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLPrintTaskSelectionPage)
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CTDLPrintTaskSelectionPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Implementation
protected:
};
/////////////////////////////////////////////////////////////////////////////
// CTDLPrintDialog dialog

class CTDLPrintDialog : public CTDLDialog
{
// Construction
public:
	CTDLPrintDialog(LPCTSTR szTitle,
					BOOL bPreview,
					const CTDCImportExportMgr& mgrImpExp,
					BOOL bEnableSubtaskSelection,
					LPCTSTR szStylesheet,
					const CTDCCustomAttribDefinitionArray& aAttribDefs,
					LPCTSTR szExportToImageView,
					CWnd* pParent = NULL);

	CString GetTitle() const { return m_sTitle; }
	COleDateTime GetDate() const;

	TDLPD_STYLE GetExportStyle() const { return m_pageStyle.GetExportStyle(); }
	BOOL GetStylesheet(CString& sStylesheet) const { return m_pageStyle.GetStylesheet(sStylesheet); }
	BOOL GetOtherExporterTypeID(CString& sExporterID) const { return m_pageStyle.GetOtherExporterTypeID(sExporterID); }

	const CTaskSelectionDlg& GetTaskSelection() const { return m_pageTaskSel.GetTaskSelection(); }

protected:
// Dialog Data
	//{{AFX_DATA(CTDLPrintDialog)
	enum { IDD = IDD_PRINT_DIALOG };
	//}}AFX_DATA
	CString	m_sTitle;
	BOOL	m_bDate;
	BOOL	m_bPrintPreview;
	int		m_nPrevActiveTab;
	
	CHistoryComboBox m_cbTitle;
	CTDLPrintStylePage m_pageStyle;
	CTDLPrintTaskSelectionPage m_pageTaskSel;
	CTabbedPropertyPageHost m_ppHost;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLPrintDialog)
protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLPrintDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLPRINTDIALOG_H__1A62F94F_687F_421C_97D2_300BAC4A3E7C__INCLUDED_)
