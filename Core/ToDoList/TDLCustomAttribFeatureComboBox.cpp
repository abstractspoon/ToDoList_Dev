// TDLCustomAttribFeatureComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "tdcenum.h"
#include "TDLCustomAttribFeatureComboBox.h"

#include "..\shared\misc.h"
#include "..\shared\dialoghelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

struct TDCFEATURE
{
	UINT nStringID;
	DWORD dwFeature;
};

const TDCFEATURE FEATURES[] = 
{
	{ IDS_CUSTOM_FEATURE_SORTABLE,		TDCCAF_SORT },
	{ IDS_CUSTOM_FEATURE_ACCUMULATED,	TDCCAF_ACCUMULATE },
	{ IDS_CUSTOM_FEATURE_MINIMUM,		TDCCAF_MINIMIZE },
	{ IDS_CUSTOM_FEATURE_MAXIMUM,		TDCCAF_MAXIMIZE },
	{ IDS_CUSTOM_FEATURE_HIDEZERO,		TDCCAF_HIDEZERO },
	{ IDS_CUSTOM_FEATURE_SHOWTIME,		TDCCAF_SHOWTIME },
	{ IDS_CUSTOM_FEATURE_FILTER,		TDCCAF_FILTER },
	{ IDS_CUSTOM_FEATURE_INHERITANCE,	TDCCAF_INHERITPARENTCHANGES },
	{ IDS_CUSTOM_FEATURE_PERCENTAGE,	TDCCAF_DISPLAYASPERCENT },
	{ IDS_CUSTOM_FEATURE_NOBLANK,		TDCCAF_EXCLUDEBLANKITEM },
	{ IDS_CUSTOM_FEATURE_ONEDECIMAL,	TDCCAF_ONEDECIMAL },
	{ IDS_CUSTOM_FEATURE_SHOWEDITFIELD,	TDCCAF_SHOWEDITFIELD },
};
const int NUM_FEATURES = sizeof(FEATURES) / sizeof(TDCFEATURE);

/////////////////////////////////////////////////////////////////////////////
// CTDLCustomAttribFeatureComboBox

CTDLCustomAttribFeatureComboBox::CTDLCustomAttribFeatureComboBox()
{
}

CTDLCustomAttribFeatureComboBox::~CTDLCustomAttribFeatureComboBox()
{
}

BEGIN_MESSAGE_MAP(CTDLCustomAttribFeatureComboBox, CCheckComboBox)
	//{{AFX_MSG_MAP(CTDLCustomAttribFeatureComboBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLCustomAttribFeatureComboBox message handlers

BOOL CTDLCustomAttribFeatureComboBox::SetAttributeDefinition(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef)
{
	return SetAttributeDefinition(attribDef.GetDataType(), attribDef.GetListType(), attribDef.dwFeatures);
}

BOOL CTDLCustomAttribFeatureComboBox::SetAttributeDefinition(DWORD dwDataType, DWORD dwListType, DWORD dwSelectedFeatures)
{
	ASSERT_VALID(this);

	if (!GetSafeHwnd())
		return FALSE;

	BuildCombo(dwDataType, dwListType, dwSelectedFeatures);

	return (GetCount() > 0);
}

void CTDLCustomAttribFeatureComboBox::BuildCombo(DWORD dwDataType, DWORD dwListType, DWORD dwSelectedFeatures)
{
	ResetContent();

	for (int nFeature = 0; nFeature < NUM_FEATURES; nFeature++)
	{
		const TDCFEATURE& feature = FEATURES[nFeature];

		if (TDCCUSTOMATTRIBUTEDEFINITION::AttributeSupportsFeature(dwDataType, dwListType, feature.dwFeature))
			CDialogHelper::AddString(*this, feature.nStringID, feature.dwFeature);
	}

	SetCheckedByItemData(dwSelectedFeatures);
	EnableTooltip();
}

DWORD CTDLCustomAttribFeatureComboBox::GetSelectedFeatures() const
{
	return GetCheckedItemData();
}

CString CTDLCustomAttribFeatureComboBox::FormatFeatureList(DWORD dwFeatures)
{
	CStringArray aFeatures;

	for (int nFeature = 0; nFeature < NUM_FEATURES; nFeature++)
	{
		const TDCFEATURE& feature = FEATURES[nFeature];

		if (dwFeatures & feature.dwFeature)
			aFeatures.Add(CEnString(feature.nStringID));
	}

	// sort and format to match combo
	if (aFeatures.GetSize())
	{
		Misc::SortArray(aFeatures);
		return Misc::FormatArray(aFeatures);
	}

	// else
	return _T("");
}

void CTDLCustomAttribFeatureComboBox::OnCheckChange(int nIndex)
{
	// if setting then turn off mutually exclusive options
	if (GetCheck(nIndex))
	{
		DWORD dwFeature = GetItemData(nIndex);

		switch (dwFeature)
		{
		case TDCCAF_ACCUMULATE:
			SetCheckByItemData(TDCCAF_MAXIMIZE, CCBC_UNCHECKED);
			SetCheckByItemData(TDCCAF_MINIMIZE, CCBC_UNCHECKED);
			break;

		case TDCCAF_MAXIMIZE:
			SetCheckByItemData(TDCCAF_ACCUMULATE, CCBC_UNCHECKED);
			SetCheckByItemData(TDCCAF_MINIMIZE, CCBC_UNCHECKED);
			break;

		case TDCCAF_MINIMIZE:
			SetCheckByItemData(TDCCAF_MAXIMIZE, CCBC_UNCHECKED);
			SetCheckByItemData(TDCCAF_ACCUMULATE, CCBC_UNCHECKED);
			break;

		default:
			return;
		}
		
		// and list box if dropped
		if (GetDroppedState())
			::InvalidateRect(GetListbox(), NULL, FALSE);
	}
}
