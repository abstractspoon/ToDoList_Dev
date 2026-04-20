#if !defined(AFX_TDLTRANSFORMDIALOG_H__1A62F94F_687F_421C_97D2_300BAC4A3E7C__INCLUDED_)
#define AFX_TDLTRANSFORMDIALOG_H__1A62F94F_687F_421C_97D2_300BAC4A3E7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLTransformDialog.h : header file
//

#include "TDLDialog.h"
#include "TDLTaskSelectionPage.h"

#include "..\shared\filemisc.h"
#include "..\shared\fileedit.h"
#include "..\shared\historycombobox.h"
#include "..\shared\tabbedpropertypagehost.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLTransformDialog dialog

class CTDLTransformDialog : public CTDLDialog
{
// Construction
public:
	CTDLTransformDialog(LPCTSTR szTitle, 
						BOOL bEnableSubtaskSelection,
						LPCTSTR szStylesheet,
						const CTDCCustomAttribDefinitionArray& aAttribDefs, 
						CWnd* pParent = NULL);

	CString GetStylesheet() const;
	CString GetTitle() const { return m_sTitle; }
	COleDateTime GetDate() const;

	const CTDLTaskSelectionPage& GetTaskSelection() const { return m_pageTaskSel; }

protected:
	CString	m_sStylesheet;
	CString	m_sTitle;
	BOOL	m_bDate;

	CTabbedPropertyPageHost m_ppHost;
	CTDLTaskSelectionPage m_pageTaskSel;
	CHistoryComboBox m_cbTitle;
	CFileEdit	m_eStylesheet;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnChangeStylesheet();
	afx_msg void OnConfigureStylesheet();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

protected:
	void InitStylesheet(LPCTSTR szStylesheet);
};

#endif // !defined(AFX_TDLTRANSFORMDIALOG_H__1A62F94F_687F_421C_97D2_300BAC4A3E7C__INCLUDED_)
