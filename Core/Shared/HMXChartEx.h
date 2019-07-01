#if !defined(AFX_HMXCHARTEX_H__B68E359E_553E_4F0A_98E0_A8CB76B62E03__INCLUDED_)
#define AFX_HMXCHARTEX_H__B68E359E_553E_4F0A_98E0_A8CB76B62E03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HMXChartEx.h : header file
//

#include "..\3rdParty\HMXChart.h"

/////////////////////////////////////////////////////////////////////////////
// CHMXChartEx window

class CHMXChartEx : public CHMXChart
{
// Construction
public:
	CHMXChartEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMXChartEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHMXChartEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHMXChartEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	double CalcYAxisInterval(double dDataMax) const;
	double CalcMaxYAxisValue(double dDataMax) const;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMXCHARTEX_H__B68E359E_553E_4F0A_98E0_A8CB76B62E03__INCLUDED_)
