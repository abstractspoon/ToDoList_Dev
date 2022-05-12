// TDLTasklistSaveAsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLTasklistSaveAsDlg.h"

#include "..\shared\FileMisc.h"
#include "..\shared\Misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLTasklistSaveAsDlg dialog


CTDLTasklistSaveAsDlg::CTDLTasklistSaveAsDlg(LPCTSTR szFilePath,
											 LPCTSTR szProjectName,
											 LPCTSTR szDefaultFileExt,
											 LPCTSTR szFileFilter,
											 CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_TASKLISTSAVEAS_DIALOG, _T("SaveAs"), pParent),
	m_sOrgFilePath(szFilePath),
	m_sNewFilePath(szFilePath),
	m_sProjectName(szProjectName),
	m_eFilePath(FES_COMBOSTYLEBTN | FES_SAVEAS | FES_NOPROMPTOVERWRITE)
{
	//{{AFX_DATA_INIT(CTDLTasklistSaveAsDlg)
	//}}AFX_DATA_INIT
	m_eFilePath.SetDefaultSaveAsFileExtension(szDefaultFileExt);
	m_eFilePath.SetFilter(szFileFilter);
}


void CTDLTasklistSaveAsDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLTasklistSaveAsDlg)
	DDX_Control(pDX, IDC_NEWFILEPATH, m_eFilePath);
	DDX_Text(pDX, IDC_NEWFILEPATH, m_sNewFilePath);
	DDX_Text(pDX, IDC_NEWPROJNAME, m_sProjectName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLTasklistSaveAsDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLTasklistSaveAsDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLTasklistSaveAsDlg message handlers

int CTDLTasklistSaveAsDlg::DoModal()
{
	while (true)
	{
		switch (CTDLDialog::DoModal())
		{
		case IDOK:
			if (FileMisc::FileExists(m_sNewFilePath) && !FileMisc::IsSamePath(m_sNewFilePath, m_sOrgFilePath))
			{
				UINT nMsgResult = ::MessageBox(*this,
											   CEnString(IDS_CONFIRMSAVEAS, m_sNewFilePath),
											   CEnString(IDS_CONFIRMSAVEAS_TITLE),
											   MB_ICONWARNING | MB_YESNO);
				if (nMsgResult != IDYES)
					continue;
			}
			return IDOK;

		case IDCANCEL:
		default:
			return IDCANCEL;
		}
	}

	return IDCANCEL;
}
