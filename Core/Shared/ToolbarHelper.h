// ToolbarHelper.h: interface for the CToolbarHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBARHELPER_H__86A32540_80BF_421C_97E3_6E760BF427A8__INCLUDED_)
#define AFX_TOOLBARHELPER_H__86A32540_80BF_421C_97E3_6E760BF427A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "Subclass.h"
#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

class CShortcutManager;

//////////////////////////////////////////////////////////////////////

class CToolbarHelper : protected CSubclassWnd, protected CSubclasser 
{
public:
	CToolbarHelper();
	virtual ~CToolbarHelper();

	BOOL Initialize(CToolBar* pToolbar, CWnd* pToolbarParent, const CShortcutManager* pShortcutMgr = NULL);
	BOOL Release(BOOL bClearDropBtns = TRUE);
	void InitTooltips();

	BOOL ClearDropButton(UINT nBtnCmdID, BOOL bRedraw = TRUE);
	BOOL SetDropButton(UINT nBtnCmdID, UINT nMenuID, int nSubMenu, UINT nDefCmdID = 0, char cHotkey = 0);
	BOOL SetDefaultMenuID(UINT nBtnCmdID, UINT nDefMenuID = 0);
	BOOL ClearTooltip(UINT nBtnCmdID);
	BOOL SetTooltip(UINT nBtnCmdID, LPCTSTR szTip);
	BOOL SetTooltip(UINT nBtnCmdID, UINT nIDTip);
	CString GetTooltip(UINT nBtnCmdID) const;

	BOOL IsInitialized() { return IsHooked(); }

	void EnableMultilineText(BOOL bEnable = TRUE, int nWidth = 200);
	BOOL ProcessMessage(MSG* pMsg); // called by parent
	
	static void PrepareMenuItems(CMenu* pMenu, CWnd* pWnd);
	static CString GetResourceTip(UINT nID);

protected:
	CToolBar* m_pToolbar;
	CSubclassWnd m_scToolbar;

	const CShortcutManager* m_pShortcutMgr;

	struct THButton
	{
		UINT nMenuID;
		int nSubMenu;
		UINT nDefMenuID;
		TCHAR cHotKey;

#define MAX_TIP_LEN 128
		TCHAR szTip[MAX_TIP_LEN];
	};

	CMap<UINT, UINT, THButton, THButton&> m_mapTHButtons;
	BOOL m_bMultiline;
	int m_nMultilineWidth;
	CToolTipCtrl m_tt;
	CString m_sActiveTipText;
	UINT m_nIDLastShow;

protected:
	CString GetTip(UINT nID, const CPoint& point) const;
	CString GetTip(UINT nID, BOOL bOverDropBtn) const;
	BOOL DisplayDropMenu(UINT nCmdID, BOOL bPressBtn = FALSE);
	BOOL IsCmdEnabled(UINT nCmdID) const;
	BOOL SetButton(UINT nBtnCmdID, UINT nMenuID, int nSubMenu, UINT nDefCmdID, TCHAR cHotkey, LPCTSTR szTip);
	void RefreshTooltipRects();

	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);
};

#endif // !defined(AFX_TOOLBARHELPER_H__86A32540_80BF_421C_97E3_6E760BF427A8__INCLUDED_)
