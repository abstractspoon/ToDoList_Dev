// TDLStylesheetParamConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLStylesheetParamConfigDlg.h"

#include "..\Shared\FileMisc.h"

#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLStylesheetParamConfigDlg dialog


CTDLStylesheetParamConfigDlg::CTDLStylesheetParamConfigDlg(LPCTSTR szStylesheetPath, CWnd* pParent /*=NULL*/)
	: 
	CStylesheetParamConfigDlg(szStylesheetPath, 0/*IDS_STYLESHEETNOPARAMS_ERROR*/, pParent)
{
}

void CTDLStylesheetParamConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CStylesheetParamConfigDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLStylesheetParamConfigDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTDLStylesheetParamConfigDlg, CStylesheetParamConfigDlg)
	//{{AFX_MSG_MAP(CTDLStylesheetParamConfigDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLStylesheetParamConfigDlg message handlers

int CTDLStylesheetParamConfigDlg::DoModal()
{
	if (CStylesheetParamConfigDlg::DoModal() == IDOK)
	{
		CString sConfigPath(GetConfigStylesheetPath(m_sStylesheetPath));
		VERIFY(CStylesheetParamConfigDlg::SaveChanges(sConfigPath));

		return IDOK;
	}

	// else
	return IDCANCEL;
}

CString CTDLStylesheetParamConfigDlg::GetStylesheetPath() const
{ 
	if (GetUsingConfiguredStylesheet())
		return GetConfigStylesheetPath(m_sStylesheetPath);

	// else
	return m_sStylesheetPath;
}

CString CTDLStylesheetParamConfigDlg::GetConfigStylesheetPath(LPCTSTR szStylesheetPath)
{ 
	if (::PathIsRelative(szStylesheetPath) || !FileMisc::HasExtension(szStylesheetPath, _T("xsl")))
		return _T("");

	CString sConfigPath(szStylesheetPath);
	return FileMisc::ReplaceExtension(sConfigPath, _T("cfg"));
}

BOOL CTDLStylesheetParamConfigDlg::GetUsingConfiguredStylesheet() const
{
	CString sConfigPath = GetConfigStylesheetPath(m_sStylesheetPath);

	if (!FileMisc::FileExists(sConfigPath))
		return FALSE;

	return (FileMisc::GetFileLastModified(sConfigPath) > FileMisc::GetFileLastModified(m_sStylesheetPath));
}
