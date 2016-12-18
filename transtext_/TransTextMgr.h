// TransTextMgr.h: interface for the CTransTextMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSTEXTMGR_H__1F1C6D57_D2C6_4C06_9E7C_1E0C54B65D30__INCLUDED_)
#define AFX_TRANSTEXTMGR_H__1F1C6D57_D2C6_4C06_9E7C_1E0C54B65D30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TransDictionary.h"

#include "..\shared\hookwndmgr.h"

#include "..\Interfaces\itranstext.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

class CTransTextMgr : protected CHookWndMgr<CTransTextMgr>
{
	friend class CHookMgr<CTransTextMgr>; // so CHookMgr can access protected constructor
	friend class CHookWndMgr<CTransTextMgr>; // so CHookMgr can access protected constructor
	friend class CTransWnd;

public:
	virtual ~CTransTextMgr();

	static BOOL Initialize(LPCTSTR szDictPath = _T(""), ITT_TRANSLATEOPTION nOption = ITTTO_TRANSLATEONLY);
	static void Release();

	static LPCTSTR GetDictionaryFile();
	static LPCTSTR GetDictionaryVersion();
	static void SetTranslationOption(ITT_TRANSLATEOPTION nOption);
	static ITT_TRANSLATEOPTION GetTranslationOption();
	
	static BOOL TranslateText(CString& sText, HWND hWndRef, LPCTSTR szClassID = NULL);
	static BOOL TranslateMenu(HMENU hMenu, HWND hWndRef = NULL, BOOL bRecursive = FALSE);

	static void UpdateMenu(HWND hWnd);
	static void UpdateMenu(HMENU hMenu);
	
	static void EnableTranslation(HWND hWnd, BOOL bEnable);
	static void EnableTranslation(HMENU hMenu, BOOL bEnable);
	static void EnableTranslation(UINT nMenuID, BOOL bEnable);

	static void IgnoreString(const CString& sText, BOOL bPrepare);

	static BOOL CleanupDictionary(LPCTSTR szMasterDictPath, LPCTSTR szDictPath = NULL);
	static BOOL ReportPossibleDictionaryDuplicates(LPCTSTR szDictPath = NULL);

protected:
	CTransDictionary m_dictionary;
	CMap<HWND, HWND&, void*, void*&> m_mapWndIgnore;
	CMap<UINT, UINT&, void*, void*&> m_mapMenuIgnore;

protected:
	CTransTextMgr();

	BOOL InitHooks(LPCTSTR szDictPath, ITT_TRANSLATEOPTION nOption);
	void ReleaseHooks();

	virtual CSubclassWnd* NewHookWnd(HWND hWnd, const CString& sClass, DWORD dwStyle) const;
	virtual BOOL WantHookWnd(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp) const;
	virtual void PostHookWnd(HWND hWnd);
	virtual BOOL RemoveHookWnd(HWND hWnd);
	virtual BOOL OnCallWndProc(const MSG& msg);

	BOOL HandleInitMenuPopup(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp);
	BOOL HandleTootipNeedText(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp);
	BOOL HandleSetText(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp);

	BOOL WantTranslation(HWND hWnd, UINT nMsg = 0) const;
	BOOL WantTranslation(UINT nMenuID) const;
};

#endif // !defined(AFX_TRANSTEXTMGR_H__1F1C6D57_D2C6_4C06_9E7C_1E0C54B65D30__INCLUDED_)
