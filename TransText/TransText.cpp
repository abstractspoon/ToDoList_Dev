// TransText.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TransTextMgr.h"

#include <afxdllx.h>

#include "..\shared\enstring.h"
#include "..\shared\enmenu.h"
#include "..\shared\misc.h"

#include "..\Interfaces\ITransText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE TransTextDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("TRANSTEXT.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(TransTextDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(TransTextDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("TRANSTEXT.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(TransTextDLL);
	}
	return 1;   // ok
}

class CTransText : public ITransText
{
public:
	CTransText(LPCTSTR szDictPath = NULL, ITT_TRANSLATEOPTION nOption = ITTTO_TRANSLATEONLY)
	{
		Initialize(szDictPath, nOption);
	}

	virtual ~CTransText()
	{
	}

	// interface implementation
	BOOL Initialize(LPCTSTR szDictPath, ITT_TRANSLATEOPTION nOption)
	{
		Release();

		BOOL bRes = CTransTextMgr::Initialize(szDictPath, nOption);

		if (bRes)
		{
			CEnString::SetLocalizer(this);
			CEnMenu::SetLocalizer(this);
		}

		return bRes;
	}

	void Release()
	{ 
		CEnString::SetLocalizer(NULL);
		CEnMenu::SetLocalizer(NULL);
		CTransTextMgr::Release();
	}

	LPCTSTR GetDictionaryFile() const
	{
		return CTransTextMgr::GetDictionaryFile();
	}

	LPCTSTR GetDictionaryVersion() const
	{
		return CTransTextMgr::GetDictionaryVersion();
	}

	void SetTranslationOption(ITT_TRANSLATEOPTION nOption)
	{
		CTransTextMgr::SetTranslationOption(nOption);
	}

	ITT_TRANSLATEOPTION GetTranslationOption() const
	{
		return CTransTextMgr::GetTranslationOption();
	}
	
	BOOL TranslateText(LPCTSTR szText, LPTSTR& szTranslated)
	{
		return TranslateText(szText, NULL, szTranslated);
	}

	BOOL TranslateText(LPCTSTR szText, HWND hWndRef, LPTSTR& szTranslated)
	{
		CString sText(szText);

		if (CTransTextMgr::TranslateText(sText, hWndRef))
		{
			szTranslated = new TCHAR[sText.GetLength() + 1];
			lstrcpy(szTranslated, sText);

			return TRUE;
		}

		// else
		return FALSE;
	}

	void CTransText::FreeTextBuffer(LPTSTR& szTranslated)
	{
		delete [] szTranslated; 
		szTranslated = NULL;
	}

	BOOL TranslateMenu(HMENU hMenu, HWND hWndRef, BOOL bRecursive)
	{
		return CTransTextMgr::TranslateMenu(hMenu, hWndRef, bRecursive);
	}
	
	void UpdateMenu(HWND hWnd)
	{
		CTransTextMgr::UpdateMenu(hWnd);
	}

	void EnableTranslation(HWND hWnd, BOOL bEnable)
	{
		CTransTextMgr::EnableTranslation(hWnd, bEnable);
	}

	void EnableTranslation(HMENU hMenu, BOOL bEnable)
	{
		CTransTextMgr::EnableTranslation(hMenu, bEnable);
	}

	void EnableTranslation(UINT nMenuID, BOOL bEnable)
	{
		CTransTextMgr::EnableTranslation(nMenuID, bEnable);
	}

	void IgnoreString(LPCTSTR szText)
	{
		CTransTextMgr::IgnoreString(szText, TRUE);
	}

	BOOL CleanupDictionary(LPCTSTR szMasterDictPath, LPCTSTR szDictPath)
	{
		return CTransTextMgr::CleanupDictionary(szMasterDictPath, szDictPath);
	}

	BOOL ReportPossibleDictionaryDuplicates(LPCTSTR szDictPath)
	{
		return CTransTextMgr::ReportPossibleDictionaryDuplicates(szDictPath);
	}

};

DLL_DECLSPEC ITransText* CreateTransTextInterface(LPCTSTR szDictPath, ITT_TRANSLATEOPTION nOption)
{
	if ((nOption == ITTTO_TRANSLATEONLY) && Misc::IsEmpty(szDictPath))
		return NULL;

	static CTransText tt;
	
	tt.Initialize(szDictPath, nOption);

	return &tt;
}

DLL_DECLSPEC int GetInterfaceVersion()
{
	return ITRANSTEXT_VERSION;
}
