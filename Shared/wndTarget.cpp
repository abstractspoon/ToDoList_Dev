#include "stdafx.h"
//#include <windows.h>
#include "wndtarget.h"

//==================================================================
// Class EWindowTarget by Giovanni Bavestrelli
//	heavily altered by d.r.godson
//==================================================================

EWindowTarget::EWindowTarget()
{ 
	m_hWnd = NULL; 
	EOpenWindow();
}

EWindowTarget::~EWindowTarget() 	 
{ 
	ECloseWindow(); 
}

// Create hidden popup window to receive messages
HWND EWindowTarget::EOpenWindow()
{	
	if (EIsWindowOpen()) 
		return m_hWnd;
	
	WNDCLASS wc;  
	memset(&wc,0,sizeof(wc));
	
	char ClassName[]="EWindowTarget";
	HINSTANCE hInstance=GetModuleHandle(NULL);
	
	if (!GetClassInfo(hInstance,ClassName,&wc))
	{	
		wc.lpszClassName = ClassName;
		wc.lpfnWndProc   = EWindowTargetProc;
		wc.cbWndExtra	  = sizeof(EWindowTarget *); 
		wc.hInstance	  = hInstance;
		
		if (!RegisterClass(&wc)) return NULL;
	}
	
	CREATESTRUCT cs;
	memset(&cs,0,sizeof(cs));
	
	cs.hInstance=hInstance; 
	cs.style	=WS_POPUP;
	cs.lpszClass=ClassName;
	cs.lpszName ="";
	
	ECustomizeCreation(cs); // Give user a chance to modify styles
	
	m_hWnd = CreateWindowEx(cs.dwExStyle,cs.lpszClass,
							cs.lpszName, cs.style, cs.x, cs.y, cs.cx,cs.cy,
							cs.hwndParent, cs.hMenu, cs.hInstance, this);

	return m_hWnd;
}	 

// Destroy popup window inside this object
void EWindowTarget::ECloseWindow()
{						
	if (EIsWindowOpen()) 
	{  
		HWND hwnd=m_hWnd;
		m_hWnd=NULL; 
		DestroyWindow(hwnd); 
	}
	else 
		m_hWnd=NULL;
}

long EWindowTarget::EOnWndMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(m_hWnd, msg, wParam, lParam);
}

// Static Window Procedure for windows created inside my objects
LRESULT CALLBACK EWindowTarget::EWindowTargetProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{  

	// If first message, set up pointer to object in window bytes
	// Such pointer was passed in CreateWindowEx as last parameter
	if (msg==WM_NCCREATE)
		SetWindowLong(hwnd, 0, (LONG)((LPCREATESTRUCT)lParam)->lpCreateParams);
	
	// Get pointer to object from window extra bytes
	EWindowTarget * pWindowTarget = (EWindowTarget*)GetWindowLong(hwnd,0); 
	ASSERT (!pWindowTarget->m_hWnd || pWindowTarget->m_hWnd == hwnd);
	
	// Let object process messages
	if (pWindowTarget->m_hWnd)
		return pWindowTarget->EOnWndMessage(msg, wParam, lParam);

	// else
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT EWindowTarget::ESendMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	ASSERT (EIsWindowOpen());

	return ::SendMessage(m_hWnd, message, wParam, lParam);
}

BOOL EWindowTarget::EPostMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	ASSERT (EIsWindowOpen());

	return ::PostMessage(m_hWnd, message, wParam, lParam);
}

