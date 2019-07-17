// HMXChart.cpp : implementation file
//

#include "stdafx.h"
#include "HMXChart.h"
#include "GDIPlus.h"

#include <math.h>

/////////////////////////////////////////////////////////////////////////////

#define HMX_AREA_MARGINS	80		// fraction of area
#define HMX_AREA_TITLE		10		// fraction of area
#define HMX_AREA_YAXIS		7		// percentage
#define HMX_AREA_XAXIS		10		// percentage
#define HMX_AREA_MINAXIS	50		// pixels
#define HMX_XSCALE_OFFSET   14		// pixels

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHMXChart

CHMXChart::CHMXChart() : m_strFont(_T("Arial")), m_nFontPixelSize(-1)
{
	// set defaul value
	m_clrBkGnd = RGB(200, 255, 255);
	m_strTitle = _T("");
	m_strXText = _T("");
	m_strYText = _T("");
	m_nYTicks = 0;
	m_nRoundY = 10;
	m_nXMax = 0;
	m_nYMin = 0;
	m_nYMax = 0;
	m_nXLabelStep = 1;
	m_clrGrid = GetSysColor(COLOR_3DSHADOW);
	m_bXLabelsAreTicks = false;
	m_nXLabelDegrees = 0;
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

void CHMXChart::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CDC             memDC;
	memDC.CreateCompatibleDC(&dc);

	CBitmap         bitmap;
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

	// Recreate pens, etc
	if (!m_penGrid.GetSafeHandle())
		m_penGrid.CreatePen(PS_SOLID, 1, m_clrGrid);

	dc.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));

	DrawGrid(dc);
	DrawDatasets(dc);
	DrawTitle(dc);
	DrawBaseline(dc);
	DrawAxes(dc);
	DrawYScale(dc);
	DrawXScale(dc);
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

bool CHMXChart::CopyToClipboard()
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

	PaintBkGnd(memDC);
	DrawGrid(memDC);
	DrawDatasets(memDC);
	DrawTitle(memDC);
	DrawBaseline(memDC);
	DrawAxes(memDC);
	DrawYScale(memDC);
	DrawXScale(memDC);

	this->OpenClipboard() ;
	EmptyClipboard() ;
	SetClipboardData (CF_BITMAP, bitmap.GetSafeHandle()) ;
	CloseClipboard () ;

	bitmap.Detach();
	memDC.SelectObject(pOldBitmap);

	EndWaitCursor();

	return true;
}

bool CHMXChart::CopyToFile()
{
	CFileDialog dlg(false, _T("BMP"), _T("Chart"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Bitmap |*.bmp|"));
	if(dlg.DoModal() == IDOK)
		return CopyToFile(dlg.GetPathName());

	return false;

}

bool CHMXChart::CopyToFile(CString sFile)
{

	CBitmap 	bitmap;
	CWindowDC	dc(this);
	CDC 		memDC;
	CFile		file;
	
	memDC.CreateCompatibleDC(&dc); 
	bitmap.CreateCompatibleBitmap(&dc, m_rectArea.Width(), m_rectArea.Height());	
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	
	PaintBkGnd(memDC);
	DrawGrid(memDC);
	DrawDatasets(memDC);
	DrawTitle(memDC);
	DrawBaseline(memDC);
	DrawAxes(memDC);
	DrawYScale(memDC);
	DrawXScale(memDC);

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
		return false;

	// Write it to file
	if(!file.Open(sFile, CFile::modeWrite|CFile::modeCreate))
		return FALSE;

	SaveDIB((HDIB)hDIB, file);

	// Free the memory allocated by DDBToDIB for the DIB
	GlobalFree(hDIB);

	bitmap.Detach();
	memDC.SelectObject(pOldBitmap);

	return true;
}

///////////////////////////////////////////////////////////////////////

//
//	PaintBkGnd
//
//	arguments
//
//		dc = Decive Context
//
//	return
//
//		true if ok, else false
//
bool CHMXChart::PaintBkGnd(CDC &dc)
{
	dc.FillSolidRect(m_rectArea, m_clrBkGnd);
	return true;
}

//
//	DrawTitle
//
//	arguments
//
//		dc = Decive Context
//
//	return
//
//		true if ok, else false
//
bool CHMXChart::DrawTitle(CDC & dc)
{
	if(m_strTitle.IsEmpty())
		return false;

	CFont font, *pFontOld;
	int nFontSize = CalcTitleFontSize();

	font.CreateFont(nFontSize, 0, 0, 0, FW_NORMAL,
					 FALSE, FALSE, FALSE, ANSI_CHARSET,
					 OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
					 DEFAULT_PITCH, m_strFont);

	
	COLORREF clrBkOld = dc.SetBkColor(m_clrBkGnd);
	pFontOld = dc.SelectObject(&font);
	dc.DrawText(m_strTitle, m_rectTitle, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS);

	dc.SetBkColor(clrBkOld);
	dc.SelectObject(pFontOld);

	return true;
}

//
//	DrawGrid
//
//	arguments
//
//		dc = Decive Context
//
//	return
//
//		true if ok, else false
//
bool CHMXChart::DrawGrid(CDC & dc)
{
	DrawVertGridLines(dc);
	DrawHorzGridLines(dc);

	return true;
}

//
//	DrawAxes
//
//	arguments
//
//		dc = Decive Context
//
//	return
//
//		true if ok, else false
//
bool CHMXChart::DrawAxes(CDC &dc)
{
	// draw Y
	dc.MoveTo(m_rectYAxis.right, m_rectYAxis.bottom);
	dc.LineTo(m_rectYAxis.right, m_rectYAxis.top   );
	
	// draw X
	dc.MoveTo(m_rectXAxis.left , m_rectXAxis.top);
	dc.LineTo(m_rectXAxis.right, m_rectXAxis.top);
	return true;
}

//
//	DrawHorzGridLines
//
//	arguments
//
//		dc = Decive Context
//
//	return
//
//		true if ok, else false
//
bool CHMXChart::DrawHorzGridLines(CDC & dc)
{
	int nTicks = GetYTicks();
	
	if(!nTicks)
		return false;

	double nY = ((m_nYMax - m_nYMin)/(double)nTicks);
	CPen* pPenOld = dc.SelectObject(&m_penGrid);

	for(int f=0; f<=nTicks; f++) 
	{
		double nTemp = m_rectData.bottom - (nY*f) * m_rectData.Height()/(m_nYMax-m_nYMin);

		dc.MoveTo(m_rectData.left , (int)nTemp);
		dc.LineTo(m_rectData.right, (int)nTemp);
	}

	dc.SelectObject(pPenOld);

	return true;
}


//
//	DrawVertLine
//
//	arguments
//
//		dc = Decive Context
//
//	return
//
//		true if ok, else false
//
bool CHMXChart::DrawVertGridLines(CDC & dc)
{
	if(!m_nXMax)
		return false;

	double nX = (double)m_rectData.Width()/(double)m_nXMax;
	CPen* pPenOld = dc.SelectObject(&m_penGrid);

	for(int f=0; f<m_nXMax; f=f+m_nXLabelStep) 
	{
		dc.MoveTo(m_rectData.left + (int)(nX*(f+0.5)), m_rectData.top);
		dc.LineTo(m_rectData.left + (int)(nX*(f+0.5)), m_rectData.bottom + 8);
	}

	dc.MoveTo(m_rectData.left + m_rectData.Width() - 1, m_rectData.top);
	dc.LineTo(m_rectData.left + m_rectData.Width() - 1, m_rectData.bottom);

	dc.SelectObject(pPenOld);

	return true;
}

//
//	DrawBaseLine
//
//	arguments
//
//		dc = Decive Context
//
//	return
//
//		true if ok, else false
//
bool CHMXChart::DrawBaseline(CDC & dc)
{
	CPen* pPenOld = dc.SelectObject(&m_penGrid);

	// cannot draw baseline outside the m_rectData
	if(m_nYMin > 0)
		return false;
	
	double nTemp = (- m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin); // this is the zero baseline

	dc.MoveTo(m_rectData.left , m_rectData.bottom - (int)nTemp);
	dc.LineTo(m_rectData.right, m_rectData.bottom - (int)nTemp);

	dc.SelectObject(pPenOld);

	return true;
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
	int nSize = m_nFontPixelSize;

	if (nSize == -1)
		nSize = (m_rectYAxis.Width() / 4);

	if (bTitle)
	{
		nSize *= 2;

		// also check length of scale text
		if (!m_strYText.IsEmpty())
			nSize = min(nSize, 2 * (m_rectYAxis.Height() / m_strYText.GetLength()));
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
//		true if ok, else false
//
bool CHMXChart::DrawXScale(CDC & dc)
{
	int nCount = min(m_strarrScaleXLabel.GetSize(), m_nXMax);

	if (!nCount && m_strXText.IsEmpty())
		return false;
	
	const int nBkModeOld = dc.SetBkMode(TRANSPARENT);
	
	if (nCount)
	{
		CFont font;
		VERIFY(CreateXAxisFont(FALSE, font));
		CFont* pFontOld = dc.SelectObject(&font);

		// dX is the size of a division
		double dX = (double)m_rectData.Width()/m_nXMax;

		for(int f=0; f<nCount; f=f+m_nXLabelStep) 
		{
			CRect rText(m_rectXAxis);
			
			rText.top += HMX_XSCALE_OFFSET;
			rText.left += (int)(dX*(f+0.5)) + 4;

			if (m_nXLabelDegrees > 0)
			{
				dc.SetTextAlign(TA_BASELINE | TA_RIGHT);
				dc.TextOut(rText.left, rText.top, m_strarrScaleXLabel.GetAt(f));
			}
			else
			{
				rText.right = rText.left + (int)(dX * m_nXLabelStep);

				if (m_bXLabelsAreTicks)
					dc.DrawText(m_strarrScaleXLabel.GetAt(f), rText, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS);
				else
					dc.DrawText(m_strarrScaleXLabel.GetAt(f), rText, DT_CENTER | DT_TOP | DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS);
			}
		}
	
		dc.SelectObject(pFontOld);
	}

	if (!m_strXText.IsEmpty()) 
	{
		CFont font;
		VERIFY(CreateXAxisFont(TRUE, font));

		CFont* pFontOld = dc.SelectObject(&font);
		dc.DrawText(m_strXText, m_rectXAxis, DT_CENTER | DT_BOTTOM | DT_SINGLELINE | DT_NOPREFIX);

		dc.SelectObject(pFontOld);
	}

	dc.SetBkMode(nBkModeOld);

	return true;
}

//
//	DrawYScale
//
//	arguments
//
//		dc = Decive Context
//
//	return
//
//		true if ok, else false
//
bool CHMXChart::DrawYScale(CDC & dc)
{
	int nTicks = GetYTicks();

	if (!nTicks && m_strYText.IsEmpty())
		return false;
	
	const int nBkModeOld = dc.SetBkMode(TRANSPARENT);
	CRect rTitle(m_rectYAxis);
	
	if (nTicks)
	{
		CFont font;
		VERIFY(CreateYAxisFont(FALSE, font));

		CFont* pFontOld = dc.SelectObject(&font);

		// nY is the size of a division
		double nY = (m_nYMax - m_nYMin)/nTicks, nTemp1, nTemp2;
		int nFontSize = CalcYScaleFontSize(FALSE);
		CString sBuffer;

		// draw text
		for(int f=0; f<=nTicks; f++) 
		{
			nTemp1 = m_rectYAxis.bottom + nFontSize/2 - (nY*(f)  ) * m_rectData.Height()/(m_nYMax-m_nYMin);
			nTemp2 = m_rectYAxis.bottom + nFontSize/2 - (nY*(f+1)) * m_rectData.Height()/(m_nYMax-m_nYMin);

			CRect rTemp(m_rectYAxis.left, (int)nTemp2, m_rectYAxis.right - 4, (int)nTemp1);
			sBuffer.Format(_T("%g"), m_nYMin + nY*f);

			dc.DrawText(sBuffer, &rTemp, DT_RIGHT | DT_BOTTOM | DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS);

			int nLabelLeft = (m_rectYAxis.right - 4 - dc.GetTextExtent(sBuffer).cx);
			rTitle.right = min(rTitle.right, nLabelLeft);
		}

		dc.SelectObject(pFontOld);
	}

	if (!m_strYText.IsEmpty()) 
	{
		CFont font;
		VERIFY(CreateYAxisFont(TRUE, font));

		CFont* pFontOld = dc.SelectObject(&font);

		dc.SetTextAlign(TA_CENTER | TA_BASELINE);
		dc.TextOut(rTitle.CenterPoint().x, rTitle.CenterPoint().y, m_strYText);

		dc.SelectObject(pFontOld);
	}

	dc.SetBkMode(nBkModeOld);

	return true;
}

//
//	DrawDatasets
//
//	arguments
//
//		dc = Decive Context
//
//	return
//
//		true if ok, else false
//
bool CHMXChart::DrawDatasets(CDC& dc)
{
	int f = HMX_MAX_DATASET;
	
	// Draw dataset from last to first so I can show 
	// first dataset in foreground, below the second dataset and so on
	while (f--)
	{
		DrawDataset(dc, f, 128);
	}

	return true;
}

//
//	DrawDataset
//
//	arguments
//
//		dc = Decive Context
//		ds = Dataset
//
//	return
//
//		true if ok, else false
//

BOOL CHMXChart::GetMarker(HMX_DATASET_MARKER nMarker, const gdix_PointF& pt, int nSize, CArray<gdix_PointF, gdix_PointF&>& ptMarker) const
{
	switch (nMarker) 
	{
	case HMX_DATASET_MARKER_TRIANGLE:
		ptMarker.SetSize(3);
		ptMarker[ 0 ].x = pt.x;
		ptMarker[ 0 ].y = pt.y - nSize;
		ptMarker[ 1 ].x = pt.x + nSize;
		ptMarker[ 1 ].y = pt.y + nSize;
		ptMarker[ 2 ].x = pt.x - nSize;
		ptMarker[ 2 ].y = pt.y + nSize;
		break;

	case HMX_DATASET_MARKER_SQUARE:
	case HMX_DATASET_MARKER_CIRCLE:
		ptMarker.SetSize(2);
		ptMarker[ 0 ].x = pt.x - nSize;
		ptMarker[ 0 ].y = pt.y - nSize;
		ptMarker[ 1 ].x = pt.x + nSize;
		ptMarker[ 1 ].y = pt.y + nSize;
		break;

	case HMX_DATASET_MARKER_DIAMOND:
		ptMarker.SetSize(4);
		ptMarker[ 0 ].x = pt.x;
		ptMarker[ 0 ].y = pt.y - nSize;
		ptMarker[ 1 ].x = pt.x + nSize;
		ptMarker[ 1 ].y = pt.y;
		ptMarker[ 2 ].x = pt.x;
		ptMarker[ 2 ].y = pt.y + nSize;
		ptMarker[ 3 ].x = pt.x - nSize;
		ptMarker[ 3 ].y = pt.y;
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

bool CHMXChart::DrawDataset(CDC &dc, int nDatasetIndex, BYTE alpha)
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;

	CHMXDataset& ds = m_dataset[nDatasetIndex];

	if (ds.GetDatasetSize() == 0)
		return false;

	// let's calc the bar size
	double dSpacing = (double)m_rectData.Width()/(double)m_nXMax;
	double nFirstSample, nTemp, nTemp1, nZeroLine;

	// get first sample, if dataset is empty continue to next dataset
	int f=0;
	do 
	{
		if(!ds.GetData(f++, nFirstSample))
			return false;
	} 
	while(nFirstSample == HMX_DATASET_VALUE_INVALID);
	f--;

	double nSample = nFirstSample;

	switch (ds.GetStyle())
	{
	case HMX_DATASET_STYLE_LINE:
		{
			CArray<gdix_PointF, gdix_PointF&> points;
			int nPoints = GetPoints(ds, points, FALSE);

			if (nPoints < 2)
				return false;

			CGdiPlusGraphics graphics(dc);
			CGdiPlusPen pen(ds.GetLineColor(), ds.GetSize());

			VERIFY(CGdiPlus::DrawPolygon(graphics, pen, points.GetData(), points.GetSize()));
			
			HMX_DATASET_MARKER nMarker = ds.GetMarker();

			if (nMarker == HMX_DATASET_MARKER_NONE) 
				break;

			CGdiPlusBrush brush(ds.GetFillColor(), alpha);
			int nSize = ds.GetSize()*2;

			CArray<gdix_PointF, gdix_PointF&> ptMarker;

			for (f=0; f<nPoints; f++) 
			{
				VERIFY(GetMarker(nMarker, points[f], nSize, ptMarker));

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
		break;
		
	case HMX_DATASET_STYLE_VBAR:
		{
			CGdiPlusGraphics graphics(dc);
			CGdiPlusPen pen(ds.GetLineColor(), 1);
			CGdiPlusBrush brush(ds.GetFillColor(), alpha);

			CRect rectTemp;

			for(f=0; f<ds.GetDatasetSize(); f++) 
			{
				ds.GetData(f, nSample);

				if(nSample == HMX_DATASET_VALUE_INVALID)
					continue;

				nTemp =  (nSample - m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin);

				CRect rBar;

				if(nSample > 0.0) 
				{
					//  bar is positive
					nZeroLine = m_nYMin > 0 ? m_nYMin : 0;
					nTemp1 = (nZeroLine -m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin);
					
					rBar.top    = m_rectData.bottom - (int)(nTemp);
					rBar.bottom = m_rectData.bottom - (int)(nTemp1);
				} 
				else 
				{
					// bar is negative
					nZeroLine = m_nYMax < 0 ? m_nYMax : 0;
					nTemp1 = (nZeroLine -m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin);
					
					rBar.top    = m_rectData.bottom - (int)(nTemp1);
					rBar.bottom = m_rectData.bottom - (int)(nTemp);
				}

				int nBarPos = m_rectData.left + (int)(dSpacing*(f + 0.5));

				rBar.left   = (nBarPos - (int)(dSpacing*(ds.GetSize()/2.0)/10.0));
				rBar.right  = (nBarPos + (int)(dSpacing*(ds.GetSize()/2.0)/10.0));

				if (rBar.Width() <= 0)
					rBar.right = (rBar.left + 1);

				VERIFY(CGdiPlus::DrawRect(graphics, pen, CGdiPlusRectF(rBar), brush));
			}
		}
		break;
		
	case HMX_DATASET_STYLE_AREALINE:
	case HMX_DATASET_STYLE_AREA:
		{
			BOOL bAreaLine = (ds.GetStyle() == HMX_DATASET_STYLE_AREALINE);

			CGdiPlusGraphics graphics(dc);
			CGdiPlusBrush brush(ds.GetFillColor(), alpha);
			
			CArray<gdix_PointF, gdix_PointF&> points;
			int nPoints = GetPoints(ds, points, TRUE);

			if (nPoints > 4)
			{
				VERIFY(CGdiPlus::FillPolygon(graphics, brush, points.GetData(), nPoints));

				// draw line too?
				if (bAreaLine)
				{
					CGdiPlusPen pen(ds.GetLineColor(), ds.GetSize());
				
					// don't draw the first/last closure points
					VERIFY(CGdiPlus::DrawLines(graphics, pen, points.GetData() + 1, nPoints - 2));
				}
			}
		}
		break;
	}

	return true;
}

COLORREF CHMXChart::GetLineColor(int nDatasetIndex, double dValue) const
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return CLR_NONE;

	return m_dataset[nDatasetIndex].GetLineColor();
}

COLORREF CHMXChart::GetFillColor(int nDatasetIndex, double dValue) const
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return CLR_NONE;

	return m_dataset[nDatasetIndex].GetFillColor();
}

int CHMXChart::GetPoints(const CHMXDataset& ds, CArray<gdix_PointF, gdix_PointF&>& points, BOOL bArea) const
{
	// let's calc real dataset size (excluding invalid data)
	int nPoints = 0, g;
	double nSample;

	for(g=0; g<ds.GetDatasetSize(); g++) 
	{
		ds.GetData(g, nSample);
		if(nSample != HMX_DATASET_VALUE_INVALID)
			nPoints++;
	}
	// add two points, for first and last point 
	if (bArea)
		nPoints += 2;

	points.SetSize(nPoints);
	
	double nBarWidth = (double)m_rectData.Width()/(double)m_nXMax;

	// the first (area only)
	g = 0;
	if (bArea)
	{
		int f=0;
		do 
		{
			ds.GetData(f, nSample);
			f++;
		} 
		while(nSample == HMX_DATASET_VALUE_INVALID);

		points[0].x = m_rectData.left + (float)(nBarWidth/2.0) + (int)(nBarWidth*(f-1.0));
		double nZeroLine = m_nYMin > 0 ? m_nYMin : 0;
		nZeroLine = m_nYMax < 0 ? m_nYMax : nZeroLine;
		double nTemp = (nZeroLine -m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin);
		points[0].y = m_rectData.bottom - (float)nTemp;

		g++;
	}

	for(int f=0; f<ds.GetDatasetSize(); f++) 
	{
		ds.GetData(f, nSample);
		if(nSample == HMX_DATASET_VALUE_INVALID)
			continue;
		// nTemp will contains a parametrized data 
		double nTemp =  (nSample - m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin);
		points[g].x = m_rectData.left + (float)(nBarWidth/2.0) + (int)(nBarWidth*f);
		points[g].y = m_rectData.bottom - (float) nTemp;

		g++;
	}
	
	// the last (area only)
	if (bArea)
	{
		points[nPoints-1].x = points[g-1].x;
		double nZeroLine = m_nYMin > 0 ? m_nYMin : 0;
		nZeroLine = m_nYMax < 0 ? m_nYMax : nZeroLine;
		double nTemp = (nZeroLine - m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin);
		points[nPoints-1].y = m_rectData.bottom - (float) nTemp;
	}

	return points.GetSize();
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
//		true if ok, else false
//
bool CHMXChart::CalcDatas()
{
	int f=0, nTemp3;

	GetClientRect(m_rectArea);

	int nMargin = 15/*(max(m_rectArea.Height(), m_rectArea.Width()) / HMX_AREA_MARGINS)*/;

	m_rectUsable = m_rectArea;
	m_rectUsable.DeflateRect(nMargin, nMargin);

	// let's calc everything
	m_rectGraph = m_rectUsable;

	if (!m_strTitle.IsEmpty()) 
	{
		m_rectTitle = m_rectUsable;

		m_rectTitle.bottom = (m_rectUsable.bottom / HMX_AREA_TITLE);
		m_rectGraph.top    = m_rectTitle.bottom;
	} 

	// make axis width the same for horz and vert
	int nAxisSize = 0;
	{
		CClientDC dc(this);
		nAxisSize = CalcAxisSize(m_rectGraph, dc);
	}

	m_rectYAxis.top    = m_rectGraph.top;
	m_rectYAxis.left   = m_rectGraph.left;
	m_rectYAxis.bottom = m_rectGraph.top  + m_rectGraph.Height() - nAxisSize;
	m_rectYAxis.right  = m_rectGraph.left + nAxisSize;

	m_rectXAxis.top    = m_rectGraph.top  + m_rectGraph.Height() - nAxisSize;
	m_rectXAxis.left   = m_rectGraph.left + nAxisSize;
	m_rectXAxis.bottom = m_rectGraph.bottom;
	m_rectXAxis.right  = m_rectGraph.right;

	m_rectData.top     = m_rectGraph.top;
	m_rectData.bottom  = m_rectXAxis.top;
	m_rectData.left    = m_rectYAxis.right;
	m_rectData.right   = m_rectGraph.right;

	m_nXMax = 0;

	for(f=0; f<HMX_MAX_DATASET; f++) 
	{
		nTemp3 = m_dataset[f].GetDatasetSize();
		m_nXMax = max(m_nXMax, nTemp3);
	}
	
	GetMinMax(m_nYMin, m_nYMax, false);

	// with this 'strange' function I can set m_nYmin & m_nYMax so that 
	// they are multiply of m_nRoundY
	if(m_nRoundY > 0.0) 
	{
		if (fmod(m_nYMin, m_nRoundY) != 0.0)
			m_nYMin = (((int)m_nYMin-(int)m_nRoundY)/(int)m_nRoundY)*m_nRoundY;

		if (fmod(m_nYMax, m_nRoundY) != 0.0)
			m_nYMax = (((int)m_nYMax+(int)m_nRoundY)/(int)m_nRoundY)*m_nRoundY;
	}

	// now nYMin & nYMax contain absolute min and absolute max
	// and these data can be used to calc the graphic's Y scale factor
	// nXMax contains the maximum number of elements, useful to 
	// calculate the X scale factor

	// prevent divide by zero
	m_nYMax = max(m_nYMax, m_nYMin + 10);

	return true;
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
		int nXAxisHeight = (m_strXText.IsEmpty() ? 0 : (m_nFontPixelSize * 2));
		int nYAxisWidth = (m_strYText.IsEmpty() ? 0 : (m_nFontPixelSize * 2));

		if (m_strarrScaleXLabel.GetSize())
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

		if (m_nYTicks > 0)
		{
			CFont font;
			VERIFY(CreateYAxisFont(FALSE, font));

			CFont* pFontOld = dc.SelectObject(&font);

			CString sBuffer;
			sBuffer.Format(_T("%g"), m_nYMax);
			nYAxisWidth += dc.GetTextExtent(sBuffer).cy;
			
			dc.SelectObject(pFontOld);
		}

		nAxisSize = max(HMX_AREA_MINAXIS, max(nXAxisHeight, nYAxisWidth));
	}
	
	return nAxisSize;
}

bool CHMXChart::GetMinMax(double& nMin, double& nMax, bool bDataOnly) const
{
	// Get the min and max of all our datasets
	int f = 0;

	while((f < HMX_MAX_DATASET) && !m_dataset[f].GetMinMax(nMin, nMax, bDataOnly))
		f++;

	if (f == HMX_MAX_DATASET)
		return false;

	double nTemp1, nTemp2;
	
	for(; f<HMX_MAX_DATASET; f++) 
	{
		if (m_dataset[f].GetMinMax(nTemp1, nTemp2, bDataOnly)) 
		{
			nMin = min(nMin, nTemp1);
			nMax = max(nMax, nTemp2);
		}
	}

	return true;
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
//		true if ok, else false
//
bool CHMXChart::Redraw()
{
	Invalidate(true);
	UpdateWindow();

	return true;
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
//		true if ok, else false
//
bool CHMXChart::AddData(int nDatasetIndex, double nData)
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;

	return m_dataset[nDatasetIndex].AddData(nData);

}

bool CHMXChart::ClearData( int nDatasetIndex)
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;

	return m_dataset[nDatasetIndex].ClearData();

}

void CHMXChart::ClearData()
{
	for (int f=0; f<HMX_MAX_DATASET; f++) 
		ClearData(f);
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
//		true if ok, else false
//
bool CHMXChart::SetData(int nDatasetIndex, int nIndex, double nData)
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;

	return m_dataset[nDatasetIndex].SetData(nIndex, nData);

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
//		true if ok, else false
//
bool CHMXChart::GetData(int nDatasetIndex, int nIndex, double& nData) const
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;

	return m_dataset[nDatasetIndex].GetData(nIndex,nData);

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
//		true if ok, else false
//
bool CHMXChart::SetDatasetStyle(int nDatasetIndex, HMX_DATASET_STYLE nStyle)
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;

	return m_dataset[nDatasetIndex].SetStyle(nStyle);
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
//		true if ok, else false
//
bool CHMXChart::GetDatasetStyle(int nDatasetIndex, HMX_DATASET_STYLE& nStyle) const
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;

	nStyle = m_dataset[nDatasetIndex].GetStyle();

	return true;
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
//		true if ok, else false
//
bool CHMXChart::SetDatasetMarker(int nDatasetIndex, HMX_DATASET_MARKER nMarker)
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;

	return m_dataset[nDatasetIndex].SetMarker(nMarker);
}

bool CHMXChart::SetDatasetMin(int nDatasetIndex, double dMin)
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;

	m_dataset[nDatasetIndex].SetMin(dMin);
	return true;
}

bool CHMXChart::SetDatasetMax(int nDatasetIndex, double dMax)
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;

	m_dataset[nDatasetIndex].SetMax(dMax);
	return true;
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
//		true if ok, else false
//
bool CHMXChart::GetDatasetMarker(int nDatasetIndex, HMX_DATASET_MARKER& nMarker) const
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;

	nMarker = m_dataset[nDatasetIndex].GetMarker();

	return true;
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
//		true if ok, else false
//
bool CHMXChart::SetDatasetSizeFactor(int nDatasetIndex, int nSize)
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;

	return m_dataset[nDatasetIndex].SetSize(nSize);
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
//		true if ok, else false
//
bool CHMXChart::GetDatasetLineThickness(int nDatasetIndex, int& nSize) const
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;

	nSize = m_dataset[nDatasetIndex].GetSize();

	return true;
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
//		true if ok, else false
//
bool CHMXChart::SetDatasetLineColor(int nDatasetIndex, COLORREF clr)
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;

	return m_dataset[nDatasetIndex].SetLineColor(clr);
}

bool CHMXChart::SetDatasetFillColor(int nDatasetIndex, COLORREF clr)
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;
	
	return m_dataset[nDatasetIndex].SetFillColor(clr);
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
//		true if ok, else false
//
bool CHMXChart::GetDatasetLineColor(int nDatasetIndex, COLORREF& clr) const
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;

	clr = m_dataset[nDatasetIndex].GetLineColor();

	return (clr != CLR_NONE);
}

bool CHMXChart::GetDatasetFillColor(int nDatasetIndex, COLORREF& clr) const
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;
	
	clr = m_dataset[nDatasetIndex].GetFillColor();
	
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
//		true if ok, else false
//
bool CHMXChart::SetBkGnd(COLORREF clr)
{
	m_clrBkGnd = clr;
	Redraw();

	return true;
}

bool CHMXChart::SetGridColor(COLORREF clr)
{
	if (clr != m_clrGrid)
	{
		m_clrGrid = clr;
		m_penGrid.DeleteObject();

		Redraw();
	}

	return true;
}

COLORREF CHMXChart::GetGridColor() const
{
	return m_clrGrid;
}

//
//	SetBkGnd
//
//	arguments
//
//		none
//
//	return
//
//		true if ok, else false
//
bool CHMXChart::SetBkGnd()
{
	CColorDialog dlg(GetBkGnd());

	if(dlg.DoModal() == IDOK)
		return SetBkGnd(dlg.GetColor());
	else
		return false;

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
//		true if ok, else false
//
bool CHMXChart::SetTitle(CString strTitle)
{
	m_strTitle = strTitle;
	Redraw();

	return true;
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
//		true if ok, else false
//
bool CHMXChart::SetYTicks(int nTicks)
{
	m_nYTicks = nTicks;
	m_nYTicks = min(m_nYTicks, 100);
	m_nYTicks = max(m_nYTicks, 0);

	return true;
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
int CHMXChart::GetYTicks() const
{
	return m_nYTicks;
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
//		true if ok, else false
//
bool CHMXChart::SetRoundY(double nRound)
{
	if(nRound <= 0)
		return false;
	
	m_nRoundY = nRound;

	return true;
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
	return m_nRoundY;
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
//		true if ok, else false
//
bool CHMXChart::SetYText(CString sText)
{
	m_strYText = sText;
	Redraw();

	return true;
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
//		true if ok, else false
//
bool CHMXChart::SetXText(CString sText)
{
	m_strXText = sText;
	Redraw();

	return true;
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
//		true if ok, else false
//
bool CHMXChart::SetXScaleLabel(int nIndex, CString str)
{
	try 
	{
		m_strarrScaleXLabel.SetAtGrow(nIndex, str);
	} 
	catch(...) 
	{
		return false;
	}

	return true;
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
//		true if ok, else false
//
bool CHMXChart::GetXScaleLabel(int nIndex, CString& str) const
{
	if(nIndex < 0 || nIndex > m_strarrScaleXLabel.GetUpperBound())
		return false;
	
	str = m_strarrScaleXLabel.GetAt(nIndex);

	return true;
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
//		true if ok, else false
//
bool CHMXChart::SetXLabelStep(int nStep)
{
	m_nXLabelStep = max(nStep, 1);

	return true;
}

void CHMXChart::SetXLabelsAreTicks(bool bTicks)
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


WORD WINAPI CHMXChart::PaletteSize(LPSTR lpbi)
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


WORD WINAPI CHMXChart::DIBNumColors(LPSTR lpbi)
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


BOOL WINAPI CHMXChart::SaveDIB(HDIB hDib, CFile& file)
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

