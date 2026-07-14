#if !defined(AFX_TDLWIZARD_H__A8A46BFB_E2CA_4BBF_A76D_C363065DEFA7__INCLUDED_)
#define AFX_TDLWIZARD_H__A8A46BFB_E2CA_4BBF_A76D_C363065DEFA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardDlg.h : header file
//

#include <afxdlgs.h>

/////////////////////////////////////////////////////////////////////////////
// CWizardDlg dialog

class CTDLWizard : public CPropertySheetEx
{
	DECLARE_DYNAMIC(CTDLWizard)

protected:
	CTDLWizard();
	explicit CTDLWizard(UINT nIDCaption);
	explicit CTDLWizard(LPCTSTR pszCaption);

	void EnableProgressBar(BOOL bEnable = TRUE); // Handled internally
	void EnableProgressBar(BOOL bEnable, int nNumSteps); // Caller in/decrements steps

	void IncrementProgress();
	void DecrementProgress();

private:
	CProgressCtrl m_wndProgress;
	CFont m_font;

	int m_nNumSteps;
	int m_nCurStep;
	int m_nStepSize;
	BOOL m_bProgressEnabled;
	BOOL m_bAutoAdvance;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

protected:
	void InitSheet(LPCTSTR szTitle);
};

#endif // !defined(AFX_TDLWIZARD_H__A8A46BFB_E2CA_4BBF_A76D_C363065DEFA7__INCLUDED_)
