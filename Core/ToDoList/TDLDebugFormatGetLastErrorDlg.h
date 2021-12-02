#if !defined(AFX_DEBUGFORMATGETLASTERRORDLG_H__978108CD_CE03_4422_B483_0E533B0C6F62__INCLUDED_)
#define AFX_DEBUGFORMATGETLASTERRORDLG_H__978108CD_CE03_4422_B483_0E533B0C6F62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLDebugFormatGetLastErrorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTDLDebugFormatGetLastError dialog

#ifdef _DEBUG

class CTDLDebugFormatGetLastErrorDlg : public CDialog
{
// Construction
public:
	CTDLDebugFormatGetLastErrorDlg(CWnd* pParent = NULL);   // standard constructor

protected:
// Dialog Data
	//{{AFX_DATA(CTDLDebugFormatGetLastErrorDlg)
	//}}AFX_DATA
	UINT m_nErrorCode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLDebugFormatGetLastErrorDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK() {}
//	virtual void OnCancel() {}
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTDLDebugFormatGetLastErrorDlg)
	afx_msg void OnChangeErrorCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // _DEBUG

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEBUGFORMATGETLASTERRORDLG_H__978108CD_CE03_4422_B483_0E533B0C6F62__INCLUDED_)
