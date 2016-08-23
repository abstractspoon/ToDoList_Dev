/////////////////////////////////////////////////////////////////////////////
// CComboBox
#if !defined ( FONTCOMBOBOX_H )
#define FONTCOMBOBOX_H 

class CFontComboBox : public CComboBox
{
public:
	CFontComboBox();

	CString GetSelectedFont();
	void SetSelectedFont(LPCTSTR lpszFaceName, BOOL bMatchExact = FALSE);

//Attributes
protected:
	CPtrArray m_arrayFontDesc;
	static int m_nFontHeight;
	int m_nLimitText;

//Overridables
protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCIS);
	virtual void PreSubclassWindow();
	// Generated message map functions
	//{{AFX_MSG(CFontComboBox)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	inline BOOL HasFocus()
	{
		HWND hWnd = ::GetFocus();
		return (hWnd == m_hWnd || ::IsChild(m_hWnd, hWnd));
	}

	void EnumFontFamiliesEx(CDC* pDC = NULL, BYTE nCharSet = DEFAULT_CHARSET);
	void AddFont(ENUMLOGFONT* pelf, DWORD dwType, LPCTSTR lpszScript = NULL);
	void MatchFont(LPCTSTR lpszName, BYTE nCharSet);
	void EmptyContents();

	static BOOL CALLBACK AFX_EXPORT EnumFamScreenCallBack(ENUMLOGFONT* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType, LPVOID pThis);
	static BOOL CALLBACK AFX_EXPORT EnumFamPrinterCallBack(ENUMLOGFONT* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType, LPVOID pThis);
	static BOOL CALLBACK AFX_EXPORT EnumFamScreenCallBackEx(ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType, LPVOID pThis);
	static BOOL CALLBACK AFX_EXPORT EnumFamPrinterCallBackEx(ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType, LPVOID pThis);
};

#endif
