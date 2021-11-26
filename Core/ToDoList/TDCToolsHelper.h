// ToolsHelper.h: interface for the CToolsHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLSHELPER_H__6BAD432D_0189_46A9_95ED_EF869CFC6CE1__INCLUDED_)
#define AFX_TOOLSHELPER_H__6BAD432D_0189_46A9_95ED_EF869CFC6CE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDCToolsCmdlineParser.h"
#include "TDCcustomattribdata.h"
#include "TDCStruct.h"

#include "..\shared\menuiconmgr.h"

//////////////////////////////////////////////////////////////////////

struct USERTOOL
{
	BOOL operator==(const USERTOOL& other) const;

	CString sToolName;
	CString sToolPath;
	CString sCmdline;
	BOOL bRunMinimized;
	CString sIconPath;
};

struct USERTOOLARGS
{
	CString sTasklist;
	CString sTaskIDs;
	CString sTaskTitle;
	CString sTaskExtID;
	CString sTaskComments;
	CString sTaskFileLink;
	CString sTaskAllocBy;
	CString sTaskAllocTo;
	CString sTaskPath;

	CTDCCustomAttributeDataMap mapTaskCustData;

	// For generating combo content
	TDCAUTOLISTDATA tdlListData;
};

//////////////////////////////////////////////////////////////////////

class CPreferencesDlg;
class CEnToolBar;
class CMenuIconMgr;

//////////////////////////////////////////////////////////////////////

typedef CArray<USERTOOL, USERTOOL&> CUserToolArray;
typedef CArray<int, int&> CToolIndexArray;

//////////////////////////////////////////////////////////////////////

class CTDCToolsHelper  
{
public:
	CTDCToolsHelper(BOOL bTDLEnabled, BOOL bISODates);
	virtual ~CTDCToolsHelper();
	
	BOOL RunTool(const USERTOOL& tool, const USERTOOLARGS& args, const CTDCCustomAttribDefinitionArray& aCustAttribDefs);
	BOOL TestTool(const USERTOOL& tool, const USERTOOLARGS& args, const CTDCCustomAttribDefinitionArray& aCustAttribDefs);
	void AddToolsToToolbar(const CUserToolArray& aTools, CEnToolBar& toolbar, UINT nCmdAfter, BOOL bGrouped = TRUE);
	void AddToolsToMenu(const CUserToolArray& aTools, CMenu& menu, CMenuIconMgr& mgrMenuIcons, BOOL bGrouped = TRUE);
	int RemoveToolsFromToolbar(CEnToolBar& toolbar, UINT nCmdAfter);

	BOOL PrepareCmdline(const USERTOOL& tool, const USERTOOLARGS& args, 
						const CTDCCustomAttribDefinitionArray& aCustAttribDefs, CString& sCmdline);

	static HICON GetToolIcon(const USERTOOL& tool);
	static BOOL GetToolIcon(const USERTOOL& tool, CBitmap& bmp, COLORREF crBkgnd);
	static CString GetToolPath(const USERTOOL& tool);
	static BOOL IsToolCmdID(UINT nCmdID);

protected:
	BOOL m_bTDLEnabled, m_bISODates;
	
protected:
   	LPCTSTR GetFileFilter();
	LPCTSTR GetDefaultFileExt();
	BOOL CheckToDoListVersionCompatibility(const CString& sToolPath) const;
	BOOL RunTestTool(const USERTOOL& tool, const USERTOOLARGS& args, const CTDCCustomAttribDefinitionArray& aCustAttribDefs, BOOL bTest);

	static BOOL GetToolPaths(const USERTOOL& tool, CString& sToolPath, CString& sIconPath);

	static BOOL ReplaceToolArgument(CTDCToolsCmdlineParser& tcp, CLA_TYPE nType, 
									const CString& sValue, BOOL bWebTool);
	static BOOL ReplaceToolArgument(CTDCToolsCmdlineParser& tcp, const CString& sName, 
									const CString& sValue, BOOL bWebTool);
	static CString EscapeCharacters(const CString& sValue, BOOL bWebTool);

	static int IndexArraySortProc(const void* pV1, const void* pV2);
	static int BuildToolIndexArray(const CUserToolArray& aTools, CToolIndexArray& aIndices, BOOL bGrouped);
	static BOOL GetToolButtonRange(const CEnToolBar& toolbar, int& nFirstBtn, int& nLastBtn);
};

#endif // !defined(AFX_TOOLSHELPER_H__6BAD432D_0189_46A9_95ED_EF869CFC6CE1__INCLUDED_)
