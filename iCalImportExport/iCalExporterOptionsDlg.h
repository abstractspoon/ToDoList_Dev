#pragma once

#if !defined(AFX_ICALEXPORTEROPTIONSDLG_H__5CDCA505_6C5C_4342_8F80_67368C7EE6A5__INCLUDED_)
#define AFX_ICALEXPORTEROPTIONSDLG_H__5CDCA505_6C5C_4342_8F80_67368C7EE6A5__INCLUDED_

// CiCalExporterOptionsDlg dialog

class CiCalExporterOptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(CiCalExporterOptionsDlg)

public:
	CiCalExporterOptionsDlg(BOOL bExportAsTasks, CWnd* pParent = NULL);   // standard constructor
	virtual ~CiCalExporterOptionsDlg();

	BOOL GetWantExportTasksAsTodos() const;

protected:
	BOOL m_nExportAs;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_ICALEXPORTEROPTIONSDLG_H__5CDCA505_6C5C_4342_8F80_67368C7EE6A5__INCLUDED_)
