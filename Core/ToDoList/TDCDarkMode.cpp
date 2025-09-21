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
const CString NOPREFIX;

/////////////////////////////////////////////////////////////////////////////

enum 
{
	LM_DEFAULT,
	DM_DEFAULT,
};

// ------------------------------------------------------

struct NUMERICPREF
{
	LPCTSTR szKey;
	int nDefault[2];
};

const NUMERICPREF NUMERICPREFS[] =
{
	{ _T("PriorityColorOption"),			PRIORITYOPT_GRADIENT,	PRIORITYOPT_GRADIENT },
	{ _T("TextColorOption"),				TEXTOPT_DEFAULT,		TEXTOPT_DEFAULT},
	{ _T("ColorAttribute"),					TDCA_CATEGORY,			TDCA_CATEGORY},

	{ _T("ColorTaskBackground"),			FALSE,					TRUE }, // The only difference by default
	{ _T("ColorPriority"),					TRUE,					TRUE },
	{ _T("SpecifyGridColor"),				TRUE,					TRUE },
	{ _T("SpecifyDoneColor"),				TRUE,					TRUE },
	{ _T("SpecifyStartColor"),				FALSE,					FALSE },
	{ _T("SpecifyStartTodayColor"),			FALSE,					FALSE },
	{ _T("SpecifyDueColor"),				TRUE,					TRUE },
	{ _T("SpecifyDueTodayColor"),			TRUE,					TRUE },
	{ _T("HLSColorGradient"),				TRUE,					TRUE },
	{ _T("AlternateLineColor"),				TRUE,					TRUE },
	{ _T("FlaggedColor"),					FALSE,					FALSE },
	{ _T("ReferenceColor"),					FALSE,					FALSE },
	{ _T("SpecifyGroupHeaderBkgndColor"),	FALSE,					FALSE },
};

const int NUM_NUMERICS = (sizeof(NUMERICPREFS) / sizeof(NUMERICPREFS[0]));

// -------------------------------------------------------

struct COLORPREF
{
	LPCTSTR szKey;
	COLORREF crDefault[2];
};

const COLORPREF COLORPREFS[] = 
{
	{ _T("Gridlines"),			DEF_GRIDLINECOLOR,			DM_3DFACE },
	{ _T("TaskDone"),			DEF_TASKDONECOLOR,			DM_WINDOW },
	{ _T("TaskStart"),			DEF_TASKSTARTCOLOR,			RGB(0, 100, 0) },
	{ _T("TaskStartToday"),		DEF_TASKSTARTCOLOR,			RGB(0, 150, 0) },
	{ _T("TaskDue"),			DEF_TASKDUECOLOR,			RGB(100, 0, 0) },
	{ _T("TaskDueToday"),		DEF_TASKDUETODAYCOLOR,		RGB(150, 0, 0) },
	{ _T("AlternateLines"),		DEF_ALTERNATELINECOLOR,		RGB(33, 33, 33) },
	{ _T("Flagged"),			DEF_FLAGGEDCOLOR,			RGB(150, 0, 150) },
	{ _T("Reference"),			DEF_REFERENCECOLOR,			RGB(0, 0, 150) },
	{ _T("GroupHeaderBkgnd"),	DEF_GROUPHEADERBKCOLOR,		RGB(0, 25, 0) },
	{ _T("PriorityLow"),		DEF_PRIORITYLOWCOLOR,		DEF_PRIORITYLOWCOLOR },
	{ _T("PriorityHigh"),		DEF_PRIORITYHIGHCOLOR,		DEF_PRIORITYHIGHCOLOR },
};

const int NUM_COLORS = (sizeof(COLORPREFS) / sizeof(COLORPREFS[0]));

// -------------------------------------------------------

const LPCTSTR ARRAYPREFS[] =
{
	{ _T("PriorityColors") },
	{ _T("PriorityScheme") },
	{ _T("AttribColors") },
};

const int NUM_ARRAYS = (sizeof(ARRAYPREFS) / sizeof(ARRAYPREFS[0]));

/////////////////////////////////////////////////////////////////////////////

void CTDCDarkMode::Initialize(CPreferences& prefs)
{
	if (!IsSupported())
		return;

	BOOL bDarkMode = prefs.GetProfileInt(PREFSSECTION, ISDARKMODE, FALSE);

	// Cannot use this method for turning off dark-mode
	// Have to use Release()
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
	if (IsSupported())
	{
		// Only save colours if we're:
		//
		// 1. Currently in Dark Mode 
		// OR
		// 2. We will be in Dark Mode next time 
		// OR
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
	{
		for (int nKey = 0; nKey < NUM_NUMERICS; nKey++)
		{
			const NUMERICPREF& num = NUMERICPREFS[nKey];
			int nValue = prefs.GetProfileInt(sFromSection, (sFromPrefix + num.szKey), num.nDefault[nFromDefault]);

			prefs.WriteProfileInt(sToSection, (sToPrefix + num.szKey), nValue);
		}
	}

	// Individual colours
	{
		for (int nKey = 0; nKey < NUM_COLORS; nKey++)
		{
			const COLORPREF& color = COLORPREFS[nKey];
			COLORREF crValue = prefs.GetProfileInt(sFromColorSection, (sFromPrefix + color.szKey), color.crDefault[nFromDefault]);

			prefs.WriteProfileInt(sToColorSection, (sToPrefix + color.szKey), crValue);
		}
	}

	// Colour Arrays
	{
		for (int nKey = 0; nKey < NUM_ARRAYS; nKey++)
		{
			LPCTSTR szKey = ARRAYPREFS[nKey];
			CString sValues = prefs.GetProfileString(sFromColorSection, (sFromPrefix + szKey));

			prefs.WriteProfileString(sToColorSection, (sToPrefix + szKey), sValues);
		}
	}
}

