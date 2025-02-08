// tdlfindtaskattributecombobox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "tdcstatic.h"
#include "tdlattributecombobox.h"

#include "..\shared\dialoghelper.h"
#include "..\shared\enstring.h"
#include "..\shared\localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeComboBox

CTDLAttributeComboBox::CTDLAttributeComboBox(BOOL bIncRelativeDates)
	:
	m_bIncRelativeDates(bIncRelativeDates)
{
}

CTDLAttributeComboBox::~CTDLAttributeComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLAttributeComboBox, CComboBox)
	//{{AFX_MSG_MAP(CTDLTaskAttributeComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeComboBox message handlers

void CTDLAttributeComboBox::SetCustomAttributes(const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	m_aAttribDefs.Copy(aAttribDefs);

	if (GetSafeHwnd())
		BuildCombo();
}

void CTDLAttributeComboBox::SetAttributeFilter(const CTDCAttributeMap& mapAttrib)
{
	if (!m_mapWantedAttrib.MatchAll(mapAttrib))
	{
		m_mapWantedAttrib.Copy(mapAttrib);
		BuildCombo();
	}
}

BOOL CTDLAttributeComboBox::SetSelectedAttribute(TDC_ATTRIBUTE nAttribID, BOOL bRelative)
{
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

void CTDLAttributeComboBox::BuildCombo()
{
	TDC_ATTRIBUTE nSelAttrib = GetSelectedAttribute();
	ResetContent();

	CLocalizer::EnableTranslation(*this, FALSE);

	int nAtt;
	for (nAtt = 0; nAtt < ATTRIB_COUNT; nAtt++)
	{
		const TDCATTRIBUTE& ap = TASKATTRIBUTES[nAtt];

		if (ap.nAttribResID && WantAttribute(ap.nAttributeID))
		{
			CEnString sAttrib(ap.nAttribResID);
			DWORD dwItemData = EncodeItemData(ap.nAttributeID);

			CDialogHelper::AddStringT(*this, sAttrib, dwItemData); 

			// relative dates
			if (m_bIncRelativeDates && AttributeIsDate(ap.nAttributeID))
			{
				// then add relative version too
				dwItemData = EncodeItemData(ap.nAttributeID, TRUE);

				sAttrib += ' ';
				sAttrib += CEnString(IDS_TDLBC_RELATIVESUFFIX);

				CDialogHelper::AddStringT(*this, sAttrib, dwItemData); 
			}
		}
	}

	// custom attributes
	for (nAtt = 0; nAtt < m_aAttribDefs.GetSize(); nAtt++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aAttribDefs[nAtt];

		if (WantAttribute(attribDef.GetAttributeID()))
		{
			CEnString sAttrib(IDS_CUSTOMCOLUMN, attribDef.sLabel);
			TDC_ATTRIBUTE attrib = attribDef.GetAttributeID();

			DWORD dwItemData = EncodeItemData(attrib);
			CDialogHelper::AddStringT(*this, sAttrib, dwItemData);

			// is it a date
			if (m_bIncRelativeDates && AttributeIsDate(attrib))
			{
				// then add relative version too
				dwItemData = EncodeItemData(attrib, TRUE);
				sAttrib.Format(IDS_CUSTOMRELDATECOLUMN, attribDef.sLabel);

				CDialogHelper::AddStringT(*this, sAttrib, dwItemData);
			}
		}
	}

	// Misc others
	if (WantAttribute(TDCA_REMINDER))
		CDialogHelper::AddStringT(*this, IDS_TDLBC_REMINDER, EncodeItemData(TDCA_REMINDER));

	if (WantAttribute(TDCA_TODAY))
		CDialogHelper::AddStringT(*this, IDS_TODAY, EncodeItemData(TDCA_TODAY));

	// restore selection
	SetSelectedAttribute(nSelAttrib);

	// recalc combo drop width
	CDialogHelper::RefreshMaxDropWidth(*this);
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
	return (m_mapWantedAttrib.IsEmpty() || m_mapWantedAttrib.Has(nAttribID));
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
