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

const CString PREFSSECTION			= _T("Preferences");
const CString PREFSCOLORSECTION		= _T("Preferences\\Colors");
const CString DMPREFSSECTION		= _T("DarkMode\\Colors");
const CString DMPREFSCOLORSECTION	= DMPREFSSECTION;

const CString ISDARKMODE			= _T("DarkMode");

const CString COLORISTASKBKGND		= _T("ColorTaskBackground");
const CString PRIORITYCOLOROPTION	= _T("PriorityColorOption");
const CString TEXTCOLOROPTION		= _T("TextColorOption");
const CString COLORATTRIBUTE		= _T("ColorAttribute");

const CString PRIORITYCOLORS		= _T("PriorityColors");
const CString PRIORITYSCHEME		= _T("PriorityScheme");
const CString ATTRIBUTECOLORS		= _T("AttribColors");

const CString NOPREFIX;

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

void CTDCDarkMode::Initialize(CPreferences& prefs)
{
	if (!IsSupported())
		return;

	BOOL bDarkMode = prefs.GetProfileInt(PREFSSECTION, ISDARKMODE, FALSE);

	// Cannot use this method for de-initialisation
	if (IsEnabled() && !bDarkMode)
	{
		ASSERT(0);
		return;
	}

	CDarkMode::Enable(bDarkMode);

	// Only restore colours if something has been previously saved
	// else we'll overwrite the user's current preferences
	if (prefs.HasProfileSection(DMPREFSCOLORSECTION))
		RestoreColors(prefs);
}

void CTDCDarkMode::Release()
{
	// Only save colours if we're:
	//
	// 1. Already in Dark Mode OR
	// 2. We will be in Dark Mode next time OR
	// 3. We've been in DarkMode at some point in the past
	//
	// This reduces the processing for people who will never use Dark Mode
	CPreferences prefs;

	if (IsEnabled() || 
		prefs.GetProfileInt(PREFSSECTION, ISDARKMODE, FALSE) ||
		prefs.HasProfileSection(DMPREFSCOLORSECTION))
	{
		SaveColors(prefs);
	}

	CDarkMode::Enable(FALSE);
}

void CTDCDarkMode::SaveColors(CPreferences& prefs)
{
	ASSERT(IsSupported());

	BOOL bDarkMode = IsEnabled();
	
	CString sToPrefix(bDarkMode ? _T("DM_") : _T("LM_"));
	int nFromDefault = (bDarkMode ? DM_DEFAULT : LM_DEFAULT);

	CopyColors(prefs,
			   PREFSSECTION,
			   PREFSCOLORSECTION,
			   NOPREFIX,
			   nFromDefault,
			   DMPREFSSECTION,
			   DMPREFSCOLORSECTION,
			   sToPrefix);
}

void CTDCDarkMode::RestoreColors(CPreferences& prefs)
{
	ASSERT(IsSupported());

	BOOL bDarkMode = IsEnabled();

	CString sFromPrefix(bDarkMode ? _T("DM_") : _T("LM_"));
	int nFromDefault = (bDarkMode ? DM_DEFAULT : LM_DEFAULT);

	CopyColors(prefs,
			   DMPREFSSECTION,
			   DMPREFSCOLORSECTION,
			   sFromPrefix,
			   nFromDefault,
			   PREFSSECTION,
			   PREFSCOLORSECTION,
			   NOPREFIX);

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
	BOOL bColorIsBkgnd = prefs.GetProfileInt(sFromSection, (sFromPrefix + COLORISTASKBKGND), (nFromDefault == DM_DEFAULT));
	prefs.WriteProfileInt(sToSection, (sToPrefix + COLORISTASKBKGND), bColorIsBkgnd);

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

