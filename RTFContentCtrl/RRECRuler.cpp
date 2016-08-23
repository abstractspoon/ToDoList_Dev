/* ==========================================================================
	File :			RRECRuler.cpp

	Class :			CRRECRuler

	Author :		Johan Rosengren, Abstrakt Mekanik AB
					Iain Clarke

	Date :			2004-04-19

	Purpose :		This class encapsulates a ruler that can be used with 
					"CRulerRichEditCtrl". The class is derived from "CWnd", and 
					draws a ruler. 

	Description :	A plain "CWnd"-derived class. The mouse messages 
					"WM_LBUTTONDOWN", "WM_MOUSEMOVE" and "WM_LBUTTONUP" are handled, 
					and a registered message is sent to the control parent.	

	Usage :			Only tested with "CRulerRichEditCtrl". Add a member to the 
					parent class, and create it with "Create". Handle the 
					registered message "urm_RULERACTION" in the parent class. 
					The parent class is also expected to handle 
					"urm_GETSCROLLPOS", which should return the current 
					horisontal scrollbar position.

   ========================================================================*/

#include "stdafx.h"
#include "RRECRuler.h"
#include "ids.h"
#include "..\shared\themed.h"
#include "..\shared\misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRRECRuler::CRRECRuler()
/* ============================================================
	Function :		CRRECRuler::CRRECRuler
	Description :	constructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
	
	m_physicalInch = 0;
	m_mode = Misc::IsMetricMeasurementSystem() ? MODE_METRIC : MODE_INCH; 	m_margin = 0;
	m_crBack = GetSysColor(COLOR_3DFACE);
}

CRRECRuler::~CRRECRuler()
/* ============================================================
	Function :		CRRECRuler::~CRRECRuler
	Description :	destructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

BEGIN_MESSAGE_MAP(CRRECRuler, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ENABLE()
END_MESSAGE_MAP()


BOOL CRRECRuler::Create( const CRect& rect, CWnd* parent, UINT id )
/* ============================================================
	Function :		CRRECRuler::Create
	Description :	Creates the ruler control
	Access :		Public
					
	Return :		BOOL				-	Result, "TRUE" if ok.
	Parameters :	const CRect& rect	-	Coordinate rect
					CWnd* parent		-	Parent
					UINT id				-	Child window id.
					
	Usage :			Call to create the ruler control

   ============================================================*/
{
	BOOL result = CWnd::Create( NULL, NULL, WS_CHILD | WS_VISIBLE, rect, parent, id );
	if( result )
	{
		CClientDC dc( this );
		m_physicalInch = dc.GetDeviceCaps( LOGPIXELSX );
	}

	return result;

}

void CRRECRuler::OnPaint()
/* ============================================================
	Function :		CRRECRuler::OnPaint
	Description :	Handles the "WM_PAINT" message from Windows.
					Draws the ruler to a memory "CDC" that is 
					blitted to the screen (double buffering)
	Access :		Protected
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC

   ============================================================*/
{
	CPaintDC mainDC(this);

	// Set up data
	int pos = GetParent()->SendMessage( urm_GETSCROLLPOS );

	CRect rect;
	GetClientRect( rect );

	// Create off-screen canvas
	CDC dc;
	CBitmap bmp;
	bmp.CreateCompatibleBitmap( &mainDC, rect.Width(), rect.Height() );
	dc.CreateCompatibleDC( &mainDC );
	CBitmap* oldbmp = dc.SelectObject( &bmp );

	dc.FillSolidRect( rect, m_crBack);

	// Set up canvas
	BOOL bEnabled = IsWindowEnabled();

	COLORREF crBack = GetSysColor(bEnabled ? COLOR_WINDOW : COLOR_3DFACE);
	COLORREF crText = GetSysColor(bEnabled ? COLOR_WINDOWTEXT : COLOR_3DSHADOW);

	// Set up data for the inner ruler window
	CRect winRect( rect );
	winRect.top += 3;
	winRect.bottom -= 5;
	winRect.right -= 3;
	winRect.left += m_margin - 2;

	if (bEnabled) // else same as dialog bk color
		dc.FillSolidRect( winRect, GetSysColor(COLOR_WINDOW));

	// Frame the inside
	CThemed th(this, _T("EDIT"));

	if (th.AreControlsThemed()) 
	{
		winRect.InflateRect(0, 1, 0, 1);

		int nState = ETS_NORMAL;

		if (!bEnabled)
			nState = ETS_DISABLED;
		else if (GetStyle() & ES_READONLY)
			
			nState = ETS_READONLY;

		th.DrawBackground(&dc, EP_EDITTEXT, nState, winRect);

		// get the theme bk color for the text
		crBack = th.GetThemeColor(EP_EDITTEXT, nState, TMT_FILLCOLOR);
	}
	else
	{
		dc.Draw3dRect(winRect, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DHILIGHT));
		winRect.DeflateRect(1, 1);
		dc.Draw3dRect(winRect, ::GetSysColor(COLOR_3DDKSHADOW), ::GetSysColor(COLOR_3DFACE));
		winRect.InflateRect(1, 1);
	}

	int midpoint = winRect.top + ( winRect.Height() / 2 );
	int leftmarg = winRect.left + 2 - pos;
	int width = winRect.Height();
	int t;

	// Print the values in the ruler scale
	dc.SetBkColor(crBack);
	dc.SetTextColor(crText);
	dc.SelectStockObject( BLACK_PEN );
	dc.SelectStockObject( DEFAULT_GUI_FONT );
	
	if( m_mode == MODE_INCH )
	{
		int inch4 = ( int ) ( ( double ) m_physicalInch / 4.0 +.5 );
		int inch8 = ( int ) ( ( double ) m_physicalInch / 8.0 +.5 );

		// Drawing scale markers
		for( t = ( leftmarg + ( int ) ( inch8 +.5 ) ) ; t < rect.right - m_margin ; t += ( int ) ( inch8 + .5 ) )
		{
			dc.MoveTo( t, midpoint - 1 );
			dc.LineTo( t, midpoint + 1 );
		}

		for( t = leftmarg + inch4 ; t < rect.right - m_margin ; t += inch4 )
		{
			dc.MoveTo( t, midpoint - 3 );
			dc.LineTo( t, midpoint + 3 );
		}

		CRect rectInch;
		CString counter;
		int count = 1;

		// Drawing numbers
		for( t = leftmarg + m_physicalInch ; t < rect.right - m_margin ; t += m_physicalInch )
		{

			rectInch.SetRect( t - width / 2, winRect.top + 2, t + width / 2, winRect.bottom - 2 );
			counter.Format( _T( "%i" ), count );
			dc.DrawText( counter, rectInch, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
			count++;

		}
	}
	else
	{
		int cm = ( int ) ( ( double ) m_physicalInch / 2.54 + .5 );
		int cm2 = ( int ) ( ( double ) cm / 2.0 );

		// Drawing scale markers
		for( t = leftmarg + cm2 ; t < rect.right - m_margin ; t += cm2 )
		{
			dc.MoveTo( t, midpoint - 1 );
			dc.LineTo( t, midpoint + 2 );
		}

		CRect rectNum;
		CString counter;
		int count = 1;

		// Drawing numbers
		for( t = leftmarg + cm ; t < rect.right - m_margin ; t += cm )
		{
			rectNum.SetRect( t - width / 2, winRect.top + 2, t + width / 2, winRect.bottom - 2 );
			counter.Format( _T( "%i" ), count );
			dc.DrawText( counter, rectNum, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
			count++;

		}
	}

	// Draw tab markers
	int max = m_tabs.GetSize();
	for( t = 0 ; t < max ; t++ )
	{

		int x = ( leftmarg + m_tabs[ t ] - 2 );
		if( x > winRect.left && x + 3 < winRect.right )
		{
			dc.MoveTo( x, midpoint + 5 );
			dc.LineTo( x + 6, midpoint + 5 );
			dc.MoveTo( x, midpoint + 6 );
			dc.LineTo( x + 6, midpoint + 6 );

			dc.MoveTo( x + 2, midpoint + 7 );
			dc.LineTo( x + 2, midpoint + 10 );
			dc.MoveTo( x + 3, midpoint + 7 );
			dc.LineTo( x + 3, midpoint + 10 );
		}
	}

	//... and out to screen
	mainDC.BitBlt( 0, 0, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY );
	dc.SelectObject( oldbmp );
}

BOOL CRRECRuler::OnEraseBkgnd( CDC* /*pDC*/ ) 
/* ============================================================
	Function :		CRRECRuler::OnEraseBkgnd
	Description :	Returns "TRUE" to avoid flicker.
	Access :		Protected
					
	Return :		BOOL		-	Always "TRUE"
	Parameters :	CDC* pDC	-	Not used
					
	Usage :			Called from MFC. 

   ============================================================*/
{
	
	return TRUE;

}

void CRRECRuler::SetMode( int mode )
/* ============================================================
	Function :		CRRECRuler::SetMode
	Description :	Sets the internal mode, that is, if the 
					ruler should display inches or centimeters.
	Access :		Public
					
	Return :		void
	Parameters :	int mode	-	Mode to use, "MODE_INCH" or 
									"MODE_METRIC" (default)
					
	Usage :			Call to change the mode.

   ============================================================*/
{

	m_mode = mode;

	if( m_hWnd )
	{

		RedrawWindow();

	}

}

void CRRECRuler::SetMargin( int margin )
/* ============================================================
	Function :		CRRECRuler::SetMargin
	Description :	Sets the margin, in pixels, of the ruler.
	Access :		Public
					
	Return :		void
	Parameters :	int margin	-	The margin to set
					
	Usage :			Call to set the margin of the ruler scale.

   ============================================================*/
{

	m_margin = margin;

	if( m_hWnd )
	{

		RedrawWindow();

	}

}

int CRRECRuler::GetMode() const
/* ============================================================
	Function :		CRRECRuler::GetMode
	Description :	Gets the mode, that is, either "MODE_INCH" or 
					"MODE_METRIC", that is used to draw the ruler.
	Access :		Public
					
	Return :		int		-	The mode, either "MODE_INCH" or 
								"MODE_METRIC"
	Parameters :	none

	Usage :			Call to get the current mode.

   ============================================================*/
{

	return m_mode;

}

void CRRECRuler::SetTabStops( const CDWordArray& arr )
/* ============================================================
	Function :		CRRECRuler::SetTabStops
	Description :	Sets the tab stops in the internal tab stop 
					array, in device pixels
	Access :		Public
					
	Return :		void
	Parameters :	const CDWordArray& arr	-	The array to 
												copy from
					
	Usage :			Call to set the tab stops of the ruler.

   ============================================================*/
{

	m_tabs.RemoveAll();
	int max = arr.GetSize();
	for ( int t = 0 ; t < max ; t++ )
	{
		m_tabs.Add( arr[ t ] );
	}

	if (GetSafeHwnd())
	{
		Invalidate(FALSE);
		UpdateWindow();
	}
}

void CRRECRuler::OnLButtonDown(UINT nFlags, CPoint point)
/* ============================================================
	Function :		CRRECRuler::OnLButtonDown
	Description :	Handles the "WM_LBUTTONDOWN" message. We send 
					a message with the current cursor position 
					to the parent control.
	Access :		Protected
					
	Return :		void
	Parameters :	UINT nFlags		-	Not used
					CPoint point	-	Cursor position
					
	Usage :			Called from MFC

   ============================================================*/
{

	SetCapture();
	GetParent()->SendMessage( urm_RULERACTION, DOWN, ( LPARAM ) &point );
	CWnd::OnLButtonDown( nFlags,  point );

}

void CRRECRuler::OnLButtonUp(UINT nFlags, CPoint point)
/* ============================================================
	Function :		CRRECRuler::OnLButtonUp
	Description :	Handles the "WM_LBUTTONUP" message. We send 
					a message with the current cursor position 
					to the parent control.
	Access :		Protected
					
	Return :		void
	Parameters :	UINT nFlags		-	Not used
					CPoint point	-	Cursor position
					
	Usage :			Called from MFC

   ============================================================*/
{

	ReleaseCapture();
	GetParent()->SendMessage( urm_RULERACTION, UP, ( LPARAM ) &point );
	CWnd::OnLButtonUp( nFlags,  point );

}

void CRRECRuler::OnMouseMove(UINT nFlags, CPoint point)
/* ============================================================
	Function :		CRRECRuler::OnMouseMove
	Description :	Handles the "WM_MOUSEMOVE" message. We send 
					a message with the current cursor position 
					to the parent control.
	Access :		Protected
					
	Return :		void
	Parameters :	UINT nFlags		-	Not used
					CPoint point	-	Cursor position
					
	Usage :			Called from MFC

   ============================================================*/
{

	GetParent()->SendMessage( urm_RULERACTION, MOVE, ( LPARAM ) &point );
	CWnd::OnMouseMove( nFlags,  point );

}

void CRRECRuler::OnEnable(BOOL bEnable) 
{
	CWnd::OnEnable(bEnable);
	Invalidate(FALSE);
}
