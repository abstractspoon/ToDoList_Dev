// ImageProcessors.cpp: C32BitImageProcessor derivations (c) daniel godson 2002.
//
// credits: Karl Lager's 'A Fast Algorithm for Rotating Bitmaps' 
// 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "imageprocessors.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////
// C32BitImageProcessor derivations

const double PI = 3.14159265358979323846;

CImageRotator::CImageRotator(int nDegrees, BOOL bEnableWeighting)
	: C32BitImageProcessor(bEnableWeighting)
{
	// normalize the angle
	while (nDegrees >= 360)
		nDegrees -= 360;

	while (nDegrees < 0)
		nDegrees += 360;

	ASSERT (nDegrees >= 0 && nDegrees < 360);

	m_dRadians = nDegrees * PI / 180;
}

CImageRotator::CImageRotator(double dRadians)
{
	// normalize the angle
	while (dRadians >= 2 * PI)
		dRadians -= 2 * PI;

	while (dRadians <= 0)
		dRadians += 2 * PI;

	ASSERT (dRadians >= 0 && dRadians < 2 * PI);

	m_dRadians = dRadians;
}

CImageRotator::~CImageRotator()
{
}

CSize CImageRotator::CalcDestSize(CSize sizeSrc)
{
	if (!m_dRadians || !sizeSrc.cx || !sizeSrc.cy)
		return sizeSrc;

	// calculate the four rotated corners
	double dCosA = cos(m_dRadians);
	double dSinA = sin(m_dRadians);

	CPoint ptTopLeft, ptTopRight, ptBottomLeft, ptBottomRight;

	ptTopLeft.x = (int)(-sizeSrc.cx * dCosA / 2 + sizeSrc.cy * dSinA / 2);
	ptTopLeft.y = (int)(sizeSrc.cy * dCosA / 2 - (-sizeSrc.cx) * dSinA / 2);

	ptTopRight.x = (int)(sizeSrc.cx * dCosA / 2 + sizeSrc.cy * dSinA / 2);
	ptTopRight.y = (int)(sizeSrc.cy * dCosA / 2 - sizeSrc.cx * dSinA / 2);

	ptBottomLeft.x = (int)(-sizeSrc.cx * dCosA / 2 + (-sizeSrc.cy) * dSinA / 2);
	ptBottomLeft.y = (int)(-sizeSrc.cy * dCosA / 2 - (-sizeSrc.cx) * dSinA / 2);

	ptBottomRight.x = (int)(sizeSrc.cx * dCosA / 2 + (-sizeSrc.cy) * dSinA / 2);
	ptBottomRight.y = (int)(-sizeSrc.cy * dCosA / 2 - sizeSrc.cx * dSinA / 2);

	// find the max absolute values in each direction
	int nMaxY = max(abs(ptTopLeft.y), max(abs(ptTopRight.y), max(abs(ptBottomLeft.y), abs(ptBottomRight.y))));
	int nMaxX = max(abs(ptTopLeft.x), max(abs(ptTopRight.x), max(abs(ptBottomLeft.x), abs(ptBottomRight.x))));
	
	return CSize((nMaxX + 1) * 2, (nMaxY + 1) * 2);
}

BOOL CImageRotator::ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF /*crMask*/)
{
	BOOL bRes = TRUE;

	if (!m_dRadians)
		bRes = C32BitImageProcessor::ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);
	else
	{
		// note: we also need to translate the coords after rotating
		CSize sizeDestOffset(sizeDest.cx / 2 + sizeDest.cx % 2, sizeDest.cy / 2 + sizeDest.cy % 2);
		CSize sizeSrcOffset(sizeSrc.cx / 2 + sizeSrc.cx % 2, sizeSrc.cy / 2 + sizeSrc.cy % 2);

		CRect rSrc(0, 0, sizeSrc.cx - 1, sizeSrc.cy - 1);
		rSrc.OffsetRect(-sizeSrcOffset);

		// note: traversing the src bitmap leads to artifacts in the destination image
		// what we do is to traverse the destination bitmaps and compute the equivalent 
		// source color - credit for this observation goes to Yves Maurer (GDIRotate) 2002
		double dCosA = cos(m_dRadians);
		double dSinA = sin(m_dRadians);

		for (int nY = 0; nY < sizeDest.cy; nY++)
		{
			// calc y components of rotation
			double dCosYComponent = (nY - sizeDestOffset.cy) * dCosA;
			double dSinYComponent = (nY - sizeDestOffset.cy) * dSinA;

			double dSrcX = -sizeDestOffset.cx * dCosA + dSinYComponent;
			double dSrcY = dCosYComponent - (-sizeDestOffset.cx * dSinA);

			for (int nX = 0; nX < sizeDest.cx; nX++)
			{
				dSrcX += dCosA;
				dSrcY -= dSinA;

				CPoint ptSrc((int)dSrcX, (int)dSrcY);
				int nPixel = (nY * sizeDest.cx + nX);

				if (rSrc.PtInRect(ptSrc))
				{
					if (!m_bWeightingEnabled)
					{
						ptSrc.Offset(sizeSrcOffset);
						RGBX* pRGBSrc = &pSrcPixels[ptSrc.y * sizeSrc.cx + ptSrc.x];
						
						pDestPixels[nPixel] = *pRGBSrc;
					}
					else
						CalcWeightedColor(pSrcPixels, sizeSrc, 
										dSrcX + sizeSrcOffset.cx, dSrcY + sizeSrcOffset.cy,
										pDestPixels[nPixel]);
				}
			}
		}
	}

	return bRes;
}

///////

CImageShearer::CImageShearer(int nHorz, int nVert, BOOL bEnableWeighting)
	: C32BitImageProcessor(bEnableWeighting), m_nHorz(nHorz), m_nVert(nVert)
{
}

CImageShearer::~CImageShearer()
{
}

CSize CImageShearer::CalcDestSize(CSize sizeSrc)
{
	return CSize(sizeSrc.cx + abs(m_nHorz), sizeSrc.cy + abs(m_nVert));
}

BOOL CImageShearer::ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF /*crMask*/)
{
	BOOL bRes = TRUE;

	if (!m_nHorz && !m_nVert)
		bRes = C32BitImageProcessor::ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);
	else
	{
		// shears +ve (down) or -ve (up)
		for (int nX = 0; nX < sizeDest.cx; nX++)
		{
			double dYOffset = 0;

			// calc the offset to src Y coord
			if (m_nVert > 0)
				dYOffset = (double)m_nVert * nX / sizeDest.cx;
				
			else if (m_nVert < 0)
				dYOffset = (double)-m_nVert * (sizeDest.cx - nX) / sizeDest.cx;

			// shears +ve (right) or -ve (left)
			for (int nY = 0; nY < sizeDest.cy; nY++)
			{
				double dXOffset = 0;

				// calc the offset to src X coord
				if (m_nHorz < 0)
					dXOffset = (double)-m_nHorz * nY / sizeDest.cy;
				
				else if (m_nHorz > 0)
					dXOffset = (double)m_nHorz * (sizeDest.cy - nY) / sizeDest.cy;

				double dSrcX = nX - dXOffset;
				double dSrcY = nY - dYOffset;

				if ((int)dSrcX >= 0 && (int)dSrcX < sizeSrc.cx && (int)dSrcY >= 0 && (int)dSrcY < sizeSrc.cy)
				{
					if (!m_bWeightingEnabled)
					{
						RGBX* pRGBSrc = &pSrcPixels[(int)dSrcY * sizeSrc.cx + (int)dSrcX];
						pDestPixels[nY * sizeDest.cx + nX] = *pRGBSrc;
					}
					else
						CalcWeightedColor(pSrcPixels, sizeSrc, dSrcX, dSrcY, 
											pDestPixels[nY * sizeDest.cx + nX]);
				}
			}
		}
	}

	return bRes;
}

///////

CImageGrayer::CImageGrayer()
	: m_dRedFactor(0), m_dGreenFactor(0), m_dBlueFactor(0), m_bDefault(TRUE)
{
}

CImageGrayer::CImageGrayer(double dRedFactor, double dGreenFactor, double dBlueFactor)
	: m_dRedFactor(dRedFactor), m_dGreenFactor(dGreenFactor), m_dBlueFactor(dBlueFactor), m_bDefault(FALSE)
{
}

CImageGrayer::~CImageGrayer()
{
}

BOOL CImageGrayer::ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask)
{
	UNREFERENCED_PARAMETER(sizeDest);
	ASSERT (sizeSrc == sizeDest);

	for (int nX = 0; nX < sizeSrc.cx; nX++)
	{
		for (int nY = 0; nY < sizeSrc.cy; nY++)
		{
			RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];
			RGBX* pRGBDest = &pDestPixels[nY * sizeSrc.cx + nX];

			if (crMask == -1 || !(crMask == *pRGBSrc))
			{
				if (m_bDefault)
					pRGBDest->MakeGray(*pRGBSrc);
				else
					pRGBDest->MakeGray(*pRGBSrc, m_dRedFactor, m_dGreenFactor, m_dBlueFactor);
			}
			else
				*pRGBDest = *pRGBSrc;
		}
	}

	return TRUE;
}

///////

CImageLightener::CImageLightener(double dAmount) : m_dAmount(dAmount)
{
}

CImageLightener::~CImageLightener()
{
}

BOOL CImageLightener::ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask)
{
	ASSERT (sizeSrc == sizeDest);
	BOOL bRes = TRUE;

	if (m_dAmount == 0)
		bRes = C32BitImageProcessor::ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);
	else
	{
		for (int nX = 0; nX < sizeSrc.cx; nX++)
		{
			for (int nY = 0; nY < sizeSrc.cy; nY++)
			{
				RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];
				RGBX* pRGBDest = &pDestPixels[nY * sizeSrc.cx + nX];

				if (crMask == -1 || !(crMask == *pRGBSrc))
				{
					// convert to HLS
					HLSX hls;
					RGBX::RGB2HLS(*pRGBSrc, hls);

					// then tweak the luminosity
					hls.fLuminosity = min(max(0.0f, hls.fLuminosity + (float)m_dAmount), 1.0f);
					RGBX::HLS2RGB(hls, *pRGBDest);
				}
				else
					*pRGBDest = *pRGBSrc;
			}
		}
	}

	return bRes;
}

///////

CImageBlurrer::CImageBlurrer(int nAmount)
{
	m_nAmount = max(0, min(100, nAmount));
}

CImageBlurrer::~CImageBlurrer()
{
}

BOOL CImageBlurrer::ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF /*crMask*/)
{
	BOOL bRes = TRUE;

	if (m_nAmount == 0)
		bRes = C32BitImageProcessor::ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);
	else
	{
		ASSERT (sizeSrc == sizeDest);

		char nPeak = (char)(0.5 * (110 - m_nAmount));
		char cMask[9] = { 1, 1, 1, 
						  1, nPeak, 1, 
						  1, 1, 1 };

		for (int nX = 0; nX < sizeSrc.cx; nX++)
		{
			for (int nY = 0; nY < sizeSrc.cy; nY++)
			{
				int nRed = 0, nGreen = 0, nBlue = 0, nSubCount = 0, nDivisor = 0;

				for (int nSubX = nX - 1; nSubX <= nX + 1; nSubX++)
				{
					for (int nSubY = nY - 1; nSubY <= nY + 1; nSubY++)
					{
						if (nSubX >= 0 && nSubX < sizeSrc.cx && nSubY >= 0 && nSubY < sizeSrc.cy)
						{
							RGBX* pRGBSub = &pSrcPixels[nSubY * sizeSrc.cx + nSubX];

							int nMask = cMask[nSubCount];

							nRed += pRGBSub->btRed * nMask;
							nGreen += pRGBSub->btGreen * nMask;
							nBlue += pRGBSub->btBlue * nMask;

							nDivisor += nMask;
						}

						nSubCount++;
					}
				}

				RGBX* pRGBDest = &pDestPixels[nY * sizeDest.cx + nX];

				pRGBDest->btRed = (BYTE)min(255, nRed / nDivisor);
				pRGBDest->btGreen = (BYTE)min(255, nGreen / nDivisor);
				pRGBDest->btBlue = (BYTE)min(255, nBlue / nDivisor);
			}
		}
	}

	return TRUE;
}

///////

CImageSharpener::CImageSharpener(int nAmount)
{
	m_nAmount = max(0, min(100, nAmount));
}

CImageSharpener::~CImageSharpener()
{
}

BOOL CImageSharpener::ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF /*crMask*/)
{
	BOOL bRes = TRUE;

	if (m_nAmount == 0)
		bRes = C32BitImageProcessor::ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);
	else
	{
		ASSERT (sizeSrc == sizeDest);

		double dMinMaxRatio = (double)1 / (1 + (100 - m_nAmount) * 0.5);

		double dMaxFactor = 1 / (4 * (1 + dMinMaxRatio));
		double dMinFactor = dMaxFactor * dMinMaxRatio;

		double dMask[9] = { -dMinFactor, -dMaxFactor, -dMinFactor, 
							-dMaxFactor, 2, -dMaxFactor, 
							-dMinFactor, -dMaxFactor, -dMinFactor };

		for (int nX = 0; nX < sizeSrc.cx; nX++)
		{
			for (int nY = 0; nY < sizeSrc.cy; nY++)
			{
				if (nX == 0 || nX == sizeSrc.cx - 1 || nY == 0 || nY == sizeSrc.cy - 1)
				{
					pDestPixels[nY * sizeDest.cx + nX] = pSrcPixels[nY * sizeSrc.cx + nX];
				}
				else
				{
					double dRed = 0, dGreen = 0, dBlue = 0;
					int nSubCount = 0;
					
					for (int nSubX = nX - 1; nSubX <= nX + 1; nSubX++)
					{
						for (int nSubY = nY - 1; nSubY <= nY + 1; nSubY++)
						{
							RGBX* pRGBSub = &pSrcPixels[nSubY * sizeSrc.cx + nSubX];

							double dVMask = dMask[nSubCount];
							
							dRed += pRGBSub->btRed * dVMask;
							dGreen += pRGBSub->btGreen * dVMask;
							dBlue += pRGBSub->btBlue * dVMask;
							
							nSubCount++;
						}
					}

					RGBX* pRGBDest = &pDestPixels[nY * sizeDest.cx + nX];
					
					dRed = min(255, dRed);
					dGreen = min(255, dGreen);
					dBlue = min(255, dBlue);
					dRed = max(0, dRed);
					dGreen = max(0, dGreen);
					dBlue = max(0, dBlue);

					pRGBDest->btRed = (BYTE)dRed;
					pRGBDest->btGreen = (BYTE)dGreen;
					pRGBDest->btBlue = (BYTE)dBlue;
				}
			}
		}
	}

	return TRUE;
}

////////

BOOL CImageEmbosser::ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask)
{
	ASSERT (sizeSrc == sizeDest);
	
	double dMask[9] = { -0.5, 0, 0, 
						0, 1, 0, 
						0, 0, 0 };
	
	for (int nX = 0; nX < sizeSrc.cx; nX++)
	{
		for (int nY = 0; nY < sizeSrc.cy; nY++)
		{
			if (nX == 0 || nX == sizeSrc.cx - 1 || nY == 0 || nY == sizeSrc.cy - 1)
			{
				pDestPixels[nY * sizeDest.cx + nX] = pSrcPixels[nY * sizeSrc.cx + nX];
			}
			else
			{
				RGBX* pRGBDest = &pDestPixels[nY * sizeDest.cx + nX];
				RGBX* pRGBSrc = &pSrcPixels[nY * sizeDest.cx + nX];

				if (crMask == -1 || !(crMask == *pRGBSrc))
				{
					double dRed = 0, dGreen = 0, dBlue = 0;
					int nSubCount = 0;
					
					for (int nSubX = nX - 1; nSubX <= nX + 1; nSubX++)
					{
						for (int nSubY = nY - 1; nSubY <= nY + 1; nSubY++)
						{
							RGBX* pRGBSub = &pSrcPixels[nSubY * sizeSrc.cx + nSubX];
						
							double dVMask = dMask[nSubCount];

							if (dVMask != 0)
							{
								dRed += pRGBSub->btRed * dVMask;
								dGreen += pRGBSub->btGreen * dVMask;
								dBlue += pRGBSub->btBlue * dVMask;
							}
							
							nSubCount++;
						}
					}
				
					dRed = min(255, dRed + 128);
					dGreen = min(255, dGreen + 128);
					dBlue = min(255, dBlue + 128);
					dRed = max(0, dRed);
					dGreen = max(0, dGreen);
					dBlue = max(0, dBlue);
					
					pRGBDest->btRed = (BYTE)dRed;
					pRGBDest->btGreen = (BYTE)dGreen;
					pRGBDest->btBlue = (BYTE)dBlue;
				}
				else
				{
					*pRGBDest = *pRGBSrc;
				}
			}
		}
	}

	return TRUE;
}

////////

CImageResizer::CImageResizer(double dFactor) : m_dFactor(dFactor)
{
	ASSERT (m_dFactor > 0);

	if (m_dFactor > 1)
		m_bWeightingEnabled = TRUE;
}

CImageResizer::~CImageResizer()
{
}

CSize CImageResizer::CalcDestSize(CSize sizeSrc)
{
	return CSize((int)(sizeSrc.cx * m_dFactor), (int)(sizeSrc.cy * m_dFactor));
}

BOOL CImageResizer::ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF /*crMask*/)
{
	BOOL bRes = TRUE;

	if (m_dFactor <= 0)
		return FALSE;

	if (m_dFactor == 1)
		bRes = C32BitImageProcessor::ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);

	else if (m_dFactor > 1)
		bRes = Enlarge(pSrcPixels, sizeSrc, pDestPixels, sizeDest);

	else
		bRes = Shrink(pSrcPixels, sizeSrc, pDestPixels, sizeDest);

	return TRUE;
}

BOOL CImageResizer::Enlarge(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest)
{
	ASSERT (m_dFactor > 1);

	if (m_dFactor <= 1)
		return FALSE;

	double dFactor = 1 / m_dFactor;
	double dXSrc = 0;

	for (int nX = 0; nX < sizeDest.cx; nX++)
	{
		double dYSrc = 0;

		for (int nY = 0; nY < sizeDest.cy; nY++)
		{
			CalcWeightedColor(pSrcPixels, sizeSrc, dXSrc, dYSrc, pDestPixels[nY * sizeDest.cx + nX]);

			dYSrc += dFactor; // next dest pixel in source coords
		}

		dXSrc += dFactor; // next dest pixel in source coords
	}

	return TRUE;
}

BOOL CImageResizer::Shrink(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest)
{
	ASSERT (m_dFactor < 1 && m_dFactor > 0);

	if (m_dFactor >= 1 || m_dFactor <= 0)
		return FALSE;

	double dFactor = 1 / m_dFactor;
	double dXEnd = -dFactor / 2;
	int nXStart, nXEnd = -1;

	for (int nX = 0; nX < sizeDest.cx; nX++)
	{
		int nYStart, nYEnd = -1;
		double dYEnd = -dFactor / 2;

		nXStart = nXEnd + 1;
		dXEnd += dFactor;
		nXEnd = min(sizeSrc.cx - 1, (int)dXEnd + 1);

		if (nXStart > nXEnd)
			continue;

		for (int nY = 0; nY < sizeDest.cy; nY++)
		{
			nYStart = nYEnd + 1;
			dYEnd += dFactor;
			nYEnd = min(sizeSrc.cy - 1, (int)dYEnd + 1);

			if (nYStart > nYEnd)
				continue;

			int nCount = 0, nRed = 0, nGreen = 0, nBlue = 0;

			// average the pixels over the range
			for (int nXSub = nXStart; nXSub <= nXEnd; nXSub++)
			{
				for (int nYSub = nYStart; nYSub <= nYEnd; nYSub++)
				{
					RGBX* pRGBSrc = &pSrcPixels[nYSub * sizeSrc.cx + nXSub];

					nRed += pRGBSrc->btRed;
					nGreen += pRGBSrc->btGreen;
					nBlue += pRGBSrc->btBlue;
					nCount++;
				}
			}

			RGBX* pRGBDest = &pDestPixels[nY * sizeDest.cx + nX];

			pRGBDest->btRed = (BYTE)(nRed / nCount);
			pRGBDest->btGreen = (BYTE)(nGreen / nCount);
			pRGBDest->btBlue = (BYTE)(nBlue / nCount);
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////

CImageNegator::CImageNegator()
{
}

CImageNegator::~CImageNegator()
{
}

BOOL CImageNegator::ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize /*sizeDest*/, 
								COLORREF /*crMask*/)
{
	for (int nX = 0; nX < sizeSrc.cx; nX++)
	{
		for (int nY = 0; nY < sizeSrc.cy; nY++)
		{
			RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];
			RGBX* pRGBDest = &pDestPixels[nY * sizeSrc.cx + nX];

			pRGBDest->btRed = (BYTE)(255 - pRGBSrc->btRed);
			pRGBDest->btGreen = (BYTE)(255 - pRGBSrc->btGreen);
			pRGBDest->btBlue = (BYTE)(255 - pRGBSrc->btBlue);
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////

CImageFlipper::CImageFlipper(BOOL bHorz, BOOL bVert) : m_bHorz(bHorz), m_bVert(bVert)
{
}

CImageFlipper::~CImageFlipper()
{
}

BOOL CImageFlipper::ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF /*crMask*/)
{
	for (int nX = 0; nX < sizeSrc.cx; nX++)
	{
		int nDestX = m_bHorz ? sizeDest.cx - nX - 1 : nX;

		for (int nY = 0; nY < sizeSrc.cy; nY++)
		{
			RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];

			int nDestY = m_bVert ? sizeDest.cy - nY - 1 : nY;
			RGBX* pRGBDest = &pDestPixels[nDestY * sizeDest.cx + nDestX];

			*pRGBDest = *pRGBSrc;
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////

CColorReplacer::CColorReplacer(COLORREF crFrom, COLORREF crTo) : m_crFrom(crFrom), m_crTo(crTo)
{
}

CColorReplacer::~CColorReplacer()
{
}

BOOL CColorReplacer::ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF /*crMask*/)
{
	BOOL bRes = TRUE;

	if (m_crFrom == m_crTo)
		bRes = C32BitImageProcessor::ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);
	else
	{
		RGBX rgbFrom(m_crFrom), rgbTo(m_crTo);

		for (int nX = 0; nX < sizeSrc.cx; nX++)
		{
			for (int nY = 0; nY < sizeSrc.cy; nY++)
			{
				RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];
				RGBX* pRGBDest = &pDestPixels[nY * sizeDest.cx + nX];

				if (*pRGBSrc == rgbFrom)
					*pRGBDest = rgbTo;
				else
					*pRGBDest = *pRGBSrc;
			}
		}
	}

	return bRes;
}

//////////////////////////////////////////////////////////////////////

CImageColorizer::CImageColorizer(COLORREF color) : m_color(color)
{
}

CImageColorizer::~CImageColorizer()
{
}

BOOL CImageColorizer::ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF /*crMask*/)
{
	BOOL bRes = TRUE;

	if (m_color == 0)
		FillMemory(pDestPixels, sizeDest.cx * sizeDest.cy * 4, 0);

	else if (m_color == RGB(255, 255, 255))
		return CImageGrayer::ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);

	else
	{
		RGBX rgb(m_color);
		int nColorLum = rgb.Luminance();

		if (!nColorLum)
			FillMemory(pDestPixels, sizeDest.cx * sizeDest.cy * 4, 0);
		else
		{
			for (int nX = 0; nX < sizeSrc.cx; nX++)
			{
				for (int nY = 0; nY < sizeSrc.cy; nY++)
				{
					RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];
					RGBX* pRGBDest = &pDestPixels[nY * sizeDest.cx + nX];

					pRGBDest->MakeGray(*pRGBSrc);

					int nPixelLum = pRGBDest->Luminance();
					
					pRGBDest->btRed = (BYTE)min(255, MulDiv((int)rgb.btRed, nPixelLum, nColorLum));
					pRGBDest->btGreen = (BYTE)min(255, MulDiv((int)rgb.btGreen, nPixelLum, nColorLum));
					pRGBDest->btBlue = (BYTE)min(255, MulDiv((int)rgb.btBlue, nPixelLum, nColorLum));
				}
			}
		}
	}

	return bRes;
}

//////////////////////////////////////////////////////////////////////

CImageTinter::CImageTinter(COLORREF color, int nAmount) : m_color(color)
{
	m_nAmount = max(-100, min(100, nAmount));
}

CImageTinter::~CImageTinter()
{
}

BOOL CImageTinter::ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask)
{
	BOOL bRes = TRUE;

	if (!m_nAmount)
		bRes = C32BitImageProcessor::ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);
	else
	{
		RGBX rgb(m_color);
		float fFactor = m_nAmount / 100.0f;
		
		for (int nX = 0; nX < sizeSrc.cx; nX++)
		{
			for (int nY = 0; nY < sizeSrc.cy; nY++)
			{
				RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];
				RGBX* pRGBDest = &pDestPixels[nY * sizeDest.cx + nX];
				
				if (crMask == -1 || !(crMask == *pRGBSrc))
				{
					pRGBDest->btRed = (BYTE)min(255, max(0, (int)(pRGBSrc->btRed + (int)(rgb.btRed * fFactor))));
					pRGBDest->btGreen = (BYTE)min(255, max(0, (int)(pRGBSrc->btGreen + (int)(rgb.btGreen * fFactor))));
					pRGBDest->btBlue = (BYTE)min(255, max(0, (int)(pRGBSrc->btBlue + (int)(rgb.btBlue * fFactor))));
				}
				else
					*pRGBDest = *pRGBSrc;
			}
		}
	}

	return bRes;
}

//////////////////////////////////////////////////////////////////////

CImageContraster::CImageContraster(int nAmount)
{
	m_nAmount = max(-100, min(200, nAmount));
}

CImageContraster::~CImageContraster()
{
}

BOOL CImageContraster::ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask)
{
	BOOL bRes = TRUE;

	if (!m_nAmount)
		bRes = C32BitImageProcessor::ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);
	else
	{
		float fFactor = 1.0f + m_nAmount / 100.0f;

		for (int nX = 0; nX < sizeSrc.cx; nX++)
		{
			for (int nY = 0; nY < sizeSrc.cy; nY++)
			{
				RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];
				RGBX* pRGBDest = &pDestPixels[nY * sizeDest.cx + nX];

				if (crMask == -1 || !(crMask == *pRGBSrc))
				{
					pRGBDest->btRed = (BYTE)max(0, min(255, (int)((pRGBSrc->btRed - 128) * fFactor) + 128));
					pRGBDest->btGreen = (BYTE)max(0, min(255, (int)((pRGBSrc->btGreen - 128) * fFactor) + 128));
					pRGBDest->btBlue = (BYTE)max(0, min(255, (int)((pRGBSrc->btBlue - 128) * fFactor) + 128));
				}
				else
					*pRGBDest = *pRGBSrc;
			}
		}
	}

	return bRes;
}

////////////////////////////////////////////////////////////////////

// color mapping
struct COLORMAPPING
{
   COLORREF color;
   UINT nSysColor;
};

static COLORMAPPING COLORMAPPINGS[] = 
{
   { 0x000000, COLOR_BTNTEXT },       // black
   { 0x808080, COLOR_BTNSHADOW },     // dark gray
   { 0xC0C0C0, COLOR_BTNFACE },       // bright gray
   { 0xFFFFFF, COLOR_BTNHIGHLIGHT }   // white
};

BOOL CImageSysColorMapper::ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF /*crMask*/)
{
	static int NUMCOLORMAPS = sizeof(COLORMAPPINGS) / sizeof(COLORMAPPING);

	for (int nMap = 0; nMap < NUMCOLORMAPS; nMap++)
	{
		CColorReplacer cr(COLORMAPPINGS[nMap].color, GetSysColor(COLORMAPPINGS[nMap].nSysColor));

		cr.ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);

		// switch source and dest for next iteration provided its not the last
		if (nMap < NUMCOLORMAPS - 1)
		{
			RGBX* pTemp = pSrcPixels;
			pSrcPixels = pDestPixels;
			pDestPixels = pTemp;
		}
	}
	
	return TRUE;
}

////////////////////////////////////////////////////////////////////
