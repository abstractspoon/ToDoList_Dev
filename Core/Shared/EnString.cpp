// EnString.cpp: implementation of the CEnString class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EnString.h"
#include "Misc.h"

#include "..\Interfaces\itranstext.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

ITransText* CEnString::s_pTT = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnString::CEnString() : CString()
{
}

CEnString::CEnString(LPCTSTR lpszFormat, ... )
{
	CString strFormat(lpszFormat);
		
	if (!strFormat.IsEmpty())
	{
		TranslateString(strFormat);

		va_list argList;
		va_start(argList, lpszFormat);
		CString::FormatV(strFormat, argList);
		va_end(argList);

		CompareIgnoreString(strFormat);
	}
}

CEnString::CEnString(UINT nStrID, HWND hwndRef)
{
	LoadString(nStrID, hwndRef);
}

CEnString::CEnString(UINT nStrID, HMENU hMenu)
{
	LoadString(nStrID, hMenu);
}

CEnString::CEnString(const CString& str) : CString(str)
{

}

CEnString::~CEnString()
{

}

void CEnString::CompareIgnoreString(LPCTSTR szText)
{
	// if there was a change, mark the formatted string as non-translatable
	if (s_pTT && Compare(szText) != 0)
		s_pTT->IgnoreString(*this);
}

void CEnString::Format(LPCTSTR lpszFormat, ...)
{
	CString strFormat(lpszFormat);
		
	if (!strFormat.IsEmpty())
	{
		TranslateString(strFormat);

		va_list argList;
		va_start(argList, lpszFormat);
		CString::FormatV(strFormat, argList);
		va_end(argList);

		CompareIgnoreString(strFormat);
	}
}

void CEnString::Format(UINT nFormatID, ...)
{
	CString strFormat;
		
	if (LoadString(nFormatID, NULL, strFormat))
	{
		va_list argList;
		va_start(argList, nFormatID);
		CString::FormatV(strFormat, argList);
		va_end(argList);

		CompareIgnoreString(strFormat);
	}
}

// static helper
BOOL CEnString::LoadString(UINT nID, HWND hwndRef, CString& sText)
{
	if (sText.LoadString(nID))
	{
		TranslateString(sText, hwndRef);
		return (!sText.IsEmpty());
	}

	return FALSE;
}

BOOL CEnString::LoadString(UINT nID, HWND hwndRef)
{
	return LoadString(nID, hwndRef, *this);
}

BOOL CEnString::LoadString(UINT nID, HMENU hMenu)
{
	CString sText;

	if (sText.LoadString(nID))
	{
		if (TranslateString(sText, hMenu))
			*this = sText;

		return !IsEmpty();
	}

	return FALSE;
}

BOOL CEnString::Translate(HWND hwndRef)
{
	return TranslateString(*this, hwndRef);
}

BOOL CEnString::Translate(HMENU hMenu)
{
	return TranslateString(*this, hMenu);
}

void CEnString::DoNotTranslate() const
{
	DoNotTranslateString(*this);
}

void CEnString::DoNotTranslateString(const CString& sText)
{
	if (s_pTT && !sText.IsEmpty())
		s_pTT->IgnoreString(sText);
}

BOOL CEnString::TranslateStrings(CStringArray& aText, HWND hwndRef)
{
	BOOL bResult = FALSE;

	int nItem = aText.GetSize();

	while (nItem--)
		bResult |= TranslateString(aText[nItem], hwndRef);

	return bResult;
}

BOOL CEnString::TranslateString(CString& sText, HWND hwndRef)
{
	if (s_pTT && !sText.IsEmpty())
	{
		LPTSTR szTranslated = NULL;

		if (s_pTT->TranslateText(sText, hwndRef, szTranslated))
		{
			sText = szTranslated;

			// cleanup
			s_pTT->FreeTextBuffer(szTranslated);

			return TRUE;
		}
	}

	// all else
	return FALSE;
}

BOOL CEnString::TranslateString(CString& sText, HMENU /*hMenu*/)
{
	if (s_pTT && !sText.IsEmpty())
	{
		LPTSTR szTranslated = NULL;

		if (s_pTT->TranslateMenu(sText, szTranslated))
		{
			sText = szTranslated;

			// cleanup
			s_pTT->FreeTextBuffer(szTranslated);

			return TRUE;
		}
	}

	// all else
	return FALSE;
}

int CEnString::GetCharacterCount(TCHAR nChar, BOOL bCaseSensitive) const
{
    return GetCharacterCount(*this, nChar, bCaseSensitive);
}

int CEnString::GetCharacterCount(const CString& sText, TCHAR nChar, BOOL bCaseSensitive)
{
	return GetCharacterCount(sText, sText.GetLength(), nChar, bCaseSensitive);
}

int CEnString::GetCharacterCount(LPCTSTR szText, int nLen, TCHAR nChar, BOOL bCaseSensitive)
{
	if (!szText || !nLen)
		return 0;

    int nCount = 0;
	int n = nLen;

	while (n--)
	{
		ASSERT(*szText);

		if (bCaseSensitive)
		{
			if (*szText == nChar)
				nCount++;
		}
		else
		{
			if (_totupper(*szText) == _totupper(nChar))
				nCount++;
		}

		szText++;
	}
	
    return nCount;
}

CSize CEnString::FormatDC(CDC* pDC, int nWidth, int nStyle)
{
	CRect rect(0, 0, nWidth, 20);
	UINT uFlags = (DT_CALCRECT | DT_SINGLELINE | DT_MODIFYSTRING | DT_NOPREFIX);

	// special case: ES_START
	if (nStyle == ES_START)
	{
		FormatDCEx(pDC, nWidth, nStyle);
	}
	else
	{
		switch (nStyle)
		{
			case ES_END:
				uFlags |= DT_END_ELLIPSIS;
				break;

			case ES_WORD:
				uFlags |= DT_WORD_ELLIPSIS;
				break;

			case ES_PATH:
				uFlags |= DT_PATH_ELLIPSIS;
				break;
		}

		::DrawText(pDC->GetSafeHdc(), GetBuffer(GetLength() + 4), -1, rect, uFlags);
		ReleaseBuffer();

		// if its still too big then do our internal version
		if (rect.Width() > nWidth)
			FormatDCEx(pDC, nWidth, nStyle);
	}

	return pDC->GetTextExtent(*this);
}

CSize CEnString::Draw(CDC* pDC, LPRECT lpRect, int nStyle)
{
	// we must do the format independently of DrawText because we override it
	CEnString sTemp(*this);
	sTemp.FormatDC(pDC, lpRect->right - lpRect->left, nStyle);

	pDC->DrawText(sTemp, lpRect, DT_SINGLELINE);

	return pDC->GetTextExtent(sTemp);
}

CSize CEnString::FormatDCEx(CDC* pDC, int nWidth, int nStyle)
{
	CString& sThis = *this;

	const CString ELLIPSIS(_T("..."));

	CSize sizeText = pDC->GetTextExtent(sThis);
	CSize sizeEllipsis = pDC->GetTextExtent(ELLIPSIS);

	bool bEndEllipsis = (nStyle == ES_END || nStyle == ES_WORD);

	// Remove any existing ellipsis
	if (bEndEllipsis)
		Misc::RemoveSuffix(sThis, ELLIPSIS);
	else
		Misc::RemovePrefix(sThis, ELLIPSIS);
	
	// truncate string if too long adding ellipsis (...)
	if ((sizeText.cx + sizeEllipsis.cx) > nWidth)
	{
	    while ((sizeText.cx + sizeEllipsis.cx) > nWidth)
	    {
			// truncate another char
			if (bEndEllipsis)
				Misc::TrimLast(sThis);
			else
				Misc::TrimFirst(sThis);

			sizeText = pDC->GetTextExtent(sThis);

			if (sThis.IsEmpty())
				break;
		}

		sizeText.cx += sizeEllipsis.cx;
			
		if (bEndEllipsis)
			sThis += ELLIPSIS;
		else
			sThis = ELLIPSIS + sThis;
	}

	return sizeText;
}

void CEnString::SetLocalizer(ITransText* pTT)
{
	s_pTT = pTT;
}

int CEnString::Allocate(int nMinLen)
{
	return Allocate(*this, nMinLen);
}

// static version
int CEnString::Allocate(CString& str, int nMinLen)
{
	int nCurLen = str.GetLength();

	if (nCurLen < nMinLen)
	{
		str.GetBuffer(nMinLen);
		str.ReleaseBuffer(nCurLen);

		return nMinLen;
	}

	return nCurLen;
}
