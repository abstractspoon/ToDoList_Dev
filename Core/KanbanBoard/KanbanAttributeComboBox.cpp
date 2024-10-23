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

CKanbanAttributeComboBox::CKanbanAttributeComboBox() 
	: 
	m_bShowCustomAttrib(TRUE), 
	m_bShowFixedColumns(FALSE)
{
}

CKanbanAttributeComboBox::~CKanbanAttributeComboBox()
{
}


BEGIN_MESSAGE_MAP(CKanbanAttributeComboBox, CComboBox)
	//{{AFX_MSG_MAP(CKanbanAttributeComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
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

#ifdef _DEBUG
		//sCustAttrib += Misc::Format(_T(" [%ld]"), (TDCA_CUSTOMATTRIB + nCust));
#endif

		CLocalizer::IgnoreString(sCustAttrib);
		CDialogHelper::AddStringT(*this, sCustAttrib, (TDCA_CUSTOMATTRIB + nCust));
	}

	if (m_bShowFixedColumns)
		CDialogHelper::AddStringT(*this, CEnString(IDS_FIXEDCOLUMNS), TDCA_FIXEDCOLUMNS);

	// Restore selection
	if (!SetSelectedAttribute(nSelAttrib, sSelCustID))
		SetSelectedAttribute(TDCA_STATUS, _T(""));
}

void CKanbanAttributeComboBox::DDX(CDataExchange* pDX, TDC_ATTRIBUTE& value, CString& sCustomAttribID)
{
	if (pDX->m_bSaveAndValidate)
		value = GetSelectedAttribute(sCustomAttribID);
	else
		SetSelectedAttribute(value, sCustomAttribID);
}

void CKanbanAttributeComboBox::ShowFixedColumns(BOOL bShow)
{
	if (!m_bShowFixedColumns != !bShow) // normalise
	{
		m_bShowFixedColumns = bShow;

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
	TDC_ATTRIBUTE nSelAttrib = CDialogHelper::GetSelectedItemDataT(*this, GetFallbackAttribute());

	if (KBUtils::IsCustomAttribute(nSelAttrib))
		sCustomAttribID = m_aCustAttribDefs.GetDefinitionID(nSelAttrib);
	else
		sCustomAttribID.Empty();

	return nSelAttrib;
}

BOOL CKanbanAttributeComboBox::SetSelectedAttribute(TDC_ATTRIBUTE nAttribID, const CString& sCustomAttribID)
{
	BOOL bCustom = KBUtils::IsCustomAttribute(nAttribID);

	if ((bCustom && sCustomAttribID.IsEmpty()) || (!bCustom && !sCustomAttribID.IsEmpty()))
	{
		ASSERT(0);
		return FALSE;
	}

	if (bCustom)
		nAttribID = m_aCustAttribDefs.GetDefinitionID(sCustomAttribID);

	return (CB_ERR != CDialogHelper::SelectItemByDataT(*this, nAttribID));
}

void CKanbanAttributeComboBox::SetAttributeDefinitions(const CKanbanCustomAttributeDefinitionArray& aAttribDefs)
{
	if (Misc::MatchAllT(m_aCustAttribDefs, aAttribDefs, FALSE))
		return;

	m_aCustAttribDefs.Copy(aAttribDefs);

	if (GetSafeHwnd())
		BuildCombo();
}
