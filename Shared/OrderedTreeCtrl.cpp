// ToDoCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "orderedtreeCtrl.h"

#include "..\shared\themed.h"
#include "..\shared\treectrlhelper.h"
#include "..\shared\misc.h"
#include "..\shared\graphicsmisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COrderedTreeCtrl

const UINT MINGUTTER = 16;

static CMap<int, int&, UINT, UINT&> g_mapWidths;

COrderedTreeCtrl::COrderedTreeCtrl(DWORD dwGutterStyles) : 

    // because CTreeCtrlHelper wants a reference passed
    // to its constructor we have to pass '*this'. however
    // the compiler complains because 'this' is not yet
    // fully constructed.
#pragma warning (disable: 4355)
	m_ht(*this),
#pragma warning (default: 4355)

	m_gutter(dwGutterStyles),
	m_bShowingPosColumn(TRUE), 
	m_crGridlines(OTC_GRIDCOLOR),
	m_crAltLines(CLR_NONE),
	m_bWantInit(FALSE)
{
	AddGutterColumn(OTC_POSCOLUMNID, _T("Pos")); // for the pos string
	
	EnableGutterColumnHeaderClicking(OTC_POSCOLUMNID, FALSE);
}

COrderedTreeCtrl::~COrderedTreeCtrl()
{
}

BEGIN_MESSAGE_MAP(COrderedTreeCtrl, CTreeCtrl)
//{{AFX_MSG_MAP(COrderedTreeCtrl)
//}}AFX_MSG_MAP
	ON_WM_SETTINGCHANGE()
	ON_WM_STYLECHANGED()
	ON_NOTIFY_REFLECT_EX(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT_EX(TVN_ITEMEXPANDED, OnItemexpanded)
	ON_NOTIFY_REFLECT_EX(NM_CUSTOMDRAW, OnCustomDraw)
	ON_NOTIFY_REFLECT_EX(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBeginDrag)

	ON_REGISTERED_MESSAGE(WM_NCG_GETFIRSTVISIBLETOPLEVELITEM, OnGutterGetFirstVisibleTopLevelItem)
	ON_REGISTERED_MESSAGE(WM_NCG_GETNEXTITEM, OnGutterGetNextItem)
	ON_REGISTERED_MESSAGE(WM_NCG_DRAWITEMCOLUMN, OnGutterDrawItemColumn)
	ON_REGISTERED_MESSAGE(WM_NCG_PREDRAWITEM, OnGutterPreDrawItem)
	ON_REGISTERED_MESSAGE(WM_NCG_RECALCCOLWIDTH, OnGutterRecalcColWidth)
	ON_REGISTERED_MESSAGE(WM_NCG_POSTNCDRAW, OnGutterPostNcDraw)
	ON_REGISTERED_MESSAGE(WM_NCG_GETITEMRECT, OnGutterGetItemRect)
	ON_REGISTERED_MESSAGE(WM_NCG_GETITEMHEIGHT, OnGutterGetItemHeight)
	ON_REGISTERED_MESSAGE(WM_NCG_GETITEMCOLORS, OnGutterGetItemColors)
	ON_REGISTERED_MESSAGE(WM_NCG_GETFIRSTCHILDITEM, OnGutterGetFirstChildItem)
	ON_REGISTERED_MESSAGE(WM_NCG_POSTDRAWITEM, OnGutterPostDrawItem)
	ON_REGISTERED_MESSAGE(WM_NCG_HITTEST, OnGutterHitTest)
	ON_REGISTERED_MESSAGE(WM_NCG_NOTIFYITEMCLICK, OnGutterNotifyItemClick)
	ON_REGISTERED_MESSAGE(WM_NCG_GETPARENTITEM, OnGutterGetParentID)
	ON_REGISTERED_MESSAGE(WM_NCG_WANTRECALC, OnGutterWantRecalc)
	ON_REGISTERED_MESSAGE(WM_NCG_WANTREDRAW, OnGutterWantRedraw)
	ON_REGISTERED_MESSAGE(WM_NCG_WIDTHCHANGE, OnGutterNotifyWidthChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderedTreeCtrl message handlers

void COrderedTreeCtrl::PreSubclassWindow() 
{
	m_gutter.AddRecalcMessage(TVM_INSERTITEM);
	m_gutter.AddRecalcMessage(TVM_DELETEITEM);

	// don't handle TVM_EXPAND because expanding the 
	// entire tree will call this for every item expanded
	//m_gutter.AddRecalcMessage(TVM_EXPAND);
	
	m_gutter.AddRedrawMessage(WM_KEYUP); // only way to catch keyboard navigation (at present)
	m_gutter.AddRedrawMessage(WM_SYSKEYUP); // only way to catch ALT+keyboard navigation (at present)
	m_gutter.AddRedrawMessage(WM_KEYDOWN); // only way to catch keyboard navigation (at present)
	m_gutter.AddRedrawMessage(WM_SYSKEYDOWN); // only way to catch ALT+keyboard navigation (at present)
	m_gutter.AddRedrawMessage(TVM_SELECTITEM);
	m_gutter.AddRedrawMessage(TVM_SORTCHILDREN);
	m_gutter.AddRedrawMessage(TVM_SORTCHILDRENCB);
	m_gutter.AddRedrawMessage(WM_COMMAND, EN_KILLFOCUS);
	
	CTreeCtrl::PreSubclassWindow();
	
	// note: its too early to initialize the gutter here because 
	// MFC hasn't done its bit yet, so initilization is done
	// in WindowProc (for now)
	m_bWantInit = TRUE;
}

void COrderedTreeCtrl::RecalcGutter(BOOL bForceRedraw) 
{ 
	m_gutter.RecalcGutter(bForceRedraw); 

	if (m_crAltLines != CLR_NONE)
		TCH().BuildVisibleIndexMap();
}

LRESULT COrderedTreeCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (m_bWantInit && !m_gutter.IsInitialized())
	{
		m_bWantInit = FALSE;
		m_gutter.Initialize(GetSafeHwnd());
	}
	
	return CTreeCtrl::WindowProc(message, wParam, lParam);
}

void COrderedTreeCtrl::SetAlternateLineColor(COLORREF color)
{
	if (m_crAltLines != color)
	{
		m_crAltLines = color;

		if (GetSafeHwnd())
		{
			Invalidate();
			RedrawGutter();
		}
	}
}

BOOL COrderedTreeCtrl::PtInHeader(const CPoint& ptScreen) const
{
	return m_gutter.PtInHeader(ptScreen);
}

UINT COrderedTreeCtrl::ColumnHitTest(const CPoint& ptScreen) const
{
	return m_gutter.ColumnIDHitTest(ptScreen);
}

COLORREF COrderedTreeCtrl::GetItemLineColor(HTREEITEM hti)
{
	if (m_crAltLines != -1 && TCH().ItemLineIsOdd(hti))
		return m_crAltLines;
	
	// else
	return GetSysColor(COLOR_WINDOW);
}

LRESULT COrderedTreeCtrl::OnGutterGetItemColors(WPARAM /*wParam*/, LPARAM lParam)
{
	NCGITEMCOLORS* pColors = (NCGITEMCOLORS*)lParam;
	HTREEITEM hti = (HTREEITEM)pColors->dwItem;

	if (GetSelectedItem() == hti)
	{
		BOOL bHasFocus = (GetFocus() == this);

		if (!pColors->bBackSet)
			pColors->crBack = GetSysColor(bHasFocus ? COLOR_HIGHLIGHT : COLOR_3DFACE);

		if (!pColors->bTextSet)
		{
			GM_ITEMSTATE nState = (bHasFocus ? GMIS_SELECTED : GMIS_SELECTEDNOTFOCUSED);
			pColors->crText = GraphicsMisc::GetExplorerItemTextColor(pColors->crBack, nState, 0);
			pColors->bTextSet = TRUE;
		}
	}
	else if (!pColors->bBackSet)
	{
		pColors->crBack = GetItemLineColor(hti);
	}

	pColors->bBackSet = TRUE;

	return 0L;
}

BOOL COrderedTreeCtrl::ShowGutterPosColumn(BOOL bShow)
{
	if (m_bShowingPosColumn != bShow)
	{
		m_bShowingPosColumn = bShow;
		m_gutter.RecalcGutter();

		return TRUE;
	}

	// else no change
	return FALSE;
}

void COrderedTreeCtrl::SetGridlineColor(COLORREF color)
{
	if (m_crGridlines != color)
	{
		m_crGridlines = color;
		
		if (GetSafeHwnd())
		{
			m_gutter.Redraw();
			Invalidate();
		}
	}
}

BOOL COrderedTreeCtrl::AddGutterColumn(UINT nColID, LPCTSTR szTitle, UINT nWidth, UINT nTextAlign)
{
	return m_gutter.AddColumn(nColID, szTitle, nWidth, nTextAlign);
}

BOOL COrderedTreeCtrl::InsertGutterColumn(int nPos, UINT nColID, LPCTSTR szTitle, UINT nWidth, UINT nTextAlign)
{
	return m_gutter.InsertColumn(nPos, nColID, szTitle, nWidth, nTextAlign);
}

void COrderedTreeCtrl::RemoveGutterColumn(UINT nColID)
{
	m_gutter.RemoveColumn(nColID);
}

void COrderedTreeCtrl::PressGutterColumnHeader(UINT nColID, BOOL bPress)
{
	m_gutter.PressHeader(nColID, bPress);
}

void COrderedTreeCtrl::SetGutterColumnHeaderTitle(UINT nColID, LPCTSTR szTitle, LPCTSTR szFont)
{
	m_gutter.SetHeaderTitle(nColID, szTitle, szFont, FALSE);
}

void COrderedTreeCtrl::SetGutterColumnHeaderTitle(UINT nColID, UINT nSymbol, LPCTSTR szFont)
{
	// Valik - cast to TCHAR is necessary or the compiler complains under VC 7.1
	m_gutter.SetHeaderTitle(nColID, CString(static_cast<TCHAR>(nSymbol)), szFont, TRUE);
}

void COrderedTreeCtrl::SetGutterColumnHeaderTip(UINT nColID, LPCTSTR szTip)
{
	if (!Misc::IsEmpty(szTip))
	{
		m_mapTooltips[nColID] = szTip;
	
		if (m_gutter.IsInitialized())
		{
			CToolTipCtrl* pTTC = GetToolTips();

			if (pTTC)
			{
				CRect rTool;
				m_gutter.GetColumnHeaderClientRect(nColID, rTool);
				pTTC->AddTool(this, szTip, rTool, nColID);
			}
		}
	}
	else
	{
		m_mapTooltips.RemoveKey(nColID);
	
		if (m_gutter.IsInitialized())
		{
			CToolTipCtrl* pTTC = GetToolTips();

			if (pTTC)
			{
				pTTC->DelTool(this, nColID);
			}
		}
	}
}

void COrderedTreeCtrl::UpdateHeaderTooltipRects()
{
	if (m_gutter.IsInitialized())
	{
		CToolTipCtrl* pTTC = GetToolTips();
		
		if (pTTC)
		{
			int nCol = m_gutter.GetColumnCount();
			CString sTip;

			while (nCol--)
			{
				UINT nColID = m_gutter.GetColumnID(nCol);

				if (m_mapTooltips.Lookup(nColID, sTip) && !sTip.IsEmpty())
				{
					// some debugging
					CToolInfo ti;
					pTTC->GetToolInfo(ti, this, nColID);

					// update rect
					CRect rTool;
					m_gutter.GetColumnHeaderClientRect(nColID, rTool);

					pTTC->SetToolRect(this, nColID, rTool);
				}
			}
		}
	}
}

void COrderedTreeCtrl::SetGutterColumnSort(UINT nColID, NCGSORT nSortDir)
{
	m_gutter.SetColumnSort(nColID, nSortDir);
}

NCGSORT COrderedTreeCtrl::GetGutterColumnSort(UINT nColID) const
{
	return m_gutter.GetColumnSort(nColID);
}

void COrderedTreeCtrl::SetColumnTextAlignment(UINT nColID, UINT nTextAlign, BOOL bRedraw)
{
	m_gutter.SetColumnTextAlignment(nColID, nTextAlign, bRedraw);
}

UINT COrderedTreeCtrl::GetColumnTextAlignment(UINT nColID) const
{
	return m_gutter.GetColumnTextAlignment(nColID);
}

void COrderedTreeCtrl::EnableGutterColumnHeaderClicking(UINT nColID, BOOL bEnable)
{
	m_gutter.EnableHeaderClicking(nColID, bEnable);
}

LRESULT COrderedTreeCtrl::OnGutterGetFirstVisibleTopLevelItem(WPARAM /*wParam*/, LPARAM lParam)
{
	ASSERT (lParam);

	// on tasklists with 1000s of tasks it can really slow down the 
	// rendering if we return the First Visible Top Level Item in all cases
	// so we only do so when we are actually displaying the position column
	// because the level information is only required for displaying a 
	// task's position
	// Unfortunately this had some unexpected side-effects concerning
	// ClearType and expanding tasks when ToDoList is maximized so I've
	// disabled the fix for now which is a real bummer. 26/08/2009
	if (m_bShowingPosColumn)
		return (LRESULT)TCH().GetFirstVisibleTopLevelItem(*((LPINT)lParam));
	else
		return (LRESULT)TCH().GetFirstVisibleTopLevelItem();
//		return (LRESULT)GetFirstVisibleItem();
}

LRESULT COrderedTreeCtrl::OnGutterGetNextItem(WPARAM /*wParam*/, LPARAM lParam)
{
//	if (m_bShowingPosColumn)
		return (LRESULT)GetNextItem((HTREEITEM)lParam, TVGN_NEXT);
//	else
//		return (LRESULT)TCH().GetNextVisibleItem((HTREEITEM)lParam);
}

LRESULT COrderedTreeCtrl::OnGutterNotifyWidthChange(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	UpdateHeaderTooltipRects();
	return 0L;
}

LRESULT COrderedTreeCtrl::OnGutterWantRecalc(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	TCH().BuildVisibleIndexMap();

	// do not recalc if editing a label
	if (GetEditControl() != NULL)
		return 1L; // cancel recalc

	// else
	return 0L;
}

LRESULT COrderedTreeCtrl::OnGutterWantRedraw(WPARAM /*wParam*/, LPARAM lParam)
{
	MSG* pMsg = (MSG*)lParam;

	switch (pMsg->message)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		{
			switch (pMsg->wParam) // what key
			{
			// ignore modifiers
			case VK_CONTROL:
			case VK_SHIFT:
			case VK_MENU:
				return TRUE; // prevent redraw
			}
		}
		break;
	}

	return FALSE; // allow redraw
}

LRESULT COrderedTreeCtrl::OnGutterGetFirstChildItem(WPARAM /*wParam*/, LPARAM lParam)
{
	HTREEITEM hti = (HTREEITEM)lParam;
	
	if (ItemHasChildren(hti) && (GetItemState(hti, TVIS_EXPANDED) & TVIS_EXPANDED))
		return (LRESULT)GetChildItem(hti);
	
	return 0;
}

LRESULT COrderedTreeCtrl::OnGutterDrawItemColumn(WPARAM /*wParam*/, LPARAM lParam)
{
	NCGDRAWITEM* pNCGDI = (NCGDRAWITEM*)lParam;
	
	if (pNCGDI->nColID == OTC_POSCOLUMNID)
	{
		CRect rItem(pNCGDI->rItem);

		NcDrawItemColumn(pNCGDI->pDC, pNCGDI->dwItem, pNCGDI->dwParentItem, 
						pNCGDI->nColID, rItem, pNCGDI->nLevel, 
						pNCGDI->nItemPos, pNCGDI->rWindow);
		
		return TRUE; // we handled it
	}
	
	// else
	return FALSE;
}

LRESULT COrderedTreeCtrl::OnGutterPreDrawItem(WPARAM /*wParam*/, LPARAM lParam)
{
	NCGDRAWITEM* pNCGDI = (NCGDRAWITEM*)lParam;
	
	// if the item rect does not intersect the window rect
	// then there's no need to draw but we do still have to 
	// call Format position if the position column is visible
	if (pNCGDI->rItem->bottom < pNCGDI->rWindow->top)
	{
		if (m_bShowingPosColumn)
			FormatPosition(pNCGDI->dwItem, pNCGDI->dwParentItem, pNCGDI->nLevel, pNCGDI->nItemPos);

		return 1L; // don't draw
	}
	
	// else
	return 0L; // means draw
}

LRESULT COrderedTreeCtrl::OnGutterPostDrawItem(WPARAM /*wParam*/, LPARAM lParam)
{
	NCGDRAWITEM* pNCGDI = (NCGDRAWITEM*)lParam;

	//if (pNCGDI->dwParentItem)
		PostNcDrawItem(pNCGDI->pDC, pNCGDI->dwParentItem, pNCGDI->rItem, pNCGDI->nLevel);
	
	return FALSE; // to let our parent handle it too
}

LRESULT COrderedTreeCtrl::OnGutterPostNcDraw(WPARAM /*wParam*/, LPARAM lParam)
{
	NCGDRAWITEM* pNCGDI = (NCGDRAWITEM*)lParam;
	
	PostNcDraw(pNCGDI->pDC, pNCGDI->rWindow);
	
	return FALSE; // to let our parent handle it too
}

LRESULT COrderedTreeCtrl::OnGutterRecalcColWidth(WPARAM /*wParam*/, LPARAM lParam)
{
	NCGRECALCCOLUMN* pNCRC = (NCGRECALCCOLUMN*)lParam;
	
	return RecalcColumnWidth(pNCRC->pDC, pNCRC->nColID, pNCRC->nWidth);
}

LRESULT COrderedTreeCtrl::OnGutterGetItemRect(WPARAM /*wParam*/, LPARAM lParam)
{
	NCGITEMRECT* pNCGGI = (NCGITEMRECT*)lParam;
	
	return GetItemRect((HTREEITEM)pNCGGI->dwItem, &pNCGGI->rItem, TRUE);
}

LRESULT COrderedTreeCtrl::OnGutterGetItemHeight(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return GetItemHeight();
}

LRESULT COrderedTreeCtrl::OnGutterGetParentID(WPARAM /*wParam*/, LPARAM lParam)
{
	return (DWORD)GetParentItem((HTREEITEM)lParam);
}

LRESULT COrderedTreeCtrl::OnGutterHitTest(WPARAM /*wParam*/, LPARAM lParam)
{
	CPoint point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	
	UINT nFlags = 0;
	HTREEITEM hti = HitTest(point, &nFlags);
	
	return (LRESULT)hti;
}

void COrderedTreeCtrl::PostNcDraw(CDC* pDC, const CRect& rWindow)
{
	// vertical divider
	if (m_crGridlines != CLR_NONE && GutterHasStyle(NCGS_RIGHTCOLUMNS) && !(GetStyle() & WS_VSCROLL))
	{
		pDC->FillSolidRect(rWindow.left, rWindow.top, 1, rWindow.Height(), m_crGridlines);
	}
}

CString COrderedTreeCtrl::FormatPosition(DWORD dwItem, DWORD dwParentItem, int nLevel, int nPos)
{
	// extract the parent pos
	CString sPos, sParentPos; 
	static CMap<DWORD, DWORD, CString, LPCTSTR> mapParentPos;
	
	HTREEITEM hti = (HTREEITEM)dwItem;

	if (dwParentItem)
		VERIFY(mapParentPos.Lookup(dwParentItem, sParentPos));
	
	BOOL bHasChildren = ItemHasChildren(hti);
	
	if (nPos == -1) // it means we have to figure it out for ourselves
		nPos = TCH().GetItemPos(hti, (HTREEITEM)dwParentItem);
	
	if (nLevel == -1) // likewise
		nLevel = TCH().GetItemLevel(hti);
	
	// default
	if (sParentPos.IsEmpty())
		sPos.Format(_T("%d"), nPos);
	else
		sPos.Format(_T("%s.%d"), sParentPos, nPos);
	
	// add to map for our children
	if (bHasChildren)
		mapParentPos[dwItem] = sPos;
	
	// modify for actual output
	if (bHasChildren && (TCH().IsItemExpanded(hti) == FALSE))
		sPos += "...";
	
	else if (nLevel == 0)
		sPos += ".";
	
	return sPos;
}

void COrderedTreeCtrl::NcDrawItemColumn(CDC* pDC, DWORD dwItem, DWORD dwParentItem, UINT nColID, CRect& rItem, 
								  int nLevel, int nPos, /*BOOL bSelected,*/ const CRect& rWindow)
{
	if (nColID == OTC_POSCOLUMNID) // this is all we deal with
	{
		CString sPos = FormatPosition(dwItem, dwParentItem, nLevel, nPos);

		if (CRect().IntersectRect(rItem, rWindow)) // something to see
		{
			rItem.left += NCG_COLPADDING;

			UINT nFlags = (DT_SINGLELINE | DT_VCENTER | DT_LEFT | GraphicsMisc::GetRTLDrawTextFlags(*this));
			pDC->DrawText(sPos, rItem, nFlags);
			
			// vertical divider
			if (m_crGridlines != CLR_NONE)
				pDC->FillSolidRect(rItem.right - 1, rItem.top, 1, rItem.Height(), m_crGridlines);
		}
	}
}

void COrderedTreeCtrl::PostNcDrawItem(CDC* pDC, DWORD /*dwItem*/, const CRect& rItem, int /*nLevel*/)
{
	// horizontal gridline
	if (m_crGridlines != CLR_NONE)
		pDC->FillSolidRect(rItem.left, rItem.bottom - 1, rItem.Width(), 1, m_crGridlines);
}

BOOL COrderedTreeCtrl::OnItemexpanding(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	SetRedraw(FALSE);

	*pResult = 0;
	return FALSE;
}

BOOL COrderedTreeCtrl::OnItemexpanded(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	m_gutter.RecalcGutter();
	SetRedraw(TRUE);
	
	*pResult = 0;
	return FALSE;
}

BOOL COrderedTreeCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMCUSTOMDRAW* pNMCD = (NMCUSTOMDRAW*)pNMHDR;
	NMTVCUSTOMDRAW* pTVCD = (NMTVCUSTOMDRAW*)pNMCD;
	
	HTREEITEM hti = (HTREEITEM)pTVCD->nmcd.dwItemSpec;
	
	switch (pNMCD->dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult |= CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;	
		break;
	
	case CDDS_ITEMPREPAINT:
		{
			*pResult = CDRF_NOTIFYPOSTPAINT;

			// do the odd lines
			if ((m_crAltLines != CLR_NONE) && TCH().ItemLineIsOdd(hti))
			{
				CDC* pDC = CDC::FromHandle(pTVCD->nmcd.hdc);
				GraphicsMisc::FillItemRect(pDC, &pTVCD->nmcd.rc, m_crAltLines, GetSafeHwnd());
				
				pTVCD->clrTextBk = pTVCD->clrText = m_crAltLines;
				*pResult |= CDRF_NEWFONT;
			}

			// themed selection
			BOOL bThemedSel = ((hti == GetSelectedItem()) && CThemed::AreControlsThemed());
			
			if (bThemedSel)
			{
				pTVCD->clrTextBk = pTVCD->clrText = GetSysColor(COLOR_WINDOW);
				*pResult |= CDRF_NEWFONT;
			}
		}
		break;

	case CDDS_ITEMPOSTPAINT:
		{
			CDC* pDC = CDC::FromHandle(pNMCD->hdc);

			// horz gridline
			if (m_crGridlines != CLR_NONE)
				pDC->FillSolidRect(pNMCD->rc.left, pNMCD->rc.bottom - 1, pNMCD->rc.right - pNMCD->rc.left, 1, m_crGridlines);

			// draw selection background
			BOOL bThemedSel = ((hti == GetSelectedItem()) && CThemed::AreControlsThemed());

			if (bThemedSel)
			{
				BOOL bFocused = (::GetFocus() == GetSafeHwnd());
				BOOL bFullRow = (GetStyle() & TVS_FULLROWSELECT);
				DWORD dwFlags = GMIB_EXTENDRIGHT;

				if (bFullRow)
					dwFlags |= GMIB_EXTENDLEFT;
				
				// if the columns are on the right then
				// we don't want to draw the rounded end 
				// on the right so it looks continuous with the columns
				if (m_gutter.HasStyle(NCGS_RIGHTCOLUMNS))
					dwFlags |= GMIB_CLIPRIGHT;
				else
					dwFlags |= GMIB_CLIPLEFT;
				
				CRect rItem;
				GetItemRect(hti, rItem, TRUE);
				rItem.left += 2;
				
				GM_ITEMSTATE nState = (bFocused ? GMIS_SELECTED : GMIS_SELECTEDNOTFOCUSED);
				GraphicsMisc::DrawExplorerItemBkgnd(pDC, GetSafeHwnd(), nState, rItem, dwFlags);

				// draw text
				COLORREF crText = GraphicsMisc::GetExplorerItemTextColor(GetSysColor(COLOR_WINDOWTEXT), nState, dwFlags);

				pDC->SetTextColor(crText);
				pDC->SetBkMode(TRANSPARENT);
				pDC->DrawText(GetItemText(hti), rItem, (DT_SINGLELINE | DT_LEFT | DT_VCENTER));
				
				*pResult |= CDRF_SKIPDEFAULT;
			}
		}
	}
	
	return FALSE; // to continue routing
}

int COrderedTreeCtrl::RecalcColumnWidth(CDC* pDC, UINT nColID, UINT& nWidth)
{
	switch (nColID)
	{
	case OTC_POSCOLUMNID:
		if (m_bShowingPosColumn)
		{
			g_mapWidths.RemoveAll(); // to handle a font change
			HTREEITEM hti = GetChildItem(NULL);
			int nPos = 1;
			int nMaxWidth = 0;
			
			while (hti)
			{
				int nItemWidth = GetGutterWidth(hti, 0, nPos, pDC);
				nMaxWidth = max(nMaxWidth, nItemWidth);
				
				hti = CTreeCtrl::GetNextItem(hti, TVGN_NEXT);
				nPos++;
			}
			
			nWidth = max(nMaxWidth, MINGUTTER);
		}
		else
			nWidth = 0;

		//TRACE("RecalcColumnWidth width=%d\n", nWidth);
		
		// rebuild visible item map if there are other columns
		if ((m_crAltLines != CLR_NONE) && (m_bShowingPosColumn || m_gutter.GetColumnCount() > 1))
			TCH().BuildVisibleIndexMap();
		
		return TRUE;
	}
	
	return 0;
}

UINT COrderedTreeCtrl::GetGutterWidth(HTREEITEM hti, int nLevel, int nPos, CDC* pDC)
{
	UINT nMaxWidth = 0;
	
	if (GetItemState(hti, TVIS_EXPANDED) & TVIS_EXPANDED)
	{
		HTREEITEM htiChild = GetChildItem(hti);
		int nPosChild = 1;
		nLevel++;
		
		while (htiChild)
		{
			UINT nWidth = GetGutterWidth(htiChild, nLevel, nPosChild, pDC);
			nMaxWidth = max(nMaxWidth, nWidth);
			
			htiChild = CTreeCtrl::GetNextItem(htiChild, TVGN_NEXT);
			nPosChild++;
		}
	}
	else if (ItemHasChildren(hti)/* && ShowingEllipsis(nLevel)*/)
	{
		nMaxWidth = GetWidth(-1, pDC); // ellipsis
	}
	
	return nMaxWidth + GetWidth(nPos, pDC);
}

BOOL COrderedTreeCtrl::OnClick(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	// make sure the selection is correctly set
	CPoint point(::GetMessagePos());
	ScreenToClient(&point);
	
	HTREEITEM htiHit = HitTest(point);
	
	if (htiHit && htiHit != GetSelectedItem())
		SelectItem(htiHit);
	
	*pResult = 0;
	
	return FALSE; // to continue routing
}

void COrderedTreeCtrl::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMTREEVIEW* pNMTV = (NMTREEVIEW*)pNMHDR;
	SelectItem(pNMTV->itemNew.hItem);
	
	m_gutter.Redraw();
	*pResult = 0;
}

UINT COrderedTreeCtrl::GetWidth(int nNumber, CDC* pDC)
{
	UINT nWidth = 0;
	
	if (g_mapWidths.Lookup(nNumber, nWidth))
		return nWidth;
	
	if (nNumber >= 0)
	{
		CString sNumber;
		sNumber.Format(_T("%d."), nNumber);
		nWidth = pDC->GetTextExtent(sNumber).cx;
	}
	else if (nNumber == -1)
		nWidth = pDC->GetTextExtent(_T("...")).cx;
	
	g_mapWidths[nNumber] = nWidth;
	return nWidth;
}

LRESULT COrderedTreeCtrl::OnGutterNotifyItemClick(WPARAM /*wParam*/, LPARAM lParam)
{
	NCGITEMCLICK* pNGIC = (NCGITEMCLICK*)lParam;
	ASSERT (pNGIC);
	
	TCH().EndLabelEdit(FALSE); // always
	
	HTREEITEM hti = (HTREEITEM)pNGIC->dwItem;

	if (!hti)
		return TRUE; // eat it

	SelectItem(hti); // always
	
	switch (pNGIC->nMsgID)
	{
	case WM_NCLBUTTONDBLCLK:
	case WM_LBUTTONDBLCLK:
		if (hti && ItemHasChildren(hti))
		{
			// kill any label edit that might have been started by
			// the first button click
			TCH().EndLabelEdit(TRUE);
			
			// handle expand/collapse when double-clicking in the non-client area,
			// or when double clicking off the  tree item
			UINT nHitFlags = 0;
			HitTest(pNGIC->ptClick, &nHitFlags);

			BOOL bNcDblClk = (pNGIC->nMsgID == WM_NCLBUTTONDBLCLK);
			BOOL bOnTreeItem = (nHitFlags & (TVHT_ONITEMBUTTON | TVHT_ONITEMLABEL | TVHT_ONITEMICON));

			// full-row select is special case in so far as
			// the whole row is considered the tree item
			if (GetStyle() & TVS_FULLROWSELECT)
			{
				bOnTreeItem = !bNcDblClk;
			}
			
			if (bNcDblClk || !bOnTreeItem)
			{
				BOOL bExpanded = TCH().IsItemExpanded(hti);
				Expand(hti, bExpanded ? TVE_COLLAPSE : TVE_EXPAND);
				
				CRect rItem;
				TCH().GetClientRect(rItem, hti);
				InvalidateRect(rItem);
				
				return TRUE; // handled
			}
		}
		break;
	}
	
	return FALSE;
}

void COrderedTreeCtrl::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpSS)
{
	if (nStyleType == GWL_STYLE)
	{
		BOOL bButtonsOld = (lpSS->styleOld & TVS_HASBUTTONS);
		BOOL bButtonsNew = (lpSS->styleNew & TVS_HASBUTTONS);
		
		if (bButtonsOld != bButtonsNew)
		{
			RecalcGutter();
		}
		else if (bButtonsNew)
		{
			UINT uStyleOld = (lpSS->styleOld & (TVS_HASLINES | TVS_LINESATROOT));
			UINT uStyleNew = (lpSS->styleNew & (TVS_HASLINES | TVS_LINESATROOT));
			
			if (uStyleOld != uStyleNew)
				RecalcGutter();
		}
	}
	
	CTreeCtrl::OnStyleChanged(nStyleType, lpSS);
}


void COrderedTreeCtrl::OnSettingChange(UINT /*uFlags*/, LPCTSTR /*lpszSection*/) 
{
	// there is the strangest 'bug' under XP using the default 96DPI font
	// setting, such that allowing the treectrl to receive this message
	// results in corrupted expansion icons.
	
	// to cause the minimum sideeffects we check for XP, theming and whether
	// buttons are visible
	// nope. disable completed for now
	//	if (!((GetStyle() & TVS_HASBUTTONS) && CThemed(this).AreControlsThemed()))
	//		CTreeCtrl::OnSettingChange(uFlags, lpszSection);
}


