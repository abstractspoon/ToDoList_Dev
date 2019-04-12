////////////////////////////////////////////////////////////////
// PixieLib(TM) Copyright 1997-2004 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
// CStaticLink implements a static control that's a hyperlink
// to any file on your desktop or web. You can use it in dialog boxes
// to create hyperlinks to web sites. When clicked, opens the file/URL
//
#include "StdAfx.h"
#include "StatLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

COLORREF CStaticLink::g_colorUnvisited = RGB(0,0,255);		 // blue
COLORREF CStaticLink::g_colorVisited   = RGB(128,0,128);		 // purple
HCURSOR	CStaticLink::g_hCursorLink = NULL;

IMPLEMENT_DYNAMIC(CStaticLink, CStatic)
BEGIN_MESSAGE_MAP(CStaticLink, CStatic)
	ON_WM_NCHITTEST()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_GETDLGCODE()
	ON_WM_CHAR()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

///////////////////
// Constructor sets default colors = blue/purple.
//
CStaticLink::CStaticLink(LPCTSTR lpText, BOOL bDeleteOnDestroy)
{
	m_link = lpText;								// link text (NULL ==> window text)
	m_color = g_colorUnvisited;				// not visited yet
	m_bDeleteOnDestroy = bDeleteOnDestroy;	// delete object with window?
}

//////////////////
// Normally,	a static control does not get mouse events unless it has
// SS_NOTIFY. This achieves the same effect as SS_NOTIFY, but it's fewer
// lines of code and more reliable than turning on SS_NOTIFY in OnCtlColor
// because Windows doesn't send WM_CTLCOLOR to bitmap static controls.
//
//fabio_2005
#if _MSC_VER >= 1400
LRESULT CStaticLink::OnNcHitTest(CPoint /*point*/)
#else
UINT CStaticLink::OnNcHitTest(CPoint /*point*/)
#endif
{
	return HTCLIENT;
}

//////////////////
// Handle reflected WM_CTLCOLOR to set custom control color.
// For a text control, use visited/unvisited colors and underline font.
// For non-text controls, do nothing. Also ensures SS_NOTIFY is on.
//
HBRUSH CStaticLink::CtlColor(CDC* pDC, UINT nCtlColor)
{
	UNREFERENCED_PARAMETER(nCtlColor);
	ASSERT(nCtlColor == CTLCOLOR_STATIC);
	DWORD dwStyle = GetStyle();
	
	HBRUSH hbr = NULL;
	if ((dwStyle & 0xFF) <= SS_RIGHT) {

		// this is a text control: set up font and colors
		if (!(HFONT)m_font) {
			// first time init: create font
			LOGFONT lf;
			GetFont()->GetObject(sizeof(lf), &lf);
			lf.lfUnderline = TRUE;
			m_font.CreateFontIndirect(&lf);
		}

		// use underline font and visited/unvisited colors
		pDC->SelectObject(&m_font);
		pDC->SetTextColor(m_color);
		pDC->SetBkMode(TRANSPARENT);

		// return hollow brush to preserve parent background color
		hbr = (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
	}
	return hbr;
}

/////////////////
// Handle mouse click: navigate link
//
void CStaticLink::OnLButtonUp(UINT /*nFlags*/, CPoint /*point*/)
{
	Navigate();
}

//////////////////
// Handle key
//
void CStaticLink::OnChar(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
	if (nChar==VK_SPACE) {
		Navigate(); // User pressed SPACE: navigate
	}
}

//////////////////
// Navigate the link
//
BOOL CStaticLink::Navigate()
{
	// first try whatever the hyperlink is
	if (m_link.IsEmpty() || !m_link.Navigate()) {
		if (!m_link.LoadString(GetDlgCtrlID()) || !m_link.Navigate()) {
			GetWindowText(m_link);
			if (!m_link.Navigate()) {
				MessageBeep(0); // unable to navigate!
				TRACE(_T("*** CStaticLink: can't navigate %s ***\n"), (LPCTSTR)m_link);
				return FALSE;
			}
		}
	}
	// success!
	m_color = g_colorVisited;			 // change color
	Invalidate();							 // repaint
	return TRUE;
}
			
//////////////////
// Set "hand" cursor to cue user that this is a link. If app has not set
// g_hCursorLink, then try to get the cursor from winhlp32.exe,
// resource 106, which is a pointing finger. This is a bit of a kludge,
// but it works.
//
BOOL CStaticLink::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/)
{
	if (g_hCursorLink == NULL) {
#ifdef IDC_HAND
		g_hCursorLink = ::LoadCursor(NULL,IDC_HAND);
#else
		static bTriedOnce = FALSE;
		if (!bTriedOnce) {
			CString sWinHlp32;
				
			GetWindowsDirectory(sWinHlp32.GetBuffer(MAX_PATH+1), MAX_PATH);
			sWinHlp32.ReleaseBuffer();
			sWinHlp32 += _T("\\winhlp32.exe");
			HMODULE hModule = LoadLibrary(_T(sWinHlp32));
			if (hModule) {
				g_hCursorLink = CopyCursor(::LoadCursor(hModule, MAKEINTRESOURCE(106)));
			}
			FreeLibrary(hModule);
			bTriedOnce = TRUE;
		}
#endif
	}
	if (g_hCursorLink) {
		::SetCursor(g_hCursorLink);
		return TRUE;
	}
	return FALSE;
}

//////////////////
// Normally, a control class is not destoyed when the window is;
// however, CPixieDlg creates static controls with "new" instead of
// as class members, so it's convenient to allow the option of destroying
// object with window. In applications where you want the object to be
// destoyed along with the window, you can call constructor with
// bDeleteOnDestroy=TRUE.
//
void CStaticLink::PostNcDestroy()
{
	if (m_bDeleteOnDestroy)
		delete this;
}

UINT CStaticLink::OnGetDlgCode()
{
	return DLGC_WANTCHARS;
}

void CStaticLink::OnSetFocus(CWnd* /*pOldWnd*/)
{
	DrawFocusRect();
}

void CStaticLink::OnKillFocus(CWnd* /*pNewWnd*/)
{
	DrawFocusRect();
}

/////////////////
// Gaining or losing focus: draw focus rectangle.
// For bitmaps, use window rect; for text, use actual text rectangle.
//
void CStaticLink::DrawFocusRect()
{
	CWnd* pParent = GetParent();
	ASSERT(pParent);

	// calculate where to draw focus rectangle, in screen coords
	CRect rc;
	DWORD dwStyle = GetStyle();
	if (dwStyle & (SS_BITMAP|SS_ICON|SS_ENHMETAFILE|SS_OWNERDRAW)) {
		GetWindowRect(&rc); // images use full window rectangle

	} else {
		// text uses text rectangle. Don't forget to select font!
		CClientDC dc(this);
		CString s;
		GetWindowText(s);
		CFont* pOldFont = dc.SelectObject(GetFont());
		rc.SetRectEmpty();					 // important--DT_CALCRECT expands, so start empty
		dc.DrawText(s, &rc, DT_CALCRECT); // calculate text rect
		dc.SelectObject(pOldFont);
		ClientToScreen(&rc);					 // convert to screen coords
	}

	rc.InflateRect(1,1);						 // add one pixel all around
	pParent->ScreenToClient(&rc);			 // convert to parent window client coords
	CClientDC dcParent(pParent);			 // parent window's DC
	dcParent.DrawFocusRect(&rc);			 // draw it!
}

