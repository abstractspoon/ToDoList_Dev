// Themed.cpp: implementation of the CThemed class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Themed.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

// theme function definitions
typedef BOOL (STDAPICALLTYPE *PFNISTHEMEACTIVE)();
typedef BOOL (STDAPICALLTYPE *PFNISAPPTHEMED)();
typedef DWORD (STDAPICALLTYPE *PFNGETTHEMEAPPPROPERTIES)();
typedef HTHEME (STDAPICALLTYPE *PFNOPENTHEMEDATA)(HWND hwnd, LPCWSTR pszClassList);
typedef HRESULT (STDAPICALLTYPE *PFNSETWINDOWTHEME)(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList);
typedef HRESULT (STDAPICALLTYPE *PFNCLOSETHEMEDATA)(HTHEME hTheme);
typedef HRESULT (STDAPICALLTYPE *PFNDRAWTHEMEBACKGROUND)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, 
														 LPCRECT pRect, LPCRECT pClipRect);
typedef HRESULT (STDAPICALLTYPE *PFNENABLETHEMEDIALOGTEXTURE)(HWND  hwnd, DWORD dwFlags); 
typedef HRESULT (STDAPICALLTYPE *PFNDRAWTHEMEPARENTBACKGROUND)(HWND hWnd, HDC hdc, LPCRECT pRect);
typedef HRESULT (STDAPICALLTYPE *PFNDRAWTHEMETEXT)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, 
												   LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, 
												   DWORD dwTextFlags2, LPCRECT pRect);
typedef HRESULT (STDAPICALLTYPE *PFNDRAWTHEMEEDGE)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, 
												   LPCRECT pDestRect, UINT uEdge, UINT uFlags, 
												   RECT *pContentRect);
typedef HRESULT (STDAPICALLTYPE *PFNDRAWTHEMEICON)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, 
												   LPCRECT pRect, HIMAGELIST himl, int iImageIndex);
typedef HRESULT (STDAPICALLTYPE *PFNDRAWTHEMEBORDER)(HTHEME hTheme, HDC hdc, int iStateId, LPCRECT pRect);
typedef HRESULT (STDAPICALLTYPE *PFNGETTHEMEPARTSIZE)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId,
                                                      RECT *prc, THEMESIZE eSize, SIZE *psz);
typedef HRESULT (STDAPICALLTYPE *PFNGETTHEMETEXTEXTENT)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, 
													  LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, 
													  LPCRECT pBoundingRect, RECT *pExtentRect);
typedef HRESULT (STDAPICALLTYPE *PFNGETTHEMECOLOR)(HTHEME hTheme, int iPartId, int iStateId, int iPropID,
												   COLORREF* pColor);
typedef HRESULT (STDAPICALLTYPE *PFNGETTHEMEBACKGROUNDCONTENTRECT)(HTHEME hTheme, HDC hdc, int iPartId,
												int iStateId, LPCRECT pBoundingRect, LPRECT pContentRect);

typedef HPAINTBUFFER (STDAPICALLTYPE *PFNBEGINBUFFEREDPAINT)(HDC, const RECT*, TH_BUFFERFORMAT, TH_PAINTPARAMS*, HDC*);
typedef HRESULT (STDAPICALLTYPE *PFNGETBUFFEREDPAINTBITS)(HPAINTBUFFER, RGBQUAD**, int*);
typedef HRESULT (STDAPICALLTYPE *PFNENDBUFFEREDPAINT)(HPAINTBUFFER, BOOL);

//////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
#	define WSTR(string) (string)
#else
#	define WSTR(string) (LPCWSTR)COleVariant(string).bstrVal
#endif

//////////////////////////////////////////////////////////////////////

#ifndef DFCS_TRANSPARENT
#	define DFCS_TRANSPARENT        0x0800
#	define DFCS_HOT                0x1000
#	define DFC_POPUPMENU           5
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HMODULE CThemed::s_hUxTheme = HMODULE(-1);

CThemed::CThemed(const CWnd* pWnd, LPCTSTR szClassList) : m_hWnd(NULL), m_hTheme(NULL)
{
	InitUxTheme();
	
	if (pWnd)
		Open(pWnd->GetSafeHwnd(), szClassList);
}

CThemed::CThemed(HWND hwnd, LPCTSTR szClassList) : m_hWnd(NULL), m_hTheme(NULL)
{
	InitUxTheme();
	
	Open(hwnd, szClassList);
}

CThemed::~CThemed()
{
	Close();
}

BOOL CThemed::InitUxTheme()
{
	if (s_hUxTheme == HMODULE(-1)) // one time only
		s_hUxTheme = LoadLibrary(_T("UxTheme.dll"));
	
	return (NULL != s_hUxTheme);
}

BOOL CThemed::SupportsTheming(int nFlag)
{
	return (IsAppThemed() && (GetAppThemeProperties() & nFlag));
}

HPAINTBUFFER CThemed::BeginBufferedPaint(HDC hdcTarget, const RECT* rTarget, TH_BUFFERFORMAT dwFormat, TH_PAINTPARAMS * pParams, HDC* hdcBuffered)
{
	if (InitUxTheme())
	{
		PFNBEGINBUFFEREDPAINT fnBeginBufferedPaint = (PFNBEGINBUFFEREDPAINT)GetProcAddress(s_hUxTheme, "BeginBufferedPaint");
		
		if (fnBeginBufferedPaint)
			return (HPAINTBUFFER)fnBeginBufferedPaint(hdcTarget, rTarget, dwFormat, pParams, hdcBuffered);
	}
	
	return NULL;
}

BOOL CThemed::GetBufferedPaintBits(HPAINTBUFFER pb, RGBQUAD** pBits, int* nBitsCount)
{
	if (InitUxTheme())
	{
		PFNGETBUFFEREDPAINTBITS fnGetBufferedPaintBits = (PFNGETBUFFEREDPAINTBITS)GetProcAddress(s_hUxTheme, "GetBufferedPaintBits");
		
		if (fnGetBufferedPaintBits)
			return (SUCCEEDED(fnGetBufferedPaintBits(pb, pBits, nBitsCount)));
	}
	
	return FALSE;
}

BOOL CThemed::EndBufferedPaint(HPAINTBUFFER pb, BOOL bUpdatetarget)
{
	if (InitUxTheme())
	{
		PFNENDBUFFEREDPAINT fnEndBufferedPaint = (PFNENDBUFFEREDPAINT)GetProcAddress(s_hUxTheme, "EndBufferedPaint");
		
		if (fnEndBufferedPaint)
			return (SUCCEEDED(fnEndBufferedPaint(pb, bUpdatetarget)));
	}
	
	return FALSE;
}

BOOL CThemed::Open(const CWnd* pWnd, LPCTSTR szClassList)
{
	return (pWnd && Open(pWnd->GetSafeHwnd(), szClassList));
}

BOOL CThemed::Open(HWND hWnd, LPCTSTR szClassList)
{
	if (::IsWindow(hWnd))
	{
		if (!s_hUxTheme)
		{
			s_hUxTheme = LoadLibrary(_T("UxTheme.dll"));

			if (!s_hUxTheme)
				return FALSE;
		}
		else
			Close();
		
		CString sClassList(szClassList);
		
		if (sClassList.IsEmpty())
		{
			::GetClassName(hWnd, sClassList.GetBuffer(128), 128);
			sClassList.ReleaseBuffer();
		}
		
		m_hTheme = OpenThemeData(hWnd, WSTR(sClassList));
		m_hWnd = hWnd;
		
		return (NULL != m_hTheme);
	}
	
	return FALSE;
}

BOOL CThemed::Close()
{
	if (CloseThemeData(m_hTheme))
		m_hTheme = NULL;
	
	return (NULL == m_hTheme);
}

BOOL CThemed::AreControlsThemed()
{
	return SupportsTheming(STAP_ALLOW_CONTROLS);
}

BOOL CThemed::IsNonClientThemed()
{
	return SupportsTheming(STAP_ALLOW_NONCLIENT);
}

BOOL CThemed::IsWebContentThemed()
{
	return SupportsTheming(STAP_ALLOW_WEBCONTENT);
}

// static 
BOOL CThemed::SetWindowTheme(const CWnd* pWnd, LPCTSTR szAppName)
{
	if (InitUxTheme())
	{
		PFNSETWINDOWTHEME fnSetWindowTheme = (PFNSETWINDOWTHEME)GetProcAddress(s_hUxTheme, "SetWindowTheme");

		if (fnSetWindowTheme)
			return (SUCCEEDED(fnSetWindowTheme(*pWnd, szAppName, NULL)));
	}

	return FALSE;
}

BOOL CThemed::EnableDialogTexture(const CWnd* pWnd, DWORD dwFlags)
{
	CThemed th;
	
	if (th.AreControlsThemed())
		return th.EnableThemeDialogTexture(*pWnd, dwFlags);

	return FALSE;
}

BOOL CThemed::DrawFrameControl(const CWnd* pWnd, CDC* pDC, const CRect& rect, UINT nType, UINT nState, LPCRECT pClip)
{
	CThemed th;
	
	if (th.SupportsTheming(STAP_ALLOW_CONTROLS) && th.SupportsTheming(STAP_ALLOW_NONCLIENT))
	{
		// determine the appropriate class, part and state
		CString sThClass;
		int nThPart = 0, nThState = 0;
		
		if (!th.GetThemeClassPartState(nType, nState, sThClass, nThPart, nThState))
			return FALSE;
		
		if (!th.Open(pWnd, sThClass))
			return FALSE;

		// Don't scale check boxes or radio buttons
		CRect rImage(rect);

		switch (nThPart)
		{
		case BP_CHECKBOX:
		case BP_RADIOBUTTON:
			{
				CSize size;
				th.GetSize(nThPart, 1, size);

				rImage.OffsetRect((rImage.Width() - size.cx) / 2,
					(rImage.Height() - size.cy) / 2);

				rImage.right = (rImage.left + size.cx);
				rImage.bottom = (rImage.top + size.cy);
			}
			break;
		}
				
		th.DrawBackground(pDC, nThPart, nThState, rImage, pClip);
		
		return TRUE;
	}

	// else
	return pDC->DrawFrameControl((LPRECT)(LPCRECT)rect, nType, nState);
}

BOOL CThemed::DrawEdge(const CWnd* pWnd, CDC* pDC, const CRect& rect, UINT nType, UINT nState, UINT nEdge, UINT nFlags)
{
	CThemed th;
	
	if (th.SupportsTheming(STAP_ALLOW_CONTROLS) && th.SupportsTheming(STAP_ALLOW_NONCLIENT))
	{
		// determine the appropriate class, part and state
		CString sThClass;
		int nThPart = 0, nThState = 0;
		
		if (!th.GetThemeClassPartState(nType, nState, sThClass, nThPart, nThState))
			return FALSE;
		
		if (!th.Open(pWnd, sThClass))
			return FALSE;
		
		th.DrawEdge(pDC, nThPart, nThState, rect, nEdge, nFlags);
		
		return TRUE;
	}

	// else
	return pDC->DrawFrameControl((LPRECT)(LPCRECT)rect, nType, nState);
}

BOOL CThemed::DrawBackground(CDC* pDC, int nPart, int nState, const CRect& rect, LPCRECT prClip)
{
	ASSERT (m_hTheme);
	ASSERT_VALID (pDC);
	
	return DrawThemeBackground(*pDC, nPart, nState, rect, prClip);
}

BOOL CThemed::DrawParentBackground(const CWnd* pWnd, CDC* pDC, LPCRECT pRect)
{
	ASSERT(m_hTheme);
	ASSERT_VALID(pDC);
	ASSERT_VALID(pWnd);

	return DrawThemeParentBackground(*pWnd, *pDC, pRect);
}

BOOL CThemed::DrawText(CDC* pDC, int nPart, int nState, const CString& sText, DWORD dwTextFlags, DWORD dwTextFlags2, const CRect& rect)
{
	ASSERT (m_hTheme);
	ASSERT_VALID (pDC);
	
	return DrawThemeText(*pDC, nPart, nState, WSTR(sText), sText.GetLength(), dwTextFlags, dwTextFlags2, rect);
}

BOOL CThemed::DrawEdge(CDC* pDC, int nPart, int nState, const CRect& rDest, UINT nEdge, UINT nFlags, LPRECT prContent)
{
	ASSERT (m_hTheme);
	ASSERT_VALID (pDC);
	
	return DrawThemeEdge(*pDC, nPart, nState, rDest, nEdge, nFlags, prContent);
}

BOOL CThemed::DrawIcon(CDC* pDC, int nPart, int nState, const CRect& rect, const CImageList* pIL, int nImage)
{
	ASSERT (m_hTheme);
	ASSERT_VALID (pDC);
	ASSERT_VALID (pIL);
	
	return DrawThemeIcon(*pDC, nPart, nState, rect, *pIL, nImage);
}

BOOL CThemed::DrawBorder(CDC* pDC, int nState, const CRect& rect)
{
	ASSERT (m_hTheme);
	ASSERT_VALID (pDC);
	
	return DrawThemeBorder(*pDC, nState, rect);
}

BOOL CThemed::GetSize(int nPart, int nState, CSize& size, BOOL bMin)
{
	ASSERT (m_hTheme);
	
	return GetThemePartSize(nPart, nState, bMin ? TS_MIN : TS_TRUE, &size);
}

BOOL CThemed::GetTextExtent(CDC* pDC, int nPart, int nState, const CString& sText, DWORD dwTextFlags, CRect& rExtent, LPCRECT prBounding)
{
	ASSERT (m_hTheme);
	
	return GetThemeTextExtent(pDC ? *pDC : (HDC)NULL, nPart, nState, WSTR(sText), sText.GetLength(), dwTextFlags, prBounding, rExtent);
}

BOOL CThemed::BuildImageList(CImageList& il, int nPart, const int nStates[], int nNumStates, COLORREF crMask, LPCRECT prPadding)
{
	ASSERT (!il.GetSafeHandle());
	
	if (il.GetSafeHandle())
		return FALSE;
	
	if (!nNumStates)
		return FALSE;
	
	CSize sizePart;
	GetSize(nPart, 1, sizePart);

	// create a bitmap containing the required images
	HWND hWnd = m_hWnd ? m_hWnd : GetDesktopWindow();
	CClientDC dc(CWnd::FromHandle(hWnd));
	CDC dcBack;
	
	if (dcBack.CreateCompatibleDC(&dc))
	{
		CRect rPadding(0, 0, 0, 0);

		if (prPadding)
			rPadding.CopyRect(prPadding);
		
		// Pad check boxes to a minimum unscaled size of 16
		if (nPart == BP_CHECKBOX)
		{
			ASSERT(sizePart.cx == sizePart.cy);

			double dScale = (dc.GetDeviceCaps(LOGPIXELSY) / 96.0);
			int nPadding = ((int)(16 * dScale) - sizePart.cx);

			if (nPadding > 0)
			{
				rPadding.left += (nPadding / 2);
				rPadding.right += (nPadding - (nPadding / 2));
				rPadding.top += (nPadding - (nPadding / 2));
				rPadding.bottom += (nPadding / 2);
			}
		}

		int nImWidth = sizePart.cx + (rPadding.left + rPadding.right);
		int nImHeight = sizePart.cy + (rPadding.top + rPadding.bottom);

		CBitmap bitmap;

		if (bitmap.CreateCompatibleBitmap(&dc, nImWidth * nNumStates, nImHeight))
		{
			CGdiObject* pOld = dcBack.SelectObject(&bitmap);
			dcBack.FillSolidRect(0, 0, nImWidth * nNumStates, nImHeight, crMask);

			CRect rState(rPadding.TopLeft(), sizePart);

			for (int nState = 0; nState < nNumStates; nState++)
			{
				if (nStates[nState] != -1)
					DrawBackground(&dcBack, nPart, nStates[nState], rState);
				
				// next state
				rState.OffsetRect(nImWidth, 0);
			}
			
			dcBack.SelectObject(pOld);
			
			// create imagelist
			if (il.Create(nImWidth, nImHeight, ILC_COLOR32 | ILC_MASK, 0, 1))
				il.Add(&bitmap, crMask);
		}
	}
	
	return (il.GetSafeHandle() != NULL);
}

BOOL CThemed::GetThemeBackgroundContentRect(CDC* pDC, int nPart, int nState, const CRect& rBounding, CRect& rContent)
{
	ASSERT (m_hTheme);
	ASSERT_VALID (pDC);

	return GetThemeBackgroundContentRect(pDC->GetSafeHdc(), nPart, nState, rBounding, rContent);
}

// -----------------------------------------------------------------------------------------------------------

BOOL CThemed::IsWindowsThemed()
{
	if (InitUxTheme())
	{
		PFNISTHEMEACTIVE fnIsThemeActive = (PFNISTHEMEACTIVE)GetProcAddress(s_hUxTheme, "IsThemeActive");
		
		if (fnIsThemeActive)
			return fnIsThemeActive();
	}
	
	return FALSE;
}

BOOL CThemed::IsAppThemed()
{
	if (InitUxTheme())
	{
		PFNISAPPTHEMED fnIsAppThemed = (PFNISAPPTHEMED)GetProcAddress(s_hUxTheme, "IsAppThemed");
		
		if (fnIsAppThemed)
			return fnIsAppThemed();
	}
	
	return FALSE;

}

DWORD CThemed::GetAppThemeProperties()
{
	if (InitUxTheme())
	{
		PFNGETTHEMEAPPPROPERTIES fnGetThemeAppProperties = (PFNGETTHEMEAPPPROPERTIES)GetProcAddress(s_hUxTheme, "GetThemeAppProperties");
		
		if (fnGetThemeAppProperties)
			return fnGetThemeAppProperties();
	}
	
	return 0;
}

HTHEME CThemed::OpenThemeData(HWND hwnd, LPCWSTR pszClassList)
{
	if (InitUxTheme())
	{
		PFNOPENTHEMEDATA fnOpenThemeData = (PFNOPENTHEMEDATA)GetProcAddress(s_hUxTheme, "OpenThemeData");
		
		if (fnOpenThemeData)
			return fnOpenThemeData(hwnd, pszClassList);
	}
	
	return NULL;
}

BOOL CThemed::CloseThemeData(HTHEME hTheme)
{
	if (InitUxTheme() && hTheme)
	{
		PFNCLOSETHEMEDATA fnCloseThemeData = (PFNCLOSETHEMEDATA)GetProcAddress(s_hUxTheme, "CloseThemeData");
		
		if (fnCloseThemeData)
			return SUCCEEDED(fnCloseThemeData(m_hTheme));
	}
	
	return FALSE;
}

BOOL CThemed::DrawThemeBackground(HDC hdc, int iPartId, int iStateId, LPCRECT pRect,
								  LPCRECT pClipRect)
{
	if (InitUxTheme() && m_hTheme)
	{
		PFNDRAWTHEMEBACKGROUND fnDrawThemeBackground = (PFNDRAWTHEMEBACKGROUND)GetProcAddress(s_hUxTheme, "DrawThemeBackground");
		
		if (fnDrawThemeBackground)
			return (SUCCEEDED(fnDrawThemeBackground(m_hTheme, hdc, iPartId, iStateId, pRect, pClipRect)));
	}
	
	return FALSE;
}

BOOL CThemed::EnableThemeDialogTexture(HWND hWnd, DWORD dwFlags)
{
	if (InitUxTheme())
	{
		PFNENABLETHEMEDIALOGTEXTURE fnEnableThemeDialogTexture = (PFNENABLETHEMEDIALOGTEXTURE)GetProcAddress(s_hUxTheme, "EnableThemeDialogTexture");

		if (fnEnableThemeDialogTexture)
			return (SUCCEEDED(fnEnableThemeDialogTexture(hWnd, dwFlags)));
	}

	return FALSE;
}

BOOL CThemed::DrawThemeParentBackground(HWND hWnd, HDC hdc, LPCRECT pRect)
{
	if (InitUxTheme() && m_hTheme)
	{
		PFNDRAWTHEMEPARENTBACKGROUND fnDrawThemeParentBackground = (PFNDRAWTHEMEPARENTBACKGROUND)GetProcAddress(s_hUxTheme, "DrawThemeParentBackground");
		
		if (fnDrawThemeParentBackground)
			return (SUCCEEDED(fnDrawThemeParentBackground(hWnd, hdc, pRect)));
	}
	
	return FALSE;
}

BOOL CThemed::DrawThemeText(HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, 
							DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect)
{
	if (InitUxTheme() && m_hTheme)
	{
		PFNDRAWTHEMETEXT fnDrawThemeText = (PFNDRAWTHEMETEXT)GetProcAddress(s_hUxTheme, "DrawThemeText");
		
		if (fnDrawThemeText)
			return (SUCCEEDED(fnDrawThemeText(m_hTheme, hdc, iPartId, iStateId, pszText, iCharCount, dwTextFlags, dwTextFlags2, pRect)));
	}
	
	return FALSE;
}

BOOL CThemed::DrawThemeEdge(HDC hdc, int iPartId, int iStateId, LPCRECT pDestRect, UINT uEdge, 
							UINT uFlags, RECT *pContentRect)
{
	if (InitUxTheme() && m_hTheme)
	{
		PFNDRAWTHEMEEDGE fnDrawThemeEdge = (PFNDRAWTHEMEEDGE)GetProcAddress(s_hUxTheme, "DrawThemeEdge");
		
		if (fnDrawThemeEdge)
			return (SUCCEEDED(fnDrawThemeEdge(m_hTheme, hdc, iPartId, iStateId, pDestRect, uEdge, uFlags, pContentRect)));
	}
	
	return FALSE;
}

BOOL CThemed::DrawThemeIcon(HDC hdc, int iPartId, int iStateId, LPCRECT pRect, HIMAGELIST himl, 
							int iImageIndex)
{
	if (InitUxTheme() && m_hTheme)
	{
		PFNDRAWTHEMEICON fnDrawThemeIcon = (PFNDRAWTHEMEICON)GetProcAddress(s_hUxTheme, "DrawThemeIcon");
		
		if (fnDrawThemeIcon)
			return (SUCCEEDED(fnDrawThemeIcon(m_hTheme, hdc, iPartId, iStateId, pRect, himl, iImageIndex)));
	}
	
	return FALSE;
}

BOOL CThemed::DrawThemeBorder(HDC hdc, int iStateId, LPCRECT pRect)
{
	if (InitUxTheme() && m_hTheme)
	{
		PFNDRAWTHEMEBORDER fnDrawThemeBorder = (PFNDRAWTHEMEBORDER)GetProcAddress(s_hUxTheme, "DrawThemeBorder");
		
		if (fnDrawThemeBorder)
			return (SUCCEEDED(fnDrawThemeBorder(m_hTheme, hdc, iStateId, pRect)));
	}
	
	return FALSE;
}

BOOL CThemed::GetThemePartSize(int iPartId, int iStateId, THEMESIZE eSize, SIZE *psz)
{
	if (InitUxTheme() && m_hTheme)
	{
		PFNGETTHEMEPARTSIZE fnGetThemePartSize = (PFNGETTHEMEPARTSIZE)GetProcAddress(s_hUxTheme, "GetThemePartSize");
		
		if (fnGetThemePartSize)
			return (SUCCEEDED(fnGetThemePartSize(m_hTheme, NULL, iPartId, iStateId, NULL, eSize, psz)));
	}
	
	return FALSE;
}

BOOL CThemed::GetThemeTextExtent(HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, 
								DWORD dwTextFlags, LPCRECT pBoundingRect, RECT *pExtentRect)
{
	if (InitUxTheme() && m_hTheme)
	{
		PFNGETTHEMETEXTEXTENT fnGetThemeTextExtent = (PFNGETTHEMETEXTEXTENT)GetProcAddress(s_hUxTheme, "GetThemeTextExtent");
		
		if (fnGetThemeTextExtent)
			return (SUCCEEDED(fnGetThemeTextExtent(m_hTheme, hdc, iPartId, iStateId, pszText, iCharCount, dwTextFlags, pBoundingRect, pExtentRect)));
	}
	
	return FALSE;
}

BOOL CThemed::GetThemeColor(int iPartId, int iStateId, int iPropId, COLORREF& color)
{
	if (InitUxTheme() && m_hTheme)
	{
		PFNGETTHEMECOLOR fnGetThemeColor = (PFNGETTHEMECOLOR)GetProcAddress(s_hUxTheme, "GetThemeColor");
		
		if (fnGetThemeColor)
			return (SUCCEEDED(fnGetThemeColor(m_hTheme, iPartId, iStateId, iPropId, &color)));
	}
	
	return FALSE;
}

BOOL CThemed::GetThemeBackgroundContentRect(HDC hdc, int iPartId, int iStateId, LPCRECT pBoundingRect, LPRECT pContentRect)
{
	if (InitUxTheme() && m_hTheme)
	{
		PFNGETTHEMEBACKGROUNDCONTENTRECT fnGetContentRect = (PFNGETTHEMEBACKGROUNDCONTENTRECT)
															GetProcAddress(s_hUxTheme, "GetThemeBackgroundContentRect");
		
		if (fnGetContentRect)
			return (SUCCEEDED(fnGetContentRect(m_hTheme, hdc, iPartId, iStateId, pBoundingRect, pContentRect)));
	}
	
	return FALSE;
}

// -----------------------------------------------------------------------------------------------------------

BOOL CThemed::GetThemeClassPartState(int nType, int nState, CString& sThClass, int& nThPart, int& nThState)
{
	sThClass.Empty();
	nThPart = 0;
	nThState = 0;
	
	switch (nType)
	{
	case DFC_BUTTON:
		{
			sThClass = "BUTTON";
			nThState = PBS_NORMAL;
			
			if (nState & DFCS_BUTTONPUSH) 
			{
				nThPart = BP_PUSHBUTTON;
				
				if (nState & (DFCS_CHECKED | DFCS_PUSHED))
				{
					nThState = PBS_PRESSED;
				}
				else if ((nState & DFCS_INACTIVE) == DFCS_INACTIVE)
				{
					nThState = PBS_DISABLED;
				}
				else if ((nState & DFCS_HOT) == DFCS_HOT)
				{
					nThState = PBS_HOT;
				}
			}
			else if (nState & DFCS_BUTTONRADIO) 
			{
				nThPart = BP_RADIOBUTTON;

				if (nState & (DFCS_CHECKED | DFCS_PUSHED))
				{
					if ((nState & DFCS_INACTIVE) == DFCS_INACTIVE)
					{
						nThState = RBS_CHECKEDDISABLED;
					}
					else if ((nState & DFCS_HOT) == DFCS_HOT)
					{
						nThState = RBS_CHECKEDHOT;
					}
					else
					{
						nThState = RBS_CHECKEDNORMAL;
					}
				}
			}
			else if ((nState & DFCS_BUTTONCHECK) == DFCS_BUTTONCHECK) 
			{
				nThPart = BP_CHECKBOX;
				
				if (nState & (DFCS_CHECKED | DFCS_PUSHED))
				{
					if ((nState & DFCS_INACTIVE) == DFCS_INACTIVE)
					{
						nThState = CBS_CHECKEDDISABLED;
					}
					else if ((nState & DFCS_HOT) == DFCS_HOT)
					{
						nThState = CBS_CHECKEDHOT;
					}
					else if ((nState & DFCS_MIXED) == DFCS_MIXED)
					{
						nThState = CBS_MIXEDNORMAL;
					}
					else
					{
						nThState = CBS_CHECKEDNORMAL;
					}
				}
				else
				{
					if ((nState & DFCS_INACTIVE) == DFCS_INACTIVE)
					{
						nThState = CBS_UNCHECKEDDISABLED;
					}
					else if ((nState & DFCS_HOT) == DFCS_HOT)
					{
						nThState = CBS_UNCHECKEDHOT;
					}
					else
					{
						nThState = CBS_UNCHECKEDNORMAL;
					}
				}
			}
			else 
				return FALSE;
		}
		break;
		
	case DFC_CAPTION:
		break;
		
	case DFC_MENU:
		break;
		
	case DFC_POPUPMENU:
		break;
		
	case DFC_SCROLL:
		{
			if (nState & DFCS_SCROLLCOMBOBOX) 
			{
				sThClass = "COMBOBOX";
				nThPart = CP_DROPDOWNBUTTON;
				nThState = CBXS_NORMAL;
				
				if (nState & (DFCS_CHECKED | DFCS_PUSHED))
					nThState = CBXS_PRESSED;
				
				else if (nState & DFCS_INACTIVE)
					nThState = CBXS_DISABLED;
				
				else if (nState & DFCS_HOT)
					nThState = CBXS_HOT;
			}
			else if (nState & DFCS_SCROLLSIZEGRIP)
			{
				sThClass = "SCROLLBAR";
				nThPart = SBP_SIZEBOX;
				nThState = (nState & DFCS_SCROLLLEFT) ? SZB_LEFTALIGN : SZB_RIGHTALIGN;
			}
			else if (nState & DFCS_SCROLLDOWN)
			{
				sThClass = "SCROLLBAR";
				nThPart = SBP_LOWERTRACKVERT;
				nThState = SCRBS_NORMAL;
			}
			else
			{
				ASSERT(0);
			}
		}
		break;
	}
	
	return (!sThClass.IsEmpty() && (nThPart && nThState));
}
