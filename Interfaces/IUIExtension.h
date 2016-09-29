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

#define IUIEXTENSION_VERSION 0x0001

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
	IUI_EDIT,	// pTasks contains selected tasks only
	IUI_NEW,	// pTasks contains new task(s) and their parents
	IUI_DELETE, // pTasks contains ALL tasks
	IUI_MOVE,	// pTasks contains ALL tasks
//  IUI_

	IUI_ALL = 0xffff
};

enum IUI_APPCOMMAND
{ 
	// NEVER CHANGE THE ORDER OF THIS LIST
	IUI_EXPANDALL,
	IUI_COLLAPSEALL,
	IUI_EXPANDSELECTED,
	IUI_COLLAPSESELECTED,
	IUI_SORT,					// dwExtra is column ID
	IUI_TOGGLABLESORT,			// dwExtra is column ID
	IUI_SETFOCUS,
	IUI_RESIZEATTRIBCOLUMNS,
	IUI_GETNEXTTASK,			// dwExtra is DWORD* 
	IUI_GETNEXTTOPLEVELTASK,	// dwExtra is DWORD*
	IUI_GETPREVTASK,			// dwExtra is DWORD* 
	IUI_GETPREVTOPLEVELTASK,	// dwExtra is DWORD*

	// new values here
//  IUI_

};

enum IUI_HITTEST
{
	// NEVER CHANGE THE ORDER OF THIS LIST
	IUI_NOWHERE,
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

	// new values here
};

//////////////////////////////////////////////////////////////////////

struct IUITASKMOD
{
	IUI_ATTRIBUTE nAttrib;

	// The attribute value
	union
	{
		int nValue;
		double dValue;
		LPCWSTR szValue;
		__int64 tValue;
		BOOL bValue;
		COLORREF crValue;
	};

	// Extra info
	union
	{
		LPCWSTR szCustomAttribID; // IUI_CUSTOMATTRIB
		TDC_UNITS nTimeUnits;     // IUI_TIMEEST, IUI_TIMESPENT
	};
};

//////////////////////////////////////////////////////////////////////

// if   wParam == 0,		lParam = Task ID
// else wParam = LPDWORD,	lParam = ID count
const UINT WM_IUI_SELECTTASK			= ::RegisterWindowMessageW(L"WM_IUI_SELECTTASK"); 

// wParam = Number of Mods, lParam == &IUITASKMOD[0]
const UINT WM_IUI_MODIFYSELECTEDTASK	= ::RegisterWindowMessageW(L"WM_IUI_MODIFYSELECTEDTASK"); 

// wParam = lParam == 0
const UINT WM_IUI_EDITSELECTEDTASKTITLE	= ::RegisterWindowMessageW(L"WM_IUI_EDITSELECTEDTASKTITLE"); 

// wParam = 0, lParam == Column ID
const UINT WM_IUI_SORTCOLUMNCHANGE		= ::RegisterWindowMessageW(L"WM_IUI_SORTCOLUMNCHANGE"); 

// wParam = 0, lParam == identifying string (LPCTSTR)
const UINT WM_IUI_DOHELP				= ::RegisterWindowMessageW(L"WM_IUI_DOHELP"); 

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
	virtual bool WantEditUpdate(IUI_ATTRIBUTE nAttribute) const = 0;
	virtual bool WantSortUpdate(IUI_ATTRIBUTE nAttribute) const = 0;
	virtual bool PrepareNewTask(ITaskList* pTask) const = 0;
	
	virtual bool ProcessMessage(MSG* pMsg) = 0;
	virtual bool DoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra = 0) = 0;
	virtual bool CanDoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra = 0) const = 0;

	virtual bool GetLabelEditRect(LPRECT pEdit) = 0; // screen coordinates
	virtual IUI_HITTEST HitTest(const POINT& ptScreen) const = 0;

	virtual void SetUITheme(const UITHEME* pTheme) = 0;
	virtual void SetReadOnly(bool bReadOnly) = 0;
	virtual HWND GetHwnd() const = 0;

	virtual void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const = 0;
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly) = 0;

	virtual void Release() = 0;
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
