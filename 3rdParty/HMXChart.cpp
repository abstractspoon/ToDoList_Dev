// HMXChart.cpp : implementation file
//

#include "stdafx.h"
#include "HMXChart.h"

#include <math.h>

#define HMX_AREA_MARGINS	80		// fraction of area
#define HMX_AREA_TITLE		10		// fraction of area
#define HMX_AREA_YAXIS		7		// percentage
#define HMX_AREA_XAXIS		10		// percentage
#define HMX_AREA_MINAXIS	50		// pixels

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHMXChart

CHMXChart::CHMXChart()
{
	// set defaul value
	m_clrBkGnd = RGB(200, 255, 255);
	m_strTitle = _T("");
	m_strXText = _T("");
	m_strYText = _T("");
	m_nYTicks = 0;
	m_bShowYScale = true;
	m_nRoundY = 10;
	m_nXMax = 0;
	m_nYMin = 0;
	m_nYMax = 0;
	m_nXLabelStep = 1;
	m_clrGrid = GetSysColor(COLOR_3DSHADOW);
	m_bXLabelsAreTicks = false;
}

CHMXChart::~CHMXChart()
{
}


BEGIN_MESSAGE_MAP(CHMXChart, CWnd)
	//{{AFX_MSG_MAP(CHMXChart)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHMXChart message handlers

void CHMXChart::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	PaintBkGnd(dc);

	DrawDatasets(dc);
	DrawGrid(dc);
	DrawTitle(dc);
	DrawBaseline(dc);
	DrawAxes(dc);
	DrawYScale(dc);
	DrawXScale(dc);

}

void CHMXChart::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CalcDatas();

	// reset fonts
	m_fontXScale.DeleteObject();
	m_fontYScale.DeleteObject();
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
	DrawTitle(memDC);
	DrawGrid(memDC);
	DrawBaseline(memDC);
	DrawDatasets(memDC);
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
	DrawTitle(memDC);
	DrawGrid(memDC);
	DrawBaseline(memDC);
	DrawDatasets(memDC);
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
	font.CreateFont(m_rectTitle.Height(), 0, 0, 0, FW_NORMAL,
					 FALSE, FALSE, FALSE, ANSI_CHARSET,
					 OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
					 DEFAULT_PITCH, _T("Arial"));

	
	COLORREF clrBkOld = dc.SetBkColor(m_clrBkGnd);
	pFontOld = dc.SelectObject(&font);
	dc.DrawText(m_strTitle, m_rectTitle, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

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
	DrawVertLine(dc);
	DrawHorzLine(dc);

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
//	DrawHorzLine
//
//	arguments
//
//		dc = Decive Context
//
//	return
//
//		true if ok, else false
//
bool CHMXChart::DrawHorzLine(CDC & dc)
{
	int nTicks = GetYTicks();
	
	if(!nTicks)
		return false;

	double nY = ((m_nYMax - m_nYMin)/(double)nTicks);

	CPen pen(PS_SOLID, 1, m_clrGrid), *pPenOld;
	pPenOld = dc.SelectObject(&pen);

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
bool CHMXChart::DrawVertLine(CDC & dc)
{
	if(!m_nXMax)
		return false;

	double nX = (double)m_rectData.Width()/(double)m_nXMax;
	int f;

	CPen pen(PS_SOLID, 1, m_clrGrid), *pPenOld;
	pPenOld = dc.SelectObject(&pen);

	for(f=0; f<m_nXMax; f=f+m_nXLabelStep) 
	{
		dc.MoveTo(m_rectData.left + (int)(nX*(f+0.5)), m_rectData.top);
		dc.LineTo(m_rectData.left + (int)(nX*(f+0.5)), m_rectData.bottom + 10);
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
	CPen pen(PS_SOLID, 1, m_clrGrid), *pPenOld;
	pPenOld = dc.SelectObject(&pen);

	// cannot draw baseline outside the m_rectData
	if(m_nYMin > 0)
		return false;
	
	double nTemp = (- m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin); // this is the zero baseline

	dc.MoveTo(m_rectData.left , m_rectData.bottom - (int)nTemp);
	dc.LineTo(m_rectData.right, m_rectData.bottom - (int)nTemp);

	dc.SelectObject(pPenOld);

	return true;
}

int CHMXChart::CalcScaleFontSize(CDC& /*dc*/) const
{
	int nSize = min((m_rectXAxis.Height() / 3), (m_rectYAxis.Width() / 4));

	// also check length of scale text
	if (!m_strXText.IsEmpty())
	{
		nSize = min(nSize, (m_rectXAxis.Width() / m_strXText.GetLength()));
	}

	if (!m_strYText.IsEmpty())
	{
		nSize = min(nSize, (m_rectYAxis.Height() / m_strYText.GetLength()));
	}

	return nSize;
}

//
//	DrawXScale
//
//	arguments
//
//		dc = Decive Context
//
//	return
//
//		true if ok, else false
//
bool CHMXChart::DrawXScale(CDC & dc)
{
	int nCount = min(m_strarrScaleXLabel.GetSize(), m_nXMax);

	if (!m_bShowXScale || (!nCount && m_strXText.IsEmpty()))
		return false;
	
	int nFontSize = CalcScaleFontSize(dc);
	const int nBkModeOld = dc.SetBkMode(TRANSPARENT);
	
	if (nCount)
	{
		CFont font;
		font.CreateFont(nFontSize, 0, 0, 0, FW_NORMAL,
						 FALSE, FALSE, FALSE, ANSI_CHARSET,
						 OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
						 DEFAULT_PITCH, _T("Arial"));
		CFont* pFontOld = dc.SelectObject(&font);

		// dX is the size of a division
		double dX = (double)m_rectData.Width()/m_nXMax;

		for(int f=0; f<nCount; f=f+m_nXLabelStep) 
		{
			CRect rText(m_rectXAxis);
			
			rText.top += 4;
			rText.left += (int)(dX*(f+0.5)) + 4;
			rText.right = rText.left + (int)(dX * m_nXLabelStep);

			if (m_bXLabelsAreTicks)
				dc.DrawText(m_strarrScaleXLabel.GetAt(f), rText, DT_LEFT | DT_TOP | DT_SINGLELINE);
			else
				dc.DrawText(m_strarrScaleXLabel.GetAt(f), rText, DT_CENTER | DT_TOP | DT_SINGLELINE);
		}
	
		dc.SelectObject(pFontOld);
	}

	if(!m_strXText.IsEmpty()) 
	{
		if (m_fontXScale.GetSafeHandle() == NULL)
		{
			m_fontXScale.CreateFont(nFontSize*2, 0, 0, 0, FW_NORMAL,
								  FALSE, FALSE, FALSE, ANSI_CHARSET,
								  OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
								  DEFAULT_PITCH, _T("Arial"));
		}

		CFont* pFontOld = dc.SelectObject(&m_fontXScale);

		dc.DrawText(m_strXText, m_rectXAxis, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);
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

	if (!m_bShowYScale || (!nTicks && m_strYText.IsEmpty()))
		return false;
	
	int nFontSize = CalcScaleFontSize(dc);
	const int nBkModeOld = dc.SetBkMode(TRANSPARENT);
	
	if (nTicks)
	{
		// nY is the size of a division
		double nY = (m_nYMax - m_nYMin)/nTicks, nTemp1, nTemp2;
		CString sBuffer;
		CFont font;

		font.CreateFont(nFontSize, 0, 0, 0, FW_NORMAL,
						 FALSE, FALSE, FALSE, ANSI_CHARSET,
						 OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
						 DEFAULT_PITCH, _T("Arial"));

		CFont* pFontOld = dc.SelectObject(&font);

		// draw text
		for(int f=0; f<=nTicks; f++) 
		{
			nTemp1 = m_rectYAxis.bottom + nFontSize/2 - (nY*(f)  ) * m_rectData.Height()/(m_nYMax-m_nYMin);
			nTemp2 = m_rectYAxis.bottom + nFontSize/2 - (nY*(f+1)) * m_rectData.Height()/(m_nYMax-m_nYMin);
			sBuffer.Format(_T("%g"), m_nYMin + nY*f);
			dc.DrawText(sBuffer, CRect(m_rectYAxis.left,(int)nTemp2, m_rectYAxis.right - 4, (int)nTemp1), DT_RIGHT | DT_BOTTOM | DT_SINGLELINE);
		}

		dc.SelectObject(pFontOld);
	}

	if (!m_strYText.IsEmpty()) 
	{
		if (m_fontYScale.GetSafeHandle() == NULL)
		{
			m_fontYScale.CreateFont(nFontSize*2, 0, 900, 0, FW_NORMAL,
								  FALSE, FALSE, FALSE, ANSI_CHARSET,
								  OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
								  DEFAULT_PITCH, _T("Arial"));

		}

		CFont* pFontOld = dc.SelectObject(&m_fontYScale);

		dc.DrawText(m_strYText, m_rectYAxis, DT_BOTTOM | DT_LEFT | DT_SINGLELINE );
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
	int f;
	
	// Draw dataset from last to first so I can show 
	// first dataset in foreground, below the second dataset and so on
	for(f=HMX_MAX_DATASET-1; f>=0; f--)
		DrawDataset(dc, m_dataset[f]);

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
bool CHMXChart::DrawDataset(CDC &dc, CHMXDataset & ds)
{
	// let's calc the bar size
	double nBarWidth = (double)m_rectData.Width()/(double)m_nXMax;
	int	f, nMarkerType;
	double nFirstSample, nTemp, nTemp1, nZeroLine;

	// get first sample, if dataset is empty continue to next dataset
	f=0;
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
			// let's rock
			CPen pen(PS_SOLID, ds.GetSize(), ds.GetColor()), *pPenOld;
			CBrush brush(ds.GetColor()), *pBrushOld;
			pPenOld = dc.SelectObject(&pen);
			pBrushOld = dc.SelectObject(&brush);
			
			// nTemp will contains a parametrized data 
			nTemp = (nSample - m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin);
			dc.MoveTo(m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*f), m_rectData.bottom - (int)nTemp);
			for(; f<ds.GetDatasetSize(); f++) 
			{
				ds.GetData(f, nSample);
				if(nSample != HMX_DATASET_VALUE_INVALID) 
				{
					nTemp =  (nSample - m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin);
					dc.LineTo(m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*f), m_rectData.bottom - (int)(nTemp));
				}
			}
			
			nMarkerType = ds.GetMarker();
			for(f=0; f<ds.GetDatasetSize(); f++) 
			{
				ds.GetData(f, nSample);
				if(nMarkerType != HMX_DATASET_MARKER_NONE) 
				{
					POINT* pPoint = new POINT[ 5 ];
					ds.GetData(f, nSample);
					if(nSample != HMX_DATASET_VALUE_INVALID) 
					{
						nTemp =  (nSample - m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin);
						switch(ds.GetMarker()) 
						{
						case HMX_DATASET_MARKER_TRI:
							pPoint[ 0 ].x = m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*f);
							pPoint[ 0 ].y = m_rectData.bottom - (int)nTemp - ds.GetSize()*2;
							pPoint[ 1 ].x = m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*f) + ds.GetSize()*2;
							pPoint[ 1 ].y = m_rectData.bottom - (int)nTemp + ds.GetSize()*2;
							pPoint[ 2 ].x = m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*f) - ds.GetSize()*2;
							pPoint[ 2 ].y = m_rectData.bottom - (int)nTemp + ds.GetSize()*2;
							dc.Polygon(pPoint, 3);
							break;
						case HMX_DATASET_MARKER_BOX:
							ds.GetData(f, nSample);
							nTemp =  (nSample - m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin);
							pPoint[ 0 ].x = m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*f) - ds.GetSize()*2;
							pPoint[ 0 ].y = m_rectData.bottom - (int)nTemp - ds.GetSize()*2;
							pPoint[ 1 ].x = m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*f) + ds.GetSize()*2;
							pPoint[ 1 ].y = m_rectData.bottom - (int)nTemp - ds.GetSize()*2;
							pPoint[ 2 ].x = m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*f) + ds.GetSize()*2;
							pPoint[ 2 ].y = m_rectData.bottom - (int)nTemp + ds.GetSize()*2;
							pPoint[ 3 ].x = m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*f) - ds.GetSize()*2;
							pPoint[ 3 ].y = m_rectData.bottom - (int)nTemp + ds.GetSize()*2;
							dc.Polygon(pPoint, 4);
							break;
						case HMX_DATASET_MARKER_SPH:
							ds.GetData(f, nSample);
							nTemp =  (nSample - m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin);
							pPoint[ 0 ].x = m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*f) - ds.GetSize()*2;
							pPoint[ 0 ].y = m_rectData.bottom - (int)nTemp - ds.GetSize()*2;
							pPoint[ 1 ].x = m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*f) + ds.GetSize()*2;
							pPoint[ 1 ].y = m_rectData.bottom - (int)nTemp + ds.GetSize()*2;
							dc.Ellipse(pPoint[0].x, pPoint[0].y, pPoint[1].x, pPoint[1].y);
							break;
						case HMX_DATASET_MARKER_DIA:
							ds.GetData(f, nSample);
							nTemp =  (nSample - m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin);
							pPoint[ 0 ].x = m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*f);
							pPoint[ 0 ].y = m_rectData.bottom - (int)nTemp - ds.GetSize()*2;
							pPoint[ 1 ].x = m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*f) + ds.GetSize()*2;
							pPoint[ 1 ].y = m_rectData.bottom - (int)nTemp;
							pPoint[ 2 ].x = m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*f);
							pPoint[ 2 ].y = m_rectData.bottom - (int)nTemp + ds.GetSize()*2;
							pPoint[ 3 ].x = m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*f) - ds.GetSize()*2;
							pPoint[ 3 ].y = m_rectData.bottom - (int)nTemp;
							dc.Polygon(pPoint, 4);
							break;
						}
					}
					delete []pPoint;
				}
			}
			
			dc.SelectObject(pPenOld);
			dc.SelectObject(pBrushOld);
		}
		break;
		
	case HMX_DATASET_STYLE_VBAR:
		{
			// let's rock
			CBrush brush(ds.GetColor()), *pBrushOld;
			pBrushOld = dc.SelectObject(&brush);
			dc.SelectStockObject(NULL_PEN);
			
			CRect rectTemp;
			// nTemp will contains a parametrized data 
			for(f=0; f<ds.GetDatasetSize(); f++) 
			{
				ds.GetData(f, nSample);
				if(nSample == HMX_DATASET_VALUE_INVALID)
					continue;
				nTemp =  (nSample - m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin);
				if(nSample > 0.0) 
				{
					//  bar is positive
					
					nZeroLine = m_nYMin > 0 ? m_nYMin : 0;
					nTemp1 = (nZeroLine -m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin);
					
					rectTemp.top    = m_rectData.bottom - (int)(nTemp);
					rectTemp.bottom = m_rectData.bottom - (int)(nTemp1);
					rectTemp.left   = m_rectData.left + (int)(nBarWidth/2.0) - (int)(nBarWidth*(ds.GetSize()/2.0)/10.0) + (int)(nBarWidth*f);
					rectTemp.right  = m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*(ds.GetSize()/2.0)/10.0) + (int)(nBarWidth*f);
					// show at least 1 line bar
					rectTemp.right += (rectTemp.right == rectTemp.left) ? 1 : 0 ;
				} 
				else 
				{
					// bar is negative
					
					nZeroLine = m_nYMax < 0 ? m_nYMax : 0;
					nTemp1 = (nZeroLine -m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin);
					
					rectTemp.top    = m_rectData.bottom - (int)(nTemp1);
					rectTemp.bottom = m_rectData.bottom - (int)(nTemp);
					rectTemp.left   = m_rectData.left + (int)(nBarWidth/2.0) - (int)(nBarWidth*(ds.GetSize()/2.0)/10.0) + (int)(nBarWidth*f);
					rectTemp.right  = m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*(ds.GetSize()/2.0)/10.0) + (int)(nBarWidth*f);
					// show at least 1 line bar
					rectTemp.right += (rectTemp.right == rectTemp.left) ? 1 : 0 ;
				}
				dc.Rectangle(rectTemp);
			}
			
			dc.SelectObject(pBrushOld);
		}
		break;
		
	case HMX_DATASET_STYLE_AREALINE:
	case HMX_DATASET_STYLE_AREA:
		{
			BOOL bAreaLine = (ds.GetStyle() == HMX_DATASET_STYLE_AREALINE);
			COLORREF crArea = ds.GetColor();

			if (bAreaLine)
			{
				// lighten the colour
				BYTE bRed = GetRValue(crArea);
				BYTE bGreen = GetGValue(crArea);
				BYTE bBlue = GetBValue(crArea);

				bRed = (BYTE)min(255, (bRed * 5)/3);
				bGreen = (BYTE)min(255, (bGreen * 5)/3);
				bBlue = (BYTE)min(255, (bBlue * 5)/3);

				crArea = RGB(bRed, bGreen, bBlue);
			}

			// let's rock
			CBrush brush(crArea);
			CBrush* pBrushOld = dc.SelectObject(&brush);
			dc.SelectStockObject(NULL_PEN);
			
			// let's cale real dataset size (excluding invalid data)
			int nPoints = 0, g;
			for(g=0; g<ds.GetDatasetSize(); g++) 
			{
				ds.GetData(g, nSample);
				if(nSample != HMX_DATASET_VALUE_INVALID)
					nPoints++;
			}
			// add two points, for firs and last point 
			nPoints += 2;
			
			// create a dynamic array
			POINT* pPoint = new POINT[ nPoints ];
			
			// the first
			f=0;
			do 
			{
				ds.GetData(f, nSample);
				f++;
			} 
			while(nSample == HMX_DATASET_VALUE_INVALID);
			
			pPoint[0].x = m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*(f-1.0));
			nZeroLine = m_nYMin > 0 ? m_nYMin : 0;
			nZeroLine = m_nYMax < 0 ? m_nYMax : nZeroLine;
			nTemp = (nZeroLine -m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin);
			pPoint[0].y = m_rectData.bottom - (int)nTemp;
			
			g = 1;
			for(f=0; f<ds.GetDatasetSize(); f++) 
			{
				ds.GetData(f, nSample);
				if(nSample == HMX_DATASET_VALUE_INVALID)
					continue;
				// nTemp will contains a parametrized data 
				nTemp =  (nSample - m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin);
				pPoint[g].x = m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*f);
				pPoint[g].y = m_rectData.bottom - (int) nTemp;
				g++;
			}
			
			// the last
			//		pPoint[nPoints-1].x = m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*(g-2));
			pPoint[nPoints-1].x = pPoint[g-1].x;
			nZeroLine = m_nYMin > 0 ? m_nYMin : 0;
			nZeroLine = m_nYMax < 0 ? m_nYMax : nZeroLine;
			nTemp = (nZeroLine - m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin);
			pPoint[nPoints-1].y = m_rectData.bottom - (int) nTemp;
			dc.Polygon(pPoint, nPoints);

			// draw line too?
			if (bAreaLine)
			{
				CPen pen(PS_SOLID, 2, ds.GetColor());
				CPen* pPenOld = dc.SelectObject(&pen);

				// nTemp will contains a parametrized data 
				for(f=0; f<ds.GetDatasetSize(); f++) 
				{
					ds.GetData(f, nSample);
					if(nSample != HMX_DATASET_VALUE_INVALID) 
					{
						nTemp =  (nSample - m_nYMin) * m_rectData.Height()/(m_nYMax-m_nYMin);

						if (f == 0)
							dc.MoveTo(m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*f), m_rectData.bottom - (int)(nTemp));
						else
							dc.LineTo(m_rectData.left + (int)(nBarWidth/2.0) + (int)(nBarWidth*f), m_rectData.bottom - (int)(nTemp));
					}
				}

				dc.SelectObject(pPenOld);
			}
			
			dc.SelectObject(pBrushOld);
			
			delete []pPoint;
		}
		break;
	}

	return true;
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
	double nTemp1, nTemp2;
	int f=0, nTemp3;

	GetClientRect(m_rectArea);

	m_rectUsable.top    = m_rectArea.top    + m_rectArea.Height()/HMX_AREA_MARGINS;
	m_rectUsable.bottom = m_rectArea.bottom - m_rectArea.Height()/HMX_AREA_MARGINS;
	m_rectUsable.left   = m_rectArea.left   + m_rectArea.Width() /HMX_AREA_MARGINS;
	m_rectUsable.right  = m_rectArea.right  - m_rectArea.Width() /HMX_AREA_MARGINS;

	// let's calc everything
	if(!m_strTitle.IsEmpty()) 
	{
		m_rectTitle.top    = m_rectUsable.top;
		m_rectTitle.left   = m_rectUsable.left;
		m_rectTitle.bottom = m_rectUsable.bottom/HMX_AREA_TITLE;
		m_rectTitle.right  = m_rectUsable.right;
		m_rectGraph.top    = m_rectTitle.bottom;
		m_rectGraph.left   = m_rectUsable.left;
		m_rectGraph.bottom = m_rectUsable.bottom;
		m_rectGraph.right  = m_rectUsable.right;
	} 
	else 
	{
		m_rectGraph = m_rectUsable;
	}

	// make axis width the same for horz and vert
	int nYAxisWidth = (m_rectGraph.Width()*HMX_AREA_YAXIS)/100;
	int nXAxisHeight = (m_rectGraph.Height()*HMX_AREA_XAXIS)/100;

	int nAxisSize = max(HMX_AREA_MINAXIS, min(nXAxisHeight, nYAxisWidth));

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

	// reset 
	m_nXMax = 0;

	for(f=0; f<HMX_MAX_DATASET; f++) 
	{
		nTemp3 = m_dataset[f].GetDatasetSize();
		m_nXMax = max(m_nXMax, nTemp3);
	}
	
	// Hey man, there's nothing to draw
	if(m_nXMax == 0)
		return false;

	// search min & max in first non-empty dataset
	f = 0;
	while(!m_dataset[f].GetMinMax(m_nYMin, m_nYMax))
		f++;
	
	// let's search 
	for(; f<HMX_MAX_DATASET; f++) 
	{
		if(m_dataset[f].GetMinMax(nTemp1, nTemp2)) 
		{
			m_nYMin = min(m_nYMin ,nTemp1);
			m_nYMax = max(m_nYMax, nTemp2);
		}
	}

	// now I modify m_nYMin & m_nXMax to improve readability
	m_nYMin -= (m_nYMax - m_nYMin)*0.0; 
	m_nYMax += (m_nYMax - m_nYMin)*0.0; 

	// whid this 'strange' function I can set m_nYmin & m_nYMax so that 
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
bool CHMXChart::GetData(int nDatasetIndex, int nIndex, double& nData)
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
bool CHMXChart::SetDatasetStyle(int nDatasetIndex, int nStyle)
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
bool CHMXChart::GetDatasetStyle(int nDatasetIndex, int& nStyle)
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
bool CHMXChart::SetDatasetMarker(int nDatasetIndex, int nMarker)
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;

	return m_dataset[nDatasetIndex].SetMarker(nMarker);
}

bool CHMXChart::SetDatasetMinToZero(int nDatasetIndex, bool bSet)
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;

	m_dataset[nDatasetIndex].SetMinToZero(bSet);
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
bool CHMXChart::GetDatasetMarker(int nDatasetIndex, int& nMarker)
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
bool CHMXChart::SetDatasetPenSize(int nDatasetIndex, int nSize)
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
bool CHMXChart::GetDatasetPenSize(int nDatasetIndex, int& nSize)
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
bool CHMXChart::SetDatasetPenColor(int nDatasetIndex, COLORREF clr)
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;

	return m_dataset[nDatasetIndex].SetColor(clr);
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
bool CHMXChart::GetDatasetPenColor(int nDatasetIndex, COLORREF& clr)
{
	if(nDatasetIndex < 0 || nDatasetIndex >= HMX_MAX_DATASET)
		return false;

	clr = m_dataset[nDatasetIndex].GetColor();

	return true;
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
	m_clrGrid = clr;
	Redraw();

	return true;
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
COLORREF CHMXChart::GetBkGnd()
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
CString CHMXChart::GetTitle()
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
int CHMXChart::GetYTicks()
{
	return m_nYTicks;
}

//
//	ShowYScale
//
//	arguments
//
//		bShow = show scale if true
//
//	return
//
//		true if ok, else false
//
bool CHMXChart::ShowYScale(bool bShow)
{
	m_bShowYScale = bShow;

	return true;
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
double CHMXChart::GetRoundY()
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
CString CHMXChart::GetYText()
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
CString CHMXChart::GetXText()
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
bool CHMXChart::GetXScaleLabel(int nIndex, CString& str)
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
int CHMXChart::GetXLabelStep()
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

