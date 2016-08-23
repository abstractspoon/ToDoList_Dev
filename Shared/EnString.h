// EnString.h: interface for the CEnString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENSTRING_H__BBF8AAC0_9B2C_11D5_95AD_444553540000__INCLUDED_)
#define AFX_ENSTRING_H__BBF8AAC0_9B2C_11D5_95AD_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum { ES_END, ES_WORD, ES_PATH, ES_START };

class ITransText;

class CEnString : public CString  
{
public:
	CEnString();
	CEnString(LPCTSTR lpszFormat, ... );
	CEnString(UINT nFormatID, LPCTSTR szText = NULL);
	CEnString(UINT nFormatID, int nData);
	CEnString(UINT nID, HWND hwndRef);
	CEnString(const CString& str);

	~CEnString(); // non-virtual else it messes up LPCTSTR operator

	void Format(UINT nFormatID, ...);
	void Format(LPCTSTR lpszFormat, ...);

	BOOL LoadString(UINT nFormatID, HWND hwndRef = NULL);
	CSize FormatDC(CDC* pDC, int nWidth, int nStyle = ES_END);
	CSize Draw(CDC* pDC, LPRECT lpRect, int nStyle = ES_END);
	int GetCharacterCount(TCHAR nChar, BOOL bCaseSensitive = TRUE) const;
	int Allocate(int nMinLen);

	BOOL Translate(HWND hwndRef = NULL);
	void DoNotTranslate() const;

	static void SetLocalizer(ITransText* pTT);
	static BOOL TranslateString(CString& sText, HWND hwndRef);
	static BOOL TranslateStrings(CStringArray& aText, HWND hwndRef);
	static void DoNotTranslateString(const CString& sText);
	static int Allocate(CString& str, int nMinLen);
	static int GetCharacterCount(const CString& sText, TCHAR nChar, BOOL bCaseSensitive);
	static int GetCharacterCount(LPCTSTR szText, int nLen, TCHAR nChar, BOOL bCaseSensitive);

protected:
	static ITransText* s_pTT;

protected:
	CSize FormatDCEx(CDC* pDC, int nWidth, int nStyle = ES_END);
	void CompareIgnoreString(LPCTSTR szText);

	static BOOL LoadString(UINT nID, HWND hwndRef, CString& sText);
};

#endif // !defined(AFX_ENSTRING_H__BBF8AAC0_9B2C_11D5_95AD_444553540000__INCLUDED_)
