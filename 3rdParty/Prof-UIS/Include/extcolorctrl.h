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

#if (!defined __EXTCOLORCTRL_H)
#define __EXTCOLORCTRL_H

// 
// The idea of the HSL colorspace algorithm  belongs to Patrick Prache
// The idea of the hexagon pallette algorithm belongs to Jack Mesic
//

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_MOUSECAPTURESINK_H)
	#include <../Src/ExtMouseCaptureSink.h>
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>

#if (!defined __AFXPLEX_H__)
	#include <AfxTempl.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtColorCtrl window

class __PROF_UIS_API CExtColorCtrl
	: public CWnd
	, public CExtMouseCaptureSink
{
// Construction
public:
	enum e_mode_t
	{
		MODE_HEXAGON = 0,
		MODE_HSL_PANEL = 1,
		MODE_HSL_BASIC_HUE = 2,
		MODE_HSL_ROLLER = 3,
		MODE_RGB_CUBE = 4,
		MODE_RGB_MIX = 5,
		MODE_GRAY_SCALE_MIX = 6,
		MODE_CMY_CUBE = 7,
		MODE_CMY_MIX = 8,
		MODE_XYZ_R = 9,
		MODE_XYZ_G = 10,
		MODE_XYZ_B = 11,
		MODE_XYZ_C = 12,
		MODE_XYZ_M = 13,
		MODE_XYZ_Y = 14,
		MODE_DIFF = 15,
		MODE_VAL_MIN = 0,
		MODE_VAL_MAX = 15
	};

	CExtColorCtrl(
		e_mode_t eMode = MODE_HEXAGON
		);

	struct __PROF_UIS_API selection_colors_t
	{
		COLORREF m_clrCurrent;
		COLORREF m_clrNew;
		COLORREF m_clrPrev;
		selection_colors_t(
			COLORREF clrCurrent = RGB(255,255,255),
			COLORREF clrNew = RGB(255,255,255),
			COLORREF clrPrev = RGB(255,255,255)
			)
			: m_clrCurrent( clrCurrent )
			, m_clrNew( clrNew )
			, m_clrPrev( clrPrev )
		{
		};
		selection_colors_t(
			const selection_colors_t & other
			)
			: m_clrCurrent( RGB(255,255,255) )
			, m_clrNew( RGB(255,255,255) )
			, m_clrPrev( RGB(255,255,255) )
		{
			AssignFromOther( other );
		};
		~selection_colors_t()
		{
		};
		selection_colors_t & operator=(
			const selection_colors_t & other
			)
		{
			AssignFromOther( other );
			return *this;
		};
	private:
		void AssignFromOther(
			const selection_colors_t & other
			)
		{
			m_clrCurrent = other.m_clrCurrent;
			m_clrNew = other.m_clrNew;
			m_clrPrev = other.m_clrPrev;
		};
	}; // struct selection_colors_t

// Attributes
public:

private:
	e_mode_t m_eMode;
	bool m_bMouseTracking;

	struct __PROF_UIS_API any_picker_data_t
	{
		enum eMoveMode_t
		{
			_MM_NONE = 0,
			_MM_RED = 1,
			_MM_GREEN = 2,
			_MM_BLUE = 3,
			_MM_SLIDER = 4,
			_MM_PICKER = 5,
			_MM_CELL_SCAN = 6,
		};
		eMoveMode_t m_eMoveMode;

		CSize m_sizeTotal;
		COLORREF m_clr;
		COLORREF m_clrOld;
		double
			m_fCurrLuminance,
			m_fCurrSaturation,
			m_fCurrHue;

		any_picker_data_t();
		~any_picker_data_t();

		virtual void OnInit(
			CDC & dc,
			CPalette * pPalette,
			CSize sizeTotal,
			bool bOnlySizeChanged
			);
		virtual void OnDone() = 0;
		virtual void OnDraw(
			CDC & dc,
			CPalette * pPalette
			) = 0;
		virtual void OnLButtonDown(
			UINT nFlags,
			CPoint point
			) = 0; 
		virtual void OnLButtonUp(
			UINT nFlags,
			CPoint point
			) = 0; 
		virtual void OnMouseMove(
			UINT nFlags,
			CPoint point
			) = 0; 
		virtual void OnSetOldRGB(
			COLORREF clr
			);
		virtual void OnSetRGB(
			COLORREF clr
			);
		virtual void OnSetHLS(
			double hue,
			double luminance,
			double saturation
			);

	}; // struct any_picker_data_t

	struct __PROF_UIS_API cell_t
	{
		COLORREF	m_clr;
		CRect		m_rcPosition;
		int			m_nBorderSize;
		CPoint		m_vPoints[6];
		bool		m_bAtFirstType;

		cell_t( COLORREF clr = RGB(255,255,255) )
			: m_clr( clr )
			, m_nBorderSize( 1 )
			, m_bAtFirstType( true )
		{
			for( int i = 0; i < 6; i++ )
				m_vPoints[i] = CPoint(0,0);
		};
		cell_t( const cell_t & other )
			: m_clr( RGB(255,255,255) )
			, m_nBorderSize( 1 )
			, m_bAtFirstType( true )
		{
			for( int i = 0; i < 6; i++ )
				m_vPoints[i] = CPoint(0,0);
			AssignFromOther( other );
		};
		~cell_t()
		{
		};
		cell_t& operator=( const cell_t & other )
		{
			AssignFromOther( other );
			return *this;
		};
		cell_t& operator=( COLORREF clr )
		{
			m_clr = clr;
			return *this;
		};
		void AdjustCellPosition(
			int x,
			int y,
			int nWidth
			)
		{
			float nSideLength =
				(float)((float)nWidth * 0.57735026918962F);
			m_vPoints[0] = CPoint(MoreNear(x - (float)(nWidth / 2)),MoreNear(y - (float)(nSideLength/2)));
			m_vPoints[1] = CPoint(MoreNear((float)x),MoreNear(y - (float)(nWidth / 2)));
			m_vPoints[2] = CPoint(MoreNear(x + (float)(nWidth / 2)),MoreNear(y - (float)(nSideLength/2)));
			m_vPoints[3] = CPoint(MoreNear(x + (float)(nWidth / 2)),MoreNear(y + (float)(nSideLength/2))+1);
			m_vPoints[4] = CPoint(MoreNear((float)x),MoreNear(y + (float)(nWidth / 2))+1);
			m_vPoints[5] = CPoint(MoreNear(x - (float)(nWidth / 2)),MoreNear(y + (float)(nSideLength/2))+1);
		};
		static int MoreNear(float fNear)
		{
			int nRound = (int)fabs(fNear);
			nRound = (fabs(fNear) - (float)nRound >= 0.5) ? nRound+1 : nRound;
			return (fNear < 0) ? -nRound : nRound;
		};
		void SetRect( const CRect & rcPos )
		{
			m_rcPosition = rcPos;
		};
		CRect GetRect() const
		{
			if( !m_bAtFirstType )
			{
				return m_rcPosition;
			}
			CRect rcBounds;
			
			rcBounds.left = rcBounds.right  = m_vPoints[0].x;
			rcBounds.top  = rcBounds.bottom = m_vPoints[0].y;
			
			for (int i=1; i<6; i++)
			{
				rcBounds.left   = min(rcBounds.left,   m_vPoints[i].x);
				rcBounds.top    = min(rcBounds.top,    m_vPoints[i].y);
				rcBounds.right  = max(rcBounds.right,  m_vPoints[i].x);
				rcBounds.bottom = max(rcBounds.bottom, m_vPoints[i].y);
			}
			return rcBounds;
		};
		void SetCellColor(const COLORREF &cr) 
		{ 
			m_clr = cr; 
		};
		void OnDraw(
			CDC & dc,
			CPalette * pPalette,
			bool bFocused
			)
		{
			// prepare DC
			ASSERT( dc.GetSafeHdc() != NULL );
			dc.SaveDC();
			dc.SetBkMode( TRANSPARENT );
			CBrush _brush( m_clr );
			dc.SelectObject( &_brush );
			CPen _pen( PS_SOLID, 0, m_clr );
			dc.SelectObject( &_pen );

			CBrush * pBrWhite = NULL;
			CBrush * pBrBlack = NULL;
			if( bFocused )
			{
				pBrWhite = CBrush::FromHandle ((HBRUSH) ::GetStockObject (WHITE_BRUSH));
				ASSERT_VALID (pBrWhite);
				pBrBlack = CBrush::FromHandle ((HBRUSH) ::GetStockObject (BLACK_BRUSH));
				ASSERT_VALID (pBrBlack);
			} // if( bFocused )

			if( !m_bAtFirstType )
			{ // draw rectangular
				if( bFocused )
				{
//					CRgn rgnOne, rgnTwo, rgnThree;
//					CRect rcPos;
//					rcPos = m_rcPosition;
//					rcPos.DeflateRect(1,1,1,1);
//					rgnTwo.CreateRectRgnIndirect(&rcPos);
//					dc.FrameRgn(&rgnTwo, pBrWhite, 2, 2);
//					rcPos.InflateRect(2,2,2,2);
//					rgnThree.CreateRectRgnIndirect(&rcPos);
//					dc.FrameRgn(&rgnThree, pBrBlack, 1, 1);
					CRect rcPos(m_rcPosition);
					rcPos.InflateRect(3,3);
					dc.FillSolidRect(rcPos,RGB(0,0,0));
					rcPos.DeflateRect(1,1);
					dc.FillSolidRect(rcPos,RGB(255,255,255));
					rcPos.DeflateRect(1,1);
					dc.FillSolidRect(rcPos,RGB(0,0,0));
					dc.FillSolidRect(m_rcPosition,m_clr);
				} // if( bFocused )
				else
				{
					//dc.Rectangle( rcPos );
					if( pPalette != NULL )
					{
						CBrush brush( m_clr );
						dc.FillRect(
							m_rcPosition,
							& brush
							);
					}
					else
						dc.FillSolidRect(m_rcPosition,m_clr);
				} // else from if( bFocused )
			} // draw rectangular
			else
			{ // draw hexagonal
				if( bFocused )
				{
//					if(m_clr == RGB(255,255,255))
//					{
//						CRgn rgnOne;
//						rgnOne.CreatePolygonRgn(m_vPoints, 6, ALTERNATE);
//						dc.FrameRgn(&rgnOne, pBrWhite, 3, 3);
//						dc.FrameRgn(&rgnOne, pBrBlack, 2, 2);
//					}
//					else
//					{
//						CRgn rgnOne;
//						rgnOne.CreatePolygonRgn(m_vPoints, 6, ALTERNATE);
//						dc.FrameRgn(&rgnOne, pBrWhite, 3, 3);
//						dc.FrameRgn(&rgnOne, pBrBlack, 1, 1);
//					}

					CRgn rgnOne,rgnTwo;
					CPoint vPoints[6];
					for( int i = 0; i < 6; i++ )
						vPoints[i] = m_vPoints[i];
					vPoints[0].x -= 2;
					vPoints[0].y -= 1;
					
					vPoints[5].x -= 2;
					vPoints[5].y += 1;
					
					vPoints[2].x += 2;
					vPoints[2].y -= 1;
					vPoints[3].x += 2;
					vPoints[3].y += 1;
					
					vPoints[1].y -= 2;
					vPoints[4].y += 2;
					rgnOne.CreatePolygonRgn(
						vPoints, 6, ALTERNATE );
					rgnTwo.CreatePolygonRgn(
						m_vPoints, 6, ALTERNATE );

					dc.SelectObject( pBrWhite );
					dc.Polygon( vPoints, 6 );
					dc.SelectObject( &_brush );
					dc.Polygon( m_vPoints, 6 );

					dc.FrameRgn(
						&rgnOne, pBrWhite, 3, 3 );
					dc.FrameRgn(
						&rgnOne, pBrBlack, 1, 1 );
					dc.FrameRgn(
						&rgnTwo, pBrBlack, 1, 1 );
				} // if( bFocused )
				else
					dc.Polygon( m_vPoints, 6 );
			} // draw hexagonal

			// resrore DC
			dc.RestoreDC(-1);
		};
		BOOL HitTest(CPoint point)
		{
			if( m_bAtFirstType )
			{
				CRgn rgn;
				rgn.CreatePolygonRgn(m_vPoints, 6, ALTERNATE);
				return rgn.PtInRegion (point);
			}
			else
			{
				CRgn rgn;
				rgn.CreateRectRgn(m_rcPosition.left,m_rcPosition.top,m_rcPosition.right,m_rcPosition.bottom);
				return rgn.PtInRegion (point);
			}
		};
		
	private:
		void AssignFromOther( const cell_t & other )
		{
			m_clr = other.m_clr;
			m_rcPosition = other.m_rcPosition;
			m_nBorderSize = other.m_nBorderSize;
			for( int i = 0; i < 6; i++ )
				m_vPoints[i] = other.m_vPoints[i];
			m_bAtFirstType = other.m_bAtFirstType;
		};
	}; // struct cell_t

	typedef
		CArray < cell_t, cell_t & >
		vCells_t;

	struct __PROF_UIS_API cell_based_data_t : public any_picker_data_t
	{
		vCells_t m_vCells;

		cell_based_data_t()
		{
		};
		~cell_based_data_t()
		{
			ReleaseAll();
		};

		virtual void OnInit(
			CDC & dc,
			CPalette * pPalette,
			CSize sizeTotal,
			bool bOnlySizeChanged
			);
		virtual void OnDone();
		virtual void OnDraw(
			CDC & dc,
			CPalette * pPalette
			);
		virtual void OnLButtonDown(
			UINT nFlags,
			CPoint point
			); 
		virtual void OnLButtonUp(
			UINT nFlags,
			CPoint point
			); 
		virtual void OnMouseMove(
			UINT nFlags,
			CPoint point
			); 

	private:
		void Generate(
			bool bOnlySizeChanged
			);
		void ReleaseAll();
	}; // struct cell_based_data_t

	struct __PROF_UIS_API grayscale_mixer_data_t : public cell_based_data_t
	{
		grayscale_mixer_data_t()
		{
		};
		~grayscale_mixer_data_t()
		{
		};
		virtual void OnInit(
			CDC & dc,
			CPalette * pPalette,
			CSize sizeTotal,
			bool bOnlySizeChanged
			);
		virtual void OnDraw(
			CDC & dc,
			CPalette * pPalette
			);
	}; // struct grayscale_mixer_data_t

	struct __PROF_UIS_API rgb_mixer_data_t : public cell_based_data_t
	{
		rgb_mixer_data_t()
		{
		};
		~rgb_mixer_data_t()
		{
		};
		virtual void OnInit(
			CDC & dc,
			CPalette * pPalette,
			CSize sizeTotal,
			bool bOnlySizeChanged
			);
		virtual void OnDraw(
			CDC & dc,
			CPalette * pPalette
			);
		virtual void OnMouseMove(
			UINT nFlags,
			CPoint point
			); 
	}; // struct rgb_mixer_data_t

	struct __PROF_UIS_API rgb_cube_data_t : public any_picker_data_t
	{
		CDC m_dcCube;
		CSize m_sizeHalfSelBox;
		CRect m_rcCube;
		int m_nSideGap;
		CPoint
			m_ptCubeR,m_ptCubeG,m_ptCubeB,m_ptCubeC,
			m_ptClrR,m_ptClrG,m_ptClrB,m_ptClrCurr,
			m_ptPlainRG,m_ptPlainRB,m_ptPlainGB;

		rgb_cube_data_t();
		~rgb_cube_data_t();

		virtual void OnInit(
			CDC & dc,
			CPalette * pPalette,
			CSize sizeTotal,
			bool bOnlySizeChanged
			);
		virtual void OnDone();
		virtual void OnDraw(
			CDC & dc,
			CPalette * pPalette
			);
		virtual void OnLButtonDown(
			UINT nFlags,
			CPoint point
			); 
		virtual void OnLButtonUp(
			UINT nFlags,
			CPoint point
			); 
		virtual void OnMouseMove(
			UINT nFlags,
			CPoint point
			); 
		virtual void OnSetRGB(
			COLORREF clr
			);
		virtual void OnSetHLS(
			double hue,
			double luminance,
			double saturation
			);

	private:
		void ReleaseAll();
		void Generate(
			CDC & dc,
			CPalette * pPalette
			);
		void RecalcLayout();

	}; // struct rgb_cube_data_t

	struct __PROF_UIS_API cmy_mixer_data_t : public cell_based_data_t
	{
		cmy_mixer_data_t()
		{
		};
		~cmy_mixer_data_t()
		{
		};
		virtual void OnInit(
			CDC & dc,
			CPalette * pPalette,
			CSize sizeTotal,
			bool bOnlySizeChanged
			);
		virtual void OnDraw(
			CDC & dc,
			CPalette * pPalette
			);
		virtual void OnMouseMove(
			UINT nFlags,
			CPoint point
			); 
	}; // struct cmy_mixer_data_t

	struct __PROF_UIS_API cmy_cube_data_t : public any_picker_data_t
	{
		CDC m_dcCube;
		CSize m_sizeHalfSelBox;
		CRect m_rcCube;
		int m_nSideGap;
		CPoint
			m_ptCubeR,m_ptCubeG,m_ptCubeB,m_ptCubeC,
			m_ptClrR,m_ptClrG,m_ptClrB,m_ptClrCurr,
			m_ptPlainRG,m_ptPlainRB,m_ptPlainGB;

		cmy_cube_data_t();
		~cmy_cube_data_t();

		virtual void OnInit(
			CDC & dc,
			CPalette * pPalette,
			CSize sizeTotal,
			bool bOnlySizeChanged
			);
		virtual void OnDone();
		virtual void OnDraw(
			CDC & dc,
			CPalette * pPalette
			);
		virtual void OnLButtonDown(
			UINT nFlags,
			CPoint point
			); 
		virtual void OnLButtonUp(
			UINT nFlags,
			CPoint point
			); 
		virtual void OnMouseMove(
			UINT nFlags,
			CPoint point
			); 
		virtual void OnSetRGB(
			COLORREF clr
			);
		virtual void OnSetHLS(
			double hue,
			double luminance,
			double saturation
			);

	private:
		void ReleaseAll();
		void Generate(
			CDC & dc,
			CPalette * pPalette
			);
		void RecalcLayout();

	}; // struct cmy_cube_data_t

	struct __PROF_UIS_API hsl_panel_data_t : public any_picker_data_t
	{
		CDC m_dcPicker;
		CDC m_dcSlider;
		bool m_bBmpValidPicker;
		bool m_bBmpValidSlider;

		CPoint m_ptMousePos; // position of the picker

		SIZE m_sizeColorPicker;
		SIZE m_sizeColorSlider;
	
		int m_nLumBarDy;
		int m_nAreaGap;

		hsl_panel_data_t();
		~hsl_panel_data_t();

		virtual void OnInit(
			CDC & dc,
			CPalette * pPalette,
			CSize sizeTotal,
			bool bOnlySizeChanged
			);
		virtual void OnDone();
		virtual void OnDraw(
			CDC & dc,
			CPalette * pPalette
			);
		virtual void OnLButtonDown(
			UINT nFlags,
			CPoint point
			); 
		virtual void OnLButtonUp(
			UINT nFlags,
			CPoint point
			); 
		virtual void OnMouseMove(
			UINT nFlags,
			CPoint point
			); 
		virtual void OnSetRGB(
			COLORREF clr
			);
		virtual void OnSetHLS(
			double hue,
			double luminance,
			double saturation
			);
	private:
		void ReleaseAll();
	protected:
		virtual void GenerateSlider(
			CDC &dc,
			CPalette * pPalette
			);
		virtual void GeneratePicker(
			CDC &dc,
			CPalette * pPalette
			);
		void DrawCrossAt(
			CPoint & point,
			CDC & dc,
			SIZE & sz
			);
	}; // struct hsl_panel_data_t

	struct __PROF_UIS_API hsl_basic_hue_data_t : public hsl_panel_data_t
	{
		hsl_basic_hue_data_t()
		{
		};
		~hsl_basic_hue_data_t()
		{
		};

		virtual void OnDraw(
			CDC & dc,
			CPalette * pPalette
			);
		virtual void OnMouseMove(
			UINT nFlags,
			CPoint point
			); 
		virtual void OnSetRGB(
			COLORREF clr
			);
		virtual void OnSetHLS(
			double hue,
			double luminance,
			double saturation
			);
	protected:
		virtual void GenerateSlider(
			CDC &dc,
			CPalette * pPalette
			);
		virtual void GeneratePicker(
			CDC &dc,
			CPalette * pPalette
			);
	}; // struct hsl_basic_hue_data_t

	struct __PROF_UIS_API hsl_roller_data_t : public hsl_panel_data_t
	{
		hsl_roller_data_t()
		{
			m_ptCenterSpaceHueSat.x = m_ptCenterSpaceHueSat.y = 0;
			m_nSatTranslation = 1;
		};
		~hsl_roller_data_t()
		{
		};

		virtual void OnDraw(
			CDC & dc,
			CPalette * pPalette
			);
		virtual void OnLButtonDown(
			UINT nFlags,
			CPoint point
			); 
		virtual void OnLButtonUp(
			UINT nFlags,
			CPoint point
			); 
		virtual void OnMouseMove(
			UINT nFlags,
			CPoint point
			); 
		virtual void OnSetRGB(
			COLORREF clr
			);
		virtual void OnSetHLS(
			double hue,
			double luminance,
			double saturation
			);
	protected:
		virtual void GeneratePicker(
			CDC &dc,
			CPalette * pPalette
			);
		CPoint m_ptCenterSpaceHueSat;
		CRgn m_rgnSpaceHueSat,m_rgnMouseSelHueSat;
		INT m_nSatTranslation;
	}; // struct hsl_roller_data_t

	struct __PROF_UIS_API xyz_plain_data_t : public hsl_panel_data_t
	{
		enum eXYZmode_t
		{
			MXYZ_R = 0,
			MXYZ_G = 1,
			MXYZ_B = 2,
			MXYZ_C = 3,
			MXYZ_M = 4,
			MXYZ_Y = 5,
			MXYZ_MIN = 0,
			MXYZ_MAX = 5
		};
		eXYZmode_t m_eXYZmode;

		xyz_plain_data_t(
			eXYZmode_t eXYZmode
			)
			: m_eXYZmode( eXYZmode )
		{
			ASSERT(
				MXYZ_MIN <= m_eXYZmode
				&& m_eXYZmode <= MXYZ_MAX
				);
		};
		~xyz_plain_data_t()
		{
		};

		virtual void OnDraw(
			CDC & dc,
			CPalette * pPalette
			);
		virtual void OnMouseMove(
			UINT nFlags,
			CPoint point
			); 
		virtual void OnSetRGB(
			COLORREF clr
			);
		virtual void OnSetHLS(
			double hue,
			double luminance,
			double saturation
			);
	protected:
		virtual void GenerateSlider(
			CDC &dc,
			CPalette * pPalette
			);
		virtual void GeneratePicker(
			CDC &dc,
			CPalette * pPalette
			);
		COLORREF CalcColorPicker( int nX, int nY );
		COLORREF CalcColorSlider( int nZ );
		void CalcPoints(
			int &nX,
			int &nY,
			int &nZ,
			bool bDcScale
			);
		void RecalcByZ(
			int nZ
			);
	}; // struct xyz_plain_data_t

	struct __PROF_UIS_API diff_data_t : public grayscale_mixer_data_t
	{
		diff_data_t()
		{
		};
		~diff_data_t()
		{
		};
		virtual void OnInit(
			CDC & dc,
			CPalette * pPalette,
			CSize sizeTotal,
			bool bOnlySizeChanged
			);
		virtual void OnDraw(
			CDC & dc,
			CPalette * pPalette
			);
		virtual void OnSetRGB(
			COLORREF clr
			);
		virtual void OnSetHLS(
			double hue,
			double luminance,
			double saturation
			);
	private:
		void Generate( bool bOnlySizeChanged );
	}; // struct diff_data_t
	
public:
	selection_colors_t m_selClrs;

private:
	cell_based_data_t m_hexagon_data;
	grayscale_mixer_data_t m_grayscale_mixer_data;
	rgb_mixer_data_t m_rgb_mixer_data;
	rgb_cube_data_t m_rgb_cube_data;
	cmy_mixer_data_t m_cmy_mixer_data;
	cmy_cube_data_t m_cmy_cube_data;
	hsl_panel_data_t m_hsl_panel_data;
	hsl_basic_hue_data_t m_hsl_basic_hue_data;
	hsl_roller_data_t m_hsl_roller_data;
	xyz_plain_data_t m_xyz_r;
	xyz_plain_data_t m_xyz_g;
	xyz_plain_data_t m_xyz_b;
	xyz_plain_data_t m_xyz_c;
	xyz_plain_data_t m_xyz_m;
	xyz_plain_data_t m_xyz_y;
	diff_data_t m_diff_data;
	any_picker_data_t * m_algorithms[MODE_VAL_MAX+1];

// Operations
public:
	void SetupColor(
		COLORREF clr,
		bool bNotifyChanged
		);
	void SetupOldColor( // suggested by Olaf Baeyens
		COLORREF clr
		);

	// get / set mode
	operator e_mode_t() const
	{
		return m_eMode;
	};
	CExtColorCtrl & operator=( e_mode_t eMode )
	{
		ASSERT(
			MODE_VAL_MIN <= eMode
			&& eMode <= MODE_VAL_MAX
			);
		_Cleanup();
		m_eMode = eMode;
		_Init( false );
		Invalidate();
		UpdateWindow();
		return *this;
	};
private:
	void _Cleanup();
	void _Init( bool bOnlySizeChanged );
	void _MouseTracking( bool bStart = true );

public:
	static UINT g_nMsgColorChanged;
protected:
	virtual void NotifyColorChanged();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtColorCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtColorCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtColorCtrl)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __EXTCOLORCTRL_H
