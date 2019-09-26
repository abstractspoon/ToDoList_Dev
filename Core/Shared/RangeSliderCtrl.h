#if !defined(AFX_RANGESLIDERCTRL_H__74C5339F_0A22_4008_88C8_3A566B69785E__INCLUDED_)
#define AFX_RANGESLIDERCTRL_H__74C5339F_0A22_4008_88C8_3A566B69785E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RangeSliderCtrl.h : header file
//

#include "HotTracker.h"

#include "..\3rdParty\RangeSlider.h"

/////////////////////////////////////////////////////////////////////////////
// CRangeSliderCtrl window

class CRangeSliderCtrl : public CRangeSlider
{
// Construction
public:
	CRangeSliderCtrl(UINT nThumbStyle = TBS_BOTTOM | TBS_NOTICKS);

	void SetParentBackgroundColor(COLORREF crBkgnd);
	BOOL SetMinMaxRangeWidths(double dMinWidth = 0, double dMaxWidth = -1);
	BOOL HasSelectedRange() const;
	BOOL SetMinTickSpacing(int nPixels); // affects tick visibility only

protected:
	CHotTracker m_hotTrack;
	COLORREF m_crParentBkgnd;
	UINT m_nSliderDrawStyles;
	double m_dMinRangeWidth, m_dMaxRangeWidth;
	int m_nMinTickSpacing;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRangeSliderCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRangeSliderCtrl();
	virtual void PreSubclassWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRangeSliderCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnHotChange(WPARAM wp, LPARAM lp);
	
	DECLARE_MESSAGE_MAP()

	virtual void DrawRegion(CDC& dc, RS_DRAWREGION nRegion, const CRect& rRegion);
	virtual void DrawButton(CDC& dc, BUTTON_ID nBtn, const CRect& rButton, const CString& sText);
	virtual BOOL IsValidMove(double dLeft, double dRight) const;

	void DrawTicks(CDC& dc, const CRect& rTrack, double dFrom, double dTo);
	void RegionToTrack(CRect& rRegion) const;
	BOOL Initialize();
	BOOL IsHot(BUTTON_ID nBtn) const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RANGESLIDERCTRL_H__74C5339F_0A22_4008_88C8_3A566B69785E__INCLUDED_)
