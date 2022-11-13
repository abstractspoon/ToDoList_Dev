#if !defined(AFX_RICHEDITBASECTRL_H__E7F84BEA_24A6_42D4_BE92_4B8891484048__INCLUDED_)
#define AFX_RICHEDITBASECTRL_H__E7F84BEA_24A6_42D4_BE92_4B8891484048__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "findreplace.h"
#include "tooltipctrlex.h"
#include "popuplistboxctrl.h"

#include <richole.h>

/////////////////////////////////////////////////////////////////////////////

// Override afxwin.h definitions

#undef TEXTRANGE

#ifdef _UNICODE
#	define TEXTRANGE	TEXTRANGEW
#else
#	define TEXTRANGE	TEXTRANGEA
#endif

#undef FINDTEXTEX

#ifdef _UNICODE
#	define FINDTEXTEX	FINDTEXTEXW
#else
#	define FINDTEXTEX	FINDTEXTEXA
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef IMF_SPELLCHECKING
#	define IMF_SPELLCHECKING 0x0800
#endif
 
#ifndef EM_SETEDITSTYLE
#	define EM_SETEDITSTYLE			(WM_USER + 204)
#	define EM_GETEDITSTYLE			(WM_USER + 205)
#endif

// Extended edit style masks 
enum RECB_EDITSTYLE
{
	RECBES_NOFOCUSLINKNOTIFY = 0x00000020,
	RECBES_USECTF			 = 0x00010000,
	RECBES_CTFALLOWEMBED	 = 0x00200000,
	RECBES_CTFALLOWSMARTTAG	 = 0x00400000,
	RECBES_CTFALLOWPROOFING	 = 0x00800000,
};

/////////////////////////////////////////////////////////////////////////////

#ifndef IMF_AUTOFONT
#	define IMF_AUTOFONT				0x00000002
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef CP_UNICODE
#	define CP_UNICODE 1200
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef EM_SETTEXTEX
#	define EM_SETTEXTEX (WM_USER + 97)
#	define ST_DEFAULT 0
#	define ST_KEEPUNDO 1
#	define ST_SELECTION 2

	struct SETTEXTEX
	{
		DWORD flags; 
		UINT  codepage; 
	};
#endif

/////////////////////////////////////////////////////////////////////////////

enum REBC_BORDERS
{
	REBCB_NONE		= 0x00,
	REBCB_TOP		= 0x01,
	REBCB_LEFT		= 0x02,
	REBCB_BOTTOM	= 0x04,
	REBCB_RIGHT		= 0x08,
	REBCB_ALL		= 0x0f
};

/////////////////////////////////////////////////////////////////////////////

enum REBC_URL
{
	REBCU_ENABLEURL			 = 0x01,
	REBCU_ENABLEEMAILADDR	 = 0x02,
	REBCU_ENABLETELNO		 = 0x04,
	REBCU_ENABLEEAURLS		 = 0x08,
	REBCU_ENABLEDRIVELETTERS = 0x10,
	REBCU_DISABLEMIXEDLGC	 = 0x20,
};

/////////////////////////////////////////////////////////////////////////////

const UINT WM_REB_NOTIFYSELECTPOPUPLISTITEM = ::RegisterWindowMessage(_T("WM_REB_NOTIFYSELECTPOPUPLISTITEM")); // wParam = DlgCtrlID, lParam = LPCTSTR

/////////////////////////////////////////////////////////////////////////////
// CRichEditBaseCtrl window

class CRichEditBaseCtrl : public CRichEditCtrl, protected IFindReplaceCmdHandler
{
// Construction
public:
	CRichEditBaseCtrl(BOOL bAutoRTL = TRUE);
	virtual ~CRichEditBaseCtrl();

	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwExStyle = 0);
	void TemporarilyDisableChangeNotifications();

	BOOL Save(const CString& filename);
	BOOL Load(const CString& filename);
	
	BOOL GetRTF(CString& sRTF) const; // ansi encoded string 
	void SetRTF(const CString& rtf);

	// Size of buffer must be >= GetRTFLength()
	int GetRTF(unsigned char* pRTF, int nRTFLen) const; // ansi encoded string 
	int	GetRTFLength() const; // in bytes

	void DoEditFind(UINT nIDTitle = 0);
	void DoEditReplace(UINT nIDTitle = 0);
	int ReplaceAll(LPCTSTR szFind, LPCTSTR szReplace, bool bCaseSensitive, bool bWholeWord);

	BOOL Undo();
	BOOL Redo();

	CString GetTextRange(const CHARRANGE& cr) const;
	CString GetSelText() const;
	BOOL SetTextEx(const CString& sText, DWORD dwFlags = ST_KEEPUNDO | ST_SELECTION, UINT nCodePage = CP_ACP); 
	int GetSelStart() const;
	int GetSelEnd() const;
	void CollapseSelection(BOOL bStart);
	void SelectCharacterAtCaret(BOOL bForwards);
	CString GetCharacterAtCaret(BOOL bForwards) const;

	void EnableSelectOnFocus(BOOL bEnable) { m_bEnableSelectOnFocus = bEnable; }
	BOOL CanEdit() const;

	BOOL PasteSpecial(CLIPFORMAT nFormat); // EM_PASTESPECIAL
	BOOL PasteSimpleText();
	BOOL InsertSimpleText(LPCWSTR szText, bool bAtEnd);
	BOOL CanPasteSimpleText() const;
	BOOL CopySimpleText();
	BOOL CanCopySelectedText() const;
	BOOL CutSimpleText();
	BOOL CanCutSelectedText() const;

	void GetMargins(CRect& rMargins) const { rMargins = m_rMargins; }
	void SetMargins(LPCRECT pMargins);
	void SetMargins(int nLeft, int nTop, int nRight, int nBottom);
	void SetMargins(int nMargin);
	
	BOOL InsertTable(int nRows, int nCols, int nColWidth = 1000, int nTextIndent = 50, DWORD dwBorders = REBCB_ALL);
	BOOL InsertHorizontalLine();

	BOOL SetParaFormat(PARAFORMAT2 &pf);
	BOOL SetParaFormat(PARAFORMAT &pf);
	DWORD GetParaFormat(PARAFORMAT2 &pf) const;
	DWORD GetParaFormat(PARAFORMAT &pf) const;

	DWORD GetSelectionCharFormat(CHARFORMAT2 &cf) const;
	DWORD GetSelectionCharFormat(CHARFORMAT &cf) const;
	BOOL SetSelectionCharFormat(CHARFORMAT2 &cf);
	BOOL SetSelectionCharFormat(CHARFORMAT &cf);

	BOOL SelectionHasEffect(DWORD dwMask, DWORD dwEffect) const;
	BOOL SetSelectedEffect(DWORD dwMask, DWORD dwEffect);
	BOOL SetSelectedEffect(const CHARFORMAT2& cf);
	BOOL HasSelection() const;

	int CharFromPoint(const CPoint& point) const; 
	void PointFromChar(int nCharPos, CPoint& point) const; 
	int GetLineHeight() const;
	void SetFirstVisibleLine(int nLine);
	CPoint GetCaretPos() const;
	void MoveCaretToEnd();
	void SetCaretPos(int nCharPos);

	BOOL IsStartOfLine(int nCharPos) const;
	BOOL IsEndOfLine(int nCharPos) const;

	BOOL EnableToolTips(BOOL bEnable = TRUE);
	void FilterToolTipMessage(MSG* pMsg); // for MFC non-extension Dlls

	// Windows 8.1 and up only
	BOOL EnableAutoFontChanging(BOOL bEnable = TRUE);
	BOOL EnableInlineSpellChecking(BOOL bEnable = TRUE);
	BOOL IsInlineSpellCheckingEnabled() const;
	static BOOL SupportsInlineSpellChecking();

	void SetParaAlignment(int alignment);
	BOOL GetParaAlignment() const;
	BOOL SetSelectedWebLink(const CString& sWebLink, const CString& sText);

	BOOL EnableAutoUrlDetection(DWORD dwFlags = REBCU_ENABLEURL);
	BOOL EnableAutoUrlDetection(const CStringArray& aProtocols, DWORD dwFlags = REBCU_ENABLEURL);
	BOOL IsAutoUrlDetectionEnabled() const;

	BOOL ShowPopupListBoxAtCaret(const CStringArray& aListItems, CWnd* pNotify = NULL, UINT nID = 0xffff);

	// Attributes
protected:
	BOOL m_bEnableSelectOnFocus;
	BOOL m_bInOnFocus;
	BOOL m_bAutoRTL;

	CRect m_rMargins;
	FIND_STATE m_findState;

	CToolTipCtrlEx m_tooltip;
	CPopupListBoxCtrl m_lbPopupList;
	CWnd* m_pPopupListOwner;
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichEditBaseCtrl)
protected:
	virtual void PreSubclassWindow();
	virtual int OnToolHitTest(CPoint pt, TOOLINFO* pTI) const;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	class CRichEditOleCallback : public IRichEditOleCallback
	{
	public:
		CRichEditOleCallback();
		virtual ~CRichEditOleCallback();
		
		void SetOwner(CRichEditBaseCtrl* pOwner) { m_pOwner = pOwner; }
		
		// IRichEditOleCallback
		virtual HRESULT STDMETHODCALLTYPE GetNewStorage(LPSTORAGE* lplpstg);
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject);
		virtual ULONG   STDMETHODCALLTYPE AddRef();
		virtual ULONG   STDMETHODCALLTYPE Release();
		virtual HRESULT STDMETHODCALLTYPE GetInPlaceContext(LPOLEINPLACEFRAME FAR *lplpFrame,
			LPOLEINPLACEUIWINDOW FAR *lplpDoc, LPOLEINPLACEFRAMEINFO lpFrameInfo);
		virtual HRESULT STDMETHODCALLTYPE ShowContainerUI(BOOL fShow);
		virtual HRESULT STDMETHODCALLTYPE QueryInsertObject(LPCLSID lpclsid, LPSTORAGE lpstg, LONG cp);
		virtual HRESULT STDMETHODCALLTYPE DeleteObject(LPOLEOBJECT lpoleobj);
		virtual HRESULT STDMETHODCALLTYPE QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT FAR *lpcfFormat,
            DWORD reco, BOOL fReally, HGLOBAL hMetaPict);
		virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode);
		virtual HRESULT STDMETHODCALLTYPE GetClipboardData(CHARRANGE FAR *lpchrg, DWORD reco, LPDATAOBJECT FAR *lplpdataobj);
		virtual HRESULT STDMETHODCALLTYPE GetDragDropEffect(BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect);
		virtual HRESULT STDMETHODCALLTYPE GetContextMenu(WORD seltyp, LPOLEOBJECT lpoleobj, CHARRANGE FAR *lpchrg,
            HMENU FAR *lphmenu);
		
	protected:
		int m_iNumStorages;
		IStorage* m_pStorage;
		DWORD m_dwRef;
		CRichEditBaseCtrl* m_pOwner;
	};
	
	friend class CRichEditOleCallback;
	
protected:
	CRichEditOleCallback m_callback;
	
	// Generated message map functions
protected:
	virtual HRESULT GetNewStorage(LPSTORAGE* /*lplpstg*/) { return S_OK; }
	virtual HRESULT GetInPlaceContext(LPOLEINPLACEFRAME FAR* /*lplpFrame*/,
		LPOLEINPLACEUIWINDOW FAR* /*lplpDoc*/, LPOLEINPLACEFRAMEINFO /*lpFrameInfo*/) { return S_FALSE; }
	virtual HRESULT ShowContainerUI(BOOL /*fShow*/) { return S_FALSE; }
	virtual HRESULT QueryInsertObject(LPCLSID /*lpclsid*/, LPSTORAGE /*lpstg*/, LONG /*cp*/) { return S_OK; }
	virtual HRESULT DeleteObject(LPOLEOBJECT /*lpoleobj*/) { return S_OK; }
	virtual HRESULT QueryAcceptData(LPDATAOBJECT /*lpdataobj*/, CLIPFORMAT FAR* /*lpcfFormat*/,
		DWORD /*reco*/, BOOL /*fReally*/, HGLOBAL /*hMetaPict*/) { return S_OK; }
	virtual HRESULT ContextSensitiveHelp(BOOL /*fEnterMode*/) { return S_OK; }
	virtual HRESULT GetClipboardData(CHARRANGE FAR* /*lpchrg*/, DWORD /*reco*/, LPDATAOBJECT FAR* /*lplpdataobj*/) { return E_NOTIMPL; }
	virtual HRESULT GetDragDropEffect(BOOL /*fDrag*/, DWORD /*grfKeyState*/, LPDWORD /*pdwEffect*/) { return S_OK; }
	virtual HRESULT GetContextMenu(WORD /*seltyp*/, LPOLEOBJECT /*lpoleobj*/, CHARRANGE FAR* /*lpchrg*/,
		HMENU FAR* /*lphmenu*/) { return S_OK; }
	
	virtual void OnFindNext(const CString& sFind, BOOL bNext, BOOL bCase, BOOL bWord);
	virtual void OnReplaceSel(const CString& sFind, const CString& sReplace, 
								BOOL bNext, BOOL bCase,	BOOL bWord);
	virtual void OnReplaceAll(const CString& sFind, const CString& sReplace,
								BOOL bCase, BOOL bWord);
	virtual void OnSelectPopupListItem(const CString& /*sSelItem*/) {}
	virtual void Initialise();
	
	//{{AFX_MSG(CRichEditBaseCtrl)
	//}}AFX_MSG
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);

	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFindReplaceMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditSetSelection(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnToolHitTest(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnReenableChangeNotifications(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDropListEndEdit(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnDropListCancelEdit(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()
		
	void AdjustFindDialogPosition();
	BOOL FindText(LPCTSTR lpszFind, BOOL bNext = TRUE, BOOL bCase = TRUE, BOOL bWord = TRUE, BOOL bWrap = TRUE);
	void TextNotFound(LPCTSTR lpszFind);
	BOOL FindText(BOOL bWrap = TRUE);
	long FindAndSelect(DWORD dwFlags, FINDTEXTEX& ft);
	void DoEditFindReplace(BOOL bFindOnly, UINT nIDTitle);
	BOOL SameAsSelected(LPCTSTR lpszCompare, BOOL bCase, BOOL bWord);
	BOOL IsFindDialog(HWND hwnd) const;
	BOOL InsertSoftReturn();
	void GetCharacterAtCaret(CHARRANGE& cr, BOOL bForwards) const;

	BOOL EnableLanguageOptions(DWORD dwOption, BOOL bEnable);
	BOOL EnableEditStyles(DWORD dwStyles, BOOL bEnable);
	BOOL EnableChangeNotifications(BOOL bEnable);
	BOOL HasChangeNotifications() const;

	static BOOL EnableStateFlags(HWND hWnd, UINT nGetMsg, UINT nSetMsg, DWORD dwFlags, BOOL bEnable);

	/////////////////////////////////////////////////////////////////////////////
	// Stream callback functions
	// Callbacks to the Save and Load functions.
	static DWORD CALLBACK StreamInCB(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
	static DWORD CALLBACK StreamOutCB(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
	static DWORD CALLBACK StreamOutLenCB(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);

	static CLIPFORMAT GetAcceptableClipFormat(LPDATAOBJECT lpDataOb, CLIPFORMAT format, 
							const CLIPFORMAT fmtPreferred[], int nNumFmts, BOOL bAllowFallback);
	
};

/////////////////////////////////////////////////////////////////////////////

inline BOOL operator==(const CHARRANGE& cr1, const CHARRANGE& cr2)
{
	return ((cr1.cpMin == cr2.cpMin) && (cr1.cpMax == cr2.cpMax));
}

inline BOOL operator!=(const CHARRANGE& cr1, const CHARRANGE& cr2)
{
	return !(cr1 == cr2);
}

inline BOOL ContainsPos(const CHARRANGE& cr, int nPos)
{
	return ((nPos >= cr.cpMin) && (nPos <= cr.cpMax));
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICHEDITBASECTRL_H__E7F84BEA_24A6_42D4_BE92_4B8891484048__INCLUDED_)
