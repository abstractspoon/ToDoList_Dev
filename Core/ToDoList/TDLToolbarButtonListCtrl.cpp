// TDLToolbarButtonListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLToolbarButtonListCtrl.h"
#include "TDLTaskIconDlg.h"
#include "TDCMainMenu.h"

#include "..\Shared\DialogHelper.h"
#include "..\Shared\GraphicsMisc.h"
#include "..\Shared\Localizer.h"
#include "..\Shared\Misc.h"

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
	if (!Misc::MatchAllT(aButtons, m_aButtons, TRUE))
	{
		m_aButtons.Copy(aButtons);

		if (GetSafeHwnd())
			PopulateList();
	}
}

UINT CTDLToolbarButtonListCtrl::GetLastButtonID() const
{
	int nBtn = m_aButtons.GetSize();

	while (nBtn--)
	{
		if (!m_aButtons[nBtn].IsSeparator())
			return m_aButtons[nBtn].nMenuID;
	}

	// all else
	return 0;
}

void CTDLToolbarButtonListCtrl::InitState()
{
	CInputListCtrl::InitState();

	OverrideSelectionTheming(TRUE, TRUE);
	ShowGrid(TRUE, TRUE);

	CreateControl(m_cbMenuItems, IDC_MENUID_COMBO, FALSE);

	CTDCMainMenu menu;
	VERIFY(menu.LoadMenu());

	VERIFY(m_cbMenuItems.Initialise(menu, IDS_TOOLBARMENUSEPARATOR));

	AddCol(_T("Menu Item"), GraphicsMisc::ScaleByDPIFactor(350), ILCT_DROPLIST);
	AddCol(_T("Image"), GraphicsMisc::ScaleByDPIFactor(75), ILCT_BROWSE);

	SetAutoRowPrompt(CEnString(IDS_NEW_TOOLBARBUTTON));
	AutoAdd(TRUE, FALSE);

	m_ilImages.LoadDefaultImages(TRUE);

	PopulateList();
}

void CTDLToolbarButtonListCtrl::PopulateList()
{
	DeleteAllItems(FALSE);

	for (int nBtn = 0; nBtn < m_aButtons.GetSize(); nBtn++)
	{
		const TOOLBARBUTTON& tb = m_aButtons[nBtn];

		int nRow = AddRow(m_cbMenuItems.GetMenuItemText(tb.nMenuID));
		SetItemText(nRow, IMAGE_COL, tb.sImageID);
		SetItemData(nRow, tb.nMenuID);
	}
}

void CTDLToolbarButtonListCtrl::EditCell(int nItem, int nCol, BOOL /*bBtnClick*/)
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
	
	// update item text and keep data store synced
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
		// Don't draw images for separators
		if (GetItemData(nRow) == ID_SEPARATOR)
			return;

		int nImage = m_ilImages.GetImageIndex(sText);
				
		if (nImage != -1)
		{
			CRect rImage(rText);
			rImage.bottom = (rImage.top + m_ilImages.GetImageSize());

			GraphicsMisc::CentreRect(rImage, rText);

			m_ilImages.Draw(pDC, nImage, rImage.TopLeft(), ILD_TRANSPARENT);
		}
	}
	else
	{
		CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sText, crText, nDrawTextFlags);
	}
}

BOOL CTDLToolbarButtonListCtrl::CanEditCell(int nRow, int nCol) const
{
	if ((nCol == IMAGE_COL) && !IsPrompt(nRow))
	{
		return (GetItemData(nRow) != ID_SEPARATOR);
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

BOOL CTDLToolbarButtonListCtrl::CanDuplicateSelectedButton() const
{
	return !IsPrompt(GetCurSel());
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
	if (CanDeleteSelectedButton())
	{
		int nRow = GetCurSel();
		
		m_aButtons.RemoveAt(nRow);
		DeleteItem(nRow);

		SetCurSel(nRow);

		return TRUE;
	}

	return FALSE;
}

BOOL CTDLToolbarButtonListCtrl::DuplicateSelectedButton()
{
	if (CanDuplicateSelectedButton())
	{
		int nRow = GetCurSel();
		
		TOOLBARBUTTON tb = m_aButtons[nRow]; // copy
		m_aButtons.InsertAt(nRow + 1, tb);

		int nDupeRow = InsertRow(GetItemText(nRow, MENUID_COL), nRow + 1);

		SetItemData(nDupeRow, tb.nMenuID);
		SetItemText(nDupeRow, IMAGE_COL, tb.sImageID);
		
		SetCurSel(nDupeRow);
		
		return TRUE;
	}
	
	return FALSE;
}
