// KanbanListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KanbanListCtrlEx.h"
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

#ifndef TTM_ADJUSTRECT
#	define TTM_ADJUSTRECT       (WM_USER + 31)
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

/////////////////////////////////////////////////////////////////////////////

const int MIN_LABEL_EDIT_WIDTH	= GraphicsMisc::ScaleByDPIFactor(200);
const int BAR_WIDTH				= GraphicsMisc::ScaleByDPIFactor(6);
const int NUM_TEXTLINES			= 2;
const int TITLE_VPADDING		= GraphicsMisc::ScaleByDPIFactor(8);
const int LV_PADDING			= GraphicsMisc::ScaleByDPIFactor(3);
const int CHECKBOX_PADDING		= GraphicsMisc::ScaleByDPIFactor(1);
const int ICON_OFFSET			= GraphicsMisc::ScaleByDPIFactor(2);
const int ATTRIB_INDENT			= GraphicsMisc::ScaleByDPIFactor(6);
const int TIP_PADDING			= GraphicsMisc::ScaleByDPIFactor(4);
const int IMAGE_SIZE			= GraphicsMisc::ScaleByDPIFactor(16);
const int LEVEL_INDENT			= GraphicsMisc::ScaleByDPIFactor(16);

const CRect TEXT_BORDER			= CRect(4, 3, 3, 2);

/////////////////////////////////////////////////////////////////////////////

CKanbanListCtrlExArray::~CKanbanListCtrlExArray()
{
	RemoveAll();
}

void CKanbanListCtrlExArray::RemoveAll()
{
	int nList = GetSize();
	
	while (nList--)
		RemoveAt(nList);
}

BOOL CKanbanListCtrlExArray::RemoveAt(int nList)
{
	if ((nList < 0) || (nList >= GetSize()))
	{
		ASSERT(0);
		return FALSE;
	}

	CKanbanListCtrlEx* pList = GetAt(nList);

	// remove it first before deleting
	CArray<CKanbanListCtrlEx*, CKanbanListCtrlEx*&>::RemoveAt(nList);

	if (pList->GetSafeHwnd())
		pList->DestroyWindow();

	delete pList;

	return TRUE;
}

int CKanbanListCtrlExArray::Find(DWORD dwTaskID) const
{
	HTREEITEM htiUnused;
	return Find(dwTaskID, htiUnused);
}

int CKanbanListCtrlExArray::Find(DWORD dwTaskID, HTREEITEM& hti) const
{
	if (dwTaskID)
	{
		int nNumList = GetSize();

		for (int nList = 0; nList < nNumList; nList++)
		{
			CKanbanListCtrlEx* pList = GetAt(nList);
			ASSERT(pList);

			hti = pList->FindTask(dwTaskID);
			
			if (hti)
				return nList;
		}
	}

	// not found
	hti = NULL;
	return -1;
}

int CKanbanListCtrlExArray::Find(const CString& sAttribValue) const
{
	CString sAttribValueID(Misc::ToUpper(sAttribValue));
	int nNumList = GetSize();

	for (int nList = 0; nList < nNumList; nList++)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		if (pList->GetAttributeValueID() == sAttribValueID)
			return nList;
	}

	// not found
	return -1;
}

int CKanbanListCtrlExArray::Find(HWND hwnd) const
{
	ASSERT(hwnd);

	int nNumList = GetSize();

	for (int nList = 0; nList < nNumList; nList++)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		if (pList->GetSafeHwnd() == hwnd)
			return nList;
	}

	// not found
	return -1;
}

int CKanbanListCtrlExArray::Find(const CKanbanListCtrlEx* pList) const
{
	int nList = GetSize();

	while (nList--)
	{
		if (GetAt(nList) == pList)
			return nList;
	}

	// not found
	return -1;
}

CKanbanListCtrlEx* CKanbanListCtrlExArray::Get(DWORD dwTaskID) const
{
	HTREEITEM htiUnused;
	return Get(dwTaskID, htiUnused);
}

CKanbanListCtrlEx* CKanbanListCtrlExArray::Get(DWORD dwTaskID, HTREEITEM& hti) const
{
	int nFind = Find(dwTaskID, hti);

	if (nFind != -1)
		return GetAt(nFind);

	// not found
	hti = NULL;
	return NULL;
}

CKanbanListCtrlEx* CKanbanListCtrlExArray::Get(const CString& sAttribValue) const
{
	int nFind = Find(sAttribValue);

	if (nFind != -1)
		return GetAt(nFind);

	// not found
	return NULL;
}

CKanbanListCtrlEx* CKanbanListCtrlExArray::Get(HWND hwnd) const
{
	int nFind = Find(hwnd);

	if (nFind != -1)
		return GetAt(nFind);

	// not found
	return NULL;
}

CKanbanListCtrlEx* CKanbanListCtrlExArray::GetFirstNonEmpty() const
{
	int nNumList = GetSize();

	for (int nList = 0; nList < nNumList; nList++)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		if (pList->GetCount())
			return pList;
	}

	// all empty
	return NULL;
}

CKanbanListCtrlEx* CKanbanListCtrlExArray::GetLastNonEmpty() const
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		if (pList->GetCount())
			return pList;
	}

	// all empty
	return NULL;
}

CKanbanListCtrlEx* CKanbanListCtrlExArray::GetBacklog() const
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		if (pList->IsBacklog())
			return pList;
	}

	// no backlog
	return NULL;
}

void CKanbanListCtrlExArray::OnDisplayAttributeChanged()
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		pList->OnDisplayAttributeChanged();
	}
}

void CKanbanListCtrlExArray::SetTextColorIsBackground(BOOL bSet)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		pList->SetTextColorIsBackground(bSet);
	}
}

void CKanbanListCtrlExArray::SetShowTaskColorAsBar(BOOL bSet)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		pList->SetShowTaskColorAsBar(bSet);
	}
}

void CKanbanListCtrlExArray::SetStrikeThruDoneTasks(BOOL bSet)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		pList->SetStrikeThruDoneTasks(bSet);
	}
}

void CKanbanListCtrlExArray::SetColorTaskBarByPriority(BOOL bSet)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		pList->SetColorTaskBarByPriority(bSet);
	}
}

void CKanbanListCtrlExArray::SetAttributeLabelVisibility(KBC_ATTRIBLABELS nLabelVis)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		pList->SetAttributeLabelVisibility(nLabelVis);
	}
}

void CKanbanListCtrlExArray::SetShowCompletionCheckboxes(BOOL bShow)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		pList->SetShowCompletionCheckboxes(bShow);
	}
}

void CKanbanListCtrlExArray::SetIndentSubtasks(BOOL bIndent)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		pList->SetIndentSubtasks(bIndent);
	}
}

void CKanbanListCtrlExArray::Exclude(CDC* pDC)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		CDialogHelper::ExcludeChild(pList, pDC);
	}
}

void CKanbanListCtrlExArray::SortItems(IUI_ATTRIBUTE nBy, BOOL bAscending, BOOL bSubtasksBelowParent)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		pList->Sort(nBy, bAscending, bSubtasksBelowParent);
	}
}

void CKanbanListCtrlExArray::Sort()
{
	if (GetSize() > 1)
		qsort(GetData(), GetSize(), sizeof(CKanbanListCtrlEx**), ListSortProc);
}

int CKanbanListCtrlExArray::ListSortProc(const void* pV1, const void* pV2)
{
	typedef CKanbanListCtrlEx* LPCKanbanListCtrlEx;

	const CKanbanListCtrlEx* pKLC1 = *(static_cast<const LPCKanbanListCtrlEx*>(pV1));
	const CKanbanListCtrlEx* pKLC2 = *(static_cast<const LPCKanbanListCtrlEx*>(pV2));

	// backlog always comes first
	if (!pKLC1->HasAnyValues())
		return -1;

	if (!pKLC2->HasAnyValues())
		return 1;

	// Sort by ID
	return Misc::NaturalCompare(pKLC1->GetAttributeValue(), pKLC2->GetAttributeValue());
}

int CKanbanListCtrlExArray::GetVisibleCount(BOOL bIncBacklog) const
{
	int nList = GetSize(), nNumVis = 0;

	while (nList--)
	{
		const CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		if (pList->GetCount() || (bIncBacklog && pList->IsBacklog()))
			nNumVis++;
	}

	return nNumVis;
}

int CKanbanListCtrlExArray::GetVisibleTaskCount() const
{
	int nList = GetSize(), nNumVis = 0;

	while (nList--)
	{
		const CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		nNumVis += pList->GetCount();
	}

	return nNumVis;
}

float CKanbanListCtrlExArray::GetAverageCharWidth()
{
	ASSERT(GetSize());

	if (!GetSize())
		return 1.0f;

	// else
	CWnd* pRef = GetAt(0);
	CClientDC dc(pRef);

	CFont* pOldFont = GraphicsMisc::PrepareDCFont(&dc, *pRef);
	float fAveCharWidth = GraphicsMisc::GetAverageCharWidth(&dc);

	dc.SelectObject(pOldFont);

	return fAveCharWidth;
}

CKanbanListCtrlEx* CKanbanListCtrlExArray::GetNext(const CKanbanListCtrlEx* pList, BOOL bNext, 
												BOOL bExcludeEmpty, BOOL bFixedColumns) const
{
	int nList = -1;

	if (pList)
	{
		nList = Misc::FindT(*this, pList);

		if (nList == -1)
		{
			ASSERT(0);
			return NULL;
		}
	}
	else if (bNext)
	{
		nList = -1;
	}
	else
	{
		nList = GetSize();
	}

	if (bNext)
	{
		for (int nNext = (nList + 1); nNext < GetSize(); nNext++)
		{
			CKanbanListCtrlEx* pNext = GetAt(nNext);
			ASSERT(pNext);

			if (bExcludeEmpty && !pNext->GetCount())
				continue;

			if (bFixedColumns && !pNext->IsWindowVisible())
				continue;

			// else
			return pNext;
		}

		// return to start
		return GetFirstNonEmpty();
	}
	else // prev
	{
		int nPrev(nList);

		while (nPrev--)
		{
			CKanbanListCtrlEx* pPrev = GetAt(nPrev);
			ASSERT(pPrev);

			if (bExcludeEmpty && !pPrev->GetCount())
				continue;

			if (bFixedColumns && !pPrev->IsWindowVisible())
				continue;

			// else
			return pPrev;
		}

		// return to end
		return GetLastNonEmpty();
	}

	return NULL;
}

CKanbanListCtrlEx* CKanbanListCtrlExArray::HitTest(const CPoint& ptScreen, BOOL* pbHeader) const
{
	HWND hwnd = ::WindowFromPoint(ptScreen);

	if (hwnd == NULL)
		return NULL;

	int nFind = Find(hwnd);
	CKanbanListCtrlEx* pList = ((nFind == -1) ? NULL : GetAt(nFind));

	// If not a list, try for a list's header
	if (!pList && pbHeader && CWinClasses::IsClass(hwnd, WC_HEADER))
	{
		hwnd = ::GetParent(hwnd);
		pList = Get(hwnd);

		*pbHeader = (pList != NULL);
	}

	return pList;
}

DWORD CKanbanListCtrlExArray::HitTestTask(const CPoint& ptScreen) const
{
	const CKanbanListCtrlEx* pList = HitTest(ptScreen);

	if (pList)
	{
		HTREEITEM hti = pList->FindTask(ptScreen);

		if (hti)
			return pList->GetTaskID(hti);
	}

	// else
	return 0;
}

void CKanbanListCtrlExArray::ClearOtherSelections(const CKanbanListCtrlEx* pIgnore)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		if (pList != pIgnore)
			pList->ClearSelection();

		pList->SetSelected(pList == pIgnore);
	}
}

void CKanbanListCtrlExArray::DeleteTaskFromOthers(DWORD dwTaskID, const CKanbanListCtrlEx* pIgnore)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		if (pList != pIgnore)
			pList->DeleteTask(dwTaskID);
	}
}

int CKanbanListCtrlExArray::CalcRequiredColumnWidthForImage() const
{
	int nMaxWidth = 0;
	int nList = GetSize();

	while (nList--)
	{
		const CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		int nListWidth = pList->CalcRequiredColumnWidthForImage();
		nMaxWidth = max(nMaxWidth, nListWidth);
	}

	return nMaxWidth;
}

BOOL CKanbanListCtrlExArray::CanSaveToImage() const
{
	// At least one column must have items
	// And the item count per page must be 1 or more
/*
	int nList = GetSize();

	while (nList--)
	{
		const CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		if (pList->GetCountPerPage() == 0)
			return FALSE;

		// else
		if (pList->GetCount())
			return TRUE;
	}
*/

	return FALSE;
}

BOOL CKanbanListCtrlExArray::SaveToImage(CBitmap& bmImage)
{
	if (!CanSaveToImage())
		return FALSE;

	int nNumLists = GetSize();

	CArray<CBitmap, CBitmap&> aListBmps;
	aListBmps.SetSize(nNumLists, 1);

	int nListWidth = 0, nListHeight = 0;
	int nReqColWidth = CalcRequiredColumnWidthForImage();

	for (int nList = 0; nList < nNumLists; nList++)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		CEnBitmap bmp;

		if (!pList->SaveToImage(bmp, nReqColWidth))
			return FALSE;

		CSize size = bmp.GetSize();
		nListHeight = max(nListHeight, size.cy);

		if (nListWidth == 0)
			nListWidth = size.cx;
		else
			ASSERT(nListWidth == size.cx);

		aListBmps[nList].Attach(bmp.Detach());
	}

	// Create some memory DCs
	CClientDC dc(GetAt(0));
	CDC dcImage, dcList;

	if (dcImage.CreateCompatibleDC(&dc) && dcList.CreateCompatibleDC(&dc))
	{
		// Create the image big enough to fit the lists side-by-side
		int nTotalWidth = (nNumLists * nListWidth);

		if (bmImage.CreateCompatibleBitmap(&dc, nTotalWidth, nListHeight))
		{
			CBitmap* pOldImage = dcImage.SelectObject(&bmImage);

			// Fill with background colour
			dcImage.FillSolidRect(0, 0, nTotalWidth, nListHeight, GetSysColor(COLOR_WINDOW));

			for (int nList = 0, nPos = 0; nList < nNumLists; nList++)
			{
				CBitmap* pOldList = dcList.SelectObject(&(aListBmps[nList]));

				dcImage.BitBlt(nPos, 0, nListWidth, nListHeight, &dcList, 0, 0, SRCCOPY);
				nPos += nListWidth;

				dcImage.FillSolidRect((nPos - 1) , 0, 1, nListHeight, GetSysColor(COLOR_3DFACE));
				dcList.SelectObject(pOldList);
			}
		}
	}

	return (bmImage.GetSafeHandle() != NULL);
}

void CKanbanListCtrlExArray::OnSetFont(HFONT hFont)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		pList->SendMessage(WM_SETFONT, (WPARAM)hFont);
	}
}

void CKanbanListCtrlExArray::Redraw(BOOL bErase)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		if (pList)
			pList->Invalidate(bErase);
	}
}

void CKanbanListCtrlExArray::RemoveDeletedTasks(const CDWordSet& mapCurIDs)
{
	// Go thru each list removing deleted items
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);

		HTREEITEM hti = pList->GetChildItem(NULL);

		while (hti)
		{
			DWORD dwTaskID = pList->GetTaskID(hti);
	
			// get next item before deleting this one
			HTREEITEM htiNext = pList->GetNextItem(hti, TVGN_NEXT);

			if (!mapCurIDs.Has(dwTaskID))
				pList->DeleteItem(hti);

			hti = htiNext;
		}
	}
}

void CKanbanListCtrlExArray::RefreshColumnTitles()
{
	int nList = GetSize();
	
	while (nList--)
	{
		CKanbanListCtrlEx* pList = GetAt(nList);
		ASSERT(pList);
		
		pList->RefreshColumnTitle();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CKanbanListCtrlEx

CKanbanListCtrlEx::CKanbanListCtrlEx(const CKanbanItemMap& data, const KANBANCOLUMN& columnDef,
								CFontCache& fonts, const CDWordArray& aPriorityColors, 
								const CKanbanAttributeArray& aDisplayAttrib)
	:
	m_data(data),
	m_tch(*this),
	m_columnDef(columnDef),
	m_aDisplayAttrib(aDisplayAttrib),
	m_fonts(fonts),
	m_aPriorityColors(aPriorityColors),
	m_bStrikeThruDoneTasks(FALSE),
	m_bIndentSubtasks(FALSE),
	m_bTextColorIsBkgnd(FALSE),
	m_bSelected(FALSE),
	m_bShowTaskColorAsBar(FALSE),
	m_bShowCompletionCheckboxes(FALSE),
	m_bColorBarByPriority(FALSE),
	m_nLineHeight(-1),
	m_nAttribLabelVisiability(KBCAL_LONG),
	m_bSavingToImage(FALSE)
{
}

CKanbanListCtrlEx::~CKanbanListCtrlEx()
{
}

IMPLEMENT_DYNAMIC(CKanbanListCtrlEx, CTreeCtrl);

BEGIN_MESSAGE_MAP(CKanbanListCtrlEx, CTreeCtrl)
	//{{AFX_MSG_MAP(CKanbanListCtrlEx)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnListCustomDraw)
//	ON_NOTIFY(NM_CUSTOMDRAW, 0, OnHeaderCustomDraw)
	ON_MESSAGE(WM_THEMECHANGED, OnThemeChanged)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_NOTIFY(TTN_SHOW, 0, OnTooltipShow)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_REGISTERED_MESSAGE(WM_TTC_TOOLHITTEST, OnToolHitTest)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKanbanListCtrlEx message handlers

BOOL CKanbanListCtrlEx::Create(UINT nID, CWnd* pParentWnd)
{
	UINT nFlags = (WS_CHILD | WS_VISIBLE | LVS_REPORT | WS_TABSTOP | TVS_SHOWSELALWAYS | TVS_EDITLABELS | TVS_FULLROWSELECT | TVS_NOTOOLTIPS);

	return CTreeCtrl::Create(nFlags, CRect(0, 0, 0, 0), pParentWnd, nID);
}

int CKanbanListCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	VERIFY(InitTooltip());

	CFont* pFont = m_fonts.GetFont();
	ASSERT(pFont);

	if (pFont)
		SendMessage(WM_SETFONT, (WPARAM)pFont->GetSafeHandle());
	
	// the only column
//	InsertColumn(0, _T(""), LVCFMT_RIGHT, 100);
	
// 	DWORD dwExStyle = GetExtendedStyle();
// 	dwExStyle |= (TVS_EX_DOUBLEBUFFER);
// 	SetExtendedStyle(dwExStyle);

	if (GraphicsMisc::InitCheckboxImageList(*this, m_ilCheckboxes, IDB_CHECKBOXES, 255))
	{
		//TreeView_SetCallbackMask(TVIS_STATEIMAGEMASK);
		SetImageList(&m_ilCheckboxes, TVSIL_STATE);
	}
	
// 	VERIFY(m_header.SubclassWindow(ListView_GetHeader(*this)));
// 	m_header.EnableTracking(FALSE);
// 	m_header.ModifyStyle(HDS_FULLDRAG | HDS_DRAGDROP, 0, 0);

	m_ilFlags.Create(IDB_FLAG, 16, 1, RGB(255, 0, 255));
	CEnImageList::ScaleByDPIFactor(m_ilFlags);

	RefreshColumnTitle();
	OnDisplayAttributeChanged();
	RefreshBkgndColor();

	return 0;
}

LRESULT CKanbanListCtrlEx::OnThemeChanged(WPARAM /*wp*/, LPARAM /*lp*/)
{
	RefreshBkgndColor();
	return 0L;
}

void CKanbanListCtrlEx::RefreshBkgndColor()
{
	if (!Misc::IsHighContrastActive() && (m_columnDef.crBackground != CLR_NONE))
		ListView_SetBkColor(*this, m_columnDef.crBackground);
	else
		ListView_SetBkColor(*this, GetSysColor(COLOR_WINDOW));
}

void CKanbanListCtrlEx::SetBackgroundColor(COLORREF color)
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

void CKanbanListCtrlEx::SetExcessColor(COLORREF color)
{
	// TODO
	ASSERT(0);
}

void CKanbanListCtrlEx::SetMaximumTaskCount(int nMaxTasks)
{
	// TODO
	ASSERT(0);
}

void CKanbanListCtrlEx::OnDisplayAttributeChanged()
{
	SetImageList(NULL, LVSIL_SMALL);
	m_ilHeight.DeleteImageList();

	m_nLineHeight = CalcLineHeight();
	int nItemHeight = CalcRequiredItemHeight();

	// Use an image list to force required height
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

BOOL CKanbanListCtrlEx::NeedVScrollbar() const
{
	// Calc distance between first and last items
/*
	int nNumItems = GetCount();

	if (!nNumItems)
		return FALSE;

	CRect rFirst;
	GetItemRect(GetChildItem(NULL), rFirst, NULL);

	int nDistance = 0;

	if (nNumItems == 1)
	{
		nDistance = rFirst.Height();
	}
	else // > 1
	{
		CRect rLast;
		GetItemRect((nNumItems - 1), rLast, NULL);

		nDistance = (rLast.bottom - rFirst.top);
	}

	// and compare to client height
	CRect rClient;
	GetClientRect(rClient);

	return (rClient.Height() < nDistance);
*/
	return TRUE;
}

void CKanbanListCtrlEx::SetTextColorIsBackground(BOOL bSet)
{
	if (bSet != m_bTextColorIsBkgnd)
	{
		m_bTextColorIsBkgnd = bSet;

		if (GetSafeHwnd())
			Invalidate(FALSE);
	}
}

void CKanbanListCtrlEx::SetShowTaskColorAsBar(BOOL bSet)
{
	if (bSet != m_bShowTaskColorAsBar)
	{
		m_bShowTaskColorAsBar = bSet;
		
		if (GetSafeHwnd())
			Invalidate(FALSE);
	}
}

void CKanbanListCtrlEx::SetStrikeThruDoneTasks(BOOL bSet)
{
	if (bSet != m_bStrikeThruDoneTasks)
	{
		m_bStrikeThruDoneTasks = bSet;

		if (GetSafeHwnd())
			Invalidate(FALSE);
	}
}

void CKanbanListCtrlEx::SetColorTaskBarByPriority(BOOL bSet)
{
	if (bSet && (m_aPriorityColors.GetSize() != 11))
	{
		ASSERT(0);
		return;
	}

	// else
	m_bColorBarByPriority = bSet;

	if (GetSafeHwnd())
		Invalidate(TRUE);
}

void CKanbanListCtrlEx::SetAttributeLabelVisibility(KBC_ATTRIBLABELS nLabelVis)
{
	m_nAttribLabelVisiability = nLabelVis;

	if (GetSafeHwnd())
		Invalidate(TRUE);
}

void CKanbanListCtrlEx::SetSelected(BOOL bSelected)
{
	m_bSelected = bSelected;

// 	if (m_header.GetSafeHwnd())
// 		m_header.Invalidate(TRUE);
}

void CKanbanListCtrlEx::SetShowCompletionCheckboxes(BOOL bShow)
{
	m_bShowCompletionCheckboxes = bShow;

	if (GetSafeHwnd())
		Invalidate(TRUE);
}

void CKanbanListCtrlEx::SetIndentSubtasks(BOOL bIndent)
{
	m_bIndentSubtasks = bIndent;

	if (GetSafeHwnd())
		Invalidate(TRUE);
}

int CKanbanListCtrlEx::CalcRequiredItemHeight(int nNumLines) const
{
	ASSERT(m_nLineHeight != -1);

	int nHeight = TITLE_VPADDING;

	if (nNumLines == -1) // Full height of item
	{
		nHeight += CalcItemTitleTextHeight();
		nHeight += (m_aDisplayAttrib.GetSize() * m_nLineHeight);
	}
	else
	{
		nHeight += (m_nLineHeight * nNumLines);
	}

	return nHeight;
}

int CKanbanListCtrlEx::CalcItemTitleTextHeight() const
{
	ASSERT(m_nLineHeight != -1);

	return ((NUM_TEXTLINES * m_nLineHeight) + 2);
}

int CKanbanListCtrlEx::CalcLineHeight() const
{
	int nLineHeight = GraphicsMisc::GetTextHeight(_T("A"), *this, 0xFFFF);

	return nLineHeight;
}

HTREEITEM CKanbanListCtrlEx::AddTask(const KANBANITEM& ki, BOOL bSelect)
{
	HTREEITEM hti = FindTask(ki.dwTaskID);

	if (hti)
	{
		ASSERT(m_columnDef.aAttribValues.GetSize() > 1);
		return hti;
	}

	hti = InsertItem(TVIF_TEXT | TVIF_PARAM, 
					 ki.sTitle,
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
		// select item and make visible
		if (bSelect)
		{
			SetItemState(hti, TVIS_SELECTED, TVIS_SELECTED);
			EnsureVisible(hti);
		}

		RefreshColumnTitle();
	}

	return hti;
}

CString CKanbanListCtrlEx::GetAttributeID() const 
{ 
	return m_columnDef.sAttribID; 
}

BOOL CKanbanListCtrlEx::IsBacklog() const
{
	return ((m_columnDef.aAttribValues.GetSize() == 1) && (m_columnDef.aAttribValues[0].IsEmpty()));
}

int CKanbanListCtrlEx::GetAttributeValues(CStringArray& aValues) const 
{ 
	aValues.Copy(m_columnDef.aAttribValues); 
	return aValues.GetSize();
}

int CKanbanListCtrlEx::GetAttributeValueIDs(CStringArray& aValueIDs) const 
{ 
	aValueIDs.Copy(m_columnDef.aAttribValues); 
	Misc::MakeUpper(aValueIDs);

	return aValueIDs.GetSize();
}

CString CKanbanListCtrlEx::GetAttributeValue() const
{
	return Misc::FormatArray(m_columnDef.aAttribValues);
}

CString CKanbanListCtrlEx::GetAttributeValueID() const
{
	return Misc::ToUpper(GetAttributeValue());
}

BOOL CKanbanListCtrlEx::HasMultipleValues() const
{
	return (m_columnDef.aAttribValues.GetSize() > 1);
}

BOOL CKanbanListCtrlEx::HasAnyValues() const
{
	return (m_columnDef.aAttribValues.GetSize() > 0);
}

void CKanbanListCtrlEx::RefreshColumnTitle()
{
	CEnString sTitle(m_columnDef.sTitle);

	if (sTitle.IsEmpty())
		sTitle.LoadString(IDS_BACKLOG);

	CString sFormat;
	sFormat.Format(_T("%s (%d)"), sTitle, GetCount());

//	m_header.SetItemText(0, sFormat);
}

int CKanbanListCtrlEx::CalcAvailableAttributeWidth(int nListWidth) const
{
	if (nListWidth < 0)
	{
		CRect rClient;
		GetClientRect(rClient);
		nListWidth = rClient.Width();
	}
	else
	{
		nListWidth -= GetSystemMetrics(SM_CXVSCROLL);
	}

	int nAvailWidth = (nListWidth - (2 * LV_PADDING));

	if (m_bShowTaskColorAsBar)
		nAvailWidth -= BAR_WIDTH;

	nAvailWidth -= (TEXT_BORDER.left + TEXT_BORDER.right);
	nAvailWidth -= ATTRIB_INDENT;

	return nAvailWidth;
}

void CKanbanListCtrlEx::OnSize(UINT nType, int cx, int cy)
{
	CTreeCtrl::OnSize(nType, cx, cy);

// 	if (!m_bSavingToImage && m_header.GetSafeHwnd() && m_header.GetCount())
// 	{
// 		if (GetStyle() & WS_VSCROLL)
// 			SetColumnWidth(0, (cx - 1));
// 		else
// 			SetColumnWidth(0, (cx - 1 - GetSystemMetrics(SM_CXVSCROLL)));
// 	}
}

void CKanbanListCtrlEx::FillItemBackground(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText, BOOL bSelected) const
{
	if (bSelected)
	{
		BOOL bFocused = (bSelected && (::GetFocus() == GetSafeHwnd()));

		GM_ITEMSTATE nState = (bFocused ? GMIS_SELECTED : GMIS_SELECTEDNOTFOCUSED);
		crText = GraphicsMisc::GetExplorerItemTextColor(crText, nState, GMIB_THEMECLASSIC);

		GraphicsMisc::DrawExplorerItemBkgnd(pDC, GetSafeHwnd(), nState, rItem, GMIB_THEMECLASSIC);
	}
	else if (m_bShowTaskColorAsBar)
	{
		COLORREF crFill = GetSysColor(COLOR_WINDOW);
		COLORREF crBorder = GetSysColor(COLOR_WINDOWFRAME);

		GraphicsMisc::DrawRect(pDC, rItem, crFill, crBorder);
	}
	else // use task's own colour
	{
		COLORREF crFill = pKI->GetFillColor(m_bTextColorIsBkgnd);
		COLORREF crBorder = pKI->GetBorderColor(m_bTextColorIsBkgnd);

		GraphicsMisc::DrawRect(pDC, rItem, crFill, crBorder);
	}
}

void CKanbanListCtrlEx::OnListCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
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
			const KANBANITEM* pKI = GetKanbanItem(pTVCD->nmcd.lItemlParam);
			
			if (pKI)
			{
				HTREEITEM hti = (HTREEITEM)pTVCD->nmcd.dwItemSpec;
				CDC* pDC = CDC::FromHandle(pTVCD->nmcd.hdc);
		
				CRect rItem;
				GetItemRect(hti, rItem, NULL);
				rItem.DeflateRect(1, 1);

				// Checkbox
				DrawItemCheckbox(pDC, pKI, rItem);
				
				BOOL bSelected = (!m_bSavingToImage && (GetItemState(hti, TVIS_SELECTED) == TVIS_SELECTED));
				COLORREF crText = pKI->GetTextColor(bSelected, (m_bTextColorIsBkgnd && !m_bShowTaskColorAsBar));

				// Background
				FillItemBackground(pDC, pKI, rItem, crText, bSelected);

				// Icon
				BOOL bHasIcon = DrawItemIcons(pDC, pKI, rItem);

				// Bar as required
				CRect rAttributes(rItem), rTitle(rItem);

				if (DrawItemBar(pDC, pKI, bHasIcon, rAttributes))
					rTitle = rAttributes;

				// Attributes are not affected by the icon
				if (bHasIcon)
					rTitle.left = max(rAttributes.left, (rItem.left + IMAGE_SIZE + ICON_OFFSET));
				
				// Text
				DrawItemTitle(pDC, pKI, rTitle, crText);

				if (!bSelected && !Misc::IsHighContrastActive() && !pKI->IsDone(TRUE))
					crText = pDC->SetTextColor(GraphicsMisc::Lighter(crText, 0.3));

				rAttributes.top += CalcItemTitleTextHeight();

				DrawItemAttributes(pDC, pKI, rAttributes, crText);
			}
			
			*pResult |= CDRF_SKIPDEFAULT;
		}
	}
}

void CKanbanListCtrlEx::DrawItemTitle(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText)
{
	CRect rTitle(rItem);

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

	// first 'n' lines is the task title
	rTitle.DeflateRect(TEXT_BORDER);
	rTitle.bottom = (rTitle.top + CalcItemTitleTextHeight());

	int nFlags = (DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX | DT_WORDBREAK); // multi-line for titles only

	pDC->DrawText(pKI->sTitle, rTitle, nFlags);

	if (pOldFont)
		pDC->SelectObject(pOldFont);
}

void CKanbanListCtrlEx::DrawItemAttributes(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem, COLORREF crText)
{
	pDC->SetBkMode(TRANSPARENT);

	CFont* pOldFont = NULL;
	DWORD dwFontFlags = 0;

	if (m_bStrikeThruDoneTasks && pKI->IsDone(FALSE))
		pOldFont = pDC->SelectObject(m_fonts.GetFont(GMFS_STRIKETHRU));

	CRect rAttrib(rItem);
	rAttrib.DeflateRect(TEXT_BORDER);

	int nFlags = (DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX);

	// Attribute display
	//rAttrib.left += ATTRIB_INDENT;

	for (int nDisp = 0; nDisp < m_aDisplayAttrib.GetSize(); nDisp++)
	{
		IUI_ATTRIBUTE nAttrib = m_aDisplayAttrib[nDisp];

		DrawAttribute(pDC, rAttrib, nAttrib, pKI->GetAttributeDisplayValue(nAttrib), nFlags);
	}

	if (pOldFont)
		pDC->SelectObject(pOldFont);
}

BOOL CKanbanListCtrlEx::DrawItemIcons(CDC* pDC, const KANBANITEM* pKI, const CRect& rItem) const
{
	BOOL bHasIcons = FALSE;
	CRect rIcon(rItem);

	rIcon.left += ICON_OFFSET;
	rIcon.top += ICON_OFFSET;

	if (pKI->bHasIcon || pKI->bParent)
	{
		int iImageIndex = -1;
		HIMAGELIST hilTask = (HIMAGELIST)GetParent()->SendMessage(WM_KLCN_GETTASKICON, pKI->dwTaskID, (LPARAM)&iImageIndex);

		if (hilTask && (iImageIndex != -1))
		{
			ImageList_Draw(hilTask, iImageIndex, *pDC, rIcon.left, rIcon.top, ILD_TRANSPARENT);
			bHasIcons = TRUE;

			rIcon.top += (IMAGE_SIZE + ICON_OFFSET);

			if (m_bShowTaskColorAsBar && !pKI->IsDone(TRUE))
				rIcon.left += BAR_WIDTH;
		}
	}

	if (pKI->bFlag)
	{
		ImageList_Draw(m_ilFlags, 0, *pDC, rIcon.left, rIcon.top, ILD_TRANSPARENT);
		bHasIcons = TRUE;
	}

	return bHasIcons;
}

BOOL CKanbanListCtrlEx::DrawItemBar(CDC* pDC, const KANBANITEM* pKI, BOOL bHasIcon, CRect& rItem) const
{
	if (m_bShowTaskColorAsBar)
	{
		// Don't draw for completed items but ensure same indentation
		CRect rBar(rItem);

		rBar.DeflateRect(2, 2);
		rBar.right = (rBar.left + BAR_WIDTH);

		if (bHasIcon)
			rBar.top += (IMAGE_SIZE + ICON_OFFSET);

		if (!pKI->IsDone(TRUE))
		{
			if (m_bColorBarByPriority)
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
		return TRUE;
	}

	return FALSE;
}

BOOL CKanbanListCtrlEx::DrawItemCheckbox(CDC* pDC, const KANBANITEM* pKI, CRect& rItem)
{
	if (m_bShowCompletionCheckboxes)
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

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CKanbanListCtrlEx::GetItemCheckboxRect(HTREEITEM hti, CRect& rItem, const KANBANITEM* pKI) const
{
	if (m_bShowCompletionCheckboxes)
	{
		GetItemRect(hti, rItem, pKI);

		return GetItemCheckboxRect(rItem);
	}

	// else
	return FALSE;
}

BOOL CKanbanListCtrlEx::GetItemBounds(HTREEITEM hti, LPRECT lpRect) const
{
	return CTreeCtrl::GetItemRect(hti, lpRect, LVIR_BOUNDS);
}

BOOL CKanbanListCtrlEx::GetItemRect(HTREEITEM hti, CRect& rItem, const KANBANITEM* pKI) const
{
	if (!GetItemBounds(hti, rItem))
		return FALSE;

	if (m_bIndentSubtasks)
	{
		if (!pKI)
			pKI = GetKanbanItem(GetTaskID(hti));

		ASSERT(pKI);

		// Indent to match level
		rItem.left += (pKI->nLevel * LEVEL_INDENT);
	}

	return TRUE;
}

BOOL CKanbanListCtrlEx::GetItemCheckboxRect(CRect& rItem) const
{
	if (m_bShowCompletionCheckboxes)
	{
		rItem.bottom = (rItem.top + CalcRequiredItemHeight(1));

		rItem.DeflateRect(0, ((rItem.Height() - CEnImageList::GetImageSize(m_ilCheckboxes)) / 2));
		rItem.right = (rItem.left + rItem.Height());

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CKanbanListCtrlEx::GetItemLabelTextRect(HTREEITEM hti, CRect& rItem, BOOL bEdit, const KANBANITEM* pKI) const
{
	if (!GetItemRect(hti, rItem, pKI))
		return FALSE;

	rItem.DeflateRect(1, 1);

	CRect rCheckbox(rItem);
	VERIFY(GetItemCheckboxRect(rCheckbox));

	rItem.left = (rCheckbox.right + CHECKBOX_PADDING);
	
	if (m_bShowTaskColorAsBar)
		rItem.left += (2 + BAR_WIDTH);

	if (!bEdit)
	{
		rItem.DeflateRect(TEXT_BORDER);
		rItem.bottom = (rItem.top + CalcItemTitleTextHeight());
		rItem.top--;
		rItem.right -= 2;
	}
	else
	{
		// make width of list column or 200 whichever is larger
		int nWidth = max(rItem.Width(), MIN_LABEL_EDIT_WIDTH);
			
		rItem.right = rItem.left + nWidth;
		rItem.bottom = (rItem.top + (CalcRequiredItemHeight(2) / 2));
	}

	return TRUE;
}

BOOL CKanbanListCtrlEx::GetItemTooltipRect(HTREEITEM hti, CRect& rTip, const KANBANITEM* pKI) const
{
	if (!pKI)
	{
		pKI = GetKanbanItem(GetTaskID(hti));

		if (!pKI)
		{
			ASSERT(0);
			return FALSE;
		}
	}

	if (!GetItemLabelTextRect(hti, rTip, FALSE, pKI))
	{
		ASSERT(0);
		return FALSE;
	}

	HFONT hFont = m_fonts.GetHFont((pKI->dwParentID == 0) ? GMFS_BOLD : 0);

	int nWidth = rTip.Width();
	int nAvailHeight = (NUM_TEXTLINES * m_nLineHeight);
	int nTextHeight = GraphicsMisc::GetTextHeight(pKI->sTitle, GetSafeHwnd(), nWidth, hFont);

	if (nTextHeight <= nAvailHeight)
		return FALSE;

	while (nTextHeight > nAvailHeight)
	{
		rTip.right += (nWidth / 2);
		nTextHeight = GraphicsMisc::GetTextHeight(pKI->sTitle, GetSafeHwnd(), rTip.Width(), hFont);
	}

	m_tooltip.AdjustRect(rTip, TRUE);
	ClientToScreen(&rTip);

	return TRUE;
}

UINT CKanbanListCtrlEx::GetDisplayFormat(IUI_ATTRIBUTE nAttrib, BOOL bLong)
{
	switch (nAttrib)
	{
	case IUI_ALLOCBY:		return (bLong ? IDS_DISPLAY_ALLOCBY : IDS_DISPLAY_ALLOCBY_SHORT);
	case IUI_ALLOCTO:		return (bLong ? IDS_DISPLAY_ALLOCTO : IDS_DISPLAY_ALLOCTO_SHORT);
	case IUI_CATEGORY:		return (bLong ? IDS_DISPLAY_CATEGORY : IDS_DISPLAY_CATEGORY_SHORT);
	case IUI_COST:			return (bLong ? IDS_DISPLAY_COST : IDS_DISPLAY_COST_SHORT);
	case IUI_CREATEDBY:		return (bLong ? IDS_DISPLAY_CREATEDBY : IDS_DISPLAY_CREATEDBY_SHORT);
	case IUI_CREATIONDATE:	return (bLong ? IDS_DISPLAY_CREATEDATE : IDS_DISPLAY_CREATEDATE_SHORT);
	case IUI_DONEDATE:		return (bLong ? IDS_DISPLAY_DONEDATE : IDS_DISPLAY_DONEDATE_SHORT);
	case IUI_DUEDATE:		return (bLong ? IDS_DISPLAY_DUEDATE : IDS_DISPLAY_DUEDATE_SHORT);
	case IUI_EXTERNALID:	return (bLong ? IDS_DISPLAY_EXTERNALID : IDS_DISPLAY_EXTERNALID_SHORT);
	case IUI_FLAG:			return (bLong ? IDS_DISPLAY_FLAG : IDS_DISPLAY_FLAG_SHORT);
	case IUI_FILEREF:		return (bLong ? IDS_DISPLAY_FILEREF : IDS_DISPLAY_FILEREF_SHORT);
	case IUI_ID:			return (bLong ? IDS_DISPLAY_TASKID : IDS_DISPLAY_TASKID_SHORT);
	case IUI_LASTMOD:		return (bLong ? IDS_DISPLAY_LASTMOD : IDS_DISPLAY_LASTMOD_SHORT);
	case IUI_PARENT:		return (bLong ? IDS_DISPLAY_PARENT : IDS_DISPLAY_PARENT_SHORT);
	case IUI_PERCENT:		return (bLong ? IDS_DISPLAY_PERCENT : IDS_DISPLAY_PERCENT_SHORT);
	case IUI_PRIORITY:		return (bLong ? IDS_DISPLAY_PRIORITY : IDS_DISPLAY_PRIORITY_SHORT);
	case IUI_RECURRENCE:	return (bLong ? IDS_DISPLAY_RECURRENCE : IDS_DISPLAY_RECURRENCE_SHORT);
	case IUI_RISK:			return (bLong ? IDS_DISPLAY_RISK : IDS_DISPLAY_RISK_SHORT);
	case IUI_STARTDATE:		return (bLong ? IDS_DISPLAY_STARTDATE : IDS_DISPLAY_STARTDATE_SHORT);
	case IUI_STATUS:		return (bLong ? IDS_DISPLAY_STATUS : IDS_DISPLAY_STATUS_SHORT);
	case IUI_TAGS:			return (bLong ? IDS_DISPLAY_TAGS : IDS_DISPLAY_TAGS_SHORT);
	case IUI_TIMEEST:		return (bLong ? IDS_DISPLAY_TIMEEST : IDS_DISPLAY_TIMEEST_SHORT);
	case IUI_TIMESPENT:		return (bLong ? IDS_DISPLAY_TIMESPENT : IDS_DISPLAY_TIMESPENT_SHORT);
	case IUI_VERSION:		return (bLong ? IDS_DISPLAY_VERSION : IDS_DISPLAY_VERSION_SHORT);
	}

	ASSERT(0);
	return 0;
}

void CKanbanListCtrlEx::DrawAttribute(CDC* pDC, CRect& rLine, IUI_ATTRIBUTE nAttrib, const CString& sValue, int nFlags) const
{
	KBC_ATTRIBLABELS nLabelVis = m_nAttribLabelVisiability;
	
	if (m_bSavingToImage)
		nLabelVis = KBCAL_LONG;

	CString sAttrib = FormatAttribute(nAttrib, sValue, nLabelVis);
	pDC->DrawText(sAttrib, rLine, nFlags);

	rLine.top += m_nLineHeight;
}

CString CKanbanListCtrlEx::FormatAttribute(IUI_ATTRIBUTE nAttrib, const CString& sValue, KBC_ATTRIBLABELS nLabelVis)
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

/*
void CKanbanListCtrlEx::OnHeaderCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMCUSTOMDRAW* pNMCD = (NMCUSTOMDRAW*)pNMHDR;
	*pResult = CDRF_DODEFAULT;

	HWND hwndHdr = pNMCD->hdr.hwndFrom;

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
*/

BOOL CKanbanListCtrlEx::GetLabelEditRect(LPRECT pEdit)
{
	if (!m_bSelected || !GetCount() || !GetSelectedCount())
	{
		ASSERT(0);
		return FALSE;
	}

	ASSERT(GetSelectedCount() == 1);

	HTREEITEM hti = GetSelectedItem();
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

DWORD CKanbanListCtrlEx::GetSelectedTaskID() const
{
	HTREEITEM hti = GetSelectedItem();

	return (hti ? GetTaskID(hti) : 0);
}

void CKanbanListCtrlEx::ScrollToSelection()
{
	EnsureVisible(GetSelectedItem());
}

void CKanbanListCtrlEx::ClearSelection()
{
	SelectItem(NULL);
}

BOOL CKanbanListCtrlEx::SelectTask(DWORD dwTaskID)
{
	return SelectItem(FindTask(dwTaskID));
}

BOOL CKanbanListCtrlEx::SelectTasks(const CDWordArray& aTaskIDs)
{
	if (aTaskIDs.GetSize() != 1)
		return FALSE;

	return SelectTask(aTaskIDs[0]);
}

const KANBANITEM* CKanbanListCtrlEx::GetKanbanItem(DWORD dwTaskID) const
{
	ASSERT(dwTaskID);
	
	KANBANITEM* pKI = NULL;
	
	if (dwTaskID && m_data.Lookup(dwTaskID, pKI))
	{
		ASSERT(pKI);
	}
	
	return pKI;
}

HTREEITEM CKanbanListCtrlEx::FindTask(DWORD dwTaskID) const
{
	return m_tch.FindItem(dwTaskID);
}

HTREEITEM CKanbanListCtrlEx::FindTask(const CPoint& ptScreen) const
{
	CPoint ptClient(ptScreen);
	ScreenToClient(&ptClient);
	
	return CTreeCtrl::HitTest(ptClient);
}

HTREEITEM CKanbanListCtrlEx::FindTask(const IUISELECTTASK& select, BOOL bNext, HTREEITEM htiStart) const
{
	HTREEITEM htiNext = NULL;

	if (htiStart == NULL)
		htiNext = (bNext ? m_tch.GetFirstItem() : m_tch.GetLastItem());
	else
		htiNext = GetNextItem(htiStart, (bNext ? TVGN_NEXT : TVGN_PREVIOUS));

	while (htiNext)
	{
		DWORD dwTaskID = GetTaskID(htiNext);
		const KANBANITEM* pKI = GetKanbanItem(dwTaskID);

		if (pKI->MatchesAttribute(select))
		{
			return htiNext;
		}

		htiNext = GetNextItem(htiNext, (bNext ? TVGN_NEXT : TVGN_PREVIOUS));
	}

	return NULL; // no match
}

BOOL CKanbanListCtrlEx::DeleteTask(DWORD dwTaskID)
{
	HTREEITEM hti = FindTask(dwTaskID);

	if (hti && DeleteItem(hti))
	{
		//RefreshColumnTitle();
		return TRUE;
	}

	return FALSE;
}

int CALLBACK CKanbanListCtrlEx::SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	const KANBANSORT* pSort = (KANBANSORT*)lParamSort;
	
	const KANBANITEM* pKI1 = pSort->data.GetItem(lParam1);
	const KANBANITEM* pKI2 = pSort->data.GetItem(lParam2);

	int nCompare = 0;
	
	if (pKI1 && pKI2)
	{
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
		ASSERT(pKI1->dwParentID == pKI2->dwParentID);
	
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

				CString sValue1 = pKI1->GetAttributeDisplayValue(pSort->nBy);
				CString sValue2 = pKI2->GetAttributeDisplayValue(pSort->nBy);

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
			nCompare = CDateHelper::Compare(pKI1->dtCreate, pKI2->dtCreate, DHC_COMPARETIME);
			break;
			
		case IUI_CREATEDBY:
			nCompare = Misc::NaturalCompare(pKI1->sCreatedBy, pKI2->sCreatedBy);
			break;
			
		case IUI_DONEDATE:
			nCompare = CDateHelper::Compare(pKI1->dtDone, pKI2->dtDone, (DHC_COMPARETIME | DHC_NOTIMEISENDOFDAY));
			break;
			
		case IUI_DUEDATE:
			nCompare = CDateHelper::Compare(pKI1->dtDue, pKI2->dtDue, (DHC_COMPARETIME | DHC_NOTIMEISENDOFDAY));
			break;
			
		case IUI_EXTERNALID:
			nCompare = Misc::NaturalCompare(pKI1->sExternalID, pKI2->sExternalID);
			break;
			
		case IUI_FLAG:
			if (pKI1->bFlag && pKI2->bFlag)
			{
				nCompare = 0;
			}
			else if (pKI1->bFlag)
			{
				nCompare = -1;
			}
			else if (pKI2->bFlag)
			{
				nCompare = 1;
			}
			break;
			
		case IUI_LASTMOD:
			nCompare = CDateHelper::Compare(pKI1->dtLastMod, pKI2->dtLastMod, DHC_COMPARETIME);
			break;
			
		case IUI_PERCENT:
			nCompare = ((pKI1->nPercent > pKI2->nPercent) ? 1 : -1);
			break;
			
		case IUI_RECURRENCE:
			nCompare = Misc::NaturalCompare(pKI1->sRecurrence, pKI2->sRecurrence);
			break;
			
		case IUI_STARTDATE:
			nCompare = CDateHelper::Compare(pKI1->dtStart, pKI2->dtStart, DHC_COMPARETIME);
			break;
			
		case IUI_TIMEEST:
			nCompare = CTimeHelper().Compare(pKI1->dTimeEst, MapUnitsToTHUnits(pKI1->nTimeEstUnits), 
											pKI2->dTimeEst, MapUnitsToTHUnits(pKI2->nTimeEstUnits));
			break;
			
		case IUI_TIMESPENT:
			nCompare = CTimeHelper().Compare(pKI1->dTimeSpent, MapUnitsToTHUnits(pKI1->nTimeSpentUnits), 
											pKI2->dTimeSpent, MapUnitsToTHUnits(pKI2->nTimeSpentUnits));
			break;

		case IUI_NONE: // Synonymous with IUI_POSITION
			ASSERT(pSort->bSubtasksBelowParent);

			// Avoid reversal of sign below
			return ((pKI1->nPosition > pKI2->nPosition) ? 1 : -1);
		}
	}
	
	return (pSort->bAscending ? nCompare : -nCompare);
}

void CKanbanListCtrlEx::Sort(IUI_ATTRIBUTE nBy, BOOL bAscending, BOOL bSubtasksBelowParent)
{
	KANBANSORT ks(m_data);
	
	ks.nBy = nBy;
	ks.bAscending = bAscending;
	ks.bSubtasksBelowParent = bSubtasksBelowParent;

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
		ks.sAttribID = KANBANITEM::GetAttributeID(nBy);
		break;
	}

	TVSORTCB tvs = { NULL, SortProc, (DWORD)&ks };

	SortChildrenCB(&tvs);
}

void CKanbanListCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (HandleLButtonClick(point))
		return;

	// else
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CKanbanListCtrlEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (HandleLButtonClick(point))
		return;

	// else
	CTreeCtrl::OnLButtonDblClk(nFlags, point);
}

BOOL CKanbanListCtrlEx::HandleLButtonClick(CPoint point)
{
	m_tooltip.Pop();

	if (!m_bSelected)
		GetParent()->SendMessage(WM_KLCN_WANTFOCUS, (WPARAM)GetSafeHwnd());

	// don't let the selection to be set to -1
	// when clicking below the last item
	HTREEITEM hti = HitTest(point);

	if (hti)
	{
		ClientToScreen(&point);

		// we don't want to disable drag selecting
		if (!::DragDetect(GetSafeHwnd(), point))
		{
			// Still set focus
			SetFocus();

			TRACE(_T("Ate Treeview ButtonDown\n"));
			return TRUE; // eat it
		}
	}
	else
	{
		// Test for checkbox hit
		CRect rCheckbox;

		if (GetItemCheckboxRect(hti, rCheckbox, NULL) && rCheckbox.PtInRect(point))
		{
			BOOL bWantFocus = (GetFocus() != this);

			ClearSelection();
			SelectItem(hti);

			if (bWantFocus)
				SetFocus();

			// Post message to let mouse-click time to process
			GetParent()->PostMessage(WM_KLCN_CHECKCHANGE, (WPARAM)GetSafeHwnd(), GetSelectedTaskID());
			return TRUE;
		}
	}
	
	// all else
	return FALSE;
}

BOOL CKanbanListCtrlEx::SaveToImage(CBitmap& bmImage, int nColWidth)
{
/*
	CLockUpdates lock(*this);
	CAutoFlag af(m_bSavingToImage, TRUE);

	// Resize column width to suit text
	int nPrevColWidth = m_header.GetItemWidth(0);

	SetColumnWidth(0, nColWidth);

	// Do the copy
	BOOL bRes = CCopyListCtrlContents(*this).DoCopy(bmImage);

	// Restore column width
	SetColumnWidth(0, nPrevColWidth);

	return bRes;
*/
	return FALSE;
}

int CKanbanListCtrlEx::CalcRequiredColumnWidthForImage() const
{
/*
	CClientDC dc(const_cast<CKanbanListCtrlEx*>(this));
	float fAveCharWidth = GraphicsMisc::GetAverageCharWidth(&dc);

	int nMinHeaderWidth = ((int)(m_header.GetItemText(0).GetLength() * fAveCharWidth) + (2 * LV_PADDING));
	
	HTREEITEM hti = GetCount();
	int nMaxItemWidth = 0;
	
	while (nItem--)
	{
		DWORD dwTaskID = GetTaskID(nItem);
		const KANBANITEM* pKI = GetKanbanItem(dwTaskID);
		ASSERT(pKI);

		if (pKI)
		{
			int nMaxItemWidth = (int)(pKI->sTitle.GetLength() * fAveCharWidth / 2); // title is on two lines
			nMaxItemWidth += (pKI->nLevel * LEVEL_INDENT);
			nMaxItemWidth += (pKI->bHasIcon ? (IMAGE_SIZE + ICON_OFFSET) : 0);
			
			for (int nDisp = 0; nDisp < m_aDisplayAttrib.GetSize(); nDisp++)
			{
				IUI_ATTRIBUTE nAttrib = m_aDisplayAttrib[nDisp];
				CString sAttrib = FormatAttribute(nAttrib, pKI->GetAttributeDisplayValue(nAttrib), KBCAL_LONG);

				nMaxItemWidth = max(nMaxItemWidth, ((int)(sAttrib.GetLength() * fAveCharWidth) + ATTRIB_INDENT));
			}
		}
	}
	
	return max(nMinHeaderWidth, nMaxItemWidth);
*/
	return 0;
}

BOOL CKanbanListCtrlEx::SelectionHasLockedTasks() const
{
	HTREEITEM hti = GetSelectedItem();

	if (hti)
	{
		DWORD dwTaskID = GetTaskID(hti);

		if (m_data.IsLocked(dwTaskID))
			return TRUE;
	}
/*
	POSITION pos = GetFirstSelectedItemPosition();

	while (pos)
	{
		HTREEITEM hti = GetNextSelectedItem(pos);
		DWORD dwTaskID = GetTaskID(nItem);

		if (m_data.IsLocked(dwTaskID))
			return TRUE;
	}
*/

	return FALSE;
}

LRESULT CKanbanListCtrlEx::OnSetFont(WPARAM wp, LPARAM lp)
{
	LRESULT lr = Default();

	OnDisplayAttributeChanged();

	return lr;
}

BOOL CKanbanListCtrlEx::InitTooltip()
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

LRESULT CKanbanListCtrlEx::OnToolHitTest(WPARAM wp, LPARAM lp)
{
	CPoint point(wp);
	TOOLINFO* pTI = (TOOLINFO*)lp;
	
	HTREEITEM hti = HitTest(point);

	if (hti)
	{
		CRect rTip;

		if (GetItemTooltipRect(hti, rTip, NULL))
		{
			pTI->hwnd = GetSafeHwnd();
			pTI->uId = GetTaskID(hti);
			pTI->uFlags |= TTF_TRANSPARENT;
			pTI->lpszText = _tcsdup(GetItemText(hti)); // MFC will free the duplicated string

			GetItemRect(hti, rTip, NULL);
			pTI->rect = rTip;
			
			return pTI->uId;
		}
	}

	// else
	return -1;
}

void CKanbanListCtrlEx::OnTooltipShow(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	DWORD dwTaskID = m_tooltip.GetLastHitToolInfo().uId;
	ASSERT(dwTaskID);

	HTREEITEM hti = FindTask(dwTaskID);
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

BOOL CKanbanListCtrlEx::AttributeValuesMatch(const CKanbanListCtrlEx& other) const
{
	CStringArray aOtherValues;
	other.GetAttributeValues(aOtherValues);

	return Misc::MatchAll(m_columnDef.aAttribValues, aOtherValues);
}

BOOL CKanbanListCtrlEx::CanDrag(const CKanbanListCtrlEx* pSrcList, const CKanbanListCtrlEx* pDestList)
{
	if (!pDestList)
		return FALSE;
	
	if (pDestList == pSrcList)
		return FALSE;
		
	if (pSrcList->AttributeValuesMatch(*pDestList))
		return FALSE;

	// Can't copy FROM the backlog
	if (pSrcList->IsBacklog() && Misc::ModKeysArePressed(MKS_CTRL))
		return FALSE;

	return TRUE;
}

