// EnBitmap.h: interface for the CEnBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENBITMAPEX_H__1FDE0A4E_8AB4_11D6_95AD_EFA89432A428__INCLUDED_)
#define AFX_ENBITMAPEX_H__1FDE0A4E_8AB4_11D6_95AD_EFA89432A428__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "enbitmap.h"

/////////////////////////////////////////////////////////////////////////////////////

class CEnBitmapEx : public CEnBitmap  
{
public:
	CEnBitmapEx(COLORREF crBkgnd = RGB(255, 255, 255));
	CEnBitmapEx(int cx, int cy, COLORREF crBkgnd = RGB(255, 255, 255));
	virtual ~CEnBitmapEx();

	BOOL LoadSysBitmap(UINT nID);

	BOOL RotateImage(int nDegrees, BOOL bEnableWeighting = FALSE); // rotates about centrepoint, +ve (clockwise) or -ve (anticlockwise) from 12 o'clock
	BOOL ShearImage(int nHorz, int nVert, BOOL bEnableWeighting = FALSE); // shears +ve (right, down) or -ve (left, up)
	BOOL GrayImage(COLORREF crMask = -1);
	BOOL BlurImage(int nAmount = 5);
	BOOL SharpenImage(int nAmount = 5);
	BOOL ResizeImage(double dFactor);
	BOOL FlipImage(BOOL bHorz, BOOL bVert = 0);
	BOOL NegateImage();
	BOOL ReplaceColor(COLORREF crFrom, COLORREF crTo);
	BOOL LightenImage(double dAmount, COLORREF crMask = -1);
	BOOL ColorizeImage(COLORREF color); // converts to grayscale first
	BOOL ContrastImage(int nAmount);
	BOOL TintImage(COLORREF color, int nAmount, COLORREF crMask = -1);
	BOOL RemapSysColors(); // just like AfxLoadSysColorBitmap does
	BOOL GrayScale(COLORREF crMask = GetSysColor(COLOR_3DFACE));
	
	static BOOL GrayScale(CBitmap& bitmap, COLORREF crMask = GetSysColor(COLOR_3DFACE));
	static HICON CreateDisabledIcon(HICON hIcon);
	static BOOL CreateDisabledImageList(const CImageList& ilSrc, CImageList& ilDest);

};


#endif // !defined(AFX_ENBITMAPEX_H__1FDE0A4E_8AB4_11D6_95AD_EFA89432A428__INCLUDED_)
