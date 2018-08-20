// ColorBrewer.h: interface for the CColorBrewer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLORBREWER_H__B8945E59_E0C7_4C2A_A688_D0F4C2EAD899__INCLUDED_)
#define AFX_COLORBREWER_H__B8945E59_E0C7_4C2A_A688_D0F4C2EAD899__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "..\3rdParty\ColorBrewerPalettes.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

class CColorBrewerPalette : public CDWordArray
{
public:
	CColorBrewerPalette& operator=(const COLORBREWER_PALETTE& pal);
	CColorBrewerPalette& operator=(const CColorBrewerPalette& pal);
	CColorBrewerPalette& operator=(const CDWordArray& aColors);

	BOOL operator==(const CColorBrewerPalette& other) const;
	BOOL operator==(const CDWordArray& aColors) const;

	BOOL Set(const CColorBrewerPalette& palBegin, const CColorBrewerPalette& palEnd, COLORREF crMiddle = CLR_NONE);
};

//////////////////////////////////////////////////////////////////////

class CColorBrewerPaletteArray : public CArray<CColorBrewerPalette, CColorBrewerPalette&>
{
public:
	int Find(const CColorBrewerPalette& pal) const;
	int Find(const CDWordArray& aColors) const;
};

//////////////////////////////////////////////////////////////////////

enum // Flags
{
	CBF_SYNTHESIZE	= 0x01,
	CBF_TEXTSAFE	= 0x02,
	// CBF_
};

//////////////////////////////////////////////////////////////////////

class CColorBrewer  
{
public:
	CColorBrewer(DWORD dwFlags = 0);

	// Supports text-safety only
	int GetAllPalettes(CColorBrewerPaletteArray& aPalettes) const;
	int GetPalettes(LPCTSTR szName, CColorBrewerPaletteArray& aPalettes, BOOL bPartial = TRUE, BOOL bAppend = FALSE) const;
	
	// Supports text-safety and synthesis
	// nNumColors must be >= 3
	int GetPalettes(int nNumColors, CColorBrewerPaletteArray& aPalettes, BOOL bAppend = FALSE) const;

	// Supports text-safety
	// Supports synthesis if nNumColors >= 3
	int GetPalettes(COLORBREWER_PALETTETYPE nType, CColorBrewerPaletteArray& aPalettes, int nNumColors = -1, BOOL bAppend = FALSE) const;

	static int FindMatchingPalette(COLORREF color, const CColorBrewerPaletteArray& aPalettes);

protected:
	DWORD m_dwFlags;

protected:
	void CopyGroup(const COLORBREWER_PALETTEGROUP& group, CColorBrewerPaletteArray& aPalettes) const;
	int CopyPalette(const COLORBREWER_PALETTE& pal, CColorBrewerPaletteArray& aPalettes) const;
	int CopyPalette(const COLORBREWER_PALETTE& palFrom, CColorBrewerPalette& aTo) const;

	BOOL GroupMatches(const COLORBREWER_PALETTEGROUP& group, int nNumColors) const;
	BOOL PaletteMatches(const COLORBREWER_PALETTE& pal, int nNumColors) const;
	int FindPalette(const COLORBREWER_PALETTEGROUP& group, int nNumColors) const;

	BOOL SynthesizePalette(int nNumFinalColors, const COLORBREWER_PALETTEGROUP& groupFrom, 
							CColorBrewerPalette& palFinal) const;
	BOOL SynthesizePalette(int nNumFinalColors, COLORBREWER_PALETTETYPE nTypeFrom, 
							const COLORBREWER_PALETTE& palFrom, CColorBrewerPalette& palFinal, 
							int nNumIntervals) const;

	BOOL SampleColors(COLORBREWER_PALETTETYPE nTypeFrom, const COLORBREWER_PALETTE& palFrom, 
						int nNumColors, CColorBrewerPalette& aTo) const;

	static BOOL ValidatePalettes();
	static BOOL ValidateGroup(const COLORBREWER_PALETTEGROUP& group);
	static BOOL ValidatePalette(const COLORBREWER_PALETTE& pal);

	static BOOL GroupMatches(const COLORBREWER_PALETTEGROUP& group, LPCTSTR szName, BOOL bPartial);
	static BOOL GroupMatches(const COLORBREWER_PALETTEGROUP& group, COLORBREWER_PALETTETYPE nType);

	static COLORREF BlendColors(COLORREF color1, int nAmount1, COLORREF color2, int nAmount2);
	
	static BOOL IsTextSafeColor(COLORREF color);
	static int GetTextSafeColorCount(const COLORBREWER_PALETTE& palFrom);

};

#endif // !defined(AFX_COLORBREWER_H__B8945E59_E0C7_4C2A_A688_D0F4C2EAD899__INCLUDED_)
