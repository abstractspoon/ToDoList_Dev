// ColorDef.h: interface and implementation of the CColorDef class.
//
/////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLORDEF_H__2A4E63F6_A106_4295_BCBA_06D03CD67AE7__INCLUDED_)
#define AFX_COLORDEF_H__2A4E63F6_A106_4295_BCBA_06D03CD67AE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////////////////////////////////////////////////////////////////////////////////////
// helper struct. equates to COLORREF

#pragma pack(push)
#pragma pack(1)

struct HLSX
{
	HLSX();
	HLSX(float hue, float lum, float sat);
	HLSX(const COLORREF& color);
	
	operator COLORREF() const;

	float fHue; // (0-360)
	float fLuminosity; // (0-1)
	float fSaturation; // (0-1)
};

class RGBX
{
public:
	BYTE btBlue;
	BYTE btGreen;
	BYTE btRed;

protected:
	BYTE btUnused;

public:
	RGBX();
	RGBX(BYTE red, BYTE green, BYTE blue);
	RGBX(const RGBX& color);
	RGBX(const COLORREF& color);

	inline const RGBX& operator=(const RGBX& rgb) { btRed = rgb.btRed; btBlue = rgb.btBlue; btGreen = rgb.btGreen; btUnused = 0; return *this; }
	inline BOOL operator==(const RGBX& rgb) const { return (btRed == rgb.btRed && btGreen == rgb.btGreen && btBlue == rgb.btBlue); }
	inline operator COLORREF() const { return RGB(btRed, btGreen, btBlue); }
	inline BYTE Luminance() const { return (BYTE)(((int)btBlue + (int)btGreen * 6 + (int)btRed * 3) / 10); }
	inline RGBX Gray() const { BYTE btGray = Luminance(); return RGBX(btGray, btGray, btGray); }
	inline void MakeGray() { btRed = btGreen = btBlue = (BYTE)Luminance(); }
	inline void MakeGray(const RGBX& rgbSrc) { btRed = btGreen = btBlue = (BYTE)rgbSrc.Luminance(); }

	void MakeGray(const RGBX& rgbSrc, double dRedFactor, double dGreenFactor, double dBlueFactor);
	void MakeGray(double dRedFactor, double dGreenFactor, double dBlueFactor);
	
	BOOL IsGray(int nTolerance = 0) const;
	void AdjustLighting(double dFactor, bool bRGB);

	static COLORREF HLS2RGB(const HLSX& hls, RGBX& rgb);
	static void RGB2HLS(const RGBX& rgb, HLSX& hls);

protected:
	static BYTE Hue2Triplet(float fT1, float fT2, float fHue);
};

#pragma pack(pop)

#endif // !defined(AFX_COLORDEF_H__2A4E63F6_A106_4295_BCBA_06D03CD67AE7__INCLUDED_)

///////////////////////////////////////////////////////////////////////

