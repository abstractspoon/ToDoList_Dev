/* ==========================================================================
	File :			RuleRichEditCtrl.cpp

	Class :			CRulerRichEditCtrl

	Author :		Johan Rosengren, Abstrakt Mekanik AB
					Iain Clarke

	Date :			2004-04-17

	Purpose :		"CRulerRichEditCtrl" is a "CWnd" derived class containing an 
					embedded RTF-control, a ruler-control with dragable tab-
					positions and a formatting toolbar. The class can be used 
					to - for example - add a complete mini-editor to a modal 
					or modeless dialog box. 

	Description :	The class mainly handles mouse messages. The mouse
					messages are sent from the ruler control, and are 
					button down, where the a check is made for the cursor 
					located on one of the tab-markers, mouse move, where an 
					XORed line is drawn across the RTF-control and button up, 
					where a new tab position is set. The class also handles 
					the toolbar buttons, setting styles as 
					appropriate for the selected text.

	Usage :			Add a "CRulerRichEditCtrl"-member to the parent class. 
					Call Create to create the control. "GetRichEditCtrl" can 
					be used to access the embedded RTF-control. Remember to 
					call "AfxInitRichEdit(2)"!

					The contents can be saved to disk by calling "Save", and 
					loaded from disk by calling "Load". The two functions 
					will automatically display a file dialog if the file 
					name parameter of the calls are left empty.

					"GetRTF" and "SetRTF" can be used to get and set the 
					contents of the embedded RTF-control as RTF 
					respectively.

					The ruler measures can be displayed as inches or 
					centimeters, by calling "SetMode". "GetMode" will get the 
					current mode.

   ========================================================================*/

#include "stdafx.h"
#include "RulerRichEditCtrl.h"
#include "resource.h"
#include "inserttabledlg.h"
#include "ids.h"

#include "..\shared\graphicsmisc.h"
#include "..\shared\enstring.h"
#include "..\shared\holdredraw.h"
#include "..\shared\filemisc.h"
#include "..\shared\misc.h"

#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef WM_THEMECHANGED
#	define WM_THEMECHANGED 0x031A
#endif

/////////////////////////////////////////////////////////////////////////////

const int SCMARGIN = 4;
const LPCTSTR FILEPREFIX = _T("file://");
const int DEFTABSTOP = 320;
const WORD PFN_NUMBERLIST = 0x0002;

/////////////////////////////////////////////////////////////////////////////
// Registered messages for ruler/CRulerRichEditCtrl communication

UINT urm_RULERACTION = ::RegisterWindowMessage(_T("_RULERRICHEDITCTRL_RULER_TRACK_"));
UINT urm_GETSCROLLPOS = ::RegisterWindowMessage(_T("_RULERRICHEDITCTRL_GET_SCROLL_POS_"));
UINT urm_SETCURRENTFONTNAME = ::RegisterWindowMessage(_T("_RULERRICHEDITCTRL_SET_CURRENT_FONT_NAME"));
UINT urm_SETCURRENTFONTSIZE = ::RegisterWindowMessage(_T("_RULERRICHEDITCTRL_SET_CURRENT_FONT_SIZE"));
UINT urm_SETCURRENTFONTCOLOR = ::RegisterWindowMessage(_T("_RULERRICHEDITCTRL_SET_CURRENT_FONT_COLOR"));

/////////////////////////////////////////////////////////////////////////////
// CRulerRichEditCtrl

CRulerRichEditCtrl::CRulerRichEditCtrl() : m_pen(PS_DOT, 0, RGB(0, 0, 0))
{
	m_rulerPosition = 0;
	m_margin = 0;
	m_movingtab = -1;
	m_offset = 0;
	m_readOnly = FALSE;
	m_bWordWrap = -1;
	m_crSel.cpMax = m_crSel.cpMin = -1;

	ShowToolbar();
	ShowRuler();

	m_cfDefault.dwMask = CFM_SIZE | CFM_FACE | CFM_BOLD | CFM_ITALIC | 
						 CFM_UNDERLINE | CFM_STRIKEOUT | CFM_COLOR | CFM_LINK;
	m_cfDefault.yHeight = 200;
	m_cfDefault.dwEffects = CFE_AUTOBACKCOLOR | CFE_AUTOCOLOR;
	lstrcpy(m_cfDefault.szFaceName, _T("Times New Roman"));
}

CRulerRichEditCtrl::~CRulerRichEditCtrl()
{
	m_pen.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CRulerRichEditCtrl, CWnd)
	//{{AFX_MSG_MAP(CRulerRichEditCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_MESSAGE(WM_GETTEXT, OnGetText )
	ON_MESSAGE(WM_GETTEXTLENGTH, OnGetTextLength)
	ON_WM_ENABLE()
	ON_REGISTERED_MESSAGE(urm_RULERACTION, OnTrackRuler)
	ON_REGISTERED_MESSAGE(urm_GETSCROLLPOS, OnGetScrollPos)
	ON_REGISTERED_MESSAGE(urm_SETCURRENTFONTNAME, OnSetCurrentFontName)
	ON_REGISTERED_MESSAGE(urm_SETCURRENTFONTSIZE, OnSetCurrentFontSize)
	ON_REGISTERED_MESSAGE(urm_SETCURRENTFONTCOLOR, OnSetCurrentFontColor)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_EDIT_BACKCOLOR, OnEditBackColor)
	ON_COMMAND(ID_EDIT_BOLD, OnEditBold)
	ON_COMMAND(ID_EDIT_BULLET, OnEditBulletList)
	ON_COMMAND(ID_EDIT_CENTERALIGN, OnEditCenterAlign)
	ON_COMMAND(ID_EDIT_COLOR, OnEditColor)
	ON_COMMAND(ID_EDIT_FONT, OnEditFont)
	ON_COMMAND(ID_EDIT_GROWFONT, OnEditGrowFont)
	ON_COMMAND(ID_EDIT_INDENT, OnEditIndent)
	ON_COMMAND(ID_EDIT_INSERTTABLE, OnEditInsertTable)
	ON_COMMAND(ID_EDIT_ITALIC, OnEditItalic)
	ON_COMMAND(ID_EDIT_JUSTIFY, OnEditJustify)
	ON_COMMAND(ID_EDIT_LEFTALIGN, OnEditLeftAlign)
	ON_COMMAND(ID_EDIT_NUMBER, OnEditNumberList)
	ON_COMMAND(ID_EDIT_OUTDENT, OnEditOutdent)
	ON_COMMAND(ID_EDIT_RIGHTALIGN, OnEditRightAlign)
	ON_COMMAND(ID_EDIT_SHRINKFONT, OnEditShrinkFont)
	ON_COMMAND(ID_EDIT_STRIKETHRU, OnEditStrikethrough)
	ON_COMMAND(ID_EDIT_TEXTCOLOR, OnEditTextColor)
	ON_COMMAND(ID_EDIT_UNDERLINE, OnEditUnderline)
	ON_COMMAND(ID_EDIT_WORDWRAP, OnEditWordwrap)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BACKCOLOR, OnUpdateEditBackColor)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BOLD, OnUpdateEditBold)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BULLET, OnUpdateEditBullet)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CENTERALIGN, OnUpdateEditCenterAlign)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FONT, OnUpdateEditFont)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GROWFONT, OnUpdateEditGrowFont)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INDENT, OnUpdateEditIndent)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSERTTABLE, OnUpdateEditInsertTable)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ITALIC, OnUpdateEditItalic)
	ON_UPDATE_COMMAND_UI(ID_EDIT_JUSTIFY, OnUpdateEditJustify)
	ON_UPDATE_COMMAND_UI(ID_EDIT_LEFTALIGN, OnUpdateEditLeftAlign)
	ON_UPDATE_COMMAND_UI(ID_EDIT_NUMBER, OnUpdateEditNumberList)
	ON_UPDATE_COMMAND_UI(ID_EDIT_OUTDENT, OnUpdateEditOutdent)
	ON_UPDATE_COMMAND_UI(ID_EDIT_RIGHTALIGN, OnUpdateEditRightAlign)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SHRINKFONT, OnUpdateEditShrinkFont)
	ON_UPDATE_COMMAND_UI(ID_EDIT_STRIKETHRU, OnUpdateEditStrikethru)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TEXTCOLOR, OnUpdateEditTextColor)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDERLINE, OnUpdateEditUnderline)
	ON_UPDATE_COMMAND_UI(ID_EDIT_WORDWRAP, OnUpdateEditWordwrap)

	ON_WM_CREATE()
	ON_MESSAGE(WM_THEMECHANGED, OnThemeChanged)
	ON_NOTIFY(NM_KILLFOCUS, TOOLBAR_CONTROL, OnKillFocusToolbar)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_EN_HSCROLL(RTF_CONTROL, OnEnHScroll)
	ON_NOTIFY(EN_SELCHANGE, RTF_CONTROL, OnEnSelChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CRulerRichEditCtrl::Create(DWORD dwStyle, const RECT &rect, CWnd* pParentWnd, UINT nID, BOOL /*autohscroll*/)
{
	BOOL result = CWnd::Create(NULL, _T(""), dwStyle, rect, pParentWnd, nID);

	return result;
}

BOOL CRulerRichEditCtrl::CreateToolbar()
{
	return m_toolbar.Create(this);
}

LRESULT CRulerRichEditCtrl::OnThemeChanged(WPARAM /*wp*/, LPARAM /*lp*/)
{
	m_toolbar.DestroyWindow();
	m_toolbar.Create(this);
	UpdateToolbarButtons();

	CRect rClient;
	GetClientRect(rClient);
	LayoutControls(rClient.Width(), rClient.Height());
	
	m_rtf.Invalidate(TRUE);

	return 0L;
}

BOOL CRulerRichEditCtrl::CreateRuler()
{
	CRect rect;
	GetClientRect(rect);

	CRect rulerRect(0, TOOLBAR_HEIGHT, rect.right, TOOLBAR_HEIGHT + RULER_HEIGHT);
	return m_ruler.Create(rulerRect, this, RULER_CONTROL);
}

void CRulerRichEditCtrl::BuildTabStops(ParaFormat& para)
{
	para.dwMask = PFM_TABSTOPS;
	para.cTabCount = MAX_TAB_STOPS;

	for(int t = 0; t < MAX_TAB_STOPS ; t++)
		para.rgxTabs[ t ] = DEFTABSTOP * (t + 1);
}

BOOL CRulerRichEditCtrl::CreateRTFControl(BOOL autohscroll)
{
	BOOL result = FALSE;

	CRect rect;
	GetClientRect(rect);

	int top = TOOLBAR_HEIGHT + RULER_HEIGHT;
	CRect rtfRect(0, top, rect.right, rect.bottom);
	DWORD style = ES_NOHIDESEL | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_WANTRETURN | ES_MULTILINE | WS_TABSTOP;

	if (autohscroll)
		style |= ES_AUTOHSCROLL;

	if (m_rtf.Create(style, rtfRect, this, RTF_CONTROL))
	{
		// Setting up default tab stops
		ParaFormat para(PFM_TABSTOPS);
		BuildTabStops(para);

		m_rtf.SetParaFormat(para);

		// Setting default character format
		m_rtf.SendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&m_cfDefault);

		// Set the internal tabs array
		SetTabStops((LPLONG) (para.rgxTabs), MAX_TAB_STOPS);

		m_rtf.SendMessage(EM_SETTYPOGRAPHYOPTIONS, TO_ADVANCEDTYPOGRAPHY, TO_ADVANCEDTYPOGRAPHY);
		m_rtf.SetEventMask(m_rtf.GetEventMask() | ENM_SELCHANGE | ENM_SCROLL | ENM_CHANGE);
		m_rtf.ModifyStyleEx(0, WS_EX_CLIENTEDGE);

		result = TRUE;
	}

	return result;
}

void CRulerRichEditCtrl::SetRTF(const CString& rtf)
{
	m_rtf.SetRTF(rtf);
	
	// reset the formatting if the new content is empty else
	// the previous formatting hangs around
	if (rtf.IsEmpty())
	{
		CHoldRedraw hr(m_rtf);
		CharFormat cf(m_cfDefault);
		
		m_rtf.SetSel(0, -1);
		m_rtf.SendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
		m_rtf.SetSel(-1, 0);
	}
	UpdateEditRect();

	m_rtf.ParseAndFormatText(TRUE);
}

void CRulerRichEditCtrl::SetSelectedWebLink(const CString& sWebLink)
{
	CHARRANGE cr;
	m_rtf.GetSel(cr);

	CString sText = m_rtf.GetTextRange(cr);

	if (!sText.IsEmpty())
	{
		CEnString sUrl(_T("HYPERLINK\"%s\"%s"), sWebLink, sText);
		m_rtf.ReplaceSel(sUrl, TRUE);

		cr.cpMax = cr.cpMin + sUrl.GetLength();
		m_rtf.SetSel(cr);
		SetEffect(CFM_PROTECTED | CFM_LINK | CFM_HIDDEN, CFE_PROTECTED | CFE_LINK);

		// hide all but the text
		cr.cpMax -= sText.GetLength();
		m_rtf.SetSel(cr);
		SetEffect(CFM_PROTECTED | CFM_LINK | CFM_HIDDEN, CFE_PROTECTED | CFE_LINK | CFE_HIDDEN);
	}
}

void CRulerRichEditCtrl::UpdateEditRect()
{
	// Set up edit rect margins
	CRect rc;
	m_rtf.GetClientRect(rc);

	rc.top = SCMARGIN;
	rc.left = SCMARGIN * 2;
	rc.right -= SCMARGIN * 2;

	m_rtf.SetRect(rc);
}

void CRulerRichEditCtrl::CreateMargins()
{
	UpdateEditRect();

	// Get the diff between the window- and client 
	// rect of the RTF-control. This gives the actual 
	// size of the RTF-control border.
	CRect	r1;
	CRect	r2;

	m_rtf.GetWindowRect(r1);
	m_rtf.GetClientRect(r2);
	m_rtf.ClientToScreen(r2);

	// Create the margin for the toolbar 
	// controls and the ruler.
	m_margin = SCMARGIN * 2 + r2.left - r1.left;
	m_ruler.SetMargin(m_margin);
}

/////////////////////////////////////////////////////////////////////////////
// CRulerRichEditCtrl message handlers

int CRulerRichEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Save screen resolution for later on.
	CClientDC dc(this);
	m_physicalInch = dc.GetDeviceCaps(LOGPIXELSX);

	// Create sub-controls
	BOOL autohscroll = TRUE;

	if (CreateRTFControl(autohscroll) && CreateToolbar() && CreateRuler())
	{
		CreateMargins();
		SetReadOnly(GetReadOnly()); 
		UpdateToolbarButtons();

		CRect rClient;
		GetClientRect(rClient);

		LayoutControls(rClient.Width(), rClient.Height());

		return 0;
	}

	return -1;
}

void CRulerRichEditCtrl::OnEnHScroll()
{
	m_ruler.Invalidate(FALSE);
	m_ruler.UpdateWindow();
}

void CRulerRichEditCtrl::OnEnSelChange(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	CHARRANGE crSel;
	m_rtf.GetSel(crSel);

	if (m_crSel.cpMin != crSel.cpMin || m_crSel.cpMax != crSel.cpMax)
	{
		// Update the toolbar
		UpdateToolbarButtons();
		
		// Update ruler
		UpdateTabStops();
		m_ruler.Invalidate(FALSE);
		m_ruler.UpdateWindow();

		m_crSel = crSel;
	}

	*pResult = 0;
}

LRESULT CRulerRichEditCtrl::OnSetFont(WPARAM wp, LPARAM /*lp*/)
{
	// reverse engineer the hFont and use the results
	// for the rtf defaults
	CString sFace;
	int nPoint = GraphicsMisc::GetFontNameAndPointSize((HFONT)wp, sFace);

	if (nPoint && !sFace.IsEmpty())
	{
		// lets have a backup plan
		if (!m_toolbar.SetFontName(sFace))
		{
			sFace = "MS Sans Serif";
			nPoint = 9;
			m_toolbar.SetFontName(sFace);
		}

		m_toolbar.SetFontSize(nPoint);

		// update default char format
		lstrcpy(m_cfDefault.szFaceName, sFace);
		m_cfDefault.yHeight = nPoint * 20;
		
		m_rtf.SendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&m_cfDefault);
	}
	// else eat it

	return 0L;
}

BOOL CRulerRichEditCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

BOOL CRulerRichEditCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	return CWnd::OnNotify(wParam, lParam, pResult);
}

void CRulerRichEditCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (m_rtf.m_hWnd)
	{
		UpdateEditRect();
		LayoutControls(cx, cy);
	}
}

void CRulerRichEditCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	m_rtf.SetFocus();
}

LRESULT CRulerRichEditCtrl::OnGetScrollPos(WPARAM, LPARAM)
{
	return m_rtf.GetScrollPos(SB_HORZ);
}

LRESULT CRulerRichEditCtrl::OnTrackRuler(WPARAM mode, LPARAM pt)
{
	CPoint* point = (CPoint*) pt;
	int toolbarHeight = 0;
	if (m_showToolbar)
		toolbarHeight = TOOLBAR_HEIGHT;

	switch(mode)
	{
		case DOWN:
			// The left mouse button is clicked
			{
				// Check if we clicked on a tab-marker.
				int pos = m_rtf.GetScrollPos(SB_HORZ);
				m_movingtab = -1;
				CRect hitRect;
				int y = RULER_HEIGHT - 9;
				for(int t = 0 ; t < MAX_TAB_STOPS ; t++)
				{
					int x = m_tabs[ t ] + m_margin - pos;
					hitRect.SetRect(x - 2, y - 1, x + 3, y + 3);
					if (hitRect.PtInRect(*point))
					{
						// Yes, we did.
						m_movingtab = t;

						// Calc offset, as the hit area is wider than
						// the 1 pixel tab line
						m_offset = point->x - (m_tabs[ t ] + m_margin - pos);
					}
				}

				if (m_movingtab != -1)
				{

					// We did click in a tab marker.
					// Start dragging

					// Find initial ruler position
					m_rulerPosition = point->x - m_offset;
					CRect rect;
					GetClientRect(rect);

					// Draw a new ruler line
					CClientDC dc(this);
					dc.SelectObject(&m_pen);
					dc.SetROP2(R2_XORPEN);

					dc.MoveTo(m_rulerPosition, toolbarHeight + 3);
					dc.LineTo(m_rulerPosition, rect.Height());

					dc.SelectStockObject(BLACK_PEN);

				}
			}
			break;

		case MOVE:
			// The mouse is moved
			if (m_movingtab != -1)
			{
				CRect rect;
				GetClientRect(rect);

				CClientDC dc(this);
				
				// Erase previous line
				dc.SelectObject(&m_pen);
				dc.SetROP2(R2_XORPEN);
				
				dc.MoveTo(m_rulerPosition, toolbarHeight + 3);
				dc.LineTo(m_rulerPosition, rect.Height());
				
				// Set up new line
				// Calc min and max. We can not place this marker 
				// before the previous or after the next. Neither 
				// can we move the marker outside the ruler.
				int pos = m_rtf.GetScrollPos(SB_HORZ);
				int min = m_margin + m_offset;
				if (m_movingtab > 0)
					min = (m_tabs[ m_movingtab - 1 ] + m_margin - pos) + 3 + m_offset;
				
				int max = rect.Width() - 5 + m_offset;
				if (m_movingtab < m_tabs.GetUpperBound())
					max = (m_tabs[ m_movingtab + 1 ] + m_margin - pos) - 3 + m_offset;
				max = min(max, rect.Width() - 5 + m_offset);
				
				// Set new positions
				m_rulerPosition = max(min, point->x - m_offset);
				m_rulerPosition = min(m_rulerPosition, max);
				
				// Draw the new line
				dc.MoveTo(m_rulerPosition, toolbarHeight + 3);
				dc.LineTo(m_rulerPosition, rect.Height());
				
				dc.SelectStockObject(BLACK_PEN);
				
			}
			break;

		case UP:
			// The mouse button is released
			if (m_movingtab != -1)
			{
				// Erase previous line
				CRect rect, ruler;
				GetClientRect(rect);

				CClientDC dc(this);
				dc.SelectObject(&m_pen);
				dc.SetROP2(R2_XORPEN);
				
				dc.MoveTo(m_rulerPosition, toolbarHeight + 3);
				dc.LineTo(m_rulerPosition, rect.Height());

				// if the drop is ouside the ruler then delete the tab
				m_ruler.GetClientRect(ruler);
				ruler.InflateRect(0, 10);
				
				//if (ruler.PtInRect(point))



				// Set new value for tab position
				int pos = m_rtf.GetScrollPos(SB_HORZ);
				m_tabs[ m_movingtab ] = m_rulerPosition - m_margin + pos - m_offset;

				// Get the current tabstops, as we
				// must set all tabs in one operation
				ParaFormat para(PFM_TABSTOPS);
				para.cTabCount = MAX_TAB_STOPS;
				m_rtf.GetParaFormat(para);

				// Convert current position to twips
				double twip = (double)m_physicalInch / 1440;
				int tabpos = m_tabs[ m_movingtab ];
				tabpos = (int) ((double) tabpos / twip +.5);
				para.rgxTabs[ m_movingtab ] = tabpos; 

				// Set tabs to control
				m_rtf.SetParaFormat(para);

				// Update the ruler
				m_ruler.SetTabStops(m_tabs);

				m_movingtab = -1;
				m_rtf.SetFocus();
			}
			break;
	}

	return 0;

}

LRESULT CRulerRichEditCtrl::OnSetText(WPARAM wParam, LPARAM lParam)
{
	return m_rtf.SendMessage(WM_SETTEXT, wParam, lParam);
}

LRESULT CRulerRichEditCtrl::OnGetText(WPARAM wParam, LPARAM lParam)
{
	return m_rtf.SendMessage(WM_GETTEXT, wParam, lParam);
}

LRESULT CRulerRichEditCtrl::OnGetTextLength(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return m_rtf.GetTextLength();
}

/////////////////////////////////////////////////////////////////////////////
// CRulerRichEditCtrl public implementation

void CRulerRichEditCtrl::SetMode(int mode)
{
	m_ruler.SetMode(mode);
}

int CRulerRichEditCtrl::GetMode() const
{
	return m_ruler.GetMode();
}

/////////////////////////////////////////////////////////////////////////////
// CRulerRichEditCtrl toolbar button handlers

void CRulerRichEditCtrl::OnEditFont() 
{
	DoFont();
}

void CRulerRichEditCtrl::OnEditColor() 
{
	DoColor();
}

void CRulerRichEditCtrl::OnEditBold() 
{
	DoBold();
}

void CRulerRichEditCtrl::OnEditStrikethrough() 
{
	DoStrikethrough();
}

void CRulerRichEditCtrl::OnEditItalic() 
{
	DoItalic();
}

void CRulerRichEditCtrl::OnEditUnderline() 
{
	DoUnderline();
}

void CRulerRichEditCtrl::OnEditLeftAlign() 
{
	DoLeftAlign();
}

void CRulerRichEditCtrl::OnEditJustify() 
{
	DoJustify();
}

void CRulerRichEditCtrl::OnEditShrinkFont() 
{
	IncrementFontSize(FALSE);
}

void CRulerRichEditCtrl::OnEditGrowFont() 
{
	IncrementFontSize(TRUE);
}

void CRulerRichEditCtrl::OnEditInsertTable() 
{
	DoInsertTable();
}

void CRulerRichEditCtrl::DoInsertHorzLine()
{
	m_rtf.InsertHorizontalLine();
}

void CRulerRichEditCtrl::OnEditCenterAlign() 
{
	DoCenterAlign();
}

void CRulerRichEditCtrl::OnEditRightAlign() 
{
	DoRightAlign();
}

void CRulerRichEditCtrl::OnEditIndent() 
{
	DoIndent();
}

void CRulerRichEditCtrl::OnEditOutdent() 
{
	DoOutdent();
}

void CRulerRichEditCtrl::OnEditBulletList() 
{
	DoBulletList();
}

void CRulerRichEditCtrl::OnEditNumberList() 
{
	DoNumberList();
}

LRESULT CRulerRichEditCtrl::OnSetCurrentFontName(WPARAM font, LPARAM)
{
	SetCurrentFontName((LPCTSTR) font);

	return 0;
}

LRESULT CRulerRichEditCtrl::OnSetCurrentFontSize(WPARAM, LPARAM size)
{
	SetCurrentFontSize(size);
	return 0;
}

void CRulerRichEditCtrl::OnKillFocusToolbar(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	// pretend the richedit lost focus to force an update
	SendMessage(WM_COMMAND, MAKEWPARAM(RTF_CONTROL, EN_KILLFOCUS), (LPARAM)m_rtf.GetSafeHwnd());
	*pResult = 0;
}

LRESULT CRulerRichEditCtrl::OnSetCurrentFontColor(WPARAM bForeground, LPARAM color)
{
	SetCurrentFontColor((COLORREF) color, bForeground);

	m_rtf.SetFocus();
	return 0;
}

void CRulerRichEditCtrl::OnEditTextColor()
{
	SetCurrentFontColor(m_toolbar.GetFontColor(TRUE), TRUE);
}

void CRulerRichEditCtrl::OnEditBackColor()
{
	SetCurrentFontColor(m_toolbar.GetFontColor(FALSE), FALSE);
}

void CRulerRichEditCtrl::OnEditWordwrap()
{
	SetWordWrap(!HasWordWrap());
}

void CRulerRichEditCtrl::OnEnable(BOOL bEnable) 
{
	CWnd::OnEnable(bEnable);

	// update children
	// we don't disable the rtf because this causes the scrollbar
	// to remain disabled even after re-enabling
	SetReadOnly(m_readOnly || !bEnable);
}

void CRulerRichEditCtrl::OnUpdateEditBold(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_rtf.SelectionHasEffect(CFM_BOLD, CFE_BOLD) ? 1 : 0);
	pCmdUI->Enable(m_rtf.CanEdit());
}

void CRulerRichEditCtrl::OnUpdateEditBullet(CCmdUI* pCmdUI) 
{
	ParaFormat pf(PFM_NUMBERING);
	m_rtf.GetParaFormat(pf);
				
	pCmdUI->SetCheck((pf.wNumbering == PFN_BULLET) ? 1 : 0);
	pCmdUI->Enable(m_rtf.CanEdit());
}

void CRulerRichEditCtrl::OnUpdateEditCenterAlign(CCmdUI* pCmdUI) 
{
	ParaFormat pf(PFM_ALIGNMENT);
	m_rtf.GetParaFormat(pf);
				
	pCmdUI->SetCheck((pf.wAlignment == PFA_CENTER) ? 1 : 0);
	pCmdUI->Enable(m_rtf.CanEdit());
}

void CRulerRichEditCtrl::OnUpdateEditGrowFont(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.CanEdit());
}

void CRulerRichEditCtrl::OnUpdateEditIndent(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.CanEdit());
}

void CRulerRichEditCtrl::OnUpdateEditInsertTable(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.CanEdit());
}

void CRulerRichEditCtrl::OnUpdateEditItalic(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_rtf.SelectionHasEffect(CFM_ITALIC, CFE_ITALIC) ? 1 : 0);
	pCmdUI->Enable(m_rtf.CanEdit());
}

void CRulerRichEditCtrl::OnUpdateEditJustify(CCmdUI* pCmdUI) 
{
	ParaFormat pf(PFM_ALIGNMENT);
	m_rtf.GetParaFormat(pf);
				
	pCmdUI->SetCheck((pf.wAlignment == PFA_JUSTIFY) ? 1 : 0);
	pCmdUI->Enable(m_rtf.CanEdit());
}

void CRulerRichEditCtrl::OnUpdateEditLeftAlign(CCmdUI* pCmdUI) 
{
	ParaFormat pf(PFM_ALIGNMENT);
	m_rtf.GetParaFormat(pf);
				
	pCmdUI->SetCheck((pf.wAlignment == PFA_LEFT) ? 1 : 0);
	pCmdUI->Enable(m_rtf.CanEdit());
}

void CRulerRichEditCtrl::OnUpdateEditNumberList(CCmdUI* pCmdUI) 
{
	ParaFormat pf(PFM_NUMBERING);
	m_rtf.GetParaFormat(pf);
				
	pCmdUI->SetCheck((pf.wNumbering > PFN_BULLET) ? 1 : 0);
	pCmdUI->Enable(m_rtf.CanEdit());
}

void CRulerRichEditCtrl::OnUpdateEditOutdent(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.CanEdit());
}

void CRulerRichEditCtrl::OnUpdateEditRightAlign(CCmdUI* pCmdUI) 
{
	ParaFormat pf(PFM_ALIGNMENT);
	m_rtf.GetParaFormat(pf);
				
	pCmdUI->SetCheck((pf.wAlignment == PFA_RIGHT) ? 1 : 0);
	pCmdUI->Enable(m_rtf.CanEdit());
}

void CRulerRichEditCtrl::OnUpdateEditShrinkFont(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.CanEdit());
}

void CRulerRichEditCtrl::OnUpdateEditStrikethru(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_rtf.SelectionHasEffect(CFM_STRIKEOUT, CFE_STRIKEOUT) ? 1 : 0);
	pCmdUI->Enable(m_rtf.CanEdit());
}

void CRulerRichEditCtrl::OnUpdateEditUnderline(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_rtf.SelectionHasEffect(CFM_UNDERLINE, CFE_UNDERLINE) ? 1 : 0);
	pCmdUI->Enable(m_rtf.CanEdit());
}

void CRulerRichEditCtrl::OnUpdateEditWordwrap(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(HasWordWrap() ? 1 : 0);
}

void CRulerRichEditCtrl::OnUpdateEditBackColor(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.CanEdit());

	// NOTE: We no longer change the toolbar colors to
	// match the currently selected text to be more
	// consistent with MS Word
}

void CRulerRichEditCtrl::OnUpdateEditFont(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.CanEdit());
}

void CRulerRichEditCtrl::OnUpdateEditTextColor(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_rtf.CanEdit());

	// NOTE: We no longer change the toolbar colors to
	// match the currently selected text to be more
	// consistent with MS Word
}

/////////////////////////////////////////////////////////////////////////////
// CRulerRichEditCtrl private helpers

void CRulerRichEditCtrl::SetTabStops(LPLONG tabs, int size)
{
	m_tabs.RemoveAll();

	double twip = (double)m_physicalInch / 1440;

	for(int t = 0 ; t < size ; t++)
	{
		// Convert from twips to pixels
		int tabpos = *(tabs + t);
		tabpos = (int) ((double) tabpos * twip +.5);
		m_tabs.Add(tabpos);
	}

	m_ruler.SetTabStops(m_tabs);
}

void CRulerRichEditCtrl::UpdateTabStops()
{
	ParaFormat para(PFM_TABSTOPS);
	m_rtf.GetParaFormat(para);

	FixupTabStops(para);
	SetTabStops((LPLONG)(para.rgxTabs), MAX_TAB_STOPS);
}

void CRulerRichEditCtrl::UpdateToolbarButtons()
{
	if (m_showToolbar && m_toolbar.GetSafeHwnd())
	{
		//TRACE(_T("CRulerRichEditCtrl::UpdateToolbarButtons()\n"));

		CharFormat cf(CFM_FACE | CFM_SIZE);
		m_rtf.GetSelectionCharFormat(cf);

		BOOL bConsistentFont = (cf.dwMask & CFM_FACE);
		m_toolbar.SetFontName(bConsistentFont ? cf.szFaceName : _T(""));

		BOOL bConsistentSize = (cf.dwMask & CFM_SIZE);
		m_toolbar.SetFontSize(bConsistentSize ? cf.yHeight / 20 : -1);

		m_toolbar.RefreshButtonStates(FALSE);
	}
}

void CRulerRichEditCtrl::SetEffect(int mask, int effect)
{
	CharFormat cf;
	cf.dwMask = mask;
	cf.dwEffects = effect;

	m_rtf.SendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
	m_rtf.SetFocus();
}

void CRulerRichEditCtrl::IncrementFontSize(BOOL bIncrement)
{
	if (m_toolbar.IncrementFontSize(bIncrement ? 1 : -1))
		SetCurrentFontSize(m_toolbar.GetFontSize());
}

void CRulerRichEditCtrl::DoInsertTable()
{
	CInsertTableDlg dialog;

	if (dialog.DoModal() == IDOK)
	{
		// calculate column width from first tab
		int nColWidth = 1000;
		ParaFormat para(PFM_TABSTOPS);
		m_rtf.GetParaFormat(para);

		if (para.rgxTabs[0])
			nColWidth = (dialog.GetColumnWidth() * para.rgxTabs[0]);

		m_rtf.InsertTable(dialog.GetNumRows(), 
						dialog.GetNumColumns(),
						nColWidth);
	}
}

/*
void CRulerRichEditCtrl::InsertHorizontalRule()
{
	ParaFormat	para(PFM_BORDER);
	m_rtf.GetParaFormat(para);
	
	para.wBorders = 0x063f;
	para.wBorderWidth = 257;
	para.wBorderSpace = 10000;

	para.dwMask = PFM_BORDER;
	m_rtf.SetParaFormat(para);
	m_rtf.SetFocus();

	para.dwMask = PFM_BORDER;
	m_rtf.GetParaFormat(para);
}
*/

void CRulerRichEditCtrl::SetParaAlignment(int alignment)
{
	ParaFormat2	para(PFM_ALIGNMENT);
	para.wAlignment = (WORD) alignment;

	m_rtf.SetParaFormat(para);
	m_rtf.SetFocus();
}

int CRulerRichEditCtrl::GetParaAlignment() const
{
	ParaFormat pf(PFM_ALIGNMENT);
	m_rtf.GetParaFormat(pf);
				
	return (int)pf.wAlignment;
}

void CRulerRichEditCtrl::DoFont()
{
	// Get the current font
	LOGFONT	lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	CharFormat	cf;
	m_rtf.SendMessage(EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
	int height;

	// Creating a LOGFONT from the current font settings

	// Font
	if (cf.dwMask & CFM_FACE)
		lstrcpy(lf.lfFaceName, cf.szFaceName);

	if (cf.dwMask & CFM_SIZE)
	{
		double twip = (double)m_physicalInch / 1440;
		height = cf.yHeight;
		height = -(int) ((double) height * twip +.5);
		lf.lfHeight = height;

	}

	// Effects
	if (cf.dwMask & CFM_BOLD)
	{
		if (cf.dwEffects & CFE_BOLD)
			lf.lfWeight = FW_BOLD;
		else
			lf.lfWeight = FW_NORMAL;
	}

	if ((cf.dwMask & CFM_ITALIC) && (cf.dwEffects & CFE_ITALIC))
		lf.lfItalic = TRUE;

	if ((cf.dwMask & CFM_UNDERLINE) && (cf.dwEffects & CFE_UNDERLINE))
		lf.lfUnderline = TRUE;

	if ((cf.dwMask & CFM_STRIKEOUT) && (cf.dwEffects & CFE_STRIKEOUT))
		lf.lfStrikeOut = TRUE;

	// Show font dialog
	CFontDialog	dlg(&lf);

	// color
	dlg.m_cf.rgbColors = cf.crTextColor;

	if (dlg.DoModal() == IDOK)
	{
		// Apply new font
		cf.yHeight = dlg.GetSize() * 2;
		lstrcpy(cf.szFaceName, dlg.GetFaceName());

		cf.dwMask = CFM_FACE | CFM_SIZE | CFM_COLOR | CFM_BOLD | 
					CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT;
		cf.dwEffects = 0;
		cf.crTextColor = dlg.GetColor();

		if (dlg.IsBold())
			cf.dwEffects |= CFE_BOLD;

		if (dlg.IsItalic())
			cf.dwEffects |= CFE_ITALIC;

		if (dlg.IsUnderline())
			cf.dwEffects |= CFE_UNDERLINE;

		if (dlg.IsStrikeOut())
			cf.dwEffects |= CFE_STRIKEOUT;

		m_rtf.SendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
/*
		m_toolbar.SetFontColor(cf.crTextColor, TRUE);
*/
	}

	m_rtf.SetFocus();

}

void CRulerRichEditCtrl::SetCurrentFontName(const CString& font)
{
	CharFormat	cf(CFM_FACE);
	lstrcpy(cf.szFaceName, font);

	m_rtf.SendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
}

void CRulerRichEditCtrl::SetCurrentFontSize(int size)
{
	CharFormat	cf(CFM_SIZE);
	cf.yHeight = size * 20;

	m_rtf.SetSelectionCharFormat(cf);
}

void CRulerRichEditCtrl::SetCurrentFontColor(COLORREF color, BOOL bForeground)
{
	CharFormat	cf(bForeground ? CFM_COLOR : CFM_BACKCOLOR);

	if (bForeground)
	{
		if (color == CLR_DEFAULT)
			cf.dwEffects = CFE_AUTOCOLOR;
		else
			cf.crTextColor = color;
	}
	else // background
	{
		if (color == CLR_DEFAULT)
			cf.dwEffects = CFE_AUTOBACKCOLOR;
		else
			cf.crBackColor = color;
	}

	m_rtf.SetSelectionCharFormat(cf);
}

void CRulerRichEditCtrl::DoColor()
{
	// Get the current color
	COLORREF	clr(RGB(0, 0, 0));
	CharFormat	cf;

	m_rtf.GetSelectionCharFormat(cf);

	if (cf.dwMask & CFM_COLOR)
		clr = cf.crTextColor;

	// Display color selection dialog
	CColorDialog dlg(clr);

	if (dlg.DoModal() == IDOK)
	{
		// Apply new color
		cf.dwMask = CFM_COLOR;
		cf.dwEffects = 0;
		cf.crTextColor = dlg.GetColor();

		m_rtf.SetSelectionCharFormat(cf);
	}

	m_rtf.SetFocus();
}

void CRulerRichEditCtrl::DoBold()
{
	// toggle current state
	BOOL bNewState = !m_rtf.SelectionHasEffect(CFM_BOLD, CFE_BOLD);
	SetEffect(CFM_BOLD, (bNewState ? CFE_BOLD : 0));
}

void CRulerRichEditCtrl::DoStrikethrough()
{
	// toggle current state
	BOOL bNewState = !m_rtf.SelectionHasEffect(CFM_STRIKEOUT, CFE_STRIKEOUT);
	SetEffect(CFM_STRIKEOUT, (bNewState ? CFE_STRIKEOUT : 0));
}

void CRulerRichEditCtrl::DoSuperscript()
{
	// toggle current state
	BOOL bNewState = !m_rtf.SelectionHasEffect(CFM_SUPERSCRIPT, CFE_SUPERSCRIPT);
	SetEffect(CFM_SUPERSCRIPT, (bNewState ? CFE_SUPERSCRIPT : 0));
}

void CRulerRichEditCtrl::DoSubscript()
{
	// toggle current state
	BOOL bNewState = !m_rtf.SelectionHasEffect(CFM_SUBSCRIPT, CFE_SUBSCRIPT);
	SetEffect(CFM_SUBSCRIPT, (bNewState ? CFE_SUBSCRIPT : 0));
}

void CRulerRichEditCtrl::DoItalic()
{
	// toggle current state
	BOOL bNewState = !m_rtf.SelectionHasEffect(CFM_ITALIC, CFE_ITALIC);
	SetEffect(CFM_ITALIC, (bNewState ? CFE_ITALIC : 0));
}

void CRulerRichEditCtrl::DoUnderline()
{
	// toggle current state
	BOOL bNewState = !m_rtf.SelectionHasEffect(CFM_UNDERLINE, CFE_UNDERLINE);
	SetEffect(CFM_UNDERLINE, (bNewState ? CFE_UNDERLINE : 0));
}

void CRulerRichEditCtrl::DoLeftAlign()
{
	if (GetParaAlignment() != PFA_LEFT)
		SetParaAlignment(PFA_LEFT);
}

void CRulerRichEditCtrl::DoCenterAlign()
{
	if (GetParaAlignment() != PFA_CENTER)
		SetParaAlignment(PFA_CENTER);
}

void CRulerRichEditCtrl::DoJustify()
{
	if (GetParaAlignment() != PFA_JUSTIFY)
	{
		if (m_rtf.SendMessage(EM_SETTYPOGRAPHYOPTIONS, TO_ADVANCEDTYPOGRAPHY, TO_ADVANCEDTYPOGRAPHY))
			SetParaAlignment(PFA_JUSTIFY);
	}
}

void CRulerRichEditCtrl::DoRightAlign()
{
	if (GetParaAlignment() != PFA_RIGHT)
		SetParaAlignment(PFA_RIGHT);
}

BOOL CRulerRichEditCtrl::FixupTabStops(ParaFormat& para)
{
	// returns TRUE if any changes were made to the tabstops
	int t = MAX_TAB_STOPS;
	
	// find the last non-zero tabstop
	while (t--)
	{
		if (para.rgxTabs[t] != 0)
			break;
	}

	// rebuild the tabs from the last non-zero element
	if (t < MAX_TAB_STOPS - 1)
	{
		int nTabstop = (t >= 0) ? para.rgxTabs[t] : 0;

		for (++t; t < MAX_TAB_STOPS; t++)
		{
			nTabstop += 320;
			para.rgxTabs[t] = nTabstop;
		}

		return TRUE;
	}

	// else no change
	return FALSE;
}

void CRulerRichEditCtrl::DoIndent()
{
	// Get current indent and list style
	ParaFormat	para(PFM_STARTINDENT | PFM_TABSTOPS);
	m_rtf.GetParaFormat(para);

	if (!para.HasTabs())
		BuildTabStops(para);

	int curindent = para.dxStartIndent;
	int newindent = curindent;

	// Find next larger tab
	for (int t = 0; t < MAX_TAB_STOPS; t++)
	{
		if (para.rgxTabs[ t ] > para.dxStartIndent)
		{
			newindent = para.rgxTabs[ t ];
			break;
		}
	}

	// handle exceeding MAX_TAB_STOPS by incrementing by default amount
	if (newindent == para.dxStartIndent)
		newindent += 320;

	// Set indent to this value and update tabs
	para.dwMask = (PFM_OFFSET | PFM_OFFSETINDENT);
	para.dxStartIndent = (newindent - curindent);

	m_rtf.SetParaFormat(para);
	m_rtf.SetFocus();
}

void CRulerRichEditCtrl::DoOutdent()
{
	// Get the current indent, if any
	ParaFormat	para(PFM_STARTINDENT | PFM_TABSTOPS);
	m_rtf.GetParaFormat(para);

	int curindent = para.dxStartIndent;
	int newindent = curindent;

	// Find closest smaller tab
	for(int t = 0 ; t < MAX_TAB_STOPS ; t++)
	{
		if (para.rgxTabs[ t ] < para.dxStartIndent)
		{
			newindent = para.rgxTabs[ t ];
		}
	}

	if (newindent == curindent)
		newindent = 0;

	// Set indent to this value or 0 if none
	para.dwMask = (PFM_OFFSET | PFM_OFFSETINDENT);
	para.dxStartIndent = (newindent - curindent);

	m_rtf.SetParaFormat(para);
	m_rtf.SetFocus();
}

void CRulerRichEditCtrl::DoBulletList()
{
	ParaFormat2 pf(PFM_NUMBERING);
	m_rtf.GetParaFormat(pf);
				
	BOOL bNewState = !(pf.wNumbering == PFN_BULLET);
	pf.Reset(PFM_NUMBERING | PFM_OFFSET);

	pf.wNumbering = (WORD)(bNewState ? PFN_BULLET : 0);
	pf.dxOffset = (bNewState ? 300 : 0);

	m_rtf.SetParaFormat(pf);
	m_rtf.SetFocus();
}

void CRulerRichEditCtrl::DoNumberList()
{
	ParaFormat2 pf(PFM_NUMBERING);
	m_rtf.GetParaFormat(pf);
				
	BOOL bNewState = (pf.wNumbering <= PFN_BULLET);
	pf.Reset(PFM_NUMBERING | PFM_NUMBERINGSTYLE | PFM_NUMBERINGSTART | PFM_OFFSET);

	if (bNewState)
	{
		pf.wNumbering = PFN_NUMBERLIST;
		pf.wNumberingStart = 1;
		pf.wNumberingStyle = 0x200;
		pf.dxOffset = 300;
	}
	else
	{
		pf.wNumbering = 0;
		pf.dxOffset = 0;
	}

	m_rtf.SetParaFormat(pf);
	m_rtf.SetFocus();
}

void CRulerRichEditCtrl::ShowToolbar(BOOL show)
{
	m_showToolbar = show;

	if (m_hWnd)
	{
		if (show)
			m_toolbar.GetToolBarCtrl().ShowWindow(SW_SHOW);
		else
			m_toolbar.GetToolBarCtrl().ShowWindow(SW_HIDE);

		CRect rect;
		GetClientRect(rect);
		LayoutControls(rect.Width(), rect.Height());
	}
}

void CRulerRichEditCtrl::SetWordWrap(BOOL bWrap)
{
	if (bWrap != m_bWordWrap)
		m_rtf.SetTargetDevice(NULL, bWrap ? 0 : 1);

	m_bWordWrap = bWrap;
	m_toolbar.CheckButton(ID_EDIT_WORDWRAP, m_bWordWrap ? 1 : 0);
}

void CRulerRichEditCtrl::ShowRuler(BOOL show)
{
	m_showRuler = show;

	if (m_hWnd)
	{
		if (show)
			m_ruler.ShowWindow(SW_SHOW);
		else
			m_ruler.ShowWindow(SW_HIDE);

		CRect rect;
		GetClientRect(rect);
		LayoutControls(rect.Width(), rect.Height());
	}
}

void CRulerRichEditCtrl::LayoutControls(int width, int height)
{
	if (!width || !height)
		return;

	int toolbarHeight = 0;

	if (m_showToolbar)
	{
		toolbarHeight = m_toolbar.Resize(width, CPoint(0, 0));
	}

	int rulerHeight = 0;

	if (m_showRuler)
		rulerHeight = RULER_HEIGHT;

	m_ruler.MoveWindow(0, toolbarHeight, width, rulerHeight);

	CRect rect(0, toolbarHeight + rulerHeight, width, height);
	m_rtf.MoveWindow(rect);

}

BOOL CRulerRichEditCtrl::IsToolbarVisible() const
{
	return m_showToolbar;
}

BOOL CRulerRichEditCtrl::IsRulerVisible() const
{
	return m_showRuler;
}

void CRulerRichEditCtrl::SetReadOnly(BOOL readOnly)
{
	if (m_rtf.m_hWnd)
	{
		m_rtf.SetReadOnly(readOnly);
		m_rtf.SetBackgroundColor(!readOnly, GetSysColor(COLOR_3DFACE));

		m_toolbar.EnableWindow(!readOnly);
		m_ruler.EnableWindow(!readOnly);

		UpdateToolbarButtons();
	}

	m_readOnly = readOnly;
}

BOOL CRulerRichEditCtrl::GetReadOnly() const
{
	return m_readOnly;
}

BOOL CRulerRichEditCtrl::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (CWnd::OnCommand(wParam, lParam))
	{
		if (m_toolbar.GetSafeHwnd())
			m_toolbar.RefreshButtonStates(FALSE);

		return TRUE;
	}

	// else
	return FALSE;
}
