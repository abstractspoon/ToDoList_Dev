////////////////////////////////////////////////////////////////
// 1997 Microsoft Sytems Journal
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
#ifndef _SUBCLASSW_H
#define _SUBCLASSW_H

//////////////////
// Generic class to hook messages on behalf of a CWnd.
// Once hooked, all messages go to CSubclassWnd::WindowProc before going
// to the window. Specific subclasses can trap messages and do something.
//
// To use:
//
// * Derive a class from CSubclassWnd.
//
// * Override CSubclassWnd::WindowProc to handle messages. Make sure you call
//   CSubclassWnd::WindowProc if you don't handle the message, or your
//   window will never get messages. If you write separate message handlers,
//   you can call Default() to pass the message to the window.
//
// * Instantiate your derived class somewhere and call HookWindow(pWnd)
//   to hook your window, AFTER it has been created.
//	  To unhook, call HookWindow(NULL).
//
// This is a very important class, crucial to many of the widgets Window
// widgets implemented in PixieLib. To see how it works, look at the HOOK
// sample program.
//
// heavily modified and extended by .dan.g. abstractspoon

////////////////////////////////////////////////////////////////////

#ifndef GET_X_LPARAM
	#define GET_X_LPARAM(lp)	((int)(short)LOWORD(lp))
	#define GET_Y_LPARAM(lp)	((int)(short)HIWORD(lp))
#endif

////////////////////////////////////////////////////////////////////

class ISubclassCallback
{
public:
	virtual void PostNcDestroy(HWND hWnd) = 0;
};

////////////////////////////////////////////////////////////////////

class CSubclassWnd
{
	friend class CSubclassWndMap;
	friend class CSubclasser;

public:
	CSubclassWnd();
	virtual ~CSubclassWnd();

	virtual BOOL HookWindow(HWND hRealWnd, CSubclasser* pSubclasser = NULL);
	virtual BOOL IsValid() const { return IsValidHook(); }

	inline HWND GetHwnd() const { return m_hWndHooked; }
	inline CWnd* GetCWnd() const { return CWnd::FromHandle(m_hWndHooked); }

	inline UINT GetDlgCtrlID() const { return ::GetDlgCtrlID(m_hWndHooked); }
	void SetRedraw(BOOL bRedraw = TRUE) { ::SendMessage(m_hWndHooked, WM_SETREDRAW, bRedraw, 0); }
	inline DWORD GetExStyle() const { return ::GetWindowLong(m_hWndHooked, GWL_EXSTYLE); }
	inline DWORD GetStyle() const { return ::GetWindowLong(m_hWndHooked, GWL_STYLE); }
	inline BOOL SetExStyle(DWORD dwExStyle) const { return ::SetWindowLong(m_hWndHooked, GWL_EXSTYLE, dwExStyle); }
	inline BOOL SetStyle(DWORD dwStyle) const { return ::SetWindowLong(m_hWndHooked, GWL_STYLE, dwStyle); }
	inline BOOL HasExStyle(DWORD dwExStyle) const { return (GetExStyle() & dwExStyle) == dwExStyle; }
	inline BOOL HasStyle(DWORD dwStyle) const { return (GetStyle() & dwStyle) == dwStyle; }

	inline HWND GetParent() const { return ::GetParent(m_hWndHooked); }
	inline void GetClientRect(LPRECT pRect) const { ::GetClientRect(m_hWndHooked, pRect); }
	inline void GetWindowRect(LPRECT pRect) const { ::GetWindowRect(m_hWndHooked, pRect); }
	inline void Invalidate(BOOL bErase = TRUE) const { ::InvalidateRect(m_hWndHooked, NULL, bErase); }
	inline BOOL IsWindowEnabled() const { return ::IsWindowEnabled(m_hWndHooked); }
	inline BOOL IsWindowVisible() const { return ::IsWindowVisible(m_hWndHooked); }

	void ClientToWindow(LPRECT pRect) const;
	void ScreenToClient(LPRECT pRect) const;
	void ClientToScreen(LPRECT pRect) const;
	void ScreenToWindow(LPRECT pRect) const;
	void ClientToWindow(LPPOINT pPoint) const;
	void ScreenToClient(LPPOINT pPoint) const;
	void ClientToScreen(LPPOINT pPoint) const;
	void ScreenToWindow(LPPOINT pPoint) const;

	virtual void Redraw() const { Invalidate(); }

	virtual BOOL PostMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0) const;
	virtual BOOL SendMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0) const;

	static void SetCallback(ISubclassCallback* pCallback) { s_pCallback = pCallback; }
	static const MSG* GetCurrentMessage();

	void TraceMessages(BOOL bTrace) { m_bTracing = bTrace; };

protected:
	HWND			m_hWndHooked;		// the window hooked
	WNDPROC			m_pOldWndProc;		// ..and original window proc
	CSubclassWnd*	m_pNext;			// next in chain of hooks for this window
	CSubclasser*	m_pSubclasser;
	BOOL			m_bPreMFCSubclass;
	BOOL			m_bTracing;

#ifdef _DEBUG
	CString			m_sClass;
#endif

	static ISubclassCallback* s_pCallback;

protected:
	// this is called only when m_hWndHooked is detached as a result
	// of receiving WM_NCDESTROY else HookWindow(NULL) was called
	virtual void PreDetachWindow() { }
	virtual void PostDetachWindow() { }

	// Subclass a window. Hook(NULL) to unhook (automatic on WM_NCDESTROY)
	virtual BOOL IsHooked() const { return m_hWndHooked != NULL; }
	virtual BOOL IsValidHook() const { return ::IsWindow(m_hWndHooked); }

	static LRESULT CALLBACK HookWndProc(HWND, UINT, WPARAM, LPARAM);

	static CMapPtrToPtr& GetValidMap(); // map containing every CSubclassWnd
	static BOOL IsValid(const CSubclassWnd* pScWnd);

	virtual LRESULT WindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT Default(); // call this at the end of handler fns if you are happy with the defaults
};

class CSubclasser
{
	friend class CSubclassWndMap;
	friend class CSubclassWnd;

protected:
	CSubclasser() {}

	LRESULT ScWindowProc(CSubclassWnd& sc, UINT msg, WPARAM wp, LPARAM lp)
	{
		ASSERT(sc.IsHooked()); 
		return sc.WindowProc(sc.GetHwnd(), msg, wp, lp); 
	}

	LRESULT ScDefault(CSubclassWnd& sc)
	{
		ASSERT(sc.IsHooked()); 
		return sc.Default(); 
	}

	// this is called only when m_subclass.m_hWndHooked is detached as a result
	// of receiving WM_NCDESTROY else m_subclass.HookWindow(NULL) was called
	virtual void ScPreDetachWindow(HWND /*hRealWnd*/) { }
	virtual void ScPostDetachWindow(HWND /*hRealWnd*/) { }

private:
	virtual LRESULT ScWindowProc(HWND hRealWnd, UINT msg, WPARAM wp, LPARAM lp) = 0;

protected:
};

#endif // _SUBCLASSW_H

