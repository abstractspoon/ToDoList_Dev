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

	BOOL ClearDropButton(UINT nBtnCmdID, BOOL bRedraw = TRUE);
	BOOL SetDropButton(UINT nBtnCmdID, UINT nMenuID, int nSubMenu, UINT nDefCmdID = 0, char cHotkey = 0);
	BOOL SetDefaultMenuID(UINT nBtnCmdID, UINT nDefCmdID = 0);
	BOOL SetTooltip(UINT nBtnCmdID, LPCTSTR szTip);
	BOOL SetTooltip(UINT nBtnCmdID, UINT nIDTip);

	BOOL IsInitialized() { return IsHooked(); }

	void EnableMultilineText(BOOL bEnable = TRUE, int nWidth = 200);
	BOOL ProcessMessage(MSG* pMsg); // called by parent
	
	static void PrepareMenuItems(CMenu* pMenu, CWnd* pWnd);
	static CString GetTip(UINT nID);

protected:
	CToolBar* m_pToolbar;
	CSubclassWnd m_scToolbar;

	const CShortcutManager* m_pShortcutMgr;

	struct THButton
	{
		UINT nMenuID;
		int nSubMenu;
		UINT nDefCmdID;
		TCHAR cHotKey;
		TCHAR szTip[128];
	};

	CMap<UINT, UINT, THButton, THButton&> m_mapTHButtons;
	BOOL m_bMultiline;
	int m_nMultilineWidth;
	CToolTipCtrl m_tt;

protected:
	CString GetTip(UINT nID, LPPOINT pPoint) const;
	BOOL DisplayDropMenu(UINT nCmdID, BOOL bPressBtn = FALSE);
	BOOL IsCmdEnabled(UINT nCmdID) const;
	BOOL SetButton(UINT nBtnCmdID, UINT nMenuID, int nSubMenu, UINT nDefCmdID, TCHAR cHotkey, LPCTSTR szTip);
	void RefreshTooltipRects();
	void InitTooltips();

	LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);
};

#endif // !defined(AFX_TOOLBARHELPER_H__86A32540_80BF_421C_97E3_6E760BF427A8__INCLUDED_)
