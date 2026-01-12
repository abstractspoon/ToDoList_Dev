// RichEditSpellCheck.h: interface for the CRichEditSpellCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RICHEDITSPELLCHECK_H__9A6FC513_71CB_4207_9FED_7B1429010FE5__INCLUDED_)
#define AFX_RICHEDITSPELLCHECK_H__9A6FC513_71CB_4207_9FED_7B1429010FE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ISpellCheck.h"

class CRichEditBaseCtrl;

class CRichEditSpellCheck : public ISpellCheck  
{
public:
	CRichEditSpellCheck();
	virtual ~CRichEditSpellCheck();

	BOOL Initialise(HWND hwndRE);

	LPCTSTR GetFirstWord() const;
	LPCTSTR GetNextWord() const;
	LPCTSTR GetCurrentWord() const;

	void SelectCurrentWord();
	void ReplaceCurrentWord(LPCTSTR szWord);
	void ClearSelection();

	LPCTSTR GetReferenceTextBeingChecked() const;

protected:
	HWND m_hwndRichEdit;

	mutable CHARRANGE m_crCurrentWord;
	mutable CString m_sCurrentWord;
	mutable CString m_sText;
	mutable CString m_sLeftover;

protected:
	int GetWord(const CHARRANGE& cr, CString& sWord) const;

};

#endif // !defined(AFX_RICHEDITSPELLCHECK_H__9A6FC513_71CB_4207_9FED_7B1429010FE5__INCLUDED_)
