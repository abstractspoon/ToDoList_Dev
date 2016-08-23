// EditShortcutMgr.cpp: implementation of the CEditShortcutMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EditShortcutMgr.h"
#include "misc.h"
#include "winclasses.h"
#include "wclassdefines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEditShortcutMgr::CEditShortcutMgr()
{

}

CEditShortcutMgr::~CEditShortcutMgr()
{

}

BOOL CEditShortcutMgr::Initialize(DWORD dwSelectAllShortcut)
{
	GetInstance().m_dwSelectAllShortcut = dwSelectAllShortcut;

	return GetInstance().InitHooks(HM_KEYBOARD, WC_EDIT);
}

void CEditShortcutMgr::Release()
{
	GetInstance().ReleaseHooks();
}

BOOL CEditShortcutMgr::OnKeyboard(UINT uVirtKey, UINT uFlags)
{
	// only handle key down messages
	BOOL bKeyDown = !(HIWORD(uFlags) & KF_UP);
	BOOL bRepeat = ((HIWORD(uFlags) & KF_REPEAT) > 1);

	HWND hwnd = ::GetFocus();
	
	if (CHookMgr<CEditShortcutMgr>::ClassMatches(hwnd))
	{
		// check for 'Ctrl + backspace'
		if ((uVirtKey == VK_BACK) && Misc::IsKeyPressed(VK_CONTROL))
		{
			// only handle key down but always eat the message
			if (bKeyDown)
			{
				// find first delimiter before selection start
				DWORD dwStart = 0, dwEnd = 0;
				::SendMessage(hwnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);

				// get as much of the text as we need
				if (dwStart)
				{
					CString sText;

					::GetWindowText(hwnd, sText.GetBufferSetLength(dwStart), dwStart+1);
					sText.ReleaseBuffer();

					// reverse it and look for the first word delimiter
					sText.MakeReverse();

					// force the first char to be a non-delim so that
					// it always gets skipped
					sText.SetAt(0, 'P');

					int nFind = sText.FindOneOf(_T(" \t\n.,\\/-"));

					if (nFind != -1)
						nFind = (sText.GetLength() - nFind);
					else
						nFind = 0; // start of string

					// select text
					::SendMessage(hwnd, EM_SETSEL, nFind, dwStart);

					// and delete it
					::SendMessage(hwnd, EM_REPLACESEL, TRUE, (LPARAM)_T(""));
				}
			}

			return TRUE; // eat
		}
		else // check for 'select all' 
		{
			WORD wModifiers = 0;
			
			if (Misc::IsKeyPressed(VK_CONTROL))
				wModifiers |= HOTKEYF_CONTROL;
			
			if (Misc::IsKeyPressed(VK_SHIFT))
				wModifiers |= HOTKEYF_SHIFT;
			
			if (Misc::IsKeyPressed(VK_MENU))
				wModifiers |= HOTKEYF_ALT;
			
			DWORD dwShortcut = MAKELONG(uVirtKey, wModifiers);
			
			if (dwShortcut == m_dwSelectAllShortcut)
			{
				// do the selection first time but always eat
				if (bKeyDown && !bRepeat)
					::SendMessage(hwnd, EM_SETSEL, 0, -1);

				return TRUE; // eat
			}
		}
	}
	
	// all else
	return FALSE;
}
