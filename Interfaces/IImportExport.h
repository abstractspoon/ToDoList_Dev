// IImportExport.h: interface and implementation of the IImportExport class.
//
/////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IIMPORTEXPORT_H__7741547B_BA15_4851_A41B_2B4EC1DC12D5__INCLUDED_)
#define AFX_IIMPORTEXPORT_H__7741547B_BA15_4851_A41B_2B4EC1DC12D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Windows.h>

/////////////////////////////////////////////////////////////////////////////////

// function to be exported from dll to create instance of interface
#ifdef _EXPORTING // declare this in project settings for dll _only_
#	define DLL_DECLSPEC __declspec(dllexport)
#else
#	define DLL_DECLSPEC __declspec(dllimport)
#endif 

const UINT IIMPORTEXPORT_VERSION = 3;

//////////////////////////////////////////////////////////////////////

class IImportTasklist;
class IExportTasklist;
class ITaskList;
class IMultiTaskList;
class IPreferences;
class ITransText;

//////////////////////////////////////////////////////////////////////

typedef IImportTasklist* (*PFNCREATEIMPORT)(); // function prototype
typedef IExportTasklist* (*PFNCREATEEXPORT)(); // function prototype

extern "C" DLL_DECLSPEC IImportTasklist* CreateImportInterface();
extern "C" DLL_DECLSPEC IExportTasklist* CreateExportInterface();

typedef int (*PFNGETVERSION)(); // function prototype
extern "C" DLL_DECLSPEC int GetInterfaceVersion();

//////////////////////////////////////////////////////////////////////

#pragma warning(disable:4505)

// helper methods
static IImportTasklist* CreateImportInterface(LPCWSTR szDllPath)
{
    IImportTasklist* pInterface = NULL;
    HMODULE hDll = LoadLibraryW(szDllPath);
	
    if (hDll)
    {
        PFNCREATEIMPORT pCreate = (PFNCREATEIMPORT)GetProcAddress(hDll, "CreateImportInterface");
		
        if (pCreate)
		{
			try
			{
				if (!IIMPORTEXPORT_VERSION)
				{
					pInterface = pCreate();
				}
				else
				{
					// check version
					PFNGETVERSION pVersion = (PFNGETVERSION)GetProcAddress(hDll, "GetInterfaceVersion");

					if (pVersion && pVersion() >= IIMPORTEXPORT_VERSION)
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

static IExportTasklist* CreateExportInterface(LPCWSTR szDllPath)
{
    IExportTasklist* pInterface = NULL;
    HMODULE hDll = LoadLibraryW(szDllPath);
	
    if (hDll)
    {
        PFNCREATEEXPORT pCreate = (PFNCREATEEXPORT)GetProcAddress(hDll, "CreateExportInterface");
		
        if (pCreate)
		{
			try
			{
				if (!IIMPORTEXPORT_VERSION)
				{
					pInterface = pCreate();
				}
				else
				{
					// check version
					PFNGETVERSION pVersion = (PFNGETVERSION)GetProcAddress(hDll, "GetInterfaceVersion");

					if (pVersion && pVersion() >= IIMPORTEXPORT_VERSION)
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

static BOOL IsImportExportDll(LPCWSTR szDllPath)
{
    HMODULE hDll = LoadLibraryW(szDllPath);
	
    if (hDll)
    {
        PFNCREATEEXPORT pCreateExp = NULL;
		PFNCREATEIMPORT pCreateImp = (PFNCREATEIMPORT)GetProcAddress(hDll, "CreateImportInterface");

		if (!pCreateImp)
			pCreateExp = (PFNCREATEEXPORT)GetProcAddress(hDll, "CreateExportInterface");

		FreeLibrary(hDll);

		return (pCreateImp || pCreateExp);
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////

enum IIMPORT_RESULT 
{
	IIR_CANCELLED	= -1,
	IIR_SUCCESS		= 0,
	IIR_BADFILE,
	IIR_BADFORMAT,
	IIR_BADINTERFACE,
	IIR_SOMEFAILED,
	IIR_OTHER,
};

class IImportTasklist
{
public:
    virtual void Release() = 0; // releases the interface
	virtual void SetLocalizer(ITransText* pTT) = 0;

	// caller must copy result only
	virtual LPCWSTR GetMenuText() const = 0;
	virtual LPCWSTR GetFileFilter() const = 0;
	virtual LPCWSTR GetFileExtension() const = 0;
	virtual HICON GetIcon() const = 0;

	virtual IIMPORT_RESULT Import(LPCWSTR szSrcFilePath, ITaskList* pDestTaskFile, BOOL bSilent, IPreferences* pPrefs, LPCWSTR szKey) = 0;
};

//////////////////////////////////////////////////////////////////////

class IExportTasklist
{
public:
    virtual void Release() = 0; // releases the interface
	virtual void SetLocalizer(ITransText* pTT) = 0;

	// caller must copy result only
	virtual LPCWSTR GetMenuText() const = 0;
	virtual LPCWSTR GetFileFilter() const = 0;
	virtual LPCWSTR GetFileExtension() const = 0;
	virtual HICON GetIcon() const = 0;

	virtual bool Export(const ITaskList* pSrcTaskFile, LPCWSTR szDestFilePath, BOOL bSilent, IPreferences* pPrefs, LPCWSTR szKey) = 0;
	virtual bool Export(const IMultiTaskList* pSrcTaskFile, LPCWSTR szDestFilePath, BOOL bSilent, IPreferences* pPrefs, LPCWSTR szKey) = 0;
};

//////////////////////////////////////////////////////////////////////

static void ReleaseImportInterface(IImportTasklist*& pInterface)
{
    if (pInterface)
    {
        pInterface->Release();
        pInterface = NULL;
    }
}

static void ReleaseExportInterface(IExportTasklist*& pInterface)
{
    if (pInterface)
    {
        pInterface->Release();
        pInterface = NULL;
    }
}

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_IImportExport_H__7741547B_BA15_4851_A41B_2B4EC1DC12D5__INCLUDED_)
