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

#if (!defined __EXT_MEMORY_DC_H)
#define __EXT_MEMORY_DC_H

//
// The enhanced version of Keith Rule's memory DC class
//

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

class __PROF_UIS_API CExtMemoryDC : public CDC 
{
protected:	
	CBitmap m_bmp;			// offscreen bitmap
	CBitmap * m_pOldBmp;	// bitmap originally found in CExtMemoryDC
	CDC * m_pDC;			// saves CDC passed in constructor
	CRect m_rect;			// rectangle of drawing area
	BOOL m_bMemDC;			// TRUE if CDC really is a memory DC
	COLORREF * m_pHelperDibSurface; // DIB surface if created as DIB

public:
	enum eOptFlags
	{
		MDCOPT_TO_MEMORY	= 0x001L,
		MDCOPT_FILL_SURFACE	= 0x002L,
		MDCOPT_FORCE_BLACK	= 0x004L,
		MDCOPT_FORCE_DIB	= 0x008L,
		MDCOPT_NO_COPY_OPT	= 0x010L,
		MDCOPT_FILL_BITS	= 0x020L,
		MDCOPT_DEFAULT = MDCOPT_TO_MEMORY|MDCOPT_FILL_SURFACE
	};

	void __InitMemoryDC(
		CDC * pDC,
		const CRect * pRect = NULL,
		DWORD dwOptFlags = MDCOPT_DEFAULT
		)
	{
		ASSERT( m_pDC == NULL ); 
		ASSERT( pDC != NULL ); 
		ASSERT( pDC->GetSafeHdc() != NULL ); 

		m_pDC = pDC;
		m_pOldBmp = NULL;
		if( dwOptFlags & MDCOPT_TO_MEMORY )
			m_bMemDC = !pDC->IsPrinting();
		else
			m_bMemDC = FALSE;

		if( pRect == NULL ) 
			pDC->GetClipBox( &m_rect );
		else 
			m_rect = *pRect;
		if( m_bMemDC ) 
		{
			CreateCompatibleDC( pDC );
			if( (dwOptFlags & MDCOPT_NO_COPY_OPT) == 0 )
				pDC->LPtoDP( &m_rect );
			if( dwOptFlags & MDCOPT_FORCE_BLACK )
				m_bmp.CreateBitmap(
					m_rect.Width(),
					m_rect.Height(),
					1,
					1,
					NULL
					);
			else if( dwOptFlags & MDCOPT_FORCE_DIB )
			{
				BITMAPINFOHEADER bih;
				bih.biSize = sizeof(BITMAPINFOHEADER);
				bih.biWidth = m_rect.Width();
				bih.biHeight = m_rect.Height();
				bih.biPlanes = 1;
				bih.biBitCount = 32;
				bih.biCompression = BI_RGB;
				bih.biSizeImage = bih.biWidth*bih.biHeight;
				bih.biXPelsPerMeter = 0;
				bih.biYPelsPerMeter = 0;
				bih.biClrUsed = 0;
				bih.biClrImportant = 0;
				ASSERT( m_pHelperDibSurface == NULL );
				HBITMAP hDIB =
					::CreateDIBSection(
						pDC->GetSafeHdc(),
						(LPBITMAPINFO)&bih,
						DIB_RGB_COLORS,
						(void **)&m_pHelperDibSurface,
						NULL,
						NULL
						);
				if( hDIB == NULL || m_pHelperDibSurface == NULL )
				{
					ASSERT( FALSE );
					return;
				}
				m_bmp.Attach( hDIB );
			}
			else
				m_bmp.CreateCompatibleBitmap(
					pDC,
					m_rect.Width(),
					m_rect.Height()
					);
			m_pOldBmp = SelectObject( &m_bmp );
			if( (dwOptFlags & MDCOPT_NO_COPY_OPT) == 0 )
			{
				SetMapMode( pDC->GetMapMode() );
				pDC->DPtoLP( &m_rect );
				SetWindowOrg( m_rect.left, m_rect.top );
			}
		} 
		else 
		{
			if( pDC->IsPrinting() )
				m_bPrinting = pDC->m_bPrinting;
			m_hDC       = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}

		if( dwOptFlags & MDCOPT_FILL_BITS )
			BitBlt(
				0, 0,
				m_rect.Width(), m_rect.Height(),
				pDC,
				m_rect.left, m_rect.top,
				SRCCOPY
				);
		else if( dwOptFlags & MDCOPT_FILL_SURFACE )
			FillSolidRect( m_rect, pDC->GetBkColor() );
	};

	void __Flush( BOOL bMoveImage = TRUE )
	{
		if( m_pDC == NULL )
			return;

		if( m_bMemDC ) 
		{
			if( bMoveImage )
				m_pDC->BitBlt(
					m_rect.left, m_rect.top,
					m_rect.Width(), m_rect.Height(),
					this,
					m_rect.left, m_rect.top,
					SRCCOPY
					);			
			SelectObject( m_pOldBmp );
		} 
		else 
		{
			m_hDC = m_hAttribDC = NULL;
		}	
		m_pDC = NULL;
		m_pOldBmp = NULL;
		m_rect.SetRectEmpty();
		m_bMemDC = FALSE;
		m_pHelperDibSurface = NULL;
	};

	CExtMemoryDC(
		CDC * pDC = NULL,
		const CRect * pRect = NULL,
		DWORD dwOptFlags = MDCOPT_DEFAULT
		) : CDC()
	{
		m_pOldBmp = NULL;
		m_pDC = NULL;
		m_rect.SetRectEmpty();
		m_pHelperDibSurface = NULL;
		if( pDC != NULL )
			__InitMemoryDC(
				pDC,
				pRect,
				dwOptFlags
				);
	};
	~CExtMemoryDC()	
	{
		__Flush();
	};
	
	CRect & __GetDrawRect()
	{
		return m_rect;
	};
	const CRect & __GetDrawRect() const
	{
		return m_rect;
	};

	BOOL __IsMemDC() const
	{
		return m_bMemDC;
	}

	CExtMemoryDC* operator->() 
	{
		return this;
	};
	operator CExtMemoryDC*() 
	{
		return this;
	};

	COLORREF * GetDibSurface()
	{
		ASSERT( m_pHelperDibSurface != NULL );
		return m_pHelperDibSurface;
	};
	const COLORREF * GetDibSurface() const
	{
		ASSERT( m_pHelperDibSurface != NULL );
		return m_pHelperDibSurface;
	};

#if (defined __EXT_PAINT_MANAGER_H)

	void __TransparentBltFrom(
		CDC & dcFrom,       // source DC
		int nXOriginDest,   // x-coord of destination upper-left corner
		int nYOriginDest,   // y-coord of destination upper-left corner
		int nWidthDest,     // width of destination rectangle
		int hHeightDest,    // height of destination rectangle
		int nXOriginSrc,    // x-coord of source upper-left corner
		int nYOriginSrc,    // y-coord of source upper-left corner
		int nWidthSrc,      // width of source rectangle
		int nHeightSrc,     // height of source rectangle
		COLORREF crTransparent  // color to make transparent
		)
	{
		CExtPaintManager::stat_TransparentBlt(
			GetSafeHdc(),
			nXOriginDest, nYOriginDest,
			nWidthDest, hHeightDest,
			dcFrom.GetSafeHdc(),
			nXOriginSrc, nYOriginSrc,
			nWidthSrc, nHeightSrc,
			crTransparent
			);
	};

	void __TransparentBltTo(
		CDC & dcTo,         // destination DC
		int nXOriginDest,   // x-coord of destination upper-left corner
		int nYOriginDest,   // y-coord of destination upper-left corner
		int nWidthDest,     // width of destination rectangle
		int hHeightDest,    // height of destination rectangle
		int nXOriginSrc,    // x-coord of source upper-left corner
		int nYOriginSrc,    // y-coord of source upper-left corner
		int nWidthSrc,      // width of source rectangle
		int nHeightSrc,     // height of source rectangle
		COLORREF crTransparent  // color to make transparent
		)
	{
		CExtPaintManager::stat_TransparentBlt(
			dcTo.GetSafeHdc(),
			nXOriginDest, nYOriginDest,
			nWidthDest, hHeightDest,
			GetSafeHdc(),
			nXOriginSrc, nYOriginSrc,
			nWidthSrc, nHeightSrc,
			crTransparent
			);
	};

#endif // if with CExtPaintManager

}; // class __PROF_UIS_API CExtMemoryDC


#endif // !defined(__EXT_MEMORY_DC_H)
