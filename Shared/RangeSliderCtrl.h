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
	CRangeSliderCtrl(UINT nThumbStyle);

	void SetParentBackgroundColor(COLORREF crBkgnd);
	void SetThumbStyle(UINT nStyle = TBS_BOTTOM);

	virtual void DrawRegion(CDC& dc, RS_DRAWREGION nRegion, const CRect& rRegion);
	virtual void DrawButton(CDC& dc, RS_DRAWREGION nRegion, const CRect& rButton, const CString& sText, BOOL bPressed);

protected:
	CHotTracker m_hotTrack;
	COLORREF m_crParentBkgnd;
	UINT m_nThumbStyle;

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

	void RegionToTrack(CRect& rRegion) const;
	BOOL Initialize();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RANGESLIDERCTRL_H__74C5339F_0A22_4008_88C8_3A566B69785E__INCLUDED_)
