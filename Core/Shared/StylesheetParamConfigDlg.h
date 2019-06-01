#if !defined(AFX_STYLESHEETPARAMCONFIGDLG_H__0EF2C085_3C26_42CC_A404_BE286B6F4D81__INCLUDED_)
#define AFX_STYLESHEETPARAMCONFIGDLG_H__0EF2C085_3C26_42CC_A404_BE286B6F4D81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// stylesheetconfigdlg.h : header file
//

#include "RuntimeDlg.h"
#include "InputListCtrl.h"
#include "xslfile.h"

/////////////////////////////////////////////////////////////////////////////
// CStylesheetConfigDlg dialog

class CStylesheetParamConfigDlg : public CRuntimeDlg
{
// Construction
public:
	CStylesheetParamConfigDlg(LPCTSTR szStylesheetPath, UINT nIDNoParamsMsg, CWnd* pParent = NULL);   // standard constructor

	int DoModal();
	int GetParams(CXslParamArray& aParams) const;
	BOOL SaveChanges(LPCTSTR szStylesheetPath = NULL);

protected:
	CString m_sStylesheetPath;
	UINT m_nIDNoParamsMsg;
	CInputListCtrl m_lcParams;
	CXslParamArray m_aParams;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStylesheetConfigDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStylesheetConfigDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STYLESHEETPARAMCONFIGDLG_H__0EF2C085_3C26_42CC_A404_BE286B6F4D81__INCLUDED_)
