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

class CXmlItem;

//////////////////////////////////////////////////////////////////////

struct DICTITEM
{
public:
	DICTITEM();
	DICTITEM(const DICTITEM& di);
	DICTITEM(const CXmlItem* pXI);
	DICTITEM(const CString& sTextIn, LPCTSTR szTextOut = NULL, LPCTSTR szClassID = NULL);
	virtual ~DICTITEM();

	DICTITEM& operator= (const DICTITEM& di);

	BOOL Translate(CString& sText);
	BOOL Translate(CString& sText, HWND hWndRef, LPCTSTR szClassID = NULL);
	BOOL Translate(CString& sText, HMENU hMenu, int nMenuID);
	BOOL IsTranslated() const;

	BOOL ToXml(CXmlItem* pXI) const;
	BOOL FromXml(const CXmlItem* pXI);

	BOOL ToCsv(CStringArray& aTransLines, CStringArray& aNeedTransLines) const;
	BOOL FromCsv(const CStringArray& aLines, int& nLine);
	
	BOOL Merge(const DICTITEM& di);
	BOOL HasClassID(const CString& sClassID) const;
	BOOL Fixup();
	BOOL NeedFixup(const CString& sClassID, CString& sReplaceID, CString& sReplaceText) const;
	BOOL GetPossibleDuplicates(DICTITEM& diDup) const;

	void ClearTextOut();
	void Reset();

	const CString& GetTextIn() const { return m_sTextIn; }
	const CString& GetTextOut() const { return m_sTextOut; }
 	void SetTextIn(const CString& sText) { m_sTextIn = sText; }

	static void SetTranslationOption(ITT_TRANSLATEOPTION nOption);
	static ITT_TRANSLATEOPTION GetTranslationOption() { return s_nTranslationOption; }
	static BOOL WantAddToDictionary();
	static BOOL WantTranslateOnly();
	static BOOL WantUppercase();

protected:
	CString m_sTextIn;
	CString m_sTextOut;
	CString m_sClassID;

	static ITT_TRANSLATEOPTION s_nTranslationOption;

	// map alternatives by path
	CMapStringToString m_mapAlternatives;

	static void ToXml(CXmlItem* pXI, const DICTITEM& di);
	static int GetDlgCtrlID(HWND hWnd);
	static BOOL FromCsv(const CString& sLine, DICTITEM& di);

	static void FixupFormatString(CString& sFormat);

	BOOL Translate(CString& sText, const CString& sClassID);
	int GetClassIDs(CStringArray& aClassIDs) const;
	CString GetTextOut(const CString& sClassID) const;
};

//////////////////////////////////////////////////////////////////////

typedef CMap<CString, LPCTSTR, DICTITEM*, DICTITEM*> CDictionaryItems;

class CTransDictionary 
{
public:
	CTransDictionary();
	~CTransDictionary();

	CString GetDictionaryPath() const { return m_sDictFile; }
	CString GetDictionaryVersion() const { return m_sDictVersion; }
	WORD GetDictionaryLanguageID() const { return m_wDictLanguageID; }

	BOOL SaveDictionary(LPCTSTR szAltPath = NULL, BOOL bForce = FALSE);
	BOOL SaveXmlDictionary(LPCTSTR szDictPath) const;
	BOOL SaveCsvDictionary(LPCTSTR szDictPath) const;

	BOOL LoadDictionary(LPCTSTR szDictPath);
	BOOL LoadXmlDictionary(LPCTSTR szDictPath);
	BOOL LoadCsvDictionary(LPCTSTR szDictPath);
	void LoadItem(const CXmlItem* pXI);
	BOOL LoadDictionaryItem(const CXmlItem* pXIDict);
	BOOL IsEmpty() const { return (m_mapItems.GetCount() == 0); }

	BOOL Translate(CString& sText);
	BOOL Translate(CString& sText, HWND hWndRef, LPCTSTR szClassID = NULL);
	BOOL Translate(CString& sText, HMENU hMenu, int nMenuID);

	void DeleteDictionary();
	void FixupDictionary();
	BOOL CleanupDictionary(const CTransDictionary& tdMaster, CTransDictionary& tdRemoved);
	BOOL GetPossibleDuplicates(CTransDictionary& tdDuplicates) const;
	
	void IgnoreString(const CString& sText, BOOL bPrepare);
	BOOL WantIgnore(const CString& sText) const;

	const CDictionaryItems& GetItems() const { return m_mapItems; }
	
protected:
	CString m_sDictFile, m_sDictVersion;
	WORD m_wDictLanguageID;
	CMapStringToPtr m_mapStringIgnore;
	CDictionaryItems m_mapItems;
	
protected:
	DICTITEM* GetDictItem(CString& sText, BOOL bAutoCreate = TRUE);
	BOOL HasDictItem(CString& sText) const;
	BOOL TranslateMenuShortcut(CString& sShortcut);

	static int CompareProc(const void* pFirst, const void* pSecond);
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TRANSDICTIONARY_H__1F1C6D57_D2C6_4C06_9E7C_1E0C54B65D30__INCLUDED_)
