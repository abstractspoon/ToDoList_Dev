// CiCalExporterOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "iCalExporterOptionsDlg.h"


// CiCalExporterOptionsDlg dialog

IMPLEMENT_DYNAMIC(CiCalExporterOptionsDlg, CDialog)

CiCalExporterOptionsDlg::CiCalExporterOptionsDlg(BOOL bExportAsTasks, CWnd* pParent /*=NULL*/)
	: CDialog(IDD_EXPORTOPTIONSDIALOG, pParent)
{
	m_nExportAs = (bExportAsTasks ? 1 : 0);
}

CiCalExporterOptionsDlg::~CiCalExporterOptionsDlg()
{
}

void CiCalExporterOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_EXPORTASVEVENT, m_nExportAs);
}


BEGIN_MESSAGE_MAP(CiCalExporterOptionsDlg, CDialog)
END_MESSAGE_MAP()

// CiCalExporterOptionsDlg message handlers


BOOL CiCalExporterOptionsDlg::GetWantExportTasksAsTodos() const
{
	return (m_nExportAs == 1);
}
