#if !defined(AFX_WEBUPDATEPROGRESSDLG_H__63C954EF_E75B_4EF3_A042_1A5DAF62F89D__INCLUDED_)
#define AFX_WEBUPDATEPROGRESSDLG_H__63C954EF_E75B_4EF3_A042_1A5DAF62F89D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WebUpdateProgressDlg.h : header file
//

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
		
		// Construction
public:
	CTDLWebUpdateProgressPage();   // standard constructor
	
	void AttachFont(HFONT hFont) { m_hFont = hFont; }
	
	void SetProgressStatus(TDL_WEBUPDATE_PROGRESS nStatus, int nPercent = 0);
	TDL_WEBUPDATE_PROGRESS GetProgressStatus() const { return m_nStatus; }
	
protected:
	TDL_WEBUPDATE_PROGRESS m_nStatus;
	CListCtrl m_lcProgress;
	CStringArray m_aProgressDescriptions;
	CString m_sDone;
	HFONT m_hFont;

	// Dialog Data
	//{{AFX_DATA(CTDLWebUpdatePromptDlg)
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLWebUpdatePromptDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	
	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTDLWebUpdatePromptDlg)
	//}}AFX_MSG
	void OnProgressCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CTDLWebUpdateProgressDlg dialog

class CTDLWebUpdateProgressDlg : public CPropertySheetEx
{
// Construction
public:
	CTDLWebUpdateProgressDlg(const CPoint& ptPos);
	virtual ~CTDLWebUpdateProgressDlg();

	void SetProgressStatus(TDL_WEBUPDATE_PROGRESS nStatus, int nPercent = 0);
	TDL_WEBUPDATE_PROGRESS GetProgressStatus() const;
	
	BOOL IsCancelled() const;

protected:
// Dialog Data
	//{{AFX_DATA(CTDLWebUpdateProgressDlg)
	//}}AFX_DATA
	CTDLWebUpdateProgressPage m_page;
	HFONT m_hFont;
	BOOL m_bCancelled;
	CPoint m_ptInitialPos;
	CWindowIcons m_icons;
	HBITMAP m_hbmHeader;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLWebUpdateProgressDlg)
	//}}AFX_VIRTUAL
	virtual BOOL OnInitDialog();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTDLWebUpdateProgressDlg)
	//}}AFX_MSG
	afx_msg void OnClose();
	afx_msg void OnCancel();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBUPDATEPROGRESSDLG_H__63C954EF_E75B_4EF3_A042_1A5DAF62F89D__INCLUDED_)
