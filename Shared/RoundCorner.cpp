// Roundcnr.cpp: implementation of the CRoundCorner class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Roundcorner.h"
#include "syscolors.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const double Pi = 3.141592654;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

enum { COLD, DOWN , HOT };

struct Corner
{
	Corner() { bInfillBetween = FALSE; }

	CPoint ptQuadrant;
	CPoint ptOriginOffset;
	COLORREF crFrom, crTo;
	int nAngleFrom, nAngleTo;
	BOOL bInfillBetween;
};

// precalculated color factors
float fHiliteFactor[] = { 0.2f, 0.35f, 0.29f, 0.24f, 0.18f, 0.12f, 0.06f };
float fShadowFactor[] = { 0.7f, 0.75f, 0.81f, 0.85f, 0.88f, 0.91f, 0.95f };

const int NUMFACTORS = sizeof(fHiliteFactor) / sizeof(float);

//////////////////////////////////////////////////////////////////////

CRoundCorner::CRoundCorner(CWnd* pWnd, int nRadius, COLORREF crBkgnd)
{
	ASSERT (pWnd);
	m_pWnd = pWnd;
	m_nRadius = max(0, nRadius);
	m_crBkgnd = crBkgnd;
}

CRoundCorner::~CRoundCorner()
{

}

void CRoundCorner::Draw(int nCorners, DWORD dwStyle)
{
	ASSERT (m_pWnd);
	
	if (!m_pWnd)
		return;

	CRect rWnd;
	m_pWnd->GetWindowRect(rWnd);

	CWindowDC dc(m_pWnd);

	Draw(&dc, rWnd, m_nRadius, nCorners, dwStyle, m_crBkgnd);
}

// static version
void CRoundCorner::Draw(CDC* pDC, LPRECT pRect, int nRadius, int nCorners, DWORD dwStyle, COLORREF crBkgnd)
{
	// special case: if cold and the backgnd color 
	// has not been specified then nothing to draw
	int nState = (dwStyle & RC_DOWN) ? DOWN : (dwStyle & RC_UP) ? HOT : COLD;

	if (nState == COLD && crBkgnd == -1)
		return;

	// edge style
	if (dwStyle & RC_RENDER)
	{
		ASSERT (dwStyle & RC_EDGES);

		if (!(dwStyle & RC_EDGES))
			return;

		if (crBkgnd == (COLORREF)-1)
			crBkgnd = GetColor(COLOR_3DFACE, -1);
	}

	CRect rect(pRect);

//	BOOL bDefBtn = (dwStyle & RC_DEFBTN) && (dwStyle & RC_EDGES); 
	
//	if (bDefBtn)
//		rect.DeflateRect(1, 1);

	rect.right--;
	rect.bottom--;

	nRadius = max(0, nRadius);

	// the maximum allowable radius depends one which Corners are to drawn
	int nMaxRadius = 0;
	BOOL bTopLeft = (nCorners & RC_TOPLEFT);
	BOOL bTopRight = (nCorners & RC_TOPRIGHT);
	BOOL bBottomLeft = (nCorners & RC_BOTTOMLEFT);
	BOOL bBottomRight = (nCorners & RC_BOTTOMRIGHT);

	if (bTopLeft && bTopRight && !bBottomLeft && !bBottomRight)
		nMaxRadius = min(rect.Height(), rect.Width() / 2);

	else if (!bTopLeft && !bTopRight && bBottomLeft && bBottomRight)
		nMaxRadius = min(rect.Height(), rect.Width() / 2); // same

	else if (bTopLeft && bBottomLeft && !bTopRight && !bBottomRight)
		nMaxRadius = min(rect.Width(), rect.Height() / 2);

	else if (!bTopLeft && !bBottomLeft && bTopRight && bBottomRight)
		nMaxRadius = min(rect.Width(), rect.Height() / 2); // same

	else 
		nMaxRadius = min(rect.Width(), rect.Height()) / 2;

	nRadius = min(nRadius, nMaxRadius);

	if (nRadius)
	{
		// draw the Corners first and then the connecting edges
		if (bTopLeft)
			DrawTopLeft(pDC, rect, nRadius, dwStyle, crBkgnd);

		if (bTopRight)
			DrawTopRight(pDC, rect, nRadius, dwStyle, crBkgnd);

		if (bBottomRight)
			DrawBottomRight(pDC, rect, nRadius, dwStyle, crBkgnd);

		if (bBottomLeft)
			DrawBottomLeft(pDC, rect, nRadius, dwStyle, crBkgnd);
	}

	// now the edges
	DrawEdges(pDC, rect, nRadius, nCorners, dwStyle, crBkgnd);

	// now the def button border
/*	if (bDefBtn)
	{
		rect.InflateRect(1, 1);
		DrawDefBtnBorder(pDC, rect, nRadius, nCorners);
	}
*/
}

void CRoundCorner::DrawDefBtnBorder(CDC* pDC, LPRECT pRect, int nRadius, int nCorners)
{
	if (!nRadius || !nCorners)
	{
		pDC->Draw3dRect(pRect, 0, 0);
		return;
	}

	BOOL bTopLeft = (nCorners & RC_TOPLEFT);
	BOOL bTopRight = (nCorners & RC_TOPRIGHT);
	BOOL bBottomLeft = (nCorners & RC_BOTTOMLEFT);
	BOOL bBottomRight = (nCorners & RC_BOTTOMRIGHT);

	Corner cnr;
	CRect rect(pRect);
	nRadius = max(0, nRadius);
	nRadius = min(nRadius, min(rect.Width(), rect.Height()) / 2);

	cnr.crFrom = cnr.crTo = 0;
	cnr.bInfillBetween = TRUE;

	// top left
	if (bTopLeft)
	{
		cnr.ptQuadrant = CPoint(-1, 1);
		cnr.ptOriginOffset = CPoint(rect.left + nRadius, rect.top + nRadius);
		cnr.nAngleFrom = 270;
		cnr.nAngleTo = 360;

		DrawCorner(pDC, nRadius, &cnr);
	}

	// top right
	if (bTopRight)
	{
		cnr.ptQuadrant = CPoint(1, 1);
		cnr.ptOriginOffset = CPoint(rect.right - nRadius, rect.top + nRadius);
		cnr.nAngleFrom = 0;
		cnr.nAngleTo = 90;

		DrawCorner(pDC, nRadius, &cnr);
	}

	// bottom left
	if (bBottomLeft)
	{
		cnr.ptQuadrant = CPoint(-1, -1);
		cnr.ptOriginOffset = CPoint(rect.left + nRadius, rect.bottom - nRadius);
		cnr.nAngleFrom = 180;
		cnr.nAngleTo = 270;

		DrawCorner(pDC, nRadius, &cnr);
	}

	// bottom right
	if (bBottomRight)
	{
		cnr.ptQuadrant = CPoint(1, -1);
		cnr.ptOriginOffset = CPoint(rect.right - nRadius, rect.bottom - nRadius);
		cnr.nAngleFrom = 90;
		cnr.nAngleTo = 180;

		DrawCorner(pDC, nRadius, &cnr);
	}

	// edges

	// top
	CRect rHLine(rect.left + (bTopLeft ? nRadius : 0), rect.top, 
				rect.right - (bTopRight ? nRadius: 0), rect.top + 1);

	// left
	CRect rVLine(rect.left, rect.top + (bTopLeft ? nRadius : 0), 
				rect.left + 1, rect.bottom - (bBottomLeft ? nRadius : 0));

	pDC->FillSolidRect(rHLine, 0);
	pDC->FillSolidRect(rVLine, 0);
	
	// bottom
	rHLine.SetRect(rect.left + (bBottomLeft ? nRadius : 0), rect.bottom, 
					rect.right - ((bBottomRight && nRadius) ? nRadius : -1), rect.bottom + 1);
	
	// right
	rVLine.SetRect(rect.right, rect.top + (bTopRight ? nRadius : 0), 
					rect.right + 1, rect.bottom - ((bBottomRight && nRadius) ? nRadius : -1));

	pDC->FillSolidRect(rHLine, 0);
	pDC->FillSolidRect(rVLine, 0);
}

void CRoundCorner::DrawEdges(CDC* pDC, LPRECT pRect, int nRadius, int nCorners, DWORD dwStyle, COLORREF crBkgnd)
{
	if (!(dwStyle & RC_EDGES)) // sanity check
		return;

	BOOL bRender = (dwStyle & RC_RENDER);
	BOOL bThin = !bRender && (dwStyle & RC_THIN);

	int nState = (dwStyle & RC_DOWN) ? DOWN : (dwStyle & RC_UP) ? HOT : COLD;

	// if cold and the backgnd color has not been set then nothing to draw
	if (nState == COLD && crBkgnd == -1)
		return;

	BOOL bTopLeft = (nCorners & RC_TOPLEFT);
	BOOL bTopRight = (nCorners & RC_TOPRIGHT);
	BOOL bBottomLeft = (nCorners & RC_BOTTOMLEFT);
	BOOL bBottomRight = (nCorners & RC_BOTTOMRIGHT);

	CRect rect(pRect);
	COLORREF color;

	nRadius = max(0, nRadius);
	nRadius = min(nRadius, min(rect.Width(), rect.Height()) / 2);

	// top
	CRect rHLine(rect.left + (bTopLeft ? nRadius : 0), rect.top, 
				rect.right - (bTopRight ? nRadius: 0), rect.top + 1);

	// left
	CRect rVLine(rect.left, rect.top + (bTopLeft ? nRadius : 0), 
				rect.left + 1, rect.bottom - (bBottomLeft ? nRadius : 0));

	if (bRender && crBkgnd != -1)
	{
		int nLine = 0;
		int nMaxLine = nRadius ? min(NUMFACTORS, nRadius) : 
								min(NUMFACTORS, min(rect.Width(), rect.Height()) / 2);

		do
		{
			int nFactor = (nLine * NUMFACTORS) / nMaxLine;

			COLORREF color = (nState == DOWN) ? Darker(crBkgnd, fShadowFactor[nFactor]) : 
							(nState == HOT) ? Lighter(crBkgnd, fHiliteFactor[nFactor]) : crBkgnd;

			pDC->FillSolidRect(rHLine, color);
			pDC->FillSolidRect(rVLine, color);
			rHLine.OffsetRect(0, 1);
			rVLine.OffsetRect(1, 0);

			if (!nRadius)
			{
				// shorten by one pixel
				rHLine.DeflateRect(1, 0);
				rVLine.DeflateRect(0, 1);
			}

			nLine++;
		}
		while (nLine < nMaxLine);
	}
	else
	{
		if (!bThin)
		{
			color = (nState == DOWN) ? GetColor(COLOR_3DSHADOW, crBkgnd) : 
					(nState == HOT) ? GetColor(COLOR_3DFACE, crBkgnd) : crBkgnd;

			pDC->FillSolidRect(rHLine, color);
			pDC->FillSolidRect(rVLine, color);
			rHLine.OffsetRect(0, 1);
			rVLine.OffsetRect(1, 0);

			if (!nRadius)
			{
				// shorten by one pixel
				rHLine.DeflateRect(1, 0);
				rVLine.DeflateRect(0, 1);
			}
			
			color = (nState == DOWN) ? GetColor(COLOR_3DDKSHADOW, crBkgnd) : 
					(nState == HOT) ? GetColor(COLOR_3DHILIGHT, crBkgnd) : crBkgnd;
		}
		else
		{
			color = (nState == DOWN) ? GetColor(COLOR_3DSHADOW, crBkgnd) : 
					(nState == HOT) ? GetColor(COLOR_3DHILIGHT, crBkgnd) : crBkgnd;
		}

		pDC->FillSolidRect(rHLine, color);
		pDC->FillSolidRect(rVLine, color);
	}
	
	// bottom
	rHLine.SetRect(rect.left + (bBottomLeft ? nRadius : 0), rect.bottom, 
					rect.right - ((bBottomRight && nRadius) ? nRadius : -1), rect.bottom + 1);
	
	// right
	rVLine.SetRect(rect.right, rect.top + (bTopRight ? nRadius : 0), 
					rect.right + 1, rect.bottom - ((bBottomRight && nRadius) ? nRadius : -1));
	
	if (bRender && crBkgnd != -1)
	{
		int nLine = 0;
		int nMaxLine = nRadius ? min(NUMFACTORS, nRadius) : 
								min(NUMFACTORS, min(rect.Width(), rect.Height()) / 2);

		do
		{
			int nFactor = (nLine * NUMFACTORS) / nMaxLine;

			COLORREF color = (nState == DOWN) ? Lighter(crBkgnd, fHiliteFactor[nFactor]) : 
							(nState == HOT) ? Darker(crBkgnd, fShadowFactor[nFactor]) : crBkgnd;

			pDC->FillSolidRect(rHLine, color);
			pDC->FillSolidRect(rVLine, color);
			rHLine.OffsetRect(0, -1);
			rVLine.OffsetRect(-1, 0);

			if (!nRadius)
			{
				// shorten by one pixel
				rHLine.DeflateRect(1, 0);
				rVLine.DeflateRect(0, 1);
			}

			nLine++;
		}
		while (nLine < nMaxLine);
	}
	else
	{
		if (!bThin)
		{
			color = (nState == DOWN) ? GetColor(COLOR_3DHILIGHT, crBkgnd) : 
					(nState == HOT) ? GetColor(COLOR_3DDKSHADOW, crBkgnd) : crBkgnd;

			pDC->FillSolidRect(rHLine, color);
			pDC->FillSolidRect(rVLine, color);
			rHLine.OffsetRect(0, -1);
			rVLine.OffsetRect(-1, 0);

			if (!nRadius)
			{
				// shorten by one pixel
				rHLine.DeflateRect(1, 0); 
				rVLine.DeflateRect(0, 1);
			}
			
			color = (nState == DOWN) ? GetColor(COLOR_3DFACE, crBkgnd) : 
					(nState == HOT) ? GetColor(COLOR_3DSHADOW, crBkgnd) : crBkgnd;
		}
		else
		{
			color = (nState == DOWN) ? GetColor(COLOR_3DHIGHLIGHT, crBkgnd) : 
					(nState == HOT) ? GetColor(COLOR_3DSHADOW, crBkgnd) : crBkgnd;
		}

		pDC->FillSolidRect(rHLine, color);
		pDC->FillSolidRect(rVLine, color);
	}
}

COLORREF CRoundCorner::GetColor(int nAngle, int nDegreesFrom, int nDegreesTo, COLORREF crFrom, COLORREF crTo)
{
	if (crFrom == crTo)
		return crFrom;

	COLORREF crResult = RGB(0, 0, 0);

	if (nDegreesFrom > nDegreesTo)
		nDegreesTo += 360;

	ASSERT (nAngle >= nDegreesFrom && nAngle <= nDegreesTo);

	if (nAngle >= nDegreesFrom && nAngle <= nDegreesTo)
	{
		// all this does is to approximate the averageof the two colors
		// at the point being queried
		BYTE byRedFrom = GetRValue(crFrom);
		BYTE byGreenFrom = GetGValue(crFrom);
		BYTE byBlueFrom = GetBValue(crFrom);
		BYTE byRedTo = GetRValue(crTo);
		BYTE byGreenTo = GetGValue(crTo);
		BYTE byBlueTo = GetBValue(crTo);

		int nOffset = nAngle - nDegreesFrom;
		int nDiff = nDegreesTo - nDegreesFrom;

		BYTE byRedResult = (byRedFrom * (nDiff - nOffset) + byRedTo * nOffset) / nDiff;
		BYTE byGreenResult = (byGreenFrom * (nDiff - nOffset) + byGreenTo * nOffset) / nDiff;
		BYTE byBlueResult = (byBlueFrom * (nDiff - nOffset) + byBlueTo * nOffset) / nDiff;

		crResult = RGB(byRedResult, byGreenResult, byBlueResult);
	}

	return crResult;
}

void CRoundCorner::DrawTopLeft(CDC* pDC, LPRECT pRect, int nRadius, DWORD dwStyle, COLORREF crBkgnd)
{
	if (!nRadius)
		return;

	BOOL bRender = (dwStyle & RC_RENDER) && (crBkgnd != -1);
	BOOL bThin = !bRender && (dwStyle & RC_THIN);

	int nState = (dwStyle & RC_DOWN) ? DOWN : (dwStyle & RC_UP) ? HOT : COLD;

	Corner cnr;

	cnr.ptQuadrant = CPoint(-1, 1);
	cnr.ptOriginOffset = CPoint(pRect->left + nRadius, pRect->top + nRadius);
	cnr.nAngleFrom = 270;
	cnr.nAngleTo = 360;

	if (bRender)
	{
		int nLine = 0;
		int nMaxLine = min(NUMFACTORS, nRadius);

		cnr.bInfillBetween = TRUE;

		do
		{
			int nFactor = (nLine * NUMFACTORS) / nMaxLine;

			cnr.crFrom = (nState == DOWN) ? Darker(crBkgnd, fShadowFactor[nFactor]) : 
							(nState == HOT) ? Lighter(crBkgnd, fHiliteFactor[nFactor]) : crBkgnd;

			cnr.crTo = cnr.crFrom;

			DrawCorner(pDC, nRadius, &cnr);
			nRadius--;
			nLine++;
		}
		while (nLine < nMaxLine);
	}
	else
	{
		if (!bThin)
		{
			cnr.crFrom = (nState == DOWN) ? GetColor(COLOR_3DSHADOW, crBkgnd) : 
							(nState == HOT) ? GetColor(COLOR_3DFACE, crBkgnd) : crBkgnd;

			cnr.crTo = cnr.crFrom;

			cnr.bInfillBetween = TRUE;
			DrawCorner(pDC, nRadius, &cnr);
			nRadius--;

			cnr.crFrom = (nState == DOWN) ? GetColor(COLOR_3DDKSHADOW, crBkgnd) : 
							(nState == HOT) ? GetColor(COLOR_3DHILIGHT, crBkgnd) : crBkgnd;
		}
		else
		{
			cnr.crFrom = (nState == DOWN) ? GetColor(COLOR_3DSHADOW, crBkgnd) : 
							(nState == HOT) ? GetColor(COLOR_3DHILIGHT, crBkgnd) : crBkgnd;
		}

		cnr.crTo = cnr.crFrom;

		cnr.bInfillBetween = FALSE;
		DrawCorner(pDC, nRadius, &cnr);
	}
}

void CRoundCorner::DrawTopRight(CDC* pDC, LPRECT pRect, int nRadius, DWORD dwStyle, COLORREF crBkgnd)
{
	if (!nRadius)
		return;

	BOOL bRender = (dwStyle & RC_RENDER) && (crBkgnd != -1);
	BOOL bThin = !bRender && (dwStyle & RC_THIN);

	int nState = (dwStyle & RC_DOWN) ? DOWN : (dwStyle & RC_UP) ? HOT : COLD;

	Corner cnr;

	cnr.ptQuadrant = CPoint(1, 1);
	cnr.ptOriginOffset = CPoint(pRect->right - nRadius, pRect->top + nRadius);
	cnr.nAngleFrom = 0;
	cnr.nAngleTo = 90;

	if (bRender)
	{
		int nLine = 0;
		int nMaxLine = min(NUMFACTORS, nRadius);

		cnr.bInfillBetween = TRUE;

		do
		{
			int nFactor = (nLine * NUMFACTORS) / nMaxLine;

			cnr.crFrom = (nState == DOWN) ? Darker(crBkgnd, fShadowFactor[nFactor]) : 
							(nState == HOT) ? Lighter(crBkgnd, fHiliteFactor[nFactor]) : crBkgnd;

			cnr.crTo = (nState == DOWN) ? Lighter(crBkgnd, fHiliteFactor[nFactor]) : 
							(nState == HOT) ? Darker(crBkgnd, fShadowFactor[nFactor]) : crBkgnd;

			DrawCorner(pDC, nRadius, &cnr);
			nRadius--;
			nLine++;
		}
		while (nLine < nMaxLine);
	}
	else
	{
		if (!bThin)
		{
			cnr.crFrom = (nState == DOWN) ? GetColor(COLOR_3DSHADOW, crBkgnd) : 
							(nState == HOT) ? GetColor(COLOR_3DFACE, crBkgnd) : crBkgnd;

			cnr.crTo = (nState == DOWN) ? GetColor(COLOR_3DFACE, crBkgnd) : 
							(nState == HOT) ? GetColor(COLOR_3DSHADOW, crBkgnd) : crBkgnd;

			cnr.bInfillBetween = TRUE;
			DrawCorner(pDC, nRadius, &cnr);
			nRadius--;

			cnr.crFrom = (nState == DOWN) ? GetColor(COLOR_3DDKSHADOW, crBkgnd) : 
							(nState == HOT) ? GetColor(COLOR_3DHILIGHT, crBkgnd) : crBkgnd;

			cnr.crTo = (nState == DOWN) ? GetColor(COLOR_3DHILIGHT, crBkgnd) : 
							(nState == HOT) ? GetColor(COLOR_3DDKSHADOW, crBkgnd) : crBkgnd;
		}
		else
		{
			cnr.crFrom = (nState == DOWN) ? GetColor(COLOR_3DSHADOW, crBkgnd) : 
							(nState == HOT) ? GetColor(COLOR_3DHILIGHT, crBkgnd) : crBkgnd;

			cnr.crTo = (nState == DOWN) ? GetColor(COLOR_3DHILIGHT, crBkgnd) : 
							(nState == HOT) ? GetColor(COLOR_3DSHADOW, crBkgnd) : crBkgnd;
		}

		cnr.bInfillBetween = FALSE;
		DrawCorner(pDC, nRadius, &cnr);
	}
}

void CRoundCorner::DrawBottomLeft(CDC* pDC, LPRECT pRect, int nRadius, DWORD dwStyle, COLORREF crBkgnd)
{
	if (!nRadius)
		return;

	BOOL bRender = (dwStyle & RC_RENDER) && (crBkgnd != -1);
	BOOL bThin = !bRender && (dwStyle & RC_THIN);

	int nState = (dwStyle & RC_DOWN) ? DOWN : (dwStyle & RC_UP) ? HOT : COLD;

	Corner cnr;

	cnr.ptQuadrant = CPoint(-1, -1);
	cnr.ptOriginOffset = CPoint(pRect->left + nRadius, pRect->bottom - nRadius);
	cnr.nAngleFrom = 180;
	cnr.nAngleTo = 270;

	if (bRender)
	{
		int nLine = 0;
		int nMaxLine = min(NUMFACTORS, nRadius);

		cnr.bInfillBetween = TRUE;

		do
		{
			int nFactor = (nLine * NUMFACTORS) / nMaxLine;

			cnr.crFrom = (nState == DOWN) ? Lighter(crBkgnd, fHiliteFactor[nFactor]) : 
							(nState == HOT) ? Darker(crBkgnd, fShadowFactor[nFactor]) : crBkgnd;

			cnr.crTo = (nState == DOWN) ? Darker(crBkgnd, fShadowFactor[nFactor]) : 
							(nState == HOT) ? Lighter(crBkgnd, fHiliteFactor[nFactor]) : crBkgnd;

			DrawCorner(pDC, nRadius, &cnr);
			nRadius--;
			nLine++;
		}
		while (nLine < nMaxLine);
	}
	else
	{
		if (!bThin)
		{
			cnr.crFrom = (nState == DOWN) ? GetColor(COLOR_3DHILIGHT, crBkgnd) : 
							(nState == HOT) ? GetColor(COLOR_3DDKSHADOW, crBkgnd) : crBkgnd;

			cnr.crTo = (nState == DOWN) ? GetColor(COLOR_3DSHADOW, crBkgnd) : 
							(nState == HOT) ? GetColor(COLOR_3DFACE, crBkgnd) : crBkgnd;

			cnr.bInfillBetween = TRUE;
			DrawCorner(pDC, nRadius, &cnr);
			nRadius--;

			cnr.crFrom = (nState == DOWN) ? GetColor(COLOR_3DFACE, crBkgnd) : 
							(nState == HOT) ? GetColor(COLOR_3DSHADOW, crBkgnd) : crBkgnd;

			cnr.crTo = (nState == DOWN) ? GetColor(COLOR_3DDKSHADOW, crBkgnd) : 
							(nState == HOT) ? GetColor(COLOR_3DHILIGHT, crBkgnd) : crBkgnd;
		}
		else
		{
			cnr.crFrom = (nState == DOWN) ? GetColor(COLOR_3DHIGHLIGHT, crBkgnd) : 
							(nState == HOT) ? GetColor(COLOR_3DSHADOW, crBkgnd) : crBkgnd;

			cnr.crTo = (nState == DOWN) ? GetColor(COLOR_3DSHADOW, crBkgnd) : 
							(nState == HOT) ? GetColor(COLOR_3DHILIGHT, crBkgnd) : crBkgnd;
		}

		cnr.bInfillBetween = FALSE;
		DrawCorner(pDC, nRadius, &cnr);
	}
}

void CRoundCorner::DrawBottomRight(CDC* pDC, LPRECT pRect, int nRadius, DWORD dwStyle, COLORREF crBkgnd)
{
	if (!nRadius)
		return;

	BOOL bRender = (dwStyle & RC_RENDER) && (crBkgnd != -1);
	BOOL bThin = !bRender && (dwStyle & RC_THIN);

	int nState = (dwStyle & RC_DOWN) ? DOWN : (dwStyle & RC_UP) ? HOT : COLD;

	Corner cnr;

	cnr.ptQuadrant = CPoint(1, -1);
	cnr.ptOriginOffset = CPoint(pRect->right - nRadius, pRect->bottom - nRadius);
	cnr.nAngleFrom = 90;
	cnr.nAngleTo = 180;

	if (bRender)
	{
		int nLine = 0;
		int nMaxLine = min(NUMFACTORS, nRadius);

		cnr.bInfillBetween = TRUE;

		do
		{
			int nFactor = (nLine * NUMFACTORS) / nMaxLine;

			cnr.crFrom = (nState == DOWN) ? Lighter(crBkgnd, fHiliteFactor[nFactor]) : 
							(nState == HOT) ? Darker(crBkgnd, fShadowFactor[nFactor]) : crBkgnd;

			cnr.crTo = cnr.crFrom;

			DrawCorner(pDC, nRadius, &cnr);
			nRadius--;
			nLine++;
		}
		while (nLine < nMaxLine);
	}
	else
	{
		if (!bThin)
		{
			cnr.crFrom = (nState == DOWN) ? GetColor(COLOR_3DHILIGHT, crBkgnd) : 
							(nState == HOT) ? GetColor(COLOR_3DDKSHADOW, crBkgnd) : crBkgnd;

			cnr.crTo = cnr.crFrom;

			cnr.bInfillBetween = TRUE;
			DrawCorner(pDC, nRadius, &cnr);
			nRadius--;

			cnr.crFrom = (nState == DOWN) ? GetColor(COLOR_3DFACE, crBkgnd) : 
							(nState == HOT) ? GetColor(COLOR_3DSHADOW, crBkgnd) : crBkgnd;
		}
		else
		{
			cnr.crFrom = (nState == DOWN) ? GetColor(COLOR_3DHIGHLIGHT, crBkgnd) : 
							(nState == HOT) ? GetColor(COLOR_3DSHADOW, crBkgnd) : crBkgnd;
		}

		cnr.crTo = cnr.crFrom;

		cnr.bInfillBetween = FALSE;
		DrawCorner(pDC, nRadius, &cnr);
	}
}

void CRoundCorner::DrawCorner(CDC* pDC, int nRadius, Corner* pCorner)
{
	CPoint ptBase(0, nRadius);
	int nDecision = 1 - nRadius;

	while (ptBase.y >= ptBase.x)
	{
		// translate to target quadrant
		CPoint ptQuadrant(ptBase.x * pCorner->ptQuadrant.x, ptBase.y * pCorner->ptQuadrant.y);

		// flip to adjacent octant
		int nFlip = ((pCorner->ptQuadrant.x > 0 && pCorner->ptQuadrant.y > 0) ||
					(pCorner->ptQuadrant.x < 0 && pCorner->ptQuadrant.y < 0)) ? 1 : -1;
		
		CPoint ptOctant(ptQuadrant.y * nFlip, ptQuadrant.x * nFlip);

		// adjust y value for MM_TEXT
		ptQuadrant.y = -ptQuadrant.y;
		ptOctant.y = -ptOctant.y;

		// offset to actual position
		ptQuadrant.Offset(pCorner->ptOriginOffset);
		ptOctant.Offset(pCorner->ptOriginOffset);

		// draw pixels
		double dRad = asin((double)ptBase.x / nRadius);
		int nAngle = (int)((dRad * 180) / Pi);

		COLORREF crQuadrant = GetColor(pCorner->nAngleFrom + nAngle, 
									pCorner->nAngleFrom, 
									pCorner->nAngleTo, 
									pCorner->crFrom, 
									pCorner->crTo);

		pDC->SetPixelV(ptQuadrant, crQuadrant);

		COLORREF crOctant = GetColor(pCorner->nAngleTo - nAngle, 
									pCorner->nAngleFrom, 
									pCorner->nAngleTo, 
									pCorner->crFrom, 
									pCorner->crTo);

		pDC->SetPixelV(ptOctant, crOctant);

		// next iteration
		if (nDecision < 0)
		{
			nDecision += (2 * ptBase.x + 3);
			ptBase.x++;
		}
		else
		{
			nDecision += (2 * (ptBase.x - ptBase.y) + 5);
			ptBase.y--;
			ptBase.x++;

			if (pCorner->bInfillBetween)
			{
				// draw fill in pixel
				ptQuadrant.y += pCorner->ptQuadrant.y;
				ptOctant.x -= pCorner->ptQuadrant.x;

				// draw pixels
				pDC->SetPixelV(ptQuadrant, crQuadrant);
				pDC->SetPixelV(ptOctant, crOctant);
			}
		}
	}
}

//////////////////////////////////

COLORREF CRoundCorner::GetColor(int nColor, COLORREF crBase)
{
	if (crBase != -1)
	{
		switch (nColor)
		{
		case COLOR_3DHILIGHT:
			return Lighter(crBase);
			
		case COLOR_3DSHADOW:	
			return Darker(crBase, 0.75f);
			
		case COLOR_3DDKSHADOW:	
			return Darker(crBase, 0.2f);

		case COLOR_3DFACE:	
//		case COLOR_PARENTBKGND:
			return crBase;
		}
	}

	// default
	if (nColor == COLOR_PARENTBKGND)
		nColor = COLOR_3DFACE;

	return ::GetSysColor(nColor);
}

COLORREF CRoundCorner::Darker(COLORREF crBase, float fFactor)
{
	if (fFactor <= 0.0f)
		return 0;

	else if (fFactor >= 1.0f)
		return crBase;

	fFactor = min(fFactor, 1.0f);
	fFactor = max(fFactor, 0.0f);

	BYTE bRed, bBlue, bGreen;
	BYTE bRedShadow, bBlueShadow, bGreenShadow;

	bRed = GetRValue(crBase);
	bBlue = GetBValue(crBase);
	bGreen = GetGValue(crBase);

	bRedShadow = (BYTE)(bRed * fFactor) + (bRed % 2);
	bBlueShadow = (BYTE)(bBlue * fFactor) + (bBlue % 2);
	bGreenShadow = (BYTE)(bGreen * fFactor) + (bGreen % 2);

	return RGB(bRedShadow, bGreenShadow, bBlueShadow);
}

COLORREF CRoundCorner::Lighter(COLORREF crBase, float fFactor) 
{
	if (fFactor <= 0.0f)
		return crBase;

	else if (fFactor >= 1.0f)
		return RGB(255, 255, 255);

	fFactor = min(fFactor, 1.0f);
	fFactor = max(fFactor, 0.0f);

	BYTE bRed, bBlue, bGreen;
	BYTE bRedHilite, bBlueHilite, bGreenHilite;

	bRed = GetRValue(crBase);
	bBlue = GetBValue(crBase);
	bGreen = GetGValue(crBase);

	bRedHilite = min(255, bRed + (BYTE)(255 * fFactor));
	bBlueHilite = min(255, bBlue + (BYTE)(255 * fFactor));
	bGreenHilite = min(255, bGreen + (BYTE)(255 * fFactor));

	return RGB(bRedHilite, bGreenHilite, bBlueHilite);
}

