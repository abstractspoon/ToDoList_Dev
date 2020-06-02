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
	DWORD dwOption;
};

static const COMBOOPTION TIMESERIESOPTIONS[] =
{
	{ IDS_TREND_NONE,			BGO_TIMESERIES_TREND_NONE },
	{ IDS_TREND_BESTFIT,		BGO_TIMESERIES_TREND_BESTFIT },
	{ IDS_TREND_7DAYAVERAGE,	BGO_TIMESERIES_TREND_7DAYAVERAGE },
	{ IDS_TREND_30DAYAVERAGE,	BGO_TIMESERIES_TREND_30DAYAVERAGE },
	{ IDS_TREND_90DAYAVERAGE,	BGO_TIMESERIES_TREND_90DAYAVERAGE },
};
static const int NUM_TIMEOPTIONS = sizeof(TIMESERIESOPTIONS) / sizeof(COMBOOPTION);

static const COMBOOPTION FREQUENCYOPTIONS[] =
{
	{ IDS_FREQUENCY_BAR,		BGO_FREQUENCY_STYLE_BAR },
	{ IDS_FREQUENCY_LINE,		BGO_FREQUENCY_STYLE_LINE },
	{ IDS_FREQUENCY_PIE,		BGO_FREQUENCY_STYLE_PIE },
	{ IDS_FREQUENCY_DONUT,		BGO_FREQUENCY_STYLE_DONUT },
};
static const int NUM_FREQOPTIONS = sizeof(FREQUENCYOPTIONS) / sizeof(COMBOOPTION);

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
	VERIFY(BuildSortedList(nType, aOptions));

	// Cache selection and rebuild combo
	DWORD dwSelOpt = GetSelectedOption();
	ResetContent();
	
	for (int nItem = 0; nItem < aOptions.GetSize(); nItem++)
	{
		const SORTITEM& st = aOptions[nItem];
		CDialogHelper::AddString(*this, st.sLabel, st.dwOption);
	}

	CDialogHelper::RefreshMaxDropWidth(*this);
	CLocalizer::EnableTranslation(GetSafeHwnd(), FALSE);

	m_nGraphType = nType;

	// restore selection
	if (!IsValidOption(dwSelOpt, nType))
		dwSelOpt = GetDefaultOption(nType);

	SetSelectedOption(dwSelOpt);

	return TRUE;
}

int CBurndownOptionsComboBox::BuildSortedList(BURNDOWN_GRAPHTYPE nType, CArray<SORTITEM, SORTITEM&>& aOptions)
{
	aOptions.RemoveAll();

	const COMBOOPTION* pOptions = NULL;
	int nNumOpt = 0;

	switch (nType)
	{
	case BCT_TIMESERIES:
		pOptions = TIMESERIESOPTIONS;
		nNumOpt = NUM_TIMEOPTIONS;
		break;

	case BCT_FREQUENCY:
		pOptions = FREQUENCYOPTIONS;
		nNumOpt = NUM_FREQOPTIONS;
		break;

	default:
		return 0;
	}

	for (int nOpt = 0; nOpt < nNumOpt; nOpt++)
	{
		const COMBOOPTION& opt = pOptions[nOpt];
		SORTITEM st;

		st.dwOption = opt.dwOption;
		st.sLabel = CEnString(opt.nLabelID);

		aOptions.Add(st);
	}
	Misc::SortArrayT<SORTITEM>(aOptions, SortProc);

	return aOptions.GetSize();
}

int CBurndownOptionsComboBox::SortProc(const void* pV1, const void* pV2)
{
	const SORTITEM* pSI1 = (const SORTITEM*)pV1;
	const SORTITEM* pSI2 = (const SORTITEM*)pV2;

	return Misc::NaturalCompare(pSI1->sLabel, pSI2->sLabel);
}

DWORD CBurndownOptionsComboBox::GetSelectedOption() const
{
	return (DWORD)CDialogHelper::GetSelectedItemData(*this, BCT_UNKNOWNTYPE);
}

BOOL CBurndownOptionsComboBox::SetSelectedOption(DWORD dwOption)
{
	ASSERT(IsValidOption(dwOption, m_nGraphType));

	switch (m_nGraphType)
	{
	case BCT_TIMESERIES:
		dwOption &= BGO_TIMESERIES_TREND_MASK;
		break;

	case BCT_FREQUENCY:
		dwOption &= BGO_FREQUENCY_STYLE_MASK;
		break;

	default:
		return 0;
	}

	return (CDialogHelper::SelectItemByData(*this, dwOption) != CB_ERR);
}
