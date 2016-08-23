// EncryptDecryptTestDlg.h : header file
//

#if !defined(AFX_ENCRYPTDECRYPTTESTDLG_H__94307FCC_34A3_4DC8_92A1_45C1B0AC1148__INCLUDED_)
#define AFX_ENCRYPTDECRYPTTESTDLG_H__94307FCC_34A3_4DC8_92A1_45C1B0AC1148__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CEncryptDecryptTestDlg dialog

class IEncryption;

class CEncryptDecryptTestDlg : public CDialog
{
// Construction
public:
	CEncryptDecryptTestDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CEncryptDecryptTestDlg();

// Dialog Data
	//{{AFX_DATA(CEncryptDecryptTestDlg)
	enum { IDD = IDD_ENCRYPTDECRYPTTEST_DIALOG };
	CString	m_sFilePath;
	CString	m_sEncryptedPath;
	CString	m_sDecryptedPath;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEncryptDecryptTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	IEncryption* m_pEncryptor;

	// Generated message map functions
	//{{AFX_MSG(CEncryptDecryptTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBrowse();
	afx_msg void OnEncrypt();
	afx_msg void OnDecrypt();
	afx_msg void OnOpendecrypted();
	afx_msg void OnOpenencrypted();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL ReadFile(LPCTSTR szFilePath, unsigned char*& pBuffer, int& nLenBuffer);
	BOOL WriteFile(LPCTSTR szFilePath, unsigned char* pBuffer, int nLenBuffer);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENCRYPTDECRYPTTESTDLG_H__94307FCC_34A3_4DC8_92A1_45C1B0AC1148__INCLUDED_)
