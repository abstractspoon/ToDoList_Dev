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

enum // Flags
{
	CBF_ALLOWSYNTHESIS = 0x01,
	CBF_EXCLUDEPALECOLORS = 0x02,
	// CBF_
};

//////////////////////////////////////////////////////////////////////

class CColorBrewer  
{
public:
	CColorBrewer(DWORD dwFlags = 0);

	int GetAllPalettes(CColorBrewerPaletteArray& aPalettes) const;
	int GetAllPalettes(CDWordPaletteArray& aPalettes) const;

	int GetPalettes(COLORBREWER_PALETTETYPE nType, CColorBrewerPaletteArray& aPalettes) const;
	int GetPalettes(COLORBREWER_PALETTETYPE nType, CDWordPaletteArray& aPalettes) const;

	int GetPalettes(int nNumColors, CColorBrewerPaletteArray& aPalettes) const;
	int GetPalettes(int nNumColors, CDWordPaletteArray& aPalettes) const;

	int GetPalettes(COLORBREWER_PALETTETYPE nType, CColorBrewerPaletteArray& aPalettes, int nNumColors = -1) const;
	int GetPalettes(COLORBREWER_PALETTETYPE nType, CDWordPaletteArray& aPalettes, int nNumColors = -1) const;

	int GetPalettes(LPCTSTR szName, CColorBrewerPaletteArray& aPalettes, BOOL bPartial = TRUE) const;
	int GetPalettes(LPCTSTR szName, CDWordPaletteArray& aPalettes, BOOL bPartial = TRUE const);

protected:
	DWORD m_dwFlags;

protected:
	void CopyPalette(const COLORBREWER_PALETTE& palFrom, COLORBREWER_PALETTE& palTo) const;
	void CopyPalette(const COLORBREWER_PALETTE& palFrom, CDWordArray& aTo) const;

	BOOL CanSynthesize(const COLORBREWER_PALETTE& palFrom, int nNumColors) const;
	BOOL Synthesize(const COLORBREWER_PALETTE& palFrom, COLORBREWER_PALETTE& palTo, int nNumColors) const;

	static BOOL ValidatePalettes();
	static BOOL ValidateGroup(const COLORBREWER_PALETTEGROUP& group);
	static BOOL ValidatePalette(const COLORBREWER_PALETTE& pal);

	static BOOL GroupMatches(const COLORBREWER_PALETTEGROUP& group, LPCTSTR szName, BOOL bPartial);
	static BOOL GroupMatches(const COLORBREWER_PALETTEGROUP& group, COLORBREWER_PALETTETYPE nType);
	static BOOL GroupMatches(const COLORBREWER_PALETTEGROUP& group, int nNumColors);
	static BOOL PaletteMatches(const COLORBREWER_PALETTE& pal, int nNumColors);

	static COLORREF GetAverageColor(COLORREF color1, COLORREF color2);

private:
	template <class T, class U> 
	void CopyGroupT(const COLORBREWER_PALETTEGROUP& group, const T& aPalettes) const
	{
		for (int nPal = 0; nPal < group.nNumPalettes; nPal++)
			CopyPaletteT(group.palettes[nPal], aPalettes);
	}
	
	template <class T, class U> 
	void CopyPaletteT(const COLORBREWER_PALETTE& pal, const T& aPalettes) const
	{
		U temp;
		CopyPalette(pal, temp);
		aPalettes.Add(temp);
	}

	template <class T, class U> 
	int GetAllPalettesT(T& aPalettes) const
	{
#ifdef _DEBUG
		ValidatePalettes();
#endif
		aPalettes.RemoveAll();

		for (int nGroup = 0; nGroup < COLORBREWER_NUMGROUPS; nGroup++)
			CopyGroupT(COLORBREWER_GROUPS[nGroup], aPalettes);
		
		return aPalettes.GetSize();
	}

	template <class T> 
	int GetPalettesT(COLORBREWER_PALETTETYPE nType, T& aPalettes, int nNumColors) const
	{
#ifdef _DEBUG
		ValidatePalettes();
#endif
		aPalettes.RemoveAll();

		for (int nGroup = 0; nGroup < COLORBREWER_NUMGROUPS; nGroup++)
		{
			const COLORBREWER_PALETTEGROUP& group = COLORBREWER_GROUPS[nGroup];

			if (GroupMatches(group, nType))
			{
				for (int nPal = 0; nPal < group.nNumPalettes; nPal++)
				{
					if (PaletteMatches(group.palettes[nPal], nNumColors))
						CopyPaletteT(group.palettes[nPal], aPalettes);
				}
			}
		}

		return aPalettes.GetSize();
	}

	template <class T> 
	int GetPalettesT(int nNumColors, T& aPalettes) const
	{
#ifdef _DEBUG
		ValidatePalettes();
#endif
		aPalettes.RemoveAll();

		for (int nGroup = 0; nGroup < COLORBREWER_NUMGROUPS; nGroup++)
		{
			const COLORBREWER_PALETTEGROUP& group = COLORBREWER_GROUPS[nGroup];

			for (int nPal = 0; nPal < group.nNumPalettes; nPal++)
			{
				if (PaletteMatches(group.palettes[nPal], nNumColors))
					CopyPaletteT(group.palettes[nPal], aPalettes);
			}
		}

		return aPalettes.GetSize();
	}

	template <class T> 
	int GetPalettesT(LPCTSTR szName, T& aPalettes, BOOL bPartial) const
	{
#ifdef _DEBUG
		ValidatePalettes();
#endif
		aPalettes.RemoveAll();

		for (int nGroup = 0; nGroup < COLORBREWER_NUMGROUPS; nGroup++)
		{
			const COLORBREWER_PALETTEGROUP& group = COLORBREWER_GROUPS[nGroup];

			if (GroupMatches(group, szName, bPartial))
			{
				for (int nPal = 0; nPal < group.nNumPalettes; nPal++)
				{
					U temp =  { 0 };
					CopyPalette(group.palettes[nPal], temp);
					aPalettes.Add(temp);
				}
			}
		}

		return aPalettes.GetSize();
	}

};

#endif // !defined(AFX_COLORBREWER_H__B8945E59_E0C7_4C2A_A688_D0F4C2EAD899__INCLUDED_)
