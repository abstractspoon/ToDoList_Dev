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

BOOL CTDLCustomAttribFeatureComboBox::SetAttributeDefintion(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef)
{
	ASSERT_VALID(this);

	if (!GetSafeHwnd())
		return FALSE;

	BuildCombo(attribDef);

	return (GetCount() > 0);
}

void CTDLCustomAttribFeatureComboBox::BuildCombo(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef)
{
	ResetContent();

	for (int nFeature = 0; nFeature < NUM_FEATURES; nFeature++)
	{
		const TDCFEATURE& feature = FEATURES[nFeature];

		if (attribDef.SupportsFeature(feature.dwFeature))
		{
			int nIndex = AddString(CEnString(feature.nStringID));

			if (nIndex != CB_ERR)
			{
				SetCheck(nIndex, attribDef.HasFeature(feature.dwFeature));
				SetItemData(nIndex, feature.dwFeature);
			}
		}
	}
}

DWORD CTDLCustomAttribFeatureComboBox::GetSelectedFeatures() const
{
	DWORD dwFeatures = 0;
	int nFeature = GetCount();

	while (nFeature--)
	{
		if (GetCheck(nFeature))
			dwFeatures |= GetItemData(nFeature);
	}

	return dwFeatures;
}

CString CTDLCustomAttribFeatureComboBox::FormatSelectedFeatures() const
{
	CStringArray aFeatures;

	if (GetChecked(aFeatures))
		return Misc::FormatArray(aFeatures);

	// else
	return _T("");
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
			SetCheckByData(TDCCAF_MAXIMIZE, FALSE);
			SetCheckByData(TDCCAF_MINIMIZE, FALSE);
			break;

		case TDCCAF_MAXIMIZE:
			SetCheckByData(TDCCAF_ACCUMULATE, FALSE);
			SetCheckByData(TDCCAF_MINIMIZE, FALSE);
			break;

		case TDCCAF_MINIMIZE:
			SetCheckByData(TDCCAF_MAXIMIZE, FALSE);
			SetCheckByData(TDCCAF_ACCUMULATE, FALSE);
			break;

		default:
			return;
		}
		
		// and list box if dropped
		if (GetDroppedState())
			::InvalidateRect(GetListbox(), NULL, FALSE);
	}
}
