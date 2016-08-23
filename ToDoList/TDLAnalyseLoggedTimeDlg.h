#if !defined(AFX_TDLANALYSELOGGEDTIMEDLG_H__407FCF58_234B_4917_A66F_D5AB46A5C397__INCLUDED_)
#define AFX_TDLANALYSELOGGEDTIMEDLG_H__407FCF58_234B_4917_A66F_D5AB46A5C397__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLAnalyseLoggedTimeDlg.h : header file
//

#include "TDLDialog.h"
#include "tdcenum.h"

#include "..\Shared\fileedit.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLAnalyseLoggedTimeDlg dialog

class CTDLAnalyseLoggedTimeDlg : public CTDLDialog
{
// Construction
public:
	CTDLAnalyseLoggedTimeDlg(const CString& sLogPath, CWnd* pParent = NULL);   // standard constructor

	TDCTTL_BREAKDOWN GetBreakdown() const { return m_nBreakdown; }
	TDCTTL_PERIOD GetPeriod() const { return m_nTimePeriod; }
	TDCTTL_FORMAT GetOutputFormat() const { return m_nOutputFormat; }
	CString GetOutputFilepath() const { return m_sOutputFilePath; }
	BOOL GetDateRange(COleDateTime& dtFrom, COleDateTime& dtTo) const;

protected:
// Dialog Data
	//{{AFX_DATA(CTDLAnalyseLoggedTimeDlg)
	enum { IDD = IDD_ANALYSELOGGEDTIME_DIALOG };
	CComboBox	m_cbOutputFormat;
	COleDateTime	m_dtFrom;
	COleDateTime	m_dtTo;
	CString	m_sOutputFilePath;
	//}}AFX_DATA
	TDCTTL_FORMAT m_nOutputFormat;
	TDCTTL_PERIOD m_nTimePeriod;
	TDCTTL_BREAKDOWN m_nBreakdown;
	CFileEdit m_eOutputFile;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLAnalyseLoggedTimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLAnalyseLoggedTimeDlg)
	afx_msg void OnChangePeriod();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeOutputFormat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLANALYSELOGGEDTIMEDLG_H__407FCF58_234B_4917_A66F_D5AB46A5C397__INCLUDED_)
