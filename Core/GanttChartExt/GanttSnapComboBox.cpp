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
	//{{AFX_MSG_MAP(CGanttSnapComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGanttSnapComboBox message handlers

void CGanttSnapComboBox::Rebuild(GTLC_MONTH_DISPLAY nDisplay, GTLC_SNAPMODE nSnap)
{
	ResetContent();

	switch (nDisplay)
	{
	case GTLC_DISPLAY_QUARTERCENTURIES:
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTHALFYEAR, GTLCSM_NEARESTHALFYEAR);
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTYEAR, GTLCSM_NEARESTYEAR);
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTDECADE, GTLCSM_NEARESTDECADE);
		break;

	case GTLC_DISPLAY_DECADES:
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTMONTH, GTLCSM_NEARESTMONTH);
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTHALFYEAR, GTLCSM_NEARESTHALFYEAR);
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTYEAR, GTLCSM_NEARESTYEAR);
		break;

	case GTLC_DISPLAY_YEARS:
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTMONTH, GTLCSM_NEARESTMONTH);
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTHALFYEAR, GTLCSM_NEARESTHALFYEAR);
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTYEAR, GTLCSM_NEARESTYEAR);
		break;

	case GTLC_DISPLAY_QUARTERS_SHORT:
	case GTLC_DISPLAY_QUARTERS_MID:
	case GTLC_DISPLAY_QUARTERS_LONG:
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTMONTH, GTLCSM_NEARESTMONTH);
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTQUARTER, GTLCSM_NEARESTQUARTER);
		break;

	case GTLC_DISPLAY_MONTHS_SHORT:
	case GTLC_DISPLAY_MONTHS_MID:
	case GTLC_DISPLAY_MONTHS_LONG:
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTDAY, GTLCSM_NEARESTDAY);
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTMONTH, GTLCSM_NEARESTMONTH);
		break;

	case GTLC_DISPLAY_WEEKS_SHORT:
	case GTLC_DISPLAY_WEEKS_MID:
	case GTLC_DISPLAY_WEEKS_LONG:
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTHALFDAY, GTLCSM_NEARESTHALFDAY);
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTDAY, GTLCSM_NEARESTDAY);
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTWEEK, GTLCSM_NEARESTWEEK);
		break;

	case GTLC_DISPLAY_DAYS_SHORT:
	case GTLC_DISPLAY_DAYS_MID:
	case GTLC_DISPLAY_DAYS_LONG:
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTHOUR, GTLCSM_NEARESTHOUR);
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTHALFDAY, GTLCSM_NEARESTHALFDAY);
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTDAY, GTLCSM_NEARESTDAY);
		break;

	case GTLC_DISPLAY_HOURS:
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTHALFHOUR, GTLCSM_NEARESTHALFHOUR);
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTHOUR, GTLCSM_NEARESTHOUR);
		CDialogHelper::AddString(*this, IDS_SNAP_NEARESTHALFDAY, GTLCSM_NEARESTHALFDAY);
		break;
	}
	// Add to all
	CDialogHelper::AddString(*this, IDS_SNAP_FREE, GTLCSM_FREE);

	SelectMode(nSnap);
}

BOOL CGanttSnapComboBox::SelectMode(GTLC_SNAPMODE nSnap)
{
	return (CB_ERR != CDialogHelper::SelectItemByData(*this, nSnap));
}

GTLC_SNAPMODE CGanttSnapComboBox::GetSelectedMode() const
{
	return CDialogHelper::GetSelectedItemData(*this, GTLCSM_NONE);
}

