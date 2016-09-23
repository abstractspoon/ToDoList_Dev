// EnBitmap.cpp: implementation of the CEnBitmapEx class (c) daniel godson 2002.
//
// credits: Peter Hendrix's CPicture implementation for the original IPicture code 
//          Yves Maurer's GDIRotate implementation for the idea of working directly on 32 bit representations of bitmaps 
//          Karl Lager's 'A Fast Algorithm for Rotating Bitmaps' 
// 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EnBitmapex.h"
#include "imageprocessors.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnBitmapEx::CEnBitmapEx(COLORREF crBkgnd) : CEnBitmap(crBkgnd)
{

}

CEnBitmapEx::~CEnBitmapEx()
{

}

BOOL CEnBitmapEx::LoadSysBitmap(UINT nID)
{
	if (LoadBitmap(nID))
		return RemapSysColors();

	return FALSE;
}

BOOL CEnBitmapEx::RotateImage(int nDegrees, BOOL bEnableWeighting)
{
	CImageRotator proc(nDegrees, bEnableWeighting);
	return ProcessImage(&proc);
}

BOOL CEnBitmapEx::ShearImage(int nHorz, int nVert, BOOL bEnableWeighting)
{
	CImageShearer proc(nHorz, nVert, bEnableWeighting);
	return ProcessImage(&proc);
}

BOOL CEnBitmapEx::GrayImage(COLORREF crMask)
{
	CImageGrayer proc;
	return ProcessImage(&proc, crMask);
}

BOOL CEnBitmapEx::BlurImage(int nAmount)
{
	CImageBlurrer proc(nAmount);
	return ProcessImage(&proc);
}

BOOL CEnBitmapEx::SharpenImage(int nAmount)
{
	CImageSharpener proc(nAmount);
	return ProcessImage(&proc);
}

BOOL CEnBitmapEx::ResizeImage(double dFactor)
{
	CImageResizer proc(dFactor);
	return ProcessImage(&proc);
}

BOOL CEnBitmapEx::LightenImage(double dAmount, COLORREF crMask)
{
	CImageLightener proc(dAmount);
	return ProcessImage(&proc, crMask);
}

BOOL CEnBitmapEx::FlipImage(BOOL bHorz, BOOL bVert)
{
	CImageFlipper proc(bHorz, bVert);
	return ProcessImage(&proc);
}

BOOL CEnBitmapEx::NegateImage()
{
	CImageNegator proc;
	return ProcessImage(&proc);
}

BOOL CEnBitmapEx::ReplaceColor(COLORREF crFrom, COLORREF crTo)
{
	CColorReplacer proc(crFrom, crTo);
	return ProcessImage(&proc);
}

BOOL CEnBitmapEx::ColorizeImage(COLORREF color)
{
	CImageColorizer proc(color);
	return ProcessImage(&proc);
}

BOOL CEnBitmapEx::ContrastImage(int nAmount)
{
	CImageContraster proc(nAmount);
	return ProcessImage(&proc);
}

BOOL CEnBitmapEx::TintImage(COLORREF color, int nAmount, COLORREF crMask)
{
	CImageTinter proc(color, nAmount);
	return ProcessImage(&proc, crMask);
}

BOOL CEnBitmapEx::RemapSysColors()
{
	CImageSysColorMapper proc;
	return ProcessImage(&proc);
}

BOOL CEnBitmapEx::GrayScale(COLORREF crMask)
{
	// create 'nice' disabled image
	C32BIPArray aProcessors;
	CImageGrayer ip2(0.33, 0.33, 0.33);
	CImageLightener ip3(0.2);
	CImageTinter ip4(GetSysColor(COLOR_3DSHADOW), 10);
	CImageContraster ip5(-30);
	
	aProcessors.Add(&ip2);
	aProcessors.Add(&ip3);
	aProcessors.Add(&ip4);
	aProcessors.Add(&ip5);
	
	return ProcessImage(aProcessors, crMask);
}

BOOL CEnBitmapEx::GrayScale(CBitmap& bitmap, COLORREF crMask)
{
	ASSERT(bitmap.GetSafeHandle());

	if (!bitmap.GetSafeHandle())
		return FALSE;

	CEnBitmapEx bmpEnhanced(crMask);
	bmpEnhanced.Attach(bitmap.Detach());
	
	if (bmpEnhanced.GrayScale(crMask))
		bitmap.Attach(bmpEnhanced.Detach());

	// else
	return FALSE;
}

HICON CEnBitmapEx::CreateDisabledIcon(HICON hIcon)
{
	CEnBitmapEx hbmDisabled;
	const COLORREF crMask = RGB(255, 0, 255);
	
	hbmDisabled.CopyImage(hIcon, crMask, 16, 16);
	hbmDisabled.GrayImage(crMask);
	hbmDisabled.LightenImage(0.3, crMask);
	
	return hbmDisabled.ExtractIcon(crMask, 16, 16);
}

BOOL CEnBitmapEx::CreateDisabledImageList(const CImageList& ilSrc, CImageList& ilDest)
{
	if (!ilSrc.GetSafeHandle() || !ilSrc.GetImageCount())
	{
		ASSERT(0);
		return FALSE;
	}

	ilDest.DeleteImageList();
	CImageList* pILSrc = (CImageList*)&ilSrc;

	if (ilDest.Create(pILSrc))
	{
		int nImage = ilDest.GetImageCount();

		while (nImage--)
		{
			HICON hIcon = pILSrc->ExtractIcon(nImage);
			HICON hDis = CreateDisabledIcon(hIcon);

			ilDest.Replace(nImage, hDis);

			::DestroyIcon(hIcon);
			::DestroyIcon(hDis);
		}
	}

	// else
	return FALSE;
}
