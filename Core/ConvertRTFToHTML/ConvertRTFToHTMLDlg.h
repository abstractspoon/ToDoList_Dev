
// ConvertRTFToHTMLDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "..\shared\FileEdit.h"
#include "..\shared\urlricheditctrl.h"

// CConvertRTFToHTMLDlg dialog
class CConvertRTFToHTMLDlg : public CDialogEx
{
// Construction
public:
	CConvertRTFToHTMLDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONVERTRTFTOHTML_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void OnOK();


// Implementation
protected:
	HICON m_hIcon;
	CUrlRichEditCtrl m_parser;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CFileEdit m_eInputTasklist;
	CFileEdit m_eOutputTasklist;
	CString m_sInputTasklist;
	CString m_sOutputTasklist;

protected:
	BOOL PostProcessHtml(CString& sHtml) const;

};
