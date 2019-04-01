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

CKanbanColumnCtrlArray::~CKanbanColumnCtrlArray()
{
	RemoveAll();
}

void CKanbanColumnCtrlArray::RemoveAll()
{
	int nList = GetSize();
	
	while (nList--)
		RemoveAt(nList);
}

BOOL CKanbanColumnCtrlArray::RemoveAt(int nList)
{
	if ((nList < 0) || (nList >= GetSize()))
	{
		ASSERT(0);
		return FALSE;
	}

	CKanbanColumnCtrl* pList = GetAt(nList);

	// remove it first before deleting
	CArray<CKanbanColumnCtrl*, CKanbanColumnCtrl*&>::RemoveAt(nList);

	if (pList->GetSafeHwnd())
		pList->DestroyWindow();

	delete pList;

	return TRUE;
}

int CKanbanColumnCtrlArray::Find(DWORD dwTaskID) const
{
	HTREEITEM htiUnused;
	return Find(dwTaskID, htiUnused);
}

int CKanbanColumnCtrlArray::Find(DWORD dwTaskID, HTREEITEM& hti) const
{
	if (dwTaskID)
	{
		int nNumList = GetSize();

		for (int nList = 0; nList < nNumList; nList++)
		{
			CKanbanColumnCtrl* pList = GetAt(nList);
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

int CKanbanColumnCtrlArray::Find(const CString& sAttribValue) const
{
	CString sAttribValueID(Misc::ToUpper(sAttribValue));
	int nNumList = GetSize();

	for (int nList = 0; nList < nNumList; nList++)
	{
		CKanbanColumnCtrl* pList = GetAt(nList);
		ASSERT(pList);

		if (pList->GetAttributeValueID() == sAttribValueID)
			return nList;
	}

	// not found
	return -1;
}

int CKanbanColumnCtrlArray::Find(HWND hwnd) const
{
	ASSERT(hwnd);

	int nNumList = GetSize();

	for (int nList = 0; nList < nNumList; nList++)
	{
		CKanbanColumnCtrl* pList = GetAt(nList);
		ASSERT(pList);

		if (pList->GetSafeHwnd() == hwnd)
			return nList;
	}

	// not found
	return -1;
}

int CKanbanColumnCtrlArray::Find(const CKanbanColumnCtrl* pList) const
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

CKanbanColumnCtrl* CKanbanColumnCtrlArray::Get(DWORD dwTaskID) const
{
	HTREEITEM htiUnused;
	return Get(dwTaskID, htiUnused);
}

CKanbanColumnCtrl* CKanbanColumnCtrlArray::Get(DWORD dwTaskID, HTREEITEM& hti) const
{
	int nFind = Find(dwTaskID, hti);

	if (nFind != -1)
		return GetAt(nFind);

	// not found
	hti = NULL;
	return NULL;
}

CKanbanColumnCtrl* CKanbanColumnCtrlArray::Get(const CString& sAttribValue) const
{
	int nFind = Find(sAttribValue);

	if (nFind != -1)
		return GetAt(nFind);

	// not found
	return NULL;
}

CKanbanColumnCtrl* CKanbanColumnCtrlArray::Get(HWND hwnd) const
{
	int nFind = Find(hwnd);

	if (nFind != -1)
		return GetAt(nFind);

	// not found
	return NULL;
}

CKanbanColumnCtrl* CKanbanColumnCtrlArray::GetFirstNonEmpty() const
{
	int nNumList = GetSize();

	for (int nList = 0; nList < nNumList; nList++)
	{
		CKanbanColumnCtrl* pList = GetAt(nList);
		ASSERT(pList);

		if (pList->GetCount())
			return pList;
	}

	// all empty
	return NULL;
}

CKanbanColumnCtrl* CKanbanColumnCtrlArray::GetLastNonEmpty() const
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanColumnCtrl* pList = GetAt(nList);
		ASSERT(pList);

		if (pList->GetCount())
			return pList;
	}

	// all empty
	return NULL;
}

CKanbanColumnCtrl* CKanbanColumnCtrlArray::GetBacklog() const
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanColumnCtrl* pList = GetAt(nList);
		ASSERT(pList);

		if (pList->IsBacklog())
			return pList;
	}

	// no backlog
	return NULL;
}

void CKanbanColumnCtrlArray::OnDisplayAttributeChanged()
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanColumnCtrl* pList = GetAt(nList);
		ASSERT(pList);

		pList->OnDisplayAttributeChanged();
	}
}

void CKanbanColumnCtrlArray::RefreshItemLineHeights()
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanColumnCtrl* pList = GetAt(nList);
		ASSERT(pList);

		pList->RefreshItemLineHeights();
	}
}

void CKanbanColumnCtrlArray::SetOptions(DWORD dwOptions)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanColumnCtrl* pList = GetAt(nList);
		ASSERT(pList);

		pList->SetOptions(dwOptions);
	}
}

void CKanbanColumnCtrlArray::SetAttributeLabelVisibility(KBC_ATTRIBLABELS nLabelVis)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanColumnCtrl* pList = GetAt(nList);
		ASSERT(pList);

		pList->SetAttributeLabelVisibility(nLabelVis);
	}
}

void CKanbanColumnCtrlArray::Exclude(CDC* pDC)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanColumnCtrl* pList = GetAt(nList);
		ASSERT(pList);

		CDialogHelper::ExcludeChild(pList, pDC);
	}
}

void CKanbanColumnCtrlArray::SortItems(IUI_ATTRIBUTE nBy, BOOL bAscending)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanColumnCtrl* pList = GetAt(nList);
		ASSERT(pList);

		pList->Sort(nBy, bAscending);
	}
}

void CKanbanColumnCtrlArray::Sort()
{
	if (GetSize() > 1)
		qsort(GetData(), GetSize(), sizeof(CKanbanColumnCtrl**), ListSortProc);
}

int CKanbanColumnCtrlArray::ListSortProc(const void* pV1, const void* pV2)
{
	typedef CKanbanColumnCtrl* LPCKanbanListCtrlEx;

	const CKanbanColumnCtrl* pKLC1 = *(static_cast<const LPCKanbanListCtrlEx*>(pV1));
	const CKanbanColumnCtrl* pKLC2 = *(static_cast<const LPCKanbanListCtrlEx*>(pV2));

	// backlog always comes first
	if (!pKLC1->HasAnyValues())
		return -1;

	if (!pKLC2->HasAnyValues())
		return 1;

	// Sort by ID
	return Misc::NaturalCompare(pKLC1->GetAttributeValue(), pKLC2->GetAttributeValue());
}

int CKanbanColumnCtrlArray::GetVisibleTaskCount() const
{
	int nList = GetSize(), nNumVis = 0;

	while (nList--)
	{
		const CKanbanColumnCtrl* pList = GetAt(nList);
		ASSERT(pList);

		nNumVis += pList->GetCount();
	}

	return nNumVis;
}

float CKanbanColumnCtrlArray::GetAverageCharWidth()
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

CKanbanColumnCtrl* CKanbanColumnCtrlArray::GetNext(const CKanbanColumnCtrl* pList, BOOL bNext, 
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
			CKanbanColumnCtrl* pNext = GetAt(nNext);
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
			CKanbanColumnCtrl* pPrev = GetAt(nPrev);
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

CKanbanColumnCtrl* CKanbanColumnCtrlArray::HitTest(const CPoint& ptScreen, HTREEITEM* pHit, UINT* pHitFlags) const
{
	int nList = GetSize();
	CRect rWindow;

	while (nList--)
	{
		CKanbanColumnCtrl* pList = GetAt(nList);
		pList->GetWindowRect(rWindow);

		rWindow.right++; // to allow for the 1 pixel gap

		if (rWindow.PtInRect(ptScreen))
		{
			if (pHit)
			{
				CPoint ptClient(ptScreen);
				pList->ScreenToClient(&ptClient);

				*pHit = pList->HitTest(ptClient, pHitFlags);
			}

			return pList;
		}
	}

	return NULL;
}


DWORD CKanbanColumnCtrlArray::HitTestTask(const CPoint& ptScreen) const
{
	const CKanbanColumnCtrl* pList = HitTest(ptScreen);

	if (pList)
	{
		HTREEITEM hti = pList->FindTask(ptScreen);

		if (hti)
			return pList->GetTaskID(hti);
	}

	// else
	return 0;
}

void CKanbanColumnCtrlArray::SetSelectedList(const CKanbanColumnCtrl* pSelList)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanColumnCtrl* pList = GetAt(nList);
		ASSERT(pList);

		if (pList != pSelList)
			pList->ClearSelection();

		pList->SetSelected(pList == pSelList);
	}
}

void CKanbanColumnCtrlArray::SetDropTarget(const CKanbanColumnCtrl* pTarget)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanColumnCtrl* pList = GetAt(nList);
		ASSERT(pList);

		pList->SetDropTarget(pList == pTarget);
	}
}

void CKanbanColumnCtrlArray::DeleteTaskFromOthers(DWORD dwTaskID, const CKanbanColumnCtrl* pIgnore)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanColumnCtrl* pList = GetAt(nList);
		ASSERT(pList);

		if (pList != pIgnore)
			pList->DeleteTask(dwTaskID);
	}
}

CSize CKanbanColumnCtrlArray::CalcRequiredColumnSizeForImage() const
{
	CSize reqSize(0, 0);
	int nList = GetSize();

	while (nList--)
	{
		const CKanbanColumnCtrl* pList = GetAt(nList);
		ASSERT(pList);

		CSize listSize = pList->CalcRequiredSizeForImage();

		reqSize.cx = max(reqSize.cx, listSize.cx);
		reqSize.cy = max(reqSize.cy, listSize.cy);
	}

	return reqSize;
}

BOOL CKanbanColumnCtrlArray::CanSaveToImage() const
{
	// At least one column must have items
	// And the item count per page must be 1 or more
	int nList = GetSize();

	while (nList--)
	{
		const CKanbanColumnCtrl* pList = GetAt(nList);
		ASSERT(pList);

		if (pList->GetCount() && pList->GetVisibleCount())
			return TRUE;
	}

	return FALSE;
}

BOOL CKanbanColumnCtrlArray::SaveToImage(CBitmap& bmImage)
{
	if (!CanSaveToImage())
		return FALSE;

	int nNumLists = GetSize();

	CArray<CBitmap, CBitmap&> aListBmps;
	aListBmps.SetSize(nNumLists, 1);

	int nListWidth = 0, nListHeight = 0;
	CSize reqColSize = CalcRequiredColumnSizeForImage();

	for (int nList = 0; nList < nNumLists; nList++)
	{
		CKanbanColumnCtrl* pList = GetAt(nList);
		ASSERT(pList);

		CEnBitmap bmp;

		if (!pList->SaveToImage(bmp, reqColSize))
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

void CKanbanColumnCtrlArray::SetFont(HFONT hFont)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanColumnCtrl* pList = GetAt(nList);
		ASSERT(pList);

		pList->SendMessage(WM_SETFONT, (WPARAM)hFont);
	}
}

void CKanbanColumnCtrlArray::Redraw(BOOL bErase, BOOL bUpdate)
{
	int nList = GetSize();

	while (nList--)
	{
		CKanbanColumnCtrl* pList = GetAt(nList);
		ASSERT(pList);

		if (pList)
		{
			pList->Invalidate(bErase);

			if (bUpdate)
				pList->UpdateWindow();
		}
	}
}

void CKanbanColumnCtrlArray::RemoveDeletedTasks(const CDWordSet& mapCurIDs)
{
	// Go thru each list removing deleted items
	int nList = GetSize();

	while (nList--)
	{
		CKanbanColumnCtrl* pList = GetAt(nList);
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

