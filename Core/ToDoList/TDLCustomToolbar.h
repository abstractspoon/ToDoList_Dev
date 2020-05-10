#if !defined(AFX_TDLCUSTOMTOOLBAR_H__5A3C2745_89C6_421B_AD8B_AAC459C0106B__INCLUDED_)
#define AFX_TDLCUSTOMTOOLBAR_H__5A3C2745_89C6_421B_AD8B_AAC459C0106B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLCustomToolbar.h : header file
//

#include "TDCImageList.h"

#include "..\Shared\entoolbar.h"
#include "..\Shared\toolbarhelper.h"

/////////////////////////////////////////////////////////////////////////////

class CShortcutManager;
class CToolbarHelper;
class CTDCMainMenu;

/////////////////////////////////////////////////////////////////////////////

struct TOOLBARBUTTON
{
	TOOLBARBUTTON(UINT nID = 0, LPCTSTR szImage = NULL) 
		: nMenuID(nID), sImageID(szImage) 
	{
	}

	BOOL operator==(const TOOLBARBUTTON& tbOther) const
	{
		return ((nMenuID == tbOther.nMenuID) &&
				(sImageID == tbOther.sImageID));
	}

	BOOL IsSeparator() const
	{
		return (nMenuID == ID_SEPARATOR);
	}
	
	UINT nMenuID;
	CString sImageID;
};

typedef CArray<TOOLBARBUTTON, TOOLBARBUTTON&> CToolbarButtonArray;

/////////////////////////////////////////////////////////////////////////////
// CTDLCustomToolbar window

class CTDLCustomToolbar : public CEnToolBar
{
// Construction
public:
	CTDLCustomToolbar();

// Operations
public:
	BOOL InitialiseButtons(const CToolbarButtonArray& aButtons,
						   const CTDCMainMenu& mainMenu,
						   const CShortcutManager& mgrShortcuts);
	
	// Must have arrangement of buttons and separators
	BOOL ModifyButtonAttributes(const CToolbarButtonArray& aButtons,
								const CTDCMainMenu& mainMenu);

	static BOOL RemapMenuItemIDs(const CMap<UINT, UINT, UINT, UINT&>& mapCmdIDs,
								 CToolbarButtonArray& aButtons);

protected:
	CToolbarHelper m_tbHelper;

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLCustomToolbar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTDLCustomToolbar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLCustomToolbar)
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	static BOOL GetItemTooltip(UINT nMenuID, const CTDCMainMenu& mainMenu, CString& sTooltip);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLCUSTOMTOOLBAR_H__5A3C2745_89C6_421B_AD8B_AAC459C0106B__INCLUDED_)
