// TaskListDropTarget.cpp: implementation of the CTaskListDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaskListDropTarget.h"

#include "..\shared\wclassdefines.h"
#include "..\shared\winclasses.h"
#include "..\shared\filemisc.h"
#include "..\shared\webmisc.h"
#include "..\shared\msoutlookhelper.h"
#include "..\shared\fileedit.h"
#include "..\shared\holdredraw.h"

#include "..\3rdparty\msoutl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace OutlookAPI;

//////////////////////////////////////////////////////////////////////

TLDT_DATA::TLDT_DATA(COleDataObject* pObj, CPoint pt) 
	: 
	hti(NULL), 
	nItem(-1), 
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
	m_nLVPrevHilite(-1), 
	m_htiTVPrevItem(NULL),
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

DROPEFFECT CTaskListDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* /*pObject*/, DWORD /*dwKeyState*/, CPoint /*point*/)
{
	if (IS_WND_TYPE(pWnd, CTreeCtrl, WC_TREEVIEW))
	{
		TreeView_SelectDropTarget(pWnd->GetSafeHwnd(), NULL);

		m_htiTVPrevItem = NULL;
		m_dwTVHoverStart = 0;
	}
	else if (IS_WND_TYPE(pWnd, CListCtrl, WC_LISTVIEW))
	{
		if (m_nLVPrevHilite != -1) // shouldn't happen
			ListView_SetItemState(pWnd->GetSafeHwnd(), m_nLVPrevHilite, 0, LVIS_DROPHILITED); // all items

		m_nLVPrevHilite = -1;
	}

	return DROPEFFECT_NONE;
}

void CTaskListDropTarget::OnDragLeave(CWnd* pWnd)
{
	if (IS_WND_TYPE(pWnd, CTreeCtrl, WC_TREEVIEW))
	{
		TreeView_SelectDropTarget(pWnd->GetSafeHwnd(), NULL);

		m_htiTVPrevItem = NULL;
		m_dwTVHoverStart = 0;
	}
	else if (IS_WND_TYPE(pWnd, CListCtrl, WC_LISTVIEW))
	{
		if (m_nLVPrevHilite != -1) // shouldn't happen
			ListView_SetItemState(pWnd->GetSafeHwnd(), m_nLVPrevHilite, 0, LVIS_DROPHILITED); // all items

		m_nLVPrevHilite = -1;
	}
}

CTaskListDropTarget::TLDT_HITTEST CTaskListDropTarget::DoHitTest(CWnd* pWnd, CPoint point, 
																 HTREEITEM& htiHit, int& nHit, BOOL& bClient)
{
	TLDT_HITTEST nHitResult = TLDTHT_NONE;

	htiHit = NULL;
	nHit = -1;

	if (IS_WND_TYPE(pWnd, CTreeCtrl, WC_TREEVIEW))
	{
		TVHITTESTINFO tvhti = { { point.x, point.y }, 0, 0 };
		TreeView_HitTest(pWnd->GetSafeHwnd(), &tvhti);
		
		htiHit = tvhti.hItem;
		nHitResult = TLDTHT_TREE;
		bClient = (point.x >= 0);
	}
	else if (IS_WND_TYPE(pWnd, CListCtrl, WC_LISTVIEW))
	{
		LVHITTESTINFO lvhti = { { point.x, point.y }, 0 };
		ListView_HitTest(pWnd->GetSafeHwnd(), &lvhti);
		
		nHit = lvhti.iItem;
		nHitResult = TLDTHT_LIST;
		bClient = (point.x >= 0);
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

	HTREEITEM htiHit = NULL;
	int nListHit = -1;
	BOOL bClientHit = TRUE;

	TLDT_HITTEST nHitTest = DoHitTest(pWnd, point, htiHit, nListHit, bClientHit);

	// update drop hilites
	if (nHitTest == TLDTHT_TREE)
	{
		// check hover time and expand parent tasks appropriately
		if (htiHit && (m_htiTVPrevItem == htiHit))
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
			m_htiTVPrevItem = htiHit;

			if (htiHit)
				m_dwTVHoverStart = GetTickCount();
			else
				m_dwTVHoverStart = 0;
		}
	}
	else if (nHitTest == TLDTHT_LIST)
	{
		// remove previous highlighting
		if (m_nLVPrevHilite != -1 && m_nLVPrevHilite != nListHit)
			ListView_SetItemState(pWnd->GetSafeHwnd(), m_nLVPrevHilite, 0, LVIS_DROPHILITED); 
		
		if (nListHit != -1)
			ListView_SetItemState(pWnd->GetSafeHwnd(), nListHit, LVIS_DROPHILITED, LVIS_DROPHILITED);
		
		m_nLVPrevHilite = nListHit;
	}

	DROPEFFECT deResult = DROPEFFECT_NONE;

	if (CMSOutlookHelper::IsOutlookObject(pObject))
	{
		switch (nHitTest)
		{
		case TLDTHT_TREE:
			deResult = (bClientHit ? DROPEFFECT_LINK : DROPEFFECT_COPY);
			break;
			
		case TLDTHT_LIST:
			deResult = (bClientHit ? DROPEFFECT_LINK : DROPEFFECT_COPY);
			break;
			
		case TLDTHT_FILEEDIT:
			deResult = DROPEFFECT_LINK;
			break;
		}
	}
	else if (nHitTest != TLDTHT_NONE)
	{
		CStringArray aFilePaths;
		BOOL bFromText = FALSE;

		int nNumFiles = GetDropFilePaths(pObject, aFilePaths, bFromText);

		if (nNumFiles)
		{
			switch (nHitTest)
			{
			case TLDTHT_TREE:
			case TLDTHT_LIST:
			case TLDTHT_FILEEDIT:
				deResult = (bFromText ? DROPEFFECT_COPY : DROPEFFECT_LINK);
				break;

			case TLDTHT_CAPTION:
				if (FileMisc::HasExtension(aFilePaths[0], _T(".TDL")) ||
					FileMisc::HasExtension(aFilePaths[0], _T(".XML")))
				{
					deResult = DROPEFFECT_COPY;
				}
				break;
			}
		}
	}

	// else
	return deResult;
}

int CTaskListDropTarget::GetDropFilePaths(COleDataObject* pObject, CStringArray& aFiles, BOOL& bFromText)
{
	aFiles.RemoveAll();
	bFromText = FALSE;

	CLIPFORMAT nTextFmt =
#ifndef _UNICODE
	CF_TEXT;
#else
	CF_UNICODETEXT;
#endif

	if (!FileMisc::GetDropFilePaths(pObject, aFiles) && pObject->IsDataAvailable(nTextFmt))
	{
		// look for files and URLs in text
		HGLOBAL hGlobal = pObject->GetGlobalData(nTextFmt);

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
	BOOL bClientHit = TRUE;

	TLDT_HITTEST nHitTest = DoHitTest(pWnd, point, data.hti, data.nItem, bClientHit);

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
	if (nHitTest == TLDTHT_TREE)
	{
		TreeView_SelectDropTarget(pWnd->GetSafeHwnd(), NULL);
		m_htiTVPrevItem = NULL;
	}
	else if (nHitTest == TLDTHT_LIST)
	{
		// remove previous highlighting
		if (m_nLVPrevHilite != -1)
		{
			ListView_SetItemState(pWnd->GetSafeHwnd(), m_nLVPrevHilite, 0, LVIS_DROPHILITED); 
			m_nLVPrevHilite = -1;
		}
		
		if (data.nItem != -1)
			ListView_SetItemState(pWnd->GetSafeHwnd(), data.nItem, 0, LVIS_DROPHILITED);
	}

	return TRUE; // because we handle it
}

BOOL CTaskListDropTarget::InitializeOutlook()
{
	if (m_pOutlook == NULL)
	{
		m_pOutlook = new CMSOutlookHelper;
	}

	return (m_pOutlook != NULL);
}

