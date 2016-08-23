// GdiPlus.h: interface for the CGdiPlus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GDIPLUS_H__41B5F6AC_E7D4_49CA_9E2F_96FDA94EBB28__INCLUDED_)
#define AFX_GDIPLUS_H__41B5F6AC_E7D4_49CA_9E2F_96FDA94EBB28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//////////////////////////////////////////////////////////////////////

/*
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

typedef float gdix_Real;
typedef DWORD gdix_ARGB;

//////////////////////////////////////////////////////////////////////

enum gdix_Status 
{
	gdix_Ok = 0
};

enum gdix_Unit 
{
	gdix_UnitWorld = 0
};

enum gdix_SmoothingMode 
{
	gdix_SmoothingModeHighQuality = 2
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

//////////////////////////////////////////////////////////////////////

#define gdix_StringFormatFlagsNoWrap   0x00001000
#define gdix_StringFormatFlagsNoClip   0x00004000

//////////////////////////////////////////////////////////////////////

#if _MSC_VER < 1400
#	ifndef ULONG_PTR
		typedef unsigned __int64    ULONG_PTR;
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

class CGdiPlus  
{
public:
	static BOOL CreateBitmapFromStream(IStream* stream, gdix_Bitmap **bitmap);
	static BOOL CreateBitmapFromFile(const WCHAR* filename, gdix_Bitmap **bitmap);
	static BOOL CreateHBITMAPFromBitmap(gdix_Bitmap* bitmap, HBITMAP* hbmReturn, gdix_ARGB background);
	static BOOL DeleteBitmap(gdix_Bitmap* bitmap);
// 	static BOOL CreateBitmapFromFile(const WCHAR* filename, GpBitmap **bitmap);
// 	static BOOL CreateHBITMAPFromBitmap(GpBitmap* bitmap, HBITMAP* hbmReturn, gdix_ARGB background);

protected:
	static HMODULE s_hGdiPlus;
	static ULONG_PTR s_GdiToken;

protected:
	static BOOL Initialize(); // initialize of demand
	static void Free();
};

#endif // !defined(AFX_GDIPLUS_H__41B5F6AC_E7D4_49CA_9E2F_96FDA94EBB28__INCLUDED_)
