// MenuComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "MenuComboBox.h"
#include "enMenu.h"
#include "AcceleratorString.h"
#include "Localizer.h"
#include "enString.h"
#include "dialoghelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuComboBox

CMenuComboBox::CMenuComboBox()
{
}

CMenuComboBox::~CMenuComboBox()
{
}


BEGIN_MESSAGE_MAP(CMenuComboBox, CComboBox)
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
		AddMenuItem(menu, nPos, _T(""));

	return (GetCount() > 0);
}

void CMenuComboBox::AddMenuItem(HMENU hMenu, int nPos, const CString& sPath)
{
	if (CEnMenu::IsSeparator(hMenu, nPos))
		return;

	CString sItem = CEnMenu::GetMenuString(hMenu, nPos, MF_BYPOSITION), sItemPath;
	CAcceleratorString::RemoveAccelerator(sItem);
	
	if (!sPath.IsEmpty())
		sItemPath = (sPath + _T(" > "));
	
	sItemPath += sItem;

	if (CEnMenu::IsPopop(hMenu, nPos))
	{
		HMENU hSubMenu = ::GetSubMenu(hMenu, nPos);
		int nNumPos = ::GetMenuItemCount(hSubMenu);
		
		for (int nPos = 0; nPos < nNumPos; nPos++)
			AddMenuItem(hSubMenu, nPos, sItemPath); // RECURSIVE CALL
	}
	else
	{
		CDialogHelper::AddString(*this, sItemPath, ::GetMenuItemID(hMenu, nPos));
	}
}

CString CMenuComboBox::GetMenuItemText(UINT nMenuID) const
{
	int nItem = CDialogHelper::FindItemByData(*this, nMenuID);

	return ((nItem == -1) ? _T("") : CDialogHelper::GetItem(*this, nItem));
}
