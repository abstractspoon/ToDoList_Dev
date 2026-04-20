#if !defined(AFX_TDLSENDTASKSDLG_H__02DB12DD_A125_4754_AE1C_F8BB9EE45903__INCLUDED_)
#define AFX_TDLSENDTASKSDLG_H__02DB12DD_A125_4754_AE1C_F8BB9EE45903__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLSendTasksDlg.h : header file
//

#include "TDLPrintDialog.h" // For CTDLDialog, CTDLHtmlStyleStatic, CTDLHtmlStyleComboBox
#include "TDLTaskSelectionPage.h"
#include "TDCImportExportMgr.h"

#include "..\shared\tabbedpropertypagehost.h"

#include "..\Interfaces\ImportExportComboBox.h"

/////////////////////////////////////////////////////////////////////////////

enum TD_SENDAS
{
	TDSA_TASKLIST,
	TDSA_BODYTEXT,	
};

/////////////////////////////////////////////////////////////////////////////
// CTDLSendTasksDlg dialog

class CTDLSendTasksDlg : public CTDLDialog
{
// Construction
public:
	CTDLSendTasksDlg(const CTDCImportExportMgr& mgr, 
					 BOOL bSelectedTasks, 
					 BOOL bEnableSubtaskSelection,
					 const CTDCCustomAttribDefinitionArray& aAttribDefs, 
					 CWnd* pParent = NULL);

	TD_SENDAS GetSendAs() const { return (TD_SENDAS)m_nSendTasksAsOption; }
	CString GetFormatTypeID() const { return m_sFormatTypeID; }
	TDLPD_STYLE GetHtmlStyle() const { return m_nHtmlStyle; } 

	const CTDLTaskSelectionPage& GetTaskSelection() const { return m_pageTaskSel; }

protected:
	TDLPD_STYLE m_nHtmlStyle;
	int	m_nSendTasksAsOption;
	CString	m_sFormatTypeID;

	CImportExportComboBox m_cbFormat;
	CTDLTaskSelectionPage m_pageTaskSel;
	CTDLHtmlStyleStatic m_stHtmlOptionIcon;
	CTDLHtmlStyleComboBox m_cbHtmlOptions;
	CTabbedPropertyPageHost m_ppHost;

	const CTDCImportExportMgr& m_mgrImportExport;

protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

// Implementation
protected:
	afx_msg void OnSelChangeFormat();
	afx_msg void OnSelChangeHtmlStyle();
	DECLARE_MESSAGE_MAP()

protected:
	void UpdateHtmlOptionsVisibility();

};

#endif // !defined(AFX_TDLSENDTASKSDLG_H__02DB12DD_A125_4754_AE1C_F8BB9EE45903__INCLUDED_)
