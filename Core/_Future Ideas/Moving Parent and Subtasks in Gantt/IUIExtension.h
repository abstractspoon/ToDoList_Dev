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

//////////////////////////////////////////////////////////////////////

const UINT IUIEXTENSION_VERSION = 2;

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

					if ((pVersion != NULL) && (pVersion() == IUIEXTENSION_VERSION))
						pInterface = pCreate();
				}
			}
			catch (...)
			{
			}
		}

		if (pInterface == NULL)
			FreeLibrary(hDll);
    }
	
    return pInterface;
}

static BOOL IsUIExtensionDll(LPCWSTR szDllPath)
{
    HMODULE hDll = LoadLibraryW(szDllPath);
	
    if (hDll)
    {
		PFNCREATEUIEXT pCreate = (PFNCREATEUIEXT)GetProcAddress(hDll, "CreateUIExtensionInterface");

		// Only free the library if we're NOT just about to reload it
		if (pCreate == NULL)
			FreeLibrary(hDll);

		return (pCreate != NULL);
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
	IUI_TOGGLABLESORT_DEPRECATED,	// IUIAPPCOMMANDDATA::nSortBy		[in]
	IUI_SETFOCUS,
	IUI_SELECTTASK,					// IUIAPPCOMMANDDATA::dwTaskID		[in]
	IUI_RESIZEATTRIBCOLUMNS,		   
	IUI_GETNEXTVISIBLETASK,			// IUIAPPCOMMANDDATA::dwTaskID		[in/out]
	IUI_GETNEXTTOPLEVELTASK,		// IUIAPPCOMMANDDATA::dwTaskID		[in/out]
	IUI_GETPREVVISIBLETASK,			// IUIAPPCOMMANDDATA::dwTaskID		[in/out]
	IUI_GETPREVTOPLEVELTASK,		// IUIAPPCOMMANDDATA::dwTaskID		[in/out]
	IUI_SAVETOIMAGE,				// IUIAPPCOMMANDDATA::szFilePath	[in/out]
	IUI_MULTISORT,					// IUIAPPCOMMANDDATA::sort			[in]
	IUI_SELECTFIRSTTASK,			// IUIAPPCOMMANDDATA::select		[in]
	IUI_SELECTNEXTTASK,				// IUIAPPCOMMANDDATA::select		[in]
	IUI_SELECTNEXTTASKINCLCURRENT,	// IUIAPPCOMMANDDATA::select		[in]
	IUI_SELECTPREVTASK,				// IUIAPPCOMMANDDATA::select		[in]
	IUI_SELECTLASTTASK,				// IUIAPPCOMMANDDATA::select		[in]
	IUI_MOVETASK,					// IUIAPPCOMMANDDATA::move			[in]
	IUI_GETNEXTTASK,				// IUIAPPCOMMANDDATA::dwTaskID		[in/out]
	IUI_GETPREVTASK,				// IUIAPPCOMMANDDATA::dwTaskID		[in/out]

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

//////////////////////////////////////////////////////////////////////

struct IUITASKMOD
{
	TDC_ATTRIBUTE nAttrib;
	DWORD dwSelectedTaskID;		// 'zero' for _ALL_ selected tasks
	LPCWSTR szCustomAttribID;	// TDCA_CUSTOMATTRIB

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
		TDC_UNITS nTimeUnits;	// TDCA_TIMEEST, TDCA_TIMESPENT, TDCA_CUSTOMATTRIB
		bool bCostIsRate;		// TDCA_COST
		bool bOffsetSubtasks;	// TDCA_OFFSETTASK
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
	TDC_ATTRIBUTE nAttrib1;
	bool bAscending1;

	TDC_ATTRIBUTE nAttrib2;
	bool bAscending2;

	TDC_ATTRIBUTE nAttrib3;
	bool bAscending3;
};

//////////////////////////////////////////////////////////////////////

struct IUISELECTTASK
{
	TDC_ATTRIBUTE nAttrib; // IUI_TASKNAME, IUI_TASKNAMEORCOMMENTS or IUI_ANYTEXTATTRIBUTE
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
		TDC_ATTRIBUTE nSortBy;
		DWORD dwTaskID;
		WCHAR szFilePath[MAX_PATH + 1];
		IUIMULTISORT sort;
		IUISELECTTASK select;
		IUITASKMOVE move;	
	};

	union // extra
	{
		bool bSortAscending;

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

// wParam = lParam = 0
const UINT WM_IUI_EDITSELECTEDTASKICON	= ::RegisterWindowMessageW(L"WM_IUI_EDITSELECTEDTASKICON"); 

// wParam = sort ascending, lParam = Column ID
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

	virtual void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate) = 0;
	virtual bool WantTaskUpdate(TDC_ATTRIBUTE nAttribute) const = 0;
	virtual bool PrepareNewTask(ITaskList* pTask) const = 0;
	
	virtual bool ProcessMessage(MSG* pMsg) = 0;
	virtual void FilterToolTipMessage(MSG* pMsg) = 0;

	virtual bool DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData = NULL) = 0;
	virtual bool CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData = NULL) const = 0;

	virtual bool GetLabelEditRect(LPRECT pEdit) = 0; // screen coordinates
	virtual IUI_HITTEST HitTest(POINT ptScreen) const = 0;
	virtual DWORD HitTestTask(POINT ptScreen, bool bTitleColumnOnly) const = 0;

	virtual void SetUITheme(const UITHEME* pTheme) = 0;
	virtual void SetTaskFont(HFONT hFont) = 0;

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
