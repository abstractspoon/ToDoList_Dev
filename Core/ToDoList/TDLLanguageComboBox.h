#if !defined(AFX_TDLLANGUAGECOMBOBOX_H__5EEB7D2C_19CF_460E_B188_646AB514678E__INCLUDED_)
#define AFX_TDLLANGUAGECOMBOBOX_H__5EEB7D2C_19CF_460E_B188_646AB514678E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLLanguageComboBox.h : header file
//

#include "..\shared\OwnerdrawComboBoxBase.h"
#include "..\shared\EnImageList.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLLanguageComboBox window

class CTDLLanguageComboBox : public COwnerdrawComboBoxBase
{
// Construction
public:
	CTDLLanguageComboBox(LPCTSTR szFilter = _T("*.csv"));
	virtual ~CTDLLanguageComboBox();

	void SelectLanguageFile(LPCTSTR szFile);
	int SelectUserLanguage();

	CString GetSelectedLanguageFile(BOOL bRelative = FALSE) const;
	LANGID GetSelectedPrimaryLanguageID() const;
	BOOL IsDefaultLanguageSelected() const;

	static CString GetDefaultLanguage();
	static BOOL HasLanguages();
	
protected:
	CString m_sFilter;
	CEnImageList m_il;

	int m_nLangCountryColWidth;

	mutable CString m_sSelLanguage;

protected:
	// Generated message map functions
	afx_msg void OnDestroy();
	afx_msg BOOL OnDropDown();
	DECLARE_MESSAGE_MAP()

protected:
	struct LCB_ITEMDATA : public ODCB_ITEMDATA
	{
		CString sCompletion;
	};

	virtual void BuildCombo();
	virtual ODCB_ITEMDATA* NewExtItemData() const { return new LCB_ITEMDATA(); }
	virtual int CalcMinItemHeight(BOOL bList) const;
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
							  DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);
protected:
	BOOL AddDefaultLanguage();
	int AddString(LPCTSTR szLanguage, LANGID nLangID, const CString& sCompletion, HBITMAP hbmFlag, COLORREF crBack = CLR_NONE);
	int SelectLanguage(LPCTSTR szLanguage);
	void InitialiseDropWidth();

	static CString GetSelectedLanguageFile(LPCTSTR szLanguage, LPCTSTR szExt = _T("csv"), BOOL bRelative = FALSE);
	static CString GetTranslationFolder();
	static LANGID GetLanguageID(const CString& sTransFile);
	static CString GetPercentTranslated(const CString& sTransFile);
	static void GetLanguageAndCountry(const CString& sItem, CString& sLanguage, CString& sCountry);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLLANGUAGECOMBOBOX_H__5EEB7D2C_19CF_460E_B188_646AB514678E__INCLUDED_)
