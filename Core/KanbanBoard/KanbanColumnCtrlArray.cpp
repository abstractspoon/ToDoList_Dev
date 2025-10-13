// KanbanListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KanbanColumnCtrl.h"
#include "KanbanColumnCtrlArray.h"

#include "..\shared\graphicsMisc.h"
#include "..\shared\enbitmap.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\DeferWndMove.h"

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
			const CKanbanColumnCtrl* pCol = GetAt(nCol);
			ASSERT(pCol);

			hti = pCol->FindItem(dwTaskID);
			
			if (hti)
				return nCol;
		}
	}

	// not found
	hti = NULL;
	return -1;
}

int CKanbanColumnCtrlArray::Find(const CDWordArray& aTaskIDs) const
{
	if (aTaskIDs.GetSize())
	{
		int nNumCol = GetSize();

		for (int nCol = 0; nCol < nNumCol; nCol++)
		{
			const CKanbanColumnCtrl* pCol = GetAt(nCol);
			ASSERT(pCol);

			if (pCol->HasTasks(aTaskIDs))
				return nCol;
		}
	}

	// not found
	return -1;
}

int CKanbanColumnCtrlArray::Find(const CString& sAttribValue) const
{
	CString sAttribValueID(Misc::ToUpper(sAttribValue));
	int nNumCol = GetSize();

	for (int nCol = 0; nCol < nNumCol; nCol++)
	{
		const CKanbanColumnCtrl* pCol = GetAt(nCol);
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
		const CKanbanColumnCtrl* pCol = GetAt(nCol);
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

// ---------------------------------------------------

#define ARRAY_FN(fn)						\
int nCol = GetSize();						\
while (nCol--)								\
{											\
	CKanbanColumnCtrl* pCol = GetAt(nCol);	\
	ASSERT(pCol);							\
	fn;										\
}

// ---------------------------------------------------

void CKanbanColumnCtrlArray::OnDisplayAttributeChanged()
{
	ARRAY_FN(pCol->OnDisplayAttributeChanged());
}

void CKanbanColumnCtrlArray::RefreshItemLineHeights()
{
	ARRAY_FN(pCol->RefreshItemLineHeights());
}

void CKanbanColumnCtrlArray::SetRedraw(BOOL bRedraw)
{
	ARRAY_FN(pCol->SetRedraw(bRedraw));
}

void CKanbanColumnCtrlArray::SetOptions(DWORD dwOptions)
{
	ARRAY_FN(pCol->SetOptions(dwOptions));
}

void CKanbanColumnCtrlArray::SetReadOnly(BOOL bReadOnly)
{
	ARRAY_FN(pCol->SetReadOnly(bReadOnly));
}

void CKanbanColumnCtrlArray::SetAttributeLabelVisibility(KBC_ATTRIBLABELS nLabelVis)
{
	ARRAY_FN(pCol->SetAttributeLabelVisibility(nLabelVis));
}

void CKanbanColumnCtrlArray::SetFullColumnColor(COLORREF crFull)
{
	ARRAY_FN(pCol->SetFullColor(crFull));
}

void CKanbanColumnCtrlArray::Exclude(CDC* pDC)
{
	ARRAY_FN(CDialogHelper::ExcludeChild(pCol, pDC));
}

void CKanbanColumnCtrlArray::Sort(TDC_ATTRIBUTE nBy, BOOL bAscending)
{
	ARRAY_FN(pCol->Sort(nBy, bAscending));
}

BOOL CKanbanColumnCtrlArray::GroupBy(TDC_ATTRIBUTE nAttribID)
{
	BOOL bSuccess = TRUE;
	ARRAY_FN(bSuccess &= pCol->GroupBy(nAttribID));

	return bSuccess;
}

void CKanbanColumnCtrlArray::SetGroupHeaderBackgroundColor(COLORREF color)
{
	ARRAY_FN(pCol->SetGroupHeaderBackgroundColor(color));
}

void CKanbanColumnCtrlArray::SetDropTarget(const CKanbanColumnCtrl* pTarget)
{
	ARRAY_FN(pCol->SetDropTarget(pCol == pTarget));
}

void CKanbanColumnCtrlArray::SetFont(HFONT hFont)
{
	ARRAY_FN(pCol->SendMessage(WM_SETFONT, (WPARAM)hFont))
}

void CKanbanColumnCtrlArray::Redraw(BOOL bErase, BOOL bUpdate)
{
	ARRAY_FN
	(
		pCol->Invalidate(bErase); 
	
		if (bUpdate)	
			pCol->UpdateWindow()
	);
}

void CKanbanColumnCtrlArray::Offset(CDeferWndMove& dwm, int nAmount)
{
	ARRAY_FN(dwm.OffsetChild(pCol, nAmount, 0));
}

int CKanbanColumnCtrlArray::RemoveDeletedTasks(const CDWordSet& mapCurIDs)
{
	// Go thru each list removing deleted items
	int nNumDeleted = 0;
	ARRAY_FN
	(
		nNumDeleted += pCol->RemoveDeletedTasks(mapCurIDs)
	);

	return nNumDeleted;
}

int CKanbanColumnCtrlArray::GetVisibleTaskCount() const
{
	int nNumVis = 0;
	ARRAY_FN
	(
		nNumVis += pCol->GetCount()
	);

	return nNumVis;
}

void CKanbanColumnCtrlArray::SetSelectedColumn(const CKanbanColumnCtrl* pSelCol)
{
	ARRAY_FN
	(
		if (pCol != pSelCol) 
			pCol->ClearSelection(); 
	
		pCol->SetSelected(pCol == pSelCol)
	);
}

BOOL CKanbanColumnCtrlArray::DeleteTaskFromOthers(DWORD dwTaskID, const CKanbanColumnCtrl* pIgnore)
{
	BOOL bSomeDeleted = FALSE;
	ARRAY_FN
	(
		if (pCol != pIgnore) 
			bSomeDeleted |= pCol->RemoveTask(dwTaskID)
	);

	return bSomeDeleted;
}

CSize CKanbanColumnCtrlArray::CalcRequiredColumnSizeForImage() const
{
	CSize reqSize(0, 0);
	ARRAY_FN
	(
		CSize listSize = pCol->CalcRequiredSizeForImage();

		reqSize.cx = max(reqSize.cx, listSize.cx);
		reqSize.cy = max(reqSize.cy, listSize.cy)
	);

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

void CKanbanColumnCtrlArray::Sort()
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

float CKanbanColumnCtrlArray::GetAverageCharWidth()
{
	ASSERT(GetSize());

	if (!GetSize())
		return 1.0f;

	// else
	CWnd* pRef = GetAt(0);
	CClientDC dc(pRef);

	HFONT hOldFont = GraphicsMisc::PrepareDCFont(&dc, *pRef);
	float fAveCharWidth = GraphicsMisc::GetAverageCharWidth(&dc);

	dc.SelectObject(hOldFont);

	return fAveCharWidth;
}

CKanbanColumnCtrl* CKanbanColumnCtrlArray::GetNext(const CKanbanColumnCtrl* pCol, BOOL bNext, 
													BOOL bExcludeEmpty, BOOL bFixedColumns, BOOL bWrap) const
{
	int nStart = -1;

	if (pCol)
	{
		nStart = Misc::FindT(pCol, *this);

		if (nStart == -1)
		{
			ASSERT(0);
			return NULL;
		}
	}

	int nNext = (pCol ? Misc::NextIndexT(*this, nStart, bNext, bWrap) : (bNext ? 0 : (GetSize() - 1)));

	while (nNext != -1)
	{
		CKanbanColumnCtrl* pNext = GetAt(nNext);
		ASSERT(pNext);

		if (bExcludeEmpty && !pNext->GetCount())
		{
			// skip
		}
		else if (bFixedColumns && !pNext->IsWindowVisible())
		{
			// skip
		}
		else
		{
			return pNext;
		}

		nNext = Misc::NextIndexT(*this, nNext, bNext, bWrap);
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

		if (pCol->IsWindowEnabled() && pCol->IsWindowVisible())
		{
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
	}

	return NULL;
}

DWORD CKanbanColumnCtrlArray::HitTestTask(const CPoint& ptScreen) const
{
	CKanbanColumnCtrl* pUnused = NULL;

	return HitTestTask(ptScreen, pUnused);
}

DWORD CKanbanColumnCtrlArray::HitTestTask(const CPoint& ptScreen, CKanbanColumnCtrl*& pCol) const
{
	pCol = HitTest(ptScreen);

	if (pCol)
	{
		CPoint ptClient(ptScreen);
		pCol->ScreenToClient(&ptClient);

		HTREEITEM hti = pCol->HitTest(ptClient);

		if (hti)
			return pCol->GetTaskID(hti);
	}

	// else
	return 0;
}

void CKanbanColumnCtrlArray::FilterToolTipMessage(MSG* pMsg)
{
	CKanbanColumnCtrl* pCol = HitTest(pMsg->pt);

	if (pCol)
		pCol->FilterToolTipMessage(pMsg);
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

