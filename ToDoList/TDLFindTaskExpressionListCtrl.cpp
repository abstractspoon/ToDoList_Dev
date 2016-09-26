// FindTaskExpressionListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLFindTaskExpressionListCtrl.h"
#include "tdcstatic.h"
#include "TDCCustomAttributeHelper.h"
#include "TDLTaskIconDlg.h"

#include "..\shared\HoldRedraw.h"
#include "..\shared\dialoghelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum 
{ 
	ATTRIB_COL, 
	OPERATOR_COL, 
	VALUE_COL, 
	ANDOR_COL 
};

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
};

const float COL_PROPORTIONS[] = { 15.0f/47, 13.0f/47, 13.0f/47, 6.0f/47 };

/////////////////////////////////////////////////////////////////////////////
// CTDLFindTaskExpressionListCtrl

CTDLFindTaskExpressionListCtrl::CTDLFindTaskExpressionListCtrl()
	:
	m_dtcDate(MCS_WEEKNUMBERS),
	m_cbCustomIcons(m_ilIcons, TRUE)
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
	ON_CBN_SELCHANGE(CUSTOMICON_ID, OnCustomIconEditChange)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnValueEditOK)
	ON_NOTIFY_REFLECT_EX(LVN_ITEMCHANGED, OnSelItemChanged)
	ON_NOTIFY(DTN_DATETIMECHANGE, DATE_ID, OnDateChange)
	ON_NOTIFY(DTN_CLOSEUP, DATE_ID, OnDateCloseUp)
	ON_REGISTERED_MESSAGE(WM_TEN_UNITSCHANGE, OnTimeUnitsChange)
	ON_EN_CHANGE(TIME_ID, OnTimeChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLFindTaskExpressionListCtrl message handlers

void CTDLFindTaskExpressionListCtrl::PreSubclassWindow() 
{
	// create child controls
	CreateControl(m_cbAttributes, ATTRIB_ID);
	CreateControl(m_cbOperators, OPERATOR_ID, FALSE);
	CreateControl(m_cbAndOr, ANDOR_ID, FALSE);
	CreateControl(m_dtcDate, DATE_ID);
	CreateControl(m_eTime, TIME_ID);
	CreateControl(m_cbListValues, LISTVALUES_ID);
	CreateControl(m_cbPriority, PRIORITY_ID, FALSE);
	CreateControl(m_cbRisk, RISK_ID, FALSE);
	CreateControl(m_cbCustomIcons, CUSTOMICON_ID, FALSE);

	CInputListCtrl::PreSubclassWindow(); // we need combo to be created first

	// build and/or combo too
	int nItem = m_cbAndOr.AddString(CEnString(IDS_FP_AND));
	m_cbAndOr.SetItemData(nItem, TRUE);
				
	nItem = m_cbAndOr.AddString(CEnString(IDS_FP_OR));
	m_cbAndOr.SetItemData(nItem, FALSE);

	// post message for our setup
	m_header.EnableTracking(FALSE);
	ShowGrid(TRUE, TRUE);

	InsertColumn(ATTRIB_COL, CEnString(IDS_FT_ATTRIB), LVCFMT_LEFT, 120);
	InsertColumn(OPERATOR_COL, CEnString(IDS_FT_MATCHES), LVCFMT_LEFT, 160);
	InsertColumn(VALUE_COL, CEnString(IDS_FT_VALUE), LVCFMT_LEFT, 130);
	InsertColumn(ANDOR_COL, CEnString(IDS_FT_ANDOR), LVCFMT_LEFT, 60);
	SetView(LVS_REPORT);

	AutoAdd(TRUE, FALSE);

	BuildListCtrl();

	SetColumnType(ATTRIB_COL, ILCT_DROPLIST);
	SetColumnType(OPERATOR_COL, ILCT_DROPLIST);
	SetColumnType(ANDOR_COL, ILCT_DROPLIST);

	// Not sure why this doesn't work 'out of the box'
	// but we need to set the style manually
	m_dtcDate.SetMonthCalStyle(MCS_WEEKNUMBERS);
}

void CTDLFindTaskExpressionListCtrl::SetCustomAttributes(const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	m_aAttribDefs.Copy(aAttribDefs);
	m_cbAttributes.SetCustomAttributes(m_aAttribDefs);
}

void CTDLFindTaskExpressionListCtrl::SetAttributeListData(const TDCAUTOLISTDATA& tld)
{
	m_tldListContents.Copy(tld);
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

	const SEARCHPARAM& sp = m_aSearchParams[nItem];

	switch (nCol)
	{
	case ATTRIB_COL:
		return &m_cbAttributes;

	case OPERATOR_COL:
		if (!sp.AttributeIs(TDCA_NONE))
		{
			return &m_cbOperators;
		}
		break;

	case VALUE_COL:
		if (sp.OperatorIs(FOP_SET) || sp.OperatorIs(FOP_NOT_SET))
		{
			// handled by operator
		}
		else
		{
			FIND_ATTRIBTYPE nType = sp.GetAttribType();

			switch (nType)
			{
			case FT_DATE:
				return &m_dtcDate;

			case FT_TIME:
				return &m_eTime;

			case FT_BOOL:
				// do nothing: it's handled by the operator
				break;

			case FT_NONE:
				// do nothing.
				break;

			default:
				switch (sp.GetAttribute())
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

				default:
					{
						TDCCUSTOMATTRIBUTEDEFINITION attribDef;

						if (CTDCCustomAttributeHelper::GetAttributeDef(sp.GetAttribute(), m_aAttribDefs, attribDef))
						{
							if (attribDef.IsList())
							{
								if (attribDef.GetDataType() == TDCCA_ICON)
									return &m_cbCustomIcons;
								else
									return &m_cbListValues;
							}
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

void CTDLFindTaskExpressionListCtrl::EditCell(int nItem, int nCol)
{
	// handle new rules
	if (nItem == GetRuleCount() && nCol == ATTRIB_COL)
		AddRule();

	CWnd* pEdit = GetEditControl(nItem, nCol);
	ASSERT (pEdit);

	if (!pEdit)
		return;

	SEARCHPARAM& sp = m_aSearchParams[nItem];

	switch (nCol)
	{
	case ATTRIB_COL:
	case OPERATOR_COL:
	case ANDOR_COL:
		ShowControl(*pEdit, nItem, nCol);
		break;

	case VALUE_COL:
		if ((sp.GetOperator() != FOP_SET) && (sp.GetOperator() != FOP_NOT_SET))
		{
			switch (sp.GetAttribType())
			{
			case FT_DATE:
			case FT_TIME:
				ShowControl(*pEdit, nItem, nCol);
				break;

			case FT_ICON:
				{
					BOOL bBrowse = (sp.GetAttribute() == TDCA_ICON);

					if (!bBrowse)
					{
						TDCCUSTOMATTRIBUTEDEFINITION attribDef;

						if (CTDCCustomAttributeHelper::GetAttributeDef(sp.GetAttribute(), m_aAttribDefs, attribDef))
							bBrowse = !attribDef.IsList();
					}
					
					if (bBrowse)
					{
						CTDLTaskIconDlg dialog(m_ilIcons, sp.ValueAsString());

						if (dialog.DoModal() == IDOK)
						{
							sp.SetValue(dialog.GetIconName());
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
				switch (sp.GetAttribute())
				{
				case TDCA_ICON:
					{
					}
					break;

				default:
					PrepareEdit(nItem, nCol);

					if (pEdit == &m_editBox)
						CInputListCtrl::EditCell(nItem, nCol);
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

	const SEARCHPARAM& sp = m_aSearchParams[nRow];

	switch (nCol)
	{
	case ATTRIB_COL:
		return ILCT_DROPLIST;

	case OPERATOR_COL:
		if (!sp.AttributeIs(TDCA_NONE))
		{
			return ILCT_DROPLIST;
		}
		break;

	case VALUE_COL:
		if (sp.OperatorIs(FOP_SET) || sp.OperatorIs(FOP_NOT_SET))
		{
			// handled by operator
		}
		else
		{
			switch (sp.GetAttribType())
			{
			case FT_DATE:
				return ILCT_DATE;

			case FT_ICON:
				{
					TDCCUSTOMATTRIBUTEDEFINITION attribDef;
					BOOL bList = FALSE;

					if (CTDCCustomAttributeHelper::GetAttributeDef(sp.GetAttribute(), m_aAttribDefs, attribDef))
						bList = attribDef.IsList();

					return (bList ? ILCT_DROPLIST : ILCT_BROWSE);
				}
				break;

			case FT_TIME:
				// do nothing.
				break;

			case FT_BOOL:
				// do nothing: it's handled by the operator
				break;

			case FT_NONE:
				// do nothing.
				break;

			default:
				{
					TDC_ATTRIBUTE nAttrib = sp.GetAttribute();

					switch (nAttrib)
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
						{
							TDCCUSTOMATTRIBUTEDEFINITION attribDef;

							if (CTDCCustomAttributeHelper::GetAttributeDef(nAttrib, m_aAttribDefs, attribDef))
							{
								if (attribDef.IsList())
								{
									return ILCT_DROPLIST;
								}
							}
						}
						break;
					}
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
			m_cbCustomIcons.IsWindowVisible());
}

BOOL CTDLFindTaskExpressionListCtrl::CanEditSelectedCell() const
{
	int nRow, nCol;
	GetCurSel(nRow, nCol);

	if (nRow < m_aSearchParams.GetSize())
	{
		const SEARCHPARAM& rule = m_aSearchParams[nRow];

		if (nCol == VALUE_COL)
		{
			// Boolean type is a special case
			if (rule.GetAttribType() == FT_BOOL)
				return FALSE;

			// as is using the set/notset operators
			if (rule.OperatorIs(FOP_SET) || rule.OperatorIs(FOP_NOT_SET))
				return FALSE;
		}
	}

	// else
	return CInputListCtrl::CanEditSelectedCell();
}

void CTDLFindTaskExpressionListCtrl::PrepareEdit(int nRow, int /*nCol*/)
{
	const SEARCHPARAM& sp = m_aSearchParams[nRow];

	switch (sp.GetAttribType())
	{
	case FT_STRING:
		m_editBox.SetMask(_T(""));
		break;
		
	case FT_INTEGER:
		m_editBox.SetMask(_T("1234567890"));
		break;
		
	case FT_DATE_REL:
		m_editBox.SetMask(_T("tTdDWMY+-1234567890"));
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

BOOL CTDLFindTaskExpressionListCtrl::AddRule()
{
	SEARCHPARAM sp(TDCA_TASKNAMEORCOMMENTS, FOP_INCLUDES);

	int nRow = InsertRule(GetRuleCount(), sp);

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
		SEARCHPARAM sp = m_aSearchParams[nRow];

		// delete rule
		m_aSearchParams.RemoveAt(nRow);
		DeleteItem(nRow);

		// reinsert rule
		nRow = InsertRule(nRow - 1, sp);

		// sanity check
		ValidateListData();

		// restore selection
		SetCurSel(nRow, nCol);
		EnsureVisible(nRow, FALSE);
	}
}

int CTDLFindTaskExpressionListCtrl::InsertRule(int nRow, const SEARCHPARAM& sp)
{
	m_aSearchParams.InsertAt(nRow, SEARCHPARAM(sp));

	CString sItem = m_cbAttributes.GetAttributeName(sp);
	int nNew = InsertItem(nRow, sItem);

	SetItemText(nNew, OPERATOR_COL, GetOpName(sp.GetOperator()));

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
		SEARCHPARAM sp = m_aSearchParams[nRow];

		// delete rule
		m_aSearchParams.RemoveAt(nRow);
		DeleteItem(nRow);

		// reinsert rule
		nRow = InsertRule(nRow + 1, sp);
	
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

	SEARCHPARAM& sp = m_aSearchParams[nRow];

	switch (nCol)
	{
	case ATTRIB_COL:
		{
			ASSERT (&ctrl == &m_cbAttributes);
	
			m_cbAttributes.SelectAttribute(sp);
			CDialogHelper::RefreshMaxDropWidth(m_cbAttributes);
		}
		break;

	case OPERATOR_COL:
		{
			ASSERT (&ctrl == &m_cbOperators);

			m_cbOperators.ResetContent();
			
			FIND_ATTRIBTYPE nType = sp.GetAttribType();
			
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
			case FT_TIME:
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
				// fall thru
			case FT_DATE_REL:
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
			}
	
			CDialogHelper::SelectItemByData(m_cbOperators, (DWORD)sp.GetOperator());
		}
		break;

	case ANDOR_COL:
		{
			ASSERT (&ctrl == &m_cbAndOr);

			if (sp.GetAnd())
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
			if (!CDateHelper::IsDateSet(sp.ValueAsDate()))
			{
				sp.SetValue(COleDateTime::GetCurrentTime());
				SetItemText(nRow, nCol, sp.ValueAsDate().Format(VAR_DATEVALUEONLY));
			}
			else
			{
				m_dtcDate.SetTime(sp.ValueAsDouble());
			}
		}
		else if (&ctrl == &m_eTime)
		{
			m_eTime.SetTime(sp.ValueAsDouble(), TDC::MapUnitsToTHUnits((TDC_UNITS)sp.GetFlags()));
		}
		else if (&ctrl == &m_cbCustomIcons)
		{
			TDCCUSTOMATTRIBUTEDEFINITION attribDef;

			if (CTDCCustomAttributeHelper::GetAttributeDef(sp.GetAttribute(), m_aAttribDefs, attribDef))
			{
				ASSERT(attribDef.IsList());

 				CDialogHelper::SetComboBoxItems(m_cbCustomIcons, attribDef.aDefaultListData);

				CStringArray aIcons;
				Misc::Split(sp.ValueAsString(), aIcons);
				m_cbCustomIcons.SetChecked(aIcons);
			}

		}
		else if (&ctrl == &m_cbListValues)
		{
			m_cbListValues.ResetContent();

			TDCCUSTOMATTRIBUTEDEFINITION attribDef;

			if (CTDCCustomAttributeHelper::GetAttributeDef(sp.GetAttribute(), m_aAttribDefs, attribDef))
			{
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
			else
			{
				switch (sp.GetAttribute())
				{
				case TDCA_CATEGORY: CDialogHelper::SetComboBoxItems(m_cbListValues, m_tldListContents.aCategory); break;
				case TDCA_ALLOCTO:	CDialogHelper::SetComboBoxItems(m_cbListValues, m_tldListContents.aAllocTo); break;
				case TDCA_ALLOCBY:	CDialogHelper::SetComboBoxItems(m_cbListValues, m_tldListContents.aAllocBy); break;
				case TDCA_VERSION:	CDialogHelper::SetComboBoxItems(m_cbListValues, m_tldListContents.aVersion); break;
				case TDCA_TAGS:		CDialogHelper::SetComboBoxItems(m_cbListValues, m_tldListContents.aTags); break;
				case TDCA_STATUS:	CDialogHelper::SetComboBoxItems(m_cbListValues, m_tldListContents.aStatus); break;
				}
			}

			m_cbListValues.SelectString(0, sp.ValueAsString());
		}
		else if (&ctrl == &m_cbPriority)
		{
			m_cbPriority.SetSelectedPriority(sp.ValueAsInteger());
		}
		else if (&ctrl == &m_cbRisk)
		{
			m_cbRisk.SetSelectedRisk(sp.ValueAsInteger());
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
		SEARCHPARAM& sp = m_aSearchParams[nRow];
		SEARCHPARAM spNew(sp);

		if (m_cbAttributes.GetSelectedAttribute(spNew) && (sp != spNew))
		{
			sp = spNew;

			// update list text
			CString sAttrib = m_cbAttributes.GetSelectedAttributeText();
			SetItemText(nRow, ATTRIB_COL, sAttrib);

			// clear the operator cell text if the operator was no longer valid
			if (sp.OperatorIs(FOP_NONE))
				SetItemText(nRow, OPERATOR_COL, _T(""));

			// Always clear the text value
			sp.SetValue(_T(""));
			UpdateValueColumnText(nRow);
			
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

	SEARCHPARAM& sp = m_aSearchParams[nRow];

	switch (sp.GetAttribType())
	{
	case FT_STRING:
	case FT_DATE_REL:
	case FT_INTEGER:
	case FT_DOUBLE:
		sp.SetValue(pDispInfo->item.pszText);
		break;
		
	case FT_DATE:
	case FT_BOOL:
	case FT_ICON:
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
		const SEARCHPARAM& sp = m_aSearchParams[nParam];

		// attrib
		CString sAttrib = m_cbAttributes.GetAttributeName(sp);
		int nItem = InsertItem(nParam, sAttrib);

		// operator
		CString sOp = GetOpName(sp.GetOperator());
		SetItemText(nItem, OPERATOR_COL, sOp);

		// value
		UpdateValueColumnText(nItem);

		// and/or (but not for last row)
		if (nParam < GetRuleCount() - 1)
		{
			CEnString sAndOr(sp.GetAnd() ? IDS_FP_AND : IDS_FP_OR);
			SetItemText(nItem, ANDOR_COL, sAndOr);
		}
	}

	CRect rCombo;
	m_cbAttributes.GetWindowRect(rCombo);
	
	SetMinItemHeight(rCombo.Height());

	ValidateListData();
	SetCurSel(0);
}

void CTDLFindTaskExpressionListCtrl::RefreshAndOrColumnText()
{
	for (int nParam = 0; nParam < GetRuleCount(); nParam++)
	{
		const SEARCHPARAM& sp = m_aSearchParams[nParam];
		
		// and/or (but not for last row)
		if (nParam < GetRuleCount() - 1)
		{
			CEnString sAndOr(sp.GetAnd() ? IDS_FP_AND : IDS_FP_OR);
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

	const SEARCHPARAM& sp = m_aSearchParams[nRow];
	CString sValue;
		
	// value (but not boolean)
	if (sp.GetOperator() != FOP_SET && sp.GetOperator() != FOP_NOT_SET)
	{
		try
		{
			switch (sp.GetAttribType())
			{
			case FT_STRING:
			case FT_DATE_REL:
			case FT_INTEGER:
			case FT_DOUBLE:
			case FT_ICON:
				sValue = sp.ValueAsString();
				break;
				
			case FT_DATE:
				sValue = sp.ValueAsDate().Format(VAR_DATEVALUEONLY);
				break;
				
			case FT_TIME:
				sValue = CTimeHelper().FormatTime(sp.ValueAsDouble(), TDC::MapUnitsToTHUnits((TDC_UNITS)sp.GetFlags()), 2);
				break;
				
			case FT_BOOL:
				// handled by operator
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
}

void CTDLFindTaskExpressionListCtrl::OnTimeChange()
{
	// update the rule 
	int nRow = GetCurSel();
	SEARCHPARAM& rule = m_aSearchParams[nRow];

	rule.SetValue(m_eTime.GetTime());
	rule.SetFlags(TDC::MapTHUnitsToUnits(m_eTime.GetUnits()));

	UpdateValueColumnText(nRow);
}

LRESULT CTDLFindTaskExpressionListCtrl::OnTimeUnitsChange(WPARAM /*wp*/, LPARAM /*lp*/)
{
	// update the rule 
	int nRow = GetCurSel();
	SEARCHPARAM& rule = m_aSearchParams[nRow];

	rule.SetFlags(TDC::MapTHUnitsToUnits(m_eTime.GetUnits()));

	UpdateValueColumnText(nRow);

	return 0L;
}

void CTDLFindTaskExpressionListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CInputListCtrl::OnSize(nType, cx, cy);
	
	// resize columns by proportion
	SetColumnWidth(ATTRIB_COL, (int)(cx * COL_PROPORTIONS[ATTRIB_COL]));
	SetColumnWidth(OPERATOR_COL, (int)(cx * COL_PROPORTIONS[OPERATOR_COL]));
	SetColumnWidth(VALUE_COL, (int)(cx * COL_PROPORTIONS[VALUE_COL]));
	SetColumnWidth(ANDOR_COL, (int)(cx * COL_PROPORTIONS[ANDOR_COL]));
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
			EditCell(nItem, nCol);

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
			// if the focus is on the attrib list and the user hits the spacebar
			// then toggle the enabled state
			switch (pMsg->wParam)
			{
			case VK_F2:
				EditSelectedCell();
				break;

			case VK_DELETE:
				DeleteSelectedRule();
				break;

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
		const SEARCHPARAM& sp = m_aSearchParams[nRow];

		if (sp.HasIcon())
		{
			CStringArray aIcons;
			int nNumIcons = Misc::Split(sText, aIcons);

			CRect rIcon(rText);
			rIcon.DeflateRect(0, ((rText.Height() - 16) / 2));

			for (int nIcon = 0; nIcon < nNumIcons; nIcon++)
			{
				int nIconIdx = m_ilIcons.GetImageIndex(aIcons[nIcon]);
				
				if (nIconIdx != -1)
				{
					m_ilIcons.Draw(pDC, nIconIdx, rIcon.TopLeft(), ILD_TRANSPARENT);
					rIcon.left += 18;
				}
			}

			return;
		}
	}

	// all else
	CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sText, crText, nDrawTextFlags);
}
