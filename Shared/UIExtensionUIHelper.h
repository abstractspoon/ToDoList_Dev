// UIExtensionUIHelper.h: interface for the CUIExtensionUIHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UIEXTENSIONUIHELPER_H__AC709E3B_C876_43D8_A965_CAD2D2E7AD34__INCLUDED_)
#define AFX_UIEXTENSIONUIHELPER_H__AC709E3B_C876_43D8_A965_CAD2D2E7AD34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUIExtensionMgr;
class CMenuIconMgr;

class CUIExtensionUIHelper  
{
public:
	CUIExtensionUIHelper(const CUIExtensionMgr& mgrUIExt, UINT nStart, int nSize = 16);
	virtual ~CUIExtensionUIHelper();

	void UpdateMenu(CCmdUI* pCmdUI, CMenuIconMgr& mgrIcon, BOOL bEnabled = TRUE) const;
	void AppendExtensionsToToolbar(CToolBar& toolbar, UINT nCmdAfter);
	void RemoveExtensionsFromToolbar(CToolBar& toolbar, UINT nCmdAfter);

protected:
	const CUIExtensionMgr& m_mgrUIExt;
	UINT m_nStartID;
	int m_nSize;

};

#endif // !defined(AFX_UIEXTENSIONUIHELPER_H__AC709E3B_C876_43D8_A965_CAD2D2E7AD34__INCLUDED_)
