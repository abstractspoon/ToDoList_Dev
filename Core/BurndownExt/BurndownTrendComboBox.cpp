// BurndownTrendComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BurndownTrendComboBox.h"
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

struct SORTITEM
{
	BURNDOWN_TREND nTrend;
	CString sLabel;
};

static int SortProc(const void* pV1, const void* pV2)
{
	const SORTITEM* pSI1 = (const SORTITEM*)pV1;
	const SORTITEM* pSI2 = (const SORTITEM*)pV2;

	return Misc::NaturalCompare(pSI1->sLabel, pSI2->sLabel);
}

/////////////////////////////////////////////////////////////////////////////
// CBurndownTrendComboBox

CBurndownTrendComboBox::CBurndownTrendComboBox()
{
}

CBurndownTrendComboBox::~CBurndownTrendComboBox()
{
}


BEGIN_MESSAGE_MAP(CBurndownTrendComboBox, CComboBox)
	//{{AFX_MSG_MAP(CBurndownTrendComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBurndownTrendComboBox message handlers

BOOL CBurndownTrendComboBox::Initialise()
{
	// Once only
	if (GetCount())
	{
		ASSERT(0);
		return FALSE;
	}

	// We build the combo in a specific order
	if (GetStyle() & CBS_SORT)
	{
		ASSERT(0);
		return FALSE;
	}

	// Build our own sorted array because we have numbers
	CArray<SORTITEM, SORTITEM&> aTrends;
	SORTITEM st;

	for (int nTrend = 0; nTrend < NUM_TRENDS; nTrend++)
	{
		st.nTrend = TRENDS[nTrend].nTrend;
		st.sLabel = CEnString(TRENDS[nTrend].nLabelID);

		aTrends.Add(st);
	}

	Misc::SortArrayT<SORTITEM>(aTrends, SortProc);

	for (int nItem = 0; nItem < aTrends.GetSize(); nItem++)
	{
		const SORTITEM& st = aTrends[nItem];
		CDialogHelper::AddString(*this, st.sLabel, st.nTrend);
	}

	CDialogHelper::RefreshMaxDropWidth(*this);

	CLocalizer::EnableTranslation(GetSafeHwnd(), FALSE);

	return TRUE;
}

void CBurndownTrendComboBox::DDX(CDataExchange* pDX, BURNDOWN_TREND& nTrend)
{
	CDialogHelper::DDX_CBData(pDX, *this, nTrend, BTL_NONE);
}
