////////////////////////////////////////////////////////////////
// PixieLib(TM) Copyright 1997-2004 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
// CStaticLink implements a static control that's a hyperlink
// to any file on your desktop or web. You can use it in dialog boxes
// to create hyperlinks to web sites. When clicked, opens the file/URL
//
#pragma once

//////////////////
// Use this class to force STA (single thread apartment model) threading in
// mixed-mode apps. To use, instantiate a static instance in your main
// application module, eg MyApp.cpp--or anywhere where the constructor will
// run before the CLR.
//
// in app main module:
// CSTAThread forceSTAThread;
//
class CSTAThread {
public:
	CSTAThread() {
		CoInitialize(NULL);
	}
	~CSTAThread() {
		CoUninitialize();
	}
};

//////////////////
// Simple text hyperlink derived from CString
//
class CHyperlink : public CString {
public:
	CHyperlink(LPCTSTR lpLink = NULL) : CString(lpLink) { }
	~CHyperlink() { }
	const CHyperlink& operator=(LPCTSTR lpsz) {
		CString::operator=(lpsz);
		return *this;
	}
	operator LPCTSTR() {
		return CString::operator LPCTSTR(); 
	}

	// Navigate the link. Use rundll32.exe
	BOOL Navigate() {
		return IsEmpty() ? NULL :
			((INT64)ShellExecute(0, _T("open"), *this, 0, 0, SW_SHOWNORMAL)>32);
	}
};

//////////////////
// CStaticLink implements a static control in a dialog that clicks to a web
// link, just like an anchor in HTML. If you want to invoke from keyboard as
// well as moust, don't forget to turn on WS_TABSTOP--either through dialog
// editor or code.
//
class CStaticLink : public CStatic {
public:
	CStaticLink(LPCTSTR lpLink = NULL, BOOL bDeleteOnDestroy=FALSE);
	~CStaticLink() { }

	BOOL Navigate(); // navigate the link (like clicking)

	// Use this if you want to subclass and also set different URL
	BOOL SubclassDlgItem(UINT nID, CWnd* pParent, LPCTSTR lpszLink=NULL) {
		m_link = lpszLink;
		return CStatic::SubclassDlgItem(nID, pParent);
	}

	// Hyperlink contains URL/filename. If NULL, I will use the window text.
	// (GetWindowText) to get the target.
	CHyperlink	m_link;
	COLORREF		m_color;

	// Default colors you can change
	// These are global, so they're the same for all links.
	static COLORREF g_colorUnvisited;
	static COLORREF g_colorVisited;

	// Cursor used when mouse is on a link--you can set, or
	// it will default to the standard hand with pointing finger.
	// This is global, so it's the same for all links.
	static HCURSOR	 g_hCursorLink;

protected:
	CFont			m_font;					// underline font for text control
	BOOL			m_bDeleteOnDestroy;	// delete object when window destroyed?

	void DrawFocusRect();	// draw focus rectangle

	virtual void PostNcDestroy();

	// message handlers
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
//fabio_2005
#if _MSC_VER >= 1400
	afx_msg LRESULT OnNcHitTest(CPoint point);
#else
	afx_msg UINT OnNcHitTest(CPoint point);
#endif

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	DECLARE_DYNAMIC(CStaticLink)
};
