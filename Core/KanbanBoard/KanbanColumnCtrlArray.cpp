// KanbanListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KanbanColumnCtrl.h"
#include "KanbanColumnCtrlArray.h"

#include "..\shared\graphicsMisc.h"
#include "..\shared\enbitmap.h"
#include "..\shared\dialoghelper.h"

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
	int nCol = GetSize();
	
	while (nCol--)
		RemoveAt(nCol);
}

BOOL CKanbanColumnCtrlArray::RemoveAt(int nCol)
{
	if ((nCol < 0) || (nCol >= GetSize()))
	{
		ASSERT(0);
		return FALSE;
	}

	CKanbanColumnCtrl* pCol = GetAt(nCol);

	// remove it first before deleting
	CArray<CKanbanColumnCtrl*, CKanbanColumnCtrl*&>::RemoveAt(nCol);

	if (pCol->GetSafeHwnd())
		pCol->DestroyWindow();

	delete pCol;

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
		int nNumCol = GetSize();

		for (int nCol = 0; nCol < nNumCol; nCol++)
		{
			CKanbanColumnCtrl* pCol = GetAt(nCol);
			ASSERT(pCol);

			hti = pCol->FindTask(dwTaskID);
			
			if (hti)
				return nCol;
		}
	}

	// not found
	hti = NULL;
	return -1;
}

int CKanbanColumnCtrlArray::Find(const CString& sAttribValue) const
{
	CString sAttribValueID(Misc::ToUpper(sAttribValue));
	int nNumCol = GetSize();

	for (int nCol = 0; nCol < nNumCol; nCol++)
	{
		CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		if (pCol->GetAttributeValueID() == sAttribValueID)
			return nCol;
	}

	// not found
	return -1;
}

int CKanbanColumnCtrlArray::Find(HWND hwnd) const
{
	ASSERT(hwnd);

	int nNumCol = GetSize();

	for (int nCol = 0; nCol < nNumCol; nCol++)
	{
		CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		if (pCol->GetSafeHwnd() == hwnd)
			return nCol;
	}

	// not found
	return -1;
}

int CKanbanColumnCtrlArray::Find(const CKanbanColumnCtrl* pCol) const
{
	int nCol = GetSize();

	while (nCol--)
	{
		if (GetAt(nCol) == pCol)
			return nCol;
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
	int nNumCol = GetSize();

	for (int nCol = 0; nCol < nNumCol; nCol++)
	{
		CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		if (pCol->GetCount())
			return pCol;
	}

	// all empty
	return NULL;
}

CKanbanColumnCtrl* CKanbanColumnCtrlArray::GetLastNonEmpty() const
{
	int nCol = GetSize();

	while (nCol--)
	{
		CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		if (pCol->GetCount())
			return pCol;
	}

	// all empty
	return NULL;
}

CKanbanColumnCtrl* CKanbanColumnCtrlArray::GetBacklog() const
{
	int nCol = GetSize();

	while (nCol--)
	{
		CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		if (pCol->IsBacklog())
			return pCol;
	}

	// no backlog
	return NULL;
}

void CKanbanColumnCtrlArray::OnDisplayAttributeChanged()
{
	int nCol = GetSize();

	while (nCol--)
	{
		CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		pCol->OnDisplayAttributeChanged();
	}
}

void CKanbanColumnCtrlArray::RefreshItemLineHeights()
{
	int nCol = GetSize();

	while (nCol--)
	{
		CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		pCol->RefreshItemLineHeights();
	}
}

void CKanbanColumnCtrlArray::SetOptions(DWORD dwOptions)
{
	int nCol = GetSize();

	while (nCol--)
	{
		CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		pCol->SetOptions(dwOptions);
	}
}

void CKanbanColumnCtrlArray::SetAttributeLabelVisibility(KBC_ATTRIBLABELS nLabelVis)
{
	int nCol = GetSize();

	while (nCol--)
	{
		CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		pCol->SetAttributeLabelVisibility(nLabelVis);
	}
}

void CKanbanColumnCtrlArray::Exclude(CDC* pDC)
{
	int nCol = GetSize();

	while (nCol--)
	{
		CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		CDialogHelper::ExcludeChild(pCol, pDC);
	}
}

void CKanbanColumnCtrlArray::SortItems(TDC_ATTRIBUTE nBy, BOOL bAscending)
{
	int nCol = GetSize();

	while (nCol--)
	{
		CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		pCol->Sort(nBy, bAscending);
	}
}

void CKanbanColumnCtrlArray::SortColumns()
{
	if (GetSize() > 1)
		qsort(GetData(), GetSize(), sizeof(CKanbanColumnCtrl**), SortProc);
}

int CKanbanColumnCtrlArray::SortProc(const void* pV1, const void* pV2)
{
	typedef CKanbanColumnCtrl* LPCKanbanColumnCtrl;

	const CKanbanColumnCtrl* pKLC1 = *(static_cast<const LPCKanbanColumnCtrl*>(pV1));
	const CKanbanColumnCtrl* pKLC2 = *(static_cast<const LPCKanbanColumnCtrl*>(pV2));

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
	int nCol = GetSize(), nNumVis = 0;

	while (nCol--)
	{
		const CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		nNumVis += pCol->GetCount();
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

CKanbanColumnCtrl* CKanbanColumnCtrlArray::GetNext(const CKanbanColumnCtrl* pCol, BOOL bNext, 
												BOOL bExcludeEmpty, BOOL bFixedColumns) const
{
	int nCol = -1;

	if (pCol)
	{
		nCol = Misc::FindT(pCol, *this);

		if (nCol == -1)
		{
			ASSERT(0);
			return NULL;
		}
	}
	else if (bNext)
	{
		nCol = -1;
	}
	else
	{
		nCol = GetSize();
	}

	if (bNext)
	{
		for (int nNext = (nCol + 1); nNext < GetSize(); nNext++)
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
		int nPrev(nCol);

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
	int nCol = GetSize();
	CRect rWindow;

	while (nCol--)
	{
		CKanbanColumnCtrl* pCol = GetAt(nCol);
		pCol->GetWindowRect(rWindow);

		rWindow.right++; // to allow for the 1 pixel gap

		if (rWindow.PtInRect(ptScreen))
		{
			if (pHit)
			{
				CPoint ptClient(ptScreen);
				pCol->ScreenToClient(&ptClient);

				*pHit = pCol->HitTest(ptClient, pHitFlags);
			}

			return pCol;
		}
	}

	return NULL;
}


DWORD CKanbanColumnCtrlArray::HitTestTask(const CPoint& ptScreen) const
{
	const CKanbanColumnCtrl* pCol = HitTest(ptScreen);

	if (pCol)
	{
		HTREEITEM hti = pCol->FindTask(ptScreen);

		if (hti)
			return pCol->GetTaskID(hti);
	}

	// else
	return 0;
}

void CKanbanColumnCtrlArray::SetSelectedColumn(const CKanbanColumnCtrl* pSelCol)
{
	int nCol = GetSize();

	while (nCol--)
	{
		CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		if (pCol != pSelCol)
			pCol->ClearSelection();

		pCol->SetSelected(pCol == pSelCol);
	}
}

void CKanbanColumnCtrlArray::SetDropTarget(const CKanbanColumnCtrl* pTarget)
{
	int nCol = GetSize();

	while (nCol--)
	{
		CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		pCol->SetDropTarget(pCol == pTarget);
	}
}

void CKanbanColumnCtrlArray::DeleteTaskFromOthers(DWORD dwTaskID, const CKanbanColumnCtrl* pIgnore)
{
	int nCol = GetSize();

	while (nCol--)
	{
		CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		if (pCol != pIgnore)
			pCol->DeleteTask(dwTaskID);
	}
}

CSize CKanbanColumnCtrlArray::CalcRequiredColumnSizeForImage() const
{
	CSize reqSize(0, 0);
	int nCol = GetSize();

	while (nCol--)
	{
		const CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		CSize listSize = pCol->CalcRequiredSizeForImage();

		reqSize.cx = max(reqSize.cx, listSize.cx);
		reqSize.cy = max(reqSize.cy, listSize.cy);
	}

	return reqSize;
}

BOOL CKanbanColumnCtrlArray::CanSaveToImage() const
{
	// At least one column must have items
	// And the item count per page must be 1 or more
	int nCol = GetSize();

	while (nCol--)
	{
		const CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		if (pCol->GetCount() && pCol->GetVisibleCount())
			return TRUE;
	}

	return FALSE;
}

BOOL CKanbanColumnCtrlArray::SaveToImage(CBitmap& bmImage)
{
	if (!CanSaveToImage())
		return FALSE;

	int nNumCols = GetSize();

	CArray<CBitmap, CBitmap&> aColBmps;
	aColBmps.SetSize(nNumCols, 1);

	int nColWidth = 0, nColHeight = 0;
	CSize reqColSize = CalcRequiredColumnSizeForImage();

	for (int nCol = 0; nCol < nNumCols; nCol++)
	{
		CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		CEnBitmap bmp;

		if (!pCol->SaveToImage(bmp, reqColSize))
			return FALSE;

		CSize size = bmp.GetSize();
		nColHeight = max(nColHeight, size.cy);

		if (nColWidth == 0)
			nColWidth = size.cx;
		else
			ASSERT(nColWidth == size.cx);

		aColBmps[nCol].Attach(bmp.Detach());
	}

	// Create some memory DCs
	CClientDC dc(GetAt(0));
	CDC dcImage, dcCol;

	if (dcImage.CreateCompatibleDC(&dc) && dcCol.CreateCompatibleDC(&dc))
	{
		// Create the image big enough to fit the lists side-by-side
		int nTotalWidth = (nNumCols * nColWidth);

		if (bmImage.CreateCompatibleBitmap(&dc, nTotalWidth, nColHeight))
		{
			CBitmap* pOldImage = dcImage.SelectObject(&bmImage);

			// Fill with background colour
			dcImage.FillSolidRect(0, 0, nTotalWidth, nColHeight, GetSysColor(COLOR_WINDOW));

			for (int nCol = 0, nPos = 0; nCol < nNumCols; nCol++)
			{
				CBitmap* pOldCol = dcCol.SelectObject(&(aColBmps[nCol]));

				dcImage.BitBlt(nPos, 0, nColWidth, nColHeight, &dcCol, 0, 0, SRCCOPY);
				nPos += nColWidth;

				dcImage.FillSolidRect((nPos - 1) , 0, 1, nColHeight, GetSysColor(COLOR_3DFACE));
				dcCol.SelectObject(pOldCol);
			}
		}
	}

	return (bmImage.GetSafeHandle() != NULL);
}

void CKanbanColumnCtrlArray::SetFont(HFONT hFont)
{
	int nCol = GetSize();

	while (nCol--)
	{
		CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		pCol->SendMessage(WM_SETFONT, (WPARAM)hFont);
	}
}

void CKanbanColumnCtrlArray::Redraw(BOOL bErase, BOOL bUpdate)
{
	int nCol = GetSize();

	while (nCol--)
	{
		CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		if (pCol)
		{
			pCol->Invalidate(bErase);

			if (bUpdate)
				pCol->UpdateWindow();
		}
	}
}

int CKanbanColumnCtrlArray::RemoveDeletedTasks(const CDWordSet& mapCurIDs)
{
	// Go thru each list removing deleted items
	int nCol = GetSize(), nNumDeleted = 0;

	while (nCol--)
	{
		CKanbanColumnCtrl* pCol = GetAt(nCol);
		ASSERT(pCol);

		if (pCol)
			nNumDeleted += pCol->RemoveDeletedTasks(mapCurIDs);
	}

	return nNumDeleted;
}

