// CiCalExporterOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "iCalExporterOptionsDlg.h"


// CiCalExporterOptionsDlg dialog

IMPLEMENT_DYNAMIC(CiCalExporterOptionsDlg, CDialog)

CiCalExporterOptionsDlg::CiCalExporterOptionsDlg(ICALEXPORTAS nExportFormat, CWnd* pParent /*=NULL*/)
	: CDialog(IDD_EXPORTOPTIONSDIALOG, pParent)
{
	m_nExportFormat = nExportFormat;
}

CiCalExporterOptionsDlg::~CiCalExporterOptionsDlg()
{
}

void CiCalExporterOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_EXPORTASVEVENT, m_nExportFormat);
}


BEGIN_MESSAGE_MAP(CiCalExporterOptionsDlg, CDialog)
END_MESSAGE_MAP()

// CiCalExporterOptionsDlg message handlers


ICALEXPORTAS CiCalExporterOptionsDlg::GetTaskExportFormat() const
{
	return (ICALEXPORTAS)m_nExportFormat;
}
