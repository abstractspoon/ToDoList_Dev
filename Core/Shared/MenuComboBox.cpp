// MenuComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "MenuComboBox.h"
#include "enMenu.h"
#include "AcceleratorString.h"
#include "Localizer.h"
#include "enString.h"
#include "dialoghelper.h"
#include "graphicsmisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

DWORD POPUP_ITEMDATA = -1;

/////////////////////////////////////////////////////////////////////////////
// CMenuComboBox

CMenuComboBox::CMenuComboBox() : CTabbedComboBox(GraphicsMisc::ScaleByDPIFactor(16))
{
}

CMenuComboBox::~CMenuComboBox()
{
}


BEGIN_MESSAGE_MAP(CMenuComboBox, CTabbedComboBox)
	//{{AFX_MSG_MAP(CMenuComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuComboBox message handlers

BOOL CMenuComboBox::Initialise(UINT nMenuID, UINT nSeparatorResID)
{
	CMenu menu;

	if (!menu.LoadMenu(nMenuID))
		return FALSE;

	CLocalizer::TranslateMenu(menu);
	CLocalizer::EnableTranslation(*this, FALSE);

	return Initialise(menu, nSeparatorResID);
}

BOOL CMenuComboBox::Initialise(const CMenu& menu, UINT nSeparatorResID)
{
	if (!GetSafeHwnd() || (GetStyle() & CBS_SORT))
	{
		ASSERT(0);
		return FALSE;
	}

	ResetContent();

	if (nSeparatorResID > 0)
	{
		AddString(CEnString(nSeparatorResID));
		SetItemData(0, 0);
	}

	// Add recursively
	int nNumPos = ::GetMenuItemCount(menu);
	
	for (int nPos = 0; nPos < nNumPos; nPos++)
		AddMenuItem(menu, nPos, _T(""), 0);

	return (GetCount() > 0);
}

void CMenuComboBox::AddMenuItem(HMENU hMenu, int nPos, const CString& sPath, int nIndent)
{
	if (CEnMenu::IsSeparator(hMenu, nPos))
		return;

	CString sItem = CEnMenu::GetMenuString(hMenu, nPos, MF_BYPOSITION), sItemPath;
	CAcceleratorString::RemoveAccelerator(sItem);
	
	if (!sPath.IsEmpty())
		sItemPath = (sPath + _T(" > "));
	
	sItemPath += sItem;

	if (nIndent)
		sItem = (CString('\t', nIndent) + sItem);

	if (CEnMenu::IsPopop(hMenu, nPos))
	{
		CDialogHelper::AddString(*this, sItem, POPUP_ITEMDATA);

		// children
		HMENU hSubMenu = ::GetSubMenu(hMenu, nPos);
		int nNumPos = ::GetMenuItemCount(hSubMenu);
		
		for (int nPos = 0; nPos < nNumPos; nPos++)
			AddMenuItem(hSubMenu, nPos, sItemPath, nIndent + 1); // RECURSIVE CALL
	}
	else
	{
		int nItem = CDialogHelper::AddString(*this, sItem, ::GetMenuItemID(hMenu, nPos));

		MCB_ITEMDATA* pItemData = (MCB_ITEMDATA*)GetAddExtItemData(nItem);

		if (pItemData)
			pItemData->sFullPath = sItemPath;
	}
}

CString CMenuComboBox::GetMenuItemText(UINT nMenuID, BOOL bFullPath) const
{
	int nItem = CDialogHelper::FindItemByData(*this, nMenuID);

	return GetItemText(nItem, bFullPath);
}

CString CMenuComboBox::GetItemText(int nItem, BOOL bFullPath) const
{
	if (nItem == -1)
		return _T("");

	const MCB_ITEMDATA* pItemData = (MCB_ITEMDATA*)GetExtItemData(nItem);

	if (!bFullPath || !pItemData || !pItemData->dwItemData)
		return CDialogHelper::GetItem(*this, nItem);

	// else
	ASSERT(!pItemData->sFullPath.IsEmpty());

	return pItemData->sFullPath;
}

void CMenuComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
						  DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	CString sText = (bList ? sItem : GetItemText(nItem, TRUE));

	CTabbedComboBox::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sText, bList, crText);
}

void CMenuComboBox::GetItemColors(int nItem, UINT nItemState, DWORD dwItemData,
								  COLORREF& crText, COLORREF& crBack) const
{
	CTabbedComboBox::GetItemColors(nItem, nItemState, dwItemData, crText, crBack);

	if (dwItemData == POPUP_ITEMDATA)
	{
		crText = GetSysColor(COLOR_WINDOWTEXT);
		crBack = GetSysColor(COLOR_3DLIGHT);
	}
}

BOOL CMenuComboBox::IsSelectableItem(int nItem) const
{
	if (!CTabbedComboBox::IsSelectableItem(nItem))
		return FALSE;

	return (GetItemData(nItem) != POPUP_ITEMDATA);
}
