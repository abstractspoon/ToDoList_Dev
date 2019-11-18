#if !defined(AFX_TDLIMPORTFROMTEXTDLG_H__EAD04A7C_6F3E_400A_8837_C74897B84054__INCLUDED_)
#define AFX_TDLIMPORTFROMTEXTDLG_H__EAD04A7C_6F3E_400A_8837_C74897B84054__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLPasteFromClipboardDlg.h : header file
//

#include "TDLDialog.h"

#include "..\Interfaces\ImportExportComboBox.h"

/////////////////////////////////////////////////////////////////////////////

class CTDCImportExportMgr;

/////////////////////////////////////////////////////////////////////////////
// CTDLPasteFromClipboardDlg dialog

class CTDLImportFromTextBaseDlg : public CTDLDialog
{
// Construction
protected:
	CTDLImportFromTextBaseDlg(UINT nDlgTemplateID, LPCTSTR szPrefsKey, const CTDCImportExportMgr& mgr, CWnd* pParent = NULL);   // standard constructor

public:
	CString GetText() const { return m_sText; }
	CString GetFormatTypeID() const { return m_sFormatTypeID; } 

protected:
// Dialog Data
	//{{AFX_DATA(CTDLPasteFromClipboardDlg)
	CImportExportComboBox	m_cbFormat;
	CString	m_sText;
	//}}AFX_DATA

	CString	m_sFormatTypeID;
	CFont m_fontMonospace;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual BOOL OnInitDialog();

// Implementation
protected:

};

////////////////////////////////////////////////////////////////////////////////////

class CTDLPasteFromClipboardDlg : public CTDLImportFromTextBaseDlg
{
// Construction
public:
	CTDLPasteFromClipboardDlg(const CTDCImportExportMgr& mgr, CWnd* pParent = NULL);   // standard constructor

// Implementation
protected:
	afx_msg void OnRefresh();

	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

class CTDLImportFromDropTextDlg : public CTDLImportFromTextBaseDlg
{
// Construction
public:
	CTDLImportFromDropTextDlg(const CString& sDropText, const CTDCImportExportMgr& mgr, CWnd* pParent = NULL);   // standard constructor

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLIMPORTFROMTEXTDLG_H__EAD04A7C_6F3E_400A_8837_C74897B84054__INCLUDED_)
