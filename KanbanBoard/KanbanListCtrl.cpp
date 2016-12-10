// KanbanListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KanbanListCtrl.h"
#include "Kanbanstatic.h"
#include "Kanbanenum.h"

#include "..\shared\graphicsMisc.h"
#include "..\shared\enstring.h"
#include "..\shared\misc.h"
#include "..\shared\datehelper.h"
#include "..\shared\timehelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef LVS_EX_DOUBLEBUFFER
#	define LVS_EX_DOUBLEBUFFER  0x00010000
#endif

#ifndef LVS_EX_LABELTIP
#	define LVS_EX_LABELTIP		0x00004000
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef WM_THEMECHANGED
#	define WM_THEMECHANGED 0x031A
#endif

/////////////////////////////////////////////////////////////////////////////

const int MIN_LABEL_EDIT_WIDTH	= 200;
const int BAR_WIDTH				= 6;

/////////////////////////////////////////////////////////////////////////////

CKanbanListCtrlArray::~CKanbanListCtrlArray()
{
	RemoveAll();
}


void CKanbanListCtrlArray::RemoveAll()
{
	int nList = GetSize();
	
	while (nList--)
		RemoveAt(nList);
}

BOOL CKanbanListCtrlArray::RemoveAt(int nList)
{
	if ((nList < 0) || (nList >= GetSize()))
	{
		ASSERT(0);
		return FALSE;
	}

	CKanbanListCtrl* pList = GetAt(nList);

	// remove it first before deleting
	CArray<CKanbanListCtrl*, CKanbanListCtrl*&>::RemoveAt(nList);

	if (pList->GetSafeHwnd())
		pList->DestroyWindow();

	delete pList;

	return TRUE;
}

int CKanbanListCtrlArray::FindListCtrl(DWORD dwTaskID) const
{
	if (dwTaskID)
	{
		int nList = GetSize();
		
		while (nList--)
		{
			if (GetAt(nList)->FindTask(dwTaskID) != -1)
				return nList;
		}
	}

	// else
	ASSERT(0);
	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// CKanbanListCtrl

CKanbanListCtrl::CKanbanListCtrl(const CKanbanItemMap& data, const KANBANCOLUMN& columnDef,
								CFontCache& fonts, const CDWordArray& aPriorityColors, 
								const CKanbanAttributeArray& aDisplayAttrib)
	:
	m_data(data),
	m_columnDef(columnDef),
	m_aDisplayAttrib(aDisplayAttrib),
	m_fonts(fonts),
	m_aPriorityColors(aPriorityColors),
	m_bStrikeThruDoneTasks(TRUE),
	m_bTextColorIsBkgnd(FALSE),
	m_bSelected(FALSE),
	m_bShowTaskColorAsBar(TRUE),
	m_bColorByPriority(FALSE),
	m_nLineHeight(-1)
{
}

CKanbanListCtrl::~CKanbanListCtrl()
{
}

IMPLEMENT_DYNAMIC(CKanbanListCtrl, CListCtrl);

BEGIN_MESSAGE_MAP(CKanbanListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CKanbanListCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnListCustomDraw)
	ON_NOTIFY(NM_CUSTOMDRAW, 0, OnHeaderCustomDraw)
	ON_MESSAGE(WM_THEMECHANGED, OnThemeChanged)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKanbanListCtrl message handlers

BOOL CKanbanListCtrl::Create(UINT nID, CWnd* pParentWnd)
{
	UINT nFlags = (WS_CHILD | WS_VISIBLE | /*WS_BORDER |*/ LVS_REPORT | WS_TABSTOP | LVS_SHOWSELALWAYS | LVS_NOSORTHEADER);

	return CListCtrl::Create(nFlags, CRect(0, 0, 0, 0), pParentWnd, nID);
}

int CKanbanListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	CFont* pFont = m_fonts.GetFont();
	ASSERT(pFont);

	if (pFont)
		SendMessage(WM_SETFONT, (WPARAM)pFont->GetSafeHandle());
	
	// the only column
	InsertColumn(0, _T(""), LVCFMT_RIGHT, 100);
	
	SetExtendedStyle(GetExtendedStyle() | (LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP));

	VERIFY(m_header.SubclassWindow(ListView_GetHeader(*this)));
	m_header.EnableTracking(FALSE);
	m_header.ModifyStyle(HDS_FULLDRAG | HDS_DRAGDROP, 0, 0);

	RefreshColumnTitle();
	OnDisplayAttributeChanged();
	RefreshBkgndColor();
	
	return 0;
}

LRESULT CKanbanListCtrl::OnThemeChanged(WPARAM /*wp*/, LPARAM /*lp*/)
{
	RefreshBkgndColor();
	return 0L;
}

void CKanbanListCtrl::RefreshBkgndColor()
{
	if (!Misc::IsHighContrastActive() && (m_columnDef.crBackground != CLR_NONE))
		ListView_SetBkColor(*this, m_columnDef.crBackground);
	else
		ListView_SetBkColor(*this, GetSysColor(COLOR_WINDOW));
}

void CKanbanListCtrl::SetBackgroundColor(COLORREF color)
{
	if (color == GetSysColor(COLOR_WINDOW))
		color = CLR_NONE;

	if (m_columnDef.crBackground != color)
	{
		m_columnDef.crBackground = color;
		RefreshBkgndColor();
		Invalidate(TRUE);
	}
}

void CKanbanListCtrl::SetExcessColor(COLORREF color)
{
	// TODO
	ASSERT(0);
}

void CKanbanListCtrl::SetMaximumTaskCount(int nMaxTasks)
{
	// TODO
	ASSERT(0);
}

void CKanbanListCtrl::OnDisplayAttributeChanged()
{
	SetImageList(NULL, LVSIL_SMALL);
	m_ilHeight.DeleteImageList();

	m_nLineHeight = CalcLineHeight();
	int nItemHeight = CalcRequiredItemHeight();

	m_ilHeight.Create(1, nItemHeight, ILC_COLOR32, 1, 0);
	
	SetImageList(&m_ilHeight, LVSIL_SMALL);

	// Update need for scrollbar
	BOOL bHasVScrollbar = (GetStyle() & WS_VSCROLL);
	BOOL bNeedVScrollbar = NeedVScrollbar();

	if (bNeedVScrollbar)
		ModifyStyle(0, WS_VSCROLL);
	else
		ModifyStyle(WS_VSCROLL, 0);

	SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER); 

	// Force scroll event to update scrollbar
	if ((bNeedVScrollbar && !bHasVScrollbar) || (!bNeedVScrollbar && bHasVScrollbar))
	{
		SendMessage(WM_VSCROLL, SB_TOP, 0);
		ScrollToSelection();
	}
}

BOOL CKanbanListCtrl::NeedVScrollbar() const
{
	// Calc distance between first and last items
	int nNumItems = GetItemCount();

	if (!nNumItems)
		return FALSE;

	CRect rFirst;
	GetItemRect(0, rFirst, LVIR_BOUNDS);

	int nDistance = 0;

	if (nNumItems == 1)
	{
		nDistance = rFirst.Height();
	}
	else // > 1
	{
		CRect rLast;
		GetItemRect((nNumItems - 1), rLast, LVIR_BOUNDS);

		nDistance = (rLast.bottom - rFirst.top);
	}

	// and compare to client height
	CRect rClient;
	GetClientRect(rClient);

	return (rClient.Height() < nDistance);
}

void CKanbanListCtrl::SetTextColorIsBackground(BOOL bSet)
{
	if (bSet != m_bTextColorIsBkgnd)
	{
		m_bTextColorIsBkgnd = bSet;

		if (GetSafeHwnd())
			Invalidate(FALSE);
	}
}

void CKanbanListCtrl::SetShowTaskColorAsBar(BOOL bSet)
{
	if (bSet != m_bShowTaskColorAsBar)
	{
		m_bShowTaskColorAsBar = bSet;
		
		if (GetSafeHwnd())
			Invalidate(FALSE);
	}
}

void CKanbanListCtrl::SetStrikeThruDoneTasks(BOOL bSet)
{
	if (bSet != m_bStrikeThruDoneTasks)
	{
		m_bStrikeThruDoneTasks = bSet;

		if (GetSafeHwnd())
			Invalidate(FALSE);
	}
}

void CKanbanListCtrl::SetColorTasksByPriority(BOOL bSet)
{
	if (bSet && (m_aPriorityColors.GetSize() != 11))
	{
		ASSERT(0);
		return;
	}

	// else
	m_bColorByPriority = bSet;

	if (GetSafeHwnd())
		Invalidate(TRUE);
}

void CKanbanListCtrl::SetSelected(BOOL bSelected)
{
	m_bSelected = bSelected;

	if (m_header.GetSafeHwnd())
		m_header.Invalidate(TRUE);
}

int CKanbanListCtrl::CalcRequiredItemHeight(int nNumLines) const
{
	ASSERT(m_nLineHeight != -1);

	if (nNumLines == -1)
		nNumLines = (1 + m_aDisplayAttrib.GetSize());

	return ((m_nLineHeight * nNumLines) + 6);
}

int CKanbanListCtrl::CalcLineHeight() const
{
	HFONT hFont = (HFONT)::SendMessage(GetSafeHwnd(), WM_GETFONT, 0, 0);
	int nLineHeight = GraphicsMisc::GetFontPixelSize(hFont);

	return (nLineHeight + 1);
}

int CKanbanListCtrl::AddTask(LPCTSTR szTitle, DWORD dwTaskID, BOOL bSelect)
{
	int nNewItem = InsertItem(GetItemCount(), szTitle, -1);
	ASSERT(nNewItem != -1);

	if (nNewItem != -1)
	{
		SetItemData(nNewItem, dwTaskID);
		
		// select item and make visible
		if (bSelect)
		{
			SetItemState(nNewItem, LVIS_SELECTED, LVIS_SELECTED);
			EnsureVisible(nNewItem, FALSE);
		}

		RefreshColumnTitle();
	}

	return nNewItem;
}

CString CKanbanListCtrl::GetAttributeID() const 
{ 
	return m_columnDef.sAttribID; 
}

BOOL CKanbanListCtrl::IsBacklog() const
{
	return ((m_columnDef.aAttribValues.GetSize() == 1) && (m_columnDef.aAttribValues[0].IsEmpty()));
}

int CKanbanListCtrl::GetAttributeValues(CStringArray& aValues) const 
{ 
	aValues.Copy(m_columnDef.aAttribValues); 
	return aValues.GetSize();
}

int CKanbanListCtrl::GetAttributeValueIDs(CStringArray& aValueIDs) const 
{ 
	aValueIDs.Copy(m_columnDef.aAttribValues); 
	Misc::MakeUpper(aValueIDs);

	return aValueIDs.GetSize();
}

CString CKanbanListCtrl::GetAttributeValue() const
{
	return Misc::FormatArray(m_columnDef.aAttribValues);
}

CString CKanbanListCtrl::GetAttributeValueID() const
{
	return Misc::ToUpper(GetAttributeValue());
}

BOOL CKanbanListCtrl::HasMultipleValues() const
{
	return (m_columnDef.aAttribValues.GetSize() > 1);
}

BOOL CKanbanListCtrl::HasAnyValues() const
{
	return (m_columnDef.aAttribValues.GetSize() > 0);
}

void CKanbanListCtrl::RefreshColumnTitle()
{
	CEnString sTitle(m_columnDef.sTitle);

	if (sTitle.IsEmpty())
		sTitle.LoadString(IDS_BACKLOG);

	CString sFormat;
	sFormat.Format(_T("%s (%d)"), sTitle, GetItemCount());

	m_header.SetItemText(0, sFormat);
}

void CKanbanListCtrl::OnListCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
	*pResult = CDRF_DODEFAULT;
	
	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
		
	case CDDS_ITEMPREPAINT:
		{
			const KANBANITEM* pKI = GetKanbanItem(pLVCD->nmcd.lItemlParam);
			//ASSERT(pKI);
			
			if (pKI)
			{
				int nItem = (int)pLVCD->nmcd.dwItemSpec;
				CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		
				// draw backgound
				CRect rItem;
				GetItemRect(nItem, rItem, LVIR_BOUNDS);
				rItem.DeflateRect(1, 1);
				
				BOOL bSelected = (GetItemState(nItem, LVIS_SELECTED) == LVIS_SELECTED);
				BOOL bFocused = (bSelected && (::GetFocus() == pNMHDR->hwndFrom));
				COLORREF crText = pKI->GetTextColor(bSelected, m_bTextColorIsBkgnd);

				if (bSelected)
				{
					GM_ITEMSTATE nState = (bFocused ? GMIS_SELECTED : GMIS_SELECTEDNOTFOCUSED);
					crText = GraphicsMisc::GetExplorerItemTextColor(crText, nState, GMIB_THEMECLASSIC);

					GraphicsMisc::DrawExplorerItemBkgnd(pDC, pNMHDR->hwndFrom, nState, rItem, GMIB_THEMECLASSIC);
				}
				else if (m_bShowTaskColorAsBar && !m_bTextColorIsBkgnd)
				{
					COLORREF crFill = GetSysColor(COLOR_WINDOW);
					COLORREF crBorder = GetSysColor(COLOR_WINDOWFRAME);
					
					GraphicsMisc::DrawRect(pDC, rItem, crFill, crBorder);
				}
				else
				{
					COLORREF crFill = pKI->GetFillColor(m_bTextColorIsBkgnd);
					COLORREF crBorder = pKI->GetBorderColor(m_bTextColorIsBkgnd);

					GraphicsMisc::DrawRect(pDC, rItem, crFill, crBorder);
				}

				// Draw bar as required
				if (m_bShowTaskColorAsBar && (!m_bTextColorIsBkgnd || m_bColorByPriority))
				{
					CRect rBar(rItem);
					
					rBar.DeflateRect(2, 2);
					rBar.right = (rBar.left + BAR_WIDTH);

					if (!pKI->IsDone(TRUE))
					{
						if (m_bColorByPriority)
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
				
				// draw text
				int nFlags = (DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX);
				
				// first line is the task title
				CRect rLine(rItem);
				rLine.DeflateRect(4, 1, 3, 2);

				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(crText);

				CFont* pOldFont = NULL;
				DWORD dwFontFlags = 0;

				if (m_bStrikeThruDoneTasks && pKI->IsDone(FALSE))
					dwFontFlags |= GMFS_STRIKETHRU;

				if (pKI->dwParentID == 0)
					dwFontFlags |= GMFS_BOLD;

				if (dwFontFlags)
					pOldFont = pDC->SelectObject(m_fonts.GetFont(dwFontFlags));
				
				DrawAttribute(pDC, rLine, 0, pKI->sTitle, nFlags);

				// Remove boldness for attributes
				if (dwFontFlags & GMFS_BOLD)
				{
					pDC->SelectObject(pOldFont);

					dwFontFlags &= ~GMFS_BOLD;
					pOldFont = pDC->SelectObject(m_fonts.GetFont(dwFontFlags));
				}

				// Attribute display
				if (!bSelected && !Misc::IsHighContrastActive() && !pKI->IsDone(TRUE))
					crText = pDC->SetTextColor(GraphicsMisc::Lighter(crText, 0.3));
				
				rLine.left += 6; // indent

				for (int nDisp = 0; nDisp < m_aDisplayAttrib.GetSize(); nDisp++)
				{
					IUI_ATTRIBUTE nAttrib = m_aDisplayAttrib[nDisp];
					UINT nFormatID = GetDisplayNameID(nAttrib);

					DrawAttribute(pDC, rLine, nFormatID, pKI->GetAttributeValue(nAttrib), nFlags);
				}

				if (pOldFont)
					pDC->SelectObject(pOldFont);
			}
			
			*pResult |= CDRF_SKIPDEFAULT;
		}
	}
}

UINT CKanbanListCtrl::GetDisplayNameID(IUI_ATTRIBUTE nAttrib)
{
	switch (nAttrib)
	{
	case IUI_ALLOCBY:		return IDS_DISPLAY_ALLOCBY;
	case IUI_ALLOCTO:		return IDS_DISPLAY_ALLOCTO;
	case IUI_CATEGORY:		return IDS_DISPLAY_CATEGORY;
	case IUI_COST:			return IDS_DISPLAY_COST;
	case IUI_CREATEDBY:		return IDS_DISPLAY_CREATEDBY;
	case IUI_CREATIONDATE:	return IDS_DISPLAY_CREATEDATE;
	case IUI_DONEDATE:		return IDS_DISPLAY_DONEDATE;
	case IUI_DUEDATE:		return IDS_DISPLAY_DUEDATE;
	case IUI_EXTERNALID:	return IDS_DISPLAY_EXTERNALID;
	case IUI_FLAG:			return IDS_DISPLAY_FLAG;
	case IUI_FILEREF:		return IDS_DISPLAY_FILEREF;
	case IUI_ID:			return IDS_DISPLAY_TASKID;
	case IUI_LASTMOD:		return IDS_DISPLAY_LASTMOD;
	case IUI_PARENT:		return IDS_DISPLAY_PARENT;
	case IUI_PERCENT:		return IDS_DISPLAY_PERCENT;
	case IUI_PRIORITY:		return IDS_DISPLAY_PRIORITY;
	case IUI_RECURRENCE:	return IDS_DISPLAY_RECURRENCE;
	case IUI_RISK:			return IDS_DISPLAY_RISK;
	case IUI_STARTDATE:		return IDS_DISPLAY_STARTDATE;
	case IUI_STATUS:		return IDS_DISPLAY_STATUS;
	case IUI_TAGS:			return IDS_DISPLAY_TAGS;
	case IUI_TIMEEST:		return IDS_DISPLAY_TIMEEST;
	case IUI_TIMESPENT:		return IDS_DISPLAY_TIMESPENT;
	case IUI_VERSION:		return IDS_DISPLAY_VERSION;
	}

	ASSERT(0);
	return 0;

}

void CKanbanListCtrl::DrawAttribute(CDC* pDC, CRect& rLine, UINT nFormatID, const CString& sValue, int nFlags) const
{
	CEnString sAttrib;
				
	if (nFormatID)
	{
		sAttrib.LoadString(nFormatID);
		sAttrib += _T(": ");

		if (!sValue.IsEmpty())
			sAttrib += sValue;
// 		else
// 			sAttrib += CEnString(IDS_NONE);
	}
	else
	{
		sAttrib = sValue;
	}

	pDC->DrawText(sAttrib, rLine, nFlags);

	rLine.top += m_nLineHeight;
}

void CKanbanListCtrl::OnHeaderCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMCUSTOMDRAW* pNMCD = (NMCUSTOMDRAW*)pNMHDR;
	*pResult = CDRF_DODEFAULT;

	HWND hwndHdr = pNMCD->hdr.hwndFrom;

	//ASSERT(hwndHdr == m_header);
	if (hwndHdr != m_header)
		return;
	
	switch (pNMCD->dwDrawStage)
	{
	case CDDS_PREPAINT:
		// Handle RTL text column headers and selected column
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
		
	case CDDS_ITEMPREPAINT:
		if (GraphicsMisc::GetRTLDrawTextFlags(hwndHdr) == DT_RTLREADING)
		{
			*pResult = CDRF_NOTIFYPOSTPAINT;
		}
		else if (m_bSelected)
		{
			CDC* pDC = CDC::FromHandle(pNMCD->hdc);
			pDC->SelectObject(m_fonts.GetFont(GMFS_BOLD));

			*pResult = CDRF_NEWFONT;
		}
		break;
		
	case CDDS_ITEMPOSTPAINT:
		{
			ASSERT(GraphicsMisc::GetRTLDrawTextFlags(hwndHdr) == DT_RTLREADING);

			CRect rItem(pNMCD->rc);
			rItem.DeflateRect(3, 0);

			CDC* pDC = CDC::FromHandle(pNMCD->hdc);
			UINT nFlags = (DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | GraphicsMisc::GetRTLDrawTextFlags(hwndHdr));
			
			pDC->SetBkMode(TRANSPARENT);

			// Show the text of the selected column in bold
			CFont* pOldFont = (m_bSelected ? pDC->SelectObject(m_fonts.GetFont(GMFS_BOLD)) : NULL);
			pDC->DrawText(m_header.GetItemText(0), rItem, nFlags);

			if (m_bSelected)
				pDC->SelectObject(pOldFont);
			
			*pResult = CDRF_SKIPDEFAULT;
		}
		break;
	}
}

BOOL CKanbanListCtrl::GetLabelEditRect(LPRECT pEdit)
{
	if (!m_bSelected || !GetItemCount())
	{
		ASSERT(0);
		return FALSE;
	}

	ASSERT(GetSelectedCount() == 1);

	POSITION pos = GetFirstSelectedItemPosition();
	int nSelItem = GetNextSelectedItem(pos);

	if (nSelItem < 0)
	{
		ASSERT(0);
		return FALSE;
	}

	// scroll into view first
	EnsureVisible(nSelItem, FALSE);

	CRect rItem;

	if (GetItemRect(nSelItem, rItem, LVIR_BOUNDS))
	{
		// Adjust rect as we do in OnListCustomDraw
		rItem.DeflateRect(3, 1, 1, 0);

		if (m_bShowTaskColorAsBar)
			rItem.left += (2 + BAR_WIDTH);

		// make width of list column or 200 whichever is larger
		int nWidth = max(rItem.Width(), MIN_LABEL_EDIT_WIDTH);

		rItem.right = rItem.left + nWidth;
		rItem.bottom = (rItem.top + CalcRequiredItemHeight(1));

		*pEdit = rItem;

		return TRUE;
	}

	return FALSE;
}

int CKanbanListCtrl::GetSelectedTasks(CDWordArray& aItemIDs) const
{
	aItemIDs.RemoveAll();
	POSITION pos = GetFirstSelectedItemPosition();
	
	while (pos)
		aItemIDs.Add(GetItemData(GetNextSelectedItem(pos)));

	return aItemIDs.GetSize();
}

void CKanbanListCtrl::ScrollToSelection()
{
	POSITION pos = GetFirstSelectedItemPosition();

	if (pos)
		EnsureVisible(GetNextSelectedItem(pos), FALSE);
}

BOOL CKanbanListCtrl::SelectItem(int nItem, BOOL bFocus)
{
	UINT nMask = (LVIS_SELECTED | (bFocus ? LVIS_FOCUSED : 0));

	if (nItem == -1)
		return SetItemState(-1, 0, nMask); // deselect all
	
	// else
	return SetItemState(nItem, nMask, nMask);
}

BOOL CKanbanListCtrl::SelectTask(DWORD dwTaskID)
{
	int nItem = FindTask(dwTaskID);

	if (nItem != -1)
	{
		SelectItem(nItem, TRUE);
		return TRUE;
	}

	// not found
	return FALSE;
}

BOOL CKanbanListCtrl::SelectTasks(const CDWordArray& aTaskIDs)
{
	// make sure we have all the items first
	CArray<int, int> aItems;

	int nID = aTaskIDs.GetSize();

	while (nID--)
	{
		int nItem = FindTask(aTaskIDs[nID]);

		if (nItem == -1)
			return FALSE;

		aItems.Add(nItem);
	}

	// deselect all and reselect
	SelectItem(-1);

	if (aItems.GetSize())
	{
		nID = aItems.GetSize();

		// set the first item to be focused
		while (nID--)
			SelectItem(aItems[nID], (nID == 0));
	}
	
	return TRUE;
}

const KANBANITEM* CKanbanListCtrl::GetKanbanItem(DWORD dwTaskID) const
{
	ASSERT(dwTaskID);
	
	KANBANITEM* pKI = NULL;
	
	if (dwTaskID && m_data.Lookup(dwTaskID, pKI))
	{
		ASSERT(pKI);
	}
	
	return pKI;
}

int CKanbanListCtrl::FindTask(DWORD dwTaskID) const
{
	LVFINDINFO lvfi = { 0 };
	
	lvfi.flags = LVFI_PARAM;
	lvfi.lParam = dwTaskID;
	
	return CListCtrl::FindItem(&lvfi);
}

int CKanbanListCtrl::FindTask(const CPoint& ptScreen) const
{
	LVFINDINFO lvfi = { 0 };
	
	lvfi.flags = LVFI_NEARESTXY;
	lvfi.pt = ptScreen;

	ScreenToClient(&lvfi.pt);
	
	return CListCtrl::FindItem(&lvfi);
}

int CALLBACK CKanbanListCtrl::SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	const KANBANSORT* pSort = (KANBANSORT*)lParamSort;
	
	const KANBANITEM* pKI1 = pSort->data.GetItem(lParam1);
	const KANBANITEM* pKI2 = pSort->data.GetItem(lParam2);
	
	int nCompare = 0;
	
	if (pKI1 && pKI2)
	{
		switch (pSort->nBy)
		{
		case IUI_TASKNAME:
			nCompare = Misc::NaturalCompare(pKI1->sTitle, pKI2->sTitle);
			break;
			
		case IUI_ALLOCBY:
		case IUI_ALLOCTO:
		case IUI_CATEGORY:
		case IUI_PRIORITY:
		case IUI_RISK:
		case IUI_STATUS:
		case IUI_TAGS:
		case IUI_VERSION:
		{
				ASSERT(!pSort->sAttribID.IsEmpty());

				CString sValue1 = pKI1->GetAttributeValue(pSort->sAttribID);
				CString sValue2 = pKI2->GetAttributeValue(pSort->sAttribID);

				nCompare = Misc::NaturalCompare(sValue1, sValue2);
			}
			break;

		case IUI_CUSTOMATTRIB:
			// TODO
			break;

		// Other display attributes
		case IUI_COST:
			nCompare = ((pKI1->dCost > pKI2->dCost) ? 1 : -1);
			break;
			
		case IUI_CREATIONDATE:
			nCompare = CDateHelper::Compare(pKI1->dtCreate, pKI2->dtCreate);
			break;
			
		case IUI_CREATEDBY:
			nCompare = Misc::NaturalCompare(pKI1->sCreatedBy, pKI2->sCreatedBy);
			break;
			
		case IUI_DONEDATE:
			nCompare = CDateHelper::Compare(pKI1->dtDone, pKI2->dtDone);
			break;
			
		case IUI_DUEDATE:
			nCompare = CDateHelper::Compare(pKI1->dtDue, pKI2->dtDue);
			break;
			
		case IUI_EXTERNALID:
			nCompare = Misc::NaturalCompare(pKI1->sExternalID, pKI2->sExternalID);
			break;
			
		case IUI_FLAG:
			//nCompare = Misc::NaturalCompare(pKI1->sFTitle, pKI2->sTitle);
			break;
			
		case IUI_LASTMOD:
			nCompare = CDateHelper::Compare(pKI1->dtLastMod, pKI2->dtLastMod);
			break;
			
		case IUI_PERCENT:
			nCompare = ((pKI1->nPercent > pKI2->nPercent) ? 1 : -1);
			break;
			
		case IUI_RECURRENCE:
			nCompare = Misc::NaturalCompare(pKI1->sRecurrence, pKI2->sRecurrence);
			break;
			
		case IUI_STARTDATE:
			nCompare = CDateHelper::Compare(pKI1->dtStart, pKI2->dtStart);
			break;
			
		case IUI_TIMEEST:
			nCompare = CTimeHelper().Compare(pKI1->dTimeEst, MapUnitsToTHUnits(pKI1->nTimeEstUnits), 
											pKI2->dTimeEst, MapUnitsToTHUnits(pKI2->nTimeEstUnits));
			break;
			
		case IUI_TIMESPENT:
			nCompare = CTimeHelper().Compare(pKI1->dTimeSpent, MapUnitsToTHUnits(pKI1->nTimeSpentUnits), 
											pKI2->dTimeSpent, MapUnitsToTHUnits(pKI2->nTimeSpentUnits));
			break;
		}
	}
	
	return (pSort->bAscending ? nCompare : -nCompare);
}

void CKanbanListCtrl::Sort(IUI_ATTRIBUTE nBy, BOOL bAscending)
{
	KANBANSORT ks(m_data);
	
	ks.nBy = nBy;
	ks.bAscending = bAscending;

	switch (nBy)
	{
	case IUI_STATUS:
	case IUI_ALLOCTO:
	case IUI_CATEGORY:
	case IUI_ALLOCBY:
	case IUI_TAGS:
	case IUI_RISK:
	case IUI_PRIORITY:
	case IUI_VERSION:
		ks.sAttribID = KANBANITEM::GetAttribID(nBy);
		break;
	}

	SortItems(SortProc, (DWORD)&ks);
}

void CKanbanListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (HandleLButtonClick(point))
		return;

	// else
	CListCtrl::OnLButtonDown(nFlags, point);
}

void CKanbanListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (HandleLButtonClick(point))
		return;

	// else
	CListCtrl::OnLButtonDblClk(nFlags, point);
}

BOOL CKanbanListCtrl::HandleLButtonClick(CPoint point)
{
	// don't let the selection to be set to -1
	// when clicking below the last item
	if (HitTest(point) == -1)
	{
		ClientToScreen(&point);

		// we don't want to disable drag selecting
		if (!::DragDetect(GetSafeHwnd(), point))
		{
			// Still set focus
			SetFocus();

			TRACE(_T("Ate Listview ButtonDown\n"));
			return TRUE; // eat it
		}
	}
	
	// all else
	return FALSE;
}
