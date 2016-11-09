// TDLTransEditDlg.h : header file
//

#if !defined(AFX_TDLTRANSEDITDLG_H__0F67B66D_B692_4CF0_B237_D0698B2E44ED__INCLUDED_)
#define AFX_TDLTRANSEDITDLG_H__0F67B66D_B692_4CF0_B237_D0698B2E44ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

#include "..\transtext\transdictionary.h"

#include "..\shared\InputListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLTransEditDlg dialog

class CTDLTransEditDlg : public CDialog
{
// Construction
public:
	CTDLTransEditDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTDLTransEditDlg)
	enum { IDD = IDD_TDLTRANSEDIT_DIALOG };
	//}}AFX_DATA
	CInputListCtrl	m_lcDictItems;
	CTransDictionary m_dictionary;
	BOOL m_bEdited;
	CString m_sBaseTitle;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLTransEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK();
	virtual void OnCancel();

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTDLTransEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnFileLoadDictionary();
	afx_msg void OnFileSaveDictionary();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEndlabeleditDictionary(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFileExit();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void ResizeList(int cx = 0, int cy = 0);
	void UpdateCaption();
	BOOL PromptAndSave();
	void LoadState();
	void SaveState();
	BOOL LoadDictionary(LPCTSTR szDictPath);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTRANSEDITDLG_H__0F67B66D_B692_4CF0_B237_D0698B2E44ED__INCLUDED_)
