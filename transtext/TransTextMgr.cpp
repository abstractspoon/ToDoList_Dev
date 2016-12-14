// TransTextMgr.cpp: implementation of the CTransTextMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TransTextMgr.h"
#include "TransTextUtils.h"
#include "TransWnd.h"

#include "..\shared\filemisc.h"
#include "..\shared\misc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const UINT POPUPMENU_ID = 0xFFFFFFFF;

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

///////////////////////////////////////////////////////////////////////////////////

CTransTextMgr::CTransTextMgr()
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
	
BOOL CTransTextMgr::ReportPossibleDictionaryDuplicates(LPCTSTR szDictPath)
{
	CTransDictionary dtActive, dtDuplicates;

	if (!dtActive.LoadDictionary(szDictPath))
		return FALSE;

	if (dtActive.GetPossibleDuplicates(dtDuplicates))
	{
		CString sDuplicatesPath = CFileBackup::BuildBackupPath(szDictPath, _T("duplicates"), 0, _T(".dup"));

		VERIFY(FileMisc::CreateFolderFromFilePath(sDuplicatesPath));
		VERIFY(dtDuplicates.SaveDictionary(sDuplicatesPath, TRUE));

		return TRUE;
	}

	// else
	return FALSE; // no duplicates
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
			CString sClass = CWinClasses::GetClass(hWnd);

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

void CTransTextMgr::UpdateMenu(HWND hWnd)
{
	CTransTextMgr& ttm = GetInstance();
	CTransWnd* pTWnd = (CTransWnd*)ttm.GetHookWnd(hWnd);

	if (pTWnd)
		pTWnd->UpdateMenu();
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
#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTW)
	{
		pTTTA->lpszText = (LPTSTR)(LPCTSTR)strTipText;
	}
	else // TTN_NEEDTEXTW
	{
		Misc::EncodeAsUnicode(strTipText);
		pTTTW->lpszText = (LPWSTR)(LPCWSTR)(LPCSTR)strTipText;
	}
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
	{
		Misc::EncodeAsMultiByte(strTipText);
		pTTTA->lpszText = (LPSTR)(LPCSTR)(LPCWSTR)strTipText;
	}
	else // TTN_NEEDTEXTW
	{
		pTTTW->lpszText = (LPTSTR)(LPCTSTR)strTipText;
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
		// let hook wnd override
		if (!IsHooked(msg.hwnd))
			bRes = HandleInitMenuPopup(msg.hwnd, msg.message, msg.wParam, msg.lParam);
		break;

	case WM_NOTIFY:
		// let hook wnd override
		if (!IsHooked(msg.hwnd))
			bRes = HandleTootipNeedText(msg.hwnd, msg.message, msg.wParam, msg.lParam);
		break;

	case WM_CONTEXTMENU:
		// we handle this directly
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
		if (nMsg != WM_PARENTNOTIFY)
		{
			if (CWinClasses::IsDialog(hWnd))
			{
				// only translate common dialogs if our dictionary
				// differs from the users GUI language
				if (CWinClasses::IsCommonDialog(hWnd))
				{
					//static LANGID nUILangID = Misc::GetUserDefaultUILanguage();
					
					//if (PRIMARYLANGID(nUILangID) == m_wDictLanguageID)
						return FALSE;
				}
#ifdef _DEBUG	// don't translate debug windows
				else
				{
					CString sDlgTitle;
					CWnd::FromHandle(hWnd)->GetWindowText(sDlgTitle);
					
					if (sDlgTitle.Find(_T("Microsoft Visual C++ Debug Library")) == 0)
						return FALSE;
				}
#endif
				return TRUE; 
			}
			else if (TransText::IsPopup(hWnd))
			{
				return TRUE; 
			}
		}
		else if (GetHookWnd(::GetParent(hWnd)))
			return TRUE;

		// else 
		return FALSE;
	}

	return FALSE;
}

BOOL CTransTextMgr::TranslateMenu(HMENU hMenu, HWND hWndRef, BOOL bRecursive)
{
	if (!hMenu || !::IsMenu(hMenu))
		return FALSE;

	int nCount = (int)::GetMenuItemCount(hMenu);
	CTransTextMgr& ttm = CTransTextMgr::GetInstance();

	for (int nPos = 0; nPos < nCount; nPos++)
	{
		int nCmdID = (int)::GetMenuItemID(hMenu, nPos);

		// we don't do separators or ownerdraw or menus tagged as not-translatable
		if (!nCmdID || TransText::IsOwnerDraw(nCmdID, hMenu) || !ttm.WantTranslation(nCmdID))
			continue;

		CString sItem;
		int nLen = ::GetMenuString(hMenu, nPos, NULL, 0, MF_BYPOSITION);

		::GetMenuString(hMenu, nPos, sItem.GetBuffer(nLen + 1), nLen + 1, MF_BYPOSITION);
		sItem.ReleaseBuffer();

		if (!sItem.IsEmpty())
		{
			if (ttm.m_dictionary.Translate(sItem, hMenu, nCmdID))
			{
				ASSERT (!sItem.IsEmpty());
			
				MENUITEMINFO minfo;
				minfo.cbSize = sizeof(minfo);
				minfo.fMask = MIIM_STRING;
				minfo.dwTypeData = (LPTSTR)(LPCTSTR)sItem;
				
				SetMenuItemInfo(hMenu, nPos, TRUE, &minfo);
			}
		
			// submenus?
			if (bRecursive && nCmdID == -1)
			{
				HMENU hSubMenu = ::GetSubMenu(hMenu, nPos);

				if (hSubMenu)
					TranslateMenu(hSubMenu, hWndRef, bRecursive);
			}
		}
	}

	return TRUE;
}



