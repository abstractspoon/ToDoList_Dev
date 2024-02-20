// ShortcutManager.cpp: implementation of the CShortcutManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShortcutManager.h"
#include "wclassdefines.h"
#include "runtimedlg.h"
#include "winclasses.h"
#include "Misc.h"
#include "enmenu.h"
#include "AcceleratorString.h"

#include "..\Interfaces\ipreferences.h"

#include <afxtempl.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

enum
{
	VK_0 = 0x30,
	VK_9 = 0x39,
	VK_A = 0x41,
	VK_Z = 0x5A,
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShortcutManager::CShortcutManager(BOOL bAutoSendCmds)
	: m_bAutoSendCmds(bAutoSendCmds), m_wInvalidComb(0), m_wFallbackModifiers(0)
{

}

CShortcutManager::~CShortcutManager()
{

}

BOOL CShortcutManager::Initialize(CWnd* pOwner, const IPreferences* pPrefs, LPCTSTR szKey, WORD wInvalidComb, WORD wFallbackModifiers)
{
	ASSERT((pPrefs && szKey) || !(pPrefs || szKey));

	if (!IsHooked() && pOwner && HookWindow(*pOwner))
	{
		m_wInvalidComb = wInvalidComb;
		m_wFallbackModifiers = wFallbackModifiers;
		
		if (pPrefs)
			LoadSettings(pPrefs, szKey);
		
		return TRUE;
	}

	return FALSE;
}

BOOL CShortcutManager::Release(IPreferences* pPrefs, LPCTSTR szKey)
{
	if (!IsHooked())
		return TRUE;

	if (pPrefs)
		SaveSettings(pPrefs, szKey);

	return HookWindow(NULL);
}

void CShortcutManager::SetShortcut(UINT nCmdID, WORD wVirtKeyCode, WORD wModifiers)
{
	UINT nOtherCmdID = 0;
	DWORD dwShortcut = MAKELONG(wVirtKeyCode, wModifiers);

	// if the shortcut == 0 then remove the existing shortcut associated with nCmdID
	if (!dwShortcut)
	{
		m_mapID2Shortcut.Lookup(nCmdID, dwShortcut);

		m_mapShortcut2ID.RemoveKey(dwShortcut);

		// mark these commands explicitly as having no shortcut so that
		// the user's intent is clear. 
		m_mapID2Shortcut[nCmdID] = NO_SHORTCUT;

		return;
	}
	// check for existing cmds using this shortcut to remove
	else if (m_mapShortcut2ID.Lookup(dwShortcut, nOtherCmdID))
	{
		m_mapShortcut2ID.RemoveKey(dwShortcut);

		// mark these commands explicitly as having no shortcut so that
		// they subsequently will not be overwritten 
		m_mapID2Shortcut[nOtherCmdID] = NO_SHORTCUT;
	}

	// then simple add
	AddShortcut(nCmdID, wVirtKeyCode, wModifiers);
}

void CShortcutManager::SetShortcut(UINT nCmdID, DWORD dwShortcut)
{
	SetShortcut(nCmdID, LOWORD(dwShortcut), HIWORD(dwShortcut));
}

BOOL CShortcutManager::AddShortcut(UINT nCmdID, DWORD dwShortcut)
{
	return AddShortcut(nCmdID, LOWORD(dwShortcut), HIWORD(dwShortcut));
}

BOOL CShortcutManager::AddShortcut(UINT nCmdID, WORD wVirtKeyCode, WORD wModifiers)
{
	// test for invalid modifiers
	if (ValidateModifiers(wModifiers, wVirtKeyCode) != wModifiers)
		return FALSE;

	// check for existing cmds using this shortcut
	DWORD dwShortcut = MAKELONG(wVirtKeyCode, wModifiers);

	if (!nCmdID || !dwShortcut)
		return FALSE;

	UINT nOtherCmdID = 0;

	if (m_mapShortcut2ID.Lookup(dwShortcut, nOtherCmdID) && nOtherCmdID)
		return FALSE;

	// check for existing shortcut on this cmd that we'll need to clean up
	DWORD dwOtherShortcut = 0;

	if (m_mapID2Shortcut.Lookup(nCmdID, dwOtherShortcut))
		m_mapShortcut2ID.RemoveKey(dwOtherShortcut);

	m_mapShortcut2ID[dwShortcut] = nCmdID;
	m_mapID2Shortcut[nCmdID] = dwShortcut;

	return TRUE;
}

WORD CShortcutManager::ValidateModifiers(WORD wModifiers, WORD wVirtKeyCode) const
{
	if (!m_wInvalidComb) // optimization
		return wModifiers;

	// check for our special modifiers first
	if ((m_wInvalidComb & HKCOMB_EXFKEYS) && (wVirtKeyCode >= VK_F1 && wVirtKeyCode <= VK_F24))
		return wModifiers;

	// test for invalid combinations
	BOOL bCtrl = (wModifiers & HOTKEYF_CONTROL);
	BOOL bShift = (wModifiers & HOTKEYF_SHIFT);
	BOOL bAlt = (wModifiers & HOTKEYF_ALT);
	BOOL bExtended = (wModifiers & HOTKEYF_EXT);

	BOOL bFail = ((m_wInvalidComb & HKCOMB_NONE) && !bCtrl && !bShift && !bAlt);

	bFail |= ((m_wInvalidComb & HKCOMB_S) && !bCtrl && bShift && !bAlt);
	bFail |= ((m_wInvalidComb & HKCOMB_C) && bCtrl && !bShift && !bAlt);
	bFail |= ((m_wInvalidComb & HKCOMB_A) && !bCtrl && !bShift && bAlt);
	bFail |= ((m_wInvalidComb & HKCOMB_SC) && bCtrl && bShift && !bAlt);
	bFail |= ((m_wInvalidComb & HKCOMB_SA) && !bCtrl && bShift && bAlt);
	bFail |= ((m_wInvalidComb & HKCOMB_CA) && bCtrl && !bShift && bAlt);
	bFail |= ((m_wInvalidComb & HKCOMB_SCA) && bCtrl && bShift && bAlt);

	if (bFail)
		return (WORD)(m_wFallbackModifiers | (bExtended ? HOTKEYF_EXT : 0x0));

	// else ok
	return wModifiers;
}

UINT CShortcutManager::ProcessMessage(const MSG* pMsg, DWORD* pShortcut) const
{
	// Default handling
	return ProcessKeyDown(pMsg, GetHwnd(), 0, pShortcut);
}


BOOL CShortcutManager::WantProcessMessage(const MSG* pMsg) const
{
	// only process accelerators if we are enabled and visible
	if (IsWindowEnabled() && IsWindowVisible())
	{
		switch (pMsg->message)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			// we only process keypresses
			return TRUE;
		}
	}

	// All else
	return FALSE;
}

UINT CShortcutManager::ProcessKeyDown(const MSG* pMsg, HWND hwndAllowedParent, UINT nAllowedCmdID, DWORD* pShortcut) const
{
	if (!WantProcessMessage(pMsg))
		return 0;

	// also check that it's one of our children with the focus
	// not another popup window, unless it's an edit control
	HWND hFocus = pMsg->hwnd;

	if ((hFocus != hwndAllowedParent) && !::IsChild(hwndAllowedParent, hFocus))
	{
		UINT nStyle = ::GetWindowLong(hFocus, GWL_STYLE);
		
		if (!(nStyle & WS_POPUP) || !CWinClasses::IsClass(hFocus, WC_EDIT))
			return 0;
	}
			
	switch (pMsg->wParam)
	{
	case VK_CONTROL:
	case VK_SHIFT:
	case VK_MENU:
	case VK_NUMLOCK:
	case VK_SCROLL:
	case VK_CAPITAL:
		return 0;
		
	case VK_RETURN:
	case VK_CANCEL:
		// don't handle return/cancel keys
		return 0;

	case VK_MBUTTON:
		// Used for closing tabs
		return 0;

	case VK_UP:
	case VK_DOWN:
		// Don't process 'Alt + Up/Down' destined for a date time control
		if (Misc::IsKeyPressed(VK_MENU) && CWinClasses::IsClass(hFocus, WC_DATETIMEPICK))
			return 0;
		// else fall through
		
	default: 
		// shortcut keys
		{
			// don't process messages destined for hotkey controls!
			if (CWinClasses::IsClass(hFocus, WC_HOTKEY))
				return 0;

			// don't process AltGr if destined for edit control
			BOOL bEdit = CWinClasses::IsEditControl(hFocus);

			if (bEdit && Misc::IsKeyPressed(VK_RMENU))
				return 0;
			
			// get DWORD shortcut
			BOOL bExtKey = (pMsg->lParam & 0x01000000);
			DWORD dwShortcut = Misc::GetShortcut((WORD)pMsg->wParam, bExtKey);
			
			// look it up
			UINT nCmdID = 0;
			
			if (!m_mapShortcut2ID.Lookup(dwShortcut, nCmdID) || !nCmdID)
				return 0;

			if (nAllowedCmdID && (nCmdID != nAllowedCmdID))
				return 0;
			
			// check if HKCOMB_EDITCTRLS is set and a edit has the focus
			// and the shortcut clashes
			if (bEdit && (m_wInvalidComb & HKCOMB_EDITCTRLS))
			{
				// 1. check does not clash with edit shortcuts
				if (IsEditShortcut(dwShortcut))
					return 0;
				
				//WORD wVirtKeyCode = LOWORD(dwShortcut);
				WORD wModifiers = HIWORD(dwShortcut);
				
				// 2. can be a function key
				if (pMsg->wParam >= VK_F1 && pMsg->wParam <= VK_F24)
				{
					// ok
				}
				// 3. else must have <ctrl> or <alt>
				else if (!(wModifiers & (HOTKEYF_ALT | HOTKEYF_CONTROL)))
				{
					return 0;
				}
			}
			
			// return command ID
			if (m_bAutoSendCmds)
				SendMessage(WM_COMMAND, nCmdID);

			if (pShortcut)
				*pShortcut = dwShortcut;
			
			return nCmdID;
		}
	}
	
	return 0;
}

BOOL CShortcutManager::IsEditShortcut(DWORD dwShortcut)
{
	switch (dwShortcut)
	{
	case MAKELONG('C', HOTKEYF_CONTROL): // copy
	case MAKELONG('V', HOTKEYF_CONTROL): // paste
	case MAKELONG('X', HOTKEYF_CONTROL): // cut
	case MAKELONG(VK_LEFT, HOTKEYF_CONTROL | HOTKEYF_EXT): // left one word
	case MAKELONG(VK_RIGHT, HOTKEYF_CONTROL | HOTKEYF_EXT): // right one word
	case MAKELONG(VK_DELETE, 0):
		return TRUE;
	}

	// all else
	return FALSE;
}

LRESULT CShortcutManager::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_INITMENUPOPUP:
		{
			// default processing so all text changes 
			// are complete before we have a go
			LRESULT lr = Default();
			PrepareMenuItems(CMenu::FromHandle((HMENU)wp));

			return lr;
		}
		break;

	case WM_DESTROY:
		{
			// must call rest of chain first 
			LRESULT lr =  CSubclassWnd::WindowProc(hRealWnd, msg, wp, lp);
			HookWindow(NULL);

			return lr;
		}
		break;
	}

	return CSubclassWnd::WindowProc(hRealWnd, msg, wp, lp);
}

void CShortcutManager::PrepareMenuItems(CMenu* pMenu) const
{
	if (!pMenu || !pMenu->GetSafeHmenu())
		return;

	// we iterate all the menu items
	// if we find a match we get the menu text and add the shortcut
	// first removing any existing one
	int nItem = pMenu->GetMenuItemCount();

	while (nItem--)
	{
		UINT nCmdID = pMenu->GetMenuItemID(nItem);
		DWORD dwShortcut = GetShortcut(nCmdID);

		if (!nCmdID || nCmdID == (UINT)-1)
			continue;
		
		// note: we must handle items without shortcuts as well
		// as ones with

		// get the menu text
		MENUITEMINFO minfo;
		minfo.cbSize = sizeof(minfo);
        minfo.fMask = MIIM_FTYPE | MIIM_STRING;
        minfo.fType = MFT_STRING;
		minfo.dwTypeData = NULL;
		minfo.cch = 0;

        ::GetMenuItemInfo(pMenu->GetSafeHmenu(), nItem, TRUE, &minfo);
		CString sMenuText;

		if (!minfo.cch)
			continue; // ??

		minfo.cch++;
		minfo.dwTypeData = sMenuText.GetBuffer(minfo.cch);
		::GetMenuItemInfo(pMenu->GetSafeHmenu(), nItem, TRUE, &minfo);
		sMenuText.ReleaseBuffer();

		// look for '\t' indicating existing hint
		int nTab = sMenuText.Find('\t');
		
		// remove it
		if (nTab >= 0)
			sMenuText = sMenuText.Left(nTab);
		// else if it didn't have one and it has no shortcut then continue
		else if (!dwShortcut)
			continue;
		
		// add new hint
		CString sShortcut = GetShortcutText(dwShortcut);

		if (!sShortcut.IsEmpty())
		{
			sMenuText += '\t';
			sMenuText += sShortcut;
		}

		// update menu item text
		minfo.dwTypeData = (LPTSTR)(LPCTSTR)sMenuText;
		
        ::SetMenuItemInfo(pMenu->GetSafeHmenu(), nItem, TRUE, &minfo);
	}
}

int CShortcutManager::BuildMapping(UINT nMenuID, CStringArray& aMapping, char cDelim) const
{
	CEnMenu menu;

	if (!menu.LoadMenu(nMenuID, GetHwnd(), TRUE, TRUE))
		return 0;

	return BuildMapping(menu, aMapping, cDelim);
}

int CShortcutManager::BuildMapping(const CMenu& menu, CStringArray& aMapping, char cDelim) const
{
	if (!menu.GetMenuItemCount())
		return 0;

	return BuildMapping(&menu, NULL, aMapping, cDelim);
}

int CShortcutManager::BuildMapping(const CMenu* pMenu, LPCTSTR szParentName, 
								   CStringArray& aMapping, char cDelim) const
{
	int nItems = pMenu->GetMenuItemCount();

	for (int nItem = 0; nItem < nItems; nItem++)
	{
		CString sMenuText, sItemText;

		pMenu->GetMenuString(nItem, sMenuText, MF_BYPOSITION);

		if (!Misc::IsEmpty(szParentName))
			sItemText.Format(_T("%s > %s"), szParentName, sMenuText);
		else
			sItemText = sMenuText;

		UINT nCmdID = pMenu->GetMenuItemID(nItem);

		if (nCmdID == (UINT)-1) // sub menu
		{
			CMenu* pSubMenu = pMenu->GetSubMenu(nItem);

			BuildMapping(pSubMenu, sItemText, aMapping, cDelim);
		}
		else
		{
			DWORD dwShortcut = GetShortcut(nCmdID);

			if (dwShortcut)
			{
				CString sShortcut = GetShortcutText(dwShortcut), sItem;
				sItem.Format(_T("%s%c%s"), sShortcut, cDelim, sItemText);

				// remove '&'
				CAcceleratorString::RemoveAccelerator(sItem);

				aMapping.Add(sItem);
			}
		}
	}
	
	// add a space between sections unless already added
	if (aMapping.GetSize() && !aMapping[aMapping.GetSize() - 1].IsEmpty())
		aMapping.Add("");

	return aMapping.GetSize();
}

BOOL CShortcutManager::RemapMenuItemIDs(const CMap<UINT, UINT, UINT, UINT&>& mapCmdIDs)
{
	// Cache and clear the affected shortcuts so we can
	// make in-situ changes which must be unique at all times
	CMap<UINT, UINT, DWORD, DWORD&> mapOldShortcuts;

	POSITION pos = mapCmdIDs.GetStartPosition();
	UINT nOldCmdID, nNewCmdID;
	DWORD dwShortcut;
	BOOL bRemapped = FALSE;

	while (pos)
	{
		mapCmdIDs.GetNextAssoc(pos, nOldCmdID, nNewCmdID);

		if (nNewCmdID == nOldCmdID)
			continue;
		
		dwShortcut = GetShortcut(nOldCmdID);

		if (dwShortcut)
		{
			DeleteShortcut(nOldCmdID);
			mapOldShortcuts[nOldCmdID] = dwShortcut;

			bRemapped = TRUE;
		}
	}

	// Do the remapping
	pos = mapCmdIDs.GetStartPosition();

	while (pos)
	{
		mapCmdIDs.GetNextAssoc(pos, nOldCmdID, nNewCmdID);

		if (nNewCmdID == nOldCmdID)
			continue;
		
		if (mapOldShortcuts.Lookup(nOldCmdID, dwShortcut))
		{
			ASSERT(dwShortcut);

			SetShortcut(nNewCmdID, dwShortcut);
			bRemapped = TRUE;
		}
	}

	return bRemapped;
}

UINT CShortcutManager::GetCommandID(DWORD dwShortcut) const
{
	UINT nCmdID = 0;
	m_mapShortcut2ID.Lookup(dwShortcut, nCmdID);

	return nCmdID;
}

DWORD CShortcutManager::GetShortcut(UINT nCmdID) const
{
	DWORD dwShortcut = 0;
	m_mapID2Shortcut.Lookup(nCmdID, dwShortcut);
	
	return (dwShortcut == NO_SHORTCUT) ? 0 : dwShortcut;
}

void CShortcutManager::DeleteShortcut(UINT nCmdID)
{
	DWORD dwShortcut = 0;

	if (m_mapID2Shortcut.Lookup(nCmdID, dwShortcut))
	{
		m_mapID2Shortcut.RemoveKey(nCmdID);

		// remove reverse mapping too
		m_mapShortcut2ID.RemoveKey(dwShortcut);
	}
}

void CShortcutManager::DeleteAllShortcuts()
{
	m_mapShortcut2ID.RemoveAll();
	m_mapID2Shortcut.RemoveAll();
}

CString CShortcutManager::GetShortcutTextByCmd(UINT nCmdID) const 
{
	return GetShortcutText(GetShortcut(nCmdID));
}

CString CShortcutManager::GetShortcutText(DWORD dwShortcut)
{
	WORD wVirtKeyCode = LOWORD(dwShortcut);
	WORD wModifiers = HIWORD(dwShortcut);

	CString sKey = Misc::GetKeyName(wVirtKeyCode, (wModifiers & HOTKEYF_EXT));

	if (!sKey.IsEmpty())
	{
		CString sText;

		if (wModifiers & HOTKEYF_CONTROL)
		{
			sText += Misc::GetKeyName(VK_CONTROL);
			sText += _T("+");
		}

		if (wModifiers & HOTKEYF_SHIFT)
		{
			sText += Misc::GetKeyName(VK_SHIFT);
			sText += _T("+");
		}

		if (wModifiers & HOTKEYF_ALT)
		{
			sText += Misc::GetKeyName(VK_MENU);
			sText += _T("+");
		}

		return (sText + sKey);
	}

	return _T("");
}

void CShortcutManager::LoadSettings(const IPreferences* pPrefs, LPCTSTR szKey)
{
	ASSERT(pPrefs);

	// load shortcuts overriding any defaults
	CString sShortcuts = pPrefs->GetProfileString(szKey, _T("Shortcuts"));

	if (!sShortcuts.IsEmpty())
	{
		CStringArray aCmdPairs;
		int nPair = Misc::Split(sShortcuts, aCmdPairs, '|');

		while (nPair--)
		{
			CString sShortcut, sCmdID = aCmdPairs[nPair];
						
			if (Misc::Split(sCmdID, sShortcut, ':'))
			{
				UINT nCmdID = (UINT)_ttoi(sCmdID);
				DWORD dwShortcut = (DWORD)_ttoi(sShortcut);

				if (nCmdID && dwShortcut)
					SetShortcut(nCmdID, dwShortcut);
			}
		}
	}
	else  // Backwards compatibility
	{
		int nItem = pPrefs->GetProfileInt(szKey, _T("NumItems"), 0);

		while (nItem--)
		{
			CString sKey;
			sKey.Format(_T("%s\\Item%02d"), szKey, nItem);

			UINT nCmdID = (UINT)pPrefs->GetProfileInt(sKey, _T("CmdID"), 0);
			DWORD dwShortcut = (DWORD)pPrefs->GetProfileInt(sKey, _T("Shortcut"), 0);

			if (nCmdID && dwShortcut)
				SetShortcut(nCmdID, dwShortcut);
		}
	}
}

void CShortcutManager::SaveSettings(IPreferences* pPrefs, LPCTSTR szKey) const
{
	ASSERT(pPrefs);

	CString sShortcuts;
	POSITION pos = m_mapID2Shortcut.GetStartPosition();
	int nItem = 0;

	while (pos)
	{
		UINT nCmdID = 0;
		DWORD dwShortcut = 0;

		m_mapID2Shortcut.GetNextAssoc(pos, nCmdID, dwShortcut);

		if (nCmdID && dwShortcut)
			sShortcuts += Misc::Format(_T("%d:%ld|"), nCmdID, dwShortcut);
	}
	sShortcuts.TrimRight('|');

	// Check and delete old preferences once only
	if (pPrefs->HasProfileSection(Misc::Format(_T("%s\\Item00"), szKey)))
		pPrefs->DeleteProfileSection(szKey, true);

	pPrefs->WriteProfileString(szKey, _T("Shortcuts"), sShortcuts);
}
