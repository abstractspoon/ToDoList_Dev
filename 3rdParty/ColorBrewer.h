// ColorBrewer.h: interface for the CColorBrewer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLORBREWER_H__B8945E59_E0C7_4C2A_A688_D0F4C2EAD899__INCLUDED_)
#define AFX_COLORBREWER_H__B8945E59_E0C7_4C2A_A688_D0F4C2EAD899__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "ColorBrewerPalettes.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

typedef CArray<COLORBREWER_PALETTE, COLORBREWER_PALETTE&> CColorBrewerPaletteArray;
typedef CArray<CDWordArray, CDWordArray&> CDWordPaletteArray;

//////////////////////////////////////////////////////////////////////

class CColorBrewer  
{
public:
	static int GetAllPalettes(CColorBrewerPaletteArray& aPalettes);
	static int GetAllPalettes(CDWordPaletteArray& aPalettes);

	static int GetPalettes(COLORBREWER_PALETTETYPE nType, CColorBrewerPaletteArray& aPalettes);
	static int GetPalettes(COLORBREWER_PALETTETYPE nType, CDWordPaletteArray& aPalettes);

	static int GetPalettes(int nNumColors, CColorBrewerPaletteArray& aPalettes, BOOL bAllowSynthesis = FALSE);
	static int GetPalettes(int nNumColors, CDWordPaletteArray& aPalettes, BOOL bAllowSynthesis = FALSE);

	static int GetPalettes(COLORBREWER_PALETTETYPE nType, CColorBrewerPaletteArray& aPalettes, int nNumColors = -1, BOOL bAllowSynthesis = FALSE);
	static int GetPalettes(COLORBREWER_PALETTETYPE nType, CDWordPaletteArray& aPalettes, int nNumColors = -1, BOOL bAllowSynthesis = FALSE);

	static int GetPalettes(LPCTSTR szName, CColorBrewerPaletteArray& aPalettes, BOOL bPartial = TRUE);
	static int GetPalettes(LPCTSTR szName, CDWordPaletteArray& aPalettes, BOOL bPartial = TRUE);

protected:
	static BOOL ValidatePalettes();
	static BOOL ValidateGroup(const COLORBREWER_PALETTEGROUP& group);
	static BOOL ValidatePalette(const COLORBREWER_PALETTE& pal);

	static BOOL GroupMatches(const COLORBREWER_PALETTEGROUP& group, LPCTSTR szName, BOOL bPartial);
	static BOOL GroupMatches(const COLORBREWER_PALETTEGROUP& group, COLORBREWER_PALETTETYPE nType);
	static BOOL GroupMatches(const COLORBREWER_PALETTEGROUP& group, int nNumColors);
	static BOOL PaletteMatches(const COLORBREWER_PALETTE& pal, int nNumColors);

	static int CountPalettes(int nNumColors);
	static int CountPalettes(COLORBREWER_PALETTETYPE nType, int nNumColors);
	static int CountPalettes(LPCTSTR szName, BOOL bPartial);

	static void Copy(const COLORBREWER_PALETTE& palFrom, COLORBREWER_PALETTE& palTo);
	static void Copy(const COLORBREWER_PALETTE& palFrom, CDWordArray& aTo);

	static BOOL CanSynthesize(const COLORBREWER_PALETTE& palFrom, int nNumColors);
	static BOOL Synthesize(const COLORBREWER_PALETTE& palFrom, COLORBREWER_PALETTE& palTo, int nNumColors);

	static COLORREF GetAverageColor(COLORREF color1, COLORREF color2);

private:
	template <class T> 
	static int GetAllPalettesT(T& aPalettes)
	{
#ifdef _DEBUG
		ValidatePalettes();
#endif

		int nCount = CountPalettes(-1), nItem = 0;
		aPalettes.SetSize(nCount);

		for (int nGroup = 0; nGroup < COLORBREWER_NUMGROUPS; nGroup++)
		{
			const COLORBREWER_PALETTEGROUP& group = COLORBREWER_GROUPS[nGroup];

			for (int nPal = 0; nPal < group.nNumPalettes; nPal++)
				Copy(group.palettes[nPal], aPalettes[nItem++]);
		}
		
		return aPalettes.GetSize();
	}

	template <class T> 
	static int GetPalettesT(COLORBREWER_PALETTETYPE nType, T& aPalettes, int nNumColors, BOOL bAllowSynthesis)
	{
#ifdef _DEBUG
		ValidatePalettes();
#endif

		int nCount = CountPalettes(nType, nNumColors), nItem = 0;
		aPalettes.SetSize(nCount);

		for (int nGroup = 0; nGroup < COLORBREWER_NUMGROUPS; nGroup++)
		{
			const COLORBREWER_PALETTEGROUP& group = COLORBREWER_GROUPS[nGroup];

			if (GroupMatches(group, nType))
			{
				for (int nPal = 0; nPal < group.nNumPalettes; nPal++)
				{
					if (PaletteMatches(group.palettes[nPal], nNumColors))
						Copy(group.palettes[nPal], aPalettes[nItem++]);
				}
			}
		}

		return aPalettes.GetSize();
	}

	template <class T> 
	static int GetPalettesT(int nNumColors, T& aPalettes, BOOL bAllowSynthesis)
	{
#ifdef _DEBUG
		ValidatePalettes();
#endif

		int nCount = CountPalettes(nNumColors), nItem = 0;
		aPalettes.SetSize(nCount);

		for (int nGroup = 0; nGroup < COLORBREWER_NUMGROUPS; nGroup++)
		{
			const COLORBREWER_PALETTEGROUP& group = COLORBREWER_GROUPS[nGroup];

			for (int nPal = 0; nPal < group.nNumPalettes; nPal++)
			{
				if (PaletteMatches(group.palettes[nPal], nNumColors))
					Copy(group.palettes[nPal], aPalettes[nItem++]);
			}
		}

		return aPalettes.GetSize();
	}

	template <class T> 
	static int GetPalettesT(LPCTSTR szName, T& aPalettes, BOOL bPartial)
	{
#ifdef _DEBUG
		ValidatePalettes();
#endif

		int nCount = CountPalettes(szName, bPartial), nItem = 0;
		aPalettes.SetSize(nCount);

		for (int nGroup = 0; nGroup < COLORBREWER_NUMGROUPS; nGroup++)
		{
			const COLORBREWER_PALETTEGROUP& group = COLORBREWER_GROUPS[nGroup];

			if (GroupMatches(group, szName, bPartial))
			{
				for (int nPal = 0; nPal < group.nNumPalettes; nPal++)
					Copy(group.palettes[nPal], aPalettes[nItem++]);
			}
		}

		return aPalettes.GetSize();
	}

};

#endif // !defined(AFX_COLORBREWER_H__B8945E59_E0C7_4C2A_A688_D0F4C2EAD899__INCLUDED_)
