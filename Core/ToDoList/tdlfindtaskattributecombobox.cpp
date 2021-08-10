// tdlfindtaskattributecombobox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "tdcstatic.h"
#include "tdlfindtaskattributecombobox.h"
#include "TDCSearchParamHelper.h"

#include "..\shared\dialoghelper.h"
#include "..\shared\enstring.h"
#include "..\shared\localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLFindTaskAttributeComboBox

CTDLFindTaskAttributeComboBox::CTDLFindTaskAttributeComboBox()
{
}

CTDLFindTaskAttributeComboBox::~CTDLFindTaskAttributeComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLFindTaskAttributeComboBox, CComboBox)
	//{{AFX_MSG_MAP(CTDLFindTaskAttributeComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLFindTaskAttributeComboBox message handlers

void CTDLFindTaskAttributeComboBox::SetCustomAttributes(const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	m_aAttribDefs.Copy(aAttribDefs);

	if (GetSafeHwnd())
		BuildCombo();
}

BOOL CTDLFindTaskAttributeComboBox::SelectAttribute(const SEARCHPARAM& rule)
{
	DWORD dwItemData = EncodeItemData(rule.GetAttribute(), rule.GetFlags());

	return (CB_ERR != CDialogHelper::SelectItemByData(*this, dwItemData));
}

BOOL CTDLFindTaskAttributeComboBox::SelectedAttributeIsDate() const
{
	return AttributeIsDate(GetSelectedAttribute());
}

TDC_ATTRIBUTE CTDLFindTaskAttributeComboBox::GetSelectedAttribute() const
{
	TDC_ATTRIBUTE nAttrib = TDCA_NONE;
	int nSel = GetCurSel();

	if (nSel != CB_ERR)
	{
		BOOL bRelative = FALSE;
		DWORD dwItemData = GetItemData(nSel);

		DecodeItemData(dwItemData, nAttrib, bRelative);
	}

	return nAttrib;
}

BOOL CTDLFindTaskAttributeComboBox::GetSelectedAttribute(SEARCHPARAM& rule) const
{
	int nSel = GetCurSel();

	if (nSel != CB_ERR)
	{
		FIND_ATTRIBTYPE nType = FT_NONE;
		TDC_ATTRIBUTE nAttribID = TDCA_NONE;
		BOOL bRelative = FALSE;

		DWORD dwItemData = GetItemData(nSel);
		DecodeItemData(dwItemData, nAttribID, bRelative);

		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			CString sUniqueID = m_aAttribDefs.GetAttributeTypeID(nAttribID);
			nType = CTDCSearchParamHelper::GetAttributeFindType(sUniqueID, bRelative, m_aAttribDefs);

			rule.SetCustomAttribute(nAttribID, sUniqueID, nType);
		}
		else
		{
			nType = SEARCHPARAM::GetAttribType(nAttribID, bRelative);
			rule.SetAttribute(nAttribID, nType);
		}

		return TRUE;
	}

	// else
	return FALSE;
}

CString CTDLFindTaskAttributeComboBox::GetSelectedAttributeText() const
{
	int nSel = GetCurSel();
	CString sItem;

	if (nSel != CB_ERR)
		GetLBText(nSel, sItem);

	return sItem;

}

void CTDLFindTaskAttributeComboBox::BuildCombo()
{
	ResetContent();

	CLocalizer::EnableTranslation(*this, FALSE);

	int nAtt;
	for (nAtt = 0; nAtt < ATTRIB_COUNT; nAtt++)
	{
		const TDCATTRIBUTE& ap = ATTRIBUTES[nAtt];

		if (ap.nAttribResID)
		{
			CEnString sAttrib(ap.nAttribResID);
			DWORD dwItemData = EncodeItemData(ap.nAttribID);

			CDialogHelper::AddString(*this, sAttrib, dwItemData); 

			// is it a date
			if (AttributeIsDate(ap.nAttribID))
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
		CEnString sAttrib(IDS_CUSTOMCOLUMN, attribDef.sLabel);
		TDC_ATTRIBUTE nAttribID = attribDef.GetAttributeID();

		DWORD dwItemData = EncodeItemData(nAttribID);
		CDialogHelper::AddString(*this, sAttrib, dwItemData); 

		// is it a date
		if (AttributeIsDate(nAttribID))
		{
			// then add relative version too
			dwItemData = EncodeItemData(nAttribID, TRUE);
			sAttrib.Format(IDS_CUSTOMRELDATECOLUMN, attribDef.sLabel);
			
			CDialogHelper::AddString(*this, sAttrib, dwItemData); 
		}
	}

	// Misc others
	CDialogHelper::AddString(*this, IDS_TDLBC_REMINDER, EncodeItemData(TDCA_REMINDER));

	// recalc combo drop width
	CDialogHelper::RefreshMaxDropWidth(*this);
}

CString CTDLFindTaskAttributeComboBox::GetAttributeName(const SEARCHPARAM& rule) const
{
	TDC_ATTRIBUTE nAttribID = rule.GetAttribute();
	CEnString sName;

	switch (nAttribID)
	{
	case TDCA_PATH:
		sName.LoadString(IDS_TDC_COLUMN_PATH);
		break;

	case TDCA_REMINDER:
		sName.LoadString(IDS_TDLBC_REMINDER);
		break;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			// try custom attributes
			int nAttrib = m_aAttribDefs.GetSize();

			while (nAttrib--)
			{
				const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aAttribDefs[nAttrib];

				if (attribDef.GetAttributeID() == nAttribID)
				{
					if (attribDef.IsDataType(TDCCA_DATE) && (rule.GetAttribType() == FT_DATERELATIVE))
						sName.Format(IDS_CUSTOMRELDATECOLUMN, attribDef.sLabel);
					else
						sName.Format(IDS_CUSTOMCOLUMN, attribDef.sLabel);
					break;
				}
			}

		}
		else // default attribute
		{
			int nAttrib = ATTRIB_COUNT;

			while (nAttrib--)
			{
				if (ATTRIBUTES[nAttrib].nAttribID == nAttribID)
				{
					if (ATTRIBUTES[nAttrib].nAttribResID)
					{
						// handle relative dates
						sName.LoadString(ATTRIBUTES[nAttrib].nAttribResID);

						if (AttributeIsDate(nAttribID) && rule.IsRelativeDate())
						{
							sName += ' ';
							sName += CEnString(IDS_TDLBC_RELATIVESUFFIX);
						}
						
						break;
					}
				}
			}
		}
		break;

	}

	ASSERT(!sName.IsEmpty()); // not found

	return sName;
}

DWORD CTDLFindTaskAttributeComboBox::EncodeItemData(TDC_ATTRIBUTE nAttribID, BOOL bRelative) const
{
	// sanity check
	if (!AttributeIsDate(nAttribID))
		bRelative = FALSE;

	return MAKELONG(nAttribID, bRelative);
}

BOOL CTDLFindTaskAttributeComboBox::AttributeIsTime(TDC_ATTRIBUTE nAttribID) const
{
	switch (nAttribID)
	{
	case TDCA_TIMEESTIMATE:
	case TDCA_TIMESPENT:
		return TRUE;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_DEF_RET(m_aAttribDefs, nAttribID, pDef, FALSE);

			return pDef->IsDataType(TDCCA_TIMEPERIOD);
		}
		break;
	}

	// all else
	return FALSE;
}

BOOL CTDLFindTaskAttributeComboBox::AttributeIsDate(TDC_ATTRIBUTE nAttribID) const
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
		{
			const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
			GET_DEF_RET(m_aAttribDefs, nAttribID, pDef, FALSE);

			return pDef->IsDataType(TDCCA_DATE);
		}
		break;
	}

	// all else
	return FALSE;
}

void CTDLFindTaskAttributeComboBox::DecodeItemData(DWORD dwItemData, TDC_ATTRIBUTE& nAttribID, BOOL& bRelative) const
{
	nAttribID = (TDC_ATTRIBUTE)LOWORD(dwItemData);
	bRelative = (BOOL)HIWORD(dwItemData);

	// sanity check
	ASSERT (!bRelative || AttributeIsDate(nAttribID));
}
