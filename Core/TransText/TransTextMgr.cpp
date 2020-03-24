// TransTextMgr.cpp: implementation of the CTransTextMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TransTextMgr.h"
#include "TransTextUtils.h"
#include "TransWnd.h"

#include "..\shared\filemisc.h"
#include "..\shared\misc.h"
#include "..\shared\enmenu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////////

CTransTextMgr::CTransTextMgr() : m_hwndMenuCallback(NULL) 
{
	CTransDictionary::SetAppVersion(FileMisc::GetAppVersion());
}

CTransTextMgr::~CTransTextMgr()
{
	m_dictionary.DeleteDictionary();
}

BOOL CTransTextMgr::Initialize(LPCTSTR szDictPath, ITT_TRANSLATEOPTION nOption)
{
	return GetInstance().InitHooks(szDictPath, nOption);
}

void CTransTextMgr::Release()
{
	GetInstance().m_dictionary.SaveDictionary();
	GetInstance().ReleaseHooks();
}

BOOL CTransTextMgr::InitHooks(LPCTSTR szDictPath, ITT_TRANSLATEOPTION nOption)
{
	if (!m_dictionary.LoadDictionary(szDictPath))
	{
		// if the dictionary did not load and we're only
		// in translation mode then we fail
		if (nOption == ITTTO_TRANSLATEONLY)
			return FALSE;
	}

	DICTITEM::SetTranslationOption(nOption);
	
	return CHookWndMgr<CTransTextMgr>::InitHooks();
}

LPCTSTR CTransTextMgr::GetDictionaryFile() 
{ 
	return GetInstance().m_dictionary.GetDictionaryPath(); 
}

LPCTSTR CTransTextMgr::GetDictionaryVersion() 
{ 
	return GetInstance().m_dictionary.GetDictionaryVersion(); 
}

ITT_TRANSLATEOPTION CTransTextMgr::GetTranslationOption() 
{ 
	return DICTITEM::GetTranslationOption(); 
}

BOOL CTransTextMgr::CleanupDictionary(LPCTSTR szMasterDictPath, LPCTSTR szDictPath)
{
	CTransDictionary dtActive, dtMaster, dtRemoved;
	CString sDictPath(szDictPath);

	if (sDictPath.IsEmpty())
		sDictPath = GetDictionaryFile();
	
	if (TransText::CleanupDictionary(szMasterDictPath, sDictPath))
	{
		if (FileMisc::IsSamePath(sDictPath, GetDictionaryFile()))
		{
			// reload dictionary
			GetInstance().m_dictionary.LoadDictionary(sDictPath);
		}
		return TRUE;
	}	

	return FALSE;
}
	
void CTransTextMgr::SetTranslationOption(ITT_TRANSLATEOPTION nOption)
{
	// if adding to dictionary there must be a dictionary specified
	CTransTextMgr& ttm = GetInstance();

	if ((nOption == ITTTO_ADD2DICTIONARY) || !ttm.m_dictionary.GetDictionaryPath().IsEmpty())
	{
		DICTITEM::SetTranslationOption(nOption);
	}
}

void CTransTextMgr::ReleaseHooks()
{
	// cleanup
	m_dictionary.DeleteDictionary();

	CHookWndMgr<CTransTextMgr>::Release();
}

void CTransTextMgr::PostHookWnd(HWND hWnd)
{
	CTransWnd* pTWnd = (CTransWnd*)GetHookWnd(hWnd);
	ASSERT (pTWnd);

	if (pTWnd)
		pTWnd->PostHookWindow();
}

void CTransTextMgr::IgnoreString(const CString& sText, BOOL bPrepare)
{
	GetInstance().m_dictionary.IgnoreString(sText, bPrepare);
}

BOOL CTransTextMgr::TranslateText(CString& sText, HWND hWndRef, LPCTSTR szClassID)
{
	return GetInstance().m_dictionary.Translate(sText, hWndRef, szClassID);
}

void CTransTextMgr::SetMenuPostTranslationCallback(HWND hwndCallback)
{
	GetInstance().m_hwndMenuCallback = hwndCallback;
}

void CTransTextMgr::EnableTranslation(HWND hWnd, BOOL bEnable)
{
	if (hWnd)
	{
		CTransTextMgr& ttm = GetInstance();

		if (bEnable)
		{
			// we don't hook it ourselves
			// just let the normal mechanism do its stuff

			// remove from map
			ttm.m_mapWndIgnore.RemoveKey(hWnd);
		}
		else // unhook window if already hooked
		{
			if (ttm.WantTranslation(hWnd))
				ttm.RemoveHookWnd(hWnd);

			// add to map
			ttm.m_mapWndIgnore[hWnd] = NULL;
		}
	}
}

BOOL CTransTextMgr::WantTranslation(HWND hWnd, UINT nMsg) const
{
	void* pDummy = NULL;

	if (m_mapWndIgnore.Lookup(hWnd, pDummy))
		return FALSE;

	// weed out various window types
	switch (nMsg)
	{
	case WM_SETTEXT:
		{
			CString sClass = CWinClasses::GetClassEx(hWnd);

			if (CWinClasses::IsEditControl(sClass))
			{
				return FALSE;
			}
			else if (CWinClasses::IsCommonDialog(hWnd))
			{
				return FALSE;
			}
			// if we are a combobox and our parent is a comboboxex
			// also check that window for ignoring
			else if (CWinClasses::IsComboBox(sClass))
			{
				HWND hParent = ::GetParent(hWnd);

				if (CWinClasses::IsClass(hParent, WC_COMBOBOXEX))
				{
					if (m_mapWndIgnore.Lookup(hParent, pDummy))
						return FALSE;
				}
			}
		}
		break;
	}

	// else
	return TRUE;
}

void CTransTextMgr::EnableTranslation(HMENU hMenu, BOOL bEnable)
{
	if (hMenu)
	{
		for (int nItem = 0; nItem < ::GetMenuItemCount(hMenu); nItem++)
		{
			EnableTranslation(::GetMenuItemID(hMenu, nItem), bEnable);
		}
	}
}

void CTransTextMgr::EnableTranslation(UINT nMenuID, BOOL bEnable)
{
	const UINT POPUPMENU_ID = 0xFFFFFFFF;

	if (nMenuID && (nMenuID != POPUPMENU_ID))
	{
		CTransTextMgr& ttm = GetInstance();

		if (bEnable)
			ttm.m_mapMenuIgnore.RemoveKey(nMenuID);
		else
			ttm.m_mapMenuIgnore[nMenuID] = NULL;
	}
}

BOOL CTransTextMgr::WantTranslation(UINT nMenuID) const
{
	void* pDummy = NULL;
	
	if (nMenuID && m_mapMenuIgnore.Lookup(nMenuID, pDummy))
		return FALSE;
	
	// else
	return TRUE;
}

BOOL CTransTextMgr::RemoveHookWnd(HWND hWnd)
{
	if (hWnd)
		return UnhookWnd(hWnd);
	
	// else
	return FALSE;
}

CSubclassWnd* CTransTextMgr::NewHookWnd(HWND hWnd, const CString& sClass, DWORD dwStyle) const
{
	// pre-check
	if (!WantTranslation(hWnd))
		return NULL;

	return CTransWnd::NewTransWnd(sClass, dwStyle);
}

BOOL CTransTextMgr::HandleInitMenuPopup(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM /*lp*/)
{
	if (nMsg != WM_INITMENUPOPUP)
		return FALSE;
	
	if (!WantTranslation(hWnd, nMsg))
		return FALSE;
	
	TranslateMenu((HMENU)wp, hWnd);

	return TRUE; // handled
}

BOOL CTransTextMgr::HandleTootipNeedText(HWND hWnd, UINT nMsg, WPARAM /*wp*/, LPARAM lp)
{
	if (nMsg != WM_NOTIFY)
		return FALSE;

	NMHDR* pNMHDR = (NMHDR*)lp;

	if (pNMHDR->code != TTN_NEEDTEXTA && pNMHDR->code != TTN_NEEDTEXTW)
		return FALSE;

	if (!WantTranslation(hWnd, nMsg))
		return FALSE;

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	
	UINT_PTR nID = pNMHDR->idFrom;
	
	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
		nID = ::GetDlgCtrlID((HWND)nID);
	}
	
	if (nID == 0) // will be zero on a separator
		return FALSE;

	// see if the tooltip has already been handled
	static CString strTipText;

	if (pNMHDR->code == TTN_NEEDTEXTA)
	{
		strTipText = pTTTA->szText;

		if (strTipText.IsEmpty())
			strTipText = pTTTA->lpszText;
	}
	else // TTN_NEEDTEXTW
	{
		strTipText = pTTTW->szText;

		if (strTipText.IsEmpty())
			strTipText = pTTTW->lpszText;
	}

	if (strTipText.IsEmpty())
	{
		CString sFullText;
		
		// don't handle the message if no string resource found
		if (!sFullText.LoadString(nID))
			return FALSE;
		
		// this is the command id, not the button index
		AfxExtractSubString(strTipText, sFullText, 1, '\n');
		
		if (strTipText.IsEmpty())
			return FALSE;
	}
	
	// override class id
	TranslateText(strTipText, hWnd, _T("tooltip"));
	
	// copy back to TOOLTIPTEXT
	const int MAX_TIP_LEN = ((sizeof(pTTTW->szText) / sizeof(pTTTW->szText[0])) - 1);

#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTW)
	{
#if _MSC_VER >= 1400
		strncpy_s(pTTTA->szText, MAX_TIP_LEN, strTipText, MAX_TIP_LEN);
#else
		strncpy(pTTTA->szText, strTipText, MAX_TIP_LEN);
#endif
		pTTTA->lpszText = (LPSTR)(LPCSTR)strTipText;
	}
	else // TTN_NEEDTEXTW
	{
		Misc::EncodeAsUnicode(strTipText);

		lstrcpyn(pTTTW->szText, (LPCWSTR)(LPCSTR)strTipText, MAX_TIP_LEN);
		pTTTW->lpszText = (LPWSTR)(LPCWSTR)(LPCSTR)strTipText;
	}
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
	{
		Misc::EncodeAsMultiByte(strTipText);

#if _MSC_VER >= 1400
		strncpy_s(pTTTA->szText, MAX_TIP_LEN, (LPCSTR)(LPCWSTR)strTipText, MAX_TIP_LEN);
#else
		strncpy(pTTTA->szText, (LPCSTR)(LPCWSTR)strTipText, MAX_TIP_LEN);
#endif
		pTTTA->lpszText = (LPSTR)(LPCSTR)(LPCWSTR)strTipText;
	}
	else // TTN_NEEDTEXTW
	{
		lstrcpyn(pTTTW->szText, strTipText, MAX_TIP_LEN);
		pTTTW->lpszText = (LPWSTR)(LPCWSTR)strTipText;
	}
#endif
	
	return TRUE; // handled
}

BOOL CTransTextMgr::OnCallWndProc(const MSG& msg)
{   
#ifdef _USRDLL
	// If this is a DLL, need to set up MFC state
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
	
	BOOL bRes = FALSE;
	
	switch (msg.message)
	{
	case WM_INITMENUPOPUP:
		// We only handle this for unhooked windows
		if (!IsHooked(msg.hwnd))
			bRes = HandleInitMenuPopup(msg.hwnd, msg.message, msg.wParam, msg.lParam);
		break;

	case WM_NOTIFY:
		// We only handle this for unhooked windows
		if (!IsHooked(msg.hwnd))
			bRes = HandleTootipNeedText(msg.hwnd, msg.message, msg.wParam, msg.lParam);
		break;

	default:
		bRes = CHookWndMgr<CTransTextMgr>::OnCallWndProc(msg);
		break;
	}
	
	return bRes;
}


BOOL CTransTextMgr::WantHookWnd(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp) const
{
	// filter out marked windows
	if (!WantTranslation(hWnd))
	{
		return FALSE;
	}
	else if (CHookWndMgr<CTransTextMgr>::WantHookWnd(hWnd, nMsg, wp, lp))
	{
		// we hook dialog/popup only when then are about to be shown.
		// as a byproduct their children will be hooked also at that point
		if (nMsg == WM_PARENTNOTIFY)
		{
			// We're interested the sender's parent
			if (GetHookWnd(::GetParent(hWnd)))
				return TRUE;
		}
		else
		{
			if (CWinClasses::IsDialog(hWnd))
			{
				// Don't translate common dialogs for now
				return !CWinClasses::IsCommonDialog(hWnd);
			}
			else if (TransText::IsPopup(hWnd))
			{
				return TRUE; 
			}
		}

		// else 
		return FALSE;
	}

	return FALSE;
}

BOOL CTransTextMgr::TranslateMenu(HMENU hMenu, HWND hWndRef, BOOL bRecursive)
{
	if (!hMenu || !::IsMenu(hMenu))
		return FALSE;

	CTransTextMgr& ttm = CTransTextMgr::GetInstance();
	int nCount = (int)::GetMenuItemCount(hMenu);

	for (int nPos = 0; nPos < nCount; nPos++)
	{
		int nCmdID = (int)::GetMenuItemID(hMenu, nPos);

		// we don't do separators or ownerdraw or menus tagged as not-translatable
		if (!nCmdID || TransText::IsOwnerDraw(nCmdID, hMenu) || !ttm.WantTranslation(nCmdID))
			continue;

		CString sItem = CEnMenu::GetMenuString(hMenu, nPos, MF_BYPOSITION);

		if (!sItem.IsEmpty())
		{
			if (ttm.m_dictionary.Translate(sItem, hMenu, FALSE))
				VERIFY(CEnMenu::SetMenuString(hMenu, nPos, sItem, MF_BYPOSITION));
		
			// submenus?
			if (bRecursive && (nCmdID == -1))
			{
				HMENU hSubMenu = ::GetSubMenu(hMenu, nPos);

				if (hSubMenu)
					TranslateMenu(hSubMenu, hWndRef, bRecursive);
			}
		}
	}

	VERIFY(CEnMenu::EnsureUniqueAccelerators(hMenu));

	if (ttm.m_hwndMenuCallback)
	{
		ASSERT(::IsWindow(ttm.m_hwndMenuCallback));
		::SendMessage(ttm.m_hwndMenuCallback, WM_ITT_POSTTRANSLATEMENU, 0, (LPARAM)hMenu);
	}

	return TRUE;
}
