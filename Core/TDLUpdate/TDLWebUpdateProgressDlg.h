#if !defined(AFX_WEBUPDATEPROGRESSDLG_H__63C954EF_E75B_4EF3_A042_1A5DAF62F89D__INCLUDED_)
#define AFX_WEBUPDATEPROGRESSDLG_H__63C954EF_E75B_4EF3_A042_1A5DAF62F89D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WebUpdateProgressDlg.h : header file
//

#include "..\ToDoList\TDLWizard.h"

#include "..\shared\WindowIcons.h"

//////////////////////////////////////////////////////////////////////

enum TDL_WEBUPDATE_PROGRESS
{
	TDLWP_NONE = -1,
	TDLWP_DOWNLOAD,
	TDLWP_UNZIP,
	TDLWP_BACKUP,
	TDLWP_COPY,
	TDLWP_CLEANUP,
	TDLWP_COMPLETE,

	// Last item
	TDLWP_NUMSTATES
};

/////////////////////////////////////////////////////////////////////////////
// CTDLWebUpdateProgressPage dialog

class CTDLWebUpdateProgressPage : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CTDLWebUpdateProgressPage)
		
public:
	CTDLWebUpdateProgressPage();
	
	void SetProgressStatus(TDL_WEBUPDATE_PROGRESS nStatus, int nPercent = 0);
	TDL_WEBUPDATE_PROGRESS GetProgressStatus() const { return m_nStatus; }
	
protected:
	TDL_WEBUPDATE_PROGRESS m_nStatus;
	CListCtrl m_lcProgress;
	CStringArray m_aProgressDescriptions;
	CString m_sDone;

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	
protected:
	void OnProgressCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CTDLWebUpdateProgressDlg dialog

class CTDLWebUpdateProgressDlg : public CTDLWizard
{
public:
	CTDLWebUpdateProgressDlg(const CPoint& ptPos);
	virtual ~CTDLWebUpdateProgressDlg();

	void SetProgressStatus(TDL_WEBUPDATE_PROGRESS nStatus, int nPercent = 0);
	TDL_WEBUPDATE_PROGRESS GetProgressStatus() const;
	
	BOOL IsCancelled() const;

protected:
	CTDLWebUpdateProgressPage m_page;

	BOOL m_bCancelled;
	CPoint m_ptInitialPos;
	CWindowIcons m_icons;
	HBITMAP m_hbmHeader;

protected:
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnClose();
	afx_msg void OnCancel();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_WEBUPDATEPROGRESSDLG_H__63C954EF_E75B_4EF3_A042_1A5DAF62F89D__INCLUDED_)
