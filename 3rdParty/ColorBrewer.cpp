// ColorBrewer.cpp: implementation of the CColorBrewer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ColorBrewer.h"

#include <shlwapi.h>

//////////////////////////////////////////////////////////////////////

template <class T>
CColorBrewer::CColorBrewer(DWORD dwFlags) : m_dwFlags(dwFlags)
{

}

template <class T>
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

template <class T>
BOOL CColorBrewer::ValidateGroup(const COLORBREWER_PALETTEGROUP& group)
{
	if (group.nNumPalettes > COLORBREWER_MAXPALETTES)
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

template <class T>
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

template <class T>
BOOL CColorBrewer::GroupMatches(const COLORBREWER_PALETTEGROUP& group, LPCTSTR szName, BOOL bPartial)
{
	if (bPartial)
		return (StrStrI(group.szName, szName) != NULL);

	// else
	return (StrCmpI(group.szName, szName) != NULL);
}

template <class T>
BOOL CColorBrewer::GroupMatches(const COLORBREWER_PALETTEGROUP& group, COLORBREWER_PALETTETYPE nType)
{
	return (group.nType == nType);
}

template <class T>
BOOL CColorBrewer::GroupMatches(const COLORBREWER_PALETTEGROUP& group, int nNumColors)
{
	if (nNumColors == -1)
		return TRUE;

	int nPal = group.nNumPalettes;

	while (nPal--)
	{
		if (group.palettes[nPal].nNumColors == nNumColors)
			return TRUE;
	}

	return FALSE;
}

template <class T>
BOOL CColorBrewer::PaletteMatches(const COLORBREWER_PALETTE& pal, int nNumColors)
{
	return ((nNumColors == -1) || (pal.nNumColors == nNumColors));
}

template <class T>
void CColorBrewer::CopyPalette(const COLORBREWER_PALETTE& palFrom, COLORBREWER_PALETTE& palTo) const
{
	palTo = palFrom;
}

void CColorBrewer::CopyPalette(const COLORBREWER_PALETTE& palFrom, CColorrefArray& aTo) const
{
	int nCol = palFrom.nNumColors;
	aTo.SetSize(nCol);

	while (nCol--)
		aTo[nCol] = palFrom.crPalette[nCol];
}

template <class T>
BOOL CColorBrewer::CanSynthesize(const COLORBREWER_PALETTE& palFrom, int nNumColors) const
{
	if ((nNumColors % 2) == 0)
		return FALSE;

	return (palFrom.nNumColors == ((nNumColors / 2) + 1));
}

template <class T>
BOOL CColorBrewer::Synthesize(const COLORBREWER_PALETTE& palFrom, COLORBREWER_PALETTE& palTo, int nNumColors) const
{
	if (!CanSynthesize(palFrom, nNumColors))
		return FALSE;

	palTo.nNumColors = nNumColors;

	// Copy known colours
	for (int nCol = 0; nCol < palFrom.nNumColors; nCol++)
	{
		palTo.crPalette[nCol * 2] = palFrom.crPalette[nCol];
	}

	// Calc missing colours
	for (int nCol = 1; nCol < nNumColors; nCol += 2)
	{
		palTo.crPalette[nCol] = GetAverageColor(palTo.crPalette[nCol - 1], palTo.crPalette[nCol + 1]);
	}

	return TRUE;
}

template <class T>
COLORREF CColorBrewer::GetAverageColor(COLORREF color1, COLORREF color2)
{
	return RGB(((GetRValue(color1) + GetRValue(color2)) / 2),
				((GetGValue(color1) + GetGValue(color2)) / 2),
				((GetBValue(color1) + GetBValue(color2)) / 2));
}

