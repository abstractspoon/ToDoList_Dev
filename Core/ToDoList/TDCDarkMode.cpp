// ToDoList.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TDCDarkMode.h"
#include "PreferencesUITasklistColorsPage.h"

#include "..\Interfaces\Preferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

LPCTSTR PREFSSECTION		= _T("Preferences");
LPCTSTR PREFSCOLORSECTION	= _T("Preferences\\Colors");
LPCTSTR DMPREFSCOLORSECTION	= _T("DarkMode\\Colors");

LPCTSTR COLORTASKBKGNDKEY	= _T("ColorTaskBackground");
LPCTSTR TEXTCOLOROPTION		= _T("PriorityColorOption");

/////////////////////////////////////////////////////////////////////////////

struct COLORDEF
{
	LPCTSTR szKey;
	COLORREF crLMDefault;
	COLORREF crDMDefault;
};

COLORDEF COLORDEFS[] = 
{
	{ _T("Gridlines"),			DEF_GRIDLINECOLOR,		DM_3DFACE },
	{ _T("TaskDone"),			DEF_TASKDONECOLOR,		DM_WINDOW },
	{ _T("TaskStart"),			DEF_TASKSTARTCOLOR,		RGB(0, 100, 0) },
	{ _T("TaskStartToday"),		DEF_TASKSTARTCOLOR,		RGB(0, 150, 0) },
	{ _T("TaskDue"),			DEF_TASKDUECOLOR,		RGB(100, 0, 0) },
	{ _T("TaskDueToday"),		DEF_TASKDUETODAYCOLOR,	RGB(150, 0, 0) },
	{ _T("AlternateLines"),		DEF_ALTERNATELINECOLOR,	RGB(33, 33, 33) },
	{ _T("Flagged"),			DEF_FLAGGEDCOLOR,		RGB(150, 0, 150) },
	{ _T("Reference"),			DEF_REFERENCECOLOR,		RGB(0, 0, 150) },
	{ _T("GroupHeaderBkgnd"),	DEF_GROUPHEADERBKCOLOR,	RGB(0, 25, 0) },
	
	// priority specific			
	{ _T("Low"),				DEF_PRIORITYLOWCOLOR,	DEF_PRIORITYLOWCOLOR },
	{ _T("High"),				DEF_PRIORITYHIGHCOLOR,	DEF_PRIORITYHIGHCOLOR },

	{ _T("P0"),					DEF_PRIORITYCOLOR_0,	DEF_PRIORITYCOLOR_0 },
	{ _T("P1"),					DEF_PRIORITYCOLOR_1,	DEF_PRIORITYCOLOR_1 },
	{ _T("P2"),					DEF_PRIORITYCOLOR_2,	DEF_PRIORITYCOLOR_2 },
	{ _T("P3"),					DEF_PRIORITYCOLOR_3,	DEF_PRIORITYCOLOR_3 },
	{ _T("P4"),					DEF_PRIORITYCOLOR_4,	DEF_PRIORITYCOLOR_4 },
	{ _T("P5"),					DEF_PRIORITYCOLOR_5,	DEF_PRIORITYCOLOR_5 },
	{ _T("P6"),					DEF_PRIORITYCOLOR_6,	DEF_PRIORITYCOLOR_6 },
	{ _T("P7"),					DEF_PRIORITYCOLOR_7,	DEF_PRIORITYCOLOR_7 },
	{ _T("P8"),					DEF_PRIORITYCOLOR_8,	DEF_PRIORITYCOLOR_8 },
	{ _T("P9"),					DEF_PRIORITYCOLOR_9,	DEF_PRIORITYCOLOR_9 },
	{ _T("P10"),				DEF_PRIORITYCOLOR_10,	DEF_PRIORITYCOLOR_1 },

	{ _T("S0"),					CLR_NONE,				CLR_NONE },
	{ _T("S1"),					CLR_NONE,				CLR_NONE },
	{ _T("S2"),					CLR_NONE,				CLR_NONE },
	{ _T("S3"),					CLR_NONE,				CLR_NONE },
	{ _T("S4"),					CLR_NONE,				CLR_NONE },
	{ _T("S5"),					CLR_NONE,				CLR_NONE },
	{ _T("S6"),					CLR_NONE,				CLR_NONE },
	{ _T("S7"),					CLR_NONE,				CLR_NONE },
	{ _T("S8"),					CLR_NONE,				CLR_NONE },
	{ _T("S9"),					CLR_NONE,				CLR_NONE },
	{ _T("S10"),				CLR_NONE,				CLR_NONE },

};
const int NUM_COLORS = (sizeof(COLORDEFS) / sizeof(COLORDEFS[0]));

/////////////////////////////////////////////////////////////////////////////

void CTDCDarkMode::Initialize(BOOL bEnable, CPreferences& prefs)
{
	CDarkMode::Enable(bEnable);

	RestoreColors(prefs);
}

void CTDCDarkMode::Release()
{
	SaveColors();

	CDarkMode::Enable(FALSE);
}

void CTDCDarkMode::SaveColors()
{
	if (!IsSupported())
		return;

	CPreferences prefs;
	CString sPrefix(IsEnabled() ? _T("DM_") : _T("LM_"));

	for (int nKey = 0; nKey < NUM_COLORS; nKey++)
	{
		const COLORDEF& def = COLORDEFS[nKey];
		COLORREF color = prefs.GetProfileInt(PREFSCOLORSECTION, def.szKey, def.crLMDefault);

		prefs.WriteProfileInt(DMPREFSCOLORSECTION, (sPrefix + def.szKey), color);
	}

	prefs.WriteProfileInt(DMPREFSCOLORSECTION, (sPrefix + COLORTASKBKGNDKEY), prefs.GetProfileInt(PREFSSECTION, COLORTASKBKGNDKEY));
	prefs.WriteProfileInt(DMPREFSCOLORSECTION, (sPrefix + TEXTCOLOROPTION), prefs.GetProfileInt(PREFSSECTION, TEXTCOLOROPTION, -1));
}

void CTDCDarkMode::RestoreColors(CPreferences& prefs)
{
	if (!IsSupported())
		return;

	BOOL bDarkMode = IsEnabled();
	CString sPrefix(bDarkMode ? _T("DM_") : _T("LM_"));

	for (int nKey = 0; nKey < NUM_COLORS; nKey++)
	{
		const COLORDEF& def = COLORDEFS[nKey];
		COLORREF color = prefs.GetProfileInt(DMPREFSCOLORSECTION, (sPrefix + def.szKey), (bDarkMode ? def.crDMDefault : def.crLMDefault));

		prefs.WriteProfileInt(PREFSCOLORSECTION, def.szKey, color);
	}

	prefs.WriteProfileInt(PREFSSECTION, COLORTASKBKGNDKEY, prefs.GetProfileInt(DMPREFSCOLORSECTION, (sPrefix + COLORTASKBKGNDKEY), IsEnabled()));
	prefs.WriteProfileInt(PREFSSECTION, TEXTCOLOROPTION, prefs.GetProfileInt(DMPREFSCOLORSECTION, (sPrefix + TEXTCOLOROPTION), -1));
}
