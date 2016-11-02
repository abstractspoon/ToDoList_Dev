// GraphicsMisc.cpp: implementation of the GraphicsMisc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphicsMisc.h"
#include "Misc.h"
#include "themed.h"
#include "colordef.h"
#include "osversion.h"

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

static int PointsPerInch() { return 72; }

//////////////////////////////////////////////////////////////////////

// private helpers
void InitBitmapInfo(BITMAPINFO *pbmi, ULONG cbInfo, LONG cx, LONG cy, WORD bpp);
BOOL Create32BitHBITMAP(HDC hdc, const SIZE *psize, void **ppvBits, HBITMAP* phBmp);
BOOL HasAlpha(ARGB *pargb, SIZE& sizImage, int cxRow);
BOOL ConvertToPARGB32(HDC hdc, ARGB *pargb, HBITMAP hbmp, SIZE& sizImage, int cxRow);
BOOL ConvertBufferToPARGB32(HPAINTBUFFER hPaintBuffer, HDC hdc, HICON hicon, SIZE& sizIcon);

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

	vert[0] .x      = pRect->left;
	vert[0] .y      = pRect->top;
	vert[0] .Red    = MAKEWORD(0, GetRValue(crFrom));
	vert[0] .Green  = MAKEWORD(0, GetGValue(crFrom));
	vert[0] .Blue   = MAKEWORD(0, GetBValue(crFrom));
	vert[0] .Alpha  = 0x0000;
	vert[1] .x      = pRect->right;
	vert[1] .y      = pRect->bottom; 
	vert[1] .Red    = MAKEWORD(0, GetRValue(crTo));
	vert[1] .Green  = MAKEWORD(0, GetGValue(crTo));
	vert[1] .Blue   = MAKEWORD(0, GetBValue(crTo));
	vert[1] .Alpha  = 0x0000;
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
	DWORD dwEdges = (bEdged ? (bVert ? (GMDR_LEFT | GMDR_RIGHT) : (GMDR_TOP | GMDR_BOTTOM)) : 0);

	DrawRect(pDC, rSplitter, crSplitBar, Darker(crSplitBar, 0.2), 0, dwEdges);

	// draw drag marker (2 x 20)
	int nSplitWidth = min(rSplitter.Width(), rSplitter.Height());
	
	if (nSplitWidth > 2)
	{
		CRect rMarker(rSplitter);
		CPoint ptCentre = rMarker.CenterPoint();
		
		if (bVert)
		{
			rMarker.left = ptCentre.x - 1;
			rMarker.right = ptCentre.x + 1;

			rMarker.top = ptCentre.y - 10;
			rMarker.bottom = ptCentre.y + 10;
		}
		else // horizontal
		{
			rMarker.left = ptCentre.x - 10;
			rMarker.right = ptCentre.x + 10;
			
			rMarker.top = ptCentre.y - 1;
			rMarker.bottom = ptCentre.y + 1;
		}
		
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
	if (!SameFontNameSize(hFont, hFontOut))
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
	
	// set the charset
	if (dwFlags & GMFS_SYMBOL)
	{
		lf.lfCharSet = SYMBOL_CHARSET;
	}
	else if (!lf.lfCharSet)
	{
		lf.lfCharSet = DEFAULT_CHARSET;
	}
	
	if (!Misc::IsEmpty(szFaceName))
	{
		lstrcpy(lf.lfFaceName, szFaceName);
		
		// reset character-set 
		lf.lfCharSet = DEFAULT_CHARSET;
	}
	
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
	HDC hDC = ::GetDC(NULL);
	int nPPI = GetDeviceCaps(hDC, LOGPIXELSY);
	::ReleaseDC(NULL, hDC);

	return nPPI;
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

HFONT GraphicsMisc::GetFont(HWND hWnd)
{
	ASSERT(hWnd);

	return (HFONT)::SendMessage(hWnd, WM_GETFONT, 0, 0);
}

BOOL GraphicsMisc::SameFont(HFONT hFont, LPCTSTR szFaceName, int nPoint)
{
	CString sFontName;
	int nFontSize = GetFontNameAndPointSize(hFont, sFontName);

	return ((nPoint <= 0 || nPoint == nFontSize) && 
			(!szFaceName || sFontName.CompareNoCase(szFaceName) == 0));
}

BOOL GraphicsMisc::SameFontNameSize(HFONT hFont1, HFONT hFont2)
{
	CString sName1;
	int nSize1 = GetFontNameAndPointSize(hFont1, sName1);

	return SameFont(hFont2, sName1, nSize1);
}

HICON GraphicsMisc::LoadIcon(UINT nIDIcon, int nSize)
{
	HICON hIcon = (HICON)::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(nIDIcon), 
									IMAGE_ICON, nSize, nSize, LR_LOADMAP3DCOLORS);

	return hIcon;
}

HCURSOR GraphicsMisc::HandCursor()
{
	static HCURSOR cursor = ::LoadCursor(NULL, IDC_HAND);

	return cursor;
}

HCURSOR GraphicsMisc::OleDragDropCursor(GM_OLECURSOR nCursor)
{
	static HCURSOR cursors[GMOC_COUNT] = { 0 };

	// load once only
	if (cursors[nCursor] == NULL)
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
			cursors[nCursor] = ::LoadCursor(hMod, MAKEINTRESOURCE(nIDCursor));
		}
	}

	return cursors[nCursor];
}

CFont& GraphicsMisc::WingDings()
{
	static CFont font;
				
	if (!font.GetSafeHandle())
		font.Attach(CreateFont(_T("Wingdings"), -1, GMFS_SYMBOL));

	return font;
}

CFont& GraphicsMisc::Marlett()
{
	static CFont font;
				
	if (!font.GetSafeHandle())
		font.Attach(CreateFont(_T("Marlett"), -1, GMFS_SYMBOL));

	return font;
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

int GraphicsMisc::GetTextWidth(const CString& sText, HWND hWnd, HFONT hFontRef)
{
	CWnd* pRefWnd = CWnd::FromHandle(hWnd);

	if (!pRefWnd)
		return -1;

	// else
	return GetTextWidth(sText, *pRefWnd, CFont::FromHandle(hFontRef));
}

int AFX_CDECL GraphicsMisc::GetTextWidth(CDC* pDC, LPCTSTR lpszFormat, ...)
{
	static TCHAR BUFFER[2048];

	ASSERT(AfxIsValidString(lpszFormat));

	va_list argList;
	va_start(argList, lpszFormat);
//fabio_2005
#if _MSC_VER >= 1400
	_vstprintf_s(BUFFER, lpszFormat, argList);
#else
	_vstprintf(BUFFER, lpszFormat, argList);
#endif
	va_end(argList);

	return pDC->GetTextExtent(BUFFER).cx;
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

int GraphicsMisc::GetAverageStringWidth(const CString& sText, CDC* pDC)
{
	if (sText.IsEmpty())
		return 0;

	return (int)(GetAverageCharWidth(pDC) * sText.GetLength());
}

int GraphicsMisc::GetAverageMaxStringWidth(const CString& sText, CDC* pDC)
{
	if (sText.IsEmpty())
		return 0;

	int nAveWidth = GetAverageStringWidth(sText, pDC);
	int nActualWidth = pDC->GetTextExtent(sText).cx;

	return max(nAveWidth, nActualWidth);
}

CFont* GraphicsMisc::PrepareDCFont(CDC* pDC, HWND hwndRef, CFont* pFont, int nStockFont)
{
	if ((pFont == NULL) && (hwndRef != NULL))
		pFont = CFont::FromHandle(GetFont(hwndRef));

	if (pFont)
	{
#ifdef _DEBUG
		CString sFont;
		int nPoint = GetFontNameAndPointSize(*pFont, sFont);
#endif
		return pDC->SelectObject(pFont);
	}
	
	// else
	return (CFont*)pDC->SelectStockObject(nStockFont);
}

COLORREF GraphicsMisc::GetBestTextColor(COLORREF crBack)
{
	// base text color on luminance
	return (RGBX(crBack).Luminance() < 128) ? RGB(255, 255, 255) : 0;
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

COLORREF GraphicsMisc::Blend(COLORREF color1, COLORREF color2, double dAmount)
{
	if (color1 == CLR_NONE || color2 == CLR_NONE)
		return CLR_NONE;

	int red1 = GetRValue(color1);
	int green1 = GetGValue(color1);
	int blue1 = GetBValue(color1);
	
	int red2 = GetRValue(color2);
	int green2 = GetGValue(color2);
	int blue2 = GetBValue(color2);
	
	int redBlend = (int)((red1 + red2) * dAmount);
	int greenBlend = (int)((green1 + green2) * dAmount);
	int blueBlend = (int)((blue1 + blue2) * dAmount);

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

		for (int nColor = 1; nColor < (nNumColors - 1); nColor++)
		{
			double dRed = (redFrom + (redRange * nColor / (nNumColors - 1)));
			double dGreen = (greenFrom + (greenRange * nColor / (nNumColors - 1)));
			double dBlue = (blueLow + (blueRange * nColor / (nNumColors - 1)));

			double dColor = dRed + (dGreen * 256) + (dBlue * 256 * 256);
			COLORREF color = min(RGB(255, 255, 255), (COLORREF)(int)dColor);

			aColors[nColor] = color;
		}
	}
	else // HLS Gradient
	{
		HLSX hlsLow, hlsHigh;
		RGBX rgbLow(crFrom), rgbHigh(crTo);

		// convert rgb limits to hls
		RGBX::RGB2HLS(rgbLow, hlsLow);
		RGBX::RGB2HLS(rgbHigh, hlsHigh);

		float fHueInc = ((hlsHigh.fHue - hlsLow.fHue) / (nNumColors - 1));
		float fSatInc = ((hlsHigh.fSaturation - hlsLow.fSaturation) / (nNumColors - 1));
		float fLumInc = ((hlsHigh.fLuminosity - hlsLow.fLuminosity) / (nNumColors - 1));

		HLSX hlsColor = hlsLow;

		for (int nColor = 1; nColor < (nNumColors - 1); nColor++)
		{
			hlsColor.fHue += fHueInc;
			hlsColor.fSaturation += fSatInc;
			hlsColor.fLuminosity += fLumInc;

			RGBX rgbColor;
			RGBX::HLS2RGB(hlsColor, rgbColor);

			COLORREF color = min(RGB(255, 255, 255), (COLORREF)rgbColor);
			aColors[nColor] = color;
		}
	}
}

BOOL GraphicsMisc::ForceIconicRepresentation(HWND hWnd, BOOL bForce)
{
#ifndef DWMWA_FORCE_ICONIC_REPRESENTATION
# define DWMWA_FORCE_ICONIC_REPRESENTATION 7
#endif
	
	return DwmSetWindowAttribute(hWnd, DWMWA_FORCE_ICONIC_REPRESENTATION, &bForce, sizeof(bForce));
}

BOOL GraphicsMisc::EnableAeroPeek(HWND hWnd, BOOL bEnable)
{
#ifndef DWMWA_DISALLOW_PEEK
# define DWMWA_DISALLOW_PEEK 11
#endif
	
	BOOL bDisallow = !bEnable;
	
	return DwmSetWindowAttribute(hWnd, DWMWA_DISALLOW_PEEK, &bDisallow, sizeof(bDisallow));
}

BOOL GraphicsMisc::GetExtendedFrameBounds(HWND hWnd, CRect& rBounds)
{
#ifndef DWMWA_EXTENDED_FRAME_BOUNDS
# define DWMWA_EXTENDED_FRAME_BOUNDS 9
#endif

	return DwmGetWindowAttribute(hWnd, DWMWA_EXTENDED_FRAME_BOUNDS, &rBounds, sizeof(rBounds));
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
	
	return DwmSetWindowAttribute(hWnd, DWMWA_FLIP3D_POLICY, &nPolicy, sizeof(nPolicy));
}

BOOL GraphicsMisc::DwmSetWindowAttribute(HWND hWnd, DWORD dwAttrib, LPCVOID pData, DWORD dwDataSize)
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

BOOL GraphicsMisc::DwmGetWindowAttribute(HWND hWnd, DWORD dwAttrib, PVOID pData, DWORD dwDataSize)
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

int GraphicsMisc::DrawAnsiSymbol(CDC* pDC, char cSymbol, const CRect& rText, UINT nFlags, CFont* pFont)
{
	if (cSymbol == 0)
		return 0;

	CFont* pOldFont = pFont ? pDC->SelectObject(pFont) : NULL;
	pDC->SetBkMode(TRANSPARENT);
	int nResult = 0;

	// draw as ANSI string
	char szAnsi[2] = { cSymbol, 0 };
	nResult = ::DrawTextA(*pDC, szAnsi, 1, (LPRECT)(LPCRECT)rText, nFlags);
	
	if (pFont)
		pDC->SelectObject(pOldFont);

	return nResult;
}

void GraphicsMisc::DrawRect(CDC* pDC, const CRect& rect, COLORREF crFill, COLORREF crBorder, int nCornerRadius, DWORD dwEdges)
{
	if (rect.IsRectEmpty())
		return;

	if (nCornerRadius == 0)
	{
		// can't have border color and no edges
		if (dwEdges == 0)
			crBorder = CLR_NONE;

		// if both colours are set there's an optimisation we can do
		if ((crFill != CLR_NONE) && (crBorder != CLR_NONE))
		{
			pDC->FillSolidRect(rect, crBorder);

			if (crFill != crBorder)
			{
				CRect rFill(rect);

				if ((dwEdges & GMDR_ALL) == GMDR_ALL)
				{
					rFill.DeflateRect(1, 1);
				}
				else
				{
					if (dwEdges & GMDR_LEFT)
						rFill.left++;

					if (dwEdges & GMDR_TOP)
						rFill.top++;

					if (dwEdges & GMDR_RIGHT)
						rFill.right--;

					if (dwEdges & GMDR_BOTTOM)
						rFill.bottom--;
				}

				pDC->FillSolidRect(rFill, crFill);
			}
		}
		else if (crFill != CLR_NONE) // inside of rect
		{
			pDC->FillSolidRect(rect, crFill);
		}
		else if (crBorder != CLR_NONE) // border
		{
			if (dwEdges & GMDR_TOP)
				pDC->FillSolidRect(rect.left, rect.top, rect.Width(), 1, crBorder);

			if (dwEdges & GMDR_BOTTOM)
				pDC->FillSolidRect(rect.left, rect.bottom - 1, rect.Width(), 1, crBorder);

			if (dwEdges & GMDR_LEFT)
				pDC->FillSolidRect(rect.left, rect.top, 1, rect.Height(), crBorder);

			if (dwEdges & GMDR_RIGHT)
				pDC->FillSolidRect(rect.right - 1, rect.top, 1, rect.Height(), crBorder);
		}
	}
	else // round-rect
	{
		CPen* pOldPen = NULL, penBorder;
		CBrush* pOldBrush = NULL, brFill;

		// inside of rect
		if (crFill != CLR_NONE)
		{
			brFill.CreateSolidBrush(crFill);
			pOldBrush = pDC->SelectObject(&brFill);
		}

		// border
		if (crBorder != CLR_NONE)
		{
			penBorder.CreatePen(PS_SOLID, 1, crBorder);
			pOldPen = pDC->SelectObject(&penBorder);
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

	if (hwnd)
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

COLORREF GraphicsMisc::GetExplorerItemTextColor(COLORREF crBase, GM_ITEMSTATE nState, DWORD dwFlags)
{
	if (nState != GMIS_NONE)
	{
		BOOL bHighContrast = Misc::IsHighContrastActive();
		BOOL bThemed = (CThemed::AreControlsThemed() && (COSVersion() >= OSV_VISTA));
		
		if (!bHighContrast && (bThemed || (dwFlags & GMIB_THEMECLASSIC)))
		{
			switch (nState)
			{
			case GMIS_SELECTED:
			case GMIS_SELECTEDNOTFOCUSED:
			case GMIS_DROPHILITED:
				// darken the base color to have a luminance <= 20%
				{
					HLSX hlsText(crBase);

					if (hlsText.fLuminosity > 0.2f)
					{
						hlsText.fLuminosity = 0.2f;
						return hlsText;
					}
					
					// else
					return crBase;
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

BOOL GraphicsMisc::DrawExplorerItemBkgnd(CDC* pDC, HWND hwnd, GM_ITEMSTATE nState, const CRect& rItem, DWORD dwFlags, LPCRECT prClip)
{
	if ((nState == GMIS_NONE) || (hwnd == NULL))
		return FALSE;

	BOOL bHighContrast = Misc::IsHighContrastActive();
	BOOL bThemed = (!bHighContrast && CThemed::AreControlsThemed() && (COSVersion() >= OSV_VISTA));

	// adjust drawing rect/flags accordingly
	CRect rDraw(rItem), rClip(prClip);
	DWORD dwDRFlags = GMDR_ALL; // for classic theme

	if (dwFlags)
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
				dwDRFlags &= ~GMDR_LEFT;
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
				dwDRFlags &= ~GMDR_RIGHT;
			}
		}

		prClip = rClip;
	}
	
	// draw the rect
	if (bThemed)
	{
		CThemed th(hwnd, _T("Explorer::ListView"));
		
		// always fill background with white to get the exact selection colour
		pDC->FillSolidRect(rDraw, RGB(255, 255, 255));
		
		switch (nState)
		{
		case GMIS_SELECTED:
			th.DrawBackground(pDC, LVP_LISTITEM, LIS_MORESELECTED, rDraw, prClip);
			// fall thru to overdraw again

		case GMIS_SELECTEDNOTFOCUSED:
		case GMIS_DROPHILITED:
			th.DrawBackground(pDC, LVP_LISTITEM, LIS_MORESELECTED, rDraw, prClip);
			break;
			
		default:
			ASSERT(0);
			return FALSE;
		}
	}
	// Can be XP, Linux or Classic theme, or themes disabled for this app only
	else if (!bHighContrast && (dwFlags & GMIB_THEMECLASSIC))
	{
		// draw a bordered box based on standard colours
		COLORREF crBorder = CLR_NONE, crFill = CLR_NONE;
		
		switch (nState)
		{
		case GMIS_SELECTED:
			// Because the 'Classic' selection colour varies between
			// OS versions I'm am going to standardise on XP's
			// Navy Blue colour as the 'original'
			crBorder = RGB(50, 105, 200); 
			crFill = RGB(175, 195, 240);
			break;
			
		case GMIS_SELECTEDNOTFOCUSED:
		case GMIS_DROPHILITED:
			crBorder = GetSysColor(COLOR_3DSHADOW);
			crFill = GetSysColor(COLOR_3DFACE);
			break;
			
		default:
			ASSERT(0);
			return FALSE;
		}
		
		DrawRect(pDC, rDraw, crFill, crBorder, 0, dwDRFlags);
	}
	else
	{
		switch (nState)
		{
		case GMIS_SELECTED:
			pDC->FillSolidRect(rDraw, GetSysColor(COLOR_HIGHLIGHT));
			break;
			
		case GMIS_SELECTEDNOTFOCUSED:
		case GMIS_DROPHILITED:
			pDC->FillSolidRect(rDraw, GetSysColor(COLOR_3DFACE));
			break;
			
		default:
			ASSERT(0);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL GraphicsMisc::GetAvailableScreenSpace(const CRect& rWnd, CRect& rScreen)
{
	if (rWnd.IsRectEmpty())
		return FALSE;

	HMONITOR hMon = MonitorFromPoint(rWnd.CenterPoint(), MONITOR_DEFAULTTONULL);

	if (hMon == NULL)
		return FALSE;

	MONITORINFO mi = { sizeof(MONITORINFO), 0 };
	
	if (GetMonitorInfo(hMon, &mi))
	{
		rScreen = mi.rcWork;
		return TRUE;
	}

	// else
	ASSERT(0);
	return FALSE;

}

BOOL GraphicsMisc::GetAvailableScreenSpace(HWND hWnd, CRect& rScreen)
{
	if (hWnd == NULL)
		return FALSE;

	// else
	CRect rWnd;
	::GetWindowRect(hWnd, rWnd);

	return GetAvailableScreenSpace(rWnd, rScreen);
}

void GraphicsMisc::GetTotalAvailableScreenSpace(CRect& rScreen)
{
	rScreen.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
	rScreen.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
	rScreen.right = (rScreen.left + GetSystemMetrics(SM_CXVIRTUALSCREEN));
	rScreen.bottom = (rScreen.top + GetSystemMetrics(SM_CYVIRTUALSCREEN));
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

UINT GraphicsMisc::GetRTLDrawTextFlags(HWND hwnd)
{
	ASSERT(hwnd);

	if (hwnd)
	{
		DWORD dwStyle = (DWORD)GetWindowLong(hwnd, GWL_EXSTYLE);
		BOOL bRTLLayout = ((dwStyle & WS_EX_LAYOUTRTL) ? TRUE : FALSE);
		BOOL bRTLReading = ((dwStyle & WS_EX_RTLREADING) ? TRUE : FALSE);
		
		return ((bRTLReading != bRTLLayout) ? DT_RTLREADING : 0);
	}

	// else
	return 0;
}

UINT GraphicsMisc::GetRTLDrawTextFlags(CDC* pDC)
{

	if (pDC && pDC->GetWindow())
		return GetRTLDrawTextFlags(pDC->GetWindow()->GetSafeHwnd());

	// else
	ASSERT(0);
	return 0;
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

	static HMODULE hMod = ::LoadLibrary(_T("User32.dll"));
	
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
