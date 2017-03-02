#if !defined(AFX_URLRICHEDITCTRL_H__B5421D69_41F2_4FCF_AC58_13D2B3D3D3C8__INCLUDED_)
#define AFX_URLRICHEDITCTRL_H__B5421D69_41F2_4FCF_AC58_13D2B3D3D3C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// urlricheditctrl.h : header file
//

#include "richeditbasectrl.h"
#include "richeditncborder.h"

/////////////////////////////////////////////////////////////////////////////
// CUrlRichEditCtrl window

const UINT WM_UREN_CUSTOMURL = ::RegisterWindowMessage(_T("WM_UREN_CUSTOMURL")); // lParam == full url
const UINT WM_UREN_FAILEDURL = ::RegisterWindowMessage(_T("WM_UREN_FAILEDURL")); // lParam == full url

struct URLITEM
{
	CHARRANGE cr;
	CString sUrl;
	BOOL bWantNotify;
};

struct PROTOCOL
{
   PROTOCOL(LPCTSTR szProtocol = NULL, BOOL bNotify = FALSE) : sProtocol(szProtocol), bWantNotify(bNotify) {}

	CString sProtocol;
	BOOL bWantNotify;
};

#include <afxtempl.h>

typedef CArray<URLITEM, URLITEM&> CUrlArray;
typedef CArray<PROTOCOL, PROTOCOL&> CProtocolArray;

class CUrlRichEditCtrl : public CRichEditBaseCtrl
{
	// Construction
public:
	CUrlRichEditCtrl();
	
	int GetUrlCount() const { return m_aUrls.GetSize(); }
	CString GetUrl(int nURL, BOOL bAsFile = FALSE) const;
	void PathReplaceSel(LPCTSTR lpszPath, BOOL bFile);
	BOOL GoToUrl(int nUrl) const;
	BOOL CopyUrlToClipboard(int nUrl) const;
	CPoint GetContextMenuPos() { return m_ptContextMenu; }
	int AddProtocol(LPCTSTR szProtocol, BOOL bWantNotify = TRUE);
	void ParseAndFormatText(BOOL bForceReformat = FALSE);
	int GetContextUrl() { return m_nContextUrl; }
	void Paste(BOOL bAppendSourceUrl, CLIPFORMAT nFormat = 0);
	BOOL PasteSimpleText(BOOL bAppendSourceUrl);

	// Attributes
protected:
	CUrlArray m_aUrls;
	CProtocolArray m_aProtocols;
	CRichEditNcBorder m_ncBorder;

	CPoint m_ptContextMenu;
	int m_nContextUrl;
	CHARRANGE m_crDropSel;
	LPDATAOBJECT m_lpDragObject;
	int m_nFileProtocol, m_nFileProtocol2;

	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUrlRichEditCtrl)
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	virtual LRESULT SendNotifyCustomUrl(LPCTSTR szUrl) const;
	virtual LRESULT SendNotifyFailedUrl(LPCTSTR szUrl) const;
	
	// Implementation
public:
	virtual ~CUrlRichEditCtrl();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CUrlRichEditCtrl)
	afx_msg BOOL OnChangeText();
	afx_msg void OnRButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnSetFont(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSetText(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnDropFiles(WPARAM wp, LPARAM lp);
	afx_msg BOOL OnNotifyLink(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
	
protected:
	virtual HRESULT QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT FAR *lpcfFormat,
		DWORD reco, BOOL fReally, HGLOBAL hMetaPict);
	virtual HRESULT GetDragDropEffect(BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect);
	virtual HRESULT GetContextMenu(WORD seltyp, LPOLEOBJECT lpoleobj, CHARRANGE FAR *lpchrg,
		HMENU FAR *lphmenu);
	virtual CLIPFORMAT GetAcceptableClipFormat(LPDATAOBJECT lpDataOb, CLIPFORMAT format);
	
protected:
	int FindUrl(const CHARRANGE& cr) const;
	int FindUrl(const CPoint& point) const;
	BOOL UrlsMatch(const CUrlArray& aUrls); 
	void TrackDragCursor();
	int MatchProtocol(LPCTSTR szText) const;
	BOOL AppendSourceUrls(LPCTSTR szUrl);
	BOOL IsFileProtocol(int nProtocol) const;
	
	static BOOL FindEndOfUrl(LPCTSTR& szPos, int& nUrlLen, BOOL bBraced, BOOL bFile);
	static BOOL IsBaseDelim(LPCTSTR szText);
	static void InsertInOrder(URLITEM& urli, CUrlArray& aUrls);
	static CString CreateFileLink(LPCTSTR szFile);
	static void AppendURLsToLinkText(CString& sLinkText, const CString& sURLs);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_URLRICHEDITCTRL_H__B5421D69_41F2_4FCF_AC58_13D2B3D3D3C8__INCLUDED_)
