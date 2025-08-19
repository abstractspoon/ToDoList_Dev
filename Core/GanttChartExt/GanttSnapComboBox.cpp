// GanttSnapComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "GanttSnapComboBox.h"
#include "GanttCtrl.h"
#include "GanttStatic.h"

#include "..\Shared\DialogHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGanttSnapComboBox

CGanttSnapComboBox::CGanttSnapComboBox()
{
}

CGanttSnapComboBox::~CGanttSnapComboBox()
{
}


BEGIN_MESSAGE_MAP(CGanttSnapComboBox, CComboBox)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGanttSnapComboBox message handlers

void CGanttSnapComboBox::Rebuild(GTLC_MONTH_DISPLAY nDisplay, GTLC_SNAPMODE nSnap)
{
	ResetContent();

	switch (nDisplay)
	{
	case GTLC_DISPLAY_QUARTERCENTURIES:
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTHALFYEAR, GTLCSM_NEARESTHALFYEAR);
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTYEAR, GTLCSM_NEARESTYEAR);
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTDECADE, GTLCSM_NEARESTDECADE);
		break;

	case GTLC_DISPLAY_DECADES:
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTMONTH, GTLCSM_NEARESTMONTH);
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTHALFYEAR, GTLCSM_NEARESTHALFYEAR);
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTYEAR, GTLCSM_NEARESTYEAR);
		break;

	case GTLC_DISPLAY_YEARS:
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTMONTH, GTLCSM_NEARESTMONTH);
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTHALFYEAR, GTLCSM_NEARESTHALFYEAR);
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTYEAR, GTLCSM_NEARESTYEAR);
		break;

	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTMONTH, GTLCSM_NEARESTMONTH);
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTQUARTER, GTLCSM_NEARESTQUARTER);
		break;

	case GTLC_DISPLAY_MONTHS_SHORT:
	case GTLC_DISPLAY_MONTHS_MID:
	case GTLC_DISPLAY_MONTHS_LONG:
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTDAY, GTLCSM_NEARESTDAY);
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTMONTH, GTLCSM_NEARESTMONTH);
		break;

	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTHALFDAY, GTLCSM_NEARESTHALFDAY);
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTDAY, GTLCSM_NEARESTDAY);
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTWEEK, GTLCSM_NEARESTWEEK);
		break;

	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTHOUR, GTLCSM_NEARESTHOUR);
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTHALFDAY, GTLCSM_NEARESTHALFDAY);
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTDAY, GTLCSM_NEARESTDAY);
		break;

	case GTLC_DISPLAY_HOURS:
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTHALFHOUR, GTLCSM_NEARESTHALFHOUR);
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTHOUR, GTLCSM_NEARESTHOUR);
		CDialogHelper::AddStringT(*this, IDS_SNAP_NEARESTHALFDAY, GTLCSM_NEARESTHALFDAY);
		break;
	}
	// Add to all
	CDialogHelper::AddStringT(*this, IDS_SNAP_FREE, GTLCSM_FREE);

	SelectMode(nSnap);
}

BOOL CGanttSnapComboBox::SelectMode(GTLC_SNAPMODE nSnap)
{
	return (CB_ERR != CDialogHelper::SelectItemByDataT(*this, nSnap));
}

GTLC_SNAPMODE CGanttSnapComboBox::GetSelectedMode() const
{
	return CDialogHelper::GetSelectedItemDataT(*this, GTLCSM_NONE);
}

