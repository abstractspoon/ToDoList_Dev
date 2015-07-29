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

/////////////////////////////////////////////////////////////////////////////////

// function to be exported from dll to create instance of interface
#ifdef _EXPORTING // declare this in project settings for dll _only_
#	define DLL_DECLSPEC __declspec(dllexport)
#else
#	define DLL_DECLSPEC __declspec(dllimport)
#endif 

#define IUIEXTENSION_VERSION 0x0000

//////////////////////////////////////////////////////////////////////

class IUIExtensionWindow;
class IUIExtension;
class ITransText;
class ITaskList;
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
    HMODULE hDll = LoadLibrary(szDllPath);
	
    if (hDll)
    {
        PFNCREATEUIEXT pCreate = (PFNCREATEUIEXT)GetProcAddress(hDll, "CreateUIExtensionInterface");
		
        if (pCreate)
		{
			if (!IUIEXTENSION_VERSION)
				pInterface = pCreate();
			else
			{
				// check version
				PFNGETVERSION pVersion = (PFNGETVERSION)GetProcAddress(hDll, "GetInterfaceVersion");

				if (pVersion && pVersion() >= IUIEXTENSION_VERSION)
					pInterface = pCreate();
			}
		}

		if (hDll && !pInterface)
			FreeLibrary(hDll);
    }
	
    return pInterface;
}

static BOOL IsUIExtemsionDll(LPCWSTR szDllPath)
{
    HMODULE hDll = LoadLibrary(szDllPath);
	
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
};

//////////////////////////////////////////////////////////////////////

enum IUI_UPDATETYPE
{ 
	IUI_EDIT,	// pTasks contains selected tasks only
	IUI_ADD,	// pTasks contains ALL tasks
	IUI_DELETE, // pTasks contains ALL tasks
	IUI_MOVE,	// pTasks contains ALL tasks
//  IUI_

	IUI_ALL = 0xffff
};

enum IUI_APPCOMMAND
{ 
	IUI_EXPANDALL,
	IUI_COLLAPSEALL,
	IUI_EXPANDSELECTED,
	IUI_COLLAPSESELECTED,
	IUI_SORT,				// dwExtra is column ID
	IUI_TOGGLABLESORT,		// dwExtra is column ID
	IUI_SETFOCUS,
	IUI_RESIZEATTRIBCOLUMNS,
//  IUI_

};

enum IUI_HITTEST
{
	IUI_NOWHERE,
	IUI_TASKLIST,
	IUI_COLUMNHEADER,
	IUI_TASK,
};

//////////////////////////////////////////////////////////////////////

struct IUITASKMOD
{
	int nAttrib;

	union
	{
		int nValue;
		double dValue;
		LPCWSTR szValue;
		__int64 tValue;
		BOOL bValue;
		COLORREF crValue;
	};
};

//////////////////////////////////////////////////////////////////////

// wParam = 0, lParam == Task ID
const UINT WM_IUI_SELECTTASK			= ::RegisterWindowMessage(_T("WM_IUI_SELECTTASK")); 

// wParam = Number of Mods, lParam == IUITASKMOD[0]
const UINT WM_IUI_MODIFYSELECTEDTASK	= ::RegisterWindowMessage(_T("WM_IUI_MODIFYSELECTEDTASK")); 

// wParam = lParam == 0
const UINT WM_IUI_EDITSELECTEDTASKTITLE	= ::RegisterWindowMessage(_T("WM_IUI_EDITSELECTEDTASKTITLE")); 

// wParam = 0, lParam == Column ID
const UINT WM_IUI_SORTCOLUMNCHANGE		= ::RegisterWindowMessage(_T("WM_IUI_SORTCOLUMNCHANGE")); 

//////////////////////////////////////////////////////////////////////

class IUIExtensionWindow
{
public:
	virtual HICON GetIcon() const = 0;
	virtual LPCWSTR GetMenuText() const = 0; // caller must copy result only
	virtual LPCWSTR GetTypeID() const = 0; // caller must copy result only

	virtual bool SelectTask(DWORD dwTaskID) = 0;
	virtual bool SelectTasks(DWORD* pdwTaskIDs, int nTaskCount) = 0;

	virtual void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, int nEditAttribute = -1) = 0;
	virtual bool WantUpdate(int nAttribute) const = 0;
	virtual bool PrepareNewTask(ITaskList* pTask) const = 0;
	
	virtual bool ProcessMessage(MSG* pMsg) = 0;
	virtual void DoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra = 0) = 0;
	virtual bool CanDoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra = 0) const = 0;

	virtual bool GetLabelEditRect(LPRECT pEdit) = 0; // screen coordinates
	virtual IUI_HITTEST HitTest(const POINT& ptScreen) const = 0;

	virtual void SetUITheme(const UITHEME* pTheme) = 0;
	virtual void SetReadOnly(bool bReadOnly) = 0;
	virtual HWND GetHwnd() const = 0;

	virtual void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const = 0;
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, BOOL bAppOnly = FALSE) = 0;

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
