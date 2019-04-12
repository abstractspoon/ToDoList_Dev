#if !defined(AFX_TOOLSUSERINPUTDLG_H__7C10499F_E103_4106_8581_DCD5D55FAEF5__INCLUDED_)
#define AFX_TOOLSUSERINPUTDLG_H__7C10499F_E103_4106_8581_DCD5D55FAEF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolsUserInputDlg.h : header file
//

#include "TDCToolsCmdlineParser.h"

#include "..\shared\runtimedlg.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\winhelpbutton.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CToolsUserInputDlg dialog

class CTDLToolsUserInputDlg : public CRuntimeDlg
{
// Construction
public:
	CTDLToolsUserInputDlg(const CTDCToolsCmdlineParser& tcp); 
	virtual ~CTDLToolsUserInputDlg();

	int DoModal(LPCTSTR szTitle);
	CString GetResult(LPCTSTR szItemName);

protected:
	struct TUINPUTITEM
	{
		UINT nCtrlID;
		CString sLabel; // this will be a simple static text item
		CString sName;
		CString sDefValue;
		CLA_TYPE nType;
		UINT nStyle;
		CSize sizeDLU;
		CWnd* pCtrl; // ptr to dynamically allocated items
	};

	CArray<TUINPUTITEM, TUINPUTITEM&> m_aInputItems;
	CMapStringToString m_mapResults; // mapped by name
	CRect m_rWindowOrg, m_rClientOrg;
	UINT m_nDividerID, m_nHelpBtnID;
	CWinHelpButton m_btnHelp;

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolsUserInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CToolsUserInputDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);
	DECLARE_MESSAGE_MAP()

protected:
	void SetInitialPos(LPCRECT pRect, DWORD dwStyle);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLSUSERINPUTDLG_H__7C10499F_E103_4106_8581_DCD5D55FAEF5__INCLUDED_)
