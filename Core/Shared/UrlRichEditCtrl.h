#if !defined(AFX_URLRICHEDITCTRL_H__B5421D69_41F2_4FCF_AC58_13D2B3D3D3C8__INCLUDED_)
#define AFX_URLRICHEDITCTRL_H__B5421D69_41F2_4FCF_AC58_13D2B3D3D3C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// urlricheditctrl.h : header file
//

#include "richeditbasectrl.h"
#include "richeditncborder.h"
#include "UrlParser.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

const UINT WM_UREN_CUSTOMURL  = ::RegisterWindowMessage(_T("WM_UREN_CUSTOMURL"));  // lParam == full url
const UINT WM_UREN_FAILEDURL  = ::RegisterWindowMessage(_T("WM_UREN_FAILEDURL"));  // lParam == full url

/////////////////////////////////////////////////////////////////////////////

enum URE_LINKHANDLING
{
	CTRLCLICKTOFOLLOW,
	SHIFTCLICKTOEDIT,
};

/////////////////////////////////////////////////////////////////////////////
// CUrlRichEditCtrl window
class CUrlRichEditCtrl : public CRichEditBaseCtrl
{
	// Construction
public:
	CUrlRichEditCtrl(URE_LINKHANDLING nLinkHandling = CTRLCLICKTOFOLLOW, UINT nIDLinkInstructionMsg = 0);
	
	void PathReplaceSel(LPCTSTR lpszPath, BOOL bFile);
	BOOL GoToUrl(const CString& sUrl) const;
	CPoint GetContextMenuPos() { return m_ptContextMenu; }
	int AddProtocol(LPCTSTR szProtocol, BOOL bWantNotify = TRUE);
	void ParseAndFormatText(BOOL bForceReformat = FALSE);
	CString GetContextUrl(BOOL bAsFile = FALSE) const;
	void Paste(BOOL bAppendSourceUrl);
	BOOL PasteSimpleText(BOOL bAppendSourceUrl);

	// Attributes
protected:
	CUrlArray m_aUrls;
	CUrlParser m_parser;
	CRichEditNcBorder m_ncBorder;

	CPoint m_ptContextMenu;
	CString m_sContextUrl;
	CString m_sLinkInstruction;
	CHARRANGE m_crDropSel;
	LPDATAOBJECT m_lpDragObject;
	URE_LINKHANDLING m_nLinkHandling;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUrlRichEditCtrl)
protected:
	virtual int OnToolHitTest(CPoint pt, TOOLINFO* pTI) const;
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
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
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
	CString FindUrl(const CPoint& point) const;
	CString FindUrl(int nPos) const;
	void TrackDragCursor();
	BOOL AppendSourceUrls(LPCTSTR szUrl);
	BOOL GetUrlTooltip(const CString& sUrl, CString& sTooltip) const;
	void Initialise();
	BOOL FindStartOfUrl(LPCTSTR szText, int nTextLen, LPCTSTR& szPos) const;
	BOOL WantFollowLink(BOOL bCtrl, BOOL bShift) const;
	BOOL SelectionContainsMessagePos() const;
	BOOL SelectionContainsPos(const CPoint& ptClient) const;
	BOOL UrlsMatch(const CUrlArray& aUrls) const;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_URLRICHEDITCTRL_H__B5421D69_41F2_4FCF_AC58_13D2B3D3D3C8__INCLUDED_)
