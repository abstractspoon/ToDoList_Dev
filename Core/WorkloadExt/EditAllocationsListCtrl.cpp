// EditAllocationsListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "EditAllocationsListCtrl.h"

#include "..\Shared\misc.h"
#include "..\Shared\graphicsmisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

enum COLUMNS
{
	ALLOCTO_COL,
	ALLOCDAYS_COL
};

const UINT IDC_ALLOCTO_COMBO = 1001;

/////////////////////////////////////////////////////////////////////////////
// CEditAllocationsListCtrl

CEditAllocationsListCtrl::CEditAllocationsListCtrl(const WORKLOADITEM& wi, const CStringArray& aAllocTo)
{
	m_wi = wi;
	Misc::SortArray(m_wi.aAllocTo);

	m_aAllocTo.Copy(aAllocTo);
}

CEditAllocationsListCtrl::~CEditAllocationsListCtrl()
{
}


BEGIN_MESSAGE_MAP(CEditAllocationsListCtrl, CInputListCtrl)
	//{{AFX_MSG_MAP(CEditAllocationsListCtrl)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_CBN_CLOSEUP(IDC_ALLOCTO_COMBO, OnAllocationComboCancel)
	ON_CBN_SELENDCANCEL(IDC_ALLOCTO_COMBO, OnAllocationComboCancel)
	ON_CBN_SELENDOK(IDC_ALLOCTO_COMBO, OnAllocationComboOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditAllocationsListCtrl message handlers

void CEditAllocationsListCtrl::InitState()
{
	OverrideSelectionTheming(TRUE, TRUE);
	ShowGrid(TRUE, TRUE);

	CreateControl(m_cbAllocTo, IDC_ALLOCTO_COMBO);

	AddCol(CEnString(IDS_ATTRIB_ALLOCTO), GraphicsMisc::ScaleByDPIFactor(200));
	AddCol(CEnString(IDS_COL_WEEKDAYS), GraphicsMisc::ScaleByDPIFactor(75));

	SetEditMask(_T(".0123456789"), ME_LOCALIZEDECIMAL);
	SetAutoRowPrompt(CEnString(IDS_NEW_ALLOCATION));
	AutoAdd(TRUE, FALSE);

	// Build list
	CStringArray aAllocTo;
	m_wi.GetNames(m_aAllocTo, aAllocTo);

	for (int nAllocTo = 0; nAllocTo < aAllocTo.GetSize(); nAllocTo++)
	{
		const CString& sAllocTo = aAllocTo[nAllocTo];
		CString sDays = m_wi.mapAllocatedDays.FormatDays(sAllocTo, 2);

		int nRow = AddRow(sAllocTo);
		SetItemText(nRow, ALLOCDAYS_COL, sDays);
	}
}

void CEditAllocationsListCtrl::EditCell(int nItem, int nCol, BOOL bBtnClick)
{
	switch (nCol)
	{
	case ALLOCTO_COL:
		ShowControl(m_cbAllocTo, nItem, nCol);
		break;

	case ALLOCDAYS_COL:
		CInputListCtrl::EditCell(nItem, nCol, bBtnClick);
		break;
	}
}

void CEditAllocationsListCtrl::PrepareControl(CWnd& ctrl, int nRow, int nCol)
{
	UNREFERENCED_PARAMETER(ctrl);

	if (!m_aAllocTo.GetSize())
		return;

	switch (nCol)
	{
	case ALLOCTO_COL:
		{
			ASSERT (&ctrl == &m_cbAllocTo);

			// Rebuild the list leaving out any already selected
			m_cbAllocTo.ResetContent();
			CStringArray aAllocTo;

			aAllocTo.Copy(m_aAllocTo);
			Misc::RemoveItems(m_wi.aAllocTo, aAllocTo);

			int nAllocTo = aAllocTo.GetSize();

			while (nAllocTo--)
				m_cbAllocTo.AddString(aAllocTo[nAllocTo]);
			
			if (!IsPrompt(nRow))
				m_cbAllocTo.SelectString(-1, GetItemText(nRow, nCol));
		}
		break;
	}
}

void CEditAllocationsListCtrl::OnAllocationComboCancel()
{
	HideControl(m_cbAllocTo);
}

void CEditAllocationsListCtrl::OnAllocationComboOK()
{
	HideControl(m_cbAllocTo);

	int nAllocTo = m_cbAllocTo.GetCurSel();

	if (nAllocTo != CB_ERR)
	{
		CString sAllocTo;
		m_cbAllocTo.GetLBText(nAllocTo, sAllocTo);

		// handle new allocations
		int nRow = GetCurSel();

		if (IsPrompt(nRow))
			AddRow(sAllocTo);
		else
			SetItemText(nRow, ALLOCTO_COL, sAllocTo);
	}
}

const WORKLOADITEM& CEditAllocationsListCtrl::GetAllocations() const
{
	return m_wi;
}

void CEditAllocationsListCtrl::OnDestroy() 
{
	m_wi.mapAllocatedDays.RemoveAll();
	m_wi.aAllocTo.RemoveAll();

	int nNumRows = (GetItemCount() - 1);

	for (int nRow = 0; nRow < nNumRows; nRow++)
	{
		CString sAllocTo = GetItemText(nRow, ALLOCTO_COL);
		CString sDays = GetItemText(nRow, ALLOCDAYS_COL);

		// Allow for the user adding the same name more than once
		m_wi.mapAllocatedDays.AddDays(sAllocTo, sDays);
		Misc::AddUniqueItem(sAllocTo, m_wi.aAllocTo);
	}

	CInputListCtrl::OnDestroy();
}
