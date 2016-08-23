// ColorDef.cpp: implementation of the CColorDef class.
//
/////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ColorDef.h"

///////////////////////////////////////////////////////////////////////

HLSX::HLSX() : fHue(0.0f), fLuminosity(0.0f), fSaturation(0.0f)
{
}

HLSX::HLSX(float hue, float lum, float sat) : fHue(hue), fLuminosity(lum), fSaturation(sat)
{
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

///////////////////////////////////////////////////////////////////////

RGBX::RGBX() : btRed(0), btBlue(0), btGreen(0), btUnused(0)
{
}

RGBX::RGBX(BYTE red, BYTE green, BYTE blue) : btRed(red), btBlue(blue), btGreen(green), btUnused(0)
{
}

RGBX::RGBX(const RGBX& color) 
{ 
	*this = color; 
}

RGBX::RGBX(const COLORREF& color) : btRed(GetRValue(color)), btBlue(GetBValue(color)), btGreen(GetGValue(color)), btUnused(0)
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

void RGBX::AdjustLighting(double dFactor, bool bRGB)
{
	if (dFactor == 0.0)
		return;
	
	if (bRGB)
	{
		if (dFactor > 0.0) // Lighter
		{
			btRed = (BYTE)min(255, (btRed + ((255 - btRed) * dFactor)));
			btGreen = (BYTE)min(255, (btGreen + ((255 - btGreen) * dFactor)));
			btBlue = (BYTE)min(255, (btBlue + ((255 - btBlue) * dFactor)));
		}
		else // < 0.0 - Darker
		{
			btRed = (BYTE)max(0, (btRed + (btRed * dFactor)));
			btGreen = (BYTE)max(0, (btGreen + (btGreen * dFactor)));
			btBlue = (BYTE)max(0, (btBlue + (btBlue * dFactor)));
		}
	}
	else // HLS
	{
		HLSX hls;
		RGB2HLS(*this, hls);
		
		hls.fLuminosity = (float)(hls.fLuminosity + dFactor);
		hls.fLuminosity = max(0.0f, min(1.0f, hls.fLuminosity));
		
		HLS2RGB(hls, *this);
	}
}

COLORREF RGBX::HLS2RGB(const HLSX& hls, RGBX& rgb)
{
	if (!hls.fSaturation)
	{
		rgb.btRed = rgb.btBlue = rgb.btGreen = (BYTE)(min(255.0f, hls.fLuminosity * 255));
	}
	else
	{
		float fT1, fT2;
		
		if (hls.fLuminosity < 0.5f)
			fT2 = hls.fLuminosity * (1.0f + hls.fSaturation);
		else
			fT2= (hls.fLuminosity + hls.fSaturation) - (hls.fLuminosity * hls.fSaturation);
		
		fT1 = 2 * hls.fLuminosity - fT2;
		
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

