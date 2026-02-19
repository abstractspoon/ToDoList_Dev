#if !defined(AFX_CREATEFILELINKDLG_H__F70FEFCC_D6D8_40D7_8FB3_ACE6CD2DEEDB__INCLUDED_)
#define AFX_CREATEFILELINKDLG_H__F70FEFCC_D6D8_40D7_8FB3_ACE6CD2DEEDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreateFileLinkDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#include "..\shared\RichEditHelper.h"
#include "..\shared\icon.h"

/////////////////////////////////////////////////////////////////////////////
// CCreateFileLinkDlg dialog

class CCreateFileLinkDlg : public CDialog
{
// Construction
public:
	CCreateFileLinkDlg(LPCTSTR szRefFile, RE_PASTE nLinkOption, BOOL bDefault, BOOL bReduceColors, CWnd* pParent = NULL);

	RE_PASTE GetLinkOption() const { return (RE_PASTE)m_nLinkOption; }
	BOOL GetMakeLinkOptionDefault() const { return m_bMakeDefault; }
	BOOL GetReduceImageColors() const { return (m_bReduceImageColors && (m_nLinkOption == REP_ASIMAGE)); }

protected:
	BOOL	m_bReduceImageColors;
	BOOL	m_bMakeDefault;
	int		m_nLinkOption;
	CString m_sRefFile;
	CIcon m_icon;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_CREATEFILELINKDLG_H__F70FEFCC_D6D8_40D7_8FB3_ACE6CD2DEEDB__INCLUDED_)
