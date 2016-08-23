// FocusWatcher.cpp: implementation of the CFocusWatcher class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FocusWatcher.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFocusWatcher::CFocusWatcher() : m_pMainWnd(NULL)
{

}

CFocusWatcher::~CFocusWatcher()
{

}

BOOL CFocusWatcher::Initialize(CWnd* pMainWnd)
{
	if (Instance().InitHooks(HM_CBT))
	{
		Instance().m_pMainWnd = pMainWnd;
		return TRUE;
	}

	return FALSE;
}

void CFocusWatcher::Release()
{
	GetInstance().ReleaseHooks();
}

BOOL CFocusWatcher::OnCbt(int nCode, WPARAM wParam, LPARAM lParam)
{   
	ASSERT (m_hCbtHook);

	if (nCode == HCBT_SETFOCUS)
	{
		ASSERT (m_pMainWnd && m_pMainWnd->GetSafeHwnd());
		m_pMainWnd->SendMessage(WM_FW_FOCUSCHANGE, wParam, lParam);
	}
	
	return FALSE; // to continue routing
}

void CFocusWatcher::UpdateFocus(CWnd* pFocus)
{
	if (pFocus == NULL)
		pFocus = CWnd::GetFocus();

	ASSERT(Instance().m_pMainWnd && Instance().m_pMainWnd->GetSafeHwnd());
	Instance().m_pMainWnd->SendMessage(WM_FW_FOCUSCHANGE, (WPARAM)(pFocus ? pFocus->GetSafeHwnd() : NULL), 0L);
}
