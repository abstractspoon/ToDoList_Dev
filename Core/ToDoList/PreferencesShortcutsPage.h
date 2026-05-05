#if !defined(AFX_TDLShortcutsTreeListCtrl_H__DA5D005D_C6CC_453A_A431_A2B85A920CE5__INCLUDED_)
#define AFX_TDLShortcutsTreeListCtrl_H__DA5D005D_C6CC_453A_A431_A2B85A920CE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// PrefererencesShortcutsPage.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#include "TDLShortcutsTreeListCtrl.h"

#include "..\shared\hotkeyctrlex.h"
#include "..\shared\preferencesbase.h"

/////////////////////////////////////////////////////////////////////////////

class CMenuIconMgr;
class CShortcutManager;

/////////////////////////////////////////////////////////////////////////////
// CPreferencesShortcutsPage dialog

class CPreferencesShortcutsPage : public CPreferencesPageBase 
{
public:
	CPreferencesShortcutsPage(const CMenuIconMgr& mgrIcons, CShortcutManager* pMgrShortcuts);
	~CPreferencesShortcutsPage();

	UINT GetShortcutCmdID(DWORD dwShortcut) const;
	BOOL RemapMenuItemIDs(const CMap<UINT, UINT, UINT, UINT&>& mapCmdIDs);

protected:
	CTDLShortcutsTreeListCtrl m_ctrlCommands;

	CHotKeyCtrlEx m_hkCur;
	CHotKeyCtrlEx m_hkNew;
	CEnString m_sOtherCmdID;
	BOOL m_bShowCommandIDs;

	CShortcutManager* m_pMgrShortcuts;

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnFirstShow();

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnSelchangedShortcuts(WPARAM wp, LPARAM lp);
	afx_msg void OnAssignshortcut();
	afx_msg void OnShowCmdIDs();
	afx_msg void OnCopyall();
	afx_msg void OnChangeShortcut();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_MESSAGE_MAP()

protected:
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
	virtual void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

	virtual int HighlightUIText(const CStringArray& aSearch, COLORREF crHighlight);
	virtual void ClearHighlights();
};

#endif // !defined(AFX_PREFERERENCESSHORTCUTSPAGE_H__DA5D005D_C6CC_453A_A431_A2B85A920CE5__INCLUDED_)
