// ImportExportUIHelper.cpp: implementation of the CImportExportUIHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImportExportUIHelper.h"
#include "ImportExportmgr.h"
#include "sysimagelist.h"
#include "MenuIconMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImportExportUIHelper::CImportExportUIHelper(const CImportExportMgr& mgrImpExp, CMenuIconMgr& mgrIcon) : 
	m_mgrImpExp(mgrImpExp), m_mgrIcon(mgrIcon)
{

}

CImportExportUIHelper::~CImportExportUIHelper()
{

}

void CImportExportUIHelper::UpdateExportMenu(CCmdUI* pCmdUI, int nMaxCount, BOOL bEnabled) const
{
	if (pCmdUI->m_pMenu)
	{
		UINT nStartID = pCmdUI->m_nID;

		// delete existing tool entries and their icons first
		int nTool;
		for (nTool = 0; nTool < nMaxCount; nTool++)
		{
			pCmdUI->m_pMenu->DeleteMenu(nStartID + nTool, MF_BYCOMMAND);
			m_mgrIcon.DeleteImage(nStartID + nTool);
		}
		
		// if we have any tools to add we do it here
		int nNumExporters = m_mgrImpExp.GetNumExporters();

		if (nNumExporters)
		{
			CSysImageList sil;

			int nPos = 0;
			UINT nFlags = MF_BYPOSITION | MF_STRING | (bEnabled ? 0 : MF_GRAYED);
			
			for (nTool = 0; nTool < nMaxCount && nTool < nNumExporters; nTool++)
			{
				CString sMenuItem, sText = m_mgrImpExp.GetExporterMenuText(nTool);
								
				if (nPos < 9)
					sMenuItem.Format(_T("&%d %s"), nPos + 1, sText);
				else
					sMenuItem = sText;

				pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex++, nFlags, 
											nStartID + nTool, sMenuItem);

				// icon
				CString sExt('.');
				sExt += m_mgrImpExp.GetExporterFileExtension(nTool);

				HICON hIcon = sil.ExtractFileIcon(sExt);
				m_mgrIcon.SetImage(nStartID + nTool, hIcon);
				
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
				nStartID, _T("3rd Party Exporters"));
		}
	}
}

void CImportExportUIHelper::UpdateImportMenu(CCmdUI* pCmdUI, int nMaxCount, BOOL bEnabled) const
{
	if (pCmdUI->m_pMenu)
	{
		UINT nStartID = pCmdUI->m_nID;

		// delete existing tool entries and their icons first
		for (int nTool = 0; nTool < nMaxCount; nTool++)
		{
			pCmdUI->m_pMenu->DeleteMenu(nStartID + nTool, MF_BYCOMMAND);
			m_mgrIcon.DeleteImage(nStartID + nTool);
		}
		
		// if we have any tools to add we do it here
		int nNumImporters = m_mgrImpExp.GetNumImporters();

		if (nNumImporters)
		{
//			CSysImageList sil;

			int nPos = 0;
			UINT nFlags = MF_BYPOSITION | MF_STRING | (bEnabled ? 0 : MF_GRAYED);
			
			for (int nTool = 0; nTool < nMaxCount && nTool < nNumImporters; nTool++)
			{
				CString sMenuItem, sText = m_mgrImpExp.GetImporterMenuText(nTool);
				
				if (nPos < 9)
					sMenuItem.Format(_T("&%d %s"), nPos + 1, sText);
				else
					sMenuItem = sText;
				
				pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex++, nFlags, 
											nStartID + nTool, sMenuItem);
				
				// icon
				CString sExt('.');
				sExt += m_mgrImpExp.GetImporterFileExtension(nTool);

//				HICON hIcon = sil.ExtractFileIcon(sExt);
//				m_mgrIcon.SetImage(nStartID + nTool, hIcon);
				
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
				nStartID, _T("3rd Party Importers"));
		}
	}
}

