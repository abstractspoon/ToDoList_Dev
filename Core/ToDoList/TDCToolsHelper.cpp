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
#include "..\shared\enmenu.h"

#include "..\Interfaces\Preferences.h"

#include <shlwapi.h>

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const int MAX_NUM_TOOLS = 50;

const UINT FIRST_TOOLID = ID_TOOLS_USERTOOL1;
const UINT LAST_TOOLID = (ID_TOOLS_USERTOOL1 + MAX_NUM_TOOLS - 1);

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

CTDCToolsHelper::CTDCToolsHelper(BOOL bTDLEnabled)
	: 
	m_bTDLEnabled(bTDLEnabled)
{
	
}

CTDCToolsHelper::~CTDCToolsHelper()
{
	
}

BOOL CTDCToolsHelper::IsToolCmdID(UINT nCmdID)
{
	return ((nCmdID >= FIRST_TOOLID) && (nCmdID <= LAST_TOOLID));
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

		CTDLToolsUserInputDlg dialog(aArgs, args.tdlListData, aCustAttribDefs);
		
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

int CTDCToolsHelper::RemoveToolsFromToolbar(CEnToolBar& toolbar, UINT nCmdAfter)
{
	ASSERT(toolbar.GetSafeHwnd());

	int nNumRemoved = 0, nFirstBtn, nLastBtn;

	if (GetToolButtonRange(toolbar, nFirstBtn, nLastBtn))
	{
		// Remove the range working backwards
		CToolBarCtrl& tbc = toolbar.GetToolBarCtrl();
		CImageList* pIL = tbc.GetImageList();

		for (int nBtn = nLastBtn; nBtn >= nFirstBtn; nBtn--)
		{
			if (!toolbar.IsItemSeparator(nBtn))
			{
				// delete the image too
				TBBUTTON tbb;
				VERIFY(toolbar.GetToolBarCtrl().GetButton(nBtn, &tbb));
			
				pIL->Remove(tbb.iBitmap);
				nNumRemoved++; // only count actual tools
			}

			VERIFY(toolbar.GetToolBarCtrl().DeleteButton(nBtn));
		}
	
		// Cleanup extra separator added in AddToolsToToolbar
		if (nNumRemoved)
		{
			toolbar.DeleteDuplicateSeparators(toolbar.CommandToIndex(nCmdAfter) + 1);
			toolbar.DeleteTrailingSeparator();
		}
	}

	return nNumRemoved;
}

BOOL CTDCToolsHelper::GetToolButtonRange(const CEnToolBar& toolbar, int& nFirstBtn, int& nLastBtn)
{
	nFirstBtn = nLastBtn = -1;

	// Tools are a contiguous range
	int nNumBtns = toolbar.GetToolBarCtrl().GetButtonCount(), nBtn;
	
	// First
	for (nBtn = 0; nBtn < nNumBtns; nBtn++)
	{
		if (IsToolCmdID(toolbar.GetItemID(nBtn)))
		{
			nFirstBtn = nBtn;
			break;
		}
	}

	// Last
	nBtn = nNumBtns;

	while (nBtn--)
	{
		if (IsToolCmdID(toolbar.GetItemID(nBtn)))
		{
			nLastBtn = nBtn;
			break;
		}
	}

	return ((nFirstBtn != -1) && (nLastBtn >= nFirstBtn));
}

void CTDCToolsHelper::AddToolsToToolbar(const CUserToolArray& aTools, CEnToolBar& toolbar, UINT nCmdAfter, BOOL bGrouped)
{
	ASSERT(toolbar.GetSafeHwnd());

	CToolIndexArray aIndices;
	int nNumItems = BuildToolIndexArray(aTools, aIndices, bGrouped);

	if (nNumItems == 0)
		return;

	// figure out if we want the large or small images
	CSize sizeBtn(toolbar.GetToolBarCtrl().GetButtonSize());
	sizeBtn -= CSize(7, 7); // btn borders from BarTool.cpp

	CImageList* pIL = toolbar.GetToolBarCtrl().GetImageList();

	// work out where to start adding
	int nStartPos = toolbar.CommandToIndex(nCmdAfter) + 1;

	// Leading separator
	toolbar.InsertSeparator(nStartPos);
		
	for (int nItem = 0, nAdded = 1; nItem < nNumItems; nItem++)
	{
		const int nTool = aIndices[nItem];
		int nPos = (nStartPos + nAdded);

		if (nTool == -1)
		{
			// Prevent duplicate separators
			if (!toolbar.IsItemSeparator(nPos - 1) && toolbar.InsertSeparator(nPos))
			{
				nAdded++;
			}
		}
		else
		{
			CBitmap bmp;

			if (GetToolIcon(aTools[nTool], bmp, 255))
			{
				int nImage = pIL->Add(&bmp, 255);

				TBBUTTON tbb = { nImage, (nTool + (int)FIRST_TOOLID), 0, TBSTYLE_BUTTON, 0, 0, (UINT)-1 };

				if (toolbar.GetToolBarCtrl().InsertButton(nPos, &tbb))
				{
					nAdded++;
				}
				else
				{
					pIL->Remove(nImage);
				}
			}
		}
	}

	toolbar.DeleteDuplicateSeparators(nStartPos);
}

void CTDCToolsHelper::AddToolsToMenu(const CUserToolArray& aTools, CMenu& menu, CMenuIconMgr& mgrMenuIcons, BOOL bGrouped)
{
	// Before deleting, work out where we will re-insert
	const int MENUSTARTPOS = CEnMenu::FindFirstMenuItem(menu, FIRST_TOOLID, LAST_TOOLID);
	const UINT MENUSTARTID = menu.GetMenuItemID(MENUSTARTPOS);

	// delete existing tool entries and their icons
	int nTool = MAX_NUM_TOOLS;

	while (nTool--)
	{
		menu.DeleteMenu(FIRST_TOOLID + nTool, MF_BYCOMMAND);
		mgrMenuIcons.DeleteImage(FIRST_TOOLID + nTool);
	}

	CEnMenu::RemoveDuplicateSeparators(menu, MENUSTARTPOS);

	// if we have any tools to add we do it here
	CToolIndexArray aIndices;
	int nNumItems = CTDCToolsHelper::BuildToolIndexArray(aTools, aIndices, bGrouped);

	if (nNumItems)
	{
		for (int nItem = 0; nItem < nNumItems; nItem++)
		{
			const int nTool = aIndices[nItem];

			if (nTool == -1)
			{
				menu.InsertMenu(MENUSTARTPOS + nItem, MF_BYPOSITION | MF_SEPARATOR);
			}
			else
			{
				const USERTOOL& tool = aTools[nTool];
				CString sMenuItem(tool.sToolName);

				if (nTool < 9)
					sMenuItem.Format(_T("&%d %s"), nTool + 1, tool.sToolName); // add accelerator

				menu.InsertMenu(MENUSTARTPOS + nItem, MF_BYPOSITION | MF_STRING, MENUSTARTID + nTool, sMenuItem);

				// Icon manager will free the icon
				mgrMenuIcons.SetImage(MENUSTARTID + nTool, CTDCToolsHelper::GetToolIcon(tool));
			}
		}
	}
	else // if nothing to add just re-add placeholder
	{
		menu.InsertMenu(MENUSTARTPOS, MF_BYPOSITION | MF_STRING | MF_GRAYED, MENUSTARTID, CEnString(IDS_USERDEFINEDTOOLS));
	}
}

LPCTSTR CTDCToolsHelper::GetFileFilter()
{
	static CEnString TDLFILEFILTER(IDS_TDLFILEFILTER);
	static CEnString XMLFILEFILTER(IDS_XMLFILEFILTER);

	return m_bTDLEnabled ? TDLFILEFILTER : XMLFILEFILTER;
}

LPCTSTR CTDCToolsHelper::GetDefaultFileExt()
{
	static LPCTSTR TDLEXT = _T("tdl");
	static LPCTSTR XMLEXT = _T("xml");

	return m_bTDLEnabled ? TDLEXT : XMLEXT;
}

int CTDCToolsHelper::BuildToolIndexArray(const CUserToolArray& aTools, CToolIndexArray& aIndices, BOOL bGrouped)
{
	aIndices.RemoveAll();

	int nNumTools = aTools.GetSize(), nNumGroups = 1;

	if (nNumTools == 0)
		return 0;

	// else
	nNumTools = min(nNumTools, MAX_NUM_TOOLS);

	for (int nTool = 0; nTool < nNumTools; nTool++)
		aIndices.Add(nTool);

	if (bGrouped && (nNumTools > 2))
	{
		// Separate tools by type
		CMapStringToContainer<CToolIndexArray> mapTools;

		for (int nTool = 0; nTool < nNumTools; nTool++)
		{
			CString sFileName, sExt;
			FileMisc::SplitPath(aTools[nTool].sToolPath, NULL, NULL, &sFileName, &sExt);

			sFileName.MakeLower();
			sExt.MakeLower();

			CString sGroup(sExt); // default

			// Special cases: executables and folders
			if (sExt == _T(".exe"))
			{
				sGroup = (sFileName + sExt);
			}
			else if (sExt.IsEmpty())
			{
				sGroup = _T("_FOLDER_");
			}
				
			CToolIndexArray* pIndices = mapTools.GetAddMapping(sGroup);
			ASSERT(pIndices);

			pIndices->Add(nTool);
		}

		nNumGroups = mapTools.GetCount();

		if (nNumGroups > 1)
		{
			// Sort grouped arrays by their first index for overall ordering
			CArray<CToolIndexArray*, CToolIndexArray*&> aIndexArrays;

			POSITION pos = mapTools.GetStartPosition();
			CString sUnused;
			CToolIndexArray* pIndices;

			while (pos)
			{
				mapTools.GetNextAssoc(pos, sUnused, pIndices);
				aIndexArrays.Add(pIndices);
			}
						
			Misc::SortArrayT<CToolIndexArray*>(aIndexArrays, IndexArraySortProc);

			// Build final index array with separators between groups
			aIndices.RemoveAll();

			for (int nGroup = 0; nGroup < nNumGroups; nGroup++)
			{
				aIndices.Append(*aIndexArrays[nGroup]);

				// Omit trailing separator
				if (nGroup < (nNumGroups - 1))
				{
					int nSeparator = -1;
					aIndices.Add(nSeparator);
				}
			}
		}
	}

	ASSERT(aIndices.GetSize() == (aTools.GetSize() + nNumGroups - 1));

	return aIndices.GetSize();
}

int CTDCToolsHelper::IndexArraySortProc(const void* pV1, const void* pV2)
{
	typedef CToolIndexArray* PToolIndexArray;

	const CToolIndexArray* pIndices1 = *(static_cast<const PToolIndexArray*>(pV1));
	const CToolIndexArray* pIndices2 = *(static_cast<const PToolIndexArray*>(pV2));

	// Compare first index
	return (pIndices1->GetAt(0) - pIndices2->GetAt(0));
}
