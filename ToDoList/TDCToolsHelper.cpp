// ToolsHelper.cpp: implementation of the CToolsHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TDCToolsHelper.h"
#include "TDLToolsUserInputDlg.h"

#include "..\shared\holdredraw.h"
#include "..\shared\SysImageList.h"
#include "..\shared\enfiledialog.h"
#include "..\shared\enstring.h"
#include "..\shared\enbitmap.h"
#include "..\shared\filemisc.h"
#include "..\shared\preferences.h"
#include "..\shared\misc.h"
#include "..\shared\webmisc.h"

#include <shlwapi.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCToolsHelper::CTDCToolsHelper(BOOL bTDLEnabled, UINT nStart, int nSize) : m_nStartID(nStart), m_nSize(nSize), m_bTDLEnabled(bTDLEnabled)
{
	
}

CTDCToolsHelper::~CTDCToolsHelper()
{
	
}

BOOL CTDCToolsHelper::RunTool(const USERTOOL& tool, const USERTOOLARGS& args, CWnd* pWnd)
{
	return RunTestTool(tool, args, pWnd, FALSE);
}

BOOL CTDCToolsHelper::TestTool(const USERTOOL& tool, const USERTOOLARGS& args, CWnd* pWnd)
{
	return RunTestTool(tool, args, pWnd, TRUE);
}

BOOL CTDCToolsHelper::RunTestTool(const USERTOOL& tool, const USERTOOLARGS& args, CWnd* pWnd, BOOL bTest)
{
	if (!pWnd || !pWnd->GetSafeHwnd())
	{
		ASSERT(0);
		return FALSE;
	}
	
	CString sToolPath(GetToolPath(tool));

	if (!CheckToDoListVersionCompatibility(sToolPath, pWnd))
	{
		return FALSE;
	}
	else if (!FileMisc::FileExists(sToolPath))
	{
		CEnString sMessage;
		sMessage.Format(IDS_TH_TOOLNOTFOUND, sToolPath);

		pWnd->MessageBox(sMessage, CEnString(IDS_TH_RUNTOOLERROR_TITLE), MB_OK | MB_ICONERROR);
		return FALSE;
	}
	
	CString sCmdline;
	BOOL bEscapeSpaces = (WebMisc::IsBrowser(sToolPath) || WebMisc::IsURL(tool.sCmdline));
	
	if (!PrepareCmdline(tool, args, bEscapeSpaces, sCmdline))
	{
		return FALSE; // user cancelled dialog
	}

	if (bTest)
	{
		CEnString sMessage;
		sMessage.Format(IDS_TH_TESTTOOL, sToolPath, sCmdline);

		if (pWnd->MessageBox(sMessage, CEnString(IDS_TH_TESTTOOL_TITLE), MB_YESNO | MB_ICONQUESTION) == IDNO)
			return FALSE;
	}

	DWORD dwRes = FileMisc::Run(*pWnd, sToolPath, sCmdline, (tool.bRunMinimized ? SW_MINIMIZE : SW_SHOWNORMAL));
	
	// error handling
	if (dwRes <= 32)
		pWnd->MessageBox(CEnString(IDS_TH_RUNTOOLERROR, tool.sToolName), CEnString(IDS_TH_RUNTOOLERROR_TITLE), MB_OK | MB_ICONERROR);

	return (dwRes > 32);
}

BOOL CTDCToolsHelper::CheckToDoListVersionCompatibility(const CString& sToolPath, CWnd* pWnd) const
{
	if (!pWnd || !pWnd->GetSafeHwnd())
	{
		ASSERT(0);
		return FALSE;
	}

	// Check if we have anything to do first
	if (Misc::ToUpper(FileMisc::GetFileNameFromPath(sToolPath)) != _T("TODOLIST.EXE"))
		return TRUE;

	// Make sure tool version is same as 'our' version
	CDWordArray aToolVersion, aAppVersion;
	VERIFY (FileMisc::GetAppVersion(aAppVersion));
	VERIFY (FileMisc::GetModuleVersion(sToolPath, aToolVersion));

	if (FileMisc::CompareVersions(aAppVersion, aToolVersion, 2) == 0)
		return TRUE;

	// else
	CEnString sMessage;
	sMessage.Format(IDS_TH_TOOLVERSIONERROR, aToolVersion[0], aToolVersion[1], aAppVersion[0], aAppVersion[1]);

	pWnd->MessageBox(sMessage, CEnString(IDS_TH_TOOLVERSIONERROR_TITLE), MB_OK | MB_ICONERROR);

	return FALSE;
}

CString CTDCToolsHelper::GetToolPath(const USERTOOL& tool)
{
	CString sToolPath = tool.sToolPath;
	
	if (!sToolPath.IsEmpty() && !::PathIsURL(sToolPath))
	{
		if (!CTDCToolsCmdlineParser::PrepareToolPath(sToolPath))
			FileMisc::MakeFullPath(sToolPath, FileMisc::GetAppFolder());
	}

	return sToolPath;
}

BOOL CTDCToolsHelper::GetToolPaths(const USERTOOL& tool, CString& sToolPath, CString& sIconPath)
{
	sToolPath = GetToolPath(tool);
	sIconPath = tool.sIconPath;

	if (!tool.sIconPath.IsEmpty())
		sIconPath = FileMisc::GetFullPath(tool.sIconPath, FileMisc::GetAppFolder());
	else
		sIconPath.IsEmpty();

	return !sToolPath.IsEmpty();
}

HICON CTDCToolsHelper::GetToolIcon(CSysImageList& sil, const USERTOOL& tool)
{
	HICON hIcon = NULL;

	// Check valid tool path. Note: could also be url
	CString sToolPath, sIconPath;
	VERIFY(GetToolPaths(tool, sToolPath, sIconPath));

	if (FileMisc::FileExists(sToolPath) || FileMisc::FileExists(sIconPath) || ::PathIsURL(sToolPath))
	{
		if (tool.sIconPath.IsEmpty())
		{
			hIcon = sil.ExtractFileIcon(sToolPath);	
		}
		else // try for supported image
		{

			hIcon = CEnBitmap::LoadImageFileAsIcon(sIconPath, CLR_NONE, 16, 16);

			// All else/Fallback
			if (hIcon == NULL)
				hIcon = sil.ExtractFileIcon(sIconPath);
		}
	}

	return hIcon;
}

void CTDCToolsHelper::UpdateMenu(CCmdUI* pCmdUI, const CUserToolArray& tools, CMenuIconMgr& iconMgr)
{
	if (pCmdUI->m_pMenu)
	{
		CUserToolArray aTools;
		aTools.Copy(tools);
		
		// delete existing tool entries and their icons first
		for (int nTool = 0; nTool < m_nSize; nTool++)
		{
			pCmdUI->m_pMenu->DeleteMenu(pCmdUI->m_nID + nTool, MF_BYCOMMAND);
			iconMgr.DeleteImage(m_nStartID + nTool);
		}
		
		// if we have any tools to add we do it here
		if (aTools.GetSize())
		{
			// add valid tools only by first removing invalid items
			int nTool = aTools.GetSize();
			
			while (nTool--)
			{
				if (aTools[nTool].sToolName.IsEmpty() || aTools[nTool].sToolPath.IsEmpty())
					aTools.RemoveAt(nTool);
			}
			
			if (aTools.GetSize())
			{
				CSysImageList sil; // for menu icons
				VERIFY(sil.Initialize());
		
				int nPos = 0;
				
				for (nTool = 0; nTool < aTools.GetSize(); nTool++)
				{
					const USERTOOL& tool = aTools[nTool];
					CString sMenuItem;
					
					if (nPos < 9)
						sMenuItem.Format(_T("&%d %s"), nPos + 1, tool.sToolName);
					else
						sMenuItem = tool.sToolName;
					
					pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex++, MF_BYPOSITION | MF_STRING, 
						m_nStartID + nTool, sMenuItem);

					iconMgr.SetImage(m_nStartID + nTool, GetToolIcon(sil, tool));
					
					nPos++;
				}
				
				
				// update end menu count
				pCmdUI->m_nIndex--; // point to last menu added
				pCmdUI->m_nIndexMax = pCmdUI->m_pMenu->GetMenuItemCount();
				
				pCmdUI->m_bEnableChanged = TRUE;    // all the added items are enabled
			}
		}
		
		// if nothing to add just re-add placeholder
		if (!aTools.GetSize())
		{
			pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex, MF_BYPOSITION | MF_STRING | MF_GRAYED, 
				m_nStartID, CEnString(IDS_USERDEFINEDTOOLS));
		}
	}
}

BOOL CTDCToolsHelper::PrepareCmdline(const USERTOOL& tool, const USERTOOLARGS& args, BOOL bEscapeSpaces, CString& sCmdline)
{
	// do necessary substitutions
	CTDCToolsCmdlineParser tcp(tool.sCmdline);
	CString sTasklist(args.sTasklist);
	
	if (tcp.HasTasklistArgument())
	{
		// allow for no loaded file
		if (!FileMisc::FileExists(sTasklist))
		{
			CPreferences prefs;
			CFileOpenDialog dialog(IDS_SELECTTASKLIST_TITLE, 
									GetDefaultFileExt(), 
									NULL, 
									EOFN_DEFAULTOPEN, 
									GetFileFilter());
			
			if (dialog.DoModal(prefs) != IDOK)
				return FALSE;
			
			sTasklist = dialog.GetPathName();
		}
		
		CString sDrive, sPath, sFName, sExt;
		
		FileMisc::SplitPath(sTasklist, &sDrive, &sPath, &sFName, &sExt);
		
		ReplaceToolArgument(tcp, CLAT_PATHNAME, sTasklist, bEscapeSpaces);
		ReplaceToolArgument(tcp, CLAT_FOLDER, (sDrive + sPath), bEscapeSpaces);
		ReplaceToolArgument(tcp, CLAT_FILENAME, (sFName + sExt), bEscapeSpaces);
		ReplaceToolArgument(tcp, CLAT_FILETITLE, sFName, bEscapeSpaces);
	}
	
	ReplaceToolArgument(tcp, CLAT_TODOLIST, FileMisc::GetAppFilePath(), bEscapeSpaces);
	ReplaceToolArgument(tcp, CLAT_SELTASKID, args.sTaskIDs, bEscapeSpaces);
	ReplaceToolArgument(tcp, CLAT_SELTASKTITLE, args.sTaskTitle, bEscapeSpaces);
	ReplaceToolArgument(tcp, CLAT_SELTASKEXTID, args.sTaskExtID, bEscapeSpaces);
	ReplaceToolArgument(tcp, CLAT_SELTASKCOMMENTS, args.sTaskComments, bEscapeSpaces);
	ReplaceToolArgument(tcp, CLAT_SELTASKFILELINK, args.sTaskFileLink, bEscapeSpaces);
	ReplaceToolArgument(tcp, CLAT_SELTASKALLOCBY, args.sTaskAllocBy, bEscapeSpaces);
	ReplaceToolArgument(tcp, CLAT_SELTASKALLOCTO, args.sTaskAllocTo, bEscapeSpaces);

	CCLArgArray aCustomArgs;
	int nArg = tcp.GetCustomAttributeArguments(aCustomArgs);

	while (nArg--)
	{
		CString sAttribID(aCustomArgs[nArg].sName), sValue;
		args.mapTaskCustData.Lookup(Misc::ToUpper(sAttribID), sValue);

		ReplaceToolArgument(tcp, aCustomArgs[nArg].sPlaceHolder, sValue, bEscapeSpaces);
	}
	
	if (tcp.IsUserInputRequired())
	{
		CTDLToolsUserInputDlg dialog(tcp);
		
		if (dialog.DoModal(tool.sToolName) != IDOK)
			return FALSE;
		
		// process the results
		CCLArgArray aArgs;
		int nArg = tcp.GetUserArguments(aArgs);
		
		while (nArg--)
		{
			CString sResult(dialog.GetResult(aArgs[nArg].sName));
			ReplaceToolArgument(tcp, aArgs[nArg].sName, sResult, bEscapeSpaces);
		}
	}
	
	sCmdline = tcp.GetCmdLine();

	return TRUE;
}

BOOL CTDCToolsHelper::ReplaceToolArgument(CTDCToolsCmdlineParser& tcp, CLA_TYPE nType, 
										const CString& sValue, BOOL bEscapeSpaces)
{
	CString sArgValue(sValue);

	if (bEscapeSpaces)
		sArgValue.Replace(_T(" "), _T("%20"));

	return tcp.ReplaceArgument(nType, sArgValue);
}

BOOL CTDCToolsHelper::ReplaceToolArgument(CTDCToolsCmdlineParser& tcp, const CString& sName, 
										const CString& sValue, BOOL bEscapeSpaces)
{
	CString sArgValue(sValue);
	
	if (bEscapeSpaces)
		sArgValue.Replace(_T(" "), _T("%20"));
	
	return tcp.ReplaceArgument(sName, sArgValue);
}

void CTDCToolsHelper::RemoveToolsFromToolbar(CToolBar& toolbar, UINT nCmdAfter)
{
	int nRemoved = 0;
	
	TBBUTTON tbb;
	CImageList* pIL = toolbar.GetToolBarCtrl().GetImageList();
	
	for (UINT nToolID = m_nStartID; nToolID <= m_nStartID + m_nSize; nToolID++)
	{
		int nBtn = toolbar.CommandToIndex(nToolID);
		
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

void CTDCToolsHelper::AppendToolsToToolbar(const CUserToolArray& aTools, CToolBar& toolbar, UINT nCmdAfter)
{
	// remove tools first
	RemoveToolsFromToolbar(toolbar, nCmdAfter);
	
	// then re-add
	if (aTools.GetSize())
	{
		// figure out if we want the large or small images
		CSize sizeBtn(toolbar.GetToolBarCtrl().GetButtonSize());
		sizeBtn -= CSize(7, 7); // btn borders from BarTool.cpp

		CSysImageList sil((sizeBtn.cx > 16));
		VERIFY(sil.Initialize());
		
		// start adding after the pref button
		int nStartPos = toolbar.CommandToIndex(nCmdAfter) + 1;
		int nAdded = 0;
		
		for (int nTool = 0; nTool < aTools.GetSize(); nTool++)
		{
			const USERTOOL& tool = aTools[nTool];
			HICON hIcon = GetToolIcon(sil, tool);
				
			if (hIcon)
			{
				CImageList* pIL = toolbar.GetToolBarCtrl().GetImageList();
				int nImage = pIL->Add(hIcon);
				
				TBBUTTON tbb = { nImage, nTool + m_nStartID, 0, TBSTYLE_BUTTON, 0, 0, (UINT)-1 };
				
				if (toolbar.GetToolBarCtrl().InsertButton(nStartPos + nAdded, &tbb))
					nAdded++;
				else // remove image
					pIL->Remove(nImage);
				
				// cleanup
				::DestroyIcon(hIcon);
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

LPCTSTR CTDCToolsHelper::GetFileFilter()
{
	static CEnString TDLFILEFILTER(IDS_TDLFILEOPENFILTER);
	static CEnString XMLFILEFILTER(IDS_XMLFILEOPENFILTER);

	return m_bTDLEnabled ? TDLFILEFILTER : XMLFILEFILTER;
}

LPCTSTR CTDCToolsHelper::GetDefaultFileExt()
{
	static LPCTSTR TDLEXT = _T("tdl");
	static LPCTSTR XMLEXT = _T("xml");

	return m_bTDLEnabled ? TDLEXT : XMLEXT;
}

