// GdiPlus.cpp: implementation of the CGdiPlus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GdiPlus.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

struct IMAGECODECINFO
{
    CLSID Clsid;
    GUID  FormatID;
    const WCHAR* CodecName;
    const WCHAR* DllName;
    const WCHAR* FormatDescription;
    const WCHAR* FilenameExtension;
    const WCHAR* MimeType;
    DWORD Flags;
    DWORD Version;
    DWORD SigCount;
    DWORD SigSize;
    const BYTE* SigPattern;
    const BYTE* SigMask;
};

//////////////////////////////////////////////////////////////////////

struct IMAGEMIMETYPE
{
	LPCWSTR szExt;
	LPCWSTR szMimeType;
};

static IMAGEMIMETYPE IMAGEMIMETYPES[] = 
{
	{ L"gif", L"gif" },
	{ L"jpg", L"jpeg" },
	{ L"jpeg", L"jpeg" },
	{ L"tif", L"tiff" },
	{ L"tiff", L"tiff" },
	{ L"bmp", L"bmp" },
	{ L"png", L"png" },
};
const int NUM_IMAGEMIMETYPES = (sizeof(IMAGEMIMETYPES) / sizeof(IMAGEMIMETYPE));

//////////////////////////////////////////////////////////////////////

CGdiPlusGraphics::CGdiPlusGraphics(HDC hDC, gdix_SmoothingMode smoothing) : m_graphics(NULL), m_hDC(NULL)
{
	VERIFY(CGdiPlus::CreateGraphics(hDC, &m_graphics));
	VERIFY(CGdiPlus::SetSmoothingMode(m_graphics, smoothing));
}

CGdiPlusGraphics::CGdiPlusGraphics(gdix_Bitmap* bitmap) : m_graphics(NULL), m_hDC(NULL)
{
	VERIFY(CGdiPlus::CreateGraphics(bitmap, &m_graphics));
}

CGdiPlusGraphics::~CGdiPlusGraphics()
{
	ReleaseDC();
	VERIFY(CGdiPlus::DeleteGraphics(m_graphics));
}

HDC CGdiPlusGraphics::GetHDC()
{
	if (!m_hDC)
		VERIFY(CGdiPlus::GetDC(m_graphics, &m_hDC));
	
	return m_hDC;
}

void CGdiPlusGraphics::ReleaseDC()
{
	if (m_hDC)
	{
		VERIFY(CGdiPlus::ReleaseDC(m_graphics, m_hDC));
		m_hDC = NULL;
	}
}

//////////////////////////////////////////////////////////////////////

CGdiPlusPen::CGdiPlusPen() : m_pen(NULL)
{
}

CGdiPlusPen::CGdiPlusPen(COLORREF color, int nWidth, gdix_PenStyle nStyle) : m_pen(NULL)
{
	Create(color, nWidth, nStyle);
}

BOOL CGdiPlusPen::Create(COLORREF color, int nWidth, gdix_PenStyle nStyle)
{
	if (!CGdiPlus::CreatePen(CGdiPlus::MakeARGB(color), (float)nWidth, &m_pen))
	{
		ASSERT(0);
		return FALSE;
	}

	if (nStyle != gdix_PenStyleSolid)
		VERIFY(SetStyle(nStyle));

	return TRUE;
}

BOOL CGdiPlusPen::SetStyle(gdix_PenStyle nStyle)
{
	return CGdiPlus::SetPenStyle(m_pen, nStyle);
}

CGdiPlusPen::~CGdiPlusPen()
{
	VERIFY(!m_pen || CGdiPlus::DeletePen(m_pen));
}

//////////////////////////////////////////////////////////////////////

CGdiPlusBrush::CGdiPlusBrush() : m_brush(NULL)
{
}

CGdiPlusBrush::CGdiPlusBrush(COLORREF color, BYTE alpha) : m_brush(NULL)
{
	Create(color, alpha);
}

BOOL CGdiPlusBrush::Create(COLORREF color, BYTE alpha)
{
	if (!CGdiPlus::CreateBrush(CGdiPlus::MakeARGB(color, alpha), &m_brush))
	{
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

CGdiPlusBrush::~CGdiPlusBrush()
{
	VERIFY(!m_brush || CGdiPlus::DeleteBrush(m_brush));
}

//////////////////////////////////////////////////////////////////////

CGdiPlusPointF::CGdiPlusPointF(int nX, int nY)
{
	x = (float)nX;
	y = (float)nY;
}

CGdiPlusPointF::CGdiPlusPointF(const POINT& pt)
{
	x = (float)pt.x;
	y = (float)pt.y;
}

CGdiPlusPointF::CGdiPlusPointF(const gdix_PointF& pt)
{
	x = pt.x;
	y = pt.y;
}

//////////////////////////////////////////////////////////////////////

CGdiPlusRectF::CGdiPlusRectF(int l, int t, int r, int b)
{
	x = (float)l;
	y = (float)t;
	w = (float)(r - l);
	h = (float)(b - t);
}

CGdiPlusRectF::CGdiPlusRectF(const RECT& rect)
{
	x = (float)rect.left;
	y = (float)rect.top;
	w = (float)(rect.right - rect.left);
	h = (float)(rect.bottom - rect.top);
}

CGdiPlusRectF::CGdiPlusRectF(const POINT& pt, const SIZE& size)
{
	x = (float)pt.x;
	y = (float)pt.y;
	w = (float)size.cx;
	h = (float)size.cy;
}

CGdiPlusRectF::CGdiPlusRectF(const POINT& ptTopLeft, const POINT& ptBottomRight)
{
	x = (float)ptTopLeft.x;
	y = (float)ptTopLeft.y;
	w = (float)(ptBottomRight.x - ptTopLeft.x);
	h = (float)(ptBottomRight.y - ptTopLeft.y);
}

CGdiPlusRectF::CGdiPlusRectF(const gdix_RectF& rect)
{
	x = rect.x;
	y = rect.y;
	w = rect.w;
	h = rect.h;
}

CGdiPlusRectF::CGdiPlusRectF(const gdix_PointF& ptTopLeft, const gdix_PointF& ptBottomRight)
{
	x = ptTopLeft.x;
	y = ptTopLeft.y;
	w = (ptBottomRight.x - ptTopLeft.x);
	h = (ptBottomRight.y - ptTopLeft.y);
}

//////////////////////////////////////////////////////////////////////

CGdiPlusBitmap::CGdiPlusBitmap(IStream* stream) : m_bitmap(NULL)
{
	CGdiPlus::CreateBitmapFromStream(stream, &m_bitmap);
}

CGdiPlusBitmap::CGdiPlusBitmap(const WCHAR* filename) : m_bitmap(NULL)
{
	CGdiPlus::CreateBitmapFromFile(filename, &m_bitmap);
}

CGdiPlusBitmap::CGdiPlusBitmap(HBITMAP hbitmap) : m_bitmap(NULL)
{
	CGdiPlus::CreateBitmapFromHBITMAP(hbitmap, NULL, &m_bitmap);
}

CGdiPlusBitmap::CGdiPlusBitmap(int width, int height, gdix_PixelFormat format)
{
	CGdiPlus::CreateBitmap(width, height, &m_bitmap, format);
}

BOOL CGdiPlusBitmap::SaveAsFile(const WCHAR* filename)
{
	return CGdiPlus::SaveBitmapToFile(m_bitmap, filename);
}

CGdiPlusBitmap::~CGdiPlusBitmap()
{
	CGdiPlus::DeleteBitmap(m_bitmap);
}

//////////////////////////////////////////////////////////////////////

struct gdix_StartupInput
{
    UINT32 GdiplusVersion;
    void* DebugEventCallback;  /* DebugEventProc */
    BOOL SuppressBackgroundThread;
    BOOL SuppressExternalCodecs;
};

struct gdix_StartupOutput
{
	// not used
};

//////////////////////////////////////////////////////////////////////

typedef gdix_Status (STDAPICALLTYPE* PFNSTARTUP)(ULONG_PTR*, const gdix_StartupInput*, gdix_StartupOutput*);
typedef void		(STDAPICALLTYPE* PFNSHUTDOWN)(ULONG_PTR);

// Graphics management 
typedef gdix_Status (STDAPICALLTYPE *PFNCREATEFROMHDC)(HDC,gdix_Graphics**);
typedef gdix_Status (STDAPICALLTYPE *PFNDELETEGRAPHICS)(gdix_Graphics*);
typedef gdix_Status (STDAPICALLTYPE *PFNSETSMOOTHINGMODE)(gdix_Graphics*,gdix_SmoothingMode);
typedef gdix_Status (STDAPICALLTYPE *PFNGETIMAGEGRAPHICSCONTEXT)(gdix_Image*, gdix_Graphics**);
typedef gdix_Status (STDAPICALLTYPE *PFNGETDC)(gdix_Graphics* graphics, HDC* hdc);
typedef gdix_Status (STDAPICALLTYPE *PFNRELEASEDC)(gdix_Graphics* graphics, HDC hdc);

// Pen management 
typedef gdix_Status (STDAPICALLTYPE *PFNCREATEPEN1)(gdix_ARGB,gdix_Real,gdix_Unit,gdix_Pen**);
typedef gdix_Status (STDAPICALLTYPE *PFNDELETEPEN)(gdix_Pen*);
typedef gdix_Status (STDAPICALLTYPE *PFNSETPENWIDTH)(gdix_Pen*,gdix_Real);
typedef gdix_Status (STDAPICALLTYPE *PFNSETPENCOLOR)(gdix_Pen*,gdix_ARGB);
typedef gdix_Status (STDAPICALLTYPE *PFNSETPENSTYLE)(gdix_Pen*,gdix_PenStyle);

// Brush management 
typedef gdix_Status (STDAPICALLTYPE *PFNCREATESOLIDFILL)(gdix_ARGB,gdix_SolidFill**);
typedef gdix_Status (STDAPICALLTYPE *PFNDELETEBRUSH)(gdix_Brush*);
typedef gdix_Status (STDAPICALLTYPE *PFNSETSOLIDFILLCOLOR)(gdix_SolidFill*,gdix_ARGB);

// Font management 
typedef gdix_Status (STDAPICALLTYPE *PFNCREATEFONTFROMDC)(HDC,gdix_Font**);
typedef gdix_Status (STDAPICALLTYPE *PFNCREATEFONTFROMLOGFONTW)(HDC,const LOGFONTW*,gdix_Font**);
typedef gdix_Status (STDAPICALLTYPE *PFNDELETEFONT)(gdix_Font*);

// String format management 
typedef gdix_Status (STDAPICALLTYPE *PFNCREATESTRINGFORMAT)(INT,LANGID,gdix_StringFormat**);
typedef gdix_Status (STDAPICALLTYPE *PFNDELETESTRINGFORMAT)(gdix_StringFormat*);
typedef gdix_Status (STDAPICALLTYPE *PFNSETSTRINGFORMATFLAGS)(gdix_StringFormat*,INT);
typedef gdix_Status (STDAPICALLTYPE *PFNSETSTRINGFORMATALIGN)(gdix_StringFormat*,gdix_StringAlignment);

// Path management 
typedef gdix_Status (STDAPICALLTYPE *PFNCREATEPATH)(gdix_FillMode,gdix_Path**);
typedef gdix_Status (STDAPICALLTYPE *PFNDELETEPATH)(gdix_Path*);
typedef gdix_Status (STDAPICALLTYPE *PFNRESETPATH)(gdix_Path*);
typedef gdix_Status (STDAPICALLTYPE *PFNADDPATHARC)(gdix_Path*,gdix_Real,gdix_Real,gdix_Real,gdix_Real,gdix_Real,gdix_Real);
typedef gdix_Status (STDAPICALLTYPE *PFNADDPATHLINE)(gdix_Path*,gdix_Real,gdix_Real,gdix_Real,gdix_Real);
typedef gdix_Status (STDAPICALLTYPE *PFNADDPATHRECTANGLE)(gdix_Path*,gdix_Real,gdix_Real,gdix_Real,gdix_Real);

// Draw methods 
typedef gdix_Status (STDAPICALLTYPE *PFNDRAWLINE)(gdix_Graphics*,gdix_Pen*,gdix_Real,gdix_Real,gdix_Real,gdix_Real);
typedef gdix_Status (STDAPICALLTYPE *PFNDRAWLINES)(gdix_Graphics*,gdix_Pen*,const gdix_PointF*,INT);
typedef gdix_Status (STDAPICALLTYPE *PFNDRAWPIE)(gdix_Graphics*,gdix_Pen*,gdix_Real,gdix_Real,gdix_Real,gdix_Real,gdix_Real,gdix_Real);
typedef gdix_Status (STDAPICALLTYPE *PFNDRAWARC)(gdix_Graphics*,gdix_Pen*,gdix_Real,gdix_Real,gdix_Real,gdix_Real,gdix_Real,gdix_Real);
typedef gdix_Status (STDAPICALLTYPE *PFNDRAWLINES)(gdix_Graphics*,gdix_Pen*,const gdix_PointF*,INT);
typedef gdix_Status (STDAPICALLTYPE *PFNDRAWPOLYGON)(gdix_Graphics*,gdix_Pen*,const gdix_PointF*,INT);
typedef gdix_Status (STDAPICALLTYPE *PFNDRAWELLIPSE)(gdix_Graphics*,gdix_Pen*,gdix_Real,gdix_Real,gdix_Real,gdix_Real);
typedef gdix_Status (STDAPICALLTYPE *PFNDRAWRECTANGLE)(gdix_Graphics*,gdix_Pen*,gdix_Real,gdix_Real,gdix_Real,gdix_Real);

// Fill methods 
typedef gdix_Status (STDAPICALLTYPE *PFNFILLRECTANGLE)(gdix_Graphics*,gdix_Brush*,gdix_Real,gdix_Real,gdix_Real,gdix_Real);
typedef gdix_Status (STDAPICALLTYPE *PFNFILLPOLYGON)(gdix_Graphics*,gdix_Brush*,const gdix_PointF*,INT,gdix_FillMode);
typedef gdix_Status (STDAPICALLTYPE *PFNFILLPOLYGON2)(gdix_Graphics*,gdix_Brush*,const gdix_PointF*,INT);
typedef gdix_Status (STDAPICALLTYPE *PFNFILLELLIPSE)(gdix_Graphics*,gdix_Brush*,gdix_Real,gdix_Real,gdix_Real,gdix_Real);
typedef gdix_Status (STDAPICALLTYPE *PFNFILLPIE)(gdix_Graphics*,gdix_Brush*,gdix_Real,gdix_Real,gdix_Real,gdix_Real,gdix_Real,gdix_Real);
typedef gdix_Status (STDAPICALLTYPE *PFNFILLPATH)(gdix_Graphics*,gdix_Brush*,gdix_Path*);

// String methods 
typedef gdix_Status (STDAPICALLTYPE *PFNDRAWSTRING)(gdix_Graphics*,const WCHAR*,INT,const gdix_Font*,const gdix_RectF*,const gdix_StringFormat*,const gdix_Brush*);
typedef gdix_Status (STDAPICALLTYPE *PFNMEASURESTRING)(gdix_Graphics*,const WCHAR*,INT,const gdix_Font*,const gdix_RectF*,const gdix_StringFormat*,gdix_RectF*,INT*,INT*);

// Bitmap methods
typedef gdix_Status (STDAPICALLTYPE *PFNCREATEBITMAPFROMSTREAM)(IStream*, gdix_Bitmap**);
typedef gdix_Status (STDAPICALLTYPE *PFNCREATEBITMAPFROMFILE)(const WCHAR*, gdix_Bitmap**);
typedef gdix_Status (STDAPICALLTYPE *PFNCREATEHBITMAPFROMBITMAP)(gdix_Bitmap*, HBITMAP*, gdix_ARGB);
typedef gdix_Status (STDAPICALLTYPE *PFNCREATEHICONFROMBITMAP)(gdix_Bitmap*, HICON*);
typedef gdix_Status (STDAPICALLTYPE *PFNCREATEBITMAPFROMHBITMAP)(HBITMAP, HPALETTE, gdix_Bitmap**);
typedef gdix_Status (STDAPICALLTYPE *PFNCREATEBITMAPFROMSCAN0)(int, int, int, int, byte*, gdix_Bitmap**);
typedef gdix_Status (STDAPICALLTYPE *PFNDELETEBITMAP)(gdix_Bitmap*);
typedef gdix_Status (STDAPICALLTYPE *PFNCREATEBITMAPFROMFILE2)(const WCHAR*, gdix_Bitmap**);
typedef gdix_Status (STDAPICALLTYPE *PFNCREATEHBITMAPFROMBITMAP2)(gdix_Bitmap*, HBITMAP*, gdix_ARGB);

// Image methods
typedef gdix_Status(STDAPICALLTYPE *PFNSAVEIMAGETOFILE)(gdix_Image*, const WCHAR*, const CLSID*, const void*);
typedef gdix_Status(STDAPICALLTYPE *PFNSAVEIMAGETOSTREAM)(gdix_Image*, IStream* pStream, const CLSID*, const void*);

// Misc
typedef gdix_Status (STDAPICALLTYPE *PFNGETIMAGEENCODERS)(UINT, UINT, IMAGECODECINFO*);
typedef gdix_Status (STDAPICALLTYPE *PFNGETIMAGEENCODERSSIZE)(UINT*, UINT*);

//////////////////////////////////////////////////////////////////////

HMODULE   CGdiPlus::s_hGdiPlus = HMODULE(-1);
ULONG_PTR CGdiPlus::s_GdiToken = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CGdiPlus::Initialize()
{
	if (s_hGdiPlus == HMODULE(-1)) // one time only
	{
		s_hGdiPlus = LoadLibrary(_T("GdiPlus.dll"));

		if (s_hGdiPlus)
		{
		    PFNSTARTUP pFn = (PFNSTARTUP)GetProcAddress(s_hGdiPlus, "GdiplusStartup");

			if (pFn)
			{
			    gdix_StartupInput input = { 0 };

				input.GdiplusVersion = 1;
				input.SuppressExternalCodecs = TRUE;

				if (pFn(&s_GdiToken, &input, NULL) == gdix_Ok) 
				{
					return TRUE;
				}
			}

			// failed
			FreeLibrary(s_hGdiPlus);

			s_hGdiPlus = NULL;
			s_GdiToken = NULL;
		}
	}
	
	return (NULL != s_hGdiPlus);
}

void CGdiPlus::Free()
{
#if _MSC_VER <= 1200
	// For reasons I don't understand trying to 
	// access s_hGdiPlus under VC6 causes a crash due
	// to a memory corruption (possibly of s_hGdiPlus)
	// so for now we just let Windows do the cleanup
#else
	if (s_hGdiPlus && (s_hGdiPlus != HMODULE(-1)))
	{
		ASSERT(s_GdiToken);

		PFNSHUTDOWN pFn = (PFNSHUTDOWN)GetProcAddress(s_hGdiPlus, "GdiplusShutdown");

		if (pFn)
			pFn(s_GdiToken);

		FreeLibrary(s_hGdiPlus);

		s_hGdiPlus = HMODULE(-1);
		s_GdiToken = NULL;
	}
#endif
}

//////////////////////////////////////////////////////////////////////

enum
{
	AlphaShift  = 24,
	RedShift    = 16,
	GreenShift  = 8,
	BlueShift   = 0
};

gdix_ARGB CGdiPlus::MakeARGB(COLORREF color, BYTE alpha)
{
	return (((gdix_ARGB) (GetBValue(color)) << BlueShift) |
			((gdix_ARGB) (GetGValue(color)) << GreenShift) |
			((gdix_ARGB) (GetRValue(color)) << RedShift) |
			((gdix_ARGB) (alpha) << AlphaShift));
}

//////////////////////////////////////////////////////////////////////

#define GETPROCADDRESS(tdef, fnName)                            \
	if (!Initialize()) return FALSE;                            \
	static tdef pFN = (tdef)GetProcAddress(s_hGdiPlus, fnName); \
	if (!pFN) return FALSE;

//////////////////////////////////////////////////////////////////////

BOOL CGdiPlus::CreateBitmapFromStream(IStream* stream, gdix_Bitmap **bitmap)
{
	GETPROCADDRESS(PFNCREATEBITMAPFROMSTREAM, "GdipCreateBitmapFromStream");
	return (pFN(stream, bitmap) == gdix_Ok);
}

BOOL CGdiPlus::CreateBitmapFromFile(const WCHAR* filename, gdix_Bitmap **bitmap)
{
	GETPROCADDRESS(PFNCREATEBITMAPFROMFILE, "GdipCreateBitmapFromFile");
	return (pFN(filename, bitmap) == gdix_Ok);
}

BOOL CGdiPlus::CreateHBITMAPFromBitmap(gdix_Bitmap* bitmap, HBITMAP* hbmReturn, gdix_ARGB background)
{
	*hbmReturn = NULL;

	GETPROCADDRESS(PFNCREATEHBITMAPFROMBITMAP, "GdipCreateHBITMAPFromBitmap");
	return (pFN(bitmap, hbmReturn, background) == gdix_Ok);
}

BOOL CGdiPlus::CreateHICONFromBitmap(gdix_Bitmap* bitmap, HICON* hicoReturn)
{
	*hicoReturn = NULL;

	GETPROCADDRESS(PFNCREATEHICONFROMBITMAP, "GdipCreateHICONFromBitmap");
	return (pFN(bitmap, hicoReturn) == gdix_Ok);
}

BOOL CGdiPlus::CreateBitmapFromHBITMAP(HBITMAP hbitmap, HPALETTE hPal, gdix_Bitmap **bitmap)
{
	GETPROCADDRESS(PFNCREATEBITMAPFROMHBITMAP, "GdipCreateBitmapFromHBITMAP");
	return (pFN(hbitmap, hPal, bitmap) == gdix_Ok);
}

BOOL CGdiPlus::CreateBitmap(int width, int height, gdix_Bitmap **bitmap, gdix_PixelFormat format)
{
	GETPROCADDRESS(PFNCREATEBITMAPFROMSCAN0, "GdipCreateBitmapFromScan0");
	return (pFN(width, height, 0, (int)format, NULL, bitmap) == gdix_Ok);
}

BOOL CGdiPlus::GetImageMimeType(const WCHAR* filename, CString& sMimeType)
{
	CString sExt;

#if _MSC_VER >= 1400
	_wsplitpath_s(filename, NULL, 0, NULL, 0, NULL, 0, sExt.GetBuffer(_MAX_EXT + 1), _MAX_EXT);
#else
	_wsplitpath(filename, NULL, NULL, NULL, sExt.GetBuffer(_MAX_EXT + 1));
#endif
	sExt.ReleaseBuffer();

	if (sExt.IsEmpty())
		return FALSE;

	if (sExt[0] == '.')
		sExt = sExt.Mid(1);

	int nType = NUM_IMAGEMIMETYPES;

	while (nType--)
	{
		if (sExt.CompareNoCase(IMAGEMIMETYPES[nType].szExt) == 0)
		{
			sMimeType.Format(L"image/%s", IMAGEMIMETYPES[nType].szMimeType);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CGdiPlus::SaveBitmapToFile(gdix_Bitmap* bitmap, const WCHAR* filename)
{
	if (!bitmap)
		return FALSE;

	// Use the file extension to determine the required encoder
	CString sMimeType;

	if (!GetImageMimeType(filename, sMimeType))
		return FALSE;

	CLSID clsidEncoder = { 0 };

	if (!GetEncoderClsid(sMimeType, &clsidEncoder))
		return FALSE;

	GETPROCADDRESS(PFNSAVEIMAGETOFILE, "GdipSaveImageToFile");
	
	return (pFN((gdix_Image*)bitmap, filename, &clsidEncoder, NULL) == gdix_Ok);
}

BOOL CGdiPlus::SaveBitmapToStream(gdix_Bitmap* bitmap, IStream* pStream, const CLSID& clsidEncoder)
{
	if (!bitmap)
		return FALSE;

	GETPROCADDRESS(PFNSAVEIMAGETOSTREAM, "GdipSaveImageToStream");
	
	return (pFN((gdix_Image*)bitmap, pStream, &clsidEncoder, NULL) == gdix_Ok);
}

BOOL CGdiPlus::DeleteBitmap(gdix_Bitmap* bitmap)
{
	if (!bitmap)
		return FALSE;

	GETPROCADDRESS(PFNDELETEBITMAP, "GdipDisposeImage");
	return (pFN(bitmap) == gdix_Ok);
}

BOOL CGdiPlus::CreatePen(gdix_ARGB color, gdix_Real width, gdix_Pen** pen)
{
	GETPROCADDRESS(PFNCREATEPEN1, "GdipCreatePen1");

	return (pFN(color, width, gdix_UnitPixel, pen) == gdix_Ok);
}

BOOL CGdiPlus::DeletePen(gdix_Pen* pen)
{
	if (!pen)
		return FALSE;
	
	GETPROCADDRESS(PFNDELETEPEN, "GdipDeletePen");

	return (pFN(pen) == gdix_Ok);
}

BOOL CGdiPlus::SetPenStyle(gdix_Pen* pen, gdix_PenStyle style)
{
	if (!pen)
		return FALSE;

	GETPROCADDRESS(PFNSETPENSTYLE, "GdipSetPenDashStyle");

	return (pFN(pen, style) == gdix_Ok);
}

BOOL CGdiPlus::CreateBrush(gdix_ARGB color, gdix_Brush** brush)
{
	GETPROCADDRESS(PFNCREATESOLIDFILL, "GdipCreateSolidFill");
	
	return (pFN(color, brush) == gdix_Ok);
}

BOOL CGdiPlus::DeleteBrush(gdix_Brush* brush)
{
	if (!brush)
		return FALSE;
	
	GETPROCADDRESS(PFNDELETEBRUSH, "GdipDeleteBrush");
	
	return (pFN(brush) == gdix_Ok);
}

BOOL CGdiPlus::CreateGraphics(HDC hdc, gdix_Graphics** graphics)
{
	GETPROCADDRESS(PFNCREATEFROMHDC, "GdipCreateFromHDC");

	return (pFN(hdc, graphics) == gdix_Ok);
}

BOOL CGdiPlus::CreateGraphics(gdix_Bitmap* bitmap, gdix_Graphics** graphics)
{
	GETPROCADDRESS(PFNGETIMAGEGRAPHICSCONTEXT, "GdipGetImageGraphicsContext");

	return (pFN((gdix_Image*)bitmap, graphics) == gdix_Ok);
}

BOOL CGdiPlus::DeleteGraphics(gdix_Graphics* graphics)
{
	if (!graphics)
		return FALSE;
	
	GETPROCADDRESS(PFNDELETEGRAPHICS, "GdipDeleteGraphics");

	return (pFN(graphics) == gdix_Ok);
}

BOOL CGdiPlus::GetDC(gdix_Graphics* graphics, HDC* hDC)
{
	if (!graphics)
		return FALSE;

	GETPROCADDRESS(PFNGETDC, "GdipGetDC");

	return (pFN(graphics, hDC) == gdix_Ok);
}

BOOL CGdiPlus::ReleaseDC(gdix_Graphics* graphics, HDC hDC)
{
	if (!graphics)
		return FALSE;

	GETPROCADDRESS(PFNRELEASEDC, "GdipReleaseDC");

	return (pFN(graphics, hDC) == gdix_Ok);
}

BOOL CGdiPlus::SetSmoothingMode(gdix_Graphics* graphics, gdix_SmoothingMode mode)
{
	GETPROCADDRESS(PFNSETSMOOTHINGMODE, "GdipSetSmoothingMode");

	return (pFN(graphics, mode) == gdix_Ok);
}

BOOL CGdiPlus::DrawLine(gdix_Graphics* graphics, gdix_Pen* pen, const gdix_PointF* from, const gdix_PointF* to)
{
	GETPROCADDRESS(PFNDRAWLINE, "GdipDrawLine");

	return (pFN(graphics, pen, from->x, from->y, to->x, to->y) == gdix_Ok);
}

BOOL CGdiPlus::DrawLines(gdix_Graphics* graphics, gdix_Pen* pen, const gdix_PointF* points, int count)
{
	GETPROCADDRESS(PFNDRAWLINES, "GdipDrawLines");

	return (pFN(graphics, pen, points, count) == gdix_Ok);
}

BOOL CGdiPlus::DrawPolygon(gdix_Graphics* graphics, gdix_Pen* pen, const gdix_PointF* points, int count, gdix_Brush* brush)
{
	if (brush && !FillPolygon(graphics, brush, points, count))
	{
		ASSERT(0);
		return FALSE;
	}

	GETPROCADDRESS(PFNDRAWPOLYGON, "GdipDrawPolygon");

	return (pFN(graphics, pen, points, count) == gdix_Ok);
}

BOOL CGdiPlus::DrawEllipse(gdix_Graphics* graphics, gdix_Pen* pen, const gdix_RectF* rect, gdix_Brush* brush)
{
	if (brush && !FillEllipse(graphics, brush, rect))
	{
		ASSERT(0);
		return FALSE;
	}
	
	GETPROCADDRESS(PFNDRAWELLIPSE, "GdipDrawEllipse");
	
	return (pFN(graphics, pen, rect->x, rect->y, rect->w, rect->h) == gdix_Ok);
}

BOOL CGdiPlus::DrawRect(gdix_Graphics* graphics, gdix_Pen* pen, const gdix_RectF* rect, gdix_Brush* brush)
{
	if (brush && !FillRect(graphics, brush, rect))
	{
		ASSERT(0);
		return FALSE;
	}
	
	GETPROCADDRESS(PFNDRAWRECTANGLE, "GdipDrawRectangle");
	
	return (pFN(graphics, pen, rect->x, rect->y, rect->w, rect->h) == gdix_Ok);
}

BOOL CGdiPlus::DrawPie(gdix_Graphics* graphics, gdix_Pen* pen, const gdix_RectF* rect, float startDegrees, float sweepDegrees, gdix_Brush* brush)
{
	if (brush && !FillPie(graphics, brush, rect, startDegrees, sweepDegrees))
	{
		ASSERT(0);
		return FALSE;
	}
	
	GETPROCADDRESS(PFNDRAWPIE, "GdipDrawPie");
	
	return (pFN(graphics, pen, rect->x, rect->y, rect->w, rect->h, startDegrees, sweepDegrees) == gdix_Ok);
}

BOOL CGdiPlus::DrawArc(gdix_Graphics* graphics, gdix_Pen* pen, const gdix_RectF* rect, float startDegrees, float sweepDegrees)
{
	GETPROCADDRESS(PFNDRAWARC, "GdipDrawArc");

	return (pFN(graphics, pen, rect->x, rect->y, rect->w, rect->h, startDegrees, sweepDegrees) == gdix_Ok);
}

BOOL CGdiPlus::FillPolygon(gdix_Graphics* graphics, gdix_Brush* brush, const gdix_PointF* points, int count)
{
	GETPROCADDRESS(PFNFILLPOLYGON2, "GdipFillPolygon2");
	
	return (pFN(graphics, brush, points, count) == gdix_Ok);
}

BOOL CGdiPlus::FillEllipse(gdix_Graphics* graphics, gdix_Brush* brush, const gdix_RectF* rect)
{
	GETPROCADDRESS(PFNFILLELLIPSE, "GdipFillEllipse");
	
	return (pFN(graphics, brush, rect->x, rect->y, rect->w, rect->h) == gdix_Ok);
}

BOOL CGdiPlus::FillRect(gdix_Graphics* graphics, gdix_Brush* brush, const gdix_RectF* rect)
{
	GETPROCADDRESS(PFNFILLRECTANGLE, "GdipFillRectangle");
	
	return (pFN(graphics, brush, rect->x, rect->y, rect->w, rect->h) == gdix_Ok);
}

BOOL CGdiPlus::FillPie(gdix_Graphics* graphics, gdix_Brush* brush, const gdix_RectF* rect, float startDegrees, float sweepDegrees)
{
	GETPROCADDRESS(PFNFILLPIE, "GdipFillPie");
	
	return (pFN(graphics, brush, rect->x, rect->y, rect->w, rect->h, startDegrees, sweepDegrees) == gdix_Ok);
}

// Win32 support -----------------------------------------------------------------------------------------

BOOL CGdiPlus::DrawLine(CGdiPlusGraphics& graphics, CGdiPlusPen& pen, const POINT& from, const POINT& to)
{
	return DrawLine(graphics, pen, CGdiPlusPointF(from), CGdiPlusPointF(to));
}

BOOL CGdiPlus::DrawLines(CGdiPlusGraphics& graphics, CGdiPlusPen& pen, const POINT points[], int count)
{
	CArray<gdix_PointF, gdix_PointF&> pointFs;
	GetPointFs(points, count, pointFs);

	return DrawLines(graphics, pen, pointFs.GetData(), count);
}

BOOL CGdiPlus::DrawPolygon(CGdiPlusGraphics& graphics, CGdiPlusPen& pen, const POINT points[], int count, gdix_Brush* brush)
{
	CArray<gdix_PointF, gdix_PointF&> pointFs;
	GetPointFs(points, count, pointFs);

	return DrawPolygon(graphics, pen, pointFs.GetData(), count, brush);
}

BOOL CGdiPlus::DrawEllipse(gdix_Graphics* graphics, gdix_Pen* pen, const RECT& rect, gdix_Brush* brush)
{
	return DrawEllipse(graphics, pen, CGdiPlusRectF(rect), brush);
}

BOOL CGdiPlus::DrawRect(gdix_Graphics* graphics, gdix_Pen* pen, const RECT& rect, gdix_Brush* brush)
{
	return DrawRect(graphics, pen, CGdiPlusRectF(rect), brush);
}

BOOL CGdiPlus::DrawPie(gdix_Graphics* graphics, gdix_Pen* pen, const RECT& rect, float startDegrees, float sweepDegrees, gdix_Brush* brush)
{
	return DrawPie(graphics, pen, CGdiPlusRectF(rect), startDegrees, sweepDegrees, brush);
}

BOOL CGdiPlus::DrawArc(gdix_Graphics* graphics, gdix_Pen* pen, const RECT& rect, float startDegrees, float sweepDegrees)
{
	return DrawArc(graphics, pen, CGdiPlusRectF(rect), startDegrees, sweepDegrees);
}

BOOL CGdiPlus::FillPolygon(gdix_Graphics* graphics, gdix_Brush* brush, const POINT points[], int count)
{
	CArray<gdix_PointF, gdix_PointF&> pointFs;
	GetPointFs(points, count, pointFs);

	return FillPolygon(graphics, brush, pointFs.GetData(), count);
}

BOOL CGdiPlus::FillEllipse(gdix_Graphics* graphics, gdix_Brush* brush, const RECT& rect)
{
	return FillEllipse(graphics, brush, CGdiPlusRectF(rect));
}

BOOL CGdiPlus::FillRect(gdix_Graphics* graphics, gdix_Brush* brush, const RECT& rect)
{
	return FillRect(graphics, brush, CGdiPlusRectF(rect));
}

BOOL CGdiPlus::FillPie(gdix_Graphics* graphics, gdix_Brush* brush, const RECT& rect, float startDegrees, float sweepDegrees)
{
	return FillPie(graphics, brush, CGdiPlusRectF(rect), startDegrees, sweepDegrees);
}

void CGdiPlus::GetPointFs(const POINT points[], int count, CArray<gdix_PointF, gdix_PointF&>& pointFs)
{
	pointFs.SetSize(count);

	for (int nPt = 0; nPt < count; nPt++)
	{
		pointFs[nPt].x = (float)points[nPt].x;
		pointFs[nPt].y = (float)points[nPt].y;
	}
}

BOOL CGdiPlus::GetImageEncoders(UINT numEncoders, UINT sizeEncoderArray, IMAGECODECINFO* pEncoders)
{
	GETPROCADDRESS(PFNGETIMAGEENCODERS, "GdipGetImageEncoders");
	
	return (pFN(numEncoders, sizeEncoderArray, pEncoders) == gdix_Ok);
}

BOOL CGdiPlus::GetImageEncodersSize(UINT* numEncoders, UINT* sizeEncoderArray)
{
	GETPROCADDRESS(PFNGETIMAGEENCODERSSIZE, "GdipGetImageEncodersSize");
	
	return (pFN(numEncoders, sizeEncoderArray) == gdix_Ok);
}

BOOL CGdiPlus::GetEncoderClsid(const WCHAR* szFormat, CLSID* pClsid)
{
    UINT numEncoders = 0;
    UINT sizeEncoderArray = 0;
	
    if (!GetImageEncodersSize(&numEncoders, &sizeEncoderArray) || !sizeEncoderArray)
		return FALSE;

    IMAGECODECINFO* pImageCodecInfo = (IMAGECODECINFO*)malloc(sizeEncoderArray);

    if (pImageCodecInfo == NULL)
		return FALSE;
	
    VERIFY (GetImageEncoders(numEncoders, sizeEncoderArray, pImageCodecInfo));
	
    for (UINT nEncoder = 0; nEncoder < numEncoders; ++nEncoder)
    {
        if (wcscmp(pImageCodecInfo[nEncoder].MimeType, szFormat) == 0)
        {
            *pClsid = pImageCodecInfo[nEncoder].Clsid;
            free(pImageCodecInfo);

            return TRUE;
        }
    }
	
    free(pImageCodecInfo);
    return FALSE;
}