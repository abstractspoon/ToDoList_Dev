// XNamedColors.cpp  Version 1.1
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// This software is released into the public domain.  You are free to use it
// in any way you like, except that you may not sell this source code.
//
// This software is provided "as is" with no expressed or implied warranty.
// I accept no liability for any damage or loss of business that this software
// may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XNamedColors.h"

///////////////////////////////////////////////////////////////////////////////
// array of RGB colors and names
CXNamedColors::CN CXNamedColors::m_aCN[] = {
	{ CLR_NONE, _T("") },
/*	{ colorAliceBlue,				_T("AliceBlue") },
	{ colorAntiqueWhite,			_T("AntiqueWhite") },
	{ colorAqua,					_T("Aqua") },
	{ colorAquamarine,				_T("Aquamarine") },
	{ colorAzure,					_T("Azure") },
	{ colorBeige,					_T("Beige") },
	{ colorBisque,					_T("Bisque") },
	{ colorBlack,					_T("Black") },
	{ colorBlanchedAlmond,			_T("BlanchedAlmond") },
	{ colorBlue,					_T("Blue") },
	{ colorBlueViolet,				_T("BlueViolet") },
	{ colorBrown,					_T("Brown") },
	{ colorBurlywood,				_T("Burlywood") },
	{ colorCadetBlue,				_T("CadetBlue") },
	{ colorChartreuse,				_T("Chartreuse") },
	{ colorChocolate,				_T("Chocolate") },
	{ colorCoral,					_T("Coral") },
	{ colorCornflowerBlue,			_T("CornflowerBlue") },
	{ colorCornsilk,				_T("Cornsilk") },
	{ colorCrimson,					_T("Crimson") },
	{ colorCyan,					_T("Cyan") },
	{ colorDarkBlue,				_T("DarkBlue") },
	{ colorDarkCyan,				_T("DarkCyan") },
	{ colorDarkGoldenRod,			_T("DarkGoldenRod") },
	{ colorDarkGray,				_T("DarkGray") },
	{ colorDarkGreen,				_T("DarkGreen") },
	{ colorDarkKhaki,				_T("DarkKhaki") },
	{ colorDarkMagenta,				_T("DarkMagenta") },
	{ colorDarkOliveGreen,			_T("DarkOliveGreen") },
	{ colorDarkOrange,				_T("DarkOrange") },
	{ colorDarkOrchid,				_T("DarkOrchid") },
	{ colorDarkRed,					_T("DarkRed") },
	{ colorDarkSalmon,				_T("DarkSalmon") },
	{ colorDarkSeaGreen,			_T("DarkSeaGreen") },
	{ colorDarkSlateBlue,			_T("DarkSlateBlue") },
	{ colorDarkSlateGray,			_T("DarkSlateGray") },
	{ colorDarkTurquoise,			_T("DarkTurquoise") },
	{ colorDarkViolet,				_T("DarkViolet") },
	{ colorDeepPink,				_T("DeepPink") },
	{ colorDeepSkyBlue,				_T("DeepSkyBlue") },
	{ colorDimGray,					_T("DimGray") },
	{ colorDodgerBlue,				_T("DodgerBlue") },
	{ colorFireBrick,				_T("FireBrick") },
	{ colorFloralWhite,				_T("FloralWhite") },
	{ colorForestGreen,				_T("ForestGreen") },
	{ colorFuchsia,					_T("Fuchsia") },
	{ colorGainsboro,				_T("Gainsboro") },
	{ colorGhostWhite,				_T("GhostWhite") },
	{ colorGold,					_T("Gold") },
	{ colorGoldenRod,				_T("GoldenRod") },
	{ colorGray,					_T("Gray") },
	{ colorGreen,					_T("Green") },
	{ colorGreenYellow,				_T("GreenYellow") },
	{ colorHoneyDew,				_T("HoneyDew") },
	{ colorHotPink,					_T("HotPink") },
	{ colorIndianRed,				_T("IndianRed") },
	{ colorIndigo,					_T("Indigo") },
	{ colorIvory,					_T("Ivory") },
	{ colorKhaki,					_T("Khaki") },
	{ colorLavender,				_T("Lavender") },
	{ colorLavenderBlush,			_T("LavenderBlush") },
	{ colorLawngreen,				_T("Lawngreen") },
	{ colorLemonChiffon,			_T("LemonChiffon") },
	{ colorLightBlue,				_T("LightBlue") },
	{ colorLightCoral,				_T("LightCoral") },
	{ colorLightCyan,				_T("LightCyan") },
	{ colorLightGoldenRodYellow,	_T("LightGoldenRodYellow")},
	{ colorLightGreen,				_T("LightGreen") },
	{ colorLightGrey,				_T("LightGrey") },
	{ colorLightPink,				_T("LightPink") },
	{ colorLightSalmon,				_T("LightSalmon") },
	{ colorLightSeaGreen,			_T("LightSeaGreen") },
	{ colorLightSkyBlue,			_T("LightSkyBlue") },
	{ colorLightSlateGray,			_T("LightSlateGray") },
	{ colorLightSteelBlue,			_T("LightSteelBlue") },
	{ colorLightYellow,				_T("LightYellow") },
	{ colorLime,					_T("Lime") },
	{ colorLimeGreen,				_T("LimeGreen") },
	{ colorLinen,					_T("Linen") },
	{ colorMagenta,					_T("Magenta") },
	{ colorMaroon,					_T("Maroon") },
	{ colorMediumAquamarine,		_T("MediumAquamarine") },
	{ colorMediumBlue,				_T("MediumBlue") },
	{ colorMediumOrchid,			_T("MediumOrchid") },
	{ colorMediumPurple,			_T("MediumPurple") },
	{ colorMediumSeaGreen,			_T("MediumSeaGreen") },
	{ colorMediumSlateBlue,			_T("MediumSlateBlue") },
	{ colorMediumSpringGreen,		_T("MediumSpringGreen") },
	{ colorMediumTurquoise,			_T("MediumTurquoise") },
	{ colorMediumVioletRed,			_T("MediumVioletRed") },
	{ colorMidnightBlue,			_T("MidnightBlue") },
	{ colorMintCream,				_T("MintCream") },
	{ colorMistyRose,				_T("MistyRose") },
	{ colorMoccasin,				_T("Moccasin") },
	{ colorNavajoWhite,				_T("NavajoWhite") },
	{ colorNavy,					_T("Navy") },
	{ colorOldLace,					_T("OldLace") },
	{ colorOlive,					_T("Olive") },
	{ colorOliveDrab,				_T("OliveDrab") },
	{ colorOrange,					_T("Orange") },
	{ colorOrangeRed,				_T("OrangeRed") },
	{ colorOrchid,					_T("Orchid") },
	{ colorPaleGoldenRod,			_T("PaleGoldenRod") },
	{ colorPaleGreen,				_T("PaleGreen") },
	{ colorPaleTurquoise,			_T("PaleTurquoise") },
	{ colorPaleVioletRed,			_T("PaleVioletRed") },
	{ colorPapayaWhip,				_T("PapayaWhip") },
	{ colorPeachPuff,				_T("PeachPuff") },
	{ colorPeru,					_T("Peru") },
	{ colorPink,					_T("Pink") },
	{ colorPlum,					_T("Plum") },
	{ colorPowderBlue,				_T("PowderBlue") },
	{ colorPurple,					_T("Purple") },
	{ colorRed,						_T("Red") },
	{ colorRosyBrown,				_T("RosyBrown") },
	{ colorRoyalBlue,				_T("RoyalBlue") },
	{ colorSaddleBrown,				_T("SaddleBrown") },
	{ colorSalmon,					_T("Salmon") },
	{ colorSandyBrown,				_T("SandyBrown") },
	{ colorSeaGreen,				_T("SeaGreen") },
	{ colorSeaShell,				_T("SeaShell") },
	{ colorSienna,					_T("Sienna") },
	{ colorSilver,					_T("Silver") },
	{ colorSkyBlue,					_T("SkyBlue") },
	{ colorSlateBlue,				_T("SlateBlue") },
	{ colorSlateGray,				_T("SlateGray") },
	{ colorSnow,					_T("Snow") },
	{ colorSpringGreen,				_T("SpringGreen") },
	{ colorSteelBlue,				_T("SteelBlue") },
	{ colorTan,						_T("Tan") },
	{ colorTeal,					_T("Teal") },
	{ colorThistle,					_T("Thistle") },
	{ colorTomato,					_T("Tomato") },
	{ colorTurquoise,				_T("Turquoise") },
	{ colorViolet,					_T("Violet") },
	{ colorWheat,					_T("Wheat") },
	{ colorWhite,					_T("White") },
	{ colorWhiteSmoke,				_T("WhiteSmoke") },
	{ colorYellow,					_T("Yellow") },
	{ colorYellowGreen,				_T("YellowGreen") }
*/
};

///////////////////////////////////////////////////////////////////////////////
CXNamedColors::CXNamedColors()
{
	m_RGB = RGB(0,0,0);		// initialize to black
}

///////////////////////////////////////////////////////////////////////////////
// can be:  "red" or "255,0,0" or "#0000FF"
CXNamedColors::CXNamedColors(LPCTSTR lpszColor)
{
	SetColorFromString(lpszColor);
}

///////////////////////////////////////////////////////////////////////////////
CXNamedColors::CXNamedColors(COLORREF color)
{
	m_RGB = color;
}

///////////////////////////////////////////////////////////////////////////////
CXNamedColors::~CXNamedColors()
{
}

///////////////////////////////////////////////////////////////////////////////
CString CXNamedColors::GetHex()
{
	BYTE r, g, b;
	r = GetRValue(m_RGB);
	g = GetGValue(m_RGB);
	b = GetBValue(m_RGB);
	CString str;
	str.Format(_T("#%02X%02X%02X"), b, g, r);
	return str;
}

///////////////////////////////////////////////////////////////////////////////
COLORREF CXNamedColors::GetRGB()
{
	return m_RGB;
}

///////////////////////////////////////////////////////////////////////////////
CString CXNamedColors::GetName()
{
	CString str = _T("not defined");

	int n = sizeof(m_aCN) / sizeof(m_aCN[0]);
	for (int i = 0; i < n; i++)
	{
		if (m_RGB == m_aCN[i].color)
		{
			str = m_aCN[i].pszName;
			break;
		}
	}
	return str;
}

///////////////////////////////////////////////////////////////////////////////
void CXNamedColors::SetHex(LPCTSTR lpszHex)
{
	COLORREF rgb = RGB(0,0,0);
	TCHAR *cp = (TCHAR *) lpszHex;
	if (*cp == _T('#') && _tcslen(lpszHex) == 7)
	{
		BYTE r = 0;
		BYTE g = 0;
		BYTE b = 0;
		TCHAR s[3];
		cp++;
		s[0] = *cp++;
		s[1] = *cp++;
		s[2] = _T('\0');
		r = (BYTE)_tcstoul(s, NULL, 16);
		s[0] = *cp++;
		s[1] = *cp++;
		g = (BYTE)_tcstoul(s, NULL, 16);
		s[0] = *cp++;
		s[1] = *cp++;
		b = (BYTE)_tcstoul(s, NULL, 16);
		rgb = RGB(r,g,b);
	}
	m_RGB = rgb;
}

///////////////////////////////////////////////////////////////////////////////
void CXNamedColors::SetRGB(COLORREF rgb)
{
	m_RGB = rgb;
}

///////////////////////////////////////////////////////////////////////////////
void CXNamedColors::SetName(LPCTSTR lpszColorName)
{
	COLORREF rgb = RGB(0,0,0);
	CString strColorName = lpszColorName;

	int n = sizeof(m_aCN) / sizeof(m_aCN[0]);
	for (int i = 0; i < n; i++)
	{
		if (_tcsicmp(lpszColorName, m_aCN[i].pszName) == 0)
		{
			rgb = m_aCN[i].color;
			break;
		}
	}
	m_RGB = rgb;
}

///////////////////////////////////////////////////////////////////////////////
// can be:  "red" or "255,0,0" or "#0000FF"
void CXNamedColors::SetColorFromString(LPCTSTR lpszColor)
{
	m_RGB = RGB(0,0,0);		// initialize to black
	BYTE r = 0;
	BYTE g = 0;
	BYTE b = 0;
	//fabio_2005
	const TCHAR *cp;
	//TCHAR *cp;

	if ((cp = _tcschr(lpszColor, _T(','))) != NULL)
	{
		// "255,0,0"
		r = (BYTE) _ttoi(lpszColor);
		cp++;
		g = (BYTE) _ttoi(cp);
		cp = _tcschr(cp, _T(','));
		if (cp)
		{
			cp++;
			b = (BYTE) _ttoi(cp);
		}
		m_RGB = RGB(r,g,b);
	}
	else if ((cp = _tcschr(lpszColor, _T('#'))) != NULL)
	{
		// "#0000FF"
		if (_tcslen(lpszColor) == 7)
		{
			TCHAR s[3];
			cp++;
			s[0] = *cp++;
			s[1] = *cp++;
			s[2] = _T('\0');
			r = (BYTE)_tcstoul(s, NULL, 16);
			s[0] = *cp++;
			s[1] = *cp++;
			g = (BYTE)_tcstoul(s, NULL, 16);
			s[0] = *cp++;
			s[1] = *cp++;
			b = (BYTE)_tcstoul(s, NULL, 16);
			m_RGB = RGB(r,g,b);
		}
	}
	else
	{
		// "red"
		int n = sizeof(m_aCN) / sizeof(m_aCN[0]);
		for (int i = 0; i < n; i++)
		{
			if (_tcsicmp(lpszColor, m_aCN[i].pszName) == 0)
			{
				m_RGB = m_aCN[i].color;
				break;
			}
		}
	}
}
