#if !defined(AFX_TDLANALYSELOGGEDTIMEDLG_H__407FCF58_234B_4917_A66F_D5AB46A5C397__INCLUDED_)
#define AFX_TDLANALYSELOGGEDTIMEDLG_H__407FCF58_234B_4917_A66F_D5AB46A5C397__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLAnalyseLoggedTimeDlg.h : header file
//

#include "TDLDialog.h"
#include "TaskFile.h"
#include "tdcenum.h"

#include "..\Shared\fileedit.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLAnalyseLoggedTimeDlg dialog

class CTDLAnalyseLoggedTimeDlg : public CTDLDialog
{
// Construction
public:
	CTDLAnalyseLoggedTimeDlg(const CString& sTaskFile, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs, CWnd* pParent = NULL);   // standard constructor

	TDCTTL_BREAKDOWN GetBreakdown() const { return m_nBreakdown; }
	TDCTTL_FORMAT GetOutputFormat() const { return m_nOutputFormat; }
	TDC_ATTRIBUTE GetGroupBy() const { return (m_bGroupBy? m_nGroupByAttrib : TDCA_NONE); }

	CString GetOutputFilepath() const { return m_sOutputFilePath; }
	BOOL GetDateRange(COleDateTime& dtFrom, COleDateTime& dtTo) const;
	BOOL GetAutoDisplayReport() const { return m_bAutoDisplayReport; }

protected:
// Dialog Data
	//{{AFX_DATA(CTDLAnalyseLoggedTimeDlg)
	enum { IDD = IDD_ANALYSELOGGEDTIME_DIALOG };
	//}}AFX_DATA
	TDCTTL_FORMAT m_nOutputFormat;
	TDCTTL_PERIOD m_nTimePeriod;
	TDCTTL_BREAKDOWN m_nBreakdown;
	TDC_ATTRIBUTE m_nGroupByAttrib;
	BOOL m_bAutoDisplayReport;

	BOOL m_bGroupBy;
	COleDateTime m_dtFrom;
	COleDateTime m_dtTo;
	CString	m_sOutputFilePath;

	CFileEdit m_eOutputFile;
	CComboBox m_cbTimePeriod;
	CComboBox m_cbBreakdown;
	CComboBox m_cbGroupByAttrib;
	CComboBox m_cbOutputFormat;

	const CTDCCustomAttribDefinitionArray& m_aCustomAttribDefs;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLAnalyseLoggedTimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK();
	virtual BOOL OnInitDialog();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLAnalyseLoggedTimeDlg)
	afx_msg void OnSelChangeTimePeriod();
	afx_msg void OnSelchangeOutputFormat();
	//}}AFX_MSG
	afx_msg void OnGroupBy();
	DECLARE_MESSAGE_MAP()

protected:
	void BuildTimePeriodCombo();
	void BuildBreakdownCombo(TDCTTL_PERIOD nPeriod);
	void BuildGroupByCombo();
	void BuildOutputFormatCombo();
	void UpdateDateFields();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLANALYSELOGGEDTIMEDLG_H__407FCF58_234B_4917_A66F_D5AB46A5C397__INCLUDED_)
