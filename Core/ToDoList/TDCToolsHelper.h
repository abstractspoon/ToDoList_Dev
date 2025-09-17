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

struct TDCUSERTOOL
{
	TDCUSERTOOL();
	BOOL operator==(const TDCUSERTOOL& other) const;

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
class CTDCImageList;

//////////////////////////////////////////////////////////////////////

typedef CArray<TDCUSERTOOL, TDCUSERTOOL&> CTDCUserToolArray;

//////////////////////////////////////////////////////////////////////

class CTDCToolsHelper  
{
public:
	CTDCToolsHelper(BOOL bTDLEnabled);
	virtual ~CTDCToolsHelper();
	
	BOOL RunTool(const TDCUSERTOOL& tool, const USERTOOLARGS& args, const CTDCCustomAttribDefinitionArray& aCustAttribDefs);
	BOOL TestTool(const TDCUSERTOOL& tool, const USERTOOLARGS& args, const CTDCCustomAttribDefinitionArray& aCustAttribDefs);
	void AddToolsToToolbar(const CTDCUserToolArray& aTools, CEnToolBar& toolbar, UINT nCmdAfter, BOOL bGrouped = TRUE);
	void AddToolsToMenu(const CTDCUserToolArray& aTools, CMenu& menu, CMenuIconMgr& mgrMenuIcons, BOOL bGrouped = TRUE);
	int RemoveToolsFromToolbar(CEnToolBar& toolbar, UINT nCmdAfter);

	BOOL PrepareCmdline(const TDCUSERTOOL& tool, const USERTOOLARGS& args, 
						const CTDCCustomAttribDefinitionArray& aCustAttribDefs, CString& sCmdline);

	static CString GetToolPath(const TDCUSERTOOL& tool);
	static BOOL IsToolCmdID(UINT nCmdID);
	static int AddToolToImageList(const TDCUSERTOOL& tool, CTDCImageList& ilTools);

protected:
	BOOL m_bTDLEnabled;
	
protected:
   	LPCTSTR GetFileFilter();
	LPCTSTR GetDefaultFileExt();
	BOOL CheckToDoListVersionCompatibility(const CString& sToolPath) const;
	BOOL RunTestTool(const TDCUSERTOOL& tool, const USERTOOLARGS& args, const CTDCCustomAttribDefinitionArray& aCustAttribDefs, BOOL bTest);

	static BOOL ReplaceToolArgument(CTDCToolsCmdlineParser& tcp, CLA_TYPE nType, const CString& sValue, BOOL bWebTool);
	static BOOL ReplaceToolArgument(CTDCToolsCmdlineParser& tcp, const CString& sName, const CString& sValue, BOOL bWebTool);

	static CString EscapeCharacters(const CString& sValue, BOOL bWebTool);
	static int IndexArraySortProc(const void* pV1, const void* pV2);
	static BOOL GetToolButtonRange(const CEnToolBar& toolbar, int& nFirstBtn, int& nLastBtn);

	typedef CArray<int, int&> CTDCToolIndexArray;
	static int BuildToolIndexArray(const CTDCUserToolArray& aTools, CTDCToolIndexArray& aIndices, BOOL bGrouped);
};

#endif // !defined(AFX_TOOLSHELPER_H__6BAD432D_0189_46A9_95ED_EF869CFC6CE1__INCLUDED_)
