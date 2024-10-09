// ToDoList.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TDCDarkMode.h"

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

/////////////////////////////////////////////////////////////////////////////

struct DMCOLOR
{
	LPCTSTR szKey;
	COLORREF crDMDefault;
};

DMCOLOR COLORS[] = 
{
	{ _T("Gridlines"),			RGB(75, 75, 75) },
	{ _T("TaskDone"),			RGB(63, 64, 65) },
	{ _T("TaskStart"),			RGB(0, 100, 0) },
	{ _T("TaskStartToday"),		RGB(0, 150, 0) },
	{ _T("TaskDue"),			RGB(100, 0, 0) },
	{ _T("TaskDueToday"),		RGB(150, 0, 0) },
	{ _T("AlternateLines"),		RGB(33, 33, 33) },
	{ _T("Flagged"),			RGB(150, 0, 150) },
	{ _T("Reference"),			RGB(0, 0, 150) },
	{ _T("GroupHeaderBkgnd"),	RGB(0, 25, 0) },

	// priority specific		
	{ _T("Low"),				RGB(0, 150, 0) },
	{ _T("High"),				RGB(150, 0, 0) },
	{ _T("P0"),					RGB(0, 150, 0) },
	{ _T("P1"),					RGB(0, 150, 0) },
	{ _T("P2"),					RGB(0, 150, 0) },
	{ _T("P3"),					RGB(0, 150, 0) },
	{ _T("P4"),					RGB(0, 0, 150) },
	{ _T("P5"),					RGB(0, 0, 150) },
	{ _T("P6"),					RGB(0, 0, 150) },
	{ _T("P7"),					RGB(150, 0, 0) },
	{ _T("P8"),					RGB(150, 0, 0) },
	{ _T("P9"),					RGB(150, 0, 0) },
	{ _T("P10"),				RGB(150, 0, 0) },
};
const int NUM_COLORS = (sizeof(COLORS) / sizeof(COLORS[0]));

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
		const DMCOLOR& color = COLORS[nKey];
		COLORREF crColor = prefs.GetProfileInt(PREFSCOLORSECTION, color.szKey);

		prefs.WriteProfileInt(DMPREFSCOLORSECTION, (sPrefix + color.szKey), crColor);
	}

	prefs.WriteProfileInt(DMPREFSCOLORSECTION, (sPrefix + COLORTASKBKGNDKEY), prefs.GetProfileInt(PREFSSECTION, COLORTASKBKGNDKEY));
}

void CTDCDarkMode::RestoreColors(CPreferences& prefs)
{
	if (!IsSupported())
		return;

	CString sPrefix(IsEnabled() ? _T("DM_") : _T("LM_"));

	for (int nKey = 0; nKey < NUM_COLORS; nKey++)
	{
		const DMCOLOR& color = COLORS[nKey];
		COLORREF crColor = prefs.GetProfileInt(DMPREFSCOLORSECTION, (sPrefix + color.szKey), color.crDMDefault);

		prefs.WriteProfileInt(PREFSCOLORSECTION, color.szKey, crColor);
	}

	prefs.WriteProfileInt(PREFSSECTION, COLORTASKBKGNDKEY, prefs.GetProfileInt(DMPREFSCOLORSECTION, (sPrefix + COLORTASKBKGNDKEY), TRUE));
}
