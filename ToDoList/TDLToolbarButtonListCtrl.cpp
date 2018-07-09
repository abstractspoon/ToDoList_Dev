// TDLToolbarButtonListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLToolbarButtonListCtrl.h"
#include "TDLTaskIconDlg.h"

#include "..\Shared\DialogHelper.h"
#include "..\Shared\enmenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

enum COLUMNS
{
	MENUID_COL,
	IMAGE_COL
};

const UINT IDC_MENUID_COMBO = 1001;

/////////////////////////////////////////////////////////////////////////////
// CTDLToolbarButtonListCtrl

CTDLToolbarButtonListCtrl::CTDLToolbarButtonListCtrl()
{
}

CTDLToolbarButtonListCtrl::~CTDLToolbarButtonListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTDLToolbarButtonListCtrl, CInputListCtrl)
	//{{AFX_MSG_MAP(CTDLToolbarButtonListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_CBN_CLOSEUP(IDC_MENUID_COMBO, OnMenuItemCancel)
	ON_CBN_SELENDCANCEL(IDC_MENUID_COMBO, OnMenuItemCancel)
	ON_CBN_SELENDOK(IDC_MENUID_COMBO, OnMenuItemOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLToolbarButtonListCtrl message handlers

BOOL CTDLToolbarButtonListCtrl::HasButtons() const
{
	return (m_aButtons.GetSize() > 0);
}

int CTDLToolbarButtonListCtrl::GetButtons(CToolbarButtonArray& aButtons) const
{
	aButtons.Copy(m_aButtons);
	return aButtons.GetSize();
}

void CTDLToolbarButtonListCtrl::SetButtons(const CToolbarButtonArray& aButtons)
{
	m_aButtons.Copy(aButtons);
}

void CTDLToolbarButtonListCtrl::InitState()
{
	OverrideSelectionTheming(TRUE, TRUE);
	ShowGrid(TRUE, TRUE);

	CreateControl(m_cbMenuItems, IDC_MENUID_COMBO, FALSE);

	CEnMenu menu;

	if (menu.LoadMenu(IDR_MAINFRAME, NULL, TRUE, TRUE))
	{
		menu.TranslateDynamicMenuItems(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, _T("Recent Tasklist %d"));
		menu.TranslateDynamicMenuItems(ID_WINDOW1, ID_WINDOW16, _T("Window %d"));
		menu.TranslateDynamicMenuItems(ID_TOOLS_USERTOOL1, ID_TOOLS_USERTOOL50, _T("User Defined Tool %d"));
		menu.TranslateDynamicMenuItems(ID_FILE_OPEN_USERSTORAGE1, ID_FILE_OPEN_USERSTORAGE16, _T("3rd Party Storage %d"));
		menu.TranslateDynamicMenuItems(ID_FILE_SAVE_USERSTORAGE1, ID_FILE_SAVE_USERSTORAGE16, _T("3rd Party Storage %d"));
		menu.TranslateDynamicMenuItems(ID_SHOWVIEW_UIEXTENSION1, ID_SHOWVIEW_UIEXTENSION16, _T("Task View Visibility %d"));
		
		m_cbMenuItems.Initialise(menu, IDS_TOOLBARMENUSEPARATOR);
	}
	else
	{
		m_cbMenuItems.Initialise(IDR_MAINFRAME, IDS_TOOLBARMENUSEPARATOR);
	}

	AddCol(_T("Menu Item"), 350);
	SetColumnType(MENUID_COL, ILCT_DROPLIST);

	AddCol(_T("Image"), 75);
	SetColumnType(IMAGE_COL, ILCT_BROWSE);

	SetAutoRowPrompt(CEnString(IDS_NEW_TOOLBARBUTTON));
	AutoAdd(TRUE, FALSE);

	m_ilImages.LoadDefaultImages(TRUE);

	// Build list
	for (int nBtn = 0; nBtn < m_aButtons.GetSize(); nBtn++)
	{
		const TOOLBARBUTTON& tb = m_aButtons[nBtn];

		int nRow = AddRow(m_cbMenuItems.GetMenuItemText(tb.nMenuID));
		SetItemText(nRow, IMAGE_COL, tb.sImageID);
		SetItemData(nRow, tb.nMenuID);
	}
}

void CTDLToolbarButtonListCtrl::EditCell(int nItem, int nCol, BOOL bBtnClick)
{
	// handle new buttons
	UINT nMenuID = 0;

	if ((nItem == m_aButtons.GetSize()) && (nCol == MENUID_COL))
	{ 
		// separator
		m_aButtons.Add(TOOLBARBUTTON());
		int nRow = AddRow(CDialogHelper::GetItem(m_cbMenuItems, 0));

		SetCurSel(nRow, MENUID_COL);
		EnsureVisible(nRow, FALSE);
	}
	else
	{
		nMenuID = GetItemData(nItem);
	}

	switch (nCol)
	{
	case MENUID_COL:
		ShowControl(m_cbMenuItems, nItem, nCol);
		break;
		
	case IMAGE_COL:
		{
			TOOLBARBUTTON& tb = m_aButtons[nItem];
			CTDLTaskIconDlg dialog(m_ilImages, tb.sImageID, FALSE);
			
			if (dialog.DoModal() == IDOK)
			{
				tb.sImageID = dialog.GetIconName();
				SetItemText(nItem, nCol, tb.sImageID);
			}
		}
		break;
	}
}

void CTDLToolbarButtonListCtrl::PrepareControl(CWnd& ctrl, int nRow, int nCol)
{
	UNREFERENCED_PARAMETER(ctrl);

	if (!m_aButtons.GetSize())
		return;
	
	const TOOLBARBUTTON& tb = m_aButtons[nRow];
	
	switch (nCol)
	{
	case MENUID_COL:
		{
			ASSERT (&ctrl == &m_cbMenuItems);
			
			CDialogHelper::SelectItemByData(m_cbMenuItems, tb.nMenuID);
			CDialogHelper::RefreshMaxDropWidth(m_cbMenuItems);
		}
		break;
	}
}

void CTDLToolbarButtonListCtrl::OnMenuItemCancel()
{
	HideControl(m_cbMenuItems);
}

void CTDLToolbarButtonListCtrl::OnMenuItemOK()
{
	HideControl(m_cbMenuItems);
	
	// update item text and keep data store synched
	int nRow = GetCurSel();
	
	if (nRow != CB_ERR)
	{
		TOOLBARBUTTON& tb = m_aButtons[nRow];
		tb.nMenuID = CDialogHelper::GetSelectedItemData(m_cbMenuItems);

		SetItemData(nRow, tb.nMenuID);
		SetItemText(nRow, MENUID_COL, CDialogHelper::GetSelectedItem(m_cbMenuItems));
	}
}

void CTDLToolbarButtonListCtrl::DrawCellText(CDC* pDC, int nRow, int nCol, 
												  const CRect& rText, const CString& sText, 
												  COLORREF crText, UINT nDrawTextFlags)
{
	if ((nCol == IMAGE_COL) && !IsPrompt(nRow))
	{
		int nImage = m_ilImages.GetImageIndex(sText);
				
		if (nImage != -1)
			m_ilImages.Draw(pDC, nImage, rText.TopLeft(), ILD_TRANSPARENT);
			
		return;
	}
	
	// all else
	CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sText, crText, nDrawTextFlags);
}

BOOL CTDLToolbarButtonListCtrl::CanEditCell(int nRow, int nCol) const
{
	if ((nCol == IMAGE_COL) && !IsPrompt(nRow))
	{
		return (GetItemData(nRow) != 0);
	}

	return CInputListCtrl::CanEditCell(nRow, nCol);
}

BOOL CTDLToolbarButtonListCtrl::MoveButton(int nBtn, int nRows)
{
	if (!CanMoveButton(nBtn, nRows))
		return FALSE;

	TOOLBARBUTTON tb = m_aButtons[nBtn]; // copy
	m_aButtons.RemoveAt(nBtn);

	CString sMenuItem = GetItemText(nBtn, MENUID_COL);
	DeleteItem(nBtn);
	
	int nNewPos = (nBtn + nRows);
	m_aButtons.InsertAt(nNewPos, tb);

	int nRow = InsertRow(sMenuItem, nNewPos);
	SetItemData(nRow, tb.nMenuID);
	SetItemText(nRow, IMAGE_COL, tb.sImageID);

	SetCurSel(nRow);

	return TRUE;
}

BOOL CTDLToolbarButtonListCtrl::CanMoveButton(int nBtn, int nRows) const
{
	if ((nRows == 0) || IsPrompt(nBtn))
		return FALSE;

	int nNewPos = (nBtn + nRows);

	if ((nNewPos < 0) || (nNewPos >= (GetItemCount() - 1)))
		return FALSE;

	return TRUE;
}

BOOL CTDLToolbarButtonListCtrl::CanMoveSelectedButtonUp() const
{
	return CanMoveButton(GetCurSel(), -1);
}

BOOL CTDLToolbarButtonListCtrl::CanMoveSelectedButtonDown() const
{
	return CanMoveButton(GetCurSel(), 1);
}

BOOL CTDLToolbarButtonListCtrl::CanDeleteSelectedButton() const
{
	return CanDeleteCell(GetCurSel(), 0);
}

BOOL CTDLToolbarButtonListCtrl::MoveSelectedButtonUp()
{
	return MoveButton(GetCurSel(), -1);
}

BOOL CTDLToolbarButtonListCtrl::MoveSelectedButtonDown()
{
	return MoveButton(GetCurSel(), 1);
}

BOOL CTDLToolbarButtonListCtrl::DeleteSelectedButton()
{
	int nRow = GetCurSel();

	if (CanDeleteSelectedButton())
	{
		m_aButtons.RemoveAt(nRow);
		DeleteItem(nRow);

		SetCurSel(nRow);

		return TRUE;
	}

	return FALSE;
}
