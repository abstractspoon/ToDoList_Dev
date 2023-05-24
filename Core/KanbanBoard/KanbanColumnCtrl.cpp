// KanbanListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KanbanColumnCtrl.h"
#include "Kanbanstatic.h"
#include "Kanbanenum.h"

#include "..\shared\graphicsMisc.h"
#include "..\shared\enstring.h"
#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\datehelper.h"
#include "..\shared\timehelper.h"
#include "..\shared\autoflag.h"
#include "..\shared\copywndcontents.h"
#include "..\shared\holdredraw.h"
#include "..\Shared\enimagelist.h"
#include "..\Shared\osversion.h"
#include "..\Shared\themed.h"

#include "..\3rdparty\XNamedColors.h"
#include "..\3rdparty\ColorDef.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef TVS_EX_DOUBLEBUFFER
#	define TVS_EX_DOUBLEBUFFER  0x0004
#endif

#ifndef TTM_ADJUSTRECT
#	define TTM_ADJUSTRECT       (WM_USER + 31)
#endif

#ifndef TVS_NOHSCROLL
#	define TVS_NOHSCROLL        0x8000
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef WM_THEMECHANGED
#	define WM_THEMECHANGED 0x031A
#endif

/////////////////////////////////////////////////////////////////////////////

enum // checkbox images 
{ 
	KLCC_NONE,
	KLCC_UNCHECKED, 
	KLCC_CHECKED, 
	KLCC_MIXED, 
};

enum // Icon images
{
	FLAG_SET,
	FLAG_UNSET,
	PIN_SET,
	PIN_UNSET,
	ICON_UNSET,
};

/////////////////////////////////////////////////////////////////////////////

const int MIN_LABEL_EDIT_WIDTH	= GraphicsMisc::ScaleByDPIFactor(200);
const int BAR_WIDTH				= GraphicsMisc::ScaleByDPIFactor(6);
const int LV_PADDING			= GraphicsMisc::ScaleByDPIFactor(3);
const int CHECKBOX_PADDING		= GraphicsMisc::ScaleByDPIFactor(1);
const int ATTRIB_INDENT			= GraphicsMisc::ScaleByDPIFactor(6);
const int TIP_PADDING			= GraphicsMisc::ScaleByDPIFactor(4);
const int DEF_IMAGE_SIZE		= GraphicsMisc::ScaleByDPIFactor(16);
const int LEVEL_INDENT			= GraphicsMisc::ScaleByDPIFactor(16);
const int MAX_DRAG_ITEM_WIDTH	= GraphicsMisc::ScaleByDPIFactor(200) + DEF_IMAGE_SIZE;
const int PIN_FLAG_IMAGE_SIZE	= GraphicsMisc::ScaleByDPIFactor(12);

const int IMAGE_PADDING			= 2;
const int BAR_PADDING			= 2;
const int ITEM_PADDING			= 1;
const int ITEM_BORDER			= 1;

const CRect TEXT_BORDER			= CRect(1, 1, 2, 0);
const CString NOFILELINK;

const COLORREF SHADOW_COLOR		= RGB(0xD8, 0xD8, 0xD8);

const BOOL SORT_1ABOVE2 = -1;
const BOOL SORT_2ABOVE1 = 1;

/////////////////////////////////////////////////////////////////////////////
// CKanbanListCtrlEx

CKanbanColumnCtrl::CKanbanColumnCtrl(const CKanbanItemMap& data, const KANBANCOLUMN& columnDef,
								CFontCache& fonts, const CDWordArray& aPriorityColors, 
								const CKanbanAttributeArray& aDisplayAttrib)
	:
	m_data(data),
	m_columnDef(columnDef),
	m_aDisplayAttrib(aDisplayAttrib),
	m_fonts(fonts),
	m_aPriorityColors(aPriorityColors),
	m_bSelected(FALSE),
	m_nItemTextHeight(-1),
	m_nItemTextBorder(-1),
	m_nNumTitleLines(2),
	m_nAttribLabelVisibility(KBCAL_LONG),
	m_bSavingToImage(FALSE),
	m_bDropTarget(FALSE),
	m_bDrawTaskFlags(FALSE),
	m_bDrawTaskFileLinks(FALSE),
	m_dwDisplay(0),
	m_dwOptions(0),
	m_crItemShadow(CLR_NONE),
	m_tch(*this)
{
}

CKanbanColumnCtrl::~CKanbanColumnCtrl()
{
}

IMPLEMENT_DYNAMIC(CKanbanColumnCtrl, CTreeCtrl);

BEGIN_MESSAGE_MAP(CKanbanColumnCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CKanbanListCtrlEx)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_MESSAGE(WM_THEMECHANGED, OnThemeChanged)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYDOWN()
	ON_WM_SETCURSOR()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_MOUSEWHEEL()
	ON_WM_CHAR()
	ON_NOTIFY(TTN_SHOW, 0, OnTooltipShow)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKanbanListCtrlEx message handlers

BOOL CKanbanColumnCtrl::Create(UINT nID, CWnd* pParentWnd)
{
	UINT nFlags = (WS_CHILD | WS_VISIBLE | LVS_REPORT | WS_TABSTOP |
				   TVS_NONEVENHEIGHT | TVS_SHOWSELALWAYS | 
				   TVS_FULLROWSELECT | TVS_NOTOOLTIPS | TVS_NOHSCROLL);

	return CTreeCtrl::Create(nFlags, CRect(0, 0, 0, 0), pParentWnd, nID);
}

int CKanbanColumnCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (Misc::HasFlag(m_dwOptions, KBCF_SHOWLABELTIPS))
		VERIFY(InitTooltip());

	m_ilIcons.Create(IDB_ICONS, 16, 1, colorMagenta);
	CEnImageList::ScaleByDPIFactor(m_ilIcons);

	CFont* pFont = m_fonts.GetFont();
	ASSERT(pFont);

	if (pFont)
	{
		SendMessage(WM_SETFONT, (WPARAM)pFont->GetSafeHandle());
	}
	else
	{
		m_nItemTextHeight = -1;
		OnDisplayAttributeChanged();
	}

	if (GraphicsMisc::InitCheckboxImageList(*this, m_ilCheckboxes, IDB_CHECKBOXES, 255))
		SetImageList(&m_ilCheckboxes, TVSIL_STATE);

	TreeView_SetExtendedStyle(GetSafeHwnd(), TVS_EX_DOUBLEBUFFER, TVS_EX_DOUBLEBUFFER);
	RefreshBkgndColor();

	return 0;
}

void CKanbanColumnCtrl::OnSetFocus(CWnd* pOldWnd)
{
	CTreeCtrl::OnSetFocus(pOldWnd);

	Invalidate(FALSE);
}

void CKanbanColumnCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CTreeCtrl::OnKillFocus(pNewWnd);

	Invalidate(FALSE);
}

BOOL CKanbanColumnCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_tooltip.Pop();

	// Two bugs in Windows 7
	if ((COSVersion() < OSV_WIN8) && (GetStyle() & WS_VSCROLL))
	{
		CRect rClient, rItem;
		GetClientRect(rClient);

		HTREEITEM htiLast = TCH().GetLastVisibleItem();
		CTreeCtrl::GetItemRect(htiLast, rItem, FALSE);

		BOOL bAtBottom = (rItem.bottom < rClient.bottom);

		// 1. Mouse-wheeling past the bottom of the tree causes
		//    considerable flickering
		if (bAtBottom)
		{
			// 2. Sometimes this also breaks mouse-wheeling up
			//    until the scrollbar is manually adjusted
			BOOL bScrollUp = (zDelta > 0);

			if (bScrollUp)
			{
				HTREEITEM hti = GetFirstVisibleItem();

				// Shift up 2 ITEMS to get past the 'problem zone'
				HTREEITEM htiPrev = GetPrevVisibleItem(hti);

				if (htiPrev)
				{
					hti = htiPrev;
					htiPrev = GetPrevVisibleItem(hti);
					
					SelectSetFirstVisible(htiPrev ? htiPrev : hti);
				}
			}

			return TRUE;
		}
	}

	return CTreeCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void CKanbanColumnCtrl::SetDropTarget(BOOL bTarget)
{
	if (bTarget != m_bDropTarget)
	{
		m_bDropTarget = bTarget;
		RefreshBkgndColor();
	}
}

LRESULT CKanbanColumnCtrl::OnThemeChanged(WPARAM /*wp*/, LPARAM /*lp*/)
{
	RefreshBkgndColor();
	return 0L;
}

void CKanbanColumnCtrl::RefreshBkgndColor()
{
	COLORREF crBack = GetSysColor(COLOR_WINDOW);

	if (m_columnDef.crBackground != CLR_NONE)
		crBack = m_columnDef.crBackground;

	if (m_bDropTarget)
	{
		if ((COSVersion() < OSV_VISTA) || !CThemed::IsAppThemed() || (m_columnDef.crBackground != CLR_NONE))
			crBack = GraphicsMisc::Darker(crBack, 0.1);
		else
			crBack = RGB(215, 240, 255);
	}
	
	TreeView_SetBkColor(*this, crBack);

	RecalcItemShadowColor();
}

void CKanbanColumnCtrl::SetBackgroundColor(COLORREF color)
{
	if (color == GetSysColor(COLOR_WINDOW))
		color = CLR_NONE;

	if (m_columnDef.crBackground != color)
	{
		m_columnDef.crBackground = color;
		RefreshBkgndColor();
	}
}

void CKanbanColumnCtrl::RecalcItemShadowColor()
{
	COLORREF crBack = TreeView_GetBkColor(*this);
	float fLum = (RGBX(crBack).Luminance() / 255.0f);

	const BYTE nDarkestGray = 80, nLightestGray = 216;
	const BYTE nGrayRange = (nLightestGray - nDarkestGray);

	BYTE btShadow = (BYTE)(nDarkestGray + (fLum * nGrayRange));

	m_crItemShadow = RGB(btShadow, btShadow, btShadow);
}

void CKanbanColumnCtrl::SetExcessColor(COLORREF /*color*/)
{
	// TODO
	ASSERT(0);
}

void CKanbanColumnCtrl::SetMaximumTaskCount(int /*nMaxTasks*/)
{
	// TODO
	ASSERT(0);
}

void CKanbanColumnCtrl::OnDisplayAttributeChanged()
{
	m_bDrawTaskFlags = (Misc::FindT(TDCA_FLAG, m_aDisplayAttrib) != -1);
	m_bDrawTaskFileLinks = (Misc::FindT(TDCA_FILELINK, m_aDisplayAttrib) != -1);

	RecalcItemLineHeight();
	RefreshItemLineHeights();
}

void CKanbanColumnCtrl::RefreshItemLineHeights()
{
	HTREEITEM hti = GetChildItem(NULL);

	while (hti)
	{
		RefreshItemLineHeights(hti);
		hti = GetNextSiblingItem(hti);
	}
}

void CKanbanColumnCtrl::RefreshItemLineHeights(HTREEITEM hti)
{
	ASSERT(hti);

	const KANBANITEM* pKI = m_data.GetItem(GetTaskID(hti));
	ASSERT(pKI);

	if (pKI)
	{
		int nNumLines = m_nNumTitleLines;
		nNumLines += GetItemDisplayAttributeCount(*pKI);

		TCH().SetItemIntegral(hti, nNumLines);
	}
}

int CKanbanColumnCtrl::GetItemDisplayAttributeCount(const KANBANITEM& ki) const
{
	int nCount = 0, nDisp = m_aDisplayAttrib.GetSize();

	while (nDisp--)
	{
		TDC_ATTRIBUTE nAttribID = m_aDisplayAttrib[nDisp];

		if (WantDisplayAttribute(nAttribID, &ki))
		{
			switch (nAttribID)
			{
			case TDCA_PARENT:
				nCount += max(1, ki.nLevel);
				break;

			case TDCA_FILELINK:
				nCount += max(1, ki.aFileLinks.GetSize());
				break;

			case TDCA_FLAG:
				break; // handled separately

			default:
				nCount++;
				break;
			}
		}
	}

	return nCount;
}

void CKanbanColumnCtrl::RefreshItemLineHeights(DWORD dwTaskID)
{
	HTREEITEM hti = TCH().FindItem(dwTaskID);

	if (hti)
		RefreshItemLineHeights(hti);
}

void CKanbanColumnCtrl::SetOptions(DWORD dwOptions)
{
	if (Misc::HasFlag(dwOptions, KBCF_COLORBARBYPRIORITY) && (m_aPriorityColors.GetSize() != 11))
	{
		ASSERT(0);
		Misc::ModifyFlags(dwOptions, KBCF_COLORBARBYPRIORITY);
	}

	if (dwOptions != m_dwOptions)
	{
		DWORD dwPrevOptions = m_dwOptions;
		m_dwOptions = dwOptions;

		if (GetSafeHwnd())
		{
			if (Misc::FlagHasChanged(KBCF_HIDEEMPTYATTRIBUTES, dwPrevOptions, m_dwOptions))
				RefreshItemLineHeights();
			else
				Invalidate(FALSE);

			if (Misc::HasFlag(m_dwOptions, KBCF_SHOWLABELTIPS) && !m_tooltip.GetSafeHwnd())
			{
				InitTooltip();
			}
			else if (!Misc::HasFlag(m_dwOptions, KBCF_SHOWLABELTIPS) && m_tooltip.GetSafeHwnd())
			{
				m_tooltip.DestroyWindow();
			}
		}
	}
}

void CKanbanColumnCtrl::SetAttributeLabelVisibility(KBC_ATTRIBLABELS nLabelVis)
{
	m_nAttribLabelVisibility = nLabelVis;

	if (GetSafeHwnd())
		Invalidate(TRUE);
}

void CKanbanColumnCtrl::SetSelected(BOOL bSelected)
{
	m_bSelected = bSelected;

	if (!bSelected)
		ClearSelection();
	else
		Invalidate();
}

int CKanbanColumnCtrl::CalcItemTitleTextHeight() const
{
	return (m_nNumTitleLines * m_nItemTextHeight);
}

HTREEITEM CKanbanColumnCtrl::AddTask(const KANBANITEM& ki)
{
	HTREEITEM hti = FindItem(ki.dwTaskID);

	if (hti)
	{
		ASSERT(m_columnDef.aAttribValues.GetSize() > 1);
		return hti;
	}

	hti = CTreeCtrl::InsertItem(TVIF_TEXT | TVIF_PARAM,
								LPSTR_TEXTCALLBACK,
								0,
								0,
								0,
								0,
								ki.dwTaskID,
								TVI_ROOT,
								TVI_LAST);
	ASSERT(hti);

	if (hti)
	{
		m_mapHTItems.AddItem(*this, hti);
		RefreshItemLineHeights(hti);
	}

	return hti;
}

CString CKanbanColumnCtrl::GetAttributeID() const 
{ 
	return m_columnDef.sAttribID; 
}

BOOL CKanbanColumnCtrl::IsTaskSelected(DWORD dwTaskID) const
{
	ASSERT(dwTaskID);

	if (m_bSavingToImage || !m_bSelected)
		return FALSE;
	
	return (Misc::FindT(dwTaskID, m_aSelTaskIDs) != -1);
}

BOOL CKanbanColumnCtrl::IsBacklog() const
{
	return ((m_columnDef.aAttribValues.GetSize() == 1) && (m_columnDef.aAttribValues[0].IsEmpty()));
}

int CKanbanColumnCtrl::GetAttributeValues(CStringArray& aValues) const 
{ 
	aValues.Copy(m_columnDef.aAttribValues); 
	return aValues.GetSize();
}

int CKanbanColumnCtrl::GetAttributeValueIDs(CStringArray& aValueIDs) const 
{ 
	aValueIDs.Copy(m_columnDef.aAttribValues); 
	Misc::MakeUpper(aValueIDs);

	return aValueIDs.GetSize();
}

CString CKanbanColumnCtrl::GetAttributeValue() const
{
	return Misc::FormatArray(m_columnDef.aAttribValues);
}

CString CKanbanColumnCtrl::GetAttributeValueID() const
{
	return Misc::ToUpper(GetAttributeValue());
}

BOOL CKanbanColumnCtrl::HasMultipleValues() const
{
	return (m_columnDef.aAttribValues.GetSize() > 1);
}

BOOL CKanbanColumnCtrl::HasAnyValues() const
{
	return (m_columnDef.aAttribValues.GetSize() > 0);
}

int CKanbanColumnCtrl::CalcAvailableAttributeWidth(int nColWidth) const
{
	if (nColWidth < 0)
	{
		CRect rClient;
		GetClientRect(rClient);
		nColWidth = rClient.Width();
	}
	else
	{
		nColWidth -= GetSystemMetrics(SM_CXVSCROLL);
	}

	int nAvailWidth = (nColWidth - (2 * LV_PADDING));

	if (HasOption(KBCF_SHOWTASKCOLORASBAR))
		nAvailWidth -= BAR_WIDTH;

	nAvailWidth -= (TEXT_BORDER.left + TEXT_BORDER.right);
	nAvailWidth -= ATTRIB_INDENT;

	return nAvailWidth;
}

void CKanbanColumnCtrl::DrawItemShadow(CDC* pDC, CRect& rItem) const
{
	ASSERT(m_crItemShadow > 0);

	int nSave = pDC->SaveDC();

	rItem.DeflateRect(0, 0, 1, 1);
	pDC->ExcludeClipRect(rItem);

	CRect rShadow(rItem);
	rShadow.DeflateRect(3, 3, -1, -1);

	GraphicsMisc::DrawRect(pDC, rShadow, m_crItemShadow);

	pDC->RestoreDC(nSave);
}

void CKanbanColumnCtrl::DrawItemBackground(CDC* pDC, const KANBANITEM* pKI, CRect& rItem) const
{
	DrawItemShadow(pDC, rItem);

	if (IsTaskSelected(pKI->dwTaskID))
	{
		BOOL bFocused = (::GetFocus() == *this);
		GM_ITEMSTATE nState = (bFocused ? GMIS_SELECTED : GMIS_SELECTEDNOTFOCUSED);

		GraphicsMisc::DrawExplorerItemSelection(pDC, GetSafeHwnd(), nState, rItem, GMIB_THEMECLASSIC);
	}
	else if (HasOption(KBCF_SHOWTASKCOLORASBAR))
	{
		COLORREF crFill = GetSysColor(COLOR_WINDOW);
		COLORREF crBorder = GetSysColor(COLOR_WINDOWFRAME);

		GraphicsMisc::DrawRect(pDC, rItem, crFill, crBorder);
	}
	else // use task's own colour
	{
		COLORREF crFill = pKI->GetFillColor(HasOption(KBCF_TASKTEXTCOLORISBKGND));
		COLORREF crBorder = pKI->GetBorderColor(HasOption(KBCF_TASKTEXTCOLORISBKGND));

		GraphicsMisc::DrawRect(pDC, rItem, crFill, crBorder);
	}

	rItem.DeflateRect(ITEM_BORDER, ITEM_BORDER);
}

void CKanbanColumnCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMTVCUSTOMDRAW* pTVCD = (NMTVCUSTOMDRAW*)pNMHDR;
	*pResult = CDRF_DODEFAULT;
	
	switch (pTVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
		
	case CDDS_ITEMPREPAINT:
		{
			DWORD dwTaskID = pTVCD->nmcd.lItemlParam;
			
			if (m_data.HasItem(dwTaskID))
			{
				HTREEITEM hti = (HTREEITEM)pTVCD->nmcd.dwItemSpec;
				CDC* pDC = CDC::FromHandle(pTVCD->nmcd.hdc);
		
				CRect rItem;
				GetItemRect(hti, rItem);
				rItem.DeflateRect(ITEM_PADDING, ITEM_PADDING);

				DrawItem(pDC, dwTaskID, rItem);
			}
			
			*pResult |= CDRF_SKIPDEFAULT;
		}
	}
}

void CKanbanColumnCtrl::DrawItem(CDC* pDC, DWORD dwTaskID, const CRect& rItem)
{
	const KANBANITEM* pKI = m_data.GetItem(dwTaskID);

	if (!pKI)
	{
		ASSERT(0);
		return;
	}

	// Checkbox
	CRect rBody(rItem);
	DrawItemCheckbox(pDC, pKI, rBody);

	// Background
	DrawItemBackground(pDC, pKI, rBody);

	// Bar affects everything else
	DrawItemBar(pDC, pKI, rBody);

	// Snapshot attribute rect which is not affected by the images
	CRect rAttributes(rBody);

	// Draw task icon, pin and flag icons
	DrawItemImages(pDC, pKI, rBody);

	// Item title
	BOOL bSelected = IsTaskSelected(dwTaskID);
	COLORREF crText = pKI->GetTextColor(bSelected, (HasOption(KBCF_TASKTEXTCOLORISBKGND) && !HasOption(KBCF_SHOWTASKCOLORASBAR)));

	DrawItemTitle(pDC, pKI, rBody, crText);

	// Rest of attributes
	rAttributes.top += CalcItemTitleTextHeight();

	COLORREF crOtherText = crText;

	if (!bSelected && !Misc::IsHighContrastActive() && !pKI->IsDone(TRUE))
		crOtherText = GraphicsMisc::Lighter(crText, 0.3);

	DrawItemAttributes(pDC, pKI, rAttributes, crOtherText);
}

void CKanbanColumnCtrl::DrawItemTitle(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText)
{
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(crText);

	CFont* pOldFont = NULL;
	DWORD dwFontFlags = 0;

	if (HasOption(KBCF_STRIKETHRUDONETASKS) && pKI->IsDone(FALSE))
		dwFontFlags |= GMFS_STRIKETHRU;

	if (pKI->dwParentID == 0)
		dwFontFlags |= GMFS_BOLD;

	if (dwFontFlags)
		pOldFont = pDC->SelectObject(m_fonts.GetFont(dwFontFlags));

	// first 'n' lines is the task title
	CRect rTitle(rItem);

	rTitle.bottom = (rTitle.top + CalcItemTitleTextHeight());
	rTitle.bottom = min(rTitle.bottom, rItem.bottom);
	rTitle.DeflateRect(TEXT_BORDER);

	int nFlags = (DT_LEFT | DT_BOTTOM | DT_END_ELLIPSIS | DT_NOPREFIX | DT_WORDBREAK); // multi-line for titles only

	pDC->DrawText(pKI->sTitle, rTitle, nFlags);

	if (pOldFont)
		pDC->SelectObject(pOldFont);
}

BOOL CKanbanColumnCtrl::WantDisplayAttribute(TDC_ATTRIBUTE nAttrib, const KANBANITEM* pKI) const
{
	ASSERT(Misc::FindT(nAttrib, m_aDisplayAttrib) != -1);

	if (HasOption(KBCF_HIDEEMPTYATTRIBUTES) && !pKI->HasAttributeDisplayValue(nAttrib))
		return FALSE;

	// If this attribute matches the tracked attribute then we are
	// necessarily part of a fixed-column setup, and we need to do 
	// some extra checks to see if we really do want to display it
	if (KANBANITEM::IsTrackableAttribute(nAttrib) &&
		(KANBANITEM::GetAttributeID(nAttrib) == GetAttributeID()) &&
		(m_columnDef.aAttribValues.GetSize() == 1))
	{
		return FALSE;
	}

	return TRUE;
}

void CKanbanColumnCtrl::DrawItemAttributes(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText)
{
	int nSave = pDC->SaveDC();

	pDC->SetBkMode(TRANSPARENT);
	pDC->IntersectClipRect(rItem);

	DWORD dwFontFlags = 0;

	if (HasOption(KBCF_STRIKETHRUDONETASKS) && pKI->IsDone(FALSE))
		pDC->SelectObject(m_fonts.GetFont(GMFS_STRIKETHRU));

	CRect rAttrib(rItem);
	rAttrib.DeflateRect(TEXT_BORDER);

	int nFlags = (DT_LEFT | DT_BOTTOM | DT_END_ELLIPSIS | DT_NOPREFIX);

	for (int nDisp = 0; nDisp < m_aDisplayAttrib.GetSize(); nDisp++)
	{
		TDC_ATTRIBUTE nAttrib = m_aDisplayAttrib[nDisp];

		if (WantDisplayAttribute(nAttrib, pKI))
		{
			switch (nAttrib)
			{
			case TDCA_PARENT:
				DrawItemParents(pDC, pKI, rAttrib, crText);
				break;

			case TDCA_FILELINK:
				DrawItemFileLinks(pDC, pKI, rAttrib, crText);
				break;

			case TDCA_FLAG:
				break; // handled elsewhere

			default:
				DrawAttribute(pDC, rAttrib, nAttrib, pKI->GetAttributeDisplayValue(nAttrib), nFlags, crText);
				break;
			}
		}
	}

	pDC->RestoreDC(nSave);
}

void CKanbanColumnCtrl::DrawItemParents(CDC* pDC, const KANBANITEM* pKI, CRect& rItem, COLORREF crText) const
{
	CRect rParent(rItem);

	// Draw label
	KBC_ATTRIBLABELS nLabelVis = (m_bSavingToImage ? KBCAL_LONG : m_nAttribLabelVisibility);
	CString sLabel = GetAttributeLabel(TDCA_PARENT, nLabelVis);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(crText);

	if (!sLabel.IsEmpty())
	{
		pDC->DrawText(sLabel, rItem, DT_LEFT | DT_NOPREFIX);
		rParent.left += pDC->GetTextExtent(sLabel).cx;
	}

	if (pKI->dwParentID)
	{
		CKanbanItemArray aParents;
		const KANBANITEM* pKIParent = m_data.GetParentItem(pKI);

		while (pKIParent)
		{
			aParents.Add(pKIParent);
			pKIParent = m_data.GetParentItem(pKIParent);
		}

		// Draw in reverse order
		int nFlags = (DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX);
		int nParent = aParents.GetSize();
		BOOL bFirstParent = TRUE;

		while (nParent--)
		{
			pKIParent = aParents[nParent];
			ASSERT(pKIParent);

			int iImageIndex = -1;
			HIMAGELIST hilTask = (HIMAGELIST)GetParent()->SendMessage(WM_KLCN_GETTASKICON, pKIParent->dwTaskID, (LPARAM)&iImageIndex);

			BOOL bHasIcon = (hilTask && (iImageIndex != -1));

			if (bHasIcon)
			{
				int nImageTop = rParent.top;

				if (DEF_IMAGE_SIZE < m_nItemTextHeight)
					nImageTop += ((m_nItemTextHeight - DEF_IMAGE_SIZE) / 2);

				ImageList_DrawEx(hilTask, iImageIndex, *pDC, rParent.left, nImageTop, 0, 0, 0, colorWhite, ILD_TRANSPARENT | ILD_BLEND50);
			}
			else if (bFirstParent)
			{
				// don't indent the first parent's text if it doesn't have an icon
				// Note: We modify the parent rect because we want this to also
				// impact all the following parents
				rParent.OffsetRect(-DEF_IMAGE_SIZE, 0);
			}

			CRect rText(rParent);
			rText.left += DEF_IMAGE_SIZE; // always set aside space for an icon

			if (bHasIcon)
				rText.left += IMAGE_PADDING;

			pDC->DrawText(pKIParent->sTitle, rText, nFlags);

			rParent.top += (m_nItemTextHeight + m_nItemTextBorder);
			rParent.left += DEF_IMAGE_SIZE; // indent at each level

			bFirstParent = FALSE;
		}
	}
	else if (!sLabel.IsEmpty())
	{
		rParent.top += (m_nItemTextHeight + m_nItemTextBorder);
	}

	rItem.top = rParent.top;
}


void CKanbanColumnCtrl::DrawItemFileLinks(CDC* pDC, const KANBANITEM* pKI, CRect& rItem, COLORREF crText) const
{
	CRect rLink(rItem);

	// Draw label
	KBC_ATTRIBLABELS nLabelVis = (m_bSavingToImage ? KBCAL_LONG : m_nAttribLabelVisibility);
	CString sLabel = GetAttributeLabel(TDCA_FILELINK, nLabelVis);

	if (!sLabel.IsEmpty())
	{
		pDC->DrawText(sLabel, rItem, DT_LEFT | DT_NOPREFIX);
		rLink.left += pDC->GetTextExtent(sLabel).cx;
	}

	if (pKI->aFileLinks.GetSize())
	{
		int nFlags = (DT_LEFT | DT_PATH_ELLIPSIS | DT_NOPREFIX);

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(GetSysColor(COLOR_HOTLIGHT));

		CFont* pOldFont = pDC->SelectObject(m_fonts.GetFont(GMFS_UNDERLINED));

		for (int nLink = 0; nLink < pKI->aFileLinks.GetSize(); nLink++)
		{
			CString sLink(pKI->aFileLinks[nLink]);

			// Get the shortest meaningful bit because of space constraints
			if (FileMisc::IsPath(sLink))
				sLink = FileMisc::GetFileNameFromPath(sLink);

			pDC->DrawText(sLink, rLink, nFlags);
			rLink.top += (m_nItemTextHeight + m_nItemTextBorder);
		}

		pDC->SelectObject(pOldFont);
		pDC->SetTextColor(crText);
	}
	else if (!sLabel.IsEmpty())
	{
		rLink.top += (m_nItemTextHeight + m_nItemTextBorder);
	}

	rItem.top = rLink.top;
}

BOOL CKanbanColumnCtrl::DrawTaskIcon(CDC* pDC, const KANBANITEM* pKI, const CRect& rIcon) const
{
	if (pKI->bHasIcon || pKI->bParent)
	{
		int iImageIndex = -1;
		HIMAGELIST hilTask = (HIMAGELIST)GetParent()->SendMessage(WM_KLCN_GETTASKICON, pKI->dwTaskID, (LPARAM)&iImageIndex);

		if (hilTask && (iImageIndex != -1))
		{
			DrawItemImage(pDC, rIcon, KBCI_ICON, TRUE, hilTask, iImageIndex);
			return TRUE;
		}
	}

	return FALSE;
}

void CKanbanColumnCtrl::DrawItemImages(CDC* pDC, const KANBANITEM* pKI, CRect& rItem) const
{
	CSaveDC sdc(pDC);
	CRect rClip(rItem), rIcon(rItem);

	rClip.DeflateRect(1, 1);
	pDC->IntersectClipRect(rClip);

	// Task icon
	rIcon.left += IMAGE_PADDING;
	rIcon.top += IMAGE_PADDING;

	BOOL bIconDrawn = DrawTaskIcon(pDC, pKI, rIcon);

	// Allow for rest of images being smaller than default image size
	rIcon.top -= IMAGE_PADDING;

	// Draw placeholder image if icon not drawn and not locked
	if (!pKI->bLocked && !bIconDrawn)
		DrawItemImage(pDC, rIcon, KBCI_ICON, FALSE);
	
	// Draw pin icon always
	rIcon.left = (rItem.right - PIN_FLAG_IMAGE_SIZE - IMAGE_PADDING);

	DrawItemImage(pDC, rIcon, KBCI_PIN, pKI->bPinned);
	rIcon.OffsetRect(0, PIN_FLAG_IMAGE_SIZE);

	// Draw flag icon if set or not locked
	if (m_bDrawTaskFlags && (pKI->bFlagged || !pKI->bLocked))
	{
		DrawItemImage(pDC, rIcon, KBCI_FLAG, pKI->bFlagged);
	}

	// Update available rect
	rItem.left += (IMAGE_PADDING + DEF_IMAGE_SIZE);
	rItem.right -= (IMAGE_PADDING + DEF_IMAGE_SIZE);
}

void CKanbanColumnCtrl::DrawItemImage(CDC* pDC, const CRect& rImage, KBC_IMAGETYPE nType, BOOL bSet, HIMAGELIST hIL, int nIndex) const
{
	switch (nType)
	{
	case KBCI_ICON:
		if (bSet)
		{
			ASSERT(hIL && (nIndex != -1));
		}
		else
		{
			hIL = m_ilIcons;
			nIndex = ICON_UNSET;
		}
		break;

	case KBCI_FLAG:
		hIL = m_ilIcons;
		nIndex = (bSet ? FLAG_SET : FLAG_UNSET);
		break;

	case KBCI_PIN:
		hIL = m_ilIcons;
		nIndex = (bSet ? PIN_SET : PIN_UNSET);
		break;

	default:
		ASSERT(0);
	}

	if (!m_bSavingToImage || bSet)
		ImageList_Draw(hIL, nIndex, *pDC, rImage.left, rImage.top, ILD_TRANSPARENT);
}

void CKanbanColumnCtrl::DrawItemBar(CDC* pDC, const KANBANITEM* pKI, CRect& rItem) const
{
	if (HasOption(KBCF_SHOWTASKCOLORASBAR))
	{
		// Don't draw for completed items but ensure same indentation
		CRect rBar(rItem);

		rBar.DeflateRect(BAR_PADDING, BAR_PADDING);
		rBar.right = (rBar.left + BAR_WIDTH);

		if (!pKI->IsDone(TRUE))
		{
			if (HasOption(KBCF_COLORBARBYPRIORITY))
			{
				int nPriority = pKI->GetPriority(m_dwOptions);

				if (nPriority != -2)
				{
					COLORREF crFill = m_aPriorityColors[min(nPriority, 10)];
					COLORREF crBorder = GraphicsMisc::Darker(crFill, 0.4);

					GraphicsMisc::DrawRect(pDC, rBar, crFill, crBorder);
				}
			}
			else if (pKI->HasColor())
			{
				COLORREF crFill = pKI->GetFillColor(TRUE);
				COLORREF crBorder = pKI->GetBorderColor(TRUE);

				GraphicsMisc::DrawRect(pDC, rBar, crFill, crBorder);
			}
		}

		rItem.left = rBar.right;
	}
}

void CKanbanColumnCtrl::DrawItemCheckbox(CDC* pDC, const KANBANITEM* pKI, CRect& rItem)
{
	CRect rCheckbox(rItem);

	if (GetItemCheckboxRect(rCheckbox))
	{
		int iImage = KLCC_UNCHECKED;

		if (pKI->IsDone(FALSE))
		{
			iImage = KLCC_CHECKED;
		}
		else if (pKI->bSomeSubtaskDone)
		{
			iImage = KLCC_MIXED;
		}

		m_ilCheckboxes.Draw(pDC, iImage, rCheckbox.TopLeft(), ILD_TRANSPARENT);

		rItem.left = (rCheckbox.right + CHECKBOX_PADDING);
	}
}

BOOL CKanbanColumnCtrl::GetItemCheckboxRect(HTREEITEM hti, CRect& rItem) const
{
	if (HasOption(KBCF_SHOWCOMPLETIONCHECKBOXES))
	{
		if (GetItemRect(hti, rItem))
			return GetItemCheckboxRect(rItem);
	}

	// else
	return FALSE;
}

BOOL CKanbanColumnCtrl::GetItemBounds(HTREEITEM hti, LPRECT lpRect) const
{
	return CTreeCtrl::GetItemRect(hti, lpRect, FALSE);
}

BOOL CKanbanColumnCtrl::GetItemRect(HTREEITEM hti, CRect& rItem) const
{
	if (!GetItemBounds(hti, rItem))
		return FALSE;

	rItem.left += CalcIndentation(hti);
	return TRUE;
}

int CKanbanColumnCtrl::CalcIndentation(HTREEITEM hti) const
{
	if (HasOption(KBCF_INDENTSUBTASKS) && !HasOption(KBCF_HIDEPARENTTASKS))
	{
		ASSERT(hti);
		ASSERT(HasOption(KBCF_SORTSUBTASTASKSBELOWPARENTS));

		// If we assume that the sort state is correct then we simply look up
		// the tree for the first parent having the same pin state as ourselves
		const KANBANITEM* pKI = m_data.GetItem(GetTaskID(hti));
		ASSERT(pKI);

		if (pKI && pKI->dwParentID)
		{
			BOOL bInheritedPin = m_data.CalcInheritedPinState(pKI);

			HTREEITEM htiPrev = GetNextItem(hti, TVGN_PREVIOUS);

			while (htiPrev)
			{
				const KANBANITEM* pKIPrev = m_data.GetItem(GetTaskID(htiPrev));
				ASSERT(pKIPrev);

				if (m_data.IsParent(pKIPrev, pKI))
				{
					if (pKIPrev->bPinned || !pKI->bPinned)
					{
						return (CalcIndentation(htiPrev) + LEVEL_INDENT);
					}
				}

				htiPrev = GetNextItem(htiPrev, TVGN_PREVIOUS);
			}
		}
	}

	return 0; // no indentation, first item or no parent
}

BOOL CKanbanColumnCtrl::GetItemCheckboxRect(CRect& rItem) const
{
	if (HasOption(KBCF_SHOWCOMPLETIONCHECKBOXES))
	{
		rItem.bottom = (rItem.top + GetItemHeight());

		rItem.DeflateRect(0, ((rItem.Height() - CEnImageList::GetImageSize(m_ilCheckboxes)) / 2));
		rItem.right = (rItem.left + rItem.Height());

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CKanbanColumnCtrl::GetItemLabelTextRect(HTREEITEM hti, CRect& rItem, BOOL bEdit) const
{
	if (!GetItemRect(hti, rItem))
		return FALSE;

	rItem.DeflateRect(1, 1); // border

	if (HasOption(KBCF_SHOWCOMPLETIONCHECKBOXES))
	{
		rItem.left += CEnImageList::GetImageSize(m_ilCheckboxes);
		rItem.left += CHECKBOX_PADDING;
	}

	rItem.DeflateRect((DEF_IMAGE_SIZE + IMAGE_PADDING), 0);

	if (HasOption(KBCF_SHOWTASKCOLORASBAR))
		rItem.left += (BAR_WIDTH + IMAGE_PADDING);

	if (!bEdit)
	{
		rItem.DeflateRect(TEXT_BORDER);
		rItem.bottom = (rItem.top + CalcItemTitleTextHeight());
		rItem.right -= 2;
	}
	else
	{
		// make width of list column or 200 whichever is larger
		int nWidth = max(rItem.Width(), MIN_LABEL_EDIT_WIDTH);
			
		rItem.right = rItem.left + nWidth;
		rItem.bottom = (rItem.top + m_nItemTextHeight + m_nItemTextBorder + 2);
	}

	return TRUE;
}

BOOL CKanbanColumnCtrl::GetItemTooltipRect(HTREEITEM hti, CRect& rTip) const
{
	if (!GetItemLabelTextRect(hti, rTip, FALSE))
	{
		ASSERT(0);
		return FALSE;
	}

	const KANBANITEM* pKI = m_data.GetItem(GetTaskID(hti));

	if (!pKI)
	{
		ASSERT(0);
		return FALSE;
	}

	HFONT hFont = m_fonts.GetHFont((pKI->dwParentID == 0) ? GMFS_BOLD : 0);

	int nWidth = rTip.Width();
	int nAvailHeight = CalcItemTitleTextHeight();
	
	CSize sizeText = GraphicsMisc::GetTextSize(pKI->sTitle, GetSafeHwnd(), nWidth, hFont);

	if ((sizeText.cx <= nWidth) && (sizeText.cy <= nAvailHeight))
		return FALSE;

	int nIncrement = (nWidth / 2);

	while ((sizeText.cx > nWidth) || (sizeText.cy > nAvailHeight))
	{
		nWidth += nIncrement;
		sizeText = GraphicsMisc::GetTextSize(pKI->sTitle, GetSafeHwnd(), nWidth, hFont);
	}

	rTip.right = (rTip.left + sizeText.cx);

	m_tooltip.AdjustRect(rTip, TRUE);
	ClientToScreen(&rTip);

	return TRUE;
}

UINT CKanbanColumnCtrl::GetDisplayFormat(TDC_ATTRIBUTE nAttrib, BOOL bLong)
{
	switch (nAttrib)
	{
	case TDCA_ALLOCBY:		return (bLong ? IDS_DISPLAY_ALLOCBY : IDS_DISPLAY_ALLOCBY_SHORT);
	case TDCA_ALLOCTO:		return (bLong ? IDS_DISPLAY_ALLOCTO : IDS_DISPLAY_ALLOCTO_SHORT);
	case TDCA_CATEGORY:		return (bLong ? IDS_DISPLAY_CATEGORY : IDS_DISPLAY_CATEGORY_SHORT);
	case TDCA_COST:			return (bLong ? IDS_DISPLAY_COST : IDS_DISPLAY_COST_SHORT);
	case TDCA_CREATEDBY:	return (bLong ? IDS_DISPLAY_CREATEDBY : IDS_DISPLAY_CREATEDBY_SHORT);
	case TDCA_CREATIONDATE:	return (bLong ? IDS_DISPLAY_CREATEDATE : IDS_DISPLAY_CREATEDATE_SHORT);
	case TDCA_DONEDATE:		return (bLong ? IDS_DISPLAY_DONEDATE : IDS_DISPLAY_DONEDATE_SHORT);
	case TDCA_DUEDATE:		return (bLong ? IDS_DISPLAY_DUEDATE : IDS_DISPLAY_DUEDATE_SHORT);
	case TDCA_EXTERNALID:	return (bLong ? IDS_DISPLAY_EXTERNALID : IDS_DISPLAY_EXTERNALID_SHORT);
	case TDCA_FLAG:			return (bLong ? IDS_DISPLAY_FLAG : IDS_DISPLAY_FLAG_SHORT);
	case TDCA_FILELINK:		return (bLong ? IDS_DISPLAY_FILELINK : IDS_DISPLAY_FILELINK_SHORT);
	case TDCA_ID:			return (bLong ? IDS_DISPLAY_TASKID : IDS_DISPLAY_TASKID_SHORT);
	case TDCA_LASTMODDATE:	return (bLong ? IDS_DISPLAY_LASTMOD : IDS_DISPLAY_LASTMOD_SHORT);
	case TDCA_PARENT:		return (bLong ? IDS_DISPLAY_PARENT : IDS_DISPLAY_PARENT_SHORT);
	case TDCA_PERCENT:		return (bLong ? IDS_DISPLAY_PERCENT : IDS_DISPLAY_PERCENT_SHORT);
	case TDCA_PRIORITY:		return (bLong ? IDS_DISPLAY_PRIORITY : IDS_DISPLAY_PRIORITY_SHORT);
	case TDCA_RECURRENCE:	return (bLong ? IDS_DISPLAY_RECURRENCE : IDS_DISPLAY_RECURRENCE_SHORT);
	case TDCA_RISK:			return (bLong ? IDS_DISPLAY_RISK : IDS_DISPLAY_RISK_SHORT);
	case TDCA_STARTDATE:	return (bLong ? IDS_DISPLAY_STARTDATE : IDS_DISPLAY_STARTDATE_SHORT);
	case TDCA_STATUS:		return (bLong ? IDS_DISPLAY_STATUS : IDS_DISPLAY_STATUS_SHORT);
	case TDCA_TAGS:			return (bLong ? IDS_DISPLAY_TAGS : IDS_DISPLAY_TAGS_SHORT);
	case TDCA_TIMEESTIMATE:	return (bLong ? IDS_DISPLAY_TIMEEST : IDS_DISPLAY_TIMEEST_SHORT);
	case TDCA_TIMESPENT:	return (bLong ? IDS_DISPLAY_TIMESPENT : IDS_DISPLAY_TIMESPENT_SHORT);
	case TDCA_VERSION:		return (bLong ? IDS_DISPLAY_VERSION : IDS_DISPLAY_VERSION_SHORT);
	}

	ASSERT(0);
	return 0;
}

void CKanbanColumnCtrl::DrawAttribute(CDC* pDC, CRect& rLine, TDC_ATTRIBUTE nAttrib, const CString& sValue, int nFlags, COLORREF crText) const
{
	KBC_ATTRIBLABELS nLabelVis = (m_bSavingToImage ? KBCAL_LONG : m_nAttribLabelVisibility);
	CString sAttrib = FormatAttribute(nAttrib, sValue, nLabelVis);

	if (!sAttrib.IsEmpty())
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(crText);
		pDC->DrawText(sAttrib, rLine, nFlags);

		rLine.top += (m_nItemTextHeight + m_nItemTextBorder);
	}
}

CString CKanbanColumnCtrl::FormatAttribute(TDC_ATTRIBUTE nAttrib, const CString& sValue, KBC_ATTRIBLABELS nLabelVis)
{
	return GetAttributeLabel(nAttrib, nLabelVis) + sValue;
}

CString CKanbanColumnCtrl::GetAttributeLabel(TDC_ATTRIBUTE nAttrib, KBC_ATTRIBLABELS nLabelVis)
{
	UINT nFormatID = 0;

	if (nLabelVis != KBCAL_NONE)
		nFormatID = GetDisplayFormat(nAttrib, (nLabelVis == KBCAL_LONG));

	if (nFormatID == 0)
		return _T("");

	CEnString sLabel(nFormatID);
	sLabel += _T(": ");

	return sLabel;
}

BOOL CKanbanColumnCtrl::GetLabelEditRect(LPRECT pEdit)
{
	if (!m_bSelected || !GetCount() || (m_aSelTaskIDs.GetSize() != 1))
	{
		ASSERT(0);
		return FALSE;
	}

	HTREEITEM hti = FindItem(m_aSelTaskIDs[0]);
	ASSERT(hti);

	// scroll into view first
	EnsureVisible(hti);

	CRect rItem;

	if (GetItemLabelTextRect(hti, rItem, TRUE))
	{
		*pEdit = rItem;
		return TRUE;
	}

	return FALSE;
}

int CKanbanColumnCtrl::GetSelectedTaskIDs(CDWordArray& aTaskIDs) const
{
	if (m_bSelected)
		aTaskIDs.Copy(m_aSelTaskIDs);
	else
		aTaskIDs.RemoveAll();

	return aTaskIDs.GetSize();
}

int CKanbanColumnCtrl::GetSelectedCount() const
{
	if (m_bSelected)
		return m_aSelTaskIDs.GetSize();

	return 0;
}

HTREEITEM CKanbanColumnCtrl::GetFirstSelectedItem() const
{
	CHTIList lstHTI;

	if (BuildSortedSelection(lstHTI))
		return lstHTI.GetHead();

	return NULL;
}

HTREEITEM CKanbanColumnCtrl::GetLastSelectedItem() const
{
	CHTIList lstHTI;

	if (BuildSortedSelection(lstHTI))
		return lstHTI.GetTail();

	return NULL;
}

int CKanbanColumnCtrl::BuildSortedSelection(CHTIList& lstHTI) const
{
	lstHTI.RemoveAll();
	
	if (!m_bSelected || !m_aSelTaskIDs.GetSize())
		return 0;

	int nSel = m_aSelTaskIDs.GetSize();

	while (nSel--)
		lstHTI.AddTail(FindItem(m_aSelTaskIDs[nSel]));

	CKanbanColumnCtrl& tree = const_cast<CKanbanColumnCtrl&>(*this);

	CTreeSelectionHelper(tree).SortSelection(lstHTI, TRUE);

	return lstHTI.GetCount();
}

BOOL CKanbanColumnCtrl::SelectTasks(const CDWordArray& aTaskIDs)
{
	if (HasTasks(aTaskIDs))
		m_aSelTaskIDs.Copy(aTaskIDs);
	else
		m_aSelTaskIDs.RemoveAll();

	Invalidate(FALSE);

	return m_aSelTaskIDs.GetSize();
}

BOOL CKanbanColumnCtrl::SelectAll()
{
	if (!GetCount())
		return FALSE;

	m_aSelTaskIDs.RemoveAll();

	HTREEITEM hti = CTreeCtrl::GetChildItem(NULL);

	while (hti)
	{
		m_aSelTaskIDs.Add(GetTaskID(hti));
		hti = CTreeCtrl::GetNextItem(hti, TVGN_NEXT);
	}
	
	Invalidate(FALSE);
	return TRUE;
}

BOOL CKanbanColumnCtrl::HasTasks(const CDWordArray& aTaskIDs) const
{
	if (aTaskIDs.GetSize() == 0)
		return FALSE;

	int nID = aTaskIDs.GetSize();

	while (nID--)
	{
		if (!m_mapHTItems.HasItem(aTaskIDs[nID]))
			return FALSE;
	}

	return TRUE;
}

void CKanbanColumnCtrl::ScrollToSelection()
{
	if (m_bSelected && GetSelectedCount())
	{
		CRect rClient;
		GetClientRect(rClient);

		// Check for any selected item being completely visible
		int nID = GetSelectedCount();
		HTREEITEM htiPartial = NULL;

		while (nID--)
		{
			HTREEITEM hti = FindItem(m_aSelTaskIDs[nID]);
			
			if (hti)
			{
				CRect rItem;
				CTreeCtrl::GetItemRect(hti, rItem, FALSE);

				if ((rItem.top >= rClient.top) && (rItem.bottom <= rClient.bottom))
					return;

				// Keep track of any partially visible item
				if (htiPartial == NULL)
				{
					BOOL bNotVisible = ((rItem.bottom <= rClient.top) || (rItem.top >= rClient.bottom));

					if (!bNotVisible)
						htiPartial = hti;
				}
			}
		}

		// else just scroll to the first
		HTREEITEM hti = (htiPartial ? htiPartial : FindItem(m_aSelTaskIDs[0]));
		
		if (hti)
			TCH().EnsureItemVisible(hti, FALSE);
	}
}

void CKanbanColumnCtrl::ClearSelection()
{
	if (m_aSelTaskIDs.GetSize())
	{
		m_aSelTaskIDs.RemoveAll();
		Invalidate(FALSE);
	}
}

BOOL CKanbanColumnCtrl::SelectTask(DWORD dwTaskID)
{
	if (IsOnlySelectedTask(dwTaskID))
		return TRUE;

	ClearSelection();

	if (dwTaskID)
	{
		HTREEITEM hti = FindItem(dwTaskID);

		if (hti)
		{
			m_aSelTaskIDs.Add(dwTaskID);
			TCH().EnsureItemVisible(hti, FALSE);
		}
	}

	return m_aSelTaskIDs.GetSize();
}

BOOL CKanbanColumnCtrl::IsOnlySelectedTask(DWORD dwTaskID)
{
	return (dwTaskID && (dwTaskID == GetOnlySelectedTask()));
}

DWORD CKanbanColumnCtrl::GetOnlySelectedTask() const
{
	if (GetSelectedCount() != 1)
		return 0;

	return m_aSelTaskIDs[0];
}

HTREEITEM CKanbanColumnCtrl::FindItem(DWORD dwTaskID) const
{
	return m_mapHTItems.GetItem(dwTaskID);
}

HTREEITEM CKanbanColumnCtrl::HitTestItemSidebar(const CPoint& ptScreen) const
{
	CPoint ptClient(ptScreen);
	ScreenToClient(&ptClient);

	HTREEITEM htiHit = HitTest(ptClient);

	if (htiHit)
	{
		CRect rSidebar;
		GetItemBounds(htiHit, rSidebar);

		CRect rLabel;
		GetItemLabelTextRect(htiHit, rLabel, TRUE);

		rSidebar.left = (rLabel.left - (DEF_IMAGE_SIZE + IMAGE_PADDING));
		rSidebar.right = rLabel.left;

		if (HasOption(KBCF_SHOWTASKCOLORASBAR))
			rSidebar.left -= (BAR_WIDTH + IMAGE_PADDING);

		if (!rSidebar.PtInRect(ptClient))
			htiHit = NULL;
	}

	return htiHit;
}

HTREEITEM CKanbanColumnCtrl::FindItem(const IUISELECTTASK& select, BOOL bNext, HTREEITEM htiStart) const
{
	HTREEITEM htiNext = NULL;

	if (htiStart == NULL)
		htiNext = (bNext ? m_tch.GetFirstItem() : m_tch.GetLastItem());
	else
		htiNext = GetNextItem(htiStart, (bNext ? TVGN_NEXT : TVGN_PREVIOUS));

	while (htiNext)
	{
		DWORD dwTaskID = GetTaskID(htiNext);
		const KANBANITEM* pKI = m_data.GetItem(dwTaskID);

		if (pKI->MatchesAttribute(select))
		{
			return htiNext;
		}

		htiNext = GetNextItem(htiNext, (bNext ? TVGN_NEXT : TVGN_PREVIOUS));
	}

	return NULL; // no match
}

BOOL CKanbanColumnCtrl::DeleteTask(DWORD dwTaskID)
{
	HTREEITEM hti = FindItem(dwTaskID);

	if (hti && CTreeCtrl::DeleteItem(hti))
	{
		m_mapHTItems.RemoveKey(dwTaskID);
		return TRUE;
	}

	return FALSE;
}

BOOL CKanbanColumnCtrl::DeleteAll()
{
	m_mapHTItems.RemoveAll();
	m_aSelTaskIDs.RemoveAll();

	return CTreeCtrl::DeleteAllItems();
}

int CKanbanColumnCtrl::RemoveDeletedTasks(const CDWordSet& mapCurIDs)
{
	int nNumDeleted = 0;
	HTREEITEM hti = GetChildItem(NULL);

	while (hti)
	{
		DWORD dwTaskID = GetTaskID(hti);

		// get next item before deleting this one
		HTREEITEM htiNext = GetNextItem(hti, TVGN_NEXT);

		if (!mapCurIDs.Has(dwTaskID) && CTreeCtrl::DeleteItem(hti))
		{	
			m_mapHTItems.RemoveKey(dwTaskID);
			nNumDeleted++;
		}

		hti = htiNext;
	}

	ASSERT(m_mapHTItems.GetCount() == (int)GetCount());

	return nNumDeleted;
}

void CKanbanColumnCtrl::Sort(TDC_ATTRIBUTE nBy, BOOL bAscending)
{
	if (GetCount() < 2)
		return;

	CHoldRedraw hr(*this);
	KANBANSORT ks(m_data, m_mapHTItems);
	
	ks.nBy = nBy;
	ks.bAscending = bAscending;
	ks.dwOptions = m_dwOptions;

	switch (nBy)
	{
	case TDCA_STATUS:
	case TDCA_ALLOCTO:
	case TDCA_CATEGORY:
	case TDCA_ALLOCBY:
	case TDCA_TAGS:
	case TDCA_RISK:
	case TDCA_PRIORITY:
	case TDCA_VERSION:
		ks.sAttribID = KANBANITEM::GetAttributeID(nBy);
		break;
	}

	TVSORTCB tvs = { NULL, SortProc, (LPARAM)&ks };

	SortChildrenCB(&tvs);
	ScrollToSelection();
}

int CALLBACK CKanbanColumnCtrl::SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	const KANBANSORT* pSort = (KANBANSORT*)lParamSort;
	
	const KANBANITEM* pKI1 = pSort->data.GetItem(lParam1);
	const KANBANITEM* pKI2 = pSort->data.GetItem(lParam2);

	if (!pKI1 || !pKI2)
		return 0;

	BOOL bPinned1 = pKI1->bPinned;
	BOOL bPinned2 = pKI2->bPinned;

	if (pSort->HasOption(KBCF_SORTSUBTASTASKSBELOWPARENTS) &&
		!pSort->HasOption(KBCF_HIDEPARENTTASKS) &&
		!pSort->data.HasSameParent(pKI1, pKI2))
	{
		BOOL bAggregatePinned1 = pSort->data.CalcInheritedPinState(pKI1);
		BOOL bAggregatePinned2 = pSort->data.CalcInheritedPinState(pKI2);

		// If one is the parent of another always sort below
		// unless the child is pinned and the parent not
		if (pSort->data.IsParent(pKI2, pKI1))
		{
			if (bAggregatePinned1 && !bAggregatePinned2)
				return SORT_1ABOVE2; // child above parent
			else
				return SORT_2ABOVE1; // parent above child
		}

		if (pSort->data.IsParent(pKI1, pKI2))
		{
			if (bAggregatePinned2 && !bAggregatePinned1)
				return SORT_2ABOVE1; // child above parent
			else
				return SORT_1ABOVE2; // parent above child
		}

		// We can't sort items that are not in the same 
		// branch of the tree ie. they need to have the same parent
		const KANBANITEM* pKITemp1 = pKI1;
		const KANBANITEM* pKITemp2 = pKI2;

		// First we raise the items to the same level
		if (pKI1->nLevel > pKI2->nLevel)
		{
			while (pKITemp1->nLevel > pKITemp2->nLevel)
				pKITemp1 = pSort->data.GetParentItem(pKITemp1);
		}
		else if (pKI2->nLevel > pKI1->nLevel)
		{
			while (pKITemp2->nLevel > pKITemp1->nLevel)
				pKITemp2 = pSort->data.GetParentItem(pKITemp2);
		}
		ASSERT(pKITemp1 && pKITemp2);

		// Then we raise them to have the same parent
		while (!pSort->data.HasSameParent(pKITemp1, pKITemp2))
		{
			pKITemp1 = pSort->data.GetParentItem(pKITemp1);
			pKITemp2 = pSort->data.GetParentItem(pKITemp2);
		}
		ASSERT(pKITemp1 && pKITemp2);

		// And both parents must exist in this tree
		if (pSort->items.HasItem(pKITemp1->dwTaskID) &&
			pSort->items.HasItem(pKITemp2->dwTaskID))
		{
			pKI1 = pKITemp1;
			pKI2 = pKITemp2;

			bPinned1 = bAggregatePinned1;
			bPinned2 = bAggregatePinned2;
		}
	}

	// Pinned tasks always above unpinned
	if (bPinned1 && !bPinned2)
		return SORT_1ABOVE2;

	if (!bPinned1 && bPinned2)
		return SORT_2ABOVE1;

	// else
	return CompareAttributeValues(pKI1, pKI2, *pSort);
}

int CKanbanColumnCtrl::CompareAttributeValues(const KANBANITEM* pKI1, const KANBANITEM* pKI2, const KANBANSORT& sort)
{
	int nCompare = 0;

	switch (sort.nBy)
	{
	case TDCA_NONE:
		return Misc::NaturalCompare(pKI1->sFullPosition, pKI2->sFullPosition);

	case TDCA_TASKNAME:
		nCompare = Misc::NaturalCompare(pKI1->sTitle, pKI2->sTitle);
		break;

	case TDCA_ALLOCBY:
	case TDCA_ALLOCTO:
	case TDCA_CATEGORY:
	case TDCA_STATUS:
	case TDCA_TAGS:
	case TDCA_VERSION:
		{
			ASSERT(!sort.sAttribID.IsEmpty());

			CString sValue1 = pKI1->GetAttributeDisplayValue(sort.nBy);
			CString sValue2 = pKI2->GetAttributeDisplayValue(sort.nBy);

			nCompare = Misc::NaturalCompare(sValue1, sValue2);
		}
		break;

	case TDCA_PRIORITY:
		{
			ASSERT(!sort.sAttribID.IsEmpty());

			int nPriority1 = pKI1->GetPriority(sort.dwOptions);
			int nPriority2 = pKI2->GetPriority(sort.dwOptions);

			nCompare = Misc::CompareNumT(nPriority1, nPriority2);
		}
		break;

	case TDCA_RISK:
		{
			ASSERT(!sort.sAttribID.IsEmpty());

			int nRisk1 = pKI1->GetRisk(sort.dwOptions);
			int nRisk2 = pKI2->GetRisk(sort.dwOptions);

			nCompare = Misc::CompareNumT(nRisk1, nRisk2);
		}
		break;

	case TDCA_CUSTOMATTRIB:
		// TODO
		break;

		// Other display attributes
	case TDCA_COST:
		nCompare = Misc::CompareNumT(pKI1->dCost, pKI2->dCost);
		break;

	case TDCA_CREATIONDATE:
		nCompare = CDateHelper::Compare(pKI1->dtCreate, pKI2->dtCreate, DHC_COMPARETIME);
		break;

	case TDCA_CREATEDBY:
		nCompare = Misc::NaturalCompare(pKI1->sCreatedBy, pKI2->sCreatedBy);
		break;

	case TDCA_DONEDATE:
		nCompare = CDateHelper::Compare(pKI1->dtDone, pKI2->dtDone, (DHC_COMPARETIME | DHC_NOTIMEISENDOFDAY));
		break;

	case TDCA_DUEDATE:
		nCompare = CDateHelper::Compare(pKI1->dtDue, pKI2->dtDue, (DHC_COMPARETIME | DHC_NOTIMEISENDOFDAY));
		break;

	case TDCA_EXTERNALID:
		nCompare = Misc::NaturalCompare(pKI1->sExternalID, pKI2->sExternalID);
		break;

	case TDCA_FLAG:
		nCompare = Misc::CompareNumT(pKI1->bFlagged, pKI2->bFlagged);
		break;

	case TDCA_LASTMODDATE:
		nCompare = CDateHelper::Compare(pKI1->dtLastMod, pKI2->dtLastMod, DHC_COMPARETIME);
		break;

	case TDCA_PERCENT:
		nCompare = Misc::CompareNumT(pKI1->nPercent, pKI2->nPercent);
		break;

	case TDCA_RECURRENCE:
		nCompare = Misc::NaturalCompare(pKI1->sRecurrence, pKI2->sRecurrence);
		break;

	case TDCA_STARTDATE:
		nCompare = CDateHelper::Compare(pKI1->dtStart, pKI2->dtStart, DHC_COMPARETIME);
		break;

	case TDCA_TIMEESTIMATE:
		nCompare = CTimeHelper().Compare(pKI1->dTimeEst, MapUnitsToTHUnits(pKI1->nTimeEstUnits),
										 pKI2->dTimeEst, MapUnitsToTHUnits(pKI2->nTimeEstUnits));
		break;

	case TDCA_TIMESPENT:
		nCompare = CTimeHelper().Compare(pKI1->dTimeSpent, MapUnitsToTHUnits(pKI1->nTimeSpentUnits),
										 pKI2->dTimeSpent, MapUnitsToTHUnits(pKI2->nTimeSpentUnits));
		break;

	default:
		ASSERT(0);
		break;
	}

	// In the absence of a result we sort by POSITION to ensure a stable sort, 
	// but without reversing the sign. This also handles sorting by 'TDCA_NONE'
	if ((nCompare == 0) && sort.data.HasSameParent(pKI1, pKI2))
	{
		return Misc::CompareNumT(pKI1->nPosition, pKI2->nPosition);
	}

	if (sort.bAscending)
		return nCompare;
	
	// else
	return -nCompare;
}

void CKanbanColumnCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	HTREEITEM htiUnused = NULL;
	HandleButtonClick(point, FALSE, htiUnused);
	
	CTreeCtrl::OnRButtonDown(nFlags, point);
}

void CKanbanColumnCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Note: The complexity of this function arises out of the 
	// indentation of subtasks, because we want clicking to the
	// left of the task to select the task BUT NOT begin a 
	// drag operation or a label edit. 
	DWORD dwPrevOnlyTaskID = GetOnlySelectedTask();

	HTREEITEM htiHit = NULL;
	BOOL bHandled = HandleButtonClick(point, TRUE, htiHit);

	DWORD dwHitTaskID = GetTaskID(htiHit);

	if (dwHitTaskID)
	{
		const KANBANITEM* pKI = m_data.GetItem(dwHitTaskID);
		ASSERT(pKI);

		UINT nSetMsgID = 0;
		BOOL bSet = FALSE;

		if (HitTestCheckbox(htiHit, point))
		{
			nSetMsgID = WM_KLCN_EDITTASKDONE;
			bSet = !pKI->bDone;
		}
		else
		{
			KBC_IMAGETYPE nImage = HitTestImage(htiHit, point);

			if (nImage != KBCI_NONE)
			{
				switch (nImage)
				{
				case KBCI_ICON:
					if (!pKI->bLocked)
						nSetMsgID = WM_KLCN_EDITTASKICON;
					break;

				case KBCI_FLAG:
					if (!pKI->bLocked)
					{
						nSetMsgID = WM_KLCN_EDITTASKFLAG;
						bSet = !pKI->bFlagged;
					}
					break;

				case KBCI_PIN:
					nSetMsgID = WM_KLCN_EDITTASKPIN;
					bSet = !pKI->bPinned;
					break;
				}
			}
			else if (m_bDrawTaskFileLinks)
			{
				CString sFileLink = HitTestFileLink(htiHit, point);

				if (!sFileLink.IsEmpty())
				{
					GetParent()->PostMessage(WM_KLCN_SHOWFILELINK, (WPARAM)GetSafeHwnd(), (LPARAM)(LPCTSTR)sFileLink);
					return;
				}
			}
		}

		if (nSetMsgID)
		{
			// Post message to give mouse-click time to complete
			GetParent()->PostMessage(nSetMsgID, (WPARAM)GetSafeHwnd(), bSet);
			Invalidate(FALSE);

			bHandled = TRUE;
		}
		else if (bHandled)
		{
			// Perform drag if the point is to the right of any indent
			// because we won't be calling the base class handler
			CRect rIndentedItem;
			GetItemLabelTextRect(htiHit, rIndentedItem, FALSE);

			if (point.x > rIndentedItem.left)
			{
				if (::DragDetect(*this, point))
				{
					TRACE(_T("CKanbanColumnCtrl::OnLButtonDown(Faking drag start)\n"));

					NMTREEVIEW nmtv = { *this, (UINT)GetDlgCtrlID(), TVN_BEGINDRAG, 0 };

					nmtv.itemNew.hItem = htiHit;
					nmtv.itemNew.lParam = dwHitTaskID;
					nmtv.ptDrag = point;

					GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmtv);
				}
			}
		}
	}
	
	if (!bHandled)
		CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CKanbanColumnCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	HTREEITEM htiHit = NULL;
	BOOL bHandled = HandleButtonClick(point, TRUE, htiHit);

	if (htiHit && 
		(m_aSelTaskIDs.GetSize() == 1) && 
		(HitTestImage(htiHit, point) == KBCI_NONE))
	{
		CRect rIndentedItem;
		GetItemLabelTextRect(htiHit, rIndentedItem, FALSE);

		if (point.x > rIndentedItem.left)
		{
			bHandled = TRUE;
			GetParent()->PostMessage(WM_KLCN_EDITTASKLABEL, (WPARAM)GetSafeHwnd(), GetTaskID(htiHit));
		}
	}

	if (!bHandled)
		CTreeCtrl::OnLButtonDblClk(nFlags, point);
}

BOOL CKanbanColumnCtrl::HandleExtendedSelection(HTREEITEM htiSelected)
{
	DWORD dwTaskID = GetTaskID(htiSelected);

	if (Misc::IsKeyPressed(VK_SHIFT))
	{
		// select all items between first item (anchor) and clicked item
		if (m_aSelTaskIDs.GetSize())
		{
			DWORD dwAnchorID = m_aSelTaskIDs[0];

			HTREEITEM htiAnchor = FindItem(dwAnchorID);
			ASSERT(htiAnchor);

			CTreeSelectionHelper tsh(*this);
			tsh.SetItems(htiAnchor, htiSelected, TSHS_SELECT, FALSE);

			// If control is pressed we append these to the current
			// selection else we overwrite the current selection
			if (Misc::IsKeyPressed(VK_CONTROL))
			{
				CDWordArray aNewTaskIDs;
				tsh.GetItemData(aNewTaskIDs);

				Misc::AddUniqueItems(aNewTaskIDs, m_aSelTaskIDs);
			}
			else
			{
				tsh.GetItemData(m_aSelTaskIDs);
			}

			// Move the anchor to the head
			Misc::RemoveItemT(dwAnchorID, m_aSelTaskIDs);
			m_aSelTaskIDs.InsertAt(0, dwAnchorID);
		}
		else
		{
			SelectTask(dwTaskID);
		}

		return TRUE;
	}
	
	// Note: This could be the start of a multiple item drag
	if (Misc::IsKeyPressed(VK_CONTROL))
	{
		if (Misc::FindT(dwTaskID, m_aSelTaskIDs) == -1)
		{
			m_aSelTaskIDs.InsertAt(0, dwTaskID); // new anchor
		}
		else
		{
			CPoint point = ::GetMessagePos();
			ScreenToClient(&point);
			
			if (!::DragDetect(*this, point))
				Misc::RemoveItemT(dwTaskID, m_aSelTaskIDs);
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CKanbanColumnCtrl::HandleButtonClick(CPoint point, BOOL bLeftBtn, HTREEITEM& htiHit)
{
	BOOL bHandled = FALSE;

	htiHit = HitTest(point);

	if (!htiHit)
	{
		// Select the column if right-clicking
		if (!bLeftBtn && !m_bSelected)
		{
			htiHit = GetChildItem(NULL);

			if (htiHit)
				SelectItem(htiHit, TRUE);
			else
				NotifyParentSelectionChange(htiHit, TRUE);
		}
		else if (m_bSelected)
		{
			SetFocus();
		}

		bHandled = TRUE;
	}
	else
	{
		bHandled = !m_bSelected;
		SetFocus();

		if (HandleExtendedSelection(htiHit))
		{
			bHandled = TRUE;
		}
		else
		{
			DWORD dwTaskID = GetTaskID(htiHit);

			if (!IsTaskSelected(dwTaskID))
			{
				SelectTask(dwTaskID);
				bHandled = TRUE;
			}
			else
			{
				// We need to be careful here not to clear
				// an existing multiple selection which the 
				// user intends either to drag or to edit
				BOOL bSameTask = IsOnlySelectedTask(dwTaskID);
				BOOL bWantEdit = (!bLeftBtn ||
								  HitTestCheckbox(htiHit, point) || 
								  (HitTestImage(htiHit, point) != KBCI_NONE));

				if (!bSameTask && !bWantEdit && !::DragDetect(*this, point))
				{
					SelectTask(dwTaskID);
					bHandled = TRUE;
				}
			}
		}

		if (bHandled)
		{
			NotifyParentSelectionChange(htiHit, TRUE);
			Invalidate(FALSE);
		}
	}

	return bHandled;
}

void CKanbanColumnCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_ESCAPE:
		// prevent beep
		return;
	}

	CTreeCtrl::OnChar(nChar, nRepCnt, nFlags);
}

void CKanbanColumnCtrl::OnKeyDown(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
	HTREEITEM htiSel = NULL, htiNext = NULL;

	switch (nChar)
	{
	case VK_UP:
		htiSel = GetFirstSelectedItem();
		htiNext = GetPrevVisibleItem(htiSel);
		break;

	case VK_DOWN:
		htiSel = GetLastSelectedItem();
		htiNext = GetNextVisibleItem(htiSel);
		break;

	case VK_PRIOR:
		htiSel = GetFirstSelectedItem();
		htiNext = TCH().GetPrevPageVisibleItem(htiSel);
		break;

	case VK_NEXT:
		htiSel = GetLastSelectedItem();
		htiNext = TCH().GetNextPageVisibleItem(htiSel);
		break;

	case VK_HOME:
		htiNext = TCH().GetFirstItem();
		break;

	case VK_END:
		htiNext = TCH().GetLastItem();
		break;
	}

	if (!htiNext)
		htiNext = htiSel;

	if (htiNext)
	{
		if (HandleExtendedSelection(htiNext) || SelectItem(htiNext, FALSE))
		{
			NotifyParentSelectionChange(htiNext, FALSE);
			Invalidate(FALSE);
		}
	}
}

KBC_IMAGETYPE CKanbanColumnCtrl::HitTestImage(HTREEITEM hti, CPoint point) const
{
	CRect rText;
	GetItemLabelTextRect(hti, rText);

	// Task Icon
	CRect rIcon(rText);
	rIcon.right = (rIcon.left - IMAGE_PADDING);
	rIcon.left = (rIcon.right - DEF_IMAGE_SIZE);
	rIcon.bottom = (rIcon.top + DEF_IMAGE_SIZE);

	if (rIcon.PtInRect(point))
		return KBCI_ICON;

	// Pin
	rIcon = rText;
	rIcon.left = (rIcon.right + IMAGE_PADDING);
	rIcon.right = (rIcon.left + DEF_IMAGE_SIZE);
	rIcon.bottom = (rIcon.top + PIN_FLAG_IMAGE_SIZE);

	if (rIcon.PtInRect(point))
		return KBCI_PIN;

	// Flag
	if (m_bDrawTaskFlags)
	{
		rIcon.OffsetRect(0, DEF_IMAGE_SIZE);

		if (rIcon.PtInRect(point))
			return KBCI_FLAG;
	}

	// all else
	return KBCI_NONE;
}

CString CKanbanColumnCtrl::HitTestFileLink(HTREEITEM hti, CPoint point) const
{
	if (m_bDrawTaskFileLinks)
	{
		const KANBANITEM* pKI = m_data.GetItem(GetTaskID(hti));
		ASSERT(pKI);

		int nNumLinks = pKI->aFileLinks.GetSize();

		if (nNumLinks)
		{
			CRect rLinks;
			GetItemLabelTextRect(hti, rLinks, FALSE);

			// Undo the indent for the icon
			rLinks.left -= (DEF_IMAGE_SIZE + IMAGE_PADDING);

			// Work down from the top until either we pass the 
			// point in question or we reach the file link field,
			// doing as little work as possible
			int nItemHeight = GetItemHeight();
			rLinks.top = rLinks.bottom;

			for (int nDisp = 0; nDisp < m_aDisplayAttrib.GetSize(); nDisp++)
			{
				// We can stop as soon as pass below the specified point
				if (point.y < rLinks.top)
					return NOFILELINK;

				TDC_ATTRIBUTE nAttribID = m_aDisplayAttrib[nDisp];

				if (nAttribID == TDCA_FILELINK)
				{
					rLinks.bottom = (rLinks.top + (nNumLinks * nItemHeight));
					break;
				}

				// else next attrib
				if (WantDisplayAttribute(nAttribID, pKI))
					rLinks.top += nItemHeight;
			}

			if (point.y < rLinks.bottom) // within the right vertical zone
			{
				if (m_nAttribLabelVisibility != KBCAL_NONE)
				{
					CString sLabel = GetAttributeLabel(TDCA_FILELINK, m_nAttribLabelVisibility);

					if (!sLabel.IsEmpty())
						rLinks.left += GraphicsMisc::GetTextWidth(sLabel, *this, m_fonts.GetHFont());
				}

				if (point.x > rLinks.left)
				{
					int nLink = ((point.y - rLinks.top) / nItemHeight);
					ASSERT(nLink < nNumLinks);

					rLinks.right = (rLinks.left + GraphicsMisc::GetTextWidth(pKI->aFileLinks[nLink], *this, m_fonts.GetHFont(GMFS_UNDERLINED)));

					if (point.x < rLinks.right)
						return pKI->aFileLinks[nLink];
				}
			}
		}
	}
	
	return NOFILELINK;
}

BOOL CKanbanColumnCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CPoint point(::GetMessagePos());
	ScreenToClient(&point);

	HTREEITEM hti = HitTest(point);

	if (hti)
	{
		BOOL bLocked = m_data.IsLocked(GetTaskID(hti));
		KBC_IMAGETYPE nImage = HitTestImage(hti, point);

		switch (nImage)
		{
		case KBCI_FLAG:
		case KBCI_ICON:
			if (!bLocked)
				return GraphicsMisc::SetHandCursor();
			break;

		case KBCI_PIN:
			return GraphicsMisc::SetHandCursor();

		case KBCI_NONE:
			if (m_bDrawTaskFileLinks && !HitTestFileLink(hti, point).IsEmpty())
				return GraphicsMisc::SetHandCursor();
			break;
		}

		if (bLocked)
			return GraphicsMisc::SetAppCursor(_T("Locked"), _T("Resources\\Cursors"));
	}

	// else
	return CTreeCtrl::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CKanbanColumnCtrl::HitTestCheckbox(HTREEITEM hti, CPoint point) const
{
	CRect rCheckbox;

	return (GetItemCheckboxRect(hti, rCheckbox) && rCheckbox.PtInRect(point));
}

BOOL CKanbanColumnCtrl::SaveToImage(CBitmap& bmImage, const CSize& reqColSize)
{
	CAutoFlag af(m_bSavingToImage, TRUE);

	// resize our window to remove any vertical scrollbar
	CRect rPrev = CDialogHelper::GetChildRect(this);
	CRect rTemp(rPrev.TopLeft(), reqColSize);
	
	MoveWindow(rTemp);

	// Do the copy
	BOOL bRes = CCopyWndContents(*this).DoCopy(bmImage);

	// Restore size
	MoveWindow(rPrev);

	return bRes;
}

CSize CKanbanColumnCtrl::CalcRequiredSizeForImage() const
{
	CClientDC dc(const_cast<CKanbanColumnCtrl*>(this));

	int nMinHeaderWidth = (GraphicsMisc::GetTextWidth(&dc, m_columnDef.sTitle) + (2 * LV_PADDING));
	CSize reqSize(nMinHeaderWidth, 0);

	int nDefItemIndent = ((2 * ITEM_BORDER) + TEXT_BORDER.left + TEXT_BORDER.right) + 1; // +1 for shadow

	if (HasOption(KBCF_SHOWTASKCOLORASBAR))
		nDefItemIndent += (BAR_WIDTH + (2 * BAR_PADDING));

	if (HasOption(KBCF_SHOWCOMPLETIONCHECKBOXES))
		nDefItemIndent += (DEF_IMAGE_SIZE + CHECKBOX_PADDING);
	
	HTREEITEM hti = GetChildItem(NULL);
	
	while (hti)
	{
		DWORD dwTaskID = GetTaskID(hti);
		const KANBANITEM* pKI = m_data.GetItem(dwTaskID);

		if (pKI)
		{
			int nItemIndent = nDefItemIndent + CalcIndentation(hti);

			// Start with attributes
			CFont* pOldFont = dc.SelectObject(m_fonts.GetFont(0));
			int nMaxAttribWidth = 0;

			for (int nDisp = 0; nDisp < m_aDisplayAttrib.GetSize(); nDisp++)
			{
				TDC_ATTRIBUTE nAttrib = m_aDisplayAttrib[nDisp];

				switch (nAttrib)
				{
				case TDCA_FLAG: 
					// vertically below Pin icon
					break;

				case TDCA_PARENT:
					// TODO
					break;

				case TDCA_FILELINK:
					// TODO
					break;

				default: // Rest
					{
						CString sAttrib = FormatAttribute(nAttrib, pKI->GetAttributeDisplayValue(nAttrib), KBCAL_LONG);
						int nAttribWidth = (nItemIndent + GraphicsMisc::GetTextWidth(&dc, sAttrib));

						nMaxAttribWidth = max(nMaxAttribWidth, nAttribWidth);
					}
					break;
				}
			}

			// title
			int nTitleWidth = nItemIndent;

			// Task and Pin icons
			nTitleWidth += (2 * (DEF_IMAGE_SIZE + IMAGE_PADDING));

			// Text
			if (pKI->nLevel == 0)
			{
				dc.SelectObject(pOldFont);
				pOldFont = dc.SelectObject(m_fonts.GetFont(GMFS_BOLD));
			}

			// Title can be spread over two lines so we need to figure out
			// where a sensible halfway mark is and measure that, but only 
			// if the title doesn't already fit on one line
			int nTextWidth = GraphicsMisc::GetTextWidth(&dc, pKI->sTitle);

			if ((nTextWidth + nTitleWidth) > nMaxAttribWidth)
			{
				int nEndPos = Misc::FindNextOneOf(_T(" "), pKI->sTitle, TRUE, (pKI->sTitle.GetLength() / 2));

				if (nEndPos != -1)
					nTextWidth = GraphicsMisc::GetTextWidth(&dc, pKI->sTitle.Left(nEndPos));
			}

			nTitleWidth += nTextWidth;

			// Accumulate
			reqSize.cx = max(reqSize.cx, max(nTitleWidth, nMaxAttribWidth));

			// cleanup
			dc.SelectObject(pOldFont);
		}

		hti = GetNextSiblingItem(hti);
	}

	CRect rFirst, rLast;

	if (GetItemBounds(TCH().GetFirstItem(), rFirst) &&
		GetItemBounds(TCH().GetLastVisibleItem(), rLast))
	{
		reqSize.cy = (rLast.bottom - rFirst.top);
	}
		
	return reqSize;
}

LRESULT CKanbanColumnCtrl::OnSetFont(WPARAM /*wp*/, LPARAM /*lp*/)
{
	LRESULT lr = Default();

	m_nItemTextHeight = -1;
	OnDisplayAttributeChanged();

	return lr;
}

void CKanbanColumnCtrl::RecalcItemLineHeight()
{
	if (m_nItemTextHeight == -1)
	{
		CDC* pDC = GetDC();
		HGDIOBJ hPrev = pDC->SelectObject(m_fonts.GetHFont());

		TEXTMETRIC metrics = { 0 };
		pDC->GetTextMetrics(&metrics);

		ReleaseDC(pDC);

		m_nItemTextHeight = (metrics.tmHeight + metrics.tmExternalLeading);
	}

	if (m_nItemTextBorder == -1)
		m_nItemTextBorder = (GetItemHeight() - m_nItemTextHeight);

	int nItemHeight = (m_nItemTextHeight + m_nItemTextBorder);
		
	SetItemHeight((short)nItemHeight);
}

BOOL CKanbanColumnCtrl::SelectItem(HTREEITEM hItem, BOOL bByMouse)
{
	DWORD dwTaskID = GetTaskID(hItem);

	if (!dwTaskID || !SelectTask(dwTaskID))
		return FALSE;
	
	// We must synthesize our own notification because the default
	// one will be ignored because its action is TVC_UNKNOWN
	NotifyParentSelectionChange(hItem, bByMouse);

	return TRUE;
}

void CKanbanColumnCtrl::NotifyParentSelectionChange(HTREEITEM hItem, BOOL bByMouse)
{
	NMTREEVIEW nmtv = { 0 };

	nmtv.hdr.hwndFrom = GetSafeHwnd();
	nmtv.hdr.idFrom = GetDlgCtrlID();
	nmtv.hdr.code = TVN_SELCHANGED;

	nmtv.itemNew.hItem = hItem;
	nmtv.itemNew.state = TVIS_SELECTED;
	nmtv.itemNew.mask = TVIF_STATE;
	nmtv.itemNew.lParam = GetTaskID(hItem);

	nmtv.action = (bByMouse ? TVC_BYMOUSE : TVC_BYKEYBOARD);

	GetParent()->SendMessage(WM_NOTIFY, nmtv.hdr.idFrom, (LPARAM)&nmtv);
}

BOOL CKanbanColumnCtrl::InitTooltip()
{
	if (!m_tooltip.GetSafeHwnd())
	{
		if (!m_tooltip.Create(this))
			return FALSE;

		m_tooltip.ModifyStyleEx(0, WS_EX_TRANSPARENT);
		m_tooltip.SetDelayTime(TTDT_INITIAL, 50);
		m_tooltip.SetDelayTime(TTDT_AUTOPOP, 10000);
		m_tooltip.SetMaxTipWidth((UINT)(WORD)-1);

		HWND hwndTooltips = (HWND)SendMessage(LVM_GETTOOLTIPS);
		::SendMessage(hwndTooltips, TTM_ACTIVATE, FALSE, 0);
	}

	return TRUE;
}

void CKanbanColumnCtrl::FilterToolTipMessage(MSG* pMsg)
{
	if (m_tooltip.GetSafeHwnd())
		m_tooltip.FilterToolTipMessage(pMsg);
}

int CKanbanColumnCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	HTREEITEM hti = HitTest(point);

	if (hti)
	{
		CRect rText, rUnused;

		if (GetItemLabelTextRect(hti, rText, FALSE) && rText.PtInRect(point))
		{
			if (GetItemTooltipRect(hti, rUnused))
			{
				DWORD dwTaskID = GetTaskID(hti);

				return CToolTipCtrlEx::SetToolInfo(*pTI, this, m_data.GetItemTitle(dwTaskID), dwTaskID, rText);
			}
		}
	}

	return CTreeCtrl::OnToolHitTest(point, pTI);
}

void CKanbanColumnCtrl::OnTooltipShow(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Only handle our tooltips
	if (pNMHDR->hwndFrom != m_tooltip)
		return;

	DWORD dwTaskID = m_tooltip.GetLastHitToolInfo().uId;
	ASSERT(dwTaskID);

	HTREEITEM hti = FindItem(dwTaskID);
	ASSERT(hti);

	CRect rTip;
	VERIFY(GetItemTooltipRect(hti, rTip));

	const KANBANITEM* pKI = m_data.GetItem(dwTaskID);
	ASSERT(pKI);

	m_tooltip.SetMaxTipWidth(rTip.Width());
	m_tooltip.SetFont(m_fonts.GetFont((pKI->dwParentID == 0) ? GMFS_BOLD : 0));
	m_tooltip.SetWindowPos(NULL, rTip.left, rTip.top, rTip.Width(), rTip.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	*pResult = TRUE; // we do the positioning
}

BOOL CKanbanColumnCtrl::AttributeValuesMatch(const CKanbanColumnCtrl& other) const
{
	CStringArray aOtherValues;
	other.GetAttributeValues(aOtherValues);

	return Misc::MatchAll(m_columnDef.aAttribValues, aOtherValues);
}

BOOL CKanbanColumnCtrl::CanDrag(const CKanbanColumnCtrl* pSrcCol, const CKanbanColumnCtrl* pDestCol)
{
	if (!pDestCol)
		return FALSE;
	
	if (pDestCol == pSrcCol)
		return FALSE;
		
	if (pSrcCol->AttributeValuesMatch(*pDestCol))
		return FALSE;

	// Can't copy FROM the backlog
	if (pSrcCol->IsBacklog() && Misc::ModKeysArePressed(MKS_CTRL))
		return FALSE;

	return TRUE;
}

BOOL CKanbanColumnCtrl::CreateDragImage(CImageList& ilDrag, CSize& sizeImage)
{
	if (!m_bSelected)
	{
		ASSERT(0);
		return FALSE;
	}

	return CDragDropData::CreateDragImage(this, ilDrag, sizeImage);
}

CSize CKanbanColumnCtrl::OnGetDragSize(CDC& /*dc*/)
{
	ASSERT(m_aSelTaskIDs.GetSize());

	// Calculate the aggregate height with 
	// horizontal indents but no vertical gaps
	CHTIList lstSelection;
	BuildSortedSelection(lstSelection);
	
	POSITION pos = lstSelection.GetHeadPosition();
	CRect rDrag(0, 0, 0, 0);

	while (pos)
	{
		HTREEITEM htiSel = lstSelection.GetNext(pos);
		ASSERT(htiSel);

		CRect rItem;
		VERIFY(GetItemRect(htiSel, rItem));

		rDrag.left = min(rDrag.left, rItem.left);
		rDrag.right = max(rDrag.right, rItem.right);
	}
	rDrag.right = min(rDrag.right, rDrag.left + MAX_DRAG_ITEM_WIDTH + DEF_IMAGE_SIZE + IMAGE_PADDING);

	int nItemHeight = (TEXT_BORDER.Height() + (m_nNumTitleLines * (m_nItemTextBorder + m_nItemTextHeight)));

	return CSize(rDrag.Width(), (nItemHeight * m_aSelTaskIDs.GetSize()));
}

void CKanbanColumnCtrl::OnDrawDragData(CDC& dc, const CRect& rc, COLORREF& crMask)
{
	// Draw item sequentially with no gaps
	CHTIList lstSelection;
	BuildSortedSelection(lstSelection);

	crMask = 1;
	dc.FillSolidRect(rc, crMask);

	POSITION pos = lstSelection.GetHeadPosition();
	int nItemHeight = (TEXT_BORDER.Height() + (m_nNumTitleLines * (m_nItemTextBorder + m_nItemTextHeight)));
	int nVPos = 0;

	while (pos)
	{
		HTREEITEM htiSel = lstSelection.GetNext(pos);
		ASSERT(htiSel);

		CRect rItem;
		VERIFY(GetItemRect(htiSel, rItem));

		rItem.top = nVPos;
		rItem.bottom = rItem.top + nItemHeight;
		rItem.right = min(rItem.right, rc.right);

		GraphicsMisc::DrawExplorerItemSelection(&dc, *this, GMIS_SELECTED, rItem);

		CRect rBody(rItem);
		rBody.DeflateRect(1, 1);

		DWORD dwTaskID = GetTaskID(htiSel);
		const KANBANITEM* pKI = m_data.GetItem(dwTaskID);

		CRect rIcon(rBody);
		rIcon.OffsetRect(IMAGE_PADDING, IMAGE_PADDING);

		DrawTaskIcon(&dc, pKI, rIcon);

		rBody.left += DEF_IMAGE_SIZE + IMAGE_PADDING;
		DrawItemTitle(&dc, pKI, rBody, ::GetSysColor(COLOR_WINDOWTEXT));

		nVPos += nItemHeight;
	}
}
