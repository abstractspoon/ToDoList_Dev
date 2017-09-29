// TaskListDropTarget.cpp: implementation of the CTaskListDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaskListDropTarget.h"
#include "TDLTaskCtrlBase.h"

#include "..\shared\wclassdefines.h"
#include "..\shared\winclasses.h"
#include "..\shared\filemisc.h"
#include "..\shared\webmisc.h"
#include "..\shared\msoutlookhelper.h"
#include "..\shared\fileedit.h"
#include "..\shared\holdredraw.h"
#include "..\shared\clipboard.h"

#include "..\3rdparty\msoutl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

using namespace OutlookAPI;

//////////////////////////////////////////////////////////////////////

static const CPoint NULL_POINT(-1000, -1000);

//////////////////////////////////////////////////////////////////////

TLDT_DATA::TLDT_DATA(COleDataObject* pObj, CPoint pt) 
	: 
	dwTaskID(0), 
	pObject(pObj), 
	pFilePaths(NULL), 
	pOutlookSelection(NULL),
	ptClient(pt) 
{
}

CString TLDT_DATA::GetFirstFile() const 
{ 
	if (pFilePaths && pFilePaths->GetSize())
		return pFilePaths->GetAt(0);
	
	// else
	return _T(""); 
}

int TLDT_DATA::GetFileCount() const 
{ 
	if (pFilePaths)
		return pFilePaths->GetSize();
	
	// else
	return 0; 
}

CString TLDT_DATA::GetFile(int nFile) const 
{ 
	if (pFilePaths && nFile < pFilePaths->GetSize())
		return pFilePaths->GetAt(nFile);
	
	// else
	return _T(""); 
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define IS_WND_TYPE(pWnd, cls, type) (pWnd->IsKindOf(RUNTIME_CLASS(cls)) || CWinClasses::IsClass(*pWnd, type))

CTaskListDropTarget::CTaskListDropTarget() 
	: 
	m_pOwner(NULL), 
	m_dwPrevItem(0), 
	m_dwTVHoverStart(0),
	m_pOutlook(NULL)
{
}

CTaskListDropTarget::~CTaskListDropTarget()
{
	delete m_pOutlook;
	m_pOutlook = NULL;
}

BOOL CTaskListDropTarget::Register(CWnd* pTarget, CWnd* pOwner)
{
	m_pOwner = pOwner;

	return COleDropTarget::Register(pTarget);
}

DROPEFFECT CTaskListDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pObject, DWORD dwKeyState, CPoint point)
{
	ResetDrag(pWnd);

	if (IS_WND_TYPE(pWnd, CTreeCtrl, WC_TREEVIEW))
	{
		CTreeCtrlHelper tch((CTreeCtrl&)*pWnd);
		tch.BuildHTIMap(m_mapTVItems);
	}

	return OnDragOver(pWnd, pObject, dwKeyState, point);
}

void CTaskListDropTarget::OnDragLeave(CWnd* pWnd)
{
	ResetDrag(pWnd);
}

void CTaskListDropTarget::ResetDrag(CWnd* pWnd)
{
	if (IS_WND_TYPE(pWnd, CTreeCtrl, WC_TREEVIEW))
	{
		TreeView_SelectDropTarget(pWnd->GetSafeHwnd(), NULL);
	}
	else if (IS_WND_TYPE(pWnd, CListCtrl, WC_LISTVIEW))
	{
		ListView_SetItemState(pWnd->GetSafeHwnd(), -1, 0, LVIS_DROPHILITED); // all items
	}
	
	m_dwTVHoverStart = 0;
	m_dwPrevItem = 0;
	m_mapTVItems.RemoveAll();
}

CTaskListDropTarget::TLDT_HITTEST CTaskListDropTarget::DoHitTest(CWnd* pWnd, CPoint point, DWORD& dwHitTaskID)
{
	TLDT_HITTEST nHitResult = TLDTHT_NONE;
	dwHitTaskID = 0;
	
	if (IS_WND_TYPE(pWnd, CTreeCtrl, WC_TREEVIEW))
	{
		TVHITTESTINFO tvhti = { { point.x, point.y }, 0, 0 };
		HTREEITEM htiHit = TreeView_HitTest(pWnd->GetSafeHwnd(), &tvhti);

		if (htiHit != NULL)
		{
			TVITEM item = { 0 };
			item.hItem = htiHit;
			item.mask = TVIF_PARAM;
			VERIFY(::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item));

			dwHitTaskID = item.lParam;
		}

		nHitResult = TLDTHT_TASKVIEW;
	}
	else if (IS_WND_TYPE(pWnd, CListCtrl, WC_LISTVIEW))
	{
		LVHITTESTINFO lvhti = { { point.x, point.y }, 0 };
		int nHit = ListView_HitTest(pWnd->GetSafeHwnd(), &lvhti);

		if (nHit != -1)
		{
			LVITEM lvi = { 0 };
			lvi.iItem = nHit;
			lvi.mask = LVIF_PARAM;
			VERIFY(::SendMessage(m_hWnd, LVM_GETITEM, 0, (LPARAM)&lvi));

			dwHitTaskID = lvi.lParam;
		}
		nHitResult = TLDTHT_TASKVIEW;
	}
	else if (IS_WND_TYPE(pWnd, CFileEdit, WC_COMBOBOX))
	{
		if (pWnd->GetStyle() & CBS_DROPDOWN)
			nHitResult = TLDTHT_FILEEDIT;
	}
	else if (pWnd->IsKindOf(RUNTIME_CLASS(CDialog)) ||
	 		pWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
	{
		// allow dropping only on titlebar
		if ((pWnd->GetStyle() & WS_CAPTION) && point.y < 0)
			nHitResult = TLDTHT_CAPTION;
	}

	return nHitResult;
}

DROPEFFECT CTaskListDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pObject, DWORD /*dwKeyState*/, CPoint point)
{
	if (!pWnd->IsWindowEnabled())
		return DROPEFFECT_NONE;

	DWORD dwHitTaskID = 0;
	TLDT_HITTEST nHitTest = DoHitTest(pWnd, point, dwHitTaskID);

	if (nHitTest == TLDTHT_NONE)
		return DROPEFFECT_NONE;

	// update drop hilite
	if (IS_WND_TYPE(pWnd, CTreeCtrl, WC_TREEVIEW))
	{
		HTREEITEM htiHit = NULL;
		m_mapTVItems.Lookup(dwHitTaskID, htiHit);

		// check hover time and expand parent tasks appropriately
		if (dwHitTaskID && (m_dwPrevItem == dwHitTaskID))
		{
			ASSERT(m_dwTVHoverStart);

			if ((GetTickCount() - m_dwTVHoverStart) >= 1000)
			{
				if (TreeView_GetChild(*pWnd, htiHit))
				{	
					CHoldRedraw hr(*pWnd);
					TreeView_Expand(*pWnd, htiHit, TVE_EXPAND);
				}
			}
		}
		else 
		{
			TreeView_SelectDropTarget(*pWnd, htiHit);
			m_dwPrevItem = dwHitTaskID;

			if (dwHitTaskID)
				m_dwTVHoverStart = GetTickCount();
			else
				m_dwTVHoverStart = 0;
		}
	}
	else if (IS_WND_TYPE(pWnd, CListCtrl, WC_LISTVIEW))
	{
		int nHit = -1;

		// remove previous highlighting
		if ((m_dwPrevItem != 0) && (m_dwPrevItem != dwHitTaskID))
		{
			LVFINDINFO lvfi = { 0 };
			lvfi.flags = LVFI_PARAM;
			lvfi.lParam = m_dwPrevItem;
			lvfi.vkDirection = VK_DOWN;

			int nPrev = ListView_FindItem(*pWnd, -1, &lvfi);
			ListView_SetItemState(pWnd->GetSafeHwnd(), nPrev, 0, LVIS_DROPHILITED); 
		}

		if (dwHitTaskID != 0)
		{
			LVFINDINFO lvfi = { 0 };
			lvfi.flags = LVFI_PARAM;
			lvfi.lParam = dwHitTaskID;
			lvfi.vkDirection = VK_DOWN;

			int nHit = ListView_FindItem(*pWnd, -1, &lvfi);
			ListView_SetItemState(pWnd->GetSafeHwnd(), nHit, LVIS_DROPHILITED, LVIS_DROPHILITED);
		}

		m_dwPrevItem = dwHitTaskID;
	}

	// Allow parent to veto the drop
	TLDT_DATA drop(pObject, point);
	drop.dwTaskID = dwHitTaskID;

	CStringArray aFilePaths;
	BOOL bFromText = FALSE;

	if (GetDropFilePaths(pObject, aFilePaths, bFromText))
		drop.pFilePaths = &aFilePaths;

	BOOL bCanDrop = m_pOwner->SendMessage(WM_TLDT_CANDROP, (WPARAM)&drop, (LPARAM)pWnd);

	if (bCanDrop)
		return GetDropEffect(nHitTest, drop, bFromText);
	
	// else
	return DROPEFFECT_NONE;
}

DROPEFFECT CTaskListDropTarget::GetDropEffect(TLDT_HITTEST nHitTest, const TLDT_DATA& drop, BOOL bFromText)
{
	DROPEFFECT deResult = DROPEFFECT_NONE;

	if (CMSOutlookHelper::IsOutlookObject(drop.pObject))
	{
		switch (nHitTest)
		{
		case TLDTHT_TASKVIEW:
			deResult = GetDropEffect(nHitTest, (drop.dwTaskID != 0));
			break;

		case TLDTHT_FILEEDIT:
			deResult = GetDropEffect(nHitTest);
			break;
		}
	}
	else if ((nHitTest != TLDTHT_NONE) && drop.GetFileCount())
	{
		switch (nHitTest)
		{
		case TLDTHT_TASKVIEW:
			deResult = GetDropEffect(nHitTest, (drop.dwTaskID != 0), bFromText);
			break;

		case TLDTHT_FILEEDIT:
			deResult = GetDropEffect(nHitTest, FALSE, bFromText);
			break;

		case TLDTHT_CAPTION:
			{
				CString sFilePath(drop.GetFile(0));

				if (FileMisc::HasExtension(sFilePath, _T(".TDL")) ||
					FileMisc::HasExtension(sFilePath, _T(".XML")))
				{
					deResult = DROPEFFECT_MOVE;
				}
			}
			break;
		}
	}

	return deResult;
}

DROPEFFECT CTaskListDropTarget::GetDropEffect(TLDT_HITTEST nHitTest, BOOL bItemHit, BOOL bFromText)
{
	switch (nHitTest)
	{
	case TLDTHT_TASKVIEW:
		return (bItemHit ? DROPEFFECT_LINK : DROPEFFECT_COPY);

	case TLDTHT_FILEEDIT:
		return (bFromText ? DROPEFFECT_COPY : DROPEFFECT_LINK);
	}

	// all else
	ASSERT(0);
	return DROPEFFECT_NONE;
}

int CTaskListDropTarget::GetDropFilePaths(COleDataObject* pObject, CStringArray& aFiles, BOOL& bFromText)
{
	aFiles.RemoveAll();
	bFromText = FALSE;

	if (!FileMisc::GetDropFilePaths(pObject, aFiles) && pObject->IsDataAvailable(CB_TEXTFORMAT))
	{
		// look for files and URLs in text
		HGLOBAL hGlobal = pObject->GetGlobalData(CB_TEXTFORMAT);

		if (hGlobal)
		{
			CString sText((LPCTSTR)GlobalLock(hGlobal));
			::GlobalUnlock(hGlobal);

			if (FileMisc::IsPath(sText) || WebMisc::IsURL(sText))
			{
				aFiles.Add(sText);
				bFromText = TRUE;
			}
		}
	}

	return aFiles.GetSize();
}

BOOL CTaskListDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pObject, DROPEFFECT /*dropEffect*/, CPoint point)
{
	CString sClass = CWinClasses::GetClass(*pWnd);
	m_pOwner->SetForegroundWindow();

	TLDT_DATA data(pObject, point);
	TLDT_HITTEST nHitTest = DoHitTest(pWnd, point, data.dwTaskID);

	if (CMSOutlookHelper::IsOutlookObject(pObject) && InitializeOutlook())
	{
		data.pOutlookSelection = m_pOutlook->GetSelection();
		data.pObject = NULL;

		m_pOwner->SendMessage(WM_TLDT_DROP, (WPARAM)&data, (LPARAM)pWnd);

		// cleanup
		delete data.pOutlookSelection;
		delete m_pOutlook;
		m_pOutlook = NULL;
	}
	else if (nHitTest != TLDTHT_NONE)
	{
		CStringArray aFilePaths;
		BOOL bFromText = FALSE;
		
		if (GetDropFilePaths(pObject, aFilePaths, bFromText))
		{
			data.pFilePaths = &aFilePaths;
			m_pOwner->SendMessage(WM_TLDT_DROP, (WPARAM)&data, (LPARAM)pWnd);

			data.pFilePaths = NULL;
		}
	}
	
	// cleanup
	ResetDrag(pWnd);

	return TRUE; // because we handle it
}

BOOL CTaskListDropTarget::InitializeOutlook()
{
	if (m_pOutlook == NULL)
		m_pOutlook = new CMSOutlookHelper;

	return (m_pOutlook != NULL);
}

