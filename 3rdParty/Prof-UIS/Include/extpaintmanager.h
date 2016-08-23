// This is part of the Professional User Interface Suite library.
// Copyright (C) 2001-2004 FOSS Software, Inc.
// All rights reserved.
//
// http://www.prof-uis.com
// http://www.fossware.com
// mailto:foss@fossware.com
//
// This source code can be used, modified and redistributed
// under the terms of the license agreement that is included
// in the Professional User Interface Suite package.
//
// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

#if (!defined __EXT_PAINT_MANAGER_H)
#define __EXT_PAINT_MANAGER_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_CMD_ICON_H)
	#include <ExtCmdIcon.h>
#endif // __EXT_CMD_ICON_H

#if (!defined __AFXTEMPL_H__)
	#include <AfxTempl.h>
#endif

#if (!defined __DEFAULT_PAINT_MANAGER_CLASS)
	#define __DEFAULT_PAINT_MANAGER_CLASS CExtPaintManagerXP
#endif

#define __ExtMfc_WM_THEMECHANGED 0x031A

// Prof-UIS impl defines

#define __ExtMfc_MIN_SYS_COLOR_VAL	0L
#define __ExtMfc_MAX_SYS_COLOR_VAL	49L
#define __ExtMfc_COLOR_MAP_BASE		1000L
#define __ExtMfc_DISABLED_COLOR_SHIFT 1000L
#define __ExtMfc_XP_COLOR_SHIFT (__ExtMfc_DISABLED_COLOR_SHIFT+1000L)
#define __ExtMfc_2003_COLOR_SHIFT (__ExtMfc_XP_COLOR_SHIFT+1000L)

//#define __ExtMfc_MENU_GAP__ 4

#define __EXT_MENU_ICON_CX 16
#define __EXT_MENU_ICON_CY 16
#define __EXT_MENU_GAP 3
#define __EXT_MENU_MIN_HEIGHT (__EXT_MENU_ICON_CY+__EXT_MENU_GAP*2)
#define __EXT_MENU_SEPARATOR_HEIGHT 4
#define __EXT_MENU_POPUP_ARROW_AREA_DX 16
#define __EXT_MENU_IMG_RECT_OUT_GAP 2
#define __EXT_MENU_VERT_DISTANCE_NORM 1
#define __EXT_MENU_VERT_DISTANCE_TOOL 1
#define __EXT_MENU_TOOLBTN_GAP 2

#define __EXT_VERT_FONT_ESCAPEMENT_X__ (900)
#define __EXT_VERT_FONT_ESCAPEMENT__ (2700)

#define __DROPDOWN_ARROW_GAP 3

// uxtheme.dll defines

#define __EXT_UX_THEMEAPI(_name_)           HRESULT ( __stdcall * _name_ )
#define __EXT_UX_THEMEAPI_(_type_,_name_)   _type_ ( __stdcall * _name_ )
#define __EXT_UX_CALL                       __stdcall
#define __EXT_UX_DTT_GRAYED                 0x1
#define __EXT_UX_HTTB_BACKGROUNDSEG         0x0000  
#define __EXT_UX_HTTB_FIXEDBORDER           0x0002
#define __EXT_UX_HTTB_CAPTION               0x0004  
#define __EXT_UX_HTTB_RESIZINGBORDER_LEFT   0x0010
#define __EXT_UX_HTTB_RESIZINGBORDER_TOP    0x0020
#define __EXT_UX_HTTB_RESIZINGBORDER_RIGHT  0x0040
#define __EXT_UX_HTTB_RESIZINGBORDER_BOTTOM 0x0080
#define __EXT_UX_HTTB_RESIZINGBORDER \
	(__EXT_UX_HTTB_RESIZINGBORDER_LEFT \
	|__EXT_UX_HTTB_RESIZINGBORDER_TOP \
	|__EXT_UX_HTTB_RESIZINGBORDER_RIGHT \
	|__EXT_UX_HTTB_RESIZINGBORDER_BOTTOM)
#define __EXT_UX_HTTB_SIZINGTEMPLATE        0x0100
#define __EXT_UX_HTTB_SYSTEMSIZINGMARGINS   0x0200
#define __EXT_UX_MAX_INTLIST_COUNT          10
#define __EXT_UX_ETDT_DISABLE               0x00000001
#define __EXT_UX_ETDT_ENABLE                0x00000002
#define __EXT_UX_ETDT_USETABTEXTURE         0x00000004
#define __EXT_UX_ETDT_ENABLETAB \
	(__EXT_UX_ETDT_ENABLE| \
	__EXT_UX_ETDT_USETABTEXTURE)
#define __EXT_UX_STAP_ALLOW_NONCLIENT       (1 << 0)
#define __EXT_UX_STAP_ALLOW_CONTROLS        (1 << 1)
#define __EXT_UX_STAP_ALLOW_WEBCONTENT      (1 << 2)
#define __EXT_UX_SZ_THDOCPROP_DISPLAYNAME   L"DisplayName"
#define __EXT_UX_SZ_THDOCPROP_CANONICALNAME L"ThemeName"
#define __EXT_UX_SZ_THDOCPROP_TOOLTIP       L"ToolTip"
#define __EXT_UX_SZ_THDOCPROP_AUTHOR        L"author"
#define __EXT_UX_DTBG_CLIPRECT              0x00000001
#define __EXT_UX_DTBG_DRAWSOLID             0x00000002
#define __EXT_UX_DTBG_OMITBORDER            0x00000004
#define __EXT_UX_DTBG_OMITCONTENT           0x00000008
#define __EXT_UX_DTBG_COMPUTINGREGION       0x00000010
#define __EXT_UX_DTBG_MIRRORDC              0x00000020

// paint managers

class CExtControlBar;
class CExtToolControlBar;

class __PROF_UIS_API CExtPaintManager : public CObject
{
	DECLARE_DYNCREATE( CExtPaintManager )
protected:
	// color values container
	typedef CArray < COLORREF, COLORREF & >
		sys_colors_container;
	sys_colors_container m_colors;

	// system brushes container
	typedef CArray < HBRUSH, HBRUSH & >
		sys_brashes_container;

	sys_brashes_container m_brushes;

	// translated colors map
	typedef
		CMap < int, int, int, int >
		translate_color_map;

	translate_color_map m_mapColorTranslate;

	class surface_cache_data_t
	{
		CDC m_dc;
		HGDIOBJ m_hBmpOld;
	public:
		surface_cache_data_t()
			: m_hBmpOld( NULL )
		{
		}
		~surface_cache_data_t()
		{
			Free();
		}
		void Free()
		{
			if( m_dc.GetSafeHdc() != NULL )
			{
				::SelectObject( m_dc, m_hBmpOld );
				m_dc.DeleteDC();
				m_hBmpOld = NULL;
			} // if( m_dc.GetSafeHdc() != NULL )
		}
		CDC & Alloc( HGDIOBJ hBmp )
		{
			ASSERT( hBmp != NULL );
			if( m_dc.GetSafeHdc() != NULL )
			{
				::SelectObject( m_dc, hBmp );
				return m_dc;
			} // if( m_dc.GetSafeHdc() != NULL )
			if( m_dc.CreateCompatibleDC(NULL) )
			{
				m_hBmpOld = ::SelectObject( m_dc, hBmp );
			}
#ifdef _DEBUG
			else
			{
				ASSERT( FALSE );
			}
#endif // _DEBUG
			return m_dc;
		}
	}; // class surface_cache_data_t

	surface_cache_data_t m_arrSurfaceCache[2];

public:
	CPalette m_PaletteWide;

	CBrush m_brushLightDefault, m_brushLightSystem,
		m_brushDarkDefault, m_brushDarkSystem,
		m_brushLighterDefault, m_brushLighterSystem,
		m_brushDarkerDefault, m_brushDarkerSystem,
		m_brushLightestDefault, m_brushLightestSystem,
		m_brushDarkestDefault, m_brushDarkestSystem;

	// normal
	CFont m_FontNormal,m_FontNormalVert,m_FontNormalVertX;
	CFont m_FontBold,m_FontBoldVert,m_FontBoldVertX;
	// bar caption
	CFont m_FontNormalBC,m_FontNormalVertBC,m_FontNormalVertXBC;
	CFont m_FontBoldBC,m_FontBoldVertBC,m_FontBoldVertXBC;
	// marlett symbols
	CFont m_FontMarlett;

	int m_DSS_DISABLED_style;

	// global auto-pointer class for CExtPaintManager
	class __PROF_UIS_API CExtPaintManagerAutoPtr
	{
		CExtPaintManager * m_pPaintManager;
		
		bool m_bHaveScrollLines:1;
		UINT m_uCachedScrollLines;
		bool InitPaintManagerInstance();
	
	public:

		CExtSafeString m_strOsVer;
		OSVERSIONINFO m_osVerData;
		// quick boolean helpers
		bool
			  m_bIsWin32s:1			// old Win32s
			, m_bIsWin9x:1			// any Windows 9x
			, m_bIsWin98:1			// exactly Windows 98
			, m_bIsWin98orLater:1	// Windows 98 or later
			, m_bIsWinNT:1			// any NT
			, m_bIsWinNT4:1			// exactly Windows NT 4
			, m_bIsWin2000:1		// exactly Windows 2000
			, m_bIsWinXP:1			// exactly Windows XP
			, m_bIsWinNT4orLater:1	// Windows NT 4 or later
			, m_bIsWin2000orLater:1	// Windows 2000 or later
			, m_bIsWinXPorLater:1	// Windows XP or later
			;

		CExtPaintManagerAutoPtr();
		~CExtPaintManagerAutoPtr();

		CExtPaintManager * operator->();
		
		bool InstallPaintManager(
			CExtPaintManager * pPaintManager
			);
		bool InstallPaintManager(
			CRuntimeClass * pRtcPaintManager
			);

		UINT GetMouseWheelScrollLines();

		// uxtheme.dll access
		typedef enum THEMESIZE { TS_MIN, TS_TRUE, TS_DRAW, };
		typedef enum PROPERTYORIGIN { PO_STATE, PO_PART, PO_CLASS, PO_GLOBAL, PO_NOTFOUND };
		typedef struct _EXT_UX_MARGINS
		{
			int cxLeftWidth,cxRightWidth,cyTopHeight,cyBottomHeight;
		} EXT_UX_MARGINS, *PEXT_UX_MARGINS;
		typedef struct _EXT_UX_INTLIST
		{
			int iValueCount, iValues[__EXT_UX_MAX_INTLIST_COUNT];
		} EXT_UX_INTLIST, *PEXT_UX_INTLIST;
		typedef struct _EXT_UX_DTBGOPTS
		{
			DWORD dwSize,dwFlags;
			RECT rcClip;
		} EXT_UX_DTBGOPTS, *PEXT_UX_DTBGOPTS;

		typedef HANDLE EXT_UX_HTHEME;

		__EXT_UX_THEMEAPI_(EXT_UX_HTHEME, m_pUxApi_OpenThemeData)( HWND hwnd, LPCWSTR pszClassList );
		__EXT_UX_THEMEAPI( m_pUxApi_CloseThemeData)(EXT_UX_HTHEME hTheme);
		__EXT_UX_THEMEAPI( m_pUxApi_DrawThemeBackground)(EXT_UX_HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect);
		__EXT_UX_THEMEAPI( m_pUxApi_DrawThemeText)(EXT_UX_HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, const RECT *pRect);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemeBackgroundContentRect)(EXT_UX_HTHEME hTheme, HDC hdc, int iPartId, int iStateId,  const RECT *pBoundingRect, RECT *pContentRect);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemeBackgroundExtent)(EXT_UX_HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pContentRect, RECT *pExtentRect);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemePartSize)(EXT_UX_HTHEME hTheme, HDC hdc, int iPartId, int iStateId, RECT *prc, enum THEMESIZE eSize, SIZE *psz);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemeTextExtent)(EXT_UX_HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, const RECT *pBoundingRect, RECT *pExtentRect);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemeTextMetrics)(EXT_UX_HTHEME hTheme, HDC hdc, int iPartId, int iStateId, TEXTMETRIC* ptm);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemeBackgroundRegion)(EXT_UX_HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, HRGN *pRegion);
		__EXT_UX_THEMEAPI( m_pUxApi_HitTestThemeBackground)(EXT_UX_HTHEME hTheme, HDC hdc, int iPartId, int iStateId, DWORD dwOptions, const RECT *pRect, HRGN hrgn, POINT ptTest, WORD *pwHitTestCode);
		__EXT_UX_THEMEAPI( m_pUxApi_DrawThemeEdge)(EXT_UX_HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pDestRect, UINT uEdge, UINT uFlags, RECT *pContentRect);
		__EXT_UX_THEMEAPI( m_pUxApi_DrawThemeIcon)(EXT_UX_HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, HIMAGELIST himl, int iImageIndex);
		__EXT_UX_THEMEAPI_(BOOL, m_pUxApi_IsThemePartDefined)(EXT_UX_HTHEME hTheme, int iPartId, int iStateId);
		__EXT_UX_THEMEAPI_(BOOL, m_pUxApi_IsThemeBackgroundPartiallyTransparent)(EXT_UX_HTHEME hTheme, int iPartId, int iStateId);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemeColor)(EXT_UX_HTHEME hTheme, int iPartId, int iStateId, int iPropId, COLORREF *pColor);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemeMetric)(EXT_UX_HTHEME hTheme, HDC hdc, int iPartId, int iStateId, int iPropId, int *piVal);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemeString)(EXT_UX_HTHEME hTheme, int iPartId, int iStateId, int iPropId, LPWSTR pszBuff, int cchMaxBuffChars);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemeBool)(EXT_UX_HTHEME hTheme, int iPartId, int iStateId, int iPropId, BOOL *pfVal);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemeInt)(EXT_UX_HTHEME hTheme, int iPartId, int iStateId, int iPropId, int *piVal);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemeEnumValue)(EXT_UX_HTHEME hTheme, int iPartId, int iStateId, int iPropId,int *piVal);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemePosition)(EXT_UX_HTHEME hTheme, int iPartId, int iStateId, int iPropId,POINT *pPoint);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemeFont)(EXT_UX_HTHEME hTheme, HDC hdc, int iPartId, int iStateId, int iPropId,LOGFONT *pFont);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemeRect)(EXT_UX_HTHEME hTheme, int iPartId, int iStateId, int iPropId,RECT *pRect);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemeMargins)(EXT_UX_HTHEME hTheme, HDC hdc, int iPartId, int iStateId, int iPropId, RECT *prc,EXT_UX_MARGINS *pMargins);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemeIntList)(EXT_UX_HTHEME hTheme, int iPartId, int iStateId, int iPropId,EXT_UX_INTLIST *pIntList);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemePropertyOrigin)(EXT_UX_HTHEME hTheme, int iPartId, int iStateId, int iPropId,enum PROPERTYORIGIN *pOrigin);
		__EXT_UX_THEMEAPI( m_pUxApi_SetWindowTheme)(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemeFilename)(EXT_UX_HTHEME hTheme, int iPartId, int iStateId, int iPropId,LPWSTR pszThemeFileName, int cchMaxBuffChars);
		__EXT_UX_THEMEAPI_(COLORREF, m_pUxApi_GetThemeSysColor)(EXT_UX_HTHEME hTheme, int iColorId);
		__EXT_UX_THEMEAPI_(HBRUSH, m_pUxApi_GetThemeSysColorBrush)(EXT_UX_HTHEME hTheme, int iColorId);
		__EXT_UX_THEMEAPI_(BOOL, m_pUxApi_GetThemeSysBool)(EXT_UX_HTHEME hTheme, int iBoolId);
		__EXT_UX_THEMEAPI_(int, m_pUxApi_GetThemeSysSize)(EXT_UX_HTHEME hTheme, int iSizeId);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemeSysFont)(EXT_UX_HTHEME hTheme, int iFontId,LOGFONT *plf);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemeSysString)(EXT_UX_HTHEME hTheme, int iStringId, LPWSTR pszStringBuff, int cchMaxStringChars);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemeSysInt)(EXT_UX_HTHEME hTheme, int iIntId, int *piValue);
		__EXT_UX_THEMEAPI_(BOOL, m_pUxApi_IsThemeActive)();
		__EXT_UX_THEMEAPI_(BOOL, m_pUxApi_IsAppThemed)();
		__EXT_UX_THEMEAPI_(EXT_UX_HTHEME, m_pUxApi_GetWindowTheme)(HWND hwnd);
		__EXT_UX_THEMEAPI( m_pUxApi_EnableThemeDialogTexture)(HWND hwnd, DWORD dwFlags);
		__EXT_UX_THEMEAPI_(BOOL, m_pUxApi_IsThemeDialogTextureEnabled)(HWND hwnd);
		__EXT_UX_THEMEAPI_(DWORD, m_pUxApi_GetThemeAppProperties)();
		__EXT_UX_THEMEAPI_(void, m_pUxApi_SetThemeAppProperties)(DWORD dwFlags);
		__EXT_UX_THEMEAPI( m_pUxApi_GetCurrentThemeName)(LPWSTR pszThemeFileName, int cchMaxNameChars, LPWSTR pszColorBuff, int cchMaxColorChars, LPWSTR pszSizeBuff, int cchMaxSizeChars);
		__EXT_UX_THEMEAPI( m_pUxApi_GetThemeDocumentationProperty)(LPCWSTR pszThemeName, LPCWSTR pszPropertyName,LPWSTR pszValueBuff, int cchMaxValChars);
		__EXT_UX_THEMEAPI( m_pUxApi_DrawThemeParentBackground)(HWND hwnd, HDC hdc, RECT* prc);
		__EXT_UX_THEMEAPI( m_pUxApi_EnableTheming)(BOOL fEnable);
		__EXT_UX_THEMEAPI( m_pUxApi_DrawThemeBackgroundEx)(EXT_UX_HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, const EXT_UX_DTBGOPTS *pOptions);

		void InitUserExApi();
		HINSTANCE m_hInstUserExTheme;
		bool m_bUxApiInitPassed:1,
			m_bUxAppThemed:1,
			m_bUxValidColorsExtracted:1,
			m_bUxUseIfAvailOnWinXpOrLater:1;
		COLORREF
			m_clrUxTaskBandEdgeLightColor,
			m_clrUxTaskBandEdgeHighLightColor,
			m_clrUxTaskBandEdgeShadowColor,
			m_clrUxTaskBandEdgeDkShadowColor,
			m_clrUxTaskBandEdgeFillColor,
			m_clrUxTaskBandFillColorHint,
			m_clrUxTaskBandAccentColorHint;

		bool IsWndUpdateSource(
			CWnd * pWndNotifySrc
			);
	
		void OnSysColorChange(
			CWnd * pWndNotifySrc
			);
		void OnSettingChange(
			CWnd * pWndNotifySrc,
			UINT uFlags,
			__EXT_MFC_SAFE_LPCTSTR lpszSection
			);
		void OnDisplayChange(
			CWnd * pWndNotifySrc,
			INT nDepthBPP,
			CPoint ptSizes
			);
		void OnThemeChanged(
			CWnd * pWndNotifySrc,
			WPARAM wParam,
			LPARAM lParam
			);
	}; // class CExtPaintManagerAutoPtr

	// construction/destruction
	CExtPaintManager();
	virtual ~CExtPaintManager();

	// dc cache
	CDC & CachedDcGet(
		HGDIOBJ hBmp,
		INT nIdx = 0
		);
	CDC & CachedDcGet(
		CBitmap & bmp,
		INT nIdx = 0
		)
	{
		HGDIOBJ hBmp = bmp.GetSafeHandle();
		return CachedDcGet( hBmp, nIdx );
	}
	void CachedDcFreeAll();

	// paint session complete event - free all cached objects
	virtual void OnPaintSessionComplete(
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	// init part of color table and system brushes table
	bool m_bUseTahomaFont:1; // uses in SyncSysColors() to re-init font
	void SyncSysColors();

	// get any system color based brush
	virtual HBRUSH GetBrush( int nColorIndex );

	// get any system or translated color value
	virtual COLORREF GetColor( int nColorIndex );
	virtual COLORREF GetMenuFrameFillColor()
	{
		return GetColor(CLR_3DFACE_OUT);
	};
	
	virtual COLORREF GetShadowAdjustColor();
	virtual void FixedBar_AdjustClientRect(
		CExtControlBar * pBar,
		CRect & rcPreCalc
		);
	virtual INT FixedBar_GetRowDistance(
		CExtToolControlBar * pBar
		);
	virtual bool FixedBar_IsPaintRowBkMode(
		CExtToolControlBar * pBar
		);

	// install new color
	int InstallColor(
		COLORREF clr,
		int nColorIndex = -1
		);
	
	// install required translated color as
	// described in e_translated_colors_t enumeration
	virtual void InitTranslatedColors();


	// used always by default used translated colors
	enum e_translated_colors_t
	{
		CLR_3DFACE_OUT = __ExtMfc_COLOR_MAP_BASE+0,
		CLR_3DFACE_IN = __ExtMfc_COLOR_MAP_BASE+1,
		CLR_3DFACE_DISABLED = CLR_3DFACE_OUT+__ExtMfc_DISABLED_COLOR_SHIFT,

		CLR_3DLIGHT_OUT = __ExtMfc_COLOR_MAP_BASE+2,
		CLR_3DLIGHT_IN = __ExtMfc_COLOR_MAP_BASE+3,
		CLR_3DLIGHT_DISABLED = CLR_3DLIGHT_OUT+__ExtMfc_DISABLED_COLOR_SHIFT,

		CLR_3DHILIGHT_OUT = __ExtMfc_COLOR_MAP_BASE+4,
		CLR_3DHILIGHT_IN = __ExtMfc_COLOR_MAP_BASE+5,
		CLR_3DHILIGHT_DISABLED = CLR_3DHILIGHT_OUT+__ExtMfc_DISABLED_COLOR_SHIFT,

		CLR_3DSHADOW_OUT = __ExtMfc_COLOR_MAP_BASE+6,
		CLR_3DSHADOW_IN = __ExtMfc_COLOR_MAP_BASE+7,
		CLR_3DSHADOW_DISABLED = CLR_3DSHADOW_OUT+__ExtMfc_DISABLED_COLOR_SHIFT,

		CLR_3DDKSHADOW_OUT = __ExtMfc_COLOR_MAP_BASE+8,
		CLR_3DDKSHADOW_IN = __ExtMfc_COLOR_MAP_BASE+9,
		CLR_3DDKSHADOW_DISABLED = CLR_3DDKSHADOW_OUT+__ExtMfc_DISABLED_COLOR_SHIFT,
		
		CLR_TEXT_OUT = __ExtMfc_COLOR_MAP_BASE+10,
		CLR_TEXT_IN = __ExtMfc_COLOR_MAP_BASE+11,
		CLR_TEXT_DISABLED = CLR_TEXT_OUT+__ExtMfc_DISABLED_COLOR_SHIFT,
	};

	// align types
	enum e_align_t
	{
		__ALIGN_HORIZ_CENTER = 1,
		__ALIGN_HORIZ_RIGHT = 2,
		__ALIGN_VERT = 4
	};

	// gets the pushed metic offset
	virtual CSize GetPushedOffset();

	class __PROF_UIS_API glyph_t
	{
		CSize m_size; // size in pixels
		UINT m_nColors; // count of used indexes (including 0)
			// index 0 - transparent
		const UINT * m_pColorIndexes;
		bool m_bDynamicData;
	public:
		glyph_t()
		{
			_Init();
		};
		glyph_t(
			const SIZE & _size,
			UINT nColors,
			const UINT * pColorIndexes
			)
		{
			m_bDynamicData = false;

			ASSERT( _size.cx > 0 );
			ASSERT( _size.cy > 0 );
			m_size.cx = _size.cx;
			m_size.cy = _size.cy;
			
			ASSERT( nColors > 0 );
			m_nColors = nColors;
			
			ASSERT( pColorIndexes != NULL );
			m_pColorIndexes = pColorIndexes;
		};
		glyph_t(
			int cx,
			int cy,
			UINT nColors,
			const UINT * pColorIndexes
			)
		{
			m_bDynamicData = false;
			
			ASSERT( cx > 0 );
			ASSERT( cy > 0 );
			m_size.cx = cx;
			m_size.cy = cy;
			
			ASSERT( nColors > 0 );
			m_nColors = nColors;
		
			ASSERT( pColorIndexes != NULL );
			m_pColorIndexes = pColorIndexes;
		};
		glyph_t(
			const glyph_t & other,
			UINT nRotateAngle = 0
			)
		{
			_Init();
			_CopyFromOther(other);
#ifdef _DEBUG
			if( other.m_pColorIndexes != NULL )
			{
				ASSERT( m_pColorIndexes != NULL );
				ASSERT( m_pColorIndexes != other.m_pColorIndexes );
				ASSERT( m_bDynamicData );
			}
#endif // _DEBUG
			ASSERT(
				nRotateAngle == 0
				|| nRotateAngle == 90
				|| nRotateAngle == 180
				|| nRotateAngle == 270
				);
			Rotate(nRotateAngle);
		};
		~glyph_t()
		{
			_Done();
		};
		const glyph_t & operator=( const glyph_t & other )
		{
			_CopyFromOther(other);
#ifdef _DEBUG
			if( other.m_pColorIndexes != NULL )
			{
				ASSERT( m_pColorIndexes != NULL );
				ASSERT( m_pColorIndexes != other.m_pColorIndexes );
				ASSERT( m_bDynamicData );
			}
#endif // _DEBUG
			return *this;
		};

		UINT GetColorPixel(
			int x,
			int y
			) const
		{
			ASSERT( x >= 0 && x < m_size.cx );
			ASSERT( y >= 0 && y < m_size.cy );
			ASSERT( m_size.cx > 0 );
			ASSERT( m_size.cy > 0 );
			ASSERT( m_nColors > 0 );
			ASSERT( m_pColorIndexes != NULL );
			int nOffset =
				y * m_size.cx + x;
			UINT nValue =
				*( m_pColorIndexes + nOffset );
			return nValue;
		};

		void Rotate(UINT nRotateAngle)
		{
			ASSERT(
				nRotateAngle == 0
				|| nRotateAngle == 90
				|| nRotateAngle == 180
				|| nRotateAngle == 270
				);
			ASSERT( m_bDynamicData );
			ASSERT( m_size.cx > 0 );
			ASSERT( m_size.cy > 0 );
			ASSERT( m_nColors > 0 );
			ASSERT( m_pColorIndexes != NULL );
			if( nRotateAngle == 0 )
				return;
			
			int nBufferSize =
				m_size.cx * m_size.cy;
			ASSERT( nBufferSize > 0 );
			UINT * pNew =
				new/*(std::nothrow)*/ UINT[nBufferSize];
			/*
				(UINT *)::calloc(
					nBufferSize,
					sizeof(UINT)
					);
			*/
			ASSERT( pNew != NULL );
			if( pNew == NULL )
				return;

			int x, y;
			UINT * pOld = (UINT *)m_pColorIndexes;
			switch( nRotateAngle )
			{
			case 90:
			{
				for( y = 0; y<m_size.cy; y++ )
				{
					for( x = 0; x<m_size.cx; x++, pOld++ )
					{
						int nNewOffset =
							x*m_size.cy + (m_size.cy - y - 1);
						ASSERT(
							nNewOffset >= 0
							&&
							nNewOffset < nBufferSize
							);
						UINT * p = pNew + nNewOffset;
						*p = *pOld;
					} // for( x = 0; x<m_size.cx; x++, pOld++ )
				} // for( y = 0; y<m_size.cy; y++ )
				m_size = CSize(m_size.cy,m_size.cx);
			}
			break; // case 90
			case 180:
			{
				for( y = 0; y<m_size.cy; y++ )
				{
					for( x = 0; x<m_size.cx; x++, pOld++ )
					{
						int nNewOffset =
							(m_size.cy-y-1)*m_size.cx
							+ (m_size.cx-x-1);
						ASSERT(
							nNewOffset >= 0
							&&
							nNewOffset < nBufferSize
							);
						UINT * p = pNew + nNewOffset;
						*p = *pOld;
					} // for( x = 0; x<m_size.cx; x++, pOld++ )
				} // for( y = 0; y<m_size.cy; y++ )
			}
			break; // case 180
			case 270:
			{
				for( y = 0; y<m_size.cy; y++ )
				{
					for( x = 0; x<m_size.cx; x++, pOld++ )
					{
						int nNewOffset =
							(m_size.cx-x-1)*m_size.cy + y;
						ASSERT(
							nNewOffset >= 0
							&&
							nNewOffset < nBufferSize
							);
						UINT * p = pNew + nNewOffset;
						*p = *pOld;
					} // for( x = 0; x<m_size.cx; x++, pOld++ )
				} // for( y = 0; y<m_size.cy; y++ )
				m_size = CSize(m_size.cy,m_size.cx);
			}
			break; // case 270
			} // switch( nRotateAngle )
			::memcpy(
				(UINT *)m_pColorIndexes,
				pNew,
				nBufferSize*sizeof(UINT)
				);
			delete [] pNew; // ::free(pNew);
		};
		
		CSize Size() const
		{
			return m_size;
		};
		operator CSize() const
		{
			return Size();
		};
		
		const UINT * GetSurface() const
		{
			ASSERT( m_size.cx > 0 );
			ASSERT( m_size.cy > 0 );
			ASSERT( m_nColors > 0 );
			ASSERT( m_pColorIndexes != NULL );
			return m_pColorIndexes;
		};
		
		UINT GetColorsCount() const
		{
			ASSERT( m_size.cx > 0 );
			ASSERT( m_size.cy > 0 );
			ASSERT( m_nColors > 0 );
			ASSERT( m_pColorIndexes != NULL );
			return m_nColors;
		};

	private:
		void _Init()
		{
			m_size.cx = m_size.cy = 0;
			m_nColors = 1;
			m_pColorIndexes = NULL;
			m_bDynamicData = false;
		};
		void _Done()
		{
			if( m_bDynamicData )
			{
				ASSERT( m_pColorIndexes != NULL );
				delete [] ((UINT *)m_pColorIndexes); // ::free( (void *)m_pColorIndexes );
				m_pColorIndexes = NULL;
			}
			_Init();
		};
		bool _CopyFromOther( const glyph_t & other )
		{
			_Done();

			if( other.m_pColorIndexes != NULL )
			{
				int nBufferSize =
					other.m_size.cx * other.m_size.cy;
				ASSERT( nBufferSize > 0 );
				m_pColorIndexes =
					new/*(std::nothrow)*/ UINT[nBufferSize];
					/*
					(UINT *)::calloc(
						nBufferSize,
						sizeof(UINT)
						);
					*/
				ASSERT( m_pColorIndexes != NULL );
				if( m_pColorIndexes == NULL )
					return false;
				::memcpy(
					(void *)m_pColorIndexes,
					other.m_pColorIndexes,
					nBufferSize*sizeof(UINT)
					);
				m_bDynamicData = true;
			}
			m_size.cx = other.m_size.cx;
			m_size.cy = other.m_size.cy;
			
			m_nColors = other.m_nColors;

			return true;
		};
		void _SetColorPixel(
			int x,
			int y,
			UINT nValue
			)
		{
			ASSERT( x >= 0 && x < m_size.cx );
			ASSERT( y >= 0 && y < m_size.cy );
			ASSERT( m_bDynamicData );
			ASSERT( m_size.cx > 0 );
			ASSERT( m_size.cy > 0 );
			ASSERT( m_nColors > 0 );
			ASSERT( m_pColorIndexes != NULL );
			int nOffset =
				y * m_size.cx + x;
			UINT * ptr =
				(UINT *)m_pColorIndexes + nOffset;
			*ptr = nValue;
		};
	public:
	
		static bool stat_DefGlyphRgnTransparencyFunc(
			const glyph_t & _glyph,
			int x,
			int y,
			UINT nColor,
			LPVOID lpCoockie
			);
		CRgn & GenerateRgn(
			CRgn & rgn,
			bool (*pGlyphRgnTransparencyFunc)(
				const glyph_t & _glyph,
				int x,
				int y,
				UINT nColor,
				LPVOID lpCoockie
				) = stat_DefGlyphRgnTransparencyFunc,
			LPVOID lpCallbackCoockie = NULL,
			LPLONG pMinNonTransparentX = NULL,
			LPLONG pMinNonTransparentY = NULL,
			LPLONG pMaxNonTransparentX = NULL,
			LPLONG pMaxNonTransparentY = NULL,
			LPLONG pMinTransparentX = NULL,
			LPLONG pMinTransparentY = NULL,
			LPLONG pMaxTransparentX = NULL,
			LPLONG pMaxTransparentY = NULL
			) const
		{
			if( pMinNonTransparentX != NULL )
				*pMinNonTransparentX = 0;
			if( pMinNonTransparentY != NULL )
				*pMinNonTransparentY = 0;
			if( pMaxNonTransparentX != NULL )
				*pMaxNonTransparentX = 0;
			if( pMaxNonTransparentY != NULL )
				*pMaxNonTransparentY;

			if( pMinTransparentX != NULL )
				*pMinTransparentX = 0;
			if( pMinTransparentY != NULL )
				*pMinTransparentY = 0;
			if( pMaxTransparentX != NULL )
				*pMaxTransparentX = 0;
			if( pMaxTransparentY != NULL )
				*pMaxTransparentY;

			if( rgn.GetSafeHandle() != NULL )
				rgn.DeleteObject();
			ASSERT( rgn.GetSafeHandle() == NULL );
			
			CSize _size = Size();
			if( _size.cx == 0 || _size.cy == 0 )
			{
				ASSERT( FALSE );
				return rgn;
			}
			
			if( !rgn.CreateRectRgn(0,0,0,0) )
			{
				ASSERT( FALSE );
				return rgn;
			}
			
			
			for( int x = 0; x < _size.cx; x++ )
			{
				for( int y = 0; y < _size.cy; y++ )
				{
					UINT nColor = GetColorPixel( x, y );
					if( pGlyphRgnTransparencyFunc(
							*this,
							x,
							y,
							nColor,
							lpCallbackCoockie
							)
						)
					{ // if pixel transparent

						if( pMinTransparentX != NULL
							&& (*pMinTransparentX) > x
							)
							(*pMinTransparentX) = x;
						if( pMinTransparentY != NULL
							&& (*pMinTransparentY) > y
							)
							(*pMinTransparentY) = y;

						if( pMaxTransparentX != NULL
							&& (*pMaxTransparentX) < x
							)
							(*pMaxTransparentX) = x;
						if( pMaxTransparentY != NULL
							&& (*pMaxTransparentY) < y
							)
							(*pMaxTransparentY) = y;

						continue;
					} // if pixel transparent

					if( pMinNonTransparentX != NULL
						&& (*pMinNonTransparentX) > x
						)
						(*pMinNonTransparentX) = x;
					if( pMinNonTransparentY != NULL
						&& (*pMinNonTransparentY) > y
						)
						(*pMinNonTransparentY) = y;

					if( pMaxNonTransparentX != NULL
						&& (*pMaxNonTransparentX) < x
						)
						(*pMaxNonTransparentX) = x;
					if( pMaxNonTransparentY != NULL
						&& (*pMaxNonTransparentY) < y
						)
						(*pMaxNonTransparentY) = y;

					CRgn rgnPixel;
					if( !rgnPixel.CreateRectRgn(
							x,
							y,
							x+1,
							y+1
							)
						)
					{
						ASSERT( FALSE );
						return rgn;
					}
					rgn.CombineRgn( &rgn, &rgnPixel, RGN_OR );
				} // for( int y = 0; y < _size.cy; y++ )
			} // for( int x = 0; x < _size.cx; x++ )
			
			return rgn;
		};
	}; // class glyph_t

	static const glyph_t g_glyph_btn_expand_left;
	static const glyph_t g_glyph_btn_expand_right;
	static const glyph_t g_glyph_btn_expand_right2;
	static const glyph_t g_glyph_btn_expand_bottom;
	static const glyph_t g_glyph_btn_expand_bottom2;

	static const glyph_t g_glyph_btn_expand_left_2003;
	static const glyph_t g_glyph_btn_expand_right_2003;
	static const glyph_t g_glyph_btn_expand_right2_2003;
	static const glyph_t g_glyph_btn_expand_bottom_2003;
	static const glyph_t g_glyph_btn_expand_bottom2_2003;

	static const glyph_t g_glyph_customize_popup_src;
	static const glyph_t g_glyph_customize_dropdown_src;

	virtual void PaintGlyph(
		CDC & dc,
		POINT ptDest,
		const glyph_t & glyph,
		COLORREF * pColorValues
		);
	virtual void PaintGlyphCentered(
		CDC & dc,
		const RECT & rect,
		const glyph_t & glyph,
		COLORREF * pColorValues
		);

	virtual void PaintFloatToolbarRowBk(
		CDC & dc,
		CExtToolControlBar * pBar,
		int nLastReviewBtnIdx,
		CRect & rcRowBk
		);

	virtual void PaintToolbarExpandButton(
		CDC & dc,
		const RECT & rcButtonArea,
		bool bHorz, // if false - down
		bool bBarIsCompletelyVisible,
		bool bEnabled,
		bool bPushed,
		bool bHover,
		CObject * pHelperSrc,
		LPARAM lParam = 0L,
		bool bTransparentBackground = false
		);
	virtual void PaintMenuExpandButton(
		CDC & dc,
		const RECT & rectButton,
		bool bPushed,
		bool bHover,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual int GetMenuExpandButtonHeight();

	// statically implemented paint/helper algorithms

	static void stat_PassPaintMessages(
		bool bPassNcPaint = true,
		bool bPassEraseBkgnd = true,
		bool bPassPaint = true
		);

	static bool stat_ResizeBitmap(
		CBitmap & bmpSrc,
		CSize _size,
		CBrush & brushFill
		);
	static HICON stat_HBITMAP2HICON(
		HBITMAP hBmpSrc,
		COLORREF clrTransparent,
		const RECT * pRcPart = NULL,
		bool bReplaceSysColors = true
		);
	static bool stat_CloneBitmap(
		CBitmap & bmpDst,
		const CBitmap & bmpSrc,
		const RECT * pRcPart = NULL,
		COLORREF ** ppSurface = NULL // if not NULL - create 32 bit surface
		);
	static HBITMAP stat_CloneBitmap(
		HBITMAP hBmpSrc,
		const RECT * pRcPart = NULL,
		COLORREF ** ppSurface = NULL // if not NULL - create 32 bit surface
		);
	static INT stat_GetBitmapColorMap( // returns bitmap's color bit count or zero if error
		HBITMAP hBmpSrc,
		CMap < COLORREF, COLORREF, DWORD, DWORD > & _map // key is color, value is count of color entries in bitmap
		);
	static INT stat_GetBitmapColorArray( // returns bitmap's color bit count or zero if error
		HBITMAP hBmpSrc,
		CArray < COLORREF, COLORREF > & arr,
		bool bMakeUniqueColors = true // used only if bit count <= 8
		);
	static HGLOBAL stat_HBITMAPtoHGLOBAL(
		HBITMAP hBmpSrc,
		HPALETTE hPalDst = NULL
		);
	static bool stat_SerializeBitmap(
		CArchive & ar,
		HBITMAP * p_hBmp,
		DWORD dwResourceSize = 0L // not 0 only if loaded from resources
		);
	static bool stat_SerializeBitmap(
		__EXT_MFC_SAFE_LPCTSTR strFileName,
		bool bRead,
		HBITMAP * p_hBmp
		);
	static HBITMAP stat_LoadBitmapResource(
		HINSTANCE hInst,
		HRSRC hRsrc
		);
	static HBITMAP stat_LoadBitmapResource(
		__EXT_MFC_SAFE_LPCTSTR sResourceID,
		__EXT_MFC_SAFE_LPCTSTR sResourceType = RT_BITMAP, // default bitmap section
		HINSTANCE hInst = NULL // find it automatically
		);

	struct monitor_parms_t
	{
		CRect m_rcMonitor, m_rcWorkArea;
		bool m_bPrimaryMonitor:1;
		monitor_parms_t()
			: m_rcMonitor( 0, 0, 0, 0 )
			, m_rcWorkArea( 0, 0, 0, 0 )
			, m_bPrimaryMonitor( true )
		{
		}
	}; // struct monitor_parms_t
	static void stat_GetMonitorParms(
		monitor_parms_t & _mp
		);
	static void stat_GetMonitorParms(
		monitor_parms_t & _mp,
		const POINT & _ptScreenFrom
		);
	static void stat_GetMonitorParms(
		monitor_parms_t & _mp,
		const RECT & _rcScreenFrom
		);
	static void stat_GetMonitorParms(
		monitor_parms_t & _mp,
		HWND hWndFrom
		);
	static void stat_GetMonitorParms(
		monitor_parms_t & _mp,
		CWnd * pWndFrom
		);
	static CRect stat_AlignWndRectToMonitor(
		CRect rcInitial,
		bool bUseWorkArea = true, // if false - use all monitor area
		bool bNoPartialVisibility = false
		);

	static COLORREF stat_RGBtoCMY( COLORREF clr );
	static COLORREF stat_CMYtoRGB( COLORREF clr );

	//----------------------------------------------------------------------
	// Conversion between the HSL (Hue, Saturation, and Luminosity) 
	// and RBG color model.
	//----------------------------------------------------------------------
	// The conversion algorithms presented here come from the book by 
	// Fundamentals of Interactive Computer Graphics by Foley and van Dam. 
	// In the example code, HSL values are represented as floating point 
	// number in the range 0 to 1. RGB tridrants use the Windows convention 
	// of 0 to 255 of each element. 
	//----------------------------------------------------------------------
	static double stat_HuetoRGB(double m1, double m2, double h );
	static COLORREF stat_HLStoRGB( double H, double L, double S );
	static void stat_RGBtoHSL( COLORREF rgb, double *H, double *S, double *L );

	static BYTE stat_GetRGBFromHue(float rm1, float rm2, float rh);
	static COLORREF stat_GetRGBFromHLSExtend( double H, double L, double S );

	static COLORREF stat_HLS_Adjust(
		COLORREF clr,
		double percentH = 0.0,
		double percentL = 0.0,
		double percentS = 0.0
		);

	static BOOL stat_PaintParentBk(
		HWND hWnd,
		HDC hDC,
		LPCRECT rectClip = NULL
		);

	typedef bool (*pfnExcludeChildAreaCallback)(
		HDC hDC,
		HWND hWnd,
		HWND hWndChild,
		LPVOID pCookie
		);

	static void stat_ExcludeChildAreas(
		HDC hDC,
		HWND hWnd,
		pfnExcludeChildAreaCallback pCallback = NULL,
		LPVOID pCallbackCookie = NULL
		);

	static void stat_TransparentBlt(
		HDC hdcDest,        // handle to destination DC
		int nXOriginDest,   // x-coord of destination upper-left corner
		int nYOriginDest,   // y-coord of destination upper-left corner
		int nWidthDest,     // width of destination rectangle
		int hHeightDest,    // height of destination rectangle
		HDC hdcSrc,         // handle to source DC
		int nXOriginSrc,    // x-coord of source upper-left corner
		int nYOriginSrc,    // y-coord of source upper-left corner
		int nWidthSrc,      // width of source rectangle
		int nHeightSrc,     // height of source rectangle
		UINT crTransparent  // color to make transparent
		);

	// generate DIB for UI light brush (when BPP <=8)
	static HBITMAP stat_GenLBDIB(
		COLORREF clrDarker = COLORREF(-1), // by default COLORREF(-1) is changed to ::GetSysColor(COLOR_BTNFACE)
		COLORREF clrLighter = COLORREF(-1), // by default COLORREF(-1) is changed to ::GetSysColor(COLOR_BTNHIGHLIGHT)
		int nColorMaskHorz = 1,
		int nColorMaskVert = 1,
		int nSkipMaskHorz = 0,
		int nSkipMaskVert = 0,
		COLORREF clrSkip = COLORREF(-1L) // // default COLORREF(-1) is changed to clrDarker
		);

	static void stat_PaintGradientRect(
		CDC & dc,
		const CRect & rcPaintGradient,
		COLORREF clrLeft,
		COLORREF clrRight,
		bool bHorz = false,
		UINT nCountOfSteps = 256
		);

	static void stat_PaintDotNet3dRect(
		CDC & dc,
		LPCRECT lpRect,
		COLORREF clrTopLeft,
		COLORREF clrBottomRight
		);
	static void stat_PaintDotNet3dRect(
		CDC & dc,
		int x,
		int y,
		int cx,
		int cy,
		COLORREF clrTopLeft,
		COLORREF clrBottomRight
		);

	static void stat_PaintRadioDot(
		CDC & dc,
		int x,
		int y,
		COLORREF color
		);

	static void stat_PaintCheckMark(
		CDC & dc,
		int x,
		int y,
		COLORREF color
		);

	static void stat_PaintTabItemImpl(
		CDC & dc,
		CRect & rcTabItemsArea,
		bool bTopLeft,
		bool bHorz,
		bool bSelected,
		bool bCenteredText,
		bool bGroupedMode,
		bool bInGroupActive,
		bool bInvertedVerticalMode,
		const CRect & rcEntireItem,
		CSize sizeTextMeasured,
		CFont * pFont,
		__EXT_MFC_SAFE_LPCTSTR sText,
		CExtCmdIcon * pIcon,
		COLORREF clrText,
		COLORREF clrTabBk,
		COLORREF clrTabBorderLT,
		COLORREF clrTabBorderRB,
		COLORREF clrTabSeparator
		);

	static void stat_PaintTabButtonImpl(
		CDC & dc,
		CRect & rcButton,
		LONG nHitTest,
		bool bTopLeft,
		bool bHorz,
		bool bEnabled,
		bool bHover,
		bool bPushed,
		bool bGroupedMode,
		COLORREF clrGlyph,
		COLORREF clrTL,
		COLORREF clrBR
		);

	static void stat_PaintTabClientAreaImpl(
		CDC & dc,
		CRect & rcClient,
		CRect & rcTabItemsArea,
		CRect & rcTabNearBorderArea,
		DWORD dwOrientation,
		bool bGroupedMode,
		COLORREF clrTabItemsAreaBk,
		COLORREF clrTabNearBorderAreaBk,
		COLORREF clrTabNearMargin
		);

	virtual void PaintTabItem(
		CDC & dc,
		CRect & rcTabItemsArea,
		bool bTopLeft,
		bool bHorz,
		bool bSelected,
		bool bCenteredText,
		bool bGroupedMode,
		bool bInGroupActive,
		bool bInvertedVerticalMode,
		const CRect & rcEntireItem,
		CSize sizeTextMeasured,
		CFont * pFont,
		__EXT_MFC_SAFE_LPCTSTR sText,
		CExtCmdIcon * pIcon,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintTabClientArea(
		CDC & dc,
		CRect & rcClient,
		CRect & rcTabItemsArea,
		CRect & rcTabNearBorderArea,
		DWORD dwOrientation,
		bool bGroupedMode,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	
	virtual void PaintTabButton(
		CDC & dc,
		CRect & rcButton,
		LONG nHitTest,
		bool bTopLeft,
		bool bHorz,
		bool bEnabled,
		bool bHover,
		bool bPushed,
		bool bGroupedMode,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintTabNcAreaRect(
		CDC & dc,
		const RECT & rc,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	
	virtual void PaintResizableBarChildNcAreaRect(
		CDC & dc,
		const RECT & rc,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	static CRect stat_CalcTextDimension(
		CDC & dc,
		CFont & font,
		const CExtSafeString & strText
		);
	static int stat_CalcTextWidth(
		CDC & dc,
		CFont & font,
		const CExtSafeString & strText
		);

	// paint methods

	virtual void PaintIcon(
		CDC & dc,
		bool bHorz,
		CExtSafeString & sTitle,
		CExtCmdIcon * pIcon,
		const CRect & rectItem,
		CRect & rectCaption,
		bool bPushed,
		bool bEnabled,
		bool bHover,
		int eAlign
		);

	enum e_docking_caption_button_t
	{
		__DCBT_EMPTY				= 0,
		__DCBT_CLOSE				= 1,
		__DCBT_ARROW_UP				= 2,
		__DCBT_ARROW_DOWN			= 3,
		__DCBT_ARROW_LEFT			= 4,
		__DCBT_ARROW_RIGHT			= 5,
		__DCBT_WND_MINIMIZE			= 6,
		__DCBT_WND_RESTORE			= 7,
		__DCBT_WND_MAXIMIZE			= 8,
		__DCBT_WND_CONTEXTHELP		= 9,
		__DCBT_WND_KEEP				= 10,
		__DCBT_WND_UNKEEP			= 11,
		__DCBT_ARROW_UP_L			= 12,
		__DCBT_ARROW_DOWN_L			= 13,
		__DCBT_ARROW_LEFT_L			= 14,
		__DCBT_ARROW_RIGHT_L		= 15,
		__DCBT_XP_THIN_CLOSE		= 16,
		__DCBT_HOLLOW_UP			= 17,
		__DCBT_HOLLOW_DOWN			= 18,
		__DCBT_HOLLOW_LEFT			= 19,
		__DCBT_HOLLOW_RIGHT			= 20,
		__DCBT_CLOSE_DC2K			= 21,
		__DCBT_HOLLOW_PLUS			= 22,
		__DCBT_HOLLOW_MINUS			= 23,
		__DCBT_TREE_PLUS			= 24,
		__DCBT_TREE_MINUS			= 25,
		__DCBT_CIRCLE_PLUS			= 26,
		__DCBT_CIRCLE_MINUS			= 27,
		__DCBT_ARROW_PT_UP			= 28,
		__DCBT_ARROW_PT_DOWN		= 29,
		__DCBT_ARROW_PT_LEFT		= 30,
		__DCBT_ARROW_PT_RIGHT		= 31,
		__DCBT_ARROW_PT_LT			= 32,
		__DCBT_ARROW_PT_RT			= 33,
		__DCBT_ARROW_PT_RB			= 34,
		__DCBT_ARROW_PT_LB			= 35,
		__DCBT_ARROW_SMALL_UP		= 36,
		__DCBT_ARROW_SMALL_DOWN		= 37,
		__DCBT_ARROW_SMALL_LEFT		= 38,
		__DCBT_ARROW_SMALL_RIGHT	= 39,
		__DCBT_ELLIPSIS				= 40,
		__DCBT_ARROW_SORT_UP		= 41,
		__DCBT_ARROW_SORT_DOWN		= 42,
		__DCBT_ARROW_SORT_LEFT		= 43,
		__DCBT_ARROW_SORT_RIGHT		= 44,

		__DCBT_VALUE_MIN			= 0,
		__DCBT_VALUE_MAX			= 44,
	};
	
	static glyph_t * g_DockingCaptionGlyphs[__DCBT_VALUE_MAX+1];

	struct __PROF_UIS_API PAINTDOCKINGCAPTIONBUTTONDATA
	{
		CObject * m_pHelperSrc;
		LPARAM m_lParam;
		CRect m_rcClient;
		int m_eType; // CExtPaintManager::e_docking_caption_button_t
		bool m_bHover:1;
		bool m_bPushed:1;
		bool m_bEnabled:1;
		bool m_bSideBar:1;
		bool m_bBarWndActive:1;
		bool m_bFloating:1;

		PAINTDOCKINGCAPTIONBUTTONDATA();
		PAINTDOCKINGCAPTIONBUTTONDATA(
			CObject * pHelperSrc,
			const RECT & rcClient,
			int eType, // CExtPaintManager::e_docking_caption_button_t
			bool bHover,
			bool bPushed,
			bool bEnabled,
			bool bSideBar,
			bool bBarWndActive,
			bool bFloating
			);
	}; // struct PAINTDOCKINGCAPTIONBUTTONDATA

	virtual void PaintDockingCaptionButton(
		CDC & dc,
		CExtPaintManager::PAINTDOCKINGCAPTIONBUTTONDATA & _pdcbd
		);

	virtual void PaintSysCmdGlyph(
		CDC & dc,
		UINT nHelperSysCmdID,
		const RECT & rcItem,
		bool bHover,
		bool bPushed,
		bool bEnabled
		);

	struct __PROF_UIS_API PAINTPUSHBUTTONDATA
	{
		CObject * m_pHelperSrc;
		LPARAM m_lParam;
		CRect m_rcClient;
		__EXT_MFC_SAFE_LPCTSTR m_sText;
		CExtCmdIcon * m_pIcon;
		int m_eAlign;
		HFONT m_hFont;
		UINT m_nHelperSysCmdID;

		bool m_bHorz:1;
		bool m_bFlat:1;
		bool m_bHover:1;
		bool m_bPushed:1;
		bool m_bIndeterminate:1;
		bool m_bEnabled:1;
		bool m_bDrawBorder:1;
		bool m_bDrawFocusRect:1;
		bool m_bDefaultPushButton:1;
		bool m_bDropDown:1;
		bool m_bTransparentBackground:1;
		bool m_bSeparatedDropDown:1;
		bool m_bPushedDropDown:1;

		// custom colors
		COLORREF m_clrForceTextDisabled, m_clrForceTextNormal,
			m_clrForceTextHover, m_clrForceTextPressed;
		UINT m_nDrawTextFlagsH;
		bool m_bForceUseDrawTextApiH:1;

		PAINTPUSHBUTTONDATA();
		PAINTPUSHBUTTONDATA(
			CObject * pHelperSrc,
			bool bHorz,
			const RECT & rcClient,
			__EXT_MFC_SAFE_LPCTSTR sText,
			CExtCmdIcon * pIcon,
			bool bFlat,
			bool bHover,
			bool bPushed,
			bool bIndeterminate,
			bool bEnabled,
			bool bDrawBorder,
			bool bDrawFocusRect,
			bool bDefaultPushButton,
			int eAlign,
			HFONT hFont = NULL,
			bool bDropDown = false,
			UINT nHelperSysCmdID = 0,
			bool bTransparentBackground = false
			);
	}; // struct PAINTPUSHBUTTONDATA

	virtual void PaintPushButton(
		CDC & dc,
		CExtPaintManager::PAINTPUSHBUTTONDATA & _ppbd
		);

	virtual CSize GetDropDividerMerics();
	virtual void PaintDropDivider(
		CDC & dc,
		const RECT & rcItem,
		bool bBefore,
		bool bHorz
		);
	virtual void PaintDragSourceRect(
		CDC & dc,
		const RECT & rcItem
		);

	struct __PROF_UIS_API PAINTMENUITEMDATA
	{
		CObject * m_pHelperSrc;
		LPARAM m_lParam;
		CRect m_rcItem;
		INT m_nIconAreaWidth;
		__EXT_MFC_SAFE_LPCTSTR m_sText, m_sAccelText;
		CExtCmdIcon * m_pIcon;
		UINT m_nHelperSysCmdID; // for painting system menu glyphs

		bool m_bPopup:1;
		bool m_bSelected:1;
		bool m_bPushedCheck:1;
		bool m_bPushedRadio:1;
		bool m_bIndeterminate:1;
		bool m_bEnabled:1;
		bool m_bRarelyUsed:1;
		bool m_bRarelyUsedPrev:1;
		bool m_bRarelyUsedNext:1;
		bool m_bForceNoLeftGradient:1;
		bool m_bCustomizeListMode:1;
		bool m_bSeparatedDropDown:1;
		bool m_bXtraMark:1;
		bool m_bXtraChecked:1;
		CRect m_rcInPlaceWnd;
		CExtSafeString m_strInPlaceText;
		bool m_bHelperPopupDropped:1;

		PAINTMENUITEMDATA();
		PAINTMENUITEMDATA(
			CObject * pHelperSrc,
			const RECT & rcItem,
			INT nIconAreaWidth,
			__EXT_MFC_SAFE_LPCTSTR sText,
			__EXT_MFC_SAFE_LPCTSTR sAccelText,
			CExtCmdIcon * pIcon,
			bool bPopup,
			bool bSelected,
			bool bPushedCheck,
			bool bPushedRadio,
			bool bIndeterminate,
			bool bEnabled,
			bool bRarelyUsed,
			bool bRarelyUsedPrev,
			bool bRarelyUsedNext,
			UINT nHelperSysCmdID // for painting system menu glyphs
			);
	}; // struct PAINTMENUITEMDATA

	virtual void PaintMenuItem(
		CDC & dc,
		CExtPaintManager::PAINTMENUITEMDATA & _pmid
		);
	virtual void PaintMenuSeparator(
		CDC & dc,
		const RECT & rectItem,
		int nIconAreaWidth,
		bool bRarelyUsed,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintSeparator(
		CDC & dc,
		const RECT & rectItem,
		bool bHorz,
		bool bTransparentBk,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	struct __PROF_UIS_API PAINTGRIPPERDATA
	{
		CObject * m_pHelperSrc;
		LPARAM m_lParam;
		const CRect m_rcGripper, m_rcText;
		bool m_bActive:1;
		bool m_bFloating:1;
		bool m_bHorz:1;
		bool m_bSideBar:1;
		__EXT_MFC_SAFE_LPCTSTR m_sCaption;
		bool m_bHelperNoRect:1;
		bool m_bHelperNoFill:1;

		PAINTGRIPPERDATA();
		PAINTGRIPPERDATA(
			CObject * pHelperSrc,
			const RECT & rcGripper,
			const RECT & rcText,
			bool bActive,
			bool bFloating,
			bool bHorz,
			bool bSideBar = false,
			__EXT_MFC_SAFE_LPCTSTR sCaption = NULL
			);
	}; // struct PAINTGRIPPERDATA

	virtual void PaintGripper(
		CDC & dc,
		CExtPaintManager::PAINTGRIPPERDATA & _pgd
		);

	struct __PROF_UIS_API PAINTDOCKINGFRAMEDATA
	{
		CObject * m_pHelperSrc;
		LPARAM m_lParam;
		CRect m_rcWindow, m_rcClient;
		bool m_bFloating:1;
		bool m_bExtBar:1;
		bool m_bSideBar:1;

		PAINTDOCKINGFRAMEDATA();
		PAINTDOCKINGFRAMEDATA(
			CObject * pHelperSrc,
			const RECT & rcWindow,
			const RECT & rcClient,
			bool bFloating,
			bool bExtBar,
			bool bSideBar = false
			);
	}; // struct PAINTDOCKINGFRAMEDATA

	virtual void PaintDockingFrame(
		CDC & dc,
		CExtPaintManager::PAINTDOCKINGFRAMEDATA & _pdfd
		);

	struct __PROF_UIS_API PAINTCONTROLFRAMEDATA
	{
		CObject * m_pHelperSrc;
		LPARAM m_lParam;
		CRect m_rcClient;
		bool m_bFlat:1;
		bool m_bHover:1;
		bool m_bEnabled:1;
		bool m_bFocus:1;

		PAINTCONTROLFRAMEDATA();
		PAINTCONTROLFRAMEDATA(
			CObject * pHelperSrc,
			const RECT & rcClient,
			bool bFlat,
			bool bHover,
			bool bEnabled,
			bool bFocus
			);
	}; // struct PAINTCONTROLFRAMEDATA

	virtual void PaintControlFrame(
		CDC & dc,
		CExtPaintManager::PAINTCONTROLFRAMEDATA & _pcfd
		);

	struct __PROF_UIS_API PAINTCOMBOFRAMEDATA
	{
		CObject * m_pHelperSrc;
		LPARAM m_lParam;
		CRect m_rcClient;
		bool m_bHover:1;
		bool m_bPushed:1;
		bool m_bEnabled:1;
		COLORREF
			m_clrForceNABorder,
			m_clrForceNAContent,
			m_clrForceActiveOuterBorder
			;
		bool m_bNoTopOuterBorder:1;

		PAINTCOMBOFRAMEDATA();
		PAINTCOMBOFRAMEDATA(
			CObject * pHelperSrc,
			const RECT & rcClient,
			bool bHover,
			bool bPushed,
			bool bEnabled
			);
	}; // struct PAINTCOMBOFRAMEDATA

	virtual void PaintComboFrame(
		CDC &dc,
		CExtPaintManager::PAINTCOMBOFRAMEDATA & _pcfd
		);

	static HICON stat_GenerateColorIconBox(
		COLORREF clrIcon
		);
	static HICON stat_GenerateColorIconChar(
		COLORREF clrIcon,
		HFONT hFontForLetter,
		__EXT_MFC_SAFE_TCHAR tChr = _T('A')
		);
	static HICON stat_GenerateColorIconFrame(
		COLORREF clrIcon
		);

	static bool stat_DefIsHwndNeedsDirectRepaint(
		HWND hWndChild
		);
	static bool stat_DefExcludeChildAreaCallback(
		HDC hDC,
		HWND hWnd,
		HWND hWndChild,
		LPVOID pCookie
		);

	static int stat_GetBPP();

	CSize GetTextSizes(bool bBold = false);
	virtual int GetTextHeight(bool bHorz);

	virtual int GetMenuShadowSize();
	virtual int GetMenuBorderSize();
	virtual void PaintMenuBorder(
		CDC & dc,
		const RECT & rectClient,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual bool IsMenuMustCombineExcludeArea();
	virtual void PaintMenuCombinedArea(
		CDC & dc,
		const RECT & rcArea,
		const RECT & rcClient,
		int eCombineAlign, // CExtPopupMenuWnd::e_combine_align_t values
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	enum e_control_bar_border_type_t
	{
		__CB_OUTER_DOCKBAR = 0,
		__CB_OUTER_STATUSBAR = 1,
		__CB_INNER_STATUSBAR_ITEM = 2,
	};

	struct __PROF_UIS_API PAINTCONTROLBARBORDERSDATA
	{
		CObject * m_pHelperSrc;
		LPARAM m_lParam;

		int m_eCBBT; // CExtPaintManager::e_control_bar_border_type_t
		DWORD m_dwStyle;
		CRect m_rc;

		PAINTCONTROLBARBORDERSDATA();
		PAINTCONTROLBARBORDERSDATA(
			CObject * pHelperSrc,
			int eCBBT, // CExtPaintManager::e_control_bar_border_type_t
			DWORD dwStyle,
			const RECT & rc
			);
	}; // struct PAINTCONTROLBARBORDERSDATA
	
	void PaintControlBarBordersDefImpl(
		CDC & dc,
		CExtPaintManager::PAINTCONTROLBARBORDERSDATA & _pcbbd,
		COLORREF clrHi,
		COLORREF clrLo
		);

	virtual void PaintControlBarBorders(
		CDC & dc,
		CExtPaintManager::PAINTCONTROLBARBORDERSDATA & _pcbbd
		);

	virtual bool GetCb2DbTransparentMode(
		CObject * pObjHelperSrc,
		LPARAM lParam = 0L
		);
	virtual bool PaintDockerBkgnd(
		CDC & dc,
		CWnd * pWnd,
		LPARAM lParam = NULL
		);

	virtual void PaintControlBarClientArea(
		CDC & dc,
		const RECT & rcClient,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintDockBarClientArea(
		CDC & dc,
		const RECT & rcClient,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

#if (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)
	virtual void PaintShortcutListBkgnd(
		CDC & dc,
		const RECT & rcClient,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintShortcutListScrollButton(
		CDC & dc,
		bool bHorzLayout,
		const RECT & rcButton,
		bool bUpOrLeftBtn,
		bool bHover,
		bool bPressed,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	struct __PROF_UIS_API PAINTSHORTCUTLISTITEMSDATA
	{
		CObject * m_pHelperSrc;
		LPARAM m_lParam;

		CExtCmdIcon * m_pIconBig, * m_pIconSmall;
		LPARAM m_lParamItem;
		CRect m_rcBoundsAll, m_rcBoundsIcon, m_rcBoundsText,
			m_rcCtrlClient;
		CExtSafeString m_sText;
		bool m_bHover:1, m_bPressed:1, m_bVisible:1,
			m_bChecked:1, m_bExpandMode:1, m_bHorzLayout:1,
			m_bBigIconView:1, m_bEraseQuery:1;
		DWORD m_dwShortcutListStyle;
		HFONT m_hItemTextFont;

		PAINTSHORTCUTLISTITEMSDATA();
		PAINTSHORTCUTLISTITEMSDATA(
			CObject * pHelperSrc,
			DWORD dwShortcutListStyle,
			HFONT hItemTextFont,
			CExtCmdIcon * pIconBig,
			CExtCmdIcon * pIconSmall,
			LPARAM lParamItem,
			const RECT & rcBoundsAll,
			const RECT & rcBoundsIcon,
			const RECT & rcBoundsText,
			const RECT & rcCtrlClient,
			__EXT_MFC_SAFE_LPCTSTR sText,
			bool bHover,
			bool bPressed,
			bool bVisible,
			bool bChecked,
			bool bExpandMode,
			bool bHorzLayout,
			bool BigIconView,
			bool bEraseQuery
			);
	}; // struct PAINTSHORTCUTLISTITEMSDATA

	virtual void PaintShortcutListItem(
		CDC & dc,
		CExtPaintManager::PAINTSHORTCUTLISTITEMSDATA & _pslid
		);
#endif // (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)

	virtual void OnSysColorChange();
	virtual void OnSettingChange(
		UINT uFlags,
		__EXT_MFC_SAFE_LPCTSTR lpszSection
		);
	virtual void OnDisplayChange(
		INT nDepthBPP,
		CPoint ptSizes
		);
	virtual void OnThemeChanged(
		WPARAM wParam,
		LPARAM lParam
		);

	virtual CSize GetToolBarRgnRoundersSize();

	struct __PROF_UIS_API PAINTTOOLBARTEXTFIELDDATA
	{
		CObject * m_pHelperSrc;
		LPARAM m_lParam;
		UINT m_nDrawTextFlags;
		CExtSafeString m_sText;
		CRect m_rcButton, m_rcTextField;
		bool m_bComboField:1;
		bool m_bHover:1;
		bool m_bPushed:1;
		bool m_bEnabled:1;

		PAINTTOOLBARTEXTFIELDDATA();
		PAINTTOOLBARTEXTFIELDDATA(
			CObject * pHelperSrc,
			__EXT_MFC_SAFE_LPCTSTR sText,
			const RECT & rcButton,
			const RECT & rcTextField,
			bool bComboField,
			bool bHover,
			bool bPushed,
			bool bEnabled
			);
	}; // struct PAINTTOOLBARTEXTFIELDDATA

	virtual void PaintToolbarTextField(
		CDC & dc,
		PAINTTOOLBARTEXTFIELDDATA & _ptbtfd
		);

	virtual COLORREF GetIconAlphaColor() const;
	virtual double GetIconAlphaAdjustPercent() const;

	virtual void PaintResizingGripper(
		CDC &dc,
		const RECT & rcGrip,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

}; // class CExtPaintManager

class __PROF_UIS_API CExtPaintManagerXP : public CExtPaintManager
{
protected:
	bool m_bEnabledHoverIconShadows:1;
	bool m_bHelperXpStyle8BitBarRect:1;
	bool m_bExpBtnSwapVGlyphs:1;
	const glyph_t * m_pGlyphTbEpBtnH0;
	const glyph_t * m_pGlyphTbEpBtnH1;
	const glyph_t * m_pGlyphTbEpBtnV0;
	const glyph_t * m_pGlyphTbEpBtnV1;
	int m_nIdxClrMlaNormLeft, m_nIdxClrMlaNormRight,
		m_nIdxClrMlaRarelyLeft, m_nIdxClrMlaRarelyRight,
		m_nIdxClrBtnNormLeft, m_nIdxClrBtnNormRight,
		m_nIdxClrBtnHoverLeft, m_nIdxClrBtnHoverRight,
		m_nIdxClrBtnPressedLeft, m_nIdxClrBtnPressedRight,
		m_nIdxClrBtnHovPresLeft, m_nIdxClrBtnHovPresRight;
public:
	DECLARE_DYNCREATE( CExtPaintManagerXP )

	// construction/destruction
	CExtPaintManagerXP();
	virtual ~CExtPaintManagerXP();

	virtual COLORREF GetShadowAdjustColor();
	virtual void FixedBar_AdjustClientRect(
		CExtControlBar * pBar,
		CRect & rcPreCalc
		);
	virtual INT FixedBar_GetRowDistance(
		CExtToolControlBar * pBar
		);
	virtual bool FixedBar_IsPaintRowBkMode(
		CExtToolControlBar * pBar
		);

	// paint session complete event - free all cached objects
	virtual void OnPaintSessionComplete(
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	// install required translated color as
	// described in e_translated_colors_t enumeration
	virtual void InitTranslatedColors();

	// gets the pushed metic offset
	virtual CSize GetPushedOffset();

	// paint methods

	virtual void PaintPushButton(
		CDC & dc,
		CExtPaintManager::PAINTPUSHBUTTONDATA & _ppbd
		);

	virtual void PaintGripper(
		CDC & dc,
		CExtPaintManager::PAINTGRIPPERDATA & _pgd
		);

	virtual void PaintSeparator(
		CDC & dc,
		const RECT & rectItem,
		bool bHorz,
		bool bTransparentBk,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintIcon(
		CDC & dc,
		bool bHorz,
		CExtSafeString & sTitle,
		CExtCmdIcon * pIcon,
		const CRect & rectItem,
		CRect & rectCaption,
		bool bPushed,
		bool bEnabled,
		bool bHover,
		int eAlign
		);

	virtual void PaintMenuItem(
		CDC & dc,
		CExtPaintManager::PAINTMENUITEMDATA & _pmid
		);
	virtual void PaintMenuSeparator(
		CDC & dc,
		const RECT & rectItem,
		int nIconAreaWidth,
		bool bRarelyUsed,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintControlFrame(
		CDC & dc,
		CExtPaintManager::PAINTCONTROLFRAMEDATA & _pcfd
		);

	virtual void PaintComboFrame(
		CDC &dc,
		CExtPaintManager::PAINTCOMBOFRAMEDATA & _pcfd
		);

	virtual void PaintSysCmdGlyph(
		CDC & dc,
		UINT nHelperSysCmdID,
		const RECT & rcItem,
		bool bHover,
		bool bPushed,
		bool bEnabled
		);

	virtual void PaintFloatToolbarRowBk(
		CDC & dc,
		CExtToolControlBar * pBar,
		int nLastReviewBtnIdx,
		CRect & rcRowBk
		);

	virtual void PaintToolbarExpandButton(
		CDC & dc,
		const RECT & rcButtonArea,
		bool bHorz, // if false - down
		bool bBarIsCompletelyVisible,
		bool bEnabled,
		bool bPushed,
		bool bHover,
		CObject * pHelperSrc,
		LPARAM lParam = 0L,
		bool bTransparentBackground = false
		);
	virtual void PaintMenuExpandButton(
		CDC & dc,
		const RECT & rectButton,
		bool bPushed,
		bool bHover,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual int GetMenuExpandButtonHeight();

	virtual void PaintDockingFrame(
		CDC & dc,
		CExtPaintManager::PAINTDOCKINGFRAMEDATA & _pdfd
		);

	virtual int GetMenuShadowSize();
	virtual int GetMenuBorderSize();

	virtual void PaintMenuBorder(
		CDC & dc,
		const RECT & rectClient,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	enum e_xp_colors_t
	{
		XPCLR_3DFACE_DARK		= __ExtMfc_XP_COLOR_SHIFT+0,
		XPCLR_3DFACE_NORMAL		= __ExtMfc_XP_COLOR_SHIFT+1,
		XPCLR_SEPARATOR			= __ExtMfc_XP_COLOR_SHIFT+2,
		XPCLR_HILIGHT			= __ExtMfc_XP_COLOR_SHIFT+3,
		XPCLR_HILIGHT_BORDER	= __ExtMfc_XP_COLOR_SHIFT+4,
		XPCLR_RARELY_BORDER		= __ExtMfc_XP_COLOR_SHIFT+5,
		XPCLR_PUSHEDHOVERTEXT	= __ExtMfc_XP_COLOR_SHIFT+6,
		XPCLR_MENU_BORDER		= __ExtMfc_XP_COLOR_SHIFT+7,
		XPCLR_3DFACE_FLOAT_R	= __ExtMfc_XP_COLOR_SHIFT+8,
		XPCLR_3DFACE_FLOAT_F	= __ExtMfc_XP_COLOR_SHIFT+9,
	};

	virtual bool IsMenuMustCombineExcludeArea();
	virtual void PaintMenuCombinedArea(
		CDC & dc,
		const RECT & rcArea,
		const RECT & rcClient,
		int eCombineAlign, // CExtPopupMenuWnd::e_combine_align_t values
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintDockingCaptionButton(
		CDC & dc,
		CExtPaintManager::PAINTDOCKINGCAPTIONBUTTONDATA & _pdcbd
		);
	virtual COLORREF GetMenuFrameFillColor()
	{
		return GetColor(XPCLR_3DFACE_NORMAL);
	};

	virtual void PaintControlBarBorders(
		CDC & dc,
		CExtPaintManager::PAINTCONTROLBARBORDERSDATA & _pcbbd
		);

	virtual void PaintTabItem(
		CDC & dc,
		CRect & rcTabItemsArea,
		bool bTopLeft,
		bool bHorz,
		bool bSelected,
		bool bCenteredText,
		bool bGroupedMode,
		bool bInGroupActive,
		bool bInvertedVerticalMode,
		const CRect & rcEntireItem,
		CSize sizeTextMeasured,
		CFont * pFont,
		__EXT_MFC_SAFE_LPCTSTR sText,
		CExtCmdIcon * pIcon,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintTabClientArea(
		CDC & dc,
		CRect & rcClient,
		CRect & rcTabItemsArea,
		CRect & rcTabNearBorderArea,
		DWORD dwOrientation,
		bool bGroupedMode,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintTabButton(
		CDC & dc,
		CRect & rcButton,
		LONG nHitTest,
		bool bTopLeft,
		bool bHorz,
		bool bEnabled,
		bool bHover,
		bool bPushed,
		bool bGroupedMode,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintTabNcAreaRect(
		CDC & dc,
		const RECT & rc,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintResizableBarChildNcAreaRect(
		CDC & dc,
		const RECT & rc,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual bool GetCb2DbTransparentMode(
		CObject * pObjHelperSrc,
		LPARAM lParam = 0L
		);
	virtual bool PaintDockerBkgnd(
		CDC & dc,
		CWnd * pWnd,
		LPARAM lParam = NULL
		);

	virtual void PaintControlBarClientArea(
		CDC & dc,
		const RECT & rcClient,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintDockBarClientArea(
		CDC & dc,
		const RECT & rcClient,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

#if (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)
	virtual void PaintShortcutListBkgnd(
		CDC & dc,
		const RECT & rcClient,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintShortcutListScrollButton(
		CDC & dc,
		bool bHorzLayout,
		const RECT & rcButton,
		bool bUpOrLeftBtn,
		bool bHover,
		bool bPressed,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintShortcutListItem(
		CDC & dc,
		CExtPaintManager::PAINTSHORTCUTLISTITEMSDATA & _pslid
		);
#endif // (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)

	virtual CSize GetToolBarRgnRoundersSize();

	virtual void PaintToolbarTextField(
		CDC & dc,
		PAINTTOOLBARTEXTFIELDDATA & _ptbtfd
		);

	virtual COLORREF GetIconAlphaColor() const;
	virtual double GetIconAlphaAdjustPercent() const;

	virtual void PaintResizingGripper(
		CDC &dc,
		const RECT & rcGrip,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

}; // class CExtPaintManagerXP

class __PROF_UIS_API CExtPaintManagerOffice2003 : public CExtPaintManagerXP
{
	CBitmap
		m_bmpCtBarGradientHorz,
		m_bmpCtBarGradientVert,
		m_bmpDockerGradient;
	CSize
		m_sizeCtBarGradientHorz,
		m_sizeCtBarGradientVert,
		m_sizeDockerGradient,
		m_sizeMenuExpBtnCircle;
	CRgn m_rgnMenuExpBtnCircle;
	int m_nIdxClrTbFillMargin;
	CSize m_sizeToolBarRgnRounders;
public:
	DECLARE_DYNCREATE( CExtPaintManagerOffice2003 )

	// construction/destruction
	CExtPaintManagerOffice2003();
	virtual ~CExtPaintManagerOffice2003();

	virtual COLORREF GetShadowAdjustColor();
	virtual void FixedBar_AdjustClientRect(
		CExtControlBar * pBar,
		CRect & rcPreCalc
		);
	virtual INT FixedBar_GetRowDistance(
		CExtToolControlBar * pBar
		);
	virtual bool FixedBar_IsPaintRowBkMode(
		CExtToolControlBar * pBar
		);

	// paint session complete event - free all cached objects
	virtual void OnPaintSessionComplete(
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	enum e_2003_colors_t
	{
		_2003CLR_GRADIENT_LIGHT			= __ExtMfc_2003_COLOR_SHIFT+0,
		_2003CLR_GRADIENT_DARK			= __ExtMfc_2003_COLOR_SHIFT+1,
		_2003CLR_GRIPPER_DOT_LIGHT		= __ExtMfc_2003_COLOR_SHIFT+2,
		_2003CLR_GRIPPER_DOT_DARK		= __ExtMfc_2003_COLOR_SHIFT+3,
		_2003CLR_EXPBTN_LIGHT			= __ExtMfc_2003_COLOR_SHIFT+4,
		_2003CLR_EXPBTN_DARK			= __ExtMfc_2003_COLOR_SHIFT+5,
		_2003CLR_EXPBTN_HOVER_LIGHT		= __ExtMfc_2003_COLOR_SHIFT+6,
		_2003CLR_EXPBTN_HOVER_DARK		= __ExtMfc_2003_COLOR_SHIFT+7,
		_2003CLR_EXPBTN_PRESSED_LIGHT	= __ExtMfc_2003_COLOR_SHIFT+8,
		_2003CLR_EXPBTN_PRESSED_DARK	= __ExtMfc_2003_COLOR_SHIFT+9,
		_2003CLR_SEPARATOR_LIGHT		= __ExtMfc_2003_COLOR_SHIFT+10,
		_2003CLR_SEPARATOR_DARK			= __ExtMfc_2003_COLOR_SHIFT+11,
		_2003CLR_EXPGLYPH_LIGHT			= __ExtMfc_2003_COLOR_SHIFT+12,
		_2003CLR_EXPGLYPH_DARK			= __ExtMfc_2003_COLOR_SHIFT+13,
		_2003CLR_STATUSBAR_ITEM			= __ExtMfc_2003_COLOR_SHIFT+14,
		_2003CLR_MLA_NORM_LEFT			= __ExtMfc_2003_COLOR_SHIFT+15,
		_2003CLR_MLA_NORM_RIGHT			= __ExtMfc_2003_COLOR_SHIFT+16,
		_2003CLR_MLA_RARELY_LEFT		= __ExtMfc_2003_COLOR_SHIFT+17,
		_2003CLR_MLA_RARELY_RIGHT		= __ExtMfc_2003_COLOR_SHIFT+18,
		_2003CLR_BTN_HOVER_LEFT			= __ExtMfc_2003_COLOR_SHIFT+19,
		_2003CLR_BTN_HOVER_RIGHT		= __ExtMfc_2003_COLOR_SHIFT+20,
		_2003CLR_BTN_PRESSED_LEFT		= __ExtMfc_2003_COLOR_SHIFT+21,
		_2003CLR_BTN_PRESSED_RIGHT		= __ExtMfc_2003_COLOR_SHIFT+22,
		_2003CLR_BTN_HP_LEFT			= __ExtMfc_2003_COLOR_SHIFT+23,
		_2003CLR_BTN_HP_RIGHT			= __ExtMfc_2003_COLOR_SHIFT+24,
		_2003CLR_MENUEXPBALOON			= __ExtMfc_2003_COLOR_SHIFT+25,
	};

	// install required translated color as
	// described in e_translated_colors_t enumeration
	virtual void InitTranslatedColors();

	virtual bool GetCb2DbTransparentMode(
		CObject * pObjHelperSrc,
		LPARAM lParam = 0L
		);
	virtual bool PaintDockerBkgnd(
		CDC & dc,
		CWnd * pWnd,
		LPARAM lParam = NULL
		);

	virtual void PaintControlBarClientArea(
		CDC & dc,
		const RECT & rcClient,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintDockBarClientArea(
		CDC & dc,
		const RECT & rcClient,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintDockingFrame(
		CDC & dc,
		CExtPaintManager::PAINTDOCKINGFRAMEDATA & _pdfd
		);

	virtual void PaintGripper(
		CDC & dc,
		CExtPaintManager::PAINTGRIPPERDATA & _pgd
		);

	virtual void PaintSeparator(
		CDC & dc,
		const RECT & rectItem,
		bool bHorz,
		bool bTransparentBk,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintTabClientArea(
		CDC & dc,
		CRect & rcClient,
		CRect & rcTabItemsArea,
		CRect & rcTabNearBorderArea,
		DWORD dwOrientation,
		bool bGroupedMode,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintComboFrame(
		CDC &dc,
		CExtPaintManager::PAINTCOMBOFRAMEDATA & _pcfd
		);

	virtual void PaintFloatToolbarRowBk(
		CDC & dc,
		CExtToolControlBar * pBar,
		int nLastReviewBtnIdx,
		CRect & rcRowBk
		);

	virtual void PaintToolbarExpandButton(
		CDC & dc,
		const RECT & rcButtonArea,
		bool bHorz, // if false - down
		bool bBarIsCompletelyVisible,
		bool bEnabled,
		bool bPushed,
		bool bHover,
		CObject * pHelperSrc,
		LPARAM lParam = 0L,
		bool bTransparentBackground = false
		);

	virtual void PaintControlBarBorders(
		CDC & dc,
		CExtPaintManager::PAINTCONTROLBARBORDERSDATA & _pcbbd
		);

	virtual void PaintMenuCombinedArea(
		CDC & dc,
		const RECT & rcArea,
		const RECT & rcClient,
		int eCombineAlign, // CExtPopupMenuWnd::e_combine_align_t values
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintMenuExpandButton(
		CDC & dc,
		const RECT & rectButton,
		bool bPushed,
		bool bHover,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual int GetMenuExpandButtonHeight();

	// gets the pushed metic offset
	virtual CSize GetPushedOffset();

	virtual CSize GetToolBarRgnRoundersSize();

	virtual COLORREF GetIconAlphaColor() const;
	virtual double GetIconAlphaAdjustPercent() const;

	virtual void PaintResizingGripper(
		CDC &dc,
		const RECT & rcGrip,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
}; // class CExtPaintManagerOffice2003

extern __PROF_UIS_API CExtPaintManager::CExtPaintManagerAutoPtr g_PaintManager;

#endif // __EXT_PAINT_MANAGER_H

