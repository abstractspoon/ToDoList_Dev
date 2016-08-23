#if !defined(AFX_HMXCHART_H__19217819_7E9F_11D3_BD1D_0060520365FB__INCLUDED_)
#define AFX_HMXCHART_H__19217819_7E9F_11D3_BD1D_0060520365FB__INCLUDED_

#include <afxtempl.h>
#include <io.h>
#include <errno.h>
#include <direct.h>

#include "HMXDataset.h"	// Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HMXChart.h : header file
//

#ifdef _DEBUG
	#define HMX_MAX_DATASET	6
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
	virtual int			GetXLabelStep();
	void				SetXLabelsAreTicks(bool bTicks = TRUE);

	virtual bool		SetXScaleLabel( int nIndex, CString str );
	virtual bool		GetXScaleLabel( int nIndex, CString& str );
	void				ClearXScaleLabels();

	// X text
	virtual bool		SetXText( CString sText );
	virtual CString		GetXText();

	// Y text
	virtual bool		SetYText( CString sText );
	virtual CString		GetYText();

	// useful Y functions
	virtual bool		SetRoundY( double nRound );
	virtual double		GetRoundY();

	virtual bool		ShowYScale( bool bShow );

	virtual bool		SetYTicks( int nTicks );
	virtual int			GetYTicks();

	// main title functions
	virtual	bool		SetTitle( CString strTitle );
	virtual	CString		GetTitle();

	// background functions
	virtual bool		SetBkGnd();
	virtual bool		SetBkGnd( COLORREF clr );
	virtual COLORREF	GetBkGnd();

	virtual bool		SetGridColor(COLORREF clr);

	// dataset functions
	virtual bool		SetDatasetPenColor( int nDatasetIndex, COLORREF clr );
	virtual bool		GetDatasetPenColor( int nDatasetIndex, COLORREF& clr );

	virtual bool		SetDatasetPenSize( int nDatasetIndex, int nSize );
	virtual bool		GetDatasetPenSize( int nDatasetIndex, int& nSize );

	virtual bool		SetDatasetStyle( int nDatasetIndex, int nStyle );
	virtual bool		GetDatasetStyle( int nDatasetIndex, int& nStyle );

	virtual bool		SetDatasetMarker( int nDatasetIndex, int nMarker );
	virtual bool		GetDatasetMarker( int nDatasetIndex, int& nMarker );

	virtual bool		AddData( int nDatasetIndex, double nData );
	virtual bool		SetData( int nDatasetIndex, int nIndex, double nData );
	virtual bool 		GetData( int nDatasetIndex, int nIndex, double& nData );

	virtual bool		ClearData( int nDatasetIndex);
	CSize				GetDataArea() const { return m_rectData.Size(); }
	virtual bool		SetDatasetMinToZero(int nDatasetIndex, bool bSet = true);

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
	
	virtual bool PaintBkGnd(CDC &dc);						// paint backgroud
	virtual bool DrawTitle( CDC &dc );						// draw main title
	virtual bool DrawGrid( CDC& dc);						// draw grid
	virtual bool DrawAxes(CDC &dc);							// draw axes
	virtual bool DrawHorzLine( CDC &dc);					// draw horz line
	virtual bool DrawVertLine( CDC &dc);					// draw vert line
	virtual bool DrawBaseline( CDC& dc );					// draw baseline
	virtual bool DrawXScale( CDC& dc );						// draw x scale
	virtual bool DrawYScale( CDC& dc );						// draw y scale
	virtual bool DrawDataset(CDC &dc, CHMXDataset &ds );	// draw specific dataset
	virtual bool DrawDatasets(CDC &dc);						// draw all datasets

	CStringArray	m_strarrScaleXLabel;					// x labels
	int				m_nXLabelStep;							// x label step
	double			m_nRoundY;								// y scale rounding
	bool			m_bShowXScale;							// show x scale
	bool			m_bShowYScale;							// show y scale
	int				m_nYTicks;								// y ticks
	int				m_nXMax;								// max x value
	double			m_nYMax;								// max y value
	double			m_nYMin;								// min y value
	CHMXDataset		m_dataset[HMX_MAX_DATASET];				// datasets
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
	COLORREF		m_clrBkGnd;								// backgroung color
	COLORREF		m_clrGrid;	
	CFont			m_fontXScale, m_fontYScale;
	bool			m_bXLabelsAreTicks;

	//{{AFX_MSG(CHMXChart)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int CalcScaleFontSize(CDC& dc) const;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMXCHART_H__19217819_7E9F_11D3_BD1D_0060520365FB__INCLUDED_)
