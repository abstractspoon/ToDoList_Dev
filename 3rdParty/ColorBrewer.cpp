// ColorBrewer.cpp: implementation of the CColorBrewer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ColorBrewer.h"
#include "ColorDef.h"

#include <shlwapi.h>

//////////////////////////////////////////////////////////////////////

BYTE TEXTFRIENDLY_TOLERANCE = (255 / 5); // 20%

//////////////////////////////////////////////////////////////////////

CColorBrewer::CColorBrewer(DWORD dwFlags) : m_dwFlags(dwFlags)
{
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

BOOL CColorBrewer::Synthesize(int nNumColors, const COLORBREWER_PALETTEGROUP& groupFrom, CColorBrewerPalette& palTo) const
{
	ASSERT(m_dwFlags & CBF_ALLOWSYNTHESIS);

	if (GroupMatches(groupFrom, nNumColors))
		return FALSE;

	switch (groupFrom.nType)
	{
	case CBPT_SEQUENTIAL:
		{
			palTo.SetSize(nNumColors);

			// Copy known colours
// 			int nCol;
// 			for (nCol = 0; nCol < palFrom.nNumColors; nCol++)
// 			{
// 				palTo.crPalette[nCol * 2] = palFrom.crPalette[nCol];
// 			}
// 			
// 			// Calc missing colours
// 			for (nCol = 1; nCol < nNumColors; nCol += 2)
// 			{
// 				palTo.crPalette[nCol] = GetAverageColor(palTo.crPalette[nCol - 1], palTo.crPalette[nCol + 1]);
// 			}
		}
		break;

	case CBPT_DIVERGING:
		break;

	case CBPT_QUALITATIVE:
		break;

	default:
		ASSERT(0);
		return FALSE;
	}

	
	return TRUE;
}

BOOL CColorBrewer::Synthesize(int nNumColors, const COLORBREWER_PALETTE& palFrom, CColorBrewerPalette& palTo) const
{

	return FALSE;
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

COLORREF CColorBrewer::GetAverageColor(COLORREF color1, COLORREF color2)
{
	return RGB(((GetRValue(color1) + GetRValue(color2)) / 2),
				((GetGValue(color1) + GetGValue(color2)) / 2),
				((GetBValue(color1) + GetBValue(color2)) / 2));
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

int CColorBrewer::GetPalettes(int nNumColors, CColorBrewerPaletteArray& aPalettes, BOOL bAppend) const
{
#ifdef _DEBUG
	ValidatePalettes();
#endif
	if (!bAppend)
		aPalettes.RemoveAll();
	
	for (int nGroup = 0; nGroup < COLORBREWER_NUMGROUPS; nGroup++)
	{
		const COLORBREWER_PALETTEGROUP& group = COLORBREWER_GROUPS[nGroup];
		
		for (int nPal = 0; nPal < group.nNumPalettes; nPal++)
		{
			if (PaletteMatches(group.palettes[nPal], nNumColors))
			{
				CopyPalette(group.palettes[nPal], aPalettes);

				if (nNumColors != -1)
					break; // one palette per group
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
	
	for (int nGroup = 0; nGroup < COLORBREWER_NUMGROUPS; nGroup++)
	{
		const COLORBREWER_PALETTEGROUP& group = COLORBREWER_GROUPS[nGroup];
		
		if (GroupMatches(group, nType))
		{
			for (int nPal = 0; nPal < group.nNumPalettes; nPal++)
			{
				if (PaletteMatches(group.palettes[nPal], nNumColors))
				{
					CopyPalette(group.palettes[nPal], aPalettes);

					if (nNumColors != -1)
						break; // one palette per group
				}
			}
		}
	}
	
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

void CColorBrewer::CopyGroup(const COLORBREWER_PALETTEGROUP& group, CColorBrewerPaletteArray& aPalettes) const 
{
	for (int nPal = 0; nPal < group.nNumPalettes; nPal++)
		CopyPalette(group.palettes[nPal], aPalettes);
}

void CColorBrewer::CopyPalette(const COLORBREWER_PALETTE& pal, CColorBrewerPaletteArray& aPalettes) const
{
	CColorBrewerPalette temp;
	CopyPalette(pal, temp);
	aPalettes.Add(temp);
}

void CColorBrewer::CopyPalette(const COLORBREWER_PALETTE& palFrom, CColorBrewerPalette& aTo) const
{
	int nCol = palFrom.nNumColors;

	if (m_dwFlags & CBF_TEXTSAFE)
	{
		while (nCol--)
		{
			if (IsTextSafeColor(palFrom.crPalette[nCol]))
				aTo.Add(palFrom.crPalette[nCol]);
		}
	}
	else // all
	{
		ASSERT(sizeof(COLORREF) == sizeof(DWORD));

		aTo.SetSize(nCol);
		CopyMemory(aTo.GetData(), palFrom.crPalette, (nCol * sizeof(COLORREF)));
	}
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
