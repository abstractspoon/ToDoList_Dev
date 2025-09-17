// KanbanAttributeComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KanbanAttributeComboBox.h"
#include "Kanbanenum.h"

#include "..\shared\dialoghelper.h"
#include "..\shared\enstring.h"
#include "..\shared\localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKanbanAttributeComboBox

CKanbanAttributeComboBox::CKanbanAttributeComboBox(BOOL bShowNone)
	: 
	m_bShowNone(bShowNone),
	m_nFixedColumnsAttribID(TDCA_NONE)
{
}

CKanbanAttributeComboBox::~CKanbanAttributeComboBox()
{
}


BEGIN_MESSAGE_MAP(CKanbanAttributeComboBox, CComboBox)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKanbanAttributeComboBox message handlers

void CKanbanAttributeComboBox::PreSubclassWindow() 
{
	CComboBox::PreSubclassWindow();

	CLocalizer::EnableTranslation(*this, FALSE);
	BuildCombo();
}

void CKanbanAttributeComboBox::BuildCombo()
{
	// Cache selection
	CString sSelCustID;
	TDC_ATTRIBUTE nSelAttrib = GetSelectedAttribute(sSelCustID);

	ResetContent();

	// Default attributes
	if (m_bShowNone)
		CDialogHelper::AddStringT(*this, CEnString(IDS_NONE), TDCA_NONE);

	CDialogHelper::AddStringT(*this, CEnString(IDS_STATUSATTRIB), TDCA_STATUS);
	CDialogHelper::AddStringT(*this, CEnString(IDS_ALLOCTOATTRIB), TDCA_ALLOCTO);
	CDialogHelper::AddStringT(*this, CEnString(IDS_ALLOCBYATTRIB), TDCA_ALLOCBY);
	CDialogHelper::AddStringT(*this, CEnString(IDS_CATEGORYATTRIB), TDCA_CATEGORY);
	CDialogHelper::AddStringT(*this, CEnString(IDS_PRIORITYATTRIB), TDCA_PRIORITY);
	CDialogHelper::AddStringT(*this, CEnString(IDS_RISKATTRIB), TDCA_RISK);
	CDialogHelper::AddStringT(*this, CEnString(IDS_VERSIONATTRIB), TDCA_VERSION);
	CDialogHelper::AddStringT(*this, CEnString(IDS_TAGSATTRIB), TDCA_TAGS);

	// Custom attributes
	for (int nCust = 0; nCust < m_aCustAttribDefs.GetSize(); nCust++)
	{
		const KANBANCUSTOMATTRIBDEF& kcaDef = m_aCustAttribDefs.GetData()[nCust];

		CEnString sCustAttrib;
		sCustAttrib.Format(IDS_CUSTOMATTRIB, kcaDef.sAttribName);

		CLocalizer::IgnoreString(sCustAttrib);
		CDialogHelper::AddStringT(*this, sCustAttrib, (TDCA_CUSTOMATTRIB + nCust));
	}

	if (m_nFixedColumnsAttribID != TDCA_NONE)
	{
		CString sAttrib = KBUtils::GetAttributeLabel(m_nFixedColumnsAttribID, KBCAL_LONG, m_aCustAttribDefs);

		if (sAttrib.IsEmpty())
			ASSERT(KBUtils::IsCustomAttribute(m_nFixedColumnsAttribID) && !m_aCustAttribDefs.GetSize());
		else
			CDialogHelper::AddStringT(*this, CEnString(IDS_FIXEDCOLUMNS, sAttrib), TDCA_FIXEDCOLUMNS);
	}

	// Restore selection
	if (!SetSelectedAttribute(nSelAttrib, sSelCustID))
		SetSelectedAttribute(GetFallbackAttribute(), _T(""));
}

void CKanbanAttributeComboBox::DDX(CDataExchange* pDX, TDC_ATTRIBUTE& value, CString& sCustomAttribID)
{
	if (pDX->m_bSaveAndValidate)
		value = GetSelectedAttribute(sCustomAttribID);
	else
		SetSelectedAttribute(value, sCustomAttribID);
}

void CKanbanAttributeComboBox::ShowFixedColumns(TDC_ATTRIBUTE nAttribID)
{
	if (nAttribID != m_nFixedColumnsAttribID)
	{
		m_nFixedColumnsAttribID = nAttribID;

		if (GetSafeHwnd())
			BuildCombo();
	}
}

TDC_ATTRIBUTE CKanbanAttributeComboBox::GetSelectedAttribute() const
{
	return CDialogHelper::GetSelectedItemDataT(*this, GetFallbackAttribute());
}

TDC_ATTRIBUTE CKanbanAttributeComboBox::GetSelectedAttribute(CString& sCustomAttribID) const
{
	TDC_ATTRIBUTE nSelAttribID = GetSelectedAttribute();

	if (KBUtils::IsCustomAttribute(nSelAttribID))
		sCustomAttribID = m_aCustAttribDefs.GetDefinitionID(nSelAttribID);
	else
		sCustomAttribID.Empty();

	return nSelAttribID;
}

BOOL CKanbanAttributeComboBox::SetSelectedAttribute(TDC_ATTRIBUTE nAttribID, const CString& sCustomAttribID)
{
	if (CDialogHelper::FindItemByDataT(*this, nAttribID) == -1)
		return (CB_ERR != CDialogHelper::SelectItemByDataT(*this, GetFallbackAttribute()));

	BOOL bCustom = (KBUtils::IsCustomAttribute(nAttribID) || ((nAttribID == TDCA_FIXEDCOLUMNS) && !sCustomAttribID.IsEmpty()));

	if ((bCustom && sCustomAttribID.IsEmpty()) || (!bCustom && !sCustomAttribID.IsEmpty()))
	{
		ASSERT(0);
		return FALSE;
	}

	if (bCustom)
		nAttribID = m_aCustAttribDefs.GetDefinitionID(sCustomAttribID);

	return (CB_ERR != CDialogHelper::SelectItemByDataT(*this, nAttribID));
}

void CKanbanAttributeComboBox::SetCustomAttributeDefs(const CKanbanCustomAttributeDefinitionArray& aAttribDefs)
{
	if (!Misc::MatchAllT(m_aCustAttribDefs, aAttribDefs, FALSE))
	{
		m_aCustAttribDefs.Copy(aAttribDefs);

		if (GetSafeHwnd())
			BuildCombo();
	}
}
