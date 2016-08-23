// UIExtensionUIHelper.cpp: implementation of the CUIExtensionUIHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UIExtensionUIHelper.h"
#include "UIExtensionmgr.h"
#include "sysimagelist.h"
#include "MenuIconMgr.h"
#include "toolbarhelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIExtensionUIHelper::CUIExtensionUIHelper(const CUIExtensionMgr& mgrUIExt, UINT nStart, int nSize) : 
	m_mgrUIExt(mgrUIExt), m_nStartID(nStart), m_nSize(nSize)
{

}

CUIExtensionUIHelper::~CUIExtensionUIHelper()
{

}

void CUIExtensionUIHelper::UpdateMenu(CCmdUI* pCmdUI, CMenuIconMgr& mgrIcon, BOOL bEnabled) const
{
	if (pCmdUI->m_pMenu)
	{
		ASSERT (m_nStartID == pCmdUI->m_nID);

		// delete existing tool entries and their icons first
		int nExt;
		for (nExt = 0; nExt < m_nSize; nExt++)
		{
			pCmdUI->m_pMenu->DeleteMenu(m_nStartID + nExt, MF_BYCOMMAND);
			mgrIcon.DeleteImage(m_nStartID + nExt);
		}
		
		// if we have any tools to add we do it here
		int nNumExt = m_mgrUIExt.GetNumUIExtensions();

		if (nNumExt)
		{
			int nPos = 0;
			UINT nFlags = MF_BYPOSITION | MF_STRING | (bEnabled ? 0 : MF_GRAYED);
			
			for (nExt = 0; nExt < m_nSize && nExt < nNumExt; nExt++)
			{
				CString sMenuItem, sText = m_mgrUIExt.GetUIExtensionMenuText(nExt);
								
				if (nPos < 9)
					sMenuItem.Format(_T("&%d %s"), nPos + 1, sText);
				else
					sMenuItem = sText;

				pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex++, nFlags, 
											m_nStartID + nExt, sMenuItem);

				// icon
				HICON hIcon = m_mgrUIExt.GetUIExtensionIcon(nExt);
				mgrIcon.AddImage(m_nStartID + nExt, hIcon);
				
				nPos++;
			}
			
			// update end menu count
			pCmdUI->m_nIndex--; // point to last menu added
			pCmdUI->m_nIndexMax = pCmdUI->m_pMenu->GetMenuItemCount();
			
			pCmdUI->m_bEnableChanged = TRUE;    // all the added items are enabled
		}
		else // if nothing to add just re-add placeholder
		{
			pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex, MF_BYPOSITION | MF_STRING | MF_GRAYED, 
				m_nStartID, _T("3rd Party Extensions"));
		}
	}
}

void CUIExtensionUIHelper::RemoveExtensionsFromToolbar(CToolBar& toolbar, UINT nCmdAfter)
{
	int nRemoved = 0;
	
	TBBUTTON tbb;
	CImageList* pIL = toolbar.GetToolBarCtrl().GetImageList();
	
	for (UINT nExtID = m_nStartID; nExtID <= m_nStartID + m_nSize; nExtID++)
	{
		int nBtn = toolbar.CommandToIndex(nExtID);
		
		if (nBtn != -1)
		{
			VERIFY(toolbar.GetToolBarCtrl().GetButton(nBtn, &tbb));
			
			if (toolbar.GetToolBarCtrl().DeleteButton(nBtn))
			{
				// delete the image too
				pIL->Remove(tbb.iBitmap);
				nRemoved++;
			}
		}
	}
	
	// remove separator
	if (nRemoved)
	{
		int nSep = toolbar.CommandToIndex(nCmdAfter) + 1;
		toolbar.GetToolBarCtrl().DeleteButton(nSep);
	}
}

void CUIExtensionUIHelper::AppendExtensionsToToolbar(CToolBar& toolbar, UINT nCmdAfter)
{
	// remove tools first
	RemoveExtensionsFromToolbar(toolbar, nCmdAfter);
	
	// then re-add
	int nNumExt = m_mgrUIExt.GetNumUIExtensions();

	if (nNumExt)
	{
		// start adding after the pref button
		int nStartPos = toolbar.CommandToIndex(nCmdAfter) + 1;
		int nAdded = 0;
		
		for (int nExt = 0; nExt < m_mgrUIExt.GetNumUIExtensions(); nExt++)
		{
			HICON hIcon = m_mgrUIExt.GetUIExtensionIcon(nExt);
				
			if (hIcon)
			{
				CImageList* pIL = toolbar.GetToolBarCtrl().GetImageList();
				int nImage = pIL->Add(hIcon);
				
				TBBUTTON tbb = { nImage, nExt + m_nStartID, 0, TBSTYLE_BUTTON, 0, 0, (UINT)-1 };
				
				if (toolbar.GetToolBarCtrl().InsertButton(nStartPos + nAdded, &tbb))
					nAdded++;
				else // remove image
					pIL->Remove(nImage);
				
				// Note: we do not delete the extensions icon
			}
		}
		
		// add a separator if any buttons added
		if (nAdded)
		{
			TBBUTTON tbb = { -1, 0, 0, TBSTYLE_SEP, 0, 0, (UINT)-1 };
			toolbar.GetToolBarCtrl().InsertButton(nStartPos, &tbb);
		}
	}
}


