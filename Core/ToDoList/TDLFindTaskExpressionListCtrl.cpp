// FindTaskExpressionListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLFindTaskExpressionListCtrl.h"
#include "tdcstatic.h"
#include "TDLTaskIconDlg.h"

#include "..\shared\HoldRedraw.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\localizer.h"
#include "..\shared\GraphicsMisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

enum 
{ 
	ATTRIB_COL, 
	OPERATOR_COL, 
	VALUE_COL, 
	ANDOR_COL, 

	NUM_COLS
};

/////////////////////////////////////////////////////////////////////////////

enum 
{ 
	ATTRIB_ID = 5000, 
	OPERATOR_ID, 
	ANDOR_ID, 
	DATE_ID, 
	TIME_ID, 
	LISTVALUES_ID,
	PRIORITY_ID,
	RISK_ID,
	CUSTOMICON_ID,
	RECURRENCE_ID,
};

/////////////////////////////////////////////////////////////////////////////

const int IMAGE_SIZE = GraphicsMisc::ScaleByDPIFactor(16);

/////////////////////////////////////////////////////////////////////////////

const float COL_PROPORTIONS[] = { 15.0f/47, 13.0f/47, 13.0f/47, 6.0f/47 };

/////////////////////////////////////////////////////////////////////////////

const UINT RELATIVEDATEPLACEHOLDER_BTNID = 1;

/////////////////////////////////////////////////////////////////////////////
// CTDLFindTaskExpressionListCtrl

CTDLFindTaskExpressionListCtrl::CTDLFindTaskExpressionListCtrl()
	:
	m_dtcDate(MCS_WEEKNUMBERS),
	m_cbPriority(FALSE),
	m_cbRisk(FALSE),
	m_cbCustomIcons(m_ilIcons, TRUE, FALSE),
	m_cbRecurrence(FALSE)
{
}

CTDLFindTaskExpressionListCtrl::~CTDLFindTaskExpressionListCtrl()
{
}

BEGIN_MESSAGE_MAP(CTDLFindTaskExpressionListCtrl, CInputListCtrl)
	//{{AFX_MSG_MAP(CTDLFindTaskExpressionListCtrl)
	ON_WM_KILLFOCUS()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_WM_CHAR()
	ON_CBN_CLOSEUP(ATTRIB_ID, OnAttribEditCancel)
	ON_CBN_SELENDCANCEL(ATTRIB_ID, OnAttribEditCancel)
	ON_CBN_SELENDOK(ATTRIB_ID, OnAttribEditOK)
	ON_CBN_SELENDCANCEL(OPERATOR_ID, OnOperatorEditCancel)
	ON_CBN_SELENDOK(OPERATOR_ID, OnOperatorEditOK)
	ON_CBN_SELENDCANCEL(ANDOR_ID, OnAndOrEditCancel)
	ON_CBN_SELENDOK(ANDOR_ID, OnAndOrEditOK)
	ON_CBN_SELENDCANCEL(LISTVALUES_ID, OnListValuesEditCancel)
	ON_CBN_SELENDOK(LISTVALUES_ID, OnListValuesEditOK)
	ON_CBN_SELCHANGE(LISTVALUES_ID, OnListValuesEditOK)
	ON_CBN_SELENDCANCEL(PRIORITY_ID, OnPriorityEditCancel)
	ON_CBN_SELENDOK(PRIORITY_ID, OnPriorityEditOK)
	ON_CBN_SELENDCANCEL(RISK_ID, OnRiskEditCancel)
	ON_CBN_SELENDOK(RISK_ID, OnRiskEditOK)
	ON_CBN_SELENDCANCEL(RECURRENCE_ID, OnRecurrenceEditCancel)
	ON_CBN_SELENDOK(RECURRENCE_ID, OnRecurrenceEditOK)
	ON_CBN_SELCHANGE(CUSTOMICON_ID, OnCustomIconEditChange)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnValueEditOK)
	ON_NOTIFY_REFLECT_EX(LVN_ITEMCHANGED, OnSelItemChanged)
	ON_NOTIFY(DTN_DATETIMECHANGE, DATE_ID, OnDateChange)
	ON_NOTIFY(DTN_CLOSEUP, DATE_ID, OnDateCloseUp)
	ON_EN_CHANGE(TIME_ID, OnTimeChange)
	ON_REGISTERED_MESSAGE(WM_TEN_UNITSCHANGE, OnTimeUnitsChange)
	ON_REGISTERED_MESSAGE(WM_EE_BTNCLICK, OnEEBtnClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLFindTaskExpressionListCtrl message handlers

void CTDLFindTaskExpressionListCtrl::PreSubclassWindow() 
{
	CInputListCtrl::PreSubclassWindow();

	m_header.EnableTracking(FALSE);
	ShowGrid(TRUE, TRUE);

	AddCol(CEnString(IDS_FT_ATTRIB), 120);
	AddCol(CEnString(IDS_FT_MATCHES), 160, ILCT_DROPLIST);
	AddCol(CEnString(IDS_FT_VALUE), 130, ILCT_TEXT); // overridden in GetCellType()
	AddCol(CEnString(IDS_FT_ANDOR), 60, ILCT_DROPLIST);
	SetView(LVS_REPORT);

	AutoAdd(TRUE, FALSE);

	BuildListCtrl();

	// create child controls last because they will
	// get used to update the minimum item height
	CreateControl(m_cbAttributes, ATTRIB_ID);
	CreateControl(m_cbOperators, OPERATOR_ID, CBS_DROPDOWNLIST); // no sort
	CreateControl(m_cbAndOr, ANDOR_ID, FALSE);
	CreateControl(m_dtcDate, DATE_ID);
	CreateControl(m_eTime, TIME_ID);
	CreateControl(m_cbListValues, LISTVALUES_ID);
	CreateControl(m_cbPriority, PRIORITY_ID, CBS_DROPDOWNLIST); // no sort
	CreateControl(m_cbRisk, RISK_ID, CBS_DROPDOWNLIST); // no sort
	CreateControl(m_cbCustomIcons, CUSTOMICON_ID, CBS_DROPDOWNLIST); // no sort
	CreateControl(m_cbRecurrence, RECURRENCE_ID, CBS_DROPDOWNLIST); // no sort

	// build and/or combo too
	int nItem = m_cbAndOr.AddString(CEnString(IDS_FP_AND));
	m_cbAndOr.SetItemData(nItem, TRUE);
				
	nItem = m_cbAndOr.AddString(CEnString(IDS_FP_OR));
	m_cbAndOr.SetItemData(nItem, FALSE);

	// Not sure why this doesn't work 'out of the box'
	// but we need to set the style manually
	m_dtcDate.SetMonthCalStyle(MCS_WEEKNUMBERS);
}

void CTDLFindTaskExpressionListCtrl::SetCustomAttributes(const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	m_aAttribDefs.Copy(aAttribDefs);
	m_cbAttributes.SetCustomAttributes(m_aAttribDefs);
}

void CTDLFindTaskExpressionListCtrl::SetAttributeListData(const TDCAUTOLISTDATA& tld, TDC_ATTRIBUTE nAttribID)
{
	m_tldListContents.Copy(tld, nAttribID);
}

void CTDLFindTaskExpressionListCtrl::SetActiveTasklist(const CString& sTasklist, BOOL bWantDefaultIcons)
{
	if (!m_ilIcons.LoadImages(sTasklist) && (bWantDefaultIcons || sTasklist.IsEmpty()))
		m_ilIcons.LoadDefaultImages();
}

void CTDLFindTaskExpressionListCtrl::SetSearchParams(const SEARCHPARAM& param)
{
	m_aSearchParams.RemoveAll();
	m_aSearchParams.Add(SEARCHPARAM(param));

	if (GetSafeHwnd())
		BuildListCtrl();
}

void CTDLFindTaskExpressionListCtrl::ClearSearch()
{
	m_aSearchParams.RemoveAll();

	if (GetSafeHwnd())
		BuildListCtrl();
}

void CTDLFindTaskExpressionListCtrl::SetSearchParams(const CSearchParamArray& params)
{
	m_aSearchParams.Copy(params);

	if (GetSafeHwnd())
		BuildListCtrl();
}

int CTDLFindTaskExpressionListCtrl::GetSearchParams(CSearchParamArray& params) const
{
	params.Copy(m_aSearchParams);

	return params.GetSize();
}

BOOL CTDLFindTaskExpressionListCtrl::DeleteSelectedCell()
{
	int nRow, nCol;
	GetCurSel(nRow, nCol);

	if (nRow < GetRuleCount())
	{
		if (nCol == ATTRIB_COL)
		{
			CInputListCtrl::DeleteSelectedCell();
			m_aSearchParams.RemoveAt(nRow);

			ValidateListData();
			SetCurSel(nRow);

			return TRUE;
		}
		else if (nCol == VALUE_COL) // clear text
		{
			SetItemText(nRow, nCol, _T(""));
			m_aSearchParams[nRow].ClearValue();
		}
	}
	
	// else
	return FALSE;
}

CWnd* CTDLFindTaskExpressionListCtrl::GetEditControl(int nItem, int nCol)
{
	if (nItem < 0 || nItem > GetRuleCount() || nCol > ANDOR_COL)
		return NULL;

	const SEARCHPARAM& rule = m_aSearchParams[nItem];

	switch (nCol)
	{
	case ATTRIB_COL:
		return &m_cbAttributes;

	case OPERATOR_COL:
		if (!rule.AttributeIs(TDCA_NONE))
		{
			return &m_cbOperators;
		}
		break;

	case VALUE_COL:
		if (rule.OperatorIs(FOP_SET) || rule.OperatorIs(FOP_NOT_SET))
		{
			// handled by operator
		}
		else
		{
			FIND_ATTRIBTYPE nType = rule.GetAttribType();
			TDC_ATTRIBUTE nAttribID = rule.GetAttribute();

			switch (nType)
			{
			case FT_DATE:
				return &m_dtcDate;

			case FT_TIMEPERIOD:
				return &m_eTime;

			case FT_BOOL:
				// do nothing: it's handled by the operator
				break;

			case FT_NONE:
				// do nothing.
				break;

			default:
				switch (nAttribID)
				{
				case TDCA_ALLOCTO:
				case TDCA_ALLOCBY:
				case TDCA_STATUS:
				case TDCA_CATEGORY:
				case TDCA_VERSION:
				case TDCA_TAGS: 
					return &m_cbListValues;

				case TDCA_PRIORITY:
					return &m_cbPriority;

				case TDCA_RISK:
					return &m_cbRisk;

				case TDCA_RECURRENCE:
					return &m_cbRecurrence;

				default:
					if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
					{
						const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
						GET_DEF_ALT(m_aAttribDefs, nAttribID, pDef, break);

						if (pDef->IsList())
						{
							if (pDef->IsDataType(TDCCA_ICON))
								return &m_cbCustomIcons;
							else
								return &m_cbListValues;
						}
					}
					break;
				}

				// all else
				return CInputListCtrl::GetEditControl();
			}
		}
		break;

	case ANDOR_COL:
		return &m_cbAndOr;
	}

	// all else
	return NULL;
}

void CTDLFindTaskExpressionListCtrl::EditCell(int nItem, int nCol, BOOL bBtnClick)
{
	if (!CanEditCell(nItem, nCol))
		return;

	// handle new rules
	if (nItem == GetRuleCount() && nCol == ATTRIB_COL)
		AddRule();

	CWnd* pEdit = GetEditControl(nItem, nCol);
	ASSERT (pEdit);

	if (!pEdit)
		return;

	SEARCHPARAM& rule = m_aSearchParams[nItem];

	switch (nCol)
	{
	case ATTRIB_COL:
	case OPERATOR_COL:
	case ANDOR_COL:
		ShowControl(*pEdit, nItem, nCol);
		break;

	case VALUE_COL:
		if ((rule.GetOperator() != FOP_SET) && (rule.GetOperator() != FOP_NOT_SET))
		{
			switch (rule.GetAttribType())
			{
			case FT_DATE:
			case FT_TIMEPERIOD:
			case FT_RECURRENCE:
				ShowControl(*pEdit, nItem, nCol);
				break;

			case FT_DATERELATIVE:
				PrepareEdit(nItem, nCol);
				CInputListCtrl::EditCell(nItem, nCol, bBtnClick);

				if (bBtnClick)
					OnEEBtnClick(m_editBox.GetDlgCtrlID(), RELATIVEDATEPLACEHOLDER_BTNID);
				break;

			case FT_ICON:
				{
					BOOL bBrowse = (rule.GetAttribute() == TDCA_ICON); // else custom attribute

					if (!bBrowse)
					{
						const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
						GET_DEF_ALT(m_aAttribDefs, rule.GetAttribute(), pDef, return);

						bBrowse = !pDef->IsList();
					}
					
					if (bBrowse)
					{
						CTDLTaskIconDlg dialog(m_ilIcons, rule.ValueAsString());

						if (dialog.DoModal() == IDOK)
						{
							rule.SetValue(dialog.GetIconName());
							UpdateValueColumnText(nItem);
						}
					}
					else // combo
					{
						ShowControl(*pEdit, nItem, nCol);
						break;		
					}
				}
				break;

			default:
				switch (rule.GetAttribute())
				{
				case TDCA_ICON:
					{
					}
					break;

				default:
					PrepareEdit(nItem, nCol);

					if (pEdit == &m_editBox)
						CInputListCtrl::EditCell(nItem, nCol, bBtnClick);
					else
						ShowControl(*pEdit, nItem, nCol);
					break;
				}
			}
		}
		break;
	}
}

IL_COLUMNTYPE CTDLFindTaskExpressionListCtrl::GetCellType(int nRow, int nCol) const
{
	if (nRow < 0 || nRow >= GetRuleCount() || nCol > ANDOR_COL)
	{
		return CInputListCtrl::GetCellType(nRow, nCol);
	}

	const SEARCHPARAM& rule = m_aSearchParams[nRow];

	switch (nCol)
	{
	case ATTRIB_COL:
		return ILCT_DROPLIST;

	case OPERATOR_COL:
		if (!rule.AttributeIs(TDCA_NONE))
		{
			return ILCT_DROPLIST;
		}
		break;

	case VALUE_COL:
		if (rule.OperatorIs(FOP_SET) || rule.OperatorIs(FOP_NOT_SET))
		{
			// handled by operator
		}
		else
		{
			TDC_ATTRIBUTE nAttribID = rule.GetAttribute();

			switch (rule.GetAttribType())
			{
			case FT_DATE:
				return ILCT_DATE;

			case FT_DATERELATIVE:
				return ILCT_POPUPMENU;

			case FT_ICON:
				if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID) && m_aAttribDefs.GetSize())
				{
					const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
					GET_DEF_RET(m_aAttribDefs, nAttribID, pDef, ILCT_BROWSE);

					if (pDef->IsList())
						return ILCT_DROPLIST;
				}
				return ILCT_BROWSE;

			case FT_TIMEPERIOD:
			case FT_BOOL:
			case FT_DEPENDENCY:
			case FT_NONE:
				// Nothing or default edit control
				break;

			case FT_RECURRENCE:
				return ILCT_DROPLIST;

			default:
				switch (nAttribID)
				{
				case TDCA_ALLOCTO:
				case TDCA_ALLOCBY:
				case TDCA_STATUS:
				case TDCA_CATEGORY:
				case TDCA_VERSION:
				case TDCA_TAGS:
				case TDCA_PRIORITY:
				case TDCA_RISK:
					return ILCT_DROPLIST;

				default:
					if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID) && m_aAttribDefs.GetSize())
					{
						const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
						GET_DEF_ALT(m_aAttribDefs, nAttribID, pDef, break);

						if (pDef->IsList())
							return ILCT_DROPLIST;

					}
					break;
				}
			}
		}
		break;

	case ANDOR_COL:
		return ILCT_DROPLIST;
	}

	// all else
	return CInputListCtrl::GetCellType(nRow, nCol);
}

BOOL CTDLFindTaskExpressionListCtrl::IsEditing() const
{
	return (CInputListCtrl::IsEditing() ||
			m_cbOperators.IsWindowVisible() ||
			m_cbAndOr.IsWindowVisible() ||
			m_cbAttributes.IsWindowVisible() ||
			m_dtcDate.IsWindowVisible() ||
			m_eTime.IsWindowVisible() ||
			m_cbListValues.IsWindowVisible() ||
			m_cbPriority.IsWindowVisible() ||
			m_cbRisk.IsWindowVisible() ||
			m_cbCustomIcons.IsWindowVisible() ||
			m_cbRecurrence.IsWindowVisible());
}

void CTDLFindTaskExpressionListCtrl::CancelEdit()
{
	OnEditCancel(0, TRUE); // TRUE - escaped
}

BOOL CTDLFindTaskExpressionListCtrl::CanEditCell(int nRow, int nCol) const
{
	if (nRow < m_aSearchParams.GetSize())
	{
		const SEARCHPARAM& rule = m_aSearchParams[nRow];

		if (nCol == VALUE_COL)
		{
			// special cases
			switch (rule.GetAttribType())
			{
			case FT_BOOL:
			case FT_DEPENDENCY:
				return FALSE;
			}

			switch (rule.GetOperator())
			{
			case FOP_SET:
			case FOP_NOT_SET:
				return FALSE;
			}
		}
	}

	// else
	return CInputListCtrl::CanEditCell(nRow, nCol);
}

void CTDLFindTaskExpressionListCtrl::PrepareEdit(int nRow, int /*nCol*/)
{
	const SEARCHPARAM& rule = m_aSearchParams[nRow];

	m_editBox.DeleteAllButtons();

	switch (rule.GetAttribType())
	{
	case FT_STRING:
		m_editBox.ClearMask();
		break;
		
	case FT_INTEGER:
		m_editBox.SetMask(_T("1234567890"));
		break;
		
	case FT_DATERELATIVE:
		m_editBox.SetMask(_T("nNtTdDWMY+-1234567890"));
		m_editBox.AddButton(RELATIVEDATEPLACEHOLDER_BTNID, _T(""), _T(""));
		m_editBox.SetDropMenuButton(RELATIVEDATEPLACEHOLDER_BTNID);
		break;
		
	case FT_DOUBLE:
		m_editBox.SetMask(_T("1234567890."), ME_LOCALIZEDECIMAL);
		break;

	case FT_DATE:
	case FT_BOOL:
	case FT_NONE:
	case FT_ICON:
	default:
		ASSERT(0);
	}
}

BOOL CTDLFindTaskExpressionListCtrl::HasRule(TDC_ATTRIBUTE nAttrib) const
{
	int nRule = m_aSearchParams.GetSize();

	while (nRule--)
	{
		if (m_aSearchParams.GetAt(nRule).AttributeIs(nAttrib))
			return TRUE;
	}

	// not found
	return FALSE;
}

BOOL CTDLFindTaskExpressionListCtrl::AddRule()
{
	SEARCHPARAM rule(TDCA_TASKNAMEORCOMMENTS, FOP_INCLUDES);

	int nRow = InsertRule(GetRuleCount(), rule);

	// make sure the 'and/or' text of the preceding rule is set
	RefreshAndOrColumnText();

	SetCurSel(nRow, ATTRIB_COL);
	EnsureVisible(nRow, FALSE);

	return TRUE;
}

BOOL CTDLFindTaskExpressionListCtrl::DeleteSelectedRule()
{
	int nRow = GetCurSel();

	if (nRow != -1 && CanDeleteSelectedCell())
	{
		DeleteItem(nRow);
		m_aSearchParams.RemoveAt(nRow);

		ValidateListData();
	
		// Fixup And/or column
		RefreshAndOrColumnText();

		SetCurSel(nRow);
		EnsureVisible(nRow, FALSE);

		return TRUE;
	}

	return FALSE;
}

void CTDLFindTaskExpressionListCtrl::MoveSelectedRuleUp()
{
	if (CanMoveSelectedRuleUp())
	{
		int nRow, nCol;
		GetCurSel(nRow, nCol);

		// save off rule
		SEARCHPARAM rule = m_aSearchParams[nRow];

		// delete rule
		m_aSearchParams.RemoveAt(nRow);
		DeleteItem(nRow);

		// reinsert rule
		nRow = InsertRule(nRow - 1, rule);

		// sanity check
		ValidateListData();

		// restore selection
		SetCurSel(nRow, nCol);
		EnsureVisible(nRow, FALSE);
	}
}

int CTDLFindTaskExpressionListCtrl::InsertRule(int nRow, const SEARCHPARAM& rule)
{
	m_aSearchParams.InsertAt(nRow, SEARCHPARAM(rule));

	CString sItem = m_cbAttributes.GetAttributeName(rule);
	int nNew = InsertRow(sItem, nRow);

	SetItemText(nNew, OPERATOR_COL, GetOpName(rule.GetOperator()));

	UpdateValueColumnText(nRow);
	
	// Fixup And/or column
	RefreshAndOrColumnText();

	return nNew;
}

BOOL CTDLFindTaskExpressionListCtrl::CanMoveSelectedRuleUp() const
{
	int nRow = GetCurSel();

	return (nRow > 0 && nRow < GetRuleCount());
}

void CTDLFindTaskExpressionListCtrl::MoveSelectedRuleDown()
{
	if (CanMoveSelectedRuleDown())
	{
		int nRow, nCol;
		GetCurSel(nRow, nCol);

		// save off rule
		SEARCHPARAM rule = m_aSearchParams[nRow];

		// delete rule
		m_aSearchParams.RemoveAt(nRow);
		DeleteItem(nRow);

		// reinsert rule
		nRow = InsertRule(nRow + 1, rule);
	
		// sanity check
		ValidateListData();

		// restore selection
		SetCurSel(nRow, nCol);
		EnsureVisible(nRow, FALSE);
	}
}

BOOL CTDLFindTaskExpressionListCtrl::CanMoveSelectedRuleDown() const
{
	return (GetCurSel() < GetRuleCount() - 1);
}

void CTDLFindTaskExpressionListCtrl::PrepareControl(CWnd& ctrl, int nRow, int nCol)
{
	UNREFERENCED_PARAMETER(ctrl);

	if (!GetRuleCount())
		return;

	SEARCHPARAM& rule = m_aSearchParams[nRow];

	switch (nCol)
	{
	case ATTRIB_COL:
		{
			ASSERT (&ctrl == &m_cbAttributes);
	
			m_cbAttributes.SelectAttribute(rule);
			CDialogHelper::RefreshMaxDropWidth(m_cbAttributes);
		}
		break;

	case OPERATOR_COL:
		{
			ASSERT (&ctrl == &m_cbOperators);

			m_cbOperators.ResetContent();
			
			FIND_ATTRIBTYPE nType = rule.GetAttribType();

			switch (nType)
			{
			case FT_STRING:
			case FT_ICON:
				AddOperatorToCombo(FOP_SET);
				AddOperatorToCombo(FOP_NOT_SET);
				AddOperatorToCombo(FOP_EQUALS);
				AddOperatorToCombo(FOP_NOT_EQUALS);
				AddOperatorToCombo(FOP_INCLUDES);
				AddOperatorToCombo(FOP_NOT_INCLUDES);
				break;

			case FT_INTEGER:
			case FT_DOUBLE:
			case FT_TIMEPERIOD:
				AddOperatorToCombo(FOP_SET);
				AddOperatorToCombo(FOP_NOT_SET);
				AddOperatorToCombo(FOP_EQUALS);
				AddOperatorToCombo(FOP_NOT_EQUALS);
				AddOperatorToCombo(FOP_GREATER);
				AddOperatorToCombo(FOP_GREATER_OR_EQUAL);
				AddOperatorToCombo(FOP_LESS);
				AddOperatorToCombo(FOP_LESS_OR_EQUAL);
				break;

			case FT_DATE:
				AddOperatorToCombo(FOP_SET);
				AddOperatorToCombo(FOP_NOT_SET);
				AddOperatorToCombo(FOP_EQUALS);
				AddOperatorToCombo(FOP_NOT_EQUALS);
				AddOperatorToCombo(FOP_AFTER);
				AddOperatorToCombo(FOP_ON_OR_AFTER);
				AddOperatorToCombo(FOP_BEFORE);
				AddOperatorToCombo(FOP_ON_OR_BEFORE);
				break;

			case FT_DATERELATIVE:
				AddOperatorToCombo(FOP_EQUALS);
				AddOperatorToCombo(FOP_NOT_EQUALS);
				AddOperatorToCombo(FOP_AFTER);
				AddOperatorToCombo(FOP_ON_OR_AFTER);
				AddOperatorToCombo(FOP_BEFORE);
				AddOperatorToCombo(FOP_ON_OR_BEFORE);
				break;

			case FT_BOOL:
				AddOperatorToCombo(FOP_SET);
				AddOperatorToCombo(FOP_NOT_SET);
				break;

			case FT_DEPENDENCY:
				AddOperatorToCombo(FOP_SET);
				AddOperatorToCombo(FOP_NOT_SET);
				AddOperatorToCombo(FOP_EQUALS);
				AddOperatorToCombo(FOP_NOT_EQUALS);
				AddOperatorToCombo(FOP_INCLUDES);
				AddOperatorToCombo(FOP_NOT_INCLUDES);
				AddOperatorToCombo(FOP_IS_COMPLETE);
				break;
				
			case FT_RECURRENCE:
				AddOperatorToCombo(FOP_SET);
				AddOperatorToCombo(FOP_NOT_SET);
				AddOperatorToCombo(FOP_EQUALS);
				AddOperatorToCombo(FOP_NOT_EQUALS);
				break;
			}
	
			CDialogHelper::SelectItemByData(m_cbOperators, (DWORD)rule.GetOperator());
		}
		break;

	case ANDOR_COL:
		{
			ASSERT (&ctrl == &m_cbAndOr);

			if (rule.GetAnd())
				m_cbAndOr.SelectString(-1, CEnString(IDS_FP_AND));
			else
				m_cbAndOr.SelectString(-1, CEnString(IDS_FP_OR));
		}
		break;

	case VALUE_COL:
		if (&ctrl == &m_dtcDate)
		{
			// if the rule does not yet have a date then set it now to
			// the current date because that's whats the date ctrl will default to
			if (!CDateHelper::IsDateSet(rule.ValueAsDate()))
			{
				rule.SetValue(COleDateTime::GetCurrentTime());
				SetItemText(nRow, nCol, rule.ValueAsDate().Format(VAR_DATEVALUEONLY));
			}
			else
			{
				m_dtcDate.SetTime(rule.ValueAsDouble());
			}
		}
		else if (&ctrl == &m_eTime)
		{
			m_eTime.SetTime(rule.ValueAsDouble(), TDC::MapUnitsToTHUnits(rule.GetTimeUnits()));
		}
		else if (&ctrl == &m_cbCustomIcons)
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_DEF_ALT(m_aAttribDefs, rule.GetAttribute(), pDef, return);

			ASSERT(pDef->IsList());


			CDialogHelper::SetComboBoxItems(m_cbCustomIcons, pDef->aDefaultListData);

			CStringArray aIcons;
			Misc::Split(rule.ValueAsString(), aIcons);
			m_cbCustomIcons.SetChecked(aIcons);
		}
		else if (&ctrl == &m_cbListValues)
		{
			m_cbListValues.ResetContent();

			TDC_ATTRIBUTE nAttribID = rule.GetAttribute();

			if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
			{
				int nAttrib = m_aAttribDefs.Find(nAttribID);
				ASSERT(nAttrib != -1);

				if (nAttrib != -1)
				{
					const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aAttribDefs[nAttrib];
					ASSERT(attribDef.IsList());

					if (attribDef.IsList())
					{
						int nItem = attribDef.aDefaultListData.GetSize();

						while (nItem--)
							m_cbListValues.AddString(attribDef.aDefaultListData[nItem]);

						nItem = attribDef.aAutoListData.GetSize();

						while (nItem--)
							m_cbListValues.AddString(attribDef.aAutoListData[nItem]);
					}
				}
			}
			else
			{
				switch (nAttribID)
				{
				case TDCA_CATEGORY: CDialogHelper::SetComboBoxItems(m_cbListValues, m_tldListContents.aCategory); break;
				case TDCA_ALLOCTO:	CDialogHelper::SetComboBoxItems(m_cbListValues, m_tldListContents.aAllocTo); break;
				case TDCA_ALLOCBY:	CDialogHelper::SetComboBoxItems(m_cbListValues, m_tldListContents.aAllocBy); break;
				case TDCA_VERSION:	CDialogHelper::SetComboBoxItems(m_cbListValues, m_tldListContents.aVersion); break;
				case TDCA_TAGS:		CDialogHelper::SetComboBoxItems(m_cbListValues, m_tldListContents.aTags); break;
				case TDCA_STATUS:	CDialogHelper::SetComboBoxItems(m_cbListValues, m_tldListContents.aStatus); break;
				}
			}

			m_cbListValues.SelectString(0, rule.ValueAsString());
		}
		else if (&ctrl == &m_cbPriority)
		{
			m_cbPriority.SetSelectedPriority(rule.ValueAsInteger());
		}
		else if (&ctrl == &m_cbRisk)
		{
			m_cbRisk.SetSelectedRisk(rule.ValueAsInteger());
		}
		else if (&ctrl == &m_cbRecurrence)
		{
			m_cbRecurrence.SetSelectedRegularity((TDC_REGULARITY)rule.ValueAsInteger());
		}
		break;
	}
}

void CTDLFindTaskExpressionListCtrl::AddOperatorToCombo(FIND_OPERATOR nOp)
{
	int i = m_cbOperators.AddString(GetOpName(nOp)); 
	m_cbOperators.SetItemData(i, (DWORD)nOp); 
}

void CTDLFindTaskExpressionListCtrl::ValidateListData() const
{
#ifdef _DEBUG
	for (int nRule = 0; nRule < GetRuleCount(); nRule++)
	{
		const SEARCHPARAM& rule = m_aSearchParams[nRule];

		// check matching attribute text 
		CString sRuleAttrib = m_cbAttributes.GetAttributeName(rule);
		CString sListAttrib = GetItemText(nRule, ATTRIB_COL);
		ASSERT (sRuleAttrib == sListAttrib);

		// check matching operator text 
		CString sRuleOp = GetOpName(rule.GetOperator());
		CString sListOp = GetItemText(nRule, OPERATOR_COL);
		ASSERT (sListOp.IsEmpty() || sRuleOp == sListOp);

		// check valid operator
		ASSERT(rule.HasValidOperator());
	}
#endif
}

void CTDLFindTaskExpressionListCtrl::OnAttribEditCancel()
{
	HideControl(m_cbAttributes);
}

void CTDLFindTaskExpressionListCtrl::OnAttribEditOK()
{
	HideControl(m_cbAttributes);

	// update item text and keep data store synched
	int nRow = GetCurSel();

	if (nRow != CB_ERR)
	{
		SEARCHPARAM& rule = m_aSearchParams[nRow];
		SEARCHPARAM ruleNew(rule);

		if (m_cbAttributes.GetSelectedAttribute(ruleNew) && (rule != ruleNew))
		{
			rule = ruleNew;

			// update list text
			CString sAttrib = m_cbAttributes.GetSelectedAttributeText();
			SetItemText(nRow, ATTRIB_COL, sAttrib);

			// clear the operator cell text if the operator was no longer valid
			if (rule.OperatorIs(FOP_NONE))
				SetItemText(nRow, OPERATOR_COL, _T(""));

			// Clear the text value if the attribute type has changed
			if (rule.GetAttribType() != ruleNew.GetAttribType())
			{
				rule.SetValue(_T(""));
				UpdateValueColumnText(nRow);
			}
			
			ValidateListData();
		}
	}
}

BOOL CTDLFindTaskExpressionListCtrl::OnSelItemChanged(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	// always make sure we hide our non-default controls
	HideControl(m_dtcDate);
	HideControl(m_eTime);
	HideControl(m_cbListValues);
	HideControl(m_cbPriority);
	HideControl(m_cbRisk);

	*pResult = 0;
	
	return FALSE; // continue routing
}

void CTDLFindTaskExpressionListCtrl::OnValueEditOK(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	
	int nRow = pDispInfo->item.iItem;
	int nCol = pDispInfo->item.iSubItem;

	ASSERT (nCol == VALUE_COL);

	SEARCHPARAM& rule = m_aSearchParams[nRow];

	switch (rule.GetAttribType())
	{
	case FT_STRING:
	case FT_DATERELATIVE:
	case FT_INTEGER:
	case FT_DOUBLE:
	case FT_DEPENDENCY:
	case FT_TIMEPERIOD:
		rule.SetValue(pDispInfo->item.pszText);
		break;
		
	default:
		// not handled here
		ASSERT(0);
		break;
	}
		
	*pResult = 0;
}

void CTDLFindTaskExpressionListCtrl::OnOperatorEditCancel()
{
	HideControl(m_cbOperators);
}

void CTDLFindTaskExpressionListCtrl::OnOperatorEditOK()
{
	HideControl(m_cbOperators);

	// update operator type
	int nSel = m_cbOperators.GetCurSel();

	if (nSel != CB_ERR)
	{
		CString sSel;
		m_cbOperators.GetLBText(nSel, sSel);

		int nRow = GetCurSel();
		SetItemText(nRow, OPERATOR_COL, sSel);

		// keep data store synched
		SEARCHPARAM& rule = m_aSearchParams[nRow];
		FIND_OPERATOR nNewOp = (FIND_OPERATOR)m_cbOperators.GetItemData(nSel);

		rule.SetOperator(nNewOp);

		// if the op is set/notset then clear the field
		if (nNewOp == FOP_SET || nNewOp == FOP_NOT_SET)
		{
			rule.ClearValue();
		}

		UpdateValueColumnText(nRow);
		ValidateListData();
	}
}

void CTDLFindTaskExpressionListCtrl::OnAndOrEditCancel()
{
	HideControl(m_cbAndOr);
}

void CTDLFindTaskExpressionListCtrl::OnAndOrEditOK()
{
	HideControl(m_cbAndOr);

	// update operator type
	int nSel = m_cbAndOr.GetCurSel();

	if (nSel != CB_ERR)
	{
		CString sSel;
		m_cbAndOr.GetLBText(m_cbAndOr.GetCurSel(), sSel);

		int nRow = GetCurSel();
		SetItemText(nRow, ANDOR_COL, sSel);

		// keep data store synched
		m_aSearchParams[nRow].SetAnd(m_cbAndOr.GetItemData(nSel));

		ValidateListData();
	}
}

void CTDLFindTaskExpressionListCtrl::OnListValuesEditCancel()
{
	HideControl(m_cbListValues);
}

void CTDLFindTaskExpressionListCtrl::OnListValuesEditOK()
{
	HideControl(m_cbListValues);

	// update rule and cell text
	int nRow = GetCurSel();

	CStringArray aSel;
	m_cbListValues.GetChecked(aSel);

	m_aSearchParams[nRow].SetValue(Misc::FormatArray(aSel));
	m_aSearchParams[nRow].SetMatchWholeWord(TRUE); // because lists are read-only

	UpdateValueColumnText(nRow);
}

void CTDLFindTaskExpressionListCtrl::OnPriorityEditCancel()
{
	HideControl(m_cbPriority);
}

void CTDLFindTaskExpressionListCtrl::OnPriorityEditOK()
{
	HideControl(m_cbPriority);

	// update value
	int nRow = GetCurSel();
	int nPriority = m_cbPriority.GetSelectedPriority();

	// keep data store synched
	m_aSearchParams[nRow].SetValue(nPriority);
	UpdateValueColumnText(nRow);
}

void CTDLFindTaskExpressionListCtrl::OnRiskEditCancel()
{
	HideControl(m_cbRisk);
}

void CTDLFindTaskExpressionListCtrl::OnRiskEditOK()
{
	HideControl(m_cbRisk);

	// update value
	int nRow = GetCurSel();
	int nRisk = m_cbRisk.GetSelectedRisk();

	// keep data store synched
	m_aSearchParams[nRow].SetValue(nRisk);
	UpdateValueColumnText(nRow);
}

void CTDLFindTaskExpressionListCtrl::OnRecurrenceEditCancel()
{
	HideControl(m_cbRecurrence);
}

void CTDLFindTaskExpressionListCtrl::OnRecurrenceEditOK()
{
	HideControl(m_cbRecurrence);

	// update value
	int nRow = GetCurSel();
	int nRegularity = m_cbRecurrence.GetSelectedRegularity();

	// keep data store synched
	m_aSearchParams[nRow].SetValue(nRegularity);
	UpdateValueColumnText(nRow);
}

void CTDLFindTaskExpressionListCtrl::OnCustomIconEditChange()
{
	HideControl(m_cbCustomIcons);

	// update value
	int nRow = GetCurSel();

	CStringArray aIcons;
	m_cbCustomIcons.GetChecked(aIcons);

	// keep data store synched
	m_aSearchParams[nRow].SetValue(Misc::FormatArray(aIcons));
	UpdateValueColumnText(nRow);
}

void CTDLFindTaskExpressionListCtrl::BuildListCtrl()
{
	DeleteAllItems();

	for (int nParam = 0; nParam < GetRuleCount(); nParam++)
	{
		const SEARCHPARAM& rule = m_aSearchParams[nParam];

		// attrib
		CString sAttrib = m_cbAttributes.GetAttributeName(rule);
		int nItem = InsertRow(sAttrib, nParam);

		// operator
		CString sOp = GetOpName(rule.GetOperator());
		SetItemText(nItem, OPERATOR_COL, sOp);

		// value
		UpdateValueColumnText(nItem);

		// and/or (but not for last row)
		if (nParam < GetRuleCount() - 1)
		{
			CEnString sAndOr(rule.GetAnd() ? IDS_FP_AND : IDS_FP_OR);
			SetItemText(nItem, ANDOR_COL, sAndOr);
		}
	}

	ValidateListData();
	SetCurSel(0);
}

void CTDLFindTaskExpressionListCtrl::RefreshAndOrColumnText()
{
	for (int nParam = 0; nParam < GetRuleCount(); nParam++)
	{
		const SEARCHPARAM& rule = m_aSearchParams[nParam];
		
		// and/or (but not for last row)
		if (nParam < GetRuleCount() - 1)
		{
			CEnString sAndOr(rule.GetAnd() ? IDS_FP_AND : IDS_FP_OR);
			SetItemText(nParam, ANDOR_COL, sAndOr);
		}
		else
		{
			SetItemText(nParam, ANDOR_COL, _T(""));
		}
	}
}

void CTDLFindTaskExpressionListCtrl::UpdateValueColumnText(int nRow)
{
	ASSERT(nRow >= 0 && nRow < m_aSearchParams.GetSize());
	ASSERT(m_aSearchParams.GetSize() >= GetItemCount() - 1);

	if (nRow < 0 || nRow >= m_aSearchParams.GetSize())
		return;
	
	if (m_aSearchParams.GetSize() < GetItemCount() - 1)
		return;

	const SEARCHPARAM& rule = m_aSearchParams[nRow];
	CString sValue;
		
	// value (but not boolean)
	if (rule.GetOperator() != FOP_SET && rule.GetOperator() != FOP_NOT_SET)
	{
		try
		{
			switch (rule.GetAttribType())
			{
			case FT_INTEGER:
			case FT_STRING:
			case FT_DATERELATIVE:
			case FT_DOUBLE:
			case FT_ICON:
			case FT_DEPENDENCY:
				sValue = rule.ValueAsString();
				break;

			case FT_DATE:
				sValue = rule.ValueAsDate().Format(VAR_DATEVALUEONLY);
				break;

			case FT_TIMEPERIOD:
				sValue = CTimeHelper().FormatTime(rule.ValueAsDouble(), TDC::MapUnitsToTHUnits(rule.GetTimeUnits()), 2);
				break;

			case FT_BOOL:
				// handled by operator
				break;

			case FT_RECURRENCE:
				sValue = m_cbRecurrence.GetRegularity((TDC_REGULARITY)rule.ValueAsInteger());
				break;
			}
		}
		catch (...)
		{
			// bad value but we continue
			sValue.Empty();
		}
	}
			
	SetItemText(nRow, VALUE_COL, sValue);
}

CString CTDLFindTaskExpressionListCtrl::GetOpName(FIND_OPERATOR op)
{
	int nOp = OP_COUNT;

	while (nOp--)
	{
		if (OPERATORS[nOp].op == op)
		{
			if (OPERATORS[nOp].nOpResID)
				return CEnString(OPERATORS[nOp].nOpResID);
			else
				return _T("");
		}
	}

	ASSERT (0); // not found
	return _T("");
}

void CTDLFindTaskExpressionListCtrl::OnDateChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTHDR = (LPNMDATETIMECHANGE)pNMHDR;
	COleDateTime dt(pDTHDR->st);

	// update the rule 
	int nRow = GetCurSel();

	m_aSearchParams[nRow].SetValue(dt);
	UpdateValueColumnText(nRow);
	
	*pResult = 0;
}

void CTDLFindTaskExpressionListCtrl::OnDateCloseUp(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	HideControl(m_dtcDate);
	
	*pResult = 0;
}


void CTDLFindTaskExpressionListCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CInputListCtrl::OnKillFocus(pNewWnd);

	if (pNewWnd == this)
		return;

	// else
	HideAllControls(pNewWnd);
}

void CTDLFindTaskExpressionListCtrl::HideAllControls(const CWnd* pWndIgnore)
{
	if (!pWndIgnore || ((pWndIgnore != &m_dtcDate) && (pWndIgnore != m_dtcDate.GetMonthCalCtrl())))
		HideControl(m_dtcDate);

	if (!pWndIgnore || (pWndIgnore != &m_eTime))
		HideControl(m_eTime);

	if (!pWndIgnore || (pWndIgnore != &m_cbListValues))
		HideControl(m_cbListValues);

	if (!pWndIgnore || (pWndIgnore != &m_cbPriority))
		HideControl(m_cbPriority);

	if (!pWndIgnore || (pWndIgnore != &m_cbRisk))
		HideControl(m_cbRisk);
	
	if (!pWndIgnore || (pWndIgnore != &m_cbCustomIcons))
		HideControl(m_cbCustomIcons);

	if (!pWndIgnore || (pWndIgnore != &m_cbRecurrence))
		HideControl(m_cbRecurrence);
}

void CTDLFindTaskExpressionListCtrl::OnTimeChange()
{
	// update the rule 
	int nRow = GetCurSel();
	SEARCHPARAM& rule = m_aSearchParams[nRow];

	rule.SetValue(m_eTime.Convert());
	rule.SetTimeUnits(TDC::MapTHUnitsToUnits(m_eTime.GetUnits()));

	UpdateValueColumnText(nRow);
}

LRESULT CTDLFindTaskExpressionListCtrl::OnTimeUnitsChange(WPARAM /*wp*/, LPARAM /*lp*/)
{
	// update the rule 
	int nRow = GetCurSel();
	SEARCHPARAM& rule = m_aSearchParams[nRow];

	rule.SetTimeUnits(TDC::MapTHUnitsToUnits(m_eTime.GetUnits()));

	UpdateValueColumnText(nRow);

	return 0L;
}

LRESULT CTDLFindTaskExpressionListCtrl::OnEEBtnClick(WPARAM /*wp*/, LPARAM lp)
{
	switch (lp)
	{
	case RELATIVEDATEPLACEHOLDER_BTNID:
		{
			CMenu menu;

			if (menu.LoadMenu(IDR_FINDTASKS))
			{
				CLocalizer::TranslateMenu(menu);

				UINT nID = m_editBox.TrackPopupMenu(lp, menu.GetSubMenu(0), EETPM_RETURNCMD);
				CString sRelDate;

				switch (nID)
				{
				case ID_RELATIVEDATE_TODAY:			sRelDate = _T("t");	  break;
				case ID_RELATIVEDATE_TOMORROW:		sRelDate = _T("t+1"); break;
				case ID_RELATIVEDATE_YESTERDAY:		sRelDate = _T("t-1"); break;

				case ID_RELATIVEDATE_ENDTHISWEEK:	sRelDate = _T("W");   break;
				case ID_RELATIVEDATE_ENDNEXTWEEK:	sRelDate = _T("W+1"); break;
				case ID_RELATIVEDATE_ENDLASTWEEK:	sRelDate = _T("W-1"); break;

				case ID_RELATIVEDATE_ENDTHISMONTH:	sRelDate = _T("M");   break;
				case ID_RELATIVEDATE_ENDNEXTMONTH:	sRelDate = _T("M+1"); break;
				case ID_RELATIVEDATE_ENDLASTMONTH:	sRelDate = _T("M-1"); break;

				case ID_RELATIVEDATE_ENDTHISYEAR:	sRelDate = _T("Y");   break;
				case ID_RELATIVEDATE_ENDNEXTYEAR:	sRelDate = _T("Y+1"); break;
				case ID_RELATIVEDATE_ENDLASTYEAR:	sRelDate = _T("Y-1"); break;

				case 0: // lost focus == cancel
				case IDCANCEL:
					break;

				default:
					ASSERT(0);
				}

				if (!sRelDate.IsEmpty())
					m_editBox.ReplaceSel(sRelDate, TRUE);
			}
		}
		break;
	}

	return 0L;
}

void CTDLFindTaskExpressionListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	// resize columns by proportion before calling base class
	CHoldRedraw hr(*this);

	for (int nCol = 0; nCol < NUM_COLS; nCol++)
		SetColumnWidth(nCol, (int)(cx * COL_PROPORTIONS[nCol]));

	CInputListCtrl::OnSize(nType, cx, cy);
}

void CTDLFindTaskExpressionListCtrl::OnCancelEdit()
{
	CInputListCtrl::OnCancelEdit();

	HideAllControls();
}

void CTDLFindTaskExpressionListCtrl::InitState()
{
	CInputListCtrl::InitState();

	SetAutoRowPrompt(CEnString(IDS_FP_NEW_RULE));
	OverrideSelectionTheming(TRUE, TRUE);
}

void CTDLFindTaskExpressionListCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int nItem, nCol;
	GetCurSel(nItem, nCol);
	
	if (GetRuleCount() > 0 || nCol == ATTRIB_COL)
	{
		// if the user typed an alphanumeric char then begin editing automatically
		// numeric keys only work for value column
		if (isalpha(nChar) || (nCol == VALUE_COL && isdigit(nChar)))
		{
			EditCell(nItem, nCol, FALSE);

			// forward key down on to edit control
			CWnd* pEdit = GetEditControl(nItem, nCol);

			if (pEdit)
			{
				pEdit->PostMessage(WM_CHAR, nChar, MAKELPARAM(nRepCnt, nFlags));
				return; // eat it
			}
		}
	}
	
	CInputListCtrl::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CTDLFindTaskExpressionListCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		int nSel = GetCurSel();
		
		if (nSel != -1)
		{
			switch (pMsg->wParam)
			{
			case VK_UP:
				if (Misc::ModKeysArePressed(MKS_CTRL))
				{
					MoveSelectedRuleUp();
					return TRUE; // eat it
				}
				break;

			case VK_DOWN:
				if (Misc::ModKeysArePressed(MKS_CTRL))
				{
					MoveSelectedRuleDown();
					return TRUE; // eat it
				}
				break;
			}
		}
	}
	
	return CInputListCtrl::PreTranslateMessage(pMsg);
}

void CTDLFindTaskExpressionListCtrl::DrawCellText(CDC* pDC, int nRow, int nCol, 
													const CRect& rText, const CString& sText, 
													COLORREF crText, UINT nDrawTextFlags)
{
	if ((nCol == VALUE_COL) && !IsPrompt(nRow))
	{
		const SEARCHPARAM& rule = m_aSearchParams[nRow];

		if (rule.HasIcon())
		{
			// Don't use sText because it might have been truncated
			CStringArray aIcons;
			int nNumIcons = Misc::Split(GetItemText(nRow, nCol), aIcons);

			CRect rIcon(rText);
			rIcon.DeflateRect(0, ((rText.Height() - IMAGE_SIZE) / 2));

			for (int nIcon = 0; nIcon < nNumIcons; nIcon++)
			{
				int nIconIdx = m_ilIcons.GetImageIndex(aIcons[nIcon]);
				
				if (nIconIdx != -1)
				{
					m_ilIcons.Draw(pDC, nIconIdx, rIcon.TopLeft(), ILD_TRANSPARENT);
					rIcon.left += (IMAGE_SIZE + 2);
				}
			}

			return;
		}
	}

	// all else
	CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sText, crText, nDrawTextFlags);
}
