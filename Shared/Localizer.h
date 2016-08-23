// Localizer.h: interface for the CLocalizer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOCALIZER_H__6AE74534_3914_4C84_97D1_25C6FF75B9FE__INCLUDED_)
#define AFX_LOCALIZER_H__6AE74534_3914_4C84_97D1_25C6FF75B9FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Interfaces\ITransText.h"

class CLocalizer  
{
public:
	CLocalizer() {}

	static BOOL Initialize(LPCTSTR szTransFile = _T(""), ITT_TRANSLATEOPTION nOption = ITTTO_TRANSLATEONLY);
	static BOOL Initialize(ITransText* pTT);
	static BOOL IsInitialized();
	static void Release();

	static void SetTranslationOption(ITT_TRANSLATEOPTION nOption);
	static ITT_TRANSLATEOPTION GetTranslationOption();
	static CString GetDictionaryPath();
	static CString GetDictionaryVersion();
	static BOOL GetDictionaryVersion(CDWordArray& aVersionParts);

	static void ForceTranslateAllUIElements(UINT nIDFirstStr = 1, UINT nIDLastStr = AFX_IDS_SCFIRST - 1);

	static CString TranslateText(LPCTSTR szText);
	static BOOL TranslateText(CString& sText, HWND hWndRef = NULL);
	static BOOL TranslateMenu(HMENU hMenu, HWND hWndRef = NULL, BOOL bRecursive = TRUE);

	static void UpdateMenu(HWND hWnd);
	static void UpdateMenu(HMENU hMenu);

	static void EnableTranslation(const CWnd& hWnd, BOOL bEnable = TRUE);
	static void EnableTranslation(HWND hWnd, BOOL bEnable = TRUE);
	static void EnableTranslation(HMENU hMenu, BOOL bEnable = TRUE);
	static void EnableTranslation(UINT nMenuID, BOOL bEnable = TRUE);
	static void EnableTranslation(UINT nMenuIDFirst, UINT nMenuIDLast, BOOL bEnable = TRUE);

	static void IgnoreString(const CString& sText);
	static void IgnoreString(UINT nIDText);

	static ITransText* GetLocalizer() { return s_pTransText; }
	static BOOL CleanupDictionary(LPCTSTR szMasterDictPath, LPCWSTR szDictPath = NULL);
	static BOOL ReportPossibleDictionaryDuplicates(LPCTSTR szDictPath = NULL);

protected:
	static BOOL ValidLocalizer();
	static BOOL ValidLocalizer(ITransText* pTT);

protected:
	static ITransText* s_pTransText;
	static BOOL s_bOwner;

};

#endif // !defined(AFX_LOCALIZER_H__6AE74534_3914_4C84_97D1_25C6FF75B9FE__INCLUDED_)
