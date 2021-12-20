// GanttDisplayComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "GanttDisplayComboBox.h"
#include "GanttCtrl.h"
#include "GanttStatic.h"

#include "..\Shared\DialogHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGanttDisplayComboBox

CGanttDisplayComboBox::CGanttDisplayComboBox() : CTabbedComboBox(-1), m_nMinEnabledDisplay(GTLC_DISPLAY_HOURS)
{
}

CGanttDisplayComboBox::~CGanttDisplayComboBox()
{
}


BEGIN_MESSAGE_MAP(CGanttDisplayComboBox, CComboBox)
	//{{AFX_MSG_MAP(CGanttDisplayComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT_EX(CBN_SELCHANGE, OnSelChange)
	ON_CONTROL_REFLECT_EX(CBN_SELENDOK, OnSelEndOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGanttDisplayComboBox message handlers

BOOL CGanttDisplayComboBox::SelectDisplay(GTLC_MONTH_DISPLAY nDisplay)
{
	return (CB_ERR != CDialogHelper::SelectItemByData(*this, nDisplay));
}

// External
GTLC_MONTH_DISPLAY CGanttDisplayComboBox::GetSelectedDisplay() const
{
	GTLC_MONTH_DISPLAY nDisplay = GetSelectedDisplayRaw();

	if (!IsEnabledDisplay(nDisplay))
		nDisplay = m_nMinEnabledDisplay;

	return nDisplay;
}

// Internal
GTLC_MONTH_DISPLAY CGanttDisplayComboBox::GetSelectedDisplayRaw() const
{
	return CDialogHelper::GetSelectedItemData(*this, GTLC_DISPLAY_NONE);
}

BOOL CGanttDisplayComboBox::IsEnabledDisplay(GTLC_MONTH_DISPLAY nDisplay) const
{
	return (GanttStatic::CompareDisplays(nDisplay, m_nMinEnabledDisplay) <= 0);
}

void CGanttDisplayComboBox::UpdateDisplayOptions(const CGanttCtrl& ctrl)
{
	GTLC_MONTH_DISPLAY nCurDisplay = ctrl.GetMonthDisplay(); // preserve selection
	BOOL bOneBasedDecades = !ctrl.HasOption(GTLCF_DECADESAREZEROBASED);

	ResetContent();

	// Build a list of all items and then separate the 
	// options from their examples, normalising tabs
	CStringArray aItems;
	int nItem;

	for (nItem = 0; nItem < NUM_DISPLAYMODES; nItem++)
		aItems.Add(CEnString(DISPLAYMODES[nItem].nStringID));

	CStringArray aOptions, aExamples;
	FixupTabOffsets(aItems, aOptions, aExamples);

	// Rebuild the items with hints for disabled items,
	// and recording the minimum valid option for later
	m_nMinEnabledDisplay = GTLC_DISPLAY_HOURS;

	for (nItem = 0; nItem < NUM_DISPLAYMODES; nItem++)
	{
		const GTCDISPLAYMODE& mode = DISPLAYMODES[nItem];
		aItems[nItem] = aOptions[nItem];

		if (!ctrl.CanSetMonthDisplay(mode.nDisplay))
		{
			if (m_nMinEnabledDisplay == GTLC_DISPLAY_HOURS)
				m_nMinEnabledDisplay = DISPLAYMODES[nItem - 1].nDisplay; // previous display

			aItems[nItem] += CEnString(IDS_DISPLAYHINT);
		}
		else
		{
			aItems[nItem] += aExamples[nItem];
		}

		CDialogHelper::AddString(*this, aItems[nItem], mode.nDisplay);
	}

	CDialogHelper::RefreshMaxDropWidth(*this, NULL, TABSTOPS);

	// Restore previous selection
	if (!IsEnabledDisplay(nCurDisplay))
		nCurDisplay = m_nMinEnabledDisplay;

	SelectDisplay(nCurDisplay);
}

void CGanttDisplayComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	GTLC_MONTH_DISPLAY nDisplay = (GTLC_MONTH_DISPLAY)lpDrawItemStruct->itemData;

	if (!IsEnabledDisplay(nDisplay))
		lpDrawItemStruct->itemState |= ODS_DISABLED;

	CTabbedComboBox::DrawItem(lpDrawItemStruct);
}

void CGanttDisplayComboBox::FixupTabOffsets(const CStringArray& aItems, CStringArray& aOptions, CStringArray& aExamples) const
{
	// Split the items at the location of their tab(s)
	// and normalise the option by adding a tab 
	int nItem = aItems.GetSize();

	aOptions.SetSize(nItem);
	aExamples.SetSize(nItem);

	while (nItem--)
	{
		CString sOption(aItems[nItem]), sExample;
		Misc::Split(sOption, sExample, _T("\t"));

		aOptions[nItem] = (sOption + '\t');
		aExamples[nItem] = sExample;
	}

	// Keep track of the widest option
	CClientDC dc(const_cast<CGanttDisplayComboBox*>(this));
	CFont* pOldFont = GraphicsMisc::PrepareDCFont(&dc, *this);

	CArray<int, int> aOptionWidths;
	aOptionWidths.SetSize(aOptions.GetSize());

	int nMaxLabelWidth = 0;
	CRect rOption(0, 0, 10000, 100);

	int nOption = aOptions.GetSize();

	while (nOption--)
	{
		dc.DrawText(aOptions[nOption], rOption, DT_EXPANDTABS | DT_CALCRECT);

		int nWidth = rOption.Width();
		aOptionWidths[nOption] = nWidth;

		nMaxLabelWidth = max(nMaxLabelWidth, nWidth);
	}

	// Now add tabs to each option until they all have the same length 
	dc.DrawText(_T("\t"), rOption, DT_EXPANDTABS | DT_CALCRECT);
	int nTabWidth = rOption.Width();

	nOption = aOptions.GetSize();

	while (nOption--)
	{
		int nWidth = aOptionWidths[nOption];

		while (nWidth < nMaxLabelWidth)
		{
			aOptions[nOption] += '\t';
			nWidth += nTabWidth;
		}
	}

	dc.SelectObject(pOldFont);
}

BOOL CGanttDisplayComboBox::OnSelChange()
{
	if (!IsEnabledDisplay(GetSelectedDisplayRaw()))
		SelectDisplay(m_nMinEnabledDisplay);

	return FALSE; // rout to parent
}

BOOL CGanttDisplayComboBox::OnSelEndOK()
{
	if (!IsEnabledDisplay(GetSelectedDisplayRaw()))
		SelectDisplay(m_nMinEnabledDisplay);

	return FALSE; // rout to parent
}
