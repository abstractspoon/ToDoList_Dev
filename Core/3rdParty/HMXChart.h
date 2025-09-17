#if !defined(AFX_HMXCHART_H__19217819_7E9F_11D3_BD1D_0060520365FB__INCLUDED_)
#define AFX_HMXCHART_H__19217819_7E9F_11D3_BD1D_0060520365FB__INCLUDED_

#include <afxtempl.h>
#include <io.h>
#include <errno.h>
#include <direct.h>

#include "HMXDataset.h"
#include "GdiPlus.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HMXChart.h : header file
//

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
	#define HMX_MAX_DATASET	16
#else
	#define HMX_MAX_DATASET	64
#endif	// _DEBUG

/////////////////////////////////////////////////////////////////////////////

enum 
{
	HMX_RENDER_TITLE		= 0x0001,
	HMX_RENDER_XAXIS		= 0x0004,
	HMX_RENDER_YAXIS		= 0x0008,
	HMX_RENDER_XAXISTITLE	= 0x0010,
	HMX_RENDER_YAXISTITLE	= 0x0020,
	HMX_RENDER_XAXISSCALE	= 0x0040,
	HMX_RENDER_YAXISSCALE	= 0x0080,
	HMX_RENDER_HORZGRID		= 0x0100,
	HMX_RENDER_VERTGRID		= 0x0200,
	HMX_RENDER_BASELINE		= 0x0400,
	HMX_RENDER_AXES			= HMX_RENDER_XAXIS | HMX_RENDER_YAXIS | HMX_RENDER_XAXISTITLE | HMX_RENDER_YAXISTITLE | HMX_RENDER_XAXISSCALE | HMX_RENDER_YAXISSCALE,
	HMX_RENDER_GRID			= HMX_RENDER_HORZGRID | HMX_RENDER_VERTGRID,

	HMX_RENDER_ALL			= 0xffff
};

/////////////////////////////////////////////////////////////////////////////
// CHMXChart window

class CHMXChart : public CWnd
{
// Construction
public:
	CHMXChart();
	virtual ~CHMXChart();

// Attributes
public:
	void SetFont(LPCTSTR szFaceName, int nPointSize = -1);
	void SetRenderFlags(DWORD dwFlags, BOOL bRedraw = TRUE);
	DWORD ModifyRenderFlags(DWORD dwRemove, DWORD dwAdd, BOOL bRedraw = TRUE);
	BOOL HasRenderFlag(DWORD dwFlag) const { return ((m_dwRenderFlags & dwFlag) == dwFlag); }
	CSize GetDataArea() const { return m_rectData.Size(); }
	BOOL HasData() const { return m_datasets[0].HasData(); }

public:
	// export chart
	virtual BOOL		CopyToFile( CString sFile );
	virtual BOOL		CopyToFile();
	virtual BOOL		CopyToClipboard();

	// X labels scale functions
	virtual BOOL		SetXLabelStep( int nStep );
	virtual int			GetXLabelStep() const;
	void				SetXLabelsAreTicks(BOOL bTicks = TRUE);
	void				SetXLabelAngle(int nDegrees);

	virtual BOOL		SetXScaleLabel( int nIndex, CString str );
	virtual BOOL		GetXScaleLabel( int nIndex, CString& str ) const;
	void				ClearXScaleLabels();

	// X text
	virtual BOOL		SetXText( CString sText );
	virtual CString		GetXText() const;

	// Y text
	virtual BOOL		SetYText( CString sText );
	virtual CString		GetYText() const;

	// useful Y functions
	virtual BOOL		SetRoundY( double dRound );
	virtual double		GetRoundY() const;

	virtual BOOL		SetNumYTicks( int nTicks );
	virtual int			GetNumYTicks() const;

	// main title functions
	virtual	BOOL		SetTitle( CString strTitle );
	virtual	CString		GetTitle() const;

	// background functions
	virtual BOOL		SetBkGnd( COLORREF clr );
	virtual COLORREF	GetBkGnd() const;

	virtual BOOL		SetGridColor(COLORREF clr);
	virtual COLORREF	GetGridColor() const;

	// dataset functions
	virtual BOOL		SetDatasetLineColor( int nDatasetIndex, COLORREF clr );
	virtual BOOL		GetDatasetLineColor( int nDatasetIndex, COLORREF& clr ) const;
	virtual BOOL		SetDatasetFillColor( int nDatasetIndex, COLORREF clr );
	virtual BOOL		GetDatasetFillColor( int nDatasetIndex, COLORREF& clr ) const;

	virtual BOOL		SetDatasetSizeFactor( int nDatasetIndex, int nSize );
	virtual BOOL		GetDatasetLineThickness( int nDatasetIndex, int& nSize ) const;

	virtual BOOL		SetDatasetStyle( int nDatasetIndex, HMX_DATASET_STYLE nStyle );
	virtual BOOL		GetDatasetStyle( int nDatasetIndex, HMX_DATASET_STYLE& nStyle ) const;

	virtual BOOL		SetDatasetMarker( int nDatasetIndex, HMX_DATASET_MARKER nMarker );
	virtual BOOL		GetDatasetMarker( int nDatasetIndex, HMX_DATASET_MARKER& nMarker ) const;
	
	virtual BOOL		AddData( int nDatasetIndex, double dData );
	virtual BOOL		SetData( int nDatasetIndex, int nIndex, double dData );
	virtual BOOL 		GetData( int nDatasetIndex, int nIndex, double& dData ) const;

	virtual BOOL		ResetDataset(int nDatasetIndex);
	virtual void		ResetDatasets();
	virtual BOOL		ClearData( int nDatasetIndex);
	virtual void		ClearData();

	virtual BOOL		SetDatasetMin(int nDatasetIndex, double dMin);
	virtual BOOL		SetDatasetMax(int nDatasetIndex, double dMax);
	virtual BOOL		GetMinMax(double& dMin, double& dnMax, BOOL bDataOnly) const;

	// useful global functions
	virtual BOOL		CalcDatas();
	virtual BOOL		Redraw();

	// Generated message map functions
protected:
	
	virtual BOOL PaintBkGnd(CDC &dc);						
	virtual BOOL DrawTitle( CDC &dc );						
	virtual BOOL DrawGrid( CDC& dc);						
	virtual BOOL DrawAxes(CDC &dc);							
	virtual BOOL DrawHorzGridLines( CDC &dc);					
	virtual BOOL DrawVertGridLines( CDC &dc);					
	virtual BOOL DrawBaseline( CDC& dc );					
	virtual BOOL DrawXScale( CDC& dc);						
	virtual BOOL DrawYScale( CDC& dc);						
	virtual BOOL DrawDataset(CDC &dc, int nDatasetIndex, BYTE fillOpacity = 255);	
	virtual BOOL DrawDatasets(CDC &dc);						
	virtual void DoPaint(CDC& dc, BOOL bPaintBkgnd = TRUE);

	virtual COLORREF GetLineColor(int nDatasetIndex, double dValue) const;
	virtual COLORREF GetFillColor(int nDatasetIndex, double dValue) const;
	virtual CString GetYTickText(int nTick, double dValue) const;
	
	virtual int CalcXScaleFontSize(BOOL bTitle) const;
	virtual int CalcYScaleFontSize(BOOL bTitle) const;
	virtual int CalcTitleFontSize() const;

	virtual BOOL XScaleHasRTLDates() const { return FALSE; }
	virtual BOOL YScaleHasRTLDates() const { return FALSE; }

	CStringArray	m_strarrScaleXLabel;					// x labels

	int				m_nXLabelStep;							// x label step
	int				m_nNumYTicks;							// y ticks
	int				m_nXMax;								// max x value
	int				m_nCountDataset;						// dataset counter
	int				m_nXLabelDegrees;
	int				m_nFontPixelSize;						// -1 -> dynamic sizing

	double			m_dRoundY;								// y scale rounding
	double			m_dYMax;								// max y value
	double			m_dYMin;								// min y value

	CHMXDataset		m_datasets[HMX_MAX_DATASET];			// datasets

	CString			m_strTitle;								// main title
	CString			m_strYText;								// Y text
	CString			m_strXText;								// X text
	CString			m_strFont;

	CRect			m_rectUsable;							// usable area
	CRect			m_rectData;								// data area
	CRect			m_rectGraph;							// graph area
	CRect			m_rectXAxis;							// x axis area
	CRect			m_rectYAxis;							// y axis area
	CRect			m_rectTitle;							// main title area
	CRect			m_rectArea;								// entire control area

	COLORREF		m_clrBkGnd;								// background color
	COLORREF		m_clrGrid;	

	BOOL			m_bXLabelsAreTicks;
	DWORD			m_dwRenderFlags;

	CPen			m_penGrid;

	//{{AFX_MSG(CHMXChart)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnPrintClient(WPARAM wp, LPARAM lp);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int GetPoints(const CHMXDataset& ds, CArray<gdix_PointF, gdix_PointF&>& points, BOOL bArea) const;
	BOOL GetMarker(HMX_DATASET_MARKER nMarker, const gdix_PointF& pt, float fSize, CArray<gdix_PointF, gdix_PointF&>& ptMarker) const;
	int CalcAxisSize(const CRect& rAvail, CDC& dc) const;
	BOOL CreateXAxisFont(BOOL bTitle, CFont& font) const;
	BOOL CreateYAxisFont(BOOL bTitle, CFont& font) const;
	BOOL GetMinMax(double& dMin, double& dMax, BOOL bDataOnly, double dIgnoreVal) const;
	UINT GetXScaleDrawFlags() const;
	UINT GetYScaleDrawFlags() const;

	BOOL DrawDataset(CDC &dc, const CHMXDataset& dataset, const CDWordArray& aAltItemColors, BYTE fillOpacity = 255);
	BOOL DrawLineGraph(CDC &dc, const CHMXDataset& dataset, const CDWordArray& aAltMarkerColors, BYTE fillOpacity = 255);
	BOOL DrawAreaGraph(CDC &dc, const CHMXDataset& dataset, BYTE fillOpacity = 255);
	BOOL DrawBarChart(CDC &dc, const CHMXDataset& dataset, const CDWordArray& aAltBarColors, BYTE fillOpacity = 255);
	BOOL DrawPieChart(CDC &dc, const CHMXDataset& dataset, const CDWordArray& aAltPieColors, BYTE fillOpacity = 255);
	BOOL DrawMinMaxChart(CDC& dc, const CHMXDataset& dsMin, const CHMXDataset& dsMax, BYTE fillOpacity = 255);

	struct PIESEGMENT
	{
		CString sLabel;
		float fStartDegrees, fSweepDegrees;
	};
	int CalcPieSegments(const CHMXDataset& dataset, CArray<PIESEGMENT, PIESEGMENT&>& aSegments) const;
	void DrawPieLabels(CDC& dc, const CRect& rPie, const CArray<PIESEGMENT, PIESEGMENT&>& aSegments);
	BOOL CalcPieRects(CRect& rPie, CRect& rDonut) const;
	
	inline BOOL IsValidDatasetIndex(int nDatasetIndex) const
	{
		return ((nDatasetIndex >= 0) && (nDatasetIndex < HMX_MAX_DATASET));
	}

	float CalcRelativeYValue(double dDataValue) const;
	BOOL GetPointXY(int nDatasetIndex, int nIndex, CPoint& point, double dBarWidth = -1) const;
	BOOL GetPointXY(int nDatasetIndex, int nIndex, gdix_PointF& point, double dBarWidth = -1) const;

	static BOOL CreateDefaultItemDrawingTools(const CHMXDataset& dataset, const CDWordArray& aColors, BYTE fillOpacity, CGdiPlusPen& pen, CGdiPlusBrush& brush);
	static BOOL CreateItemDrawingTools(int nItem, const CDWordArray& aColors, BYTE fillOpacity, CGdiPlusPen& pen, CGdiPlusBrush& brush);
	static float NormaliseAngle(float fDegrees);
	static float ScaleByDPIFactor(int nValue);

	DECLARE_HANDLE(HDIB);

	HANDLE DDBToDIB( CBitmap& bitmap, DWORD dwCompression, CPalette* pPal );	
	BOOL SaveDIB (HDIB hDib, CFile& file);									
	WORD PaletteSize( LPSTR lpbi );											
	WORD DIBNumColors(LPSTR lpbi);											
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMXCHART_H__19217819_7E9F_11D3_BD1D_0060520365FB__INCLUDED_)
