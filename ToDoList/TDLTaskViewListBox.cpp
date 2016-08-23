// TDLTaskViewListBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLTaskViewListBox.h"

#include "..\shared\DialogHelper.h"
#include "..\shared\UIExtensionMgr.h"
#include "..\shared\EnString.h"
#include "..\shared\misc.h"
#include "..\shared\graphicsmisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#define WM_INITLISTBOX (WM_APP+1)
#define LISTVIEW_INDEX 0xFFFF

const int IMAGESIZE = 16;

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskViewListBox

CTDLTaskViewListBox::CTDLTaskViewListBox(const CUIExtensionMgr* pMgr) : m_pMgrUIExt(pMgr)
{
}

CTDLTaskViewListBox::~CTDLTaskViewListBox()
{
}


BEGIN_MESSAGE_MAP(CTDLTaskViewListBox, CCheckListBoxEx) 
	//{{AFX_MSG_MAP(CTDLTaskViewListBox)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_INITLISTBOX, OnInitListBox)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskViewListBox message handlers

void CTDLTaskViewListBox::PreSubclassWindow() 
{
	if (m_pMgrUIExt)
		PostMessage(WM_INITLISTBOX);
	
	CCheckListBoxEx::PreSubclassWindow();
}

int CTDLTaskViewListBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CCheckListBoxEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_pMgrUIExt)
		return -1;
	
	PostMessage(WM_INITLISTBOX);
	
	return 0;
}

LRESULT CTDLTaskViewListBox::OnInitListBox(WPARAM /*wp*/, LPARAM /*lp*/)
{
	BuildList();
	return 0L;
}

void CTDLTaskViewListBox::BuildList()
{
	// once only
	if (CCheckListBoxEx::GetCount())
		return;

	// Remove sort so that items have same order as images 
//	ASSERT((GetStyle() & CBS_SORT) == 0);
	ASSERT(m_pMgrUIExt);

	if (m_pMgrUIExt)
	{
		VERIFY(m_ilTaskViews.Create(16, 16, (ILC_MASK | ILC_COLOR32), 1, 1));

		// 'list view' is special
		int nItem = AddString(CString(MAKEINTRESOURCE(IDS_LISTVIEW)));

		SetItemData(nItem, LISTVIEW_INDEX);
		SetCheck(nItem, TRUE); // default

		int nImage = AddItemImage(GraphicsMisc::LoadIcon(IDI_LISTVIEW_STD));
		ASSERT(nImage == nItem);

		// add rest of extensions
		int nNumExt = m_pMgrUIExt->GetNumUIExtensions();
		
		for (int nExt = 0; nExt < nNumExt; nExt++)
		{
			nItem = AddString(m_pMgrUIExt->GetUIExtensionMenuText(nExt));

			SetItemData(nItem, nExt);
			SetCheck(nItem, TRUE); // default

			int nImage = AddItemImage(m_pMgrUIExt->GetUIExtensionIcon(nExt), FALSE);
			ASSERT(nImage == nItem);
		}

		if (m_aHiddenViews.GetSize())
		{
			SetHiddenViews(m_aHiddenViews);
			m_aHiddenViews.RemoveAll();
		}
	}
}

int CTDLTaskViewListBox::AddItemImage(HICON hIcon, BOOL bDeleteIcon)
{
	ASSERT(m_ilTaskViews.GetSafeHandle());
	ASSERT(m_ilTaskViews.GetImageCount() < (m_pMgrUIExt->GetNumUIExtensions() + 1));

	if (hIcon == NULL)
	{
		hIcon = AfxGetApp()->LoadIcon(IDI_NULL);
		bDeleteIcon = TRUE;
	}

	int nImage = m_ilTaskViews.Add(hIcon);

	if (bDeleteIcon)
		::DestroyIcon(hIcon);

	return nImage;
}

void CTDLTaskViewListBox::SetHiddenViews(const CStringArray& aTypeIDs)
{
	if (GetSafeHwnd())
	{
		ASSERT(m_pMgrUIExt);

		if (!m_pMgrUIExt)
			return;

		ASSERT(GetCount());

		SetAllChecked(TRUE);

		int nExt = aTypeIDs.GetSize();

		while (nExt--)
		{
			CString sTypeID = aTypeIDs[nExt];

			int nFind = m_pMgrUIExt->FindUIExtension(sTypeID);

			if ((nFind == -1) && (sTypeID == LISTVIEW_TYPE))
				nFind = LISTVIEW_INDEX;

			ASSERT(nFind != -1);

			if (nFind != -1)
			{
				int nItem = CDialogHelper::FindItemByData(*this, nFind);
				ASSERT(nItem != -1);
				
				SetCheck(nItem, FALSE);
			}
		}
	}
	else
	{
		m_aHiddenViews.Copy(aTypeIDs);
	}
}

int CTDLTaskViewListBox::GetHiddenViews(CStringArray& aTypeIDs) const
{
	return GetViews(aTypeIDs, FALSE);
}

int CTDLTaskViewListBox::GetVisibleViews(CStringArray& aTypeIDs) const
{
	return GetViews(aTypeIDs, TRUE);
}

int CTDLTaskViewListBox::GetViews(CStringArray& aTypeIDs, BOOL bVisible) const
{
	aTypeIDs.RemoveAll();
	
	if (GetSafeHwnd())
	{
		ASSERT(m_pMgrUIExt);
		
		if (!m_pMgrUIExt)
			return 0;
		
		int nItem = GetCount();
		CTDLTaskViewListBox* pThis = const_cast<CTDLTaskViewListBox*>(this);
		
		while (nItem--)
		{
			BOOL bChecked = pThis->GetCheck(nItem);

			if ((bVisible && bChecked) || (!bVisible && !bChecked))
			{
				DWORD dwItemData = GetItemData(nItem);
				
				if (dwItemData == LISTVIEW_INDEX)
					aTypeIDs.Add(LISTVIEW_TYPE);
				else
					aTypeIDs.Add(m_pMgrUIExt->GetUIExtensionTypeID((int)dwItemData));
			}
		}
	}
	else if (bVisible)
	{
		int nItem = m_pMgrUIExt->GetExtensionTypeIDs(aTypeIDs);

		// remove hidden views
		Misc::RemoveItems(m_aHiddenViews, aTypeIDs);
	}
	else  // hidden
	{
		aTypeIDs.Copy(m_aHiddenViews);
	}
	
	return aTypeIDs.GetSize();
}

void CTDLTaskViewListBox::OnDestroy() 
{
	GetHiddenViews(m_aHiddenViews);

	CCheckListBoxEx::OnDestroy();
}

void CTDLTaskViewListBox::PreDrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	DRAWITEMSTRUCT drawItem = *lpDrawItemStruct;
	
	if (!DrawCheckbox(&drawItem))
	{
		CCheckListBox::PreDrawItem(lpDrawItemStruct);
		return;
	}
	
	if (m_ilTaskViews.GetSafeHandle())
	{
		int nItem = drawItem.itemID;
		
		if ((nItem >= 0) && (drawItem.itemAction & (ODA_DRAWENTIRE | ODA_SELECT)))
		{
	 		CDC* pDC = CDC::FromHandle(drawItem.hDC);
			CRect rItem(drawItem.rcItem);

			rItem.DeflateRect(0, ((rItem.Height() - IMAGESIZE) / 2));
			m_ilTaskViews.Draw(pDC, GetImageIndex(nItem), rItem.TopLeft(), ILD_TRANSPARENT);
		}
		
		drawItem.rcItem.left += (IMAGESIZE + 2);
	}

	DrawItem(&drawItem);
}

int CTDLTaskViewListBox::GetImageIndex(int nItem) const
{
	int nImage = GetItemData(nItem);
	
	if (nImage == LISTVIEW_INDEX)
		return 0;

	// else
	return (nImage + 1);
}
