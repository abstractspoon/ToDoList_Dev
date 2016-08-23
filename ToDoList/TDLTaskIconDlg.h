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
// CTDLTaskIconDlg dialog

class CTDCImageList;

class CTDLTaskIconDlg : public CTDLDialog
{
// Construction
public:
	CTDLTaskIconDlg(const CTDCImageList& ilIcons, const CString& sSelName, CWnd* pParent = NULL);   // standard constructor
	CTDLTaskIconDlg(const CTDCImageList& ilIcons, const CStringArray& aSelNames, CWnd* pParent = NULL);   // standard constructor

	CString GetIconName() const; 
	int GetIconNames(CStringArray& aSelNames) const; 

	CString GetUserIconName(const CString& sImage) const;

protected:
// Dialog Data
	//{{AFX_DATA(CTDLTaskIconDlg)
	enum { IDD = IDD_TASKICON_DIALOG };
	CListCtrl	m_lcIcons;
	//}}AFX_DATA
	const CTDCImageList& m_ilIcons;
	CString m_sIconName;
	CMapStringToString m_mapRenamedItems;
	BOOL m_bMultiSel;
	CStringArray m_aIconNames;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLTaskIconDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLTaskIconDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkIconlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedIconlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditIconlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnEditlabel();
	afx_msg void OnBeginlabeleditIconlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void BuildListCtrl();
	void SaveRenamedImages();
	int FindListItem(int nImage) const;
	void EnableDisable();
	CString GetImageName(int iImage) const; 

	static int CALLBACK SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort); 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLTASKICONDLG_H__73512EBA_90D6_40EA_A2C3_F2089A567486__INCLUDED_)
