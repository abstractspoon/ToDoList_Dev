#if !defined(AFX_TDLPASTETASKATTRIBUTESDLG_H__476B6698_0A0C_40B8_AD87_9DF2C0ED355F__INCLUDED_)
#define AFX_TDLPASTETASKATTRIBUTESDLG_H__476B6698_0A0C_40B8_AD87_9DF2C0ED355F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLPasteTaskAttributesDlg.h : header file
//

#include "TDLDialog.h"
#include "TDLAttribListBox.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLPasteTaskAttributesDlg dialog

class CTDLPasteTaskAttributesDlg : public CTDLDialog
{
// Construction
public:
	CTDLPasteTaskAttributesDlg(const CTDCCustomAttribDefinitionArray& aCustAttribs, CWnd* pParent = NULL);   // standard constructor

	int GetSelectedAttributes(CTDCAttributeMap& mapAttrib) const;

protected:
// Dialog Data
	//{{AFX_DATA(CTDLPasteTaskAttributesDlg)
	enum { IDD = IDD_PASTETASKATTRIB_DIALOG };
	CTDLAttributeListBox m_lbSpecificAttributes;
	BOOL	m_bIncludeComments;
	BOOL	m_bIncludeCustomAttribs;
	BOOL	m_bIncludeDates;
	BOOL	m_bOnlyOverwriteEmpty;
	BOOL	m_bOnlyOverwriteWithNonEmpty;
	int		m_bSelectSpecificAttribs;
	BOOL	m_bIncludeTaskTitle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLPasteTaskAttributesDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTDLPasteTaskAttributesDlg)
	afx_msg void OnSelectAllAttributes();
	afx_msg void OnSelectSpecificAttributes();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void EnableDisableControls();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLPASTETASKATTRIBUTESDLG_H__476B6698_0A0C_40B8_AD87_9DF2C0ED355F__INCLUDED_)
