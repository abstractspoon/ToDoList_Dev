#if !defined(AFX_TDLTASKICONDLG_H__73512EBA_90D6_40EA_A2C3_F2089A567486__INCLUDED_)
#define AFX_TDLTASKICONDLG_H__73512EBA_90D6_40EA_A2C3_F2089A567486__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLTaskIconDlg.h : header file
//

#include "TDLDialog.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

// Sent to parent 
const static UINT WM_TDCTI_RELOADICONS = ::RegisterWindowMessage(_T("WM_TDCTI_RELOADICONS"));

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskIconDlg dialog

class CTDCImageList;

class CTDLTaskIconDlg : public CTDLDialog
{
// Construction
public:
	CTDLTaskIconDlg(const CTDCImageList& ilIcons, const CString& sSelName, BOOL bWantNoneItem = TRUE, int nNumImages = -1, CWnd* pParent = NULL);
	CTDLTaskIconDlg(const CTDCImageList& ilIcons, const CStringArray& aSelNames, int nNumImages = -1, CWnd* pParent = NULL);

	CString GetIconName() const; 
	int GetIconNames(CStringArray& aSelNames) const; 

	int DoModal(HICON hIcon = NULL);
	int DoModal(UINT nTitleStrID);

	static CString GetUserIconName(const CString& sImage);

protected:
// Dialog Data
	//{{AFX_DATA(CTDLTaskIconDlg)
	enum { IDD = IDD_TASKICON_DIALOG };
	//}}AFX_DATA

	CListCtrl	m_lcIcons;
	const CTDCImageList& m_ilIcons;

	int m_nNumImages; 
	BOOL m_bMultiSel;
	BOOL m_bWantNone;
	BOOL m_bAllowReload;

	CString m_sTitle;
	CString m_sIconName;
	CStringArray m_aIconNames;
	CMapStringToString m_mapRenamedItems;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLTaskIconDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	virtual void OnRepositionControls(int dx, int dy);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLTaskIconDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkIconlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedIconlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditIconlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnEditlabel();
	afx_msg void OnBeginlabeleditIconlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnReloadIcons();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void BuildListCtrl();
	void SaveRenamedImages();
	int FindListItem(int nImage) const;
	void EnableDisable();
	CString GetImageName(int iImage) const; 
	BOOL SelectNextMatch(TCHAR cKeypress, int nFrom, int nTo);

	static int CALLBACK SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort); 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTASKICONDLG_H__73512EBA_90D6_40EA_A2C3_F2089A567486__INCLUDED_)
