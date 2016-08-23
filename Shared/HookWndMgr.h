// SkinCtrlMgr.h: interface for the CHookWndMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOOKWNDMGR_H__5045517A_7B51_4A99_ABA9_6307094A1B4F__INCLUDED_)
#define AFX_HOOKWNDMGR_H__5045517A_7B51_4A99_ABA9_6307094A1B4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "hookmgr.h"
#include "wclassdefines.h"
#include "winclasses.h"
#include "subclass.h"

#include <afxtempl.h>

template<class MGRTYPE>
class CHookWndMgr : public CHookMgr<MGRTYPE>, ISubclassCallback
{
	friend class CHookMgr<CHookWndMgr>;

public:
	virtual ~CHookWndMgr()
	{
		ReleaseAll();
	}

	static BOOL Initialize()
	{
		return GetInstance().InitHooks();
	}

	static void Release()
	{
		GetInstance().ReleaseAll();
	}

protected:
	CMap<HWND, HWND, CSubclassWnd*, CSubclassWnd*&> m_mapWnds;
	BOOL m_bHooking, m_bClosing;

protected:
	CHookWndMgr() : m_bHooking(FALSE), m_bClosing(FALSE)
	{
	}

	BOOL IsHooked(HWND hWnd)
	{
		return (NULL != GetHookWnd(hWnd));
	}

	virtual CSubclassWnd* NewHookWnd(HWND /*hWnd*/, const CString& /*sClass*/, DWORD /*dwStyle*/) const 
	{ 
		ASSERT(0); 
		return NULL; 
	}

	virtual void PostHookWnd(HWND /*hWnd*/) 
	{ 
	}

	virtual void PostUnhookWnd(HWND /*hWnd*/) 
	{ 
	}

	virtual void OnHookMsg(UINT /*nMsg*/, HWND /*hWnd*/) 
	{ 
	}

	void ReleaseAll()
	{ 
		m_bClosing = TRUE;

		if (m_mapWnds.GetCount())
		{
			DebugMapContents();
			DeleteMap();
		}

		m_bClosing = FALSE;
	}

	virtual BOOL OnCallWndProc(const MSG& msg)
	{   
#ifdef _USRDLL
		// If this is a DLL, need to set up MFC state
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
		
		m_bHooking = TRUE; 
		BOOL bRes = FALSE;

		switch (msg.message)
		{
		case WM_PARENTNOTIFY:
			// special case: we're actually interested in the window
			// sending this message
			if (LOWORD(msg.wParam) == WM_CREATE)
			{
				HWND hwnd = (HWND)msg.lParam;
				
				if (WantHookWnd(hwnd, WM_PARENTNOTIFY, WM_CREATE, 0))
				{
					bRes = HookWnd(hwnd, WM_PARENTNOTIFY);
				}
			}
			break;

		default:
			if (WantHookWnd(msg.hwnd, msg.message, msg.wParam, msg.lParam))
			{
				bRes = HookWnd(msg.hwnd, msg.message);
			}
			break;
		}

		m_bHooking = FALSE;
		return bRes;
	}

	virtual BOOL WantHookWnd(HWND /*hWnd*/, UINT nMsg, WPARAM wp, LPARAM lp) const
	{
#ifdef _USRDLL
		// If this is a DLL, need to set up MFC state
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
		
#ifdef _DEBUG
//		CString sClass = CWinClasses::GetClass(hWnd);
//		CWnd* pWnd = CWnd::FromHandle(hWnd);
#endif

		switch (nMsg)
		{
		case WM_STYLECHANGED:
			if (wp == GWL_STYLE)
			{
				LPSTYLESTRUCT lpSS = (LPSTYLESTRUCT)lp;
				
				if (!(lpSS->styleOld & WS_VISIBLE) && (lpSS->styleNew & WS_VISIBLE))
					return TRUE;
			}
			break;

		case WM_INITMENUPOPUP:
			return TRUE;

		case WM_PARENTNOTIFY:
			if (LOWORD(wp) == WM_CREATE)
				return TRUE;
			break;
			
		case WM_WINDOWPOSCHANGED: // alternative to WM_SHOWWINDOW
			if (wp)
			{
				WINDOWPOS* pWPos = (WINDOWPOS*)lp;

				if (pWPos->flags & SWP_SHOWWINDOW)
					return TRUE;
			}
			break;
			
		case WM_SHOWWINDOW:
			if (wp)
				return TRUE;
			break;
		}

		return FALSE;
	}

	// called after a CSubclassWnd has handled WM_NCDESTROY
	void PostNcDestroy(HWND hWnd)
	{
		// we can now cleanup afterwards 
		UnhookWnd(hWnd);
	}

	BOOL UnhookWnd(HWND hWnd)
	{
#ifdef _USRDLL
		// If this is a DLL, need to set up MFC state
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
		
		CSubclassWnd* pWnd = GetHookWnd(hWnd);

		if (pWnd)
		{
			// don't fully unhook the window yet because it needs
			// to handle WM_NCD
			pWnd->HookWindow(NULL);
			delete pWnd;

			m_mapWnds.RemoveKey(hWnd);
			PostUnhookWnd(hWnd);

			return TRUE;
		}

		// else
		return FALSE;
	}

	void DeleteMap()
	{
		POSITION pos = m_mapWnds.GetStartPosition();

		while (pos)
		{
			HWND hwnd = NULL;
			CSubclassWnd* pWnd = NULL;

			m_mapWnds.GetNextAssoc(pos, hwnd, pWnd);
			delete pWnd;
		}

		m_mapWnds.RemoveAll();
	}

	BOOL HookWnd(HWND hWnd, UINT nMsgID = 0)
	{
#ifdef _USRDLL
		// If this is a DLL, need to set up MFC state
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif

		// if this is the first hook request setup callback for 
		// handling window destruction
		if (m_mapWnds.GetCount() == 0)
			CSubclassWnd::SetCallback(this);

		if (!hWnd)
			return FALSE;
		
		CSubclassWnd* pHookWnd = GetHookWnd(hWnd);
		//CWnd* pWnd = CWnd::FromHandle(hWnd);
		
		if (pHookWnd)
		{
			if (nMsgID)
				OnHookMsg(nMsgID, hWnd);

			// hook any unhooked children
			HookAllChildren(hWnd);

			return TRUE; // already done
		}

		DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
		CString sClass(CWinClasses::GetClassEx(hWnd)); // converts Afx class names into something more useful

		pHookWnd = NewHookWnd(hWnd, sClass, dwStyle);
		
		if (pHookWnd && pHookWnd->HookWindow(hWnd))
		{
			m_mapWnds[hWnd] = pHookWnd;
			PostHookWnd(hWnd);

			// hook children first
			HookAllChildren(hWnd);

			return TRUE;
		}
		
		// else
		delete pHookWnd;
		
		return TRUE;
	}

	CSubclassWnd* GetHookWnd(HWND hWnd) const
	{
		CSubclassWnd* pHookWnd = NULL;
		
		if (m_mapWnds.Lookup(hWnd, pHookWnd))
			return pHookWnd;
		
		return NULL;
	}

	virtual void HookAllChildren(HWND hWnd)
	{
		HWND hChild = ::GetWindow(hWnd, GW_CHILD); 
		
		while (hChild)    
		{
			HookWnd(hChild);
			hChild = GetNextWindow(hChild, GW_HWNDNEXT);
		}
	}

	void DebugMapContents() const
	{
	#ifdef _DEBUG
		// then any popups
		int nItem = 0;
		POSITION pos = m_mapWnds.GetStartPosition();

		while (pos)
		{
			nItem++;
			HWND hwnd = NULL;
			CSubclassWnd* pHookWnd = NULL;

			m_mapWnds.GetNextAssoc(pos, hwnd, pHookWnd);
			ASSERT (pHookWnd);

			TRACE(_T("CHookWndMgr::CMap::Item%d - hWnd = %08X, classname = %s\n"), nItem, hwnd, CWinClasses::GetClass(pHookWnd->GetHwnd()));
		}
	#endif
	}

};

#endif // !defined(AFX_SKINCTRLMGR_H__5045517A_7B51_4A99_ABA9_6307094A1B4F__INCLUDED_)
