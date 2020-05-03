// CAcceleratorString.cpp: implementation of the CAcceleratorString class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AcceleratorString.h"
#include "Misc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAcceleratorString::CAcceleratorString()
{
}

CAcceleratorString::CAcceleratorString(const CString& sOther, BOOL bRemoveAccelerator)
	: CString(sOther)
{
	if (bRemoveAccelerator)
		RemoveAccelerator();
}

CAcceleratorString::~CAcceleratorString()
{
}

TCHAR CAcceleratorString::GetAccelerator(BOOL bMakeLowercase) const
{
	return GetAccelerator(*this, bMakeLowercase);
}

int CAcceleratorString::FindAccelerator() const
{
	return FindAccelerator(*this);
}

BOOL CAcceleratorString::HasAccelerator() const
{
	return HasAccelerator(*this);
}

BOOL CAcceleratorString::HasAccelerator(TCHAR cAccel, BOOL bCaseSensitive) const
{
	return HasAccelerator(*this, cAccel, bCaseSensitive);
}

BOOL CAcceleratorString::RemoveAccelerator()
{
	return RemoveAccelerator(*this);
}

TCHAR CAcceleratorString::EnsureUniqueAccelerator(const CString& sExclude)
{
	return EnsureUniqueAccelerator(*this, sExclude);
}

TCHAR CAcceleratorString::SetAcceleratorPos(int nPos)
{
	return SetAcceleratorPos(*this, nPos);
}

int CAcceleratorString::FindAccelerator(const CString& sText)
{
	if (!sText.IsEmpty())
	{
		// Look for first single '&'
		int nAccel = sText.Find('&'), nLast = Misc::LastIndex(sText); 

		while ((nAccel != -1) && (nAccel < nLast))
		{
			int nNext = (nAccel + 1);
			TCHAR cNext = sText[nNext];

			if (cNext != '&')
				return nNext;

			nAccel = sText.Find('&', (nNext + 1)); 
		}
	}

	// no accelerator
	return -1;
}

BOOL CAcceleratorString::HasAccelerator(const CString& sText)
{
	return (FindAccelerator(sText) != -1);
}

BOOL CAcceleratorString::HasAccelerator(const CString& sText, TCHAR cAccel, BOOL bCaseSensitive)
{
	if (!cAccel)
		return FALSE;

	TCHAR cTextAccel = GetAccelerator(sText, FALSE);

	if (!cTextAccel)
		return FALSE;

	if (bCaseSensitive)
		return (cTextAccel == cAccel);

	// else
	return (_totlower(cTextAccel) == _totlower(cAccel));
}

TCHAR CAcceleratorString::GetAccelerator(const CString& sText, BOOL bMakeLowercase)
{
	int nAccel = FindAccelerator(sText);
	TCHAR cAccel = 0;

	if (nAccel != -1)
	{
		ASSERT(nAccel < sText.GetLength());
		cAccel = sText[nAccel];

		if (bMakeLowercase)
			cAccel = _totlower(cAccel);
	}

	return cAccel;
}

CString CAcceleratorString::GetAccelerators(const CStringArray& aText, BOOL bMakeLowercase)
{
	int nNumItems = aText.GetSize();
	CString sAccelerators(' ', nNumItems);

	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		const CString& sText = aText[nItem];
		sAccelerators.SetAt(nItem, GetAccelerator(sText, bMakeLowercase));
	}

	return sAccelerators;
}

BOOL CAcceleratorString::RemoveAccelerator(CString& sText)
{
	int nAccel = FindAccelerator(sText);

	if (nAccel != -1)
	{
		ASSERT(nAccel < sText.GetLength());

		// Remove preceding '&'
		sText = (sText.Left(nAccel - 1) + sText.Mid(nAccel));
		return TRUE;
	}

	// else
	return FALSE;
}

TCHAR CAcceleratorString::SetAcceleratorPos(CString& sText, int nPos)
{
	if ((nPos < 0) || (nPos >= sText.GetLength()))
		return 0;

	TCHAR cAccel = sText[nPos];

	if (!IsValidAccelerator(cAccel))
		return 0;

	sText.Insert(nPos, '&');
	return cAccel;
}

TCHAR CAcceleratorString::EnsureUniqueAccelerator(CString& sText, const CString& sExclude)
{
	if (sText.IsEmpty())
		return 0;

	// First remove duplicates
	int nAccel = FindAccelerator(sText);

	if (nAccel != -1)
	{
		TCHAR cChar = sText[nAccel];

		if (IsValidAccelerator(cChar, sExclude))
			return _totlower(cChar);

		// else remove preceding '&'
		sText = (sText.Left(nAccel - 1) + sText.Mid(nAccel));
	}

	// Find a unique character
	int nTextLen = sText.GetLength();

	// Prefer the start of words
	int nChar;
	TCHAR cPrevChar = 0;

	for (nChar = 0; nChar < nTextLen; nChar++)
	{
		TCHAR cChar = sText[nChar];
		BOOL bCharIsWhiteSpace = ((cChar < 256) && isspace(cChar));
		BOOL bPrevCharIsWhiteSpace = ((cPrevChar < 256) && isspace(cPrevChar));
		
		if ((nChar == 0) || (!bCharIsWhiteSpace && bPrevCharIsWhiteSpace))
		{
			if (IsValidAccelerator(cChar, sExclude) && SetAcceleratorPos(sText, nChar))
				return _totlower(cChar);
		}

		cPrevChar = cChar;
	}

	// else anything will do
	for (nChar = 0; nChar < nTextLen; nChar++)
	{
		TCHAR cChar = sText[nChar];

		if (IsValidAccelerator(cChar, sExclude) && SetAcceleratorPos(sText, nChar))
			return _totlower(cChar);
	}

	// No unique character found
	return 0;
}

BOOL CAcceleratorString::EnsureUniqueAccelerators(CStringArray& aText)
{
	int nNumItems = aText.GetSize();

	if (!nNumItems)
		return TRUE;

	CString sAccelerators = GetAccelerators(aText, FALSE);
	ASSERT(sAccelerators.GetLength() == nNumItems);

	if (Misc::IsEmpty(sAccelerators))
		return FALSE;

	// Replace missing accelerators with spaces for easier debugging
#ifdef _DEBUG
	const TCHAR NULLCHAR = ' ';
	sAccelerators.Replace('\0', NULLCHAR);
#else
	const TCHAR NULLCHAR = '\0';
#endif
	
	// First loop: Process all empty items and those with valid accelerators.
	// Where duplicates arise, prefer uppercase accelerators
	CUIntArray aProcessed;
	aProcessed.SetSize(nNumItems);

	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		const CString& sText = aText[nItem];
		TCHAR cAccel = sAccelerators[nItem];

		if (sText.IsEmpty())
		{
			ASSERT(sAccelerators[nItem] == NULLCHAR);

			aProcessed[nItem] = TRUE;
			continue;
		}

		if (cAccel == NULLCHAR) // no accelerator
		{
			// process after
			continue;
		}

		BOOL bIsUppercase = _istupper(cAccel);

		if (bIsUppercase)
		{
			// See if the accelerator is the first occurrence
			int nExist = sAccelerators.Find(cAccel);
			ASSERT(nExist != -1);

			if (nExist >= nItem)
			{
				aProcessed[nItem] = TRUE;
				continue;
			}

			// Process after
			sAccelerators.SetAt(nItem, NULLCHAR);
			continue;
		}

		// Lowercase
		// See if the uppercase accelerator appears ahead
		int nFindUpper = sAccelerators.Find(_totupper(cAccel), nItem);

		if (nFindUpper != -1)
		{
			// Remove the uppercase item
			aProcessed[nFindUpper] = TRUE;
			continue;
		}

		// See if the existing accelerator has already appeared
		int nFindLower = sAccelerators.Find(cAccel);

		if ((nFindLower == -1) || (nFindLower >= nItem))
		{
			aProcessed[nItem] = TRUE;
			continue;
		}

		// Process after
		sAccelerators.SetAt(nItem, NULLCHAR);
	}

	// Loop 2: Give any remaining items unique accelerators
	if (sAccelerators.Remove(NULLCHAR))
	{
#ifdef _DEBUG
		CString sDupes;
#endif
		for (int nItem = 0; nItem < nNumItems; nItem++)
		{
			if (aProcessed[nItem])
				continue;

			CString& sText = aText[nItem];

			TCHAR cAccel = EnsureUniqueAccelerator(sText, sAccelerators);

			if (cAccel != 0)
			{
				sAccelerators += cAccel;
				continue;
			}

			// This means that every char in 'sText' appears in 'sAccelerators'
			// or the string is in a non-latin language eg. Chinese
			int nAccel = Misc::FindFirstOf(sText, sAccelerators);

			if (nAccel == -1) // non-latin language
				break;

			// Use the next char 'sAccelerators' appearing in 'sText'
			cAccel = sAccelerators[nAccel];
			ASSERT(cAccel != NULLCHAR);

			// Move this accelerator to the end of the string
			sAccelerators.Delete(nAccel);
			sAccelerators += cAccel;

			// Set the corresponding string accelerator
			nAccel = Misc::Find(cAccel, sText);
			ASSERT(nAccel != -1);

			cAccel = SetAcceleratorPos(sText, nAccel);

#ifdef _DEBUG
			if (Misc::Find(cAccel, sDupes) != -1)
			{
				int breakpoint = 0;
			}
			sDupes += cAccel;
#endif
		}
	}

	return TRUE;
}

BOOL CAcceleratorString::IsValidAccelerator(TCHAR cChar)
{
	if (cChar >= 256)
		return FALSE;

	return isalnum(cChar);
}

BOOL CAcceleratorString::IsValidAccelerator(TCHAR cChar, const CString& sExclude)
{
	if (!IsValidAccelerator(cChar))
		return FALSE;
	
	if (sExclude.Find(_totlower(cChar)) != -1)
		return FALSE;
	
	return (sExclude.Find(_totupper(cChar)) == -1);
}

