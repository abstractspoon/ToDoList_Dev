#if !defined(AFX_PREFERENCESEXPORTPAGE_H__0A884806_5921_4C13_B368_6D14A441ADAC__INCLUDED_)
#define AFX_PREFERENCESEXPORTPAGE_H__0A884806_5921_4C13_B368_6D14A441ADAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesFilePage.h : header file
//

#include "..\3rdparty\fontcombobox.h"

#include "..\shared\preferencesbase.h"
#include "..\shared\enedit.h"

/////////////////////////////////////////////////////////////////////////////
// CPreferencesExportPage dialog

class CPreferencesExportPage : public CPreferencesPageBase
{
	DECLARE_DYNCREATE(CPreferencesExportPage)

// Construction
public:
	CPreferencesExportPage();
	~CPreferencesExportPage();

	int GetTextIndent() const { return m_bUseSpaceIndents ? m_nTextIndent : -1; }
	BOOL GetExportVisibleColsOnly() const { return m_bExportVisibleOnly; }
	CString GetHtmlFont() const { return m_sHtmlFont; }
	int GetHtmlFontSize() const { return m_nHtmlFontSize; }
	BOOL GetPreviewExport() const { return m_bPreviewExport; }
	BOOL GetExportParentTitleCommentsOnly() const { return m_bExportParentTitleCommentsOnly; }
	int GetExportSpaceForNotes() const { return m_bExportSpaceForNotes ? m_nLineSpaces : 0; }
//	BOOL Get() const { return m_b; }

protected:
// Dialog Data
	//{{AFX_DATA(CPreferencesExportPage)
	enum { IDD = IDD_PREFEXPORT_PAGE };
	CMaskEdit	m_eLineSpaces;
	CComboBox	m_cbFontSize;
	BOOL	m_bExportParentTitleCommentsOnly;
	BOOL	m_bExportSpaceForNotes;
	int		m_bUseSpaceIndents;
	int		m_nTextIndent;
	int		m_nLineSpaces;
	//}}AFX_DATA
	CFontNameComboBox	m_cbFonts;
	BOOL	m_bPreviewExport;
	BOOL	m_bExportVisibleOnly;
	CString m_sHtmlFont;
	int m_nHtmlFontSize;
	CMaskEdit m_eTextIndent;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesExportPage)
	//}}AFX_VIRTUAL

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnFirstShow();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPreferencesExportPage)
	afx_msg void OnChangeTextIndentType();
	afx_msg void OnExportspacefornotes();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

   virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
   virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESEXPORTPAGE_H__0A884806_5921_4C13_B368_6D14A441ADAC__INCLUDED_)
