// ITransText.h: interface for the ITransText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITRANSTEXT_H__B685332D_C395_46B1_8DDB_ED8196B3A63F__INCLUDED_)
#define AFX_ITRANSTEXT_H__B685332D_C395_46B1_8DDB_ED8196B3A63F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// function to be exported from dll to create instance of interface
#ifdef _EXPORTING // declare this in project settings for dll _only_
#	define DLL_DECLSPEC __declspec(dllexport)
#else
#	define DLL_DECLSPEC __declspec(dllimport)
#endif 

const UINT ITRANSTEXT_VERSION = 0x0000;

enum ITT_TRANSLATEOPTION
{
	ITTTO_TRANSLATEONLY,
	ITTTO_ADD2DICTIONARY,
	ITTTO_UPPERCASE
};

class ITransText;

typedef ITransText* (*PFNCREATETTI)(LPCTSTR, int); // function prototype
extern "C" DLL_DECLSPEC ITransText* CreateTransTextInterface(LPCTSTR szDictPath, ITT_TRANSLATEOPTION nOption); // single exported function

typedef int (*PFNGETVERSION)(); // function prototype
extern "C" DLL_DECLSPEC int GetInterfaceVersion();

#pragma warning( disable:4505)

static ITransText* CreateTransTextInterface(LPCTSTR szDllPath, LPCTSTR szTransFile, ITT_TRANSLATEOPTION nOption)
{
    ITransText* pInterface = NULL;
    HMODULE hDll = LoadLibrary(szDllPath);
	
    if (hDll)
    {
        PFNCREATETTI pCreate = (PFNCREATETTI)GetProcAddress(hDll, "CreateTransTextInterface");
		
        if (pCreate)
		{
			if (ITRANSTEXT_VERSION == 0)
				pInterface = pCreate(szTransFile, nOption);
			else
			{
				// check version
				PFNGETVERSION pVersion = (PFNGETVERSION)GetProcAddress(hDll, "GetInterfaceVersion");

				if ((pVersion && pVersion() >= ITRANSTEXT_VERSION))
					pInterface = pCreate(szTransFile, nOption);
			}
		}
    }
	
    return pInterface;
}

static BOOL IsTransTextDll(LPCTSTR szDllPath)
{
    HMODULE hDll = LoadLibrary(szDllPath);
	
    if (hDll)
    {
        PFNCREATETTI pCreate = (PFNCREATETTI)GetProcAddress(hDll, "CreateTransTextInterface");
		FreeLibrary(hDll);

		return (NULL != pCreate);
	}

	return FALSE;
}

class ITransText
{
public:
	virtual BOOL Initialize(LPCTSTR szDictPath, ITT_TRANSLATEOPTION nOption = ITTTO_TRANSLATEONLY) = 0;
    virtual void Release() = 0; // releases the interface
	
	virtual LPCTSTR GetDictionaryFile() const = 0;
	virtual LPCTSTR GetDictionaryVersion() const = 0;
	virtual void SetTranslationOption(ITT_TRANSLATEOPTION nOption) = 0;
	virtual ITT_TRANSLATEOPTION GetTranslationOption() const = 0;

	virtual BOOL CleanupDictionary(LPCTSTR szMasterDictPath) = 0;

	virtual BOOL TranslateText(LPCTSTR szText, LPTSTR& szTranslated) = 0;
	virtual BOOL TranslateText(LPCTSTR szText, HWND hWndRef, LPTSTR& szTranslated) = 0;
	virtual BOOL TranslateMenu(HMENU hMenu, HWND hWndRef, BOOL bRecursive = TRUE) = 0;

	virtual void UpdateMenu(HWND hWnd) = 0;

	virtual void EnableTranslation(HWND hWnd, BOOL bEnable = TRUE) = 0;
	virtual void EnableTranslation(HMENU hMenu, BOOL bEnable = TRUE) = 0;
	virtual void EnableTranslation(UINT nMenuID, BOOL bEnable = TRUE) = 0;

	virtual void IgnoreString(LPCTSTR szText) = 0;
	virtual void FreeTextBuffer(LPTSTR& szTranslated) = 0;
};


#endif // !defined(AFX_ITRANSTEXT_H__B685332D_C395_46B1_8DDB_ED8196B3A63F__INCLUDED_)
