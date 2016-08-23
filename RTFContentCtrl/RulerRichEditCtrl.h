#if !defined(AFX_RULERRICHEDITCTRL_H__4CD13283_82E4_484A_83B4_DBAD5B64F17C__INCLUDED_)
#define AFX_RULERRICHEDITCTRL_H__4CD13283_82E4_484A_83B4_DBAD5B64F17C__INCLUDED_

#include "RulerRichEdit.h"
#include "RRECRuler.h"
#include "RRECToolbar.h"
#include "ids.h"

/////////////////////////////////////////////////////////////////////////////
// CRulerRichEditCtrl window

class CRulerRichEditCtrl : public CWnd
{

public:
// Construction/creation/destruction
	CRulerRichEditCtrl();
	virtual ~CRulerRichEditCtrl();
	virtual BOOL Create(DWORD dwStyle, const RECT &rect, CWnd* pParentWnd, UINT nID, BOOL autohscroll = FALSE);

// Attributes
	void	SetMode(int mode);
	int		GetMode() const;

	void ShowToolbar(BOOL show = TRUE);
	void ShowRuler(BOOL show = TRUE);

	BOOL IsToolbarVisible() const;
	BOOL IsRulerVisible() const;

	void SetWordWrap(BOOL bWrap = TRUE);
	BOOL HasWordWrap() const { return m_bWordWrap; }

	void SetRTF(const CString& rtf);
	void SetSelectedWebLink(const CString& sWebLink);

	void SetFileLinkOption(RE_PASTE nLinkOption, BOOL bDefault) { m_rtf.SetFileLinkOption(nLinkOption, bDefault); }
	RE_PASTE GetFileLinkOption() const { return m_rtf.GetFileLinkOption(); }
	BOOL IsFileLinkOptionDefault() const { return m_rtf.IsFileLinkOptionDefault(); }

	void SetReadOnly(BOOL readOnly);
	BOOL GetReadOnly() const;

protected:
// Formatting
	void DoFont();
	void DoColor();
	void DoBold();
	void DoItalic();
	void DoUnderline();
	void DoLeftAlign();
	void DoCenterAlign();
	void DoRightAlign();
	void DoJustify();
	void DoIndent();
	void DoOutdent();
	void DoBulletList();
	void DoNumberList();
	void DoStrikethrough();
	void DoSuperscript();
	void DoSubscript();
	void DoInsertTable();
	void DoInsertHorzLine();

	void IncrementFontSize(BOOL bIncrement = TRUE);
	void SetCurrentFontName(const CString& font);
	void SetCurrentFontSize(int points);
	void SetCurrentFontColor(COLORREF color, BOOL bForeground);

	//void InsertHorizontalRule();

// Overrides
	//{{AFX_VIRTUAL(CRulerRichEditCtrl)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
// Message handlers
	//{{AFX_MSG(CRulerRichEditCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnSetText (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetText (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetTextLength (WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG
	afx_msg void OnKillFocusToolbar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSetFont(WPARAM wp, LPARAM lp);
	afx_msg void OnEnHScroll();
	afx_msg void OnEnSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnThemeChanged(WPARAM wp, LPARAM lp);

	afx_msg void OnEditBackColor();
	afx_msg void OnEditBold();
	afx_msg void OnEditBulletList();
	afx_msg void OnEditCenterAlign();
	afx_msg void OnEditColor();
	afx_msg void OnEditFont();
	afx_msg void OnEditGrowFont();
	afx_msg void OnEditIndent();
	afx_msg void OnEditInsertTable();
	afx_msg void OnEditItalic();
	afx_msg void OnEditJustify();
	afx_msg void OnEditLeftAlign();
	afx_msg void OnEditNumberList();
	afx_msg void OnEditOutdent();
	afx_msg void OnEditRightAlign();
	afx_msg void OnEditShrinkFont();
	afx_msg void OnEditStrikethrough();
	afx_msg void OnEditTextColor();
	afx_msg void OnEditUnderline();
	afx_msg void OnEditWordwrap();
	afx_msg void OnUpdateEditBackColor(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditBold(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditBullet(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCenterAlign(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditFont(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditGrowFont(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditIndent(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditInsertTable(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditItalic(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditJustify(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditLeftAlign(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditNumberList(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditOutdent(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRightAlign(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditShrinkFont(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditStrikethru(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditTextColor(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUnderline(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditWordwrap(CCmdUI* pCmdUI);


	LRESULT OnTrackRuler(WPARAM mode, LPARAM pt);
	LRESULT OnGetScrollPos(WPARAM, LPARAM);
	LRESULT OnSetCurrentFontName(WPARAM font, LPARAM size);
	LRESULT OnSetCurrentFontSize(WPARAM font, LPARAM size);
	LRESULT OnSetCurrentFontColor(WPARAM font, LPARAM size);

	DECLARE_MESSAGE_MAP()

protected:
	// Internal data
	int				m_rulerPosition;	// The x-position of the ruler line when dragging a tab
	CPen			m_pen;				// The pen to use for drawing the XORed ruler line

	CDWordArray		m_tabs;				// An array containing the tab-positions in device pixels
	int				m_margin;			// The margin to use for the ruler and buttons

	int				m_physicalInch;		// The number of pixels for an inch on screen
	int				m_movingtab;		// The tab-position being moved, or -1 if none
	int				m_offset;			// Internal offset of the tab-marker being moved.
	CharFormat		m_cfDefault;
	CHARRANGE		m_crSel;

	BOOL			m_showToolbar;
	BOOL			m_showRuler;
	BOOL			m_readOnly;
	BOOL			m_bWordWrap;
	
	// Sub-controls
	CRulerRichEdit	m_rtf;
	CRRECToolBar	m_toolbar;
	CRRECRuler		m_ruler;

	// Handle to the RTF 2.0 dll
//	HINSTANCE		m_richEditModule;

	// Private helpers
	void SetTabStops(LPLONG tabs, int size);
	void UpdateTabStops();
	BOOL FixupTabStops(ParaFormat& para);
	void BuildTabStops(ParaFormat& para);

	BOOL CreateToolbar();
	BOOL CreateRuler();
	BOOL CreateRTFControl(BOOL autohscroll);
	void CreateMargins();

	void UpdateToolbarButtons();
	void UpdateEditRect();

	void SetEffect(int mask, int effect);
	void SetParaAlignment(int alignment);
	BOOL GetParaAlignment() const;

	void LayoutControls(int width, int height);

};

#endif // !defined(AFX_RULERRICHEDITCTRL_H__4CD13283_82E4_484A_83B4_DBAD5B64F17C__INCLUDED_)
