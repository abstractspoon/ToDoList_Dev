// ColorDef.cpp: implementation of the CColorDef class.
//
/////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ColorDef.h"

#include <math.h>

///////////////////////////////////////////////////////////////////////

HLSX::HLSX() : fHue(0.0f), fLuminosity(0.0f), fSaturation(0.0f)
{
}

HLSX::HLSX(double hue, double lum, double sat, BOOL bWrapHue) 
	: 
	fHue((float)hue), 
	fLuminosity((float)lum), 
	fSaturation((float)sat)
{
	Validate(bWrapHue);
}

HLSX::HLSX(const COLORREF& color) 
{ 
	RGBX::RGB2HLS(color, *this); 
}

HLSX::operator COLORREF() const 
{ 
	RGBX color; 
	return RGBX::HLS2RGB(*this, color); 
}

void HLSX::Increment(double hue, double lum, double sat, BOOL bWrapHue)
{
	fHue += (float)hue;
	fSaturation += (float)sat;
	fLuminosity += (float)lum;

	Validate(bWrapHue);
}

void HLSX::Validate(BOOL bWrapHue)
{
	if (bWrapHue)
	{
		while (fHue < 0.0f)
			fHue += 360.0f;

		while (fHue > 360.0f)
			fHue -= 360.0f;
	}
	else
	{
		fHue = max(0, min(fHue, 360.0f));
	}

	fSaturation = max(0, min(fSaturation, 1.0f));
	fLuminosity = max(0, min(fLuminosity, 1.0f));
}

///////////////////////////////////////////////////////////////////////

RGBX::RGBX() : btRed(0), btBlue(0), btGreen(0), btUnused(0)
{
}

RGBX::RGBX(int red, int green, int blue) : btRed(0), btBlue(0), btGreen(0), btUnused(0)
{
	Set(red, green, blue);
}

RGBX::RGBX(const RGBX& color) 
{ 
	*this = color; 
}

RGBX::RGBX(COLORREF color) : btRed(GetRValue(color)), btBlue(GetBValue(color)), btGreen(GetGValue(color)), btUnused(0)
{
}

void RGBX::MakeGray(const RGBX& rgbSrc, double dRedFactor, double dGreenFactor, double dBlueFactor) 
{ 
	*this = rgbSrc;
	MakeGray(dRedFactor, dGreenFactor, dBlueFactor);
}

void RGBX::MakeGray(double dRedFactor, double dGreenFactor, double dBlueFactor) 
{ 
	btRed = btGreen = btBlue = 
		(BYTE)((btBlue * dBlueFactor) +	(btGreen * dGreenFactor) + (btRed * dRedFactor)); 
}

BOOL RGBX::IsGray(int nTolerance) const
{
	return (abs(btRed - btGreen) <= nTolerance && abs(btGreen - btBlue) <= nTolerance);
}

COLORREF RGBX::AdjustLighting(COLORREF color, double dFactor, bool bRGB)
{
	RGBX rgb(color);
	rgb.AdjustLighting(dFactor, bRGB);
	
	return rgb;
}

void RGBX::AdjustLighting(double dFactor, bool bRGB)
{
	if (dFactor == 0.0)
		return;
	
	if (bRGB)
	{
		if (dFactor > 0.0) // Lighter
		{
			Increment(((255 - btRed) * dFactor),
					  ((255 - btGreen) * dFactor),
					  ((255 - btBlue) * dFactor));
		}
		else // < 0.0 - Darker
		{
			Increment((btRed * dFactor),
					  (btGreen * dFactor),
					  (btBlue * dFactor));
		}
	}
	else // HLS
	{
		HLSX hls(*this);

		if (dFactor > 0.0) // Lighter
		{
			hls.Increment(0.0, 
						  ((1.0 - hls.fLuminosity) * dFactor), 
						  0.0);
		}
		else // < 0.0 - Darker
		{
			hls.Increment(0.0, 
						  (hls.fLuminosity * dFactor), 
						  0.0);
		}
		
		*this = hls;
	}
}

void RGBX::Increment(int red, int green, int blue)
{
	Set((btRed + red), (btGreen + green), (btBlue + blue));
}

void RGBX::Increment(double red, double green, double blue)
{
	Set((int)(btRed + red), (int)(btGreen + green), (int)(btBlue + blue));
}

void RGBX::Set(int red, int green, int blue)
{
	btRed = max(0, min(red, 255));
	btGreen = max(0, min(green, 255));
	btBlue = max(0, min(blue, 255));
}

void RGBX::Set(double red, double green, double blue)
{
	Set((int)red, (int)green, (int)blue);
}

float RGBX::CalcColorDifference(COLORREF crFrom, COLORREF crTo)
{
	// Algorithm from https://www.compuphase.com/cmetric.htm
	double dAverageRed = ((GetRValue(crFrom) + GetRValue(crTo)) / 2.0);

	int nDiffRed = (GetRValue(crFrom) - GetRValue(crTo));
	int nDiffGreen = (GetGValue(crFrom) - GetGValue(crTo));
	int nDiffBlue = (GetBValue(crFrom) - GetBValue(crTo));

	double dRedCalc = ((2 + (dAverageRed / 256)) * (nDiffRed * nDiffRed));
	double dGreenCalc = (4 * (nDiffGreen * nDiffGreen));
	double dBlueCalc = ((2 + ((255 - dAverageRed) / 256)) * (nDiffBlue * nDiffBlue));

	return (float)(sqrt(dRedCalc + dGreenCalc + dBlueCalc) / 256);
}

float RGBX::CalcLuminanceDifference(COLORREF crFrom, COLORREF crTo)
{
	return (float)fabs(HLSX(crFrom).fLuminosity - HLSX(crTo).fLuminosity);
}

COLORREF RGBX::Complement(COLORREF color, bool bRGB)
{
	if (bRGB)
		return RGB((255 - GetRValue(color)), (255 - GetGValue(color)), (255 - GetBValue(color)));

	// else
	HLSX hls(color);
	hls.Increment(180.0f, 0, 0, TRUE); // wrap hue

	return hls;
}

COLORREF RGBX::HLS2RGB(const HLSX& hls, RGBX& rgb)
{
	if (!hls.fSaturation)
	{
		rgb.btRed = rgb.btBlue = rgb.btGreen = (BYTE)(min(255.0f, hls.fLuminosity * 255));
	}
	else
	{
		float fT2;
		
		if (hls.fLuminosity < 0.5f)
			fT2 = hls.fLuminosity * (1.0f + hls.fSaturation);
		else
			fT2= (hls.fLuminosity + hls.fSaturation) - (hls.fLuminosity * hls.fSaturation);
		
		float fT1 = 2 * hls.fLuminosity - fT2;
		
		rgb.btRed = Hue2Triplet(fT1, fT2, hls.fHue + 120.0f);
		rgb.btGreen = Hue2Triplet(fT1, fT2, hls.fHue);
		rgb.btBlue = Hue2Triplet(fT1, fT2, hls.fHue - 120.0f);
	}
	
	return rgb;
}

void RGBX::RGB2HLS(const RGBX& rgb, HLSX& hls)
{
	int nMax = max(rgb.btRed, max(rgb.btGreen, rgb.btBlue));
	int nMin = min(rgb.btRed, min(rgb.btGreen, rgb.btBlue));
	
	float fDiff = (float)(nMax - nMin);
	float fSum = (float)(nMax + nMin);
	
	// calculate luminosity
	hls.fLuminosity = fSum / 510.0f;
	
	if (nMax == nMin) // gray
	{
		hls.fHue = hls.fSaturation = 0.0f;
	}
	else
	{
		// calculate Saturation
		if (hls.fLuminosity < 0.5f)
			hls.fSaturation = fDiff / fSum;
		else
			hls.fSaturation = fDiff / (510.0f - fSum);
		
		// calculate Hue
		float fRNorm = (nMax - rgb.btRed) / fDiff;	  
		float fGNorm = (nMax - rgb.btGreen) / fDiff;
		float fBNorm = (nMax - rgb.btBlue) / fDiff;   
		
		if (rgb.btRed == nMax)
		{
			hls.fHue = 60.0f * (6.0f + fBNorm - fGNorm);
		}
		else if (rgb.btGreen == nMax)
		{
			hls.fHue = 60.0f * (2.0f + fRNorm - fBNorm);
		}
		else
		{
			hls.fHue = 60.0f * (4.0f + fGNorm - fRNorm);
		}
		
		if (hls.fHue > 360.0f)
			hls.fHue -= 360.0f;
	}
}

BYTE RGBX::Hue2Triplet(float fT1, float fT2, float fHue)
{
	if (fHue < 0)
		fHue += 360.0f;
	
	else if (fHue > 360.0f)
		fHue -= 360.0f;
	
	if (fHue < 60.0f) 
		fT1 = fT1 + (fT2 - fT1) * fHue / 60.0f; 
	
	else if (fHue < 180.0f) 
		fT1 = fT2;
	
	else if (fHue < 240.0f) 
		fT1 = fT1 + (fT2 - fT1) * (240.0f - fHue) / 60.0f;  
	
	return (BYTE)min(fT1 * 255.0f, 255.0f);
}

