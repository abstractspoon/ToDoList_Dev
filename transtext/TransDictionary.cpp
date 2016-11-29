// TransDictionary.cpp: implementation of the CTransDictionary class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TransDictionary.h"
#include "TransTextUtils.h"

#include "..\shared\filemisc.h"
#include "..\shared\misc.h"
#include "..\shared\wclassdefines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

static const CString TRANSTEXT_HEADER	= _T("TRANSTEXT");
static const CString TEXTOUT			= _T("TEXTOUT");
static const CString TEXTIN				= _T("TEXTIN");
static const CString ALTERNATIVE		= _T("ALTERNATIVE");
static const CString CLASSID			= _T("CLASSID");
static const CString TRANSLATED			= _T("TRANSLATED");
static const CString DICTVER			= _T("VERSION");
static const CString NEED_TRANSLATION	= _T("NEED_TRANSLATION");
static const CString ITEM				= _T("ITEM");
static const CString CSVCOLUMN_HEADER	= _T("English Text\tTranslated Text\tItem Type");
static const CString PRIMARY_LANGID		= _T("PRIMARY_LANGID");
static const CString STATIC_ID			= _T("65535");
// static CString _T("");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

ITT_TRANSLATEOPTION DICTITEM::s_nTranslationOption = ITTTO_TRANSLATEONLY;

void DICTITEM::SetTranslationOption(ITT_TRANSLATEOPTION nOption)
{
	s_nTranslationOption = nOption;
}

BOOL DICTITEM::WantAddToDictionary()
{
	return (s_nTranslationOption == ITTTO_ADD2DICTIONARY);
}

BOOL DICTITEM::WantTranslateOnly()
{
	return (s_nTranslationOption == ITTTO_TRANSLATEONLY);
}

BOOL DICTITEM::WantUppercase()
{
	return (s_nTranslationOption == ITTTO_UPPERCASE);
}

// --------------------------------

DICTITEM::DICTITEM()
{
}

DICTITEM::DICTITEM(const DICTITEM& di)
{
	*this = di;
}

DICTITEM::DICTITEM(const CString& sText, LPCTSTR szTextOut, LPCTSTR szClassID) 
	: m_sTextIn(sText), m_sClassID(szClassID), m_sTextOut(szTextOut)
{
}

DICTITEM::~DICTITEM()
{
}

DICTITEM& DICTITEM::operator= (const DICTITEM& di)
{
	m_sTextIn = di.m_sTextIn;
	m_sTextOut = di.m_sTextOut;
	m_sClassID = di.m_sClassID;

	Misc::Copy(di.m_mapAlternatives, m_mapAlternatives);

	return *this;
}

// --------------------------------

void DICTITEM::Reset()
{
	m_sTextIn.Empty();
	m_sTextOut.Empty();
	m_sClassID.Empty();

	m_mapAlternatives.RemoveAll();
}

BOOL DICTITEM::IsTranslated() const 
{ 
	return !m_sTextOut.IsEmpty(); 
}

int DICTITEM::GetDlgCtrlID(HWND hWnd)
{
	int nCtrlID = ::GetDlgCtrlID(hWnd);

	return max(nCtrlID, -1);
}

CString DICTITEM::GetTextOut(const CString& sClassID) const
{
	if (m_sClassID == sClassID)
		return m_sTextOut;

	CString sTextOut;
	m_mapAlternatives.Lookup(sClassID, sTextOut);

	return sTextOut;
}

void DICTITEM::FixupFormatString(CString& sFormat)
{
	// check that the char following the % is valid
	static const CString FMTCHARS = _T("cdiouxXeEfgGnps.*012346789 \t\n%");
	int nFind = sFormat.Find('%', 0);

	if (nFind != -1)
	{
		int nLen = sFormat.GetLength();

		while (nFind != -1 && ((nFind + 1) < nLen))
		{
			TCHAR cNext = sFormat[nFind + 1];
			
			if (FMTCHARS.Find(cNext) == -1) // invalid char
			{
				// try flipping the char
				if (_istlower(cNext))
					cNext = _totupper(cNext);
				else
					cNext = _totlower(cNext);

				sFormat.SetAt(nFind + 1, cNext);
				ASSERT(FMTCHARS.Find(cNext) != -1);
			}

			// next
			nFind = sFormat.Find('%', nFind + 1);
		}
	}
}

BOOL DICTITEM::ToCsv(CStringArray& aTransLines, CStringArray& aNeedTransLines) const
{
	aTransLines.RemoveAll();
	aNeedTransLines.RemoveAll();

	if (!m_sTextIn.IsEmpty())
	{
		// replace certain chars in text else they'll trip up the dictionary when it's read back in
		CString sTextIn(m_sTextIn), sTextOut(m_sTextOut);

		TransText::EncodeChars(sTextIn);
		TransText::EncodeChars(sTextOut);
			
		CString sLine;
		sLine.Format(_T("\"%s\"\t\"%s\"\t\"%s\""), sTextIn, sTextOut, m_sClassID);

		if (m_sTextOut.IsEmpty())
			aNeedTransLines.Add(sLine);
		else
			aTransLines.Add(sLine);

		// add alternatives, indented 
		POSITION pos = m_mapAlternatives.GetStartPosition();

		while (pos)
		{
			CString sAltTextOut, sAltClassID;
			m_mapAlternatives.GetNextAssoc(pos, sAltClassID, sAltTextOut);

			if (!sAltClassID.IsEmpty())
			{
				TransText::EncodeChars(sAltTextOut);

				sLine.Format(_T("  \"%s\"\t\"%s\"\t\"%s\""), m_sTextIn, sAltTextOut, sAltClassID);

				if (sAltTextOut.IsEmpty())
					aNeedTransLines.Add(sLine);
				else
					aTransLines.Add(sLine);
			}
		}
	}

	return (aTransLines.GetSize() > 0 || aNeedTransLines.GetSize() > 0);
}

BOOL DICTITEM::FromCsv(const CStringArray& aLines, int& nLine, BOOL bDecodeChars)
{
	const CString& sLine = Misc::GetItem(aLines, nLine);

	if (sLine.Find(NEED_TRANSLATION) == 0 || sLine.Find(TRANSLATED) == 0)
		return FALSE;

	if (FromCsv(sLine, *this, bDecodeChars))
	{
		// check for alternatives
		int nNextLine = nLine + 1;

		while (nNextLine < aLines.GetSize())
		{
			const CString& sNextLine = Misc::GetItem(aLines, nNextLine);
			DICTITEM diAlt;

			if (FromCsv(sNextLine, diAlt, bDecodeChars) && diAlt.m_sTextIn == m_sTextIn)
			{
				ASSERT(!diAlt.m_sClassID.IsEmpty());

				if (!diAlt.m_sClassID.IsEmpty())
					m_mapAlternatives[diAlt.m_sClassID] = diAlt.m_sTextOut;

				nLine++;
				nNextLine++;
			}
			else
				break;
		}

		return TRUE;
	}

	return FALSE;
}

// static helper
BOOL DICTITEM::FromCsv(const CString& sLine, DICTITEM& di, BOOL bDecodeChars)
{
	CStringArray aFields;
	int nNumFields = Misc::Split(sLine, aFields, '\t', TRUE);

	switch (nNumFields)
	{
	case 3:
		di.m_sClassID = aFields[2];
		// fall thru

	case 2:
		di.m_sTextOut = aFields[1];

		if (bDecodeChars)
			TransText::DecodeChars(di.m_sTextOut);
		// fall thru

	case 1:
		di.m_sTextIn = aFields[0];

		if (TransText::PrepareLookupText(di.m_sTextIn, bDecodeChars))
			return TRUE;
	}

	// all else
	return FALSE;
}

BOOL DICTITEM::Merge(const DICTITEM& di)
{
	ASSERT(di.m_sTextIn == m_sTextIn);

	if (di.m_sTextIn != m_sTextIn)
		return FALSE;

	// merge main ID if not exists
	if (!HasClassID(di.m_sClassID))
		m_mapAlternatives[di.m_sClassID] = di.m_sTextOut;

	// merge alternatives if not exist
	POSITION pos = di.m_mapAlternatives.GetStartPosition();
	
	while (pos)
	{
		CString sAltTextOut, sAltClassID;
		di.m_mapAlternatives.GetNextAssoc(pos, sAltClassID, sAltTextOut);
		
		if (!HasClassID(sAltClassID))
			m_mapAlternatives[sAltClassID] = sAltTextOut;
	}

	return TRUE;
}

int DICTITEM::GetTextOut(CStringArray& aTextOut) const
{
	aTextOut.RemoveAll();

	if (!m_sTextOut.IsEmpty())
		aTextOut.Add(m_sTextOut);

	// add non-empty alternatives
	POSITION pos = m_mapAlternatives.GetStartPosition();
	
	while (pos)
	{
		CString sAltTextOut, sUnused;
		m_mapAlternatives.GetNextAssoc(pos, sUnused, sAltTextOut);
		
		if (!sAltTextOut.IsEmpty())
			aTextOut.Add(sAltTextOut);
	}

	return aTextOut.GetSize();
}

int DICTITEM::GetClassIDs(CStringArray& aClassIDs) const
{
	aClassIDs.RemoveAll();
	aClassIDs.Add(m_sClassID);

	// alternatives
	POSITION pos = m_mapAlternatives.GetStartPosition();
	
	while (pos)
	{
		CString sNotUsed, sAltClassID;
		m_mapAlternatives.GetNextAssoc(pos, sAltClassID, sNotUsed);

		aClassIDs.Add(sAltClassID);
	}

	return aClassIDs.GetSize();
}

BOOL DICTITEM::HasClassID(const CString& sClassID) const
{
	ASSERT(!sClassID.IsEmpty());

	if (m_sClassID == sClassID)
		return TRUE;

	// check alternatives
	CString sTemp;
	return m_mapAlternatives.Lookup(sClassID, sTemp);
}

void DICTITEM::ClearTextOut()
{
	m_sTextOut.Empty();

	// alternatives
	POSITION pos = m_mapAlternatives.GetStartPosition();
	
	while (pos)
	{
		CString sText, sClassID;

		m_mapAlternatives.GetNextAssoc(pos, sClassID, sText);
		m_mapAlternatives[sClassID] = _T("");
	}
}

BOOL DICTITEM::Fixup()
{
	CString sReplaceID, sReplaceText;
	BOOL bCleaned = FALSE;

	if (NeedFixup(m_sClassID, sReplaceID, sReplaceText))
	{
		// overwrite text if empty
		if (m_sTextOut.IsEmpty())
		{
			m_sTextOut = sReplaceText;
		}

		m_sClassID = sReplaceID;
		m_mapAlternatives.RemoveKey(sReplaceID);
		bCleaned = TRUE;
	}

	// alternatives
	POSITION pos = m_mapAlternatives.GetStartPosition();
	
	while (pos)
	{
		CString sText, sClassID, sReplaceText;
		m_mapAlternatives.GetNextAssoc(pos, sClassID, sText);
		
		if (NeedFixup(sClassID, sReplaceID, sReplaceText))
		{
			// save the text for sClassID before deleting
			if (!sText.IsEmpty() && sReplaceText.IsEmpty())
			{
				m_mapAlternatives[sReplaceID] = sText;
			}

			m_mapAlternatives.RemoveKey(sClassID);
			bCleaned = TRUE;
		}
	}

	return bCleaned;
}

BOOL DICTITEM::NeedFixup(const CString& sClassID, CString& sReplaceID, CString& sReplaceText) const
{
	CStringArray aParts;

	if (Misc::Split(sClassID, aParts, '.', TRUE) == 2)
	{
		const CString& sClass = Misc::GetItem(aParts, 0);
		const CString& sID = Misc::GetItem(aParts, 1);

		if (sClass.CompareNoCase(WC_BUTTON) == 0)
		{
			// cleanup any 'button' text if there is a corresponding
			// 'checkbox', 'radiobutton' or 'groupbox' item with the 
			// same control ID
			LPCTSTR szButtons[3] = { _T("checkbox"), _T("radiobutton"), _T("groupbox") };

			for (int nBtn = 0; nBtn < 3; nBtn++)
			{
				CString sTryID;
				sTryID.Format(_T("%s.%s"), szButtons[nBtn], sID);

				// try top level item
				if (m_sClassID == sTryID)
				{
					sReplaceID = m_sClassID;
					sReplaceText = m_sTextOut;
					return TRUE;
				}
				// try alternatives
				else if (m_mapAlternatives.Lookup(sTryID, sReplaceText))
				{
					sReplaceID = sTryID;
					return TRUE;
				}
			}
		}
		else if (sClass == TransText::GetFriendlyClass(WC_TOOLTIPS))
		{
			// Replace any 'tooltip.ID' with 'tooltip'

			// try top level item
			if (m_sClassID == sClass)
			{
				sReplaceID = sClass;
				sReplaceText = m_sTextOut;
				return TRUE;
			}
			// try alternatives
			else if (m_mapAlternatives.Lookup(sClass, sReplaceText))
			{
				sReplaceID = sClass;
				return TRUE;
			}
			// just strip off the ID part
			else if (!sID.IsEmpty())
			{
				sReplaceID = sClass;
				return TRUE;
			}
		}

		// cleanup any items with '.65535'
		if (sID == STATIC_ID)
		{
			// look for item with same class without ctrlID
			if (m_sClassID == sClass)
			{
				sReplaceID = m_sClassID;
				sReplaceText = m_sTextOut;
				return TRUE;
			}
			
			// try alternatives
			if (m_mapAlternatives.Lookup(sID, sReplaceText))
			{
				sReplaceID = sID;
				return TRUE;
			}

			// not found?
			// just remove the control ID
			sReplaceID = sClass;
			sReplaceText = m_sTextOut;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL DICTITEM::Translate(CString& sText)
{
	ASSERT (!sText.IsEmpty() && sText == m_sTextIn);

	if (m_sTextOut.IsEmpty())
	{
		if (WantUppercase())
		{
			sText.MakeUpper();
			FixupFormatString(sText);
			return TRUE;
		}

		// else
		return FALSE;
	}

	// else
	sText = m_sTextOut;

	return TRUE;
}

BOOL DICTITEM::Translate(CString& sText, HWND hWndRef, LPCTSTR szClassID)
{
	if (!Misc::IsEmpty(szClassID))
		return Translate(sText, szClassID);
	 
	// else
	return Translate(sText, TransText::GetClassIDName(hWndRef));
}

BOOL DICTITEM::Translate(CString& sText, HMENU hMenu, int nMenuID)
{
	return Translate(sText, TransText::GetClassIDName(hMenu, nMenuID)); 
}

BOOL DICTITEM::Translate(CString& sText, const CString& sClassID)
{
	// 1. check for an 'alternative' entry
	if (!sClassID.IsEmpty() && !m_mapAlternatives.IsEmpty())
	{
		CString sTextOut;

		if (m_mapAlternatives.Lookup(sClassID, sTextOut) && !sTextOut.IsEmpty())
		{
			sText = sTextOut;
			return TRUE;
		}
	}

	// 2. check root item
	if (sClassID.IsEmpty() || m_sClassID.IsEmpty() || sClassID == m_sClassID)
	{
		BOOL bTrans = Translate(sText);

		// if the root item has no class ID then use this one
		if (WantAddToDictionary() && m_sClassID.IsEmpty() && !sClassID.IsEmpty())
			m_sClassID = sClassID;

		return bTrans;
	}

	// 3. No translation so add as an alternative and use the root translation
	if (WantAddToDictionary())
		m_mapAlternatives[sClassID] = _T("");

	return Translate(sText);
}

BOOL DICTITEM::GetPossibleDuplicates(DICTITEM& diDup) const
{
	diDup.Reset();
	
	// Build a sorted list of all the class IDs
	CStringArray aClassIDs;
	int nNumIDs = GetClassIDs(aClassIDs);
	
	if (nNumIDs > 1)
	{
		Misc::SortArray(aClassIDs);
		
		CString sLastID(aClassIDs[0]), sLastType(sLastID), sNotUsed;
		Misc::Split(sLastType, sNotUsed, '.');

		BOOL bFirst = TRUE;
		
		for (int nID = 1; nID < nNumIDs; nID++)
		{
			CString sID(aClassIDs[nID]), sType(sID);
			Misc::Split(sType, sNotUsed, '.');
			
			if (sType == sLastType)	// Possible duplicate
			{
				if (bFirst)
				{
					if (diDup.m_sTextIn.IsEmpty())
					{
						diDup.m_sTextIn = m_sTextIn;
						diDup.m_sClassID = sLastID;
					}
					else
					{
						diDup.m_mapAlternatives.SetAt(sLastID, GetTextOut(sLastID));
					}

					bFirst = FALSE;
				}
				
				diDup.m_mapAlternatives.SetAt(sID, GetTextOut(sID));
			}
			else // new class type
			{
				sLastType = sType;
				sLastID = sID;
			}
		}
	}
	
	return (diDup.m_mapAlternatives.GetCount() > 0);
}

BOOL DICTITEM::ModifyItem(const CString& sClassID, const CString& sTextOut)
{
	if (sClassID == m_sClassID)
	{
		if (m_sTextOut != sTextOut)
		{
			m_sTextOut = sTextOut;
			return TRUE;
		}

		return FALSE; // no change
	}

	// else check alternatives
	CString sExistTrans;

	if (m_mapAlternatives.Lookup(sClassID, sExistTrans))
	{
		if (sExistTrans != sTextOut)
		{
			m_mapAlternatives[sClassID] = sTextOut;
			return TRUE;
		}

		return FALSE; // no change
	}
	
	ASSERT(0);
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////

CString CTransDictionary::s_sAppVersion;

///////////////////////////////////////////////////////////////////////////////////

CTransDictionary::CTransDictionary() 
	: 
	m_bDecodeChars(TRUE), 
	m_wDictLanguageID(0)
{
	LANGID nLangID = Misc::GetUserDefaultUILanguage();
	m_wDictLanguageID = (WORD)PRIMARYLANGID(nLangID);

	// estimated size of dictionary
	m_mapItems.InitHashTable(1991); // prime number closest to 2000

	// estimated size of ignore list
	m_mapStringIgnore.InitHashTable(1991); // prime number closest to 2000
}

CTransDictionary::~CTransDictionary()
{
	DeleteDictionary();
}

BOOL CTransDictionary::LoadCsvDictionary(LPCTSTR szDictPath)
{
	if (!FileMisc::HasExtension(szDictPath, _T("csv")) &&
		!FileMisc::HasExtension(szDictPath, _T("txt")))
		return FALSE;

	CString sCsvContents;
	CStdioFileEx file;

	if (file.Open(szDictPath, CFile::modeRead) && file.ReadFile(sCsvContents) && !sCsvContents.IsEmpty())
	{
		file.Close();

		DeleteDictionary();

		CStringArray aLines;
		int nNumLines = Misc::Split(sCsvContents, aLines, '\n');

		if (nNumLines)
		{
			// first line is header
			const CString& sHeader = aLines[0];

			if (sHeader.Find(TRANSTEXT_HEADER) != 0)
				return FALSE;

			// extract dictionary version
			if (sHeader != TRANSTEXT_HEADER)
				m_sDictVersion = sHeader.Mid(TRANSTEXT_HEADER.GetLength() + 1);

			// start reading the lines
			int nStartLine = 1;

			// always check there's more to read
			if (nStartLine >= nNumLines)
				return TRUE;

			const CString& sLine = aLines[nStartLine];

			// extract language ID and skip
			if (sLine.Find(PRIMARY_LANGID) == 0)
			{
				m_wDictLanguageID = (WORD)_ttoi(sLine.Mid(PRIMARY_LANGID.GetLength() + 1));
				nStartLine++;
			}

			// always check there's more to read
			if (nStartLine >= nNumLines)
				return TRUE;

			// skip column header
			if (aLines[nStartLine] == CSVCOLUMN_HEADER)
				nStartLine++;

			// always check there's more to read
			if (nStartLine >= nNumLines)
				return TRUE;

			// the dictionary itself
			for (int nLine = nStartLine; nLine < nNumLines; nLine++)
			{
// #ifdef _DEBUG
// 				const CString& sLine = aLines[nLine];
// #endif
				DICTITEM diTemp;

				// this call will pull all consecutive lines having the same text
				if (diTemp.FromCsv(aLines, nLine, m_bDecodeChars))
				{
					// NOTE: because we separate translated and untranslated
					// text in the dictionary, and the untranslated comes first
					// we need to append the untranslated AFTER the translated
					// so that when we save the dictionary the untranslated 
					// strings get moved to the translated section
					CString sItem(diTemp.GetTextIn());
					DICTITEM* pDI = GetDictItem(sItem, FALSE);

					// if it has we merge the items together
					if (pDI)
					{
						// merge the untranslated dictionary into the 
						// translated one and then delete the untranslated
						// before reassign to the dictionary map
						diTemp.Merge(*pDI);
						delete pDI;
					}

					// assign to map
					pDI = new DICTITEM(diTemp);
					m_mapItems.SetAt(sItem, pDI);
				}			
			}
			
			FixupDictionary();
			IgnoreTranslatedText();
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CTransDictionary::IsReadOnly() const
{
	return (s_sAppVersion.IsEmpty() ||
		    (FileMisc::CompareVersions(s_sAppVersion, m_sDictVersion) < 0));
}

void CTransDictionary::FixupDictionary()
{
	if (IsReadOnly())
		return;

	POSITION pos = m_mapItems.GetStartPosition();
	BOOL bCleaned = FALSE;

	while (pos)
	{
		DICTITEM* pDI = NULL;
		CString sKey;

		m_mapItems.GetNextAssoc(pos, sKey, pDI);
		ASSERT(KeyMatches(sKey, pDI));

		if (pDI->Fixup())
			bCleaned = TRUE;
	}

	if (bCleaned)
		SaveDictionary();
}

void CTransDictionary::DeleteDictionary()
{
	POSITION pos = m_mapItems.GetStartPosition();

	while (pos)
	{
		DICTITEM* pDI = NULL;
		CString sPath;

		m_mapItems.GetNextAssoc(pos, sPath, pDI);
		delete pDI;
	}

	m_mapItems.RemoveAll();
	m_sDictFile.Empty();
	m_sDictVersion.Empty();
}

TD_CLEANUP CTransDictionary::CleanupDictionary(const CTransDictionary& tdMaster, CTransDictionary& tdRemoved)
{
	if (IsReadOnly())
	{
		return TDCLEAN_READONLY;
	}
	else if (FileMisc::CompareVersions(tdMaster.GetDictionaryVersion(), GetDictionaryVersion()) < 0)
	{
		return TDCLEAN_BADVER;
	}
	else if (tdMaster.IsEmpty())
	{
		return TDCLEAN_EMPTY;
	}

	// build a list of all items not found in 'tdMaster'
	POSITION pos = m_mapItems.GetStartPosition();
	CStringArray aMissing;

	DICTITEM* pDI = NULL;
	CString sItem;
	BOOL bCleaned = FALSE;

	while (pos)
	{
		m_mapItems.GetNextAssoc(pos, sItem, pDI);

		if (!tdMaster.HasDictItem(sItem))
		{
			aMissing.Add(pDI->GetTextIn());
			bCleaned = TRUE;
		}
	}

	// and move them to 'dtRemoved'
	int nItem = aMissing.GetSize();

	while (nItem--)
	{
		sItem = aMissing[nItem];

		VERIFY(m_mapItems.Lookup(sItem, pDI));

		if (m_mapItems.RemoveKey(sItem))
			tdRemoved.m_mapItems[sItem] = pDI;
	}

	// now add all 'tdMaster' items not found in 'this'
	pos = tdMaster.m_mapItems.GetStartPosition();
	
	while (pos)
	{
		DICTITEM* pDIMaster = NULL;
		CString sItemMaster;
		
		tdMaster.m_mapItems.GetNextAssoc(pos, sItemMaster, pDIMaster);
		
		if (!HasDictItem(sItemMaster))
		{
			DICTITEM* pDI = new DICTITEM(*pDIMaster);
	
			// remove text out from item before adding
			pDI->ClearTextOut();
			m_mapItems.SetAt(sItemMaster, pDI);

			bCleaned = TRUE;
		}
	}

	return (bCleaned ? TDCLEAN_CHANGE : TDCLEAN_NOCHANGE);
}

void CTransDictionary::IgnoreTranslatedText()
{
	POSITION pos = m_mapItems.GetStartPosition();
	
	while (pos)
	{
		DICTITEM* pDI = NULL;
		CString sKey;
		
		m_mapItems.GetNextAssoc(pos, sKey, pDI);
		ASSERT(KeyMatches(sKey, pDI));

		CStringArray aTextOut;
		int nItem = pDI->GetTextOut(aTextOut);

		while (nItem--)
		{
			CString sItem = aTextOut[nItem];
			ASSERT(!sItem.IsEmpty());

			// Only ignore translated items NOT having the same
			// text as an untranslated string elsewhere
			if (!HasDictItem(sItem))
				IgnoreString(sItem, TRUE);
		}
	}
}

BOOL CTransDictionary::KeyMatches(const CString& sKey, const DICTITEM* pDI) const
{
	if (sKey.IsEmpty() || !pDI)
		return FALSE;

	CString sTextIn(pDI->GetTextIn());
	
	if (!m_bDecodeChars)
	{
		if (!TransText::PrepareLookupText(sTextIn))
			return FALSE;
	}

	return (sKey == sTextIn);
}
	

void CTransDictionary::IgnoreString(const CString& sText, BOOL bPrepare)
{
	if (sText.IsEmpty())
		return;
	
	CString sTemp(sText);
	
	if (bPrepare && !TransText::PrepareLookupText(sTemp))
		return;
	
	m_mapStringIgnore[sTemp] = NULL;
}

BOOL CTransDictionary::WantIgnore(const CString& sText) const
{
	void* pDummy = 0;
	return m_mapStringIgnore.Lookup(sText, pDummy);
}

BOOL CTransDictionary::SaveDictionary(LPCTSTR szAltPath, BOOL bForce)
{
	if (IsReadOnly())
		return FALSE;

	if (!bForce && !DICTITEM::WantAddToDictionary())
		return TRUE; // nothing to do

	if (szAltPath == NULL)
	{
		if (m_sDictVersion.IsEmpty())
			return FALSE;
		else
			szAltPath = m_sDictFile;
	}

	if (SaveCsvDictionary(szAltPath))
	{
		m_sDictFile = szAltPath;
		return TRUE;
	}

	return FALSE;
}

BOOL CTransDictionary::SaveCsvDictionary(LPCTSTR szDictPath) const
{
	ASSERT(!IsReadOnly());

	if (!FileMisc::HasExtension(szDictPath, _T("csv")) && 
		!FileMisc::HasExtension(szDictPath, _T("txt")))
		return FALSE;

	// build csv file
	CStringArray aLines, aTranslated, aNeedTranslation;

	// header
	CString sHeader;
	sHeader.Format(_T("%s %s"), TRANSTEXT_HEADER, s_sAppVersion);
	aLines.Add(sHeader);

	// language identifier
	CString sLangID;
	sLangID.Format(_T("%s %d"), PRIMARY_LANGID, m_wDictLanguageID);
	aLines.Add(sLangID);

	// column header
	aLines.Add(CSVCOLUMN_HEADER);

	// dictionary
	POSITION pos = m_mapItems.GetStartPosition();

	while (pos)
	{
		DICTITEM* pDI = NULL;
		CString sKey;
		CStringArray aTransLines, aNeedTransLines;

		m_mapItems.GetNextAssoc(pos, sKey, pDI);
		ASSERT(KeyMatches(sKey, pDI));

		if (pDI->ToCsv(aTransLines, aNeedTransLines))
		{
			if (pDI->IsTranslated())
			{
				aTranslated.Append(aTransLines);
				aTranslated.Append(aNeedTransLines);
			}
			else
			{
				aNeedTranslation.Append(aTransLines);
				aNeedTranslation.Append(aNeedTransLines);
			}
		}
	}

	// sort by original text to maintain some sort of order
	Misc::SortArray(aTranslated, CompareProc);
	Misc::SortArray(aNeedTranslation, CompareProc);

	// put NEED_TRANSLATION first
	if (aNeedTranslation.GetSize() > 0)
	{
		aLines.Add(NEED_TRANSLATION);
		aLines.Append(aNeedTranslation);
	}

	if (aTranslated.GetSize() > 0)
	{
		aLines.Add(TRANSLATED);
		aLines.Append(aTranslated);
	}

	CString sFileContents = Misc::FormatArray(aLines, _T("\r\n"));

	return FileMisc::SaveFile(szDictPath, sFileContents, SFEF_UTF8);
}

int CTransDictionary::CompareProc(const void* pFirst, const void* pSecond)
{
	ASSERT(pFirst && pSecond);

	const CString& sFirst = *((CString*)pFirst);
	const CString& sSecond = *((CString*)pSecond);

	// compare only between the first set of double-quote (ie the input text only)
	int nFirstStart = sFirst.Find(_T("\""));
	int nFirstEnd = sFirst.Find(_T("\"\t"), nFirstStart + 1);

	int nSecondStart = sSecond.Find(_T("\""));
	int nSecondEnd = sSecond.Find(_T("\"\t"), nSecondStart + 1);

	if ((nFirstEnd - nFirstStart) != (nSecondEnd - nSecondStart)) // not the same input string
		return _tcscmp((LPCTSTR)sFirst + nFirstStart, (LPCTSTR)sSecond + nSecondStart);

	// else only compare up to the tab chars
	int nCompare = _tcsncmp((LPCTSTR)sFirst + nFirstStart, (LPCTSTR)sSecond + nSecondStart, nFirstEnd - nFirstStart);

	// if the string is the same make sure the alternatives 
	// go beneath the primary entry
	if (nCompare == 0)
		nCompare = (nFirstStart - nSecondStart);

	return nCompare;
}

BOOL CTransDictionary::HasDictItem(CString& sText) const
{
	if (TransText::PrepareLookupText(sText) && !WantIgnore(sText))
	{
		DICTITEM* pDI = NULL;
		return m_mapItems.Lookup(sText, pDI);
	}	

	// all else
	return FALSE;
}

DICTITEM* CTransDictionary::GetDictItem(CString& sText, BOOL bAutoCreate)
{
	// check for valid text
	// and that we're ignoring this item
	if (!TransText::PrepareLookupText(sText))
	{
		IgnoreString(sText, FALSE);
		return NULL;
	}
	else if (WantIgnore(sText))
	{
		return NULL;
	}

	// can't auto-create if translating only
	if (DICTITEM::WantTranslateOnly())
		bAutoCreate = FALSE;
	
	// check list of items to be ignored
	// NOTE: it is valid for (text.In == text.Out) so we
	// only check for text out if it would otherwise lead
	// to a new dictionary item
	DICTITEM* pDI = NULL;
	
	if (!m_mapItems.Lookup(sText, pDI) && bAutoCreate)
	{
		pDI = new DICTITEM(sText);
		m_mapItems.SetAt(sText, pDI);
	}
#ifdef DEBUG
	else if (pDI && !pDI->GetTextOut().IsEmpty())
	{
		//int breakpoint = 0;
	}
#endif

	return pDI;
}

BOOL CTransDictionary::LoadDictionary(LPCTSTR szDictPath, BOOL bDecodeChars)
{
	if (s_sAppVersion.IsEmpty())
	{
		ASSERT(0);
		return FALSE;
	}

	m_bDecodeChars = bDecodeChars;

	if (LoadCsvDictionary(szDictPath))
	{
		m_sDictFile = szDictPath;
		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CTransDictionary::Translate(CString& sText, HWND hWndRef, LPCTSTR szClassID)
{
	DICTITEM* pDI = GetDictItem(sText); 

	if (pDI && pDI->Translate(sText, hWndRef, szClassID))
	{
		ASSERT(!sText.IsEmpty());

		// mark text out as being not-translatable
		// else the translated text can itself be translated!
		IgnoreString(sText, FALSE);
		return TRUE;
	}
#ifdef DEBUG
	else
	{
		//int breakpoint = 0;
	}
#endif

	return FALSE;
}

BOOL CTransDictionary::TranslateMenuShortcut(CString& sShortcut)
{
	if (sShortcut.IsEmpty())
		return FALSE;

	// split by '+'
	CStringArray aParts;
	int nNumParts = Misc::Split(sShortcut, aParts, '+');

	// translate each part and rebuild shortcut as we go
	CString sTransShortcut;

	for (int i = 0; i < nNumParts; i++)
	{
		CString& sPart = aParts[i];
		sPart.MakeUpper();

		if (sPart == "CTRL")
		{
			sPart = Misc::GetKeyName(VK_CONTROL);
		}
		else if (sPart == "SHIFT")
		{
			sPart = Misc::GetKeyName(VK_SHIFT);
		}
		else if (sPart == "ALT")
		{
			sPart = Misc::GetKeyName(VK_MENU);
		}
		else if (sPart == "SPACE" || sPart == "SPACEBAR")
		{
			sPart = Misc::GetKeyName(VK_SPACE);
		}
		else if (sPart == "DEL" || sPart == "DELETE")
		{
			sPart = Misc::GetKeyName(VK_DELETE);
		}
		else if (sPart == "ESC" || sPart == "ESCAPE")
		{
			sPart = Misc::GetKeyName(VK_ESCAPE);
		}
		else
		{
			ASSERT(i == (nNumParts - 1));

			// Try for a single key or function key else quit
			BOOL bSingleKey = (sPart.GetLength() == 1);
			BOOL bFuncKey = ((sPart.GetLength() == 2) && (sPart[0] == 'F'));

			if (!bSingleKey && !bFuncKey)
				return FALSE;
		}

		// add delimiter
		if (!sTransShortcut.IsEmpty())
			sTransShortcut += '+';
		
		sTransShortcut += sPart;
	}

	// replace shortcut
	sShortcut = sTransShortcut;

	return (!sShortcut.IsEmpty());
}
			
BOOL CTransDictionary::Translate(CString& sItem, HMENU hMenu, int nMenuID)
{
	// trim off trailing shortcut
	CString sShortcut;
	int nTab = sItem.Find('\t');
				
	// remove it
	if (nTab >= 0)
	{
		sShortcut = sItem.Mid(nTab+1);
		sItem = sItem.Left(nTab);
	}

	DICTITEM* pDI = GetDictItem(sItem);
	
	if (pDI && pDI->Translate(sItem, hMenu, nMenuID))
	{
		// translate and reattach any shortcut
		if (TranslateMenuShortcut(sShortcut))
		{
			sItem += '\t';
			sItem += sShortcut;
		}
		
		// mark text out as being not-translatable
		// else the translated text can itself be translated!
		IgnoreString(sItem, FALSE);
		
		return TRUE;
	}
#ifdef DEBUG
	else
	{
		//int breakpoint = 0;
	}
#endif
	
	return FALSE;
}

BOOL CTransDictionary::GetPossibleDuplicates(CTransDictionary& tdDuplicates) const
{
	POSITION pos = m_mapItems.GetStartPosition();
	DICTITEM* pDI = NULL;
	CString sItem;
	DICTITEM diDup;
	
	while (pos)
	{
		m_mapItems.GetNextAssoc(pos, sItem, pDI);
		
		if (pDI->GetPossibleDuplicates(diDup))
			tdDuplicates.m_mapItems.SetAt(sItem, new DICTITEM(diDup));
	}

	return !tdDuplicates.IsEmpty();
}

BOOL CTransDictionary::ModifyItem(const CString& sTextIn, const CString& sClassID, const CString& sTextOut)
{
	if (s_sAppVersion.IsEmpty())
	{
		ASSERT(0);
		return FALSE;
	}

	CString sTemp(sTextIn);
	DICTITEM* pDI = GetDictItem(sTemp, FALSE);

	if (pDI)
		return pDI->ModifyItem(sClassID, sTextOut);

	// else
	ASSERT(0);
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////



