// SpellChecker.cpp: implementation of the CSpellChecker class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "SpellChecker.h"
#include "..\3rdparty\myspell\MySpell.hxx"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpellChecker::CSpellChecker(LPCTSTR szAffPath, LPCTSTR szDicPath) : m_pMySpell(NULL)
{
	// convert to multibyte strings because MySpell library is not UNICODE aware
#ifdef _UNICODE

	int nLen = lstrlen(szAffPath);
	char* szAffPathMulti = WideToMultiByte(szAffPath, nLen);

	nLen = lstrlen(szDicPath);
	char* szDicPathMulti = WideToMultiByte(szDicPath, nLen);

	m_pMySpell = new MySpell(szAffPathMulti, szDicPathMulti);

	delete [] szAffPathMulti;
	delete [] szDicPathMulti;

#else
	m_pMySpell = new MySpell(szAffPath, szDicPath);
#endif
}

CSpellChecker::~CSpellChecker()
{
	delete m_pMySpell;
}

bool CSpellChecker::IsValid()
{
	return (m_pMySpell && m_pMySpell->isvalid());
}

void CSpellChecker::Release()
{
	delete this;
}

bool CSpellChecker::CheckSpelling(LPCTSTR szWord)
{
#ifdef _UNICODE

	int nLen = lstrlen(szWord);
	char* szWordMulti = WideToMultiByte(szWord, nLen);

	bool bResult = (m_pMySpell && m_pMySpell->spell(szWordMulti) > 0);

	delete [] szWordMulti;

#else
	bool bResult = (m_pMySpell && m_pMySpell->spell(szWord) > 0);
#endif

	return bResult;
}

bool CSpellChecker::CheckSpelling(LPCTSTR szWord, LPTSTR*& pSuggestions, int& nNumSuggestions)
{
	bool bResult = CheckSpelling(szWord);

	if (!bResult && m_pMySpell)
	{
#ifdef _UNICODE

	// must make the spell check calls in ANSI
	int nLen = lstrlen(szWord);
	char* szWordMulti = WideToMultiByte(szWord, nLen);

	char** pSuggestionsMulti = NULL;
	nNumSuggestions = m_pMySpell->suggest(&pSuggestionsMulti, szWordMulti);

	if (pSuggestionsMulti)
	{
		// convert back to unicode
		pSuggestions = new LPTSTR[nNumSuggestions];

		for (int nSuggest = 0; nSuggest < nNumSuggestions; nSuggest++)
		{
			nLen = strlen(pSuggestionsMulti[nSuggest]);
			pSuggestions[nSuggest] = MultiByteToWide(pSuggestionsMulti[nSuggest], nLen);
		}
	}

	delete [] szWordMulti;

#else
		nNumSuggestions = m_pMySpell->suggest(&pSuggestions, szWord);
#endif
	}

	return bResult;
}

void CSpellChecker::FreeSuggestions(LPTSTR*& pSuggestions, int nNumSuggestions)
{
	for (int nSuggest = 0; nSuggest < nNumSuggestions; nSuggest++)
		delete [] pSuggestions[nSuggest];

	delete [] pSuggestions;
	pSuggestions = NULL;
}

#ifdef _UNICODE

WCHAR* CSpellChecker::MultiByteToWide(const char* szFrom, int& nLength, UINT nCodepage)
{
	// Use api convert routine
	int wlen = MultiByteToWideChar(nCodepage, 0, szFrom, nLength, NULL, 0);

	// if wlen == 0, some unknown codepage was probably used.
	//ASSERT((nLength && wlen) || (!nLength && !wlen));

	if (wlen == 0 && nLength) 
		return NULL;

	WCHAR* wTo = new WCHAR[wlen+1];
	wTo[wlen] = 0; // null terminator

	if (wlen)
		MultiByteToWideChar(nCodepage, 0, szFrom, nLength, wTo, wlen);

	nLength = wlen;

	return wTo;
}

char* CSpellChecker::WideToMultiByte(const WCHAR* szFrom, int& nLength, UINT nCodePage)
{
	int clen = WideCharToMultiByte(nCodePage, 0, szFrom, nLength, NULL, 0, NULL, NULL);

	// if clen == 0, some unknown codepage was probably used.
	//ASSERT((nLength && clen) || (!nLength && !clen));

	if (clen == 0 && nLength) 
		return NULL;

	char* cTo = new char[clen+1];
	cTo[clen] = 0; // null terminator

	if (clen)
		WideCharToMultiByte(nCodePage, 0, szFrom, nLength, cTo, clen, NULL, NULL);

	nLength = clen;

	return cTo;
}

#endif // _UNICODE