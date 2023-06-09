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

struct ATTRIBUTE
{
	UINT nIDLabel;
	TDC_ATTRIBUTE nAttrib;
};

static ATTRIBUTE DEFAULTATTRIB[]
{
	{ IDS_STATUSATTRIB,		TDCA_STATUS },
	{ IDS_ALLOCTOATTRIB,	TDCA_ALLOCTO },
	{ IDS_ALLOCBYATTRIB,	TDCA_ALLOCBY },
	{ IDS_CATEGORYATTRIB,	TDCA_CATEGORY },
	{ IDS_PRIORITYATTRIB,	TDCA_PRIORITY },
	{ IDS_RISKATTRIB,		TDCA_RISK },
	{ IDS_VERSIONATTRIB,	TDCA_VERSION },
	{ IDS_TAGSATTRIB,		TDCA_TAGS },
};

const int NUM_DEFAULTATTRIB = sizeof(DEFAULTATTRIB) / sizeof(ATTRIBUTE);

/////////////////////////////////////////////////////////////////////////////
// CKanbanAttributeComboBox

CKanbanAttributeComboBox::CKanbanAttributeComboBox(BOOL bIncludeNone) 
	: 
	m_bIncludeNone(bIncludeNone),
	m_bShowCustomAttrib(TRUE), 
	m_bShowFixedColumns(FALSE),
	m_nExcludeAttribID(TDCA_NONE)
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

	BuildCombo();
}

void CKanbanAttributeComboBox::ExcludeAttribute(TDC_ATTRIBUTE nAttrib, const CString& sCustomAttribID)
{
	if (nAttrib != m_nExcludeAttribID || sCustomAttribID != m_sExcludeCustomAttribID)
	{
		m_nExcludeAttribID = nAttrib;
		m_sExcludeCustomAttribID != sCustomAttribID;

		BuildCombo();
	}
}

void CKanbanAttributeComboBox::BuildCombo()
{
	// Cache selection
	CString sSelCustID;
	TDC_ATTRIBUTE nSelAttrib = GetSelectedAttribute(sSelCustID);

	ResetContent();

	if (m_bIncludeNone)
		CDialogHelper::AddString(*this, CEnString(IDS_NONE), TDCA_NONE);

	// Default attributes
	for (int nAtt = 0; nAtt < NUM_DEFAULTATTRIB; nAtt++)
	{
		const ATTRIBUTE attrib = DEFAULTATTRIB[nAtt];

		if (attrib.nAttrib != m_nExcludeAttribID)
			CDialogHelper::AddString(*this, CEnString(attrib.nIDLabel), attrib.nAttrib);
	}

	// Custom attributes
	for (int nCust = 0; nCust < m_aCustAttribDefs.GetSize(); nCust++)
	{
		const KANBANCUSTOMATTRIBDEF& kcaDef = m_aCustAttribDefs.GetData()[nCust];

		if (kcaDef.sAttribID != m_sExcludeCustomAttribID)
		{
			CEnString sCustAttrib;
			sCustAttrib.Format(IDS_CUSTOMATTRIB, kcaDef.sAttribName);

			CLocalizer::IgnoreString(sCustAttrib);
			CDialogHelper::AddString(*this, sCustAttrib, (TDCA_CUSTOMATTRIB + nCust));
		}
	}

	if (m_bShowFixedColumns)
		CDialogHelper::AddString(*this, CEnString(IDS_FIXEDCOLUMNS), TDCA_FIXEDCOLUMNS);

	// Restore selection
	if (!SetSelectedAttribute(nSelAttrib, sSelCustID))
	{
		if (m_bIncludeNone)
			SetSelectedAttribute(TDCA_NONE, _T(""));
		else
			SetSelectedAttribute(TDCA_STATUS, _T(""));
	}
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

TDC_ATTRIBUTE CKanbanAttributeComboBox::GetSelectedAttribute(CString& sCustomAttribID) const
{
	TDC_ATTRIBUTE nSelAttrib = CDialogHelper::GetSelectedItemData(*this, TDCA_STATUS);

	if ((nSelAttrib >= TDCA_CUSTOMATTRIB_FIRST) && (nSelAttrib <= TDCA_CUSTOMATTRIB_LAST))
	{
		int nCust = (nSelAttrib - TDCA_CUSTOMATTRIB_FIRST);
		ASSERT( nCust < m_aCustAttribDefs.GetSize());

		sCustomAttribID = m_aCustAttribDefs[nCust].sAttribID;
		nSelAttrib = TDCA_CUSTOMATTRIB;
	}
	else
	{	
		sCustomAttribID.Empty();
	}

	return nSelAttrib;
}

BOOL CKanbanAttributeComboBox::SetSelectedAttribute(TDC_ATTRIBUTE nAttrib, const CString& sCustomAttribID)
{
	BOOL bCustom = ((nAttrib >= TDCA_CUSTOMATTRIB_FIRST) && (nAttrib <= TDCA_CUSTOMATTRIB_LAST));

	if ((bCustom && sCustomAttribID.IsEmpty()) || (!bCustom && !sCustomAttribID.IsEmpty()))
	{
		ASSERT(0);
		return FALSE;
	}

	int nSelItem = CB_ERR;

	if (bCustom)
	{
		int nCust = m_aCustAttribDefs.FindDefinition(sCustomAttribID);

		if (nCust == -1)
			return FALSE;

		nAttrib = (TDC_ATTRIBUTE)(TDCA_CUSTOMATTRIB + nCust);
	}

	return (CB_ERR != CDialogHelper::SelectItemByData(*this, nAttrib));
}

void CKanbanAttributeComboBox::SetAttributeDefinitions(const CKanbanCustomAttributeDefinitionArray& aAttribDefs)
{
	if (Misc::MatchAllT(m_aCustAttribDefs, aAttribDefs, FALSE))
		return;

	m_aCustAttribDefs.Copy(aAttribDefs);

	if (GetSafeHwnd())
		BuildCombo();
}
