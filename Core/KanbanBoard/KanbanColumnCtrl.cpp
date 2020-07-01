// KanbanListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KanbanColumnCtrl.h"
#include "Kanbanstatic.h"
#include "Kanbanenum.h"

#include "..\shared\graphicsMisc.h"
#include "..\shared\enstring.h"
#include "..\shared\enbitmap.h"
#include "..\shared\misc.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\datehelper.h"
#include "..\shared\timehelper.h"
#include "..\shared\autoflag.h"
#include "..\shared\copywndcontents.h"
#include "..\shared\holdredraw.h"
#include "..\shared\winclasses.h"
#include "..\shared\wclassdefines.h"
#include "..\Shared\enimagelist.h"
#include "..\Shared\osversion.h"
#include "..\Shared\themed.h"

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
	FLAG_HOVER,
	PIN_SET,
	PIN_HOVER,
};

/////////////////////////////////////////////////////////////////////////////

const int MIN_LABEL_EDIT_WIDTH	= GraphicsMisc::ScaleByDPIFactor(200);
const int BAR_WIDTH				= GraphicsMisc::ScaleByDPIFactor(6);
const int LV_PADDING			= GraphicsMisc::ScaleByDPIFactor(3);
const int CHECKBOX_PADDING		= GraphicsMisc::ScaleByDPIFactor(1);
const int ATTRIB_INDENT			= GraphicsMisc::ScaleByDPIFactor(6);
const int TIP_PADDING			= GraphicsMisc::ScaleByDPIFactor(4);
const int IMAGE_SIZE			= GraphicsMisc::ScaleByDPIFactor(16);
const int IMAGE_PADDING			= 2/*GraphicsMisc::ScaleByDPIFactor(2)*/;
const int LEVEL_INDENT			= GraphicsMisc::ScaleByDPIFactor(16);

const CRect TEXT_BORDER			= CRect(2, 1, 3, 1);
const COLORREF WHITE			= RGB(255, 255, 255);

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
	m_nAttribLabelVisiability(KBCAL_LONG),
	m_bSavingToImage(FALSE),
	m_bDropTarget(FALSE),
	m_bDrawTaskFlags(FALSE),
	m_bDrawTaskParents(FALSE),
	m_dwDisplay(0),
	m_dwOptions(0),
	m_dwHotItem(0),
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
	ON_NOTIFY(TTN_SHOW, 0, OnTooltipShow)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKanbanListCtrlEx message handlers

BOOL CKanbanColumnCtrl::Create(UINT nID, CWnd* pParentWnd)
{
	UINT nFlags = (WS_CHILD | WS_VISIBLE | LVS_REPORT | WS_TABSTOP |
				   TVS_NONEVENHEIGHT | TVS_SHOWSELALWAYS | TVS_EDITLABELS | 
				   TVS_FULLROWSELECT | TVS_NOTOOLTIPS | TVS_NOHSCROLL);

	return CTreeCtrl::Create(nFlags, CRect(0, 0, 0, 0), pParentWnd, nID);
}

int CKanbanColumnCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (Misc::HasFlag(m_dwOptions, KBCF_SHOWLABELTIPS))
		VERIFY(InitTooltip());

	m_ilIcons.Create(IDB_ICONS, 16, 1, RGB(255, 0, 255));
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

void CKanbanColumnCtrl::SetExcessColor(COLORREF color)
{
	// TODO
	ASSERT(0);
}

void CKanbanColumnCtrl::SetMaximumTaskCount(int nMaxTasks)
{
	// TODO
	ASSERT(0);
}

void CKanbanColumnCtrl::OnDisplayAttributeChanged()
{
	m_bDrawTaskFlags = (Misc::FindT(TDCA_FLAG, m_aDisplayAttrib) != -1);
	m_bDrawTaskParents = (Misc::FindT(TDCA_PARENT, m_aDisplayAttrib) != -1);

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

		if (m_bDrawTaskParents)
			nNumLines += pKI->nLevel;

		TCH().SetItemIntegral(hti, nNumLines);
	}
}

int CKanbanColumnCtrl::GetItemDisplayAttributeCount(const KANBANITEM& ki) const
{
	int nCount = 0, nDisp = m_aDisplayAttrib.GetSize();

	while (nDisp--)
	{
		TDC_ATTRIBUTE nAttribID = m_aDisplayAttrib[nDisp];

		switch (nAttribID)
		{
		case TDCA_FLAG:
		case TDCA_PARENT:
			break;

		default:
			if (!HasOption(KBCF_HIDEEMPTYATTRIBUTES) || ki.HasAttributeDisplayValue(nAttribID))
				nCount++;
			break;
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
	m_nAttribLabelVisiability = nLabelVis;

	if (GetSafeHwnd())
		Invalidate(TRUE);
}

void CKanbanColumnCtrl::SetSelected(BOOL bSelected)
{
	m_bSelected = bSelected;

	if (!bSelected)
		ClearSelection();
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

void CKanbanColumnCtrl::FillItemBackground(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText, BOOL bSelected) const
{
	if (bSelected)
	{
		BOOL bFocused = (bSelected && (::GetFocus() == GetSafeHwnd()));

		GM_ITEMSTATE nState = (bFocused ? GMIS_SELECTED : GMIS_SELECTEDNOTFOCUSED);
		crText = GraphicsMisc::GetExplorerItemSelectionTextColor(crText, nState, GMIB_THEMECLASSIC);

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
			const KANBANITEM* pKI = m_data.GetItem(dwTaskID);
			
			if (pKI)
			{
				HTREEITEM hti = (HTREEITEM)pTVCD->nmcd.dwItemSpec;
				CDC* pDC = CDC::FromHandle(pTVCD->nmcd.hdc);
		
				CRect rItem;
				GetItemRect(hti, rItem, NULL);
				rItem.DeflateRect(1, 1);

				// Checkbox
				DrawItemCheckbox(pDC, pKI, rItem);
				
				BOOL bSelected = IsTaskSelected(dwTaskID);
				COLORREF crText = pKI->GetTextColor(bSelected, (HasOption(KBCF_TASKTEXTCOLORISBKGND) && !HasOption(KBCF_SHOWTASKCOLORASBAR)));

				// Background
				FillItemBackground(pDC, pKI, rItem, crText, bSelected);

				// Parents
				COLORREF crOtherText = crText;
				
				if (!bSelected && !Misc::IsHighContrastActive() && !pKI->IsDone(TRUE))
					crOtherText = GraphicsMisc::Lighter(crText, 0.3);

				DrawItemParents(pDC, pKI, rItem, crOtherText);
	
				// Bar affects everything else
				DrawItemBar(pDC, pKI, rItem);

				// Icons don't affect attributes
				CRect rAttributes(rItem);

				DrawItemImages(pDC, pKI, rItem);
				DrawItemTitle(pDC, pKI, rItem, crText);

				rAttributes.top += CalcItemTitleTextHeight();
				rAttributes.top += IMAGE_PADDING;

				DrawItemAttributes(pDC, pKI, rAttributes, crOtherText);
			}
			
			*pResult |= CDRF_SKIPDEFAULT;
		}
	}
}

void CKanbanColumnCtrl::DrawItemTitle(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText)
{
	CRect rTitle(rItem);

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
	rTitle.DeflateRect(TEXT_BORDER);
	rTitle.bottom = (rTitle.top + CalcItemTitleTextHeight());
	rTitle.bottom = min(rTitle.bottom, rItem.bottom);

	int nFlags = (DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX | DT_WORDBREAK); // multi-line for titles only

	pDC->DrawText(pKI->sTitle, rTitle, nFlags);

	if (pOldFont)
		pDC->SelectObject(pOldFont);
}

void CKanbanColumnCtrl::DrawItemAttributes(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText)
{
	pDC->SetBkMode(TRANSPARENT);

	CFont* pOldFont = NULL;
	DWORD dwFontFlags = 0;

	if (HasOption(KBCF_STRIKETHRUDONETASKS) && pKI->IsDone(FALSE))
		pOldFont = pDC->SelectObject(m_fonts.GetFont(GMFS_STRIKETHRU));

	CRect rAttrib(rItem);
	rAttrib.DeflateRect(TEXT_BORDER);

	int nFlags = (DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX);

	for (int nDisp = 0; nDisp < m_aDisplayAttrib.GetSize(); nDisp++)
	{
		TDC_ATTRIBUTE nAttrib = m_aDisplayAttrib[nDisp];

		switch (nAttrib)
		{
		case TDCA_FLAG:
		case TDCA_PARENT:
			break;

		default:
			if (!HasOption(KBCF_HIDEEMPTYATTRIBUTES) || pKI->HasAttributeDisplayValue(nAttrib))
				DrawAttribute(pDC, rAttrib, nAttrib, pKI->GetAttributeDisplayValue(nAttrib), nFlags, crText);
			break;
		}
	}

	if (pOldFont)
		pDC->SelectObject(pOldFont);
}

void CKanbanColumnCtrl::DrawItemParents(CDC* pDC, const KANBANITEM* pKI, CRect& rItem, COLORREF crText) const
{
	if (m_bDrawTaskParents && pKI->dwParentID)
	{
		CKanbanItemArray aParents;
		const KANBANITEM* pKIParent = m_data.GetItem(pKI->dwParentID);

		while (pKIParent)
		{
			aParents.Add(pKIParent);
			pKIParent = m_data.GetItem(pKIParent->dwParentID);
		}

		// Draw in reverse order
		int nFlags = (DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX);
		int nParent = aParents.GetSize();
		
		CRect rParent(rItem);
		rParent.DeflateRect(TEXT_BORDER);
	
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(crText);

		while (nParent--)
		{
			const KANBANITEM* pKIParent = aParents[nParent];
			ASSERT(pKIParent);
			
			int iImageIndex = -1;
			HIMAGELIST hilTask = (HIMAGELIST)GetParent()->SendMessage(WM_KLCN_GETTASKICON, pKIParent->dwTaskID, (LPARAM)&iImageIndex);

			if (hilTask && (iImageIndex != -1))
			{
				ImageList_DrawEx(hilTask, iImageIndex, *pDC, rParent.left, rParent.top, 0, 0, 0, WHITE, ILD_TRANSPARENT | ILD_BLEND50);
				rParent.left += (IMAGE_SIZE + IMAGE_PADDING);
			}

			pDC->DrawText(pKIParent->sTitle, rParent, nFlags);

			rParent.top += (m_nItemTextHeight + m_nItemTextBorder);
			rParent.left = (rItem.left + 6);
		}

		rItem.top = rParent.top;
	}
}

void CKanbanColumnCtrl::DrawItemImages(CDC* pDC, const KANBANITEM* pKI, CRect& rItem) const
{
	CSaveDC sdc(pDC);
	CRect rClip(rItem), rIcon(rItem);

	rClip.DeflateRect(1, 1);
	pDC->IntersectClipRect(rClip);

	rIcon.left += IMAGE_PADDING;
	rIcon.top += IMAGE_PADDING;
	rIcon.bottom = (rIcon.top + IMAGE_SIZE);

	if (pKI->bHasIcon || pKI->bParent)
	{
		int iImageIndex = -1;
		HIMAGELIST hilTask = (HIMAGELIST)GetParent()->SendMessage(WM_KLCN_GETTASKICON, pKI->dwTaskID, (LPARAM)&iImageIndex);

		if (hilTask && (iImageIndex != -1))
			DrawItemImage(pDC, rIcon, KBCI_ICON, FALSE, hilTask, iImageIndex);
	}

	// Always leave space for icon
	rIcon.OffsetRect(0, IMAGE_SIZE);

	BOOL bHot = (!m_bSavingToImage && (pKI->dwTaskID == m_dwHotItem));

	if (m_bDrawTaskFlags)
	{
		if (pKI->bFlagged || bHot)
			DrawItemImage(pDC, rIcon, KBCI_FLAG, (bHot && !pKI->bFlagged));

		rIcon.OffsetRect(0, IMAGE_SIZE);
	}

	if (pKI->bPinned || bHot)
		DrawItemImage(pDC, rIcon, KBCI_PIN, (bHot && !pKI->bPinned));

	rItem.left = (rIcon.left + IMAGE_SIZE);
}

void CKanbanColumnCtrl::DrawItemImage(CDC* pDC, const CRect& rImage, KBC_IMAGETYPE nType, BOOL bHover, HIMAGELIST hIL, int nIndex) const
{
	switch (nType)
	{
	case KBCI_ICON:
		ASSERT(hIL && (nIndex != -1));
		break;

	case KBCI_FLAG:
		hIL = m_ilIcons;
		nIndex = (bHover ? FLAG_HOVER : FLAG_SET);
		break;

	case KBCI_PIN:
		hIL = m_ilIcons;
		nIndex = (bHover ? PIN_HOVER : PIN_SET);
		break;

	default:
		ASSERT(0);
	}

	ImageList_Draw(hIL, nIndex, *pDC, rImage.left, rImage.top, ILD_TRANSPARENT);
}

void CKanbanColumnCtrl::DrawItemBar(CDC* pDC, const KANBANITEM* pKI, CRect& rItem) const
{
	if (HasOption(KBCF_SHOWTASKCOLORASBAR))
	{
		// Don't draw for completed items but ensure same indentation
		CRect rBar(rItem);

		rBar.DeflateRect(2, 2);
		rBar.right = (rBar.left + BAR_WIDTH);

		if (!pKI->IsDone(TRUE))
		{
			if (HasOption(KBCF_COLORBARBYPRIORITY))
			{
				int nPriority = pKI->GetPriority();

				if (nPriority != -2)
				{
					COLORREF crFill = m_aPriorityColors[nPriority];
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
	if (HasOption(KBCF_SHOWCOMPLETIONCHECKBOXES))
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
}

BOOL CKanbanColumnCtrl::GetItemCheckboxRect(HTREEITEM hti, CRect& rItem, const KANBANITEM* pKI) const
{
	if (HasOption(KBCF_SHOWCOMPLETIONCHECKBOXES))
	{
		GetItemRect(hti, rItem, pKI);

		return GetItemCheckboxRect(rItem);
	}

	// else
	return FALSE;
}

BOOL CKanbanColumnCtrl::GetItemBounds(HTREEITEM hti, LPRECT lpRect) const
{
	return CTreeCtrl::GetItemRect(hti, lpRect, FALSE);
}

BOOL CKanbanColumnCtrl::GetItemRect(HTREEITEM hti, CRect& rItem, const KANBANITEM* pKI) const
{
	if (!GetItemBounds(hti, rItem))
		return FALSE;

	if (HasOption(KBCF_INDENTSUBTASKS))
	{
		if (!pKI)
			pKI = m_data.GetItem(GetTaskID(hti));

		ASSERT(pKI);

		// Indent to match level
		rItem.left += (pKI->nLevel * LEVEL_INDENT);
	}

	return TRUE;
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

BOOL CKanbanColumnCtrl::GetItemLabelTextRect(HTREEITEM hti, CRect& rItem, BOOL bEdit, const KANBANITEM* pKI) const
{
	if (!GetItemRect(hti, rItem, pKI))
		return FALSE;

	rItem.DeflateRect(1, 1);

	CRect rCheckbox(rItem);
	
	if (GetItemCheckboxRect(rCheckbox))
		rItem.left = (rCheckbox.right + CHECKBOX_PADDING);

	rItem.left += (IMAGE_SIZE + IMAGE_PADDING);

	if (!pKI)
		pKI = m_data.GetItem(GetTaskID(hti));
	
	if (m_bDrawTaskParents)
		rItem.top += (pKI->nLevel * (m_nItemTextHeight + m_nItemTextBorder));

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

BOOL CKanbanColumnCtrl::GetItemTooltipRect(HTREEITEM hti, CRect& rTip, const KANBANITEM* pKI) const
{
	if (!pKI)
		pKI = m_data.GetItem(GetTaskID(hti));

	if (!pKI)
	{
		ASSERT(0);
		return FALSE;
	}

	if (!GetItemLabelTextRect(hti, rTip, FALSE, pKI))
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
	case TDCA_TIMEEST:		return (bLong ? IDS_DISPLAY_TIMEEST : IDS_DISPLAY_TIMEEST_SHORT);
	case TDCA_TIMESPENT:	return (bLong ? IDS_DISPLAY_TIMESPENT : IDS_DISPLAY_TIMESPENT_SHORT);
	case TDCA_VERSION:		return (bLong ? IDS_DISPLAY_VERSION : IDS_DISPLAY_VERSION_SHORT);
	}

	ASSERT(0);
	return 0;
}

void CKanbanColumnCtrl::DrawAttribute(CDC* pDC, CRect& rLine, TDC_ATTRIBUTE nAttrib, const CString& sValue, int nFlags, COLORREF crText) const
{
	KBC_ATTRIBLABELS nLabelVis = m_nAttribLabelVisiability;
	
	if (m_bSavingToImage)
		nLabelVis = KBCAL_LONG;

	CString sAttrib = FormatAttribute(nAttrib, sValue, nLabelVis);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(crText);
	pDC->DrawText(sAttrib, rLine, nFlags);

	rLine.top += (m_nItemTextHeight + m_nItemTextBorder);
}

CString CKanbanColumnCtrl::FormatAttribute(TDC_ATTRIBUTE nAttrib, const CString& sValue, KBC_ATTRIBLABELS nLabelVis)
{
	UINT nFormatID = 0;
	
	if (nLabelVis != KBCAL_NONE)
		nFormatID = GetDisplayFormat(nAttrib, (nLabelVis == KBCAL_LONG));

	if (nFormatID == 0)
		return sValue;

	CEnString sAttrib(nFormatID);

	sAttrib += _T(": ");
	sAttrib += sValue;

	return sAttrib;
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

BOOL CKanbanColumnCtrl::HasTasks(const CDWordArray& aTaskIDs) const
{
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
	ASSERT(m_bSelected);

	if (m_aSelTaskIDs.GetSize())
	{
		CRect rClient;
		GetClientRect(rClient);

		// Check for any selected item being completely visible
		int nID = m_aSelTaskIDs.GetSize();
		HTREEITEM htiPartial = NULL;

		while (nID--)
		{
			HTREEITEM hti = FindItem(m_aSelTaskIDs[nID]);
			ASSERT(hti);

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

		// else just scroll to the first
		HTREEITEM hti = (htiPartial ? htiPartial : FindItem(m_aSelTaskIDs[0]));
		ASSERT(hti);

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

void CKanbanColumnCtrl::SetHotItem(DWORD dwTaskID)
{
	if (m_dwHotItem == dwTaskID)
		return;

	if (m_dwHotItem)
		TCH().InvalidateItem(FindItem(m_dwHotItem));

	if (dwTaskID)
		TCH().InvalidateItem(FindItem(dwTaskID));

	m_dwHotItem = dwTaskID;
}

HTREEITEM CKanbanColumnCtrl::FindItem(DWORD dwTaskID) const
{
	return m_mapHTItems.GetItem(dwTaskID);
}

HTREEITEM CKanbanColumnCtrl::FindItem(const CPoint& ptScreen) const
{
	CPoint ptClient(ptScreen);
	ScreenToClient(&ptClient);
	
	return CTreeCtrl::HitTest(ptClient);
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

		if (m_dwHotItem == dwTaskID)
			m_dwHotItem = 0;

		return TRUE;
	}

	return FALSE;
}

BOOL CKanbanColumnCtrl::DeleteAll()
{
	m_mapHTItems.RemoveAll();

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

int CALLBACK CKanbanColumnCtrl::SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	const KANBANSORT* pSort = (KANBANSORT*)lParamSort;
	
	const KANBANITEM* pKI1 = pSort->data.GetItem(lParam1);
	const KANBANITEM* pKI2 = pSort->data.GetItem(lParam2);

	int nCompare = 0;
	
	if (pKI1 && pKI2)
	{
		// Pinned tasks always at the top
		if (pKI1->bPinned && !pKI2->bPinned)
			return -1;

		if (!pKI1->bPinned && pKI2->bPinned)
			return 1;

		if (pSort->bSubtasksBelowParent && (pKI1->dwParentID != pKI2->dwParentID))
		{
			// If one is the parent of another always sort below
			if (pSort->IsParent(lParam2, pKI1))
				return 1;

			if (pSort->IsParent(lParam1, pKI2))
				return -1;

			// We can't sort items that are not in the same 
			// branch of the tree ie. they need to have the same parent

			// First we raise the items to the same level
			while (pKI1->nLevel > pKI2->nLevel)
				pKI1 = pSort->GetParent(pKI1);

			while (pKI2->nLevel > pKI1->nLevel)
				pKI2 = pSort->GetParent(pKI2);

			// Then we raise them to have the same parent
			while (pKI1->dwParentID != pKI2->dwParentID)
			{
				pKI1 = pSort->GetParent(pKI1);
				pKI2 = pSort->GetParent(pKI2);
			}
		}
	
		switch (pSort->nBy)
		{
		case TDCA_TASKNAME:
			nCompare = Misc::NaturalCompare(pKI1->sTitle, pKI2->sTitle);
			break;
			
		case TDCA_ALLOCBY:
		case TDCA_ALLOCTO:
		case TDCA_CATEGORY:
		case TDCA_PRIORITY:
		case TDCA_RISK:
		case TDCA_STATUS:
		case TDCA_TAGS:
		case TDCA_VERSION:
		{
				ASSERT(!pSort->sAttribID.IsEmpty());

				CString sValue1 = pKI1->GetAttributeDisplayValue(pSort->nBy);
				CString sValue2 = pKI2->GetAttributeDisplayValue(pSort->nBy);

				nCompare = Misc::NaturalCompare(sValue1, sValue2);
			}
			break;

		case TDCA_CUSTOMATTRIB:
			// TODO
			break;

		// Other display attributes
		case TDCA_COST:
			nCompare = Misc::NaturalCompare(pKI1->sCost, pKI2->sCost);
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
			if (pKI1->bFlagged && pKI2->bFlagged)
			{
				nCompare = 0;
			}
			else if (pKI1->bFlagged)
			{
				nCompare = -1;
			}
			else if (pKI2->bFlagged)
			{
				nCompare = 1;
			}
			break;
			
		case TDCA_LASTMODDATE:
			nCompare = CDateHelper::Compare(pKI1->dtLastMod, pKI2->dtLastMod, DHC_COMPARETIME);
			break;
			
		case TDCA_PERCENT:
			nCompare = ((pKI1->nPercent > pKI2->nPercent) ? 1 : -1);
			break;
			
		case TDCA_RECURRENCE:
			nCompare = Misc::NaturalCompare(pKI1->sRecurrence, pKI2->sRecurrence);
			break;
			
		case TDCA_STARTDATE:
			nCompare = CDateHelper::Compare(pKI1->dtStart, pKI2->dtStart, DHC_COMPARETIME);
			break;
			
		case TDCA_TIMEEST:
			nCompare = CTimeHelper().Compare(pKI1->dTimeEst, MapUnitsToTHUnits(pKI1->nTimeEstUnits), 
											pKI2->dTimeEst, MapUnitsToTHUnits(pKI2->nTimeEstUnits));
			break;
			
		case TDCA_TIMESPENT:
			nCompare = CTimeHelper().Compare(pKI1->dTimeSpent, MapUnitsToTHUnits(pKI1->nTimeSpentUnits), 
											pKI2->dTimeSpent, MapUnitsToTHUnits(pKI2->nTimeSpentUnits));
			break;

		case TDCA_NONE: // Synonymous with IUI_POSITION
			ASSERT(pSort->bSubtasksBelowParent);

			// Avoid reversal of sign below
			return ((pKI1->nPosition > pKI2->nPosition) ? 1 : -1);
		}
	}
	
	return (pSort->bAscending ? nCompare : -nCompare);
}

void CKanbanColumnCtrl::Sort(TDC_ATTRIBUTE nBy, BOOL bAscending)
{
	KANBANSORT ks(m_data);
	
	ks.nBy = nBy;
	ks.bAscending = bAscending;
	ks.bSubtasksBelowParent = HasOption(KBCF_SORTSUBTASTASKSBELOWPARENTS);

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
}

void CKanbanColumnCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	HTREEITEM htiUnused = NULL;
	HandleButtonClick(point, htiUnused);
	
	CTreeCtrl::OnRButtonDown(nFlags, point);
}

void CKanbanColumnCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Note: The complexity of this function arises out of the 
	// indentation of subtasks, because we want clicking to the
	// left of the task to select the task BUT NOT begin a 
	// drag operation or a label edit. 
	// My intuition tells me that it can be simplified...
	HTREEITEM htiHit = NULL;
	BOOL bHandled = HandleButtonClick(point, htiHit);

	if (htiHit)
	{
		DWORD dwTaskID = GetTaskID(htiHit);

		const KANBANITEM* pKI = m_data.GetItem(dwTaskID);
		ASSERT(pKI);

		UINT nMsgID = 0;
		BOOL bSet = FALSE;

		if (HitTestCheckbox(htiHit, point))
		{
			nMsgID = WM_KLCN_EDITTASKDONE;
			bSet = !pKI->bDone;
		}
		else
		{
			switch (HitTestImage(htiHit, point))
			{
			case KBCI_ICON:	
				nMsgID = WM_KLCN_EDITTASKICON;
				break;

			case KBCI_FLAG:
				nMsgID = WM_KLCN_EDITTASKFLAG;
				bSet = !pKI->bFlagged;
				break;

			case KBCI_PIN:
				nMsgID = WM_KLCN_EDITTASKPIN;
				bSet = !pKI->bPinned;
				break;
			}
		}

		if (nMsgID)
		{
			// Post message to give mouse-click time to complete
			GetParent()->PostMessage(nMsgID, 0, bSet);
			bHandled = TRUE;
		}
		else if (bHandled)
		{
			// Handle item drag because we WON'T be calling base class
			CPoint ptScreen(point);
			ClientToScreen(&ptScreen);

			if (::DragDetect(*this, ptScreen))
			{
				TRACE(_T("CKanbanColumnCtrl::OnLButtonDown(Faking drag start)\n"));

				NMTREEVIEW nmtv = { *this, (UINT)GetDlgCtrlID(), TVN_BEGINDRAG, 0 };

				nmtv.itemNew.hItem = htiHit;
				nmtv.itemNew.lParam = dwTaskID;
				nmtv.ptDrag = point;

				GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmtv);
			}
		}
	}

	// If not handled second click should cause label edit
	if (!bHandled && htiHit)
	{
		// Adjust 'point' to compensate for the indent
		CRect rTreeItem, rIndentedItem;

		CTreeCtrl::GetItemRect(htiHit, rTreeItem, FALSE);
		GetItemLabelTextRect(htiHit, rIndentedItem, FALSE);

		if (rTreeItem.left != rIndentedItem.left)
		{
			point.x -= (rIndentedItem.left - rTreeItem.left);
			DefWindowProc(WM_LBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));
		
			bHandled = TRUE;
		}
	}
	
	if (!bHandled)
		CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CKanbanColumnCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	HTREEITEM htiHit = NULL;
	BOOL bHandled = HandleButtonClick(point, htiHit);

	if (htiHit)
	{
		bHandled = TRUE;
		EditLabel(htiHit);
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
	
	if (Misc::IsKeyPressed(VK_CONTROL))
	{
		if (Misc::FindT(dwTaskID, m_aSelTaskIDs) == -1)
		{
			m_aSelTaskIDs.InsertAt(0, dwTaskID); // new anchor
		}
		else
		{
			Misc::RemoveItemT(dwTaskID, m_aSelTaskIDs);
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CKanbanColumnCtrl::HandleButtonClick(CPoint point, HTREEITEM& htiHit)
{
	BOOL bHandled = FALSE;

	// Ignore clicks not hitting an item
	htiHit = HitTest(point);

	if (!htiHit)
	{
		if (!m_bSelected)
			SetFocus();

		bHandled = TRUE;
	}
	else
	{
		if (m_bSelected)
		{
			SetFocus();
			bHandled = TRUE;
		}

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
				BOOL bWantEdit = (HitTestCheckbox(htiHit, point) || 
									(HitTestImage(htiHit, point) != KBCI_NONE));

				if (!bWantEdit && !::DragDetect(*this, point))
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

void CKanbanColumnCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
	CRect rImage;
	GetItemLabelTextRect(hti, rImage);

	// Icon
	rImage.right = (rImage.left - IMAGE_PADDING);
	rImage.left = (rImage.right - IMAGE_SIZE);
	rImage.bottom = (rImage.top + IMAGE_SIZE);

	if (rImage.PtInRect(point))
		return KBCI_ICON;

	// Flag
	if (m_bDrawTaskFlags)
	{
		rImage.OffsetRect(0, IMAGE_SIZE);

		if (rImage.PtInRect(point))
			return KBCI_FLAG;
	}

	// Pin
	rImage.OffsetRect(0, IMAGE_SIZE);

	if (rImage.PtInRect(point))
		return KBCI_PIN;

	// all else
	return KBCI_NONE;
}

BOOL CKanbanColumnCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CPoint point(::GetMessagePos());
	ScreenToClient(&point);

	HTREEITEM hti = HitTest(point);

	if (hti && (HitTestImage(hti, point) != KBCI_NONE))
		return GraphicsMisc::SetHandCursor();

	// else
	return CTreeCtrl::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CKanbanColumnCtrl::HitTestCheckbox(HTREEITEM hti, CPoint point) const
{
	CRect rCheckbox;

	return (GetItemCheckboxRect(hti, rCheckbox, NULL) && rCheckbox.PtInRect(point));
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
	float fAveCharWidth = GraphicsMisc::GetAverageCharWidth(&dc);

	int nMinHeaderWidth = ((int)(m_columnDef.sTitle.GetLength() * fAveCharWidth) + (2 * LV_PADDING));
	CSize reqSize(nMinHeaderWidth, 0);
	
	HTREEITEM hti = GetChildItem(NULL);
	
	while (hti)
	{
		DWORD dwTaskID = GetTaskID(hti);
		const KANBANITEM* pKI = m_data.GetItem(dwTaskID);

		if (pKI)
		{
			int nItemWidth = (int)(pKI->sTitle.GetLength() * fAveCharWidth / 2); // title is on two lines
			nItemWidth += (pKI->nLevel * LEVEL_INDENT);
			nItemWidth += (IMAGE_SIZE + IMAGE_PADDING);
			
			for (int nDisp = 0; nDisp < m_aDisplayAttrib.GetSize(); nDisp++)
			{
				TDC_ATTRIBUTE nAttrib = m_aDisplayAttrib[nDisp];

				if (nAttrib != TDCA_FLAG)
				{
					CString sAttrib = FormatAttribute(nAttrib, pKI->GetAttributeDisplayValue(nAttrib), KBCAL_LONG);
					nItemWidth = max(nItemWidth, ((int)(sAttrib.GetLength() * fAveCharWidth) + ATTRIB_INDENT));
				}
			}

			reqSize.cx = max(reqSize.cx, nItemWidth);
		}

		hti = GetNextSiblingItem(hti);
	}

	CRect rFirst, rLast;

	if (GetItemRect(TCH().GetFirstItem(), rFirst, FALSE) &&
		GetItemRect(TCH().GetLastVisibleItem(), rLast, FALSE))
	{
		reqSize.cy = (rLast.bottom - rFirst.top);
	}
		
	return reqSize;
}

BOOL CKanbanColumnCtrl::SelectionHasLockedTasks() const
{
	int nID = m_aSelTaskIDs.GetSize();

	while (nID--)
	{
		if (m_data.IsLocked(m_aSelTaskIDs[nID]))
			return TRUE;
	}

	return FALSE;
}

LRESULT CKanbanColumnCtrl::OnSetFont(WPARAM wp, LPARAM lp)
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
		
	if (m_bDrawTaskFlags && (m_aDisplayAttrib.GetSize() == 1))
		nItemHeight = max(nItemHeight, (IMAGE_SIZE + IMAGE_PADDING));

	SetItemHeight(nItemHeight);

	// Recalc the required number of title lines
	int nNumIcons = (2 + (m_bDrawTaskFlags ? 1 : 0));
	double dTotalIconHeight = (nNumIcons * IMAGE_SIZE);

	m_nNumTitleLines = max(2, (int)ceil(dTotalIconHeight / m_nItemTextHeight));
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
		CRect rTip;

		if (GetItemTooltipRect(hti, rTip, NULL))
		{
			GetItemRect(hti, rTip, NULL);

			DWORD dwTaskID = GetTaskID(hti);


			return CToolTipCtrlEx::SetToolInfo(*pTI, this, m_data.GetItemTitle(dwTaskID), dwTaskID, rTip);
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

	const KANBANITEM* pKI = m_data.GetItem(dwTaskID);
	ASSERT(pKI);

	CRect rTip;
	VERIFY(GetItemTooltipRect(hti, rTip, pKI));

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

