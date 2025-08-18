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
#include "..\shared\icon.h"
#include "..\shared\historycombobox.h"
#include "..\shared\TabbedPropertyPageHost.h"

#include "..\interfaces\ImportExportComboBox.h"

/////////////////////////////////////////////////////////////////////////////

class CTDLHtmlStyleStatic : public CStatic
{
public:
	void SetStyle(TDLPD_STYLE nFormat);

protected:
	CIcon m_iconStyles[3];
};

/////////////////////////////////////////////////////////////////////////////

class CTDLHtmlStyleComboBox : public CComboBox
{
public:
	void DDX(CDataExchange* pDX, TDLPD_STYLE& value);

protected:
	virtual void PreSubclassWindow();

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
	enum { IDD = IDD_PRINT_STYLE_PAGE };

	CTDLHtmlStyleStatic m_stSimpleIcon;
	CTDLHtmlStyleComboBox m_cbSimpleOptions;
	CImportExportComboBox m_cbOtherExporters;
	CFileEdit m_eStylesheet;
	CStatic m_stImageIcon;

	CString	m_sStylesheet;
	int m_nStyleOption;
	CString m_sExportToImageView;
	CString m_sPrefsKey;
	CString m_sOtherExporterTypeID;
	TDLPD_STYLE m_nSimpleStyle;
	CIcon m_iconImage;

	const CTDCImportExportMgr& m_mgrImpExp;

protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnSelchangeSimplePageOption();
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
	enum { IDD = IDD_PRINT_TASKSEL_PAGE };

	CTaskSelectionDlg m_dlgTaskSel;
	TDLPD_STYLE	m_nExportStyle;

protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	DECLARE_MESSAGE_MAP()
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
	enum { IDD = IDD_PRINT_DIALOG };

	CString	m_sTitle;
	BOOL	m_bDate;
	BOOL	m_bPrintPreview;
	int		m_nPrevActiveTab;
	
	CHistoryComboBox m_cbTitle;
	CTDLPrintStylePage m_pageStyle;
	CTDLPrintTaskSelectionPage m_pageTaskSel;
	CTabbedPropertyPageHost m_ppHost;

protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLPRINTDIALOG_H__1A62F94F_687F_421C_97D2_300BAC4A3E7C__INCLUDED_)
