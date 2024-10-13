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

const CString PREFSSECTION				= _T("Preferences");
const CString PREFSCOLORSECTION			= _T("Preferences\\Colors");
const CString DMPREFSCOLORSECTION		= _T("DarkMode\\Colors");

const CString COLORTASKBKGNDKEY			= _T("ColorTaskBackground");
const CString PRIORITYCOLOROPTION		= _T("PriorityColorOption");
const CString TEXTCOLOROPTION			= _T("TextColorOption");
const CString COLORATTRIBUTE			= _T("ColorAttribute");

const CString PRIORITYCOLORS			= _T("PriorityColors");
const CString PRIORITYSCHEME			= _T("PriorityScheme");
const CString ATTRIBUTECOLORS			= _T("AttribColors");

const CString NO_PREFIX;

/////////////////////////////////////////////////////////////////////////////

enum 
{
	LM_DEFAULT,
	DM_DEFAULT,
};

struct COLORDEF
{
	LPCTSTR szKey;
	COLORREF crDefault[2];
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
	{ _T("PriorityLow"),		DEF_PRIORITYLOWCOLOR,	DEF_PRIORITYLOWCOLOR },
	{ _T("PriorityHigh"),		DEF_PRIORITYHIGHCOLOR,	DEF_PRIORITYHIGHCOLOR },
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
	BOOL bDarkMode = IsEnabled();

	CString sToPrefix(bDarkMode ? _T("DM_") : _T("LM_"));
	int nFromDefault = (bDarkMode ? DM_DEFAULT : LM_DEFAULT);

	CopyColors(prefs,
			   PREFSSECTION,
			   PREFSCOLORSECTION,
			   NO_PREFIX,
			   nFromDefault,
			   DMPREFSCOLORSECTION,
			   DMPREFSCOLORSECTION,
			   sToPrefix);
}

void CTDCDarkMode::RestoreColors(CPreferences& prefs)
{
	if (!IsSupported())
		return;

	BOOL bDarkMode = IsEnabled();

	CString sFromPrefix(bDarkMode ? _T("DM_") : _T("LM_"));
	int nFromDefault = (bDarkMode ? DM_DEFAULT : LM_DEFAULT);

	CopyColors(prefs,
			   DMPREFSCOLORSECTION,
			   DMPREFSCOLORSECTION,
			   sFromPrefix,
			   nFromDefault,
			   PREFSSECTION,
			   PREFSCOLORSECTION,
			   NO_PREFIX);

	prefs.Save();
}

void CTDCDarkMode::CopyColors(CPreferences& prefs,
							  const CString& sFromSection,
							  const CString& sFromColorSection,
							  const CString& sFromPrefix,
							  int nFromDefault,
							  const CString& sToSection,
							  const CString& sToColorSection,
							  const CString& sToPrefix)
{
	// Preferences
	BOOL bColorIsBkgnd = prefs.GetProfileInt(sFromSection, (sFromPrefix + COLORTASKBKGNDKEY), (nFromDefault == DM_DEFAULT));
	prefs.WriteProfileInt(sToSection, (sToPrefix + COLORTASKBKGNDKEY), bColorIsBkgnd);

	int nPriorityOption = prefs.GetProfileInt(sFromSection, (sFromPrefix + PRIORITYCOLOROPTION), PRIORITYOPT_GRADIENT);
	prefs.WriteProfileInt(sToSection, (sToPrefix + PRIORITYCOLOROPTION), nPriorityOption);

	int nTextOption = prefs.GetProfileEnum(sFromSection, (sFromPrefix + TEXTCOLOROPTION), TEXTOPT_DEFAULT);
	prefs.WriteProfileInt(sToSection, (sToPrefix + TEXTCOLOROPTION), nTextOption);

	int nColorAttrib = prefs.GetProfileEnum(sFromSection, (sFromPrefix + COLORATTRIBUTE), TDCA_CATEGORY);
	prefs.WriteProfileInt(sToSection, (sToPrefix + COLORATTRIBUTE), nTextOption);

	// Colors

	// Individual
	for (int nKey = 0; nKey < NUM_COLORS; nKey++)
	{
		const COLORDEF& def = COLORDEFS[nKey];
		COLORREF color = prefs.GetProfileInt(sFromColorSection, (sFromPrefix + def.szKey), def.crDefault[nFromDefault]);

		prefs.WriteProfileInt(sToColorSection, (sToPrefix + def.szKey), color);
	}

	// Arrays
	CString sValues = prefs.GetProfileString(sFromColorSection, (sFromPrefix + PRIORITYCOLORS));
	prefs.WriteProfileString(sToColorSection, (sToPrefix + PRIORITYCOLORS), sValues);

	sValues = prefs.GetProfileString(sFromColorSection, (sFromPrefix + PRIORITYSCHEME));
	prefs.WriteProfileString(sToColorSection, (sToPrefix + PRIORITYSCHEME), sValues);

	sValues = prefs.GetProfileString(sFromColorSection, (sFromPrefix + ATTRIBUTECOLORS));
	prefs.WriteProfileString(sToColorSection, (sToPrefix + ATTRIBUTECOLORS), sValues);
}

