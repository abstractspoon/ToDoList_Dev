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

#include "StdAfx.h"

#if (!defined __EXT_CMD_ICON_H)
	#include <ExtCmdIcon.h>
#endif

#if (!defined __EXT_PAINT_MANAGER_H)
	#include <ExtPaintManager.h>
#endif

#if (!defined __EXT_MEMORY_DC_H)
	#include <../Src/ExtMemoryDC.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtCmdIcon::BitmapCache

CExtCmdIcon::BitmapCache::BitmapCache(
	HBITMAP hBitmap, //  = NULL
	COLORREF clrTransparent, // = RGB( 0, 0, 0 )
	LPCRECT pRectBitmapSrc // = NULL
	)
	: m_hBitmap( NULL )
	, m_clrTransparent( RGB( 0, 0, 0 ) )
	, m_sizeBitmap( 0, 0 )
{
	AssignIndirect(
		hBitmap,
		clrTransparent,
		pRectBitmapSrc
		);
}

CExtCmdIcon::BitmapCache::BitmapCache(
	const BitmapCache & other
	)
	: m_hBitmap( NULL )
	, m_clrTransparent( RGB( 0, 0, 0 ) )
	, m_sizeBitmap( 0, 0 )
{
	AssignFromOther( other );
}

CExtCmdIcon::BitmapCache::~BitmapCache()
{
	Empty();
}

void CExtCmdIcon::BitmapCache::Empty()
{
	if( m_hBitmap == NULL )
	{
		ASSERT( m_sizeBitmap.cx == 0 && m_sizeBitmap.cy == 0 );
		return;
	}
	ASSERT( m_sizeBitmap.cx > 0 && m_sizeBitmap.cy > 0 );
	m_sizeBitmap.cx = m_sizeBitmap.cy = 0;
	::DeleteObject( m_hBitmap );
	m_hBitmap = NULL;
	m_clrTransparent = RGB( 0, 0, 0 );
}

bool CExtCmdIcon::BitmapCache::IsEmpty() const
{
	return ( m_hBitmap == NULL ) ? true : false;
}

void CExtCmdIcon::BitmapCache::AssignIndirect(
	HBITMAP hBitmap,
	COLORREF clrTransparent,
	LPCRECT pRectBitmapSrc // = NULL
	)
{
	if( hBitmap == NULL )
	{
		Empty();
		ASSERT( m_sizeBitmap.cx == 0 && m_sizeBitmap.cy == 0 );
		return;
	}
	if(		m_hBitmap == hBitmap
		&&	pRectBitmapSrc == NULL
		)
	{
		ASSERT( m_sizeBitmap.cx > 0 && m_sizeBitmap.cy > 0 );
		m_clrTransparent = clrTransparent;
		return;
	}
HBITMAP hBitmapNew =
		CExtPaintManager::stat_CloneBitmap(
			hBitmap,
			pRectBitmapSrc
			);
	if( hBitmapNew == NULL )
	{
		ASSERT( FALSE );
		return;
	}
	Empty();
	m_hBitmap = hBitmapNew;
	m_clrTransparent = clrTransparent;
BITMAP bmpSrcDescription;
	::memset( &bmpSrcDescription, 0, sizeof(BITMAP) );
	::GetObject( m_hBitmap, sizeof(BITMAP), &bmpSrcDescription );
	ASSERT( bmpSrcDescription.bmWidth > 0 && bmpSrcDescription.bmHeight > 0 );
	m_sizeBitmap.cx = bmpSrcDescription.bmWidth;
	m_sizeBitmap.cy = bmpSrcDescription.bmHeight;
}

void CExtCmdIcon::BitmapCache::Serialize( CArchive & ar )
{
	if( ar.IsStoring() )
	{
		ASSERT( !IsEmpty() );
		VERIFY(
			CExtPaintManager::stat_SerializeBitmap(
				ar,
				&m_hBitmap
				)
			);
		ar << m_clrTransparent;
	} // if( ar.IsStoring() )
	else
	{
		Empty();
		ASSERT( m_hBitmap == NULL );
		VERIFY(
			CExtPaintManager::stat_SerializeBitmap(
				ar,
				&m_hBitmap
				)
			);
		ASSERT( m_hBitmap != NULL );
		ar >> m_clrTransparent;
		BITMAP bmpSrcDescription;
		::memset( &bmpSrcDescription, 0, sizeof(BITMAP) );
		::GetObject( m_hBitmap, sizeof(BITMAP), &bmpSrcDescription );
		ASSERT( bmpSrcDescription.bmWidth > 0 && bmpSrcDescription.bmHeight > 0 );
		m_sizeBitmap.cx = bmpSrcDescription.bmWidth;
		m_sizeBitmap.cy = bmpSrcDescription.bmHeight;
		ASSERT( !IsEmpty() );
	} // else from if( ar.IsStoring() )
}

void CExtCmdIcon::BitmapCache::AssignFromOther(
	const CExtCmdIcon::BitmapCache & other
	)
{
	AssignIndirect(
		other.m_hBitmap,
		other.m_clrTransparent
		);
}

CExtCmdIcon::BitmapCache &
	CExtCmdIcon::BitmapCache::operator=(
		const CExtCmdIcon::BitmapCache & other
		)
{
	AssignFromOther( other );
	return (*this);
}

HICON CExtCmdIcon::BitmapCache::CreateHICON(
	bool bReplaceSysColors // = true
	) const
{
	if( IsEmpty() )
	{
		ASSERT( FALSE );
		return NULL;
	}
CRect rcClone( CPoint(0,0), m_sizeBitmap );
HICON hIcon =
		CExtPaintManager::stat_HBITMAP2HICON(
			m_hBitmap,
			m_clrTransparent,
			&rcClone,
			bReplaceSysColors
			);
	ASSERT( hIcon != NULL );
	return hIcon;
}

CSize CExtCmdIcon::BitmapCache::GetSize() const
{
	ASSERT( m_sizeBitmap.cx > 0 && m_sizeBitmap.cy > 0 );
	return m_sizeBitmap;
}

COLORREF CExtCmdIcon::BitmapCache::GetTransparentColor() const
{
	return m_clrTransparent;
}

HBITMAP CExtCmdIcon::BitmapCache::GetBitmap() const
{
	ASSERT( m_hBitmap != NULL );
	return (HBITMAP)m_hBitmap;
}

INT CExtCmdIcon::BitmapCache::GetColorMap(
	CMap < COLORREF, COLORREF, DWORD, DWORD > & _map
	) const
{
	ASSERT( !IsEmpty() );
	return
		CExtPaintManager::stat_GetBitmapColorMap(
			(HBITMAP)m_hBitmap,
			_map
			);
}

INT CExtCmdIcon::BitmapCache::GetBitsPerPixel() const
{
	ASSERT( !IsEmpty() );
BITMAP bi;
	::memset( &bi, 0, sizeof(BITMAP) );
	::GetObject( m_hBitmap, sizeof(BITMAP), &bi );
	ASSERT(
			bi.bmWidth == m_sizeBitmap.cx
		&&	bi.bmHeight == m_sizeBitmap.cy
		&&	bi.bmBitsPixel > 0
		);
	return (INT)bi.bmBitsPixel;
}

/////////////////////////////////////////////////////////////////////////////
// CExtCmdIcon::IconCache
CExtCmdIcon::IconCache::IconCache()
	: m_hIcon( NULL )
	, m_sizeIcon( 0, 0 )
{
}

CExtCmdIcon::IconCache::IconCache(
	const CExtCmdIcon::IconCache & other
	)
	: m_hIcon( NULL )
	, m_sizeIcon( 0, 0 )
{
	AssignFromOther( other );
}

CExtCmdIcon::IconCache::~IconCache()
{
	Empty();
}

void CExtCmdIcon::IconCache::Empty()
{
	if( m_hIcon == NULL )
	{
		ASSERT( m_sizeIcon.cx == 0 && m_sizeIcon.cy == 0 );
		return;
	}
	ASSERT( m_sizeIcon.cx > 0 && m_sizeIcon.cy > 0 );
	::DestroyIcon( m_hIcon );
	m_hIcon = NULL;
	m_sizeIcon.cx = m_sizeIcon.cy = 0;
}

bool CExtCmdIcon::IconCache::IsEmpty() const
{
	return (m_hIcon == NULL) ? true : false;
}

void CExtCmdIcon::IconCache::Serialize( CArchive & ar )
{
	if( ar.IsStoring() )
	{
		ASSERT( !IsEmpty() );
		ASSERT( m_hIcon != NULL );
		
		ICONINFO ii;
		::memset( &ii, 0, sizeof(ICONINFO) );
		VERIFY( ::GetIconInfo( m_hIcon, &ii ) );
		ASSERT( ii.hbmColor != NULL || ii.hbmMask != NULL );

		BYTE nFlags = 0;
		if( ii.fIcon )
			nFlags |= 0x01;
		if( ii.hbmColor != NULL )
			nFlags |= 0x02;
		if( ii.hbmMask != NULL )
			nFlags |= 0x04;
		ar << nFlags;
		ar << ii.xHotspot;
		ar << ii.yHotspot;
		if( ii.hbmColor != NULL )
		{
			VERIFY(
				CExtPaintManager::stat_SerializeBitmap(
					ar,
					&ii.hbmColor
					)
				);
		}
		if( ii.hbmMask != NULL )
		{
			VERIFY(
				CExtPaintManager::stat_SerializeBitmap(
					ar,
					&ii.hbmMask
					)
				);
		}
		
		::DeleteObject( ii.hbmColor );
		::DeleteObject( ii.hbmMask );
	} // if( ar.IsStoring() )
	else
	{
		Empty();
		ASSERT( m_hIcon == NULL );
		
		ICONINFO ii;
		::memset( &ii, 0, sizeof(ICONINFO) );
		BYTE nFlags = 0;
		ar >> nFlags;
		ar >> ii.xHotspot;
		ar >> ii.yHotspot;
		if( nFlags & 0x01 )
			ii.fIcon = TRUE;
		else
			ii.fIcon = TRUE;
		if( nFlags & 0x02 )
		{
			VERIFY(
				CExtPaintManager::stat_SerializeBitmap(
					ar,
					&ii.hbmColor
					)
				);
			ASSERT( ii.hbmColor != NULL );
		}
		if( nFlags & 0x04 )
		{
			VERIFY(
				CExtPaintManager::stat_SerializeBitmap(
					ar,
					&ii.hbmMask
					)
				);
			ASSERT( ii.hbmMask != NULL );
		}
		ASSERT( ii.hbmColor != NULL || ii.hbmMask != NULL );

		m_hIcon = ::CreateIconIndirect( &ii );
		ASSERT( m_hIcon != NULL );

		m_sizeIcon.cx = (INT)( (BYTE)(ii.xHotspot * 2) );
		m_sizeIcon.cy = (INT)( (BYTE)(ii.yHotspot * 2) );
		if( m_sizeIcon.cx == 0 || m_sizeIcon.cy == 0 )
		{
			BITMAP bi;
			::memset( &bi, 0, sizeof(BITMAP) );
			::GetObject(
				(ii.hbmColor != NULL) ? ii.hbmColor : ii.hbmMask,
				sizeof(BITMAP),
				&bi
				);
			ASSERT( bi.bmWidth > 0 && bi.bmHeight > 0 );
			m_sizeIcon.cx = (INT)bi.bmWidth;
			m_sizeIcon.cy = (INT)bi.bmHeight;
		} // if( m_sizeIcon.cx == 0 || m_sizeIcon.cy == 0 )
		ASSERT( m_sizeIcon.cx > 0 );
		ASSERT( m_sizeIcon.cy > 0 );

		::DeleteObject( ii.hbmColor );
		::DeleteObject( ii.hbmMask );
		if( m_hIcon == NULL )
			m_sizeIcon.cx = m_sizeIcon.cy = 0;

	} // else from if( ar.IsStoring() )
}

void CExtCmdIcon::IconCache::AssignFromOther(
	const CExtCmdIcon::IconCache & other
	)
{
	Empty();
	if( other.IsEmpty() )
		return;
	AssignFromHICON( (HICON)other.m_hIcon, true );
}

CExtCmdIcon::IconCache &
	CExtCmdIcon::IconCache::operator=(
		const CExtCmdIcon::IconCache & other
	)
{
	AssignFromOther( other );
	return (*this);
}

void CExtCmdIcon::IconCache::AssignFromHICON(
	HICON & hIcon,
	bool bCreateCopy
	)
{
	if(		(!bCreateCopy)
		&&	m_hIcon != NULL
		&&	m_hIcon == hIcon
		)
	{
		ASSERT( m_sizeIcon.cx > 0 && m_sizeIcon.cy > 0 );
		return;
	}
	Empty();
	ASSERT( IsEmpty() );
	if( hIcon == NULL )
		return;
	if( bCreateCopy )
	{
		ICONINFO ii;
		::memset( &ii, 0, sizeof(ICONINFO) );
		VERIFY( ::GetIconInfo( hIcon, &ii ) );
		ASSERT( ii.hbmColor != NULL || ii.hbmMask != NULL );
		m_sizeIcon.cx = (INT)( (BYTE)(ii.xHotspot * 2) );
		m_sizeIcon.cy = (INT)( (BYTE)(ii.yHotspot * 2) );
		if( m_sizeIcon.cx == 0 || m_sizeIcon.cy == 0 )
		{
			BITMAP bi;
			::memset( &bi, 0, sizeof(BITMAP) );
			::GetObject(
				(ii.hbmColor != NULL) ? ii.hbmColor : ii.hbmMask,
				sizeof(BITMAP),
				&bi
				);
			ASSERT( bi.bmWidth > 0 && bi.bmHeight > 0 );
			m_sizeIcon.cx = (INT)bi.bmWidth;
			m_sizeIcon.cy = (INT)bi.bmHeight;
		} // if( m_sizeIcon.cx == 0 || m_sizeIcon.cy == 0 )
		ASSERT( m_sizeIcon.cx > 0 );
		ASSERT( m_sizeIcon.cy > 0 );
		m_hIcon = ::CreateIconIndirect( &ii );
		ASSERT( m_hIcon != NULL );
		::DeleteObject( ii.hbmColor );
		::DeleteObject( ii.hbmMask );
		if( m_hIcon == NULL )
			m_sizeIcon.cx = m_sizeIcon.cy = 0;
	} // if( bCreateCopy )
	else
	{
		m_hIcon = hIcon;
		RecalcIconOriginalSize();
		ASSERT( m_sizeIcon.cx > 0 && m_sizeIcon.cy > 0 );
	} // else from if( bCreateCopy )
}

void CExtCmdIcon::IconCache::RecalcIconOriginalSize()
{
	if( m_hIcon == NULL )
		return;
	m_sizeIcon.cx = m_sizeIcon.cy = 0;
ICONINFO ii;
	::memset( &ii, 0, sizeof(ICONINFO) );
	VERIFY( ::GetIconInfo( m_hIcon, &ii ) );
	ASSERT( ii.hbmColor != NULL || ii.hbmMask != NULL );
	m_sizeIcon.cx = (BYTE)(ii.xHotspot * 2);
	m_sizeIcon.cy = (BYTE)(ii.yHotspot * 2);
	if( m_sizeIcon.cx == 0 || m_sizeIcon.cy == 0 )
	{
		BITMAP bi;
		::memset( &bi, 0, sizeof(BITMAP) );
		::GetObject(
			(ii.hbmColor != NULL) ? ii.hbmColor : ii.hbmMask,
			sizeof(BITMAP),
			&bi
			);
		ASSERT( bi.bmWidth > 0 && bi.bmHeight > 0 );
		m_sizeIcon.cx = bi.bmWidth;
		m_sizeIcon.cy = bi.bmHeight;
	} // if( m_sizeIcon.cx == 0 || m_sizeIcon.cy == 0 )
	ASSERT( m_sizeIcon.cx > 0 && m_sizeIcon.cy > 0 );
	::DeleteObject(ii.hbmMask);
	::DeleteObject(ii.hbmColor);
}

HICON CExtCmdIcon::IconCache::GetIcon() const
{
	ASSERT( m_hIcon != NULL );
	return (HICON)m_hIcon;
}

CSize CExtCmdIcon::IconCache::GetSize() const
{
	ASSERT( m_sizeIcon.cx > 0 && m_sizeIcon.cy > 0 );
	return m_sizeIcon;
}

HICON CExtCmdIcon::IconCache::Detach()
{
	ASSERT( m_hIcon != NULL );
	ASSERT( m_sizeIcon.cx > 0 && m_sizeIcon.cy > 0 );
HICON hIcon = m_hIcon;
	m_hIcon = NULL;
	m_sizeIcon.cx = m_sizeIcon.cy = 0;
	return hIcon;
}

void CExtCmdIcon::IconCache::Paint(
	HDC hDC,
	const POINT & pt,
	CExtCmdIcon::e_draw_type_t eDT,
	HBRUSH hBrush
	) const
{
	ASSERT( !IsEmpty() );
	ASSERT( hDC != NULL );
	ASSERT(
			CExtCmdIcon::e_draw_type_t::__DRAW_VAL_MIN <= eDT
		&&	eDT <= CExtCmdIcon::e_draw_type_t::__DRAW_VAL_MAX
		&&	eDT != CExtCmdIcon::e_draw_type_t::__DRAW_ALPHA
		);
UINT nFlags = DST_ICON|DSS_NORMAL;
	if( eDT == CExtCmdIcon::e_draw_type_t::__DRAW_DISABLED )
		nFlags |= DSS_DISABLED;
	else
		if( eDT == CExtCmdIcon::e_draw_type_t::__DRAW_MONO )
			nFlags |= DSS_MONO;
	::DrawState(
		hDC,
		hBrush,
		NULL,
		(LPARAM)m_hIcon,
		0,
		pt.x,
		pt.y,
		m_sizeIcon.cx,
		m_sizeIcon.cy,
		nFlags
		);
}

INT CExtCmdIcon::IconCache::GetColorMap(
	CMap < COLORREF, COLORREF, DWORD, DWORD > & _map
	) const
{
	ASSERT( !IsEmpty() );
ICONINFO ii;
	::memset( &ii, 0, sizeof(ICONINFO) );
	VERIFY( ::GetIconInfo( m_hIcon, &ii ) );
	ASSERT( ii.hbmColor != NULL || ii.hbmMask != NULL );
INT nRetVal = 0;
	if( ii.hbmColor == NULL )
	{
		ASSERT( ii.hbmMask != NULL );
		nRetVal =
			CExtPaintManager::stat_GetBitmapColorMap(
				ii.hbmMask,
				_map
				);
	} // if( ii.hbmColor == NULL )
	else
	{
		nRetVal =
			CExtPaintManager::stat_GetBitmapColorMap(
				ii.hbmColor,
				_map
				);
	} // else from if( ii.hbmColor == NULL )
	::DeleteObject(ii.hbmMask);
	::DeleteObject(ii.hbmColor);
	return nRetVal;
}

INT CExtCmdIcon::IconCache::GetBitsPerPixel() const
{
	ASSERT( !IsEmpty() );
ICONINFO ii;
	::memset( &ii, 0, sizeof(ICONINFO) );
	VERIFY( ::GetIconInfo( m_hIcon, &ii ) );
	ASSERT( ii.hbmColor != NULL || ii.hbmMask != NULL );
	if( ii.hbmColor == NULL )
	{	
		ASSERT( ii.hbmMask != NULL );
		::DeleteObject(ii.hbmMask);
		return 1; // monochrome
	}
BITMAP bi;
	::memset( &bi, 0, sizeof(BITMAP) );
	::GetObject( ii.hbmColor, sizeof(BITMAP), &bi );
	ASSERT( bi.bmBitsPixel > 0 );
	::DeleteObject(ii.hbmMask);
	::DeleteObject(ii.hbmColor);
	return (INT)bi.bmBitsPixel;
}

/////////////////////////////////////////////////////////////////////////////
// CExtCmdIcon
// command icon class

#define __DEFAULT_ICON_ALPHA_ADJUST_PERCENT 1.4
#define __DEFAULT_ICON_CLR_ALPHA COLORREF(-1L)

CExtCmdIcon::CExtCmdIcon(
	HBITMAP hBitmap, // = NULL
	COLORREF clrTransparent, // = RGB( 0, 0, 0 )
	LPCRECT pRectBitmapSrc // = NULL
	)
	: m_BitmapCache( hBitmap, clrTransparent, pRectBitmapSrc )
	, m_clrAlpha( __DEFAULT_ICON_CLR_ALPHA )
	, m_lfAlphaAdjustPercent( __DEFAULT_ICON_ALPHA_ADJUST_PERCENT )
{
}

CExtCmdIcon::CExtCmdIcon(
	HICON & hIcon,
	bool bCreateCopy
	)
	: m_clrAlpha( __DEFAULT_ICON_CLR_ALPHA )
	, m_lfAlphaAdjustPercent( __DEFAULT_ICON_ALPHA_ADJUST_PERCENT )
{
	AssignFromHICON( hIcon, bCreateCopy );
}

CExtCmdIcon::CExtCmdIcon( const CExtCmdIcon & other )
	: m_clrAlpha( __DEFAULT_ICON_CLR_ALPHA )
	, m_lfAlphaAdjustPercent( __DEFAULT_ICON_ALPHA_ADJUST_PERCENT )
{
	AssignFromOther( other );
}

CExtCmdIcon::~CExtCmdIcon()
{
	Empty();
}

bool CExtCmdIcon::IsEmpty() const
{
	if( m_BitmapCache.IsEmpty() )
		return m_IconCache.IsEmpty();
	return false;
}

void CExtCmdIcon::Empty()
{
	m_BitmapCache.Empty();
	m_IconCache.Empty();
	m_IconAlpha.Empty();
	m_clrAlpha = __DEFAULT_ICON_CLR_ALPHA;
	m_lfAlphaAdjustPercent = __DEFAULT_ICON_ALPHA_ADJUST_PERCENT;
}

void CExtCmdIcon::Serialize( CArchive & ar )
{
	if( ar.IsStoring() )
	{
		if( IsEmpty() )
		{
			BYTE nFlags = 0;
			ar << nFlags;
			return;
		}
		if( IsBitmapBased() )
		{
			BYTE nFlags = 0x01;
			ar << nFlags;
			m_BitmapCache.Serialize( ar );
			return;
		} // if( ar.IsStoring() )
		BYTE nFlags = 0x02;
		ar << nFlags;
		m_IconCache.Serialize( ar );
	} // if( ar.IsStoring() )
	else
	{
		Empty();
		BYTE nFlags = 0;
		ar >> nFlags;
		if( nFlags == 0 )
			return;
		if( nFlags & 0x01 )
			m_BitmapCache.Serialize( ar );
		else if( nFlags & 0x02 )
			m_IconCache.Serialize( ar );
	} // else from if( ar.IsStoring() )
}

void CExtCmdIcon::AssignFromOther( const CExtCmdIcon & other )
{
LPVOID lpvThis = (LPVOID)this;
LPVOID lpvOther = (LPVOID)(&other);
	if( lpvThis == lpvOther )
		return;
	if( !m_IconAlpha.IsEmpty() )
		m_IconAlpha.Empty();
	if( other.m_BitmapCache.IsEmpty() )
	{
		m_BitmapCache.Empty();
		m_IconCache.AssignFromOther( other.m_IconCache );
	} // if( other.m_BitmapCache.IsEmpty() )
	else
	{
		m_IconCache.Empty();
		m_BitmapCache.AssignFromOther( other.m_BitmapCache );
	} // else from if( other.m_BitmapCache.IsEmpty() )
	m_clrAlpha = other.m_clrAlpha;
	m_lfAlphaAdjustPercent = other.m_lfAlphaAdjustPercent;
}

void CExtCmdIcon::AssignFromHICON(
	HICON hIcon,
	bool bCreateCopy
	)
{
	if( !m_IconAlpha.IsEmpty() )
		m_IconAlpha.Empty();
	if( !m_BitmapCache.IsEmpty() )
		m_BitmapCache.Empty();
	m_IconCache.AssignFromHICON( hIcon, bCreateCopy );
}

void CExtCmdIcon::AssignFromHBITMAP(
	HBITMAP hBitmap, // = NULL
	COLORREF clrTransparent, // = RGB( 0, 0, 0 )
	LPCRECT pRectBitmapSrc // = NULL
	)
{
	if( !m_IconCache.IsEmpty() )
		m_IconCache.Empty();
	if( !m_IconAlpha.IsEmpty() )
		m_IconAlpha.Empty();
	m_BitmapCache.AssignIndirect(
		hBitmap,
		clrTransparent,
		pRectBitmapSrc
		);
}


CExtCmdIcon & CExtCmdIcon::operator=( const CExtCmdIcon & other )
{
	AssignFromOther( other );
	return (*this);
}

CSize CExtCmdIcon::GetSize() const
{
	if( !m_BitmapCache.IsEmpty() )
		return m_BitmapCache.GetSize();
	if( !m_IconCache.IsEmpty() )
		return m_IconCache.GetSize();
	return CSize( 0, 0 );
}

bool CExtCmdIcon::IsBitmapBased() const
{
	ASSERT( !IsEmpty() );
	if( m_BitmapCache.IsEmpty() )
		return false;
	return true;
}

HBITMAP CExtCmdIcon::GetBitmap() const
{
	ASSERT( IsBitmapBased() );
	return m_BitmapCache.GetBitmap();
}

COLORREF CExtCmdIcon::GetBitmapTransparentColor() const
{
	ASSERT( IsBitmapBased() );
	return m_BitmapCache.GetTransparentColor();
}

bool CExtCmdIcon::ExtractEditableBitmap(
	HBITMAP & hBitmap,
	COLORREF & clrTransparent,
	CArray <COLORREF,COLORREF> * pArr, // = NULL // recommended palette, item 0 is clrTransparent
	COLORREF ** ppClrSurface // = NULL // if not NULL - force 32 bit
	) const
{
	if( IsEmpty() )
		return false;

static COLORREF arrTransparentColors[] =
{ RGB(255,0,255), RGB(0,255,0), RGB(192,192,192), };

	if( IsBitmapBased() )
	{
		CArray <COLORREF,COLORREF> _arr;
		CExtPaintManager::stat_GetBitmapColorArray(
			m_BitmapCache.GetBitmap(),
			_arr,
			true
			);
		ASSERT( _arr.GetSize() >= 1 );
		COLORREF clrFoundTransparent = ((COLORREF)(-1L));
		if( ppClrSurface != NULL )
		{
			for( int i = 0; i < sizeof(arrTransparentColors)/(sizeof(arrTransparentColors[0])); i++ )
			{
				COLORREF clrAnalyze = arrTransparentColors[i];
				bool bAbandone = false;
				for( int j = 0; j < _arr.GetSize(); j++ )
				{
					COLORREF clrPal = _arr[j];
					if( clrAnalyze == clrPal )
					{
						bAbandone = true;
						break;
					} // if( clrAnalyze == clrPal )
				} // for( int j = 0; j < _arr.GetSize(); j++ )
				if( bAbandone )
					continue;
				clrFoundTransparent = clrAnalyze;
				break;
			} // for( int i = 0; i < sizeof(arrTransparentColors)/(sizeof(arrTransparentColors[0])); i++ )
			if( clrFoundTransparent == ((COLORREF)(-1L)) )
			{
				// do not use black/white colors as transparent
				for( DWORD k = 0x00000001; k < 0x00FFFFFE; k++ )
				{
					COLORREF clrAnalyze = ((COLORREF)(k));
					bool bAbandone = false;
					for( int j = 0; j < _arr.GetSize(); j++ )
					{
						COLORREF clrPal = _arr[j];
						if( clrAnalyze == clrPal )
						{
							bAbandone = true;
							break;
						} // if( clrAnalyze == clrPal )
					} // for( int j = 0; j < _arr.GetSize(); j++ )
					if( bAbandone )
						continue;
					clrFoundTransparent = clrAnalyze;
					break;
				} // for( DWORD k = 0x00000001; k < 0x00FFFFFE; k++ )
			} // if( clrFoundTransparent == ((COLORREF)(-1L)) )
		} // if( ppClrSurface != NULL )
		else
		{
			clrFoundTransparent = m_BitmapCache.GetTransparentColor();
		} // else from if( ppClrSurface != NULL )
		if( clrFoundTransparent == ((COLORREF)(-1L)) )
		{
			ASSERT( false );
			return false;
		}

		hBitmap =
			CExtPaintManager::stat_CloneBitmap(
				m_BitmapCache.GetBitmap(),
				NULL,
				ppClrSurface
				);
		if( hBitmap == NULL )
		{
			ASSERT( FALSE );
			return false;
		}
	
		if( ppClrSurface != NULL )
		{
			COLORREF * pSurf = (*ppClrSurface);
			ASSERT( pSurf != NULL );
			COLORREF clrActualTransparent = m_BitmapCache.GetTransparentColor();
			if( clrActualTransparent != clrFoundTransparent )
			{
				CSize _sizeIcon = GetSize();
				ASSERT( _sizeIcon.cx > 0 && _sizeIcon.cy > 0 );
				LONG nIconPixelCount = _sizeIcon.cx * _sizeIcon.cy;
				COLORREF clrQuadSearch =
					RGB(
						GetBValue( clrActualTransparent ),
						GetGValue( clrActualTransparent ),
						GetRValue( clrActualTransparent )
						);
				COLORREF clrQuadReplace =
					RGB(
						GetBValue( clrFoundTransparent ),
						GetGValue( clrFoundTransparent ),
						GetRValue( clrFoundTransparent )
						);
				for( LONG nPixel = 0; nPixel < nIconPixelCount ; nPixel++, pSurf++ )
				{
					if( (*pSurf) == clrQuadSearch )
						(*pSurf) = clrQuadReplace;
				} // for( LONG nPixel = 0; nPixel < nIconPixelCount ; nPixel++, pSurf++ )
			} // if( clrActualTransparent != clrFoundTransparent )
		} // if( ppClrSurface != NULL )

		clrTransparent = clrFoundTransparent;

		if( pArr != NULL )
		{
			pArr->RemoveAll();
			pArr->Append( _arr );
			if( pArr->GetAt(0) != clrTransparent )
			{
				pArr->InsertAt( 0, clrTransparent, 1 );
				for( int i = 1; i < pArr->GetSize(); i++ )
				{
					if( pArr->GetAt(i) == clrTransparent )
					{
						pArr->RemoveAt( i, 1 );
						break;
					} // if( pArr->GetAt(i) == clrTransparent )
				} // for( int i = 1; i < pArr->GetSize(); i++ )
			} // if( pArr->GetAt(0) != clrTransparent )
		} // if( pArr != NULL )
		return true;
	} // if( IsBitmapBased() )
	else
	{
		HICON hIcon = m_IconCache.GetIcon();
		ASSERT( hIcon != NULL );
		CSize _sizeIcon = GetSize();
		ASSERT( _sizeIcon.cx > 0 && _sizeIcon.cy > 0 );
		
		ICONINFO ii;
		::memset( &ii, 0, sizeof(ICONINFO) );
		VERIFY( ::GetIconInfo( hIcon, &ii ) );
		ASSERT( ii.hbmColor != NULL || ii.hbmMask != NULL );
		CBitmap bmpDestroy1, bmpDestroy2;
		if( ii.hbmColor != NULL )
			bmpDestroy1.Attach( ii.hbmColor );
		if( ii.hbmMask != NULL )
			bmpDestroy2.Attach( ii.hbmMask );

		CMap < COLORREF, COLORREF, DWORD, DWORD > _map;
		if( ii.hbmColor == NULL )
		{
			ASSERT( ii.hbmMask != NULL );
			CArray <COLORREF,COLORREF> _arr;
			CExtPaintManager::stat_GetBitmapColorArray(
				ii.hbmMask,
				_arr,
				true
				);
			ASSERT( _arr.GetSize() >= 2 );
			for( int i=0; i < _arr.GetSize(); i++ )
				_map.SetAt( _arr[i], 1 );
		} // if( ii.hbmColor == NULL )
		else
		{
			ASSERT( ii.hbmColor != NULL );
			CArray <COLORREF,COLORREF> _arr;
			CExtPaintManager::stat_GetBitmapColorArray(
				ii.hbmColor,
				_arr,
				true
				);
			ASSERT( _arr.GetSize() >= 1 );
			for( int i=0; i < _arr.GetSize(); i++ )
				_map.SetAt( _arr[i], 1 );
		} // else

		COLORREF clrFoundTransparent = ((COLORREF)(-1L));
		for( int i = 0; i < sizeof(arrTransparentColors)/(sizeof(arrTransparentColors[0])); i++ )
		{
			DWORD dwEntries = 0;
			if( !_map.Lookup( arrTransparentColors[i], dwEntries ) )
			{
				clrFoundTransparent = arrTransparentColors[i];
				break;
			} // if( !_map.Lookup( arrTransparentColors[i], dwEntries ) )
			ASSERT( dwEntries >= 1 );
		} // for( int i = 0; i < sizeof(arrTransparentColors)/(sizeof(arrTransparentColors[0])); i++ )
		if( clrFoundTransparent == ((COLORREF)(-1L)) )
		{
			// do not use black/white colors as transparent
			for( DWORD j = 0x00000001; j < 0x00FFFFFE; j++ )
			{
				DWORD dwEntries = 0;
				if( !_map.Lookup( ((COLORREF)(j)), dwEntries ) )
				{
					clrFoundTransparent = ((COLORREF)(j));
					break;
				} // if( !_map.Lookup( ((COLORREF)(j)), dwEntries ) )
				ASSERT( dwEntries >= 1 );
			} // for( DWORD j = 0x00000001; j < 0x00FFFFFE; j++ )
		} // if( clrFoundTransparent == ((COLORREF)(-1L)) )
		if( clrFoundTransparent == ((COLORREF)(-1L)) )
		{
			ASSERT( false );
			return false;
		}

		HDC hDC = ::CreateCompatibleDC( NULL );
		if( hDC == NULL )
		{
			ASSERT( FALSE );
			return 0;
		}
		
		LONG nIconPixelCount = _sizeIcon.cx * _sizeIcon.cy;
		BITMAPINFOHEADER bih;
		::memset( &bih, 0, sizeof(BITMAPINFOHEADER) );
		bih.biSize = sizeof(BITMAPINFOHEADER);
		bih.biWidth = _sizeIcon.cx;
		bih.biHeight = _sizeIcon.cy;
		bih.biPlanes = 1;
		bih.biBitCount = 32;
		bih.biCompression = BI_RGB;
		bih.biSizeImage = nIconPixelCount;
		COLORREF * pOutSurface = NULL;
		HBITMAP hDIB =
			::CreateDIBSection(
				hDC,
				(LPBITMAPINFO)&bih,
				DIB_RGB_COLORS,
				(void**)&pOutSurface,
				NULL,
				NULL
				);
		if( hDIB == NULL )
		{
			ASSERT( FALSE );
			::DeleteDC( hDC );
			return false;
		}
		ASSERT( pOutSurface != NULL );
		COLORREF clrQuadTransparent =
			RGB(
				GetBValue( clrFoundTransparent ),
				GetGValue( clrFoundTransparent ),
				GetRValue( clrFoundTransparent )
				);
		for( LONG nPixel = 0; nPixel < nIconPixelCount ; nPixel++ )
			pOutSurface[nPixel] = clrQuadTransparent;
		HGDIOBJ hOldBitmap = ::SelectObject( hDC, hDIB );

		VERIFY(
			::DrawIconEx(
				hDC, 0, 0, hIcon, _sizeIcon.cx, _sizeIcon.cy,
				0, (HBRUSH)NULL, DI_NORMAL
				)
			);
		if( ii.hbmColor != NULL && ii.hbmMask != NULL )
		{
			::SelectObject( hDC, ii.hbmMask );	
			for( int y = 0; y < _sizeIcon.cy; y++ )
			{
				for( int x = 0; x < _sizeIcon.cx; x++ )
				{
					COLORREF clrMask = ::GetPixel( hDC, x, y );
					if( clrMask == RGB(255,255,255) )
					{
						COLORREF * pClr =
							pOutSurface + nIconPixelCount - 1
							- (y * _sizeIcon.cx)
							- _sizeIcon.cx + x + 1
							;
						*pClr = clrQuadTransparent;
					}
				} // for( int x = 0; x < _sizeIcon.cx; x++ )
			} // for( int y = 0; y < _sizeIcon.cy; y++ )
		} // if( ii.hbmColor != NULL && ii.hbmMask != NULL )

		::SelectObject( hDC, hOldBitmap );
		::DeleteDC( hDC );

		hBitmap = hDIB;
		clrTransparent = clrFoundTransparent;
		if( ppClrSurface != NULL )
			(*ppClrSurface) = pOutSurface;
		if( pArr != NULL )
		{
			ASSERT( _map.GetCount() >= 1 );
			pArr->Add( clrTransparent );
			POSITION pos = _map.GetStartPosition();
			ASSERT( pos != NULL );
			while( pos != NULL )
			{
				DWORD dwEntries = 0;
				COLORREF clr = RGB(0,0,0);
				_map.GetNextAssoc( pos, clr, dwEntries );
				ASSERT( dwEntries > 0 );
				if( clr != clrTransparent )
					pArr->Add( clr );
			} // while( pos != NULL )
		} // if( pArr != NULL )
		return true;

	} // else from if( IsBitmapBased() )
}

HICON CExtCmdIcon::GetIcon(
	bool bReplaceSysColors // = true // for bimap based items
	) const
{
	if( !m_IconCache.IsEmpty() )
	{
		HICON hIcon = m_IconCache.GetIcon();
		ASSERT( hIcon != NULL );
		return hIcon;
	}
HICON hIcon = m_BitmapCache.CreateHICON( bReplaceSysColors );
	ASSERT( hIcon != NULL );
	if( hIcon != NULL )
		((IconCache *)&m_IconCache)->AssignFromHICON( hIcon, false );
	return hIcon;
}

HICON CExtCmdIcon::DetachAsHICON(
	bool bReplaceSysColors // = true // for bimap based items
	)
{
	if( IsEmpty() )
		return NULL;
	if( !m_IconAlpha.IsEmpty() )
		m_IconAlpha.Empty();
	if( !m_IconCache.IsEmpty() )
	{
		m_BitmapCache.Empty();
		HICON hIcon = m_IconCache.Detach();
		ASSERT( hIcon != NULL );
		return hIcon;
	}
HICON hIcon = m_BitmapCache.CreateHICON( bReplaceSysColors );
	ASSERT( hIcon != NULL );
	m_BitmapCache.Empty();
	return hIcon;
}

void CExtCmdIcon::SyncSysColors()
{
COLORREF clrAlpha = g_PaintManager->GetIconAlphaColor();
	AlphaColorSet( clrAlpha );
double lfAlphaAdjustPercent = g_PaintManager->GetIconAlphaAdjustPercent();
	AlphaAdjustPercentSet( lfAlphaAdjustPercent );
	if(		IsEmpty()
		||	(! IsBitmapBased() )
		)
		return;
	m_IconCache.Empty();
}

void CExtCmdIcon::OnSysColorChange()
{
	SyncSysColors();
}

void CExtCmdIcon::OnSettingChange(
	UINT uFlags,
	__EXT_MFC_SAFE_LPCTSTR lpszSection
	)
{
	uFlags;
	lpszSection;
	SyncSysColors();
}

void CExtCmdIcon::OnDisplayChange(
	INT nDepthBPP,
	CPoint ptSizes
	)
{
	nDepthBPP;
	ptSizes;
	SyncSysColors();
}

void CExtCmdIcon::OnThemeChanged(
	WPARAM wParam,
	LPARAM lParam
	)
{
	wParam;
	lParam;
	SyncSysColors();
}

void CExtCmdIcon::Paint(
	HDC hDC,
	const POINT & pt,
	CExtCmdIcon::e_draw_type_t eDT, // = __DRAW_NORMAL
	HBRUSH hBrush // = NULL
	) const
{
	ASSERT( !IsEmpty() );
	ASSERT( hDC != NULL );
	ASSERT(
			CExtCmdIcon::e_draw_type_t::__DRAW_VAL_MIN <= eDT
		&&	eDT <= CExtCmdIcon::e_draw_type_t::__DRAW_VAL_MAX
		);
	if( eDT == CExtCmdIcon::e_draw_type_t::__DRAW_ALPHA )
	{
		int nBPP = ::GetDeviceCaps( hDC, BITSPIXEL );
		if( nBPP <= 8 )
			eDT = CExtCmdIcon::e_draw_type_t::__DRAW_MONO;
		else
		{
			HICON hIcon = GetIconAlpha();
			if( hIcon == NULL )
				eDT = CExtCmdIcon::e_draw_type_t::__DRAW_MONO;
			else
			{
				ASSERT( !m_IconAlpha.IsEmpty() );
				m_IconAlpha.Paint( hDC, pt, CExtCmdIcon::e_draw_type_t::__DRAW_NORMAL, hBrush );
				return;
			} // else from if( hIcon == NULL )
		} // else from if( nBPP <= 8 )
	} // if( eDT == CExtCmdIcon::e_draw_type_t::__DRAW_ALPHA )
HICON hIcon = GetIcon();
	ASSERT( hIcon != NULL );
	if( hIcon == NULL )
		return;
	ASSERT( !m_IconCache.IsEmpty() );
	m_IconCache.Paint( hDC, pt, eDT, hBrush );
}

INT CExtCmdIcon::GetColorMap(
	CMap < COLORREF, COLORREF, DWORD, DWORD > & _map
	) const
{
	ASSERT( !IsEmpty() );
	if( !m_BitmapCache.IsEmpty() )
		return m_BitmapCache.GetColorMap( _map );
	else
		return m_IconCache.GetColorMap( _map );
}

INT CExtCmdIcon::GetBitsPerPixel() const
{
	ASSERT( !IsEmpty() );
	if( !m_BitmapCache.IsEmpty() )
		return m_BitmapCache.GetBitsPerPixel();
	else
		return m_IconCache.GetBitsPerPixel();
}

bool CExtCmdIcon::CreateScaledCopy(
	const CExtCmdIcon & other,
	CSize newSize
	)
{
	if(		other.IsEmpty()
		||	newSize.cx <= 0
		||	newSize.cy <= 0
		)
	{
		ASSERT( FALSE );
		return false;
	}
	Empty();
CSize sizeOther = other.GetSize();
	if( sizeOther == newSize )
	{
		AssignFromOther( other );
		ASSERT( ! IsEmpty() );
		return (! IsEmpty() );
	}
HBITMAP hBitmap = NULL;
COLORREF clrTransparent = RGB( 0, 0, 0 );
	if(	!other.ExtractEditableBitmap(
			hBitmap,
			clrTransparent
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	ASSERT( hBitmap != NULL );
CBitmap bmpSrc;
	bmpSrc.Attach( hBitmap );
CDC dcSrc;
	if( !dcSrc.CreateCompatibleDC(NULL) )
	{
		ASSERT( FALSE );
		return false;
	}
CBitmap * pBmpSrcOld = dcSrc.SelectObject( &bmpSrc );
CBitmap bmpDst;
	if( !bmpDst.CreateCompatibleBitmap( &dcSrc, newSize.cx, newSize.cy ) )
	{
		dcSrc.SelectObject( pBmpSrcOld );
		ASSERT( FALSE );
		return false;
	}
CDC dcDst;
	if( !dcDst.CreateCompatibleDC(NULL) )
	{
		dcSrc.SelectObject( pBmpSrcOld );
		ASSERT( FALSE );
		return false;
	}
CBitmap * pBmpDstOld = dcDst.SelectObject( &bmpDst );
	dcDst.StretchBlt(
		0,
		0,
		newSize.cx,
		newSize.cy,
		&dcSrc,
		0,
		0,
		sizeOther.cx,
		sizeOther.cy,
		SRCCOPY
		);
	dcDst.SelectObject( pBmpDstOld );
	dcSrc.SelectObject( pBmpSrcOld );
	if( other.IsBitmapBased() )
	{
		AssignFromHBITMAP(
			(HBITMAP)bmpDst.GetSafeHandle(),
			clrTransparent
			);
		ASSERT( ! IsEmpty() );
	} // if( other.IsBitmapBased() )
	else
	{
		CExtCmdIcon _iconBmp(
			(HBITMAP)bmpDst.GetSafeHandle(),
			clrTransparent
			);
		if( _iconBmp.IsEmpty() )
		{
			ASSERT( FALSE );
			return false;
		}
		AssignFromHICON(
			_iconBmp.DetachAsHICON(),
			false
			);
		ASSERT( ! IsEmpty() );
	} // else from if( other.IsBitmapBased() )
	return (! IsEmpty() );
}

COLORREF CExtCmdIcon::AlphaColorGet() const
{
	return m_clrAlpha;
}

COLORREF CExtCmdIcon::AlphaColorSet(
	COLORREF clrAlpha // = COLORREF(-1L)
	)
{
	if( clrAlpha == m_clrAlpha )
		return m_clrAlpha;
COLORREF clrAlphaOld = m_clrAlpha;
	m_clrAlpha = clrAlpha;
	if( !m_IconAlpha.IsEmpty() )
		m_IconAlpha.Empty();
	return clrAlphaOld;
}

double CExtCmdIcon::AlphaAdjustPercentGet() const
{
	return m_lfAlphaAdjustPercent;
}

double CExtCmdIcon::AlphaAdjustPercentSet(
	double lfAlphaAdjustPercent // = 0.0
	)
{
	if( lfAlphaAdjustPercent == m_lfAlphaAdjustPercent )
		return m_lfAlphaAdjustPercent;
double lfAlphaAdjustPercentOld = m_lfAlphaAdjustPercent;
	m_lfAlphaAdjustPercent = lfAlphaAdjustPercent;
	if( !m_IconAlpha.IsEmpty() )
		m_IconAlpha.Empty();
	return lfAlphaAdjustPercentOld;
}

HICON CExtCmdIcon::GetIconAlpha() const
{
	if( m_clrAlpha == COLORREF(-1L) )
		return NULL;
	if( m_IconAlpha.IsEmpty() )
	{
		OnGenerateAlphaIconCache();
		ASSERT( !m_IconAlpha.IsEmpty() );
		if( m_IconAlpha.IsEmpty() )
			return NULL;
	} // if( m_IconAlpha.IsEmpty() )
HICON hIconAlpha = m_IconAlpha.GetIcon();
	ASSERT( hIconAlpha != NULL );
	return hIconAlpha;
}

void CExtCmdIcon::OnGenerateAlphaIconCache() const
{
	if( !m_IconAlpha.IsEmpty() )
		m_IconAlpha.Empty();
COLORREF clrAlpha = AlphaColorGet();
	if( clrAlpha == COLORREF(-1L) )
		return;
	if( IsEmpty() )
		return;
CSize _size = GetSize();
	ASSERT( _size.cx > 0 && _size.cy > 0 );
HBITMAP hBitmap = NULL;
COLORREF clrTransparent = RGB(0,0,0);
COLORREF * pClrSurface = NULL;
	if( !ExtractEditableBitmap(
			hBitmap,
			clrTransparent,
			NULL,
			&pClrSurface
			)
		)
	{
		ASSERT( FALSE );
		return;
	}
	ASSERT( hBitmap != NULL && pClrSurface != NULL );
COLORREF clrQuadTransparent =
		RGB(
			GetBValue(clrTransparent),
			GetGValue(clrTransparent),
			GetRValue(clrTransparent)
			);
int nMakeSpecR = GetBValue(clrAlpha);
int nMakeSpecG = GetGValue(clrAlpha);
int nMakeSpecB = GetRValue(clrAlpha);
INT nPixelCount = _size.cx * _size.cy;
	for( INT nPixelIdx = 0; nPixelIdx < nPixelCount; nPixelIdx++ )
	{
		COLORREF & clrQuad = pClrSurface[nPixelIdx];
		if( clrQuad == clrQuadTransparent )
			continue;
		double lfVal =
				( double(GetBValue(clrQuad)) / 255.0 ) * 0.3
			+	( double(GetGValue(clrQuad)) / 255.0 ) * 0.5
			+	( double(GetRValue(clrQuad)) / 255.0 ) * 0.2
			;
		if( m_lfAlphaAdjustPercent != 0.0 )
		{
			if( m_lfAlphaAdjustPercent > 0.0 )
				lfVal = lfVal + (1.0 - lfVal) * m_lfAlphaAdjustPercent;
			else //  m_lfAlphaAdjustPercent < 0.0 
				lfVal = lfVal * (1.0 + m_lfAlphaAdjustPercent);
			if( lfVal > 1.0 )
				lfVal = 1.0;
			else if( lfVal < 0.0 )
				lfVal = 0.0;
		} // if( m_lfAlphaAdjustPercent != 0.0 )
		//clrQuad = RGB( nMakeSpec, nMakeSpec, nMakeSpec ); <-- gray-scale version
		int nVal = (int)( lfVal * 255.0 );
		clrQuad = 
			RGB(
				( ( ::MulDiv( nMakeSpecR, nVal, 255 ) * 3 + nVal ) / 4 ),
				( ( ::MulDiv( nMakeSpecG, nVal, 255 ) * 3 + nVal ) / 4 ),
				( ( ::MulDiv( nMakeSpecB, nVal, 255 ) * 3 + nVal ) / 4 )
				);
	} // for( INT nPixelIdx = 0; nPixelIdx < nPixelCount; nPixelIdx++ )
HICON hIconAlpha =
		CExtPaintManager::stat_HBITMAP2HICON(
			hBitmap,
			clrTransparent,
			NULL,
			false // bReplaceSysColors
			);
	ASSERT( hIconAlpha != NULL );
	if( hIconAlpha != NULL )
	{
		m_IconAlpha.AssignFromHICON( hIconAlpha, false );
		ASSERT( !m_IconAlpha.IsEmpty() );
	} // if( hIconAlpha != NULL )
	::DeleteObject( hBitmap );
}

