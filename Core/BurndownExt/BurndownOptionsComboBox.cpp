// CBurndownOptionsComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BurndownOptionsComboBox.h"
#include "BurndownStatic.h"

#include "..\Shared\DialogHelper.h"
#include "..\Shared\Misc.h"
#include "..\Shared\Localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

struct COMBOOPTION
{
	UINT nLabelID;
	BURNDOWN_GRAPHOPTION nOption;
};

static const COMBOOPTION COMBOOPTIONS[] =
{
	{ IDS_TREND_NONE,			BGO_TREND_NONE },
	{ IDS_TREND_BESTFIT,		BGO_TREND_BESTFIT },
	{ IDS_TREND_7DAYAVERAGE,	BGO_TREND_7DAYAVERAGE },
	{ IDS_TREND_30DAYAVERAGE,	BGO_TREND_30DAYAVERAGE },
	{ IDS_TREND_90DAYAVERAGE,	BGO_TREND_90DAYAVERAGE },

	{ IDS_FREQUENCY_BAR,		BGO_FREQUENCY_BAR },
	{ IDS_FREQUENCY_LINE,		BGO_FREQUENCY_LINE },
	{ IDS_FREQUENCY_PIE,		BGO_FREQUENCY_PIE },
	{ IDS_FREQUENCY_DONUT,		BGO_FREQUENCY_DONUT },
};

static const int NUM_COMBO = sizeof(COMBOOPTIONS) / sizeof(COMBOOPTION);

/////////////////////////////////////////////////////////////////////////////

struct SORTITEM
{
	BURNDOWN_GRAPHOPTION nOption;
	CString sLabel;
};

static int SortProc(const void* pV1, const void* pV2)
{
	const SORTITEM* pSI1 = (const SORTITEM*)pV1;
	const SORTITEM* pSI2 = (const SORTITEM*)pV2;

	return Misc::NaturalCompare(pSI1->sLabel, pSI2->sLabel);
}

/////////////////////////////////////////////////////////////////////////////
// CCBurndownOptionsComboBox

CBurndownOptionsComboBox::CBurndownOptionsComboBox() : m_nGraphType(BCT_UNKNOWNTYPE)
{
}

CBurndownOptionsComboBox::~CBurndownOptionsComboBox()
{
}


BEGIN_MESSAGE_MAP(CBurndownOptionsComboBox, CComboBox)
	//{{AFX_MSG_MAP(CCBurndownOptionsComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCBurndownOptionsComboBox message handlers

BOOL CBurndownOptionsComboBox::SetActiveGraph(BURNDOWN_GRAPH nGraph)
{
	BURNDOWN_GRAPHTYPE nType = GetGraphType(nGraph);

	if (nType == BCT_UNKNOWNTYPE)
	{
		ASSERT(0);
		return FALSE;
	}
	else if (nType == m_nGraphType)
	{
		return TRUE;
	}

	// We build the combo in a specific order
	if (!GetSafeHwnd() || (GetStyle() & CBS_SORT))
	{
		ASSERT(0);
		return FALSE;
	}

	// Build our own sorted array because we have leading numbers
	CArray<SORTITEM, SORTITEM&> aOptions;
	int nItem;

	for (nItem = 0; nItem < NUM_COMBO; nItem++)
	{
		const COMBOOPTION& opt = COMBOOPTIONS[nItem];
		SORTITEM st;

		if (IsValidOption(opt.nOption, nType))
		{
			st.nOption = opt.nOption;
			st.sLabel = CEnString(COMBOOPTIONS[nItem].nLabelID);

			aOptions.Add(st);
		}
	}
	Misc::SortArrayT<SORTITEM>(aOptions, SortProc);

	// Cache selection and rebuild combo
	BURNDOWN_GRAPHOPTION nSelOpt = GetSelectedOption();
	ResetContent();
	
	for (nItem = 0; nItem < aOptions.GetSize(); nItem++)
	{
		const SORTITEM& st = aOptions[nItem];
		CDialogHelper::AddString(*this, st.sLabel, st.nOption);
	}

	CDialogHelper::RefreshMaxDropWidth(*this);
	CLocalizer::EnableTranslation(GetSafeHwnd(), FALSE);

	m_nGraphType = nType;

	// restore selection
	if (!IsValidOption(nSelOpt, nGraph))
		nSelOpt = GetDefaultOption(m_nGraphType);

	SetSelectedOption(nSelOpt);

	return TRUE;
}

BURNDOWN_GRAPHOPTION CBurndownOptionsComboBox::GetSelectedOption() const
{
	return (BURNDOWN_GRAPHOPTION)CDialogHelper::GetSelectedItemData(*this, BCT_UNKNOWNTYPE);
}

BOOL CBurndownOptionsComboBox::SetSelectedOption(BURNDOWN_GRAPHOPTION nOption)
{
	return (CDialogHelper::SelectItemByData(*this, nOption) != CB_ERR);
}
