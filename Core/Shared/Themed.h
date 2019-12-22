// Themed.h: interface for the CThemed class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THEMED_H__2FA586FE_B790_4315_93B9_01000725B68A__INCLUDED_)
#define AFX_THEMED_H__2FA586FE_B790_4315_93B9_01000725B68A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "..\3rdparty\uxtheme.h"

#if _MSC_VER < 1400
#include "..\3rdparty\TmSchema.h" // for part and state enumerations
#else
#	if _MSC_VER >= 1500
#	include <vssym32.h>
#	define LIS_NORMAL LISS_NORMAL
#	define LIS_HOT LISS_HOT
#	define LIS_SELECTED LISS_SELECTED
#	define LIS_DISABLED LISS_DISABLED
#	define LIS_SELECTEDNOTFOCUS LISS_SELECTEDNOTFOCUS
#	define LIS_HOTSELECTED LISS_HOTSELECTED
#	endif

#define LIS_MORESELECTED 6
#endif

//////////////////////////////////////////////////////////////////////

#ifndef ETDT_DISABLE
#	define ETDT_DISABLE        0x00000001
#	define ETDT_ENABLE         0x00000002
#	define ETDT_USETABTEXTURE  0x00000004
#	define ETDT_ENABLETAB      (ETDT_ENABLE  | ETDT_USETABTEXTURE)
#endif // !ETDT_DISABLE


//////////////////////////////////////////////////////////////////////

#if (WINVER < 0x0500)
#	define DFCS_TRANSPARENT        0x0800
#	define DFCS_HOT                0x1000
#	define DFC_POPUPMENU           5
#endif

#define DFCS_MIXED (DFCS_BUTTON3STATE | DFCS_CHECKED) // DrawFrameControl for checkbox

//////////////////////////////////////////////////////////////////////

#ifndef HPAINTBUFFER

typedef HANDLE HPAINTBUFFER;  // handle to a buffered paint context

enum TH_BUFFERFORMAT
{
    THBPBF_COMPATIBLEBITMAP,    // Compatible bitmap
	THBPBF_DIB,                 // Device-independent bitmap
	THBPBF_TOPDOWNDIB,          // Top-down device-independent bitmap
	THBPBF_TOPDOWNMONODIB       // Top-down monochrome device-independent bitmap
};

#define BPPF_ERASE		0x0001
#define BPPF_NOCLIP		0x0002
#define BPPF_NONCLIENT	0x0004

// BP_PAINTPARAMS
struct TH_PAINTPARAMS
{
    DWORD                       cbSize;
    DWORD                       dwFlags; // BPPF_ flags
    const RECT *                prcExclude;
    const BLENDFUNCTION *       pBlendFunction;
};
#endif // !HPAINTBUFFER

//////////////////////////////////////////////////////////////////////

class CThemed  
{
public:
	CThemed(const CWnd* pWnd = NULL, LPCTSTR szClassList = NULL);
	CThemed(HWND hwnd, LPCTSTR szClassList = NULL);
	virtual ~CThemed();
	
	static BOOL AreControlsThemed();
	static BOOL IsNonClientThemed();
	static BOOL IsWebContentThemed();
	static BOOL SupportsTheming(int nFlag);
	static BOOL IsAppThemed();
	static BOOL IsWindowsThemed();
	
	static BOOL DrawFrameControl(const CWnd* pWnd, CDC* pDC, const CRect& rect, UINT nType, UINT nState, LPCRECT prClip = NULL);
	static BOOL DrawEdge(const CWnd* pWnd, CDC* pDC, const CRect& rect, UINT nType, UINT nState, UINT nEdge, UINT nFlags);
	static BOOL SetWindowTheme(const CWnd* pWnd, LPCTSTR szAppName);
	static BOOL EnableDialogTexture(const CWnd* pWnd, DWORD dwFlags);
	
	static HPAINTBUFFER BeginBufferedPaint(HDC hdcTarget, const RECT* rTarget, TH_BUFFERFORMAT dwFormat, TH_PAINTPARAMS * pParams, HDC* hdcBuffered);
	static BOOL GetBufferedPaintBits(HPAINTBUFFER pb, RGBQUAD** pBits, int* nBitsCount);
	static BOOL EndBufferedPaint(HPAINTBUFFER pb, BOOL bUpdatetarget);

	BOOL Open(HWND hWnd, LPCTSTR szClassList);
	BOOL Open(const CWnd* pWnd, LPCTSTR szClassList);
	BOOL IsValid() { return (s_hUxTheme && m_hTheme); }
	BOOL Close();
	
	BOOL DrawParentBackground(const CWnd* pWnd, CDC* pDC, LPCRECT pRect = NULL);
	BOOL DrawBackground(CDC* pDC, int nPart, int nState, const CRect& rect, LPCRECT prClip = NULL);
	BOOL DrawText(CDC* pDC, int nPart, int nState, const CString& sText, DWORD dwTextFlags, DWORD dwTextFlags2, const CRect& pRect);
	BOOL DrawEdge(CDC* pDC, int nPart, int nState, const CRect& rDest, UINT nEdge, UINT nFlags, LPRECT prContent = NULL);
	BOOL DrawIcon(CDC* pDC, int nPart, int nState, const CRect& rect, const CImageList* pIL, int nImage);
	BOOL DrawBorder(CDC* pDC, int nState, const CRect& rect);
	
	BOOL GetSize(int nPart, int nState, CSize& size, BOOL bMin = FALSE);
	BOOL GetTextExtent(CDC* pDC, int nPart, int nState, const CString& sText, DWORD dwTextFlags, CRect& rExtent, LPCRECT prBounding = NULL);
	BOOL BuildImageList(CImageList& il, int nPart, const int nStates[], int nNumStates, COLORREF crMask = 255, LPCRECT prPadding = NULL);

	BOOL GetThemeColor(int nPart, int nState, int nProp, COLORREF& color);
	BOOL GetThemeBackgroundContentRect(CDC* pDC, int nPart,	int nState, const CRect& rBounding, CRect& rContent);

protected:
	static HMODULE s_hUxTheme;
	HTHEME m_hTheme;
	HWND m_hWnd;
	
protected:
	static BOOL InitUxTheme();
	
	// uxtheme functions
	static DWORD GetAppThemeProperties();

	HTHEME OpenThemeData(HWND hwnd, LPCWSTR pszClassList);
	BOOL CloseThemeData(HTHEME hTheme);
	BOOL DrawThemeBackground(HDC hdc, int iPartId, int iStateId, LPCRECT pRect, 
		LPCRECT pClipRect);
	BOOL EnableThemeDialogTexture(HWND hWnd, DWORD dwFlags);
	BOOL DrawThemeParentBackground(HWND hWnd, HDC hdc, LPCRECT pRect);
	BOOL DrawThemeText(HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, 
		DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect);
	BOOL DrawThemeEdge(HDC hdc, int iPartId, int iStateId, LPCRECT pDestRect, UINT uEdge, 
		UINT uFlags, RECT *pContentRect);
	BOOL DrawThemeIcon(HDC hdc, int iPartId, int iStateId, LPCRECT pRect, HIMAGELIST himl, 
		int iImageIndex);
	BOOL DrawThemeBorder(HDC hdc, int iStateId, LPCRECT pRect);
	BOOL GetThemePartSize(int iPartId, int iStateId, THEMESIZE eSize, SIZE *psz);
	BOOL GetThemeTextExtent(HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, 
		DWORD dwTextFlags, LPCRECT pBoundingRect, RECT *pExtentRect);
	BOOL GetThemeBackgroundContentRect(HDC hdc, int iPartId, int iStateId, 
										LPCRECT pBoundingRect, LPRECT pContentRect);

	// helpers
	static BOOL GetThemeClassPartState(int nType, int nState, CString& sThClass, int& nThPart, int& nThState); 
};

#endif // !defined(AFX_THEMED_H__2FA586FE_B790_4315_93B9_01000725B68A__INCLUDED_)
