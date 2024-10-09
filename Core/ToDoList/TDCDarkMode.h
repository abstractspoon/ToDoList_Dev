
#if !defined(AFX_TDCDARKMODE_H__CA63D273_DB5E_4DBF_8915_1885E1987A65__INCLUDED_)
#define AFX_TDCDARKMODE_H__CA63D273_DB5E_4DBF_8915_1885E1987A65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

#include "..\shared\DarkMode.h"

/////////////////////////////////////////////////////////////////////////////

class CPreferences;

/////////////////////////////////////////////////////////////////////////////

class CTDCDarkMode : public CDarkMode
{
public:
	static void Initialize(BOOL bEnable, CPreferences& prefs);
	static void Release();

protected:
	// Hide base class method
	static void Enable(BOOL bEnable);

protected:
	static void RestoreColors(CPreferences& prefs);
	static void SaveColors();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDCDARKMODE_H__CA63D273_DB5E_4DBF_8915_1885E1987A65__INCLUDED_)
