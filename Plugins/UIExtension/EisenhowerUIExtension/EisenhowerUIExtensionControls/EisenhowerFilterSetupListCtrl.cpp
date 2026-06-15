// EisenhowerUIExtensionControls.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include "EisenhowerFilterSetupListCtrl.h"

#include <Shared\WndPrompt.h>
#include <Shared\EnString.h>
#include <Shared\Localizer.h>
#include <Shared\Themed.h>
#include <Shared\DialogHelper.h>
#include <Shared\Misc.h>
#include <Shared\GraphicsMisc.h>
#include <Shared\DateHelper.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

using namespace EisenhowerUIExtension;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

enum // Variable ValueType
{
	VAR_UNKNOWN = -1,
	VAR_INTEGER,
	VAR_DECIMAL,
	VAR_BOOLEAN,
	VAR_TIMEPERIOD,
	VAR_DATE,
};

////////////////////////////////////////////////////////////////////////////////////////////////

FILTER::FILTER() : nXVarIndex(-1), nYVarIndex(-1)
{
}

BOOL FILTER::IsValid() const
{
	if ((nXVarIndex == -1) || (nYVarIndex == -1))
		return FALSE;

	if (nXVarIndex == nYVarIndex)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////

enum
{
	XVAR_COL,
	XCUTOFF_COL,
	YVAR_COL,
	YCUTOFF_COL,
};

const int IDC_VARIABLES		= 1001;
const int IDC_LISTCUTOFFS	= 1002;
const int IDC_DATECUTOFFS	= 1003;
const int IDC_TIMECUTOFFS	= 1004;

const UINT WM_ESLCN_EDITCHANGE = RegisterWindowMessage(L"WM_ESLCN_EDITCHANGE");

// --------------------------------

CEisenhowerSetupListCtrl::CEisenhowerSetupListCtrl()
{
}

BEGIN_MESSAGE_MAP(CEisenhowerSetupListCtrl, CInputListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY(NM_CUSTOMDRAW, 0, OnHeaderCustomDraw)
	ON_CBN_CLOSEUP(IDC_VARIABLES, OnVariableComboCloseUp)
	ON_CBN_CLOSEUP(IDC_LISTCUTOFFS, OnCutoffComboCloseUp)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATECUTOFFS, OnDateCutoffChange)
	ON_EN_KILLFOCUS(IDC_TIMECUTOFFS, OnTimeCutoffKillFocus)
END_MESSAGE_MAP()

int CEisenhowerSetupListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CInputListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateControl(m_cbCutoffs, IDC_LISTCUTOFFS, CBS_DROPDOWNLIST);
	CreateControl(m_cbVariables, IDC_VARIABLES, CBS_DROPDOWNLIST | CBS_SORT);
	CreateControl(m_dtcCutoffs, IDC_DATECUTOFFS, DTS_SHOWNONE);
	CreateControl(m_tpCutoffs, IDC_TIMECUTOFFS, ES_AUTOHSCROLL);

	// Periods are always positive days
	m_tpCutoffs.SetUnits(THU_DAYS);
	m_tpCutoffs.EnableUnitsChanges(FALSE);
	m_tpCutoffs.EnableNegativeTimes(FALSE);

	return 0;
}

void CEisenhowerSetupListCtrl::Initialise(const CArray<VARIABLE, VARIABLE&>& aVars,
										  const CArray<FILTER, FILTER&>& aFilters)
{
	// Set the row count before adding columns
	GetHeader()->SetRowCount(2);
	GetHeader()->EnableTracking(FALSE);

	CRect rClient;
	GetClientRect(rClient);

	AddCol(CEnString(IDS_VARIABLE_HEADER), (rClient.Width() / 4), ILCT_COMBO);
	AddCol(CEnString(IDS_CUTOFF_HEADER), (rClient.Width() / 4), ILCT_TEXT);
	AddCol(CEnString(IDS_VARIABLE_HEADER), (rClient.Width() / 4), ILCT_COMBO);
	AddCol(CEnString(IDS_CUTOFF_HEADER), (rClient.Width() / 4), ILCT_TEXT);

	AutoAdd(TRUE, FALSE);

	// Save these for populating the combobox
	m_aVariables.Copy(aVars);

	// Populate the rows
	m_aFilters.Copy(aFilters);

	for (int nFilter = 0; nFilter < aFilters.GetSize(); nFilter++)
		AddRow();

	SetCurSel(0, 0);
}

void CEisenhowerSetupListCtrl::InitState()
{
	CInputListCtrl::InitState();

	ShowGrid(TRUE, TRUE);
	SetAutoRowPrompt(CEnString(IDS_FILTERROW_PROMPT));
}

int CEisenhowerSetupListCtrl::AddRow()
{
	int nFilter = (GetItemCount() - 1); // item before prompt 
	ASSERT(nFilter <= m_aFilters.GetSize());

	if (nFilter == m_aFilters.GetSize())
		m_aFilters.Add(FILTER());

	const FILTER& filter = m_aFilters[nFilter];

	int nRow = CInputListCtrl::AddRow(GetVarLabel(filter.nXVarIndex));
	ASSERT(nRow == nFilter);

	UpdateCellText(nRow, XCUTOFF_COL);
	UpdateCellText(nRow, YVAR_COL);
	UpdateCellText(nRow, YCUTOFF_COL);

	return nRow;
}

int CEisenhowerSetupListCtrl::GetFilters(CArray<FILTER, FILTER&>& aFilters) const
{
	aFilters.Copy(m_aFilters);
	return aFilters.GetSize();
}

BOOL CEisenhowerSetupListCtrl::UpdateCellText(int nRow, int nCol)
{
	if (IsPrompt(nRow))
		return FALSE;

	const FILTER& filter = m_aFilters[nRow];
	CString sNewText;

	switch (nCol)
	{
	case XVAR_COL:
	case XCUTOFF_COL:
		sNewText = FormatCellText(nRow, nCol, filter.nXVarIndex, filter.sXCutoff);
		break;

	case YVAR_COL:
	case YCUTOFF_COL:
		sNewText = FormatCellText(nRow, nCol, filter.nYVarIndex, filter.sYCutoff);
		break;
	}

	if (sNewText == GetItemText(nRow, nCol))
		return FALSE;

	SetItemText(nRow, nCol, sNewText);
	return TRUE;
}

CString CEisenhowerSetupListCtrl::FormatCellText(int nRow, int nCol, int nVar, const CString& sCutoff) const
{
	const FILTER& filter = m_aFilters[nRow];

	switch (nCol)
	{
	case XVAR_COL: 
	case YVAR_COL: 
		return GetVarLabel(nVar);

	case XCUTOFF_COL:
	case YCUTOFF_COL:
		if (nVar != -1)
		{
			const VARIABLE& var = m_aVariables[nVar];

			if (var.nType == VAR_BOOLEAN)
				return CEnString(IDS_BOOLCUTOFF_PROMPT); // always\
	
			if (!CanEditCutOff(nVar))
				return L"";

			// else
			switch (var.nType)
			{
			case VAR_INTEGER:
			case VAR_DECIMAL:
				break;

			case VAR_TIMEPERIOD:
				if (!sCutoff.IsEmpty())
					return (sCutoff + ' ' + CLocalizer::TranslateText(_T("D")));
				break;

			case VAR_DATE:
				if (!sCutoff.IsEmpty())
					return CDateHelper::FormatDate(Misc::Atof(sCutoff));
				break;

			default:
				ASSERT(0);
				break;
			}
		}
		break;

	default: 
		ASSERT(0);
		break;
	}

	return (sCutoff.IsEmpty() ? CEnString(IDS_DEFCUTOFF_PROMPT) : sCutoff);
}

CString CEisenhowerSetupListCtrl::GetCellPrompt(int nItem, int nCol, const CString& sText) const
{
	if (!IsPrompt(nItem))
	{
		switch (nCol)
		{
		case XVAR_COL:
		case YVAR_COL:
			if (sText.IsEmpty())
				return CEnString(IDS_UNDEFINEDVAR_PROMPT);
			break;

		case XCUTOFF_COL:
			return GetCellPrompt(nItem, nCol, sText, m_aFilters[nItem].nXVarIndex);

		case YCUTOFF_COL:
			return GetCellPrompt(nItem, nCol, sText, m_aFilters[nItem].nYVarIndex);
		}
	}

	return L"";
}

int CEisenhowerSetupListCtrl::GetVarIndex(int nRow, int nCol) const
{
	if (!IsPrompt(nRow))
	{
		switch (nCol)
		{
		case XVAR_COL:
		case XCUTOFF_COL:
			return m_aFilters[nRow].nXVarIndex;

		case YVAR_COL:
		case YCUTOFF_COL:
			return m_aFilters[nRow].nYVarIndex;
		}
	}

	return -1;
}

CString CEisenhowerSetupListCtrl::GetCellPrompt(int nItem, int nCol, const CString& sText, int nVar) const
{
	switch (GetVarType(nVar))
	{
	case VAR_BOOLEAN:
		return CEnString(IDS_BOOLCUTOFF_PROMPT);

	default:
		if (sText.IsEmpty())
			return CEnString(IDS_DEFCUTOFF_PROMPT);
		break;
	}

	return L"";
}

int CEisenhowerSetupListCtrl::GetVarType(int nVar) const
{
	if (nVar < 0 || nVar >= m_aVariables.GetSize())
		return VAR_UNKNOWN;

	return m_aVariables[nVar].nType;
}

CString CEisenhowerSetupListCtrl::GetVarLabel(int nVar) const
{
	if (nVar < 0 || nVar >= m_aVariables.GetSize())
		return CEnString(IDS_UNDEFINEDVAR_PROMPT);

	return m_aVariables[nVar].sLabel;
}

BOOL CEisenhowerSetupListCtrl::CanEditCutOff(int nVar) const
{
	switch (GetVarType(nVar))
	{
	case VAR_UNKNOWN:
	case VAR_BOOLEAN:
		return FALSE;
	}

	return TRUE;
}

void CEisenhowerSetupListCtrl::PrepareControl(CWnd& /*ctrl*/, int nRow, int nCol)
{
	PrepareControl(nRow, nCol);
}

void CEisenhowerSetupListCtrl::PrepareControl(int nRow, int nCol)
{
	const FILTER& filter = m_aFilters[nRow];

	switch (nCol)
	{
	case XVAR_COL:
	case YVAR_COL:
		PrepareVariableCombo(nRow, nCol);
		break;

	case XCUTOFF_COL:
		PrepareCutoffControl(filter.nXVarIndex, filter.sXCutoff);
		break;

	case YCUTOFF_COL:
		PrepareCutoffControl(filter.nYVarIndex, filter.sYCutoff);
		break;
	}
}

void CEisenhowerSetupListCtrl::PrepareCutoffControl(int nVar, const CString& sCutoff)
{
	ASSERT(CanEditCutOff(nVar));

	const VARIABLE& var = m_aVariables[nVar];

	if (!var.sListData.IsEmpty())
	{
		m_cbCutoffs.ResetContent();

		CStringArray aCutoffs;
		Misc::Split(var.sListData, aCutoffs, '\n');

		CDialogHelper::SetComboBoxItems(m_cbCutoffs, aCutoffs);
		m_cbCutoffs.SelectString(-1, sCutoff);

		return;
	}

	switch (var.nType)
	{
	case VAR_INTEGER:
		CInputListCtrl::GetEditControl()->SetMask(L"0123456789");
		break;

	case VAR_DECIMAL:
		CInputListCtrl::GetEditControl()->SetMask(L"0123456789.", ME_LOCALIZEDECIMAL);
		break;

	case VAR_TIMEPERIOD:
		m_tpCutoffs.SetTime(Misc::Atof(sCutoff), THU_DAYS);
		break;

	case VAR_DATE:
		if (sCutoff.IsEmpty())
			m_dtcCutoffs.SendMessage(DTM_SETSYSTEMTIME, GDT_NONE);
		else
			m_dtcCutoffs.SetTime(Misc::Atof(sCutoff));
		break;

	case VAR_BOOLEAN:
	default:
		ASSERT(0);
		break;
	}
}

BOOL CEisenhowerSetupListCtrl::CanEditCell(int nRow, int nCol) const
{
	if (!IsPrompt(nRow))
	{
		const FILTER& filter = m_aFilters[nRow];

		switch (nCol)
		{
		case XVAR_COL:
		case YVAR_COL:
			return TRUE;

		case XCUTOFF_COL:
			return CanEditCutOff(filter.nXVarIndex);

		case YCUTOFF_COL:
			return CanEditCutOff(filter.nYVarIndex);
		}
	}

	// All else
	return CInputListCtrl::CanEditCell(nRow, nCol);
}

void CEisenhowerSetupListCtrl::EditCell(int nItem, int nCol, BOOL bBtnClick)
{
	switch (nCol)
	{
	case XVAR_COL:
		if (IsPrompt(nItem))
			SetCurSel(AddRow(), XVAR_COL);

		ShowControl(m_cbVariables, nItem, nCol, bBtnClick);
		break;

	case YVAR_COL:
		ShowControl(m_cbVariables, nItem, nCol, bBtnClick);
		break;

	case XCUTOFF_COL:
		EditCutoffCell(nItem, nCol, m_aFilters[nItem].nXVarIndex, bBtnClick);
		break;

	case YCUTOFF_COL:
		EditCutoffCell(nItem, nCol, m_aFilters[nItem].nYVarIndex, bBtnClick);
		break;

	default:
		ASSERT(0);
		break;
	}
}

void CEisenhowerSetupListCtrl::EditCutoffCell(int nItem, int nCol, int nVar, BOOL bBtnClick)
{
	const VARIABLE& var = m_aVariables[nVar];

	if (!var.sListData.IsEmpty())
	{
		ShowControl(m_cbCutoffs, nItem, nCol, bBtnClick);
		return;
	}

	switch (var.nType)
	{
	case VAR_INTEGER:
		CInputListCtrl::GetEditControl()->SetMask(L"0123456789");
		break;

	case VAR_DECIMAL:
		CInputListCtrl::GetEditControl()->SetMask(L"0123456789.", ME_LOCALIZEDECIMAL);
		break;

	case VAR_TIMEPERIOD:
		ShowControl(m_tpCutoffs, nItem, nCol, bBtnClick);
		return;

	case VAR_DATE:
		ShowControl(m_dtcCutoffs, nItem, nCol, bBtnClick);
		return;

	case VAR_BOOLEAN:
	default:
		ASSERT(0);
		break;
	}

	// default
	CInputListCtrl::EditCell(nItem, nCol, bBtnClick);
}

IL_COLUMNTYPE CEisenhowerSetupListCtrl::GetCellType(int nRow, int nCol) const
{
	switch (nCol)
	{
	case XVAR_COL:
	case YVAR_COL:
		return ILCT_COMBO;

	case XCUTOFF_COL:
		if (!IsPrompt(nRow))
			return GetCutoffCellType(nRow, nCol, m_aFilters[nRow].nXVarIndex);
		break;

	case YCUTOFF_COL:
		if (!IsPrompt(nRow))
			return GetCutoffCellType(nRow, nCol, m_aFilters[nRow].nYVarIndex);
		break;

	default:
		ASSERT(0);
		break;
	}

	return CInputListCtrl::GetCellType(nRow, nCol);
}

IL_COLUMNTYPE CEisenhowerSetupListCtrl::GetCutoffCellType(int nRow, int nCol, int nVar) const
{
	if (nVar != -1)
	{
		const VARIABLE& var = m_aVariables[nVar];

		if (!var.sListData.IsEmpty())
			return ILCT_COMBO;

		switch (var.nType)
		{
		case VAR_DATE:
			return ILCT_DATE;

		case VAR_TIMEPERIOD:
			// Because we ALWAYS work in DAYS it would be
			// misleading to show the drop-menu arrow so
			// we always show time periods as text even though
			// we use a CTimeEdit for the editing
			break;

		case VAR_INTEGER:
		case VAR_DECIMAL:
		case VAR_BOOLEAN:
			break;

		default:
			ASSERT(0);
			break;
		}
	}

	return CInputListCtrl::GetCellType(nRow, nCol);
}

BOOL CEisenhowerSetupListCtrl::DeleteSelectedCell()
{
	if (!CInputListCtrl::DeleteSelectedCell())
		return FALSE;

	// Synchronise underlying filter array
	int nRow = GetCurSel();
	FILTER& filter = m_aFilters[nRow];

	switch (m_nCurCol)
	{
		case XVAR_COL:
			m_aFilters.RemoveAt(nRow);
			break;

		case YVAR_COL:
			filter.nYVarIndex = -1;
			break;

		case XCUTOFF_COL:
			filter.sXCutoff.Empty();
			break;

		case YCUTOFF_COL:
			filter.sYCutoff.Empty();
			break;
	}

	if (UpdateCellText(nRow, m_nCurCol))
		NotifyEditChange();

	return TRUE;
}

COLORREF CEisenhowerSetupListCtrl::GetItemTextColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	if (!IsPrompt(nItem))
	{
		const FILTER& filter = m_aFilters[nItem];
		BOOL bHasValue = FALSE;

		switch (nCol)
		{
		case XVAR_COL:
			bHasValue = (filter.nXVarIndex != -1);
			break;

		case YVAR_COL:
			bHasValue = (filter.nYVarIndex != -1);
			break;

		case XCUTOFF_COL:
			bHasValue = !(filter.sXCutoff.IsEmpty() || (GetVarType(filter.nXVarIndex) == VAR_BOOLEAN));
			break;

		case YCUTOFF_COL:
			bHasValue = !(filter.sYCutoff.IsEmpty() || (GetVarType(filter.nYVarIndex) == VAR_BOOLEAN));
			break;

		default:
			ASSERT(0);
			break;
		}

		if (!bHasValue)
			return CWndPrompt::GetTextColor();
	}

	// else
	return CInputListCtrl::GetItemTextColor(nItem, nCol, bSelected, bDropHighlighted, bWndFocus);
}

COLORREF CEisenhowerSetupListCtrl::GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	if (!bSelected && !bDropHighlighted && !IsPrompt(nItem) && !CanEditCell(nItem, nCol))
		return GetSysColor(COLOR_3DFACE);

	// else
	return CInputListCtrl::GetItemBackColor(nItem, nCol, bSelected, bDropHighlighted, bWndFocus);
}

void CEisenhowerSetupListCtrl::PrepareVariableCombo(int nRow, int nCol)
{
	int nFilter = (IsPrompt(nRow) ? -1 : nRow);
	int nVarExclude = -1, nSelVar = -1;

	// Exclude whichever variable is selected in the 'other' variable cell
	if ((nFilter != -1) && (nFilter < m_aFilters.GetSize()))
	{
		const FILTER& filter = m_aFilters[nFilter];

		switch (nCol)
		{
		case XVAR_COL:
			nSelVar = filter.nXVarIndex;
			nVarExclude = filter.nYVarIndex;
			break;

		case YVAR_COL:
			nSelVar = filter.nYVarIndex;
			nVarExclude = filter.nXVarIndex;
			break;

		default:
			return;
		}
	}
	
	m_cbVariables.ResetContent();

	for (int nVar = 0; nVar < m_aVariables.GetSize(); nVar++)
	{
		if (nVar == nVarExclude)
			continue;

		int nItem = m_cbVariables.AddString(m_aVariables[nVar].sLabel);
		m_cbVariables.SetItemData(nItem, nVar); // Because the listed is sorted
	}

	m_cbVariables.SelectString(-1, GetVarLabel(nSelVar));
}

void CEisenhowerSetupListCtrl::OnVariableComboCloseUp()
{
	int nRow, nCol;
	
	if (GetCurSel(nRow, nCol))
	{
		FILTER& filter = m_aFilters[nRow];

		int nSelVar = m_cbVariables.GetCurSel();
		int nVar = (int)m_cbVariables.GetItemData(nSelVar);

		switch (nCol)
		{
		case XVAR_COL:
			filter.nXVarIndex = nVar;
			break;

		case YVAR_COL:
			filter.nYVarIndex = nVar;
			break;

		default:
			ASSERT(0);
			break;
		}

		if (UpdateCellText(nRow, nCol))
			NotifyEditChange();
	}

	HideControl(m_cbVariables);
}

void CEisenhowerSetupListCtrl::OnCutoffComboCloseUp()
{
	SetSelectedFilterCutoff(CDialogHelper::GetSelectedItem(m_cbCutoffs));
	HideControl(m_cbCutoffs);
}

void CEisenhowerSetupListCtrl::OnDateCutoffChange(NMHDR* pNMHDR, LPARAM* lResult)
{
	NMDATETIMECHANGE* pNMDTC = (NMDATETIMECHANGE*)pNMHDR;
	COleDateTime date;

	if ((pNMDTC->dwFlags == GDT_NONE) || !m_dtcCutoffs.GetTime(date))
		SetSelectedFilterCutoff(L"");
	else
		SetSelectedFilterCutoff(Misc::Format((int)date.m_dt));

	HideControl(m_dtcCutoffs);
}

void CEisenhowerSetupListCtrl::OnTimeCutoffKillFocus()
{
	CString sText;
	m_tpCutoffs.GetWindowText(sText);

	SetSelectedFilterCutoff(sText);
	HideControl(m_tpCutoffs);
}

void CEisenhowerSetupListCtrl::OnEndEdit(UINT uIDCtrl, int* pResult)
{
	CString sText;
	CInputListCtrl::GetEditControl()->GetWindowText(sText);

	SetSelectedFilterCutoff(sText);
}

void CEisenhowerSetupListCtrl::OnCancelEdit()
{
	// Reinitialise the ctrls with their original values to revert changes
	int nRow, nCol;
	
	if (GetCurSel(nRow, nCol))
		PrepareControl(nRow, nCol);

	CInputListCtrl::OnCancelEdit();
}

BOOL CEisenhowerSetupListCtrl::SetSelectedFilterCutoff(const CString& sCutoff)
{
	int nRow, nCol;

	if (!GetCurSel(nRow, nCol))
	{
		ASSERT(0);
		return FALSE;
	}

	FILTER& filter = m_aFilters[nRow];

	switch (nCol)
	{
	case XCUTOFF_COL:
		{
			if (sCutoff == filter.sXCutoff)
				return FALSE;

			// else
			filter.sXCutoff = sCutoff;
		}
		break;

	case YCUTOFF_COL:
		{
			if (sCutoff == filter.sYCutoff)
				return FALSE;

			// else
			filter.sYCutoff = sCutoff;
		}
		break;

	default:
		ASSERT(0);
		return FALSE;
	}

	VERIFY(UpdateCellText(nRow, nCol));
	NotifyEditChange();
	
	return TRUE;
}

void CEisenhowerSetupListCtrl::NotifyEditChange()
{
	GetParent()->SendMessage(WM_ESLCN_EDITCHANGE);
}

void CEisenhowerSetupListCtrl::OnHeaderCustomDraw(NMHDR* pNMHDR, LPARAM* lResult)
{
	NMCUSTOMDRAW* pNMCD = (NMCUSTOMDRAW*)pNMHDR;
	*lResult = CDRF_DODEFAULT;

	if (m_header.GetItemCount())
	{
		switch (pNMCD->dwDrawStage)
		{
		case CDDS_PREPAINT:
			*lResult = CDRF_NOTIFYITEMDRAW;
			break;

		case CDDS_ITEMPREPAINT:
			{
				// For each column draw its own 'lower' text and 
				// its portion of the double-width 'upper' text
				// because I can find no way around the clipping
				// rectangle assigned to each notification
				int nItem = (int)pNMCD->dwItemSpec;
				CDC* pDC = CDC::FromHandle(pNMCD->hdc);

				// Lower text
				CRect rDraw(pNMCD->rc);
				rDraw.top += (rDraw.Height() / 2);

				DrawHeaderRect(pDC, rDraw, m_header.GetItemText(nItem), FALSE);

				// Upper text
				switch (nItem)
				{
				case XVAR_COL:
				case XCUTOFF_COL:
					{
						m_header.GetItemRect(XVAR_COL, rDraw);

						rDraw.right += m_header.GetItemWidth(XCUTOFF_COL);
						rDraw.bottom -= (rDraw.Height() / 2);

						DrawHeaderRect(pDC, rDraw, CEnString(IDS_URGENCY_HEADER), TRUE);
					}
					break;

				case YVAR_COL:
				case YCUTOFF_COL:
					{
						m_header.GetItemRect(YVAR_COL, rDraw);

						rDraw.right += m_header.GetItemWidth(YCUTOFF_COL);
						rDraw.bottom -= (rDraw.Height() / 2);

						DrawHeaderRect(pDC, rDraw, CEnString(IDS_IMPORTANCE_HEADER), TRUE);
					}
					break;

				default:
					ASSERT(0);
				}
			}
			*lResult = CDRF_SKIPDEFAULT;
			break;
		}
	}
}

void CEisenhowerSetupListCtrl::DrawHeaderRect(CDC* pDC, const CRect& rItem, const CString& sItem, BOOL bBold)
{
	if (CThemed::AreControlsThemed())
	{
		CThemed th;
		VERIFY(th.Open(&m_header, _T("HEADER")));

		th.DrawBackground(pDC, HP_HEADERITEM, HIS_NORMAL, rItem);
	}
	else
	{
		pDC->FillSolidRect(rItem, ::GetSysColor(COLOR_3DFACE));
		pDC->Draw3dRect(rItem, ::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DSHADOW));
	}

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(GetSysColor(COLOR_BTNTEXT));

	UINT nFlags = (DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_CENTER);

	if (bBold)
	{
		HFONT hFontBold = GraphicsMisc::CreateFont((HFONT)GetFont()->GetSafeHandle(), GMFS_BOLD);
		HGDIOBJ hFontOld = pDC->SelectObject(hFontBold);

		pDC->DrawText(sItem, (LPRECT)(LPCRECT)rItem, nFlags);

		pDC->SelectObject(hFontOld);
		GraphicsMisc::VerifyDeleteObject(hFontBold);
	}
	else
	{
		pDC->DrawText(sItem, (LPRECT)(LPCRECT)rItem, nFlags);
	}
}

BOOL CEisenhowerSetupListCtrl::PreTranslateMessage(MSG* pMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return CInputListCtrl::PreTranslateMessage(pMsg);
}

void CEisenhowerSetupListCtrl::HideAllControls(const CWnd* pWndIgnore)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CInputListCtrl::HideControl(m_cbVariables);
	CInputListCtrl::HideControl(m_cbCutoffs);
	CInputListCtrl::HideControl(m_dtcCutoffs);
	CInputListCtrl::HideControl(m_tpCutoffs);
}


////////////////////////////////////////////////////////////////////////////////////////////////

HWND GetHwnd(IntPtr hWnd)
{
	return static_cast<HWND>(hWnd.ToPointer());
}

HFONT GetHfont(IntPtr hFont)
{
	return static_cast<HFONT>(hFont.ToPointer());
}

// ---------------------------------------------

HostedEisenhowerSetupListCtrl::HostedEisenhowerSetupListCtrl(HWND hwndParent)
{
	VERIFY(m_WndOfManagedHandle.Attach(hwndParent));
}

HostedEisenhowerSetupListCtrl* HostedEisenhowerSetupListCtrl::Attach(HWND hwndParent, HFONT hFont)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HostedEisenhowerSetupListCtrl* pCtrl = new HostedEisenhowerSetupListCtrl(hwndParent);

	// Create control to fill the entire managed client area
	CRect rClient;
	pCtrl->m_WndOfManagedHandle.GetClientRect(rClient);

	DWORD dwFlags = WS_CHILD | WS_VISIBLE | WS_VSCROLL | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED | LVS_NOSORTHEADER;

	if (!pCtrl->m_WndOfManagedHandle.IsWindowEnabled())
		dwFlags |= WS_DISABLED;

	pCtrl->m_ListCtrl.Create(dwFlags, rClient, &(pCtrl->m_WndOfManagedHandle), 101);
	pCtrl->m_ListCtrl.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	pCtrl->m_ListCtrl.SendMessage(WM_SETFONT, (WPARAM)hFont, 0);

	return pCtrl;
}

void HostedEisenhowerSetupListCtrl::Detach()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_WndOfManagedHandle.Detach();
	m_ListCtrl.DestroyWindow();

	delete this;
}

void HostedEisenhowerSetupListCtrl::DrawItem(WPARAM wp, LPARAM lp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_ListCtrl.SendMessage(WM_DRAWITEM, wp, lp);
}

void HostedEisenhowerSetupListCtrl::SetFocus()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_ListCtrl.SetFocus();
}

bool HostedEisenhowerSetupListCtrl::PreTranslateMessage(MSG* pMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (m_ListCtrl.PreTranslateMessage(pMsg) != FALSE);
}

void HostedEisenhowerSetupListCtrl::UpdateSize()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CRect rSlider;
	m_WndOfManagedHandle.GetClientRect(rSlider);

	m_ListCtrl.MoveWindow(rSlider);
}

void HostedEisenhowerSetupListCtrl::Initialise(ITransText* pTrans,
											   const CArray<VARIABLE, VARIABLE&>& aVars,
											   const CArray<FILTER, FILTER&>& aFilters)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (pTrans && !CLocalizer::IsInitialized())
		CLocalizer::Initialize(pTrans);

	m_ListCtrl.Initialise(aVars, aFilters);
}

int HostedEisenhowerSetupListCtrl::GetFilters(CArray<FILTER, FILTER&>& aFilters)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_ListCtrl.GetFilters(aFilters);
}

////////////////////////////////////////////////////////////////////////////////////////////////

HostedEisenhowerSetupListCtrl* ListCtrl(IntPtr ptr)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	return (HostedEisenhowerSetupListCtrl*)ptr.ToPointer();
}

// ------------------------------------------------

EisenhowerFilterSetupListCtrl::EisenhowerFilterSetupListCtrl()
	: 
	m_Trans(nullptr),
	m_Filters(nullptr),
	m_Vars(nullptr)
{
} 

void EisenhowerFilterSetupListCtrl::Initialise(Translator^ trans,
											   EisenhowerVariables^ supportedVars,
											   EisenhowerFilters^ filters)
{
	m_Trans = trans;
	m_Vars = supportedVars;
	m_Filters = filters;

	CheckInitListCtrl();
}

EisenhowerFilters^ EisenhowerFilterSetupListCtrl::Filters::get()
{
	CheckUpdateFilters();

	return m_Filters;
}

void EisenhowerFilterSetupListCtrl::CheckUpdateFilters()
{
	// Build on demand only
	if (m_Filters == nullptr)
	{
		// Get modified filters
		m_Filters = gcnew EisenhowerFilters();

		CArray<FILTER, FILTER&> aFilters;
		int numFilters = ListCtrl(m_pMFCInfo)->GetFilters(aFilters);

		for (int f = 0; f < numFilters; f++)
		{
			const FILTER& filter = aFilters[f];

			auto ef = gcnew EisenhowerFilter();

			if (filter.nXVarIndex != -1)
				ef->XVariable = m_Vars[filter.nXVarIndex];

			if (filter.nYVarIndex != -1)
				ef->YVariable = m_Vars[filter.nYVarIndex];

			ef->XCutoff = gcnew String(filter.sXCutoff);
			ef->YCutoff = gcnew String(filter.sYCutoff);

			m_Filters->Add(ef);
		}
	}
}

// ------------------------------------------------

void EisenhowerFilterSetupListCtrl::OnHandleCreated(EventArgs^ e)
{
	Control::OnHandleCreated(e);

	m_pMFCInfo = IntPtr(HostedEisenhowerSetupListCtrl::Attach(GetHwnd(Handle), GetHfont(Font->ToHfont())));
	CheckInitListCtrl();
}

void EisenhowerFilterSetupListCtrl::OnHandleDestroyed(EventArgs^ e)
{
	if (m_pMFCInfo != IntPtr::Zero)
	{
		CheckUpdateFilters();

		// Clean up
		ListCtrl(m_pMFCInfo)->Detach();
		m_pMFCInfo = IntPtr::Zero;
	}

	Control::OnHandleDestroyed(e);
}

void EisenhowerFilterSetupListCtrl::OnSizeChanged(EventArgs^ e)
{
	Control::OnSizeChanged(e);

	if (m_pMFCInfo != IntPtr::Zero)
		ListCtrl(m_pMFCInfo)->UpdateSize();
}

bool EisenhowerFilterSetupListCtrl::PreProcessMessage(Message% m)
{
	if (m_pMFCInfo != IntPtr::Zero)
	{
		MSG msg = 
		{
			GetHwnd(m.HWnd),
			(UINT)m.Msg,
			(WPARAM)m.WParam.ToInt64(),
			(LPARAM)m.LParam.ToInt64(),
			0,
			0
		};

		if (ListCtrl(m_pMFCInfo)->PreTranslateMessage(&msg))
			return true;
	}

	return Control::PreProcessMessage(m);
}

void EisenhowerFilterSetupListCtrl::WndProc(Message% m)
{
	Control::WndProc(m);

	if (m_pMFCInfo == IntPtr::Zero)
		return;

	// C# case statements must be const
	if (m.Msg == WM_ESLCN_EDITCHANGE)
	{
		m_Filters = nullptr;
		ChangeEvent(this, gcnew EventArgs());

		return;
	}

	switch (m.Msg)
	{
	case WM_DRAWITEM:
		ListCtrl(m_pMFCInfo)->DrawItem(m.WParam.ToInt32(), m.LParam.ToInt32());
		break;

	case WM_SETFOCUS:
		ListCtrl(m_pMFCInfo)->SetFocus();
		break;

	case WM_GETDLGCODE:
		m.Result = IntPtr(DLGC_WANTARROWS);
		break;
	}
}

void EisenhowerFilterSetupListCtrl::CheckInitListCtrl()
{
	if ((m_pMFCInfo != IntPtr::Zero) &&
		(m_Vars != nullptr)	&&
		(m_Filters != nullptr))
	{
		// Convert managed arrays to MFC equivalents
		CArray<VARIABLE, VARIABLE&> aVars;

		for each (EisenhowerVariable^ ev in m_Vars)
		{
			VARIABLE var;

			var.sLabel = MarshalledString(ev->Attribute->Label);
			var.nType = (int)ev->Type;
			var.sListData = MarshalledString(ev->ListData);

			if (ev->Attribute->IsCustom())
				var.sAttribID = MarshalledString(ev->Attribute->CustomAttributeId);
			else
				var.sAttribID = MarshalledString(ev->Attribute->AttributeId.ToString());

			aVars.Add(var);
		}

		CArray<FILTER, FILTER&> aFilters;

		for each (EisenhowerFilter^ ef in m_Filters)
		{
			FILTER filter;

			if (ef->XVariable == nullptr)
				filter.nXVarIndex = -1;
			else
				filter.nXVarIndex = m_Vars->IndexOf(ef->XVariable);

			if (ef->YVariable == nullptr)
				filter.nYVarIndex = -1;
			else
				filter.nYVarIndex = m_Vars->IndexOf(ef->YVariable);

			filter.sXCutoff = MarshalledString(ef->XCutoff);
			filter.sYCutoff = MarshalledString(ef->YCutoff);

			aFilters.Add(filter);
		}

		ListCtrl(m_pMFCInfo)->Initialise(m_Trans->GetITransText(),
										 aVars, 
										 aFilters);
	}
}
