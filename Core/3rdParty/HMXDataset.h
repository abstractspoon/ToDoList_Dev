// HMXDataset.h: interface for the CHMXDataset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HMXDATASET_H__1921781C_7E9F_11D3_BD1D_0060520365FB__INCLUDED_)
#define AFX_HMXDATASET_H__1921781C_7E9F_11D3_BD1D_0060520365FB__INCLUDED_

#include <afxtempl.h>

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//////////////////////////////////////////////////////////////////////

enum HMX_DATASET_AXIS
{
	HMX_DATASET_X_AXIS = 0,
	HMX_DATASET_Y_AXIS,
};

enum HMX_DATASET_STYLE
{
	HMX_DATASET_STYLE_NONE = 0,
	HMX_DATASET_STYLE_LINE,
	HMX_DATASET_STYLE_LINE_DOTTED,
	HMX_DATASET_STYLE_LINE_DASHED,
	HMX_DATASET_STYLE_VBAR,
	HMX_DATASET_STYLE_AREA,
	HMX_DATASET_STYLE_AREALINE,
	HMX_DATASET_STYLE_PIE,
	HMX_DATASET_STYLE_PIELINE,
	HMX_DATASET_STYLE_DONUT,
	HMX_DATASET_STYLE_DONUTLINE,
};

enum HMX_DATASET_MARKER
{
	HMX_DATASET_MARKER_NONE = 0,
	HMX_DATASET_MARKER_TRIANGLE,
	HMX_DATASET_MARKER_SQUARE,
	HMX_DATASET_MARKER_CIRCLE,
	HMX_DATASET_MARKER_DIAMOND,
};

//////////////////////////////////////////////////////////////////////

#define HMX_DATASET_VALUE_INVALID	1.7976931348623158e+308

//////////////////////////////////////////////////////////////////////

class CHMXDataset : public CObject  
{
public:
	CHMXDataset();
	virtual ~CHMXDataset();

	void Copy(const CHMXDataset& other);

	virtual bool		SetStyle( HMX_DATASET_STYLE nStyle );			// set data style
	virtual HMX_DATASET_STYLE GetStyle() const;							// get data style

	virtual bool		SetLineColor( COLORREF clr );					// set data color
	virtual COLORREF	GetLineColor() const;							// get data color
	virtual bool		SetFillColor( COLORREF clr );					// set data color
	virtual COLORREF	GetFillColor() const;							// get data color

	virtual int			GetSize() const;								// get size
	virtual bool		SetSize( int nSize );							// set pen size (pixels) OR bar size (1-10)	OR unused if area

	virtual void		Reset();										// clears data and attributes
	virtual void		ClearData();									// clears data only
	virtual bool		HasData() const;

	virtual bool		SetMarker( HMX_DATASET_MARKER nMarker );		// set marker type (see #define section) unused if style is bar or area
	virtual HMX_DATASET_MARKER GetMarker() const;						// get marker

	virtual bool		AddData( double nData );						// set data adding new point
	virtual bool		SetData( int nIndex, double nData );			// set data at specified index
	virtual bool		GetData( int nCount, double &nSample ) const;	// get data
	virtual void		SetDatasetSize(int nSize);						// set dataset size (how many points in dataset?)
	virtual int			GetDatasetSize() const;							// get dataset size (how many points in dataset?)

	virtual bool		GetMinMax( double& nMin, double& nMax, bool bDataOnly ) const;	// get min & max
	virtual void		SetMin(double dMin);
	virtual void		SetMax(double dMax);

protected:
	CArray<double,double>	m_data;			// the data
	COLORREF				m_crLine, m_crFill;	// colors
	HMX_DATASET_MARKER		m_nMarker;		// marker type (see #define section)
	int						m_nSize;		// pen size if style is line
											// bar size (0-10) if style is bar
											// unused if style is area
	HMX_DATASET_STYLE		m_nStyle;		// data style (see #define section)

	bool					m_bSetMinTo, m_bSetMaxTo;
	double					m_dSetMinTo, m_dSetMaxTo;

};

#endif // !defined(AFX_HMXDATASET_H__1921781C_7E9F_11D3_BD1D_0060520365FB__INCLUDED_)
