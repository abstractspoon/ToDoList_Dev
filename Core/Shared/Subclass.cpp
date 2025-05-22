////////////////////////////////////////////////////////////////
// 1997 Microsoft Systems Journal. 
// If this program works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
// CSubclassWnd is a generic class for hooking another window's messages.

#include "StdAfx.h"
#include "Subclass.h"
#include "winclasses.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// The message hook map is derived from CMapPtrToPtr, which associates
// a pointer with another pointer. It maps an HWND to a CSubclassWnd, like
// the way MFC's internal maps map HWND's to CWnd's. The first CSubclassWnd
// attached to a window is stored in the map; all other CSubclassWnd's for that
// window are then chained via CSubclassWnd::m_pNext.
//
class CSubclassWndMap : private CMapPtrToPtr 
{
public:
	CSubclassWndMap();
	~CSubclassWndMap();

	void Add(HWND hwnd, CSubclassWnd* pSubclassWnd);
	void Remove(CSubclassWnd* pSubclassWnd);
	void RemoveAll(HWND hwnd);

	CSubclassWnd* Lookup(HWND hwnd);
};

static CSubclassWndMap theHookMap;
static CMapPtrToPtr theSafeMap;

ISubclassCallback* CSubclassWnd::s_pCallback = NULL;

CSubclassWnd::CSubclassWnd() : m_bTracing(FALSE)
{
	m_pNext = NULL;
	m_pOldWndProc = NULL;	
	m_hWndHooked  = NULL;
	m_pSubclasser = NULL;
}

CSubclassWnd::~CSubclassWnd()
{
	if (m_hWndHooked)
		HookWindow((HWND)NULL);

	ASSERT(m_hWndHooked==NULL);		// can't destroy while still hooked!
	ASSERT(m_pOldWndProc==NULL);
}

//////////////////
// Hook a window.
// This installs a new window proc that directs messages to the CSubclassWnd.
// pWnd=NULL to remove.
//

BOOL CSubclassWnd::HookWindow(HWND hWnd, CSubclasser* pSubclasser)
{
	if (hWnd) 
	{
		// Hook the window
		ASSERT(m_hWndHooked == NULL);

		if (m_hWndHooked) // only once
			return FALSE;

		ASSERT(::IsWindow(hWnd));

		if (!::IsWindow(hWnd))
			return FALSE;

		m_hWndHooked = hWnd;
		m_pSubclasser = pSubclasser;

		theHookMap.Add(m_hWndHooked, this);			// Add to map of hooks
		theSafeMap.SetAt((void*)this, NULL);

#ifdef _DEBUG
		m_sClass = CWinClasses::GetClass(hWnd);
#endif
	} 
	else 
	{
		// Unhook the window
		if (m_hWndHooked) 
		{
			// Keep MFC happy by Detaching and then re-Attaching
			CWnd* pPerm = CWnd::FromHandlePermanent(m_hWndHooked);

			if (pPerm)
				pPerm->Detach();

			theHookMap.Remove(this);

			if (pPerm)
				pPerm->Attach(m_hWndHooked);
		}

		theSafeMap.RemoveKey((void*)this);

		m_pOldWndProc = NULL;
		m_pSubclasser = NULL;
		m_hWndHooked = NULL;
		m_pNext = NULL;
	}

	return TRUE;
}

//////////////////
// Window proc-like virtual function which specific CSubclassWnds will
// override to do stuff. Default passes the message to the next hook; 
// the last hook passes the message to the original window.
// You MUST call this at the end of your WindowProc if you want the real
// window to get the message. This is just like CWnd::WindowProc, except that
// a CSubclassWnd is not a window.
//
LRESULT CSubclassWnd::WindowProc(HWND /*hRealWnd*/, UINT msg, WPARAM wp, LPARAM lp)
{
	ASSERT (::IsWindow(m_hWndHooked));

	if (!::IsWindow(m_hWndHooked))
		return 0;

#ifdef _DEBUG
	if (m_bTracing)
		TRACE(_T("CSubclassWnd::WindowProc(msg = %d)\n"), msg);
#endif

	ASSERT(m_pOldWndProc);

	if (m_pNext)
	{
		if (m_pNext->m_pSubclasser)
			return m_pNext->m_pSubclasser->ScWindowProc(m_hWndHooked, msg, wp, lp);
		
		// else
		return m_pNext->WindowProc(m_hWndHooked, msg, wp, lp);
	}
	
	// else	
	return ::CallWindowProc(m_pOldWndProc, m_hWndHooked, msg, wp, lp);
}

//////////////////
// Like calling base class WindowProc, but with no args, so individual
// message handlers can do the default thing. Like CWnd::Default
//
LRESULT CSubclassWnd::Default()
{
	ASSERT (::IsWindow(m_hWndHooked));

	if (!::IsWindow(m_hWndHooked))
		return 0;

	// MFC stores current MSG in thread state
	MSG& curMsg = AfxGetThreadState()->m_lastSentMsg;

	// Note: must explicitly call CSubclassWnd::WindowProc to avoid infinite
	// recursion on virtual function
	return CSubclassWnd::WindowProc(m_hWndHooked, curMsg.message, curMsg.wParam, curMsg.lParam);
}

const MSG* CSubclassWnd::GetCurrentMessage()
{
	static MSG curMsg;
	
	curMsg = AfxGetThreadState()->m_lastSentMsg;
	return &curMsg;
}

//////////////////
// Subclassed window proc for message hooks. Replaces AfxWndProc (or whatever
// else was there before.)
//
LRESULT CALLBACK CSubclassWnd::HookWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	// If this is a non-extension DLL, dynamically linked to MFC
	// then we need to set up MFC state
#if defined(_WINDLL) && defined(_AFXDLL) && !defined(_AFXEXT)
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif

	// Set up MFC message state just in case anyone wants it
	// This is just like AfxCallWindowProc, but we can't use that because
	// a CSubclassWnd is not a CWnd.
	//
	MSG& curMsg = AfxGetThreadState()->m_lastSentMsg;
	MSG  oldMsg = curMsg;   // save for nesting
	curMsg.hwnd		= hwnd;
	curMsg.message = msg;
	curMsg.wParam  = wp;
	curMsg.lParam  = lp;

	// Get hook object for this window. Get from hook map
	CSubclassWnd* pSubclassWnd = theHookMap.Lookup(hwnd);

	if (!pSubclassWnd)
		return 0;

	// see if this is a re-entrant call
	BOOL bInHookWndProc = FALSE;

	VERIFY (theSafeMap.Lookup((void*)pSubclassWnd, (void*&)bInHookWndProc));
	ASSERT (!bInHookWndProc);

	LRESULT lr;

	if (msg == WM_NCDESTROY) 
	{
		// Window is being destroyed: unhook all hooks (for this window)
		// and pass msg to orginal window proc
		WNDPROC wndproc = pSubclassWnd->m_pOldWndProc;
		theHookMap.RemoveAll(hwnd);

		lr = ::CallWindowProc(wndproc, hwnd, msg, wp, lp);

		if (s_pCallback)
			s_pCallback->PostNcDestroy(hwnd);
	} 
	else // pass to msg hook
	{
		if (pSubclassWnd->m_pSubclasser)
			lr = pSubclassWnd->m_pSubclasser->ScWindowProc(pSubclassWnd->GetHwnd(), msg, wp, lp);
		else
			lr = pSubclassWnd->WindowProc(pSubclassWnd->GetHwnd(), msg, wp, lp);
	}

	theSafeMap[(void*)pSubclassWnd] = NULL;

	curMsg = oldMsg;			// pop state
	return lr;
}

BOOL CSubclassWnd::PostMessage(UINT message, WPARAM wParam, LPARAM lParam) const
{
	if (IsValid())
		return ::PostMessage(m_hWndHooked, message, wParam, lParam);

	// else
	return FALSE;
}

BOOL CSubclassWnd::SendMessage(UINT message, WPARAM wParam, LPARAM lParam) const
{
	if (IsValid())
		return ::SendMessage(m_hWndHooked, message, wParam, lParam);

	// else
	return FALSE;
}

BOOL CSubclassWnd::IsValid(const CSubclassWnd* pScWnd)
{
	void* pResult;

	return theSafeMap.Lookup((void*)pScWnd, pResult);
}

void CSubclassWnd::ClientToWindow(LPRECT pRect) const
{
	ClientToWindow((LPPOINT)pRect);
	ClientToWindow(((LPPOINT)pRect) + 1);
}

void CSubclassWnd::ScreenToClient(LPRECT pRect) const
{
	ScreenToClient((LPPOINT)pRect);
	ScreenToClient(((LPPOINT)pRect) + 1);
}

void CSubclassWnd::ClientToScreen(LPRECT pRect) const
{
	ClientToScreen((LPPOINT)pRect);
	ClientToScreen(((LPPOINT)pRect) + 1);
}

void CSubclassWnd::ScreenToWindow(LPRECT pRect) const
{
	CRect rWindow;
	GetWindowRect(rWindow);
	::OffsetRect(pRect, -rWindow.left, -rWindow.top);
}

void CSubclassWnd::ClientToWindow(LPPOINT pPoint) const
{
	ClientToScreen(pPoint);
	ScreenToWindow(pPoint);
}

void CSubclassWnd::ScreenToWindow(LPPOINT pPoint) const
{
	CRect rWindow;
	GetWindowRect(rWindow);

	pPoint->x -= rWindow.left;
	pPoint->y -= rWindow.top;
}

void CSubclassWnd::ScreenToClient(LPPOINT pPoint) const
{
	::ScreenToClient(m_hWndHooked, pPoint);
}

void CSubclassWnd::ClientToScreen(LPPOINT pPoint) const
{
	::ClientToScreen(m_hWndHooked, pPoint);
}

////////////////////////////////////////////////////////////////
// CSubclassWndMap implementation

CSubclassWndMap::CSubclassWndMap()
{
}

CSubclassWndMap::~CSubclassWndMap()
{
//	ASSERT(IsEmpty());	// all hooks should be removed!	
}

/////////////////
// Add hook to map; i.e., associate hook with window
//
void CSubclassWndMap::Add(HWND hwnd, CSubclassWnd* pSubclassWnd)
{
	ASSERT(hwnd && ::IsWindow(hwnd));

	// Add to front of list
	pSubclassWnd->m_pNext = Lookup(hwnd);
	SetAt(hwnd, pSubclassWnd);
	
	// If this is the first hook added, subclass the window
	if (pSubclassWnd->m_pNext == NULL) 
	{
		//WNDPROC wndProc = (WNDPROC)GetWindowLong(hwnd, GWL_WNDPROC);

		pSubclassWnd->m_pOldWndProc = 
			(WNDPROC)SetWindowLong(hwnd, GWL_WNDPROC, (DWORD)CSubclassWnd::HookWndProc);
	} 
	else // just copy wndproc from next hook
	{
		
		pSubclassWnd->m_pOldWndProc = pSubclassWnd->m_pNext->m_pOldWndProc;
	}
	ASSERT(pSubclassWnd->m_pOldWndProc);
}

//////////////////
// Remove hook from map
//
void CSubclassWndMap::Remove(CSubclassWnd* pUnHook)
{
	HWND hwnd = pUnHook->m_hWndHooked;
	CSubclassWnd* pHook = Lookup(hwnd);

	if (!pHook)
		return;

	if (pHook == pUnHook) 
	{
		// hook to remove is the one in the hash table: replace w/next
		if (pHook->m_pNext)
			SetAt(hwnd, pHook->m_pNext);
		else 
		{
			// This is the last hook for this window: restore wnd proc
			RemoveKey(hwnd);
			SetWindowLong(hwnd, GWL_WNDPROC, (DWORD)pHook->m_pOldWndProc);
		}
	} 
	else 
	{
		// Hook to remove is in the middle: just remove from linked list
		while (pHook->m_pNext && pHook->m_pNext != pUnHook)
			pHook = pHook->m_pNext;

		if (pHook)
		{
			ASSERT(pHook->m_pNext == pUnHook);
			pHook->m_pNext = pUnHook->m_pNext;
		}
	}
}

//////////////////
// Remove all the hooks for a window
//
void CSubclassWndMap::RemoveAll(HWND hwnd)
{
	CSubclassWnd* pSubclassWnd;

	while ((pSubclassWnd = Lookup(hwnd))!=NULL)
	{
		if (pSubclassWnd->m_pSubclasser)
			pSubclassWnd->m_pSubclasser->ScPreDetachWindow(hwnd);
		else
			pSubclassWnd->PreDetachWindow();

		pSubclassWnd->HookWindow((HWND)NULL);	// (unhook)

		if (pSubclassWnd->m_pSubclasser)
			pSubclassWnd->m_pSubclasser->ScPostDetachWindow(hwnd);
		else
			pSubclassWnd->PostDetachWindow();
	}
}

/////////////////
// Find first hook associate with window
//
CSubclassWnd* CSubclassWndMap::Lookup(HWND hwnd)
{
	CSubclassWnd* pFound = NULL;

	if (!CMapPtrToPtr::Lookup(hwnd, (void*&)pFound))
		return NULL;

	return pFound;
}
