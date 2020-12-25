#if !defined(AFX_RANGESLIDER_H__CEC8D48F_0FDC_415C_A751_3AF12AF8B736__INCLUDED_)
#define AFX_RANGESLIDER_H__CEC8D48F_0FDC_415C_A751_3AF12AF8B736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RangeSlider.h : header file
//

extern const UINT RANGE_CHANGED;
/*
   Registered Window Message

   WPARAM: One of enum _RangeMessages
   LPARAM: NULL (all the time)
 */

/////////////////////////////////////////////////////////////////////////////
// CRangeSlider window

enum _RangeMessages {
	RS_LEFTCHANGED,
	RS_RIGHTCHANGED,
	RS_BOTHCHANGED,
};

class CRangeSlider : public CWnd
{
// Construction
public:
	CRangeSlider();

	void Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CCreateContext *pContext = NULL);
// Attributes
public:
	void SetMinMax(double min, double max);   //< Set Interval [Min, Max] of RangeSlider
	void SetRange(double left, double right); //< Set Selected Range in [Min, Max] 
	void SetStep(double step = -1); // -1 == no step

	double GetMin() const { return (m_bInvertedMode ? -m_Max : m_Min); };    
	double GetMax() const { return (m_bInvertedMode ? -m_Min : m_Max); };
	double GetLeft() const { return (m_bInvertedMode ? -m_Right : m_Left); };
	double GetRight() const { return (m_bInvertedMode ? -m_Left : m_Right); };
	double GetRangeWidth() const { return (m_Right - m_Left); }

	void GetMinMax(double &min, double &max) const 
	{ 
		min = (m_bInvertedMode) ? -m_Max : m_Min; 
		max = (m_bInvertedMode) ? -m_Min : m_Max; 
	};

	void GetRange(double &left, double &right) const 
	{ 
		left = (m_bInvertedMode) ? -m_Right : m_Left; 
		right = (m_bInvertedMode) ? -m_Left : m_Right; 
	};

	double GetVisualMin() const { return (m_bInvertedMode) ? -m_VisualMax : m_VisualMin; }; 
	double GetVisualMax() const { return (m_bInvertedMode) ? -m_VisualMin : m_VisualMax; };
	void GetVisualMinMax(double &VisualMin, double &VisualMax) const 
	{ 
		VisualMin = (m_bInvertedMode) ? -m_VisualMax : m_VisualMin; 
		VisualMax = (m_bInvertedMode) ? -m_VisualMin : m_VisualMax; 
	};

	void SetVisualMinMax(double InnerMin, double InnerMax);

// Operations
public:
	void SetVisualMode(BOOL bVisualMinMax = TRUE);
	BOOL GetVisualMode() const { return m_bVisualMinMax; };

	void SetVerticalMode(BOOL bVerticalMode = TRUE);
	BOOL GetVerticalMode() const { return !m_bHorizontal; };

	void SetInvertedMode(BOOL bInvertedMode = TRUE);
	BOOL GetInvertedMode() const { return m_bInvertedMode; };
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRangeSlider)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRangeSlider();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRangeSlider)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	//}}AFX_MSG
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

	enum RS_DRAWREGION
	{
		RSDR_BACKGROUND,
		RSDR_LEFT,
		RSDR_TOP = RSDR_LEFT,
		RSDR_LEFTBUTTON,
		RSDR_TOPBUTTON = RSDR_LEFTBUTTON,
		RSDR_MIDDLE,
		RSDR_RIGHTBUTTON,
		RSDR_BOTTOMBUTTON = RSDR_RIGHTBUTTON,
		RSDR_RIGHT,
		RSDR_BOTTOM = RSDR_RIGHT,
		RSDR_FOCUS,
	};

	// Tracking the Mouse
	enum BUTTON_ID 
	{
		BUTTON_LEFT,             // Left Arrow is being slided
		BUTTON_RIGHT,            // Right Arrow is being slided
		BUTTON_MIDDLE,           // Middle Area is being slided
	};

	BOOL m_bTracking;           // Follow Mouse Input if T
	BUTTON_ID m_TrackedButton; 

	virtual void DrawRegion(CDC& dc, RS_DRAWREGION nRegion, const CRect& rRegion);
	virtual void DrawButton(CDC& dc, BUTTON_ID nBtn, const CRect& rButton, const CString& sText);
	virtual BOOL IsValidMove(double /*left*/, double /*right*/) const { return TRUE; }

	// Data
	double m_Min, m_Max;           // Outer Edges of the Control
	double m_Left, m_Right;        // Position of Arrows
	double m_VisualMin, m_VisualMax; // Filled Rectangle between Arrows
	double m_Step;

	// Displaying
	CRect RectMiddle() const;
	void Normalize();             // Make intervall [Left, Right] containing in [Min, Max]
	void NormalizeVisualMinMax();  // Make intervall [InnerMin, InnerMax] containing in [Min, Max]
	double NormalizeByStep(double value) const;

	int m_nArrowWidth;          // width of arrow buttons in pixels.

	BOOL m_bHorizontal;         // Horinzontal or Vertical Display of the range.
	void OnPaintHorizontal(CDC &dc);
	void OnPaintVertical(CDC &dc);
	
	BOOL m_bVisualMinMax;        // Display Inner MinMax Range

	BOOL m_bInvertedMode;        // Mirror the control (exchange left and right) 

	CRect m_RectLeft, m_RectRight; // Rectangles of the Left and Right Arrow
	                               // For Hit Testing.
	int m_dx;                      // Size of the Window in x-direction (long axis).
	CPoint m_ClickOffset;          // Drag mouse at point of first click.

	BOOL RegisterWindowClass(); // Make it work as custom control

	BOOL IsPressed(BUTTON_ID nButton) const { return (m_bTracking && (nButton == m_TrackedButton)); }
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RANGESLIDER_H__CEC8D48F_0FDC_415C_A751_3AF12AF8B736__INCLUDED_)
