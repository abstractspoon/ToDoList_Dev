/* ==========================================================================
	File :			RRECEnToolBar.cpp

	Class :			CRRECToolBar

	Author :		Johan Rosengren, Abstrakt Mekanik AB
					Iain Clarke

	Date :			2004-05-07

	Purpose :		This class encapsulates a toolbar that can be used with 
					"CRulerRichEditCtrl". The class is derived from "CEnToolBar", 
					and manages a formatting toolbar

	Description :	A "CEnToolBar"-derived class. Reads a toolbar resource 
					with the ID "TOOLBAR_CONTROL" and adds combo controls for 
					font name and -size, as well as a color picker at the 
					positions "FONT_NAME_POS", "FONT_SIZE_POS" and 
					"FONT_COLOR_POS" respectively.

	Usage :			Created by the rich edit mini-editor.

    Note :			Very heavily modified by .dan.g.

   ========================================================================*/

#include "stdafx.h"
#include "RRECToolBar.h"
#include "resource.h"
#include "ids.h"

#include "..\shared\GraphicsMisc.h"
#include "..\shared\Themed.h"
#include "..\shared\enstring.h"
#include "..\shared\colourpopupEx.h"
#include "..\shared\Localizer.h"


#include <tchar.h>
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern UINT urm_SETCURRENTFONTNAME;
extern UINT urm_SETCURRENTFONTSIZE;
extern UINT urm_SETCURRENTFONTCOLOR;

/////////////////////////////////////////////////////////////////////////////

const int COMBO_VOFFSET			= (GraphicsMisc::ScaleByDPIFactor(2) - 1);
const int FONT_COMBO_WIDTH		= GraphicsMisc::ScaleByDPIFactor(128);
const int SIZE_COMBO_WIDTH		= GraphicsMisc::ScaleByDPIFactor(48);
const int COMBO_HEIGHT			= GraphicsMisc::ScaleByDPIFactor(256);
const int COMBO_WIDTH			= GraphicsMisc::ScaleByDPIFactor(40);
const int MIN_FONT_COMBO_WIDTH	= GraphicsMisc::ScaleByDPIFactor(60);
const int IMAGE_WIDTH			= GraphicsMisc::ScaleByDPIFactor(16);

/////////////////////////////////////////////////////////////////////////////
// CRRECToolBar

CRRECToolBar::CRRECToolBar() : 
	m_crBack(GetSysColor(COLOR_WINDOW)), 
	m_crText(GetSysColor(COLOR_WINDOWTEXT))
{
}

CRRECToolBar::~CRRECToolBar()
{
}

BEGIN_MESSAGE_MAP(CRRECToolBar, CEnToolBar)
	//{{AFX_MSG_MAP(CRRECToolBar)
	ON_CBN_SELCHANGE(DROPDOWN_FONT, OnSelchangeFont)
	ON_CBN_SELCHANGE(DROPDOWN_SIZE, OnSelchangeSize)
	ON_CBN_KILLFOCUS(DROPDOWN_FONT, OnKillFocusCombo)
	ON_CBN_KILLFOCUS(DROPDOWN_SIZE, OnKillFocusCombo)
	ON_MESSAGE(CPN_SELENDOK, OnColorButton)
	ON_WM_ENABLE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_NOTIFY_REFLECT(TBN_DROPDOWN, OnColorDropDown)
END_MESSAGE_MAP()


BOOL CRRECToolBar::Create(CWnd* parent, LPCTSTR szImageFile, COLORREF crMask)
{
	if (CreateEx(parent, TBSTYLE_FLAT | TBSTYLE_WRAPABLE, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_SIZE_DYNAMIC/* | WS_CLIPCHILDREN*/, 
				CRect(0, 0, 0, 0), TOOLBAR_CONTROL) && LoadToolBar(TOOLBAR_CONTROL))
	{
		if (szImageFile)
			SetImage(szImageFile, crMask);
		else 
			SetImage(IDB_TOOLBAR, RGB(255, 0, 255));

		UINT nStyle = GetBarStyle();
		nStyle |= (CBRS_TOOLTIPS | CBRS_FLYBY);
		SetBarStyle(nStyle);

		SetOwner(parent);
		GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

		// colour drop buttons
		int nIndex = CommandToIndex(ID_EDIT_BACKCOLOR);
		SetButtonStyle(nIndex, GetButtonStyle(nIndex) | TBSTYLE_DROPDOWN);

		nIndex = CommandToIndex(ID_EDIT_TEXTCOLOR);
		SetButtonStyle(nIndex, GetButtonStyle(nIndex) | TBSTYLE_DROPDOWN);

		/////////////////////////////////////
		// Map in combo boxes

		TBBUTTON buttons[] = 
		{
			{ 0, FONT_NAME_ID, 0, TBSTYLE_SEP, 0, NULL },
			{ 0, -1, 0, TBSTYLE_SEP, 0, NULL },
			{ 0, FONT_SIZE_ID, 0, TBSTYLE_SEP, 0, NULL },
			{ 0, -1, 0, TBSTYLE_SEP, 0, NULL },
		};
		const int BTNCOUNT = sizeof(buttons) / sizeof(TBBUTTON);

		for (int nBtn = 0; nBtn < BTNCOUNT; nBtn++)
			GetToolBarCtrl().InsertButton(nBtn, &buttons[nBtn]);

		CRect rect;

		TBBUTTONINFO tbi;
		tbi.cbSize = sizeof(TBBUTTONINFO);
		tbi.cx = (WORD)FONT_COMBO_WIDTH;
		tbi.dwMask = TBIF_SIZE;  // By index

		// The font name combo
		GetToolBarCtrl().SetButtonInfo(FONT_NAME_ID, &tbi);
		GetItemRect(FONT_NAME_POS, &rect);

		rect.left++;
		rect.top += COMBO_VOFFSET;		
		rect.bottom += COMBO_HEIGHT;

		if (!m_font.Create(WS_CHILD | WS_VSCROLL |	WS_VISIBLE | CBS_AUTOHSCROLL | 
							CBS_DROPDOWNLIST | CBS_SORT, rect, this, DROPDOWN_FONT))
			return FALSE;

		CLocalizer::EnableTranslation(m_font, FALSE);
		m_font.SetFont(CFont::FromHandle((HFONT) ::GetStockObject(DEFAULT_GUI_FONT)));
		m_font.FillCombo();
		m_font.SetMaxWidth(FONT_COMBO_WIDTH);
		
		// The font size combo
		tbi.cx = (WORD)COMBO_WIDTH;
		GetToolBarCtrl().SetButtonInfo(FONT_SIZE_ID, &tbi);
		GetItemRect(FONT_SIZE_POS, &rect);

		rect.top += COMBO_VOFFSET;		
		rect.bottom += COMBO_HEIGHT;
		
		if (!m_size.Create(WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST | 
							CBS_HASSTRINGS, rect, this, DROPDOWN_SIZE))
			return FALSE;

		m_size.SetFont(CFont::FromHandle((HFONT) ::GetStockObject(DEFAULT_GUI_FONT)));
		m_size.FillCombo();

		return TRUE;
	}
	
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CRRECToolBar message handlers


LRESULT CRRECToolBar::OnSetFont(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// eat this
	return 0L;
}

BOOL CRRECToolBar::GetDroppedState() const
{
	return m_font.GetDroppedState() ||
			m_size.GetDroppedState();
}

void CRRECToolBar::OnSelchangeFont() 
{
	CString font = GetFontName();

	if (!font.IsEmpty())
		GetParent()->SendMessage(urm_SETCURRENTFONTNAME, (WPARAM) (LPCTSTR) font, 0);
}

void CRRECToolBar::OnSelchangeSize() 
{
	GetParent()->SendMessage(urm_SETCURRENTFONTSIZE, 0, (LPARAM) GetFontSize());
}

void CRRECToolBar::OnKillFocusCombo() 
{
	// send a kill foxus message to our parent
	NMHDR nmhdr = { *this, (UINT)GetDlgCtrlID(), NM_KILLFOCUS };

	GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmhdr);
}

LRESULT CRRECToolBar::OnColorButton(WPARAM color, LPARAM nCtrlID) 
{
	BOOL bForeground = (nCtrlID == ID_EDIT_TEXTCOLOR);

	if (bForeground)
		m_crText = color;
	else
		m_crBack = color;

	Invalidate();
	UpdateWindow();

	GetParent()->SendMessage(urm_SETCURRENTFONTCOLOR, bForeground, (LPARAM) color);
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CRRECToolBar UI updaters

BOOL CRRECToolBar::SetFontName(const CString& font)
{
	if(m_font.m_hWnd)
		return m_font.SelectFontName(font);

	return FALSE;
}

BOOL CRRECToolBar::SetFontSize(int size)
{
	if(m_size.m_hWnd)
		return m_size.SelectSize(size);

	return FALSE;
}

CString CRRECToolBar::GetFontName() const
{
	CString font;
	int index = m_font.GetCurSel();

	if(index != CB_ERR)
		m_font.GetLBText(index, font);

	return font;
}	

int CRRECToolBar::GetFontSize() const
{
	if(m_size.m_hWnd)
	{
		int index = m_size.GetCurSel();

		if(index != CB_ERR)
		{
			CString sz;
			m_size.GetLBText(index, sz);
			return _ttoi(sz);
		}
	}

	// else
	return 11;
}

BOOL CRRECToolBar::IncrementFontSize(int nBy)
{
	if(m_size.m_hWnd)
		return m_size.IncrementSize(nBy);

	return FALSE;
}

void CRRECToolBar::SetFontColor(COLORREF color, BOOL bForeground)
{
	if (bForeground)
	{
		if (m_crText == color)
			return;

		m_crText = color;
	}
	else
	{
		if (m_crBack == color)
			return;

		m_crBack = color;
	}

	if (GetSafeHwnd())
		Invalidate();
}

COLORREF CRRECToolBar::GetFontColor(BOOL bForeground, BOOL bTranslateDefault) const 
{ 
	if (bForeground)
	{
		if (m_crText == CLR_DEFAULT && bTranslateDefault)
			return GetSysColor(COLOR_WINDOWTEXT);
		
		// else
		return m_crText;
	}
	else // background
	{
		if (m_crBack == CLR_DEFAULT && bTranslateDefault)
			return GetSysColor(COLOR_WINDOW);
		
		// else
		return m_crBack;
	}
}

BOOL CRRECToolBar::SetButtonState(int nID, UINT nState)
{
	if (!IsWindowEnabled())
		nState &= ~TBSTATE_ENABLED;
	else
		nState |= TBSTATE_ENABLED;

	return GetToolBarCtrl().SetState(nID, nState);
}

void CRRECToolBar::OnEnable(BOOL bEnable) 
{
	CEnToolBar::OnEnable(bEnable);
	
	m_font.EnableWindow(bEnable);
	m_size.EnableWindow(bEnable);
}

BOOL CRRECToolBar::IsButtonChecked(int nID) const
{
	return (GetToolBarCtrl().GetState(nID) & TBSTATE_CHECKED);
}

BOOL CRRECToolBar::CheckButton(int nID, BOOL bChecked)
{
	UINT nState = GetToolBarCtrl().GetState(nID);

	if (!bChecked)
		nState &= ~TBSTATE_CHECKED;
	else
		nState |= TBSTATE_CHECKED;

	return SetButtonState(nID, nState);
}

void CRRECToolBar::OnSize(UINT nType, int cx, int cy) 
{
	CEnToolBar::OnSize(nType, cx, cy);
	
	if (m_font.GetSafeHwnd())
	{
		CRect rDiv;

		// calc length after which we want to start shortening ctrls
		GetItemRect(1, &rDiv); // divider
		const int DEFCTRLSWIDTH = FONT_COMBO_WIDTH + SIZE_COMBO_WIDTH + 
									(5 * rDiv.Width()) / 2; // 2.5 separators

		// if the toolbar length is less than the default
		// width of the embedded widgets we reduce the 
		// font combo as far as we can.
		TBBUTTONINFO tbi;
		tbi.cbSize = sizeof(TBBUTTONINFO);
		tbi.dwMask = TBIF_SIZE;  // By index
		
 		CRect rect;
		GetItemRect(FONT_NAME_POS, &rect);

		int nNewWidth = FONT_COMBO_WIDTH - max(0, DEFCTRLSWIDTH - cx);
		nNewWidth = max(nNewWidth, MIN_FONT_COMBO_WIDTH);

		if (nNewWidth != rect.Width())
		{
			rect.right = rect.left + nNewWidth;
			rect.top += COMBO_VOFFSET;
			rect.bottom += COMBO_HEIGHT;
			m_font.MoveWindow(rect);
			
			// update toolbar item size also
			tbi.cx = (WORD)rect.Width();
			GetToolBarCtrl().SetButtonInfo(FONT_NAME_ID, &tbi);
			
			// move the other two items to suit their toolbar rects
			GetItemRect(FONT_SIZE_POS, &rect);
			rect.top += COMBO_VOFFSET;
			rect.bottom += COMBO_HEIGHT;
			m_size.MoveWindow(rect);
		}
	}
}

void CRRECToolBar::OnColorDropDown(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTOOLBAR pNMTB = (LPNMTOOLBAR)pNMHDR;
	UINT nBtnID = pNMTB->iItem;

	switch (nBtnID)
	{
	case ID_EDIT_BACKCOLOR:
	case ID_EDIT_TEXTCOLOR:
		{
			CRect rButton;
			GetItemRect(CommandToIndex(pNMTB->iItem), rButton);
			ClientToScreen(rButton);

			COLORREF color = GetFontColor(nBtnID == ID_EDIT_TEXTCOLOR);
			CEnString defaultText(STRING_DEFAULT), customText(STRING_CUSTOM);

			new CColourPopupEx(CPoint(rButton.left, rButton.bottom), color, 
							 this, nBtnID, defaultText, customText);
		}
		break;
	}

	*pResult = 0;
}

LRESULT CRRECToolBar::OnButtonPostPaint(LPNMTBCUSTOMDRAW lpNMCustomDraw)
{
	UINT nBtnID = lpNMCustomDraw->nmcd.dwItemSpec;

	switch (nBtnID)
	{
	case ID_EDIT_BACKCOLOR:
	case ID_EDIT_TEXTCOLOR:
		{
			// paint the lowest 3 lines with the appropriate colour
			// but only as far as the beginning of the drop button
			CRect rColor(lpNMCustomDraw->nmcd.rc);

			if (CThemed().AreControlsThemed())
				rColor.bottom++;
			else
				rColor.DeflateRect(2, 2);

			rColor.top = rColor.bottom - GraphicsMisc::ScaleByDPIFactor(3);
			rColor.right = rColor.left + IMAGE_WIDTH;

			CDC* pDC = CDC::FromHandle(lpNMCustomDraw->nmcd.hdc);
			COLORREF color = GetFontColor(nBtnID == ID_EDIT_TEXTCOLOR, TRUE);

			pDC->FillSolidRect(rColor, color);
		}
		break;
	}

	return CDRF_DODEFAULT;
}
