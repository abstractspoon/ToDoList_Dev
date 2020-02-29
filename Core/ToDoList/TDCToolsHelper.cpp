// ToolsHelper.cpp: implementation of the CToolsHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TDCToolsHelper.h"
#include "TDLToolsUserInputDlg.h"

#include "..\shared\holdredraw.h"
#include "..\shared\enfiledialog.h"
#include "..\shared\enstring.h"
#include "..\shared\enbitmap.h"
#include "..\shared\filemisc.h"
#include "..\shared\misc.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\webmisc.h"
#include "..\shared\messagebox.h"
#include "..\shared\entoolbar.h"
#include "..\shared\fileicons.h"

#include "..\Interfaces\Preferences.h"

#include <shlwapi.h>

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

BOOL USERTOOL::operator==(const USERTOOL& other) const
{
	return ((sToolName == other.sToolName) &&
			(sToolPath == other.sToolPath) &&
			(sCmdline == other.sCmdline) &&
			(bRunMinimized == other.bRunMinimized) &&
			(sIconPath == other.sIconPath));
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDCToolsHelper::CTDCToolsHelper(BOOL bTDLEnabled, BOOL bISODates, UINT nStart, int nSize)
	: 
	m_nStartID(nStart), 
	m_nSize(nSize), 
	m_bTDLEnabled(bTDLEnabled),
	m_bISODates(bISODates)
{
	
}

CTDCToolsHelper::~CTDCToolsHelper()
{
	
}

BOOL CTDCToolsHelper::RunTool(const USERTOOL& tool, const USERTOOLARGS& args, const CTDCCustomAttribDefinitionArray& aCustAttribDefs)
{
	return RunTestTool(tool, args, aCustAttribDefs, FALSE);
}

BOOL CTDCToolsHelper::TestTool(const USERTOOL& tool, const USERTOOLARGS& args, const CTDCCustomAttribDefinitionArray& aCustAttribDefs)
{
	return RunTestTool(tool, args, aCustAttribDefs, TRUE);
}

BOOL CTDCToolsHelper::RunTestTool(const USERTOOL& tool, const USERTOOLARGS& args, const CTDCCustomAttribDefinitionArray& aCustAttribDefs, BOOL bTest)
{
	CString sToolPath(GetToolPath(tool));

	if (!CheckToDoListVersionCompatibility(sToolPath))
	{
		return FALSE;
	}
	else if (!FileMisc::FileExists(sToolPath))
	{
		CEnString sMessage;
		sMessage.Format(IDS_TH_TOOLNOTFOUND, sToolPath);

		CMessageBox::AfxShow(IDS_TH_RUNTOOLERROR_TITLE, sMessage, MB_OK | MB_ICONERROR);
		return FALSE;
	}
	
	CString sCmdline;
	
	if (!PrepareCmdline(tool, args, aCustAttribDefs, sCmdline))
	{
		return FALSE; // user cancelled dialog
	}

	if (bTest)
	{
		CEnString sMessage;
		sMessage.Format(IDS_TH_TESTTOOL, sToolPath, sCmdline);

		if (CMessageBox::AfxShow(IDS_TH_TESTTOOL_TITLE, sMessage, MB_YESNO | MB_ICONQUESTION) == IDNO)
			return FALSE;
	}

	DWORD dwRes = FileMisc::Run(*AfxGetMainWnd(), sToolPath, sCmdline, (tool.bRunMinimized ? SW_MINIMIZE : SW_SHOWNORMAL));
	
	// error handling
	if (dwRes < SE_ERR_SUCCESS)
		CMessageBox::AfxShow(IDS_TH_RUNTOOLERROR_TITLE, CEnString(IDS_TH_RUNTOOLERROR, tool.sToolName), MB_OK | MB_ICONERROR);

	return (dwRes >= SE_ERR_SUCCESS);
}

BOOL CTDCToolsHelper::CheckToDoListVersionCompatibility(const CString& sToolPath) const
{
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

	CMessageBox::AfxShow(IDS_TH_TOOLVERSIONERROR_TITLE, sMessage, MB_OK | MB_ICONERROR);

	return FALSE;
}

CString CTDCToolsHelper::GetToolPath(const USERTOOL& tool)
{
	CString sToolPath = tool.sToolPath;
	CTDCToolsCmdlineParser::PrepareToolPath(sToolPath, FALSE);

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

HICON CTDCToolsHelper::GetToolIcon(const USERTOOL& tool)
{
	HICON hIcon = NULL;

	// Check valid tool path. Note: could also be url
	CString sToolPath, sIconPath;
	
	if (GetToolPaths(tool, sToolPath, sIconPath))
	{
		if (tool.sIconPath.IsEmpty())
		{
			hIcon = CFileIcons::ExtractIcon(sToolPath);	
		}
		else // try for supported image
		{
			hIcon = CEnBitmap::LoadImageFileAsIcon(sIconPath, CLR_NONE, 16, 16);

			// All else/Fallback
			if (hIcon == NULL)
				hIcon = CFileIcons::ExtractIcon(sIconPath);
		}
	}

	return hIcon;
}

BOOL CTDCToolsHelper::GetToolIcon(const USERTOOL& tool, CBitmap& bmp, COLORREF crBkgnd)
{
	bmp.DeleteObject();

	// Check valid tool path. Note: could also be url
	CString sToolPath, sIconPath;
	
	if (GetToolPaths(tool, sToolPath, sIconPath))
	{
		if (tool.sIconPath.IsEmpty())
			return CFileIcons::GetImage(sToolPath, bmp, crBkgnd);	
		
		// else try for supported image
		int nSize = GraphicsMisc::ScaleByDPIFactor(16);
		CEnBitmap ebmp;

		if (ebmp.LoadImage(sIconPath, crBkgnd, nSize, nSize))
			return bmp.Attach(ebmp.Detach());

		return CFileIcons::GetImage(sIconPath, bmp, crBkgnd);
	}

	return FALSE;
}

BOOL CTDCToolsHelper::PrepareCmdline(const USERTOOL& tool, const USERTOOLARGS& args, 
									 const CTDCCustomAttribDefinitionArray& aCustAttribDefs, CString& sCmdline)
{
	CString sToolPath(GetToolPath(tool));
	BOOL bWebTool = (WebMisc::IsBrowser(sToolPath) || WebMisc::IsURL(tool.sCmdline));

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
		
		ReplaceToolArgument(tcp, CLAT_PATHNAME, sTasklist, bWebTool);
		ReplaceToolArgument(tcp, CLAT_FOLDER, (sDrive + sPath), bWebTool);
		ReplaceToolArgument(tcp, CLAT_FILENAME, (sFName + sExt), bWebTool);
		ReplaceToolArgument(tcp, CLAT_FILETITLE, sFName, bWebTool);
	}
	
	ReplaceToolArgument(tcp, CLAT_TODOLIST, FileMisc::GetAppFilePath(), bWebTool);
	ReplaceToolArgument(tcp, CLAT_SELTASKID, args.sTaskIDs, bWebTool);
	ReplaceToolArgument(tcp, CLAT_SELTASKTITLE, args.sTaskTitle, bWebTool);
	ReplaceToolArgument(tcp, CLAT_SELTASKEXTID, args.sTaskExtID, bWebTool);
	ReplaceToolArgument(tcp, CLAT_SELTASKCOMMENTS, args.sTaskComments, bWebTool);
	ReplaceToolArgument(tcp, CLAT_SELTASKFILELINK, args.sTaskFileLink, bWebTool);
	ReplaceToolArgument(tcp, CLAT_SELTASKALLOCBY, args.sTaskAllocBy, bWebTool);
	ReplaceToolArgument(tcp, CLAT_SELTASKALLOCTO, args.sTaskAllocTo, bWebTool);
	ReplaceToolArgument(tcp, CLAT_SELTASKPATH, args.sTaskPath, bWebTool);

	CCLArgArray aCustomArgs;
	TDCCADATA data;

	int nArg = tcp.GetCustomAttributeArguments(aCustomArgs);

	while (nArg--)
	{
		CString sAttribID(aCustomArgs[nArg].sName);
		args.mapTaskCustData.Lookup(sAttribID, data);

		ReplaceToolArgument(tcp, aCustomArgs[nArg].sPlaceHolder, data.AsString(), bWebTool);
	}
	
	if (tcp.IsUserInputRequired())
	{
		CCLArgArray aArgs;
		int nArg = tcp.GetUserArguments(aArgs);

		CTDLToolsUserInputDlg dialog(aArgs, args.tdlListData, aCustAttribDefs, m_bISODates);
		
		if (dialog.DoModal(tool.sToolName) != IDOK)
			return FALSE;
		
		// process the results
		while (nArg--)
		{
			CString sResult(dialog.GetResult(aArgs[nArg].sName));
			ReplaceToolArgument(tcp, aArgs[nArg].sName, sResult, bWebTool);
		}
	}
	
	sCmdline = tcp.GetCmdLine();

	return TRUE;
}

BOOL CTDCToolsHelper::ReplaceToolArgument(CTDCToolsCmdlineParser& tcp, CLA_TYPE nType, 
										const CString& sValue, BOOL bWebTool)
{
	return tcp.ReplaceArgument(nType, EscapeCharacters(sValue, bWebTool));
}

BOOL CTDCToolsHelper::ReplaceToolArgument(CTDCToolsCmdlineParser& tcp, const CString& sName, 
										const CString& sValue, BOOL bWebTool)
{
	return tcp.ReplaceArgument(sName, EscapeCharacters(sValue, bWebTool));
}

CString CTDCToolsHelper::EscapeCharacters(const CString& sValue, BOOL bWebTool)
{
	CString sEscaped(sValue);

	if (bWebTool)
	{
		// Escape embedded spaces
		sEscaped.Replace(_T(" "), _T("%20"));
	}
	else
	{
		// Double-up embedded quotes
		sEscaped.Replace(_T("\""), _T("\"\""));
	}

	return sEscaped;
}

void CTDCToolsHelper::RemoveToolsFromToolbar(CEnToolBar& toolbar, UINT nCmdAfter)
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
		if (nCmdAfter > 0)
			toolbar.DeleteItem(toolbar.CommandToIndex(nCmdAfter) + 1);
		else
			toolbar.GetToolBarCtrl().DeleteButton(0);
	}
}

void CTDCToolsHelper::AppendToolsToToolbar(const CUserToolArray& aTools, CEnToolBar& toolbar, UINT nCmdAfter)
{
	// remove tools first
	RemoveToolsFromToolbar(toolbar, nCmdAfter);
	
	// then re-add
	if (aTools.GetSize())
	{
		// figure out if we want the large or small images
		CSize sizeBtn(toolbar.GetToolBarCtrl().GetButtonSize());
		sizeBtn -= CSize(7, 7); // btn borders from BarTool.cpp

		// start adding after the pref button
		CImageList* pIL = toolbar.GetToolBarCtrl().GetImageList();
		int nStartPos = toolbar.CommandToIndex(nCmdAfter) + 1;
		int nAdded = 0;
		
		for (int nTool = 0; nTool < aTools.GetSize(); nTool++)
		{
			const USERTOOL& tool = aTools[nTool];
			CBitmap bmp;

			if (GetToolIcon(tool, bmp, 255))
			{
				int nImage = pIL->Add(&bmp, 255);
				
				TBBUTTON tbb = { nImage, nTool + m_nStartID, 0, TBSTYLE_BUTTON, 0, 0, (UINT)-1 };
				
				if (toolbar.GetToolBarCtrl().InsertButton(nStartPos + nAdded, &tbb))
					nAdded++;
				else
					pIL->Remove(nImage);
			}
		}
		
		// add a separator if any buttons added
		if (nAdded)
			toolbar.InsertSeparator(nStartPos);
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

