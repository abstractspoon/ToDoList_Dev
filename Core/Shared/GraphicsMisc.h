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

enum GM_ITEMSTATE // DrawExplorerItemSelection
{
	GMIS_NONE,
	GMIS_SELECTED,
	GMIS_SELECTEDNOTFOCUSED,
	GMIS_DROPHILITED,
};

enum // DrawExplorerItemSelection
{
	GMIB_NONE			= 0x0000,
	GMIB_EXTENDRIGHT	= 0x0001,
	GMIB_EXTENDLEFT		= 0x0002,
	GMIB_CLIPRIGHT		= 0x0004,
	GMIB_CLIPLEFT		= 0x0008,
	GMIB_THEMECLASSIC	= 0x0010,
	GMIB_PREDRAW		= 0x0020,
	GMIB_POSTDRAW		= 0x0040,
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

#ifndef DPI_AWARENESS_CONTEXT
DECLARE_HANDLE(DPI_AWARENESS_CONTEXT);

#	define DPI_AWARENESS_CONTEXT_UNAWARE              ((DPI_AWARENESS_CONTEXT)-1)
#	define DPI_AWARENESS_CONTEXT_SYSTEM_AWARE         ((DPI_AWARENESS_CONTEXT)-2)
#	define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE    ((DPI_AWARENESS_CONTEXT)-3)
#	define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 ((DPI_AWARENESS_CONTEXT)-4)
#	define DPI_AWARENESS_CONTEXT_UNAWARE_GDISCALED    ((DPI_AWARENESS_CONTEXT)-5)
#endif

//////////////////////////////////////////////////////////////////////

class CSaveDC
{
public:
	CSaveDC(HDC hDC);
	CSaveDC(CDC* pDC);
	~CSaveDC();

protected:
	HDC m_hDC;
	int m_nSave;
};

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
	
	HICON LoadIcon(UINT nIDIcon, int nSize = 16);
	HBITMAP IconToPARGB32Bitmap(HICON hIcon);
	CSize GetIconSize(HICON hIcon);
	CSize GetBitmapSize(HBITMAP hBmp);
	HICON GetAppWindowIcon(BOOL bLarge);

	HCURSOR LoadHandCursor();
	HCURSOR LoadDragDropCursor(GM_OLECURSOR nCursor);
	HCURSOR LoadAppCursor(LPCTSTR szName, LPCTSTR szSubFolder = NULL);
	HCURSOR LoadStandardCursor(LPCTSTR szCursorID);
	BOOL SetHandCursor();
	BOOL SetDragDropCursor(GM_OLECURSOR nCursor);
	BOOL SetAppCursor(LPCTSTR szName, LPCTSTR szSubFolder = NULL);
	BOOL SetStandardCursor(LPCTSTR szCursorID);

	int PointToPixel(int nPoints);
	int PixelToPoint(int nPixels);
	int PixelsPerInch();

	BOOL WantDPIScaling();
	int GetSystemDPI();
	double GetDPIScaleFactor();
	BOOL ScaleByDPIFactor(LPRECT pRect);
	BOOL ScaleByDPIFactor(LPSIZE pSize);
	BOOL ScaleByDPIFactor(LPPOINT pPoint);
	int ScaleByDPIFactor(int nValue);
	
	DWORD GetFontFlags(HFONT hFont);
	int GetFontNameAndPointSize(HFONT hFont, CString& sFaceName);
	int GetFontNameAndPointSize(HWND hWnd, CString& sFaceName);
	int GetFontPointSize(HFONT hFont);
	int GetFontPixelSize(HFONT hFont);
	int GetFontPointSize(HWND hWnd);
	int GetFontPixelSize(HWND hWnd);
	HFONT GetFont(HWND hWnd, BOOL bFallback = TRUE);
	BOOL SameFont(HFONT hFont, LPCTSTR szFaceName, int nPoint);
	BOOL SameFontNameSize(HFONT hFont1, HFONT hFont2);
	CFont& WingDings();
	CFont& Marlett();
	int DrawAnsiSymbol(CDC* pDC, char cSymbol, const CRect& rText, UINT nFlags, CFont* pFont = NULL);
	CFont* PrepareDCFont(CDC* pDC, HWND hwndRef = NULL, CFont* pFont = NULL, int nStockFont = DEFAULT_GUI_FONT); // returns 'old' font
	
	int GetTextWidth(const CString& sText, HWND hWndRef, HFONT hFontRef = NULL);
	int GetTextWidth(const CString& sText, CWnd& wndRef, CFont* pRefFont = NULL);
	int GetTextWidth(CDC* pDC, const CString& sText);
	int GetTabbedTextWidth(CDC* pDC, const CString& sText, int nTabWidth);
	int GetFormattedTextWidth(CDC* pDC, LPCTSTR lpszFormat, ...);
	float GetAverageCharWidth(CDC* pDC, CFont* pFont = NULL);
	int GetAverageStringWidth(const CString& sText, CDC* pDC, CFont* pFont = NULL);
	int GetAverageMaxStringWidth(const CString& sText, CDC* pDC, CFont* pFont = NULL);
	
	CSize GetTextSize(const CString& sText, HWND hWndRef, int nWidth, HFONT hFontRef = NULL);
	CSize GetTextSize(const CString& sText, CWnd& wndRef, int nWidth, CFont* pRefFont = NULL);
	
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
	COLORREF ParseWebColor(const CString& sHexColor);
	void CalculateBoxColors(COLORREF crBase, BOOL bEnabled, COLORREF& crFill, COLORREF& crBorder);

	void DrawRect(CDC* pDC, 
				  const CRect& rect, 
				  COLORREF crFill, 
				  COLORREF crBorder = CLR_NONE, 
				  int nCornerRadius = 0, 
				  DWORD dwBorders = GMDR_ALL, 
				  BYTE cFillOpacity = 255,
				  int nBorderStyle = PS_SOLID);

	BOOL FillItemRect(CDC* pDC, CRect& rItem, COLORREF color, HWND hwnd); // will update rItem
	BOOL FillItemRect(CDC* pDC, LPCRECT prcItem, COLORREF color, HWND hwnd);
	BOOL CentreRect(LPRECT pRect, LPCRECT prcOther, BOOL bCentreHorz = TRUE, BOOL bCentreVert = TRUE);
	CPoint CentrePoint(LPCRECT prcRect);
	void AlignRect(LPRECT pRect, LPCRECT prcOther, int nDrawTextFlags); // DT_LEFT, etc

	BOOL DrawExplorerItemSelection(CDC* pDC, HWND hwnd, GM_ITEMSTATE nState, const CRect& rItem, DWORD dwFlags = GMIB_NONE, LPCRECT prClip = NULL); 
	COLORREF GetExplorerItemSelectionBackColor(GM_ITEMSTATE nState, DWORD dwFlags = GMIB_NONE);
	COLORREF GetExplorerItemSelectionTextColor(COLORREF crBase, GM_ITEMSTATE nState, DWORD dwFlags);
	COLORREF GetSolidColor(HBRUSH hBrush);
	
	BOOL ForceIconicRepresentation(HWND hWnd, BOOL bForce = TRUE);
	BOOL EnableAeroPeek(HWND hWnd, BOOL bEnable = TRUE);
	BOOL EnableFlip3D(HWND hWnd, BOOL bEnable = TRUE);
	BOOL GetExtendedFrameBounds(HWND hWnd, CRect& rBounds);
	BOOL ChangeWindowMessageFilter(UINT nMessage, BOOL bOn);
	BOOL EnableNonClientDpiScaling(HWND hWnd);

	BOOL DwmSetWindowAttributeEx(HWND hWnd, DWORD dwAttrib, LPCVOID pData, DWORD dwDataSize);
	BOOL DwmGetWindowAttributeEx(HWND hWnd, DWORD dwAttrib, PVOID pData, DWORD dwDataSize);
	BOOL DwmIsCompositionEnabled();

	BOOL GetAvailableScreenSpace(const CPoint& point, CRect& rScreen, UINT nFallback = MONITOR_DEFAULTTONULL);
	BOOL GetAvailableScreenSpace(const CRect& rWnd, CRect& rScreen, UINT nFallback = MONITOR_DEFAULTTONULL);
	BOOL GetAvailableScreenSpace(HWND hWnd, CRect& rScreen, UINT nFallback = MONITOR_DEFAULTTONULL);

	BOOL GetMonitorAvailableScreenSpace(HMONITOR hMon, CRect& rScreen, UINT nFallback/* = MONITOR_DEFAULTTONULL*/);
	BOOL GetMonitorAvailableScreenSpace(HMONITOR hMon, CRect& rScreen);

	BOOL GetTotalAvailableScreenSpace(CRect& rScreen);
	BOOL GetPrimaryMonitorScreenSpace(CRect& rScreen);
	int GetScreenBitDepth();

	DPI_AWARENESS_CONTEXT SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT nContext);
	
	void DrawGradient(CDC* pDC, LPCRECT pRect, COLORREF crFrom, COLORREF crTo, BOOL bHorz, int nBorder = -1);
	void DrawGlass(CDC* pDC, LPCRECT pRect, COLORREF crFrom, COLORREF crTo, BOOL bHorz, int nBorder = -1);
	void DrawGlassWithGradient(CDC* pDC, LPCRECT pRect, COLORREF crFrom, COLORREF crTo, BOOL bHorz, int nBorder = -1);

	BOOL FlashWindowEx(HWND hWnd, DWORD dwFlags, UINT  uCount, DWORD dwTimeout);
	BOOL InitCheckboxImageList(HWND hWnd, CImageList& ilCheckboxes, UINT nFallbackBmpID, COLORREF crBkgnd, LPCRECT prPadding = NULL);
	BOOL DrawShortcutOverlay(CDC* pDC, LPCRECT pRect);

};

#endif // !defined(AFX_GRAPHICSMISC_H__A3408501_A44D_407B_A8C3_B6AB31370CD2__INCLUDED_)
