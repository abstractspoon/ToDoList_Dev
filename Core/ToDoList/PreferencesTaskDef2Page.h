#if !defined(AFX_PREFERENCESTASKDEF2PAGE_H__852964E3_4ABD_4B66_88BA_F553177616F2__INCLUDED_)
#define AFX_PREFERENCESTASKDEF2PAGE_H__852964E3_4ABD_4B66_88BA_F553177616F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesTaskDefPage.h : header file
//

#include "tdcenum.h"
#include "tdlprioritycombobox.h"
#include "tdlriskcombobox.h"
#include "tdcimagelist.h"
#include "tdlreminderperiodcombobox.h"
#include "tdcenumcontainers.h"

#include <afxtempl.h>

#include "..\shared\preferencesbase.h"
#include "..\shared\colorbutton.h"
#include "..\shared\timeedit.h"
#include "..\shared\autocombobox.h"
#include "..\shared\maskedit.h"
#include "..\Shared\checklistboxex.h"
#include "..\Shared\iconbutton.h"

/////////////////////////////////////////////////////////////////////////////
// CPreferencesTaskDef2Page dialog

class CPreferencesTaskDef2Page : public CPreferencesPageBase
{
	DECLARE_DYNCREATE(CPreferencesTaskDef2Page)

// Construction
public:
	CPreferencesTaskDef2Page();
	~CPreferencesTaskDef2Page();

	BOOL EnableCustomAttributeInheritance();

	int GetInheritParentAttributes(CTDCAttributeMap& mapAttribs, BOOL& bUpdateAttrib) const;

	int GetListItems(TDC_ATTRIBUTE nList, CStringArray& aItems) const;
	BOOL AddListItem(TDC_ATTRIBUTE nList, LPCTSTR szItem);
	BOOL DeleteListItem(TDC_ATTRIBUTE nList, LPCTSTR szItem);
	BOOL GetListIsReadonly(TDC_ATTRIBUTE nList) const;

protected:
// Dialog Data
	//{{AFX_DATA(CPreferencesTaskDef2Page)
	enum { IDD = IDD_PREFTASKDEF2_PAGE };
	//}}AFX_DATA

	BOOL	m_bUpdateInheritAttributes;
	CString	m_sDefCategoryList;
	CString	m_sDefStatusList;
	CString	m_sDefAllocToList;
	CString	m_sDefAllocByList;
	CString	m_sDefVersionList;
	CString	m_sDefTagList;
	BOOL	m_bCatListReadonly;
	BOOL	m_bStatusListReadonly;
	BOOL	m_bAllocToListReadonly;
	BOOL	m_bAllocByListReadonly;
	BOOL	m_bVersionListReadonly;
	BOOL	m_bTagListReadonly;
	CCheckListBoxEx	m_lbInheritAttrib;
	BOOL	m_bInheritParentAttributes;
	int		m_nSelAttribUse;

	struct ATTRIBPREF
	{
		ATTRIBPREF() : nAttrib(TDCA_NONE), bUse(FALSE) {}
		ATTRIBPREF(UINT nIDName, TDC_ATTRIBUTE attrib, BOOL use) : nAttrib(attrib), bUse(use) { sName.LoadString(nIDName); }

		CString sName;
		TDC_ATTRIBUTE nAttrib;
		BOOL bUse;
	};
	CArray<ATTRIBPREF, ATTRIBPREF&> m_aAttribPrefs;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesTaskDef2Page)
	//}}AFX_VIRTUAL

protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnFirstShow();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPreferencesTaskDef2Page)
	//}}AFX_MSG
	afx_msg void OnUseparentattrib();
	afx_msg void OnAttribUseChange();
	DECLARE_MESSAGE_MAP()
		
protected:
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
	virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
	
	void UpdateCustomAttributeInfoVisibility();
	BOOL HasCheckedAttributes() const;
	CString* GetListText(TDC_ATTRIBUTE nList);

	static CString LoadDefaultListItems(const IPreferences* pPrefs, LPCTSTR szKey);
	static void SaveDefaultListItems(LPCTSTR szValueList, IPreferences* pPrefs, LPCTSTR szKey);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESTASKDEF2PAGE_H__852964E3_4ABD_4B66_88BA_F553177616F2__INCLUDED_)
