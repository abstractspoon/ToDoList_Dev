// UIExtensionWnd.cpp: implementation of the CUIExtensionWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UIExtensionWnd.h"
#include "misc.h"

#include "..\Interfaces\IUIExtension.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIExtensionWnd::CUIExtensionWnd(IUIExtensionWindow* pExtensionWnd) : m_pExtensionWnd(pExtensionWnd)
{

}

CUIExtensionWnd::~CUIExtensionWnd()
{
	if (m_pExtensionWnd)
	{
		m_pExtensionWnd->Release();
		m_pExtensionWnd = NULL;
	}
}

CUIExtensionWnd::operator HWND() const
{ 
	if (m_pExtensionWnd)
		return m_pExtensionWnd->GetHwnd();
	
	// else
	return NULL;
}

BOOL CUIExtensionWnd::PreTranslateMessage(MSG* pMsg)
{
	if (m_pExtensionWnd)
	{
		// allow tooltip handling thru
		CWnd* pWnd = CWnd::FromHandle(GetSafeHwnd());
		pWnd->FilterToolTipMessage(pMsg);

		// only process if we have the focus
		if (HasFocus())
			return m_pExtensionWnd->ProcessMessage(pMsg);
	}

	return FALSE;
}

BOOL CUIExtensionWnd::Attach(IUIExtensionWindow* pExtensionWnd)
{
	ASSERT (pExtensionWnd && pExtensionWnd->GetHwnd());

	if (pExtensionWnd && pExtensionWnd->GetHwnd())
	{
		// release existing control
		if (m_pExtensionWnd)
			m_pExtensionWnd->Release();

		m_pExtensionWnd = pExtensionWnd;
		return TRUE;
	}

	// else
	return FALSE;
}

void CUIExtensionWnd::SetUITheme(const UITHEME* pTheme)
{
	if (m_pExtensionWnd)
		m_pExtensionWnd->SetUITheme(pTheme);
}

void CUIExtensionWnd::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	if (m_pExtensionWnd && !Misc::IsEmpty(szKey))
		m_pExtensionWnd->SavePreferences(pPrefs, szKey);
}

void CUIExtensionWnd::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	if (m_pExtensionWnd && !Misc::IsEmpty(szKey))
		m_pExtensionWnd->LoadPreferences(pPrefs, szKey, false);
}

BOOL CUIExtensionWnd::HasFocus() const
{
	if (m_pExtensionWnd)
	{
		HWND hFocus = ::GetFocus();
		HWND hThis = GetSafeHwnd();

		return (hFocus == hThis) || ::IsChild(hThis, hFocus);
	}

	// else
	return FALSE;
}

void CUIExtensionWnd::SetFocus()
{
	HWND hwndThis = GetSafeHwnd();

	if (::IsWindowEnabled(hwndThis)/* && !HasFocus()*/)
		::SetFocus(hwndThis);
}
/*
BOOL CUIExtensionWnd::Undo()
{
	if (m_pExtensionWnd)
		return m_pExtensionWnd->Undo();

	// else
	return FALSE;
}

BOOL CUIExtensionWnd::Redo()
{
	if (m_pExtensionWnd)
		return m_pExtensionWnd->Redo();

	// else
	return FALSE;
}
*/
BOOL CUIExtensionWnd::ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
	if (GetSafeHwnd())
		return CWnd::ModifyStyle(*this, dwRemove, dwAdd, nFlags);

	// else
	return FALSE;
}

BOOL CUIExtensionWnd::ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
	if (GetSafeHwnd())
		return CWnd::ModifyStyleEx(*this, dwRemove, dwAdd, nFlags);

	// else
	return FALSE;
}

LRESULT CUIExtensionWnd::SendMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pExtensionWnd)
		return ::SendMessage(*this, message, wParam, lParam);

	return 0L;
}

BOOL CUIExtensionWnd::SetReadOnly(BOOL bReadOnly)
{
	if (m_pExtensionWnd)
	{
		m_pExtensionWnd->SetReadOnly(bReadOnly != FALSE);
		return TRUE;
	}

	return FALSE;
}

BOOL CUIExtensionWnd::PostMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pExtensionWnd)
		return ::PostMessage(*this, message, wParam, lParam);

	return FALSE;
}

LPCTSTR CUIExtensionWnd::GetTypeID() const
{
	if (m_pExtensionWnd)
		return m_pExtensionWnd->GetTypeID();

	// else
	return _T("");
}
