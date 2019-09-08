#if !defined(AFX_HMXCHARTEX_H__B68E359E_553E_4F0A_98E0_A8CB76B62E03__INCLUDED_)
#define AFX_HMXCHARTEX_H__B68E359E_553E_4F0A_98E0_A8CB76B62E03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HMXChartEx.h : header file
//

#include "..\3rdParty\HMXChart.h"

#include "..\Shared\TooltipCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////

namespace HMXUtils
{
	bool GetMinMax(const CHMXDataset datasets[], int nNumSets, double& nMin, double& nMax, bool bDataOnly);	// get min & max

	double CalcYAxisInterval(double dDataMax, int nNumTicks);
	double CalcMaxYAxisValue(double dDataMax, int nNumTicks);
};

/////////////////////////////////////////////////////////////////////////////
// CHMXChartEx window

class CHMXChartEx : public CHMXChart
{
// Construction
public:
	CHMXChartEx();
	virtual ~CHMXChartEx();

	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void FilterToolTipMessage(MSG* pMsg);

protected:
	CToolTipCtrlEx m_tooltip;

	mutable int m_nLastTooltipHit;

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMXChartEx)
	//}}AFX_VIRTUAL
	virtual bool DrawHorzGridLines(CDC& dc);
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	virtual CString GetTooltip(int nHit) const;
	virtual int HitTest(const CPoint& ptClient) const;
	virtual void DoPaint(CDC& dc, BOOL bPaintBkgnd);
	virtual BOOL AdjustTooltipRect(CRect& rScreen);

	// Generated message map functions
	//{{AFX_MSG(CHMXChartEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnShowTooltip(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

protected:
	BOOL InitTooltip(BOOL bMultiline);
	int GetYSubTicks(double dInterval) const;
	BOOL GetAveragePointXY(int nIndex, CPoint& point) const;

	BOOL HighlightDataPoints(int nIndex);
	void HideLastHighlightedPoint();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMXCHARTEX_H__B68E359E_553E_4F0A_98E0_A8CB76B62E03__INCLUDED_)
