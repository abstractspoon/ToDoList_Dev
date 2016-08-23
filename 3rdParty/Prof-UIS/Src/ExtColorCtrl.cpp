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

#include "stdafx.h"

// 
// The idea of the HSL colorspace algorithm  belongs to Patrick Prache
// The idea of the hexagon pallette algorithm belongs to Jack Mesic
//

#if (!defined __EXTCOLORCTRL_H)
	#include <ExtColorCtrl.h>
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

#define __SEL_BOX_DX 6
#define __SEL_BOX_DY 6

static void _CreateCompatibleBitmapImpl(
	CBitmap & bmp,
	CDC & dc,
	int dX,
	int dY
	)
{
	ASSERT( bmp.GetSafeHandle() == NULL );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( dX > 0 && dY > 0 );
/**/
	VERIFY(
		bmp.CreateCompatibleBitmap(
			&dc,
			dX,
			dY
			)
		);
	ASSERT( bmp.GetSafeHandle() != NULL );
/**/

/**
BITMAPINFOHEADER bih;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = dX;
	bih.biHeight = dY;
	bih.biPlanes = 1;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = dX * dY;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

COLORREF *pClrFadeBitsSrc = NULL;
HBITMAP	hDIB =
		::CreateDIBSection(
			dc.GetSafeHdc(),
			(LPBITMAPINFO)&bih,
			DIB_RGB_COLORS,
			(void **)pClrFadeBitsSrc,
			NULL,
			NULL
			);
//	ASSERT( hDIB != NULL && pClrFadeBitsSrc != NULL );
	ASSERT( hDIB != NULL );
	pClrFadeBitsSrc;

	bmp.Attach( hDIB );
**/

}

/////////////////////////////////////////////////////////////////////////////
// CExtColorCtrl::any_picker_data_t

CExtColorCtrl::any_picker_data_t::any_picker_data_t()
	: m_eMoveMode( _MM_NONE )
	, m_sizeTotal( 50, 50 )
	, m_clr( RGB(255,255,255) )
	, m_clrOld( RGB(255,255,255) )
{
	CExtPaintManager::stat_RGBtoHSL(
		m_clr,
		&m_fCurrHue,
		&m_fCurrSaturation,
		&m_fCurrLuminance
		);
}

CExtColorCtrl::any_picker_data_t::~any_picker_data_t()
{
}

void CExtColorCtrl::any_picker_data_t::OnInit(
	CDC & dc,
	CPalette * pPalette,
	CSize sizeTotal,
	bool bOnlySizeChanged
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	dc;
	pPalette;
	bOnlySizeChanged;
	m_sizeTotal = sizeTotal;
	if( m_sizeTotal.cx <= 0 )
		m_sizeTotal.cx = 1;
	if( m_sizeTotal.cy <= 0 )
		m_sizeTotal.cy = 1;
}

void CExtColorCtrl::any_picker_data_t::OnSetOldRGB(
	COLORREF clr
	)
{
	m_clrOld = clr;
}

void CExtColorCtrl::any_picker_data_t::OnSetRGB(
	COLORREF clr
	)
{
	m_clr = clr;
	CExtPaintManager::stat_RGBtoHSL(
		m_clr,
		&m_fCurrHue,
		&m_fCurrSaturation,
		&m_fCurrLuminance
		);
}

void CExtColorCtrl::any_picker_data_t::OnSetHLS(
	double hue,
	double luminance,
	double saturation
	)
{
	m_fCurrHue			= hue;
	m_fCurrSaturation	= saturation;
	m_fCurrLuminance	= luminance;
	m_clr =
		CExtPaintManager::stat_HLStoRGB(
			m_fCurrHue,
			m_fCurrSaturation,
			m_fCurrLuminance
			);
}

/////////////////////////////////////////////////////////////////////////////
// CExtColorCtrl::cell_based_data_t

#define NUM_LEVELS			7
#define TAN30				0.57735026918962F
#define YOFFSET				(1.5F * TAN30)
#define PI					3.14159265358979
#define NUM_HEX_FULL_CELLS 144
#define NUM_HEX_GRAY_CELLS 17
#define HEX_DIST_GAP 10

#define NUM_RGB_SINGLE_CELLS 256
#define NUM_RGB_FULL_CELLS (NUM_RGB_SINGLE_CELLS*3)

static const float cfxFOOffset[] = { -0.5, -1.0, -0.5, 0.5, 1.0, 0.5 };
static const float cfyFOOffset[] = { YOFFSET, 0.0, -YOFFSET, -YOFFSET, 0.0, YOFFSET };

static int GetAngleFromPoint(int nX, int nY)
{
	double dAngle = atan2( (double)nY, (double)nX );
	
	return (int) (dAngle * 180.0/PI);
}

void CExtColorCtrl::cell_based_data_t::OnInit(
	CDC & dc,
	CPalette * pPalette,
	CSize sizeTotal,
	bool bOnlySizeChanged
	)
{
	any_picker_data_t::OnInit(
		dc,
		pPalette,
		sizeTotal,
		bOnlySizeChanged
		);
	Generate( bOnlySizeChanged );
}

void CExtColorCtrl::cell_based_data_t::OnDone()
{
	ReleaseAll();
}

void CExtColorCtrl::cell_based_data_t::OnDraw(
	CDC & dc,
	CPalette * pPalette
	)
{
CPalette * pOldPalette = NULL;
    if( pPalette != NULL )
    {
        pOldPalette =
			dc.SelectPalette( pPalette, FALSE );
        dc.RealizePalette();
    }

#ifdef _DEBUG
int nCellCount = m_vCells.GetSize();
	ASSERT( nCellCount > 0 );
#endif // _DEBUG

INT		it = 0,
		itFocus0 = m_vCells.GetSize(),
		itFocus1 = m_vCells.GetSize();
	for( ; it < m_vCells.GetSize(); ++it )
	{
		cell_t & _c = m_vCells[it];
		_c.OnDraw( dc, pPalette, false );
		if( _c.m_clr != m_clr )
			continue;
		if( itFocus0 == m_vCells.GetSize() )
		{
			itFocus0 = it;
			continue;
		}
		if( itFocus1 == m_vCells.GetSize() )
			itFocus1 = it;
	}
	if( itFocus0 != m_vCells.GetSize() )
		m_vCells[itFocus0].OnDraw( dc, pPalette, true );
	if( itFocus1 != m_vCells.GetSize() )
		m_vCells[itFocus1].OnDraw( dc, pPalette, true );

	if( pOldPalette != NULL )
		dc.SelectPalette( pOldPalette, FALSE );
}

void CExtColorCtrl::cell_based_data_t::OnLButtonDown(
	UINT nFlags,
	CPoint point
	)
{
	m_eMoveMode = _MM_CELL_SCAN;
	OnMouseMove(
		nFlags|MK_LBUTTON,
		point
		);
}

void CExtColorCtrl::cell_based_data_t::OnLButtonUp(
	UINT nFlags,
	CPoint point
	)
{
	nFlags;
	point;
	m_eMoveMode = _MM_NONE;
}

void CExtColorCtrl::cell_based_data_t::OnMouseMove(
	UINT nFlags,
	CPoint point
	)
{
	nFlags;
	if( m_eMoveMode != _MM_CELL_SCAN )
		return;

#ifdef _DEBUG
int nCellCount = m_vCells.GetSize();
	ASSERT( nCellCount > 0 );
#endif // _DEBUG

INT it = 0;
	for( ; it < m_vCells.GetSize(); ++it )
	{
		cell_t & _c = m_vCells[it];
		if( _c.HitTest(point) )
		{
			OnSetRGB( _c.m_clr );
			return;
		}
	}
}

void CExtColorCtrl::cell_based_data_t::Generate(
	bool bOnlySizeChanged
	)
{
int nCellSizeByX =
		m_sizeTotal.cx / (2*(NUM_LEVELS+1)) + 2;
int nCellSizeByY =
		(m_sizeTotal.cy - HEX_DIST_GAP)
		/
		(2*(NUM_LEVELS+1)) + 2;
int nCellSize =
		min(
			nCellSizeByX,
			nCellSizeByY
			);
int x = m_sizeTotal.cx / 2;
int y =	(m_sizeTotal.cy
			- nCellSize * 2 - HEX_DIST_GAP )
		/ 2;

	m_vCells.SetSize( NUM_HEX_FULL_CELLS );

	// white hexagon at center
	if( !bOnlySizeChanged )
		m_vCells[0] = RGB(255,255,255);
	m_vCells[0].m_bAtFirstType = true;
	m_vCells[0].AdjustCellPosition(x,y,nCellSize);
	
int nNumber = 1;
	for( int nLevel = 1; nLevel < NUM_LEVELS; nLevel++ )
	{ // walk through all levels
		// level start position
		int nPosX = x + (nCellSize * nLevel);
		int nPosY = y;
		for( int nSide = 0; nSide < NUM_LEVELS - 1; nSide++ )
		{ // walk through all sided
			// set the deltas for the side
			int nDx = int(float(nCellSize)*cfxFOOffset[nSide]);
			int nDy = int(float(nCellSize)*cfyFOOffset[nSide]);
			for( int nCell = 0; nCell < nLevel; nCell++ )
			{ // walk through all cells at one side
				int nAngle =
					GetAngleFromPoint(
						nPosX - x,
						nPosY - y
						);
				double L =
					1. * (NUM_LEVELS - nLevel)
						/ NUM_LEVELS
					+ .1
					;
				if( !bOnlySizeChanged )
					m_vCells[nNumber].SetCellColor(
						CExtPaintManager::stat_GetRGBFromHLSExtend(
							(float) nAngle,
							L,
							1.0F
							)
						);
				m_vCells[nNumber].m_bAtFirstType = true;
				m_vCells[nNumber].AdjustCellPosition(
					nPosX,
					nPosY,
					nCellSize
					);
				nNumber++;
				// offset the position
				nPosX += nDx;
				nPosY += nDy;
			} // // walk through all cells at one side
		} // walk through all sided
	} // walk through all levels


int xGrayPos =
	(
		m_sizeTotal.cx -
		(
			nCellSize * 3
			+ nCellSize / 2
			+ ( nCellSize * (NUM_HEX_GRAY_CELLS-2) ) / 2
		)
	) / 2;

int yGrayCenter =
		m_sizeTotal.cy - nCellSize * 1 - HEX_DIST_GAP/2;
int nTmp = int( float(nCellSize) / 2.3 );
int yUpper = yGrayCenter - nTmp;
int yLower = yGrayCenter + nTmp;
int nGrayColorPos = 0;
	for(	int nCell = 0;
			nCell < NUM_HEX_GRAY_CELLS;
			nCell++, nNumber++,
				nGrayColorPos += 255/NUM_HEX_GRAY_CELLS
			)
	{ // walk through all gray-scale positions
		if( nCell == 0 )
		{ // if first - white cell
			if( !bOnlySizeChanged )
				m_vCells[nNumber].
					m_clr = RGB( 255, 255, 255 );
			m_vCells[nNumber].
				m_bAtFirstType = true;
			m_vCells[nNumber].
				AdjustCellPosition(
					xGrayPos,
					yGrayCenter,
					nCellSize*2
					);
			xGrayPos += nCellSize*2;
			continue;
		} // if first - white cell
		if( nCell == NUM_HEX_GRAY_CELLS-1 )
		{ // if last - black cell
			xGrayPos += (nCellSize*3)/2 - 1;
			if( !bOnlySizeChanged )
				m_vCells[nNumber].
					m_clr = RGB( 0, 0, 0 );
			m_vCells[nNumber].
				m_bAtFirstType = true;
			m_vCells[nNumber].
				AdjustCellPosition(
					xGrayPos,
					yGrayCenter,
					nCellSize*2
					);
			break;
		} // if last - black cell

		COLORREF clrGray =
			RGB(
				255-nGrayColorPos,
				255-nGrayColorPos,
				255-nGrayColorPos
				);
		if( !bOnlySizeChanged )
			m_vCells[nNumber].m_clr =
				clrGray;
		m_vCells[nNumber].
			m_bAtFirstType = true;
		m_vCells[nNumber].AdjustCellPosition(
			xGrayPos,
			( (nCell & 1 ) != 0 ) ? yUpper : yLower,
			nCellSize
			);
		xGrayPos += nCellSize / 2;
	} // walk through all gray-scale positions

}

void CExtColorCtrl::cell_based_data_t::ReleaseAll()
{
//	m_vCells.erase(
//		m_vCells.begin(),
//		m_vCells.end()
//		);
	m_eMoveMode = _MM_NONE;
}


/////////////////////////////////////////////////////////////////////////////
// CExtColorCtrl::grayscale_mixer_data_t
void CExtColorCtrl::grayscale_mixer_data_t::OnInit(
	CDC & dc,
	CPalette * pPalette,
	CSize sizeTotal,
	bool bOnlySizeChanged
	)
{
	any_picker_data_t::OnInit(
		dc,
		pPalette,
		sizeTotal,
		bOnlySizeChanged
		);
//	if( !bOnlySizeChanged )
//	{
//		ASSERT( m_vCells.empty() );
		m_vCells.SetSize( NUM_RGB_SINGLE_CELLS );
//	}
int nCellWidth = m_sizeTotal.cx/3 - 20;
int nCellCenter = m_sizeTotal.cx/2;
int nWorkHeight =
		(m_sizeTotal.cy - 20);
int nWorkY1 = 10;
int nCellHeight = nWorkHeight/NUM_RGB_SINGLE_CELLS;
	if( nCellHeight < 2 )
		nCellHeight = 2;
	for(int i=0; i < NUM_RGB_SINGLE_CELLS; i++ )
	{
		if( !bOnlySizeChanged )
		{
			m_vCells[i].m_clr = RGB(i,i,i);
			m_vCells[i].m_bAtFirstType = false;
			m_vCells[i].m_nBorderSize = 0;
		}
		CRect rc;
		rc.top =
			nWorkY1
			+ (nWorkHeight*i)/NUM_RGB_SINGLE_CELLS;
		rc.bottom = rc.top + nCellHeight;
		rc.left = nCellCenter - nCellWidth/2;
		rc.right = rc.left + nCellWidth;
		m_vCells[i].m_rcPosition = rc;
	}
}

void CExtColorCtrl::grayscale_mixer_data_t::OnDraw(
	CDC & dc,
	CPalette * pPalette
	)
{
CPalette * pOldPalette = NULL;
    if( pPalette != NULL )
    {
        pOldPalette =
			dc.SelectPalette( pPalette, FALSE );
        dc.RealizePalette();
    }

INT it = 0;
	for( ; it < m_vCells.GetSize(); ++it )
		m_vCells[it].OnDraw( dc, pPalette, false );
	if(	GetRValue(m_clr) == GetGValue(m_clr)
		&&
		GetRValue(m_clr) == GetBValue(m_clr)
		)
		m_vCells[ GetRValue(m_clr) ].OnDraw( dc, pPalette, true );

	if( pOldPalette != NULL )
		dc.SelectPalette( pOldPalette, FALSE );
}

/////////////////////////////////////////////////////////////////////////////
// CExtColorCtrl::rgb_mixer_data_t
void CExtColorCtrl::rgb_mixer_data_t::OnInit(
	CDC & dc,
	CPalette * pPalette,
	CSize sizeTotal,
	bool bOnlySizeChanged
	)
{
	any_picker_data_t::OnInit(
		dc,
		pPalette,
		sizeTotal,
		bOnlySizeChanged
		);
//	if( !bOnlySizeChanged )
//	{
//		ASSERT( m_vCells.empty() );
		m_vCells.SetSize( NUM_RGB_FULL_CELLS );
//	}

int nCellWidth = m_sizeTotal.cx/3 - 20;
int nCellCenter =
		m_sizeTotal.cx/2;
int nWorkHeight =
		(m_sizeTotal.cy - 20);
int nWorkY1 = 10; // + nWorkHeight/2;
	int nCellHeight = nWorkHeight/NUM_RGB_SINGLE_CELLS;
	if( nCellHeight < 2 )
		nCellHeight = 2;
	for(int i=0; i < NUM_RGB_SINGLE_CELLS; i++ )
	{
		if( !bOnlySizeChanged )
		{
			m_vCells[i].m_clr = RGB(i,0,0);
			m_vCells[i].m_bAtFirstType = false;
			m_vCells[i].m_nBorderSize = 0;
			m_vCells[i+NUM_RGB_SINGLE_CELLS].m_clr = RGB(0,i,0);
			m_vCells[i+NUM_RGB_SINGLE_CELLS].m_bAtFirstType = false;
			m_vCells[i+NUM_RGB_SINGLE_CELLS].m_nBorderSize = 0;
			m_vCells[i+NUM_RGB_SINGLE_CELLS*2].m_clr = RGB(0,0,i);
			m_vCells[i+NUM_RGB_SINGLE_CELLS*2].m_bAtFirstType = false;
			m_vCells[i+NUM_RGB_SINGLE_CELLS*2].m_nBorderSize = 0;
		}
		CRect rcG;
		rcG.top =
			nWorkY1
			+ (nWorkHeight*i)/NUM_RGB_SINGLE_CELLS;
		rcG.bottom = rcG.top + nCellHeight;
		rcG.left = nCellCenter - nCellWidth/2;
		rcG.right = rcG.left + nCellWidth;
		CRect rcR(rcG),rcB(rcG);
		rcR.OffsetRect(-nCellWidth-20,0);
		rcB.OffsetRect(nCellWidth+20,0);
		m_vCells[i].m_rcPosition = rcR;
		m_vCells[i+NUM_RGB_SINGLE_CELLS].m_rcPosition = rcG;
		m_vCells[i+NUM_RGB_SINGLE_CELLS*2].m_rcPosition = rcB;
	}
}

void CExtColorCtrl::rgb_mixer_data_t::OnDraw(
	CDC & dc,
	CPalette * pPalette
	)
{
CPalette * pOldPalette = NULL;
    if( pPalette != NULL )
    {
        pOldPalette =
			dc.SelectPalette( pPalette, FALSE );
        dc.RealizePalette();
    }

INT it = 0;
	for( ; it < m_vCells.GetSize(); ++it )
		m_vCells[it].OnDraw( dc, pPalette, false );
	m_vCells[ GetRValue(m_clr) ].OnDraw( dc, pPalette, true );
	m_vCells[ GetGValue(m_clr)+NUM_RGB_SINGLE_CELLS ].OnDraw( dc, pPalette, true );
	m_vCells[ GetBValue(m_clr)+NUM_RGB_SINGLE_CELLS*2 ].OnDraw( dc, pPalette, true );

	if( pOldPalette != NULL )
		dc.SelectPalette( pOldPalette, FALSE );
}

void CExtColorCtrl::rgb_mixer_data_t::OnMouseMove(
	UINT nFlags,
	CPoint point
	)
{
	nFlags;
	if( m_eMoveMode != _MM_CELL_SCAN )
		return;
	for( int i = 0; i < NUM_RGB_FULL_CELLS; i++ )
	{
		if( !m_vCells[i].HitTest(point) )
			continue;
		if( i < NUM_RGB_SINGLE_CELLS )
		{
				m_clr =
					RGB(
						i,
						GetGValue(m_clr),
						GetBValue(m_clr)
						);
				return;
		}
		if( i < NUM_RGB_SINGLE_CELLS*2 )
		{
				m_clr =
					RGB(
						GetRValue(m_clr),
						i-NUM_RGB_SINGLE_CELLS,
						GetBValue(m_clr)
						);
				return;
		}
				m_clr =
					RGB(
						GetRValue(m_clr),
						GetGValue(m_clr),
						i-NUM_RGB_SINGLE_CELLS*2
						);
				return;
	} // for( int i = 0; i<NUM_RGB_FULL_CELLS; i++ )
}

/////////////////////////////////////////////////////////////////////////////
// CExtColorCtrl::rgb_cube_data_t

#define DEFAULT_RGB_CUBE_GAP 5
#define DEF_RGB_CUBE_HL 0
#define DEF_RGB_CUBE_TRANSPARENT RGB(255,255,255)

CExtColorCtrl::rgb_cube_data_t::rgb_cube_data_t()
{
	m_nSideGap = DEFAULT_RGB_CUBE_GAP;
	m_sizeHalfSelBox.cx = m_sizeHalfSelBox.cy = 4;
	RecalcLayout();
}

CExtColorCtrl::rgb_cube_data_t::~rgb_cube_data_t()
{
	ReleaseAll();
}

void CExtColorCtrl::rgb_cube_data_t::ReleaseAll()
{
	if( m_dcCube.GetSafeHdc() )
		m_dcCube.DeleteDC();
	m_eMoveMode	= _MM_NONE;
}

void CExtColorCtrl::rgb_cube_data_t::Generate(
	CDC &dc,
	CPalette * pPalette
	)
{
	if( m_dcCube.GetSafeHdc() == NULL )
	{ // if needs re-create anything
		m_rcCube.SetRect(
			0, 0, m_sizeTotal.cx, m_sizeTotal.cy );
		m_rcCube.DeflateRect( m_nSideGap, m_nSideGap );
		
		CSize sizeCube = m_rcCube.Size();
		if( sizeCube.cx > sizeCube.cy )
			m_rcCube.DeflateRect(
				(sizeCube.cx - sizeCube.cy)/2,
				0
				);
		if( sizeCube.cy > sizeCube.cx )
			m_rcCube.DeflateRect(
				0,
				(sizeCube.cy - sizeCube.cx)/2
				);
		sizeCube = m_rcCube.Size();
		
		if( (sizeCube.cx & 0x3) != 0 )
			m_rcCube.right--;
		if( (sizeCube.cx & 0x3) != 0 )
			m_rcCube.right--;
		if( (sizeCube.cx & 0x3) != 0 )
			m_rcCube.right--;

		if( (sizeCube.cy & 0x3) != 0 )
			m_rcCube.bottom--;
		if( (sizeCube.cy & 0x3) != 0 )
			m_rcCube.bottom--;
		if( (sizeCube.cy & 0x3) != 0 )
			m_rcCube.bottom--;
		
		sizeCube = m_rcCube.Size();
		int nMetric = sizeCube.cy/4;
		sizeCube = m_rcCube.Size();
		m_ptCubeC = m_rcCube.CenterPoint();
		m_ptCubeR.x = m_ptCubeC.x;
		m_ptCubeR.y = m_rcCube.top;
		m_ptCubeG.x = m_rcCube.left;
		m_ptCubeG.y = m_rcCube.bottom-nMetric;
		m_ptCubeB.x = m_rcCube.right;
		m_ptCubeB.y = m_rcCube.bottom-nMetric;

		CBitmap bmp;
		_CreateCompatibleBitmapImpl(
			bmp,
			dc,
			sizeCube.cx,
			sizeCube.cy
			);
		m_dcCube.CreateCompatibleDC( &dc );
		m_dcCube.SelectObject( &bmp );

		CPalette * pOldPalette = NULL;
		if( pPalette != NULL )
		{
			pOldPalette =
				m_dcCube.SelectPalette( pPalette, FALSE );
			m_dcCube.RealizePalette();
		}

		int nBPP = g_PaintManager->stat_GetBPP();
		int nStep = ( nBPP <= 8 ) ? 4 : 1;

		m_dcCube.FillSolidRect(
			0, 0, sizeCube.cx, sizeCube.cy,
			DEF_RGB_CUBE_TRANSPARENT
			);
		CPoint ptStart(
			sizeCube.cx/2,
			sizeCube.cy/2
			);
		int nHalf = sizeCube.cy/2;
		int nR,nG,nB;
		for( nR = 0; nR <= nHalf; nR++ )
		{
			for( nG = 0; nG <= nHalf; nG++ )
			{
				CPoint pt(ptStart);
				pt.x -= nG+1;
				pt.y += nG/2;
				pt.y -= nR;
				COLORREF clr =
					RGB(
						(nR*255)/nHalf,
						(nG*255)/nHalf,
						DEF_RGB_CUBE_HL
						);
				if( nBPP <= 8 )
				{
					CBrush brush( clr );
					m_dcCube.FillRect(
						CRect(
							CPoint(
								pt.x - nStep/2,
								pt.y - nStep/2
								),
							CSize(nStep,nStep)
							),
						& brush
						);
				}
				else
					m_dcCube.SetPixelV( pt.x, pt.y, clr );
			} // for( nG = 0; nG < nHalf; nG++ )
		} // for( nR = 0; nR < nHalf; nR++ )
		for( nR = 0; nR <= nHalf; nR++ )
		{
			for( nB = 0; nB <= nHalf; nB++ )
			{
				CPoint pt(ptStart);
				pt.x += nB;
				pt.y += nB/2;
				pt.y -= nR;
				COLORREF clr =
					RGB(
						(nR*255)/nHalf,
						DEF_RGB_CUBE_HL,
						(nB*255)/nHalf
						);
				if( nBPP <= 8 )
				{
					CBrush brush( clr );
					m_dcCube.FillRect(
						CRect(
							CPoint(
								pt.x - nStep/2,
								pt.y - nStep/2
								),
							CSize(nStep,nStep)
							),
						& brush
						);
				}
				else
					m_dcCube.SetPixelV( pt.x, pt.y, clr );
			} // for( nB = 0; nB <= nHalf; nB++ )
		} // for( nR = 0; nR < nHalf; nR++ )
		for( nB = 0; nB <= nHalf; nB++ )
		{
			for( nG = 0; nG <= nHalf; nG++ )
			{
				CPoint pt(ptStart);
				pt.x -= nG+1;
				pt.y += nG/2;
				pt.x += nB+1;
				pt.y += nB/2;
				COLORREF clr =
					RGB(
						DEF_RGB_CUBE_HL,
						(nG*255)/nHalf,
						(nB*255)/nHalf
						);
				if( nBPP <= 8 )
				{
					CBrush brush( clr );
					m_dcCube.FillRect(
						CRect(
							CPoint(
								pt.x - nStep/2,
								pt.y - nStep/2
								),
							CSize(nStep,nStep)
							),
						& brush
						);
				}
				else
				{
					m_dcCube.SetPixelV( pt.x, pt.y, clr );
					m_dcCube.SetPixelV( pt.x-1, pt.y, clr );
				}
			} // for( nG = 0; nG <= nHalf; nG++ )
		} // for( nB = 0; nB <= nHalf; nB++ )

		if( pOldPalette != NULL )
			m_dcCube.SelectPalette( pOldPalette, FALSE );

		RecalcLayout();
	} // if needs re-create anything
}

void CExtColorCtrl::rgb_cube_data_t::OnInit(
	CDC & dc,
	CPalette * pPalette,
	CSize sizeTotal,
	bool bOnlySizeChanged
	)
{
	if( m_sizeTotal != sizeTotal )
		ReleaseAll();
	any_picker_data_t::OnInit(
		dc,
		pPalette,
		sizeTotal,
		bOnlySizeChanged
		);
	Generate( dc, pPalette );
	RecalcLayout();
}

void CExtColorCtrl::rgb_cube_data_t::OnDone()
{
	ReleaseAll();
}

void CExtColorCtrl::rgb_cube_data_t::OnDraw(
	CDC & dc,
	CPalette * pPalette
	)
{
CPalette * pOldPalette = NULL;
    if( pPalette != NULL )
    {
        pOldPalette =
			dc.SelectPalette( pPalette, FALSE );
        dc.RealizePalette();
    }

	Generate( dc, pPalette );

CSize sizeCube = m_rcCube.Size();
//	dc.BitBlt(
//		m_rcCube.left,
//		m_rcCube.top,
//		sizeCube.cx,
//		sizeCube.cy,
//		&m_dcCube,
//		0,
//		0,
//		SRCCOPY
//		);
	CExtPaintManager::stat_TransparentBlt(
		dc.GetSafeHdc(),
		m_rcCube.left,
		m_rcCube.top,
		-1,
		-1,
		m_dcCube.GetSafeHdc(),
		0,
		0,
		sizeCube.cx,
		sizeCube.cy,
		DEF_RGB_CUBE_TRANSPARENT
		);

CPen penBlack, penWhite, * pOldPen;
	VERIFY( penBlack.CreateStockObject( BLACK_PEN ) );
	VERIFY( penWhite.CreateStockObject( WHITE_PEN ) );
	pOldPen = dc.SelectObject( &penBlack );
	ASSERT( pOldPen != NULL );

	// draw axes
	dc.MoveTo( m_ptCubeR );
	dc.LineTo( m_ptCubeC );
	dc.LineTo( m_ptCubeB );
	dc.MoveTo( m_ptCubeG );
	dc.LineTo( m_ptCubeC );

	// draw value lines
	pOldPen = dc.SelectObject( &penWhite );
	dc.MoveTo( m_ptPlainRG );
	dc.LineTo( m_ptClrG );
	dc.LineTo( m_ptPlainGB );
	dc.LineTo( m_ptClrB );
	dc.LineTo( m_ptPlainRB );
	dc.LineTo( m_ptClrR );
	dc.LineTo( m_ptPlainRG );
	dc.LineTo( m_ptClrCurr );
	dc.LineTo( m_ptPlainRB );
	dc.MoveTo( m_ptClrCurr );
	dc.LineTo( m_ptPlainGB );

	// draw value boxes
CRect rcDrag;
	rcDrag.SetRect( m_ptClrR, m_ptClrR );
	rcDrag.InflateRect(
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy,
		m_sizeHalfSelBox.cx+1,
		m_sizeHalfSelBox.cy+1
		);
//	dc.Draw3dRect( &rcDrag, RGB(255,255,255), RGB(255,255,255) );
//	rcDrag.InflateRect(1,1);
//	dc.Draw3dRect( &rcDrag, RGB(0,0,0), RGB(0,0,0) );
//	rcDrag.DeflateRect(2,2);
//	dc.Draw3dRect( &rcDrag, RGB(0,0,0), RGB(0,0,0) );
	dc.Draw3dRect( &rcDrag, RGB(0,0,0), RGB(0,0,0) );
	rcDrag.DeflateRect(1,1);
	dc.Draw3dRect( &rcDrag, RGB(255,255,255), RGB(255,255,255) );

	rcDrag.SetRect( m_ptClrG, m_ptClrG );
	rcDrag.InflateRect(
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy,
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy
		);
//	dc.Draw3dRect( &rcDrag, RGB(255,255,255), RGB(255,255,255) );
//	rcDrag.InflateRect(1,1);
//	dc.Draw3dRect( &rcDrag, RGB(0,0,0), RGB(0,0,0) );
//	rcDrag.DeflateRect(2,2);
//	dc.Draw3dRect( &rcDrag, RGB(0,0,0), RGB(0,0,0) );
	dc.Draw3dRect( &rcDrag, RGB(0,0,0), RGB(0,0,0) );
	rcDrag.DeflateRect(1,1);
	dc.Draw3dRect( &rcDrag, RGB(255,255,255), RGB(255,255,255) );

	rcDrag.SetRect( m_ptClrB, m_ptClrB );
	rcDrag.InflateRect(
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy,
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy
		);
//	dc.Draw3dRect( &rcDrag, RGB(255,255,255), RGB(255,255,255) );
//	rcDrag.InflateRect(1,1);
//	dc.Draw3dRect( &rcDrag, RGB(0,0,0), RGB(0,0,0) );
//	rcDrag.DeflateRect(2,2);
//	dc.Draw3dRect( &rcDrag, RGB(0,0,0), RGB(0,0,0) );
	dc.Draw3dRect( &rcDrag, RGB(0,0,0), RGB(0,0,0) );
	rcDrag.DeflateRect(1,1);
	dc.Draw3dRect( &rcDrag, RGB(255,255,255), RGB(255,255,255) );

	dc.SelectObject( pOldPen );

	if( pOldPalette != NULL )
		dc.SelectPalette( pOldPalette, FALSE );
}

void CExtColorCtrl::rgb_cube_data_t::RecalcLayout()
{
CSize sizeCube = m_rcCube.Size();
	if( sizeCube.cx < 1
		|| sizeCube.cy < 1
		)
		return;
	m_ptClrR = m_ptCubeC;
	m_ptClrG = m_ptCubeC;
	m_ptClrB = m_ptCubeC;
	m_ptClrCurr = m_ptCubeC;
int nHalf = sizeCube.cy/2;
	m_ptClrR.y -= (GetRValue(m_clr)*nHalf)/255;
	m_ptClrG.x -= (GetGValue(m_clr)*nHalf)/255;
	m_ptClrG.y += (GetGValue(m_clr)*nHalf)/255/2;
	m_ptClrB.x += (GetBValue(m_clr)*nHalf)/255;
	m_ptClrB.y += (GetBValue(m_clr)*nHalf)/255/2;

	m_ptPlainRG.x = m_ptClrG.x;
	m_ptPlainRG.y = m_ptClrR.y - (m_ptCubeC.y-m_ptClrG.y);
	m_ptPlainRB.x = m_ptClrB.x;
	m_ptPlainRB.y = m_ptClrR.y - (m_ptCubeC.y-m_ptClrB.y);
	m_ptPlainGB.x = m_ptClrG.x - (m_ptCubeC.x-m_ptClrB.x);
	m_ptPlainGB.y = m_ptClrG.y - (m_ptCubeC.y-m_ptClrB.y);

	m_ptClrCurr = m_ptPlainGB;
	m_ptClrCurr.y -= m_ptCubeC.y-m_ptClrR.y;
}

void CExtColorCtrl::rgb_cube_data_t::OnLButtonDown(
	UINT nFlags,
	CPoint point
	)
{
	m_eMoveMode = _MM_NONE;

CRect rcDrag;
	rcDrag.SetRect( m_ptClrR, m_ptClrR );
	rcDrag.InflateRect(
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy,
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy
		);
	if( rcDrag.PtInRect( point ) )
	{
		m_eMoveMode = _MM_RED;
		OnMouseMove(nFlags|MK_LBUTTON,point);
		return;
	}
	rcDrag.SetRect( m_ptClrG, m_ptClrG );
	rcDrag.InflateRect(
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy,
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy
		);
	if( rcDrag.PtInRect( point ) )
	{
		m_eMoveMode = _MM_GREEN;
		OnMouseMove(nFlags|MK_LBUTTON,point);
		return;
	}
	rcDrag.SetRect( m_ptClrB, m_ptClrB );
	rcDrag.InflateRect(
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy,
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy
		);
	if( rcDrag.PtInRect( point ) )
	{
		m_eMoveMode = _MM_BLUE;
		OnMouseMove(nFlags|MK_LBUTTON,point);
		return;
	}

int nBPP = g_PaintManager->stat_GetBPP();
	if( nBPP > 8 )
	{
		CPoint pta[4];
		CRgn rgn;
		int nQuad = m_rcCube.Size().cy/4;

		pta[0] = m_ptCubeG;
		pta[1] = CPoint(m_ptCubeG.x,m_ptCubeR.y+nQuad);
		pta[2] = m_ptCubeR;
		pta[3] = m_ptCubeC;
		rgn.CreatePolygonRgn(pta, 4, ALTERNATE);
		if( rgn.PtInRegion( point ) )
		{
			ASSERT( m_dcCube.GetSafeHdc() != NULL );
			point -= m_rcCube.TopLeft();
			COLORREF clr = m_dcCube.GetPixel( point );
			if( clr == DEF_RGB_CUBE_TRANSPARENT )
				return;
			if( (nFlags & MK_CONTROL) != 0 )
			{
				clr = RGB(
					GetRValue(clr),
					GetGValue(clr),
					GetBValue(m_clr)
					);
			}
			OnSetRGB( clr );
			return;
		}
		
		rgn.DeleteObject();
		pta[0] = m_ptCubeB;
		pta[1] = CPoint(m_ptCubeB.x,m_ptCubeR.y+nQuad);
		pta[2] = m_ptCubeR;
		pta[3] = m_ptCubeC;
		rgn.CreatePolygonRgn(pta, 4, ALTERNATE);
		if( rgn.PtInRegion( point ) )
		{
			ASSERT( m_dcCube.GetSafeHdc() != NULL );
			point -= m_rcCube.TopLeft();
			COLORREF clr = m_dcCube.GetPixel( point );
			if( clr == DEF_RGB_CUBE_TRANSPARENT )
				return;
			if( (nFlags & MK_CONTROL) != 0 )
			{
				clr = RGB(
					GetRValue(clr),
					GetGValue(m_clr),
					GetBValue(clr)
					);
			}
			OnSetRGB( clr );
			return;
		}

		rgn.DeleteObject();
		pta[0] = m_ptCubeG;
		pta[1] = CPoint(m_ptCubeC.x,m_ptCubeG.y+nQuad);
		pta[2] = m_ptCubeB;
		pta[3] = m_ptCubeC;
		rgn.CreatePolygonRgn(pta, 4, ALTERNATE);
		if( rgn.PtInRegion( point ) )
		{
			ASSERT( m_dcCube.GetSafeHdc() != NULL );
			point -= m_rcCube.TopLeft();
			COLORREF clr = m_dcCube.GetPixel( point );
			if( clr == DEF_RGB_CUBE_TRANSPARENT )
				return;
			if( (nFlags & MK_CONTROL) != 0 )
			{
				clr = RGB(
					GetRValue(m_clr),
					GetGValue(clr),
					GetBValue(clr)
					);
			}
			OnSetRGB( clr );
			return;
		}
	} // if( nBPP > 8 )
}

void CExtColorCtrl::rgb_cube_data_t::OnLButtonUp(
	UINT nFlags,
	CPoint point
	)
{
	nFlags;
	point;
	m_eMoveMode = _MM_NONE;
}

void CExtColorCtrl::rgb_cube_data_t::OnMouseMove(
	UINT nFlags,
	CPoint point
	)
{
	nFlags;
	if( m_eMoveMode == _MM_RED )
	{
		if( point.y > m_ptCubeC.y )
			point.y = m_ptCubeC.y;
		if( point.y < m_ptCubeR.y )
			point.y = m_ptCubeR.y;
		int nVal = GetRValue( m_clr );
		int nValNew =
			(	255*
				(point.y-m_ptCubeC.y)
				)
			/ (m_ptCubeR.y-m_ptCubeC.y);
		if( nVal == nValNew )
			return;
		m_clr =
			RGB(
				nValNew,
				GetGValue(m_clr),
				GetBValue(m_clr)
				);
		RecalcLayout();
		// color changed, needs repaint
		return;
	}
	if( m_eMoveMode == _MM_GREEN )
	{
		if( point.x > m_ptCubeC.x )
			point.x = m_ptCubeC.x;
		if( point.x < m_ptCubeG.x )
			point.x = m_ptCubeG.x;
		int nVal = GetGValue( m_clr );
		int nValNew =
			(	255*
				(point.x-m_ptCubeC.x)
				)
			/ (m_ptCubeG.x-m_ptCubeC.x);
		if( nVal == nValNew )
			return;
		m_clr =
			RGB(
				GetRValue(m_clr),
				nValNew,
				GetBValue(m_clr)
				);
		RecalcLayout();
		// color changed, needs repaint
		return;
	}
	if( m_eMoveMode == _MM_BLUE )
	{
		if( point.x < m_ptCubeC.x )
			point.x = m_ptCubeC.x;
		if( point.x > m_ptCubeB.x )
			point.x = m_ptCubeB.x;
		int nVal = GetBValue( m_clr );
		int nValNew =
			(	255*
				(m_ptCubeC.x-point.x)
				)
			/ (m_ptCubeC.x-m_ptCubeB.x);
		if( nVal == nValNew )
			return;
		m_clr =
			RGB(
				GetRValue(m_clr),
				GetGValue(m_clr),
				nValNew
				);
		RecalcLayout();
		// color changed, needs repaint
		return;
	}
}

void CExtColorCtrl::rgb_cube_data_t::OnSetRGB(
	COLORREF clr
	)
{
	any_picker_data_t::OnSetRGB( clr );
	RecalcLayout();
}

void CExtColorCtrl::rgb_cube_data_t::OnSetHLS(
	double hue,
	double luminance,
	double saturation
	)
{
	any_picker_data_t::OnSetHLS(
		hue,
		luminance,
		saturation
		);
	RecalcLayout();
}

/////////////////////////////////////////////////////////////////////////////
// CExtColorCtrl::cmy_mixer_data_t
void CExtColorCtrl::cmy_mixer_data_t::OnInit(
	CDC & dc,
	CPalette * pPalette,
	CSize sizeTotal,
	bool bOnlySizeChanged
	)
{
	any_picker_data_t::OnInit(
		dc,
		pPalette,
		sizeTotal,
		bOnlySizeChanged
		);
//	if( !bOnlySizeChanged )
//	{
//		ASSERT( m_vCells.empty() );
		m_vCells.SetSize( NUM_RGB_FULL_CELLS );
//	}

int nCellWidth = m_sizeTotal.cx/3 - 20;
int nCellCenter =
		m_sizeTotal.cx/2;
int nWorkHeight =
		(m_sizeTotal.cy - 20);
int nWorkY1 = 10; // + nWorkHeight/2;
	int nCellHeight = nWorkHeight/NUM_RGB_SINGLE_CELLS;
	if( nCellHeight < 2 )
		nCellHeight = 2;
	for(int i=0; i < NUM_RGB_SINGLE_CELLS; i++ )
	{
		if( !bOnlySizeChanged )
		{
			m_vCells[i].m_clr =
				CExtPaintManager::stat_CMYtoRGB(
					RGB(i,0,0)
					);
			m_vCells[i].m_bAtFirstType = false;
			m_vCells[i].m_nBorderSize = 0;
			m_vCells[i+NUM_RGB_SINGLE_CELLS].m_clr =
				CExtPaintManager::stat_CMYtoRGB(
					RGB(0,i,0)
					);
			m_vCells[i+NUM_RGB_SINGLE_CELLS].m_bAtFirstType = false;
			m_vCells[i+NUM_RGB_SINGLE_CELLS].m_nBorderSize = 0;
			m_vCells[i+NUM_RGB_SINGLE_CELLS*2].m_clr =
				CExtPaintManager::stat_CMYtoRGB(
					RGB(0,0,i)
					);
			m_vCells[i+NUM_RGB_SINGLE_CELLS*2].m_bAtFirstType = false;
			m_vCells[i+NUM_RGB_SINGLE_CELLS*2].m_nBorderSize = 0;
		}
		CRect rcM;
		rcM.top =
			nWorkY1
			+ (nWorkHeight*i)/NUM_RGB_SINGLE_CELLS;
		rcM.bottom = rcM.top + nCellHeight;
		rcM.left = nCellCenter - nCellWidth/2;
		rcM.right = rcM.left + nCellWidth;
		CRect rcC(rcM),rcY(rcM);
		rcC.OffsetRect(-nCellWidth-20,0);
		rcY.OffsetRect(nCellWidth+20,0);
		m_vCells[i].m_rcPosition = rcC;
		m_vCells[i+NUM_RGB_SINGLE_CELLS].m_rcPosition = rcM;
		m_vCells[i+NUM_RGB_SINGLE_CELLS*2].m_rcPosition = rcY;
	}
}

void CExtColorCtrl::cmy_mixer_data_t::OnDraw(
	CDC & dc,
	CPalette * pPalette
	)
{
CPalette * pOldPalette = NULL;
    if( pPalette != NULL )
    {
        pOldPalette =
			dc.SelectPalette( pPalette, FALSE );
        dc.RealizePalette();
    }

INT it = 0;
	for( ; it < m_vCells.GetSize(); ++it )
		m_vCells[it].OnDraw( dc, pPalette, false );
COLORREF clr = CExtPaintManager::stat_RGBtoCMY( m_clr );
	m_vCells[
			GetRValue(clr)
		].OnDraw( dc, pPalette, true );
	m_vCells[
			GetGValue(clr)+NUM_RGB_SINGLE_CELLS
		].OnDraw( dc, pPalette, true );
	m_vCells[
			GetBValue(clr)+NUM_RGB_SINGLE_CELLS*2
		].OnDraw( dc, pPalette, true );

	if( pOldPalette != NULL )
		dc.SelectPalette( pOldPalette, FALSE );
}

void CExtColorCtrl::cmy_mixer_data_t::OnMouseMove(
	UINT nFlags,
	CPoint point
	)
{
	nFlags;
	if( m_eMoveMode != _MM_CELL_SCAN )
		return;
	for( int i = 0; i < NUM_RGB_FULL_CELLS; i++ )
	{
		if( !m_vCells[i].HitTest(point) )
			continue;
		COLORREF clrOrg =
			CExtPaintManager::stat_RGBtoCMY( m_clr );
		if( i < NUM_RGB_SINGLE_CELLS )
		{
				COLORREF clr =
					RGB(
						i,
						GetGValue( clrOrg ),
						GetBValue( clrOrg )
						);
				m_clr =
					CExtPaintManager::stat_CMYtoRGB( clr );
				return;
		}
		if( i < NUM_RGB_SINGLE_CELLS*2 )
		{
				COLORREF clr =
					RGB(
						GetRValue( clrOrg ),
						i,
						GetBValue( clrOrg )
						);
				m_clr =
					CExtPaintManager::stat_CMYtoRGB( clr );
				return;
		}
				COLORREF clr =
					RGB(
						GetRValue( clrOrg ),
						GetGValue( clrOrg ),
						i
						);
				m_clr =
					CExtPaintManager::stat_CMYtoRGB( clr );
				return;
	} // for( int i = 0; i<NUM_RGB_FULL_CELLS; i++ )
}

/////////////////////////////////////////////////////////////////////////////
// CExtColorCtrl::cmy_cube_data_t

#define DEFAULT_RGB_CUBE_GAP 5
#define DEF_RGB_CUBE_HL 0
#define DEF_RGB_CUBE_TRANSPARENT RGB(255,255,255)

CExtColorCtrl::cmy_cube_data_t::cmy_cube_data_t()
{
	m_nSideGap = DEFAULT_RGB_CUBE_GAP;
	m_sizeHalfSelBox.cx = m_sizeHalfSelBox.cy = 4;
	RecalcLayout();
}

CExtColorCtrl::cmy_cube_data_t::~cmy_cube_data_t()
{
	ReleaseAll();
}

void CExtColorCtrl::cmy_cube_data_t::ReleaseAll()
{
	if( m_dcCube.GetSafeHdc() )
		m_dcCube.DeleteDC();
	m_eMoveMode	= _MM_NONE;
}

void CExtColorCtrl::cmy_cube_data_t::Generate(
	CDC & dc,
	CPalette * pPalette
	)
{
	if( m_dcCube.GetSafeHdc() == NULL )
	{ // if needs re-create anything
		m_rcCube.SetRect(
			0, 0, m_sizeTotal.cx, m_sizeTotal.cy );
		m_rcCube.DeflateRect( m_nSideGap, m_nSideGap );
		
		CSize sizeCube = m_rcCube.Size();
		if( sizeCube.cx > sizeCube.cy )
			m_rcCube.DeflateRect(
				(sizeCube.cx - sizeCube.cy)/2,
				0
				);
		if( sizeCube.cy > sizeCube.cx )
			m_rcCube.DeflateRect(
				0,
				(sizeCube.cy - sizeCube.cx)/2
				);
		sizeCube = m_rcCube.Size();
		
		if( (sizeCube.cx & 0x3) != 0 )
			m_rcCube.right--;
		if( (sizeCube.cx & 0x3) != 0 )
			m_rcCube.right--;
		if( (sizeCube.cx & 0x3) != 0 )
			m_rcCube.right--;

		if( (sizeCube.cy & 0x3) != 0 )
			m_rcCube.bottom--;
		if( (sizeCube.cy & 0x3) != 0 )
			m_rcCube.bottom--;
		if( (sizeCube.cy & 0x3) != 0 )
			m_rcCube.bottom--;
		
		sizeCube = m_rcCube.Size();
		int nMetric = sizeCube.cy/4;
		sizeCube = m_rcCube.Size();
		m_ptCubeC = m_rcCube.CenterPoint();
		m_ptCubeR.x = m_ptCubeC.x;
		m_ptCubeR.y = m_rcCube.top;
		m_ptCubeG.x = m_rcCube.left;
		m_ptCubeG.y = m_rcCube.bottom-nMetric;
		m_ptCubeB.x = m_rcCube.right;
		m_ptCubeB.y = m_rcCube.bottom-nMetric;

		CBitmap bmp;
		_CreateCompatibleBitmapImpl(
			bmp,
			dc,
			sizeCube.cx,
			sizeCube.cy
			);
		m_dcCube.CreateCompatibleDC( &dc );
		m_dcCube.SelectObject( &bmp );

		CPalette * pOldPalette = NULL;
		if( pPalette != NULL )
		{
			pOldPalette =
				m_dcCube.SelectPalette( pPalette, FALSE );
			m_dcCube.RealizePalette();
		}

		int nBPP = g_PaintManager->stat_GetBPP();
		int nStep = ( nBPP <= 8 ) ? 4 : 1;

		m_dcCube.FillSolidRect(
			0, 0, sizeCube.cx, sizeCube.cy,
			DEF_RGB_CUBE_TRANSPARENT
			);
		CPoint ptStart(
			sizeCube.cx/2,
			sizeCube.cy/2
			);
		int nHalf = sizeCube.cy/2;
		int nR,nG,nB;
		for( nR = 0; nR <= nHalf; nR++ )
		{
			for( nG = 0; nG <= nHalf; nG++ )
			{
				CPoint pt(ptStart);
				pt.x -= nG+1;
				pt.y += nG/2;
				pt.y -= nR;
				COLORREF clr =
					CExtPaintManager::stat_CMYtoRGB(
						RGB(
							(nR*255)/nHalf,
							(nG*255)/nHalf,
							DEF_RGB_CUBE_HL
							)
						);
				if( nBPP <= 8 )
				{
					CBrush brush( clr );
					m_dcCube.FillRect(
						CRect(
							CPoint(
								pt.x - nStep/2,
								pt.y - nStep/2
								),
							CSize(nStep,nStep)
							),
						& brush
						);
				}
				else
					m_dcCube.SetPixelV( pt.x, pt.y, clr );
			} // for( nG = 0; nG < nHalf; nG++ )
		} // for( nR = 0; nR < nHalf; nR++ )
		for( nR = 0; nR <= nHalf; nR++ )
		{
			for( nB = 0; nB <= nHalf; nB++ )
			{
				CPoint pt(ptStart);
				pt.x += nB;
				pt.y += nB/2;
				pt.y -= nR;
				COLORREF clr =
					CExtPaintManager::stat_CMYtoRGB(
						RGB(
							(nR*255)/nHalf,
							DEF_RGB_CUBE_HL,
							(nB*255)/nHalf
							)
						);
				if( nBPP <= 8 )
				{
					CBrush brush( clr );
					m_dcCube.FillRect(
						CRect(
							CPoint(
								pt.x - nStep/2,
								pt.y - nStep/2
								),
							CSize(nStep,nStep)
							),
						& brush
						);
				}
				else
					m_dcCube.SetPixelV( pt.x, pt.y, clr );
			} // for( nB = 0; nB <= nHalf; nB++ )
		} // for( nR = 0; nR < nHalf; nR++ )
		for( nB = 0; nB <= nHalf; nB++ )
		{
			for( nG = 0; nG <= nHalf; nG++ )
			{
				CPoint pt(ptStart);
				pt.x -= nG+1;
				pt.y += nG/2;
				pt.x += nB+1;
				pt.y += nB/2;
				COLORREF clr =
					CExtPaintManager::stat_CMYtoRGB(
						RGB(
							DEF_RGB_CUBE_HL,
							(nG*255)/nHalf,
							(nB*255)/nHalf
							)
						);
				if( nBPP <= 8 )
				{
					CBrush brush( clr );
					m_dcCube.FillRect(
						CRect(
							CPoint(
								pt.x - nStep/2,
								pt.y - nStep/2
								),
							CSize(nStep,nStep)
							),
						& brush
						);
				}
				else
				{
					m_dcCube.SetPixelV( pt.x, pt.y, clr );
					m_dcCube.SetPixelV( pt.x-1, pt.y, clr );
				}
			} // for( nG = 0; nG <= nHalf; nG++ )
		} // for( nB = 0; nB <= nHalf; nB++ )

		if( pOldPalette != NULL )
			m_dcCube.SelectPalette( pOldPalette, FALSE );

		RecalcLayout();
	} // if needs re-create anything
}

void CExtColorCtrl::cmy_cube_data_t::OnInit(
	CDC & dc,
	CPalette * pPalette,
	CSize sizeTotal,
	bool bOnlySizeChanged
	)
{
	if( m_sizeTotal != sizeTotal )
		ReleaseAll();
	any_picker_data_t::OnInit(
		dc,
		pPalette,
		sizeTotal,
		bOnlySizeChanged
		);
	Generate( dc, pPalette );
	RecalcLayout();
}

void CExtColorCtrl::cmy_cube_data_t::OnDone()
{
	ReleaseAll();
}

void CExtColorCtrl::cmy_cube_data_t::OnDraw(
	CDC & dc,
	CPalette * pPalette
	)
{
CPalette * pOldPalette = NULL;
    if( pPalette != NULL )
    {
        pOldPalette =
			dc.SelectPalette( pPalette, FALSE );
        dc.RealizePalette();
    }

	Generate( dc, pPalette );

CSize sizeCube = m_rcCube.Size();
//	dc.BitBlt(
//		m_rcCube.left,
//		m_rcCube.top,
//		sizeCube.cx,
//		sizeCube.cy,
//		&m_dcCube,
//		0,
//		0,
//		SRCCOPY
//		);
	CExtPaintManager::stat_TransparentBlt(
		dc.GetSafeHdc(),
		m_rcCube.left,
		m_rcCube.top,
		-1,
		-1,
		m_dcCube.GetSafeHdc(),
		0,
		0,
		sizeCube.cx,
		sizeCube.cy,
		DEF_RGB_CUBE_TRANSPARENT
		);

CPen penBlack, penWhite, * pOldPen;
	VERIFY( penBlack.CreateStockObject( BLACK_PEN ) );
	VERIFY( penWhite.CreateStockObject( WHITE_PEN ) );
	pOldPen = dc.SelectObject( &penBlack );
	ASSERT( pOldPen != NULL );

	// draw axes
	dc.MoveTo( m_ptCubeR );
	dc.LineTo( m_ptCubeC );
	dc.LineTo( m_ptCubeB );
	dc.MoveTo( m_ptCubeG );
	dc.LineTo( m_ptCubeC );

	// draw value lines
	pOldPen = dc.SelectObject( &penWhite );
	dc.MoveTo( m_ptPlainRG );
	dc.LineTo( m_ptClrG );
	dc.LineTo( m_ptPlainGB );
	dc.LineTo( m_ptClrB );
	dc.LineTo( m_ptPlainRB );
	dc.LineTo( m_ptClrR );
	dc.LineTo( m_ptPlainRG );
	dc.LineTo( m_ptClrCurr );
	dc.LineTo( m_ptPlainRB );
	dc.MoveTo( m_ptClrCurr );
	dc.LineTo( m_ptPlainGB );

	// draw value boxes
CRect rcDrag;
	rcDrag.SetRect( m_ptClrR, m_ptClrR );
	rcDrag.InflateRect(
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy,
		m_sizeHalfSelBox.cx+1,
		m_sizeHalfSelBox.cy+1
		);
//	dc.Draw3dRect( &rcDrag, RGB(255,255,255), RGB(255,255,255) );
//	rcDrag.InflateRect(1,1);
//	dc.Draw3dRect( &rcDrag, RGB(0,0,0), RGB(0,0,0) );
//	rcDrag.DeflateRect(2,2);
//	dc.Draw3dRect( &rcDrag, RGB(0,0,0), RGB(0,0,0) );
	dc.Draw3dRect( &rcDrag, RGB(0,0,0), RGB(0,0,0) );
	rcDrag.DeflateRect(1,1);
	dc.Draw3dRect( &rcDrag, RGB(255,255,255), RGB(255,255,255) );

	rcDrag.SetRect( m_ptClrG, m_ptClrG );
	rcDrag.InflateRect(
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy,
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy
		);
//	dc.Draw3dRect( &rcDrag, RGB(255,255,255), RGB(255,255,255) );
//	rcDrag.InflateRect(1,1);
//	dc.Draw3dRect( &rcDrag, RGB(0,0,0), RGB(0,0,0) );
//	rcDrag.DeflateRect(2,2);
//	dc.Draw3dRect( &rcDrag, RGB(0,0,0), RGB(0,0,0) );
	dc.Draw3dRect( &rcDrag, RGB(0,0,0), RGB(0,0,0) );
	rcDrag.DeflateRect(1,1);
	dc.Draw3dRect( &rcDrag, RGB(255,255,255), RGB(255,255,255) );

	rcDrag.SetRect( m_ptClrB, m_ptClrB );
	rcDrag.InflateRect(
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy,
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy
		);
//	dc.Draw3dRect( &rcDrag, RGB(255,255,255), RGB(255,255,255) );
//	rcDrag.InflateRect(1,1);
//	dc.Draw3dRect( &rcDrag, RGB(0,0,0), RGB(0,0,0) );
//	rcDrag.DeflateRect(2,2);
//	dc.Draw3dRect( &rcDrag, RGB(0,0,0), RGB(0,0,0) );
	dc.Draw3dRect( &rcDrag, RGB(0,0,0), RGB(0,0,0) );
	rcDrag.DeflateRect(1,1);
	dc.Draw3dRect( &rcDrag, RGB(255,255,255), RGB(255,255,255) );

	dc.SelectObject( pOldPen );

	if( pOldPalette != NULL )
		dc.SelectPalette( pOldPalette, FALSE );
}

void CExtColorCtrl::cmy_cube_data_t::RecalcLayout()
{
CSize sizeCube = m_rcCube.Size();
	if( sizeCube.cx < 1
		|| sizeCube.cy < 1
		)
		return;
	m_ptClrR = m_ptCubeC;
	m_ptClrG = m_ptCubeC;
	m_ptClrB = m_ptCubeC;
	m_ptClrCurr = m_ptCubeC;
int nHalf = sizeCube.cy/2;
COLORREF clr =
		CExtPaintManager::stat_RGBtoCMY( m_clr );
	m_ptClrR.y -= (GetRValue(clr)*nHalf)/255;
	m_ptClrG.x -= (GetGValue(clr)*nHalf)/255;
	m_ptClrG.y += (GetGValue(clr)*nHalf)/255/2;
	m_ptClrB.x += (GetBValue(clr)*nHalf)/255;
	m_ptClrB.y += (GetBValue(clr)*nHalf)/255/2;

	m_ptPlainRG.x = m_ptClrG.x;
	m_ptPlainRG.y = m_ptClrR.y - (m_ptCubeC.y-m_ptClrG.y);
	m_ptPlainRB.x = m_ptClrB.x;
	m_ptPlainRB.y = m_ptClrR.y - (m_ptCubeC.y-m_ptClrB.y);
	m_ptPlainGB.x = m_ptClrG.x - (m_ptCubeC.x-m_ptClrB.x);
	m_ptPlainGB.y = m_ptClrG.y - (m_ptCubeC.y-m_ptClrB.y);

	m_ptClrCurr = m_ptPlainGB;
	m_ptClrCurr.y -= m_ptCubeC.y-m_ptClrR.y;
}

void CExtColorCtrl::cmy_cube_data_t::OnLButtonDown(
	UINT nFlags,
	CPoint point
	)
{
	m_eMoveMode = _MM_NONE;

CRect rcDrag;
	rcDrag.SetRect( m_ptClrR, m_ptClrR );
	rcDrag.InflateRect(
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy,
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy
		);
	if( rcDrag.PtInRect( point ) )
	{
		m_eMoveMode = _MM_RED;
		OnMouseMove(nFlags|MK_LBUTTON,point);
		return;
	}
	rcDrag.SetRect( m_ptClrG, m_ptClrG );
	rcDrag.InflateRect(
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy,
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy
		);
	if( rcDrag.PtInRect( point ) )
	{
		m_eMoveMode = _MM_GREEN;
		OnMouseMove(nFlags|MK_LBUTTON,point);
		return;
	}
	rcDrag.SetRect( m_ptClrB, m_ptClrB );
	rcDrag.InflateRect(
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy,
		m_sizeHalfSelBox.cx,
		m_sizeHalfSelBox.cy
		);
	if( rcDrag.PtInRect( point ) )
	{
		m_eMoveMode = _MM_BLUE;
		OnMouseMove(nFlags|MK_LBUTTON,point);
		return;
	}

int nBPP = g_PaintManager->stat_GetBPP();
	if( nBPP > 8 )
	{
		CPoint pta[4];
		CRgn rgn;
		int nQuad = m_rcCube.Size().cy/4;

		pta[0] = m_ptCubeG;
		pta[1] = CPoint(m_ptCubeG.x,m_ptCubeR.y+nQuad);
		pta[2] = m_ptCubeR;
		pta[3] = m_ptCubeC;
		rgn.CreatePolygonRgn(pta, 4, ALTERNATE);
		if( rgn.PtInRegion( point ) )
		{
			ASSERT( m_dcCube.GetSafeHdc() != NULL );
			point -= m_rcCube.TopLeft();
			COLORREF clr = m_dcCube.GetPixel( point );
			if( clr == DEF_RGB_CUBE_TRANSPARENT )
				return;
//			COLORREF clr =
//				RGB(
//					((m_ptCubeR.y-point.y)*255)/nHalf,
//					((point.x - m_ptCubeR.x)*255)/nHalf,
//					0
//					);
//			clr = CExtPaintManager::stat_CMYtoRGB( clr );
			if( (nFlags & MK_CONTROL) != 0 )
			{
				clr = RGB(
					GetRValue(clr),
					GetGValue(clr),
					GetBValue(m_clr)
					);
			}
			OnSetRGB( clr );
			return;
		}
		
		rgn.DeleteObject();
		pta[0] = m_ptCubeB;
		pta[1] = CPoint(m_ptCubeB.x,m_ptCubeR.y+nQuad);
		pta[2] = m_ptCubeR;
		pta[3] = m_ptCubeC;
		rgn.CreatePolygonRgn(pta, 4, ALTERNATE);
		if( rgn.PtInRegion( point ) )
		{
			ASSERT( m_dcCube.GetSafeHdc() != NULL );
			point -= m_rcCube.TopLeft();
			COLORREF clr = m_dcCube.GetPixel( point );
			if( clr == DEF_RGB_CUBE_TRANSPARENT )
				return;
			if( (nFlags & MK_CONTROL) != 0 )
			{
				clr = RGB(
					GetRValue(clr),
					GetGValue(m_clr),
					GetBValue(clr)
					);
			}
			OnSetRGB( clr );
			return;
		}

		rgn.DeleteObject();
		pta[0] = m_ptCubeG;
		pta[1] = CPoint(m_ptCubeC.x,m_ptCubeG.y+nQuad);
		pta[2] = m_ptCubeB;
		pta[3] = m_ptCubeC;
		rgn.CreatePolygonRgn(pta, 4, ALTERNATE);
		if( rgn.PtInRegion( point ) )
		{
			ASSERT( m_dcCube.GetSafeHdc() != NULL );
			point -= m_rcCube.TopLeft();
			COLORREF clr = m_dcCube.GetPixel( point );
			if( clr == DEF_RGB_CUBE_TRANSPARENT )
				return;
			if( (nFlags & MK_CONTROL) != 0 )
			{
				clr = RGB(
					GetRValue(m_clr),
					GetGValue(clr),
					GetBValue(clr)
					);
			}
			OnSetRGB( clr );
			return;
		}
	} // if( nBPP > 8 )
}

void CExtColorCtrl::cmy_cube_data_t::OnLButtonUp(
	UINT nFlags,
	CPoint point
	)
{
	nFlags;
	point;
	m_eMoveMode = _MM_NONE;
}

void CExtColorCtrl::cmy_cube_data_t::OnMouseMove(
	UINT nFlags,
	CPoint point
	)
{
	nFlags;
	if( m_eMoveMode == _MM_RED )
	{
		COLORREF clr =
				CExtPaintManager::stat_RGBtoCMY( m_clr );
		if( point.y > m_ptCubeC.y )
			point.y = m_ptCubeC.y;
		if( point.y < m_ptCubeR.y )
			point.y = m_ptCubeR.y;
		int nVal = GetRValue( m_clr );
		int nValNew =
			(	255*
				(point.y-m_ptCubeC.y)
				)
			/ (m_ptCubeR.y-m_ptCubeC.y);
		if( nVal == nValNew )
			return;
		clr =
			RGB(
				nValNew,
				GetGValue(clr),
				GetBValue(clr)
				);
		m_clr =
				CExtPaintManager::stat_CMYtoRGB( clr );
		RecalcLayout();
		// color changed, needs repaint
		return;
	}
	if( m_eMoveMode == _MM_GREEN )
	{
		COLORREF clr =
				CExtPaintManager::stat_RGBtoCMY( m_clr );
		if( point.x > m_ptCubeC.x )
			point.x = m_ptCubeC.x;
		if( point.x < m_ptCubeG.x )
			point.x = m_ptCubeG.x;
		int nVal = GetGValue( clr );
		int nValNew =
			(	255*
				(point.x-m_ptCubeC.x)
				)
			/ (m_ptCubeG.x-m_ptCubeC.x);
		if( nVal == nValNew )
			return;
		clr =
			RGB(
				GetRValue(clr),
				nValNew,
				GetBValue(clr)
				);
		m_clr =
				CExtPaintManager::stat_CMYtoRGB( clr );
		RecalcLayout();
		// color changed, needs repaint
		return;
	}
	if( m_eMoveMode == _MM_BLUE )
	{
		COLORREF clr =
				CExtPaintManager::stat_RGBtoCMY( m_clr );
		if( point.x < m_ptCubeC.x )
			point.x = m_ptCubeC.x;
		if( point.x > m_ptCubeB.x )
			point.x = m_ptCubeB.x;
		int nVal = GetBValue( clr );
		int nValNew =
			(	255*
				(m_ptCubeC.x-point.x)
				)
			/ (m_ptCubeC.x-m_ptCubeB.x);
		if( nVal == nValNew )
			return;
		clr =
			RGB(
				GetRValue(clr),
				GetGValue(clr),
				nValNew
				);
		m_clr =
				CExtPaintManager::stat_CMYtoRGB( clr );
		RecalcLayout();
		// color changed, needs repaint
		return;
	}
}

void CExtColorCtrl::cmy_cube_data_t::OnSetRGB(
	COLORREF clr
	)
{
	any_picker_data_t::OnSetRGB( clr );
	RecalcLayout();
}

void CExtColorCtrl::cmy_cube_data_t::OnSetHLS(
	double hue,
	double luminance,
	double saturation
	)
{
	any_picker_data_t::OnSetHLS(
		hue,
		luminance,
		saturation
		);
	RecalcLayout();
}

/////////////////////////////////////////////////////////////////////////////
// CExtColorCtrl::hsl_roller_data_t

#define DEFAULT_LUMINANCE		0.50f
#define DEFAULT_LUM_BAR_HEIGHT	20

#define DEFAULT_HSL_AREA_GAP	4

#define DEF_HSL_TRANSPARENT RGB(255,255,255)

void CExtColorCtrl::hsl_roller_data_t::GeneratePicker(
	CDC &dc,
	CPalette * pPalette
	)
{
	if( !m_bBmpValidPicker
		&& m_dcPicker.GetSafeHdc()
		)
		m_dcPicker.DeleteDC();
	
	if( m_dcPicker.GetSafeHdc() == NULL )
	{
		if( m_rgnSpaceHueSat.GetSafeHandle() != NULL )
		{
			VERIFY( m_rgnSpaceHueSat.DeleteObject() );
			ASSERT( m_rgnSpaceHueSat.GetSafeHandle() == NULL );
		}
		if( m_rgnMouseSelHueSat.GetSafeHandle() != NULL )
		{
			VERIFY( m_rgnMouseSelHueSat.DeleteObject() );
			ASSERT( m_rgnMouseSelHueSat.GetSafeHandle() == NULL );
		}
		CBitmap bmp;
		_CreateCompatibleBitmapImpl(
			bmp,
			dc,
			m_sizeColorPicker.cx,
			m_sizeColorPicker.cy
			);
		m_dcPicker.CreateCompatibleDC( &dc );
		m_dcPicker.SelectObject( &bmp );

		CPalette * pOldPalette = NULL;
		if( pPalette != NULL )
		{
			pOldPalette =
				m_dcPicker.SelectPalette( pPalette, FALSE );
			m_dcPicker.RealizePalette();
		}

		CRect rcArea(0, 0, m_sizeTotal.cx, m_sizeTotal.cy);
		m_dcPicker.FillSolidRect(
			rcArea, DEF_HSL_TRANSPARENT
			);

		int nBPP = g_PaintManager->stat_GetBPP();

		int nBorder =
			min( m_sizeColorPicker.cx, m_sizeColorPicker.cy );
		nBorder -= m_nAreaGap;

		m_ptCenterSpaceHueSat.x = m_sizeColorPicker.cx / 2;
		m_ptCenterSpaceHueSat.y = m_sizeColorPicker.cy / 2;
		double fLuminance = DEFAULT_LUMINANCE;

		m_nSatTranslation = nBorder/2;
		INT nRad2 = m_nSatTranslation*m_nSatTranslation;
		CRect rcEllipse(
			m_ptCenterSpaceHueSat.x,
			m_ptCenterSpaceHueSat.y,
			m_ptCenterSpaceHueSat.x,
			m_ptCenterSpaceHueSat.y
			);
		rcEllipse.InflateRect(
			m_nSatTranslation,
			m_nSatTranslation
			);
		ASSERT( rcEllipse.left < rcEllipse.right );
		ASSERT( rcEllipse.top < rcEllipse.bottom );
		VERIFY(
			m_rgnSpaceHueSat.CreateEllipticRgn(
				rcEllipse.left,
				rcEllipse.top,
				rcEllipse.right,
				rcEllipse.bottom
				)
			);
		CRect rcEllipseMouseSel( rcEllipse );
		rcEllipseMouseSel.InflateRect(
			__SEL_BOX_DX*3,
			__SEL_BOX_DY*3
			);
		VERIFY(
			m_rgnMouseSelHueSat.CreateEllipticRgn(
				rcEllipseMouseSel.left,
				rcEllipseMouseSel.top,
				rcEllipseMouseSel.right,
				rcEllipseMouseSel.bottom
				)
			);
		bool bResetRgn = false;
		if( nBPP <= 8 )
		{
			bResetRgn = true;
			m_dcPicker.SelectClipRgn( &m_rgnSpaceHueSat );
		} // if( nBPP <= 8 )
		for( INT nPosX = rcEllipse.left; nPosX < rcEllipse.right; nPosX++ )
		{
			INT nPosXoffs = m_ptCenterSpaceHueSat.x - nPosX;
			INT nPosX2 = nPosXoffs*nPosXoffs;
			for( INT nPosY = rcEllipse.top; nPosY < rcEllipse.bottom; nPosY++ )
			{
				INT nPosYoffs = m_ptCenterSpaceHueSat.y - nPosY;
				INT nPosY2 = nPosYoffs*nPosYoffs;
				INT nRadCalc2 = nPosX2 + nPosY2;
				if( nRadCalc2 > nRad2 )
					continue;
				ASSERT( abs(nPosXoffs) <= m_nSatTranslation );
				ASSERT( abs(nPosYoffs) <= m_nSatTranslation );

				if( nBPP <= 8 )
				{
					if( (nPosX%4)!=0 || (nPosY%4)!=0 )
						continue;
				} // if( nBPP <= 8 )

//				INT nRadCalc = INT( ::sqrt(float(nRadCalc2)) );
//				ASSERT( nRadCalc <= m_nSatTranslation );
//				double fSaturation =
//					double(nRadCalc) / double(m_nSatTranslation);
				double fSaturation =
					double(nRadCalc2) / double(nRad2);
				double fHue =
					::atan2(
						double(nPosYoffs), double(nPosXoffs)
						);
				fHue /= 2.0 * PI;
				COLORREF clr =
					CExtPaintManager::stat_HLStoRGB(
						fHue,
						fLuminance,
						fSaturation
						);
				if( nBPP <= 8 )
				{
					CRect _rect( nPosX, nPosY, nPosX, nPosY );
					_rect.InflateRect( 2, 2 );
					CBrush _brush( clr );
					m_dcPicker.FillRect(
						&_rect,
						&_brush
						);
				} // if( nBPP <= 8 )
				else
				{
					m_dcPicker.SetPixel(
						nPosX,
						nPosY,
						clr
						);
				} // else from if( nBPP <= 8 )
			} // for( INT nPosY = rcEllipse.top; nPosY < rcEllipse.bottom; nPosY++ )
		} // for( INT nPosX = rcEllipse.left; nPosX < rcEllipse.right; nPosX++ )
		
		if( pOldPalette != NULL )
			m_dcPicker.SelectPalette( pOldPalette, FALSE );

		if( bResetRgn )
			m_dcPicker.SelectClipRgn( NULL );

		m_bBmpValidPicker = true;
	} // if( m_dcPicker.GetSafeHdc() == NULL )
}

void CExtColorCtrl::hsl_roller_data_t::OnDraw(
	CDC & dc,
	CPalette * pPalette
	)
{
CPalette * pOldPalette = NULL;
    if( pPalette != NULL )
    {
        pOldPalette =
			dc.SelectPalette( pPalette, FALSE );
        dc.RealizePalette();
    }

	GeneratePicker( dc, pPalette );
	GenerateSlider( dc, pPalette );

//	dc.BitBlt(
//		m_nAreaGap,
//		m_nAreaGap,
//		m_sizeColorPicker.cx,
//		m_sizeColorPicker.cy,
//		&m_dcPicker,
//		0,
//		0,
//		SRCCOPY
//		);

	CExtPaintManager::stat_TransparentBlt(
		dc.GetSafeHdc(),
		m_nAreaGap,
		m_nAreaGap,
		-1,
		-1,
		m_dcPicker.GetSafeHdc(),
		0,
		0,
		m_sizeColorPicker.cx,
		m_sizeColorPicker.cy,
		DEF_HSL_TRANSPARENT
		);
	dc.BitBlt(
		m_nAreaGap,
		m_nAreaGap*2 + m_sizeColorPicker.cy,
		m_sizeColorSlider.cx,
		m_sizeTotal.cy,
		&m_dcSlider,
		0,
		0,
		SRCCOPY
		);

	//draw rectangle on hue/sat color space
CSize szHueSat( __SEL_BOX_DX, __SEL_BOX_DY );
double fHueAngle =
		m_fCurrHue * 2.0 * PI;
double fSatRadius =
		sqrt(m_fCurrSaturation) * double(m_nSatTranslation);
		//m_fCurrSaturation * double(m_nSatTranslation);
CPoint ptHueSat(
		INT( ::cos(fHueAngle)*fSatRadius ),
		INT( ::sin(fHueAngle)*fSatRadius )
		);
CPoint ptDrawCalc( m_ptCenterSpaceHueSat - ptHueSat );
	ptDrawCalc += CPoint(m_nAreaGap,m_nAreaGap);
	DrawCrossAt(
		ptDrawCalc,
		dc,
		szHueSat
		);

	//draw rectangle on luminance bar
CSize szLumTracker( __SEL_BOX_DX, m_sizeColorSlider.cy );
CPoint ptLum(
		(int)(m_fCurrLuminance*m_sizeColorSlider.cx),
		m_sizeColorPicker.cy+m_sizeColorSlider.cy/2
		);
	ptDrawCalc =
		ptLum + CPoint(m_nAreaGap,m_nAreaGap*2);
	DrawCrossAt(
		ptDrawCalc,
		dc,
		szLumTracker
		);

	if( pOldPalette != NULL )
		dc.SelectPalette( pOldPalette, FALSE );
}

void CExtColorCtrl::hsl_roller_data_t::OnLButtonDown(
	UINT nFlags,
	CPoint point
	) 
{
//COLORREF ref = m_dcPicker.GetPixel( point );
	if( m_bBmpValidPicker )
	{
		m_eMoveMode = _MM_NONE;
		ASSERT( m_rgnSpaceHueSat.GetSafeHandle() != NULL );
		ASSERT( m_rgnMouseSelHueSat.GetSafeHandle() != NULL );
		if( m_rgnMouseSelHueSat.PtInRegion(point) )
		{
			// change hue/sat
			m_eMoveMode = _MM_PICKER;
		} // if( m_rgnMouseSelHueSat.PtInRegion(point) )
		else if( point.y - m_nAreaGap*2 > m_sizeColorPicker.cy )
		{
			// change luminance
			m_eMoveMode = _MM_SLIDER;
		} // else from if( m_rgnMouseSelHueSat.PtInRegion(point) )
		OnMouseMove(nFlags|MK_LBUTTON,point);
///		OnMouseMove(nFlags|MK_LBUTTON,point); // for double precission (after recalculations)
		// ... color chaged ... needs repaint ...
	}
}

void CExtColorCtrl::hsl_roller_data_t::OnLButtonUp(
	UINT nFlags,
	CPoint point
	) 
{
	nFlags;
	point;
	m_eMoveMode = _MM_NONE;
}

void CExtColorCtrl::hsl_roller_data_t::OnMouseMove(
	UINT nFlags,
	CPoint point
	) 
{
	if( m_bBmpValidPicker && (nFlags & MK_LBUTTON) )
	{
		if( m_eMoveMode == _MM_SLIDER )
		{
			point.x -= m_nAreaGap;
			if( point.x < 0 )
				point.x = 0;
			point.y -= m_nAreaGap*2;
			if( point.y < 0 )
				point.y = 0;
			// change luminance
			if( point.x > m_sizeColorSlider.cx )
				point.x = m_sizeColorSlider.cx;
			m_fCurrLuminance =
				((double)point.x) /
					((double)m_sizeColorSlider.cx);
			m_clr =
				CExtPaintManager::stat_HLStoRGB(
					m_fCurrHue,
					m_fCurrLuminance,
					m_fCurrSaturation
					);
		}
		else if(m_eMoveMode == _MM_PICKER)
		{
			ASSERT( m_rgnSpaceHueSat.GetSafeHandle() != NULL );
			ASSERT( m_rgnMouseSelHueSat.GetSafeHandle() != NULL );
			if( m_rgnMouseSelHueSat.PtInRegion(point) )
			{
				CPoint ptOffsHueSat(
					m_ptCenterSpaceHueSat.x - point.x,
					m_ptCenterSpaceHueSat.y - point.y
					);
				double fHueAngle =
					::atan2(
						double(ptOffsHueSat.y), double(ptOffsHueSat.x)
						);
				double fHue = fHueAngle / (2.0 * PI);
				double fSaturation = 1.0;
				double fLuminance = m_fCurrLuminance;

				INT nRadCalc2 =
					ptOffsHueSat.x*ptOffsHueSat.x
					+ ptOffsHueSat.y*ptOffsHueSat.y;
				INT nRadCalc = INT( ::sqrt(float(nRadCalc2)) );
				
				if( nRadCalc > m_nSatTranslation )
				{
					ptOffsHueSat.x =
						INT(
							double(m_nSatTranslation)
							* ::cos( fHueAngle )
							);
					ptOffsHueSat.y =
						INT(
							double(m_nSatTranslation)
							* ::sin( fHueAngle )
							);
					nRadCalc2 =
						ptOffsHueSat.x*ptOffsHueSat.x
						+ ptOffsHueSat.y*ptOffsHueSat.y;
					ASSERT( ptOffsHueSat.x <= m_nSatTranslation );
					ASSERT( ptOffsHueSat.y <= m_nSatTranslation );
					ASSERT( fSaturation == 1.0 );
				} // if( nRadCalc > m_nSatTranslation )
				else
				{
					ASSERT( ptOffsHueSat.x <= m_nSatTranslation );
					ASSERT( ptOffsHueSat.y <= m_nSatTranslation );
//					INT nRadCalc = INT( ::sqrt(float(nRadCalc2)) );
//					ASSERT( nRadCalc <= m_nSatTranslation );
//					double fSaturation =
//						double(nRadCalc) / double(m_nSatTranslation);
					fSaturation =
						double(nRadCalc2) / double(m_nSatTranslation*m_nSatTranslation);
				} // else from if( nRadCalc > m_nSatTranslation )


				OnSetHLS(
					fHue,
					m_fCurrLuminance,
					fSaturation
					);
				OnSetRGB(
					CExtPaintManager::stat_HLStoRGB(
						fHue,
						m_fCurrLuminance,
						fSaturation
						)
					);
				m_fCurrLuminance = fLuminance;

			} // if( m_rgnMouseSelHueSat.PtInRegion(point) )

		}
		// ... color chaged ... needs repaint ...
	}
}

void CExtColorCtrl::hsl_roller_data_t::OnSetRGB(
	COLORREF clr
	)
{
	any_picker_data_t::OnSetRGB( clr );
	m_bBmpValidSlider = false; 
}

void CExtColorCtrl::hsl_roller_data_t::OnSetHLS(
	double hue,
	double luminance,
	double saturation
	)
{
	any_picker_data_t::OnSetHLS(
		hue,
		luminance,
		saturation
		);
	m_bBmpValidSlider = false; 
}

/////////////////////////////////////////////////////////////////////////////
// CExtColorCtrl::hsl_panel_data_t

CExtColorCtrl::hsl_panel_data_t::hsl_panel_data_t()
{
	m_bBmpValidSlider	= false;
	m_bBmpValidPicker		= false;
	m_sizeColorPicker.cx	= 10;
	m_sizeColorPicker.cy	= 10;
	m_sizeColorSlider.cx	= 10;
	m_sizeColorSlider.cy	= 10;
	m_nLumBarDy	= DEFAULT_LUM_BAR_HEIGHT;
	m_nAreaGap	= DEFAULT_HSL_AREA_GAP;
};

CExtColorCtrl::hsl_panel_data_t::~hsl_panel_data_t()
{
	ReleaseAll();
};

void CExtColorCtrl::hsl_panel_data_t::ReleaseAll()
{
	if( m_dcPicker.GetSafeHdc() )
		m_dcPicker.DeleteDC();
	if( m_dcSlider.GetSafeHdc() )
		m_dcSlider.DeleteDC();
	m_bBmpValidPicker = false;
	m_bBmpValidSlider = false;
	m_eMoveMode = _MM_NONE;
}

void CExtColorCtrl::hsl_panel_data_t::GenerateSlider(
	CDC &dc,
	CPalette * pPalette
	)
{
	if( !m_bBmpValidSlider
		&& m_dcSlider.GetSafeHdc()
		)
		m_dcSlider.DeleteDC();
	if( m_dcSlider.GetSafeHdc() == NULL )
	{
		CBitmap bmp;
		_CreateCompatibleBitmapImpl(
			bmp,
			dc,
			m_sizeColorSlider.cx,
			m_sizeColorSlider.cy
			);
		m_dcSlider.CreateCompatibleDC( &dc );
		m_dcSlider.SelectObject( &bmp );
		
		CPalette * pOldPalette = NULL;
		if( pPalette != NULL )
		{
			pOldPalette =
				m_dcSlider.SelectPalette( pPalette, FALSE );
			m_dcSlider.RealizePalette();
		}

		int nBPP = g_PaintManager->stat_GetBPP();
		int nStep = ( nBPP <= 8 ) ? 4 : 1;

		for( int j = m_sizeColorSlider.cx; j > 0; j-=nStep )
		{
			double fLuminance =
				double(j-1) /
					double(m_sizeColorSlider.cx);
			COLORREF clr =
				CExtPaintManager::stat_HLStoRGB(
					m_fCurrHue,
					fLuminance,
					m_fCurrSaturation
					);
			if( nBPP <= 8 )
			{
				CBrush brush( clr );
				m_dcSlider.FillRect(
					CRect(
						CPoint(j-nStep,0),
						CSize(nStep,m_sizeColorSlider.cy)
						),
					& brush
					);
			}
			else
				m_dcSlider.FillSolidRect(
					j-1,
					0,
					1,
					m_sizeColorSlider.cy,
					clr
					);
		}

		if( pOldPalette != NULL )
			m_dcSlider.SelectPalette( pOldPalette, FALSE );

		m_bBmpValidSlider = true;
	}
}

void CExtColorCtrl::hsl_panel_data_t::GeneratePicker(
	CDC &dc,
	CPalette * pPalette
	)
{
	if( !m_bBmpValidPicker
		&& m_dcPicker.GetSafeHdc()
		)
		m_dcPicker.DeleteDC();
	if( m_dcPicker.GetSafeHdc() == NULL )
	{
		CBitmap bmp;
		_CreateCompatibleBitmapImpl(
			bmp,
			dc,
			m_sizeColorPicker.cx,
			m_sizeColorPicker.cy
			);
		m_dcPicker.CreateCompatibleDC( &dc );
		m_dcPicker.SelectObject( &bmp );
		
		CPalette * pOldPalette = NULL;
		if( pPalette != NULL )
		{
			pOldPalette =
				m_dcPicker.SelectPalette( pPalette, FALSE );
			m_dcPicker.RealizePalette();
		}

		int nBPP = g_PaintManager->stat_GetBPP();
		int nStep = ( nBPP <= 8 ) ? 4 : 1;

		for( int i = 0; i < m_sizeColorPicker.cx; i++ )
		{
			for( int j = 0; j < m_sizeColorPicker.cy; j++ )
			{
				COLORREF clr =
					CExtPaintManager::stat_HLStoRGB(
						double(i) /
							double(m_sizeColorPicker.cx),
						DEFAULT_LUMINANCE,
						double(m_sizeColorPicker.cy - j - 1) /
							double(m_sizeColorPicker.cy)
						);
				if( nBPP <= 8 )
				{
					CBrush brush( clr );
					m_dcPicker.FillRect(
						CRect(
							CPoint(
								i - nStep/2,
								j - nStep/2
								),
							CSize(nStep,nStep)
							),
						& brush
						);
				}
				else
					m_dcPicker.SetPixelV(i,j,clr);
			}
		}

		if( pOldPalette != NULL )
			m_dcPicker.SelectPalette( pOldPalette, FALSE );

		m_bBmpValidPicker = true;
	}	
}

void CExtColorCtrl::hsl_panel_data_t::OnInit(
	CDC & dc,
	CPalette * pPalette,
	CSize sizeTotal,
	bool bOnlySizeChanged
	)
{
bool bRegen = false;
	if( m_sizeTotal != sizeTotal )
	{
		ReleaseAll();
		bRegen = true;
	}
	any_picker_data_t::OnInit(
		dc,
		pPalette,
		sizeTotal,
		bOnlySizeChanged
		);

	if( bRegen )
	{
		int maxi = m_sizeTotal.cx - m_nAreaGap*2;
		int maxj = m_sizeTotal.cy - m_nAreaGap*3;
		m_sizeColorPicker.cx = maxi;
		m_sizeColorPicker.cy = maxj - m_nLumBarDy;
		m_sizeColorSlider.cx = maxi;
		m_sizeColorSlider.cy = m_nLumBarDy;
		m_sizeTotal.cx = maxi;
		m_sizeTotal.cy = maxj;

		GeneratePicker( dc, pPalette );
		GenerateSlider( dc, pPalette );
	}
}

void CExtColorCtrl::hsl_panel_data_t::OnDone()
{
	ReleaseAll();
}

void CExtColorCtrl::hsl_panel_data_t::OnDraw(
	CDC & dc,
	CPalette * pPalette
	)
{
CPalette * pOldPalette = NULL;
    if( pPalette != NULL )
    {
        pOldPalette =
			dc.SelectPalette( pPalette, FALSE );
        dc.RealizePalette();
    }

	GeneratePicker( dc, pPalette );
	GenerateSlider( dc, pPalette );

	dc.BitBlt(
		m_nAreaGap,
		m_nAreaGap,
		m_sizeColorPicker.cx,
		m_sizeColorPicker.cy,
		&m_dcPicker,
		0,
		0,
		SRCCOPY
		);
	dc.BitBlt(
		m_nAreaGap,
		m_nAreaGap*2 + m_sizeColorPicker.cy,
		m_sizeColorSlider.cx,
		m_sizeTotal.cy,
		&m_dcSlider,
		0,
		0,
		SRCCOPY
		);

SIZE sz = { __SEL_BOX_DX, __SEL_BOX_DY };
CPoint ptDrawCalc = m_ptMousePos + CPoint(m_nAreaGap,m_nAreaGap);
	DrawCrossAt(
		ptDrawCalc,
		dc,
		sz
		); // draw a rectangle on picker
	
	sz.cx = __SEL_BOX_DX;
	sz.cy = m_sizeColorSlider.cy;
CPoint pt(	(int)(m_fCurrLuminance*m_sizeColorSlider.cx),
			m_sizeColorPicker.cy+m_sizeColorSlider.cy/2
			);
	ptDrawCalc = pt + CPoint(m_nAreaGap,m_nAreaGap*2);
	DrawCrossAt(
		ptDrawCalc,
		dc,
		sz
		); //draw rectangle on luminance bar

	if( pOldPalette != NULL )
		dc.SelectPalette( pOldPalette, FALSE );
}

void CExtColorCtrl::hsl_panel_data_t::DrawCrossAt(
	CPoint & point,
	CDC & dc,
	SIZE & sz
	)
{
CRect rc( point, sz );	
	rc.OffsetRect( -sz.cx/2, -sz.cy/2 );
	dc.Draw3dRect(
		&rc,
		RGB(255,255,255),
		RGB(255,255,255)
		);
	rc.InflateRect( 1, 1 );
	dc.Draw3dRect(
		&rc,
		RGB(0,0,0),
		RGB(0,0,0)
		);
}

// compute conversion to HSL
// place the rectangle on the nearest color
void CExtColorCtrl::hsl_panel_data_t::OnSetRGB( COLORREF clr )
{
	any_picker_data_t::OnSetRGB( clr );

	m_ptMousePos.x =
		long(
			double(m_sizeColorPicker.cx) *
				m_fCurrHue
			);
	m_ptMousePos.y =
		long(
			double(m_sizeColorPicker.cy) *
				m_fCurrSaturation
			);
	m_ptMousePos.y =
		m_sizeColorPicker.cy - m_ptMousePos.y - 1;
	// regenerate luminance bar
	m_bBmpValidSlider = false; 
	// ... color chaged ... needs repaint ...
}

void CExtColorCtrl::hsl_panel_data_t::OnSetHLS(
	double hue,
	double luminance,
	double saturation
	)
{
	any_picker_data_t::OnSetHLS(
		hue,
		luminance,
		saturation
		);
	
	m_ptMousePos.x =
		long(
			double(m_sizeColorPicker.cx) *
				m_fCurrHue
			);	
	m_ptMousePos.y =
		long(
			double(m_sizeColorPicker.cy) *
				m_fCurrSaturation
			);	
	m_ptMousePos.y =
		m_sizeColorPicker.cy - m_ptMousePos.y - 1;
	// regenerate luminance bar
	m_bBmpValidSlider = false;
	// ... color chaged ... needs repaint ...
}

void CExtColorCtrl::hsl_panel_data_t::OnLButtonDown(
	UINT nFlags,
	CPoint point
	) 
{
//COLORREF ref = m_dcPicker.GetPixel( point );
	if( m_bBmpValidPicker )
	{
		// change color
		m_eMoveMode = _MM_PICKER;
		if( point.y - m_nAreaGap*2 > m_sizeColorPicker.cy )
		{
			// change luminance
			m_eMoveMode = _MM_SLIDER;
		}
		OnMouseMove(nFlags|MK_LBUTTON,point);
		OnMouseMove(nFlags|MK_LBUTTON,point); // for double precission (after recalculations)
		// ... color chaged ... needs repaint ...
	}
}

void CExtColorCtrl::hsl_panel_data_t::OnLButtonUp(
	UINT nFlags,
	CPoint point
	) 
{
	nFlags;
	point;
	m_eMoveMode = _MM_NONE;
}

void CExtColorCtrl::hsl_panel_data_t::OnMouseMove(
	UINT nFlags,
	CPoint point
	) 
{
	if( m_bBmpValidPicker && (nFlags & MK_LBUTTON) )
	{
		if( m_eMoveMode == _MM_SLIDER )
		{
			point.x -= m_nAreaGap;
			if( point.x < 0 )
				point.x = 0;
			point.y -= m_nAreaGap*2;
			if( point.y < 0 )
				point.y = 0;
			// change luminance
			if( point.x > m_sizeColorSlider.cx )
				point.x = m_sizeColorSlider.cx;
			m_fCurrLuminance =
				((double)point.x) /
					((double)m_sizeColorSlider.cx);
			m_clr =
				CExtPaintManager::stat_HLStoRGB(
					m_fCurrHue,
					m_fCurrLuminance,
					m_fCurrSaturation
					);
		}
		else if(m_eMoveMode == _MM_PICKER)
		{
			point.x -= m_nAreaGap;
			if( point.x < 0 )
				point.x = 0;
			point.y -= m_nAreaGap;
			if( point.y < 0 )
				point.y = 0;
			
			if( point.x > m_sizeColorPicker.cx )
				point.x = m_sizeColorPicker.cx;
			if( point.y > m_sizeColorPicker.cy )
				point.y = m_sizeColorPicker.cy;
			
			if( nFlags & MK_CONTROL )
				point.x = m_ptMousePos.x;
			
			if( nFlags & MK_SHIFT )
				point.y = m_ptMousePos.y;

			m_fCurrSaturation =
				double(m_sizeColorPicker.cy - point.y) /
					double(m_sizeColorPicker.cy);
			m_fCurrHue =
				double(point.x) /
					double(m_sizeColorPicker.cx);
			m_clr =
				CExtPaintManager::stat_HLStoRGB(
					m_fCurrHue,
					m_fCurrLuminance,
					m_fCurrSaturation
					);
			m_ptMousePos = point;
			// regenerate luminance bar
			m_bBmpValidSlider = false; 
		}
		// ... color chaged ... needs repaint ...
	}
}

/////////////////////////////////////////////////////////////////////////////
// CExtColorCtrl::hsl_basic_hue_data_t

// compute conversion to HSL
// place the rectangle on the nearest color
void CExtColorCtrl::hsl_basic_hue_data_t::OnSetRGB( COLORREF clr )
{
	any_picker_data_t::OnSetRGB( clr );
	// regenerate picker
	m_bBmpValidPicker = false; 
	// ... color chaged ... needs repaint ...
}

void CExtColorCtrl::hsl_basic_hue_data_t::OnSetHLS(
	double hue,
	double luminance,
	double saturation
	)
{
	any_picker_data_t::OnSetHLS(
		hue,
		luminance,
		saturation
		);
	// regenerate picker
	m_bBmpValidPicker = false; 
	// ... color chaged ... needs repaint ...
}

void CExtColorCtrl::hsl_basic_hue_data_t::GenerateSlider(
	CDC &dc,
	CPalette * pPalette
	)
{
	if( !m_bBmpValidSlider
		&& m_dcSlider.GetSafeHdc()
		)
		m_dcSlider.DeleteDC();
	if( m_dcSlider.GetSafeHdc() == NULL )
	{
		CBitmap bmp;
		_CreateCompatibleBitmapImpl(
			bmp,
			dc,
			m_sizeColorSlider.cx,
			m_sizeColorSlider.cy
			);
		m_dcSlider.CreateCompatibleDC( &dc );
		m_dcSlider.SelectObject( &bmp );
		
		CPalette * pOldPalette = NULL;
		if( pPalette != NULL )
		{
			pOldPalette =
				m_dcSlider.SelectPalette( pPalette, FALSE );
			m_dcSlider.RealizePalette();
		}

		int nBPP = g_PaintManager->stat_GetBPP();
		int nStep = ( nBPP <= 8 ) ? 4 : 1;

		for( int j = 0; j < m_sizeColorSlider.cx; j+=nStep )
		{
			double fHue =
				double(j) /
					double(m_sizeColorSlider.cx);
			COLORREF clr =
				CExtPaintManager::stat_HLStoRGB(
					fHue,
					DEFAULT_LUMINANCE,
					1.0
					);
			if( nBPP <= 8 )
			{
				CBrush brush( clr );
				m_dcSlider.FillRect(
					CRect(
						CPoint(j,0),
						CSize(nStep,m_sizeColorSlider.cy)
						),
					& brush
					);
			}
			else
				m_dcSlider.FillSolidRect(
					j,
					0,
					1,
					m_sizeColorSlider.cy,
					clr
					);
		}

		if( pOldPalette != NULL )
			m_dcSlider.SelectPalette( pOldPalette, FALSE );

		m_bBmpValidSlider = true;
	}
}

void CExtColorCtrl::hsl_basic_hue_data_t::GeneratePicker(
	CDC &dc,
	CPalette * pPalette
	)
{
	if( !m_bBmpValidPicker
		&& m_dcPicker.GetSafeHdc()
		)
		m_dcPicker.DeleteDC();
	if( m_dcPicker.GetSafeHdc() == NULL )
	{
		CBitmap bmp;
		_CreateCompatibleBitmapImpl(
			bmp,
			dc,
			m_sizeColorPicker.cx,
			m_sizeColorPicker.cy
			);
		m_dcPicker.CreateCompatibleDC( &dc );
		m_dcPicker.SelectObject( &bmp );
		
		CPalette * pOldPalette = NULL;
		if( pPalette != NULL )
		{
			pOldPalette =
				m_dcPicker.SelectPalette( pPalette, FALSE );
			m_dcPicker.RealizePalette();
		}

		int nBPP = g_PaintManager->stat_GetBPP();
		int nStep = ( nBPP <= 8 ) ? 4 : 1;

		for( int i = 0; i < m_sizeColorPicker.cx; i+=nStep )
		{
			for( int j = 0; j < m_sizeColorPicker.cy; j+=nStep )
			{
				COLORREF clr =
					CExtPaintManager::stat_HLStoRGB(
						m_fCurrHue,
						double(m_sizeColorPicker.cy - j - 1) /
							double(m_sizeColorPicker.cy),
						double(i) /
							double(m_sizeColorPicker.cx)
						);
				if( nBPP <= 8 )
				{
					CBrush brush( clr );
					m_dcPicker.FillRect(
						CRect(
							CPoint(
								i - nStep/2,
								j - nStep/2
								),
							CSize(nStep,nStep)
							),
						& brush
						);
				}
				else
					m_dcPicker.SetPixelV(i,j,clr);
			}
		}

		if( pOldPalette != NULL )
			m_dcPicker.SelectPalette( pOldPalette, FALSE );

		m_bBmpValidPicker = true;
	}	
}

void CExtColorCtrl::hsl_basic_hue_data_t::OnDraw(
	CDC & dc,
	CPalette * pPalette
	)
{
CPalette * pOldPalette = NULL;
    if( pPalette != NULL )
    {
        pOldPalette =
			dc.SelectPalette( pPalette, FALSE );
        dc.RealizePalette();
    }

	GeneratePicker( dc, pPalette );
	GenerateSlider( dc, pPalette );

	dc.BitBlt(
		m_nAreaGap,
		m_nAreaGap,
		m_sizeColorPicker.cx,
		m_sizeColorPicker.cy,
		&m_dcPicker,
		0,
		0,
		SRCCOPY
		);
	dc.BitBlt(
		m_nAreaGap,
		m_nAreaGap*2 + m_sizeColorPicker.cy,
		m_sizeColorSlider.cx,
		m_sizeTotal.cy,
		&m_dcSlider,
		0,
		0,
		SRCCOPY
		);

CPoint ptValidLS;
	ptValidLS.x = 
		int(		m_fCurrSaturation *
					double(m_sizeColorPicker.cx)
				)
		;
	ptValidLS.y = 
		m_sizeColorPicker.cy -
			int(	m_fCurrLuminance *
					double(m_sizeColorPicker.cy)
					)
		;

SIZE sz = { __SEL_BOX_DX, __SEL_BOX_DY };
CPoint ptDrawCalc = 
		ptValidLS + CPoint(m_nAreaGap,m_nAreaGap);
	DrawCrossAt(
		ptDrawCalc,
		dc,
		sz
		); // draw a rectangle on picker
	
	sz.cx = __SEL_BOX_DX;
	sz.cy = m_sizeColorSlider.cy;
CPoint pt(	(int)(m_fCurrHue*m_sizeColorSlider.cx),
			m_sizeColorPicker.cy+m_sizeColorSlider.cy/2
			);
	ptDrawCalc = pt + CPoint(m_nAreaGap,m_nAreaGap*2);
	DrawCrossAt(
		ptDrawCalc,
		dc,
		sz
		); //draw rectangle on luminance bar

	if( pOldPalette != NULL )
		dc.SelectPalette( pOldPalette, FALSE );
}

void CExtColorCtrl::hsl_basic_hue_data_t::OnMouseMove(
	UINT nFlags,
	CPoint point
	) 
{
	if( m_bBmpValidSlider && (nFlags & MK_LBUTTON) )
	{
		if( m_eMoveMode == _MM_SLIDER )
		{
			point.x -= m_nAreaGap;
			if( point.x < 0 )
				point.x = 0;
			point.y -= m_nAreaGap*2;
			if( point.y < 0 )
				point.y = 0;
			// change luminance
			if( point.x > m_sizeColorSlider.cx )
				point.x = m_sizeColorSlider.cx;
			m_fCurrHue =
				((double)point.x) /
					((double)m_sizeColorSlider.cx);
			m_clr =
				CExtPaintManager::stat_HLStoRGB(
					m_fCurrHue,
					m_fCurrLuminance,
					m_fCurrSaturation
					);
			// regenerate picker bar
			m_bBmpValidPicker = false; 
		}
		else if(m_eMoveMode == _MM_PICKER)
		{
			point.x -= m_nAreaGap;
			if( point.x < 0 )
				point.x = 0;
			point.y -= m_nAreaGap;
			if( point.y < 0 )
				point.y = 0;
			
			if( point.x > m_sizeColorPicker.cx )
				point.x = m_sizeColorPicker.cx;
			if( point.y > m_sizeColorPicker.cy )
				point.y = m_sizeColorPicker.cy;
			
			if( nFlags & MK_CONTROL )
				point.x = m_ptMousePos.x;
			
			if( nFlags & MK_SHIFT )
				point.y = m_ptMousePos.y;

			m_fCurrLuminance =
				double(m_sizeColorPicker.cy - point.y) /
					double(m_sizeColorPicker.cy);
			m_fCurrSaturation =
				double(point.x) /
					double(m_sizeColorPicker.cx);
			m_clr =
				CExtPaintManager::stat_HLStoRGB(
					m_fCurrHue,
					m_fCurrLuminance,
					m_fCurrSaturation
					);
			m_ptMousePos = point;
		}
		// ... color chaged ... needs repaint ...
	}
}

/////////////////////////////////////////////////////////////////////////////
// CExtColorCtrl::xyz_plain_data_t

void CExtColorCtrl::xyz_plain_data_t::OnSetRGB(
	COLORREF clr
	)
{
	any_picker_data_t::OnSetRGB( clr );
	// regenerate picker
	m_bBmpValidPicker = false;
}

void CExtColorCtrl::xyz_plain_data_t::OnSetHLS(
	double hue,
	double luminance,
	double saturation
	)
{
	any_picker_data_t::OnSetHLS( hue, luminance, saturation );
	// regenerate picker
	m_bBmpValidPicker = false;
}

COLORREF CExtColorCtrl::xyz_plain_data_t::CalcColorPicker(
	int nX, int nY
	)
{
	ASSERT( nX >= 0 && nX <= m_sizeColorPicker.cx );
	ASSERT( nY >= 0 && nY <= m_sizeColorPicker.cy );
	ASSERT(
		MXYZ_MIN <= m_eXYZmode
		&& m_eXYZmode <= MXYZ_MAX
		);

	nX = (nX * 255) / m_sizeColorPicker.cx;
	nY = (nY * 255) / m_sizeColorPicker.cy;
	if( nX > 255 )
		nX = 255;
	if( nY > 255 )
		nY = 255;

	switch( m_eXYZmode )
	{
	case MXYZ_R:
		return
			RGB(
				GetRValue(m_clr),
				nX,
				nY
				);
	case MXYZ_G:
		return
			RGB(
				nX,
				GetGValue(m_clr),
				nY
				);
	case MXYZ_B:
		return
			RGB(
				nX,
				nY,
				GetBValue(m_clr)
				);
	case MXYZ_C:
	{
		COLORREF clr = 
			CExtPaintManager::stat_RGBtoCMY( m_clr );
		return
			CExtPaintManager::stat_CMYtoRGB(
				RGB(
					GetRValue(clr),
					nX,
					nY
					)
				);
	}
	case MXYZ_M:
	{
		COLORREF clr = 
			CExtPaintManager::stat_RGBtoCMY( m_clr );
		return
			CExtPaintManager::stat_CMYtoRGB(
				RGB(
					nX,
					GetGValue(clr),
					nY
					)
				);
	}
	case MXYZ_Y:
	{
		COLORREF clr = 
			CExtPaintManager::stat_RGBtoCMY( m_clr );
		return
			CExtPaintManager::stat_CMYtoRGB(
				RGB(
					nX,
					nY,
					GetBValue(clr)
					)
				);
	}
#ifdef _DEBUG
	default:
		ASSERT( FALSE );
		break;
#endif // _DEBUG
	}; // switch( m_eXYZmode )
	
	ASSERT( FALSE );
	return RGB( 0, 0, 0 );
}

COLORREF CExtColorCtrl::xyz_plain_data_t::CalcColorSlider(
	int nZ
	)
{
	ASSERT( nZ >= 0 && nZ <= m_sizeColorSlider.cx );
	ASSERT(
		MXYZ_MIN <= m_eXYZmode
		&& m_eXYZmode <= MXYZ_MAX
		);

	nZ = (nZ * 255) / m_sizeColorSlider.cx;
	if( nZ > 255 )
		nZ = 255;

	switch( m_eXYZmode )
	{
	case MXYZ_R:
		return
			RGB(
				nZ,
				0,
				0
				);
	case MXYZ_G:
		return
			RGB(
				0,
				nZ,
				0
				);
	case MXYZ_B:
		return
			RGB(
				0,
				0,
				nZ
				);
	case MXYZ_C:
	{
		//COLORREF clr = 
		//	CExtPaintManager::stat_RGBtoCMY( m_clr );
		return
			CExtPaintManager::stat_CMYtoRGB(
				RGB(
					nZ,
					0,
					0
					)
				);
	}
	case MXYZ_M:
	{
		//COLORREF clr = 
		//	CExtPaintManager::stat_RGBtoCMY( m_clr );
		return
			CExtPaintManager::stat_CMYtoRGB(
				RGB(
					0,
					nZ,
					0
					)
				);
	}
	case MXYZ_Y:
	{
		//COLORREF clr = 
		//	CExtPaintManager::stat_RGBtoCMY( m_clr );
		return
			CExtPaintManager::stat_CMYtoRGB(
				RGB(
					0,
					0,
					nZ
					)
				);
	}
#ifdef _DEBUG
	default:
		ASSERT( FALSE );
		break;
#endif // _DEBUG
	}; // switch( m_eXYZmode )

	ASSERT( FALSE );
	return RGB( 0, 0, 0 );
}

void CExtColorCtrl::xyz_plain_data_t::GenerateSlider(
	CDC &dc,
	CPalette * pPalette
	)
{
	if( !m_bBmpValidSlider
		&& m_dcSlider.GetSafeHdc()
		)
		m_dcSlider.DeleteDC();
	if( m_dcSlider.GetSafeHdc() == NULL )
	{
		CBitmap bmp;
		_CreateCompatibleBitmapImpl(
			bmp,
			dc,
			m_sizeColorSlider.cx,
			m_sizeColorSlider.cy
			);
		m_dcSlider.CreateCompatibleDC( &dc );
		m_dcSlider.SelectObject( &bmp );

		CPalette * pOldPalette = NULL;
		if( pPalette != NULL )
		{
			pOldPalette =
				m_dcSlider.SelectPalette( pPalette, FALSE );
			m_dcSlider.RealizePalette();
		}

		int nBPP = g_PaintManager->stat_GetBPP();
		int nStep = ( nBPP <= 8 ) ? 4 : 1;

		for( int j = 0; j < m_sizeColorSlider.cx; j += nStep )
		{
			COLORREF clr =
				CalcColorSlider( j );
			if( nBPP <= 8 )
			{
				CBrush brush( clr );
				m_dcSlider.FillRect(
					CRect(
						CPoint(j,0),
						CSize(nStep,m_sizeColorSlider.cy)
						),
					& brush
					);
			}
			else
				m_dcSlider.FillSolidRect(
					j,
					0,
					1,
					m_sizeColorSlider.cy,
					clr
					);
		}
		
		if( pOldPalette != NULL )
			m_dcSlider.SelectPalette( pOldPalette, FALSE );

		m_bBmpValidSlider = true;
	}
}

void CExtColorCtrl::xyz_plain_data_t::GeneratePicker(
	CDC &dc,
	CPalette * pPalette
	)
{
	if( !m_bBmpValidPicker
		&& m_dcPicker.GetSafeHdc()
		)
		m_dcPicker.DeleteDC();
	if( m_dcPicker.GetSafeHdc() == NULL )
	{
		CBitmap bmp;
		_CreateCompatibleBitmapImpl(
			bmp,
			dc,
			m_sizeColorPicker.cx,
			m_sizeColorPicker.cy
			);
		m_dcPicker.CreateCompatibleDC( &dc );
		m_dcPicker.SelectObject( &bmp );

		CPalette * pOldPalette = NULL;
		if( pPalette != NULL )
		{
			pOldPalette =
				m_dcPicker.SelectPalette( pPalette, FALSE );
			m_dcPicker.RealizePalette();
		}

		int nBPP = g_PaintManager->stat_GetBPP();
		int nStep = ( nBPP <= 8 ) ? 4 : 1;

		for( int i = 0; i < m_sizeColorPicker.cx; i += nStep )
		{
			for( int j = 0; j < m_sizeColorPicker.cy; j += nStep )
			{
				COLORREF clr =
					CalcColorPicker( i, j );
				if( nBPP <= 8 )
				{
					CBrush brush( clr );
					m_dcPicker.FillRect(
						CRect(
							CPoint(i,j),
							CSize(nStep,nStep)
							),
						& brush
						);
				}
				else
					m_dcPicker.SetPixelV( i, j, clr );
			}
		}
		
		if( pOldPalette != NULL )
			m_dcPicker.SelectPalette( pOldPalette, FALSE );

		m_bBmpValidPicker = true;
	}	
}

void CExtColorCtrl::xyz_plain_data_t::RecalcByZ(
	int nZ
	)
{
int nX,nY;
	switch( m_eXYZmode )
	{
	case MXYZ_R:
		nX = GetGValue(m_clr);
		nY = GetBValue(m_clr);
		//nZ = GetRValue(m_clr);
		m_clr = RGB(nZ,nX,nY);
		break;
	case MXYZ_G:
		nX = GetRValue(m_clr);
		nY = GetBValue(m_clr);
		//nZ = GetGValue(m_clr);
		m_clr = RGB(nX,nZ,nY);
		break;
	case MXYZ_B:
		nX = GetRValue(m_clr);
		nY = GetGValue(m_clr);
		//nZ = GetBValue(m_clr);
		m_clr = RGB(nX,nY,nZ);
		break;
	case MXYZ_C:
	{
		COLORREF clr = 
			CExtPaintManager::stat_RGBtoCMY( m_clr );
		nX = GetGValue(clr);
		nY = GetBValue(clr);
		//nZ = GetRValue(clr);
		m_clr =
			CExtPaintManager::stat_CMYtoRGB( 
				RGB(nZ,nX,nY)
				);
		break;
	}
	case MXYZ_M:
	{
		COLORREF clr = 
			CExtPaintManager::stat_RGBtoCMY( m_clr );
		nX = GetRValue(clr);
		nY = GetBValue(clr);
		//nZ = GetGValue(clr);
		m_clr =
			CExtPaintManager::stat_CMYtoRGB( 
				RGB(nX,nZ,nY)
				);
		break;
	}
	case MXYZ_Y:
	{
		COLORREF clr = 
			CExtPaintManager::stat_RGBtoCMY( m_clr );
		nX = GetRValue(clr);
		nY = GetGValue(clr);
		//nZ = GetBValue(clr);
		m_clr =
			CExtPaintManager::stat_CMYtoRGB( 
				RGB(nX,nY,nZ)
				);
		break;
	}
#ifdef _DEBUG
	default:
		ASSERT( FALSE );
		break;
#endif // _DEBUG
	}; // switch( m_eXYZmode )
	CExtPaintManager::stat_RGBtoHSL(
		m_clr,
		&m_fCurrHue,
		&m_fCurrLuminance,
		&m_fCurrSaturation
		);
}

void CExtColorCtrl::xyz_plain_data_t::CalcPoints(
	int &nX,
	int &nY,
	int &nZ,
	bool bDcScale
	)
{
	nX = nY = nZ = 0;
	switch( m_eXYZmode )
	{
	case MXYZ_R:
		nX = GetGValue(m_clr);
		nY = GetBValue(m_clr);
		nZ = GetRValue(m_clr);
		break;
	case MXYZ_G:
		nX = GetRValue(m_clr);
		nY = GetBValue(m_clr);
		nZ = GetGValue(m_clr);
		break;
	case MXYZ_B:
		nX = GetRValue(m_clr);
		nY = GetGValue(m_clr);
		nZ = GetBValue(m_clr);
		break;
	case MXYZ_C:
	{
		COLORREF clr = 
			CExtPaintManager::stat_RGBtoCMY( m_clr );
		nX = GetGValue(clr);
		nY = GetBValue(clr);
		nZ = GetRValue(clr);
		break;
	}
	case MXYZ_M:
	{
		COLORREF clr = 
			CExtPaintManager::stat_RGBtoCMY( m_clr );
		nX = GetRValue(clr);
		nY = GetBValue(clr);
		nZ = GetGValue(clr);
		break;
	}
	case MXYZ_Y:
	{
		COLORREF clr = 
			CExtPaintManager::stat_RGBtoCMY( m_clr );
		nX = GetRValue(clr);
		nY = GetGValue(clr);
		nZ = GetBValue(clr);
		break;
	}
#ifdef _DEBUG
	default:
		ASSERT( FALSE );
		break;
#endif // _DEBUG
	}; // switch( m_eXYZmode )
	if( bDcScale )
	{
		nX =
			int( (double(nX)/255.0) * m_sizeColorPicker.cx );
		nY =
			int( (double(nY)/255.0) * m_sizeColorPicker.cy );
		nZ =
			int( (double(nZ)/255.0) * m_sizeColorSlider.cx );
	} // if( bDcScale )
}

void CExtColorCtrl::xyz_plain_data_t::OnDraw(
	CDC & dc,
	CPalette * pPalette
	)
{
CPalette * pOldPalette = NULL;
    if( pPalette != NULL )
    {
        pOldPalette =
			dc.SelectPalette( pPalette, FALSE );
        dc.RealizePalette();
    }

	GeneratePicker( dc, pPalette );
	GenerateSlider( dc, pPalette );

	dc.BitBlt(
		m_nAreaGap,
		m_nAreaGap,
		m_sizeColorPicker.cx,
		m_sizeColorPicker.cy,
		&m_dcPicker,
		0,
		0,
		SRCCOPY
		);
	dc.BitBlt(
		m_nAreaGap,
		m_nAreaGap*2 + m_sizeColorPicker.cy,
		m_sizeColorSlider.cx,
		m_sizeTotal.cy,
		&m_dcSlider,
		0,
		0,
		SRCCOPY
		);
int nX, nY, nZ;
	CalcPoints( nX, nY, nZ, true );

SIZE sz = { __SEL_BOX_DX, __SEL_BOX_DY };
CPoint ptDrawCalc = CPoint(nX,nY) + CPoint(m_nAreaGap,m_nAreaGap);
	DrawCrossAt(
		ptDrawCalc,
		dc,
		sz
		); // draw a rectangle on picker
	
	sz.cx = __SEL_BOX_DX;
	sz.cy = m_sizeColorSlider.cy;

CPoint pt(	nZ,
			m_sizeColorPicker.cy+m_sizeColorSlider.cy/2
			);
	ptDrawCalc = pt + CPoint(m_nAreaGap,m_nAreaGap*2);
	DrawCrossAt(
		ptDrawCalc,
		dc,
		sz
		); //draw rectangle on luminance bar

	if( pOldPalette != NULL )
		dc.SelectPalette( pOldPalette, FALSE );
}

void CExtColorCtrl::xyz_plain_data_t::OnMouseMove(
	UINT nFlags,
	CPoint point
	) 
{
	if( m_bBmpValidSlider && (nFlags & MK_LBUTTON) )
	{
		if( m_eMoveMode == _MM_SLIDER )
		{
			point.x -= m_nAreaGap;
			if( point.x < 0 )
				point.x = 0;
			point.y -= m_nAreaGap*2;
			if( point.y < 0 )
				point.y = 0;
			// change luminance
			if( point.x > m_sizeColorSlider.cx )
				point.x = m_sizeColorSlider.cx;

			int nZ = (point.x*255)/m_sizeColorSlider.cx;
			if( nZ > 255 )
				nZ = 255;
			RecalcByZ( nZ );
			// regenerate picker bar
			m_bBmpValidPicker = false; 
		}
		else if(m_eMoveMode == _MM_PICKER)
		{
			point.x -= m_nAreaGap;
			if( point.x < 0 )
				point.x = 0;
			point.y -= m_nAreaGap;
			if( point.y < 0 )
				point.y = 0;
			
			if( point.x > m_sizeColorPicker.cx )
				point.x = m_sizeColorPicker.cx;
			if( point.y > m_sizeColorPicker.cy )
				point.y = m_sizeColorPicker.cy;
			
			if( nFlags & MK_CONTROL )
				point.x = m_ptMousePos.x;
			
			if( nFlags & MK_SHIFT )
				point.y = m_ptMousePos.y;

			m_clr =
				CalcColorPicker( point.x, point.y );
			CExtPaintManager::stat_RGBtoHSL(
				m_clr,
				&m_fCurrHue,
				&m_fCurrLuminance,
				&m_fCurrSaturation
				);

			m_ptMousePos = point;
		}
		// ... color chaged ... needs repaint ...
	}
}

/////////////////////////////////////////////////////////////////////////////
// CExtColorCtrl::diff_data_t
void CExtColorCtrl::diff_data_t::Generate(bool bOnlySizeChanged)
{
//	if( !bOnlySizeChanged )
//	{
//		ASSERT( m_vCells.empty() );
		m_vCells.SetSize( 2 );
//	}

	if( !bOnlySizeChanged )
	{
		m_vCells[0].m_clr = m_clr;
		m_vCells[0].m_bAtFirstType = false;
		m_vCells[0].m_nBorderSize = 0;
		m_vCells[1].m_clr = m_clrOld;
		m_vCells[1].m_bAtFirstType = false;
		m_vCells[1].m_nBorderSize = 0;
	}
	m_vCells[0].m_rcPosition =
		CRect(
			CPoint(0,0),
			CSize( m_sizeTotal.cx, m_sizeTotal.cy/2 )
			);
	m_vCells[1].m_rcPosition =
		CRect(
			CPoint(0,m_sizeTotal.cy/2),
			CSize( m_sizeTotal.cx, m_sizeTotal.cy/2 )
			);
}

void CExtColorCtrl::diff_data_t::OnInit(
	CDC & dc,
	CPalette * pPalette,
	CSize sizeTotal,
	bool bOnlySizeChanged
	)
{
	any_picker_data_t::OnInit(
		dc,
		pPalette,
		sizeTotal,
		bOnlySizeChanged
		);
	Generate(bOnlySizeChanged);
}

void CExtColorCtrl::diff_data_t::OnDraw(
	CDC & dc,
	CPalette * pPalette
	)
{
CPalette * pOldPalette = NULL;
    if( pPalette != NULL )
    {
        pOldPalette =
			dc.SelectPalette( pPalette, FALSE );
        dc.RealizePalette();
    }

INT it = 0;
	for( ; it < m_vCells.GetSize(); ++it )
		m_vCells[it].OnDraw( dc, pPalette, false );

	if( pOldPalette != NULL )
		dc.SelectPalette( pOldPalette, FALSE );
}

void CExtColorCtrl::diff_data_t::OnSetRGB(
	COLORREF clr
	)
{
	grayscale_mixer_data_t::OnSetRGB( clr );
	Generate( false );
}

void CExtColorCtrl::diff_data_t::OnSetHLS(
	double hue,
	double luminance,
	double saturation
	)
{
	grayscale_mixer_data_t::OnSetHLS(
		hue,
		luminance,
		saturation
		);
	Generate( false );
}

/////////////////////////////////////////////////////////////////////////////
// CExtColorCtrl

CExtColorCtrl::CExtColorCtrl(
	e_mode_t eMode // = MODE_HEXAGON
	)
	: m_xyz_r( xyz_plain_data_t::MXYZ_R )
	, m_xyz_g( xyz_plain_data_t::MXYZ_G )
	, m_xyz_b( xyz_plain_data_t::MXYZ_B )
	, m_xyz_c( xyz_plain_data_t::MXYZ_C )
	, m_xyz_m( xyz_plain_data_t::MXYZ_M )
	, m_xyz_y( xyz_plain_data_t::MXYZ_Y )
{
	ASSERT(
		MODE_VAL_MIN <= eMode
		&& eMode <= MODE_VAL_MAX
		);
	m_eMode = eMode;
	m_bMouseTracking = false;

	m_algorithms[ 0] = &m_hexagon_data;
	m_algorithms[ 1] = &m_hsl_panel_data;
	m_algorithms[ 2] = &m_hsl_basic_hue_data;
	m_algorithms[ 3] = &m_hsl_roller_data;
	m_algorithms[ 4] = &m_rgb_cube_data;
	m_algorithms[ 5] = &m_rgb_mixer_data;
	m_algorithms[ 6] = &m_grayscale_mixer_data;
	m_algorithms[ 7] = &m_cmy_cube_data;
	m_algorithms[ 8] = &m_cmy_mixer_data;
	m_algorithms[ 9] = &m_xyz_r;
	m_algorithms[10] = &m_xyz_g;
	m_algorithms[11] = &m_xyz_b;
	m_algorithms[12] = &m_xyz_c;
	m_algorithms[13] = &m_xyz_m;
	m_algorithms[14] = &m_xyz_y;
	m_algorithms[15] = &m_diff_data;
}

CExtColorCtrl::~CExtColorCtrl()
{
	_Cleanup();
}

void CExtColorCtrl::_Cleanup()
{
	for( int i=0; i<=MODE_VAL_MAX; i++ )
	{
		ASSERT( m_algorithms[i] != NULL );
		m_algorithms[i]->OnDone();
	}
}

void CExtColorCtrl::_Init( bool bOnlySizeChanged )
{
	if( !bOnlySizeChanged )
		_Cleanup();
	if( GetSafeHwnd() == NULL )
		return;
CRect rcClient;
	GetClientRect( &rcClient );
//	rcClient.DeflateRect(2,2);

	ASSERT(
		MODE_VAL_MIN <= m_eMode
		&& m_eMode <= MODE_VAL_MAX
		);
	ASSERT( m_algorithms[m_eMode] != NULL );
CClientDC dcClient( this );
	m_algorithms[m_eMode]->OnInit(
		dcClient,
		&g_PaintManager->m_PaletteWide,
		rcClient.Size(),
		bOnlySizeChanged
		);
	m_algorithms[m_eMode]->OnSetRGB(
		m_selClrs.m_clrNew
		);
	m_algorithms[m_eMode]->OnSetOldRGB(
		m_selClrs.m_clrCurrent
		);
}

BEGIN_MESSAGE_MAP(CExtColorCtrl, CWnd)
	//{{AFX_MSG_MAP(CExtColorCtrl)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CANCELMODE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	ON_WM_KILLFOCUS()
	ON_WM_CAPTURECHANGED()
	ON_WM_SIZE()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CExtColorCtrl message handlers

void CExtColorCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	_MouseTracking( false );
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CExtColorCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
//	CWnd::OnLButtonDown(nFlags, point);
	_MouseTracking( true );

	ASSERT(
		MODE_VAL_MIN <= m_eMode
		&& m_eMode <= MODE_VAL_MAX
		);
	ASSERT( m_algorithms[m_eMode] != NULL );

	if( m_algorithms[m_eMode]->m_clr != m_selClrs.m_clrNew )
	{
		m_algorithms[m_eMode]->OnSetRGB( m_selClrs.m_clrNew );
		Invalidate();
		UpdateWindow();
	}
	m_algorithms[m_eMode]->OnLButtonDown( nFlags, point );
	OnMouseMove( 0, point );
	if( m_algorithms[m_eMode]->m_clr != m_selClrs.m_clrNew )
	{
		m_selClrs.m_clrPrev = m_selClrs.m_clrNew;
		m_selClrs.m_clrNew = m_algorithms[m_eMode]->m_clr;
		NotifyColorChanged();
		Invalidate();
		UpdateWindow();
	}
}

void CExtColorCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
//	CWnd::OnLButtonUp(nFlags, point);
	_MouseTracking( false );

	ASSERT(
		MODE_VAL_MIN <= m_eMode
		&& m_eMode <= MODE_VAL_MAX
		);
	ASSERT( m_algorithms[m_eMode] != NULL );

	m_algorithms[m_eMode]->OnLButtonUp( nFlags, point );
}

UINT CExtColorCtrl::g_nMsgColorChanged =
	::RegisterWindowMessage(
		_T("CExtColorCtrl::g_nMsgColorChanged")
		);
void CExtColorCtrl::NotifyColorChanged()
{
	GetParent()->SendMessage(
		g_nMsgColorChanged,
		WPARAM( GetSafeHwnd() ),
		WPARAM( &m_selClrs )
		);
}

void CExtColorCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
//	CWnd::OnMouseMove(nFlags, point);
	if( !m_bMouseTracking )
		return;
	ASSERT( GetSafeHwnd() != NULL );
	ASSERT(
		MODE_VAL_MIN <= m_eMode
		&& m_eMode <= MODE_VAL_MAX
		);
	ASSERT( m_algorithms[m_eMode] != NULL );
	m_algorithms[m_eMode]->OnMouseMove( nFlags, point );
	if( m_algorithms[m_eMode]->m_clr != m_selClrs.m_clrNew )
	{
		m_selClrs.m_clrPrev = m_selClrs.m_clrNew;
		m_selClrs.m_clrNew = m_algorithms[m_eMode]->m_clr;
		NotifyColorChanged();
		Invalidate();
		UpdateWindow();
	}
}

void CExtColorCtrl::OnCancelMode() 
{
	CWnd::OnCancelMode();
	_MouseTracking( false );
}

void CExtColorCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	_MouseTracking( false );
}

void CExtColorCtrl::OnCaptureChanged(CWnd *pWnd) 
{
//	if( m_bMouseTracking )
//		_MouseTracking( false );
	CWnd::OnCaptureChanged(pWnd);
}

BOOL CExtColorCtrl::OnQueryNewPalette() 
{
	Invalidate();
	return CWnd::OnQueryNewPalette();
}

void CExtColorCtrl::OnPaletteChanged(CWnd* pFocusWnd) 
{
	CWnd::OnPaletteChanged(pFocusWnd);
	Invalidate();
}

BOOL CExtColorCtrl::OnEraseBkgnd(CDC* pDC) 
{
	pDC;
	return TRUE;
}

void CExtColorCtrl::OnPaint() 
{
CPaintDC dcPaint(this); // device context for painting
CRect rcClient;
	GetClientRect(&rcClient);
	if( rcClient.IsRectEmpty() ||
		!dcPaint.RectVisible(&rcClient)
		)
		return;

    // Select and realize the palette
CPalette * pOldPalette1 = NULL, * pOldPalette2 = NULL;
    if( dcPaint.GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
    {
        pOldPalette1 =
			dcPaint.SelectPalette( &g_PaintManager->m_PaletteWide, FALSE );
        dcPaint.RealizePalette();
    }

CExtMemoryDC dc( &dcPaint,&rcClient );
	dc.FillSolidRect(
		&rcClient,
		g_PaintManager->GetColor(
			CExtPaintManager::CLR_3DFACE_OUT
			)
		);
    if( dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
    {
        pOldPalette2 =
			dc.SelectPalette( &g_PaintManager->m_PaletteWide, FALSE );
        dc.RealizePalette();
    }

	ASSERT(
		MODE_VAL_MIN <= m_eMode
		&& m_eMode <= MODE_VAL_MAX
		);
	ASSERT( m_algorithms[m_eMode] != NULL );
	m_algorithms[m_eMode]->OnDraw(
		dc,
		(pOldPalette2 == NULL) ? NULL : &g_PaintManager->m_PaletteWide
		);

	if( pOldPalette2 != NULL )
		dc.SelectPalette( pOldPalette2, FALSE );
	if( pOldPalette1 != NULL )
		dcPaint.SelectPalette( pOldPalette1, FALSE );
}

void CExtColorCtrl::_MouseTracking( bool bStart /*= true*/ )
{
	if( m_bMouseTracking && bStart )
		return;
	if( !m_bMouseTracking && !bStart )
		return;
	m_bMouseTracking = bStart;
	if( m_bMouseTracking )
	{
		CExtMouseCaptureSink::SetCapture( GetSafeHwnd() );
	} // if( m_bMouseTracking )
	else
	{
		if( CExtMouseCaptureSink::GetCapture() == GetSafeHwnd() )
			CExtMouseCaptureSink::ReleaseCapture();
	} // else from if( m_bMouseTracking )
}

void CExtColorCtrl::PreSubclassWindow() 
{
	CWnd::PreSubclassWindow();
	_Init( false );
}

void CExtColorCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	_Init( true );
	Invalidate();
	UpdateWindow();
}

void CExtColorCtrl::SetupColor(
	COLORREF clr,
	bool bNotifyChanged
	)
{
	if( clr != m_selClrs.m_clrNew )
	{
		m_selClrs.m_clrNew = clr;
		if( bNotifyChanged )
			NotifyColorChanged();
	}

	ASSERT(
		MODE_VAL_MIN <= m_eMode
		&& m_eMode <= MODE_VAL_MAX
		);
	ASSERT( m_algorithms[m_eMode] != NULL );

	m_algorithms[m_eMode]->OnSetRGB( m_selClrs.m_clrNew );

	Invalidate();
	UpdateWindow();
}

void CExtColorCtrl::SetupOldColor( // suggested by Olaf Baeyens
	COLORREF clr
	)
{
	if( m_selClrs.m_clrPrev == clr )
		return;
	m_selClrs.m_clrPrev = clr;
    m_algorithms[m_eMode]->OnSetOldRGB( m_selClrs.m_clrPrev );
    Invalidate();
    UpdateWindow();
}

void CExtColorCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	nChar;
	nRepCnt;
	nFlags;
//	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CExtColorCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	nFlags;
	point;
//	CWnd::OnLButtonDblClk(nFlags, point);
}

void CExtColorCtrl::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
	nFlags;
	point;
//	CWnd::OnMButtonDblClk(nFlags, point);
}

void CExtColorCtrl::OnMButtonDown(UINT nFlags, CPoint point) 
{
	nFlags;
	point;
//	CWnd::OnMButtonDown(nFlags, point);
}

void CExtColorCtrl::OnMButtonUp(UINT nFlags, CPoint point) 
{
	nFlags;
	point;
//	CWnd::OnMButtonUp(nFlags, point);
}

void CExtColorCtrl::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	nFlags;
	point;
//	CWnd::OnRButtonDblClk(nFlags, point);
}

void CExtColorCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	nFlags;
	point;
//	CWnd::OnRButtonDown(nFlags, point);
}

void CExtColorCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	nFlags;
	point;
//	CWnd::OnRButtonUp(nFlags, point);
}
