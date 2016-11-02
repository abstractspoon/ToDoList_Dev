// GraphicsMisc.h: interface for the GraphicsMisc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHICSMISC_H__A3408501_A44D_407B_A8C3_B6AB31370CD2__INCLUDED_)
#define AFX_GRAPHICSMISC_H__A3408501_A44D_407B_A8C3_B6AB31370CD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

enum 
{ 
	GMFS_PLAIN		= 0x00, 
	GMFS_BOLD		= 0x01, 
	GMFS_ITALIC		= 0x02, 
	GMFS_UNDERLINED	= 0x04, 
	GMFS_STRIKETHRU	= 0x08, 
	GMFS_SYMBOL		= 0x10,

	GMFS_ALL		= 0xff
};

enum // DrawRect
{
	GMDR_NONE		= 0x0, 	
		
	GMDR_LEFT		= 0x1, 		
	GMDR_TOP		= 0x2, 
	GMDR_RIGHT		= 0x4, 
	GMDR_BOTTOM		= 0x8, 

	GMDR_ALL		= 0xf
};

enum GM_GRADIENT
{
	GMG_NONE,
	GMG_GRADIENT,
	GMG_GLASS,
	GMG_GLASSWITHGRADIENT,
};

enum GM_ITEMSTATE // DrawExplorerItemBackground
{
	GMIS_NONE,
	GMIS_SELECTED,
	GMIS_SELECTEDNOTFOCUSED,
	GMIS_DROPHILITED,
};

enum // DrawExplorerItemBackground
{
	GMIB_NONE			= 0x00,
	GMIB_EXTENDRIGHT	= 0x01,
	GMIB_EXTENDLEFT		= 0x02,
	GMIB_CLIPRIGHT		= 0x04,
	GMIB_CLIPLEFT		= 0x08,
	GMIB_THEMECLASSIC	= 0x10,
};

enum GM_OLECURSOR // OLE drag'n'drop cursors
{
	GMOC_NO,
	GMOC_MOVE,
	GMOC_COPY,
	GMOC_LINK,

	GMOC_COUNT
};

//////////////////////////////////////////////////////////////////////

#ifndef DCX_NODELETERGN
#	define DCX_NODELETERGN 0x00040000
#endif

#ifndef DCX_USESTYLE
#	define DCX_USESTYLE 0x00010000
#endif

//////////////////////////////////////////////////////////////////////

namespace GraphicsMisc  
{
	BOOL DrawGradient(GM_GRADIENT nType, CDC* pDC, LPCRECT pRect, COLORREF crFrom, COLORREF crTo, BOOL bHorz, int nBorder = -1);
	COLORREF GetGradientEdgeColor(GM_GRADIENT nType, COLORREF color, BOOL bFrom);
	GM_GRADIENT GetGradientType(BOOL bGlass, BOOL bGradient);

	void DrawHorzLine(CDC* pDC, int nXFrom, int nXTo, int nYPos, COLORREF crFrom, COLORREF crTo = CLR_NONE);
	void DrawVertLine(CDC* pDC, int nYFrom, int nYTo, int nXPos, COLORREF crFrom, COLORREF crTo = CLR_NONE);
	void DrawSplitBar(CDC* pDC, const CRect& rSplitter, COLORREF crSplitBar, BOOL bEdged = TRUE);

	HFONT CreateFont(HFONT hFont, DWORD dwFlags = 0, DWORD dwMask = GMFS_ALL);
	HFONT CreateFont(LPCTSTR szFaceName, int nPoint = -1, DWORD dwFlags = 0);
	BOOL CreateFont(CFont& font, LPCTSTR szFaceName, int nPoint = -1, DWORD dwFlags = 0);
	BOOL CreateFont(CFont& fontOut, HFONT fontIn, DWORD dwFlags = 0, DWORD dwMask = GMFS_ALL);

	UINT GetRTLDrawTextFlags(HWND hwnd);
	UINT GetRTLDrawTextFlags(CDC* pDC);
	
	HCURSOR HandCursor();
	HICON LoadIcon(UINT nIDIcon, int nSize = 16);
	HBITMAP IconToPARGB32Bitmap(HICON hIcon);
	CSize GetIconSize(HICON hIcon);
	CSize GetBitmapSize(HBITMAP hBmp);
	HCURSOR OleDragDropCursor(GM_OLECURSOR nCursor);
	HICON GetAppWindowIcon(BOOL bLarge);

	int PointToPixel(int nPoints);
	int PixelToPoint(int nPixels);
	int PixelsPerInch();

	DWORD GetFontFlags(HFONT hFont);
	int GetFontNameAndPointSize(HFONT hFont, CString& sFaceName);
	int GetFontPointSize(HFONT hFont);
	int GetFontPixelSize(HFONT hFont);
	int GetFontPointSize(HWND hWnd);
	int GetFontPixelSize(HWND hWnd);
	HFONT GetFont(HWND hWnd);
	BOOL SameFont(HFONT hFont, LPCTSTR szFaceName, int nPoint);
	BOOL SameFontNameSize(HFONT hFont1, HFONT hFont2);
	CFont& WingDings();
	CFont& Marlett();
	int DrawAnsiSymbol(CDC* pDC, char cSymbol, const CRect& rText, UINT nFlags, CFont* pFont = NULL);
	CFont* PrepareDCFont(CDC* pDC, HWND hwndRef = NULL, CFont* pFont = NULL, int nStockFont = DEFAULT_GUI_FONT); // returns 'old' font
	
	int GetTextWidth(const CString& sText, HWND hWnd, HFONT hFontRef = NULL);
	int GetTextWidth(const CString& sText, CWnd& wndRef, CFont* pRefFont = NULL);
	int AFX_CDECL GetTextWidth(CDC* pDC, LPCTSTR lpszFormat, ...);
	float GetAverageCharWidth(CDC* pDC, CFont* pFont = NULL);
	int GetAverageStringWidth(const CString& sText, CDC* pDC);
	int GetAverageMaxStringWidth(const CString& sText, CDC* pDC);

	inline BOOL VerifyDeleteObject(HGDIOBJ hObj)
	{
		if (hObj == NULL)
			return TRUE;

		// else
		if (::DeleteObject(hObj))
			return TRUE;

		// else
		ASSERT(0);
		return FALSE;
	}

	inline BOOL VerifyDeleteObject(CGdiObject& obj)
	{
		if (obj.m_hObject == NULL)
			return TRUE;

		// else
		if (obj.DeleteObject())
			return TRUE;

		// else
		ASSERT(0);
		return FALSE;
	}
	
	COLORREF Lighter(COLORREF color, double dAmount, BOOL bRGB = TRUE);
	COLORREF Darker(COLORREF color, double dAmount, BOOL bRGB = TRUE);
	COLORREF Blend(COLORREF color1, COLORREF color2, double dAmount);
	COLORREF GetBestTextColor(COLORREF crBack);
	void CalculateColorGradient(COLORREF crFrom, COLORREF crTo, int nNumColors, CDWordArray& aColors, BOOL bRGB = TRUE);

	CString GetWebColor(COLORREF color);

	void DrawRect(CDC* pDC, const CRect& rect, COLORREF crFill, COLORREF crBorder = CLR_NONE, 
					int nCornerRadius = 0, DWORD dwEdges = GMDR_ALL);
	BOOL FillItemRect(CDC* pDC, CRect& rItem, COLORREF color, HWND hwnd); // will update rItem
	BOOL FillItemRect(CDC* pDC, LPCRECT prcItem, COLORREF color, HWND hwnd);

	BOOL DrawExplorerItemBkgnd(CDC* pDC, HWND hwnd, GM_ITEMSTATE nState, const CRect& rItem, DWORD dwFlags = GMIB_NONE, LPCRECT prClip = NULL); 
	COLORREF GetExplorerItemTextColor(COLORREF crBase, GM_ITEMSTATE nState, DWORD dwFlags);
	
	BOOL ForceIconicRepresentation(HWND hWnd, BOOL bForce = TRUE);
	BOOL EnableAeroPeek(HWND hWnd, BOOL bEnable = TRUE);
	BOOL EnableFlip3D(HWND hWnd, BOOL bEnable = TRUE);
	BOOL GetExtendedFrameBounds(HWND hWnd, CRect& rBounds);

	BOOL DwmSetWindowAttribute(HWND hWnd, DWORD dwAttrib, LPCVOID pData, DWORD dwDataSize);
	BOOL DwmGetWindowAttribute(HWND hWnd, DWORD dwAttrib, PVOID pData, DWORD dwDataSize);

	BOOL GetAvailableScreenSpace(const CRect& rWnd, CRect& rScreen);
	BOOL GetAvailableScreenSpace(HWND hWnd, CRect& rScreen);
	void GetTotalAvailableScreenSpace(CRect& rScreen);

	void DrawGradient(CDC* pDC, LPCRECT pRect, COLORREF crFrom, COLORREF crTo, BOOL bHorz, int nBorder);
	void DrawGlass(CDC* pDC, LPCRECT pRect, COLORREF crFrom, COLORREF crTo, BOOL bHorz, int nBorder);
	void DrawGlassWithGradient(CDC* pDC, LPCRECT pRect, COLORREF crFrom, COLORREF crTo, BOOL bHorz, int nBorder);

	BOOL FlashWindowEx(HWND hWnd, DWORD dwFlags, UINT  uCount, DWORD dwTimeout);
};

#endif // !defined(AFX_GRAPHICSMISC_H__A3408501_A44D_407B_A8C3_B6AB31370CD2__INCLUDED_)
