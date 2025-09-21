#if !defined(AFX_OPTIONSDLG_H__D9ADE42C_81F7_476C_B8D9_7B3D5FA2C91E__INCLUDED_)
#define AFX_OPTIONSDLG_H__D9ADE42C_81F7_476C_B8D9_7B3D5FA2C91E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDlg.h : header file
//

#include "..\Shared\Icon.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog

class COptionsDlg : public CDialog
{
public:
	COptionsDlg(BOOL bImport, BOOL bIncludeProject, LPCTSTR szIndent, CWnd* pParent = NULL);

	CString GetIndent() const { return m_sIndent; }
	BOOL GetWantProject() const { return m_bIncludeProject; }

protected:
	BOOL m_bIncludeProject;
	BOOL m_bImport;
	int m_nIndent;

	CString m_sTitle, m_sProjectLabel;
	CString m_sIndent;
	CString	m_sLabel;

	CComboBox m_cbIndent;
	CIcon m_icon;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_OPTIONSDLG_H__D9ADE42C_81F7_476C_B8D9_7B3D5FA2C91E__INCLUDED_)
