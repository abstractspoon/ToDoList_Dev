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

class CTDCUIExtensionAppCmdData : public IUIAPPCOMMANDDATA
{
public:
	// IUI_SAVETOIMAGE
	CTDCUIExtensionAppCmdData(LPCWSTR szImagePath);

	// IUI_SORT
	// IUI_TOGGLABLESORT
	CTDCUIExtensionAppCmdData(IUI_ATTRIBUTE nSortCol, BOOL bAscending);

	// IUI_SELECTTASK
	// IUI_GETNEXTTASK
	// IUI_GETNEXTTOPLEVELTASK
	// IUI_GETPREVTASK
	// IUI_GETPREVTOPLEVELTASK, 
	CTDCUIExtensionAppCmdData(DWORD dwTaskID);

	// IUI_SETTASKFONT
	CTDCUIExtensionAppCmdData(HFONT hFont);

	// IUI_MOVETASK
	CTDCUIExtensionAppCmdData(DWORD dwSelTaskID, 
								DWORD dwParentID, 
								DWORD dwAfterSiblingID);
	// IUI_MULTISORT
	CTDCUIExtensionAppCmdData(const TDSORTCOLUMN* pCols);

	// IUI_SELECTFIRSTTASK
	// IUI_SELECTNEXTTASK
	// IUI_SELECTNEXTTASKINCLCURRENT
	// IUI_SELECTPREVTASK
	// IUI_SELECTLASTTASK
	CTDCUIExtensionAppCmdData(IUI_ATTRIBUTE nAttrib, 
								BOOL bFindReplace, 
								LPCWSTR szWords, 
								BOOL bCaseSensitive, 
								BOOL bWholeWord);
};

//////////////////////////////////////////////////////////////////////

class CStringArray;
class CUIExtensionMgr;

class CTDCUIExtensionHelper  
{
public:
	static void PrepareViewVisibilityMenu(CMenu* pMenu, const CUIExtensionMgr& mgr, const CStringArray& aTypeIDs);
	static BOOL ProcessViewVisibilityMenuCmd(UINT nCmdID, const CUIExtensionMgr& mgr, CStringArray& aTypeIDs);

};

#endif // !defined(AFX_TDCUIEXTENSIONHELPER_H__AD55BB4A_A382_4DE3_9B26_531DC4EBBA88__INCLUDED_)
