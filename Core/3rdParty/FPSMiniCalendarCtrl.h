// CFPSMiniCalendarCtrl
// Author:	Matt Gullett
//			gullettm@yahoo.com
// Copyright (c) 2001
//
// This is a user-interface componenet similar to the MS outlook mini
// calendar control.  (The one used in date picker control and the
// appointment (day view)).
//
// You may freely use this source code in personal, freeware, shareware
// or commercial applications provided that 1) my name is recognized in the
// code and if this code represents a substantial portion of the application
// that my name be included in the credits for the application (about box, etc)
//
// Use this code at your own risk.  This code is provided AS-IS.  No warranty
// is granted as to the correctness, usefulness or value of this code.
//
// Special thanks to Keith Rule for the CMemDC class
// http://www.codeproject.com/gdi/flickerfree.asp
//
// *******************************************************************
// TECHNICAL NOTES:
//
// See .cpp file for tech notes
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_FPSMINICALENDARCTRL_H__53B94A35_03B0_4521_8F07_06812F1E8208__INCLUDED_)
#define AFX_FPSMINICALENDARCTRL_H__53B94A35_03B0_4521_8F07_06812F1E8208__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// FPSMiniCalendarCtrl.h : header file
//

// custom window styles
#define FMC_MULTISELECT			0x0040L
#define FMC_NOHIGHLIGHTTODAY	0x0080L		
#define FMC_TODAYBUTTON			0x0100L	
#define FMC_NONEBUTTON			0x0200L	
#define FMC_AUTOSETTINGS		0x0400L
#define FMC_NO3DBORDER			0x0800L
#define FMC_NOSHOWNONMONTHDAYS	0x1000L

// I usually don't use preprocessor macros like this
// but it makes sense here
#define FMC_MAX_SIZE_NONE		CSize(-2,-2)
#define FMC_MAX_SIZE_PARENT		CSize(-1,-1)

// default font settings
#define FMC_DEFAULT_FONT			_T("Tahoma")
#define FMC_DEFAULT_FONT_SIZE		8
#define FMC_DEFAULT_FONT_SIZE_640	7
#define FMC_DEFAULT_FONT_SIZE_800	8
#define FMC_DEFAULT_FONT_SIZE_1024	10
#define FMC_DEFAULT_FONT_SIZE_1280	12
#define FMC_DEFAULT_MIN_FONT_SIZE	5

// used to disable Maximum selection day rules
#define FMC_NO_MAX_SEL_DAYS			-1

// callback function (definition) for the IsSpecialDate
// function
typedef BOOL (CALLBACK* funcSPECIALDATE)(const COleDateTime&, DWORD);

enum FMC_FONT_TYPE
{
	FMC_FONT_HEADER,
	FMC_FONT_DAYSOFWEEK,
	FMC_FONT_SPECIALDAYS,
	FMC_FONT_DAYS,

	FMC_NUM_FONTS
};

/////////////////////////////////////////////////////////////////////////////

// forward declration for list popup
class CFPSMiniCalendarListCtrl;

/////////////////////////////////////////////////////////////////////////////
// CFPSMiniCalendarCtrl window

class CFPSMiniCalendarCtrlFontInfo
{
public:
	CFPSMiniCalendarCtrlFontInfo();
	CFPSMiniCalendarCtrlFontInfo(const CFPSMiniCalendarCtrlFontInfo& font);
	virtual ~CFPSMiniCalendarCtrlFontInfo();

	CFPSMiniCalendarCtrlFontInfo& operator=(const CFPSMiniCalendarCtrlFontInfo& font);
	virtual void CreateFont(CDC* pDC);

	CString m_strFontName;
	int m_iFontSize;
	BOOL m_bBold;
	BOOL m_bItalic;
	BOOL m_bUnderline;
	COLORREF m_crTextColor, m_crBkColor;
	CFont*	m_pFont;
	BOOL m_bFontCreated;
};

/////////////////////////////////////////////////////////////////////////////

class CFPSMiniCalendarCtrlFontHotSpot
{
public:
	CFPSMiniCalendarCtrlFontHotSpot() {};
	~CFPSMiniCalendarCtrlFontHotSpot() {};

	CRect			m_rect;
	COleDateTime	m_dt;
};

/////////////////////////////////////////////////////////////////////////////

class CFPSMiniCalendarCtrlCell
{
public:
	CFPSMiniCalendarCtrlCell();
	~CFPSMiniCalendarCtrlCell();

	CRect m_rectHeader;
	int	  m_iRow;
	int	  m_iCol;

	CRect m_rect;
	
	CFPSMiniCalendarCtrlFontHotSpot* m_parHotSpots;
};

/////////////////////////////////////////////////////////////////////////////

class CFPSMiniCalendarCtrl : public CWnd
{
// Construction
public:
	CFPSMiniCalendarCtrl();

// Attributes
public:
	int GetMinWidth();

// Operations
public:
	BOOL IsDateSelected(COleDateTime& dt);
	void AutoConfigure();
	void AutoSize();

	void ScrollMonth(int nNumMonths);
	void ScrollPage(int nNumPages);
	void ClearSelections();

	CFPSMiniCalendarCtrlFontHotSpot* HitTest(POINT& pt) const;
	CFPSMiniCalendarCtrlCell* HeaderHitTest(POINT& point) const;

	COleDateTime GetDate();
	void SetDate(COleDateTime dt);

	void GetDateSel(COleDateTime& dtBegin, COleDateTime& dtEnd);
	void SetDateSel(COleDateTime dtBegin, COleDateTime dtEnd);

	void SetRowsAndColumns(int iRows, int iCols);
	int GetRows() const {return m_iRows;}
	int GetCols() const {return m_iCols;}

	void SetShowNonMonthDays(BOOL bValue) {m_bShowNonMonthDays = bValue;}
	BOOL GetShowNonMonthDays() {return m_bShowNonMonthDays;}

	void SetDefaultMinFontSize(int iValue);
	int GetDefaultMinFontSize() const {return m_iDefaultMinFontSize;}

	void SetDefaultFont(LPCTSTR lpszValue);
	CString GetDefaultFont() {return m_strDefaultFontName;}

	void SetMonthName(int iMonth, LPCTSTR lpszName);
	CString GetMonthName(int iMonth);

	void SetDayOfWeekName(int iDayOfWeek, LPCTSTR lpszName);
	CString GetDayOfWeekName(int iDayOfWeek);

	void SetFirstDayOfWeek(int iDayOfWeek);
	int GetFirstDayOfWeek() const;

	void SetCurrentMonthAndYear(int iMonth, int iYear);
	int GetCurrentMonth() const {return m_iCurrentMonth;}
	int GetCurrentYear() const {return m_iCurrentYear;}
	void SetCurrentMonthAndYear(const COleDateTime& date);
	COleDateTime GetCurrentMonthAndYear() const { return COleDateTime(m_iCurrentYear, m_iCurrentMonth, 1, 0, 0, 0); }

	void SetMultiSelect(BOOL bValue) {m_bMultiSel = bValue;}
	BOOL IsMultiSelect() {return m_bMultiSel;}

	void SetBackColor(COLORREF cColor) {m_cBackColor = cColor;}
	COLORREF GetBackColor() {return m_cBackColor;}

	void SetHighlightToday(BOOL bValue, COLORREF crBorder) {m_bHighlightToday = bValue; m_crTodayBorder = crBorder; }	
	BOOL GetHighlightToday() {return m_bHighlightToday;}
	
	void SetShowTodayButton(BOOL bValue) {m_bShowTodayButton = bValue;}
	BOOL GetShowTodayButton() {return m_bShowTodayButton;}

	void SetShowNoneButton(BOOL bValue) {m_bShowNoneButton = bValue;}
	BOOL GetShowNoneButton() {return m_bShowNoneButton;}

	void SetNonMonthDayColor(COLORREF cColor) {m_cNonMonthDayColor = cColor;}
	COLORREF GetNonMonthDayColor() const {return m_cNonMonthDayColor;}

	void SetShow3DBorder(BOOL bValue) {m_bShow3dBorder = bValue;}
	BOOL GetShow3DBorder() {return m_bShow3dBorder;}

	void SetMaxSize(SIZE size);
	CSize GetMaxSize() {return m_szMaxSize;}

	void SetUseAutoSettings(BOOL bValue) {m_bUseAutoSettings = bValue;}
	BOOL GetUseAutoSettings() {return m_bUseAutoSettings;}

	void SetMaxSelDays(int iValue) {m_iMaxSelDays = iValue;}
	int GetMaxSelDays() const {return m_iMaxSelDays;}

	void SetSpecialDaysCallback(funcSPECIALDATE pValue, DWORD dwUserData);
	funcSPECIALDATE GetSpecialDaysCallback() {return m_pfuncCallback;}

	void SetFontInfo(FMC_FONT_TYPE nFont, LPCTSTR lpszFont, int iSize, BOOL bBold = FALSE, BOOL bItalic = FALSE, BOOL bUnderline = FALSE, COLORREF crText = GetSysColor(COLOR_WINDOWTEXT), COLORREF crBkgnd = GetSysColor(COLOR_WINDOW));
	void SetFontInfo(FMC_FONT_TYPE nFont, const CFPSMiniCalendarCtrlFontInfo& font);
	void GetFontInfo(FMC_FONT_TYPE nFont, CFPSMiniCalendarCtrlFontInfo& font) const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFPSMiniCalendarCtrl)
	public:
	BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:

	virtual ~CFPSMiniCalendarCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFPSMiniCalendarCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDevModeChange(LPTSTR lpDeviceName);
	afx_msg void OnFontChange();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnTimeChange();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnCaptureChanged(CWnd* pWnd);

	DECLARE_MESSAGE_MAP()

	int								m_iCurrentMonth;
	int								m_iCurrentYear;
	CFPSMiniCalendarCtrlFontInfo	m_FontInfo[FMC_NUM_FONTS];
	COLORREF						m_cBackColor;
	BOOL							m_bHighlightToday;
	BOOL							m_bShowTodayButton;
	BOOL							m_bShowNoneButton;
	COLORREF						m_cNonMonthDayColor, m_crTodayBorder;
	CString							m_arMonthNames[12];
	CString							m_arDaysOfWeekNames[7];
	int								m_iFirstDayOfWeek;
	int								m_iRows;
	int								m_iCols;
	BOOL							m_bMultiSel;
	BOOL							m_bShow3dBorder;
	BOOL							m_bUseAutoSettings;
	CSize							m_szMaxSize;
	CString							m_strDefaultFontName;
	int								m_iDefaultMinFontSize;
	int								m_iMaxSelDays;
	BOOL							m_bShowNonMonthDays;
	funcSPECIALDATE					m_pfuncCallback;
	DWORD							m_dwCallbackData;

	BOOL							m_bTracking;
	COleDateTime					m_dtTrackingStart;
	BOOL							m_bScrollTracking;
	BOOL							m_bTodayTracking;
	BOOL							m_bTodayDown;
	BOOL							m_bNoneTracking;
	BOOL							m_bNoneDown;
	BOOL							m_bHeaderTracking;
	CFPSMiniCalendarListCtrl*		m_pHeaderList;
	UINT							m_iHeaderTimerID;
	CFPSMiniCalendarCtrlCell*		m_pHeaderCell;

	CRect							m_rectScrollLeft;
	CRect							m_rectScrollRight;
	CRect							m_rectToday;
	CRect							m_rectNone;

protected:
	void AllocateCells();
	void SetCellHeaderPosition(int iMonthRow, int iMonthCol, RECT rect);
	void SetCellPosition(int iMonthRow, int iMonthCol, RECT rect);
	void SetHotSpot(int iMonthRow, int iMonthCol, int iDayCounter, COleDateTime& dt, RECT rect);
	void ClearHotSpots();
	int DrawTodayButton(CDC& dc, int iY);
	BOOL IsToday(const COleDateTime& dt) const;
	CString CStr(long lValue);
	void CreateFontObjects();

	virtual void Draw(CDC& dc, const CRect& rDraw);
	virtual int DrawHeader(CDC& dc, int iY, int iLeftX, int iRow, int iCol, int iMonth, int iYear);
	virtual int DrawDays(CDC& dc, int iY, int iLeftX, int iRow, int iCol, int iMonth, int iYear);
	virtual int DrawDaysOfWeek(CDC& dc, int iY, int iLeftX, int iRow, int iCol);
	virtual void DrawCellBkgnd(CDC& dc, const CRect& rCell, const COleDateTime& dt, BOOL bSelected, BOOL bSpecial, BOOL bActiveMonth);

	virtual int ComputeTodayNoneHeight();
	virtual CSize ComputeTotalSize();
	virtual CSize ComputeSize();
	virtual CSize Compute3DBorderSize();
	void FireNoneButton();
	void FireNotifyDblClick();
	void FireNotifyClick();
	void FireNotifyHScroll(int nDirection);
	void FireTodayButton();

	virtual BOOL IsSpecialDate(const COleDateTime& dt) const;
	virtual COLORREF GetCellBkgndColor(const COleDateTime& dt, BOOL bSelected, BOOL bSpecial, BOOL bActiveMonth) const;
	virtual COLORREF GetCellTextColor(const COleDateTime& dt, BOOL bSelected, BOOL bSpecial, BOOL bActiveMonth) const;

	// computed values of importance
	BOOL							m_bFontsCreated;
	BOOL							m_bSizeComputed;
	int								m_iHeaderHeight;
	int								m_iIndividualDayWidth;
	int								m_iDaysOfWeekHeight;
	int								m_iDaysHeight;
	CSize							m_szMonthSize;

	// cells
	COleDateTime					m_dtSelBegin;
	COleDateTime					m_dtSelEnd;
	CFPSMiniCalendarCtrlCell*		m_parCells;
	int								m_iCells;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FPSMINICALENDARCTRL_H__53B94A35_03B0_4521_8F07_06812F1E8208__INCLUDED_)
