// RichEditSpellCheck.cpp: implementation of the CRichEditSpellCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RichEditSpellCheck.h"
#include "RichEditBaseCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const CString DELIMS(" \t\r\n.,:;-/?<>|~!@#$%^&*(){}+=");

CRichEditSpellCheck::CRichEditSpellCheck(CRichEditBaseCtrl& re) : m_re(re)
{
	m_crCurrentWord.cpMin = m_crCurrentWord.cpMax = 0;
}

CRichEditSpellCheck::~CRichEditSpellCheck()
{
}

LPCTSTR CRichEditSpellCheck::GetFirstWord() const
{
	m_crCurrentWord.cpMin = m_crCurrentWord.cpMax = -1;
	m_sLeftover.Empty();
	m_sCurrentWord.Empty();

	// make sure text is current
	m_sText.Empty();
	GetReferenceTextBeingChecked();

	// start
	return GetNextWord();
}

LPCTSTR CRichEditSpellCheck::GetNextWord() const
{
	CHARRANGE cr = { 0 };
	int nLength = 0;

	m_sCurrentWord.Empty();

	// have we got anything left from cleaning up
	// after richedit's mishandling of line endings
	if (!m_sLeftover.IsEmpty())
	{
		// move to start of word
		int nStart = 0;
		
		while (nStart < m_sLeftover.GetLength())
		{
			TCHAR c = m_sLeftover[nStart];

			if (DELIMS.Find(c) == -1)
				break; 

			nStart++;
		}

		// get end of next word
		int nEnd = (nStart + 1);

		while (nEnd < m_sLeftover.GetLength())
		{
			TCHAR c = m_sLeftover[nEnd];

			if (DELIMS.Find(c) != -1)
				break; 
			
			nEnd++;
		}

		if (nStart < m_sLeftover.GetLength())
		{
			m_sCurrentWord = m_sLeftover.Mid(nStart, nEnd - nStart);
			m_sLeftover = m_sLeftover.Mid(nEnd);

			cr.cpMin = (m_crCurrentWord.cpMax + nStart);
			cr.cpMax = (m_crCurrentWord.cpMax + nEnd);
		}
		else
		{
			m_sLeftover.Empty();
		}

		nLength = m_sCurrentWord.GetLength();
	}

	if (nLength == 0)
	{
		// default word detection
		cr.cpMin = m_re.SendMessage(EM_FINDWORDBREAK, WB_RIGHT, m_crCurrentWord.cpMax);
		cr.cpMax = m_re.SendMessage(EM_FINDWORDBREAK, WB_RIGHT, cr.cpMin + 1);
		
		nLength = GetWord(cr, m_sCurrentWord);
	}

	if (nLength)
	{
		m_crCurrentWord = cr;

		// if there's any leading delimiters then trim them off
		m_sCurrentWord.TrimLeft(DELIMS);

		// and update char range
		m_crCurrentWord.cpMin += (nLength - m_sCurrentWord.GetLength());
		nLength = m_sCurrentWord.GetLength();

		// richedit sometimes mis-handles trailing line endings
		// leaving them in and including the next word, so we
		// try to detect it here
		int nFind = m_sCurrentWord.FindOneOf(DELIMS);
		
		if (nFind != -1)
		{
			// store what's left for further processing next time round
			m_sLeftover = m_sCurrentWord.Mid(nFind);

			m_crCurrentWord.cpMax = (m_crCurrentWord.cpMin + nFind);
			m_sCurrentWord = m_sCurrentWord.Left(nFind);
			nLength = m_sCurrentWord.GetLength();
		}

		// if there's any trailing delimiters then trim them off
		m_sCurrentWord.TrimRight(DELIMS);

		// and update char range
		m_crCurrentWord.cpMax -= (nLength - m_sCurrentWord.GetLength());
		nLength = m_sCurrentWord.GetLength();

		// if there was some text but it was all whitespace, return
		// a non-empty string so that searching is not terminated
		// and move the selection to the end of the whitespace
		if (m_sCurrentWord.IsEmpty())
		{
			m_crCurrentWord.cpMin = m_crCurrentWord.cpMax = cr.cpMax;
			m_sCurrentWord = " ";
		}
		//else 
		//{
		//	TRACE(_T("CRichEditSpellCheck::GetNextWord(%s)\n"), m_sCurrentWord);
		//}
	}

	return m_sCurrentWord;
}

LPCTSTR CRichEditSpellCheck::GetReferenceTextBeingChecked() const
{
	if (m_sText.IsEmpty())
		m_re.GetWindowText(m_sText);

	return m_sText;
}

LPCTSTR CRichEditSpellCheck::GetCurrentWord() const
{
	return m_sCurrentWord;
}

int CRichEditSpellCheck::GetWord(const CHARRANGE& cr, CString& sWord) const
{
	if (cr.cpMax > cr.cpMin)
		sWord = m_re.GetTextRange(cr);
	else
		sWord.Empty();

	// else
	return sWord.GetLength();
}

void CRichEditSpellCheck::SelectCurrentWord()
{
	m_re.SetSel(m_crCurrentWord);
	
	// need to make sure line is visible
	CPoint ptSel = m_re.GetCharPos(m_crCurrentWord.cpMax);
	
	CRect rClient;
	m_re.GetClientRect(rClient);
	
	if (ptSel.y >= rClient.bottom)
	{
		while (ptSel.y >= rClient.bottom)
		{
			m_re.LineScroll(1);
			ptSel = m_re.GetCharPos(m_crCurrentWord.cpMax);
		}
		
		// one more for good measure
		m_re.LineScroll(1);
	}
	else if (ptSel.y < rClient.top)
	{
		while (ptSel.y < rClient.top)
		{
			m_re.LineScroll(-1);
			ptSel = m_re.GetCharPos(m_crCurrentWord.cpMax);
		}
		
		// one more for good measure
		m_re.LineScroll(-1);
	}
}

void CRichEditSpellCheck::ReplaceCurrentWord(LPCTSTR szWord)
{
	if (m_sCurrentWord != szWord)
	{
		m_re.SetSel(m_crCurrentWord);
		m_re.ReplaceSel(szWord, TRUE);

		// update cached text
		m_re.GetWindowText(m_sText);

		// update cached selection
		m_crCurrentWord.cpMax += (lstrlen(szWord) - m_sCurrentWord.GetLength());
	}
}

void CRichEditSpellCheck::ClearSelection()
{
	m_crCurrentWord.cpMin = m_crCurrentWord.cpMax;
	m_re.SetSel(m_crCurrentWord);
}
