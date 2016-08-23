// HMXDataset.h: interface for the CHMXDataset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HMXDATASET_H__1921781C_7E9F_11D3_BD1D_0060520365FB__INCLUDED_)
#define AFX_HMXDATASET_H__1921781C_7E9F_11D3_BD1D_0060520365FB__INCLUDED_

#include <afxtempl.h>

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define HMX_DATASET_X_AXIS			0
#define HMX_DATASET_Y_AXIS			1

#define HMX_DATASET_STYLE_LINE		1
#define HMX_DATASET_STYLE_VBAR		2
#define HMX_DATASET_STYLE_AREA		3
#define HMX_DATASET_STYLE_AREALINE	4

#define HMX_DATASET_MARKER_NONE		0
#define HMX_DATASET_MARKER_TRI		1
#define HMX_DATASET_MARKER_BOX		2
#define HMX_DATASET_MARKER_SPH		3
#define HMX_DATASET_MARKER_DIA		4

#define HMX_DATASET_VALUE_INVALID	1.7976931348623158e+308

class CHMXDataset : public CObject  
{
public:
	virtual bool		SetStyle( int nStyle );							// set data style
	virtual int			GetStyle();										// get data style

	virtual bool		SetColor( COLORREF clr );						// set data color
	virtual COLORREF	GetColor();										// get data color

	virtual bool		SetSize( int nSize );							// set pen size (in pixel)
																		// set bar size (range 1-10)
																		// unused if area
	virtual int			GetSize();										// get size

	virtual bool		SetMarker( int nMarker );						// set marker type (see #define section)
																		// unused if style is bar or area
	virtual int			GetMarker();									// get marker

	virtual bool		AddData( double nData );						// set data adding new point
	virtual bool		SetData( int nIndex, double nData );			// set data at specified index
	virtual bool		GetData( int nCount, double &nSample );			// get data
	virtual int			GetDatasetSize();								// get dataset size (how many points in dataset?)

	virtual bool		GetMinMax( double& nMin, double& nMax );		// gte min & max
	virtual void		SetMinToZero(bool bSet = true);

	virtual bool		ClearData();

	CHMXDataset();
	virtual ~CHMXDataset();

protected:
	CArray<double,double>	m_data;			// the data
	COLORREF				m_clr;			// color
	int						m_nMarker;		// marker type (see #define section)
	int						m_nSize;		// pen size if style is line
											// bar size (0-10) if style is bar
											// unused if style is area
	int						m_nStyle;		// data style (see #define section)
	bool					m_bSetMinToZero;
};

#endif // !defined(AFX_HMXDATASET_H__1921781C_7E9F_11D3_BD1D_0060520365FB__INCLUDED_)
