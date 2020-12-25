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
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "memdc.h"
#include "FPSMiniCalendarCtrl.h"
#include "FPSMiniCalendarListCtrl.h"

#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FMC_HEADER_TIMER_ID				1001
#define FMC_HEADER_TIMER_INTERVAL		30
#define FMC_MINICALENDAR_HEIGHT			133

const UINT DEFTEXTFLAGS = (DT_VCENTER | DT_CENTER | DT_SINGLELINE/* | DT_RTLREADING*/);
const int WEEKNUMBERPADDING = 8;

/////////////////////////////////////////////////////////////////////////////
// CFPSMiniCalendarCtrl

CFPSMiniCalendarCtrlCell::CFPSMiniCalendarCtrlCell() : m_iRow(-1), m_iCol(-1)
{
	m_parHotSpots = new CFPSMiniCalendarCtrlFontHotSpot[42];

	if (!m_parHotSpots)
		throw (new CMemoryException());
}

CFPSMiniCalendarCtrlCell::~CFPSMiniCalendarCtrlCell()
{
	if (m_parHotSpots)
		delete[] m_parHotSpots;
}

/////////////////////////////////////////////////////////////////////////////

CFPSMiniCalendarCtrlFontInfo::CFPSMiniCalendarCtrlFontInfo() 
	:
	m_iFontSize(0),
	m_bBold(FALSE),
	m_bItalic(FALSE),
	m_bUnderline(FALSE),
	m_crTextColor(CLR_NONE), 
	m_crBkColor(CLR_NONE),
	m_pFont(NULL),
	m_bFontCreated(FALSE)
{
}

CFPSMiniCalendarCtrlFontInfo::~CFPSMiniCalendarCtrlFontInfo() 
{
	if (m_bFontCreated) 
		delete m_pFont;
}

CFPSMiniCalendarCtrlFontInfo::CFPSMiniCalendarCtrlFontInfo(const CFPSMiniCalendarCtrlFontInfo& font)
{
	*this = font;
}

CFPSMiniCalendarCtrlFontInfo& CFPSMiniCalendarCtrlFontInfo::operator=(const CFPSMiniCalendarCtrlFontInfo& font)
{
	m_strFontName = font.m_strFontName;
	m_iFontSize = font.m_iFontSize;
	m_bBold = font.m_bBold;
	m_bItalic = font.m_bItalic;
	m_bUnderline = font.m_bUnderline;
	m_crTextColor = font.m_crTextColor;
	m_crBkColor = font.m_crBkColor;
	m_pFont = font.m_pFont;

	m_bFontCreated = FALSE; // always

	return *this;
}

// create the CFont GDI objects
void CFPSMiniCalendarCtrlFontInfo::CreateFont(CDC* pDC)
{
	ASSERT(pDC);

	if (pDC)
	{
		if (m_bFontCreated)
			delete m_pFont;

		LOGFONT FontInfo;
		memset(&FontInfo, 0, sizeof(LOGFONT));
		
		m_pFont = new CFont;

		ASSERT(m_pFont);
		if (!m_pFont)
			throw (new CMemoryException());
		
		m_pFont->CreatePointFont(m_iFontSize * 10, m_strFontName);
		m_pFont->GetLogFont(&FontInfo);
		
		FontInfo.lfHeight = -MulDiv(m_iFontSize, GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSY), 72);
		FontInfo.lfWeight = 100;
		FontInfo.lfQuality = PROOF_QUALITY;
		
		if (m_bBold)
			FontInfo.lfWeight = 700;

		if (m_bItalic)
			FontInfo.lfItalic = TRUE;

		if (m_bUnderline)
			FontInfo.lfUnderline = TRUE;
		
		delete m_pFont;
		
		m_pFont = new CFont;
		ASSERT(m_pFont);
		
		if (!m_pFont)
			throw (new CMemoryException());
		
		m_pFont->CreateFontIndirect(&FontInfo);
		m_bFontCreated = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////

CFPSMiniCalendarCtrl::CFPSMiniCalendarCtrl()
{
	m_bFontsCreated = FALSE;
	m_bSizeComputed = FALSE;
	m_bNoneTracking = FALSE;
	m_bNoneDown = FALSE;
	m_bTodayDown = FALSE;
	m_bTodayTracking = FALSE;
	m_bScrollTracking = FALSE;
	m_bTracking = FALSE;
	m_bHeaderTracking = FALSE;
	m_pHeaderList = NULL;
	m_iHeaderTimerID = 0;
	m_pHeaderCell = NULL;
	m_iFirstDayOfWeek = 1;

	m_iCells = 0;
	m_parCells = NULL;
	m_bShow3dBorder = TRUE;

	// set default startup options
	SetDefaultFont(FMC_DEFAULT_FONT);
	SetDefaultMinFontSize(FMC_DEFAULT_MIN_FONT_SIZE);
	SetCurrentMonthAndYear(COleDateTime::GetCurrentTime().GetMonth(), COleDateTime::GetCurrentTime().GetYear());
	SetBackColor(::GetSysColor(COLOR_WINDOW));
	SetHighlightToday(TRUE, 0);
	SetShowTodayButton(FALSE);
	SetShowNoneButton(FALSE);
	SetNonMonthDayColor(::GetSysColor(COLOR_GRAYTEXT));
	SetRowsAndColumns(1, 1);
	SetMultiSelect(FALSE);
	SetUseAutoSettings(FALSE);
	SetMaxSize(FMC_MAX_SIZE_PARENT);
	SetMaxSelDays(FMC_NO_MAX_SEL_DAYS);
	SetShowNonMonthDays(TRUE);
	SetSpecialDaysCallback(NULL, 0);

	SetFontInfo(FMC_FONT_HEADER, m_strDefaultFontName, FMC_DEFAULT_FONT_SIZE, TRUE);
	SetFontInfo(FMC_FONT_DAYSOFWEEK, m_strDefaultFontName, FMC_DEFAULT_FONT_SIZE);
	SetFontInfo(FMC_FONT_DAYS, m_strDefaultFontName, FMC_DEFAULT_FONT_SIZE);
	SetFontInfo(FMC_FONT_SPECIALDAYS, m_strDefaultFontName, FMC_DEFAULT_FONT_SIZE, TRUE);

	// set month names
	setlocale(LC_TIME, "");		// should I be doing this here AND am I doing it right???
	COleDateTime dtTemp;
	dtTemp.SetDate(2000, 1, 1); SetMonthName(1, dtTemp.Format(_T("%B")));
	dtTemp.SetDate(2000, 2, 1); SetMonthName(2, dtTemp.Format(_T("%B")));
	dtTemp.SetDate(2000, 3, 1); SetMonthName(3, dtTemp.Format(_T("%B")));
	dtTemp.SetDate(2000, 4, 1); SetMonthName(4, dtTemp.Format(_T("%B")));
	dtTemp.SetDate(2000, 5, 1); SetMonthName(5, dtTemp.Format(_T("%B")));
	dtTemp.SetDate(2000, 6, 1); SetMonthName(6, dtTemp.Format(_T("%B")));
	dtTemp.SetDate(2000, 7, 1); SetMonthName(7, dtTemp.Format(_T("%B")));
	dtTemp.SetDate(2000, 8, 1); SetMonthName(8, dtTemp.Format(_T("%B")));
	dtTemp.SetDate(2000, 9, 1); SetMonthName(9, dtTemp.Format(_T("%B")));
	dtTemp.SetDate(2000, 10, 1); SetMonthName(10, dtTemp.Format(_T("%B")));
	dtTemp.SetDate(2000, 11, 1); SetMonthName(11, dtTemp.Format(_T("%B")));
	dtTemp.SetDate(2000, 12, 1); SetMonthName(12, dtTemp.Format(_T("%B")));

	// we need to determine the first-day-of-week to use in the
	// calendar.  We use the GetLocaleInfo function to do this
	TCHAR szDayOfWeek[20];
	int iFirstDayOfWeek = 0;
	memset(szDayOfWeek, 0, 20);
	VERIFY(::GetLocaleInfo(LOCALE_USER_DEFAULT,
							LOCALE_IFIRSTDAYOFWEEK,
							szDayOfWeek,
							20) != 0);

	// the return value of GetLocaleInfo is 0=Monday, 6=sunday
	// therefore we need to convert it to 1=Sunday, 7=Saturday 
	//    which is the standard for COleDateTime
	iFirstDayOfWeek = _ttoi(szDayOfWeek) + 2; // move from zero base to 1 base + 1 to accomodate subday being 6
	if (iFirstDayOfWeek > 7)
	{
		ASSERT(iFirstDayOfWeek == 8);		// we should never exceed 8
		iFirstDayOfWeek = 1;
	}
	ASSERT(iFirstDayOfWeek >= 1);
	ASSERT(iFirstDayOfWeek <= 7);

	// set days of week names
	if (iFirstDayOfWeek >= 1 && iFirstDayOfWeek <= 7)
		SetFirstDayOfWeek(iFirstDayOfWeek);
	else
		SetFirstDayOfWeek(1);
}

CFPSMiniCalendarCtrl::~CFPSMiniCalendarCtrl()
{
	ClearSelections();
	ClearHotSpots();

	if (m_parCells)
		delete[] m_parCells;

	if (m_pHeaderList)
		delete m_pHeaderList;

	if (m_iHeaderTimerID != 0)
		KillTimer(m_iHeaderTimerID);
}


BEGIN_MESSAGE_MAP(CFPSMiniCalendarCtrl, CWnd)
	//{{AFX_MSG_MAP(CFPSMiniCalendarCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_DEVMODECHANGE()
	ON_WM_FONTCHANGE()
	ON_WM_PALETTECHANGED()
	ON_WM_SETTINGCHANGE()
	ON_WM_TIMECHANGE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_CAPTURECHANGED()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFPSMiniCalendarCtrl message handlers
void CFPSMiniCalendarCtrl::SetCurrentMonthAndYear(int iMonth, int iYear)
{
	// From MSDN: The COleDateTime class handles dates from 1 January 100 to 31 December 9999.
	ASSERT(iYear >= 100);
	ASSERT(iYear <= 9999);
	ASSERT(iMonth >= 1);
	ASSERT(iMonth <= 12);

	if (iMonth >= 1 && iMonth <= 12)
		m_iCurrentMonth = iMonth;

	if (iYear >= 100 && iYear <= 9999)
		m_iCurrentYear = iYear;

	if (GetSafeHwnd())
		Invalidate(FALSE);
}

void CFPSMiniCalendarCtrl::SetCurrentMonthAndYear(const COleDateTime& date)
{
	SetCurrentMonthAndYear(date.GetMonth(), date.GetYear());
}

void CFPSMiniCalendarCtrl::SetFontInfo(FMC_FONT_TYPE nFont, LPCTSTR lpszFont, int iSize, BOOL bBold, BOOL bItalic, BOOL bUnderline, COLORREF crText, COLORREF crBkgnd)
{
	ASSERT(nFont >= 0 && nFont < FMC_NUM_FONTS);

	ASSERT(iSize > 0);
	ASSERT(iSize <= 72);
	ASSERT(lpszFont);
	ASSERT(AfxIsValidString(lpszFont));

	if (nFont >= 0 && nFont < FMC_NUM_FONTS && iSize > 0 && iSize <= 72 && lpszFont && AfxIsValidString(lpszFont))
	{
		CFPSMiniCalendarCtrlFontInfo& font = m_FontInfo[nFont];

		font.m_bBold = bBold;
		font.m_bItalic = bItalic;
		font.m_bUnderline = bUnderline;
		font.m_iFontSize = iSize;
		font.m_strFontName = lpszFont;
		font.m_crTextColor = crText;
		font.m_crBkColor = crBkgnd;

		// make sure font object gets recreated
		m_bFontsCreated = FALSE;
	}
}

void CFPSMiniCalendarCtrl::SetFontInfo(FMC_FONT_TYPE nFont, const CFPSMiniCalendarCtrlFontInfo& fi)
{
	ASSERT(nFont >= 0 && nFont < FMC_NUM_FONTS);

	if (nFont >= 0 && nFont < FMC_NUM_FONTS)
	{
		CFPSMiniCalendarCtrlFontInfo& font = m_FontInfo[nFont];

		// cleanup existing font first
		delete font.m_pFont;

		// copy attributes except font
		font = fi;
		font.m_pFont = NULL;

		// make sure font object gets recreated
		m_bFontsCreated = FALSE;
	}
}

void CFPSMiniCalendarCtrl::GetFontInfo(FMC_FONT_TYPE nFont, CFPSMiniCalendarCtrlFontInfo& font) const
{
	ASSERT(nFont >= 0 && nFont < FMC_NUM_FONTS);

	if (nFont >= 0 && nFont < FMC_NUM_FONTS)
		font = m_FontInfo[nFont];
}

// utility function for converting long to CString 
CString CFPSMiniCalendarCtrl::CStr(long lValue)
{
	CString sVal;
	sVal.Format(_T("%ld"), lValue);

	return sVal;
}

void CFPSMiniCalendarCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (!m_bSizeComputed)
		ComputeSize();

	SetRowsAndColumns(max(1, (cy / m_szMonthSize.cy)), 1);
}

// computes the size of the special 3d border this control 
// draws (when option is set).
// I draw this myself because I could not find a standard
// border-style which closely mimicked the MS Outlook controls
// look and feel
CSize CFPSMiniCalendarCtrl::Compute3DBorderSize()
{
	// MENTAL NOTE: Why am I hard-coding this?
	CSize szReturn(6,6);

	return szReturn;
}

// determine height of today/none buttons
int CFPSMiniCalendarCtrl::ComputeTodayNoneHeight()
{
	int iReturn = 0;
	CFont* pOldFont = NULL;
	CClientDC* pDC = NULL;

	if (!m_bFontsCreated)
		CreateFontObjects();

	// allocate a DC to use when testing sizes, etc
	if (::IsWindow(GetSafeHwnd())) 
		pDC = new CClientDC(this);
	else
		pDC = new CClientDC(AfxGetMainWnd());
	ASSERT(pDC);

	if (!pDC)
		throw (new CMemoryException());

	// get current font and save for later restoration
	pOldFont = pDC->GetCurrentFont();

	// now, compute heigt of today button
	if (m_bShowTodayButton || m_bShowNoneButton)
	{
		pDC->SelectObject(m_FontInfo[FMC_FONT_DAYSOFWEEK].m_pFont);

		iReturn = pDC->GetTextExtent("Today").cy + 8 + 7;
	}

	// cleanup DC
	pDC->SelectObject(pOldFont);

	// destroy DC
	delete pDC;

	return iReturn;
}


// determine size of a given cell based on actual font settings 
CSize CFPSMiniCalendarCtrl::ComputeSize()
{
	CSize szReturn(0, 0);
	int iXDaysSpaceFromBorder = 15;
	int iHeaderSpaceForBorder = 15;
	int iHeaderYSpaceForBorder = 3;
	int iDaysXSpacing = 5;
	int iDaysYSpacing = 2;
	int iWidthByDaysIndividual = 0;
	int iWidthByDays = 0;
	int iWidthByDaysOfWeekIndividual = 0;
	int iWidthByDaysOfWeek = 0;
	int iWidthByHeader = 0;
	int iHeaderHeight = 0;
	int iDaysOfWeekHeight = 0;
	int iDaysHeight = 0;
	int iTodayHeight = 0;
	int iTotalWidth = 0;
	int iTotalHeight = 0;
	CFont* pOldFont = NULL;
	CClientDC* pDC = NULL;

	if (!m_bFontsCreated)
		CreateFontObjects();

	// allocate a DC to use when testing sizes, etc
	if (::IsWindow(GetSafeHwnd())) 
		pDC = new CClientDC(this);
	else
		pDC = new CClientDC(AfxGetMainWnd());
	ASSERT(pDC);

	if (!pDC)
		throw (new CMemoryException());

	// get current font and save for later restoration
	pOldFont = pDC->GetCurrentFont();

	// first, use days to determine width
	// NOTE: typically, most fonts use the same pixel sizes for all numbers,
	// but this is not mandatory and many "fancy" fonts change this.  To deal
	// with this, I am calculating the width of all possible dates the control
	// will display
	pDC->SelectObject(m_FontInfo[FMC_FONT_DAYS].m_pFont);

	int iX;

	for (iX = 1; iX <= 31; iX++)
	{
		CString strTemp = "00";
		strTemp += CStr(iX);
		strTemp = strTemp.Right(2);

		CSize szTemp = pDC->GetTextExtent(strTemp);
		if (szTemp.cx > iWidthByDays)
			iWidthByDays = szTemp.cx;
		if (szTemp.cy > iDaysHeight)
			iDaysHeight = szTemp.cy;
	}

	// make sure we also try it with the special days font
	pDC->SelectObject(m_FontInfo[FMC_FONT_SPECIALDAYS].m_pFont);
	for (iX = 1; iX <= 31; iX++)
	{
		CString strTemp = "00";
		strTemp += CStr(iX);
		strTemp = strTemp.Right(2);

		CSize szTemp = pDC->GetTextExtent(strTemp);
		if (szTemp.cx > iWidthByDays)
			iWidthByDays = szTemp.cx;
		if (szTemp.cy > iDaysHeight)
			iDaysHeight = szTemp.cy;
	}

	// finish computation
	iWidthByDaysIndividual = iWidthByDays;
	m_iDaysHeight = iDaysHeight;
	m_iIndividualDayWidth = iWidthByDaysIndividual;

	iWidthByDays = (iWidthByDays * 7) + (iDaysXSpacing*6) + (iXDaysSpaceFromBorder*2);	
	iDaysHeight = (iDaysHeight * 6) + (iDaysYSpacing*6);

	// next use days of week to determine width and height
	// here again, we test each variant 
	pDC->SelectObject(m_FontInfo[FMC_FONT_DAYSOFWEEK].m_pFont);

	iWidthByDaysOfWeek = pDC->GetTextExtent("S").cx;
	if (pDC->GetTextExtent("M").cx > iWidthByDaysOfWeek)
		iWidthByDaysOfWeek = pDC->GetTextExtent("M").cx;
	if (pDC->GetTextExtent("T").cx > iWidthByDaysOfWeek)
		iWidthByDaysOfWeek = pDC->GetTextExtent("T").cx;
	if (pDC->GetTextExtent("W").cx > iWidthByDaysOfWeek)
		iWidthByDaysOfWeek = pDC->GetTextExtent("W").cx;
	if (pDC->GetTextExtent("F").cx > iWidthByDaysOfWeek)
		iWidthByDaysOfWeek = pDC->GetTextExtent("F").cx;

	iDaysOfWeekHeight = pDC->GetTextExtent("S").cy;
	if (pDC->GetTextExtent("M").cy > iDaysOfWeekHeight)
		iDaysOfWeekHeight = pDC->GetTextExtent("M").cy;
	if (pDC->GetTextExtent("T").cy > iDaysOfWeekHeight)
		iDaysOfWeekHeight = pDC->GetTextExtent("T").cy;
	if (pDC->GetTextExtent("W").cy > iDaysOfWeekHeight)
		iDaysOfWeekHeight = pDC->GetTextExtent("W").cy;
	if (pDC->GetTextExtent("F").cy > iDaysOfWeekHeight)
		iDaysOfWeekHeight = pDC->GetTextExtent("F").cy;

	// finish computation
	iWidthByDaysOfWeekIndividual = iWidthByDaysOfWeek;
	iWidthByDaysOfWeek = (iWidthByDaysOfWeek * 7) + (iDaysXSpacing*6) + (iXDaysSpaceFromBorder*2);

	if (iWidthByDaysOfWeekIndividual > m_iIndividualDayWidth)
		m_iIndividualDayWidth = iWidthByDaysOfWeekIndividual;

	m_iDaysOfWeekHeight = iDaysOfWeekHeight;

	// next compute width and height of header (month name and year)
	// again, because of font variations we will use a 20 year window and
	// attempt the calculation with every month name
	pDC->SelectObject(m_FontInfo[FMC_FONT_HEADER].m_pFont);

	for (int iYear = m_iCurrentYear-10; iYear <= m_iCurrentYear+10; iYear++)
	{
		for (int iMonth = 1; iMonth <= 12; iMonth++)
		{
			CString strTest = GetMonthName(iMonth);
			strTest += " ";
			strTest += CStr(iYear);

			if (pDC->GetTextExtent(strTest).cx > iWidthByHeader)
				iWidthByHeader = pDC->GetTextExtent(strTest).cx;

			if (pDC->GetTextExtent(strTest).cy > iHeaderHeight)
				iHeaderHeight = pDC->GetTextExtent(strTest).cy;
		}
	}

	// finish computation
	iWidthByHeader = iWidthByHeader + (iHeaderSpaceForBorder*2);
	iHeaderHeight = iHeaderHeight + (iHeaderYSpaceForBorder*2);
	m_iHeaderHeight = iHeaderHeight;

	// cleanup DC
	pDC->SelectObject(pOldFont);

	// destroy DC
	delete pDC;

	// NOW, adjust return size if needed
	// start with widths (these are easy at this point)
	iTotalWidth = iWidthByHeader;
	if (iWidthByDaysOfWeek > iTotalWidth)
		iTotalWidth = iWidthByDaysOfWeek;
	if (iWidthByDays > iTotalWidth)
		iTotalWidth = iWidthByDays;
	if (iTotalWidth > szReturn.cx)
		szReturn.cx = iTotalWidth;

	// adjust heights
	iTotalHeight = iHeaderHeight + (iDaysOfWeekHeight + 3) + 1 + iDaysHeight + 3 + 1 + 4 + iTodayHeight;
	if (iTotalHeight > szReturn.cy)
		szReturn.cy = iTotalHeight;

	m_szMonthSize = szReturn;
	m_bSizeComputed = TRUE;

	return szReturn;
}

int CFPSMiniCalendarCtrl::GetMinWidth()
{
	if (!m_bSizeComputed)
		ComputeSize();

	return m_szMonthSize.cx;
}

// determine total size of control (all rows/cols + border + none/today)
CSize CFPSMiniCalendarCtrl::ComputeTotalSize()
{
	CSize size;

	size = ComputeSize();
	size.cx *= m_iCols;
	size.cy *= m_iRows;
	size.cy += ComputeTodayNoneHeight();

	if (m_bShow3dBorder)
		size += Compute3DBorderSize();

	return size;
}

// create the CFont GDI objects
void CFPSMiniCalendarCtrl::CreateFontObjects()
{
	CClientDC* pDC = NULL;

	// allocate a DC to use when testing sizes, etc
	if (::IsWindow(GetSafeHwnd())) 
		pDC = new CClientDC(this);
	else
		pDC = new CClientDC(AfxGetMainWnd());

	ASSERT(pDC);
	if (!pDC)
		throw (new CMemoryException());

	for (int i = 0; i < FMC_NUM_FONTS; i++)
		m_FontInfo[i].CreateFont(pDC);

	delete pDC;

	m_bFontsCreated = TRUE;
}

void CFPSMiniCalendarCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect ClientRect;
	CMemDC dcDraw(&dc);

	if (!m_bSizeComputed)
		ComputeSize();

	GetClientRect(ClientRect);
	dcDraw.FillSolidRect(0, 0, ClientRect.Width(), ClientRect.Height(), m_cBackColor);

	if (m_bShow3dBorder)
	{
		dcDraw.Draw3dRect(0, 0, ClientRect.Width(), ClientRect.Height(), ::GetSysColor(COLOR_BTNFACE), ::GetSysColor(COLOR_BTNTEXT));
		dcDraw.Draw3dRect(1, 1, ClientRect.Width()-2, ClientRect.Height()-2, ::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DSHADOW));
		dcDraw.Draw3dRect(2, 2, ClientRect.Width()-4, ClientRect.Height()-4, ::GetSysColor(COLOR_BTNFACE), ::GetSysColor(COLOR_BTNFACE));
		dcDraw.Draw3dRect(3, 3, ClientRect.Width()-6, ClientRect.Height()-6, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_WINDOW));

		ClientRect.DeflateRect(4, 4);
	}

	Draw(dcDraw, ClientRect);
}

void CFPSMiniCalendarCtrl::Draw(CDC& dc, const CRect& rDraw)
{
	int iY = rDraw.top;
	int iMonth = m_iCurrentMonth;
	int iYear = m_iCurrentYear;

	// draw each row/column individually
	for (int iRow = 1; iRow <= m_iRows; iRow++)
	{		
		int iCurrentX = rDraw.left;

		for (int iCol = 1; iCol <= m_iCols; iCol++)
		{
			int iCurrentY = iY;

			iCurrentY += DrawHeader(dc, iCurrentY, iCurrentX, iRow, iCol, iMonth, iYear);
			iCurrentY += DrawDaysOfWeek(dc, iCurrentY, iCurrentX, iRow, iCol);
			iCurrentY += DrawDays(dc, iCurrentY, iCurrentX, iRow, iCol, iMonth, iYear);

			iCurrentX += m_szMonthSize.cx;

			iMonth++;
			if (iMonth > 12)
			{
				iMonth = 1;
				iYear++;
			}
		}

		iY += m_szMonthSize.cy;
	}

	DrawTodayButton(dc, iY);
}

BOOL CFPSMiniCalendarCtrl::OnEraseBkgnd(CDC*) 
{
	return FALSE;
}

// draw the month/year header (ie. January 2000)
int CFPSMiniCalendarCtrl::DrawHeader(CDC &dc, int iY, int iLeftX, int iMonthRow, int iMonthCol, int iMonth, int iYear)
{
	CRect ClientRect;
	CString strText = GetMonthName(iMonth);
	strText += " ";
	strText += CStr(iYear);

	if (!m_bFontsCreated)
		CreateFontObjects();

	GetClientRect(ClientRect);

	dc.FillSolidRect(iLeftX, iY, m_szMonthSize.cx, m_iHeaderHeight, ::GetSysColor(COLOR_BTNFACE));
	dc.Draw3dRect(iLeftX, iY, m_szMonthSize.cx, m_iHeaderHeight, ::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DSHADOW));

	CFont* pOldFont = dc.SelectObject(m_FontInfo[FMC_FONT_HEADER].m_pFont);
	dc.SetBkColor(::GetSysColor(COLOR_BTNFACE));
	dc.SetTextColor(m_FontInfo[FMC_FONT_HEADER].m_crTextColor);
	dc.SetBkMode(TRANSPARENT);

	dc.DrawText(strText, CRect(iLeftX+1, iY+1, iLeftX+m_szMonthSize.cx - 10, iY+m_iHeaderHeight-2), DEFTEXTFLAGS);

	dc.SelectObject(pOldFont);

	SetCellHeaderPosition(iMonthRow, iMonthCol, CRect(iLeftX+10, iY+1, iLeftX+m_szMonthSize.cx - 20, iY+m_iHeaderHeight-2));

	// deal with left scroll bar
	if (iMonthRow == 1 && iMonthCol == 1)
	{
		int iMiddle = iY + (m_iHeaderHeight / 2);
		int iTop = iMiddle - (m_iHeaderHeight / 4);
		int iBottom = iMiddle + (m_iHeaderHeight / 4);

		int iX1 = iLeftX + 6;
		int iX2 = iX1 + ((m_iHeaderHeight / 4));

		for (int iLineY = iTop; iLineY <= iBottom; iLineY++)
		{
			dc.MoveTo(iX1, iMiddle);
			dc.LineTo(iX2, iLineY);
		}

		m_rectScrollLeft.SetRect(iX1-3, iTop-3, iX2+3, iBottom+3);
	}

	// deal with right scroll bar
	if (iMonthRow == 1 && iMonthCol == m_iCols)
	{
		int iMiddle = iY + (m_iHeaderHeight / 2);
		int iTop = iMiddle - (m_iHeaderHeight / 4);
		int iBottom = iMiddle + (m_iHeaderHeight / 4);

		int iX1 = iLeftX + m_szMonthSize.cx - 10;
		int iX2 = iX1 - (m_iHeaderHeight / 4);

		//int iLeftX = ClientRect.Width() - 10;
		//int iRightX = iLeftX - (m_iHeaderHeight / 4);

		for (int iLineY = iTop; iLineY <= iBottom; iLineY++)
		{
			dc.MoveTo(iX1, iMiddle);
			dc.LineTo(iX2, iLineY);
		}

		m_rectScrollRight.SetRect(iX2-3, iTop-3, iX1+3, iBottom+3);
	}

	return m_iHeaderHeight;
}

// draw days of week header (ie. S M T W T F S)
int CFPSMiniCalendarCtrl::DrawDaysOfWeek(CDC &dc, int iY, int iLeftX, int, int)
{
	CRect ClientRect;
	int iStartX = 0;
	int iEndX = 0;
	int iX = 0;

	if (!m_bFontsCreated)
		CreateFontObjects();

	GetClientRect(ClientRect);

	// calculate starting X position
	iStartX = ((iLeftX + (m_szMonthSize.cx / 2))) - (((m_iIndividualDayWidth*7) + 30) / 2);
	iEndX = ((iLeftX + (m_szMonthSize.cx / 2))) + (((m_iIndividualDayWidth*7) + 30) / 2);

	iX = iStartX;

	CFont* pOldFont = dc.SelectObject(m_FontInfo[FMC_FONT_DAYSOFWEEK].m_pFont);
	dc.SetBkColor(::GetSysColor(COLOR_BTNFACE));
	dc.SetTextColor(m_FontInfo[FMC_FONT_DAYSOFWEEK].m_crTextColor);
	dc.SetBkMode(TRANSPARENT);

	CRect rect(iX, iY+1, iX+m_iIndividualDayWidth,iY+2+m_iDaysOfWeekHeight);

	for (int i = m_iFirstDayOfWeek; i < (m_iFirstDayOfWeek + 7); i++)
	{
		int nDOW = (((i - 1) % 7) + 1);
		dc.DrawText(GetDayOfWeekName(nDOW), 1, rect, DEFTEXTFLAGS);
		rect.OffsetRect(m_iIndividualDayWidth + 5, 0);
	}

	dc.SelectObject(pOldFont);

	// draw the divider line
	CPen* pPen = new CPen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
	CPen* pOldPen = dc.SelectObject(pPen);

	dc.MoveTo(iStartX-4, iY+3+m_iDaysOfWeekHeight);
	dc.LineTo(iEndX+4, iY+3+m_iDaysOfWeekHeight);

	dc.SelectObject(pOldPen);
	delete pPen;

	return m_iDaysOfWeekHeight+4;
}

// draw days of month
int CFPSMiniCalendarCtrl::DrawDays(CDC &dc, int iY, int iLeftX, int iMonthRow, int iMonthCol, int iMonth, int iYear)
{
	CRect ClientRect;
	int iReturn = 0;
	int iStartY = iY;
	int iStartX = 0;
	int iEndX = 0;
//	int iX = 0;
	COleDateTime dtStart;
	COleDateTime dt;
	int iDayCounter = 0;

	if (!m_bFontsCreated)
		CreateFontObjects();

	dtStart.SetDate(iYear, iMonth, 1);

	dt = dtStart;
	while (dt.GetDayOfWeek() != m_iFirstDayOfWeek)
		dt -= 1;

	GetClientRect(ClientRect);

	// calculate starting X position
	iStartX = ((iLeftX + (m_szMonthSize.cx / 2))) - (((m_iIndividualDayWidth*7) + 30) / 2);
	iEndX = ((iLeftX + (m_szMonthSize.cx / 2))) + (((m_iIndividualDayWidth*7) + 30) / 2);

	CFont* pOldFont = dc.SelectObject(m_FontInfo[FMC_FONT_DAYS].m_pFont);
	dc.SetBkColor(::GetSysColor(COLOR_BTNFACE));
	dc.SetTextColor(m_FontInfo[FMC_FONT_DAYS].m_crTextColor);
	dc.SetBkMode(TRANSPARENT);

	// we allow up to 6 rows of days.  This is the actual maximum # of calendar
	// weeks that can occur in a month.
	int iRow;

	for (iRow = 1; iRow <= 6; iRow++)
	{
		int iX = iStartX;

		for (int iDayOfWeek = 1; iDayOfWeek <= 7; iDayOfWeek++)
		{
			if (dt.GetMonth() == dtStart.GetMonth() ||
				(dt > dtStart && m_bShowNonMonthDays) ||
				(dt < dtStart && m_bShowNonMonthDays))
			{
				BOOL bSelected = IsDateSelected(dt);
				BOOL bSpecial = IsSpecialDate(dt);
				BOOL bActiveMonth = (dt.GetMonth() == iMonth);

				CRect rCell(iX, iY, iX + m_iIndividualDayWidth+5, iY + m_iDaysHeight+2);
				DrawCellBkgnd(dc, rCell, dt, bSelected, bSpecial, bActiveMonth);

				if (bSpecial && bActiveMonth)
					pOldFont = dc.SelectObject(m_FontInfo[FMC_FONT_SPECIALDAYS].m_pFont);
				else
					pOldFont = dc.SelectObject(m_FontInfo[FMC_FONT_DAYS].m_pFont);

				COLORREF cTextColor = GetCellTextColor(dt, bSelected, bSpecial, bActiveMonth);
				dc.SetTextColor(cTextColor);
				dc.DrawText(CStr(dt.GetDay()), rCell, DEFTEXTFLAGS);

				SetHotSpot(iMonthRow, iMonthCol, iDayCounter, dt, rCell);
			}

			dt += 1;
			iX += (m_iIndividualDayWidth + 5);
			iDayCounter++;
		}

		iY += (2 + m_iDaysHeight);
		iReturn += (2 + m_iDaysHeight);
	}

	dc.SelectObject(pOldFont);

	// draw the divider line
	if (iRow == m_iRows)
	{
		CPen* pPen = new CPen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
		CPen* pOldPen = dc.SelectObject(pPen);

		dc.MoveTo(iStartX, iY);
		dc.LineTo(iEndX, iY);

		dc.SelectObject(pOldPen);
		delete pPen;
	}

	iReturn += 5;

	SetCellPosition(iMonthRow, iMonthCol, CRect(iStartX, iStartY, iEndX, iY));

	return iReturn;
}

void CFPSMiniCalendarCtrl::DrawCellBkgnd(CDC& dc, const CRect& rCell, const COleDateTime& dt, BOOL bSelected, BOOL bSpecial, BOOL bActiveMonth)
{
	COLORREF crBkgnd = GetCellBkgndColor(dt, bSelected, bSpecial, bActiveMonth);

	if (crBkgnd != CLR_NONE)
		dc.FillSolidRect(rCell, crBkgnd);

	if (m_bHighlightToday && IsToday(dt))
		dc.Draw3dRect(rCell, m_crTodayBorder, m_crTodayBorder);
}

COLORREF CFPSMiniCalendarCtrl::GetCellBkgndColor(const COleDateTime& dt, BOOL bSelected, BOOL bSpecial, BOOL bActiveMonth) const
{
	if (bActiveMonth)
	{
		if (bSelected)
			return ::GetSysColor(COLOR_HIGHLIGHT);

		if (bSpecial)
			return m_FontInfo[FMC_FONT_SPECIALDAYS].m_crBkColor;
	}

	// else
	return m_FontInfo[FMC_FONT_DAYS].m_crBkColor;
}

COLORREF CFPSMiniCalendarCtrl::GetCellTextColor(const COleDateTime& dt, BOOL bSelected, BOOL bSpecial, BOOL bActiveMonth) const
{
	if (bActiveMonth)
	{
		if (bSelected)
			return ::GetSysColor(COLOR_HIGHLIGHTTEXT);

		if (bSpecial)
			return m_FontInfo[FMC_FONT_SPECIALDAYS].m_crTextColor;
	}
	else
	{
		return m_cNonMonthDayColor;
	}

	return m_FontInfo[FMC_FONT_DAYS].m_crTextColor;
}

BOOL CFPSMiniCalendarCtrl::IsToday(const COleDateTime &dt) const
{
	COleDateTime dtNow = COleDateTime::GetCurrentTime();

	return ((int)dt.m_dt == (int)dtNow.m_dt);
}

int CFPSMiniCalendarCtrl::DrawTodayButton(CDC &dc, int iY)
{
	CRect ClientRect;
	int iReturn = 0;
	CString strText = "Today";

	if (!m_bFontsCreated)
		CreateFontObjects();

	GetClientRect(ClientRect);

	CFont* pOldFont = dc.SelectObject(m_FontInfo[FMC_FONT_DAYSOFWEEK].m_pFont);
	dc.SetBkColor(::GetSysColor(COLOR_BTNFACE));
	dc.SetTextColor(m_FontInfo[FMC_FONT_DAYSOFWEEK].m_crTextColor);
	dc.SetBkMode(TRANSPARENT);

	int iTextWidth = dc.GetTextExtent(strText).cx;
	int iTextHeight = dc.GetTextExtent(strText).cy;
	int iButtonWidth = iTextWidth + 12;
	int iButtonHeight = iTextHeight + 6;

	int iX = (ClientRect.Width() / 2) - (iButtonWidth / 2);

	if (m_bShowTodayButton && m_bShowNoneButton)
		iX = (ClientRect.Width() - ((iButtonWidth*2)+10)) / 2;

	if (m_bShowTodayButton)
	{
		strText = "Today";
		dc.FillSolidRect(iX, iY, iButtonWidth, iButtonHeight, ::GetSysColor(COLOR_BTNFACE));

		if (m_bTodayTracking && m_bTodayDown)
		{
			dc.Draw3dRect(iX, iY, iButtonWidth, iButtonHeight, ::GetSysColor(COLOR_BTNTEXT), ::GetSysColor(COLOR_BTNFACE));
			dc.Draw3dRect(iX+1, iY+1, iButtonWidth-2, iButtonHeight-2, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_WINDOW));

			dc.DrawText(strText, CRect(iX+2,iY+2,iX+iButtonWidth,iY+iButtonHeight), DEFTEXTFLAGS);
		}
		else
		{
			dc.Draw3dRect(iX, iY, iButtonWidth, iButtonHeight, ::GetSysColor(COLOR_BTNFACE), ::GetSysColor(COLOR_BTNTEXT));
			dc.Draw3dRect(iX+1, iY+1, iButtonWidth-2, iButtonHeight-2, ::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DSHADOW));

			dc.DrawText(strText, CRect(iX,iY,iX+iButtonWidth,iY+iButtonHeight), DEFTEXTFLAGS);
		}

		m_rectToday.SetRect(iX, iY, iX+iButtonWidth, iY+iButtonHeight);

		iX += iButtonWidth;
		iX += 10;
	}

	if (m_bShowNoneButton)
	{
		strText = "None";
		dc.FillSolidRect(iX, iY, iButtonWidth, iButtonHeight, ::GetSysColor(COLOR_BTNFACE));

		if (m_bNoneTracking && m_bNoneDown)
		{
			dc.Draw3dRect(iX, iY, iButtonWidth, iButtonHeight, ::GetSysColor(COLOR_BTNTEXT), ::GetSysColor(COLOR_BTNFACE));
			dc.Draw3dRect(iX+1, iY+1, iButtonWidth-2, iButtonHeight-2, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_WINDOW));

			dc.DrawText(strText, CRect(iX+2,iY+2,iX+iButtonWidth,iY+iButtonHeight), DEFTEXTFLAGS);
		}
		else
		{
			dc.Draw3dRect(iX, iY, iButtonWidth, iButtonHeight, ::GetSysColor(COLOR_BTNFACE), ::GetSysColor(COLOR_BTNTEXT));
			dc.Draw3dRect(iX+1, iY+1, iButtonWidth-2, iButtonHeight-2, ::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DSHADOW));

			dc.DrawText(strText, CRect(iX,iY,iX+iButtonWidth,iY+iButtonHeight), DEFTEXTFLAGS);
		}

		m_rectNone.SetRect(iX, iY, iX+iButtonWidth, iY+iButtonHeight);

		iX += iButtonWidth;
		iX += 10;
	}

	dc.SelectObject(pOldFont);


	return iReturn;
}

void CFPSMiniCalendarCtrl::OnDevModeChange(LPTSTR lpDeviceName) 
{
	CWnd::OnDevModeChange(lpDeviceName);
		
	RedrawWindow();	
}

void CFPSMiniCalendarCtrl::OnFontChange() 
{
	CWnd::OnFontChange();
	
	RedrawWindow();	
}

void CFPSMiniCalendarCtrl::OnPaletteChanged(CWnd* pFocusWnd) 
{
	CWnd::OnPaletteChanged(pFocusWnd);
	
	RedrawWindow();	
}

void CFPSMiniCalendarCtrl::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CWnd::OnSettingChange(uFlags, lpszSection);
	
	RedrawWindow();	
}

void CFPSMiniCalendarCtrl::OnTimeChange() 
{
	CWnd::OnTimeChange();
	
	RedrawWindow();	
}

void CFPSMiniCalendarCtrl::OnSysColorChange() 
{
	CWnd::OnSysColorChange();
	
	RedrawWindow();	
}

// clear all hot-spots
void CFPSMiniCalendarCtrl::ClearHotSpots()
{
	for (int iX = 0; iX < m_iCells; iX++)
	{
		for (int iDay = 0; iDay < 42; iDay++)
		{
			m_parCells[iX].m_parHotSpots[iDay].m_dt.SetStatus(COleDateTime::valid);
			m_parCells[iX].m_parHotSpots[iDay].m_rect.SetRect(0,0,0,0);
		}
	}
}

BOOL CFPSMiniCalendarCtrl::IsDateSelected(COleDateTime &dt)
{
	BOOL bReturn = FALSE;

	if (m_dtSelBegin.GetStatus() == COleDateTime::valid &&
		m_dtSelEnd.GetStatus() == COleDateTime::valid)
	{
		if (dt >= m_dtSelBegin && dt <= m_dtSelEnd)
			bReturn = TRUE;
	}

	return bReturn;
}

void CFPSMiniCalendarCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_dtTrackingStart.SetStatus(COleDateTime::invalid);

	SetFocus();

	if (m_rectScrollLeft.PtInRect(point))
	{
		m_bScrollTracking = TRUE;
		ScrollPage(-1);
	}
	else if (m_rectScrollRight.PtInRect(point))
	{
		m_bScrollTracking = TRUE;
		ScrollPage(1);
	}
	else if (m_rectToday.PtInRect(point))
	{
		m_bTodayTracking = TRUE;
		m_bTodayDown = TRUE;
		RedrawWindow();
	}
	else if (m_rectNone.PtInRect(point))
	{
		m_bNoneTracking = TRUE;
		m_bNoneDown = TRUE;
		RedrawWindow();
	}
	else if (HeaderHitTest(point))
	{
		CFPSMiniCalendarCtrlCell* pCell = HeaderHitTest(point);

		ASSERT(pCell);

		if (pCell)
		{
			m_pHeaderCell = pCell;

			int iCell = (((pCell->m_iRow-1) * m_iCols) + pCell->m_iCol)-1;

			// determine month/year of header
			int iMonth = m_iCurrentMonth;
			int iYear = m_iCurrentYear;

			for (int iX = 0; iX < iCell; iX++)
			{
				iMonth++;
				if (iMonth > 12)
				{
					iMonth = 1;
					iYear++;
				}
			}

			// make sure list is not already created
			ASSERT(!m_pHeaderList);

			if (m_pHeaderList)
				delete m_pHeaderList;
			m_pHeaderList = NULL;

			// create list
			m_pHeaderList = new CFPSMiniCalendarListCtrl;
			ASSERT(m_pHeaderList);

			if (!m_pHeaderList)
				throw (new CMemoryException());

			// create list control
			DWORD dwStyle = WS_POPUP | WS_EX_TOPMOST | WS_EX_WINDOWEDGE | WS_BORDER;
			LPCTSTR szWndCls = AfxRegisterWndClass(CS_BYTEALIGNCLIENT | CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS,
												   0,0,0);

			m_iHeaderTimerID = SetTimer(FMC_HEADER_TIMER_ID, FMC_HEADER_TIMER_INTERVAL, NULL);

			m_pHeaderList->SetMiddleMonthYear(iMonth, iYear);
			m_pHeaderList->SetCalendar(this);
			m_pHeaderList->CreateEx(0, 
								szWndCls, 
								NULL, 
								dwStyle, 
								0, 
								0, 
								0, 
								0, 
								m_hWnd, 
								NULL, 
								NULL);

			// set font
			m_pHeaderList->SetFontInfo(m_FontInfo[FMC_FONT_DAYS]);

			m_pHeaderList->AutoConfigure();
			m_pHeaderList->ShowWindow(TRUE);

			SetCapture();
			m_bHeaderTracking = TRUE;
		}
	}
	else
	{
		CFPSMiniCalendarCtrlFontHotSpot* pSpot = HitTest(point);

		if (pSpot)
		{
			if (m_bMultiSel)
			{
				ClearSelections();

				m_dtTrackingStart = pSpot->m_dt;
				m_dtSelBegin = pSpot->m_dt;
				m_dtSelEnd = pSpot->m_dt;

				SetCapture();
				m_bTracking = TRUE;
				RedrawWindow();
			}
			else
			{
				m_dtSelBegin = pSpot->m_dt;
				m_dtSelEnd = pSpot->m_dt;

				SetCapture();
				m_bTracking = TRUE;
				RedrawWindow();
				//FireNotifyClick();
			}
		}
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CFPSMiniCalendarCtrl::OnCaptureChanged(CWnd* pWnd)
{
	CWnd::OnCaptureChanged(pWnd);

	m_bTracking = FALSE;
	m_bHeaderTracking = FALSE;
}

void CFPSMiniCalendarCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bTracking)
	{
		ReleaseCapture();
		RedrawWindow();
		FireNotifyClick();
	}
	else if (m_bScrollTracking)
	{
		m_bScrollTracking = FALSE;
		RedrawWindow();
	}
	else if (m_bTodayTracking)
	{
		if (m_bTodayDown)
			FireTodayButton();

		m_bTodayDown = FALSE;
		m_bTodayTracking = FALSE;
		RedrawWindow();
	}
	else if (m_bNoneTracking)
	{
		if (m_bNoneDown)
			FireNoneButton();

		m_bNoneDown = FALSE;
		m_bNoneTracking = FALSE;
		RedrawWindow();
	}
	else if (m_bHeaderTracking)
	{
		ASSERT(m_pHeaderList);

		if (m_pHeaderList && m_pHeaderCell)
		{
			COleDateTime dtPrev(GetCurrentMonthAndYear());

			int iSelMonth = m_pHeaderList->GetSelMonth();
			int iSelYear = m_pHeaderList->GetSelYear();

			int iCell = (((m_pHeaderCell->m_iRow-1) * m_iCols) + m_pHeaderCell->m_iCol)-1;

			for (int iX = 0; iX < iCell; iX++)
			{
				iSelMonth--;
				if (iSelMonth < 1)
				{
					iSelMonth = 12;
					iSelYear--;
				}
			}

			SetCurrentMonthAndYear(iSelMonth, iSelYear);

			int nDirection = ((GetCurrentMonthAndYear() < dtPrev) ? SB_LEFT : SB_RIGHT);
			FireNotifyHScroll(nDirection);
		}

		if (m_pHeaderList)
			delete m_pHeaderList;
		m_pHeaderList = NULL;

		m_pHeaderCell = NULL;

		if (m_iHeaderTimerID != 0)
			KillTimer(m_iHeaderTimerID);
		m_iHeaderTimerID = 0;

		ReleaseCapture();
		RedrawWindow();
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CFPSMiniCalendarCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bHeaderTracking)
	{
		m_pHeaderList->ForwardMessage(GetCurrentMessage());
	}
	else if (m_bTracking)
	{
		CFPSMiniCalendarCtrlFontHotSpot* pSpot = HitTest(point);

		if (pSpot)
		{
			if (m_bMultiSel)
			{
				COleDateTime dtStart;
				COleDateTime dtEnd;
				COleDateTime dt;

				if (pSpot->m_dt < m_dtTrackingStart)
				{
					dtStart = pSpot->m_dt;
					dtEnd = m_dtTrackingStart;
				}
				else if (pSpot->m_dt >m_dtTrackingStart)
				{
					dtStart = m_dtTrackingStart;
					dtEnd = pSpot->m_dt;
				}
				else
				{
					dtStart = pSpot->m_dt;
					dtEnd = pSpot->m_dt;
				}

				// if a maximum # of selection days is set, we must
				// make sure this rule is adhered to
				COleDateTimeSpan span = dtEnd - dtStart;

				if (m_iMaxSelDays == FMC_NO_MAX_SEL_DAYS || span.GetDays() <= m_iMaxSelDays)
				{
					ClearSelections();
					m_dtSelBegin = dtStart;
					m_dtSelEnd = dtEnd;
				}
				else
				{
					ClearSelections();

					m_dtSelBegin = dtStart;
					m_dtSelEnd = dtStart;
					m_dtSelEnd += m_iMaxSelDays;

					if (m_dtSelBegin > m_dtTrackingStart && m_dtSelEnd > m_dtTrackingStart)
					{
						m_dtSelBegin = m_dtTrackingStart;
						m_dtSelEnd = m_dtTrackingStart;
						m_dtSelEnd += m_iMaxSelDays;
					}
					else if (m_dtSelBegin < m_dtTrackingStart && m_dtSelEnd < m_dtTrackingStart)
					{
						m_dtSelEnd = m_dtTrackingStart;
						m_dtSelBegin = m_dtSelEnd;
						m_dtSelBegin -= m_iMaxSelDays;
					}
				}
			}
			else
			{
				SetDate(pSpot->m_dt);
			}

			RedrawWindow();
		}
	}
	else if (m_bScrollTracking)
	{
		if (m_rectScrollLeft.PtInRect(point))
		{
			ScrollPage(-1);
		}
		else if (m_rectScrollRight.PtInRect(point))
		{
			ScrollPage(1);
		}
	}
	else if (m_bTodayTracking)
	{
		if (m_rectToday.PtInRect(point))
		{
			if (!m_bTodayDown)
			{
				m_bTodayDown = TRUE;
				RedrawWindow();
			}
		}
		else
		{
			if (m_bTodayDown)
			{
				m_bTodayDown = FALSE;
				RedrawWindow();
			}
		}
	}
	else if (m_bNoneTracking)
	{
		if (m_rectNone.PtInRect(point))
		{
			if (!m_bNoneDown)
			{
				m_bNoneDown = TRUE;
				RedrawWindow();
			}
		}
		else
		{
			if (m_bNoneDown)
			{
				m_bNoneDown = FALSE;
				RedrawWindow();
			}
		}
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CFPSMiniCalendarCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{	
	ScrollMonth((zDelta < 0) ? 1 : -1);

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CFPSMiniCalendarCtrl::ScrollMonth(int nNumMonths)
{
	int nInc = ((nNumMonths < 0) ? -1 : 1);

	for (int nMonth = 0; nMonth != nNumMonths; nMonth += nInc)
	{
		m_iCurrentMonth += nInc;

		if ((nInc < 0) && (m_iCurrentMonth < 1))
		{
			m_iCurrentMonth = 12;
			m_iCurrentYear--;
		}
		else if ((nInc > 0) && (m_iCurrentMonth > 12))
		{
			m_iCurrentMonth = 1;
			m_iCurrentYear++;
		}
	}
	
	ClearHotSpots();
	RedrawWindow();
	FireNotifyHScroll((nNumMonths < 0) ? SB_LEFT : SB_RIGHT);
}

void CFPSMiniCalendarCtrl::ScrollPage(int nNumPages)
{
	ASSERT((nNumPages >= -100) && (nNumPages <= 100));

	ScrollMonth(nNumPages * m_iRows * m_iCols);
}

void CFPSMiniCalendarCtrl::FireTodayButton()
{
	COleDateTime dtNow = COleDateTime::GetCurrentTime();
	int iMonth = dtNow.GetMonth();
	int iDay = dtNow.GetDay();
	int iYear = dtNow.GetYear();

	dtNow.SetStatus(COleDateTime::invalid);
	dtNow.SetDate(iYear, iMonth, iDay);

	m_iCurrentMonth = dtNow.GetMonth();
	m_iCurrentYear = dtNow.GetYear();

	ClearSelections();
	m_dtSelBegin = dtNow;
	m_dtSelEnd = dtNow;
	RedrawWindow();

	FireNotifyClick();
}

void CFPSMiniCalendarCtrl::FireNotifyClick()
{
	NMHDR	NotifyArea;

	NotifyArea.code = NM_CLICK;
	NotifyArea.hwndFrom = m_hWnd;
	NotifyArea.idFrom = ::GetDlgCtrlID(m_hWnd);

	GetParent()->SendMessage(WM_NOTIFY, ::GetDlgCtrlID(m_hWnd), (WPARAM)&NotifyArea);
}

void CFPSMiniCalendarCtrl::FireNotifyDblClick()
{
	NMHDR	NotifyArea;

	NotifyArea.code = NM_DBLCLK;
	NotifyArea.hwndFrom = m_hWnd;
	NotifyArea.idFrom = ::GetDlgCtrlID(m_hWnd);

	GetParent()->SendMessage(WM_NOTIFY, ::GetDlgCtrlID(m_hWnd), (WPARAM)&NotifyArea);
}

void CFPSMiniCalendarCtrl::FireNotifyHScroll(int nDirection)
{
	GetParent()->SendMessage(WM_HSCROLL, MAKEWPARAM(nDirection, 0), (LPARAM)GetSafeHwnd());
}

COleDateTime CFPSMiniCalendarCtrl::GetDate()
{
	if (m_bMultiSel)
	{
		COleDateTime dtReturn;
		dtReturn.SetStatus(COleDateTime::invalid);

		return dtReturn;
	}
	else
	{
		COleDateTime dtReturn;

		dtReturn = m_dtSelBegin;

		return dtReturn;
	}
}

void CFPSMiniCalendarCtrl::GetDateSel(COleDateTime& dtBegin, COleDateTime& dtEnd)
{
	dtBegin = m_dtSelBegin;
	dtEnd = m_dtSelEnd;
}

void CFPSMiniCalendarCtrl::FireNoneButton()
{
	COleDateTime dtNow = COleDateTime::GetCurrentTime();

	m_iCurrentMonth = dtNow.GetMonth();
	m_iCurrentYear = dtNow.GetYear();

	ClearSelections();
	RedrawWindow();

	FireNotifyClick();
}

void CFPSMiniCalendarCtrl::SetRowsAndColumns(int iRows, int iCols)
{
	// MENTAL NOTE: I am still debating whether or not to set a max # of rows/columns
	// it seems like it would make sense, but I don't know what to make it so I am 
	// making it rediculously? large

	ASSERT(iRows > 0);
	ASSERT(iCols > 0);
	ASSERT(iRows < 100);
	ASSERT(iCols < 100);

	if (iRows > 0 && iCols > 0 && iRows < 100 && iCols < 100)
	{
		m_iRows = iRows;
		m_iCols = iCols;

		AllocateCells();

		if (m_bUseAutoSettings)
			AutoConfigure();
	}
}

CFPSMiniCalendarCtrlFontHotSpot* CFPSMiniCalendarCtrl::HitTest(POINT& pt) const
{
	int iCell = 0;
	BOOL bFound = FALSE;
	CFPSMiniCalendarCtrlFontHotSpot* pReturn = NULL;

	while (iCell < m_iCells && !bFound)
	{
		if (m_parCells[iCell].m_rect.PtInRect(pt))
		{
			int iDay = 0;

			while (iDay < 42 && !bFound)
			{
				if (m_parCells[iCell].m_parHotSpots[iDay].m_rect.PtInRect(pt))
				{
					pReturn = &m_parCells[iCell].m_parHotSpots[iDay];
					bFound = TRUE;
				}

				iDay++;
			}
		}

		iCell++;
	}

	return pReturn;
}

void CFPSMiniCalendarCtrl::SetCellHeaderPosition(int iMonthRow, int iMonthCol, RECT rect)
{
	ASSERT(iMonthRow >= 0);
	ASSERT(iMonthCol >= 0);
	ASSERT(iMonthRow <= m_iRows);
	ASSERT(iMonthCol <= m_iCols);

	int iCell = (((iMonthRow-1)*m_iCols) + iMonthCol) - 1;

	ASSERT(iCell >= 0);
	ASSERT(iCell < m_iCells);

	if (iCell >= 0 && iCell < m_iCells)
	{
		m_parCells[iCell].m_rectHeader = rect;
		m_parCells[iCell].m_iRow = iMonthRow;
		m_parCells[iCell].m_iCol = iMonthCol;
	}
}

void CFPSMiniCalendarCtrl::SetCellPosition(int iMonthRow, int iMonthCol, RECT rect)
{
	ASSERT(iMonthRow >= 0);
	ASSERT(iMonthCol >= 0);
	ASSERT(iMonthRow <= m_iRows);
	ASSERT(iMonthCol <= m_iCols);

	int iCell = (((iMonthRow-1)*m_iCols) + iMonthCol) - 1;

	ASSERT(iCell >= 0);
	ASSERT(iCell < m_iCells);

	if (iCell >= 0 && iCell < m_iCells)
	{
		m_parCells[iCell].m_rect = rect;
	}
}

void CFPSMiniCalendarCtrl::SetHotSpot(int iMonthRow, int iMonthCol, int iDayCounter, COleDateTime& dt, RECT rect)
{
	ASSERT(iMonthRow >= 0);
	ASSERT(iMonthCol >= 0);
	ASSERT(iMonthRow <= m_iRows);
	ASSERT(iMonthCol <= m_iCols);

	int iCell = (((iMonthRow-1)*m_iCols) + iMonthCol) - 1;

	ASSERT(iCell >= 0);
	ASSERT(iCell < m_iCells);
	ASSERT(iDayCounter >= 0);
	ASSERT(iDayCounter < 42);

	if (iCell >= 0 && iCell < m_iCells && iDayCounter >= 0 && iDayCounter < 42)
	{
		m_parCells[iCell].m_parHotSpots[iDayCounter].m_dt = dt;
		m_parCells[iCell].m_parHotSpots[iDayCounter].m_rect = rect;
	}
}

void CFPSMiniCalendarCtrl::AllocateCells()
{
	int iCells = m_iRows * m_iCols;

	if (iCells > 0)
	{
		if (m_parCells)
			delete[] m_parCells;
		m_parCells = NULL;

		m_parCells = new CFPSMiniCalendarCtrlCell[iCells];

		ASSERT(m_parCells);
		if (!m_parCells)
			throw (new CMemoryException());

		m_parCells->m_rect.SetRect(0,0,0,0);

		m_iCells = iCells;
	}
}

void CFPSMiniCalendarCtrl::ClearSelections()
{
	m_dtSelBegin.SetStatus(COleDateTime::invalid);
	m_dtSelEnd.SetStatus(COleDateTime::invalid);
}


int CFPSMiniCalendarCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (lpCreateStruct->style & FMC_MULTISELECT)
		SetMultiSelect(TRUE);
	if (lpCreateStruct->style & FMC_NOHIGHLIGHTTODAY)
		SetHighlightToday(FALSE, 0);
	if (lpCreateStruct->style & FMC_TODAYBUTTON)
		SetShowTodayButton(TRUE);
	if (lpCreateStruct->style & FMC_NONEBUTTON)
		SetShowNoneButton(TRUE);
	if (lpCreateStruct->style & FMC_NO3DBORDER)
		SetShow3DBorder(FALSE);
	if (lpCreateStruct->style & FMC_AUTOSETTINGS)
		SetUseAutoSettings(TRUE);
	if (lpCreateStruct->style & FMC_NOSHOWNONMONTHDAYS)
		SetShowNonMonthDays(FALSE);
	
	if (m_bUseAutoSettings)
		AutoConfigure();

	return 0;
}

void CFPSMiniCalendarCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CFPSMiniCalendarCtrlFontHotSpot* pSpot = HitTest(point);

	if (pSpot)
	{
		m_dtSelBegin = pSpot->m_dt;
		m_dtSelEnd = pSpot->m_dt;
		RedrawWindow();
		FireNotifyDblClick();
	}
	
	CWnd::OnLButtonDblClk(nFlags, point);
}

// resize this window within its parent
void CFPSMiniCalendarCtrl::AutoSize()
{
	if (::IsWindow(m_hWnd))
	{
		CSize size = ComputeTotalSize();

		SetWindowPos(NULL, 0, 0, size.cx, size.cy, SWP_NOMOVE);
	}
}

// auto-configue attempts to optimize the font size for the control
// based on 1) maximum final size of the control, 2) number of rows
// and columns, 3) screen resolution, 4) minimum allowed font size
void CFPSMiniCalendarCtrl::AutoConfigure()
{
	if (::IsWindow(m_hWnd))
	{
		// first, lets determine the font sizes based on
		// screen resolution
		CString strFontName = m_strDefaultFontName;
		int iFontSize = FMC_DEFAULT_FONT_SIZE;

		if (::GetSystemMetrics(SM_CXSCREEN) < 800)
			iFontSize = FMC_DEFAULT_FONT_SIZE_640;
		else if (::GetSystemMetrics(SM_CXSCREEN) < 1024)
			iFontSize = FMC_DEFAULT_FONT_SIZE_800;
		else if (::GetSystemMetrics(SM_CXSCREEN) < 1280)
			iFontSize = FMC_DEFAULT_FONT_SIZE_1024;
		else if (::GetSystemMetrics(SM_CXSCREEN) >= 1280)
			iFontSize = FMC_DEFAULT_FONT_SIZE_1280;

		SetFontInfo(FMC_FONT_HEADER, m_strDefaultFontName, FMC_DEFAULT_FONT_SIZE, TRUE);
		SetFontInfo(FMC_FONT_DAYSOFWEEK, m_strDefaultFontName, FMC_DEFAULT_FONT_SIZE);
		SetFontInfo(FMC_FONT_DAYS, m_strDefaultFontName, FMC_DEFAULT_FONT_SIZE);
		SetFontInfo(FMC_FONT_SPECIALDAYS, m_strDefaultFontName, FMC_DEFAULT_FONT_SIZE, TRUE);

		CSize szMax = m_szMaxSize;
		CSize size = ComputeTotalSize();

		if (szMax != FMC_MAX_SIZE_NONE)
		{
			if (szMax == FMC_MAX_SIZE_PARENT && GetParent())
			{
				CRect rectParent;
				CRect rectMe;

				GetWindowRect(rectMe);
				GetParent()->GetWindowRect(rectParent);
				GetParent()->ScreenToClient(rectMe);

				szMax.cx = rectParent.Width();
				szMax.cy = rectParent.Height();

				szMax.cx -= rectMe.left;
				szMax.cy -= rectMe.top;
			}

			while ((size.cx > szMax.cx || size.cy > szMax.cy) && iFontSize > m_iDefaultMinFontSize)
			{
				iFontSize--;

				SetFontInfo(FMC_FONT_HEADER, strFontName, iFontSize, TRUE);
				SetFontInfo(FMC_FONT_DAYSOFWEEK, strFontName, iFontSize);
				SetFontInfo(FMC_FONT_DAYS, strFontName, iFontSize);
				SetFontInfo(FMC_FONT_SPECIALDAYS, strFontName, iFontSize, TRUE);

				size = ComputeTotalSize();
			}
		}
	}

	AutoSize();
}

void CFPSMiniCalendarCtrl::SetMaxSize(SIZE size)
{
	m_szMaxSize = size;
}

void CFPSMiniCalendarCtrl::SetDefaultMinFontSize(int iValue)
{
	ASSERT(iValue >= 0); 
	ASSERT(iValue <= 72);
	
	if (iValue >= 0 && iValue <= 72) 
		m_iDefaultMinFontSize = iValue;
}

void CFPSMiniCalendarCtrl::SetDefaultFont(LPCTSTR lpszValue) 
{
	ASSERT(lpszValue);
	ASSERT(AfxIsValidString(lpszValue));

	if (lpszValue && AfxIsValidString(lpszValue))
		m_strDefaultFontName = lpszValue;
}

void CFPSMiniCalendarCtrl::SetMonthName(int iMonth, LPCTSTR lpszName) 
{
	ASSERT(iMonth > 0); 
	ASSERT(iMonth <= 12); 
	ASSERT(lpszName);
	ASSERT(AfxIsValidString(lpszName));
	
	if (iMonth > 0 && iMonth <= 12) 
		m_arMonthNames[iMonth-1] = lpszName;
}

CString CFPSMiniCalendarCtrl::GetMonthName(int iMonth) 
{
	ASSERT(iMonth > 0); 
	ASSERT(iMonth <= 12); 
	
	if (iMonth > 0 && iMonth <= 12)
		return m_arMonthNames[iMonth-1];
	else
		return "";
}

void CFPSMiniCalendarCtrl::SetDayOfWeekName(int iDayOfWeek, LPCTSTR lpszName) 
{
	ASSERT(iDayOfWeek > 0); 
	ASSERT(iDayOfWeek <= 7); 
	ASSERT(lpszName);
	ASSERT(AfxIsValidString(lpszName));
	
	if (iDayOfWeek > 0 && iDayOfWeek <= 7) 
		m_arDaysOfWeekNames[iDayOfWeek-1] = lpszName;
}

CString CFPSMiniCalendarCtrl::GetDayOfWeekName(int iDayOfWeek) 
{
	ASSERT(iDayOfWeek > 0); 
	ASSERT(iDayOfWeek <= 7); 
	
	if (iDayOfWeek > 0 && iDayOfWeek <= 7)
		return m_arDaysOfWeekNames[iDayOfWeek-1];
	else
		return "";
}

void CFPSMiniCalendarCtrl::SetFirstDayOfWeek(int iDayOfWeek)
{
	ASSERT(iDayOfWeek > 0); 
	ASSERT(iDayOfWeek <= 7); 
	
	if (iDayOfWeek > 0 && iDayOfWeek <= 7)
	{
		m_iFirstDayOfWeek = iDayOfWeek;

		setlocale(LC_TIME, "");			// should I be doing this here AND am I doing it right???
		COleDateTime dtTemp = COleDateTime::GetCurrentTime();

		// find the specified day of the week
		while (dtTemp.GetDayOfWeek() != iDayOfWeek)
			dtTemp += 1;

		for (int iX = 0; iX < 7; iX++)
		{
			CString strName = dtTemp.Format(_T("%A")).Left(1);
			m_arDaysOfWeekNames[iX] = strName;

			dtTemp += 1;
		}
	}
}

int CFPSMiniCalendarCtrl::GetFirstDayOfWeek() const
{
	return m_iFirstDayOfWeek;
}

void CFPSMiniCalendarCtrl::SetDateSel(COleDateTime dtBegin, COleDateTime dtEnd)
{
	if (dtEnd.GetStatus() == COleDateTime::valid && dtBegin.GetStatus() == COleDateTime::valid)
	{
		if (dtEnd < dtBegin)
		{
			m_dtSelBegin.SetDate(dtEnd.GetYear(), dtEnd.GetMonth(), dtEnd.GetDay());
			m_dtSelEnd.SetDate(dtBegin.GetYear(), dtBegin.GetMonth(), dtBegin.GetDay());
		}
		else
		{
			m_dtSelBegin = dtBegin;
			m_dtSelEnd.SetDate(dtBegin.GetYear(), dtBegin.GetMonth(), dtBegin.GetDay());
		}
	}
	else
	{
		m_dtSelBegin.SetStatus(COleDateTime::invalid);
		m_dtSelEnd.SetStatus(COleDateTime::invalid);
	}
}

void CFPSMiniCalendarCtrl::SetDate(COleDateTime dt) 
{
	if (dt.GetStatus() == COleDateTime::valid)
	{
		m_dtSelBegin.SetDate(dt.GetYear(), dt.GetMonth(), dt.GetDay());
		m_dtSelEnd.SetDate(dt.GetYear(), dt.GetMonth(), dt.GetDay());

		// make sure the appropriate month and year are visible
		int nMonth = dt.GetMonth(), nYear = dt.GetYear();

		if ((nMonth < m_iCurrentMonth) || (nMonth >= (m_iCurrentMonth + m_iRows)) || (nYear != m_iCurrentYear))
		{
			SetCurrentMonthAndYear(nMonth, nYear);
		}
	}
	else
	{
		m_dtSelBegin.SetStatus(COleDateTime::invalid);
		m_dtSelEnd.SetStatus(COleDateTime::invalid);
	}

	if (GetSafeHwnd())
		Invalidate(FALSE);
}

// determines whether or not a given date is special.  This function
// calls a callback function to make this determination
BOOL CFPSMiniCalendarCtrl::IsSpecialDate(const COleDateTime &dt) const
{
	if (m_pfuncCallback)
	{
		try
		{
			if (m_pfuncCallback(dt, m_dwCallbackData))
				return TRUE;
		}
		catch(...)
		{
			ASSERT(FALSE);
		}
	}

	return FALSE;
}

// set the special days callback function
void CFPSMiniCalendarCtrl::SetSpecialDaysCallback(funcSPECIALDATE pValue, DWORD dwUserData)
{
	m_pfuncCallback = pValue;
	m_dwCallbackData = dwUserData;
}

CFPSMiniCalendarCtrlCell* CFPSMiniCalendarCtrl::HeaderHitTest(POINT& point) const
{
	CFPSMiniCalendarCtrlCell* pReturn = NULL;

	int iCell = 0;

	while (iCell < m_iCells && !pReturn)
	{
		if (m_parCells[iCell].m_rectHeader.PtInRect(point))
			pReturn = &m_parCells[iCell];

		iCell++;
	}

	return pReturn;
}

BOOL CFPSMiniCalendarCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (m_bHeaderTracking && m_pHeaderList)
		m_pHeaderList->ForwardMessage(pMsg);

	return CWnd::PreTranslateMessage(pMsg);
}

