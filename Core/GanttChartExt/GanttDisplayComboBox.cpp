// GanttDisplayComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
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

CGanttDisplayComboBox::CGanttDisplayComboBox()
{
}

CGanttDisplayComboBox::~CGanttDisplayComboBox()
{
}


BEGIN_MESSAGE_MAP(CGanttDisplayComboBox, CComboBox)
	//{{AFX_MSG_MAP(CGanttDisplayComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGanttDisplayComboBox message handlers

BOOL CGanttDisplayComboBox::SelectDisplay(GTLC_MONTH_DISPLAY nDisplay)
{
	return (CB_ERR != CDialogHelper::SelectItemByData(*this, nDisplay));
}

GTLC_MONTH_DISPLAY CGanttDisplayComboBox::GetSelectedDisplay() const
{
	return CDialogHelper::GetSelectedItemData(*this, GTLC_DISPLAY_NONE);
}

void CGanttDisplayComboBox::UpdateDisplayOptions(const CGanttCtrl& ctrl)
{
	GTLC_MONTH_DISPLAY nCurDisplay = ctrl.GetMonthDisplay(); // preserve selection
	BOOL bOneBasedDecades = !ctrl.HasOption(GTLCF_DECADESAREZEROBASED);

	ResetContent();

	for (int nMode = 0; nMode < NUM_DISPLAYMODES; nMode++)
	{
		const GTCDISPLAYMODE& mode = DISPLAYMODES[nMode];

		if (!ctrl.CanSetMonthDisplay(mode.nDisplay))
		{
			int nCurMode = GanttStatic::FindDisplay(nCurDisplay);

			if (nMode < nCurMode)
				nCurDisplay = mode.nDisplay;

			break;
		}

		// else
		CEnString sItemText(mode.nStringID);

		// Handle one-based decades
		if (bOneBasedDecades)
		{
			if (mode.nDisplay == GTLC_DISPLAY_DECADES)
			{
				sItemText.Replace(_T("2000-2009"), _T("2001-2010"));
			}
			else if (mode.nDisplay == GTLC_DISPLAY_QUARTERCENTURIES)
			{
				sItemText.Replace(_T("2000-2024"), _T("2001-2025"));
			}
		}

		CDialogHelper::AddString(*this, sItemText, mode.nDisplay);
	}

	CDialogHelper::RefreshMaxDropWidth(*this);

	SelectDisplay(nCurDisplay);
}
