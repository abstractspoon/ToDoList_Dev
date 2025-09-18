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
	CTDLAttributeComboBox(TDLACB_INCRELATIVEDATES | TDLACB_GROUPCUSTOMATTRIBS)
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

void CTDLFindTaskAttributeComboBox::BuildCombo()
{
	CTDLAttributeComboBox::BuildCombo();

	BOOL bHadHeadings = (m_nNumHeadings > 0);

	// Add grouping to the head of the list
	int nItem = InsertString(0, CEnString(IDS_FIND_GROUPHEADER));
	SetHeadingItem(nItem);

	CDialogHelper::InsertStringT(*this, 1, IDS_FIND_GROUPSTART, EncodeItemData(TDCA_MATCHGROUPSTART));
	CDialogHelper::InsertStringT(*this, 2, IDS_FIND_GROUPEND, EncodeItemData(TDCA_MATCHGROUPEND));

	if (!bHadHeadings)
	{
		nItem = InsertString(3, CEnString(IDS_ATTRIBUTES));
		SetHeadingItem(nItem);
	}
}

BOOL CTDLFindTaskAttributeComboBox::WantAttribute(TDC_ATTRIBUTE nAttribID) const
{
	if (nAttribID == TDCA_TODAY) 
		return FALSE;
	
	// All else
	return CTDLAttributeComboBox::WantAttribute(nAttribID);
}

BOOL CTDLFindTaskAttributeComboBox::SelectAttribute(const SEARCHPARAM& rule)
{
	DWORD dwItemData = EncodeItemData(rule.GetAttribute(), rule.GetFlags());

	return (CB_ERR != CDialogHelper::SelectItemByDataT(*this, dwItemData));
}

BOOL CTDLFindTaskAttributeComboBox::SelectedAttributeIsDate() const
{
	return AttributeIsDate(CTDLAttributeComboBox::GetSelectedAttribute());
}

BOOL CTDLFindTaskAttributeComboBox::GetSelectedAttribute(SEARCHPARAM& rule) const
{
	BOOL bRelative = FALSE;
	TDC_ATTRIBUTE nAttribID = CTDLAttributeComboBox::GetSelectedAttribute(bRelative);

	if (nAttribID != TDCA_NONE)
	{
		FIND_ATTRIBTYPE nType = FT_NONE;

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

	case TDCA_MATCHGROUPSTART:
		sName = _T("<Begin Group>");
		break;

	case TDCA_MATCHGROUPEND:
		sName = _T("<End Group>");
		break;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			// try custom attributes
			int nAtt = m_aAttribDefs.GetSize();

			while (nAtt--)
			{
				const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aAttribDefs[nAtt];

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
			sName = TDC::GetAttributeLabel(nAttribID);

			// handle relative dates
			if (!sName.IsEmpty() && AttributeIsDate(nAttribID) && rule.IsRelativeDate(FALSE))
			{
				sName += ' ';
				sName += CEnString(IDS_TDLBC_RELATIVESUFFIX);
			}
		}
		break;

	}

	ASSERT(!sName.IsEmpty()); // not found

	return sName;
}

