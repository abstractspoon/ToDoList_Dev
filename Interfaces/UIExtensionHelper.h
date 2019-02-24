// TDCUIExtensionHelper.h: interface for the CTDCUIExtensionHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCUIEXTENSIONHELPER_H__AD55BB4A_A382_4DE3_9B26_531DC4EBBA88__INCLUDED_)
#define AFX_TDCUIEXTENSIONHELPER_H__AD55BB4A_A382_4DE3_9B26_531DC4EBBA88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Interfaces\IUIExtension.h"

//////////////////////////////////////////////////////////////////////

struct TDSORTCOLUMN;

class CUIExtensionAppCmdData : public IUIAPPCOMMANDDATA
{
public:
	// IUI_SAVETOIMAGE
	CUIExtensionAppCmdData(LPCWSTR szImagePath);

	// IUI_SORT
	// IUI_TOGGLABLESORT
	CUIExtensionAppCmdData(IUI_ATTRIBUTE nSortCol, BOOL bAscending);

	// IUI_SELECTTASK
	// IUI_GETNEXTTASK
	// IUI_GETNEXTTOPLEVELTASK
	// IUI_GETPREVTASK
	// IUI_GETPREVTOPLEVELTASK, 
	CUIExtensionAppCmdData(DWORD dwTaskID);

	// IUI_SETTASKFONT
	CUIExtensionAppCmdData(HFONT hFont);

	// IUI_MOVETASK
	CUIExtensionAppCmdData(DWORD dwSelTaskID, 
								DWORD dwParentID, 
								DWORD dwAfterSiblingID);
	// IUI_MULTISORT
	CUIExtensionAppCmdData(const IUIMULTISORT& sortCols);

	// IUI_SELECTFIRSTTASK
	// IUI_SELECTNEXTTASK
	// IUI_SELECTNEXTTASKINCLCURRENT
	// IUI_SELECTPREVTASK
	// IUI_SELECTLASTTASK
	CUIExtensionAppCmdData(IUI_ATTRIBUTE nAttrib, 
								BOOL bFindReplace, 
								LPCWSTR szWords, 
								BOOL bCaseSensitive, 
								BOOL bWholeWord);
};

//////////////////////////////////////////////////////////////////////

class CStringArray;
class CUIExtensionMgr;

class CUIExtensionHelper  
{
public:
	CUIExtensionHelper(UINT nFirstMenuID, int nMaxNumExtensions);

	void UpdateExtensionVisibilityState(CMenu* pMenu, const CUIExtensionMgr& mgr, const CStringArray& aTypeIDs) const;
	BOOL ProcessExtensionVisibilityMenuCmd(UINT nCmdID, const CUIExtensionMgr& mgr, CStringArray& aTypeIDs) const;

protected:
	UINT m_nFirstMenuID;
	int m_nMaxNumExtensions;

};

#endif // !defined(AFX_TDCUIEXTENSIONHELPER_H__AD55BB4A_A382_4DE3_9B26_531DC4EBBA88__INCLUDED_)
