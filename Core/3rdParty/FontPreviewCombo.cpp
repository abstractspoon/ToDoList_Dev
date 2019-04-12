/*********************************************************************

   Copyright (C) 2002 Smaller Animals Software, Inc.

   This software is provided 'as-is', without any express or implied
   warranty.  In no event will the authors be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.

   3. This notice may not be removed or altered from any source distribution.

   http://www.smalleranimals.com
   smallest@smalleranimals.com

   --------

   This code is based, in part, on:
   "A WTL-based Font preview combo box", Ramon Smits
   http://www.codeproject.com/wtl/rsprevfontcmb.asp

**********************************************************************/

#include "stdafx.h"
#include "FontPreviewCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CFontPreviewCombo *m_pComboBox = 0;
#define SPACING      10
#define GLYPH_WIDTH  15

/////////////////////////////////////////////////////////////////////////////
// CFontPreviewCombo

CFontPreviewCombo::CFontPreviewCombo(PreviewStyle style, LPCTSTR szSample,
									 COLORREF clrSample, int nFontHeight) :
CComboBox(), m_style(style), m_csSample(szSample), m_clrSample(clrSample), m_iFontHeight(nFontHeight)

{
	m_iMaxNameWidth = 0;
    m_iMaxSampleWidth = 0;
}

CFontPreviewCombo::~CFontPreviewCombo()
{
}


BEGIN_MESSAGE_MAP(CFontPreviewCombo, CComboBox)
//{{AFX_MSG_MAP(CFontPreviewCombo)
ON_WM_MEASUREITEM()
ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontPreviewCombo message handlers


static BOOL CALLBACK EnumFontProc (LPLOGFONT lplf, LPTEXTMETRIC lptm, DWORD dwType, LPARAM lpData)	
{	
	CFontPreviewCombo *pThis = reinterpret_cast<CFontPreviewCombo*>(lpData);		
	int index = pThis->AddString(lplf->lfFaceName);
	ASSERT(index!=-1);
	
	int maxLen = lptm->tmMaxCharWidth * strlen(lplf->lfFaceName);
	int ret = pThis->SetItemData (index, dwType); 
	
	ASSERT(ret!=-1);
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

void CFontPreviewCombo::Init()
{		
	ASSERT (GetSafeHwnd());

	if (m_iFontHeight == -1)
	{
		CRect rect(0, 0, 0, 9); // 9 DLU
		MapDialogRect(::GetParent(m_hWnd), rect);
		m_iFontHeight = rect.bottom;
	}

	//	m_img.Create(IDB_TTF_BMP, GLYPH_WIDTH, 1, RGB(255,255,255));
	CClientDC dc(this);		
	
	EnumFonts (dc, 0,(FONTENUMPROC) EnumFontProc,(LPARAM)this); //Enumerate font
	
	SetItemHeight(-1, m_iFontHeight);
	SetCurSel(0);
}

/////////////////////////////////////////////////////////////////////////////

void CFontPreviewCombo::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	ASSERT(lpDIS->CtlType == ODT_COMBOBOX); 
	
	CRect rc = lpDIS->rcItem;
	
	CDC dc;
	dc.Attach(lpDIS->hDC);
	
	if (lpDIS->itemState & ODS_FOCUS)
		dc.DrawFocusRect(&rc);
	
	if (lpDIS->itemID == -1)
		return;
	
	int nIndexDC = dc.SaveDC();
	int iOffsetX = SPACING;
	
	CBrush br;
	
	COLORREF clrSample = m_clrSample, clrBack, clrText;
	BOOL bSelected = (lpDIS->itemState & ODS_SELECTED);
	
	// colours
	if (!IsWindowEnabled())
	{
		clrText = clrSample = GetSysColor(COLOR_GRAYTEXT);
		clrBack = GetSysColor(COLOR_3DFACE);
	}
	else if (bSelected)
	{
		clrText = clrSample = GetSysColor(COLOR_HIGHLIGHTTEXT);
		clrBack = GetSysColor(COLOR_HIGHLIGHT);
	}
	else 
	{
		clrText = GetSysColor(COLOR_WINDOWTEXT);
		clrBack = GetSysColor(COLOR_WINDOW);
	}
	
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(clrText);
	dc.FillSolidRect(&rc, clrBack);
	
	// which one are we working on?
	CString csCurFontName;
	GetLBText(lpDIS->itemID, csCurFontName);
	
	// draw the cute TTF glyph
	if (m_img.GetSafeHandle())
	{
		DWORD dwData = GetItemData(lpDIS->itemID);
		if (dwData & TRUETYPE_FONTTYPE)
		{
			m_img.Draw(&dc, 0, CPoint(rc.left+5, rc.top+4),ILD_TRANSPARENT);
		}
		rc.left += GLYPH_WIDTH + iOffsetX;
		
	}
	
	
	// i feel bad creating this font on each draw. but i can't think of a better way (other than creating ALL fonts at once and saving them - yuck
	CFont cf;
	if (m_style != FPC_NAME_GUI_FONT)
	{
		if (!cf.CreateFont(m_iFontHeight,0,0,0,FW_NORMAL,FALSE, FALSE, FALSE,DEFAULT_CHARSET ,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH, csCurFontName))
		{
			ASSERT(0);
			return;
		}
	}
	
	// draw the text
	CSize sz;
	int iPosY = 0;
	HFONT hf = NULL;
	switch (m_style)
	{
	case FPC_NAME_GUI_FONT:
		{
			// font name in GUI font
			sz = dc.GetTextExtent(csCurFontName);
			iPosY = (rc.Height() - sz.cy) / 2;
			dc.TextOut(rc.left, rc.top + iPosY,csCurFontName);
		}
		break;
	case FPC_NAME_ONLY:
		{
			// font name in current font
			hf = (HFONT)dc.SelectObject(cf);
			sz = dc.GetTextExtent(csCurFontName);
			iPosY = (rc.Height() - sz.cy) / 2;
			dc.TextOut(rc.left, rc.top + iPosY,csCurFontName);
			dc.SelectObject(hf);
		}
		break;
	case FPC_NAME_THEN_SAMPLE:
		{
			// font name in GUI font
			sz = dc.GetTextExtent(csCurFontName);
			iPosY = (rc.Height() - sz.cy) / 2;
			dc.TextOut(rc.left, rc.top + iPosY, csCurFontName);
			
			// condense, for edit
			int iSep = m_iMaxNameWidth;
			if ((lpDIS->itemState & ODS_COMBOBOXEDIT) == ODS_COMBOBOXEDIT)
			{
				iSep = sz.cx;
			}
			
			// sample in current font
			hf = (HFONT)dc.SelectObject(cf);
			sz = dc.GetTextExtent(m_csSample);
			iPosY = (rc.Height() - sz.cy) / 2;
			COLORREF clr = dc.SetTextColor(clrSample);
			dc.TextOut(rc.left + iSep + iOffsetX, rc.top + iPosY, m_csSample);
			dc.SetTextColor(clr);
			dc.SelectObject(hf);
		}
		break;
	case FPC_SAMPLE_THEN_NAME:
		{
			// sample in current font
			hf = (HFONT)dc.SelectObject(cf);
			sz = dc.GetTextExtent(m_csSample);
			iPosY = (rc.Height() - sz.cy) / 2;
			COLORREF clr = dc.SetTextColor(clrSample);
			dc.TextOut(rc.left, rc.top + iPosY, m_csSample);
			dc.SetTextColor(clr);
			dc.SelectObject(hf);
			
			// condense, for edit
			int iSep = m_iMaxSampleWidth;
			if ((lpDIS->itemState & ODS_COMBOBOXEDIT) == ODS_COMBOBOXEDIT)
			{
				iSep = sz.cx;
			}
			
			// font name in GUI font
			sz = dc.GetTextExtent(csCurFontName);
			iPosY = (rc.Height() - sz.cy) / 2;
			dc.TextOut(rc.left + iSep + iOffsetX, rc.top + iPosY, csCurFontName);
		}
		break;
	case FPC_SAMPLE_ONLY:
		{			
			// sample in current font
			hf = (HFONT)dc.SelectObject(cf);
			sz = dc.GetTextExtent(m_csSample);
			iPosY = (rc.Height() - sz.cy) / 2;
			dc.TextOut(rc.left, rc.top + iPosY, m_csSample);
			dc.SelectObject(hf);
		}
		break;
	}
	
	dc.RestoreDC(nIndexDC);
	
	dc.Detach();
}

/////////////////////////////////////////////////////////////////////////////

void CFontPreviewCombo::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// ok, how big is this ?
	
	CString csFontName;
	GetLBText(lpMeasureItemStruct->itemID, csFontName);
	
	CFont cf;
	if (!cf.CreateFont(m_iFontHeight,0,0,0,FW_NORMAL,FALSE, FALSE, FALSE,DEFAULT_CHARSET ,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH, csFontName))
	{
		ASSERT(0);
		return;
	}
	
	LOGFONT lf;
	cf.GetLogFont(&lf);
	
	if ((m_style == FPC_NAME_ONLY) || (m_style == FPC_SAMPLE_ONLY) || (m_style == FPC_NAME_GUI_FONT))
	{
		m_iMaxNameWidth = 0;
		m_iMaxSampleWidth = 0;
	}
	else
	{
		CClientDC dc(this);
		
		// measure font name in GUI font
		HFONT hFont = ((HFONT)GetStockObject( DEFAULT_GUI_FONT )); 
		HFONT hf = (HFONT)dc.SelectObject(hFont);
		CSize sz = dc.GetTextExtent(csFontName);
		m_iMaxNameWidth = max(m_iMaxNameWidth, sz.cx);
		dc.SelectObject(hf);
		
		// measure sample in cur font
		hf = (HFONT)dc.SelectObject(cf);
		if (hf)
		{
			sz = dc.GetTextExtent(m_csSample);
			m_iMaxSampleWidth = max(m_iMaxSampleWidth, sz.cx);
			dc.SelectObject(hf);
		}
	}
	
	lpMeasureItemStruct->itemHeight = lf.lfHeight;// + 4;
}

/////////////////////////////////////////////////////////////////////////////

void CFontPreviewCombo::OnDropdown() 
{
	m_pComboBox = this;
	
	int nScrollWidth = ::GetSystemMetrics(SM_CXVSCROLL);
	int nWidth = nScrollWidth;
	
	if (m_img.GetSafeHandle())
		nWidth += GLYPH_WIDTH;
	
	switch (m_style)
	{
	case FPC_NAME_GUI_FONT:
		nWidth += m_iMaxNameWidth;
		break;
	case FPC_NAME_ONLY:
		nWidth += m_iMaxNameWidth;
		break;
	case FPC_NAME_THEN_SAMPLE:
		nWidth += m_iMaxNameWidth;
		nWidth += m_iMaxSampleWidth;
		nWidth += SPACING * 2;
		break;
	case FPC_SAMPLE_THEN_NAME:
		nWidth += m_iMaxNameWidth;
		nWidth += m_iMaxSampleWidth;
		nWidth += SPACING * 2;
		break;
	case FPC_SAMPLE_ONLY:
		nWidth += m_iMaxSampleWidth;
		break;
	}
	
	SetDroppedWidth(nWidth);
}

