#if !defined(AFX_IMAGEPROCESSORS_H__1FDE0A4E_8AB4_11D6_95AD_EFA89432A428__INCLUDED_)
#define AFX_IMAGEPROCESSORS_H__1FDE0A4E_8AB4_11D6_95AD_EFA89432A428__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "enbitmap.h"
#include "colordef.h"

//////////////////////////////////////////////////////////////////////////////////////
// derived image processors

class CImageRotator : public C32BitImageProcessor
{
public:
	CImageRotator(int nDegrees, BOOL bEnableWeighting = TRUE);
	CImageRotator(double dRadians);
	virtual ~CImageRotator();

	virtual CSize CalcDestSize(CSize sizeSrc);
	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask = -1);

protected:
	double m_dRadians;
};

class CImageShearer : public C32BitImageProcessor
{
public:
	CImageShearer(int nHorz, int nVert, BOOL bEnableWeighting = TRUE);
	virtual ~CImageShearer();

	virtual CSize CalcDestSize(CSize sizeSrc);
	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask = -1);

protected:
	int m_nHorz, m_nVert;
};

class CImageGrayer : public C32BitImageProcessor
{
public:
	CImageGrayer();
	CImageGrayer(double dRedFactor, double dGreenFactor, double dBlueFactor);
	virtual ~CImageGrayer();

	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask = -1);

protected:
	double m_dRedFactor, m_dGreenFactor, m_dBlueFactor;
	BOOL m_bDefault;
};

class CImageLightener : public C32BitImageProcessor
{
public:
	CImageLightener(double dAmount); 
	virtual ~CImageLightener();

	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask = -1);

protected:
	double m_dAmount;
};

class CImageBlurrer : public C32BitImageProcessor
{
public:
	CImageBlurrer(int nAmount = 50); // 1 - 100 (100 is very blurred)
	virtual ~CImageBlurrer();

	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask = -1);

protected:
	int m_nAmount;
};

class CImageSharpener : public C32BitImageProcessor
{
public:
	CImageSharpener(int nAmount = 50); // 1 - 100 (100 is very sharp)
	virtual ~CImageSharpener();

	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask = -1);

protected:
	int m_nAmount;
};

class CImageResizer : public C32BitImageProcessor
{
public:
	CImageResizer(double dFactor);
	virtual ~CImageResizer();

	virtual CSize CalcDestSize(CSize sizeSrc);
	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask = -1);

protected:
	double m_dFactor;

protected:
	BOOL Enlarge(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest);
	BOOL Shrink(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest);
};

class CImageNegator : public C32BitImageProcessor
{
public:
	CImageNegator();
	virtual ~CImageNegator();

	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask = -1);
};

class CImageFlipper : public C32BitImageProcessor
{
public:
	CImageFlipper(BOOL bHorz, BOOL bVert = 0); 
	virtual ~CImageFlipper();

	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask = -1);

protected:
	BOOL m_bHorz, m_bVert;
};

class CColorReplacer : public C32BitImageProcessor
{
public:
	CColorReplacer(COLORREF crFrom, COLORREF crTo);
	virtual ~CColorReplacer();

	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask = -1);

protected:
	COLORREF m_crFrom, m_crTo;
};

class CImageColorizer : public CImageGrayer
{
public:
	CImageColorizer(COLORREF color);
	virtual ~CImageColorizer();

	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask = -1);

protected:
	COLORREF m_color;
};

class CImageTinter : public C32BitImageProcessor
{
public:
	CImageTinter(COLORREF color, int nAmount = 40); // amount: -100 -> 100
	virtual ~CImageTinter();

	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask = -1);

protected:
	COLORREF m_color;
	int m_nAmount;
};

class CImageContraster : public C32BitImageProcessor
{
public:
	CImageContraster(int nAmount = 40); // -100 -> 200
	virtual ~CImageContraster();

	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask = -1);

protected:
	int m_nAmount;
};

class CImageSysColorMapper : public C32BitImageProcessor
{
public:
	CImageSysColorMapper() {}
	virtual ~CImageSysColorMapper() {}

	// 0x000000 (black)			-> COLOR_BTNTEXT        
	// 0x808080 (dark gray)		-> COLOR_BTNSHADOW      
	// 0xC0C0C0 (bright gray)	-> COLOR_BTNFACE        
	// 0xFFFFFF (white)			-> COLOR_BTNHIGHLIGHT   
	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask = -1);
};

class CImageEmbosser : public C32BitImageProcessor
{
public:
	CImageEmbosser() {}
	virtual ~CImageEmbosser() {}

	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask = -1);
};


#endif