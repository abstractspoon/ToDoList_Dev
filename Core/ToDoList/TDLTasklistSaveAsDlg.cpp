// TDLTasklistSaveAsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLTasklistSaveAsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLTasklistSaveAsDlg dialog


CTDLTasklistSaveAsDlg::CTDLTasklistSaveAsDlg(LPCTSTR szFilePath,
											 LPCTSTR szProjectName,
											 LPCTSTR szFileFilter,
											 LPCTSTR szDefaultFileExt, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_TASKLISTSAVEAS_DIALOG, _T("SaveAs"), pParent),
	m_sFilePath(szFilePath),
	m_sProjectName(szProjectName)
{
	//{{AFX_DATA_INIT(CTDLTasklistSaveAsDlg)
	//}}AFX_DATA_INIT
	m_eFilePath.SetDefaultSaveAsFileExtension(szDefaultFileExt);
	m_eFilePath.SetFilter(szFileFilter);
}


void CTDLTasklistSaveAsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLTasklistSaveAsDlg)
	DDX_Control(pDX, IDC_NEWFILEPATH, m_eFilePath);
	DDX_Text(pDX, IDC_NEWFILEPATH, m_sFilePath);
	DDX_Text(pDX, IDC_NEWPROJNAME, m_sProjectName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLTasklistSaveAsDlg, CDialog)
	//{{AFX_MSG_MAP(CTDLTasklistSaveAsDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLTasklistSaveAsDlg message handlers
