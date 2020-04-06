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
	ptClient(pt),
	bImportTasks(FALSE)
{
}

CString TLDT_DATA::GetFirstFile() const 
{ 
	if (pFilePaths && pFilePaths->GetSize())
		return pFilePaths->GetAt(0);
	
	// else
	return _T(""); 
}

BOOL TLDT_DATA::HasFiles() const
{
	return (GetFileCount() > 0);
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

BOOL TLDT_DATA::HasText() const
{
	return (pObject && CClipboard::HasText(pObject));
}

CString TLDT_DATA::GetText() const
{
	if (pObject)
		return CClipboard::GetText(pObject);
	
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
		m_mapTVItems.BuildMap((CTreeCtrl&)*pWnd);

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

CTaskListDropTarget::TLDT_HITTEST CTaskListDropTarget::DoHitTest(CWnd* pWnd, CPoint point, DWORD& dwHitTaskID, BOOL& bImport)
{
	dwHitTaskID = 0;
	bImport = FALSE;
	
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
			bImport = Misc::ModKeysArePressed(MKS_ALT);
		}
		else
		{
			bImport = TRUE;
		}

		return TLDTHT_TASKVIEW;
	}

	// else
	if (IS_WND_TYPE(pWnd, CListCtrl, WC_LISTVIEW))
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
			bImport = Misc::ModKeysArePressed(MKS_ALT);
		}
		else
		{
			bImport = TRUE;
		}

		return TLDTHT_TASKVIEW;
	}

	// else 
	if (IS_WND_TYPE(pWnd, CFileEdit, WC_COMBOBOX))
	{
		if (pWnd->GetStyle() & CBS_DROPDOWN) // must be editable
			return TLDTHT_FILEEDIT;
	}
	else if (pWnd->IsKindOf(RUNTIME_CLASS(CDialog)) || pWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
	{
		// allow dropping only on titlebar
		if ((pWnd->GetStyle() & WS_CAPTION) && (point.y < 0))
			return TLDTHT_CAPTION;
	}

	// all else
	return TLDTHT_NONE;
}

DROPEFFECT CTaskListDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pObject, DWORD /*dwKeyState*/, CPoint point)
{
	if (!pWnd->IsWindowEnabled())
		return DROPEFFECT_NONE;

	DWORD dwHitTaskID = 0;
	BOOL bImport = FALSE;
	TLDT_HITTEST nHitTest = DoHitTest(pWnd, point, dwHitTaskID, bImport);

	if (nHitTest == TLDTHT_NONE)
		return DROPEFFECT_NONE;

	// update drop hilite
	if (IS_WND_TYPE(pWnd, CTreeCtrl, WC_TREEVIEW))
	{
		HTREEITEM htiHit = m_mapTVItems.GetItem(dwHitTaskID);

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
	drop.bImportTasks = bImport;

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

DROPEFFECT CTaskListDropTarget::GetDropEffect(TLDT_HITTEST nHitTest, const TLDT_DATA& drop, BOOL bFilesFromText)
{
	// Note: CF_HDROP does not support DROPEFFECT_LINK when the source is text
	if (CMSOutlookHelper::IsOutlookObject(drop.pObject))
	{
		switch (nHitTest)
		{
		case TLDTHT_TASKVIEW:
			return ((drop.dwTaskID && !drop.bImportTasks && !bFilesFromText) ? DROPEFFECT_LINK : DROPEFFECT_COPY);

		case TLDTHT_FILEEDIT:
			return (bFilesFromText ? DROPEFFECT_COPY : DROPEFFECT_LINK);
		}
	}
	else if ((nHitTest != TLDTHT_NONE) && (drop.HasFiles() || drop.HasText()))
	{
		switch (nHitTest)
		{
		case TLDTHT_TASKVIEW:
			return ((drop.dwTaskID && !drop.bImportTasks && !bFilesFromText && !drop.HasText()) ? DROPEFFECT_LINK : DROPEFFECT_COPY);

		case TLDTHT_FILEEDIT:
			return (bFilesFromText ? DROPEFFECT_COPY : DROPEFFECT_LINK);

		case TLDTHT_CAPTION:
			{
				CString sFilePath(drop.GetFile(0));

				if (FileMisc::HasExtension(sFilePath, _T(".TDL")) ||
					FileMisc::HasExtension(sFilePath, _T(".XML")))
				{
					return DROPEFFECT_COPY;
				}
			}
			break;
		}
	}

	// All else
	return DROPEFFECT_NONE;
}

int CTaskListDropTarget::GetDropFilePaths(COleDataObject* pObject, CStringArray& aFiles, BOOL& bFromText)
{
	ASSERT(pObject);

	aFiles.RemoveAll();
	bFromText = FALSE;

	if (!FileMisc::GetDropFilePaths(pObject, aFiles) && CClipboard::HasText(pObject))
	{
		// look for files and URLs in text
		CString sText = CClipboard::GetText(pObject);

		if (FileMisc::IsPath(sText) || (WebMisc::IsURL(sText) && (sText.FindOneOf(_T("\t\r\n")) == -1)))
		{
			aFiles.Add(sText);
			bFromText = TRUE;
		}
	}

	return aFiles.GetSize();
}

BOOL CTaskListDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pObject, DROPEFFECT /*dropEffect*/, CPoint point)
{
	CString sClass = CWinClasses::GetClass(*pWnd);
	m_pOwner->SetForegroundWindow();

	TLDT_DATA data(pObject, point);
	TLDT_HITTEST nHitTest = DoHitTest(pWnd, point, data.dwTaskID, data.bImportTasks);

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
		else if (CClipboard::HasText(pObject))
		{
			m_pOwner->SendMessage(WM_TLDT_DROP, (WPARAM)&data, (LPARAM)pWnd);
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

