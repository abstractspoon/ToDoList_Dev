// GanttStruct.h: interface for the CGanttStruct class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ganttstatic.h"

/////////////////////////////////////////////////////////////////////////////

int FindDisplay(GTLC_MONTH_DISPLAY nDisplay)
{
	int nMode = NUM_DISPLAYMODES;
	
	while (nMode--)
	{
		if (DISPLAYMODES[nMode].nDisplay == nDisplay)
			return nMode;
	}
	
	return FALSE;
}

GTLC_MONTH_DISPLAY GetPreviousDisplay(GTLC_MONTH_DISPLAY nDisplay)
{
	int nMode = FindDisplay(nDisplay);

	switch (nMode)
	{
	case -1: 
		return GTLC_DISPLAY_NONE;

	case 0:  
		return nDisplay;
	}

	// Default
	return DISPLAYMODES[nMode - 1].nDisplay;
}

GTLC_MONTH_DISPLAY GetNextDisplay(GTLC_MONTH_DISPLAY nDisplay)
{
	int nMode = FindDisplay(nDisplay);

	switch (nMode)
	{
	case -1:
		return GTLC_DISPLAY_NONE;

	case (NUM_DISPLAYMODES - 1):
		return nDisplay;
	}

	// Default
	return DISPLAYMODES[nMode + 1].nDisplay;
}

GTLC_MONTH_DISPLAY GetLastDisplay()
{
	return DISPLAYMODES[NUM_DISPLAYMODES + 1].nDisplay;
}

GTLC_MONTH_DISPLAY GetFirstDisplay()
{
	return DISPLAYMODES[0].nDisplay;
}

int CompareDisplays(GTLC_MONTH_DISPLAY nDisplay1, GTLC_MONTH_DISPLAY nDisplay2)
{
	return (FindDisplay(nDisplay1) - FindDisplay(nDisplay2));
}

BOOL IsValidDisplay(GTLC_MONTH_DISPLAY nDisplay)
{
	return (FindDisplay(nDisplay) != -1);
}

/////////////////////////////////////////////////////////////////////////////

GTLC_DRAG MapHitTestToDrag(GTLC_HITTEST nHit)
{
	switch (nHit)
	{
	case GTLCHT_BEGIN:	return GTLCD_START;
	case GTLCHT_END:	return GTLCD_END;
	case GTLCHT_MIDDLE:	return GTLCD_WHOLE;
	}

	// all else
	return GTLCD_NONE;
}

GTLC_HITTEST MapDragToHitTest(GTLC_DRAG nDrag)
{
	switch (nDrag)
	{
	case GTLCD_START:	return GTLCHT_BEGIN;
	case GTLCD_END:		return GTLCHT_END;
	case GTLCD_WHOLE:	return GTLCHT_MIDDLE;
	}

	// all else
	return GTLCHT_NOWHERE;
	
}
