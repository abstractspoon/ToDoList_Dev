// TDCUIExtensionHelper.h: interface for the CTDCUIExtensionHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCUIEXTENSIONHELPER_H__AD55BB4A_A382_4DE3_9B26_531DC4EBBA88__INCLUDED_)
#define AFX_TDCUIEXTENSIONHELPER_H__AD55BB4A_A382_4DE3_9B26_531DC4EBBA88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStringArray;
class CUIExtensionMgr;

class CTDCUIExtensionHelper  
{
public:
	static void PrepareViewVisibilityMenu(CMenu* pMenu, const CUIExtensionMgr& mgr, const CStringArray& aTypeIDs);
	static BOOL ProcessViewVisibilityMenuCmd(UINT nCmdID, const CUIExtensionMgr& mgr, CStringArray& aTypeIDs);

};

#endif // !defined(AFX_TDCUIEXTENSIONHELPER_H__AD55BB4A_A382_4DE3_9B26_531DC4EBBA88__INCLUDED_)
