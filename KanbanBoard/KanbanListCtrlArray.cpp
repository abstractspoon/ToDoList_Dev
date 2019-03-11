// KanbanListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KanbanListCtrl.h"
#include "KanbanListCtrlArray.h"

#include "..\shared\graphicsMisc.h"
// #include "..\shared\enstring.h"
#include "..\shared\enbitmap.h"
// #include "..\shared\misc.h"
#include "..\shared\dialoghelper.h"
// #include "..\shared\datehelper.h"
// #include "..\shared\timehelper.h"
// #include "..\shared\autoflag.h"
// #include "..\shared\copywndcontents.h"
// #include "..\shared\holdredraw.h"
// #include "..\shared\winclasses.h"
// #include "..\shared\wclassdefines.h"
// #include "..\Shared\enimagelist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

int CKanbanListCtrlArray::Find(DWORD dwTaskID) const
{
	HTREEITEM htiUnused;
	return Find(dwTaskID, htiUnused);
}

int CKanbanListCtrlArray::Find(DWORD dwTaskID, HTREEITEM& hti) const
{
	if (dwTaskID)
	{
		int nNumList = GetSize();

		for (int nList = 0; nList < nNumList; nList++)
		{
			CKanbanListCtrl* pList = GetAt(nList);
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

int CKanbanListCtrlArray::Find(const CString& sAttribValue) const
{
	CString sAttribValueID(Misc::ToUpper(sAttribValue));
	int nNumList = GetSize();

	for (int nList = 0; nList < nNumList; nList++)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		if (pList->GetAttributeValueID() == sAttribValueID)
			return nList;
	}

	// not found
	return -1;
}

int CKanbanListCtrlArray::Find(HWND hwnd) const
{
	ASSERT(hwnd);

	int nNumList = GetSize();

	for (int nList = 0; nList < nNumList; nList++)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		if (pList->GetSafeHwnd() == hwnd)
			return nList;
	}

	// not found
	return -1;
}

int CKanbanListCtrlArray::Find(const CKanbanListCtrl* pList) const
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

CKanbanListCtrl* CKanbanListCtrlArray::Get(DWORD dwTaskID) const
{
	HTREEITEM htiUnused;
	return Get(dwTaskID, htiUnused);
}

CKanbanListCtrl* CKanbanListCtrlArray::Get(DWORD dwTaskID, HTREEITEM& hti) const
{
	int nFind = Find(dwTaskID, hti);

	if (nFind != -1)
		return GetAt(nFind);

	// not found
	hti = NULL;
	return NULL;
}

CKanbanListCtrl* CKanbanListCtrlArray::Get(const CString& sAttribValue) const
{
	int nFind = Find(sAttribValue);

	if (nFind != -1)
		return GetAt(nFind);

	// not found
	return NULL;
}

CKanbanListCtrl* CKanbanListCtrlArray::Get(HWND hwnd) const
{
	int nFind = Find(hwnd);

	if (nFind != -1)
		return GetAt(nFind);

	// not found
	return NULL;
}

CKanbanListCtrl* CKanbanListCtrlArray::GetFirstNonEmpty() const
{
	int nNumList = GetSize();

	for (int nList = 0; nList < nNumList; nList++)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		if (pList->GetCount())
			return pList;
	}

	// all empty
	return NULL;
}

CKanbanListCtrl* CKanbanListCtrlArray::GetLastNonEmpty() const
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		if (pList->GetCount())
			return pList;
	}

	// all empty
	return NULL;
}

CKanbanListCtrl* CKanbanListCtrlArray::GetBacklog() const
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		if (pList->IsBacklog())
			return pList;
	}

	// no backlog
	return NULL;
}

void CKanbanListCtrlArray::OnDisplayAttributeChanged()
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		pList->OnDisplayAttributeChanged();
	}
}

void CKanbanListCtrlArray::RefreshItemLineHeights()
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		pList->RefreshItemLineHeights();
	}
}

void CKanbanListCtrlArray::SetTextColorIsBackground(BOOL bSet)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		pList->SetTextColorIsBackground(bSet);
	}
}

void CKanbanListCtrlArray::SetShowTaskColorAsBar(BOOL bSet)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		pList->SetShowTaskColorAsBar(bSet);
	}
}

void CKanbanListCtrlArray::SetStrikeThruDoneTasks(BOOL bSet)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		pList->SetStrikeThruDoneTasks(bSet);
	}
}

void CKanbanListCtrlArray::SetColorTaskBarByPriority(BOOL bSet)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		pList->SetColorTaskBarByPriority(bSet);
	}
}

void CKanbanListCtrlArray::SetAttributeLabelVisibility(KBC_ATTRIBLABELS nLabelVis)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		pList->SetAttributeLabelVisibility(nLabelVis);
	}
}

void CKanbanListCtrlArray::SetShowCompletionCheckboxes(BOOL bShow)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		pList->SetShowCompletionCheckboxes(bShow);
	}
}

void CKanbanListCtrlArray::SetIndentSubtasks(BOOL bIndent)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		pList->SetIndentSubtasks(bIndent);
	}
}

void CKanbanListCtrlArray::SetHideEmptyAttributes(BOOL bHide)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		pList->SetHideEmptyAttributes(bHide);
	}
}

void CKanbanListCtrlArray::Exclude(CDC* pDC)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		CDialogHelper::ExcludeChild(pList, pDC);
	}
}

void CKanbanListCtrlArray::SortItems(IUI_ATTRIBUTE nBy, BOOL bAscending, BOOL bSubtasksBelowParent)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		pList->Sort(nBy, bAscending, bSubtasksBelowParent);
	}
}

void CKanbanListCtrlArray::Sort()
{
	if (GetSize() > 1)
		qsort(GetData(), GetSize(), sizeof(CKanbanListCtrl**), ListSortProc);
}

int CKanbanListCtrlArray::ListSortProc(const void* pV1, const void* pV2)
{
	typedef CKanbanListCtrl* LPCKanbanListCtrlEx;

	const CKanbanListCtrl* pKLC1 = *(static_cast<const LPCKanbanListCtrlEx*>(pV1));
	const CKanbanListCtrl* pKLC2 = *(static_cast<const LPCKanbanListCtrlEx*>(pV2));

	// backlog always comes first
	if (!pKLC1->HasAnyValues())
		return -1;

	if (!pKLC2->HasAnyValues())
		return 1;

	// Sort by ID
	return Misc::NaturalCompare(pKLC1->GetAttributeValue(), pKLC2->GetAttributeValue());
}

int CKanbanListCtrlArray::GetVisibleCount(BOOL bIncBacklog) const
{
	int nList = GetSize(), nNumVis = 0;

	while (nList--)
	{
		const CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		if (pList->GetCount() || (bIncBacklog && pList->IsBacklog()))
			nNumVis++;
	}

	return nNumVis;
}

int CKanbanListCtrlArray::GetVisibleTaskCount() const
{
	int nList = GetSize(), nNumVis = 0;

	while (nList--)
	{
		const CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		nNumVis += pList->GetCount();
	}

	return nNumVis;
}

float CKanbanListCtrlArray::GetAverageCharWidth()
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

CKanbanListCtrl* CKanbanListCtrlArray::GetNext(const CKanbanListCtrl* pList, BOOL bNext, 
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
			CKanbanListCtrl* pNext = GetAt(nNext);
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
			CKanbanListCtrl* pPrev = GetAt(nPrev);
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

CKanbanListCtrl* CKanbanListCtrlArray::HitTest(const CPoint& ptScreen) const
{
	int nList = GetSize();
	CRect rWindow;

	while (nList--)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		pList->GetWindowRect(rWindow);

		if (rWindow.PtInRect(ptScreen))
			return pList;
	}

	return NULL;

// 	HWND hwnd = ::WindowFromPoint(ptScreen);
// 
// 	if (hwnd == NULL)
// 		return NULL;
// 
// 	int nFind = Find(hwnd);
// 	
// 	if (nFind == -1)
// 		return NULL;
// 	
// 	// else
// 	return GetAt(nFind);
}

DWORD CKanbanListCtrlArray::HitTestTask(const CPoint& ptScreen) const
{
	const CKanbanListCtrl* pList = HitTest(ptScreen);

	if (pList)
	{
		HTREEITEM hti = pList->FindTask(ptScreen);

		if (hti)
			return pList->GetTaskID(hti);
	}

	// else
	return 0;
}

void CKanbanListCtrlArray::ClearOtherSelections(const CKanbanListCtrl* pIgnore)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		if (pList != pIgnore)
			pList->ClearSelection();

		pList->SetSelected(pList == pIgnore);
	}
}

void CKanbanListCtrlArray::DeleteTaskFromOthers(DWORD dwTaskID, const CKanbanListCtrl* pIgnore)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		if (pList != pIgnore)
			pList->DeleteTask(dwTaskID);
	}
}

int CKanbanListCtrlArray::CalcRequiredColumnWidthForImage() const
{
	int nMaxWidth = 0;
	int nList = GetSize();

	while (nList--)
	{
		const CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		int nListWidth = pList->CalcRequiredColumnWidthForImage();
		nMaxWidth = max(nMaxWidth, nListWidth);
	}

	return nMaxWidth;
}

BOOL CKanbanListCtrlArray::CanSaveToImage() const
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

BOOL CKanbanListCtrlArray::SaveToImage(CBitmap& bmImage)
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
		CKanbanListCtrl* pList = GetAt(nList);
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

void CKanbanListCtrlArray::OnSetFont(HFONT hFont)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		pList->SendMessage(WM_SETFONT, (WPARAM)hFont);
	}
}

void CKanbanListCtrlArray::Redraw(BOOL bErase)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = GetAt(nList);
		ASSERT(pList);

		if (pList)
			pList->Invalidate(bErase);
	}
}

void CKanbanListCtrlArray::RemoveDeletedTasks(const CDWordSet& mapCurIDs)
{
	// Go thru each list removing deleted items
	int nList = GetSize();

	while (nList--)
	{
		CKanbanListCtrl* pList = GetAt(nList);
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

