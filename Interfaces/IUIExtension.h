// IUIExtension.h: interface and implementation of the IUIExtension class.
//
/////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IUIEXTENSION_H__7741547B_BA15_4851_A41B_2B4EC1DC12D5__INCLUDED_)
#define AFX_IUIEXTENSION_H__7741547B_BA15_4851_A41B_2B4EC1DC12D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////////

#include <Windows.h>

#include "ITaskList.h"

/////////////////////////////////////////////////////////////////////////////////

// function to be exported from dll to create instance of interface
#ifdef _EXPORTING // declare this in project settings for dll _only_
#	define DLL_DECLSPEC __declspec(dllexport)
#else
#	define DLL_DECLSPEC __declspec(dllimport)
#endif 

// Change this value when new interface methods get added
#define IUIEXTENSION_VERSION 0x0002

//////////////////////////////////////////////////////////////////////

class IUIExtensionWindow;
class IUIExtension;
class ITransText;
class IPreferences;

struct UITHEME;

//////////////////////////////////////////////////////////////////////

typedef IUIExtension* (*PFNCREATEUIEXT)(); // function prototype
extern "C" DLL_DECLSPEC IUIExtension* CreateUIExtensionInterface();

typedef int (*PFNGETVERSION)(); // function prototype
extern "C" DLL_DECLSPEC int GetInterfaceVersion();

//////////////////////////////////////////////////////////////////////

#pragma warning(disable:4505)
#pragma warning(disable:4189)

// helper method
static IUIExtension* CreateUIExtensionInterface(LPCWSTR szDllPath)
{
    IUIExtension* pInterface = NULL;
    HMODULE hDll = LoadLibraryW(szDllPath);
	
    if (hDll)
    {
        PFNCREATEUIEXT pCreate = (PFNCREATEUIEXT)GetProcAddress(hDll, "CreateUIExtensionInterface");
		
        if (pCreate)
		{
			try
			{
				if (!IUIEXTENSION_VERSION)
				{
					pInterface = pCreate();
				}
				else
				{
					// check version
					PFNGETVERSION pVersion = (PFNGETVERSION)GetProcAddress(hDll, "GetInterfaceVersion");

					if (pVersion && pVersion() >= IUIEXTENSION_VERSION)
						pInterface = pCreate();
				}
			}
			catch (...)
			{
			}
		}

		if (hDll && !pInterface)
			FreeLibrary(hDll);
    }
	
    return pInterface;
}

static BOOL IsUIExtensionDll(LPCWSTR szDllPath)
{
    HMODULE hDll = LoadLibraryW(szDllPath);
	
    if (hDll)
    {
		PFNCREATEUIEXT pCreateImp = (PFNCREATEUIEXT)GetProcAddress(hDll, "CreateUIExtensionInterface");
		FreeLibrary(hDll);

		return (pCreateImp != NULL);
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////

class IUIExtension
{
public:
    virtual void Release() = 0; // releases the interface

	virtual LPCWSTR GetMenuText() const = 0; // caller must copy result only
	virtual HICON GetIcon() const = 0;
	virtual LPCWSTR GetTypeID() const = 0; // caller must copy result only

	virtual void SetLocalizer(ITransText* pTT) = 0;

	virtual IUIExtensionWindow* CreateExtWindow(UINT nCtrlID, DWORD nStyle, 
												long nLeft, long nTop, long nWidth, long nHeight, 
												HWND hwndParent) = 0;
	
	virtual void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const = 0;
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey) = 0;
};

//////////////////////////////////////////////////////////////////////

enum IUI_UPDATETYPE
{ 
	// NEVER CHANGE THE ORDER OF THIS LIST
	IUI_EDIT		= 0,			// pTasks contains selected tasks only
	IUI_NEW,						// pTasks contains new task(s) and their parents
	IUI_DELETE,						// pTasks contains ALL tasks
//  IUI_

	IUI_ALL = 0xffff
};

enum IUI_APPCOMMAND
{ 
	// NEVER CHANGE THE ORDER OF THIS LIST
	IUI_NOCOMMAND	= -1,
	IUI_EXPANDALL	= 0,
	IUI_COLLAPSEALL,
	IUI_EXPANDSELECTED,
	IUI_COLLAPSESELECTED,
	IUI_SORT,						// IUIAPPCOMMANDDATA::nSortBy		[in]
	IUI_TOGGLABLESORT,				// IUIAPPCOMMANDDATA::nSortBy		[in]
	IUI_SETFOCUS,
	IUI_SELECTTASK,					// IUIAPPCOMMANDDATA::dwTaskID		[in]
	IUI_RESIZEATTRIBCOLUMNS,		   
	IUI_GETNEXTTASK,				// IUIAPPCOMMANDDATA::dwTaskID		[in/out]
	IUI_GETNEXTTOPLEVELTASK,		// IUIAPPCOMMANDDATA::dwTaskID		[in/out]
	IUI_GETPREVTASK,				// IUIAPPCOMMANDDATA::dwTaskID		[in/out]
	IUI_GETPREVTOPLEVELTASK,		// IUIAPPCOMMANDDATA::dwTaskID		[in/out]
	IUI_SAVETOIMAGE,				// IUIAPPCOMMANDDATA::szFilePath	[in/out]
	IUI_SETTASKFONT,				// IUIAPPCOMMANDDATA::hFont			[in]
	IUI_MULTISORT,					// IUIAPPCOMMANDDATA::sort			[in]
	IUI_SELECTFIRSTTASK,			// IUIAPPCOMMANDDATA::select		[in]
	IUI_SELECTNEXTTASK,				// IUIAPPCOMMANDDATA::select		[in]
	IUI_SELECTNEXTTASKINCLCURRENT,	// IUIAPPCOMMANDDATA::select		[in]
	IUI_SELECTPREVTASK,				// IUIAPPCOMMANDDATA::select		[in]
	IUI_SELECTLASTTASK,				// IUIAPPCOMMANDDATA::select		[in]
	IUI_MOVETASK,					// IUIAPPCOMMANDDATA::move			[in]

	// new values here
//  IUI_

};

enum IUI_HITTEST
{
	// NEVER CHANGE THE ORDER OF THIS LIST
	IUI_NOWHERE		= 0,
	IUI_TASKLIST,
	IUI_COLUMNHEADER,
	IUI_TASK,

	// new values here
};

enum IUI_ATTRIBUTE
{
	// NEVER CHANGE THE ORDER OF THIS LIST
	IUI_NONE		= 0,
	IUI_TASKNAME,			
	IUI_DONEDATE,			
	IUI_DUEDATE	,			
	IUI_STARTDATE,			
	IUI_PRIORITY,		
	IUI_COLOR,		
	IUI_ALLOCTO,		
	IUI_ALLOCBY,		
	IUI_STATUS,		
	IUI_CATEGORY,		
	IUI_PERCENT,		
	IUI_TIMEEST,		
	IUI_TIMESPENT,		
	IUI_FILEREF,		
	IUI_COMMENTS,		
	IUI_FLAG,		
	IUI_CREATIONDATE,		
	IUI_CREATEDBY,		
	IUI_RISK,				
	IUI_EXTERNALID,		
	IUI_COST,				
	IUI_DEPENDENCY,		
	IUI_RECURRENCE,		
	IUI_VERSION,			
	IUI_POSITION,		
	IUI_ID,		
	IUI_LASTMOD,		
	IUI_ICON,		
	IUI_TAGS,		
	IUI_CUSTOMATTRIB,		
	IUI_OFFSETTASK,		
	IUI_LOCK,
	IUI_SUBTASKDONE,
	IUI_TASKNAMEORCOMMENTS,
	IUI_ANYTEXTATTRIBUTE,

	// new values here
	// IUI_

	// ALWAYS THE LAST VALUE
	IUI_NUMATTRIBUTES
};

//////////////////////////////////////////////////////////////////////

struct IUITASKMOD
{
	IUI_ATTRIBUTE nAttrib;
	DWORD dwSelectedTaskID;		// 'zero' for _ALL_ selected tasks
	LPCWSTR szCustomAttribID;	// IUI_CUSTOMATTRIB

	// The attribute value
	union
	{
		int nValue;
		double dValue;
		LPCWSTR szValue;
		__int64 tValue;
		bool bValue;
		COLORREF crValue;
	};

	// Extra info
	union
	{
		TDC_UNITS nTimeUnits;	// IUI_TIMEEST, IUI_TIMESPENT, IUI_CUSTOMATTRIB
	};
};

//////////////////////////////////////////////////////////////////////

struct IUITASKMOVE
{
	DWORD dwSelectedTaskID;		// 'zero' for _ALL_ selected tasks
	
	DWORD dwParentID;
	DWORD dwAfterSiblingID;

	bool bCopy;
};

//////////////////////////////////////////////////////////////////////

struct IUIMULTISORT
{
	IUI_ATTRIBUTE nAttrib1;
	bool bAscending1;

	IUI_ATTRIBUTE nAttrib2;
	bool bAscending2;

	IUI_ATTRIBUTE nAttrib3;
	bool bAscending3;
};

//////////////////////////////////////////////////////////////////////

struct IUISELECTTASK
{
	IUI_ATTRIBUTE nAttrib; // IUI_TASKNAME, IUI_TASKNAMEORCOMMENTS or IUI_ANYTEXTATTRIBUTE
	bool bFindReplace;

	LPCWSTR szWords;	
	bool bCaseSensitive;
	bool bWholeWord;	
};

//////////////////////////////////////////////////////////////////////

struct IUIAPPCOMMANDDATA
{
	union
	{
		IUI_ATTRIBUTE nSortBy;
		DWORD dwTaskID;
		WCHAR szFilePath[MAX_PATH + 1];
		HFONT hFont;			
		IUIMULTISORT sort;
		IUISELECTTASK select;
		IUITASKMOVE move;	
	};
};

//////////////////////////////////////////////////////////////////////

// if   wParam == 0,		lParam = Task ID
// else wParam = LPDWORD,	lParam = ID count
const UINT WM_IUI_SELECTTASK			= ::RegisterWindowMessageW(L"WM_IUI_SELECTTASK"); 

// wParam = Number of Mods, lParam = &IUITASKMOD[0]
const UINT WM_IUI_MODIFYSELECTEDTASK	= ::RegisterWindowMessageW(L"WM_IUI_MODIFYSELECTEDTASK"); 

// wParam = 0, lParam = IUITASKMOVE*
const UINT WM_IUI_MOVESELECTEDTASK		= ::RegisterWindowMessageW(L"WM_IUI_MOVESELECTEDTASK"); 

// wParam = lParam = 0
const UINT WM_IUI_EDITSELECTEDTASKTITLE	= ::RegisterWindowMessageW(L"WM_IUI_EDITSELECTEDTASKTITLE"); 

// wParam = 0, lParam = Column ID
const UINT WM_IUI_SORTCOLUMNCHANGE		= ::RegisterWindowMessageW(L"WM_IUI_SORTCOLUMNCHANGE"); 

// wParam = 0, lParam = identifying string (LPCTSTR)
const UINT WM_IUI_DOHELP				= ::RegisterWindowMessageW(L"WM_IUI_DOHELP"); 

// WPARAM = Task ID, LPARAM = int* (imageIndex), return HIMAGELIST
const UINT WM_IUI_GETTASKICON			= ::RegisterWindowMessageW(L"WM_IUI_GETTASKICON"); 

//////////////////////////////////////////////////////////////////////

class IUIExtensionWindow
{
public:
	virtual HICON GetIcon() const = 0;
	virtual LPCWSTR GetMenuText() const = 0; // caller must copy result only
	virtual LPCWSTR GetTypeID() const = 0; // caller must copy result only

	virtual bool SelectTask(DWORD dwTaskID) = 0;
	virtual bool SelectTasks(const DWORD* pdwTaskIDs, int nTaskCount) = 0;

	virtual void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const IUI_ATTRIBUTE* pAttributes, int nNumAttributes) = 0;
	virtual bool WantTaskUpdate(IUI_ATTRIBUTE nAttribute) const = 0;
	virtual bool PrepareNewTask(ITaskList* pTask) const = 0;
	
	virtual bool ProcessMessage(MSG* pMsg) = 0;
	virtual void FilterToolTipMessage(MSG* pMsg) = 0;

	virtual bool DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData = NULL) = 0;
	virtual bool CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData = NULL) const = 0;

	virtual bool GetLabelEditRect(LPRECT pEdit) = 0; // screen coordinates
	virtual IUI_HITTEST HitTest(const POINT& ptScreen) const = 0;

	virtual void SetUITheme(const UITHEME* pTheme) = 0;
	virtual void SetReadOnly(bool bReadOnly) = 0;
	virtual HWND GetHwnd() const = 0;

	virtual void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const = 0;
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly) = 0;
};

//////////////////////////////////////////////////////////////////////

static void ReleaseUIExtensionInterface(IUIExtension*& pInterface)
{
    if (pInterface)
    {
        pInterface->Release();
        pInterface = NULL;
    }
}

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_IUIEXTENSION_H__7741547B_BA15_4851_A41B_2B4EC1DC12D5__INCLUDED_)
