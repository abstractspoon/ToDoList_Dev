#if !defined(AFX_TDLCUSTOMTOOLBAR_H__5A3C2745_89C6_421B_AD8B_AAC459C0106B__INCLUDED_)
#define AFX_TDLCUSTOMTOOLBAR_H__5A3C2745_89C6_421B_AD8B_AAC459C0106B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLCustomToolbar.h : header file
//

#include "TDCImageList.h"
#include "TDCToolsHelper.h"

#include "..\Shared\entoolbar.h"
#include "..\Shared\toolbarhelper.h"

/////////////////////////////////////////////////////////////////////////////

class CShortcutManager;
class CToolbarHelper;
class CTDCMainMenu;

/////////////////////////////////////////////////////////////////////////////

struct TDCCUSTOMTOOLBARBUTTON
{
	TDCCUSTOMTOOLBARBUTTON(UINT nID = 0, LPCTSTR szImage = NULL) 
		: 
		nMenuID(nID), 
		sImageID(szImage) 
	{
	}

	BOOL operator==(const TDCCUSTOMTOOLBARBUTTON& tbOther) const
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

typedef CArray<TDCCUSTOMTOOLBARBUTTON, TDCCUSTOMTOOLBARBUTTON&> CTDCToolbarButtonArray;

/////////////////////////////////////////////////////////////////////////////
// CTDLCustomToolbar window

class CTDLCustomToolbar : public CEnToolBar
{
// Construction
public:
	CTDLCustomToolbar();

// Operations
public:
	BOOL InitialiseButtons(const CTDCToolbarButtonArray& aButtons,
						   const CTDCMainMenu& mainMenu,
						   const CShortcutManager& mgrShortcuts);

	BOOL AppendTools(const CTDCUserToolArray& aTools, BOOL bGrouped = TRUE);
	int RemoveTools();
		
	// Must have exact same arrangement of 
	// buttons and separators as original initialisation
	BOOL ModifyButtonAttributes(const CTDCToolbarButtonArray& aButtons,
								const CTDCMainMenu& mainMenu);

	static BOOL RemapMenuItemIDs(const CMap<UINT, UINT, UINT, UINT&>& mapCmdIDs,
								 CTDCToolbarButtonArray& aButtons);

protected:
	CToolbarHelper m_tbHelper;
	int m_nInitBtnCount;

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
