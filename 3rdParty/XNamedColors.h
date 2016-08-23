// XNamedColors.h  Version 1.1
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

#ifndef XNAMEDCOLORS_H
#define XNAMEDCOLORS_H

class CXNamedColors
{
public:
	CXNamedColors();
	CXNamedColors(LPCTSTR lpszColor);	// can be:  "red" or "255,0,0" or "#0000FF"
	CXNamedColors(COLORREF color);
	virtual ~CXNamedColors();

	CString		GetHex();
	COLORREF	GetRGB();
	CString		GetName();
	void		SetHex(LPCTSTR lpszHex);
	void		SetRGB(COLORREF rgb);
	void		SetName(LPCTSTR lpszColorName);
	void		SetColorFromString(LPCTSTR lpszColor);

private:
	COLORREF	m_RGB;
	struct CN
	{
		COLORREF	color;
		TCHAR*		pszName;
	};
	static CN m_aCN[];
};


const COLORREF colorAliceBlue            = RGB(240,248,255);
const COLORREF colorAntiqueWhite         = RGB(250,235,215);
const COLORREF colorAqua                 = RGB(  0,255,255);
const COLORREF colorAquamarine           = RGB(127,255,212);
const COLORREF colorAzure                = RGB(240,255,255);
const COLORREF colorBeige                = RGB(245,245,220);
const COLORREF colorBisque               = RGB(255,228,196);
const COLORREF colorBlack                = RGB(  0,  0,  0);
const COLORREF colorBlanchedAlmond       = RGB(255,255,205);
const COLORREF colorBlue                 = RGB(  0,  0,255);
const COLORREF colorBlueViolet           = RGB(138, 43,226);
const COLORREF colorBrown                = RGB(165, 42, 42);
const COLORREF colorBurlywood            = RGB(222,184,135);
const COLORREF colorCadetBlue            = RGB( 95,158,160);
const COLORREF colorChartreuse           = RGB(127,255,  0);
const COLORREF colorChocolate            = RGB(210,105, 30);
const COLORREF colorCoral                = RGB(255,127, 80);
const COLORREF colorCornflowerBlue       = RGB(100,149,237);
const COLORREF colorCornsilk             = RGB(255,248,220);
const COLORREF colorCrimson              = RGB(220, 20, 60);
const COLORREF colorCyan                 = RGB(  0,255,255);
const COLORREF colorDarkBlue             = RGB(  0,  0,139);
const COLORREF colorDarkCyan             = RGB(  0,139,139);
const COLORREF colorDarkGoldenRod        = RGB(184,134, 11);
const COLORREF colorDarkGray             = RGB(169,169,169);
const COLORREF colorDarkGreen            = RGB(  0,100,  0);
const COLORREF colorDarkKhaki            = RGB(189,183,107);
const COLORREF colorDarkMagenta          = RGB(139,  0,139);
const COLORREF colorDarkOliveGreen       = RGB( 85,107, 47);
const COLORREF colorDarkOrange           = RGB(255,140,  0);
const COLORREF colorDarkOrchid           = RGB(153, 50,204);
const COLORREF colorDarkRed              = RGB(139,  0,  0);
const COLORREF colorDarkSalmon           = RGB(233,150,122);
const COLORREF colorDarkSeaGreen         = RGB(143,188,143);
const COLORREF colorDarkSlateBlue        = RGB( 72, 61,139);
const COLORREF colorDarkSlateGray        = RGB( 47, 79, 79);
const COLORREF colorDarkTurquoise        = RGB(  0,206,209);
const COLORREF colorDarkViolet           = RGB(148,  0,211);
const COLORREF colorDeepPink             = RGB(255, 20,147);
const COLORREF colorDeepSkyBlue          = RGB(  0,191,255);
const COLORREF colorDimGray              = RGB(105,105,105);
const COLORREF colorDodgerBlue           = RGB( 30,144,255);
const COLORREF colorFireBrick            = RGB(178, 34, 34);
const COLORREF colorFloralWhite          = RGB(255,250,240);
const COLORREF colorForestGreen          = RGB( 34,139, 34);
const COLORREF colorFuchsia              = RGB(255,  0,255);
const COLORREF colorGainsboro            = RGB(220,220,220);
const COLORREF colorGhostWhite           = RGB(248,248,255);
const COLORREF colorGold                 = RGB(255,215,  0);
const COLORREF colorGoldenRod            = RGB(218,165, 32);
const COLORREF colorGray                 = RGB(127,127,127);
const COLORREF colorGreen                = RGB(  0,128,  0);
const COLORREF colorGreenYellow          = RGB(173,255, 47);
const COLORREF colorHoneyDew             = RGB(240,255,240);
const COLORREF colorHotPink              = RGB(255,105,180);
const COLORREF colorIndianRed            = RGB(205, 92, 92);
const COLORREF colorIndigo               = RGB( 75,  0,130);
const COLORREF colorIvory                = RGB(255,255,240);
const COLORREF colorKhaki                = RGB(240,230,140);
const COLORREF colorLavender             = RGB(230,230,250);
const COLORREF colorLavenderBlush        = RGB(255,240,245);
const COLORREF colorLawngreen            = RGB(124,252,  0);
const COLORREF colorLemonChiffon         = RGB(255,250,205);
const COLORREF colorLightBlue            = RGB(173,216,230);
const COLORREF colorLightCoral           = RGB(240,128,128);
const COLORREF colorLightCyan            = RGB(224,255,255);
const COLORREF colorLightGoldenRodYellow = RGB(250,250,210);
const COLORREF colorLightGreen           = RGB(144,238,144);
const COLORREF colorLightGrey            = RGB(211,211,211);
const COLORREF colorLightPink            = RGB(255,182,193);
const COLORREF colorLightSalmon          = RGB(255,160,122);
const COLORREF colorLightSeaGreen        = RGB( 32,178,170);
const COLORREF colorLightSkyBlue         = RGB(135,206,250);
const COLORREF colorLightSlateGray       = RGB(119,136,153);
const COLORREF colorLightSteelBlue       = RGB(176,196,222);
const COLORREF colorLightYellow          = RGB(255,255,224);
const COLORREF colorLime                 = RGB(  0,255,  0);
const COLORREF colorLimeGreen            = RGB( 50,205, 50);
const COLORREF colorLinen                = RGB(250,240,230);
const COLORREF colorMagenta              = RGB(255,  0,255);
const COLORREF colorMaroon               = RGB(128,  0,  0);
const COLORREF colorMediumAquamarine     = RGB(102,205,170);
const COLORREF colorMediumBlue           = RGB(  0,  0,205);
const COLORREF colorMediumOrchid         = RGB(186, 85,211);
const COLORREF colorMediumPurple         = RGB(147,112,219);
const COLORREF colorMediumSeaGreen       = RGB( 60,179,113);
const COLORREF colorMediumSlateBlue      = RGB(123,104,238);
const COLORREF colorMediumSpringGreen    = RGB(  0,250,154);
const COLORREF colorMediumTurquoise      = RGB( 72,209,204);
const COLORREF colorMediumVioletRed      = RGB(199, 21,133);
const COLORREF colorMidnightBlue         = RGB( 25, 25,112);
const COLORREF colorMintCream            = RGB(245,255,250);
const COLORREF colorMistyRose            = RGB(255,228,225);
const COLORREF colorMoccasin             = RGB(255,228,181);
const COLORREF colorNavajoWhite          = RGB(255,222,173);
const COLORREF colorNavy                 = RGB(  0,  0,128);
const COLORREF colorOldLace              = RGB(253,245,230);
const COLORREF colorOlive                = RGB(128,128,  0);
const COLORREF colorOliveDrab            = RGB(107,142, 35);
const COLORREF colorOrange               = RGB(255,165,  0);
const COLORREF colorOrangeRed            = RGB(255, 69,  0);
const COLORREF colorOrchid               = RGB(218,112,214);
const COLORREF colorPaleGoldenRod        = RGB(238,232,170);
const COLORREF colorPaleGreen            = RGB(152,251,152);
const COLORREF colorPaleTurquoise        = RGB(175,238,238);
const COLORREF colorPaleVioletRed        = RGB(219,112,147);
const COLORREF colorPapayaWhip           = RGB(255,239,213);
const COLORREF colorPeachPuff            = RGB(255,218,185);
const COLORREF colorPeru                 = RGB(205,133, 63);
const COLORREF colorPink                 = RGB(255,192,203);
const COLORREF colorPlum                 = RGB(221,160,221);
const COLORREF colorPowderBlue           = RGB(176,224,230);
const COLORREF colorPurple               = RGB(128,  0,128);
const COLORREF colorRed                  = RGB(255,  0,  0);
const COLORREF colorRosyBrown            = RGB(188,143,143);
const COLORREF colorRoyalBlue            = RGB( 65,105,225);
const COLORREF colorSaddleBrown          = RGB(139, 69, 19);
const COLORREF colorSalmon               = RGB(250,128,114);
const COLORREF colorSandyBrown           = RGB(244,164, 96);
const COLORREF colorSeaGreen             = RGB( 46,139, 87);
const COLORREF colorSeaShell             = RGB(255,245,238);
const COLORREF colorSienna               = RGB(160, 82, 45);
const COLORREF colorSilver               = RGB(192,192,192);
const COLORREF colorSkyBlue              = RGB(135,206,235);
const COLORREF colorSlateBlue            = RGB(106, 90,205);
const COLORREF colorSlateGray            = RGB(112,128,144);
const COLORREF colorSnow                 = RGB(255,250,250);
const COLORREF colorSpringGreen          = RGB(  0,255,127);
const COLORREF colorSteelBlue            = RGB( 70,130,180);
const COLORREF colorTan                  = RGB(210,180,140);
const COLORREF colorTeal                 = RGB(  0,128,128);
const COLORREF colorThistle              = RGB(216,191,216);
const COLORREF colorTomato               = RGB(255, 99, 71);
const COLORREF colorTurquoise            = RGB( 64,224,208);
const COLORREF colorViolet               = RGB(238,130,238);
const COLORREF colorWheat                = RGB(245,222,179);
const COLORREF colorWhite                = RGB(255,255,255);
const COLORREF colorWhiteSmoke           = RGB(245,245,245);
const COLORREF colorYellow               = RGB(255,255,  0);
const COLORREF colorYellowGreen          = RGB(139,205, 50);

#endif //XNAMEDCOLORS_H
