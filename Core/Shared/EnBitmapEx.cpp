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
#include "Icon.h"
#include "GraphicsMisc.h"

#include "..\3rdparty\XNamedColors.h"

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

BOOL CEnBitmapEx::LightenImage(double dAmount, BOOL bRGB, COLORREF crMask)
{
	CImageLightener proc(dAmount, bRGB);
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

BOOL CEnBitmapEx::Disable(COLORREF crMask)
{
	// create 'nice' disabled image
	C32BIPArray aProcessors;
	CImageGrayer ip2(0.33, 0.33, 0.33);
	CImageLightener ip3(0.2, TRUE);
	CImageTinter ip4(GetSysColor(COLOR_3DSHADOW), 10);
	CImageContraster ip5(-30);
	
	aProcessors.Add(&ip2);
	aProcessors.Add(&ip3);
	aProcessors.Add(&ip4);
	aProcessors.Add(&ip5);
	
	return ProcessImage(aProcessors, crMask);
}

BOOL CEnBitmapEx::Disable(CBitmap& bitmap, COLORREF crMask)
{
	ASSERT(bitmap.GetSafeHandle());

	if (!bitmap.GetSafeHandle())
		return FALSE;

	CEnBitmapEx bmpEnhanced(crMask);
	bmpEnhanced.Attach(bitmap.Detach());
	
	if (bmpEnhanced.Disable(crMask))
		bitmap.Attach(bmpEnhanced.Detach());

	// else
	return FALSE;
}

HICON CEnBitmapEx::CreateDisabledIcon(HICON hIcon)
{
	CEnBitmapEx hbmDisabled;
	const COLORREF crMask = colorMagenta;
	
	hbmDisabled.CopyImage(hIcon, crMask, 16, 16);
	hbmDisabled.Disable(crMask);
	
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
			CIcon icon(pILSrc->ExtractIcon(nImage));
			CIcon iconDis(CreateDisabledIcon(icon));

			ilDest.Replace(nImage, iconDis);
		}

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CEnBitmapEx::ProcessImage(C32BitImageProcessor* pProcessor, COLORREF crMask)
{
	C32BIPArray aProcessors;

	aProcessors.Add(pProcessor);

	return ProcessImage(aProcessors, crMask);
}

BOOL CEnBitmapEx::ProcessImage(C32BIPArray& aProcessors, COLORREF crMask)
{
	ASSERT(GetSafeHandle());

	if (!GetSafeHandle())
		return FALSE;

	if (!aProcessors.GetSize())
		return TRUE;

	int nProcessor, nCount = aProcessors.GetSize();

	// retrieve src and final dest sizes
	CSize sizeSrc = GetSize();
	CSize sizeDest(sizeSrc), sizeMax(sizeSrc);

	for (nProcessor = 0; nProcessor < nCount; nProcessor++)
	{
		sizeDest = aProcessors[nProcessor]->CalcDestSize(sizeDest);
		sizeMax = CSize(max(sizeMax.cx, sizeDest.cx), max(sizeMax.cy, sizeDest.cy));
	}

	// prepare src and dest bits
	RGBX* pSrcPixels = GetDIBits32();

	if (!pSrcPixels)
		return FALSE;

	RGBX* pDestPixels = new RGBX[sizeMax.cx * sizeMax.cy];

	if (!pDestPixels)
		return FALSE;

	Fill(pDestPixels, sizeMax, m_crBkgnd);

	BOOL bRes = TRUE;
	sizeDest = sizeSrc;

	// do the processing
	for (nProcessor = 0; bRes && nProcessor < nCount; nProcessor++)
	{
		// if its the second processor or later then we need to copy
		// the previous dest bits back into source.
		// we also need to check that sizeSrc is big enough
		if (nProcessor > 0)
		{
			if (sizeSrc.cx < sizeDest.cx || sizeSrc.cy < sizeDest.cy)
			{
				delete[] pSrcPixels;
				pSrcPixels = new RGBX[sizeDest.cx * sizeDest.cy];
			}

			CopyMemory(pSrcPixels, pDestPixels, sizeDest.cx * 4 * sizeDest.cy); // default
			Fill(pDestPixels, sizeDest, m_crBkgnd);
		}

		sizeSrc = sizeDest;
		sizeDest = aProcessors[nProcessor]->CalcDestSize(sizeSrc);

		bRes = aProcessors[nProcessor]->ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest, crMask);
	}

	// update the bitmap
	if (bRes)
	{
		// set the bits
		HDC hdc = GetDC(NULL);
		HBITMAP hbmSrc = ::CreateCompatibleBitmap(hdc, sizeDest.cx, sizeDest.cy);

		if (hbmSrc)
		{
			BITMAPINFO bi;

			if (PrepareBitmapInfo32(bi, hbmSrc))
			{
				if (SetDIBits(hdc, hbmSrc, 0, sizeDest.cy, pDestPixels, &bi, DIB_RGB_COLORS))
				{
					// delete the bitmap and attach new
					GraphicsMisc::VerifyDeleteObject(*this);
					bRes = Attach(hbmSrc);
				}
			}

			VERIFY(::ReleaseDC(NULL, hdc));

			if (!bRes)
				GraphicsMisc::VerifyDeleteObject(hbmSrc);
		}
	}

	delete[] pSrcPixels;
	delete[] pDestPixels;

	return bRes;
}

