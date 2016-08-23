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

#if (!defined __EXT_FUNNY_DLG_BASE_H)
	#include <exdlgbase.h>
#endif

#pragma warning (disable : 4244)

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _TNID_1 221
#define _TNID_2 222

#define __COLOR_ACTIVE_TEXT__ RGB(255,255,255)
#define __COLOR_ACTIVE_SYSBUTTON__ RGB(255,64,64)
#define __COLOR_INACTIVE_SYSBUTTON__ RGB(196,196,196)
#define __COLOR_ACTIVE_SYSBUTTONSHADOW__ RGB(128,128,128)
#define __COLOR_INACTIVE_SYSBUTTONSHADOW__ RGB(0,0,0)

#define __MAZE_BUFFER_DX 64
#define __MAZE_BUFFER_DY 64
#define __MAZE_BUFFER_SIZE \
	(__MAZE_BUFFER_DX*__MAZE_BUFFER_DY)

#define __PI__ (3.141529)

static bool bWaveInitialized = false;
static unsigned char m_pMazeBuffer[__MAZE_BUFFER_SIZE];
static char wavetable[256];
static char spd1 = 1, spd2 = 2, spd3 = 3, spd4 = 4;
static char pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0;
static char tpos1, tpos2, tpos3, tpos4;
static unsigned char sp_pal[(256*3)];

static void mapPalColor(
	int val,
	unsigned char r,
	unsigned char g,
	unsigned char b
	)
{
   sp_pal[val*3]  =r;
   sp_pal[val*3+1]=g;
   sp_pal[val*3+2]=b;
}

static void genWavePal(
	float redFx,
	float greenFx,
	float blueFx
	)
{
   for(int x=0; x<64; x++)
   {
      mapPalColor(
		 x,
         UCHAR( (int) ( ((float)x) * 4 * redFx   ) ),
         UCHAR( (int) ( ((float)x) * 4 * greenFx ) ),
         UCHAR( (int) ( ((float)x) * 4 * blueFx  ) )
		 );
      mapPalColor(
		 127 - x,
         UCHAR( (int) ( ((float)x) * 4 * redFx   ) ),
         UCHAR( (int) ( ((float)x) * 4 * greenFx ) ),
         UCHAR( (int) ( ((float)x) * 4 * blueFx  ) )
		 );
   }
}

/////////////////////////////////////////////////////////////////////////////
// CExtDlgBase dialog

CExtDlgBase::CExtDlgBase(
	UINT __IDD,
	CWnd* pParent /*=NULL*/
	)
	: CExtResizableDialog(
		__IDD,
		pParent
		)
{
	//{{AFX_DATA_INIT(CExtDlgBase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bClose = m_bSize = false;
	m_bEnableResizing = true;

	if(!bWaveInitialized)
	{
		bWaveInitialized = TRUE;
		// initialize buffer wavetable
		memset(
			m_pMazeBuffer,
			0,
			__MAZE_BUFFER_SIZE
			);

		// initialize maze wavetable
		for(int x=0; x<256; x++)
			wavetable[x] = (char)
				(
				120.0 * (1.0+sin( ((double)x)*2*__PI__/256) )
				);

		// initialize maze palette
		memset(
			(void *)&sp_pal,
			0,
			sizeof(sp_pal)
			);
		genWavePal( (float)0.5, (float)0.0, (float)0.7 );
	} // if(!bWaveInitialized)

	//genWavePal( (float)0.4, (float)1.0, (float)1.5 );
	//genWavePal( (float)0.100, (float)0.100, (float)0.430 );

NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));
	ncm.lfMessageFont.lfWeight = 900;
	m_CoolFont.CreateFontIndirect(&(ncm.lfMessageFont));
	ASSERT(m_CoolFont.GetSafeHandle() != NULL);
}


void CExtDlgBase::DoDataExchange(CDataExchange* pDX)
{
	CExtResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtDlgBase)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtDlgBase, CExtResizableDialog)
	//{{AFX_MSG_MAP(CExtDlgBase)
	ON_WM_NCHITTEST()
	ON_WM_NCACTIVATE()
	ON_WM_NCPAINT()
	ON_WM_NCMOUSEMOVE()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtDlgBase message handlers

UINT CExtDlgBase::__DoNcHitTest(CPoint point)
{
	if(!m_bEnableResizing)
		return HTCLIENT;

CRect crClose,crSize;
	__CalcRect(
		crClose,
		crSize
		);
CRect rcBar;
	GetWindowRect(rcBar);
	point -= rcBar.TopLeft();
	if(crClose.PtInRect(point))
		return HTCLOSE;
	else
	{
		if(crSize.PtInRect(point))
			return HTZOOM;
		else
			return HTCLIENT;
	}
}

UINT CExtDlgBase::OnNcHitTest(CPoint point) 
{
UINT nHitTest = __DoNcHitTest(point);
	switch(nHitTest)
	{
	case HTCLOSE:
	case HTZOOM:
		return nHitTest;
	break;
	default:
		return CExtResizableDialog::OnNcHitTest(point);
	break;
	} // switch(nHitTest)
}

BOOL CExtDlgBase::OnNcActivate(BOOL bActive) 
{
BOOL retVal =
		CExtResizableDialog::OnNcActivate(bActive);
	__DoNcPaint();
	return retVal;
}

static void stat_gradient_fill_rect(
	CDC * pDC,
	CRect rect,
	COLORREF clrCptn,
	COLORREF clrCptnRight,
	BOOL bHorz=FALSE,
	int nShift = 8 // this will make 2^6 = 64 fountain steps
	)
{
	ASSERT(pDC!=NULL);
int nSteps = 1 << nShift;
	for (int i = 0; i < nSteps; i++)
	{
		// do a little alpha blending
		int nR = (GetRValue(clrCptn) * (nSteps - i) +
				  GetRValue(clrCptnRight) * i) >> nShift;
		int nG = (GetGValue(clrCptn) * (nSteps - i) +
				  GetGValue(clrCptnRight) * i) >> nShift;
		int nB = (GetBValue(clrCptn) * (nSteps - i) +
				  GetBValue(clrCptnRight) * i) >> nShift;

		COLORREF cr = RGB(nR, nG, nB);

		// then paint with the resulting color
		CRect r2 = rect;
		if (bHorz)
		{
			r2.bottom = rect.bottom - 
				((i * rect.Height()) >> nShift);
			r2.top = rect.bottom - 
				(((i + 1) * rect.Height()) >> nShift);
			if (r2.Height() > 0)
				pDC->FillSolidRect(r2, cr);
		}
		else
		{
			r2.left = rect.left + 
				((i * rect.Width()) >> nShift);
			r2.right = rect.left + 
				(((i + 1) * rect.Width()) >> nShift);
			if (r2.Width() > 0)
				pDC->FillSolidRect(r2, cr);
		}
	}
}

// gradient defines (if not already defined)
#ifndef COLOR_GRADIENTACTIVECAPTION
#define COLOR_GRADIENTACTIVECAPTION     27
#define COLOR_GRADIENTINACTIVECAPTION   28
#define SPI_GETGRADIENTCAPTIONS         0x1008
#endif

void CExtDlgBase::__DoNcPaint(void)
{
// get window DC that is clipped to the non-client area
CWindowDC dc(this);
CRect rcClient, rcBar;
	GetClientRect(rcClient);
	ClientToScreen(rcClient);
	GetWindowRect(rcBar);
	rcClient.OffsetRect(-rcBar.TopLeft());
	rcBar.OffsetRect(-rcBar.TopLeft());

CRect crClose,crSize;
	__CalcRect(
		crClose,
		crSize
		);

CDC mdc;
	mdc.CreateCompatibleDC(&dc);
    
CBitmap bm;
	bm.CreateCompatibleBitmap(&dc, rcBar.Width(), rcBar.Height());
CBitmap* pOldBm = mdc.SelectObject(&bm);
	// client area is not our bussiness :)
	dc.ExcludeClipRect(rcClient);
CFont * pOldFont = mdc.SelectObject(&m_CoolFont);
int old_bk_mode = mdc.SetBkMode(TRANSPARENT);

	// draw things
COLORREF clr3DText = g_PaintManager->GetColor(COLOR_BTNTEXT);
COLORREF clr3DFace = g_PaintManager->GetColor(COLOR_3DFACE);
COLORREF clr3DLight = g_PaintManager->GetColor(COLOR_3DHIGHLIGHT);
COLORREF clr3DShadow = g_PaintManager->GetColor(COLOR_3DSHADOW);
COLORREF clr3DDkShadow = g_PaintManager->GetColor(COLOR_3DDKSHADOW);
//COLORREF clrInfoBk = g_PaintManager->GetColor(COLOR_INFOBK);
COLORREF old_text_color = mdc.SetTextColor(
		(GetActiveWindow() == this)?
			__COLOR_ACTIVE_TEXT__ //clr3DLight
			:
			clr3DText
		);

CRect rcDraw = rcBar;
	// erase nc background
	mdc.FillSolidRect(rcDraw,clr3DFace);
	mdc.Draw3dRect(rcDraw,clr3DShadow,clr3DDkShadow);
	rcDraw.DeflateRect(1,1);
	mdc.Draw3dRect(rcDraw,clr3DLight,clr3DShadow);

	// paint caption
	rcDraw.DeflateRect(1,1);
	rcDraw.bottom = rcClient.top;

	__Maze_Paint(mdc,rcDraw);

	// paint text and icon on caption
HICON hIcon = GetIcon(FALSE);
CRect crText(rcDraw);
	crText.right = crSize.left - 4;
	crText.right -= 16;
	crText.left += 4;
	if(hIcon != NULL)
	{
		crText.left += 16 + 2;
		CRect crIcon(crText);
		crIcon.OffsetRect(-16-2,2);
		crIcon.right = crIcon.left+16;
		crIcon.bottom = crIcon.top+16;
//		mdc.DrawIcon(crIcon.TopLeft(),hIcon);
		::DrawIconEx(
			mdc.GetSafeHdc(),
			crIcon.left,
			crIcon.top,
			hIcon,
			crIcon.Width(),
			crIcon.Height(),
			0,
			(GetActiveWindow() == this) ?
				(HBRUSH)::GetStockObject(NULL_BRUSH)
				:
				g_PaintManager->GetBrush(COLOR_BTNFACE)
				,
			DI_IMAGE
			);
	} // if(hIcon != NULL)

	if(crText.right > crText.left)
	{
		CString str_window_text( _T("") );
		GetWindowText( str_window_text );
		mdc.DrawText(
			str_window_text,
			str_window_text.GetLength(),
			crText,
			DT_SINGLELINE
				|DT_LEFT|DT_VCENTER
				|DT_END_ELLIPSIS
			);
	} // if(crText.right > crText.left)

	if(m_bEnableResizing)
	{
		// paint system buttons
		CFont font;
		int ppi = dc.GetDeviceCaps(LOGPIXELSX);
		int pointsize = MulDiv(
			60, //(crClose.Height()-4)*10, // 60-6 points at 96 ppi
			96,
			ppi
			);
		font.CreatePointFont(pointsize, _T("Marlett"));
		mdc.SelectObject(&font);
		//	mdc.FillSolidRect(crClose,clr3DShadow);
		//	mdc.FillSolidRect(crSize,clr3DShadow);
		//	1-maximize 2-restore r-cross
		char chrSize[2] = "1";
		char chrClose[2] = "r";
		WINDOWPLACEMENT wp;
		memset((void *)&wp,0,sizeof(WINDOWPLACEMENT));
		wp.length = sizeof(WINDOWPLACEMENT);
		if(GetWindowPlacement(&wp))
		{
			if(wp.showCmd == SW_SHOWMAXIMIZED)
				chrSize[0] = '2';
		}

		mdc.SetTextColor(
			m_bSize ?
				__COLOR_ACTIVE_SYSBUTTONSHADOW__ // clr3DShadow
				:
				__COLOR_INACTIVE_SYSBUTTONSHADOW__ // clr3DDkShadow
			);
		mdc.DrawText(
			chrSize,crSize,DT_SINGLELINE|DT_CENTER|DT_VCENTER
			);
		crSize.OffsetRect(-1,-1);
		mdc.SetTextColor(
			m_bSize ?
				__COLOR_ACTIVE_SYSBUTTON__ // clr3DLight
				:
				__COLOR_INACTIVE_SYSBUTTON__ // clr3DFace
			);
		mdc.DrawText(
			chrSize,crSize,DT_SINGLELINE|DT_CENTER|DT_VCENTER
			);

		mdc.SetTextColor(
			m_bClose ?
				__COLOR_ACTIVE_SYSBUTTONSHADOW__ // clr3DShadow
				:
				__COLOR_INACTIVE_SYSBUTTONSHADOW__ // clr3DDkShadow
			);
		mdc.DrawText(
			chrClose,crClose,DT_SINGLELINE|DT_CENTER|DT_VCENTER
			);
		crClose.OffsetRect(-1,-1);
		mdc.SetTextColor(
			m_bClose ?
				__COLOR_ACTIVE_SYSBUTTON__ // clr3DLight
				:
				__COLOR_INACTIVE_SYSBUTTON__ // clr3DFace
			);
		mdc.DrawText(
			chrClose,crClose,DT_SINGLELINE|DT_CENTER|DT_VCENTER
			);
	} // if(m_bEnableResizing)


	// finish this things!
	if(pOldFont)
		mdc.SelectObject(pOldFont);
	mdc.SetTextColor(old_text_color);
	mdc.SetBkMode(old_bk_mode);
	dc.BitBlt(0, 0, rcBar.Width(), rcBar.Height(), &mdc, 0, 0, SRCCOPY);
	ReleaseDC(&dc);
	mdc.SelectObject(pOldBm);
	bm.DeleteObject();
	mdc.DeleteDC();
}

void CExtDlgBase::OnNcPaint() 
{
//	CExtResizableDialog::OnNcPaint();
	__DoNcPaint();
}

void CExtDlgBase::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	CExtResizableDialog::OnNcMouseMove(nHitTest, point);

CRect crClose,crSize;
	__CalcRect(
		crClose,
		crSize
		);
CRect rcBar;
	GetWindowRect(rcBar);
	point -= rcBar.TopLeft();

bool bDoNcPaint = false;
	if(crSize.PtInRect(point))
	{
		if(!m_bSize)
		{
			m_bSize = true;
			bDoNcPaint = true;
		}
	}
	else
	{
		if(m_bSize)
		{
			m_bSize = false;
			bDoNcPaint = true;
		}
	}

	if(crClose.PtInRect(point))
	{
		if(!m_bClose)
		{
			m_bClose = true;
			bDoNcPaint = true;
		}
	}
	else
	{
		if(m_bClose)
		{
			m_bClose = false;
			bDoNcPaint = true;
		}
	}
	if(bDoNcPaint)
		__DoNcPaint();

	SetTimer(_TNID_2,100,NULL);
}

void CExtDlgBase::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	CExtResizableDialog::OnNcLButtonUp(nHitTest, point);
	__DoNcPaint();
}

void CExtDlgBase::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	if(nHitTest == HTZOOM)
	{
		WINDOWPLACEMENT wp;
		memset((void *)&wp,0,sizeof(WINDOWPLACEMENT));
		wp.length = sizeof(WINDOWPLACEMENT);
		if(GetWindowPlacement(&wp))
		{
			if(wp.showCmd == SW_SHOWMAXIMIZED)
				ShowWindow(SW_RESTORE);
			else
				ShowWindow(SW_SHOWMAXIMIZED);
		}
		return;
	}
	if(nHitTest == HTCLOSE)
	{
		//OnCancel();
		//ShowWindow(SW_HIDE);
		PostMessage(WM_COMMAND,IDCANCEL);
		return;
	}
	CExtResizableDialog::OnNcLButtonDown(nHitTest, point);
	__DoNcPaint();
}

void CExtDlgBase::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case _TNID_1:
		if(!IsWindowVisible())
			__Maze_Enable(FALSE);
		else
			__Maze_Step();
	break;
	case _TNID_2:
	{
		if(!IsWindowVisible())
		{
			KillTimer(_TNID_2);
			m_bClose = m_bSize = false;
		}
		if(m_bClose || m_bSize)
		{
			CRect crClose,crSize;
			__CalcRect(
				crClose,
				crSize
				);
			CPoint point;
			if( ! ::GetCursorPos(&point) )
				return;
			CRect rcBar;
			GetWindowRect(rcBar);
			point -= rcBar.TopLeft();
			if(!crClose.PtInRect(point) && !crSize.PtInRect(point))
			{
				KillTimer(_TNID_2);
				m_bClose = m_bSize = false;
				__DoNcPaint();
			}
		}
		else
		{
			KillTimer(_TNID_2);
			m_bClose = m_bSize = false;
			__DoNcPaint();
		}
	}
	break;
	default:
		CExtResizableDialog::OnTimer(nIDEvent);
	break;
	}; // switch(nIDEvent)
}

void CExtDlgBase::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CExtResizableDialog::OnShowWindow(bShow, nStatus);

	m_bClose = m_bSize = false;
	__Maze_Enable(bShow);
}

void CExtDlgBase::__Maze_Init(void)
{
	if(!__Maze_IsOsAllowed())
		return;

//	__Maze_Done();
	if(!GetSafeHwnd())
		return;

	if(m_bmpMaze.GetSafeHandle() == NULL)
	{
		CWindowDC dc(this);
		m_bmpMaze.CreateCompatibleBitmap(
			&dc,
			__MAZE_BUFFER_DX,
			__MAZE_BUFFER_DY
			);
		ASSERT(m_bmpMaze.GetSafeHandle() != NULL);
		m_dcMaze.CreateCompatibleDC(&dc);
		ReleaseDC(&dc);
	} // if(m_bmpMaze.GetSafeHandle() == NULL)
}

void CExtDlgBase::__Maze_Done(void)
{
	if(!__Maze_IsOsAllowed())
		return;
	if(m_bmpMaze.GetSafeHandle() != NULL)
	{
		VERIFY( m_bmpMaze.DeleteObject() );
		VERIFY( m_dcMaze.DeleteDC() );
	}
}

void CExtDlgBase::__Maze_Enable(BOOL bEnable)
{
	if(bEnable)
		SetTimer(_TNID_1,100,NULL);
	else
		KillTimer(_TNID_1);
	__DoNcPaint();
}

void CExtDlgBase::__Maze_Step(void)
{
	if(!IsWindowVisible())
	{
		__Maze_Enable(FALSE);
		return;
	} // if(!IsWindowVisible())

	if(__Maze_IsOsAllowed())
	{
		int  v,x,y;
		char z;
		v=0;
		tpos1=pos1;
		tpos2=pos2;
		for(x=0; x < __MAZE_BUFFER_DY; x++)
		{
			tpos3=pos3;
			tpos4=pos4;
			for(y=0; y < __MAZE_BUFFER_DX; y++)
			{
				z =
					char(
						wavetable[tpos1]
						+ wavetable[tpos2]
						+ wavetable[tpos3]
						+ wavetable[tpos4]
						);
				m_pMazeBuffer[ v++ ] = UCHAR( z&0x7F );
				tpos3 += 1;
				tpos4 += 3;
			} // for(y=0; y < __MAZE_BUFFER_DX; y++)
			tpos1 += 2;
			tpos2 += 1;
		} // for(x=0; x < __MAZE_BUFFER_DY; x++)
		pos1 += spd1;
		pos2 -= spd2;
		pos3 += spd3;
		pos4 -= spd4;
	} // if(__Maze_IsOsAllowed())
	
	__DoNcPaint();
}

void CExtDlgBase::__Maze_Paint(
	CDC & dc,
	CRect & rcDraw
	)
{
	if(!IsWindowVisible())
		return;

	if(!__Maze_IsOsAllowed())
	{
		if(GetActiveWindow() == this)
		{
			COLORREF clrCptn =
				g_PaintManager->GetColor(COLOR_ACTIVECAPTION);
			COLORREF clrCptnRight =
				g_PaintManager->GetColor(COLOR_GRADIENTACTIVECAPTION);
			stat_gradient_fill_rect(
				&dc,
				rcDraw,
				clrCptn,
				clrCptnRight,
				FALSE,
				8
				);
			return;
		}
	}

	if(		(m_bmpMaze.GetSafeHandle() == NULL)
			|| (GetActiveWindow() != this)
			|| (!__Maze_IsOsAllowed())
			)
	{
		dc.FillSolidRect(
			rcDraw,
			g_PaintManager->GetColor(COLOR_3DFACE)
			);
		return;
	}

//int width = rcDraw.Width();
int height = rcDraw.Height();
	if(height > __MAZE_BUFFER_DY)
		dc.FillSolidRect(
			rcDraw,
			RGB(0,0,0)
			);

CBitmap * pOldBm = m_dcMaze.SelectObject(&m_bmpMaze);

int v=0;
	for(int x=0; x < __MAZE_BUFFER_DX; x++)
	{
		for(int y=0; y < __MAZE_BUFFER_DY; y++)
		{
			m_dcMaze.SetPixel(
				x,
				y,
				//RGB(m_pMazeBuffer[v],0,0)
				RGB(
					sp_pal[(m_pMazeBuffer[v])*3],
					sp_pal[(m_pMazeBuffer[v])*3+1],
					sp_pal[(m_pMazeBuffer[v])*3+2]
					)
				);
			v++;
		}
	}

/*
	dc.BitBlt(
		rcDraw.left, // 0,
		rcDraw.top, // 0,
		rcDraw.Width(), // __MAZE_BUFFER_DX,
		height, // __MAZE_BUFFER_DY,
		&m_dcMaze,
		0,
		0,
		SRCCOPY
		);

	dc.StretchBlt (
		rcDraw.left,
		rcDraw.top,
		rcDraw.Width(),
		height,
		&m_dcMaze,
		0,
		0,
		__MAZE_BUFFER_DX,
		__MAZE_BUFFER_DY,
		SRCCOPY
		);
*/

//CRect rcOldClipRect;
//int nObtainedClipBoxResult = dc.GetClipBox(&rcOldClipRect);

	dc.IntersectClipRect(rcDraw);

int rr = rcDraw.right; //-2;
	for(x=rcDraw.left; x<rr; x+=__MAZE_BUFFER_DX)
	{
		int x2 = x+__MAZE_BUFFER_DX;
		if(x2 > rr)
			x2 = rr;
		dc.BitBlt(
			x,
			rcDraw.top,
			x2,
			height,
			&m_dcMaze,
			0,
			0,
			SRCCOPY
			);
		/*
		dc.StretchBlt (
			x,
			rcDraw.top,
			x2,
			height,
			&m_dcMaze,
			0,
			0,
			__MAZE_BUFFER_DX,
			__MAZE_BUFFER_DY,
			SRCCOPY
			);
		*/
	}

	if(pOldBm)
		m_dcMaze.SelectObject(pOldBm);
}

void CExtDlgBase::__CalcRect(
	CRect & crClose,
	CRect & crSize
	)
{
CRect rcClient, rcBar;
	GetClientRect(rcClient);
	ClientToScreen(rcClient);
	GetWindowRect(rcBar);
	rcClient.OffsetRect(-rcBar.TopLeft());
	rcBar.OffsetRect(-rcBar.TopLeft());

int h = rcClient.top-rcBar.top;
	crClose.SetRect(
		rcBar.right - h,
		rcBar.top,
		rcBar.right,
		rcClient.top
		);
	crClose.DeflateRect(4,4);

	crSize = crClose;
	crSize.OffsetRect(-crClose.Width()-4,0);
}


void CExtDlgBase::PostNcDestroy() 
{
	__Maze_Done();
	CExtResizableDialog::PostNcDestroy();
}

BOOL CExtDlgBase::OnInitDialog() 
{
	CExtResizableDialog::OnInitDialog();
	
	__Maze_Init();

	ShowSizeGrip(m_bEnableResizing);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExtDlgBase::OnSize(UINT nType, int cx, int cy) 
{
	CExtResizableDialog::OnSize(nType, cx, cy);
	
	__Maze_Init();
}

void CExtDlgBase::EnableResizing(bool bEnable /*= true*/ )
{
	m_bEnableResizing = bEnable;
	if(GetSafeHwnd() != NULL)
	{
		ShowSizeGrip(m_bEnableResizing);
		__DoNcPaint();
	} // if(GetSafeHwnd() != NULL)
};

bool CExtDlgBase::__Maze_IsOsAllowed(void)
{
	return true;
}

void CExtDlgBase::OnKillFocus(CWnd* pNewWnd) 
{
	CExtResizableDialog::OnKillFocus(pNewWnd);
	
//	__Maze_Enable(FALSE);
	
}
