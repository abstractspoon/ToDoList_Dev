// XHTMLStatic.cpp  Version 1.4
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
// History
//     Version 1.4 - 2007 October 19
//     - Fixed bug where text could be written outside of control's client 
//       rect, reported (with fix) by David Pritchard.
//     - Expanded table of character entities.
//     - Added VS2005 project.
//     - Implemented memory DC to improve performance, suggested by conan.ks.
//     - Switched from WM_TIMER to WM_MOUSEMOVE for display of hand cursor, 
//       suggested by rm2 and RichardC.  This prevents hand cursor from 
//       appearing on overlapping windows.
//
//     Version 1.3 - 2006 August 15
//     - Added transparency support, suggested by Anna
//     - Added support for WM_PRINT, suggested by beaus07
//     - Added support for <center>, requested by several readers
//     - Added support for tooltips for hyperlinks
//     - Load hand cursor from IDC_HAND, suggested by kamnas
//     - Fixed font object leak, reported by furbo
//     - Fixed problem with SetWindowText() reported by Andro67;
//       the background and text colors are no longer reset
//       when SetWindowText() is called.
//     - Fixed bug when control is hidden, reported by RichardC
//
//     Version 1.2 - 2004 June 12
//     - Changed APP: hyperlink to use HWND instead of GetParent();
//     - Added wParam to XHTMLSTATIC_APP_COMMAND struct
//     - Added function SetTextColor(LPCTSTR lpszColor)
//     - Added function SetLogFont(const LOGFONT * pLogFont)
//     - Added function SetWindowText() to call Init and RedrawWindow
//     - Fixed bug with XNamedColors in handling of "255,0,0" style 
//       in SetColorFromString()
//     - Fixed bug with descenders of large serif fonts
//
//     Version 1.1 - 2004 May 20
//     - Implemented SUB tag
//     - Implemented SUP tag
//     - Implemented BIG tag
//     - Implemented SMALL tag
//     - Implemented CODE tag
//     - Implemented HR tag
//     - Implemented APP: hyperlink
//     - Implemented common character entities
//     - Improved parsing performance
//     - Bug fixes
//
//     Version 1.0 - 2002 September 16
//     - Initial public release
//
// Acknowledgements:
//     Thanks to Charles Petzold for explaining how GetTextExtentPoint32()
//     works, in his excellent "Programming Windows", Fifth Edition:
//         http://www.amazon.com/gp/product/157231995X
//
//     Thanks to Chris Maunder for showing how to set the cursor and receive 
//     mouse clicks for static controls, and for all the code that I used 
//     from his CHyperLink class:
//         http://www.codeproject.com/miscctrl/hyperlink.asp
//
// License:
//     This software is released into the public domain.  You are free to use
//     it in any way you like, except that you may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this 
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XHTMLStatic.h"
#include "atlconv.h"		// for Unicode conversion

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4996)	// disable bogus deprecation warning

#ifndef __noop
#if _MSC_VER < 1300
#define __noop ((void)0)
#endif
#endif

#undef TRACE
#define TRACE __noop

#ifndef WM_UPDATEUISTATE
#define WM_UPDATEUISTATE	0x0128
#define UIS_CLEAR			0x2
#define UISF_ACTIVE			0x4
#define UISF_HIDEACCEL		0x2
#define UISF_HIDEFOCUS		0x1
#endif

//=============================================================================
// if you want to see the TRACE output, uncomment this line:
//#include "XTrace.h"
//=============================================================================


//=============================================================================
// some common character entities
//=============================================================================
XHTMLSTATIC_CHAR_ENTITIES CXHTMLStatic::m_aCharEntities[] = 
{
	{ _T("&amp;"),		0,	_T('&') },		// ampersand
	{ _T("&bull;"),		0,	_T('\x95') },	// bullet      NOT IN MS SANS SERIF
	{ _T("&cent;"),		0,	_T('\xA2') },	// cent sign
	{ _T("&copy;"),		0,	_T('\xA9') },	// copyright
	{ _T("&deg;"),		0,	_T('\xB0') },	// degree sign
	{ _T("&euro;"),		0,	_T('\x80') },	// euro sign
	{ _T("&frac12;"),	0,	_T('\xBD') },	// fraction one half
	{ _T("&frac14;"),	0,	_T('\xBC') },	// fraction one quarter
	{ _T("&gt;"),		0,	_T('>') },		// greater than
	{ _T("&iquest;"),	0,	_T('\xBF') },	// inverted question mark
	{ _T("&lt;"),		0,	_T('<') },		// less than
	{ _T("&micro;"),	0,	_T('\xB5') },	// micro sign
	{ _T("&middot;"),	0,	_T('\xB7') },	// middle dot = Georgian comma
	{ _T("&nbsp;"),		0,	_T(' ') },		// nonbreaking space
	{ _T("&para;"),		0,	_T('\xB6') },	// pilcrow sign = paragraph sign
	{ _T("&plusmn;"),	0,	_T('\xB1') },	// plus-minus sign
	{ _T("&pound;"),	0,	_T('\xA3') },	// pound sign
	{ _T("&quot;"),		0,	_T('"') },		// quotation mark
	{ _T("&reg;"),		0,	_T('\xAE') },	// registered trademark
	{ _T("&sect;"),		0,	_T('\xA7') },	// section sign
	{ _T("&sup1;"),		0,	_T('\xB9') },	// superscript one
	{ _T("&sup2;"),		0,	_T('\xB2') },	// superscript two
	{ _T("&times;"),	0,	_T('\xD7') },	// multiplication sign
	{ _T("&trade;"),	0,	_T('\x99') },	// trademark   NOT IN MS SANS SERIF
	{ NULL,				0,	0 }				// MUST BE LAST
};


//=============================================================================
BEGIN_MESSAGE_MAP(CXHTMLStatic, CStatic)
//=============================================================================
	//{{AFX_MSG_MAP(CXHTMLStatic)
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PRINT, OnPrint)
    ON_CONTROL_REFLECT(STN_CLICKED, OnClicked)
	ON_MESSAGE(WM_UPDATEUISTATE, OnUpdateUIState)
END_MESSAGE_MAP()

//=============================================================================
CXHTMLStatic::CXHTMLStatic()
//=============================================================================
{
	memset(&m_lf, 0, sizeof(m_lf));

	m_bRefresh          = TRUE;
	m_bLogFont          = FALSE;
	m_hLinkCursor		= NULL;
	m_paAppCommands     = NULL;
	m_nAppCommands      = 0;
	m_nLeftMargin       = 0;
	m_nRightMargin      = 0;
	m_bToolTip          = FALSE;			// TRUE = display tooltip
	m_hMemDC            = NULL;
	m_hBitmap           = NULL;
	m_hOldBitmap        = NULL;

	m_AnchorRectPtrs.RemoveAll();
	m_AnchorUrls.RemoveAll();

	m_hLinkCursor = AfxGetApp()->LoadStandardCursor(IDC_HAND);

	if (m_hLinkCursor == NULL)			// Still no cursor handle - 
										// load the WinHelp hand cursor
	{
		// this probably necessary only for Win95

		TRACE(_T("Loading hand cursor from winhlp32.exe\n"));

		// This retrieves cursor #106 from winhlp32.exe, which is a hand pointer

		// get hand cursor
		CString strWndDir;
		GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH+1), MAX_PATH);
		strWndDir.ReleaseBuffer();
		strWndDir += _T("\\winhlp32.exe");
		HMODULE hModule = ::LoadLibrary(strWndDir);
		if (hModule) 
		{
			HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
			if (hHandCursor)
				m_hLinkCursor = CopyCursor(hHandCursor);
		}
		::FreeLibrary(hModule);
	}

	InitCharEntities();

	ResetAll();
}

//=============================================================================
CXHTMLStatic::~CXHTMLStatic()
//=============================================================================
{
	ResetAll();

	if (m_hLinkCursor)
		::DestroyCursor(m_hLinkCursor);

	if (m_hMemDC)
	{
		if (m_hOldBitmap)
			SelectObject(m_hMemDC, m_hOldBitmap);
		
		DeleteDC(m_hMemDC);
		m_hMemDC = NULL;
	}
}

//=============================================================================
BOOL CXHTMLStatic::PreTranslateMessage(MSG* pMsg) 
//=============================================================================
{
	if (m_bToolTip && ::IsWindow(m_ToolTip.m_hWnd))
		m_ToolTip.RelayEvent(pMsg);
	return CStatic::PreTranslateMessage(pMsg);
}

//=============================================================================
void CXHTMLStatic::InitCharEntities()
//=============================================================================
{
	for (int i = 0; m_aCharEntities[i].pszName != NULL; i++)
	{
		m_aCharEntities[i].cCode = (TCHAR) (i + 2);	// don't use 0 or 1
	}
}

//=============================================================================
TCHAR CXHTMLStatic::GetCharEntity(TCHAR cCode)
//=============================================================================
{
	TCHAR c = _T(' ');

	for (int i = 0; m_aCharEntities[i].pszName != NULL; i++)
	{
		if (cCode == m_aCharEntities[i].cCode)
		{
			c = m_aCharEntities[i].cSymbol;
			break;
		}
	}

	return c;
}

//=============================================================================
void CXHTMLStatic::ResetAll()
//=============================================================================
{
	TRACE(_T("in CXHTMLStatic::ResetAll\n"));

	Reset();

	m_crBackGround	= ::GetSysColor(COLOR_WINDOW);
	m_crText		= ::GetSysColor(COLOR_WINDOWTEXT);
	m_nLeftMargin	= 0;
	m_nRightMargin	= 0;

	int n = (int)m_AnchorRectPtrs.GetSize();

	for (int i = 0; i < n; i++)
	{
		CRect *pRect = (CRect *) m_AnchorRectPtrs[i];
		if (pRect)
			delete pRect;
	}

	m_AnchorRectPtrs.RemoveAll();
	m_AnchorUrls.RemoveAll();

	if (m_paAppCommands)
		delete [] m_paAppCommands;
	m_paAppCommands = NULL;

	m_nAppCommands = 0;
}

//=============================================================================
// Reset    (now called by SetWindowText() instead of ResetAll())
void CXHTMLStatic::Reset()
//=============================================================================
{
	TRACE(_T("in CXHTMLStatic::Reset\n"));

	m_bRefresh				= TRUE;
	m_bUnderline			= FALSE;
	m_bBold					= FALSE;
	m_bItalic				= FALSE;
	m_bCenter				= FALSE;
	m_bStrikeThrough		= FALSE;
	m_bSubscript			= FALSE;
	m_bSuperscript			= FALSE;
	m_bHorizontalRule		= FALSE;
	m_nHorizontalRuleSize	= 2;
	m_bOnHyperlink			= FALSE;
	m_hPrevCursor			= NULL;
	m_bInAnchor				= FALSE;
	m_bGeneratedText		= FALSE;
}

//=============================================================================
LRESULT CXHTMLStatic::OnPrint(WPARAM wParam, LPARAM /*lParam*/)
//=============================================================================
{
	Draw((HDC) wParam);
	return 0;
}

//=============================================================================
void CXHTMLStatic::OnPaint() 
//=============================================================================
{
	CPaintDC dc(this); // device context for painting
	Draw(dc.m_ps.hdc);
}

//=============================================================================
void CXHTMLStatic::Draw(HDC hDC)
//=============================================================================
{
	TRACE(_T("in CXHTMLStatic::Draw\n"));

	ASSERT(hDC);
	if (hDC == NULL)
		return;

	CRect rect;
	GetClientRect(&rect);

	int nRectWidth  = rect.Width();
	int nRectHeight = rect.Height();

	if (m_bRefresh)
	{
		TRACE(_T("creating bitmap\n"));

		if (m_hOldBitmap && m_hMemDC)
			SelectObject(m_hMemDC, m_hOldBitmap);
		m_hOldBitmap = NULL;

		if (m_hBitmap)
			DeleteObject(m_hBitmap);
		m_hBitmap = NULL;

		if (m_hMemDC)
			DeleteDC(m_hMemDC);
		m_hMemDC = CreateCompatibleDC(hDC);
		ASSERT(m_hMemDC);

		// create bitmap for entire client area
		m_hBitmap = CreateCompatibleBitmap(hDC, nRectWidth, nRectHeight);
		ASSERT(m_hBitmap);

		m_hOldBitmap = (HBITMAP) SelectObject(m_hMemDC, m_hBitmap);

		// copy background from parent
		BitBlt(m_hMemDC, 0, 0, nRectWidth, nRectHeight, hDC, 0, 0, SRCCOPY);

		m_bRefresh = FALSE;
	}
	else
	{
		TRACE(_T("restoring bitmap\n"));
		// restore cached bitmap
		ASSERT(m_hMemDC);
		ASSERT(m_hBitmap);
		BitBlt(hDC, 0, 0, nRectWidth, nRectHeight, m_hMemDC, 0, 0, SRCCOPY);
		return;
	}

	int nAnchors = (int)m_AnchorRectPtrs.GetSize();

	int i = 0;

	for (i = 0; i < nAnchors; i++)
	{
		CRect *pRect = (CRect *) m_AnchorRectPtrs[i];
		if (pRect)
			delete pRect;
		if (m_bToolTip && m_ToolTip.GetToolCount())
		{
			TRACE(_T("deleting tool %d -----\n"), i+1);
			m_ToolTip.DelTool(this, i+1);
		}
	}

	m_AnchorRectPtrs.RemoveAll();
	m_AnchorUrls.RemoveAll();

	int nToolId = 1;	// next tool id

	// get text from control
	CString strText = _T("");
	GetWindowText(strText);

	// replace character entity names with codes

	TCHAR ent[3] = { 0 };
	ent[0] = _T('\001');	// each entity name is replaced with a two-character
							// code that begins with \001

	for (i = 0; m_aCharEntities[i].pszName != NULL; i++)
	{
		ent[1] = m_aCharEntities[i].cCode;
		strText.Replace(m_aCharEntities[i].pszName, ent);
	}

	CString str1 = _T("");
	int index = 0;

	// set text and background colors
	COLORREF crText = m_crText;
	COLORREF prev_crText = crText;

	COLORREF crBackground = m_crBackGround;
	COLORREF prev_crBackground = crBackground;

	if (!(GetExStyle() & WS_EX_TRANSPARENT))
	{
		HBRUSH hbrush = CreateSolidBrush(m_crBackGround); 
		ASSERT(hbrush);
		FillRect(m_hMemDC, &rect, hbrush);
		if (hbrush)
			DeleteObject(hbrush);
	}

	// nothing to do if no text or not visible
	if (strText.IsEmpty() || !IsWindowVisible())
		return;

	int n = strText.GetLength();

	// allow for margins
	rect.left += m_nLeftMargin;
	rect.right -= m_nRightMargin;

	int nInitialXOffset = 0;//m_nLeftMargin;
	m_yStart = rect.top;

	LOGFONT lf = { 0 };
	LOGFONT prev_lf = { 0 };

	if (m_bLogFont)
	{
		memcpy(&lf, &m_lf, sizeof(lf));
	}
	else
	{
		CFont* cf = GetFont();
		if (cf != NULL)
		{
			VERIFY(cf->GetObject(sizeof(lf), &lf));
		}
		else
		{
			HFONT hFont = (HFONT)::GetCurrentObject(hDC, OBJ_FONT);	//+++1.1
			if (hFont)
				GetObject(hFont, sizeof(lf), &lf);
			else
				GetObject(GetStockObject(SYSTEM_FONT), sizeof(lf), &lf);
		}
	}

	lf.lfWeight    = m_bBold ? FW_BOLD : FW_NORMAL;
	lf.lfUnderline = (BYTE) m_bUnderline;
	lf.lfItalic    = (BYTE) m_bItalic;
	lf.lfStrikeOut = (BYTE) m_bStrikeThrough;

	memcpy(&prev_lf, &lf, sizeof(lf));

	// create initial font
	HFONT hNewFont = CreateFontIndirect(&lf);
	ASSERT(hNewFont);
	HFONT hOldFont = (HFONT) SelectObject(m_hMemDC, hNewFont);

	CString strAnchorText = _T("");

	BOOL bSizeChange = FALSE;
	TEXTMETRIC tm = { 0 };
	GetTextMetrics(m_hMemDC, &tm);

	while (n > 0)
	{
		///////////////////////////////////////////////////////////////////////
		if (_tcsnicmp(strText, _T("<B>"), 3) == 0)	// check for <b> or <B>
		{
			n -= 3;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			m_bBold++;// = TRUE;
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("</B>"), 4) == 0)	// check for </B>
		{
			n -= 4;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			if (m_bBold)
				m_bBold--;// = FALSE;
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("<I>"), 3) == 0)	// check for <I>
		{
			n -= 3;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			m_bItalic++;// = TRUE;
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("</I>"), 4) == 0)	// check for </I>
		{
			n -= 4;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			if (m_bItalic)
				m_bItalic--;// = FALSE;
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("<U>"), 3) == 0)		// check for <U>
		{
			n -= 3;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			m_bUnderline++;// = TRUE;
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("</U>"), 4) == 0)	// check for </U>
		{
			n -= 4;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			if (m_bUnderline)
				m_bUnderline--;// = FALSE;
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("<CENTER>"), 8) == 0)	// check for <CENTER>
		{
			n -= 8;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			m_bCenter++;// = TRUE;
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("</CENTER>"), 9) == 0)	// check for </CENTER>
		{
			n -= 9;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			if (m_bCenter)
				m_bCenter--;// = FALSE;
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("<STRIKE>"), 8) == 0)	// check for <STRIKE>
		{
			n -= 8;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			m_bStrikeThrough++;// = TRUE;
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("</STRIKE>"), 9) == 0)	// check for </STRIKE>
		{
			n -= 9;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			if (m_bStrikeThrough)
				m_bStrikeThrough--;// = FALSE;
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("<BIG>"), 5) == 0)	// check for <BIG>
		{
			n -= 5;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			if (lf.lfHeight > 0)
				lf.lfHeight++;
			else
				lf.lfHeight--;
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("</BIG>"), 6) == 0)	// check for </BIG>
		{
			n -= 6;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			if (lf.lfHeight > 0)
				lf.lfHeight--;
			else
				lf.lfHeight++;
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("<SMALL>"), 7) == 0)	// check for <SMALL>
		{
			n -= 7;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			if (lf.lfHeight > 0)
				lf.lfHeight--;
			else
				lf.lfHeight++;
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("</SMALL>"), 8) == 0)	// check for </SMALL>
		{
			n -= 8;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			if (lf.lfHeight > 0)
				lf.lfHeight++;
			else
				lf.lfHeight--;
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("<SUB>"), 5) == 0)	// check for <SUB>
		{
			n -= 5;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			m_bSubscript++;// = TRUE;
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("</SUB>"), 6) == 0)	// check for </SUB>
		{
			n -= 6;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			if (m_bSubscript)
				m_bSubscript--;// = FALSE;
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("<SUP>"), 5) == 0)	// check for <SUP>
		{
			n -= 5;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			m_bSuperscript++;// = TRUE;
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("</SUP>"), 6) == 0)	// check for </SUP>
		{
			n -= 6;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			if (m_bSuperscript)
				m_bSuperscript--;// = FALSE;
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("<FONT"), 5) == 0)	// check for <FONT
		{
			index = strText.Find(_T('>'));
			if (index != -1)
			{
				CString strAttributes = strText.Mid(5, index-5);
				int m = strAttributes.GetLength();
				strText = strText.Mid(index+1);

				// loop to parse FONT attributes
				while (m > 0)
				{
					// trim left whitespace
					if ((strAttributes.GetLength() > 0) && 
						(strAttributes[0] == _T(' ')))
					{
						m--;
						strAttributes = strAttributes.Mid(1);
						continue;
					}

					///////////////////////////////////////////////////////////
					if (_tcsnicmp(strAttributes, _T("COLOR"), 5) == 0)
					{
						int index2 = strAttributes.Find(_T('"'));
						if (index2 != -1)
						{
							m -= index2 + 1;
							strAttributes = strAttributes.Mid(index2+1);

							index2 = strAttributes.Find(_T('"'));
							if (index2 != -1)
							{
								CString strColor = strAttributes.Left(index2);
								CXNamedColors nc(strColor);
								crText = nc.GetRGB();
								strAttributes = strAttributes.Mid(index2+1);
								m = strAttributes.GetLength();
							}
						}
						else
							break;
					}
					///////////////////////////////////////////////////////////
					else if (_tcsnicmp(strAttributes, _T("BGCOLOR"), 7) == 0)
					{
						int index2 = strAttributes.Find(_T('"'));
						if (index2 != -1)
						{
							m -= index2 + 1;
							strAttributes = strAttributes.Mid(index2+1);

							index2 = strAttributes.Find(_T('"'));
							if (index2 != -1)
							{
								CString strBgColor = strAttributes.Left(index2);
								CXNamedColors nc(strBgColor);
								crBackground = nc.GetRGB();
								strAttributes = strAttributes.Mid(index2+1);
								m = strAttributes.GetLength();
							}
						}
						else
							break;
					}
					///////////////////////////////////////////////////////////
					else if (_tcsnicmp(strAttributes, _T("FACE"), 4) == 0)
					{
						int index2 = strAttributes.Find(_T('"'));
						if (index2 != -1)
						{
							m -= index2 + 1;
							strAttributes = strAttributes.Mid(index2+1);
							index2 = strAttributes.Find(_T('"'));
							if (index2 != -1)
							{
								memset(lf.lfFaceName, 0, sizeof(lf.lfFaceName));
								_tcsncpy(lf.lfFaceName, strAttributes, index2);

								m -= index2 + 1;
								if (m > 0)
									strAttributes = strAttributes.Mid(index2+1);
								else
									strAttributes = _T("");
								m = strAttributes.GetLength();
							}
						}
						else
							break;
					}
					///////////////////////////////////////////////////////////
					else if (_tcsnicmp(strAttributes, _T("SIZE"), 4) == 0)
					{
						int index2 = strAttributes.Find(_T('"'));
						if (index2 != -1)
						{
							m -= index2 + 1;
							strAttributes = strAttributes.Mid(index2+1);
							index2 = strAttributes.Find(_T('"'));
							if (index2 != -1)
							{
								int nSize = 0;
								nSize = _ttoi(strAttributes);
								lf.lfHeight -= nSize;
								bSizeChange = TRUE;

								m -= index2 + 1;
								if (m > 0)
									strAttributes = strAttributes.Mid(index2+1);
								else
									strAttributes = _T("");
								m = strAttributes.GetLength();
							}
						}
						else
							break;
					}
					else
					{
						while ((strAttributes.GetLength() > 0) && 
							   (strAttributes[0] != _T(' ')))
						{
							m--;
							strAttributes = strAttributes.Mid(1);
						}
					}
				}
				n -= index + 1;
			}
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("</FONT>"), 7) == 0)	// check for </FONT>
		{
			n -= 7;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			crText = prev_crText;
			crBackground = prev_crBackground;
			memcpy(&lf, &prev_lf, sizeof(lf));
			if (bSizeChange)
				m_yStart += tm.tmDescent;
			bSizeChange = FALSE;
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("<CODE>"), 6) == 0)	// check for <CODE>
		{
			n -= 6;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			_tcscpy(lf.lfFaceName, _T("Courier New"));
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("</CODE>"), 7) == 0)	// check for </CODE>
		{
			n -= 7;
			index = strText.Find(_T('>'));
			if (index != -1)
				strText = strText.Mid(index+1);
			memcpy(&lf, &prev_lf, sizeof(lf));
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		// <a href=www.xyz.com>XYZ Web Site</a>
		else if (_tcsnicmp(strText, _T("<A HREF="), 8) == 0)	// check for <A HREF=
		{
			index = strText.Find(_T('>'));
			if (index != -1)
			{
				strAnchorText = strText.Mid(9, index-10);
				strText = strText.Mid(index+1);
				n = strText.GetLength();
				m_bInAnchor = TRUE;
				continue;
			}
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("</A>"), 4) == 0)	// check for </A>
		{
			strText = strText.Mid(4);
			n -= 4;
			m_bInAnchor = FALSE;
			continue;
		}
		///////////////////////////////////////////////////////////////////////
		else if (_tcsnicmp(strText, _T("<HR"), 3) == 0)	// check for <HR>
		{
			index = strText.Find(_T('>'));
			if (index != -1)
			{
				CString strAttributes = strText.Mid(3); //, index-3);
				int m = strAttributes.GetLength();
				strText = strText.Mid(index+1);

				// loop to parse attributes
				while (m > 0)
				{
					// trim left whitespace
					if ((strAttributes.GetLength() > 0) && 
						(strAttributes[0] == _T(' ')))
					{
						m--;
						strAttributes = strAttributes.Mid(1);
						continue;
					}

					///////////////////////////////////////////////////////////
					if (_tcsnicmp(strAttributes, _T("SIZE"), 4) == 0)
					{
						int index2 = strAttributes.Find(_T('='));
						if (index2 != -1)
						{
							m -= index2 + 1;
							strAttributes = strAttributes.Mid(index2+1);

							index2 = strAttributes.Find(_T('>'));
							if (index2 != -1)
							{
								CString strSize = strAttributes.Left(index2);
								m_nHorizontalRuleSize = _ttoi(strSize);
								strAttributes = strAttributes.Mid(index2+1);
								m = strAttributes.GetLength();
							}
						}
						else
							break;
					}
					else
					{
						while ((strAttributes.GetLength() > 0) && 
							   (strAttributes[0] != _T(' ')))
						{
							m--;
							strAttributes = strAttributes.Mid(1);
						}
					}
				}
				n -= index + 1;
			}

			m_bHorizontalRule++;// = TRUE;
			str1 = _T("\r\n");
			m_bGeneratedText = TRUE;
		}
		///////////////////////////////////////////////////////////////////////
		// <br> or \r\n or plain text
		else
		{
			str1 = strText;
			index = str1.Find(_T('<'));
			if (index != -1)
			{
				if (_tcsnicmp(strText, _T("<BR>"), 4) == 0)	// check for <BR>
				{
					n -= 4;
					str1 = _T("\r\n");
					m_bGeneratedText = TRUE;
					strText = strText.Mid(4);
				}
				else
				{
					str1 = strText.Left(index);
					if (str1.GetLength() <= 0)
					{
						if (strText.GetLength() != 0)
						{
							str1 = strText[0];
							index = 1;
							n -= 1;
						}
					}
					strText = strText.Mid(index);
				}
			}
			else
			{
				str1 = strText;
				strText = _T("");
			}
		}

		lf.lfWeight    = m_bBold ? FW_BOLD : FW_NORMAL;
		lf.lfUnderline = (BYTE) (m_bUnderline || m_bInAnchor);
		lf.lfItalic    = (BYTE) m_bItalic;
		lf.lfStrikeOut = (BYTE) m_bStrikeThrough;

		// update font

		if (hOldFont)
			SelectObject(m_hMemDC, hOldFont);
		if (hNewFont)
			DeleteObject(hNewFont);

		hNewFont = CreateFontIndirect(&lf);
		ASSERT(hNewFont);

		hOldFont = (HFONT) SelectObject(m_hMemDC, hNewFont);

		::SetTextColor(m_hMemDC, crText);

		if (GetExStyle() & WS_EX_TRANSPARENT)
			::SetBkMode(m_hMemDC, TRANSPARENT);
		else
			::SetBkColor(m_hMemDC, crBackground);

		GetTextMetrics(m_hMemDC, &tm);
		int nBaselineAdjust = tm.tmAscent / 2;

		if (m_bSubscript)
		{
			rect.top += nBaselineAdjust;
			rect.bottom += nBaselineAdjust;
			m_yStart += nBaselineAdjust;
		}
		if (m_bSuperscript)
		{
			rect.top -= nBaselineAdjust;
			rect.bottom -= nBaselineAdjust;
			m_yStart -= nBaselineAdjust;
		}
		int saved_left = rect.left;
		if (m_bCenter)
		{
			SIZE size;
			GetTextExtentPoint32(m_hMemDC, str1, (int)_tcslen(str1), &size);
			int w = rect.Width();
			rect.left = (w - size.cx) / 2;
		}

		nInitialXOffset = FormatText(m_hMemDC, str1, &rect, nInitialXOffset);
		rect.left = saved_left;
		if (str1 == _T("\r\n"))
		{
			nInitialXOffset = 0;
		}

		if (m_bSubscript)
		{
			rect.top -= nBaselineAdjust;
			rect.bottom -= nBaselineAdjust;
			m_yStart -= nBaselineAdjust;
		}
		if (m_bSuperscript)
		{
			rect.top += nBaselineAdjust;
			rect.bottom += nBaselineAdjust;
			m_yStart += nBaselineAdjust;
		}

		if (m_bInAnchor)
		{
			SIZE size;
			GetTextExtentPoint32(m_hMemDC, str1, str1.GetLength(), &size); 

			CRect rectDraw = rect;
			rectDraw.bottom = rectDraw.top + size.cy;
			rectDraw.left = nInitialXOffset - size.cx;
			rectDraw.right = nInitialXOffset;

			// save rect for this text - save in window coordinates
			CRect *pRect = new CRect(rectDraw);

			m_AnchorRectPtrs.Add(pRect);
			m_AnchorUrls.Add(strAnchorText);
			TRACE(_T("added anchor:  <%s>\n"), strAnchorText);
			//TRACERECT(*pRect);

			if (m_bToolTip)
			{
				// display tooltip if this is not an app: command
				if ((_tcsnicmp(strAnchorText, _T("APP:"), 4) != 0) &&
					(_tcsnicmp(strAnchorText, _T("\"APP:"), 5) != 0))
				{
					TRACE(_T("adding tool %d <%s>\n"), nToolId, strAnchorText);
					m_ToolTip.AddTool(this, strAnchorText, pRect, nToolId++);
				}
			}
		}

		// draw horizontal rule 
		if (m_bHorizontalRule)
		{
			int nPenWidth = m_nHorizontalRuleSize;
			HPEN hPen = CreatePen(PS_SOLID, nPenWidth, crText);
			ASSERT(hPen);

			if (hPen)
			{
				HPEN hOldPen = (HPEN) SelectObject(m_hMemDC, hPen);

				::MoveToEx(m_hMemDC, rect.left-m_nLeftMargin, rect.top, NULL);
				::LineTo(m_hMemDC, rect.right+m_nRightMargin, rect.top);

				if (hOldPen)
					SelectObject(m_hMemDC, hOldPen);

				DeleteObject(hPen);
			}

			m_yStart += nPenWidth;
			rect.top += nPenWidth;
			rect.bottom += nPenWidth;
			nInitialXOffset = 0;

			m_bHorizontalRule--;
		}

		if (!m_bGeneratedText)
			n -= str1.GetLength();
		m_bGeneratedText = FALSE;
	}

	// end double buffering - limit output to client rect
	BitBlt(hDC, 0, 0, nRectWidth, nRectHeight, m_hMemDC, 0, 0, SRCCOPY);

	// clean up font
	if (hOldFont)
		SelectObject(m_hMemDC, hOldFont);
	if (hNewFont)
		DeleteObject(hNewFont);

	// Do not call CStatic::OnPaint() for painting messages
}

//=============================================================================
BOOL CXHTMLStatic::IsBlank(LPCTSTR lpszText)
//=============================================================================
{
	TCHAR c;
	while ((c = *lpszText++) != _T('\0'))
		if (c != _T(' ') && c != _T('\t'))
			return FALSE;
	return TRUE;
}

//=============================================================================
int CXHTMLStatic::FormatText(HDC hdc, 
							 LPCTSTR lpszText, 
							 RECT * pRect, 
							 int nInitialXOffset)
//=============================================================================
{
	TRACE(_T("in CXHTMLStatic::FormatText:  nInitialXOffset=%d  <%-20.20s>\n"), 
		nInitialXOffset, lpszText);
	//TRACERECT(*pRect);

	int		xStart, nWord, xNext, xLast, nLeftMargin;
	TCHAR	*pText = (TCHAR *) lpszText;
	SIZE	size;

	xNext = nInitialXOffset;
	nLeftMargin = nInitialXOffset;
	xLast = 0;

	if (pRect->top >= (pRect->bottom-1))
		return 0;

	// set initial size
	TCHAR * szTest = _T("abcdefgABCDEFG");
	GetTextExtentPoint32(hdc, szTest, (int)_tcslen(szTest), &size);

	// prepare for next line - clear out the error term
	SetTextJustification(hdc, 0, 0);

	CString strOut = _T("");

	BOOL bReturnSeen = FALSE;

	TEXTMETRIC tm = { 0 };
	::GetTextMetrics(hdc, &tm);

	do									// for each text line
	{
		nWord = 0;						// initialize number of spaces in line

		// skip to first non-space in line
		while (/**pText != _T('\0') && */*pText == _T(' '))
		{
			if (xNext)
				strOut += *pText;
			pText++;
		}

		for(;;)							// process each word
		{
			CString strWord;
			TCHAR *saved_pText = pText;
			strWord = GetNextWord(&pText, &bReturnSeen);

			CString strTrial;
			strTrial = strOut + strWord;

			// after each word, calculate extents
			nWord++;
			GetTextExtentPoint32(hdc, strTrial, strTrial.GetLength(), &size);

			BOOL bOverflow = (size.cx >= (pRect->right - xNext - 2));	
											// don't get too close to margin,
											// in case of italic text

			if (bOverflow)
			{
				if (strOut.IsEmpty())
				{
					bOverflow = FALSE;
					strOut = strWord;

					// FOLLOWING CHANGE SUGGESTED BY DAVID PRITCHARD
					if (xNext > 0) 
					{
						m_yStart += size.cy;
						xStart = 0;
						xNext = 0;
					}
					// --------  END CHANGE  --------
				}
			}
			else
			{
				strOut += strWord;
			}

			if (bReturnSeen || bOverflow || (*pText == _T('\0')))
			{
				if (strOut.IsEmpty())
					break;

				if (bOverflow)
					pText = saved_pText;
				nWord--;               // discount last space at end of line

				// if end of text and no space characters, set pEnd to end

				GetTextExtentPoint32(hdc, strOut, strOut.GetLength(), &size);

				xStart = pRect->left;
				xStart += xNext;
				xNext = 0;
				xLast = xStart + size.cx;

				// display the text

				if ((m_yStart <= (pRect->bottom-size.cy)))// && (!IsBlank(strOut)))
				{
					TextOut(hdc, xStart, m_yStart, strOut, strOut.GetLength());
					if (*pText || bReturnSeen)
						m_yStart += size.cy;
				}

				// prepare for next line - clear out the error term
				SetTextJustification(hdc, 0, 0);

				strOut.Empty();
			}
			else	// new word will fit
			{

			}
		}

		nWord--;               // discount last space at end of line

		// if end of text and no space characters, set pEnd to end

		// prepare for next line - clear out the error term
		SetTextJustification(hdc, 0, 0);

		strOut.Empty();

	} while (*pText && (m_yStart < pRect->bottom));

	if (m_yStart > (pRect->bottom-size.cy))
		pRect->top = pRect->bottom;
	else
		pRect->top = m_yStart;

	return xLast;
}

//=============================================================================
CString CXHTMLStatic::GetNextWord(TCHAR **ppText, BOOL * pbReturnSeen)
//=============================================================================
{
	CString strWord;
	strWord = _T("");
	TCHAR *pText = *ppText;

	*pbReturnSeen = FALSE;

	// skip to next word

	for(;;)
	{
		if (*pText == _T('\0'))
			break;

		// skip \r
		if (*pText == _T('\r'))
			pText++;

		// \n = new line
		if (*pText == _T('\n'))
		{
			strWord += _T(' ');
			pText++;
			*pbReturnSeen = TRUE;
			break;
		}

		TCHAR c = *pText;

		// process character entities
		if (c == _T('\001'))
		{
			c = *++pText;

			c = GetCharEntity(c);
		}

		strWord += c;

		if (*pText++ == _T(' '))
			break;
	}

	*ppText = pText;

	return strWord;
}

//=============================================================================
void CXHTMLStatic::PreSubclassWindow() 
//=============================================================================
{
	TRACE(_T("in CXHTMLStatic::PreSubclassWindow\n"));

    // we want to get mouse clicks via STN_CLICKED
    DWORD dwStyle = GetStyle();
    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);

	CStatic::PreSubclassWindow();

	// create the tooltip
	m_ToolTip.Create(this);
}

//=============================================================================
BOOL CXHTMLStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
//=============================================================================
{
	if (!m_bOnHyperlink)	
		return CStatic::OnSetCursor(pWnd, nHitTest, message);
	else
		return FALSE;
}

//=============================================================================
void CXHTMLStatic::OnClicked()
//=============================================================================
{
	CPoint point;
	::GetCursorPos(&point);

	BOOL bOnHyperlink = FALSE;

	int n = (int)m_AnchorRectPtrs.GetSize();

	if (n != 0)
	{
		int i = 0;
		for (i = 0; i < n; i++)
		{
			CRect *pRect = (CRect *) m_AnchorRectPtrs[i];
			CRect rect = *pRect;
			ClientToScreen(&rect);

			if (rect.PtInRect(point))		 // Cursor is currently over control
			{
				bOnHyperlink = TRUE;
				break;
			}
		}

		if (bOnHyperlink)
			GotoURL(m_AnchorUrls[i], SW_SHOW);
	}
}

//=============================================================================
BOOL CXHTMLStatic::GotoURL(LPCTSTR url, int showcmd)
//=============================================================================
{
	TRACE(_T("in CXHTMLStatic::GotoURL  <%s>\n"), url);

	BOOL bRet = FALSE;

	ASSERT(url);
	ASSERT(url[0] != _T('\0'));

	// check if this is "app:" protocol	
	int nAppSize = 0;
	if (_tcsnicmp(url, _T("APP:"), 4) == 0)
		nAppSize = 4;
	else if (_tcsnicmp(url, _T("\"APP:"), 5) == 0)
		nAppSize = 5;
	if (nAppSize)
	{
		bRet = ProcessAppCommand(&url[nAppSize]);
	}
	else
	{
		// not "app" - assume http: or mailto:

		// First try ShellExecute()
		HINSTANCE result = ::ShellExecute(NULL, _T("open"), url, NULL,NULL, showcmd);

		// If it failed, get the .htm regkey and lookup the program
		if (result <= (HINSTANCE)HINSTANCE_ERROR) 
		{
			TCHAR key[MAX_PATH * 2] = { _T('\0') };

			if (GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) 
			{
				_tcscat(key, _T("\\shell\\open\\command"));

				if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) 
				{
					TCHAR *pos;
					pos = _tcsstr(key, _T("\"%1\""));
					if (pos == NULL) {					   // No quotes found
						pos = _tcsstr(key, _T("%1"));	   // Check for %1, without quotes 
						if (pos == NULL)				   // No parameter at all...
							pos = key+lstrlen(key)-1;
						else
							*pos = _T('\0');			   // Remove the parameter
					}
					else
						*pos = _T('\0');				   // Remove the parameter

					_tcscat(pos, _T(" "));
					_tcscat(pos, url);
					USES_CONVERSION;
					result = (HINSTANCE) (UINT_PTR) WinExec(T2A(key),showcmd);
				}
			}
		}

		bRet = result > (HINSTANCE) HINSTANCE_ERROR;
	}

	return bRet;
}

//=============================================================================
BOOL CXHTMLStatic::ProcessAppCommand(LPCTSTR lpszCommand)
//=============================================================================
{
	TRACE(_T("in CXHTMLStatic::ProcessAppCommand:  %s\n"), lpszCommand);

	BOOL bRet = FALSE;

	CString strCommand(lpszCommand);
	if (strCommand[0] == _T('"'))
		strCommand = strCommand.Mid(1);
	if (strCommand[strCommand.GetLength()-1] == _T('"'))
		strCommand = strCommand.Left(strCommand.GetLength()-1);

	if ((m_nAppCommands > 0) && (m_paAppCommands != NULL))
	{
		for (int i = 0; i < m_nAppCommands; i++)
		{
			if (_tcsicmp(m_paAppCommands[i].pszCommand, strCommand) == 0)
			{
				TRACE(_T("found app command %s\n"), strCommand);
				if (m_paAppCommands[i].hWnd &&
					::IsWindow(m_paAppCommands[i].hWnd))
				{
					::SendMessage(m_paAppCommands[i].hWnd, 
								  m_paAppCommands[i].uMessage, 
								  m_paAppCommands[i].wParam,
								  0);
					bRet = TRUE;
					break;
				}
			}
		}
	}

	return bRet;
}

//=============================================================================
LONG CXHTMLStatic::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
//=============================================================================
{
	HKEY hkey;
	LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);
	
	*retdata = 0;

	if (retval == ERROR_SUCCESS) 
	{
		long datasize = MAX_PATH;
		TCHAR data[MAX_PATH];
		retval = RegQueryValue(hkey, NULL, data, &datasize);
		if (retval == ERROR_SUCCESS) 
		{
			lstrcpy(retdata, data);
			RegCloseKey(hkey);
		}
	}
	
	return retval;
}

//=============================================================================
void CXHTMLStatic::OnDestroy() 
//=============================================================================
{
	CStatic::OnDestroy();
}

//=============================================================================
void CXHTMLStatic::SetWindowText(LPCTSTR lpszString)
//=============================================================================
{
	Reset();
	CStatic::SetWindowText(lpszString);
	RedrawWindow();
}

//=============================================================================
BOOL CXHTMLStatic::OnEraseBkgnd(CDC* /*pDC*/) 
//=============================================================================
{
	return TRUE;//CStatic::OnEraseBkgnd(pDC);
}

//=============================================================================
void CXHTMLStatic::OnMouseMove(UINT nFlags, CPoint pointx) 
//=============================================================================
{
	int n = (int)m_AnchorRectPtrs.GetSize();

	if (n && IsWindowVisible())
	{
		CPoint point;
		::GetCursorPos(&point);

		m_bOnHyperlink = FALSE;

		for (int i = 0; i < n; i++)
		{
			CRect *pRect = (CRect *) m_AnchorRectPtrs[i];
			CRect rect = *pRect;
			ClientToScreen(&rect);

			if (rect.PtInRect(point))		 // Cursor is currently over control
			{
				if (m_hLinkCursor)
				{
					HCURSOR hPrevCursor = ::SetCursor(m_hLinkCursor);
					if (m_hPrevCursor == NULL)
						m_hPrevCursor = hPrevCursor;
					m_bOnHyperlink = TRUE;
					break;
				}
			}
		}

		if (!m_bOnHyperlink)
		{
			if (m_hPrevCursor)
				::SetCursor(m_hPrevCursor);
			m_hPrevCursor = NULL;
		}
	}
	else
	{
		// no hyperlinks, or window is hidden
	}
	
	CStatic::OnMouseMove(nFlags, pointx);
}

LRESULT CXHTMLStatic::OnUpdateUIState(WPARAM wp, LPARAM lp)
{
	// prevents redraw when ALT is pressed
	WORD wAction = LOWORD(wp);
	WORD wElements = HIWORD(wp);

	if (wAction == UIS_CLEAR)
		wElements &= ~UISF_HIDEACCEL;

	return DefWindowProc(WM_UPDATEUISTATE, MAKEWPARAM(wAction, wElements), lp);
}
