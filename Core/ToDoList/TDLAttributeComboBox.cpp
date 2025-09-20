// tdlfindtaskattributecombobox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "tdcstatic.h"
#include "tdlattributecombobox.h"

#include "..\shared\dialoghelper.h"
#include "..\shared\enstring.h"
#include "..\shared\localizer.h"
#include "..\shared\misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeComboBox

CTDLAttributeComboBox::CTDLAttributeComboBox(DWORD dwOptions)
	:
	m_dwOptions(dwOptions)
{
}

CTDLAttributeComboBox::~CTDLAttributeComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLAttributeComboBox, COwnerdrawComboBoxBase)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeComboBox message handlers

void CTDLAttributeComboBox::SetCustomAttributes(const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	if (!Misc::MatchAllT(aAttribDefs, m_aAttribDefs, FALSE))
	{
		m_aAttribDefs.Copy(aAttribDefs);
		RebuildCombo();
	}
}

void CTDLAttributeComboBox::SetAttributeFilter(const CTDCAttributeMap& mapAttrib)
{
	if (!m_mapWantedAttrib.MatchAll(mapAttrib))
	{
		m_mapWantedAttrib.Copy(mapAttrib);
		RebuildCombo();
	}
}

BOOL CTDLAttributeComboBox::SetSelectedAttribute(TDC_ATTRIBUTE nAttribID, BOOL bRelative)
{
	CheckBuildCombo();

	DWORD dwItemData = EncodeItemData(nAttribID, bRelative);

	return (CDialogHelper::SelectItemByDataT(*this, dwItemData) != CB_ERR);
}

BOOL CTDLAttributeComboBox::SetSelectedAttribute(const CString& sCustAttribID, BOOL bRelative)
{
	TDC_ATTRIBUTE nAttribID = m_aAttribDefs.GetAttributeID(sCustAttribID);

	if (!TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
	{
		ASSERT(0);
		return FALSE;
	}

	return SetSelectedAttribute(nAttribID, bRelative);
}

TDC_ATTRIBUTE CTDLAttributeComboBox::GetSelectedAttribute() const
{
	BOOL bUnused;
	return GetSelectedAttribute(bUnused);
}

TDC_ATTRIBUTE CTDLAttributeComboBox::GetSelectedAttribute(CString& sCustAttribID) const
{
	TDC_ATTRIBUTE nAttribID = GetSelectedAttribute();

	if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
	{
		sCustAttribID = m_aAttribDefs.GetAttributeTypeID(nAttribID);
		ASSERT(!sCustAttribID.IsEmpty());
	}
	else
	{
		sCustAttribID.Empty();
	}

	return nAttribID;
}

TDC_ATTRIBUTE CTDLAttributeComboBox::GetSelectedAttribute(BOOL& bRelative) const
{
	int nSel = GetCurSel();

	TDC_ATTRIBUTE nAttribID = TDCA_NONE;
	bRelative = FALSE;

	if (nSel != CB_ERR)
	{
		DWORD dwItemData = GetItemData(nSel);
		DecodeItemData(dwItemData, nAttribID, bRelative);
	}

	return nAttribID;
}

void CTDLAttributeComboBox::DDX(CDataExchange* pDX, TDC_ATTRIBUTE& nAttribID)
{
	if (pDX->m_bSaveAndValidate)
		nAttribID = GetSelectedAttribute();
	else
		SetSelectedAttribute(nAttribID);
}

void CTDLAttributeComboBox::DDX(CDataExchange* pDX, TDC_ATTRIBUTE& nAttribID, CString& sCustAttribID)
{
	if (pDX->m_bSaveAndValidate)
	{
		nAttribID = GetSelectedAttribute(sCustAttribID);
	}
	else
	{
		if (sCustAttribID.IsEmpty())
			SetSelectedAttribute(nAttribID);
		else
			SetSelectedAttribute(sCustAttribID);
	}
}

CString CTDLAttributeComboBox::GetSelectedAttributeText() const
{
	int nSel = GetCurSel();
	CString sItem;

	if (nSel != CB_ERR)
		GetLBText(nSel, sItem);

	return sItem;
}

void CTDLAttributeComboBox::CheckAddItem(TDC_ATTRIBUTE nAttribID, UINT nStrResID, CSortItemArray& aItems)
{
	if (!nStrResID || !WantAttribute(nAttribID))
		return;

	AddItem(CEnString(nStrResID), nAttribID, aItems);
}

void CTDLAttributeComboBox::CheckAddItem(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, CSortItemArray& aItems)
{
	if (!WantAttribute(attribDef.GetAttributeID()))
		return;

	CEnString sItem;

	if (m_dwOptions & TDLACB_GROUPCUSTOMATTRIBS)
		sItem = attribDef.sLabel; // No need for suffix
	else
		sItem.Format(IDS_CUSTOMCOLUMN, attribDef.sLabel);

	AddItem(sItem, attribDef.GetAttributeID(), aItems);
}

void CTDLAttributeComboBox::AddItem(const CString& sItem, TDC_ATTRIBUTE nAttribID, CSortItemArray& aItems)
{
	SORTITEM si;

	si.sItem = sItem;
	si.nAttribID = nAttribID;
	si.bRelativeDate = FALSE;

	aItems.Add(si);

	if (Misc::HasFlag(m_dwOptions, TDLACB_INCRELATIVEDATES) && AttributeIsDate(nAttribID))
	{
		si.bRelativeDate = TRUE;

		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID) && !Misc::HasFlag(m_dwOptions, TDLACB_GROUPCUSTOMATTRIBS))
			si.sItem = CEnString().Format(IDS_CUSTOMRELDATECOLUMN, si.sItem);
		else 
			si.sItem += (' ' + CEnString(IDS_TDLBC_RELATIVESUFFIX));

		aItems.Add(si);
	}
}

void CTDLAttributeComboBox::BuildCombo()
{
	ASSERT(GetSafeHwnd());
	ASSERT(GetCount() == 0);

	TDC_ATTRIBUTE nSelAttrib = GetSelectedAttribute();

	CArray<SORTITEM, SORTITEM&> aItems, aCustomItems;
	SORTITEM si;

	// Built-in attributes
	CheckAddItem(TDCA_REMINDER, IDS_TDLBC_REMINDER, aItems);

	for (int nAtt = 0; nAtt < ATTRIB_COUNT; nAtt++)
	{
		const TDCATTRIBUTE& ap = TASKATTRIBUTES[nAtt];
		CheckAddItem(ap.nAttributeID, ap.nLabelResID, aItems);
	}

	// custom attributes
	for (int nCust = 0; nCust < m_aAttribDefs.GetSize(); nCust++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aAttribDefs[nCust];
		CheckAddItem(attribDef, aCustomItems);
	}

	// Rebuild combo
	CLocalizer::EnableTranslation(*this, FALSE);

	ResetContent();

	// Today is always first regardless
	if (WantAttribute(TDCA_TODAY))
		CDialogHelper::AddStringT(*this, CEnString(IDS_TODAY), EncodeItemData(TDCA_TODAY));

	if (Misc::HasFlag(m_dwOptions, TDLACB_GROUPCUSTOMATTRIBS) && aCustomItems.GetSize())
	{
		ASSERT(!HasStyle(CBS_SORT));
		ASSERT(HasStyle(CBS_OWNERDRAWFIXED));
		ASSERT(HasStyle(CBS_HASSTRINGS));

		int nItem = AddString(CEnString(IDS_TDLBC_CUSTOMATTRIBS));
		SetHeadingItem(nItem);

		Misc::SortArrayT<SORTITEM>(aCustomItems, SortProc);
		AddItemsToCombo(aCustomItems);

		nItem = AddString(CEnString(IDS_DEFAULTATTRIBUTES));
		SetHeadingItem(nItem);
	}
	else
	{
		aItems.Append(aCustomItems);
	}

	Misc::SortArrayT<SORTITEM>(aItems, SortProc);
	AddItemsToCombo(aItems);

	// restore selection
	SetSelectedAttribute(nSelAttrib);

	// recalc combo drop width
	CDialogHelper::RefreshMaxDropWidth(*this);
}

void CTDLAttributeComboBox::AddItemsToCombo(const CSortItemArray& aItems)
{
	for (int nItem = 0; nItem < aItems.GetSize(); nItem++)
	{
		const SORTITEM& si = aItems[nItem];

		CDialogHelper::AddStringT(*this, si.sItem, EncodeItemData(si.nAttribID, si.bRelativeDate));
	}
}

int CTDLAttributeComboBox::SortProc(const void* v1, const void* v2)
{
	const SORTITEM* pSI1 = (SORTITEM*)v1;
	const SORTITEM* pSI2 = (SORTITEM*)v2;

	if (pSI1->nAttribID == TDCA_TODAY)
		return -1;

	if (pSI2->nAttribID == TDCA_TODAY)
		return 1;

	return Misc::NaturalCompare(pSI1->sItem, pSI2->sItem);
}

DWORD CTDLAttributeComboBox::EncodeItemData(TDC_ATTRIBUTE nAttribID, BOOL bRelative) const
{
	// sanity check
	if (!AttributeIsDate(nAttribID))
		bRelative = FALSE;

	return MAKELONG(nAttribID, bRelative);
}

BOOL CTDLAttributeComboBox::AttributeIsTimePeriod(TDC_ATTRIBUTE nAttribID) const
{
	switch (nAttribID)
	{
	case TDCA_TIMEESTIMATE:
	case TDCA_TIMESPENT:
		return TRUE;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
			return (m_aAttribDefs.GetAttributeDataType(nAttribID) == TDCCA_TIMEPERIOD);
		break;
	}

	// all else
	return FALSE;
}

BOOL CTDLAttributeComboBox::WantAttribute(TDC_ATTRIBUTE nAttribID) const
{
	return (m_mapWantedAttrib.IsEmpty() || m_mapWantedAttrib.HasAttribOrAll(nAttribID));
}

BOOL CTDLAttributeComboBox::AttributeIsDate(TDC_ATTRIBUTE nAttribID) const
{
	switch (nAttribID)
	{
	case TDCA_DONEDATE:
	case TDCA_DUEDATE:
	case TDCA_STARTDATE:
	case TDCA_LASTMODDATE:
	case TDCA_CREATIONDATE:
		return TRUE;
		
	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
			return (m_aAttribDefs.GetAttributeDataType(nAttribID) == TDCCA_DATE);
		break;
	}

	// all else
	return FALSE;
}

void CTDLAttributeComboBox::DecodeItemData(DWORD dwItemData, TDC_ATTRIBUTE& nAttribID, BOOL& bRelative) const
{
	nAttribID = (TDC_ATTRIBUTE)LOWORD(dwItemData);
	bRelative = (BOOL)HIWORD(dwItemData);

	// sanity check
	ASSERT (!bRelative || AttributeIsDate(nAttribID));
}

void CTDLAttributeComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
						  DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	CRect rItem(rect);

	// Don't indent <today> when grouped
	if (bList && 
		(dwItemData == TDCA_TODAY) && 
		Misc::HasFlag(m_dwOptions, TDLACB_GROUPCUSTOMATTRIBS))
	{
		rItem.left = 0;
	}

	COwnerdrawComboBoxBase::DrawItemText(dc, rItem, nItem, nItemState, dwItemData, sItem, bList, crText);
}
