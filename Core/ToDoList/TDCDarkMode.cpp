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
const CString PREFSATTRIBCOLORSECTION	= _T("Preferences\\AttribColors");
const CString DMPREFSCOLORSECTION		= _T("DarkMode\\Colors");
const CString DMPREFSATTRIBCOLORSECTION	= _T("DarkMode\\AttribColors");

const CString COLORTASKBKGNDKEY			= _T("ColorTaskBackground");
const CString PRIORITYCOLOROPTION		= _T("PriorityColorOption");
const CString TEXTCOLOROPTION			= _T("TextColorOption");
const CString ATTRIBCOLORCOUNT			= _T("Count");
const CString ATTRIBCOLORATTRIB			= _T("Attribute");
const CString ATTRIBCOLOR				= _T("Color");
const CString ATTRIBVALUE				= _T("Attrib");

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
	BOOL bDarkMode = IsEnabled();

	CString sToPrefix(bDarkMode ? _T("DM_") : _T("LM_"));
	int nFromDefault = (bDarkMode ? DM_DEFAULT : LM_DEFAULT);

	CopyColors(prefs,
			   PREFSSECTION,
			   PREFSCOLORSECTION,
			   PREFSATTRIBCOLORSECTION,
			   NO_PREFIX,
			   nFromDefault,
			   DMPREFSCOLORSECTION,
			   DMPREFSCOLORSECTION,
			   DMPREFSATTRIBCOLORSECTION, 
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
			   DMPREFSATTRIBCOLORSECTION,
			   sFromPrefix,
			   nFromDefault,
			   PREFSSECTION,
			   PREFSCOLORSECTION,
			   PREFSATTRIBCOLORSECTION,
			   NO_PREFIX);
}

void CTDCDarkMode::CopyColors(CPreferences& prefs,
							  const CString& sFromSection,
							  const CString& sFromColorSection,
							  const CString& sFromAttribColorSection,
							  const CString& sFromPrefix,
							  int nFromDefault,
							  const CString& sToSection,
							  const CString& sToColorSection,
							  const CString& sToAttribColorSection,
							  const CString& sToPrefix)
{
	BOOL bColorIsBkgnd = prefs.GetProfileInt(sFromSection, (sFromPrefix + COLORTASKBKGNDKEY), (nFromDefault == DM_DEFAULT));
	prefs.WriteProfileInt(sToSection, (sToPrefix + COLORTASKBKGNDKEY), bColorIsBkgnd);

	int nPriorityOption = prefs.GetProfileInt(sFromSection, (sFromPrefix + PRIORITYCOLOROPTION), PRIORITYOPT_GRADIENT);
	prefs.WriteProfileInt(sToSection, (sToPrefix + PRIORITYCOLOROPTION), nPriorityOption);

	int nTextOption = prefs.GetProfileEnum(sFromSection, (sFromPrefix + TEXTCOLOROPTION), TEXTOPT_DEFAULT);
	prefs.WriteProfileInt(sToSection, (sToPrefix + TEXTCOLOROPTION), nTextOption);

	for (int nKey = 0; nKey < NUM_COLORS; nKey++)
	{
		const COLORDEF& def = COLORDEFS[nKey];
		COLORREF color = prefs.GetProfileInt(sFromColorSection, (sFromPrefix + def.szKey), def.crDefault[nFromDefault]);

		prefs.WriteProfileInt(sToColorSection, (sToPrefix + def.szKey), color);
	}

	CopyAttributeColors(prefs,
						sFromAttribColorSection, 
						sFromPrefix, 
						sToAttribColorSection, 
						sToPrefix);
}

void CTDCDarkMode::CopyAttributeColors(CPreferences& prefs,
									   const CString& sFromSection,
									   const CString& sFromPrefix,
									   const CString& sToSection,
									   const CString& sToPrefix)
{
	int nNumColor = prefs.GetProfileInt(sFromSection, (sFromPrefix + ATTRIBCOLORCOUNT), 0);
	prefs.WriteProfileInt(sToSection, (sToPrefix + ATTRIBCOLORCOUNT), nNumColor);

	TDC_ATTRIBUTE nAttribID = prefs.GetProfileEnum(sFromSection, (sFromPrefix + ATTRIBCOLORATTRIB), TDCA_CATEGORY);
	prefs.WriteProfileInt(sToSection, (sToPrefix + ATTRIBCOLORATTRIB), nAttribID);

	for (int nColor = 0; nColor < nNumColor; nColor++)
	{
		CString sFromKey = Misc::MakeKey(sFromPrefix + ("P%d"), nColor, sFromSection);
		CString sToKey = Misc::MakeKey(sToPrefix + _T("P%d"), nColor, sToSection);

		prefs.WriteProfileInt(sToKey, ATTRIBCOLOR, prefs.GetProfileInt(sFromKey, ATTRIBCOLOR, CLR_NONE));
		prefs.WriteProfileString(sToKey, ATTRIBVALUE, prefs.GetProfileString(sFromKey, ATTRIBVALUE));
	}
}
