// TransTextMgr.h: interface for the CTransTextMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSDICTIONARY_H__1F1C6D57_D2C6_4C06_9E7C_1E0C54B65D30__INCLUDED_)
#define AFX_TRANSDICTIONARY_H__1F1C6D57_D2C6_4C06_9E7C_1E0C54B65D30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Interfaces\itranstext.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

struct DICTITEM
{
public:
	DICTITEM();
	DICTITEM(const DICTITEM& di);
	DICTITEM(const CString& sTextIn, LPCTSTR szTextOut = NULL, LPCTSTR szClassID = NULL);
	virtual ~DICTITEM();

	DICTITEM& operator= (const DICTITEM& di);

	BOOL Translate(CString& sText);
	BOOL Translate(CString& sText, HWND hWndRef, const CString& sClassID, BOOL bValidateAccelerator);
	BOOL Translate(CString& sText, HMENU hMenu, BOOL bValidateAccelerator);
	BOOL IsTranslated() const;

	BOOL ToCsv(CStringArray& aTransLines, CStringArray& aNeedTransLines) const;
	BOOL FromCsv(const CStringArray& aLines, int& nLine, BOOL bDecodeChars = TRUE);
	
	BOOL Merge(const DICTITEM& di);
	BOOL HasClassID(const CString& sClassID) const;
	BOOL Fixup();
	BOOL Cleanup(const DICTITEM& diMaster);
	BOOL ModifyItem(const CString& sClassID, const CString& sTextOut);

	void ClearTextOut();
	void Reset();

	const CString& GetTextIn() const { return m_sTextIn; }
	const CString& GetTextOut() const { return m_sTextOut; }
	const CString& GetClassID() const { return m_sClassID; }
	const CMapStringToString& GetAlternatives() const { return m_mapAlternatives; }
	int GetTextOut(CStringArray& aTextOut) const; // including alternatives

	static void SetTranslationOption(ITT_TRANSLATEOPTION nOption);
	static ITT_TRANSLATEOPTION GetTranslationOption() { return s_nTranslationOption; }
	static BOOL WantAddToDictionary();
	static BOOL WantTranslateOnly();

protected:
	CString m_sTextIn;
	CString m_sTextOut;
	CString m_sClassID;

	CMapStringToString m_mapAlternatives;

	static ITT_TRANSLATEOPTION s_nTranslationOption;

protected:
	static int GetDlgCtrlID(HWND hWnd);
	static BOOL FromCsv(const CString& sLine, DICTITEM& di, BOOL bDecodeChars);

	static void FixupFormatString(CString& sFormat);

	BOOL Translate(CString& sText, const CString& sClassID);
	int GetClassIDs(CStringArray& aClassIDs) const;
	CString GetTextOut(const CString& sClassID) const;
	BOOL FixupClassID(CString& sClassID) const;
};

//////////////////////////////////////////////////////////////////////

typedef CMap<CString, LPCTSTR, DICTITEM*, DICTITEM*&> CDictItemMap;
typedef CArray<DICTITEM*, DICTITEM*&> CDictItemArray;

//////////////////////////////////////////////////////////////////////

enum TD_CLEANUP
{
	TDCLEAN_NOCHANGE	= 0,
	TDCLEAN_CHANGE		= 1,
	TDCLEAN_BADVER		= -1,
	TDCLEAN_EMPTY		= -2,
	TDCLEAN_READONLY	= -3,
};

//////////////////////////////////////////////////////////////////////

class CTransDictionary 
{
public:
	CTransDictionary();
	~CTransDictionary();

	CString GetDictionaryPath() const { return m_sDictFile; }
	CString GetDictionaryVersion() const { return m_sDictVersion; }
	WORD GetDictionaryLanguageID() const { return m_wDictLanguageID; }
	CString GetDictionaryTwoLetterLanguageCode() const;

	BOOL LoadDictionary(LPCTSTR szDictPath, BOOL bDecodeChars = TRUE);
	BOOL SaveDictionary(LPCTSTR szAltPath = NULL, BOOL bForce = FALSE);
	BOOL IsEmpty() const { return (m_mapItems.GetCount() == 0); }
	BOOL IsReadOnly() const;

	BOOL Translate(CString& sText, HWND hWndRef, LPCTSTR szClassID = NULL, BOOL bValidateAccelerator = TRUE);
	BOOL Translate(CString& sText, HMENU hMenu, BOOL bValidateAccelerator = TRUE);

	BOOL ModifyItem(const CString& sTextIn, const CString& sClassID, const CString& sTextOut);
	int ModifyItems(const CStringArray& aTextIn, const CStringArray& aTextOut);

	TD_CLEANUP CleanupDictionary(const CTransDictionary& tdMaster, CTransDictionary& tdRemoved);
	int GetUntranslatedItems(CStringArray& aItems, BOOL bSorted) const;
	void DeleteDictionary();
	
	void IgnoreString(const CString& sText, BOOL bPrepare);
	BOOL WantIgnore(const CString& sText) const;

	const CDictItemMap& GetItems() const { return m_mapItems; }

	static void SetAppVersion(LPCTSTR szAppVer)	{ s_sAppVersion = szAppVer; }
	static CString GetAppVersion() { return s_sAppVersion; }
		
protected:
	CString m_sDictFile, m_sDictVersion;
	WORD m_wDictLanguageID;
	CMapStringToPtr m_mapStringIgnore;
	CDictItemMap m_mapItems;
	BOOL m_bDecodeChars;

	static CString s_sAppVersion;
	
protected:
	const DICTITEM* GetDictItem(CString& sText) const;
	DICTITEM* GetDictItem(CString& sText, BOOL bAutoCreate);
	BOOL HasDictItem(CString& sText) const;
	BOOL TranslateMenuShortcut(CString& sShortcut);
	void IgnoreTranslatedText();
	void FixupDictionary();
	BOOL KeyMatches(const CString& sKey, const DICTITEM* pDI) const;

	BOOL SaveCsvDictionary(LPCTSTR szDictPath) const;
	BOOL LoadCsvDictionary(LPCTSTR szDictPath);

	static int CompareProc(const void* pFirst, const void* pSecond);
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TRANSDICTIONARY_H__1F1C6D57_D2C6_4C06_9E7C_1E0C54B65D30__INCLUDED_)
