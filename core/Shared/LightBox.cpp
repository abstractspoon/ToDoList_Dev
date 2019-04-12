// LightBox.cpp: implementation of the CLightBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LightBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifndef SM_REMOTESESSION
#	define SM_REMOTESESSION 0x1000
#endif

CLightBoxMgr::CLightBoxMgr() : m_crBkgnd(GetSysColor(COLOR_3DHILIGHT)), m_nOpaquePercent(50)
{
	
}

CLightBoxMgr::~CLightBoxMgr()
{
	CleanupLightBoxes();
}

void CLightBoxMgr::CleanupLightBoxes()
{
	POSITION pos = m_mapCtrls.GetStartPosition();
	
	while (pos)
	{
		HWND hwnd = NULL;
		CLightBox* pCtrl = NULL;
		
		m_mapCtrls.GetNextAssoc(pos, hwnd, pCtrl);
		delete pCtrl;
	}
	
	m_mapCtrls.RemoveAll();
}

BOOL CLightBoxMgr::Initialize(CWnd* pMainWnd, COLORREF crBkgnd, int nOpaquePercent)
{
	// don't lightbox over remote desktop
	if (GetSystemMetrics(SM_REMOTESESSION))
		return TRUE;

	Instance().m_crBkgnd = crBkgnd;
	Instance().m_nOpaquePercent = nOpaquePercent;
	
	if (Instance().CHookMgr<CLightBoxMgr>::InitHooks(HM_CALLWNDPROC))
	{
		Instance().AttachLightBox(pMainWnd);
		return TRUE;
	}
	
	// else
	return FALSE;
}

void CLightBoxMgr::Release() 
{ 
	Instance().ReleaseHooks(); 
	Instance().CleanupLightBoxes();
}

BOOL CLightBoxMgr::OnCallWndProc(const MSG& msg)
{   
	ASSERT (m_hCallWndHook);

	switch (msg.message)
	{
	case WM_CREATE:
		{
			CWnd* pWnd = CWnd::FromHandle(msg.hwnd);
			AttachLightBox(pWnd);
		}
		break;
		
		
	case WM_NCDESTROY:
		{
			CWnd* pWnd = CWnd::FromHandle(msg.hwnd);
			
			if ((pWnd->GetStyle() & WS_CAPTION) == WS_CAPTION)
			{
				
				CLightBox* pCtrl = NULL;
				
				if (m_mapCtrls.Lookup(msg.hwnd, pCtrl))
				{
					//TRACE ("CLightBoxMgr::DetachLightBox(%s)\n", pWnd->GetRuntimeClass()->m_lpszClassName);
					
					delete pCtrl;
					m_mapCtrls.RemoveKey(msg.hwnd);
				}
			}
		}
		break;
	}
	
	return FALSE; // to continue routing
}

BOOL CLightBoxMgr::AttachLightBox(CWnd* pWnd)
{
	ASSERT(pWnd);
	
	// only captioned top-level windows
	if (((pWnd->GetStyle() & WS_CAPTION) != WS_CAPTION))
		return TRUE;
	
	// don't hook CLightBoxes
	if (pWnd->IsKindOf(RUNTIME_CLASS(CLightBox)))
		return TRUE;
	
	// don't hook temporary windows
	if (CWnd::FromHandlePermanent(*pWnd) == NULL)
		return TRUE;
	
	CLightBox* pCtrl = NULL;
	
	if (!m_mapCtrls.Lookup(*pWnd, pCtrl))
	{
		//TRACE ("CLightBoxMgr::AttachLightBox(%s)\n", pWnd->GetRuntimeClass()->m_lpszClassName);
		
		pCtrl = new CLightBox;
		
		if (!pCtrl->Initialize(pWnd, m_crBkgnd, m_nOpaquePercent))
		{
			delete pCtrl;
			return FALSE;
		}
		
		// else
		m_mapCtrls[pWnd->m_hWnd] = pCtrl;
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CLightBox, CRuntimeDlg)

CLightBox::CLightBox() : m_crBkgnd(GetSysColor(COLOR_3DHILIGHT)), m_nOpaquePercent(50), 
						m_pSetLayeredWindowAttributes(NULL)
{
	
}

CLightBox::~CLightBox()
{
	
}

BEGIN_MESSAGE_MAP(CLightBox, CRuntimeDlg)
//{{AFX_MSG_MAP(CLightBox)
ON_WM_ERASEBKGND()
//ON_WM_TIMER()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CLightBox::Initialize(CWnd* pWnd, COLORREF crBkgnd, int nOpaquePercent)
{
	if ((pWnd->GetStyle() & WS_CAPTION) != WS_CAPTION)
		return TRUE; // nothing to do
	
	m_crBkgnd = crBkgnd;
	m_nOpaquePercent = min(max(0, nOpaquePercent), 100);
	
	// make opacity multiple of 4
	m_nOpaquePercent = (m_nOpaquePercent / 4) * 4;
	
	if (HookWindow(*pWnd))
	{
		// create ourselves hidden
		return CRuntimeDlg::Create(_T(""), WS_POPUP | WS_DISABLED, WS_EX_LAYERED, rectAuto, GetCWnd());
	}
	
	return TRUE;
}

LRESULT CLightBox::WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_ENABLE:
		{
			bool bShow = !wp;

			// only visible windows
			if (!bShow || CSubclassWnd::IsWindowVisible())
				ShowTransparentWnd(bShow);
		}
		break;
	}
	
	return CSubclassWnd::WindowProc(hRealWnd, msg, wp, lp);
}

BOOL CLightBox::ShowTransparentWnd(BOOL bShow)
{
	ASSERT(GetSafeHwnd());
	
	if (!GetSafeHwnd())
		return FALSE;
	
	if (bShow && !m_pSetLayeredWindowAttributes)
	{
		HMODULE hDLL = LoadLibrary(_T("user32"));
		m_pSetLayeredWindowAttributes = (PSLWA) GetProcAddress(hDLL,"SetLayeredWindowAttributes");
		
		if (m_pSetLayeredWindowAttributes == NULL) 
		{
			DestroyWindow();
			return FALSE;
		}
	}
	
	CWnd* pWndUnder = GetCWnd();
	
	// make sure to redraw underlying window first to avoid artifacts
	pWndUnder->Invalidate(TRUE);
	pWndUnder->UpdateWindow();
		
	if (bShow)
	{
		CRect rect;

		pWndUnder->GetClientRect(rect);
		pWndUnder->ClientToScreen(rect);

		// adjust for menu bar
		if (pWndUnder->GetMenu())
			rect.top -= GetSystemMetrics(SM_CYMENU);

		MoveWindow(rect);
		
		m_pSetLayeredWindowAttributes(*this, 0, (unsigned char)((255 * m_nOpaquePercent) / 100), LWA_ALPHA);
		
		ShowWindow(SW_SHOWNOACTIVATE);
	}
	else
		ShowWindow(SW_HIDE);
	
	return TRUE;
}

BOOL CLightBox::OnEraseBkgnd(CDC* pDC)
{
	if (m_crBkgnd != CLR_NONE)
	{
		CRect rClient;
		CRuntimeDlg::GetClientRect(rClient);
		
		pDC->FillSolidRect(rClient, m_crBkgnd);
		return FALSE;
	}
	
	// else
	return CRuntimeDlg::OnEraseBkgnd(pDC);
}
