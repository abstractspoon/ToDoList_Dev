// ColorBrewer.cpp: implementation of the CColorBrewer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ColorBrewer.h"

#include "..\3rdParty\ColorDef.h"

#include <shlwapi.h>

//////////////////////////////////////////////////////////////////////

BYTE TEXTFRIENDLY_TOLERANCE = (255 / 5); // 20%

//////////////////////////////////////////////////////////////////////

CColorBrewerPalette& CColorBrewerPalette::operator=(const COLORBREWER_PALETTE& pal)
{
	ASSERT(sizeof(DWORD) == sizeof(COLORREF));

	SetSize(pal.nNumColors);
	CopyMemory(GetData(), pal.crPalette, (pal.nNumColors * sizeof(COLORREF)));

	return *this;
}

CColorBrewerPalette& CColorBrewerPalette::operator=(const CColorBrewerPalette& pal)
{
	Copy(pal);
	return *this;
}

BOOL CColorBrewerPalette::operator==(const CColorBrewerPalette& other) const
{
	int nColor = GetSize();

	if (nColor != other.GetSize())
		return FALSE;

	while (nColor--)
	{
		if (GetAt(nColor) != other[nColor])
			return FALSE;
	}
		
	return TRUE;
}

//////////////////////////////////////////////////////////////////////

int CColorBrewerPaletteArray::Find(const CColorBrewerPalette& pal) const
{
	int nPal = GetSize();

	while (nPal--)
	{
		if (GetData()[nPal] == pal)
			return nPal;
	}

	// Not found
	return -1;
}

//////////////////////////////////////////////////////////////////////

CColorBrewer::CColorBrewer(DWORD dwFlags) : m_dwFlags(dwFlags)
{
}

int CColorBrewer::GetAllPalettes(CColorBrewerPaletteArray& aPalettes) const
{
#ifdef _DEBUG
	ValidatePalettes();
#endif
	aPalettes.RemoveAll();

	for (int nGroup = 0; nGroup < COLORBREWER_NUMGROUPS; nGroup++)
		CopyGroup(COLORBREWER_GROUPS[nGroup], aPalettes);

	return aPalettes.GetSize();
}

int CColorBrewer::GetPalettes(LPCTSTR szName, CColorBrewerPaletteArray& aPalettes, BOOL bPartial, BOOL bAppend) const
{
#ifdef _DEBUG
	ValidatePalettes();
#endif
	if (!bAppend)
		aPalettes.RemoveAll();

	for (int nGroup = 0; nGroup < COLORBREWER_NUMGROUPS; nGroup++)
	{
		const COLORBREWER_PALETTEGROUP& group = COLORBREWER_GROUPS[nGroup];

		if (GroupMatches(group, szName, bPartial))
		{
			for (int nPal = 0; nPal < group.nNumPalettes; nPal++)
				CopyPalette(group.palettes[nPal], aPalettes);
		}
	}

	return aPalettes.GetSize();
}

int CColorBrewer::GetPalettes(int nNumColors, CColorBrewerPaletteArray& aPalettes, BOOL bAppend) const
{
#ifdef _DEBUG
	ValidatePalettes();
#endif
	if (!bAppend)
		aPalettes.RemoveAll();

	if (nNumColors < COLORBREWER_MINCOLORS)
	{
		return 0;
	}

	for (int nGroup = 0; nGroup < COLORBREWER_NUMGROUPS; nGroup++)
	{
		const COLORBREWER_PALETTEGROUP& group = COLORBREWER_GROUPS[nGroup];
		BOOL bMatchFound = FALSE;

		for (int nPal = 0; nPal < group.nNumPalettes; nPal++)
		{
			if (PaletteMatches(group.palettes[nPal], nNumColors))
			{
				CopyPalette(group.palettes[nPal], aPalettes);
				bMatchFound = TRUE;

				break; // one palette per group
			}
		}

		if (!bMatchFound && (m_dwFlags & CBF_SYNTHESIZE))
		{
			CColorBrewerPalette pal;

			if (SynthesizePalette(nNumColors, group, pal))
			{
				aPalettes.Add(pal);
			}
		}
	}

	return aPalettes.GetSize();
}

int CColorBrewer::GetPalettes(COLORBREWER_PALETTETYPE nType, CColorBrewerPaletteArray& aPalettes, int nNumColors, BOOL bAppend) const
{
#ifdef _DEBUG
	ValidatePalettes();
#endif
	if (!bAppend)
		aPalettes.RemoveAll();

	if ((nNumColors != -1) && (nNumColors < COLORBREWER_MINCOLORS))
		return 0;

	for (int nGroup = 0; nGroup < COLORBREWER_NUMGROUPS; nGroup++)
	{
		const COLORBREWER_PALETTEGROUP& group = COLORBREWER_GROUPS[nGroup];
		BOOL bMatchFound = FALSE;

		if (GroupMatches(group, nType))
		{
			for (int nPal = 0; nPal < group.nNumPalettes; nPal++)
			{
				if (PaletteMatches(group.palettes[nPal], nNumColors))
				{
					CopyPalette(group.palettes[nPal], aPalettes);
					bMatchFound = TRUE;

					if (nNumColors != -1)
						break; // one palette per group
				}
			}

			if (!bMatchFound && (nNumColors != -1) && (m_dwFlags & CBF_SYNTHESIZE))
			{
				CColorBrewerPalette pal;

				if (SynthesizePalette(nNumColors, group, pal))
				{
					aPalettes.Add(pal);
				}
			}
		}
	}

	return aPalettes.GetSize();
}

BOOL CColorBrewer::ValidatePalettes()
{
	int nGroup = COLORBREWER_NUMGROUPS;
	
	while (nGroup--)
	{
		if (!ValidateGroup(COLORBREWER_GROUPS[nGroup]))
		{
			ASSERT(0);
			return FALSE;
		}
	}
	
	// all good
	return TRUE;
}

BOOL CColorBrewer::ValidateGroup(const COLORBREWER_PALETTEGROUP& group)
{
	if ((group.nNumPalettes <= 0) || (group.nNumPalettes > COLORBREWER_MAXPALETTES))
	{
		ASSERT(0);
		return FALSE;
	}
	
	// Check trailing sentinel value
	if (group.nNumPalettes < COLORBREWER_MAXPALETTES)
	{
		if (group.palettes[group.nNumPalettes].nNumColors != 0)
		{
			ASSERT(0);
			return FALSE;
		}
	}
	
	// check palettes
	int nPal = group.nNumPalettes;
	
	while (nPal--)
	{
		if (!ValidatePalette(group.palettes[nPal]))
		{
			ASSERT(0);
			return FALSE;
		}
	}
	
	// all good
	return TRUE;
}

BOOL CColorBrewer::ValidatePalette(const COLORBREWER_PALETTE& pal)
{
	if (pal.nNumColors > COLORBREWER_MAXCOLORS)
	{
		ASSERT(0);
		return FALSE;
	}
	
	// Check trailing sentinel value
	if (pal.nNumColors < COLORBREWER_MAXCOLORS)
	{
		if (pal.crPalette[pal.nNumColors] != CLR_NONE)
		{
			ASSERT(0);
			return FALSE;
		}
	}
	
	// Check colors
	int nCol = pal.nNumColors;
	
	while (nCol--)
	{
		if (pal.crPalette[nCol] == CLR_NONE)
		{
			ASSERT(0);
			return FALSE;
		}
	}
	
	// all good
	return TRUE;
}

BOOL CColorBrewer::GroupMatches(const COLORBREWER_PALETTEGROUP& group, LPCTSTR szName, BOOL bPartial)
{
	if (bPartial)
		return (StrStrI(group.szName, szName) != NULL);
	
	// else
	return (StrCmpI(group.szName, szName) != NULL);
}

BOOL CColorBrewer::GroupMatches(const COLORBREWER_PALETTEGROUP& group, COLORBREWER_PALETTETYPE nType)
{
	return (group.nType == nType);
}

BOOL CColorBrewer::GroupMatches(const COLORBREWER_PALETTEGROUP& group, int nNumColors) const
{
	if (nNumColors == -1)
		return TRUE;
	
	int nPal = group.nNumPalettes;
	
	while (nPal--)
	{
		if (PaletteMatches(group.palettes[nPal], nNumColors))
			return TRUE;
	}
	
	return FALSE;
}

BOOL CColorBrewer::PaletteMatches(const COLORBREWER_PALETTE& pal, int nNumColors) const
{
	if (nNumColors == -1)
		return TRUE;
	
	if (m_dwFlags & CBF_TEXTSAFE)
		return (GetTextSafeColorCount(pal) == nNumColors);

	// else
	return (pal.nNumColors == nNumColors);
}

BOOL CColorBrewer::SynthesizePalette(int nNumFinalColors, const COLORBREWER_PALETTEGROUP& groupFrom, CColorBrewerPalette& palFinal) const
{
	// Sanity checks
	if ((m_dwFlags & CBF_SYNTHESIZE) == 0)
	{
		ASSERT(0);
		return FALSE;
	}

	// Can only synthesize from a group not having a
	// native palette with that number of colours
	if (GroupMatches(groupFrom, nNumFinalColors))
	{
		ASSERT(0);
		return FALSE;
	}

	const int INTERVALS[] = { 2, 3, 4, 5, 6, 7, 8 };
	const int NUMINTERVALS = (sizeof(INTERVALS) / sizeof(int));

	for (int i = 0; i < NUMINTERVALS; i++)
	{
		int nPal = groupFrom.nNumPalettes;

		while (nPal--)
		{
			if (SynthesizePalette(nNumFinalColors, groupFrom.nType, groupFrom.palettes[nPal], palFinal, INTERVALS[i]))
				return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CColorBrewer::SynthesizePalette(int nNumFinalColors, COLORBREWER_PALETTETYPE nTypeFrom, const COLORBREWER_PALETTE& palFrom, CColorBrewerPalette& palFinal, int nNumIntervals) const
{
	switch (nTypeFrom)
	{
	case CBPT_SEQUENTIAL:
		{
			int nReqColorCount = (((nNumFinalColors + nNumIntervals) / nNumIntervals) + 1);

			if ((((nReqColorCount - 1) * nNumIntervals) + 1) < nNumFinalColors)
				return FALSE;

			CColorBrewerPalette temp;
						
			if (CopyPalette(palFrom, temp) != nReqColorCount)
				return FALSE;

			palFinal.SetSize(nNumFinalColors);

			for (int nCol = 0; nCol < nNumFinalColors; nCol++)
			{
				int nTempCol = (nCol / nNumIntervals);
				int nOffset = (nCol % nNumIntervals);

				palFinal[nCol] = BlendColors(temp[nTempCol], 
											(nNumIntervals - nOffset), 
											temp[nTempCol + 1], 
											nOffset);
			}
		}
		break;

	case CBPT_DIVERGING:
		{
			CColorBrewerPalette temp;
			int nPalColorCount = CopyPalette(palFrom, temp);

			// TODO
		}
		break;

	case CBPT_QUALITATIVE:
		// Not supported because no colour has any relationship to its adjacent
		// colours and therefore blending adjacent colours makes no sense
		return FALSE;

	default:
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

int CColorBrewer::FindPalette(const COLORBREWER_PALETTEGROUP& group, int nNumColors) const
{
	for (int nPal = 0; nPal < group.nNumPalettes; nPal++)
	{
		if (PaletteMatches(group.palettes[nPal], nNumColors))
			return nPal;
	}

	// else
	return -1;
}

COLORREF CColorBrewer::BlendColors(COLORREF color1, int nAmount1, COLORREF color2, int nAmount2)
{
	if (nAmount1 == 0)
		return color2;

	if (nAmount2 == 0)
		return color1;

	// else
	BYTE nRed = (BYTE)(((GetRValue(color1) * nAmount1) + (GetRValue(color2) * nAmount2)) / (nAmount1 + nAmount2));
	BYTE nGreen = (BYTE)(((GetGValue(color1) * nAmount1) + (GetGValue(color2) * nAmount2)) / (nAmount1 + nAmount2));
	BYTE nBlue = (BYTE)(((GetBValue(color1) * nAmount1) + (GetBValue(color2) * nAmount2)) / (nAmount1 + nAmount2));

	return RGB(nRed, nGreen, nBlue);
}

void CColorBrewer::CopyGroup(const COLORBREWER_PALETTEGROUP& group, CColorBrewerPaletteArray& aPalettes) const 
{
	for (int nPal = 0; nPal < group.nNumPalettes; nPal++)
		CopyPalette(group.palettes[nPal], aPalettes);
}

int CColorBrewer::CopyPalette(const COLORBREWER_PALETTE& pal, CColorBrewerPaletteArray& aPalettes) const
{
	int nNew = aPalettes.Add(CColorBrewerPalette());

	return CopyPalette(pal, aPalettes[nNew]);
}

int CColorBrewer::CopyPalette(const COLORBREWER_PALETTE& palFrom, CColorBrewerPalette& aTo) const
{
	if (m_dwFlags & CBF_TEXTSAFE)
	{
		for (int nCol = 0; nCol < palFrom.nNumColors; nCol++)
		{
			if (IsTextSafeColor(palFrom.crPalette[nCol]))
				aTo.Add(palFrom.crPalette[nCol]);
		}
	}
	else
	{
		aTo = palFrom;
	}

	return aTo.GetSize();
}

BOOL CColorBrewer::IsTextSafeColor(COLORREF color)
{
	BYTE nBkgndLum = RGBX(GetSysColor(COLOR_WINDOW)).Luminance();
	BYTE nColorLum = RGBX(color).Luminance();

	return (abs(nBkgndLum - nColorLum) >= TEXTFRIENDLY_TOLERANCE);
}

int CColorBrewer::GetTextSafeColorCount(const COLORBREWER_PALETTE& palFrom)
{
	int nCol = palFrom.nNumColors, nCount = 0;

	while (nCol--)
	{
		if (IsTextSafeColor(palFrom.crPalette[nCol]))
			nCount++;
	}

	return nCount;
}
