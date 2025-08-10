// GraphicsMisc.cpp: implementation of the GraphicsMisc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphicsMisc.h"
#include "Misc.h"
#include "FileMisc.h"
#include "themed.h"
#include "osversion.h"
#include "icon.h"
#include "EnImageList.h"
#include "EnBitmapEx.h"
#include "darkmode.h"

#include "..\3rdparty\colordef.h"
#include "..\3rdparty\ShellIcons.h"
#include "..\3rdparty\GdiPlus.h"
#include "..\3rdparty\XNamedColors.h"

#include <windef.h>
#include <afxpriv.h>
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////

// for GradientFill
#pragma comment(lib, "msimg32.lib")

//////////////////////////////////////////////////////////////////////

struct GMFLASHWINFO
{
    UINT  cbSize;
    HWND  hwnd;
    DWORD dwFlags;
    UINT  uCount;
    DWORD dwTimeout;
};

//////////////////////////////////////////////////////////////////////

#ifndef AC_SRC_ALPHA
#	define AC_SRC_ALPHA 0x01
#endif

typedef DWORD ARGB;

//////////////////////////////////////////////////////////////////////

const COLORREF HIGHCONTRAST_SEL_BKCOLOR				= GetSysColor(COLOR_HIGHLIGHT);

const COLORREF THEME_SEL_BKCOLOR					= RGB(160, 215, 255);
const COLORREF THEME_SEL_BORDERCOLOR				= RGB(90, 180, 255);
const COLORREF THEME_SELNOFOCUS_BKCOLOR				= RGB(204, 232, 255);
const COLORREF THEME_SELNOFOCUS_BORDERCOLOR			= THEME_SEL_BKCOLOR;

const COLORREF CLASSICTHEME_SEL_BKCOLOR				= THEME_SEL_BKCOLOR;
const COLORREF CLASSICTHEME_SEL_BORDERCOLOR			= THEME_SEL_BORDERCOLOR;
const COLORREF CLASSICTHEME_SELNOFOCUS_BKCOLOR		= RGB(192, 192, 192);
const COLORREF CLASSICTHEME_SELNOFOCUS_BORDERCOLOR	= RGB(128, 128, 128);

const COLORREF NOTHEME_SEL_BKCOLOR					= GetSysColor(COLOR_HIGHLIGHT);
const COLORREF NOTHEME_SELNOFOCUS_BKCOLOR			= GetSysColor(COLOR_3DFACE);

//////////////////////////////////////////////////////////////////////

static int PointsPerInch() { return 72; }

const int DEFAULT_DPI = 96;

//////////////////////////////////////////////////////////////////////

static int s_nPPI = 0;
static BOOL s_bWantDPIScaling = -1;

static HCURSOR s_curDragDrop[GMOC_COUNT] = { 0 };
static HFONT s_hFontGroup = NULL;

static CMap<CString, LPCTSTR, HCURSOR, HCURSOR> s_mapAppCursors;
static CFont s_fontWingDings;
static CFont m_fontMarlett;
static CEnImageList s_ilShortcutOverlay;

//////////////////////////////////////////////////////////////////////

// private helpers
void InitBitmapInfo(BITMAPINFO *pbmi, ULONG cbInfo, LONG cx, LONG cy, WORD bpp);
BOOL Create32BitHBITMAP(HDC hdc, const SIZE *psize, void **ppvBits, HBITMAP* phBmp);
BOOL HasAlpha(ARGB *pargb, SIZE& sizImage, int cxRow);
BOOL ConvertToPARGB32(HDC hdc, ARGB *pargb, HBITMAP hbmp, SIZE& sizImage, int cxRow);
BOOL ConvertBufferToPARGB32(HPAINTBUFFER hPaintBuffer, HDC hdc, HICON hicon, SIZE& sizIcon);
BOOL HasColor(COLORREF color) { return (color != CLR_NONE); }

//////////////////////////////////////////////////////////////////////

CSaveDC::CSaveDC(HDC hDC) : m_hDC(hDC), m_nSave(-1)
{
	if (m_hDC)
		m_nSave = ::SaveDC(m_hDC);
}

CSaveDC::CSaveDC(CDC* pDC) : m_hDC(pDC ? pDC->m_hDC : NULL), m_nSave(-1)
{
	if (m_hDC)
		m_nSave = ::SaveDC(m_hDC);
}

CSaveDC::~CSaveDC()
{
	if (m_nSave != -1)
		::RestoreDC(m_hDC, m_nSave);
}

//////////////////////////////////////////////////////////////////////

BOOL GraphicsMisc::DrawGradient(GM_GRADIENT nType, CDC* pDC, LPCRECT pRect, 
								COLORREF crFrom, COLORREF crTo, BOOL bHorz, int nBorder)
{
	switch (nType)
	{
	case GMG_GRADIENT:
		DrawGradient(pDC, pRect, crFrom, crTo, bHorz, nBorder);
		return TRUE;

	case GMG_GLASS:
		DrawGlass(pDC, pRect, crFrom, crTo, bHorz, nBorder);
		return TRUE;

	case GMG_GLASSWITHGRADIENT:
		DrawGlassWithGradient(pDC, pRect, crFrom, crTo, bHorz, nBorder);
		return TRUE;
	}
	
	// all the rest
	ASSERT(0);
	return FALSE;
}

GM_GRADIENT GraphicsMisc::GetGradientType(BOOL bGlass, BOOL bGradient)
{
	if (bGradient && bGlass) 
	{
		return GMG_GLASSWITHGRADIENT;
	}
	else if (bGradient) 
	{
		return GMG_GRADIENT;
	}
	else if (bGlass)
	{
		return GMG_GLASS;
	}

	// else
	return GMG_NONE;
}

COLORREF GraphicsMisc::GetGradientEdgeColor(GM_GRADIENT nType, COLORREF color, BOOL /*bFrom*/)
{
	switch (nType)
	{
	case GMG_GRADIENT:
	case GMG_GLASSWITHGRADIENT:
		return color;
		
	case GMG_GLASS:
		return Lighter(color, 0.2);
	}

	// all the rest
	ASSERT(0);
	return color;
}

void GraphicsMisc::DrawGradient(CDC* pDC, LPCRECT pRect, COLORREF crFrom, COLORREF crTo, BOOL bHorz, int nBorder)
{
	TRIVERTEX vert[2];
	GRADIENT_RECT gRect;

	vert[0].x      = pRect->left;
	vert[0].y      = pRect->top;
	vert[0].Red    = MAKEWORD(0, GetRValue(crFrom));
	vert[0].Green  = MAKEWORD(0, GetGValue(crFrom));
	vert[0].Blue   = MAKEWORD(0, GetBValue(crFrom));
	vert[0].Alpha  = 0x0000;
	vert[1].x      = pRect->right;
	vert[1].y      = pRect->bottom; 
	vert[1].Red    = MAKEWORD(0, GetRValue(crTo));
	vert[1].Green  = MAKEWORD(0, GetGValue(crTo));
	vert[1].Blue   = MAKEWORD(0, GetBValue(crTo));
	vert[1].Alpha  = 0x0000;

	gRect.UpperLeft  = 0;
	gRect.LowerRight = 1;

	GradientFill(pDC->GetSafeHdc(), vert, 2, &gRect, 1, bHorz ? GRADIENT_FILL_RECT_H : GRADIENT_FILL_RECT_V);

	if (nBorder >= 0)
	{
		// draw a border in from the edge
		CRect rBorder(pRect);
		rBorder.DeflateRect(nBorder, nBorder);
		DrawRect(pDC, rBorder, CLR_NONE, crFrom);
	}
}

void GraphicsMisc::DrawGlass(CDC* pDC, LPCRECT pRect, COLORREF crFrom, COLORREF crTo, BOOL bHorz, int nBorder)
{
	CRect rBarFrom(pRect), rBarTo(pRect);

	if (bHorz)
	{
		rBarFrom.right = rBarFrom.left + (rBarFrom.Width() * 2 / 5);
		rBarTo.left = rBarFrom.right;
	}
	else // vert
	{
		rBarFrom.bottom = rBarFrom.top + (rBarFrom.Height() * 2 / 5);
		rBarTo.top = rBarFrom.bottom;
	}

	DrawGradient(pDC, rBarFrom, Lighter(crFrom, 0.2), crFrom, bHorz, -1);
	DrawGradient(pDC, rBarTo, crTo, Lighter(crTo, 0.2), bHorz, -1);

	if (nBorder >= 0)
	{
		// draw a border in from the edge
		CRect rBorder(pRect);
		rBorder.DeflateRect(nBorder, nBorder);
		DrawRect(pDC, rBorder, CLR_NONE, crFrom);
	}
}

void GraphicsMisc::DrawGlassWithGradient(CDC* pDC, LPCRECT pRect, COLORREF crFrom, COLORREF crTo, BOOL bHorz, int nBorder)
{
	// draw the glass first
	CRect rBarFrom(pRect), rBarTo(pRect);

	if (bHorz)
	{
		rBarFrom.right = rBarFrom.left + (rBarFrom.Width() * 2 / 10);
		rBarTo.left = rBarTo.right - (rBarTo.Width() * 4 / 10);
	}
	else // vert
	{
		rBarFrom.bottom = rBarFrom.top + (rBarFrom.Height() * 2 / 10);
		rBarTo.top = rBarTo.bottom - (rBarTo.Height() * 4 / 10);
	}

	pDC->FillSolidRect(rBarFrom, crFrom);
	pDC->FillSolidRect(rBarTo, crTo);

	// then the gradient
	CRect rGrad(pRect);

	if (bHorz)
	{
		rGrad.left = rBarFrom.right;
		rGrad.right = rBarTo.left;
	}
	else
	{
		rGrad.top = rBarFrom.bottom;
		rGrad.bottom = rBarTo.top;
	}

	DrawGradient(pDC, rGrad, crFrom, crTo, bHorz, -1); // no border

	if (nBorder >= 0)
	{
		// draw a border in from the edge
		CRect rBorder(pRect);
		rBorder.DeflateRect(nBorder, nBorder);
		DrawRect(pDC, rBorder, CLR_NONE, crFrom);
	}
}

void GraphicsMisc::DrawSplitBar(CDC* pDC, const CRect& rSplitter, COLORREF crSplitBar, BOOL bEdged)
{
	BOOL bVert = (rSplitter.Height() > rSplitter.Width());

	if (bEdged)
	{
		DWORD dwEdges = (bEdged ? (bVert ? (GMDR_LEFT | GMDR_RIGHT) : (GMDR_TOP | GMDR_BOTTOM)) : 0);

		if (Misc::IsHighContrastActive())
			DrawRect(pDC, rSplitter, crSplitBar, GetBestTextColor(crSplitBar), 0, dwEdges);
		else
			DrawRect(pDC, rSplitter, crSplitBar, Darker(crSplitBar, 0.2), 0, dwEdges);
	}
	else
	{
		DrawRect(pDC, rSplitter, crSplitBar);
	}

	// draw drag marker (2 x 20)
	int nSplitWidth = min(rSplitter.Width(), rSplitter.Height());
	
	if (nSplitWidth > 2)
	{
		int nWidth = ScaleByDPIFactor(2);
		int nHeight = ScaleByDPIFactor(20);
		
		CRect rMarker(0, 0, (bVert ? nWidth : nHeight), (bVert ? nHeight : nWidth));
		CentreRect(rMarker, rSplitter, TRUE, TRUE);
		
		// use the splitter bkgnd luminance to decide whether
		// to draw the marker lighter or darker
		if (RGBX(crSplitBar).Luminance() > 128)
			pDC->FillSolidRect(rMarker, Darker(crSplitBar, 0.3));
		else
			pDC->FillSolidRect(rMarker, Lighter(crSplitBar, 0.3));
	}
}

HFONT GraphicsMisc::CreateFont(HFONT hFont, DWORD dwFlags, DWORD dwMask)
{
	if (hFont == NULL)
		hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	LOGFONT lf;
	::GetObject(hFont, sizeof(lf), &lf);
	
	if (dwMask & GMFS_UNDERLINED)
		lf.lfUnderline = (BYTE)(dwFlags & GMFS_UNDERLINED);
	
	if (dwMask & GMFS_ITALIC)
		lf.lfItalic = (BYTE)(dwFlags & GMFS_ITALIC);
	
	if (dwMask & GMFS_STRIKETHRU)
		lf.lfStrikeOut = (BYTE)(dwFlags & GMFS_STRIKETHRU);
	
	if (dwMask & GMFS_BOLD)
		lf.lfWeight = (dwFlags & GMFS_BOLD) ? FW_BOLD : FW_NORMAL;
	
	HFONT hFontOut = CreateFontIndirect(&lf);
	
	// verify the font creation
	if (!IsSameFontNameAndSize(hFont, hFontOut))
	{
		VerifyDeleteObject(hFontOut);
		hFontOut = NULL;
	}
	
	return hFontOut;
}

BOOL GraphicsMisc::CreateFont(CFont& fontOut, HFONT fontIn, DWORD dwFlags, DWORD dwMask)
{
	VerifyDeleteObject(fontOut);

	return fontOut.Attach(CreateFont(fontIn, dwFlags, dwMask));
}

HFONT GraphicsMisc::CreateFont(LPCTSTR szFaceName, int nPoint, DWORD dwFlags)
{
	HFONT hDefFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	ASSERT (hDefFont);
	
	LOGFONT lf;
	::GetObject(hDefFont, sizeof(lf), &lf);
	
	// Fix for regional settings option
	// "Beta: Use Unicode UTF-8 for worldwide language support"
	if (::GetACP() != CP_UTF8)
	{
		lf.lfCharSet = DEFAULT_CHARSET;
	}
	else if (dwFlags & GMFS_SYMBOL)
	{
		lf.lfCharSet = SYMBOL_CHARSET;
	}
	else if (!lf.lfCharSet)
	{
		lf.lfCharSet = DEFAULT_CHARSET;
	}
	
	if (!Misc::IsEmpty(szFaceName))
		lstrcpy(lf.lfFaceName, szFaceName);
	
	if (nPoint > 0)
	{
		lf.lfHeight = -PointToPixel(nPoint);
	}
	else if (dwFlags & GMFS_SYMBOL)
	{
		lf.lfHeight = MulDiv(lf.lfHeight, 12, 10);
	}
	
	lf.lfWidth = 0;
	lf.lfUnderline = (BYTE)(dwFlags & GMFS_UNDERLINED);
	lf.lfItalic = (BYTE)(dwFlags & GMFS_ITALIC);
	lf.lfStrikeOut = (BYTE)(dwFlags & GMFS_STRIKETHRU);
	lf.lfWeight = (dwFlags & GMFS_BOLD) ? FW_BOLD : FW_NORMAL;
	
	HFONT hFont = CreateFontIndirect(&lf);

	// verify the font creation
	if (!SameFont(hFont, szFaceName, nPoint))
	{
		VerifyDeleteObject(hFont);
		hFont = NULL;
	}
	
	return hFont;
}

BOOL GraphicsMisc::CreateFont(CFont& font, LPCTSTR szFaceName, int nPoint, DWORD dwFlags)
{
	VerifyDeleteObject(font);

	return font.Attach(CreateFont(szFaceName, nPoint, dwFlags));
}

DWORD GraphicsMisc::GetFontFlags(HFONT hFont)
{
	if (!hFont)
		return 0;

	LOGFONT lf;
	::GetObject(hFont, sizeof(lf), &lf);

	DWORD dwFlags = 0;
	
	dwFlags |= (lf.lfItalic ? GMFS_ITALIC : 0);
	dwFlags |= (lf.lfUnderline ? GMFS_UNDERLINED : 0);
	dwFlags |= (lf.lfStrikeOut ? GMFS_STRIKETHRU : 0);
	dwFlags |= (lf.lfWeight >= FW_BOLD ? GMFS_BOLD : 0);
	
	return dwFlags;
}

int GraphicsMisc::GetFontNameAndPointSize(HWND hWnd, CString& sFaceName)
{
	return GetFontNameAndPointSize(GetFont(hWnd), sFaceName);
}

int GraphicsMisc::GetFontNameAndPointSize(HFONT hFont, CString& sFaceName)
{
	if (!hFont)
	{
		sFaceName.Empty();
		return 0;
	}
	
	LOGFONT lf = { 0 };
	::GetObject(hFont, sizeof(lf), &lf);
	
	sFaceName = lf.lfFaceName;

	return PixelToPoint(lf.lfHeight);
}

int GraphicsMisc::PointToPixel(int nPoints)
{
	return MulDiv(abs(nPoints), PixelsPerInch(), PointsPerInch());
}

int GraphicsMisc::PixelToPoint(int nPixels)
{
	return MulDiv(abs(nPixels), PointsPerInch(), PixelsPerInch());
}

int GraphicsMisc::PixelsPerInch()
{
	if (s_nPPI == 0)
	{
		HDC hDC = ::GetDC(NULL);
		s_nPPI = GetDeviceCaps(hDC, LOGPIXELSY);
		::ReleaseDC(NULL, hDC);
	}

	return s_nPPI;
}

int GraphicsMisc::GetFontPointSize(HFONT hFont)
{
	return PixelToPoint(GetFontPixelSize(hFont));
}

int GraphicsMisc::GetFontPixelSize(HFONT hFont)
{
	if (!hFont)
		return 0;
	
	LOGFONT lf = { 0 };
	::GetObject(hFont, sizeof(lf), &lf);
	
	return abs(lf.lfHeight);
}

int GraphicsMisc::GetFontPointSize(HWND hWnd)
{
	return GetFontPointSize(GetFont(hWnd));
}

int GraphicsMisc::GetFontPixelSize(HWND hWnd)
{
	return GetFontPixelSize(GetFont(hWnd));
}

BOOL GraphicsMisc::GetFontMetrics(HWND hWnd, TEXTMETRIC& tm, HFONT hFont)
{
	ASSERT(hWnd);

	BOOL bResult = FALSE;

	if (hWnd)
	{
		CClientDC dc(CWnd::FromHandle(hWnd));

		if (!hFont)
			hFont = GetFont(hWnd);

		HFONT hOldFont = PrepareDCFont(&dc, hWnd);

		bResult = dc.GetTextMetrics(&tm);

		dc.SelectObject(hOldFont);
	}

	return bResult;
}

HFONT GraphicsMisc::GetFont(HWND hWnd, BOOL bFallback)
{
	ASSERT(hWnd || bFallback);

	HFONT hFont = NULL;
	
	if (hWnd)
		hFont = (HFONT)::SendMessage(hWnd, WM_GETFONT, 0, 0);

	if (!hFont && bFallback)
		hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	return hFont;
}

BOOL GraphicsMisc::SameFont(HFONT hFont, LPCTSTR szFaceName, int nPoint)
{
	if (!hFont)
		return FALSE;

	CString sFontName;
	int nFontSize = GetFontNameAndPointSize(hFont, sFontName);

	return ((nPoint <= 0 || nPoint == nFontSize) && 
			(!szFaceName || sFontName.CompareNoCase(szFaceName) == 0));
}

BOOL GraphicsMisc::IsSameFontNameAndSize(HFONT hFont1, HFONT hFont2)
{
	if (!hFont1 || !hFont2)
		return FALSE;
	
	CString sName1;
	int nSize1 = GetFontNameAndPointSize(hFont1, sName1);

	return SameFont(hFont2, sName1, nSize1);
}

HICON GraphicsMisc::LoadIcon(UINT nIDIcon, int nSize)
{
	HICON hIcon = (HICON)::LoadImage(AfxGetResourceHandle(), 
									MAKEINTRESOURCE(nIDIcon), 
									IMAGE_ICON, 
									nSize, 
									nSize, 
									LR_LOADMAP3DCOLORS);

	ASSERT(GetIconSize(hIcon).cx == nSize);

	return hIcon;
}

HCURSOR GraphicsMisc::LoadStandardCursor(LPCTSTR szCursorID)
{
	return ::LoadCursor(NULL, szCursorID); 
}

BOOL GraphicsMisc::SetStandardCursor(LPCTSTR szCursorID)
{
	HCURSOR hCursor = LoadStandardCursor(szCursorID);

	if (hCursor)
		::SetCursor(hCursor);

	return (hCursor != NULL);
}

HCURSOR GraphicsMisc::LoadAfxCursor(int nCursorID)
{
	return AfxGetApp()->LoadCursor(nCursorID);
}

BOOL GraphicsMisc::SetAfxCursor(int nCursorID)
{
	HCURSOR hCursor = LoadAfxCursor(nCursorID);

	if (hCursor)
		::SetCursor(hCursor);

	return (hCursor != NULL);
}

HCURSOR GraphicsMisc::LoadHandCursor()
{
	HCURSOR cursor = ::LoadCursor(NULL, IDC_HAND);

	return cursor;
}

BOOL GraphicsMisc::SetHandCursor()
{
	HCURSOR hCursor = LoadHandCursor();

	if (hCursor)
		::SetCursor(hCursor);

	return (hCursor != NULL);
}

HCURSOR GraphicsMisc::LoadDragDropCursor(GM_OLECURSOR nCursor)
{
	// load once only
	if (s_curDragDrop[nCursor] == NULL)
	{
		HMODULE hMod = LoadLibrary(_T("Ole32.dll"));
		
		if (hMod)
		{
			UINT nIDCursor = 0;

			switch (nCursor)
			{
			case GMOC_NO:	nIDCursor = 1; break;
			case GMOC_MOVE: nIDCursor = 2; break;
			case GMOC_COPY: nIDCursor = 3; break;
			case GMOC_LINK: nIDCursor = 4; break;

			default:
				ASSERT(0);
				return NULL;
			}
			s_curDragDrop[nCursor] = ::LoadCursor(hMod, MAKEINTRESOURCE(nIDCursor));
		}
	}

	return s_curDragDrop[nCursor];
}

BOOL GraphicsMisc::SetDragDropCursor(GM_OLECURSOR nCursor)
{
	HCURSOR hCursor = LoadDragDropCursor(nCursor);

	if (hCursor)
		::SetCursor(hCursor);

	return (hCursor != NULL);
}

HCURSOR GraphicsMisc::LoadAppCursor(LPCTSTR szName, LPCTSTR szSubFolder)
{
	CString sCursorPath = FileMisc::TerminatePath(FileMisc::GetAppFolder(szSubFolder));
	sCursorPath += szName;

	FileMisc::ReplaceExtension(sCursorPath, _T("cur")).MakeUpper();

	if (FileMisc::FileExists(sCursorPath))
	{
		HCURSOR hCursor = NULL;
		s_mapAppCursors.Lookup(sCursorPath, hCursor);

		if (!hCursor)
		{
			hCursor = (HCURSOR)::LoadImage(NULL,
										   sCursorPath,
										   IMAGE_CURSOR,
										   GetSystemMetrics(SM_CXCURSOR),
										   GetSystemMetrics(SM_CYCURSOR),
										   LR_LOADFROMFILE | LR_MONOCHROME | LR_SHARED);

			s_mapAppCursors[sCursorPath] = hCursor;
		}

		return hCursor;
	}

	return NULL;
}

BOOL GraphicsMisc::SetAppCursor(LPCTSTR szName, LPCTSTR szSubFolder)
{
	HCURSOR hCursor = LoadAppCursor(szName, szSubFolder);

	if (hCursor)
		::SetCursor(hCursor);

	return (hCursor != NULL);
}

CFont& GraphicsMisc::WingDings()
{
	if (!s_fontWingDings.GetSafeHandle())
		s_fontWingDings.Attach(CreateFont(_T("Wingdings"), -1, GMFS_SYMBOL));

	return s_fontWingDings;
}

CFont& GraphicsMisc::Marlett()
{
	if (!m_fontMarlett.GetSafeHandle())
		m_fontMarlett.Attach(CreateFont(_T("Marlett"), -1, GMFS_SYMBOL));

	return m_fontMarlett;
}

int GraphicsMisc::GetTextWidth(const CString& sText, CWnd& wndRef, CFont* pRefFont)
{
	CClientDC dc(&wndRef);
	ASSERT_VALID(&dc);

	if (pRefFont == NULL)
		pRefFont = wndRef.GetFont();

	CFont* pOldFont = dc.SelectObject(pRefFont);
	int nLength = dc.GetTextExtent(sText).cx;

	dc.SelectObject(pOldFont);

	return nLength;
}

int GraphicsMisc::GetTextWidth(const CString& sText, HWND hWndRef, HFONT hFontRef)
{
	CWnd* pRefWnd = CWnd::FromHandle(hWndRef);

	if (!pRefWnd)
		return -1;

	// else
	return GetTextWidth(sText, *pRefWnd, CFont::FromHandle(hFontRef));
}

CSize GraphicsMisc::GetTextSize(const CString& sText, HWND hWndRef, int nWidth, HFONT hFontRef)
{
	CWnd* pRefWnd = CWnd::FromHandle(hWndRef);

	if (!pRefWnd)
		return CSize(-1, -1);

	// else
	return GetTextSize(sText, *pRefWnd, nWidth, CFont::FromHandle(hFontRef));
}

CSize GraphicsMisc::GetTextSize(const CString& sText, CWnd& wndRef, int nWidth, CFont* pRefFont)
{
	CClientDC dc(&wndRef);
	ASSERT_VALID(&dc);

	if (pRefFont == NULL)
		pRefFont = wndRef.GetFont();

	CFont* pOldFont = dc.SelectObject(pRefFont);

	CRect rText(0, 0, nWidth, 0);
	VERIFY(dc.DrawText(sText, rText, (DT_CALCRECT | DT_WORDBREAK)) > 0);

	dc.SelectObject(pOldFont);

	return CSize(rText.Width(), rText.Height());
}

int GraphicsMisc::GetTextWidth(CDC* pDC, const CString& sText)
{
	return pDC->GetTextExtent(sText).cx;
}

int GraphicsMisc::GetTabbedTextWidth(CDC* pDC, const CString& sText, int nTabWidth)
{
	if (nTabWidth < 0)
	{
		CRect rItem(0, 0, 10000, 100);

		if (pDC->DrawText(sText, rItem, DT_EXPANDTABS | DT_CALCRECT) > 0)
			return rItem.Width();

		// else
		ASSERT(0);
		return 0;
	}
	
	if (nTabWidth == 0)
		return pDC->GetTabbedTextExtent(sText, 0, NULL).cx;
	
	// else
	return pDC->GetTabbedTextExtent(sText, 1, &nTabWidth).cx;
}

int GraphicsMisc::GetFormattedTextWidth(CDC* pDC, LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));

	va_list argList;
	va_start(argList, lpszFormat);

	CString sText;
	sText.FormatV(lpszFormat, argList);

	va_end(argList);

	return pDC->GetTextExtent(sText).cx;
}

float GraphicsMisc::GetAverageCharWidth(HWND hWndRef, HFONT hFont)
{
	CClientDC dc(CWnd::FromHandle(hWndRef));
	HFONT hOldFont = PrepareDCFont(&dc, hWndRef, hFont);

	float fAveWidth = GetAverageCharWidth(&dc);
	dc.SelectObject(hOldFont);

	return fAveWidth;
}

float GraphicsMisc::GetAverageCharWidth(CDC* pDC, CFont* pFont)
{
	ASSERT(pDC);

	CFont* pOld = NULL;
	
	if (pFont)
		pOld = pDC->SelectObject(pFont);
	
	TEXTMETRIC tm = { 0 };
	pDC->GetTextMetrics(&tm);
	
	if (pFont)
		pDC->SelectObject(pOld);
	
	return (float)tm.tmAveCharWidth;
}

int GraphicsMisc::GetAverageStringWidth(const CString& sText, CDC* pDC, CFont* pFont)
{
	if (sText.IsEmpty())
		return 0;

	return (int)(GetAverageCharWidth(pDC, pFont) * sText.GetLength());
}

int GraphicsMisc::GetAverageMaxStringWidth(const CString& sText, CDC* pDC, CFont* pFont)
{
	if (sText.IsEmpty())
		return 0;

	CFont* pOld = NULL;
	
	if (pFont)
		pOld = pDC->SelectObject(pFont);
	
	int nAveWidth = GetAverageStringWidth(sText, pDC);
	int nActualWidth = pDC->GetTextExtent(sText).cx;

	if (pFont)
		pDC->SelectObject(pOld);
	
	return max(nAveWidth, nActualWidth);
}

HFONT GraphicsMisc::PrepareDCFont(CDC* pDC, HWND hwndRef, HFONT hFont, int nStockFont)
{
	if ((hFont == NULL) && (hwndRef != NULL))
		hFont = GetFont(hwndRef);

	if (hFont)
	{
#ifdef _DEBUG
		CString sFont;
		int nPoint = GetFontNameAndPointSize(hFont, sFont);
#endif
		return (HFONT)pDC->SelectObject(hFont);
	}
	
	// else
	return (HFONT)pDC->SelectObject(::GetStockObject(nStockFont));
}

COLORREF GraphicsMisc::GetBestTextColor(COLORREF crBack, BOOL bEnabled)
{
	BYTE bLum = RGBX(crBack).Luminance();

	if (bLum < 128) // darker
	{
		if (bEnabled)
			bLum = 255; // white
		else
			bLum += 96; // lighter
	}
	else // lighter
	{
		if (bEnabled)
			bLum = 0; // black
		else
			bLum -= 96; // darker
	}
	
	return RGB(bLum, bLum, bLum);
}

COLORREF GraphicsMisc::Lighter(COLORREF color, double dAmount, BOOL bRGB)
{
	if (color == CLR_NONE)
		return CLR_NONE;

	RGBX rgbx(color);
	rgbx.AdjustLighting(fabs(dAmount), (bRGB != FALSE));

	return rgbx;
}

COLORREF GraphicsMisc::Darker(COLORREF color, double dAmount, BOOL bRGB)
{
	if (color == CLR_NONE)
		return CLR_NONE;

	RGBX rgbx(color);
	rgbx.AdjustLighting(-fabs(dAmount), (bRGB != FALSE));

	return rgbx;
}

void GraphicsMisc::CalculateBoxColors(COLORREF crBase, BOOL bEnabled, COLORREF& crFill, COLORREF& crBorder)
{
	if (crBase == CLR_NONE)
	{
		crFill = CLR_NONE;
		crBorder = GetSysColor(bEnabled ? COLOR_3DDKSHADOW : COLOR_GRAYTEXT);
	}
	else
	{
		if (!bEnabled)
		{
			// Make colour lighter and grayer
			HLSX hlx(crBase); 

			hlx.fSaturation *= 0.35f;
			hlx.fLuminosity += (1 - hlx.fLuminosity) * 0.25f;

			crFill = hlx;
		}
		else
		{
			crFill = crBase;
		}

		crBorder = GraphicsMisc::Darker(crFill, 0.5);
	}
}

COLORREF GraphicsMisc::Blend(COLORREF color1, COLORREF color2, double dAmount)
{
	if ((dAmount < 0) || (dAmount > 1.0))
	{
		ASSERT(0);
		return CLR_NONE;
	}

	if (color1 == CLR_NONE || color2 == CLR_NONE)
		return CLR_NONE;

	int red1 = GetRValue(color1);
	int green1 = GetGValue(color1);
	int blue1 = GetBValue(color1);
	
	int red2 = GetRValue(color2);
	int green2 = GetGValue(color2);
	int blue2 = GetBValue(color2);
	
	int redBlend = (int)((red1 * dAmount) + (red2 * (1.0 - dAmount)));
	int greenBlend = (int)((green1 * dAmount) + (green2 * (1.0 - dAmount)));
	int blueBlend = (int)((blue1 * dAmount) + (blue2 * (1.0 - dAmount)));

	return RGB(redBlend, greenBlend, blueBlend);
}

void GraphicsMisc::CalculateColorGradient(COLORREF crFrom, COLORREF crTo, int nNumColors, CDWordArray& aColors, BOOL bRGB)
{
	aColors.SetSize(nNumColors);

	// First/Last colours
	aColors[0] = crFrom;
	aColors[nNumColors - 1] = crTo;
	
	// Rest of colours
	if (bRGB)
	{
		BYTE redFrom = GetRValue(crFrom);
		BYTE greenFrom = GetGValue(crFrom);
		BYTE blueLow = GetBValue(crFrom);

		double redRange = (GetRValue(crTo) - redFrom);
		double greenRange = (GetGValue(crTo) - greenFrom);
		double blueRange = (GetBValue(crTo) - blueLow);

		int nRange = (nNumColors - 1);

		for (int nColor = 1; nColor < nRange; nColor++)
		{
			RGBX color(crFrom);
			double dFactor = (nColor / (double)nRange);

			color.Increment((redRange * dFactor),
							(greenRange * dFactor),
							(blueRange * dFactor));

			aColors[nColor] = color;
		}
	}
	else // HLS Gradient
	{
		HLSX hlsLow(crFrom), hlsHigh(crTo);

		float fHueInc = ((hlsHigh.fHue - hlsLow.fHue) / (nNumColors - 1));
		float fSatInc = ((hlsHigh.fSaturation - hlsLow.fSaturation) / (nNumColors - 1));
		float fLumInc = ((hlsHigh.fLuminosity - hlsLow.fLuminosity) / (nNumColors - 1));

		HLSX hls = hlsLow;

		for (int nColor = 1; nColor < (nNumColors - 1); nColor++)
		{
			hls.Increment(fHueInc, fLumInc, fSatInc);

			aColors[nColor] = hls;
		}
	}
}

BOOL GraphicsMisc::ForceIconicRepresentation(HWND hWnd, BOOL bForce)
{
#ifndef DWMWA_FORCE_ICONIC_REPRESENTATION
# define DWMWA_FORCE_ICONIC_REPRESENTATION 7
#endif
	
	return DwmSetWindowAttributeEx(hWnd, DWMWA_FORCE_ICONIC_REPRESENTATION, &bForce, sizeof(bForce));
}

BOOL GraphicsMisc::EnableAeroPeek(HWND hWnd, BOOL bEnable)
{
#ifndef DWMWA_DISALLOW_PEEK
# define DWMWA_DISALLOW_PEEK 11
#endif
	
	BOOL bDisallow = !bEnable;
	
	return DwmSetWindowAttributeEx(hWnd, DWMWA_DISALLOW_PEEK, &bDisallow, sizeof(bDisallow));
}

BOOL GraphicsMisc::GetExtendedFrameBounds(HWND hWnd, CRect& rBounds)
{
#ifndef DWMWA_EXTENDED_FRAME_BOUNDS
# define DWMWA_EXTENDED_FRAME_BOUNDS 9
#endif

	return DwmGetWindowAttributeEx(hWnd, DWMWA_EXTENDED_FRAME_BOUNDS, &rBounds, sizeof(rBounds));
}

BOOL GraphicsMisc::EnableFlip3D(HWND hWnd, BOOL bEnable)
{
#ifndef DWMWA_FLIP3D_POLICY
# define DWMWA_FLIP3D_POLICY 8
# define DWMFLIP3D_DEFAULT      0
# define DWMFLIP3D_EXCLUDEBELOW 1
# define DWMFLIP3D_EXCLUDEABOVE 2
#endif
	
	int nPolicy = bEnable ? DWMFLIP3D_DEFAULT : DWMFLIP3D_EXCLUDEBELOW;
	
	return DwmSetWindowAttributeEx(hWnd, DWMWA_FLIP3D_POLICY, &nPolicy, sizeof(nPolicy));
}

BOOL GraphicsMisc::DwmSetWindowAttributeEx(HWND hWnd, DWORD dwAttrib, LPCVOID pData, DWORD dwDataSize)
{
	HMODULE hMod = ::LoadLibrary(_T("Dwmapi.dll"));
	
	if (hMod)
	{
		typedef HRESULT (WINAPI *PFNDWMSETWINDOWATTRIBUTE)(HWND, DWORD, LPCVOID, DWORD);
		PFNDWMSETWINDOWATTRIBUTE pFn = (PFNDWMSETWINDOWATTRIBUTE)::GetProcAddress(hMod, "DwmSetWindowAttribute");
		
		if (pFn)
		{
			HRESULT hr = pFn(hWnd, dwAttrib, pData, dwDataSize);
			return SUCCEEDED(hr);
		}
	}
	
	return FALSE;
}

BOOL GraphicsMisc::DwmGetWindowAttributeEx(HWND hWnd, DWORD dwAttrib, PVOID pData, DWORD dwDataSize)
{
	HMODULE hMod = ::LoadLibrary(_T("Dwmapi.dll"));

	if (hMod)
	{
		typedef HRESULT (WINAPI *PFNDWMGETWINDOWATTRIBUTE)(HWND, DWORD, PVOID, DWORD);
		PFNDWMGETWINDOWATTRIBUTE pFn = (PFNDWMGETWINDOWATTRIBUTE)::GetProcAddress(hMod, "DwmGetWindowAttribute");

		if (pFn)
		{
			HRESULT hr = pFn(hWnd, dwAttrib, pData, dwDataSize);
			return SUCCEEDED(hr);
		}
	}

	return FALSE;
}

BOOL GraphicsMisc::DwmIsCompositionEnabled()
{
	HMODULE hMod = ::LoadLibrary(_T("Dwmapi.dll"));

	if (hMod)
	{
		typedef HRESULT (WINAPI *PFNDWMISCOMPOSITIONENABLED)(BOOL*);
		PFNDWMISCOMPOSITIONENABLED pFn = (PFNDWMISCOMPOSITIONENABLED)::GetProcAddress(hMod, "DwmIsCompositionEnabled");

		if (pFn)
		{
			BOOL bEnabled = FALSE;
			HRESULT hr = pFn(&bEnabled);

			return (SUCCEEDED(hr) && bEnabled);
		}
	}

	return FALSE;
}

BOOL GraphicsMisc::ChangeWindowMessageFilter(UINT nMessage, BOOL bOn)
{
#ifndef MSGFLT_ADD
#	define MSGFLT_ADD 1
#	define MSGFLT_REMOVE 2
#endif

	HMODULE hMod = ::LoadLibrary(_T("User32.dll"));
	
	if (hMod)
	{
		typedef BOOL (WINAPI *PFNCHANGEWINDOWMESSAGEFILTER)(UINT, DWORD);
		PFNCHANGEWINDOWMESSAGEFILTER pFn = (PFNCHANGEWINDOWMESSAGEFILTER)::GetProcAddress(hMod, "ChangeWindowMessageFilter");
		
		if (pFn)
			return pFn(nMessage, (bOn ? MSGFLT_ADD : MSGFLT_REMOVE));
	}

	// All else
	return FALSE;
}

BOOL GraphicsMisc::EnableNonClientDpiScaling(HWND hWnd)
{
	HMODULE hMod = ::LoadLibrary(_T("User32.dll"));
	
	if (hMod)
	{
		typedef BOOL (WINAPI *PFNENABLENONCLIENTDPISCALING)(HWND);
		PFNENABLENONCLIENTDPISCALING pFn = (PFNENABLENONCLIENTDPISCALING)::GetProcAddress(hMod, "EnableNonClientDpiScaling");
		
		if (pFn)
			return pFn(hWnd);
	}

	// All else
	return FALSE;
}

DPI_AWARENESS_CONTEXT GraphicsMisc::SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT nContext)
{
	HMODULE hMod = ::LoadLibrary(_T("User32.dll"));
	
	if (hMod)
	{
		typedef DPI_AWARENESS_CONTEXT(WINAPI *PFNSETPROCESSDPIAWARENESSCONTEXT)(DPI_AWARENESS_CONTEXT);
		PFNSETPROCESSDPIAWARENESSCONTEXT pFn = (PFNSETPROCESSDPIAWARENESSCONTEXT)::GetProcAddress(hMod, "SetProcessDpiAwarenessContext");
		
		if (pFn)
			return pFn(nContext);
	}

	// All else
	return DPI_AWARENESS_CONTEXT_UNAWARE;
}

int GraphicsMisc::DrawAnsiSymbol(CDC* pDC, char cSymbol, const CRect& rText, UINT nFlags, CFont* pFont)
{
	if (cSymbol == 0)
		return 0;

	CFont* pOldFont = pFont ? pDC->SelectObject(pFont) : NULL;
	pDC->SetBkMode(TRANSPARENT);

	// draw as ANSI string
	char szAnsi[2] = { cSymbol, 0 };
	int nResult = ::DrawTextA(*pDC, szAnsi, 1, (LPRECT)(LPCRECT)rText, nFlags);
	
	if (pFont)
		pDC->SelectObject(pOldFont);

	return nResult;
}

void GraphicsMisc::DrawRect(CDC* pDC, const CRect& rect, COLORREF crFill, COLORREF crBorder, 
							int nCornerRadius, DWORD dwBorders, BYTE cFillOpacity, int nBorderStyle)
{
	ASSERT(HasColor(crBorder) || HasColor(crFill));
	ASSERT((cFillOpacity == 255) || (nCornerRadius == 0));

 	if (rect.IsRectEmpty())
	{
		// draw a line
		if (rect.Width() > 0)
		{
			DrawHorzLine(pDC, rect.left, rect.right, rect.top, (HasColor(crBorder) ? crBorder : crFill));
		}
		else if (rect.Height() > 0)
		{
			DrawVertLine(pDC, rect.top, rect.bottom, rect.left, (HasColor(crBorder) ? crBorder : crFill));
		}

 		return;
	}

	if (nCornerRadius == 0)
	{
		// can't have border color and no edges
		if (dwBorders == GMDR_NONE)
			crBorder = CLR_NONE;

		// if both colours are set, the fill is opaque and the border is not dotted
		// there's an optimisation we can do
		if (HasColor(crFill) && HasColor(crBorder) && (cFillOpacity == 255) && (nBorderStyle == PS_SOLID))
		{
			pDC->FillSolidRect(rect, crBorder);

			if (crFill != crBorder)
			{
				CRect rFill(rect);

				if ((dwBorders & GMDR_ALL) == GMDR_ALL)
				{
					rFill.DeflateRect(1, 1);
				}
				else
				{
					if (dwBorders & GMDR_LEFT)
						rFill.left++;

					if (dwBorders & GMDR_TOP)
						rFill.top++;

					if (dwBorders & GMDR_RIGHT)
						rFill.right--;

					if (dwBorders & GMDR_BOTTOM)
						rFill.bottom--;
				}

				if (!rFill.IsRectEmpty())
					pDC->FillSolidRect(rFill, crFill);
			}

			return;
		}

		// else
		if (HasColor(crFill)) // inside of rect
		{
			if (cFillOpacity != 255)
				CGdiPlus::FillRect(CGdiPlusGraphics(*pDC, gdix_SmoothingModeNone), CGdiPlusBrush(crFill, cFillOpacity), rect);
			else
				pDC->FillSolidRect(rect, crFill);
		}

		if (HasColor(crBorder)) // border
		{
			if (nBorderStyle == PS_SOLID)
			{
				if (dwBorders & GMDR_TOP)
					pDC->FillSolidRect(rect.left, rect.top, rect.Width(), 1, crBorder);

				if (dwBorders & GMDR_BOTTOM)
					pDC->FillSolidRect(rect.left, rect.bottom - 1, rect.Width(), 1, crBorder);

				if (dwBorders & GMDR_LEFT)
					pDC->FillSolidRect(rect.left, rect.top, 1, rect.Height(), crBorder);

				if (dwBorders & GMDR_RIGHT)
					pDC->FillSolidRect(rect.right - 1, rect.top, 1, rect.Height(), crBorder);
			}
			else
			{
				CPen pen;
				VERIFY(pen.CreatePen(nBorderStyle, 1, crBorder));
				
				CPen* pOldPen = pDC->SelectObject(&pen);

				if (dwBorders & GMDR_TOP)
				{
					pDC->MoveTo(rect.left, rect.top);
					pDC->LineTo(rect.right, rect.top);
				}

				if (dwBorders & GMDR_BOTTOM)
				{
					pDC->MoveTo(rect.left, rect.bottom - 1);
					pDC->LineTo(rect.right, rect.bottom - 1);
				}

				if (dwBorders & GMDR_LEFT)
				{
					pDC->MoveTo(rect.left, rect.top);
					pDC->LineTo(rect.left, rect.bottom - 1);
				}

				if (dwBorders & GMDR_RIGHT)
				{
					pDC->MoveTo(rect.right, rect.top);
					pDC->LineTo(rect.right, rect.bottom - 1);
				}

				pDC->SelectObject(pOldPen);
			}
		}
	}
	else // round-rect or dotted pen
	{
		CPen* pOldPen = NULL, penBorder;
		CBrush* pOldBrush = NULL, brFill;

		// inside of rect
		if (HasColor(crFill))
		{
			brFill.CreateSolidBrush(crFill);
			pOldBrush = pDC->SelectObject(&brFill);
		}
		else
		{
			pOldBrush = (CBrush* )pDC->SelectStockObject(NULL_BRUSH);
		}

		// border
		if (HasColor(crBorder))
		{
			penBorder.CreatePen(nBorderStyle, 1, crBorder);
			pOldPen = pDC->SelectObject(&penBorder);
		}
		else
		{
			pOldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);
		}

		pDC->RoundRect(rect, CPoint(nCornerRadius, nCornerRadius));

		// cleanup
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}
}

BOOL GraphicsMisc::FillItemRect(CDC* pDC, CRect& rItem, COLORREF color, HWND hwnd)
{
	if (color == CLR_NONE)
		return FALSE;

	COLORREF crOldBk = pDC->GetBkColor();

	if (::IsWindow(hwnd))
	{
		CRect rClient;
		::GetClientRect(hwnd, rClient);
		
		rItem.left = 0;
		rItem.right = (rClient.right + GetSystemMetrics(SM_CXVSCROLL));
	}

	pDC->FillSolidRect(rItem, color);
	pDC->SetBkColor(crOldBk);

	return TRUE;
}

BOOL GraphicsMisc::FillItemRect(CDC* pDC, LPCRECT prcItem, COLORREF color, HWND hwnd)
{
	CRect rItem(prcItem);
	return FillItemRect(pDC, rItem, color, NULL);
}

CPoint GraphicsMisc::CentrePoint(LPCRECT prcRect)
{
	return CPoint(((prcRect->left + prcRect->right) / 2), 
				  ((prcRect->top + prcRect->bottom) / 2));
}

void GraphicsMisc::CentreRect(LPRECT pRect, LPCRECT prcOther, BOOL bCentreHorz, BOOL bCentreVert)
{
	ASSERT(bCentreHorz || bCentreVert);

	if (bCentreHorz)
	{
		int nXOffset = ((prcOther->left + prcOther->right) - (pRect->left + pRect->right)) / 2;
		::OffsetRect(pRect, nXOffset, 0);
	}
		
	if (bCentreVert)
	{
		int nYOffset = ((prcOther->top + prcOther->bottom) - (pRect->top + pRect->bottom)) / 2;
		::OffsetRect(pRect, 0, nYOffset);
	}
}

CRect GraphicsMisc::CalcCentredRect(int nSize, LPCRECT prcOther, BOOL bCentreHorz, BOOL bCentreVert)
{
	return CalcCentredRect(nSize, nSize, prcOther, bCentreHorz, bCentreVert);
}

CRect GraphicsMisc::CalcCentredRect(int cx, int cy, LPCRECT prcOther, BOOL bCentreHorz, BOOL bCentreVert)
{
	CRect rect(prcOther);

	rect.right = (rect.left + cx);
	rect.bottom = (rect.top + cy);

	CentreRect(rect, prcOther, bCentreHorz, bCentreVert);

	return rect;
}

void GraphicsMisc::AlignRect(LPRECT pRect, LPCRECT prcOther, int nDrawTextFlags)
{
	CPoint ptOtherRef = CentrePoint(prcOther);

	int nWidth = (pRect->right - pRect->left);
	int nHeight= (pRect->bottom - pRect->top);
	
	// Vertical
	if (Misc::HasFlag(nDrawTextFlags, DT_BOTTOM))
	{
		ptOtherRef.y = (prcOther->bottom - (nHeight / 2));
	}
	else if (Misc::HasFlag(nDrawTextFlags, DT_VCENTER))
	{
		 // done
	}
	else // DT_TOP
	{
		ptOtherRef.y = (prcOther->top + (nHeight / 2));
	}
	
	// Horizontal
	if (Misc::HasFlag(nDrawTextFlags, DT_RIGHT))
	{
		ptOtherRef.x = (prcOther->right - (nWidth / 2));
	}
	else if (Misc::HasFlag(nDrawTextFlags, DT_CENTER))
	{
		 // done
	}
	else // DT_LEFT
	{
		ptOtherRef.x = (prcOther->left + (nWidth / 2));
	}

	CPoint ptRef = CentrePoint(pRect);
	::OffsetRect(pRect, (ptOtherRef.x - ptRef.x), (ptOtherRef.y - ptRef.y));
}

BOOL GraphicsMisc::DrawCentred(CDC* pDC, HIMAGELIST hIl, int nImage, LPCRECT prcImage, BOOL bCentreHorz, BOOL bCentreVert, UINT nStyle)
{
	ASSERT(bCentreHorz || bCentreVert);

	if (nImage == -1)
		return FALSE;

	int cxIcon, cyIcon;
	VERIFY(CEnImageList::GetImageSize(hIl, cxIcon, cyIcon));

	CRect rImage = CalcCentredRect(cxIcon, cyIcon, prcImage, bCentreHorz, bCentreVert);

	return ImageList_Draw(hIl, nImage, *pDC, rImage.left, rImage.top, nStyle);
}

BOOL GraphicsMisc::DrawCentred(CDC* pDC, HICON hIcon, LPCRECT prcIcon, BOOL bCentreHorz, BOOL bCentreVert)
{
	ASSERT(bCentreHorz || bCentreVert);

	if (!hIcon)
		return FALSE;

	CSize sizeIcon = GetIconSize(hIcon);
	CRect rIcon = CalcCentredRect(sizeIcon.cx, sizeIcon.cy, prcIcon, bCentreHorz, bCentreVert);

	return ::DrawIconEx(*pDC, rIcon.left, rIcon.top, hIcon, sizeIcon.cx, sizeIcon.cy, 0, NULL, DI_NORMAL);
}

COLORREF GraphicsMisc::GetExplorerItemSelectionTextColor(COLORREF crBase, GM_ITEMSTATE nState, DWORD dwFlags)
{
	if (nState != GMIS_NONE)
	{
		BOOL bHighContrast = Misc::IsHighContrastActive();
		BOOL bThemed = (!bHighContrast && CThemed::AreControlsThemed() && (COSVersion() >= OSV_VISTA));

		if ((crBase == CLR_NONE) || (crBase == GetSysColor(COLOR_WINDOWTEXT)))
			crBase = 0;
		
		if (bHighContrast)
		{
			switch (nState)
			{
			case GMIS_SELECTED:
			case GMIS_SELECTEDNOTFOCUSED:
			case GMIS_DROPHILITED:
				return GetSysColor(COLOR_HIGHLIGHTTEXT);
				
			default:
				ASSERT(0);
			}
		}
		else if (bThemed || (dwFlags & GMIB_THEMECLASSIC))
		{
			switch (nState)
			{
			case GMIS_SELECTED:
			case GMIS_SELECTEDNOTFOCUSED:
			case GMIS_DROPHILITED:
				{
					// darken the base color to have a luminance <= 30%
					HLSX hlsText(crBase);
					hlsText.fLuminosity = min(hlsText.fLuminosity, 0.3f);

					return hlsText;
				}
				break;

			default:
				ASSERT(0);
			}
		}
		else // XP/Unthemed
		{
			switch (nState)
			{
			case GMIS_SELECTED:
				return GetSysColor(COLOR_HIGHLIGHTTEXT);
				
			case GMIS_SELECTEDNOTFOCUSED:
			case GMIS_DROPHILITED:
				return GetSysColor(COLOR_WINDOWTEXT);
				
			default:
				ASSERT(0);
			}
		}
	}

	return crBase;
}

BOOL GraphicsMisc::DrawExplorerItemSelection(CDC* pDC, HWND hwnd, GM_ITEMSTATE nState, const CRect& rItem, DWORD dwFlags, LPCRECT prClip)
{
	if (nState == GMIS_NONE)
		return FALSE;

	int nOSVer = COSVersion();

	BOOL bHighContrast = Misc::IsHighContrastActive();
	BOOL bThemed = (!bHighContrast && CThemed::AreControlsThemed() && (nOSVer >= OSV_VISTA));

	if (hwnd == NULL)
	{
		hwnd = ::WindowFromDC(*pDC);
		
		// Certain options are not doable without a valid HWND
		if (hwnd == NULL)
		{
			if (!prClip && (dwFlags & (GMIB_EXTENDLEFT | GMIB_EXTENDRIGHT | GMIB_CLIPLEFT | GMIB_CLIPRIGHT)))
				return FALSE;

			if (bThemed)
				return FALSE;
		}
	}

	// Adjust drawing rect/flags accordingly
	CRect rDraw(rItem), rClip(prClip);
	DWORD dwClassicBorders = GMDR_ALL;

	if (dwFlags & (GMIB_EXTENDLEFT | GMIB_EXTENDRIGHT | GMIB_CLIPLEFT | GMIB_CLIPRIGHT))
	{
		if (!prClip)
			::GetClientRect(hwnd, rClip);

		if (dwFlags & GMIB_EXTENDLEFT)
		{
			rDraw.left = rClip.left;
		}
		
		if (dwFlags & GMIB_EXTENDRIGHT)
		{
			rDraw.right = rClip.right;
		}
		
		if (dwFlags & GMIB_CLIPLEFT)
		{
			if (bThemed)
			{
				rDraw.left = (rClip.left - 2);
			}
			else // classic
			{
				dwClassicBorders &= ~GMDR_LEFT;
			}
		}
		
		if (dwFlags & GMIB_CLIPRIGHT)
		{
			if (bThemed)
			{
				rDraw.right = (rClip.right + 2);
			}
			else // classic
			{
				dwClassicBorders &= ~GMDR_RIGHT;
			}
		}

		prClip = rClip;
	}
	
	// Do the draw
	BOOL bPreDraw = ((dwFlags & GMIB_PREDRAW)), bPostDraw = ((dwFlags & GMIB_POSTDRAW));
	BOOL bSingleStageDraw = ((!bPreDraw && !bPostDraw) || CDarkMode::IsEnabled());
	BOOL bDrawn = FALSE;

	// Fill background with white if single stage draw
	if (bSingleStageDraw && !bHighContrast)
	{
		CRect rBkgnd;

		if (prClip)
			rBkgnd.IntersectRect(prClip, rDraw);
		else
			rBkgnd = rDraw;

		// Themed Windows 7 and Vista require a round-rect
		if (bThemed && (nOSVer < OSV_WIN8))
			GraphicsMisc::DrawRect(pDC, rBkgnd, colorWhite, CLR_NONE, 1);
		else
			pDC->FillSolidRect(rBkgnd, colorWhite);

		bDrawn = TRUE;
	}

	if (bHighContrast)
	{
		if (bSingleStageDraw || bPreDraw)
		{
			switch (nState)
			{
			case GMIS_SELECTED:
			case GMIS_SELECTEDNOTFOCUSED:
			case GMIS_DROPHILITED:
				pDC->FillSolidRect(rDraw, HIGHCONTRAST_SEL_BKCOLOR);
				break;

			default:
				ASSERT(0);
				return FALSE;
			}
			bDrawn = TRUE;
		}
	}
	else if (bThemed)
	{
		BOOL bWantDraw = bSingleStageDraw;

		// Windows 7 selection is too opaque to draw over text so we draw it under
		if (nOSVer < OSV_WIN8)
			bWantDraw |= bPreDraw;
		else
			bWantDraw |= bPostDraw;

		if (bWantDraw)
		{
			CThemed th(hwnd, _T("Explorer::ListView"));

			switch (nState)
			{
			case GMIS_SELECTED:
				// Draw twice to intensify the colour
				th.DrawBackground(pDC, LVP_LISTITEM, LIS_MORESELECTED, rDraw, prClip);
				th.DrawBackground(pDC, LVP_LISTITEM, LIS_MORESELECTED, rDraw, prClip);
				break;

			case GMIS_SELECTEDNOTFOCUSED:
			case GMIS_DROPHILITED:
				th.DrawBackground(pDC, LVP_LISTITEM, LIS_MORESELECTED, rDraw, prClip);
				break;
			
			default:
				ASSERT(0);
				return FALSE;
			}
			bDrawn = TRUE;
		}
	}
	// Can be XP, Linux or Classic theme, or themes disabled for this app only
	else if (dwFlags & GMIB_THEMECLASSIC)
	{
		if (bSingleStageDraw || bPostDraw)
		{
			// draw a bordered box based on standard colours
			COLORREF crBorder = CLR_NONE, crFill = CLR_NONE;
			BOOL bTransparent = bPostDraw;

			switch (nState)
			{
			case GMIS_SELECTED:
				// Similar to windows 10 colours
				crBorder = CLASSICTHEME_SEL_BORDERCOLOR;
				crFill = (bTransparent ? CLASSICTHEME_SEL_BORDERCOLOR : CLASSICTHEME_SEL_BKCOLOR);
				break;
			
			case GMIS_SELECTEDNOTFOCUSED:
			case GMIS_DROPHILITED:
				crBorder = CLASSICTHEME_SELNOFOCUS_BORDERCOLOR;
				crFill = (bTransparent ? CLASSICTHEME_SEL_BORDERCOLOR : CLASSICTHEME_SELNOFOCUS_BKCOLOR);
				break;
			
			default:
				ASSERT(0);
				return FALSE;
			}
		
			DrawRect(pDC, rDraw, crFill, crBorder, 0, dwClassicBorders, (bTransparent ? 128 : 255));
			bDrawn = TRUE;
		}
	}
	else // unthemed classic 
	{
		if (bSingleStageDraw || bPreDraw)
		{
			switch (nState)
			{
			case GMIS_SELECTED:
				pDC->FillSolidRect(rDraw, NOTHEME_SEL_BKCOLOR);
				break;
			
			case GMIS_SELECTEDNOTFOCUSED:
			case GMIS_DROPHILITED:
				pDC->FillSolidRect(rDraw, NOTHEME_SELNOFOCUS_BKCOLOR);
				break;
			
			default:
				ASSERT(0);
				return FALSE;
			}
			bDrawn = TRUE;
		}
	}

	return bDrawn;
}

COLORREF GraphicsMisc::GetExplorerItemSelectionBackColor(GM_ITEMSTATE nState, DWORD dwFlags)
{
	ASSERT(nState != GMIS_NONE);

	if (Misc::IsHighContrastActive())
	{
		return HIGHCONTRAST_SEL_BKCOLOR;
	}
	else if (CThemed::AreControlsThemed() && (COSVersion() >= OSV_VISTA))
	{
		switch (nState)
		{
		case GMIS_SELECTED:
			return THEME_SEL_BKCOLOR;

		case GMIS_SELECTEDNOTFOCUSED:
		case GMIS_DROPHILITED:
			return THEME_SELNOFOCUS_BKCOLOR;
		}
	}
	else if (dwFlags & GMIB_THEMECLASSIC)
	{
		switch (nState)
		{
		case GMIS_SELECTED:
			return CLASSICTHEME_SEL_BKCOLOR;

		case GMIS_SELECTEDNOTFOCUSED:
		case GMIS_DROPHILITED:
			return CLASSICTHEME_SELNOFOCUS_BKCOLOR;
		}
	}
	else
	{
		switch (nState)
		{
		case GMIS_SELECTED:
			return NOTHEME_SEL_BKCOLOR;

		case GMIS_SELECTEDNOTFOCUSED:
		case GMIS_DROPHILITED:
			return NOTHEME_SELNOFOCUS_BKCOLOR;
		}
	}

	return GetSysColor(COLOR_WINDOW);
}

COLORREF GraphicsMisc::GetExplorerItemSelectionBorderColor(GM_ITEMSTATE nState, DWORD dwFlags)
{
	ASSERT(nState != GMIS_NONE);

	if (Misc::IsHighContrastActive())
	{
		return HIGHCONTRAST_SEL_BKCOLOR;
	}
	else if (CThemed::AreControlsThemed() && (COSVersion() >= OSV_VISTA))
	{
		switch (nState)
		{
		case GMIS_SELECTED:
			return THEME_SEL_BORDERCOLOR;

		case GMIS_SELECTEDNOTFOCUSED:
		case GMIS_DROPHILITED:
			return THEME_SELNOFOCUS_BORDERCOLOR;
		}
	}
	else if (dwFlags & GMIB_THEMECLASSIC)
	{
		switch (nState)
		{
		case GMIS_SELECTED:
			return CLASSICTHEME_SEL_BORDERCOLOR;

		case GMIS_SELECTEDNOTFOCUSED:
		case GMIS_DROPHILITED:
			return CLASSICTHEME_SELNOFOCUS_BORDERCOLOR;
		}
	}
	else
	{
		switch (nState)
		{
		case GMIS_SELECTED:
			return NOTHEME_SEL_BKCOLOR;

		case GMIS_SELECTEDNOTFOCUSED:
		case GMIS_DROPHILITED:
			return NOTHEME_SELNOFOCUS_BKCOLOR;
		}
	}

	return GetSysColor(COLOR_WINDOW);
}

BOOL GraphicsMisc::FitRect(CRect& rect, const CRect& rOther)
{
	int nXOffset = 0, nYOffset = 0;

	if (rect.left < rOther.left)
	{
		nXOffset = (rOther.left - rect.left);
	}
	else if (rect.right > rOther.right)
	{
		nXOffset = (rOther.right - rect.right);
	}

	if (rect.top < rOther.top)
	{
		nYOffset = (rOther.top - rect.top);
	}
	else if (rect.bottom > rOther.bottom)
	{
		nYOffset = (rOther.bottom - rect.bottom);
	}

	if (!nXOffset && !nYOffset)
		return FALSE;

	rect.OffsetRect(nXOffset, nYOffset);
	return TRUE;
}

BOOL GraphicsMisc::FitRectToWindow(CRect& rect, HWND hWnd, BOOL bScreen)
{
	CRect rWnd;

	if (bScreen)
		::GetWindowRect(hWnd, rWnd);
	else
		::GetClientRect(hWnd, rWnd);

	return FitRect(rect, rWnd);
}

BOOL GraphicsMisc::FitRectToScreen(CRect& rect, LPPOINT pPtRef, UINT nFallback)
{
	CRect rScreen;

	if (pPtRef)
		GetAvailableScreenSpace(*pPtRef, rScreen, nFallback);
	else
		GetAvailableScreenSpace(rect, rScreen, nFallback);

	return FitRect(rect, rScreen);
}

BOOL GraphicsMisc::GetMonitorAvailableScreenSpace(HMONITOR hMon, CRect& rScreen, UINT nFallback)
{
	if (hMon && GetMonitorAvailableScreenSpace(hMon, rScreen))
		return TRUE;

	// else
	if (nFallback == MONITOR_DEFAULTTOPRIMARY)
		return GetPrimaryMonitorScreenSpace(rScreen);

	rScreen.SetRectEmpty();
	return FALSE;
}

BOOL GraphicsMisc::GetAvailableScreenSpace(const CPoint& point, CRect& rScreen, UINT nFallback)
{
	HMONITOR hMon = MonitorFromPoint(point, nFallback);

	return GetMonitorAvailableScreenSpace(hMon, rScreen, nFallback);
}

BOOL GraphicsMisc::GetAvailableScreenSpace(const CRect& rWnd, CRect& rScreen, UINT nFallback)
{
	HMONITOR hMon = MonitorFromRect(rWnd, nFallback);

	return GetMonitorAvailableScreenSpace(hMon, rScreen, nFallback);
}

BOOL GraphicsMisc::GetAvailableScreenSpace(HWND hWnd, CRect& rScreen, UINT nFallback)
{
	if (hWnd == NULL)
		return GetMonitorAvailableScreenSpace(NULL, rScreen, nFallback);

	// else
	CRect rWnd;
	::GetWindowRect(hWnd, rWnd);

	return GetAvailableScreenSpace(rWnd, rScreen, nFallback);
}

BOOL GraphicsMisc::GetPrimaryMonitorScreenSpace(CRect& rScreen)
{
	HMONITOR hMon = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);
	ASSERT(hMon);

	return GetMonitorAvailableScreenSpace(hMon, rScreen);
}

BOOL GraphicsMisc::GetMonitorAvailableScreenSpace(HMONITOR hMon, CRect& rScreen)
{
	MONITORINFO mi = { sizeof(MONITORINFO), 0 };
	
	if (GetMonitorInfo(hMon, &mi))
	{
		rScreen = mi.rcWork;
		return TRUE;
	}

	// else
	ASSERT(0);
	rScreen.SetRectEmpty();

	return FALSE;
}

BOOL GraphicsMisc::GetTotalAvailableScreenSpace(CRect& rScreen)
{
	rScreen.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
	rScreen.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
	rScreen.right = (rScreen.left + GetSystemMetrics(SM_CXVIRTUALSCREEN));
	rScreen.bottom = (rScreen.top + GetSystemMetrics(SM_CYVIRTUALSCREEN));

	return !rScreen.IsRectEmpty();
}

void GraphicsMisc::DrawHorzLine(CDC* pDC, int nXFrom, int nXTo, int nYPos, COLORREF crFrom, COLORREF crTo)
{
	CRect rLine(nXFrom, nYPos, nXTo, nYPos + 1);

	if ((crFrom == crTo) || (crTo == CLR_NONE))
		pDC->FillSolidRect(rLine, crFrom);
	else
		DrawGradient(pDC, rLine, crFrom, crTo, TRUE, -1);
}

void GraphicsMisc::DrawVertLine(CDC* pDC, int nYFrom, int nYTo, int nXPos, COLORREF crFrom, COLORREF crTo)
{
	CRect rLine(nXPos, nYFrom, nXPos + 1, nYTo);
	
	if ((crFrom == crTo) || (crTo == CLR_NONE))
		pDC->FillSolidRect(rLine, crFrom);
	else
		DrawGradient(pDC, rLine, crFrom, crTo, FALSE, -1);
}

CString GraphicsMisc::GetWebColor(COLORREF color)
{
	unsigned char cRed = GetRValue(color);
	unsigned char cGreen = GetGValue(color);
	unsigned char cBlue = GetBValue(color);
	
	CString sColor;
	sColor.Format(_T("#%02X%02X%02X"), cRed, cGreen, cBlue);
	
	return sColor;
}

COLORREF GraphicsMisc::ParseWebColor(const CString& sHexColor)
{
	COLORREF color = CLR_NONE;
	UINT wRed = 0, wBlue = 0, wGreen = 0;

	switch (sHexColor.GetLength())
	{
	case 7:
		if (_stscanf(sHexColor, _T("#%02X%02X%02X"), &wRed, &wGreen, &wBlue) == 3)
			color = RGB(wRed, wGreen, wBlue);
		break;

	case 6:
		if (_stscanf(sHexColor, _T("%02X%02X%02X"), &wRed, &wGreen, &wBlue) == 3)
			color = RGB(wRed, wGreen, wBlue);
		break;
	}

	return color;
}

CSize GraphicsMisc::GetIconSize(HICON hIcon)
{
	ICONINFO iconinfo = { 0 };
	::GetIconInfo(hIcon, &iconinfo);

	CSize sIcon = GetBitmapSize(iconinfo.hbmColor);
		
	// cleanup
	GraphicsMisc::VerifyDeleteObject(iconinfo.hbmColor);
	GraphicsMisc::VerifyDeleteObject(iconinfo.hbmMask);

	return sIcon;
}

HICON GraphicsMisc::GetAppWindowIcon(BOOL bLarge)
{
	return (HICON)AfxGetMainWnd()->SendMessage(WM_GETICON, (bLarge ? ICON_BIG : ICON_SMALL));
}

CSize GraphicsMisc::GetBitmapSize(HBITMAP hBmp)
{
	BITMAP bitmap = { 0 };
	::GetObject(hBmp, sizeof(bitmap), &bitmap);

	return CSize(bitmap.bmWidth, bitmap.bmHeight);
}

HBITMAP GraphicsMisc::MakeWizardImage(HICON hIcon)
{
	// The wizard demands an image of size 49x49 which is a bit of a nuisance
	CClientDC dcDesktop(CWnd::GetDesktopWindow());
	ASSERT_VALID(&dcDesktop);

	CDC dcMem;

	if (!dcMem.CreateCompatibleDC(&dcDesktop))
		return NULL;

	CBitmap bmMem;

	if (!bmMem.CreateCompatibleBitmap(&dcDesktop, 49, 49))
		return NULL;

	CBitmap* pOldBM = dcMem.SelectObject(&bmMem);

	dcMem.FillSolidRect(0, 0, 49, 49, GetSysColor(COLOR_WINDOW));
	::DrawIconEx(dcMem, 0, 0, hIcon, 48, 48, 0, NULL, DI_NORMAL);
	dcMem.SelectObject(pOldBM);

	return (HBITMAP)bmMem.Detach();
}

///////////////////////////////////////////////////////////////////////////////////////
// private helper for IconToPARGB32Bitmap
void InitBitmapInfo(BITMAPINFO *pbmi, ULONG cbInfo, LONG cx, LONG cy, WORD bpp)
{
    ZeroMemory(pbmi, cbInfo);

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biPlanes = 1;
    pbmi->bmiHeader.biCompression = BI_RGB;
	
    pbmi->bmiHeader.biWidth = cx;
    pbmi->bmiHeader.biHeight = cy;
    pbmi->bmiHeader.biBitCount = bpp;
}

///////////////////////////////////////////////////////////////////////////////////////
// private helper for IconToPARGB32Bitmap
BOOL Create32BitHBITMAP(HDC hdc, const SIZE *psize, void **ppvBits, HBITMAP* phBmp)
{
    *phBmp = NULL;
	
    BITMAPINFO bmi = { 0 };
    InitBitmapInfo(&bmi, sizeof(bmi), psize->cx, psize->cy, 32);
	
	HDC hdcUsed = (hdc ? hdc : ::GetDC(NULL));

    if (hdcUsed)
    {
        *phBmp = CreateDIBSection(hdcUsed, &bmi, DIB_RGB_COLORS, ppvBits, NULL, 0);

        if (hdc != hdcUsed)
        {
			::ReleaseDC(NULL, hdcUsed);
        }
    }
	
    return (NULL != *phBmp);
}

///////////////////////////////////////////////////////////////////////////////////////
// private helper for IconToPARGB32Bitmap
BOOL HasAlpha(ARGB *pargb, SIZE& sizImage, int cxRow)
{
    ULONG cxDelta = (cxRow - sizImage.cx);

    for (ULONG y = sizImage.cy; y; --y)
    {
        for (ULONG x = sizImage.cx; x; --x)
        {
            if (*pargb++ & 0xFF000000)
            {
                return TRUE;
            }
        }
		
        pargb += cxDelta;
    }
	
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////
// private helper for IconToPARGB32Bitmap
BOOL ConvertToPARGB32(HDC hdc, ARGB *pargb, HBITMAP hbmp, SIZE& sizImage, int cxRow)
{
    BOOL bSuccess = FALSE;

    BITMAPINFO bmi =  { 0 };
    InitBitmapInfo(&bmi, sizeof(bmi), sizImage.cx, sizImage.cy, 32);
	
    HANDLE hHeap = GetProcessHeap();
    void *pvBits = HeapAlloc(hHeap, 0, bmi.bmiHeader.biWidth * 4 * bmi.bmiHeader.biHeight);

    if (pvBits)
    {
        if (GetDIBits(hdc, hbmp, 0, bmi.bmiHeader.biHeight, pvBits, &bmi, DIB_RGB_COLORS) == bmi.bmiHeader.biHeight)
        {
            ULONG cxDelta = (cxRow - bmi.bmiHeader.biWidth);
            ARGB *pargbMask = static_cast<ARGB *>(pvBits);
			
            for (ULONG y = bmi.bmiHeader.biHeight; y; --y)
            {
                for (ULONG x = bmi.bmiHeader.biWidth; x; --x)
                {
                    if (*pargbMask++)
                    {
                        // transparent pixel
                        *pargb++ = 0;
                    }
                    else
                    {
                        // opaque pixel
                        *pargb++ |= 0xFF000000;
                    }
                }
				
                pargb += cxDelta;
            }
			
            bSuccess = TRUE;
        }
		
        HeapFree(hHeap, 0, pvBits);
    }
	
    return bSuccess;
}

///////////////////////////////////////////////////////////////////////////////////////
// private helper for IconToPARGB32Bitmap
BOOL ConvertBufferToPARGB32(HPAINTBUFFER hPaintBuffer, HDC hdc, HICON hicon, SIZE& sizIcon)
{
    RGBQUAD *prgbQuad = NULL;
    int cxRow = 0;

	if (CThemed::GetBufferedPaintBits(hPaintBuffer, &prgbQuad, &cxRow))
	{
		ARGB *pargb = reinterpret_cast<ARGB *>(prgbQuad);

		if (!HasAlpha(pargb, sizIcon, cxRow))
		{
			ICONINFO info = { 0 };

			if (GetIconInfo(hicon, &info))
			{
				if (info.hbmMask)
				{
					ConvertToPARGB32(hdc, pargb, info.hbmMask, sizIcon, cxRow);
				}

				// cleanup
				VERIFY(DeleteObject(info.hbmColor));
				VERIFY(DeleteObject(info.hbmMask));
			}
		}
		
		return TRUE;
	}
	
    return FALSE;
}

HBITMAP GraphicsMisc::IconToPARGB32Bitmap(HICON hIcon)
{
    HBITMAP hBmp = NULL;

	if (hIcon)
	{
		HDC hdcDest = CreateCompatibleDC(NULL);
		BOOL bSuccess = FALSE;

		if (hdcDest) 
		{
			CSize sIcon = GetIconSize(hIcon);
			
			if (Create32BitHBITMAP(hdcDest, &sIcon, NULL, &hBmp))
			{
				HBITMAP hbmpOld = (HBITMAP)SelectObject(hdcDest, hBmp);

				if (hbmpOld) 
				{
					BLENDFUNCTION bfAlpha = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

					TH_PAINTPARAMS paintParams = { 0 };
					paintParams.cbSize = sizeof(paintParams);
					paintParams.dwFlags = BPPF_ERASE;
					paintParams.pBlendFunction = &bfAlpha;
					
					HDC hdcBuffer = NULL;
					CRect rIcon(0, 0, sIcon.cx, sIcon.cy);
					HPAINTBUFFER hPaintBuffer = CThemed::BeginBufferedPaint(hdcDest, rIcon, THBPBF_DIB, &paintParams, &hdcBuffer);

					if (hPaintBuffer) 
					{
						ASSERT(hdcBuffer);

						if (DrawIconEx(hdcBuffer, 0, 0, hIcon, sIcon.cx, sIcon.cy, 0, NULL, DI_NORMAL)) 
						{
							// If icon did not have an alpha channel, we need to convert buffer to PARGB.
							bSuccess = ConvertBufferToPARGB32(hPaintBuffer, hdcDest, hIcon, sIcon);
						}
						
						// This will write the buffer contents to the destination bitmap.
						CThemed::EndBufferedPaint(hPaintBuffer, TRUE);
					}
					
					SelectObject(hdcDest, hbmpOld);
				}
			}
			
			VERIFY(DeleteDC(hdcDest));
		}
		
		if (!bSuccess && hBmp)
		{
			VERIFY(DeleteObject(hBmp));
			hBmp = NULL;
		}
	}
	
    return hBmp;
}

BOOL GraphicsMisc::FlashWindowEx(HWND hWnd, DWORD dwFlags, UINT uCount, DWORD dwTimeout)
{
	ASSERT(::IsWindow(hWnd));

	HMODULE hMod = ::LoadLibrary(_T("User32.dll"));
	
	if (hMod)
	{
		typedef BOOL (WINAPI *PFNFLASHWINDOWEX)(GMFLASHWINFO*);
		PFNFLASHWINDOWEX pFn = (PFNFLASHWINDOWEX)::GetProcAddress(hMod, "FlashWindowEx");
		
		if (pFn)
		{
			GMFLASHWINFO fwi = { sizeof(fwi), 0 };
			fwi.hwnd = hWnd;
			fwi.dwFlags = dwFlags;
			fwi.uCount = uCount;
			fwi.dwTimeout = dwTimeout;
			
			return pFn(&fwi);
		}
	}

	// else
	return FALSE;
}

BOOL GraphicsMisc::InitCheckboxImageList(HWND hWnd, CImageList& ilCheckboxes, UINT nFallbackBmpID, COLORREF crBkgnd, LPCRECT prPadding)
{
	if (ilCheckboxes.GetSafeHandle())
		return TRUE;

	const int nStates[] = { -1, CBS_UNCHECKEDNORMAL, CBS_CHECKEDNORMAL, CBS_MIXEDNORMAL };
	const int nNumStates = sizeof(nStates) / sizeof(int);

	CThemed th;

	if (th.Open(hWnd, _T("BUTTON")) && th.AreControlsThemed())
	{
		th.BuildImageList(ilCheckboxes, BP_CHECKBOX, nStates, nNumStates, crBkgnd, prPadding);
	}

	// unthemed + fallback
	if (!ilCheckboxes.GetSafeHandle() && nFallbackBmpID)
	{
		CBitmap bitmap;
		
		if (bitmap.LoadBitmap(nFallbackBmpID))
		{
			BITMAP BM;
			bitmap.GetBitmap(&BM);

			if (ilCheckboxes.Create(BM.bmWidth / nNumStates, BM.bmHeight, ILC_COLOR32 | ILC_MASK, 0, 1) &&
				ilCheckboxes.Add(&bitmap, crBkgnd) == 0)
			{
				CEnImageList::ScaleByDPIFactor(ilCheckboxes, crBkgnd);
			}
		}
	}

	return (NULL != ilCheckboxes.GetSafeHandle());
}

int GraphicsMisc::GetSystemDPI()
{
	return PixelsPerInch();
}

double GraphicsMisc::GetDPIScaleFactor()
{
	return ((double)GetSystemDPI() / DEFAULT_DPI);
}

int GraphicsMisc::GetScreenBitDepth()
{
	HDC hDC = ::GetDC(NULL);
	int nBits = GetDeviceCaps(hDC, BITSPIXEL);
	::ReleaseDC(NULL, hDC);

	return nBits;
}

BOOL GraphicsMisc::WantDPIScaling()
{
	enum PROCESSDPIAWARENESS
	{
		DPI_UNAWARE            = 0,
		SYSTEM_DPI_AWARE       = 1,
		PER_MONITOR_DPI_AWARE  = 2
	};

	typedef HRESULT (WINAPI *PFNGETPROCESSDPIAWARENESS)(HANDLE, PROCESSDPIAWARENESS*);

	if (s_bWantDPIScaling == -1)
	{
		HMODULE hMod = ::LoadLibrary(_T("Shcore.dll"));
		s_bWantDPIScaling = FALSE;

		if (hMod)
		{
			PFNGETPROCESSDPIAWARENESS pFn = (PFNGETPROCESSDPIAWARENESS)::GetProcAddress(hMod, "GetProcessDpiAwareness");

			if (pFn)
			{
				PROCESSDPIAWARENESS nAwareness = DPI_UNAWARE;

				if (pFn(NULL, &nAwareness) == S_OK)
				{
					if (nAwareness != DPI_UNAWARE)
						s_bWantDPIScaling = (GetDPIScaleFactor() > 1.0);
				}
			}
		}
	}

	return s_bWantDPIScaling;
}

BOOL GraphicsMisc::ScaleByDPIFactor(LPRECT pRect)
{
	if (!WantDPIScaling())
		return FALSE;

	pRect->left		= ScaleByDPIFactor(pRect->left);
	pRect->top		= ScaleByDPIFactor(pRect->top);
	pRect->right	= ScaleByDPIFactor(pRect->right);
	pRect->bottom	= ScaleByDPIFactor(pRect->bottom);

	return TRUE;
}

BOOL GraphicsMisc::ScaleByDPIFactor(LPSIZE pSize)
{
	if (!WantDPIScaling())
		return FALSE;
	
	pSize->cx = ScaleByDPIFactor(pSize->cx);
	pSize->cy = ScaleByDPIFactor(pSize->cy);
	
	return TRUE;
}

BOOL GraphicsMisc::ScaleByDPIFactor(LPPOINT pPoint)
{
	if (!WantDPIScaling())
		return FALSE;
	
	pPoint->x = ScaleByDPIFactor(pPoint->x);
	pPoint->y = ScaleByDPIFactor(pPoint->y);
	
	return TRUE;
}

int GraphicsMisc::ScaleByDPIFactor(int nValue)
{
	return ::MulDiv(nValue, GetSystemDPI(), DEFAULT_DPI);
}

COLORREF GraphicsMisc::GetSolidColor(HBRUSH hBrush)
{
	LOGBRUSH lb = { 0 };
	VERIFY(::GetObject(hBrush, sizeof(lb), &lb));

	return lb.lbColor;
}

COLORREF GraphicsMisc::GetGroupHeaderColor() 
{
	return GetSysColor(COLOR_HOTLIGHT);
}

void GraphicsMisc::DrawGroupHeaderRow(CDC* pDC, HWND hWnd, CRect& rRow, const CString& sText, COLORREF crText, COLORREF crBack)
{
	CSaveDC dc(pDC);

	if (crText == CLR_NONE)
	{
		if (crBack == CLR_NONE)
			crText = GetGroupHeaderColor();
		else
			crText = GetBestTextColor(crBack);
	}

	if (crBack == CLR_NONE)
		crBack = GetSysColor(COLOR_WINDOW);

	FillItemRect(pDC, rRow, crBack, hWnd); // Modifies rRow

	CRect rLine(rRow);

	if (!sText.IsEmpty())
	{
		// Force text to always be visible on the LHS
		CRect rText(rRow);
		rText.left = ScaleByDPIFactor(10);

		{
			// We share a single font across the entire app
			// and if ever the font changes for a given window
			// we just recreate the font
			HFONT hFont = GetFont(hWnd);

			if (hFont && !IsSameFontNameAndSize(hFont, s_hFontGroup))
			{
				VerifyDeleteObject(s_hFontGroup);

				s_hFontGroup = CreateFont(hFont, GMFS_BOLD);
				ASSERT(s_hFontGroup);
			}
			pDC->SelectObject(s_hFontGroup);

			// Note: The CSaveDC at the top will de-select the 
			// font from the dc when it goes out of scope
		}

		pDC->SetTextColor(crText);
		pDC->SetBkColor(crBack);
		pDC->SetBkMode(OPAQUE);
		pDC->DrawText(sText, rText, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

		rLine.left = (rText.left + pDC->GetTextExtent(sText).cx + ScaleByDPIFactor(5));
	}

	DrawHorzLine(pDC, rLine.left, rLine.right, rRow.CenterPoint().y, crText);
}

BOOL GraphicsMisc::DrawShortcutOverlay(CDC* pDC, LPCRECT pRect)
{
	int nSize = 0;

	if (!s_ilShortcutOverlay.GetSafeHandle())
	{
		// Try first for small version of high-res shortcut
		UINT nFlags = ILC_MASK;
		HICON hIcon = ShellIcons::GetIcon(ShellIcons::SI_SHORTCUTNEW, false);

		if (hIcon)
		{
			nSize = ScaleByDPIFactor(8); 
			nFlags |= ILC_COLOR24; // opaque
		}
		else
		{
			// Full size version of low-res shortcut
			hIcon = ShellIcons::GetIcon(ShellIcons::SI_SHORTCUT, true);
			nSize = 32; // full size
			nFlags |= ILC_COLOR32; // transparent
		}

		VERIFY(s_ilShortcutOverlay.Create(nSize, nSize, nFlags, 0, 1));
		VERIFY(s_ilShortcutOverlay.Add(hIcon) == 0);
	}
	else
	{
		nSize = s_ilShortcutOverlay.GetImageSize();
	}

	CPoint ptPos(pRect->left, (pRect->bottom - nSize));

	return s_ilShortcutOverlay.Draw(pDC, 0, ptPos, ILD_TRANSPARENT);
}