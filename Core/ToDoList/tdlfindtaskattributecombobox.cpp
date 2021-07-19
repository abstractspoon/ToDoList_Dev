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
	: 
	CTDLAttributeComboBox(TRUE) // want relative dates
{
}

CTDLFindTaskAttributeComboBox::~CTDLFindTaskAttributeComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLFindTaskAttributeComboBox, CTDLAttributeComboBox)
	//{{AFX_MSG_MAP(CTDLFindTaskAttributeComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLFindTaskAttributeComboBox message handlers

BOOL CTDLFindTaskAttributeComboBox::SelectAttribute(const SEARCHPARAM& rule)
{
	DWORD dwItemData = EncodeItemData(rule.GetAttribute(), rule.GetFlags());

	return (CB_ERR != CDialogHelper::SelectItemByData(*this, dwItemData));
}

BOOL CTDLFindTaskAttributeComboBox::SelectedAttributeIsDate() const
{
	return AttributeIsDate(CTDLAttributeComboBox::GetSelectedAttribute());
}

BOOL CTDLFindTaskAttributeComboBox::GetSelectedAttribute(SEARCHPARAM& rule) const
{
	BOOL bRelative = FALSE;
	TDC_ATTRIBUTE nAttrib = CTDLAttributeComboBox::GetSelectedAttribute(bRelative);

	if (nAttrib != TDCA_NONE)
	{
		FIND_ATTRIBTYPE nType = FT_NONE;

		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttrib))
		{
			CString sUniqueID = m_aAttribDefs.GetAttributeTypeID(nAttrib);
			nType = CTDCSearchParamHelper::GetAttributeFindType(sUniqueID, bRelative, m_aAttribDefs);

			rule.SetCustomAttribute(nAttrib, sUniqueID, nType);
		}
		else
		{
			nType = SEARCHPARAM::GetAttribType(nAttrib, bRelative);
			rule.SetAttribute(nAttrib, nType);
		}

		return TRUE;
	}

	// else
	return FALSE;
}

CString CTDLFindTaskAttributeComboBox::GetAttributeName(const SEARCHPARAM& rule) const
{
	TDC_ATTRIBUTE attrib = rule.GetAttribute();
	CEnString sName;

	switch (attrib)
	{
	case TDCA_PATH:
		sName.LoadString(IDS_TDC_COLUMN_PATH);
		break;

	case TDCA_REMINDER:
		sName.LoadString(IDS_TDLBC_REMINDER);
		break;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(attrib))
		{
			// try custom attributes
			int nAttrib = m_aAttribDefs.GetSize();

			while (nAttrib--)
			{
				if (m_aAttribDefs[nAttrib].GetAttributeID() == attrib)
				{
					if (m_aAttribDefs[nAttrib].IsDataType(TDCCA_DATE) && 
						(rule.GetAttribType() == FT_DATERELATIVE))
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
				if (ATTRIBUTES[nAttrib].nAttribID == attrib)
				{
					if (ATTRIBUTES[nAttrib].nAttribResID)
					{
						// handle relative dates
						sName.LoadString(ATTRIBUTES[nAttrib].nAttribResID);

						if (AttributeIsDate(attrib) && rule.IsRelativeDate())
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

