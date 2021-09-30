// IContentControl.h: IContentControl interface.
//
/////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICONTENTCONTROL_H__7741547B_BA15_4851_A41B_2B4EC1DC12D5__INCLUDED_)
#define AFX_ICONTENTCONTROL_H__7741547B_BA15_4851_A41B_2B4EC1DC12D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Windows.h>

// function to be exported from dll to create instance of interface
#ifdef _EXPORTING // declare this in project settings for dll _only_
#	define DLL_DECLSPEC __declspec(dllexport)
#else
#	define DLL_DECLSPEC __declspec(dllimport)
#endif 

//////////////////////////////////////////////////////////////////////

const UINT ICONTENTCTRL_VERSION = 3;

//////////////////////////////////////////////////////////////////////

const UINT WM_ICC_WANTSPELLCHECK	= ::RegisterWindowMessageW(L"WM_ICC_WANTSPELLCHECK");
const UINT WM_ICC_CONTENTCHANGE		= ::RegisterWindowMessageW(L"WM_ICC_CONTENTCHANGE");
const UINT WM_ICC_KILLFOCUS			= ::RegisterWindowMessageW(L"WM_ICC_KILLFOCUS");
const UINT WM_ICC_DOHELP			= ::RegisterWindowMessageW(L"WM_ICC_DOHELP");			// lParam = Help key (LPCWSTR)
const UINT WM_ICC_GETCLIPBOARD		= ::RegisterWindowMessageW(L"WM_ICC_GETCLIPBOARD");		// lParam = HWND
const UINT WM_ICC_HASCLIPBOARD		= ::RegisterWindowMessageW(L"WM_ICC_HASCLIPBOARD");		// lParam = HWND
const UINT WM_ICC_TASKLINK			= ::RegisterWindowMessageW(L"WM_ICC_TASKLINK");			// lParam = URL (LPCWSTR)
const UINT WM_ICC_FAILEDLINK		= ::RegisterWindowMessageW(L"WM_ICC_FAILEDLINK");		// wParam = HWND, lParam = URL (LPCWSTR)
const UINT WM_ICC_GETLINKTOOLTIP	= ::RegisterWindowMessageW(L"WM_ICC_GETLINKTOOLTIP");	// lParam = URL (LPCWSTR)
const UINT WM_ICC_GETATTRIBUTELIST	= ::RegisterWindowMessageW(L"WM_ICC_GETATTRIBUTELIST");	// wParam = TDC_ATTRIBUTE, lParam = separator (0 -> default separator)

//////////////////////////////////////////////////////////////////////

class IContent;
class IContentControl;
class IPreferences;
class ITransText;
class ISpellCheck;

struct UITHEME;

//////////////////////////////////////////////////////////////////////

typedef IContent* (*PFNCREATECONTENT)(); // function prototype

extern "C" DLL_DECLSPEC IContent* CreateContentInterface();

typedef int (*PFNGETVERSION)(); // function prototype
extern "C" DLL_DECLSPEC int GetInterfaceVersion();

//////////////////////////////////////////////////////////////////////

#pragma warning(disable:4505)

// helper method
static IContent* CreateContentInterface(LPCWSTR szDllPath, int* pVer = 0)
{
    IContent* pInterface = NULL;
    HMODULE hDll = LoadLibraryW(szDllPath);
	
    if (hDll)
    {
        PFNCREATECONTENT pCreate = (PFNCREATECONTENT)GetProcAddress(hDll, "CreateContentInterface");
		
        if (pCreate)
		{
			try
			{
				if (ICONTENTCTRL_VERSION == 0)
				{
					pInterface = pCreate();
				}
				else
				{
					// check version
					PFNGETVERSION pVersion = (PFNGETVERSION)GetProcAddress(hDll, "GetInterfaceVersion");

					if (pVersion != NULL)
					{
						// pass version back to caller
						if (pVer)
							*pVer = pVersion();

						if (pVersion() == ICONTENTCTRL_VERSION)
							pInterface = pCreate();
					}
				}
			}
			catch (...)
			{
			}
		}

		if (!pInterface)
			FreeLibrary(hDll);
    }
	
    return pInterface;
}

static BOOL IsContentDll(LPCWSTR szDllPath)
{
    HMODULE hDll = LoadLibrary(szDllPath);
	
    if (hDll)
    {
		PFNCREATECONTENT pCreate = (PFNCREATECONTENT)GetProcAddress(hDll, "CreateContentInterface");

		// Only free the library if we're NOT just about to reload it
		if (pCreate == NULL)
			FreeLibrary(hDll);

		return (pCreate != NULL);
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////

class IContent  
{
public:
	virtual LPCWSTR GetTypeID() const = 0;
	virtual LPCWSTR GetTypeDescription() const = 0;
	virtual HICON GetTypeIcon() const = 0;

	virtual void SetLocalizer(ITransText* pTT) = 0;

	virtual IContentControl* CreateCtrl(unsigned short nCtrlID, unsigned long nStyle, 
						long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent) = 0;

	virtual void Release() = 0;
	
	// returns the length of the html or zero if not supported
	virtual int ConvertToHtml(const unsigned char* pContent, int nLength,
							  LPCWSTR szCharSet, LPWSTR& szHtml, LPCWSTR szImageDir) = 0;
	virtual void FreeHtmlBuffer(LPWSTR& szHtml) = 0;
	
	virtual void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const = 0;
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly) = 0;
};

//////////////////////////////////////////////////////////////////////

class IContentControl  
{
public:
	// custom/binary data format
	virtual int GetContent(unsigned char* pContent) const = 0;
	virtual bool SetContent(const unsigned char* pContent, int nLength, bool bResetSelection) = 0;
	virtual LPCWSTR GetTypeID() const = 0;

	// text content if supported. return false if not supported
	virtual int GetTextContent(LPWSTR szContent, int nLength = -1) const = 0;
	virtual bool SetTextContent(LPCWSTR szContent, bool bResetSelection) = 0;
	virtual bool InsertTextContent(LPCWSTR szContent, bool bAtEnd = false) = 0;

	virtual bool FindReplaceAll(LPCWSTR szFind, LPCWSTR szReplace, bool bCaseSensitive, bool bWholeWord) = 0;

	virtual void Enable(bool bEnable) = 0;
	virtual void SetReadOnly(bool bReadOnly) = 0;
	virtual HWND GetHwnd() const = 0;

	virtual void Release() = 0;

	virtual bool ProcessMessage(MSG* pMsg) = 0;
	virtual void FilterToolTipMessage(MSG* pMsg) = 0;

	virtual ISpellCheck* GetSpellCheckInterface() = 0;
	
	virtual bool Undo() = 0;
	virtual bool Redo() = 0;
	
	virtual void SetUITheme(const UITHEME* pTheme) = 0;
	virtual void SetContentFont(HFONT hFont) = 0;

	virtual void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const = 0;
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly) = 0;
};

//////////////////////////////////////////////////////////////////////

#endif // AFX_ICONTENTCONTROL_H__7741547B_BA15_4851_A41B_2B4EC1DC12D5__INCLUDED_
