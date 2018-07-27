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

class CColorBrewerPalette : public CDWordArray
{
public:
	CColorBrewerPalette& operator=(const CColorBrewerPalette& arr)
	{
		Copy(arr);
		return *this;
	}
};

typedef CArray<CColorBrewerPalette, CColorBrewerPalette&> CColorBrewerPaletteArray;

//////////////////////////////////////////////////////////////////////

enum // Flags
{
	CBF_ALLOWSYNTHESIS	= 0x01,
	CBF_TEXTSAFE		= 0x02,
	// CBF_
};

//////////////////////////////////////////////////////////////////////

class CColorBrewer  
{
public:
	CColorBrewer(DWORD dwFlags = 0);

	int GetAllPalettes(CColorBrewerPaletteArray& aPalettes) const;
	
	int GetPalettes(int nNumColors, CColorBrewerPaletteArray& aPalettes, BOOL bAppend = FALSE) const;
	int GetPalettes(COLORBREWER_PALETTETYPE nType, CColorBrewerPaletteArray& aPalettes, int nNumColors = -1, BOOL bAppend = FALSE) const;
	int GetPalettes(LPCTSTR szName, CColorBrewerPaletteArray& aPalettes, BOOL bPartial = TRUE, BOOL bAppend = FALSE) const;

protected:
	DWORD m_dwFlags;

protected:
	void CopyGroup(const COLORBREWER_PALETTEGROUP& group, CColorBrewerPaletteArray& aPalettes) const;
	void CopyPalette(const COLORBREWER_PALETTE& pal, CColorBrewerPaletteArray& aPalettes) const;
	void CopyPalette(const COLORBREWER_PALETTE& palFrom, CColorBrewerPalette& aTo) const;

	BOOL GroupMatches(const COLORBREWER_PALETTEGROUP& group, int nNumColors) const;
	BOOL PaletteMatches(const COLORBREWER_PALETTE& pal, int nNumColors) const;
	int FindPalette(const COLORBREWER_PALETTEGROUP& group, int nNumColors) const;

	BOOL Synthesize(int nNumColors, const COLORBREWER_PALETTEGROUP& groupFrom, CColorBrewerPalette& palTo) const;
	BOOL Synthesize(int nNumColors, const COLORBREWER_PALETTE& palFrom, CColorBrewerPalette& palTo) const;

	static BOOL ValidatePalettes();
	static BOOL ValidateGroup(const COLORBREWER_PALETTEGROUP& group);
	static BOOL ValidatePalette(const COLORBREWER_PALETTE& pal);

	static BOOL GroupMatches(const COLORBREWER_PALETTEGROUP& group, LPCTSTR szName, BOOL bPartial);
	static BOOL GroupMatches(const COLORBREWER_PALETTEGROUP& group, COLORBREWER_PALETTETYPE nType);

	static COLORREF GetAverageColor(COLORREF color1, COLORREF color2);
	
	static BOOL IsTextSafeColor(COLORREF color);
	static int GetTextSafeColorCount(const COLORBREWER_PALETTE& palFrom);
};

#endif // !defined(AFX_COLORBREWER_H__B8945E59_E0C7_4C2A_A688_D0F4C2EAD899__INCLUDED_)
