// HMXChart.cpp : implementation file
//

#include "stdafx.h"
#include "HMXChart.h"
#include "GDIPlus.h"

#include <math.h>

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#define HMX_AREA_MARGINS	80		// fraction of area
#define HMX_AREA_TITLE		10		// fraction of area
#define HMX_AREA_YAXIS		7		// percentage
#define HMX_AREA_XAXIS		10		// percentage
#define HMX_AREA_MINAXIS	50		// pixels
#define HMX_XSCALE_OFFSET   14		// pixels
#define HMX_GRAPH_MARGINS	15		// pixels

/////////////////////////////////////////////////////////////////////////////

/* DIB constants */
#define PALVERSION   0x300

/* DIB Macros*/
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)

// WIDTHBYTES performs DWORD-aligning of DIB scanlines.  The "bits"
// parameter is the bit count for the scanline (biWidth * biBitCount),
// and this macro returns the number of DWORD-aligned bytes needed
// to hold those bits.
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

/////////////////////////////////////////////////////////////////////////////

#define DEG2RAD(d) ((d) * 3.141592654f / 180)

/////////////////////////////////////////////////////////////////////////////
// CHMXChart

CHMXChart::CHMXChart() 
	: 
	m_strFont(_T("Arial")), 
	m_nFontPixelSize(-1), 
	m_dwRenderFlags(HMX_RENDER_ALL),
	m_clrBkGnd(RGB(200, 255, 255)),
	m_nNumYTicks(0),
	m_dRoundY(10),
	m_nXMax(0),
	m_dYMin(0),
	m_dYMax(0),
	m_nXLabelStep(1),
	m_clrGrid(GetSysColor(COLOR_3DSHADOW)),
	m_bXLabelsAreTicks(FALSE),
	m_nXLabelDegrees(0),
	m_nCountDataset(0)
{
}

CHMXChart::~CHMXChart()
{
}


BEGIN_MESSAGE_MAP(CHMXChart, CWnd)
	//{{AFX_MSG_MAP(CHMXChart)
	ON_WM_PAINT()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_MESSAGE(WM_PRINT, OnPrintClient)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHMXChart message handlers

void CHMXChart::SetFont(LPCTSTR szFaceName, int nPointSize)
{
	// Convert points to pixels
	int nPointsPerInch = 72;
	
	HDC hDC = ::GetDC(NULL);
	int nPixelsPerInch = GetDeviceCaps(hDC, LOGPIXELSY);
	::ReleaseDC(NULL, hDC);

	int nPixelSize = MulDiv(abs(nPointSize), nPixelsPerInch, nPointsPerInch);

	if ((m_strFont.CompareNoCase(szFaceName) != 0) || (nPixelSize != m_nFontPixelSize))
	{
		m_strFont = szFaceName;
		m_nFontPixelSize = nPixelSize;

		if (GetSafeHwnd())
		{
			CalcDatas();
			Invalidate();
		}
	}
}

void CHMXChart::SetRenderFlags(DWORD dwFlags, BOOL bRedraw)
{
	dwFlags &= HMX_RENDER_ALL;

	if (dwFlags != m_dwRenderFlags)
	{
		m_dwRenderFlags = dwFlags;

		if (GetSafeHwnd())
		{
			CalcDatas();

			if (bRedraw)
				Invalidate();
		}
	}
}

DWORD CHMXChart::ModifyRenderFlags(DWORD dwRemove, DWORD dwAdd, BOOL bRedraw)
{
	DWORD dwFlags = m_dwRenderFlags;

	// Remove before adding
	dwFlags &= ~dwRemove;
	dwFlags |= dwAdd;
	
	SetRenderFlags(dwFlags, bRedraw);

	return m_dwRenderFlags;
}

void CHMXChart::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, m_rectArea.Width(),m_rectArea.Height());

	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	DoPaint(memDC);

	dc.BitBlt(0, 0, m_rectArea.Width(),m_rectArea.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
}

LRESULT CHMXChart::OnPrintClient(WPARAM wp, LPARAM lp)
{
	CDC* pDC = CDC::FromHandle((HDC)wp);

	if (pDC)
		DoPaint(*pDC, (lp & PRF_ERASEBKGND));

	return 0L;
}

void CHMXChart::DoPaint(CDC& dc, BOOL bPaintBkgnd)
{
	if (bPaintBkgnd)
		PaintBkGnd(dc);

	if (m_rectData.Height() <= 0)
		return;

	dc.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));

	if (m_dwRenderFlags & HMX_RENDER_GRID)
		DrawGrid(dc);

	DrawDatasets(dc); // always

	if (m_dwRenderFlags & HMX_RENDER_TITLE)
		DrawTitle(dc);

	if (m_dwRenderFlags & HMX_RENDER_BASELINE)
		DrawBaseline(dc);

	if (m_dwRenderFlags & HMX_RENDER_AXES)
		DrawAxes(dc);
}

BOOL CHMXChart::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CHMXChart::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CalcDatas();
}

BOOL CHMXChart::CopyToClipboard()
{
	//
	// Special thanks to Zafir Anjum for a large part of following code.
	//
	CBitmap         bitmap;
	CClientDC       dc(this);
	CDC             memDC;

	BeginWaitCursor();

	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, m_rectArea.Width(),m_rectArea.Height());

	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	DoPaint(memDC, TRUE);

	this->OpenClipboard() ;
	EmptyClipboard() ;
	SetClipboardData (CF_BITMAP, bitmap.GetSafeHandle()) ;
	CloseClipboard () ;

	bitmap.Detach();
	memDC.SelectObject(pOldBitmap);

	EndWaitCursor();

	return TRUE;
}

BOOL CHMXChart::CopyToFile()
{
	CFileDialog dlg(FALSE, _T("BMP"), _T("Chart"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Bitmap |*.bmp|"));
	if(dlg.DoModal() == IDOK)
		return CopyToFile(dlg.GetPathName());

	return FALSE;

}

BOOL CHMXChart::CopyToFile(CString sFile)
{

	CBitmap 	bitmap;
	CWindowDC	dc(this);
	CDC 		memDC;
	CFile		file;
	
	memDC.CreateCompatibleDC(&dc); 
	bitmap.CreateCompatibleBitmap(&dc, m_rectArea.Width(), m_rectArea.Height());	
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	
	DoPaint(memDC, TRUE);

	// Create logical palette if device supports a palette
	CPalette pal;
	if(memDC.GetDeviceCaps(RASTERCAPS) & RC_PALETTE)	
	{
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;
		pLP->palNumEntries = (WORD)GetSystemPaletteEntries(memDC, 0, 255, pLP->palPalEntry);
		// Create the palette
		pal.CreatePalette(pLP);
		delete[] pLP;
	}

	// Convert the bitmap to a DIB
	HANDLE hDIB = DDBToDIB(bitmap, BI_RGB, &pal);

	if(hDIB == NULL)
		return FALSE;

	// Write it to file
	if(!file.Open(sFile, CFile::modeWrite|CFile::modeCreate))
		return FALSE;

	SaveDIB((HDIB)hDIB, file);

	// Free the memory allocated by DDBToDIB for the DIB
	GlobalFree(hDIB);

	bitmap.Detach();
	memDC.SelectObject(pOldBitmap);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////

//
//	PaintBkGnd
//
//	arguments
//
//		dc = Device Context
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::PaintBkGnd(CDC &dc)
{
	dc.FillSolidRect(m_rectArea, m_clrBkGnd);
	return TRUE;
}

//
//	DrawTitle
//
//	arguments
//
//		dc = Device Context
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::DrawTitle(CDC & dc)
{
	if(m_strTitle.IsEmpty())
		return FALSE;

	int nFontSize = CalcTitleFontSize();

	CFont font;
	font.CreateFont(nFontSize, 0, 0, 0, FW_NORMAL,
					 FALSE, FALSE, FALSE, ANSI_CHARSET,
					 OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
					 DEFAULT_PITCH, m_strFont);

	
	COLORREF clrBkOld = dc.SetBkColor(m_clrBkGnd);
	CFont* pFontOld = dc.SelectObject(&font);
	dc.DrawText(m_strTitle, m_rectTitle, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS);

	dc.SetBkColor(clrBkOld);
	dc.SelectObject(pFontOld);

	return TRUE;
}

//
//	DrawGrid
//
//	arguments
//
//		dc = Device Context
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::DrawGrid(CDC & dc)
{
	if (m_dwRenderFlags & HMX_RENDER_VERTGRID)
		DrawVertGridLines(dc);

	if (m_dwRenderFlags & HMX_RENDER_HORZGRID)
		DrawHorzGridLines(dc);

	return TRUE;
}

//
//	DrawAxes
//
//	arguments
//
//		dc = Device Context
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::DrawAxes(CDC &dc)
{
	// draw Y
	if (m_dwRenderFlags & HMX_RENDER_YAXIS)
	{
		dc.MoveTo(m_rectYAxis.right, m_rectYAxis.bottom);
		dc.LineTo(m_rectYAxis.right, m_rectYAxis.top);
	}
	
	if (m_dwRenderFlags & (HMX_RENDER_YAXISSCALE | HMX_RENDER_YAXISTITLE))
		DrawYScale(dc);


	// draw X
	if (m_dwRenderFlags & HMX_RENDER_XAXIS)
	{
		int nVPos = (m_rectData.bottom - (int)CalcRelativeYValue(0.0));

		dc.MoveTo(m_rectXAxis.left, nVPos);
		dc.LineTo(m_rectXAxis.right, nVPos);
	}
	
	if (m_dwRenderFlags & (HMX_RENDER_XAXISSCALE | HMX_RENDER_XAXISTITLE))
		DrawXScale(dc);

	return TRUE;
}

//
//	DrawHorzGridLines
//
//	arguments
//
//		dc = Device Context
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::DrawHorzGridLines(CDC & dc)
{
	int nTicks = GetNumYTicks();
	
	if(!nTicks)
		return FALSE;

	double dY = ((m_dYMax - m_dYMin) / nTicks);

	if (!m_penGrid.GetSafeHandle())
		m_penGrid.CreatePen(PS_SOLID, 1, m_clrGrid);

	CPen* pPenOld = dc.SelectObject(&m_penGrid);

	for(int f=0; f<=nTicks; f++) 
	{
		double dTemp = m_rectData.bottom - (int)CalcRelativeYValue(m_dYMin + (dY*f));

		dc.MoveTo(m_rectData.left , (int)dTemp);
		dc.LineTo(m_rectData.right, (int)dTemp);
	}

	dc.SelectObject(pPenOld);

	return TRUE;
}


//
//	DrawVertLine
//
//	arguments
//
//		dc = Device Context
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::DrawVertGridLines(CDC & dc)
{
	if(!m_nXMax)
		return FALSE;

	if (!m_penGrid.GetSafeHandle())
		m_penGrid.CreatePen(PS_SOLID, 1, m_clrGrid);

	CPen* pPenOld = dc.SelectObject(&m_penGrid);

	int nCount = min(m_strarrScaleXLabel.GetSize(), m_nXMax);
	double dX = (double)m_rectData.Width()/(double)m_nXMax;

	for(int f=0; f < nCount; f += m_nXLabelStep)
	{
		CPoint ptLine;

		if (!m_strarrScaleXLabel[f].IsEmpty() && GetPointXY(0, f, ptLine))
		{
			dc.MoveTo(ptLine.x, m_rectData.top);
			dc.LineTo(ptLine.x, m_rectData.bottom + 8);
		}
	}

	dc.MoveTo(m_rectData.left + m_rectData.Width() - 1, m_rectData.top);
	dc.LineTo(m_rectData.left + m_rectData.Width() - 1, m_rectData.bottom);

	dc.SelectObject(pPenOld);

	return TRUE;
}

//
//	DrawBaseLine
//
//	arguments
//
//		dc = Device Context
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::DrawBaseline(CDC & dc)
{
	CPen* pPenOld = dc.SelectObject(&m_penGrid);

	// cannot draw baseline outside the m_rectData
	if(m_dYMin > 0)
		return FALSE;
	
	double dTemp = CalcRelativeYValue(0); // this is the zero baseline

	dc.MoveTo(m_rectData.left , m_rectData.bottom - (int)dTemp);
	dc.LineTo(m_rectData.right, m_rectData.bottom - (int)dTemp);

	dc.SelectObject(pPenOld);

	return TRUE;
}

int CHMXChart::CalcTitleFontSize() const
{
	if (m_nFontPixelSize == -1)
		return m_rectTitle.Height();

	// else
	return (m_nFontPixelSize * 2);
}

BOOL CHMXChart::CreateXAxisFont(BOOL bTitle, CFont& font) const
{
	int nFontSize = CalcXScaleFontSize(bTitle); // pixels
	int nAngle = (bTitle ? 0 : (m_nXLabelDegrees * 10));

	return font.CreateFont(-nFontSize, 0, nAngle, 0, FW_NORMAL,
						   FALSE, FALSE, FALSE, ANSI_CHARSET,
						   OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
						   DEFAULT_PITCH, m_strFont);
}

BOOL CHMXChart::CreateYAxisFont(BOOL bTitle, CFont& font) const
{
	int nFontSize = CalcYScaleFontSize(bTitle); // pixels
	int nAngle = (bTitle ? 900 : 0);
	
	return font.CreateFont(-nFontSize, 0, nAngle, 0, FW_NORMAL,
							FALSE, FALSE, FALSE, ANSI_CHARSET,
							OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
							DEFAULT_PITCH, m_strFont);
}

int CHMXChart::CalcYScaleFontSize(BOOL bTitle) const
{
	int nDefSize = (m_rectYAxis.Width() / 4);
	int nSize = m_nFontPixelSize;

	if (nSize == -1)
		nSize = nDefSize;

	if (bTitle)
	{
		nSize += min(8, nSize); // bit bigger

		// also check length of scale text
		if (!m_strYText.IsEmpty())
			nSize = min(nSize, 2 * (m_rectYAxis.Height() / m_strYText.GetLength()));
	}
	else
	{
		nSize = min(nSize, (m_rectYAxis.Height() / GetNumYTicks()));
	}

	return nSize;
}

int CHMXChart::CalcXScaleFontSize(BOOL bTitle) const
{
	return CalcYScaleFontSize(bTitle);
}

//
//	DrawXScale
//
//	arguments
//
//		dc = Device Context
//
//	return
//
//		TRUE if ok, else FALSE
//

UINT CHMXChart::GetXScaleDrawFlags() const
{
	if (m_nXLabelDegrees > 0) // => CDC::TextOut for rotated fonts
	{
		UINT nDrawFlags = (TA_BASELINE | TA_RIGHT);

		if (XScaleHasRTLDates())
			nDrawFlags |= TA_RTLREADING;

		return nDrawFlags;
	}

	// else => CDC::DrawText
	UINT nDrawFlags = (DT_LEFT | DT_TOP | DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS);

	if (XScaleHasRTLDates())
		nDrawFlags |= DT_RTLREADING;

	if (!m_bXLabelsAreTicks)
		nDrawFlags |= DT_CENTER;

	return nDrawFlags;
}

UINT CHMXChart::GetYScaleDrawFlags() const
{
	// Y scale labels are always horizontal
	UINT nDrawFlags = (DT_RIGHT | DT_BOTTOM | DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS | DT_NOCLIP);

	if (YScaleHasRTLDates())
		nDrawFlags |= DT_RTLREADING;

	return nDrawFlags;
}

BOOL CHMXChart::DrawXScale(CDC & dc)
{
	const int nBkModeOld = dc.SetBkMode(TRANSPARENT);

	if (m_dwRenderFlags & HMX_RENDER_XAXISSCALE)
	{
		int nCount = min(m_strarrScaleXLabel.GetSize(), m_nXMax);

		if (!nCount && m_strXText.IsEmpty())
			return FALSE;
	
		if (nCount)
		{
			CFont font;
			VERIFY(CreateXAxisFont(FALSE, font));
			CFont* pFontOld = dc.SelectObject(&font);

			// dX is the size of a division
			double dX = (double)m_rectData.Width()/m_nXMax;
			
			UINT nFlags = GetXScaleDrawFlags();

			for(int f=0; f<nCount; f=f+m_nXLabelStep) 
			{
				const CString& sLabel = m_strarrScaleXLabel.GetAt(f);

				if (!sLabel.IsEmpty())
				{
					CRect rText(m_rectXAxis);
			
					rText.top += HMX_XSCALE_OFFSET;
					rText.left += (int)(dX*(f+0.5)) + 4;

					if (m_nXLabelDegrees > 0)
					{
						// Must use CDC::TextOut for rotated fonts
						dc.SetTextAlign(nFlags);
						dc.TextOut(rText.left, rText.top, sLabel);
					}
					else
					{
						rText.right = (rText.left + (int)(dX * m_nXLabelStep));
						dc.DrawText(sLabel, rText, nFlags);
					}
				}
			}
	
			dc.SelectObject(pFontOld);
		}

	}

	if (!m_strXText.IsEmpty() && (m_dwRenderFlags & HMX_RENDER_XAXISTITLE))
	{
		CFont font;
		VERIFY(CreateXAxisFont(TRUE, font));

		CFont* pFontOld = dc.SelectObject(&font);
		UINT nFlags = (DT_CENTER | DT_BOTTOM | DT_SINGLELINE | DT_NOPREFIX);

		dc.DrawText(m_strXText, m_rectXAxis, nFlags);
		dc.SelectObject(pFontOld);
	}

	dc.SetBkMode(nBkModeOld);

	return TRUE;
}

//
//	DrawYScale
//
//	arguments
//
//		dc = Device Context
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::DrawYScale(CDC & dc)
{
	const int nBkModeOld = dc.SetBkMode(TRANSPARENT);
	CRect rTitle(m_rectYAxis);

	if (m_dwRenderFlags & HMX_RENDER_YAXISSCALE)
	{
		int nTicks = GetNumYTicks();

		if (!nTicks && m_strYText.IsEmpty())
			return FALSE;

		if (nTicks && (m_rectData.Height() >= nTicks))
		{
			CFont font;
			VERIFY(CreateYAxisFont(FALSE, font));

			CFont* pFontOld = dc.SelectObject(&font);

			// nY is the size of a division in data units
			double dY = (m_dYMax - m_dYMin)/nTicks;
			int nFontSize = CalcYScaleFontSize(FALSE);

			// draw text
			UINT nFlags = GetYScaleDrawFlags();

			for(int f=0; f<=nTicks; f++) 
			{
				CString sTick = GetYTickText(f, (m_dYMin + dY*f));

				if (!sTick.IsEmpty())
				{
					int nTop = m_rectYAxis.bottom + nFontSize / 2 - (int)((dY*(f + 1)) * m_rectData.Height() / (m_dYMax - m_dYMin));
					int nBot = m_rectYAxis.bottom + nFontSize / 2 - (int)((dY*(f)) * m_rectData.Height() / (m_dYMax - m_dYMin));
					ASSERT(nBot > nTop);

					CRect rTick(m_rectYAxis.left, nTop, m_rectYAxis.right - 4, nBot);
					dc.DrawText(sTick, &rTick, nFlags);

					int nLabelLeft = (m_rectYAxis.right - 4 - dc.GetTextExtent(sTick).cx);
					rTitle.right = min(rTitle.right, nLabelLeft);
				}
			}

			dc.SelectObject(pFontOld);
		}
	}

	if (!m_strYText.IsEmpty() && (m_dwRenderFlags & HMX_RENDER_YAXISTITLE))
	{
		CFont font;
		VERIFY(CreateYAxisFont(TRUE, font));

		CFont* pFontOld = dc.SelectObject(&font);

		// Only draw if there is enough space
		int nTitleExtent = dc.GetTextExtent(m_strYText).cx;

		if (nTitleExtent > rTitle.Height())
		{
			rTitle.top = m_rectArea.top + 5;
			rTitle.bottom = m_rectUsable.bottom;

			if (nTitleExtent < rTitle.Height())
			{
				// Must use CDC::TextOut for rotated fonts
				dc.SetTextAlign(TA_BASELINE | TA_RIGHT);
				dc.TextOut(rTitle.CenterPoint().x, rTitle.top, m_strYText);
			}
		}
		else
		{
			// Must use CDC::TextOut for rotated fonts
			dc.SetTextAlign(TA_BASELINE | TA_CENTER);
			dc.TextOut(rTitle.CenterPoint().x, rTitle.CenterPoint().y, m_strYText);
		}

		dc.SelectObject(pFontOld);
	}

	dc.SetBkMode(nBkModeOld);

	return TRUE;
}

CString CHMXChart::GetYTickText(int /*nTick*/, double dValue) const
{
	CString sBuffer;
	sBuffer.Format(_T("%g"), dValue);

	return sBuffer;
}

//
//	DrawDatasets
//
//	arguments
//
//		dc = Device Context
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::DrawDatasets(CDC& dc)
{
	for (int f = 0; f < HMX_MAX_DATASET; f++)
	{
		DrawDataset(dc, f, 128);
	}
	
	return TRUE;
}

//
//	DrawDataset
//
//	arguments
//
//		dc = Device Context
//		ds = Dataset
//
//	return
//
//		TRUE if ok, else FALSE
//

BOOL CHMXChart::GetMarker(HMX_DATASET_MARKER nMarker, const gdix_PointF& pt, float fSize, CArray<gdix_PointF, gdix_PointF&>& ptMarker) const
{
	switch (nMarker) 
	{
	case HMX_DATASET_MARKER_TRIANGLE:
		ptMarker.SetSize(3);
		ptMarker[ 0 ].x = pt.x;
		ptMarker[ 0 ].y = pt.y - fSize;
		ptMarker[ 1 ].x = pt.x + fSize;
		ptMarker[ 1 ].y = pt.y + fSize;
		ptMarker[ 2 ].x = pt.x - fSize;
		ptMarker[ 2 ].y = pt.y + fSize;
		break;

	case HMX_DATASET_MARKER_SQUARE:
	case HMX_DATASET_MARKER_CIRCLE:
		ptMarker.SetSize(2);
		ptMarker[ 0 ].x = pt.x - fSize;
		ptMarker[ 0 ].y = pt.y - fSize;
		ptMarker[ 1 ].x = pt.x + fSize;
		ptMarker[ 1 ].y = pt.y + fSize;
		break;

	case HMX_DATASET_MARKER_DIAMOND:
		ptMarker.SetSize(4);
		ptMarker[ 0 ].x = pt.x;
		ptMarker[ 0 ].y = pt.y - fSize;
		ptMarker[ 1 ].x = pt.x + fSize;
		ptMarker[ 1 ].y = pt.y;
		ptMarker[ 2 ].x = pt.x;
		ptMarker[ 2 ].y = pt.y + fSize;
		ptMarker[ 3 ].x = pt.x - fSize;
		ptMarker[ 3 ].y = pt.y;
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

BOOL CHMXChart::DrawDataset(CDC &dc, int nDatasetIndex, BYTE fillOpacity)
{
	if (!IsValidDatasetIndex(nDatasetIndex))
		return FALSE;

	const CHMXDataset& ds = m_datasets[nDatasetIndex];

	// Min-max charts are a special case, requiring two datasets of the same size
	if (ds.GetStyle() == HMX_DATASET_STYLE_MINMAX)
	{
		if (!IsValidDatasetIndex(nDatasetIndex + 1))
			return FALSE;

		CHMXDataset& ds2 = m_datasets[nDatasetIndex + 1];

		if (ds2.GetDatasetSize() != ds.GetDatasetSize())
			return FALSE;

		return DrawMinMaxChart(dc, ds, m_datasets[nDatasetIndex + 1], fillOpacity);
	}

	return DrawDataset(dc, ds, CDWordArray(), fillOpacity);
}

BOOL CHMXChart::DrawDataset(CDC &dc, const CHMXDataset& ds, const CDWordArray& aAltItemColors, BYTE fillOpacity)
{
	switch (ds.GetStyle())
	{
	case HMX_DATASET_STYLE_LINE:
	case HMX_DATASET_STYLE_LINE_DASHED:
	case HMX_DATASET_STYLE_LINE_DOTTED:
		return DrawLineGraph(dc, ds, aAltItemColors, fillOpacity);
		
	case HMX_DATASET_STYLE_AREALINE:
	case HMX_DATASET_STYLE_AREA:
		return DrawAreaGraph(dc, ds, fillOpacity);
		
	case HMX_DATASET_STYLE_VBAR:
		return DrawBarChart(dc, ds, aAltItemColors, fillOpacity);

	case HMX_DATASET_STYLE_PIE:
	case HMX_DATASET_STYLE_PIELINE:
	case HMX_DATASET_STYLE_DONUT:
	case HMX_DATASET_STYLE_DONUTLINE:
		return DrawPieChart(dc, ds, aAltItemColors, fillOpacity);

	case HMX_DATASET_STYLE_MINMAX:
		ASSERT(0);
		return FALSE; // drawn separately because it requires two datasets
	}

	ASSERT(0);
	return FALSE;
}

BOOL CHMXChart::DrawMinMaxChart(CDC& dc, const CHMXDataset& dsMin, const CHMXDataset& dsMax, BYTE fillOpacity)
{
	if (dsMin.GetDatasetSize() == 0)
		return FALSE;
	
	if (dsMax.GetDatasetSize() != dsMin.GetDatasetSize())
		return FALSE;

	CGdiPlusGraphics graphics(dc);

	// Get the points of both datasets
	CArray<gdix_PointF, gdix_PointF&> points[2];

	int nNumPoints = GetPoints(dsMin, points[0], FALSE);
	VERIFY(nNumPoints == GetPoints(dsMax, points[1], FALSE));

	float fPenSize = ScaleByDPIFactor(dsMin.GetSize()), fMarkerSize = (fPenSize + 1.0f);
	HMX_DATASET_MARKER nMarkers[2] = { dsMin.GetMarker(), dsMax.GetMarker() };

	// Create pens and brushes
	CGdiPlusPen pens[2];
	CGdiPlusBrush brushes[2];

	pens[0].Create(dsMin.GetLineColor(), fPenSize, gdix_PenStyleSolid);
	pens[1].Create(dsMax.GetLineColor(), fPenSize, gdix_PenStyleSolid);
	
	if (fillOpacity > 0)
	{
		brushes[0].Create(dsMin.GetFillColor(), fillOpacity);
		brushes[1].Create(dsMax.GetFillColor(), fillOpacity);
	}

	for (int f = 0; f < nNumPoints; f++)
	{
		BOOL bHasVal[2] = { ((int)points[0][f].y < m_rectData.bottom), 
							((int)points[1][f].y < m_rectData.bottom) };

		// Draw vertical line from Min to Max in a colour depending on the direction of the line
		int nPen = 0;

		if (bHasVal[0] && bHasVal[1])
		{
			if (points[0][f].y > points[1][f].y)
				nPen = 1;

			CGdiPlus::DrawLine(graphics, pens[nPen], &points[0][f], &points[1][f]);
		}

		// Markers
		for (int i = 0; i < 2; i++)
		{
			if (bHasVal[i] && (nMarkers[i] != HMX_DATASET_MARKER_NONE))
			{
				CArray<gdix_PointF, gdix_PointF&> ptMarker;
				VERIFY(GetMarker(nMarkers[i], points[i][f], fMarkerSize, ptMarker));

				switch (nMarkers[i])
				{
				case HMX_DATASET_MARKER_TRIANGLE:
				case HMX_DATASET_MARKER_DIAMOND:
					VERIFY(CGdiPlus::DrawPolygon(graphics, pens[nPen], ptMarker.GetData(), ptMarker.GetSize(), brushes[nPen]));
					break;

				case HMX_DATASET_MARKER_SQUARE:
					VERIFY(CGdiPlus::DrawRect(graphics, pens[nPen], CGdiPlusRectF(ptMarker[0], ptMarker[1]), brushes[nPen]));
					break;

				case HMX_DATASET_MARKER_CIRCLE:
					VERIFY(CGdiPlus::DrawEllipse(graphics, pens[nPen], CGdiPlusRectF(ptMarker[0], ptMarker[1]), brushes[nPen]));
					break;
				}
			}
		}
	}

	return TRUE;
}

BOOL CHMXChart::DrawLineGraph(CDC &dc, const CHMXDataset& ds, const CDWordArray& aAltMarkerColors, BYTE fillOpacity)
{
	if (ds.GetDatasetSize() == 0)
		return FALSE;

	CArray<gdix_PointF, gdix_PointF&> points;
	int nPoints = GetPoints(ds, points, FALSE);

	if (nPoints < 2)
		return FALSE;

	gdix_PenStyle nPenStyle = gdix_PenStyleSolid;

	switch (ds.GetStyle())
	{
	case HMX_DATASET_STYLE_LINE_DASHED:	
		nPenStyle = gdix_PenStyleDash;	
		break;

	case HMX_DATASET_STYLE_LINE_DOTTED:	
		nPenStyle = gdix_PenStyleDot;	
		break;
	}

	CGdiPlusGraphics graphics(dc);
	CGdiPlusPen linePen(ds.GetLineColor(), ScaleByDPIFactor(ds.GetSize()), nPenStyle);

	VERIFY(CGdiPlus::DrawLines(graphics, linePen, points.GetData(), points.GetSize()));

	HMX_DATASET_MARKER nMarker = ds.GetMarker();

	if (nMarker != HMX_DATASET_MARKER_NONE)
	{
		CGdiPlusPen defMarkerPen;
		CGdiPlusBrush defMarkerBrush;
		CreateDefaultItemDrawingTools(ds, aAltMarkerColors, fillOpacity, defMarkerPen, defMarkerBrush);

		float fSize = (ds.GetSize() * 2.0f);

		CArray<gdix_PointF, gdix_PointF&> ptMarker;

		for (int f = 0; f < nPoints; f++)
		{
			VERIFY(GetMarker(nMarker, points[f], fSize, ptMarker));

			CGdiPlusPen markerPen;
			CGdiPlusBrush markerBrush;
			CreateItemDrawingTools(f, aAltMarkerColors, fillOpacity, markerPen, markerBrush);

			gdix_Pen* pen = (markerPen.IsValid() ? markerPen : defMarkerPen);
			gdix_Brush* brush = (markerBrush.IsValid() ? markerBrush : defMarkerBrush);

			switch (nMarker)
			{
			case HMX_DATASET_MARKER_TRIANGLE:
			case HMX_DATASET_MARKER_DIAMOND:
				VERIFY(CGdiPlus::DrawPolygon(graphics, pen, ptMarker.GetData(), ptMarker.GetSize(), brush));
				break;

			case HMX_DATASET_MARKER_SQUARE:
				VERIFY(CGdiPlus::DrawRect(graphics, pen, CGdiPlusRectF(ptMarker[0], ptMarker[1]), brush));
				break;

			case HMX_DATASET_MARKER_CIRCLE:
				VERIFY(CGdiPlus::DrawEllipse(graphics, pen, CGdiPlusRectF(ptMarker[0], ptMarker[1]), brush));
				break;
			}
		}
	}

	return TRUE;
}

BOOL CHMXChart::DrawAreaGraph(CDC &dc, const CHMXDataset& ds, BYTE fillOpacity)
{
	if (ds.GetDatasetSize() == 0)
		return FALSE;

	CArray<gdix_PointF, gdix_PointF&> points;
	int nPoints = GetPoints(ds, points, TRUE);

	if (nPoints >= 4)
	{
		CGdiPlusGraphics graphics(dc);
		CGdiPlusBrush brush(ds.GetFillColor(), fillOpacity);
	
		VERIFY(CGdiPlus::FillPolygon(graphics, brush, points.GetData(), nPoints));

		// draw line too?
		if ((ds.GetStyle() == HMX_DATASET_STYLE_AREALINE))
		{
			// don't draw the first/last closure points
			// That's why we need at least 4 points.
			CGdiPlusPen pen(ds.GetLineColor(), ScaleByDPIFactor(1), gdix_PenStyleSolid);

			VERIFY(CGdiPlus::DrawLines(graphics, pen, points.GetData() + 1, nPoints - 2));
		}
	}

	return TRUE;
}

BOOL CHMXChart::DrawBarChart(CDC &dc, const CHMXDataset& ds, const CDWordArray& aAltBarColors, BYTE fillOpacity)
{
	if (ds.GetDatasetSize() == 0)
		return FALSE;

	CGdiPlusGraphics graphics(dc);
	CGdiPlusPen defBarPen;
	CGdiPlusBrush defBarBrush;
	CreateDefaultItemDrawingTools(ds, aAltBarColors, fillOpacity, defBarPen, defBarBrush);
	
	double dBarSpacing = (double)m_rectData.Width() / (double)m_nXMax;

	for (int f = 0; f < ds.GetDatasetSize(); f++)
	{
		double dSample = 0.0;
		ds.GetData(f, dSample);

		if (dSample == HMX_DATASET_VALUE_INVALID)
			break;

		if (dSample == 0.0)
			continue;

		double dTemp = CalcRelativeYValue(dSample);
		CRect rBar;

		if (dSample > 0.0)
		{
			//  bar is positive
			double dZeroLine = max(m_dYMin, 0);
			double dTemp1 = CalcRelativeYValue(dZeroLine);

			rBar.top = (int)(m_rectData.bottom - dTemp);
			rBar.bottom = (int)(m_rectData.bottom - dTemp1);

			// Ensure something is visible
			if (rBar.Height() == 0)
				rBar.top--;
		}
		else // if (nSample < 0.0)
		{
			// bar is negative
			double dZeroLine = min(m_dYMax, 0);
			double dTemp1 = CalcRelativeYValue(dZeroLine);

			rBar.top = (int)(m_rectData.bottom - dTemp1);
			rBar.bottom = (int)(m_rectData.bottom - dTemp);

			// Ensure something is visible
			if (rBar.Height() == 0)
				rBar.bottom++;
		}

		int nBarPos = m_rectData.left + (int)(dBarSpacing*(f + 0.5));

		rBar.left = (nBarPos - (int)(dBarSpacing*(ds.GetSize() / 2.0) / 10.0));
		rBar.right = (nBarPos + (int)(dBarSpacing*(ds.GetSize() / 2.0) / 10.0));

		if (rBar.Width() <= 0)
			rBar.right = (rBar.left + 1);

		CGdiPlusPen barPen;
		CGdiPlusBrush barBrush;
		CreateItemDrawingTools(f, aAltBarColors, fillOpacity, barPen, barBrush);

		gdix_Pen* pen = (barPen.IsValid() ? barPen : defBarPen);
		gdix_Brush* brush = (barBrush.IsValid() ? barBrush : defBarBrush);

		VERIFY(CGdiPlus::DrawRect(graphics, pen, CGdiPlusRectF(rBar), brush));
	}

	return TRUE;
}

BOOL CHMXChart::CalcPieRects(CRect& rPie, CRect& rDonut) const
{
	// Make the chart as big as possible
	rPie = m_rectData;

	// Allow for item label near the top
	rPie.top += CalcYScaleFontSize(FALSE);
	
	// Only use half the width to allow for labels
	int nSize = min(rPie.Height(), (rPie.Width() / 2));
	nSize -= (nSize % 2); // make even size

	rPie.right = rPie.left + nSize;
	rPie.top = rPie.bottom - nSize;

	// And centre it
	CPoint ptOffset = (m_rectData.CenterPoint() - rPie.CenterPoint());
	rPie.OffsetRect(ptOffset);

	if ((rPie.right <= rPie.left) || (rPie.bottom <= rPie.top))
	{
		rPie.SetRectEmpty();
		return FALSE;
	}

	// else
	rDonut = rPie;
	rDonut.DeflateRect((nSize / 4), (nSize / 4));

	return TRUE;
}

BOOL CHMXChart::DrawPieChart(CDC &dc, const CHMXDataset& ds, const CDWordArray& aAltPieColors, BYTE fillOpacity)
{
	CArray<PIESEGMENT, PIESEGMENT&> aSegments;
	int nNumSeg = CalcPieSegments(ds, aSegments);
	
	if (nNumSeg == 0)
		return FALSE;

	CRect rPie, rDonut;
	
	if (!CalcPieRects(rPie, rDonut))
		return FALSE;
	
	// Create default drawing tools
	CGdiPlusGraphics graphics(dc);
	CGdiPlusPen defPiePen;
	CGdiPlusBrush defPieBrush;
	VERIFY(CreateDefaultItemDrawingTools(ds, aAltPieColors, fillOpacity, defPiePen, defPieBrush));

	// Draw the items
	HMX_DATASET_STYLE nStyle = ds.GetStyle();

	BOOL bAreaLine = ((nStyle == HMX_DATASET_STYLE_PIELINE) || (nStyle == HMX_DATASET_STYLE_DONUTLINE));
	BOOL bDonut = ((nStyle == HMX_DATASET_STYLE_DONUT) || (nStyle == HMX_DATASET_STYLE_DONUTLINE));

	for (int f = 0; f < nNumSeg; f++)
	{
		const PIESEGMENT& seg = aSegments[f];

		if (seg.fSweepDegrees > 0.0f)
		{
			CGdiPlusPen piePen;
			CGdiPlusBrush pieBrush;
			CreateItemDrawingTools(f, aAltPieColors, fillOpacity, piePen, pieBrush);

			gdix_Pen* pen = (piePen.IsValid() ? piePen : defPiePen);
			gdix_Brush* brush = (pieBrush.IsValid() ? pieBrush : defPieBrush);

			if (bAreaLine)
			{
				CGdiPlus::DrawPie(graphics, pen, rPie, seg.fStartDegrees, seg.fSweepDegrees, brush);

				if (bDonut)
					CGdiPlus::DrawArc(graphics, pen, rDonut, seg.fStartDegrees, seg.fSweepDegrees);
			}
			else
			{
				CGdiPlus::FillPie(graphics, brush, rPie, seg.fStartDegrees, seg.fSweepDegrees);
			}
		}
	}

	if (bDonut)
	{
		rDonut.DeflateRect(1, 1);
		CGdiPlus::FillEllipse(graphics, CGdiPlusBrush(GetSysColor(COLOR_WINDOW)), rDonut);
	}

	DrawPieLabels(dc, rPie, aSegments);

	return TRUE;
}

int CHMXChart::CalcPieSegments(const CHMXDataset& dataset, CArray<PIESEGMENT, PIESEGMENT&>& aSegments) const
{
	// Get the total aggregate value to be distributed
	int nNumData = dataset.GetDatasetSize(), f;

	if (nNumData != m_strarrScaleXLabel.GetSize())
	{
		ASSERT(0);
		return 0;
	}

	double dTotalData = 0.0;

	for (f = 0; f < nNumData; f++)
	{
		double dValue;
		VERIFY(dataset.GetData(f, dValue));

		dTotalData += max(0.0, dValue);
	}

	float fAngleFactor = (float)(360 / dTotalData);
	double dDataValue;
	
	PIESEGMENT seg;
	seg.fStartDegrees = 270.0f; // 12 o'clock

	for (f = 0; f < nNumData; f++)
	{
		seg.sLabel = m_strarrScaleXLabel[f];
		
		if (dataset.GetData(f, dDataValue) && (dDataValue > 0.0))
			seg.fSweepDegrees = (float)(dDataValue * fAngleFactor);
		else
			seg.fSweepDegrees = 0.0f;

		aSegments.Add(seg);

		// Next segment
		seg.fStartDegrees = NormaliseAngle(seg.fStartDegrees + seg.fSweepDegrees);
	}

	return nNumData;
}

float CHMXChart::NormaliseAngle(float fDegrees)
{
	while (fDegrees > 360.0f)
		fDegrees -= 360.0f;

	while (fDegrees < 0.0f)
		fDegrees += 360.0f;

	return fDegrees;
}

float CHMXChart::ScaleByDPIFactor(int nValue)
{
	static float fScaleFactor = 0.0f;

	if (fScaleFactor == 0.0f)
	{
		HDC hDC = ::GetDC(NULL);
		int nPPI = GetDeviceCaps(hDC, LOGPIXELSY);
		::ReleaseDC(NULL, hDC);

		fScaleFactor = (nPPI / 96.0f);
	}

	return (nValue * fScaleFactor);
}

void CHMXChart::DrawPieLabels(CDC& dc, const CRect& rPie, const CArray<PIESEGMENT, PIESEGMENT&>& aSegments)
{
	int nNumData = aSegments.GetSize();

	if (!nNumData)
	{
		ASSERT(0);
		return;
	}

	CGdiPlusGraphics graphics(dc);
	CGdiPlusPen labelPen(m_clrGrid);

	CFont fontLabel;
	VERIFY(CreateYAxisFont(FALSE, fontLabel));
	CFont* pFontOld = dc.SelectObject(&fontLabel);

	int nRadius = (rPie.Width() / 2), nLabelHeight = CalcYScaleFontSize(FALSE);
	CPoint ptCentre(rPie.CenterPoint());

	for (int f = 0; f < nNumData; f++)
	{
		const PIESEGMENT& seg = aSegments[f];

		if (!seg.sLabel.IsEmpty() && (seg.fSweepDegrees > 0.0f))
		{
			float fAveAngle = DEG2RAD(seg.fStartDegrees + (seg.fSweepDegrees / 2));
			CPoint ptLabel[3];

			// Initial point on the circumference
			ptLabel[0] = ptCentre;
			ptLabel[0].x += (int)(cos(fAveAngle) * (nRadius + 2));
			ptLabel[0].y += (int)(sin(fAveAngle) * (nRadius + 2));

			// Next point a little further out
			ptLabel[1] = ptLabel[0];
			ptLabel[1].x += (int)(cos(fAveAngle) * 10);
			ptLabel[1].y += (int)(sin(fAveAngle) * 10);

			// Last point projected horizontally depending on 
			// which side of the centre point we are
			BOOL bRightSide = (ptLabel[0].x >= ptCentre.x);

			ptLabel[2] = ptLabel[1];
			ptLabel[2].x = ptCentre.x;
			
			if (bRightSide)
				ptLabel[2].x += (nRadius * 3 / 2);
			else
				ptLabel[2].x -= (nRadius * 3 / 2);

			CGdiPlus::DrawLines(graphics, labelPen, ptLabel, 3);

			// Label text
			int nDrawFlags = DT_VCENTER;
			CRect rLabel(m_rectData);
				
			rLabel.top = (ptLabel[2].y - (nLabelHeight / 2));
			rLabel.bottom = (rLabel.top + nLabelHeight);

			if (bRightSide)
			{
				rLabel.left = ptLabel[2].x;
				nDrawFlags |= DT_LEFT;
			}
			else
			{
				rLabel.right = ptLabel[2].x;
				nDrawFlags |= DT_RIGHT;
			}
			
			dc.DrawText(seg.sLabel, rLabel, nDrawFlags);
		}
	}

	dc.SelectObject(pFontOld);
}


BOOL CHMXChart::CreateDefaultItemDrawingTools(const CHMXDataset& ds, const CDWordArray& aAltItemColors, BYTE fillOpacity, CGdiPlusPen& pen, CGdiPlusBrush& brush)
{
	COLORREF crPen = CLR_NONE, crBrush = CLR_NONE;

	switch (aAltItemColors.GetSize())
	{
	case 0:	
		crPen = ds.GetLineColor(); 
		crBrush = ds.GetFillColor();
		break;

	case 1:
	default:
		crPen = crBrush = aAltItemColors[0];
		break;
	}
	ASSERT((crPen != CLR_NONE) && (crBrush != CLR_NONE));

	float fThickness = ScaleByDPIFactor(1);

	return (pen.Create(crPen, fThickness, gdix_PenStyleSolid) && brush.Create(crBrush, fillOpacity));
}

BOOL CHMXChart::CreateItemDrawingTools(int nItem, const CDWordArray& aColors, BYTE fillOpacity, CGdiPlusPen& pen, CGdiPlusBrush& brush)
{
	if (aColors.GetSize() <= 1)
		return FALSE;

	COLORREF color = aColors[nItem % aColors.GetSize()];
	float fThickness = ScaleByDPIFactor(1);

	return (pen.Create(color, fThickness, gdix_PenStyleSolid) && brush.Create(color, fillOpacity));
}

COLORREF CHMXChart::GetLineColor(int nDatasetIndex, double dValue) const
{
	if (!IsValidDatasetIndex(nDatasetIndex))
		return CLR_NONE;

	return m_datasets[nDatasetIndex].GetLineColor();
}

COLORREF CHMXChart::GetFillColor(int nDatasetIndex, double dValue) const
{
	if (!IsValidDatasetIndex(nDatasetIndex))
		return CLR_NONE;

	return m_datasets[nDatasetIndex].GetFillColor();
}

int CHMXChart::GetPoints(const CHMXDataset& ds, CArray<gdix_PointF, gdix_PointF&>& points, BOOL bArea) const
{
	// let's calc real dataset size (excluding invalid data)
	int nNumPoints = 0, g;
	double dSample;

	for(g=0; g<ds.GetDatasetSize(); g++) 
	{
		ds.GetData(g, dSample);
		if(dSample != HMX_DATASET_VALUE_INVALID)
			nNumPoints++;
	}

	// If we only have a single point then we need
	// two extra points to forma triangle
	BOOL bSinglePoint = (nNumPoints == 1);

	if (bSinglePoint)
		nNumPoints += 2;

	// If in area mode then we need a further two
	// points vertically beneath the existing first
	// and last points to close off the area
	if (bArea)
		nNumPoints += 2;

	points.SetSize(nNumPoints);
	
	double dBarWidth = (double)m_rectData.Width()/(double)m_nXMax;
	
	// Calculate the zero line where we will close off the area vertically
	double dZeroLine = max(m_dYMin, min(m_dYMax, 0));

	// First points (area and/or single point)
	g = 0;
	if (bArea || bSinglePoint)
	{
		int f = 0;
		do
		{
			ds.GetData(f, dSample);
			f++;
		}
		while (dSample == HMX_DATASET_VALUE_INVALID);

		// Calculate the x pos vertically beneath the first data point
		points[0].x = (m_rectData.left + (float)(dBarWidth / 2.0) + (int)(dBarWidth*(f - 1.0)));
		points[0].y = (m_rectData.bottom - CalcRelativeYValue(dZeroLine));

		g++;

		// If we only have a single point then offset this point
		// to the left by a quarter of the nominal bar width
		if (bSinglePoint)
		{
			points[0].x -= (gdix_Real)(dBarWidth / 4);

			// If we are in also area we just copy this point
			if (bArea)
			{
				points[1] = points[0];
				g++;
			}
		}
	}

	// The actual data points
	for (int f = 0; f < ds.GetDatasetSize(); f++)
	{
		ds.GetData(f, dSample);
		if (dSample == HMX_DATASET_VALUE_INVALID)
			continue;

		points[g].x = m_rectData.left + (float)(dBarWidth / 2.0) + (int)(dBarWidth*f);
		points[g].y = m_rectData.bottom - CalcRelativeYValue(dSample);

		g++;
	}

	// Last points (area and/or single point)
	if (bArea || bSinglePoint)
	{
		points[g].x = points[g - 1].x;
		points[g].y = (m_rectData.bottom - CalcRelativeYValue(dZeroLine));

		// If we only have a single point then we offset this point
		// to the right by a quarter of the nominal bar width
		if (bSinglePoint)
		{
			points[g].x += (gdix_Real)(dBarWidth / 4);

			// If we are in also area we just copy this point
			if (bArea)
			{
				points[g + 1] = points[g];
			}
		}
	}

	return points.GetSize();
}

float CHMXChart::CalcRelativeYValue(double dSample) const
{
	return (float)((dSample - m_dYMin) * m_rectData.Height() / (m_dYMax - m_dYMin));
}

BOOL CHMXChart::GetPointXY(int nDatasetIndex, int nIndex, CPoint& point, double dBarWidth) const
{
	gdix_PointF ptTemp;

	if (!GetPointXY(nDatasetIndex, nIndex, ptTemp, dBarWidth))
		return FALSE;

	point.x = (int)ptTemp.x;
	point.y = (int)ptTemp.y;

	return TRUE;
}

BOOL CHMXChart::GetPointXY(int nDatasetIndex, int nIndex, gdix_PointF& point, double dBarWidth) const
{
	double dSample;

	if (!GetData(nDatasetIndex, nIndex, dSample))
		return FALSE;

	if (dSample == HMX_DATASET_VALUE_INVALID)
		return FALSE;

	if (dBarWidth <= 0)
		dBarWidth = (double)m_rectData.Width() / (double)m_nXMax;

	point.x = m_rectData.left + (float)(dBarWidth/2.0) + (int)(dBarWidth*nIndex);
	point.y = m_rectData.bottom - CalcRelativeYValue(dSample);

	return TRUE;
}

//
//	CalcDatas
//	calculate all useful variables starting from the control size
//
//	arguments
//
//		none
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::CalcDatas()
{
	m_nXMax = 0;

	for(int f=0; f<HMX_MAX_DATASET; f++) 
	{
		int nDataSize = m_datasets[f].GetDatasetSize();
		m_nXMax = max(m_nXMax, nDataSize);
	}
	
	if (!GetMinMax(m_dYMin, m_dYMax, FALSE))
		m_dYMin = m_dYMax = 0;

	// with this 'strange' function I can set m_nYmin & m_nYMax so that 
	// they are multiply of m_nRoundY
	if(m_dRoundY > 0.0) 
	{
		if (fmod(m_dYMin, m_dRoundY) != 0.0)
			m_dYMin = (((int)m_dYMin-(int)m_dRoundY)/(int)m_dRoundY)*m_dRoundY;

		if (fmod(m_dYMax, m_dRoundY) != 0.0)
			m_dYMax = (((int)m_dYMax+(int)m_dRoundY)/(int)m_dRoundY)*m_dRoundY;
	}

	// prevent divide by zero
	if (m_dYMax == m_dYMin)
		m_dYMax = (m_dYMin + 10);

	GetClientRect(m_rectArea);

	m_rectUsable = m_rectArea;
	m_rectUsable.DeflateRect(HMX_GRAPH_MARGINS, HMX_GRAPH_MARGINS);

	// let's calc everything
	m_rectGraph = m_rectUsable;

	if (!m_strTitle.IsEmpty() && HasRenderFlag(HMX_RENDER_TITLE)) 
	{
		m_rectTitle = m_rectUsable;

		m_rectTitle.bottom = (m_rectUsable.bottom / HMX_AREA_TITLE);
		m_rectGraph.top    = m_rectTitle.bottom;
	} 

	// make axis width the same for horz and vert
	int nAxisSize = 0;

	if (m_dwRenderFlags & HMX_RENDER_AXES)
	{
		CClientDC dc(this);
		nAxisSize = CalcAxisSize(m_rectGraph, dc);
	}

	// Initialise to zero width/height
	m_rectXAxis = m_rectGraph;
	m_rectXAxis.top = m_rectXAxis.bottom;

	m_rectYAxis = m_rectGraph;
	m_rectYAxis.right = m_rectYAxis.left;

	if (m_dwRenderFlags & (HMX_RENDER_XAXIS | HMX_RENDER_XAXISTITLE | HMX_RENDER_XAXISSCALE))
	{
		m_rectXAxis.top = m_rectXAxis.bottom - nAxisSize;
		m_rectYAxis.bottom = m_rectXAxis.top;
	}

	if (m_dwRenderFlags & (HMX_RENDER_YAXIS | HMX_RENDER_YAXISTITLE | HMX_RENDER_YAXISSCALE))
	{
		m_rectYAxis.right  = m_rectYAxis.left + nAxisSize;
		m_rectXAxis.left = m_rectYAxis.right;
	}

	// Data rect is whatever is left
	m_rectData.top     = m_rectGraph.top;
	m_rectData.bottom  = m_rectXAxis.top;
	m_rectData.left    = m_rectYAxis.right;
	m_rectData.right   = m_rectGraph.right;

	return TRUE;
}

int CHMXChart::CalcAxisSize(const CRect& rAvail, CDC& dc) const
{
	int nAxisSize = 0;

	if (m_nFontPixelSize == -1)
	{
		// make axis width the same for horz and vert
		int nYAxisWidth = (rAvail.Width()*HMX_AREA_YAXIS) / 100;
		int nXAxisHeight = (rAvail.Height()*HMX_AREA_XAXIS) / 100;

		nAxisSize = max(HMX_AREA_MINAXIS, min(nXAxisHeight, nYAxisWidth));
	}
	else
	{
		int nXAxisHeight = 0, nYAxisWidth = 0;
		
		if (!m_strXText.IsEmpty() && HasRenderFlag(HMX_RENDER_XAXISTITLE))
			nXAxisHeight = (m_nFontPixelSize * 2);

		if (!m_strYText.IsEmpty() && HasRenderFlag(HMX_RENDER_YAXISTITLE))
			nYAxisWidth = (m_nFontPixelSize * 2);

		if (m_strarrScaleXLabel.GetSize() && HasRenderFlag(HMX_RENDER_XAXISSCALE))
		{
			CFont font;
			VERIFY(CreateXAxisFont(FALSE, font));

			CFont* pFontOld = dc.SelectObject(&font);
			int nLabelHeight = dc.GetTextExtent(_T("ABC123")).cy;

			if (m_nXLabelDegrees == 0)
			{
				nXAxisHeight += nLabelHeight;
			}
			else
			{
				int nLabel = m_strarrScaleXLabel.GetSize(), nMaxLabelLen = 0;
				CRect rLabel;

				while (nLabel--)
				{
					if (!m_strarrScaleXLabel[nLabel].IsEmpty())
					{
						int nLabelLen = dc.GetTextExtent(m_strarrScaleXLabel[nLabel]).cx;
						nMaxLabelLen = max(nMaxLabelLen, nLabelLen);
					}
				}

				nXAxisHeight += (int)(sin(m_nXLabelDegrees / 90.0) * nMaxLabelLen);
				nXAxisHeight += (int)(cos(m_nXLabelDegrees / 90.0) * nLabelHeight);
				
				dc.SelectObject(pFontOld);
			}
		}

		if ((GetNumYTicks() > 0) && HasRenderFlag(HMX_RENDER_YAXISSCALE))
		{
			CFont font;
			VERIFY(CreateYAxisFont(FALSE, font));

			CFont* pFontOld = dc.SelectObject(&font);

			CString sBuffer = GetYTickText(0, m_dYMax);
			nYAxisWidth += dc.GetTextExtent(sBuffer).cx;
			
			dc.SelectObject(pFontOld);
		}

		nAxisSize = max(HMX_AREA_MINAXIS, max(nXAxisHeight, nYAxisWidth));
	}
	
	return nAxisSize;
}

BOOL CHMXChart::GetMinMax(double& dMin, double& dMax, BOOL bDataOnly) const
{
	return GetMinMax(dMin, dMax, bDataOnly, HMX_DATASET_VALUE_NOIGNORE);
}

BOOL CHMXChart::GetMinMax(double& dMin, double& dMax, BOOL bDataOnly, double dIgnoreVal) const
{
	// First dataset
	if (!m_datasets[0].GetMinMax(dMin, dMax, bDataOnly, dIgnoreVal))
		return FALSE;

	// Rest of datasets
	for(int f = 1; f<HMX_MAX_DATASET; f++) 
	{
		double dTemp1, dTemp2;

		if (!m_datasets[f].GetMinMax(dTemp1, dTemp2, bDataOnly, dIgnoreVal)) 
			break;

		dMin = min(dMin, dTemp1);
		dMax = max(dMax, dTemp2);
	}

	return TRUE;
}

//
//	Redraw
//	redraw everything
//
//	arguments
//
//		none
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::Redraw()
{
	Invalidate(TRUE);
	UpdateWindow();

	return TRUE;
}

//
//	SetData
//	Add new data to the dataset
//
//	arguments
//
//		nDatasetIndex = the dataset index
//		nData         = the value
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::AddData(int nDatasetIndex, double dData)
{
	if (!IsValidDatasetIndex(nDatasetIndex))
		return FALSE;

	return m_datasets[nDatasetIndex].AddData(dData);
}

BOOL CHMXChart::ClearData( int nDatasetIndex)
{
	if (!IsValidDatasetIndex(nDatasetIndex))
		return FALSE;

	m_datasets[nDatasetIndex].ClearData();
	return TRUE;
}

void CHMXChart::ClearData()
{
	for (int f=0; f<HMX_MAX_DATASET; f++) 
		ClearData(f);
}

BOOL CHMXChart::ResetDataset(int nDatasetIndex)
{
	if (!IsValidDatasetIndex(nDatasetIndex))
		return FALSE;

	m_datasets[nDatasetIndex].Reset();
	return TRUE;
}

void CHMXChart::ResetDatasets()
{
	for (int f=0; f<HMX_MAX_DATASET; f++) 
		ResetDataset(f);
}

//
//	SetData
//	Modify a data into the dataset
//
//	arguments
//
//		nDatasetIndex = dataset index
//		nIndex        = index
//		nData         = value
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::SetData(int nDatasetIndex, int nIndex, double dData)
{
	if (!IsValidDatasetIndex(nDatasetIndex))
		return FALSE;

	return m_datasets[nDatasetIndex].SetData(nIndex, dData);
}

//
//	GetData
//	get data from the dataset
//
//	arguments
//
//		nDatasetIndex = dataset index
//		nIndex        = index
//		nData         = value
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::GetData(int nDatasetIndex, int nIndex, double& dData) const
{
	if (!IsValidDatasetIndex(nDatasetIndex))
		return FALSE;

	return m_datasets[nDatasetIndex].GetData(nIndex,dData);

}

//
//	SetDatasetStyle
//
//	arguments
//
//		nDatasetIndex = dataset index
//		nStyle        = style
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::SetDatasetStyle(int nDatasetIndex, HMX_DATASET_STYLE nStyle)
{
	if (!IsValidDatasetIndex(nDatasetIndex))
		return FALSE;

	return m_datasets[nDatasetIndex].SetStyle(nStyle);
}

//
//	GetDatasetStyle
//
//	arguments
//
//		nDatasetIndex = dataset index
//		nStyle        = style
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::GetDatasetStyle(int nDatasetIndex, HMX_DATASET_STYLE& nStyle) const
{
	if (!IsValidDatasetIndex(nDatasetIndex))
		return FALSE;

	nStyle = m_datasets[nDatasetIndex].GetStyle();

	return TRUE;
}

//
//	SetDatasetMarker
//
//	arguments
//
//		nDatasetIndex = dataset index
//		nMarker       = marker
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::SetDatasetMarker(int nDatasetIndex, HMX_DATASET_MARKER nMarker)
{
	if (!IsValidDatasetIndex(nDatasetIndex))
		return FALSE;

	return m_datasets[nDatasetIndex].SetMarker(nMarker);
}

BOOL CHMXChart::SetDatasetMin(int nDatasetIndex, double dMin)
{
	if (!IsValidDatasetIndex(nDatasetIndex))
		return FALSE;

	m_datasets[nDatasetIndex].SetMin(dMin);
	return TRUE;
}

BOOL CHMXChart::SetDatasetMax(int nDatasetIndex, double dMax)
{
	if (!IsValidDatasetIndex(nDatasetIndex))
		return FALSE;

	m_datasets[nDatasetIndex].SetMax(dMax);
	return TRUE;
}

//
//	GetDatasetMarker
//
//	arguments
//
//		nDatasetIndex = dataset index
//		nMarker       = marker
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::GetDatasetMarker(int nDatasetIndex, HMX_DATASET_MARKER& nMarker) const
{
	if (!IsValidDatasetIndex(nDatasetIndex))
		return FALSE;

	nMarker = m_datasets[nDatasetIndex].GetMarker();

	return TRUE;
}

//
//	SetDatasetPenSize
//
//	arguments
//
//		nDatasetIndex = dataset index
//		nSize         = pen size in pixel or bar size (range 1-10)
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::SetDatasetSizeFactor(int nDatasetIndex, int nSize)
{
	if (!IsValidDatasetIndex(nDatasetIndex))
		return FALSE;

	return m_datasets[nDatasetIndex].SetSize(nSize);
}

//
//	GetDatasetPenSize
//
//	arguments
//
//		nDatasetIndex = dataset index
//		nSize         = size
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::GetDatasetLineThickness(int nDatasetIndex, int& nSize) const
{
	if (!IsValidDatasetIndex(nDatasetIndex))
		return FALSE;

	nSize = m_datasets[nDatasetIndex].GetSize();

	return TRUE;
}

//
//	SetDatasetPenColor
//
//	arguments
//
//		nDatasetIndex = dataset index
//		clr           = color
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::SetDatasetLineColor(int nDatasetIndex, COLORREF clr)
{
	if (!IsValidDatasetIndex(nDatasetIndex))
		return FALSE;

	return m_datasets[nDatasetIndex].SetLineColor(clr);
}

BOOL CHMXChart::SetDatasetFillColor(int nDatasetIndex, COLORREF clr)
{
	if (!IsValidDatasetIndex(nDatasetIndex))
		return FALSE;
	
	return m_datasets[nDatasetIndex].SetFillColor(clr);
}

//
//	GetDatasetPenColor
//
//	arguments
//
//		nDatasetIndex = dataset index
//		clr           = color
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::GetDatasetLineColor(int nDatasetIndex, COLORREF& clr) const
{
	if (!IsValidDatasetIndex(nDatasetIndex))
		return FALSE;

	clr = m_datasets[nDatasetIndex].GetLineColor();

	return (clr != CLR_NONE);
}

BOOL CHMXChart::GetDatasetFillColor(int nDatasetIndex, COLORREF& clr) const
{
	if (!IsValidDatasetIndex(nDatasetIndex))
		return FALSE;
	
	clr = m_datasets[nDatasetIndex].GetFillColor();
	
	return (clr != CLR_NONE);
}

//
//	SetBkGnd
//
//	arguments
//
//		clr        = color
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::SetBkGnd(COLORREF clr)
{
	m_clrBkGnd = clr;
	Redraw();

	return TRUE;
}

BOOL CHMXChart::SetGridColor(COLORREF clr)
{
	if (clr != m_clrGrid)
	{
		m_clrGrid = clr;
		m_penGrid.DeleteObject();

		Redraw();
	}

	return TRUE;
}

COLORREF CHMXChart::GetGridColor() const
{
	return m_clrGrid;
}

//
//	GetBkGnd
//
//	arguments
//
//		none
//
//	return
//
//		background color
//
COLORREF CHMXChart::GetBkGnd() const
{
	return m_clrBkGnd;
}

//
//	SetTitle
//
//	arguments
//
//		sTitle = title
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::SetTitle(CString strTitle)
{
	// If switching to/from an empty string we need to recalc data too
	BOOL bRecalcData = (m_strTitle.IsEmpty() != strTitle.IsEmpty());

	m_strTitle = strTitle;

	if (bRecalcData)
		CalcDatas();
	else
		Redraw();

	return TRUE;
}

//
//	GetTitle
//
//	arguments
//
//		none
//
//	return
//
//		title
//
CString CHMXChart::GetTitle() const
{
	return m_strTitle;
}

//
//	SetYTicks
//
//	arguments
//
//		nTicks = Y divisions (or ticks) in range 0-100
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::SetNumYTicks(int nTicks)
{
	m_nNumYTicks = min(100, max(0, nTicks));
	return TRUE;
}

//
//	GetYTicks
//
//	arguments
//
//		none
//
//	return
//
//		nTicks
//
int CHMXChart::GetNumYTicks() const
{
	return m_nNumYTicks;
}

//
//	SetRoundY
//	let's try to improve readability
//
//	arguments
//
//		nRoundY	= rounding value
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::SetRoundY(double dRound)
{
	if(dRound <= 0)
		return FALSE;
	
	m_dRoundY = dRound;

	return TRUE;
}

//
//	GetRoundY
//
//	arguments
//
//		none
//
//	return
//
//		nRoundY = rounding value
//
double CHMXChart::GetRoundY() const
{
	return m_dRoundY;
}

//
//	SetYText
//
//	arguments
//
//		sText	= text
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::SetYText(CString sText)
{
	m_strYText = sText;
	Redraw();

	return TRUE;
}

//
//	GetYText
//
//	arguments
//
//		none
//
//	return
//
//		sText	= text
//
CString CHMXChart::GetYText() const
{
	return m_strYText;
}

//
//	SetXText
//
//	arguments
//
//		sText	= text
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::SetXText(CString sText)
{
	m_strXText = sText;
	Redraw();

	return TRUE;
}

//
//	GetXText
//
//	arguments
//
//		none
//
//	return
//
//		sText	= text
//
CString CHMXChart::GetXText() const
{
	return m_strXText;
}

//
//	SetXScaleLabel
//
//	arguments
//
//		nIndex = index
//		str    = text
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::SetXScaleLabel(int nIndex, CString str)
{
	try 
	{
		m_strarrScaleXLabel.SetAtGrow(nIndex, str);
	} 
	catch(...) 
	{
		return FALSE;
	}

	return TRUE;
}

void CHMXChart::ClearXScaleLabels()
{
	m_strarrScaleXLabel.RemoveAll();
}

//
//	GetXScaleLabel
//
//	arguments
//
//		nIndex = index
//		str    = text
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::GetXScaleLabel(int nIndex, CString& str) const
{
	if(nIndex < 0 || nIndex > m_strarrScaleXLabel.GetUpperBound())
		return FALSE;
	
	str = m_strarrScaleXLabel.GetAt(nIndex);

	return TRUE;
}

//
//	SetXLabelStep
//
//	arguments
//
//		nStep = x label step
//
//	return
//
//		TRUE if ok, else FALSE
//
BOOL CHMXChart::SetXLabelStep(int nStep)
{
	m_nXLabelStep = max(nStep, 1);

	return TRUE;
}

void CHMXChart::SetXLabelsAreTicks(BOOL bTicks)
{
	m_bXLabelsAreTicks = bTicks;
}

void CHMXChart::SetXLabelAngle(int nDegrees)
{
	while (nDegrees > 360)
		nDegrees -= 360;

	while (nDegrees < 0)
		nDegrees += 360;
	
	m_nXLabelDegrees = nDegrees;
}

//
//	GetXLabelStep
//
//	arguments
//
//		none
//
//	return
//
//		nStep = x label step
//
int CHMXChart::GetXLabelStep() const
{
	return m_nXLabelStep;
}


// DDBToDIB             - Creates a DIB from a DDB
// bitmap               - Device dependent bitmap
// dwCompression        - Type of compression - see BITMAPINFOHEADER
// pPal                 - Logical palette
HANDLE CHMXChart::DDBToDIB(CBitmap& bitmap, DWORD dwCompression, CPalette* pPal) 
{
	BITMAP                  bm;
	BITMAPINFOHEADER        bi;
	LPBITMAPINFOHEADER      lpbi;
	DWORD                   dwLen;
	HANDLE                  hDIB;
	HANDLE                  handle;
	HDC                     hDC;
	HPALETTE                hPal;


	ASSERT(bitmap.GetSafeHandle());

	// The function has no arg for bitfields
	if(dwCompression == BI_BITFIELDS)
		return NULL;

	// If a palette has not been supplied use defaul palette
	hPal = (HPALETTE) pPal->GetSafeHandle();
	if (hPal==NULL)
		hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

	// Get bitmap information
	bitmap.GetObject(sizeof(bm),(LPSTR)&bm);

	// Initialize the bitmapinfoheader
	bi.biSize               = sizeof(BITMAPINFOHEADER);
	bi.biWidth              = bm.bmWidth;
	bi.biHeight             = bm.bmHeight;
	bi.biPlanes             = 1;
	bi.biBitCount           = (WORD)(bm.bmPlanes * bm.bmBitsPixel);
	bi.biCompression        = dwCompression;
	bi.biSizeImage          = 0;
	bi.biXPelsPerMeter      = 0;
	bi.biYPelsPerMeter      = 0;
	bi.biClrUsed            = 0;
	bi.biClrImportant       = 0;

	// Compute the size of the  infoheader and the color table
	int nColors = (1 << bi.biBitCount);
	if(nColors > 256) 
		nColors = 0;
	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

	// We need a device context to get the DIB from
	hDC = ::GetDC(NULL);
	hPal = SelectPalette(hDC,hPal,FALSE);
	RealizePalette(hDC);

	// Allocate enough memory to hold bitmapinfoheader and color table
	hDIB = GlobalAlloc(GMEM_FIXED,dwLen);

	if (!hDIB)
	{
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	*lpbi = bi;

	// Call GetDIBits with a NULL lpBits param, so the device driver 
	// will calculate the biSizeImage field 
	GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(), 0L, (DWORD)bi.biHeight,
	(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	// If the driver did not fill in the biSizeImage field, then compute it
	// Each scan line of the image is aligned on a DWORD (32bit) boundary
	if (bi.biSizeImage == 0)
	{
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) * bi.biHeight;

		// If a compression scheme is used the result may infact be larger
		// Increase the size to account for this.
		if (dwCompression != BI_RGB)
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}

	// Realloc the buffer so that it can hold all the bits
	dwLen += bi.biSizeImage;
	handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE);

	if (handle)
	{
		hDIB = handle;
	}
	else
	{
		GlobalFree(hDIB);

		// Reselect the original palette
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	// Get the bitmap bits
	lpbi = (LPBITMAPINFOHEADER)hDIB;

	// FINALLY get the DIB
	BOOL bGotBits = GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(),
							   0L,                             // Start scan line
							   (DWORD)bi.biHeight,             // # of scan lines
							   (LPBYTE)lpbi                    // address for bitmap bits
									   + (bi.biSize + nColors * sizeof(RGBQUAD)),
							   (LPBITMAPINFO)lpbi,             // address of bitmapinfo
							   (DWORD)DIB_RGB_COLORS);         // Use RGB for color table

	if(!bGotBits) 
	{
		GlobalFree(hDIB);

		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	SelectPalette(hDC,hPal,FALSE);
	::ReleaseDC(NULL,hDC);
	return hDIB;
}

/*************************************************************************
 *
 * PaletteSize()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * WORD             - size of the color palette of the DIB
 *
 * Description:
 *
 * This function gets the size required to store the DIB's palette by
 * multiplying the number of colors by the size of an RGBQUAD (for a
 * Windows 3.0-style DIB) or by the size of an RGBTRIPLE (for an other-
 * style DIB).
 *
 ************************************************************************/


WORD CHMXChart::PaletteSize(LPSTR lpbi)
{
   /* calculate the size required by the palette */
   if (IS_WIN30_DIB (lpbi))
	  return (WORD)(DIBNumColors(lpbi) * sizeof(RGBQUAD));
   else
	  return (WORD)(DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
}

/*************************************************************************
 *
 * DIBNumColors()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * WORD             - number of colors in the color table
 *
 * Description:
 *
 * This function calculates the number of colors in the DIB's color table
 * by finding the bits per pixel for the DIB (whether Win3.0 or other-style
 * DIB). If bits per pixel is 1: colors=2, if 4: colors=16, if 8: colors=256,
 * if 24, no colors in color table.
 *
 ************************************************************************/


WORD CHMXChart::DIBNumColors(LPSTR lpbi)
{
	WORD wBitCount;  // DIB bit count

	/*  If this is a Windows-style DIB, the number of colors in the
	 *  color table can be less than the number of bits per pixel
	 *  allows for (i.e. lpbi->biClrUsed can be set to some value).
	 *  If this is the case, return the appropriate value.
	 */

	if (IS_WIN30_DIB(lpbi))
	{
		DWORD dwClrUsed;

		dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
		if (dwClrUsed != 0)
			return (WORD)dwClrUsed;
	}

	/*  Calculate the number of colors in the color table based on
	 *  the number of bits per pixel for the DIB.
	 */
	if (IS_WIN30_DIB(lpbi))
		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	else
		wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;

	/* return number of colors based on bits per pixel */
	switch (wBitCount)
	{
		case 1:
			return 2;

		case 4:
			return 16;

		case 8:
			return 256;

		default:
			return 0;
	}
}

/*************************************************************************
 *
 * SaveDIB()
 *
 * Saves the specified DIB into the specified CFile.  The CFile
 * is opened and closed by the caller.
 *
 * Parameters:
 *
 * HDIB hDib - Handle to the dib to save
 *
 * CFile& file - open CFile used to save DIB
 *
 * Return value: TRUE if successful, else FALSE or CFileException
 *
 *************************************************************************/


BOOL CHMXChart::SaveDIB(HDIB hDib, CFile& file)
{
	BITMAPFILEHEADER bmfHdr; // Header for Bitmap file
	LPBITMAPINFOHEADER lpBI;   // Pointer to DIB info structure
	DWORD dwDIBSize;

	if (hDib == NULL)
		return FALSE;

	/*
	 * Get a pointer to the DIB memory, the first of which contains
	 * a BITMAPINFO structure
	 */
	lpBI = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) hDib);
	if (lpBI == NULL)
		return FALSE;

	if (!IS_WIN30_DIB(lpBI))
	{
		::GlobalUnlock((HGLOBAL) hDib);
		return FALSE;       // It's an other-style DIB (save not supported)
	}

	/*
	 * Fill in the fields of the file header
	 */

	/* Fill in file type (first 2 bytes must be "BM" for a bitmap) */
	bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM"

	// Calculating the size of the DIB is a bit tricky (if we want to
	// do it right).  The easiest way to do this is to call GlobalSize()
	// on our global handle, but since the size of our global memory may have
	// been padded a few bytes, we may end up writing out a few too
	// many bytes to the file (which may cause problems with some apps).
	//
	// So, instead let's calculate the size manually (if we can)
	//
	// First, find size of header plus size of color table.  Since the
	// first DWORD in both BITMAPINFOHEADER and BITMAPCOREHEADER conains
	// the size of the structure, let's use this.

	dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPSTR)lpBI);  // Partial Calculation

	// Now calculate the size of the image

	if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4))
	{
		// It's an RLE bitmap, we can't calculate size, so trust the
		// biSizeImage field

		dwDIBSize += lpBI->biSizeImage;
	}
	else
	{
		DWORD dwBmBitsSize;  // Size of Bitmap Bits only

		// It's not RLE, so size is Width (DWORD aligned) * Height

		dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;

		dwDIBSize += dwBmBitsSize;

		// Now, since we have calculated the correct size, why don't we
		// fill in the biSizeImage field (this will fix any .BMP files which
		// have this field incorrect).

		lpBI->biSizeImage = dwBmBitsSize;
	}


	// Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER)

	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;

	/*
	 * Now, calculate the offset the actual bitmap bits will be in
	 * the file -- It's the Bitmap file header plus the DIB header,
	 * plus the size of the color table.
	 */
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize
											  + PaletteSize((LPSTR)lpBI);

	TRY
	{
		// Write the file header
		file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
		//
		// Write the DIB header and the bits
		//
#if _MSC_VER < 1400
		file.WriteHuge(lpBI, dwDIBSize);
#else
		file.Write(lpBI, dwDIBSize);
#endif
	}
	CATCH (CFileException, e)
	{
		::GlobalUnlock((HGLOBAL) hDib);
		THROW_LAST();
	}
	END_CATCH

	::GlobalUnlock((HGLOBAL) hDib);
	return TRUE;
}

