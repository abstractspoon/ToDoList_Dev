// entoolbar.cpp : implementation file
//

#include "stdafx.h"
#include "entoolbar.h"
#include "enbitmapex.h"
#include "imageprocessors.h"
#include "osversion.h"
#include "graphicsmisc.h"
#include "themed.h"

#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const UINT WM_REFRESHBUTTONSTATES = WM_APP+1;

/////////////////////////////////////////////////////////////////////////////

#ifndef TBCDRF_NOBACKGROUND
#	define TBCDRF_NOBACKGROUND   0x00400000
#endif
/////////////////////////////////////////////////////////////////////////////

void AFXAPI AfxDeleteObject(HGDIOBJ* pObject);

/////////////////////////////////////////////////////////////////////////////
// CEnToolBar

CEnToolBar::CEnToolBar() : m_crFrom(CLR_NONE), m_crTo(CLR_NONE), m_crHot(CLR_NONE)
{
}

CEnToolBar::~CEnToolBar()
{
	m_ilNormal.DeleteImageList();
	m_ilDisabled.DeleteImageList();
}

BEGIN_MESSAGE_MAP(CEnToolBar, CToolBar)
	//{{AFX_MSG_MAP(CEnToolBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_SETTINGCHANGE()
	ON_MESSAGE(WM_REFRESHBUTTONSTATES, OnRefreshButtonStates)
	ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
	ON_WM_DESTROY()
	ON_WM_NCPAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnToolBar message handlers

BOOL CEnToolBar::LoadToolBar(LPCTSTR lpszResourceName, LPCTSTR szImagePath, COLORREF crMask)
{
	if (CToolBar::LoadToolBar(lpszResourceName))
	{
		if (szImagePath && ::GetFileAttributes(szImagePath) != 0xffffffff)
		{
			SetImage(szImagePath, crMask);
		}
		else
		{
			GetToolBarCtrl().SetDisabledImageList(NULL);
			m_ilDisabled.DeleteImageList();
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CEnToolBar::LoadToolBar(UINT nIDResource, LPCTSTR szImagePath, COLORREF crMask)
{
	return LoadToolBar(MAKEINTRESOURCE(nIDResource), szImagePath, crMask);
}

BOOL CEnToolBar::LoadToolBar(UINT nIDResource, UINT nIDImage, COLORREF crMask)
{
	if (CToolBar::LoadToolBar(MAKEINTRESOURCE(nIDResource)))
	{
		if (nIDImage)
		{
			SetImage(nIDImage, crMask);
		}
		else
		{
			GetToolBarCtrl().SetDisabledImageList(NULL);
			m_ilDisabled.DeleteImageList();
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CEnToolBar::SetImage(UINT nIDImage, COLORREF crMask)
{
	CEnBitmapEx bitmap;
	
	if (!bitmap.LoadBitmap(nIDImage))
		return FALSE;
	
	return SetImage(&bitmap, crMask);
}

BOOL CEnToolBar::SetImage(const CString& sImagePath, COLORREF crMask)
{
	CEnBitmapEx bitmap;
	
	if (!bitmap.LoadImage(sImagePath))
		return FALSE;
	
	return SetImage(&bitmap, crMask);
}

BOOL CEnToolBar::SetImage(CEnBitmapEx* pBitmap, COLORREF crMask)
{
	CEnBitmapEx bmDis;
	bmDis.CopyImage(pBitmap); // for later
	
    pBitmap->RemapSysColors();
	
	// button size
	BITMAP BM;
	pBitmap->GetBitmap(&BM);
	
	int nCount = GetButtonCount(TRUE);
	ASSERT (nCount);
	
	if (!nCount)
		return FALSE;
	
	CSize sizeBmp((BM.bmWidth / nCount), BM.bmHeight);
	CSize sizeBtn(sizeBmp.cx + 7, sizeBmp.cy + 7);
	
	SetSizes(sizeBtn, sizeBmp);

	m_ilNormal.DeleteImageList();
	
	if (m_ilNormal.Create(sizeBmp.cx, sizeBmp.cy, ILC_COLOR32 | ILC_MASK, 0, 1)) 
	{
		m_ilNormal.Add(pBitmap, crMask);
				
		CImageList* pILPrev = GetToolBarCtrl().SetImageList(&m_ilNormal);

		if (pILPrev)
			pILPrev->DeleteImageList(); // cleanup

		pILPrev = GetToolBarCtrl().SetHotImageList(&m_ilNormal);

		if (pILPrev)
			pILPrev->DeleteImageList(); // cleanup
		
		RefreshDisabledImageList(&bmDis, crMask);
		return TRUE;
	}
	
	// else 
	return FALSE;
}

int CEnToolBar::GetButtonCount(BOOL bIgnoreSeparators) const
{
	int nCount = GetToolBarCtrl().GetButtonCount();

	if (bIgnoreSeparators)
	{
		int nBtn = nCount;

		while (nBtn--)
		{
			if (GetItemID(nBtn) == ID_SEPARATOR)
				nCount--;
		}
	}

	return nCount;
}

void CEnToolBar::SetBackgroundColors(COLORREF crFrom, COLORREF crTo, BOOL bGradient, BOOL bGlass)
{
	m_crFrom = crFrom;

	if (crTo == CLR_NONE)
		m_crTo = m_crFrom;
	else
		m_crTo = crTo;

	m_bGradient = bGradient;
	m_bGlass = bGlass;

	if (GetSafeHwnd())
		Invalidate();
}

void CEnToolBar::RefreshButtonStates(BOOL bImmediate)
{
	if (bImmediate)
		OnUpdateCmdUI((CFrameWnd*)GetParent(), FALSE);
	else
		PostMessage(WM_REFRESHBUTTONSTATES);
}

LRESULT CEnToolBar::OnRefreshButtonStates(WPARAM /*wp*/, LPARAM /*lp*/)
{
	RefreshButtonStates(TRUE);
	return 0L;
}

void CEnToolBar::RefreshDisabledImageList(CEnBitmapEx* pBitmap, COLORREF crMask) 
{
	// create 'nice' disabled imagelist 
	if (pBitmap->GrayScale(crMask))
	{
		if (crMask == CLR_NONE) // map 3d colors
			pBitmap->RemapSysColors();
		
		// button size
		int nCx = m_sizeImage.cx, nCy = m_sizeImage.cy;
		
		m_ilDisabled.DeleteImageList();
		m_ilDisabled.Create(nCx, nCy, ILC_COLOR24 | ILC_MASK, 0, 1);
		m_ilDisabled.Add(pBitmap, crMask);
		
		CImageList* pILPrev = GetToolBarCtrl().SetDisabledImageList(&m_ilDisabled);
		
		if (pILPrev)
			pILPrev->DeleteImageList(); // cleanup
		
		Invalidate();
	}
}

void CEnToolBar::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    *pResult = CDRF_DODEFAULT;

	if (!CThemed::AreControlsThemed())
		return;

    LPNMTBCUSTOMDRAW lpNMCustomDraw = ( LPNMTBCUSTOMDRAW )pNMHDR;
    
    switch ( lpNMCustomDraw->nmcd.dwDrawStage )
    {
    case CDDS_PREPAINT:
 		DrawBkgnd(CDC::FromHandle(lpNMCustomDraw->nmcd.hdc), TRUE);

		*pResult = CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
		break;
		
    case CDDS_POSTPAINT:
       *pResult = CDRF_DODEFAULT | CDRF_NOTIFYITEMDRAW;
       break;
       
    case CDDS_PREERASE:
       *pResult = CDRF_SKIPDEFAULT | CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
       break;
       
    case CDDS_ITEMPREPAINT:
       *pResult = OnItemPrePaint(lpNMCustomDraw) | CDRF_NOTIFYPOSTPAINT;
       break;

    case CDDS_ITEMPOSTPAINT:
       *pResult = OnItemPostPaint(lpNMCustomDraw);
       break;

    default:
       break;
    }
}

LRESULT CEnToolBar::OnItemPrePaint(LPNMTBCUSTOMDRAW lpNMCustomDraw) 
{ 
	int nBtnID = lpNMCustomDraw->nmcd.dwItemSpec;
	const CToolBarCtrl& tbc = GetToolBarCtrl();

	CDC* pDC = CDC::FromHandle(lpNMCustomDraw->nmcd.hdc);

	COLORREF crFill = CLR_NONE;
	LRESULT nFlags = (TBCDRF_NOBACKGROUND | TBCDRF_NOEDGES);

	// Note: Only offset pressed buttons
	if (tbc.IsButtonPressed(nBtnID))
	{
		crFill = GraphicsMisc::Darker(GetHotColor(), 0.1, FALSE);
	}
	else if (CommandToIndex(nBtnID) == tbc.GetHotItem())
	{
		crFill = GetHotColor();
		nFlags |= TBCDRF_NOOFFSET;
	}
	else if (tbc.IsButtonChecked(nBtnID))
	{
		crFill = GraphicsMisc::Darker(GetHotColor(), 0.1, FALSE);
		nFlags |= TBCDRF_NOOFFSET;
	}
	else
	{
		return CDRF_DODEFAULT;
	}

	DrawButtonBackground(pDC, lpNMCustomDraw->nmcd.rc, crFill);
	return nFlags;
}

COLORREF CEnToolBar::GetHotColor() const
{
	return ((m_crHot != CLR_NONE) ? m_crHot : ((m_crFrom != CLR_NONE) ? m_crFrom : GetSysColor(COLOR_3DFACE)));
}

void CEnToolBar::DrawButtonBackground(CDC* pDC, const CRect& rBtn, COLORREF crFill)
{
	static int nRadius = ((COSVersion() >= OSV_WIN8) ? 0 : 3);

	COLORREF crBorder = GraphicsMisc::Darker(crFill, 0.2, FALSE);

	GraphicsMisc::DrawRect(pDC, rBtn, crFill, crBorder, nRadius);
}

LRESULT CEnToolBar::OnItemPostPaint(LPNMTBCUSTOMDRAW /*lpNMCustomDraw*/) 
{ 
	return CDRF_DODEFAULT; 
}

void CEnToolBar::OnNcPaint()
{
	CDC* pDC = GetWindowDC();

	DrawBkgnd(pDC, FALSE);
	ReleaseDC(pDC);

	Default();
}

void CEnToolBar::DrawBkgnd(CDC* pDC, BOOL bClient)
{
	// get client rect always
	CRect rect;
	GetClientRect(rect);

	// if rendering the non-client background
	// we first clip out the client rect and
	// translate the window rect to (0, 0)
	if (!bClient)
	{
		CRect rWindow;
		GetWindowRect(rWindow);

		ClientToScreen(rect);
		rect.OffsetRect(-rWindow.TopLeft());
		pDC->ExcludeClipRect(rect);

		rect = rWindow;
		rect.OffsetRect(-rect.TopLeft());
	}

	if (HasBkgndColor())
	{
		if (m_crTo == m_crFrom)
		{
			pDC->FillSolidRect(rect, m_crFrom);
		}
		else
		{
			GM_GRADIENT nType = GraphicsMisc::GetGradientType(m_bGlass, m_bGradient);

			// if rendering the non-client background
			// first fill the top and bottom borders
			if (!bClient)
			{
				if (m_cyTopBorder > 0)
				{
					CRect rBorder(rect);
					rBorder.bottom = m_cyTopBorder;
					pDC->FillSolidRect(rBorder, GraphicsMisc::GetGradientEdgeColor(nType, m_crFrom, TRUE));

					// update rect
					rect.top = rBorder.bottom;
				}

				if (m_cyBottomBorder > 0)
				{
					CRect rBorder(rect);
					rBorder.top = (rect.bottom - m_cyBottomBorder);
					pDC->FillSolidRect(rBorder, GraphicsMisc::GetGradientEdgeColor(nType, m_crTo, FALSE));

					// update rect
					rect.bottom = rBorder.top;
				}
			}

			// draw the non-client rows only if we have 
			// a left or right border
			if (bClient || (m_cxLeftBorder > 0) || (m_cxRightBorder > 0))
			{
				int nNumRows = m_aRowHeights.GetSize();
				CRect rRow(rect);

				for (int nRow = 0; nRow < nNumRows; nRow++)
				{
					rRow.bottom = rRow.top + m_aRowHeights[nRow];
					
					GraphicsMisc::DrawGradient(nType, pDC, rRow, m_crFrom, m_crTo, FALSE);

					// next row
					rRow.top = rRow.bottom;
				}
			}
		}
	}
	else
	{
		pDC->FillSolidRect(rect, GetSysColor(COLOR_3DFACE));
	}
}

void CEnToolBar::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	// toolbar button sizes get screwed if any button has the drop style applied
	// at the time this is handled. to get round it we remove any such button styles
	// and then readd them after the default processing
	CUIntArray aDropBtns;
	int nBtn = GetToolBarCtrl().GetButtonCount();

	while (nBtn--)
	{
		UINT nBtnID = GetItemID(nBtn);
		DWORD dwStyle = GetButtonStyle(nBtn);
		
		if (dwStyle & TBSTYLE_DROPDOWN)
		{
			SetButtonStyle(nBtn, dwStyle & ~TBSTYLE_DROPDOWN);
			aDropBtns.Add(nBtnID);
		}
	}

	// default processing
	CToolBar::OnSettingChange(uFlags, lpszSection);

	// restore styles
	int nItem = aDropBtns.GetSize();

	while (nItem--)
	{
		int nBtn = CommandToIndex(aDropBtns[nItem]);
		DWORD dwStyle = GetButtonStyle(nBtn);
	
		SetButtonStyle(nBtn, dwStyle | TBSTYLE_DROPDOWN);
	}
}

int CEnToolBar::EstimateHeightRequired(int cx) const
{
	int nRows = EstimateRowsRequired(cx);
	int nRowHeight = (nRows == 1) ? (m_sizeButton.cy + 2) : 
					(nRows < 4) ? (m_sizeButton.cy + 4) : (m_sizeButton.cy + 6);

	return ((nRows * nRowHeight) + m_cyTopBorder + m_cyBottomBorder);
}

int CEnToolBar::GetMinReqLength() const
{
	int nLen = (m_cxLeftBorder + m_cxLeftBorder);
	int nBtn = GetButtonCount();

	while (nBtn--)
	{
		CRect rBtn;
		GetItemRect(nBtn, rBtn);

		nLen += rBtn.Width();
	}

	return nLen;
}

int CEnToolBar::EstimateRowsRequired(int cx) const
{
	int nRows = 1;
	int nXPos = 0;
	int nBtnCount = GetButtonCount();

	for (int nBtn = 0; nBtn < nBtnCount; nBtn++)
	{
		CRect rBtn;
		GetItemRect(nBtn, rBtn);

		if (nXPos) // a button has been added
		{
			if (nXPos + rBtn.Width() > cx)
			{
				nRows++;
				nXPos = 0;
			}
		}
		
		// else
		nXPos += rBtn.Width();
	}

	return nRows;
}

int CEnToolBar::RefreshRowHeights()
{
	m_aRowHeights.RemoveAll();

	int nBtnCount = GetToolBarCtrl().GetButtonCount();

	if (nBtnCount == 0)
		return 0;

	// work thru the buttons noticing the changes in vertical position
	int nRowHead = 0;
	int nMaxRowHeight = 0, nTotalHeight = 0;

	// get the first button rect as our start point
	CRect rBtn;
	GetItemRect(0, rBtn);

	for (int nBtn = 1; nBtn < nBtnCount; nBtn++)
	{
		CRect rNext;
		GetItemRect(nBtn, rNext);

		if (rNext.top > rBtn.top) // new row
		{
			// add current row
			int nRowBase = (rBtn.bottom + rNext.top) / 2;
			int nRowHeight = nRowBase - nRowHead;

			m_aRowHeights.Add(nRowHeight);

			// keep track of max row height
			if (nRowHeight > nMaxRowHeight)
				nMaxRowHeight = nRowHeight;

			// and total row height
			nTotalHeight += nRowHeight;

			// next row
			rBtn = rNext;
			nRowHead = nRowBase;
		}
	}

	// last row is the maximum of the button base + 2,
	int nLastRow = (rBtn.bottom + 2 - nRowHead);
	
	// or the maximum row height calculated
	nLastRow = max(nMaxRowHeight, nLastRow);
	
	m_aRowHeights.Add(nLastRow);
	nTotalHeight += (nLastRow + m_cyBottomBorder + m_cyTopBorder);

	return nTotalHeight;
}

int CEnToolBar::Resize(int cx, CPoint ptTopLeft)
{
	int nEstHeight = EstimateHeightRequired(cx);
	CRect rToolbar(ptTopLeft, CSize(cx, nEstHeight));

	MoveWindow(rToolbar);
	
	int nRealHeight = RefreshRowHeights();

	if (nRealHeight != nEstHeight)
	{
		rToolbar.bottom = rToolbar.top + nRealHeight;
		MoveWindow(rToolbar);
	}

	return nRealHeight;
}

int CEnToolBar::GetHeight() const
{
	CRect rToolbar;
	GetWindowRect(rToolbar);
	return rToolbar.Height();
}

void CEnToolBar::OnDestroy()
{
	CToolBar::OnDestroy();

	AfxDeleteObject((HGDIOBJ*)&m_hbmImageWell);

	m_ilNormal.DeleteImageList();
	m_ilDisabled.DeleteImageList();

}