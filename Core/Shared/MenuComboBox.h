#if !defined(AFX_MENUCOMBOBOX_H__4D7E9344_F42F_4003_B1DE_0D567FC86B82__INCLUDED_)
#define AFX_MENUCOMBOBOX_H__4D7E9344_F42F_4003_B1DE_0D567FC86B82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MenuComboBox.h : header file
//

#include "tabbedcombobox.h"

/////////////////////////////////////////////////////////////////////////////
// CMenuComboBox window

class CMenuComboBox : public CTabbedComboBox
{
// Construction
public:
	CMenuComboBox();

// Operations
public:
	BOOL Initialise(UINT nMenuID, UINT nSeparatorResID = 0);
	BOOL Initialise(const CMenu& menu, UINT nSeparatorResID = 0);

	CString GetMenuItemText(UINT nMenuID, BOOL bFullPath) const;
	CString GetItemText(int nItem, BOOL bFullPath) const;

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
	void AddMenuItem(HMENU hMenu, int nPos, const CString& sPath, int nIndent);

	struct MCB_ITEMDATA : public ODCB_ITEMDATA
	{
		CString sFullPath;
	};

	virtual ODCB_ITEMDATA* NewExtItemData() const { return new MCB_ITEMDATA(); }
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
							  DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);
	virtual void GetItemColors(int nItem, UINT nItemState, DWORD dwItemData,
							   COLORREF& crText, COLORREF& crBack) const;
	virtual BOOL IsSelectableItem(int nItem) const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUCOMBOBOX_H__4D7E9344_F42F_4003_B1DE_0D567FC86B82__INCLUDED_)
