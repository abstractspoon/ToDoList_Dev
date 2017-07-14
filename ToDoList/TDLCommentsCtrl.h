#if !defined(AFX_TDLCOMMENTCTRL_H__852964E3_4ABD_4B66_88BA_F553177616F2__INCLUDED_)
#define AFX_TDLCOMMENTCTRL_H__852964E3_4ABD_4B66_88BA_F553177616F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tdlcontenttypecombobox.h"

#include "..\shared\runtimedlg.h"
#include "..\shared\contentctrl.h"
#include "..\Shared\binarydata.h"
#include "..\Shared\UIThemeFile.h"

/////////////////////////////////////////////////////////////////////////////
// PreferencesTaskDefPage.h : header file

class CContentMgr;

/////////////////////////////////////////////////////////////////////////////

class CTDLCommentsCtrl : public CRuntimeDlg
{
	DECLARE_DYNAMIC(CTDLCommentsCtrl)

public:
	CTDLCommentsCtrl(LPCTSTR szLabel, int nComboLenDLU, const CContentMgr* pMgrContent = NULL);
	virtual ~CTDLCommentsCtrl();

	BOOL Create(CWnd* pParent, const CRect& rPos, UINT nID);
	void SetUITheme(const CUIThemeFile& theme);
	void SetDefaultCommentsFont(HFONT hFont);

	int GetSelectedFormat(CONTENTFORMAT& cf) const;
	int SetSelectedFormat(const CONTENTFORMAT& cf);

	BOOL GetContent(CString& sTextContent, CBinaryData& customContent) const;
	BOOL SetContent(const CString& sTextContent, const CBinaryData& customContent);

protected:
	const CContentMgr* m_pMgrContent;

	CTDLContentTypeComboBox	m_cbCommentsFmt;
	CContentCtrl m_ctrlComments;
	CUIThemeFile m_theme;
	CBrush m_brBack;
	HFONT m_hFont;

protected:
	int DoModal() { ASSERT(0); return IDCANCEL; }

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSelchangeCommentsformat();

	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsDoHelp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsKillFocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommentsWantSpellCheck(WPARAM wParam, LPARAM lParam);

protected:
	void CalcCommentsCtrlRect(CRect& rCtrl, int cx = 0, int cy = 0) const;
	BOOL UpdateControlFormat();
};

#endif // AFX_TDLCOMMENTCTRL_H__852964E3_4ABD_4B66_88BA_F553177616F2__INCLUDED_