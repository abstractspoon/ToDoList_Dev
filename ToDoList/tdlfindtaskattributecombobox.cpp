// tdlfindtaskattributecombobox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "tdcstatic.h"
#include "tdlfindtaskattributecombobox.h"
#include "TDCCustomAttributeHelper.h"

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

BOOL CTDLFindTaskAttributeComboBox::SelectAttribute(const SEARCHPARAM& sp)
{
	DWORD dwItemData = EncodeItemData(sp.GetAttribute(), sp.GetFlags());

	return CDialogHelper::SelectItemByData(*this, dwItemData);
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

BOOL CTDLFindTaskAttributeComboBox::GetSelectedAttribute(SEARCHPARAM& sp) const
{
	int nSel = GetCurSel();

	if (nSel != CB_ERR)
	{
		FIND_ATTRIBTYPE nType = FT_NONE;
		TDC_ATTRIBUTE nAttrib = TDCA_NONE;
		BOOL bRelative = FALSE;

		DWORD dwItemData = GetItemData(nSel);
		DecodeItemData(dwItemData, nAttrib, bRelative);

		if (SEARCHPARAM::IsCustomAttribute(nAttrib))
		{
			CString sUniqueID = CTDCCustomAttributeHelper::GetAttributeTypeID(nAttrib, m_aAttribDefs);
			nType = CTDCCustomAttributeHelper::GetAttributeFindType(sUniqueID, bRelative, m_aAttribDefs);

			sp.SetCustomAttribute(nAttrib, sUniqueID, nType);
		}
		else
		{
			nType = SEARCHPARAM::GetAttribType(nAttrib, bRelative);
			sp.SetAttribute(nAttrib, nType);
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

	int nAttrib;
	for (nAttrib = 0; nAttrib < ATTRIB_COUNT; nAttrib++)
	{
		const TDCATTRIBUTE& ap = ATTRIBUTES[nAttrib];

		if (ap.nAttribResID)
		{
			CEnString sAttrib(ap.nAttribResID);
			DWORD dwItemData = EncodeItemData(ap.attrib);

			CDialogHelper::AddString(*this, sAttrib, dwItemData); 

			// is it a date
			// then add relative version too
			if (AttributeIsDate(ap.attrib))
			{
				dwItemData = EncodeItemData(ap.attrib, TRUE);

				sAttrib += ' ';
				sAttrib += CEnString(IDS_TDLBC_RELATIVESUFFIX);

				CDialogHelper::AddString(*this, sAttrib, dwItemData); 
			}
		}
	}

	// custom attributes
	for (nAttrib = 0; nAttrib < m_aAttribDefs.GetSize(); nAttrib++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aAttribDefs[nAttrib];
		CEnString sAttrib(IDS_CUSTOMCOLUMN, attribDef.sLabel);
		TDC_ATTRIBUTE attrib = attribDef.GetAttributeID();

		DWORD dwItemData = EncodeItemData(attrib);
		CDialogHelper::AddString(*this, sAttrib, dwItemData); 

		// is it a date
		if (AttributeIsDate(attrib))
		{
			dwItemData = EncodeItemData(attrib, TRUE);
			sAttrib.Format(IDS_CUSTOMRELDATECOLUMN, attribDef.sLabel);
			
			CDialogHelper::AddString(*this, sAttrib, dwItemData); 
		}
	}

	// recalc combo drop width
	CDialogHelper::RefreshMaxDropWidth(*this);
}

CString CTDLFindTaskAttributeComboBox::GetAttributeName(const SEARCHPARAM& sp) const
{
	TDC_ATTRIBUTE attrib = sp.GetAttribute();
	BOOL bRelative = sp.GetFlags();
	CEnString sName;

	switch (attrib)
	{
	case TDCA_PATH:
		sName.LoadString(IDS_TDC_COLUMN_PATH);
		break;

	default:
		if (CTDCCustomAttributeHelper::IsCustomAttribute(attrib))
		{
			// try custom attributes
			int nAttrib = m_aAttribDefs.GetSize();

			while (nAttrib--)
			{
				if (m_aAttribDefs[nAttrib].GetAttributeID() == attrib)
				{
					if (AttributeIsDate(attrib) && bRelative)
					{
						sName.Format(IDS_CUSTOMRELDATECOLUMN, m_aAttribDefs[nAttrib].sLabel);
					}
					else
					{
						sName.Format(IDS_CUSTOMCOLUMN, m_aAttribDefs[nAttrib].sLabel);
					}
					break;
				}
			}

		}
		else // default attribute
		{
			int nAttrib = ATTRIB_COUNT;

			while (nAttrib--)
			{
				if (ATTRIBUTES[nAttrib].attrib == attrib)
				{
					if (ATTRIBUTES[nAttrib].nAttribResID)
					{
						// handle relative dates
						sName.LoadString(ATTRIBUTES[nAttrib].nAttribResID);

						if (AttributeIsDate(attrib) && bRelative)
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

DWORD CTDLFindTaskAttributeComboBox::EncodeItemData(TDC_ATTRIBUTE nAttrib, BOOL bRelative) const
{
	// sanity check
	if (!AttributeIsDate(nAttrib))
		bRelative = FALSE;

	return MAKELONG(nAttrib, bRelative);
}

BOOL CTDLFindTaskAttributeComboBox::AttributeIsTime(TDC_ATTRIBUTE attrib) const
{
	switch (attrib)
	{
	case TDCA_TIMEEST:
	case TDCA_TIMESPENT:
		return TRUE;
	}

	// all else
	return FALSE;
}

BOOL CTDLFindTaskAttributeComboBox::AttributeIsDate(TDC_ATTRIBUTE attrib) const
{
	switch (attrib)
	{
	case TDCA_DONEDATE:
	case TDCA_DUEDATE:
	case TDCA_STARTDATE:
	case TDCA_LASTMOD:
	case TDCA_CREATIONDATE:
		return TRUE;
		
	default:
		if (CTDCCustomAttributeHelper::IsCustomAttribute(attrib))
		{
			TDCCUSTOMATTRIBUTEDEFINITION attribDef;
			
			// check for user date attributes
			if (CTDCCustomAttributeHelper::GetAttributeDef(attrib, m_aAttribDefs, attribDef))
			{
				return (attribDef.GetDataType() == TDCCA_DATE);
			}
		}
		break;
	}

	// all else
	return FALSE;
}

void CTDLFindTaskAttributeComboBox::DecodeItemData(DWORD dwItemData, TDC_ATTRIBUTE& nAttrib, BOOL& bRelative) const
{
	nAttrib = (TDC_ATTRIBUTE)LOWORD(dwItemData);
	bRelative = (BOOL)HIWORD(dwItemData);

	// sanity check
	ASSERT (!bRelative || AttributeIsDate(nAttrib));
}
