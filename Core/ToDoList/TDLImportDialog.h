#if !defined(AFX_TDLIMPORTDIALOG_H__F3B10AEE_B46C_4183_AC05_FB72D7C5AFA4__INCLUDED_)
#define AFX_TDLIMPORTDIALOG_H__F3B10AEE_B46C_4183_AC05_FB72D7C5AFA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

#include "TDLDialog.h"

#include "..\shared\fileedit.h"
#include "..\shared\TabbedPropertyPageHost.h"

#include "..\Interfaces\importexportmgr.h"
#include "..\Interfaces\ImportExportComboBox.h"

/////////////////////////////////////////////////////////////////////////////

class CTDCImportExportMgr;

/////////////////////////////////////////////////////////////////////////////
// CTDLImportFromPage dialog

class CTDLImportFromPage : public CCmdNotifyPropertyPage, CDialogHelper
{
public:
	CTDLImportFromPage(const CTDCImportExportMgr& mgr, CWnd* pParent = NULL);

	void SetImporterFormatID(LPCTSTR szFormatID);
	BOOL SetUseFile(LPCTSTR szFilePath);
	void SetUseClipboard();
	BOOL SetUseText(LPCTSTR szText);

	BOOL GetImportFromText() const;
	CString GetImportFilePath() const;
	CString GetImportText() const;

protected:
	CFileEdit m_eFilePath;
	CFont m_fontMonospace;

	CString	m_sFormatTypeID;
	CString	m_sFromFilePath;
	CString	m_sFromText;
	int		m_bFromText;

	const CImportExportMgr& m_mgrImportExport;

	enum IMPORT_MODE
	{
		TDCIM_ALL,
		TDCIM_FILEONLY,
		TDCIM_CLIPBOARDONLY,
		TDCIM_TEXTONLY,
	};

	IMPORT_MODE m_nImportMode;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
//	virtual void OnRepositionControls(int dx, int dy);

protected:
	afx_msg void OnChangeImportFrom();
	afx_msg void OnChangeClipboardtext();
	afx_msg void OnChangeFilepath();
	afx_msg void OnRefreshclipboard();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

	BOOL IsCurrentImporterFileBased() const;
	CString GetCurrentImporterFilter() const;
	void UpdateTextField();
	BOOL IsMode(IMPORT_MODE nMode) const { return (m_nImportMode == nMode); }
	void EnableDisableControls();
};


/////////////////////////////////////////////////////////////////////////////
// CTDLImportToPage dialog

class CTDLImportToPage : public CCmdNotifyPropertyPage, CDialogHelper
{
public:
	CTDLImportToPage(BOOL bReadonlyTasklist, BOOL bTasklistHasSelection, CWnd* pParent = NULL);   // standard constructor

	BOOL SetImportTo(TDLID_IMPORTTO nImportTo);
	TDLID_IMPORTTO GetImportTo() const;

protected:
	CComboBox	m_cbTasklistPos;

	int		m_bMatchByTaskID;
	int		m_nActiveTasklistPos;
	int		m_nImportTo;
	BOOL	m_bTasklistHasSelection;
	BOOL	m_bReadonlyTasklist;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//virtual void OnRepositionControls(int dx, int dy);

protected:
	afx_msg void OnChangeImportTo();
	DECLARE_MESSAGE_MAP()

	BOOL IsCurrentImporterFileBased() const;
	CString GetCurrentImporterFilter() const;
	void EnableDisableControls();
};

/////////////////////////////////////////////////////////////////////////////
// CTDLImportDialog dialog

class CTDLImportDialog : public CTDLDialog
{
public:
	CTDLImportDialog(const CTDCImportExportMgr& mgr, BOOL bReadonlyTasklist, BOOL bTasklistHasSelection, CWnd* pParent = NULL);   // standard constructor

	BOOL SetImportTo(TDLID_IMPORTTO nImportTo) { return m_pageTo.SetImportTo(nImportTo); }
	BOOL SetUseFile(LPCTSTR szFilePath) { return m_pageFrom.SetUseFile(szFilePath); }
	void SetUseClipboard() { return m_pageFrom.SetUseClipboard(); }
	BOOL SetUseText(LPCTSTR szText) { return m_pageFrom.SetUseText(szText); }

	CString GetFormatTypeID() const;
	TDLID_IMPORTTO GetImportTo() const { return m_pageTo.GetImportTo(); }
	BOOL GetImportFromText() const { return m_pageFrom.GetImportFromText(); }
	CString GetImportFilePath() const { return m_pageFrom.GetImportFilePath(); }
	CString GetImportText() const { return m_pageFrom.GetImportText(); }

protected:
	CImportExportComboBox m_cbFormat;
	CTabbedPropertyPageHost m_ppHost;
	CTDLImportFromPage m_pageFrom;
	CTDLImportToPage m_pageTo;

	const CImportExportMgr& m_mgrImportExport;
	CString	m_sFormatTypeID;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnRepositionControls(int dx, int dy);

protected:
	afx_msg void OnChangeImportFrom();
	afx_msg void OnSelchangeImporter();
	afx_msg void OnChangeClipboardtext();
	afx_msg void OnChangeFilepath();
	afx_msg void OnRefreshclipboard();
	afx_msg void OnChangeImportTo();
	DECLARE_MESSAGE_MAP()

	BOOL IsCurrentImporterFileBased() const;
	CString GetCurrentImporterFilter() const;
	void EnableOK();
	void UpdateTextField();
	void EnableDisableControls();
};

#endif // !defined(AFX_TDLIMPORTDIALOG_H__F3B10AEE_B46C_4183_AC05_FB72D7C5AFA4__INCLUDED_)
