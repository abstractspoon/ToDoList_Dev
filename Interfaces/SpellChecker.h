// SpellChecker.h: interface for the CSpellChecker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPELLCHECKER_H__74596014_29E0_4289_9BC7_28A7996FA37B__INCLUDED_)
#define AFX_SPELLCHECKER_H__74596014_29E0_4289_9BC7_28A7996FA37B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Interfaces\ISpellCheck.h"

class MySpell;

class CSpellChecker : public ISpellChecker
{
public:
	CSpellChecker(LPCTSTR szAffPath, LPCTSTR szDicPath);
	virtual ~CSpellChecker();

    void Release(); // releases the interface
    bool CheckSpelling(LPCTSTR szWord);
    bool CheckSpelling(LPCTSTR szWord, LPTSTR*& pSuggestions, int& nNumSuggestions);
    void FreeSuggestions(LPTSTR*& pSuggestions, int nNumSuggestions);

	bool IsValid();

protected:
	MySpell* m_pMySpell;

#ifdef _UNICODE
protected:
	WCHAR* MultiByteToWide(const char* szFrom, int& nLength, UINT nCodepage = CP_ACP);
	char* WideToMultiByte(const WCHAR* szFrom, int& nLength, UINT nCodePage = CP_ACP);
#endif
};

#endif // !defined(AFX_SPELLCHECKER_H__74596014_29E0_4289_9BC7_28A7996FA37B__INCLUDED_)
