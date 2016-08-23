// CreateFileLinkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rtfcontentctrl.h"
#include "CreateFileLinkDlg.h"

#include "..\shared\EnBitmap.h"
#include "..\shared\EnString.h"
#include "..\shared\FileMisc.h"
#include "..\shared\DialogHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreateFileLinkDlg dialog

CCreateFileLinkDlg::CCreateFileLinkDlg(LPCTSTR szRefFile, RE_PASTE nLinkOption, BOOL bDefault, CWnd* pParent /*=NULL*/)
	: 
	CDialog(IDD_LINKTOFILE_DIALOG, pParent),
	m_sRefFile(szRefFile), 
	m_nLinkOption(nLinkOption), 
	m_bMakeDefault(bDefault)
{
	//{{AFX_DATA_INIT(CCreateFileLinkDlg)
	//}}AFX_DATA_INIT
}


void CCreateFileLinkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateFileLinkDlg)
	DDX_Check(pDX, IDC_MAKEDEFAULT, m_bMakeDefault);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_FILEURL, m_nLinkOption);
}


BEGIN_MESSAGE_MAP(CCreateFileLinkDlg, CDialog)
	//{{AFX_MSG_MAP(CCreateFileLinkDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateFileLinkDlg message handlers

BOOL CCreateFileLinkDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Disable the 'display image file' if no ref file or ref file is not an image
	if (CEnBitmap::GetFileType(m_sRefFile) == FT_UNKNOWN)
	{
		// If the link option is 'display image file' then reset it first
		// and turn off 'make this my default' so we don't overwrite
		// the original default
		if (m_nLinkOption == REP_ASIMAGE)
		{
			m_nLinkOption = 0;
			m_bMakeDefault = FALSE;

			UpdateData(FALSE);
		}

		GetDlgItem(IDC_FILEIMAGE)->EnableWindow(FALSE);
	}

	// use the actual filename to format the first item
	if (!m_sRefFile.IsEmpty())
	{
		CEnString sText;
		GetDlgItem(IDC_FILEURL)->GetWindowText(sText);
		
		CString sFile;
		sFile.Format(_T(".../%s"), FileMisc::GetFileNameFromPath(m_sRefFile));
		
		// Note: the label will not yet have been translated
		// so we have to do that first
		sText.Translate(*GetDlgItem(IDC_FILEURL));
		sText.Replace(_T("..."), sFile);
		
		// disable translation on this composite string
		sText.DoNotTranslate();
		
		// then set it
		GetDlgItem(IDC_FILEURL)->SetWindowText(sText);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
