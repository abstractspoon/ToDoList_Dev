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

#define ICONTENTCTRL_VERSION 0x0002

//////////////////////////////////////////////////////////////////////

const UINT WM_ICC_WANTSPELLCHECK 	= ::RegisterWindowMessageW(L"WM_ICC_WANTSPELLCHECK");
const UINT WM_ICC_COMMENTSCHANGE	= ::RegisterWindowMessageW(L"WM_ICC_COMMENTSCHANGE");
const UINT WM_ICC_COMMENTSKILLFOCUS	= ::RegisterWindowMessageW(L"WM_ICC_COMMENTSKILLFOCUS");
const UINT WM_ICC_DOHELP			= ::RegisterWindowMessageW(L"WM_ICC_DOHELP"); // lParam is literal string


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
				// check version
				PFNGETVERSION pVersion = (PFNGETVERSION)GetProcAddress(hDll, "GetInterfaceVersion");
				int nInterfaceVer = ICONTENTCTRL_VERSION;

				// pass version back to caller
				if (pVer)
					*pVer = pVersion ? pVersion() : 0;

				if (!nInterfaceVer || (pVersion && pVersion() >= nInterfaceVer))
					pInterface = pCreate();
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

static BOOL IsContentDll(LPCWSTR szDllPath)
{
    HMODULE hDll = LoadLibrary(szDllPath);
	
    if (hDll)
    {
		PFNCREATECONTENT pCreate = (PFNCREATECONTENT)GetProcAddress(hDll, "CreateContentInterface");
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

	virtual void SetReadOnly(bool bReadOnly) = 0;
	virtual HWND GetHwnd() const = 0;

	virtual void Release() = 0;
	virtual bool ProcessMessage(MSG* pMsg) = 0;

	virtual ISpellCheck* GetSpellCheckInterface() = 0;
	
	virtual bool Undo() = 0;
	virtual bool Redo() = 0;
	
	virtual void SetUITheme(const UITHEME* pTheme) = 0;

	virtual void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const = 0;
	virtual void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey) = 0;
};

//////////////////////////////////////////////////////////////////////

#endif // AFX_ICONTENTCONTROL_H__7741547B_BA15_4851_A41B_2B4EC1DC12D5__INCLUDED_
