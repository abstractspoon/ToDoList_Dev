// GdiPlus.h: interface for the CGdiPlus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GDIPLUS_H__41B5F6AC_E7D4_49CA_9E2F_96FDA94EBB28__INCLUDED_)
#define AFX_GDIPLUS_H__41B5F6AC_E7D4_49CA_9E2F_96FDA94EBB28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

/*
#include <gdiplusflat.h>

//---------------------------------------------------------------------------
// GDI+ classes for forward reference
//---------------------------------------------------------------------------

class Graphics;
class Pen;
class Brush;
class Matrix;
class Bitmap;
class Metafile;
class GraphicsPath;
class PathIterator;
class Region;
class Image;
class TextureBrush;
class HatchBrush;
class SolidBrush;
class LinearGradientBrush;
class PathGradientBrush;
class Font;
class FontFamily;
class FontCollection;
class InstalledFontCollection;
class PrivateFontCollection;
class ImageAttributes;
class CachedBitmap;

//---------------------------------------------------------------------------
// Private GDI+ classes for internal type checking
//---------------------------------------------------------------------------
class GpGraphics {};

class GpBrush {};
class GpTexture : public GpBrush {};
class GpSolidFill : public GpBrush {};
class GpLineGradient : public GpBrush {};
class GpPathGradient : public GpBrush {};
class GpHatch : public GpBrush {};

class GpPen {};
class GpCustomLineCap {};
class GpAdjustableArrowCap : public GpCustomLineCap {};

class GpImage {};
class GpBitmap : public GpImage {};
class GpMetafile : public GpImage {};
class GpImageAttributes {};

class GpPath {};
class GpRegion {};
class GpPathIterator {};

class GpFontFamily {};
class GpFont {};
class GpStringFormat {};
class GpFontCollection {};
class GpInstalledFontCollection : public GpFontCollection {};
class GpPrivateFontCollection : public GpFontCollection {};

class GpCachedBitmap;
*/

//////////////////////////////////////////////////////////////////////

typedef void* gdix_Graphics;
typedef void* gdix_SolidFill;
typedef void* gdix_Brush;
typedef void* gdix_Pen;
typedef void* gdix_Font;
typedef void* gdix_StringFormat;
typedef void* gdix_Path;
typedef void* gdix_Bitmap;
typedef void* gdix_Image;

typedef float gdix_Real;
typedef DWORD gdix_ARGB;

//////////////////////////////////////////////////////////////////////

struct IMAGECODECINFO;

//////////////////////////////////////////////////////////////////////

enum gdix_Status 
{
	gdix_Ok = 0
};

enum gdix_Unit 
{
	gdix_UnitWorld = 0,
	gdix_UnitPixel = 2,
};

enum gdix_SmoothingMode 
{
	gdix_SmoothingModeNone = 0,
	gdix_SmoothingModeLow = 2,
	gdix_SmoothingModeMedium = 4,
	gdix_SmoothingModeHigh = 5
};

enum gdix_StringAlignment 
{
	gdix_StringAlignmentCenter = 1,
	gdix_StringAlignmentFar = 2
};

enum gdix_FillMode  
{
	gdix_FillModeAlternate = 0
};

enum gdix_PenStyle
{
	gdix_PenStyleSolid = 0,
	gdix_PenStyleDash,
	gdix_PenStyleDot,
	gdix_PenStyleDashDot,
	gdix_PenStyleDashDotDot,
	//gdix_PenStyleCustom
};

#define PixelFormatGDI          0x00020000 // Is a GDI-supported format
#define PixelFormatAlpha        0x00040000 // Has an alpha component
#define PixelFormatCanonical    0x00200000 

enum gdix_PixelFormat
{
	gdiX_PixelFormat24bppRGB =  (8 | (24 << 8) | PixelFormatGDI),
	gdix_PixelFormat32bppRGB =  (9 | (32 << 8) | PixelFormatGDI),
	gdix_PixelFormat32bppARGB = (10 | (32 << 8) | PixelFormatAlpha | PixelFormatGDI | PixelFormatCanonical),
};

//////////////////////////////////////////////////////////////////////

#define gdix_StringFormatFlagsNoWrap   0x00001000
#define gdix_StringFormatFlagsNoClip   0x00004000

//////////////////////////////////////////////////////////////////////

#if _MSC_VER < 1400
#	ifndef ULONG_PTR
		typedef ULONG ULONG_PTR;
#	endif
#endif

//////////////////////////////////////////////////////////////////////

struct gdix_PointF 
{
	gdix_Real x;
	gdix_Real y;
};

struct gdix_RectF 
{
	gdix_Real x;
	gdix_Real y;
	gdix_Real w;
	gdix_Real h;
};

//////////////////////////////////////////////////////////////////////

class CGdiPlusGraphics
{
public:
	CGdiPlusGraphics(HDC hDC, gdix_SmoothingMode smoothing = gdix_SmoothingModeMedium);
	CGdiPlusGraphics(gdix_Bitmap* bitmap);
	virtual ~CGdiPlusGraphics();

	operator gdix_Graphics*() const { return m_graphics; }
	BOOL IsValid() const { return (m_graphics != NULL); }

	HDC GetHDC();
	void ReleaseDC();

protected:
	gdix_Graphics* m_graphics;
	HDC m_hDC;
};

//////////////////////////////////////////////////////////////////////

class CGdiPlusPen
{
public:
	CGdiPlusPen();
	CGdiPlusPen(COLORREF color, int nWidth = 1, gdix_PenStyle nStyle = gdix_PenStyleSolid);
	virtual ~CGdiPlusPen();

	operator gdix_Pen*() const { return m_pen; }

	BOOL Create(COLORREF color, int nWidth = 1, gdix_PenStyle nStyle = gdix_PenStyleSolid);
	BOOL IsValid() const { return (m_pen != NULL); }
	BOOL SetStyle(gdix_PenStyle nStyle);

protected:
	gdix_Pen* m_pen;
};

//////////////////////////////////////////////////////////////////////

class CGdiPlusBrush
{
public:
	CGdiPlusBrush();
	CGdiPlusBrush(COLORREF color, BYTE alpha = 255);
	virtual ~CGdiPlusBrush();

	operator gdix_Brush*() const { return m_brush; }

	BOOL Create(COLORREF color, BYTE alpha = 255);
	BOOL IsValid() const { return (m_brush != NULL); }

protected:
	gdix_Brush* m_brush;
};

//////////////////////////////////////////////////////////////////////

class CGdiPlusBitmap
{
public:
	CGdiPlusBitmap(IStream* stream);
	CGdiPlusBitmap(const WCHAR* filename);
	CGdiPlusBitmap(HBITMAP hbitmap);
	CGdiPlusBitmap(int width, int height, gdix_PixelFormat format = gdix_PixelFormat32bppRGB);

	virtual ~CGdiPlusBitmap();

	operator gdix_Bitmap*() const { return m_bitmap; }
	gdix_Image* AsImage() const { return (gdix_Image*)m_bitmap; }

	BOOL IsValid() const { return (m_bitmap != NULL); }
	BOOL SaveAsFile(const WCHAR* filename);

protected:
	gdix_Bitmap* m_bitmap;
};

//////////////////////////////////////////////////////////////////////

class CGdiPlusPointF : public gdix_PointF
{
public:
	CGdiPlusPointF(int nX = 0, int nY = 0);
	CGdiPlusPointF(const POINT& pt);
	CGdiPlusPointF(const gdix_PointF& pt);

	operator gdix_PointF*() { return this; }
};

//////////////////////////////////////////////////////////////////////

class CGdiPlusRectF : public gdix_RectF
{
public:
	CGdiPlusRectF(int l = 0, int t = 0, int r = 0, int b = 0);
	CGdiPlusRectF(const RECT& rect);
	CGdiPlusRectF(const POINT& ptTopLeft, const SIZE& size);
	CGdiPlusRectF(const POINT& ptTopLeft, const POINT& ptBottomRight);

	CGdiPlusRectF(const gdix_RectF& rect);
	CGdiPlusRectF(const gdix_PointF& ptTopLeft, const gdix_PointF& ptBottomRight);
	
	operator gdix_RectF*() { return this; }
};

//////////////////////////////////////////////////////////////////////

class CGdiPlus  
{
public:
	static void Free(); // Initialized on demand

	// Native GDI+
	static BOOL CreateBitmapFromStream(IStream* stream, gdix_Bitmap **bitmap);
	static BOOL CreateBitmapFromFile(const WCHAR* filename, gdix_Bitmap **bitmap);
	static BOOL CreateBitmapFromHBITMAP(HBITMAP hbitmap, HPALETTE hPal, gdix_Bitmap **bitmap);
	static BOOL CreateBitmap(int width, int height, gdix_Bitmap **bitmap, gdix_PixelFormat format = gdix_PixelFormat32bppRGB);

	static BOOL CreateHBITMAPFromBitmap(gdix_Bitmap* bitmap, HBITMAP* hbmReturn, gdix_ARGB background);
	static BOOL CreateHICONFromBitmap(gdix_Bitmap* bitmap, HICON* hicoReturn);
	static BOOL DeleteBitmap(gdix_Bitmap* bitmap);
	static BOOL SaveBitmapToFile(gdix_Bitmap* bitmap, const WCHAR* filename);
	static BOOL SaveBitmapToStream(gdix_Bitmap* bitmap, IStream* pStream, const CLSID& clsidEncoder);

	static BOOL CreatePen(gdix_ARGB color, gdix_Real width, gdix_Pen** pen);
	static BOOL DeletePen(gdix_Pen* pen);
	static BOOL SetPenStyle(gdix_Pen* pen, gdix_PenStyle style);
	
	static BOOL CreateBrush(gdix_ARGB color, gdix_Brush** brush);
	static BOOL DeleteBrush(gdix_Brush* brush);

	static BOOL CreateGraphics(HDC hdc, gdix_Graphics**);
	static BOOL CreateGraphics(gdix_Bitmap* bitmap, gdix_Graphics**);
	static BOOL DeleteGraphics(gdix_Graphics* graphics);
	static BOOL GetDC(gdix_Graphics* graphics, HDC* hDC);
	static BOOL ReleaseDC(gdix_Graphics* graphics, HDC hDC);

	static BOOL DrawLine(gdix_Graphics* graphics, gdix_Pen* pen, const gdix_PointF* from, const gdix_PointF* to);
	static BOOL DrawLines(gdix_Graphics* graphics, gdix_Pen* pen, const gdix_PointF* points, int count);
	static BOOL DrawPolygon(gdix_Graphics* graphics, gdix_Pen* pen, const gdix_PointF* points, int count, gdix_Brush* brush = NULL);
	static BOOL DrawEllipse(gdix_Graphics* graphics, gdix_Pen* pen, const gdix_RectF* rect, gdix_Brush* brush = NULL);
	static BOOL DrawRect(gdix_Graphics* graphics, gdix_Pen* pen, const gdix_RectF* rect, gdix_Brush* brush = NULL);
	static BOOL DrawPie(gdix_Graphics* graphics, gdix_Pen* pen, const gdix_RectF* rect, float startDegrees, float sweepDegrees, gdix_Brush* brush = NULL);
	static BOOL DrawArc(gdix_Graphics* graphics, gdix_Pen* pen, const gdix_RectF* rect, float startDegrees, float sweepDegrees);

	static BOOL FillPolygon(gdix_Graphics* graphics, gdix_Brush* brush, const gdix_PointF* points, int count);
	static BOOL FillEllipse(gdix_Graphics* graphics, gdix_Brush* brush, const gdix_RectF* rect);
	static BOOL FillRect(gdix_Graphics* graphics, gdix_Brush* brush, const gdix_RectF* rect);
	static BOOL FillPie(gdix_Graphics* graphics, gdix_Brush* brush, const gdix_RectF* rect, float startDegrees, float sweepDegrees);

	static gdix_ARGB MakeARGB(COLORREF color, BYTE alpha = 255);
	static BOOL SetSmoothingMode(gdix_Graphics* graphics, gdix_SmoothingMode mode);

	// Win32 support
	static BOOL DrawLine(CGdiPlusGraphics& graphics, CGdiPlusPen& pen, const POINT& from, const POINT& to);
	static BOOL DrawLines(CGdiPlusGraphics& graphics, CGdiPlusPen& pen, const POINT points[], int count);
	static BOOL DrawPolygon(CGdiPlusGraphics& graphics, CGdiPlusPen& pen, const POINT points[], int count, gdix_Brush* brush = NULL);
	static BOOL DrawEllipse(gdix_Graphics* graphics, gdix_Pen* pen, const RECT& rect, gdix_Brush* brush = NULL);
	static BOOL DrawRect(gdix_Graphics* graphics, gdix_Pen* pen, const RECT& rect, gdix_Brush* brush = NULL);
	static BOOL DrawPie(gdix_Graphics* graphics, gdix_Pen* pen, const RECT& rect, float startDegrees, float sweepDegrees, gdix_Brush* brush = NULL);
	static BOOL DrawArc(gdix_Graphics* graphics, gdix_Pen* pen, const RECT& rect, float startDegrees, float sweepDegrees);
	
	static BOOL FillPolygon(gdix_Graphics* graphics, gdix_Brush* brush, const POINT points[], int count);
	static BOOL FillEllipse(gdix_Graphics* graphics, gdix_Brush* brush, const RECT& rect);
	static BOOL FillRect(gdix_Graphics* graphics, gdix_Brush* brush, const RECT& rect);
	static BOOL FillPie(gdix_Graphics* graphics, gdix_Brush* brush, const RECT& rect, float startDegrees, float sweepDegrees);

	static void GetPointFs(const POINT points[], int count, CArray<gdix_PointF, gdix_PointF&>& pointFs);

protected:
	static HMODULE s_hGdiPlus;
	static ULONG_PTR s_GdiToken;

protected:
	static BOOL Initialize(); // initialize on demand

	static BOOL GetImageMimeType(const WCHAR* filename, CString& sMimeType);
	static BOOL GetImageEncoders(UINT numEncoders, UINT sizeEncoderArray, IMAGECODECINFO* pEncoders);
	static BOOL GetImageEncodersSize(UINT* numEncoders, UINT* sizeEncoderArray);
	static BOOL GetEncoderClsid(const WCHAR* szFormat, CLSID* pClsid);

};

#endif // !defined(AFX_GDIPLUS_H__41B5F6AC_E7D4_49CA_9E2F_96FDA94EBB28__INCLUDED_)
