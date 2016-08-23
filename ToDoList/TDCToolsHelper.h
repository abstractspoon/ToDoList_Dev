// ToolsHelper.h: interface for the CToolsHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLSHELPER_H__6BAD432D_0189_46A9_95ED_EF869CFC6CE1__INCLUDED_)
#define AFX_TOOLSHELPER_H__6BAD432D_0189_46A9_95ED_EF869CFC6CE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDCToolsCmdlineParser.h"
#include "..\shared\menuiconmgr.h"

struct USERTOOL
{
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
	CMapStringToString mapTaskCustData;
};

class CSysImageList;
class CPreferencesDlg;

typedef CArray<USERTOOL, USERTOOL&> CUserToolArray;

class CTDCToolsHelper  
{
public:
	CTDCToolsHelper(BOOL bTDLEnabled, UINT nStart, int nSize = 16);
	virtual ~CTDCToolsHelper();
	
	void UpdateMenu(CCmdUI* pCmdUI, const CUserToolArray& tools, CMenuIconMgr& iconMgr);
	BOOL RunTool(const USERTOOL& tool, const USERTOOLARGS& args, CWnd* pWnd);
	BOOL TestTool(const USERTOOL& tool, const USERTOOLARGS& args, CWnd* pWnd);
	void AppendToolsToToolbar(const CUserToolArray& aTools, CToolBar& toolbar, UINT nCmdAfter);
	void RemoveToolsFromToolbar(CToolBar& toolbar, UINT nCmdAfter);
	
protected:
	UINT m_nStartID;
	int m_nSize;
	BOOL m_bTDLEnabled;
	
protected:
	BOOL PrepareCmdline(const USERTOOL& tool, const USERTOOLARGS& args, CString& sCmdline);
   	LPCTSTR GetFileFilter();
	LPCTSTR GetDefaultFileExt();
	BOOL CheckToDoListVersionCompatibility(const CString& sToolPath, CWnd* pWnd) const;
	BOOL RunTestTool(const USERTOOL& tool, const USERTOOLARGS& args, CWnd* pWnd, BOOL bTest);

	static HICON GetToolIcon(CSysImageList& sil, const USERTOOL& ut);
	static CString GetToolPath(const USERTOOL& tool);
	static BOOL GetToolPaths(const USERTOOL& tool, CString& sToolPath, CString& sIconPath);

};

#endif // !defined(AFX_TOOLSHELPER_H__6BAD432D_0189_46A9_95ED_EF869CFC6CE1__INCLUDED_)
