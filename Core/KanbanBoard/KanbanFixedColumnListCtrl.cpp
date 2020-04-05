// KanbanFixedColumnListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KanbanFixedColumnListCtrl.h"

#include "..\shared\misc.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\Localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

enum
{
	KFCL_TITLECOL,
	KFCL_VALUECOL,
//	KFCL_MAXNUMCOL,
	KFCL_COLORCOL,
};

const UINT IDC_VALUES = 5000;

/////////////////////////////////////////////////////////////////////////////
// CKanbanFixedColumnListCtrl

CKanbanFixedColumnListCtrl::CKanbanFixedColumnListCtrl()
{
	//m_cbValues.SetMinDLUHeight(9);
}

CKanbanFixedColumnListCtrl::~CKanbanFixedColumnListCtrl()
{
}


BEGIN_MESSAGE_MAP(CKanbanFixedColumnListCtrl, CInputListCtrl)
	//{{AFX_MSG_MAP(CKanbanFixedColumnListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_CBN_CLOSEUP(IDC_VALUES, OnValueEditCancel)
	ON_CBN_SELENDCANCEL(IDC_VALUES, OnValueEditCancel)
	ON_CBN_SELCHANGE(IDC_VALUES, OnValueEditOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKanbanFixedColumnListCtrl message handlers

COLORREF CKanbanFixedColumnListCtrl::GetItemColor(int nRow) const
{
	ASSERT(GetSafeHwnd());

	// Zero == CLR_NONE
	return (GetItemData(nRow) - 1);
}

void CKanbanFixedColumnListCtrl::SetItemColor(int nRow, COLORREF color)
{
	ASSERT(GetSafeHwnd());

	// Zero == CLR_NONE
	SetItemData(nRow, (color + 1));
}

BOOL CKanbanFixedColumnListCtrl::SetColumnDefinitions(const CKanbanColumnArray& aColumnDefs)
{
	if (!GetSafeHwnd())
	{
		ASSERT(0);
		return FALSE;
	}

	// fill column defs
	DeleteAllItems(FALSE);
	int nNumDefs = aColumnDefs.GetSize();
	
	for (int nDef = 0; nDef < nNumDefs; nDef++)
	{
		const KANBANCOLUMN& colDef = aColumnDefs.GetData()[nDef];
		
		int nItem = InsertRow(colDef.sTitle, nDef);
		
		SetItemText(nItem, KFCL_VALUECOL, Misc::FormatArray(colDef.aAttribValues));
		SetItemColor(nItem, colDef.crBackground);
		
// 		if (colDef.nMaxTaskCount > 0)
// 			SetItemText(nItem, KFCL_MAXNUMCOL, Misc::Format(colDef.nMaxTaskCount));
	}

	return TRUE;
}

int CKanbanFixedColumnListCtrl::GetColumnDefinitions(CKanbanColumnArray& aColumnDefs) const
{
	aColumnDefs.RemoveAll();
	
	int nNumDefs = (GetItemCount() - 1);
	
	for (int nDef = 0; nDef < nNumDefs; nDef++)
	{
		KANBANCOLUMN colDef;
		
		colDef.sTitle = GetItemText(nDef, KFCL_TITLECOL);
		colDef.crBackground = GetItemColor(nDef);
// 		colDef.nMaxTaskCount = _ttoi(GetItemText(nDef, KFCL_MAXNUMCOL));

		CString sValues(GetItemText(nDef, KFCL_VALUECOL));

		if (sValues.IsEmpty())
			colDef.aAttribValues.Add(sValues);
		else
			Misc::Split(sValues, colDef.aAttribValues);

		aColumnDefs.Add(colDef);
	}

	return nNumDefs;
}

void CKanbanFixedColumnListCtrl::SetAttributeValues(const CStringArray& aValues)
{
	ASSERT(GetSafeHwnd());

	CDialogHelper::SetComboBoxItems(m_cbValues, aValues);
}

void CKanbanFixedColumnListCtrl::InitState()
{
	CInputListCtrl::InitState();

	// create child controls 
	// we need combo to be created first
	CreateControl(m_cbValues, IDC_VALUES);
	CLocalizer::EnableTranslation(m_cbValues, FALSE);

	CRect rClient;
	GetClientRect(rClient);
	
	AddCol(CEnString(IDS_COLUMNTITLE),	((rClient.Width() * 3) / 10));
	AddCol(CEnString(IDS_ATTRIBVALUES),	((rClient.Width() * 3) / 10), ILCT_DROPLIST);
//	AddCol(CEnString(IDS_MAXTASKCOUNT),	((rClient.Width() * 2) / 10));

	if (!Misc::IsHighContrastActive())
		AddCol(CEnString(IDS_BKGNDCOLOR),	((rClient.Width() * 2) / 10), ILCT_BROWSE);
		
	SetAutoRowPrompt(CEnString(IDS_NEWCOLUMN));
	AutoAdd(TRUE, FALSE);
	ShowGrid(TRUE, TRUE);
	EnableHeaderTracking(FALSE);
}

void CKanbanFixedColumnListCtrl::EditCell(int nItem, int nCol, BOOL bBtnClick)
{
	switch (nCol)
	{
	case KFCL_VALUECOL:
		PrepareEdit(nItem, nCol);
		ShowControl(m_cbValues, nItem, nCol); 
		break;

	case KFCL_COLORCOL:
		ASSERT(!Misc::IsHighContrastActive());
		{
			CColorDialog dialog(GetItemColor(nItem), CC_FULLOPEN | CC_RGBINIT);

			if (dialog.DoModal() == IDOK)
			{
				COLORREF color = dialog.GetColor();

				if (color == GetSysColor(COLOR_WINDOW))
					color = CLR_NONE;

				SetItemColor(nItem, color);
			}
		}
		break;

	case KFCL_TITLECOL:
	default:
		CInputListCtrl::EditCell(nItem, nCol, bBtnClick);
		break;
	}
}

BOOL CKanbanFixedColumnListCtrl::IsEditing() const 
{
	return (CInputListCtrl::IsEditing() || m_cbValues.IsWindowVisible());
}

void CKanbanFixedColumnListCtrl::PrepareEdit(int nRow, int nCol)
{
	if (nCol == KFCL_VALUECOL)
	{
		CStringArray aValues;
		Misc::Split(GetItemText(nRow, nCol), aValues);

		m_cbValues.SetChecked(aValues);
	}		
}

void CKanbanFixedColumnListCtrl::OnValueEditCancel()
{
	HideControl(m_cbValues);
}

void CKanbanFixedColumnListCtrl::OnValueEditOK()
{
	HideControl(m_cbValues);

	CStringArray aValues;
	m_cbValues.GetChecked(aValues);

	SetCellText(GetCurSel(), KFCL_VALUECOL, Misc::FormatArray(aValues));
}

void CKanbanFixedColumnListCtrl::DrawCellText(CDC* pDC, int nRow, int nCol, 
												const CRect& rText, const CString& sText, 
												COLORREF crText, UINT nDrawTextFlags)
{
	if ((nCol == KFCL_COLORCOL) && !IsPrompt(nRow))
	{
		ASSERT (!Misc::IsHighContrastActive());

		COLORREF color = GetItemColor(nRow);

		if (color != CLR_NONE)
		{
			CRect rColor(rText);
			rColor.DeflateRect(1, 3, 4, 3);

			GraphicsMisc::DrawRect(pDC, rColor, color, GraphicsMisc::Darker(color, 0.4));
		}
	}
	else
	{
		CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sText, crText, nDrawTextFlags);
	}
}

BOOL CKanbanFixedColumnListCtrl::CanMoveSelectedColumnRow(BOOL bUp) const
{
	int nRow = GetCurSel();
	int nNumRows = (GetItemCount() - 1);

	if ((nRow < 0) || (nRow >= nNumRows))
		return FALSE;

	if (bUp)
		nRow--;
	else
		nRow++;

	return ((nRow >= 0) && (nRow < nNumRows));
}

BOOL CKanbanFixedColumnListCtrl::MoveSelectedColumnRow(BOOL bUp)
{
	if (!CanMoveSelectedColumnRow(bUp))
		return FALSE;

	int nRow = GetCurSel();

	CString sTitle = GetItemText(nRow, 0);
	CString sValue = GetItemText(nRow, 1);
	CString sMaxTasks = GetItemText(nRow, 2);
	COLORREF crBack = GetItemColor(nRow);

	DeleteItem(nRow);

	int nNewRow = (nRow + (bUp ? -1 : 1));

	nRow = InsertItem(nNewRow, sTitle);
	SetItemText(nRow, 1, sValue);
	SetItemText(nRow, 2, sMaxTasks);
	SetItemColor(nRow, crBack);

	SetCurSel(nRow);

	return TRUE;
}
