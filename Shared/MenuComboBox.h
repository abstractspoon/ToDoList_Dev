#if !defined(AFX_MENUCOMBOBOX_H__4D7E9344_F42F_4003_B1DE_0D567FC86B82__INCLUDED_)
#define AFX_MENUCOMBOBOX_H__4D7E9344_F42F_4003_B1DE_0D567FC86B82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MenuComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMenuComboBox window

class CMenuComboBox : public CComboBox
{
// Construction
public:
	CMenuComboBox();

// Operations
public:
	BOOL Initialise(UINT nMenuID, UINT nSeparatorResID = 0);
	BOOL Initialise(const CMenu& menu, UINT nSeparatorResID = 0);

	CString GetMenuItemText(UINT nMenuID) const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMenuComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMenuComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	void AddMenuItem(HMENU hMenu, int nPos, const CString& sPath);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUCOMBOBOX_H__4D7E9344_F42F_4003_B1DE_0D567FC86B82__INCLUDED_)
