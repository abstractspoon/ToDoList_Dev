#if !defined(AFX_RULERRICHEDIT_H__E10A8ED3_2E1D_402E_A599_003214085F1A__INCLUDED_)
#define AFX_RULERRICHEDIT_H__E10A8ED3_2E1D_402E_A599_003214085F1A__INCLUDED_

// RulerRichEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRulerRichEdit window

#include "..\shared\urlricheditctrl.h"
#include "..\shared\richedithelper.h"

/////////////////////////////////////////////////////////////////////////////

class CRtfHtmlConverter;

/////////////////////////////////////////////////////////////////////////////

class CRulerRichEdit : public CUrlRichEditCtrl
{
public:
// Construction/destruction
	CRulerRichEdit(CRtfHtmlConverter& rtfHtml);
	virtual ~CRulerRichEdit();

	BOOL IsIMEComposing() const { return m_bIMEComposing; }
	BOOL IsRTF(const char* szRTF) const;

	void SetFileLinkOption(RE_PASTE nLinkOption, BOOL bDefault, BOOL bReduceImageColors);
	RE_PASTE GetFileLinkOption() const { return m_nFileLinkOption; }
	BOOL IsFileLinkOptionDefault() const { return m_bLinkOptionIsDefault; }
	BOOL GetReduceImageColors() const { return m_bReduceImageColors; }

	BOOL PasteFiles(const CStringArray& aFiles);
	BOOL PasteSimple();
	BOOL Paste();
	BOOL CanPaste() const;
	BOOL CopyToClipboardAsHtml();
	BOOL Cut();

	static void SetPasteSourceUrls(BOOL bPasteUrls) { s_bPasteSourceUrls = bPasteUrls; }
	static void SetConvertWithMSWord(BOOL bUseMSWord) { s_bConvertWithMSWord = bUseMSWord; }
	static BOOL GetConvertWithMSWord() { return s_bConvertWithMSWord; }

protected:
	BOOL m_bIMEComposing;
	RE_PASTE m_nFileLinkOption;
	BOOL m_bLinkOptionIsDefault;
	BOOL m_bReduceImageColors;
	CRtfHtmlConverter& m_rtfHtml;

	static BOOL s_bPasteSourceUrls;
	static BOOL s_bConvertWithMSWord;

protected:
// Message handlers
	//{{AFX_MSG(CRulerRichEdit)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnDropFiles(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnIMEStartComposition(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnIMEEndComposition(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

	virtual HRESULT QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT FAR *lpcfFormat,
									DWORD reco, BOOL fReally, HGLOBAL hMetaPict);
	virtual CLIPFORMAT GetAcceptableClipFormat(LPDATAOBJECT lpDataOb, CLIPFORMAT format);
	virtual HRESULT GetDragDropEffect(BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect);
	virtual HRESULT GetContextMenu(WORD seltyp, LPOLEOBJECT lpoleobj, CHARRANGE FAR* lpchrg,
									HMENU FAR* lphmenu);
	
	virtual CFindReplaceDialog* NewFindReplaceDlg();

	BOOL AppendSourceUrls(LPCTSTR szUrls);
	BOOL CopyRtfToClipboardAsHtml(const CString& sRTF, BOOL bAppend = TRUE);

	static BOOL GetClipboardHtmlForPasting(CString& sHtml, CString& sSourceUrl);
	static BOOL ProcessHtmlForPasting(CString& sHtml, CString& sSourceUrl);

};

#endif // !defined(AFX_RULERRICHEDIT_H__E10A8ED3_2E1D_402E_A599_003214085F1A__INCLUDED_)
