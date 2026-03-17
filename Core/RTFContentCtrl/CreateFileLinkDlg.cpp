// CreateFileLinkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rtfcontentctrl.h"
#include "CreateFileLinkDlg.h"

#include "..\shared\EnBitmap.h"
#include "..\shared\EnString.h"
#include "..\shared\FileMisc.h"
#include "..\shared\DialogHelper.h"
#include "..\shared\OSVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreateFileLinkDlg dialog

CCreateFileLinkDlg::CCreateFileLinkDlg(LPCTSTR szRefFile, RE_PASTE nLinkOption, BOOL bDefault, BOOL bReduceColors, CWnd* pParent /*=NULL*/)
	: 
	CDialog(IDD_LINKTOFILE_DIALOG, pParent),
	m_sRefFile(szRefFile), 
	m_nLinkOption(nLinkOption), 
	m_bMakeDefault(bDefault),
	m_bReduceImageColors(bReduceColors),
	m_icon(IDR_RTFCOMMENTS)
{
	ASSERT(COSVersion() != OSV_LINUX);
}

void CCreateFileLinkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_MAKEDEFAULT, m_bMakeDefault);
	DDX_Radio(pDX, IDC_FILEURL, m_nLinkOption);
	DDX_Check(pDX, IDC_REDUCEIMAGECOLORS, m_bReduceImageColors);
}

BEGIN_MESSAGE_MAP(CCreateFileLinkDlg, CDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

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
		GetDlgItem(IDC_REDUCEIMAGECOLORS)->EnableWindow(FALSE);
	}

	// use the actual filename to format the first item
	if (!m_sRefFile.IsEmpty())
	{
		CEnString sText;
		GetDlgItemText(IDC_FILEURL, sText);
		
		CString sFile;
		sFile.Format(_T(".../%s"), FileMisc::GetFileNameFromPath(m_sRefFile));
		
		// Note: the label will not yet have been translated
		// so we have to do that first
		sText.Translate(*GetDlgItem(IDC_FILEURL));
		sText.Replace(_T("..."), sFile);
		
		// disable translation on this composite string
		sText.DoNotTranslate();
		
		// then set it
		SetDlgItemText(IDC_FILEURL, sText);
	}

	SetIcon(m_icon, FALSE);
	
	return TRUE;
}
