#if !defined(AFX_HMXCHART_H__19217819_7E9F_11D3_BD1D_0060520365FB__INCLUDED_)
#define AFX_HMXCHART_H__19217819_7E9F_11D3_BD1D_0060520365FB__INCLUDED_

#include <afxtempl.h>
#include <io.h>
#include <errno.h>
#include <direct.h>

#include "HMXDataset.h"	// Added by ClassView

#include "..\3rdParty\GdiPlus.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HMXChart.h : header file
//

#ifdef _DEBUG
	#define HMX_MAX_DATASET	16
#else
	#define HMX_MAX_DATASET	64
#endif	// _DEBUG

/* Handle to a DIB */
DECLARE_HANDLE(HDIB);

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
// CHMXChart window

class CHMXChart : public CWnd
{
// Construction
public:
	CHMXChart();

// Attributes
public:
	void SetFont(LPCTSTR szFaceName, int nPointSize = -1);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMXChart)
	//}}AFX_VIRTUAL

// Implementation
public:

	// export chart
	virtual bool		CopyToFile( CString sFile );
	virtual bool		CopyToFile();
	virtual bool		CopyToClipboard();

	// X labels scale functions
	virtual bool		SetXLabelStep( int nStep );
	virtual int			GetXLabelStep() const;
	void				SetXLabelsAreTicks(bool bTicks = TRUE);
	void				SetXLabelAngle(int nDegrees);

	virtual bool		SetXScaleLabel( int nIndex, CString str );
	virtual bool		GetXScaleLabel( int nIndex, CString& str ) const;
	void				ClearXScaleLabels();

	// X text
	virtual bool		SetXText( CString sText );
	virtual CString		GetXText() const;

	// Y text
	virtual bool		SetYText( CString sText );
	virtual CString		GetYText() const;

	// useful Y functions
	virtual bool		SetRoundY( double nRound );
	virtual double		GetRoundY() const;

	virtual bool		SetYTicks( int nTicks );
	virtual int			GetYTicks() const;

	// main title functions
	virtual	bool		SetTitle( CString strTitle );
	virtual	CString		GetTitle() const;

	// background functions
	virtual bool		SetBkGnd();
	virtual bool		SetBkGnd( COLORREF clr );
	virtual COLORREF	GetBkGnd() const;

	virtual bool		SetGridColor(COLORREF clr);
	virtual COLORREF	GetGridColor() const;

	// dataset functions
	virtual bool		SetDatasetLineColor( int nDatasetIndex, COLORREF clr );
	virtual bool		GetDatasetLineColor( int nDatasetIndex, COLORREF& clr ) const;
	virtual bool		SetDatasetFillColor( int nDatasetIndex, COLORREF clr );
	virtual bool		GetDatasetFillColor( int nDatasetIndex, COLORREF& clr ) const;

	virtual bool		SetDatasetSizeFactor( int nDatasetIndex, int nSize );
	virtual bool		GetDatasetLineThickness( int nDatasetIndex, int& nSize ) const;

	virtual bool		SetDatasetStyle( int nDatasetIndex, HMX_DATASET_STYLE nStyle );
	virtual bool		GetDatasetStyle( int nDatasetIndex, HMX_DATASET_STYLE& nStyle ) const;

	virtual bool		SetDatasetMarker( int nDatasetIndex, HMX_DATASET_MARKER nMarker );
	virtual bool		GetDatasetMarker( int nDatasetIndex, HMX_DATASET_MARKER& nMarker ) const;
	
	virtual bool		AddData( int nDatasetIndex, double nData );
	virtual bool		SetData( int nDatasetIndex, int nIndex, double nData );
	virtual bool 		GetData( int nDatasetIndex, int nIndex, double& nData ) const;

	virtual bool		ResetDataset(int nDatasetIndex);
	virtual void		ResetDatasets();
	virtual bool		ClearData( int nDatasetIndex);
	virtual void		ClearData();
	CSize				GetDataArea() const { return m_rectData.Size(); }
	bool				HasData() const { return m_datasets[0].HasData(); }

	virtual bool		SetDatasetMin(int nDatasetIndex, double dMin);
	virtual bool		SetDatasetMax(int nDatasetIndex, double dMax);
	virtual bool		GetMinMax(double& nMin, double& nMax, bool bDataOnly) const;	// get min & max

	// useful global functions
	virtual bool		CalcDatas();
	virtual bool		Redraw();

	virtual HANDLE      DDBToDIB( CBitmap& bitmap, DWORD dwCompression, CPalette* pPal );	// functions used to
	virtual WORD WINAPI PaletteSize( LPSTR lpbi );											// save the chart in
	virtual WORD WINAPI DIBNumColors(LPSTR lpbi);											// a BMP file or in
	virtual BOOL WINAPI SaveDIB (HDIB hDib, CFile& file);									// the clipboard

	virtual ~CHMXChart();

	// Generated message map functions
protected:
	
	virtual bool PaintBkGnd(CDC &dc);						
	virtual bool DrawTitle( CDC &dc );						
	virtual bool DrawGrid( CDC& dc);						
	virtual bool DrawAxes(CDC &dc);							
	virtual bool DrawHorzGridLines( CDC &dc);					
	virtual bool DrawVertGridLines( CDC &dc);					
	virtual bool DrawBaseline( CDC& dc );					
	virtual bool DrawXScale( CDC& dc );						
	virtual bool DrawYScale( CDC& dc );						
	virtual bool DrawDataset(CDC &dc, int nDatasetIndex, BYTE alpha = 255);	
	virtual bool DrawDatasets(CDC &dc);						
	virtual void DoPaint(CDC& dc, BOOL bPaintBkgnd = TRUE);

	virtual COLORREF GetLineColor(int nDatasetIndex, double dValue) const;
	virtual COLORREF GetFillColor(int nDatasetIndex, double dValue) const;
	
	virtual int CalcXScaleFontSize(BOOL bTitle) const;
	virtual int CalcYScaleFontSize(BOOL bTitle) const;
	virtual int CalcTitleFontSize() const;

	CStringArray	m_strarrScaleXLabel;					// x labels
	int				m_nXLabelStep;							// x label step
	double			m_nRoundY;								// y scale rounding
	int				m_nYTicks;								// y ticks
	int				m_nXMax;								// max x value
	double			m_nYMax;								// max y value
	double			m_nYMin;								// min y value
	CHMXDataset		m_datasets[HMX_MAX_DATASET];				// datasets
	int				m_nCountDataset;						// dataset counter
	CString			m_strTitle;								// main title
	CString			m_strYText;								// Y text
	CString			m_strXText;								// X text
	CRect			m_rectUsable;							// usable area
	CRect			m_rectData;								// data area
	CRect			m_rectGraph;							// graph area
	CRect			m_rectXAxis;							// x axis area
	CRect			m_rectYAxis;							// y axis area
	CRect			m_rectTitle;							// main title area
	CRect			m_rectArea;								// entire control area
	COLORREF		m_clrBkGnd;								// background color
	COLORREF		m_clrGrid;	
	bool			m_bXLabelsAreTicks;
	int				m_nXLabelDegrees;

	CString			m_strFont;
	int				m_nFontPixelSize;						// -1 -> dynamic sizing

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
	BOOL GetMarker(HMX_DATASET_MARKER nMarker, const gdix_PointF& pt, int nSize, CArray<gdix_PointF, gdix_PointF&>& ptMarker) const;
	int CalcAxisSize(const CRect& rAvail, CDC& dc) const;
	BOOL CreateXAxisFont(BOOL bTitle, CFont& font) const;
	BOOL CreateYAxisFont(BOOL bTitle, CFont& font) const;

	inline BOOL IsValidDatasetIndex(int nDatasetIndex) const
	{
		return ((nDatasetIndex >= 0) && (nDatasetIndex < HMX_MAX_DATASET));
	}

	BOOL GetPointXY(int nDatasetIndex, int nIndex, CPoint& point, double nBarWidth = -1) const;
	BOOL GetPointXY(int nDatasetIndex, int nIndex, gdix_PointF& point, double nBarWidth = -1) const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMXCHART_H__19217819_7E9F_11D3_BD1D_0060520365FB__INCLUDED_)
