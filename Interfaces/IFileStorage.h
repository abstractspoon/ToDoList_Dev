// IImportExport.h: interface and implementation of the IImportExport class.
//
/////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IFILEHANDLER_H__12345678_BA15_4851_A41B_2B4EC1DC12D5__INCLUDED_)
#define AFX_IFILEHANDLER_H__12345678_BA15_4851_A41B_2B4EC1DC12D5__INCLUDED_

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

/////////////////////////////////////////////////////////////////////////////////

#define IFILEHANDLER_VERSION 0x0000

/////////////////////////////////////////////////////////////////////////////////

class IFileHandler;
class ITaskList;

/////////////////////////////////////////////////////////////////////////////////

typedef IFileHandler* (*PFNCREATEHANDLER)(); // function prototype
extern "C" DLL_DECLSPEC IFileHandler* CreateFileHandlerInterface();

typedef int (*PFNGETVERSION)(); // function prototype
extern "C" DLL_DECLSPEC int GetInterfaceVersion();

/////////////////////////////////////////////////////////////////////////////////

#pragma warning(disable:4505)

/////////////////////////////////////////////////////////////////////////////////

// helper methods
static IFileHandler* CreateFileHandlerInterface(LPCWSTR szDllPath)
{
    IFileHandler* pInterface = NULL;
    HMODULE hDll = LoadLibraryW(szDllPath);
	
    if (hDll)
    {
        PFNCREATEHANDLER pCreate = (PFNCREATEHANDLER)GetProcAddress(hDll, "CreateFileHandlerInterface");
		
        if (pCreate)
		{
			// check version
			PFNGETVERSION pVersion = (PFNGETVERSION)GetProcAddress(hDll, "GetInterfaceVersion");

			if (!IIMPORTEXPORT_VERSION || (pVersion && pVersion() >= IFILEHANDLER_VERSION))
			{
				try
				{
					pInterface = pCreate();
				}
				catch (...)
				{
				}
			}
		}

		if (hDll && !pInterface)
			FreeLibrary(hDll);
    }
	
    return pInterface;
}

static BOOL IsFileHandlerDll(LPCWSTR szDllPath)
{
    HMODULE hDll = LoadLibraryW(szDllPath);
	
    if (hDll)
    {
		PFNCREATEHANDLER pCreate = (PFNCREATEHANDLER)GetProcAddress(hDll, "CreateFileHandlerInterface");
		FreeLibrary(hDll);

		return (pCreate != NULL);
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////

struct IFH_FILEINFO
{
	TCHAR szFileID[512];
	TCHAR szDisplayName[512];
};

/////////////////////////////////////////////////////////////////////////////////

enum IFH_TYPE
{
	IFHT_
};

/////////////////////////////////////////////////////////////////////////////////

class IFileHandler
{
public:
    virtual void Release() = 0; // releases the interface

	virtual bool LoadTasklist(IFH_FILEINFO* pFInfo, ITaskList* pDestTaskFile) = 0;
	virtual bool SaveTasklist(IFH_FILEINFO* pFInfo, ITaskList* pSrcTaskFile) = 0;

};

/////////////////////////////////////////////////////////////////////////////////

static void ReleaseFileHandlerInterface(IFileHandler*& pInterface)
{
    if (pInterface)
    {
        pInterface->Release();
        pInterface = NULL;
    }
}

/////////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_IFILEHANDLER_H__12345678_BA15_4851_A41B_2B4EC1DC12D5__INCLUDED_)
