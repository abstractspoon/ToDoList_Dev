// hotkeyctrlex.cpp : implementation file
//

#include "stdafx.h"
#include "hotkeyctrlex.h"
#include "misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHotKeyCtrlEx

CHotKeyCtrlEx::CHotKeyCtrlEx(BOOL bGlobal) : 
	m_wInvalidComb(0), m_wModifiers(0), m_bGlobal(bGlobal)
{
}

CHotKeyCtrlEx::~CHotKeyCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CHotKeyCtrlEx, CHotKeyCtrl)
	//{{AFX_MSG_MAP(CHotKeyCtrlEx)
	//}}AFX_MSG_MAP
	ON_MESSAGE(HKM_SETRULES, OnSetRules)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHotKeyCtrlEx message handlers

LRESULT CHotKeyCtrlEx::OnSetRules(WPARAM wParam, LPARAM lParam)
{
	m_wInvalidComb = (WORD)wParam;
	m_wModifiers = LOWORD(lParam);

	return 0L;
}

void CHotKeyCtrlEx::SetHotKey(WORD wVirtualKeyCode, WORD wModifiers)
{
	CHotKeyCtrl::SetHotKey(wVirtualKeyCode, wModifiers);
}

void CHotKeyCtrlEx::SetHotKey(DWORD dwHotkey)
{
	SetHotKey(LOWORD(dwHotkey), HIWORD(dwHotkey));
}

BOOL CHotKeyCtrlEx::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->hwnd == *this)
	{
		UINT nChar = pMsg->wParam;
		UINT nFlags = HIWORD(pMsg->lParam);
		
		BOOL bCtrl = Misc::IsKeyPressed(VK_CONTROL);
		BOOL bShift = Misc::IsKeyPressed(VK_SHIFT);
		BOOL bAlt = Misc::IsKeyPressed(VK_MENU);
		BOOL bExtended = (nFlags & 0x100);
		
		WORD wModifiers = (WORD)(
			(bCtrl ? HOTKEYF_CONTROL : 0) |
			(bShift ? HOTKEYF_SHIFT : 0) |
			(bAlt ? HOTKEYF_ALT : 0) |
			(bExtended ? HOTKEYF_EXT : 0));
		
		switch (nChar)
		{
		case VK_DELETE:
		case VK_BACK:
		case VK_SPACE:
		case VK_TAB:
			{
				// some keys not to process by themselves
				if (!bCtrl && !bShift && !bAlt)
				{
					switch (nChar)
					{
					case VK_BACK:
						return CHotKeyCtrl::PreTranslateMessage(pMsg);

					case VK_TAB:
						{
							CWnd* pNext = GetParent()->GetNextDlgTabItem(this);

							if (pNext != this)
							{
								pNext->SetFocus();
								return TRUE; // eat the message
							}
						}
						break;
					}
				}

				// check for breaking rules
				BOOL bFail = ((m_wInvalidComb & HKCOMB_NONE) && !bCtrl && !bShift && !bAlt);

				bFail |= ((m_wInvalidComb & HKCOMB_S) && !bCtrl && bShift && !bAlt);
				bFail |= ((m_wInvalidComb & HKCOMB_C) && bCtrl && !bShift && !bAlt);
				bFail |= ((m_wInvalidComb & HKCOMB_A) && !bCtrl && !bShift && bAlt);
				bFail |= ((m_wInvalidComb & HKCOMB_SC) && bCtrl && bShift && !bAlt);
				bFail |= ((m_wInvalidComb & HKCOMB_SA) && !bCtrl && bShift && bAlt);
				bFail |= ((m_wInvalidComb & HKCOMB_CA) && bCtrl && !bShift && bAlt);
				bFail |= ((m_wInvalidComb & HKCOMB_SCA) && bCtrl && bShift && bAlt);

				if (bFail)
					wModifiers = (WORD)(m_wModifiers | (bExtended ? HOTKEYF_EXT : 0));

				SetHotKey((WORD)nChar, wModifiers);

				// send parent notification manually
				GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), EN_CHANGE), (LPARAM)GetSafeHwnd());

				return TRUE;
			}
			break;

			// keys not allowed as global (MS reserved)
		case VK_F12:
			if (m_bGlobal && !bCtrl && !bShift && !bAlt)
				return TRUE; // eat
			break;

			// keys having too much meaning elsewhere
		case VK_RETURN:
		case VK_CANCEL:
			return TRUE; // eat
			
		}
	}
	
	return CHotKeyCtrl::PreTranslateMessage(pMsg);
}
