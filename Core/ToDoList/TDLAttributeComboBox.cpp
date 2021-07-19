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

BOOL CTDLAttributeComboBox::SetSelectedAttribute(TDC_ATTRIBUTE nAttrib, BOOL bRelative) const
{
	DWORD dwItemData = EncodeItemData(nAttrib, bRelative);

	return (CDialogHelper::FindItemByData(*this, dwItemData) != CB_ERR);
}

TDC_ATTRIBUTE CTDLAttributeComboBox::GetSelectedAttribute() const
{
	BOOL bUnused;
	return GetSelectedAttribute(bUnused);
}

TDC_ATTRIBUTE CTDLAttributeComboBox::GetSelectedAttribute(BOOL& bRelative) const
{
	int nSel = GetCurSel();

	TDC_ATTRIBUTE nAttrib = TDCA_NONE;
	bRelative = FALSE;

	if (nSel != CB_ERR)
	{
		DWORD dwItemData = GetItemData(nSel);
		DecodeItemData(dwItemData, nAttrib, bRelative);
	}

	return nAttrib;
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
	ResetContent();

	CLocalizer::EnableTranslation(*this, FALSE);

	int nAtt;
	for (nAtt = 0; nAtt < ATTRIB_COUNT; nAtt++)
	{
		const TDCATTRIBUTE& ap = ATTRIBUTES[nAtt];

		if (ap.nAttribResID && WantAttribute(ap.nAttribID))
		{
			CEnString sAttrib(ap.nAttribResID);
			DWORD dwItemData = EncodeItemData(ap.nAttribID);

			CDialogHelper::AddString(*this, sAttrib, dwItemData); 

			// relative dates
			if (m_bIncRelativeDates && AttributeIsDate(ap.nAttribID))
			{
				// then add relative version too
				dwItemData = EncodeItemData(ap.nAttribID, TRUE);

				sAttrib += ' ';
				sAttrib += CEnString(IDS_TDLBC_RELATIVESUFFIX);

				CDialogHelper::AddString(*this, sAttrib, dwItemData); 
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
			CDialogHelper::AddString(*this, sAttrib, dwItemData);

			// is it a date
			if (m_bIncRelativeDates && AttributeIsDate(attrib))
			{
				// then add relative version too
				dwItemData = EncodeItemData(attrib, TRUE);
				sAttrib.Format(IDS_CUSTOMRELDATECOLUMN, attribDef.sLabel);

				CDialogHelper::AddString(*this, sAttrib, dwItemData);
			}
		}
	}

	// Misc others
	if (WantAttribute(TDCA_REMINDER))
		CDialogHelper::AddString(*this, IDS_TDLBC_REMINDER, EncodeItemData(TDCA_REMINDER));

	// recalc combo drop width
	CDialogHelper::RefreshMaxDropWidth(*this);
}

DWORD CTDLAttributeComboBox::EncodeItemData(TDC_ATTRIBUTE nAttrib, BOOL bRelative) const
{
	// sanity check
	if (!AttributeIsDate(nAttrib))
		bRelative = FALSE;

	return MAKELONG(nAttrib, bRelative);
}

BOOL CTDLAttributeComboBox::AttributeIsTimePeriod(TDC_ATTRIBUTE nAttrib) const
{
	switch (nAttrib)
	{
	case TDCA_TIMEESTIMATE:
	case TDCA_TIMESPENT:
		return TRUE;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttrib))
		{
			TDCCUSTOMATTRIBUTEDEFINITION attribDef;

			// check for user date attributes
			if (m_aAttribDefs.GetAttributeDef(nAttrib, attribDef))
			{
				return (attribDef.GetDataType() == TDCCA_TIMEPERIOD);
			}
		}
		break;
	}

	// all else
	return FALSE;
}

BOOL CTDLAttributeComboBox::WantAttribute(TDC_ATTRIBUTE nAttrib) const
{
	return (m_mapWantedAttrib.IsEmpty() || m_mapWantedAttrib.Has(nAttrib));
}

BOOL CTDLAttributeComboBox::AttributeIsDate(TDC_ATTRIBUTE nAttrib) const
{
	switch (nAttrib)
	{
	case TDCA_DONEDATE:
	case TDCA_DUEDATE:
	case TDCA_STARTDATE:
	case TDCA_LASTMODDATE:
	case TDCA_CREATIONDATE:
		return TRUE;
		
	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttrib))
		{
			TDCCUSTOMATTRIBUTEDEFINITION attribDef;
			
			// check for user date attributes
			if (m_aAttribDefs.GetAttributeDef(nAttrib, attribDef))
			{
				return (attribDef.GetDataType() == TDCCA_DATE);
			}
		}
		break;
	}

	// all else
	return FALSE;
}

void CTDLAttributeComboBox::DecodeItemData(DWORD dwItemData, TDC_ATTRIBUTE& nAttrib, BOOL& bRelative) const
{
	nAttrib = (TDC_ATTRIBUTE)LOWORD(dwItemData);
	bRelative = (BOOL)HIWORD(dwItemData);

	// sanity check
	ASSERT (!bRelative || AttributeIsDate(nAttrib));
}
