#if !defined(AFX_ENCOLORDIALOG_H__0C8EC4ED_8B90_4F81_96FC_085F8F36229D__INCLUDED_)
#define AFX_ENCOLORDIALOG_H__0C8EC4ED_8B90_4F81_96FC_085F8F36229D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// encolordialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEnColorDialog dialog

class CEnColorDialog : public CColorDialog
{
	DECLARE_DYNAMIC(CEnColorDialog)

public:
	CEnColorDialog(COLORREF clrInit = 0, DWORD dwFlags = 0, CWnd* pParentWnd = NULL);
	~CEnColorDialog();

	int DoModal();

protected:
	BOOL OnInitDialog();

protected:
	//{{AFX_MSG(CEnColorDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENCOLORDIALOG_H__0C8EC4ED_8B90_4F81_96FC_085F8F36229D__INCLUDED_)
