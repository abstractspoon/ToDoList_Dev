// RangeSlider.cpp : implementation file
//
/* Written By and (C) 2002 Jens Scheidtmann

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:


  1. Redistributions of source code must retain the above copyright notice, this list 
     of conditions and the following disclaimer. 

  2. Redistributions in binary form must reproduce the above copyright notice, this list 
     of conditions and the following disclaimer in the documentation and/or other 
     materials provided with the distribution. 

  3. The name of the author may not be used to endorse or promote products derived 
     from this software without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, 
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY 
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.



  alternatively (at your option) you can apply the Gnu Public License V 2 or above (GPL).

*/

#include "stdafx.h"
#include "RangeSlider.h"
#include "MemDC.h"

#include <math.h> // for fabs

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#define RANGESLIDER_CLASSNAME _T("MFCRangeSlider")

const UINT RANGE_CHANGED = 
   ::RegisterWindowMessage(_T("RANGE_CHANGED{2E7439A4-12DB-42a2-BB95-3462B3EC39CE}CRangeSlider"));
/////////////////////////////////////////////////////////////////////////////
// CRangeSlider

CRangeSlider::CRangeSlider()
{
	RegisterWindowClass();
	m_Min = 0.0;
	m_Max = 1.0;
	m_Left = 0.0;
	m_Right = 1.0;
	m_bHorizontal = TRUE;
	m_nArrowWidth = 10;
	m_bTracking = FALSE;
	m_bVisualMinMax = FALSE;
	m_bInvertedMode = FALSE;
	m_Step = -1;
}

CRangeSlider::~CRangeSlider()
{
}

// Relay call to create
void CRangeSlider::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CCreateContext *pContext) 
{
	CWnd::Create(NULL, _T(""), dwStyle, rect, pParentWnd, nID, pContext);
}


// Register the window class, so you can use this class as a custom control.
BOOL CRangeSlider::RegisterWindowClass() 
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, RANGESLIDER_CLASSNAME, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc      = ::DefWindowProc;
		wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
		wndcls.hInstance        = hInst;
		wndcls.hIcon            = NULL;
		wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground    = NULL; // No Background brush (gives flicker)
		wndcls.lpszMenuName     = NULL;
		wndcls.lpszClassName    = RANGESLIDER_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

BEGIN_MESSAGE_MAP(CRangeSlider, CWnd)
	//{{AFX_MSG_MAP(CRangeSlider)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_GETDLGCODE()
	ON_WM_CAPTURECHANGED()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRangeSlider message handlers
void CRangeSlider::OnPaint() 
{
	CPaintDC RawDC(this); // device context for painting
	CMemDC dc(&RawDC);
	dc.SetMapMode(MM_TEXT);

	if (m_bHorizontal) 
	{
		OnPaintHorizontal(dc);
	} 
	else 
	{
		OnPaintVertical(dc);
	}
}

// Paint in Horizontal Mode.
void CRangeSlider::OnPaintHorizontal(CDC &dc) 
{
	ASSERT(m_bHorizontal);

	// Calculate Arrow width and x-axis width.
	CRect ClientRect;
	GetClientRect(&ClientRect);

	// Erase Background.
	DrawRegion(dc, RSDR_BACKGROUND, ClientRect);

	m_nArrowWidth = ((ClientRect.Height() * 3) / 5);
	m_dx = (ClientRect.Width() - (2 * m_nArrowWidth) - 1);

	// Check if [Left, Right] is contained in [Min, Max].
	ASSERT(m_Min <= m_Left);
	ASSERT(m_Left <= m_Right);
	ASSERT(m_Right <= m_Max);

	ClientRect.DeflateRect(1, 1);

	// Position of Left and Right
	CRect rLeft(ClientRect);
	int x1 = static_cast<int>((m_Left - m_Min)/(m_Max - m_Min) * m_dx + 0.5);

	rLeft.right = rLeft.left + x1;
	DrawRegion(dc, RSDR_LEFT, rLeft);

	// Draw Left Arrow
	rLeft.left = rLeft.right;
	rLeft.right += m_nArrowWidth;

	m_RectLeft = rLeft;
	DrawRegion(dc, RSDR_LEFTBUTTON, rLeft);

	// Draw Right Arrow
	CRect rRight(ClientRect);
	int x2 = static_cast<int>((m_Right - m_Min)/(m_Max - m_Min) * m_dx + 0.5);

	rRight.left = x2 + 2 * m_nArrowWidth;
	DrawRegion(dc, RSDR_RIGHT, rRight);

	rRight.right = rRight.left + 1;
	rRight.left -= m_nArrowWidth;

	m_RectRight = rRight;
	DrawRegion(dc, RSDR_RIGHTBUTTON, rRight);
	
	// Draw Area in between.
	CRect rMiddle(ClientRect);
	rMiddle.left = rLeft.right;
	rMiddle.right = rRight.left;

	DrawRegion(dc, RSDR_MIDDLE, rMiddle);

	// Store value for drawing the focus rectangle
	CRect focusRect = rMiddle;

	// Draw Area of InnerMin, Max.
	if (m_bVisualMinMax) 
	{
		ASSERT(m_Min <= m_VisualMax);
		ASSERT(m_VisualMin <= m_VisualMax);
		ASSERT(m_VisualMax <= m_Max);

		// See wether [InnerMin, InnerMax] geschnitten [Left, Right] is non-empty.
		// Then Draw
		if (m_VisualMin < m_Right && m_VisualMax > m_Left) 
		{
			int visualMin = static_cast<int>((m_VisualMin - m_Min)/(m_Max - m_Min) * m_dx + 0.5);
			int visualMax = static_cast<int>((m_VisualMax - m_Min)/(m_Max - m_Min) * m_dx + 0.5);

			// Stay in [m_Left, m_Right] intervall.
			visualMin = max(visualMin + m_nArrowWidth, x1 + m_nArrowWidth);
			visualMax = min(visualMax + m_nArrowWidth, x2 + m_nArrowWidth);

			rMiddle = CRect(visualMin, 1, visualMax, (ClientRect.Height() - 1));

			CBrush br;
			br.CreateSolidBrush(RGB(255,255,150)); // light yellow;
			dc.SelectObject(br);
			dc.SelectStockObject(NULL_PEN);
			dc.Rectangle(rMiddle);
			CPen pen;
			dc.SelectStockObject(HOLLOW_BRUSH);
		}
	}
	
	if (GetFocus() == this) 
	{
		focusRect.DeflateRect(3,3);
		DrawRegion(dc, RSDR_FOCUS, focusRect);
	}
}

void CRangeSlider::DrawRegion(CDC& dc, RS_DRAWREGION nRegion, const CRect& rRegion)
{
	int nSaveDC = dc.SaveDC();

	switch (nRegion)
	{
	case RSDR_BACKGROUND:
		dc.FillSolidRect(rRegion, ::GetSysColor(COLOR_3DFACE));

		if (GetExStyle() && WS_EX_CLIENTEDGE)
		{
			dc.Draw3dRect(rRegion, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHIGHLIGHT));
		}
		else if (GetStyle() && WS_BORDER)
		{
			dc.SelectStockObject(GRAY_BRUSH);
			dc.SelectStockObject(BLACK_PEN);
			dc.Rectangle(rRegion);
		}
		break;

	case RSDR_LEFT:  // RSDR_TOP
	case RSDR_RIGHT: // RSDR_BOTTOM
		dc.FillSolidRect(rRegion, ::GetSysColor(COLOR_SCROLLBAR));
		break;

	case RSDR_LEFTBUTTON: // RSDR_TOPBUTTON
		DrawButton(dc, 
					BUTTON_LEFT,
					rRegion, 
					(m_bHorizontal ? _T("<") : _T("^")));
		break;

	case RSDR_MIDDLE:
		DrawButton(dc, 
					BUTTON_MIDDLE,
					rRegion, 
					_T("..."));
		break;

	case RSDR_RIGHTBUTTON: // RSDR_BOTTOMBUTTON
		DrawButton(dc, 
					BUTTON_RIGHT,
					rRegion, 
					(m_bHorizontal ? _T(">") : _T("V")));
		break;

	case RSDR_FOCUS:
		dc.DrawFocusRect(&rRegion);
		break;
	}

	dc.RestoreDC(nSaveDC);
}

void CRangeSlider::DrawButton(CDC& dc, BUTTON_ID nBtn, const CRect& rButton, const CString& sText)
{
	dc.FillSolidRect(rButton, ::GetSysColor(COLOR_BTNFACE));

	// Make appearance 3D
	if (IsPressed(nBtn))
		dc.DrawFrameControl((LPRECT)&rButton, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_PUSHED);
	else
		dc.DrawFrameControl((LPRECT)&rButton, DFC_BUTTON, DFCS_BUTTONPUSH);

	if (!sText.IsEmpty())
	{
		dc.SetBkMode(TRANSPARENT);

		if (IsPressed(nBtn))
		{
			CRect rText(rButton);
			rText.OffsetRect(1, 1);

			dc.DrawText(sText, (LPRECT)&rText, DT_CENTER | DT_VCENTER);
		}
		else
		{
			dc.DrawText(sText, (LPRECT)&rButton, DT_CENTER | DT_VCENTER);
		}
	}
}

// Paint in Vertical Mode.
void CRangeSlider::OnPaintVertical(CDC &dc) 
{
	ASSERT(!m_bHorizontal);
	CRect ClientRect;
	GetClientRect(&ClientRect);

	int dy = ClientRect.Width();
	m_nArrowWidth = dy * 3 / 4;

	m_dx = ClientRect.Height() - 2 * m_nArrowWidth;

	CRect ClipBox;
	dc.GetClipBox(&ClipBox);

	// Erase Background.
	CGdiObject *pBr  = dc.SelectStockObject(GRAY_BRUSH);
	CGdiObject *pPen = dc.SelectStockObject(BLACK_PEN);
	dc.Rectangle(&ClipBox);

		// Check if [Left, Right] is contained in [Min, Max].
	ASSERT(m_Min <= m_Left);
	ASSERT(m_Left <= m_Right);
	ASSERT(m_Right <= m_Max);

		// Position of Left and Right
	int x1 = static_cast<int>((m_Left - m_Min)/(m_Max - m_Min) * m_dx + 0.5);
	int x2 = static_cast<int>((m_Right - m_Min)/(m_Max - m_Min) * m_dx + 0.5);
	dc.SelectStockObject(BLACK_PEN);
	CBrush ButtonFace;
	ButtonFace.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
	dc.SelectObject(ButtonFace);
	// Draw Left Arrow
	m_RectLeft = CRect (CPoint(0, x1), CSize(dy, m_nArrowWidth));
	{ 
		CRect & rect = m_RectLeft;
		dc.Rectangle(&rect);
			// Make appearance 3D
		if (!m_bTracking || m_TrackedButton != BUTTON_LEFT) {
			rect.DeflateRect(1,1,2,2);
			CPen pen1;
			pen1.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNHIGHLIGHT));
			dc.SelectObject(pen1);
			dc.MoveTo(rect.left, rect.bottom);
			dc.LineTo(rect.left, rect.top);
			dc.LineTo(rect.right, rect.top);
			CPen pen2;
			pen2.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));
			dc.SelectObject(pen2);
			dc.LineTo(rect.right, rect.bottom);
			dc.LineTo(rect.left, rect.bottom);
		}

			// Draw Arrow
		dc.SelectStockObject(BLACK_PEN);
		dc.SelectStockObject(BLACK_BRUSH);
		CPoint poly [] = { 
			CPoint(dy / 3, x1 + m_nArrowWidth / 3), 
			CPoint(dy*2/3, x1 + m_nArrowWidth / 3),
			CPoint(dy / 2, x1 + m_nArrowWidth*2/3),
		};
		dc.Polygon(poly, 3);
	}

	// Draw Right Arrow
	m_RectRight = CRect(CPoint(0,m_nArrowWidth + x2), CSize(dy, m_nArrowWidth));
	{
		CRect & rect = m_RectRight;
		dc.SelectObject(ButtonFace);
		dc.Rectangle(&rect);

			// Make appearance 3D
		if (!m_bTracking || m_TrackedButton != BUTTON_RIGHT) {
			rect.DeflateRect(1,1,2,2);
			CPen pen1;
			pen1.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNHIGHLIGHT));
			dc.SelectObject(pen1);
			dc.MoveTo(rect.left, rect.bottom);
			dc.LineTo(rect.left, rect.top);
			dc.LineTo(rect.right, rect.top);
			CPen pen2;
			pen2.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));
			dc.SelectObject(pen2);
			dc.LineTo(rect.right, rect.bottom);
			dc.LineTo(rect.left, rect.bottom);
		}

			// Draw Arrow
		dc.SelectStockObject(BLACK_PEN);
		dc.SelectStockObject(BLACK_BRUSH);
		CPoint poly [] = { 
			CPoint(dy / 3, m_nArrowWidth + x2 + m_nArrowWidth * 2 / 3),
			CPoint(dy*2/3, m_nArrowWidth + x2 + m_nArrowWidth * 2 / 3),
			CPoint(dy / 2, m_nArrowWidth + x2 + m_nArrowWidth / 3),
		};
		dc.Polygon(poly, 3);
	}

	dc.SelectStockObject(WHITE_BRUSH);
	// Draw Area in between.
	CRect rect = CRect(CPoint(1, m_nArrowWidth + x1 + 1), CPoint (dy - 1, m_nArrowWidth + x2));

	dc.SelectStockObject(NULL_PEN);
	dc.SelectStockObject(WHITE_BRUSH);
	dc.Rectangle(&rect);

		// Store value for drawing the focus rectangle
	CRect focusRect = rect;

	// Draw Area of InnerMin, Max.
	if (m_bVisualMinMax) 
	{
		ASSERT(m_Min <= m_VisualMax);
		ASSERT(m_VisualMin <= m_VisualMax);
		ASSERT(m_VisualMax <= m_Max);

			// See wether [InnerMin, InnerMax] geschnitten [Left, Right] is non-empty.
			// Then Draw
		if (m_VisualMin < m_Right && m_VisualMax > m_Left) 
		{
			int visualMin = static_cast<int>((m_VisualMin - m_Min)/(m_Max - m_Min) * m_dx + 0.5);
			int visualMax = static_cast<int>((m_VisualMax - m_Min)/(m_Max - m_Min) * m_dx + 0.5);

				// Stay in [m_Left, m_Right] intervall.

			visualMin = max(visualMin + m_nArrowWidth, x1 + m_nArrowWidth);
			visualMax = min(visualMax + m_nArrowWidth, x2 + m_nArrowWidth);

			rect = CRect(dy - 1, visualMin, 1, visualMax);
			CBrush br;
			br.CreateSolidBrush(RGB(255,255,150)); // light yellow;
			dc.SelectObject(br);
			dc.SelectStockObject(NULL_PEN);
			dc.Rectangle(&rect);
			dc.SelectStockObject(HOLLOW_BRUSH);
		}
	}
	if (GetFocus() == this) 
	{
		focusRect.DeflateRect(3,3);
		dc.DrawFocusRect(&focusRect);
	}
	dc.SelectObject(pBr);
	dc.SelectObject(pPen);
}


void CRangeSlider::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDblClk(nFlags, point);

	if ((m_Left > m_Min) || (m_Right < m_Max))
	{
		if (RectMiddle().PtInRect(point))
		{
			SetRange(m_Min, m_Max);
			GetParent()->SendMessage(RANGE_CHANGED, RS_BOTHCHANGED);
		}
	}
}

CRect CRangeSlider::RectMiddle() const
{
	CRect rect;
	GetClientRect(&rect);

	if (m_bHorizontal)
		return CRect(m_RectLeft.right + 1, 0, m_RectRight.left - 1, rect.bottom);

	// else vertical
	return  CRect(0, m_RectLeft.bottom + 1, m_RectLeft.right, m_RectRight.top - 1);
}

void CRangeSlider::OnLButtonDown(UINT nFlags, CPoint point) 
{
	TRACE(_T("Down Point %d, %d\n"), point.x, point.y);
	SetFocus();
	Invalidate();

	if (!m_bTracking) 
	{
		// Hit Testing into Rects.
		// Left, Middle or Right?
		if (m_RectLeft.PtInRect(point)) 
		{
			m_bTracking = TRUE;
			m_TrackedButton = BUTTON_LEFT;
			m_ClickOffset = point - m_RectLeft.CenterPoint();
		} 
		else if (m_RectRight.PtInRect(point)) 
		{
			m_bTracking = TRUE;
			m_TrackedButton = BUTTON_RIGHT;
			m_ClickOffset = point - m_RectRight.CenterPoint();
		} 
		else
		{
			CRect rMiddle(RectMiddle());
			
			if (rMiddle.PtInRect(point) && ((m_Left > m_Min) || (m_Right < m_Max)))
			{
				m_bTracking = TRUE;
				m_TrackedButton = BUTTON_MIDDLE;
				m_ClickOffset = point - rMiddle.CenterPoint();
			}
		}

		if (m_bTracking)
		{
			SetCapture();
		}
		else if (m_Step > 0)
		{
			CRect rClient;
			GetClientRect(rClient);

			if (m_Left > m_Min)
			{
				CRect rLeft(rClient);

				if (m_bHorizontal)
					rLeft.right = m_RectLeft.left;
				else
					rLeft.bottom = m_RectLeft.top;

				if (rLeft.PtInRect(point))
				{
					m_Left = max(m_Min, m_Left - m_Step);
					Invalidate();

					GetParent()->SendMessage(RANGE_CHANGED, RS_LEFTCHANGED);
					return;
				}
			}
			
			if (m_Right < m_Max)
			{
				CRect rRight(rClient);
	
				if (m_bHorizontal)
					rRight.left = m_RectRight.right;
				else
					rRight.top = m_RectRight.bottom;
				
				if (rRight.PtInRect(point))
				{
					m_Right = min(m_Max, m_Right + m_Step);
					Invalidate();

					GetParent()->SendMessage(RANGE_CHANGED, RS_RIGHTCHANGED);
					return;
				}
			}
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CRangeSlider::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);

	if (m_bTracking) 
	{
		int x = 0;

		if (m_bHorizontal)
			x = point.x - m_ClickOffset.x;
		else
			x = point.y - m_ClickOffset.y;

		double oldLeft = m_Left;
		double oldRight = m_Right;

		switch (m_TrackedButton) 
		{
		case BUTTON_LEFT: 
			{
				m_Left = static_cast<double>(x - m_nArrowWidth / 2) / m_dx * (m_Max - m_Min) + m_Min; 
				m_Left = NormalizeByStep(m_Left);

				if (m_Left >= m_Right)
					m_Left = m_Right;
				if (m_Left <= m_Min)
					m_Left = m_Min;
			} 
			break;

		case BUTTON_RIGHT: 
			{
				m_Right = static_cast<double>(x - m_nArrowWidth * 3 / 2) / m_dx * (m_Max - m_Min) + m_Min;
				m_Right = NormalizeByStep(m_Right);

				if (m_Right <= m_Left)
					m_Right = m_Left;
				if (m_Right >= m_Max)
					m_Right = m_Max;
			} 
			break;

		case BUTTON_MIDDLE:
			{
				double delta = m_Right - m_Left;
				ASSERT(delta >= 0.0);

				m_Left  = static_cast<double>(x - m_nArrowWidth) / m_dx * (m_Max - m_Min) + m_Min - delta/2.0;
				m_Left = NormalizeByStep(m_Left);

				m_Right = static_cast<double>(x - m_nArrowWidth) / m_dx * (m_Max - m_Min) + m_Min + delta/2.0; 
				m_Right = NormalizeByStep(m_Right);

				if (m_Left <= m_Min) 
				{
					m_Left = m_Min;
					m_Right = m_Left + delta;
				}

				if (m_Right >= m_Max) 
				{
					m_Right = m_Max;
					m_Left = m_Right - delta;
				}
			}
			break;

		default:
			TRACE(_T("Unknown Track Mode\n"));
			ASSERT(FALSE);
			break;
		}

		// Check for change
		if ((oldLeft == m_Left) && (oldRight == m_Right))
			return;

		// Validate change
		if (!IsValidMove(m_Left, m_Right))
		{
			m_Left = oldLeft;
			m_Right = oldRight;
		}
		else
		{
			if ((oldLeft != m_Left) && (oldRight != m_Right))
			{
				GetParent()->SendMessage(RANGE_CHANGED, RS_BOTHCHANGED);
			}
			else if (oldLeft != m_Left)
			{
				GetParent()->SendMessage(RANGE_CHANGED, RS_LEFTCHANGED);
			}
			else if (oldRight != m_Right)
			{
				GetParent()->SendMessage(RANGE_CHANGED, RS_RIGHTCHANGED);
			}

			Invalidate();
		}
		
	}
}

void CRangeSlider::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bTracking)
		ReleaseCapture();
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CRangeSlider::OnCaptureChanged(CWnd* pWnd)
{
	if (pWnd != this)
	{
		m_bTracking = FALSE;
		Invalidate();
	}
}

void CRangeSlider::SetMinMax(double min, double max) 
{
	if (min > max) 
	{
		double x = max;
		max = min;
		min = x;
	}

	m_Min = (m_bInvertedMode) ? -max : min;
	m_Max = (m_bInvertedMode) ? -min : max;

	Normalize();
	Invalidate();
}

void CRangeSlider::SetStep(double step)
{
	if (step == m_Step)
		return;

	if (step == -1)
	{
		m_Step = step;
	}
	else if ((step > 0.0) && (step <= (m_Max - m_Min)))
	{
		m_Step = step;
	}
	else
	{
		ASSERT(0);
	}

	Normalize();
	Invalidate();
}

void CRangeSlider::SetRange(double left, double right) 
{
	m_Left = (m_bInvertedMode) ? -right : left;
	m_Right = (m_bInvertedMode) ? -left : right;

	Normalize();
	Invalidate();
}

void CRangeSlider::NormalizeVisualMinMax() 
{
	// Exchange if reversed.
	if (m_VisualMax < m_VisualMin) 
	{
		double dummy = m_VisualMin;
		m_VisualMin = m_VisualMax;
		m_VisualMax = dummy;
	}
	if (m_VisualMin < m_Min) 
	{
		m_VisualMin = m_Min;
		if (m_VisualMax < m_VisualMin) 
			m_VisualMax = m_VisualMin;
	}
	if (m_VisualMax > m_Max) 
	{
		m_VisualMax = m_Max;
		if (m_VisualMin > m_VisualMax)
			m_VisualMin = m_VisualMax;
	}
}

double CRangeSlider::NormalizeByStep(double value) const
{
	if (m_Step > 0.0)
	{
		value = ((int)((value / m_Step) + 0.5) * m_Step);
		value = max(min(value, m_Max), m_Min);
	}

	return value;
}

void CRangeSlider::Normalize() 
{
	double prevLeft = m_Left;
	double prevRight = m_Right;

	if (m_Left < m_Min) 
		m_Left = m_Min;
	else
		m_Left = NormalizeByStep(m_Left);

	if (m_Right > m_Max) 
		m_Right = m_Max;
	else
		m_Right = NormalizeByStep(m_Right);

	if (m_Right < m_Left) 
		m_Right = m_Left;

	if ((prevLeft != m_Left) && (prevRight != m_Right))
	{
		GetParent()->SendMessage(RANGE_CHANGED, RS_BOTHCHANGED);
	}
	else if (prevLeft != m_Left)
	{
		GetParent()->SendMessage(RANGE_CHANGED, RS_LEFTCHANGED);
	}
	else if (prevRight != m_Right)
	{
		GetParent()->SendMessage(RANGE_CHANGED, RS_RIGHTCHANGED);
	}
}

void CRangeSlider::SetVisualMode(BOOL bVisualMinMax) 
{
	if (m_bVisualMinMax != bVisualMinMax) 
		Invalidate();

	m_bVisualMinMax = bVisualMinMax;
}

void CRangeSlider::SetVisualMinMax(double VisualMin, double VisualMax) 
{
	m_VisualMin = (m_bInvertedMode) ? -VisualMax : VisualMin;
	m_VisualMax = (m_bInvertedMode) ? -VisualMin : VisualMax;
	NormalizeVisualMinMax();
	Invalidate();
}


UINT CRangeSlider::OnGetDlgCode() 
{
	return DLGC_WANTARROWS;
}

void CRangeSlider::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	BOOL bCtrl = (::GetKeyState(VK_CONTROL) & 0x8000)!= 0; // T if Ctrl is pressed.

	UINT Key1, Key2;
	if (m_bHorizontal) 
	{
		Key1 = VK_RIGHT; Key2 = VK_LEFT;
	} 
	else 
	{
		Key1 = VK_DOWN; Key2 = VK_UP;
	}
	
	if ((nChar == Key1 || nChar == Key2) && !bCtrl) 
	{
		double dx = (m_Max - m_Min) / m_dx;

		if (dx != 0.0 ) 
		{
			int left = static_cast<int>((m_Left - m_Min) / dx + 0.5);
			int right = static_cast<int>((m_Right - m_Min) / dx + 0.5);
			BOOL bShift = ::GetKeyState(VK_SHIFT) < 0; // T if Shift is pressed.
			if (nChar == Key2) 
			{
				if (!bShift) // Shift not pressed => move intervall
					left--;
				right--; 
			} 
			else 
			{
				if (!bShift) // Shift not pressed => move intervall
					left++;
				right++;
			}
			if (left >= 0 && right <= m_dx) 
			{
				m_Left = m_Min + left * dx;
				m_Right = m_Min + right * dx;
			}
			if (!bShift)
				::SendMessage(GetParent()->GetSafeHwnd(), RANGE_CHANGED, RS_BOTHCHANGED, 0);
			else 
				::SendMessage(GetParent()->GetSafeHwnd(), RANGE_CHANGED, RS_LEFTCHANGED, 0);
			Invalidate();
		}
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CRangeSlider::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	
	Invalidate();
}

void CRangeSlider::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	Invalidate();	
}

void CRangeSlider::SetVerticalMode(BOOL bVerticalMode) 
{
	if (m_bHorizontal != bVerticalMode) 
		return; 

	m_bHorizontal = !bVerticalMode;
	Invalidate();
}

void CRangeSlider::SetInvertedMode(BOOL bInvertedMode) 
{
	if (m_bInvertedMode == bInvertedMode)
		return;

	m_bInvertedMode = bInvertedMode;

	double dummy = m_Left;
	m_Left = - m_Right;
	m_Right = -dummy;

	dummy = m_Min;
	m_Min = - m_Max;
	m_Max = - dummy;

	dummy = m_VisualMin;
	m_VisualMin = - m_VisualMax;
	m_VisualMax = - dummy;

	Invalidate();
}
